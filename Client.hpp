/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Client Class to handl user input and interface with the server. *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef Client_hpp
#define Client_hpp

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

#include "ClientSocket.hpp"

class Client
{
	typedef std::vector<std::string> ListOfWords;

private:
	std::unordered_map< std::string, std::function<std::string(std::string, ListOfWords)> > commands;
	std::string SendRecieveOnce (std::string line, ListOfWords params);
	std::string ConnectToServer (std::string line, ListOfWords params);
    std::string SendFile(std::string line, ListOfWords params);
    std::string RecieveFile(std::string line, ListOfWords params);

	ClientSocket socket;

public:
	std::string InterpretLine (std::string& line);
	std::string DisconnectFromServer (std::string line, ListOfWords params);
	Client ();
};

#endif
