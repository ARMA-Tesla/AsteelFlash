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

#define  TRACA                            1
#define  TRACA_LED_CONNECT                2       /* control type: deco, callback function: (none) */
#define  TRACA_SET_LAST_MVT               3       /* control type: command, callback function: OnMvt */
#define  TRACA_GET_SMM_SN                 4       /* control type: command, callback function: OnSMM */
#define  TRACA_TEST_SMM                   5       /* control type: command, callback function: OnSMM */
#define  TRACA_GET_LAST_MVT               6       /* control type: command, callback function: OnMvt */
#define  TRACA_FIELD_LABEL                7       /* control type: command, callback function: OnFieldLabel */
#define  TRACA_GET_BOARD_VER_PFIN         8       /* control type: command, callback function: OnTraca */
#define  TRACA_REFERENCE_PRODUCT          9       /* control type: command, callback function: OnTraca */
#define  TRACA_BOARD_VER_SFIN_ACTIVE      10      /* control type: command, callback function: OnTraca */
#define  TRACA_BOARD_VER_SFIN_VALIDE      11      /* control type: command, callback function: OnTraca */
#define  TRACA_DISCONNECT                 12      /* control type: command, callback function: OnConnection */
#define  TRACA_CONNECT                    13      /* control type: command, callback function: OnConnection */
#define  TRACA_LED_ACTIVE_SFIN            14      /* control type: LED, callback function: (none) */
#define  TRACA_LED_VALIDE_SFIN            15      /* control type: LED, callback function: (none) */
#define  TRACA_PRODUCT_REFERENCE          16      /* control type: string, callback function: (none) */
#define  TRACA_BOARD_VER_PFIN             17      /* control type: string, callback function: (none) */
#define  TRACA_DATE_FIN_PFIN              18      /* control type: string, callback function: (none) */
#define  TRACA_DATE_DEBUT_PFIN            19      /* control type: string, callback function: (none) */
#define  TRACA_ID_MACHINE_PFIN            20      /* control type: string, callback function: (none) */
#define  TRACA_TYPE_TEST_PFIN             21      /* control type: string, callback function: (none) */
#define  TRACA_NUM_SERIE_SMM              22      /* control type: string, callback function: (none) */
#define  TRACA_NUM_SERIE_PFIN             23      /* control type: string, callback function: (none) */
#define  TRACA_TYPE_TEST_SFIN             24      /* control type: string, callback function: (none) */
#define  TRACA_RESULT_SFIN_2              25      /* control type: LED, callback function: (none) */
#define  TRACA_NUM_SERIE_SFIN_2           26      /* control type: string, callback function: (none) */
#define  TRACA_NUM_SERIE_SFIN             27      /* control type: string, callback function: (none) */
#define  TRACA_BOARD_VER_SFIN             28      /* control type: string, callback function: (none) */
#define  TRACA_ID_BOARD_PFIN              29      /* control type: numeric, callback function: (none) */
#define  TRACA_RESULT_SFIN                30      /* control type: LED, callback function: (none) */
#define  TRACA_LED_WITH_SMM               31      /* control type: LED, callback function: (none) */
#define  TRACA_LED_VALIDE                 32      /* control type: LED, callback function: (none) */
#define  TRACA_ID_BOARD_SFIN              33      /* control type: numeric, callback function: (none) */
#define  TRACA_FIELD_LABEL_DATA           34      /* control type: table, callback function: (none) */
#define  TRACA_RESULT_PFIN                35      /* control type: binary, callback function: (none) */
#define  TRACA_DECORATION                 36      /* control type: deco, callback function: (none) */
#define  TRACA_DECORATION_2               37      /* control type: deco, callback function: (none) */
#define  TRACA_DECORATION_4               38      /* control type: deco, callback function: (none) */
#define  TRACA_DECORATION_5               39      /* control type: deco, callback function: (none) */
#define  TRACA_DECORATION_3               40      /* control type: deco, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK OnConnection(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFieldLabel(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnMvt(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSMM(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnTraca(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif