#include "msxmldom.h"
#include "dbs.h"
#include "dbs_kosu.h"
#include <utility.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <guid.h>
#include "dbs_define_local.h"

/*---------------------------------------------------------------------------*/
static SElExceptionPtr kosu_Initialize(
	struct _SDBSKOSU*	me, 
	const char*			xml_config
);
static SElExceptionPtr kosu_Lock(
    struct _SDBSKOSU*	me
);
static SElExceptionPtr kosu_Unlock(
    struct _SDBSKOSU*	me
);
static SElExceptionPtr	kosu_GetEndShiftTime(
	struct _SDBSKOSU*	me,
	time_t				begin,
	time_t*				pEndTime
);
static SElExceptionPtr	kosu_GetPauseIntervals(
	struct _SDBSKOSU*	me,
	time_t				t1,
	time_t				t2,
	SIntervalResultPtr*	pResult,
	int32_t*			pPause
);
static bool_t IsNewTime(
	int32_t		actual_h,
	int32_t		actual_m,
	int32_t		config_h,
	int32_t		config_m
);
static uint32_t GetNbUserAtTime( 
	SIntervalResultPtr pUserIntervals,
	time_t			   ts
);
static SElExceptionPtr GetNextUserChange(
	SIntervalResultPtr	pUserIntervals,
	time_t				TS,
	time_t*				pNewTS,
	uint32_t			*pUserCount
);
static SElExceptionPtr kosu_GetUsersTimeInIntervals(
	struct _SDBSKOSU*	me,
	SIntervalResultPtr	pWorkIntervals,
	SUserIntervalsPtr*	pUserIntervalsResult
);
static SElExceptionPtr kosu_GetRequiredPanels(
	struct _SDBSKOSU*	me,
	SUserIntervalsPtr	pUserIntervalsResult,
	double*			pPanels
);
static SElExceptionPtr kosu_GetRequiredKOSU(
	struct _SDBSKOSU*	me,
	uint32_t			UserCount,
	uint32_t*			pKOSUValue
);
static SElExceptionPtr kosu_GetRequiredKOSUAlarm(
	struct _SDBSKOSU*	me,
	uint32_t			alarm,
	uint32_t			UserCount,
	uint32_t*			pKOSUValue
);
static SElExceptionPtr kosu_AddKosu(
	struct _SDBSKOSU* me,
	uint8_t        UserCount,
	uint32_t       KOSU
);
static SElExceptionPtr kosu_AddPause(
	struct _SDBSKOSU* me,
	uint8_t			hour,
	uint8_t			min,
	uint16_t		delay
);
static SElExceptionPtr kosu_AddEndShift(
	struct _SDBSKOSU* me,
	uint8_t			hour,
	uint8_t			min
);
static SElExceptionPtr kosu_SetAlarm(
	struct _SDBSKOSU*	me,
	uint32_t		user,
	uint32_t		alarm,
	uint32_t		kosu
);
static SElExceptionPtr kosu_GetKOSU(
	struct _SDBSKOSU* me,
	uint32_t*   pPANEL_actual_good,
    uint32_t*   pPANEL_actual_bad,
    uint32_t*   pPANEL_actual_retest_good,
    uint32_t*   pPANEL_actual_retest_bad,
    uint32_t*   pPANEL_required, 
    uint32_t*   pPANEL_END_actual,
    uint32_t*   pPANEL_END_required, 
    uint32_t*   pKOSU_actual,
    uint32_t*   pKOSU_required,  
    uint32_t*   pUSER_COUNT,  
    uint32_t*   pPAUSE_TIME
);
static SElExceptionPtr kosu_GetAvgKOSU(
	struct _SDBSKOSU* 	me,
	time_t				t1,
	time_t				t2,
	uint32_t*			pAvg,
	uint32_t*			pPause 
);
static SElExceptionPtr kosu_GetShiftRange(
	struct _SDBSKOSU* me,
	time_t*   pStart,
	time_t*   pEnd
);
static SElExceptionPtr kosu_GetAlarmInInterval(
	struct _SDBSKOSU* me,
	int32_t			alarm,
	time_t			t1,
	time_t			t2,
	SIntervalResultPtr*	pInterval
);
static SElExceptionPtr kosu_GetMaxAlarm(
	struct _SDBSKOSU* me, 
	int32_t* max_alarm
);   

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ConvertTimestampToANSI"
static SElExceptionPtr ConvertTimestampToANSI(char* timestamp, time_t* pTime)
{
	SElExceptionPtr	pexception = NULL;   
	struct tm   stm;
	int			month, day, year;
	int			hours, minutes, seconds;
	
	sscanf(timestamp, "%d-%02d-%02d %02d:%02d:%02d", &year, &month, &day, &hours, &minutes, &seconds);

	memset(&stm, 0, sizeof(stm));
	stm.tm_min = minutes;
	stm.tm_hour = hours;
	stm.tm_sec  = seconds;
	stm.tm_mday = day;
	stm.tm_year = year-1900;
	stm.tm_mon  = month-1;
	stm.tm_isdst = -1;
	*pTime = mktime(&stm);

/* Error: */
	EXCRETHROW( pexception); 
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbskosu_new"
/*!
 * \brief
 * This function creates a new instance.
 * 
 * \param pError 
 * Returns a value indicating whether an error occurred.
 * A negative error code indicates function failure.	
 * 
 * \return 
 * Pointer to new instance of the KOSU object. If the function fails, NULL
 * pointer is returned
 *
 * This function creates a new instance of the KOSU object.
 */
SElExceptionPtr dbskosu_new(SDBSKOSUPtr* pKOSU, void* pDBS)
{
	SElExceptionPtr	pexception = NULL; 
	SDBSKOSUPtr		me = NULL;
	
    me = calloc(1, sizeof(SDBSKOSU));
	EXCCHECKALLOC( me);

	if(pKOSU) *pKOSU = me;
	
	InitializeCriticalSection(&(me->cs));

	me->Initialize		= kosu_Initialize;
	me->AddKOSU			= kosu_AddKosu;
	me->AddPause		= kosu_AddPause;
	me->AddEndShift		= kosu_AddEndShift;
	me->SetAlarm		= kosu_SetAlarm;
	me->GetKOSU			= kosu_GetKOSU;
	me->GetAvgKOSU		= kosu_GetAvgKOSU;
	me->GetShiftRange	= kosu_GetShiftRange;
	me->GetAlarmInInterval	= kosu_GetAlarmInInterval;
	me->GetMaxAlarm 		= kosu_GetMaxAlarm;
	me->GetRequiredKOSUAlarm = kosu_GetRequiredKOSUAlarm;

	me->pdbs = pDBS;

Error:
	EXCRETHROW( pexception);    
} /* kosu_new */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbskosu_delete"
/*!
 * \brief
 * This function destroys specified instance.
 * 
 * \param pKOSU 
 * Pointer to instance of the KOSU object cretaed by kosu_new() function.
 * 
 * \return 
 * A value indicating whether an error occurred.
 * A negative error code indicates function failure.	
 *
 * This function destroys specified instance and release all resources.
 */
SElExceptionPtr dbskosu_delete(SDBSKOSUPtr* pKOSU)
{
	if (pKOSU && *pKOSU)
	{
		SDBSKOSUPause* ppause      = (*pKOSU)->pPause;
		SDBSKOSUPause* ppause_next = NULL;
		SDBSKOSUPause* pshift      = (*pKOSU)->pShiftEnd;
		SDBSKOSUPause* pshift_next = NULL;
	    
		for ( ; ppause; ppause = ppause_next)
		{
			ppause_next = ppause->pNext;
			free( ppause);
		}
	    
		for ( ; pshift; pshift = pshift_next)
		{
			pshift_next = pshift->pNext;
			free( pshift);
		}
	    
		DeleteCriticalSection(&(*pKOSU)->cs);

		free(*pKOSU);
		*pKOSU = NULL;
	} 

/* Error: */
	return NULL;
} /* kosu_delete */

/*****************************************************************************
 * Static no-exported functions declaration **********************************
 *****************************************************************************/
#undef __FUNC__
#define __FUNC__ "kosu_Initialize"
static SElExceptionPtr kosu_Initialize(
	struct _SDBSKOSU*	me, 
	const char*			fileName
)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL;
	HRESULT         hr = 0; 
	CAObjHandle     pxml  = 0;
	CAObjHandle		pnode = 0;
	CAObjHandle		pnode2 = 0;
	CAObjHandle		pnode2old = 0;  
	CAObjHandle		pparameter = 0;
	VBOOL           is_xml_success = 1; 
	char*           ptext= NULL;
	const char*     pfile_name = (fileName) ? fileName : "\\dbs.xml";
	VARIANT         pfile_name_var = CA_VariantEmpty();
	char			path[MAX_PATHNAME_LEN ];
	uint32_t		kosu;
	int				i,j;

	EXCCHECKCVI( GetModuleDir (__CVIUserHInst, path));  
	strcat(path, pfile_name);
	
	EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml));
	EXCCHECKCVI( CA_VariantSetCString(  &pfile_name_var, path));
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (pxml, NULL, pfile_name_var, &is_xml_success));
	
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//module[@id='"MODULE_DBS"']", &pnode));   

	if(pnode)
	{
		/* Configure all allowed pauses */
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "kosu/pauses/pause", &pnode2));
		while(pnode2!=0)
		{
			int h,m,d; 
			
			EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode2, NULL, "@time", &pparameter)); 
			if(pparameter)
			{
				EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
	
				sscanf(ptext, "%d:%d", &h, &m);

				EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
				pparameter = 0;
				CA_FreeMemory(ptext);
				ptext=NULL;
				
				EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode2, NULL, "@length", &pparameter));
				if(pparameter)
				{
					EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
	
					d = strtol(ptext, NULL, 10);
					
					if ( h < 0 || m < 0 || d < 0 || h > 23 || m > 59 || d > 24*60 )
					{
						;/* One pause ignored. Incorrect time format or delay is greater as 24 hours. */
					}
					else
					{
						EXCCHECK(me->AddPause(me, h, m, d));
					}

					EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
					pparameter = 0;
					CA_FreeMemory(ptext);
					ptext=NULL;
				}
			}
			
			pnode2old = pnode2;
			MSXML_IXMLDOMNodeGetnextSibling (pnode2, NULL, &pnode2); 
			EXCCHECKCVI( CA_DiscardObjHandle(pnode2old));
			pnode2old = 0;   
		}
		
		/* Configure end of shift */
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "kosu/shifts/end", &pnode2));
		while(pnode2!=0)
		{
			int h,m;   
			
			EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode2, NULL, "@time", &pparameter));  
			if(pparameter)
			{
				EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
	
				sscanf(ptext, "%d:%d", &h, &m);

				if ( h < 0 || m < 0 || h > 23 || m > 59 )
				{
					;/* End of shift ignored. Incorrect time format. */
				}
				else
				{
					EXCCHECK(me->AddEndShift(me, h, m));
				}
				
				EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
				pparameter = 0;
				CA_FreeMemory(ptext);
				ptext=NULL;	
			}
			
			pnode2old = pnode2;
			MSXML_IXMLDOMNodeGetnextSibling (pnode2, NULL, &pnode2); 
			EXCCHECKCVI( CA_DiscardObjHandle(pnode2old));
			pnode2old = 0;   
		}
		
		/* Read KOSU settings for all allowed operators */
		for(i=1; i<KOSU_MAX_USERS; i++)
		{
			sprintf( path, "kosu/operators/operator[@count='%d']", i);
			EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pnode2));
			if(pnode2)
			{
				EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode2, NULL, "kosu/@time", &pparameter));  
				if(pparameter)
				{
					EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
					
					kosu = strtol(ptext, NULL, 10);
					
					EXCCHECK( me->AddKOSU(me, i,  kosu));   
				
					EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
					pparameter = 0;
					CA_FreeMemory(ptext);
					ptext=NULL;	
					
					for(j=1; j<=KOSU_MAX_ALARMS; j++)
					{
						sprintf(path, "alarm%d/@time", j);
						EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode2, NULL, path, &pparameter));
						if(pparameter)
						{
							EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
					
							kosu = strtol(ptext, NULL, 10);
							
							EXCCHECK(me->SetAlarm(me, i, j, kosu));
							
							EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
							pparameter = 0;
							CA_FreeMemory(ptext);
							ptext=NULL;	
						}
					}
				}
			
				pnode2old = pnode2;
				MSXML_IXMLDOMNodeGetnextSibling (pnode2, NULL, &pnode2); 
				EXCCHECKCVI( CA_DiscardObjHandle(pnode2old));
				pnode2old = 0;
			}
		}
	}

Error:
	CA_FreeMemory(ptext);    
	if (pxml) CA_DiscardObjHandle(pxml);
	if (pnode) CA_DiscardObjHandle(pnode);
	if (pnode2) CA_DiscardObjHandle(pnode2);
	if (pnode2old) CA_DiscardObjHandle(pnode2old);
	if (pparameter) CA_DiscardObjHandle(pparameter);
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "kosu_AddKOSU"
/*!
 * \brief
 * This function adds KOSU configuration
 * 
 * \param pKOSU 
 * Pointer to instance of the KOSU object cretaed by kosu_new() function.
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
 *  KOSU_ERROR_MAX_USER_COUNT_EXCEED - user is out of range
 *                                     See KOSU_MAX_USERS for limit.
 *
 * This function adds KOSU informations. If the entry for specified
 * user count exist, this entry will be updated.
 */
static SElExceptionPtr kosu_AddKosu(
	struct _SDBSKOSU*	me,
	uint8_t				UserCount,
	uint32_t			KOSU
)
{
	SElExceptionPtr	pexception = NULL;   
	int32_t			error = 0;
	
	EXCCHECK( kosu_Lock(me));
	
	if ( UserCount > KOSU_MAX_USERS )
		CHECKERR( DBS_KOSU_ERROR_MAX_USER_COUNT_EXCEED);

	me->KOSUSettings[UserCount].UserCount = UserCount; 
	me->KOSUSettings[UserCount].KOSU      = KOSU; 
  
Error:
	kosu_Unlock(me);
	EXCRETHROW( pexception); 
} /* kosuAddKosu */ 
 
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "kosu_AddPause"
/*!
 * \brief
 * This function adds scheduled pause into KOSU calculator.
 * 
 * \param pKOSU 
 * Pointer to instance of the KOSU object cretaed by kosu_new() function.
 * 
 * \param hour
 * Specifies begin of the scheduled pause, which begin at hour:min every day.
 * 
 * \param min
 * Specifies begin of the scheduled pause, which begin at hour:min every day.
 * 
 * \param delay
 * Specifies the time of the specified pause in minutes, whitch begin at
 * hour:min every day.
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
 *  EL_ERROR_LOW_MEMORY - low memory
 * 
 * This function adds scheduled pause into KOSU calculator. Scheduled pause
 * means breaks like lunch, breakfast, cofee break etc.
 */
static SElExceptionPtr kosu_AddPause(
	struct _SDBSKOSU*	me,
	uint8_t				hour,
	uint8_t				min,
	uint16_t			delay
)
{
	SElExceptionPtr	pexception = NULL;   
	SDBSKOSUPausePtr	ppause = NULL;
	SDBSKOSUPausePtr	pnew_pause = NULL;
	
	EXCCHECK( kosu_Lock(me));

	ppause = me->pPause;

	if (ppause!= NULL)
	{
		for( ; ppause->pNext != NULL; ppause = ppause->pNext)
			; /* Do nothing */
	}
	
	pnew_pause = (SDBSKOSUPausePtr)calloc(1, sizeof(SDBSKOSUPause));
	EXCCHECKALLOC(pnew_pause);

	pnew_pause->hour  = hour;
	pnew_pause->min   = min;
	pnew_pause->delay = delay;
	
	if ( ppause != NULL )
		ppause->pNext = pnew_pause;
	else
		me->pPause = pnew_pause;	

Error:
	kosu_Unlock(me);
	EXCRETHROW( pexception); 
} /* kosuAddPause */ 

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "kosu_AddEndShift"
/*!
 * \brief
 * This function .
 * 
 * \param pKOSU 
 * Pointer to instance of the KOSU object cretaed by kosu_new() function.
 * 
 * \param hour
 * Specifies begin of the scheduled pause, which begin at hour:min every day.
 * 
 * \param min
 * Specifies begin of the scheduled pause, which begin at hour:min every day.
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
 *  EL_ERROR_LOW_MEMORY - Low memory
 * 
 * This function adds scheduled pause into KOSU calculator. Scheduled pause
 * means breaks like lunch, breakfast, cofee break etc.
 */
static SElExceptionPtr kosu_AddEndShift(
	struct _SDBSKOSU*	me,
	uint8_t				hour,
	uint8_t				min
)
{
	SElExceptionPtr	pexception = NULL;       
	SDBSKOSUPausePtr	pshiftend = NULL;
	SDBSKOSUPausePtr	pnew_end = NULL;
	
	EXCCHECK(kosu_Lock(me));

	pshiftend = me->pShiftEnd;

	if (pshiftend!=NULL)
	{
		for(;pshiftend->pNext!=NULL;pshiftend=pshiftend->pNext)
			; /* Do nothing */
	}
	
	pnew_end = (SDBSKOSUPausePtr)calloc(1, sizeof(SDBSKOSUPause));
	EXCCHECKALLOC(pnew_end);

	pnew_end->hour  = hour;
	pnew_end->min   = min;

	if (pshiftend!=NULL)
		pshiftend->pNext = pnew_end;
	else
		me->pShiftEnd = pnew_end;

Error:
	kosu_Unlock(me);
	EXCRETHROW( pexception); 
} /* kosuAddEndShift */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "kosu_SetAlarm"
/*!
 * \brief
 * This function adds new alarm for specified number o users.
 * 
 * \param pKOSU
 * Pointer to instance of the KOSU object cretaed by kosu_new() function.
 *
 * \param user
 * Specifies number or logged operators for this alarm
 *
 * \param alarm
 * Specifies level of the alarm.
 *
 * \param kosu
 * Specifies kosu value for specified alarm.
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
 *  KOSU_ERROR_MAX_USER_COUNT_EXCEED - user is out of range
 *  KOSU_ERROR_MAX_ALARM_COUNT_EXCEED - alarm is out of range
 *
 * Function defines alarm for specified number of operators. Alarms means
 * max allowed avarage of KOSU. If this value is exceed, system shoul display it
 * in reprot.
 */
static SElExceptionPtr kosu_SetAlarm(
	struct _SDBSKOSU*	me,
	uint32_t			user,
	uint32_t			alarm,
	uint32_t			kosu
)
{
	SElExceptionPtr	pexception = NULL;       
	
	EXCCHECK(kosu_Lock(me));

	if (user<1 || user>KOSU_MAX_USERS)
		EXCTHROW(DBS_KOSU_ERROR_MAX_USER_COUNT_EXCEED, "DBS_KOSU_ERROR_MAX_USER_COUNT_EXCEED");

	if (alarm<1 || alarm>KOSU_MAX_ALARMS)
		EXCTHROW(DBS_KOSU_ERROR_MAX_ALARM_COUNT_EXCEED, "DBS_KOSU_ERROR_MAX_ALARM_COUNT_EXCEED");

	me->KOSUAlarms[user].alarms[alarm] = kosu;

Error:
	kosu_Unlock(me);
	EXCRETHROW( pexception); 
} /* KOSUSetAlarm */
 
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "kosu_GetKOSU"
/*!
 * \brief
 * This function sets storage, which will produce data for statistics
 * 
 * \param pKOSU
 * Pointer to instance of the KOSU object cretaed by kosu_new() function.
 *
 * \param pUserCount
 * Returns current login users. You can pass NULL pointer for this value.
 *  
 * \param pRequiredKOSU
 * Returns KOSU for current nb of usres. You can pass NULL pointer
 * for this value.
 * 
 * \param pAktualRequiredProduct
 * Returns number of product required at actual time.
 * You can pass NULL pointer for this value. 
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
 *  KOSU_ERROR_KOSU_NOT_AVAILABLE - Kosu is not specified for this number of operators.
 *
 * Function defines storage interface, which will produce data for statistics.
 */
static SElExceptionPtr kosu_GetKOSU(
  struct _SDBSKOSU*	me,
  uint32_t*   pPANEL_actual_good,
  uint32_t*   pPANEL_actual_bad,
  uint32_t*   pPANEL_actual_retest_good,
  uint32_t*   pPANEL_actual_retest_bad,
  uint32_t*   pPANEL_required, 
  uint32_t*   pPANEL_END_actual,
  uint32_t*   pPANEL_END_required, 
  uint32_t*   pKOSU_actual,
  uint32_t*   pKOSU_required,  
  uint32_t*   pUSER_COUNT,  
  uint32_t*   pPAUSE_TIME
)
{
	SElExceptionPtr	pexception = NULL;  
	uint32_t	users = 0;
	uint32_t 	rKOSU = 0;
	uint32_t 	cKOSU = 0;
	double 		aKOSU = 0;
	uint32_t	cProduct = 0;
	uint32_t	cProductBad = 0;   
	uint32_t	cRProduct = 0;
	uint32_t	cRProductBad = 0;
	double		rProduct = 0;
	uint32_t	cEndProduct = 0;
	double		rEndProduct = 0;
	uint32_t    intervalTime = 0;
	uint32_t    downTime = 0;
	uint32_t    pauseTime = 0; 
	time_t		shift_start,
				shift_stop,
				t1, t2;
	SYSTEMTIME	st;	
	char		datetime_start[DBS_RECORD_LENGHT_TIME+1];  
	char		datetime_stop[DBS_RECORD_LENGHT_TIME+1];
	
	SIntervalResultPtr	pDownTimes     = NULL;
	SIntervalResultPtr	pPauseTimes    = NULL;
	SIntervalResultPtr	pPauses        = NULL;
	SIntervalResultPtr	pWorkIntervals = NULL;
	SUserIntervalsPtr	pUserIntervalsResult = NULL;
	SStatusItem			StatusItem;

	EXCCHECK( kosu_Lock(me));

	/*
	 * Calculate Required Panels
	 * a) Get All downtime Intervals
	 * b) Get All Pause Intervals
	 * c) Merge Intervals
	 * d) split time to intervals
	 * e) Get Users in interval
	 * f) Calculate KOSU
	 */

#if 0    /* formerly excluded lines */
	EXCCHECK( PSTORAGE->GetSelectedShift(PSTORAGE, 0, &StatusItem) );
	shift_start = StatusItem.time;
	shift_stop = StatusItem.time_end; /* if opened interval shift_stop is a actual time; */
	
	/* get pause intervals from storage */
	EXCCHECK( PSTORAGE->GetPauseIntervals(PSTORAGE, shift_start, shift_stop, &pDownTimes, &downTime));
	
	/* get planned pause intevals */
	EXCCHECK( kosu_GetPauseIntervals(me, shift_start, shift_stop, &pPauseTimes, &pauseTime));
	
	EXCCHECK( MergeIntervals(pDownTimes, pPauseTimes, &pPauses, &intervalTime));
	EXCCHECK( InverseIntervals(shift_start, shift_stop, pPauses, &pWorkIntervals, &intervalTime));
	
	EXCCHECK( kosu_GetUsersTimeInIntervals(me, pWorkIntervals, &pUserIntervalsResult));
	EXCCHECK( kosu_GetRequiredPanels(me, pUserIntervalsResult, &rProduct));

	EXCCHECK( PSTORAGE->GetOperatorCount(PSTORAGE, &users));
	EXCCHECK( kosu_GetRequiredKOSU(me, users, &rKOSU));
	
	pexception = PSTORAGE->GetLastTwoProducts(PSTORAGE, shift_start, &t1, &t2); 
	if ( pexception == NULL)
	{
		cKOSU = (int32_t)(t1 - t2);
	}
	else if (pexception->GetErrorCode(pexception) == DBS_STORAGE_ERROR_NOT_PRODUCT_DEFINED)
	{
		cKOSU = 0;
		EXCDELETE( pexception);
	}
	else
	{
		EXCCHECK( pexception);
	}
#endif   /* formerly excluded lines */

	
	/* GetDate today*/
	GetLocalTime(&st);
		
	/* today from 00:00:00 */
	sprintf (datetime_start, "%d-%02d-%02d 00:00:00", st.wYear, st.wMonth, st.wDay);
	//sprintf (datetime_start, "%d-%02d-%02d 00:00:00", st.wYear, st.wMonth, 4);
	EXCCHECK( ConvertTimestampToANSI(datetime_start , &shift_start));
	
	/* today to 23:59:59 */
	sprintf (datetime_stop, "%d-%02d-%02d 23:59:59", st.wYear , st.wMonth, st.wDay);
	//sprintf (datetime_stop, "%d-%02d-%02d 23:59:59", st.wYear , st.wMonth, 4);
	EXCCHECK( ConvertTimestampToANSI(datetime_stop , &shift_stop));
	
	EXCCHECK(PSTORAGE->GetProductCount(PSTORAGE, &shift_start, &shift_stop, FALSE, ID_MODE_PRODUCTION, &cProduct));
	EXCCHECK(PSTORAGE->GetProductCount(PSTORAGE, &shift_start, &shift_stop, TRUE, ID_MODE_PRODUCTION, &cProductBad));   
	
	
#if 0    /* formerly excluded lines */
	EXCCHECK(PSTORAGE->GetRetestedProductCount(PSTORAGE, &shift_start, &shift_stop, FALSE, ID_MODE_QUALITY_RETEST, &cRProduct));   
	EXCCHECK(PSTORAGE->GetRetestedProductCount(PSTORAGE, &shift_start, &shift_stop, TRUE, ID_MODE_QUALITY_RETEST, &cRProductBad));   

	if(StatusItem.opened)
	{
		time_t		shift_end_scheduled;
		int32_t		time_pause, time_diff;
			
		EXCCHECK( kosu_GetEndShiftTime(me, shift_stop, &shift_end_scheduled));
		EXCCHECK( kosu_GetPauseIntervals(me, shift_stop, shift_end_scheduled, NULL, &time_pause));
		
		time_diff  = (int32_t)(shift_end_scheduled - shift_stop) - time_pause;
		
		/* one hour avarage kosu */
		EXCCHECK( PSTORAGE->GetAvgKOSU( PSTORAGE, 
									   	(shift_start > (shift_stop-3600))? shift_start : (shift_stop-3600), 
										shift_stop, 
										&aKOSU,
										NULL) );
	
		if (rKOSU>0)
			rEndProduct = rProduct + (double)time_diff/(double)rKOSU;
		else
			rEndProduct = rProduct;
		
		if (aKOSU>0)
			cEndProduct = cProduct + (int)floor( (double)time_diff/aKOSU ); //cKOSU
		else
			cEndProduct = cProduct;	
	}
	else
	{
		cEndProduct = cProduct;  
		rEndProduct = rProduct;
	}
#endif   /* formerly excluded lines */

Error:
	intervalresult_delete( &pDownTimes);
	intervalresult_delete( &pPauseTimes);
	intervalresult_delete( &pPauses);
	intervalresult_delete( &pWorkIntervals);
	userintervals_delete( &pUserIntervalsResult);

	if (pPANEL_actual_good) 		*pPANEL_actual_good = cProduct;
	if (pPANEL_actual_bad) 			*pPANEL_actual_bad = cProductBad;
	if (pPANEL_actual_retest_good) 	*pPANEL_actual_retest_good = cRProduct;
	if (pPANEL_actual_retest_bad) 	*pPANEL_actual_retest_bad = cRProductBad;
	if (pPANEL_required) 			*pPANEL_required = (uint32_t)floor(rProduct);
	if (pPANEL_END_actual) 			*pPANEL_END_actual = cEndProduct;
	if (pPANEL_END_required) 		*pPANEL_END_required = (uint32_t)floor(rEndProduct);
	if (pKOSU_actual) 				*pKOSU_actual = cKOSU;
	if (pKOSU_required) 			*pKOSU_required = rKOSU;
	if (pUSER_COUNT) 				*pUSER_COUNT = users;
	if (pPAUSE_TIME) 				*pPAUSE_TIME = downTime;
  
	kosu_Unlock(me);
	EXCRETHROW( pexception); 
} /* kosuGetKOSU */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "kosu_GetAvgKOSU"
static SElExceptionPtr kosu_GetAvgKOSU(
	struct _SDBSKOSU*	me,
	time_t				t1,
	time_t				t2,
	uint32_t*			pAvg,
	uint32_t*			pPause
)
{
	SElExceptionPtr	pexception = NULL;  
	double			avarage;

	EXCCHECK( kosu_Lock(me));

	EXCCHECK( PSTORAGE->GetAvgKOSU(PSTORAGE, t1, t2, &avarage, pPause));
	
	if(pAvg && avarage==0.0) 
		*pAvg = (uint32_t)(t2-t1);
	else
		*pAvg = (uint32_t)floor(avarage);

Error:
	kosu_Unlock(me);
	EXCRETHROW( pexception); 
} /* kosuGetAvgKOSU* /

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "kosu_GetShiftRange"
/*!
 * \brief
 * This function returns start time and stop time of the sfift, If the shift
 * has not been terminated, the scheduled end of the shift will be returned.
 * 
 * \param pKOSU
 * Pointer to instance of the KOSU object cretaed by kosu_new() function.
 *
 * \param pStart
 * Returns start of the last shift
 *
 * \param pEnd
 * Returns end of the last shift or scheduled end of the shift
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
 *  None
 */
static SElExceptionPtr kosu_GetShiftRange(
  struct _SDBSKOSU* me,
  time_t*			pStart,
  time_t*			pEnd
)
{
	SElExceptionPtr	pexception = NULL;  
	SStatusItem		StatusItem;
	
	EXCCHECK(kosu_Lock(me));

	EXCCHECK(PSTORAGE->GetSelectedShift(PSTORAGE, *pStart, &StatusItem) );

	if( StatusItem.terminated)
		StatusItem.time_end = StatusItem.time;		
		
	if( StatusItem.opened)
		EXCCHECK( kosu_GetEndShiftTime(me, StatusItem.time_end, &StatusItem.time_end));

Error:
	if (pStart) *pStart = StatusItem.time;
	if (pEnd) *pEnd 	= StatusItem.time_end;
	kosu_Unlock(me);
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "kosu_GetAlarmInInterval"
static SElExceptionPtr kosu_GetAlarmInInterval(
	struct _SDBSKOSU*	me,
	int32_t				alarm,
	time_t				t1,
	time_t				t2,
	SIntervalResultPtr*	pInterval
)
{
	SElExceptionPtr	pexception = NULL;  
	SIntervalResultPtr	pUserIntervals = NULL; 
	time_t			ts, new_ts, tp;
	uint32_t		user_count, new_user_count = 0;
	uint32_t		rKOSU = 0;

	EXCCHECK( kosu_Lock(me));
	
	if( pInterval==NULL)
		EXCCHECK( intervalresult_new( pInterval));

	/*
	 * Get users loged in interval
	 */
	EXCCHECK( PSTORAGE->GetOperatorIntervals(PSTORAGE, t1, t2, &pUserIntervals));
	
	if( pUserIntervals->Count(pUserIntervals)>0 )
	{
		ts = (pUserIntervals->intervals)->start;
		tp = (pUserIntervals->intervals)->stop;
	}
	else
		ts = t1; 
	
	user_count = GetNbUserAtTime( pUserIntervals, ts);
	
	while ( ts < t2 )
	{
		GetNextUserChange(pUserIntervals, ts, &new_ts, &new_user_count);
		if ( new_ts == 0 || new_ts == ts )
			break;
	
		EXCCHECK( kosu_GetRequiredKOSUAlarm(me, alarm, user_count, &rKOSU));
		if ( rKOSU > 0 )
		{
			EXCCHECK( (*pInterval)->AddEx( *pInterval, ts, new_ts, rKOSU));
		}

		ts		   = new_ts;
		user_count = new_user_count;
	} /* while ( ts < t2 ) */
	
	if ( ts < t2)
	{
		int32_t	count = (*pInterval)->Count(*pInterval);
		user_count = GetNbUserAtTime( pUserIntervals, ts-1);
		
		if ( count > 0 && user_count > 0)
		{
			((*pInterval)->intervals+(count-1))->stop = t2;
		}
	}

Error:
	intervalresult_delete( &pUserIntervals);
	kosu_Unlock(me);
	EXCRETHROW( pexception); 
} /* kosuGetAlarmInInterval */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "kosu_GetMaxAlarm"

static SElExceptionPtr kosu_GetMaxAlarm(
	struct _SDBSKOSU* me, 
	int32_t* max_alarm
)
{
	SElExceptionPtr	pexception = NULL;  
	int				i, j;
	int32_t			max = 0;
	
	for (i=0; i<KOSU_MAX_USERS+1; i++)
	{
		for (j=0; j<KOSU_MAX_ALARMS+1; j++)
		{
			if ( (int32_t)me->KOSUAlarms[i].alarms[j] > max)
				max = me->KOSUAlarms[i].alarms[j];
		}
	} 	
	
	if(max_alarm) *max_alarm = max;
	
/* Error: */
	EXCRETHROW( pexception); 
}

/*****************************************************************************
 * Static no-exported functions declaration **********************************
 *****************************************************************************/
#undef __FUNC__
#define __FUNC__ "kosu_Lock"
/**
 * This function is used to obtain a lock on the specified resource.
 *
 * \param	pKOSU	Pointer to resource which should be locked
 *
 * \return	A value indicating whether an error occurred.
 *          A negative error code indicates function failure.		
 */
static SElExceptionPtr kosu_Lock(struct _SDBSKOSU* me)
{
	SElExceptionPtr	pexception = NULL;  

	EXCCHECKALLOC(me); /* TODO: change it */

	EnterCriticalSection(&(me->cs));

Error:	
	EXCRETHROW( pexception); 
} /* kosu_Lock */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "kosu_Unlock"
/**
 * Relinquishes a lock to the resource associated with the given session.
 *
 * \param	pKOSU	Pointer to resource which should be unlocked
 *
 * \return	A value indicating whether an error occurred.
 *          A negative error code indicates function failure.		
 *
 * \warning Call this function only when the resource is locked.
 *          See kosu_Lock().
 */
static SElExceptionPtr kosu_Unlock(struct _SDBSKOSU* me)
{
	SElExceptionPtr	pexception = NULL; 

	EXCCHECKALLOC(me); /* TODO: change it */

	LeaveCriticalSection(&(me->cs));

Error:	
	EXCRETHROW( pexception); 
} /* kosu_Unlock */


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "kosu_GetEndShiftTime"
/*!
 * \brief
 * This function returns stop time of the shift, If the shift
 * has not been terminated, the scheduled end of the shift will be returned.
 * 
 * \param pKOSU
 * Pointer to instance of the KOSU object cretaed by kosu_new() function.
 *
 * \param begin
 * Specifies time in shift interval.
 *
 * \param pEndTime
 * Returns the scheduled end of the shift.
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
 *  None
 */
static SElExceptionPtr	kosu_GetEndShiftTime(
	struct _SDBSKOSU*	me,
	time_t			begin,
	time_t*			pEndTime
)
{
	SElExceptionPtr	pexception = NULL; 
	int32_t			error = 0;
	SDBSKOSUPausePtr	pshift_end = NULL;
	struct tm*		lt = localtime(&begin);
	int32_t			h, new_h;
	int32_t			m, new_m;
	time_t			end_time = begin;
	bool_t			is_next_day = FALSE;

	if ( lt == NULL )
		CHECKERR(-1);

	h = lt->tm_hour;
	m = lt->tm_min;
	new_h = -1;
	new_m = -1;

	pshift_end = me->pShiftEnd;
	for( ; pshift_end != NULL; pshift_end = pshift_end->pNext)
	{
		if ( pshift_end->hour > h )
		{
			if ( IsNewTime( new_h, new_m, pshift_end->hour, pshift_end->min) )
			{
				new_h = pshift_end->hour;
				new_m = pshift_end->min;
			}
		}
		else
		if ( pshift_end->hour == h && pshift_end->min > m)
		{
			if ( IsNewTime( new_h, new_m, pshift_end->hour, pshift_end->min) )
			{
				new_h = pshift_end->hour;
				new_m = pshift_end->min;
			}
		}
	} /* for( ; pshift_end != NULL; pshift_end = pshift_end->pNext) */
	
	if ( new_h < 0 )
	{
		/*
		 * We did not find end of shoft in current day, end of shift is
		 * the first time in seconda day
		 */
		 pshift_end = me->pShiftEnd;
		 for( ; pshift_end != NULL; pshift_end = pshift_end->pNext)
		 {
			if ( IsNewTime( new_h, new_m, pshift_end->hour, pshift_end->min) )
			{
				new_h = pshift_end->hour;
				new_m = pshift_end->min;
			}
		 }
		 is_next_day = TRUE;
	}

	if ( new_h >= 0 )
	{
		time_t		atime = time(NULL);
		struct tm*	ptm = NULL;
		struct tm	ntime;

		ptm = localtime( &atime);
		
		memcpy( &ntime, ptm, sizeof(struct tm)); 
		ntime.tm_sec   = 0;
		ntime.tm_isdst = -1;
		ntime.tm_hour  = new_h;
		ntime.tm_min   = new_m;
		if ( is_next_day )
		{
			ntime.tm_mday  += 1;
		}
		end_time = mktime( &ntime); 
	}

Error:
	*pEndTime =  end_time; 
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "kosu_GetScheduledPauses"
/*!
 * \brief
 * This function returns all scheduled pauses in specified time interval
 * 
 * \param pKOSU
 * Pointer to instance of the KOSU object cretaed by kosu_new() function.
 *
 * \param t1
 * Specifies start time of the interval
 *
 * \param t2
 * Specifies stop time of the interval
 *
 * \param pPouse
 * Returns sum of all pauses in the interval (seconds)
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
 *  None
 */
static SElExceptionPtr	kosu_GetPauseIntervals(
	struct _SDBSKOSU*	me,
	time_t				t1,
	time_t				t2,
	SIntervalResultPtr*	pResult,
	int32_t*			pPause
)
{
	SElExceptionPtr	pexception = NULL; 
	int32_t			error = 0;
	uint32_t		pause = 0;
	time_t			start;
	time_t			stop;
	struct tm*		time_tm;
	SDBSKOSUPausePtr	ppause;

	if(pPause) *pPause = 0;

	if(pResult)
	{
		if (*pResult!=NULL )
			intervalresult_delete( pResult);

		EXCCHECK( intervalresult_new( pResult));
	}
   
	for(ppause = me->pPause; ppause; ppause = ppause->pNext)
	{
		/* day before */
		time(&start);
		start -= 24*60*60;
		time_tm = localtime(&start);
		time_tm->tm_sec  = 0;
		time_tm->tm_hour = ppause->hour;
		time_tm->tm_min  = ppause->min;
		time_tm->tm_isdst = -1;
		start = mktime(time_tm);
		stop = start + (ppause->delay*60); /* delay in minutes */

		if(start == (time_t)-1 || stop == (time_t)-1 )
			CHECKERR(-1);

		if( (start > t1 && start < t2) || (stop > t1 && stop < t2) )						 
		{
			if(pResult) (*pResult)->Add(*pResult, (start>t1)? start : t1 , (stop<t2)? stop : t2 );
			
			pause += (uint32_t)( ((stop<t2)? stop : t2) - ((start>t1)? start : t1) );
		}
	} 
	
	for(ppause = me->pPause; ppause; ppause = ppause->pNext)
	{
		/* actual day */
		time(&start);
		time_tm = localtime(&start);
		time_tm->tm_sec  = 0;
		time_tm->tm_hour = ppause->hour;
		time_tm->tm_min  = ppause->min;
		time_tm->tm_isdst = -1;
		start = mktime(time_tm);
		stop = start + (ppause->delay*60); /* delay in minutes */

		if (start == (time_t)-1 || stop == (time_t)-1 )
			CHECKERR(-1);

		if( (start > t1 && start < t2) || (stop > t1 && stop < t2) )						 
		{
			if(pResult) (*pResult)->Add(*pResult, (start>t1)? start : t1 , (stop<t2)? stop : t2 );
			
			pause += (uint32_t)( ((stop<t2)? stop : t2) - ((start>t1)? start : t1) );
		}
	} 
	
	for(ppause = me->pPause; ppause; ppause = ppause->pNext)
	{
		/* next day */
		time(&start);
		start += 24*60*60;
		time_tm = localtime(&start);
		time_tm->tm_sec  = 0;
		time_tm->tm_hour = ppause->hour;
		time_tm->tm_min  = ppause->min;
		time_tm->tm_isdst = -1;
		start = mktime(time_tm);
		stop = start + (ppause->delay*60); /* delay in minutes */

		if (start == (time_t)-1 || stop == (time_t)-1 )
			CHECKERR(-1);

		if( (start > t1 && start < t2) || (stop > t1 && stop < t2) )						 
		{
			if(pResult) (*pResult)->Add(*pResult, (start>t1)? start : t1 , (stop<t2)? stop : t2 );
			
			pause += (uint32_t)( ((stop<t2)? stop : t2) - ((start>t1)? start : t1) );
		}
	} 
	
	if(pPause) *pPause = (int32_t)pause;

Error:
	EXCRETHROW( pexception); 
} /* kosu_GetScheduledPauses */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "IsNewTime"
/*!
 * This function detects if the new time defined by configh and config_m is
 * time, which should be stored into variables actual_h, actual_m
 *
 * \param actual_h
 * Actual stored time (hours), if not initialized -1
 *
 * \param actual_m
 * Actual stored time (minutes), if not initialized -1
 *
 * \param config_h
 * time from configuration (hours), if not initialized -1
 *
 * \param config_m
 * time from stored time (minutes), if not initialized -1
 *
 * \return TRUE if the time should be stored, otherwise FALSE.
 *
 * Rules for compare.
 * a) actual time_h is not initialized (-1) => TRUE
 * b) config time is less as actual time => TRUE
 * c) all other combination => FALSE
 */
static bool_t IsNewTime(
	int32_t		actual_h,
	int32_t		actual_m,
	int32_t		config_h,
	int32_t		config_m
)
{
	if (actual_h<0)
		return TRUE;
	
	if (actual_h>config_h)
		return TRUE;
	
	if (actual_h==config_h && actual_m>config_h)
		return TRUE;

	return FALSE;
} /* IsNewTime */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "GetNbUserAtTime"
static uint32_t GetNbUserAtTime( 
	SIntervalResultPtr pUserIntervals,
	time_t			   ts
)
{
	uint32_t	count = 0;	
	uint32_t	i, user_count = pUserIntervals->Count(pUserIntervals);
	
	for( i = 0; i < user_count; i++)
	{
		if ((ts >= (pUserIntervals->intervals+i)->start) &&
			(ts < (pUserIntervals->intervals+i)->stop))
		{
			count++;
		}
	}
	
	return count;
} /* GetNbUserAtTime */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "GetNextUserChange"
static SElExceptionPtr GetNextUserChange(
	SIntervalResultPtr pUserIntervals,
	time_t		TS,
	time_t*		pNewTS,
	uint32_t	*pUserCount
)
{
	SElExceptionPtr	pexception = NULL; 
	uint32_t		i, user_count = pUserIntervals->Count(pUserIntervals);
	time_t			new_TS = 0;

	*pUserCount = 0;
	for( i = 0; i < user_count; i++)
	{
		if ( (pUserIntervals->intervals+i)->start > TS )
		{
			if ( (pUserIntervals->intervals+i)->start < new_TS ||
				 new_TS == 0 )
			{
				new_TS = (pUserIntervals->intervals+i)->start;
			}
		}
		
		if ( (pUserIntervals->intervals+i)->stop > TS )
		{
			if ( (pUserIntervals->intervals+i)->stop < new_TS ||
				 new_TS == 0 )
			{
				new_TS = (pUserIntervals->intervals+i)->stop;
			}
		}
	}
	if ( new_TS != 0 )
		*pUserCount = GetNbUserAtTime( pUserIntervals, new_TS);

	*pNewTS = new_TS;

/* Error: */
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "kosu_GetUsersTimeInIntervals"
static SElExceptionPtr kosu_GetUsersTimeInIntervals(
	struct _SDBSKOSU*	me,
	SIntervalResultPtr	pWorkIntervals,
	SUserIntervalsPtr*	pUserIntervalsResult
)
{
	SElExceptionPtr	pexception = NULL; 
	uint32_t		i = 0;
	uint32_t		user_count     = 0;
	uint32_t		new_user_count = 0;
	int32_t			delta_time = 0;
	time_t			ts, new_ts;
	uint32_t		count;
	SIntervalResultPtr	pUserIntervals = NULL; 
	
	userintervals_delete(pUserIntervalsResult);
	
	EXCCHECK( userintervals_new( pUserIntervalsResult));

	count = pWorkIntervals->Count(pWorkIntervals);
	
	for (i=0; i<count; i++)
	{
		EXCCHECK( intervalresult_delete( &pUserIntervals));
		/*
		 * Get users loged in interval
		 */
		EXCCHECK( PSTORAGE->GetOperatorIntervals(PSTORAGE, 
						  (pWorkIntervals->intervals+i)->start,
						  (pWorkIntervals->intervals+i)->stop,
						  &pUserIntervals
						  ));

		/*
		 * Parse interval
		 */
		ts = (pWorkIntervals->intervals+i)->start;
		user_count = GetNbUserAtTime( pUserIntervals, ts);

		while ( ts < (pWorkIntervals->intervals+i)->stop )
		{
			GetNextUserChange(pUserIntervals, ts, &new_ts, &new_user_count);
			if ( new_ts == 0 || new_ts == ts )
				break;
			
			delta_time = (int32_t)(new_ts - ts);
			
			EXCCHECK( (*pUserIntervalsResult)->Add( *pUserIntervalsResult,
												 user_count,
												 delta_time));
			ts		   = new_ts;
			user_count = new_user_count;
		}
	} 

Error:
	intervalresult_delete(&pUserIntervals);
	EXCRETHROW( pexception); 
} /* kosu_GetUsersTimeInIntervals */


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "kosu_GetRequiredPanels"
static SElExceptionPtr kosu_GetRequiredPanels(
	struct _SDBSKOSU*	me,
	SUserIntervalsPtr	pUserIntervalsResult,
	double*				pPanels
)
{
	SElExceptionPtr	pexception = NULL; 
	uint32_t		i, count = pUserIntervalsResult->Count(pUserIntervalsResult);
	uint32_t		KOSU;
	double			panels = 0.0;

	for( i = 1; i <= count; i++)
	{
		if ((pUserIntervalsResult->intervals+i)->UserCount > 0)
		{
			EXCCHECK( kosu_GetRequiredKOSU( me,	
							(pUserIntervalsResult->intervals+i)->UserCount,
							&KOSU));
			if ( KOSU > 0 )
				panels += (double)((pUserIntervalsResult->intervals+i)->TimeDelay) / (double)KOSU;
		}
	} 
	
Error:
	if(pPanels) *pPanels = panels;
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "kosu_GetRequiredKOSU"
static SElExceptionPtr kosu_GetRequiredKOSU(
	struct _SDBSKOSU*	me,
	uint32_t			UserCount,
	uint32_t*			pKOSUValue
)
{
	SElExceptionPtr	pexception = NULL; 
	uint32_t		KOSU = 0;
	int32_t			i = (int32_t)UserCount;

	i = ( i > KOSU_MAX_USERS) ? KOSU_MAX_USERS : i;

	for ( ; i >= 0; i--)
	{
		if ( me->KOSUSettings[i].UserCount > 0 &&
			 me->KOSUSettings[i].UserCount <= UserCount )
		{
			KOSU = me->KOSUSettings[i].KOSU;
			break;
		} 
	} 

/* Error: */
	if(pKOSUValue) *pKOSUValue = KOSU;
	EXCRETHROW( pexception); 
} /* kosu_GetRequiredKOSU */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "kosu_GetRequiredKOSUAlarm"
static SElExceptionPtr kosu_GetRequiredKOSUAlarm(
	struct _SDBSKOSU*	me,
	uint32_t			alarm,
	uint32_t			UserCount,
	uint32_t*			pKOSUValue
)
{
	SElExceptionPtr	pexception = NULL; 
	uint32_t		KOSU = 0;
	int32_t			i = (int32_t)UserCount;
	uint32_t		alarm_index = alarm;

	i = ( i > KOSU_MAX_USERS) ? KOSU_MAX_USERS : i;
	alarm_index = ( i > KOSU_MAX_ALARMS) ? KOSU_MAX_ALARMS : alarm_index;

	for ( ; i >= 0; i--)
	{
		if ( me->KOSUSettings[i].UserCount > 0 &&
			 me->KOSUSettings[i].UserCount <= UserCount )
		{
			KOSU = me->KOSUAlarms[i].alarms[alarm];
			break;
		} 
	} 

/* Error: */
	if(pKOSUValue) *pKOSUValue = KOSU;
	EXCRETHROW( pexception); 
} /* kosu_GetRequiredKOSUAlarm */


