#include "generators.h"
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

static SElExceptionPtr generator_open(
		struct _Generator*	me,
		const char*			resource,
		const char*			optionString
);
static SElExceptionPtr generator_close(struct _Generator* me);
static SElExceptionPtr generator_connect(struct _Generator* me);
static SElExceptionPtr generator_disconnect(struct _Generator* me);
static SElExceptionPtr generator_write(SGeneratorPtr me, const char* command);
static SElExceptionPtr generator_read(SGeneratorPtr me, char *read);
static SElExceptionPtr generator_SetModeContinuous (SGeneratorPtr me);
static SElExceptionPtr generator_SetOutput(SGeneratorPtr me, int output);
static SElExceptionPtr generator_SetFonction (SGeneratorPtr me, double frequency, double amplitude, double offset, double duty);
static SElExceptionPtr generator_SystemError(SGeneratorPtr me, char * error);
static SElExceptionPtr generator_ClearError(SGeneratorPtr me);

DWORD WINAPI Thread_Generator(
  LPVOID lpParameter
);

/*****************************************************************************/

static SElExceptionPtr generators_GetGeneratorByName(
	struct _Generators*	me,
	const char*			generatorName,
	SGeneratorPtr*		generator
);
static SElExceptionPtr generators_GetGeneratorByIndex(
	struct _Generators*	me,
	int32_t				index,
	SGeneratorPtr*		generator
);
static SElExceptionPtr generators_GetCurrentGenerator(
	struct _Generators*	me,
	SGeneratorPtr*		generator
);
static SElExceptionPtr generators_SetCurrentGeneratorByName(
	struct _Generators*	me,
	const char*			generatorName
);
static SElExceptionPtr generators_GetFirstGenerator(
	struct _Generators*	me,
	SGeneratorPtr*		generator
);
static SElExceptionPtr generators_GetNextGenerator(
	struct _Generators*	me,
	SGeneratorPtr*		generator
);
	
static SElExceptionPtr generators_InitGenerators(struct _Generators* me);
static SElExceptionPtr generators_CloseGenerators(struct _Generators* me);

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "generator_new"
SElExceptionPtr generator_new(
	SGeneratorPtr*	generator,
	const char*		name,
	const char*		optionString
)
{
	SElExceptionPtr	pexception = NULL;
	SGeneratorPtr		pnew_generator = NULL;
	
	pnew_generator = calloc(1, sizeof(SGenerator));
	EXCCHECKALLOC( pnew_generator);
	InitializeCriticalSection(&(pnew_generator->lock));
	
	pnew_generator->generatorName = calloc(strlen(name) + 1, sizeof(char));
	EXCCHECKALLOC( pnew_generator->generatorName);
	strcpy(pnew_generator->generatorName, name);
	
	pnew_generator->Open		= generator_open;
	pnew_generator->Close		= generator_close;
	pnew_generator->Connect	= generator_connect;
	pnew_generator->Disconnect= generator_disconnect;
	pnew_generator->Write		= generator_write;
	pnew_generator->Read		= generator_read;
	pnew_generator->SetModeContinuous 	= generator_SetModeContinuous;
	pnew_generator->SetOutput		= generator_SetOutput;
	pnew_generator->SetFonction 		= generator_SetFonction ;
	pnew_generator->SystemError	= generator_SystemError;
	pnew_generator->ClearError		= generator_ClearError;

	if (generator)
		*generator = pnew_generator;
	
Error:
	if (pexception && pnew_generator)
		free(pnew_generator);
	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "generator_delete"
SElExceptionPtr generator_delete(SGeneratorPtr* generator)
{
	SElExceptionPtr	pexception = NULL;
	
	if (generator && *generator)
	{
		(*generator)->Close(*generator);
		
		if ((*generator)->generatorName)
			free((*generator)->generatorName);
		
		if ((*generator)->optionString)
			free((*generator)->optionString);
		
		DeleteCriticalSection( &((*generator)->lock) );
		
		free(*generator);
		*generator = NULL;
	}
	
Error:
	EXCRETHROW( pexception);
}

/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "generators_new"
SElExceptionPtr generators_new(SGeneratorsPtr*	generators)
{
	SElExceptionPtr	pexception = NULL;
	SGeneratorsPtr	pnew_generators = NULL;
	
	pnew_generators = calloc(1, sizeof(SGenerators));
	EXCCHECKALLOC( pnew_generators);
	
	pnew_generators->generators = NULL;
	pnew_generators->numGenerators = 0;
	pnew_generators->currentIndex = -1;
	
	pnew_generators->GetGeneratorByName			= generators_GetGeneratorByName;
	pnew_generators->GetGeneratorByIndex		= generators_GetGeneratorByIndex;
	pnew_generators->GetCurrentGenerator		= generators_GetCurrentGenerator;
	pnew_generators->SetCurrentGeneratorByName	= generators_SetCurrentGeneratorByName;
	pnew_generators->GetFirstGenerator			= generators_GetFirstGenerator;
	pnew_generators->GetNextGenerator			= generators_GetNextGenerator;
	pnew_generators->InitGenerators				= generators_InitGenerators;
	pnew_generators->CloseGenerators			= generators_CloseGenerators;
	
	if (generators)
		*generators = pnew_generators;
	
Error:
	if (pexception && pnew_generators)
		free(pnew_generators);
	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "generators_delete"
SElExceptionPtr generators_delete(SGeneratorsPtr* generators)
{
	SElExceptionPtr	pexception = NULL;
	
	if (generators && *generators)
	{
		EXCCHECK( (*generators)->CloseGenerators(*generators));
		
		free(*generators);
		*generators = NULL;
	}
	
Error:
	EXCRETHROW( pexception);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "{Generator}::Open"
static SElExceptionPtr generator_open(
		struct _Generator*	me,
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
													   "/Generator/Configuration",
													   &xml_configuration));
	if (xml_configuration == 0)
		EXCTHROW( -1, "Missing Configuration Node");
	
	/* Load Specific DLL functions */
	EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(xml_configuration, NULL, "@specificDll", &xml_node));
	if (xml_node == 0)
		EXCTHROW( -3, "Missing Generator Specific Library");
	
	EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(xml_node, NULL, &xml_value));
	me->specLib = LoadLibrary(xml_value);
	
	if (me->specLib == 0)
	{
		sprintf(message, "Generator Library (%s) Loading Failed", xml_value);
		EXCTHROW( -4, message);
	}
	
	me->_ConnectSpec	= (SElExceptionPtr (*)(SGeneratorPtr))
								GetProcAddress(me->specLib, "Connect");
	me->_DisconnectSpec	= (SElExceptionPtr (*)(SGeneratorPtr))
								GetProcAddress(me->specLib, "Disconnect");
	me->_WriteSpec		= (SElExceptionPtr (*)(SGeneratorPtr,const char*))
								GetProcAddress(me->specLib, "Write");
   	me->_ReadSpec		= (SElExceptionPtr (*)(SGeneratorPtr, const char*))
								GetProcAddress(me->specLib, "Read");
	me->_SetModeContinuousSpec		= (SElExceptionPtr (*)(SGeneratorPtr))
								GetProcAddress(me->specLib, "SetModeContinuous");
	me->_SetOutputSpec		= (SElExceptionPtr (*)(SGeneratorPtr, int))
								GetProcAddress(me->specLib, "SetOutput");
	me->_SetFonctionSpec		= (SElExceptionPtr (*)(SGeneratorPtr, double, double, double, double))
								GetProcAddress(me->specLib, "SetFonction");
	me->_SystemErrorSpec		= (SElExceptionPtr (*)(SGeneratorPtr, char*))
								GetProcAddress(me->specLib, "SystemError");
	me->_ClearErrorSpec		= (SElExceptionPtr (*)(SGeneratorPtr))
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
			EXCTHROW( -5, "Missing port generator name");
	
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
		me->_SetModeContinuousSpec		= NULL;
		me->_SetOutputSpec			= NULL;
		me->_SetFonctionSpec			= NULL;
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
#define __FUNC__ "{Generator}::Close"
static SElExceptionPtr generator_close(struct _Generator* me)
{
	SElExceptionPtr	pexception = NULL;
	
		me->_DisconnectSpec (me);
		
		me->_ConnectSpec	= NULL;
		me->_DisconnectSpec	= NULL;
		me->_WriteSpec		= NULL;
		me->_ReadSpec		= NULL;
		me->_SetModeContinuousSpec		= NULL;
		me->_SetOutputSpec			= NULL;
		me->_SetFonctionSpec			= NULL;
		me->_SystemErrorSpec		= NULL;
		me->_ClearErrorSpec			= NULL;
	
Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "fncLock"
static SElExceptionPtr generatorLock(SGeneratorPtr me)
{
	EnterCriticalSection( &me->lock);    
	return NULL;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "generatorUnLock"
static SElExceptionPtr generatorUnLock(SGeneratorPtr me)
{
	LeaveCriticalSection( &me->lock);   
	return NULL;
}

#undef __FUNC__
#define __FUNC__ "{Generator}::Connect"
static SElExceptionPtr generator_connect(SGeneratorPtr me)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_ConnectSpec)
		EXCCHECK( me->_ConnectSpec(me));
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Generator}::Disconnect"
static SElExceptionPtr generator_disconnect(SGeneratorPtr me)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_ConnectSpec)
		EXCCHECK( me->_DisconnectSpec(me));
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Generator}::Write"
static SElExceptionPtr generator_write(SGeneratorPtr me, const char* command)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_WriteSpec)
		EXCCHECK( me->_WriteSpec(me, command));
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Generator}::Read"
static SElExceptionPtr generator_read(SGeneratorPtr me, char *read)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_ReadSpec)
		EXCCHECK( me->_ReadSpec(me, read));
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Generator}::SetModeContinuous "
static SElExceptionPtr generator_SetModeContinuous (SGeneratorPtr me)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_SetModeContinuousSpec)
		EXCCHECK( me->_SetModeContinuousSpec(me));
	
Error:
	EXCRETHROW( pexception);
}
#undef __FUNC__
#define __FUNC__ "{Generator}::SetOutput"
static SElExceptionPtr generator_SetOutput(SGeneratorPtr me, int output)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_SetOutputSpec)
		EXCCHECK( me->_SetOutputSpec(me, output));
	
Error:
	EXCRETHROW( pexception);
}
#undef __FUNC__
#define __FUNC__ "{Generator}::SetFonction "
static SElExceptionPtr generator_SetFonction (SGeneratorPtr me, double frequency, double amplitude, double offset, double duty)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_SetFonctionSpec)
		EXCCHECK( me->_SetFonctionSpec(me, frequency, amplitude, offset, duty));
	
Error:
	EXCRETHROW( pexception);
}
#undef __FUNC__
#define __FUNC__ "{Generator}::SystemError"
static SElExceptionPtr generator_SystemError(SGeneratorPtr me, char *error)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_SystemErrorSpec)
		EXCCHECK( me->_SystemErrorSpec(me, error));
	
Error:
	EXCRETHROW( pexception);
}
#undef __FUNC__
#define __FUNC__ "{Generator}::ClearError"
static SElExceptionPtr generator_ClearError(SGeneratorPtr me)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_ClearErrorSpec)
		EXCCHECK( me->_ClearErrorSpec(me));
	
Error:
	EXCRETHROW( pexception);
}

/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "{Generators}::GetGeneratorByName"
static SElExceptionPtr generators_GetGeneratorByName(
	struct _Generators*	me,
	const char*			generatorName,
	SGeneratorPtr*		generator
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			i = 0;
	SGeneratorPtr		found_generator = NULL;
	char			message[1024] = "";
	
	for (i = 0; i < me->numGenerators; i++)
	{
		if (!strcmp(me->generators[i]->generatorName, generatorName))
		{
			found_generator = me->generators[i];
			break;
		}
	}
	
	if (generator)
		*generator = found_generator;
	
	if (found_generator == NULL)
	{
		sprintf(message, "Generator Name (%s) Not Found", generatorName);
		EXCTHROW( -12, message);
	}
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Generators}::GetGeneratorByIndex"
static SElExceptionPtr generators_GetGeneratorByIndex(
	struct _Generators*	me,
	int32_t				index,
	SGeneratorPtr*		generator
)
{
	SElExceptionPtr	pexception = NULL;
	
	if (index < me->numGenerators && index >= 0)
	{
		if (generator)
			*generator = me->generators[index];
	}
	else
	{
		if (generator)
			*generator = NULL;
		
		EXCTHROW( -13, "Index Out of Range");
	}
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Generators}::GetCurrentGenerator"
static SElExceptionPtr generators_GetCurrentGenerator(
	struct _Generators*	me,
	SGeneratorPtr*		generator
)
{
	SElExceptionPtr	pexception = NULL;
	
	if (me->currentIndex >= 0 && me->numGenerators > 0)
	{
		if (generator)
			*generator = me->generators[me->currentIndex];
	}
	else
	{
		if (generator)
			*generator = NULL;
	}

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Generators}::SetCurrentGeneratorByName"
static SElExceptionPtr generators_SetCurrentGeneratorByName(
	struct _Generators*	me,
	const char*			generatorName
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			i = 0;
	SGeneratorPtr		generator = NULL;
	char			message[1024] = "";

	for (i = 0; i < me->numGenerators; i++)
	{
		if (!strcmp(me->generators[i]->generatorName, generatorName))
		{
			me->currentIndex = i;
			break;
		}
	}
	
	if (i >= me->numGenerators)
	{
		sprintf(message, "Generator Name (%s) Not Found", generatorName);
		EXCTHROW( -12, message);
	}

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Generators}::GetFirstGenerator"
static SElExceptionPtr generators_GetFirstGenerator(
	struct _Generators*	me,
	SGeneratorPtr*		generator
)
{
	SElExceptionPtr	pexception = NULL;
	
	me->currentIndex = 0;

	if(me->numGenerators > 0)
	{
		if (generator)
			*generator = me->generators[0];
	}
	else
	{
		if (generator)
			*generator = NULL;
	}

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Generators}::GetNextGenerator"
static SElExceptionPtr generators_GetNextGenerator(
	struct _Generators*	me,
	SGeneratorPtr*		generator
)
{
	SElExceptionPtr	pexception = NULL;

	if (++(me->currentIndex) >= me->numGenerators)
	{
		me->currentIndex = me->numGenerators - 1;
		if (generator)
			*generator = NULL;
	}
	else
	{
		if (generator)
			*generator = me->generators[me->currentIndex];
	}
	
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Generators}::InitGenerators"
static SElExceptionPtr generators_InitGenerators(struct _Generators* me)
{
	SElExceptionPtr	pexception = NULL;
	HRESULT			hr = S_OK;
	VARIANT_BOOL	is_xml_success = 1;
	VARIANT			file;
	CAObjHandle		document = 0;
	CAObjHandle		generator = 0;
	CAObjHandle		generator_name_node = 0;
	CAObjHandle		generator_list = 0;
	char*			xml_value = NULL;
	char*			generator_xml = NULL;
	
	EXCCHECKCOM( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &document));
	
	CA_VariantSetCString (&file, "modules/generators.xml");
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentload(document, NULL, file, &is_xml_success));
	
	EXCCHECKCOM( MSXML_IXMLDOMDocumentselectNodes(document, NULL, "/Generators/Generator", &generator_list));
	EXCCHECKCOM( MSXML_IXMLDOMNodeListreset (generator_list, NULL));
	
	EXCCHECKCOM( MSXML_IXMLDOMNodeListnextNode(generator_list, NULL, &generator));
	while (generator)
	{
		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(generator, NULL, "@name", &generator_name_node));
		EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(generator_name_node, NULL, &xml_value));
		EXCCHECKCOM( MSXML_IXMLDOMNodeGetxml(generator, NULL, &generator_xml));
		
		me->generators = realloc( me->generators, (me->numGenerators + 1) * sizeof(SGeneratorPtr));
		EXCCHECKALLOC( me->generators);
		
		EXCCHECK( generator_new( &me->generators[me->numGenerators], xml_value, NULL));
		EXCCHECK( me->generators[me->numGenerators]->Open(me->generators[me->numGenerators], NULL, generator_xml));
		me->numGenerators++;
		
		CA_FREE_MEMORY(xml_value);
		CA_FREE_MEMORY(generator_xml);
		DISCARD_HANDLE(generator_name_node);
		DISCARD_HANDLE(generator);
		
		EXCCHECKCOM( MSXML_IXMLDOMNodeListnextNode(generator_list, NULL, &generator));
	}
	
Error:
	CA_FREE_MEMORY(xml_value);
	CA_FREE_MEMORY(generator_xml);
	DISCARD_HANDLE(generator_name_node);
	DISCARD_HANDLE(generator);
	DISCARD_HANDLE(generator_list);
	DISCARD_HANDLE(document);
	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Generators}::CloseGenerators"
static SElExceptionPtr generators_CloseGenerators(struct _Generators* me)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			i = 0;
	
	for (i = 0; i < me->numGenerators; i++)
	{
		EXCCHECK( generator_delete(&(me->generators[i])));
	}
	free(me->generators);
	me->generators = NULL;
	me->numGenerators = 0;
	
Error:
	EXCRETHROW( pexception);
}
