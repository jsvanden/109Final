#include "SRI.hpp"

#include <iostream>
#include <future>

using namespace std;
using namespace utility;

// ==================================================================================
// DROP RULE
//     -Removes a rule from the RuleBase.
// ==================================================================================

void RuleBase::DropRule (string name)
{
    // Remove rule from (rule name -> rule subrules) dictionary.
    rules.erase(name);
}

// ==================================================================================
// ADD RULE
//     -Adds a rule from the RuleBase.
// ==================================================================================

void RuleBase::AddRule (string name, Subrule info)
{
    auto ruleEntry = rules.find (name);
 
    // If RuleBase does not contain the 'name' rule, create it.
    if ( ruleEntry == rules.end() )
    {
        rules[name] = vector<Subrule> {info};
        return;
    }
    // If RuleBase does contain the 'name' rule, append to it.
    else
    {
        rules[name].push_back(info);
    }
}

// ==================================================================================
// GET RESULT SET
//     -Returns all values that satisfy the parameters of a given rule.
// ==================================================================================

vector<vector<string>> RuleBase::GetResultSet(string name, vector<string> params)
{
    auto entry = rules.find(name);
    
    // If RuleBase does not contain the 'name' rule, return empty.
    if ( entry == rules.end() )
    {
        vector<vector<string>> empty;
        return empty;
    }
    
    // If RuleBase does contain the 'name' rule, get its subrules.
    vector<Subrule> subrules = entry->second;
    
    vector<vector<string>> results;
    
    // For each subrule...
    for (auto subrule : subrules)
    {
        // If the # of subrule parameters does not match the # of parameters inputed, skip this subrule.
        if (subrule.parameters.size() != params.size())
            continue;
        
        vector<vector<string>> subruleResults;
        
        // Depending on whether AND or OR was specified, call the corresponding subfunction.
        if (subrule.isAnd)
        {
            subruleResults = GetResultsAND(subrule, name, params);
        }
        else
        {
            subruleResults = GetResultsOR(subrule, name, params);
        }
        
        // Add the results of the subrule query to the total results.
        results.insert(end(results), begin(subruleResults), end(subruleResults));
    }
    
    return results;
}

// ==================================================================================
// (GET RESULT SET) : GetResultsOR
//     -Subfunction of GetResultSet called if the subrule uses an OR.
// ==================================================================================

vector<vector<string>> RuleBase::GetResultsOR(Subrule subrule, string name, vector<string> params)
{
    typedef vector<vector<string>> ClauseResults;
    ClauseResults results;

    vector<future<ClauseResults>> futures;
    
    // For each clause, launch a seperate thread getting the results of that clause.
    for (auto clause : subrule.clauses)
    {
        futures.push_back (std::async(&RuleBase::GetResultsClause, (this), clause, subrule, params));
    }
    
    // Wait for each clause thread to finish and merge the results.
    for(auto &e : futures)
    {
        ClauseResults result = e.get();
        results.insert(end(results), begin(result), end(result));
    }
    
    return results;
}


// ==================================================================================
// ((GET RESULT SET)) : (GetResultsOR) : Get Results Clause
//     -Thread for GetResultsOR to calculate an individual clause.
// ==================================================================================
vector<vector<string>> RuleBase::GetResultsClause (Clause clause, Subrule subrule, vector<string> params)
{
    vector<string> inputs;
    
    // For each clause parameter...
    for (auto i : clause.parameters)
    {
        int index = FindIndexOf(subrule.parameters, i);
        
        // If the user did not specify a value for this parameter, use it as is.
        if(index == -1)
            inputs.push_back(i);
        
        // Otherwise, use the value that the user specified.
        else
            inputs.push_back(params[index]);
    }
    
    // Recursively get the results for this individual clause.
    // Recursion continues when the clause is another rule.
    // Recursion terminates when the clause is a fact or non-existent.
    vector<vector<string>> results = engine->GetSet(clause.name, inputs);
    
    return results;
}


// ==================================================================================
// (GET RESULT SET) : GetResultsAND
//     -Subfunction of GetResultSet called if the subrule uses an AND.
// ==================================================================================

vector<vector<string>> RuleBase::GetResultsAND(Subrule subrule, string name, vector<string> params)
{
    // Start a recursive call to pipeline results into each AND clause.
    vector<ParamValueMap> resultMaps = ProcessResults(subrule, 0, ParamValueMap());
    
    // We must translate the vector of dictionary results into a vector of vector results.
    vector<vector<string>> resultVectors;
    
    // For each map...
    for (auto resultMap : resultMaps)
    {
        vector<string> newVector;
        
        // For each parameter.
        for (auto param : subrule.parameters)
        {
            auto entry = resultMap.find(param);
            
            // If the result has no value for this parameter, push 'ANY' as the value.
            if (entry == resultMap.end())
            {
                newVector.push_back("ANY");
                continue;
            }
            
            // Otherwise, push the mapped result as the value.
            newVector.push_back(entry->second);
        }
        
        // Add the new vector to the result vectors.
        resultVectors.push_back(newVector);
    }
    
    return resultVectors;
}

// ==================================================================================
// ((GET RESULT SET)) : (GetResultsAND) : ProcessResults
//     -Thread for GetResultsAND to pipeline a result into the next clause.
// ==================================================================================

typedef unordered_map<string, string> ParamValueMap;
vector<ParamValueMap> RuleBase::ProcessResults(Subrule subrule, int clauseNumber, ParamValueMap inResult)
{
    Clause currentClause = subrule.clauses[clauseNumber];
    
    vector<ParamValueMap> newResults;
    
    // If we are on the first clause...
    if (clauseNumber == 0)
    {
        // Recursively get the results for this individual clause.
        // Recursion continues when the clause is another rule.
        // Recursion terminates when the clause is a fact or non-existent.
        auto clauseResults = engine->GetSet(currentClause.name, currentClause.parameters);
        
        // For each result, translate the vector results into dictionary results.
        for (auto clauseResult : clauseResults)
        {
            ParamValueMap newResult;
            
            // For each parameter...
            for (int i=0; i < ((int) currentClause.parameters.size()); i++)
            {
                // Map the parameter string to the results, value string.
                string parameter = currentClause.parameters[i];
                string value = clauseResult[i];
                
                newResult[parameter] = value;
            }
            
            newResults.push_back(newResult);
        }
    }
    
    // If we are not on the first clause...
    else
    {
        vector<string> inputsToClause;
        
        // For each parameter...
        for (auto param : currentClause.parameters)
        {
            auto entry = inResult.find(param);
            
            // If the parameter is a variable or a new parameter, insert as is.
            if (!IsVariable(param) || entry == inResult.end() )
            {
                inputsToClause.push_back(param);
                continue;
            }
            
            // If the parameter is already mapped to a result, insert the mapped value.
            inputsToClause.push_back(entry->second);
        }
        
        // Recursively get the results for this individual clause.
        // Recursion continues when the clause is another rule.
        // Recursion terminates when the clause is a fact or non-existent.
        auto clauseResults = engine->GetSet(currentClause.name, inputsToClause);
        
        // If there are no results, return empty.
        // This terminates the recursion.
        if (clauseResults.size() == 0)
        {
            vector<ParamValueMap> empty;
            return empty;
        }
        
        // For each new clause result...
        for (auto clauseResult : clauseResults)
        {
            // Duplicate the old, passed in result.
            ParamValueMap newResult (inResult);
            
            // For each parameter of the current clause...
            for (int i=0; i < ((int) currentClause.parameters.size()); i++)
            {
                string parameter = currentClause.parameters[i];
                
                auto entry = newResult.find(parameter);
                
                // If we have not already used the parameter, add it to our mapping.
                if (entry == newResult.end())
                {
                    newResult[parameter] = clauseResult[i];
                }
            }
            
            // Add the mapping to our dictionary of new, pipelined results.
            newResults.push_back(newResult);
        }
    }
    
    // If we are on the last clause, return our final pipelined results.
    // This terminates the recursion.
    if (clauseNumber == ((int) (subrule.clauses.size() - 1)))
    {
        return newResults;
    }

    vector<future<vector<ParamValueMap>>> futures;

    // If we are not on the last clause...
    //    for every new result, start a new thread and pipeline that result into the next clause.
    for (auto newResult : newResults)
    {
        futures.push_back (async(&RuleBase::ProcessResults, (this), subrule, (clauseNumber + 1), newResult));
    }
    
    vector<ParamValueMap> results;
    
    // Wait for each new result thread to finish and merge the returned solutions.
    for(auto &e : futures)
    {
        vector<ParamValueMap> threadResult = e.get();
        results.insert(end(results), begin(threadResult), end(threadResult));
    }
    
    return results;
}

// ==================================================================================
// EXPORT
//     -Writes all rules in RuleBase to a specified file.
// ==================================================================================

string RuleBase::Export()
{
    string output;

    // For every rule...
    for (auto rule : rules)
    {
        // For every subrule...
        for (auto subrule : rule.second)
        {
            // Write the RULE command and the rule name.
            // EX. [ RULE Grandmother ]
            output += ("RULE " + rule.first);
            
            // Write the rule command variables.
            // EX. [ RULE Grandmother($X,$Y):- ]
            output += "(";
            for (int i=0; i < (int)subrule.parameters.size(); i++)
            {
                if( i != 0 ){ output += ","; }
                output += subrule.parameters[i];
            }
            output += "):- ";
            
            
            // Write the AND / OR.
            // EX. [ RULE Grandmother($X,$Y):- AND ]
            output += ((subrule.isAnd) ? "AND" : "OR");
            
            // For each clause, write the clause name and parameter.
            // EX. [ RULE Grandmother($X,$Y):- AND Mother($X,$Z) Parent($Z,$Y) ]
            for (auto clause : subrule.clauses)
            {
                output += (" " + clause.name + "(");
                for (int i=0; i < (int)clause.parameters.size(); i++)
                {
                    if( i != 0 )
                       output +=",";
                    output += clause.parameters[i];
                }
                output += ")";
            }
            output += ";";
        }
    }
    
    return output;
}
