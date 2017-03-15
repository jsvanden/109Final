#include "Client.hpp"
#include "Utility.hpp"
#include <iostream>
#include <fstream>

using namespace std;
using namespace utility;
using namespace placeholders;

// ==================================================================================
// CLIENT
//     -Constructor to set up string -> function interface.
// ==================================================================================

Client::Client ()
{
	commands.insert(make_pair("FACT", bind (&Client::SendRecieveOnce, this, _1, _2)));
	commands.insert(make_pair("RULE", bind (&Client::SendRecieveOnce, this, _1, _2)));
	commands.insert(make_pair("INFERENCE", bind (&Client::SendRecieveOnce, this, _1, _2)));
	commands.insert(make_pair("DROP", bind (&Client::SendRecieveOnce, this, _1, _2)));
	commands.insert(make_pair("DISCONNECT", bind (&Client::DisconnectFromServer, this, _1, _2)));
	commands.insert(make_pair("CONNECT", bind (&Client::ConnectToServer, this, _1, _2)));
    commands.insert(make_pair("LOAD", bind (&Client::SendFile, this, _1, _2)));
    commands.insert(make_pair("DUMP", bind (&Client::RecieveFile, this, _1, _2)));
}

// ==================================================================================
// INTERPRET LINE
//     -Takes line and calls corresponding function.
// ==================================================================================

string Client::InterpretLine(string& line)
{
    // Remove invlaid characters and seperate input string by spaces.
    MakeValid(line);
    
    // If line is empty or only whitespace, stop processing.
    if ( line == "" || line.find_first_not_of(' ') >= line.length())
        return "NULL";

    // Split filtered input string by spaces.
    vector<string> words = StringToVector(line, ' ');
    
    // If first word of input string is not a recognized command, throw an error.
    auto commandFunction = commands.find (words[0]);
    if ( commandFunction == commands.end() )
        throw SRIException("INTERPRETLINE", "no matching command");
    
    // Delete first word of input string (the command).
    words.erase(words.begin());

    // Call the command's corresponding function.
    string output = commandFunction->second(line, words);
    
    return output;
}

// ==================================================================================
// RECIEVE FILE
//     -Tells the server to send the file line by line and listens for file. 
// ==================================================================================

string Client::RecieveFile(string line, vector<string> params)
{
    if (params.size() != 1)
        throw SRIException("RECIEVEFILE", "invalid number of parameters");
    
    string filePath = params[0];
    ofstream outfile;
    
    // Ensure created file has ".sri" extension.
    
    int extensionIdx = (int) filePath.find_last_of('.');
    
    if( extensionIdx == -1 || filePath.substr(extensionIdx) != ".sri")
    {
        filePath += ".sri";
    }
    
    outfile.open( filePath );
    
    if( !outfile.is_open() )
    {
        throw SRIException("RECIEVEFILE", "could not DUMP file \"" + filePath + "\"");
    }
    
    if( !socket.IsConnected() )
    {
        throw SRIException("RECIEVEFILE", "client is not connected to a server");
    }
    
    socket.Write("PRINTFILE");
    
    string output;
    
    while (true)
    {
        string line = socket.Read();
        
        if (line == "ENDPRINT")
            break;
        
        output += (line + "\n");
        
        socket.Write("NULL");
    }
    
    outfile << output;
    
    outfile.close();
    
    return "NULL";
}

// ==================================================================================
// SEND FILE
//     -Sends a file line by line to the server.
// ==================================================================================

string Client::SendFile(string line, vector<string> params)
{
    if( !socket.IsConnected() )
    {
        throw SRIException("SENDFILE", "client is not connected to a server");
    }
    
    string filePath = params[0];
    string currentLine;
    ifstream infile;
    
    // Ensure that ONLY files with extension ".sri" are loaded.
    
    int extensionIdx = (int) filePath.find_last_of('.');
    if( extensionIdx == -1 || filePath.substr(extensionIdx) != ".sri")
    {
        throw SRIException("LOAD", "infile must have extension \".sri\"");
    }
    
    // Open file for reading and add its contents, or print an error.
    
    infile.open( filePath );
    
    if( infile.is_open() )
    {
        // Load input via SRI's InterpretLine.
        while( getline(infile, currentLine) )
        {
            SendRecieveOnce(currentLine, vector<string>());
        }
        
        infile.close();
    }
    
    // If the file could not be opened, throw an error.
    else throw SRIException("LOAD", "could not LOAD from file \"" + filePath + "\"");

    return "NULL";
}

// ==================================================================================
// SEND RECIEVE ONCE
//     -Send a message the server and return the response.
// ==================================================================================

string Client::SendRecieveOnce(string line, vector<string> params)
{
	if( !socket.IsConnected() )
	{
		throw SRIException("SENDRECIEVE", "client is not connected to a server");
	}

	string response;

	try
    {
    	socket.Write(line);
    	response = socket.Read();
    }
    catch (SRIException s)
    {
        s.SetLocation("SENDRECIEVE");
        throw s;
    }

	return response;
}

// ==================================================================================
// CONNECT TO SERVER
//     -Send a connection request to a server.
// ==================================================================================

string Client::ConnectToServer(string line, vector<string> params)
{
	if( socket.IsConnected() )
	{
		throw SRIException("CONNECT", "already connected to server");
	}
	
	if (params.size() != 2)
		throw SRIException("CONNECT", "invlaid number of parameters");

	string hostName = params[0];
	int portNumber = stoi(params[1]);

	try
    {
        socket.Connect(hostName, portNumber);
    }
    catch (SRIException s)
    {
        s.SetLocation("CONNECT");
        throw s;
    }

	return "Connected Successfully\n";
}

// ==================================================================================
// DISCONNECT FROM SERVER
//     -Send a disconnect request from the server.
// ==================================================================================

string Client::DisconnectFromServer(string line, vector<string> params)
{
	if( !socket.IsConnected() )
	{
		throw SRIException("CLIENT", "no connection to disconnect from");
	}

	try
    {
    	socket.Write("DISCONNECT");
    }
    catch (SRIException s)
    {
        s.SetLocation("DISCONNECT");
        throw s;
    }
    
    try
    {
        socket.Read();
        socket.Disconnect();
    }
    catch (SRIException s)
    {
        // If the Read fails, we still want to try to disconnect.
        try
        {
            socket.Disconnect();
        }
        catch (SRIException s)
        {
            s.SetLocation("DISCONNECT");
            throw s;
        }

        s.SetLocation("DISCONNECT");
        throw s;
    }
    
   
	return "Disconnected Successfully\n";
}


