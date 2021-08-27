#include <stdint.h>
#include <userint.h>
#include <ansi_c.h>
#include <windows.h>
#include "tester.h"
#include "station40.h"


/*Plug Panel station 40 */
#undef __FUNC__
#define __FUNC__	"PlugPanelStation40"
SElExceptionPtr PlugPanelStation40
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;
	
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_PLUGGED_STATION_40, &hevent));

	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	if (microCam)//PLATEAU EN POS INDEXE
	{
	EXCCHECK( me->SetSensor(me, "CYLINDER_PLUG_STATION_40", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_UNPLUG_STATION_40", 0));
	}

	if (timeout)
	{
		if(gs_plugged_station_40) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_plugged_station_40) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

/*Unplug Panel station 40 */
#undef __FUNC__
#define __FUNC__	"UnplugPanelStation40"
SElExceptionPtr UnplugPanelStation40
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;

	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_UNPLUGGED_STATION_40, &hevent));

	EXCCHECK( me->SetSensor(me, "CYLINDER_UNPLUG_STATION_40", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_PLUG_STATION_40", 0));
	
	if (timeout)
	{
		if(gs_unplugged_station_40) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_unplugged_station_40) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}
