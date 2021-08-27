#if !defined(__MULTITHREAD_H__)
#define __MULTITHREAD_H__

#include <exception/exception.h>
#include <eltypes.h>
#include <windows.h>

/* status */
#define THREAD_STATUS_EXIT_CODE			1
#define THREAD_STATUS_NOT_ACTIVE		2
#define THREAD_STATUS_STILL_ACTIVE		3
#define THREAD_STATUS_ERROR_CODE		4

#define THREAD_EXIT_CODE	-122
#define THREAD_ERROR_CODE	-133

typedef DWORD (WINAPI *ThreadProc_t)(LPVOID param);

SElExceptionPtr ThreadStart(   
		ThreadProc_t pThreadFnc, 
		void* pData,
		HANDLE* hThread,
		HANDLE* hEventStop
);

SElExceptionPtr ThreadStop(
		HANDLE* hThread,
		HANDLE* hEventStop
);

SElExceptionPtr ThreadStatus(
		HANDLE hThread,
		uint32_t timeout,
		int32_t* pStatus
);


#endif //__MULTITHREAD_H__    
