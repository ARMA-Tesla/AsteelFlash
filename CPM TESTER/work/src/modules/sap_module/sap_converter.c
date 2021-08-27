#include "sap_converter.h"
#include <time.h>
#include <multithread/multithread.h>
//#include <ansi_c.h>

#define SAP_CONVERTER_TIMEOUT	5000
#define SAP_CONVERTER_ACK		0xEC

static unsigned char gs_timeout_key[3] = { 0x12, 0xf0, 0x12 };

/* http://www.barcodeman.com/altek/mule/scandoc.php */
static unsigned char    gs_keyb_scan_codes[255][7] =
{
/* ND */	{0,0,0,0,0,0,0 },   //0
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{3, 0x5A,0xF0,0x5A,0,0,0 },	  //10
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{3, 0x5A,0xF0,0x5A,0,0,0 },  // 13
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },	
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 }, // 30
/* ND */	{0,0,0,0,0,0,0 },
/*   */		{3, 0x29, 0xf0, 0x29, 0, 0, 0},
/* ! */		{6, 0x12, 0x16, 0xf0, 0x16, 0xf0, 0x12},
/* " */		{6, 0x12, 0x52, 0xf0, 0x52, 0xf0, 0x12},
/* # */		{6, 0x12, 0x26, 0xf0, 0x26, 0xf0, 0x12},
/* $ */		{6, 0x12, 0x25, 0xf0, 0x25, 0xf0, 0x12},
/* % */		{6, 0x12, 0x2e, 0xf0, 0x2e, 0xf0, 0x12},
/* & */		{6, 0x12, 0x3d, 0xf0, 0x3d, 0xf0, 0x12},
/* ND */	{0,0,0,0,0,0,0 },
/* ( */		{6, 0x12, 0x46, 0xf0, 0x46, 0xf0, 0x12}, // 40
/* ) */		{6, 0x12, 0x45, 0xf0, 0x45, 0xf0, 0x12},
/* * */		{6, 0x12, 0x3e, 0xf0, 0x3e, 0xf0, 0x12},
/* + */		{0,0,0,0,0,0,0 },
/* , */		{3, 0x41, 0xf0, 0x41, 0, 0, 0},
/* - */		{3, 0x4e, 0xf0, 0x4e, 0, 0, 0},
/* . */		{3, 0x49, 0xf0, 0x49, 0, 0, 0},
/* / */		{3, 0x4a, 0xf0, 0x4a, 0, 0, 0},
/* 0 */		{3, 0x45, 0xf0, 0x45, 0, 0, 0},
/* 1 */		{3, 0x16, 0xf0, 0x16, 0, 0, 0},
/* 2 */		{3, 0x1e, 0xf0, 0x1e, 0, 0, 0}, // 50
/* 3 */		{3, 0x26, 0xf0, 0x26, 0, 0, 0},
/* 4 */		{3, 0x25, 0xf0, 0x25, 0, 0, 0},
/* 5 */		{3, 0x2e, 0xf0, 0x2e, 0, 0, 0},
/* 6 */		{3, 0x36, 0xf0, 0x36, 0, 0, 0},
/* 7 */		{3, 0x3d, 0xf0, 0x3d, 0, 0, 0},
/* 8 */		{3, 0x3e, 0xf0, 0x3e, 0, 0, 0},
/* 9 */		{3, 0x46, 0xf0, 0x46, 0, 0, 0},
/* : */		{6, 0x12, 0x4c, 0xf0, 0x4c, 0xf0, 0x12},
/* ; */		{3, 0x4c, 0xf0, 0x4c, 0, 0, 0},
/* < */		{6, 0x12, 0x41, 0xf0, 0x41, 0xf0, 0x12},
/* = */		{0,0,0,0,0,0,0 },
/* > */		{6, 0x12, 0x49, 0xf0, 0x49, 0xf0, 0x12},
/* ? */		{6, 0x12, 0x4a, 0xf0, 0x4a, 0xf0, 0x12},
/* @ */		{6, 0x12, 0x1e, 0xf0, 0x1e, 0xf0, 0x12},
/* A */		{6, 0x12, 0x1c, 0xf0, 0x1c, 0xf0, 0x12},
/* B */		{6, 0x12, 0x32, 0xf0, 0x32, 0xf0, 0x12},
/* C */		{6, 0x12, 0x21, 0xf0, 0x21, 0xf0, 0x12},
/* D */		{6, 0x12, 0x23, 0xf0, 0x23, 0xf0, 0x12},
/* E */		{6, 0x12, 0x24, 0xf0, 0x24, 0xf0, 0x12},
/* F */		{6, 0x12, 0x2b, 0xf0, 0x2b, 0xf0, 0x12},
/* G */		{6, 0x12, 0x34, 0xf0, 0x34, 0xf0, 0x12},
/* H */		{6, 0x12, 0x33, 0xf0, 0x33, 0xf0, 0x12},
/* I */		{6, 0x12, 0x43, 0xf0, 0x43, 0xf0, 0x12},
/* J */		{6, 0x12, 0x3b, 0xf0, 0x3b, 0xf0, 0x12},
/* K */		{6, 0x12, 0x42, 0xf0, 0x42, 0xf0, 0x12},
/* L */		{6, 0x12, 0x4b, 0xf0, 0x4b, 0xf0, 0x12},
/* M */		{6, 0x12, 0x3a, 0xf0, 0x3a, 0xf0, 0x12},
/* N */		{6, 0x12, 0x31, 0xf0, 0x31, 0xf0, 0x12},
/* O */		{6, 0x12, 0x44, 0xf0, 0x44, 0xf0, 0x12},
/* P */		{6, 0x12, 0x4d, 0xf0, 0x4d, 0xf0, 0x12},
/* Q */		{6, 0x12, 0x15, 0xf0, 0x15, 0xf0, 0x12},
/* R */		{6, 0x12, 0x2d, 0xf0, 0x2d, 0xf0, 0x12},
/* S */		{6, 0x12, 0x1b, 0xf0, 0x1b, 0xf0, 0x12},
/* T */		{6, 0x12, 0x2c, 0xf0, 0x2c, 0xf0, 0x12},				  
/* U */		{6, 0x12, 0x3c, 0xf0, 0x3c, 0xf0, 0x12},
/* V */		{6, 0x12, 0x2a, 0xf0, 0x2a, 0xf0, 0x12},
/* W */		{6, 0x12, 0x1d, 0xf0, 0x1d, 0xf0, 0x12},
/* X */		{6, 0x12, 0x22, 0xf0, 0x22, 0xf0, 0x12},
/* Y */		{6, 0x12, 0x35, 0xf0, 0x35, 0xf0, 0x12},
/* Z */		{6, 0x12, 0x1a, 0xf0, 0x1a, 0xf0, 0x12},
/* [ */		{3, 0x54, 0xf0, 0x54, 0, 0, 0},
/* \ */		{0,0,0,0,0,0,0 },
/* ] */		{3, 0x5b, 0xf0, 0x5b, 0, 0, 0},
/* ^ */		{6, 0x12, 0x36, 0xf0, 0x36, 0xf0, 0x12},
/* _ */		{6, 0x12, 0x4e, 0xf0, 0x4e, 0xf0, 0x12},
/* ` */		{0,0,0,0,0,0,0 },
/* a */		{3, 0x1c, 0xf0, 0x1c, 0, 0, 0},
/* b */		{3, 0x32, 0xf0, 0x32, 0, 0, 0},
/* c */		{3, 0x21, 0xf0, 0x21, 0, 0, 0},
/* d */		{3, 0x23, 0xf0, 0x23, 0, 0, 0},				 
/* e */		{3, 0x24, 0xf0, 0x24, 0, 0, 0},
/* f */		{3, 0x2b, 0xf0, 0x2b, 0, 0, 0},
/* g */		{3, 0x34, 0xf0, 0x34, 0, 0, 0},
/* h */		{3, 0x33, 0xf0, 0x33, 0,},
/* i */		{3, 0x43, 0xf0, 0x43},
/* j */		{3, 0x3b, 0xf0, 0x3b},
/* k */		{3, 0x42, 0xf0, 0x42},
/* l */		{3, 0x4b, 0xf0, 0x4b},
/* m */		{3, 0x3a, 0xf0, 0x3a},
/* n */		{3, 0x31, 0xf0, 0x31},
/* o */		{3, 0x44, 0xf0, 0x44},
/* p */		{3, 0x4d, 0xf0, 0x4d},
/* q */		{3, 0x15, 0xf0, 0x15},
/* r */		{3, 0x2d, 0xf0, 0x2d},
/* s */		{3, 0x1b, 0xf0, 0x1b},
/* t */		{3, 0x2c, 0xf0, 0x2c},
/* u */		{3, 0x3c, 0xf0, 0x3c},
/* v */		{3, 0x2a, 0xf0, 0x2a},
/* w */		{3, 0x1d, 0xf0, 0x1d},
/* x */		{3, 0x22, 0xf0, 0x22},
/* y */		{3, 0x35, 0xf0, 0x35},
/* z */		{3, 0x1a, 0xf0, 0x1a},
/* { */		{6, 0x12, 0x54, 0xf0, 0x54, 0xf0, 0x12},
/* | */		{0,0,0,0,0,0,0 },
/* } */		{6, 0x12, 0x5b, 0xf0, 0x5b, 0xf0, 0x12},
/* ~ */		{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },	//130
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },
/* ND */	{0,0,0,0,0,0,0 },  //140
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },  //150
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },  //160
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },  //170
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },  //180
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },  //190
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },  //200
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },  //210
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },  //220
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },  //230
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },  //240
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },  //250
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 },       
/* ND */	{0,0,0,0,0,0,0 }
};

/*****************************************************************************
 * Static no-exported functions declaration **********************************
 *****************************************************************************/
static SElExceptionPtr sapconverter_Initialize(
    SSAPConverterPtr*  pSAP
);
static SElExceptionPtr sapconverter_Write(struct _SSAPConverter* me, const char* pData);
static SElExceptionPtr sapconverter_WriteRaw(struct _SSAPConverter* me, size_t length, const unsigned char* pdata);

static SElExceptionPtr sapconverter_Open(struct _SSAPConverter* me);
static SElExceptionPtr sapconverter_Close(struct _SSAPConverter* me);
static SElExceptionPtr sapconverter_StartReading(struct _SSAPConverter* me);
static SElExceptionPtr sapconverter_StopReading(struct _SSAPConverter* me);
static SElExceptionPtr sapconverter_ReadThread(struct _SSAPConverter* me);

DWORD WINAPI Thread_SapConverter(  LPVOID lpParameter);

/*****************************************************************************
 * Exportyed functions definition ********************************************
 *****************************************************************************/

#if defined(_CVI_)
static int sprintf_s(
   		char *buffer,
   		size_t sizeOfBuffer,
   		const char *format,
		...
)
{
	va_list args;
  	va_start (args, format);

	return vsprintf (buffer,format, args);	
}


#endif


#undef __FUNC__
#define __FUNC__ "sapconverter_new"
SElExceptionPtr sapconverter_new(
	SSAPConverterPtr*  pSAP
)
{
	SElExceptionPtr			pexception = NULL;
	SSAPConverterPtr		psap = NULL;

	EXCCHECK( sapconverter_Initialize(&psap));

Error:
	if ( pexception )
	{
		sapconverter_delete( &psap);
	}
	*pSAP = psap;
	
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "sapconverter_delete"
SElExceptionPtr sapconverter_delete(
	SSAPConverterPtr*  pSAP
)
{
	SElExceptionPtr	pexception = NULL;

	if (*pSAP)
	{
		(*pSAP)->Stop(*pSAP);
		
		EnterCriticalSection(&((*pSAP)->lock));		
		if ((*pSAP)->QuitEvent) {
			CloseHandle((*pSAP)->QuitEvent);
			(*pSAP)->QuitEvent = 0;
		}
		if ((*pSAP)->COMHandle) {
			CloseHandle((*pSAP)->COMHandle);
			(*pSAP)->COMHandle = 0;
		}
		LeaveCriticalSection(&((*pSAP)->lock));
		DeleteCriticalSection(&((*pSAP)->lock));
		free(*pSAP);
		*pSAP = NULL;
	} /* if (*pSAP) { ...*/

Error:
	EXCRETHROW( pexception);
}




/*****************************************************************************
 * Static no-exported functions definition ***********************************
 *****************************************************************************/
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "sapconverter_Initialize"
/*!
 * \brief
 * This function creates a new instance of the specified object.
 * 
 * \param pSAP
 * Returns pointer to new instance of the sap converter object.
 *
 * \return	
 * A value indicating whether an error occurred.
 * A negative error code indicates function failure.	
 *
 * This function creates and initialize a new instance of the specified object.
 */
static SElExceptionPtr sapconverter_Initialize(
    SSAPConverterPtr*  pSAP
)
{
	SElExceptionPtr		pexception = NULL;
    SSAPConverterPtr	psap = NULL;

    /*
     * Initialize all data
     */     
    psap = calloc(1, sizeof(SSAPConverter));
    EXCCHECKALLOC( psap);

	psap->Close 		= sapconverter_Close;
	psap->Open 			= sapconverter_Open;
	psap->Start 		= sapconverter_StartReading;
	psap->Stop 			= sapconverter_StopReading;
	psap->Write    		= sapconverter_Write;
	psap->WriteRaw		= sapconverter_WriteRaw;
	psap->_ReadThread	= sapconverter_ReadThread;
	
	InitializeCriticalSection(&psap->lock);
	
Error:
    if ( pexception ) {
		if ( psap != NULL ){
			free(psap);
			psap = NULL;
		}
    }

    if (pSAP) *pSAP = psap;
    EXCRETHROW( pexception);
} /* handbr_Initialize */


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"SapConverter::Write"
/*!
 * \brief
 * This function write data to sap converter. data are translated 
 * to escape characters.
 * 
 * \param pSAP
 * Specifies instance of the SAP Converter
 *
 * \return	
 * A value indicating whether an error occurred.
 * A negative error code indicates function failure.	
 *
 * This function creates and initialize a new instance of the specified object.
 */
static SElExceptionPtr sapconverter_Write(SSAPConverterPtr me, const char* pData)
{
	SElExceptionPtr	pexception = NULL;
	const char	*pdata = pData;
	int32_t			wait = 0;
	OVERLAPPED		ov;

	EnterCriticalSection(&me->lock);	

	memset(&ov, 0, sizeof(ov));
	ov.hEvent = CreateEvent( 0,1,0,0);
	if(ov.hEvent==NULL)
		EXCTHROW( -1, "Create Event Fault");

	for  (; *pdata; pdata++)
	{
		WriteFile(me->COMHandle, (void*) (&(gs_keyb_scan_codes[*pdata][1])), gs_keyb_scan_codes[*pdata][0], NULL, &ov);
	
		wait = WaitForSingleObject(ov.hEvent, 500);
		if (wait == WAIT_TIMEOUT)
			EXCTHROW( SAPC_ERROR_WRITE_TIMEOUT, "Write Timeout");
	}

Error:
	CloseHandle(ov.hEvent);
	LeaveCriticalSection(&me->lock);
	EXCRETHROW(pexception);
} /* sapconverter_Write */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"SapConverter::WriteRaw"
/*!
 * \brief
 * This function write data to sap converter. 
 * 
 * \param pSAP
 * Specifies instance of the SAP Converter
 *
 * \return	
 * A value indicating whether an error occurred.
 * A negative error code indicates function failure.	
 *
 * This function creates and initialize a new instance of the specified object.
 */
static SElExceptionPtr sapconverter_WriteRaw(
	struct _SSAPConverter*	me,
	size_t					length,
	const unsigned char*	pdata
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t	error = 0;
	int32_t			wait = 0;
	OVERLAPPED		ov;

	EnterCriticalSection(&me->lock);	

	memset(&ov, 0, sizeof(ov));
	ov.hEvent = CreateEvent( 0,1,0,0);
	if(ov.hEvent==NULL)
		EXCTHROW( -1, "Create Event Fault");

	EXCCHECKWIN( WriteFile(me->COMHandle, (void*)pdata,length, NULL, &ov));

	wait = WaitForSingleObject(ov.hEvent, 500);
	if (wait == WAIT_TIMEOUT)
		EXCTHROW( SAPC_ERROR_WRITE_TIMEOUT, "Write Timeout");

Error:
	CloseHandle(ov.hEvent);
	LeaveCriticalSection(&me->lock);
	EXCRETHROW(pexception);
} /* sapconverter_WriteRaw */
  
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"SapConverter::StartReading"
static SElExceptionPtr sapconverter_StartReading(struct _SSAPConverter* me)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	
	if ( me->COMHandle == 0 )
	{
		//EXCCHECK( me->Open(me));
	//}
	EXCCHECK( me->WriteRaw(me, sizeof(gs_timeout_key), gs_timeout_key));
	EXCCHECKWIN( ThreadStart( Thread_SapConverter,
							  (void*)me,
							  &me->threadHandle,
							  &me->QuitEvent));
	}
Error:
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"SapConverter::StopReading"
static SElExceptionPtr sapconverter_StopReading(struct _SSAPConverter* me)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;

	if ( me->threadHandle )
	{
		EXCCHECKWIN( ThreadStop(&me->threadHandle, &me->QuitEvent));
	}

Error:
	EXCRETHROW(pexception);
}


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"SapConverter::Open"
static SElExceptionPtr sapconverter_Open(struct _SSAPConverter* me)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	char			message[1024] = "";
	char			file[64] = "";
	DCB				dcb = {0};

	if (me->COM.port > 8)
		sprintf(file, "\\\\.\\COM%d", (int)me->COM.port);
	else
		sprintf(file, "COM%d", (int)me->COM.port);

	
	me->COMHandle = CreateFile( file,
								GENERIC_READ|GENERIC_WRITE,//access ( read and write)
							    0,    					//(share) 0:cannot share the COM port                        
							    0,    					//security  (None)                
							    OPEN_EXISTING,			// creation : open_existing
							    FILE_FLAG_OVERLAPPED,	// we want overlapped operation
							    0						// no templates file for COM port...
		            			);
	
	if (me->COMHandle == INVALID_HANDLE_VALUE)
	{
		DWORD	bcr_error = GetLastError();
		LPVOID	lpMsgBuf;

		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					   NULL, bcr_error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					   (LPTSTR) &lpMsgBuf, 0, NULL );
		sprintf_s(message, sizeof(message), "Serial port \"%s\" can not be open\nError: %s", file, (LPCTSTR) lpMsgBuf);
		EXCTHROW( SAPC_ERROR_OPEN_FAILED, message);
	}
	
	dcb.DCBlength = sizeof(DCB);
	EXCCHECKWIN( GetCommState (me->COMHandle, &dcb));
	
	/* Set default values */
	dcb.BaudRate    = me->COM.baudRate;
	dcb.ByteSize    = me->COM.dataBits;
	dcb.Parity		= me->COM.parity;
	dcb.StopBits    = me->COM.StopBits;
	EXCCHECKWIN( SetCommState( me->COMHandle, &dcb));
	EXCCHECKWIN( SetCommMask( me->COMHandle, EV_RXCHAR));

Error:
	EXCRETHROW(pexception);
}	/* sapconverter_Open */


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"SapConverter::Close"
static SElExceptionPtr sapconverter_Close(struct _SSAPConverter* me)
{
	SElExceptionPtr	pexception = NULL;
	
	EXCCHECK( me->Stop(me));
	
	if ( me->COMHandle )
	{
		CloseHandle( me->COMHandle);
		me->COMHandle = 0;
	}
	
Error:
	EXCRETHROW(pexception);
} /* sapconverter_Close */


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"SapConverter::ReadThread"
static SElExceptionPtr sapconverter_ReadThread(struct _SSAPConverter* me)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	int32_t			wait = 0;
	DWORD			event_mask = 0,
					last_read = 0;
	HANDLE			event_list[2];
	OVERLAPPED		ov,
					ov_read;
	char			read_byte;
	bool_t			is_locked = FALSE;
	clock_t			clk = clock();
	
	event_list[0] = me->QuitEvent;
	memset(&ov, 0, sizeof(ov));
	ov.hEvent = CreateEvent(NULL, 1, 0, NULL);
	if(ov.hEvent==NULL)
		EXCTHROW( -1, "Create Event Fault");

	while (1)
	{
		WaitCommEvent(me->COMHandle, &event_mask, &ov);
		event_list[1] = ov.hEvent;
		wait = WaitForMultipleObjects( 2, event_list, 0, SAP_CONVERTER_TIMEOUT);
		switch (wait)
		{
		case WAIT_TIMEOUT:
			{
				me->WriteRaw(me, sizeof(gs_timeout_key), gs_timeout_key);
			}
			// ADD Send Shift Press
			break;
		case WAIT_OBJECT_0:
			ExitThread(1);
			break;
		case WAIT_OBJECT_0 + 1:
			EXCCHECKWIN( ResetEvent(ov.hEvent));
			EXCCHECKWIN( GetCommMask(me->COMHandle, &event_mask));
			if ((event_mask & EV_RXCHAR) == 0)
			{
				char	message[256];
				/* TODO: log invalid event */
				sprintf(message, "Invalid Read Event Mask (%d)", event_mask);
				EXCTHROW( SAPC_ERROR_INVALID_READ_EVENT, message);
			}

			/* Read data from serial port */
			EnterCriticalSection(&me->lock);
			is_locked = TRUE;
			
			last_read = 0;
			memset(&ov_read, 0, sizeof(ov_read));
			ov_read.hEvent = CreateEvent(NULL, TRUE, 0, NULL);
			
			do
			{
				EXCCHECKWIN( ResetEvent(ov_read.hEvent));
				ReadFile(me->COMHandle, &read_byte, 1, &last_read, &ov_read);
				
				if ( read_byte == SAP_CONVERTER_ACK)
				{
					clk = clock();
				}
				
				if ((((double)( clock() - clk )) / CLOCKS_PER_SEC ) > (SAP_CONVERTER_TIMEOUT / 1000.0))
				{
					clk = clock();
					me->WriteRaw(me, sizeof(gs_timeout_key), gs_timeout_key);
				}
				
			} while (last_read > 0);
			
			LeaveCriticalSection(&me->lock);
			is_locked = FALSE;

			/* Close read event and cancel read function */
			EXCCHECKWIN( CloseHandle(ov_read.hEvent));
			EXCCHECKWIN( CancelIo(me->COMHandle));
			break;
		}
	}
	
Error:
	if(is_locked)
		LeaveCriticalSection(&me->lock);
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"SapConverter::Thread_SapConverter"
DWORD WINAPI Thread_SapConverter(  LPVOID lpParameter)
{
	SSAPConverterPtr	me = (SSAPConverterPtr) lpParameter;
	SElExceptionPtr		pexception = NULL;
	
	EXCDISPLAY( (me->_ReadThread(me)));
	
	return 0;
}
