#include "SRI.hpp"
#include <iostream>
#include "Utility.hpp"
#include <fstream>
#include <algorithm>

using namespace std;
using namespace std::placeholders;
using namespace utility;


// Takes a line of input and determines which command/rule it is invoking.
void SRI::InterpretLine(string& line)
{
    // ==============================================================
    // Remove invlaid characters and seperate input string by spaces.
    // ==============================================================
    
    MakeValid(line);
    vector<string> words = StringToVector(line, ' ');
    
    // ===========================================================================
    // If first word of input string is not a recognized command, stop processing.
    // ===========================================================================
    
    auto commandFunction = commands.find (words[0]);
    if ( commandFunction == commands.end() )
        return;
    
    // ===============================================================================
    // Delete first word of input string, send the rest to the corresponding function.
    // ===============================================================================
    
    words.erase(words.begin());
    commandFunction->second(words);
}

// SRI Constructor
SRI::SRI()
{
    // ==================================================
    // Link command string to its corresponsing function.
    // ==================================================
    
    commands.insert(make_pair("LOAD", bind (&SRI::Load, this, _1)));
    commands.insert(make_pair("DUMP", bind (&SRI::Save, this, _1)));
    commands.insert(make_pair("FACT", bind (&SRI::Fact, this, _1)));
    commands.insert(make_pair("RULE", bind (&SRI::Rule, this, _1)));
    commands.insert(make_pair("INFERENCE", bind (&SRI::Infer, this, _1)));
    commands.insert(make_pair("DROP", bind (&SRI::Drop, this, _1)));
    
    commands.insert(make_pair("l", bind (&SRI::Load, this, _1)));
    commands.insert(make_pair("s", bind (&SRI::Save, this, _1)));
    commands.insert(make_pair("f", bind (&SRI::Fact, this, _1)));
    commands.insert(make_pair("r", bind (&SRI::Rule, this, _1)));
    commands.insert(make_pair("i", bind (&SRI::Infer, this, _1)));
    commands.insert(make_pair("d", bind (&SRI::Drop, this, _1)));
    
    // ===============================================================================
    // Give the rule base a reference to the SRI engine to allow inference recurrence.
    // ===============================================================================
    
    ruleBase.engine = (this);
}

SRI::~SRI(){} // SRI Destructor


// Funtion definitions for each command ---------------------------------------------


void SRI::Drop(vector<string> input)
{
    // =============================================
    // Remove input string from all facts and rules.
    // =============================================
    
    knowledgeBase.DropFact(input[0]);
    ruleBase.DropRule(input[0]);
}

void SRI::Fact(vector<string> input)
{
    // ===============================
    // Add fact to the knowledge base.
    // ===============================
    
    Clause fact = StringToClause(input[0]);
    knowledgeBase.AddFact(fact.name, fact.parameters);
}

void SRI::Rule(vector<string> input)
{
    // =============================
    // Get rule name and parameters.
    // =============================
    
    Subrule entry;
    Clause firstClause = StringToClause(input[0]);
    entry.parameters = firstClause.parameters;
    
    // ==========================================
    // Determine if rule is an AND or an OR rule.
    // ==========================================
    
    if (input[1] == "AND")
    {
        entry.isAnd = true;
    }
    else if (input[1] == "OR")
    {
        entry.isAnd = false;
    }
    else return;
    
    // ===========================================================
    // Add all clauses after AND/OR to the subrule data structure.
    // ===========================================================
    
    for (int i = 2; i < (int)input.size(); ++i)
    {
        entry.clauses.push_back( StringToClause(input[i]) );
    }
    
    // ===================================================
    // Add the rule (rulename + subrule) to the Rule Base.
    // ===================================================
    
    ruleBase.AddRule(firstClause.name, entry);
}

// Called by DUMP; Saves the KnowledgeBase and RuleBase to an external file, formatted to Interpret
//      Returns true on a successful save, false otherwise
bool SRI::Save(vector<string> input)
{
    string filePath = input[0];
    ofstream outfile;
    
    //=========================================
    // Ensure created file has ".sri" extension
    // ========================================
    
    int extensionIdx = filePath.find_last_of('.');
   
    if( extensionIdx == -1)
    {
       filePath += ".sri";
       // cout << "   Adding \".sri\" extension to \"" << filePath << "\"\n";
    }
    else if( filePath.substr(extensionIdx) != ".sri")
    {
       filePath += ".sri";
       // cout << "   Adding \".sri\" extension to \"" << filePath << "\"\n";
    }
    
    // ============================
    // Save contents of SRI to file
    // ============================
    
    outfile.open( filePath );
    
    if( outfile.is_open() )
    {
       // load RuleBase and KnowledgeBase
       ruleBase.Export( outfile );
       knowledgeBase.Export( outfile );
      
       // close file
       outfile.close();
       // cout << "Successful DUMP of file \"" << filePath << "\"\n";
       return true;
    }
    else     // if the file failed to open for writing
    {
       cerr << "SRI Error: could not DUMP file \"" << filePath << "\"\n";
       return false;
    }
    
}

// Called by LOAD; Adds the contents of a file to this SRI's RuleBase and KnowledgeBase
//      Returns true on a successful load, false otherwise
//      Will only load files with ".sri" extension; returns false if not ".sri"
bool SRI::Load(vector<string> input)
{
    string filePath = input[0];
    string line;
    ifstream infile;
    
    // =======================================================
    // Ensure that ONLY files with extension ".sri" are loaded
    // =======================================================
    
    int extensionIdx = filePath.find_last_of('.');
    if( extensionIdx == -1)
    {
       cerr << "LOAD Error: infile must have extension \".sri\"" << endl;
       cerr << "   Could not LOAD from file \"" << filePath << "\"\n";
       return false;
    }
    else if( filePath.substr(extensionIdx) != ".sri")
    {
       cerr << "LOAD Error: infile must have extension \".sri\"" << endl;
       cerr << "   Could not LOAD from file \"" << filePath << "\"\n";
       return false;
    }
    
    // =============================================================
    // Open file for reading and add its contents, or print an error
    // =============================================================
    
    infile.open( filePath );
    
    if( infile.is_open() )
    {
    
       // load input via SRI's InterpretLine
       while( getline(infile, line) )
       {
           InterpretLine( line );
       }
       
       infile.close();
       // cout << "Successful LOAD of file \"" << filePath << "\"\n";
       return true;
    }
    // if the file could not be opened, print message and return false
    else
    {
       cerr << "LOAD Error: could not LOAD from file \"" << filePath << "\"\n";
       return false;
    }
    
}

void SRI::Infer(vector<string> input)
{
    // ====================================================
    // Get Inference name and output fact name (if exists).
    // ====================================================
    
    Clause inference = StringToClause(input[0]);
    string outputFact = (input.size() > 1) ? input[1] : "";
    
    // =========================
    // Get results of inference.
    // =========================
    
    vector<vector<string>> results = GetSet(inference.name, inference.parameters);
    
    // ======================================
    // If there are no results, return empty.
    // ======================================
    
    if (results.size() == 0)
    {
        cout << "NO RESULTS" << endl;
        return;
    }
    
    // =========================
    // Remove duplicate results.
    // =========================
    
    vector<vector<string>> filteredResults;
    
    for (auto result : results)
    {
        if(find(filteredResults.begin(), filteredResults.end(), result) == filteredResults.end())
        {
            filteredResults.push_back(result);
        }
    }
    
    // ==============================================================
    // If an output fact is specified, add the results as a new fact.
    // ==============================================================
    
    if (outputFact != "")
    {
        for (auto i : filteredResults)
        {
            knowledgeBase.AddFact(outputFact, i);
        }
    }
    
    // ==========================
    // Print results!
    // ==========================
    
    for (auto result : filteredResults)
    {
        for (int i=0; i < (int)inference.parameters.size(); i++)
        {
            string parameter = inference.parameters[i];
            if (IsVariable(parameter))
                parameter.erase(0, 1);
            
            string endString = (i == (int)inference.parameters.size()-1) ? "\n" : ", ";
            
            cout << parameter << ":" << result[i] << endString;
        }
    }
}

// A recursively called function that enables inference to find a set of facts.
vector<vector<string>> SRI::GetSet (string name, vector<string> params)
{
    // =============================================
    // Get all facts and rules that match the input.
    // =============================================
    
    vector<vector<string>> factResults = knowledgeBase.GetResultSet(name, params);
    vector<vector<string>> ruleResults = ruleBase.GetResultSet(name, params);
    vector<vector<string>> output;
    
    // ==============================================
    // Combine the fact results and the rule results.
    // ==============================================
    
    output.reserve( factResults.size() + ruleResults.size() );
    output.insert( output.end(), factResults.begin(), factResults.end() );
    output.insert( output.end(), ruleResults.begin(), ruleResults.end() );
    
    return output;
}
