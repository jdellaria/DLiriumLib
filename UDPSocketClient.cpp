/*
 * UDPSocketClient.cpp
 *
 *  Created on: Apr 19, 2010
 *      Author: jdellaria
 */

#include <arpa/inet.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>


#include "UDPSocketClient.h"

UDPSocketClient::UDPSocketClient() {
	// TODO Auto-generated constructor stub

}

UDPSocketClient::~UDPSocketClient() {
	// TODO Auto-generated destructor stub
}


int UDPSocketClient::start(int portNumber, char * hostName)
{
	socketFD = socket(AF_INET,  SOCK_DGRAM, 0);
	if (socketFD < 0)
	{
//		ERRMSG("ERROR opening socket\n");
		perror("ERROR opening socket");
		return -1;
	}
	server.sin_family = AF_INET;
	hp = gethostbyname(hostName);
	if (hp == NULL)
	{
		perror("ERROR, no such host\n");
//		fprintf(stderr,"ERROR, no such host\n");
//		exit(0);
	}
	bcopy((char *)hp->h_addr,
		 (char *)&server.sin_addr.s_addr,
		 (size_t)hp->h_length);
//	bcopy((void * const )hp->h_addr, (void *)&server.sin_addr, (size_t)hp->h_length);
//	memcpy(&server.sin_addr,hp->h_addr,hp->h_length);
	server.sin_port = htons(portNumber);
}

int UDPSocketClient::sendMessage(char* const buffer,int bufferLength)
{
	int n;

	n = sendto(socketFD,buffer, bufferLength ,0,(sockaddr const*)&server,sizeof(struct sockaddr_in));
	{
		if (n < 0)
		{
	//		ERRMSG("ERROR writing to socket\n");
			perror("ERROR writing to socket");
		}
	}
	return n;

}

int UDPSocketClient::getMessage(char* const buffer, int maxLength)
{
	int n;
	socklen_t length;

	bzero(buffer,maxLength);
	n = recvfrom(socketFD,buffer,maxLength,0,(sockaddr *)&from, &length);
	if (n < 0)
	{
//		ERRMSG("ERROR reading from socket\n");
		perror("ERROR reading from socket");
	}
//	printf("Bytes Received %d TCPSocketClient::getMessage %s\n",n, buffer);
	return n;
}

int UDPSocketClient::Close()
{
	if (close(socketFD) != 0)
	{
//		ERRMSG("TCPSocketClient::Close() failed\n");
		perror("TCPSocketClient::Close() failed");
	}
}
