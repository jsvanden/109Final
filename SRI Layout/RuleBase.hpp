#ifndef RuleBase_hpp
#define RuleBase_hpp

#include <string>
#include <unordered_map>
#include <vector>

typedef struct Clause
{
    std::string name;
    std::vector<std::string> parameters;
    
} Clause;

typedef struct Subrule
{
    bool isAnd;
    std::vector<std::string> parameters;
    std::vector<Clause> clauses;
} Subrule;

class SRI;

class RuleBase
{
    friend class SRI;
    
    typedef std::string RuleName;
    typedef std::vector< Subrule > ListOfSubrules;
    typedef std::vector<std::vector<std::string>> ListOfSets;
    
private:
    SRI * engine;
    std::unordered_map<RuleName, ListOfSubrules> rules;
    ListOfSets GetResultsOR(Subrule subrule, std::string name, std::vector<std::string> params);
    ListOfSets GetResultsAND(Subrule subrule, std::string name, std::vector<std::string> params);
    
public:
    ListOfSets GetResultSet(RuleName name, std::vector<std::string> params);
    void AddRule (RuleName name, Subrule info);
    void DropRule (RuleName name);
    void Export (std::ostream& file);
};

#endif
/*defines the database of rules and their potential uses*/