#ifndef _PRINTERS_H_
#define _PRINTERS_H_

#include <windows.h>
#include <exception/Exception.h>

#define MAX_PRINTER_NAME			128
#define MAX_BUFFER_SIZE				20

#define PRINTER_CONNECTION_FAILED				-1
#define PRINTER_SEND_FAILED						-1
#define PRINTER_RESET_FAILED					-1
#define PRINTER_READ_STATUS_FAILED				-1



 
typedef struct _Printer
{
	char*				printerName;
	char*				optionString;
	WORD				port;
	WORD				baudRate;
	CRITICAL_SECTION	lock;
	int32_t				status;
	int32_t				readMode;
	char				readBuffer[MAX_BUFFER_SIZE];
	HANDLE				specLib;

	
	SElExceptionPtr (*Open)(struct _Printer* me,const char*	resource, const char* optionString);
	SElExceptionPtr (*Close)(struct _Printer* me);
	SElExceptionPtr (*Connect)(struct _Printer* me);
	SElExceptionPtr (*Disconnect)(struct _Printer* me);
	SElExceptionPtr (*Write)(struct _Printer* me, const char* command);
	SElExceptionPtr (*Reset)(struct _Printer* me);
	SElExceptionPtr (*Status)(struct _Printer* me, int *StatusOnline, char *Error, int *iAmmountLabels, int *StatusInterpreter);

	/* Private */
	SElExceptionPtr (*_ConnectSpec)(struct _Printer* me);
	SElExceptionPtr (*_DisconnectSpec)(struct _Printer* me);
	SElExceptionPtr (*_WriteSpec)(struct _Printer* me, const char* command);
	SElExceptionPtr (*_ResetSpec)(struct _Printer* me);
	SElExceptionPtr (*_StatusSpec)(struct _Printer* me, int *StatusOnline, char *Error, int *iAmmountLabels, int *StatusInterpreter);
	SElExceptionPtr (*_SendCommandSpec)(struct _Printer* me, const char* commandName);
	
} SPrinter, *SPrinterPtr;


typedef struct _Printers
{
	SPrinterPtr*	printers;
	int32_t			numPrinters;
	int32_t			currentIndex;
	
	SElExceptionPtr (*GetPrinterByName)(
		struct _Printers*	me,
		const char*			printerName,
		SPrinterPtr*		printer
	);
	SElExceptionPtr (*GetPrinterByIndex)(
		struct _Printers*	me,
		int32_t				index,
		SPrinterPtr*		printer
	);
	SElExceptionPtr (*GetCurrentPrinter)(
		struct _Printers*	me,
		SPrinterPtr*		printer
	);
	SElExceptionPtr (*SetCurrentPrinterByName)(
		struct _Printers*	me,
		const char*			printerName
	);
	SElExceptionPtr (*GetFirstPrinter)(
		struct _Printers*	me,
		SPrinterPtr*		printer
	);
	SElExceptionPtr (*GetNextPrinter)(
		struct _Printers*	me,
		SPrinterPtr*		printer
	);
	
	SElExceptionPtr (*InitPrinters)(struct _Printers* me);
	SElExceptionPtr (*ClosePrinters)(struct _Printers* me);
	
} SPrinters, *SPrintersPtr;

SElExceptionPtr printers_new(SPrintersPtr* printers);
SElExceptionPtr printers_delete(SPrintersPtr* printers);

SElExceptionPtr printer_new(
	SPrinterPtr*	printer,
	const char*		name,
	const char*		optionString
);
SElExceptionPtr printer_delete(SPrinterPtr* printer);

#endif /* _PRINTERS_H_ */
