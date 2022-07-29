// -*-c++-*-
/*
 * E.S.O. - VLT project 
 *
 * "@(#) $Id: DoubleImageData.h,v 1.1.1.1 2009/03/31 14:11:52 cguirao Exp $" 
 *
 * DoubleImageData.h - class definitions for class DoubleImageData
 * 
 * who             when      what
 * --------------  --------  ----------------------------------------
 * Peter W. Draper 30/05/01  Created
 *                 14/11/05  Added bias subtraction members.
 * Peter W. Draper 29/10/07  Added colorScale
 * pbiereic        12/08/07  Created
 * Peter W. Draper 17/05/12  Merged skycat version created by pbiereic.
 */

#include <sys/types.h>
#include "ImageData.h"

// This class is used for images where the raw data is made up of doubles.

class DoubleImageData : public ImageData {
private:
    // value of blank pixel, if known (if haveBlankPixel_ is nonzero)
    double blank_;

    double bias_;		// offset for conversion to short lookup index
    double scale_;		// factor for conversion to short lookup index

    // local methods used to get short index in lookup table
    short scaleToShort( double );

    // as above, but unsigned
    inline ushort convertToUshort( double f ) {
	return ushort( scaleToShort( f ) );
    }

    // Return X image pixel value for raw image value.
    // Convert the given double image value to byte, scaling to short
    // first and then using the short value as an index in the color
    // lookup table.
    inline BYTE lookup( double f ) {
        return lookup_[(ushort)scaleToShort(f)];
    }
    inline unsigned long llookup( double f ) {
        return lookup_[(ushort)scaleToShort(f)];
    }

    // return NTOH converted value evtl. subtracted with corresponding bias value
    double getVal(double* p, int idx);

protected:
    // initialize conversion from base type to short,
    void initShortConversion();

    // sprintf format for (x y value)
    virtual char* getXYValueFmt() {return (char *)"%.1f %.1f %.2f";}

    // sprintf format for image pixel value
    virtual char* getValueFmt() {return (char *)"%.2f";}

    int getXsamples(double *rawImage, int idx, int wbox, double *samples);
    int getBsamples(double *rawImage, int idx, int wbox, double *samples);
    int getCsamples(double *rawImage, int idx, int wbox, double *samples);
    double getMedian(double *samples, int n);
    double getBoxVal(double *rawImage, int idx, int wbox, double *samples, int xs);
    double getRMS(double *samples, int n);
    void colorScale(int ncolors, unsigned long* colors);

public:
    // constructor
    DoubleImageData(const char* name, const ImageIO& imio, int verbose)
	: ImageData(name, imio, verbose),
          blank_(0),
	  bias_(0.0),
          scale_(1.0) {}

    // return class name as a string
    virtual const char* classname() { return "DoubleImageData"; }

    // return the data type of the raw data
    int dataType() {return DOUBLE_IMAGE;}

    // return true if the data type is signed
    int isSigned() {return 1;}

    // return a copy of this object
    ImageData* copy() {return new DoubleImageData(*this);}

    // include declarations for methods that differ only in raw data type
#   include "ImageTemplates.h"
};
