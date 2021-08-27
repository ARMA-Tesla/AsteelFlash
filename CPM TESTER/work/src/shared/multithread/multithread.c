#include "multithread.h"

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "ThreadStart"
SElExceptionPtr ThreadStart( 
	ThreadProc_t pThreadFnc, 
	void* pData,
	HANDLE* hThread,
	HANDLE* hStopEvent
)
{
	SElExceptionPtr	pexception = NULL;  
	HANDLE			_hStopEvent = NULL;
	HANDLE			_hThread = NULL;

	if(pThreadFnc)
	{
		if(hStopEvent) 
		{
			_hStopEvent = CreateEvent( NULL, 1, 0, NULL);
			if (_hStopEvent == NULL)
				EXCTHROW(-1, "Create event failed");
			else
				*hStopEvent = _hStopEvent;
		}

		if(hThread)
		{
			_hThread = CreateThread( NULL, 0, pThreadFnc, pData, 0, NULL);
			if (_hThread == NULL)
				EXCTHROW(-1, "Create thread failed");
			else
				 *hThread = _hThread;
		}		
	}

Error:	
	EXCRETHROW( pexception);    
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "ThreadStatus"
SElExceptionPtr ThreadStatus(
	HANDLE hThread,
	uint32_t timeout,
	int32_t* pStatus
)
{
	SElExceptionPtr	pexception = NULL;  
	DWORD			exitCode = 0, result = 0;	
	BOOL			status;

	if( hThread)
	{
		result = WaitForSingleObject( hThread, timeout );
		switch(result)
		{
			case WAIT_TIMEOUT:
				EXCTHROW( -1, "Thread Handle Status: WAIT_TIMEOUT");
				break;
			case WAIT_OBJECT_0:
				status = GetExitCodeThread( hThread, &exitCode);
				switch(exitCode)
				{
					case 0:
						if(pStatus) *pStatus = THREAD_STATUS_NOT_ACTIVE;
						break;
					case THREAD_EXIT_CODE:
						if(pStatus) *pStatus = THREAD_STATUS_EXIT_CODE;
						break;
					case STILL_ACTIVE:
						if(pStatus) *pStatus = THREAD_STATUS_STILL_ACTIVE;
						break;
					default:
						if(pStatus) *pStatus = THREAD_STATUS_ERROR_CODE;
						break;
				}
				break;
		}
	}
		
Error:	
	EXCRETHROW( pexception);
}


/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "ThreadStop"
SElExceptionPtr ThreadStop(
	HANDLE* hThread,
	HANDLE* hStopEvent
)
{
	SElExceptionPtr	pexception = NULL;  	
	int32_t			thread_status = 0;

	if(hThread && *hThread)
	{
		if (hStopEvent && *hStopEvent)
		{
			SetEvent( *hStopEvent);
		}
		else
		{
			TerminateThread( *hThread, 0);
		}
		
		EXCCHECK( ThreadStatus( *hThread, 2000, &thread_status));
		switch(thread_status)
		{
			case THREAD_STATUS_EXIT_CODE:
			case THREAD_STATUS_NOT_ACTIVE:
			case THREAD_STATUS_ERROR_CODE:
				/* success */
				break;
			case THREAD_STATUS_STILL_ACTIVE:
				EXCTHROW( -1, "Thread Handle Status: STILL_ACTIVE");
				break;
		}

		CloseHandle( *hStopEvent);
		*hStopEvent = NULL;

		CloseHandle( *hThread);
		*hThread = NULL;
	}

Error:	
	EXCRETHROW( pexception);
}