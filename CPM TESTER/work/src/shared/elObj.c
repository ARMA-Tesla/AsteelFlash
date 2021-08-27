/*
 *  "elObj.c" - Object/Class routines.
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
static char _ident[] = "@(#) elObj.c 0.1 2007-03-05";
#else
#endif

#include <elBase.h>
#include <elObj.h>
#include <elMem.h>
#include <elErr.h>

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elObjNewO"
static elErr_t *elObjNewO(
	const elObjClass_t *oc,elObjPtr_t *po,bool alloc,va_list args
)
{
	EL_ERR_VAR;
	elObj_t *o;

	if ( alloc )
	{
		/* allocate space for object: */
		EL_ERR_CHECK( elMemAlloc(po,oc->size) );
	}
	o = *po;

	EL_ERR_ASSERT( oc->size > sizeof(elObj_t) );

	/*
	 * Initialize object's space:
	 * (this includes setting 'o->ptr' to 'NULL')
	 */
	memset(o,0,oc->size);

	o->alloc = alloc;
	o->class = oc;

	if ( oc->Ctor )		/* constructor ? */
		EL_ERR_CHECK( oc->Ctor(po,args) );		/* call constructor */

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elObjNew"
EL_DLL_API elErr_t *elObjNew(const void *oc_,void *po_)
{
	EL_ERR_VAR;
	const elObjClass_t *oc = oc_;
	elObjPtr_t *po = po_;

	EL_ERR_CHECK( elObjNewO(oc,po,TRUE,NULL) );

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elObjNewV"
EL_DLL_API elErr_t *elObjNewV(const void *oc_,void *po_,...)
{
	EL_ERR_VAR;
	const elObjClass_t *oc = oc_;
	elObjPtr_t *po = po_;
	va_list args;

	va_start(args,po_);

#if 0
	{	/* inner block */
	TCHAR *str = va_arg(args,TCHAR *);

	
	_tprintf(_T("%s(): args: 0x%lX, str: 0x%lX\n"),_T(__FUNC__),args,str);
	_tprintf(_T("%s(): str: '%s'\n"),_T(__FUNC__),str);
	}	/* inner block */
#endif

	EL_ERR_CHECK( elObjNewO(oc,po,TRUE,args) );

Error:

	va_end(args);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elObjNewH"
EL_DLL_API elErr_t *elObjNewH(const void *oc_,elObjHandle_t oh,size_t oh_size)
{
	EL_ERR_VAR;
	const elObjClass_t	*oc = oc_;
	elObj_t				*o;
	elObjPtr_t			*po;
	bool				alloc;

	/* object's handle points to pointer ? */
	if ( oh_size == sizeof(void *) )
	{
		/* object will be allocated (not static structure): */
		alloc = TRUE;
		o = NULL;
		po = (elObjPtr_t *) oh;
	}
	else
	{
		/* object will not be allocated (it is a static structure): */
		alloc = FALSE;
		o = oh;
		po = NULL;
	}

	EL_ERR_CHECK( elObjNewO(oc,&o,alloc,NULL) );

	/* object's handle points to pointer ? */
	if ( oh_size == sizeof(void *) )
		*po = o;

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elObjDeleteO"
static elErr_t *elObjDeleteO(elObj_t *o)
{
	EL_ERR_VAR;
	const elObjClass_t *oc;

	if ( o )
	{
		/* object's class: */
		oc = o->class;

		EL_ERR_ASSERT( oc->size > sizeof(elObj_t) );

		if ( oc->Dtor )		/* destructor defined ? */
			EL_ERR_CHECK( oc->Dtor(&o) );	/* call destructor */

		if ( o->alloc )
			elMemFree(&o);
	}

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elObjDelete"
EL_DLL_API elErr_t *elObjDelete(void *po_)
{
	EL_ERR_VAR;
	elObjPtr_t *po = po_;

	EL_ERR_CHECK( elObjDeleteO(*po) );

	*po = NULL;

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elObjDeleteH"
EL_DLL_API elErr_t *elObjDeleteH(elObjHandle_t oh,size_t oh_size)
{
	EL_ERR_VAR;
	elObj_t *o;

	/* object's pointer from object's handle: */
	o = oh_size == sizeof(void *) ? *(void **)oh : oh;

	EL_ERR_CHECK( elObjDeleteO(o) );

	/* object's handle points to pointer ? */
	if ( oh_size == sizeof(void *) )
		*(void **)oh = NULL;

Error:

	return err;
}

/******************************************************************************/
