#include "Utility.hpp"
#include <sstream>
#include <iostream>
#include <math.h>

using namespace std;

namespace utility
{
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
                return true;
            default:
                if (!isalnum(c))
                    return false;
                return true;
        }
    }
    
    void MakeValid (string& input)
    {
        input.erase(remove_if(input.begin(), input.end(), [](char c){return !isValidChar(c);}), input.end());
    }
    
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
    
    Clause StringToClause(string input)
    {
        Clause output;
        
        vector<string> words = StringToVector(input, '(');
        
        output.name = words[0];
        
        string paramString = words[1];
        paramString.erase( paramString.size() - 1 );
        
        vector<string> params = StringToVector(paramString, ',');
        
        for (auto i : params)
            output.parameters.push_back(i);
        
        return output;
    }
    
    bool IsVariable (string input)
    {
        return (input[0] == '$');
    }
    
    int FindIndexOf (vector<string> vector, string value)
    {
        int result = (int) (find(vector.begin(), vector.end(), value) - vector.begin());
        
        return (result == vector.size()) ? -1 : result;
    }
    
    vector<vector<string>> PermutateVector (vector<vector<string>> input)
    {
        vector<vector<string>> output2;
        
        int outputSize = 0;
        
        for (int k=0; k < input.size(); ++k)
        {
            int permutationSize = (int) input[k].size();
            
            for (int i = k+1; i < input.size(); ++i)
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
}



