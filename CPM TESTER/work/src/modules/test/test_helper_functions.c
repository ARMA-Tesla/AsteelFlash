#include <windows.h>
#include <formatio.h>
#include "msxmldom.h"
#include "test_helper_functions.h"
#include <test/eltest.h>
#include <guid.h>
#include <toolbox.h>
#include <utility.h>
#include <sdk/mmsystem.h>
#include <multithread/multithread.h>

#pragma warning( push)
#pragma warning( disable: 4996)

#define MAX_LENGTH_CHANNEL_NAME		32
#define MAX_NB_OF_CHANNELS			16

/* 
 * BFFA2008  No explicit path exists between the two channels.
 * BFFA200C  The channels are explicitly connected.
 */
#define checkSwtch(fCall) \
	if (error = (fCall), (error = (error<0 && error!=0xBFFA2008 && error!=0xBFFA200C) ? error : VI_SUCCESS)) \
		{ goto Error; }  else error = error

typedef struct _SLogicNames
{
	char		name[MAX_LENGTH_CHANNEL_NAME+1];
	char		physical[MAX_LENGTH_CHANNEL_NAME+1];
	/* switching */
	bool_t		is_active;
	char		psw1[MAX_LENGTH_CHANNEL_NAME+1]; /* channel switch */
	char		psw2[MAX_LENGTH_CHANNEL_NAME+1];
	bool_t		is_low_current;
	char		pswc1[MAX_LENGTH_CHANNEL_NAME+1]; /* low current meas switch */
	char		pswc2[MAX_LENGTH_CHANNEL_NAME+1];
	/* calibration */
	char		scale1[MAX_LENGTH_CHANNEL_NAME+1]; /* hi curent */
	char		scale2[MAX_LENGTH_CHANNEL_NAME+1]; /* low current */
} SLogicNames, *SLogicNamesPtr;

typedef struct _SPowerConfig
{
	SLogicNames		channels[MAX_NB_OF_CHANNELS];
	int32_t			channelsSize;
	char			calibration_file[MAX_LENGTH_CHANNEL_NAME+1];
} SPowerConfig, *SPowerConfigPtr;

typedef enum _ESwitchFnc
{
	ESF_CONNECT = 0,
	ESF_DISCONNECT,
	ESF_DISCONNECT_ALL,
	ESF_CHECK_PATH,
	ESF_SLEEP
} ESwitchFnc;

static SPowerConfig		gs_PowerConfig = {0,0,0};
static DWORD WINAPI Thread_DAQ(LPVOID param);

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LoadConfiguration"
static SElExceptionPtr LoadConfiguration(
   const char*   fileName,
   CAObjHandle	*xml
)
{
	SElExceptionPtr pexception = NULL;
	static CAObjHandle	pxml  = 0;

	if(pxml==0)
	{
		int32_t			error = 0;
		HRESULT         hr = 0;
		const char*     pfile_name = (fileName) ? fileName : "\\test.xml";
		VBOOL         	is_xml_success;
		VARIANT         pfile_name_var = CA_VariantEmpty();
		char			path[MAX_PATHNAME_LEN];

		EXCCHECKCVI( GetModuleDir (__CVIUserHInst, path)); 
		strcat(path, pfile_name);
		
		EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml));
		EXCCHECKCVI( CA_VariantSetCString(&pfile_name_var, path));
		EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (pxml, NULL, pfile_name_var, &is_xml_success)); 
		EXCCHECKCVI( CA_VariantClear(&pfile_name_var));
	}
	
Error:
	if(pexception && pxml)
	{
		CA_DiscardObjHandle(pxml);
		pxml = 0;
	}
	if(xml) *xml = pxml;
	EXCRETHROW( pexception);
}

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Camera_RegisterTCPServer"
SElExceptionPtr Camera_RegisterTCPServer(
	SObjListPtr		pModules,
	int				IndexCamera
)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	SCamerasPtr pCameras = (SCamerasPtr)pModules->Get(pModules, MODULE_CAMERA);
	SCameraPtr  pCamera = NULL;

	EXCCHECK( pCameras->GetCameraByIndex(pCameras, IndexCamera, &pCamera));
	EXCCHECK( pCamera->openTCPserver(pCamera));

Error:
	EXCRETHROW( pexception);
}

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Camera_login"
SElExceptionPtr Camera_login(
	SObjListPtr		pModules,
	int				IndexCamera
)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	SCamerasPtr pCameras = (SCamerasPtr)pModules->Get(pModules, MODULE_CAMERA);
	SCameraPtr  pCamera = NULL;

	EXCCHECK( pCameras->GetCameraByIndex(pCameras, IndexCamera, &pCamera));
	EXCCHECK( pCamera->Cognex_login(pCamera));

Error:
	EXCRETHROW( pexception);
}

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Camera_disconnect"
SElExceptionPtr Camera_disconnect(
	SObjListPtr		pModules,
	int				IndexCamera
)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	SCamerasPtr pCameras = (SCamerasPtr)pModules->Get(pModules, MODULE_CAMERA);
	SCameraPtr  pCamera = NULL;

	EXCCHECK( pCameras->GetCameraByIndex(pCameras, IndexCamera, &pCamera));
	EXCCHECK( pCamera->Cognex_disconnect(pCamera));

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "Camera_Trigger"
SElExceptionPtr Camera_Trigger(
	SObjListPtr		pModules,
	int				IndexCamera
)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	SCamerasPtr pCameras = (SCamerasPtr)pModules->Get(pModules, MODULE_CAMERA);
	SCameraPtr  pCamera = NULL;

	EXCCHECK( pCameras->GetCameraByIndex(pCameras, IndexCamera, &pCamera));
	EXCCHECK( pCamera->Cognex_Trigger(pCamera));

Error:
	EXCRETHROW( pexception);
}

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Camera_Read"
SElExceptionPtr Camera_Read(
	SObjListPtr		pModules,
	int				IndexCamera,
	char            data[128]
)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	SCamerasPtr pCameras = (SCamerasPtr)pModules->Get(pModules, MODULE_CAMERA);
	SCameraPtr  pCamera = NULL;

	EXCCHECK( pCameras->GetCameraByIndex(pCameras, 0, &pCamera));
	EXCCHECK( pCamera->Cognex_Read(pCamera, data));

Error:
	EXCRETHROW( pexception);
}

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Camera_Online"
SElExceptionPtr Camera_Online(
	SObjListPtr		pModules,
	int				IndexCamera
)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	SCamerasPtr pCameras = (SCamerasPtr)pModules->Get(pModules, MODULE_CAMERA);
	SCameraPtr  pCamera = NULL;

	EXCCHECK( pCameras->GetCameraByIndex(pCameras, IndexCamera, &pCamera));
	EXCCHECK( pCamera->Cognex_Online(pCamera));

Error:
	EXCRETHROW( pexception);
}

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Camera_Offline"
SElExceptionPtr Camera_Offline(
	SObjListPtr		pModules,
	int				IndexCamera
)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	SCamerasPtr pCameras = (SCamerasPtr)pModules->Get(pModules, MODULE_CAMERA);
	SCameraPtr  pCamera = NULL;

	EXCCHECK( pCameras->GetCameraByIndex(pCameras, IndexCamera, &pCamera));
	EXCCHECK( pCamera->Cognex_Offline(pCamera));

Error:
	EXCRETHROW( pexception);
}

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Camera_Set_Job"
SElExceptionPtr Camera_Set_Job(
	SObjListPtr		pModules,
	int				IndexCamera,
	char           *Job
)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	SCamerasPtr pCameras = (SCamerasPtr)pModules->Get(pModules, MODULE_CAMERA);
	SCameraPtr  pCamera = NULL;

	EXCCHECK( pCameras->GetCameraByIndex(pCameras, IndexCamera, &pCamera));
	EXCCHECK( pCamera->Cognex_Job(pCamera, Job));

Error:
	EXCRETHROW( pexception);
}

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Camera_Set_Step"
SElExceptionPtr Camera_Set_Step(
	SObjListPtr		pModules,
	int				IndexCamera,
	int             Step
)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	SCamerasPtr pCameras = (SCamerasPtr)pModules->Get(pModules, MODULE_CAMERA);
	SCameraPtr  pCamera = NULL;

	EXCCHECK( pCameras->GetCameraByIndex(pCameras, IndexCamera, &pCamera));
	EXCCHECK( pCamera->Cognex_set_param(pCamera, Step));

Error:
	EXCRETHROW( pexception);
}
#pragma warning( pop)

