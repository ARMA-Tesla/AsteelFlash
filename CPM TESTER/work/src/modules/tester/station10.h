#if !defined(__STATION_10_H__)
#define __STATION_10_H__

#include <exception/Exception.h>

int32_t	gs_locked_station_10 ;
int32_t	gs_unlocked_station_10;

SElExceptionPtr LockPanelStation10
(
	struct _STester* me,
	int32_t			timeout
);
SElExceptionPtr UnlockPanelStation10
(
	struct _STester* me,
	int32_t			timeout
);

#endif /* __STATION_10_H__*/ 