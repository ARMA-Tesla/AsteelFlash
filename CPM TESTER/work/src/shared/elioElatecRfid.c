/*
 *  "elIoElatecRfid.c" - Elatec RFID I/O routines.
 *
 *	(c) 2007, 2008 ELCOM, a.s.
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
 *	2008-02-06	Radomir Tomis (Radomir.Tomis@dvi.elcom.cz)
 *		'elIoElatecRfidMsgRead()': message format according to
 *		"Low Level Communication Protocol for the T4A Reader Family".
 *	2007-11-19	Radomir Tomis (Radomir.Tomis@dvi.elcom.cz)
 *		Created (for RFID reader Elatec AXA200/AXA250).
 */

#if 1
static char _ident[] = "@(#) elIoElatecRfid.c 0.2 2008-02-06";
#else
static char _ident[] = "@(#) elIoElatecRfid.c 0.1 2007-11-19";
#endif

#include <elBase.h>
#include <elObj.h>
#include <elMem.h>
#include <elIo.h>
#include <elErr.h>
#include <elErrWin.h>

#define IO_OVERLAPPED	1

static elErr_t *elIoElatecRfidCtor(elObjPtr_t *po,va_list args);
static elErr_t *elIoElatecRfidDtor(elObjPtr_t *po);

static elObjClass_t gs_elIoElatecRfid_c = {
	sizeof(elIoElatecRfid_t),
	"elIoElatecRfid",
	elIoElatecRfidCtor,	/* Ctor()	*/
	elIoElatecRfidDtor,	/* Dtor()	*/
	NULL,				/* Clone()	*/
	NULL,				/* Differ()	*/
};

const elObjClass_t *elIoElatecRfid_c = &gs_elIoElatecRfid_c;

#define EL_IO_ELATECRFID_METHODS							\
		elIoElatecRfidOpen,			/* Open()			*/	\
		elIoElatecRfidClose,		/* Close()			*/	\
		elIoElatecRfidRead,			/* Read()			*/	\
		elIoElatecRfidWrite,		/* Write()			*/	\
		elIoElatecRfidMsgRead,		/* MsgRead()		*/	\
		elIoElatecRfidMsgWrite,		/* MsgWrite()		*/	\
		NULL,						/* Flush()			*/	\
		NULL,						/* Abort()			*/	\
		NULL,						/* AbortReset()		*/	\
		elIoElatecRfidCtrl,			/* Ctrl()			*/	\
		elIoElatecRfidAttrSet,		/* AttrSet()		*/	\
		elIoElatecRfidAttrGet,		/* AttrGet()		*/	\
		NULL,						/* Lock()			*/	\
		NULL,						/* Unlock()			*/	\
		NULL,						/* DevLock()		*/	\
		NULL,						/* DevUnlock()		*/	\
		NULL,						/* DebugLog()		*/	\
		elIoElatecRfidTaskRead,		/* TaskRead()		*/	\
		elIoElatecRfidTaskWrite,	/* TaskWrite()		*/	\
		elIoElatecRfidTaskExecute	/* TaskExecute()	*/	\

const elIoIntfc_t g_elIoElatecRfidIntfc = {
	_T("ELATEC-RFID"),	EL_IO_ELATECRFID_METHODS
};
const elIoIntfc_t *elIoElatecRfidIntfc = &g_elIoElatecRfidIntfc;

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoElatecRfidCtor"
static elErr_t *elIoElatecRfidCtor(elObjPtr_t *po,va_list args)
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
#define	__FUNC__	"elIoElatecRfidDtor"
static elErr_t *elIoElatecRfidDtor(elObjPtr_t *po)
{
	EL_ERR_VAR;
	elIo_t		*io = (elIo_t *)*po;

	(void) io;

	;

	return err;
}

/******************************************************************************/
/*
 * 'elIoElatecRfidGetc()':
 */
/** Read a character from the RFID device
 */
static elErr_t *elIoElatecRfidGetc(
	elIoParam_t iop,	/*!< I/O descriptor */
	int *pchar			/*!< pointer where to store the character read */
)
#undef	__FUNC__
#define	__FUNC__	"elIoElatecRfidGetc"
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoElatecRfid_t	*dio = io->dio;
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
#define	__FUNC__	"elIoElatecRfidOpen"
elErr_t *elIoElatecRfidOpen(elIoParam_t iop,TCHAR *rsrc,...)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoElatecRfid_t	*dio = NULL;
	elIo_t				*sio = NULL;
	elStr_t				*s;

	(void) rsrc;
	(void) dio;

	EL_ERR_CHECK( elNew(elIoElatecRfid_c,&io->dio) );
	EL_ERR_CHECK( elNew(elStr_c,&s) );

	dio = io->dio;
	dio->io = io;

	/*
	 * Open Elatec RFID device as serial device:
	 *
	 *	ELATEC-RFID:<serial-device>  =>  Serial:<serial-device>
	 *
	 * For example:
	 *
	 *	ELATEC-RFID:COM1/9600/8/n/1/n  =>  Serial:COM1/115200/8/n/1/n
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
#define	__FUNC__	"elIoElatecRfidClose"
elErr_t *elIoElatecRfidClose(elIoParam_t iop)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoElatecRfid_t		*dio = io->dio;

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
#define	__FUNC__	"elIoElatecRfidRead"
elErr_t *elIoElatecRfidRead(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoElatecRfid_t	*dio = io->dio;
	elIo_t				*sio = dio->sio;

	EL_ERR_CHECK( sio->Read(sio,buf,bufsize,plen) );

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoElatecRfidWrite"
elErr_t *elIoElatecRfidWrite(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoElatecRfid_t	*dio = io->dio;
	elIo_t				*sio = dio->sio;

	EL_ERR_CHECK( sio->Write(sio,buf,bufsize,plen) );

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoElatecRfidMsgRead"
elErr_t *elIoElatecRfidMsgRead(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	static char			s_hexdigit[] = "0123456789ABCDEF";
	elIoElatecRfid_t	*dio = io->dio;
	int					ch;
	
	*plen = 0;

	if ( io->msg.complete )
		io->msg.complete = FALSE;

#if 0	/* test simulated RFID message: */
	{	/* inner block */
	static byte s_rfid_msg[] = { 0x0B, 0x00, 0x11, 0x00, 0x00, 0x04, 0x13, 0x97, 0xA8, 0x91, 0xA3 };
	elIo_t		*sio = dio->sio;

	/* feed serial I/O buffer with simulated RFID message: */
	memcpy(sio->buf.base,s_rfid_msg,sizeof(s_rfid_msg));
	sio->buf.len = sizeof(s_rfid_msg);
	sio->buf.ptr = sio->buf.base;
	}	/* inner block */
#endif

	while( !io->msg.complete && io->msg.buf.len < io->msg.buf.size )
	{
		byte			data;
		register int	i;

		EL_ERR_CHECK( elIoElatecRfidGetc(io,&ch) );
		data = (byte) ch;
		*io->msg.buf.ptr = data;

		/*
		 * Elatec RFID message response format:
		 *
		 *	\\data.ostrava.elcom.cz\DVI_Projects\ATS\Projects\TomSpeed_EVIP070240_LinkaX95Komponenty\Doc\
		 *		Customer_Specification\RFID\T4Wtest_and_AXA_communication_protocol.zip\
		 *		T4Wtest_and_AXA_communication_protocol\low_level_communication.pdf
		 *	http://www.elatec.cz/rfid/pdf/Multi-Tag%20communication.pdf
		 *
		 *	Offset	 Size	Parameter
		 *	------------------------------------------------------------------------
		 *		 0		1	length		total byte count of the reply packet (4+n+1)
		 *		 1		1	address		actual reader address
		 *		 2		1	command		the request type identifier
		 *		 3		1	status		command completition status byte
		 *		 4		n	data		supplementary data replied by the reader
		 *	   4+n      1	checksum	XOR sum of all bytes from offset 0 to 4+n-1
		 */
		/*
		 * Message offset (message length so far excluding the current byte
		 * 'data'):
		 */
		switch( io->msg.buf.len )
		{
		case 0:
			dio->msg.len = data;
			break;

		case 1:
			dio->msg.addr = data;
			break;

		case 2:
			dio->msg.cmd = data;
			break;

		case 3:
			dio->msg.status = data;
			break;

		case 4:
			dio->msg.data = io->msg.buf.ptr;
			break;

		default:
			if ( io->msg.buf.len == (elUInt32_t) (dio->msg.len - 1) )
			{
				elByte_t	xsum;

				/*
				 * Message checksum:
				 */
				dio->msg.xsum = data;

				/*
				 * Calculate checksum:
				 */
				for(i = xsum = 0; i < dio->msg.len - 1; i++)
					xsum ^= io->msg.buf.base[i];

				/*
				 * Compare checksum:
				 */
				if ( xsum != dio->msg.xsum )
				{
					elLog(
						"%s(): %s: received checksum (0x%02X) does not match expected checksum (0x%02X)",
						__FUNC__,io->name,dio->msg.xsum,xsum
					);
					elLogD(
					"%s(): %s: %d message bytes received including checksum byte:",
						__FUNC__,io->name,dio->msg.len
					);
					elLogHexDump(io->msg.buf.base,dio->msg.len,
						(elErr_t *(*)(char *s))elLogD,"-i2");
				}
				io->msg.complete = TRUE;		/* end of message */
			}
			break;

		}	/* switch( io->msg.buf.len ) */

		io->msg.buf.ptr++;
		io->msg.buf.len++;

	}	/* while( !io->msg.complete && ...) */

	if ( io->msg.complete )
	{
		elUInt32_t	msglen;

		/*
		 * Response "Read serial number" ?
		 */
		if ( dio->msg.cmd != 0x11 )
		{
			elLog("%s(): %s: unsupported command ID received",
				__FUNC__,io->name,dio->msg.cmd);
			EL_ERR_THROW(EL_ERR,NULL,NULL);
		}

		/*
		 * Check status:
		 */
		if ( dio->msg.status != 0x00 )
		{
			elLog("%s(): %s: unsupported status received",
				__FUNC__,io->name,dio->msg.status);
			EL_ERR_THROW(EL_ERR,NULL,NULL);
		}

		if ( dio->msg_fmt_generic )
		{
			elIoRfidMsg_t *msg;

			EL_ERR_CHECK_COND( bufsize < sizeof(*msg), EL_ERR_INVAL,NULL );

			/*
			 * Check message type-ID:
			 */
			if ( dio->msg.data[0] != 0 )	/* IDRO-A ? */
			{
				elLog("%s(): %s: 0x%02X: unsupported type-ID",
					__FUNC__,io->name,dio->msg.data[0]);
				EL_ERR_THROW(EL_ERR,NULL,NULL);
			}

			msg = (elIoRfidMsg_t *) buf;
			msglen = sizeof(*msg);
			memcpy(msg,dio->msg.data+1,msglen);
			*plen = msglen;
		}
		else
		{
			elIoElatecRfidMsg_t *msg;

			EL_ERR_CHECK_COND( bufsize < sizeof(*msg), EL_ERR_INVAL,NULL );

			msg = (elIoElatecRfidMsg_t *) buf;
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
#define	__FUNC__	"elIoElatecRfidMsgWrite"
elErr_t *elIoElatecRfidMsgWrite(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoElatecRfid_t		*dio = io->dio;

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
#define	__FUNC__	"elIoElatecRfidCtrl"
elErr_t *elIoElatecRfidCtrl(elIoParam_t iop,int cmd,void *pdata)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoElatecRfid_t	*dio = io->dio;

	EL_ERR_CHECK( dio->sio->Ctrl(dio->sio,cmd,pdata) );

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoElatecRfidAttrSet"
elErr_t *elIoElatecRfidAttrSet(elIoParam_t iop,TCHAR *attr_name,TCHAR *attr_val)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoElatecRfid_t	*dio = io->dio;

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
			EL_ERR_INVAL,NULL,elStrTmpFmt("%s: unknown attribute",attr_name)
		);
#endif
	}

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoElatecRfidAttrGet"
elErr_t *elIoElatecRfidAttrGet(
	elIoParam_t iop,TCHAR *attr_name,TCHAR **p_attr_val
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoElatecRfid_t	*dio = io->dio;

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
#define	__FUNC__	"elIoElatecRfidTaskRead"
elErr_t *elIoElatecRfidTaskRead(
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
#define	__FUNC__	"elIoElatecRfidTaskWrite"
elErr_t *elIoElatecRfidTaskWrite(
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
#define	__FUNC__	"elIoElatecRfidTaskExecute"
elErr_t *elIoElatecRfidTaskExecute(elIoTask_t *task)
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

		if ( io->intfc != elIoElatecRfidIntfc )
			continue;

		io_err = elIoElatecRfidRead(io,io->buf.ptr,io->buf.size,&len);
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
