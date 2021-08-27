
//---------------------------------------------------------------------------------------------------------------------------
/// @file		el_mutex.c
/// @version	1.0
/// @author		Zdenek Rykala
//---------------------------------------------------------------------------------------------------------------------------

#include	"el_mutex.h"

#include <stdio.h>
#include <elerror.h>


SElMutexPtr elmutex_new( int32_t* pError )
{
    int32_t		error = 0;
	SElMutexPtr pmutex = (SElMutexPtr)calloc(1, sizeof(SElMutex));
#if	defined(UNIX)	
	pthread_mutexattr_t      access_attr;
	
	CHECKALLOC( pmutex);
	pthread_mutexattr_init( &access_attr );
	pthread_mutexattr_settype( &access_attr, PTHREAD_MUTEX_RECURSIVE );
	pthread_mutex_init( &pmutex->m_access, &access_attr );

#elif	defined(WINDOWS)	

	CHECKALLOC( pmutex);
	
	pmutex->m_sa.nLength = sizeof( m_sa );
	pmutex->m_sa.lpSecurityDescriptor = NULL;
	pmutex->m_sa.bInheritHandle = TRUE;

	/* CREATE A RANDOM MUTEX UUID */
	char szUUID[ 41 ] = { 0 };
	SYSTEMTIME st;
	GetSystemTime( &st );
	sprintf( szUUID, "UUID:%02d:%02d:%02d:%03d:%02d:%02d:%02d:%04d", 
		st.wDay, st.wDayOfWeek, st.wHour, st.wMilliseconds, st.wMinute,
		st.wMonth, st.wSecond, st.wYear );

	pmutex->m_access = CreateMutex( &pmutex->m_sa, FALSE, szUUID );

#endif
}

int32_t elmutex_delete( SElMutexPtr* pMutex )
{
	if ( pMutex && *pMutex)
	{
#if	defined(UNIX)	
	pthread_mutex_destroy( &((*pMutex)->m_access) );
#elif	defined(WINDOWS)	
	CloseHandle( (*pMutex)->m_access );
#endif
		free(*pMutex);
		*pMutex = NULL;
	}
	return 0;
}

static void elMutexLock( struct _SElMutex* pMutex )
{
#if	defined(UNIX)	
	pthread_mutex_lock( &pMutex->m_access );
#elif	defined(WINDOWS)	
	WaitForSingleObject( pMutex->m_access, INFINITE );
#endif
}

static int32_t elMutexTryLock(
  struct _SElMutex* pMutex 
)
{
	// 
	// Set default to signaled.
	//
	int32_t  error = 0;

#if	defined(UNIX)	

	/*
	 * Check if object signaled and lock if success.
	 * If object is non signaled return busy otherwise object is now locked.
	 */
	if( ( error = pthread_mutex_trylock( &pMutex->m_access ) ) == EBUSY )
		 return EBUSY;		///< Return busy.

#elif	defined(WINDOWS)	

	/*
	 * Wait 10 ms of signaled state and lock if success.
	 * If object is non signaled return busy otherwise object is now locked.
	 */
	if( ( error = WaitForSingleObject( pMutex->m_access, 10 ) ) == WAIT_TIMEOUT )
		return EBUSY;			///< Return busy.

#endif

	return 0;		///< If 0 object is successfully locked otherwise error, return result.
}

static void elMutexUnlock( 
  struct _SElMutex* pMutex
)
{
#if	defined(UNIX)	
	pthread_mutex_unlock( &pMutex->m_access );
#elif	defined(WINDOWS)	
	ReleaseMutex( pMutex->m_access );
#endif
}
