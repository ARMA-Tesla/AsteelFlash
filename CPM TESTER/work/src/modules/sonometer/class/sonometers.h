#ifndef _SONOMETERS_H_
#define _SONOMETERS_H_

#include <windows.h>
#include <exception/Exception.h>

#define MAX_SONOMETER_NAME			128
#define MAX_BUFFER_SIZE				20

#define SONOMETER_CONNECTION_FAILED			-1
#define SONOMETER_SEND_FAILED				-1
#define SONOMETER_RESET_FAILED				-1
#define SONOMETER_READ_FAILED				-1



 
typedef struct _Sonometer
{
	char*				sonometerName;
	char*				optionString;
	int					port;
	int 				baudRate;
	CRITICAL_SECTION	lock;
	int32_t				status;
	int32_t				readMode;
	char				readBuffer[MAX_BUFFER_SIZE];
	HANDLE				specLib;

	
	SElExceptionPtr (*Open)(struct _Sonometer* me,const char*	resource, const char* optionString);
	SElExceptionPtr (*Close)(struct _Sonometer* me);
	SElExceptionPtr (*Connect)(struct _Sonometer* me);
	SElExceptionPtr (*Disconnect)(struct _Sonometer* me);
	SElExceptionPtr (*Write)(struct _Sonometer* me, const char* command);
	SElExceptionPtr (*Read)(struct _Sonometer* me, const char* data);
	SElExceptionPtr (*SoundLevelMeter)(struct _Sonometer* me, int group, float *data, int *count_data, char *log);

	/* Private */
	SElExceptionPtr (*_ConnectSpec)(struct _Sonometer* me);
	SElExceptionPtr (*_DisconnectSpec)(struct _Sonometer* me);
	SElExceptionPtr (*_WriteSpec)(struct _Sonometer* me, const char* command);
	SElExceptionPtr (*_ReadSpec)(struct _Sonometer* me, const char* data);
	SElExceptionPtr (*_SoundLevelMeterSpec)(struct _Sonometer* me, int group, float *data, int *count_data, char *log);
	
} SSonometer, *SSonometerPtr;


typedef struct _Sonometers
{
	SSonometerPtr*	sonometers;
	int32_t			numSonometers;
	int32_t			currentIndex;
	
	SElExceptionPtr (*GetSonometerByName)(
		struct _Sonometers*	me,
		const char*			sonometerName,
		SSonometerPtr*		sonometer
	);
	SElExceptionPtr (*GetSonometerByIndex)(
		struct _Sonometers*	me,
		int32_t				index,
		SSonometerPtr*		sonometer
	);
	SElExceptionPtr (*GetCurrentSonometer)(
		struct _Sonometers*	me,
		SSonometerPtr*		sonometer
	);
	SElExceptionPtr (*SetCurrentSonometerByName)(
		struct _Sonometers*	me,
		const char*			sonometerName
	);
	SElExceptionPtr (*GetFirstSonometer)(
		struct _Sonometers*	me,
		SSonometerPtr*		sonometer
	);
	SElExceptionPtr (*GetNextSonometer)(
		struct _Sonometers*	me,
		SSonometerPtr*		sonometer
	);
	
	SElExceptionPtr (*InitSonometers)(struct _Sonometers* me);
	SElExceptionPtr (*CloseSonometers)(struct _Sonometers* me);
	
} SSonometers, *SSonometersPtr;

SElExceptionPtr sonometers_new(SSonometersPtr* sonometers);
SElExceptionPtr sonometers_delete(SSonometersPtr* sonometers);

SElExceptionPtr sonometer_new(
	SSonometerPtr*	sonometer,
	const char*		name,
	const char*		optionString
);
SElExceptionPtr sonometer_delete(SSonometerPtr* sonometer);

#endif /* _SONOMETERS_H_ */
