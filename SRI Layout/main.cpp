#include "SRI.hpp"
#include <iostream>
#include "Utility.hpp"

using namespace std;
using namespace utility;

int main()
{
    SRI engine;
    
    cout << "SRI ENGINE START" << endl;
    
    string input;
    while (getline(cin, input))
    {
        if (input == "x")
            break;
        
        try
        {
            engine.InterpretLine(input);
        }
        catch (SRIException s)
        {
            cout << s.what();
        }
    }
    
    cout << "SRI ENGINE END" << endl;
    
    return 0;
}
