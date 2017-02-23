#include "SRI.hpp"
#include <iostream>
#include "Utility.hpp"

using namespace std;

int main()
{
    //cout << "Begin!" << endl;
    
    SRI engine;
    
    string fact = "FACT Father(Reggie,John)";
    string fact2 = "FACT Father(Roger,Jp)";
    string fact3 = "FACT Parent(Jp,road)";
    string fact4 = "FACT Father(Lu,Jo)";
    string fact5 = "FACT Parent(Jo,road2)";
    
    string rule2 = "RULE PA($X,$Y):- OR Father($X,$Z) Parent($Z,$Y)";
    string infer2 = "INFERENCE PA($X,$Y) P";
    
    engine.InterpretLine(fact);
    engine.InterpretLine(fact2);
    engine.InterpretLine(fact3);
    engine.InterpretLine(fact4);
    engine.InterpretLine(fact5);
    
    engine.InterpretLine(rule2);
    
    
    //cout << endl;
    engine.InterpretLine(infer2);
    //engine.InterpretLine(fact2);
    //engine.InterpretLine(fact3);
    //engine.InterpretLine(drop2);
    
    /*
    string input;
    while (getline(cin, input))
    {
        engine.InterpretLine(input);
    }
    */
    return 0;
}
