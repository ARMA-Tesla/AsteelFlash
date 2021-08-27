/*
 *  <elIoRfid.h> - Definitions for module "elIoRfid.c".
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
 *	2007-12-04	Radomir Tomis (Radomir.Tomis@dvi.elcom.cz)
 *		Created.
 */

#if _MSC_VER >= 1000
#pragma once
#endif 

#if !defined(_EL_IO_RFID_H)
#define _EL_IO_RFID_H

#if _MSC_VER < 1400 && !__BCOPT__ && !_CVI_  /* Borland C/C++: Unknown preprocessor directive: 'ident' */
#if 1
#ident "@(#) elIoRfid.h 0.1 2007-12-04"
#else
#endif
#endif

#include <elBase.h>
#include <elObj.h>
#include <elErr.h>

struct SelIoRfid;
typedef struct SelIoRfid elIoRfid_t, *elPIoRfid_t, *elIoRfidPtr_t;


/* 'elIoRfidMsg_t': */
typedef struct {
	byte	code[5];
} elIoRfidMsg_t;

typedef struct SelIoRfid {
	elObj_t				obj;

	/* pointer back to class I/O: */
	elIo_t				*io;

	/*
	 * Device I/O (implemented using device-specific
	 * "...-RFID:..." I/O interface):
	 */
	elIo_t				*rio;

	elIoRfidMsg_t		msg;

}SelIoRfid;

extern const elObjClass_t	*elIoRfid_c;
extern const elIoIntfc_t	g_elIoRfidIntfc;
extern const elIoIntfc_t	*elIoRfidIntfc;

#ifdef __cplusplus
extern "C" {			/* C Plus Plus function bindings */
#endif

elErr_t *elIoRfidOpen(elIoParam_t iop,TCHAR *rsrc,...);
elErr_t *elIoRfidClose(elIoParam_t iop);
elErr_t *elIoRfidRead(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoRfidWrite(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoRfidMsgRead(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoRfidMsgWrite(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoRfidCtrl(elIoParam_t iop,int cmd,void *pdata);
elErr_t *elIoRfidAttrSet(elIoParam_t iop,TCHAR *attr_name,TCHAR *attr_val);
elErr_t *elIoRfidAttrGet(elIoParam_t iop,TCHAR *attr_name,TCHAR **p_attr_val);

elErr_t *elIoRfidTaskRead(
	elIoTask_t *task,
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoRfidTaskWrite(
	elIoTask_t *task,
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoRfidTaskExecute(elIoTask_t *task);

#ifdef __cplusplus
}						/* C Plus Plus function bindings */
#endif


#endif 	/* _EL_IO_RFID_H */
