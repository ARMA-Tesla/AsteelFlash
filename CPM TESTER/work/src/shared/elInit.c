/*
 *  "elInit.c" - Startup and Cleanup routines.
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
static char _ident[] = "@(#) elInit.c 0.1 2007-03-05";
#else
#endif

#include <elBase.h>
#include <elTls.h>
#include <elErr.h>
#include <elMsg.h>
#include <elLog.h>

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elStartup"
EL_DLL_API elErr_t *elStartup(void)
{
	EL_ERR_VAR;

	elLogDbgStr("%s(TID=%lu)",__FUNC__,GetCurrentThreadId());
	
	EL_ERR_CHECK( elLogStartup() );
	EL_ERR_CHECK( elTlsStartup() );
	EL_ERR_CHECK( elErrStartup() );
	EL_ERR_CHECK( elStrStartup() );
	EL_ERR_CHECK( elMsgStartup() );

Error:
	return err;	
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elCleanup"
EL_DLL_API elErr_t *elCleanup(void)
{
	EL_ERR_VAR;

	elLogDbgStr("%s(TID=%lu)",__FUNC__,GetCurrentThreadId());
	
	elMsgCleanup();
	elStrCleanup();
	elErrCleanup();
	elTlsCleanup();
	elLogCleanup();

	return err;	
}

/******************************************************************************/
