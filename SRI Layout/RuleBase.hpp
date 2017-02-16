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
    typedef std::string RuleName;
    typedef std::vector< Subrule > ListOfSubrules;
    typedef std::vector<std::vector<std::string>> ListOfSets;
    
private:
    SRI * engine;
    std::unordered_map<RuleName, ListOfSubrules> rules;
    
public:
    ListOfSets GetResultSet(RuleName name);
    void AddRule (RuleName name, Subrule info);
    void DropRule (RuleName name);
    void Export (std::ostream file);
};

#endif
