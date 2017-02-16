#include "SRI.hpp"
#include <iostream>

using namespace std;

int main()
{
    cout << "Begin!" << endl;
    
    SRI engine;
    
    string input;
    while (getline(cin, input))
    {
        engine.InterpretLine(input);
    }
    
    return 0;
}
