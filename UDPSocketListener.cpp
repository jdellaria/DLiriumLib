/*
 * UDPSocketListener.cpp
 *
 *  Created on: Apr 19, 2010
 *      Author: jdellaria
 */
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>

#include <string.h>
#include <unistd.h>

#include "UDPSocketListener.h"

UDPSocketListener::UDPSocketListener() {
	// TODO Auto-generated constructor stub

}

UDPSocketListener::~UDPSocketListener() {
	// TODO Auto-generated destructor stub
}


int UDPSocketListener::start(int portNumber, struct sockaddr_in *server)
{
	int length;

	socketFD=socket(AF_INET, SOCK_DGRAM, 0);
	if (socketFD < 0)
	{
		perror("Opening socket");
//		ERRMSG("TCPSocketListener::start cannot create tcp socket: %s\n",strerror(errno));
		// similarly:
//		if (listen(s, 10) == -1) {
		    // this prints "an error: " + the error message from errno:
//		    printf("an error: %s\n", strerror(errno));

	}
	length = sizeof(struct sockaddr);
	bzero(server,length);
	server->sin_family=AF_INET;
	server->sin_addr.s_addr=INADDR_ANY;
	server->sin_port=htons(portNumber);
	if (bind(socketFD,(struct sockaddr *)server,length)<0)
	{
		perror("binding");
//		ERRMSG("TCPSocketListener::start cannot bind tcp socket\n");

	}
//    listen(socketFD,5);
	return (socketFD);
}

int UDPSocketListener::ListenForMessage() // if connectionFD = 0 the we accept a new connection to the socket. Otherwise we read from the socket
{
    int n, clilen;
//    struct sockaddr_in cli_addr;
//    char str[INET_ADDRSTRLEN+1];

//    clilen = sizeof(cli_addr);
 //   n = recvfrom(socketFD,buf,1024,0,(struct sockaddr *)&from,&fromlen);
//    NewSocketFD = accept(socketFD,(struct sockaddr *) &cli_addr, (socklen_t*)&clilen);
//    printf("DLiriumLib address is %s\n",inet_ntoa(cli_addr.sin_addr));
//    inet_ntop(AF_INET, &(cli_addr.sin_addr), str, INET_ADDRSTRLEN);
//    printf("%s\n", str); // prints "192.0.2.33"
//
	return (n);
}

int UDPSocketListener::getMessage(char* const buffer,int bufferLength) // if connectionFD = 0 the we accept a new connection to the socket. Otherwise we read from the socket
{
    int readBytes;
    int fromlen;

    bzero(buffer,bufferLength);
    readBytes = recvfrom(socketFD,buffer,bufferLength,0,(struct sockaddr *)&from, (socklen_t*)&fromlen);

//    readBytes = read(connectionFD,buffer,bufferLength);
    if (readBytes < 0)
	{
		perror("ERROR reading from socket");
//		ERRMSG("TCPSocketListener::getMessage ERROR reading from socket\n");
	}
    return (readBytes);
}

int UDPSocketListener::sendMessage(char* const buffer, int bufferLength, int connectionFD)
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

int UDPSocketListener::Close()
{
	if (close(socketFD) != 0)
	{
		perror("TCPSocketListener::Close() failed");
//		ERRMSG("TCPSocketListener::Close() failed\n"));
	}
}
