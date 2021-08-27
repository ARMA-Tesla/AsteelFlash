#if !defined(__SENSORS_H__)
#define __SENSORS_H__

#include <exception/Exception.h>
#include <objlist/objlist.h>
#include <Windows.h> 
    
#if defined(__cplusplus) || defined(__cplusplus__)
	extern "C" {
#endif

 
typedef SElExceptionPtr (_FUNCC *SensorCallbackFnc_t)(const char* name, uint32_t state, void* pData);

typedef struct _SDIOSensors
{
	SElExceptionPtr (*OpenWithConfiguration)( struct _SDIOSensors* me,
							  			   const char* cfgFile );
	SElExceptionPtr (*SetState)( struct _SDIOSensors* me,
							  const char* name, /* line name or task name */
							  uint32_t state);
	SElExceptionPtr (*GetState)( struct _SDIOSensors* me,
							  const char* name, /* line name or task name */
							  uint32_t* state);
	SElExceptionPtr (*GetAI)( struct _SDIOSensors* me,
							  const char* name, /* line name or task name */
							  uint32_t* value);
	SElExceptionPtr (*RegisterCallback)( struct _SDIOSensors* me,
							  const char* name, /* line name or task name */
							  SensorCallbackFnc_t pFnc,
							  void* pData);
	SElExceptionPtr	(*Close)( struct _SDIOSensors* me);
	
	void*				_pConfig;
	CRITICAL_SECTION	_Lock;
	int					_panel;
	HANDLE				_hThread;
	HANDLE				_hEventStop;
	char				_path[512];
	bool_t				_IsSimulation;
#if defined(_CVI_)     
    int					_pool_handle;
#endif
} SDIOSensors, *SDIOSensorsPtr;

SElExceptionPtr	diosenosrs_new( SDIOSensorsPtr* pSensors);	
SElExceptionPtr	diosenosrs_delete( SDIOSensorsPtr* pSensors);

SElExceptionPtr get_status_input(int input[22]) ;  

#define ERROR_SENSORS_LOGICAL_NAME_NOT_EXIST		-10000

#if defined(__cplusplus) || defined(__cplusplus__)
	}
#endif

#endif /* __SENSORS_H__ */




