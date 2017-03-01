#include "Utility.hpp"
#include <sstream>
#include <algorithm>

using namespace std;

namespace utility
{
    // Checks if a character is among the accepted characters.
    bool isValidChar(char c)
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
    
    // Removes a character if it is not among the accepted characters.
    void MakeValid (string& input)
    {
        input.erase(remove_if(input.begin(), input.end(), [](char c){return !isValidChar(c);}), input.end());
    }
    
    // Parses a string input into a vector containing each word.
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
    
    // Returns true if a parameter is a variable.
    bool IsVariable (string input)
    {
        return (input[0] == '$');
    }
    
    // Returns the index of an item in the vector.
    int FindIndexOf (vector<string> vector, string value)
    {
        int result = (int) (find(vector.begin(), vector.end(), value) - vector.begin());
        
        return (result == (int)vector.size()) ? -1 : result;
    }
    
    // Runs through each clause and returns every possible permutation that satisfies a given rule.
    vector<vector<string>> PermutateVector (vector<vector<string>> input)
    {
        vector<vector<string>> output2;
        
        int outputSize = 0;
        
        for (int k=0; k < (int)input.size(); ++k)
        {
            int permutationSize = (int) input[k].size();
            
            for (int i = k+1; i < (int)input.size(); ++i)
            {
                permutationSize *= input[i].size();
            }
            
            outputSize = (k == 0) ? permutationSize : outputSize;
            
            for (int i = 0; i < outputSize; i++)
            {
                int index = floor(i / (permutationSize / input[k].size()) % input[k].size());
                
                if (k == 0)
                    output2.push_back(vector<string> {input[k][index]});
                else
                    output2[i].push_back(input[k][index]);
            }
        }
        
        return output2;
    }
    
    // SRI Exception constructor.
    SRIException::SRIException (string location, string message)
    {
        m_location = location;
        m_message = message;
    }
    
    // SRI Exception alter location.
    void SRIException::SetLocation (std::string newLocation)
    {
        m_location = newLocation;
    }
    
    // SRI Exception throwing.
    const char* SRIException::what()
    {
        string output = m_location + " ERROR: " + m_message + "\n";
        return output.c_str();
    }
}



