#include <rs232.h>
#include <cvintwrk.h>
#include <userint.h>
#include <tcpsupp.h>
#include "CABPrinter.h"
#include <stdio.h>
#include <formatio.h>
#include <utility.h>

#pragma warning( push)
#pragma warning( disable: 4100 4127)

#define PRINTER_TIMEOUT 	5000

#undef __FUNC__
#define __FUNC__	"{CABPrinter}::Connect"
SElExceptionPtr Connect(SPrinterPtr me)
{

SElExceptionPtr	pexception = NULL;

	if (OpenComConfig (me->port, "", me->baudRate, 0, 8, 1, 512, 512) < 0) 
		EXCTHROW( PRINTER_CONNECTION_FAILED, "Probléme Ouverture COM");

Error:
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__	"{CABPrinter}::Disconnect"
SElExceptionPtr Disconnect(SPrinterPtr me)
{

SElExceptionPtr	pexception = NULL;
CloseCom (me->port);

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{CABPrinter}::Write"
SElExceptionPtr Write(SPrinterPtr me,const char* command)
{
	SElExceptionPtr	pexception = NULL;

	ComWrt (me->port, command, strlen (command));

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{CABPrinter}::Reset"
SElExceptionPtr Reset(SPrinterPtr me)
{
	SElExceptionPtr	pexception = NULL;
	char command[128];

	sprintf (command, "%ct", 27);
	ComWrt (me->port, command, strlen (command));
	Delay (0.5);
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{CABPrinter}::Status"
SElExceptionPtr Status(SPrinterPtr me, int *StatusOnline, char *Error, int *iAmmountLabels, int *StatusInterpreter)
{
	SElExceptionPtr	pexception = NULL;
	char command[128];
	char BufferRead[128];
	char sAmmountLabel[128];
	int  BitRead;
	;
	
	
	sprintf (command, "%cs", 27);
	ComWrt (me->port, command, strlen (command));
	Delay (0.7);
	BitRead = GetInQLen (me->port);
	//printf("BitRead =%d\n",BitRead);
	if(BitRead == 9)
	{
		ComRd (me->port, BufferRead, BitRead);
		//printf("BufferRead =%s\n",BufferRead);
		BufferRead[BitRead] = 0X00;
		//sprintf (message, "STATUS CAB [%s]", BufferRead);
		
		/* Online (Y=Yes, N=No) */
		if (BufferRead[0] == 'N')
			*StatusOnline	 = 0;
		else if (BufferRead[0] == 'Y')
			*StatusOnline	 = 1;
		
		/* Type of error */
		switch (BufferRead[1])
		{
		case '-' :  sprintf (Error, "No error"); break;
		case 'o' :  sprintf (Error, "Compressed air-error"); break;
		case 'B' :  sprintf (Error, "Protocol error/ invalid barcode data");break;
		default :  sprintf (Error, "Unknown Error");
		}
	
		/* amount of labels to print */
		CopyBytes (sAmmountLabel, 0, BufferRead, 2, 6);
		*iAmmountLabels = atoi(sAmmountLabel);
		
			
		/* Interpreter active (Y=Yes = print job is in process, N=No= printer in Standby mode) */
		if (BufferRead[8] == 'N')
			*StatusInterpreter = 0;
		else if (BufferRead[8] == 'Y')
			*StatusInterpreter = 1;
		
	}
	
	else 
	{
		*StatusOnline	 = -1;
		sprintf (Error, "Unknown Error");
		*iAmmountLabels = -1;
		*StatusInterpreter = -1;
		EXCTHROW( PRINTER_READ_STATUS_FAILED, "Printer Read Status Failed");	
	}
	
Error:
	EXCRETHROW( pexception);
}


#pragma warning( pop)
