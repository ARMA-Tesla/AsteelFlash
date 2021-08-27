/*
 *  <elErrWin.h> - Definitions for module "elErrWin.c".
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

#if !defined(_EL_ERR_WIN_H)
#define _EL_ERR_WIN_H

#if _MSC_VER < 1400 && !__BCOPT__ && !_CVI_  /* Borland C/C++: Unknown preprocessor directive: 'ident' */
#if 1
#ident "@(#) elErrWin.h 0.1 2007-03-05"
#else
#endif
#endif

#include <elBase.h>


#ifdef __cplusplus
extern "C" {			/* C Plus Plus function bindings */
#endif

EL_DLL_API elErr_t *elErrWinStartup(void);
EL_DLL_API elErr_t	*elErrWinCleanup(void);
EL_DLL_API bool	elErrWinHasStr(DWORD errnum);
EL_DLL_API TCHAR	*elErrWinSymStr(DWORD errnum);
EL_DLL_API DWORD	elErrWinSymNum(TCHAR *errsym);
EL_DLL_API TCHAR	*elErrWinStr(DWORD errnum);
EL_DLL_API TCHAR	*elErrGetLastErrorStr(void);
EL_DLL_API TCHAR	*elErrGetLastErrorText(void);

#ifdef __cplusplus
}						/* C Plus Plus function bindings */
#endif

#endif 	/* _EL_ERR_WIN_H */
