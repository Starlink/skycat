// -*-c++-*-
#ifndef _RtdUtils_h_
#define _RtdUtils_h_

/*
 * E.S.O. - VLT project 
 * "@(#) $Id: RtdUtils.h,v 1.1.1.1 2009/03/31 14:11:52 cguirao Exp $" 
 *
 * RtdUtils.h - definitions for the rtdimg utilities
 * 
 * who             when      what
 * --------------  --------  ----------------------------------------
 * pbiereic        01/03/01  Created
 */

#include <cstdio>
#include <iostream>
#include <cstring>
#include <cstdarg>

void clip(double& x, double x0, double x1);
void formatHM(double val, char* buf);

/*
 * Class RtdDebugLog
 */

class RtdDebugLog {
public:
    
    RtdDebugLog(char *nam, int debug);

    void log(const char *format, ...);
    void setlog(int set) {debug_ = set;}
    int  setlog() {return debug_;}
    
protected:
    char name_[100];  // name of application
    int  debug_;      // debug flag
};

#endif /* _RtdUtils_h_ */
