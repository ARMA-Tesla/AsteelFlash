#include <stdint.h>
#include <userint.h>
#include <ansi_c.h>
#include <windows.h>
#include "tester.h"
#include "station10.h"


/* Lock Panel station 10 */
#undef __FUNC__
#define __FUNC__	"LockPanelStation10"
SElExceptionPtr LockPanelStation10
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;
	int32_t			sensor_locked = FALSE;


	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_LOCKED_STATION_10, &hevent));
	
	EXCCHECK( me->SetSensor(me, "CYLINDER_UNLOCK_STATION_10", 0));
	EXCCHECK( me->SetSensor(me, "CYLINDER_LOCK_STATION_10", 1));
	
	if (timeout)
	{
		if(gs_locked_station_10) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_locked_station_10) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}


/* Unlock Panel station 10 */
#undef __FUNC__
#define __FUNC__	"UnlockPanelStation10"
SElExceptionPtr UnlockPanelStation10
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;


	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_UNLOCKED_STATION_10, &hevent));

	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	
	if (microCam)
	{
		EXCCHECK( me->SetSensor(me, "CYLINDER_LOCK_STATION_10", 0));
		EXCCHECK( me->SetSensor(me, "CYLINDER_UNLOCK_STATION_10", 1));
	}
	if (timeout)
	{
		if(gs_unlocked_station_10) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_unlocked_station_10) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}
