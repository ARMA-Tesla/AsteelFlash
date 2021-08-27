#if !defined(__DBS_LINESTEP_H__)
#define __DBS_LINESTEP_H__

#include "dbs_define.h"
#include <exception/exception.h> 

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

typedef struct _SLineStep
{
	int32_t		product_id;
	int32_t		process_nb;
	int32_t		fnc_nb;
	char		name[DBS_RECORD_LENGHT_NAME+1];
	int32_t		property;
	uint32_t	fnc_id;
	char		time[DBS_RECORD_LENGHT_TIME+1]; 
	int32_t		user_id;

} SLineStep, *SLineStepPtr;            /* Database struct. LINE_STEP.DBO */

typedef struct _SDBSLineStepList          
{
	void*				pdbs;
	SLineStepPtr		LineStep;      /* Database table LINE_STEP */  
  
	int32_t		LineStepSize;
  	int32_t		_Allocated;
	bool_t		sort;

	/* Database Connections Fnc */
	SElExceptionPtr (*LineStepRead)(struct _SDBSLineStepList* me, int32_t branch_id); 
	SElExceptionPtr (*LineStepSave)(struct _SDBSLineStepList* me, int32_t branch_id);

	/* Steps Edit Fnc */
	SElExceptionPtr (*LineStepInsert)(struct _SDBSLineStepList* me, SLineStep step);
	SElExceptionPtr (*LineStepEdit)(struct _SDBSLineStepList* me, SLineStep step);
	SElExceptionPtr (*LineStepDel)(struct _SDBSLineStepList* me, SLineStep step);  
	SElExceptionPtr (*LineStepSkip)(struct _SDBSLineStepList* me, SLineStep step); 
	SElExceptionPtr (*LineStepSkipRemove)(struct _SDBSLineStepList* me, SLineStep step);
	SElExceptionPtr (*LineStepCopy)(struct _SDBSLineStepList* me, int32_t pidSrc[], int32_t pidTrg[], int32_t pidSize, int32_t processCpy[], int32_t processSize);

} SDBSLineStepList, *SDBSLineStepListPtr;

SElExceptionPtr dbslinesteplist_new(SDBSLineStepListPtr* pDBSLineStepListPtr, void* pDBS);
SElExceptionPtr dbslinesteplist_delete(SDBSLineStepListPtr* pDBSLineStepListPtr);

/* DBS_LINE_STEP_ERROR */
#define DBS_LINE_STEP_ERROR                    -1000000

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__DBS_LINESTEP_H__)
