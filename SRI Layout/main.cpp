#include "SRI.hpp"
#include <iostream>
#include "Utility.hpp"

using namespace std;

int main()
{
    SRI engine;
    
    cout << "SRI ENGINE START" << endl;
    
    string input;
    while (getline(cin, input))
    {
        if (input == "x")
            break;
        
        engine.InterpretLine(input);
    }
    
    cout << "SRI ENGINE END" << endl;
    
    return 0;
}
