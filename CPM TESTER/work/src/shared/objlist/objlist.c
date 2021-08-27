
#include "objlist.h"
#include <string.h>
#include <stdlib.h>
#include <elerror.h>

/******************************************************************************
 * Static function declaration exported as class methods
 ******************************************************************************/
static SElExceptionPtr	Add(
    struct _SObjList* pObjList,
    const char* name,
    void* pObject
);
static SElExceptionPtr Set(
	struct _SObjList* pObjList,
	const char*		  name,
	void*			  value
);
static void* Get(
    struct _SObjList* pObjList,
    const char* name
);
static uint32_t  GetCount(
    struct _SObjList* pObjList
);
static const char* GetNameOnIndex(
    struct _SObjList* pObjList,
    uint32_t	      index
);
static SElExceptionPtr Clear(
	struct _SObjList* pObjList
);
static SElExceptionPtr Remove(
	struct _SObjList* pObjList,
	const char*		  name
);
static SElExceptionPtr RegisterDestroy(
	struct _SObjList* pObjList,
	void* pFce
);


/******************************************************************************
 * Static function declaration non-exported functions
 ******************************************************************************/
static SElExceptionPtr objlistGetIndex(
	struct _SObjList* pObjList,
	const char*		  name,
	uint32_t*		  pIndex
);


/******************************************************************************
 * Exported function definition
 ******************************************************************************/
/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "objlist_new"
SElExceptionPtr	objlist_new(
    SObjListPtr* pObjList
)
{
	SElExceptionPtr pexception = NULL;
	SObjListPtr		plist = NULL;

	*pObjList = calloc( 1, sizeof(SObjList));
	EXCCHECKALLOC( *pObjList);

	(*pObjList)->Add             = Add;
    (*pObjList)->Set             = Set;
	(*pObjList)->Get             = Get;
	(*pObjList)->GetCount        = GetCount;
	(*pObjList)->GetNameOnIndex  = GetNameOnIndex;
	
    (*pObjList)->Clear           = Clear;
    (*pObjList)->Remove          = Remove;
    
    (*pObjList)->RegisterDestroy = RegisterDestroy;

Error:
	EXCRETHROW( pexception);
} /* objlist_new */

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "objlist_delete"
SElExceptionPtr	objlist_delete(
    SObjListPtr* pList
)
{
	SElExceptionPtr pexception = NULL;
	
	if ( pList && *pList)
	{
		(*pList)->Clear( *pList);
		free( (*pList)->_pList);
		free( *pList);
		*pList = NULL;
	}
	EXCRETHROW( pexception);
} /* objlist_delete */

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SObjList::Add"
static SElExceptionPtr	Add(
    struct _SObjList* pObjList,
    const char*       name,
    void*             pObject
)
{
	SElExceptionPtr pexception = NULL;
	uint32_t	index = 0xFFFFFFFFUL;
	

	pexception = objlistGetIndex( pObjList,
							      name,
							      &index );
    EXCIGNORE( OBJLIST_ERROR_NOT_FOUND );
    EXCCHECK( pexception);
	
    if ( index != 0xFFFFFFFFUL )
	{
	   EXCTHROW( OBJLIST_ERROR_ALREADY_EXIST, "Object with this name already exist" );	
	}
	
	index = pObjList->_UsedCount;
	
	if ( pObjList->_AllocCount == pObjList->_UsedCount )
	{
		uint32_t new_count = pObjList->_AllocCount + OBJ_LIST_ALLOC_QUANTUM;
		
        pObjList->_pList = realloc( pObjList->_pList,
                                    new_count * sizeof(SObjListEntry));
		EXCCHECKALLOC( pObjList->_pList);
		
        pObjList->_AllocCount = new_count; 
	} /* if ( pObjList->_AllocCount == pObjList->_UsedCount ) */

	/*
	 * ToDo: generate warning if the name is longer
	 */	 
    strncpy( (pObjList->_pList+index)->name, name, OBJ_LIST_NAME_LENGTH );
	(pObjList->_pList + index)->pObject = pObject;
	pObjList->_UsedCount += 1;

Error:
	EXCRETHROW( pexception);
} /* SObjList::Add */


/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SObjList::Set"
static SElExceptionPtr Set(
	struct _SObjList* pObjList,
	const char*		  name,
	void*			  value
	
)
{
    SElExceptionPtr    pexception = NULL;
	uint32_t           index = 0xFFFFFFFFUL;
	
	pexception = objlistGetIndex( pObjList,
							      name,
                                 &index );
    EXCIGNORE( OBJLIST_ERROR_NOT_FOUND );
    EXCCHECK( pexception);
    
	if ( index == 0xFFFFFFFFUL )
	{
		EXCCHECK( pObjList->Add( pObjList, name, value));
	}
	else
	{
		if (pObjList->_Destroy)
		{
			/*
			 * The destroy function is set. in this case first destroy object
			 */			 
            pObjList->_Destroy( &((pObjList->_pList + index)->pObject));
		}
		(pObjList->_pList + index)->pObject = value; 
	}

Error:
	EXCRETHROW( pexception);
} /* SObjList::Set */

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SObjList::Get"
static void* Get(
    struct _SObjList* pObjList,
    const char* name
)
{
	void*		pvalue = NULL;
	SObjListEntryPtr	pitem = pObjList->_pList;

	if ( name != NULL )
    {
        uint32_t	i;
    	for( i = 0 ; i < pObjList->_UsedCount && pvalue == NULL; i++, pitem++)
    	{
    		if ( strcmp( pitem->name, name ) == 0 )
    		{
                pvalue = pitem->pObject;
                break;
    		} /* if ( strcmp() == 0 ) */
    	} /* for ... */
	} /* if ( name != NULL ) */

	return pvalue;
} /* SObjList::Get */

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SObjList::GetCount"
static uint32_t GetCount(
    struct _SObjList*   pObjList
)
{
	return  pObjList->_UsedCount;
} /* SObjList::GetCount */

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SObjList::GetNameOnIndex"
static const char* GetNameOnIndex(
    struct _SObjList*   pObjList,
    uint32_t	        index
)
{
	if ( index >= pObjList->_UsedCount )
		return NULL;
	
	return (pObjList->_pList + index)->name;
} /* SObjList::GetNameOnIndex */


/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SObjList::Clear"
static SElExceptionPtr Clear(
	struct _SObjList* pObjList
)
{
	uint32_t	i = 0;
	
	for (i = 0; i < pObjList->_UsedCount; i++)
	{
		*((pObjList->_pList + i)->name) = '\0';
		if (pObjList->_Destroy)
		{
			pObjList->_Destroy( &((pObjList->_pList + i)->pObject));
		}
	}
	pObjList->_UsedCount = 0;

	return NULL;
} /* SObjList::Clear */

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SObjList::Remove"
static SElExceptionPtr Remove(
	struct _SObjList* pObjList,
	const char*		  name
)
{
	uint32_t	i = 0;
	int32_t		count;
	
	for (i = 0; i < pObjList->_UsedCount; i++)
	{
		if ( strcmp( (pObjList->_pList + i)->name, name) == 0 )
		{
			if (pObjList->_Destroy)
			{
				pObjList->_Destroy( &((pObjList->_pList + i)->pObject));
			} /* if (pObjList->_Destroy) */
			count = (int32_t)(pObjList->_UsedCount - i) - 1; 
			
            if ( count > 0 )
			{
				memmove( pObjList->_pList + i,
                         pObjList->_pList + i + 1 ,
                         sizeof(SObjListEntry) * count
                       );
			} /* if ( count > 0 ) */
			pObjList->_UsedCount -= 1;
			break;
		} /* if ( strcmp( (pObjList->_pList + i)->name, name) == 0 ) */
		
	} /* for ( ... ) */

	return NULL;
} /* SObjList::Remove */

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SObjList::RegisterDestroy"
static SElExceptionPtr RegisterDestroy(
	struct _SObjList* pObjList,
	void*	pFce
)
{
	pObjList->_Destroy = pFce;
	return NULL;
} /* SObjList::RegisterDestroy */

/******************************************************************************
 * Static function definition non-exported functions
 ******************************************************************************/
 #undef  __FUNC__
 #define __FUNC__ "objlistGetIndex"
 static SElExceptionPtr objlistGetIndex(
	struct _SObjList* pObjList,
	const char*		  name,
	uint32_t*		  pIndex
)
{
 	SElExceptionPtr pexception = NULL;
 	uint32_t	    i, index = 0;

 	if ( name == NULL )
	{
        EXCTHROW( OBJLIST_ERROR_NULL_PARAMETER, "Parameter 2(Name) is NULL pointer" );
	}

	for( i = 0 ; i < pObjList->_UsedCount; i++)
	{
		if ( strcmp( (pObjList->_pList + i)->name, name ) == 0 )
		{
			if ( pIndex) *pIndex = i;
			return NULL;
		}
	}
	
    EXCTHROW( OBJLIST_ERROR_NOT_FOUND, "Object not found");

Error:
	EXCRETHROW( pexception);
}  /* objlistGetIndex */
