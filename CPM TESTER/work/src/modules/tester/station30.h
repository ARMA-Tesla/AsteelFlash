#if !defined(__STATION_30_H__)
#define __STATION_30_H__

#include <exception/Exception.h>

int32_t	gs_plugged_station_30;
int32_t	gs_unplugged_station_30;

int32_t	gs_vertical_plugged_station_30;
int32_t	gs_vertical_unplugged_station_30;

SElExceptionPtr PlugPanelStation30
(
	struct _STester* me,
	int32_t			timeout
);

SElExceptionPtr UnplugPanelStation30
(
	struct _STester* me,
	int32_t			timeout
);

SElExceptionPtr VerticalPlugStation30
(
	struct _STester* me,
	int32_t			timeout
);

SElExceptionPtr VerticalUnplugStation30
(
	struct _STester* me,
	int32_t			timeout
);

#endif /* __STATION_30_H__*/ 