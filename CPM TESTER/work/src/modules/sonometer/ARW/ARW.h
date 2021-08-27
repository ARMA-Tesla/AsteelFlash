
#ifndef _SONOMETER_SPECIFIC_TOSHIBA_
#define _SONOMETER_SPECIFIC_TOSHIBA_

#include <sonometer/class/sonometers.h>

#if defined(WINDOWS)
	#define SONOMETER_API		__declspec( dllexport )
#elif defined(UNIX)
	#define SONOMETER_API		extern "C"
#else
	#define SONOMETER_API
#endif

SONOMETER_API SElExceptionPtr _FUNCC Connect(SSonometerPtr me);
SONOMETER_API SElExceptionPtr _FUNCC Disconnect(SSonometerPtr me);
SONOMETER_API SElExceptionPtr _FUNCC Write(SSonometerPtr me,const char* command);
SONOMETER_API SElExceptionPtr _FUNCC Read(SSonometerPtr me,const char* data);
SONOMETER_API SElExceptionPtr _FUNCC SoundLevelMeter(SSonometerPtr me, int group, float *data, int *count_data, char *log);

#endif /* _SONOMETER_SPECIFIC_TOSHIBA_ */
