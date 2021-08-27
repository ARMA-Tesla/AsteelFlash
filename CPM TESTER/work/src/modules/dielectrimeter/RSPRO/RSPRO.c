#include <rs232.h>
#include <cvintwrk.h>
#include <userint.h>
#include <tcpsupp.h>
#include "RSPRO.h"
#include <stdio.h>
#include <formatio.h>
#include <utility.h>

#pragma warning( push)
#pragma warning( disable: 4100 4127)

#define DIELECTRIMETER_TIMEOUT 	5000

#undef __FUNC__
#define __FUNC__	"{RSPRO}::Connect"
SElExceptionPtr Connect(SDielectrimeterPtr me)
{

SElExceptionPtr	pexception = NULL;

	if (OpenComConfig (me->port, "", me->baudRate, 0, 8, 1, 512, 512) < 0) 
		EXCTHROW( DIELECTRIMETER_CONNECTION_FAILED, "Probléme Ouverture COM");

Error:
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__	"{RSPRO}::Disconnect"
SElExceptionPtr Disconnect(SDielectrimeterPtr me)
{

SElExceptionPtr	pexception = NULL;
CloseCom (me->port);

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{RSPRO}::Write"
SElExceptionPtr Write(SDielectrimeterPtr me,const char* command)
{
	SElExceptionPtr	pexception = NULL;

	ComWrt (me->port, command, strlen (command));

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{RSPRO}::Read"
SElExceptionPtr Read(SDielectrimeterPtr me,const char* read)
{
	SElExceptionPtr	pexception = NULL;
	int  BitRead;
	
	BitRead = GetInQLen (me->port);
	ComRd (me->port, read, BitRead);

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{RSPRO}::SetModeManual"
SElExceptionPtr SetModeManual(SDielectrimeterPtr me)
{
	SElExceptionPtr	pexception = NULL;
	char command[128];
	char BufferRead[128];
	char sAmmountLabel[128];
	int  BitRead;

	/*Puts the tester mode to MANU.*/
	sprintf (command, "MAIN:FUNC MANU\n");
	ComWrt (me->port, command, strlen (command));
	Delay (0.5);
		
	/*Request Mode*/
	sprintf (command, "MAIN:FUNC ?\n");
	ComWrt (me->port, command, strlen (command));
	Delay (0.5);
	BitRead = GetInQLen (me->port);
	ComRd (me->port, BufferRead, BitRead);
	if (FindPattern (BufferRead, 0, -1, "MAN", 0, 0) < 0)
		EXCTHROW( DIELECTRIMETER_READ_STATUS_FAILED, "SetModeManual Failed");	
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{RSPRO}::SetStep"
SElExceptionPtr SetStep(SDielectrimeterPtr me, int step)
{
	SElExceptionPtr	pexception = NULL;
	char command[128];
	char BufferRead[128];
	char sAmmountLabel[128];
	int  BitRead;

	/*Sets the current MANU number to 1~100.*/
	sprintf (command, "MANU:STEP %d\n", step);
	ComWrt (me->port, command, strlen (command));
	Delay (0.2);
		
	/*Request Step */
	sprintf (command, "MANU:STEP ?\n");
	ComWrt (me->port, command, strlen (command));
	Delay (0.5);
	BitRead = GetInQLen (me->port);
	ComRd (me->port, BufferRead, BitRead);
	if (atoi(BufferRead) != step)
		EXCTHROW( DIELECTRIMETER_READ_STATUS_FAILED, "SetStep Failed");	
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{RSPRO}::SetOutput"
SElExceptionPtr SetOutput(SDielectrimeterPtr me, int output)
{
	SElExceptionPtr	pexception = NULL;
	char command[128];
	char BufferRead[128];
	char sAmmountLabel[128];
	int  BitRead;

	/*Turns the currently selected test (output) on or off.*/
	if (output == 1)
	sprintf (command, "FUNC:TEST ON\n");
	else 	
	sprintf (command, "FUNC:TEST OFF\n");
	ComWrt (me->port, command, strlen (command));
	Delay (0.5);	
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{RSPRO}::TestResult"
SElExceptionPtr TestResult(SDielectrimeterPtr me, int *result, double *voltage_kv, double *current_mA)
{
	SElExceptionPtr	pexception = NULL;
	char command[128];
	char BufferRead[128]=""; 
	char sAmmountLabel[128];
	int  BitRead;
	double resistance =0.0;
	char mode[32];
	int index;	 
	char charVoltage[50]="";
	char charCurrent[50]="";
	/* Returns the test result of the current test. */
	sprintf (command, "MEAS?\n");
	ComWrt (me->port, command, strlen (command));
	Delay (0.7);
	BitRead = GetInQLen (me->port);
	ComRd (me->port, BufferRead, BitRead);
	if (BitRead > 20)
		{

			index=FindPattern (BufferRead, 0, -1, "FAIL", 1, 0);
		if (index >0)	
			*result=0;
		if (FindPattern (BufferRead, 0, -1, "PASS", 1, 0) > 0)	
			*result=1;
			//sscanf (BufferRead, "%s,%s ,%fKV,%f mA ,T=%fS",mode, result, &voltage_kv, &current_mA, &resistance);
			index=FindPattern (BufferRead, 0, -1, "mA", 1, 0);
			CopyBytes(charCurrent,0,BufferRead,index-6,5);
			CopyBytes(charVoltage,0,BufferRead,index-14,5);
			*current_mA=atof(charCurrent);
			*voltage_kv=atof(charVoltage);
			
			//current_mA=atof(charCurrent);
		}
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{RSPRO}::SystemError"
SElExceptionPtr SystemError(SDielectrimeterPtr me, char *error)
{
	SElExceptionPtr	pexception = NULL;
	char command[128];
	char BufferRead[128];
	char sAmmountLabel[128];
	int  BitRead;

	/*retrun system error */
	sprintf (command, "SYST:ERR ?\n");
	ComWrt (me->port, command, strlen (command));
	Delay (0.5);
	BitRead = GetInQLen (me->port);
	ComRd (me->port, error, BitRead);
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{RSPRO}::ClearError"
SElExceptionPtr ClearError(SDielectrimeterPtr me)
{
	SElExceptionPtr	pexception = NULL;
	char command[128];
	char BufferRead[128];
	char sAmmountLabel[128];
	int  BitRead;

	/*retrun system error */
	sprintf (command, "*CLS\n");
	ComWrt (me->port, command, strlen (command));
	Delay (0.5);
	
Error:
	EXCRETHROW( pexception);
}

#pragma warning( pop)
