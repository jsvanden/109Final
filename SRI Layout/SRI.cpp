#include "SRI.hpp"
#include <iostream>
#include "Utility.hpp"

using namespace std;
using namespace std::placeholders;
using namespace utility;

void SRI::InterpretLine(const string& line)
{
    unsigned long cutoff = line.find_first_of(" \t");
    string command = line.substr(0, cutoff);
    string params = line.substr(cutoff + 1);
    
    command.erase(remove_if(command.begin(), command.end(), [](char c){return !isValidChar(c);}), command.end());
    params.erase(remove_if(params.begin(), params.end(), [](char c){return !isValidChar(c);}), params.end());
    
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
    
    commands.insert(make_pair("l", bind (&SRI::Load, this, _1)));
    commands.insert(make_pair("s", bind (&SRI::Save, this, _1)));
    commands.insert(make_pair("f", bind (&SRI::InterpretFact, this, _1)));
    commands.insert(make_pair("r", bind (&SRI::InterpretRule, this, _1)));
    commands.insert(make_pair("i", bind (&SRI::InterpretInference, this, _1)));
    commands.insert(make_pair("d", bind (&SRI::Drop, this, _1)));
}

SRI::~SRI()
{
    
}

void SRI::Drop (string name)
{
    knowledgeBase.DropFact(name);
    ruleBase.DropRule(name);
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
    
    // ------------------------------------------
    
    knowledgeBase.AddFact(factName, params);
}

void SRI::InterpretRule(string rule)
{
    vector<string> params;
    
    // ------------------------------------------
    
    unsigned long cutoff = rule.find_first_of("(");
    
    if (cutoff == string::npos)
        return;
    
    string ruleName = rule.substr(0, cutoff);
    string paramString = rule.substr(cutoff + 1);
    
    // ------------------------------------------
    
    unsigned long cutoff_C;
    unsigned long cutoff_P;
    
    while (true)
    {
        cutoff_C = paramString.find_first_of(",");
        cutoff_P = paramString.find_first_of(")");
        
        if (cutoff_C == string::npos && cutoff_P == string::npos)
            break;
        
        if (cutoff_P <= cutoff_C)
            break;
        
        params.push_back(paramString.substr(0, cutoff_C));
        paramString = paramString.substr(cutoff_C + 1);
    }
    
    params.push_back(paramString.substr(0, cutoff_P));
    paramString = paramString.substr(cutoff_P + 1);
    
    // ------------------------------------------
    
    string s_and = "AND";
    string s_or = "OR";
    bool isAnd;
    
    if (strncmp(paramString.c_str(), s_and.c_str(), s_and.size()) == 0)
    {
        isAnd = true;
        paramString = paramString.substr(3);
    }
    else if (strncmp(paramString.c_str(), s_or.c_str(), s_or.size()) == 0)
    {
        isAnd = false;
        paramString = paramString.substr(2);
    }
    else return;
    
    // ------------------------------------------
    
    cout << ruleName << endl;
    for (auto i : params)
        cout << i << " ";
    cout << endl << paramString << endl;
}

void SRI::InterpretInference(string inference)
{
    vector<string> params;
    
    // ------------------------------------------
    
    unsigned long cutoff = inference.find_first_of("(");
    
    if (cutoff == string::npos)
        return;
    
    string inferName = inference.substr(0, cutoff);
    string paramString = inference.substr(cutoff + 1);
    
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
    
    
    
    Infer(inferName, params, paramString);
}

void SRI::Load (string filePath)
{
    cout << filePath << endl;
}

void SRI::Save (string filePath)
{
    cout << filePath << endl;
}
