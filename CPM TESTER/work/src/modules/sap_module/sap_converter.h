#if !defined(__SAP_CONVERTER_H__)
#define __SAP_CONVERTER_H__

#include <windows.h>
#include <exception/Exception.h>
#include "serial_configuration.h"

/*!
 * Specifies test information
 */ 
typedef struct _SSAPConverter 
{
	CRITICAL_SECTION	lock;				/*!< Locks/Unlocks critical sesction */
	HANDLE				COMHandle;		/*!< Handle to iButton reader */
	SSerialIntfBase		COM;

/*
 * PUBLIC FUNCTIONS
 */	
    SElExceptionPtr (*Open)(struct _SSAPConverter* me);
    SElExceptionPtr (*Close)(struct _SSAPConverter* me);
	SElExceptionPtr (*Start)(struct _SSAPConverter* me);
    SElExceptionPtr (*Stop)(struct _SSAPConverter* me);
	SElExceptionPtr (*Write)(struct _SSAPConverter* me, const char* pdata);
	SElExceptionPtr (*WriteRaw)(struct _SSAPConverter* me, size_t length, const unsigned char* pdata);
	SElExceptionPtr (*_ReadThread)(struct _SSAPConverter* me);
/*
 * PRIVARE FUNCTIONS
 */	
	HANDLE		QuitEvent;
	HANDLE		threadHandle;
} SSAPConverter, *SSAPConverterPtr;

/*! \brief Creates a new instance of the SAP Converter class */
SElExceptionPtr sapconverter_new(
	SSAPConverterPtr*  pSAP
);
/*! \brief Destroys specified instance of the SAP Converter */
SElExceptionPtr sapconverter_delete(
	SSAPConverterPtr*  pSAP
);

#define SAPC_ERROR_INVALID_READ_EVENT	-1111
#define SAPC_ERROR_OPEN_FAILED			-2222
#define SAPC_ERROR_WRITE_TIMEOUT		-3333

#endif /* __SAP_CONVERTER_H__ */
