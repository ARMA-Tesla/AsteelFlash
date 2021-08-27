/*
 *  "elMsg.c" - Message routines.
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
static char _ident[] = "@(#) elMsg.c 0.1 2007-03-05";
#else
#endif

#define EL_MSG_MODULE

#include <elBase.h>
#include <elTls.h>
#include <elMsg.h>
#include <elErr.h>
#include <elMem.h>
#include <elMtx.h>
#include <errno.h>

#define STDIO_THREAD_SAFE	0

static elErr_t *elMsgQueueCtor(elObjPtr_t *po,va_list args);
static elErr_t *elMsgQueueDtor(elObjPtr_t *po);
static elErr_t *elMsgCtor(elObjPtr_t *po,va_list args);
static elErr_t *elMsgDtor(elObjPtr_t *po);

static elObjClass_t gs_elMsgQueue_c = {
	sizeof(elMsgQueue_t),
	"elMsgQueue",
	elMsgQueueCtor,	/* Ctor()	*/
	elMsgQueueDtor,	/* Dtor()	*/
	NULL,			/* Clone()	*/
	NULL,			/* Differ()	*/
};

static elObjClass_t gs_elMsg_c = {
	sizeof(elMsg_t),
	"elMsg",
	elMsgCtor,		/* Ctor()	*/
	elMsgDtor,		/* Dtor()	*/
	NULL,			/* Clone()	*/
	NULL,			/* Differ()	*/
};

EL_DLL_EXPORT const elObjClass_t *elMsgQueue_c	= &gs_elMsgQueue_c;
EL_DLL_EXPORT const elObjClass_t *elMsg_c		= &gs_elMsg_c;

static struct {
	elMtx_t		lock;

	struct {
		TCHAR			*name;
		elMsgQueue_t	*ptr;
	} *mq;
	elUInt32_t	mq_cnt;

} gs;

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elMsgStartup"
EL_DLL_API elErr_t *elMsgStartup(void)
{
	EL_ERR_VAR;

	EL_ERR_CHECK( elMtxNew(&gs.lock,NULL) );

	;

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elMsgCleanup"
EL_DLL_API elErr_t *elMsgCleanup(void)
{
	EL_ERR_VAR;

	EL_ERR_CHECK( elMtxDelete(&gs.lock) );

	;

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elMsgQueueCtor"
static elErr_t *elMsgQueueCtor(elObjPtr_t *po,va_list args)
{
	EL_ERR_VAR;
	elMsgQueue_t *mq = (elMsgQueue_t *)*po;

	(void) args;

	mq->Open	= elMsgQueueOpen;
	mq->Close	= elMsgQueueClose;
	mq->Send	= elMsgQueueSend;
	mq->Recv	= elMsgQueueRecv;

#if 0
Error:
#endif

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elMsgQueueDtor"
static elErr_t *elMsgQueueDtor(elObjPtr_t *po)
{
	EL_ERR_VAR;
	elMsgQueue_t *mq = (elMsgQueue_t *)*po;

	(void) mq;

	;

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elMsgCtor"
static elErr_t *elMsgCtor(elObjPtr_t *po,va_list args)
{
	EL_ERR_VAR;
	elMsg_t *msg = (elMsg_t *)*po;

	(void) args;

	msg->Alloc		= elMsgAlloc;
	msg->Free		= elMsgFree;

#if 0
Error:
#endif

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elMsgDtor"
static elErr_t *elMsgDtor(elObjPtr_t *po)
{
	EL_ERR_VAR;
	elMsg_t *msg = (elMsg_t *)*po;

	(void) msg;

	;

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elMsgQueueOpen"
EL_DLL_API elErr_t *elMsgQueueOpen(
	elMsgQueue_t *mq,TCHAR *name,elUInt32_t msg_cnt_max
)
{
	EL_ERR_VAR;
	elUInt32_t	i;

	(void) name;
	(void) i;

	elMtxLock(&gs.lock);

#if 0
	for(i = 0; i < gs.mq_cnt; i++)
		if ( gs.mq[i].ptr && !_tcscmp(gs.mq[i].name,name) )
			break;
	if ( i < gs.mq_cnt )
	{
		EL_ERR_CHECK( elMemRealloc(gs.mq,(1+gs.mq_cnt) * sizeof(*gs.mq)) );
		gs.mq_cnt++;
	}
	mq = &gs.mq[i];
#endif

	;

	mq->msg_first = mq->msg_last = NULL;
	mq->msg_cnt = 0;
	mq->msg_cnt_max = msg_cnt_max;

	EL_ERR_CHECK( elMtxNew(&mq->lock,NULL) );

Error:

	elMtxUnlock(&gs.lock);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elMsgQueueClose"
EL_DLL_API elErr_t *elMsgQueueClose(elMsgQueue_t *mq)
{
	EL_ERR_VAR;

	elMtxLock(&gs.lock);

	mq->msg_first = mq->msg_last = NULL;
	mq->msg_cnt = 0;

	EL_ERR_CHECK( elMtxDelete(&mq->lock) );

	;

Error:

	elMtxUnlock(&gs.lock);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elMsgQueueSend"
EL_DLL_API elErr_t *elMsgQueueSend(
	elMsgQueue_t *mq,elMsg_t *msg,elUInt32_t timeout
)
{
	EL_ERR_VAR;
	elMsgQueueItem_t	*mqi;

	(void) mq;
	(void) msg;
	(void) timeout;

	EL_ERR_ASSERT( mq );
	
	elMtxLock(&mq->lock);

	if ( mq->msg_cnt < mq->msg_cnt_max )
	{
#if 0
		fprintf(stderr,"%s(): 10\n",__FUNC__);
#endif

		EL_ERR_CHECK( elMemAlloc(&mqi,sizeof(*mqi)) );
#if 0
		fprintf(stderr,"%s(): 12\n",__FUNC__);
#endif
		memset(mqi,0,sizeof(*mqi));
#if 0
		fprintf(stderr,"%s(): 15\n",__FUNC__);
#endif
		if ( !mq->msg_first )
			mq->msg_first = mqi;
#if 0
		fprintf(stderr,"%s(): 20\n",__FUNC__);
#endif
		if ( mq->msg_last )
		{
			mqi->prev = mq->msg_last;
#if 0
			fprintf(stderr,"%s(): 25\n",__FUNC__);
#endif
			mqi->prev->next = mqi;
		}
#if 0
		fprintf(stderr,"%s(): 30\n",__FUNC__);
#endif
		mq->msg_last = mqi;

#if 0
		fprintf(stderr,"%s(): 50\n",__FUNC__);
#endif

		mqi->next = NULL;
		mqi->msg = msg;

		mq->msg_cnt++;

#if 0
		fprintf(stderr,"%s(): 100\n",__FUNC__);
#endif
	}
	else
		EL_ERR_THROW(EL_ERR_INVAL,NULL,NULL);

Error:

	elMtxUnlock(&mq->lock);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elMsgQueueRecv"
EL_DLL_API elErr_t *elMsgQueueRecv(
	elMsgQueue_t *mq,elMsg_t **p_msg,elUInt32_t timeout
)
{
	EL_ERR_VAR;
	elMsgQueueItem_t	*mqi;
	elUInt32_t			msg_cnt;

	(void) timeout;

	do
	{
		elMtxLock(&mq->lock);
		msg_cnt = mq->msg_cnt;
		elMtxUnlock(&mq->lock);

		if ( !msg_cnt )
		{
			if ( !timeout )
				EL_ERR_THROW(EL_ERR_TIMEOUT,NULL,NULL);
#if 1
			Sleep(1);
#endif
		}

	} while( !msg_cnt );

	elMtxLock(&mq->lock);

	mqi = mq->msg_first;
	*p_msg = mqi->msg;
	mq->msg_first = mqi->next;
	elMemFree(&mqi);
	mq->msg_cnt--;
	if ( !mq->msg_cnt )
		mq->msg_last = NULL;

	elMtxUnlock(&mq->lock);

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elMsgAlloc"
EL_DLL_API elErr_t *elMsgAlloc(elMsg_t **p_msg,elUInt32_t msg_size)
{
	EL_ERR_VAR;

	EL_ERR_CHECK( elMemAlloc(p_msg,sizeof(**p_msg)) );
	memset(*p_msg,0,sizeof(**p_msg));
	EL_ERR_CHECK( elMemAlloc(&(*p_msg)->ptr,msg_size) );
	(*p_msg)->size = msg_size;

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elMsgFree"
EL_DLL_API elErr_t *elMsgFree(elMsg_t **p_msg)
{
	EL_ERR_VAR;

	elMemFree(&(*p_msg)->ptr);
	elMemFree(p_msg);

#if 0
Error:
#endif

	return err;
}

/******************************************************************************/
