#if !defined(__STATION_40_H__)
#define __STATION_40_H__

#include <exception/Exception.h>

int32_t	gs_plugged_station_40;
int32_t	gs_unplugged_station_40;

SElExceptionPtr PlugPanelStation40
(
	struct _STester* me,
	int32_t			timeout
);

SElExceptionPtr UnplugPanelStation40
(
	struct _STester* me,
	int32_t			timeout
);


#endif /* __STATION_40_H__*/ 