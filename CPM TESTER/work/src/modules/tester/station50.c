#include <stdint.h>
#include <userint.h>
#include <ansi_c.h>
#include <windows.h>
#include "tester.h"
#include "station50.h"

#define TIMEOUT_INIT() 	clock_t	_start_clock = clock();

/*Plug Panel station 50 */
#undef __FUNC__
#define __FUNC__	"PlugPanelStation50"
SElExceptionPtr PlugPanelStation50
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;
	
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_PLUGGED_STATION_50, &hevent));

	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	if (microCam)//PLATEAU EN POS INDEXE
	{
	EXCCHECK( me->SetSensor(me, "CYLINDER_PLUG_STATION_50", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_UNPLUG_STATION_50", 0));
	}

	if (timeout)
	{
		if(gs_plugged_station_50) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_plugged_station_50) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

/*Unplug Panel station 50 */
#undef __FUNC__
#define __FUNC__	"UnplugPanelStation50"
SElExceptionPtr UnplugPanelStation50
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;

	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_UNPLUGGED_STATION_50, &hevent));

	EXCCHECK( me->SetSensor(me, "CYLINDER_UNPLUG_STATION_50", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_PLUG_STATION_50", 0));
	
	if (timeout)
	{
		if(gs_unplugged_station_50) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_unplugged_station_50) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}
	

	
