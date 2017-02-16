#include "Utility.hpp"
#include <string>

using namespace std;

namespace utility
{
    bool isValidChar(char c)
    {
        if (c == '_' || c == '$' || c == '(' || c == ')' || c == ',')
            return true;
        
        if (!isalnum(c))
            return false;
        
        return true;
    }
}
