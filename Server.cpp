#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>

#include <thread>
#include <iostream>

#include "Server.hpp"
#include "SRI.hpp"

using namespace std;
using namespace utility;

Server::Server (int portNumber)
{
	struct sockaddr_in serv_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

     if (sockfd < 0) 
        throw SRIException("SOCKET", "failed to open socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = portNumber;
	serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (::bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        throw SRIException("SOCKET", "failed to bind to address");

    listen(sockfd,5);
    clilen = sizeof(cli_addr);
}

Server::~Server ()
{
    close (sockfd);
}

void Server::Update ()
{
	int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

	if (newsockfd < 0)
		throw SRIException("SOCKET", "failed to accept connection");

	std::thread newConnection (&Server::ConnectionBody, this, newsockfd);
    newConnection.detach();
}

string Server::Read(int socket)
{
    int n;
    char buffer[256];
    bzero(buffer,256);

    n = read(socket,buffer,255);

    if (n <= 0) 
        throw SRIException("SOCKET", "failed to read from socket");

    printf ("Read of size: %d\n", n);

    return buffer;
}

void Server::Write(int socket, string input)
{
    int n;
    char buffer[256];
    bzero(buffer,256);
    strcpy(buffer, input.c_str());

    n = write(socket,buffer,strlen(buffer));

    if (n < 0) 
        throw SRIException("SOCKET", "failed to write to socket");
}


void Server::ConnectionBody (int sock)
{
    SRI engine;

    string clientMessage;
    string serverMessage;

    while (true)
    {
        try
        {
            clientMessage = Read(sock);
        }
        catch (SRIException s)
        {
            printf("%s\n", s.what());
            break;
        }

        if (clientMessage == "DISCONNECT")
        {
            Write (sock, "NULL");
            break;
        }

        if (clientMessage == "PRINTFILE")
        {
            string saveCommand = "DUMP sample.sri";
            serverMessage = engine.InterpretLine(saveCommand);

            vector<string> lines = StringToVector(serverMessage, ';');

            for (auto line : lines)
            {
                if (line != "")
                {
                    Write(sock, line);
                    Read(sock);
                }
            }

            Write(sock, "ENDPRINT");
            
            continue;
        }

        try
        {
            serverMessage = engine.InterpretLine(clientMessage);
        }
        catch (SRIException s)
        {
            printf("%s\n", s.what());
            serverMessage = s.what();
        }

        if (serverMessage == "")
            serverMessage = "NULL";

        try
        {
            Write(sock, serverMessage);
        }
        catch (SRIException s)
        {
            printf("%s\n", s.what());
            break;
        }
    }

    printf("Connection Ended\n");

    close (sock);
}

