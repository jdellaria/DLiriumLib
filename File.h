/*
 * File.h
 *
 *  Created on: Aug 26, 2009
 *      Author: jdellaria
 */

#ifndef FILE_H_
#define FILE_H_
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define REMOVEFILE "rm"
#define RENAMEFILE "mv"
#define LINKFILE "ln"
#define CHANGEPERMISSIONS "chmod"
#define CHANGEOWNER "chown"
#define COPY "cp"

using namespace std;

#ifndef OSX
	#define MD5HASH "md5sum"
#else
	#define MD5HASH "md5" //apple OSX
#endif

class File {
public:
	File();
	virtual ~File();
	int rename(const char* origName, const char* newName);
	int remove(const char* name);
	uint64_t size(const char* name);
	int changePermissions(const char*  name, const char* permissions);
	int changeOwnership(const char* name, const char* owner);
	int exist(const char* fileName);
	int exist(string fileName);
	int getMD5Hash(const char* name, char* hash);
	int copy(string& source, string& destination);
	int createHardLink(string fileName, string linkName);
	int createSoftLink(string fileName, string linkName);

	char errorMessageBuffer[2048];
};
#endif /* FILE_H_ */
