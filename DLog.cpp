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
}


int main() {

	DLog myLog;
	bool myBool = false;
	string message;
	string FileName = "/home/jdellaria/Desktop/JONLog.txt";

	myLog.logFileName = FileName;
//	myLog.setLogFileName ("/home/jdellaria/Desktop/JONLog.txt");
	myLog.logValue = logDebug;
	myLog.printFile = true;
	myLog.printScreen = true;
	myLog.printTime = true;

//	myMessage.print();
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	cout << "myLog.logFileName " << myLog.logFileName << endl;
	message = "DLog Message ";
//	myLog.print(logWarning, message);

	double pi = 3.14;
	long longInt = 2212;
	myLog << "DLogDebug Message " << pi <<'w'<< longInt << DLogDebug;
	myLog << "DLogInformation Message " << pi  <<'w'<<  DLogInformation;
	myLog << "DLogWarning Message " <<  pi  <<'w'<< DLogWarning;
	myLog << "DLogError Message " <<  pi  <<'w'<< DLogError;
	myLog << "DLogLog Message " <<  pi  <<'w'<< DLogLog;

	myLog << "1 DLogDebug Message " << pi <<'s';
	myLog.print(logDebug);
	myLog << "1 DLogInformation Message " << pi <<'s';
	myLog.print(logInformation);
	myLog << "1 DLogWarning Message " <<  pi  <<'s';
	myLog.print(logWarning);
	myLog << "1 DLogError Message " <<  pi  <<'s';
	myLog.print(logError);
	myLog << "1 DLogLog Message " <<  pi  <<'s';
	myLog.print(logLog);

//	myLog.print(logDebug);
	return 0;
}
*/

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

int DLog::print (logLevel errorLevel)
{
	int returnValue;

	returnValue = print(errorLevel, DLogMessage);
	DLogMessage.clear();
	return (returnValue);
}

int DLog::print (logLevel errorLevel, string message)
{
	int returnValue;

	returnValue = print(errorLevel, message.c_str());
	return (returnValue);
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


DLog& DLog::operator<<(DLog& (*fp)(DLog&))
{
      return (*fp)(*this);
}


DLog& DLog::operator<<(const char * text)
{
	DLogMessage += std::string(text);
   return *this;
}


DLog& DLog::operator<<(const std::string &text)
{
	DLogMessage += text;
    return *this;
}

DLog& DLog::operator<<(const int number)
{
	DLogMessage += std::to_string(number);
    return *this;
}


DLog& DLog::operator<<(const long number)
{
	DLogMessage += std::to_string(number);
    return *this;
}

DLog& DLog::operator<<(const double number)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << number;
    DLogMessage += ss.str();
    return *this;
}

DLog& DLog::operator<<(const char character)
{
	DLogMessage += std::string(1, character);
	return *this;
}

DLog& DLog::operator<<(const bool boolean)
{
	if(boolean == true)
		DLogMessage +=  "'true'";
	else
		DLogMessage +=  "'false'";
	return *this;
}

void DLog::FlushDebug()
{
	print (logDebug, DLogMessage);
	DLogMessage.clear();
}

void DLog::FlushInformation()
{
	print (logInformation, DLogMessage);
	DLogMessage.clear();
}


void DLog::FlushWarning()
{
	print (logWarning, DLogMessage);
	DLogMessage.clear();
}


void DLog::FlushError()
{
	print (logError, DLogMessage);
	DLogMessage.clear();
}


void DLog::FlushLog()
{
	print (logLog, DLogMessage);
	DLogMessage.clear();
}

DLog& DLogDebug(DLog& os)
{

	os.FlushDebug();
	return os;
}


DLog& DLogInformation(DLog& os)
{
    os.FlushInformation();
    return os;
}

DLog& DLogWarning(DLog& os)
{
	os.FlushWarning();
	return os;
}


DLog& DLogError(DLog& os)
{
    os.FlushError();
    return os;
}

DLog& DLogLog(DLog& os)
{
    os.FlushLog();
    return os;
}



