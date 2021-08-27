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

#define  GEN                              1
#define  GEN_ERROR                        2       /* control type: textBox, callback function: (none) */
#define  GEN_READ_TEXT                    3       /* control type: textBox, callback function: (none) */
#define  GEN_SEND_TEXT                    4       /* control type: textBox, callback function: (none) */
#define  GEN_LED_CONNECT                  5       /* control type: deco, callback function: (none) */
#define  GEN_CLEAR_ERROR                  6       /* control type: command, callback function: OnGen */
#define  GEN_SYSTEM_ERROR                 7       /* control type: command, callback function: OnGen */
#define  GEN_SET_FUNCTION                 8       /* control type: command, callback function: OnGen */
#define  GEN_SET_OUTPUT                   9       /* control type: command, callback function: OnGen */
#define  GEN_MODE_CONT                    10      /* control type: command, callback function: OnGen */
#define  GEN_READ                         11      /* control type: command, callback function: OnGen */
#define  GEN_SEND                         12      /* control type: command, callback function: OnGen */
#define  GEN_DISCONNECT                   13      /* control type: command, callback function: OnGen */
#define  GEN_CONNECT                      14      /* control type: command, callback function: OnGen */
#define  GEN_SET_DUTY                     15      /* control type: numeric, callback function: (none) */
#define  GEN_SET_OFFSET                   16      /* control type: numeric, callback function: (none) */
#define  GEN_SET_FREQUENCY                17      /* control type: numeric, callback function: (none) */
#define  GEN_SET_AMPLITUDE                18      /* control type: numeric, callback function: (none) */
#define  GEN_LED_SET_FUNCTION             19      /* control type: deco, callback function: (none) */
#define  GEN_LED_OUTPUT                   20      /* control type: deco, callback function: (none) */
#define  GEN_LED_MODE_CONT                21      /* control type: deco, callback function: (none) */
#define  GEN_OUTPUT                       22      /* control type: binary, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK OnGen(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif