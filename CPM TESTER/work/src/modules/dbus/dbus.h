/****************************************************************************
 *                              DBUS
 *---------------------------------------------------------------------------
 *    Copyright  © 2020 ARMA, a.s.  All Rights Reserved.        
 *---------------------------------------------------------------------------
 *                                                                          
 * Title:       dbus.h                                                    
 * Purpose:     DBUS interface ARMA
 ****************************************************************************/

#if !defined(__DBUS_H__)
#define __DBUS_H__

#include <eltypes.h>
#include <windows.h>
#include <elstring.h>
#include <timer/eltimer.h>
#include <exception/exception.h>
#include <objlist/objlist.h>

#if defined(__cplusplus) || defined(__cplusplus__)
	extern "C" {
#endif
		
		
typedef enum {
  VDBUS_COM1 = 0x01,
  VDBUS_COM2 = 0x02,
  VDBUS_COM3 = 0x03,
  VDBUS_COM4 = 0x04,
  VDBUS_COM5 = 0x05,
  VDBUS_COM6 = 0x06,
  VDBUS_COM7 = 0x07,
  VDBUS_COM8 = 0x08,
  VDBUS_COM9  = 0x09,
  VDBUS_COM10 = 0x0a,
  VDBUS_COM11 = 0x0b,
  VDBUS_COM12 = 0x0c,
  VDBUS_COM13 = 0x0d,
  VDBUS_COM14 = 0x0e,
  VDBUS_COM15 = 0x0f,
  VDBUS_COM16 = 0x10,

  // supports max. 16 USB DBus devices (with this enum; total max supported is 128 USB DBus devices)
  VDBUS_USB1 = 0x101,
  VDBUS_USB2 = 0x102,
  VDBUS_USB3 = 0x103,
  VDBUS_USB4 = 0x104,
  VDBUS_USB5 = 0x105,
  VDBUS_USB6 = 0x106,
  VDBUS_USB7 = 0x107,
  VDBUS_USB8 = 0x108,
  VDBUS_USB9 = 0x109,
  VDBUS_USB10 = 0x10a,
  VDBUS_USB11 = 0x10b,
  VDBUS_USB12 = 0x10c,
  VDBUS_USB13 = 0x10d,
  VDBUS_USB14 = 0x10e,
  VDBUS_USB15 = 0x10f,
  VDBUS_USB16 = 0x110
} VDBusPortId;

/*--------------------------------------------------------------------*/
//! Port information structure
/*! 
*/
struct VDBusPortInfo
{
  unsigned long       portInfoSize; //!< Size of this structure
  CHAR        portText[64]; //!< Short name
  BOOL        portLocked;   //!< TRUE, if port is locked by another application
  VDBusPortId portId;       //!< Port Identifier
};

/*--------------------------------------------------------------------*/
//! Error codes
/*! 
*/
typedef enum {
  VDBUS_SUCCESS							= 0,      //!< The operation was completed successfully
  VDBUS_NO_FRAME_AVAILABLE				= 1,      //!< VIDBusPort::GetFrame has nothing received
  VDBUS_ERROR							= 0x100,  //!< An error occured
  
  // API erros
  VDBUS_INVALIDARG						= 0x1000, //!< An argument is not valid
  VDBUS_TX_BUFFER_FULL					= 0x1001,
  VDBUS_INVALID_HANDLE					= 0x1002, //!< The port handle is not valid
  VDBUS_TIMEOUT							= 0x1003,

  VDBUS_PORT_NOT_OPENED					= 0x1800, // the port is not opened for cummunication
  VDBUS_INVALID_MODE					= 0x1801, // the current mode (raw or dbus) is not valid for this function
  VDBUS_FW_VERSION_MISMATCH				= 0x1802, // the current FW version does not support the called function
  VDBUS_FW_VERSION_MISMATCH_RAW_MODE_SINGLE_BYTE = 0x1803, // the current FW version does not support the single byte feature of RAW mode

  VDBUS_INTERNAL_ERROR					= 0x1900,
  VDBUS_INTERNAL_ERROR_SEND_BUFFER_OVERFLOW = 0x1901,
  VDBUS_INTERNAL_ERROR_USB_WRITE		= 0x1902,
  VDBUS_INTERNAL_ERROR_ANSWER_TIMEOUT	= 0x1903,
  VDBUS_INTERNAL_ERROR_INVALID_ANSWER	= 0x1904,
  VDBUS_INTERNAL_ERROR_INVALID_ANSWER_CODE = 0x1905,
  VDBUS_INTERNAL_ERROR_INVALID_ANSWER_SIZE = 0x1906,
  VDBUS_INTERNAL_ERROR_SIZE_MISMATCH	= 0x1907,

  // hardware errors
  VDBUS_PORT_NOT_FOUND       = 0x2000, //!< Serial port not found
  VDBUS_PORT_ALREADY_LOCKED  = 0x2001, //!< Serial port is already locked by another application
  VDBUS_PORT_SET_COMM_FAILED = 0x2002, //!< Cannot set communication parameter of COM port

  VDBUS_POWER_MODULE_NOT_INSTALLED = 0x2400,

  VDBUS_ERROR_REPORTED_BY_FW = 0x2800,
  VDBUS_ERROR_WATCHDOG_RESET = 0x2801,
  VDBUS_ERROR_HARDWARE_RESET = 0x2802,
  VDBUS_ERROR_SOFTWARE_RESET = 0x2803,

  VDBUS_INVALID_FW			 = 0x3000		// the specified FW is invalid
} VResult;

typedef enum {
  VDBUS_FLAG_NO_COLLISION               = 0x0001, //!< Do not call onReceiveCallback on collisions
  VDBUS_FLAG_CLEAR_RX_QUEUE_BEFORE_SEND = 0x0002, //!< Clear the RX queue before a frame is sent
  VDBUS_FLAG_CRC8                       = 0x0004, //!< use 8Bit CRC checksum (DBUS1), default is 16Bit CRC (DBus2)
  VDBUS_FLAG_2400_BAUD                  = 0x0008, //!< use 2400 Baud
  VDBUS_FLAG_4800_BAUD                  = 0x0010, //!< use 4800 Baud
  VDBUS_FLAG_9600_BAUD                  = 0x0020, //!< use 9600 Baud (default)
  VDBUS_FLAG_19200_BAUD                 = 0x0040, //!< use 19200 Baud
  VDBUS_FLAG_38400_BAUD                 = 0x0080, //!< use 38200 Baud
  VDBUS_FLAG_RETRY_AFTER_MISSING_ACK    = 0x0100, //!< retry if a sent frame is not acknowledged
  VDBUS_FLAG_RTS_LOW                    = 0x0200, //!< set RTS line to low, RTS and DTR lines are the power supply of the adapter
  VDBUS_FLAG_DTR_HIGH                   = 0x0400, //!< set DTR line to high, RTS and DTR lines are the power supply of the adapter
  VDBUS_FLAG_RAW_MODE_NO_TS				= 0x0800, //!< do not send byte specific timestamp info upto the PC -> use this to achieve higher baudrates in raw mode!
  VDBUS_FLAG_RAW_MODE_SINGLE_BYTES		= 0x1000
} VDBusPortFlags;

/*--------------------------------------------------------------------*/
//! Frame information structure
/*!
  
*/
struct VDBusFrameInfo
{
  unsigned long frameInfoSize;    //!< Size of this structure. This field must be initialized before pass
                          //!< it to the driver functions.
  unsigned long flags;            //!< shows which fields of this structure are valid, see VDBusFrameInfoFlags
  unsigned long targetAddress;    //!< target address of the frame
  unsigned long ackType;          //!< Acknowlege type
  unsigned long ackCommPartner;   //!< Communication partner, who acknowledged the frame
  unsigned long messageSize;      //!< Number of databytes of the received message
  unsigned long checksum;         //!< Checksum of the frame
};

/*--------------------------------------------------------------------*/
//! Acknowledge types
/*! 
*/
typedef enum {
  VDBUS_ACK_OK              = 0x0a, //!< Frame successful received (CRC check ok)
  VDBUS_ACK_BUSY            = 0x03, //!< Target is busy
  VDBUS_ACK_WRONG           = 0x07, //!< CRC error
  VDBUS_ERROR_TIMEOUT       = 0x10, //!< Timeout while waiting for ACK
  VDBUS_ERROR_TX_COLLISION  = 0x11, //!< Aborted transmission, too many collisions
  VDBUS_ERROR_ACK_COLLISION = 0x12, //!< Collision, while sending ACK
  VDBUS_ERROR_RX_COLLISION  = 0x13  //!< Received a corrupt frame, it must be a collision
} VDBusAckType;

typedef unsigned long VDBusPortHandle;
SElExceptionPtr LoadLibrary_DBUS(const char* libary_name);
				
/*!
 * Specifies DBUS interface
 */ 
typedef struct _SDBUS
{
	SElExceptionPtr (*Open)(
			struct _SDBUS*	me,
			const char*		UDA_name,
		   const char*		portCommPartner,
			const char*		PortFlags
		);  /*!< \see dbusOpen */
    SElExceptionPtr (*Close)(struct _SDBUS*	me); /*!< \see dbusClose */
	
	SElExceptionPtr (*Write)(
			struct _SDBUS*	pDBUS,
			uint32_t		address,
			uint32_t		dataLength,
			uint8_t*		pData
		); /*!< \see dbusWrite */
	SElExceptionPtr (*Read)(
			struct _SDBUS*	pDBUS,
			uint32_t*		identifier,
			int32_t*		pDataLength,
			uint8_t*		pData
		); /*!< \see dbusRead */

	SElExceptionPtr (*WriteLowLevel)(
			struct _SDBUS*	pDBUS,
			uint8_t	        target_adress,
    		char        	*pMessageID,
    		char       		*pMessageData
		); /*!< \see dbusWrite */
	SElExceptionPtr (*ReadLowLevel)(
			struct _SDBUS*	pDBUS,
			uint8_t	        target_adress,
    		char        	*pMessageID,
    		char        	*pMessageData
		); /*!< \see dbusRead */
	
	
    SElExceptionPtr (*ClearCache)(struct _SDBUS* me);
    const char* (*GetCache)(struct _SDBUS* me);


/*******************************************
 * PRIVATE
 *******************************************/ 
	VDBusPortHandle 	_PortHandle;
	SELStringPtr		_pCache;		/*!< Cache data, all write and read data */
	STimerPtr			_pTimer;		/*!< Timer used in cache logs */
	bool_t				_CacheEnabled;	/*!< Is cache enabled */
	char				_prefix[32];	/*!< Channel prefix */
	char				_UDA_name[32];	/*!< Channel prefix */	
	
	uint32_t			_timeout;		/*!< DBUS timeout */
	
	CRITICAL_SECTION	_Lock; /* AppendCacheData() */
	CRITICAL_SECTION	_LockId; /* write data */
	SObjListPtr			_LockIds;

} SDBUS, *SDBUSPtr;

/*! Creates a new instance of DBUS interface */
SElExceptionPtr	dbus_new(SDBUSPtr* pDBUS);
/*! Destroys an instance of DBUS interface */
SElExceptionPtr dbus_delete(SDBUSPtr* pDBUS);

/**
 *  Specified list of Communication objects
 */
typedef struct _SDBUSList
{
	SElExceptionPtr (*AddChannel)(
			struct _SDBUSList*	me,
			const char*			UDA_name
		);  /*!< \see dbusOpen */
	SElExceptionPtr (*RemoveChannel)(
			struct _SDBUSList*	me,
			const char*			UDA_name
		);  /*!< \see dbusOpen */

	SElExceptionPtr (*Open)(
			struct _SDBUSList*	me,
			const char*			UDA_name,
		    const char*		portCommPartner,
			const char*		PortFlags
		);  /*!< \see dbusOpen */
    SElExceptionPtr (*Close)(
			struct _SDBUSList*	me,
			const char*			UDA_name
		); /*!< \see dbusClose */

	SElExceptionPtr (*WriteLowLevel)(
			struct _SDBUSList*	pDBUS,
			const char*			UDA_name,
			uint8_t	        	target_adress,
    		char        		*pMessageID,
    		char        		*pMessageData
		); /*!< \see dbusWriteLowLevel */
		
	SElExceptionPtr (*ReadLowLevel)(
			struct _SDBUSList*	pDBUS,
			const char*			UDA_name,
			uint8_t	        	target_adress,
    		char        		*pMessageID,
    		char        		*pMessageData
		); /*!< \see canReadLowLevel */

    SElExceptionPtr (*ClearCache)(struct _SDBUSList* me, const char* UDA_name);
    const char* (*GetCache)(struct _SDBUSList* me, const char* UDA_name);


/*******************************************
 * PRIVATE
 *******************************************/
	SObjListPtr			_udaList;	/*!< List of opened UDA_names */
	
	SELStringPtr		_pCache;		/*!< Cache data, all write and read data */
	STimerPtr			_pTimer;		/*!< Timer used in cache logs */
	bool_t				_CacheEnabled;	/*!< Is cache enabled */
	
} SDBUSList, *SDBUSListPtr;

/*! Creates a new instance of DBUS interface */
SElExceptionPtr	dbuslist_new(SDBUSListPtr* pDBUS);
/*! Destroys an instance of DBUS interface */
SElExceptionPtr dbuslist_delete(SDBUSListPtr* pDBUS);


#define DBUS_ERROR_CHANNEL_NOT_OPENED		-1000000
#define DBUS_ERROR_CHANNEL_NOT_EXISTS		-1000001
#define DBUS_ERROR_CHANNEL_ALLREADY_EXISTS	-1000002
#define DBUS_ERROR_CHANNEL_ALLREADY_OPENED	-1000002



#define DBUS_PORT_NOT_AVAILABLE 			-1000100
#define DBUS_PORT_NOT_FOUND 				-1000101
#define DBUS_PORT_LOCKED		 			-1000102
#define DBUS_ERROR_OPEN_PORT	 			-1000103


#define DBUS_NO_FRAME_INFOS		 			-1000200
#define DBUS_ERROR_READ_FRAME_DATA 			-1000201
#define DBUS_ERROR_READ_TARGET_ADRESS		-1000202

#define DBUS_ERROR_SEND_FRAME_DATA			-1000301







#if defined(__cplusplus) || defined(__cplusplus__)
	}
#endif

#endif /* __DBUS_H__*/ 
