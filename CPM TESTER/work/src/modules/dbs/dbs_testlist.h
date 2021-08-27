#if !defined(__DBS_TESTLIST_H__)
#define __DBS_TESTLIST_H__

#include "dbs_define.h"
#include <exception/exception.h>  

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

typedef enum _ETestStepMove
{
	TSM_PREV = 0,
	TSM_NEXT
} ETestStepMove;

typedef struct _STestStep
{
   int32_t      product_id;
   int32_t      process_nb;
   int32_t      test_nb;
   char         name[DBS_RECORD_LENGHT+1];
   char*        description;
   char         time[DBS_RECORD_LENGHT_TIME+1]; 
   int32_t      user_id;
   int32_t      property;
   uint32_t		test_id;

} STestStep, *STestStepPtr;                    /* Database struct. TEST_STEPS.DBO */

typedef struct _SDBSTestList          
{
	void*			pdbs;
	STestStepPtr	Test;               /* Database table TEST_STEPS */ 
  
	int32_t		TestSize; 
  	int32_t		_Allocated;
	bool_t		sort;

	/* Database Connections Fnc */
	SElExceptionPtr (*TestRead)(struct _SDBSTestList* me, int32_t branch_id); 
	SElExceptionPtr (*TestSave)(struct _SDBSTestList* me, int32_t branch_id); 
	SElExceptionPtr (*TestDelHist)(struct _SDBSTestList* me, int32_t branch_id); 

	/* Tests Edit Fnc */
    SElExceptionPtr (*TestInsert)(struct _SDBSTestList* me, STestStep test); 
    SElExceptionPtr (*TestEdit)(struct _SDBSTestList* me, STestStep test);
    SElExceptionPtr (*TestDel)(struct _SDBSTestList* me, STestStep test); 
    SElExceptionPtr (*TestSkip)(struct _SDBSTestList* me, STestStep test);
    SElExceptionPtr (*TestRemoveSkip)(struct _SDBSTestList* me, STestStep test);
	SElExceptionPtr (*TestCopy)(struct _SDBSTestList* me, int32_t pidSrc[], int32_t pidTrg[], int32_t pidSize);
	SElExceptionPtr (*TestMove)(struct _SDBSTestList* me, STestStep test, ETestStepMove pos, int32_t test_nb); 

	/* Test Other Fnc */  
	SElExceptionPtr (*TestSort)(struct _SDBSTestList* me);  
    SElExceptionPtr (*TestGetName)(struct _SDBSTestList* me, int32_t test_nb, char** name);  
   
} SDBSTestList, *SDBSTestListPtr;

SElExceptionPtr dbstestlist_new(SDBSTestListPtr* pDBSTestListPtr, void* pDBS);
SElExceptionPtr dbstestlist_delete(SDBSTestListPtr* pDBSTestListPtr);

/* DBS_TEST_ERROR */
#define DBS_TEST_ERROR                    -1000000

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__DBS_TESTLIST_H__)
