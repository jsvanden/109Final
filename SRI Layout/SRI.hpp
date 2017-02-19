#ifndef SRI_hpp
#define SRI_hpp

#include "RuleBase.hpp"
#include "KnowledgeBase.hpp"
#include <functional>

class SRI
{
    typedef std::vector<std::string> ListOfWords;
    
private:
    KnowledgeBase knowledgeBase;
    RuleBase ruleBase;
    
    void Drop(ListOfWords input);
    void Fact(ListOfWords input);
    void Rule(ListOfWords input);
    void Save(ListOfWords input);
    void Load(ListOfWords input);
    void Infer(ListOfWords input);

    std::unordered_map< std::string, std::function<void(ListOfWords)> > commands;

public:
    void InterpretLine (std::string& line);
    std::vector<std::vector<std::string>> GetSet (std::string name);
    SRI();
    ~SRI();
};

#endif
