/*
 *  <elIoPromagRfid.h> - Definitions for module "elIoPromagRfid.c".
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

#if !defined(_EL_IO_PROMAGRFID_H)
#define _EL_IO_PROMAGRFID_H

#if _MSC_VER < 1400 && !__BCOPT__ && !_CVI_  /* Borland C/C++: Unknown preprocessor directive: 'ident' */
#if 1
#ident "@(#) elIoPromagRfid.h 0.1 2007-11-09"
#else
#endif
#endif

#include <elBase.h>
#include <elObj.h>
#include <elErr.h>

struct SelIoPromagRfid;
typedef struct SelIoPromagRfid elIoPromagRfid_t;
typedef struct SelIoPromagRfid *elPIoPromagRfid_t, *elIoPromagRfidPtr_t;

/* 'elIoPromagRfidMsg_t': */
typedef struct SelIoPromagRfidMsg {
	elByte_t	data[5];		/* 5 RFID bytes received */
} elIoPromagRfidMsg_t;

typedef struct SelIoPromagRfid {
	elObj_t				obj;

	/* pointer back to class I/O: */
	elIo_t				*io;

	/* device I/O (implemented using "Serial:..." I/O interface): */
	elIo_t				*sio;

	/* message buffer: */
	elIoPromagRfidMsg_t	msg;

	/*
	 * Flag to indicate whether message should be returned
	 * in generic RFID format ('elIoRfidMsg_t') or not:
	 */
	bool				msg_fmt_generic;	/* TRUE: 'elIoRfidMsg_t' format */

}SelIoPromagRfid;

extern const elObjClass_t	*elIoPromagRfid_c;
extern const elIoIntfc_t	g_elIoPromagRfidIntfc;
extern const elIoIntfc_t	*elIoPromagRfidIntfc;

#ifdef __cplusplus
extern "C" {			/* C Plus Plus function bindings */
#endif

elErr_t *elIoPromagRfidOpen(elIoParam_t iop,TCHAR *rsrc,...);
elErr_t *elIoPromagRfidClose(elIoParam_t iop);
elErr_t *elIoPromagRfidRead(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoPromagRfidWrite(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoPromagRfidMsgRead(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoPromagRfidMsgWrite(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoPromagRfidCtrl(elIoParam_t iop,int cmd,void *pdata);
elErr_t *elIoPromagRfidAttrSet(elIoParam_t iop,TCHAR *attr_name,TCHAR *attr_val);
elErr_t *elIoPromagRfidAttrGet(elIoParam_t iop,TCHAR *attr_name,TCHAR **p_attr_val);

elErr_t *elIoPromagRfidTaskRead(
	elIoTask_t *task,
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoPromagRfidTaskWrite(
	elIoTask_t *task,
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoPromagRfidTaskExecute(elIoTask_t *task);

#ifdef __cplusplus
}						/* C Plus Plus function bindings */
#endif

#endif 	/* _EL_IO_PROMAGRFID_H */
