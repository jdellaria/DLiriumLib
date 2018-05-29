/*
 * DLog.h
 *
 *  Created on: Dec 12, 2011
 *      Author: jdellaria
 */
#ifndef DLOG_H_
#define DLOG_H_
#include <stdio.h>
#include <iostream>

using namespace std;

enum logLevel
{
	logDebug,
    logInformation,
    logWarning,
    logError,
    logLog
};

class DLog {
public:
	DLog();
	virtual ~DLog();
	int print (logLevel errorLevel, string message);
	void setLogFileName (string filename);
	void setErrorString (logLevel errorLevel);
	void setErrorLevel (logLevel errorValue);

	string errorString;
	string logFileName;
	unsigned long numberOfErrors;
	unsigned long numberOfWarnings;
	enum logLevel logValue;
	bool printScreen;
	bool printFile;
	bool printTime;
private:
	int print (logLevel errorLevel, const char *message);
};

#endif /* DLOG_H_ */
