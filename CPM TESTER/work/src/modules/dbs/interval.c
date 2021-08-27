#include "interval.h"
#include <string.h>

#define INRETVAL_ALLOC_COUNT      5

static SElExceptionPtr   IntervalResultAdd(
   struct _SIntervalResult* pInterval,
   time_t   st,
   time_t   sp
);
static SElExceptionPtr   IntervalResultAddEx(
   struct _SIntervalResult* pInterval,
   time_t   st,
   time_t   sp,
   int32_t  value
);
static uint32_t   IntervalResultCount(
   struct _SIntervalResult* pInterval
);
static SElExceptionPtr   IntervalResultResize(
   struct _SIntervalResult* pInterval
);
static SElExceptionPtr   IntervalResultMerge(
   struct _SIntervalResult* pInterval
);

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "intervalresult_new"
SElExceptionPtr intervalresult_new(SIntervalResultPtr* pInterval)
{
	SElExceptionPtr	pexception = NULL;    
	SIntervalResultPtr   me = NULL;

	me = calloc( 1, sizeof(SIntervalResult));
	EXCCHECKALLOC( me);

	*pInterval = me;

	me->Add			= IntervalResultAdd;
	me->AddEx		= IntervalResultAddEx;
	me->Count		= IntervalResultCount;
	me->Resize		= IntervalResultResize;
	me->Merge		= IntervalResultMerge;

Error:
    EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "intervalresult_new"
SElExceptionPtr intervalresult_delete(SIntervalResultPtr* pInterval)
{
   if ( *pInterval )
   {
      free( (*pInterval)->intervals);
      free( *pInterval);
      *pInterval = NULL;
   }

   return NULL;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "MergeIntervals"
SElExceptionPtr	MergeIntervals(
	SIntervalResultPtr		pIntervals1,
	SIntervalResultPtr		pIntervals2,
	SIntervalResultPtr*		pResult,
	uint32_t*				interval_time
)
{
	SElExceptionPtr	pexception = NULL;   
	uint32_t	count;
	uint32_t	i, sum = 0;

	EXCCHECK( intervalresult_new(pResult));

	count = pIntervals1->Count(pIntervals1);
	for( i = 0; i < count; i++)
	{
		EXCCHECK( (*pResult)->Add( *pResult,
								  (pIntervals1->intervals + i)->start,
								  (pIntervals1->intervals + i)->stop
								  ));
	} 

	count = pIntervals2->Count(pIntervals2);
	for( i = 0; i < count; i++)
	{
		EXCCHECK( (*pResult)->Add( *pResult,
								  (pIntervals2->intervals + i)->start,
								  (pIntervals2->intervals + i)->stop
								  ));
	} 

	EXCCHECK( (*pResult)->Merge( *pResult));

	count = (*pResult)->Count(*pResult);
	for( i = 0; i < count; i++)
	{
		sum += (uint32_t) (((*pResult)->intervals + i)->stop - ((*pResult)->intervals + i)->start);
	}

	if(interval_time) *interval_time = sum;
	
Error:
	EXCRETHROW( pexception);  
}  /* MergeIntervals */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "InverseIntervals"
SElExceptionPtr	InverseIntervals(
	time_t					t1,
	time_t					t2,
	SIntervalResultPtr		pIntervals,
	SIntervalResultPtr*		pResult,
	uint32_t*				interval_time  
)
{
	SElExceptionPtr	pexception = NULL; 
	time_t		start, stop;
	uint32_t	i = 0;
	uint32_t	count, sum = 0;

	EXCCHECK(  intervalresult_new( pResult));

	start = t1; 
	i = 0;
	count = pIntervals->Count(pIntervals);
	while ( start < t2 && i < count)
	{
		stop = (pIntervals->intervals + i)->start;
		EXCCHECK((*pResult)->Add(*pResult, start, stop));
		start = (pIntervals->intervals + i)->stop;
		i++;
	}
	
	if ( start < t2 )
	{
		stop = t2;
		EXCCHECK((*pResult)->Add(*pResult, start, stop));
	}
	
	count = (*pResult)->Count(*pResult);
	for( i = 0; i < count; i++)
	{
		sum += (uint32_t) (((*pResult)->intervals + i)->stop - ((*pResult)->intervals + i)->start);
	}	

	if(interval_time) *interval_time = sum;  
	
Error:
	EXCRETHROW( pexception);  
} /* SplitTimeToIntervals */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "IntervalResultAdd"
static SElExceptionPtr   IntervalResultAdd(
   struct _SIntervalResult* pInterval,
      time_t   st,
      time_t   sp
)
{
	SElExceptionPtr	pexception = NULL;  
   SIntervalResultEntryPtr   pinterval = NULL;
   uint32_t   i;

   if ( pInterval->_AllocatedCount == pInterval->_UsedCount )
   {
      pInterval->intervals = realloc( pInterval->intervals, sizeof(SIntervalResultEntry) * 
                              (pInterval->_AllocatedCount + INRETVAL_ALLOC_COUNT));
      EXCCHECKALLOC( pInterval->intervals);
      pInterval->_AllocatedCount += INRETVAL_ALLOC_COUNT;

   }
   pinterval = pInterval->intervals;

   for( i = 0; i < pInterval->_UsedCount; i++, pinterval++)
   {
      if (st < pinterval->start)
      {
         break;
      }
      else
      if ( st == pinterval->start &&
          sp < pinterval->stop)
      {
         break;
      }
   } 
   
   if ( i < pInterval->_UsedCount )
   {
      memmove( pinterval + 1,
             pinterval,
             sizeof(SIntervalResultEntry)*(pInterval->_UsedCount-i));
   } 
   pinterval->start = st;
   pinterval->stop  = sp;
   pInterval->_UsedCount += 1;

Error:
   EXCRETHROW( pexception);  
} 

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "IntervalResultAddEx"
static SElExceptionPtr   IntervalResultAddEx(
   struct _SIntervalResult* pInterval,
      time_t   st,
      time_t   sp,
      int32_t   value
)
{
	SElExceptionPtr	pexception = NULL;  
	SIntervalResultEntryPtr   pinterval = NULL;
	uint32_t            i;

	if ( pInterval->_AllocatedCount == pInterval->_UsedCount )
	{
		pInterval->intervals = realloc( pInterval->intervals, sizeof(SIntervalResultEntry) * 
							  (pInterval->_AllocatedCount + INRETVAL_ALLOC_COUNT));
		EXCCHECKALLOC( pInterval->intervals);
		pInterval->_AllocatedCount += INRETVAL_ALLOC_COUNT;

	}
	pinterval = pInterval->intervals;

	for( i = 0; i < pInterval->_UsedCount; i++, pinterval++)
	{
		if(st < pinterval->start)
			break;
		else if( st==pinterval->start && sp<pinterval->stop )
			break;
	} 
   
	if ( i < pInterval->_UsedCount )
	{
		memmove( pinterval + 1,
				 pinterval,
				 sizeof(SIntervalResultEntry)*(pInterval->_UsedCount-i));
	} 
	pinterval->start  = st;
	pinterval->stop   = sp;
	pinterval->value  = value;
	pInterval->_UsedCount += 1;

Error:
	EXCRETHROW( pexception);  
} 

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "IntervalResultCount"
static uint32_t   IntervalResultCount(
   struct _SIntervalResult* pInterval
)
{
   return pInterval->_UsedCount;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "IntervalResultResize"
static SElExceptionPtr   IntervalResultResize(
   struct _SIntervalResult* pInterval
)
{
	SElExceptionPtr	pexception = NULL;  
   int32_t   error = 0;

   if ( pInterval->_AllocatedCount > pInterval->_UsedCount )
   {
      pInterval->intervals = realloc( pInterval->intervals,
                              sizeof(SIntervalResultEntry)* 
                              pInterval->_UsedCount);
      CHECKALLOC( pInterval->intervals);
      pInterval->_AllocatedCount = pInterval->_UsedCount;
   }
Error:
   EXCRETHROW( pexception);  
} 

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "IntervalResultMerge"
static SElExceptionPtr   IntervalResultMerge(
   struct _SIntervalResult* pInterval
)
{
	SElExceptionPtr	pexception = NULL;  
	uint32_t		i;
	SIntervalResultEntryPtr   pinterval = NULL;

   pinterval = pInterval->intervals;
   for( i = 1; i < pInterval->_UsedCount; )
   {
      if ( (pinterval+i-1)->stop > (pinterval+i)->start )
      {
         
         if ( (pinterval+i-1)->stop < (pinterval+i)->stop )
         {
            (pinterval+i-1)->stop = (pinterval+i)->stop;
         }
         if ( (pInterval->_UsedCount - (i+1) ) > 0 )
         {
            memmove( pinterval+i, pinterval+i+1, sizeof(SIntervalResultEntry)* (pInterval->_UsedCount - (i+1) ));
         }
         pInterval->_UsedCount -= 1;
      }
      else
      {
         i++;
      }
   }

   for( i = 0; i < pInterval->_UsedCount; )
   {
      if ((pinterval+i)->start == (pinterval+i)->stop)
      {
         if ( (pInterval->_UsedCount - (i+1) ) > 0 )
         {
            memmove( pinterval+i, pinterval+i+1, sizeof(SIntervalResultEntry)* (pInterval->_UsedCount - (i+1) ));
         }
         pInterval->_UsedCount -= 1;
      }
      else
      {
         i++;
      }
   }

   EXCCHECK( pInterval->Resize( pInterval));

Error:
   EXCRETHROW( pexception);  
}

#define DEFAULT_USERS_RESAULT		5

static uint32_t UserIntervalsCount(struct _SUserIntervals* me);
static SElExceptionPtr UserIntervalsAdd(struct _SUserIntervals* me, uint32_t userCount, uint32_t deltaTime); 
static SElExceptionPtr UserIntervalsResize(struct _SUserIntervals* me);

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "userintervals_new"
SElExceptionPtr userintervals_new(SUserIntervalsPtr* pUserIntervals)
{
	SElExceptionPtr	pexception = NULL;  
	SUserIntervalsPtr	me = NULL;

	me = calloc(1, sizeof(SUserIntervals));
	EXCCHECKALLOC( me);
	
	*pUserIntervals = me;
	
	me->Add		= UserIntervalsAdd;
	me->Count	= UserIntervalsCount;
	me->Resize	= UserIntervalsResize;

Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "userintervals_delete"
SElExceptionPtr userintervals_delete(SUserIntervalsPtr* pUserIntervals)
{
	if ( *pUserIntervals )
	{
		free( (*pUserIntervals)->intervals);
		free( *pUserIntervals);
		*pUserIntervals = NULL;
	}
	return NULL;
} 

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "UserIntervalsAdd"
static SElExceptionPtr	UserIntervalsAdd(
	struct _SUserIntervals* pInterval,
	uint32_t userCount,
	uint32_t detlaTime
)
{
	SElExceptionPtr	pexception = NULL;  
	int32_t	error = 0;

	if ( (userCount + 1) > pInterval->_AllocatedCount )
	{
		uint32_t	alloc_size = ((userCount + 1) > DEFAULT_USERS_RESAULT) ? (userCount + 1) : DEFAULT_USERS_RESAULT;
		
		pInterval->intervals = realloc( pInterval->intervals, sizeof(SIntervalResult) * alloc_size); 
		CHECKALLOC( pInterval->intervals);
		memset( pInterval->intervals + pInterval->_UsedCount, 0, sizeof(SIntervalResult) * (alloc_size-pInterval->_UsedCount));
		pInterval->_AllocatedCount = alloc_size; 
	}
	(pInterval->intervals + userCount)->UserCount  = userCount;
	(pInterval->intervals + userCount)->TimeDelay += detlaTime;

	if (pInterval->_UsedCount < userCount )
	{
		pInterval->_UsedCount = userCount; 
	}
Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "IntervalResultCount"
static uint32_t	UserIntervalsCount(
	struct _SUserIntervals* pInterval
)
{
	return pInterval->_UsedCount;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "UserIntervalsResize"
static SElExceptionPtr	UserIntervalsResize(
	struct _SUserIntervals* pInterval
)
{
	SElExceptionPtr	pexception = NULL;  

	if ( pInterval->_AllocatedCount > pInterval->_UsedCount )
	{
		pInterval->intervals = realloc( pInterval->intervals,
										sizeof(SUserIntervalsEntry)* 
										pInterval->_UsedCount);
		EXCCHECKALLOC( pInterval->intervals);
		pInterval->_AllocatedCount = pInterval->_UsedCount;
	}
Error:
	EXCRETHROW( pexception);
} /* UserIntervalsResize */
