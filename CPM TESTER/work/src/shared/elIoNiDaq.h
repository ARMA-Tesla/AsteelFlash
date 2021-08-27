/*
 *  <elIoNiDaq.h> - Definitions for module "elIoNiDaq.c".
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

#if !defined(_EL_IO_NIDAQ_H)
#define _EL_IO_NIDAQ_H

#if _MSC_VER < 1400 && !__BCOPT__ && !_CVI_  /* Borland C/C++: Unknown preprocessor directive: 'ident' */
#if 1
#ident "@(#) elIoNiDaq.h 0.1 2007-03-05"
#else
#endif
#endif

#include <elBase.h>
#include <elObj.h>
#include <elErr.h>
#include <NIDAQmx.h>

/*
 * 'elIoNiDaqDev_t': NI-DAQ I/O device descriptor.
 */
struct SelIoNiDaqDev;
typedef struct SelIoNiDaqDev elIoNiDaqDev_t;
/* 'elIoNiDaqDev_t': */
typedef struct SelIoNiDaqDev {
	char		*name;			/* device name (e.g. "Dev1/line0" or "Dev2") */
	char		*base_name;		/* device base name (e.g. "Dev1" or "Dev2") */

	/*
	 * 'task': DAQmx task related data.
	 */
	struct {
		/*
		 * Task handle of the device.
		 * It is used for 'DAQmx...()' calls.
		 */
		TaskHandle		handle;

		/* task is running / not running: */
		bool			run;		/* 'DAQmxStartTask()', 'DAQmxStopTask()' */

		/*
		 * List of other NI-DAQ devices (if any) that are in task-run conflict
		 * with this device. This can happen when 2 or more I/O resources
		 * use one or more same DIO lines, e.g.:
		 *
		 *		WS2_MPD_light_green			NI-DAQ:Dev4/line16
		 *		WS2_MPD_light_orange		NI-DAQ:Dev4/line16:17
		 *		WS2_MPD_light_red			NI-DAQ:Dev4/line17
		 *
		 * The above I/O resources share either "Dev4/line16" or "Dev4/line17"
		 * DIO line and the NI-DAQ tasks for these resources cannot
		 * be running ('DAQmxStartTask()') at the same time. Therefore, when
		 * I/O is to be performed on such a resource (with shared DIO
		 * line(s)), the tasks for other resources that can be in task-run
		 * conflict, are checked and the one that is currently running
		 * (if at all), is stopped ('DAQmxStopTask()') before task
		 * for the current I/O is started.
		 */
		elIoNiDaqDev_t	*run_conflict;
		int				run_conflict_cnt;

	} task;

	uInt32		serial_num;		/* 0: simulated device */
	int32		chan_type;
	int			chan_cnt;
	int			chan_line_cnt;
	int			line_cnt;		/* number of DIO lines in this device */

	int			line_min;		/* number/index of 1st  DIO line in this device */
	int			line_max;		/* number/index of last DIO line in this device */

	/*
	 * Mutex lock to prevent simultaneous access from multiple threads,
	 * if needed:
	 */
	elMtx_t				lock;

} elIoNiDaqDev_t;

struct SelIoNiDaq;
typedef struct SelIoNiDaq elIoNiDaq_t, *elPIoNiDaq_t, *elIoNiDaqPtr_t;

/* 'elIoNiDaq_t': */
typedef struct SelIoNiDaq {
	elObj_t			obj;

	/* pointer back to class I/O: */
	elIo_t			*io;

	/* device: */
	elIoNiDaqDev_t	*dev;

}SelIoNiDaq ;

/* 'elIoNiDaqTaskDev_t': */
typedef struct {
	elIoNiDaqDev_t	*ptr;		/* NI-DAQ task handle and other attributes */

	/* first and last NI-DAQ DIO line number/index/address: */
	int				line_min;
	int				line_max;
	int				line_cnt;	/* line_max - line_min + 1 */

} elIoNiDaqTaskDev_t;

extern const elObjClass_t	*elIoNiDaq_c;
extern const elIoIntfc_t	g_elIoNiDaqIntfc;
extern const elIoIntfc_t	*elIoNiDaqIntfc;

#ifdef __cplusplus
extern "C" {			/* C Plus Plus function bindings */
#endif

elErr_t *elIoNiDaqOpen(elIoParam_t iop,TCHAR *rsrc,...);
elErr_t *elIoNiDaqClose(elIoParam_t iop);
elErr_t *elIoNiDaqRead(
	elIoParam_t iop,elByte_t *buf,elUInt32_t cnt,elUInt32_t *p_cnt
);
elErr_t *elIoNiDaqWrite(
	elIoParam_t iop,elByte_t *buf,elUInt32_t cnt,elUInt32_t *p_cnt
);
elErr_t *elIoNiDaqCtrl(elIoParam_t iop,int cmd,void *pdata);
elErr_t *elIoNiDaqDevLock(elIoParam_t iop);
elErr_t *elIoNiDaqDevUnlock(elIoParam_t iop);

elErr_t *elIoNiDaqTaskRead(
	elIoTask_t *task,
	elIoParam_t iop,elByte_t *buf,elUInt32_t cnt,elUInt32_t *p_cnt
);
elErr_t *elIoNiDaqTaskWrite(
	elIoTask_t *task,
	elIoParam_t iop,elByte_t *buf,elUInt32_t cnt,elUInt32_t *p_cnt
);
elErr_t *elIoNiDaqTaskExecute(elIoTask_t *task);
elErr_t *elIoNiDaqTaskDelete(elIoTask_t *task);

#ifdef __cplusplus
}						/* C Plus Plus function bindings */
#endif


#endif 	/* _EL_IO_NIDAQ_H */
