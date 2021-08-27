#if !defined(__DBS_TESTPARAMLIST_H__)
#define __DBS_TESTPARAMLIST_H__

#include "dbs_define.h"
#include <exception/exception.h> 

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

typedef struct _STestParameter
{
   int32_t		product_id;
   int32_t		test_nb;
   char			name[DBS_RECORD_LENGHT_NAME+1];
   char*		description; 
   char			value[DBS_RECORD_LENGHT_VALUE_EXT+1];
   int32_t		vtype;
   char			time[DBS_RECORD_LENGHT_TIME+1];
   int32_t		user_id; 
   int32_t		property;
   uint32_t		parameter_id;
   uint32_t		tester_id;

} STestParameter, *STestParameterPtr;            /* Database struct. TEST_PARAMETERS.DBO */

typedef struct _SDBSTestParamList          
{
	void*				pdbs;
	STestParameterPtr   TestParameter;      /* Database table TEST_PARAMETERS */  
  
	int32_t		TestParameterSize;
  	int32_t		_Allocated;
	bool_t		sort;

	/* Database Connections Fnc */
	SElExceptionPtr (*TestParamRead)(struct _SDBSTestParamList* me, int32_t branch_id); 
	SElExceptionPtr (*TestParamSave)(struct _SDBSTestParamList* me, int32_t branch_id); 
	SElExceptionPtr (*TestParamDelHist)(struct _SDBSTestParamList* me, int32_t branch_id); 

	 /* Test Parameters Edit Fnc */
	SElExceptionPtr (*TestParamInsert)(struct _SDBSTestParamList* me, STestParameter parameter); 
	SElExceptionPtr (*TestParamEdit)(struct _SDBSTestParamList* me, STestParameter parameter);
	SElExceptionPtr (*TestParamEditInsert)(struct _SDBSTestParamList* me, STestParameter parameter); 
	SElExceptionPtr (*TestParamDel)(struct _SDBSTestParamList* me, STestParameter parameter);
	SElExceptionPtr (*TestParamCopy)(struct _SDBSTestParamList* me, int32_t pidSrc[], int32_t pidTrg[], int32_t pidSize);
	
	/* Product Parameters Other Fnc */
	SElExceptionPtr (*TestParamSort)(struct _SDBSTestParamList* me);  
	SElExceptionPtr	(*TestParamSetVType)(struct _SDBSTestParamList* me, STestParameterPtr parameter); 
	SElExceptionPtr (*TestParamGetFreeLimitNb)(struct _SDBSTestParamList* me, STestParameter parameter, int32_t* limit_nb);  

} SDBSTestParamList, *SDBSTestParamListPtr;

SElExceptionPtr dbstestparamlist_new(SDBSTestParamListPtr* pDBSTestParamListPtr, void* pDBS);
SElExceptionPtr dbstestparamlist_delete(SDBSTestParamListPtr* pDBSTestParamListPtr);

/* DBS_TEST_PARAMETER_ERROR */
#define DBS_TEST_PARAMETER_ERROR                    -1000000

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__DBS_TESTPARAMLIST_H__)
