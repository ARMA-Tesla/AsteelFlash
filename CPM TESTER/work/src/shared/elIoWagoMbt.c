/*
 *  "elIoWagoMbt.c" - WAGO-MBT (Modbus TCP) I/O routines.
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
static char _ident[] = "@(#) elIoWagoMbt.c 0.1 2007-03-05";
#else
#endif

#include <elBase.h>
#include <elObj.h>
#include <elErr.h>
#include <elErrWsa.h>
#include <elErrWin.h>
#include <elMem.h>
#include <elMtx.h>
#include <elIo.h>
#include <elSocket.h>
#include <elTimer.h>

#define EL_ERR_CHECK_MBT(fn_mbt,errid,errstr)			\
		if ( (mbt_err = (fn_mbt)) != S_OK )				\
		{												\
			EL_ERR_THROW(errid,#fn_mbt,errstr);			\
			goto Error;									\
		}

#define MBT_ERR_CHECK(fn_mbt)	\
		EL_ERR_CHECK_MBT(fn_mbt,EL_ERR_IO,mbt_error_str(mbt_err))

typedef LONG mbt_errid_t;

static elErr_t *elIoWagoMbtCtor(elObjPtr_t *po,va_list args);
static elErr_t *elIoWagoMbtDtor(elObjPtr_t *po);

static elObjClass_t gs_elIoWagoMbt_c = {
	sizeof(elIoWagoMbt_t),
	"elIoWagoMbt",
	elIoWagoMbtCtor,	/* Ctor()	*/
	elIoWagoMbtDtor,	/* Dtor()	*/
	NULL,				/* Clone()	*/
	NULL,				/* Differ()	*/
};

const elObjClass_t *elIoWagoMbt_c = &gs_elIoWagoMbt_c;

#define EL_IO_WAGOMBT_METHODS								\
		elIoWagoMbtOpen,			/* Open()			*/	\
		elIoWagoMbtClose,			/* Close()			*/	\
		elIoWagoMbtRead,			/* Read()			*/	\
		elIoWagoMbtWrite,			/* Write()			*/	\
		NULL,						/* MsgRead()		*/	\
		NULL,						/* MsgWrite()		*/	\
		NULL,						/* Flush()			*/	\
		NULL,						/* Abort()			*/	\
		NULL,						/* AbortReset()		*/	\
		elIoWagoMbtCtrl,			/* Ctrl()			*/	\
		NULL,						/* AttrSet()		*/	\
		NULL,						/* AttrGet()		*/	\
		NULL,						/* Lock()			*/	\
		NULL,						/* Unlock()			*/	\
		NULL,						/* DevLock()		*/	\
		NULL,						/* DevUnlock()		*/	\
		NULL,						/* DebugLog()		*/	\
		elIoWagoMbtTaskRead,		/* TaskRead()		*/	\
		elIoWagoMbtTaskWrite,		/* TaskWrite()		*/	\
		elIoWagoMbtTaskExecute		/* TaskExecute()	*/	\

const elIoIntfc_t g_elIoWagoMbtIntfc = {
	_T("WAGO-MBT"),	EL_IO_WAGOMBT_METHODS
};
const elIoIntfc_t *elIoWagoMbtIntfc = &g_elIoWagoMbtIntfc;

static struct {
	TCHAR *str;
	byte id;
} gs_modbus_table[] = {
	_T("DI"),	MODBUSTCP_TABLE_INPUT_COIL,
	_T("DO"),	MODBUSTCP_TABLE_OUTPUT_COIL,
	_T("AI"),	MODBUSTCP_TABLE_INPUT_REGISTER,
	_T("AO"),	MODBUSTCP_TABLE_OUTPUT_REGISTER,
};

static struct {
	bool				mbt_init;
	elMtx_t				mtx;

	/* table of all unique devices used in all I/O handles: */
	elIoWagoMbtDev_t	**dev;
	int					dev_cnt;	/* # of valid devices in 'dev[]' */
	int					dev_acnt;	/* # of allocated elements in 'dev[]' */

} gs;

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"mbt_error_str"
static TCHAR *mbt_error_str(mbt_errid_t mbt_err)
{
	static struct {
		mbt_errid_t id;
		TCHAR *sym;
		TCHAR *str;
	} s_err[] = {
#define E(id)	MBT_##id,	#id
		/* errors: */
		E(THREAD_CREATION_ERROR),		_T("The work thread of the MBT library could not be created"),
		E(EXIT_TIMEOUT_ERROR),			_T("The work-thread of the MBT library could not be terminated within the timeout"),
		E(UNKNOWN_THREAD_EXIT_ERROR),	_T("The work-thread of the MBT library has terminated with an error code"),
		E(UNAVAILABLE_CLOCK_ERROR),		_T("Unavailable clock error"),
		E(NO_ENTRY_ADDABLE_ERROR),		NULL,
		E(NO_JOB_ADDABLE_ERROR),		_T("Error in the job administration of the library"),
		E(HANDLE_INVALID_ERROR),		_T("The handle submitted is invalid"),
		E(CLOSE_FLAG_SET_ERROR),		NULL,
		E(SOCKET_TIMEOUT_ERROR),		_T("A Modbus/TCP telegram for the I/O job has been sent but no response has been received within the Request Timeout"),
		E(WRONG_RESPONSE_FC_ERROR),		NULL,
		E(RESPONSE_FALSE_LENGTH_ERROR),	NULL,
		E(EXIT_ERROR),					_T("The I/O job has been aborted by an MBTExit Call"),
		/* exceptions: */
		E(ILLEGAL_FUNCTION),			_T("Illegal Function"),
		E(ILLEGAL_DATA_ADDRESS),		_T("Illegal Data Address"),
		E(ILLEGAL_DATA_VALUE),			_T("Illegal Data Value"),
		E(ILLEGAL_RESPONSE_LENGTH),		NULL,
		E(ACKNOWLEDGE),					_T("Acknowledge"),
		E(SLAVE_DEVICE_BUSY),			_T("Slave Device Busy"),
		E(NEGATIVE_ACKNOWLEDGE),		NULL,
		E(MEMORY_PARITY_ERROR),			_T("Memory Parity Error"),
		E(GATEWAY_PATH_UNAVAILABLE),	_T("Gateway Path Unavailable"),
		E(GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND),
										_T("Gateway Target Device Failed To Respond"),
#undef E
	};
	register elStr_t *str;
	register int i;

	for(i = 0; i < EL_ARRAY_CNT(s_err); i++)
		if ( s_err[i].id == mbt_err )
			break;

	str = elStrTmpIdxGet(EL_STR_IDX_ERR_USR1);
	if ( !str )
		return _T("<") _T(__FUNC__) _T("(): cannot get temporary string") _T(">");

	if ( i < EL_ARRAY_CNT(s_err) )
	{
		if ( s_err[i].str )
		{
			str->Cpy(str,s_err[i].str);
			str->Cat(str,_T(" "));
		}
		else
			str->Cpy(str,_T(""));
		str->CatFmt(str,_T("<MBT_%s/0x%lX>"),s_err[i].sym,mbt_err);
	}
	else if ( HRESULT_FACILITY(mbt_err) == FACILITY_WIN32 )
	{
		str->Cpy(str,
#if 1
			elErrWsaStr(HRESULT_CODE(mbt_err))
#else
			elErrWinStr(HRESULT_CODE(mbt_err))
#endif
		);
	}
	else
		str->Fmt(str,_T("<MBT_\?\?\?/%ld>"),mbt_err);

	return str->ptr;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"mbt_error_log"
static void mbt_error_log(elLog_t *log,TCHAR *func,mbt_errid_t mbt_err)
{
	elLogPrintf(
		log,
		_T("%s%s\n"),
		func ? elStrTmpFmt(_T("%s(): "),func) : "",
		mbt_error_str(mbt_err)
	);
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"mbt_startup"
static elErr_t *mbt_startup(elIo_t *io)
{
	EL_ERR_VAR;
	mbt_errid_t mbt_err;

	(void) io;

	if ( !gs.mbt_init )
	{
		MBT_ERR_CHECK( MBTInit() );
		gs.mbt_init = TRUE;
	}

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"mbt_cleanup"
static elErr_t *mbt_cleanup(elIo_t *io)
{
	EL_ERR_VAR;
	mbt_errid_t mbt_err;

	if ( gs.mbt_init )
	{
		elTimer_t timer;

		elTimerStart(&timer);
		mbt_err = MBTExit();
		elTimerStop(&timer);
		io->DebugLog(io,1,_T("MBTExit() took %ss\n"),elTimerStr(timer,0,3));

		EL_IO_DEBUG_CODE(
			if ( mbt_err != S_OK )
				mbt_error_log(io->log,_T("MBTExit"),mbt_err);
		);

		gs.mbt_init = FALSE;
	}

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"modbus_io_table_str2id"
static int modbus_io_table_str2id(TCHAR *str)
{
	register int i;

	for(i = 0; i < EL_ARRAY_CNT(gs_modbus_table); i++)
		if ( !stricmp(gs_modbus_table[i].str,str) )
			break;
	return i < EL_ARRAY_CNT(gs_modbus_table) ? gs_modbus_table[i].id : -1;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"modbus_io_table_id2str"
static TCHAR *modbus_io_table_id2str(register int id)
{
	register int i;

	for(i = 0; i < EL_ARRAY_CNT(gs_modbus_table); i++)
		if ( gs_modbus_table[i].id == id )
			break;
	return i < EL_ARRAY_CNT(gs_modbus_table) ? gs_modbus_table[i].str : NULL;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"modbus_io_addr_parse"
static bool modbus_io_addr_parse(TCHAR *address,WORD *p_addr,WORD *p_cnt)
{
	TCHAR *p;

	/*
	 * address: addr|addr:cnt|addr1-addr2
	 *	- addr: DI|DO: bit; AI|AO: word
	 */

	*p_addr = (WORD) strtol(address,&p,0);
	switch( *p++ )
	{
	case ':':
		*p_cnt = (WORD) strtol(p,NULL,0);
		break;

	case '-':
		*p_cnt = (WORD) (strtol(p,NULL,0) - *p_addr + 1);
		break;

	default:
		*p_cnt = 1;
		break;
	}

	return TRUE;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"modbus_host_addr_parse"
static bool modbus_host_addr_parse(
	TCHAR *str,TCHAR **p_host,int *p_port,bool *p_tcp
)
{
	TCHAR *p;
	TCHAR *p2;

	*p_tcp = TRUE;
	/*
	 * Default MODBUS port number:
	 * (http://www.modbus.org/docs/Modbus_Application_Protocol_V1_1b.pdf)
	 */
	*p_port = 502;

	/*
	 * 'str': [port[/proto]@]host
	 */
	p2 = strchr(str,'@');
	if ( p2 )
	{
		*p_port = (int) _tcstol(str,&p,0);
		*p2++ = '\0';
		*p_host = p2;

		if ( *p == _T('/') )
		{
			p++;
			if ( !_tcsicmp(p,_T("udp")) )
				*p_tcp = FALSE;
		}
	}
	else
		*p_host = str;

	if ( !**p_host )
		*p_host = NULL;		/* localhost */

	return TRUE;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elWagoMbtDevAdd"
static elErr_t *elWagoMbtDevAdd(elIoParam_t iop,TCHAR *host_addr)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
#if !EL_IO_SIMULATE
	mbt_errid_t			mbt_err;
#endif
	elIoWagoMbt_t		*dio = io->dio;
	elIoWagoMbtDev_t	*dev;
	elStr_t				*str1;
	elStr_t				*str2;
	struct sockaddr_in	saddr;
	TCHAR				*addr;
	TCHAR				*host;
	int					port;
	bool				tcp;
	register int		i;

	elNew(elStr_c,&str1);
	elNew(elStr_c,&str2);

	EL_ERR_CHECK( elMtxLock(&gs.mtx) );

	EL_ERR_CHECK( mbt_startup(io) );

	/*
	 * Parse 'host':
	 *
	 *		[port[/proto]@]host
	 */
	EL_ERR_CHECK( str1->Cpy(str1,host_addr) );	/* temporary copy */
	if ( !modbus_host_addr_parse(str1->ptr,&host,&port,&tcp) )
	{
		EL_ERR_THROW(
			EL_ERR_INVAL,NULL,
			elStrTmpFmt(_T("%s: error parsing host address"),host)
		);
	}

	io->DebugLog(
		io,1,
		_T("%s(): host: '%s', port: %d, tcp: %d\n"),_T(__FUNC__),host,port,tcp
	);

	/* translate host name / IP address to IP address: */
	EL_ERR_CHECK( str2->Printf(str2,_T("%d@%s"),port,host) );
	EL_ERR_CHECK(
		elSocketAddrStr2Val(EL_SOCKET_CONNECT,str2->ptr,
			(struct sockaddr *)&saddr)
	);
	addr = elSocketAddrVal2Str((struct sockaddr *)&saddr,sizeof(saddr));
	addr = strchr(addr,'@') + 1;	/* skip port to IP address */

	/*
	 * See if device having the same IP address, port and TCP/UDP protocol
	 * triplet is already present/opened/connected:
	 */
	for(i = 0; i < gs.dev_acnt; i++)
	{
		if ( gs.dev[i] && !_tcscmp(gs.dev[i]->addr,addr)
			&& gs.dev[i]->port == port
			&& gs.dev[i]->tcp == tcp
		)
			break;
	}
	dev = i < gs.dev_acnt ? gs.dev[i] : NULL;

	if ( !dev )		/* device not connected yet ? */
	{
		/* allocate new device: */
		EL_ERR_CHECK( elMemAlloc(&dev,sizeof(*dev)) );

		memset(dev,0,sizeof(*dev));
		dev->socket = INVALID_HANDLE_VALUE;

#if EL_IO_SIMULATE
		;
#else
#if 1
		MBT_ERR_CHECK(
			MBTConnect(addr,(WORD)port,tcp,io->timeout,&dev->socket)
		);
#endif
#endif

		EL_MEM_CHECK_CRT();
		EL_ERR_CHECK( elMemStrdup(&dev->addr,addr) );
		EL_MEM_CHECK_CRT();
		dev->port	= port;
		dev->tcp	= tcp;

		/* see if there is empty slot in 'gs.dev[]': */
		for(i = 0; i < gs.dev_acnt; i++)
			if ( !gs.dev[i] )
				break;
		if ( i == gs.dev_acnt )		/* no empty slot ? */
		{
			size_t size;

			size = (++gs.dev_acnt) * sizeof(*gs.dev);
			EL_ERR_CHECK( elMemRealloc(&gs.dev,size) );
		}

		EL_MEM_CHECK_CRT();

		gs.dev[i] = dev;
		gs.dev_cnt++;
	}

	dio->dev = dev;
	dev->ref_cnt++;

	;

Error:

	EL_ERR_CHECK( elMtxUnlock(&gs.mtx) );

	elDelete(&str1);
	elDelete(&str2);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elWagoMbtDevRemove"
static elErr_t *elWagoMbtDevRemove(elIoParam_t iop)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	mbt_errid_t		mbt_err;
	elIoWagoMbt_t	*dio = io->dio;
	register int	i;

	EL_ERR_CHECK( elMtxLock(&gs.mtx) );

	for(i = 0; i < gs.dev_acnt; i++)
		if ( gs.dev[i] == dio->dev )
			break;
	EL_ERR_CHECK_COND( i == gs.dev_acnt, -1,NULL );
	
	/* last reference to the device ? */
	if ( !--dio->dev->ref_cnt )
	{
		/* last reference: disconnect from WAGO TCP/IP controller: */
		mbt_err = MBTDisconnect(dio->dev->socket);
		EL_IO_DEBUG_CODE(
			if ( mbt_err != S_OK )
				mbt_error_log(io->log,_T("MBTDisconnect"),mbt_err);
		);

		elMemFree(&dio->dev->addr);
		elMemFree(&dio->dev);
		gs.dev[i] = NULL;
		gs.dev_cnt--;

		if ( !gs.dev_cnt )	/* last device closed ? */
		{
			mbt_cleanup(io);
			elMemFree(&gs.dev);
		}
	}

	EL_ERR_CHECK( elMtxUnlock(&gs.mtx) );

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoWagoMbtCtor"
static elErr_t *elIoWagoMbtCtor(elObjPtr_t *po,va_list args)
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
#define	__FUNC__	"elIoWagoMbtDtor"
static elErr_t *elIoWagoMbtDtor(elObjPtr_t *po)
{
	EL_ERR_VAR;
	elIo_t *io = (elIo_t *)*po;

	(void) io;

	;

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoWagoMbtOpen"
elErr_t *elIoWagoMbtOpen(elIoParam_t iop,TCHAR *rsrc,...)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoWagoMbt_t		*dio;
	elStr_t				*s;
	TCHAR				*p;
	TCHAR				*host;
	TCHAR				*table;
	TCHAR				*address;
	int					table_id;
	WORD				addr, cnt;
	int					n, i1, i2;

	(void) rsrc;

	EL_ERR_CHECK( elNew(elIoWagoMbt_c,&io->dio) );
	EL_ERR_CHECK( elNew(elStr_c,&s) );

	dio = io->dio;
	dio->io = io;

	/*
	 * Parse I/O path string ('io->str.path'):
	 *
	 *		{host|IP-address}/table/address
	 *
	 *		address: addr|addr:cnt|addr1-addr2
	 *		 - addr: DI|DO: bit; AI|AO: word
	 *
	 * For example:
	 *
	 *		192.168.5.101/DO/6
	 */
	EL_ERR_CHECK( elStrCpy(s,io->str.path->ptr) );
	p = s->ptr;
	i1 = i2 = -1;
	n = _stscanf(p,"%*[^/]/%n%*[^/]/%n%*s",&i1,&i2);
	if ( i1 == -1 || i2 == -1 )
	{
		EL_ERR_THROW(
			EL_ERR_INVAL,NULL,
			elStrTmpFmt(_T("%s: invalid WAGO I/O resource path"),
				io->str.path->ptr)
		);
	}
	p[i1-1] = _T('\0');
	p[i2-1] = _T('\0');
	host	= p;
	table	= p + i1;
	address	= p + i2;

	table_id = modbus_io_table_str2id(table);
	if ( table_id < 0 )
	{
		EL_ERR_THROW(
			EL_ERR_INVAL,NULL,
			elStrTmpFmt(_T("%s: unknown MODBUS I/O table"),table)
		);
	}

	switch( table_id )
	{
	case MODBUSTCP_TABLE_INPUT_COIL:
	case MODBUSTCP_TABLE_INPUT_REGISTER:
		io->attrs = EL_IO_INPUT;	/* input device only */
		break;
	}

	if ( !modbus_io_addr_parse(address,&addr,&cnt) )
	{
		EL_ERR_THROW(
			EL_ERR_INVAL,NULL,
			elStrTmpFmt(_T("%s: invalid MODBUS I/O address"),address)
		);
	}

	dio->table_id	= (BYTE) table_id;
	dio->addr		= addr;
	dio->cnt		= cnt;

	EL_IO_DEBUG_CODE(
		io->DebugLog(io,1,_T("%s(): %s:\n"),_T(__FUNC__),io->str.path->ptr);
		io->DebugLog(io,1,_T("  table_id: %d (%s), addr: %u, cnt: %u\n"),
			dio->table_id,modbus_io_table_id2str(dio->table_id),dio->addr,dio->cnt);
	);

	EL_ERR_CHECK( elWagoMbtDevAdd(io,host) );

	io->item_size.bits	= dio->cnt;
	io->item_size.bytes	= (dio->cnt + 7) / 8;
#if 1
	io->item_size.bpb	= MIN(io->item_size.bits,8);
#else
	io->item_size.bpb	= 1;
#endif

	EL_ERR_CHECK( elIoBufAlloc(&io->buf,io->item_size.bytes) );

Error:

	elDelete(&s);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoWagoMbtClose"
elErr_t *elIoWagoMbtClose(elIoParam_t iop)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;

	EL_ERR_CHECK( elWagoMbtDevRemove(io) );

	elDelete(&io->dio);

	;

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoWagoMbtRead"
elErr_t *elIoWagoMbtRead(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoWagoMbt_t		*dio = io->dio;
	elIoWagoMbtDev_t	*dev = dio->dev;
	mbt_errid_t			mbt_err;

	(void) bufsize;

	io->DebugLog(io,1,_T("%s(): MBTReadCoils(%s,%u,%u) ...\n"),
		_T(__FUNC__),modbus_io_table_id2str(dio->table_id),dio->addr,dio->cnt);

	EL_ERR_CHECK_COND( dev->socket == INVALID_HANDLE_VALUE, EL_ERR_INVAL,NULL );

	MBT_ERR_CHECK(
		MBTReadCoils(dev->socket,dio->table_id,dio->addr,dio->cnt,buf,NULL,0)
	);

	*plen = dio->cnt;

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoWagoMbtWrite"
elErr_t *elIoWagoMbtWrite(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoWagoMbt_t		*dio = io->dio;
	elIoWagoMbtDev_t	*dev = dio->dev;
	mbt_errid_t			mbt_err;

#if 0
	elMtxLock(&dio->mtx);
#endif

	(void) bufsize;

	io->DebugLog(io,1,_T("%s(): MBTWriteCoils(%u,%u) ...\n"),
		_T(__FUNC__),dio->addr,dio->cnt);

	EL_ERR_CHECK_COND( dev->socket == INVALID_HANDLE_VALUE, EL_ERR_INVAL,NULL );

	MBT_ERR_CHECK( MBTWriteCoils(dev->socket,dio->addr,dio->cnt,buf,NULL,0) );

	*plen = dio->cnt;

#if 0
	elMtxUnlock(&dio->mtx);
#endif

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoWagoMbtCtrl"
elErr_t *elIoWagoMbtCtrl(elIoParam_t iop,int cmd,void *pdata)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;

	(void) io;
	(void) cmd;
	(void) pdata;

	;

#if 0
Error:
#endif

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoWagoMbtTaskRead"
elErr_t *elIoWagoMbtTaskRead(
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
#define	__FUNC__	"elIoWagoMbtTaskWrite"
elErr_t *elIoWagoMbtTaskWrite(
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
#define	__FUNC__	"elIoWagoMbtTaskExecute"
elErr_t *elIoWagoMbtTaskExecute(elIoTask_t *task)
{
	EL_ERR_VAR;
	mbt_errid_t			mbt_err;
	elIo_t				*io;
	elIoWagoMbt_t		*dio;
	elIoWagoMbtDev_t	**dev;
	int					dev_cnt;
	int					cnt;
	BYTE				table_id;
	WORD				addr_min, addr_max;
	int					size;
	byte				*buf;
	register int		i, idev;

#if 0
	elLogD(_T("%s(): start"),_T(__FUNC__));
#endif

	buf = NULL;

	/* obtain list of unique WAGO devices: */
	dev = NULL;
	dev_cnt = 0;
	for(i = 0; i < task->io_cnt; i++)
	{
		io = task->io[i];

		if ( io->intfc != elIoWagoMbtIntfc )
			continue;

		dio = task->io[i]->dio;

		for(idev = 0; idev < dev_cnt; idev++)
			if ( dev[idev] == dio->dev )
				break;
		if ( idev == dev_cnt )
		{
			size = (1 + dev_cnt) * sizeof(*dev);
			EL_ERR_CHECK( elMemRealloc(&dev,size) );
			dev[idev] = dio->dev;
			dev_cnt++;
		}
	}

	table_id = (BYTE) -1;

	/* for each WAGO device: */
	for(idev = 0; idev < dev_cnt; idev++)
	{
		addr_min = (1 << (sizeof(addr_min) * 8)) - 1;
		addr_max = 0;

		for(i = 0; i < task->io_cnt; i++)
		{
			elIo_t *pio = task->io[i];

			if ( pio->intfc != elIoWagoMbtIntfc )
				continue;

			dio = pio->dio;
			if ( dio->dev != dev[idev] )
				continue;

			io = pio;

			table_id = dio->table_id;

			if ( dio->addr < addr_min )
				addr_min = dio->addr;
			if ( dio->addr > addr_max )
				addr_max = dio->addr;
			;
		}

#if 0
		elLogD(_T("%s(): addr_min: %u, addr_max: %u"),
			_T(__FUNC__),addr_min,addr_max);
#endif

		cnt = (addr_max - addr_min) + 1;
		EL_ERR_CHECK( elMemAlloc(&buf,(cnt + 7) / 8) );

		MBT_ERR_CHECK(
			MBTReadCoils(dev[idev]->socket,table_id,addr_min,(WORD)cnt,buf,NULL,0)
		);

		for(i = 0; i < task->io_cnt; i++)
		{
			register int bit;

			io = task->io[i];

			if ( io->intfc != elIoWagoMbtIntfc )
				continue;
			dio = io->dio;
			if ( dio->dev != dev[idev] )
				continue;

			bit = dio->addr - addr_min;
#if 0
			elLogD(_T("%s(): *io->buf.ptr = (buf[%d] >> %d) & 1"),
				__FUNC__,bit / 8,bit % 8);
#endif
			*(byte *)io->buf.ptr = (byte) ((buf[bit/8] >> (bit % 8)) & 1);
		}

		elMemFree(&buf);
	}

	;

Error:

	elMemFree(&buf);
	elMemFree(&dev);

	return err;
}

/******************************************************************************/
