/****************************************************************************
 *                   E X A C T   T I M E R
 *---------------------------------------------------------------------------
 *    Copyright  © 2005 ELCOM, a.s.  All Rights Reserved.        
 *---------------------------------------------------------------------------
 *                                                                          
 * Title:       eltimer.h                                                    
 * Purpose:     Exact Timer interface 
 ****************************************************************************/

#if !defined(__ELTIMER_H__)
#define __ELTIMER_H__

#include <eltypes.h>
#include <exception/exception.h>  
#include <windows.h>
#include <winbase.h>

#if defined(__cplusplus) || defined(__cplusplus__)
	extern "C" {
#endif

/*!
 * Specifies Exact timer interface
 */ 
typedef struct _STimer
{
	int32_t	(*Start)(struct _STimer* me);
	double	(*GetTimeDiff)(struct _STimer* me);

/*******************************************
 * PRIVATE
 *******************************************/
	LARGE_INTEGER _qwStart;
	LARGE_INTEGER _qwEnd;
	LARGE_INTEGER _qwFreq;
} STimer, *STimerPtr;

SElExceptionPtr	timer_new(STimerPtr* pTimerPtr);
SElExceptionPtr	timer_delete(STimerPtr* pTimerPtr);

#if defined(__cplusplus) || defined(__cplusplus__)
	}
#endif

#endif /* __ELTIMER_H__*/ 
