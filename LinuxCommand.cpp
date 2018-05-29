/*
 * LinuxCommand.cpp
 *
 *  Created on: Aug 10, 2009
 *      Author: jdellaria
 */


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "LinuxCommand.h"

LinuxCommand::LinuxCommand()
{
	programID = 0;
}

LinuxCommand::~LinuxCommand() {
	// TODO Auto-generated destructor stub
}


//int MP3Stream::ExecuteDecoder(char* const argv[], int *infd, int *outfd, int *errfd)
int LinuxCommand::Execute(const char*  darg[], char * returnBuffer, int returnBufferLength)
{
	/*main invoke a new child process
	 argv is an array exactly like the array passed to the main routine.
	 This should contain the following in order:

	 1.command to execute at the command line. Example, MPG123
	 2.any parameters. (-s)
	 3.name of music file
	 */

	int infds[2];
	int errfds[2];
	int outfds[2];
//	int pid;
	pipe(infds);
	pipe(outfds);
	pipe(errfds);
	programID = fork();
	if (programID < 0)
	{     /* fork a child process           */
		perror ("*** ERROR: LinuxCommand::Execute forking child process failed.\n");
		return(-1);
	}
	if(programID == 0)
	{
		// this is a child
		dup2(infds[1],1);// copy output pipe to standard output
		close(infds[0]);
		dup2(errfds[1],2);// copy output pipe to standard error
		close(infds[0]);
		dup2(outfds[0],0);// copy input pipe to standard input
		close(outfds[1]);
		execvp(*darg,(char* const*)darg); // this should not return
		exit(0);
	}
	else
	{
		int i;

		/* in parent */
		close(infds[1]);                /* parent closes p[1], stdout     */
		close(errfds[1]);
		close(outfds[1]);
		WaitForCommandToExit(); // we have to use this command to wait for the child process to complete.
		if (returnBufferLength > 1)
		{
//			char delimiters[] = " ";
//			char *result = NULL;
//			i = read(infds[0], returnBuffer, returnBufferLength-1); // get 1st 1023 bytes of the string returned from program that was executed
//			returnBuffer[i] = '\0';         /* string terminator     */
			i = read(errfds[0], returnBuffer, returnBufferLength-1); // get 1st 1023 bytes of the string returned from program that was executed
			returnBuffer[i] = '\0';         /* string terminator     */
//			i = read(outfds[0], returnBuffer, returnBufferLength-1); // get 1st 1023 bytes of the string returned from program that was executed
//			returnBuffer[i] = '\0';         /* string terminator     */
//			result = strtok(returnBuffer, delimiters);
		}
		close(infds[0]); //close stdin	otherwise we have a bunch of child processes forked.
		close(errfds[0]); //close errin
		close(outfds[0]); //close stdout
		return (programID);
	}
}

#ifdef JON
int LinuxCommand::Execute(const char*  darg[], char * returnBuffer, int returnBufferLength)
{
	int i;
	int p[2];
	int returnValue = 0;

//	pipe(p);
    if (pipe(p) == -1) /* setting up the pipe   */
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
	if ((programID = fork()) < 0)
	{     /* fork a child process           */
		perror ("*** ERROR: LinuxCommand::Execute forking child process failed.\n");
		return(-1);
	}
	else if (programID == 0) // child process:
	{
		dup2(p[1],STDOUT_FILENO); // copy output pipe to standard output.
		close(p[0]); // close stdin
		printf("*** programID = %d\n", programID);
		returnValue = execvp(*darg,(char* const*)darg);
		printf("*** LinuxCommand::Execute execvp errno=%d\n", errno);
		if (returnValue < 0)
		{     /* execute the command  */
			printf("*** ERROR: LinuxCommand::Execute execvp failed %s\n", darg[0]);
	    }
		close(p[1]);                   /* finished writing p[1] , stdout*/
	}
	else
	{
		printf("*** programID = %d\n", programID);
		/* in parent */
		close(p[1]);                /* parent closes p[1], stdout     */
		WaitForCommandToExit(); // we have to use this command to wait for the child process to complete.
		if (returnBufferLength > 1)
		{
			char delimiters[] = " ";
			char *result = NULL;
			i = read(p[0], returnBuffer, returnBufferLength-1); // get 1st 1023 bytes of the string returned from program that was executed

			returnBuffer[i] = '\0';         /* string terminator     */
			printf("RETURN_BUFFER: %s, returnBufferLength = %d i = %d\n",returnBuffer, returnBufferLength);
//			result = strtok(returnBuffer, delimiters);
		}


		close(p[0]); //close stdin								 // otherwise we have a bunch of child processes forked.
		return (programID);
	}
	return(returnValue);
}
#endif

int LinuxCommand::Stop()
{
	int status;

//	readFlag = 0;
	if(!programID) return 0;


	kill(programID,SIGTERM);
	WaitForCommandToExit();

	return 0;
}

int LinuxCommand::WaitForCommandToExit()
{
	int status;

	if(!programID) return 0;
	waitpid (programID, &status, 0); // performing a wait allows the system to release the resources associated with the child; if a wait is not performed, then terminated the child remains in a "zombie" state
	programID = 0;
	if (WIFEXITED(status) )
	{
		return 0;
	}
	else
	{
		printf("LinuxCommand::WaitForCommandToExit Decoder did NOT exited normally\n");
	}

	return 1;
}
