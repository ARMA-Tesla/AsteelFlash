/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2016. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PAUSE                            1       /* callback function: OnPausePanel */
#define  PAUSE_BUTTON_1                   2       /* control type: pictButton, callback function: OnPauseSelect */
#define  PAUSE_BUTTON_4                   3       /* control type: pictButton, callback function: OnPauseSelect */
#define  PAUSE_BUTTON_3                   4       /* control type: pictButton, callback function: OnPauseSelect */
#define  PAUSE_BUTTON_2                   5       /* control type: pictButton, callback function: OnPauseSelect */
#define  PAUSE_TIME                       6       /* control type: textMsg, callback function: (none) */
#define  PAUSE_PICTURE_2                  7       /* control type: picture, callback function: (none) */
#define  PAUSE_HIGHLIGHT                  8       /* control type: deco, callback function: (none) */
#define  PAUSE_MSG_PAUSE                  9       /* control type: textMsg, callback function: (none) */
#define  PAUSE_START                      10      /* control type: numeric, callback function: (none) */
#define  PAUSE_TIMER                      11      /* control type: timer, callback function: OnTimerTick */
#define  PAUSE_SPLITTER                   12      /* control type: splitter, callback function: (none) */
#define  PAUSE_SPLITTER_2                 13      /* control type: splitter, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK OnPausePanel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPauseSelect(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnTimerTick(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
