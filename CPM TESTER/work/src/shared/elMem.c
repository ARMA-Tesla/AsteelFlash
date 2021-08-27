/*
 *  "elMem.c" - Memory allocation routines.
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
static char _ident[] = "@(#) elMem.c 0.1 2007-03-05";
#else
#endif

#include <elBase.h>
#include <elObj.h>
#include <elErr.h>
#include <elMem.h>

#if !defined(_CRTDBG_MAP_ALLOC)
/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elMemAlloc"
EL_DLL_API elErr_t *elMemAlloc(void *ptr,size_t size)
{
	EL_ERR_VAR;
	void **p_ptr = ptr;

#if 0
	fprintf(stderr,"%s(): p_ptr: 0x%lX, size: %lu\n",__FUNC__,p_ptr,size);
	fprintf(stderr,"%s(): 10\n",__FUNC__);
#endif
#if 0
	EL_MEM_CHECK_CRT();
#endif
#if 0
	fprintf(stderr,"%s(): 15\n",__FUNC__);
#endif

	if ( !size )
		*p_ptr = NULL;
	else
	{
#if 1
		*p_ptr = malloc(size);
#else
		{	/* inner block */
		void *ptr2 = malloc(size);
		fprintf(stderr,"%s(): 16\n",__FUNC__);
		*p_ptr = ptr2;
		}	/* inner block */
#endif

#if 0
		fprintf(stderr,"%s(): 20\n",__FUNC__);
#endif

#if 1
		if ( !*p_ptr )
#else
		if ( 1 )	/* test */
#endif
		{
			elErrSet(
				&err,EL_ERR_MEM,(elErrId_t)-1,(elErrId_t)-1,
				_T(__FILE__),__LINE__,
				_T(__FUNC__),_T("malloc"),
				_T("cannot allocate memory (%ld bytes)"),size
			);
		}
	}

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elMemAllocZero"
EL_DLL_API elErr_t *elMemAllocZero(void *ptr,size_t size)
{
	EL_ERR_VAR;
	void **p_ptr = ptr;

	EL_ERR_CHECK( elMemAlloc(ptr,size) );
	memset(*p_ptr,0,size);

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elMemRealloc"
EL_DLL_API elErr_t *elMemRealloc(void *ptr,size_t size)
{
	EL_ERR_VAR;
	void **p_ptr = ptr;

	if ( !size )
	{
		free(*p_ptr);
		*p_ptr = NULL;
	}
	else
	{
		*p_ptr = realloc(*p_ptr,size);

		if ( !*p_ptr )
		{
			elErrSet(
				&err,EL_ERR_MEM,(elErrId_t)-1,(elErrId_t)-1,
				_T(__FILE__),__LINE__,
				_T(__FUNC__),_T("realloc"),
				_T("cannot reallocate memory (%ld bytes)"),size
			);
		}
	}

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elMemFree"
EL_DLL_API void elMemFree(void *ptr)
{
	void **p_ptr = ptr;

	if ( p_ptr )
	{
		free(*p_ptr);
		*p_ptr = NULL;
	}
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elMemStrdup"
EL_DLL_API elErr_t *elMemStrdup(TCHAR **p_dupstr,TCHAR *str)
{
	EL_ERR_VAR;
	size_t size;

	if ( !str )
		*p_dupstr = NULL;
	else
	{
		size = _tcslen(str) + EL_ST;

		EL_ERR_CHECK( elMemAlloc(p_dupstr,size * sizeof(*str)) );
		// "elMemStrdup(): elMemAlloc(): cannot allocate memory (%ld bytes)"
		_tcscpy(*p_dupstr,str);
#if 1
		(*p_dupstr)[size-EL_ST] = _T('\0');
#else
		/* BUG: undefined (not allocated) memory overwritten: */
		(*p_dupstr)[size] = _T('\0');
#endif

#if 0
		/* the following macro should handle above memory bug: */
		EL_MEM_CHECK_CRT();
#endif
	}

Error:

	return err;
}
#endif /* '!defined(_CRTDBG_MAP_ALLOC)' */

/******************************************************************************/
