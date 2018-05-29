/*
 * Audio.h
 *
 *  Created on: Aug 10, 2009
 *      Author: jdellaria
 */

#ifndef AUDIO_H_
#define AUDIO_H_


#define MUSICBUFFERLENGTH 2048
#define MAXREADSIZE 256
#define ERRMSG(args...) fprintf(stderr,"ERR: " args)
#define INFMSG(args...) fprintf(stderr,"INFO: " args)
#define DBGMSG(args...) fprintf(stderr,"DBG: " args)
#define SETVOLUME "amixer"
class Audio {
public:
	Audio();
	virtual ~Audio();
	int ExecuteDecoder(char* const argv[], int *infd, int *outfd, int *errfd);
	int StopDecoder();
	int ConfigureAudioDevice();
	int OpenAudioDevice();
	int AddToMusicBuffer(char *music, int length);
	int WriteMusicBuffer();
	int ReadAudioStream(int fdin);
	int WaitForDecoderToExit();
	int Volume(int percentage);

//	String CurrentlyPlaying;
	int inputFileDescriptor;
	int outputFileDescriptor;
	int errorFileDescriptor;
	int audioFileDescriptorOutput;
	int programID;
	char musicBuffer[MUSICBUFFERLENGTH];
	int musicBufferIndex;
	int readFlag;
	int reset;
	int speed;
	int channels;
	int fmt;

};

#endif /* AUDIO_H_ */
