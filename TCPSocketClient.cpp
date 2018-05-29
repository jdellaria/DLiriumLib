/*
 * TCPSocketClient.cpp
 *
 *  Created on: Aug 10, 2009
 *      Author: jdellaria
 */


#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/select.h>

#include "TCPSocketClient.h"


TCPSocketClient::TCPSocketClient() {
	// TODO Auto-generated constructor stub
}

TCPSocketClient::~TCPSocketClient() {
	// TODO Auto-generated destructor stub
}

int TCPSocketClient::start(int portNumber, const char * hostName)
{
//	int n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFD < 0)
		perror("ERROR opening socket");
	server = gethostbyname(hostName);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
//		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
		 (char *)&serv_addr.sin_addr.s_addr,
		 server->h_length);
	serv_addr.sin_port = htons(portNumber);
	if (connect(socketFD,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0)
		perror("ERROR connecting");
	setNonBlocking(socketFD);
//	flags = 1;
//    ioctl(SocketFD, FIOBIO, &flags); // non Blocking
}



int TCPSocketClient::lookForConnection()
{
	int n;
	unsigned int index;
	/* set FDs for select() */
	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	n = 0;
	FD_SET(socketFD, &rfds);
	FD_SET(socketFD, &wfds);
	n = MAX(socketFD, n);
	/* set FDs for the connections */

	n = select(n + 1, &rfds, &wfds, (fd_set *) NULL, (struct timeval *) 0);
	if(n < 0)
		perror("select() failed...");
	return (n); // if n is zero there are no connections available.
}

int TCPSocketClient::isReadReady()
{
	return( FD_ISSET(socketFD, &rfds) );
}

int TCPSocketClient::isWriteReady()
{
	return( FD_ISSET(socketFD, &wfds) );
}

int TCPSocketClient::sendMessage(const char* buffer,int bufferLength)
{
	int n;

	n = write(socketFD,buffer, bufferLength);
	if (n < 0)
		 perror("ERROR writing to socket");
	return n;
}


int TCPSocketClient::sendMessage(std::string *thisString)
{
    int writeBytes;

    writeBytes = write(socketFD, thisString->c_str(), thisString->length() );
    if (writeBytes < 0)
    {
    	perror("ERROR writing to socket");
//		ERRMSG("TCPSocketListener::sendMessage ERROR writing to socket\n");
    }
    return (writeBytes);
}

int TCPSocketClient::getMessage(char* buffer, int maxLength)
{
	int n;
	bzero(buffer,maxLength);
	n = read(socketFD,buffer,maxLength);
//	if (n < 0)
//		 perror("ERROR reading from socket");
//	printf("Bytes Received %d TCPSocketClient::getMessage %s\n",n, buffer);
	return n;
}

int TCPSocketClient::getMessage(std::string *thisString)
{
	char character;
	int returnValue;

    while ((returnValue = getMessage(&character,1) ) > 0)
	{
    	thisString->push_back(character);
	}
	return thisString->length();
}


void TCPSocketClient::setNonBlocking(int socketFD)
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
int TCPSocketClient::Close()
{
	if (close(socketFD) != 0)
		printf("TCPSocketClient::Close() failed\n");
}
