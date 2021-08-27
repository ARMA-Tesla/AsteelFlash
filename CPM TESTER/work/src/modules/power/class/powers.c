#include "powers.h"
#include <string.h>
#include <stdio.h>
#include <cviauto.h>
#include "msxmldom.h"
#include <utility.h>
#include "Ag34450.h"

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

static SElExceptionPtr power_open(struct _Power* me, const char* resource, const char* optionString);
static SElExceptionPtr power_close(struct _Power* me);


static SElExceptionPtr power_initialize(struct _Power* me);
static SElExceptionPtr power_disconnect(struct _Power* me);
static SElExceptionPtr power_ConfigureCurrentLimit (SPowerPtr me, double CurrentLimit);
static SElExceptionPtr power_ConfigureOutput (SPowerPtr me, double VoltageLevel);
static SElExceptionPtr power_ConfigureOVP (SPowerPtr me, double VoltageLimit);
static SElExceptionPtr power_ConfigureOCP (SPowerPtr me, double CurrentLimit);
static SElExceptionPtr power_EnableOutput(SPowerPtr me, int32_t EnableOutput);

/*****************************************************************************/

static SElExceptionPtr powers_GetPowerByName(
	struct _Powers*	me,
	const char*			powerName,
	SPowerPtr*		power
);
static SElExceptionPtr powers_GetPowerByIndex(
	struct _Powers*	me,
	int32_t				index,
	SPowerPtr*		power
);
static SElExceptionPtr powers_GetCurrentPower(
	struct _Powers*	me,
	SPowerPtr*		power
);
static SElExceptionPtr powers_SetCurrentPowerByName(
	struct _Powers*	me,
	const char*			powerName
);
static SElExceptionPtr powers_GetFirstPower(
	struct _Powers*	me,
	SPowerPtr*		power
);
static SElExceptionPtr powers_GetNextPower(
	struct _Powers*	me,
	SPowerPtr*		power
);
	
static SElExceptionPtr powers_InitPowers(struct _Powers* me);
static SElExceptionPtr powers_ClosePowers(struct _Powers* me);

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "power_new"
SElExceptionPtr power_new(
	SPowerPtr*	power,
	const char*		name,
	const char*		optionString
)
{
	SElExceptionPtr	pexception = NULL;
	SPowerPtr		pnew_power = NULL;
	
	pnew_power = calloc(1, sizeof(SPower));
	EXCCHECKALLOC( pnew_power);
	InitializeCriticalSection(&(pnew_power->lock));
	
	pnew_power->powerName = calloc(strlen(name) + 1, sizeof(char));
	EXCCHECKALLOC( pnew_power->powerName);
	strcpy(pnew_power->powerName, name);
	
	pnew_power->Open		= power_open;
	pnew_power->Close		= power_close;
	pnew_power->Initialize	= power_initialize;
	pnew_power->Disconnect		= power_disconnect;
	pnew_power->ConfigureCurrentLimit 	= power_ConfigureCurrentLimit;
	pnew_power->ConfigureOutput 	= power_ConfigureOutput;
	pnew_power->ConfigureOVP 	= power_ConfigureOVP;
	pnew_power->ConfigureOCP 	= power_ConfigureOCP;
	pnew_power->EnableOutput		= power_EnableOutput;
	
	if (power)
		*power = pnew_power;
	
Error:
	if (pexception && pnew_power)
		free(pnew_power);
	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "power_delete"
SElExceptionPtr power_delete(SPowerPtr* power)
{
	SElExceptionPtr	pexception = NULL;
	
	if (power && *power)
	{
		(*power)->Close(*power);
		
		if ((*power)->powerName)
			free((*power)->powerName);
		
		if ((*power)->optionString)
			free((*power)->optionString);
		
		DeleteCriticalSection( &((*power)->lock) );
		
		free(*power);
		*power = NULL;
	}
	
Error:
	EXCRETHROW( pexception);
}

/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "powers_new"
SElExceptionPtr powers_new(SPowersPtr*	powers)
{
	SElExceptionPtr	pexception = NULL;
	SPowersPtr	pnew_powers = NULL;
	
	pnew_powers = calloc(1, sizeof(SPowers));
	EXCCHECKALLOC( pnew_powers);
	
	pnew_powers->powers = NULL;
	pnew_powers->numPowers = 0;
	pnew_powers->currentIndex = -1;
	
	pnew_powers->GetPowerByName			= powers_GetPowerByName;
	pnew_powers->GetPowerByIndex		= powers_GetPowerByIndex;
	pnew_powers->GetCurrentPower		= powers_GetCurrentPower;
	pnew_powers->SetCurrentPowerByName	= powers_SetCurrentPowerByName;
	pnew_powers->GetFirstPower			= powers_GetFirstPower;
	pnew_powers->GetNextPower			= powers_GetNextPower;
	pnew_powers->InitPowers				= powers_InitPowers;
	pnew_powers->ClosePowers			= powers_ClosePowers;
	
	if (powers)
		*powers = pnew_powers;
	
Error:
	if (pexception && pnew_powers)
		free(pnew_powers);
	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "powers_delete"
SElExceptionPtr powers_delete(SPowersPtr* powers)
{
	SElExceptionPtr	pexception = NULL;
	
	if (powers && *powers)
	{
		EXCCHECK( (*powers)->ClosePowers(*powers));
		
		free(*powers);
		*powers = NULL;
	}
	
Error:
	EXCRETHROW( pexception);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "{Power}::Open"
static SElExceptionPtr power_open(
		struct _Power*	me,
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
													   "/Power/Configuration",
													   &xml_configuration));
	if (xml_configuration == 0)
		EXCTHROW( -1, "Missing Configuration Node");
	
	/* Load Specific DLL functions */
	EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(xml_configuration, NULL, "@specificDll", &xml_node));
	if (xml_node == 0)
		EXCTHROW( -3, "Missing Power Specific Library");
	
	EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(xml_node, NULL, &xml_value));
	me->specLib = LoadLibrary(xml_value);
	
	if (me->specLib == 0)
	{
		sprintf(message, "Power Library (%s) Loading Failed", xml_value);
		EXCTHROW( -4, message);
	}
	
	me->_InitializeSpec	= (SElExceptionPtr (*)(SPowerPtr, ViRsrc* , bool_t, bool_t, uint32_t, ViSession*))
								GetProcAddress(me->specLib, "BKPrecisionPVSSeries_Initialize");
	
	me->_DisconnectSpec	= (SElExceptionPtr (*)(SPowerPtr, int32_t*))
								GetProcAddress(me->specLib, "BKPrecisionPVSSeries_Close");
	
	me->_ConfigureCurrentLimitSpec		= (SElExceptionPtr (*)(SPowerPtr, uintptr_t *, double, int32_t, uint16_t))
								GetProcAddress(me->specLib, "BKPrecisionPVSSeries_ConfigureCurrentLimit");
	
	me->_EnableOutputSpec		= (SElExceptionPtr (*)(SPowerPtr, uintptr_t *, int32_t, uint16_t, int32_t))
								GetProcAddress(me->specLib, "BKPrecisionPVSSeries_EnableOutput");
	
	me->_ConfigureOutputSpec	= (SElExceptionPtr (*)(SPowerPtr, uintptr_t *, double, int32_t, uint16_t, int32_t))
								GetProcAddress(me->specLib, "BKPrecisionPVSSeries_ConfigureOutput");
	
	me->_ConfigureOVPSpec		= (SElExceptionPtr (*)(SPowerPtr, uintptr_t *, double, int32_t, uint16_t, int32_t))
								GetProcAddress(me->specLib, "BKPrecisionPVSSeries_ConfigureOVP");
	
	me->_ConfigureOCPSpec		= (SElExceptionPtr (*)(SPowerPtr, uintptr_t *, double, int32_t, uint16_t, int32_t))
								GetProcAddress(me->specLib, "BKPrecisionPVSSeries_ConfigureOCP");	
	

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
													   "ResourceName", &xml_node));
		if (xml_node == 0)
			EXCTHROW( -5, "Missing power ResourceName");
	
		if (xml_node)
		{
			EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(xml_node, NULL, &xml_value));
			sprintf (me->VISAResourceName, xml_value); 
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
		
		me->_InitializeSpec	= NULL;
		me->_DisconnectSpec	= NULL;
		me->_ConfigureCurrentLimitSpec		= NULL;
		me->_EnableOutputSpec			= NULL;

	}
		
	DISCARD_HANDLE(xml_node);
	CA_FREE_MEMORY(xml_value);
	CA_FREE_MEMORY(xml_type);
	DISCARD_HANDLE(xml_configuration);
	DISCARD_HANDLE(document);
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Power}::Close"
static SElExceptionPtr power_close(struct _Power* me)
{
	SElExceptionPtr	pexception = NULL;
	
		me->_DisconnectSpec (me, me->VISAResourceNameOut);
		
		me->_InitializeSpec	= NULL;
		me->_DisconnectSpec	= NULL;
		me->_ConfigureCurrentLimitSpec		= NULL;
		me->_EnableOutputSpec			= NULL;
	
Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "fncLock"
static SElExceptionPtr powerLock(SPowerPtr me)
{
	EnterCriticalSection( &me->lock);    
	return NULL;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "powerUnLock"
static SElExceptionPtr powerUnLock(SPowerPtr me)
{
	LeaveCriticalSection( &me->lock);   
	return NULL;
}

#undef __FUNC__
#define __FUNC__ "{Power}::Initialize"
static SElExceptionPtr power_initialize(SPowerPtr me)
{
	SElExceptionPtr	pexception = NULL;
	ViRsrc    BKresourceName    ="ASRL6::INSTR";
	ViSession vi_PVS 		   = 0;
	
	if (me->_InitializeSpec)
		EXCCHECK( me->_InitializeSpec(me, BKresourceName, TRUE, TRUE, 9600, &vi_PVS));//
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Power}::Disconnect"
static SElExceptionPtr power_disconnect(SPowerPtr me)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_DisconnectSpec)
		EXCCHECK( me->_DisconnectSpec(me, me->VISAResourceNameOut));
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Power}::ConfigureCurrentLimit "
static SElExceptionPtr power_ConfigureCurrentLimit (SPowerPtr me, double CurrentLimit)
{
	SElExceptionPtr	pexception = NULL;
	
	if (me->_ConfigureCurrentLimitSpec)
		EXCCHECK( me->_ConfigureCurrentLimitSpec(me, me->VISAResourceNameOut, CurrentLimit, 0x01, TRUE));
	
Error:
	EXCRETHROW( pexception);
}	
#undef __FUNC__
#define __FUNC__ "{Power}::ConfigureOutput "
static SElExceptionPtr power_ConfigureOutput (SPowerPtr me, double VoltageLevel)
{
	SElExceptionPtr	pexception = NULL;
	
	if (me->_ConfigureOutputSpec)
		EXCCHECK( me->_ConfigureOutputSpec(me, me->VISAResourceNameOut, VoltageLevel, 0x01, TRUE, TRUE));
Error:
	EXCRETHROW( pexception);
}	
#undef __FUNC__
#define __FUNC__ "{Power}::ConfigureOVP "
static SElExceptionPtr power_ConfigureOVP (SPowerPtr me, double VoltageLimit)
{
	SElExceptionPtr	pexception = NULL;
	
	if (me->_ConfigureOVPSpec)
		EXCCHECK( me->_ConfigureOVPSpec(me, me->VISAResourceNameOut, VoltageLimit, 0x01, TRUE, TRUE));
	
Error:
	EXCRETHROW( pexception);
}
#undef __FUNC__
#define __FUNC__ "{Power}::ConfigureOCP "
static SElExceptionPtr power_ConfigureOCP (SPowerPtr me, double CurrentLimit)
{
	SElExceptionPtr	pexception = NULL;
	
	if (me->_ConfigureOCPSpec)
		EXCCHECK( me->_ConfigureOCPSpec(me, me->VISAResourceNameOut, CurrentLimit, 0x01, TRUE, TRUE));
	
Error:
	EXCRETHROW( pexception);
}
#undef __FUNC__
#define __FUNC__ "{Power}::EnableOutput"
static SElExceptionPtr power_EnableOutput(SPowerPtr me, int32_t EnableOutput)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_EnableOutputSpec)
		EXCCHECK( me->_EnableOutputSpec(me, me->VISAResourceNameOut, EnableOutput, 0x01, TRUE));
	
Error:
	EXCRETHROW( pexception);
}

/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "{Powers}::GetPowerByName"
static SElExceptionPtr powers_GetPowerByName(
	struct _Powers*	me,
	const char*			powerName,
	SPowerPtr*		power
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			i = 0;
	SPowerPtr		found_power = NULL;
	char			message[1024] = "";
	
	for (i = 0; i < me->numPowers; i++)
	{
		if (!strcmp(me->powers[i]->powerName, powerName))
		{
			found_power = me->powers[i];
			break;
		}
	}
	
	if (power)
		*power = found_power;
	
	if (found_power == NULL)
	{
		sprintf(message, "Power Name (%s) Not Found", powerName);
		EXCTHROW( -12, message);
	}
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Powers}::GetPowerByIndex"
static SElExceptionPtr powers_GetPowerByIndex(
	struct _Powers*	me,
	int32_t				index,
	SPowerPtr*		power
)
{
	SElExceptionPtr	pexception = NULL;
	
	if (index < me->numPowers && index >= 0)
	{
		if (power)
			*power = me->powers[index];
	}
	else
	{
		if (power)
			*power = NULL;
		
		EXCTHROW( -13, "Index Out of Range");
	}
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Powers}::GetCurrentPower"
static SElExceptionPtr powers_GetCurrentPower(
	struct _Powers*	me,
	SPowerPtr*		power
)
{
	SElExceptionPtr	pexception = NULL;
	
	if (me->currentIndex >= 0 && me->numPowers > 0)
	{
		if (power)
			*power = me->powers[me->currentIndex];
	}
	else
	{
		if (power)
			*power = NULL;
	}

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Powers}::SetCurrentPowerByName"
static SElExceptionPtr powers_SetCurrentPowerByName(
	struct _Powers*	me,
	const char*			powerName
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			i = 0;
	SPowerPtr		power = NULL;
	char			message[1024] = "";

	for (i = 0; i < me->numPowers; i++)
	{
		if (!strcmp(me->powers[i]->powerName, powerName))
		{
			me->currentIndex = i;
			break;
		}
	}
	
	if (i >= me->numPowers)
	{
		sprintf(message, "Power Name (%s) Not Found", powerName);
		EXCTHROW( -12, message);
	}

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Powers}::GetFirstPower"
static SElExceptionPtr powers_GetFirstPower(
	struct _Powers*	me,
	SPowerPtr*		power
)
{
	SElExceptionPtr	pexception = NULL;
	
	me->currentIndex = 0;

	if(me->numPowers > 0)
	{
		if (power)
			*power = me->powers[0];
	}
	else
	{
		if (power)
			*power = NULL;
	}

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Powers}::GetNextPower"
static SElExceptionPtr powers_GetNextPower(
	struct _Powers*	me,
	SPowerPtr*		power
)
{
	SElExceptionPtr	pexception = NULL;

	if (++(me->currentIndex) >= me->numPowers)
	{
		me->currentIndex = me->numPowers - 1;
		if (power)
			*power = NULL;
	}
	else
	{
		if (power)
			*power = me->powers[me->currentIndex];
	}
	
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Powers}::InitPowers"
static SElExceptionPtr powers_InitPowers(struct _Powers* me)
{
	SElExceptionPtr	pexception = NULL;
	HRESULT			hr = S_OK;
	VARIANT_BOOL	is_xml_success = 1;
	VARIANT			file;
	CAObjHandle		document = 0;
	CAObjHandle		power = 0;
	CAObjHandle		power_name_node = 0;
	CAObjHandle		power_list = 0;
	char*			xml_value = NULL;
	char*			power_xml = NULL;
	
	EXCCHECKCOM( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &document));
	
	CA_VariantSetCString (&file, "modules/powers.xml");
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentload(document, NULL, file, &is_xml_success));
	
	EXCCHECKCOM( MSXML_IXMLDOMDocumentselectNodes(document, NULL, "/Powers/Power", &power_list));
	EXCCHECKCOM( MSXML_IXMLDOMNodeListreset (power_list, NULL));
	
	EXCCHECKCOM( MSXML_IXMLDOMNodeListnextNode(power_list, NULL, &power));
	while (power)
	{
		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(power, NULL, "@name", &power_name_node));
		EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(power_name_node, NULL, &xml_value));
		EXCCHECKCOM( MSXML_IXMLDOMNodeGetxml(power, NULL, &power_xml));
		
		me->powers = realloc( me->powers, (me->numPowers + 1) * sizeof(SPowerPtr));
		EXCCHECKALLOC( me->powers);
		
		EXCCHECK( power_new( &me->powers[me->numPowers], xml_value, NULL));
		EXCCHECK( me->powers[me->numPowers]->Open(me->powers[me->numPowers], NULL, power_xml));
		me->numPowers++;
		
		CA_FREE_MEMORY(xml_value);
		CA_FREE_MEMORY(power_xml);
		DISCARD_HANDLE(power_name_node);
		DISCARD_HANDLE(power);
		
		EXCCHECKCOM( MSXML_IXMLDOMNodeListnextNode(power_list, NULL, &power));
	}
	
Error:
	CA_FREE_MEMORY(xml_value);
	CA_FREE_MEMORY(power_xml);
	DISCARD_HANDLE(power_name_node);
	DISCARD_HANDLE(power);
	DISCARD_HANDLE(power_list);
	DISCARD_HANDLE(document);
	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Powers}::ClosePowers"
static SElExceptionPtr powers_ClosePowers(struct _Powers* me)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			i = 0;
	
	for (i = 0; i < me->numPowers; i++)
	{
		EXCCHECK( power_delete(&(me->powers[i])));
	}
	free(me->powers);
	me->powers = NULL;
	me->numPowers = 0;
	
Error:
	EXCRETHROW( pexception);
}
