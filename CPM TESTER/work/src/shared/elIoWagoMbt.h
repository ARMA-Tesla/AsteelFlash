/*
 *  <elIoWagoMbt.h> - Definitions for module "elIoWagoMbt.c".
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

#if !defined(_EL_IO_WAGOMBT_H)
#define _EL_IO_WAGOMBT_H

#if _MSC_VER < 1400 && !__BCOPT__ && !_CVI_  /* Borland C/C++: Unknown preprocessor directive: 'ident' */
#if 1
#ident "@(#) elIoWagoMbt.h 0.1 2007-03-05"
#else
#endif
#endif

#include <elBase.h>
#include <elObj.h>
#include <elErr.h>
/* modified version of "MBT.h" for ANSI-C modules instead of C++ modules: */
#include <MBTc.h>
#include <MBTError.h>

/*
 * 'elIoWagoMbtDev_t': WAGO-MBT I/O device descriptor.
 */
typedef struct SelIoWagoMbtDev {
	/*
	 * IP address of WAGO TCP/IP controller:
	 */
	TCHAR		*addr;

	/*
	 * TCP/UDP port number of the WAGO TCP/IP controller:
	 */
	int			port;

	/*
	 * Indicates whether TCP or UDP protocol is used to send/receive
	 * data to/from WAGO TCP/IP controller:
	 */
	bool		tcp;

	/*
	 * Socket handle connected to remote WAGO TCP/IP controller.
	 * It is used for 'MBT...()' calls.
	 */
	HANDLE		socket;

	/*
	 * 'ref_cnt': Reference count.
	 *
	 * Used to track references to this device from one or more 'elIoWagoMbt_t'
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

} elIoWagoMbtDev_t;

struct SelIoWagoMbt;
typedef struct SelIoWagoMbt elIoWagoMbt_t, *elPIoWagoMbt_t, *elIoWagoMbtPtr_t;

typedef struct {
	elObj_t				obj;

	/* pointer back to class I/O: */
	elIo_t				*io;

	/* device: */
	elIoWagoMbtDev_t	*dev;

	BYTE				table_id;
	WORD				addr, cnt;

}SelIoWagoMbt ;

extern const elObjClass_t	*elWagoMbt_c;
extern const elIoIntfc_t	g_elIoWagoMbtIntfc;
extern const elIoIntfc_t	*elIoWagoMbtIntfc;

#ifdef __cplusplus
extern "C" {			/* C Plus Plus function bindings */
#endif

elErr_t *elIoWagoMbtOpen(elIoParam_t iop,TCHAR *rsrc,...);
elErr_t *elIoWagoMbtClose(elIoParam_t iop);
elErr_t *elIoWagoMbtRead(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoWagoMbtWrite(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoWagoMbtCtrl(elIoParam_t iop,int cmd,void *pdata);

elErr_t *elIoWagoMbtTaskRead(
	elIoTask_t *task,
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoWagoMbtTaskWrite(
	elIoTask_t *task,
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoWagoMbtTaskExecute(elIoTask_t *task);

#ifdef __cplusplus
}						/* C Plus Plus function bindings */
#endif


#endif 	/* _EL_IO_WAGOMBT_H */
