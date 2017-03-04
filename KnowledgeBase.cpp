#include "KnowledgeBase.hpp"
#include "Utility.hpp"

#include <iostream>
#include <future>

using namespace std;
using namespace utility;

//adds a fact to the knowledge base
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

//removes a fact from the knowledge base
void KnowledgeBase::DropFact (string name)
{
    facts.erase(name);
}

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
    
    for (auto i : unfilteredResults)
    {
        futures.push_back (std::async(&KnowledgeBase::FilterResult, (this), i, name, params));
    }
    
    vector<vector<string>> finalResults;
    
    for(auto &e : futures)
    {
        vector<string> result = e.get();
        if (!result.empty())
            finalResults.push_back(result);
    }
    
    return finalResults;
}

vector<string> KnowledgeBase::FilterResult(vector<string> result, string name, vector<string> params)
{
    vector<string> emptyResult;
    
    // If parameter count does not match, skip this result
    if (result.size() != params.size())
        return emptyResult;
    
    unordered_map<string, string> usedParams;
    
    //  For every parameter
    for (int k = 0; k < (int)params.size(); ++k)
    {
        // If the parameter is variable...
        if (IsVariable(params[k]))
        {
            // If we have not yet used the parameter, set it as used
            auto entry = usedParams.find(params[k]);
            if (entry == usedParams.end())
            {
                usedParams[params[k]] = result[k];
                    continue;
            }
            
            // If we have used the parameter,
            // but the result of this parameter and the previous one do not match, skip this result
            string expectedResult = entry->second;
            
            if (result[k] != expectedResult)
            {
                return emptyResult;
            }
            
            continue;
        }
        
        // If the parameter is a constant, but doesn't match our result, skip this result
        if (params[k] != result[k])
        {
            return emptyResult;
        }
    }
    
    return result;
}

/*
//returns all the facts that satisfy the given parameters
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
    
    vector<vector<string>> finalResults;

    // For each result
    for (auto i : unfilteredResults)
    {
        // If parameter count does not match, skip this result
        if (i.size() != params.size())
            continue;
        
        bool isValid = true;
        unordered_map<string, string> usedParams;
        
        //  For every parameter
        for (int k = 0; k < (int)params.size(); ++k)
        {
            // If the parameter is variable...
            if (IsVariable(params[k]))
            {
                // If we have not yet used the parameter, set it as used
                auto entry = usedParams.find(params[k]);
                if (entry == usedParams.end())
                {
                    usedParams[params[k]] = i[k];
                    continue;
                }
                
                // If we have used the parameter,
                // but the result of this parameter and the previous one do not match, skip this result
                string expectedResult = entry->second;
                
                if (i[k] != expectedResult)
                {
                    isValid = false;
                    break;
                }
                
                continue;
            }
            
            // If the parameter is a constant, but doesn't match our result, skip this result
            if (params[k] != i[k])
            {
                isValid = false;
                break;
            }
        }
        
        // If the result has passed all tests, add it to the final results
        if (isValid)
            finalResults.push_back(i);
    }
    
    return finalResults;
}
*/
//outputs the entire knowledge base as an OS stream for the dump command; also used for debugging
void KnowledgeBase::Export(ostream& file)
{
    for (auto fact : facts)
    {
        for (auto entry : fact.second)
        {
            file << "FACT " << fact.first << "(";
            for (int i=0; i < (int)entry.size(); i++)
            {
                if( i!=0 ){ file << ","; }
                file << entry[i];
            }
            file << ")" << endl;
        }
        
    }
}


