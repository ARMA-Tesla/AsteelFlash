/*
 *  "elIoModbus.c" - Modbus (RTU/ASCII/TCP/UDP/...) I/O routines.
 *
 *	(c) 2007 ELCOM, a.s.
 *		All Rights Reserved.
 *
 *	The information  in this file  is provided for the exclusive use of
 *	the  licensees  of  ELCOM, a.s.  Such users have the right  to use,
 *	modify,  and incorporate this code into other products for purposes
 *	authorized  by the license  agreement  provided they  include  this
 *	notice  and the associated copyright notice  with any such product.
 *	The information in this file  is provided "AS IS" without warranty.
 *
 *
 * Revision History:
 * -----------------
 *	2007-03-05	Radomir Tomis (Radomir.Tomis@dvi.elcom.cz)
 *		Created.
 */

#if defined(_MSC_VER)
#pragma warning(disable : 4505)	/* 'function' : unreferenced local function has been removed */
#endif

#if 1
static char _ident[] = "@(#) elIoModbus.c 0.1 2007-03-05";
#else
#endif

#include <elBase.h>
#include <elObj.h>
#include <elErr.h>
#include <elErrWsa.h>
#include <elErrWin.h>
#include <elMem.h>
#include <elMtx.h>
#include <elIo.h>
#include <elSocket.h>
#include <elTimer.h>

static elErr_t *elIoModbusCtor(elObjPtr_t *po,va_list args);
static elErr_t *elIoModbusDtor(elObjPtr_t *po);

static elObjClass_t gs_elIoModbus_c = {
	sizeof(elIoModbus_t),
	"elIoModbus",
	elIoModbusCtor,		/* Ctor()	*/
	elIoModbusDtor,		/* Dtor()	*/
	NULL,				/* Clone()	*/
	NULL,				/* Differ()	*/
};

const elObjClass_t *elIoModbus_c		= &gs_elIoModbus_c;

#define EL_IO_MODBUS_METHODS								\
		elIoModbusOpen,				/* Open()			*/	\
		elIoModbusClose,			/* Close()			*/	\
		elIoModbusRead,				/* Read()			*/	\
		elIoModbusWrite,			/* Write()			*/	\
		NULL,						/* MsgRead()		*/	\
		NULL,						/* MsgWrite()		*/	\
		NULL,						/* Flush()			*/	\
		NULL,						/* Abort()			*/	\
		NULL,						/* AbortReset()		*/	\
		elIoModbusCtrl,				/* Ctrl()			*/	\
		NULL,						/* AttrSet()		*/	\
		NULL,						/* AttrGet()		*/	\
		NULL,						/* Lock()			*/	\
		NULL,						/* Unlock()			*/	\
		elIoModbusDevLock,			/* DevLock()		*/	\
		elIoModbusDevUnlock,		/* DevUnlock()		*/	\
		NULL,						/* DebugLog()		*/	\
		elIoModbusTaskRead,			/* TaskRead()		*/	\
		elIoModbusTaskWrite,		/* TaskWrite()		*/	\
		elIoModbusTaskExecute,		/* TaskExecute()	*/	\
		elIoModbusTaskDelete		/* TaskDelete()		*/	\

const elIoIntfc_t g_elIoModbusIntfc = {
	_T("Modbus"),	EL_IO_MODBUS_METHODS
};
const elIoIntfc_t *elIoModbusIntfc = &g_elIoModbusIntfc;

static struct {
	TCHAR *str;
	byte id;
} gs_modbus_proto[] = {
#define P(id)		_T(#id),	EL_IO_MODBUS_PROTO_##id
	P(RTU),
	P(ASCII),
	P(TCP),
	P(UDP),
#undef P
};

static struct {
	TCHAR *str;
	byte id;
} gs_modbus_table[] = {
	_T("DI"),	EL_IO_MODBUS_TABLE_INPUT_COIL,
	_T("DO"),	EL_IO_MODBUS_TABLE_OUTPUT_COIL,
	_T("AI"),	EL_IO_MODBUS_TABLE_INPUT_REGISTER,
	_T("AO"),	EL_IO_MODBUS_TABLE_OUTPUT_REGISTER,
};

static struct {
	bool				mbt_init;
	elMtx_t				mtx;

	/* table of all unique devices used in all I/O handles: */
	elIoModbusDev_t		**dev;
	int					dev_cnt;	/* # of valid devices in 'dev[]' */
	int					dev_acnt;	/* # of allocated elements in 'dev[]' */

} gs;

/*
 * -- { -- data/code from free MODBUS library 'libmodbus': --
 * (http://copyleft.free.fr/wordpress/index.php/libmodbus)
 */
#if !defined(USHRT_MAX)
#define USHRT_MAX		65535
#endif

#define HEADER_LENGTH_RTU      0
#define PRESET_QUERY_SIZE_RTU  6

#define HEADER_LENGTH_TCP      6
#define PRESET_QUERY_SIZE_TCP 12

#define CHECKSUM_SIZE_RTU      2
#define CHECKSUM_SIZE_TCP      0	

/* 8 + HEADER_LENGTH_TCP */
#define MIN_QUERY_SIZE        14

/* MIN_RESPONSE_LENGTH + MAX(MAX*) */
#define MAX_PACKET_SIZE      261

#define MAX_READ_STATUS      800
#define MAX_READ_HOLD_REGS   100
#define MAX_READ_INPUT_REGS  100
#define MAX_WRITE_COILS      800
#define MAX_WRITE_REGS       100

#define REPORT_SLAVE_ID_SIZE 75

/* Time out between trames in microsecond */
#define TIME_OUT_BEGIN_OF_TRAME 500000
#define TIME_OUT_END_OF_TRAME   500000

#define SLAVE 1

/* Protocol exceptions */
#define ILLEGAL_FUNCTION        -0x01
#define ILLEGAL_DATA_ADDRESS    -0x02
#define ILLEGAL_DATA_VALUE      -0x03
#define SLAVE_DEVICE_FAILURE    -0x04
#define SERVER_FAILURE          -0x04
#define ACKNOWLEDGE             -0x05
#define SLAVE_DEVICE_BUSY       -0x06
#define SERVER_BUSY             -0x06
#define NEGATIVE_ACKNOWLEDGE    -0x07
#define MEMORY_PARITY_ERROR     -0x08
#define GATEWAY_PROBLEM_PATH    -0x0A
#define GATEWAY_PROBLEM_TARGET  -0x0B

/* Local */
#define COMM_TIME_OUT           -0x0C
#define PORT_SOCKET_FAILURE     -0x0D
#define SELECT_FAILURE          -0x0E
#define TOO_MANY_DATAS          -0x0F
#define INVALID_CRC             -0x10
#define INVALID_EXCEPTION_CODE  -0x11

#define WAIT_DATA()																\
{																				\
    while ((select_ret = select(0, &rfds, NULL, NULL, &tv)) == -1) {			\
		;																		\
	}																			\
}

#define WAIT_DATA_orig()														\
{																				\
    while ((select_ret = select(0, &rfds, NULL, NULL, &tv)) == -1) {			\
	    if (errno == EINTR) {													\
		    printf("A non blocked signal was caught\n");						\
		    /* Necessary after an error */										\
		    FD_ZERO(&rfds);														\
			FD_SET(dev->u.tcp.s->h, &rfds);										\
		} else {																\
			libmb_error_treat(io,select_ret, "Select failure");					\
			return SELECT_FAILURE;												\
		}																		\
	}																			\
																				\
        if (select_ret == 0) {													\
                /* Call to error_treat is done later to manage exceptions */	\
		return COMM_TIME_OUT;													\
	}																			\
}

#define UNKNOWN_ERROR_MSG "Not defined in modbus specification"

static const int SIZE_TAB_ERROR_MSG = 12;
static const char *TAB_ERROR_MSG[] = {
	/* 0x00 */ UNKNOWN_ERROR_MSG,
	/* 0x01 */ "Illegal function code",
	/* 0x02 */ "Illegal data address",
	/* 0x03 */ "Illegal data value",
	/* 0x04 */ "Slave device or server failure",
	/* 0x05 */ "Acknowledge",
	/* 0x06 */ "Slave device or server busy",
	/* 0x07 */ "Negative acknowledge",
	/* 0x08 */ "Memory parity error",
	/* 0x09 */ UNKNOWN_ERROR_MSG,
	/* 0x0A */ "Gateway path unavailable",
	/* 0x0B */ "Target device failed to respond"
};

/* Table of CRC values for high-order byte */
static unsigned char table_crc_hi[] = {
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

/* Table of CRC values for low-order byte */
static unsigned char table_crc_lo[] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 
	0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 
	0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 
	0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
	0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 
	0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 
	0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
	0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 
	0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
	0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 
	0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 
	0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
	0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 
	0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
	0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 
	0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 
	0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
	0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 
	0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
	0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 
	0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 
	0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

#if 0 && defined(_MSC_VER)
/* "warning C4505: 'libmb_force_multiple_coils' : unreferenced local function has been removed": */
#pragma warning(disable : 4505)
#endif

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"libmb_build_request_packet_rtu"
/* The following functions construct the required query into
   a modbus query packet */
static elErr_t *libmb_build_request_packet_rtu(
	elIo_t *io,
	int slave, int function, int start_addr,
	int count, unsigned char *packet,
	int *p_len
)
{
	EL_ERR_VAR;

	(void) io;

	packet[0] = (byte) slave;
	packet[1] = (byte) function;
	packet[2] = (byte) (start_addr >> 8);
	packet[3] = (byte) (start_addr & 0x00ff);
	packet[4] = (byte) (count >> 8);
	packet[5] = (byte) (count & 0x00ff);

	*p_len = PRESET_QUERY_SIZE_RTU;

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"libmb_build_request_packet_tcp"
static elErr_t *libmb_build_request_packet_tcp(
	elIo_t *io,
	int slave, int function, int start_addr,
	int count, unsigned char *packet,
	int *p_len
)
{
	EL_ERR_VAR;
	elIoModbus_t		*dio = io->dio;

	(void) slave;

#if 1
	packet[0] = (byte) (dio->tid >> 8);
	packet[1] = (byte) (dio->tid & 0x00ff);
	/* Transaction ID */
	if (dio->tid < USHRT_MAX)
		dio->tid++;
	else
		dio->tid = 0;
#else
	/* Transaction ID */
	if (dio->tid < USHRT_MAX)
		dio->tid++;
	else
		dio->tid = 0;
	packet[0] = dio->tid >> 8;
	packet[1] = dio->tid & 0x00ff;
#endif

	/* Protocol Modbus */
	packet[2] = 0;
	packet[3] = 0;

	/* Length to fix later with set_packet_length_tcp (4 and 5) */

#if 1
	packet[6] = 0x01;		/* unit no. */
#else
	packet[6] = 0xFF;
#endif
	packet[7] = (byte) function;
	packet[8] = (byte) (start_addr >> 8);
	packet[9] = (byte) (start_addr & 0x00ff);
	packet[10] = (byte) (count >> 8);
	packet[11] = (byte) (count & 0x00ff);

	*p_len = PRESET_QUERY_SIZE_TCP;

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"libmb_error_treat"
static void libmb_error_treat(elIo_t *io,int ret, const char *message)
{
	elIoModbus_t		*dio = io->dio;

	if (ret == -1)
		perror(message);
	printf("\n\nERROR %s\n\n", message);

	if ( dio->dev->proto == EL_IO_MODBUS_PROTO_RTU )
	{
#if !WIN
		tcflush(mb_param->fd, TCIOFLUSH);
#endif
	} else {
#if 0
		modbus_close(mb_param);
		modbus_connect(mb_param);
#endif
	}
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"libmb_compute_response_size"
static unsigned int libmb_compute_response_size(elIo_t *io,unsigned char *query)
{
	elIoModbus_t	*dio = io->dio;
	int				response_size_computed;
	int				offset;

	offset = dio->hdr_len;

	switch (query[offset + 1]) {
	case 0x01: case 0x02: {
		/* Header + nb values (code from force_multiple_coils) */
		int coil_count = (query[offset + 4] << 8) | query[offset + 5];
		response_size_computed = 3 +
			(coil_count / 8) + ((coil_count % 8) ? 1 : 0);
	}	break;
	case 0x03:
		/* Header + 2 * nb values */
		response_size_computed = 3 + 
			2 * (query[offset + 4] << 8 | query[offset + 5]);
		break;
	case 0x07:
		response_size_computed = 4;
		break;
	default:
		response_size_computed = 6;
	}

	response_size_computed += offset + dio->xsum_size;

	return response_size_computed;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"libmb_set_packet_length_tcp"
static void libmb_set_packet_length_tcp(
	unsigned char *packet, size_t packet_size
)
{
	unsigned short mbap_length;

	/* Substract MBAP header length */
	mbap_length = (ushort) (packet_size - 6);

	packet[4] = (byte) (mbap_length >> 8);
	packet[5] = (byte) (mbap_length & 0x00FF);
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"libmb_build_request_packet"
static elErr_t *libmb_build_request_packet(
	elIo_t *io,
	int slave, int function, int start_addr, int count, unsigned char *packet,
	int *p_len
)
{
	EL_ERR_VAR;
	elIoModbus_t		*dio = io->dio;

	if ( dio->dev->proto == EL_IO_MODBUS_PROTO_RTU )
	{
		EL_ERR_CHECK(
			libmb_build_request_packet_rtu(
				io, slave, function, start_addr, count, packet, p_len
			)
		);
	}
	else
	{
		EL_ERR_CHECK(
			libmb_build_request_packet_tcp(
				io, slave, function, start_addr,count, packet, p_len
			)	
		);
	}

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"libmb_crc16"
/* Fast CRC */
static unsigned short libmb_crc16(
	unsigned char *buffer,unsigned short buffer_length
)
{
	unsigned char crc_hi = 0xFF; /* high CRC byte initialized */
	unsigned char crc_lo = 0xFF; /* low CRC byte initialized */
	unsigned int i; /* will index into CRC lookup */

	/* pass through message buffer */
	while (buffer_length--) {
		i = crc_hi ^ *buffer++; /* calculate the CRC  */
		crc_hi = (byte) (crc_lo ^ table_crc_hi[i]);
		crc_lo = table_crc_lo[i];
	}

	return (ushort) (crc_hi << 8 | crc_lo);
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"libmb_modbus_query"
/* Function to send a query out to a modbus slave */
static elErr_t *libmb_modbus_query(
	elIo_t *io,unsigned char *query,size_t query_size,int *p_len
)
{
	EL_ERR_VAR;
	elIoModbus_t		*dio = io->dio;
	elIoModbusDev_t		*dev = dio->dev;
	int					write_ret;
	unsigned short		s_crc;
	int					i;
	
	if ( dio->dev->proto == EL_IO_MODBUS_PROTO_RTU )
	{
		s_crc = libmb_crc16(query, (ushort)query_size);
		query[query_size++] = (byte) (s_crc >> 8);
		query[query_size++] = (byte) (s_crc & 0x00FF);
	}
	else
	{
		libmb_set_packet_length_tcp(query, query_size);
	}

	if ( io->debug ) {
		printf("\n");
		for (i = 0; (size_t)i < query_size; i++)
			printf("[%.2X]", query[i]);

		printf("\n");
	}
	
	if ( dio->dev->proto == EL_IO_MODBUS_PROTO_RTU )
	{
#if 0
		write_ret = write(mb_param->fd, query, query_size);
#else
		write_ret = 0;
#endif
	}
	else
	{
		EL_ERR_CHECK(
			elSocketSend(dev->u.tcp.s,query,(int)query_size,0,&write_ret)
		);
	}

	/* Return the number of bytes written (0 to n)
	   or PORT_SOCKET_FAILURE on error */
	if ((write_ret == -1) || ((size_t)write_ret != query_size)) {
		libmb_error_treat(io,write_ret, "Write port/socket failure");
	}
	
	*p_len = write_ret;

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"libmb_receive_response"
/* Function to monitor for the reply from the modbus slave.
   This function blocks for timeout seconds if there is no reply.

   Returns:
   - error_code 0 == OK, < 0 == error
   - (arg) total number of characters received.
*/
static elErr_t *libmb_receive_response(
	elIo_t *io,
	int response_size_computed,
	unsigned char *response,
	int *response_size,
	int *p_error_code
)
{
	EL_ERR_VAR;
	elIoModbus_t		*dio = io->dio;
	elIoModbusDev_t		*dev = dio->dev;
	int					select_ret;
	int					read_ret;
	fd_set				rfds;
	struct timeval		tv;
	int					size_to_read;
	unsigned char		*p_response;

	if (io->debug)
		printf("Waiting for response (%d)...\n", response_size_computed);

	/* Add a file descriptor to the set */
	FD_ZERO(&rfds);
#if defined(_MSC_VER)
/* "warning C4127: conditional expression is constant" */
#pragma warning(disable : 4127)
#endif
	FD_SET(dev->u.tcp.s->h, &rfds);
#if defined(_MSC_VER)
#pragma warning(default : 4127)
#endif
	
	/* Wait for a response */
	tv.tv_sec = 0;
	tv.tv_usec = TIME_OUT_BEGIN_OF_TRAME;

	WAIT_DATA();

	/* Read the trame */
	(*response_size) = 0;
	size_to_read = response_size_computed;
	p_response = response;

	while (select_ret) {
		if ( dio->dev->proto == EL_IO_MODBUS_PROTO_RTU )
		{
#if 0
			read_ret = read(mb_param->fd, p_response, size_to_read);
#else
			read_ret = 0;
#endif
		}
		else
		{
			EL_ERR_CHECK(
				elSocketRecv(dev->u.tcp.s,p_response,size_to_read,0,&read_ret)
			);
		}

		if (read_ret == -1) {
			libmb_error_treat(io,read_ret, "Read port/socket failure");
		} else {
			/* Sums bytes received */ 
			(*response_size) += read_ret;
			
			/* Display the hex code of each
			 * character received */
			if (io->debug) {
				int i;
				for (i=0; i < read_ret; i++)
					printf("<%.2X>", p_response[i]);
			}
			/* Moves the pointer to receive other datas */
			p_response = &(p_response[read_ret]);
			size_to_read = response_size_computed - (*response_size); 

			if ((*response_size) > MAX_PACKET_SIZE) {
				*p_error_code = TOO_MANY_DATAS;
				libmb_error_treat(io,0, "Too many datas");
				EL_ERR_THROW(EL_ERR_IO,NULL,NULL);
			}
		}

		if (size_to_read > 0) {
			/* If no character at the buffer wait
			   TIME_OUT_END_OF_TRAME before to generate an error.
			*/
			tv.tv_sec = 0;
			tv.tv_usec = TIME_OUT_END_OF_TRAME;
			
			WAIT_DATA();
		} else {
			/* All chars are received */
			select_ret = FALSE;
		}
	}
	
	if (io->debug)
		printf("\n");

	*p_error_code = 0;	/* OK */

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"libmb_check_crc16"
static elErr_t *libmb_check_crc16(
	elIo_t *io,unsigned char *response,int response_size,int *p_crc_status
)
{
	EL_ERR_VAR;
	elIoModbus_t		*dio = io->dio;
	int					ret;
	
	if ( dio->dev->proto == EL_IO_MODBUS_PROTO_RTU )
	{
		unsigned short crc_calc;
		unsigned short crc_received;
		unsigned char recv_crc_hi;
		unsigned char recv_crc_lo;
		
		crc_calc = libmb_crc16(response, (ushort)(response_size - 2));
		
		recv_crc_hi = (byte) response[response_size - 2];
		recv_crc_lo = (byte) response[response_size - 1];
		
		crc_received = response[response_size - 2];
		crc_received = (ushort) (crc_received << 8);
		crc_received = (ushort) (
			crc_received | 
			response[response_size - 1]
		);
		
		/* Check CRC of response */
		if (crc_calc == crc_received) {
			ret = TRUE;
		} else {
			libmb_error_treat(
				io,0, 
				elStrTmpFmt("invalid crc received %0X - crc_calc %0X", 
					crc_received, crc_calc)
			);


			ret = INVALID_CRC;
			EL_ERR_THROW(EL_ERR_IO,NULL,NULL);
		}
	} else {
		/* In TCP, CRC doesn't exist but it doesn't check
		   length because it's not really useful */ 
		ret = TRUE;
	}

	*p_crc_status = ret;

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"libmb_modbus_response"
static elErr_t *libmb_modbus_response(
	elIo_t *io, unsigned char *query,unsigned char *response,int *p_len
)
{
	EL_ERR_VAR;
	elIoModbus_t		*dio = io->dio;
	int					response_size;
	int					response_size_computed;
	int					offset = dio->hdr_len;
	int					error_code;

	response_size_computed = libmb_compute_response_size(io, query);
	EL_ERR_CHECK(
		libmb_receive_response(
			io, response_size_computed, response, &response_size, &error_code
		)
	);
	if (error_code == 0) {
		/* No error */
		int ret;

		EL_ERR_CHECK( libmb_check_crc16(io, response, response_size, &ret) );
		if ( ret != TRUE )
		{
			EL_ERR_THROW(EL_ERR_IO,NULL,NULL);
		}

		/* Good response */
		switch (response[offset + 1]) {
		case 0x01:
		case 0x02:
			/* Read functions 1 value = 1 byte */
			response_size = response[offset + 2];
			break;
		case 0x03:
		case 0x04:
			/* Read functions 1 value = 2 bytes */
			response_size = response[offset + 2] / 2;
			break;
		case 0x0F:
		case 0x10:
			/* N Write functions */
			response_size = response[offset + 4] << 8 |
				response[offset + 5];
			break;
		case 0x11:
			/* Report slave ID (bytes received) */
			break;
		default:
			/* 1 Write functions & others */
			response_size = 1;
		}

	} else if (error_code == COMM_TIME_OUT &&
		   response_size == offset + 3 + dio->xsum_size) {
		/* Optimisation allowed because exception response is
		   the smallest trame in modbus protocol (3) so always
		   raise an timeout error */
		int ret;
		
		/* CRC */
		EL_ERR_CHECK( libmb_check_crc16(io, response, response_size,&ret) );
		if (ret != TRUE)
		{
			EL_ERR_THROW(EL_ERR_IO,NULL,NULL);
		}

		/* Check for exception response
		   0x80 + function */
		if (0x80 + query[offset + 1] == response[offset + 1]) {

			if (response[offset + 2] < SIZE_TAB_ERROR_MSG) {
				libmb_error_treat(io,0,
					    TAB_ERROR_MSG[response[offset + 2]]);
				/* Modbus error code (negative) */
				response_size = -response[offset + 2];
			} else {
				/* The chances are low to hit this
				   case but can avoid a vicious
				   segfault */
				libmb_error_treat(
					io,0,
					elStrTmpFmt("Invalid exception code %d",response[offset + 2])
				);
				response_size = INVALID_EXCEPTION_CODE;
			}
		}
	} else if (error_code == COMM_TIME_OUT) {
		libmb_error_treat(io,0, "Communication time out");
		response_size = COMM_TIME_OUT;
	} else {
		response_size = error_code;
	}

	*p_len = response_size;

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"libmb_read_io_status"
/* Read IO status */
static elErr_t *libmb_read_io_status(
	elIo_t *io,
	int slave, int function,
	int start_addr, int count, byte *data_dest,
	int *p_status
)
{
	EL_ERR_VAR;
	elIoModbus_t		*dio = io->dio;
	int					query_size;
	int					query_ret;
	int					response_ret;
	unsigned char		query[MIN_QUERY_SIZE];
	unsigned char		response[MAX_PACKET_SIZE];

	EL_ERR_CHECK(
		libmb_build_request_packet(
			io, slave, function, start_addr, count, query, &query_size
		)
	);

	EL_ERR_CHECK( libmb_modbus_query(io, query, query_size, &query_ret)	);

	if (query_ret > 0) {
		EL_ERR_CHECK(
			libmb_modbus_response(io, query, response,&response_ret)
		);

#if 1
		/* copy response to user buffer intact (8 inputs per byte): */
		memcpy(data_dest,response + 3 + dio->hdr_len,response_ret);
#else
		/* convert bits from response to 'int's in user buffer: */
		{	/* inner block */
		int					offset;
		int					offset_length;
		int					i, temp, bit;
		int					pos = 0;
		int					processed = 0;

		offset = dio->hdr_len;

		offset_length = offset + response_ret;		
		for (i = offset; i < offset_length; i++) {
			/* Shift reg hi_byte to temp */
			temp = response[3 + i];
			
			for (bit = 0x01;
			     (bit & 0xff) && (processed < count);) {
				data_dest[pos++] = 
					(temp & bit) ? TRUE : FALSE;
				processed++;
				bit = bit << 1;
#if 1
				if ( !(bit & 0xFF) )
					bit = 0x01;
#endif
			}
			
		}
		}	/* inner block */
#endif
	} else {
		response_ret = query_ret;
	}

	*p_status = response_ret;

Error:

	return err;
}

#if 0
/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"libmb_read_coil_status"
/* Reads the boolean status of coils and sets the array elements
   in the destination to TRUE or FALSE */
static elErr_t *libmb_read_coil_status(
	elIo_t *io,
	int slave, int start_addr, int count, byte *data_dest,
	int *p_status
)
{
	EL_ERR_VAR;
	int function = 0x01;

	EL_ERR_CHECK(
		libmb_read_io_status(
			io, slave, function, start_addr,count, data_dest, p_status)
	);

	if (*p_status > 0)
		*p_status = count;
	
Error:

	return err;
}
#endif

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"libmb_read_input_status"
/* Same as read_coil_status but reads the slaves input table */
static elErr_t *libmb_read_input_status(
	elIo_t *io,
	int slave, int start_addr, int count, byte *data_dest,
	int *p_status
)
{
	EL_ERR_VAR;
	int function = 0x02;

	EL_ERR_CHECK(
		libmb_read_io_status(
			io, slave, function, start_addr, count, data_dest, p_status
		)
	);

	if (*p_status > 0)
		*p_status = count;

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"libmb_preset_response"
/* Gets the raw data from the input stream */
static elErr_t *libmb_preset_response(elIo_t *io, unsigned char *query, int *p_ret)
{
	EL_ERR_VAR;
	unsigned char	response[MAX_PACKET_SIZE];

	EL_ERR_CHECK( libmb_modbus_response(io, query, response, p_ret) );

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"libmb_force_multiple_coils"
/* Takes an array of ints and sets or resets the coils on a slave
   appropriatly */
static elErr_t *libmb_force_multiple_coils(
	elIo_t *io,
	int slave, int start_addr, int coil_count, byte *data_src,
	int *p_status
)
{
	EL_ERR_VAR;
	int function = 0x0F;
	int byte_count;
	int query_size;
	int query_ret;

	unsigned char query[MAX_PACKET_SIZE];

	if (coil_count > MAX_WRITE_COILS) {
		printf("WARNING Writing to too many coils\n");
		coil_count = MAX_WRITE_COILS;
	}

	EL_ERR_CHECK(
		libmb_build_request_packet(
			io, slave, function, start_addr, coil_count, query, &query_size
		)
	);
	byte_count = (coil_count / 8) + ((coil_count % 8) ? 1 : 0);
	query[query_size++] = (byte) byte_count;

#if 1
	/* copy source data to query buffer intact (8 outputs per byte): */
	memcpy(query+query_size,data_src,byte_count);
	query_size += byte_count;
#else
	{	/* inner block */
	int coil_check = 0;
	int i;

	/* convert 'int's from user buffer to bits in query buffer: */
	for (i = 0; i < byte_count; i++) {
		int bit;
		int pos = 0;

		bit = 0x01;
		query[query_size] = 0;

		while ((bit & 0xFF) && (coil_check++ < coil_count)) {
			if (data_src[pos++])
				query[query_size] |= bit;
			else
				query[query_size] &=~ bit;
			
			bit = bit << 1;
		}
		query_size++;
	}
	}	/* inner block */
#endif

	EL_ERR_CHECK( libmb_modbus_query(io, query, query_size, &query_ret) );
	if ( query_ret > 0 )
		EL_ERR_CHECK( libmb_preset_response(io, query, p_status) );
	else
		*p_status = query_ret;

Error:

	return err;
}

/*
 * -- } -- data/code from free MODBUS library 'libmodbus' --
 */

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"modbus_io_proto_str2id"
static int modbus_io_proto_str2id(TCHAR *str)
{
	register int i;

	for(i = 0; i < EL_ARRAY_CNT(gs_modbus_proto); i++)
		if ( !stricmp(gs_modbus_proto[i].str,str) )
			break;
	return i < EL_ARRAY_CNT(gs_modbus_proto) ? gs_modbus_proto[i].id : -1;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"modbus_io_proto_id2str"
static TCHAR *modbus_io_proto_id2str(register int id)
{
	register int i;

	for(i = 0; i < EL_ARRAY_CNT(gs_modbus_proto); i++)
		if ( gs_modbus_proto[i].id == id )
			break;
	return i < EL_ARRAY_CNT(gs_modbus_proto) ? gs_modbus_proto[i].str : NULL;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"modbus_io_table_str2id"
static int modbus_io_table_str2id(TCHAR *str)
{
	register int i;

	for(i = 0; i < EL_ARRAY_CNT(gs_modbus_table); i++)
		if ( !stricmp(gs_modbus_table[i].str,str) )
			break;
	return i < EL_ARRAY_CNT(gs_modbus_table) ? gs_modbus_table[i].id : -1;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"modbus_io_table_id2str"
static TCHAR *modbus_io_table_id2str(register int id)
{
	register int i;

	for(i = 0; i < EL_ARRAY_CNT(gs_modbus_table); i++)
		if ( gs_modbus_table[i].id == id )
			break;
	return i < EL_ARRAY_CNT(gs_modbus_table) ? gs_modbus_table[i].str : NULL;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"modbus_io_addr_parse"
static bool modbus_io_addr_parse(TCHAR *address,WORD *p_addr,WORD *p_cnt)
{
	TCHAR *p;

	/*
	 * address: addr|addr:cnt|addr1-addr2
	 *	- addr: DI|DO: bit; AI|AO: word
	 */

	if ( !isdigit(*address) )
	{
#if 0
		elLog("%s(): %s: invalid address",__FUNC__,address);
#endif
		return FALSE;
	}

	*p_addr = (WORD) strtol(address,&p,0);
	switch( *p++ )
	{
	case ':':
		*p_cnt = (WORD) strtol(p,NULL,0);
		break;

	case '-':
		*p_cnt = (WORD) (strtol(p,NULL,0) - *p_addr + 1);
		break;

	default:
		*p_cnt = 1;
		break;
	}

	return TRUE;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"modbus_host_addr_parse"
static bool modbus_host_addr_parse(TCHAR *str,TCHAR **p_host,int *p_port)
{
	TCHAR *p;
	TCHAR *p2;

	/*
	 * Default MODBUS port number:
	 * (http://www.modbus.org/docs/Modbus_Application_Protocol_V1_1b.pdf)
	 */
	*p_port = 502;

	/*
	 * 'str': [port@]host
	 */
	p2 = strchr(str,'@');
	if ( p2 )
	{
		*p_port = (int) _tcstol(str,&p,0);
		*p2++ = '\0';
		*p_host = p2;
	}
	else
		*p_host = str;

	if ( !**p_host )
		*p_host = NULL;		/* localhost */

	return TRUE;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elModbusDevAdd"
static elErr_t *elModbusDevAdd(elIoParam_t iop,int proto,TCHAR *device)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoModbus_t		*dio = io->dio;
	elIoModbusDev_t		ndev;
	elIoModbusDev_t		*dev;
	elStr_t				*str1;
	elStr_t				*str2;
	struct sockaddr_in	saddr;
	register int		i;

	elNew(elStr_c,&str1);
	elNew(elStr_c,&str2);

	EL_ERR_CHECK( elMtxLock(&gs.mtx) );

	memset(&ndev,0,sizeof(ndev));
	ndev.proto = proto;

	/*
	 * Parse 'device':
	 *
	 *		RTU/ASCII:	COM-port[{,/}attrs]
	 *		TCP/UDP:	[port@]host
	 */
	EL_ERR_CHECK( str1->Cpy(str1,device) );		/* temporary copy */
	switch( proto )
	{
	case EL_IO_MODBUS_PROTO_RTU:
	case EL_IO_MODBUS_PROTO_ASCII:
		EL_ERR_THROW(
			EL_ERR_INVAL,NULL,
			elStrTmpFmt(
				_T("%s: protocol currently not supported"),
				modbus_io_proto_id2str(proto)
			)
		);
		break;

	case EL_IO_MODBUS_PROTO_TCP:
	case EL_IO_MODBUS_PROTO_UDP:
	{	/* inner block */
		TCHAR	*addr;
		TCHAR	*host;
		int		port;

		if ( !modbus_host_addr_parse(str1->ptr,&host,&port) )
		{
			EL_ERR_THROW(
				EL_ERR_INVAL,NULL,
				elStrTmpFmt(_T("%s: error parsing host address"),host)
			);
		}

		io->DebugLog(
			io,1,
			_T("%s(): host: '%s', port: %d\n"),_T(__FUNC__),host,port
		);

		/* translate host name / IP address to IP address: */
		EL_ERR_CHECK( str2->Printf(str2,_T("%d@%s"),port,host) );
		EL_ERR_CHECK(
			elSocketAddrStr2Val(EL_SOCKET_CONNECT,str2->ptr,
				(struct sockaddr *)&saddr)
		);
		addr = elSocketAddrVal2Str((struct sockaddr *)&saddr,sizeof(saddr));
		addr = strchr(addr,'@') + 1;	/* skip port to IP address */

		ndev.u.tcp.addr = addr;
		ndev.u.tcp.port = port;
		break;
	}	/* inner block */
	}

	/*
	 * See if device having the same protocol and attributes (IP address /
	 * / serial port, TCP/UDP port, etc.) is already present/opened/connected:
	 */
	for(i = 0; i < gs.dev_acnt; i++)
	{
		if ( gs.dev[i]->proto == ndev.proto )
		{
			bool f_exists = FALSE;

			switch( proto )
			{
			case EL_IO_MODBUS_PROTO_RTU:
			case EL_IO_MODBUS_PROTO_ASCII:
				break;
			case EL_IO_MODBUS_PROTO_TCP:
			case EL_IO_MODBUS_PROTO_UDP:
				f_exists = (bool) (
					!_tcscmp(gs.dev[i]->u.tcp.addr,ndev.u.tcp.addr)
					&& gs.dev[i]->u.tcp.port == ndev.u.tcp.port
				);
				break;
			}

			if ( f_exists )
				break;
		}
	}
	dev = i < gs.dev_acnt ? gs.dev[i] : NULL;

	if ( !dev )		/* device not opened/connected yet ? */
	{
		/* allocate new device: */
		EL_ERR_CHECK( elMemAlloc(&dev,sizeof(*dev)) );

		memset(dev,0,sizeof(*dev));
		dev->proto = ndev.proto;

		EL_ERR_CHECK( elMemStrdup(&dev->name,device) );

		switch( proto )
		{
		case EL_IO_MODBUS_PROTO_TCP:
		case EL_IO_MODBUS_PROTO_UDP:
			for(i = 0; i < gs.dev_acnt; i++)
				if ( gs.dev[i]->proto == ndev.proto )
					break;
			/* no MODBUS TCP/UDP device opened/connected yet ? */
			if ( i == gs.dev_acnt )
				EL_ERR_CHECK( elSocketStartup() );

			dev->u.tcp.connected = FALSE;
			EL_MEM_CHECK_CRT();
			EL_ERR_CHECK( elMemStrdup(&dev->u.tcp.addr,ndev.u.tcp.addr) );
			EL_MEM_CHECK_CRT();
			dev->u.tcp.port	= ndev.u.tcp.port;

			EL_ERR_CHECK(
				elSocketAddrStr2Val(
					EL_SOCKET_CONNECT,
					elStrTmpFmt("%d@%s",dev->u.tcp.port,dev->u.tcp.addr),
					(struct sockaddr *)&saddr
				)
			);
			EL_ERR_CHECK( elSocketCreate(&dev->u.tcp.s) );
#if EL_IO_SIMULATE
			;
#else
#if 1
			EL_ERR_CHECK(
				elSocketConnect(
					dev->u.tcp.s,(struct sockaddr *)&saddr,sizeof(saddr))
			);
			dev->u.tcp.connected = TRUE;
#endif
#endif

			break;
		}

		/* see if there is empty slot in 'gs.dev[]': */
		for(i = 0; i < gs.dev_acnt; i++)
			if ( !gs.dev[i] )
				break;
		if ( i == gs.dev_acnt )		/* no empty slot ? */
		{
			size_t size;

			size = (++gs.dev_acnt) * sizeof(*gs.dev);
			EL_ERR_CHECK( elMemRealloc(&gs.dev,size) );
		}

		EL_MEM_CHECK_CRT();

		EL_ERR_CHECK( elMtxNew(&dev->lock,NULL) );

		gs.dev[i] = dev;
		gs.dev_cnt++;

	}	/* if ( !dev ) */

	dio->dev = dev;
	dev->ref_cnt++;

Error:

	elMtxUnlock(&gs.mtx);

	elDelete(&str1);
	elDelete(&str2);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elModbusDevRemove"
static elErr_t *elModbusDevRemove(elIoParam_t iop)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoModbus_t		*dio = io->dio;
	elIoModbusDev_t		*dev = dio->dev;
	register int		i;

	EL_ERR_CHECK( elMtxLock(&gs.mtx) );

	for(i = 0; i < gs.dev_acnt; i++)
		if ( gs.dev[i] == dev )
			break;
	EL_ERR_CHECK_COND( i == gs.dev_acnt, -1,NULL );
	
	/* last reference to the device ? */
	if ( !--dev->ref_cnt )
	{
		/* last reference: disconnect from MODBUS TCP/IP controller: */
		switch( dev->proto )
		{
		case EL_IO_MODBUS_PROTO_TCP:
		case EL_IO_MODBUS_PROTO_UDP:
#if EL_IO_SIMULATE
#else
			EL_ERR_CHECK(
				elSocketShutdown(dev->u.tcp.s,EL_SOCKET_INPUT_OUTPUT)
			);
			EL_ERR_CHECK( elSocketClose(dev->u.tcp.s) );
#endif
			elMemFree(&dev->u.tcp.addr);
			break;
		}

		elMemFree(&dev->name);

		EL_ERR_CHECK( elMtxDelete(&dev->lock) );

		elMemFree(&dio->dev);
		gs.dev[i] = NULL;
		gs.dev_cnt--;

		if ( !gs.dev_cnt )	/* last device closed ? */
		{
			EL_ERR_CHECK( elSocketCleanup() );
			elMemFree(&gs.dev);
		}

	}	/* if ( !--dev->ref_cnt ) */

	EL_ERR_CHECK( elMtxUnlock(&gs.mtx) );

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoModbusCtor"
static elErr_t *elIoModbusCtor(elObjPtr_t *po,va_list args)
{
	EL_ERR_VAR;
	elIo_t *io = (elIo_t *)*po;

	(void) args;
	(void) io;

	;

#if 0
Error:
#endif

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoModbusDtor"
static elErr_t *elIoModbusDtor(elObjPtr_t *po)
{
	EL_ERR_VAR;
	elIo_t *io = (elIo_t *)*po;

	(void) io;

	;

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoModbusOpen"
elErr_t *elIoModbusOpen(elIoParam_t iop,TCHAR *rsrc,...)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoModbus_t		*dio;
	elStr_t				*s;
	TCHAR				*p;
	TCHAR				*proto;
	TCHAR				*device;
	TCHAR				*table;
	TCHAR				*address;
	int					proto_id, table_id;
	WORD				addr, cnt;
	int					n, i1, i2, i3;

	(void) rsrc;

	EL_ERR_CHECK( elNew(elIoModbus_c,&io->dio) );
	EL_ERR_CHECK( elNew(elStr_c,&s) );

	dio = io->dio;
	dio->io = io;

	/*
	 * Parse I/O path string ('io->str.path'):
	 *
	 *		protocol:device/table/address
	 *
	 *		protocol: {RTU|ASCII|TCP|UDP}
	 *		device: {host|IP-address|COM-port}
	 *		address: addr|addr:cnt|addr1-addr2
	 *		 - addr: DI|DO: bit; AI|AO: word
	 *
	 * For example:
	 *
	 *		TCP:192.168.5.101/DO/6
	 *		UDP:502@dvi.elcom.cz/DI/20
	 *		RTU:COM1/DI/20
	 *		ASCII:COM1/DI/20
	 *
	 * Modbus resource names:
	 *
	 *	Modbus:RTU   - RS-232 (serial) binary
	 *	Modbus:ASCII - RS-232 (serial) ASCII
	 *	Modbus:TCP   - TCP binary
	 *	Modbus:UDP   - UDP binary
	 *
	 * See:
	 *
	 *	http://en.wikipedia.org/wiki/Modbus
	 *	http://www.modbus.org/specs.php
	 */
	EL_ERR_CHECK( elStrCpy(s,io->str.path->ptr) );
	p = s->ptr;
	i1 = i2 = i3 = -1;
	n = _stscanf(p,"%*[^:]:%n%*[^/]/%n%*[^/]/%n%*s",&i1,&i2,&i3);
	if ( i1 == -1 || i2 == -1 || i3 == -1 )
	{
		EL_ERR_THROW(
			EL_ERR_INVAL,NULL,
			elStrTmpFmt(_T("%s: invalid MODBUS I/O resource path"),
				io->str.path->ptr)
		);
	}
	p[i1-1] = _T('\0');
	p[i2-1] = _T('\0');
	p[i3-1] = _T('\0');
	proto	= p;
	device	= p + i1;
	table	= p + i2;
	address	= p + i3;

	proto_id = modbus_io_proto_str2id(proto);
	if ( proto_id < 0 )
	{
		EL_ERR_THROW(
			EL_ERR_INVAL,NULL,
			elStrTmpFmt(_T("%s: unknown MODBUS I/O protocol"),proto)
		);
	}

	table_id = modbus_io_table_str2id(table);
	if ( table_id < 0 )
	{
		EL_ERR_THROW(
			EL_ERR_INVAL,NULL,
			elStrTmpFmt(_T("%s: unknown MODBUS I/O table"),table)
		);
	}

	switch( table_id )
	{
	case EL_IO_MODBUS_TABLE_INPUT_COIL:
	case EL_IO_MODBUS_TABLE_INPUT_REGISTER:
		io->attrs = EL_IO_INPUT;	/* input device only */
		break;
#if 1
	case EL_IO_MODBUS_TABLE_OUTPUT_COIL:
	case EL_IO_MODBUS_TABLE_OUTPUT_REGISTER:
#if 1
		io->attrs = EL_IO_OUTPUT;	/* output device only */
#else
		io->attrs = EL_IO_INPUT | EL_IO_OUTPUT;		/* input and output device */
#endif
		break;
#endif
	}

	if ( !modbus_io_addr_parse(address,&addr,&cnt) )
	{
		EL_ERR_THROW(
			EL_ERR_INVAL,NULL,
			elStrTmpFmt(_T("%s: invalid MODBUS I/O address"),address)
		);
	}

	dio->table_id	= (BYTE) table_id;
	dio->addr		= addr;
	dio->cnt		= cnt;
	dio->tid		= 0;

	switch( proto_id )
	{
	case EL_IO_MODBUS_PROTO_RTU:
	case EL_IO_MODBUS_PROTO_ASCII:
		dio->hdr_len	= HEADER_LENGTH_RTU;
		dio->tid		= CHECKSUM_SIZE_RTU;
		break;
	case EL_IO_MODBUS_PROTO_TCP:
	case EL_IO_MODBUS_PROTO_UDP:
		dio->hdr_len	= HEADER_LENGTH_TCP;
		dio->tid		= CHECKSUM_SIZE_TCP;
		break;
	}

	EL_IO_DEBUG_CODE(
		io->DebugLog(io,1,_T("%s(): %s:\n"),_T(__FUNC__),io->str.path->ptr);
		io->DebugLog(io,1,_T("  table_id: %d (%s), addr: %u, cnt: %u\n"),
			dio->table_id,modbus_io_table_id2str(dio->table_id),dio->addr,dio->cnt);
	);

	EL_ERR_CHECK( elModbusDevAdd(io,proto_id,device) );

	io->item_size.bits	= dio->cnt;
	if(io->item_size.bits>1)
		io->attrs |= EL_IO_GROUP;

	io->item_size.bytes	= (dio->cnt + 7) / 8;
#if 1
	io->item_size.bpb	= MIN(io->item_size.bits,8);
#else
	io->item_size.bpb	= 1;
#endif

	EL_ERR_CHECK( elIoBufAlloc(&io->buf,io->item_size.bytes) );
	io->buf.item_size = io->item_size;

Error:

	elDelete(&s);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoModbusClose"
elErr_t *elIoModbusClose(elIoParam_t iop)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;

	EL_ERR_CHECK( elModbusDevRemove(io) );

	elDelete(&io->dio);

	;

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoModbusRead"
elErr_t *elIoModbusRead(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoModbus_t		*dio = io->dio;
	elIoModbusDev_t		*dev = dio->dev;
	int					len;

	{	/* inner block */

	elTimerStart(&io->timer);

#if EL_IO_SIMULATE

	(void) dev;
	(void) len;

	*buf = 0;
	*plen = bufsize;

#else

	(void) bufsize;

	switch( dev->proto )
	{
	case EL_IO_MODBUS_PROTO_TCP:
	case EL_IO_MODBUS_PROTO_UDP:
		io->DebugLog(
			io,1,_T("%s(): table: %s, addr: %u, cnt: %u\n"),
			_T(__FUNC__),modbus_io_table_id2str(dio->table_id),
			dio->addr,dio->cnt
		);

		EL_ERR_CHECK_COND( !dev->u.tcp.connected, EL_ERR_INVAL,NULL );

		EL_ERR_CHECK( elMtxLock(&dev->lock) );
		EL_ERR_CHECK(
			libmb_read_input_status(io,SLAVE,dio->addr,dio->cnt,buf,&len)
		);
		EL_ERR_CHECK( elMtxUnlock(&dev->lock) );
		break;
	}

	*plen = dio->cnt;

Error:

	;

#endif

	}	/* inner block */

	elTimerStop(&io->timer);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoModbusWrite"
elErr_t *elIoModbusWrite(
	elIoParam_t iop,elByte_t *buf,elUInt32_t bufsize,elUInt32_t *plen
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoModbus_t		*dio = io->dio;
	elIoModbusDev_t		*dev = dio->dev;
	int					len;

	{	/* inner block */

	elTimerStart(&io->timer);

#if EL_IO_SIMULATE

	(void) buf;
	(void) dev;
	(void) len;

	*plen = bufsize;

#else

#if 0
	elMtxLock(&dio->mtx);
#endif

	(void) bufsize;

	switch( dev->proto )
	{
	case EL_IO_MODBUS_PROTO_TCP:
	case EL_IO_MODBUS_PROTO_UDP:
		io->DebugLog(io,1,_T("%s(): MBTWriteCoils(%u,%u) ...\n"),
			_T(__FUNC__),dio->addr,dio->cnt);

		EL_ERR_CHECK_COND( !dev->u.tcp.connected, EL_ERR_INVAL,NULL );

		EL_ERR_CHECK( elMtxLock(&dev->lock) );
		EL_ERR_CHECK(
			libmb_force_multiple_coils(io,SLAVE,dio->addr,dio->cnt,buf,&len)
		);
		EL_ERR_CHECK( elMtxUnlock(&dev->lock) );
		break;
	}

	*plen = dio->cnt;

#if 0
	elMtxUnlock(&dio->mtx);
#endif

Error:

	;

#endif

	}	/* inner block */

	elTimerStop(&io->timer);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoModbusCtrl"
elErr_t *elIoModbusCtrl(elIoParam_t iop,int cmd,void *pdata)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;

	(void) io;
	(void) cmd;
	(void) pdata;

	;

#if 0
Error:
#endif

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoModbusDevLock"
elErr_t *elIoModbusDevLock(elIoParam_t iop)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoModbus_t		*dio = io->dio;
	elIoModbusDev_t		*dev = dio->dev;

	elMtxLock(&dev->lock);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoModbusDevUnlock"
elErr_t *elIoModbusDevUnlock(elIoParam_t iop)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoModbus_t		*dio = io->dio;
	elIoModbusDev_t		*dev = dio->dev;

	elMtxUnlock(&dev->lock);

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoModbusTaskRead"
elErr_t *elIoModbusTaskRead(
	elIoTask_t	*task,
	elIoParam_t	iop,
	elByte_t	*buf,		/* buffer for data items */
	elUInt32_t	cnt,		/* number of data items to read */
	elUInt32_t	*p_cnt		/* actual number of data items read */
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoTaskRW_t		*task_rw;
	elIoModbus_t		*dio;
	elIoTaskIntfc_t		*intfc;
	elIoTaskDev_t		*tdev;
	elIoModbusTaskDev_t	*idev;	/* interface-specific device data */
	elUInt32_t			size;
	register int		i;

	(void) buf;
	(void) cnt;
	(void) p_cnt;

	task_rw = &task->rw[EL_IO_TASK_READ];

	/* get index of the I/O interface in the task: */
	for(i = 0; i < task_rw->intfc_cnt; i++)
		if ( task_rw->intfc[i].ptr == io->intfc )
			break;
	EL_ERR_ASSERT( i < task_rw->intfc_cnt );

	/*
	 * Add Modbus device to the list of unique devices (if not present yet):
	 */
	dio = io->dio;
	intfc = &task_rw->intfc[i];
	tdev = NULL;
	idev = NULL;
	for(i = 0; i < intfc->dev_cnt; i++)
	{
		tdev = &intfc->dev[i];
		idev = tdev->data;
		if ( idev->ptr == dio->dev )
			break;	/* device already in list */
	}
	if ( i == intfc->dev_cnt )
	{
		size = (1 + intfc->dev_cnt) * sizeof(*intfc->dev);
		EL_ERR_CHECK( elMemRealloc(&intfc->dev,size) );
		intfc->dev_cnt++;
		tdev = &intfc->dev[i];
		memset(tdev,0,sizeof(*tdev));
		EL_ERR_CHECK( elIoBufInit(&tdev->io_buf) );
		EL_ERR_CHECK( elIoBufInit(&tdev->buf) );

		/* allocate interface-specific device's data: */
		EL_ERR_CHECK( elMemAllocZero(&tdev->data,sizeof(*idev)) );
		idev = tdev->data;
		idev->ptr = dio->dev;

		idev->addr_min = (1 << (sizeof(idev->addr_min) * 8)) - 1;
		idev->addr_max = 0;
		idev->table_id = dio->table_id;
	}

	if ( dio->addr < idev->addr_min )
		idev->addr_min = dio->addr;
	if ( dio->addr > idev->addr_max )
		idev->addr_max = dio->addr;
	idev->cnt = (WORD) ((idev->addr_max - idev->addr_min) + 1);

	/*
	 * Set task device name (e.g. "192.168.5.101/DO/15-16"):
	 */
	elMemFree(&tdev->name);
	EL_ERR_CHECK(
		elMemStrdup(
			&tdev->name,
			elStrTmpFmt("%s/DI/%d-%d",
				dio->dev->name,idev->addr_min,idev->addr_max
			)
		)
	);

	/* reallocate device I/O buffer: */
	EL_ERR_CHECK( elIoBufAlloc(&tdev->buf,(idev->cnt + 7) / 8) );

	/* add I/O pointer to the device's I/O list: */
	EL_ERR_CHECK(
		elMemRealloc(&tdev->io,(1 + tdev->io_cnt) * sizeof(*tdev->io))
	);
	tdev->io[tdev->io_cnt++] = io;

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoModbusTaskWrite"
elErr_t *elIoModbusTaskWrite(
	elIoTask_t *task,
	elIoParam_t iop,
	elByte_t	*buf,		/* buffer for data items */
	elUInt32_t	cnt,		/* number of data items to read */
	elUInt32_t	*p_cnt		/* actual number of data items read */
)
{
	EL_ERR_VAR;
	EL_IO_PARAM_VAR;
	elIoTaskRW_t		*task_rw;
	elIoModbus_t		*dio;
	elIoTaskIntfc_t		*intfc;
	elIoTaskDev_t		*tdev;
	elIoModbusTaskDev_t	*idev;	/* interface-specific device data */
	elUInt32_t			size;
	register int		i;

	(void) buf;
	(void) cnt;
	(void) p_cnt;

	task_rw = &task->rw[EL_IO_TASK_WRITE];

	/* get index of the I/O interface in the task: */
	for(i = 0; i < task_rw->intfc_cnt; i++)
		if ( task_rw->intfc[i].ptr == io->intfc )
			break;
	EL_ERR_ASSERT( i < task_rw->intfc_cnt );

	/*
	 * Add Modbus device to the list of unique devices (if not present yet):
	 */
	dio = io->dio;
	intfc = &task_rw->intfc[i];
	tdev = NULL;
	idev = NULL;
	for(i = 0; i < intfc->dev_cnt; i++)
	{
		tdev = &intfc->dev[i];
		idev = tdev->data;
		if ( idev->ptr == dio->dev )
			break;	/* device already in list */
	}
	if ( i == intfc->dev_cnt )
	{
		size = (1 + intfc->dev_cnt) * sizeof(*intfc->dev);
		EL_ERR_CHECK( elMemRealloc(&intfc->dev,size) );
		intfc->dev_cnt++;
		tdev = &intfc->dev[i];
		memset(tdev,0,sizeof(*tdev));
		EL_ERR_CHECK( elIoBufInit(&tdev->buf) );

		/* allocate interface-specific device's data: */
		EL_ERR_CHECK( elMemAllocZero(&tdev->data,sizeof(*idev)) );
		idev = tdev->data;
		idev->ptr = dio->dev;

		idev->addr_min = (1 << (sizeof(idev->addr_min) * 8)) - 1;
		idev->addr_max = 0;
		idev->table_id = dio->table_id;
	}

	if ( dio->addr < idev->addr_min )
		idev->addr_min = dio->addr;
	if ( dio->addr > idev->addr_max )
		idev->addr_max = dio->addr;
	idev->cnt = (WORD) ((idev->addr_max - idev->addr_min) + 1);

	/*
	 * Set task device name (e.g. "192.168.5.101/DO/15-16"):
	 */
	elMemFree(&tdev->name);
	EL_ERR_CHECK(
		elMemStrdup(
			&tdev->name,
			elStrTmpFmt("%s/DO/%d-%d",
				dio->dev->name,idev->addr_min,idev->addr_max
			)
		)
	);

	/*
	 * Reallocate output I/O buffer shared by all I/O resources
	 * in this task device:
	 */
	tdev->io_buf.size = idev->cnt;
	EL_ERR_CHECK( elIoBufAlloc(&tdev->io_buf,tdev->io_buf.size) );

	/*
	 * Reallocate output device I/O buffer:
	 */
	EL_ERR_CHECK( elIoBufAlloc(&tdev->buf,(idev->cnt + 7) / 8) );

	/* add I/O pointer to the device's I/O list: */
	EL_ERR_CHECK(
		elMemRealloc(&tdev->io,(1 + tdev->io_cnt) * sizeof(*tdev->io))
	);
	tdev->io[tdev->io_cnt++] = io;

	/*
	 * Set I/O buffer pointer of all I/O resources to the shared task device
	 * I/O buffer so that I/O resources that share the same DIO line will
	 * update/overwrite the same data space. This is important in case
	 * the data are written to memory (I/O buffer) in particular order first
	 * and then sent to the device at once from the same memory location.
	 * This way we can assure the final value of the shared DIO lines
	 * that gets written to the device. Example of possible I/O resources
	 * that share the same DIO line(s):
	 *
	 *		WS1_filt_dist_light_green	Modbus:TCP:192.168.5.101/DO/0
	 *		WS1_filt_dist_light_red		Modbus:TCP:192.168.5.101/DO/1
	 *		WS1_filt_dist_light_orange	Modbus:TCP:192.168.5.101/DO/0-1
	 */
	for(i = 0; i < tdev->io_cnt; i++)
	{
		io = tdev->io[i];
		dio = io->dio;
		io->buf.ptr = tdev->io_buf.ptr + (idev->cnt - dio->addr);
	}

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoModbusTaskExecute"
elErr_t *elIoModbusTaskExecute(elIoTask_t *task)
{
	EL_ERR_VAR;
	elIoTaskRW_t		*task_rw;
	elIo_t				*io;
	elIoTaskIntfc_t		*intfc;
	elIoTaskDev_t		*tdev;
	elIoModbusTaskDev_t	*idev;
	elIoModbusDev_t		*dev;
	int					len;
	register int		i, dev_idx;

#if 0
	elLogD(_T("%s(): start"),_T(__FUNC__));
#endif

	switch( task->fn )
	{
	case EL_IO_TASK_FN_READ:
	case EL_IO_TASK_FN_WRITE:

		task_rw = &task->rw[task->fn - EL_IO_TASK_FN_READ];

		for(i = 0; i < task_rw->intfc_cnt; i++)
			if ( task_rw->intfc[i].ptr == elIoModbusIntfc )
				break;
		EL_ERR_ASSERT( i < task_rw->intfc_cnt );
		intfc = &task_rw->intfc[i];

		/* for each MODBUS device: */
		for(dev_idx = 0; dev_idx < intfc->dev_cnt; dev_idx++)
		{
			tdev = &intfc->dev[dev_idx];

			elTimerStart(&tdev->t_val);
			tdev->t_stamp = tdev->t_val;
			idev = tdev->data;
			dev = idev->ptr;
			io = tdev->io[0];

			switch( task->fn )
			{
			case EL_IO_TASK_FN_READ:
				if ( io->dio && dev )
				{
#if EL_IO_SIMULATE
					(void) len;
					memset(tdev->buf.ptr,0,tdev->buf.size);
#else
					EL_ERR_CHECK( elMtxLock(&dev->lock) );
					EL_ERR_CHECK(
						libmb_read_input_status(
							io,SLAVE,idev->addr_min,(WORD)idev->cnt,
							tdev->buf.ptr,&len
						)
					);
					EL_ERR_CHECK( elMtxUnlock(&dev->lock) );
#endif
				}

				/* convert bits (that have been read from MODBUS device) to bytes (I/O buffer): */
				for(i = 0; i < tdev->io_cnt; i++)
				{
					elIoModbus_t	*dio;
					register int	bit;

					io = tdev->io[i];
					dio = io->dio;

					bit = dio->addr - idev->addr_min;
#if 0
					elLogD(_T("%s(): *io->buf.ptr = (buf[%d] >> %d) & 1"),
						__FUNC__,bit / 8,bit % 8);
#endif
					*(byte *)io->buf.ptr = (byte) ((tdev->buf.ptr[bit/8] >> (bit % 8)) & 1);
				}
				break;

			case EL_IO_TASK_FN_WRITE:
				if ( io->dio && dev )
				{
					/* convert bytes (I/O buffer) to bits (to be written to MODBUS device): */
					for(i = 0; i < tdev->io_cnt; i++)
					{
						elIoModbus_t	*dio;
						register int	bit;

						io = tdev->io[i];
						dio = io->dio;

						bit = dio->addr - idev->addr_min;
						if ( *(byte *)io->buf.ptr )
							tdev->buf.ptr[bit/8] |=  (1 << (bit % 8));
						else
							tdev->buf.ptr[bit/8] &= ~(1 << (bit % 8));
					}

#if EL_IO_SIMULATE
					(void) len;
#else
					EL_ERR_CHECK( elMtxLock(&dev->lock) );
					EL_ERR_CHECK(
						libmb_force_multiple_coils(
							io,SLAVE,idev->addr_min,(WORD)idev->cnt,tdev->buf.ptr,&len
						)
					);
					EL_ERR_CHECK( elMtxUnlock(&dev->lock) );
#endif
				}
				break;

			}	/* switch( irw ) */
			
			elTimerStop(&tdev->t_val);
		}	/* for(dev_idx = 0; dev_idx < intfc->dev_cnt; dev_idx++) */
		break;

	}	/* switch( fn ) */

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elIoModbusTaskDelete"
elErr_t *elIoModbusTaskDelete(elIoTask_t *task)
{
	EL_ERR_VAR;
	elIoTaskRW_t	*task_rw;
	elIoTaskIntfc_t	*intfc;
	elIoTaskDev_t	*tdev;
	register int	i, j, irw;

	for(irw = 0; irw < EL_ARRAY_CNT(task->rw); irw++)
	{
		task_rw = &task->rw[irw];
		for(i = 0; i < task_rw->intfc_cnt; i++)
		{
			intfc = &task_rw->intfc[i];
			for(j = 0; j < intfc->dev_cnt; j++)
			{
				tdev = &intfc->dev[j];

				elMemFree(&tdev->data);

				elIoBufFree(&tdev->io_buf);
				elIoBufFree(&tdev->buf);
			}
			elMemFree(&intfc->dev);
		}
	}

	return err;
}

/******************************************************************************/
