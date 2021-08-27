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

#define  MSG_ERROR                        1
#define  MSG_ERROR_TABLE                  2       /* control type: table, callback function: (none) */
#define  MSG_ERROR_DECORATION             3       /* control type: deco, callback function: (none) */
#define  MSG_ERROR_EXIT                   4       /* control type: command, callback function: OnErrorExit */

#define  MSG_SAP                          2
#define  MSG_SAP_PICTURE                  2       /* control type: picture, callback function: (none) */
#define  MSG_SAP_TRANSLATED_BARCODE       3       /* control type: string, callback function: (none) */
#define  MSG_SAP_EXIT                     4       /* control type: pictButton, callback function: OnErrorExit */
#define  MSG_SAP_STRING_4                 5       /* control type: string, callback function: (none) */
#define  MSG_SAP_STRING_5                 6       /* control type: string, callback function: (none) */
#define  MSG_SAP_STRING_3                 7       /* control type: string, callback function: (none) */
#define  MSG_SAP_BARCODE                  8       /* control type: string, callback function: (none) */
#define  MSG_SAP_DECORATION               9       /* control type: deco, callback function: (none) */
#define  MSG_SAP_DECORATION_2             10      /* control type: deco, callback function: (none) */

#define  PANEL                            3
#define  PANEL_TIMER_2                    2       /* control type: timer, callback function: OnTimerTick */
#define  PANEL_SAP                        3       /* control type: pictRing, callback function: OnSAP */
#define  PANEL_MSG_TIME                   4       /* control type: textMsg, callback function: (none) */
#define  PANEL_MSG_DATE                   5       /* control type: textMsg, callback function: (none) */
#define  PANEL_BOX                        6       /* control type: pictRing, callback function: OnBox */
#define  PANEL_ERROR                      7       /* control type: pictRing, callback function: OnError */
#define  PANEL_PRINTER                    8       /* control type: pictRing, callback function: OnPrinter */
#define  PANEL_BOX_TXT                    9       /* control type: textMsg, callback function: (none) */
#define  PANEL_TIMER_BOX                  10      /* control type: timer, callback function: OnTimerBox */
#define  PANEL_MSG_ADMIN_2                11      /* control type: textMsg, callback function: (none) */
#define  PANEL_SAP_ERR_LABEL_5            12      /* control type: textMsg, callback function: (none) */
#define  PANEL_MSG_ADMIN                  13      /* control type: textMsg, callback function: (none) */
#define  PANEL_SAP_ERR_LABEL_4            14      /* control type: textMsg, callback function: (none) */
#define  PANEL_SAP_ERR_LABEL_2            15      /* control type: textMsg, callback function: (none) */
#define  PANEL_SAP_ERR_LABEL              16      /* control type: textMsg, callback function: (none) */
#define  PANEL_SAP_ERROR                  17      /* control type: numeric, callback function: (none) */
#define  PANEL_TIMER_PRINTER              18      /* control type: timer, callback function: (none) */
#define  PANEL_TIMER                      19      /* control type: timer, callback function: OnTimer */

#define  SAP_ERROR                        4
#define  SAP_ERROR_TIME                   2       /* control type: numeric, callback function: (none) */
#define  SAP_ERROR_STRING                 3       /* control type: textBox, callback function: (none) */
#define  SAP_ERROR_TIMER                  4       /* control type: timer, callback function: OnSAPErrorTimer */
#define  SAP_ERROR_PICTURE                5       /* control type: picture, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK OnBox(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnError(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnErrorExit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPrinter(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSAP(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSAPErrorTimer(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnTimer(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnTimerBox(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnTimerTick(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif