/*
 *  "elStr.c" - String routines.
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

#if 1
static char _ident[] = "@(#) elStr.c 0.1 2007-03-05";
#else
#endif

#define EL_STR_MODULE

#include <elBase.h>
#include <elTls.h>
#include <elStr.h>
#include <elErr.h>
#include <elLog.h>
#include <elMem.h>
#include <elMtx.h>
#include <errno.h>

#define STDIO_THREAD_SAFE	0

/*
 * 'ThreadData_t': Per thread data.
 *
 * Every thread has its own single copy of this structure.
 */
typedef struct {
#if !STDIO_THREAD_SAFE
	FILE		*fp_tmp;
#endif
	elMtx_t		lock;
	elStr_t		str_tmp[EL_STR_IDX_cnt+EL_STR_IDX_TMP_cnt];
	int			tmp_idx;
} ThreadData_t, *PThreadData_t, *ThreadDataPtr_t;

static struct {
#if STDIO_THREAD_SAFE
	FILE *fp_tmp;
#endif
	int		dummy;
} gs;

static elErr_t *elStrCtor(elObjPtr_t *po,va_list args);
static elErr_t *elStrDtor(elObjPtr_t *po);

static elObjClass_t gs_elStr_c = {
	sizeof(elStr_t),
	"elStr",
	elStrCtor,		/* Ctor()	*/
	elStrDtor,		/* Dtor()	*/
	NULL,			/* Clone()	*/
	NULL,			/* Differ()	*/
};

EL_DLL_EXPORT const elObjClass_t *elStr_c = &gs_elStr_c;

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elStr::thread_data_get"
static elErrId_t thread_data_get(PThreadData_t *p_td)
{
	EL_ERRID_VAR;
	PThreadData_t td;
	register int i;

	EL_ERRID_CHECK_COND( elTlsValGet(EL_TLS_IDX_STR,&td), EL_ERR_TLS );
#if 0
	elLogDbgStr("%s(TID=%lu): td: 0x%lX",
		__FUNC__,GetCurrentThreadId(),td);
#endif
	if ( !td )
	{
		EL_ERRID_CHECK_ALLOC( td = malloc(sizeof(*td)) );

		elLogDbgStr("%s(TID=%lu)",__FUNC__,GetCurrentThreadId());

		EL_ERRID_CHECK_COND( elTlsValSet(EL_TLS_IDX_STR,td), EL_ERR_TLS );

		memset(td,0,sizeof(*td));

		elMtxNew(&td->lock,NULL);

		for(i = 0; i < EL_ARRAY_CNT(td->str_tmp); i++)
			EL_NEW(elStr_c,&td->str_tmp[i]);
	}

	*p_td = td;

Error:

	return err;
}

/******************************************************************************/
/* non-WIN pr CVI or MSVC++ 6.0 or earlier: */
#if !WIN || _CVI_ || _MSC_VER <= 1200
/*!
 * \brief
 * Returns the number of characters in the formatted string using a pointer
 * to a list of arguments.
 * 
 * \param format 
 * Format-control string.
 *
 * \param argptr
 * Pointer to list of arguments.
 * 
 * \return 
 *	Returns the number of characters that would be generated if the string
 *	pointed to by the list of arguments was printed or sent to a file
 *	or buffer using the specified formatting codes. The value returned
 *	does not include the terminating null character.
 */
EL_DLL_API int _vscprintf(const TCHAR *fmt,va_list args)
#undef  __FUNC__
#define __FUNC__ "vscprintf"
{
	FILE *fp_tmp;
	int len;

	/*
	 * If stdio routines are completely thread-safe (even when 2 asynchronous
	 * stdio calls are executing with the same 'FILE *' pointer), then the
	 * separate temporary file pointers (one for each thread) are not
	 * necessary.
	 */
#if STDIO_THREAD_SAFE
	fp_tmp = gs.fp_tmp;
#else
	{	/* inner block */
	ThreadData_t *td;

	/* get pointer to current thread's data: */
	if ( thread_data_get(&td) != EL_OK )
	{
		errno = ENOMEM;
		return -1;
	}

	if ( !td->fp_tmp )
		td->fp_tmp = tmpfile();

	fp_tmp = td->fp_tmp;
	}	/* inner block */
#endif

	if ( fp_tmp )
	{
		len = vfprintf(fp_tmp,fmt,args);
		fseek(fp_tmp,0,SEEK_SET);
		return len; 
	}
	else
	{
		errno = ENOMEM;
		return -1;
	}
}
#endif

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elStrStartup"
EL_DLL_API elErr_t *elStrStartup(void)
{
	EL_ERR_VAR;

	;

#if 0
Error:
#endif

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elStrCleanup"
EL_DLL_API elErr_t *elStrCleanup(void)
{
	EL_ERR_VAR;
	ThreadData_t	*td;
	register int	i;
	
	if ( thread_data_get(&td) == EL_OK )
	{
		elLogDbgStr("%s(TID=%lu)",__FUNC__,GetCurrentThreadId());

		elMtxDelete(&td->lock);
		for(i = 0; i < EL_ARRAY_CNT(td->str_tmp); i++)
			EL_DELETE(&td->str_tmp[i]);
		elMemFree(&td);
	}

#if 0
Error:
#endif

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elStrAlloc"
EL_DLL_API elErr_t *elStrAlloc(elStr_t *str,size_t size)
{
	EL_ERR_VAR;

	if ( size > str->size )
	{
		size_t d, alloc_step;
		
		alloc_step = str->alloc_step ? str->alloc_step : EL_STR_ALLOC_STEP;

		d = size % alloc_step;
		if ( d )
			size += alloc_step - d;
		
		if ( str->alloc )
			EL_ERR_CHECK( elMemRealloc(&str->ptr,size) );
		else
			EL_ERR_CHECK( elMemAlloc(&str->ptr,size) );

		str->size	= size;
		str->alloc	= TRUE;
	}

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elStrCtor"
static elErr_t *elStrCtor(elObjPtr_t *po,va_list args)
{
	EL_ERR_VAR;
	elStr_t *str = (elStr_t *)*po;

	if ( args )
	{
		TCHAR *ptr;
		
		ptr = va_arg(args,TCHAR *);
		EL_ERR_CHECK( elStrAlloc(str,_tcslen(ptr)+EL_ST) );
		strcpy(str->ptr,ptr);
	}
	else
	{
#if 1
		str->ptr = NULL;
#else
		str->ptr = "";
#endif
	}

	str->Ptr		= elStrPtr;
	str->Len		= elStrLen;
	str->Empty		= elStrEmpty;
	str->Cpy		= elStrCpy;
	str->NCpy		= elStrNCpy;
	str->Cat		= elStrCat;
	str->CatFmtV	= elStrCatFmtV;
	str->CatFmt		= elStrCatFmt;
	str->FmtV		= elStrFmtV;
	str->Fmt		= elStrFmt;
	str->FmtAtV		= elStrFmtAtV;
	str->FmtAt		= elStrFmtAt;
	str->PrintfAt	= elStrPrintfAt;
	str->Printf		= elStrPrintf;

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elStrDtor"
static elErr_t *elStrDtor(elObjPtr_t *po)
{
	EL_ERR_VAR;
	elStr_t *str = (elStr_t *)*po;

	if ( str->alloc )
		elMemFree(&str->ptr);
	str->size = 0;
	str->alloc = FALSE;

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elStrPtr"
EL_DLL_API TCHAR *elStrPtr(elStr_t *str)
{
	return str->ptr;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elStrLen"
EL_DLL_API int elStrLen(elStr_t *str)
{
	return str->ptr ? (int) _tcslen(str->ptr) : 0;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elStrEmpty"
EL_DLL_API elErr_t *elStrEmpty(elStr_t *str)
{
	EL_ERR_VAR;

	elStrAlloc(str,EL_ST);
	*str->ptr = _T('\0');

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elStrCpy"
EL_DLL_API elErr_t *elStrCpy(elStr_t *str,TCHAR *s)
{
	EL_ERR_VAR;

	if ( s )
	{
		EL_ERR_CHECK( elStrAlloc(str,_tcslen(s)+EL_ST) );
		_tcscpy(str->ptr,s);
	}
	
Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elStrNCpy"
EL_DLL_API elErr_t *elStrNCpy(elStr_t *str,TCHAR *s,size_t len)
{
	EL_ERR_VAR;

	EL_ERR_CHECK( elStrAlloc(str,len+EL_ST) );
	_tcsncpy(str->ptr,s,len);

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elStrCat"
EL_DLL_API elErr_t *elStrCat(elStr_t *str,TCHAR *s)
{
	EL_ERR_VAR;
	size_t len0, len, size;

	len0 = str->ptr ? _tcslen(str->ptr) : 0;
	len = len0 + _tcslen(s);

	/* number of bytes required by formatted string: */
	size = (len + EL_ST) * sizeof(TCHAR);

	EL_ERR_CHECK( elStrAlloc(str,size) );

	_tcscpy(str->ptr + len0,s);

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elStrCatFmtV"
EL_DLL_API elErr_t *elStrCatFmtV(elStr_t *str,const TCHAR *fmt,va_list args)
{
	return elStrFmtAtV(str,_tcslen(str->ptr),fmt,args);
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elStrCatFmt"
EL_DLL_API elErr_t *elStrCatFmt(elStr_t *str,const TCHAR *fmt,...)
{
	EL_ERR_VAR;
	va_list args;

	va_start(args,fmt);
	err = elStrCatFmtV(str,fmt,args);
	va_end(args);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elStrFmtV"
EL_DLL_API elErr_t *elStrFmtV(elStr_t *str,const TCHAR *fmt,va_list args)
{
	EL_ERR_VAR;
	size_t size;
	int n;

	/* copy/create formatted string: */
	n = _vscprintf(fmt,args);
	if ( n >= 0 )
	{
		elStr_t	*tmp;

		/* number of bytes required by formatted string: */
		size = (n + EL_ST) * sizeof(TCHAR);

#if 1
		/*
		 * Allow 'fmt' / 'args' to point to 'str->ptr' itself, e.g."
		 *
		 *		elStrFmt(path,"%s/%s",name,path->ptr);
		 */
		EL_ERR_CHECK( elNew(elStr_c,&tmp) );
		EL_ERR_CHECK( elStrAlloc(tmp,size) );
		_vstprintf(tmp->ptr,fmt,args);
		elStrCpy(str,tmp->ptr);
		elDelete(&tmp);
#else
		EL_ERR_CHECK( elStrAlloc(str,size) );
		_vstprintf(str->ptr,fmt,args);
#endif
	}

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elStrFmt"
EL_DLL_API elErr_t *elStrFmt(elStr_t *str,const TCHAR *fmt,...)
{
	EL_ERR_VAR;
	va_list args;

	va_start(args,fmt);
	err = elStrFmtV(str,fmt,args);
	va_end(args);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elStrFmtAtV"
EL_DLL_API elErr_t *elStrFmtAtV(
	elStr_t *str,size_t idx,const TCHAR *fmt,va_list args
)
{
	EL_ERR_VAR;
	size_t size;
	int n;

	/* Concatenate/Append formatted string: */

	n = _vscprintf(fmt,args);
	if ( n >= 0 )
	{
		/*
		 * Number of bytes required by resulting string:
		 */
		size = (idx + n + EL_ST) * sizeof(TCHAR);

		EL_ERR_CHECK( elStrAlloc(str,size) );

		_vstprintf(str->ptr + idx,fmt,args);
	}

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elStrFmtAt"
EL_DLL_API elErr_t *elStrFmtAt(elStr_t *str,size_t idx,const TCHAR *fmt,...)
{
	EL_ERR_VAR;
	va_list args;

	va_start(args,fmt);
	err = elStrFmtAtV(str,idx,fmt,args);
	va_end(args);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elStrPrintfAt"
EL_DLL_API elErr_t *elStrPrintfAt(elStr_t *str,size_t idx,const TCHAR *fmt,...)
{
	EL_ERR_VAR;
	va_list args;

	va_start(args,fmt);
	err = elStrFmtAtV(str,idx,fmt,args);
	va_end(args);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elStrPrintf"
EL_DLL_API elErr_t *elStrPrintf(elStr_t *str,const TCHAR *fmt,...)
{
	EL_ERR_VAR;
	va_list args;

	va_start(args,fmt);
	err = elStrFmtV(str,fmt,args);
	va_end(args);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elStrTmpIdxGet"
EL_DLL_API elStr_t *elStrTmpIdxGet(register int idx)
{
	ThreadData_t *td;

	/* get pointer to current thread's data: */
	if ( thread_data_get(&td) )
		return NULL;

#if 0
	elLogDbgStr("%s(TID=%lu): td: 0x%lX",
		__FUNC__,GetCurrentThreadId(),td);
#endif

	if ( idx == EL_STR_IDX_TMP )
	{
		elMtxLock(&td->lock);
		idx = td->tmp_idx + 1;
		if ( idx == EL_STR_IDX_TMP_cnt )
			idx = 0;
		td->tmp_idx = idx;
		elMtxUnlock(&td->lock);
		idx += EL_STR_IDX_cnt;
	}
	else
	{
		idx += EL_STR_IDX_cnt;
		if ( (uint)idx >= EL_STR_IDX_cnt )
			return NULL;
	}

	return &td->str_tmp[idx];
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elStrTmpIdxFmtV"
EL_DLL_API TCHAR *elStrTmpIdxFmtV(int idx,const TCHAR *fmt,va_list args)
{
	elStr_t *str;

	str = elStrTmpIdxGet(idx);
	if ( !str )
		return NULL;

	if ( elStrFmtV(str,fmt,args) )
		return NULL;

	return str->ptr;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elStrTmpIdxFmt"
EL_DLL_API TCHAR *elStrTmpIdxFmt(int idx,const TCHAR *fmt,...)
{
	TCHAR *str;
	va_list args;

	va_start(args,fmt);
	str = elStrTmpIdxFmtV(idx,fmt,args);
	va_end(args);

	return str;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elStrTmpFmtV"
EL_DLL_API TCHAR *elStrTmpFmtV(const TCHAR *fmt,va_list args)
{
	return elStrTmpIdxFmtV(EL_STR_IDX_TMP,fmt,args);
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elStrTmpFmt"
EL_DLL_API TCHAR *elStrTmpFmt(const TCHAR *fmt,...)
{
	TCHAR *str;
	va_list args;

	va_start(args,fmt);
	str = elStrTmpFmtV(fmt,args);
	va_end(args);

	return str;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elStrUnicode2Ansi"
EL_DLL_API char *elStrUnicode2Ansi(wchar_t *ustr,char *astr,size_t astr_size)
{
	(void) astr;
	(void) ustr;
	(void) astr_size;

	;

	return NULL;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elStrAnsi2Unicode"
EL_DLL_API wchar_t *elStrAnsi2Unicode(char *astr,wchar_t *ustr,size_t ustr_size)
{
	register size_t len = strlen(astr);

	while( (int)ustr_size >= sizeof(*ustr) && len >= 0 )
	{
		ustr[len] = astr[len];
		len--;
		ustr_size -= sizeof(*ustr);
	}

	return ustr;
}

/******************************************************************************/
