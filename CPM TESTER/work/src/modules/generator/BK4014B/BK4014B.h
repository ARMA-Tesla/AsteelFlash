
#ifndef _GENERATOR_SPECIFIC_
#define _GENERATOR_SPECIFIC_

#include <generator/class/generators.h>

#if defined(WINDOWS)
	#define GENERATOR_API		__declspec( dllexport )
#elif defined(UNIX)
	#define GENERATOR_API		extern "C"
#else
	#define GENERATOR_API
#endif

GENERATOR_API SElExceptionPtr _FUNCC Connect(SGeneratorPtr me);
GENERATOR_API SElExceptionPtr _FUNCC Disconnect(SGeneratorPtr me);
GENERATOR_API SElExceptionPtr _FUNCC Write(SGeneratorPtr me,const char* command);
GENERATOR_API SElExceptionPtr _FUNCC Read(SGeneratorPtr me,const char* read);
GENERATOR_API SElExceptionPtr _FUNCC SetModeContinuous(SGeneratorPtr me);
GENERATOR_API SElExceptionPtr _FUNCC SetOutput(SGeneratorPtr me, int output);
GENERATOR_API SElExceptionPtr _FUNCC SetFonction(SGeneratorPtr me, double frequency, double amplitude, double offset, double duty);
GENERATOR_API SElExceptionPtr _FUNCC SystemError(SGeneratorPtr me, char * error);
GENERATOR_API SElExceptionPtr _FUNCC ClearError(SGeneratorPtr me);

#endif /* _GENERATOR_SPECIFIC_ */
