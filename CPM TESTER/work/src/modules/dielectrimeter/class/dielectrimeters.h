#ifndef _DIELECTRIMETERS_H_
#define _DIELECTRIMETERS_H_

#include <windows.h>
#include <exception/Exception.h>

#define MAX_DIELECTRIMETER_NAME			128
#define MAX_BUFFER_SIZE				20

#define DIELECTRIMETER_CONNECTION_FAILED				-1
#define DIELECTRIMETER_SEND_FAILED						-1
#define DIELECTRIMETER_RESET_FAILED					-1
#define DIELECTRIMETER_READ_STATUS_FAILED				-1



 
typedef struct _Dielectrimeter
{
	char*				dielectrimeterName;
	char*				optionString;
	WORD				port;
	WORD				baudRate;
	CRITICAL_SECTION	lock;
	int32_t				status;
	int32_t				readMode;
	char				readBuffer[MAX_BUFFER_SIZE];
	HANDLE				specLib;

	
	SElExceptionPtr (*Open)(struct _Dielectrimeter* me,const char*	resource, const char* optionString);
	SElExceptionPtr (*Close)(struct _Dielectrimeter* me);
	SElExceptionPtr (*Connect)(struct _Dielectrimeter* me);
	SElExceptionPtr (*Disconnect)(struct _Dielectrimeter* me);
	SElExceptionPtr (*Write)(struct _Dielectrimeter* me, const char* command);
	SElExceptionPtr (*Read)(struct _Dielectrimeter* me, char* read);
	SElExceptionPtr (*SetModeManual)(struct _Dielectrimeter* me);
	SElExceptionPtr (*SetStep)(struct _Dielectrimeter* me, int step);
	SElExceptionPtr (*SetOutput)(struct _Dielectrimeter* me, int output);
	SElExceptionPtr (*TestResult)(struct _Dielectrimeter* me, int *result, double *voltage_kv, double *current_mA);
	SElExceptionPtr (*SystemError)(struct _Dielectrimeter* me, char * error);
	SElExceptionPtr (*ClearError)(struct _Dielectrimeter* me);			

	/* Private */
	SElExceptionPtr (*_ConnectSpec)(struct _Dielectrimeter* me);
	SElExceptionPtr (*_DisconnectSpec)(struct _Dielectrimeter* me);
	SElExceptionPtr (*_WriteSpec)(struct _Dielectrimeter* me, const char* command);
	SElExceptionPtr (*_ReadSpec)(struct _Dielectrimeter* me, char* read);
	SElExceptionPtr (*_SetModeManualSpec)(struct _Dielectrimeter* me);
	SElExceptionPtr (*_SetStepSpec)(struct _Dielectrimeter* me, int step);
	SElExceptionPtr (*_SetOutputSpec)(struct _Dielectrimeter* me, int output);
	SElExceptionPtr (*_TestResultSpec)(struct _Dielectrimeter* me, int *result, double *voltage_kv, double *current_mA);
	SElExceptionPtr (*_SystemErrorSpec)(struct _Dielectrimeter* me, char *error);
	SElExceptionPtr (*_ClearErrorSpec)(struct _Dielectrimeter* me);
				
} SDielectrimeter, *SDielectrimeterPtr;


typedef struct _Dielectrimeters
{
	SDielectrimeterPtr*	dielectrimeters;
	int32_t			numDielectrimeters;
	int32_t			currentIndex;
	
	SElExceptionPtr (*GetDielectrimeterByName)(
		struct _Dielectrimeters*	me,
		const char*			dielectrimeterName,
		SDielectrimeterPtr*		dielectrimeter
	);
	SElExceptionPtr (*GetDielectrimeterByIndex)(
		struct _Dielectrimeters*	me,
		int32_t				index,
		SDielectrimeterPtr*		dielectrimeter
	);
	SElExceptionPtr (*GetCurrentDielectrimeter)(
		struct _Dielectrimeters*	me,
		SDielectrimeterPtr*		dielectrimeter
	);
	SElExceptionPtr (*SetCurrentDielectrimeterByName)(
		struct _Dielectrimeters*	me,
		const char*			dielectrimeterName
	);
	SElExceptionPtr (*GetFirstDielectrimeter)(
		struct _Dielectrimeters*	me,
		SDielectrimeterPtr*		dielectrimeter
	);
	SElExceptionPtr (*GetNextDielectrimeter)(
		struct _Dielectrimeters*	me,
		SDielectrimeterPtr*		dielectrimeter
	);
	
	SElExceptionPtr (*InitDielectrimeters)(struct _Dielectrimeters* me);
	SElExceptionPtr (*CloseDielectrimeters)(struct _Dielectrimeters* me);
	
} SDielectrimeters, *SDielectrimetersPtr;

SElExceptionPtr dielectrimeters_new(SDielectrimetersPtr* dielectrimeters);
SElExceptionPtr dielectrimeters_delete(SDielectrimetersPtr* dielectrimeters);

SElExceptionPtr dielectrimeter_new(
	SDielectrimeterPtr*	dielectrimeter,
	const char*		name,
	const char*		optionString
);
SElExceptionPtr dielectrimeter_delete(SDielectrimeterPtr* dielectrimeter);

#endif /* _DIELECTRIMETERS_H_ */
