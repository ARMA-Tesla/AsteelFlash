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
#define  PANEL_PRODUCT_LEFT               2       /* control type: picture, callback function: (none) */
#define  PANEL_PICTURERING_28             3       /* control type: pictRing, callback function: (none) */
#define  PANEL_REPEAT                     4       /* control type: pictButton, callback function: OnButton */
#define  PANEL_BACKWARD                   5       /* control type: pictButton, callback function: OnButton */
#define  PANEL_FORWARD                    6       /* control type: pictButton, callback function: OnButton */
#define  PANEL_STOP                       7       /* control type: pictButton, callback function: OnButton */
#define  PANEL_TABLE                      8       /* control type: table, callback function: (none) */
#define  PANEL_PALETT_STATION_16          9       /* control type: pictRing, callback function: OnDisplay */
#define  PANEL_PALETT_STATION_60          10      /* control type: pictRing, callback function: OnDisplay */
#define  PANEL_PALETT_STATION_50          11      /* control type: pictRing, callback function: OnDisplay */
#define  PANEL_PALETT_STATION_40          12      /* control type: pictRing, callback function: OnDisplay */
#define  PANEL_PALETT_STATION_30          13      /* control type: pictRing, callback function: OnDisplay */
#define  PANEL_PALETT_STATION_20          14      /* control type: pictRing, callback function: OnDisplay */
#define  PANEL_SEQ_STATION_60             15      /* control type: pictRing, callback function: OnDisplay */
#define  PANEL_SEQ_STATION_50             16      /* control type: pictRing, callback function: OnDisplay */
#define  PANEL_SEQ_STATION_40             17      /* control type: pictRing, callback function: OnDisplay */
#define  PANEL_SEQ_STATION_30             18      /* control type: pictRing, callback function: OnDisplay */
#define  PANEL_SEQ_STATION_20             19      /* control type: pictRing, callback function: OnDisplay */
#define  PANEL_SEQ_STATION_10             20      /* control type: pictRing, callback function: OnDisplay */
#define  PANEL_PALETT_STATION_10          21      /* control type: pictRing, callback function: OnDisplay */
#define  PANEL_STATE_STATION_20           22      /* control type: pictRing, callback function: OnDisplay */
#define  PANEL_STATE_STATION_30           23      /* control type: pictRing, callback function: OnDisplay */
#define  PANEL_STATE_STATION_50           24      /* control type: pictRing, callback function: OnDisplay */
#define  PANEL_STATE_STATION_10           25      /* control type: pictRing, callback function: (none) */
#define  PANEL_STATE_STATION_60           26      /* control type: pictRing, callback function: OnDisplay */
#define  PANEL_STATE_STATION_40           27      /* control type: pictRing, callback function: OnDisplay */
#define  PANEL_CHECK_P6                   28      /* control type: textButton, callback function: (none) */
#define  PANEL_CHECK_P5                   29      /* control type: textButton, callback function: (none) */
#define  PANEL_CHECK_P4                   30      /* control type: textButton, callback function: (none) */
#define  PANEL_CHECK_P3                   31      /* control type: textButton, callback function: (none) */
#define  PANEL_CHECK_P2                   32      /* control type: textButton, callback function: (none) */
#define  PANEL_CHECK_P1                   33      /* control type: textButton, callback function: (none) */
#define  PANEL_FAULT_STATION_30           34      /* control type: textMsg, callback function: (none) */
#define  PANEL_FAULT_STATION_40           35      /* control type: textMsg, callback function: (none) */
#define  PANEL_FAULT_STATION_50           36      /* control type: textMsg, callback function: (none) */
#define  PANEL_FAULT_STATION_60           37      /* control type: textMsg, callback function: (none) */
#define  PANEL_FAULT_STATION_20           38      /* control type: textMsg, callback function: (none) */
#define  PANEL_FAULT_STATION_10           39      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_61                 40      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_58                 41      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_59                 42      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_55                 43      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_56                 44      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_52                 45      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_53                 46      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_49                 47      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_26                 48      /* control type: textMsg, callback function: (none) */
#define  PANEL_RESULT_MSG                 49      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_22                 50      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_21                 51      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_25                 52      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_23                 53      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_20                 54      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_10                 55      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_17                 56      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_16                 57      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_15                 58      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_14                 59      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_13                 60      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_48                 61      /* control type: textMsg, callback function: (none) */
#define  PANEL_TIMER_WS20                 62      /* control type: timer, callback function: OnTimerWS20 */
#define  PANEL_TIME_WS20                  63      /* control type: textMsg, callback function: (none) */
#define  PANEL_TIME_WS30                  64      /* control type: textMsg, callback function: (none) */
#define  PANEL_TIME_WS40                  65      /* control type: textMsg, callback function: (none) */
#define  PANEL_TIME_WS50                  66      /* control type: textMsg, callback function: (none) */
#define  PANEL_TIMER_WS30                 67      /* control type: timer, callback function: OnTimerWS30 */
#define  PANEL_TIME_WS60                  68      /* control type: textMsg, callback function: (none) */
#define  PANEL_TIMER_WS40                 69      /* control type: timer, callback function: OnTimerWS40 */
#define  PANEL_TIMER_WS60                 70      /* control type: timer, callback function: OnTimerWS60 */
#define  PANEL_TEXTMSG_24                 71      /* control type: textMsg, callback function: (none) */
#define  PANEL_TIMER_WS50                 72      /* control type: timer, callback function: OnTimerWS50 */
#define  PANEL_CONVOYER_MSG               73      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_11                 74      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_50                 75      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_64                 76      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_65                 77      /* control type: textMsg, callback function: (none) */
#define  PANEL_PRODUCT_PICTURE            78      /* control type: picture, callback function: (none) */
#define  PANEL_TEXTMSG_62                 79      /* control type: textMsg, callback function: (none) */

#define  TEST_10                          2
#define  TEST_10_EXIT                     2       /* control type: pictButton, callback function: OnFnc10 */
#define  TEST_10_STOP_RETEST              3       /* control type: pictButton, callback function: OnFnc10 */
#define  TEST_10_TEST10                   4       /* control type: pictButton, callback function: OnFnc10 */
#define  TEST_10_TABLE                    5       /* control type: table, callback function: (none) */
#define  TEST_10_TABLE_STATE              6       /* control type: pictRing, callback function: (none) */
#define  TEST_10_PALETT_STATION_70        7       /* control type: pictRing, callback function: OnDisplay */
#define  TEST_10_DECORATION_20            8       /* control type: pictRing, callback function: (none) */
#define  TEST_10_PALETT_STATION_69        9       /* control type: pictRing, callback function: OnDisplay */
#define  TEST_10_TEXTMSG_13               10      /* control type: textMsg, callback function: (none) */
#define  TEST_10_TEXTMSG_12               11      /* control type: textMsg, callback function: (none) */
#define  TEST_10_TEXTMSG_15               12      /* control type: textMsg, callback function: (none) */
#define  TEST_10_TEXTMSG_22               13      /* control type: textMsg, callback function: (none) */
#define  TEST_10_TEXTMSG_14               14      /* control type: textMsg, callback function: (none) */
#define  TEST_10_TEXTMSG_7                15      /* control type: textMsg, callback function: (none) */
#define  TEST_10_RETEST_FAIL              16      /* control type: textMsg, callback function: (none) */
#define  TEST_10_RETEST_PASS              17      /* control type: textMsg, callback function: (none) */
#define  TEST_10_RETEST_TOTAL             18      /* control type: textMsg, callback function: (none) */
#define  TEST_10_RETEST                   19      /* control type: numeric, callback function: (none) */
#define  TEST_10_GO_FAIL                  20      /* control type: binary, callback function: (none) */
#define  TEST_10_ACTIVE                   21      /* control type: binary, callback function: (none) */

#define  TEST_20                          3
#define  TEST_20_EXIT                     2       /* control type: pictButton, callback function: OnFnc20 */
#define  TEST_20_STOP_RETEST              3       /* control type: pictButton, callback function: OnFnc20 */
#define  TEST_20_TEST20                   4       /* control type: pictButton, callback function: OnFnc20 */
#define  TEST_20_TABLE                    5       /* control type: table, callback function: (none) */
#define  TEST_20_TABLE_STATE              6       /* control type: pictRing, callback function: (none) */
#define  TEST_20_PALETT_STATION_70        7       /* control type: pictRing, callback function: OnDisplay */
#define  TEST_20_PALETT_STATION_75        8       /* control type: pictRing, callback function: OnDisplay */
#define  TEST_20_PALETT_STATION_77        9       /* control type: pictRing, callback function: OnDisplay */
#define  TEST_20_PALETT_STATION_76        10      /* control type: pictRing, callback function: OnDisplay */
#define  TEST_20_DECORATION_20            11      /* control type: pictRing, callback function: (none) */
#define  TEST_20_PALETT_STATION_69        12      /* control type: pictRing, callback function: OnDisplay */
#define  TEST_20_TEXTMSG_13               13      /* control type: textMsg, callback function: (none) */
#define  TEST_20_COURANT_AC               14      /* control type: textMsg, callback function: (none) */
#define  TEST_20_VOLTAGE_AC               15      /* control type: textMsg, callback function: (none) */
#define  TEST_20_VOLTAGE_DC               16      /* control type: textMsg, callback function: (none) */
#define  TEST_20_TEXTMSG_12               17      /* control type: textMsg, callback function: (none) */
#define  TEST_20_TEXTMSG_15               18      /* control type: textMsg, callback function: (none) */
#define  TEST_20_TEXTMSG_22               19      /* control type: textMsg, callback function: (none) */
#define  TEST_20_TEXTMSG_14               20      /* control type: textMsg, callback function: (none) */
#define  TEST_20_TEXTMSG_7                21      /* control type: textMsg, callback function: (none) */
#define  TEST_20_RETEST_FAIL              22      /* control type: textMsg, callback function: (none) */
#define  TEST_20_RETEST_PASS              23      /* control type: textMsg, callback function: (none) */
#define  TEST_20_RETEST_TOTAL             24      /* control type: textMsg, callback function: (none) */
#define  TEST_20_RETEST                   25      /* control type: numeric, callback function: (none) */
#define  TEST_20_GO_FAIL                  26      /* control type: binary, callback function: (none) */
#define  TEST_20_TEXTBOX                  27      /* control type: textBox, callback function: (none) */
#define  TEST_20_ACTIVE                   28      /* control type: binary, callback function: (none) */
#define  TEST_20_VIEW_MORE_20             29      /* control type: command, callback function: OnFnc20 */

#define  TEST_30                          4
#define  TEST_30_EXIT                     2       /* control type: pictButton, callback function: OnFnc30 */
#define  TEST_30_STOP_RETEST              3       /* control type: pictButton, callback function: OnFnc30 */
#define  TEST_30_TEST30                   4       /* control type: pictButton, callback function: OnFnc30 */
#define  TEST_30_TABLE                    5       /* control type: table, callback function: (none) */
#define  TEST_30_TABLE_STATE              6       /* control type: pictRing, callback function: (none) */
#define  TEST_30_PALETT_STATION_70        7       /* control type: pictRing, callback function: OnDisplay */
#define  TEST_30_DECORATION_20            8       /* control type: pictRing, callback function: (none) */
#define  TEST_30_PALETT_STATION_69        9       /* control type: pictRing, callback function: OnDisplay */
#define  TEST_30_TEXTMSG_13               10      /* control type: textMsg, callback function: (none) */
#define  TEST_30_TEXTMSG_12               11      /* control type: textMsg, callback function: (none) */
#define  TEST_30_TEXTMSG_15               12      /* control type: textMsg, callback function: (none) */
#define  TEST_30_TEXTMSG_22               13      /* control type: textMsg, callback function: (none) */
#define  TEST_30_TEXTMSG_14               14      /* control type: textMsg, callback function: (none) */
#define  TEST_30_TEXTMSG_7                15      /* control type: textMsg, callback function: (none) */
#define  TEST_30_RETEST_FAIL              16      /* control type: textMsg, callback function: (none) */
#define  TEST_30_RETEST_PASS              17      /* control type: textMsg, callback function: (none) */
#define  TEST_30_RETEST_TOTAL             18      /* control type: textMsg, callback function: (none) */
#define  TEST_30_RETEST                   19      /* control type: numeric, callback function: (none) */
#define  TEST_30_GO_FAIL                  20      /* control type: binary, callback function: (none) */
#define  TEST_30_TEXTBOX                  21      /* control type: textBox, callback function: (none) */
#define  TEST_30_ACTIVE                   22      /* control type: binary, callback function: (none) */
#define  TEST_30_VIEW_MORE_30             23      /* control type: command, callback function: OnFnc30 */

#define  TEST_40                          5
#define  TEST_40_EXIT                     2       /* control type: pictButton, callback function: OnFnc10 */
#define  TEST_40_STOP_RETEST              3       /* control type: pictButton, callback function: OnFnc40 */
#define  TEST_40_TEST40                   4       /* control type: pictButton, callback function: OnFnc40 */
#define  TEST_40_VIEW_MORE_40             5       /* control type: pictButton, callback function: OnFnc40 */
#define  TEST_40_TABLE                    6       /* control type: table, callback function: (none) */
#define  TEST_40_TABLE_STATE              7       /* control type: pictRing, callback function: (none) */
#define  TEST_40_PALETT_STATION_70        8       /* control type: pictRing, callback function: OnDisplay */
#define  TEST_40_DECORATION_20            9       /* control type: pictRing, callback function: (none) */
#define  TEST_40_PALETT_STATION_69        10      /* control type: pictRing, callback function: OnDisplay */
#define  TEST_40_TEXTMSG_13               11      /* control type: textMsg, callback function: (none) */
#define  TEST_40_TEXTMSG_12               12      /* control type: textMsg, callback function: (none) */
#define  TEST_40_TEXTMSG_15               13      /* control type: textMsg, callback function: (none) */
#define  TEST_40_TEXTMSG_22               14      /* control type: textMsg, callback function: (none) */
#define  TEST_40_TEXTMSG_14               15      /* control type: textMsg, callback function: (none) */
#define  TEST_40_TEXTMSG_7                16      /* control type: textMsg, callback function: (none) */
#define  TEST_40_RETEST_FAIL              17      /* control type: textMsg, callback function: (none) */
#define  TEST_40_RETEST_PASS              18      /* control type: textMsg, callback function: (none) */
#define  TEST_40_RETEST_TOTAL             19      /* control type: textMsg, callback function: (none) */
#define  TEST_40_RETEST                   20      /* control type: numeric, callback function: (none) */
#define  TEST_40_GO_FAIL                  21      /* control type: binary, callback function: (none) */
#define  TEST_40_TEXTBOX                  22      /* control type: textBox, callback function: (none) */
#define  TEST_40_ACTIVE                   23      /* control type: binary, callback function: (none) */
#define  TEST_40_PRODUCT_RIGHT            24      /* control type: picture, callback function: (none) */

#define  TEST_50                          6
#define  TEST_50_EXIT                     2       /* control type: pictButton, callback function: OnFnc10 */
#define  TEST_50_STOP_RETEST              3       /* control type: pictButton, callback function: OnFnc50 */
#define  TEST_50_TEST50                   4       /* control type: pictButton, callback function: OnFnc50 */
#define  TEST_50_VIEW_MORE                5       /* control type: pictButton, callback function: OnFnc10 */
#define  TEST_50_TABLE                    6       /* control type: table, callback function: (none) */
#define  TEST_50_TABLE_STATE              7       /* control type: pictRing, callback function: (none) */
#define  TEST_50_PALETT_STATION_70        8       /* control type: pictRing, callback function: OnDisplay */
#define  TEST_50_DECORATION_20            9       /* control type: pictRing, callback function: (none) */
#define  TEST_50_PALETT_STATION_69        10      /* control type: pictRing, callback function: OnDisplay */
#define  TEST_50_TEXTMSG_13               11      /* control type: textMsg, callback function: (none) */
#define  TEST_50_TEXTMSG_12               12      /* control type: textMsg, callback function: (none) */
#define  TEST_50_TEXTMSG_15               13      /* control type: textMsg, callback function: (none) */
#define  TEST_50_TEXTMSG_22               14      /* control type: textMsg, callback function: (none) */
#define  TEST_50_TEXTMSG_14               15      /* control type: textMsg, callback function: (none) */
#define  TEST_50_TEXTMSG_7                16      /* control type: textMsg, callback function: (none) */
#define  TEST_50_RETEST_FAIL              17      /* control type: textMsg, callback function: (none) */
#define  TEST_50_RETEST_PASS              18      /* control type: textMsg, callback function: (none) */
#define  TEST_50_RETEST_TOTAL             19      /* control type: textMsg, callback function: (none) */
#define  TEST_50_RETEST                   20      /* control type: numeric, callback function: (none) */
#define  TEST_50_GO_FAIL                  21      /* control type: binary, callback function: (none) */
#define  TEST_50_ACTIVE                   22      /* control type: binary, callback function: (none) */

#define  TEST_60                          7
#define  TEST_60_EXIT                     2       /* control type: pictButton, callback function: OnFnc10 */
#define  TEST_60_STOP_RETEST              3       /* control type: pictButton, callback function: OnFnc60 */
#define  TEST_60_TEST60                   4       /* control type: pictButton, callback function: OnFnc60 */
#define  TEST_60_TABLE                    5       /* control type: table, callback function: (none) */
#define  TEST_60_TABLE_STATE              6       /* control type: pictRing, callback function: (none) */
#define  TEST_60_PALETT_STATION_70        7       /* control type: pictRing, callback function: OnDisplay */
#define  TEST_60_DECORATION_20            8       /* control type: pictRing, callback function: (none) */
#define  TEST_60_PALETT_STATION_69        9       /* control type: pictRing, callback function: OnDisplay */
#define  TEST_60_TEXTMSG_13               10      /* control type: textMsg, callback function: (none) */
#define  TEST_60_TEXTMSG_12               11      /* control type: textMsg, callback function: (none) */
#define  TEST_60_TEXTMSG_15               12      /* control type: textMsg, callback function: (none) */
#define  TEST_60_TEXTMSG_22               13      /* control type: textMsg, callback function: (none) */
#define  TEST_60_TEXTMSG_14               14      /* control type: textMsg, callback function: (none) */
#define  TEST_60_TEXTMSG_7                15      /* control type: textMsg, callback function: (none) */
#define  TEST_60_RETEST_FAIL              16      /* control type: textMsg, callback function: (none) */
#define  TEST_60_RETEST_PASS              17      /* control type: textMsg, callback function: (none) */
#define  TEST_60_RETEST_TOTAL             18      /* control type: textMsg, callback function: (none) */
#define  TEST_60_RETEST                   19      /* control type: numeric, callback function: (none) */
#define  TEST_60_GO_FAIL                  20      /* control type: binary, callback function: (none) */
#define  TEST_60_ACTIVE                   21      /* control type: binary, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK OnButton(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnDisplay(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFnc10(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFnc20(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFnc30(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFnc40(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFnc50(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFnc60(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnTimerWS20(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnTimerWS30(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnTimerWS40(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnTimerWS50(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnTimerWS60(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif