#include "msxmldom.h"
#include "dbs.h"
#include "dbs_storage.h"
#include <elstring.h>
#include <stdio.h>
#include <time.h>
#include <float.h>
#include <odbc/elodbc.h>
#include <utility.h>
#include "dbs_define_local.h"

#define LOCK_STR		"product_counter"

#define ITEM_ID_OPERATOR				1
#define ITEM_ID_SHIFT					2
#define ITEM_ID_PAUSE					3

#ifndef INT_MAX
#define INT_MAX							2147483647
#endif
#ifndef INT_MIN
#define INT_MIN							(- INT_MAX - 1)
#endif

static SElExceptionPtr storage_StartShift(struct _SDBSStorage* me, const EIdShift shift_id);
static SElExceptionPtr storage_StopShift(struct _SDBSStorage* me);
static SElExceptionPtr storage_IsShiftOpen(struct _SDBSStorage* me, bool_t* pIsOpen, EIdShift* shift_id);
static SElExceptionPtr storage_TerminateShift(struct _SDBSStorage* me);
static SElExceptionPtr storage_GetShiftList(struct _SDBSStorage* me, SStatusItemPtr* pList, int32_t* pListCount); 
static SElExceptionPtr storage_GetSelectedShift(struct _SDBSStorage* me, time_t start, SStatusItemPtr pList); 

static SElExceptionPtr storage_StartPause(struct _SDBSStorage* me);
static SElExceptionPtr storage_DefinePause(struct _SDBSStorage* me, const EIdPause pause_id);
static SElExceptionPtr storage_StopPause(struct _SDBSStorage* me);
static SElExceptionPtr storage_IsPauseOpen(struct _SDBSStorage* me, bool_t* isOpen);
static SElExceptionPtr storage_TerminatePause(struct _SDBSStorage* me);
static SElExceptionPtr storage_GetPauseIntervals(struct _SDBSStorage* me, time_t t1, time_t t2, SIntervalResultPtr* pResult, uint32_t*	pause_time);

static SElExceptionPtr storage_LoginOperator(struct _SDBSStorage* me, const int32_t user_id);
static SElExceptionPtr storage_LogoutOperator(struct _SDBSStorage* me, const int32_t user_id);
static SElExceptionPtr storage_LogoutOperators(struct _SDBSStorage* me);
static SElExceptionPtr storage_IsOperatorLoged(struct _SDBSStorage* me, const int32_t user_id, bool_t* isLoged);
static SElExceptionPtr storage_GetOperatorCount(struct _SDBSStorage* me, uint32_t* pCount);
static SElExceptionPtr storage_TerminateOperator(struct _SDBSStorage* me);
static SElExceptionPtr storage_GetOperatorIntervals( struct _SDBSStorage* me, time_t t1, time_t t2, SIntervalResultPtr* pResult);

static SElExceptionPtr storage_AddProduct(struct _SDBSStorage* me, const char* XMLpath, const EIdMode mode_id);
static SElExceptionPtr storage_GetProductCount(struct _SDBSStorage* me, time_t* t1, time_t* t2, uint16_t failed, int32_t mode_id, uint32_t* pCount);
static SElExceptionPtr storage_GetRetestedProductCount(struct _SDBSStorage* me, time_t* t1, time_t* t2, uint16_t failed, int32_t mode_id, uint32_t* pCount);
static SElExceptionPtr storage_GetLastTwoProducts(struct _SDBSStorage* me, time_t start, time_t* t1, time_t* t2);
static SElExceptionPtr storage_GetAvgKOSU(struct _SDBSStorage* me, time_t t1, time_t t2, double* pAvg, uint32_t* pPause);
static SElExceptionPtr storage_GetProductCounter(struct _SDBSStorage* me, int32_t product_id, uint32_t* count);
static SElExceptionPtr storage_GetBarCodeStatus(struct _SDBSStorage* me, const char* barCode, bool_t* tested, bool_t* failed);

static SElExceptionPtr storage_TestListGet(struct _SDBSStorage* me, int32_t product_id, int32_t	test_id, int32_t parameter_id, SEProductPtr* productlist, int32_t* productlistSize);
static SElExceptionPtr storage_TestListFree(struct _SDBSStorage* me, SEProductPtr* productlist, int32_t productlistSize);
static SElExceptionPtr storage_GetTestVType(struct _SDBSStorage* me, int32_t	product_id, int32_t test_id, int32_t parameter_id, int32_t* vtype, int32_t* count);
static SElExceptionPtr storage_GetTestDValue(struct _SDBSStorage* me, double* value, int32_t* count, double* max, double* min, double* avg);
static SElExceptionPtr storage_GetTestIValue(struct _SDBSStorage* me, int32_t* value, int32_t* count, int32_t* max, int32_t* min, double* avg);

static SElExceptionPtr ConvertTimestampToANSI(char* timestamp, time_t* time);
static SElExceptionPtr ConvertTimestampFromANSI(time_t time, char* timestamp);

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbsstorage_new"
SElExceptionPtr dbsstorage_new(SDBSStoragePtr* pDBSStoragePtr, void* pDBS)
{
	SElExceptionPtr	pexception = NULL;    
	SDBSStoragePtr	me = NULL;

	me = calloc(1,sizeof(SDBSStorage));
	EXCCHECKALLOC(me);
	
	if(pDBSStoragePtr) *pDBSStoragePtr = me;

	/* shift */
	me->StartShift = storage_StartShift;
	me->StopShift = storage_StopShift;
	me->IsShiftOpen = storage_IsShiftOpen;
	me->GetShiftList = storage_GetShiftList;
	me->GetSelectedShift = storage_GetSelectedShift;
	me->TerminateShift = storage_TerminateShift;

	/* pause */
	me->StartPause = storage_StartPause;
	me->DefinePause = storage_DefinePause;
	me->StopPause = storage_StopPause; 
	me->IsPauseOpen = storage_IsPauseOpen;
	me->TerminatePause = storage_TerminatePause;
	me->GetPauseIntervals = storage_GetPauseIntervals;

	/* operator */
	me->LoginOperator = storage_LoginOperator;
	me->LogoutOperator = storage_LogoutOperator; 
	me->LogoutOperators = storage_LogoutOperators; 
	me->IsOperatorLoged = storage_IsOperatorLoged; 
	me->GetOperatorCount = storage_GetOperatorCount; 
	me->TerminateOperator = storage_TerminateOperator;
	me->GetOperatorIntervals = storage_GetOperatorIntervals;

	/* product */
	me->AddProduct = storage_AddProduct;
	me->GetProductCount = storage_GetProductCount;
	me->GetRetestedProductCount = storage_GetRetestedProductCount;
	me->GetLastTwoProducts = storage_GetLastTwoProducts;
	me->GetAvgKOSU = storage_GetAvgKOSU;
	me->GetProductCounter = storage_GetProductCounter;
	me->GetBarCodeStatus = storage_GetBarCodeStatus;

	/* test */
	me->TestListGet = storage_TestListGet;
	me->TestListFree = storage_TestListFree;
	me->GetTestVType = storage_GetTestVType;
	me->GetTestDValue = storage_GetTestDValue;
	me->GetTestIValue = storage_GetTestIValue;

	me->pdbs = pDBS;

Error:
	EXCRETHROW( pexception);   
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbsstorage_delete"
SElExceptionPtr dbsstorage_delete(SDBSStoragePtr* pDBSStoragePtr)
{
	if (pDBSStoragePtr && *pDBSStoragePtr)
	{
		free(*pDBSStoragePtr);
		*pDBSStoragePtr = NULL;
	}

/* Error: */
	return NULL;     
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_StartShift"
static SElExceptionPtr storage_StartShift(struct _SDBSStorage* me, const EIdShift shift_id)
{
	SElExceptionPtr	pexception = NULL;    
	char			sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr		podbc = PDBS->podbc;
	char			datetime[DBS_RECORD_LENGHT_TIME+1];

	LOCK;
	
	DATETIME_TO_STRING(datetime);

	sprintf(sql, "INSERT INTO tester.status "
				 "(status.time,status.time_end,status.tester_id,status.item_id,status.id,status.terminated) "
				 "VALUES ('%s',NULL,%d,%d,%d,NULL)",
				 datetime,
				 PDBS_TESTER_ID,
				 ITEM_ID_SHIFT,
				 shift_id );

	EXCCHECK( podbc->ExecSQL( podbc, sql));
	EXCCHECK( podbc->Cancel(podbc));

Error:
	UNLOCK;
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_StopShift"
static SElExceptionPtr storage_StopShift(struct _SDBSStorage* me)
{
	SElExceptionPtr	pexception = NULL; 
	char			sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr		podbc = PDBS->podbc;
	char			datetime[DBS_RECORD_LENGHT_TIME+1];

	LOCK;
	
	DATETIME_TO_STRING(datetime);

	sprintf(sql, "UPDATE tester.status "
				 "SET status.time_end='%s',status.terminated=0 "
				 "WHERE status.time_end IS NULL AND status.tester_id=%d AND status.item_id=%d AND status.terminated IS NULL",
				 datetime,
				 PDBS_TESTER_ID,
				 ITEM_ID_SHIFT);

	EXCCHECK( podbc->ExecSQL( podbc, sql));
	EXCCHECK( podbc->Cancel(podbc));

Error:
	UNLOCK;
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_IsShiftOpen"
static SElExceptionPtr storage_IsShiftOpen(struct _SDBSStorage* me, bool_t* pIsOpen, EIdShift* shift_id)
{
	SElExceptionPtr	pexception = NULL; 
	int32_t		error = 0;
	char		sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr	podbc = PDBS->podbc;
	
	LOCK;
	
	sprintf(sql, "SELECT s.id "
				 "FROM tester.status s "
				 "WHERE s.time_end IS NULL AND s.tester_id=%d AND s.item_id=%d AND s.terminated IS NULL",
				 PDBS_TESTER_ID,
				 ITEM_ID_SHIFT);

	EXCCHECK( podbc->ExecSQL( podbc, sql));
	CHECKERR( podbc->Fetch(podbc, TRUE));
	if(error==ELODBC_SQL_WARN_NO_DATA_FOUND)
	{
		if(pIsOpen) *pIsOpen = FALSE;
		if(shift_id) *shift_id = ID_SHIFT_NONE;
	}
	else
	{
		if(pIsOpen) *pIsOpen = TRUE;
		if(shift_id) *shift_id = *((short*)podbc->GetFieldValuePtr(podbc, 1));
	}
	
	EXCCHECK( podbc->Cancel(podbc));

Error:
	UNLOCK;
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_GetShiftList"
static SElExceptionPtr storage_GetShiftList(struct _SDBSStorage* me, SStatusItemPtr* pList, int32_t* pListCount)
{
	SElExceptionPtr	pexception = NULL; 
	int32_t			error = 0;
	char			sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr		podbc = PDBS->podbc;
	int32_t			lenght, count, i;
	SStatusItemPtr	plist = NULL;

	LOCK;
	
	if(pList) *pList = NULL;
	if(pListCount) *pListCount = 0;  
	
	sprintf(sql, "SELECT s.time,s.time_end,s.id,s.terminated "
				 "FROM tester.status s "
				 "WHERE s.tester_id=%d AND s.item_id=%d "
				 "ORDER BY s.time DESC",
				 PDBS_TESTER_ID,
				 ITEM_ID_SHIFT);

	EXCCHECK( podbc->ExecSQL(podbc, sql));
	EXCCHECK( podbc->GetRows(podbc, &count));

	if (count==0)
		EXCTHROW(DBS_STORAGE_ERROR_NOT_SHIFT_DEFINED, "DBS_STORAGE_ERROR_NOT_SHIFT_DEFINED");
	
	plist = calloc(count, sizeof(SStatusItem));
	EXCCHECKALLOC(plist);

	CHECKERR( podbc->Fetch(podbc, TRUE));
	
	for(i=0; i<count; i++)
	{
		plist[i].tester_id = PDBS_TESTER_ID;
		plist[i].item_id = ITEM_ID_SHIFT;
		plist[i].id = *((short*)podbc->GetFieldValuePtr(podbc, 3)); 
	
		EXCCHECK( ConvertTimestampToANSI( (char*)podbc->GetFieldValuePtr(podbc, 1), &plist[i].time));

		EXCCHECK( podbc->GetFieldLen(podbc, 2, &lenght));
		if(lenght)
		{
			EXCCHECK( ConvertTimestampToANSI( (char*)podbc->GetFieldValuePtr(podbc, 2), &plist[i].time_end));
			plist[i].terminated = *((short*)podbc->GetFieldValuePtr(podbc, 4));
			plist[i].opened = FALSE;
		}
		else
		{
			time(&plist[i].time_end);
			plist[i].terminated = FALSE;
			plist[i].opened = TRUE;
		}
		CHECKERR( podbc->MoveNext(podbc));
	}
	
	EXCCHECK( podbc->Cancel(podbc));

	if(pList) *pList = plist;
	if(pListCount) *pListCount = count;  
	
Error:
	UNLOCK;
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_GetSelectedShift"
static SElExceptionPtr storage_GetSelectedShift(
	struct _SDBSStorage* me, 
	time_t	start,
	SStatusItemPtr pList
)
{
	SElExceptionPtr	pexception = NULL; 
	int32_t		error = 0;
	char		sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr	podbc = PDBS->podbc;
	int32_t		lenght;
	char		datetime[DBS_RECORD_LENGHT_TIME+1];  
	
	LOCK;
	
	if(start == 0)
	{
		sprintf(sql, "SELECT s.time,s.time_end,s.id,s.terminated "
					 "FROM tester.status s "
					 "WHERE s.tester_id=%d AND s.item_id=%d "
					 "ORDER BY s.time DESC",
					 PDBS_TESTER_ID,
					 ITEM_ID_SHIFT);
	}
	else
	{
		EXCCHECK( ConvertTimestampFromANSI(start, datetime));
	
		sprintf(sql, "SELECT s.time,s.time_end,s.id,s.terminated "
					 "FROM tester.status s "
					 "WHERE s.tester_id=%d AND s.item_id=%d AND s.time='%s' "
					 "ORDER BY s.time DESC",
					 PDBS_TESTER_ID,
					 ITEM_ID_SHIFT,
					 datetime);
	}

	EXCCHECK( podbc->ExecSQL( podbc, sql));
	CHECKERR( podbc->Fetch(podbc, TRUE));
	
	if (error==ELODBC_SQL_WARN_NO_DATA_FOUND)
       EXCTHROW(DBS_STORAGE_ERROR_NOT_SHIFT_DEFINED, "DBS_STORAGE_ERROR_NOT_SHIFT_DEFINED");

	pList->tester_id = PDBS_TESTER_ID;
	pList->item_id = ITEM_ID_SHIFT;
	pList->id = *((short*)podbc->GetFieldValuePtr(podbc, 3)); 
	
	EXCCHECK( ConvertTimestampToANSI( (char*)podbc->GetFieldValuePtr(podbc, 1), &pList->time));

	EXCCHECK( podbc->GetFieldLen(podbc, 2, &lenght));
	if(lenght)
	{
		EXCCHECK( ConvertTimestampToANSI( (char*)podbc->GetFieldValuePtr(podbc, 2), &pList->time_end));
		pList->terminated = *((short*)podbc->GetFieldValuePtr(podbc, 4));
		pList->opened = FALSE;
	}
	else
	{
		time(&pList->time_end);
		pList->terminated = FALSE;
		pList->opened = TRUE;
	}
	
	EXCCHECK( podbc->Cancel(podbc));

Error:
	UNLOCK;
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_TerminateShift"
static SElExceptionPtr storage_TerminateShift(struct _SDBSStorage* me)
{
	SElExceptionPtr	pexception = NULL; 
	char			sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr		podbc = PDBS->podbc;
	char			datetime[DBS_RECORD_LENGHT_TIME+1];

	LOCK;
	
	DATETIME_TO_STRING(datetime);

	sprintf(sql, "UPDATE tester.status "
				 "SET status.time_end='%s',status.terminated=1 "
				 "WHERE status.time_end IS NULL AND status.tester_id=%d AND status.item_id=%d AND status.terminated IS NULL",
				 datetime,
				 PDBS_TESTER_ID,
				 ITEM_ID_SHIFT);

	EXCCHECK( podbc->ExecSQL( podbc, sql));
	EXCCHECK( podbc->Cancel(podbc));

Error:
	UNLOCK;
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_StartPause"
static SElExceptionPtr storage_StartPause(struct _SDBSStorage* me)
{
	SElExceptionPtr	pexception = NULL; 
	char			sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr		podbc = PDBS->podbc;
	char			datetime[DBS_RECORD_LENGHT_TIME+1];

	LOCK;
	
	DATETIME_TO_STRING(datetime);

	sprintf(sql, "INSERT INTO tester.status "
				 "(status.time,status.time_end,status.tester_id,status.item_id,status.id,status.terminated) "
				 "VALUES ('%s',NULL,%d,%d,%d,NULL)",
				 datetime,
				 PDBS_TESTER_ID,
				 ITEM_ID_PAUSE,
				 ID_PAUSE_OPERATOR);

	EXCCHECK( podbc->ExecSQL( podbc, sql));
	EXCCHECK( podbc->Cancel(podbc));

Error:
	UNLOCK;
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_DefinePause"
static SElExceptionPtr storage_DefinePause(struct _SDBSStorage* me, const EIdPause pause_id)
{
	SElExceptionPtr	pexception = NULL; 
	char			sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr		podbc = PDBS->podbc;

	LOCK;
	
	sprintf(sql, "UPDATE tester.status "
				 "SET status.id=%d "
				 "WHERE status.time_end IS NULL AND status.tester_id=%d AND status.item_id=%d AND status.terminated IS NULL",
				 pause_id,
				 PDBS_TESTER_ID,
				 ITEM_ID_PAUSE);

	EXCCHECK( podbc->ExecSQL( podbc, sql));
	EXCCHECK( podbc->Cancel(podbc));

Error:
	UNLOCK;
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_StopPause"
static SElExceptionPtr storage_StopPause(struct _SDBSStorage* me)
{
	SElExceptionPtr	pexception = NULL; 
	char			sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr		podbc = PDBS->podbc;
	char			datetime[DBS_RECORD_LENGHT_TIME+1];

	LOCK;
	
	DATETIME_TO_STRING(datetime);

	sprintf(sql, "UPDATE tester.status "
				 "SET status.time_end='%s',status.terminated=0 "
				 "WHERE status.time_end IS NULL AND status.tester_id=%d AND status.item_id=%d AND status.terminated IS NULL",
				 datetime,
				 PDBS_TESTER_ID,
				 ITEM_ID_PAUSE);

	EXCCHECK( podbc->ExecSQL( podbc, sql));
	EXCCHECK( podbc->Cancel(podbc));

Error:
	UNLOCK;
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_IsPauseOpen"
static SElExceptionPtr storage_IsPauseOpen(struct _SDBSStorage* me, bool_t* isOpen)
{
	SElExceptionPtr	pexception = NULL; 
	int32_t		error = 0;
	char		sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr	podbc = PDBS->podbc;
	
	LOCK;
	
	sprintf(sql, "SELECT * "
				 "FROM tester.status s "
				 "WHERE s.time_end IS NULL AND s.tester_id=%d AND s.item_id=%d AND s.terminated IS NULL",
				 PDBS_TESTER_ID,
				 ITEM_ID_PAUSE);

	EXCCHECK( podbc->ExecSQL( podbc, sql));
	CHECKERR( podbc->Fetch(podbc, TRUE));
	
	if(isOpen) *isOpen = !(error==ELODBC_SQL_WARN_NO_DATA_FOUND);
	
	EXCCHECK( podbc->Cancel(podbc));

Error:
	UNLOCK;
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_TerminatePause"
static SElExceptionPtr storage_TerminatePause(struct _SDBSStorage* me)
{
	SElExceptionPtr	pexception = NULL; 
	char		sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr	podbc = PDBS->podbc;
	char		datetime[DBS_RECORD_LENGHT_TIME+1];

	LOCK;
	
	DATETIME_TO_STRING(datetime);

	sprintf(sql, "UPDATE tester.status "
				 "SET status.time_end='%s',status.terminated=1 "
				 "WHERE status.time_end IS NULL AND status.tester_id=%d AND status.item_id=%d AND status.terminated IS NULL",
				 datetime,
				 PDBS_TESTER_ID,
				 ITEM_ID_PAUSE);

	EXCCHECK( podbc->ExecSQL( podbc, sql));
	EXCCHECK( podbc->Cancel(podbc));

Error:
	UNLOCK;
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_GetPauseIntervals"
static SElExceptionPtr storage_GetPauseIntervals(
	struct _SDBSStorage* me, 
	time_t 				t1, 
	time_t 				t2, 
	SIntervalResultPtr* pResult,
	uint32_t*			pause_time
)
{
	SElExceptionPtr	pexception = NULL; 
	int32_t		error = 0;
	char		sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr	podbc = PDBS->podbc;
	char		datetime1[DBS_RECORD_LENGHT_TIME+1] = "";
	char		datetime2[DBS_RECORD_LENGHT_TIME+1] = "";
	uint32_t	length = 0;
	time_t		time_now, lt1, lt2;
	uint32_t	sum_time = 0;

	LOCK;
	
	if(pResult)
		EXCCHECK( intervalresult_new(pResult));

	time(&time_now);

	ConvertTimestampFromANSI(t1, datetime1);
	ConvertTimestampFromANSI(t2, datetime2);

	sprintf(sql, "SELECT s.time,s.time_end " 
				 "FROM tester.status s "
				 "WHERE s.tester_id=%d AND s.item_id=%d AND ( "
				 "(s.time>='%s' AND s.time<'%s') OR "
				 "(s.time_end>'%s' AND s.time_end<='%s') OR "
				 "(s.time<='%s' AND s.time_end>='%s') OR "
				 "(s.time<'%s' AND s.time_end IS NULL) "
				 ") ORDER BY time",
				 PDBS_TESTER_ID,
				 ITEM_ID_PAUSE,
				 datetime1,
				 datetime2,
				 datetime1,
				 datetime2,
				 datetime1,
				 datetime2,
				 datetime2);

	EXCCHECK( podbc->ExecSQL( podbc, sql));
	CHECKERR( podbc->Fetch(podbc, TRUE));
	
	if ( error != ELODBC_SQL_WARN_NO_DATA_FOUND )
	{
		do 
		{
			EXCCHECK( ConvertTimestampToANSI( (char*)podbc->GetFieldValuePtr(podbc, 1), &lt1));
			EXCCHECK( podbc->GetFieldLen( podbc, 2, &length));
			if(length>0)
			{
				EXCCHECK( ConvertTimestampToANSI( (char*)podbc->GetFieldValuePtr(podbc, 2), &lt2));
			}
			else
			{
				time(&lt2);
			}
			if(pResult) EXCCHECK((*pResult)->Add(*pResult, (lt1>t1)? lt1 : t1, (lt2<t2)? lt2 : t2 ));
			
			sum_time += (uint32_t) ( ( (lt2<t2)? lt2 : t2 ) - ( (lt1>t1)? lt1 : t1 ) );

		} while(podbc->MoveNext(podbc)==0);
	}
	
	EXCCHECK(podbc->Cancel(podbc));

	if(pause_time) *pause_time = sum_time;

Error:
	UNLOCK;
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_LoginOperator"
static SElExceptionPtr storage_LoginOperator(struct _SDBSStorage* me, const int32_t user_id)
{
	SElExceptionPtr	pexception = NULL; 
	char		sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr	podbc = PDBS->podbc;
	char		datetime[DBS_RECORD_LENGHT_TIME+1];

	LOCK;
	
	DATETIME_TO_STRING(datetime);

	sprintf(sql, "INSERT INTO tester.status "
				 "(status.time,status.time_end,status.tester_id,status.item_id,status.id,status.terminated) "
				 "VALUES ('%s',NULL,%d,%d,%d,NULL)",
				 datetime,
				 PDBS_TESTER_ID,
				 ITEM_ID_OPERATOR,
				 user_id );

	EXCCHECK( podbc->ExecSQL( podbc, sql));
	EXCCHECK( podbc->Cancel(podbc));

Error:
	UNLOCK;
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_LogoutOperator"
static SElExceptionPtr storage_LogoutOperator(struct _SDBSStorage* me, const int32_t user_id)
{
	SElExceptionPtr	pexception = NULL; 
	char		sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr	podbc = PDBS->podbc;
	char		datetime[DBS_RECORD_LENGHT_TIME+1];

	LOCK;
	
	DATETIME_TO_STRING(datetime);

	sprintf(sql, "UPDATE tester.status "
				 "SET status.time_end='%s',status.terminated=0 "
				 "WHERE status.time_end IS NULL AND status.tester_id=%d AND status.item_id=%d AND status.id=%d AND status.terminated IS NULL",
				 datetime,
				 PDBS_TESTER_ID,
				 ITEM_ID_OPERATOR,
				 user_id);

	EXCCHECK( podbc->ExecSQL( podbc, sql));
	EXCCHECK( podbc->Cancel(podbc));

Error:
	UNLOCK;
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_LogoutOperators"
static SElExceptionPtr storage_LogoutOperators(struct _SDBSStorage* me)
{
	SElExceptionPtr	pexception = NULL; 
	char		sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr	podbc = PDBS->podbc;
	char		datetime[DBS_RECORD_LENGHT_TIME+1];

	LOCK;
	
	DATETIME_TO_STRING(datetime);

	sprintf(sql, "UPDATE tester.status "
				 "SET status.time_end='%s',status.terminated=0 "
				 "WHERE status.time_end IS NULL AND status.tester_id=%d AND status.item_id=%d AND status.terminated IS NULL",
				 datetime,
				 PDBS_TESTER_ID,
				 ITEM_ID_OPERATOR);

	EXCCHECK( podbc->ExecSQL( podbc, sql));
	EXCCHECK( podbc->Cancel(podbc));

Error:
	UNLOCK;
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_IsOperatorLoged"
static SElExceptionPtr storage_IsOperatorLoged(struct _SDBSStorage* me, const int32_t user_id, bool_t* isLoged)
{
	SElExceptionPtr	pexception = NULL; 
	int32_t		error = 0;
	char		sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr	podbc = PDBS->podbc;
	
	LOCK;
	
	sprintf(sql, "SELECT * "
				 "FROM tester.status s "
				 "WHERE s.time_end IS NULL AND s.tester_id=%d AND s.item_id=%d AND s.id=%d AND s.terminated IS NULL",
				 PDBS_TESTER_ID,
				 ITEM_ID_OPERATOR,
				 user_id);

	EXCCHECK( podbc->ExecSQL( podbc, sql));
	CHECKERR( podbc->Fetch(podbc, TRUE));
	
	*isLoged = !(error==ELODBC_SQL_WARN_NO_DATA_FOUND);
	
	EXCCHECK( podbc->Cancel(podbc));

Error:
	UNLOCK;
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_GetOperatorCount"
static SElExceptionPtr storage_GetOperatorCount(struct _SDBSStorage* me, uint32_t* pCount)
{
	SElExceptionPtr	pexception = NULL; 
	int32_t		error = 0;
	char		sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr	podbc = PDBS->podbc;

	LOCK;
	
	sprintf(sql, "SELECT COUNT(s.id) "
				 "FROM tester.status s "
				 "WHERE s.time_end IS NULL AND s.tester_id=%d AND s.item_id=%d AND s.terminated IS NULL",
				 PDBS_TESTER_ID,
				 ITEM_ID_OPERATOR);

	EXCCHECK( podbc->ExecSQL( podbc, sql));
	CHECKERR( podbc->Fetch(podbc, TRUE));
	
	sscanf(podbc->GetFieldValuePtr(podbc, 1), "%d", pCount);
	
	EXCCHECK( podbc->Cancel(podbc));
		
Error:
	UNLOCK;
	if(pexception!=NULL) *pCount = 0;  
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_TerminateOperator"
static SElExceptionPtr storage_TerminateOperator(struct _SDBSStorage* me)
{
	SElExceptionPtr	pexception = NULL; 
	char		sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr	podbc = PDBS->podbc;
	char		datetime[DBS_RECORD_LENGHT_TIME+1];

	LOCK;
	
	DATETIME_TO_STRING(datetime);

	sprintf(sql, "UPDATE tester.status "
				 "SET status.time_end='%s',status.terminated=1 "
				 "WHERE status.time_end IS NULL AND status.tester_id=%d AND status.item_id=%d AND status.terminated IS NULL",
				 datetime,
				 PDBS_TESTER_ID,
				 ITEM_ID_OPERATOR);

	EXCCHECK( podbc->ExecSQL( podbc, sql));
	EXCCHECK( podbc->Cancel(podbc));

Error:
	UNLOCK;
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_GetOperatorIntervals"
static SElExceptionPtr storage_GetOperatorIntervals(
	struct _SDBSStorage* me, 
	time_t t1, 
	time_t t2, 
	SIntervalResultPtr* pResult
)
{
	SElExceptionPtr	pexception = NULL; 
	int32_t		error = 0;
	char		sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr	podbc = PDBS->podbc;
	char		datetime1[DBS_RECORD_LENGHT_TIME+1] = "";
	char		datetime2[DBS_RECORD_LENGHT_TIME+1] = "";
	uint32_t	length = 0;
	time_t		lt1, lt2;

	LOCK;
	
	if(pResult)
		EXCCHECK( intervalresult_new(pResult));

	ConvertTimestampFromANSI(t1, datetime1);
	ConvertTimestampFromANSI(t2, datetime2);

	sprintf(sql, "SELECT s.time,s.time_end " 
				 "FROM tester.status s "
				 "WHERE s.tester_id=%d AND s.item_id=%d AND ( "
				 "(s.time>='%s' AND s.time<'%s') OR "
				 "(s.time_end>'%s' AND s.time_end<='%s') OR "
				 "(s.time<='%s' AND s.time_end>='%s') OR "
				 "(s.time<'%s' AND s.time_end IS NULL) "
				 ") ORDER BY time",
				 PDBS_TESTER_ID,
				 ITEM_ID_OPERATOR,
				 datetime1,
				 datetime2,
				 datetime1,
				 datetime2,
				 datetime1,
				 datetime2,
				 datetime2);

	EXCCHECK( podbc->ExecSQL( podbc, sql));
	CHECKERR( podbc->Fetch(podbc, TRUE));
	
	if ( error != ELODBC_SQL_WARN_NO_DATA_FOUND )
	{
		do 
		{
			EXCCHECK( ConvertTimestampToANSI( (char*)podbc->GetFieldValuePtr(podbc, 1), &lt1));
			EXCCHECK( podbc->GetFieldLen( podbc, 2, &length));
			if(length>0)
			{
				EXCCHECK( ConvertTimestampToANSI( (char*)podbc->GetFieldValuePtr(podbc, 2), &lt2));
			}
			else
			{
				time(&lt2);
			}
			EXCCHECK((*pResult)->Add(*pResult, (lt1>t1)? lt1 : t1, (lt2<t2)? lt2 : t2 ));

		} while(podbc->MoveNext(podbc)==0);
	}
	
	EXCCHECK(podbc->Cancel(podbc));

Error:
	UNLOCK;
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_AddProduct"
static SElExceptionPtr	storage_AddProduct(
	struct _SDBSStorage* me, 
	const char* pathXML,
	const EIdMode mode_id
)
{
	SElExceptionPtr	pexception = NULL; 
	int32_t			error = 0;	
	HRESULT         hr = 0;
	CAObjHandle     pxml  = 0; 
	CAObjHandle     pnode  = 0, pnode_old = 0;
	CAObjHandle     pnode2  = 0, pnode2_old = 0;        
	CAObjHandle     pparameter  = 0;
	VBOOL           is_xml_success = 1;
	VARIANT         pfile_name_var = CA_VariantEmpty();
	char     		*ptext= NULL, *psql = NULL;
	
	SOdbcPtr		podbc = PDBS->podbc;
	char			sql[SQL_COMMAND_LENGHT] = "";
	char			duration[256] = "", 
					date_time[256] = "", 
					barcode[256] = "",
					barcode_Right[256] = "",
					barcode_Left[256] = "",
					product_id[32] = "",
					test_id[32] = "",
					parameter_id[32] = "",
		 			failed[32] = "",
					value[256] = "";
	
	SELStringPtr   	pelstring = NULL;
	bool_t			first_item = TRUE;
	
	LOCK;
	
	EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml));
	EXCCHECKCOM( CA_VariantSetCString(  &pfile_name_var, pathXML));
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (pxml, NULL, pfile_name_var, &is_xml_success));
	
	if(is_xml_success)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode( pxml, NULL, "/testseq/product", &pnode));
	 
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "date_time", &pparameter)); 
		if(pparameter)
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext( pparameter, NULL, &ptext));
			
			sprintf(date_time, ptext);
	
			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "id", &pparameter)); 
		if(pparameter)
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext( pparameter, NULL, &ptext));
			
			sprintf(product_id, ptext);
	
			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "barcode", &pparameter)); 
		if(pparameter)
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext( pparameter, NULL, &ptext));
			
			sprintf(barcode, ptext); 
	
			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "duration", &pparameter));
		if(pparameter)
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext( pparameter, NULL, &ptext));
			
			sprintf(duration, ptext);   
	
			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "failed", &pparameter));
		if(pparameter)
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext( pparameter, NULL, &ptext));
			
			sprintf(failed, ptext);   
	
			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
		
		EXCCHECKCVI( CA_DiscardObjHandle(pnode));
		pnode = 0;
		
		if(strlen(duration) == 0) //for simulation
			sprintf(duration, "0");
		
		if( strlen(date_time)>0
			&& strlen(product_id)>0
			&& strlen(barcode)>0
			&& strlen(duration)>0
			&& strlen(failed)>0)
		{
			sprintf(sql, "INSERT INTO tester.product_status "
					 "VALUES ('%s',%d,%s,'%s',%s,%s,%d)",
					 date_time,
					 PDBS_TESTER_ID,
					 product_id,
					 barcode,
					 duration,
					 failed,
					 mode_id);
//printf("barcode %s\n duration %s\n failed %s\n",barcode,duration,failed);

			EXCCHECK( podbc->ExecSQL(podbc, sql));
			EXCCHECK( podbc->Cancel(podbc));
			
#if 0    /* formerly excluded lines */
			/* save only good tests results of production mode */
			if(0==strcmp(failed, "0")
			   && mode_id==ID_MODE_PRODUCTION)
			{
				EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode( pxml, NULL, "/testseq/test", &pnode)); 
				
				EXCCHECK( elstring_new("INSERT INTO tester.test_status VALUES ", &pelstring)); 
				
				while(pnode!=0)
				{
					EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "id", &pparameter)); 
					if(pparameter)
					{
						EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext( pparameter, NULL, &ptext));
			
						sprintf(test_id, ptext);   
						
						EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
						pparameter = 0;
						CA_FreeMemory(ptext);
						ptext=NULL;
					}
					
					EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "measure", &pnode2));    
					
					while(pnode2!=0)
					{
						EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode2, NULL, "limit/id", &pparameter)); 
						if(pparameter)
						{
							EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext( pparameter, NULL, &ptext));
				
							sprintf(parameter_id, ptext); 
							
							EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
							pparameter = 0;
							CA_FreeMemory(ptext);
							ptext=NULL;
						}
						
						EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode2, NULL, "value", &pparameter)); 
						if(pparameter)
						{
							EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext( pparameter, NULL, &ptext));
				
							sprintf(value, ptext);   
		
							EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
							pparameter = 0;
							CA_FreeMemory(ptext);
							ptext=NULL;
						}
						
						sprintf(failed, "");    
						EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode2, NULL, "failed", &pparameter)); 
						if(pparameter)
						{
							EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext( pparameter, NULL, &ptext));
				
							sprintf(failed, ptext);   
		
							EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
							pparameter = 0;
							CA_FreeMemory(ptext);
							ptext=NULL;
						}
						
						EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode2, NULL, "value/@type", &pparameter)); 
						if(pparameter)
						{
							EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext( pparameter, NULL, &ptext));
							   
							if(0==strcmp(ptext, "int32"))
							{	
								sprintf(sql, ",('%s',%d,%s,%s,%s,%d,NULL,%s,NULL)",
											date_time,
											PDBS_TESTER_ID,
							 				product_id,
											test_id,
											parameter_id,
											E_DVT_INT32,
											value);
							
								psql = sql;
								EXCCHECK(pelstring->Append(pelstring, (first_item)? psql+1 : psql) );
								first_item = FALSE;
							}
							else if(0==strcmp(ptext, "real64"))
							{	
								sprintf(sql, ",('%s',%d,%s,%s,%s,%d,%s,NULL,NULL)",
											date_time,
											PDBS_TESTER_ID,
							 				product_id,
											test_id,
											parameter_id,
											E_DVT_REAL64,
											value);
							
								psql = sql;
								EXCCHECK(pelstring->Append(pelstring, (first_item)? psql+1 : psql) );
								first_item = FALSE;
							}
							
							EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
							pparameter = 0;
							CA_FreeMemory(ptext);
							ptext=NULL;
						}
						
						pnode2_old = pnode2;
						MSXML_IXMLDOMNodeGetnextSibling (pnode2, NULL, &pnode2);
						EXCCHECKCVI( CA_DiscardObjHandle(pnode2_old));
						pnode2_old = 0;   
					}
					
					pnode_old = pnode;
					MSXML_IXMLDOMNodeGetnextSibling (pnode, NULL, &pnode);
					EXCCHECKCVI( CA_DiscardObjHandle(pnode_old));
					pnode_old = 0;   
				}
				if(first_item==FALSE)
				{
					EXCCHECK( podbc->ExecSQL(podbc, pelstring->GetPtr(pelstring)));
					EXCCHECK( podbc->Cancel(podbc));
				}
			} /* if(0==strcmp(failed, "0")) */
#endif   /* formerly excluded lines */
		}
	}

Error:
	UNLOCK;
	elstring_delete(&pelstring);          
	CA_VariantClear(&pfile_name_var);
	if(pparameter) CA_DiscardObjHandle(pparameter);  
	if(pnode) CA_DiscardObjHandle(pnode);
	if (pxml) CA_DiscardObjHandle(pxml);
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_GetProductCount"
static SElExceptionPtr storage_GetProductCount(
	struct _SDBSStorage* me, 
	time_t* t1, 
	time_t* t2, 
	uint16_t failed,
	int32_t mode_id,
	uint32_t* pCount
)
{
	SElExceptionPtr	pexception = NULL; 
	int32_t		error = 0;
	char		sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr	podbc = PDBS->podbc;
		
	LOCK;
	
	if (t1==NULL && t2==NULL)
	{
		sprintf(sql, "SELECT COUNT(ps.product_id) " 
					 "FROM tester.product_status ps "
					 "WHERE ps.tester_id=%d AND ps.failed=%d AND mode_id=%d",
					 PDBS_TESTER_ID,
					 (failed)? 1:0,
					 mode_id);
	}
	else
	{
		char datetime1[DBS_RECORD_LENGHT_TIME+1] = "";
		ConvertTimestampFromANSI(*t1, datetime1);
		
		if(t2!=NULL)
		{
			char	datetime2[DBS_RECORD_LENGHT_TIME+1] = "";
			ConvertTimestampFromANSI(*t2, datetime2);

			sprintf(sql, "SELECT COUNT(ps.product_id) " 
						 "FROM tester.product_status ps "
						 "WHERE ps.tester_id=%d AND ps.failed=%d AND mode_id=%d "
						 "AND ps.time>='%s' AND ps.time<='%s'",
						 PDBS_TESTER_ID,
						 (failed)? 1:0,
						 mode_id,
						 datetime1,
						 datetime2);
		}
		else
		{
			sprintf(sql, "SELECT COUNT(ps.product_id) " 
						 "FROM tester.product_status ps "
						 "WHERE ps.tester_id=%d AND ps.failed=%d AND mode_id=%d "
						 "AND ps.time>='%s'",
						 PDBS_TESTER_ID,
						 (failed)? 1:0,
						 mode_id,
						 datetime1);
		}
	}
	EXCCHECK( podbc->ExecSQL(podbc, sql));
	CHECKERR( podbc->Fetch(podbc, TRUE));

	sscanf(podbc->GetFieldValuePtr(podbc, 1), "%d", pCount);

	EXCCHECK( podbc->Cancel(podbc));

Error:
	UNLOCK;
	if(pexception!=0) *pCount = 0; 
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_GetRetestedProductCount"
static SElExceptionPtr storage_GetRetestedProductCount(
	struct _SDBSStorage* me, 
	time_t* t1, 
	time_t* t2, 
	uint16_t failed,
	int32_t mode_id,
	uint32_t* pCount
)
{
	SElExceptionPtr	pexception = NULL; 
	int32_t			error = 0;
	char			sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr		podbc = PDBS->podbc;
	int32_t			count, i, value, product = 0;
		
	LOCK;

	if(pCount) *pCount = 0;
	
	if (t1==NULL && t2==NULL)
	{
		sprintf(sql, "SELECT ps.failed " 
					 "FROM tester.product_status ps "
					 "WHERE ps.tester_id=%d mode_id=%d "
					 "group by ps.barcode order by ps.time ",
					 PDBS_TESTER_ID,
					 mode_id);
	}
	else
	{
		char datetime1[DBS_RECORD_LENGHT_TIME+1] = "";
		ConvertTimestampFromANSI(*t1, datetime1);
		
		if(t2!=NULL)
		{
			char	datetime2[DBS_RECORD_LENGHT_TIME+1] = "";
			ConvertTimestampFromANSI(*t2, datetime2);

			sprintf(sql, "SELECT ps.failed " 
						 "FROM tester.product_status ps "
						 "WHERE ps.tester_id=%d AND mode_id=%d "
						 "AND ps.time>='%s' AND ps.time<='%s' "
						 "group by ps.barcode order by ps.time ",
						 PDBS_TESTER_ID,
						 mode_id,
						 datetime1,
						 datetime2);
		}
		else
		{
			sprintf(sql, "SELECT ps.failed " 
						 "FROM tester.product_status ps "
						 "WHERE ps.tester_id=%d AND mode_id=%d "
						 "AND ps.time>='%s' "
						 "group by ps.barcode order by ps.time ",
						 PDBS_TESTER_ID,
						 mode_id,
						 datetime1);
		}
	}
	EXCCHECK( podbc->ExecSQL(podbc, sql));
	EXCCHECK( podbc->GetRows(podbc, &count));
	CHECKERR( podbc->Fetch(podbc, TRUE));
	
	for(i=0; i<count;i++)
	{
		value = (int)(*((char*)podbc->GetFieldValuePtr(podbc, 1)));
		
		if(value==failed)
			product++;

		podbc->MoveNext(podbc);
	}
	EXCCHECK( podbc->Cancel(podbc));

	if(pCount) *pCount = product;

Error:
	UNLOCK;
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_GetLastTwoProducts"
static SElExceptionPtr storage_GetLastTwoProducts(struct _SDBSStorage* me, time_t start, time_t* t1, time_t* t2)
{
	SElExceptionPtr	pexception = NULL;   
	int32_t		error = 0;
	char		sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr	podbc = PDBS->podbc;
	char		datetime[DBS_RECORD_LENGHT_TIME+1] = "";
	
	LOCK;
	
	ConvertTimestampFromANSI(start, datetime);

	sprintf(sql, "SELECT ps.time "
				 "FROM tester.product_status ps "
				 "WHERE ps.tester_id=%d AND ps.failed=0 AND mode_id=%d AND ps.time>='%s' "
				 "ORDER BY ps.time DESC LIMIT 2",
				 PDBS_TESTER_ID,
				 ID_MODE_PRODUCTION,
				 datetime);

	EXCCHECK( podbc->ExecSQL(podbc, sql));
	CHECKERR( podbc->Fetch(podbc, TRUE));

	if(error!=ELODBC_SQL_WARN_NO_DATA_FOUND)
	{
		EXCCHECK( ConvertTimestampToANSI( (char*)podbc->GetFieldValuePtr(podbc, 1), t1));

		if(podbc->MoveNext(podbc)==0)
			EXCCHECK( ConvertTimestampToANSI( (char*)podbc->GetFieldValuePtr(podbc, 1), t2));
		else
			*t2 = *t1;
	}
	else
	{
		EXCTHROW(DBS_STORAGE_ERROR_NOT_PRODUCT_DEFINED, "DBS_STORAGE_ERROR_NOT_PRODUCT_DEFINED");
	}

	EXCCHECK( podbc->Cancel(podbc));

Error:
	UNLOCK;
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_GetAvgKOSU"
static SElExceptionPtr storage_GetAvgKOSU(
	struct _SDBSStorage* me, 
	time_t		t1, 
	time_t		t2, 
	double*		pAvg, 
	uint32_t*	pPause
)
{
	SElExceptionPtr	pexception = NULL;   
	int32_t		error = 0;
	uint32_t	count, pause_time;
	
	if(pAvg) *pAvg = 0;
	if(pPause) *pPause = 0;
	
	EXCCHECK( me->GetPauseIntervals( me , t1, t2, NULL, &pause_time) );
	
	if(pPause) *pPause = pause_time;
	
	EXCCHECK( me->GetProductCount(me, &t1, &t2, FALSE, ID_MODE_PRODUCTION, &count) );

	if( (t2-t1-pause_time)< ((t2-t1)/4) )
		error = DBS_STORAGE_WARNING_VALID_AVG_INTERVAL;
		
	if(count>0)
		if(pAvg) *pAvg = (double)(t2-t1-pause_time)/(double)count;

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_GetProductCounter"
static SElExceptionPtr storage_GetProductCounter(struct _SDBSStorage* me, int32_t product_id, uint32_t* pCount)
{
	SElExceptionPtr	pexception = NULL;   
	int32_t			error = 0;
	char			sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr		podbc = PDBS->podbc;
	int32_t			pid[16];
	int32_t			pidSize = 16;
	char			pidList[256] = "";
	char*			pdata = pidList;
	int32_t			p_id = 0, i;
	uint32_t		count = 0;
	char			datetime[DBS_RECORD_LENGHT_TIME+1] = "";
	char			datetime_last[DBS_RECORD_LENGHT_TIME+1] = "";

	
	EXCCHECK( PPRODUCT->ProductGetIdTreeUp(PPRODUCT, product_id, pid, &pidSize));
	
	for(i=0; i<pidSize; i++)
	{
		if(i==0)
			pdata += sprintf(pdata, "%d", pid[i]);
		else
			pdata += sprintf(pdata, ",%d", pid[i]);
	}

	if(pidSize==0)
		EXCTHROW( -1, "Product branch not valid!");

	LOCK;

	/* lock database tester.product_counter */
	PDBS_LOCK(LOCK_STR);	

	sprintf(sql, "SELECT pc.product_id,pc.count,pc.time "
				 "FROM product_counter pc "
				 "WHERE pc.product_id IN (%s) "
				 "ORDER BY pc.product_id DESC ",
				 pidList);
	
	EXCCHECK( podbc->ExecSQL(podbc, sql));
	CHECKERR( podbc->Fetch(podbc, TRUE));

	if(error!=ELODBC_SQL_WARN_NO_DATA_FOUND)
	{
		p_id = *((long*)podbc->GetFieldValuePtr(podbc, 1));
		count = *((unsigned long*)podbc->GetFieldValuePtr(podbc, 2));
		strncpy(datetime_last, (char*)podbc->GetFieldValuePtr(podbc, 3), DBS_RECORD_LENGHT_TIME);
	}

	EXCCHECK( podbc->Cancel(podbc));

	if(!p_id)
		EXCTHROW( -1, "Product counter not created for selected productId branch!");
		
	DATETIME_TO_STRING(datetime);
	
	if(datetime_last[0]!='\0')
	{
		SYSTEMTIME		st, 
						st_last;

		DATETIME_FROM_STRING(datetime, st);
		DATETIME_FROM_STRING(datetime_last, st_last);

		if(st.wYear>st_last.wYear || st.wMonth>st_last.wMonth || st.wDay>st_last.wDay)
		{
			sprintf(sql, "UPDATE tester.product_counter "
						 "SET product_counter.count=1, product_counter.time='%s' "
						 "WHERE product_counter.product_id=%d ",
						 datetime,
						 p_id);

			count = 1;
		}
		else
		{
			sprintf(sql, "UPDATE tester.product_counter "
						 "SET product_counter.count=product_counter.count+1, product_counter.time='%s' "
						 "WHERE product_counter.product_id=%d ",
						 datetime,
						 p_id);

			count++;
		}
	}
	else
	{
		sprintf(sql, "UPDATE tester.product_counter "
					 "SET product_counter.count=1, product_counter.time='%s' "
					 "WHERE product_counter.product_id=%d ",
					 datetime,
					 p_id);

		count = 1;
	}
	
	EXCCHECK( podbc->ExecSQL(podbc, sql));
	EXCCHECK( podbc->Cancel(podbc));

	if(pCount) *pCount = count;

Error:
	PDBS_UNLOCK(LOCK_STR);
	UNLOCK;
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_GetBarCodeStatus"
static SElExceptionPtr storage_GetBarCodeStatus(struct _SDBSStorage* me, const char* barCode, bool_t* tested, bool_t* failed)
{
	SElExceptionPtr	pexception = NULL;   
	int32_t			error = 0;
	char			sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr		podbc = PDBS->podbc;
	int32_t			count;

	LOCK;

	/* lock database tester.product_counter */
	PDBS_LOCK(LOCK_STR);	

	if(tested) *tested = FALSE;
	
	if(barCode && *barCode && tested)
	{
		/* count items */
		sprintf(sql, "select ps.failed "
					 "from product_status ps "
					 "where ps.barcode='%s' AND ps.mode_id=%d "
					 "order by ps.time desc ",
					 barCode,
					 ID_MODE_PRODUCTION);
		
		EXCCHECK( podbc->ExecSQL(podbc, sql));
		EXCCHECK( podbc->GetRows(podbc, &count));	
		CHECKERR( podbc->Fetch(podbc, TRUE));
	
		if(failed) /* last result */
			*failed = (int)(*((char*)podbc->GetFieldValuePtr(podbc, 1)));

		EXCCHECK( podbc->Cancel(podbc));

		if(tested && count>0)
			*tested = TRUE;
	}

Error:
	PDBS_UNLOCK(LOCK_STR);
	UNLOCK;
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_TestListGet"
static SElExceptionPtr storage_TestListGet(
	struct _SDBSStorage* me,
	int32_t			product_id,
	int32_t			test_id,
	int32_t			parameter_id,
	SEProductPtr*	eproductlist,
	int32_t*		eproductlistSize
)
{
	SElExceptionPtr	pexception = NULL;   
	int32_t			error = 0;
	char			sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr		podbc = PDBS->podbc;
	int				i = 0, j = 0;
	SEProductPtr	productlist = *eproductlist;
	int32_t			productlistSize = *eproductlistSize;

	LOCK;

	if(product_id==0)
	{
		if(productlist) 
			EXCCHECK( me->TestListFree(me, &productlist, productlistSize));

		productlistSize = 0;

		/* get products */
		sprintf(sql, "select pt.name, pt.product_id from product_types pt "
					 "where (pt.property&%d)>0 and (%d, product_id) "
					 "IN (select ts.tester_id, ts.product_id from test_status ts) "
					 "ORDER BY pt.product_id ",
					 PROPERTY_VALID,
					 PDBS_TESTER_ID);

		EXCCHECK( podbc->ExecSQL(podbc, sql));
		CHECKERR( podbc->Fetch(podbc, TRUE));
		if(error!=ELODBC_SQL_WARN_NO_DATA_FOUND)
		{
			do
			{
				productlist = realloc(productlist, (productlistSize+1)*sizeof(SEProduct));
				memset(productlist+productlistSize, 0, sizeof(SEProduct));
				
				strcpy(productlist[productlistSize].name, (char*)podbc->GetFieldValuePtr(podbc, 1));
				productlist[productlistSize].product_id = *((long*)podbc->GetFieldValuePtr(podbc, 2)); 

				productlistSize++;
			}while(FALSE == podbc->MoveNext(podbc));
		}
		EXCCHECK( podbc->Cancel(podbc));
	}
	else if(product_id!=0 && test_id==0)
	{
		/* get tests */
		sprintf(sql, "select st.name, st.test_id, st.description from test_steps st "
					 "where (st.property&%d)>0 and st.name!='@skip' and (%d,%d,st.test_id) "
					 "IN (select ts.tester_id, ts.product_id, ts.test_id from test_status ts) "
					 "ORDER BY st.name ",
					 PROPERTY_VALID,
					 PDBS_TESTER_ID,
					 product_id);

		EXCCHECK( podbc->ExecSQL(podbc, sql));
		CHECKERR( podbc->Fetch(podbc, TRUE));

		for(i=0; i<productlistSize; i++)
		{
			if(productlist[i].product_id == product_id)
				break;
		}

		if(error!=ELODBC_SQL_WARN_NO_DATA_FOUND 
			&& i<productlistSize 
			&& productlist[i].testSize==0)
		{
			do
			{
				productlist[i].test = (SETestPtr)realloc(productlist[i].test, (productlist[i].testSize+1)*sizeof(SETest));
				memset(productlist[i].test+productlist[i].testSize, 0, sizeof(SETest));

				strcpy(productlist[i].test[productlist[i].testSize].name, (char*)podbc->GetFieldValuePtr(podbc, 1));
				productlist[i].test[productlist[i].testSize].test_id = *((long*)podbc->GetFieldValuePtr(podbc, 2)); 
				strcpy(productlist[i].test[productlist[i].testSize].description, (char*)podbc->GetFieldValuePtr(podbc, 3));

				productlist[i].testSize++;
			}while(FALSE == podbc->MoveNext(podbc));
		}
		EXCCHECK( podbc->Cancel(podbc));
	}
	else if(product_id!=0 && test_id!=0 && parameter_id==0)
	{
		/* get limits */
		sprintf(sql, "select tp.name, tp.parameter_id, tp.value, tp.description from test_parameters tp "
					 "where (tp.property&%d)>0 and (%d,%d,%d,tp.parameter_id) "
					 "IN (select ts.tester_id, ts.product_id, ts.test_id, ts.parameter_id from test_status ts) "
					 "ORDER BY tp.name ",
					 PROPERTY_VALID,
					 PDBS_TESTER_ID,
					 product_id,
					 test_id);

		EXCCHECK( podbc->ExecSQL(podbc, sql));
		CHECKERR( podbc->Fetch(podbc, TRUE));

		for(i=0; i<productlistSize; i++)
		{
			if(productlist[i].product_id==product_id)
			{
				for(j=0; j<productlist[i].testSize; j++)
				{
					 if(productlist[i].test[j].test_id==test_id)
						break;
				}
				break;
			}
		}

		if(error!=ELODBC_SQL_WARN_NO_DATA_FOUND 
			&& i<productlistSize 
			&& j<productlist[i].testSize 
			&& productlist[i].test[j].parameterSize==0)
		{
			do
			{
				productlist[i].test[j].parameter = realloc(productlist[i].test[j].parameter, (productlist[i].test[j].parameterSize+1)*sizeof(SEParam));
				memset(productlist[i].test[j].parameter+productlist[i].test[j].parameterSize, 0, sizeof(SEParam));

				strcpy(productlist[i].test[j].parameter[productlist[i].test[j].parameterSize].name, (char*)podbc->GetFieldValuePtr(podbc, 1));
				productlist[i].test[j].parameter[productlist[i].test[j].parameterSize].parameter_id = *((long*)podbc->GetFieldValuePtr(podbc, 2)); 
				strcpy(productlist[i].test[j].parameter[productlist[i].test[j].parameterSize].value, (char*)podbc->GetFieldValuePtr(podbc, 3));
				strcpy(productlist[i].test[j].parameter[productlist[i].test[j].parameterSize].description, (char*)podbc->GetFieldValuePtr(podbc, 4));

				productlist[i].test[j].parameterSize++;
			}while(FALSE == podbc->MoveNext(podbc));
		}
		EXCCHECK( podbc->Cancel(podbc));
	}

	if(eproductlist) 
		*eproductlist = productlist;
	if(eproductlistSize) 
		*eproductlistSize = productlistSize;

Error:
	UNLOCK;
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_TestListFree"
static SElExceptionPtr storage_TestListFree(
	struct _SDBSStorage* me,
	SEProductPtr*	productlist,
	int32_t			productlistSize
)
{
	SElExceptionPtr	pexception = NULL;   
	int			i, j;

	for(i=0; i<productlistSize; i++)
	{
		for(j=0; j<(*productlist)[i].testSize; j++)
		{
			free((*productlist)[i].test[j].parameter);
			(*productlist)[i].test[j].parameter = NULL;
		}
		free((*productlist)[i].test);
		(*productlist)[i].test = NULL;
	}
	free(*productlist);
	*productlist = NULL;

/* Error: */
	EXCRETHROW( pexception); 
}

static gs_product_id;
static gs_test_id;
static gs_parameter_id;
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_GetTestVType"
static SElExceptionPtr storage_GetTestVType(
	struct _SDBSStorage* me,
	int32_t		product_id,
	int32_t		test_id,
	int32_t		parameter_id,
	int32_t*	vtype,
	int32_t*	count
)
{
	SElExceptionPtr	pexception = NULL;   
	int32_t		error = 0;
	char		sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr	podbc = PDBS->podbc;
	int32_t		acount = 0;
	int32_t		vt;
	
	LOCK;

	gs_product_id = product_id;
	gs_test_id = test_id;
	gs_parameter_id = parameter_id;

	sprintf(sql, "SELECT ts.vtype "
				 "FROM tester.test_status ts "
				 "WHERE ts.tester_id=%d AND ts.product_id=%d AND ts.test_id=%d AND ts.parameter_id=%d "
				 "ORDER BY ts.time DESC "
				 "LIMIT %d ",
				 PDBS_TESTER_ID,
				 product_id,
				 test_id,
				 parameter_id,
				 (*count==0)? 50:*count);

	EXCCHECK( podbc->ExecSQL(podbc, sql));
	CHECKERR( podbc->Fetch(podbc, TRUE));

	if(error==ELODBC_SQL_WARN_NO_DATA_FOUND)
	{
		*count = 0;
		goto Error;
	}

	do
	{
		if(acount==0)
			vt = *((short*)podbc->GetFieldValuePtr(podbc, 1));
			
		if(vt==*((short*)podbc->GetFieldValuePtr(podbc, 1)))
			acount++;
		else
			break;

	}while( podbc->MoveNext(podbc)==0);
	
	*count = acount;
	*vtype = vt;

	EXCCHECK( podbc->Cancel(podbc));

Error:
	UNLOCK;
	EXCRETHROW( pexception); 
}


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_GetTestDValue"
static SElExceptionPtr storage_GetTestDValue(
	struct _SDBSStorage* me,
	double*		value,
	int32_t*	count,
	double* 	max, 
	double* 	min, 
	double* 	avg
)
{
	SElExceptionPtr	pexception = NULL;   
	int32_t		error = 0;
	char		sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr	podbc = PDBS->podbc;
	int32_t		acount = 0;
	double		dvalue;

	LOCK;
	
	sprintf(sql, "SELECT ts.vtype,ts.dvalue "
				 "FROM tester.test_status ts "
				 "WHERE ts.tester_id=%d AND ts.product_id=%d AND ts.test_id=%d AND ts.parameter_id=%d "
				 "ORDER BY ts.time DESC "
				 "LIMIT %d ",
				 PDBS_TESTER_ID,
				 gs_product_id,
				 gs_test_id,
				 gs_parameter_id,
				 (*count==0)? 50:*count);

	EXCCHECK( podbc->ExecSQL(podbc, sql));
	CHECKERR( podbc->Fetch(podbc, TRUE));

	if(error==ELODBC_SQL_WARN_NO_DATA_FOUND)
	{
		if(count) *count = 0;
		goto Error;
	}

	if(min) *min = DBL_MAX;
	if(max) *max = DBL_MIN;
	if(avg) *avg = 0.0;
	
	do
	{
		switch( *((short*)podbc->GetFieldValuePtr(podbc, 1)) )
		{
			case E_DVT_REAL64:
				
				dvalue = *((double*)podbc->GetFieldValuePtr(podbc, 2));
				if(max && dvalue > *max)
					*max = dvalue;
				if(min && dvalue< *min)
					*min = dvalue;
				
				if(avg) *avg += dvalue;
				
				value[acount++] = dvalue;
				break;
		}

	}while( podbc->MoveNext(podbc)==0);
	
	if(count) *count = acount;
	if(avg) *avg /= acount;

	EXCCHECK( podbc->Cancel(podbc));

Error:
	UNLOCK;
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "storage_GetTestIValue"
static SElExceptionPtr storage_GetTestIValue(
	struct _SDBSStorage* me,
	int32_t*		value,
	int32_t*	count,
	int32_t* 	max, 
	int32_t* 	min, 
	double* 	avg
)
{
	SElExceptionPtr	pexception = NULL;   
	int32_t		error = 0;
	char		sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr	podbc = PDBS->podbc;
	int32_t		acount = 0;
	int32_t		ivalue;

	LOCK;
	
	sprintf(sql, "SELECT ts.vtype,ts.ivalue "
				 "FROM tester.test_status ts "
				 "WHERE ts.tester_id=%d AND ts.product_id=%d AND ts.test_id=%d AND ts.parameter_id=%d "
				 "ORDER BY ts.time DESC "
				 "LIMIT %d ",
				 PDBS_TESTER_ID,
				 gs_product_id,
				 gs_test_id,
				 gs_parameter_id,
				 (*count==0)? 50:*count);

	EXCCHECK( podbc->ExecSQL(podbc, sql));
	CHECKERR( podbc->Fetch(podbc, TRUE));

	if(error==ELODBC_SQL_WARN_NO_DATA_FOUND)
	{
		*count = 0;
		goto Error;
	}

	*min = INT_MAX;
	*max = INT_MIN;
	*avg = 0.0;
	
	do
	{
		switch( *((short*)podbc->GetFieldValuePtr(podbc, 1)) )
		{
			case E_DVT_INT32:
				
				ivalue = *((long*)podbc->GetFieldValuePtr(podbc, 2));
				if(ivalue > *max)
					*max = ivalue;
				if(ivalue< *min)
					*min = ivalue;
				
				*avg += (double)ivalue;
				
				value[acount++] = ivalue;
				break;
		}

	}while( podbc->MoveNext(podbc)==0);
	
	*count = acount;
	*avg /= acount;

	EXCCHECK( podbc->Cancel(podbc));

Error:
	UNLOCK;
	EXCRETHROW( pexception); 
}

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
#define __FUNC__ "ConvertTimestampFromANSI"
static SElExceptionPtr ConvertTimestampFromANSI(time_t time, char* timestamp)
{
	SElExceptionPtr	pexception = NULL;   
	struct tm*		ptm = NULL;

	ptm = localtime(&time);
	
	sprintf(timestamp, "%d-%02d-%02d %02d:%02d:%02d", 
			(ptm->tm_year+1900), 
			ptm->tm_mon+1, 
			ptm->tm_mday, 
			ptm->tm_hour,
			ptm->tm_min,
			ptm->tm_sec);

/* Error: */
	EXCRETHROW( pexception); 
}
