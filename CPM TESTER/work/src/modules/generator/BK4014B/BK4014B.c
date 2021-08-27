#include <rs232.h>
#include <cvintwrk.h>
#include <userint.h>
#include <tcpsupp.h>
#include "BK4014B.h"
#include <stdio.h>
#include <formatio.h>
#include <utility.h>

#pragma warning( push)
#pragma warning( disable: 4100 4127)

#define GENERATOR_TIMEOUT 	5000

#undef __FUNC__
#define __FUNC__	"{BK4014B}::Connect"
SElExceptionPtr Connect(SGeneratorPtr me)
{

SElExceptionPtr	pexception = NULL;

	if (OpenComConfig (me->port, "COM9", me->baudRate, 0, 8, 1, 512, 512) < 0) //6 //9600
		EXCTHROW( GENERATOR_CONNECTION_FAILED, "Probléme Ouverture COM");

Error:
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__	"{BK4014B}::Disconnect"
SElExceptionPtr Disconnect(SGeneratorPtr me)
{

SElExceptionPtr	pexception = NULL;
CloseCom (me->port);

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{BK4014B}::Write"
SElExceptionPtr Write(SGeneratorPtr me,const char* command)
{
	SElExceptionPtr	pexception = NULL;

	ComWrt (me->port, command, strlen (command));

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{BK4014B}::Read"
SElExceptionPtr Read(SGeneratorPtr me,const char* read)
{
	SElExceptionPtr	pexception = NULL;
	int  BitRead;
	
	BitRead = GetInQLen (me->port);
	ComRd (me->port, read, BitRead);

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{BK4014B}::SetModeContinuous"
SElExceptionPtr SetModeContinuous(SGeneratorPtr me)
{
	SElExceptionPtr	pexception = NULL;
	char command[128];
	char BufferRead[128];
	char sAmmountLabel[128];
	int  BitRead;

	/*Puts the tester mode to CONT.*/
	sprintf (command, "MODE CONT\n");
	ComWrt (me->port, command, strlen (command));
	Delay (0.1);
		
	/*Request Mode CONT*/
	FlushInQ (me->port);
	sprintf (command, "MODE?\n");
	ComWrt (me->port, command, strlen (command));
	Delay (0.1);
	BitRead = GetInQLen (me->port);
	ComRd (me->port, BufferRead, BitRead);
	if (FindPattern (BufferRead, 0, -1, "CONT", 0, 0) < 0)
	{
	    ComWrt (me->port, command, strlen (command));
		Delay (0.1);
	    BitRead = GetInQLen (me->port);
	    ComRd (me->port, BufferRead, BitRead);
	}	
	if (FindPattern (BufferRead, 0, -1, "CONT", 0, 0) < 0)
		EXCTHROW( GENERATOR_READ_STATUS_FAILED, "SetModeContinuous Failed");	
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{BK4014B}::SetOutput"
SElExceptionPtr SetOutput(SGeneratorPtr me, int output)
{
	SElExceptionPtr	pexception = NULL;
	char command[128];
	char BufferRead[128];
	char sAmmountLabel[128];
	int  BitRead;

	/*Turns the currently selected test (output) on or off.*/
	if (output == 1)
	sprintf (command, "OUT ON\n");
	else 	
	sprintf (command, "OUT OFF\n");
	ComWrt (me->port, command, strlen (command));
	Delay (0.2);	
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{BK4014B}::SetFonction"
SElExceptionPtr SetFonction(SGeneratorPtr me, double frequency, double amplitude, double offset, double duty)
{
	SElExceptionPtr	pexception = NULL;
	char command[128];
	char BufferRead[128];
	char sAmmountLabel[128];
	int  BitRead;
	double frequency_read =0.0;
	double amplitude_read =0.0;
	double offset_read =0.0;
	double duty_read =0.0;	
		
	/* Set Output Fonction type  */
	sprintf (command, "FUNC SQU\n");
	ComWrt (me->port, command, strlen (command));
	Delay (0.1);

	/*Request Output Fonction type*/
	FlushInQ (me->port);
	sprintf (command, "FUNC?\n");
	ComWrt (me->port, command, strlen (command));
	Delay (0.1);
	BitRead = GetInQLen (me->port);
	ComRd (me->port, BufferRead, BitRead);
	if (FindPattern (BufferRead, 0, -1, "SQU", 0, 0) < 0)
	{	ComWrt (me->port, command, strlen (command));
	    Delay (0.1);
	    BitRead = GetInQLen (me->port);
	    ComRd (me->port, BufferRead, BitRead);
	}
	if (FindPattern (BufferRead, 0, -1, "SQU", 0, 0) < 0)
		EXCTHROW( GENERATOR_READ_STATUS_FAILED, "SetFonction : Set Output Fonction type Failed");	
	
		
	/* Fequency set  */
	sprintf (command, "*FREQ %fHZ\n", frequency);
	ComWrt (me->port, command, strlen (command));
	Delay (0.1);
	
	/* Request frequency value */
	FlushInQ (me->port);
	sprintf (command, "FREQ?\n");
	ComWrt (me->port, command, strlen (command));
	BitRead = GetInQLen (me->port);
	ComRd (me->port, BufferRead, BitRead);
	frequency_read = atof(BufferRead);
	if (frequency_read > frequency + 0.1 || frequency_read < frequency - 0.1)
	{
		ComWrt (me->port, command, strlen (command));
		Delay (0.1);
		BitRead = GetInQLen (me->port);
		ComRd (me->port, BufferRead, BitRead);
		frequency_read = atof(BufferRead);
	}
	if (frequency_read > frequency + 0.1 || frequency_read < frequency - 0.1)
		EXCTHROW( GENERATOR_READ_STATUS_FAILED, "SetFonction : Fequency set Failed");	
	
	
	/* amplitude set  */
	sprintf (command, "*AMPL %fV\n", amplitude);
	ComWrt (me->port, command, strlen (command));
	Delay (0.1);
	
	/* Request amplitude value */
	FlushInQ (me->port);
	sprintf (command, "AMPL?\n");
	ComWrt (me->port, command, strlen (command));
	Delay (0.1);
	BitRead = GetInQLen (me->port);
	ComRd (me->port, BufferRead, BitRead);
	amplitude_read = atof(BufferRead);
	if (amplitude_read > amplitude + 0.05 || amplitude_read < amplitude - 0.05)
	{
		ComWrt (me->port, command, strlen (command));
		Delay (0.1);
		BitRead = GetInQLen (me->port);
		ComRd (me->port, BufferRead, BitRead);
		amplitude_read = atof(BufferRead);
		
	}
	if (amplitude_read > amplitude + 0.05 || amplitude_read < amplitude - 0.05)
		EXCTHROW( GENERATOR_READ_STATUS_FAILED, "SetFonction : amplitude set Failed");
	
	/* offset set  */
	sprintf (command, "*OFFS %fV\n", offset);
	ComWrt (me->port, command, strlen (command));
	Delay (0.1);
	
	/* Request offset value */
	FlushInQ (me->port);
	sprintf (command, "OFFS?\n");
	ComWrt (me->port, command, strlen (command));
	Delay (0.1);
	BitRead = GetInQLen (me->port);
	ComRd (me->port, BufferRead, BitRead);
	offset_read = atof(BufferRead);
	if (offset_read > offset + 0.005 || offset_read < offset - 0.005)
	{		
		ComWrt (me->port, command, strlen (command));
		Delay (0.1);
		BitRead = GetInQLen (me->port);
		ComRd (me->port, BufferRead, BitRead);
		amplitude_read = atof(BufferRead);
	}	
	if (offset_read > offset + 0.005 || offset_read < offset - 0.005)
		EXCTHROW( GENERATOR_READ_STATUS_FAILED, "SetFonction : offset set Failed");

		
	/* duty cycle set  */
	sprintf (command, "DCYCle %f\n", duty);
	ComWrt (me->port, command, strlen (command));
	Delay (0.1);
	/* Request duty cycle value */
	FlushInQ (me->port);
	sprintf (command, "DCYCle?\n");
	ComWrt (me->port, command, strlen (command));
	Delay (0.1);
	BitRead = GetInQLen (me->port);
	ComRd (me->port, BufferRead, BitRead);
	duty_read = atof(BufferRead);
	if (duty_read > duty + 0.05 || duty_read < duty - 0.05)
	{
		ComWrt (me->port, command, strlen (command));
		Delay (0.1);
		BitRead = GetInQLen (me->port);
		ComRd (me->port, BufferRead, BitRead);
		amplitude_read = atof(BufferRead);
	}
	if (duty_read > duty + 0.05 || duty_read < duty - 0.05)
		EXCTHROW( GENERATOR_READ_STATUS_FAILED, "SetFonction : duty cycle set Failed");
	
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{BK4014B}::SystemError"
SElExceptionPtr SystemError(SGeneratorPtr me, char *error)
{
	SElExceptionPtr	pexception = NULL;
	char command[128];
	char BufferRead[128];
	char sAmmountLabel[128];
	int  BitRead;

	/*retrun system error */
	sprintf (command, "ERR?\n");
	ComWrt (me->port, command, strlen (command));
	Delay (0.1);
	BitRead = GetInQLen (me->port);
	ComRd (me->port, error, BitRead);
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{BK4014B}::ClearError"
SElExceptionPtr ClearError(SGeneratorPtr me)
{
	SElExceptionPtr	pexception = NULL;
	char command[128];
	char BufferRead[128];
	char sAmmountLabel[128];
	int  BitRead;

	/*retrun system error */
	sprintf (command, "RST\n");
	ComWrt (me->port, command, strlen (command));
	Delay (0.1);
	
Error:
	EXCRETHROW( pexception);
}

#pragma warning( pop)
