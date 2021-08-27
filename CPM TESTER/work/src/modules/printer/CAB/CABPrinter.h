
#ifndef _PRINTER_SPECIFIC_TOSHIBA_
#define _PRINTER_SPECIFIC_TOSHIBA_

#include <printer/class/printers.h>

#if defined(WINDOWS)
	#define PRINTER_API		__declspec( dllexport )
#elif defined(UNIX)
	#define PRINTER_API		extern "C"
#else
	#define PRINTER_API
#endif

PRINTER_API SElExceptionPtr _FUNCC Connect(SPrinterPtr me);
PRINTER_API SElExceptionPtr _FUNCC Disconnect(SPrinterPtr me);
PRINTER_API SElExceptionPtr _FUNCC Write(SPrinterPtr me,const char* command);
PRINTER_API SElExceptionPtr _FUNCC Reset(SPrinterPtr me);
PRINTER_API SElExceptionPtr _FUNCC Status(SPrinterPtr me,  int *StatusOnline, char *Error, int *iAmmountLabels, int *StatusInterpreter);

#endif /* _PRINTER_SPECIFIC_TOSHIBA_ */
