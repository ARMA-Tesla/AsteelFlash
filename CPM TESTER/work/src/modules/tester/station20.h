#if !defined(__STATION_20_H__)
#define __STATION_20_H__

#include <exception/Exception.h>

int32_t	gs_plugged_station_20;
int32_t	gs_unplugged_station_20;

SElExceptionPtr PlugPanelStation20
(
	struct _STester* me,
	int32_t			timeout
);

SElExceptionPtr UnPlugPanelStation20
(
	struct _STester* me,
	int32_t			timeout
);


#endif /* __STATION_20_H__*/ 