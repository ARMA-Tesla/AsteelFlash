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

#define  POWER                            1
#define  POWER_LED_INITIALIZE             2       /* control type: deco, callback function: (none) */
#define  POWER_OCP                        3       /* control type: command, callback function: OnPower */
#define  POWER_CONFIG_OUT                 4       /* control type: command, callback function: OnPower */
#define  POWER_OVP                        5       /* control type: command, callback function: OnPower */
#define  POWER_ENABLE_OUT                 6       /* control type: command, callback function: OnPower */
#define  POWER_CONFIG_CURRENT_LIMIT       7       /* control type: command, callback function: OnPower */
#define  POWER_OVP_LIMIT                  8       /* control type: numeric, callback function: (none) */
#define  POWER_OCP_LIMIT                  9       /* control type: numeric, callback function: (none) */
#define  POWER_LED_OCP                    10      /* control type: deco, callback function: (none) */
#define  POWER_CLOSE                      11      /* control type: command, callback function: OnPower */
#define  POWER_LED_OVP                    12      /* control type: deco, callback function: (none) */
#define  POWER_INITIALIZE                 13      /* control type: command, callback function: OnPower */
#define  POWER_CURRENT_LIMIT              14      /* control type: numeric, callback function: (none) */
#define  POWER_VOLTAGE                    15      /* control type: numeric, callback function: (none) */
#define  POWER_LED_CONFIG_OUT             16      /* control type: deco, callback function: (none) */
#define  POWER_LED_OUTPUT                 17      /* control type: deco, callback function: (none) */
#define  POWER_LED_CONFIG_CUR_LIMIT       18      /* control type: deco, callback function: (none) */
#define  POWER_OUTPUT                     19      /* control type: binary, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK OnPower(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif