/*
 * E.S.O. - VLT project 
 *
 * "@(#) $Id: XImageData.C,v 1.1.1.1 2009/03/31 14:11:52 cguirao Exp $" 
 *
 * XImageData.C - member functions for class XImageData
 *
 * See the man page ImageData(3) for a complete description of this class
 * library.
 * 
 * who             when      what
 * --------------  --------  ----------------------------------------
 * Allan Brighton  05/10/95  Created
 * pbiereic        17/02/03  Added 'using namespace std'.
 * Peter W. Draper 23/06/09  Added parseBlank to get blank value in this type.
 */
static const char* const rcsId="@(#) $Id: XImageData.C,v 1.1.1.1 2009/03/31 14:11:52 cguirao Exp $";


using namespace std;
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <cassert>
#include <cmath>
#include "XImageData.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "define.h"

/*
 * Set the blank value from a given string. Return 1 if successful.
 */
int XImageData::parseBlank(const char* value) {
    long l;
    int n = sscanf(value, "%ld", &l);
    if ( n > 0 ) {
        blank_ = (BYTE) l;
    }
    return n;
}

/*
 * Include some standard methods as (cpp macro) templates:
 * These are methods that are the same for all derived classes of ImageData,
 * except that they work on a different raw data type
 */
#define CLASS_NAME XImageData
#define DATA_TYPE BYTE
#define NTOH(x) (x)
#include "ImageTemplates.icc"
#undef CLASS_NAME
#undef DATA_TYPE
#undef NTOH


