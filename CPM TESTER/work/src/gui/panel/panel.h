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

#define  PANEL                            1       /* callback function: OnPanelCallback */
#define  PANEL_MENU_12                    2       /* control type: pictRing, callback function: OnPanel */
#define  PANEL_DECORATION_MENU_12         3       /* control type: deco, callback function: (none) */
#define  PANEL_SHORTCUT_MENU_12           4       /* control type: textMsg, callback function: (none) */
#define  PANEL_LABEL_MENU_12              5       /* control type: textMsg, callback function: (none) */
#define  PANEL_MENU_11                    6       /* control type: pictRing, callback function: OnPanel */
#define  PANEL_DECORATION_MENU_11         7       /* control type: deco, callback function: (none) */
#define  PANEL_SHORTCUT_MENU_11           8       /* control type: textMsg, callback function: (none) */
#define  PANEL_LABEL_MENU_11              9       /* control type: textMsg, callback function: (none) */
#define  PANEL_MENU_10                    10      /* control type: pictRing, callback function: OnPanel */
#define  PANEL_DECORATION_MENU_10         11      /* control type: deco, callback function: (none) */
#define  PANEL_SHORTCUT_MENU_10           12      /* control type: textMsg, callback function: (none) */
#define  PANEL_LABEL_MENU_10              13      /* control type: textMsg, callback function: (none) */
#define  PANEL_MENU_9                     14      /* control type: pictRing, callback function: OnPanel */
#define  PANEL_DECORATION_MENU_9          15      /* control type: deco, callback function: (none) */
#define  PANEL_SHORTCUT_MENU_9            16      /* control type: textMsg, callback function: (none) */
#define  PANEL_LABEL_MENU_9               17      /* control type: textMsg, callback function: (none) */
#define  PANEL_MENU_8                     18      /* control type: pictRing, callback function: OnPanel */
#define  PANEL_DECORATION_MENU_8          19      /* control type: deco, callback function: (none) */
#define  PANEL_SHORTCUT_MENU_8            20      /* control type: textMsg, callback function: (none) */
#define  PANEL_LABEL_MENU_8               21      /* control type: textMsg, callback function: (none) */
#define  PANEL_MENU_7                     22      /* control type: pictRing, callback function: OnPanel */
#define  PANEL_DECORATION_MENU_7          23      /* control type: deco, callback function: (none) */
#define  PANEL_SHORTCUT_MENU_7            24      /* control type: textMsg, callback function: (none) */
#define  PANEL_LABEL_MENU_7               25      /* control type: textMsg, callback function: (none) */
#define  PANEL_MENU_6                     26      /* control type: pictRing, callback function: OnPanel */
#define  PANEL_DECORATION_MENU_6          27      /* control type: deco, callback function: (none) */
#define  PANEL_SHORTCUT_MENU_6            28      /* control type: textMsg, callback function: (none) */
#define  PANEL_LABEL_MENU_6               29      /* control type: textMsg, callback function: (none) */
#define  PANEL_MENU_5                     30      /* control type: pictRing, callback function: OnPanel */
#define  PANEL_DECORATION_MENU_5          31      /* control type: deco, callback function: (none) */
#define  PANEL_SHORTCUT_MENU_5            32      /* control type: textMsg, callback function: (none) */
#define  PANEL_LABEL_MENU_5               33      /* control type: textMsg, callback function: (none) */
#define  PANEL_MENU_4                     34      /* control type: pictRing, callback function: OnPanel */
#define  PANEL_DECORATION_MENU_4          35      /* control type: deco, callback function: (none) */
#define  PANEL_SHORTCUT_MENU_4            36      /* control type: textMsg, callback function: (none) */
#define  PANEL_LABEL_MENU_4               37      /* control type: textMsg, callback function: (none) */
#define  PANEL_MENU_3                     38      /* control type: pictRing, callback function: OnPanel */
#define  PANEL_DECORATION_MENU_3          39      /* control type: deco, callback function: (none) */
#define  PANEL_SHORTCUT_MENU_3            40      /* control type: textMsg, callback function: (none) */
#define  PANEL_LABEL_MENU_3               41      /* control type: textMsg, callback function: (none) */
#define  PANEL_MENU_2                     42      /* control type: pictRing, callback function: OnPanel */
#define  PANEL_DECORATION_MENU_2          43      /* control type: deco, callback function: (none) */
#define  PANEL_SHORTCUT_MENU_2            44      /* control type: textMsg, callback function: (none) */
#define  PANEL_LABEL_MENU_2               45      /* control type: textMsg, callback function: (none) */
#define  PANEL_MENU_1                     46      /* control type: pictRing, callback function: OnPanel */
#define  PANEL_DECORATION_MENU_1          47      /* control type: deco, callback function: (none) */
#define  PANEL_SHORTCUT_MENU_1            48      /* control type: textMsg, callback function: (none) */
#define  PANEL_LABEL_MENU_1               49      /* control type: textMsg, callback function: (none) */

#define  PANEL_2                          2
#define  PANEL_2_PANEL                    2       /* control type: table, callback function: (none) */
#define  PANEL_2_TIMER                    3       /* control type: timer, callback function: (none) */
#define  PANEL_2_EXPAND                   4       /* control type: pictButton, callback function: Cb_Expand_Menu */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Cb_Expand_Menu(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPanel(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif