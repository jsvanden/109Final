#ifndef SRI_hpp
#define SRI_hpp

#include "RuleBase.hpp"
#include "KnowledgeBase.hpp"
#include <functional>

class SRI
{
private:
    KnowledgeBase knowledgeBase;
    RuleBase ruleBase;
    void Drop (std::string name);
    void Infer (std::string name, std::vector<std::string> params, std::string outFact = NULL);
    std::unordered_map< std::string, std::function<void(std::string)> > commands;
    void InterpretFact(std::string fact);
    void InterpretRule(std::string rule);
    void InterpretInference(std::string inference);
    void Load (std::string filePath);
    void Save (std::string filePath);

public:
    void InterpretLine (const std::string& line);
    std::vector<std::string> GetSet (std::string name);
    SRI();
    ~SRI();
};

#endif
