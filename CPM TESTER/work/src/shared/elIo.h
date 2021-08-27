/*
 *  <elIo.h> - Definitions for module "elIo.c".
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

#if !defined(_EL_IO_H)
#define _EL_IO_H

#if _MSC_VER < 1400 && !__BCOPT__ && !_CVI_  /* Borland C/C++: Unknown preprocessor directive: 'ident' */
#if 1
#ident "@(#) elIo.h 0.1 2007-03-05"
#else
#endif
#endif

#include <elBase.h>
#include <elObj.h>
#include <elMtx.h>
#include <elErr.h>
#include <elLog.h>
#include <elTimer.h>

#define EL_IO_SIMULATE		0

struct SelIo;
typedef struct SelIo elIo_t, *elPIo_t, *elIoPtr_t;
struct SelIoTask;
typedef struct SelIoTask	elIoTask_t, *elPIoTask_t, *elIoTaskPtr_t;
typedef struct SelIoTaskRW	elIoTaskRW_t, *elPIoTaskRW_t;

#if EL_POBJ_PARAM_IS_HANDLE
typedef elObjHandle_t		elIoParam_t;
#define EL_IO_PARAM_VAR		elIo_t *io = (elIo_t *) elObjPtrH(iop)
#else
typedef elIo_t *			elIoParam_t;
#define EL_IO_PARAM_VAR		elIo_t *io = iop
#endif

#define EL_IO_METHODS																\
	elErr_t		*(*Open) (															\
		elIoParam_t iop,TCHAR *rsrc,...												\
	);																				\
	elErr_t		*(*Close) (elIoParam_t io);											\
	elErr_t		*(*Read) (															\
		elIoParam_t iop,elByte_t *buf,elUInt32_t cnt,elUInt32_t *p_cnt				\
	);																				\
	elErr_t		*(*Write) (															\
		elIoParam_t iop,elByte_t *buf,elUInt32_t cnt,elUInt32_t *p_cnt				\
	);																				\
	elErr_t		*(*MsgRead) (														\
		elIoParam_t iop,elByte_t *msg,elUInt32_t msgsize,elUInt32_t *plen			\
	);																				\
	elErr_t		*(*MsgWrite) (														\
		elIoParam_t iop,elByte_t *msg,elUInt32_t msgsize,elUInt32_t *plen			\
	);																				\
	elErr_t		*(*Flush)(elIoParam_t iop,elUInt32_t io_mode);						\
	elErr_t		*(*Abort)(elIoParam_t iop);											\
	elErr_t		*(*AbortReset)(elIoParam_t iop);									\
	elErr_t		*(*Ctrl)(elIoParam_t iop,int cmd,void *pdata);						\
	elErr_t		*(*AttrSet)(elIoParam_t iop,TCHAR *attr_name,TCHAR *attr_val);		\
	elErr_t		*(*AttrGet)(elIoParam_t iop,TCHAR *attr_name,TCHAR **p_attr_val);	\
	elErr_t		*(*Lock)(elIoParam_t iop);											\
	elErr_t		*(*Unlock)(elIoParam_t iop);										\
	elErr_t		*(*DevLock)(elIoParam_t iop);										\
	elErr_t		*(*DevUnlock)(elIoParam_t iop);										\
	elErr_t		*(*DebugLog)(elIoParam_t iop,int debug_level_min,					\
					const TCHAR *fmt,...)										
																					
#define EL_IO_TASK_METHODS															\
	elErr_t		*(*Read) (															\
		elIoTask_t *task,															\
		elIoParam_t iop,elByte_t *buf,elUInt32_t cnt,elUInt32_t *p_cnt				\
	);																				\
	elErr_t		*(*Write) (															\
		elIoTask_t *task,															\
		elIoParam_t iop,elByte_t *buf,elUInt32_t cnt,elUInt32_t *p_cnt				\
	);																				\
	elErr_t		*(*Execute) (														\
		elIoTask_t *task															\
	);																				\
	elErr_t		*(*Delete) (														\
		elIoTask_t *task															\
	)


/* 'elIoIntfc_t': */
typedef struct {
	TCHAR	*name;		/* "NI-DAQ", "WAGO-MBT", ... */
	EL_IO_METHODS;
	struct {
		EL_IO_TASK_METHODS;
	} task;
} elIoIntfc_t;

#define EL_IO_DEBUG_CODE(code)		\
		if ( io->debug )			\
		{							\
			code;					\
		}

/*
 * 'elIoOffset_t'	- Data type for I/O offset and size:
 * 'elIoOffset_st'	- Signed Data type for I/O offset and size:
 */
#if UNIX
typedef uint32_t elIoOffset_t;
typedef uint32_t elIoOffset_st;		/* signed type (also 'uint32_t') */
#define EL_IO_OFFSET_SIZEOF				4
#define EL_IO_OFFSET_FMTSZPFX			"l"		/* format size prefix */
#define EL_IO_SECTOR_FMTSZPFX			"l"		/* format size prefix */
#define elIoOffset2Double(offset)	(double)(offset)
#elif WIN
typedef uint64_t elIoOffset_t;
/*
 * Signed type 'fse_io_offset_st' using 'int64_t', because of unsupported
 * conversion from 'unsigned __int64' to 'double':
 */
typedef int64_t elIoOffset_st;		/* signed type */
#define EL_IO_OFFSET_SIZEOF				8
#define EL_IO_OFFSET_FMTSZPFX			"I64"	/* format size prefix */
#define EL_IO_SECTOR_FMTSZPFX			"I64"	/* format size prefix */
#define elIoOffset2Double(offset)	(double)(__int64)(offset)
#endif

/* I/O attributes: */
enum {
	EL_IO_INPUT		= EL_FLAG(0),
	EL_IO_OUTPUT	= EL_FLAG(1),
	EL_IO_GROUP		= EL_FLAG(2),
};

enum {
	EL_IO_CTRL_DISCARD_INPUT,
	EL_IO_CTRL_FLUSH_OUTPUT,
};

#define EL_IO_EOF	(-1)

/* 'elIoItemSize_t': I/O item size: */
typedef struct {
	elUInt32_t	bytes;		/* size of item in bytes (may be rounded up) */
	elUInt32_t	bits;		/* size of item in bits */
	int			bpb;		/* item's Bits Per Byte (usually 8 or 1) */
} elIoItemSize_t;

/*
 * 'elIoBuf_t': I/O buffer (cache).
 */
typedef struct {
	elByte_t		*base;		/* address of allocated buffer */
	elByte_t		*ptr;		/* pointer to 1st available byte in buffer 'base' */
	elInt_t			ptr_bit;	/* bit index (0..7) */
	elUInt32_t		len;		/* number of valid bytes in 'ptr' */
	elUInt32_t		size;		/* size of memory allocated in 'base' */
	elIoItemSize_t	item_size;	/* I/O item size */
	/* absolute device offset from which the data in 'ptr' were read: */
	elIoOffset_t	offset;
} elIoBuf_t;

#define EL_IO_BUF_ITEM_SET(buf,pitem)		elIoBufItemSet(buf,pitem)
#define EL_IO_BUF_ITEM_GET_BIT(buf,bit)		elIoBufItemGetBit(buf,bit)

/*
 * 'elIo_t': I/O descriptor.
 */
typedef struct SelIo {
	elObj_t		obj;

	int			debug;		/* debug verbose level */
	elLog_t		*log;		/* log object */

	bool		opened;		/* TRUE: I/O opened */

	/*
	 * 'str':
	 */
	struct {
		/*
		 * 'rsrc': Resource name (full original specification of I/O resource).
		 * (e.g. "NI-VISA:TCPIP::192.168.5.250::23::SOCKET,TCPIP_KEEPALIVE=TRUE")
		 */
		elStr_t		*rsrc;

		/*
		 * 'intfc': Interface taken from 'rsrc' (e.g. "NI-VISA").
		 */
		elStr_t		*intfc;

		/*
		 * 'path': Path taken from 'rsrc' (e.g. "TCPIP::192.168.5.250::23::SOCKET").
		 *
		 * This string is created/modified by interface-specific 'Open()' call
		 * that parses the 'rsrc' string.
		 */
		elStr_t		*path;

		/*
		 * 'attrs': Attributes taken from 'rsrc' (e.g. "TCPIP_KEEPALIVE=TRUE" or
		 * "ASRL_BAUD=115200").
		 *
		 * This string is created/modified by interface-specific 'Open()' call
		 * that parses the 'rsrc' string.
		 */
		elStr_t		*attrs;

		/*
		 * 'name': Name of the I/O resource used for debug/diagnostic purposes.
		 *
		 * It is usually 'intfc' + 'path' or 'path' only:
		 *
		 *		NI-DAQ:Dev2/line16				=>	NI-DAQ:Dev2/line16
		 *		Modbus:TCP:192.168.5.102/DO/18	=>	Modbus:TCP:192.168.5.102/DO/18
		 *		Serial:COM15/9600/8/n/1/n		=>	COM15
		 *		NETRX-RFID:COM7/9600/8/n/1/n	=>	NETRX-RFID:COM7
		 */
		elStr_t		*name;

	} str;

	TCHAR				*name;		/* shortcut to 'str.name->ptr' */

	/* I/O interface: */
	const elIoIntfc_t	*intfc;

	/*
	 * Interface/Device specific I/O object:
	 * (e.g. 'elIoNiDaq_t' or 'elIoModbus_t')
	 */
	void				*dio;		/* Device I/O (not Digital I/O) */

	elIoBuf_t			buf;		/* I/O buffer */

	/*
	 * 'item_size':
	 *
	 * I/O item size in bytes and bits and # of item's bits per I/O byte.
	 *
	 * Examples:
	 *
	 *	NI-DAQ:Dev2/line16:23
	 *	 - the resource consists of 8 bits / DIO lines and therefore up to
	 *	   8 bytes (in case of 'item_size::bpb' = 1) must be read/written
	 *	   to process all lines
	 *
	 *	Modbus:Modbus:TCP:192.168.5.101/DI/11
	 *	 - the resource consists of only 1 bit / DIO line and therefore
	 *	   only 1 byte must be read/written to process the DIO signal
	 */
	elIoItemSize_t	item_size;

	/*
	 * 'attr': All I/O attributes set by 'elIoAttrSet()'.
	 */
	struct {
		struct SelIoAttrItem {
			char	*name;	/* attribute name (e.g. "MSG_TERM_CHAR") */
			char	*val;	/* attribute value (e.g. "0x0A") */
		} *item;
		int	cnt;	/* number of attributes in 'item[]' */
	} attr;

	elUInt32_t			attrs;		/* basic I/O attributes */
	elInt32_t			timeout;	/* [ms] */

	/*
	 * 'poll': Flag indicating whether polling is enabled or disabled.
	 *
	 * Polling is used to check for and read data without waiting for any data
	 * to arrive. It is typically used when reading data in a loop
	 * along with other read operations from multiple different devices/sources.
	 *
	 * Polling is implemented at interface level and therefore can be different
	 * for different interfaces:
	 *
	 * Serial ('elIoSerialIntfc'):
	 *
	 *	When polling is enabled, 'WaitCommEvent()' is not used at all, which
	 *	means that we don't need to cancel asynchronous 'WaitCommEvent()'
	 *	(by 'CancelIo()' or 'CloseFile()' + 'CreateFile()'), when (immediate)
	 *	timeout occurs (no data available). Cancelling I/O or reopening
	 *	the device is time consuming and may prevent fast checking of data
	 *	on input. That's why polling is used instead of otherwise efficient
	 *	waiting for data ('WaitCommEvent()').
	 */
	bool				poll;

	/*
	 * 'async': Data for asynchronous I/O.
	 */
	struct {
		bool enabled;	/* TRUE: asynchronous I/O enabled */
	} async;

	struct {
		elIoBuf_t	buf;
		int			term_char;
		bool		complete;
	} msg;

	struct {
		volatile bool	set;		/* I/O abort flag */
#if WIN
		HANDLE			hEvent;		/* I/O abort event */
#endif
	} abort;

	/*
	 * Mutex lock to prevent simultaneous access from multiple threads,
	 * if needed:
	 */
	elMtx_t				lock;

	/*
	 * 'err': Here the error pointer is not a copy of the error pointer
	 * returned by an 'el...()' function, but it is pointer to allocated
	 * space containing the error structure pointed to by returned
	 * error pointer. This is so that every I/O descriptor can save
	 * its own error for later check (see e.g. 'elIoNiDaqTaskExecute()'
	 * and 'dio_rsrc_read()' in "ELCOM\VALEO\UTSystem\src\line\line_ctrl\
	 * \line_ctrl.c"), because the error pointer returned by all 'el...()'
	 * functions point to the same location in the given thread and the
	 * space is overwritten with each new error.
	 */
	elErr_t				*err;

	elTimer_t			timer;

	/* pointer to user-defined data: */	
	void				*user;

	EL_IO_METHODS;
} SelIo;

typedef struct SelIoAttrItem elIoAttrItem_t;

/* 'elIoTask_t': */
typedef struct SelIoTask{
	elObj_t		obj;

	elIo_t		**io;
	int			io_cnt;

	elIoBuf_t	buf;		/* task I/O buffer */
	elUInt32_t	buf_size;

	/* current I/O task function ('EL_IO_TASK_FN_...'): */
	int			fn;

	/*
	 * 'rw[]': Read and Write task functions.
	 */
	struct SelIoTaskRW {
		/* 'elIoTaskIntfc_t': */
		struct SelIoTaskIntfc {
			const elIoIntfc_t	*ptr;

			/* 'elIoTaskDev_t': */
			struct SelIoTaskDev {
				/*
				 * Device name (e.g. "Dev1/line0,Dev1/line3", "192.168.5.101/DO/0",
				 * "192.168.5.102/DO/15-16"):
				 */
				char			*name;

				/* task's I/O pointers that belong to this device: */
				elIo_t			**io;
				int				io_cnt;

				/* I/O buffer shared by all I/O resources in 'io[]': */
				elIoBuf_t		io_buf;

				elIoBuf_t		buf;	/* output device I/O buffer */

				/*
				 * Interface-specific task device's data
				 * (e.g. 'elIoModbusTaskDev_t' or 'elIoNiDaqDev_t')
				 */
				void			*data;

				/* device I/O performance timer: */
				elTimer_t		t_val;
				elTimer_t		t_stamp;

			} *dev;
			int dev_cnt;

			/* interface I/O performance timer: */
			elTimer_t	t_val;
			elTimer_t	t_stamp;

		} *intfc;
		int			intfc_cnt;

	} rw[2];	/* 'EL_IO_TASK_{READ|WRITE}' */

	EL_IO_TASK_METHODS;

}SelIoTask ;
typedef struct SelIoTaskIntfc	elIoTaskIntfc_t;
typedef struct SelIoTaskDev		elIoTaskDev_t;

/*
 * 'EL_IO_TASK_FN_...': I/O task function identifiers.
 */
enum {
	EL_IO_TASK_FN_READ,
	EL_IO_TASK_FN_WRITE
};

/* indexes to 'elIoTask_t::rw[]': */
enum {
	EL_IO_TASK_READ,
	EL_IO_TASK_WRITE,
};

#define elIoP(obj_var)		elP(Io,obj_var)

#define EL_IO_ABORTED(io)	(io)->abort.set
#define EL_IO_ABORT_CHECK(io)									\
		do {													\
			if ( EL_IO_ABORTED(io) )							\
			{													\
				io->AbortReset(io);								\
				EL_ERR_THROW(EL_ERR_ABORT,NULL,NULL);			\
			}													\
		} while( EL_CONST_COND_0 )

extern const elObjClass_t *elIo_c;
extern const elObjClass_t *elIoTask_c;

#ifdef __cplusplus
extern "C" {			/* C Plus Plus function bindings */
#endif

elErr_t *elIoNew(elPIo_t *p_io);
elErr_t *elIoBufInit(elIoBuf_t *iobuf);
elErr_t *elIoBufClear(elIoBuf_t *iobuf);
elErr_t *elIoBufAlloc(elIoBuf_t *iobuf,elUInt32_t size);
elErr_t *elIoBufFree(elIoBuf_t *iobuf);
void	 elIoBufItemSet(elIoBuf_t *iobuf,elByte_t *pitem);
bool	 elIoBufItemGetBit(elIoBuf_t *iobuf,elInt_t bit);
elErr_t *elIoOpen(elIoParam_t iop,TCHAR *path,...);
elErr_t *elIoClose(elIoParam_t iop);
elErr_t *elIoRead(
	elIoParam_t iop,elByte_t *buf,elUInt32_t cnt,elUInt32_t *p_cnt
);
elErr_t *elIoWrite(
	elIoParam_t iop,elByte_t *buf,elUInt32_t cnt,elUInt32_t *p_cnt
);
elErr_t *elIoMsgRead(
	elIoParam_t iop,elByte_t *msg,elUInt32_t msgsize,elUInt32_t *plen
);
elErr_t *elIoMsgWrite(
	elIoParam_t iop,elByte_t *msg,elUInt32_t msgsize,elUInt32_t *plen
);
elErr_t *elIoFlush(elIoParam_t iop,elUInt32_t io_mode);
elErr_t *elIoAbort(elIoParam_t iop);
elErr_t *elIoAbortReset(elIoParam_t iop);
elErr_t *elIoCtrl(elIoParam_t iop,int cmd,void *pdata);
elErr_t *elIoAttrSet(elIoParam_t iop,TCHAR *attr_name,TCHAR *attr_val);
elErr_t *elIoAttrGet(elIoParam_t iop,TCHAR *attr_name,TCHAR **p_attr_val);
elErr_t *elIoLock(elIoParam_t iop);
elErr_t *elIoUnlock(elIoParam_t iop);
elErr_t *elIoDevLock(elIoParam_t iop);
elErr_t *elIoDevUnlock(elIoParam_t iop);
elErr_t *elIoDebugLog(elIoParam_t iop,int debug_level_min,const TCHAR *fmt,...);

elErr_t *elIoTaskRead(
	elIoTask_t *task,
	elIoParam_t iop,elByte_t *buf,elUInt32_t cnt,elUInt32_t *p_cnt
);
elErr_t *elIoTaskWrite(
	elIoTask_t *task,
	elIoParam_t iop,elByte_t *buf,elUInt32_t cnt,elUInt32_t *p_cnt
);
elErr_t *elIoTaskExecute(elIoTask_t *task);


#ifdef __cplusplus
}						/* C Plus Plus function bindings */
#endif

#include <elIoNiDaq.h>
#include <elIoWagoMbt.h>
#include <elIoModbus.h>
#include <elIoSerial.h>
#include <elIoRfid.h>
#include <elIoNetrxRfid.h>
#include <elIoPromagRfid.h>
#include <elIoElatecRfid.h>

#endif 	/* _EL_ERR_H */
