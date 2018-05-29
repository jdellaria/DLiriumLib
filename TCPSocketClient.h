/*
 * TCPSocketClient.h
 *
 *  Created on: Aug 10, 2009
 *      Author: jdellaria
 */

#ifndef TCPSOCKETCLIENT_H_
#define TCPSOCKETCLIENT_H_


#include <stdio.h>
#include <string.h>
#include <iostream>
#include <unistd.h>

#define CST_FREE 0
#define CST_READ 1
#define CST_WRITE 2
#define MAX(x,y) ((x) > (y) ? (x) : (y))

typedef struct {
//  int fd;
	unsigned int bytesExpecting;
	unsigned int totalByestReceived;
	unsigned int totalBytesSent;
	char state;
	char nextState;
	char *buf;
	std::string stringBuf;
} connectionStructure;

class TCPSocketClient {
public:
	TCPSocketClient();
	virtual ~TCPSocketClient();

	int start(int portNumber, const char *hostName);
	int getMessage(char* buffer, int maxLength);
	int getMessage(std::string *thisString);
	int sendMessage(const char* buffer,int bufferLength);
	int sendMessage(std::string *thisString);
	int isWriteReady();
	int isReadReady();
	int lookForConnection();
	void setNonBlocking(int socketFD);
	int Close();

	int socketFD;

	connectionStructure connection;
//    struct sockaddr_in cli_addr;
    fd_set rfds, wfds;
};
#endif /* TCPSOCKETCLIENT_H_ */
