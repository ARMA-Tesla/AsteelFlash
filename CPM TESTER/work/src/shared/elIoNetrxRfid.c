/*
 *  "elIoNetrxRfid.c" - Netronix RFID I/O routines.
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
static char _ident[] = "@(#) elIoNetrxRfid.c 0.1 2007-03-05";
#else
#endif

#include <elBase.h>
#include <elObj.h>
#include <elMem.h>
#include <elIo.h>
#include <elErr.h>
#include <elErrWin.h>

#define IO_OVERLAPPED	1

static elErr_t *elIoNetrxRfidCtor(elObjPtr_t *po,va_list args);
static elErr_t *elIoNetrxRfidDtor(elObjPtr_t *po);

static elObjClass_t gs_elIoNetrxRfid_c = {
	sizeof(elIoNetrxRfid_t),
	"elIoNetrxRfid",
	elIoNetrxRfidCtor,	/* Ctor()	*/
	elIoNetrxRfidDtor,	/* Dtor()	*/
	NULL,				/* Clone()	*/
	NULL,				/* Differ()	*/
};

const elObjClass_t *elIoNetrxRfid_c = &gs_elIoNetrxRfid_c;

#define EL_IO_NETRXRFID_METHODS								\
		elIoNetrxRfidOpen,			/* Open()			*/	\
		elIoNetrxRfidClose,			/* Close()			*/	\
		elIoNetrxRfidRead,			/* Read()			*/	\
		elIoNetrxRfidWrite,			/* Write()			*/	\
		elIoNetrxRfidMsgRead,		/* MsgRead()		*/	\
		elIoNetrxRfidMsgWrite,		/* MsgWrite()		*/	\
		NULL,						/* Flush()			*/	\
		NULL,						/* Abort()			*/	\
		NULL,						/* AbortReset()		*/	\
		elIoNetrxRfidCtrl,			/* Ctrl()			*/	\
		elIoNetrxRfidAttrSet,		/* AttrSet()		*/	\
		elIoNetrxRfidAttrGet,		/* AttrGet()		*/	\
		NULL,						/* Lock()			*/	\
		NULL,						/* Unlock()			*/	\
		NULL,						/* DevLock()		*/	\
		NULL,						/* DevUnlock()		*/	\
		NULL,						/* DebugLog()		*/	\
		elIoNetrxRfidTaskRead,		/* TaskRead()		*/	\
		elIoNetrxRfidTaskWrite,		/* TaskWrite()		*/	\
		elIoNetrxRfidTaskExecute	/* TaskExecute()	*/	\

const elIoIntfc_t g_elIoNetrxRfidIntfc = {
	_T("NETRX-RFID"),	EL_IO_NETRXRFID_METHODS
};
const elIoIntfc_t *elIoNetrxRfidIntfc = &g_elIoNetrxRfidIntfc;

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoNetrxRfidCtor"
static elErr_t *elIoNetrxRfidCtor(elObjPtr_t *po,va_list args)
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
#define	__FUNC__	"elIoNetrxRfidDtor"
static elErr_t *elIoNetrxRfidDtor(elObjPtr_t *po)
{
	EL_ERR_VAR;
	elIo_t		*io = (elIo_t *)*po;

	(void) io;

	;

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoNetrxRfidCrcCalc"
static elUInt16_t elIoNetrxRfidCrcCalc(byte *buf,size_t bufsize)
{
	elUInt16_t		c, crc;
	register int	i;

	c = crc = 0;
	while( bufsize--)
	{
		c = (elUInt16_t) (((crc >> 8) ^ *buf) << 8);
		for(i = 0; i < 8; i++)
		{
			if ( c & 0x8000 )
				c = (elUInt16_t) ((c << 1) ^ 0x1021);
			else
				c <<= 1;
		}
		crc = (elUInt16_t) (c ^ (crc << 8));
		buf++;
	}
	return crc;
}

/******************************************************************************/
/*
 * 'elIoNetrxRfidGetc()':
 */
/** Read a character from the RFID device
 */
static elErr_t *elIoNetrxRfidGetc(
	elIoParam_t iop,	/*!< I/O descriptor */
	int *pchar			/*!< pointer where to store the character read */
)
#undef	__FUNC__
#define	__FUNC__	"elIoNetrxRfidGetc"
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoNetrxRfid_t		*dio = io->dio;
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
#define	__FUNC__	"elIoNetrxRfidOpen"
elErr_t *elIoNetrxRfidOpen(elIoParam_t iop,TCHAR *rsrc,...)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoNetrxRfid_t	*dio = NULL;
	elIo_t			*sio = NULL;
	elStr_t			*s;

	(void) rsrc;
	(void) dio;

	EL_ERR_CHECK( elNew(elIoNetrxRfid_c,&io->dio) );
	EL_ERR_CHECK( elNew(elStr_c,&s) );

	dio = io->dio;
	dio->io = io;

	/*
	 * Open Netronix RFID device as serial device:
	 *
	 *	NETRX-RFID:<serial-device>  =>  Serial:<serial-device>
	 *
	 * For example:
	 *
	 *	NETRX-RFID:COM1/115200/8/n/1/n  =>  Serial:COM1/115200/8/n/1/n
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
#define	__FUNC__	"elIoNetrxRfidClose"
elErr_t *elIoNetrxRfidClose(elIoParam_t iop)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoNetrxRfid_t		*dio = io->dio;

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
#define	__FUNC__	"elIoNetrxRfidRead"
elErr_t *elIoNetrxRfidRead(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoNetrxRfid_t		*dio = io->dio;
	elIo_t				*sio = dio->sio;

	EL_ERR_CHECK( sio->Read(sio,buf,bufsize,plen) );

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoNetrxRfidWrite"
elErr_t *elIoNetrxRfidWrite(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoNetrxRfid_t		*dio = io->dio;
	elIo_t				*sio = dio->sio;

	EL_ERR_CHECK( sio->Write(sio,buf,bufsize,plen) );

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoNetrxRfidMsgRead"
elErr_t *elIoNetrxRfidMsgRead(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoNetrxRfid_t	*dio = io->dio;
	int				ch;

	*plen = 0;

	if ( io->msg.complete )
		io->msg.complete = FALSE;

	while( !io->msg.complete && io->msg.buf.len < io->msg.buf.size )
	{
		byte data;

		EL_ERR_CHECK( elIoNetrxRfidGetc(io,&ch) );
		data = (byte) ch;
		*io->msg.buf.ptr = data;

		/*
		 * Netronix RFID message response format:
		 *
		 *	Offset	 Size	Parameter
		 *	--------------------------------------------------------------------
		 *		 0		1	Module address
		 *		 1		1	Frame length (from 1st byte (module address) until
		 *					and including 2 CRC bytes)
		 *		 2		1	Response identifier ('EL_IO_NETRXRFID_CMD_...' + 1)
		 *		 3		n	Parameters/Data
		 *		 3+n	1	Operation code ('EL_IO_NETRXRFID_OC_...')
		 *		 4+n	1	CRC high
		 *		 5+n	1	CRC low
		 */
		switch( io->msg.buf.len )	/* message offset */
		{
		case 0:
			dio->msg.module_addr = data;
			break;
		case 1:
			dio->msg.frame_len	= data;
			dio->msg.data_len	= (byte) (dio->msg.frame_len - (3 + 3));
			break;
		case 2:
			dio->msg.cmd = data;
			break;
		default:
			if ( io->msg.buf.len >= (elUInt32_t) (3 + dio->msg.data_len) )
			{
				elUInt16_t	crc;

				switch( io->msg.buf.len - dio->msg.data_len )
				{
				case 3:		/* operation code */
					if ( data != EL_IO_NETRXRFID_OC_SUCCESSFUL )
						elLog("%s: operation code: %d",io->name,data);
					dio->msg.data = io->msg.buf.base + 3;
					dio->msg.op_code = data;
					break;
				case 4:		/* CRC high */
					dio->msg.crc = (elUInt16_t) (data << 8);
					break;
				case 5:		/* CRC low */
					dio->msg.crc |= data;

					crc = elIoNetrxRfidCrcCalc(
						io->msg.buf.base,dio->msg.frame_len - 2);
					if ( crc != dio->msg.crc )
					{
						elLog(
			"%s: received CRC (0x%04X) does not match expected CRC (0x%04X)",
							io->name,dio->msg.crc,crc);
						elLogD(
						"%s: %d message bytes received including CRC bytes:",
							io->name,dio->msg.frame_len
						);
						elLogHexDump(io->msg.buf.base,dio->msg.frame_len,
							(elErr_t *(*)(char *s))elLogD,"-i2");
					}
					break;
				case 6:		/* final (undocumented) byte (usually binary zero) */
					io->msg.complete = TRUE;		/* end of message */
					break;
				}
			}
			break;
		}	/* switch( io->msg.buf.len ) */

		io->msg.buf.ptr++;
		io->msg.buf.len++;

	}	/* while( !io->msg.complete && io->msg.buf.len < io->msg.buf.size ) */

	if ( io->msg.complete )
	{
		elUInt32_t	msglen;

		if ( dio->msg_fmt_generic )
		{
			elIoRfidMsg_t *msg;

			EL_ERR_CHECK_COND( bufsize < sizeof(*msg), EL_ERR_INVAL,NULL );

			msg = (elIoRfidMsg_t *) buf;
			msglen = dio->msg.data_len;
			memcpy(msg,dio->msg.data,msglen);
			*plen = msglen;
		}
		else
		{
			elIoNetrxRfidMsg_t *msg;

			EL_ERR_CHECK_COND( bufsize < sizeof(*msg), EL_ERR_INVAL,NULL );

			msg = (elIoNetrxRfidMsg_t *) buf;
			msglen = dio->msg.frame_len;
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
#define	__FUNC__	"elIoNetrxRfidMsgWrite"
elErr_t *elIoNetrxRfidMsgWrite(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoNetrxRfid_t		*dio = io->dio;

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
#define	__FUNC__	"elIoNetrxRfidCtrl"
elErr_t *elIoNetrxRfidCtrl(elIoParam_t iop,int cmd,void *pdata)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoNetrxRfid_t	*dio = io->dio;

	EL_ERR_CHECK( dio->sio->Ctrl(dio->sio,cmd,pdata) );

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoNetrxRfidAttrSet"
elErr_t *elIoNetrxRfidAttrSet(elIoParam_t iop,TCHAR *attr_name,TCHAR *attr_val)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoNetrxRfid_t	*dio = io->dio;

	if ( !stricmp(attr_name,"MSG_FMT") )
	{
		if ( attr_val && !stricmp(attr_val,"GENERIC") )
			dio->msg_fmt_generic = TRUE;
		else
			dio->msg_fmt_generic = FALSE;
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
#define	__FUNC__	"elIoNetrxRfidAttrGet"
elErr_t *elIoNetrxRfidAttrGet(
	elIoParam_t iop,TCHAR *attr_name,TCHAR **p_attr_val
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoNetrxRfid_t	*dio = io->dio;

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
#define	__FUNC__	"elIoNetrxRfidTaskRead"
elErr_t *elIoNetrxRfidTaskRead(
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
#define	__FUNC__	"elIoNetrxRfidTaskWrite"
elErr_t *elIoNetrxRfidTaskWrite(
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
#define	__FUNC__	"elIoNetrxRfidTaskExecute"
elErr_t *elIoNetrxRfidTaskExecute(elIoTask_t *task)
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

		if ( io->intfc != elIoNetrxRfidIntfc )
			continue;

		io_err = elIoNetrxRfidRead(io,io->buf.ptr,io->buf.size,&len);
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
