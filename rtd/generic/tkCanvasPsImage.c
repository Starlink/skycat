/*
 * E.S.O. - VLT project / ESO Archive
 *
 * "@(#) $Id: tkCanvasPsImage.c,v 1.2 2005/02/02 01:43:02 brighton Exp $"
 *
 * TkCanvasPsImage.C -  Implement Tk postscript output for images
 *
 * The code here was taken from a version of the Tk canvasps patch, which
 * was modified by Peter Draper of Starlink to only access the visible
 * portion of an image (required, to avoid problems with large or zoomed
 * in images).
 *
 * To avoid having to patch the Tk sources, I gathered all the necessary
 * code in this file, including some private struct typedefs from the Tk
 * source files, which are not available in any include files.
 *
 * NOTE: This code is Tk4.2 specific. If you upgrade to a newer version,
 * you will have to redo it, based on the latest Tk sources and/or Tk
 * canvasps patch.
 *
 * PWD (again): Redone for tk8.4 as the default printing (at least there is
 * some now) includes the empty padding once more. Using a new image
 * postscript proc will not work as there isn't enough information passed in
 * to determine the canvas viewport.
 *
 * To enable this extension, call TkCanvasPsImage_Init().
 *
 * who             when      what
 * --------------  --------  ----------------------------------------
 * Allan Brighton  19/06/98  Created
 * Peter W. Draper 05/05/06  Added support for tk8.4 as zoomed images
 *                           generate massive postscript files full of
 *                           padding around the rtdimage.
 */

#include <stdio.h>
#ifdef HAVE_TKCANVAS_H
#include "tclInt.h"
#include "tkCanvas.h"
#else
// The structure we need hasn't changed for a long time, so just include a local copy.
#include "tkCanvas.h-tk8.4.11"
#define HAVE_TKCANVAS_H 
#endif

/* Local prototypes */
static int RtdImageToPostscript( Tcl_Interp *interp, Tk_Canvas canvas,
                                 Tk_Item *itemPtr, int prepass );

/* Private function we add to Tk. */
Tk_ItemType *getTkImageType();

 
/*
 * --------------------------------------------------------------
 *
 * TkCanvasPsImage_Init --
 *
 *      This procedure is called to initialize the canvas image
 *      postscript extension by setting a field in the canvas
 *      image type control struct, causing our postscript routine
 *      to be called for canvas images.
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      None.
 *
 * --------------------------------------------------------------
 */

void
TkCanvasPsImage_Init()
{
    /*  Need to access the Tk Image type and supercede the Postscript
     *  command. */
    Tk_ItemType *tkImageType = getTkImageType();
    tkImageType->postscriptProc = RtdImageToPostscript;
}

/*
 * Note we do it this way, rather than by defining an image postscript proc,
 * as we do not have the necessary handles to canvases etc. passed to a
 * postscript proc.
 */

/*
 * The structure below defines the record for each image item (lifted from
 * tkCanvImg.c).
 */

typedef struct ImageItem  {
    Tk_Item header;             /* Generic stuff that's the same for all
                                 * types.  MUST BE FIRST IN STRUCTURE. */
    Tk_Canvas canvas;           /* Canvas containing the image. */
    double x, y;                /* Coordinates of positioning point for
                                 * image. */
    Tk_Anchor anchor;           /* Where to anchor image relative to
                                 * (x,y). */
    char *imageString;          /* String describing -image option (malloc-ed).
                                 * NULL means no image right now. */
    char *activeImageString;    /* String describing -activeimage option.
                                 * NULL means no image right now. */
    char *disabledImageString;  /* String describing -disabledimage option.
                                 * NULL means no image right now. */
    Tk_Image image;             /* Image to display in window, or NULL if
                                 * no image at present. */
    Tk_Image activeImage;       /* Image to display in window, or NULL if
                                 * no image at present. */
    Tk_Image disabledImage;     /* Image to display in window, or NULL if
                                 * no image at present. */
} ImageItem;

/*
 *--------------------------------------------------------------
 *
 * RtdImageToPostscript --
 *
 *      This procedure is called to generate Postscript for
 *      image items.
 *
 * Results:
 *      The return value is a standard Tcl result.  If an error
 *      occurs in generating Postscript then an error message is
 *      left in interp->result, replacing whatever used to be there.
 *      If no error occurs, then Postscript for the item is appended
 *      to the result.
 *
 * Side effects:
 *      None.
 *
 *--------------------------------------------------------------
 */

static int
RtdImageToPostscript(interp, canvas, itemPtr, prepass)
    Tcl_Interp *interp;                 /* Leave Postscript or error message
                                         * here. */
    Tk_Canvas canvas;                   /* Information about overall canvas. */
    Tk_Item *itemPtr;                   /* Item for which Postscript is
                                         * wanted. */
    int prepass;                        /* 1 means this is a prepass to
                                         * collect font information;  0 means
                                         * final Postscript is being created.*/
{
    ImageItem *imgPtr = (ImageItem *)itemPtr;
    Tk_Window canvasWin = Tk_CanvasTkwin(canvas);

    char buffer[256];
    double x, y;
    int width, height;
    Tk_Image image;
    Tk_State state = itemPtr->state;
    int screenX1, screenY1, screenX2, screenY2;
    TkCanvas *canvasPtr = (TkCanvas *) canvas;

    if (state == TK_STATE_NULL) {
        state = ((TkCanvas *)canvas)->canvas_state;
    }

    image = imgPtr->image;
    if (((TkCanvas *)canvas)->currentItemPtr == itemPtr) {
        if (imgPtr->activeImage != NULL) {
            image = imgPtr->activeImage;
        }
    } else if (state == TK_STATE_DISABLED) {
        if (imgPtr->disabledImage != NULL) {
            image = imgPtr->disabledImage;
        }
    }
    if (image == NULL) {
        /*
         * Image item without actual image specified.
         */
        return TCL_OK;
    }

    Tk_SizeOfImage(image, &width, &height);

    /*  Determine region of image that needs to be drawn. For rtdimages only
     *  the part visible on the display screen is done. */
    screenX1 = canvasPtr->xOrigin + canvasPtr->inset;
    screenY1 = canvasPtr->yOrigin + canvasPtr->inset;
    screenX2 = canvasPtr->xOrigin + Tk_Width(canvasWin) - canvasPtr->inset;
    screenY2 = canvasPtr->yOrigin + Tk_Height(canvasWin) - canvasPtr->inset;
    if ( width > screenX2 - screenX1 ) {
        width = screenX2 - screenX1;
    } else {
        screenX1 = 0;
    }

    if ( height > screenY2 - screenY1 ) {
        height = screenY2 - screenY1;
    } else {
        screenY1 = 0;
    }

    /*
     * Compute the coordinates of the lower-left corner of the image,
     * taking into account the anchor position for the image.
     */

    x = imgPtr->x;
    y = Tk_CanvasPsY(canvas, imgPtr->y);

    switch (imgPtr->anchor) {
        case TK_ANCHOR_NW:                      y -= height;            break;
        case TK_ANCHOR_N:       x -= width/2.0; y -= height;            break;
        case TK_ANCHOR_NE:      x -= width;     y -= height;            break;
        case TK_ANCHOR_E:       x -= width;     y -= height/2.0;        break;
        case TK_ANCHOR_SE:      x -= width;                             break;
        case TK_ANCHOR_S:       x -= width/2.0;                         break;
        case TK_ANCHOR_SW:                                              break;
        case TK_ANCHOR_W:                       y -= height/2.0;        break;
        case TK_ANCHOR_CENTER:  x -= width/2.0; y -= height/2.0;        break;
    }

    if (!prepass) {
        sprintf(buffer, "%.15g %.15g", x, y);
        Tcl_AppendResult(interp, buffer, " translate\n", (char *) NULL);
    }

    return Tk_PostscriptImage(image, interp, canvasWin,
                              ((TkCanvas *) canvas)->psInfo, 
                              screenX1, screenY1, width, height, prepass);
}
