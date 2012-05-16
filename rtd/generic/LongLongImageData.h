/*
 * E.S.O. - VLT project 
 *
 * "@(#) $Id: LongLongImageData.h,v 1.1.1.1 2009/03/31 14:11:52 cguirao Exp $" 
 * $Id: LongLongImageData.h,v 1.1.1.1 2009/03/31 14:11:52 cguirao Exp $
 *
 * LongLongImageData.h - class definitions for class LongLongImageData
 *
 * who             when      what
 * --------------  --------  ----------------------------------------
 * pbiereic        12/08/07  Created
 */

#include <sys/types.h>
#include "ImageData.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define FITS_LONGLONG long long

// This class is used for images where the raw data is made up of ints

class LongLongImageData : public ImageData 
{
private:
    // value of blank pixel, if known (if haveBlankPixel_ is nonzero)
    FITS_LONGLONG blank_;

    int bias_;			// offset for long to short conversion
    double dbias_;		// used when scale != 1.0
    double scale_;		// factor for long to short conversion
    int scaled_;		// flag, true if scale_ != 1.0

    // local methods used to get short index in lookup table
    short convertToShort(FITS_LONGLONG);	// convert long to short by adding bias
    short scaleToShort(FITS_LONGLONG);	// as above, but with scaling

    inline ushort convertToUshort(FITS_LONGLONG l) { // unsigned
	return ushort(scaled_ ? scaleToShort(l) : convertToShort(l));
    }

    // return X image pixel value for raw image value
    inline byte lookup(FITS_LONGLONG l) {
	return lookup_[convertToUshort(l)];
    }
    inline unsigned long llookup(FITS_LONGLONG l) {
	return lookup_[convertToUshort(l)];
    }

    // return NTOH converted value evtl. subtracted with corresponding bias value
    FITS_LONGLONG getVal(FITS_LONGLONG* p, int idx);

    int getXsamples(FITS_LONGLONG *rawImage, int idx, int wbox, FITS_LONGLONG *samples);
    int getBsamples(FITS_LONGLONG *rawImage, int idx, int wbox, FITS_LONGLONG *samples);
    int getCsamples(FITS_LONGLONG *rawImage, int idx, int wbox, FITS_LONGLONG *samples);
    FITS_LONGLONG getMedian(FITS_LONGLONG *samples, int n);
    FITS_LONGLONG getBoxVal(FITS_LONGLONG *rawImage, int idx, int wbox, FITS_LONGLONG *samples, int xs);
    FITS_LONGLONG getRMS(FITS_LONGLONG *samples, int n);

protected:

    // initialize conversion from base type to short,
    void initShortConversion();

public:
    // constructor
    LongLongImageData(const char* name, const ImageIO& imio, int verbose)
	: ImageData(name, imio, verbose), 
	  blank_(0) {}

    // return the data type of the raw data
    int dataType() {return LONGLONG_IMAGE;}

    // return true if the data type is signed
    int isSigned() {return 1;}

    // return a copy of this object
    ImageData* copy() {return new LongLongImageData(*this);}

    // include declarations for methods that differ only in raw data type
#   include "ImageTemplates.h"
};

