/****************************************************************************
 *                              DBUS
 *---------------------------------------------------------------------------
 *    Copyright  @ 2020 ARMA, a.s.  All Rights Reserved.        
 *---------------------------------------------------------------------------
 *                                                                          
 * Title:       dbus.c                                                    
 * Purpose:     DBUS interface (Controled Area Network)
 ****************************************************************************/

#include "dbus.h"
#include <userint.h>
#include <formatio.h>
#include <ansi_c.h>
#include <elerror.h>
#include <stdio.h>
#include <time.h>
#include "MMSystem.h"


#define DBUS_READ		0
#define DBUS_WRITE		1

#define MAX_PORT_UDA	10
#define MAX_BYTES		256
#define MAX_DATA_LENGTH	MAX_BYTES*2

#define TIMEOUT_INIT() 	clock_t	_start_clock = clock();

#define TIMEOUT_CHECK(timeout) \
	if ( ((clock() - _start_clock) / (double)CLOCKS_PER_SEC) > (timeout/1000.0) ) { EXCTHROW( DBUS_ERROR_TIMEOUT, "DBUS Timeout"); } else

/*****************************************************************************
 * Imported Functions  **********************************
 *****************************************************************************/	
static VResult __stdcall (*_dbusGetAvailablePortCount)(unsigned long *);
static VResult __stdcall (*_dbusGetAvailablePort)(unsigned long, struct VDBusPortInfo*);
static VResult __stdcall (*_dbusOpenPort)(VDBusPortId, unsigned long, unsigned long, VDBusPortHandle*);
static VResult __stdcall (*_dbusClosePort)(VDBusPortHandle);
static VResult __stdcall (*_dbusSetSimulatedDevices)(VDBusPortHandle, WORD );
static VResult __stdcall (*_dbusSendFrame)(VDBusPortHandle, unsigned long, void*, unsigned long);
static VResult __stdcall (*_dbusGetFrameData)(VDBusPortHandle, void *, unsigned long, unsigned long*);
static VResult __stdcall (*_dbusGetFrameInfo)(VDBusPortHandle, struct VDBusFrameInfo *);

/*****************************************************************************
 * Static no-exported functions declaration **********************************
 *****************************************************************************/
static SElExceptionPtr dbusLock(struct _SDBUS* me);
static SElExceptionPtr dbusUnlock(struct _SDBUS* me);
static SElExceptionPtr dbus_lock(struct _SDBUS* me, uint32_t id);
static SElExceptionPtr dbus_unlock(struct _SDBUS* me, uint32_t id);

static SElExceptionPtr dbusOpen(
    struct _SDBUS*	me,
    const char*		UDA_name,
    const char*		portCommPartner,
	const char*		PortFlags
);
static SElExceptionPtr dbusClose(struct _SDBUS* me);

static SElExceptionPtr dbusWriteLowLevel(
	struct _SDBUS*	me,
	uint8_t	        target_adress,
    char        	*pMessageID,
    char        	*pMessageData
);
static SElExceptionPtr dbusClearCache(struct _SDBUS* me);
static const char* dbusGetCache(struct _SDBUS* me);

static SElExceptionPtr dbusReadLowLevel(
    struct _SDBUS*	me,
	uint8_t	       	target_adress,
	char        	*pMessageID,
    char        	*pMessageData
);

static SElExceptionPtr AppendCacheData(
    struct _SDBUS*   	me,
	int32_t         	dType,
	uint8_t	        	target_adress,
	char        		*pMessageID,
    char        		*pMessageData
);
#if defined(_CVI_)
int CVICALLBACK SecondThreadFunc (void *pArguments);
#else
unsigned __stdcall SecondThreadFunc( void* pArguments );
#endif

/*---------------------------------------------------------------------------*/
static SElExceptionPtr dbuslistAddChannel(
	struct _SDBUSList*	me,
	const char*			UDA_name
);
static SElExceptionPtr dbuslistRemoveChannel(
	struct _SDBUSList*	me,
	const char*			UDA_name
);

static SElExceptionPtr dbuslistOpen(
	struct _SDBUSList*	me,
	const char*			UDA_name,
    const char*			portCommPartner,
	const char*			PortFlags
);
static SElExceptionPtr dbuslistClose(
	struct _SDBUSList*	me,
	const char*			UDA_name
);

static SElExceptionPtr dbuslistWriteLowLevel(
	struct _SDBUSList*	pDBUS,
	const char*			UDA_name,
	uint8_t	        	target_adress,
    char        		*pMessageID,
    char        		*pMessageData
);

static SElExceptionPtr dbuslistReadLowLevel(
	struct _SDBUSList*	pDBUS,
	const char*			UDA_name,
	uint8_t		        target_adress,
	char        		*pMessageID,
    char        		*pMessageData
);
	
static SElExceptionPtr dbuslistClearCache(struct _SDBUSList* me, const char* UDA_name);
static const char* dbuslistGetCache(struct _SDBUSList* me, const char* UDA_name);
static SElExceptionPtr DestroyLockIds(
	void** value
);

VDBusPortFlags GetVDBusPortFlags (char* VDBusPortFlags)
{
if (strcmp(VDBusPortFlags, "VDBUS_FLAG_NO_COLLISION"))	
	return VDBUS_FLAG_NO_COLLISION;
if (strcmp(VDBusPortFlags, "VDBUS_FLAG_CLEAR_RX_QUEUE_BEFORE_SEND"))	
	return VDBUS_FLAG_CLEAR_RX_QUEUE_BEFORE_SEND;
if (strcmp(VDBusPortFlags, "VDBUS_FLAG_CRC8"))	
	return VDBUS_FLAG_CRC8;
if (strcmp(VDBusPortFlags, "VDBUS_FLAG_4800_BAUD"))	
	return VDBUS_FLAG_4800_BAUD;
if (strcmp(VDBusPortFlags, "VDBUS_FLAG_19200_BAUD"))	
	return VDBUS_FLAG_19200_BAUD;
if (strcmp(VDBusPortFlags, "VDBUS_FLAG_RETRY_AFTER_MISSING_ACK"))	
	return VDBUS_FLAG_RETRY_AFTER_MISSING_ACK;
if (strcmp(VDBusPortFlags, "VDBUS_FLAG_RETRY_AFTER_MISSING_ACK"))	
	return VDBUS_FLAG_RETRY_AFTER_MISSING_ACK;
if (strcmp(VDBusPortFlags, "VDBUS_FLAG_RTS_LOW"))	
	return VDBUS_FLAG_RTS_LOW;
if (strcmp(VDBusPortFlags, "VDBUS_FLAG_DTR_HIGH"))	
	return VDBUS_FLAG_DTR_HIGH;
if (strcmp(VDBusPortFlags, "VDBUS_FLAG_RAW_MODE_NO_TS"))	
	return VDBUS_FLAG_RAW_MODE_NO_TS;
if (strcmp(VDBusPortFlags, "VDBUS_FLAG_RAW_MODE_SINGLE_BYTES"))	
	return VDBUS_FLAG_RAW_MODE_SINGLE_BYTES;
else 
		return VDBUS_FLAG_9600_BAUD; //default
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "LoadLibrary_DBUS"
SElExceptionPtr LoadLibrary_DBUS(const char* libary_name)
{
	SElExceptionPtr	pexception = NULL;
	HMODULE hModule;
	
	hModule = (HMODULE)LoadLibrary(libary_name);
		if (!hModule)
			EXCTHROW(-1, "Erreur Chargement DBusDrv.dll");

	_dbusGetAvailablePortCount = (void*)GetProcAddress(hModule,"dbusGetAvailablePortCount");				
	_dbusGetAvailablePort = (void*)GetProcAddress(hModule,"dbusGetAvailablePort");				
	_dbusOpenPort = (void*)GetProcAddress(hModule,"dbusOpenPort");				
	_dbusClosePort= (void*)GetProcAddress(hModule,"dbusClosePort");
	_dbusSetSimulatedDevices= (void*)GetProcAddress(hModule,"dbusSetSimulatedDevices");
	_dbusSendFrame= (void*)GetProcAddress(hModule,"dbusSendFrame");
	_dbusGetFrameData= (void*)GetProcAddress(hModule,"dbusGetFrameData");
	_dbusGetFrameInfo = (void*)GetProcAddress(hModule,"dbusGetFrameInfo");

Error:
	EXCRETHROW(pexception);
}/* LoadLibrary_DBUS */

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "dbus_new"
/*!
 * \brief
 * This function creates a new instance.
 * 
 * \param pError 
 * Returns a value indicating whether an error occurred.
 * A negative error code indicates function failure.	
 * 
 * \return 
 * Pointer to new instance of the DBUS object. If the function fails, NULL
 * pointer is returned
 *
 * This function creates a new instance of the DBUS object.
 */
SElExceptionPtr dbus_new(SDBUSPtr* pDBUS)
{
	SElExceptionPtr	pexception = NULL;
	SDBUSPtr			pdbus = NULL;


	pdbus = calloc( 1, sizeof(SDBUS));
	EXCCHECKALLOC( pdbus);

	pdbus->Open  = dbusOpen;
	pdbus->Close = dbusClose;

	pdbus->WriteLowLevel = dbusWriteLowLevel;  
	pdbus->ReadLowLevel  = dbusReadLowLevel;
	
	pdbus->ClearCache = dbusClearCache;
	pdbus->GetCache = dbusGetCache;
	
	InitializeCriticalSection(&(pdbus->_Lock));
	InitializeCriticalSection(&(pdbus->_LockId));
	objlist_new(&pdbus->_LockIds);
	
	pdbus->_LockIds->RegisterDestroy(pdbus->_LockIds, DestroyLockIds);

	pdbus->_timeout = 2000;

#if 1
	EXCCHECK( elstring_new(NULL, &(pdbus->_pCache)));
	EXCCHECK( timer_new( &(pdbus->_pTimer)));
#else
	pdbus->_pCache = NULL;
	pdbus->_pTimer = NULL;
#endif

Error:
	if ( pDBUS != NULL )
	{
		*pDBUS = pdbus;
	}
	
	EXCRETHROW(pexception);
}/* dbus_new */

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "dbus_delete"
/*!
 * \brief
 * This function destroys specified instance.
 * 
 * \param pDBUS 
 * Pointer to instance of the DBUS object cretaed by dbus_new() function.
 * 
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  None\n\n
 *  ERRORS:\n
 *  None\n 
 * 
 * This function destroys specified instance and release all resources.
 */
SElExceptionPtr dbus_delete(SDBUSPtr* pDBUS)
{
	SElExceptionPtr	pexception = NULL;

	if ( pDBUS && *pDBUS )
	{
		EXCCHECK( (*pDBUS)->Close(*pDBUS));
		DeleteCriticalSection( &((*pDBUS)->_Lock));
		DeleteCriticalSection( &((*pDBUS)->_LockId));
		objlist_delete(&((*pDBUS)->_LockIds));

#if 0
		EXCCHECK( elstring_delete( &((*pDBUS)->_pCache)));
		EXCCHECK( timer_delete( &((*pDBUS)->_pTimer)));
#endif

		free( *pDBUS);
		*pDBUS = NULL;
	}

Error:
	EXCRETHROW(pexception);
} /* dbus_delete */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbusOpen"
/*!
 * \brief
 * This function opens DBUS interface
 * 
 * \param pDBUS
 * Pointer to instance of the DBUS object cretaed by dbus_new() function.
 * 
 * \patam UDA_name   Reserved for future use, USe NULL or empty string
 * \patam options   Specifies the DBUS parameters as Low or High speed
 *                  and baud rate.
 *                  Format is following:\n
 *                  <Port>:<H|L><baudrate>\n
 *                  More ports dbus be spearated by semicolon\n
 *                  For example:\n
 *                  "DBUS1:H500000;DBUS2:L100000"
 *                  Dbus 1 is high dbus witk speed 500kBauds and DBUS2 is low speed dbus with 100kBauds
 * 
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  None\n\n
 *  ERRORS:\n
 *  None\n 
 *
 */
static SElExceptionPtr dbusOpen(
	struct _SDBUS*	me,
    const char*		UDA_name,
    const char*		portCommPartner,
	const char*		PortFlags
)
{
	SElExceptionPtr	pexception = NULL;
	unsigned long count = 0;
	struct VDBusPortInfo 	infoPort;
	int			   iportCommPartner;
	VDBusPortFlags iPortFlags;
	VResult 	   Ret;
	char           msg[128];
	
	/* Read available dbus ports */
	_dbusGetAvailablePortCount( &count );

	for( unsigned long index = 0; index < count; index++ ) 
	{
		if (_dbusGetAvailablePort( index, &infoPort ) == VDBUS_SUCCESS) 
		{
			 if (strcmp(infoPort.portText, UDA_name)==0 )
			 {
				 if ( infoPort.portLocked == FALSE )
			 	 {
					/* open dbus port */
					sscanf(portCommPartner, "0x%x", &iportCommPartner);
					iPortFlags = GetVDBusPortFlags (PortFlags);
				
						Ret = _dbusOpenPort( infoPort.portId , iportCommPartner, VDBUS_FLAG_CLEAR_RX_QUEUE_BEFORE_SEND, &me->_PortHandle );
						if (Ret != 	VDBUS_SUCCESS) 
						{
							sprintf(msg, "Port %s Open Port error %d", UDA_name, Ret);
							EXCTHROW(DBUS_ERROR_OPEN_PORT, msg);
						}
						else 
						{
							break;
						}
				 }
				  else 
			 	 {
					sprintf(msg, "Port %s locked by another application", UDA_name);
					EXCTHROW(DBUS_PORT_LOCKED, msg);
				 }
			 }
			 else if (count == index+1)
			 {
				sprintf(msg, "Port %s not found", UDA_name);
				EXCTHROW(DBUS_PORT_NOT_FOUND, msg);
			 }
		}
		else 
		{
			sprintf(msg, "Port %s Not available", UDA_name);
			EXCTHROW(DBUS_PORT_NOT_AVAILABLE, msg);
		}
	}

	
Error:
	EXCRETHROW(pexception);
} /* dbusOpen */

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "dbusClose"
/*!
 * \brief
 * This function closes DBUS interface
 * 
 * \param pDBUS
 * Pointer to instance of the DBUS object cretaed by dbus_new() function.
 * 
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  None\n\n
 *  ERRORS:\n
 *  None\n 
 */
static SElExceptionPtr dbusClose(struct _SDBUS* me)
{
	SElExceptionPtr	pexception = NULL;
	
	/* close dbus port */
	_dbusClosePort(me->_PortHandle);
	
Error:
	EXCRETHROW(pexception);
} /* dbusClose */


/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "dbusWriteLowLevel"
/*!
 * \brief
 * This function writes data to DBUS interface
 * 
 * \param pDBUS
 * Pointer to instance of the DBUS object cretaed by dbus_new() function.
 * 
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  None\n\n
 *  ERRORS:\n
 *  None\n 
 *
 */
static SElExceptionPtr dbusWriteLowLevel(
	struct _SDBUS*  me,
	uint8_t	        target_adress,
    char        	*pMessageID,
    char        	*pMessageData
)
{
	SElExceptionPtr		pexception = NULL;
	BYTE 				data[MAX_BYTES];
	BYTE				targetAddr = 0x21;
	int 				i;
	VResult 			Ret;
	char 				byte_String[2+1];
	int 				length1, length2;
	int 				byte_decimal;
	int 				size;
	char                msg[128];
		
	/* send frame dbus */
	targetAddr = (BYTE)target_adress;
			
	length1 = strlen(pMessageID);
	length2 = strlen(pMessageData);	
	
	memset(data, 0x00, MAX_BYTES);
	
	for (i=0; i<length1;i+=2)
		{
			CopyBytes(byte_String, 0, pMessageID, i, 2);
			byte_String[2]= 0x00;
			sscanf(byte_String, "%02X", &byte_decimal);
			data [i/2] = byte_decimal;
		}
	for (; i<length1+length2; i+=2)
		{
			CopyBytes(byte_String, 0, pMessageData, i-length1, 2);
			byte_String[2]= 0x00;
			sscanf(byte_String, "%02X", &byte_decimal);
			data [i/2] = byte_decimal;
		}
				
	// port was opened successfully
	size = (length1+length2)/2;
	Ret = _dbusSendFrame( me->_PortHandle, targetAddr, data, size);
	if (Ret != VDBUS_SUCCESS) 
	{
		sprintf(msg, "Port %s error send frame ERROR = %d \r\n Target adress : 0x%2X Message ID : %s Message data : %s", 
				me->_UDA_name, Ret, targetAddr, pMessageID, pMessageData);
		EXCTHROW(DBUS_ERROR_SEND_FRAME_DATA, msg);	
	}
	
Error:
	AppendCacheData(me, DBUS_WRITE, target_adress, pMessageID, pMessageData);
	EXCRETHROW(pexception);
} /* dbusWriteLowLevel */

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "dbusReadLowLevel"
/*!
 * \brief
 * This function reads data from DBUS interface
 * 
 * \param pDBUS
 * Pointer to instance of the DBUS object cretaed by dbus_new() function.
 * 
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  None\n\n
 *  ERRORS:\n
 *  None\n 
 *
 */
static SElExceptionPtr dbusReadLowLevel(
	struct _SDBUS*	me,
	uint8_t	        target_adress,
	char        	*pMessageID,
    char        	*pMessageData
)
{
	SElExceptionPtr		pexception = NULL;
	struct 				VDBusFrameInfo info;
	BYTE 				data[255];
	ULONG 				count;
	int 				i;
	char 				byte [5+1];
	VResult 			Ret;
	char                msg[128];
	char                MessageID[4+1] = "";
	char                MessageData[256]= "";
	
	/* dbus Read Frame */
	
	Ret = _dbusGetFrameInfo( me->_PortHandle, &info);	
	if (Ret == VDBUS_SUCCESS) 
		{
		Ret = _dbusGetFrameData( me->_PortHandle, data,sizeof(data), &count );
		if (Ret == VDBUS_SUCCESS) 
			{
			if (info.ackType == VDBUS_ACK_OK) 
				{
				if (info.targetAddress == target_adress ) 
					{
					memset(MessageID, 0x00, 5); 
					memset(MessageData, 0x00, 256); 
					
					for (i=0; i< 2; i++)
						{
							sprintf (byte, "%02X", data[i]);
							strcat(MessageID, byte);
						}
					for (i=2; i< count; i++)
						{
							sprintf (byte, "%02X", data[i]);
							strcat(MessageData, byte);
						}
						
					}	
				else
					{
						sprintf(msg, "Port %s error Target adress not found! \r\n ('Traget found = 0x%2X')", me->_UDA_name, info.targetAddress);
						EXCTHROW(DBUS_ERROR_READ_TARGET_ADRESS, msg);		
					}
				}
			else
				{
					sprintf(msg, "Port %s error frame read Acknowledge types \r\n ERROR = %d", me->_UDA_name, info.ackType);
					EXCTHROW(DBUS_ERROR_READ_FRAME_DATA, msg);		
				}
			}
			
		else
			{
				sprintf(msg, "Port %s error read frame data!", me->_UDA_name);
				EXCTHROW(DBUS_ERROR_READ_FRAME_DATA, msg);		
			}
		}
	else 
		{
			sprintf(msg, "Port %s no frame info!", me->_UDA_name);
			EXCTHROW(DBUS_NO_FRAME_INFOS, msg);	
		}
	
	sprintf(pMessageID, "%s\0", MessageID);
	sprintf(pMessageData, "%s\0", MessageData);
	
Error:
	AppendCacheData(me, DBUS_READ, target_adress, MessageID, MessageData);
	EXCRETHROW(pexception);
} /* dbusReadLowLevel */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbusClearCache"
/*!
 * \brief
 * This function clears allocated cache.
 * 
 * \param pDBUS
 * Pointer to instance of the DBUS object cretaed by dbus_new() function.
 * 
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  None\n\n
 *  ERRORS:\n
 *  None\n 
 *
 * This function clears cache.
 */
static SElExceptionPtr dbusClearCache(struct _SDBUS* me)
{
	SElExceptionPtr		pexception = NULL;

	if ( me->_pCache != NULL )
    {
        EXCCHECK( (me->_pCache)->Empty(me->_pCache));
    }

Error:
	EXCRETHROW(pexception);
} /* dbusClearCache */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbusGetCache"
/*!
 * \brief
 * This function returns pointer to allocated string.
 * 
 * \param pDBUS
 * Pointer to instance of the DBUS object cretaed by dbus_new() function.
 * 
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  None\n\n
 *  ERRORS:\n
 *  None\n 
 *
 * This function returns pointer to string witch cache data.
 */
static const char* dbusGetCache(struct _SDBUS* me)
{
    static char*  pempty_cache = "";
    if ( me->_pCache == NULL )
    {
        return pempty_cache;
    }
    return  (me->_pCache)->GetPtr(me->_pCache);
} /* dbusGetCache */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbusLock"
static SElExceptionPtr dbusLock(struct _SDBUS* me)
{
	EnterCriticalSection( &(me->_Lock));

	return NULL;
} /* dbusLock */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbusUnlock"
static SElExceptionPtr dbusUnlock(struct _SDBUS* me)
{
	LeaveCriticalSection( &(me->_Lock));

	return NULL;
} /* dbusUnlock */

#define LOCK_TIMEOUT	20 /* timeout in ms */

typedef struct _SLock
{
	clock_t				time;
	CRITICAL_SECTION	lock;
} SLock, *SLockPtr;

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "DestroyLockIds"
static SElExceptionPtr DestroyLockIds(void** value)
{
	SLockPtr plock = *value;

	DeleteCriticalSection(&(plock->lock));
	free(plock);
	*value = NULL;

	return NULL;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbus_lock"
static SElExceptionPtr dbus_lock(struct _SDBUS* me, uint32_t id)
{
	EnterCriticalSection( &(me->_LockId));

	{
		char sid[16];
		SLockPtr plock = NULL;

		sprintf(sid, "%u", id);
		plock = me->_LockIds->Get(me->_LockIds, sid);
		if(plock==NULL)
		{
			plock = (SLockPtr)calloc(1, sizeof(SLock));
			me->_LockIds->Set(me->_LockIds, sid, plock);

			InitializeCriticalSection( &(plock->lock));
		}
		plock->time = clock();
		LeaveCriticalSection( &(me->_LockId));

		EnterCriticalSection( &(plock->lock));
	}
	
	return NULL;
} /* dbus_lock */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbus_unlock"
static SElExceptionPtr dbus_unlock(struct _SDBUS* me, uint32_t id)
{
	EnterCriticalSection( &(me->_LockId));

	{
		char sid[16];
		SLockPtr plock = NULL;
		sprintf(sid, "%u", id);
		plock = me->_LockIds->Get(me->_LockIds, sid);
		if(plock!=NULL)
		{
#ifdef _DEBUG
			double	expired_time = ((clock() - (plock->time)) / (double)CLOCKS_PER_SEC);

			if(expired_time > (LOCK_TIMEOUT/1000.0)) 
			{
				char dbgstring[1024];
				sprintf(dbgstring, "[DBUS] Lock timeout expired [%dms] for resource '0x%X': %1.3fs\n", LOCK_TIMEOUT, id, expired_time);
				OutputDebugString(dbgstring);
			}
#endif
			LeaveCriticalSection( &(plock->lock));
		}
	}

	LeaveCriticalSection( &(me->_LockId));
	return NULL;
} /* dbus_unlock */


/*****************************************************************************/
#undef  __FUNC__
#define __FUNC__ "dbuslist_new"
/*!
 * \brief
 * This function creates a new instance.
 * 
 * \param pError 
 * Returns a value indicating whether an error occurred.
 * A negative error code indicates function failure.	
 * 
 * \return 
 * Pointer to new instance of the DBUS object. If the function fails, NULL
 * pointer is returned
 *
 * This function creates a new instance of the DBUS object.
 */
SElExceptionPtr dbuslist_new(SDBUSListPtr* pDBUS)
{
	SElExceptionPtr	pexception = NULL;
	SDBUSListPtr		pdbus_list = NULL;
	
	pdbus_list = calloc( 1, sizeof(SDBUSList));
	EXCCHECKALLOC( pdbus_list);

	pdbus_list->AddChannel  = dbuslistAddChannel;
	pdbus_list->RemoveChannel = dbuslistRemoveChannel;
	pdbus_list->Open  = dbuslistOpen;
	pdbus_list->Close = dbuslistClose;
	pdbus_list->WriteLowLevel = dbuslistWriteLowLevel;
	pdbus_list->ReadLowLevel  = dbuslistReadLowLevel;
	pdbus_list->ClearCache = dbuslistClearCache;
	pdbus_list->GetCache = dbuslistGetCache;

	EXCCHECK( objlist_new(&(pdbus_list->_udaList)));
	EXCCHECK( (pdbus_list->_udaList)->RegisterDestroy(pdbus_list->_udaList, dbus_delete));
	
	EXCCHECK( elstring_new(NULL, &(pdbus_list->_pCache)));
	EXCCHECK( timer_new( &(pdbus_list->_pTimer)));

Error:
	if ( pDBUS != NULL )
	{
		*pDBUS = pdbus_list;
	}
	
	EXCRETHROW(pexception);
}/* dbuslist_new */

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "dbuslist_delete"
/*!
 * \brief
 * This function destroys specified instance.
 * 
 * \param pDBUS 
 * Pointer to instance of the DBUS object cretaed by dbus_new() function.
 * 
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  None\n\n
 *  ERRORS:\n
 *  None\n 
 * 
 * This function destroys specified instance and release all resources.
 */
SElExceptionPtr dbuslist_delete(SDBUSListPtr* pDBUS)
{
	SElExceptionPtr	pexception = NULL;

	if ( pDBUS && *pDBUS )
	{
		EXCCHECK( objlist_delete( &((*pDBUS)->_udaList)));
		EXCCHECK( elstring_delete( &((*pDBUS)->_pCache)));
		EXCCHECK( timer_delete( &((*pDBUS)->_pTimer)));

		free( *pDBUS);
		*pDBUS = NULL;
	}

Error:
	EXCRETHROW(pexception);
} /* dbuslist_delete */


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#undef __FUNC__
#define __FUNC__ "AppendCacheData"
static SElExceptionPtr AppendCacheData(
    struct _SDBUS*   me,
    int32_t         dType,
	uint8_t	       	target_adress,
	char        	*pMessageID,
    char        	*pMessageData
)
{
	SElExceptionPtr	pexception = NULL;
	char 			Log [256];
	
	
	dbusLock(me);

	{
		const char* prefix = (dType == DBUS_WRITE ) ? "[%0.3Lf ms] TX " : "[%0.3Lf ms] RX "; 
	
		EXCCHECK( (me->_pCache)->Format(me->_pCache, prefix,
										(me->_pTimer)->GetTimeDiff( me->_pTimer)
										));
		//strncat (me->_pCache->_p_data, Log, strlen(Log));
	}
	
			
	sprintf (Log, "%s::%02x::%s::%s", me->_UDA_name, target_adress, pMessageID, pMessageData);

	strncat (me->_pCache->_p_data, Log, strlen(Log));
	//strncat (me->_pCache->_p_data, ": ", strlen(": "));
	
#if 0    /* formerly excluded lines */
	for (i = 0; i < strlen(pMessageData); i++)
	{   
		sprintf (Log, "%02x ", pMessageData[i]);
		strncat (me->_pCache->_p_data, Log, strlen(Log));
	}
#endif   /* formerly excluded lines */
	
	strncat (me->_pCache->_p_data, "\n", strlen("\n"));
	
	me->_pCache->_used_size = strlen(me->_pCache->_p_data) + 1;
	
Error:
	dbusUnlock(me);
	EXCRETHROW(pexception);
} /* AppendCacheData */


#undef __FUNC__
#define __FUNC__ "dbuslistAddChannel"
static SElExceptionPtr dbuslistAddChannel(
	struct _SDBUSList*	me,
	const char*			UDA_name
)
{
	SElExceptionPtr	pexception = NULL;
	SDBUSPtr			pdbus = NULL;

	if (me->_udaList->Get(me->_udaList, UDA_name) != NULL)
	{
		char	message[256] = "";

		sprintf(message, "Channel %s Allready Exists", UDA_name);
		EXCTHROW( DBUS_ERROR_CHANNEL_ALLREADY_EXISTS, message);
	}

	EXCCHECK( dbus_new(&pdbus));
	EXCCHECK( me->_udaList->Add(me->_udaList, UDA_name, pdbus));
	pdbus->_pCache = me->_pCache;
	sprintf (pdbus->_UDA_name, "%s",UDA_name); 
	
Error:
	EXCRETHROW(pexception);
}

#undef __FUNC__
#define __FUNC__ "dbuslistRemoveChannel"
static SElExceptionPtr dbuslistRemoveChannel(
	struct _SDBUSList*	me,
	const char*			UDA_name
)
{
	SElExceptionPtr	pexception = NULL;
	SDBUSPtr			pdbus = NULL;

	if (UDA_name == NULL || strlen(UDA_name) == 0)
	{
		EXCCHECK( me->_udaList->Clear(me->_udaList));
	}
	else
	{
		if ((pdbus = me->_udaList->Get(me->_udaList, UDA_name)) == NULL)
		{
			char	message[256] = "";

			sprintf(message, "Channel %s Not Exists", UDA_name);
			EXCTHROW( DBUS_ERROR_CHANNEL_NOT_EXISTS, message);
		}

		EXCCHECK( dbus_delete(&pdbus));
	}

Error:
	EXCRETHROW(pexception);
}

#undef __FUNC__
#define __FUNC__ "dbuslistOpen"
static SElExceptionPtr dbuslistOpen(
	struct _SDBUSList*	me,
	const char*			UDA_name,
    const char*			portCommPartner,
	const char*			PortFlags
)
{
	SElExceptionPtr	pexception = NULL;
	SDBUSPtr			pdbus = NULL;
	char			message[256] = "";

	if ((pdbus = me->_udaList->Get(me->_udaList, UDA_name)) == NULL)
	{
		sprintf(message, "Channel %s Not Exists", UDA_name);
		EXCTHROW( DBUS_ERROR_CHANNEL_NOT_EXISTS, message);
	}

	if (pdbus->_PortHandle != 0)
	{
		sprintf(message, "Channel %s Allready Open", UDA_name);
		EXCTHROW( DBUS_ERROR_CHANNEL_ALLREADY_OPENED, message);
	}

	EXCCHECK( pdbus->Open(pdbus, UDA_name, portCommPartner, PortFlags));

Error:
	EXCRETHROW(pexception);
}

#undef __FUNC__
#define __FUNC__ "dbuslistClose"
static SElExceptionPtr dbuslistClose(
	struct _SDBUSList*	me,
	const char*			UDA_name
)
{
	SElExceptionPtr	pexception = NULL;
	SDBUSPtr			pdbus = NULL;
	int32_t			UDA_name_count = 0,
					i = 0;
	const char*		p2name = NULL;
	char			message[256] = "";

	if (UDA_name == NULL || strlen(UDA_name) == 0)
	{
		UDA_name_count = me->_udaList->GetCount(me->_udaList);

		for (i = 0; i < UDA_name_count; i++)
		{
			p2name = me->_udaList->GetNameOnIndex(me->_udaList, i);
			pdbus = me->_udaList->Get(me->_udaList, p2name);

			EXCCHECK( pdbus->Close(pdbus));
		}
	}
	else
	{
		if ((pdbus = me->_udaList->Get(me->_udaList, UDA_name)) == NULL)
		{
			sprintf(message, "Channel %s Not Exists", UDA_name);
			EXCTHROW( DBUS_ERROR_CHANNEL_NOT_EXISTS, message);
		}

		if (pdbus->_PortHandle == 0)
		{
			sprintf(message, "Channel %s Not Open", UDA_name);
			EXCTHROW( DBUS_ERROR_CHANNEL_NOT_OPENED, message);
		}

		EXCCHECK( pdbus->Close(pdbus));
	}

Error:
	EXCRETHROW(pexception);
}

#undef __FUNC__
#define __FUNC__ "dbuslistWriteLowLevel"
static SElExceptionPtr dbuslistWriteLowLevel(
	struct _SDBUSList*	me,
	const char*			UDA_name,
	uint8_t	        	target_adress,
    char        		*pMessageID,
    char        		*pMessageData
)
{
	SElExceptionPtr	pexception = NULL;
	SDBUSPtr		pdbus = NULL;
	char			message[256] = "";

	if ((pdbus = me->_udaList->Get(me->_udaList, UDA_name)) == NULL)
	{
		sprintf(message, "UDA %s Not Exists", UDA_name);
		EXCTHROW( DBUS_ERROR_CHANNEL_NOT_EXISTS, message);
	}

	if (pdbus->_PortHandle == 0)
	{
		sprintf(message, "UDA %s Not Open", UDA_name);
		EXCTHROW( DBUS_ERROR_CHANNEL_NOT_OPENED, message);
	}
	EXCCHECK( pdbus->WriteLowLevel(pdbus, target_adress, pMessageID, pMessageData));

Error:
	EXCRETHROW(pexception);
}

#undef __FUNC__
#define __FUNC__ "dbuslistReadLowLevel"
static SElExceptionPtr dbuslistReadLowLevel(
	struct _SDBUSList*	me,
	const char*			UDA_name,
	uint8_t	        	target_adress,
	char        		*pMessageID,
    char        		*pMessageData
)
{
	SElExceptionPtr	pexception = NULL;
	SDBUSPtr		pdbus = NULL;
	char			message[256] = "";
	int 			cnt = 0;
					
	if ((pdbus = me->_udaList->Get(me->_udaList, UDA_name)) == NULL)
	{
		sprintf(message, "UDA %s Not Exists", UDA_name);
		EXCTHROW( DBUS_ERROR_CHANNEL_NOT_EXISTS, message);
	}

	if (pdbus->_PortHandle == 0)
	{
		sprintf(message, "UDA %s Not Open", UDA_name);
		EXCTHROW( DBUS_ERROR_CHANNEL_NOT_OPENED, message);
	}
	
	//cnt = 0;
	//do{
		pexception = pdbus->ReadLowLevel(pdbus, target_adress, pMessageID, pMessageData);
	//	cnt++;
	//}while (pexception && cnt < 5);
	
Error:
	EXCRETHROW(pexception);
}


#undef __FUNC__
#define __FUNC__ "dbuslistClearCache"
static SElExceptionPtr dbuslistClearCache(
	struct _SDBUSList* 	me, 
	const char*			UDA_name
)
{
	SElExceptionPtr	pexception = NULL;
	SDBUSPtr			pdbus = NULL;
	char			message[256] = "";

	if ((pdbus = me->_udaList->Get(me->_udaList, UDA_name)) == NULL)
	{
		sprintf(message, "Channel %s Not Exists", UDA_name);
		EXCTHROW( DBUS_ERROR_CHANNEL_NOT_EXISTS, message);
	}

	if (pdbus->_PortHandle == 0)
	{
		sprintf(message, "Channel %s Not Open", UDA_name);
		EXCTHROW( DBUS_ERROR_CHANNEL_NOT_OPENED, message);
	}
	
	return pdbus->ClearCache (pdbus);

Error:
	EXCRETHROW(pexception);
}

#undef __FUNC__
#define __FUNC__ "dbuslistGetCache"
static const char* dbuslistGetCache(		   
	struct _SDBUSList* 	me, 
	const char*			UDA_name
)
{
	SDBUSPtr			pdbus = NULL;
	char			message[256] = "";
	
	if ((pdbus = me->_udaList->Get(me->_udaList, UDA_name)) == NULL)
	{
		sprintf(message, "Channel %s Not Exists", UDA_name);
	}

	if (pdbus->_PortHandle == 0)
	{
		sprintf(message, "Channel %s Not Open", UDA_name);
	}
	
	return pdbus->GetCache (pdbus);
}
