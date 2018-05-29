/*
 * Audio.cpp
 *
 *  Created on: Aug 10, 2009
 *      Author: jdellaria
 */

#include "Audio.h"
#include "LinuxCommand.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <linux/soundcard.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

Audio::Audio() {
	// TODO Auto-generated constructor stub
	inputFileDescriptor = 0;
	outputFileDescriptor = 0;
	errorFileDescriptor = 0;
	programID=0;

	reset = 0;
	speed = 44100;
	channels = 2;
	fmt = AFMT_S16_LE;
//	OpenAudioDevice();
//	ConfigureAudioDevice();
	memset (&musicBuffer, 0, sizeof(musicBuffer)); // clear buffer
	musicBufferIndex = 0;
	readFlag=1;
}

Audio::~Audio() {
	// TODO Auto-generated destructor stub
//	close(audioFileDescriptorOutput);
}

int Audio::AddToMusicBuffer(char *music, int length)
{
	int buflen;
	buflen = musicBufferIndex + length;
	if (readFlag != 1)
		return -1;
	if(buflen >= MUSICBUFFERLENGTH - MAXREADSIZE)
	{
		/* if we are approaching the max size of the write buffer, then stop reading the information
		 * coming from the stream. The calling program must check the readFlag to make sure that we
		 * have enough storage space before reading.
		 */
		readFlag = 0;
//		printf("Audio::AddToMusicBuffer  Audio::readFlag = %d:\n",readFlag);
	}
	if(buflen >= MUSICBUFFERLENGTH)
	{
		printf("Audio::AddToMusicBuffer overflow  Audio::musicBufferIndex = %d:\n",musicBufferIndex);
		return -1;
	}

	memcpy(musicBuffer + musicBufferIndex, music, length);
	musicBufferIndex += length;
	return musicBufferIndex;
}

int Audio::WriteMusicBuffer()
{
	int writeLen;

	if (musicBufferIndex == 0)
	{
		readFlag = 1;
		return musicBufferIndex;
	}
	writeLen = write (audioFileDescriptorOutput,musicBuffer,musicBufferIndex);
	musicBufferIndex = 0;
	readFlag = 1;
	if(writeLen<0){
		ERRMSG("%s: write error: %s\n", __func__, strerror(errno));
		return -1;
	}
	if(writeLen==0){
		INFMSG("%s: write, disconnected on the other end\n", __func__);
		return -1;
	}
	return musicBufferIndex;
}

int Audio::ReadAudioStream(int fdin)
{
	int readLen;
	int returnValue;
	char mychar[MAXREADSIZE];

	if (readFlag != 1)
		return -1;
	readLen = read (fdin,mychar,256);
	if(readLen<0) ERRMSG("%s: read error: %s\n", __func__, strerror(errno));
	if(readLen==0) INFMSG("%s: read, disconnected on the other end\n", __func__);
	returnValue = AddToMusicBuffer(mychar,readLen);
	if (returnValue < 0)
		return returnValue;
	return readLen;
}

int Audio::OpenAudioDevice()
{
	audioFileDescriptorOutput = open ("/dev/dsp",O_WRONLY); // Write only seems to only work when using the select() statement
	if (audioFileDescriptorOutput == -1)
	{
		perror("Cannot open output file\n");
		exit(1);
	}
	return audioFileDescriptorOutput;
}

int Audio::ConfigureAudioDevice()
{
	printf("Audio::ConfigureAudioDevice fdout %d:\n",audioFileDescriptorOutput);
	ioctl(audioFileDescriptorOutput, SNDCTL_DSP_RESET, &reset);
	ioctl(audioFileDescriptorOutput, SNDCTL_DSP_SETFMT, &fmt);
	ioctl(audioFileDescriptorOutput, SNDCTL_DSP_CHANNELS, &channels);
	ioctl(audioFileDescriptorOutput, SNDCTL_DSP_SPEED, &speed);
}

int Audio::ExecuteDecoder(char* const argv[], int *infd, int *outfd, int *errfd)
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
	int pid;
	pipe(infds);
	pipe(outfds);
	pipe(errfds);
	pid=fork();
	if(pid==0)
	{
		// this is a child
		if(infd){
			dup2(infds[1],1);// copy output pipe to standard output
		}else{
			close(infds[1]);
		}
		close(infds[0]);

		if(errfd){
			dup2(errfds[1],2);// copy output pipe to standard error
		}else{
			close(errfds[1]);
		}
		close(infds[0]);

		if(outfd){
			dup2(outfds[0],0);// copy input pipe to standard input
		}else{
			close(outfds[0]);
		}
		close(outfds[1]);

		execvp(argv[0], argv);
		exit(0);
	}

	if(infd)
	{
		*infd=infds[0];
		inputFileDescriptor = infds[0];
//		CurrentlyPlaying <<
	}
	else
		close(infds[0]);
	close(infds[1]);//close output pipe

	if(errfd)
	{
		*errfd=errfds[0];
		errorFileDescriptor = errfds[0];
	}
	else
		close(errfds[0]);
	close(errfds[1]);//close output pipe

	if(outfd)
	{
		*outfd=outfds[1];
		outputFileDescriptor=outfds[1];
	}
	else
		close(outfds[1]);
	close(outfds[0]);// close read pipe
	programID = pid;
	readFlag = 1;
	return pid;
}

int Audio::StopDecoder()
{
	int status;

//	readFlag = 0;
	if(!programID) return 0;

//	printf("Audio::StopDecoder killing programID %d:\n",programID);
	kill(programID,SIGTERM);
	usleep(1000);
	WaitForDecoderToExit();

	return 0;
}

int Audio::WaitForDecoderToExit()
{
	int status;

//	readFlag = 1;
	if(!programID) return 0;
	waitpid (programID, &status, 0); // performing a wait allows the system to release the resources associated with the child; if a wait is not performed, then terminated the child remains in a "zombie" state
	programID = 0;
	if (WIFEXITED(status) )
	{
//		printf("Audio::StopDecoder Decoder exited normally\n");
	}
	else
	{
		printf("Audio::StopDecoder Decoder did NOT exited normally\n");
	}

	return 0;
}


// The following command will adjust the volume to the specified percentage level
int Audio::Volume(int percentage)
{
	LinuxCommand myCommand;
	char asciiNumber[5];
	const char *darg[5]={SETVOLUME,"set","Master", NULL, NULL}; // -p option makes parent directories as needed

	if(percentage <= 100)
	{
		sprintf(asciiNumber,"%d%%",percentage);
		darg[3] = asciiNumber;
		myCommand.Execute(darg,NULL,0);
		return (1);
	}
	return(0);
}
