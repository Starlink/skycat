/*
 * E.S.O. - VLT project 
 *
 * "@(#) $Id: LongLongImageData.C,v 1.1.1.1 2009/03/31 14:11:52 cguirao Exp $" 
 *
 * LongLongImageData.C - member functions for class LongLongImageData
 * 
 * who             when      what
 * --------------  --------  ----------------------------------------
 * pbiereic        12/08/07  Created
 */

using namespace std;
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <cassert>
#include <cmath>
#include "LongLongImageData.h"
#include "define.h"


/* 
 * convert the given long int to short by adding the integer bias
 * and checking the range
 */
short LongLongImageData::convertToShort(FITS_LONGLONG l) 
{
    register FITS_LONGLONG v = l + bias_;
    short s;

    //  If have blank pixels then test for this as cannot deal with
    //  pixel values at extremes (these wrap and come out in strange
    //  places).
    if ( haveBlank_ ) {
	if ( blank_ == l ) {
	    return LOOKUP_BLANK;
	}
    }
    if (v < LOOKUP_MIN )
	s = LOOKUP_MIN;
    else if (v > LOOKUP_MAX )
	s = LOOKUP_MAX;
    else 
        s = (short) v;
    return s;
}


/* 
 * convert the given long int to short by adding the integer bias,
 * scaling, rounding if necessary and checking the range
 */
short LongLongImageData::scaleToShort(FITS_LONGLONG l) 
{
    //  If have blank pixels then test for this as cannot deal with
    //  pixel values at extremes (these wrap and come out in strange
    //  places).
    if ( haveBlank_ ) {
	if ( blank_ == l ) {
	    return LOOKUP_BLANK;
	}
    }

    short s;
    double d = (l + dbias_) * scale_;
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
 * initialize conversion from base type long to short 
 * and scale the low and high cut levels to short range
 *
 * Method: 3 member variables are set here and used later to convert 
 * the long raw image data to short, which is then used as an index 
 * in the lookup table to get the byte value:
 * 
 *   bias_   = offset
 *   dbias_  = double offset, used when scale != 1.0 
 *   scale_  = scale factor for conversion
 * : 
 */
void LongLongImageData::initShortConversion() 
{
    // full-up scaling required. (+/- (tmax-tmin)/2) 
    // offset values to be zero centered 
    scale_ = LOOKUP_WIDTH / (highCut_ - lowCut_);
    dbias_ = -((lowCut_ + highCut_) * 0.5);
    bias_ = (int)dbias_;
    scaledLowCut_ = scaleToShort((FITS_LONGLONG)lowCut_);
    scaledHighCut_ = scaleToShort((FITS_LONGLONG)highCut_);
    if (haveBlank_)
	scaledBlankPixelValue_ = LOOKUP_BLANK;

    // set int flag for later quick check if scale_ != 1.0
    scaled_ = (scale_ != 1.0);  // Sense inverted - PWD
}


/*
 * Include some standard methods as (cpp macro) templates:
 * These are methods that are the same for all derived classes of ImageData,
 * except that they work on a different raw data type
 */
#define CLASS_NAME LongLongImageData
#define DATA_TYPE FITS_LONGLONG
#ifndef NTOH
#    define NTOH(x) SWAP64(x)
#endif
#include "ImageTemplates.icc"
#undef NTOH
#undef CLASS_NAME
#undef DATA_TYPE
#undef NTOH
