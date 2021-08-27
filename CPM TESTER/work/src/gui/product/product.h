/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2020. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PRODUCT                          1
#define  PRODUCT_LIST                     2       /* control type: listBox, callback function: OnList */
#define  PRODUCT_SEARCH                   3       /* control type: string, callback function: (none) */
#define  PRODUCT_PATH                     4       /* control type: string, callback function: (none) */
#define  PRODUCT_PICT                     5       /* control type: picture, callback function: (none) */
#define  PRODUCT_PICT_2                   6       /* control type: picture, callback function: (none) */
#define  PRODUCT_RING                     7       /* control type: ring, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK OnList(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
