#include "SRI.hpp"
#include <iostream>

using namespace std;

int main()
{
    cout << "Begin!" << endl;
    
    SRI engine;
    
    string rule = "RULE Parent($X,$Y):- OR Father($X,$Y) Mother($X,$Y)";
    string rule2 = "RULE Parent2($X,$Y):- AND Father($X,$Z) FA($Z,$Y)";
    string fact = "FACT Father(Roger,John)";
    string fact2 = "FACT Father(Roger,Jp)";
    string fact3 = "FACT Mother(Lu,We)";
    string infer = "INFERENCE GrandFather($X,$Y) GF";
    string drop = "DROP Parent";
    string drop2 = "DROP Father";
    string dump = "DUMP updated_family_relation.sri";
    
    engine.InterpretLine(fact);
    engine.InterpretLine(fact2);
    engine.InterpretLine(fact3);
    engine.InterpretLine(drop2);
    
    /*
    string input;
    while (getline(cin, input))
    {
        engine.InterpretLine(input);
    }
    */
    
    return 0;
}
