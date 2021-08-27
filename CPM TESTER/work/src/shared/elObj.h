/*
 *  <elObj.h> - Definitions for module "elObj.c".
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

#if /* (WIN32 || _WIN32 || _WINDOWS) && */ _MSC_VER >= 1000
#pragma once
#endif 

#if !defined(_EL_OBJ_H)
#define _EL_OBJ_H

#if _MSC_VER < 1400 && !__BCOPT__ && !_CVI_  /* Borland C/C++: Unknown preprocessor directive: 'ident' */
#if 1
#ident "@(#) elObj.h 0.1 2007-03-05"
#else
#endif
#endif

/* forward reference: */
struct SelObj;
typedef struct SelObj elObj_t, *elPObj_t, *elObjPtr_t;

/* forward reference: */
struct SelErr;
typedef struct SelErr elErr_t, *elPErr_t, *elErrPtr_t;

#include <elTypes.h>

typedef struct SelObjClass {
	size_t		size;
	char 		*name;
	elErr_t		*(*Ctor)	(elObjPtr_t *po,va_list args);
	elErr_t		*(*Dtor)	(elObjPtr_t *po);
	elObj_t		*(*Clone)	(const elObj_t *o);
	int			(*Differ)	(const elObj_t *o,const elObj_t *o2);
} elObjClass_t, *elPObjClass_t, *elObjClassPtr_t;

/*
 * 'elObj_t': Object data type (common members of all objects).
 */
struct SelObj {
	/*
	 * 'ptr': Object's pointer.
	 *
	 * It is used to determine whether object's handle passed to a function
	 * points to structure (object itself) or to pointer (object's pointer).
	 *
	 * The 'ptr' in this structure must be first member and is always set
	 * to 'NULL', so that object's method that supports object's *handle*
	 * parameter can check, wthether the parameter is pointer to object itself
	 * (the handle points to 'ptr' that is 'NULL') or parameter is pointer
	 * to object's pointer (the handle points to 'ptr' that is not 'NULL').
	 * See 'elObjPtrH()' macro.
	 *
	 * It is used in case an object is defined as either structure or pointer,
	 * but object's method/function is called always with object's handle
	 * (address of variable defining object), for example:
	 *
	 *	#if 1
	 *		elIo_t *io;
	 *	#else
	 *		elIo_t  io;
	 *	#else
	 *
	 *		EL_NEW(elIo-c,&io);
	 *		elIoOpen(&io,...);				// passing object's handle
	 *		elIoP(io)->Open(&io,...);		// passing object's handle
	 *		EL_DELETE(&io);
	 */
	elObj_t			*ptr;		/* always 'NULL' */

	/*
	 * 'alloc': Object is either allocated or not (static structure).
	 */
	bool			alloc;

	/*
	 * 'class': Object's class.
	 *
	 * Defines size of object, constructor, desctructor and other common methods.
	 */
	const elObjClass_t
					*class;
};

/*
 * 'elObjHandle_t': Object's handle.
 *
 * Object's handle is pointer to variable ('&var') that defines the object.
 * It is either (more often) pointer to pointer to the structure defining
 * object:
 *
 *		elIo_t *io;			// object's instance as pointer
 *		elNew(elIo_c,&io);	// '&io' is object's handle
 *
 * or just pointer to the structure defining object:
 *
 *		elIo_t io;			// object's instance as structure 
 *		elNew(elIo_c,&io);	// '&io' is object's handle
 */
typedef void *		elObjHandle_t;

#include <elErr.h>

/*
 * 'EL_POBJ_PARAM_IS_HANDLE':
 *
 * Defines, whether object's methods support object's handle parameter.
 *
 * WARNING: If this feature is enabled, then type checking is not performed
 * for the object's handle parameter passed to object's method,
 * because the object's handle is of type 'void *'.
 */
#define EL_POBJ_PARAM_IS_HANDLE		1

#if 0	/* debug: */
#define elObjPtrH_stc_debug(obj_handle)		\
		fprintf(stderr,"%s is pointer to structure\n",#obj_handle)
#define elObjPtrH_ptr_debug(obj_handle)		\
		fprintf(stderr,"%s is pointer to pointer\n",  #obj_handle)
#else
#define elObjPtrH_stc_debug(obj_handle)		0
#define elObjPtrH_ptr_debug(obj_handle)		0
#endif

/*
 * 'elObjPtrHS()'
 * 'elObjPtr()'
 * 'elP()'
 *
 * These macros obtain object's pointer from object's handle and object's handle
 * size.
 */
#define elObjPtrHS(obj_handle,obj_handle_size)	(						\
			obj_handle_size == sizeof(void *)							\
			? (elObjPtrH_ptr_debug(obj_handle), *(void **)obj_handle)	\
			: (elObjPtrH_stc_debug(obj_handle),   (void *)obj_handle)	\
		)
#define elObjPtr(type,obj_var)		\
		( (el##type##_t *) elObjPtrHS(&obj_var,sizeof(obj_var)) )
#define elP			elObjPtr

/*
 * 'elObjPtrH()':
 *
 * This macro obtains object's pointer from object's handle.
 */
#define elObjPtrH(obj_handle)	\
		(*((void **)obj_handle) ? *((void **)obj_handle) : (obj_handle) )

#define elNew			elObjNew
#define elNewV			elObjNewV
#define elDelete(obj)	elObjDelete(obj)

#define EL_NEW(obj_class,obj_handle)		\
		elObjNewH(obj_class,obj_handle,sizeof(*obj_handle))
#define EL_DELETE(obj_handle)				\
		elObjDeleteH(obj_handle,sizeof(*obj_handle))


#ifdef __cplusplus
extern "C" {			/* C Plus Plus function bindings */
#endif

EL_DLL_API elErr_t *elObjNew(const void *oc,void *po);
EL_DLL_API elErr_t *elObjNewV(const void *oc_,void *po_,...);
EL_DLL_API elErr_t *elObjNewH(const void *oc,elObjHandle_t oh,size_t oh_size);
EL_DLL_API elErr_t *elObjDelete(void *po_);
EL_DLL_API elErr_t *elObjDeleteH(elObjHandle_t oh,size_t oh_size);

#ifdef __cplusplus
}						/* C Plus Plus function bindings */
#endif

#endif 	/* _EL_OBJ_H */
