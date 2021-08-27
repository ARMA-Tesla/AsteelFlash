/*
 *  "elLog.c" - Logging routines.
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
static char _ident[] = "@(#) elLog.c 0.1 2007-03-05";
#else
#endif

#define EL_LOG_MODULE

#include <elBase.h>
#include <elLog.h>
#include <elMem.h>

static struct {
	int		dummy;
} gs;

static elErr_t *elLogCtor(elObjPtr_t *po,va_list args);
static elErr_t *elLogDtor(elObjPtr_t *po);

static elObjClass_t gs_elLog_c = {
	sizeof(elLog_t),
	"elLog",
	elLogCtor,		/* Ctor()	*/
	elLogDtor,		/* Dtor()	*/
	NULL,			/* Clone()	*/
	NULL,			/* Differ()	*/
};

/*
 * 'elLog_c' ('<type>_c') instead of just 'elLog' ('<type>') (looks better)
 * because of 'elLog()' (and 'elLogD()') being used for general global log
 * output ('elLog(...)' looks better than 'elLogPrintf(NULL,...)').
 */
EL_DLL_EXPORT const elObjClass_t *elLog_c = &gs_elLog_c;

static elLogDev_t gs_dev_stderr = {
	EL_LOG_DISPLAY | EL_LOG_AUTO_SUFFIX_LF,
	elLogStderrWrite
};

static elLogDev_t gs_dev_stdio = {
	EL_LOG_FILE | EL_LOG_AUTO_SUFFIX_LF,
	elLogStdioWrite
};

/*
 * 'gs_log': Global log descriptor that can be changed using 'elLogSet()'.
 */
static elLog_t gs_log;

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elLogStartup"
elErr_t *elLogStartup(void)
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
#define	__FUNC__	"elLogCleanup"
elErr_t *elLogCleanup(void)
{
	EL_ERR_VAR;
	/*
	 * Cleanup is currently not implemented.
	 *
	 * Do NOT free/remove the global data used by all threads
	 * (only last thread can remove free the global data):
	 */
#if 0
	elLog_t			*log;
	register int	i;

	log = &gs_log;

	for(i = 0; i < log->dev_cnt; i++)
	{
		elLogDev_t	*dev = log->dev[i];

		elMemFree(&dev->data);

		elMtxDelete(&dev->lock);
	}
	elMemFree(&log->dev);
	log->dev_cnt = 0;
#endif

#if 0
Error:
#endif

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elLogCtor"
static elErr_t *elLogCtor(elObjPtr_t *po,va_list args)
{
	EL_ERR_VAR;
	elLog_t *log = (elLog_t *)*po;

	if ( args )
	{
		TCHAR *ptr;

		ptr = va_arg(args,TCHAR *);
		;
	}
	else
		;

	EL_ERR_CHECK( elMemAlloc(&log->dev,sizeof(*log->dev)) );
	log->dev[0]		= &gs_dev_stderr;

	log->PrintfV	= elLogPrintfV;
	log->Printf		= elLogPrintf;

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elLogDtor"
static elErr_t *elLogDtor(elObjPtr_t *po)
{
	EL_ERR_VAR;
	elLog_t *log = (elLog_t *)*po;

	elMemFree(&log->dev);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elLogSet"
elErr_t *elLogSet(elLog_t *log)
{
	EL_ERR_VAR;

	gs_log = *log;

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elLogGet"
elErr_t *elLogGet(elLog_t **p_log)
{
	EL_ERR_VAR;

	*p_log = &gs_log;

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elLogStderrWrite"
elErr_t *elLogStderrWrite(elLog_t *log,elLogDev_t *dev,TCHAR *str)
{
	EL_ERR_VAR;

	(void) log;
	(void) dev;

	if ( str )
		_fputts(str,stderr);
	else
		_fputts(_T("(null)"),stderr);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elLogStdioWrite"
elErr_t *elLogStdioWrite(elLog_t *log,elLogDev_t *dev,TCHAR *str)
{
	EL_ERR_VAR;
	elLogDevStdio_t	*stdio = (elLogDevStdio_t *)dev->data;

	(void) log;

	if ( str )
	{
		if ( stdio && stdio->fp )
		{
			_fputts(str,stdio->fp);
#if 1
			fflush(stdio->fp);
#endif
		}
		else
			OutputDebugString(str);
	}
	else
		_fputts(_T("(null)"),stdio->fp);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elLogOutput"
static elErr_t *elLogOutput(elLog_t *log,elUInt32_t attrs,TCHAR *str)
{
	EL_ERR_VAR;
	register int	i;

	if ( !str )
		str = "(null)";
		
	if ( !log )
	{
		log = &gs_log;
		if ( !log->dev_cnt )
		{
			EL_ERR_CHECK( elMemAlloc(&log->dev,2 * sizeof(*log->dev)) );
			log->dev[0] = &gs_dev_stderr;
			log->dev[1] = &gs_dev_stdio;
			log->dev_cnt = 2;
		}
	}

	for(i = 0; i < log->dev_cnt; i++)
	{
		elLogDev_t	*dev = log->dev[i];

		if ( (attrs & EL_LOG_DISPLAY) && !(dev->attrs & EL_LOG_DISPLAY) )
			return err;

		if ( dev->attrs & EL_LOG_AUTO_LOCK )
			EL_ERR_CHECK( elMtxLock(&dev->lock) );
			
		if ( (attrs | dev->attrs) & EL_LOG_AUTO_PREFIX_CR )
			dev->fn_write(log,dev,_T("\r"));

		dev->fn_write(log,dev,str);

		if ( (attrs | dev->attrs) & EL_LOG_AUTO_SUFFIX_LF )
			dev->fn_write(log,dev,_T("\n"));

		if ( dev->attrs & EL_LOG_AUTO_LOCK )
			EL_ERR_CHECK( elMtxUnlock(&dev->lock) );
	}

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elLogPrintfV"
elErr_t *elLogPrintfV(elLog_t *log,const TCHAR *fmt,va_list args)
{
	TCHAR *str;

	str = elStrTmpIdxFmtV(EL_STR_IDX_LOG,fmt,args);

	return elLogOutput(log,0,str);
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elLogDPrintfV"
elErr_t *elLogDPrintfV(elLog_t *log,const TCHAR *fmt,va_list args)
{
	TCHAR *str;

	str = elStrTmpIdxFmtV(EL_STR_IDX_LOG,fmt,args);

	return elLogOutput(log,EL_LOG_DISPLAY,str);
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elLogFPrintfV"
elErr_t *elLogFPrintfV(elLog_t *log,const TCHAR *fmt,va_list args)
{
	TCHAR *str;

	str = elStrTmpIdxFmtV(EL_STR_IDX_LOG,fmt,args);

	return elLogOutput(log,EL_LOG_FILE,str);
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elLogPrintf"
elErr_t *elLogPrintf(elLog_t *log,const TCHAR *fmt,...)
{
	EL_ERR_VAR;
	va_list args;

	va_start(args,fmt);
	log->PrintfV(log,fmt,args);
	va_end(args);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elLogV"
elErr_t *elLogV(const TCHAR *fmt,va_list args)
{
	EL_ERR_VAR;

	err = elLogPrintfV(NULL,fmt,args);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elLog"
elErr_t *elLog(const TCHAR *fmt,...)
{
	EL_ERR_VAR;
	va_list args;

	va_start(args,fmt);
	err = elLogPrintfV(NULL,fmt,args);
	va_end(args);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elLogD"
elErr_t *elLogD(const TCHAR *fmt,...)
{
	EL_ERR_VAR;
	va_list args;

	va_start(args,fmt);
	err = elLogDPrintfV(NULL,fmt,args);
	va_end(args);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elLogF"
elErr_t *elLogF(const TCHAR *fmt,...)
{
	EL_ERR_VAR;
	va_list args;

	va_start(args,fmt);
	err = elLogFPrintfV(NULL,fmt,args);
	va_end(args);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elLogDevAttrSet"
elErr_t *elLogDevAttrSet(elLogDev_t *dev,elUInt32_t attrs)
{
	EL_ERR_VAR;

	if ( !dev )
		dev = gs_log.dev[0];

	dev->attrs |= attrs;

	if ( attrs & EL_LOG_AUTO_LOCK )
		EL_ERR_CHECK( elMtxNew(&dev->lock,NULL) );

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elLogDevAttrClr"
elErr_t *elLogDevAttrClr(elLogDev_t *dev,elUInt32_t attrs)
{
	EL_ERR_VAR;

	if ( !dev )
		dev = gs_log.dev[0];

	dev->attrs &= ~attrs;

	if ( attrs & EL_LOG_AUTO_LOCK )
		EL_ERR_CHECK( elMtxDelete(&dev->lock) );

Error:

	return err;
}

/******************************************************************************/
/*
 * 'elLogHexDump()':
 *
 * Display binary data using given format and given function.
 */
#undef	__FUNC__
#define	__FUNC__	"elLogHexDump"
#if EL_OLD_PROTOTYPES
elErr_t *elLogHexDump(buf,nbyte,disp_fn,opts)
	void *buf;
	int nbyte;
	void (*disp_fn)();
	char *opts;
#else
elErr_t *elLogHexDump(
	register void *buf,register int nbytes,elErr_t *(*disp_fn)(char *s),char *opts
)
#endif
{
#ifdef	__FUNC__func__
		__FUNC__func__
#undef	__FUNC__
#define	__FUNC__ __func__
#endif
	EL_ERR_VAR;
	char dstr[60+EL_ST], cstr[16+EL_ST];
	int indent;
	register int i, off, c;
	register char *p;
	char *arg;

#if 0
	default hd(C) options: "-abx -A"
#endif
	indent = 0;
#if 1
	(void) opts;
	(void) p;
	(void) c;
	(void) arg;
#else
	for(p = opts; opts && (c = GetOpt("a:Ai:",p,&arg)) != 0; p = NULL)
	{
		switch(c)
		{
		case 'i': indent = atoi(arg); break;
		case -1:
			elLog("%s(): '%c': illegal option",__FUNC__,c);
			break;
		case -2:
			elLog("%s(): '%c': option requires an argument",__FUNC__,c);
			break;
		}
	}
#endif
	for(off=0; nbytes; )
	{
		char line[128+EL_ST];
		
		for(i = 0; i < MIN(nbytes,16); i++)
		{
			if ( i == 8 )
				dstr[i*3-1] = '-';
			sprintf(dstr+i*3,"%02X ",((byte *)buf)[off+i]);
			cstr[i] = (char)
				(isprint(((byte *)buf)[off+i]) ? ((byte *)buf)[off+i] : '.');
		}
		sprintf(line,"%*s%04X: %-50s %.*s",indent,"",off,dstr,i,cstr);
		(disp_fn ? disp_fn : elLogDHexDumpDisplay)(line);
		off += i;
		nbytes -= i;
	}

	return err;
}

/******************************************************************************/
/*
 * 'elLogHexDumpDisplay()':
 *
 * Output routine for 'elLogHexDump()'.
 */
elErr_t *elLogHexDumpDisplay(char *str)
{
	EL_ERR_VAR;

	/*
	 * 'Log("%s",s)' is used instead of 'Log(s)', because 'HexDump()' may
	 * produce string 'str' that contains printf-like format that would be
	 * interpreted in 'Log()'.
	 */
	elLog("%s",str);

	return err;
}

/******************************************************************************/
/*
 * 'elLogDHexDumpDisplay()':
 *
 * Default output routine for 'elLogHexDump()'.
 */
elErr_t *elLogDHexDumpDisplay(char *str)
{
	EL_ERR_VAR;

	/*
	 * 'LogD("%s",s)' is used instead of 'LogD(s)', because 'HexDump()' may
	 * produce string 'str' that contains printf-like format that would be
	 * interpreted in 'LogD()'.
	 */
	elLogD("%s",str);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elLogFlush"
elErr_t *elLogFlush(elLog_t *log)
{
	EL_ERR_VAR;
	elLogDev_t		*dev;
	register int	i;

	if ( !log )
		log = &gs_log;

	for(i = 0; i < log->dev_cnt; i++)
	{
		dev = log->dev[i];
		if ( dev == &gs_dev_stdio )
		{
			elLogDevStdio_t	*stdio = (elLogDevStdio_t *)dev->data;

			if ( stdio )
				fflush(stdio->fp);
		}
	}

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elLogOpen"
EL_DLL_API elErr_t *elLogOpen(
	elLog_t *log,TCHAR *fpath,elLogFn_t fn_write,TCHAR *prefix,bool autoflush
)
{
	EL_ERR_VAR;
	elLogDev_t *dev;

	(void) prefix;
	(void) autoflush;

	if ( !log )
	{
		elLogDevStdio_t *stdio;

		log = &gs_log;
		if ( !log->dev_cnt )
		{
			EL_ERR_CHECK( elMemAlloc(&log->dev,2 * sizeof(*log->dev)) );

			log->dev[0] = &gs_dev_stderr;
			if ( fn_write )
				log->dev[0]->fn_write = fn_write;

			dev = log->dev[1] = &gs_dev_stdio;
			if ( fn_write )
				dev->fn_write = fn_write;
			EL_ERR_CHECK( elMemAlloc(&dev->data,sizeof(elLogDevStdio_t)) );
			stdio = dev->data;
			if ( fpath )
			{
				stdio->fp = fopen(fpath,"at");
				if ( !stdio->fp )
					EL_ERR_THROW(EL_ERR_IO,NULL,NULL);
			}
			else
				stdio->fp = NULL;

			log->dev_cnt = 2;
		}
	}

Error:

	return err;
}

/******************************************************************************/
void elLogDbgStr(const TCHAR *fmt,...)
{
	char	str[2048+EL_ST];

	va_list args;

	va_start(args,fmt);
	_vstprintf(str,fmt,args);
	_tcscat(str,"\n");
	OutputDebugString(str);
	va_end(args);
}

/******************************************************************************/
