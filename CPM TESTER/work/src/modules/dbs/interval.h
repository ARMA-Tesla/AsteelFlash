#if !defined(__INTERVAL_H__)
#define __INTERVAL_H__

#include <exception/exception.h>
#include <time.h>

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

typedef struct _SIntervalResultEntry
{
	time_t   start;
	time_t   stop;
	int32_t   value;
} SIntervalResultEntry, *SIntervalResultEntryPtr;

typedef struct _SIntervalResult
{
	SIntervalResultEntryPtr intervals;
	uint32_t (*Count)(struct _SIntervalResult* me);
	
	SElExceptionPtr (*Add)(struct _SIntervalResult* me, time_t st, time_t sp);
	SElExceptionPtr (*AddEx)(struct _SIntervalResult* me, time_t st, time_t sp, int32_t value);
	SElExceptionPtr (*Resize)(struct _SIntervalResult* me);
	SElExceptionPtr (*Merge)(struct _SIntervalResult* me);

	uint32_t   _AllocatedCount;
	uint32_t   _UsedCount;
} SIntervalResult, *SIntervalResultPtr;

SElExceptionPtr intervalresult_new(SIntervalResultPtr* pInterval);
SElExceptionPtr intervalresult_delete(SIntervalResultPtr* pInterval);

SElExceptionPtr	MergeIntervals(SIntervalResultPtr pIntervals1, SIntervalResultPtr pIntervals2, SIntervalResultPtr* pResult, uint32_t* interval_time);
SElExceptionPtr	InverseIntervals(time_t	t1, time_t t2, SIntervalResultPtr pIntervals, SIntervalResultPtr* pResult, uint32_t* interval_time);

/* user interval */

typedef struct _SUserIntervalsEntry
{
	uint32_t		UserCount;
	uint32_t		TimeDelay;
} SUserIntervalsEntry, *SUserIntervalsEntryPtr;

typedef struct _SUserIntervals
{
	SUserIntervalsEntryPtr	intervals;

	uint32_t (*Count)(struct _SUserIntervals* me);
	
	SElExceptionPtr	(*Add)(struct _SUserIntervals* me, uint32_t userCount, uint32_t deltaTime); 
	SElExceptionPtr	(*Resize)(struct _SUserIntervals* me); 

	uint32_t	_AllocatedCount;
	uint32_t	_UsedCount;
} SUserIntervals, *SUserIntervalsPtr;

SElExceptionPtr userintervals_new( SUserIntervalsPtr* pUserIntervals);
SElExceptionPtr userintervals_delete( SUserIntervalsPtr* pUserIntervals);

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__INTERVAL_H__)

