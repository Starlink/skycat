#ifndef _NativeImageData_h_
#define _NativeImageData_h_
/*
 * E.S.O. - VLT project 
 *
 * "@(#) $Id: NativeImageData.h,v 1.3 2005/02/02 01:43:03 brighton Exp $" 
 *
 * who             when      what
 * --------------  --------  ----------------------------------------
 * Allan Brighton  20/03/98  Created
 * pbiereic        17/02/03  Native byte order routines revised
 */

#define ShortImageData NativeShortImageData
#include "ShortImageData.h"
#undef ShortImageData

#define UShortImageData NativeUShortImageData
#include "UShortImageData.h"
#undef UShortImageData

#define LongImageData NativeLongImageData
#include "LongImageData.h"
#undef LongImageData

#define FloatImageData NativeFloatImageData
#include "FloatImageData.h"
#undef FloatImageData

#endif /* _NativeImageData_h_ */
