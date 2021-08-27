#include <stdint.h>
#include <userint.h>
#include <ansi_c.h>
#include <windows.h>
#include "tester.h"
#include "station30.h"

/*Plug Panel station 30 */
#undef __FUNC__
#define __FUNC__	"PlugPanelStation30"
SElExceptionPtr PlugPanelStation30
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;
	int32_t verticalUnpluggedStation30;
	
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_PLUGGED_STATION_30, &hevent));

	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_V_STATION_30", &verticalUnpluggedStation30));
	if (microCam && verticalUnpluggedStation30 )
	{
	EXCCHECK( me->SetSensor(me, "CYLINDER_PLUG_STATION_30", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_UNPLUG_STATION_30", 0));
	}
	
	
	if (timeout)
	{
		if(gs_plugged_station_30) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_plugged_station_30) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

/*Unplug Panel station 30 */
#undef __FUNC__
#define __FUNC__	"UnplugPanelStation30"
SElExceptionPtr UnplugPanelStation30
(
	struct _STester* me,
	int32_t			timeout
	
)
{
	int32_t verticalUnpluggedStation30;
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
    
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_UNPLUGGED_STATION_30, &hevent));
    EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_V_STATION_30", &verticalUnpluggedStation30));
	if(verticalUnpluggedStation30)
	{
	EXCCHECK( me->SetSensor(me, "CYLINDER_UNPLUG_STATION_30", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_PLUG_STATION_30", 0));
	}
	if (timeout)
	{
		if(gs_unplugged_station_30) goto Error;//déja en état attendu
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_unplugged_station_30) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}
/*Vertical Plug station 30 */
#undef __FUNC__
#define __FUNC__	"VerticalPlugStation30"
SElExceptionPtr VerticalPlugStation30
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;
	int32_t pluggedStation30;
	
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_VERTICAL_PLUGGED_STATION_30, &hevent));

	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	//EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_STATION_30", &pluggedStation30));
	if (microCam /*&& pluggedStation30*/)
	{
	EXCCHECK( me->SetSensor(me, "CYLINDER_PLUG_V_STATION_30", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_UNPLUG_V_STATION_30", 0));
	}
	
	
	if (timeout)
	{
		if(gs_vertical_plugged_station_30) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_vertical_plugged_station_30) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

/*Vertical Unplug station 30 */
#undef __FUNC__
#define __FUNC__	"VerticalUnplugStation30"
SElExceptionPtr VerticalUnplugStation30
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;

	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_VERTICAL_UNPLUGGED_STATION_30, &hevent));

	EXCCHECK( me->SetSensor(me, "CYLINDER_UNPLUG_V_STATION_30", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_PLUG_V_STATION_30", 0));
	
	if (timeout)
	{
		if(gs_vertical_unplugged_station_30) goto Error;//déja en état attendu
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_vertical_unplugged_station_30) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}