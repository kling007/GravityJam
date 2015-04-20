//
//  GJ_Log.h
//  Gravity_Jam
//
//  Created by Garry Kling on 4/16/15.
//
//

#ifndef __Gravity_Jam__GJ_Log__
#define __Gravity_Jam__GJ_Log__

#include <stdio.h>
#include <string>
#include <sstream>
#include <ctime>

using std::string;

// We are going to create a class to handle the details of logging, but some macros to use in actual code.
// In this case we will just create one log, future changes may allow more/different logging mechanisms

class Log {
    
    string pathToLog;
    std::stringstream logLineStream;
    std::ofstream * logFile;
    
public:
    
    // constructors - we should only make this if we know where it goes
    Log(std::ofstream logStream, string logPath);
    Log(string logPath);
    
    
    // logging functions
    void infoMsg(string s);
    void warnMsg(string s);
    void errorMsg(string s);
    void fatalMsg(string s);
    void gameMsg(string s);
    
    // utility
    std::string getLinePrefix(void); // <timeStamp><dateStamp>
    bool closeLog(void);
    
private:
    bool logMsg(string s);
    bool initLogFile(void);
    std::string getLogFilePath(void);
    
    double logLineNumber=0;
};

#endif /* defined(__Gravity_Jam__GJ_Log__) */
