/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * The class that client programs interact with through InterpretLine.   *
 * Holds references to the Knowledge Base (facts) and Rule Base (rules). *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef SRI_hpp
#define SRI_hpp

#include "RuleBase.hpp"
#include "KnowledgeBase.hpp"
#include <functional>
#include <iostream>

class SRI
{
    typedef std::vector<std::string> ListOfWords;
    
private:
    KnowledgeBase knowledgeBase;
    RuleBase ruleBase;
    
    std::string Drop(ListOfWords input);
    std::string Fact(ListOfWords input);
    std::string Rule(ListOfWords input);
    std::string Save(ListOfWords input);
    std::string Load(ListOfWords input);
    std::string Infer(ListOfWords input);

    std::unordered_map< std::string, std::function<std::string(ListOfWords)> > commands;

public:
    std::string InterpretLine (std::string& line);
    std::vector<std::vector<std::string>> GetSet (std::string name, std::vector<std::string> params);
    SRI();
};

#endif
