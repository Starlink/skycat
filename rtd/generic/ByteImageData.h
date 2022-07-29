// -*-c++-*-
#ifndef _ByteImageData_h_
#define _ByteImageData_h_
/*
 * E.S.O. - VLT project 
 *
 * "@(#) $Id: ByteImageData.h,v 1.1.1.1 2009/03/31 14:11:52 cguirao Exp $" 
 *
 * ByteImageData.h - class definitions for class ByteImageData
 *
 * See the man page ImageData(3) for a complete description of this class
 * library.
 *
 * who             when      what
 * --------------  --------  ----------------------------------------
 * Allan Brighton  05/10/95  Created
 * Peter W. Draper 05/03/98  Added llookup
 *                 14/07/98  Added check for blanks in lookup.
 * P.Biereichel    22/03/99  Added parameters for bias frame
 */

#include <sys/types.h>
#include "ImageData.h"



// This class is used for images where the raw data is made up of bytes

class ByteImageData : public ImageData 
{
private:
    // value of blank pixel, if known (if haveBlankPixel_ is nonzero)
    long blank_;

    // get value as unsigned short
    inline ushort convertToUshort(BYTE b) {
	return (ushort)b;
    }

    // return X image pixel value for raw image value
    inline BYTE lookup(BYTE b) {
	if ( !haveBlank_ ) return lookup_[(ushort)b];
	if ( b != blank_ ) return lookup_[(ushort)b];
	return lookup_[128];
    } 
    inline unsigned long llookup(BYTE b) {
	if ( !haveBlank_ ) return lookup_[(ushort)b];
	if ( b != blank_ ) return lookup_[(ushort)b];
	return lookup_[128];
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
    // convert cut values to short range
    void initShortConversion();

public:
    // constructor
    ByteImageData(const char* name, const ImageIO& imio, int verbose)
	: ImageData(name, imio, verbose, 256 /* use a smaller lookup table */),
	  blank_(128) { }

    // return class name as a string
    virtual const char* classname() { return "ByteImageData"; }

    // return the data type of the raw data
    int dataType() {return BYTE_IMAGE;}

    // return true if the data type is signed
    int isSigned() {return 0;}

    // return a copy of this object
    ImageData* copy() {return new ByteImageData(*this);}

    // include declarations for methods that differ only in raw data type
#   include "ImageTemplates.h"
};


#endif /* _ByteImageData_h_ */
