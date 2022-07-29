// -*-c++-*-
/*
 * E.S.O. - VLT project 
 *
 * "@(#) $Id: UShortImageData.h,v 1.1.1.1 2009/03/31 14:11:52 cguirao Exp $" 
 *
 * UShortImageData.h - class definitions for class UShortImageData
 *
 * See the man page ImageData(3) for a complete description of this class
 * library.
 * 
 * who             when      what
 * --------------  --------  ----------------------------------------
 * Allan Brighton  05/10/95  Created
 * Peter W. Draper 04/03/98  Added llookup.
 *                 14/07/98  Added check for blanks in lookup.
 * P.Biereichel    22/03/99  Added definitions for bias subtraction
 * Peter W. Draper 09/11/09  Use a scaled value for lookup.
 */

#include "ImageData.h"


// This class is used for images where the raw data is made up of 
// unsigned shorts

class UShortImageData : public ImageData {

private:
    // value of blank pixel, if known (if haveBlankPixel_ is nonzero)
    ushort blank_;

    double bias_;              // offset
    double scale_;             // scale factor

    // local methods used to get short index in lookup table
    ushort convertToUshort(int l);

    // return X image pixel value for raw image value
    inline BYTE lookup(ushort s) {
        return lookup_[convertToUshort(s)];
    }
    inline unsigned long llookup(ushort s) {
        return lookup_[convertToUshort(s)];
    }

    // return NTOH converted value evtl. subtracted with corresponding bias value
    ushort getVal(ushort* p, int idx);

    int getXsamples(ushort *rawImage, int idx, int wbox, ushort *samples);
    int getBsamples(ushort *rawImage, int idx, int wbox, ushort *samples);
    int getCsamples(ushort *rawImage, int idx, int wbox, ushort *samples);
    ushort getMedian(ushort *samples, int n);
    ushort getBoxVal(ushort *rawImage, int idx, int wbox, ushort *samples, int xs);
    ushort getRMS(ushort *samples, int n);

protected:

    // initialize conversion from base type to (unsigned) short,
    void initShortConversion();

public:
    // constructor
    UShortImageData(const char* name, const ImageIO& imio, int verbose)
	: ImageData(name, imio, verbose), blank_(0) {}

    // return class name as a string
    virtual const char* classname() { return "UShortImageData"; }

    // return the data type of the raw data
    int dataType() {return USHORT_IMAGE;}

    // return true if the data type is signed
    int isSigned() {return 0;}

    // return a copy of this object
    ImageData* copy() {return new UShortImageData(*this);}

    // include declarations for methods that differ only in raw data type
#   include "ImageTemplates.h"
};
