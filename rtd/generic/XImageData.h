// -*-c++-*-
#ifndef _XImageData_h_
#define _XImageData_h_
/*
 * E.S.O. - VLT project 
 *
 * "@(#) $Id: XImageData.h,v 1.1.1.1 2009/03/31 14:11:52 cguirao Exp $" 
 *
 * XImageData.h - class definitions for class XImageData
 *
 * See the man page ImageData(3) for a complete description of this class
 * library.
 * 
 * who             when      what
 * --------------  --------  ----------------------------------------
 * Allan Brighton  05/10/95  Created
 * Peter W. Draper 04/03/98  Added llookup.
 *                 14/07/98  Added blank pixel check for lookup.
 * P.Biereichel    22/03/99  Added definitions for bias subtraction
 */

#include <sys/types.h>
#include "ImageData.h"



// This class is used for images where the raw data is made up of bytes.

class XImageData : public ImageData {
private:
    // value of blank pixel, if known (if haveBlankPixel_ is nonzero)
    BYTE blank_;

    // get value as unsigned short
    inline ushort convertToUshort(BYTE b) {
	return (ushort)b;
    }


    // return X image pixel value for raw image value
    inline BYTE lookup(BYTE b) {
	if ( !haveBlank_ ) return b;
	if ( b != blank_ ) return b;
	return blank_;
    } 
    inline unsigned long llookup(BYTE b) {
	if ( !haveBlank_ ) return b;
	if ( b != blank_ ) return b;
	return blank_;
    }

    // return NTOH converted value evtl. subtracted with corresponding bias value
    BYTE getVal(BYTE* p, int idx);

    int getXsamples(BYTE *rawImage, int idx, int wbox, BYTE *samples);
    int getBsamples(BYTE *rawImage, int idx, int wbox, BYTE *samples);
    int getCsamples(BYTE *rawImage, int idx, int wbox, BYTE *samples);
    BYTE getMedian(BYTE *samples, int n);
    BYTE getBoxVal(BYTE *rawImage, int idx, int wbox, BYTE *samples, int xs);
    BYTE getRMS(BYTE *samples, int n);

protected:
    // no conversion necessary
    void initShortConversion() { 
	scaledLowCut_ = 0;
	scaledHighCut_ = 255;
	scaledBlankPixelValue_ = LOOKUP_BLANK;
    }

public:
    // constructor
    XImageData(const char* name, const ImageIO& imio, int verbose)
	: ImageData(name, imio, verbose), blank_(0) {
	flipY_ = 1;
    }

    // return class name as a string
    virtual const char* classname() { return "XImageData"; }

    // return the data type of the raw data
    int dataType() {return X_IMAGE;}

    // return true if the data type is signed
    int isSigned() {return 0;}

    // return a copy of this object
    ImageData* copy() {return new XImageData(*this);}

    // set the color scale algorithm for the image (redefined from base class)
    void colorScale(int ncolors, unsigned long* colors) {}

    // include declarations for methods that differ only in raw data type
#   include "ImageTemplates.h"
};


#endif /* _XImageData_h_ */
