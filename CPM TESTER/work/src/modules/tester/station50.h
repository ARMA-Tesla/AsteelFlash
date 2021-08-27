#if !defined(__STATION_50_H__)
#define __STATION_50_H__
#include <exception/Exception.h>



int32_t	gs_plugged_station_50;
int32_t	gs_unplugged_station_50;

SElExceptionPtr PlugPanelStation50
(
	struct _STester* me,
	int32_t			timeout
);

SElExceptionPtr UnplugPanelStation50
(
	struct _STester* me,
	int32_t			timeout
);


#endif /* __STATION_50_H__*/ 