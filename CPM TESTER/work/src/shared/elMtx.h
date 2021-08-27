/*
 *  <elMtx.h> - Definitions for module "elMtx.c".
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

#if !defined(_EL_MTX_H)
#define _EL_MTX_H

#if _MSC_VER < 1400 && !__BCOPT__ && !_CVI_  /* Borland C/C++: Unknown preprocessor directive: 'ident' */
#if 1
#ident "@(#) elMtx.h 0.1 2007-03-05"
#else
#endif
#endif

#include <elBase.h>
#include <elErr.h>

typedef struct SelMtx {
	bool				init;
	bool				named;
#if WIN
	CRITICAL_SECTION	cs;
	bool				cs_init;
	HANDLE				handle;		/* 'CreateMutex()' handle */
#endif
} elMtx_t, *elPMtx_t, *elMtxPtr_t;

#ifdef __cplusplus
extern "C" {			/* C Plus Plus function bindings */
#endif

EL_DLL_API elErr_t *elMtxStartup(void);
EL_DLL_API elErr_t *elMtxCleanup(void);
EL_DLL_API elErr_t *elMtxNew(elMtxPtr_t mtx,char *name);
EL_DLL_API elErr_t *elMtxDelete(elMtxPtr_t mtx);
EL_DLL_API elErr_t *elMtxLock(elMtx_t *mtx);
EL_DLL_API elErr_t *elMtxUnlock(elMtx_t *mtx);

#ifdef __cplusplus
}						/* C Plus Plus function bindings */
#endif

#endif 	/* _EL_ERR_H */