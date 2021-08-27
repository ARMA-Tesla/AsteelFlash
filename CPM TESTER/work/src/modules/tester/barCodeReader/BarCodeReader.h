
#if !defined(__EL_BCR_CLASS_H__)
#define __EL_BCR_CLASS_H__

#include <exception/Exception.h>
#include <windows.h>
#include <eventfactory/EventFactory.h>

#define BCR_ERROR_WRITE_TIMEOUT				(-2000L)
#define BCR_ERROR_INVALID_READ_EVENT		(-2001L)
#define BCR_ERROR_INVALID_PARAMETER			(-2002L)
#define BCR_ERROR_INVALID_CONFIGURATION		(-2003L)
#define BCR_ERROR_OPEN_FAILED				(-2004L)

#define MAX_RESOURCE_NAME					256
#define MAX_BCR_BUFFER						256

#define EVNT_BARCODE_READ					"EVNT_BARCODE_READ"
#define FILE_NAME_SIZE						2048

#if defined(WINDOWS)
	#define BCR_API		__declspec( dllexport )
#elif defined(UNIX)
	#define BCR_API		extern "C"
#else
	#define BCR_API
#endif

typedef struct _BarCodeReader
{
	SElExceptionPtr (*Initialize)
	(
		struct _BarCodeReader*	me,
		const char*				configString
	);
	SElExceptionPtr (*Close)(struct _BarCodeReader* me);
	SElExceptionPtr (*StartReading)(struct _BarCodeReader* me);
	SElExceptionPtr (*StopReading)(struct _BarCodeReader* me);
	SElExceptionPtr (*GetBarCode)
	(
		struct _BarCodeReader*	me,
		char*					buffer,
		int32_t					bufferSize
	);
	SElExceptionPtr (*WriteData)
	(
		struct _BarCodeReader*	me,
		char*					data
	);
	SElExceptionPtr (*RegisterReadCallback)
	(
		struct _BarCodeReader*	me,
		SElExceptionPtr			(*callbackFnc)(struct _BarCodeReader*,char*,char*)
	);
	
	/********************************/
	/* Private                      */
	/********************************/
	int         port;
	char*		idBCR;
	char		resourceName[MAX_RESOURCE_NAME];
	char		barCodeBuffer[MAX_BCR_BUFFER];
	int32_t		bufferPosition;
	HANDLE		fd;
	HANDLE		endEvent;
	HANDLE		threadHandle;
	CRITICAL_SECTION	lock;
	SElEventFactoryPtr	eventFactory;
	
	SElExceptionPtr	(*_callbackFnc)(struct _BarCodeReader* me,char* idString,char* barCode);
	SElExceptionPtr (*_ReadThread)(struct _BarCodeReader* me);
	
} SBarCodeReader, *SBarCodeReaderPtr;


BCR_API SElExceptionPtr _FUNCC bcr_new( SBarCodeReaderPtr* pBcr);	
BCR_API SElExceptionPtr	_FUNCC bcr_delete( SBarCodeReaderPtr* pBcr);

#endif /* __EL_BCR_CLASS_H__ */
