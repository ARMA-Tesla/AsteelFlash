#include "axis.h"
#include <string.h>
#include <stdio.h>
#include <cviauto.h>
#include "msxmldom.h"
#include <utility.h>
#include <rs232.h>

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

static SElExceptionPtr axis_open(
		struct _axis*	me,
		const char*			resource,
		const char*			optionString
);
static SElExceptionPtr axis_close(struct _axis* me);

static SElExceptionPtr axis_write(SaxisPtr me, const char* command);

static SElExceptionPtr axis_PresentAlarm(SaxisPtr me, int32_t axis, int32_t* status);
static SElExceptionPtr axis_ControllerStatus(SaxisPtr me, int32_t axis, int32_t* status);
static SElExceptionPtr axis_EnableAxis(SaxisPtr me, int32_t axis);
static SElExceptionPtr axis_AlarmReset(SaxisPtr me, int32_t axis);
static SElExceptionPtr axis_HomeReturn(SaxisPtr me, int32_t axis);
static SElExceptionPtr axis_MoveAxisPosition(SaxisPtr me, int32_t axis, int32_t position);
static SElExceptionPtr axis_StopAxis(SaxisPtr me, int32_t axis);
static SElExceptionPtr axis_SetPosition(SaxisPtr me, int32_t axis, double position, double speed);
static SElExceptionPtr axis_CurrentPosition(SaxisPtr me, int32_t axis, double* position);
static SElExceptionPtr axis_RequestPosition(SaxisPtr me, int32_t axis);
static SElExceptionPtr axis_ReadPosition(SaxisPtr me, int32_t axis, double* position);




DWORD WINAPI Thread_axis(
  LPVOID lpParameter
);

/*****************************************************************************/

static SElExceptionPtr axiss_GetaxisByName(
	struct _axiss*	me,
	const char*			axisName,
	SaxisPtr*		axis
);
static SElExceptionPtr axiss_GetaxisByIndex(
	struct _axiss*	me,
	int32_t				index,
	SaxisPtr*		axis
);
static SElExceptionPtr axiss_GetCurrentaxis(
	struct _axiss*	me,
	SaxisPtr*		axis
);
static SElExceptionPtr axiss_SetCurrentaxisByName(
	struct _axiss*	me,
	const char*			axisName
);
static SElExceptionPtr axiss_GetFirstaxis(
	struct _axiss*	me,
	SaxisPtr*		axis
);
static SElExceptionPtr axiss_GetNextaxis(
	struct _axiss*	me,
	SaxisPtr*		axis
);
	
static SElExceptionPtr axiss_Initaxiss(struct _axiss* me);
static SElExceptionPtr axiss_Closeaxiss(struct _axiss* me);

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "axis_new"
SElExceptionPtr axis_new(
	SaxisPtr*	axis,
	const char*		name,
	const char*		optionString
)
{
	SElExceptionPtr	pexception = NULL;
	SaxisPtr		pnew_axis = NULL;
	
	pnew_axis = calloc(1, sizeof(Saxis));
	EXCCHECKALLOC( pnew_axis);
	InitializeCriticalSection(&(pnew_axis->lock));
	
	pnew_axis->axisName = calloc(strlen(name) + 1, sizeof(char));
	EXCCHECKALLOC( pnew_axis->axisName);
	strcpy(pnew_axis->axisName, name);
	
	pnew_axis->Open		= axis_open;
	pnew_axis->Close		= axis_close;
	
	pnew_axis->Write		= axis_write;
	pnew_axis->PresentAlarm	= axis_PresentAlarm;
	pnew_axis->ControllerStatus	= axis_ControllerStatus;
	pnew_axis->EnableAxis		= axis_EnableAxis;
	pnew_axis->AlarmReset			= axis_AlarmReset;
	pnew_axis->HomeReturn			= axis_HomeReturn;
	pnew_axis->MoveAxisPosition			= axis_MoveAxisPosition;
	pnew_axis->StopAxis					= axis_StopAxis;
	pnew_axis->SetPosition			= axis_SetPosition;
	pnew_axis->CurrentPosition			= axis_CurrentPosition;
	pnew_axis->RequestPosition			= axis_RequestPosition;
	pnew_axis->ReadPosition				= axis_ReadPosition;

	pnew_axis->readEvent = CreateEvent(NULL, 1, 0, NULL);
	
	if (axis)
		*axis = pnew_axis;
	
Error:
	if (pexception && pnew_axis)
		free(pnew_axis);
	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "axis_delete"
SElExceptionPtr axis_delete(SaxisPtr* axis)
{
	SElExceptionPtr	pexception = NULL;
	
	if (axis && *axis)
	{
		(*axis)->Close(*axis);
		
		if ((*axis)->axisName)
			free((*axis)->axisName);
		
		if ((*axis)->optionString)
			free((*axis)->optionString);
		
		DeleteCriticalSection( &((*axis)->lock) );
		
		free(*axis);
		*axis = NULL;
	}
	
Error:
	EXCRETHROW( pexception);
}

/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "axiss_new"
SElExceptionPtr axiss_new(SaxissPtr*	axiss)
{
	SElExceptionPtr	pexception = NULL;
	SaxissPtr	pnew_axiss = NULL;
	
	pnew_axiss = calloc(1, sizeof(Saxiss));
	EXCCHECKALLOC( pnew_axiss);
	
	pnew_axiss->axiss = NULL;
	pnew_axiss->numaxiss = 0;
	pnew_axiss->currentIndex = -1;
	
	pnew_axiss->GetaxisByName			= axiss_GetaxisByName;
	pnew_axiss->GetaxisByIndex		= axiss_GetaxisByIndex;
	pnew_axiss->GetCurrentaxis		= axiss_GetCurrentaxis;
	pnew_axiss->SetCurrentaxisByName	= axiss_SetCurrentaxisByName;
	pnew_axiss->GetFirstaxis			= axiss_GetFirstaxis;
	pnew_axiss->GetNextaxis			= axiss_GetNextaxis;
	pnew_axiss->Initaxiss				= axiss_Initaxiss;
	pnew_axiss->Closeaxiss			= axiss_Closeaxiss;
	
	if (axiss)
		*axiss = pnew_axiss;
	
Error:
	if (pexception && pnew_axiss)
		free(pnew_axiss);
	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "axiss_delete"
SElExceptionPtr axiss_delete(SaxissPtr* axiss)
{
	SElExceptionPtr	pexception = NULL;
	
	if (axiss && *axiss)
	{
		EXCCHECK( (*axiss)->Closeaxiss(*axiss));
		
		free(*axiss);
		*axiss = NULL;
	}
	
Error:
	EXCRETHROW( pexception);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "{axis}::Open"
static SElExceptionPtr axis_open(
		struct _axis*	me,
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
	int		        MaxAxis = 0;
	int				i;
		
	if (me->fd)
	{
		EXCTHROW( -11, "axis Allready Open");
	}
	
	me->optionString = calloc(strlen(optionString) + 1, sizeof(char));
	EXCCHECKALLOC( me->optionString);
	strcpy(me->optionString, optionString);
	
	EXCCHECKCOM( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &document));
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentloadXML (document, NULL, optionString, &is_xml_success));
	
	/* get Max Axis */	
	EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(document, NULL,"/Axis/MaxAxis", &xml_node));
		if (xml_node)
		{
			EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(xml_node, NULL, &xml_value));
			MaxAxis = atoi(xml_value);
			CA_FREE_MEMORY(xml_value);
		}
		
	/* get Configuration */	
	EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode(document, NULL,
													   "/Axis/Configuration",
													   &xml_configuration));
	if (xml_configuration == 0)
		EXCTHROW( -1, "Missing Configuration Node");
	
	/* Load Specific DLL functions */
	EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(xml_configuration, NULL, "@specificDll", &xml_node));
	if (xml_node == 0)
		EXCTHROW( -3, "Missing axis Specific Library");
	
	EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(xml_node, NULL, &xml_value));
	me->specLib = LoadLibrary(xml_value);
	if (me->specLib == 0)
	{
		sprintf(message, "axis Library (%s) Loading Failed", xml_value);
		EXCTHROW( -4, message);
	}
	
	me->_InitializeSpec	= (SElExceptionPtr (*)(SaxisPtr, int32_t))
								GetProcAddress(me->specLib, "Initialize");
	me->_PresentAlarmSpec	= (SElExceptionPtr (*)(SaxisPtr, int32_t, int32_t*))
								GetProcAddress(me->specLib, "PresentAlarm");
	me->_ControllerStatusSpec	= (SElExceptionPtr (*)(SaxisPtr, int32_t, int32_t*))
								GetProcAddress(me->specLib, "ControllerStatus");
	me->_EnableAxisSpec= (SElExceptionPtr (*)(SaxisPtr, int32_t))
								GetProcAddress(me->specLib, "EnableAxis");
	me->_AlarmResetSpec= (SElExceptionPtr (*)(SaxisPtr, int32_t))
								GetProcAddress(me->specLib, "AlarmReset");
	me->_HomeReturnSpec= (SElExceptionPtr (*)(SaxisPtr, int32_t))
								GetProcAddress(me->specLib, "HomeReturn");
	me->_StopAxisSpec= (SElExceptionPtr (*)(SaxisPtr, int32_t))
								GetProcAddress(me->specLib, "StopAxis");
	me->_MoveAxisPositionSpec = (SElExceptionPtr (*)(SaxisPtr, int32_t, int32_t))
								GetProcAddress(me->specLib, "MoveAxisPosition");
	me->_CurrentPositionSpec = (SElExceptionPtr (*)(SaxisPtr, int32_t, double*))
								GetProcAddress(me->specLib, "CurrentPosition");
	
	me->_RequestPositionSpec = (SElExceptionPtr (*)(SaxisPtr, int32_t))
								GetProcAddress(me->specLib, "RequestPosition");
		
	me->_ReadPositionSpec = (SElExceptionPtr (*)(SaxisPtr, int32_t, double*))
								GetProcAddress(me->specLib, "ReadPosition");
	
	me->_SetPositionSpec = (SElExceptionPtr (*)(SaxisPtr, int32_t, double, double))
								GetProcAddress(me->specLib, "SetPosition");
	me->_WriteSpec		= (SElExceptionPtr (*)(SaxisPtr,const char*))
								GetProcAddress(me->specLib, "Write");
	me->_callbackThreadSpec= (SElExceptionPtr (*)(SaxisPtr))
								GetProcAddress(me->specLib, "CallbackThread");
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
		DCB		dcb;
		
		dcb.BaudRate	= 9600;
		dcb.ByteSize	= 8;
		dcb.Parity		= NOPARITY;
		dcb.StopBits	= ONESTOPBIT;
		
		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(xml_configuration, NULL,
													   "port", &xml_node));
		if (xml_node)
		{
			EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(xml_node, NULL, &xml_value));
			me->port = atol(xml_value);
			CA_FREE_MEMORY(xml_value);
		}
		
		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(xml_configuration, NULL,
													   "BaudRate", &xml_node));
		if (xml_node)
		{
			EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(xml_node, NULL, &xml_value));
			dcb.BaudRate = atol(xml_value);
			CA_FREE_MEMORY(xml_value);
		}
		
		DISCARD_HANDLE(xml_node);
		
		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(xml_configuration, NULL,
													   "DataBits", &xml_node));
		if (xml_node)
		{
			EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(xml_node, NULL, &xml_value));
			dcb.ByteSize = atol(xml_value);
			CA_FREE_MEMORY(xml_value);
		}
		
		DISCARD_HANDLE(xml_node);
		
		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(xml_configuration, NULL,
													   "StopBits", &xml_node));
		if (xml_node)
		{
			EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(xml_node, NULL, &xml_value));
			if (!strcmp(xml_value, "1"))
				dcb.StopBits = 1;
			else if (!strcmp(xml_value, "2"))
				dcb.StopBits = 2;
			
			CA_FREE_MEMORY(xml_value);
		}
		
		DISCARD_HANDLE(xml_node);
		
		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(xml_configuration, NULL,
													   "Parity", &xml_node));
		if (xml_node)
		{
			EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(xml_node, NULL, &xml_value));
			if (!stricmp(xml_value, "none"))
				dcb.Parity = NOPARITY;
			else if (!strcmp(xml_value, "Mark"))
				dcb.Parity = MARKPARITY;
			else if (!strcmp(xml_value, "Even"))
				dcb.Parity = EVENPARITY;
			else if (!strcmp(xml_value, "Odd"))
				dcb.Parity = ODDPARITY;
			else
				dcb.Parity = NOPARITY;
			
			CA_FREE_MEMORY(xml_value);
		}
		
		/* Open COM port */
		EXCCHECKCOM( OpenComConfig (me->port, "", dcb.BaudRate, dcb.Parity,
                                    dcb.ByteSize, dcb.StopBits, 512, 512));
										 
			
		/*if (!SetCommState (me->fd, &dcb))
		{
		    EXCTHROW( -7, "Serial Port Setting Failed");
		}
		
		SetCommMask(me->fd, EV_RXCHAR);
		me->termEvent = CreateEvent(NULL, 1, 0, NULL);
		
		if (me->termEvent == NULL)
		{
			EXCTHROW( -8, "Terminal Event Creation Failed");
		} */
		
		me->hThread = CreateThread(NULL, 0, Thread_axis,
								   (void*) me, 0, 0);
		
		if (me->hThread == NULL)
		{
			EXCTHROW( -9, "Thread Start Failed");
		}
		
#if 0    /* formerly excluded lines */
		for (i=0; i< MaxAxis; i++)
		{
		if (me->_InitializeSpec)
			EXCCHECK( me->_InitializeSpec(me, i));
		}
#endif   /* formerly excluded lines */
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
		
		me->_InitializeSpec			= NULL;
		me->_PresentAlarmSpec		= NULL;
		me->_ControllerStatusSpec	= NULL;
		me->_EnableAxisSpec			= NULL;
		me->_AlarmResetSpec			= NULL;
		me->_HomeReturnSpec			= NULL;
		me->_StopAxisSpec			= NULL;
		me->_MoveAxisPositionSpec	= NULL;
		me->_CurrentPositionSpec	= NULL;
		me->_RequestPositionSpec	= NULL;
		me->_ReadPositionSpec		= NULL;
		me->_SetPositionSpec		= NULL;
		me->_WriteSpec				= NULL;
		me->_callbackThreadSpec		= NULL;
		
		if (me->termEvent)
		{
			CloseHandle(me->termEvent);
			me->termEvent = 0;
		}
		
		if (me->fd)
		{
			CloseHandle(me->fd);
			me->fd = 0;
		}
	}
		
	DISCARD_HANDLE(xml_node);
	CA_FREE_MEMORY(xml_value);
	CA_FREE_MEMORY(xml_type);
	DISCARD_HANDLE(xml_configuration);
	DISCARD_HANDLE(document);
	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{axis}::Close"
static SElExceptionPtr axis_close(struct _axis* me)
{
	SElExceptionPtr	pexception = NULL;
	
	if (me->fd)
	{
		SetEvent(me->termEvent);
		/* todo: wait for thread end */
		Sleep(500);
		
		me->_InitializeSpec			= NULL;
		me->_PresentAlarmSpec		= NULL;
		me->_ControllerStatusSpec	= NULL;
		me->_EnableAxisSpec			= NULL;
		me->_AlarmResetSpec			= NULL;
		me->_HomeReturnSpec			= NULL;
		me->_MoveAxisPositionSpec	= NULL;
		me->_StopAxisSpec			= NULL;
		me->_CurrentPositionSpec	= NULL;
		me->_WriteSpec				= NULL;
		me->_callbackThreadSpec		= NULL;
		if (me->specLib)
		{
			FreeLibrary(me->specLib);
			me->specLib = 0;
		}
		
		CloseHandle(me->fd);
		me->fd = 0;
		
		CloseHandle(me->hThread);
		me->hThread = 0;
		
		CloseHandle(me->termEvent);
		me->termEvent = 0;
	}
	
Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "fncLock"
static SElExceptionPtr axisLock(SaxisPtr me)
{
	EnterCriticalSection( &me->lock);    
	return NULL;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "axisUnLock"
static SElExceptionPtr axisUnLock(SaxisPtr me)
{
	LeaveCriticalSection( &me->lock);   
	return NULL;
}

#undef __FUNC__
#define __FUNC__ "{axis}::Write"
static SElExceptionPtr axis_write(SaxisPtr me, const char* command)
{
	SElExceptionPtr	pexception = NULL;
	
	EXCCHECK( axisLock(me));

	if (me->_WriteSpec)
		EXCCHECK( me->_WriteSpec(me, command));
	
Error:
	axisUnLock(me);
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{axis}::PresentAlarm"
static SElExceptionPtr axis_PresentAlarm(SaxisPtr me, int32_t axis, int32_t* status)
{
	SElExceptionPtr	pexception = NULL;

	EXCCHECK( axisLock(me));

	if (me->_PresentAlarmSpec)
		EXCCHECK( me->_PresentAlarmSpec(me, axis, status));
	
Error:
	axisUnLock(me);
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{axis}::ControllerStatus"
static SElExceptionPtr axis_ControllerStatus(SaxisPtr me, int32_t axis, int32_t* status)
{
	SElExceptionPtr	pexception = NULL;

	EXCCHECK( axisLock(me));

	if (me->_ControllerStatusSpec)
		EXCCHECK( me->_ControllerStatusSpec(me, axis, status));
	
Error:
	axisUnLock(me);
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{axis}::EnableAxis"
static SElExceptionPtr axis_EnableAxis(SaxisPtr me, int32_t axis)
{
	SElExceptionPtr	pexception = NULL;
	
	EXCCHECK( axisLock(me));

	if (me->_EnableAxisSpec)
		EXCCHECK( me->_EnableAxisSpec(me, axis));
	
Error:
	axisUnLock(me);
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{axis}::AlarmReset"
static SElExceptionPtr axis_AlarmReset(SaxisPtr me, int32_t axis)
{
	SElExceptionPtr	pexception = NULL;
	
	EXCCHECK( axisLock(me));

	if (me->_AlarmResetSpec)
		EXCCHECK( me->_AlarmResetSpec(me, axis));
	
Error:
	axisUnLock(me);
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{axis}::HomeReturn"
static SElExceptionPtr axis_HomeReturn(SaxisPtr me, int32_t axis)
{
	SElExceptionPtr	pexception = NULL;
	
	EXCCHECK( axisLock(me));

	if (me->_HomeReturnSpec)
		EXCCHECK( me->_HomeReturnSpec(me,axis));
	
Error:
	axisUnLock(me);
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{axis}::StopAxis"
static SElExceptionPtr axis_StopAxis(SaxisPtr me, int32_t axis)
{
	SElExceptionPtr	pexception = NULL;
	
	EXCCHECK( axisLock(me));

	if (me->_StopAxisSpec)
		EXCCHECK( me->_StopAxisSpec(me,axis));
	
Error:
	axisUnLock(me);
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{axis}::MoveAxisPosition"
static SElExceptionPtr axis_MoveAxisPosition(SaxisPtr me, int32_t axis, int32_t position)
{
	SElExceptionPtr	pexception = NULL;
	
	EXCCHECK( axisLock(me));

	if (me->_MoveAxisPositionSpec)
		EXCCHECK( me->_MoveAxisPositionSpec(me, axis, position));
	
Error:
	axisUnLock(me);
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{axis}::SetPosition"
static SElExceptionPtr axis_SetPosition(SaxisPtr me, int32_t axis, double position, double speed)
{
	SElExceptionPtr	pexception = NULL;
	
	EXCCHECK( axisLock(me));

	if (me->_SetPositionSpec)
		EXCCHECK( me->_SetPositionSpec(me, axis, position, speed));
	
Error:
	axisUnLock(me);
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{axis}::CurrentPosition"
static SElExceptionPtr axis_CurrentPosition(SaxisPtr me, int32_t axis, double* position)
{
	SElExceptionPtr	pexception = NULL;
	
	EXCCHECK( axisLock(me));

	if (me->_CurrentPositionSpec)
		EXCCHECK( me->_CurrentPositionSpec(me, axis, position));
	
Error:
	axisUnLock(me);
	EXCRETHROW( pexception);
}
 
#undef __FUNC__
#define __FUNC__ "{axis}::RequestPosition"
static SElExceptionPtr axis_RequestPosition(SaxisPtr me, int32_t axis)
{
	SElExceptionPtr	pexception = NULL;
	
	//EXCCHECK( axisLock(me));

	if (me->_RequestPositionSpec)
		EXCCHECK( me->_RequestPositionSpec(me, axis));
	
Error:
	//axisUnLock(me);
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{axis}::ReadPosition"
static SElExceptionPtr axis_ReadPosition(SaxisPtr me, int32_t axis, double* position)
{
	SElExceptionPtr	pexception = NULL;
	
	//EXCCHECK( axisLock(me));

	if (me->_ReadPositionSpec)
		EXCCHECK( me->_ReadPositionSpec(me, axis, position));
	
Error:
	//axisUnLock(me);
	EXCRETHROW( pexception);
}
#undef __FUNC__
#define __FUNC__ "{axis}::Thread_axis"
DWORD WINAPI Thread_axis(
  LPVOID lpParameter
)
{
	SElExceptionPtr	pexception = NULL;
	SaxisPtr	me = (SaxisPtr) lpParameter;
	
	if (me->_callbackThreadSpec)
		EXCCHECK( me->_callbackThreadSpec(me));
	
Error:
	EXCDISPLAY( pexception);
	return 0;
}

/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "{axiss}::GetaxisByName"
static SElExceptionPtr axiss_GetaxisByName(
	struct _axiss*	me,
	const char*			axisName,
	SaxisPtr*		axis
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			i = 0;
	SaxisPtr		found_axis = NULL;
	char			message[1024] = "";
	
	for (i = 0; i < me->numaxiss; i++)
	{
		if (!strcmp(me->axiss[i]->axisName, axisName))
		{
			found_axis = me->axiss[i];
			break;
		}
	}
	
	if (axis)
		*axis = found_axis;
	
	if (found_axis == NULL)
	{
		sprintf(message, "axis Name (%s) Not Found", axisName);
		EXCTHROW( -12, message);
	}
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{axiss}::GetaxisByIndex"
static SElExceptionPtr axiss_GetaxisByIndex(
	struct _axiss*	me,
	int32_t				index,
	SaxisPtr*		axis
)
{
	SElExceptionPtr	pexception = NULL;
	
	if (index < me->numaxiss && index >= 0)
	{
		if (axis)
			*axis = me->axiss[index];
	}
	else
	{
		if (axis)
			*axis = NULL;
		
		EXCTHROW( -13, "Index Out of Range");
	}
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{axiss}::GetCurrentaxis"
static SElExceptionPtr axiss_GetCurrentaxis(
	struct _axiss*	me,
	SaxisPtr*		axis
)
{
	SElExceptionPtr	pexception = NULL;
	
	if (me->currentIndex >= 0 && me->numaxiss > 0)
	{
		if (axis)
			*axis = me->axiss[me->currentIndex];
	}
	else
	{
		if (axis)
			*axis = NULL;
	}

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{axiss}::SetCurrentaxisByName"
static SElExceptionPtr axiss_SetCurrentaxisByName(
	struct _axiss*	me,
	const char*			axisName
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			i = 0;
	SaxisPtr		axis = NULL;
	char			message[1024] = "";

	for (i = 0; i < me->numaxiss; i++)
	{
		if (!strcmp(me->axiss[i]->axisName, axisName))
		{
			me->currentIndex = i;
			break;
		}
	}
	
	if (i >= me->numaxiss)
	{
		sprintf(message, "axis Name (%s) Not Found", axisName);
		EXCTHROW( -12, message);
	}

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{axiss}::GetFirstaxis"
static SElExceptionPtr axiss_GetFirstaxis(
	struct _axiss*	me,
	SaxisPtr*		axis
)
{
	SElExceptionPtr	pexception = NULL;
	
	me->currentIndex = 0;

	if(me->numaxiss > 0)
	{
		if (axis)
			*axis = me->axiss[0];
	}
	else
	{
		if (axis)
			*axis = NULL;
	}

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{axiss}::GetNextaxis"
static SElExceptionPtr axiss_GetNextaxis(
	struct _axiss*	me,
	SaxisPtr*		axis
)
{
	SElExceptionPtr	pexception = NULL;

	if (++(me->currentIndex) >= me->numaxiss)
	{
		me->currentIndex = me->numaxiss - 1;
		if (axis)
			*axis = NULL;
	}
	else
	{
		if (axis)
			*axis = me->axiss[me->currentIndex];
	}
	
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{axiss}::Initaxiss"
static SElExceptionPtr axiss_Initaxiss(struct _axiss* me)
{
	SElExceptionPtr	pexception = NULL;
	HRESULT			hr = S_OK;
	VARIANT_BOOL	is_xml_success = 1;
	VARIANT			file;
	CAObjHandle		document = 0;
	CAObjHandle		axis = 0;
	CAObjHandle		axis_name_node = 0;
	CAObjHandle		axis_list = 0;
	char*			xml_value = NULL;
	char*			axis_xml = NULL;
	
	EXCCHECKCOM( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &document));
	
	CA_VariantSetCString (&file, "modules/axis.xml");
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentload(document, NULL, file, &is_xml_success));
	
	EXCCHECKCOM( MSXML_IXMLDOMDocumentselectNodes(document, NULL, "/Axiss/Axis", &axis_list));
	EXCCHECKCOM( MSXML_IXMLDOMNodeListreset (axis_list, NULL));
	
	EXCCHECKCOM( MSXML_IXMLDOMNodeListnextNode(axis_list, NULL, &axis));
	while (axis)
	{
		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(axis, NULL, "@name", &axis_name_node));
		EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(axis_name_node, NULL, &xml_value));
		EXCCHECKCOM( MSXML_IXMLDOMNodeGetxml(axis, NULL, &axis_xml));
		
		me->axiss = realloc( me->axiss, (me->numaxiss + 1) * sizeof(SaxisPtr));
		EXCCHECKALLOC( me->axiss);
		
		EXCCHECK( axis_new( &me->axiss[me->numaxiss], xml_value, NULL));
		EXCCHECK( me->axiss[me->numaxiss]->Open(me->axiss[me->numaxiss], NULL, axis_xml));
		me->numaxiss++;
		
		CA_FREE_MEMORY(xml_value);
		CA_FREE_MEMORY(axis_xml);
		DISCARD_HANDLE(axis_name_node);
		DISCARD_HANDLE(axis);
		
		EXCCHECKCOM( MSXML_IXMLDOMNodeListnextNode(axis_list, NULL, &axis));
	}
	
Error:
	CA_FREE_MEMORY(xml_value);
	CA_FREE_MEMORY(axis_xml);
	DISCARD_HANDLE(axis_name_node);
	DISCARD_HANDLE(axis);
	DISCARD_HANDLE(axis_list);
	DISCARD_HANDLE(document);
	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{axiss}::Closeaxiss"
static SElExceptionPtr axiss_Closeaxiss(struct _axiss* me)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			i = 0;
	
	for (i = 0; i < me->numaxiss; i++)
	{
		EXCCHECK( axis_delete(&(me->axiss[i])));
	}
	free(me->axiss);
	me->axiss = NULL;
	me->numaxiss = 0;
	
Error:
	EXCRETHROW( pexception);
}
