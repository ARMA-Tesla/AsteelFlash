
#include "msgList.h"
#include <string.h>
#include <stdio.h>

static SElExceptionPtr msg_Add(struct _smsglist* me, char* msg);
static SElExceptionPtr msg_Delete(struct _smsglist* me, char* msg);
static SElExceptionPtr msg_DeleteAll(struct _smsglist* me);
static SElExceptionPtr msg_GetMsg(struct _smsglist* me, char* buffer, int32_t size);

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"msglist_new"
SElExceptionPtr msglist_new(SMsgListPtr* msglist)
{
	SElExceptionPtr	pexception = NULL;
	SMsgListPtr		new_msg_list = NULL;
	
	new_msg_list = (SMsgListPtr) malloc(sizeof(SMsgList));
	EXCCHECKALLOC( new_msg_list);
	
	new_msg_list->_allocSize = 0;
	new_msg_list->_msgList = NULL;
	new_msg_list->_valid = 0;
	
	new_msg_list->Add = msg_Add;
	new_msg_list->Delete = msg_Delete;
	new_msg_list->DeleteAll = msg_DeleteAll;
	new_msg_list->GetMsg = msg_GetMsg;
	
Error:
	if (msglist)
		*msglist = new_msg_list;

	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"msglist_delete"
SElExceptionPtr msglist_delete(SMsgListPtr* msglist)
{
	SElExceptionPtr	pexception = NULL;
	
	(*msglist)->DeleteAll(*msglist);
	
	free(*msglist);
	*msglist = NULL;
	
#if 0
Error:
#endif
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"msg_Add"
static SElExceptionPtr msg_Add(struct _smsglist* me, char* msg)
{
	SElExceptionPtr	pexception = NULL;
	SStrItemPtr*	new_item = NULL;
	
	new_item = &(me->_msgList);
	while (*new_item != NULL)
	{
		if (!strcmp((*new_item)->msg, msg))
			break;
		new_item = &((*new_item)->pnext);
	}
	if (*new_item == NULL)
	{
		*new_item = (SStrItemPtr) malloc(sizeof(SStrItem));
		(*new_item)->msg = (char*) calloc(strlen(msg) + 1, sizeof(char));
		strcpy_s((*new_item)->msg, strlen(msg) + 1, msg);
		(*new_item)->pnext = NULL;
		
		me->_valid = 0;
		me->_allocSize++;
	}
	
#if 0
Error:
#endif
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"msg_Delete"
static SElExceptionPtr msg_Delete(struct _smsglist* me, char* msg)
{
	SElExceptionPtr	pexception = NULL;
	SStrItemPtr		item = me->_msgList,
					prev = NULL;
	
	while (item != NULL)
	{
		if (!strcmp(item->msg, msg))
		{
			if (prev == NULL)
			{
				me->_msgList = item->pnext;
			}
			else
			{
				prev->pnext = item->pnext;
			}
			free(item->msg);
			free(item);
			me->_valid = 0;
			break;
		}
		
		prev = item;
		item = item->pnext;
	}
	
#if 0
Error:
#endif
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"msg_DeleteAll"
static SElExceptionPtr msg_DeleteAll(struct _smsglist* me)
{
	SElExceptionPtr	pexception = NULL;
	SStrItemPtr		item = me->_msgList,
					next = NULL;
	
	while (item != NULL)
	{
		next = item->pnext;
		
		free(item->msg);
		free(item);
		
		item = next;
	}
	
	if(me->_msgList)
		me->_msgList = NULL;

	me->_valid = 0;
	
#if 0
Error:
#endif
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"msg_GetMsg"
static SElExceptionPtr msg_GetMsg(struct _smsglist* me, char* buffer, int32_t size)
{
	SElExceptionPtr	pexception = NULL;
	SStrItemPtr		item = me->_msgList;
	int32_t			count = 0;
	char*			p2buffer = buffer;
	uint32_t		rest_size = size;
	
	memset(buffer, 0, size);
	while (item != NULL && rest_size > strlen(item->msg))
	{
#if _CVI_ || _MSC_VER <= 1200
		(void) rest_size;
		count = sprintf(p2buffer, "%s\n", item->msg);
#else
		count = sprintf_s(p2buffer, rest_size, "%s\n", item->msg);
#endif
		p2buffer += count;
		rest_size -= count;
		item = item->pnext;
	}
	
	me->_valid = 1;
	
#if 0
Error:
#endif
	EXCRETHROW( pexception);
}
