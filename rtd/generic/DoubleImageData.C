/*
 * E.S.O. - VLT project 
 *
 * "@(#) $Id: DoubleImageData.C,v 1.1.1.1 2009/03/31 14:11:52 cguirao Exp $" 
 *
 * DoubleImageData.C - member functions for class DoubleImageData
 * 
 * who             when      what
 * --------------  --------  ----------------------------------------
 * Peter W. Draper 30/05/01  Created.
 *                 29/10/07  Add colorScale so that a bin for NaN pixels
 *                           is always available (shared with blank).
 * Peter W. Draper 23/06/09  Added parseBlank to get blank value in this type.
 * pbiereic        12/08/07  Created
 * Peter W. Draper 17/05/12  Merged skycat version created by pbiereic.
 */

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <cassert>
#include <cmath>
#include "DoubleImageData.h"
#include "define.h"

/* 
 * Convert the given double to short by adding the bias, scaling,
 * rounding if necessary and checking the range.
 */
short DoubleImageData::scaleToShort( double d )
{
    if ( isnan(d) ) {
	return LOOKUP_BLANK;
    }

    //  Blank pixel value is is special lookup table position. Note
    //  Starlink uses a special value for floating point too (not a NaN).
    if ( haveBlank_ ) {
        if ( blank_ == d ) {
            return LOOKUP_BLANK;
        }
    }

    short s;
    d = (d + bias_) * scale_;
    if (d < 0.0 ) {
	if( ( d -= 0.5 ) < LOOKUP_MIN ) {
	    s = LOOKUP_MIN;
	} else {
	    s = (short)d;
        }
    } else {
	if( ( d += 0.5 ) > LOOKUP_MAX) {
	    s = LOOKUP_MAX;
        } else {
	    s = (short)d;
        }
    }
    return s;
}

/*
 * Initialize conversion from base type float to short
 * and scale the low and high cut levels to short range.
 *
 * Method: 2 member variables, bias_ and scale_, are set here and
 * used later to convert the float raw image data to short, which is
 * then used as an index in the lookup table:
 */
void DoubleImageData::initShortConversion()
{
    bias_ = -((lowCut_ + highCut_) * 0.5);

    if( (highCut_ - lowCut_) > 0.0 ) {
	scale_ = LOOKUP_WIDTH / (highCut_ - lowCut_);
    } 
    else {
	scale_ = 1.0;
    }

    scaledLowCut_ = scaleToShort(lowCut_);
    scaledHighCut_ = scaleToShort(highCut_);
    scaledBlankPixelValue_ = LOOKUP_BLANK;
}

/*
 * Define a scaledBlankPixelValue_ so that we have a blank bin for NaN's
 * not just blanks (both use same color).
 */
void DoubleImageData::colorScale(int ncolors, unsigned long* colors)
{
    ImageData::colorScale(ncolors, colors);

    // Always set value for blank pixel in case we have NaNs, not just
    // when blank is set.
    lookup_.setPixelColor( scaledBlankPixelValue_, color0_ );
}

/*
 * Set the blank value from a given string. Return 1 if successful.
 */
int DoubleImageData::parseBlank(const char* value) {
    double d;
    int n = sscanf(value, "%lf", &d);
    if ( n > 0 ) {
        blank_ = (double) d;
    }
    return n;
}

/*
 * Include some standard methods as (cpp macro) templates:
 * These are methods that are the same for all derived classes of ImageData,
 * except that they work on a different raw data type
 */
#define CLASS_NAME DoubleImageData
#define DATA_TYPE double
#ifndef NTOH
#    define NTOH(x) SWAP_DOUBLE(x)
#endif

// return true is the value x is a NAN (define to 0 for non-float types)
#define ISNAN(x) isnan(x)

#include "ImageTemplates.icc"
#undef CLASS_NAME
#undef DATA_TYPE
#undef NTOH
#undef ISNAN
