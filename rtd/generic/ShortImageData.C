/*
 * E.S.O. - VLT project
 *
 * "@(#) $Id: ShortImageData.C,v 1.1.1.1 2006/01/12 16:38:29 abrighto Exp $"
 *
 * ShortImageData.C - member functions for class ShortImageData
 *
 * See the man page ImageData(3) for a complete description of this class
 * library.
 *
 * who             when      what
 * --------------  --------  ----------------------------------------
 * Allan Brighton  05/10/95  Created
 * T. Herlin       08/12/95  Added color scale functions to avoid sign problem
 * Peter W. Draper 23/06/09  Added parseBlank to get blank value in this type.
 *                 09/11/09  Use a scaled range for this data type. Previously
 *                           values mapped directly into the lookup table.
 *                           Now, like other types, they scale between lowCut_
 *                           and highCut_ which can be outside of the range
 *                           -32768 to 32767.
 */

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <cassert>
#include <cmath>
#include <climits>
#include "ShortImageData.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "define.h"


/*
 * convert the given value to short by adding the integer bias,
 * scaling, rounding if necessary and checking the range
 */
short ShortImageData::scaleToShort(int l)
{
    //  If have blank pixels then test for this using raw value.
    if ( haveBlank_ ) {
        if ( blank_ == (short) l ) {
            return LOOKUP_BLANK;
        }
    }

    //  Scale value into range defined by bias_ and scale_. 
    //  Keep result in range of lookup table indices.
    short s;
    double d = (l + bias_) * scale_;
    if (d < 0.0 ) {
        if((d -= 0.5) < LOOKUP_MIN)
            s = LOOKUP_MIN;
        else
            s = (short)d;
    }
    else {
        if((d += 0.5) > LOOKUP_MAX)
            s = LOOKUP_MAX;
        else
            s = (short)d;
    }
    return s;
}

/*
 * Initialize conversion from data range to short and _scale_ (not clip)
 * the low and high cut levels to short range.
 *
 * Method: member variables are set here and used later to convert the short
 * raw image data to another short, which is then used as an index in the
 * lookup table to get the byte value:
 *
 *   bias_  = offset
 *   scale_  = scale factor
 *
 */
void ShortImageData::initShortConversion()
{
    scale_ = LOOKUP_WIDTH / (highCut_ - lowCut_);
    bias_ = -((lowCut_ + highCut_) * 0.5);
    scaledLowCut_ = scaleToShort(int(lowCut_));
    scaledHighCut_ = scaleToShort(int(highCut_));

    if (haveBlank_)
        scaledBlankPixelValue_ = LOOKUP_BLANK;
}

/*
 * Set the blank value from a given string. Return 1 if successful.
 */
int ShortImageData::parseBlank(const char* value) {
    long l;
    int n = sscanf(value, "%ld", &l);
    if ( n > 0 ) {
        blank_ = (short) l;
    }
    return n;
}


/*
 * Include some standard methods as (cpp macro) templates:
 * These are methods that are the same for all derived classes of ImageData,
 * except that they work on a different raw data type
 */
#define CLASS_NAME ShortImageData
#define DATA_TYPE short
#ifndef NTOH
#    define NTOH(x) SWAP16(x)
#endif
#include "ImageTemplates.icc"
#undef CLASS_NAME
#undef DATA_TYPE
#undef NTOH
