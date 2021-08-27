
//---------------------------------------------------------------------------------------------------------------------------
/// @file		el_mutex.h
/// @version	1.0
/// @brief		Win 32 mutex class.
/// @author		Zdenek Rykala
/// @remarks	Used in All thread safe classes. 
//---------------------------------------------------------------------------------------------------------------------------

#ifndef		__EL_MUTEX_H__
#define		__EL_MUTEX_H__

#include <eltypes.h>

#if defined(WINDOWS)
	#include	<windows.h>
#else
  #if	defined(UNIX)
  	#include	<errno.h>
  	#include	<pthread.h>
  #endif
#endif

#ifndef		EBUSY
#define		EBUSY	16	
#endif



/*
 * \brief		Multiplatform mutex class.
 */
typedef struct _SElMutex
{
  /*
   * \brief		Locks a section - If section is already busy then block and wait
   *          until it's ready again.
   */
  void (*Lock)( void );

	/*
   * \brief		Locks a section - If section is busy, return without blocking.
   */
	int32_t TryLock( void );

	/*
   * \brief		Unlocks a section.
   */
  void Unlock( void );

	/****************************     ATTRIBUTES     ****************************/

#if	defined(WINDOWS)
	HANDLE				       m_access;		/*!< WIN32 mutex. */
	SECURITY_ATTRIBUTES  m_sa;

#elif defined(UNIX)
	pthread_mutex_t		m_access;				/*!< POSIX mutex. */
#endif	
} SElMutex, *SElMutexPtr;


#endif	// __EL_MUTEX_H__

