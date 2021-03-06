// Unit Tests //

#define CATCH_CONFIG_MAIN
#include "Catch.hpp"

#include "Utility.hpp"
#include "SRI.hpp"

#include <thread>

using namespace std;
using namespace utility;

// =================================================================
// Example on PDF
// =================================================================

TEST_CASE( "Example on PDF" )
{
    SRI engine;
    
    // =================================================================
    
    string f1 = "FACT Father(Roger,John)";
    string f2 = "FACT Mother(Marry,John)";
    string f3 = "FACT Father(Roger,Albert)";
    string f4 = "FACT Mother(Marry,Albert)";
    string f5 = "FACT Father(Allen,Margret)";
    string f6 = "FACT Mother(Margret,Robert)";
    string f7 = "FACT Mother(Margret,Bob)";
    
    // =================================================================
    
    SECTION( "-interpreting basic facts" )
    {
        engine.InterpretLine(f1);
        engine.InterpretLine(f2);
        engine.InterpretLine(f3);
        engine.InterpretLine(f4);
        engine.InterpretLine(f5);
        engine.InterpretLine(f6);
        engine.InterpretLine(f7);
        
        string simpleInference = "INFERENCE Father($X,$Y)";
        string output = engine.InterpretLine(simpleInference);
        
        string expected = "X:Roger, Y:John\nX:Roger, Y:Albert\nX:Allen, Y:Margret\n";
        
        REQUIRE( output == expected );
    }
    
    // =================================================================
    
    string r1 = "RULE Parent($X,$Y):- OR Father($X,$Y) Mother($X,$Y)";
    
    // =================================================================
    
    SECTION( "-interpreting basic rules" )
    {
        engine.InterpretLine(f1);
        engine.InterpretLine(f2);
        engine.InterpretLine(f3);
        engine.InterpretLine(f4);
        engine.InterpretLine(f5);
        engine.InterpretLine(f6);
        engine.InterpretLine(f7);
        engine.InterpretLine(r1);

        string simpleInference = "INFERENCE Parent($A,$B)";
        string output = engine.InterpretLine(simpleInference);
        
        string expected = "A:Roger, B:John\nA:Roger, B:Albert\nA:Allen, B:Margret\nA:Marry, B:John\nA:Marry, B:Albert\nA:Margret, B:Robert\nA:Margret, B:Bob\n";
        
        REQUIRE( output == expected );
    }
    
    // =================================================================
    
    string r2 = "RULE GrandFather($X,$Y):- AND Father($X,$Z) Parent($Z,$Y)";
    
    // =================================================================
    
    SECTION( "-interpreting rules within rules" )
    {
        engine.InterpretLine(f1);
        engine.InterpretLine(f2);
        engine.InterpretLine(f3);
        engine.InterpretLine(f4);
        engine.InterpretLine(f5);
        engine.InterpretLine(f6);
        engine.InterpretLine(f7);
        engine.InterpretLine(r1);
        engine.InterpretLine(r2);
        
        string simpleInference = "INFERENCE GrandFather($A,$B)";
        string output = engine.InterpretLine(simpleInference);
        
        string expected = "A:Allen, B:Robert\nA:Allen, B:Bob\n";
        
        REQUIRE( output == expected );
    }
    
    // =================================================================
    
    SECTION( "-interpreting rules with specified constants" )
    {
        engine.InterpretLine(f1);
        engine.InterpretLine(f2);
        engine.InterpretLine(f3);
        engine.InterpretLine(f4);
        engine.InterpretLine(f5);
        engine.InterpretLine(f6);
        engine.InterpretLine(f7);
        engine.InterpretLine(r1);
        engine.InterpretLine(r2);

        string simpleInference = "INFERENCE GrandFather($A,Robert)";
        string output = engine.InterpretLine(simpleInference);
        
        string expected = "A:Allen, Robert:Robert\n";
        
        REQUIRE( output == expected );
    }
    
    // =================================================================
    
    SECTION( "-saving a file" )
    {
        engine.InterpretLine(f1);
        engine.InterpretLine(f2);
        engine.InterpretLine(f3);
        engine.InterpretLine(f4);
        engine.InterpretLine(f5);
        engine.InterpretLine(f6);
        engine.InterpretLine(f7);
        engine.InterpretLine(r1);
        engine.InterpretLine(r2);
        
        string saveCommand = "DUMP temp.sri";
        string output = engine.InterpretLine(saveCommand);
        
        string expected = "FACT Mother(Marry,John);FACT Mother(Marry,Albert);FACT Mother(Margret,Robert);FACT Mother(Margret,Bob);FACT Father(Roger,John);FACT Father(Roger,Albert);FACT Father(Allen,Margret);RULE GrandFather($X,$Y):- AND Father($X,$Z) Parent($Z,$Y);RULE Parent($X,$Y):- OR Father($X,$Y) Mother($X,$Y);";
        
        REQUIRE( output == expected );
    }
    
    // =================================================================
    
    SECTION( "-dropping a fact" )
    {
        engine.InterpretLine(f1);
        engine.InterpretLine(f2);
        engine.InterpretLine(f3);
        engine.InterpretLine(r1);
        
        string simpleDrop = "DROP Father";
        engine.InterpretLine(simpleDrop);
        
        string simpleInference = "INFERENCE Parent($A,$B)";
        string output = engine.InterpretLine(simpleInference);
        
        string expected = "A:Marry, B:John\n";
        
        REQUIRE( output == expected );
    }
    
    // =================================================================
    
    SECTION( "-dropping a rule" )
    {
        engine.InterpretLine(f1);
        engine.InterpretLine(r1);
        
        string simpleDrop = "DROP Parent";
        engine.InterpretLine(simpleDrop);
        
        string simpleInference = "INFERENCE Parent($A,$B)";
        string output = engine.InterpretLine(simpleInference);
        
        string expected = "NO RESULTS\n";
        
        REQUIRE( output == expected );
    }
}

// =================================================================
// Bad Input 1
// =================================================================

TEST_CASE( "Bad Input 1" )
{
    SRI engine;
    
    // =================================================================
    
    SECTION( "-invalid command" )
    {
        string f1 = "FAC";
        string output = "";
        
        try
        {
            engine.InterpretLine(f1);
        }
        catch (SRIException s)
        {
            output = s.what();
        }
        
        
        string expected = "SRI ERROR: no matching command\n";
        
        REQUIRE( output == expected );
    }
    
    // =================================================================
    
    SECTION( "-no command" )
    {
        string f1 = "   ";
        string output = "";
        
        try
        {
            engine.InterpretLine(f1);
        }
        catch (SRIException s)
        {
            output = s.what();
        }
        
        
        string expected = "";
        
        REQUIRE( output == expected );
    }
    
    // =================================================================
    
    SECTION( "-command with no parameters" )
    {
        string f1 = "FACT  ";
        string output = "";
        
        try
        {
            engine.InterpretLine(f1);
        }
        catch (SRIException s)
        {
            output = s.what();
        }
        
        
        string expected = "SRI ERROR: no command parameters\n";
        
        REQUIRE( output == expected );
    }
    
    // =================================================================
    
    SECTION( "-incorrect parenthesis or spaces" )
    {
        string output1 = "";
        string output2 = "";
        string output3 = "";
        string output4 = "";
        string output5 = "success";
        
        string expected = "FACT ERROR: incorrect number of parenthesis in clause\n";
        
        string f1 = "FACT f(a,c";
        string f2 = "FACT fa,c)";
        string f3 = "FACT f((a,c)";
        string f4 = "FACT f(a,c))";
        string f5 = "FACT f (a,c)";
        
        try { engine.InterpretLine(f1); }
        catch (SRIException s) { output1 = s.what(); }
        
        try { engine.InterpretLine(f2); }
        catch (SRIException s) { output2 = s.what(); }
        
        try { engine.InterpretLine(f3); }
        catch (SRIException s) { output3 = s.what(); }
        
        try { engine.InterpretLine(f4); }
        catch (SRIException s) { output4 = s.what(); }
        
        try { engine.InterpretLine(f5); }
        catch (SRIException s) { output5 = s.what(); }
        
        REQUIRE( output1 == expected );
        REQUIRE( output2 == expected );
        REQUIRE( output3 == expected );
        REQUIRE( output4 == expected );
        REQUIRE( (output5 == expected || output5 == "") );
    }
    
    // =================================================================
    
    SECTION( "-incorrect commas" )
    {
        string output1 = "";
        string output2 = "";
        string output3 = "";
        
        string expected = "FACT ERROR: misplaced comma in clause\n";
        
        string f1 = "FACT f(a,)";
        string f2 = "FACT f(a,,c)";
        string f3 = "FACT f(,c)";
        
        try { engine.InterpretLine(f1); }
        catch (SRIException s) { output1 = s.what(); }
        
        try { engine.InterpretLine(f2); }
        catch (SRIException s) { output2 = s.what(); }
        
        try { engine.InterpretLine(f3); }
        catch (SRIException s) { output3 = s.what(); }
        
        REQUIRE( output1 == expected );
        REQUIRE( output2 == expected );
        REQUIRE( output3 == expected );
    }
    
    // =================================================================
    
    SECTION( "-no AND/OR within rule" )
    {
        string output = "";
        
        string expected = "RULE ERROR: no AND / OR specified\n";
        
        string r1 = "RULE f(a,b) a(b,c) b(c,d)";
        
        try { engine.InterpretLine(r1); }
        catch (SRIException s) { output = s.what(); }
        
        REQUIRE( output == expected );
    }
    
    // =================================================================
    
    SECTION( "-not enough rule parameters" )
    {
        string output = "";
        
        string expected = "RULE ERROR: invalid number of rule parameters\n";
        
        string r1 = "RULE f(a,b) AND";
        
        try { engine.InterpretLine(r1); }
        catch (SRIException s) { output = s.what(); }
        
        REQUIRE( output == expected );
    }
}

// =================================================================
// Edge Cases
// =================================================================

TEST_CASE( "Edge Cases" )
{
    SRI engine;
    
    string f1 = "FACT Father(Roger,John)";
    string f2 = "FACT Mother(Marry,John)";
    string f3 = "FACT Father(Roger,Albert)";
    string f4 = "FACT Mother(Marry,Albert)";
    string f5 = "FACT Father(Allen,Margret)";
    string f6 = "FACT Mother(Margret,Robert)";
    string f7 = "FACT Mother(Margret,Bob)";
    string r1 = "RULE Parent($X,$Y):- OR Father($X,$Y) Mother($X,$Y)";
    
    // =================================================================
    
    SECTION( "-no matching parameters" )
    {
        string r2 = "RULE GrandFather($X,$Z):- AND Father($X,$Z) Parent($Q,$Y)";
        
        engine.InterpretLine(f1);
        engine.InterpretLine(f2);
        engine.InterpretLine(f3);
        engine.InterpretLine(f4);
        engine.InterpretLine(f5);
        engine.InterpretLine(f6);
        engine.InterpretLine(f7);
        engine.InterpretLine(r1);
        engine.InterpretLine(r2);

        string simpleInference = "INFERENCE GrandFather($A,$B)";
        string output = engine.InterpretLine(simpleInference);
        
        string expected = "A:Roger, B:John\nA:Roger, B:Albert\nA:Allen, B:Margret\n";
        
        REQUIRE( output == expected );
    }
    
    // =================================================================
    
    SECTION( "-varying number of parameters 1" )
    {
        string f8 = "FACT a(b,c)";
        string f9 = "FACT a(b,c,d)";
        
        engine.InterpretLine(f8);
        engine.InterpretLine(f9);

        string simpleInference = "INFERENCE a($A,$B)";
        string output = engine.InterpretLine(simpleInference);

        string expected = "A:b, B:c\n";
        
        REQUIRE( output == expected );
    }
    
    // =================================================================
    
    SECTION( "-varying number of paraemters 2" )
    {
        string f8 = "FACT a(b,c)";
        string f9 = "FACT a(b,c,d)";
        
        engine.InterpretLine(f8);
        engine.InterpretLine(f9);

        string simpleInference = "INFERENCE a($A,$B,$C)";
        string output = engine.InterpretLine(simpleInference);
        
        string expected = "A:b, B:c, C:d\n";
        
        REQUIRE( output == expected );
    }
    
    // =================================================================
    
    SECTION( "-single parameter" )
    {
        string f8 = "FACT love(myself)";
        string f9 = "FACT love(you)";
        
        engine.InterpretLine(f8);
        engine.InterpretLine(f9);
        
        string simpleInference = "INFERENCE love($A)";
        string output = engine.InterpretLine(simpleInference);
        
        string expected = "A:myself\nA:you\n";
        
        REQUIRE( output == expected );
    }
    
    // =================================================================
    
    SECTION( "-two of same parameter" )
    {
        string r2 = "RULE GrandFather($X,$Y):- AND Father($X,$X) Parent($X,$Y)";
        string f8 = "FACT Father(Roger,Roger)";
        
        engine.InterpretLine(f1);
        engine.InterpretLine(f2);
        engine.InterpretLine(f3);
        engine.InterpretLine(f4);
        engine.InterpretLine(f5);
        engine.InterpretLine(f6);
        engine.InterpretLine(f7);
        engine.InterpretLine(f8);
        engine.InterpretLine(r1);
        engine.InterpretLine(r2);
        
        string simpleInference = "INFERENCE GrandFather($A,$B)";
        string output = engine.InterpretLine(simpleInference);
        
        string expected = "A:Roger, B:John\nA:Roger, B:Albert\nA:Roger, B:Roger\n";
        
        REQUIRE( output == expected );
    }
}
