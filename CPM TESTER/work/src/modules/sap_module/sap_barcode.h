/*!
 *  SAP barcode interface.
 * 
 * Copyright (c) 2005 by Zdenek Rykala, ELCOM, a.s.
 */

#if !defined(__HANDBARCODEREADER_H__)
#define __HANDBARCODEREADER_H__

#if defined(__cplusplus) || defined(__cplusplus__)
	extern "C" {
#endif


#include <eltypes.h>
#include <windows.h>
#include <time.h>
#include <exception/exception.h>
#include "serial_configuration.h"

#define IBUTTON_CMD_SIZE				16
#define MAX_BARCODE_LENGTH				128

typedef struct _SBCItem {
	uint8_t		data[MAX_BARCODE_LENGTH+1];		/*!< 128 bytes code */
	time_t		TimeStamp;	/* Timestamp when ID has been received
							   The number of seconds elapsed since midnight
							   (00:00:00), January 1, 1970, coordinated
							   universal time (UTC), according to the system
							   clock.  */
} SBCItem, *SBCItemPtr;

typedef SElExceptionPtr (_FUNCC *BarcodeReaderCallbackPtr)(struct _SHBarcodeReader* me, const char* barcode);
typedef SElExceptionPtr (_FUNCC *LogCallbackPtr)(uint32_t  priority, const char* message);

/*!
 * Specifies test information
 */ 
typedef struct _SHBarcodeReader 
{
	CRITICAL_SECTION	cs;				/*!< Locks/Unlocks critical sesction */
	HANDLE				ThreadHandle;	/*!< Specifies current thread handle */
	HANDLE				QuitEvent;		/*!< Event which terminate test sequence thread */
	HANDLE				COMHandle;		/*!< Handle to iButton reader */
	SSerialIntfBase		COM;
	DWORD				ThreadID;
	SBCItem	           	item;
	bool_t				is_usb;

/*
 * PUBLIC FUNCTIONS
 */	
	SElExceptionPtr (*RegisterCallbackFnc)(struct _SHBarcodeReader* me, BarcodeReaderCallbackPtr CallbackFnc); /* low priority callbacks, can be disabled */
	SElExceptionPtr (*DisableCallbackFnc)(struct _SHBarcodeReader* me, bool_t disable);
	SElExceptionPtr (*RegisterCallbackFnc2)(struct _SHBarcodeReader* me, BarcodeReaderCallbackPtr CallbackFnc); /* high priority callback */
    SElExceptionPtr (*Run)(struct _SHBarcodeReader* me);
    SElExceptionPtr (*Terminate)(struct _SHBarcodeReader* me, int32_t timeout);
	SElExceptionPtr	(*GetLastItem)(struct _SHBarcodeReader* me, SBCItemPtr iButton);
	SElExceptionPtr (*Reset)(struct _SHBarcodeReader* me);
	SElExceptionPtr (*SetLogCallback)(struct _SHBarcodeReader* me, LogCallbackPtr callbackFnc);

/*
 * PRIVARE FUNCTIONS
 */	
	BarcodeReaderCallbackPtr* _CallBackFnc;
	int32_t				_CallBackFncSize;
	bool_t				_CallBackFncDisabled;
	BarcodeReaderCallbackPtr	_CallBackFnc2;
	LogCallbackPtr		_LogFnc;
} SHBarcodeReader, *SHBarcodeReaderPtr;

/*! \brief Creates a new instance of the iButton class */
SElExceptionPtr handbr_new(
	SHBarcodeReaderPtr*  pHBR
);
/*! \brief Destroys specified instance of the iButton */
SElExceptionPtr handbr_delete(SHBarcodeReaderPtr* pHBR);



#define HBR_ERROR_CONFIGURATION_MISSING			-12455

#if defined(__cplusplus) || defined(__cplusplus__)
	}
#endif


#endif /* __HANDBARCODEREADER_H__ */
