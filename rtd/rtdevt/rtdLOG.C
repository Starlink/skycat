/*******************************************************************************
* E.S.O. - VLT project
*
* "@(#) $Id: rtdLOG.C,v 1.1.1.1 2009/03/31 14:11:52 cguirao Exp $"
*
* who       when      what
* --------  --------  ----------------------------------------------
* pbiereic  01/03/01  created 
*/

/************************************************************************
*   NAME
*   
* 
*   SYNOPSIS
*   rtdLOG::rtdLOG(int verbose) :
*      verbose: 0 don't log, 1 print logging messages
* 
*   DESCRIPTION
*   rtdLOG is a class for printing logging messages
*
*   PUBLIC METHODS
*   void rtdLOG::log(const char *format, ...)
*     Prints logs on stdout
*
*   FILES
*
*   ENVIRONMENT
*
*   COMMANDS
*
*   RETURN VALUES
*
*   CAUTIONS 
*
*   EXAMPLES
*
*   SEE ALSO
*
*   BUGS   
* 
*------------------------------------------------------------------------
*/

#define _POSIX_SOURCE 1
#include "rtdLOG.h"

static const char *rcsId="@(#) $Id: rtdLOG.C,v 1.1.1.1 2009/03/31 14:11:52 cguirao Exp $"; 

rtdLOG::rtdLOG(int verbose) :
    verbose_(verbose)
{
}

rtdLOG::~rtdLOG() {}

void rtdLOG::log(const char *format, ...)
{
    if (! verbose_)
	return;
    printf("rtdServer: ");
    va_list ap;
    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
}
