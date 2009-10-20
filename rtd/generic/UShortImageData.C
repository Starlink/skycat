/*
 * E.S.O. - VLT project 
 *
 * "@(#) $Id: UShortImageData.C,v 1.1.1.1 2006/01/12 16:38:30 abrighto Exp $" 
 *
 * UShortImageData.C - member functions for class UShortImageData
 *
 * See the man page ImageData(3) for a complete description of this class
 * library.
 * 
 * who             when      what
 * --------------  --------  ----------------------------------------
 * Allan Brighton  05/10/95  Created
 * Peter W. Draper 23/06/09  Added parseBlank to get blank value in this type.
 *                 20/10/09  Don't let lowCut_ and highCut_ wrap when 
 *                           limits are out of ushort range.
 */


#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <cmath>
#include <climits>
#include "UShortImageData.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "define.h"


/*
 * initialize conversion from base type to short and scale the low and
 * high cut levels to short range.
 */
void UShortImageData::initShortConversion() 
{
    //  Don't let the cuts wrap for unsigned conversions.
    if ( lowCut_ < 0 ) {
        scaledLowCut_ = 0;
    }
    else {
        scaledLowCut_ = (ushort)lowCut_;
    }
    if ( highCut_ > (int) USHRT_MAX ) {
        scaledHighCut_ = USHRT_MAX;
    }
    else {
        scaledHighCut_ = (ushort)highCut_;
    }
    if (haveBlank_)
        scaledBlankPixelValue_ = LOOKUP_BLANK;
}

/*
 * Set the blank value from a given string. Return 1 if successful.
 */
int UShortImageData::parseBlank(const char* value) {
    long l;
    int n = sscanf(value, "%ld", &l);
    if ( n > 0 ) {
        blank_ = (ushort) l;
    }
    return n;
}

/*
 * Include some standard methods as (cpp macro) templates:
 * These are methods that are the same for all derived classes of ImageData,
 * except that they work on a different raw data type
 */
#define CLASS_NAME UShortImageData
#define DATA_TYPE ushort
#ifndef NTOH
#    define NTOH(x) SWAP16(x)
#endif
#include "ImageTemplates.icc"
#undef CLASS_NAME
#undef DATA_TYPE
#undef NTOH
