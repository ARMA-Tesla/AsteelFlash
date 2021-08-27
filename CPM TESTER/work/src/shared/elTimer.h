/*
 *  <elTimer.h> - Definitions for module "elTimer.c".
 *
 *	(c) 2007 ELCOM, a.s.
 *		All Rights Reserved.
 *
 *	The information  in this file  is provided for the exclusive use of
 *	the  licensees  of  ELCOM, a.s.  Such users have the right  to use,
 *	modify,  and incorporate this code into other products for purposes
 *	authorized  by the license  agreement  provided they  include  this
 *	notice  and the associated copyright notice  with any such product.
 *	The information in this file  is provided "AS IS" without warranty.
 *
 *
 * Revision History:
 * -----------------
 *	2007-03-05	Radomir Tomis (Radomir.Tomis@dvi.elcom.cz)
 *		Created.
 */

#if _MSC_VER >= 1000
#pragma once
#endif 

#if !defined(_EL_TIMER_H)
#define _EL_TIMER_H

#if _MSC_VER < 1400 && !__BCOPT__ && !_CVI_  /* Borland C/C++: Unknown preprocessor directive: 'ident' */
#if 1
#ident "@(#) elTimer.h 0.1 2007-03-05"
#else
#endif
#endif

#include <elBase.h>

/*
 * 'elTimer_t' - timer (stop-watch) with resolution of 'elTimerRes()'.
 * 'EL_TIMER_T_FMTSZPFX' - format size prefix for printf/scanf-like routines.
 */
#if WIN
typedef unsigned __int64 elTimer_t;
typedef elTimer_t elTimerRes_t;
#define elTimer2double(t)	(double)(__int64)(t)
#define EL_TIMER_T_FMTSZPFX	"I64"
#elif DOS
typedef clock_t elTimer_t;
typedef double elTimerRes_t;
#define EL_TIMER_T_FMTSZPFX    "l"
#define elTimer2double(t) (double)(t)
#else
typedef unsigned long elTimer_t;
typedef elTimer_t elTimerRes_t;
#define EL_TIMER_T_FMTSZPFX	"l"
#define elTimer2double(t)	(double)(t)
#endif

#ifdef __cplusplus
extern "C" {			/* C Plus Plus function bindings */
#endif

EL_DLL_API elErr_t		*elTimerStartup(void);
EL_DLL_API elErr_t		*elTimerCleanup(void);
EL_DLL_API elErr_t		*elTimer(elTimer_t *pt);
EL_DLL_API elErr_t		*elTimerStart(elTimer_t *pt);
EL_DLL_API elErr_t		*elTimerStop(elTimer_t *pt);
EL_DLL_API elTimerRes_t	elTimerRes(void);
EL_DLL_API TCHAR		*elTimerStr(elTimer_t t,int width,int prec);

#ifdef __cplusplus
}						/* C Plus Plus function bindings */
#endif

#endif 	/* _EL_TIMER_H */
