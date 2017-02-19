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
    
    Export(cout);
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
    
    for (int i = 0; i < subrule.parameters.size(); ++i)
    {
        nameMappings[subrule.parameters[i]] = vector<string> {params[i]};
    }
    
    // =================================
    
    vector<vector<string>> results;
    
    for (auto clause : subrule.clauses)
    {
        vector<vector<string>> clauseInputs;
        
        for (auto param : clause.parameters)
        {
            vector<string> parameterInputs;
            
            auto entry = nameMappings.find(param);
            
            if ( entry == nameMappings.end() )
            {
                parameterInputs.push_back(param);
                clauseInputs.push_back(parameterInputs);
                continue;
            }
            
            parameterInputs = entry->second;
            clauseInputs.push_back(parameterInputs);
        }
        
        vector<vector<string>> clausePermutation;
        
        
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
