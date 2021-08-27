#ifndef _CAMERAS_H_
#define _CAMERAS_H_

#include <windows.h>
#include <exception/Exception.h>

#define MAX_CAMERA_NAME			128
#define MAX_BUFFER_SIZE				20

#define CAMERA_ERROR_TIMEOUT				-50001

typedef struct _Camera
{
	char*				cameraName;
	char*				optionString;
	char				adressIP[13+1];
	int					port;
	int                 Index;
	CRITICAL_SECTION	lock;
	HANDLE				fd;
	unsigned int 		hconversation;
	HANDLE				termEvent;
	HANDLE				readEvent;
	HANDLE				specLib;
	HANDLE				hThread;
	int32_t				status;
	int32_t				readMode;
	char				readBuffer[MAX_BUFFER_SIZE];
	
	SElExceptionPtr (*Open)(
		struct _Camera*	me,
		const char*			resource,
		const char*			optionString
	);
	SElExceptionPtr (*Close)(
		struct _Camera*	me
	);
	SElExceptionPtr (*openTCPserver)(struct _Camera* me);
	SElExceptionPtr (*Cognex_login)(struct _Camera* me);
	SElExceptionPtr (*Cognex_disconnect)(struct _Camera* me);
	SElExceptionPtr (*Cognex_Trigger)(struct _Camera* me);
	SElExceptionPtr (*Cognex_Read)(struct _Camera* me, char *data);
	SElExceptionPtr (*Cognex_Online)(struct _Camera* me);
	SElExceptionPtr (*Cognex_Offline)(struct _Camera* me);
	SElExceptionPtr (*Cognex_act_job)(struct _Camera* me, int *actual_job);
	SElExceptionPtr (*Cognex_Job)(struct _Camera* me, const char *JobID);
	SElExceptionPtr (*Cognex_set_param)(struct _Camera* me, int phase);
	SElExceptionPtr (*Cognex_set_number_boutton)(struct _Camera* me, int phase);
	SElExceptionPtr (*Cognex_set_parameters)(struct _Camera* me, char* colonne, int index_pico);  
	
	/* Private */
	SElExceptionPtr (*_openTCPserverSpec)(struct _Camera* me);
	SElExceptionPtr (*_Cognex_loginSpec)(struct _Camera* me);
	SElExceptionPtr (*_Cognex_disconnectSpec)(struct _Camera* me);
	SElExceptionPtr (*_Cognex_TriggerSpec)(struct _Camera* me);
	SElExceptionPtr (*_Cognex_ReadSpec)(struct _Camera* me, char *data);
	SElExceptionPtr (*_Cognex_OnlineSpec)(struct _Camera* me);
	SElExceptionPtr (*_Cognex_OfflineSpec)(struct _Camera* me);
	SElExceptionPtr (*_Cognex_act_jobSpec)(struct _Camera* me, int *actual_job);
	SElExceptionPtr (*_Cognex_JobSpec)(struct _Camera* me, const char *JobID);
	SElExceptionPtr (*_Cognex_set_paramSpec)(struct _Camera* me, int phase);
	SElExceptionPtr (*_Cognex_set_number_bouttonSpec)(struct _Camera* me, int index_pico); 
	SElExceptionPtr (*_Cognex_set_parametersSpec)(struct _Camera* me, char* colonne, int index_pico);
	
} SCamera, *SCameraPtr;


typedef struct _Cameras
{
	SCameraPtr*	cameras;
	int32_t			numCameras;
	int32_t			currentIndex;
	
	SElExceptionPtr (*GetCameraByName)(
		struct _Cameras*	me,
		const char*			cameraName,
		SCameraPtr*		camera
	);
	SElExceptionPtr (*GetCameraByIndex)(
		struct _Cameras*	me,
		int32_t				index,
		SCameraPtr*		camera
	);
	SElExceptionPtr (*GetCurrentCamera)(
		struct _Cameras*	me,
		SCameraPtr*		camera
	);
	SElExceptionPtr (*SetCurrentCameraByName)(
		struct _Cameras*	me,
		const char*			cameraName
	);
	SElExceptionPtr (*GetFirstCamera)(
		struct _Cameras*	me,
		SCameraPtr*		camera
	);
	SElExceptionPtr (*GetNextCamera)(
		struct _Cameras*	me,
		SCameraPtr*		camera
	);
	
	SElExceptionPtr (*InitCameras)(struct _Cameras* me);
	SElExceptionPtr (*CloseCameras)(struct _Cameras* me);
	
} SCameras, *SCamerasPtr;

SElExceptionPtr cameras_new(SCamerasPtr* cameras);
SElExceptionPtr cameras_delete(SCamerasPtr* cameras);

SElExceptionPtr camera_new(
	SCameraPtr*	camera,
	const char*		name,
	const char*		optionString
);
SElExceptionPtr camera_delete(SCameraPtr* camera);

#endif /* _CAMERAS_H_ */
