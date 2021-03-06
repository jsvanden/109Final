/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Helper class to handle the find details of connecting to a server.  *
 * Modified from http://www.linuxhowtos.org/C_C++/socket.htm           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef ClientSocket_hpp
#define ClientSocket_hpp

#include <string>
#include <netinet/in.h>

class ClientSocket
{
private:
	int sockfd;

public:
	ClientSocket (std::string hostName, int portNumber);
	ClientSocket ();
	~ClientSocket ();
	std::string Read();
	void Write(std::string input);
	void Connect(std::string hostName, int portNumber);
	void Disconnect ();
	bool IsConnected();
};

#endif
