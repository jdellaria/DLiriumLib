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
#include <sstream>
#include <iomanip>

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
	int print (logLevel errorLevel);
	int print (logLevel errorLevel, string message);

	void setLogFileName (string filename);
	void setErrorString (logLevel errorLevel);
	void setErrorLevel (logLevel errorValue);

	void FlushDebug();
	void FlushInformation();
	void FlushWarning();
	void FlushError();
	void FlushLog();

	DLog& operator<<(DLog& (*)(DLog&));
	DLog& operator<<(const char);
	DLog& operator<<(const std::string&);
	DLog& operator<<(const int);
	DLog& operator<<(const long);
	DLog& operator<<(const double);
	DLog& operator<<(const char *);
	DLog& operator<<(const bool);

	string DLogMessage;
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

// overloading of the manipulators

DLog& DLogDebug(DLog& os);
DLog& DLogInformation(DLog& os);
DLog& DLogWarning(DLog& os);
DLog& DLogError(DLog& os);
DLog& DLogLog(DLog& os);


#endif /* DLOG_H_ */
