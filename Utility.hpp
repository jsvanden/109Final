/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Utility file contains independent helper functions.   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef Utility_hpp
#define Utility_hpp

#include<stdexcept>
#include <vector>
#include <string>

namespace utility
{
    typedef struct Clause
    {
        std::string name;
        std::vector<std::string> parameters;
        
    } Clause;
    
    
    void MakeValid (std::string& input);
    std::vector<std::string> StringToVector(std::string input, char delimiter);
    Clause StringToClause(std::string input);
    bool IsVariable (std::string input);
    int FindIndexOf (std::vector<std::string> vector, std::string value);
    
    class SRIException : public std::exception
    {
    private:
        std::string m_location;
        std::string m_message;
    public:
        SRIException (std::string location, std::string message);
        void SetLocation (std::string newLocation);
        const char* what();
    };
}

#endif
