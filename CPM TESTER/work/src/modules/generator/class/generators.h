#ifndef _GENERATORS_H_
#define _GENERATORS_H_

#include <windows.h>
#include <exception/Exception.h>

#define MAX_GENERATOR_NAME			128
#define MAX_BUFFER_SIZE				20

#define GENERATOR_CONNECTION_FAILED				-1
#define GENERATOR_SEND_FAILED						-1
#define GENERATOR_RESET_FAILED					-1
#define GENERATOR_READ_STATUS_FAILED				-1



 
typedef struct _Generator
{
	char*				generatorName;
	char*				optionString;
	WORD				port;
	WORD				baudRate;
	CRITICAL_SECTION	lock;
	int32_t				status;
	int32_t				readMode;
	char				readBuffer[MAX_BUFFER_SIZE];
	HANDLE				specLib;

	
	SElExceptionPtr (*Open)(struct _Generator* me,const char*	resource, const char* optionString);
	SElExceptionPtr (*Close)(struct _Generator* me);
	SElExceptionPtr (*Connect)(struct _Generator* me);
	SElExceptionPtr (*Disconnect)(struct _Generator* me);
	SElExceptionPtr (*Write)(struct _Generator* me, const char* command);
	SElExceptionPtr (*Read)(struct _Generator* me, char* read);
	SElExceptionPtr (*SetModeContinuous)(struct _Generator* me);
	SElExceptionPtr (*SetOutput)(struct _Generator* me, int output);
	SElExceptionPtr (*SetFonction )(struct _Generator* me, double frequency, double amplitude, double offset, double duty);
	SElExceptionPtr (*SystemError)(struct _Generator* me, char * error);
	SElExceptionPtr (*ClearError)(struct _Generator* me);			

	/* Private */
	SElExceptionPtr (*_ConnectSpec)(struct _Generator* me);
	SElExceptionPtr (*_DisconnectSpec)(struct _Generator* me);
	SElExceptionPtr (*_WriteSpec)(struct _Generator* me, const char* command);
	SElExceptionPtr (*_ReadSpec)(struct _Generator* me, char* read);
	SElExceptionPtr (*_SetModeContinuousSpec)(struct _Generator* me);
	SElExceptionPtr (*_SetOutputSpec)(struct _Generator* me, int output);
	SElExceptionPtr (*_SetFonctionSpec)(struct _Generator* me, double frequency, double amplitude, double offset, double duty);
	SElExceptionPtr (*_SystemErrorSpec)(struct _Generator* me, char *error);
	SElExceptionPtr (*_ClearErrorSpec)(struct _Generator* me);
				
} SGenerator, *SGeneratorPtr;


typedef struct _Generators
{
	SGeneratorPtr*	generators;
	int32_t			numGenerators;
	int32_t			currentIndex;
	
	SElExceptionPtr (*GetGeneratorByName)(
		struct _Generators*	me,
		const char*			generatorName,
		SGeneratorPtr*		generator
	);
	SElExceptionPtr (*GetGeneratorByIndex)(
		struct _Generators*	me,
		int32_t				index,
		SGeneratorPtr*		generator
	);
	SElExceptionPtr (*GetCurrentGenerator)(
		struct _Generators*	me,
		SGeneratorPtr*		generator
	);
	SElExceptionPtr (*SetCurrentGeneratorByName)(
		struct _Generators*	me,
		const char*			generatorName
	);
	SElExceptionPtr (*GetFirstGenerator)(
		struct _Generators*	me,
		SGeneratorPtr*		generator
	);
	SElExceptionPtr (*GetNextGenerator)(
		struct _Generators*	me,
		SGeneratorPtr*		generator
	);
	
	SElExceptionPtr (*InitGenerators)(struct _Generators* me);
	SElExceptionPtr (*CloseGenerators)(struct _Generators* me);
	
} SGenerators, *SGeneratorsPtr;

SElExceptionPtr generators_new(SGeneratorsPtr* generators);
SElExceptionPtr generators_delete(SGeneratorsPtr* generators);

SElExceptionPtr generator_new(
	SGeneratorPtr*	generator,
	const char*		name,
	const char*		optionString
);
SElExceptionPtr generator_delete(SGeneratorPtr* generator);

#endif /* _GENERATORS_H_ */
