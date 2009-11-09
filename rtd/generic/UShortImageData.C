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
 *                 09/11/09  Use a scaled range for this data type. Previously
 *                           values mapped directly into the lookup table.
 *                           Now, like other types, they scale between lowCut_
 *                           and highCut_ which can be outside of the range
 *                           0 to 65535.
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
 * convert the given value to ushort by adding the integer bias,
 * scaling, rounding if necessary and checking the range
 */
ushort UShortImageData::convertToUshort(int l)
{
    //  If have blank pixels then test this against the raw value.
    if ( haveBlank_ ) {
        if ( blank_ == (ushort) l ) {
            return LOOKUP_BLANK;
        }
    }

    //  Scale value into range defined by bias_ and scale_. Keep result in
    //  range of lookup table indices. Note this is already unsigned, so
    //  limits are 0, LOOKUP_SIZE-1, not the usual LOOKUP_MIN, LOOKUP_MAX.
    ushort s;
    double d = (l + bias_) * scale_;
    if (d < 0.0) {
        s = 0;
    }
    else {
        if((d += 0.5) > LOOKUP_WIDTH) {
            s = LOOKUP_WIDTH;
        }
        else {
            s = (ushort)d;
        }
    }
    return s;
}

/*
 * Initialize conversion from data range to ushort and _scale_ (not clip)
 * the low and high cut levels to ushort range.
 *
 * Method: member variables are set here and used later to convert the ushort
 * raw image data to another ushort, which is then used as an index in the
 * lookup table to get the byte value (for colour lookup).
 *
 *   bias_  = offset
 *   scale_  = scale factor
 * :
 */
void UShortImageData::initShortConversion()
{
    if ( ( highCut_ - lowCut_ ) > 0.0 ) {
        bias_ = -lowCut_;
        scale_ = LOOKUP_WIDTH / (highCut_ - lowCut_);
    }
    else {
        scale_ = 1.0;
        bias_ = 0.0;
    }
    scaledLowCut_ = convertToUshort(int(lowCut_));
    scaledHighCut_ = convertToUshort(int(highCut_));

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
