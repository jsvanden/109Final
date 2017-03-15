#include <stdlib.h>

#include "Client.hpp"

#include "Utility.hpp"

#include <iostream>

using namespace std;
using namespace utility;

int main(int argc, char *argv[])
{
	Client client;

	string input;
	while (getline(cin, input))
    {
		if (input == "x" || input == "X" || input == "EXIT")
            break;

	    try
	    {
	    	string output = client.InterpretLine(input);
	        if (output != "NULL")
	        	cout << output;
	    }
	    catch (SRIException s)
	    {
	        cout << s.what();
	    }
    }

    client.DisconnectFromServer( string(), vector<string>() );

    return 0;
}
