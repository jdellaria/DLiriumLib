/*
 * UDPSocketClient.h
 *
 *  Created on: Apr 19, 2010
 *      Author: jdellaria
 */

#ifndef UDPSOCKETCLIENT_H_
#define UDPSOCKETCLIENT_H_

#include <sys/types.h>
#include <sys/socket.h>

class UDPSocketClient {
public:
	UDPSocketClient();
	virtual ~UDPSocketClient();

	int start(int portNumber, char *hostName);
	int getMessage(char* const buffer, int maxLength);
	int sendMessage(char* const buffer,int bufferLength);
	int Close();

	int socketFD;
	struct sockaddr_in server;
	struct sockaddr_in from;
	struct hostent *hp;
};

#endif /* UDPSOCKETCLIENT_H_ */
