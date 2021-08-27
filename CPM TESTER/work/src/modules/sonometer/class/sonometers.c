#include "sonometers.h"
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

static SElExceptionPtr sonometer_open(
		struct _Sonometer*	me,
		const char*			resource,
		const char*			optionString
);
static SElExceptionPtr sonometer_close(struct _Sonometer* me);
static SElExceptionPtr sonometer_connect(struct _Sonometer* me);
static SElExceptionPtr sonometer_disconnect(struct _Sonometer* me);
static SElExceptionPtr sonometer_write(SSonometerPtr me, const char* command);
static SElExceptionPtr sonometer_read(SSonometerPtr me, const char* data);
static SElExceptionPtr sonometer_sound_level_meter(SSonometerPtr me, int group, float *data, int *count_data, char *log);

DWORD WINAPI Thread_Sonometer(
  LPVOID lpParameter
);

/*****************************************************************************/

static SElExceptionPtr sonometers_GetSonometerByName(
	struct _Sonometers*	me,
	const char*			sonometerName,
	SSonometerPtr*		sonometer
);
static SElExceptionPtr sonometers_GetSonometerByIndex(
	struct _Sonometers*	me,
	int32_t				index,
	SSonometerPtr*		sonometer
);
static SElExceptionPtr sonometers_GetCurrentSonometer(
	struct _Sonometers*	me,
	SSonometerPtr*		sonometer
);
static SElExceptionPtr sonometers_SetCurrentSonometerByName(
	struct _Sonometers*	me,
	const char*			sonometerName
);
static SElExceptionPtr sonometers_GetFirstSonometer(
	struct _Sonometers*	me,
	SSonometerPtr*		sonometer
);
static SElExceptionPtr sonometers_GetNextSonometer(
	struct _Sonometers*	me,
	SSonometerPtr*		sonometer
);
	
static SElExceptionPtr sonometers_InitSonometers(struct _Sonometers* me);
static SElExceptionPtr sonometers_CloseSonometers(struct _Sonometers* me);

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "sonometer_new"
SElExceptionPtr sonometer_new(
	SSonometerPtr*	sonometer,
	const char*		name,
	const char*		optionString
)
{
	SElExceptionPtr	pexception = NULL;
	SSonometerPtr		pnew_sonometer = NULL;
	
	pnew_sonometer = calloc(1, sizeof(SSonometer));
	EXCCHECKALLOC( pnew_sonometer);
	InitializeCriticalSection(&(pnew_sonometer->lock));
	
	pnew_sonometer->sonometerName = calloc(strlen(name) + 1, sizeof(char));
	EXCCHECKALLOC( pnew_sonometer->sonometerName);
	strcpy(pnew_sonometer->sonometerName, name);
	
	pnew_sonometer->Open		= sonometer_open;
	pnew_sonometer->Close		= sonometer_close;
	pnew_sonometer->Connect	= sonometer_connect;
	pnew_sonometer->Disconnect= sonometer_disconnect;
	pnew_sonometer->Write		= sonometer_write;
	pnew_sonometer->Read		= sonometer_read;
	pnew_sonometer->SoundLevelMeter	= sonometer_sound_level_meter;

	if (sonometer)
		*sonometer = pnew_sonometer;
	
Error:
	if (pexception && pnew_sonometer)
		free(pnew_sonometer);
	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "sonometer_delete"
SElExceptionPtr sonometer_delete(SSonometerPtr* sonometer)
{
	SElExceptionPtr	pexception = NULL;
	
	if (sonometer && *sonometer)
	{
		(*sonometer)->Close(*sonometer);
		
		if ((*sonometer)->sonometerName)
			free((*sonometer)->sonometerName);
		
		if ((*sonometer)->optionString)
			free((*sonometer)->optionString);
		
		DeleteCriticalSection( &((*sonometer)->lock) );
		
		free(*sonometer);
		*sonometer = NULL;
	}
	
Error:
	EXCRETHROW( pexception);
}

/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "sonometers_new"
SElExceptionPtr sonometers_new(SSonometersPtr*	sonometers)
{
	SElExceptionPtr	pexception = NULL;
	SSonometersPtr	pnew_sonometers = NULL;
	
	pnew_sonometers = calloc(1, sizeof(SSonometers));
	EXCCHECKALLOC( pnew_sonometers);
	
	pnew_sonometers->sonometers = NULL;
	pnew_sonometers->numSonometers = 0;
	pnew_sonometers->currentIndex = -1;
	
	pnew_sonometers->GetSonometerByName			= sonometers_GetSonometerByName;
	pnew_sonometers->GetSonometerByIndex		= sonometers_GetSonometerByIndex;
	pnew_sonometers->GetCurrentSonometer		= sonometers_GetCurrentSonometer;
	pnew_sonometers->SetCurrentSonometerByName	= sonometers_SetCurrentSonometerByName;
	pnew_sonometers->GetFirstSonometer			= sonometers_GetFirstSonometer;
	pnew_sonometers->GetNextSonometer			= sonometers_GetNextSonometer;
	pnew_sonometers->InitSonometers				= sonometers_InitSonometers;
	pnew_sonometers->CloseSonometers			= sonometers_CloseSonometers;
	
	if (sonometers)
		*sonometers = pnew_sonometers;
	
Error:
	if (pexception && pnew_sonometers)
		free(pnew_sonometers);
	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "sonometers_delete"
SElExceptionPtr sonometers_delete(SSonometersPtr* sonometers)
{
	SElExceptionPtr	pexception = NULL;
	
	if (sonometers && *sonometers)
	{
		EXCCHECK( (*sonometers)->CloseSonometers(*sonometers));
		
		free(*sonometers);
		*sonometers = NULL;
	}
	
Error:
	EXCRETHROW( pexception);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "{Sonometer}::Open"
static SElExceptionPtr sonometer_open(
		struct _Sonometer*	me,
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
													   "/Sonometer/Configuration",
													   &xml_configuration));
	if (xml_configuration == 0)
		EXCTHROW( -1, "Missing Configuration Node");
	
	/* Load Specific DLL functions */
	EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(xml_configuration, NULL, "@specificDll", &xml_node));
	if (xml_node == 0)
		EXCTHROW( -3, "Missing Sonometer Specific Library");
	
	EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(xml_node, NULL, &xml_value));
	me->specLib = LoadLibrary(xml_value);
	
	if (me->specLib == 0)
	{
		sprintf(message, "Sonometer Library (%s) Loading Failed", xml_value);
		EXCTHROW( -4, message);
	}
	
	me->_ConnectSpec	= (SElExceptionPtr (*)(SSonometerPtr))
								GetProcAddress(me->specLib, "Connect");
	me->_DisconnectSpec	= (SElExceptionPtr (*)(SSonometerPtr))
								GetProcAddress(me->specLib, "Disconnect");
	me->_WriteSpec		= (SElExceptionPtr (*)(SSonometerPtr,const char*))
								GetProcAddress(me->specLib, "Write");
   	me->_ReadSpec		= (SElExceptionPtr (*)(SSonometerPtr,const char*))
								GetProcAddress(me->specLib, "Read");
	me->_SoundLevelMeterSpec = (SElExceptionPtr (*)(SSonometerPtr, int , float *, int *, char *))
								GetProcAddress(me->specLib, "SoundLevelMeter");
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
			EXCTHROW( -5, "Missing port sonometer name");
	
		if (xml_node)
		{
			EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(xml_node, NULL, &xml_value));
			me->port = (int)atoi(xml_value);
			CA_FREE_MEMORY(xml_value);
		}
		
		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(xml_configuration, NULL,
													   "BaudRate", &xml_node));
		if (xml_node)
		{
			EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(xml_node, NULL, &xml_value));
			me->baudRate = (int)atol(xml_value);
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
		me->_SoundLevelMeterSpec		= NULL;
		
	}
		
	DISCARD_HANDLE(xml_node);
	CA_FREE_MEMORY(xml_value);
	CA_FREE_MEMORY(xml_type);
	DISCARD_HANDLE(xml_configuration);
	DISCARD_HANDLE(document);
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Sonometer}::Close"
static SElExceptionPtr sonometer_close(struct _Sonometer* me)
{
	SElExceptionPtr	pexception = NULL;
	
		me->_DisconnectSpec (me);
		
		me->_ConnectSpec	= NULL;
		me->_DisconnectSpec	= NULL;
		me->_WriteSpec		= NULL;
		me->_ReadSpec		= NULL;
		me->_SoundLevelMeterSpec		= NULL;
	
Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "fncLock"
static SElExceptionPtr sonometerLock(SSonometerPtr me)
{
	EnterCriticalSection( &me->lock);    
	return NULL;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "sonometerUnLock"
static SElExceptionPtr sonometerUnLock(SSonometerPtr me)
{
	LeaveCriticalSection( &me->lock);   
	return NULL;
}

#undef __FUNC__
#define __FUNC__ "{Sonometer}::Connect"
static SElExceptionPtr sonometer_connect(SSonometerPtr me)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_ConnectSpec)
		EXCCHECK( me->_ConnectSpec(me));
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Sonometer}::Disconnect"
static SElExceptionPtr sonometer_disconnect(SSonometerPtr me)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_ConnectSpec)
		EXCCHECK( me->_DisconnectSpec(me));
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Sonometer}::Write"
static SElExceptionPtr sonometer_write(SSonometerPtr me, const char* command)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_WriteSpec)
		EXCCHECK( me->_WriteSpec(me, command));
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Sonometer}::Read"
static SElExceptionPtr sonometer_read(SSonometerPtr me, const char* data)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_ReadSpec)
		EXCCHECK( me->_ReadSpec(me, data));
	
Error:
	EXCRETHROW( pexception);
}
#undef __FUNC__
#define __FUNC__ "{Sonometer}::SoundLevelMeter"
static SElExceptionPtr sonometer_sound_level_meter(SSonometerPtr me, int group, float *data, int *count_data, char *log)
{
	SElExceptionPtr	pexception = NULL;
			 
	if (me->_SoundLevelMeterSpec)
		EXCCHECK( me->_SoundLevelMeterSpec(me, group, data, count_data, log));
	
Error:
	EXCRETHROW( pexception);
}

/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "{Sonometers}::GetSonometerByName"
static SElExceptionPtr sonometers_GetSonometerByName(
	struct _Sonometers*	me,
	const char*			sonometerName,
	SSonometerPtr*		sonometer
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			i = 0;
	SSonometerPtr		found_sonometer = NULL;
	char			message[1024] = "";
	
	for (i = 0; i < me->numSonometers; i++)
	{
		if (!strcmp(me->sonometers[i]->sonometerName, sonometerName))
		{
			found_sonometer = me->sonometers[i];
			break;
		}
	}
	
	if (sonometer)
		*sonometer = found_sonometer;
	
	if (found_sonometer == NULL)
	{
		sprintf(message, "Sonometer Name (%s) Not Found", sonometerName);
		EXCTHROW( -12, message);
	}
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Sonometers}::GetSonometerByIndex"
static SElExceptionPtr sonometers_GetSonometerByIndex(
	struct _Sonometers*	me,
	int32_t				index,
	SSonometerPtr*		sonometer
)
{
	SElExceptionPtr	pexception = NULL;
	
	if (index < me->numSonometers && index >= 0)
	{
		if (sonometer)
			*sonometer = me->sonometers[index];
	}
	else
	{
		if (sonometer)
			*sonometer = NULL;
		
		EXCTHROW( -13, "Index Out of Range");
	}
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Sonometers}::GetCurrentSonometer"
static SElExceptionPtr sonometers_GetCurrentSonometer(
	struct _Sonometers*	me,
	SSonometerPtr*		sonometer
)
{
	SElExceptionPtr	pexception = NULL;
	
	if (me->currentIndex >= 0 && me->numSonometers > 0)
	{
		if (sonometer)
			*sonometer = me->sonometers[me->currentIndex];
	}
	else
	{
		if (sonometer)
			*sonometer = NULL;
	}

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Sonometers}::SetCurrentSonometerByName"
static SElExceptionPtr sonometers_SetCurrentSonometerByName(
	struct _Sonometers*	me,
	const char*			sonometerName
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			i = 0;
	SSonometerPtr		sonometer = NULL;
	char			message[1024] = "";

	for (i = 0; i < me->numSonometers; i++)
	{
		if (!strcmp(me->sonometers[i]->sonometerName, sonometerName))
		{
			me->currentIndex = i;
			break;
		}
	}
	
	if (i >= me->numSonometers)
	{
		sprintf(message, "Sonometer Name (%s) Not Found", sonometerName);
		EXCTHROW( -12, message);
	}

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Sonometers}::GetFirstSonometer"
static SElExceptionPtr sonometers_GetFirstSonometer(
	struct _Sonometers*	me,
	SSonometerPtr*		sonometer
)
{
	SElExceptionPtr	pexception = NULL;
	
	me->currentIndex = 0;

	if(me->numSonometers > 0)
	{
		if (sonometer)
			*sonometer = me->sonometers[0];
	}
	else
	{
		if (sonometer)
			*sonometer = NULL;
	}

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Sonometers}::GetNextSonometer"
static SElExceptionPtr sonometers_GetNextSonometer(
	struct _Sonometers*	me,
	SSonometerPtr*		sonometer
)
{
	SElExceptionPtr	pexception = NULL;

	if (++(me->currentIndex) >= me->numSonometers)
	{
		me->currentIndex = me->numSonometers - 1;
		if (sonometer)
			*sonometer = NULL;
	}
	else
	{
		if (sonometer)
			*sonometer = me->sonometers[me->currentIndex];
	}
	
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Sonometers}::InitSonometers"
static SElExceptionPtr sonometers_InitSonometers(struct _Sonometers* me)
{
	SElExceptionPtr	pexception = NULL;
	HRESULT			hr = S_OK;
	VARIANT_BOOL	is_xml_success = 1;
	VARIANT			file;
	CAObjHandle		document = 0;
	CAObjHandle		sonometer = 0;
	CAObjHandle		sonometer_name_node = 0;
	CAObjHandle		sonometer_list = 0;
	char*			xml_value = NULL;
	char*			sonometer_xml = NULL;
	
	EXCCHECKCOM( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &document));
	
	CA_VariantSetCString (&file, "modules/sonometers.xml");
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentload(document, NULL, file, &is_xml_success));
	
	EXCCHECKCOM( MSXML_IXMLDOMDocumentselectNodes(document, NULL, "/Sonometers/Sonometer", &sonometer_list));
	EXCCHECKCOM( MSXML_IXMLDOMNodeListreset (sonometer_list, NULL));
	
	EXCCHECKCOM( MSXML_IXMLDOMNodeListnextNode(sonometer_list, NULL, &sonometer));
	while (sonometer)
	{
		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(sonometer, NULL, "@name", &sonometer_name_node));
		EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(sonometer_name_node, NULL, &xml_value));
		EXCCHECKCOM( MSXML_IXMLDOMNodeGetxml(sonometer, NULL, &sonometer_xml));
		
		me->sonometers = realloc( me->sonometers, (me->numSonometers + 1) * sizeof(SSonometerPtr));
		EXCCHECKALLOC( me->sonometers);
		
		EXCCHECK( sonometer_new( &me->sonometers[me->numSonometers], xml_value, NULL));
		EXCCHECK( me->sonometers[me->numSonometers]->Open(me->sonometers[me->numSonometers], NULL, sonometer_xml));
		me->numSonometers++;
		
		CA_FREE_MEMORY(xml_value);
		CA_FREE_MEMORY(sonometer_xml);
		DISCARD_HANDLE(sonometer_name_node);
		DISCARD_HANDLE(sonometer);
		
		EXCCHECKCOM( MSXML_IXMLDOMNodeListnextNode(sonometer_list, NULL, &sonometer));
	}
	
Error:
	CA_FREE_MEMORY(xml_value);
	CA_FREE_MEMORY(sonometer_xml);
	DISCARD_HANDLE(sonometer_name_node);
	DISCARD_HANDLE(sonometer);
	DISCARD_HANDLE(sonometer_list);
	DISCARD_HANDLE(document);
	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Sonometers}::CloseSonometers"
static SElExceptionPtr sonometers_CloseSonometers(struct _Sonometers* me)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			i = 0;
	
	for (i = 0; i < me->numSonometers; i++)
	{
		EXCCHECK( sonometer_delete(&(me->sonometers[i])));
	}
	free(me->sonometers);
	me->sonometers = NULL;
	me->numSonometers = 0;
	
Error:
	EXCRETHROW( pexception);
}
