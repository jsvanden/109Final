/* * * * * * * * * * * * * * * * * * * *
 * Defines the database of all facts.  *
 * * * * * * * * * * * * * * * * * * * */

#ifndef KnowledgeBase_hpp
#define KnowledgeBase_hpp

#include <string>
#include <unordered_map>
#include <vector>

class KnowledgeBase
{
    typedef std::string FactName;
    typedef std::vector<std::string> FactParams;
    typedef std::vector<FactParams> ListOfSets;
    
private:
    std::unordered_map<FactName, ListOfSets> facts;
    
public:
    ListOfSets GetResultSet(FactName name, FactParams params);
    void AddFact (FactName name, FactParams params);
    void DropFact (FactName name);
    void Export (std::ostream& file);
};

#endif
