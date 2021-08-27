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

#define  TESTER                           1
#define  TESTER_IMAGELIST                 2       /* control type: textMsg, callback function: (none) */
#define  TESTER_CHECKBOX                  3       /* control type: radioButton, callback function: (none) */
#define  TESTER_TAB                       4       /* control type: tab, callback function: Cb_ChangeTab */

     /* tab page panel controls */
#define  TABPANEL_BKTREE                  2       /* control type: activeX, callback function: (none) */
#define  TABPANEL_PICTURE                 3       /* control type: picture, callback function: (none) */
#define  TABPANEL_HOME_STATIONS           4       /* control type: command, callback function: OnFncTester */
#define  TABPANEL_LED_HOME_STATIONS       5       /* control type: pictRing, callback function: (none) */
#define  TABPANEL_STOP_TABLE              6       /* control type: command, callback function: OnFncTester */
#define  TABPANEL_LED_STOP_TABLE          7       /* control type: pictRing, callback function: (none) */
#define  TABPANEL_ROTATE_TABLE            8       /* control type: command, callback function: OnFncTester */
#define  TABPANEL_TABLE_LED               9       /* control type: pictRing, callback function: (none) */
#define  TABPANEL_LED_6                   10      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_TEXTMSG_6               11      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_LED_5                   12      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_TEXTMSG_5               13      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_LED_4                   14      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_TEXTMSG_4               15      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_LED_3                   16      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_TEXTMSG_3               17      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_LED_2                   18      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_TEXTMSG_2               19      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_LED_1                   20      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_TEXTMSG                 21      /* control type: textMsg, callback function: (none) */

     /* tab page panel controls */
#define  TABPANEL_1_BKTREE                2       /* control type: activeX, callback function: (none) */
#define  TABPANEL_1_PICTURE               3       /* control type: picture, callback function: (none) */
#define  TABPANEL_1_BARCODE               4       /* control type: command, callback function: OnFncStation1 */
#define  TABPANEL_1_READ_ANALOG_SENSOR    5       /* control type: command, callback function: OnFncStation1 */
#define  TABPANEL_1_CODE_STRING           6       /* control type: textBox, callback function: (none) */
#define  TABPANEL_1_ANALOG_SENSOR_AI3     7       /* control type: numeric, callback function: (none) */
#define  TABPANEL_1_ANALOG_SENSOR_AI2     8       /* control type: numeric, callback function: (none) */
#define  TABPANEL_1_ANALOG_SENSOR_AI1     9       /* control type: numeric, callback function: (none) */

     /* tab page panel controls */
#define  TABPANEL_2_BKTREE                2       /* control type: activeX, callback function: (none) */
#define  TABPANEL_2_PICTURE               3       /* control type: picture, callback function: (none) */
#define  TABPANEL_2_DISCONNECT_ALL        4       /* control type: command, callback function: OnFncStation2 */
#define  TABPANEL_2_LED_DISCONNECT_ALL    5       /* control type: pictRing, callback function: (none) */
#define  TABPANEL_2_LED_2E                6       /* control type: pictRing, callback function: (none) */
#define  TABPANEL_2_LED_2D                7       /* control type: pictRing, callback function: (none) */
#define  TABPANEL_2_LED_2E_2D             8       /* control type: pictRing, callback function: (none) */
#define  TABPANEL_2_LED_2B_2D             9       /* control type: pictRing, callback function: (none) */
#define  TABPANEL_2_LED_2C_2B             10      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_2_LED_2C                11      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_2_LED_2B                12      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_2_UNPLUG                13      /* control type: command, callback function: OnFncStation2 */
#define  TABPANEL_2_LED_UNPLUG            14      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_2_PLUG                  15      /* control type: command, callback function: OnFncStation2 */
#define  TABPANEL_2_LED_PLUG              16      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_2_CMD_RELAY_2E_2D       17      /* control type: binary, callback function: OnFncStation2 */
#define  TABPANEL_2_CMD_RELAY_2D          18      /* control type: binary, callback function: OnFncStation2 */
#define  TABPANEL_2_CMD_RELAY_2B_2D       19      /* control type: binary, callback function: OnFncStation2 */
#define  TABPANEL_2_CMD_RELAY_2E          20      /* control type: binary, callback function: OnFncStation2 */
#define  TABPANEL_2_CMD_RELAY_2B_2C       21      /* control type: binary, callback function: OnFncStation2 */
#define  TABPANEL_2_CMD_RELAY_2C          22      /* control type: binary, callback function: OnFncStation2 */
#define  TABPANEL_2_CMD_RELAY_2B          23      /* control type: binary, callback function: OnFncStation2 */

     /* tab page panel controls */
#define  TABPANEL_3_BKTREE                2       /* control type: activeX, callback function: (none) */
#define  TABPANEL_3_PICTURE               3       /* control type: picture, callback function: (none) */
#define  TABPANEL_3_DISCONNECT_ALL        4       /* control type: command, callback function: OnFncStation3 */
#define  TABPANEL_3_LED_DISCONNECT_ALL    5       /* control type: pictRing, callback function: (none) */
#define  TABPANEL_3_V_UNPLUG              6       /* control type: command, callback function: OnFncStation3 */
#define  TABPANEL_3_LED_V_UNPLUG          7       /* control type: pictRing, callback function: (none) */
#define  TABPANEL_3_V_PLUG                8       /* control type: command, callback function: OnFncStation3 */
#define  TABPANEL_3_LED_V_PLUG            9       /* control type: pictRing, callback function: (none) */
#define  TABPANEL_3_UNPLUG                10      /* control type: command, callback function: OnFncStation3 */
#define  TABPANEL_3_LED_UNPLUG            11      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_3_PLUG                  12      /* control type: command, callback function: OnFncStation3 */
#define  TABPANEL_3_LED_PLUG              13      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_3_CMD_RELAY_AC_PSupply  14      /* control type: binary, callback function: OnFncStation3 */
#define  TABPANEL_3_CMD_RELAY_3L          15      /* control type: binary, callback function: OnFncStation3 */
#define  TABPANEL_3_CMD_RELAY_3O          16      /* control type: binary, callback function: OnFncStation3 */
#define  TABPANEL_3_LED_3R                17      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_3_CMD_RELAY_3M          18      /* control type: binary, callback function: OnFncStation3 */
#define  TABPANEL_3_LED_3Q                19      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_3_LED_3L                20      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_3_CMD_RELAY_3Q_3Q2      21      /* control type: binary, callback function: OnFncStation3 */
#define  TABPANEL_3_CMD_RELAY_3H          22      /* control type: binary, callback function: OnFncStation3 */
#define  TABPANEL_3_LED_3P                23      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_3_LED_3O                24      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_3_CMD_RELAY_3P          25      /* control type: binary, callback function: OnFncStation3 */
#define  TABPANEL_3_CMD_RELAY_DBUS_3F     26      /* control type: binary, callback function: OnFncStation3 */
#define  TABPANEL_3_LED_3N                27      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_3_LED_3M                28      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_3_CMD_RELAY_3N          29      /* control type: binary, callback function: OnFncStation3 */
#define  TABPANEL_3_CMD_RELAY_FTA1        30      /* control type: binary, callback function: OnFncStation3 */
#define  TABPANEL_3_LED_3I                31      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_3_LED_3H                32      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_3_CMD_RELAY_3I          33      /* control type: binary, callback function: OnFncStation3 */
#define  TABPANEL_3_LED_FTA4              34      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_3_LED_3G                35      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_3_LED_3F                36      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_3_CMD_RELAY_3G          37      /* control type: binary, callback function: OnFncStation3 */
#define  TABPANEL_3_LED_FTA3              38      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_3_LED_FTA2              39      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_3_LED_FTA1              40      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_3_CMD_RELAY_FTA2        41      /* control type: binary, callback function: OnFncStation3 */
#define  TABPANEL_3_CMD_RELAY_FTA4        42      /* control type: binary, callback function: OnFncStation3 */
#define  TABPANEL_3_CMD_RELAY_FTA3        43      /* control type: binary, callback function: OnFncStation3 */

     /* tab page panel controls */
#define  TABPANEL_4_BKTREE                2       /* control type: activeX, callback function: (none) */
#define  TABPANEL_4_PICTURE               3       /* control type: picture, callback function: (none) */
#define  TABPANEL_4_UNPLUG                4       /* control type: command, callback function: OnFncStation4 */
#define  TABPANEL_4_LED_UNPLUG            5       /* control type: pictRing, callback function: (none) */
#define  TABPANEL_4_PLUG                  6       /* control type: command, callback function: OnFncStation4 */
#define  TABPANEL_4_LED_PLUG              7       /* control type: pictRing, callback function: (none) */
#define  TABPANEL_4_CMD_RELAY_4R          8       /* control type: binary, callback function: OnFncStation4 */
#define  TABPANEL_4_LED_4R                9       /* control type: pictRing, callback function: (none) */
#define  TABPANEL_4_CMD_RELAY_4H          10      /* control type: binary, callback function: OnFncStation4 */
#define  TABPANEL_4_LED_4H                11      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_4_CMD_RELAY_4V          12      /* control type: binary, callback function: OnFncStation4 */
#define  TABPANEL_4_LED_4V                13      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_4_CMD_RELAY_4U          14      /* control type: binary, callback function: OnFncStation4 */
#define  TABPANEL_4_LED_4U                15      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_4_CMD_RELAY_4W          16      /* control type: binary, callback function: OnFncStation4 */
#define  TABPANEL_4_LED_4W                17      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_4_CMD_RELAY_4Z          18      /* control type: binary, callback function: OnFncStation4 */
#define  TABPANEL_4_LED_4Z                19      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_4_CMD_RELAY_4T          20      /* control type: binary, callback function: OnFncStation4 */
#define  TABPANEL_4_LED_4T                21      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_4_CMD_RELAY_4S          22      /* control type: binary, callback function: OnFncStation4 */
#define  TABPANEL_4_LED_4S                23      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_4_CMD_RELAY_4B          24      /* control type: binary, callback function: OnFncStation4 */
#define  TABPANEL_4_LED_4B                25      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_4_CMD_RELAY_4A          26      /* control type: binary, callback function: OnFncStation4 */
#define  TABPANEL_4_LED_4A                27      /* control type: pictRing, callback function: (none) */

     /* tab page panel controls */
#define  TABPANEL_5_BKTREE                2       /* control type: activeX, callback function: (none) */
#define  TABPANEL_5_PICTURE               3       /* control type: picture, callback function: (none) */
#define  TABPANEL_5_UNPLUG                4       /* control type: command, callback function: OnFncStation5 */
#define  TABPANEL_5_LED_UNPLUG            5       /* control type: pictRing, callback function: (none) */
#define  TABPANEL_5_PLUG                  6       /* control type: command, callback function: OnFncStation5 */
#define  TABPANEL_5_LED_PLUG              7       /* control type: pictRing, callback function: (none) */
#define  TABPANEL_5_CMD_RELAY_5C          8       /* control type: binary, callback function: OnFncStation5 */
#define  TABPANEL_5_LED_5C                9       /* control type: pictRing, callback function: (none) */
#define  TABPANEL_5_CMD_RELAY_5D          10      /* control type: binary, callback function: OnFncStation5 */
#define  TABPANEL_5_LED_5D                11      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_5_CMD_RELAY_5N          12      /* control type: binary, callback function: OnFncStation5 */
#define  TABPANEL_5_LED_5N                13      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_5_CMD_RELAY_5H          14      /* control type: binary, callback function: OnFncStation5 */
#define  TABPANEL_5_LED_5H                15      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_5_CMD_RELAY_5G          16      /* control type: binary, callback function: OnFncStation5 */
#define  TABPANEL_5_LED_5G                17      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_5_CMD_RELAY_5M          18      /* control type: binary, callback function: OnFncStation5 */
#define  TABPANEL_5_LED_5M                19      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_5_CMD_RELAY_5L          20      /* control type: binary, callback function: OnFncStation5 */
#define  TABPANEL_5_LED_5L                21      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_5_CMD_RELAY_4T          22      /* control type: binary, callback function: OnFncStation5 */
#define  TABPANEL_5_LED_4T                23      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_5_CMD_RELAY_5I          24      /* control type: binary, callback function: OnFncStation5 */
#define  TABPANEL_5_LED_5I                25      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_5_CMD_RELAY_5E          26      /* control type: binary, callback function: OnFncStation5 */
#define  TABPANEL_5_LED_5E                27      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_5_CMD_RELAY_5F          28      /* control type: binary, callback function: OnFncStation5 */
#define  TABPANEL_5_LED_5F                29      /* control type: pictRing, callback function: (none) */

     /* tab page panel controls */
#define  TABPANEL_6_BKTREE                2       /* control type: activeX, callback function: (none) */
#define  TABPANEL_6_PICTURE               3       /* control type: picture, callback function: (none) */
#define  TABPANEL_6_ACTUATOR_Z_DOWN       4       /* control type: command, callback function: OnFncStation6 */
#define  TABPANEL_6_Z_DOWN_LED            5       /* control type: pictRing, callback function: (none) */
#define  TABPANEL_6_ACTUATOR_Z_UP         6       /* control type: command, callback function: OnFncStation6 */
#define  TABPANEL_6_Z_UP_LED              7       /* control type: pictRing, callback function: (none) */
#define  TABPANEL_6_ACTUATOR_X_FRONT      8       /* control type: command, callback function: OnFncStation6 */
#define  TABPANEL_6_X_FRONT_LED           9       /* control type: pictRing, callback function: (none) */
#define  TABPANEL_6_ACTUATOR_X_REAR       10      /* control type: command, callback function: OnFncStation6 */
#define  TABPANEL_6_X_REAR_LED            11      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_6_CLOSE_GRIPPER         12      /* control type: command, callback function: OnFncStation6 */
#define  TABPANEL_6_LED_GRIPPER_CLOSED    13      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_6_STOP_CONVOYER         14      /* control type: command, callback function: OnFncStation6 */
#define  TABPANEL_6_LED_CONVOYER_STOP     15      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_6_CMD_CONVOYER          16      /* control type: command, callback function: OnFncStation6 */
#define  TABPANEL_6_LED_CONVOYER          17      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_6_OPEN_GRIPPER          18      /* control type: command, callback function: OnFncStation6 */
#define  TABPANEL_6_LED_GRIPPER_OPEN      19      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_6_PRINT                 20      /* control type: command, callback function: OnFncStation6 */
#define  TABPANEL_6_LED_PRINT             21      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_6_BARCODE               22      /* control type: command, callback function: OnFncStation6 */
#define  TABPANEL_6_READY                 23      /* control type: command, callback function: OnFncStation6 */
#define  TABPANEL_6_LED_READY             24      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_6_CODE_STRING           25      /* control type: textBox, callback function: (none) */
#define  TABPANEL_6_HOME_PICK_PLACE       26      /* control type: command, callback function: OnFncStation6 */
#define  TABPANEL_6_LED_HOME_PICK_PLACE   27      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_6_PICK_READ             28      /* control type: command, callback function: OnFncStation6 */
#define  TABPANEL_6_LED_PICK_READ         29      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_6_PICK_PRINT            30      /* control type: command, callback function: OnFncStation6 */
#define  TABPANEL_6_LED_PICK_PRINT        31      /* control type: pictRing, callback function: (none) */
#define  TABPANEL_6_PICK_PLACE            32      /* control type: command, callback function: OnFncStation6 */
#define  TABPANEL_6_LED_PICK_PLACE        33      /* control type: pictRing, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Cb_ChangeTab(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFncStation1(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFncStation2(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFncStation3(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFncStation4(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFncStation5(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFncStation6(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFncTester(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif