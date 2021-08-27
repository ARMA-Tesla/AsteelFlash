/*
 *  <elIoModbus.h> - Definitions for module "elIoModbus.c".
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

#if !defined(_EL_IO_MODBUS_H)
#define _EL_IO_MODBUS_H

#if _MSC_VER < 1400 && !__BCOPT__ && !_CVI_  /* Borland C/C++: Unknown preprocessor directive: 'ident' */
#if 1
#ident "@(#) elIoModbus.h 0.1 2007-03-05"
#else
#endif
#endif

#include <elBase.h>
#include <elObj.h>
#include <elErr.h>
#include <elSocket.h>

enum {
	EL_IO_MODBUS_PROTO_RTU,
	EL_IO_MODBUS_PROTO_ASCII,
	EL_IO_MODBUS_PROTO_TCP,
	EL_IO_MODBUS_PROTO_UDP,
};

enum {
	EL_IO_MODBUS_TABLE_INPUT_COIL,
	EL_IO_MODBUS_TABLE_OUTPUT_COIL,
	EL_IO_MODBUS_TABLE_INPUT_REGISTER,
	EL_IO_MODBUS_TABLE_OUTPUT_REGISTER,
};

/*
 * 'elIoModbusDev_t': WAGO-MBT I/O device descriptor.
 */
typedef struct SelIoModbusDev {
	char		*name;		/* device name (e.g. host name / IP address) */

	/*
	 * Indicates which Modbus protocol is used to send/receive data
	 * to/from the Modbus device:
	 */
	int			proto;		/* 'EL_IO_MODBUS_PROTO_...' */

	union {
		struct {
			HANDLE	fh;		/* handle to serial device */
		} rtu;

		struct {
			bool		connected;

			/*
			 * IP address of WAGO TCP/IP controller:
			 */
			TCHAR		*addr;

			/*
			 * TCP/UDP port number of the WAGO TCP/IP controller:
			 */
			int			port;

			/*
			 * Socket handle connected to remote WAGO TCP/IP controller.
			 * It is used for 'MBT...()' calls.
			 */
			elSocket_t	*s;
		} tcp;
	} u;

	/*
	 * Mutex lock to prevent simultaneous access from multiple threads,
	 * if needed:
	 */
	elMtx_t				lock;

	/*
	 * 'ref_cnt': Reference count.
	 *
	 * Used to track references to this device from one or more 'elIoModbus_t'
	 * I/O handles. One I/O device would need to be used/shared by more than
	 * one I/O handle in case the the WAGO controller would not support
	 * multiple TCP/IP connections or only certain number of connections.
	 *
	 * For example, WAGO TCP/IP controller (e.g. 750-841) supports only
	 * 5 TCP/IP connectons via MODBUS/TCP. See WAGO 750-341 (750/841) Manual:
	 *
	 *	    3.1.5 Data Exchange
	 *
	 *	    A coupler is able to produce a defined number of simultaneous socket
	 *	    connections to other network subscribers:
	 *	    - 3 connections for HTTP (read HTML pages from the coupler),
	 *	    - 5 connections via MODBUS/TCP (read or write input and output data
	 *		  from the coupler),
	 *		- 128 Ethernet IP connections
	 */
	int			ref_cnt;

} elIoModbusDev_t;

struct SelIoModbus;
typedef struct SelIoModbus elIoModbus_t, *elPIoModbus_t, *elIoModbusPtr_t;

/* 'elIoModbus_t': */
typedef struct SelIoModbus {
	elObj_t				obj;

	/* pointer back to class I/O: */
	elIo_t				*io;

	/* device: */
	elIoModbusDev_t		*dev;

	BYTE				table_id;
	WORD				addr, cnt;

	uint16_t			tid;		/* transaction ID */
	int					hdr_len;	/* header length used for offset */
	int					xsum_size;	/* checksum size RTU = 2 and TCP = 0 */
}SelIoModbus;

/* 'elIoModbusTaskDev_t': */
typedef struct {
	elIoModbusDev_t	*ptr;
	BYTE			table_id;					/* MODBUS table ID */
	WORD			addr_min, addr_max, cnt;	/* first and last MODBUS DIO address */
} elIoModbusTaskDev_t;

extern const elObjClass_t	*elModbus_c;
extern const elIoIntfc_t	g_elIoModbusIntfc;
extern const elIoIntfc_t	*elIoModbusIntfc;

#ifdef __cplusplus
extern "C" {			/* C Plus Plus function bindings */
#endif

elErr_t *elIoModbusOpen(elIoParam_t iop,TCHAR *rsrc,...);
elErr_t *elIoModbusClose(elIoParam_t iop);
elErr_t *elIoModbusRead(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoModbusWrite(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoModbusCtrl(elIoParam_t iop,int cmd,void *pdata);
elErr_t *elIoModbusDevLock(elIoParam_t iop);
elErr_t *elIoModbusDevUnlock(elIoParam_t iop);

elErr_t *elIoModbusTaskRead(
	elIoTask_t *task,
	elIoParam_t iop,elByte_t *buf,elUInt32_t cnt,elUInt32_t *p_cnt
);
elErr_t *elIoModbusTaskWrite(
	elIoTask_t *task,
	elIoParam_t iop,elByte_t *buf,elUInt32_t cnt,elUInt32_t *p_cnt
);
elErr_t *elIoModbusTaskExecute(elIoTask_t *task);
elErr_t *elIoModbusTaskDelete(elIoTask_t *task);

#ifdef __cplusplus
}						/* C Plus Plus function bindings */
#endif


#endif 	/* _EL_IO_MODBUS_H */
