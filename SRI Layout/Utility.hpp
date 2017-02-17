#ifndef Utility_hpp
#define Utility_hpp

#include "RuleBase.hpp"
#include <string>
#include <vector>

namespace utility
{
    void makeValid (std::string& input);
    std::vector<std::string> StringToVector(std::string input, char delimiter);
    Clause StringToClause(std::string input);
}

#endif
