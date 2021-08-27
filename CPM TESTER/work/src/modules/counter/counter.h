#if !defined(__COUNTER_H__)
#define __COUNTER_H__

#include <exception/Exception.h>
#include <objlist/objlist.h>
#include <Windows.h> 

#if defined(__cplusplus) || defined(__cplusplus__)
	extern "C" {
#endif

typedef struct _SHWCounter
{
	SElExceptionPtr (*OpenWithConfiguration)( struct _SHWCounter* me,
							  			   const char* cfgFile );

	SElExceptionPtr (*GenerateFreq)( struct _SHWCounter* me,
								  const char* name, /* logical name */
								  double freq, /* 0.0Hz = stop task */
								  double duty); /* percent */

	SElExceptionPtr (*MeasFreq)( struct _SHWCounter* me,
								  const char* name, /* logical name */
								  double* freq);

	SElExceptionPtr (*MeasPulseWidth)( struct _SHWCounter* me,
								  const char* name, /* logical name */
								  double* duty); /* sec */

	CRITICAL_SECTION	_Lock;
	void*				_TaskConfig;
	int32_t				_TaskConfigSize;

} SHWCounter, *SHWCounterPtr;

SElExceptionPtr	hwcounter_new(SHWCounterPtr* pCounter);	
SElExceptionPtr	hwcounter_delete(SHWCounterPtr* pCounter);

#define ERROR_COUNTER_LOGICAL_NAME_NOT_EXIST		-10000

#if defined(__cplusplus) || defined(__cplusplus__)
	}
#endif

#endif /* __COUNTER_H__ */




