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
#define  PANEL_LOGIN                      2       /* control type: string, callback function: (none) */
#define  PANEL_PASSWORD                   3       /* control type: string, callback function: OnPassword */
#define  PANEL_LOGINLIST                  4       /* control type: ring, callback function: OnLoginList */
#define  PANEL_TEXTMSG                    5       /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_3                  6       /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_2                  7       /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK OnLoginList(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPassword(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif