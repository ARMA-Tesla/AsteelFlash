
#ifndef _DIELECTRIMETER_SPECIFIC_
#define _DIELECTRIMETER_SPECIFIC_

#include <dielectrimeter/class/dielectrimeters.h>

#if defined(WINDOWS)
	#define DIELECTRIMETER_API		__declspec( dllexport )
#elif defined(UNIX)
	#define DIELECTRIMETER_API		extern "C"
#else
	#define DIELECTRIMETER_API
#endif

DIELECTRIMETER_API SElExceptionPtr _FUNCC Connect(SDielectrimeterPtr me);
DIELECTRIMETER_API SElExceptionPtr _FUNCC Disconnect(SDielectrimeterPtr me);
DIELECTRIMETER_API SElExceptionPtr _FUNCC Write(SDielectrimeterPtr me,const char* command);
DIELECTRIMETER_API SElExceptionPtr _FUNCC Read(SDielectrimeterPtr me,const char* read);
DIELECTRIMETER_API SElExceptionPtr _FUNCC SetModeManual(SDielectrimeterPtr me);
DIELECTRIMETER_API SElExceptionPtr _FUNCC SetStep(SDielectrimeterPtr me, int step);
DIELECTRIMETER_API SElExceptionPtr _FUNCC SetOutput(SDielectrimeterPtr me, int output);
DIELECTRIMETER_API SElExceptionPtr _FUNCC TestResult(SDielectrimeterPtr me, int *result, double *voltage_kv, double *current_mA);
DIELECTRIMETER_API SElExceptionPtr _FUNCC SystemError(SDielectrimeterPtr me, char * error);
DIELECTRIMETER_API SElExceptionPtr _FUNCC ClearError(SDielectrimeterPtr me);

#endif /* _DIELECTRIMETER_SPECIFIC_ */
