/******************************************************************************
 *  Log_struct.h
 ******************************************************************************/

#if !defined(__LOG_STRUCT_H__) 
#define __LOG_STRUCT_H__

#include <exception/Exception.h>
#include <windows.h>
#include <log/log.h>

typedef struct _SLogSession
{
	char		listener[512];
	ELogChannel	channel;
	ELogOutput	output;
	uint32_t	mask;
	bool_t		enabled;
	char		config[512]; /* file path */
	void		*file; /* FILE* */
  
	void (*Message)(struct _SLogSession* me, uint32_t priority, const char* message);
  
} SLogSession, *SLogSessionPtr;

typedef struct _SLog
{
	LogFce_t		Log; /* always on the first place */ 
 
	CRITICAL_SECTION	_Lock; 
	SLogSessionPtr		_pLogSession;
	int32_t				_pLogSessionSize;

	SElExceptionPtr (*Initialize)(struct _SLog* me, const char* file);
  
	void*			LogApi;		
	ELogChannel		LogChannel;

} SLog, *SLogPtr;

SElExceptionPtr	log_new(SLogPtr* pLog);
SElExceptionPtr	log_delete(SLogPtr* pLog);

#endif /* __LOG_STRUCT_H__ */
