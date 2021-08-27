

#if !defined(__OBJ_LIST_H__)
#define __OBJ_LIST_H__

#include <exception/Exception.h>

#define     OBJ_LIST_NAME_LENGTH		128
#define     OBJ_LIST_ALLOC_QUANTUM		5

typedef struct _SObjListEntry
{
	char	name[OBJ_LIST_NAME_LENGTH+1];
	void*	pObject;
} SObjListEntry, *SObjListEntryPtr;

typedef struct _SObjList
{
	/***************************************************************************
     * Public Members
     **************************************************************************/      
    SElExceptionPtr	(*Add)(struct _SObjList* me, const char* name, void* pObject);
	SElExceptionPtr	(*Set)(struct _SObjList* me, const char* name, void* pObject);
	void*   (*Get)(struct _SObjList* me, const char* name);
	const char*   (*GetNameOnIndex)(struct _SObjList* me, uint32_t	index);
	uint32_t  (*GetCount)(struct _SObjList* me);
	SElExceptionPtr (*Clear)(struct _SObjList* me);
	SElExceptionPtr (*Remove)(struct _SObjList* me, const char* name);
	SElExceptionPtr (*RegisterDestroy)( struct _SObjList* me, void * pFce);

	/**************************************************************************
     * Private Members
     **************************************************************************/
	SElExceptionPtr (*_Destroy)(void** value);
	uint32_t			_AllocCount;
	uint32_t			_UsedCount;
	SObjListEntryPtr	_pList;
} SObjList, *SObjListPtr;


SElExceptionPtr	objlist_new( SObjListPtr* pObjList);	
SElExceptionPtr	objlist_delete( SObjListPtr* pObjList);


#define OBJLIST_ERROR_NULL_PARAMETER            -11111
#define OBJLIST_ERROR_NOT_FOUND                 -22222
#define OBJLIST_ERROR_ALREADY_EXIST             -33333

#endif /* __OBJ_LIST_H__ */
