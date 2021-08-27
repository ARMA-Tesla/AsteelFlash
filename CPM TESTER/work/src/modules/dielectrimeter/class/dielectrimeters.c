#include "dielectrimeters.h"
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

static SElExceptionPtr dielectrimeter_open(
		struct _Dielectrimeter*	me,
		const char*			resource,
		const char*			optionString
);
static SElExceptionPtr dielectrimeter_close(struct _Dielectrimeter* me);
static SElExceptionPtr dielectrimeter_connect(struct _Dielectrimeter* me);
static SElExceptionPtr dielectrimeter_disconnect(struct _Dielectrimeter* me);
static SElExceptionPtr dielectrimeter_write(SDielectrimeterPtr me, const char* command);
static SElExceptionPtr dielectrimeter_read(SDielectrimeterPtr me, char *read);
static SElExceptionPtr dielectrimeter_SetModeManual(SDielectrimeterPtr me);
static SElExceptionPtr dielectrimeter_SetStep(SDielectrimeterPtr me, int step);
static SElExceptionPtr dielectrimeter_SetOutput(SDielectrimeterPtr me, int output);
static SElExceptionPtr dielectrimeter_TestResult(SDielectrimeterPtr me, int *result, double *voltage_kv, double *current_mA);
static SElExceptionPtr dielectrimeter_SystemError(SDielectrimeterPtr me, char * error);
static SElExceptionPtr dielectrimeter_ClearError(SDielectrimeterPtr me);

DWORD WINAPI Thread_Dielectrimeter(
  LPVOID lpParameter
);

/*****************************************************************************/

static SElExceptionPtr dielectrimeters_GetDielectrimeterByName(
	struct _Dielectrimeters*	me,
	const char*			dielectrimeterName,
	SDielectrimeterPtr*		dielectrimeter
);
static SElExceptionPtr dielectrimeters_GetDielectrimeterByIndex(
	struct _Dielectrimeters*	me,
	int32_t				index,
	SDielectrimeterPtr*		dielectrimeter
);
static SElExceptionPtr dielectrimeters_GetCurrentDielectrimeter(
	struct _Dielectrimeters*	me,
	SDielectrimeterPtr*		dielectrimeter
);
static SElExceptionPtr dielectrimeters_SetCurrentDielectrimeterByName(
	struct _Dielectrimeters*	me,
	const char*			dielectrimeterName
);
static SElExceptionPtr dielectrimeters_GetFirstDielectrimeter(
	struct _Dielectrimeters*	me,
	SDielectrimeterPtr*		dielectrimeter
);
static SElExceptionPtr dielectrimeters_GetNextDielectrimeter(
	struct _Dielectrimeters*	me,
	SDielectrimeterPtr*		dielectrimeter
);
	
static SElExceptionPtr dielectrimeters_InitDielectrimeters(struct _Dielectrimeters* me);
static SElExceptionPtr dielectrimeters_CloseDielectrimeters(struct _Dielectrimeters* me);

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "dielectrimeter_new"
SElExceptionPtr dielectrimeter_new(
	SDielectrimeterPtr*	dielectrimeter,
	const char*		name,
	const char*		optionString
)
{
	SElExceptionPtr	pexception = NULL;
	SDielectrimeterPtr		pnew_dielectrimeter = NULL;
	
	pnew_dielectrimeter = calloc(1, sizeof(SDielectrimeter));
	EXCCHECKALLOC( pnew_dielectrimeter);
	InitializeCriticalSection(&(pnew_dielectrimeter->lock));
	
	pnew_dielectrimeter->dielectrimeterName = calloc(strlen(name) + 1, sizeof(char));
	EXCCHECKALLOC( pnew_dielectrimeter->dielectrimeterName);
	strcpy(pnew_dielectrimeter->dielectrimeterName, name);
	
	pnew_dielectrimeter->Open		= dielectrimeter_open;
	pnew_dielectrimeter->Close		= dielectrimeter_close;
	pnew_dielectrimeter->Connect	= dielectrimeter_connect;
	pnew_dielectrimeter->Disconnect= dielectrimeter_disconnect;
	pnew_dielectrimeter->Write		= dielectrimeter_write;
	pnew_dielectrimeter->Read		= dielectrimeter_read;
	pnew_dielectrimeter->SetModeManual	= dielectrimeter_SetModeManual;
	pnew_dielectrimeter->SetStep		= dielectrimeter_SetStep;
	pnew_dielectrimeter->SetOutput		= dielectrimeter_SetOutput;
	pnew_dielectrimeter->TestResult		= dielectrimeter_TestResult;
	pnew_dielectrimeter->SystemError	= dielectrimeter_SystemError;
	pnew_dielectrimeter->ClearError		= dielectrimeter_ClearError;

	if (dielectrimeter)
		*dielectrimeter = pnew_dielectrimeter;
	
Error:
	if (pexception && pnew_dielectrimeter)
		free(pnew_dielectrimeter);
	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "dielectrimeter_delete"
SElExceptionPtr dielectrimeter_delete(SDielectrimeterPtr* dielectrimeter)
{
	SElExceptionPtr	pexception = NULL;
	
	if (dielectrimeter && *dielectrimeter)
	{
		(*dielectrimeter)->Close(*dielectrimeter);
		
		if ((*dielectrimeter)->dielectrimeterName)
			free((*dielectrimeter)->dielectrimeterName);
		
		if ((*dielectrimeter)->optionString)
			free((*dielectrimeter)->optionString);
		
		DeleteCriticalSection( &((*dielectrimeter)->lock) );
		
		free(*dielectrimeter);
		*dielectrimeter = NULL;
	}
	
Error:
	EXCRETHROW( pexception);
}

/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "dielectrimeters_new"
SElExceptionPtr dielectrimeters_new(SDielectrimetersPtr*	dielectrimeters)
{
	SElExceptionPtr	pexception = NULL;
	SDielectrimetersPtr	pnew_dielectrimeters = NULL;
	
	pnew_dielectrimeters = calloc(1, sizeof(SDielectrimeters));
	EXCCHECKALLOC( pnew_dielectrimeters);
	
	pnew_dielectrimeters->dielectrimeters = NULL;
	pnew_dielectrimeters->numDielectrimeters = 0;
	pnew_dielectrimeters->currentIndex = -1;
	
	pnew_dielectrimeters->GetDielectrimeterByName			= dielectrimeters_GetDielectrimeterByName;
	pnew_dielectrimeters->GetDielectrimeterByIndex		= dielectrimeters_GetDielectrimeterByIndex;
	pnew_dielectrimeters->GetCurrentDielectrimeter		= dielectrimeters_GetCurrentDielectrimeter;
	pnew_dielectrimeters->SetCurrentDielectrimeterByName	= dielectrimeters_SetCurrentDielectrimeterByName;
	pnew_dielectrimeters->GetFirstDielectrimeter			= dielectrimeters_GetFirstDielectrimeter;
	pnew_dielectrimeters->GetNextDielectrimeter			= dielectrimeters_GetNextDielectrimeter;
	pnew_dielectrimeters->InitDielectrimeters				= dielectrimeters_InitDielectrimeters;
	pnew_dielectrimeters->CloseDielectrimeters			= dielectrimeters_CloseDielectrimeters;
	
	if (dielectrimeters)
		*dielectrimeters = pnew_dielectrimeters;
	
Error:
	if (pexception && pnew_dielectrimeters)
		free(pnew_dielectrimeters);
	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "dielectrimeters_delete"
SElExceptionPtr dielectrimeters_delete(SDielectrimetersPtr* dielectrimeters)
{
	SElExceptionPtr	pexception = NULL;
	
	if (dielectrimeters && *dielectrimeters)
	{
		EXCCHECK( (*dielectrimeters)->CloseDielectrimeters(*dielectrimeters));
		
		free(*dielectrimeters);
		*dielectrimeters = NULL;
	}
	
Error:
	EXCRETHROW( pexception);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "{Dielectrimeter}::Open"
static SElExceptionPtr dielectrimeter_open(
		struct _Dielectrimeter*	me,
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
													   "/Dielectrimeter/Configuration",
													   &xml_configuration));
	if (xml_configuration == 0)
		EXCTHROW( -1, "Missing Configuration Node");
	
	/* Load Specific DLL functions */
	EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(xml_configuration, NULL, "@specificDll", &xml_node));
	if (xml_node == 0)
		EXCTHROW( -3, "Missing Dielectrimeter Specific Library");
	
	EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(xml_node, NULL, &xml_value));
	me->specLib = LoadLibrary(xml_value);
	
	if (me->specLib == 0)
	{
		sprintf(message, "Dielectrimeter Library (%s) Loading Failed", xml_value);
		EXCTHROW( -4, message);
	}
	
	me->_ConnectSpec	= (SElExceptionPtr (*)(SDielectrimeterPtr))
								GetProcAddress(me->specLib, "Connect");
	me->_DisconnectSpec	= (SElExceptionPtr (*)(SDielectrimeterPtr))
								GetProcAddress(me->specLib, "Disconnect");
	me->_WriteSpec		= (SElExceptionPtr (*)(SDielectrimeterPtr,const char*))
								GetProcAddress(me->specLib, "Write");
   	me->_ReadSpec		= (SElExceptionPtr (*)(SDielectrimeterPtr, const char*))
								GetProcAddress(me->specLib, "Read");
	me->_SetModeManualSpec		= (SElExceptionPtr (*)(SDielectrimeterPtr))
								GetProcAddress(me->specLib, "SetModeManual");
	me->_SetStepSpec		= (SElExceptionPtr (*)(SDielectrimeterPtr, int))
								GetProcAddress(me->specLib, "SetStep");
	me->_SetOutputSpec		= (SElExceptionPtr (*)(SDielectrimeterPtr, int))
								GetProcAddress(me->specLib, "SetOutput");
	me->_TestResultSpec		= (SElExceptionPtr (*)(SDielectrimeterPtr, int*,double*,double*))
								GetProcAddress(me->specLib, "TestResult");
	me->_SystemErrorSpec		= (SElExceptionPtr (*)(SDielectrimeterPtr, char*))
								GetProcAddress(me->specLib, "SystemError");
	me->_ClearErrorSpec		= (SElExceptionPtr (*)(SDielectrimeterPtr))
								GetProcAddress(me->specLib, "ClearError");

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
			EXCTHROW( -5, "Missing port dielectrimeter name");
	
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
		me->_ReadSpec		= NULL;
		me->_SetModeManualSpec		= NULL;
		me->_SetStepSpec			= NULL;
		me->_SetOutputSpec			= NULL;
		me->_TestResultSpec			= NULL;
		me->_SystemErrorSpec		= NULL;
		me->_ClearErrorSpec			= NULL;

	}
		
	DISCARD_HANDLE(xml_node);
	CA_FREE_MEMORY(xml_value);
	CA_FREE_MEMORY(xml_type);
	DISCARD_HANDLE(xml_configuration);
	DISCARD_HANDLE(document);
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Dielectrimeter}::Close"
static SElExceptionPtr dielectrimeter_close(struct _Dielectrimeter* me)
{
	SElExceptionPtr	pexception = NULL;
	
		me->_DisconnectSpec (me);
		
		me->_ConnectSpec	= NULL;
		me->_DisconnectSpec	= NULL;
		me->_WriteSpec		= NULL;
		me->_ReadSpec		= NULL;
		me->_SetModeManualSpec		= NULL;
		me->_SetStepSpec			= NULL;
		me->_SetOutputSpec			= NULL;
		me->_TestResultSpec			= NULL;
		me->_SystemErrorSpec		= NULL;
		me->_ClearErrorSpec			= NULL;
	
Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "fncLock"
static SElExceptionPtr dielectrimeterLock(SDielectrimeterPtr me)
{
	EnterCriticalSection( &me->lock);    
	return NULL;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dielectrimeterUnLock"
static SElExceptionPtr dielectrimeterUnLock(SDielectrimeterPtr me)
{
	LeaveCriticalSection( &me->lock);   
	return NULL;
}

#undef __FUNC__
#define __FUNC__ "{Dielectrimeter}::Connect"
static SElExceptionPtr dielectrimeter_connect(SDielectrimeterPtr me)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_ConnectSpec)
		EXCCHECK( me->_ConnectSpec(me));
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Dielectrimeter}::Disconnect"
static SElExceptionPtr dielectrimeter_disconnect(SDielectrimeterPtr me)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_ConnectSpec)
		EXCCHECK( me->_DisconnectSpec(me));
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Dielectrimeter}::Write"
static SElExceptionPtr dielectrimeter_write(SDielectrimeterPtr me, const char* command)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_WriteSpec)
		EXCCHECK( me->_WriteSpec(me, command));
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Dielectrimeter}::Read"
static SElExceptionPtr dielectrimeter_read(SDielectrimeterPtr me, char *read)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_ReadSpec)
		EXCCHECK( me->_ReadSpec(me, read));
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Dielectrimeter}::SetModeManual"
static SElExceptionPtr dielectrimeter_SetModeManual(SDielectrimeterPtr me)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_SetModeManualSpec)
		EXCCHECK( me->_SetModeManualSpec(me));
	
Error:
	EXCRETHROW( pexception);
}
#undef __FUNC__
#define __FUNC__ "{Dielectrimeter}::SetStep"
static SElExceptionPtr dielectrimeter_SetStep(SDielectrimeterPtr me, int step)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_SetStepSpec)
		EXCCHECK( me->_SetStepSpec(me, step));
	
Error:
	EXCRETHROW( pexception);
}
#undef __FUNC__
#define __FUNC__ "{Dielectrimeter}::SetOutput"
static SElExceptionPtr dielectrimeter_SetOutput(SDielectrimeterPtr me, int output)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_SetOutputSpec)
		EXCCHECK( me->_SetOutputSpec(me, output));
	
Error:
	EXCRETHROW( pexception);
}
#undef __FUNC__
#define __FUNC__ "{Dielectrimeter}::TestResult"
static SElExceptionPtr dielectrimeter_TestResult(SDielectrimeterPtr me, int *result, double *voltage_kv, double *current_mA)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_TestResultSpec)
		EXCCHECK( me->_TestResultSpec(me, result, voltage_kv,current_mA));
	
Error:
	EXCRETHROW( pexception);
}
#undef __FUNC__
#define __FUNC__ "{Dielectrimeter}::SystemError"
static SElExceptionPtr dielectrimeter_SystemError(SDielectrimeterPtr me, char *error)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_SystemErrorSpec)
		EXCCHECK( me->_SystemErrorSpec(me, error));
	
Error:
	EXCRETHROW( pexception);
}
#undef __FUNC__
#define __FUNC__ "{Dielectrimeter}::ClearError"
static SElExceptionPtr dielectrimeter_ClearError(SDielectrimeterPtr me)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_ClearErrorSpec)
		EXCCHECK( me->_ClearErrorSpec(me));
	
Error:
	EXCRETHROW( pexception);
}

/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "{Dielectrimeters}::GetDielectrimeterByName"
static SElExceptionPtr dielectrimeters_GetDielectrimeterByName(
	struct _Dielectrimeters*	me,
	const char*			dielectrimeterName,
	SDielectrimeterPtr*		dielectrimeter
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			i = 0;
	SDielectrimeterPtr		found_dielectrimeter = NULL;
	char			message[1024] = "";
	
	for (i = 0; i < me->numDielectrimeters; i++)
	{
		if (!strcmp(me->dielectrimeters[i]->dielectrimeterName, dielectrimeterName))
		{
			found_dielectrimeter = me->dielectrimeters[i];
			break;
		}
	}
	
	if (dielectrimeter)
		*dielectrimeter = found_dielectrimeter;
	
	if (found_dielectrimeter == NULL)
	{
		sprintf(message, "Dielectrimeter Name (%s) Not Found", dielectrimeterName);
		EXCTHROW( -12, message);
	}
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Dielectrimeters}::GetDielectrimeterByIndex"
static SElExceptionPtr dielectrimeters_GetDielectrimeterByIndex(
	struct _Dielectrimeters*	me,
	int32_t				index,
	SDielectrimeterPtr*		dielectrimeter
)
{
	SElExceptionPtr	pexception = NULL;
	
	if (index < me->numDielectrimeters && index >= 0)
	{
		if (dielectrimeter)
			*dielectrimeter = me->dielectrimeters[index];
	}
	else
	{
		if (dielectrimeter)
			*dielectrimeter = NULL;
		
		EXCTHROW( -13, "Index Out of Range");
	}
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Dielectrimeters}::GetCurrentDielectrimeter"
static SElExceptionPtr dielectrimeters_GetCurrentDielectrimeter(
	struct _Dielectrimeters*	me,
	SDielectrimeterPtr*		dielectrimeter
)
{
	SElExceptionPtr	pexception = NULL;
	
	if (me->currentIndex >= 0 && me->numDielectrimeters > 0)
	{
		if (dielectrimeter)
			*dielectrimeter = me->dielectrimeters[me->currentIndex];
	}
	else
	{
		if (dielectrimeter)
			*dielectrimeter = NULL;
	}

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Dielectrimeters}::SetCurrentDielectrimeterByName"
static SElExceptionPtr dielectrimeters_SetCurrentDielectrimeterByName(
	struct _Dielectrimeters*	me,
	const char*			dielectrimeterName
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			i = 0;
	SDielectrimeterPtr		dielectrimeter = NULL;
	char			message[1024] = "";

	for (i = 0; i < me->numDielectrimeters; i++)
	{
		if (!strcmp(me->dielectrimeters[i]->dielectrimeterName, dielectrimeterName))
		{
			me->currentIndex = i;
			break;
		}
	}
	
	if (i >= me->numDielectrimeters)
	{
		sprintf(message, "Dielectrimeter Name (%s) Not Found", dielectrimeterName);
		EXCTHROW( -12, message);
	}

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Dielectrimeters}::GetFirstDielectrimeter"
static SElExceptionPtr dielectrimeters_GetFirstDielectrimeter(
	struct _Dielectrimeters*	me,
	SDielectrimeterPtr*		dielectrimeter
)
{
	SElExceptionPtr	pexception = NULL;
	
	me->currentIndex = 0;

	if(me->numDielectrimeters > 0)
	{
		if (dielectrimeter)
			*dielectrimeter = me->dielectrimeters[0];
	}
	else
	{
		if (dielectrimeter)
			*dielectrimeter = NULL;
	}

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Dielectrimeters}::GetNextDielectrimeter"
static SElExceptionPtr dielectrimeters_GetNextDielectrimeter(
	struct _Dielectrimeters*	me,
	SDielectrimeterPtr*		dielectrimeter
)
{
	SElExceptionPtr	pexception = NULL;

	if (++(me->currentIndex) >= me->numDielectrimeters)
	{
		me->currentIndex = me->numDielectrimeters - 1;
		if (dielectrimeter)
			*dielectrimeter = NULL;
	}
	else
	{
		if (dielectrimeter)
			*dielectrimeter = me->dielectrimeters[me->currentIndex];
	}
	
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Dielectrimeters}::InitDielectrimeters"
static SElExceptionPtr dielectrimeters_InitDielectrimeters(struct _Dielectrimeters* me)
{
	SElExceptionPtr	pexception = NULL;
	HRESULT			hr = S_OK;
	VARIANT_BOOL	is_xml_success = 1;
	VARIANT			file;
	CAObjHandle		document = 0;
	CAObjHandle		dielectrimeter = 0;
	CAObjHandle		dielectrimeter_name_node = 0;
	CAObjHandle		dielectrimeter_list = 0;
	char*			xml_value = NULL;
	char*			dielectrimeter_xml = NULL;
	
	EXCCHECKCOM( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &document));
	
	CA_VariantSetCString (&file, "modules/dielectrimeters.xml");
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentload(document, NULL, file, &is_xml_success));
	
	EXCCHECKCOM( MSXML_IXMLDOMDocumentselectNodes(document, NULL, "/Dielectrimeters/Dielectrimeter", &dielectrimeter_list));
	EXCCHECKCOM( MSXML_IXMLDOMNodeListreset (dielectrimeter_list, NULL));
	
	EXCCHECKCOM( MSXML_IXMLDOMNodeListnextNode(dielectrimeter_list, NULL, &dielectrimeter));
	while (dielectrimeter)
	{
		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(dielectrimeter, NULL, "@name", &dielectrimeter_name_node));
		EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(dielectrimeter_name_node, NULL, &xml_value));
		EXCCHECKCOM( MSXML_IXMLDOMNodeGetxml(dielectrimeter, NULL, &dielectrimeter_xml));
		
		me->dielectrimeters = realloc( me->dielectrimeters, (me->numDielectrimeters + 1) * sizeof(SDielectrimeterPtr));
		EXCCHECKALLOC( me->dielectrimeters);
		
		EXCCHECK( dielectrimeter_new( &me->dielectrimeters[me->numDielectrimeters], xml_value, NULL));
		EXCCHECK( me->dielectrimeters[me->numDielectrimeters]->Open(me->dielectrimeters[me->numDielectrimeters], NULL, dielectrimeter_xml));
		me->numDielectrimeters++;
		
		CA_FREE_MEMORY(xml_value);
		CA_FREE_MEMORY(dielectrimeter_xml);
		DISCARD_HANDLE(dielectrimeter_name_node);
		DISCARD_HANDLE(dielectrimeter);
		
		EXCCHECKCOM( MSXML_IXMLDOMNodeListnextNode(dielectrimeter_list, NULL, &dielectrimeter));
	}
	
Error:
	CA_FREE_MEMORY(xml_value);
	CA_FREE_MEMORY(dielectrimeter_xml);
	DISCARD_HANDLE(dielectrimeter_name_node);
	DISCARD_HANDLE(dielectrimeter);
	DISCARD_HANDLE(dielectrimeter_list);
	DISCARD_HANDLE(document);
	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Dielectrimeters}::CloseDielectrimeters"
static SElExceptionPtr dielectrimeters_CloseDielectrimeters(struct _Dielectrimeters* me)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			i = 0;
	
	for (i = 0; i < me->numDielectrimeters; i++)
	{
		EXCCHECK( dielectrimeter_delete(&(me->dielectrimeters[i])));
	}
	free(me->dielectrimeters);
	me->dielectrimeters = NULL;
	me->numDielectrimeters = 0;
	
Error:
	EXCRETHROW( pexception);
}
