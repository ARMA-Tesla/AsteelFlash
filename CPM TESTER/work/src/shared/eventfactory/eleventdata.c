
#include "eleventdata.h"
#include <stdlib.h>

/******************************************************************************
 * Static function declaration exported as class methods
 ******************************************************************************/

static const char*	GetStringPtr(
	struct _SElEventData* me
);
static SElExceptionPtr	SetStringPtr(
	struct _SElEventData* me,
	const char* pData
);

/******************************************************************************
 * Second thread function
 ******************************************************************************/
#undef __FUNC__
#define __FUNC__ "eleventdata_new"
SElExceptionPtr eleventdata_new(
	SElEventDataPtr* pEventData
)
{
    SElExceptionPtr pexception = NULL;

    *pEventData = (SElEventDataPtr)calloc(1, sizeof(SElEventData));
    EXCCHECKALLOC( *pEventData);    
    (*pEventData)->GetStringPtr = GetStringPtr;
    (*pEventData)->SetStringPtr = SetStringPtr;

    EXCCHECK( elstring_new(NULL, &((*pEventData)->_pString)));
    
Error:
    if ( pexception )
    {
    	eleventdata_delete(pEventData);
    }
    EXCRETHROW( pexception);	
} /* eleventdata_new */

#undef __FUNC__
#define __FUNC__ "eleventdata_delete"
SElExceptionPtr eleventdata_delete(
	SElEventDataPtr* pEventData
)
{
    SElExceptionPtr pexception = NULL;
    
    if ( pEventData && *pEventData )
    {
        elstring_delete(&((*pEventData)->_pString));
        free( *pEventData);
        *pEventData = NULL;    
    }

#if 0
Error:
#endif
    EXCRETHROW( pexception);
} /* eleventdata_delete */


/******************************************************************************
 * Static function definition exported as class methods
 ******************************************************************************/
/*----------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "SElEventData::GetStringPtr" 
static const char*	GetStringPtr(struct _SElEventData* pEventData)
{
	return (pEventData->_pString)->GetPtr(pEventData->_pString);
}

/*----------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "SElEventData::SetStringPtr" 
static SElExceptionPtr	SetStringPtr(
	struct _SElEventData* pEventData,
	const char* pData
)
{
    SElExceptionPtr pexception = NULL;	
	EXCCHECK( (pEventData->_pString)->Duplicate(pEventData->_pString, pData));
Error:
    EXCRETHROW( pexception);
}


