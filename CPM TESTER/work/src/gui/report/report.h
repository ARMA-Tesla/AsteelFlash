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
#define  PANEL_CAL_DISPLAY                2       /* control type: table, callback function: OnCalDisplay */
#define  PANEL_MONTH_PREV                 3       /* control type: command, callback function: OnMonthChange */
#define  PANEL_MONTH_NEXT                 4       /* control type: command, callback function: OnMonthChange */
#define  PANEL_DECORATION                 5       /* control type: deco, callback function: (none) */
#define  PANEL_MONTH_DISPLAY              6       /* control type: textMsg, callback function: (none) */
#define  PANEL_FILELIST                   7       /* control type: table, callback function: OnFileList */
#define  PANEL_WEBBROWSER                 8       /* control type: activeX, callback function: (none) */
#define  PANEL_MODE                       9       /* control type: ring, callback function: OnMode */
#define  PANEL_STRING                     10      /* control type: string, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK OnCalDisplay(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFileList(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnMode(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnMonthChange(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif