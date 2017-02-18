#include "RuleBase.hpp"
#include <iostream>

using namespace std;

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

vector<vector<string>> RuleBase::GetResultSet(string name)
{
    vector<vector<string>> temp;
    return temp;
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
