/*
 * File.cpp
 *
 *  Created on: Aug 26, 2009
 *      Author: jdellaria
 */


#include "File.h"
#include "md5.h"


#include "LinuxCommand.h"
#include "Directory.h"

File::File() {
	// TODO Auto-generated constructor stub

}

File::~File() {
	// TODO Auto-generated destructor stub
}

int File::rename(const char* origName, const char* newName)
{
	LinuxCommand myCommand;
	const char *darg[4]={RENAMEFILE,NULL, NULL,NULL};
	darg[1] = origName;
	darg[2] = newName;
	myCommand.Execute(darg,errorMessageBuffer,sizeof(errorMessageBuffer)-2);
	if (strlen(errorMessageBuffer) > 0)
	{
		return (-1); // on error errorMessageBuffer has the error string
	}
	else
	{
		return (0);
	}
}

int File::remove(const char* name)
{
	LinuxCommand myCommand;
	const char *darg[4]={REMOVEFILE,NULL, NULL,NULL};
	darg[1] = name;

	myCommand.Execute(darg,errorMessageBuffer,sizeof(errorMessageBuffer)-2);
	if (strlen(errorMessageBuffer) > 0)
	{
		return (-1); // on error errorMessageBuffer has the error string
	}
	else
	{
		return (0);
	}
}

uint64_t File::size(const char* name)
{
	FILE *f;
	uint64_t size = -1;

	f = fopen64( name, "rb" );
    if (f > 0)
    {
    	fseeko64( f, 0L, SEEK_END );
    	size = ftello64( f );
    	fclose( f );
    }
 //   printf("File size 2 = %llu\n", i);
	return (size);
}

int File::changePermissions(const char* name, const char* permissions)
{
	LinuxCommand myCommand;
	const char *darg[5]={CHANGEPERMISSIONS,NULL, NULL, NULL, NULL};

	darg[2] = name;
	darg[1] = permissions;

	myCommand.Execute(darg,errorMessageBuffer,sizeof(errorMessageBuffer)-2);
	if (strlen(errorMessageBuffer) > 0)
	{
		return (-1); // on error errorMessageBuffer has the error string
	}
	else
	{
		return (0);
	}
}

int File::changeOwnership(const char* name, const char* owner)
{
	LinuxCommand myCommand;
	const char *darg[5]={CHANGEOWNER,NULL, NULL, NULL, NULL};

	darg[2] = name;
	darg[1] = owner;

	myCommand.Execute(darg,errorMessageBuffer,sizeof(errorMessageBuffer)-2);
	if (strlen(errorMessageBuffer) > 0)
	{
		return (-1); // on error errorMessageBuffer has the error string
	}
	else
	{
		return (0);
	}
}


int File::copy(string& source, string& destination)
{

	LinuxCommand myCommand;
	const char *darg[5]={COPY,"-p", NULL, NULL, NULL}; 	// -p preserve the specified attributes (default:
													//mode,ownership,timestamps), if possible
													//additional attributes: context, links

	darg[2] = source.c_str();
	darg[3] = destination.c_str();

	Directory myDir;
	myDir.CreateFromFileName(destination);
	myCommand.Execute(darg,errorMessageBuffer,sizeof(errorMessageBuffer)-2);

	if (strlen(errorMessageBuffer) > 0)
	{
		return (-1); // on error errorMessageBuffer has the error string
	}
	else
	{
		return (0);
	}
}

int File::exist(const char* fileName)
{
	FILE *fp;

	fp = fopen(fileName, "r");
	if (fp == NULL)
	{
		return 0; // file does not exist
	}
	fclose(fp);
	return 1;
}

int File::exist(string fileName)
{
	return (exist(fileName.c_str()));
}

//The ln command lets a file on your disk be accessed with more than one file name by hard-linking a
//new file name to it. When you hard-link a file, you create a new file name and connect it with the original file.
//Hard-linking can let two users share the same file or provide a more convenient way of referencing files buried deep in a series of directories.

//Here's an example. Suppose hermie and sigmund are working on the Top Secret Snowgun Project, and
//the formula for the Snowgun is stored in /home/hermie/projects/snowgun/formula.txt. Sigmund doesn't
//want to type that long, ugly file name every time he needs to update the file, so he creates a hard link to the file, like this:

//ln /home/hermie/projects/snowgun/formula.txt sgformula

//The preceding command links the file name sgformula to the file formula.txt contained at the end of that directory
//string. There's still only one copy of the snowgun formula (formula.txt) on the disk, but now sigmund can access it
//quickly with the name sgformula (assuming hermie gives him write permission to the file).
//Interestingly, if hermie deletes his projects/snowgun/formula.txt file, Linux will not remove the file from the file system
//because sigmund still has a link to it.

//Think of the ln command as a way of creating a kind of nickname for a file.
//By the way, there is no parallel to this file linking concept in the DOS world--you'd have to create a second copy of
//the file and keep the various copies in synch manually to get a similar result.
int File::createHardLink(string fileName, string linkName) //When you use link files, it helps us to reduce the disk space by having single copy of the original file and ease the administration tasks as the modification in original file reflects in other places. If the original is deleted, the file is still accessable via the hard link
{
	LinuxCommand myCommand;
	const char *darg[4]={LINKFILE,NULL, NULL, NULL};
	darg[1] = fileName.c_str();
	darg[2] = linkName.c_str();

	myCommand.Execute(darg,errorMessageBuffer,sizeof(errorMessageBuffer)-2);
	if (strlen(errorMessageBuffer) > 0)
	{
		return (-1); // on error errorMessageBuffer has the error string
	}
	else
	{
		return (0);
	}
}

//Symbolic links (soft links) (also called symlinks) work like hard links,
//but you can do a bit more with them. If you want to create a link to a directory (as opposed to a file),
//you must create a symlink. Symlinks are also required when linking to a file on a different disk partition or on a network.
int File::createSoftLink(string fileName, string linkName) //When the original file referred by a soft-link is deleted, the soft link will be broken
{
	LinuxCommand myCommand;
	const char *darg[5]={LINKFILE,"-s",NULL, NULL, NULL};
	darg[2] = fileName.c_str();
	darg[3] = linkName.c_str();

	myCommand.Execute(darg,errorMessageBuffer,sizeof(errorMessageBuffer)-2);
	if (strlen(errorMessageBuffer) > 0)
	{
		return (-1); // on error errorMessageBuffer has the error string
	}
	else
	{
		return (0);
	}
}

int File::getMD5Hash(const char*  name, char* hash) // hash has to be at least 34 bytes
{
	LinuxCommand myCommand;
#ifndef OSX
	const char *darg[5]={MD5HASH,NULL, NULL};
#else
	const char *darg[5]={MD5HASH,"-r", NULL, NULL}; //Apple OSX
#endif

#ifndef OSX
	darg[1] = name;
#else
	darg[2] = name; //Apple OSX
#endif

//	myCommand.Execute(darg,hash,33);
//	strtok(hash, delimiters);
//	hash[32] = 0;

    MD5 md5;
    md5.sumFile(name);
	md5.finalize();
	strcpy(hash, md5.hexdigest().c_str());
}
