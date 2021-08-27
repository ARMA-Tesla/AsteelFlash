#if !defined(__SERIAL_CONFIGURATION_H__)
#define __SERIAL_CONFIGURATION_H__

#define FLOW_CTRL_NONE			0
#define FLOW_CTRL_XON_XOFF		1
#define FLOW_CTRL_RTS_CTS		2
#define FLOW_CTRL_DTR_DSR		3

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

#endif /* __SERIAL_CONFIGURATION_H__*/