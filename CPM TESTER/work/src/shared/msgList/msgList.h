
#ifndef _MSG_LIST_H_
#define _MSG_LIST_H_

#include <exception/Exception.h>

typedef struct _sstritem
{
	struct _sstritem*	pnext;
	char*				msg;
} SStrItem, *SStrItemPtr;

typedef struct _smsglist
{
	SElExceptionPtr (*Add)(struct _smsglist* me, char* msg);
	SElExceptionPtr (*Delete)(struct _smsglist* me, char* msg);
	SElExceptionPtr (*DeleteAll)(struct _smsglist* me);
	SElExceptionPtr (*GetMsg)(struct _smsglist* me, char* buffer, int32_t size);
	
	/* private */
	int32_t		_valid;
	int32_t		_allocSize;
	SStrItemPtr	_msgList;
	
} SMsgList, *SMsgListPtr;

SElExceptionPtr msglist_new(SMsgListPtr* msglist);
SElExceptionPtr msglist_delete(SMsgListPtr* msglist);

#endif /* _MSG_LIST_H_ */