#include "KnowledgeBase.hpp"
#include <iostream>
#include "Utility.hpp"

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

//returns all the facts that satisfy the given parameters
vector<vector<string>> KnowledgeBase::GetResultSet(string name, vector<string> params)
{
    auto result = facts.find(name);
    
    if ( result == facts.end() )
    {
        vector<vector<string>> empty;
        return empty;
    }
    
    auto unfilteredResults =  result->second;
    
    vector<vector<string>> finalResults;

    for (auto i : unfilteredResults)
    {
        if (i.size() != params.size())
            continue;
        
        bool isValid = true;
        unordered_map<string, string> usedParams;
        
        for (int k = 0; k < params.size(); ++k)
        {
            if (IsVariable(params[k]))
            {
                auto entry = usedParams.find(params[k]);
                if (entry == usedParams.end())
                {
                    usedParams[params[k]] = i[k];
                    continue;
                }
                string expectedResult = entry->second;
                
                if (i[k] != expectedResult)
                {
                    isValid = false;
                    break;
                }
                
                continue;
            }
            
            if (params[k] != i[k])
            {
                isValid = false;
                break;
            }
        }
        
        if (isValid)
            finalResults.push_back(i);
    }
    
    return finalResults;
}

//outputs the entire knowledge base as an OS stream for the dump command; also used for debugging
void KnowledgeBase::Export(ostream& file)
{
    for (auto fact : facts)
    {
        for (auto entry : fact.second)
        {
            file << "FACT " << fact.first << "(";
            for (int i=0; i < entry.size(); i++)
            {
                if( i!=0 ){ file << ","; }
                file << entry[i];
            }
            file << ")" << endl;
        }
        
    }
}


