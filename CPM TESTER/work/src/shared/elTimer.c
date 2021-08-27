/*
 *  "elTimer.c" - Timer routines.
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
static char _ident[] = "@(#) elTimer.c 0.1 2007-03-05";
#else
#endif

#include <elBase.h>
#include <elErr.h>
#include <elTimer.h>

static elErr_t gs_err_timer = {
	TRUE,
	{ { {EL_ERR_TIMER},	{_T("Timer error)")} }, NULL, 1, 0 },
	elErrLog
};

#if WIN
static elTimer_t gs_timer_res;	/* timer resolution in ticks per second */
#endif

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elTimerStartup"
EL_DLL_API elErr_t *elTimerStartup(void)
{
	EL_ERR_VAR;

	;

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elTimerCleanup"
EL_DLL_API elErr_t *elTimerCleanup(void)
{
	EL_ERR_VAR;

	;

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elTimer"
EL_DLL_API
#if OLD_PROTOTYPES
elErr_t *elTimer(pt)
register elTimer_t *pt;
#else
elErr_t *elTimer(register elTimer_t *pt)
#endif
{
	register elTimer_t t;
#if MQX
	TIME_STRUCT ts;

	_time_get_elapsed(&ts);
	t = ts.SECONDS * 1000 + ts.MILLISECONDS;
#elif WIN
	LARGE_INTEGER li;

	/*
	 * WARNING: 
	 *
	 *	" Performance counter value may unexpectedly leap forward
	 *	http://support.microsoft.com/default.aspx?scid=KB;EN-US;Q274323&
	 *
	 * See also: http://www.geisswerks.com/ryan/FAQS/timing.html
	 */
	QueryPerformanceCounter(&li);
	t = li.QuadPart;
#elif DOS
	t = clock();
#elif LINUX || CYGWIN
    struct timeval tv;
    struct timezone tz;

#if 1
	gettimeofday(&tv,NULL);
#else
	gettimeofday(&tv,&tz);
#endif
	t = tv.tv_sec * 1000*1000 + tv.tv_usec;
#else
#if 1
	TimeOfDay_t td;

	if ( !TimeOfDay(&td) )
		return &gs_err_timer;
	t = td.sec * 1000 + td.usec / 1000;
#else
#if SCO	/* -- { -- */	
	static ulong tps;
	struct tms dummy;

	if ( !tps )
	{
		switch( tps = sysconf(_SC_CLK_TCK) );
		{
		case -1:
		case 0:
			if ( tps == -1 )		/* viz. times(S) */
				Log("%s(): sysconf(): %m",__FUNC__);
			else
				Log("%s(): tps = 0 !",__FUNC__);
			tps = 100;	/* implicitní hodnota */
			break;
		}
	}
	t = times(&dummy) / tps;
#endif	/* -- } -- SCO -- */	
#endif
#endif
	if ( pt )
		*pt = t;
#if 0
	LogD("%s(): t: %" EL_TIMER_T_FMTSZPFX "u",__FUNC__,t);
#endif

	return NULL;	/* OK (no error) */
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elTimerStart"
EL_DLL_API
#if OLD_PROTOTYPES
elErr_t *elTimerStart(pt)
register elTimer_t *pt;
#else
elErr_t *elTimerStart(register elTimer_t *pt)
#endif
{
#if 1
	elTimer(pt);
#else
	TimeOfDay_t td;

	if ( !TimeOfDay(&td) )
		return -1;
	*pt = td.sec * 1000 + td.usec / 1000;
#endif

	return NULL;	/* no error whatsoever */
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elTimerStop"
EL_DLL_API
#if OLD_PROTOTYPES
elErr_t *elTimerStop(pt)
register elTimer_t *pt;
#else
elErr_t *elTimerStop(register elTimer_t *pt)
#endif
{
#if 1
	elTimer_t t;

	elTimer(&t) ;
	*pt = t - *pt;
#else
	TimeOfDay_t td;

	if ( !TimeOfDay(&td) )
		return -1;
	*pt = (td.sec * 1000 + td.usec / 1000) - *pt;
#endif

	return NULL;	/* no error whatsoever */
}

/******************************************************************************/
#undef  __FUNC__
#define __FUNC__        "elTimerRes"
EL_DLL_API
#if OLD_PROTOTYPES
elTimerRes_t elTimerRes()
#else
elTimerRes_t elTimerRes(void)
#endif
{
#if LINUX || CYGWIN
	return 1000*1000;	/* resolution is 1 microsecond (1M ticks per second) */
#elif MQX || UNIX
	return 1000;	/* resolution is 1 millisecond (1000 ticks per second) */
#elif DJGPP || DOS
	return CLOCKS_PER_SEC;
#elif WIN
	if ( !gs_timer_res )
	{
		LARGE_INTEGER li;

		QueryPerformanceFrequency(&li);
		gs_timer_res = li.QuadPart;
	}
	return gs_timer_res;
#else
#error	elTimerRes not implemented
#endif
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elTimerStr"
EL_DLL_API
#if OLD_PROTOTYPES
TCHAR *elTimerStr(t,width,prec)
register elTimer_t t;
register int width;
register int prec;
#else
TCHAR *elTimerStr(register elTimer_t t,register int width,register int prec)
#endif
{
#if 0
	LogD(_T("t = %lu"),t);
#endif

#if 1
	return elStrTmpFmt(
		_T("%*.*lf"),
		width,prec,elTimer2double(t) / elTimer2double(elTimerRes())
	);
#elif 1
	return elStrTmpIdxFmt(
		EL_STR_IDX_TIMER,
		_T("%*.*lf"),
		width,prec,elTimer2double(t) / elTimer2double(elTimerRes())
	);
#else
	return elStrTmpIdxFmt(
		EL_STR_IDX_TIMER,
		_T("%*lu.%*lu"),a,b,c,d
	);
#endif
}

/******************************************************************************/
