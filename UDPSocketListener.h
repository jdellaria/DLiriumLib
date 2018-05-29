/*
 * UDPSocketListener.h
 *
 *  Created on: Apr 19, 2010
 *      Author: jdellaria
 */

#ifndef UDPSOCKETLISTENER_H_
#define UDPSOCKETLISTENER_H_


#include <sys/types.h>
#include <sys/socket.h>


class UDPSocketListener {
public:
	UDPSocketListener();
	virtual ~UDPSocketListener();
	int start(int portNumber, struct sockaddr_in *server);
	int ListenForMessage();
	int getMessage(char* const buffer, int bufferLength);
	int sendMessage(char* const buffer, int bufferLength, int connectionFD);
	int Close();

	   int socketFD;
//	   , length, fromlen, n;
	   struct sockaddr_in server;
	   struct sockaddr_in from;
};

#endif /* UDPSOCKETLISTENER_H_ */
