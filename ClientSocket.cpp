#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "ClientSocket.hpp"
#include "Utility.hpp"
#include <iostream>

using namespace std;
using namespace utility;

ClientSocket::ClientSocket (string hostName, int portNumber)
{
	sockfd = -1;
	Connect (hostName, portNumber);
}

ClientSocket::ClientSocket ()
{
	sockfd = -1;
}

ClientSocket::~ClientSocket ()
{
	Disconnect();
}

void ClientSocket::Disconnect ()
{
	close(sockfd);
	sockfd = -1;
}

void ClientSocket::Connect (string hostName, int portNumber)
{
	if (sockfd != -1) return;

	struct sockaddr_in serv_addr;
    struct hostent *server;

	int portno = portNumber;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	server = gethostbyname(hostName.c_str());

    if (server == NULL)
    {
    	sockfd = -1;
    	throw SRIException("SOCKET", "no such host");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
    	sockfd = -1;
        throw SRIException("SOCKET", "failed to connect");
    }
}

void ClientSocket::Update ()
{
	printf("Please enter the message: ");

    string myLine;

    getline(cin, myLine);

    Write(myLine);

    string readLine = Read();

    printf("%s\n", readLine.c_str());
}

string ClientSocket::Read()
{
	int n;
	char buffer[256];
	bzero(buffer,256);

	n = read(sockfd,buffer,255);

	if (n <= 0)
		throw SRIException("SOCKET", "failed to read from server");

    return buffer;
}

void ClientSocket::Write(string input)
{
	int n;
	char buffer[256];
	bzero(buffer,256);
	strcpy(buffer, input.c_str());

	n = write(sockfd,buffer,strlen(buffer));

	if (n < 0) 
        throw SRIException("SOCKET", "failed to write to server");
}

bool ClientSocket::IsConnected()
{
	return (sockfd != -1);
}






