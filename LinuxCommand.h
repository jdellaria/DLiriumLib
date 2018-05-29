/*
 * LinuxCommand.h
 *
 *  Created on: Aug 10, 2009
 *      Author: jdellaria
 */

#ifndef LINUXCOMMAND_H_
#define LINUXCOMMAND_H_

#define ERRMSG(args...) fprintf(stderr,"ERR: " args)
#define INFMSG(args...) fprintf(stderr,"INFO: " args)
#define DBGMSG(args...) fprintf(stderr,"DBG: " args)

class LinuxCommand {
public:
	LinuxCommand();
	virtual ~LinuxCommand();

//	int Execute(char* const argv[], int *infd, int *outfd, int *errfd);
	int Execute(const char*  darg[], char * returnBuffer, int returnBufferLength);
	int Stop();
	int WaitForCommandToExit();

	int programID;
//	int inputFileDescriptor;
//	int outputFileDescriptor;
//	int errorFileDescriptor;
};

#endif /* LINUXCOMMAND_H_ */
