/*
 *  <elErr.h> - Definitions for module "elErr.c".
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

#if !defined(_EL_ERR_H)
#define _EL_ERR_H

#if _MSC_VER < 1400 && !__BCOPT__ && !_CVI_  /* Borland C/C++: Unknown preprocessor directive: 'ident' */
#if 1
#ident "@(#) elErr.h 0.1 2007-03-05"
#else
#endif
#endif

/* forward reference: */
struct SelErr;
typedef struct SelErr elErr_t, *elPErr_t, *elErrPtr_t;

#include <elBase.h>
#include <elStr.h>

/*
 * EL_ERR_STR_SAVE_SPACE = 1: not implemented:
 *
 * ToDo: to implement this completely, there would
 * need to be also macros for passing the ANSI vs. UNICODE
 * string parameters to 'elErrSet()' from user modules
 * (not just in 'EL_ERR_CHECK...()' macros).
 *
 * Save space by using ANSI strings for:
 *
 *	- source file ('__FILE__')
 *	- reporting function name ('__FUNC__')
 *	- failed function name (e.g. 'elIoWrite(io,buf,budsize,&len)')
 */
#define	EL_ERR_STR_SAVE_SPACE	0
#if EL_ERR_STR_SAVE_SPACE
#define EL_ERR_ACHAR		char
#define _A(str)				str
#define EL_ERR_STRCPY_A		elErrStrCpyA
#define EL_ERR_STR_A_t		elErrStrA_t
#else
#define EL_ERR_ACHAR		TCHAR
#define _A(str)				_T(str)
#define EL_ERR_STRCPY_A		elErrStrCpy
#define EL_ERR_STR_A_t		elErrStr_t
#endif


#define EL_ERR_VAR			elErr_t *err = NULL
#define EL_ERRID_VAR		elErrId_t err = EL_OK

#define EL_ERR_SET_EX(errid1,errid2,errid3,source,errstr)					\
		elErrSet(															\
			&err,(elErrId_t)errid1,(elErrId_t)errid2,(elErrId_t)errid3,		\
			_A(__FILE__),__LINE__,_A(__FUNC__),								\
			source,errstr													\
		)																	
#define EL_ERR_SET(errid,source,errstr)										\
		EL_ERR_SET_EX(errid,-1,-1,source,errstr)
																			
#if 1
#define EL_ERR_THROW_EX(errid1,errid2,errid3,source,errstr)					\
		if ( EL_CONST_COND_1 )												\
		{																	\
			EL_ERR_SET_EX(errid1,errid2,errid3,source,errstr);				\
			goto Error;														\
		}																	\
		else
#else
#define EL_ERR_THROW_EX(errid1,errid2,errid3,source,errstr)					\
		do																	\
		{																	\
			EL_ERR_SET_EX(errid1,errid2,errid3,source,errstr);				\
			goto Error;														\
		} while( EL_CONST_COND_0 )
#endif
#define EL_ERR_THROW(errid,source,errstr)									\
		EL_ERR_THROW_EX(errid,-1,-1,source,errstr)

#define EL_ERR_THROW_OK()	EL_ERR_THROW(EL_OK,NULL,NULL)
#define EL_ERR_OK()			EL_ERR_THROW_OK()

#define EL_ERRID_THROW(errid)												\
		if ( EL_CONST_COND_1 )												\
		{																	\
			err = errid;													\
			goto Error;														\
		}																	\
		else																
																			
#define EL_ERR_THROW_WIN(fn_win,errid,errstr)								\
		EL_ERR_THROW_EX(errid,EL_ERR_WIN,GetLastError(),fn_win,errstr)

#define EL_ERR_CHECK_ERR(fn_el,errid,errstr)								\
		if ( (err = fn_el) != NULL )										\
			EL_ERR_THROW(errid,#fn_el,errstr);								\
		else

#define EL_ERR_CHECK(fn_el)													\
		if ( (err = fn_el) != NULL )										\
			EL_ERR_THROW(-1,#fn_el,NULL);									\
		else																
																			
#define EL_ERRID_CHECK(fn_el,errid)											\
		if ( (err = fn_el) != EL_OK )										\
			EL_ERRID_THROW(errid == -1 ? err : errid);						\
		else																
																			
#define EL_ERR_RETURN(fn_el)												\
		if ( (err = fn_el) != NULL )										\
			goto Error;														\
		else																
																			
#define EL_ERR_CHECK_COND(cond,errid,errstr)								\
		if ( (cond) != 0 )													\
			EL_ERR_THROW(errid,#cond,errstr);								\
		else																
																			
#define EL_ERR_ASSERT(expression)											\
		EL_ERR_CHECK_COND(													\
			!(expression), EL_ERR_INVAL,"!( " #expression " )"				\
		)
																			
#define EL_ERRID_CHECK_COND(cond,errid)										\
		if ( cond )															\
			EL_ERRID_THROW(errid);											\
		else																
																			
#define EL_ERR_CHECK_ALLOC(ptr,size)										\
		if ( !(ptr) )														\
		{																	\
			EL_ERR_THROW(													\
				EL_ERR_MEM,													\
				NULL,														\
				size == -1													\
					? _T("cannot allocate memory")							\
					: elStrTmpFmt(_T("cannot allocate memory (%ld bytes)"),	\
						size)												\
			);																\
		}																	\
		else																
																			
#define EL_ERRID_CHECK_ALLOC(ptr)											\
		if ( (ptr) == NULL )												\
			EL_ERRID_THROW(EL_ERR_MEM);										\
		else																
																			
#if 0
#define	EL_ERR_CHECK_MEM(fn_elMem)											\
		EL_ERR_CHECK(fn_elMem)
#endif

#define EL_ERR_CHECK_WIN(fn_win,errid,errstr)								\
		if ( (fn_win) == 0 )												\
		{																	\
			elErrSet(														\
				&err,errid,EL_ERR_WIN,GetLastError(),						\
				_A(__FILE__),__LINE__,_A(__FUNC__),							\
				#fn_win,errstr												\
			);																\
			goto Error;														\
		}

#define	EL_ERR_LOG()				\
		err ? err->Log(err,NULL) : 0

enum {
	EL_OK = 0,				/*!< success */
	EL_ERR,					/*!< generic error */
	EL_ERR_TLS,				/*!< TLS related error */
	EL_ERR_TIMER,			/*!< timer related error */
	EL_ERR_SOCKET,			/*!< TCP/IP socket related error */
	EL_ERR_IN_PROGRESS,		/*!< error in progress */
	EL_ERR_MEM,				/*!< memory allocation error */
	EL_ERR_INVAL,			/*!< invalid argument/parameter */
	EL_ERR_INTERNAL,		/*!< internal error */
	EL_ERR_IO,				/*!< I/O error */
	EL_ERR_TIMEOUT,			/*!< operation (e.g. I/O) timed out */
	EL_ERR_ABORT,			/*!< operation (e.g. I/O) aborted */
	EL_ERR_NO_DATA,			/*!< no data is available (similar to 'EAGAIN' or 'EWOULDBLOCK') */
	EL_ERR_WIN,				/*!< WIN32 API errors */
	EL_ERR_MQX,				/*!< MQX API errors */
	EL_ERR_CVI,				/*!< CVI API errors */
	EL_ERR_XML,				/*!< XML API errors */
};

typedef elUInt32_t elErrId_t;

/*
 * 'elErrStr_t':  ANSI/UNICODE string data type used for error description.
 * 'elErrStrA_t': ANSI-only    string data type used for error description.
 *
 *	alloc	size	ptr
 *	---------------------------------------------------------------------
 *	FALSE	   0	pointer to constant string literal (e.g. '__FUNC__')
 *					(usually located in code segment)
 *	FALSE	 > 0	pointer to array of characters of constant size
 *					(located in data segment)
 *	TRUE	 > 0	pointer to dynamically allocated space of size 'size'
 */
#define EL_ERR_STR_t(type,str_type)	\
	typedef struct {				\
		str_type	ptr;			\
		bool		alloc;			\
		size_t		size;			\
	} type;

EL_ERR_STR_t(elErrStr_t,TCHAR *);
EL_ERR_STR_t(elErrStrA_t,char *);

/*
 * 'elErrFn_t': Error description for single function.
 */
typedef struct SelErrFn {
	/*
	 * 'id': error IDs ('EL_ERR_...').
	 */
	struct {
		/*
		 * Generic error ID (e.g. 'EL_ERR_IO').
		 */
		elErrId_t		gen;

		/*
		 * Subsystem to which the specific error ID 'id[2]' belongs
		 *  ('EL_ERR_{WIN|MQX|CVI|COM|MSXML|SQL|...}'):
		 */
		elErrId_t		subsys;

		/*
		 * Subsystem specific error ID (e.g. 'ERROR_FILE_NOT_FOUND'):
		 */
		elErrId_t		subsys_spec;
	} id;
	elErrStr_t		str;		/* error message */
	EL_ERR_STR_A_t	caller;		/* reporting function ('__FUNC__') */
	EL_ERR_STR_A_t	callee;		/* failed function (may include parameters) */
	EL_ERR_ACHAR	*file;		/* source file ('__FILE__') */
	int				line;		/* source line ('__LINE__') */
	void			*data;		/* error-specific data */
} elErrFn_t, *elPErrFn_t, *elErrFnPtr_t;

/* forward reference: */
struct SelErr;
typedef struct SelErr elErr_t, *elPErr_t, *elErrPtr_t;
struct SelStr;
typedef struct SelStr elStr_t, *elPStr_t, *elStrPtr_t;

/* 'elErr_t': */
struct SelErr {
	bool			f_static;

	/* error path build up from 1 or more failed functions: */
	struct {
		elErrFn_t		level0;	/* pointed to by 'level[0]' */
		elErrFn_t		*level;	/* 'level[0..cnt-1]' */
		int				cnt;	/* # of valid     elements in 'level[]' */
		int				acnt;	/* # of allocated elements in 'level[]' */
	} fn;

	/* methods: */
	elErr_t			*(*Log)(elErr_t *err,const TCHAR *fmt,...);

	/*
	 * Copies of / Pointers to error data of the last function error reported
	 * (fn.level[fn.acnt-1]'):
	 */
	elErrId_t		id;				/* 'id.gen ' */
	TCHAR			*str;
	EL_ERR_ACHAR	*func;
	EL_ERR_ACHAR	*fn_caller;		/* == 'func' */
	EL_ERR_ACHAR	*fn_callee;
	EL_ERR_ACHAR	*file;
	int				line;

	/*
	 * 'string': Formatted error string.
	 *
	 * It is defined as structure instead of pointer, so that it is
	 * more resistant to memory allocation problems.
	 */
	elStr_t			string;
};

#ifdef __cplusplus
extern "C" {			/* C Plus Plus function bindings */
#endif

EL_DLL_API elErr_t *elErrStartup(void);
EL_DLL_API elErr_t *elErrCleanup(void);
EL_DLL_API TCHAR	*elErrStrFmt(const TCHAR *fmt,...);
EL_DLL_API TCHAR	*elErrStr(void);
EL_DLL_API elErr_t	*elErrLog(elErr_t *err,const TCHAR *fmt,...);
EL_DLL_API elErr_t	*elErrSet(
	elPErr_t *p_err,
	elErrId_t errid_gen,elErrId_t errid_subsys,elErrId_t errid_subsys_spec,
	EL_ERR_ACHAR *file,int line,EL_ERR_ACHAR *fn_caller,EL_ERR_ACHAR *fn_callee,
	TCHAR *errfmt,...
);

#ifdef __cplusplus
}						/* C Plus Plus function bindings */
#endif

#endif 	/* _EL_ERR_H */
