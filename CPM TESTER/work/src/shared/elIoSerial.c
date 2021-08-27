/*
 *  "elIoSerial.c" - Serial (RS-232, ...) I/O routines.
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
static char _ident[] = "@(#) elIoSerial.c 0.1 2007-03-05";
#else
#endif

#include <elBase.h>
#include <elObj.h>
#include <elMem.h>
#include <elIo.h>
#include <elErr.h>
#include <elErrWin.h>

#define IO_OVERLAPPED	1

static elErr_t *elIoSerialCtor(elObjPtr_t *po,va_list args);
static elErr_t *elIoSerialDtor(elObjPtr_t *po);

static elObjClass_t gs_elIoSerial_c = {
	sizeof(elIoSerial_t),
	"elIoSerial",
	elIoSerialCtor,	/* Ctor()	*/
	elIoSerialDtor,	/* Dtor()	*/
	NULL,			/* Clone()	*/
	NULL,			/* Differ()	*/
};

const elObjClass_t *elIoSerial_c = &gs_elIoSerial_c;

#define EL_IO_SERIAL_METHODS								\
		elIoSerialOpen,				/* Open()			*/	\
		elIoSerialClose,			/* Close()			*/	\
		elIoSerialRead,				/* Read()			*/	\
		elIoSerialWrite,			/* Write()			*/	\
		elIoSerialMsgRead,			/* MsgRead()		*/	\
		elIoSerialMsgWrite,			/* MsgWrite()		*/	\
		NULL,						/* Flush()			*/	\
		NULL,						/* Abort()			*/	\
		NULL,						/* AbortReset()		*/	\
		elIoSerialCtrl,				/* Ctrl()			*/	\
		elIoSerialAttrSet,			/* AttrSet()		*/	\
		elIoSerialAttrGet,			/* AttrGet()		*/	\
		NULL,						/* Lock()			*/	\
		NULL,						/* Unlock()			*/	\
		NULL,						/* DevLock()		*/	\
		NULL,						/* DevUnlock()		*/	\
		NULL,						/* DebugLog()		*/	\
		elIoSerialTaskRead,			/* TaskRead()		*/	\
		elIoSerialTaskWrite,		/* TaskWrite()		*/	\
		elIoSerialTaskExecute		/* TaskExecute()	*/	\

const elIoIntfc_t g_elIoSerialIntfc = {
	_T("Serial"),	EL_IO_SERIAL_METHODS
};
const elIoIntfc_t *elIoSerialIntfc = &g_elIoSerialIntfc;

static struct {
	BYTE id;
	char *str;
} gs_parity[] = {
	NOPARITY,		"None",
	EVENPARITY,		"Even",
	ODDPARITY,		"Odd",
	MARKPARITY,		"Mark",
	SPACEPARITY,	"Space"
};

static struct {
	BYTE id;
	char *str;
} gs_stopbits[] = {
	ONESTOPBIT,		"1",
	ONE5STOPBITS,	"1.5",
	TWOSTOPBITS,	"2",
};

/*
 * Communication events:
 */
static struct {
	DWORD id;
	char *str;
} gs_comm_evt[] = {
#define E(id)	EV_##id,	#id
	/* taken from <winbase.h> */
	E(RXCHAR),		// Any Character received
	E(RXFLAG),		// Received certain character
	E(TXEMPTY),		// Transmitt Queue Empty
	E(CTS),			// CTS changed state
	E(DSR),			// DSR changed state
	E(RLSD),		// RLSD (CD) changed state
	E(ERR),			// Line status error occurred
	E(RING),		// Ring signal detected
	E(RX80FULL),	// Receive buffer is 80 percent full
#if 1
	/* Virtual Serial Port from www.mks.zp.ua doesn't support these events: */
	E(BREAK),		// BREAK received
	E(PERR),		// Printer error occured
	E(EVENT1),		// Provider specific event 1
	E(EVENT2),		// Provider specific event 2
#endif
#undef E
};
#define COMM_EVTs	EL_ARRAY_CNT(gs_comm_evt)

/*
 * Communication errors:
 */
static struct {
	DWORD id;
	char *str;
} gs_comm_err[] = {
#define E(id)	CE_##id,	#id
	/* taken from <winbase.h> */
	E(RXOVER),		// Receive Queue overflow
	E(OVERRUN),		// Receive Overrun Error
	E(RXPARITY),	// Receive Parity Error
	E(FRAME),		// Receive Framing error
	E(BREAK),		// Break Detected
	E(TXFULL),		// TX Queue is full
	E(PTO),			// LPTx Timeout
	E(IOE),			// LPTx I/O Error
	E(DNS),			// LPTx Device not selected
	E(OOP),			// LPTx Out-Of-Paper
	E(MODE),		// Requested mode unsupported
#undef E
};
#define COMM_ERRs	EL_ARRAY_CNT(gs_comm_err)

static elErr_t *elIoSerialAbortReset(elIoParam_t iop);

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoSerialCtor"
static elErr_t *elIoSerialCtor(elObjPtr_t *po,va_list args)
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
#define	__FUNC__	"elIoSerialDtor"
static elErr_t *elIoSerialDtor(elObjPtr_t *po)
{
	EL_ERR_VAR;
	elIo_t		*io = (elIo_t *)*po;

	(void) io;

	;

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"dcb_set"
static bool dcb_set(DCB *dcb,int argc,char *argv[])
{
	bool			rts_cts, dtr_dsr, xon_xoff;
	register int	i;

	if ( argc >= 1 )
		dcb->BaudRate = atoi(argv[0]);				/* Baud Rate */
	if ( argc >= 2 )
		dcb->ByteSize = (BYTE) atoi(argv[1]);		/* Character Size */

	if ( argc >= 3 )
	{
		/*
		 * Parity:
		 */
		for(i = 0;
			i < EL_ARRAY_CNT(gs_parity)
			&& *gs_parity[i].str != toupper(*argv[2]);
			i++
		)
			;
		if ( i == EL_ARRAY_CNT(gs_parity) )
		{
			elLog("%s(): '%s': unknown parity",__FUNC__,argv[2]);
			return FALSE;
		}
		dcb->Parity = gs_parity[i].id;
		dcb->fParity = TRUE;
	}
	
	if ( argc >= 4 )
	{
		/*
		 * Stop Bits:
		 */
		for(i = 0;
			i < EL_ARRAY_CNT(gs_stopbits) && strcmp(gs_stopbits[i].str,argv[3]);
			i++
		);
		if ( i == EL_ARRAY_CNT(gs_stopbits) )
		{
			elLog("%s(): '%s': unknown number of stop-bits",__FUNC__,argv[3]);
			return FALSE;
		}
		dcb->StopBits = gs_stopbits[i].id;
	}

	if ( argc >= 5 )
	{
		/*
		 * Hand-shake:
		 */
		rts_cts = dtr_dsr = xon_xoff = FALSE;
		switch( toupper(*argv[4]) )
		{
		case 'N':	/* None */
			break;
		case 'R':	/* RTS/CTS */
			rts_cts = TRUE;
			dcb->fRtsControl		= RTS_CONTROL_HANDSHAKE;
			dcb->fOutxCtsFlow		= TRUE;
			break;
		case 'D':	/* DTR/DSR */
			dtr_dsr = TRUE;
			dcb->fDtrControl		= DTR_CONTROL_HANDSHAKE;
			dcb->fOutxDsrFlow		= TRUE;
			dcb->fDsrSensitivity	= FALSE;
			break;
		case 'X':	/* XON/XOFF */
			xon_xoff = TRUE;
			dcb->fOutX				= TRUE;
			dcb->fInX				= TRUE;
			dcb->fTXContinueOnXoff	= FALSE;
			dcb->XonChar			= 0x11;
			dcb->XoffChar			= 0x13;
			dcb->XonLim				= 0;
			dcb->XoffLim			= 0;
			break;
		default:
			elLog("%s(): '%s': unknown hand-shake",__FUNC__,argv[4]);
			return FALSE;
		}
		if ( !rts_cts )
		{
			dcb->fRtsControl		= RTS_CONTROL_ENABLE;
			dcb->fOutxCtsFlow		= FALSE;
		}
		if ( !dtr_dsr )
		{
			dcb->fDtrControl		= DTR_CONTROL_ENABLE;
			dcb->fOutxDsrFlow		= FALSE;
			dcb->fDsrSensitivity	= FALSE;
		}
		if ( !xon_xoff )
		{
			dcb->fOutX				= FALSE;
			dcb->fInX				= FALSE;
			dcb->fTXContinueOnXoff	= FALSE;
		}
	}	/* if ( argc >= 5 ) */

	if ( argc >= 6 )
	{
		/*
		 * Abort-on-error:
		 */
		dcb->fAbortOnError = atoi(argv[5]);
	}

	return TRUE;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"dcb_display"
static void dcb_display(DCB *dcb,bool verbose)
{
	struct {
		char brate[10+EL_ST], chsize[1+EL_ST], parity[5+EL_ST],
			 stopbits[3+EL_ST], handshake[8+EL_ST];
	} str;
	char			tmp_str[80+EL_ST];
	char			*p;
	register int	i;


#if 1
#define OS(mbr)	offsetof(DCB,mbr),sizeof(dcb->mbr)
	printf("sizeof(DCB): %d\n",sizeof(DCB));
	printf("DCBlength:   %4d, %3d\n",OS(DCBlength));
	printf("BaudRate:    %4d, %3d\n",OS(BaudRate));
	printf("wReserved:   %4d, %3d\n",OS(wReserved));

	printf("XonLim:      %4d, %3d\n",OS(XonLim));
	printf("XoffLim:     %4d, %3d\n",OS(XoffLim));
	printf("ByteSize:    %4d, %3d\n",OS(ByteSize));
	printf("Parity:      %4d, %3d\n",OS(Parity));

	printf("StopBits:    %4d, %3d\n",OS(StopBits));
	printf("XonChar:     %4d, %3d\n",OS(XonChar));
	printf("XoffChar:    %4d, %3d\n",OS(XoffChar));
	printf("ErrorChar:   %4d, %3d\n",OS(ErrorChar));
	printf("EofChar:     %4d, %3d\n",OS(EofChar));
	printf("EvtChar:     %4d, %3d\n",OS(EvtChar));
	printf("wReserved1:  %4d, %3d\n",OS(wReserved1));

	printf("\n");
#endif

	sprintf(str.brate,"%lu",dcb->BaudRate);
	sprintf(str.chsize,"%d",dcb->ByteSize);

	/*
	 * Parity:
	 */
	if ( dcb->fParity )
	{
		for(i = 0; i < EL_ARRAY_CNT(gs_parity)
			&& gs_parity[i].id != dcb->Parity;
			i++
		)
			;
		if ( i < EL_ARRAY_CNT(gs_parity) )
			p = gs_parity[i].str;
		else
		{
			sprintf(tmp_str,"<unknown, #%d>",dcb->Parity);
			p = tmp_str;
		}
	}
	else
		p = "None";
	strcpy(str.parity,p);

	/*
	 * Stop Bits:
	 */
	for(i = 0;
		i < EL_ARRAY_CNT(gs_stopbits) && gs_stopbits[i].id != dcb->StopBits;
		i++
	)
		;
	if ( i < EL_ARRAY_CNT(gs_stopbits) )
		p = gs_stopbits[i].str;
	else
	{
		sprintf(tmp_str,"<unknown, #%d>",dcb->StopBits);
		p = tmp_str;
	}
	strcpy(str.stopbits,p);

	/*
	 * Hand-shake:
	 */
	if ( dcb->fOutxCtsFlow )
		p = "RTS/CTS";
	else if ( dcb->fOutxDsrFlow )
		p = "DTR/DSR";
	else if ( dcb->fOutX )
		p = "XON/XOFF";
	else
		p = "None";
	strcpy(str.handshake,p);

	if ( verbose )
	{
		printf("Baud Rate:      %s\n",str.brate);
		printf("Character Size: %s\n",str.chsize);
		printf("Parity:         %s\n",str.parity);
		printf("Stop Bits:      %s\n",str.stopbits);
		printf("Hand-shake:     %s\n",str.handshake);
#if 1
		printf("dcb:\n");						
		printf("  DCBlength:        %lu\n",		dcb->DCBlength);
		printf("  BaudRate:         %lu\n",		dcb->BaudRate);
		printf("  Binary:           %d\n",		dcb->fBinary);
		printf("  Parity:           %d\n",		dcb->fParity);
		printf("  OutxCtsFlow:      %d\n",		dcb->fOutxCtsFlow);
		printf("  OutxDsrFlow:      %d\n",		dcb->fOutxDsrFlow);
		printf("  DtrControl:       %d\n",		dcb->fDtrControl);
		printf("  DsrSensitivity:   %d\n",		dcb->fDsrSensitivity);
		printf("  TXContinueOnXoff: %d\n",		dcb->fTXContinueOnXoff);
		printf("  OutX:             %d\n",		dcb->fOutX);
		printf("  InX:              %d\n",		dcb->fInX);
		printf("  ErrorChar:        %d\n",		dcb->fErrorChar);
		printf("  Null:             %d\n",		dcb->fNull);
		printf("  RtsControl:       %d\n",		dcb->fRtsControl);
		printf("  AbortOnError:     %d\n",		dcb->fAbortOnError);
		printf("  Dummy2:           %lu\n",		dcb->fDummy2);
		printf("  Reserved:         %d\n",		dcb->wReserved);
		printf("  XonLim:           %d\n",		dcb->XonLim);
		printf("  XoffLim:          %d\n",		dcb->XoffLim);
		printf("  ByteSize:         %d\n",		dcb->ByteSize);
		printf("  StopBits:         %d\n",		dcb->StopBits);
		printf("  XonChar:          0x%02X\n",	dcb->XonChar);
		printf("  XoffChar:         0x%02X\n",	dcb->XoffChar);
		printf("  ErrorChar:        0x%02X\n",	dcb->ErrorChar);
		printf("  EofChar:          0x%02X\n",	dcb->EofChar);
		printf("  EvtChar:          0x%02X\n",	dcb->EvtChar);
		printf("  Reserved1:        %d\n",		dcb->wReserved1);
#endif
	}
	else
		printf("%s %s %s %s %s\n",
			str.brate,str.chsize,str.parity,str.stopbits,str.handshake);
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"evt_mask_logd"
static void evt_mask_logd(char *func,DWORD mask)
{
	register int i;

	elLogD("%s(): WaitCommEvent(): mask: 0x%lX:",func,mask);
	for(i = 0; i < COMM_EVTs; i++)
		if ( mask & gs_comm_evt[i].id )
			elLogD("\n - %s",gs_comm_evt[i].str);
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"err_flags_logd"
static void err_flags_logd(DWORD errflags)
{
	bool			err;
	register int	i;

	for(i = 0, err = FALSE; i < COMM_ERRs; i++)
	{
		if ( errflags & gs_comm_err[i].id )
		{
			elLogD("\n - CE_%s",gs_comm_err[i].str);
			err = TRUE;
		}
	}
	if ( !err )
		elLogD("ClearCommError(): errflags: %lu = 0x%lX",errflags,errflags);
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"comstat_logd"
static void comstat_logd(COMSTAT *comstat)
{
	elLogD("\n - comstat:");
	elLogD("\n   CTS Hold:  %d",		comstat->fCtsHold);
	elLogD("\n   DSR Hold:  %d",		comstat->fDsrHold);
	elLogD("\n   RLSD Hold: %d",		comstat->fRlsdHold);
	elLogD("\n   XOFF Hold: %d",		comstat->fXoffHold);
	elLogD("\n   XOFF Sent: %d",		comstat->fXoffSent);
	elLogD("\n   EOF Sent:  %d",		comstat->fEof);
	elLogD("\n   TX Char:   %d",		comstat->fTxim);
	elLogD("\n   RX Queue:  %5lu",	comstat->cbInQue);
	elLogD("\n   TX Queue:  %5lu",	comstat->cbOutQue);
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoSerialClearError"
static void elIoSerialClearError(elIo_t *io)
{
	elIoSerial_t	*dio = io->dio;
	elIoSerialDev_t	*dev = &dio->dev;
	COMSTAT			comstat;
	DWORD			errflags;

	if ( !ClearCommError(dev->handle,&errflags,&comstat) )
	{
		elLog("%s(): ClearCommError(): %W",__FUNC__);
		return;
	}
	if ( io->debug )
	{
		err_flags_logd(errflags);
		comstat_logd(&comstat);
	}
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoSerialWaitCommEvent"
static elErr_t *elIoSerialWaitCommEvent(elIo_t *io,DWORD *pmask)
{
	EL_ERR_VAR;
	elIoSerial_t	*dio = io->dio;
	elIoSerialDev_t	*dev = &dio->dev;

	/*
	 * Start up overlapped (asynchronous) 'WaitCommEvent()':
	 */
	if ( io->debug > 1 )
		elLogD("%s(): WaitCommEvent() ...",__FUNC__);

wait:
#if 0
	elLogD("%s(): WaitCommEvent() ...",__FUNC__);
#endif
	if ( WaitCommEvent(dev->handle,pmask,dio->o.on ? &dio->o.WaitCommEvent : NULL) )
	{
		if ( io->debug > 1 )
			elLogD("\n - completed synchronously");
	}
	else
	{
		HANDLE h[2];
		DWORD ret;

		if ( GetLastError() == ERROR_OPERATION_ABORTED )
		{
			elIoSerialClearError(io);
			goto wait;
		}
		else if ( GetLastError() != ERROR_IO_PENDING )
		{
#if 0
			{	/* inner block */
			DWORD cnt;

			elLogD("%s(): HasOverlappedIoCompleted(): %d",
				__FUNC__,HasOverlappedIoCompleted(&dio->o.WaitCommEvent));
			elLogD("%s(): GetOverlappedResult(): %d",
				__FUNC__,
				GetOverlappedResult(dev->handle,&dio->o.WaitCommEvent,&cnt,FALSE)
			);
			}	/* inner block */
#endif

			EL_ERR_THROW_WIN(
				"WaitCommEvent",EL_ERR_IO,elErrGetLastErrorStr()
			);
		}

		/*
		 * 'WaitCommEvent()' is in progress (executed asynchronously):
		 */
		if ( io->debug > 1 )
			elLogD("\n - launched asynchronously (overlapped)");

		if ( io->debug > 1 )
			elLogD("%s(): MsgWaitForMultipleObjects(timeout=%ld) ...",
				__FUNC__,io->timeout);

#if 1
		h[0] = dio->o.WaitCommEvent.hEvent;
		h[1] = io->abort.hEvent;
		ret = WaitForMultipleObjects(2,h,FALSE,
			io->timeout == -1 ? INFINITE : io->timeout);
#else
		ret = MsgWaitForMultipleObjects(2,h,FALSE,
			io->timeout == -1 ? INFINITE : io->timeout,QS_ALLINPUT);
#endif
		
		if ( io->debug > 1 )
			elLogD("%s(): MsgWaitForMultipleObjects(): %lu",__FUNC__,ret);

		switch( ret )
		{
		case WAIT_FAILED:
			EL_ERR_THROW_WIN(
				"MsgWaitForMultipleObjects",EL_ERR_IO,elErrGetLastErrorStr()
			);
			break;

		case WAIT_TIMEOUT:
			if ( io->debug > 1 )
				elLogD("%s(): MsgWaitForMultipleObjects(): timeout !",__FUNC__);
			elIoSerialAbortReset(io);
			EL_ERR_THROW(EL_ERR_TIMEOUT,NULL,NULL);
			break;

		case WAIT_OBJECT_0 + 0:		/* dio->o.WaitCommEvent.hEvent */
		{	/* inner block */
			/* overlapped 'WaitCommEvent()' has completed: */
			/*
			 * 'cnt' - dummy variable, just for the sake of
			 *		   'GetOverlappedResult()', because 'WaitCommEvent()'
			 *		   doesn't have anything to do with number of bytes
			 *		   transferred ('{Read|Write}File()' does)
			 */
			DWORD cnt;

			if ( io->debug > 1 )
				elLogD("%s(): WaitCommEvent() completed",__FUNC__);
			if (
				!GetOverlappedResult(dev->handle,&dio->o.WaitCommEvent,
					&cnt,FALSE)
			)
			{
				EL_ERR_THROW_WIN(
					"MsgWaitForMultipleObjects/WaitCommEvent",
						EL_ERR_IO,elErrGetLastErrorStr()
				);
			}
			break;
		}	/* inner block */

		case WAIT_OBJECT_0 + 1:		/* dio->abort.hEvent */
			elIoSerialAbortReset(io);
			EL_ERR_THROW(EL_ERR_ABORT,NULL,NULL);
			break;

		/*
		 * Any event in the thread's input queue ?
		 */
		case WAIT_OBJECT_0 + EL_ARRAY_CNT(h):
		{	/* inner block */
			MSG msg;
			bool f;

			/*
			 * Remove all thread's messages now, because messages that would
			 * be left over, would NOT wake up next 'MsgWaitFor...()' !
			 */
			for(f = TRUE; PeekMessage(&msg,NULL,0,0,PM_REMOVE); f = FALSE)
			{
				elLogD("%s(): PeekMessage(): hwnd: 0x%lX, message: %u, wParam: %u, lParam: %lu, time: %lu",
					__FUNC__,msg.hwnd,msg.message,
					msg.wParam,msg.lParam,msg.time
				);
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if ( f )
				/* only for first call... */
				elLog("%s(): PeekMessage(): 0",__FUNC__);
			break;
		}	/* inner block */

		default:
#if 0
			if ( ret < WAIT_OBJECT_0 || ret > WAIT_OBJECT_0 + 1 )
#endif
			{
				EL_ERR_THROW_WIN(
					"MsgWaitForMultipleObjects",EL_ERR_IO,elErrWinStr(ret)
				);
			}
			break;

		}	/* switch( ret ) */
	}	/* asynchronous 'WaitCommEvent()' */

#if 1
	if ( io->debug > 1 )
#endif
		evt_mask_logd(__FUNC__,*pmask);

Error:

	return err;
}

/******************************************************************************/
/*
 * elIoSerialWaitForOverlapped() - Wait for overlapped operation to complete.
 */
#undef	__FUNC__
#define	__FUNC__	"elIoSerialWaitForOverlapped"
static elErr_t *elIoSerialWaitForOverlapped(
	elIo_t *io,DWORD *p_len,DWORD timeout
)
{
	EL_ERR_VAR;
	elIoSerial_t		*dio = io->dio;
	elIoSerialDev_t		*dev = &dio->dev;
	HANDLE				h[2];
	DWORD				ret;

	if ( io->debug > 1 )
		elLogD("%s(): WaitForMultipleObjects(timeout=%ld) ...",__FUNC__,timeout);
	h[0] = dio->o.ReadWriteFile.hEvent;
	h[1] = io->abort.hEvent;
	ret = WaitForMultipleObjects(2,h,FALSE,
			timeout == -1 ? INFINITE : timeout);
	switch( ret )
	{
	case WAIT_OBJECT_0 + 0:
		if ( !GetOverlappedResult(dev->handle,&dio->o.ReadWriteFile,p_len,FALSE) )
		{
			EL_ERR_THROW_WIN(
				"GetOverlappedResult",EL_ERR_IO,elErrGetLastErrorStr()
			);
		}
		return NULL;

	case WAIT_OBJECT_0 + 1:
		elIoSerialAbortReset(io);
		EL_ERR_THROW(EL_ERR_ABORT,NULL,NULL);
		break;

	case WAIT_TIMEOUT:
		if ( io->debug > 1 )
			elLogD("%s(): time out",__FUNC__);
		elIoSerialAbortReset(io);
		EL_ERR_THROW(EL_ERR_TIMEOUT,NULL,NULL);
		break;
	}

	elLog("%s(): WaitForMultipleObjects(): %W",__FUNC__);

Error:

	return err;
}

/******************************************************************************/
/*
 * 'elIoSerialOpenDev()':
 */
/* Open I/O descriptor to the serial device on the given serial port */
static elErr_t *elIoSerialOpenDev(
	elIoParam_t iop,	/* I/O descriptor */
	char *devname,		/* device (COM port) name (e.g. "COM3") */
	char *attrs			/* serial/RS-232 I/O attributes (e.g. "115200/8/n/1/n") */
)
#undef	__FUNC__
#define	__FUNC__	"elIoSerialOpenDev"
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoSerial_t		*dio = io->dio;
	elIoSerialDev_t		*dev = &dio->dev;
	elStr_t				*dev_str;
	HANDLE				fh;
	DCB					dcb;
	COMMTIMEOUTS		timeouts;
	DWORD				mask;
	int					size;
	register int		i;

	io->DebugLog(io,1,_T("%s(): devname: '%s', attrs: '%s'"),
		__FUNC__,devname,attrs);

	EL_ERR_CHECK( elNew(elStr_c,&dev_str) );
	dev_str->Fmt(dev_str,"\\\\.\\%s",devname);
	
	fh = CreateFile(
		dev_str->ptr,			// LPCTSTR lpFileName
		GENERIC_READ | GENERIC_WRITE,
								// DWORD dwDesiredAccess
#if 1
		0,						// DWORD dwShareMode
#else
		FILE_SHARE_READ | FILE_SHARE_WRITE,
#endif
		NULL,					// LPSECURITY_ATTRIBUTES lpSecurityAttributes
		OPEN_EXISTING,			// DWORD dwCreationDisposition
#if IO_OVERLAPPED
		FILE_FLAG_OVERLAPPED,	// DWORD dwFlagsAndAttributes
#else
		0,						// DWORD dwFlagsAndAttributes
#endif
		NULL					// HANDLE hTemplateFile
	);
	if ( fh == INVALID_HANDLE_VALUE )
	{
		EL_ERR_THROW_WIN(
			"CreateFile",
			EL_ERR_IO,
			elStrTmpFmt("%s: %s",devname,elErrGetLastErrorStr())
		);
	}

	dcb.DCBlength = sizeof(dcb);
	io->DebugLog(io,2,_T("GetCommState() ..."));
	EL_ERR_CHECK_WIN( GetCommState(fh,&dcb), EL_ERR_IO,NULL );
	io->DebugLog(io,2,_T("GetCommState(): done"));
	if ( attrs )	/* user attributes ("device[,attrs]") ? */
	{
		/* brate, chsize, parity, stop-bits, hand-shake, abort-on-error */
		char attrs_str[128+EL_ST];
		char *attr[6];
		char *p;
		register int i;

		/*
		 * Copy the attribute string to local variable so that we can modify it
		 * (if the source is constant string literal, it can reside in read-only
		 * segment...):
		 */
		strcpy(attrs_str,attrs);
		/*
		 * Attributes are separated either by '/' or ','. The comma (',') is
		 * used when the user wants to use full specification of hand-shake
		 * mode that contains slash ('/') (e.g. "XON/XOFF" and others):
		 */
		for(i = 0, p = attrs_str;
			i < EL_ARRAY_CNT(attr) && (p = strtok(p,"/,")) != NULL;
			i++, p = NULL
		)
			attr[i] = p;
		EL_ERR_CHECK_COND( !dcb_set(&dcb,i,attr), -1,NULL );
	}
	if ( io->debug > 1 )
		dcb_display(&dcb,TRUE);

	if ( io->debug > 1 )
		elLogD("SetCommState() ...");
	EL_ERR_CHECK_WIN( SetCommState(fh,&dcb), EL_ERR_IO,NULL );
	if ( io->debug > 1 )
		elLogD("SetCommState(): done");

	/*
	 * Set up I/O timeouts appropriately:
	 *
	 * We need to set up read timeout so that synchronous (non-overlapped)
	 * 'ReadFile()' (after 'WaitCommEvent()') will not block when we will
	 * reach EOF (0 bytes returned) -- no other data available. Without this
	 * timeout, the 'ReadFile()' would block when there would be no other
	 * data available.
	 *
	 * We also need to set up zero write timeout so that synchronous
	 * (non-overlapped) 'WriteFile()' will not block in case output buffer
	 * is full (for whatever reason).
	 */
	EL_ERR_CHECK_WIN( GetCommTimeouts(fh,&timeouts), EL_ERR_IO,NULL );
	/*
	 * Read timeout: MAXDWORD, 0, 0
	 * - read operation is to return immediately with the characters that
	 *	 have already been received, even if no characters have been
	 *	 received (i.e. read operation does not block)
	 */
	timeouts.ReadIntervalTimeout = MAXDWORD;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;
	/*
	 * Write timeout: 0, n
	 * - write operation will not block
	 */
#if 1
	timeouts.WriteTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 0;		/* no timeout */
#else
	/*
	 * 9600 baud is approximately 1 byte per millisecond. Allow double
	 * the expected time per character:
	 */
	timeouts.WriteTotalTimeoutMultiplier = 2*9600/gs_dev.brate;
	timeouts.WriteTotalTimeoutConstant = 1;
#endif
	EL_ERR_CHECK_WIN( SetCommTimeouts(fh,&timeouts), EL_ERR_IO,NULL );

#if 1
	size = 128 * 1024;
#else
	size = 4 * 1024
#endif
	EL_ERR_CHECK_WIN( SetupComm(fh,size,size), EL_ERR_IO,NULL );

	EL_ERR_CHECK_WIN( 
		PurgeComm(fh,
			PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR),
		EL_ERR_IO,NULL
	);

	dev->handle = fh;
	dev->dcb	= dcb;

#if IO_OVERLAPPED
	memset(&dio->o.ReadWriteFile,0,sizeof(dio->o.ReadWriteFile));
	EL_ERR_CHECK_WIN(
		dio->o.ReadWriteFile.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL),
		EL_ERR_IO,NULL
	);
	memset(&dio->o.WaitCommEvent,0,sizeof(dio->o.WaitCommEvent));
	EL_ERR_CHECK_WIN(
		dio->o.WaitCommEvent.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL),
		EL_ERR_IO,NULL
	);
	dio->o.on = IO_OVERLAPPED;
#endif

	/*
	 * Set up event mask:
	 */
	for(i = mask = 0; i < COMM_EVTs; i++)
		mask |= gs_comm_evt[i].id;
	if ( io->debug )
		elLogD("%s(): SetCommMask(0x%lX,0x%lX) ...",__FUNC__,dev->handle,mask);
	EL_ERR_CHECK_WIN( SetCommMask(dev->handle,mask), EL_ERR_IO,NULL );
	if ( io->debug )
		elLogD("%s(): SetCommMask(0x%lX,0x%lX): done",
			__FUNC__,dev->handle,mask);

	io->item_size.bits	= 8;
	io->item_size.bytes	= 1;
	io->item_size.bpb	= 8;

Error:

	elDelete(&dev_str);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoSerialCloseDev"
static elErr_t *elIoSerialCloseDev(elIoParam_t iop)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoSerial_t		*dio = io->dio;
	elIoSerialDev_t		*dev = &dio->dev;

	// elLog("%s(): io: 0x%lX",__FUNC__,io);
	if ( io->debug )
		elLog("%s(): start",__FUNC__);

	if ( !CloseHandle(dev->handle) )
		elLog("%s(): %s: CloseHandle(0x%lX): %W",
			__FUNC__,io->name,dev->handle);
	else
	{
#if OUTPUT_DEBUG_STRING
		{
		char str[128+EL_ST];

		sprintf(
			str,
			"%s(PID=0x%lX,TID=0x%lX): %s (io: 0x%lX): CloseHandle(0x%lX): TRUE",
			__FUNC__,GetCurrentProcessId(),GetCurrentThreadId(),
			io->name,io,dev->handle
		);
		OutputDebugString(str);
		}
#endif
	}

#if IO_OVERLAPPED
	if ( !CloseHandle(dio->o.ReadWriteFile.hEvent) )
		elLog("%s(): %s: CloseHandle(0x%lX): %W",
			__FUNC__,io->name,dio->o.ReadWriteFile.hEvent);
	if ( !CloseHandle(dio->o.WaitCommEvent.hEvent) )
		elLog("%s(): %s: CloseHandle(0x%lX): %W",
			__FUNC__,io->name,dio->o.WaitCommEvent.hEvent);
#endif

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoSerialBufRead"
static elErr_t *elIoSerialBufRead(
	elIoParam_t iop,byte *buf,elUInt32_t bufsize,elUInt32_t *p_buflen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoSerial_t		*dio = io->dio;
	elIoSerialDev_t		*dev = &dio->dev;
	DWORD mask;
	DWORD l, len;

#if 0
	elLogD("%s(): start",__FUNC__);
#endif

	/*
	 * If necessary, wait for data to arrive:
	 *
	 * If the last 'ReadFile()' (usually from previous call) did not return
	 * all requested bytes (i.e. it returned either partial data or no data
	 * at all), then we are going to wait via 'WaitCommEvent()' for new data
	 * to arrive.
	 *
	 * If the last 'ReadFile()' from previous call returned all requested
	 * bytes, then we have to try another 'ReadFile()' without waiting,
	 * because there still might be some data in the input buffer that have
	 * been already indicated by last 'WaitCommEvent()' (but have not been
	 * read yet) and would not be reported by next 'WaitCommEvent()', which
	 * would block even though there would be these data in the input buffer
	 * waiting to be read. This happens when data are received in large
	 * blocks that are greater than I/O buffer/cache (max. requested number
	 * of bytes to read per 'ReadFile()') or when the data are being
	 * received faster than caller is retrieving the data.
	 */
	if ( !io->poll && !dio->last_read_full )
	{
	wait_comm_event:
		do
		{
			EL_ERR_CHECK( elIoSerialWaitCommEvent(io,&mask) );

			if ( mask & EV_ERR )
			{
				COMSTAT comstat;
				DWORD errflags;

				if ( !ClearCommError(dev->handle,&errflags,&comstat) )
					elLog("%s(): ClearCommError(): %W",__FUNC__);
				else
				{
#if 1
					if ( io->debug )
#endif
					{
						elLog("%s(): --- EV_ERR ---",__FUNC__);
						err_flags_logd(errflags);
#if 1
						comstat_logd(&comstat);
#endif
					}
					elIoSerialClearError(io);
				}
			}
		} while( !(mask & EV_RXCHAR) );

	}	/* if ( !io->last_read_full ) */

#if 0
	{	/* inner block */
	COMSTAT comstat;
	DWORD errflags;

	/*
	 * Number of bytes in I/O buffers:
	 */
	if ( !ClearCommError(dev->handle,&errflags,&comstat) )
	{
		elLog("%s(): ClearCommError(): %W",__FUNC__);
		return OLS_ERR;
	}
	}	/* inner block */
#endif

	/*
	 * Read from serial port until no data are available:
	 */
	for(len = 0, l = 1; l && (int)len < bufsize; len += l)
	{
	read:
		if ( io->debug )
			elLogD("%s(): ReadFile(%d) ...",__FUNC__,bufsize-len);
		if ( !ReadFile(dev->handle,buf+len,bufsize-len,&l,
				dio->o.on ? &dio->o.ReadWriteFile : NULL) )
		{
			switch( GetLastError() )
			{
			case ERROR_OPERATION_ABORTED:
				elIoSerialClearError(io);
				goto read;
				break;
			case ERROR_IO_PENDING:
				EL_ERR_CHECK( elIoSerialWaitForOverlapped(io,&l,(DWORD)-1) );
				break;
			default:
				EL_ERR_THROW_WIN(
					"ReadFile",EL_ERR_IO,
					elStrTmpFmt("%s: ReadFile(%d): %s",io->name,
						bufsize-len,elErrGetLastErrorStr())
				);
				break;
			}
		}
		if ( io->debug )
			elLogD("%s(): ReadFile(): %ld",__FUNC__,l);

		if ( dio->capture.enable && l )
		{
			int l2;

			l2 = (int) fwrite(buf+len,sizeof(*buf),l,dio->capture.file.fp);
			if ( (uint)l2 != l )
				elLog("%s(): %s: fwrite(): %d/%d",
					__FUNC__,dio->capture.file.path,l2,l);
		}

		dio->last_read_full = (bool) (l == bufsize - len);

		if ( !l )
		{
			/*
			 * Note:
			 *
			 * It can happen, that no data are read by 'ReadFile()' even if
			 * we have waited via 'WaitCommEvent()' for event 'EV_RXCHAR'
			 * (character received) and this event has occurred. This is
			 * not a bug and no work around can be used to avoid this
			 * situation. The following pseudocode explains how this can
			 * happen:
			 *
			 *      for(;;) {
			 *          WaitCommEvent(EV_RXCHAR);
			 *          puts("EV_RXCHAR received");
			 *          //
			 *          // At this time, new data arrive and 'EV_RXCHAR' is
			 *          // set internally to be indicated by next
			 *          // 'WaitCommEvent()'.
			 *          //
			 *          // The following 'ReadFile()' reads all available
			 *          // data including new data that have just arrived,
			 *          // thus "swallowing" data that will be indicated by
			 *          // next 'WaitCommEvent()'. The next 'ReadFile()'
			 *          // will return zero bytes, because the data have
			 *          // been already read by previous 'ReadFile()'...
			 *          //
			 *          ReadFile(bufsize); [or] while( ReadFile(bufsize) );
			 *      }
			 *
			 * We cannot avoid this situation with 'SetCommMask()' being
			 * called just before 'WaitCommEvent()' in the above code,
			 * because then the 'WaitCommEvent()' would not indicate data
			 * that happened to arrive between last 'ReadFile()' and
			 * subsequent 'SetCommMask()'.
			 *
			 * 'EV_RXCHAR' event does not necessarily indicates, that there
			 * are some data available in the input buffer, it just
			 * indicates, that there were some data put in the input buffer
			 * since last call to either 'WaitCommEvent()' or
			 * 'SetCommMask()'.
			 */

			/* are we polling or have we already gathered some data ? */
			if ( io->poll || len )
			{
				/*
				 * Return whatever data have been read (even if none) to caller:
				 */
				*p_buflen = len;

				/* if no data have been read, indicate this with error ID: */
				if ( !len )
					EL_ERR_THROW(EL_ERR_NO_DATA,NULL,NULL);

				break;
			}

			/*
			 * There are no data available and so far we haven't read anything.
			 * We need to wait for some data to arrive:
			 */
			goto wait_comm_event;

		}	/* if ( !l ) */

	}	/* for(len = 0, l = 1; l && (int)len < bufsize; len += l) */

	*p_buflen = len;

	/*
	 * Return all data that have been read so far:
	 */
	if ( io->debug > 1 )
	{
		elLogD("%s(): data (%d bytes):",__FUNC__,len);
		elLogHexDump(buf,len,NULL,"-i2");
	}

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoSerialBufFill"
static elErr_t *elIoSerialBufFill(elIoParam_t iop)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elUInt32_t		len;

	EL_ERR_CHECK( elIoSerialBufRead(io,io->buf.base,io->buf.size,&len) );
	io->buf.len = len;
#if 0
	elLogD("%s(): len: %ld",__FUNC__,len);
#endif
	io->buf.ptr = io->buf.base;

Error:

	return err;
}

/******************************************************************************/
/*
 * 'elIoSerialGetc()':
 */
/** Read a character from the serial device
 */
static elErr_t *elIoSerialGetc(
	elIoParam_t iop,	/*!< I/O descriptor */
	int *pchar			/*!< pointer where to store the character read */
)
#undef	__FUNC__
#define	__FUNC__	"elIoSerialGetc"
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;

	EL_IO_ABORT_CHECK(io);

	if ( !io->buf.len )
		EL_ERR_CHECK( elIoSerialBufFill(io) );

	if ( io->buf.len )
	{
		io->buf.len--;
		*pchar = *io->buf.ptr++;

		if ( io->debug )
			elLogD("%s(): *pchar: 0x%02X = %3d = '%c'",
				__FUNC__,*pchar,*pchar,isprint(*pchar) ? *pchar : '.');
	}
	else
		*pchar = EL_IO_EOF;

Error:

	return err;
}

/******************************************************************************/
/*
 * 'elIoSerialAbortReset()':
 */
/** Reset/Acknowledge aborted I/O operation
 */
static elErr_t *elIoSerialAbortReset(
	elIoParam_t iop		/*!< I/O descriptor */
)
#undef	__FUNC__
#define	__FUNC__	"elIoSerialAbortReset"
{
	EL_IO_PARAM_VAR;

#if 1
	/*
	 * 'CancelIo()' instead of 'CloseFile()' + 'CreateFile()' is necessary
	 * for some serial devices (see "WARNING" below).
	 */
	EL_ERR_VAR;
	elIoSerial_t		*dio = io->dio;
	elIoSerialDev_t		*dev = &dio->dev;

	EL_ERR_CHECK_WIN( CancelIo(dev->handle), EL_ERR_IO,NULL );

Error:

	return err;
#else
	/*
	 * After an I/O timeout or I/O aborted by user, the overlapped I/O is still
	 * in progress (all subsequent I/O calls return 'ERROR_INVALID_PARAMETER')
	 * until finished or cancelled. To cancel overlapped I/O, we could call
	 * 'CancelIo()', but this routine is not available on Win95 or WinNT 3.1.
	 * Another way how to cancel the pending overlapped I/O is to close
	 * the I/O handle. This works on all Windows platforms. To make this I/O
	 * timeout handling transparent to the caller, we will close and reopen
	 * the serial I/O right here.
	 *
	 * WARNING: Virtual COM port driver from Symbol/Motorola (used for
	 * DataMatrix / Bar Code reader Symbol DS6608) creates 3 or 4 threads
	 * during 'CreateFile()' call, but does not wait for the thread handles
	 * to become signaled when the threads are finished (i.e. Win32 handle
	 * leaks occur). It means that if the port is closed and reopened
	 * periodically very often, then the application can freeze/hang
	 * when the number of opened handles in the process reaches some
	 * limit (e.g. ~57000).
	 */
	elIoSerialCloseDev(io);
	return elIoSerialOpenDev(io,io->name,io->str.attrs->ptr);
#endif
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoSerialOpen"
elErr_t *elIoSerialOpen(elIoParam_t iop,TCHAR *rsrc,...)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoSerial_t	*dio;
	elIoSerialDev_t	*dev;
	elStr_t			*s;
	TCHAR			*p;

	(void) rsrc;
	(void) dio;

	EL_ERR_CHECK( elNew(elIoSerial_c,&io->dio) );
	EL_ERR_CHECK( elNew(elStr_c,&s) );

	dio = io->dio;
	dio->io = io;
	dev = &dio->dev;

	/*
	 * Parse I/O path string ('io->str.path'):
	 *
	 *		device[{,/}attrs]
	 *
	 * For example:
	 *
	 *		COM1/115200/8/n/1/n
	 */
	EL_ERR_CHECK( elStrCpy(s,io->str.path->ptr) );
	p = s->ptr;
	if ( (p = strpbrk(p,",/")) != NULL )
	{
		*p = _T('\0');
		elStrCpy(io->str.path,s->ptr);
		elStrCpy(io->str.attrs,p+1);
	}

	elStrCpy(io->str.name,io->str.path->ptr);

	EL_ERR_CHECK( elIoSerialOpenDev(io,io->str.path->ptr,io->str.attrs->ptr) );

	EL_ERR_CHECK( elIoBufAlloc(&io->buf,1024) );

Error:

	elDelete(&s);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoSerialClose"
elErr_t *elIoSerialClose(elIoParam_t iop)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoSerial_t		*dio = io->dio;

	(void) dio;

	elIoSerialCloseDev(io);

	elDelete(&io->dio);

	;

#if 0
Error:
#endif

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoSerialRead"
elErr_t *elIoSerialRead(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoSerial_t	*dio = io->dio;
	elIoSerialDev_t	*dev = &dio->dev;
	byte			*buf0 = buf;
	elUInt32_t		buflen;
	int				ch;

	(void) dev;
	(void) buf;

	if ( bufsize <= 0 )
		bufsize = io->buf.size;

	for(buflen = 0; buflen < bufsize; )
	{
		err = elIoSerialGetc(io,&ch);
		if ( err )
		{
			if ( io->debug )
			{
				elLogD(
					"%s(): elIoSerialGetc(): %d/%d bytes read so far:",
					__FUNC__,buflen,bufsize
				);
				elLogHexDump(buf0,buflen,NULL,"-i2");
			}
#if 0
			elLogD("%s(): return %d",__FUNC__,ret);
#endif
			return err;
		}
		*buf++ = (byte) ch;
		buflen++;
	}

	*plen = buflen;

#if 0
Error:
#endif

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoSerialWrite"
elErr_t *elIoSerialWrite(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoSerial_t	*dio = io->dio;
	elIoSerialDev_t	*dev = &dio->dev;

	(void) dev;
	(void) buf;

	if ( bufsize <= 0 )
		bufsize = io->buf.size;

	;

	*plen = io->buf.size;

#if 0
Error:
#endif

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoSerialMsgRead"
elErr_t *elIoSerialMsgRead(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	byte			*buf0 = buf;
	elUInt32_t		buflen;
	int				ch;

	*plen = 0;

	if ( bufsize <= 0 )
		bufsize = io->buf.size;

	if ( io->msg.buf.size < bufsize )
		EL_ERR_CHECK( elIoBufAlloc(&io->msg.buf,bufsize) );
		
	io->msg.complete = FALSE;
	for(buflen = 0; buflen < bufsize; )
	{
		err = elIoSerialGetc(io,&ch);
		if ( err )
		{
			if ( io->debug )
			{
				elLogD(
					"%s(): elIoSerialGetc(): %d/%d bytes read so far:",
					__FUNC__,buflen,bufsize
				);
				elLogHexDump(buf0,buflen,NULL,"-i2");
			}
#if 0
			elLogD("%s(): return %d",__FUNC__,ret);
#endif
			goto Error;
		}

		if ( ch == io->msg.term_char )
		{
			io->msg.complete = TRUE;		/* end of message */
			break;
		}
		*io->msg.buf.ptr++ = (byte) ch;
		io->msg.buf.len++;
	}

	if ( io->msg.complete )
	{
		elUInt32_t	len;

		len = io->msg.buf.len;
#if 1
		EL_ERR_CHECK_COND( len > bufsize, EL_ERR_INVAL,NULL );
#else	/* debug test: */
		EL_ERR_CHECK_COND( 1 < 2, EL_ERR_INVAL,NULL );
#endif
		memcpy(buf,io->msg.buf.base,len);
		*plen = len;
		io->msg.buf.ptr = io->msg.buf.base;
		io->msg.buf.len = 0;
	}

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoSerialMsgWrite"
elErr_t *elIoSerialMsgWrite(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoSerial_t	*dio = io->dio;
	elIoSerialDev_t	*dev = &dio->dev;

	(void) buf;
	(void) dev;

	*plen = 0;

	if ( bufsize <= 0 )
		bufsize = io->buf.size;

	;

	*plen = io->buf.size;

#if 0
Error:
#endif

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoSerialCtrl"
elErr_t *elIoSerialCtrl(elIoParam_t iop,int cmd,void *pdata)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoSerial_t	*dio = io->dio;
	elIoSerialDev_t	*dev = &dio->dev;

	(void) pdata;

	switch( cmd )
	{
	case EL_IO_CTRL_DISCARD_INPUT:
		if ( !PurgeComm(dev->handle,PURGE_RXCLEAR) )
		{
			elLog("PurgeComm(): %W");
			EL_ERR_THROW(EL_ERR_IO,NULL,NULL);
		}
		elIoBufClear(&io->buf);
		break;

	case EL_IO_CTRL_FLUSH_OUTPUT:
		;
		break;
	}

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoSerialAttrSet"
elErr_t *elIoSerialAttrSet(elIoParam_t iop,TCHAR *attr_name,TCHAR *attr_val)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoSerial_t	*dio = io->dio;

	(void) dio;
	(void) attr_val;

	if ( !stricmp(attr_name,"attr1") )
	{
		;
	}
	else if ( !stricmp(attr_name,"attr2") )
	{
		;
	}
	else if ( !stricmp(attr_name,"POLL") )
	{
		/*
		 * No special treatment, attribute "POLL" set at generic I/O level
		 * ("elIo.c").
		 */
		 ;
	}
	else
	{
		EL_ERR_THROW(
			EL_ERR_INVAL,NULL,elStrTmpFmt("%s: unknown attribute",attr_name)
		);
	}

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoSerialAttrGet"
elErr_t *elIoSerialAttrGet(elIoParam_t iop,TCHAR *attr_name,TCHAR **p_attr_val)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoSerial_t	*dio = io->dio;

	(void) dio;
	(void) p_attr_val;

	if ( !stricmp(attr_name,"attr1") )
	{
		;
	}
	else if ( !stricmp(attr_name,"attr2") )
	{
		;
	}
	else
	{
		EL_ERR_THROW(
			EL_ERR_INVAL,NULL,elStrTmpFmt("%s: unknown attribute",attr_name)
		);
	}

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoSerialTaskRead"
elErr_t *elIoSerialTaskRead(
	elIoTask_t *task,
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;

	(void) task;
	(void) buf;
	(void) bufsize;
	(void) plen;
	(void) io;

	;

#if 0
Error:
#endif

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoSerialTaskWrite"
elErr_t *elIoSerialTaskWrite(
	elIoTask_t *task,
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;

	(void) task;
	(void) buf;
	(void) bufsize;
	(void) plen;
	(void) io;

	;

#if 0
Error:
#endif

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoSerialTaskExecute"
elErr_t *elIoSerialTaskExecute(elIoTask_t *task)
{
	EL_ERR_VAR;
	elIo_t			*io;
	elErr_t			*io_err;
	elUInt32_t		len;
	register int	i;

#if 0
	elLogD(_T("%s(): start"),_T(__FUNC__));
#endif

	for(i = 0; i < task->io_cnt; i++)
	{
		io = task->io[i];

		if ( io->intfc != elIoSerialIntfc )
			continue;

		io_err = elIoSerialRead(io,io->buf.ptr,io->buf.size,&len);
		if ( io_err )
		{
			/* save error: */
			EL_ERR_CHECK( elMemRealloc(&io->err,sizeof(*io->err)) );
			*io->err = *io_err;
		}
		else
			elMemFree(&io->err);
	}

#if 0
	elLogD(_T("%s(): end"),_T(__FUNC__));
#endif

	;

Error:

	return err;
}

/******************************************************************************/
