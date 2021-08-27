/*
 *  <elMem.h> - Definitions for module "elMem.c".
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

#if !defined(_EL_MEM_H)
#define _EL_MEM_H

#if _MSC_VER < 1400 && !__BCOPT__ && !_CVI_  /* Borland C/C++: Unknown preprocessor directive: 'ident' */
#if 1
#ident "@(#) elMem.h 0.1 2007-03-05"
#else
#endif
#endif

#include <elBase.h>
#include <elErr.h>
#if WIN && _DEBUG
/*
 * '_CRTDBG_MAP_ALLOC':
 *
 * Log/Display also file names and line numbers in memory leak dump list:
 * ('_CrtDumpMemoryLeaks()')
 */
#if 1
#define _CRTDBG_MAP_ALLOC
#endif
#include <crtdbg.h>
#include <elLog.h>
#endif

#if WIN && _DEBUG
#define EL_MEM_CHECK_CRT()													\
		if ( !_CrtCheckMemory() )											\
			elLog(_T("%s(%d) %s(): _CrtCheckMemory() failed\n"),			\
				_T(__FILE__),__LINE__,_T(__FUNC__));						\
		else

#else
#define EL_MEM_CHECK_CRT()
#endif


#ifdef __cplusplus
extern "C" {			/* C Plus Plus function bindings */
#endif

#if defined(_CRTDBG_MAP_ALLOC)
#define elMemAlloc(ptr,size)		(										\
			(*ptr = malloc(size)),											\
			*ptr ? NULL : elErrSet(											\
				&err,EL_ERR_MEM,(elErrId_t)-1,(elErrId_t)-1,				\
				_T(__FILE__),__LINE__,										\
				_T(__FUNC__),_T("malloc"),									\
				_T("cannot allocate memory (%ld bytes)"),size				\
			)																\
		)
#define elMemAllocZero(ptr,size)	(										\
			(*ptr = malloc(size)),											\
			*ptr ? (memset(*ptr,0,size), NULL) : elErrSet(					\
				&err,EL_ERR_MEM,(elErrId_t)-1,(elErrId_t)-1,				\
				_T(__FILE__),__LINE__,										\
				_T(__FUNC__),_T("malloc"),									\
				_T("cannot allocate memory (%ld bytes)"),size				\
			)																\
		)
#define elMemRealloc(ptr,size)		(										\
			(*(void **)ptr = realloc(*(void **)ptr,size)),					\
			*(void **)ptr ? NULL : elErrSet(								\
				&err,EL_ERR_MEM,(elErrId_t)-1,(elErrId_t)-1,				\
				_T(__FILE__),__LINE__,										\
				_T(__FUNC__),_T("realloc"),									\
				_T("cannot allocate memory (%ld bytes)"),size				\
			)																\
		)
#define elMemFree(ptr)		(												\
			!((void **)ptr) ?												\
				(void)0 : (free(*(void **)ptr), *(void **)ptr = NULL)		\
		)
#define elMemStrdup(pstr,str)		(										\
			!(str) ? (*(char **)pstr = NULL, NULL) : (						\
				(*(char **)pstr = strdup(str)),								\
				(															\
					*(char **)pstr ? NULL : elErrSet(						\
						&err,EL_ERR_MEM,(elErrId_t)-1,(elErrId_t)-1,		\
						_T(__FILE__),__LINE__,								\
						_T(__FUNC__),_T("strdup"),							\
						_T("cannot allocate memory (%ld bytes)"),			\
						strlen(str)											\
					)														\
				)															\
			)																\
		)
#else
EL_DLL_API elErr_t *elMemAlloc(void *ptr,size_t size);
EL_DLL_API elErr_t *elMemAllocZero(void *ptr,size_t size);
EL_DLL_API elErr_t *elMemRealloc(void *ptr,size_t size);
EL_DLL_API void	 elMemFree(void *ptr);
EL_DLL_API elErr_t *elMemStrdup(TCHAR **p_dupstr,TCHAR *str);
#endif

#ifdef __cplusplus
}						/* C Plus Plus function bindings */
#endif

#endif 	/* _EL_MEM_H */
