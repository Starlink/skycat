 /*
 * E.S.O. - VLT project/ESO Archive 
 * $Id: base64.h,v 1.1.1.1 2009/03/31 14:11:52 cguirao Exp $
 *
 * base64.h - declarations for base64 encoding and decoding
 *            utility routines.
 *
 * who             when       what
 * --------------  --------   ----------------------------------------
 * Allan Brighton  Jul 15    Created
 */

char *encode_base64(char *input);
char *decode_base64(char *input);
