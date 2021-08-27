/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1
#define  PANEL_PAUSE_PICT                 2       /* control type: picture, callback function: (none) */
#define  PANEL_PAUSE_START                3       /* control type: numeric, callback function: (none) */
#define  PANEL_TIMER                      4       /* control type: timer, callback function: OnTimerTick */
#define  PANEL_MSG_BAD                    5       /* control type: textMsg, callback function: (none) */
#define  PANEL_NUM_BAD                    6       /* control type: textMsg, callback function: (none) */
#define  PANEL_MSG_GOOD                   7       /* control type: textMsg, callback function: (none) */
#define  PANEL_NUM_GOOD                   8       /* control type: textMsg, callback function: (none) */
#define  PANEL_PALETT_STATION_69          9       /* control type: pictRing, callback function: (none) */
#define  PANEL_MSG_TOTAL                  10      /* control type: textMsg, callback function: (none) */
#define  PANEL_NUM_TOTAL                  11      /* control type: textMsg, callback function: (none) */
#define  PANEL_PALETT_STATION_68          12      /* control type: pictRing, callback function: (none) */
#define  PANEL_RESET_COUNTER              13      /* control type: command, callback function: OnResetCounter */
#define  PANEL_DECORATION_2               14      /* control type: deco, callback function: (none) */
#define  PANEL_PALETT_STATION_67          15      /* control type: pictRing, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK OnResetCounter(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnTimerTick(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif