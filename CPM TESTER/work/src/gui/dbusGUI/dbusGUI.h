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

#define  DBUSGUI                          1
#define  DBUSGUI_COMM_LIST                2       /* control type: textBox, callback function: (none) */
#define  DBUSGUI_RECEIVE                  3       /* control type: command, callback function: OnFncReceive */
#define  DBUSGUI_RECEIVE_TARGET_ADRESS    4       /* control type: numeric, callback function: (none) */
#define  DBUSGUI_RECEIVE_MSG_DATA         5       /* control type: string, callback function: (none) */
#define  DBUSGUI_RECEIVE_MSG_ID           6       /* control type: string, callback function: (none) */
#define  DBUSGUI_SEND                     7       /* control type: command, callback function: OnFncSend */
#define  DBUSGUI_SEND_TARGET_ADRESS       8       /* control type: numeric, callback function: (none) */
#define  DBUSGUI_SEND_MSG_DATA            9       /* control type: string, callback function: (none) */
#define  DBUSGUI_UDA_LIST                 10      /* control type: ring, callback function: (none) */
#define  DBUSGUI_SEND_MSG_ID              11      /* control type: string, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK OnFncReceive(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFncSend(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif