#include "SRI.hpp"
#include <iostream>
#include "Utility.hpp"

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
    string fact4 = "FACT Father(Lu,Jp)";
    string infer = "INFERENCE GrandFather($X,$Y) GF";
    string infer2 = "INFERENCE Parent($X,$Y) P";
    string infer3 = "INFERENCE Father($egew,Jp) F";
    string drop = "DROP Parent";
    string drop2 = "DROP Father";
    string dump = "DUMP updated_family_relation.sri";
    
    //engine.InterpretLine(fact);
    //engine.InterpretLine(fact2);
    //engine.InterpretLine(fact4);
    //cout << endl;
    //engine.InterpretLine(infer3);
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
