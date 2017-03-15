/* * * * * * * * * * * * * * * * * *
 * Defines the database of rules.  *
 * * * * * * * * * * * * * * * * * */

#ifndef RuleBase_hpp
#define RuleBase_hpp

#include <string>
#include <unordered_map>
#include <vector>

#include "Utility.hpp"

typedef struct Subrule
{
    bool isAnd;
    std::vector<std::string> parameters;
    std::vector<utility::Clause> clauses;
} Subrule;

class SRI;

class RuleBase
{
    friend class SRI;
    
    typedef std::string RuleName;
    typedef std::vector< Subrule > ListOfSubrules;
    typedef std::vector<std::vector<std::string>> ListOfSets;
    typedef std::unordered_map<std::string, std::string> ParamValueMap;
    
private:
    SRI * engine;
    std::unordered_map<RuleName, ListOfSubrules> rules;
    ListOfSets GetResultsOR(Subrule subrule, std::string name, std::vector<std::string> params);
    ListOfSets GetResultsAND(Subrule subrule, std::string name, std::vector<std::string> params);
    ListOfSets GetResultsClause (utility::Clause clause, Subrule subrule, std::vector<std::string> params);
    std::vector<ParamValueMap> ProcessResults (Subrule subrule, int clauseNumber, ParamValueMap inResult);
    
public:
    ListOfSets GetResultSet(RuleName name, std::vector<std::string> params);
    void AddRule (RuleName name, Subrule info);
    void DropRule (RuleName name);
    std::string Export ();
};

#endif
