/*
 * TCPSocketListener.cpp
 *
 *  Created on: Aug 10, 2009
 *      Author: jdellaria
 */

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

#include "TCPSocketListener.h"

TCPSocketListener::TCPSocketListener() {
	// TODO Auto-generated constructor stub

}

TCPSocketListener::~TCPSocketListener() {
	// TODO Auto-generated destructor stub
}

int TCPSocketListener::start(int portNumber)
{
	int length;
	int set = 1;
	struct sockaddr_in server;

	socketFD=socket(AF_INET, SOCK_STREAM, 0);
	if (socketFD < 0)
	{
		perror("Opening socket");
//		ERRMSG("TCPSocketListener::start cannot create tcp socket: %s\n",strerror(errno));
		// similarly:
//		if (listen(s, 10) == -1) {
		    // this prints "an error: " + the error message from errno:
//		    printf("an error: %s\n", strerror(errno));

	}
	if(setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, (void *) &set, sizeof(set)) < 0)
	{
		perror("Can't set SO_REUSEADDR on main socket");
	}
	setNonBlocking(socketFD);
	length = sizeof(struct sockaddr);
	bzero(&server,length);
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(portNumber);
	if (bind(socketFD,(struct sockaddr *)&server,length)<0)
	{
		perror("binding");
//		ERRMSG("TCPSocketListener::start cannot bind tcp socket\n");

	}
    listen(socketFD,5);
	return (socketFD);
}

int TCPSocketListener::isNewConnection()
{
	return (FD_ISSET(socketFD, &rfds));
}

int TCPSocketListener::lookForConnections()
{
	int n;
	unsigned int index;
	/* set FDs for select() */
	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	n = 0;
	FD_SET(socketFD, &rfds);
	n = MAX(socketFD, n);
	/* set FDs for the connections */
	for(index = 0; index < connections.size(); index++)
	{
		FD_SET(connections[index].fd, &rfds);
		n = MAX(n, connections[index].fd);
		FD_SET(connections[index].fd, &wfds);
		n = MAX(n, connections[index].fd);
	}
	n = select(n + 1, &rfds, &wfds, (fd_set *) NULL, (struct timeval *) 0);
	if(n < 0)
		perror("select() failed...");
	return (n); // if n is zero there are no connections available.
}

int TCPSocketListener::isReadReady(int index)
{
	return( FD_ISSET(connections[index].fd, &rfds) );
}

int TCPSocketListener::isWriteReady(int index)
{
	return( FD_ISSET(connections[index].fd, &wfds) );
}

int TCPSocketListener::ListenForMessage(int bufferSize) // if connectionFD = 0 the we accept a new connection to the socket. Otherwise we read from the socket
{
    int NewSocketFD, clilen;


    clilen = sizeof(cli_addr);
    NewSocketFD = accept(socketFD,(struct sockaddr *) &cli_addr, (socklen_t*)&clilen);
	if (NewSocketFD < 0)
	{
		 perror("ERROR on accept");
	}
	addConnection(NewSocketFD, bufferSize);
	return (NewSocketFD);
}

int TCPSocketListener::getMessage(char* const buffer,int bufferLength, int connectionFD) // if connectionFD = 0 the we accept a new connection to the socket. Otherwise we read from the socket
{
    int readBytes;

    bzero(buffer,bufferLength);
    readBytes = read(connectionFD,buffer,bufferLength);
    if (readBytes < 0)
	{
//		perror("ERROR reading from socket");
//		ERRMSG("TCPSocketListener::getMessage ERROR reading from socket\n");
	}
    return (readBytes);
}

int TCPSocketListener::getMessage(std::string *thisString,int connectionFD)
{
	char character;
	int returnValue;

    while ((returnValue = getMessage(&character,1, connectionFD) ) > 0)
	{
    	thisString->push_back(character);
	}
	return thisString->length();
}

int TCPSocketListener::sendMessage(char* const buffer, int bufferLength, int connectionFD)
{
    int writeBytes;

    writeBytes = write(connectionFD, buffer, bufferLength );
    if (writeBytes < 0)
    {
    	perror("ERROR writing to socket");
//		ERRMSG("TCPSocketListener::sendMessage ERROR writing to socket\n");
    }
    return (writeBytes);
}

int TCPSocketListener::sendMessage(std::string *thisString, int connectionFD)
{
    int writeBytes;

    writeBytes = write(connectionFD, thisString->c_str(), strlen(thisString->c_str()) );
    if (writeBytes < 0)
    {
    	perror("ERROR writing to socket");
//		ERRMSG("TCPSocketListener::sendMessage ERROR writing to socket\n");
    }
    return (writeBytes);
}

int TCPSocketListener::addConnection(int connectionFD, int bufferSize)
{
	connectionStructure myConnections;

	setNonBlocking(connectionFD);
	myConnections.state  = CST_READ;
	myConnections.totalBytesSent = 0;
	myConnections.totalByestReceived = 0;
	myConnections.quitClient = 0;
	myConnections.clientCommands.clear();
	if((myConnections.buf =  (char*)malloc(bufferSize)) < 0)
		perror("Can't allocate memory");
	myConnections.fd = connectionFD;
	connections.push_back(myConnections);
}

void TCPSocketListener::closeConnection(int connectionNumber)
{
	close(connections[connectionNumber].fd);
	connections[connectionNumber].state = CST_FREE;
	free(connections[connectionNumber].buf);
	connections.erase(connections.begin() + connectionNumber);
}

bool TCPSocketListener::isConnected(int connectionNumber)
{
	unsigned bytestoread;
	int rc;
	fd_set fdRead;

	FD_ZERO(&fdRead);
	FD_SET(connections[connectionNumber].fd, &fdRead);
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 250000;
	rc = ::select(int(connections[connectionNumber].fd) + 1, &fdRead, (fd_set*) 0, (fd_set*) 0, &tv);
	ioctl(connections[connectionNumber].fd, FIONREAD, &bytestoread);
	printf("bytestoread:%d rc:%d\n",bytestoread,rc);
	return ((bytestoread == 0) && (rc == 1));
}

void TCPSocketListener::setNonBlocking(int socketFD)
{
	int opts;

	opts = fcntl(socketFD,F_GETFL);
	if (opts < 0)
	{
		perror("fcntl(F_GETFL)");
//		exit(EXIT_FAILURE);
	}
	opts = (opts | O_NONBLOCK);
	if (fcntl(socketFD,F_SETFL,opts) < 0)
	{
		perror("fcntl(F_SETFL)");
//		exit(EXIT_FAILURE);
	}
	return;
}

int TCPSocketListener::Close()
{
	if (close(socketFD) != 0)
	{
		perror("TCPSocketListener::Close() failed");
//		ERRMSG("TCPSocketListener::Close() failed\n"));
	}
}
