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

#define  DIELECT                          1
#define  DIELECT_ERROR                    2       /* control type: textBox, callback function: (none) */
#define  DIELECT_READ_TEXT                3       /* control type: textBox, callback function: (none) */
#define  DIELECT_SEND_TEXT                4       /* control type: textBox, callback function: (none) */
#define  DIELECT_LED_CONNECT              5       /* control type: deco, callback function: (none) */
#define  DIELECT_CLEAR_ERROR              6       /* control type: command, callback function: OnDielectric */
#define  DIELECT_SYSTEM_ERROR             7       /* control type: command, callback function: OnDielectric */
#define  DIELECT_TEST_RESULT              8       /* control type: command, callback function: OnDielectric */
#define  DIELECT_SET_OUTPUT               9       /* control type: command, callback function: OnDielectric */
#define  DIELECT_SET_STEP                 10      /* control type: command, callback function: OnDielectric */
#define  DIELECT_MODE_MAN                 11      /* control type: command, callback function: OnDielectric */
#define  DIELECT_READ                     12      /* control type: command, callback function: OnDielectric */
#define  DIELECT_SEND                     13      /* control type: command, callback function: OnDielectric */
#define  DIELECT_DISCONNECT               14      /* control type: command, callback function: OnDielectric */
#define  DIELECT_CONNECT                  15      /* control type: command, callback function: OnDielectric */
#define  DIELECT_TEST_CURRENT             16      /* control type: numeric, callback function: (none) */
#define  DIELECT_TEST_VOLTAGE             17      /* control type: numeric, callback function: (none) */
#define  DIELECT_STEP                     18      /* control type: ring, callback function: (none) */
#define  DIELECT_LED_OUTPUT               19      /* control type: deco, callback function: (none) */
#define  DIELECT_LED_STEP                 20      /* control type: deco, callback function: (none) */
#define  DIELECT_LED_MODE_MAN             21      /* control type: deco, callback function: (none) */
#define  DIELECT_LED_TEST_RESULT          22      /* control type: deco, callback function: (none) */
#define  DIELECT_OUTPUT                   23      /* control type: binary, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK OnDielectric(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif