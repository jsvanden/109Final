/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Utility file contains functions for parsing important information     *
 *   from input strings, function for listing the available              *
 *   permutations of a given rule, exception classes.                    *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef Utility_hpp
#define Utility_hpp

#include "RuleBase.hpp"
#include<stdexcept>

namespace utility
{
    void MakeValid (std::string& input);
    std::vector<std::string> StringToVector(std::string input, char delimiter);
    Clause StringToClause(std::string input);
    bool IsVariable (std::string input);
    int FindIndexOf (std::vector<std::string> vector, std::string value);
    std::vector<std::vector<std::string>> PermutateVector (std::vector<std::vector<std::string>> input);
    
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
