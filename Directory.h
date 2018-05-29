/*
 * Directory.h
 *
 *  Created on: Aug 10, 2009
 *      Author: jdellaria
 */

#ifndef DIRECTORY_H_
#define DIRECTORY_H_

#include <iostream>
#include <cstdlib>
#include <sys/dir.h>
#include <errno.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>


#define FULLNAMESIZE 1024*10
#define CREATEDIRECTORY "mkdir"
#define REMOVEDIRECTORY "rm"

#define		DIRECTORYENTRYTYPE_DIR  0 // - Is file a directory
#define		DIRECTORYENTRYTYPE_LNK  1 // - Is file a symbolic link
#define		DIRECTORYENTRYTYPE_REG  2 // - Is file a regular file
#define		DIRECTORYENTRYTYPE_CHR  3 // - Is file a character device
#define		DIRECTORYENTRYTYPE_BLK  4 // - Is file a block device
#define		DIRECTORYENTRYTYPE_FIFO 5 // - Is file a FIFO
#define		DIRECTORYENTRYTYPE_SOCK 6 // - Is file a unix socket

using namespace std;

class Directory {
public:
	Directory();
	virtual ~Directory();

	int Get(const char* startDirectory, void (*callBack)(const char* directoryEntry, int directoryEntryType),int recurse);
	int Recurse(const char* startDirectory, void (*callBack)(const char* directoryEntry, int directoryEntryType));
	int Create(const char* name);
	void CreateFromFileName (string& fullFileName);
	int Remove(const char* name);

	char fullPathName[FULLNAMESIZE];
	struct stat finfo;		/* The information about this file. */
	char errorMessageBuffer[2048];
};

#endif /* DIRECTORY_H_ */
