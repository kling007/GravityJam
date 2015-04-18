//
//  GJ_Log.cpp
//  Gravity_Jam
//
//  Created by Garry Kling on 4/16/15.
//
//

#include "GJ_Log.h"



std::string Log::getLinePrefix()
{
    // the tm struct
    time_t t = time(0);
    struct tm * now = localtime( & t );
    
    // collect line number, time/date stamp
    std::stringstream linePrefix;
    linePrefix.str("");

    // date and time
    // if we need something more detailed, look into using the chrono header
    linePrefix << logLineNumber++ <<" ["<<asctime(now)<<"] ";
    return linePrefix.str();
    
}

