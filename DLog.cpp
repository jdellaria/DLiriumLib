/*
 * DLog.cpp
 *
 *  Created on: Dec 4, 2011
 *      Author: jdellaria
 */

#include "DLog.h"

#include <stdio.h>
#include <iostream>
#include <time.h>
#include "DLog.h"


using namespace std;


/*int main ()
{
	DLog myLog;
	char message[50];

//	myLog.setLogFileName ("jon.txt");
//	myLog.setErrorLevel(logInformation);
	sprintf (message, "this is my message %d", 100 );
	myLog.print(logDebug, message);
}*/

DLog::DLog()
{
	logValue = logDebug;
	printScreen = true; // print to Screen
	printFile = true; // print to file
	printTime = true; // print time stamp
	numberOfErrors = 0;
	numberOfWarnings = 0;
	setLogFileName ("log.txt"); //default file name
	errorString.append("Error Level Not Set");
}

DLog::~DLog()
{
	// TODO Auto-generated destructor stub
}

void DLog::setLogFileName (string filename)
{
	logFileName = filename;
}

void DLog::setErrorLevel (logLevel errorValue)
{
	logValue = errorValue;
}

void DLog::setErrorString (logLevel errorLevel)
{
	errorString.clear();
	if (errorLevel == logDebug)
	{
	   errorString.append("Debug");
	}
	else if (errorLevel == logInformation)
	{
	   errorString.append("Information");
	}
	else if (errorLevel == logWarning)
	{
	   errorString.append("Warning");
	}
	else if (errorLevel == logError)
	{
	   errorString.append("Error");
	}
	else if (errorLevel == logLog)
	{
	   errorString.append("Log");
	}
	else
	{
		errorString.append("Error Level Not Set");
	}
}

int DLog::print (logLevel errorLevel, string message)
{
	return (print(errorLevel, message.c_str()));
}

int DLog::print (logLevel errorLevel, const char *message)
{
   FILE * pFile;
   time_t rawtime;
   string myTime;
   string myLogMessage;
   size_t found;

   if (errorLevel == logError)
   {
	   numberOfErrors++;
   }
   if (errorLevel == logWarning)
   {
	   numberOfWarnings++;
   }
   if (errorLevel >= logValue)
   {
	   setErrorString (errorLevel);
	   time (&rawtime );
	   myTime.append(ctime (&rawtime)); // ctime adds a carriage return at the end of the string. We need to remove it.
	   myTime.erase (myTime.size() - 1, 3);
	   myLogMessage.append(errorString);
	   myLogMessage.append(": ");
	   myLogMessage.append(message);
	   found = myLogMessage.find_last_of("\n");
	   if(printFile)
	   {
		  pFile = fopen (logFileName.c_str(),"a+");
		  if (pFile != NULL)
		  {
			   if (printTime == true)
			   {
				   fprintf (pFile,"%s: ",myTime.c_str());
			   }
			   fprintf (pFile,"%s\n",myLogMessage.substr(0,found).c_str());
			   fclose (pFile);
		  }
		  else
		  {
			   if (printTime == true)
			   {
				   printf ("%s: ",myTime.c_str());
			   }
			   printf ("COULD NOT OPEN LOG FILE %s!!!\n", logFileName.c_str());
		  }
	   }
	   if(printScreen)
	   {
		   if (printTime == true)
		   {
			   printf ("%s: ",myTime.c_str());
		   }
		   printf ("%s\n",myLogMessage.substr(0,found).c_str());
	   }
   }
   return 0;
}
