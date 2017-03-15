#include "KnowledgeBase.hpp"
#include "Utility.hpp"

#include <iostream>
#include <future>

using namespace std;
using namespace utility;

// ==================================================================================
// ADD FACT
//     -Adds a fact to the KnowledgeBase.
// ==================================================================================

void KnowledgeBase::AddFact (string name, vector<string> params)
{
    auto factEntry = facts.find (name);
    
    if ( factEntry == facts.end() )
    {
        facts[name] = vector<vector<string>> {params};
    }
    else
    {
        facts[name].push_back(params);
    }
}

// ==================================================================================
// DROP FACT
//     -Removes a fact from the KnowledgeBase.
// ==================================================================================

void KnowledgeBase::DropFact (string name)
{
    facts.erase(name);
}

// ==================================================================================
// GET RESULT SET
//     -Returns all values that satisfy the parameters of a given fact.
// ==================================================================================

vector<vector<string>> KnowledgeBase::GetResultSet(string name, vector<string> params)
{
    auto result = facts.find(name);
    
    // If no name matches, return empty
    if ( result == facts.end() )
    {
        vector<vector<string>> empty;
        return empty;
    }
    
    // If name matches, get all results
    auto unfilteredResults =  result->second;
    
    vector<future<vector<string>>> futures;
    
    // For each result, start a new thread to filter the result.
    for (auto i : unfilteredResults)
    {
        futures.push_back (std::async(&KnowledgeBase::FilterResult, (this), i, name, params));
    }
    
    vector<vector<string>> finalResults;
    
    // Wait for each thread to finish and join the results.
    for(auto &e : futures)
    {
        vector<string> result = e.get();
        if (!result.empty())
            finalResults.push_back(result);
    }
    
    return finalResults;
}

// ==================================================================================
// (GET RESULT SET) : FilterResult
//     -Thread for GetResultSet to filter incorrect results.
// ==================================================================================

vector<string> KnowledgeBase::FilterResult(vector<string> result, string name, vector<string> params)
{
    vector<string> emptyResult;
    
    // If parameter count does not match, skip this result.
    if (result.size() != params.size())
        return emptyResult;
    
    unordered_map<string, string> usedParams;
    
    // For every parameter
    for (int k = 0; k < (int)params.size(); ++k)
    {
        // If the parameter is a variable...
        if (IsVariable(params[k]))
        {
            // If we have not yet used the parameter, set it as used.
            auto entry = usedParams.find(params[k]);
            if (entry == usedParams.end())
            {
                usedParams[params[k]] = result[k];
                    continue;
            }
            
            // If we have used the parameter,
            // but the result of this parameter and the previous one do not match, skip this result.
            string expectedResult = entry->second;
            
            if (result[k] != expectedResult)
            {
                return emptyResult;
            }
            
            continue;
        }
        
        // If the parameter is a constant, but doesn't match our result, skip this result.
        if (params[k] != result[k])
        {
            return emptyResult;
        }
    }
    
    return result;
}

// ==================================================================================
// EXPORT
//     -Writes all facts in KnowledgeBase to a specified file.
// ==================================================================================

string KnowledgeBase::Export ()
{
    string output;

    for (auto fact : facts)
    {
        for (auto entry : fact.second)
        {
            output += ( "FACT " + fact.first + "(" );
            for (int i=0; i < (int)entry.size(); i++)
            {
                if( i!=0 ){ output += ","; }
                output += entry[i];
            }
            output += ");";
        }
        
    }

    return output;
}


