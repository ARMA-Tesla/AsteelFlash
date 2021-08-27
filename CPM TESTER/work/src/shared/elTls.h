/*
 *  <elTls.h> - Definitions for module "elTls.c".
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

#if !defined(_EL_TLS_H)
#define _EL_TLS_H

#if _MSC_VER < 1400 && !__BCOPT__ && !_CVI_  /* Borland C/C++: Unknown preprocessor directive: 'ident' */
#if 1
#ident "@(#) elTls.h 0.1 2007-03-05"
#else
#endif
#endif

#include <elBase.h>
#include <elErr.h>

enum {
	EL_TLS_IDX_ERR,
	EL_TLS_IDX_STR,

	EL_TLS_IDX_cnt = 10
};

#ifdef __cplusplus
extern "C" {			/* C Plus Plus function bindings */
#endif

EL_DLL_API elErr_t *elTlsStartup(void);
EL_DLL_API elErr_t *elTlsCleanup(void);
EL_DLL_API elErr_t *elTlsValSet(int idx,void *val);
EL_DLL_API elErr_t *elTlsValGet(int idx,void **p_val);

#ifdef __cplusplus
}						/* C Plus Plus function bindings */
#endif

#endif 	/* _EL_TLS_H */
