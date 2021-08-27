/*
 *  "elIoNiDaq.c" - NI-DAQ I/O routines.
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
static char _ident[] = "@(#) elIoNiDaq.c 0.1 2007-03-05";
#else
#endif

#include <elBase.h>
#include <elObj.h>
#include <elMem.h>
#include <elIo.h>

#define EL_IO_NIDAQ_SIMULATED	1		/* 1: NI-DAQ devices are simulated using NI-DAQmx */

#define START_TASK_ONCE			1

#define EL_ERR_CHECK_DAQmx(fn_daqmx,errid,errstr)		\
		if ( DAQmxFailed(daqmx_err = (fn_daqmx)) )		\
		{												\
			EL_ERR_THROW(errid,#fn_daqmx,errstr);		\
		}

#define DAQmx_ERR_CHECK(fn_daqmx)	\
		EL_ERR_CHECK_DAQmx(fn_daqmx,EL_ERR_IO,daqmx_error_str(daqmx_err))

static elErr_t *elIoNiDaqCtor(elObjPtr_t *po,va_list args);
static elErr_t *elIoNiDaqDtor(elObjPtr_t *po);

static elObjClass_t gs_elIoNiDaq_c = {
	sizeof(elIoNiDaq_t),
	"elIoNiDaq",
	elIoNiDaqCtor,	/* Ctor()	*/
	elIoNiDaqDtor,	/* Dtor()	*/
	NULL,			/* Clone()	*/
	NULL,			/* Differ()	*/
};

const elObjClass_t *elIoNiDaq_c		= &gs_elIoNiDaq_c;

#define EL_IO_NIDAQ_METHODS									\
		elIoNiDaqOpen,				/* Open()			*/	\
		elIoNiDaqClose,				/* Close()			*/	\
		elIoNiDaqRead,				/* Read()			*/	\
		elIoNiDaqWrite,				/* Write()			*/	\
		NULL,						/* MsgRead()		*/	\
		NULL,						/* MsgWrite()		*/	\
		NULL,						/* Flush()			*/	\
		NULL,						/* Abort()			*/	\
		NULL,						/* AbortReset()		*/	\
		elIoNiDaqCtrl,				/* Ctrl()			*/	\
		NULL,						/* AttrSet()		*/	\
		NULL,						/* AttrGet()		*/	\
		NULL,						/* Lock()			*/	\
		NULL,						/* Unlock()			*/	\
		elIoNiDaqDevLock,			/* DevLock()		*/	\
		elIoNiDaqDevUnlock,			/* DevUnlock()		*/	\
		NULL,						/* DebugLog()		*/	\
		elIoNiDaqTaskRead,			/* TaskRead()		*/	\
		elIoNiDaqTaskWrite,			/* TaskWrite()		*/	\
		elIoNiDaqTaskExecute,		/* TaskExecute()	*/	\
		elIoNiDaqTaskDelete			/* TaskDelete()		*/	\

const elIoIntfc_t g_elIoNiDaqIntfc = {
	_T("NI-DAQ"),	EL_IO_NIDAQ_METHODS
};
const elIoIntfc_t *elIoNiDaqIntfc = &g_elIoNiDaqIntfc;

static struct {
	elMtx_t				mtx;

	/* table of all unique devices used in all I/O handles: */
	elIoNiDaqDev_t		**dev;
	int					dev_cnt;	/* # of valid devices in 'dev[]' */
	int					dev_acnt;	/* # of allocated elements in 'dev[]' */

} gs;

#if defined(_MSC_VER)
/* "warning C4505: 'libmb_force_multiple_coils' : unreferenced local function has been removed": */
#pragma warning(disable : 4505)
#endif

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"daqmx_error_str"
static TCHAR *daqmx_error_str(int32 daqmx_err)
{
	TCHAR		error_str[1024 + EL_ST];
	elStr_t		*str;

	DAQmxGetErrorString(daqmx_err,error_str,sizeof(error_str));

	str = elStrTmpIdxGet(EL_STR_IDX_ERR_USR1);
	if ( !str )
		return _T("<") _T(__FUNC__) _T("(): cannot get temporary string") _T(">");

	str->Cpy(str,error_str);

	return str->ptr;
}

#if 0
/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"daqmx_error_log"
static void daqmx_error_log(elIo_t *io,TCHAR *func,int32 daqmx_err)
{
	TCHAR		error_str[1024 + EL_ST];
	TCHAR		ext_error_str[1024 + EL_ST];
	elLog_t		*log = io->log;

	DAQmxGetErrorString(daqmx_err,error_str,sizeof(error_str));
	DAQmxGetExtendedErrorInfo(ext_error_str,sizeof(ext_error_str));

	log->Printf(log,_T("%sDAQmx error: %ld\n"),
		func ? elStrTmpFmt(_T("%s(): "),func) : _T(""),daqmx_err);
	log->Printf(log,_T("------ Error string: ------\n"));
	log->Printf(log,_T("%s\n"),error_str);
	log->Printf(log,_T("---------------------------\n"));
	log->Printf(log,_T("------ Extended error string: ------\n"));
	log->Printf(log,_T("%s\n"),ext_error_str);
	log->Printf(log,_T("---------------------------\n"));
}
#endif

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elNiDaqDevNew"
static elErr_t *elNiDaqDevNew(elIoNiDaqDev_t **p_dev)
{
	EL_ERR_VAR;

	EL_ERR_CHECK( elMemAllocZero(p_dev,sizeof(**p_dev)) );

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elNiDaqDevDelete"
static elErr_t *elNiDaqDevDelete(elIoNiDaqDev_t **p_dev)
{
	EL_ERR_VAR;

	elMemFree(p_dev);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoNiDaqCtor"
static elErr_t *elIoNiDaqCtor(elObjPtr_t *po,va_list args)
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
#define	__FUNC__	"elIoNiDaqDtor"
static elErr_t *elIoNiDaqDtor(elObjPtr_t *po)
{
	EL_ERR_VAR;
	elIo_t *io = (elIo_t *)*po;

	(void) io;

	;

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoNiDaqOpen"
elErr_t *elIoNiDaqOpen(elIoParam_t iop,TCHAR *rsrc,...)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
#if !EL_IO_SIMULATE || EL_IO_NIDAQ_SIMULATED
	int32			daqmx_err, daqmx_err_di, daqmx_err_do;
#endif
	elIoNiDaq_t		*dio;
	elIoNiDaqDev_t	*dev;
	elStr_t			*str;
	char			*chan_name;
	char			*device;
#if !EL_IO_SIMULATE || EL_IO_NIDAQ_SIMULATED
	char			*daq_device = NULL;
	char			product_type[1024+EL_ST];
	TaskHandle		task_di, task_do;
	char			data[4096];
	char			*task_name;
	char			*p;
	uInt32			cnt, chan_cnt, chan_line_cnt;
	int32			chan_type_di, chan_type_do;
#endif

	(void) rsrc;

	EL_ERR_CHECK( elNew(elIoNiDaq_c,&io->dio) );
	EL_ERR_CHECK( elNew(elStr_c,&str) );
	
	dio = io->dio;
	dio->io = io;
	EL_ERR_CHECK( elNiDaqDevNew(&dio->dev) );
	dev = dio->dev;

#if 1
	chan_name = NULL;
#else
	chan_name = "vchan";
#endif

	/*
	 * Create task with digital input or digital output channel
	 * (we don't know whether specified device/line is input or output):
	 * -- { --
	 */

	device = io->str.path->ptr;

	/*
	 * Create both task w/ input channel and task w/ output channel
	 * with the same device name:
	 */
#if EL_IO_SIMULATE && !EL_IO_NIDAQ_SIMULATED
	dev->line_cnt = 1;
#else
#if 0
	str->Printf(str,_T("%s_DI"),rsrc);
	task_name = str->ptr;
#else
	task_name = NULL;
#endif

	DAQmx_ERR_CHECK( DAQmxCreateTask(task_name,&task_di) );
#if 0
	str->Printf(str,_T("%s_DO"),rsrc);
	task_name = str->ptr;
#else
	task_name = NULL;
#endif
	DAQmx_ERR_CHECK( DAQmxCreateTask(task_name,&task_do) );

	DAQmx_ERR_CHECK(
		DAQmxCreateDIChan(task_di,device,chan_name,DAQmx_Val_ChanPerLine)
	);
	DAQmx_ERR_CHECK(
		DAQmxCreateDOChan(task_do,device,chan_name,DAQmx_Val_ChanPerLine)
	);
	/*
	 * Attempt to obtain channel type (this will be successful
	 * for only one task):
	 */
	daqmx_err_di = DAQmxGetChanType(task_di,device,&chan_type_di);
	daqmx_err_do = DAQmxGetChanType(task_do,device,&chan_type_do);
	if ( DAQmxFailed(daqmx_err_di) )
	{
		DAQmxClearTask(task_di);
		dev->chan_type = chan_type_do;
		dev->task.handle = task_do;
		io->attrs = EL_IO_OUTPUT;	/* output device only */
	}
	else
	{
		DAQmxClearTask(task_do);
		dev->chan_type = chan_type_di;
		dev->task.handle = task_di;
		io->attrs = EL_IO_INPUT;	/* input device only */
	}

	/*
	 * -- } --
	 */

	/*
	 * Obtain device name without channel/line name:
	 */
	daqmx_err = DAQmxGetDevProductType(device,product_type,sizeof(product_type));
	if ( DAQmxFailed(daqmx_err) )
	{	/* 'device' is not device itself, it is probably a channel(s) (e.g. "Dev1/line4"): */
		DAQmx_ERR_CHECK( DAQmxGetTaskDevices(dev->task.handle,data,sizeof(data)) );
		p = data;	/* e.g. "Dev1" */
	}
	else
	{	/* 'device' is device itself (e.g. "Dev1"): */
		p = device;
	}
	
	EL_ERR_CHECK( elMemStrdup(&daq_device,p) );

	EL_ERR_CHECK( elMemStrdup(&dev->name,device) );
	EL_ERR_CHECK( elMemStrdup(&dev->base_name,daq_device) );

	/* obtain serial number (0: simulated device): */
	DAQmx_ERR_CHECK( DAQmxGetDevSerialNum(daq_device,&dev->serial_num) );
	
	DAQmx_ERR_CHECK( DAQmxGetTaskName(dev->task.handle,data,sizeof(data)) );
	io->DebugLog(io,1,_T("DAQmxGetTaskName: %s\n"),data);

	DAQmx_ERR_CHECK( DAQmxGetTaskChannels(dev->task.handle,data,sizeof(data)) );
	io->DebugLog(io,1,_T("DAQmxGetTaskChannels: %s\n"),data);

	dev->line_min = (int) (((uint64_t)1 << (sizeof(dev->line_min) * 8)) - 1);
	dev->line_max = 0;
	for(p = data; (p = strtok(p,",")) != NULL; p = NULL)
	{
		int	line;

		/* "Dev?/Line?": */
		sscanf(p,"%*[^0-9]%*[0-9]/%*[^0-9]%d",&line);
		if ( (uint)line < (uint)dev->line_min )
			dev->line_min = line;
		if ( line > dev->line_max )
			dev->line_max = line;
	}

	DAQmx_ERR_CHECK( DAQmxGetTaskNumChans(dev->task.handle,&chan_cnt) );
	io->DebugLog(io,1,_T("DAQmxGetTaskNumChans: %ld\n"),chan_cnt);

	DAQmx_ERR_CHECK( DAQmxGetTaskDevices(dev->task.handle,data,sizeof(data)) );
	io->DebugLog(io,1,_T("DAQmxGetTaskDevices: %s\n"),data);

	DAQmx_ERR_CHECK( DAQmxGetTaskNumDevices(dev->task.handle,&cnt) );
	io->DebugLog(io,1,_T("DAQmxGetTaskNumDevices: %ld\n"),cnt);

	switch( dev->chan_type )
	{
	case DAQmx_Val_DI:
		DAQmx_ERR_CHECK( DAQmxGetDINumLines(dev->task.handle,chan_name,&chan_line_cnt) );
		io->DebugLog(io,1,_T("DAQmxGetDINumLines(%s): %ld\n"),chan_name,chan_line_cnt);
		break;

	case DAQmx_Val_DO:
		DAQmx_ERR_CHECK( DAQmxGetDONumLines(dev->task.handle,chan_name,&chan_line_cnt) );
		io->DebugLog(io,1,_T("DAQmxGetDONumLines(%s): %ld\n"),chan_name,chan_line_cnt);
		break;

	default:
		chan_line_cnt = 0;
		break;

	}

	dev->chan_cnt		= chan_cnt;
	dev->chan_line_cnt	= chan_line_cnt;
	dev->line_cnt		= chan_cnt * chan_line_cnt;

	EL_ERR_CHECK( elMtxNew(&dev->lock,NULL) );

#endif

	io->item_size.bits	= dev->line_cnt;
	if(io->item_size.bits>1)
		io->attrs |= EL_IO_GROUP;

	io->item_size.bytes	= (dev->line_cnt + 7) / 8;
#if 1
	io->item_size.bpb	= MIN(io->item_size.bits,8);
#else
	io->item_size.bpb	= 1;
#endif

	EL_ERR_CHECK( elIoBufAlloc(&io->buf,io->item_size.bytes) );
	io->buf.item_size = io->item_size;

Error:

#if !EL_IO_SIMULATE
	elMemFree(&daq_device);
#endif

	elDelete(&str);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoNiDaqClose"
elErr_t *elIoNiDaqClose(elIoParam_t iop)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoNiDaq_t		*dio = io->dio;
	elIoNiDaqDev_t	*dev = dio->dev;

	if ( dev->task.run )
	{
		DAQmxStopTask(dev->task.handle);
		dev->task.run = FALSE;
	}

	DAQmxClearTask(dev->task.handle);

	elMemFree(&dev->name);
	elMemFree(&dev->base_name);

	EL_ERR_CHECK( elMtxDelete(&dev->lock) );

	EL_ERR_CHECK( elNiDaqDevDelete(&dio->dev) );

	elDelete(&io->dio);

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoNiDaqRead"
elErr_t *elIoNiDaqRead(
	elIoParam_t	iop,
	elByte_t	*buf,		/* buffer for data items */
	elUInt32_t	cnt,		/* number of data items to read */
	elUInt32_t	*p_cnt		/* actual number of data items read */
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;

	elTimerStart(&io->timer);

	{	/* inner block */

#if EL_IO_SIMULATE

	(void) iop;

	*buf = 0;
	*p_cnt = cnt;

#else

	int32			daqmx_err;
	elIoNiDaq_t		*dio = io->dio;
	elIoNiDaqDev_t	*dev = dio->dev;
	int32			len, bytes_per_samp;
	int				bit_cnt;
	byte			*pbuf;
	register int	i;

	if ( !dev->serial_num )		/* simulated device ? */
	{
		/* zero data instead of random data provided by NI-DAQmx: */
		memset(buf,0,cnt);
		*p_cnt = cnt;
		return NULL;
	}
		
	EL_ERR_CHECK( elMtxLock(&dev->lock) );

#if START_TASK_ONCE
	if ( !dev->task.run )
	{
		DAQmx_ERR_CHECK( DAQmxStartTask(dev->task.handle) );
		dev->task.run = TRUE;
	}
#else
	DAQmx_ERR_CHECK( DAQmxStartTask(dev->task.handle) );
	dev->task.run = TRUE;
#endif

	if ( cnt <= 0 )
		cnt = 1;

	if ( io->item_size.bpb == 1 )
		pbuf = buf;
	else
		/* handle multiple digital input lines: */
		pbuf = io->buf.ptr;

	*p_cnt = 0;
	while( cnt-- )
	{
		bit_cnt = io->item_size.bits;
		io->DebugLog(io,1,_T("%s(): DAQmxReadDigitalLines() ...\n"),_T(__FUNC__));
		/*
		 * WARNING: 'DAQmxReadDigitalLines()' is going to clear the buffer before
		 * reading data:
		 */
		DAQmx_ERR_CHECK(
			DAQmxReadDigitalLines(
				dev->task.handle,1,5.0,
				DAQmx_Val_GroupByChannel,
				pbuf,
				bit_cnt,
				&len,
				&bytes_per_samp,NULL
			)
		);

		if ( io->item_size.bpb != 1 )
		{
			*buf = 0;
			for(i = 0; i < bit_cnt; i++)
				*buf |= (pbuf[i] ? 1 : 0) << (bit_cnt - i - 1);
		}

		buf += io->item_size.bytes;
		*p_cnt++;
	}

Error:

#if START_TASK_ONCE
#else
	if ( dev->task.run )
	{
		DAQmxStopTask(dev->task.handle);
		dev->task.run = FALSE;
	}
#endif

	EL_ERR_CHECK( elMtxUnlock(&dev->lock) );

#endif

	}	/* inner block */

	elTimerStop(&io->timer);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoNiDaqWrite"
elErr_t *elIoNiDaqWrite(
	elIoParam_t	iop,
	elByte_t	*buf,		/* buffer for data items */
	elUInt32_t	cnt,		/* number of data items to write */
	elUInt32_t	*p_cnt		/* actual number of data items written */
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;

	elTimerStart(&io->timer);

	{	/* inner block */

#if EL_IO_SIMULATE

	(void) iop;
	(void) buf;

	*p_cnt = cnt;

#else

	int32			daqmx_err;
	elIoNiDaq_t		*dio = io->dio;
	elIoNiDaqDev_t	*dev = dio->dev;
	int				bit_cnt;
	byte			*pbuf;
	register int	i;

	EL_ERR_CHECK( elMtxLock(&dev->lock) );

#if START_TASK_ONCE
	if ( !dev->task.run )
	{
		DAQmx_ERR_CHECK( DAQmxStartTask(dev->task.handle) );
		dev->task.run = TRUE;
	}
#else
	DAQmx_ERR_CHECK( DAQmxStartTask(dev->task.handle) );
	dev->task.run = TRUE;
#endif

	if ( cnt <= 0 )
		cnt = 1;

	/*
	 * NI-DAQmx does not permit writing digital output lines with values that are larger
	 * than number of lines can accommodate (e.g. writing of 0xFF to single digital output
	 * line is not allowed). Therefore, we have to truncate the passed value in such case.
	 */
	if ( io->item_size.bpb == 1 && *buf <= 1 )
		pbuf = buf;
	else
		pbuf = io->buf.ptr;

	*p_cnt = 0;
	while( cnt-- )
	{
		bit_cnt = io->item_size.bits;

		if ( io->item_size.bpb != 1 )
		{
			for(i = 0; i < bit_cnt; i++)
				pbuf[i] = (elByte_t) (((*buf) >> (io->item_size.bits - i - 1)) & 1);
		}
		else if ( *buf > 1 )
			*pbuf = (elByte_t) (*buf & 1);
			

		io->DebugLog(io,1,_T("%s(): DAQmxWriteDigitalLines() ...\n"),_T(__FUNC__));

		DAQmx_ERR_CHECK(
			DAQmxWriteDigitalLines(
				dev->task.handle,1,1,5.0,
				DAQmx_Val_GroupByChannel,
				pbuf,NULL,NULL
			)
		);

		buf += io->item_size.bytes;
		*p_cnt++;
	}

Error:

#if START_TASK_ONCE
#else
	if ( dev->task.run )
	{
		DAQmxStopTask(dev->task.handle);
		dev->task.run = FALSE;
	}
#endif

	EL_ERR_CHECK( elMtxUnlock(&dev->lock) );

#endif

	}	/* inner block */

	elTimerStop(&io->timer);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoNiDaqCtrl"
elErr_t *elIoNiDaqCtrl(elIoParam_t iop,int cmd,void *pdata)
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
#define	__FUNC__	"elIoNiDaqDevLock"
elErr_t *elIoNiDaqDevLock(elIoParam_t iop)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoNiDaq_t			*dio = io->dio;
	elIoNiDaqDev_t		*dev = dio->dev;

	elMtxLock(&dev->lock);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoNiDaqDevUnlock"
elErr_t *elIoNiDaqDevUnlock(elIoParam_t iop)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoNiDaq_t			*dio = io->dio;
	elIoNiDaqDev_t		*dev = dio->dev;

	elMtxUnlock(&dev->lock);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoNiDaqTaskRead"
elErr_t *elIoNiDaqTaskRead(
	elIoTask_t *task,
	elIoParam_t iop,
	elByte_t	*buf,		/* buffer for data items */
	elUInt32_t	cnt,		/* number of data items to read */
	elUInt32_t	*p_cnt		/* actual number of data items read */
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoTaskRW_t		*task_rw;
	elIoNiDaq_t			*dio;
	elIoTaskIntfc_t		*intfc;
	elIoTaskDev_t		*tdev;
	elIoNiDaqTaskDev_t	*idev;
	elIoNiDaqDev_t		*dev;
	int32				daqmx_err;
	elUInt32_t			size;
	register int		i;

	(void) buf;
	(void) cnt;
	(void) p_cnt;

	task_rw = &task->rw[EL_IO_TASK_READ];

	/* get index of the I/O interface in the task: */
	for(i = 0; i < task_rw->intfc_cnt; i++)
		if ( task_rw->intfc[i].ptr == io->intfc )
			break;
	EL_ERR_ASSERT( i < task_rw->intfc_cnt );

	/*
	 * Add NI-DAQ device to the list of unique devices (if not present yet):
	 */
	dio		= io->dio;
	intfc	= &task_rw->intfc[i];
	tdev	= NULL;
	idev	= NULL;
	dev		= NULL;
	for(i = 0; i < intfc->dev_cnt; i++)
	{
		tdev = &intfc->dev[i];
		idev = tdev->data;
		dev = idev->ptr;
		if ( !strcmp(dev->base_name,dio->dev->base_name) )
			break;	/* device already in list */
	}
	if ( i == intfc->dev_cnt )
	{
		size = (1 + intfc->dev_cnt) * sizeof(*intfc->dev);
		EL_ERR_CHECK( elMemRealloc(&intfc->dev,size) );
		intfc->dev_cnt++;
		tdev = &intfc->dev[i];
		memset(tdev,0,sizeof(*tdev));
		EL_ERR_CHECK( elIoBufInit(&tdev->io_buf) );
		EL_ERR_CHECK( elIoBufInit(&tdev->buf) );

		/* allocate interface-specific device's data: */
		EL_ERR_CHECK( elMemAllocZero(&tdev->data,sizeof(*idev)) );
		idev = tdev->data;

		/* create new device: */
		EL_ERR_CHECK( elNiDaqDevNew(&idev->ptr) );
		dev = idev->ptr;
		EL_ERR_CHECK( elMemStrdup(&dev->base_name,dio->dev->base_name) );
		EL_ERR_CHECK( elMemStrdup(&dev->name,dio->dev->name) );

		dev->line_min = (int) (((uint64_t)1 << (sizeof(dev->line_min) * 8)) - 1);
		dev->line_max = 0;
	}

	if ( (uint)dio->dev->line_min < (uint)dev->line_min )
		dev->line_min = dio->dev->line_min;
	if ( dio->dev->line_max > dev->line_max )
		dev->line_max = dio->dev->line_max;
	dev->line_cnt = ((dev->line_max - dev->line_min) + 1);

	/*
	 * Set device name (e.g. "Dev1/line0:20"):
	 */
	elMemFree(&tdev->name);
	EL_ERR_CHECK(
		elMemStrdup(
			&tdev->name,
			elStrTmpFmt("%s/Line%d:%d",
				dev->base_name,dev->line_min,dev->line_max)
		)
	);

#if 0
	/* stop NI-DAQ I/O task: */
	DAQmxStopTask(dev->task.handle);
#endif

	/*
	 * Recreate NI-DAQ task handle to include new DIO line(s):
	 */
	DAQmxClearTask(dev->task.handle);
	DAQmx_ERR_CHECK( DAQmxCreateTask(NULL,&dev->task.handle) );
	DAQmx_ERR_CHECK(
		DAQmxCreateDIChan(dev->task.handle,tdev->name,NULL,DAQmx_Val_ChanPerLine)
	);

#if 0
	/* start NI-DAQ I/O task: */
	DAQmx_ERR_CHECK( DAQmxStartTask(dev->task.handle) );
	dev->task.run = TRUE;
#endif

	/* reallocate device I/O buffer: */
	tdev->buf.size = dev->line_cnt;
	EL_ERR_CHECK( elIoBufAlloc(&tdev->buf,tdev->buf.size) );

	/* add I/O pointer to the device's I/O list: */
	EL_ERR_CHECK(
		elMemRealloc(&tdev->io,(1 + tdev->io_cnt) * sizeof(*tdev->io))
	);
	tdev->io[tdev->io_cnt++] = io;

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoNiDaqTaskWrite"
elErr_t *elIoNiDaqTaskWrite(
	elIoTask_t *task,
	elIoParam_t iop,
	elByte_t	*buf,		/* buffer for data items */
	elUInt32_t	cnt,		/* number of data items to write */
	elUInt32_t	*p_cnt		/* actual number of data items written */
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoTaskRW_t		*task_rw;
	elIoNiDaq_t			*dio;
	elIoTaskIntfc_t		*intfc;
	elIoTaskDev_t		*tdev;
	elIoNiDaqTaskDev_t	*idev;
	elIoNiDaqDev_t		*dev;
	int32				daqmx_err;
	elUInt32_t			size;
	register int		i;

	(void) buf;
	(void) cnt;
	(void) p_cnt;

	task_rw = &task->rw[EL_IO_TASK_WRITE];

	/* get index of the I/O interface in the task: */
	for(i = 0; i < task_rw->intfc_cnt; i++)
		if ( task_rw->intfc[i].ptr == io->intfc )
			break;
	EL_ERR_ASSERT( i < task_rw->intfc_cnt );

	/*
	 * Add NI-DAQ device to the list of unique devices (if not present yet):
	 */
	dio		= io->dio;
	intfc	= &task_rw->intfc[i];
	tdev	= NULL;
	idev	= NULL;
	dev		= NULL;
	for(i = 0; i < intfc->dev_cnt; i++)
	{
		tdev = &intfc->dev[i];
		idev = tdev->data;
		dev = idev->ptr;
		if ( !strcmp(dev->base_name,dio->dev->base_name) )
			break;	/* device already in list */
	}
	if ( i == intfc->dev_cnt )
	{
		size = (1 + intfc->dev_cnt) * sizeof(*intfc->dev);
		EL_ERR_CHECK( elMemRealloc(&intfc->dev,size) );
		intfc->dev_cnt++;
		tdev = &intfc->dev[i];
		memset(tdev,0,sizeof(*tdev));
		EL_ERR_CHECK( elIoBufInit(&tdev->buf) );

		/* allocate interface-specific device's data: */
		EL_ERR_CHECK( elMemAllocZero(&tdev->data,sizeof(*idev)) );
		idev = tdev->data;

		/* create new device: */
		EL_ERR_CHECK( elNiDaqDevNew(&idev->ptr) );
		dev = idev->ptr;
		EL_ERR_CHECK( elMemStrdup(&dev->base_name,dio->dev->base_name) );
		EL_ERR_CHECK( elMemStrdup(&dev->name,dio->dev->name) );

		dev->line_min = (int) (((uint64_t)1 << (sizeof(dev->line_min) * 8)) - 1);
		dev->line_max = 0;
	}

	if ( (uint)dio->dev->line_min < (uint)dev->line_min )
		dev->line_min = dio->dev->line_min;
	if ( dio->dev->line_max > dev->line_max )
		dev->line_max = dio->dev->line_max;
	dev->line_cnt = ((dev->line_max - dev->line_min) + 1);

	/*
	 * Set device name (e.g. "Dev1/line0:20"):
	 */
	elMemFree(&tdev->name);
	EL_ERR_CHECK(
		elMemStrdup(
			&tdev->name,
			elStrTmpFmt("%s/Line%d:%d",
				dev->base_name,dev->line_min,dev->line_max)
		)
	);

#if 0
	/* stop NI-DAQ I/O task: */
	DAQmxStopTask(dev->task.handle);
#endif

	/*
	 * Recreate NI-DAQ task handle to include new DIO line(s):
	 */
	DAQmxClearTask(dev->task.handle);
	DAQmx_ERR_CHECK( DAQmxCreateTask(NULL,&dev->task.handle) );
	DAQmx_ERR_CHECK(
		DAQmxCreateDOChan(dev->task.handle,tdev->name,NULL,DAQmx_Val_ChanPerLine)
	);

#if 0
	/* start NI-DAQ I/O task: */
	DAQmx_ERR_CHECK( DAQmxStartTask(dev->task.handle) );
	dev->task.run = TRUE;
#endif

	/*
	 * Reallocate output I/O buffer shared by all I/O resources
	 * in this task device:
	 */
	tdev->io_buf.size = dev->line_cnt;
	EL_ERR_CHECK( elIoBufAlloc(&tdev->io_buf,tdev->io_buf.size) );

	/*
	 * Reallocate output device I/O buffer:
	 */
	tdev->buf.size = dev->line_cnt;
	EL_ERR_CHECK( elIoBufAlloc(&tdev->buf,tdev->buf.size) );

	/* add I/O pointer to the device's I/O list: */
	EL_ERR_CHECK(
		elMemRealloc(&tdev->io,(1 + tdev->io_cnt) * sizeof(*tdev->io))
	);
	tdev->io[tdev->io_cnt++] = io;

	/*
	 * Set I/O buffer pointer of all I/O resources to the shared task device
	 * I/O buffer so that I/O resources that share the same DIO line will
	 * update/overwrite the same data space. This is important in case
	 * the data are written to memory (I/O buffer) in particular order first
	 * and then sent to the device at once from the same memory location.
	 * This way we can assure the final value of the shared DIO lines
	 * that gets written to the device. Example of I/O resources that
	 * share the same DIO line(s):
	 *
	 *		WS2_MPD_light_green		NI-DAQ:Dev4/line16
	 *		WS2_MPD_light_orange	NI-DAQ:Dev4/line16:17
	 */
	for(i = 0; i < tdev->io_cnt; i++)
	{
		io = tdev->io[i];
		dio = io->dio;
		io->buf.ptr = tdev->io_buf.ptr + (dio->dev->line_min - dev->line_min);
	}

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoNiDaqTaskExecute"
elErr_t *elIoNiDaqTaskExecute(elIoTask_t *task)
{
#define USE_TASK_BUF	1
	EL_ERR_VAR;
	elIo_t			*io;
	register int	i, dev_idx;

#if 0
	elLogD(_T("%s(): start"),_T(__FUNC__));
#endif

#if 1

	{	/* inner block */
	elIoTaskRW_t		*task_rw;
	elIoTaskIntfc_t		*intfc;
	elIoTaskDev_t		*tdev;
	elIoNiDaqTaskDev_t	*idev;
	elIoNiDaq_t			*dio;
	elIoNiDaqDev_t		*dev;
	int32				daqmx_err;
	int32				len;
	int					bit_cnt;

	(void) len;

	switch( task->fn )
	{
	case EL_IO_TASK_FN_READ:
	case EL_IO_TASK_FN_WRITE:

		task_rw = &task->rw[task->fn - EL_IO_TASK_FN_READ];

		for(i = 0; i < task_rw->intfc_cnt; i++)
			if ( task_rw->intfc[i].ptr == elIoNiDaqIntfc )
				break;
		EL_ERR_ASSERT( i < task_rw->intfc_cnt );
		intfc = &task_rw->intfc[i];

		/* for each NI-DAQ device / task handle: */
		for(dev_idx = 0; dev_idx < intfc->dev_cnt; dev_idx++)
		{
			tdev = &intfc->dev[dev_idx];
			idev = tdev->data;
			dev  = idev->ptr;

			/* start NI-DAQ I/O task (if not started yet): */
			if ( !dev->task.run )
			{
				DAQmx_ERR_CHECK( DAQmxStartTask(dev->task.handle) );
				dev->task.run = TRUE;
			}

			elTimerStart(&tdev->t_val);
			tdev->t_stamp = tdev->t_val;

			bit_cnt = dev->line_cnt;

			switch( task->fn )
			{
			case EL_IO_TASK_FN_READ:
#if EL_IO_SIMULATE
				memset(tdev->buf.ptr,0,bit_cnt);
#else
				{	/* inner block */
				int32	bytes_per_samp;

				DAQmx_ERR_CHECK(
					DAQmxReadDigitalLines(
						dev->task.handle,1,5.0,
						DAQmx_Val_GroupByChannel,
						tdev->buf.ptr,
						bit_cnt,
						&len,
						&bytes_per_samp,NULL
					)
				);
				}	/* inner block */
#endif

				/*
				 * Copy data (that have been read from NI-DAQ device)
				 * from device I/O buffer to user I/O buffer:
				 */
				for(i = 0; i < tdev->io_cnt; i++)
				{
					byte	*buf1;
					byte	*buf2;

					io = tdev->io[i];
					dio = io->dio;

					/* source buffer */
					buf1 = tdev->buf.ptr + (dio->dev->line_min - dev->line_min);
					/* destination buffer */
					buf2 = io->buf.ptr;

					if ( io->item_size.bpb != 1 )
					{
						register int	j;
						register int	bit_cnt = io->item_size.bits;
						register byte	val;

						val = 0;
						for(j = 0; j < bit_cnt; j++)
							val |= (buf1[j] ? 1 : 0) << (bit_cnt - j - 1);
						*buf2 = val;
					}
					else
						memcpy(buf2,buf1,io->item_size.bits);
				}
				break;

			case EL_IO_TASK_FN_WRITE:
				/*
				 * Copy data from user I/O buffer to device I/O buffer
				 * (to be written to NI-DAQ device):
				 */
				for(i = 0; i < tdev->io_cnt; i++)
				{
					io = tdev->io[i];
					dio = io->dio;
#if 1
					{	/* inner block */
					register int	j;

					/*
					 * Copy only one bit to avoid the following NI-DAQ error:
					 *
					 *	"Attempted writing digital data that is not supported."
					 */
					for(j = 0; j < (int)io->item_size.bits; j++)
					{
						tdev->buf.ptr[dio->dev->line_min - dev->line_min + j] =
#if 1
							EL_IO_BUF_ITEM_GET_BIT(&io->buf,j);
#else
							(byte) (((*io->buf.ptr) >> j) & 0x01);
#endif
					}
					}	/* inner block */
#else
					memcpy(
						tdev->buf.ptr + (dio->dev->line_min - dev->line_min),
						io->buf.ptr,
						io->item_size.bits
					);
#endif
				}

				daqmx_err = DAQmxWriteDigitalLines(
					dev->task.handle,
					1,							/* numSampsPerChan */
					1,							/* autoStart */
					5.0,						/* timeout */
					DAQmx_Val_GroupByChannel,
					tdev->buf.ptr,NULL,NULL
				);
				if ( DAQmxFailed(daqmx_err) )
				{
					elLog("%s(): %s: DAQmxWriteDigitalLines() failed",
						__FUNC__,tdev->name);
					EL_ERR_THROW(EL_ERR_IO,"DAQmxWriteDigitalLines()",
						daqmx_error_str(daqmx_err));
				}
				break;

			}	/* switch( task->fn ) */

			elTimerStop(&tdev->t_val);
		}

	}	/* switch( task->fn ) */

	}	/* inner block */

#else

	{	/* inner block */
	elErr_t			*io_err;

	for(i = 0; i < task->io_cnt; i++)
	{
		io = task->io[i];

		if ( io->intfc != elIoNiDaqIntfc )
			continue;

		/*
		 * WARNING: 'DAQmxReadDigitalLines()' (called in 'elIoNiDaqRead()') is going
		 * to clear the buffer before reading data.
		 *
		 * If there is other thread accessing the same buffer at the same time,
		 * it can read invalid (zero) data. To avoid this problem, data
		 * are read to separate temporary buffer.
		 */
#if USE_TASK_BUF
		io_err = elIoNiDaqRead(io,task->buf.ptr,io->item_size.bytes,&len);
#else
		io_err = elIoNiDaqRead(io,io->buf.ptr,io->item_size.bytes,&len);
#endif
		if ( io_err )
		{
			/* save error: */
			EL_ERR_CHECK( elMemRealloc(&io->err,sizeof(*io->err)) );
			*io->err = *io_err;
		}
		else
		{
			elMemFree(&io->err);
#if USE_TASK_BUF
			/* copy read data from temporary buffer to requested buffer: */
			memcpy(io->buf.ptr,task->buf.ptr,io->item_size.bytes);
#endif
		}
	}
	}	/* inner block */

#endif

#if 0
	elLogD(_T("%s(): end"),_T(__FUNC__));
#endif

	;

Error:

	return err;

#undef USE_TASK_BUF
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoNiDaqTaskDelete"
elErr_t *elIoNiDaqTaskDelete(elIoTask_t *task)
{
	EL_ERR_VAR;
	elIoTaskRW_t		*task_rw;
	elIoTaskIntfc_t		*intfc;
	elIoTaskDev_t		*tdev;
	elIoNiDaqTaskDev_t	*idev;
	elIoNiDaqDev_t		*dev;
	register int		i, j, irw;

	for(irw = 0; irw < EL_ARRAY_CNT(task->rw); irw++)
	{
		task_rw = &task->rw[irw];
		for(i = 0; i < task_rw->intfc_cnt; i++)
		{
			intfc = &task_rw->intfc[i];
			for(j = 0; j < intfc->dev_cnt; j++)
			{
				tdev = &intfc->dev[j];
				idev = tdev->data;
				dev  = idev->ptr;

				if ( dev->task.run )
				{
					DAQmxStopTask(dev->task.handle);
					dev->task.run = FALSE;
				}
				DAQmxClearTask(dev->task.handle);

				elMemFree(&tdev->data);

				elIoBufFree(&tdev->io_buf);
				elIoBufFree(&tdev->buf);
			}
		}
	}

	return err;
}

/******************************************************************************/
