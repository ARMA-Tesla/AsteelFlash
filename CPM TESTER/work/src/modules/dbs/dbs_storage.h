#if !defined(__DBS_STORAGE_H__)
#define __DBS_STORAGE_H__

#include "dbs_define.h"
#include "interval.h"
#include <exception/exception.h> 

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

typedef enum _EIdMode
{
	ID_MODE_PRODUCTION = 1,
	ID_MODE_QUALITY_RETEST,
	ID_MODE_QUALITY_MASTER,
	ID_MODE_EXPERTISE_STEP_MODE,
	ID_MODE_EXPERTISE_GO_MODE,
	ID_MODE_EXPERTISE_SEQUENCE,
	ID_MODE_MASTERPART,
	
	ID_MODE_COUNT
} EIdMode;

typedef enum _EIdShift
{
	ID_SHIFT_NONE = 0,
	ID_SHIFT_1,
	ID_SHIFT_2,
	ID_SHIFT_3,
	ID_SHIFT_4,
	ID_SHIFT_5,
	ID_SHIFT_6,
	ID_SHIFT_7,
	ID_SHIFT_8,
	ID_SHIFT_9,
	
	ID_SHIFT_COUNT
} EIdShift;

typedef enum _EIdPause
{
	ID_PAUSE_OPERATOR = 1,
	ID_PAUSE_DOWNTIME,
	ID_PAUSE_QUALITY,
	ID_PAUSE_MAINTENANCE,
	
	ID_PAUSE_COUNT
} EIdPause;

typedef struct _SStatusItem
{
	time_t		time;
	time_t		time_end;
	uint32_t	tester_id;
	uint32_t	item_id;
	uint32_t	id;
	bool_t		terminated;
	bool_t		opened;

} SStatusItem, *SStatusItemPtr;

typedef struct _SDBSStorage          
{
	void*	pdbs;
	
	/* shift */
	SElExceptionPtr (*StartShift)(struct _SDBSStorage* me, const EIdShift shift_id);
	SElExceptionPtr (*StopShift)(struct _SDBSStorage* me);
	SElExceptionPtr (*IsShiftOpen)(struct _SDBSStorage* me, bool_t* pIsOpen, EIdShift* shift_id);
	SElExceptionPtr (*TerminateShift)(struct _SDBSStorage* me);
	SElExceptionPtr (*GetShiftList)(struct _SDBSStorage* me, SStatusItemPtr* pList, int32_t* pListCount); 
	SElExceptionPtr (*GetSelectedShift)(struct _SDBSStorage* me, time_t start, SStatusItemPtr pList);

	/* pause */
	SElExceptionPtr	(*StartPause)(struct _SDBSStorage* me);
	SElExceptionPtr	(*DefinePause)(struct _SDBSStorage* me, const EIdPause pause_id);
	SElExceptionPtr (*StopPause)(struct _SDBSStorage* me);
	SElExceptionPtr (*IsPauseOpen)(struct _SDBSStorage* me, bool_t* isOpen);
	SElExceptionPtr (*TerminatePause)(struct _SDBSStorage* me);
	SElExceptionPtr (*GetPauseIntervals)(struct _SDBSStorage* me, time_t t1, time_t t2, SIntervalResultPtr* pResult, uint32_t*	pause_time);

	/* operator */
	SElExceptionPtr (*LoginOperator)(struct _SDBSStorage* me, const int32_t user_id);
	SElExceptionPtr (*LogoutOperator)(struct _SDBSStorage* me, const int32_t user_id);
	SElExceptionPtr (*LogoutOperators)(struct _SDBSStorage* me);
	SElExceptionPtr (*IsOperatorLoged)(struct _SDBSStorage* me, const int32_t user_id, bool_t* isLoged);
	SElExceptionPtr (*GetOperatorCount)(struct _SDBSStorage* me, uint32_t* pCount);
    SElExceptionPtr (*TerminateOperator)(struct _SDBSStorage* me);
	SElExceptionPtr (*GetOperatorIntervals)(struct _SDBSStorage* me, time_t t1, time_t t2, SIntervalResultPtr* pResult);

	/* product */
	SElExceptionPtr	(*AddProduct)(struct _SDBSStorage* me, const char* XMLpath, const EIdMode mode_id);
	SElExceptionPtr (*GetProductCount)(struct _SDBSStorage* me, time_t* t1, time_t* t2, uint16_t failed, int32_t mode_id, uint32_t* pCount);
	SElExceptionPtr (*GetRetestedProductCount)(struct _SDBSStorage* me, time_t* t1, time_t* t2, uint16_t failed, int32_t mode_id, uint32_t* pCount);
	SElExceptionPtr (*GetLastTwoProducts)(struct _SDBSStorage* me, time_t start, time_t* t1, time_t* t2);
	SElExceptionPtr (*GetAvgKOSU)(struct _SDBSStorage* me, time_t t1, time_t t2, double* pAvg, uint32_t* pPause);
	SElExceptionPtr (*GetProductCounter)(struct _SDBSStorage* me, int32_t product_id, uint32_t* count);
	SElExceptionPtr (*GetBarCodeStatus)(struct _SDBSStorage* me, const char* barCode, bool_t* tested, bool_t* failed);

	/* test statistic */
	SElExceptionPtr (*TestListGet)(struct _SDBSStorage* me, int32_t product_id, int32_t test_id, int32_t parameter_id,SEProductPtr* productlist, int32_t* productlistSize);
	SElExceptionPtr (*TestListFree)(struct _SDBSStorage* me, SEProductPtr* productlist, int32_t productlistSize);
	SElExceptionPtr (*GetTestVType)(struct _SDBSStorage* me, int32_t product_id, int32_t test_id, int32_t parameter_id, int32_t* vtyp, int32_t* count);
	SElExceptionPtr (*GetTestDValue)(struct _SDBSStorage* me, double* value, int32_t* count, double* max, double* min, double* avg);
	SElExceptionPtr (*GetTestIValue)(struct _SDBSStorage* me, int32_t* value, int32_t* count, int32_t* max, int32_t* min, double* avg);

} SDBSStorage, *SDBSStoragePtr;

SElExceptionPtr dbsstorage_new(SDBSStoragePtr* pDBSStoragePtr, void* pDBS);
SElExceptionPtr dbsstorage_delete(SDBSStoragePtr* pDBSStoragePtr);

#define DBS_STORAGE_WARNING_VALID_AVG_INTERVAL		 10000001

#define DBS_STORAGE_ERROR_NOT_SHIFT_DEFINED			-10000001
#define DBS_STORAGE_ERROR_NOT_PRODUCT_DEFINED		-10000002
#define DBS_STORAGE_ERROR_NOT_DATETIME_DEFINED		-10000003

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__DBS_STORAGE_H__)
