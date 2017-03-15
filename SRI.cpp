#include "SRI.hpp"
#include <iostream>
#include "Utility.hpp"
#include <fstream>
#include <algorithm>

using namespace std;
using namespace std::placeholders;
using namespace utility;

// ==================================================================================
// INTERPRET LINE
//     -Takes a string of input and invokes its corresponding function.
// ==================================================================================

string SRI::InterpretLine(string& line)
{
    // Remove invlaid characters and seperate input string by spaces.
    MakeValid(line);
    
    // If line is empty or only whitespace, stop processing.
    if ( line == "" || line.find_first_not_of(' ') >= line.length())
        return "";

    // Split filtered input string by spaces.
    vector<string> words = StringToVector(line, ' ');
    
    // If first word of input string is not a recognized command, throw an error.
    auto commandFunction = commands.find (words[0]);
    if ( commandFunction == commands.end() )
        throw SRIException("SRI", "no matching command");
    
    // Delete first word of input string (the command).
    words.erase(words.begin());
    
    // If there are no parameters after the command name, throw an error.
    if (words.size() == 0)
        throw SRIException("SRI", "no command parameters");
    
    // Call the command's corresponding function.
    string output = commandFunction->second(words);
    
    return output;
}

// ==================================================================================
// SRI CONSTRUCTOR
//     -Set up variables within SRI.
// ==================================================================================

SRI::SRI()
{
    // Link each command string to its corresponsing function.
    
    commands.insert(make_pair("LOAD", bind (&SRI::Load, this, _1)));
    commands.insert(make_pair("DUMP", bind (&SRI::Save, this, _1)));
    commands.insert(make_pair("FACT", bind (&SRI::Fact, this, _1)));
    commands.insert(make_pair("RULE", bind (&SRI::Rule, this, _1)));
    commands.insert(make_pair("INFERENCE", bind (&SRI::Infer, this, _1)));
    commands.insert(make_pair("DROP", bind (&SRI::Drop, this, _1)));

    
    // Give the rule base a reference to the SRI engine to allow inference recurrence.

    ruleBase.engine = (this);
}


// ==================================================================================
// DROP
//     -Removes facts and rules from the SRI engine.
// ==================================================================================


string SRI::Drop(vector<string> input)
{
    knowledgeBase.DropFact(input[0]);
    ruleBase.DropRule(input[0]);
    return "";
}
// ==================================================================================
// FACT
//     -Adds a fact the SRI engine.
// ==================================================================================

string SRI::Fact(vector<string> input)
{
    Clause fact;
    
    try
    {
        fact = StringToClause(input[0]);
    }
    catch (SRIException s)
    {
        s.SetLocation("FACT");
        throw s;
    }
    
    knowledgeBase.AddFact(fact.name, fact.parameters);
    return "";
}

// ==================================================================================
// FACT
//     -Adds a rule the SRI engine.
// ==================================================================================

string SRI::Rule(vector<string> input)
{
    Subrule entry;
    Clause firstClause;
    
    // Get the rule's name and parameters.
    
    try
    {
        firstClause = StringToClause(input[0]);
    }
    catch (SRIException s)
    {
        s.SetLocation("RULE");
        throw s;
    }
    
    if (input.size() < 3)
    {
        throw SRIException("RULE", "invalid number of rule parameters");
    }
    
    entry.parameters = firstClause.parameters;
    
    
    // Determine if the rule is an AND or an OR rule.
    
    if (input[1] == "AND")
    {
        entry.isAnd = true;
    }
    else if (input[1] == "OR")
    {
        entry.isAnd = false;
    }
    else throw SRIException("RULE", "no AND / OR specified");

    
    // Add all clauses after AND/OR to the subrule data structure.
    
    for (int i = 2; i < (int)input.size(); ++i)
    {
        Clause nextClause;
        
        try
        {
            nextClause = StringToClause(input[i]);
        }
        catch (SRIException s)
        {
            s.SetLocation("RULE");
            throw s;
        }
        
        entry.clauses.push_back( nextClause );
    }
    
    
    // Add the rule (rulename + subrule) to the Rule Base.
    
    ruleBase.AddRule(firstClause.name, entry);
    return "";
}

// ==================================================================================
// SAVE (DUMP)
//     -Writes the SRI engine's facts and rules to a file.
// ==================================================================================

string SRI::Save(vector<string> input)
{
    string output;

    output += knowledgeBase.Export();
    output += ruleBase.Export();

    return output;
}

// ==================================================================================
// LOAD
//     -Loads facts and rules from a file, into the SRI engine.
// ==================================================================================

string SRI::Load(vector<string> input)
{
    string filePath = input[0];
    string line;
    ifstream infile;

    // Ensure that ONLY files with extension ".sri" are loaded.
    
    int extensionIdx = (int) filePath.find_last_of('.');
    if( extensionIdx == -1 || filePath.substr(extensionIdx) != ".sri")
    {
       throw SRIException("LOAD", "infile must have extension \".sri\"");
    }
    
    // Open file for reading and add its contents, or print an error.
    
    infile.open( filePath );
    
    if( infile.is_open() )
    {
       // Load input via SRI's InterpretLine.
       while( getline(infile, line) )
       {
           InterpretLine( line );
       }
       
       infile.close();
    }
    
    // If the file could not be opened, throw an error.
    else throw SRIException("LOAD", "could not LOAD from file \"" + filePath + "\"");
    
    return "";
}

// ==================================================================================
// INFER
//     -Determines and prints the results of an inference.
// ==================================================================================

string SRI::Infer(vector<string> input)
{
    Clause inference;
    
    // Get Inference name and output fact name (if exists).
    
    try
    {
        inference = StringToClause(input[0]);
    }
    catch (SRIException s)
    {
        s.SetLocation("INFERENCE");
        throw s;
    }
    
    string outputFact = (input.size() > 1) ? input[1] : "";
    
    // Get the results of the inference.
    
    vector<vector<string>> results = GetSet(inference.name, inference.parameters);
    
    // If there are no results, notify the client and return.
    
    if (results.size() == 0)
    {
        return "NO RESULTS\n";
    }

    // Otherwise, remove duplicate results.
    
    vector<vector<string>> filteredResults;
    
    for (auto result : results)
    {
        if(find(filteredResults.begin(), filteredResults.end(), result) == filteredResults.end())
        {
            bool isValidResult = true;
            
            for (int i=0; i< (int) inference.parameters.size(); i++)
            {
                string parameter = inference.parameters[i];
                if (!IsVariable(parameter))
                {
                    if (result[i] != parameter)
                    {
                        isValidResult = false;
                        break;
                    }
                }
            }
            
            if (isValidResult)
                filteredResults.push_back(result);
        }
    }
    
    // If an output fact is specified, add the results as a new fact.
    
    if (outputFact != "")
    {
        for (auto i : filteredResults)
        {
            knowledgeBase.AddFact(outputFact, i);
        }
    }
    
    // Print results!
    
    string output;
    
    for (auto result : filteredResults)
    {
        for (int i=0; i < (int)inference.parameters.size(); i++)
        {
            string parameter = inference.parameters[i];
            if (IsVariable(parameter))
                parameter.erase(0, 1);
            
            string endString = (i == (int)inference.parameters.size()-1) ? "\n" : ", ";
            
            string addition = parameter + ":" + result[i] + endString;
            output += addition;
        }
    }
    
    return output;
}

// ==================================================================================
// (?) : GetSet
//     -A recursively called function that enables inference to work.
//     -Called from both SRI and RuleBase
// ==================================================================================

// A recursively called function that enables inference to find a set of facts.
vector<vector<string>> SRI::GetSet (string name, vector<string> params)
{
    // Get all facts and rules that match the input.
    
    vector<vector<string>> factResults = knowledgeBase.GetResultSet(name, params);
    vector<vector<string>> ruleResults = ruleBase.GetResultSet(name, params);
    vector<vector<string>> output;

    // Combine the fact results and the rule results.
    
    output.reserve( factResults.size() + ruleResults.size() );
    output.insert( output.end(), factResults.begin(), factResults.end() );
    output.insert( output.end(), ruleResults.begin(), ruleResults.end() );
    
    return output;
}
