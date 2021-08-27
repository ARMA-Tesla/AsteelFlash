#include <stdint.h>
#include <userint.h>
#include <ansi_c.h>
#include <windows.h>
#include "tester.h"
#include "station20.h"


/*Plug Panel station 20 */
#undef __FUNC__
#define __FUNC__	"PlugPanelStation20"
SElExceptionPtr PlugPanelStation20
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;
	
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_PLUGGED_STATION_20, &hevent));

	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	if (microCam)
	{
		EXCCHECK( me->SetSensor(me, "CYLINDER_PLUG_STATION_20", 1));
		EXCCHECK( me->SetSensor(me, "CYLINDER_UNPLUG_STATION_20", 0));
	}
	
	if (timeout)
	{
		if(gs_plugged_station_20) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_plugged_station_20) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

/*UnPlug Panel station 20 */
#undef __FUNC__
#define __FUNC__	"UnPlugPanelStation20"
SElExceptionPtr UnPlugPanelStation20
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;

	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_UNPLUGGED_STATION_20, &hevent));

	EXCCHECK( me->SetSensor(me, "CYLINDER_UNPLUG_STATION_20", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_PLUG_STATION_20", 0));

	if (timeout)
	{
		if(gs_unplugged_station_20) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_unplugged_station_20) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}
