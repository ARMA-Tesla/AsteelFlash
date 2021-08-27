/*
 *  "elTls.c" - Thread Local Storage (TLS) routines.
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
static char _ident[] = "@(#) elTls.c 0.1 2007-03-05";
#else
#endif

#include <elBase.h>
#include <elErr.h>
#include <elTls.h>
#include <elLog.h>

typedef struct SelTlsData {
	void *val[EL_TLS_IDX_cnt];
} elTlsData_t, *elPTlsData_t, *elTlsDataPtr_t;

static DWORD gs_tls_idx = TLS_OUT_OF_INDEXES;

#if 0
/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"log_tls_idx"
static void log_tls_idx(char *func)
{
#if 0
	(void) func;
#else
	elLogDbgStr(
		"%s(TID=%lu): gs_tls_idx (0x%p): %ld",
		func,GetCurrentThreadId(),&gs_tls_idx,gs_tls_idx
	);
#endif
}
#endif

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elTlsStartup"
EL_DLL_API elErr_t *elTlsStartup(void)
{
	EL_ERR_VAR;

	/*
	 * The mutex is not necessary here, because 'elTlsStartup()' is supposed
	 * to be called by only one thread (usually the startup one):
	 */

	if ( gs_tls_idx == TLS_OUT_OF_INDEXES )
	{
		gs_tls_idx = TlsAlloc();
		EL_ERR_CHECK_COND(
			gs_tls_idx == TLS_OUT_OF_INDEXES,
			EL_ERR_TLS,_T("TlsAlloc() failed")
		);
#if 0
		log_tls_idx(__FUNC__);
#endif
	}

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elTlsCleanup"
EL_DLL_API elErr_t *elTlsCleanup(void)
{
	EL_ERR_VAR;

	TlsFree(gs_tls_idx);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elTlsDataGet"
static elErr_t *elTlsDataGet(elTlsDataPtr_t *p_tls_data)
{
	EL_ERR_VAR;

#if 0
	log_tls_idx(__FUNC__);
#endif

	EL_ERR_CHECK_COND(
		gs_tls_idx == TLS_OUT_OF_INDEXES,
		EL_ERR_TLS,_T("TLS not initialized")
	);

	*p_tls_data = TlsGetValue(gs_tls_idx);
#if 0
	elLogDbgStr("%s(TID=%lu): gs_tls_idx: %lu, *p_tls_data: 0x%lX",
		__FUNC__,GetCurrentThreadId(),gs_tls_idx,*p_tls_data);
#endif
	if ( !*p_tls_data )
	{
		EL_ERR_CHECK_COND(
			GetLastError() != NO_ERROR,
			EL_ERR_TLS,_T("TlsGetValue() failed")
		);
		
#if 0
		/* 'LPTR' = 'LMEM_FIXED | LMEM_ZEROINIT': */
		*p_tls_data = LocalAlloc(LPTR,sizeof(elTlsData_t)); 
		EL_ERR_CHECK_COND(
			!*p_tls_data,EL_ERR_TLS,_T("LocalAlloc() failed")
		);
#else
		*p_tls_data = malloc(sizeof(elTlsData_t));
		EL_ERR_CHECK_COND(
			!*p_tls_data,EL_ERR_TLS,_T("malloc() failed")
		);
		memset(*p_tls_data,0,sizeof(elTlsData_t));
#endif

		EL_ERR_CHECK_COND(
			!TlsSetValue(gs_tls_idx,*p_tls_data),
			EL_ERR_TLS,_T("TlsSetValue() failed")
		);
#if 1
		elLogDbgStr("%s(TID=%lu): *p_tls_data: 0x%lX",
			__FUNC__,GetCurrentThreadId(),*p_tls_data);
#endif
	}

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elTlsValSet"
EL_DLL_API elErr_t *elTlsValSet(int idx,void *val)
{
	EL_ERR_VAR;
	elTlsData_t *tls_data;

	EL_ERR_CHECK_COND(
		(uint)idx >= EL_ARRAY_CNT(tls_data->val),
		EL_ERR_TLS,_T("invalid index")
	);

	EL_ERR_CHECK( elTlsDataGet(&tls_data) );

#if 1
	if ( tls_data->val[idx] != val )
	{
		elLogDbgStr("%s(TID=%lu): tls_data->val[%d] (0x%lX): 0x%lX => 0x%lX",
			__FUNC__,GetCurrentThreadId(),idx,&tls_data->val[idx],
			tls_data->val[idx],val);
	}
#endif

	tls_data->val[idx] = val;

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elTlsValGet"
EL_DLL_API elErr_t *elTlsValGet(int idx,void **p_val)
{
	EL_ERR_VAR;
	elTlsData_t *tls_data;

	EL_ERR_CHECK_COND(
		(uint)idx >= EL_ARRAY_CNT(tls_data->val),
		EL_ERR_TLS,_T("invalid index")
	);

	EL_ERR_CHECK( elTlsDataGet(&tls_data) );

#if 1
	if ( tls_data->val[idx] == (void *)-1 )
	{
		elLogDbgStr("%s(TID=%lu): tls_data->val[%d] (0x%lX) = -1 !",
			__FUNC__,GetCurrentThreadId(),idx,&tls_data->val[idx]);
	}
#endif

	*p_val = tls_data->val[idx];

Error:

	return err;
}

/******************************************************************************/
