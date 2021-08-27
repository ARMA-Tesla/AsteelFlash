/*
 *  <elLog.h> - Definitions for module "elLog.c".
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

#if !defined(_EL_LOG_H)
#define _EL_LOG_H

/* MS Visual Studio 2005: #ident ignored; Borland C/C++: Unknown preprocessor directive: 'ident' */
#if _MSC_VER < 1400 && !__BCOPT__ && !_CVI_
#if 1
#ident "@(#) elLog.h 0.1 2007-03-05"
#else
#endif
#endif

/* forward reference: */
struct SelLog;
typedef struct SelLog elLog_t, *elPLog_t, *elLogPtr_t;

#include <elBase.h>
#include <elObj.h>
#include <elMtx.h>

enum {
	/*
	 * 'EL_LOG_DISPLAY':
	 *
	 * Log output goes to display (not to permanent storage, e.g. file on disk).
	 * For example, output from 'elLogD...() routines goes only to log devices
	 * having this attribute set.
	 */
	EL_LOG_DISPLAY = EL_FLAG(0),

	/*
	 * 'EL_LOG_FILE':
	 *
	 * Log output goes to file (not to display).
	 * For example, output from 'elLogF...() routines goes only to log devices
	 * having this attribute set.
	 */
	EL_LOG_FILE = EL_FLAG(1),

	/*
	 * 'EL_LOG_AUTO_SUFFIX_CR':
	 *
	 * Add '\r' automatically at the end of every line of output.
	 */
	EL_LOG_AUTO_SUFFIX_CR = EL_FLAG(2),

	/*
	 * 'EL_LOG_AUTO_PREFIX_CR':
	 *
	 * Add '\r' automatically at the beginning of every line of output.
	 */
	EL_LOG_AUTO_PREFIX_CR = EL_FLAG(3),

	/*
	 * 'EL_LOG_AUTO_SUFFIX_LF':
	 *
	 * Add '\n' automatically at the end of every line of output.
	 */
	EL_LOG_AUTO_SUFFIX_LF = EL_FLAG(4),

	/*
	 * 'EL_LOG_AUTO_LOCK':
	 *
	 * Lock log output automatically so that prefix, message and suffix
	 * are output atomically instead of being mixed when logging
	 * from multiple threads.
	 */
	EL_LOG_AUTO_LOCK = EL_FLAG(5),

};

typedef struct {
	FILE	*fp;
} elLogDevStdio_t;

struct SelLogDev;
typedef struct SelLogDev elLogDev_t;
typedef elErr_t *(*elLogFn_t)(elLog_t *log,elLogDev_t *dev,TCHAR *str);
typedef struct SelLogDev {
	elUInt32_t	attrs;
	elLogFn_t	fn_write;
	void		*data;

	elMtx_t		lock;
} elLogDev_t;

/* forward reference: */
struct SelObj;
typedef struct SelObj elObj_t, *elPObj_t, *elObjPtr_t;

/*
 * 'elLog_t':  Log data type.
 */
struct SelLog {
	elObj_t		obj;

	elLogDev_t	**dev;		/* output device(s) */
	int			dev_cnt;

	/* methods: */
	elErr_t	*(*PrintfV)(elLog_t *log,const TCHAR *fmt,va_list args);
	elErr_t	*(*Printf) (elLog_t *log,const TCHAR *fmt,...);

};

#if !defined(EL_LOG_MODULE)
EL_DLL_IMPORT extern const elObjClass_t *elLog_c;
#endif

#ifdef __cplusplus
extern "C" {			/* C Plus Plus function bindings */
#endif

EL_DLL_API elErr_t *elLogStartup(void);
EL_DLL_API elErr_t *elLogCleanup(void);
EL_DLL_API elErr_t *elLogSet(elLog_t *log);
EL_DLL_API elErr_t *elLogGet(elLog_t **p_log);
EL_DLL_API elErr_t *elLogStderrWrite(elLog_t *log,elLogDev_t *dev,TCHAR *str);
EL_DLL_API elErr_t *elLogStdioWrite(elLog_t *log,elLogDev_t *dev,TCHAR *str);
EL_DLL_API elErr_t *elLogPrintfV(elLog_t *log,const TCHAR *fmt,va_list args);
EL_DLL_API elErr_t *elLogPrintf(elLog_t *log,const TCHAR *fmt,...);
EL_DLL_API elErr_t *elLogDevAttrSet(elLogDev_t *dev,elUInt32_t attrs);
EL_DLL_API elErr_t *elLogDevAttrClr(elLogDev_t *dev,elUInt32_t attrs);
EL_DLL_API elErr_t *elLogV(const TCHAR *fmt,va_list args);
EL_DLL_API elErr_t *elLog(const TCHAR *fmt,...);
EL_DLL_API elErr_t *elLogD(const TCHAR *fmt,...);
EL_DLL_API elErr_t *elLogF(const TCHAR *fmt,...);
EL_DLL_API elErr_t *elLogHexDump(
	void *buf,int nbytes,elErr_t *(*disp_fn)(char *s),char *opts
);
EL_DLL_API elErr_t *elLogHexDumpDisplay(char *str);
EL_DLL_API elErr_t *elLogDHexDumpDisplay(char *str);
EL_DLL_API elErr_t *elLogFlush(elLog_t *log);
EL_DLL_API elErr_t *elLogOpen(
	elLog_t *log,TCHAR *fpath,elLogFn_t fn_write,TCHAR *prefix,bool autoflush
);
EL_DLL_API void elLogDbgStr(const TCHAR *fmt,...);

#ifdef __cplusplus
}						/* C Plus Plus function bindings */
#endif

#endif 	/* _EL_LOG_H */
