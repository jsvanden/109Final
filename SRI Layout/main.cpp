#include "SRI.hpp"
#include <iostream>
#include "Utility.hpp"

using namespace std;

int main()
{
    //cout << "Begin!" << endl;
    
    SRI engine;
    
    string f1 = "FACT Father(Jo,1)";
    
    /*
    string f1 = "FACT Father(Roger,John)";
    string f3 = "FACT Father(Roger,Albert)";
    string f5 = "FACT Father(Allen,Margret)";
    
    string f2 = "FACT Mother(Marry,John)";
    string f4 = "FACT Mother(Marry,Albert)";
    string f6 = "FACT Mother(Margret,Robert)";
    string f7 = "FACT Mother(Margret,Bob)";
    
    string r1 = "RULE Parent($X,$Y):- OR Father($X,$Y) Mother($X,$Y)";
    string r2 = "RULE GrandFather($X,$Y):- AND Father($X,$Z) Parent($Z,$Y)";
    string r3 = "RULE GrandMother($X,$Y):- AND Mother($X,$Z) Mother($X,$Y)";
    string r4 = "RULE GrandMother($X,$Y):- AND Mother($X,$Z) Father($X,$Y)";
    
    string i1 = "INFERENCE Father($X,$Y)";
    string i2 = "INFERENCE Parent($A,$B)";
    string i3 = "INFERENCE GrandFather($A,$B)";
    
    
    engine.InterpretLine(f1);
    engine.InterpretLine(f2);
    engine.InterpretLine(f3);
    engine.InterpretLine(f4);
    engine.InterpretLine(f5);
    engine.InterpretLine(f6);
    engine.InterpretLine(f7);
    
    engine.InterpretLine(r1);
    engine.InterpretLine(r2);
    engine.InterpretLine(r3);
    engine.InterpretLine(r4);
    
    engine.InterpretLine(i3);
    */
    /*
    string input;
    while (getline(cin, input))
    {
        engine.InterpretLine(input);
    }
    */
    return 0;
}
