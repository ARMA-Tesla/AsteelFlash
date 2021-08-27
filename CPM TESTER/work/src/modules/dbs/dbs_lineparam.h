#if !defined(__DBS_LINEPARAM_H__)
#define __DBS_LINEPARAM_H__

#include "dbs_define.h"
#include <exception/exception.h> 

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

typedef struct _SLineParam
{
   int32_t		product_id;
   int32_t		fnc_nb;
   char			name[DBS_RECORD_LENGHT_NAME+1];
   char			value[DBS_RECORD_LENGHT_DESCRIPTION+1]; 
   int32_t		property;
   uint32_t		parameter_id;
   char         time[DBS_RECORD_LENGHT_TIME+1]; 
   int32_t      user_id;

} SLineParam, *SLineParamPtr;            /* Database struct. LINE_PARAM.DBO */

typedef struct _SDBSLineParamList          
{
	void*				pdbs;
	SLineParamPtr		LineParam;      /* Database table LINE_PARAM */  
  
	int32_t		LineParamSize;
  	int32_t		_Allocated;
	bool_t		sort;

	/* Database Connections Fnc */
	SElExceptionPtr (*LineParamRead)(struct _SDBSLineParamList* me, int32_t branch_id); 
	SElExceptionPtr (*LineParamSave)(struct _SDBSLineParamList* me, int32_t branch_id);

	SElExceptionPtr (*LineParamInsert)(struct _SDBSLineParamList* me, SLineParam parameter);
	SElExceptionPtr (*LineParamEdit)(struct _SDBSLineParamList* me, SLineParam parameter);
	SElExceptionPtr (*LineParamDel)(struct _SDBSLineParamList* me, SLineParam parameter);
	SElExceptionPtr (*LineParamCopy)(struct _SDBSLineParamList* me, int32_t pidSrc[], int32_t pidTrg[], int32_t pidSize);

} SDBSLineParamList, *SDBSLineParamListPtr;

SElExceptionPtr dbslineparamlist_new(SDBSLineParamListPtr* pDBSLineParamListPtr, void* pDBS);
SElExceptionPtr dbslineparamlist_delete(SDBSLineParamListPtr* pDBSLineParamListPtr);

/* DBS_LINE_PARAM_ERROR */
#define DBS_LINE_PARAM_ERROR                    -1000000

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__DBS_LINEPARAM_H__)
