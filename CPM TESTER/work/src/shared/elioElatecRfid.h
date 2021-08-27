/*
 *  <elIoElatecRfid.h> - Definitions for module "elIoElatecRfid.c".
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
 *		Created.
 */

#if _MSC_VER >= 1000
#pragma once
#endif 

#if !defined(_EL_IO_ELATECRFID_H)
#define _EL_IO_ELATECRFID_H

#if _MSC_VER < 1400 && !__BCOPT__ && !_CVI_  /* Borland C/C++: Unknown preprocessor directive: 'ident' */
#if 1
#ident "@(#) elIoElatecRfid.h 0.1 2007-11-09"
#else
#endif
#endif

#include <elBase.h>
#include <elObj.h>
#include <elErr.h>

struct SelIoElatecRfid;
typedef struct SelIoElatecRfid elIoElatecRfid_t;
typedef struct SelIoElatecRfid *elPIoElatecRfid_t, *elIoElatecRfidPtr_t;

/* 'elIoElatecRfidMsg_t': */
typedef struct SelIoElatecRfidMsg {
	elByte_t	len;			/* read: total byte count of the reply packet (4+n+1) */
	elByte_t	addr;			/* read: actual reader address */
	elByte_t	cmd;			/* read: the request type identifier */
	elByte_t	status;			/* read: command completition status byte */
	elByte_t	*data;			/* read: supplementary data replied by the RFID reader */
	elByte_t	xsum;			/* read: XOR sum of all bytes except checksum */
} elIoElatecRfidMsg_t;

/* 'elIoElatecRfid_t': */
typedef struct SelIoElatecRfid {
	elObj_t				obj;

	/* pointer back to class I/O: */
	elIo_t				*io;

	/* device I/O (implemented using "Serial:..." I/O interface): */
	elIo_t				*sio;

	/* message buffer: */
	elIoElatecRfidMsg_t	msg;

	/*
	 * Flag to indicate whether message should be returned
	 * in generic RFID format ('elIoRfidMsg_t') or not:
	 */
	bool				msg_fmt_generic;	/* TRUE: 'elIoRfidMsg_t' format */

}SelIoElatecRfid;

extern const elObjClass_t	*elIoElatecRfid_c;
extern const elIoIntfc_t	g_elIoElatecRfidIntfc;
extern const elIoIntfc_t	*elIoElatecRfidIntfc;

#ifdef __cplusplus
extern "C" {			/* C Plus Plus function bindings */
#endif

elErr_t *elIoElatecRfidOpen(elIoParam_t iop,TCHAR *rsrc,...);
elErr_t *elIoElatecRfidClose(elIoParam_t iop);
elErr_t *elIoElatecRfidRead(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoElatecRfidWrite(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoElatecRfidMsgRead(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoElatecRfidMsgWrite(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoElatecRfidCtrl(elIoParam_t iop,int cmd,void *pdata);
elErr_t *elIoElatecRfidAttrSet(elIoParam_t iop,TCHAR *attr_name,TCHAR *attr_val);
elErr_t *elIoElatecRfidAttrGet(elIoParam_t iop,TCHAR *attr_name,TCHAR **p_attr_val);

elErr_t *elIoElatecRfidTaskRead(
	elIoTask_t *task,
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoElatecRfidTaskWrite(
	elIoTask_t *task,
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoElatecRfidTaskExecute(elIoTask_t *task);

#ifdef __cplusplus
}						/* C Plus Plus function bindings */
#endif

#endif 	/* _EL_IO_ELATECRFID_H */
