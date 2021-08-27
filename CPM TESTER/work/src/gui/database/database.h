/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2019. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  DBS                              1
#define  DBS_GROUP1                       2       /* control type: ring, callback function: OnGroup */
#define  DBS_GROUP2                       3       /* control type: ring, callback function: OnGroup */
#define  DBS_GROUP3                       4       /* control type: ring, callback function: OnGroup */
#define  DBS_GROUP4                       5       /* control type: ring, callback function: OnGroup */
#define  DBS_GROUP5                       6       /* control type: ring, callback function: OnGroup */
#define  DBS_PICTURE5                     7       /* control type: picture, callback function: (none) */
#define  DBS_PICTURE4                     8       /* control type: picture, callback function: (none) */
#define  DBS_PICTURE3                     9       /* control type: picture, callback function: (none) */
#define  DBS_PICTURE2                     10      /* control type: picture, callback function: (none) */
#define  DBS_BKTREE                       11      /* control type: activeX, callback function: OnBKTREE */
#define  DBS_PICTURE1                     12      /* control type: picture, callback function: (none) */
#define  DBS_MEMORY                       13      /* control type: numeric, callback function: (none) */
#define  DBS_TIMER                        14      /* control type: numeric, callback function: (none) */
#define  DBS_IMAGELIST1                   15      /* control type: activeX, callback function: (none) */
#define  DBS_IMAGELIST2                   16      /* control type: activeX, callback function: (none) */
#define  DBS_IMAGELIST3                   17      /* control type: activeX, callback function: (none) */
#define  DBS_USERPRODUCT                  18      /* control type: string, callback function: (none) */
#define  DBS_USERNAME                     19      /* control type: string, callback function: (none) */
#define  DBS_RIGHTS_TXT                   20      /* control type: textMsg, callback function: (none) */
#define  DBS_USERRIGHTS                   21      /* control type: textBox, callback function: (none) */
#define  DBS_TEXTBOX                      22      /* control type: textBox, callback function: (none) */
#define  DBS_IMAGELIST4                   23      /* control type: activeX, callback function: (none) */
#define  DBS_TAB                          24      /* control type: tab, callback function: Cb_ChangeTab */

#define  DLG_USER                         2
#define  DLG_USER_NAME                    2       /* control type: string, callback function: (none) */
#define  DLG_USER_PASSW1                  3       /* control type: string, callback function: (none) */
#define  DLG_USER_PASSW2                  4       /* control type: string, callback function: (none) */
#define  DLG_USER_IBUTTON                 5       /* control type: string, callback function: (none) */
#define  DLG_USER_RIGHTS                  6       /* control type: ring, callback function: (none) */
#define  DLG_USER_DRIGHTS_1               7       /* control type: radioButton, callback function: (none) */
#define  DLG_USER_DRIGHTS_2               8       /* control type: radioButton, callback function: (none) */
#define  DLG_USER_DRIGHTS_3               9       /* control type: radioButton, callback function: (none) */
#define  DLG_USER_DRIGHTS_4               10      /* control type: radioButton, callback function: (none) */
#define  DLG_USER_DRIGHTS_5               11      /* control type: radioButton, callback function: (none) */
#define  DLG_USER_TRIGHTS_1               12      /* control type: radioButton, callback function: OnRightsSelection */
#define  DLG_USER_TRIGHTS_2               13      /* control type: radioButton, callback function: OnRightsSelection */
#define  DLG_USER_TRIGHTS_3               14      /* control type: radioButton, callback function: OnRightsSelection */
#define  DLG_USER_TRIGHTS_4               15      /* control type: radioButton, callback function: OnRightsSelection */
#define  DLG_USER_TRIGHTS_5               16      /* control type: radioButton, callback function: OnRightsSelection */
#define  DLG_USER_EXIT_OK                 17      /* control type: pictButton, callback function: (none) */
#define  DLG_USER_EXIT_CANCEL             18      /* control type: pictButton, callback function: (none) */
#define  DLG_USER_DBS_TXT                 19      /* control type: textMsg, callback function: (none) */
#define  DLG_USER_TESTER_TXT              20      /* control type: textMsg, callback function: (none) */
#define  DLG_USER_DECORATION_2            21      /* control type: deco, callback function: (none) */
#define  DLG_USER_DECORATION              22      /* control type: deco, callback function: (none) */

#define  MASTER                           3
#define  MASTER_MASTER_ID                 2       /* control type: ring, callback function: (none) */
#define  MASTER_MASTER_CODE               3       /* control type: string, callback function: (none) */
#define  MASTER_EXIT_OK                   4       /* control type: pictButton, callback function: (none) */
#define  MASTER_EXIT_CANCEL               5       /* control type: pictButton, callback function: (none) */

#define  PARAM                            4
#define  PARAM_NB                         2       /* control type: string, callback function: (none) */
#define  PARAM_NAME                       3       /* control type: string, callback function: (none) */
#define  PARAM_DESCRIPT_2                 4       /* control type: string, callback function: (none) */
#define  PARAM_DESCRIPT                   5       /* control type: string, callback function: (none) */
#define  PARAM_VALUE                      6       /* control type: string, callback function: (none) */
#define  PARAM_VTYPE                      7       /* control type: ring, callback function: OnVtype */
#define  PARAM_NUMERIC                    8       /* control type: numeric, callback function: (none) */
#define  PARAM_TESTER                     9       /* control type: radioButton, callback function: (none) */
#define  PARAM_EXIT_OK                    10      /* control type: pictButton, callback function: (none) */
#define  PARAM_EXIT_CANCEL                11      /* control type: pictButton, callback function: (none) */
#define  PARAM_TEXTBOX                    12      /* control type: textBox, callback function: (none) */

#define  PRODUCT                          5
#define  PRODUCT_NAME                     2       /* control type: string, callback function: (none) */
#define  PRODUCT_DESCRIPT                 3       /* control type: string, callback function: (none) */
#define  PRODUCT_EXIT_OK                  4       /* control type: pictButton, callback function: (none) */
#define  PRODUCT_EXIT_CANCEL              5       /* control type: pictButton, callback function: (none) */
#define  PRODUCT_PARENT                   6       /* control type: string, callback function: (none) */
#define  PRODUCT_SETTINGS                 7       /* control type: radioButton, callback function: (none) */

#define  PRODUCT_CP                       6
#define  PRODUCT_CP_SOURCE                2       /* control type: ring, callback function: OnCopySourceSelect */
#define  PRODUCT_CP_SET_PROD_PARAM        3       /* control type: radioButton, callback function: OnCP */
#define  PRODUCT_CP_SET_PROD_MASTER       4       /* control type: radioButton, callback function: OnCP */
#define  PRODUCT_CP_SET_TEST              5       /* control type: radioButton, callback function: OnCP */
#define  PRODUCT_CP_SET_LINE_PARAM        6       /* control type: radioButton, callback function: OnCP */
#define  PRODUCT_CP_SET_LINE_STEP         7       /* control type: radioButton, callback function: OnCP */
#define  PRODUCT_CP_SET_LINE_PROCESS      8       /* control type: radioButton, callback function: OnCP */
#define  PRODUCT_CP_SET_TEST_PARAM        9       /* control type: radioButton, callback function: OnCP */
#define  PRODUCT_CP_EXIT_OK               10      /* control type: pictButton, callback function: (none) */
#define  PRODUCT_CP_EXIT_CANCEL           11      /* control type: pictButton, callback function: (none) */
#define  PRODUCT_CP_DECORATION_2          12      /* control type: deco, callback function: (none) */
#define  PRODUCT_CP_LISTBOX               13      /* control type: textBox, callback function: (none) */
#define  PRODUCT_CP_SETTING_TXT           14      /* control type: textMsg, callback function: (none) */

#define  TEST                             7
#define  TEST_STEP                        2       /* control type: string, callback function: (none) */
#define  TEST_NAME                        3       /* control type: string, callback function: (none) */
#define  TEST_DESCRIPT_2                  4       /* control type: string, callback function: (none) */
#define  TEST_DESCRIPT                    5       /* control type: string, callback function: (none) */
#define  TEST_EXIT_OK                     6       /* control type: pictButton, callback function: (none) */
#define  TEST_EXIT_CANCEL                 7       /* control type: pictButton, callback function: (none) */
#define  TEST_NUMERIC                     8       /* control type: numeric, callback function: (none) */

     /* tab page panel controls */
#define  TABPANEL_1_DB_INSERT             2       /* control type: pictButton, callback function: OnDbInsert */
#define  TABPANEL_1_DB_READ               3       /* control type: pictButton, callback function: OnDbRead */
#define  TABPANEL_1_CSV_IMPORT            4       /* control type: command, callback function: OnImportCsvFile */
#define  TABPANEL_1_CSV_OPEN              5       /* control type: command, callback function: OnOpenCsvFile */
#define  TABPANEL_1_CSV_FILE              6       /* control type: string, callback function: (none) */
#define  TABPANEL_1_XML_BCK               7       /* control type: deco, callback function: (none) */
#define  TABPANEL_1_TYPE                  8       /* control type: ring, callback function: (none) */
#define  TABPANEL_1_DECORATION            9       /* control type: deco, callback function: (none) */
#define  TABPANEL_1_MESSAGE               10      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_1_TIMER_HRUBCA          11      /* control type: timer, callback function: OnTimer */

     /* tab page panel controls */
#define  TABPANEL_2_CSV_IMPORT            2       /* control type: command, callback function: OnImportCsvFile */
#define  TABPANEL_2_CSV_OPEN              3       /* control type: command, callback function: OnOpenCsvFile */
#define  TABPANEL_2_CSV_FILE              4       /* control type: string, callback function: (none) */
#define  TABPANEL_2_XML_BCK               5       /* control type: deco, callback function: (none) */
#define  TABPANEL_2_DECORATION            6       /* control type: deco, callback function: (none) */
#define  TABPANEL_2_MESSAGE               7       /* control type: textMsg, callback function: (none) */

     /* tab page panel controls */
#define  TABPANEL_3_DTPICKER              2       /* control type: activeX, callback function: (none) */
#define  TABPANEL_3_PAGE                  3       /* control type: ring, callback function: OnHistList */
#define  TABPANEL_3_NEXT                  4       /* control type: command, callback function: OnHistList */
#define  TABPANEL_3_PREW                  5       /* control type: command, callback function: OnHistList */
#define  TABPANEL_3_SWITCH_PRODMASTER     6       /* control type: radioButton, callback function: OnHisFillter */
#define  TABPANEL_3_SWITCH_TEST           7       /* control type: radioButton, callback function: OnHisFillter */
#define  TABPANEL_3_SWITCH_TESTPARAM      8       /* control type: radioButton, callback function: OnHisFillter */
#define  TABPANEL_3_SWITCH_PROD           9       /* control type: radioButton, callback function: OnHisFillter */
#define  TABPANEL_3_SWITCH_PRODPARAM      10      /* control type: radioButton, callback function: OnHisFillter */
#define  TABPANEL_3_DECORATION            11      /* control type: deco, callback function: (none) */
#define  TABPANEL_3_SWITCH_USER           12      /* control type: radioButton, callback function: OnHisFillter */
#define  TABPANEL_3_MERGE                 13      /* control type: radioButton, callback function: OnHisMerge */
#define  TABPANEL_3_SWITCH_USERRIGHTS     14      /* control type: radioButton, callback function: OnHisFillter */

     /* tab page panel controls */
#define  TABPANEL_4_DTPICKER_2            2       /* control type: activeX, callback function: (none) */
#define  TABPANEL_4_DTPICKER_1            3       /* control type: activeX, callback function: (none) */
#define  TABPANEL_4_TO_FILE               4       /* control type: radioButton, callback function: (none) */
#define  TABPANEL_4_INI_LIMIT             5       /* control type: pictButton, callback function: OnIniLimits */
#define  TABPANEL_4_INI_PARAMETER         6       /* control type: pictButton, callback function: OnIniLimits */
#define  TABPANEL_4_INI_LIMIT_HIST        7       /* control type: pictButton, callback function: OnIniLimits */
#define  TABPANEL_4_DB_DELETE_HIST        8       /* control type: pictButton, callback function: OnDbDeleteHist */
#define  TABPANEL_4_INI_PARAMETER_HIST    9       /* control type: pictButton, callback function: OnIniLimits */
#define  TABPANEL_4_INI_USER              10      /* control type: pictButton, callback function: OnIniLimits */
#define  TABPANEL_4_DECORATION            11      /* control type: deco, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Cb_ChangeTab(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnBKTREE(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnCopySourceSelect(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnCP(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnDbDeleteHist(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnDbInsert(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnDbRead(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnGroup(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnHisFillter(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnHisMerge(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnHistList(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnImportCsvFile(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnIniLimits(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnOpenCsvFile(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnRightsSelection(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnTimer(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnVtype(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
