#include "RuleBase.hpp"
#include <iostream>
#include "Utility.hpp"
#include "SRI.hpp"

using namespace std;
using namespace utility;

void RuleBase::DropRule (string name)
{
    rules.erase(name);
    Export(cout);
}

void RuleBase::AddRule (string name, Subrule info)
{
    auto ruleEntry = rules.find (name);
    
    if ( ruleEntry == rules.end() )
    {
        rules[name] = vector<Subrule> {info};
    }
    else
    {
        rules[name].push_back(info);
    }
    
    //Export(cout);
}

vector<vector<string>> RuleBase::GetResultSet(string name, vector<string> params)
{
    auto entry = rules.find(name);
    
    if ( entry == rules.end() )
    {
        vector<vector<string>> empty;
        return empty;
    }
    
    vector<vector<string>> results;
    
    vector<Subrule> subrules = entry->second;
    
    for (auto subrule : subrules)
    {
        if (subrule.parameters.size() != params.size())
            continue;
        
        vector<vector<string>> subruleResults;
        
        if (subrule.isAnd)
        {
            subruleResults = GetResultsAND(subrule, name, params);
        }
        else
        {
            subruleResults = GetResultsOR(subrule, name, params);
        }
        
        results.insert(end(results), begin(subruleResults), end(subruleResults));
    }
    
    return results;
}

vector<vector<string>> RuleBase::GetResultsOR(Subrule subrule, string name, vector<string> params)
{
    vector<vector<string>> results;
    
    for (auto clause : subrule.clauses)
    {
        vector<string> inputs;
        
        for (auto i : clause.parameters)
        {
            int index = FindIndexOf(subrule.parameters, i);
            
            if(index == -1)
                inputs.push_back(i);
            else
                inputs.push_back(params[index]);
        }
        
        vector<vector<string>> clauseSet = engine->GetSet(clause.name, inputs);
        results.insert(end(results), begin(clauseSet), end(clauseSet));
    }
    
    return results;
}

vector<vector<string>> RuleBase::GetResultsAND(Subrule subrule, string name, vector<string> params)
{
    unordered_map<string, vector<string>> nameMappings;
    vector<string> usedVariables;

    for (auto clause : subrule.clauses)
    {
        vector<vector<string>> clauseInputs;
        
        // Get all parameters to be run through GetSet
        // ======================================================================
        for (auto param : clause.parameters)
        {
            vector<string> parameterInputs;
            
            // If PARAM is not a variable, use it as an input directly
            if (!IsVariable(param))
            {
                parameterInputs.push_back(param);
                clauseInputs.push_back(parameterInputs);
                continue;
            }

            // If $PARAM has not been used yet, use the $ form as an input
            auto entry = nameMappings.find(param);
            if ( entry == nameMappings.end() )
            {
                parameterInputs.push_back(param);
                clauseInputs.push_back(parameterInputs);
                continue;
            }
            
            // If $PARAM has been used, use its candidate results as input
            parameterInputs = entry->second;
            clauseInputs.push_back(parameterInputs);
        }
        // ======================================================================
        
        vector<vector<string>> clausePermutation = PermutateVector(clauseInputs);
        
        vector<vector<string>> clauseResults;
        for (auto i : clausePermutation)
        {
            auto temp = engine->GetSet(clause.name, i);
            clauseResults.insert(end(clauseResults), begin(temp), end(temp));
        }

        // What to do with results of a Clause
        // ======================================================================
        for (int i = 0; i < clause.parameters.size(); ++i)
        {
            string param = clause.parameters[i];
            
            //cout << param << endl;
            
            // If PARAM is not a variable, ignore it
            if (!IsVariable(param))
            {
                continue;
            }
            
            vector<string> resultSet;
            for (auto result : clauseResults)
            {
                resultSet.push_back(result[i]);
            }
            
            auto entry = nameMappings.find(param);

            // If $PARAM is not used, add the results directly
            if ( entry == nameMappings.end() )
            {
                nameMappings[param] = resultSet;
                continue;
            }
            
            // If $PARAM is used, delete candidates not in result set
            vector<string> & candidateSet = entry->second;
            
            auto iter = candidateSet.begin();
            while(iter != candidateSet.end())
            {
                if(FindIndexOf(resultSet, iter[0]) == -1)
                {
                    int index = (int) (iter - candidateSet.begin());
                    
                    for (auto & entry : nameMappings)
                    {
                        if (entry.first != param)
                        {
                            entry.second.erase(entry.second.begin() + index);
                        }
                    }
                    
                    iter = candidateSet.erase(iter);
                }
                else ++iter;
            }
        }
        // ======================================================================
    }
    
    vector<vector<string>> results;
    
    for (int i = 0; i < params.size(); ++i)
    {
        vector<string> paramResults = nameMappings[params[i]];
        
        if (i == 0)
        {
            for (auto result : paramResults)
                results.push_back(vector<string> {result});
        }
        else
        {
            for (int k = 0; k < results.size(); ++k)
                results[k].push_back(paramResults[k]);
        }
    }
    
    return results;
}

void RuleBase::Export(ostream& file)
{
    file << "==== RULES ====" << endl;
    
    for (auto rule : rules)
    {
        file << rule.first << endl;
        
        for (auto subrule : rule.second)
        {
            cout << "  | ";
            for (auto param : subrule.parameters)
            {
                cout << param << " | ";
            }
            cout << endl;
            
            cout << "  " << ((subrule.isAnd) ? "AND" : "OR") << endl;
            
            for (auto clause : subrule.clauses)
            {
                cout << "    " << clause.name << "  | ";
                for (auto param : clause.parameters)
                {
                    cout << param << " | ";
                }
                cout << endl;
            }
        }
    }
    
}
