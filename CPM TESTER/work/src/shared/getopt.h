/*
 *  <getopt.h> - Definitions for module "getopt.c".
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

#if !defined(_EL_GETOPT_H)
#define _EL_GETOPT_H

#if _MSC_VER < 1400 && !__BCOPT__ && !_CVI_  /* Borland C/C++: Unknown preprocessor directive: 'ident' */
#if 1
#ident "@(#) getopt.h 0.1 2007-03-05"
#else
#endif
#endif

#ifdef __cplusplus
extern "C" {			/* C Plus Plus function bindings */
#endif

EL_DLL_API int		 GetOpt		__P((TCHAR *optstring,TCHAR *opts,TCHAR **arg));
EL_DLL_API TCHAR	*getwopt	__P((int argc,TCHAR * const argv[],
							const TCHAR *optstring,const TCHAR *woptstring));
EL_DLL_API int		 getopt		__P((int argc,
							TCHAR * const argv[],const TCHAR *optstring));
#if !defined(EL_GETOPT_MODULE)
EL_DLL_IMPORT extern int optind;
EL_DLL_IMPORT extern TCHAR *optarg;
#endif

#ifdef __cplusplus
}						/* C Plus Plus function bindings */
#endif

#endif 	/* _EL_GETOPT_H */
