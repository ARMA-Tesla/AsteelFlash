/*
 *  <elStr.h> - Definitions for module "elStr.c".
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

/* forward reference: */
struct SelStr;
typedef struct SelStr elStr_t, *elPStr_t, *elStrPtr_t;

#if !defined(_EL_STR_H)
#define _EL_STR_H

#if _MSC_VER < 1400 && !__BCOPT__ && !_CVI_  /* Borland C/C++: Unknown preprocessor directive: 'ident' */
#if 1
#ident "@(#) elStr.h 0.1 2007-03-05"
#else
#endif
#endif

#include <elBase.h>
#include <elObj.h>

#define EL_STR_ALLOC_STEP		64

enum {
	_EL_STR_IDX_TMP,
	_EL_STR_IDX_LOG,
	_EL_STR_IDX_TIMER,
	_EL_STR_IDX_ERR,
	_EL_STR_IDX_ERR_USR1,
	_EL_STR_IDX_ERR_USR2,
	_EL_STR_IDX_ERR_WIN1,
	_EL_STR_IDX_ERR_WIN2,
	_EL_STR_IDX_ERR_WSA1,
	_EL_STR_IDX_ERR_WSA2,
	_EL_STR_IDX_SOCKET,

	EL_STR_IDX_cnt,

	EL_STR_IDX_TMP_cnt = 20,

	EL_STR_IDX_rsv = -1,

	EL_STR_IDX_TMP		= (EL_STR_IDX_rsv - _EL_STR_IDX_TMP),
	EL_STR_IDX_LOG		= (EL_STR_IDX_rsv - _EL_STR_IDX_LOG),
	EL_STR_IDX_TIMER	= (EL_STR_IDX_rsv - _EL_STR_IDX_TIMER),
	EL_STR_IDX_ERR		= (EL_STR_IDX_rsv - _EL_STR_IDX_ERR),
	EL_STR_IDX_ERR_USR1	= (EL_STR_IDX_rsv - _EL_STR_IDX_ERR_USR1),
	EL_STR_IDX_ERR_USR2	= (EL_STR_IDX_rsv - _EL_STR_IDX_ERR_USR2),
	EL_STR_IDX_ERR_WIN1	= (EL_STR_IDX_rsv - _EL_STR_IDX_ERR_WIN1),
	EL_STR_IDX_ERR_WIN2	= (EL_STR_IDX_rsv - _EL_STR_IDX_ERR_WIN2),
	EL_STR_IDX_ERR_WSA1	= (EL_STR_IDX_rsv - _EL_STR_IDX_ERR_WSA1),
	EL_STR_IDX_ERR_WSA2	= (EL_STR_IDX_rsv - _EL_STR_IDX_ERR_WSA2),
	EL_STR_IDX_SOCKET	= (EL_STR_IDX_rsv - _EL_STR_IDX_SOCKET),
};

/* forward reference: */
struct SelObj;
typedef struct SelObj elObj_t, *elPObj_t, *elObjPtr_t;
struct SelErr;
typedef struct SelErr elErr_t, *elPErr_t, *elErrPtr_t;

/*
 * 'elStr_t': String data type.
 *
 *	alloc	size	ptr
 *	---------------------------------------------------------------------
 *	FALSE	   0	pointer to constant string literal (e.g. '__FUNC__')
 *					(usually located in code segment)
 *	FALSE	 > 0	pointer to array of characters of constant size
 *					(located in data segment)
 *	TRUE	 > 0	pointer to dynamically allocated space of size 'size'
 */
struct SelStr {
	elObj_t		obj;

	TCHAR		*ptr;
	size_t		size;
	bool		alloc;
	size_t		alloc_step;

	/* methods: */
	TCHAR	*(*Ptr)(elStr_t *str);
	int		(*Len)(elStr_t *str);
	elErr_t	*(*Empty)(elStr_t *str);
	elErr_t	*(*Cpy)(elStr_t *str,TCHAR *s);
	elErr_t	*(*NCpy)(elStr_t *str,TCHAR *s,size_t len);
	elErr_t	*(*Cat)(elStr_t *str,TCHAR *s);
	elErr_t	*(*CatFmtV)(elStr_t *str,const TCHAR *fmt,va_list args);
	elErr_t	*(*CatFmt)(elStr_t *str,const TCHAR *fmt,...);
	elErr_t	*(*FmtAtV)(elStr_t *str,size_t idx,const TCHAR *fmt,va_list args);
	elErr_t	*(*FmtAt)(elStr_t *str,size_t idx,const TCHAR *fmt,...);
	elErr_t	*(*FmtV)(elStr_t *str,const TCHAR *fmt,va_list args);
	elErr_t	*(*Fmt)(elStr_t *str,const TCHAR *fmt,...);
	elErr_t	*(*Printf)(elStr_t *str,const TCHAR *fmt,...);
	elErr_t	*(*PrintfAt)(elStr_t *str,size_t idx,const TCHAR *fmt,...);

#if 0
	/* small emergency static buffer in case of memory allocation problems: */
	// see 'char _s_data[MAX_ELSTRING_STATIC_LENGTH];' in C:\work\ELCOM\VALEO\UTSystem\include\elstring.h
	TCHAR	buf[64+EL_ST];
#endif
};

#define elStrP(obj_var)		elP(Str,obj_var)

#if !defined(EL_STR_MODULE)
EL_DLL_IMPORT extern const elObjClass_t *elStr_c;
#endif

#ifdef __cplusplus
extern "C" {			/* C Plus Plus function bindings */
#endif

#if _MSC_VER < 1400
int		_vscprintf(const TCHAR *fmt,va_list args);
#endif
EL_DLL_API elErr_t *elStrStartup(void);
EL_DLL_API elErr_t *elStrCleanup(void);

EL_DLL_API elErr_t *elStrAlloc(elStr_t *str,size_t size);
EL_DLL_API TCHAR   *elStrPtr(elStr_t *str);
EL_DLL_API int		elStrLen(elStr_t *str);
EL_DLL_API elErr_t *elStrEmpty(elStr_t *str);
EL_DLL_API elErr_t *elStrCpy(elStr_t *str,TCHAR *s);
EL_DLL_API elErr_t *elStrNCpy(elStr_t *str,TCHAR *s,size_t len);
EL_DLL_API elErr_t *elStrCat(elStr_t *str,TCHAR *s);
EL_DLL_API elErr_t *elStrCatFmtV(elStr_t *str,const TCHAR *fmt,va_list args);
EL_DLL_API elErr_t *elStrCatFmt(elStr_t *str,const TCHAR *fmt,...);
EL_DLL_API elErr_t	*elStrFmtV(elStr_t *str,const TCHAR *fmt,va_list args);
EL_DLL_API elErr_t	*elStrFmt(elStr_t *str,const TCHAR *fmt,...);
EL_DLL_API elErr_t *elStrFmtAtV(
	elStr_t *str,size_t idx,const TCHAR *fmt,va_list args
);
EL_DLL_API elErr_t *elStrFmtAt(elStr_t *str,size_t idx,const TCHAR *fmt,...);
EL_DLL_API elErr_t *elStrPrintfAt(elStr_t *str,size_t idx,const TCHAR *fmt,...);
EL_DLL_API elErr_t *elStrPrintf(elStr_t *str,const TCHAR *fmt,...);

EL_DLL_API elStr_t *elStrTmpIdxGet(int idx);
EL_DLL_API TCHAR	*elStrTmpIdxFmtV(int idx,const TCHAR *fmt,va_list args);
EL_DLL_API TCHAR	*elStrTmpIdxFmt(int idx,const TCHAR *fmt,...);
EL_DLL_API TCHAR	*elStrTmpFmtV(const TCHAR *fmt,va_list args);
EL_DLL_API TCHAR	*elStrTmpFmt(const TCHAR *fmt,...);
EL_DLL_API char		*elStrUnicode2Ansi(
	wchar_t *ustr,char *astr,size_t astr_size
);
EL_DLL_API wchar_t	*elStrAnsi2Unicode(
	char *astr,wchar_t *ustr,size_t ustr_size
);

#ifdef __cplusplus
}						/* C Plus Plus function bindings */
#endif

#endif 	/* _EL_STR_H */
