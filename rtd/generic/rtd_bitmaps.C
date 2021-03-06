
/*
 * E.S.O. - VLT project 
 * "@(#) $Id: rtd_bitmaps.C,v 1.1.1.1 2009/03/31 14:11:52 cguirao Exp $"
 *
 * Bitmap definitions for Tk
 *
 * This file was generated by ../bitmaps/bitmaps.tcl  - DO NO EDIT
 */

#include <tcl.h>
#include <tk.h>


void defineRtdBitmaps(Tcl_Interp *interp) {
    #include "double_left.xbm"
    Tk_DefineBitmap(interp, Tk_GetUid("double_left"), (char*)double_left_bits, double_left_width, double_left_height);

    #include "record.xbm"
    Tk_DefineBitmap(interp, Tk_GetUid("record"), (char*)record_bits, record_width, record_height);

    #include "big_right.xbm"
    Tk_DefineBitmap(interp, Tk_GetUid("big_right"), (char*)big_right_bits, big_right_width, big_right_height);

    #include "Right.xbm"
    Tk_DefineBitmap(interp, Tk_GetUid("Right"), (char*)Right_bits, Right_width, Right_height);

    #include "rect.xbm"
    Tk_DefineBitmap(interp, Tk_GetUid("rect"), (char*)rect_bits, rect_width, rect_height);

    #include "double_right.xbm"
    Tk_DefineBitmap(interp, Tk_GetUid("double_right"), (char*)double_right_bits, double_right_width, double_right_height);

}
