/*
 *  "elMtx.c" - Mutex synchronization routines.
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
static char _ident[] = "@(#) elMtx.c 0.1 2007-03-05";
#else
#endif

#include <elBase.h>
#include <elMtx.h>
#include <elLog.h>

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elMtxStartup"
EL_DLL_API elErr_t *elMtxStartup(void)
{
	EL_ERR_VAR;

	return err;	
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elMtxCleanup"
EL_DLL_API elErr_t *elMtxCleanup(void)
{
	EL_ERR_VAR;

	return err;	
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elMtxNew"
EL_DLL_API elErr_t *elMtxNew(elMtxPtr_t mtx,char *name)
{
	EL_ERR_VAR;

	if ( !mtx->init )
	{
		if ( name )
		{
			EL_ERR_THROW(EL_ERR_INVAL,NULL,NULL);
			mtx->named = TRUE;
		}
		else
		{
			mtx->named = FALSE;
#if WIN
			InitializeCriticalSection(&mtx->cs);
			mtx->cs_init = TRUE;
#endif
		}
	}

	mtx->init = TRUE;

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elMtxDelete"
EL_DLL_API elErr_t *elMtxDelete(elMtxPtr_t mtx)
{
	EL_ERR_VAR;

	if ( mtx->init )
	{
		if ( mtx->named )
		{
#if WIN
			DeleteCriticalSection(&mtx->cs);
			mtx->cs_init = FALSE;
#endif
		}
	}
	mtx->init = FALSE;

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elMtxLock"
EL_DLL_API elErr_t *elMtxLock(register elMtx_t *mtx)
{
	EL_ERR_VAR;

#if 0
	elLogDbgStr("%s(TID=%lu): mtx: 0x%lX",
		__FUNC__,GetCurrentThreadId(),mtx);
#endif

	if ( mtx->named )
		EL_ERR_THROW(EL_ERR_INVAL,NULL,NULL);
	else
	{
#if WIN
		if ( mtx->cs_init )
			EnterCriticalSection(&mtx->cs);
		else
		{
#if 1
			char mtx_name[128+EL_ST];

			sprintf(mtx_name,"elMtx 0x%p",mtx);
			mtx->handle = CreateMutex(NULL,FALSE,mtx_name);
#else
			mtx->handle = CreateMutex(NULL,FALSE,NULL);
#endif
			WaitForSingleObject(mtx->handle,INFINITE);
		}
#endif
	}

Error:
	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elMtxUnlock"
EL_DLL_API elErr_t *elMtxUnlock(register elMtx_t *mtx)
{
	EL_ERR_VAR;

	if ( mtx->named )
		EL_ERR_THROW(EL_ERR_INVAL,NULL,NULL);
	else
	{
#if WIN
		if ( mtx->cs_init )
			LeaveCriticalSection(&mtx->cs);
		else
			ReleaseMutex(mtx->handle);
#endif
	}

Error:
	return err;
}

/******************************************************************************/
