/*
 *  "elIo.c" - I/O routines.
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
static char _ident[] = "@(#) elIo.c 0.1 2007-03-05";
#else
#endif

#include <elBase.h>
#include <elObj.h>
#include <elMem.h>
#include <elIo.h>
#include <elErr.h>
#include <elErrWin.h>

static elErr_t *elIoCtor(elObjPtr_t *po,va_list args);
static elErr_t *elIoDtor(elObjPtr_t *po);
static elErr_t *elIoTaskCtor(elObjPtr_t *po,va_list args);
static elErr_t *elIoTaskDtor(elObjPtr_t *po);

static elObjClass_t gs_elIo_c = {
	sizeof(elIo_t),
	"elIo",
	elIoCtor,		/* Ctor()	*/
	elIoDtor,		/* Dtor()	*/
	NULL,			/* Clone()	*/
	NULL,			/* Differ()	*/
};

const elObjClass_t *elIo_c = &gs_elIo_c;

static elObjClass_t gs_elIoTask_c = {
	sizeof(elIoTask_t),
	"elIo",
	elIoTaskCtor,	/* Ctor()	*/
	elIoTaskDtor,	/* Dtor()	*/
	NULL,			/* Clone()	*/
	NULL,			/* Differ()	*/
};

const elObjClass_t *elIoTask_c = &gs_elIoTask_c;

/*
 * 'gs_intfc[]': Supported I/O interfaces/modules.
 */
static const elIoIntfc_t *gs_intfc[] = {
	&g_elIoSerialIntfc,
	&g_elIoRfidIntfc,
	&g_elIoNetrxRfidIntfc,
	&g_elIoPromagRfidIntfc,
	&g_elIoElatecRfidIntfc,
	&g_elIoNiDaqIntfc,
#if 0
	&g_elIoWagoMbtIntfc,
#endif
	&g_elIoModbusIntfc,
};

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoCtor"
static elErr_t *elIoCtor(elObjPtr_t *po,va_list args)
{
	EL_ERR_VAR;
	elIo_t *io = (elIo_t *)*po;

	(void) args;

	EL_ERR_CHECK( elNew(elLog_c,&io->log) );

	EL_ERR_CHECK( elNew(elStr_c,&io->str.rsrc) );
	EL_ERR_CHECK( elNew(elStr_c,&io->str.intfc) );
	EL_ERR_CHECK( elNew(elStr_c,&io->str.path) );
	EL_ERR_CHECK( elNew(elStr_c,&io->str.attrs) );
	EL_ERR_CHECK( elNew(elStr_c,&io->str.name) );

	io->Open		= elIoOpen;
	io->Close		= elIoClose;
	io->Read		= elIoRead;
	io->Write		= elIoWrite;
	io->MsgRead		= elIoMsgRead;
	io->MsgWrite	= elIoMsgWrite;
	io->Flush		= elIoFlush;
	io->Abort		= elIoAbort;
	io->AbortReset	= elIoAbortReset;
	io->Ctrl		= elIoCtrl;
	io->AttrSet		= elIoAttrSet;
	io->AttrGet		= elIoAttrGet;
	io->Lock		= elIoLock;
	io->Unlock		= elIoUnlock;
	io->DevLock		= elIoDevLock;
	io->DevUnlock	= elIoDevUnlock;
	io->DebugLog	= elIoDebugLog;

	if ( !io->timeout )
		io->timeout		= 3000;		/* default I/O timeout: 3000 ms = 3 s */

	io->poll = FALSE;

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoDtor"
static elErr_t *elIoDtor(elObjPtr_t *po)
{
	EL_ERR_VAR;
	elIo_t *io = (elIo_t *)*po;

	elDelete(&io->str.rsrc);
	elDelete(&io->str.intfc);
	elDelete(&io->str.path);
	elDelete(&io->str.attrs);
	elDelete(&io->str.name);
	io->name = NULL;

	elDelete(&io->log);

	elMemFree(&io->err);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoTaskCtor"
static elErr_t *elIoTaskCtor(elObjPtr_t *po,va_list args)
{
	EL_ERR_VAR;
	elIoTask_t *task = (elIoTask_t *) *po;

	(void) args;

	task->Read		= elIoTaskRead;
	task->Write		= elIoTaskWrite;
	task->Execute	= elIoTaskExecute;

#if 0
Error:
#endif

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoTaskDtor"
static elErr_t *elIoTaskDtor(elObjPtr_t *po)
{
	EL_ERR_VAR;
	elIoTask_t		*task = (elIoTask_t *) *po;
	elIoTaskRW_t	*task_rw;
	elIoTaskIntfc_t	*intfc;
	elIoTaskDev_t	*tdev;
	register int	i, j, irw;

	elMemFree(&task->io);
	task->io_cnt = 0;

	for(irw = 0; irw < EL_ARRAY_CNT(task->rw); irw++)
	{
		task_rw = &task->rw[irw];

		for(i = 0; i < task_rw->intfc_cnt; i++)
		{
			intfc = &task_rw->intfc[i];
			for(j = 0; j < intfc->dev_cnt; j++)
			{
				tdev = &intfc->dev[j];
				elMemFree(&tdev->io);
				tdev->io_cnt = 0;
				elMemFree(&tdev->name);
				elMemFree(&tdev->data);
				elIoBufFree(&tdev->buf);
			}
			elMemFree(&intfc->dev);
		}
		elMemFree(&task_rw->intfc);
		task_rw->intfc_cnt = 0;
	}

	EL_ERR_CHECK( elIoBufFree(&task->buf) );
	task->buf_size = 0;

	if ( task->Delete )
		task->Delete(task);

Error:

	return err;
}

/******************************************************************************/
/*
 * 'elIoBufInit()':
 *
 * Initializes I/O buffer.
 */
#undef	__FUNC__
#define	__FUNC__	"elIoBufInit"
elErr_t *elIoBufInit(
	elIoBuf_t *iobuf			/* [IN]  I/O buffer (cache) */
)
{
	EL_ERR_VAR;

	memset(iobuf,0,sizeof(*iobuf));

	iobuf->item_size.bytes	= 1;
	iobuf->item_size.bits	= 8;
	iobuf->item_size.bpb	= 8;

	return err;
}

/******************************************************************************/
/*
 * 'elIoBufClear()':
 *
 * Clears I/O buffer.
 */
#undef	__FUNC__
#define	__FUNC__	"elIoBufClear"
elErr_t *elIoBufClear(
	elIoBuf_t *iobuf			/* [IN]  I/O buffer (cache) */
)
{
	EL_ERR_VAR;

	iobuf->ptr = iobuf->base;
	iobuf->len = 0;

	return err;
}

/******************************************************************************/
/*
 * 'elIoBufAlloc()':
 *
 * Allocated I/O buffer.
 */
#undef	__FUNC__
#define	__FUNC__	"elIoBufAlloc"
elErr_t *elIoBufAlloc(
	elIoBuf_t	*iobuf,		/* [IN]  I/O buffer (cache) */
	elUInt32_t	size		/* [IN]  number of bytes to (re)allocate */
)
{
	EL_ERR_VAR;

#if WIN
	/*
	 * Allocate I/O buffer on sector size boundary for non-cached I/O:
	 *
	 * Otherwise, the 'ReadFile()' returns 'ERROR_INVALID_PARAMETER'.
	 */
	elIoBufFree(iobuf);
	iobuf->base = VirtualAlloc(NULL,size,MEM_COMMIT,PAGE_READWRITE);
	EL_ERR_CHECK_ALLOC( iobuf->base,size );
#else
	EL_ERR_CHECK( elMemReAlloc(&iobuf->base,size) );
#endif

	memset(iobuf->base,0,size);

	iobuf->size = size;
	iobuf->ptr = iobuf->base;

Error:

	return err;
}

/******************************************************************************/
/*
 * 'elIoBufFree()':
 *
 * Releases allocated I/O buffer.
 */
#undef	__FUNC__
#define	__FUNC__	"elIoBufFree"
elErr_t *elIoBufFree(
	elIoBuf_t *iobuf		/* [IN]  I/O buffer (cache) */
)
{
	EL_ERR_VAR;

	if ( iobuf->base )
	{
#if WIN
		VirtualFree(iobuf->base,iobuf->size,MEM_DECOMMIT);
#else
		elMemFree(&iobuf->base);
#endif
		iobuf->base = iobuf->ptr = NULL;
	}

#if 0
Error:
#endif

	return err;
}

/******************************************************************************/
/*
 * 'elIoBufItemSet()':
 *
 * Stores given I/O item to I/O buffer.
 */
#undef	__FUNC__
#define	__FUNC__	"elIoBufItemSet"
void elIoBufItemSet(
	elIoBuf_t	*iobuf,		/* [IN]  I/O buffer (cache) */
	elByte_t	*pitem		/* [IN]  pointer to I/O item */
)
{
	if ( iobuf->item_size.bpb == 1 )
	{
		register uint	i;

		for(i = 0; i < iobuf->item_size.bits; i++)
		{
			if ( i && !(i % 8) )
				pitem++;
			iobuf->ptr[i] = (elByte_t) ((*pitem >> (i % 8)) & 1);
		}
	}
	else
	{
#if 1
		elLog("%s(): iobuf->item_size.bpb (%d) != 1: not yet supported",
			__FUNC__,iobuf->item_size.bpb);
#else
		register int	bit, bit_cnt, bit_cnt_total;
		register byte	*buf;

		bit = iobuf->ptr_bit;
		for(buf = iobuf->ptr, bit_cnt_total = iobuf->item_size.bits; bit_cnt_total; buf++)
		{
			bit_cnt = 8 - bit;
			if ( bit_cnt_total < bit_cnt )
				bit_cnt = bit_cnt_total;

			if ( bit || bit_cnt != 8 )
			{
				*buf = (elByte_t) (
					(*buf & ~(0xFF << bit)) | ((*pitem & ~(1 << bit_cnt)) << bit)
				);
			}
			else
				*buf = *pitem;

			buf++;
			bit_cnt_total -= bit_cnt;
		}
#endif
	}
}

/******************************************************************************/
/*
 * 'elIoBufItemGetBit()':
 *
 * Retrieves specified bit of I/O item from I/O buffer.
 */
#undef	__FUNC__
#define	__FUNC__	"elIoBufItemGetBit"
bool elIoBufItemGetBit(
	register elIoBuf_t	*iobuf,		/* [IN]  I/O buffer (cache) */
	register elInt_t	bit			/* [IN]  bit index of I/O item */
)
{
	if ( iobuf->item_size.bpb == 1 )
		return iobuf->ptr[bit];
	else
	{
		bit += iobuf->ptr_bit;
		return (bool) ((iobuf->ptr[bit/8] & (1 << (bit % 8))) ? 1 : 0);
	}
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoRsrcParse"
elErr_t *elIoRsrcParse(TCHAR *rsrc,elStr_t *intfc,elStr_t *path,elStr_t *attrs)
{
	EL_ERR_VAR;

	(void) rsrc;
	(void) intfc;
	(void) path;
	(void) attrs;

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoOpen"
elErr_t *elIoOpen(elIoParam_t iop,TCHAR *rsrc,...)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elStr_t			*s;
	TCHAR			*p;
	TCHAR			*str_intfc;
	size_t			len;
	register int	i;

	EL_ERR_CHECK( elNew(elStr_c,&s) );

	/*
	 * I/O resource:
	 *
	 *		interface:path[,attrs]
	 *
	 * For example:
	 *
	 *		NI-VISA:TCPIP::192.168.5.250::23::SOCKET,TCPIP_KEEPALIVE=TRUE
	 */
	EL_ERR_CHECK( elStrCpy(io->str.rsrc,rsrc) );
	p = _tcschr(rsrc,_T(':'));
	EL_ERR_ASSERT( p );
	len = p - rsrc;
	EL_ERR_CHECK( elStrNCpy(io->str.intfc,rsrc,len) );
	str_intfc = io->str.intfc->ptr;
	str_intfc[len] = _T('\0');

	for(i = 0; i < EL_ARRAY_CNT(gs_intfc); i++)
		if ( !_tcscmp(gs_intfc[i]->name,str_intfc) )
			break;
	if ( i == EL_ARRAY_CNT(gs_intfc) )
		EL_ERR_THROW(
			EL_ERR_INVAL,NULL,
			elStrTmpFmt(_T("%s: unknown I/O interface"),str_intfc)
		);

	EL_ERR_CHECK( elStrCpy(io->str.path,p+1) );

	io->intfc = gs_intfc[i];

	elIoBufInit(&io->buf);
	EL_ERR_CHECK( elIoBufAlloc(&io->buf,1024) );

	elIoBufInit(&io->msg.buf);
	EL_ERR_CHECK( elIoBufAlloc(&io->msg.buf,1024) );
	io->msg.term_char = _T('\n');

	if ( io->debug )
	{
		io->DebugLog(io,1,_T("%s(): io->str:\n"),_T(__FUNC__));
		io->DebugLog(io,1,_T("  rsrc:  '%s'\n"),elStrPtr(io->str.rsrc));
		io->DebugLog(io,1,_T("  intfc: '%s'\n"),elStrPtr(io->str.intfc));
		io->DebugLog(io,1,_T("  path:  '%s'\n"),elStrPtr(io->str.path));
		io->DebugLog(io,1,_T("  attrs: '%s'\n"),elStrPtr(io->str.attrs));
	}

	io->attrs = EL_IO_INPUT | EL_IO_OUTPUT;

	EL_ERR_CHECK_WIN(
		io->abort.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL),
		EL_ERR_IO,NULL
	);

	if ( io->intfc->Open )
		EL_ERR_CHECK( io->intfc->Open(io,rsrc) );

	if ( !io->str.name->ptr || !*io->str.name->ptr )
		elStrFmt(io->str.name,"%s:%s",io->str.intfc->ptr,io->str.path->ptr);
	io->name = io->str.name->ptr;

	io->opened = TRUE;

	EL_ERR_CHECK( elMtxNew(&io->lock,NULL) );

Error:

	elDelete(&s);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoClose"
elErr_t *elIoClose(elIoParam_t iop)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	register int	i;

	if ( io->intfc )
		EL_ERR_CHECK( io->intfc->Close(io) );

	EL_ERR_CHECK_WIN(
		CloseHandle(io->abort.hEvent),
		EL_ERR_IO,
		elStrTmpFmt("%s: CloseHandle(0x%lX): %s",
			io->name,io->abort.hEvent,elErrGetLastErrorStr())
	);

	elIoBufFree(&io->buf);
	elIoBufFree(&io->msg.buf);

	for(i = 0; i < io->attr.cnt; i++)
	{
		elMemFree(&io->attr.item[i].name);
		elMemFree(&io->attr.item[i].val);
	}
	elMemFree(&io->attr.item);

	EL_ERR_CHECK( elMtxDelete(&io->lock) );

	io->opened = FALSE;

	;

Error:
	
	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoRead"
elErr_t *elIoRead(
	elIoParam_t iop,elByte_t *buf,elUInt32_t cnt,elUInt32_t *p_cnt
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;

	EL_IO_ABORT_CHECK(io);

	if ( io->debug )
	{
		io->DebugLog(io,1,_T("%s():\n"),_T(__FUNC__));
		io->DebugLog(io,1,_T("  buf: 0x%lX\n"),buf);
	}

	if ( io->intfc && io->intfc->Read )
		err = io->intfc->Read(io,buf,cnt,p_cnt);

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoWrite"
elErr_t *elIoWrite(
	elIoParam_t iop,elByte_t *buf,elUInt32_t cnt,elUInt32_t *p_cnt
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;

	EL_IO_ABORT_CHECK(io);

	if ( io->intfc && io->intfc->Write )
		err = io->intfc->Write(io,buf,cnt,p_cnt);

#if 0
#if 0
	EL_ERR_CHECK_WIN(
		WriteFile(io->dev->h,buf,bufsize,&len,NULL),
		EL_ERR_IO,
		NULL
	);
#else
	len = bufsize;
#endif
	EL_ERR_CHECK_COND(
		len != bufsize,
		EL_ERR_IO,
		elStrTmpFmt(_T("%d/%ld"),len,bufsize)
	);
#endif

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoMsgRead"
elErr_t *elIoMsgRead(
	elIoParam_t iop,elByte_t *msg,elUInt32_t msgsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;

	if ( io->debug )
	{
		io->DebugLog(io,1,_T("%s():\n"),_T(__FUNC__));
		io->DebugLog(io,1,_T("  msg: 0x%lX\n"),msg);
	}

	*plen = 0;
	
	if ( io->intfc && io->intfc->MsgRead )
		err = io->intfc->MsgRead(io,msg,msgsize,plen);

#if 0
Error:
#endif

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoMsgWrite"
elErr_t *elIoMsgWrite(
	elIoParam_t iop,elByte_t *msg,elUInt32_t msgsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;

	if ( io->intfc && io->intfc->MsgWrite )
		err = io->intfc->MsgWrite(io,msg,msgsize,plen);

#if 0
#if 0
	EL_ERR_CHECK_WIN(
		WriteFile(io->dev->h,msg,msgsize,&len,NULL),
		EL_ERR_IO,
		NULL
	);
#else
	len = msgsize;
#endif
	EL_ERR_CHECK_COND(
		len != msgsize,
		EL_ERR_IO,
		elStrTmpFmt(_T("%d/%ld"),len,msgsize)
	);
#endif

#if 0
Error:
#endif

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoFlush"
elErr_t *elIoFlush(elIoParam_t iop,elUInt32_t io_mode)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;

	if ( io_mode & EL_IO_INPUT )
		EL_ERR_CHECK( io->Ctrl(io,EL_IO_CTRL_DISCARD_INPUT,NULL) );

	if ( io_mode & EL_IO_OUTPUT )
		EL_ERR_CHECK( io->Ctrl(io,EL_IO_CTRL_FLUSH_OUTPUT,NULL) );

	if ( io->intfc && io->intfc->Flush)
		err = io->intfc->Flush(io,io_mode);

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoAbort"
elErr_t *elIoAbort(elIoParam_t iop)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;

#if 0
	elLogD("%s(): io->abort.set = TRUE",__FUNC__);
#endif
	io->abort.set = TRUE;

	/* send signal to a thread that is waiting for I/O to abort the I/O: */
	EL_ERR_CHECK_WIN(
		SetEvent(io->abort.hEvent),
		EL_ERR_IO,
		elStrTmpFmt("SetEvent(abort.hEvent): %s",elErrGetLastErrorStr())
	);

	if ( io->intfc && io->intfc->Abort )
		err = io->intfc->Abort(io);

Error:

	return err;
}

/******************************************************************************/
/*
 * 'elIoAbortReset()':
 */
/** Reset/Acknowledge aborted I/O operation
 */
elErr_t *elIoAbortReset(
	elIoParam_t iop		/*!< I/O descriptor */
)
#undef	__FUNC__
#define	__FUNC__	"elIoAbortReset"
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;

	if ( !ResetEvent(io->abort.hEvent) )
		elLog("%s(): ResetEvent(abort.hEvent): %W",__FUNC__);

#if 0
	elLogD("%s(): io->abort.set = FALSE",__FUNC__);
#endif
	io->abort.set = FALSE;

	if ( io->intfc && io->intfc->AbortReset )
		err = io->intfc->AbortReset(io);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoCtrl"
elErr_t *elIoCtrl(elIoParam_t iop,int cmd,void *pdata)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;

	if ( io->intfc && io->intfc->Ctrl )
		err = io->intfc->Ctrl(io,cmd,pdata);

#if 0
Error:
#endif

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"attr_val_str2val"
static elErr_t *attr_val_str2val(char *val_str,uint32_t *p_val)
{
	EL_ERR_VAR;
	int	ival;

	if ( isdigit(*val_str) )
	{
		sscanf(val_str,"%i",&ival);
		*p_val = ival;
	}
	else if ( !stricmp(val_str,"TRUE") || !stricmp(val_str,"YES") )
		*p_val = TRUE;
	else
	{
		EL_ERR_THROW(
			EL_ERR,NULL,elStrTmpFmt("%s: invalid attribute value",val_str));
	}

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoAttrSet"
elErr_t *elIoAttrSet(elIoParam_t iop,TCHAR *attr_name,TCHAR *attr_val)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	uint32_t	attr_value;

#if 1
	if ( attr_name )
	{
		elIoAttrItem_t	*a;
		register int	i;

		/* see if attribute exists: */
		for(i = 0; i < io->attr.cnt; i++)
			if ( io->attr.item[i].name
				&& !stricmp(io->attr.item[i].name,attr_name) )
				break;
		if ( i < io->attr.cnt )
		{	/* attribute exists: */
			a = &io->attr.item[i]; 
		}
		else
		{	/* attribute does NOT exist: */
			/* find empty slot: */
			for(i = 0; i < io->attr.cnt; i++)
				if ( !io->attr.item[i].name )
					break;
			if ( i < io->attr.cnt )
				a = &io->attr.item[i];	/* empty slot */
			else
			{	/* no empty slot, allocate new one: */
				EL_ERR_CHECK(
					elMemRealloc(
						&io->attr.item,(1 + io->attr.cnt) * sizeof(*io->attr.item)
					)
				);
				a = &io->attr.item[io->attr.cnt++];
				memset(a,0,sizeof(*a));
			}
			EL_ERR_CHECK( elMemStrdup(&a->name,attr_name) );
		}
		elMemFree(&a->val);
		EL_ERR_CHECK( elMemStrdup(&a->val,attr_val) );
	}
#endif

	if ( !stricmp(attr_name,"MSG_TERM_CHAR") )
	{
		EL_ERR_CHECK( attr_val_str2val(attr_val,&attr_value) );
		io->msg.term_char = (int) attr_value;
	}
	else if ( !stricmp(attr_name,"POLL") )
	{
		EL_ERR_CHECK( attr_val_str2val(attr_val,&attr_value) );
		io->poll = (bool) attr_value;
	}
	else if ( !stricmp(attr_name,"ASYNC") )
	{
		EL_ERR_CHECK( attr_val_str2val(attr_val,&attr_value) );
		io->async.enabled = (bool) attr_value;
	}
	else if ( !stricmp(attr_name,"xxx") )
	{
		;
	}
	else
	{
		;
	}

	if ( io->intfc && io->intfc->AttrSet )
		err = io->intfc->AttrSet(io,attr_name,attr_val);

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoAttrGet"
elErr_t *elIoAttrGet(elIoParam_t iop,TCHAR *attr_name,TCHAR **p_attr_val)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;

#if 1
	*p_attr_val = NULL;
	if ( attr_name )
	{
		register int	i;

		for(i = 0; i < io->attr.cnt; i++)
		{
			if ( io->attr.item[i].name
				&& !stricmp(io->attr.item[i].name,attr_name) )
			{
				*p_attr_val = io->attr.item[i].val;
				break;
			}
		}
	}
#else
	if ( !stricmp(attr,"MSG_TERM_CHAR") )
	{
		*(char *)pval = (char) io->msg.term_char;
	}
	else if ( !stricmp(attr,"POLL") )
	{
		*(bool *)pval = io->poll;
	}
	else if ( !stricmp(attr,"ASYNC") )
	{
		*(bool *)pval = io->async.enabled;
	}
	else if ( !stricmp(attr,"xxx") )
	{
		;
	}
	else
	{
		;
	}
#endif

	if ( io->intfc && io->intfc->AttrGet )
		err = io->intfc->AttrGet(io,attr_name,p_attr_val);

#if 0
Error:
#endif

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoLock"
elErr_t *elIoLock(elIoParam_t iop)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;

	EL_ERR_CHECK( elMtxLock(&io->lock) );

	if ( io->intfc && io->intfc->Lock )
		err = io->intfc->Lock(io);

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoUnlock"
elErr_t *elIoUnlock(elIoParam_t iop)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;

	if ( io->intfc && io->intfc->Unlock )
		err = io->intfc->Unlock(io);

	EL_ERR_CHECK( elMtxUnlock(&io->lock) );

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoDevLock"
elErr_t *elIoDevLock(elIoParam_t iop)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;

	if ( io->intfc && io->intfc->DevLock )
		EL_ERR_CHECK( io->intfc->DevLock(io) );

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoDevUnlock"
elErr_t *elIoDevUnlock(elIoParam_t iop)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;

	if ( io->intfc && io->intfc->DevUnlock )
		EL_ERR_CHECK( io->intfc->DevUnlock(io) );

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoDebugLog"
elErr_t *elIoDebugLog(elIoParam_t iop,int debug_level_min,const TCHAR *fmt,...)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	va_list args;

	if ( io->debug >= debug_level_min )
	{
		va_start(args,fmt);
		io->log->PrintfV(io->log,fmt,args);
		va_end(args);
	}

	return err;
}

#if 0
/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoTaskStartup"
static elErr_t *elIoTaskStartup(elIoTask_t *task)
{
	EL_ERR_VAR;
	size_t			size;
	register int	i, j;

	if ( !task->intfc_cnt )
	{
		for(i = 0; i < task->io_cnt; i++)
		{
			for(j = 0; j < task->intfc_cnt; j++)
				if ( task->intfc[j] == task->io[i]->intfc )
					break;
			if ( j == task->intfc_cnt )
			{
				size = (1 + task->intfc_cnt) * sizeof(*task->intfc);
				EL_ERR_CHECK( elMemRealloc((void *)&task->intfc,size) );
				task->intfc[j].ptr = task->io[i]->intfc;
				task->intfc_cnt++;
			}
		}
	}

Error:

	return err;
}
#endif

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoTaskRead"
elErr_t *elIoTaskRead(
	elIoTask_t *task,
	elIoParam_t iop,elByte_t *buf,elUInt32_t cnt,elUInt32_t *p_cnt
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoTaskRW_t	*task_rw;
	size_t			size;
	register int	i;

	/*
	 * Add I/O to the task, if not present yet:
	 */
	for(i = 0; i < task->io_cnt; i++)
		if ( task->io[i] == io )
			break;
	if ( i == task->io_cnt )
	{
		size = (1 + task->io_cnt) * sizeof(*task->io);
		EL_ERR_CHECK( elMemRealloc(&task->io,size) );
		task->io[i] = io;
		task->io_cnt++;
	}

	task->buf_size += cnt;
	EL_ERR_CHECK( elIoBufAlloc(&task->buf,task->buf_size) );

	task_rw = &task->rw[EL_IO_TASK_READ];

	/*
	 * Add I/O interface to the task, if not present yet:
	 */
	for(i = 0; i < task_rw->intfc_cnt; i++)
		if ( task_rw->intfc[i].ptr == io->intfc )
			break;
	if ( i == task_rw->intfc_cnt )
	{
		size = (1 + task_rw->intfc_cnt) * sizeof(*task_rw->intfc);
		EL_ERR_CHECK( elMemRealloc((void *)&task_rw->intfc,size) );
		memset(&task_rw->intfc[i],0,sizeof(task_rw->intfc[i]));
		task_rw->intfc[i].ptr = io->intfc;
		task_rw->intfc_cnt++;
	}

	/* interface-specific processing/initialization: */
	if ( io->intfc->task.Read )
		EL_ERR_CHECK( io->intfc->task.Read(task,io,buf,cnt,p_cnt) );

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoTaskWrite"
elErr_t *elIoTaskWrite(
	elIoTask_t *task,
	elIoParam_t iop,elByte_t *buf,elUInt32_t cnt,elUInt32_t *p_cnt
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoTaskRW_t	*task_rw;
	size_t			size;
	register int	i;

	/*
	 * Add I/O to the task, if not present yet:
	 */
	for(i = 0; i < task->io_cnt; i++)
		if ( task->io[i] == io )
			break;
	if ( i == task->io_cnt )
	{
		size = (1 + task->io_cnt) * sizeof(*task->io);
		EL_ERR_CHECK( elMemRealloc(&task->io,size) );
		task->io[i] = io;
		task->io_cnt++;
	}

	task->buf_size += cnt;
	EL_ERR_CHECK( elIoBufAlloc(&task->buf,task->buf_size) );

	task_rw = &task->rw[EL_IO_TASK_WRITE];

	/*
	 * Add I/O interface to the task, if not present yet:
	 */
	for(i = 0; i < task_rw->intfc_cnt; i++)
		if ( task_rw->intfc[i].ptr == io->intfc )
			break;
	if ( i == task_rw->intfc_cnt )
	{
		size = (1 + task_rw->intfc_cnt) * sizeof(*task_rw->intfc);
		EL_ERR_CHECK( elMemRealloc((void *)&task_rw->intfc,size) );
		memset(&task_rw->intfc[i],0,sizeof(task_rw->intfc[i]));
		task_rw->intfc[i].ptr = io->intfc;
		task_rw->intfc_cnt++;
	}

	/* interface-specific processing/initialization: */
	if ( io->intfc->task.Write )
		EL_ERR_CHECK( io->intfc->task.Write(task,io,buf,cnt,p_cnt) );

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoTaskExecute"
elErr_t *elIoTaskExecute(elIoTask_t *task)
{
	EL_ERR_VAR;
	elIoTaskRW_t	*task_rw;
	elIoTaskIntfc_t	*intfc;
	register int	i, irw;

#if 0
	elIoTaskStartup(task);
#endif
	
	for(irw = 0; irw < EL_ARRAY_CNT(task->rw); irw++)
	{
		task_rw = &task->rw[irw];

		for(i = 0; i < task_rw->intfc_cnt; i++)
		{
			intfc = &task_rw->intfc[i];
			elTimerStart(&intfc->t_val);
			intfc->t_stamp = intfc->t_val;
			task->fn = EL_IO_TASK_FN_READ + irw;
			err = intfc->ptr->task.Execute(task);
			elTimerStop(&intfc->t_val);
			if ( err )
				err->Log(err,NULL);
		}
	}

	return err;
}

/******************************************************************************/
