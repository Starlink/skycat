#ifndef _NativeImageData_h_
#define _NativeImageData_h_
/*
 * E.S.O. - VLT project 
 *
 * "@(#) $Id: NativeImageData.h,v 1.1.1.1 2009/03/31 14:11:52 cguirao Exp $" 
 *
 * who             when      what
 * --------------  --------  ----------------------------------------
 * Allan Brighton  20/03/98  Created
 * pbiereic        17/02/03  Native byte order routines revised
 * pbiereic        12/08/07  added data types double and long long int
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

#define LongLongImageData NativeLongLongImageData
#include "LongLongImageData.h"
#undef LongLongImageData

#define DoubleImageData NativeDoubleImageData
#include "DoubleImageData.h"
#undef DoubleImageData

#endif /* _NativeImageData_h_ */
