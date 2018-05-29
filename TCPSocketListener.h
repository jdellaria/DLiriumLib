/*
 * TCPSocketListener.h
 *
 *  Created on: Aug 10, 2009
 *      Author: jdellaria
 */

#ifndef TCPSOCKETLISTENER_H_
#define TCPSOCKETLISTENER_H_
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <fcntl.h>
#include <vector>
#include <string.h>
#include <sys/ioctl.h>
#include <inttypes.h>

#define CST_FREE 0
#define CST_READ 1
#define CST_WRITE 2

typedef struct command_Data
{
    std::string clientCommand;
    int nextState;
    std::string fileName;
    std::string md5Hash;
    std::string fileType;
    off64_t fileSize;
    off64_t fileOffset;
} commandData;

typedef struct {
  int fd;
  unsigned int bytesExpecting;
  unsigned int totalByestReceived;
  unsigned int totalBytesSent;
  off64_t fileSize;
  off64_t fileOffset;
  int quitClient;
  char state;
  char nextState;
  char *buf;
  std::string stringBuf;
  std::vector<commandData> clientCommands;
} connectionStructure;

//#define READ_BUFFER_SIZE 512
#define MAX(x,y) ((x) > (y) ? (x) : (y))

class TCPSocketListener
{
public:
	TCPSocketListener();
	virtual ~TCPSocketListener();
	int start(int portNumber);
	int ListenForMessage(int bufferSize);
	void setnonblocking(int fd);
	int getMessage(char* const buffer, int bufferLength, int connectionFD);
	int getMessage(std::string *thisString,int connectionFD);
	int sendMessage(char* const buffer, int bufferLength, int connectionFD);
	int sendMessage(std::string *thisString, int connectionFD);
	void setNonBlocking(int socketFD);
	int addConnection(int connectionFD, int bufferSize);
	void closeConnection(int connectionNumber);
	int lookForConnections();
	int isNewConnection();
	bool isConnected(int connectionNumber);
	int isReadReady(int index);
	int isWriteReady(int index);
	int Close();

	int socketFD;
	std::vector<connectionStructure> connections;
    struct sockaddr_in cli_addr;
    fd_set rfds, wfds;
};


#endif /* TCPSOCKETLISTENER_H_ */
