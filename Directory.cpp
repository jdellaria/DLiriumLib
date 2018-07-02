/*
 * Directory.cpp
 *
 *  Created on: Aug 10, 2009
 *      Author: jdellaria
 */


#include <iostream>
#include <cstdlib>
#include <sys/dir.h>
#include <errno.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "Directory.h"
#include "LinuxCommand.h"


using namespace std;


#ifdef JON
void jon(const char * text, int event);

int main(int argc, char *argv[])
{
	Directory myDirectory;

	cout << "Hello, world!" << endl;
	myDirectory.Recurse((const char*)"/home/jdellaria/Desktop/test", jon);
//	myDirectory.Create("/root/Lost/Highway");

}

void jon(const char * text, int event)
{
	printf("%s\n", text);
}
#endif

Directory::Directory() {
	// TODO Auto-generated constructor stub

}

Directory::~Directory() {
	// TODO Auto-generated destructor stub
}


int Directory::Get(const char* startDirectory, void (*callBack)(const char* directoryEntry, int directoryEntryType), int recurse)
{
	DIR *pdir;
	struct dirent *pent;
	char	fullName[FULLNAMESIZE];
	int stringLength;

	pdir=opendir(startDirectory); //"." refers to the current dir
	if (!pdir)
	{
		printf ("opendir() failure %s\n", startDirectory);
		return 0;
//		exit(1);
	}

	while ((pent=readdir(pdir)))
	{
		/*
		 * Skip the "." and ".." entries
		 */
		stringLength = strlen (startDirectory) + strlen (pent->d_name) + 2;

		if (strcmp (pent->d_name, ".") == 0 ||	strcmp (pent->d_name, "..") == 0)
			continue;

		/*
		 * Make the full filename
		 */

		if (stringLength > FULLNAMESIZE)
		{
			printf ("Directory::Get closedir(pdir): %s\n",pent->d_name); //JON
			closedir(pdir);
			return 0;
		}
		sprintf (fullName, "%s/%s", startDirectory, pent->d_name);
//		sprintf (fullPathName, fullName); // this causes an overflow with the filename 'drefhttps3A2F2Fthetecht (copy).com%25252F2012%25252F08%25252F06%25252Fi'. replaced with strncpy and solved the problem
		strncpy (fullPathName, fullName, FULLNAMESIZE);


		//This lstat is to get the file information for the recurse
#ifdef	S_IFLNK
		if (lstat (fullName, &finfo) == -1)
#else
		if (stat (fullName, &finfo) == -1)
#endif
			continue;
		if (recurse != 0)
		{
			if (S_ISDIR (finfo.st_mode))
			{
				Recurse(fullName, callBack);
				//This lstat is to get the file information for the callback cause when the recurse comes back the file information has changed
		#ifdef	S_IFLNK
				lstat (fullName, &finfo);
		#else
				stat (fullName, &finfo);
		#endif
			}
		}

/*
	S_ISDIR - Is file a directory
	S_ISLNK - Is file a symbolic link
	S_ISREG - Is file a regular file
	S_ISCHR - Is file a character device
	S_ISBLK - Is file a block device
	S_ISFIFO - Is file a FIFO
	S_ISSOCK - Is file a unix socket
*/

		if (callBack != NULL)
		{
		  // - Is file a directory
			if (S_ISDIR (finfo.st_mode))
				callBack(fullName,DIRECTORYENTRYTYPE_DIR);
			   // - Is file a symbolic link
			if (S_ISLNK (finfo.st_mode))
				callBack(fullName,DIRECTORYENTRYTYPE_LNK);
			   // - Is file a regular file
			if (S_ISREG (finfo.st_mode))
				callBack(fullName,DIRECTORYENTRYTYPE_REG);
			   // - Is file a character device
			if (S_ISCHR (finfo.st_mode))
				callBack(fullName,DIRECTORYENTRYTYPE_CHR);
			   // - Is file a block device
			if (S_ISBLK (finfo.st_mode))
				callBack(fullName,DIRECTORYENTRYTYPE_BLK);
			  // - Is file a FIFO
			if (S_ISFIFO (finfo.st_mode))
				callBack(fullName,DIRECTORYENTRYTYPE_FIFO);
			  // - Is file a unix socket
			if (S_ISSOCK (finfo.st_mode))
				callBack(fullName,DIRECTORYENTRYTYPE_SOCK);
		}

	}
	closedir(pdir);
	return 1;
}


int Directory::Recurse(const char* startDirectory, void (*callBack)(const char* directoryEntry, int directoryEntryType))
{
	return (Get(startDirectory, callBack, 1));
}

// The following command will create the complete directory structure making parent directories as needed

int Directory::Create(const char* name)
{
	LinuxCommand myCommand;
	const char* darg[4]={CREATEDIRECTORY,"-p",NULL, NULL}; // -p option makes parent directories as needed
//	char* const darg[]
	darg[2] = name;
	myCommand.Execute(darg,errorMessageBuffer,sizeof(errorMessageBuffer));
	if (strlen(errorMessageBuffer) > 0)
	{
		return (-1); // on error errorMessageBuffer has the error string
	}
	else
	{
		return (0);
	}
}

void Directory::CreateFromFileName (string& fullFileName)
{
	  int found;
	  string directoryName;

	  found=fullFileName.find_last_of("/\\");
	  directoryName = fullFileName.substr(0,found);
	  if(chdir(directoryName.c_str()) != 0)
	  {
		  Create((const char*)directoryName.c_str());
	  }
}
// The following command will delete the complete directory structure and all the files.
int Directory::Remove(const char* name)
{
	LinuxCommand myCommand;
	const char* darg[4]={REMOVEDIRECTORY,"-rf",NULL, NULL};

	darg[2] = name;
	myCommand.Execute(darg,errorMessageBuffer,sizeof(errorMessageBuffer));
	if (strlen(errorMessageBuffer) > 0)
	{
		return (-1); // on error errorMessageBuffer has the error string
	}
	else
	{
		return (0);
	}
}
