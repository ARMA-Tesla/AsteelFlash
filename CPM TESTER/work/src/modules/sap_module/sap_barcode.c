#include "sap_barcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <elerror.h>

#define HBRLOG( priority, message) 
/*
	if ( pHBR->_LogFnc ) { \
		pHBR->_LogFnc( priority, message); \
	} else
*/
/**
 * Specifies time between, when same barcode is ignored in seconds
 */
#define HBR_MIN_TIME			2
/** Specifies how long read thread sleeps, before read value from hand barcode reader. */
#define HBR_SLEEP_TIME				5000

/*****************************************************************************
 * Static no-exported functions declaration **********************************
 *****************************************************************************/
static SElExceptionPtr handbr_Initialize(
    SHBarcodeReaderPtr*  pHBR
);
static SElExceptionPtr handbr_Lock(
    struct _SHBarcodeReader*	pHBR
);
static SElExceptionPtr handbr_Unlock(
    struct _SHBarcodeReader*	pHBR
);
static SElExceptionPtr handbr_Run(
    struct _SHBarcodeReader*	pHBR
);
static SElExceptionPtr handbr_Terminate(
    struct _SHBarcodeReader*	pHBR,
    int32_t					timeout
);
static SElExceptionPtr handbr_GetLastItem(
	struct _SHBarcodeReader* me,
	SBCItemPtr pHBR
);
SElExceptionPtr handbrSetLogCallback(
	struct _SHBarcodeReader* me,
	LogCallbackPtr LogFnc
);

static SElExceptionPtr handbr_ResetItem(
	struct _SHBarcodeReader* me
);
static SElExceptionPtr handbr_RegisterCallbackFnc(
	struct _SHBarcodeReader* me,
	BarcodeReaderCallbackPtr callaback
);
static SElExceptionPtr handbr_DisableCallbackFnc(
	struct _SHBarcodeReader* me, 
	bool_t disable
);
static SElExceptionPtr handbr_RegisterCallbackFnc2(
	struct _SHBarcodeReader* me,
		BarcodeReaderCallbackPtr callback
);
static SElExceptionPtr handbr_IsRunning(
    struct _SHBarcodeReader*	pHBR,
	int32_t*				pIsRunning
);
static SElExceptionPtr handbr_AddItem(
    struct _SHBarcodeReader*	pHBR,
	const SBCItemPtr	pItem
);
static SElExceptionPtr	handbr_OpenSerial(
	struct _SHBarcodeReader*	pHBR,
	SSerialIntfBasePtr	COMSettings,
	HANDLE*				handle
);
static void handbr_LogWinError(
	const char* name,
	uint32_t	line
);

DWORD WINAPI Thread_HBarcodeReader( LPVOID lpParam );


/*****************************************************************************
 * Exported functions definition *********************************************
 *****************************************************************************/
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "handbr_new"
/*!
 * \brief
 * This function creates a new instance of hand barcode reader.
 * 
 * \param pError 
 * Returns a value indicating whether an error occurred.
 * A negative error code indicates function failure.	
 * 
 * \return 
 * Pointer to new instance of the hand barcode reader object. If the function fails, NULL
 * pointer is returned
 *
 * This function creates a new instance of the hand barcode reader object.
 */
SElExceptionPtr handbr_new(
	SHBarcodeReaderPtr*  pHBR
)
{
	SElExceptionPtr	pexception = NULL;
	SHBarcodeReaderPtr		phbr = NULL;

	EXCCHECK( handbr_Initialize(&phbr));

Error:
	if ( pexception )
	{
		handbr_delete( &phbr);
	}
	*pHBR = phbr;
	
	EXCRETHROW( pexception);
} /* handbr_new */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "handbr_delete"
/*!
 * \brief
 * This function destroys specified instance.
 * 
 * \param pKOSU 
 * Pointer to instance of the hand barcode reader object cretaed by handbr_new() function.
 * 
 * \return 
 * A value indicating whether an error occurred.
 * A negative error code indicates function failure.	
 *
 * This function destroys specified instance and release all resources.
 */
SElExceptionPtr handbr_delete(SHBarcodeReaderPtr* pHBR)
{
	SElExceptionPtr	pexception = NULL;

	if (*pHBR)
	{
		(*pHBR)->Terminate((*pHBR), 0);
		if ((*pHBR)->ThreadHandle) {
			CloseHandle((*pHBR)->ThreadHandle);
			(*pHBR)->ThreadHandle = 0;
		}
		if ((*pHBR)->QuitEvent) {
			CloseHandle((*pHBR)->QuitEvent);
			(*pHBR)->QuitEvent = 0;
		}
		if ((*pHBR)->COMHandle) {
			CloseHandle((*pHBR)->COMHandle);
			(*pHBR)->COMHandle = 0;
		}
		if((*pHBR)->_CallBackFnc) {
			free((*pHBR)->_CallBackFnc);
			(*pHBR)->_CallBackFnc = NULL;
		}
		DeleteCriticalSection(&((*pHBR)->cs));
		free(*pHBR);
		*pHBR = NULL;
	} /* if (*pHBR) { ...*/

Error:
	EXCRETHROW( pexception);
} /* handbr_delete */

/*============================================================================*/
#undef __FUNC__
#define __FUNC__ "handbr_IsRunning"
static SElExceptionPtr handbr_IsRunning(
    struct _SHBarcodeReader*	pHBR,
	int32_t*				pIsRunning
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t		error = 0;
	int32_t		state = 0;
	DWORD		exitCode = 0;
	
	if (pHBR->ThreadHandle) {
		EXCCHECKWIN( GetExitCodeThread( pHBR->ThreadHandle, &exitCode));
		if ( exitCode == STILL_ACTIVE ) {
			state = 1;	
		}
		else
		{
			EXCCHECKWIN( CloseHandle(pHBR->QuitEvent));
			EXCCHECKWIN( CloseHandle(pHBR->ThreadHandle));

			pHBR->QuitEvent    = 0;
			pHBR->ThreadHandle = 0;
			pHBR->ThreadID     = 0;
		}
	}

Error:
	*pIsRunning = state;
	EXCRETHROW( pexception);
} /* handbr_IsRunning */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "handbr_Terminate"
static SElExceptionPtr handbr_Terminate(
    struct _SHBarcodeReader*	pHBR,
    int32_t					timeout
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t 		is_running;
	BOOL			error;

	EXCCHECK( handbr_IsRunning(pHBR, &is_running));

	if ( is_running )
	{
		DWORD	state;
		DWORD exitCode;
		EXCCHECKWIN( SetEvent(pHBR->QuitEvent));
		/*
		 * TODO: wait timeout and check is _running
		 */
		state = WaitForSingleObject( pHBR->ThreadHandle, 1000);
		if ( state == WAIT_TIMEOUT )
		{
			HBRLOG( ELOG_ERR, "Thread does not terminate.");
		}
		GetExitCodeThread( pHBR->ThreadHandle, &exitCode);
		if ( exitCode == STILL_ACTIVE )
		{
			HBRLOG( ELOG_WARNING, "Thread will be terminateds.");			
			TerminateThread( pHBR->ThreadHandle, 1);
		}
		EXCCHECKWIN( CloseHandle( pHBR->ThreadHandle));
		pHBR->ThreadHandle = 0;
	}

Error:
	EXCRETHROW( pexception);
} /* handbr_Terminate */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "handbr_GetLastItem"
static SElExceptionPtr handbr_GetLastItem(
	struct _SHBarcodeReader* pHBR,
	SBCItemPtr pItem
)
{
	SElExceptionPtr	pexception = NULL;

	EXCCHECK( handbr_Lock( pHBR));
	memcpy( pItem, &pHBR->item, sizeof(SBCItem));

Error:
	handbr_Unlock( pHBR);
	EXCRETHROW( pexception);
} /* handbr_GetLastItem */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "handbrSetLogCallback"
SElExceptionPtr handbrSetLogCallback(
	struct _SHBarcodeReader* pHBR,
	LogCallbackPtr LogFnc
)
{
	pHBR->_LogFnc = LogFnc;
	return NULL;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "handbr_ResetItem"
static SElExceptionPtr handbr_ResetItem(
	struct _SHBarcodeReader* pHBR
)
{
	SElExceptionPtr	pexception = NULL;

	EXCCHECK( handbr_Lock( pHBR));
	memset( &pHBR->item, 0, sizeof(SBCItem));

Error:
	handbr_Unlock( pHBR);
	EXCRETHROW( pexception);
} /* handbr_ResetItem */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "handbr_RegisterCallbackFnc"
/*!
 * \brief
 * This function register a callback which is invoke, when new data are
 * recived.
 * 
 * \param pHBR
 * Specifies hand barcode reader cretaed by handbr_new function.
 * 
 * \param callaback
 * Pass pointer to function, which is invoked when a data are received.
 * You can pass a NULL pointer if you want disable callback mechanism.
 * 
 * \returns
 * A value indicating whether an error occurred.
 * A negative error code indicates function failure.	
 * 
 * This function register a callback which is invoke, when new data are
 * recived. If you want disable this mechanism, set NULL as callback.
 */
static SElExceptionPtr handbr_RegisterCallbackFnc(
	struct _SHBarcodeReader* me,
		BarcodeReaderCallbackPtr callback
)
{
	SElExceptionPtr	pexception = NULL;

	EXCCHECK( handbr_Lock( me));

	me->_CallBackFnc = (BarcodeReaderCallbackPtr*)realloc(me->_CallBackFnc, sizeof(BarcodeReaderCallbackPtr)*(me->_CallBackFncSize+1));
	me->_CallBackFnc[me->_CallBackFncSize] = callback;

	me->_CallBackFncSize++;

Error:
	handbr_Unlock( me);
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "handbr_DisableCallbackFnc"
static SElExceptionPtr handbr_DisableCallbackFnc(
	struct _SHBarcodeReader* me, 
	bool_t disable
)
{
	SElExceptionPtr	pexception = NULL;

	EXCCHECK( handbr_Lock( me));

	me->_CallBackFncDisabled = disable;

Error:
	handbr_Unlock( me);
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "handbr_RegisterCallbackFnc2"
/*!
 * \brief
 * This function register a callback which is invoke, when new data are
 * recived.
 * 
 * \param pHBR
 * Specifies hand barcode reader cretaed by handbr_new function.
 * 
 * \param callaback
 * Pass pointer to function, which is invoked when a data are received.
 * You can pass a NULL pointer if you want disable callback mechanism.
 * 
 * \returns
 * A value indicating whether an error occurred.
 * A negative error code indicates function failure.	
 * 
 * This function register a callback which is invoke, when new data are
 * recived. If you want disable this mechanism, set NULL as callback.
 */
static SElExceptionPtr handbr_RegisterCallbackFnc2(
	struct _SHBarcodeReader* me,
		BarcodeReaderCallbackPtr callback
)
{
	SElExceptionPtr	pexception = NULL;

	EXCCHECK( handbr_Lock( me));

	me->_CallBackFnc2 = callback;

Error:
	handbr_Unlock( me);
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
/**
 * This function Run thread which reads information from hand barcode reader
 *
 *
 * \return	A value indicating whether an error occurred.
 *          A negative error code indicates function failure.	
 */
#undef __FUNC__
#define __FUNC__ "handbr_Run"
static SElExceptionPtr handbr_Run(
    struct _SHBarcodeReader*	pHBR
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			is_running = 0;
	DWORD			dw_thread_Id;
	char			log_message[256] = "";

	HBRLOG( ELOG_NOTICE, "Starting ....");

	EXCCHECK( handbr_Lock(pHBR));
	EXCCHECK( handbr_IsRunning(pHBR, &is_running));

	if ( is_running ) {
		HBRLOG( ELOG_WARNING, "Already running ....");
		EXCTHROW( EL_ERROR_OS_SPECIFIC, __FUNC__" Already running ....");
	}

	EXCCHECK( handbr_OpenSerial( pHBR, &pHBR->COM, &pHBR->COMHandle)); 

	pHBR->QuitEvent = CreateEvent( NULL, TRUE, FALSE, NULL);
	if (pHBR->QuitEvent == NULL)
	{
		HBRLOG( ELOG_ERR, "Create event fails");
		EXCTHROW( EL_ERROR_OS_SPECIFIC, " Create event fails");
	}
	
	pHBR->ThreadHandle = CreateThread( 
							NULL,                       /* no security attributes  */
							0,                          /* use default stack size  */
							Thread_HBarcodeReader,				/* thread function  */
							pHBR,					/* argument to thread function */
							0,                          /* use default creation flags  */
							&dw_thread_Id);             /* returns the thread identifier */
	
	sprintf(log_message, "hand barcode reader Thread Started (0x%x)", (unsigned int)(pHBR->ThreadHandle));
	if (pHBR->ThreadHandle == NULL)
	{
		HBRLOG( ELOG_ERR, log_message);
		EXCTHROW( EL_ERROR_OS_SPECIFIC, __FUNC__" hand barcode reader Thread Started ");
	}
	HBRLOG( ELOG_NOTICE, log_message);

	pHBR->ThreadID =  dw_thread_Id;

Error:
	handbr_Unlock( pHBR);
	EXCRETHROW( pexception);
} /* handbr_Run */

/*****************************************************************************
 * Static no-exported functions **********************************************
 *****************************************************************************/
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "handbr_Initialize"
/*!
 * \brief
 * This function creates a new instance of the specified object.
 * 
 * \param pHBR
 * Returns pointer to new instance of the hand barcode reader object.
 *
 * \return	
 * A value indicating whether an error occurred.
 * A negative error code indicates function failure.	
 *
 * This function creates and initialize a new instance of the specified object.
 */
static SElExceptionPtr handbr_Initialize(
    SHBarcodeReaderPtr*  pHBR
)
{
	SElExceptionPtr		pexception = NULL;
    SHBarcodeReaderPtr			pibutton = NULL;

    /*
     * Initialize all data
     */     
    pibutton = malloc(sizeof(SHBarcodeReader));
    EXCCHECKALLOC( pibutton);
    memset(pibutton, 0, sizeof(SHBarcodeReader));
	InitializeCriticalSection(&(pibutton->cs));

	pibutton->Run = handbr_Run;
	pibutton->Terminate      = handbr_Terminate;
	pibutton->RegisterCallbackFnc  = handbr_RegisterCallbackFnc;
	pibutton->DisableCallbackFnc = handbr_DisableCallbackFnc;
	pibutton->RegisterCallbackFnc2 = handbr_RegisterCallbackFnc2;
	pibutton->Reset          = handbr_ResetItem;
	pibutton->GetLastItem    = handbr_GetLastItem;
	pibutton->SetLogCallback = handbrSetLogCallback;
	
	
Error:
    if ( pexception ) {
		if ( pibutton != NULL ){
			DeleteCriticalSection(&(pibutton->cs));
			free(pibutton);
			pibutton = NULL;
		}
    }

    if (pHBR) *pHBR = pibutton;
    EXCRETHROW( pexception);
} /* handbr_Initialize */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "handbr_Lock"
/**
 * This function is used to obtain a lock on the specified resource.
 *
 * \param	pHBR	Pointer to resource which should be locked
 *
 * \return	A value indicating whether an error occurred.
 *          A negative error code indicates function failure.		
 */
static SElExceptionPtr handbr_Lock(
    struct _SHBarcodeReader*	pHBR
)
{
	if ( pHBR != NULL )
	{
		EnterCriticalSection(&(pHBR->cs));
	}

Error:	
	return NULL;
} /* handbr_Lock */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "handbr_Unlock"
/**
 * Relinquishes a lock to the resource associated with the given session.
 *
 * \param	pHBR	Pointer to resource which should be unlocked
 *
 * \return	A value indicating whether an error occurred.
 *          A negative error code indicates function failure.		
 */
static SElExceptionPtr handbr_Unlock(
    struct _SHBarcodeReader*	pHBR
)
{
	if ( pHBR != NULL )
	{
		LeaveCriticalSection(&(pHBR->cs));
	}

Error:	
	return NULL;
} /* handbr_Unlock */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "handbr_AddItem"
/**
 * This function store new iBurron entry in the class and invoke callback for
 * specified session. If the barcode is same as value stored in the class
 * value is accepted as new, when time between old and this new item is 
 * greater as IBUTTON_MIN_TIME.
 *
 * \param   pHBR	Specifies hand barcode reader cretaed by handbr_new function.
 * \param	pItem		Pass a pointer to new iButton item, which has been
 *                      received from iButton reader.
 * \return	A value indicating whether an error occurred.
 *          A negative error code indicates function failure.		
 */
static SElExceptionPtr handbr_AddItem(
    struct _SHBarcodeReader*	pHBR,
	const SBCItemPtr	pItem
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			update = 0;

	EXCCHECK( handbr_Lock( pHBR));
	if ( memcmp(pHBR->item.data, pItem->data, sizeof(pItem->data)) != 0 ) 
	{
		/*
		 * New barcode has been recieved
		 */
		update = 1;
	}
	else
	if (( pItem->TimeStamp - pHBR->item.TimeStamp ) > HBR_MIN_TIME )
	{
		/*
		 * It's same ibutton but time is greater as HBR_MIN_TIME
		 */
		update = 1;	
	}

	if ( update )
	{
		int i = 0;
		char message[256] = "";

		sprintf( message, "New value detected [%s]", pItem->data);
		HBRLOG( ELOG_INFO, message);
		memcpy( &pHBR->item, pItem, sizeof(SBCItem));	

		/* high priority callback */
		if(pHBR->_CallBackFnc2)
			EXCCHECK( (pHBR->_CallBackFnc2)((void*)pHBR, pItem->data));
		/* low priority callbacks */
		if(!pHBR->_CallBackFncDisabled)
		{
			for(i=0; i<pHBR->_CallBackFncSize; i++)
			{
				EXCCHECK( (pHBR->_CallBackFnc[i])((void*)pHBR, pItem->data));
			}
		}
	}

Error:
	handbr_Unlock( pHBR);
    EXCRETHROW( pexception);
} /* handbr_AddItem */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "handbr_OpenSerial"
/**
 * Open serial port with specified configuration.
 *
 * \param	COMSettings		Configuration of serial port
 *
 * \param	handle		Session handle to specified prot
 *
 * \return	A value indicating whether an error occurred.
 *          A negative error code indicates function failure.		
 */
static SElExceptionPtr	handbr_OpenSerial(
	struct _SHBarcodeReader*	pHBR,
	SSerialIntfBasePtr	COMSettings,
	HANDLE*				handle
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error;
	HANDLE			fh = NULL;
	char			file[256];
	DCB				dcb = {0};
	COMMTIMEOUTS		timeouts;

	if (COMSettings->port > 9)
		sprintf(file, "\\\\.\\COM%d", (int)COMSettings->port);
	else
		sprintf(file, "COM%d", (int)COMSettings->port);
		
	/* TODO: Open COM port with default values or from option string values */
	fh = CreateFile(file,
					GENERIC_READ, //|GENERIC_WRITE,//access ( read and write)
					0,    					//(share) 0:cannot share the COM port                        
					0,    					//security  (None)                
					OPEN_EXISTING,			// creation : open_existing
					(pHBR->is_usb) ? 0 : FILE_FLAG_OVERLAPPED,	// we want overlapped operation
					0						// no templates file for COM port...
            	);

	EXCCHECKWIN( !(fh == INVALID_HANDLE_VALUE) )

	dcb.DCBlength = sizeof(DCB);

	EXCCHECKWIN( GetCommState (fh, &dcb));

	dcb.BaudRate    = 9600;
	dcb.ByteSize    = 8;
	dcb.Parity		= NOPARITY;
	dcb.StopBits    = ONESTOPBIT;

	EXCCHECKWIN( SetCommState (fh, &dcb));
	
	/* Register Commcallback */
	EXCCHECKWIN( SetCommMask(fh, EV_RXCHAR));

	EXCCHECKWIN( GetCommTimeouts(fh,&timeouts));

	timeouts.ReadIntervalTimeout = MAXDWORD;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;

	EXCCHECKWIN( SetCommTimeouts(fh,&timeouts));

	if(handle) *handle = fh;

Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
/*
 * LogWinError
 */
static void handbr_LogWinError(
	const char* name,
	uint32_t	line
)
{
	DWORD error = GetLastError();
	LPVOID lpMsgBuf;
#if 0
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);
	// Process any inserts in lpMsgBuf.
	// ...
	
	SLOG_BEGIN( ELOG_ERR, "%s (%s Line:%d)")
		SLOG_P( (LPCTSTR)lpMsgBuf)
		SLOG_P( name)
		SLOG_P( line)
	SLOG_END()
	// Free the buffer.
	LocalFree( lpMsgBuf );
#endif
}



/*****************************************************************************
 * THREAD FUNCTIONS **********************************************************
 *****************************************************************************/
/*---------------------------------------------------------------------------*/
/**
 * Thred function which reads values from hand barcode reader reader
 *
 * \param lpParam	pointer to session associated with this thread
 *
 * \return	A value indicating whether an error occurred.
 *          A negative error code indicates function failure.		
 */
#undef __FUNC__
#define __FUNC__ "Thread_HBarcodeReader"
DWORD WINAPI Thread_HBarcodeReader( LPVOID lpParam ) 
{
	SElExceptionPtr	pexception = NULL;
	SHBarcodeReaderPtr		pHBR = (SHBarcodeReaderPtr) lpParam;
	int32_t     	error = 0, failed = 0;
	DWORD			wrt_count;
	DWORD			rd_count, count;
	DWORD			event_mask = 0;
	SBCItem			barcode_item;
	char			log_message[256]="";
	char*			pdata;
	HANDLE			event_list[2];
	OVERLAPPED		ov,
					ov_read;
	
	
	memset(&ov, 0, sizeof(ov));
	ov.hEvent = CreateEvent(NULL, 1, 0, NULL);
	if(ov.hEvent==NULL)
		EXCTHROW( -1, "Create Event Fault (Hand Barcode Reader)");
	
#if defined(_DEBUG)
    sprintf( log_message, "[%d] START"__FUNC__"\n", (int)GetCurrentThreadId());
	OutputDebugString(log_message);
#endif

    memset(&barcode_item, 0, sizeof(barcode_item));
	event_list[0] = pHBR->QuitEvent;
	event_list[1] = pHBR->COMHandle;
	do 
	{
		
#if 1
		WaitCommEvent(pHBR->COMHandle, &event_mask, &ov);
		event_list[1] = ov.hEvent;	
		switch( WaitForMultipleObjects( 2, event_list, 0, HBR_SLEEP_TIME))
		{
		case WAIT_TIMEOUT:	/* Timeout */
			break;
		case WAIT_OBJECT_0:
			goto Error;		/* Quit Event -> terminate thread */
			break;
		case WAIT_OBJECT_0 + 1: /* Something on serial interface */
			ResetEvent(ov.hEvent);
			memset(&ov_read, 0, sizeof(ov_read));
			ov_read.hEvent = CreateEvent(0, TRUE, 0, 0);

			pdata = barcode_item.data;
			count = 0;
			
			if ( pHBR->is_usb == 0 )
			{
				int	wait_loop = 3;
			do
			{
				Sleep(50);
				do
				{
					ResetEvent(ov_read.hEvent);
					ReadFile(	pHBR->COMHandle,
								pdata,
								1,
								&rd_count,
								&ov_read);
					if ( rd_count ) {
						pdata++; count++;
					}
				} while (rd_count > 0 && count < MAX_BARCODE_LENGTH);
				Sleep(10);
			} while (--wait_loop > 0 && count < MAX_BARCODE_LENGTH);
			/* Close read event and cancel read function */
			CloseHandle(ov_read.hEvent);
			CancelIo(pHBR->COMHandle);

			if ( count )
			{
				barcode_item.TimeStamp = time(NULL);
#if 0
				for( ; count && ((10 == *(pdata-1)) || (13 == *(pdata-1))) ; count--)
					*--pdata = '\0';
#endif
				EXCCHECK( handbr_AddItem( pHBR, &barcode_item));
				//EXCCHECKWIN( CancelIo(pHBR->COMHandle));
				//ResetEvent(pHBR->COMHandle);
				memset(&barcode_item, 0, sizeof(barcode_item));
				pdata = barcode_item.data;
			}
			
			}
			else
			{
				int	wait_loop = 3;
				do {
					EXCCHECKWIN( ReadFile( pHBR->COMHandle,
										   pdata,
										   1,
										   &rd_count, 
										   NULL));

		
					if ( rd_count )
					{
						pdata++; count++;	
					}
					else if ( count )
					{
						barcode_item.TimeStamp = time(NULL);

#if 0
						for( ; count && ((10 == *(pdata-1)) || (13 == *(pdata-1))) ; count--)
							*--pdata = '\0';
#endif

						EXCCHECK( handbr_AddItem( pHBR, &barcode_item));
						EXCCHECKWIN( CancelIo(pHBR->COMHandle));
						ResetEvent(pHBR->COMHandle);
						memset(&barcode_item, 0, sizeof(barcode_item));
						pdata = barcode_item.data;
					}
				//	if ( rd_count == 0 )
				//	{
				//		--wait_loop; Sleep(10);
				//	}
				} while ( rd_count > 0 ); // || wait_loop > 0);
			}

			break;
		}
#else
		if (WAIT_OBJECT_0 == WaitForSingleObject( pHBR->QuitEvent, HBR_SLEEP_TIME) )
		{
			break;
		}
#endif
	} while (TRUE);

Error:
	if( ov.hEvent )
	{
		CloseHandle( ov.hEvent);	
	}
	EXCDISPLAY( pexception);

#if defined(_DEBUG)
    sprintf( log_message, "[%d] EXIT"__FUNC__"\n", (int)GetCurrentThreadId());
	OutputDebugString(log_message);
#endif

	ExitThread(0);
	return 0;
} /* Thread_HBarcodeReader */



