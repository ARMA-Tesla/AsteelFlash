#include "cameras.h"
#include <string.h>
#include <stdio.h>
#include <cviauto.h>
#include "msxmldom.h"

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

static SElExceptionPtr camera_open(
		struct _Camera*	me,
		const char*			resource,
		const char*			optionString
);
static SElExceptionPtr camera_close(
		struct _Camera* me
);

static SElExceptionPtr camera_openTCPserver(SCameraPtr me);
static SElExceptionPtr camera_Cognex_login(SCameraPtr me);
static SElExceptionPtr camera_Cognex_disconnect(SCameraPtr me);
static SElExceptionPtr camera_Cognex_Trigger(SCameraPtr me);
static SElExceptionPtr camera_Cognex_Read(SCameraPtr me, char *data);
static SElExceptionPtr camera_Cognex_Online(SCameraPtr me);
static SElExceptionPtr camera_Cognex_Offline(SCameraPtr me);
static SElExceptionPtr camera_Cognex_act_job(SCameraPtr me, int *actual_job);
static SElExceptionPtr camera_Cognex_Job(SCameraPtr me, const char *JobID);
static SElExceptionPtr camera_Cognex_set_param(SCameraPtr me, int phase);
static SElExceptionPtr camera_Cognex_set_number_boutton(SCameraPtr me, int phase);
static SElExceptionPtr camera_Cognex_set_parameters(SCameraPtr me, char* colonne, int index_pico); 
	
	
DWORD WINAPI Thread_Camera(
  LPVOID lpParameter
);

/*****************************************************************************/

static SElExceptionPtr cameras_GetCameraByName(
	struct _Cameras*	me,
	const char*			cameraName,
	SCameraPtr*		camera
);
static SElExceptionPtr cameras_GetCameraByIndex(
	struct _Cameras*	me,
	int32_t				index,
	SCameraPtr*		camera
);
static SElExceptionPtr cameras_GetCurrentCamera(
	struct _Cameras*	me,
	SCameraPtr*		camera
);
static SElExceptionPtr cameras_SetCurrentCameraByName(
	struct _Cameras*	me,
	const char*			cameraName
);
static SElExceptionPtr cameras_GetFirstCamera(
	struct _Cameras*	me,
	SCameraPtr*		camera
);
static SElExceptionPtr cameras_GetNextCamera(
	struct _Cameras*	me,
	SCameraPtr*		camera
);
	
static SElExceptionPtr cameras_InitCameras(struct _Cameras* me);
static SElExceptionPtr cameras_CloseCameras(struct _Cameras* me);

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "camera_new"
SElExceptionPtr camera_new(
	SCameraPtr*	camera,
	const char*		name,
	const char*		optionString
)
{
	SElExceptionPtr	pexception = NULL;
	SCameraPtr		pnewcamera = NULL;
	
	pnewcamera = calloc(1, sizeof(SCamera));
	EXCCHECKALLOC( pnewcamera);
	InitializeCriticalSection(&(pnewcamera->lock));
	
	pnewcamera->cameraName = calloc(strlen(name) + 1, sizeof(char));
	EXCCHECKALLOC( pnewcamera->cameraName);
	strcpy(pnewcamera->cameraName, name);
	
	pnewcamera->Open		= camera_open;
	pnewcamera->Close		= camera_close;
	
	pnewcamera->openTCPserver		= camera_openTCPserver;
	pnewcamera->Cognex_login	    = camera_Cognex_login;
	pnewcamera->Cognex_disconnect	= camera_Cognex_disconnect;
	pnewcamera->Cognex_Trigger		= camera_Cognex_Trigger;
	pnewcamera->Cognex_Read			= camera_Cognex_Read;
	pnewcamera->Cognex_Online		= camera_Cognex_Online;
	pnewcamera->Cognex_Offline		= camera_Cognex_Offline;
	pnewcamera->Cognex_act_job		= camera_Cognex_act_job;
	pnewcamera->Cognex_Job		    = camera_Cognex_Job;
	pnewcamera->Cognex_set_param	= camera_Cognex_set_param;
	pnewcamera->Cognex_set_number_boutton	= camera_Cognex_set_number_boutton;
	pnewcamera->Cognex_set_parameters		= camera_Cognex_set_parameters; 
	
	if (camera)
		*camera = pnewcamera;
	
Error:
	if (pexception && pnewcamera)
		free(pnewcamera);
	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "camera_delete"
SElExceptionPtr camera_delete(SCameraPtr* camera)
{
	SElExceptionPtr	pexception = NULL;
	
	if (camera && *camera)
	{
		(*camera)->Close(*camera);
		
		if ((*camera)->cameraName)
			free((*camera)->cameraName);
		
		if ((*camera)->optionString)
			free((*camera)->optionString);
		
		DeleteCriticalSection( &((*camera)->lock) );
		
		free(*camera);
		*camera = NULL;
	}
	
Error:
	EXCRETHROW( pexception);
}

/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "cameras_new"
SElExceptionPtr cameras_new(SCamerasPtr*	cameras)
{
	SElExceptionPtr	pexception = NULL;
	SCamerasPtr	pnew_cameras = NULL;
	
	pnew_cameras = calloc(1, sizeof(SCameras));
	EXCCHECKALLOC( pnew_cameras);
	
	pnew_cameras->cameras = NULL;
	pnew_cameras->numCameras = 0;
	pnew_cameras->currentIndex = -1;
	
	pnew_cameras->GetCameraByName			= cameras_GetCameraByName;
	pnew_cameras->GetCameraByIndex		= cameras_GetCameraByIndex;
	pnew_cameras->GetCurrentCamera		= cameras_GetCurrentCamera;
	pnew_cameras->SetCurrentCameraByName	= cameras_SetCurrentCameraByName;
	pnew_cameras->GetFirstCamera			= cameras_GetFirstCamera;
	pnew_cameras->GetNextCamera			= cameras_GetNextCamera;
	pnew_cameras->InitCameras				= cameras_InitCameras;
	pnew_cameras->CloseCameras			= cameras_CloseCameras;
	
	if (cameras)
		*cameras = pnew_cameras;
	
Error:
	if (pexception && pnew_cameras)
		free(pnew_cameras);
	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "cameras_delete"
SElExceptionPtr cameras_delete(SCamerasPtr* cameras)
{
	SElExceptionPtr	pexception = NULL;
	
	if (cameras && *cameras)
	{
		EXCCHECK( (*cameras)->CloseCameras(*cameras));
		
		free(*cameras);
		*cameras = NULL;
	}
	
Error:
	EXCRETHROW( pexception);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "{Camera}::Open"
static SElExceptionPtr camera_open(
		struct _Camera*	me,
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
	LONG            error;
	
	me->optionString = calloc(strlen(optionString) + 1, sizeof(char));
	EXCCHECKALLOC( me->optionString);
	strcpy(me->optionString, optionString);
	
	EXCCHECKCOM( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &document));
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentloadXML (document, NULL, optionString, &is_xml_success));
	
	EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode(document, NULL,
													   "/Camera/Configuration",
													   &xml_configuration));
	if (xml_configuration == 0)
		EXCTHROW( -1, "Missing Configuration Node");
	
	/* Load Specific DLL functions */
	EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(xml_configuration, NULL, "@specificDll", &xml_node));
	if (xml_node == 0)
		EXCTHROW( -3, "Missing Camera Specific Library");
	
	EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(xml_node, NULL, &xml_value));
	me->specLib = LoadLibrary(xml_value);
	if (me->specLib == 0)
	{
		sprintf(message, "Camera Library (%s) Loading Failed", xml_value);
		EXCTHROW( -4, message);
	}
	
		me->_openTCPserverSpec	= (SElExceptionPtr (*)(SCameraPtr))
								GetProcAddress(me->specLib, "openTCPserver");
		
	  	me->_Cognex_loginSpec	= (SElExceptionPtr (*)(SCameraPtr))
								GetProcAddress(me->specLib, "Cognex_login");
		
		me->_Cognex_disconnectSpec	= (SElExceptionPtr (*)(SCameraPtr))
								GetProcAddress(me->specLib, "Cognex_disconnect");
		me->_Cognex_TriggerSpec	= (SElExceptionPtr (*)(SCameraPtr))
								GetProcAddress(me->specLib, "Cognex_Trigger");
		me->_Cognex_ReadSpec	= (SElExceptionPtr (*)(SCameraPtr, char *))
								GetProcAddress(me->specLib, "Cognex_Read");
		me->_Cognex_OnlineSpec	= (SElExceptionPtr (*)(SCameraPtr))
								GetProcAddress(me->specLib, "Cognex_Online");
		me->_Cognex_OfflineSpec	= (SElExceptionPtr (*)(SCameraPtr))
								GetProcAddress(me->specLib, "Cognex_Offline");
		me->_Cognex_act_jobSpec	= (SElExceptionPtr (*)(SCameraPtr, int*))
								GetProcAddress(me->specLib, "Cognex_act_job");
		me->_Cognex_JobSpec	= (SElExceptionPtr (*)(SCameraPtr, const char *))
								GetProcAddress(me->specLib, "Cognex_Job");
		me->_Cognex_set_paramSpec	= (SElExceptionPtr (*)(SCameraPtr,int))
								GetProcAddress(me->specLib, "Cognex_set_param");
		me->_Cognex_set_number_bouttonSpec	= (SElExceptionPtr (*)(SCameraPtr,int))
								GetProcAddress(me->specLib, "Cognex_set_number_boutton");
		me->_Cognex_set_parametersSpec	= (SElExceptionPtr (*)(SCameraPtr,char *,int))
								GetProcAddress(me->specLib, "Cognex_set_parameters");
	
	
	CA_FREE_MEMORY(xml_value);
	DISCARD_HANDLE(xml_node);
	
	/* Get Type of communication */
	EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(xml_configuration, NULL,
												   "@type", &xml_node));
	if (xml_node == 0)
		EXCTHROW( -2, "Missing Configuration Type");

	EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(xml_node, NULL, &xml_type));
	DISCARD_HANDLE(xml_node);

	if (!stricmp(xml_type, "Ethernet"))
	{
		
		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(xml_configuration, NULL,
													   "Index", &xml_node));
		if (xml_node)
		{
			EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(xml_node, NULL, &xml_value));
			me->Index = atoi(xml_value);
			CA_FREE_MEMORY(xml_value);
		}
		
		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(xml_configuration, NULL,
													   "IP", &xml_node));
		if (xml_node)
		{
			EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(xml_node, NULL, &xml_value));
			sprintf (me->adressIP,xml_value);
			CA_FREE_MEMORY(xml_value);
		}
		
		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(xml_configuration, NULL,
													   "port", &xml_node));
		if (xml_node)
		{
			EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(xml_node, NULL, &xml_value));
			me->port = atoi(xml_value);
			CA_FREE_MEMORY(xml_value);
		}
	}
	
	
Error:
	if (pexception)
	{
		if (me->specLib)
		{
			FreeLibrary(me->specLib);
			me->specLib = 0;
		}
		
		me->_openTCPserverSpec	= NULL;
		me->_Cognex_loginSpec	= NULL;
		me->_Cognex_disconnectSpec		= NULL;
		me->_Cognex_TriggerSpec= NULL;
		me->_Cognex_OnlineSpec= NULL;
		me->_Cognex_OfflineSpec= NULL;
		me->_Cognex_act_jobSpec= NULL;
		me->_Cognex_JobSpec= NULL;
		me->_Cognex_set_paramSpec= NULL;
		me->_Cognex_set_number_bouttonSpec= NULL;
		me->_Cognex_set_parametersSpec= NULL; 
	}
		
	DISCARD_HANDLE(xml_node);
	CA_FREE_MEMORY(xml_value);
	CA_FREE_MEMORY(xml_type);
	DISCARD_HANDLE(xml_configuration);
	DISCARD_HANDLE(document);
	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Camera}::Close"
static SElExceptionPtr camera_close(struct _Camera* me)
{
	SElExceptionPtr	pexception = NULL;
	

		me->_openTCPserverSpec	= NULL;
		me->_Cognex_loginSpec	= NULL;
		me->_Cognex_disconnectSpec		= NULL;
		me->_Cognex_TriggerSpec= NULL;
		me->_Cognex_OnlineSpec= NULL;
		me->_Cognex_OfflineSpec= NULL;
		me->_Cognex_act_jobSpec= NULL;
		me->_Cognex_JobSpec= NULL;
		me->_Cognex_set_paramSpec= NULL;
		me->_Cognex_set_number_bouttonSpec= NULL;
		me->_Cognex_set_parametersSpec= NULL;
		
		if (me->specLib)
		{
			FreeLibrary(me->specLib);
			me->specLib = 0;
		}
	
Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "fncLock"
static SElExceptionPtr cameraLock(SCameraPtr me)
{
	EnterCriticalSection( &me->lock);    
	return NULL;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "cameraUnLock"
static SElExceptionPtr cameraUnLock(SCameraPtr me)
{
	LeaveCriticalSection( &me->lock);   
	return NULL;
}

#undef __FUNC__
#define __FUNC__ "{Camera}::openTCPserver"
static SElExceptionPtr camera_openTCPserver(SCameraPtr me)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_openTCPserverSpec)
		EXCCHECK( me->_openTCPserverSpec(me));
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Camera}::Cognex_login"
static SElExceptionPtr camera_Cognex_login(SCameraPtr me)
{
	SElExceptionPtr	pexception = NULL;


	if (me->_Cognex_loginSpec)
		EXCCHECK( me->_Cognex_loginSpec(me));
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Camera}::Cognex_disconnect"
static SElExceptionPtr camera_Cognex_disconnect(SCameraPtr me)
{
	SElExceptionPtr	pexception = NULL;


	if (me->_Cognex_disconnectSpec)
		EXCCHECK( me->_Cognex_disconnectSpec(me));
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Camera}::Cognex_Trigger"
static SElExceptionPtr camera_Cognex_Trigger(SCameraPtr me)
{
	SElExceptionPtr	pexception = NULL;


	if (me->_Cognex_TriggerSpec)
		EXCCHECK( me->_Cognex_TriggerSpec(me));
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Camera}::Cognex_Read"
static SElExceptionPtr camera_Cognex_Read(SCameraPtr me, char *data)
{
	SElExceptionPtr	pexception = NULL;


	if (me->_Cognex_ReadSpec)
		EXCCHECK( me->_Cognex_ReadSpec(me, data));
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Camera}::Cognex_Online"
static SElExceptionPtr camera_Cognex_Online(SCameraPtr me)
{
	SElExceptionPtr	pexception = NULL;


	if (me->_Cognex_OnlineSpec)
		EXCCHECK( me->_Cognex_OnlineSpec(me));
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Camera}::Cognex_Offline"
static SElExceptionPtr camera_Cognex_Offline(SCameraPtr me)
{
	SElExceptionPtr	pexception = NULL;


	if (me->_Cognex_OfflineSpec)
		EXCCHECK( me->_Cognex_OfflineSpec(me));
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Camera}::Cognex_act_job"
static SElExceptionPtr camera_Cognex_act_job(SCameraPtr me, int *actual_job)
{
	SElExceptionPtr	pexception = NULL;


	if (me->_Cognex_act_jobSpec)
		EXCCHECK( me->_Cognex_act_jobSpec(me, actual_job));
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Camera}::Cognex_Job"
static SElExceptionPtr camera_Cognex_Job(SCameraPtr me, const char *JobID)
{
	SElExceptionPtr	pexception = NULL;


	if (me->_Cognex_JobSpec)
		EXCCHECK( me->_Cognex_JobSpec(me, JobID));
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Camera}::Cognex_set_param"
static SElExceptionPtr camera_Cognex_set_param(SCameraPtr me, int phase)
{
	SElExceptionPtr	pexception = NULL;


	if (me->_Cognex_set_paramSpec)
		EXCCHECK( me->_Cognex_set_paramSpec(me,phase));
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Camera}::Cognex_set_number_boutton"
static SElExceptionPtr camera_Cognex_set_number_boutton(SCameraPtr me, int phase)
{
	SElExceptionPtr	pexception = NULL;


	if (me->_Cognex_set_number_bouttonSpec)
		EXCCHECK( me->_Cognex_set_number_bouttonSpec(me,phase));
	
Error:
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{Camera}::Cognex_set_parameters"
static SElExceptionPtr camera_Cognex_set_parameters(SCameraPtr me, char* colonne, int index_pico)
{
	SElExceptionPtr	pexception = NULL;


	if (me->_Cognex_set_parametersSpec)
		EXCCHECK( me->_Cognex_set_parametersSpec(me,colonne,index_pico));
	
Error:
	EXCRETHROW( pexception);
}

/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "{Cameras}::GetCameraByName"
static SElExceptionPtr cameras_GetCameraByName(
	struct _Cameras*	me,
	const char*			cameraName,
	SCameraPtr*		camera
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			i = 0;
	SCameraPtr		foundcamera = NULL;
	char			message[1024] = "";
	
	for (i = 0; i < me->numCameras; i++)
	{
		if (!strcmp(me->cameras[i]->cameraName, cameraName))
		{
			foundcamera = me->cameras[i];
			break;
		}
	}
	
	if (camera)
		*camera = foundcamera;
	
	if (foundcamera == NULL)
	{
		sprintf(message, "Camera Name (%s) Not Found", cameraName);
		EXCTHROW( -12, message);
	}
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Cameras}::GetCameraByIndex"
static SElExceptionPtr cameras_GetCameraByIndex(
	struct _Cameras*	me,
	int32_t				index,
	SCameraPtr*		camera
)
{
	SElExceptionPtr	pexception = NULL;
	
	if (index < me->numCameras && index >= 0)
	{
		if (camera)
			*camera = me->cameras[index];
	}
	else
	{
		if (camera)
			*camera = NULL;
		
		EXCTHROW( -13, "Index Out of Range");
	}
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Cameras}::GetCurrentCamera"
static SElExceptionPtr cameras_GetCurrentCamera(
	struct _Cameras*	me,
	SCameraPtr*		camera
)
{
	SElExceptionPtr	pexception = NULL;
	
	if (me->currentIndex >= 0 && me->numCameras > 0)
	{
		if (camera)
			*camera = me->cameras[me->currentIndex];
	}
	else
	{
		if (camera)
			*camera = NULL;
	}

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Cameras}::SetCurrentCameraByName"
static SElExceptionPtr cameras_SetCurrentCameraByName(
	struct _Cameras*	me,
	const char*			cameraName
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			i = 0;
	SCameraPtr		camera = NULL;
	char			message[1024] = "";

	for (i = 0; i < me->numCameras; i++)
	{
		if (!strcmp(me->cameras[i]->cameraName, cameraName))
		{
			me->currentIndex = i;
			break;
		}
	}
	
	if (i >= me->numCameras)
	{
		sprintf(message, "Camera Name (%s) Not Found", cameraName);
		EXCTHROW( -12, message);
	}

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Cameras}::GetFirstCamera"
static SElExceptionPtr cameras_GetFirstCamera(
	struct _Cameras*	me,
	SCameraPtr*		camera
)
{
	SElExceptionPtr	pexception = NULL;
	
	me->currentIndex = 0;

	if(me->numCameras > 0)
	{
		if (camera)
			*camera = me->cameras[0];
	}
	else
	{
		if (camera)
			*camera = NULL;
	}

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Cameras}::GetNextCamera"
static SElExceptionPtr cameras_GetNextCamera(
	struct _Cameras*	me,
	SCameraPtr*		camera
)
{
	SElExceptionPtr	pexception = NULL;

	if (++(me->currentIndex) >= me->numCameras)
	{
		me->currentIndex = me->numCameras - 1;
		if (camera)
			*camera = NULL;
	}
	else
	{
		if (camera)
			*camera = me->cameras[me->currentIndex];
	}
	
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Cameras}::InitCameras"
static SElExceptionPtr cameras_InitCameras(struct _Cameras* me)
{
	SElExceptionPtr	pexception = NULL;
	HRESULT			hr = S_OK;
	VARIANT_BOOL	is_xml_success = 1;
	VARIANT			file;
	CAObjHandle		document = 0;
	CAObjHandle		camera = 0;
	CAObjHandle		camera_name_node = 0;
	CAObjHandle		camera_list = 0;
	char*			xml_value = NULL;
	char*			camera_xml = NULL;
	
	EXCCHECKCOM( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &document));
	
	CA_VariantSetCString (&file, "modules/cameras.xml");
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentload(document, NULL, file, &is_xml_success));
	
	EXCCHECKCOM( MSXML_IXMLDOMDocumentselectNodes(document, NULL, "/Cameras/Camera", &camera_list));
	EXCCHECKCOM( MSXML_IXMLDOMNodeListreset (camera_list, NULL));
	
	EXCCHECKCOM( MSXML_IXMLDOMNodeListnextNode(camera_list, NULL, &camera));
	while (camera)
	{
		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(camera, NULL, "@name", &camera_name_node));
		EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(camera_name_node, NULL, &xml_value));
		EXCCHECKCOM( MSXML_IXMLDOMNodeGetxml(camera, NULL, &camera_xml));
		
		me->cameras = realloc( me->cameras, (me->numCameras + 1) * sizeof(SCameraPtr));
		EXCCHECKALLOC( me->cameras);
		
		EXCCHECK( camera_new( &me->cameras[me->numCameras], xml_value, NULL));
		EXCCHECK( me->cameras[me->numCameras]->Open(me->cameras[me->numCameras], NULL, camera_xml));
		me->numCameras++;
		
		CA_FREE_MEMORY(xml_value);
		CA_FREE_MEMORY(camera_xml);
		DISCARD_HANDLE(camera_name_node);
		DISCARD_HANDLE(camera);
		
		EXCCHECKCOM( MSXML_IXMLDOMNodeListnextNode(camera_list, NULL, &camera));
	}
	
Error:
	CA_FREE_MEMORY(xml_value);
	CA_FREE_MEMORY(camera_xml);
	DISCARD_HANDLE(camera_name_node);
	DISCARD_HANDLE(camera);
	DISCARD_HANDLE(camera_list);
	DISCARD_HANDLE(document);
	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Cameras}::CloseCameras"
static SElExceptionPtr cameras_CloseCameras(struct _Cameras* me)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			i = 0;
	
	for (i = 0; i < me->numCameras; i++)
	{
		EXCCHECK( camera_delete(&(me->cameras[i])));
	}
	free(me->cameras);
	me->cameras = NULL;
	me->numCameras = 0;
	
Error:
	EXCRETHROW( pexception);
}
