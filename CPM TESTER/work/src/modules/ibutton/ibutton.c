#include "ibutton.h"
#include <stdio.h>
#include <stdlib.h>
#include <elerror.h>

#define IBUTTONLOG( priority, message) 
/*
	if ( pIButton->_LogFnc ) { \
		pIButton->_LogFnc( priority, message); \
	} else
*/
/**
 * Specifies time between, when same ibutton number is ignored in seconds
 */
#define IBUTTON_MIN_TIME			2
/** Specifies Read command for iButton Reader */
#define IBUTTON_READ_COMMAND			"I"
/** Specifies Reset command for iButton Reader */
#define IBUTTON_RESET_COMMAND			"r"
/** Specifies how long read thread sleeps, before read value from iButton. */
#define IBUTTON_SLEEP_TIME				500

/*****************************************************************************
 * Static no-exported functions declaration **********************************
 *****************************************************************************/
static SElExceptionPtr ibutton_Initialize(
    SIButtonPtr*  pIButton
);
static SElExceptionPtr ibutton_Lock(
    struct _SIButton*	pIButton
);
static SElExceptionPtr ibutton_Unlock(
    struct _SIButton*	pIButton
);
static SElExceptionPtr ibutton_Run(
    struct _SIButton*	pIButton
);
static SElExceptionPtr ibutton_Terminate(
    struct _SIButton*	pIButton,
    int32_t					timeout
);
static SElExceptionPtr ibutton_GetLastItem(
	struct _SIButton* me,		   \
	SIButtonItemPtr iButton
);
SElExceptionPtr iButtonSetLogCallback(
	struct _SIButton* me,
	LogCallbackPtr LogFnc
);

static SElExceptionPtr ibutton_ResetItem(
	struct _SIButton* me
);
static SElExceptionPtr ibutton_RegisterCallbackFnc(
	struct _SIButton* me,
	iButtonCallbackPtr callaback
);
static SElExceptionPtr ibutton_DisableCallbackFnc(
	struct _SIButton* me, 
	bool_t disable
);
static SElExceptionPtr ibutton_RegisterCallbackFnc2(
	struct _SIButton* me,
		iButtonCallbackPtr callback
);
static SElExceptionPtr ibutton_IsRunning(
    struct _SIButton*	pIButton,
	int32_t*				pIsRunning
);
static SElExceptionPtr ibutton_AddItem(
    struct _SIButton*	pIButton,
	const SIButtonItemPtr	pItem
);
static SElExceptionPtr	ibutton_OpenSerial(
	struct _SIButton*	pIButton,
	SSerialIntfBasePtr	COMSettings,
	HANDLE*				handle
);
static void ibutton_LogWinError(
	const char* name,
	uint32_t	line
);

DWORD WINAPI Thread_IButton( LPVOID lpParam );


/*****************************************************************************
 * Exported functions definition *********************************************
 *****************************************************************************/
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ibutton_new"
/*!
 * \brief
 * This function creates a new instance of iButton.
 * 
 * \param pError 
 * Returns a value indicating whether an error occurred.
 * A negative error code indicates function failure.	
 * 
 * \return 
 * Pointer to new instance of the iButton object. If the function fails, NULL
 * pointer is returned
 *
 * This function creates a new instance of the iButton object.
 */
SElExceptionPtr ibutton_new(
	SIButtonPtr*  pIbutton
)
{
	SElExceptionPtr	pexception = NULL;
	SIButtonPtr		pibutton = NULL;

	EXCCHECK( ibutton_Initialize(&pibutton));

Error:
	if ( pexception )
	{
		ibutton_delete( &pibutton);
	}
	*pIbutton = pibutton;
	
	EXCRETHROW( pexception);
} /* ibutton_new */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ibutton_delete"
/*!
 * \brief
 * This function destroys specified instance.
 * 
 * \param pKOSU 
 * Pointer to instance of the iButton object cretaed by ibutton_new() function.
 * 
 * \return 
 * A value indicating whether an error occurred.
 * A negative error code indicates function failure.	
 *
 * This function destroys specified instance and release all resources.
 */
SElExceptionPtr ibutton_delete(SIButtonPtr* pIButton)
{
	SElExceptionPtr	pexception = NULL;

	if (*pIButton)
	{
		(*pIButton)->Terminate((*pIButton), 0);
		if ((*pIButton)->ThreadHandle) {
			CloseHandle((*pIButton)->ThreadHandle);
			(*pIButton)->ThreadHandle = 0;
		}
		if ((*pIButton)->QuitEvent) {
			CloseHandle((*pIButton)->QuitEvent);
			(*pIButton)->QuitEvent = 0;
		}
		if ((*pIButton)->COMHandle) {
			CloseHandle((*pIButton)->COMHandle);
			(*pIButton)->COMHandle = 0;
		}
		if((*pIButton)->_CallBackFnc) {
			free((*pIButton)->_CallBackFnc);
			(*pIButton)->_CallBackFnc = NULL;
		}
		DeleteCriticalSection(&((*pIButton)->cs));
		free(*pIButton);
		*pIButton = NULL;
	} /* if (*pIButton) { ...*/

Error:
	EXCRETHROW( pexception);
} /* ibutton_delete */

/*============================================================================*/
#undef __FUNC__
#define __FUNC__ "ibutton_IsRunning"
static SElExceptionPtr ibutton_IsRunning(
    struct _SIButton*	pIButton,
	int32_t*				pIsRunning
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t		error = 0;
	int32_t		state = 0;
	DWORD		exitCode = 0;
	
	if (pIButton->ThreadHandle) {
		EXCCHECKWIN( GetExitCodeThread( pIButton->ThreadHandle, &exitCode));
		if ( exitCode == STILL_ACTIVE ) {
			state = 1;	
		}
		else
		{
			EXCCHECKWIN( CloseHandle(pIButton->QuitEvent));
			EXCCHECKWIN( CloseHandle(pIButton->ThreadHandle));

			pIButton->QuitEvent    = 0;
			pIButton->ThreadHandle = 0;
			pIButton->ThreadID     = 0;
		}
	}

Error:
	*pIsRunning = state;
	EXCRETHROW( pexception);
} /* ibutton_IsRunning */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ibutton_Terminate"
static SElExceptionPtr ibutton_Terminate(
    struct _SIButton*	pIButton,
    int32_t					timeout
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t 		is_running;
	BOOL			error;

	EXCCHECK( ibutton_IsRunning(pIButton, &is_running));

	if ( is_running )
	{
		DWORD	state;
		DWORD exitCode;
		EXCCHECKWIN( SetEvent(pIButton->QuitEvent));
		/*
		 * TODO: wait timeout and check is _running
		 */
		state = WaitForSingleObject( pIButton->ThreadHandle, 1000);
		if ( state == WAIT_TIMEOUT )
		{
			IBUTTONLOG( ELOG_ERR, "Thread does not terminate.");
		}
		GetExitCodeThread( pIButton->ThreadHandle, &exitCode);
		if ( exitCode == STILL_ACTIVE )
		{
			IBUTTONLOG( ELOG_WARNING, "Thread will be terminateds.");			
			TerminateThread( pIButton->ThreadHandle, 1);
		}
		EXCCHECKWIN( CloseHandle( pIButton->ThreadHandle));
		pIButton->ThreadHandle = 0;
	}

Error:
	EXCRETHROW( pexception);
} /* ibutton_Terminate */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ibutton_GetLastItem"
static SElExceptionPtr ibutton_GetLastItem(
	struct _SIButton* pIButton,
	SIButtonItemPtr iButton
)
{
	SElExceptionPtr	pexception = NULL;

	EXCCHECK( ibutton_Lock( pIButton));
	memcpy( iButton, &pIButton->item, sizeof(SIButtonItem));

Error:
	ibutton_Unlock( pIButton);
	EXCRETHROW( pexception);
} /* ibutton_GetLastItem */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "iButtonSetLogCallback"
SElExceptionPtr iButtonSetLogCallback(
	struct _SIButton* pIButton,
	LogCallbackPtr LogFnc
)
{
	pIButton->_LogFnc = LogFnc;
	return NULL;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ibutton_ResetItem"
static SElExceptionPtr ibutton_ResetItem(
	struct _SIButton* pIButton
)
{
	SElExceptionPtr	pexception = NULL;

	EXCCHECK( ibutton_Lock( pIButton));
	memset( &pIButton->item, 0, sizeof(SIButtonItem));

Error:
	ibutton_Unlock( pIButton);
	EXCRETHROW( pexception);
} /* ibutton_ResetItem */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ibutton_RegisterCallbackFnc"
/*!
 * \brief
 * This function register a callback which is invoke, when new data are
 * recived.
 * 
 * \param pIButton
 * Specifies iButton cretaed by ibutton_new function.
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
static SElExceptionPtr ibutton_RegisterCallbackFnc(
	struct _SIButton* me,
		iButtonCallbackPtr callback
)
{
	SElExceptionPtr	pexception = NULL;

	EXCCHECK( ibutton_Lock( me));

	me->_CallBackFnc = (iButtonCallbackPtr*)realloc(me->_CallBackFnc, sizeof(iButtonCallbackPtr)*(me->_CallBackFncSize+1));
	me->_CallBackFnc[me->_CallBackFncSize] = callback;

	me->_CallBackFncSize++;

Error:
	ibutton_Unlock( me);
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ibutton_DisableCallbackFnc"
static SElExceptionPtr ibutton_DisableCallbackFnc(
	struct _SIButton* me, 
	bool_t disable
)
{
	SElExceptionPtr	pexception = NULL;

	EXCCHECK( ibutton_Lock( me));

	me->_CallBackFncDisabled = disable;

Error:
	ibutton_Unlock( me);
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ibutton_RegisterCallbackFnc2"
/*!
 * \brief
 * This function register a callback which is invoke, when new data are
 * recived.
 * 
 * \param pIButton
 * Specifies iButton cretaed by ibutton_new function.
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
static SElExceptionPtr ibutton_RegisterCallbackFnc2(
	struct _SIButton* me,
		iButtonCallbackPtr callback
)
{
	SElExceptionPtr	pexception = NULL;

	EXCCHECK( ibutton_Lock( me));

	me->_CallBackFnc2 = callback;

Error:
	ibutton_Unlock( me);
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
/**
 * This function Run thread which reads information from iButton
 *
 *
 * \return	A value indicating whether an error occurred.
 *          A negative error code indicates function failure.	
 */
#undef __FUNC__
#define __FUNC__ "ibutton_Run"
static SElExceptionPtr ibutton_Run(
    struct _SIButton*	pIButton
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			is_running = 0;
	DWORD			dw_thread_Id;
	char			log_message[256] = "";

	IBUTTONLOG( ELOG_NOTICE, "Starting ....");

	EXCCHECK( ibutton_Lock(pIButton));
	EXCCHECK( ibutton_IsRunning(pIButton, &is_running));

	if ( is_running ) {
		IBUTTONLOG( ELOG_WARNING, "Already running ....");
		EXCTHROW( EL_ERROR_OS_SPECIFIC, __FUNC__" Already running ....");
	}

	EXCCHECK( ibutton_OpenSerial( pIButton, &pIButton->COM, &pIButton->COMHandle)); 

	pIButton->QuitEvent = CreateEvent( NULL, TRUE, FALSE, NULL);
	if (pIButton->QuitEvent == NULL)
	{
		IBUTTONLOG( ELOG_ERR, "Create event fails");
		EXCTHROW( EL_ERROR_OS_SPECIFIC, " Create event fails");
	}
	
	pIButton->ThreadHandle = CreateThread( 
							NULL,                       /* no security attributes  */
							0,                          /* use default stack size  */
							Thread_IButton,				/* thread function  */
							pIButton,					/* argument to thread function */
							0,                          /* use default creation flags  */
							&dw_thread_Id);             /* returns the thread identifier */
	
	sprintf(log_message, "iButton Thread Started (0x%x)", (unsigned int)(pIButton->ThreadHandle));
	if (pIButton->ThreadHandle == NULL)
	{
		IBUTTONLOG( ELOG_ERR, log_message);
		EXCTHROW( EL_ERROR_OS_SPECIFIC, __FUNC__" iButton Thread Started ");
	}
	IBUTTONLOG( ELOG_NOTICE, log_message);

	pIButton->ThreadID =  dw_thread_Id;

Error:
	ibutton_Unlock( pIButton);
	EXCRETHROW( pexception);
} /* ibutton_Run */

/*****************************************************************************
 * Static no-exported functions **********************************************
 *****************************************************************************/
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ibutton_Initialize"
/*!
 * \brief
 * This function creates a new instance of the specified object.
 * 
 * \param pIButton
 * Returns pointer to new instance of the iButton object.
 *
 * \return	
 * A value indicating whether an error occurred.
 * A negative error code indicates function failure.	
 *
 * This function creates and initialize a new instance of the specified object.
 */
static SElExceptionPtr ibutton_Initialize(
    SIButtonPtr*  pIButton
)
{
	SElExceptionPtr		pexception = NULL;
    SIButtonPtr			pibutton = NULL;

    /*
     * Initialize all data
     */     
    pibutton = malloc(sizeof(SIButton));
    EXCCHECKALLOC( pibutton);
    memset(pibutton, 0, sizeof(SIButton));
	InitializeCriticalSection(&(pibutton->cs));

	pibutton->Run = ibutton_Run;
	pibutton->Terminate      = ibutton_Terminate;
	pibutton->RegisterCallbackFnc  = ibutton_RegisterCallbackFnc;
	pibutton->DisableCallbackFnc = ibutton_DisableCallbackFnc;
	pibutton->RegisterCallbackFnc2 = ibutton_RegisterCallbackFnc2;
	pibutton->Reset          = ibutton_ResetItem;
	pibutton->GetLastItem    = ibutton_GetLastItem;
	pibutton->SetLogCallback = iButtonSetLogCallback;
	
	
	strncpy( pibutton->_readCmd, IBUTTON_READ_COMMAND, IBUTTON_CMD_SIZE);
	strncpy( pibutton->_resetCmd, IBUTTON_RESET_COMMAND, IBUTTON_CMD_SIZE);
		
Error:
    if ( pexception ) {
		if ( pibutton != NULL ){
			DeleteCriticalSection(&(pibutton->cs));
			free(pibutton);
			pibutton = NULL;
		}
    }

    if (pIButton) *pIButton = pibutton;
    EXCRETHROW( pexception);
} /* ibutton_Initialize */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ibutton_Lock"
/**
 * This function is used to obtain a lock on the specified resource.
 *
 * \param	pIButton	Pointer to resource which should be locked
 *
 * \return	A value indicating whether an error occurred.
 *          A negative error code indicates function failure.		
 */
static SElExceptionPtr ibutton_Lock(
    struct _SIButton*	pIButton
)
{
	if ( pIButton != NULL )
	{
		EnterCriticalSection(&(pIButton->cs));
	}

Error:	
	return NULL;
} /* ibutton_Lock */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ibutton_Unlock"
/**
 * Relinquishes a lock to the resource associated with the given session.
 *
 * \param	pIButton	Pointer to resource which should be unlocked
 *
 * \return	A value indicating whether an error occurred.
 *          A negative error code indicates function failure.		
 */
static SElExceptionPtr ibutton_Unlock(
    struct _SIButton*	pIButton
)
{
	if ( pIButton != NULL )
	{
		LeaveCriticalSection(&(pIButton->cs));
	}

Error:	
	return NULL;
} /* ibutton_Unlock */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ibutton_AddItem"
/**
 * This function store new iBurron entry in the class and invoke callback for
 * specified session. If the iButton ID is same as value stored in the class
 * value is accepted as new, when time between old and this new item is 
 * greater as IBUTTON_MIN_TIME.
 *
 * \param   pIButton	Specifies iButton cretaed by ibutton_new function.
 * \param	pItem		Pass a pointer to new iButton item, which has been
 *                      received from iButton reader.
 * \return	A value indicating whether an error occurred.
 *          A negative error code indicates function failure.		
 */
static SElExceptionPtr ibutton_AddItem(
    struct _SIButton*	pIButton,
	const SIButtonItemPtr	pItem
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			update = 0;

	EXCCHECK( ibutton_Lock( pIButton));
	if ( memcmp(pIButton->item.ID, pItem->ID, sizeof(pItem->ID)) != 0 ) 
	{
		/*
		 * New iButton has been recieved
		 */
		update = 1;
	}
	else
	if (( pItem->TimeStamp - pIButton->item.TimeStamp ) > IBUTTON_MIN_TIME )
	{
		/*
		 * It's same ibutton but time is greater as IBUTTON_MIN_TIME
		 */
		update = 1;	
	}

	if ( update )
	{
		char sibbuton[256] = "";
		char* pend = sibbuton;
		int i = 0;
		char message[256] = "";

		uint32_t v1 = *((uint32_t*)pItem->ID);
		uint32_t v2 = *((uint32_t*)&(pItem->ID[4]));
		sprintf( message, "New value detected [%x%x]", v1, v2);
		IBUTTONLOG( ELOG_INFO, message);
		memcpy( &pIButton->item, pItem, sizeof(SIButtonItem));	

		for(i=0; i<8; i++)
		{
			pend += sprintf( pend, "%02X", (unsigned int)pItem->ID[i]); 	 		
		}

		/* high priority callback */
		if(pIButton->_CallBackFnc2)
			EXCCHECK( (pIButton->_CallBackFnc2)(pIButton, sibbuton));
		/* low priority callbacks */
		if(!pIButton->_CallBackFncDisabled)
		{
			for(i=0; i<pIButton->_CallBackFncSize; i++)
			{
				EXCCHECK( (pIButton->_CallBackFnc[i])(pIButton, sibbuton));
			}
		}
	}

Error:
	ibutton_Unlock( pIButton);
    EXCRETHROW( pexception);
} /* ibutton_AddItem */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ibutton_OpenSerial"
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
static SElExceptionPtr	ibutton_OpenSerial(
	struct _SIButton*	pIButton,
	SSerialIntfBasePtr	COMSettings,
	HANDLE*				handle
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error;
	HANDLE			fh = NULL;
	char			file[256];

	if (COMSettings->port > 9)
		sprintf(file, "\\\\.\\COM%d", (int)COMSettings->port);
	else
		sprintf(file, "COM%d", (int)COMSettings->port);
		
	/* TODO: Open COM port with default values or from option string values */
	fh = CreateFile(file,
					GENERIC_READ|GENERIC_WRITE,//access ( read and write)
					0,    					//(share) 0:cannot share the COM port                        
					0,    					//security  (None)                
					OPEN_EXISTING,			// creation : open_existing
					0,	
					0						// no templates file for COM port...
            	);

	EXCCHECKWIN( !(fh == INVALID_HANDLE_VALUE) )

	if(handle) *handle = fh;

Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
/*
 * LogWinError
 */
static void ibutton_LogWinError(
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
 * Thred function which reads values from iButton reader
 *
 * \param lpParam	pointer to session associated with this thread
 *
 * \return	A value indicating whether an error occurred.
 *          A negative error code indicates function failure.		
 */
#undef __FUNC__
#define __FUNC__ "Thread_IButton"
DWORD WINAPI Thread_IButton( LPVOID lpParam ) 
{
	SElExceptionPtr	pexception = NULL;
	SIButtonPtr		pIButton = (SIButtonPtr) lpParam;
	int32_t     	error = 0, failed = 0;
	DWORD			wrt_count;
	DWORD			rd_count;
	SIButtonItem	iButton_item;
	char			log_message[256]="";
	
#if defined(_DEBUG)
    sprintf( log_message, "[%d] START"__FUNC__"\n", (int)GetCurrentThreadId());
	OutputDebugString(log_message);
#endif

    memset(&iButton_item, 0, sizeof(iButton_item));

	/*
	 * Reset old value which is in memory
	 */
	EXCCHECKWIN( WriteFile( pIButton->COMHandle,
							pIButton->_resetCmd,
							(DWORD)strlen(pIButton->_resetCmd),
							&wrt_count, 
							NULL));

	do 
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject( pIButton->QuitEvent, IBUTTON_SLEEP_TIME) )
		{
			break;
		}

		EXCCHECKWIN( WriteFile( pIButton->COMHandle,
							    pIButton->_readCmd,
							    (DWORD)strlen(pIButton->_readCmd),
							   &wrt_count, 
							   NULL));

		EXCCHECKWIN( ReadFile( pIButton->COMHandle,
							   iButton_item.ID,
							   (DWORD)sizeof(iButton_item.ID),
							   &rd_count, 
							   NULL));

		if (!( iButton_item.ID[0] == 0 &&
			   iButton_item.ID[1] == 0 &&
			   iButton_item.ID[2] == 0 &&
			   iButton_item.ID[3] == 0 &&
			   iButton_item.ID[4] == 0 &&
			   iButton_item.ID[5] == 0 &&
			   iButton_item.ID[7] == 0 ))
		{
			iButton_item.TimeStamp = time(NULL);
			EXCCHECK( ibutton_AddItem( pIButton, &iButton_item));
			EXCCHECKWIN( WriteFile( pIButton->COMHandle,
								    pIButton->_resetCmd,
								   (DWORD)strlen(pIButton->_resetCmd),
								   &wrt_count, NULL));
		}
	} while (TRUE);

Error:
	EXCDISPLAY( pexception);

#if defined(_DEBUG)
    sprintf( log_message, "[%d] EXIT"__FUNC__"\n", (int)GetCurrentThreadId());
	OutputDebugString(log_message);
#endif

	ExitThread(0);
	return 0;
} /* Thread_IButton */



