/*
 *  <elMsg.h> - Definitions for module "elMsg.c".
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

#if _MSC_VER >= 1000
#pragma once
#endif 

/* forward reference: */
struct SelMsg;
typedef struct SelMsg elMsg_t, *elPMsg_t, *elMsgPtr_t;
struct SelMsgQueue;
typedef struct SelMsgQueue elMsgQueue_t, *elPMsgQueue_t, *elMsgQueuePtr_t;
struct SelMsgQueueItem;
typedef struct SelMsgQueueItem elMsgQueueItem_t, *elPMsgQueueItem_t, *elMsgQueueItemPtr_t;

#if !defined(_EL_MSG_H)
#define _EL_MSG_H

#if _MSC_VER < 1400 && !__BCOPT__ && !_CVI_  /* Borland C/C++: Unknown preprocessor directive: 'ident' */
#if 1
#ident "@(#) elMsg.h 0.1 2007-03-05"
#else
#endif
#endif

#include <elBase.h>
#include <elObj.h>
#include <elMtx.h>

/* forward reference: */
struct SelObj;
typedef struct SelObj elObj_t, *elPObj_t, *elObjPtr_t;

#define EL_MSG_HDR													\
		elObj_t		obj;											\
		void		*ptr;											\
		elUInt32_t	size;											\
		/* methods: */												\
		elErr_t		*(*Alloc)(elMsg_t **p_msg,elUInt32_t msg_size);	\
		elErr_t		*(*Free)(elMsg_t **p_msg)						\

typedef struct SelMsgHdr {
	EL_MSG_HDR;
} elMsgHdr_t;

/*
 * 'elMsg_t': Message data type.
 */
typedef struct SelMsg {
	EL_MSG_HDR;
} elMsg_t;

/*
 * 'elMsgQueueItem_t': Message Queue Item data type.
 */
typedef struct SelMsgQueueItem {
	/* doubly linked list: */
	elMsgQueueItem_t	*next;
	elMsgQueueItem_t	*prev;

	/* message itself: */
	elMsg_t				*msg;
} elMsgQueueItem_t;

typedef struct SelMsgQueue {
	elObj_t				obj;

	elMtx_t				lock;

	elMsgQueueItem_t	*msg_first;
	elMsgQueueItem_t	*msg_last;
	elUInt32_t			msg_cnt;
	elUInt32_t			msg_cnt_max;

	/* methods: */
	elErr_t		*(*Open)(elMsgQueue_t *mq,TCHAR *name,elUInt32_t msg_cnt_max);
	elErr_t		*(*Close)(elMsgQueue_t *mq);
	elErr_t		*(*Send)(elMsgQueue_t *mq,elMsg_t *msg,elUInt32_t timeout);
	elErr_t		*(*Recv)(elMsgQueue_t *mq,elMsg_t **p_msg,elUInt32_t timeout);
} elMsgQueue_t;

#if !defined(EL_MSG_MODULE)
EL_DLL_IMPORT extern const elObjClass_t *elMsgQueue_c;
EL_DLL_IMPORT extern const elObjClass_t *elMsg_c;
#endif

#ifdef __cplusplus
extern "C" {			/* C Plus Plus function bindings */
#endif

EL_DLL_API elErr_t *elMsgStartup(void);
EL_DLL_API elErr_t *elMsgCleanup(void);
EL_DLL_API elErr_t *elMsgQueueOpen(
	elMsgQueue_t *mq,TCHAR *name,elUInt32_t msg_cnt_max
);
EL_DLL_API elErr_t *elMsgQueueClose(elMsgQueue_t *mq);
EL_DLL_API elErr_t *elMsgQueueSend(
	elMsgQueue_t *mq,elMsg_t *msg,elUInt32_t timeout
);
EL_DLL_API elErr_t *elMsgQueueRecv(
	elMsgQueue_t *mq,elMsg_t **p_msg,elUInt32_t timeout
);
EL_DLL_API elErr_t *elMsgAlloc(elMsg_t **p_msg,elUInt32_t msg_size);
EL_DLL_API elErr_t *elMsgFree(elMsg_t **p_msg);

#ifdef __cplusplus
}						/* C Plus Plus function bindings */
#endif

#endif 	/* _EL_MSG_H */
