/* * * * * * * * * * * * * * * * * * * *
 *   *
 * * * * * * * * * * * * * * * * * * * */

#ifndef Server_hpp
#define Server_hpp

#include <netinet/in.h>
#include <string>

class Server
{
private:
	int sockfd;
	int portno;
	socklen_t clilen;
	struct sockaddr_in cli_addr;
	void ConnectionBody(int sock);

public:
	Server (int portNumber);
	~Server();
	void Update();
	std::string Read(int socket);
	void Write(int socket, std::string input);
};

#endif
