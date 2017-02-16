#include "SRI.hpp"
#include <iostream>

using namespace std;
using namespace std::placeholders;

void SRI::InterpretLine(const string& line)
{
    unsigned long cutoff = line.find_first_of(" \t");
    string command = line.substr(0, cutoff);
    string params = line.substr(cutoff + 1);
    
    auto commandFuncPair = commands.find (command);
    
    if ( commandFuncPair == commands.end() )
        return;
    
    commandFuncPair->second(params);
}

SRI::SRI()
{
    commands.insert(make_pair("LOAD", bind (&SRI::Load, this, _1)));
    commands.insert(make_pair("DUMP", bind (&SRI::Save, this, _1)));
    commands.insert(make_pair("FACT", bind (&SRI::InterpretFact, this, _1)));
    commands.insert(make_pair("RULE", bind (&SRI::InterpretRule, this, _1)));
    commands.insert(make_pair("INFERENCE", bind (&SRI::InterpretInference, this, _1)));
    commands.insert(make_pair("DROP", bind (&SRI::Drop, this, _1)));
}

SRI::~SRI()
{
    
}

void SRI::Drop (string name)
{
    cout << name << endl;
}

void SRI::Infer (string name, vector<std::string> params, string outFact)
{
    cout << name << endl;
}

void SRI::InterpretFact(string fact)
{
    vector<string> params;
    
    // ------------------------------------------
    
    unsigned long cutoff = fact.find_first_of("(");
    
    if (cutoff == string::npos)
        return;
    
    string factName = fact.substr(0, cutoff);
    string paramString = fact.substr(cutoff + 1);
    
    // ------------------------------------------
    
    while (true)
    {
        cutoff = paramString.find_first_of(",");
    
        if (cutoff == string::npos)
            cutoff = paramString.find_first_of(")");
    
        if (cutoff == string::npos)
            break;
        
        params.push_back(paramString.substr(0, cutoff));
        paramString = paramString.substr(cutoff + 1);
    }
    
    knowledgeBase.AddFact(factName, params);
}

void SRI::InterpretRule(string rule)
{
    cout << rule << endl;
}

void SRI::InterpretInference(string inference)
{
    cout << inference << endl;
}

void SRI::Load (string filePath)
{
    cout << filePath << endl;
}

void SRI::Save (string filePath)
{
    cout << filePath << endl;
}
