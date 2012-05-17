// -*-c++-*-
#ifndef _define_h_
#define _define_h_
/*
 *
 * E.S.O. - VLT project
 *
 * $Id: define.h,v 1.1.1.1 2009/03/31 14:11:52 cguirao Exp $
 *
 * define.h - common definitions
 * 
 * who             when      what
 * --------------  --------  ----------------------------------------
 * Allan Brighton  05/10/95  Created
 * pbiereic        17/02/03  Added defines for byte swap and SOCKLEN_T
 * Peter W. Draper 16/12/05  Redo SOCKLEN_T logic. Only set when socklen_t
 *                           is not defined. Use a typedef.
 * pbiereic        12/08/07  added support for data types double and long long int
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// Byte swap defines. On little Endian machines we use the network-to-host
// routines since they are faster (implemented in assembler code).
#ifdef WORDS_BIGENDIAN
#    define BIGENDIAN 1

// from /usr/include/bits/byteswap.h:
#    define SWAP16(x) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))
#    define SWAP32(x) ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) |  \
                       (((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24))
#else

#    define BIGENDIAN 0
#    define SWAP16(x) (ntohs((unsigned short)x))
#    define SWAP32(x) (ntohl((unsigned int)x))
#endif /* ifdef WORDS_BIGENDIAN */

#define SWAP64(x) ( \
        (((x) & 0xff00000000000000ull) >> 56)  \
      | (((x) & 0x00ff000000000000ull) >> 40)  \
      | (((x) & 0x0000ff0000000000ull) >> 24)  \
      | (((x) & 0x000000ff00000000ull) >> 8)   \
      | (((x) & 0x00000000ff000000ull) << 8)   \
      | (((x) & 0x0000000000ff0000ull) << 24)  \
      | (((x) & 0x000000000000ff00ull) << 40)  \
      | (((x) & 0x00000000000000ffull) << 56))

// Float data: Prevent auto conversions.
// No speed hits if this is compiled with gcc -O.
inline float SWAP_FLOAT(float x) {
    union {float f; unsigned int i;} u;
    u.f = x;
    u.i = SWAP32(u.i);
    return u.f;
}

inline double SWAP_DOUBLE(double x) {
    union {double d; unsigned long long l;} u;
    u.d = x;
    u.l = SWAP64(u.l);
    return u.d;
}

/* Make sure we always have a socklen_t type */
#if ! HAVE_SOCKLEN_T
#  if defined(linux)
     typedef unsigned int socklen_t;
#  elif defined(_XPG4_2)
     typedef size_t socklen_t;
#  else
     typedef int socklen_t;
#  endif
#endif


// min/max
inline int min(int x, int y) {return x<y ? x : y;}
inline int max(int x, int y) {return x>y ? x : y;}
inline double min(double x, double y) {return x<y ? x : y;}
inline double max(double x, double y) {return x>y ? x : y;}

// swap values
inline void swap(int& x, int& y) {int tmp = x; x = y; y = tmp;}
inline void swap(double& x, double& y) {double tmp = x; x = y; y = tmp;}

// inline int roundup(int nbytes, int pad) {return ((nbytes + (pad - 1)) / pad) * pad;}

/*
 * time a function call
 * usage: TIMECALL("name", function(args,...));
 */
#ifndef DEBUG
#define TIMECALL(name,func) func
#else
#include <stdio.h>
#include <sys/time.h>
#include <sys/timeb.h>
extern "C" int ftime(timeb *tp);
#define TIMECALL(name,func) {\
     timeb tp1,tp2; \
     ftime(&tp1); \
     func; \
     ftime(&tp2); \
     fprintf(stderr, "time %s: %d\n", name, (tp2.time-tp1.time)*1000+(tp2.millitm-tp1.millitm)); \
}
#endif

#endif /* _define_h_ */
