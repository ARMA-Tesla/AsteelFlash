/*!
 * iButton interface.
 * 
 * Copyright (c) 2005 by Zdenek Rykala, ELCOM, a.s.
 */

#if !defined(__IBUTTON_H__)
#define __IBUTTON_H__

#if defined(__cplusplus) || defined(__cplusplus__)
	extern "C" {
#endif


#include <eltypes.h>
#include <windows.h>
#include <time.h>
#include <exception/exception.h>

#define FLOW_CTRL_NONE			0
#define FLOW_CTRL_XON_XOFF		1
#define FLOW_CTRL_RTS_CTS		2
#define FLOW_CTRL_DTR_DSR		3
		
#define IBUTTON_CMD_SIZE				16

/*!
 * Defines the basic control setting for a serial communications device.
 */
typedef struct _SSerialIntfBase {
	uint16_t	port; 	  /*!< Specifies Port Binding 1 ... xxx */
	uint32_t	baudRate; /*!< Specifies baud rate */
	uint8_t		dataBits; /*!< Number of bits in the bytes transmitted and received. (7 or 8) */
	uint8_t		parity;   /*!< Parity scheme to be used. This member can be one of the following values. 
						       EVENPARITY
							   MARKPARITY
							   NOPARITY
							   ODDPARITY
							   SPACEPARITY 
						  */
	uint8_t		StopBits; /*!< Number of stop bits to be used. This member can be one of the following values. 
							   ONESTOPBIT
							   ONE5STOPBITS
							   TWOSTOPBITS
						  */
	uint8_t		FlowControl; /*!< Specifies flow control protocol. This member can be one of the following values. 
                                  FLOW_CTRL_NONE
                                  FLOW_CTRL_XON_XOFF
                                  FLOW_CTRL_RTS_CTS
								  FLOW_CTRL_DTR_DSR
							  */
} SSerialIntfBase, *SSerialIntfBasePtr;

typedef struct _SIButtonItem {
	uint8_t		ID[8];		/*!< 64 bit ID */
	time_t		TimeStamp;	/* Timestamp when ID has been received
							   The number of seconds elapsed since midnight
							   (00:00:00), January 1, 1970, coordinated
							   universal time (UTC), according to the system
							   clock.  */
} SIButtonItem, *SIButtonItemPtr;

typedef SElExceptionPtr (_FUNCC *iButtonCallbackPtr)(struct _SIButton* me, const char* ibutton);
typedef SElExceptionPtr (_FUNCC *LogCallbackPtr)(uint32_t  priority, const char* message);

/*!
 * Specifies test information
 */ 
typedef struct _SIButton 
{
	CRITICAL_SECTION	cs;				/*!< Locks/Unlocks critical sesction */
	HANDLE				ThreadHandle;	/*!< Specifies current thread handle */
	HANDLE				QuitEvent;		/*!< Event which terminate test sequence thread */
	HANDLE				COMHandle;		/*!< Handle to iButton reader */
	SSerialIntfBase		COM;
	DWORD				ThreadID;
	SIButtonItem		item;
	char				_resetCmd[IBUTTON_CMD_SIZE+1];
	char				_readCmd[IBUTTON_CMD_SIZE+1];

/*
 * PUBLIC FUNCTIONS
 */	
	SElExceptionPtr (*RegisterCallbackFnc)(struct _SIButton* me, iButtonCallbackPtr CallbackFnc); /* low priority callbacks, can be disabled */
	SElExceptionPtr (*DisableCallbackFnc)(struct _SIButton* me, bool_t disable);
	SElExceptionPtr (*RegisterCallbackFnc2)(struct _SIButton* me, iButtonCallbackPtr CallbackFnc); /* high priority callback */
    SElExceptionPtr (*Run)(struct _SIButton* me);
    SElExceptionPtr (*Terminate)(struct _SIButton* me, int32_t timeout);
	SElExceptionPtr	(*GetLastItem)(struct _SIButton* me, SIButtonItemPtr iButton);
	SElExceptionPtr (*Reset)(struct _SIButton* me);
	SElExceptionPtr (*SetLogCallback)(struct _SIButton* me, LogCallbackPtr callbackFnc);

/*
 * PRIVARE FUNCTIONS
 */	
	iButtonCallbackPtr* _CallBackFnc;
	int32_t				_CallBackFncSize;
	bool_t				_CallBackFncDisabled;
	iButtonCallbackPtr	_CallBackFnc2;
	LogCallbackPtr		_LogFnc;
} SIButton, *SIButtonPtr;

/*! \brief Creates a new instance of the iButton class */
SElExceptionPtr ibutton_new(
	SIButtonPtr*  pIButton
);
/*! \brief Destroys specified instance of the iButton */
SElExceptionPtr ibutton_delete(SIButtonPtr* pIButton);



#define IBUTTON_ERROR_CONFIGURATION_MISSING			-12455

#if defined(__cplusplus) || defined(__cplusplus__)
	}
#endif


#endif /* __IBUTTON_H__ */
