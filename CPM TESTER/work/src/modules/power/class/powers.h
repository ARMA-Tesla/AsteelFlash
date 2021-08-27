#ifndef _POWERS_H_
#define _POWERS_H_

#include <windows.h>
#include <exception/Exception.h>
#include "Ag34450.h"

#define MAX_POWER_NAME			128
#define MAX_BUFFER_SIZE				20

#define POWER_CONNECTION_FAILED				-1
#define POWER_SEND_FAILED						-1
#define POWER_RESET_FAILED					-1
#define POWER_READ_STATUS_FAILED				-1



 
typedef struct _Power
{
	char*				powerName;
	char*				optionString;
	char				VISAResourceName[256];
	WORD				port;
	WORD				baudRate;
	CRITICAL_SECTION	lock;
	int32_t				status;
	int32_t				readMode;
	char				readBuffer[MAX_BUFFER_SIZE];
	HANDLE				specLib;
	uint32_t            VISAResourceNameOut;

	
	SElExceptionPtr (*Open)(struct _Power* me,const char*	resource, const char* optionString);
	SElExceptionPtr (*Close)(struct _Power* me);
		
	/* Export */
	SElExceptionPtr (*Initialize)(struct _Power* me);
	SElExceptionPtr (*Disconnect)(struct _Power* me);
	SElExceptionPtr (*ConfigureCurrentLimit)(struct _Power* me, double CurrentLimit);
	SElExceptionPtr (*ConfigureOutput)(struct _Power* me, double VoltageLevel);
	SElExceptionPtr (*ConfigureOVP)(struct _Power* me, double VoltageLimit);
	SElExceptionPtr (*ConfigureOCP)(struct _Power* me, double CurrentLimit);
	SElExceptionPtr (*EnableOutput)(struct _Power* me, int32_t EnableOutput);

	/* Private */
	SElExceptionPtr (*_InitializeSpec)(struct _Power* me, ViRsrc *VISAResourceName,  bool_t IDQueryTrue, bool_t ResetTrue,  uint32_t BaudRate, ViSession *VISAResourceNameOut);
	SElExceptionPtr (*_DisconnectSpec)(struct _Power* me, int *VISAResourceNameOut);
	SElExceptionPtr (*_ConfigureCurrentLimitSpec)(struct _Power* me, uintptr_t *VISAResourceNameOut, double CurrentLimit00200A, int32_t Address1, uint16_t UnitControlOperation0Single);
	SElExceptionPtr (*_ConfigureOutputSpec)(struct _Power* me, uintptr_t *VISAResourceNameOut, double VoltageLevel500V, int32_t Address1, uint16_t UnitControlOperation0Single, int32_t EnableOutput);
	SElExceptionPtr (*_ConfigureOVPSpec)(struct _Power* me, uintptr_t *VISAResourceNameOut, double VoltageLimit66000V, int32_t Address1, uint16_t UnitControlOperation0Single, int32_t EnableOutput);	
	SElExceptionPtr (*_ConfigureOCPSpec)(struct _Power* me, uintptr_t *VISAResourceNameOut, double CurrentLimit51000A, int32_t Address1, uint16_t UnitControlOperation0Single, int32_t EnableOutput);	
	SElExceptionPtr (*_EnableOutputSpec)(struct _Power* me, uintptr_t *VISAResourceNameOut, int32_t Address1, uint16_t UnitControlOperation0Single, int32_t EnableOutput);
	
} SPower, *SPowerPtr;


typedef struct _Powers
{
	SPowerPtr*	powers;
	int32_t			numPowers;
	int32_t			currentIndex;
	
	SElExceptionPtr (*GetPowerByName)(
		struct _Powers*	me,
		const char*			powerName,
		SPowerPtr*		power
	);
	SElExceptionPtr (*GetPowerByIndex)(
		struct _Powers*	me,
		int32_t				index,
		SPowerPtr*		power
	);
	SElExceptionPtr (*GetCurrentPower)(
		struct _Powers*	me,
		SPowerPtr*		power
	);
	SElExceptionPtr (*SetCurrentPowerByName)(
		struct _Powers*	me,
		const char*			powerName
	);
	SElExceptionPtr (*GetFirstPower)(
		struct _Powers*	me,
		SPowerPtr*		power
	);
	SElExceptionPtr (*GetNextPower)(
		struct _Powers*	me,
		SPowerPtr*		power
	);
	
	SElExceptionPtr (*InitPowers)(struct _Powers* me);
	SElExceptionPtr (*ClosePowers)(struct _Powers* me);
	
} SPowers, *SPowersPtr;

SElExceptionPtr powers_new(SPowersPtr* powers);
SElExceptionPtr powers_delete(SPowersPtr* powers);

SElExceptionPtr power_new(
	SPowerPtr*	power,
	const char*		name,
	const char*		optionString
);
SElExceptionPtr power_delete(SPowerPtr* power);

#endif /* _POWERS_H_ */
