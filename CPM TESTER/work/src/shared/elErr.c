/*
 *  "elErr.c" - Error handling routines.
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
static char _ident[] = "@(#) elErr.c 0.1 2007-03-05";
#else
#endif

#include <elBase.h>
#include <elTls.h>
#include <elErr.h>
#include <elLog.h>
#include <elMem.h>
#include <elMtx.h>

/* max. number of threads utilizing static string space: */
#define THREAD_CNT_MAX		10

#define FN_EL_ERR_STRCPY(fn_name,errstr_type,str_type,fn_strlen,fn_strncpy)	\
		str_type fn_name(													\
			errstr_type str,str_type s,int ts_offset,size_t ts_size			\
		)																	\
		{																	\
			if ( !s )														\
			{																\
				if ( str->alloc )											\
				{															\
					free(str->ptr);											\
					str->size = 0;											\
				}															\
				return str->ptr = NULL;										\
			}																\
			else															\
			{																\
				size_t size = fn_strlen(s) + EL_ST;							\
																			\
				if ( elErrStrAlloc(											\
					(elErrStr_t *)str,size,ts_offset,ts_size) == EL_OK )	\
				{															\
					fn_strncpy(str->ptr,s,str->size-EL_ST);					\
					str->ptr[str->size-EL_ST] = _T('\0');					\
				}															\
				else														\
				{															\
					/* error, set at least the pointer: */					\
					str->ptr = s;											\
				}															\
				return str->ptr;											\
			}																\
		}

/*
 * 'ThreadData_t': Per thread data.
 *
 * Every thread has its own single copy of 'elErr_t' data structure.
 * The content of the structure (strings and associated data) may
 * grow as error(s) propagate(s) from the bottom most function
 * to the topmost one (error information may be added at each
 * level of the function call path).
 */
typedef struct {
	bool	err_in_progress;
	elErr_t	err;
} ThreadData_t, *PThreadData_t, *ThreadDataPtr_t;

static elErr_t gs_err_tls = {
	TRUE,
	{ { {EL_ERR_TLS},	{_T("Thread Local Storage (TLS) initialization failed ('elStartup()' not called ?)")} }, NULL, 1, 0 },
	elErrLog
};

static elErr_t gs_err_mem = {
	TRUE,
	{ { {EL_ERR_MEM},	{_T("Memory allocation failed")} }, NULL, 1, 0 },
	elErrLog
};

static elErr_t gs_err_in_progress = {
	TRUE,
	{ { {EL_ERR_IN_PROGRESS},	{_T("Error in progress")} }, NULL, 1, 0 },
	elErrLog
};

/*
 * 'gs': Global Static data.
 */
static struct {
	elMtx_t mtx;

	struct  {
		DWORD id;	/* thread ID */

		/*
		 * Set of static strings for error message for each thread,
		 * in case memory cannot be allocated or TLS cannot be initialized:
		 */
		struct Sthread_str {
			/* error message: */
			TCHAR			err[256+EL_ST];
			/* reporting function ('__FUNC__'): */
			EL_ERR_ACHAR	fn_caller[64+EL_ST];
			/* failed function (may include parameters): */
			EL_ERR_ACHAR	fn_callee[128+EL_ST];
		} str;
	} thread[THREAD_CNT_MAX];
	int thread_cnt;
} gs;
typedef struct Sthread_str thread_str_t;

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elErr::thread_data_get"
static elErrId_t thread_data_get(PThreadData_t *p_td)
{
	EL_ERRID_VAR;
	PThreadData_t td;

	EL_ERRID_CHECK_COND( elTlsValGet(EL_TLS_IDX_ERR,&td), EL_ERR_TLS );
	if ( !td )
	{
		EL_ERRID_CHECK_ALLOC( td = malloc(sizeof(*td)) );
			
		EL_ERRID_CHECK_COND( elTlsValSet(EL_TLS_IDX_ERR,td), EL_ERR_TLS );

		memset(td,0,sizeof(*td));
		/*
		 * 1st element of 'td->err.fn.level[]' is already allocated as part
		 * of the 'td', so that we have space available for at least the 1st
		 * function's error, which can be memory allocation problem related...
		 */
		td->err.fn.level = &td->err.fn.level0;
		td->err.fn.cnt  = 0;
		td->err.fn.acnt = 1;
		td->err.Log = elErrLog;
	}

	*p_td = td;

Error:

	return err;
}

/******************************************************************************/
/*
 * 'thread_str_get()':
 *
 * Obtains pointer to *statically* allocated space of strings for the current
 * thread. This routine and string space is not dependant neither upon dynamic
 * memory allocation nor TLS (thread local storage). This string space is
 * used in case memory cannot be allocated or TLS cannot be initialized.
 */
static thread_str_t *thread_str_get(void)
{
	DWORD			tid;
	register int	i;

	elMtxLock(&gs.mtx);

	tid = GetCurrentThreadId();
	for(i = 0; i < gs.thread_cnt; i++)
		if ( gs.thread[i].id == tid )
			break;
	if ( i == gs.thread_cnt )
		return NULL;	/* thread table full ('THREAD_CNT_MAX') */

	gs.thread[gs.thread_cnt++].id = tid;

	elMtxUnlock(&gs.mtx);

	return &gs.thread[i].str;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elErrStartup"
EL_DLL_API elErr_t *elErrStartup(void)
{
	EL_ERR_VAR;

	EL_ERR_CHECK( elMtxNew(&gs.mtx,NULL) );

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elErrCleanup"
EL_DLL_API elErr_t *elErrCleanup(void)
{
	EL_ERR_VAR;
	ThreadData_t	*td;

	if ( thread_data_get(&td) == EL_OK )
	{
#if 0
		elMemFree(&td->err.fn.level);
#endif
		elMemFree(&td);
	}

	elMtxDelete(&gs.mtx);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elErrStrAlloc"
static elErrId_t elErrStrAlloc(
	elErrStr_t		*str,
	size_t			size,
	int				ts_offset,
	size_t			ts_size
)
{
	if ( size > str->size )
	{
		void *ptr;

		if ( str->alloc )
			ptr = realloc(str->ptr,size);
		else
			ptr = malloc(size);
		if ( ptr )
		{
			str->ptr	= ptr;
			str->size	= size;
			str->alloc	= TRUE;
		}
		else
		{	/* alloc failed: */
			if ( str->alloc )
				free(str->ptr);
			str->ptr = NULL;
			if ( ts_size )
			{
				thread_str_t *ts;

				ts = thread_str_get();
				if ( !ts )
					return EL_ERR;
				str->ptr = (char *)ts + ts_offset;
			}
			else
			{
				/* no space needed ('str->ptr' = NULL): */
			}
			str->size  = ts_size;
			str->alloc = FALSE;
		}
	}
	return EL_OK;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elErrStrFree"
static elErrId_t elErrStrFree(elErrStr_t *str)
{
	free(str->ptr);
	str->ptr	= NULL;
	str->alloc	= FALSE;
	str->size	= 0;

	return EL_OK;
}

/******************************************************************************/
FN_EL_ERR_STRCPY(elErrStrCpy ,elErrStr_t  *,TCHAR *,_tcslen,_tcsncpy)
FN_EL_ERR_STRCPY(elErrStrCpyA,elErrStrA_t *, char *, strlen, strncpy)

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elErrSet"
EL_DLL_API elErr_t *elErrSet(
	elPErr_t		*p_err,
	elErrId_t		errid_gen,			/*!< generic error code */
	elErrId_t		errid_subsys,		/*!< subsystem error code */
	elErrId_t		errid_subsys_spec,	/*!< subsystem specific error code */
	EL_ERR_ACHAR	*file,				/*!< source file */
	int				line,
	EL_ERR_ACHAR	*fn_caller,			/*!< reporting function */
	EL_ERR_ACHAR	*fn_callee,			/*!< failed function */
	TCHAR			*errfmt,
	...
)
{
	va_list			args;
	elErr_t			*err;
	elErrFn_t		*err_fn;
	elErrFn_t		*err_fn_prev;
	ThreadData_t	*td;
	size_t			size;

	if ( errid_gen == EL_OK )
		return NULL;

	/* leave global static error ('gs_err_...') intact (pass it on): */
	err = *p_err;
	if ( err && err->f_static )
		return err;

	if ( errid_gen == EL_ERR_TLS )
	{
		/*
		 * TLS init failed, no per thread memory for thread-safe error
		 * strings/messages, just global TLS error description for all threads:
		 */
		return *p_err = &gs_err_tls;
	}

	/* get pointer to error data for the current thread: */
	if ( thread_data_get(&td) != EL_OK )
	{
		/* ToDo: handle error: */
		;
		return *p_err = &gs_err_tls;
	}

	/*
	 * Error processing already in progress for the current thread ?
	 * (i.e. is this routine being called recursively ?)
	 */
	if ( td->err_in_progress )
		return *p_err = &gs_err_in_progress;

	td->err_in_progress = TRUE;
	err = &td->err;

	switch( errid_gen )
	{
#if 0
	case EL_ERR_MEM:
		/* ToDo: handle 'EL_ERR_MEM': */
		;
#if 0
		err = *p_err = &gs_err_mem;
		goto Error;
#endif
		break;
#endif

	default:
		if ( !*p_err )	/* no error yet ? */
		{
			/* set error pointer to thread's error structure: */
			*p_err = err;

			/* 1st failed function: */
			err->fn.cnt = 1;

			err_fn = &err->fn.level[0];
			err_fn_prev = NULL;
		}
		else
		{
			int cnt, acnt;

			/*
			 * Next failed function:
			 */

			cnt = err->fn.cnt + 1;
			acnt = err->fn.acnt;

			/* no more space ? */
			if ( cnt > acnt )
			{
				/* extend the array of failed functions: */

				/* remove the 1st "static" element: */
				if ( acnt == 1 )
				{
					td->err.fn.level = NULL;
					acnt = 0;
				}
				/* allocate in steps: */
				acnt += 5;

				size = acnt * sizeof(*td->err.fn.level);
#if 1
				td->err.fn.level = realloc(td->err.fn.level,size);
				if ( !td->err.fn.level )
				{
					err = *p_err = &gs_err_mem;
					goto Error;
				}
#else
				EL_ERR_CHECK( elMemRealloc(&td->err.fn.level,size) );
#endif

				memset(&td->err.fn.level[err->fn.acnt],0,
					(acnt - err->fn.acnt) * sizeof(*td->err.fn.level));

				/* copy the 1st "static" element: */
				if ( err->fn.acnt == 1 )
					memcpy(&td->err.fn.level[0],&td->err.fn.level0,
						sizeof(td->err.fn.level0));
			
				err->fn.acnt = acnt;
			}

			err_fn		= &err->fn.level[err->fn.cnt];
			err_fn_prev	= &err->fn.level[err->fn.cnt-1];
			err->fn.cnt++;
		}
		break;

	}	/* switch( errid_gen ) */

	if ( errid_gen != EL_ERR_TLS )
	{
		int n;

		if ( errid_gen == -1 )
		{
			if ( err_fn_prev )
				err_fn->id.gen = err_fn_prev->id.gen;
		}
		else
			err_fn->id.gen = errid_gen;

		if ( errid_subsys == -1 )
		{
			if ( err_fn_prev )
				err_fn->id.subsys = err_fn_prev->id.subsys;
		}
		else
			err_fn->id.subsys = errid_subsys;

		if ( errid_subsys_spec == -1 )
		{
			if ( err_fn_prev )
				err_fn->id.subsys_spec = err_fn_prev->id.subsys_spec;
		}
		else
			err_fn->id.subsys_spec = errid_subsys_spec;


		err_fn->file = file;
		err_fn->line = line;

		/* copy 'fn_caller': */
		EL_ERR_STRCPY_A(
			&err_fn->caller,fn_caller,
			EL_STCMBR_OFFSET(thread_str_t,fn_caller),
			EL_STCMBR_SIZE(  thread_str_t,fn_caller)
		);
		/* copy 'fn_callee': */
		EL_ERR_STRCPY_A(
			&err_fn->callee,fn_callee,
			EL_STCMBR_OFFSET(thread_str_t,fn_callee),
			EL_STCMBR_SIZE(  thread_str_t,fn_callee)
		);

		/* copy/create formatted error message: */
		va_start(args,errfmt);
		if ( errfmt )
		{
			n = _vscprintf(errfmt,args);
			if ( n >= 0 )
			{
				/* number of bytes required by formatted string: */
				size = (n + EL_ST) * sizeof(TCHAR);
				if ( elErrStrAlloc(
					&err_fn->str,size,
					EL_STCMBR_OFFSET(thread_str_t,err),
					EL_STCMBR_SIZE(  thread_str_t,err)
				) == EL_OK )
					_vstprintf(err_fn->str.ptr,errfmt,args);
			}
		}
		else
		{
#if 1
			elErrStrFree(&err_fn->str);		/* err_fn->str.ptr = NULL */
#else
			if ( elErrStrAlloc(
				&err_fn->str,EL_ST,
				EL_STCMBR_OFFSET(thread_str_t,err),
				EL_STCMBR_SIZE(  thread_str_t,err)
			) == EL_OK )
				*err_fn->str.ptr = _T('\0');
#endif
		}
		va_end(args);

	}	/* if ( errid_gen != EL_ERR_TLS ) */

	/* copies/pointers for quick access to last function error reproted: */
	err->id			= err_fn->id.gen;
	err->str		= err_fn->str.ptr;
	err->func		= err_fn->caller.ptr;
	err->fn_caller	= err_fn->caller.ptr;
	err->fn_callee	= err_fn->callee.ptr;
	err->file		= err_fn->file;
	err->line		= err_fn->line;

Error:

	td->err_in_progress = FALSE;

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elErrGet"
EL_DLL_API elErr_t *elErrGet(void)
{
	ThreadData_t *td;

	if ( thread_data_get(&td) )
	{
		/* ToDo: handle error: */
		return NULL;
	}

	return &td->err;
}

/******************************************************************************/
EL_DLL_API TCHAR *elErrStr(void)
{
	elErr_t *err = elErrGet();

#if 1
	if ( elStrFmt(&err->string,
		_T("%s(%d): %s(): %s(): %s"),
		err->file,err->line,err->fn_caller,err->fn_callee,
		err->str
	) )
		return _T(__FUNC__) _T(": ") _T("cannot create error string");
#else
	if ( err->string.Fmt(
		&err->string,
		_T("%s(%d): %s(): %s(): %s"),
		err->file,err->line,err->fn_caller,err->fn_callee,
		err->str ? err->str : "(null)"
	) )
		return _T(__FUNC__) _T(": ") _T("cannot create error string");
#endif

	return err->string.ptr;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elErrLog"
EL_DLL_API elErr_t *elErrLog(elErr_t *err,const TCHAR *fmt,...)
{
	va_list args;
	elErr_t *err2 = NULL;

	va_start(args,fmt);
	if ( fmt )
	{
		elLogV(fmt,args);
	}
	else
	{
		if ( err->fn.level[0].str.ptr )
			elLog(_T("%s"),err->fn.level[0].str.ptr);
		else
		{
			elLog(
				_T("%s(): %s"),
				err->fn.level[0].caller.ptr,
				err->fn.level[0].callee.ptr
			);
		}
	}
	va_end(args);

	return err2;
}

/******************************************************************************/
