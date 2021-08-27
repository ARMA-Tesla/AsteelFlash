#if !defined(__STEPTIMER_H__)
#define __STEPTIMER_H__

#include <eltypes.h>
#include <exception/exception.h>  
#include <windows.h>
#include <winbase.h>
#include <timer/eltimer.h>

#if defined(__cplusplus) || defined(__cplusplus__)
	extern "C" {
#endif

/* step timer states: */
typedef enum _EStepTimerState
{ 
	ST_IDLE, 
	ST_RUN, 
	ST_EXPIRED
}EStepTimerState;

typedef struct _SStepTimer
{
	SElExceptionPtr (*Run)(struct _SStepTimer* me, double length);
	SElExceptionPtr (*Stop)(struct _SStepTimer* m);
	EStepTimerState (*GetState)(struct _SStepTimer* me);

	STimerPtr		timer;
	double			length;
	EStepTimerState	state;
} SStepTimer, *SStepTimerPtr;

SElExceptionPtr	steptimer_new(SStepTimerPtr* pTimerPtr);
SElExceptionPtr	steptimer_delete(SStepTimerPtr* pTimerPtr);

#if defined(__cplusplus) || defined(__cplusplus__)
	}
#endif

#endif /* __STEPTIMER_H__*/ 
