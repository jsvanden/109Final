#include "Utility.hpp"
#include <sstream>
#include <algorithm>

using namespace std;

namespace utility
{
    // ==================================================================================
    // IS VALID CHARACTER
    //     -Checks if a character is among the accepted characters.
    // ==================================================================================

    bool IsValidChar(char c)
    {
        switch (c)
        {
            case '_':
            case '$':
            case '(':
            case ')':
            case ',':
            case ' ':
            case '.':
            case '/':
                return true;
            default:
                if (!isalnum(c))
                    return false;
                return true;
        }
    }
    
    // ==================================================================================
    // MAKE VALID
    //     -Removes a character if it is not among the accepted characters.
    // ==================================================================================

    void MakeValid (string& input)
    {
        input.erase(remove_if(input.begin(), input.end(), [](char c){return !IsValidChar(c);}), input.end());
    }
    
    // ==================================================================================
    // STRING TO VECTOR
    //     -Splits a string (by a specified delimiter) into a vector of strings.
    // ==================================================================================

    vector<string> StringToVector(string input, char delimiter)
    {
        vector<string> output;
        
        istringstream stream(input);
        string word;
        while (getline(stream, word, delimiter))
        {
            if (word.find_first_not_of(' ') == string::npos)
                continue;
            
            output.push_back(word);
        }
        
        return output;
    }
    
    // ==================================================================================
    // STRING TO CLAUSE
    //     -Parses a string containing a clause name and clause parameters.
    //     -Translates string into the Clause data structure.
    // ==================================================================================
    
    // Parses the name and parameters of a clause out of a string and forms them into a data structure.
    Clause StringToClause(string input)
    {
        Clause output;
        
        int parenLCount = (int) count(input.begin(), input.end(), '(');
        int parenRCount = (int) count(input.begin(), input.end(), ')');
        
        if (parenLCount != 1 || parenRCount != 1)
        {
            throw SRIException("StringToClause", "incorrect number of parenthesis in clause");
        }
        
        vector<string> words = StringToVector(input, '(');
        
        if (words.size() == 1)
        {
            throw SRIException("StringToClause", "misplaced parenthesis in clause");
        }
        
        output.name = words[0];
        
        if (output.name == "")
        {
            throw SRIException("StringToClause", "no clause name");
        }
        
        string paramString = words[1];
        
        if (paramString[paramString.size()-1] != ')')
        {
            throw SRIException("StringToClause", "clause does not end with parenthesis");
        }
        
        paramString.erase( paramString.size() - 1 );
        
        vector<string> params = StringToVector(paramString, ',');
        
        int commaCount = (int) count(paramString.begin(), paramString.end(), ',');
        
        if ((int) params.size() != (commaCount+1))
        {
            throw SRIException("StringToClause", "misplaced comma in clause");
        }
        
        for (auto i : params)
            output.parameters.push_back(i);
        
        return output;
    }
    
    // ==================================================================================
    // IS VARIABLE
    //     -Returns true if a parameter is a variable.
    // ==================================================================================
    
    bool IsVariable (string input)
    {
        return (input[0] == '$');
    }
    
    // ==================================================================================
    // FIND INDEX OF
    //     -Returns the index of a string in the vector or -1 if the string DNE.
    // ==================================================================================
    
    int FindIndexOf (vector<string> vector, string value)
    {
        int result = (int) (find(vector.begin(), vector.end(), value) - vector.begin());
        
        return (result == (int)vector.size()) ? -1 : result;
    }
    
    // ==================================================================================
    // (SRI EXCEPTION) : SRIException
    //     -SRI Exception constructor.
    // ==================================================================================
    
    SRIException::SRIException (string location, string message)
    {
        m_location = location;
        m_message = message;
    }
    
    // ==================================================================================
    // (SRI EXCEPTION) : SetLocation
    //     -Alters where the exception is being thrown from.
    // ==================================================================================
    
    void SRIException::SetLocation (std::string newLocation)
    {
        m_location = newLocation;
    }
    
    // ==================================================================================
    // (SRI EXCEPTION) : what
    //     -Returns the error message specified when thrown.
    // ==================================================================================
    
    const char* SRIException::what()
    {
        string output = m_location + " ERROR: " + m_message + "\n";
        return output.c_str();
    }
}



