#ifndef _CAMERA_SPECIFIC_
#define _CAMERA_SPECIFIC_

#include <Camera/class/cameras.h>

#if defined(WINDOWS)
	#define CAMERA_API		__declspec( dllexport )
#elif defined(UNIX)
	#define CAMERA_API		extern "C"
#else
	#define CAMERA_API
#endif

CAMERA_API SElExceptionPtr _FUNCC openTCPserver(SCameraPtr me);
CAMERA_API SElExceptionPtr _FUNCC Cognex_login(SCameraPtr me);
CAMERA_API SElExceptionPtr _FUNCC Cognex_disconnect(SCameraPtr me);
CAMERA_API SElExceptionPtr _FUNCC Cognex_Trigger(SCameraPtr me);
CAMERA_API SElExceptionPtr _FUNCC Cognex_Read(SCameraPtr me, char *data);
CAMERA_API SElExceptionPtr _FUNCC Cognex_Online(SCameraPtr me);
CAMERA_API SElExceptionPtr _FUNCC Cognex_Offline(SCameraPtr me);
CAMERA_API SElExceptionPtr _FUNCC Cognex_act_job(SCameraPtr me, int *actual_job);
CAMERA_API SElExceptionPtr _FUNCC Cognex_Job(SCameraPtr me, const char *JobID);
CAMERA_API SElExceptionPtr _FUNCC Cognex_set_param(SCameraPtr me, int phase);
CAMERA_API SElExceptionPtr _FUNCC Cognex_set_number_boutton(SCameraPtr me, int phase);
CAMERA_API SElExceptionPtr _FUNCC Cognex_set_parameters(SCameraPtr me, char* colonne, int phase); 

#endif /* _AXIS_SPECIFIC_IAI_ */
