#include "msxmldom.h"
#include <tester/barCodeReader/BarCodeReader.h>
#include <cviauto.h>
#include <userint.h>
#include <stdio.h>
#include <stdlib.h>
#include <multithread/multithread.h>
#include <ansi_c.h>
#include <formatio.h>
#include <rs232.h>
#pragma warning( push )
#pragma warning(disable: 4100 4127)

#define LF      "\x0A"        // Line feed
#define CR      "\x0D"        // Carriage return
#define ESC     "\x1B"        // Escape character


#define BARCODE_TIMEOUT		2000


static SElExceptionPtr bcr_Initialize
(
	struct _BarCodeReader*	me,
	const char*				configString
);
static SElExceptionPtr bcr_Close(struct _BarCodeReader* me);
static SElExceptionPtr bcr_StartReading(struct _BarCodeReader* me);
static SElExceptionPtr bcr_StopReading(struct _BarCodeReader* me);
static SElExceptionPtr bcr_GetBarCode
(
	struct _BarCodeReader*	me,
	char*					buffer,
	int32_t					bufferSize
);

int __stdcall DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
		{
		case DLL_PROCESS_ATTACH:
#if (!defined(_CVI_)&&defined(_DEBUG))
			//_CrtSetBreakAlloc(78);
#endif
			if (InitCVIRTE (hinstDLL, 0, 0) == 0)
				return 0;	  /* out of memory */
			break;
		case DLL_PROCESS_DETACH:
			CloseCVIRTE ();   
#if !defined(_CVI_) && defined(_DEBUG) && defined(DUMP_MEMORY_LEAKS)
			_CrtDumpMemoryLeaks();
#endif
			break;
		}
	
	return 1;
}

#undef __FUNC__
#define __FUNC__	"bcr_new"
SElExceptionPtr	bcr_new( SBarCodeReaderPtr* pBcr)
{
	SElExceptionPtr		pexception = NULL;
	SBarCodeReaderPtr	pbcr = NULL;
	
	pbcr = calloc(1, sizeof(SBarCodeReader));
	EXCCHECKALLOC( pbcr);
	
	pbcr->Initialize	= bcr_Initialize;
	pbcr->Close			= bcr_Close;
	pbcr->StartReading	= bcr_StartReading;
	pbcr->StopReading	= bcr_StopReading;
	pbcr->GetBarCode	= bcr_GetBarCode;
	
	InitializeCriticalSection(&pbcr->lock);
	
Error:
	if (pBcr)
		*pBcr = pbcr;
	
	EXCRETHROW(pexception);
}

#undef __FUNC__
#define __FUNC__	"bcr_delete"
SElExceptionPtr	bcr_delete( SBarCodeReaderPtr* pBcr)
{
	SElExceptionPtr	pexception = NULL;
	
	if (pBcr && *pBcr)
	{
		EXCCHECK((*pBcr)->Close(*pBcr));
		
		if ((*pBcr)->idBCR != NULL)
		{
			free((*pBcr)->idBCR);
			(*pBcr)->idBCR = NULL;
		}
		
		DeleteCriticalSection(&((*pBcr)->lock));
		
		free(*pBcr);
		*pBcr = NULL;
	}
	
Error:
	EXCRETHROW(pexception);
}


/**
 *	This function initialize bar code reader.
 *
 *  @param me - pointer to self
 *  @param configString - XML string with configuration
 *                        <BarCodeReader id="1">
 *           		        <Driver name="SickCLP100.dll" />
 *           		        <Configuration>
 *           			      <Communication type="serial">
 *           				    <Port>COM1</Port>
 *							    <Baudrate>9600</Baudrate>
 *           				    <DataBits>8</DataBits>
 *           				    <StopBits>1.0</StopBits>
 *           				    <Parity>None</Parity>
 *           				    <Handshake>None</Handshake>
 *           			      </Communication>
 *           			      <Settings>
 *           				    <CommandString>'Settings commands'</CommandString>
 *           			      </Settings>
 *           		        </Configuration>
 *           	          </BarCodeReader>
 */
#undef __FUNC__
#define __FUNC__	"BCR::Initialize"
static SElExceptionPtr bcr_Initialize
(
	struct _BarCodeReader*	me,
	const char*				configString
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0,
					size = 0;
	char			message[256] = "";
	char*			init_string = NULL;
	char*			id_str = NULL;
	char*			type = NULL;
	char*			port = NULL;
	char*			tmp = NULL;
	DCB				dcb = {0};
	HRESULT			hr = S_OK;
	VBOOL			is_xml_success = 1;
	CAObjHandle		xml_doc = 0;
	CAObjHandle		pid = 0;
	CAObjHandle		presource = 0;
	CAObjHandle		ptype = 0;
	CAObjHandle		pport = 0;
	CAObjHandle		pbaudrate = 0;
	CAObjHandle		pdatabits = 0;
	CAObjHandle		pstopbits = 0;
	CAObjHandle		pparity = 0;
	CAObjHandle		phandshake = 0;
	CAObjHandle		pinit_string = 0;
	
	/* TODO: read configuration from XML configString */
	EXCCHECKCOM( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &xml_doc));
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentloadXML (xml_doc, NULL, configString, &is_xml_success));
	
	/* Get ID of barcode reader - same as index */
	EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode (xml_doc, NULL,
														"/BarCodeReader/@id", &pid));
	EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (pid, NULL, &id_str));
	size = strlen(id_str) + 1;
	me->idBCR = (char*) calloc( size, sizeof(char));
	strcpy_s(me->idBCR, size, id_str);

	/* Get communication type */
	EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode (xml_doc, NULL,
														"/BarCodeReader/Configuration/Communication",
														&presource));
	if (presource == 0)
	{
		sprintf(message, "Missing configuration node");
		EXCTHROW( BCR_ERROR_INVALID_CONFIGURATION, message);
	}
	
	EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(presource, NULL, "@type", &ptype));
	if (ptype == 0)
	{
		sprintf(message, "Missing communication type");
		EXCTHROW( BCR_ERROR_INVALID_CONFIGURATION, message);
	}
	EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (ptype, NULL, &type));
	if (!strcmp(type, "serial"))
	{
		/* Serial communication */
		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(presource, NULL, "Port", &pport));
		if (pport == 0)
		{
			sprintf(message, "Missing communication port");
			EXCTHROW( BCR_ERROR_INVALID_CONFIGURATION, message);
		}
		EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (pport, NULL, &port));
		sscanf(port,"COM%d", &(me->port));
			
		if (me->fd == INVALID_HANDLE_VALUE)
		{
			DWORD	bcr_error = GetLastError();
			LPVOID	lpMsgBuf;

			FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    					   NULL, bcr_error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
    					   (LPTSTR) &lpMsgBuf, 0, NULL );
			sprintf(message, "Serial port \"%s\" can not be open\nError: %s", port, (LPCTSTR) lpMsgBuf);
			EXCTHROW( BCR_ERROR_OPEN_FAILED, message);
		}
		
		dcb.DCBlength = sizeof(DCB);
		
		/* Set default values */
		dcb.BaudRate    = 9600;
		dcb.ByteSize    = 8;
		dcb.Parity		= NOPARITY;
		dcb.StopBits    = ONESTOPBIT;
		
		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(presource, NULL, "Baudrate", &pbaudrate));
		if (pbaudrate)
		{
			EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (pbaudrate, NULL, &tmp));
			dcb.BaudRate = atoi(tmp);
			CA_FreeMemory(tmp);
			tmp = NULL;
		}
		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(presource, NULL, "DataBits", &pdatabits));
		if (pdatabits)
		{
			EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (pdatabits, NULL, &tmp));
			dcb.ByteSize = (BYTE) atoi(tmp);
			CA_FreeMemory(tmp);
			tmp = NULL;
		}
		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(presource, NULL, "StopBits", &pstopbits));
		if (pstopbits)
		{
			EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (pstopbits, NULL, &tmp));
			if (!strcmp(tmp, "1"))
				dcb.StopBits = ONE5STOPBITS;
			else if (!strcmp(tmp, "2"))
				dcb.StopBits = TWOSTOPBITS;
			else
			{
				sprintf(message, "Invalid stop bits configuration (%s)", tmp);
				EXCTHROW( BCR_ERROR_INVALID_CONFIGURATION, message);
			}
			CA_FreeMemory(tmp);
			tmp = NULL;
		}
		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(presource, NULL, "Parity", &pparity));
		if (pparity)
		{
			EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (pparity, NULL, &tmp));
			if (!strcmp(tmp, "None"))
				dcb.Parity = NOPARITY;
			else if (!strcmp(tmp, "Odd"))
				dcb.Parity = ODDPARITY;
			else if (!strcmp(tmp, "Even"))
				dcb.Parity = EVENPARITY;
			else if (!strcmp(tmp, "Mark"))
				dcb.Parity = MARKPARITY;
			else if (!strcmp(tmp, "Space"))
				dcb.Parity = SPACEPARITY;
			else
			{
				sprintf(message, "Invalid parity configuration (%s)", tmp);
				EXCTHROW( BCR_ERROR_INVALID_CONFIGURATION, message);
			}
			CA_FreeMemory(tmp);
			tmp = NULL;
		}
		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(presource, NULL, "Handshake", &phandshake));
		if (phandshake)
		{
			EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (phandshake, NULL, &tmp));
			if (strcmp(tmp, "None"))
			{
				sprintf(message, "Invalid handshake configuration (%s)", tmp);
				EXCTHROW( BCR_ERROR_INVALID_CONFIGURATION, message);
			}
			CA_FreeMemory(tmp);
			tmp = NULL;
		}
	}
	else
	{
		sprintf(message, "Unknown communication type (%s)", type);
		EXCTHROW( BCR_ERROR_INVALID_CONFIGURATION, message);
	}
	/* Open COM port */
	EXCCHECKCOM( OpenComConfig (me->port, "", dcb.BaudRate, dcb.Parity,
                                    dcb.ByteSize, dcb.StopBits, 512, 512));

Error:
	if (init_string)
		CA_FreeMemory(init_string);

	if (tmp)
		CA_FreeMemory(tmp);
	
	if (port)
		CA_FreeMemory(port);
	
	if (type)
		CA_FreeMemory(type);
	
	if (id_str)
		CA_FreeMemory(id_str);
	
	if (pinit_string)
		CA_DiscardObjHandle(pinit_string);
	
	if (phandshake)
		CA_DiscardObjHandle(phandshake);
	
	if (pparity)
		CA_DiscardObjHandle(pparity);
	
	if (pstopbits)
		CA_DiscardObjHandle(pstopbits);
	
	if (pdatabits)
		CA_DiscardObjHandle(pdatabits);
	
	if (pbaudrate)
		CA_DiscardObjHandle(pbaudrate);
	
	if (pport)
		CA_DiscardObjHandle(pport);
	
	if (ptype)
		CA_DiscardObjHandle(ptype);
	
	if (presource)
		CA_DiscardObjHandle(presource);
	
	if (pid)
		CA_DiscardObjHandle(pid);
	
	if (xml_doc)
		CA_DiscardObjHandle(xml_doc);
	
	EXCRETHROW(pexception);
}

#undef __FUNC__
#define __FUNC__	"BCR::Close"
static SElExceptionPtr bcr_Close(struct _BarCodeReader* me)
{
	SElExceptionPtr	pexception = NULL;
	HRESULT			hr = S_OK;

	/* Close COM port */
	EXCCHECKCOM( CloseCom (me->port));
	
Error:
	EXCRETHROW(pexception);
}

#undef __FUNC__
#define __FUNC__	"BCR::StartReading"
static SElExceptionPtr bcr_StartReading(struct _BarCodeReader* me)
{
	SElExceptionPtr	pexception = NULL;

	FlushInQ (me->port);
	FlushOutQ (me->port);
	ComWrt (me->port, "+", StringLength ("+"));
	Sleep(10);
	ReturnRS232Err ();	
Error:
	EXCRETHROW(pexception);
}

#undef __FUNC__
#define __FUNC__	"BCR::StopReading"
static SElExceptionPtr bcr_StopReading(struct _BarCodeReader* me)
{
	SElExceptionPtr	pexception = NULL;
	

	FlushInQ (me->port);
	FlushOutQ (me->port);
	ComWrt (me->port, "-", StringLength ("-"));
	Sleep(10);
	ReturnRS232Err ();

	
Error:
	EXCRETHROW(pexception);
}

#undef __FUNC__
#define __FUNC__	"BCR::GetBarCode"
static SElExceptionPtr bcr_GetBarCode
(
	struct _BarCodeReader*	me,
	char*					buffer,
	int32_t					bufferSize
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			len = strlen(me->barCodeBuffer),
					size = 0;
	int32_t received = 0;
	int      i;

	received  = GetInQLen (me->port); 
	for(i=0;i<received;i++)
	{
		buffer[i] = ComRdByte (me->port);
		if (buffer[i] == 0x20)
			break;
	}

	buffer[i] = '\0';
	
	if (!received)
		EXCTHROW( BCR_ERROR_WRITE_TIMEOUT, "Barcode Timeouted");
Error:
	EXCRETHROW(pexception);
}

#pragma warning( pop )
