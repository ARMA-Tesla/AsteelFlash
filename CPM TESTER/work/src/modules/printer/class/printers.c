#include "printers.h"
#include <string.h>
#include <stdio.h>
#include <cviauto.h>
#include "msxmldom.h"
#include <utility.h>

#define DISCARD_HANDLE(handle) \
	if (handle) \
	{ \
		CA_DiscardObjHandle(handle); \
		handle = 0; \
	}

#define CA_FREE_MEMORY(mem) \
	if (mem) \
	{ \
		CA_FreeMemory(mem); \
		mem = NULL; \
	}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

static SElExceptionPtr printer_open(
		struct _Printer*	me,
		const char*			resource,
		const char*			optionString
);
static SElExceptionPtr printer_close(struct _Printer* me);
static SElExceptionPtr printer_connect(struct _Printer* me);
static SElExceptionPtr printer_disconnect(struct _Printer* me);
static SElExceptionPtr printer_write(SPrinterPtr me, const char* command);
static SElExceptionPtr printer_reset(SPrinterPtr me);
static SElExceptionPtr printer_status(SPrinterPtr me, int *StatusOnline, char *Error, int *iAmmountLabels, int *Interpreter);

DWORD WINAPI Thread_Printer(
  LPVOID lpParameter
);

/*****************************************************************************/

static SElExceptionPtr printers_GetPrinterByName(
	struct _Printers*	me,
	const char*			printerName,
	SPrinterPtr*		printer
);
static SElExceptionPtr printers_GetPrinterByIndex(
	struct _Printers*	me,
	int32_t				index,
	SPrinterPtr*		printer
);
static SElExceptionPtr printers_GetCurrentPrinter(
	struct _Printers*	me,
	SPrinterPtr*		printer
);
static SElExceptionPtr printers_SetCurrentPrinterByName(
	struct _Printers*	me,
	const char*			printerName
);
static SElExceptionPtr printers_GetFirstPrinter(
	struct _Printers*	me,
	SPrinterPtr*		printer
);
static SElExceptionPtr printers_GetNextPrinter(
	struct _Printers*	me,
	SPrinterPtr*		printer
);
	
static SElExceptionPtr printers_InitPrinters(struct _Printers* me);
static SElExceptionPtr printers_ClosePrinters(struct _Printers* me);

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "printer_new"
SElExceptionPtr printer_new(
	SPrinterPtr*	printer,
	const char*		name,
	const char*		optionString
)
{
	SElExceptionPtr	pexception = NULL;
	SPrinterPtr		pnew_printer = NULL;
	
	pnew_printer = calloc(1, sizeof(SPrinter));
	EXCCHECKALLOC( pnew_printer);
	InitializeCriticalSection(&(pnew_printer->lock));
	
	pnew_printer->printerName = calloc(strlen(name) + 1, sizeof(char));
	EXCCHECKALLOC( pnew_printer->printerName);
	strcpy(pnew_printer->printerName, name);
	
	pnew_printer->Open		= printer_open;
	pnew_printer->Close		= printer_close;
	pnew_printer->Connect	= printer_connect;
	pnew_printer->Disconnect= printer_disconnect;
	pnew_printer->Write		= printer_write;
	pnew_printer->Reset		= printer_reset;
	pnew_printer->Status	= printer_status;

	if (printer)
		*printer = pnew_printer;
	
Error:
	if (pexception && pnew_printer)
		free(pnew_printer);
	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "printer_delete"
SElExceptionPtr printer_delete(SPrinterPtr* printer)
{
	SElExceptionPtr	pexception = NULL;
	
	if (printer && *printer)
	{
		(*printer)->Close(*printer);
		
		if ((*printer)->printerName)
			free((*printer)->printerName);
		
		if ((*printer)->optionString)
			free((*printer)->optionString);
		
		DeleteCriticalSection( &((*printer)->lock) );
		
		free(*printer);
		*printer = NULL;
	}
	
Error:
	EXCRETHROW( pexception);
}

/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "printers_new"
SElExceptionPtr printers_new(SPrintersPtr*	printers)
{
	SElExceptionPtr	pexception = NULL;
	SPrintersPtr	pnew_printers = NULL;
	
	pnew_printers = calloc(1, sizeof(SPrinters));
	EXCCHECKALLOC( pnew_printers);
	
	pnew_printers->printers = NULL;
	pnew_printers->numPrinters = 0;
	pnew_printers->currentIndex = -1;
	
	pnew_printers->GetPrinterByName			= printers_GetPrinterByName;
	pnew_printers->GetPrinterByIndex		= printers_GetPrinterByIndex;
	pnew_printers->GetCurrentPrinter		= printers_GetCurrentPrinter;
	pnew_printers->SetCurrentPrinterByName	= printers_SetCurrentPrinterByName;
	pnew_printers->GetFirstPrinter			= printers_GetFirstPrinter;
	pnew_printers->GetNextPrinter			= printers_GetNextPrinter;
	pnew_printers->InitPrinters				= printers_InitPrinters;
	pnew_printers->ClosePrinters			= printers_ClosePrinters;
	
	if (printers)
		*printers = pnew_printers;
	
Error:
	if (pexception && pnew_printers)
		free(pnew_printers);
	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "printers_delete"
SElExceptionPtr printers_delete(SPrintersPtr* printers)
{
	SElExceptionPtr	pexception = NULL;
	
	if (printers && *printers)
	{
		EXCCHECK( (*printers)->ClosePrinters(*printers));
		
		free(*printers);
		*printers = NULL;
	}
	
Error:
	EXCRETHROW( pexception);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "{Printer}::Open"
static SElExceptionPtr printer_open(
		struct _Printer*	me,
		const char*			resource,
		const char*			optionString
)
{
	SElExceptionPtr	pexception = NULL;
	HRESULT			hr = S_OK;
	VARIANT_BOOL	is_xml_success = 1;
	CAObjHandle		document = 0;
	CAObjHandle		xml_configuration = 0;
	CAObjHandle		xml_node = 0;
	char*			xml_type = NULL;
	char*			xml_value = NULL;
	char			message[1024] = "";
	
	
	me->optionString = calloc(strlen(optionString) + 1, sizeof(char));
	EXCCHECKALLOC( me->optionString);
	strcpy(me->optionString, optionString);
	
	EXCCHECKCOM( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &document));
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentloadXML (document, NULL, optionString, &is_xml_success));
	
	EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode(document, NULL,
													   "/Printer/Configuration",
													   &xml_configuration));
	if (xml_configuration == 0)
		EXCTHROW( -1, "Missing Configuration Node");
	
	/* Load Specific DLL functions */
	EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(xml_configuration, NULL, "@specificDll", &xml_node));
	if (xml_node == 0)
		EXCTHROW( -3, "Missing Printer Specific Library");
	
	EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(xml_node, NULL, &xml_value));
	me->specLib = LoadLibrary(xml_value);
	
	if (me->specLib == 0)
	{
		sprintf(message, "Printer Library (%s) Loading Failed", xml_value);
		EXCTHROW( -4, message);
	}
	
	me->_ConnectSpec	= (SElExceptionPtr (*)(SPrinterPtr))
								GetProcAddress(me->specLib, "Connect");
	me->_DisconnectSpec	= (SElExceptionPtr (*)(SPrinterPtr))
								GetProcAddress(me->specLib, "Disconnect");
	me->_WriteSpec		= (SElExceptionPtr (*)(SPrinterPtr,const char*))
								GetProcAddress(me->specLib, "Write");
   	me->_ResetSpec		= (SElExceptionPtr (*)(SPrinterPtr))
								GetProcAddress(me->specLib, "Reset");
	me->_StatusSpec		= (SElExceptionPtr (*)(SPrinterPtr, int *, char*, int *, int *))
								GetProcAddress(me->specLib, "Status");
	CA_FREE_MEMORY(xml_value);
	DISCARD_HANDLE(xml_node);
	
	/* Get Type of communication */
	EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(xml_configuration, NULL,
												   "@type", &xml_node));
	if (xml_node == 0)
		EXCTHROW( -2, "Missing Configuration Type");

	EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(xml_node, NULL, &xml_type));
	DISCARD_HANDLE(xml_node);
	
	if (!stricmp(xml_type, "Serial"))
	{
		
		CA_FREE_MEMORY(xml_value);
		DISCARD_HANDLE(xml_node);
		
		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(xml_configuration, NULL,
													   "port", &xml_node));
		if (xml_node == 0)
			EXCTHROW( -5, "Missing port printer name");
	
		if (xml_node)
		{
			EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(xml_node, NULL, &xml_value));
			me->port = atoi(xml_value);
			CA_FREE_MEMORY(xml_value);
		}
		
		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(xml_configuration, NULL,
													   "BaudRate", &xml_node));
		if (xml_node)
		{
			EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(xml_node, NULL, &xml_value));
			me->baudRate = atol(xml_value);
			CA_FREE_MEMORY(xml_value);
		}
		
		me->_ConnectSpec(me);
		
		DISCARD_HANDLE(xml_node);
	}
		
	else
	{
		EXCTHROW( -10, "Unsupported Configuration Type");
	}


Error:

	if (pexception)
	{
		if (me->specLib)
		{
			FreeLibrary(me->specLib);
			me->specLib = 0;
		}
		
		me->_ConnectSpec	= NULL;
		me->_DisconnectSpec	= NULL;
		me->_WriteSpec		= NULL;
		me->_ResetSpec		= NULL;
		me->_StatusSpec		= NULL;
		me->_SendCommandSpec= NULL;
		
	}
		
	DISCARD_HANDLE(xml_node);
	CA_FREE_MEMORY(xml_value);
	CA_FREE_MEMORY(xml_type);
	DISCARD_HANDLE(xml_configuration);
	DISCARD_HANDLE(document);
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Printer}::Close"
static SElExceptionPtr printer_close(struct _Printer* me)
{
	SElExceptionPtr	pexception = NULL;
	
		me->_DisconnectSpec (me);
		
		me->_ConnectSpec	= NULL;
		me->_DisconnectSpec	= NULL;
		me->_WriteSpec		= NULL;
		me->_ResetSpec		= NULL;
		me->_StatusSpec		= NULL;
	
Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "fncLock"
static SElExceptionPtr printerLock(SPrinterPtr me)
{
	EnterCriticalSection( &me->lock);    
	return NULL;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "printerUnLock"
static SElExceptionPtr printerUnLock(SPrinterPtr me)
{
	LeaveCriticalSection( &me->lock);   
	return NULL;
}

#undef __FUNC__
#define __FUNC__ "{Printer}::Connect"
static SElExceptionPtr printer_connect(SPrinterPtr me)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_ConnectSpec)
		EXCCHECK( me->_ConnectSpec(me));
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Printer}::Disconnect"
static SElExceptionPtr printer_disconnect(SPrinterPtr me)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_ConnectSpec)
		EXCCHECK( me->_DisconnectSpec(me));
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Printer}::Write"
static SElExceptionPtr printer_write(SPrinterPtr me, const char* command)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_WriteSpec)
		EXCCHECK( me->_WriteSpec(me, command));
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Printer}::Reset"
static SElExceptionPtr printer_reset(SPrinterPtr me)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_ResetSpec)
		EXCCHECK( me->_ResetSpec(me));
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Printer}::Status"
static SElExceptionPtr printer_status(SPrinterPtr me, int *StatusOnline, char *Error, int *iAmmountLabels, int *Interpreter)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_StatusSpec)
		EXCCHECK( me->_StatusSpec(me, StatusOnline, Error, iAmmountLabels, Interpreter));
	
Error:
	EXCRETHROW( pexception);
}

/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "{Printers}::GetPrinterByName"
static SElExceptionPtr printers_GetPrinterByName(
	struct _Printers*	me,
	const char*			printerName,
	SPrinterPtr*		printer
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			i = 0;
	SPrinterPtr		found_printer = NULL;
	char			message[1024] = "";
	
	for (i = 0; i < me->numPrinters; i++)
	{
		if (!strcmp(me->printers[i]->printerName, printerName))
		{
			found_printer = me->printers[i];
			break;
		}
	}
	
	if (printer)
		*printer = found_printer;
	
	if (found_printer == NULL)
	{
		sprintf(message, "Printer Name (%s) Not Found", printerName);
		EXCTHROW( -12, message);
	}
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Printers}::GetPrinterByIndex"
static SElExceptionPtr printers_GetPrinterByIndex(
	struct _Printers*	me,
	int32_t				index,
	SPrinterPtr*		printer
)
{
	SElExceptionPtr	pexception = NULL;
	
	if (index < me->numPrinters && index >= 0)
	{
		if (printer)
			*printer = me->printers[index];
	}
	else
	{
		if (printer)
			*printer = NULL;
		
		EXCTHROW( -13, "Index Out of Range");
	}
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Printers}::GetCurrentPrinter"
static SElExceptionPtr printers_GetCurrentPrinter(
	struct _Printers*	me,
	SPrinterPtr*		printer
)
{
	SElExceptionPtr	pexception = NULL;
	
	if (me->currentIndex >= 0 && me->numPrinters > 0)
	{
		if (printer)
			*printer = me->printers[me->currentIndex];
	}
	else
	{
		if (printer)
			*printer = NULL;
	}

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Printers}::SetCurrentPrinterByName"
static SElExceptionPtr printers_SetCurrentPrinterByName(
	struct _Printers*	me,
	const char*			printerName
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			i = 0;
	SPrinterPtr		printer = NULL;
	char			message[1024] = "";

	for (i = 0; i < me->numPrinters; i++)
	{
		if (!strcmp(me->printers[i]->printerName, printerName))
		{
			me->currentIndex = i;
			break;
		}
	}
	
	if (i >= me->numPrinters)
	{
		sprintf(message, "Printer Name (%s) Not Found", printerName);
		EXCTHROW( -12, message);
	}

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Printers}::GetFirstPrinter"
static SElExceptionPtr printers_GetFirstPrinter(
	struct _Printers*	me,
	SPrinterPtr*		printer
)
{
	SElExceptionPtr	pexception = NULL;
	
	me->currentIndex = 0;

	if(me->numPrinters > 0)
	{
		if (printer)
			*printer = me->printers[0];
	}
	else
	{
		if (printer)
			*printer = NULL;
	}

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Printers}::GetNextPrinter"
static SElExceptionPtr printers_GetNextPrinter(
	struct _Printers*	me,
	SPrinterPtr*		printer
)
{
	SElExceptionPtr	pexception = NULL;

	if (++(me->currentIndex) >= me->numPrinters)
	{
		me->currentIndex = me->numPrinters - 1;
		if (printer)
			*printer = NULL;
	}
	else
	{
		if (printer)
			*printer = me->printers[me->currentIndex];
	}
	
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Printers}::InitPrinters"
static SElExceptionPtr printers_InitPrinters(struct _Printers* me)
{
	SElExceptionPtr	pexception = NULL;
	HRESULT			hr = S_OK;
	VARIANT_BOOL	is_xml_success = 1;
	VARIANT			file;
	CAObjHandle		document = 0;
	CAObjHandle		printer = 0;
	CAObjHandle		printer_name_node = 0;
	CAObjHandle		printer_list = 0;
	char*			xml_value = NULL;
	char*			printer_xml = NULL;
	
	EXCCHECKCOM( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &document));
	
	CA_VariantSetCString (&file, "modules/printers.xml");
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentload(document, NULL, file, &is_xml_success));
	
	EXCCHECKCOM( MSXML_IXMLDOMDocumentselectNodes(document, NULL, "/Printers/Printer", &printer_list));
	EXCCHECKCOM( MSXML_IXMLDOMNodeListreset (printer_list, NULL));
	
	EXCCHECKCOM( MSXML_IXMLDOMNodeListnextNode(printer_list, NULL, &printer));
	while (printer)
	{
		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(printer, NULL, "@name", &printer_name_node));
		EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(printer_name_node, NULL, &xml_value));
		EXCCHECKCOM( MSXML_IXMLDOMNodeGetxml(printer, NULL, &printer_xml));
		
		me->printers = realloc( me->printers, (me->numPrinters + 1) * sizeof(SPrinterPtr));
		EXCCHECKALLOC( me->printers);
		
		EXCCHECK( printer_new( &me->printers[me->numPrinters], xml_value, NULL));
		EXCCHECK( me->printers[me->numPrinters]->Open(me->printers[me->numPrinters], NULL, printer_xml));
		me->numPrinters++;
		
		CA_FREE_MEMORY(xml_value);
		CA_FREE_MEMORY(printer_xml);
		DISCARD_HANDLE(printer_name_node);
		DISCARD_HANDLE(printer);
		
		EXCCHECKCOM( MSXML_IXMLDOMNodeListnextNode(printer_list, NULL, &printer));
	}
	
Error:
	CA_FREE_MEMORY(xml_value);
	CA_FREE_MEMORY(printer_xml);
	DISCARD_HANDLE(printer_name_node);
	DISCARD_HANDLE(printer);
	DISCARD_HANDLE(printer_list);
	DISCARD_HANDLE(document);
	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Printers}::ClosePrinters"
static SElExceptionPtr printers_ClosePrinters(struct _Printers* me)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			i = 0;
	
	for (i = 0; i < me->numPrinters; i++)
	{
		EXCCHECK( printer_delete(&(me->printers[i])));
	}
	free(me->printers);
	me->printers = NULL;
	me->numPrinters = 0;
	
Error:
	EXCRETHROW( pexception);
}
