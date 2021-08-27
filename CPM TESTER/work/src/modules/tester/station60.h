#if !defined(__STATION_60_H__)
#define __STATION_60_H__

#include <exception/Exception.h>
static int32_t	gs_printer_in_front = 0;	
static int32_t	gs_printer_in_rear = 0;

int32_t	gs_open_gripper;
int32_t	gs_close_gripper;
int32_t gs_x_rear;
int32_t gs_x_front;
int32_t gs_z_up;
int32_t gs_z_down;

SElExceptionPtr OPEN_GRIPPER
(
	struct _STester* me,
	int32_t			timeout
);
SElExceptionPtr CLOSE_GRIPPER
(
	struct _STester* me,
	int32_t			timeout
);

SElExceptionPtr PickPlacePanelStation60
(
	struct _STester* me,
	int32_t			timeout
);

SElExceptionPtr HomePickPlacePanelStation60
(
	struct _STester* me,
	int32_t			timeout
);

SElExceptionPtr PickPrintPanelStation60
(
	struct _STester* me,
	int32_t			timeout
);
SElExceptionPtr ResetPickPrintPanelStation60
(
	struct _STester* me,
	int32_t			timeout
);

SElExceptionPtr PickReadBcStation60
(
	struct _STester* me,
	int32_t			timeout
);

SElExceptionPtr ReadyPrint
(
	struct _STester* me,
	int32_t			timeout
);
SElExceptionPtr CyclePrint
(
	struct _STester* me,
	int32_t			timeout
);
SElExceptionPtr PrinterFront
(
	struct _STester* me,
	int32_t			timeout
);
SElExceptionPtr PrinterRear
(
	struct _STester* me,
	int32_t			timeout
);

SElExceptionPtr ACTUATOR_Z_UP
(
	struct _STester* me,
	int32_t			timeout
);
SElExceptionPtr ACTUATOR_Z_DOWN
(
	struct _STester* me,
	int32_t			timeout
);
SElExceptionPtr ACTUATOR_X_REAR
(
	struct _STester* me,
	int32_t			timeout
);
SElExceptionPtr ACTUATOR_X_FRONT
(
	struct _STester* me,
	int32_t			timeout
);

#endif /* __STATION_60_H__*/ 