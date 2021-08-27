/*
 *  <elIoNetrxRfid.h> - Definitions for module "elIoNetrxRfid.c".
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

#if !defined(_EL_IO_NETRXRFID_H)
#define _EL_IO_NETRXRFID_H

#if _MSC_VER < 1400 && !__BCOPT__ && !_CVI_  /* Borland C/C++: Unknown preprocessor directive: 'ident' */
#if 1
#ident "@(#) elIoNetrxRfid.h 0.1 2007-03-05"
#else
#endif
#endif

#include <elBase.h>
#include <elObj.h>
#include <elErr.h>

#define EL_IO_NETRXRFID_CMD_UNIQUEREAD			0x02
#define EL_IO_NETRXRFID_CMD_SETINTERFACECONFIG	0x36
#define EL_IO_NETRXRFID_CMD_SETPORTCONFIG		0x40
#define EL_IO_NETRXRFID_CMD_WRITEPORTS			0x70
#define EL_IO_NETRXRFID_CMD_READPORTS			0x72
#define EL_IO_NETRXRFID_CMD_SOFTWAREVERSION		0xFE

#define EL_IO_NETRXRFID_OC_SUCCESSFUL			0xFF
#define EL_IO_NETRXRFID_OC_PARITYERROR			0x1A
#define EL_IO_NETRXRFID_OC_ERROR				0xFE

struct SelIoNetrxRfid;
typedef struct SelIoNetrxRfid elIoNetrxRfid_t, *elPIoNetrxRfid_t, *elIoNetrxRfidPtr_t;

/* 'elIoNetrxRfidMsg_t': */
typedef struct SelIoNetrxRfidMsg {
	elByte_t	module_addr;
	elByte_t	frame_len;
	elByte_t	cmd;			/* read: response ID, write: cmd ID */
	elByte_t	*data;			/* read: data received, write: data to send */
	elByte_t	data_len;		/* number of bytes in 'data[]' */
	elByte_t	op_code;		/* read: operation code (status) */
	elUInt16_t	crc;			/* read: received CRC value */
} elIoNetrxRfidMsg_t;

/* 'elIoNetrxRfid_t': */
typedef struct SelIoNetrxRfid {
	elObj_t				obj;

	/* pointer back to class I/O: */
	elIo_t				*io;

	/* device I/O (implemented using "Serial:..." I/O interface): */
	elIo_t				*sio;

	/* message buffer: */
	elIoNetrxRfidMsg_t	msg;

	/*
	 * Flag to indicate whether message should be returned
	 * in generic RFID format ('elIoRfidMsg_t') or not:
	 */
	bool				msg_fmt_generic;	/* TRUE: 'elIoRfidMsg_t' format */

}SelIoNetrxRfid;

extern const elObjClass_t	*elIoNetrxRfid_c;
extern const elIoIntfc_t	g_elIoNetrxRfidIntfc;
extern const elIoIntfc_t	*elIoNetrxRfidIntfc;

#ifdef __cplusplus
extern "C" {			/* C Plus Plus function bindings */
#endif

elErr_t *elIoNetrxRfidOpen(elIoParam_t iop,TCHAR *rsrc,...);
elErr_t *elIoNetrxRfidClose(elIoParam_t iop);
elErr_t *elIoNetrxRfidRead(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoNetrxRfidWrite(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoNetrxRfidMsgRead(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoNetrxRfidMsgWrite(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoNetrxRfidCtrl(elIoParam_t iop,int cmd,void *pdata);
elErr_t *elIoNetrxRfidAttrSet(elIoParam_t iop,TCHAR *attr_name,TCHAR *attr_val);
elErr_t *elIoNetrxRfidAttrGet(elIoParam_t iop,TCHAR *attr_name,TCHAR **p_attr_val);

elErr_t *elIoNetrxRfidTaskRead(
	elIoTask_t *task,
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoNetrxRfidTaskWrite(
	elIoTask_t *task,
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoNetrxRfidTaskExecute(elIoTask_t *task);

#ifdef __cplusplus
}						/* C Plus Plus function bindings */
#endif


#endif 	/* _EL_IO_NETRXRFID_H */
