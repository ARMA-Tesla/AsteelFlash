#if !defined(__DBS_PARAMLIST_H__)
#define __DBS_PARAMLIST_H__

#include "dbs_define.h"
#include <exception/exception.h> 

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

typedef struct _SProductParameter
{
   int32_t		product_id;
   char			name[DBS_RECORD_LENGHT_NAME+1];
   char*		description; 
   char			value[DBS_RECORD_LENGHT_VALUE_EXT+1];
   char*		text;
   int32_t		vtype;
   char			time[DBS_RECORD_LENGHT_TIME+1];
   int32_t		user_id; 
   int32_t		property;
   uint32_t		parameter_id;
   uint32_t		tester_id;

} SProductParameter, *SProductParameterPtr;     /* Database struct. PRODUCT_PARAMETERS.DBO */

typedef struct _SDBSParamList          
{
	void*					pdbs;
	SProductParameterPtr	ProductParameter;   /* Database table PRODUCT_PARAMETERS */

	int32_t  ProductParameterSize;
	int32_t  _Allocated;
	bool_t	 sort;

	/* Database Connections Fnc */
	SElExceptionPtr (*ProductParamRead)(struct _SDBSParamList* me, int32_t branch_id); 
	SElExceptionPtr (*ProductParamSave)(struct _SDBSParamList* me, int32_t branch_id); 
	SElExceptionPtr (*ProductParamDelHist)(struct _SDBSParamList* me, int32_t branch_id); 

	/* Product Parameters Edit Fnc */ 
    SElExceptionPtr (*ProductParamInsert)(struct _SDBSParamList* me, SProductParameter parameter);  
    SElExceptionPtr (*ProductParamEdit)(struct _SDBSParamList* me, SProductParameter parameter);
    SElExceptionPtr (*ProductParamDel)(struct _SDBSParamList* me, SProductParameter parameter);
	SElExceptionPtr (*ProductParamCopy)(struct _SDBSParamList* me, int32_t pidSrc[], int32_t pidTrg[], int32_t pidSize);  
    SElExceptionPtr (*ProductParamSort)(struct _SDBSParamList* me);  
	
	/* Product Parameters Other Fnc */
	SElExceptionPtr	(*ProductParamSetVType)(struct _SDBSParamList* me, SProductParameterPtr parameter); 

} SDBSParamList, *SDBSParamListPtr;

SElExceptionPtr dbsparamlist_new(SDBSParamListPtr* pDBSParListPtr, void* pDBS);
SElExceptionPtr dbsparamlist_delete(SDBSParamListPtr* pDBSParListPtr);

/* DBS_ERROR */
#define DBS_PRODUCT_PARAMETER_ERROR                    -1000000

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__DBS_PARAMLIST_H__)
