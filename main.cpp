#include "SRI.hpp"
#include <iostream>
#include "Utility.hpp"

using namespace std;
using namespace utility;

int main()
{
    SRI engine;
    
    cout << endl << "SRI ENGINE START" << endl << endl;
    
    string input;
    while (getline(cin, input))
    {
        if (input == "x" || input == "EXIT")
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
    
    cout << endl << "SRI ENGINE END" << endl << endl;
    
    return 0;
}
