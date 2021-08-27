#ifndef _AXIS_H_
#define _AXIS_H_

#include <windows.h>
#include <exception/Exception.h>

#define MAX_AXIS_NAME				128
#define MAX_BUFFER_SIZE				20

#define AXIS_ERROR_TIMEOUT				-50001

typedef struct _axis
{
	char*				axisName;
	char*				optionString;
	CRITICAL_SECTION	lock;
	HANDLE				fd;
	int 				port;
	HANDLE				termEvent;
	HANDLE				readEvent;
	HANDLE				specLib;
	HANDLE				hThread;
	int32_t				status;
	int32_t				readMode;
	char				readBuffer[MAX_BUFFER_SIZE];
	
	SElExceptionPtr (*Open)(
		struct _axis*	me,
		const char*			resource,
		const char*			optionString
	);
	SElExceptionPtr (*Close)(struct _axis* me);
	SElExceptionPtr (*Write)(struct _axis* me, const char* command);
	
	SElExceptionPtr (*PresentAlarm)(struct _axis* me, int32_t axis, int32_t* status);
	SElExceptionPtr (*ControllerStatus)(struct _axis* me, int32_t axis, int32_t* status);
	SElExceptionPtr (*EnableAxis)(struct _axis* me, int32_t axis);
	SElExceptionPtr (*AlarmReset)(struct _axis* me, int32_t axis);
	SElExceptionPtr (*HomeReturn)(struct _axis* me, int32_t axis);
	SElExceptionPtr (*StopAxis)(struct _axis* me, int32_t axis);
	SElExceptionPtr (*MoveAxisPosition)(struct _axis* me, int32_t axis, int32_t position);
	SElExceptionPtr (*CurrentPosition)(struct _axis* me, int32_t axis, double *position);
	SElExceptionPtr (*SetPosition)(struct _axis* me, int32_t axis, double position, double speed);
	SElExceptionPtr (*RequestPosition)(struct _axis* me, int32_t axis);
	SElExceptionPtr (*ReadPosition)(struct _axis* me, int32_t axis, double *position);

	/* Private */
	SElExceptionPtr (*_InitializeSpec)(struct _axis* me, int32_t axis);
	SElExceptionPtr (*_PresentAlarmSpec)(struct _axis* me, int32_t axis, int32_t* status);
	SElExceptionPtr (*_ControllerStatusSpec)(struct _axis* me,  int32_t axis, int32_t* status);
	SElExceptionPtr (*_EnableAxisSpec)(struct _axis* me, int32_t axis);
	SElExceptionPtr (*_AlarmResetSpec)(struct _axis* me, int32_t axis);
	SElExceptionPtr (*_HomeReturnSpec)(struct _axis* me, int32_t axis);
	SElExceptionPtr (*_StopAxisSpec)(struct _axis* me, int32_t axis);
	SElExceptionPtr (*_MoveAxisPositionSpec)(struct _axis* me, int32_t axis, int32_t position);
	SElExceptionPtr (*_CurrentPositionSpec)(struct _axis* me, int32_t axis, double* position);
	SElExceptionPtr (*_SetPositionSpec)(struct _axis* me, int32_t axis, double position, double speed);
	SElExceptionPtr (*_WriteSpec)(struct _axis* me, const char* command);
	SElExceptionPtr (*_callbackThreadSpec)(struct _axis* me);
	SElExceptionPtr (*_RequestPositionSpec)(struct _axis* me, int32_t axis);
	SElExceptionPtr (*_ReadPositionSpec)(struct _axis* me, int32_t axis, double* position);
	
} Saxis, *SaxisPtr;


typedef struct _axiss
{
	SaxisPtr*		axiss;
	int32_t			numaxiss;
	int32_t			currentIndex;
	
	SElExceptionPtr (*GetaxisByName)(
		struct _axiss*	me,
		const char*			axisName,
		SaxisPtr*		axis
	);
	SElExceptionPtr (*GetaxisByIndex)(
		struct _axiss*	me,
		int32_t				index,
		SaxisPtr*		axis
	);
	SElExceptionPtr (*GetCurrentaxis)(
		struct _axiss*	me,
		SaxisPtr*		axis
	);
	SElExceptionPtr (*SetCurrentaxisByName)(
		struct _axiss*	me,
		const char*			axisName
	);
	SElExceptionPtr (*GetFirstaxis)(
		struct _axiss*	me,
		SaxisPtr*		axis
	);
	SElExceptionPtr (*GetNextaxis)(
		struct _axiss*	me,
		SaxisPtr*		axis
	);
	
	SElExceptionPtr (*Initaxiss)(struct _axiss* me);
	SElExceptionPtr (*Closeaxiss)(struct _axiss* me);
	
} Saxiss, *SaxissPtr;

SElExceptionPtr axiss_new(SaxissPtr* axiss);
SElExceptionPtr axiss_delete(SaxissPtr* axiss);

SElExceptionPtr axis_new(
	SaxisPtr*	axis,
	const char*		name,
	const char*		optionString
);
SElExceptionPtr axis_delete(SaxisPtr* axis);

#endif /* _AXIS_H_ */
