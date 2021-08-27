/*
 *  "elIoPromagRfid.c" - Promag RFID I/O routines.
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
 *	2007-11-09	Radomir Tomis (Radomir.Tomis@dvi.elcom.cz)
 *		Created (for RFID reader Promag GP20).
 */

#if 1
static char _ident[] = "@(#) elIoPromagRfid.c 0.1 2007-11-09";
#else
#endif

#include <elBase.h>
#include <elObj.h>
#include <elMem.h>
#include <elIo.h>
#include <elErr.h>
#include <elErrWin.h>

#define IO_OVERLAPPED	1

static elErr_t *elIoPromagRfidCtor(elObjPtr_t *po,va_list args);
static elErr_t *elIoPromagRfidDtor(elObjPtr_t *po);

static elObjClass_t gs_elIoPromagRfid_c = {
	sizeof(elIoPromagRfid_t),
	"elIoPromagRfid",
	elIoPromagRfidCtor,	/* Ctor()	*/
	elIoPromagRfidDtor,	/* Dtor()	*/
	NULL,				/* Clone()	*/
	NULL,				/* Differ()	*/
};

const elObjClass_t *elIoPromagRfid_c = &gs_elIoPromagRfid_c;

#define EL_IO_PROMAGRFID_METHODS							\
		elIoPromagRfidOpen,			/* Open()			*/	\
		elIoPromagRfidClose,		/* Close()			*/	\
		elIoPromagRfidRead,			/* Read()			*/	\
		elIoPromagRfidWrite,		/* Write()			*/	\
		elIoPromagRfidMsgRead,		/* MsgRead()		*/	\
		elIoPromagRfidMsgWrite,		/* MsgWrite()		*/	\
		NULL,						/* Flush()			*/	\
		NULL,						/* Abort()			*/	\
		NULL,						/* AbortReset()		*/	\
		elIoPromagRfidCtrl,			/* Ctrl()			*/	\
		elIoPromagRfidAttrSet,		/* AttrSet()		*/	\
		elIoPromagRfidAttrGet,		/* AttrGet()		*/	\
		NULL,						/* Lock()			*/	\
		NULL,						/* Unlock()			*/	\
		NULL,						/* DevLock()		*/	\
		NULL,						/* DevUnlock()		*/	\
		NULL,						/* DebugLog()		*/	\
		elIoPromagRfidTaskRead,		/* TaskRead()		*/	\
		elIoPromagRfidTaskWrite,	/* TaskWrite()		*/	\
		elIoPromagRfidTaskExecute	/* TaskExecute()	*/	\

const elIoIntfc_t g_elIoPromagRfidIntfc = {
	_T("PROMAG-RFID"),	EL_IO_PROMAGRFID_METHODS
};
const elIoIntfc_t *elIoPromagRfidIntfc = &g_elIoPromagRfidIntfc;

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoPromagRfidCtor"
static elErr_t *elIoPromagRfidCtor(elObjPtr_t *po,va_list args)
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
#define	__FUNC__	"elIoPromagRfidDtor"
static elErr_t *elIoPromagRfidDtor(elObjPtr_t *po)
{
	EL_ERR_VAR;
	elIo_t		*io = (elIo_t *)*po;

	(void) io;

	;

	return err;
}

/******************************************************************************/
/*
 * 'elIoPromagRfidGetc()':
 */
/** Read a character from the RFID device
 */
static elErr_t *elIoPromagRfidGetc(
	elIoParam_t iop,	/*!< I/O descriptor */
	int *pchar			/*!< pointer where to store the character read */
)
#undef	__FUNC__
#define	__FUNC__	"elIoPromagRfidGetc"
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoPromagRfid_t	*dio = io->dio;
	elIo_t				*sio = dio->sio;
	elByte_t			ch;
	elUInt32_t			len;

	EL_ERR_CHECK( sio->Read(sio,&ch,sizeof(ch),&len) );
	*pchar = ch;

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoPromagRfidOpen"
elErr_t *elIoPromagRfidOpen(elIoParam_t iop,TCHAR *rsrc,...)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoPromagRfid_t	*dio = NULL;
	elIo_t				*sio = NULL;
	elStr_t				*s;

	(void) rsrc;
	(void) dio;

	EL_ERR_CHECK( elNew(elIoPromagRfid_c,&io->dio) );
	EL_ERR_CHECK( elNew(elStr_c,&s) );

	dio = io->dio;
	dio->io = io;

	/*
	 * Open Promag RFID device as serial device:
	 *
	 *	PROMAG-RFID:<serial-device>  =>  Serial:<serial-device>
	 *
	 * For example:
	 *
	 *	PROMAG-RFID:COM1/9600/8/n/1/n  =>  Serial:COM1/115200/8/n/1/n
	 */
	EL_ERR_CHECK( elNew(elIo_c,&dio->sio) );
	sio = dio->sio;
	EL_ERR_CHECK( s->Fmt(s,"Serial:%s",io->str.path->ptr) );
	EL_ERR_CHECK( sio->Open(sio,s->ptr) );

	elStrCpy(io->str.path,sio->str.path->ptr);
	elStrCpy(io->str.attrs,sio->str.attrs->ptr);

	io->item_size.bits	= 8;
	io->item_size.bytes	= 1;
	io->item_size.bpb	= 8;

	EL_ERR_CHECK( elIoBufAlloc(&io->msg.buf,1024) );

Error:

	if ( err )
	{
		elIoBufFree(&io->msg.buf);
		elDelete(&dio->sio);
		elDelete(&io->dio);
	}

	elDelete(&s);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoPromagRfidClose"
elErr_t *elIoPromagRfidClose(elIoParam_t iop)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoPromagRfid_t		*dio = io->dio;

	elIoBufFree(&io->msg.buf);

	/* close serial I/O device: */
	dio->sio->Close(dio->sio);

	elDelete(&dio->sio);
	elDelete(&io->dio);

	;

#if 0
Error:
#endif

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoPromagRfidRead"
elErr_t *elIoPromagRfidRead(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoPromagRfid_t	*dio = io->dio;
	elIo_t				*sio = dio->sio;

	EL_ERR_CHECK( sio->Read(sio,buf,bufsize,plen) );

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoPromagRfidWrite"
elErr_t *elIoPromagRfidWrite(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoPromagRfid_t	*dio = io->dio;
	elIo_t				*sio = dio->sio;

	EL_ERR_CHECK( sio->Write(sio,buf,bufsize,plen) );

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoPromagRfidMsgRead"
elErr_t *elIoPromagRfidMsgRead(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	static char			s_hexdigit[] = "0123456789ABCDEF";
	elIoPromagRfid_t	*dio = io->dio;
	int					ch;
	
	*plen = 0;

	if ( io->msg.complete )
		io->msg.complete = FALSE;

	while( !io->msg.complete && io->msg.buf.len < io->msg.buf.size )
	{
		char			*p;
		byte			data;
		register int	i;

		EL_ERR_CHECK( elIoPromagRfidGetc(io,&ch) );
		data = (byte) ch;
		*io->msg.buf.ptr = data;

		/*
		 * Promag RFID message response format:
		 *
		 *	Offset	 Size	Parameter
		 *	--------------------------------------------------------------------
		 *		 0		1	0x02 (STX) - Start of text
		 *		 1	   10	Data (5 RFID bytes coded as hexadecimal ASCII
		 *					characters, e.g.: "01054F76E8")
		 *		11		1	0x0D (CR)
		 *		12		1	0x0A (LF)
		 *		13		1	0x03 (ETX) - End of text
		 */
		/*
		 * Message offset (message length so far excluding the current byte
		 * 'data'):
		 */
		switch( io->msg.buf.len )
		{
		case 0:
			if ( data != 0x02 )		/* STX (start of text) ? */
				continue;			/* no, wait for STX */
			break;

		default:
			/*
			 * RFID byte:
			 */
			p = strchr(s_hexdigit,toupper(data));
			if ( !p )
			{
				/* invalid data, wait for STX: */
				io->msg.buf.len = 0;
				continue;
			}
			data = (byte) (p - s_hexdigit);

			i = io->msg.buf.len - 1;
			if ( i % 2 )
				data = (byte) ((dio->msg.data[i/2] << 4) | data);
			
			dio->msg.data[i/2] = data;
			break;

		case 11:
			if ( data != 0x0D )		/* CR ? */
			{
				/* invalid data, wait for STX: */
				io->msg.buf.len = 0;
				continue;
			}
			break;

		case 12:
			if ( data != 0x0A )		/* LF ? */
			{
				/* invalid data, wait for STX: */
				io->msg.buf.len = 0;
				continue;
			}
			break;

		case 13:
			if ( data != 0x03 )		/* ETX ? */
			{
				/* invalid data, wait for STX: */
				io->msg.buf.len = 0;
				continue;
			}
			io->msg.complete = TRUE;		/* end of message */
			break;

		}	/* switch( io->msg.buf.len ) */

		io->msg.buf.len++;

	}	/* while( !io->msg.complete && ...) */

	if ( io->msg.complete )
	{
		elUInt32_t	msglen;

		if ( dio->msg_fmt_generic )
		{
			elIoRfidMsg_t *msg;

			EL_ERR_CHECK_COND( bufsize < sizeof(*msg), EL_ERR_INVAL,NULL );

			msg = (elIoRfidMsg_t *) buf;
			msglen = sizeof(*msg);
			memcpy(msg,dio->msg.data,msglen);
			*plen = msglen;
		}
		else
		{
			elIoPromagRfidMsg_t *msg;

			EL_ERR_CHECK_COND( bufsize < sizeof(*msg), EL_ERR_INVAL,NULL );

			msg = (elIoPromagRfidMsg_t *) buf;
			msglen = sizeof(*msg);
			memcpy(msg,&dio->msg,msglen);
			*plen = msglen;

		}

		io->msg.buf.ptr = io->msg.buf.base;
		io->msg.buf.len = 0;
	}

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoPromagRfidMsgWrite"
elErr_t *elIoPromagRfidMsgWrite(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoPromagRfid_t		*dio = io->dio;

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
#define	__FUNC__	"elIoPromagRfidCtrl"
elErr_t *elIoPromagRfidCtrl(elIoParam_t iop,int cmd,void *pdata)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoPromagRfid_t	*dio = io->dio;

	EL_ERR_CHECK( dio->sio->Ctrl(dio->sio,cmd,pdata) );

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoPromagRfidAttrSet"
elErr_t *elIoPromagRfidAttrSet(elIoParam_t iop,TCHAR *attr_name,TCHAR *attr_val)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoPromagRfid_t	*dio = io->dio;

	if ( !stricmp(attr_name,"MSG_FMT") )
	{
		if ( attr_val && !stricmp(attr_val,"GENERIC") )
			dio->msg_fmt_generic = TRUE;
		else
			dio->msg_fmt_generic = FALSE;
	}
	else if ( !stricmp(attr_name,"attr1") )
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
		EL_ERR_CHECK( dio->sio->AttrSet(dio->sio,attr_name,attr_val) );
#else
		EL_ERR_THROW(
			EL_ERR_INVAL,NULL,elStrTmpFmt("%s: unknown attribute",attr)
		);
#endif
	}

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoPromagRfidAttrGet"
elErr_t *elIoPromagRfidAttrGet(
	elIoParam_t iop,TCHAR *attr_name,TCHAR **p_attr_val
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoPromagRfid_t	*dio = io->dio;

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
#define	__FUNC__	"elIoPromagRfidTaskRead"
elErr_t *elIoPromagRfidTaskRead(
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
#define	__FUNC__	"elIoPromagRfidTaskWrite"
elErr_t *elIoPromagRfidTaskWrite(
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
#define	__FUNC__	"elIoPromagRfidTaskExecute"
elErr_t *elIoPromagRfidTaskExecute(elIoTask_t *task)
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

		if ( io->intfc != elIoPromagRfidIntfc )
			continue;

		io_err = elIoPromagRfidRead(io,io->buf.ptr,io->buf.size,&len);
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
