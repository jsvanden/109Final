#include "KnowledgeBase.hpp"
#include <iostream>

using namespace std;

void KnowledgeBase::AddFact (string name, vector<string> params)
{
    // DEBUGGING HELP
    cout << "Fact Name :  " << name << endl;
    cout << "Fact Paramaters :  ";
    for (auto i: params)
        cout << i << "  ";
    cout << endl;
    
    // YOUR CODE HERE
}
