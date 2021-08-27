/*
 *  <elIoSerial.h> - Definitions for module "elIoSerial.c".
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

#if !defined(_EL_IO_SERIAL_H)
#define _EL_IO_SERIAL_H

#if _MSC_VER < 1400 && !__BCOPT__ && !_CVI_  /* Borland C/C++: Unknown preprocessor directive: 'ident' */
#if 1
#ident "@(#) elIoSerial.h 0.1 2007-03-05"
#else
#endif
#endif

#include <elBase.h>
#include <elObj.h>
#include <elErr.h>

#if WIN
typedef HANDLE	elIoSerialDevHandle_t;
#else
typedef int		elIoSerialDevHandle_t;
#endif

/*
 * 'elIoSerialDev_t': Serial I/O device descriptor.
 */
typedef struct SelIoSerialDev {
	elIoSerialDevHandle_t	handle;
#if WIN
	DCB						dcb;
#endif
} elIoSerialDev_t;

struct SelIoSerial;
typedef struct SelIoSerial elIoSerial_t, *elPIoSerial_t, *elIoSerialPtr_t;

typedef struct SelIoSerial {
	elObj_t			obj;

	/* pointer back to class I/O: */
	elIo_t			*io;

	/* device: */
	elIoSerialDev_t	dev;

	/*
	 * 'last_read_full':
	 *
	 * Flag indicating whether last 'ReadFile()' returned all requested bytes
	 * or not.
	 */
	bool			last_read_full;

	struct {
		bool		on;				/* overlapped I/O enabled/disabled */
		OVERLAPPED	ReadWriteFile;
		OVERLAPPED	WaitCommEvent;
	} o;

	/*
	 * 'capture': I/O capture attributes.
	 */
	struct {
		bool enable;
		struct {
			char *path;
			FILE *fp;
		} file;
	} capture;

}SelIoSerial;

extern const elObjClass_t	*elIoSerial_c;
extern const elIoIntfc_t	g_elIoSerialIntfc;
extern const elIoIntfc_t	*elIoSerialIntfc;

#ifdef __cplusplus
extern "C" {			/* C Plus Plus function bindings */
#endif

elErr_t *elIoSerialOpen(elIoParam_t iop,TCHAR *rsrc,...);
elErr_t *elIoSerialClose(elIoParam_t iop);
elErr_t *elIoSerialRead(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoSerialWrite(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoSerialMsgRead(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoSerialMsgWrite(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoSerialCtrl(elIoParam_t iop,int cmd,void *pdata);
elErr_t *elIoSerialAttrSet(elIoParam_t iop,TCHAR *attr_name,TCHAR *attr_val);
elErr_t *elIoSerialAttrGet(elIoParam_t iop,TCHAR *attr_name,TCHAR **p_attr_val);

elErr_t *elIoSerialTaskRead(
	elIoTask_t *task,
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoSerialTaskWrite(
	elIoTask_t *task,
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
);
elErr_t *elIoSerialTaskExecute(elIoTask_t *task);

#ifdef __cplusplus
}						/* C Plus Plus function bindings */
#endif


#endif 	/* _EL_IO_SERIAL_H */
