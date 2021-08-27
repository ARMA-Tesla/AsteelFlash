/*============================================================================*/
/*                        L a b W i n d o w s / C V I                         */
/*----------------------------------------------------------------------------*/
/*    Copyright (c) National Instruments 1987-1999.  All Rights Reserved.     */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Title:       pwctrl.h                                                      */
/* Purpose:     implements password controls                                  */
/*                                                                            */
/*============================================================================*/

#ifndef PASSWORD_CTRL_HEADER
#define PASSWORD_CTRL_HEADER

#include <cvidef.h>

#ifdef __cplusplus
    extern "C" {
#endif


#define ATTR_PASSWORD_MAX_LENGTH       1000
#define ATTR_PASSWORD_VAL              1001
#define ATTR_PASSWORD_MASK_CHARACTER   1002

#define MAX_POSSIBLE_PASSWORD_LENGTH    255

int CVIFUNC PasswordCtrl_Create(int panel, const char *label, int top, int left);
int CVIFUNC PasswordCtrl_ConvertFromString(int panel, int stringCtrl);
int CVIFUNC PasswordCtrl_RevertToString(int panel, int control);
int CVIFUNC_C PasswordCtrl_SetAttribute(int panel, int passwordCtrl, int attribute, ...);
int CVIFUNC PasswordCtrl_GetAttribute(int panel, int passwordCtrl, int attribute, void *value);

#ifdef __cplusplus
    }
#endif

#endif /* PASSWORD_CTRL_HEADER */

