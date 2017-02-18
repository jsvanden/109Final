#include "KnowledgeBase.hpp"
#include <iostream>

using namespace std;

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
    
    Export(cout);
}

void KnowledgeBase::DropFact (string name)
{
    facts.erase(name);
    Export(cout);
}

vector<vector<string>> KnowledgeBase::GetResultSet(string name)
{
    vector<vector<string>> temp;
    return temp;
}

void KnowledgeBase::Export(ostream& file)
{
    file << "==== FACTS ====" << endl;
    
    for (auto fact : facts)
    {
        file << fact.first << ": | ";
        
        for (auto entry : fact.second)
        {
            for (auto set : entry)
            {
                file << set << " ";
            }
            file << "| ";
        }
        
        file << endl;
    }
}


