#include "steptimer.h"

static SElExceptionPtr Run(struct _SStepTimer* me, double length);
static SElExceptionPtr Stop(struct _SStepTimer* me);
static EStepTimerState GetState(struct _SStepTimer* me);

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "steptimer_new"
SElExceptionPtr steptimer_new(SStepTimerPtr* pTimer)
{
	SElExceptionPtr     pexception = NULL; 
	SStepTimerPtr		me = NULL;

	me = calloc( 1, sizeof(SStepTimer));
	EXCCHECKALLOC( me);

	me->Run = Run;
	me->Stop = Stop;
	me->GetState = GetState;

	EXCCHECK( timer_new(&me->timer));
	me->state = ST_IDLE;

	if(pTimer) *pTimer = me;

Error:
	EXCRETHROW( pexception);	 
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "steptimer_delete"
SElExceptionPtr	steptimer_delete(SStepTimerPtr* pTimer)
{
	SElExceptionPtr	pexception = NULL; 

	if(pTimer && *pTimer)
	{
		SStepTimerPtr	me = *pTimer;

		EXCCHECK( timer_delete(&me->timer));

		free( *pTimer);
		*pTimer = NULL;
	}

Error:
	EXCRETHROW( pexception);	 
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "Run"
static SElExceptionPtr Run(struct _SStepTimer* me, double length)
{
	me->timer->Start(me->timer);

	me->length = length;
	me->state = ST_RUN;

	return NULL;		 
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "Stop"
static SElExceptionPtr Stop(struct _SStepTimer* me)
{
	me->length = 0;
	me->state = ST_IDLE;

	return NULL;	
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "GetState"
static EStepTimerState GetState(struct _SStepTimer* me)
{
	if(me->state == ST_RUN)
	{
		if((me->timer->GetTimeDiff(me->timer)*1000.0) >= me->length)
		{
			me->state = ST_EXPIRED;
		}
	}

	return me->state;
}