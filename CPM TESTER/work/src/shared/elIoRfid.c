/*
 *  "elIoRfid.c" - Generic RFID I/O routines.
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
static char _ident[] = "@(#) elIoRfid.c 0.1 2007-03-05";
#else
#endif

#include <elBase.h>
#include <elObj.h>
#include <elMem.h>
#include <elIo.h>
#include <elErr.h>
#include <elErrWin.h>

#define IO_OVERLAPPED	1

static elErr_t *elIoRfidCtor(elObjPtr_t *po,va_list args);
static elErr_t *elIoRfidDtor(elObjPtr_t *po);

static elObjClass_t gs_elIoRfid_c = {
	sizeof(elIoRfid_t),
	"elIoRfid",
	elIoRfidCtor,		/* Ctor()	*/
	elIoRfidDtor,		/* Dtor()	*/
	NULL,				/* Clone()	*/
	NULL,				/* Differ()	*/
};

const elObjClass_t *elIoRfid_c = &gs_elIoRfid_c;

#define EL_IO_RFID_METHODS									\
		elIoRfidOpen,				/* Open()			*/	\
		elIoRfidClose,				/* Close()			*/	\
		elIoRfidRead,				/* Read()			*/	\
		elIoRfidWrite,				/* Write()			*/	\
		elIoRfidMsgRead,			/* MsgRead()		*/	\
		elIoRfidMsgWrite,			/* MsgWrite()		*/	\
		NULL,						/* Flush()			*/	\
		NULL,						/* Abort()			*/	\
		NULL,						/* AbortReset()		*/	\
		elIoRfidCtrl,				/* Ctrl()			*/	\
		elIoRfidAttrSet,			/* AttrSet()		*/	\
		elIoRfidAttrGet,			/* AttrGet()		*/	\
		NULL,						/* Lock()			*/	\
		NULL,						/* Unlock()			*/	\
		NULL,						/* DevLock()		*/	\
		NULL,						/* DevUnlock()		*/	\
		NULL,						/* DebugLog()		*/	\
		elIoRfidTaskRead,			/* TaskRead()		*/	\
		elIoRfidTaskWrite,			/* TaskWrite()		*/	\
		elIoRfidTaskExecute			/* TaskExecute()	*/	\

const elIoIntfc_t g_elIoRfidIntfc = {
	_T("RFID"),		EL_IO_RFID_METHODS
};
const elIoIntfc_t *elIoRfidIntfc = &g_elIoRfidIntfc;

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoRfidCtor"
static elErr_t *elIoRfidCtor(elObjPtr_t *po,va_list args)
{
	EL_ERR_VAR;
	elIo_t *io = (elIo_t *)*po;

	(void) args;
	(void) io;

	;

#if 0
Error:
#endif

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoRfidDtor"
static elErr_t *elIoRfidDtor(elObjPtr_t *po)
{
	EL_ERR_VAR;
	elIo_t		*io = (elIo_t *)*po;

	(void) io;

	;

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoRfidOpen"
elErr_t *elIoRfidOpen(elIoParam_t iop,TCHAR *rsrc,...)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoRfid_t		*dio = NULL;
	elIo_t			*rio = NULL;
	elStr_t			*s;

	(void) rsrc;
	(void) dio;

	EL_ERR_CHECK( elNew(elIoRfid_c,&io->dio) );
	EL_ERR_CHECK( elNew(elStr_c,&s) );

	dio = io->dio;
	dio->io = io;

	/*
	 * Open RFID device using device-specific RFID I/O module:
	 *
	 *	RFID:<RFID-interface>:<serial-device>  =>  <RFID-interface>:<serial-device>
	 *
	 * For example:
	 *
	 *	RFID:NETRX-RFID:COM1/115200/8/n/1/n  =>  NETRX-RFID:COM1/115200/8/n/1/n
	 */
	EL_ERR_CHECK( elNew(elIo_c,&dio->rio) );
	rio = dio->rio;
	EL_ERR_CHECK( s->Cpy(s,io->str.path->ptr) );
	EL_ERR_CHECK( rio->Open(rio,s->ptr) );
	/*
	 * Set device-specific RFID I/O to return messages
	 * (read by 'elIoMsgRead()') in generic RFID format ('elIoRfidMsg_t'):
	 */
	EL_ERR_CHECK( rio->AttrSet(rio,"MSG_FMT","GENERIC") );

	elStrCpy(io->str.path,rio->str.path->ptr);
	elStrCpy(io->str.attrs,rio->str.attrs->ptr);

	io->item_size.bits	= 8;
	io->item_size.bytes	= 1;
	io->item_size.bpb	= 8;

	EL_ERR_CHECK( elIoBufAlloc(&io->msg.buf,1024) );

Error:

	if ( err )
	{
		elIoBufFree(&io->msg.buf);
		elDelete(&dio->rio);
		elDelete(&io->dio);
	}

	elDelete(&s);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoRfidClose"
elErr_t *elIoRfidClose(elIoParam_t iop)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoRfid_t		*dio = io->dio;

	elIoBufFree(&io->msg.buf);

	/* close serial I/O device: */
	dio->rio->Close(dio->rio);

	elDelete(&dio->rio);
	elDelete(&io->dio);

	;

#if 0
Error:
#endif

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoRfidRead"
elErr_t *elIoRfidRead(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoRfid_t		*dio = io->dio;
	elIo_t			*rio = dio->rio;

	EL_ERR_CHECK( rio->Read(rio,buf,bufsize,plen) );

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoRfidWrite"
elErr_t *elIoRfidWrite(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoRfid_t			*dio = io->dio;
	elIo_t				*rio = dio->rio;

	EL_ERR_CHECK( rio->Write(rio,buf,bufsize,plen) );

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoRfidMsgRead"
elErr_t *elIoRfidMsgRead(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoRfid_t		*dio = io->dio;
	int				ch;

	(void) dio;
	(void) ch;
	(void) buf;
	(void) bufsize;

	*plen = 0;

	if ( io->msg.complete )
		io->msg.complete = FALSE;

	if ( dio->rio && dio->rio->MsgRead )
		EL_ERR_CHECK( dio->rio->MsgRead(dio->rio,buf,bufsize,plen) );

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoRfidMsgWrite"
elErr_t *elIoRfidMsgWrite(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoRfid_t		*dio = io->dio;

	(void) dio;
	(void) buf;
	(void) bufsize;

	*plen = 0;

	;

	*plen = bufsize;

#if 0
Error:
#endif

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoRfidCtrl"
elErr_t *elIoRfidCtrl(elIoParam_t iop,int cmd,void *pdata)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoRfid_t	*dio = io->dio;

	EL_ERR_CHECK( dio->rio->Ctrl(dio->rio,cmd,pdata) );

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoRfidAttrSet"
elErr_t *elIoRfidAttrSet(elIoParam_t iop,TCHAR *attr_name,TCHAR *attr_val)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoRfid_t	*dio = io->dio;

	if ( !stricmp(attr_name,"attr1") )
	{
		;
	}
	else if ( !stricmp(attr_name,"attr2") )
	{
		;
	}
	else
	{
#if 1
		/* other attributes, e.g. "POLL": */
		EL_ERR_CHECK( dio->rio->AttrSet(dio->rio,attr_name,attr_val) );
#else
		EL_ERR_THROW(
			EL_ERR_INVAL,NULL,elStrTmpFmt("%s: unknown attribute",attr_name)
		);
#endif
	}

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoRfidAttrGet"
elErr_t *elIoRfidAttrGet(elIoParam_t iop,TCHAR *attr_name,TCHAR **p_attr_val)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoRfid_t	*dio = io->dio;

	(void) dio;
	(void) p_attr_val;

	if ( !stricmp(attr_name,"attr1") )
	{
		;
	}
	else if ( !stricmp(attr_name,"attr2") )
	{
		;
	}
	else
	{
		EL_ERR_THROW(
			EL_ERR_INVAL,NULL,elStrTmpFmt("%s: unknown attribute",attr_name)
		);
	}

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoRfidTaskRead"
elErr_t *elIoRfidTaskRead(
	elIoTask_t *task,
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;

	(void) task;
	(void) buf;
	(void) bufsize;
	(void) plen;
	(void) io;

	;

#if 0
Error:
#endif

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoRfidTaskWrite"
elErr_t *elIoRfidTaskWrite(
	elIoTask_t *task,
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;

	(void) task;
	(void) buf;
	(void) bufsize;
	(void) plen;
	(void) io;

	;

#if 0
Error:
#endif

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoRfidTaskExecute"
elErr_t *elIoRfidTaskExecute(elIoTask_t *task)
{
	EL_ERR_VAR;
	elIo_t			*io;
	elErr_t			*io_err;
	elUInt32_t		len;
	register int	i;

#if 0
	elLogD(_T("%s(): start"),_T(__FUNC__));
#endif

	for(i = 0; i < task->io_cnt; i++)
	{
		io = task->io[i];

		if ( io->intfc != elIoRfidIntfc )
			continue;

		io_err = elIoRfidRead(io,io->buf.ptr,io->buf.size,&len);
		if ( io_err )
		{
			/* save error: */
			EL_ERR_CHECK( elMemRealloc(&io->err,sizeof(*io->err)) );
			*io->err = *io_err;
		}
		else
			elMemFree(&io->err);
	}

#if 0
	elLogD(_T("%s(): end"),_T(__FUNC__));
#endif

	;

Error:

	return err;
}

/******************************************************************************/
