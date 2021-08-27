#if !defined(__DBS_MASTERLIST_H__)
#define __DBS_MASTERLIST_H__

#include "dbs_define.h"
#include <exception/exception.h> 

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

typedef struct _SProductMaster
{
   int32_t      product_id;
   int32_t      master_id;  
   char         master[DBS_RECORD_LENGHT_VALUE+1];
   char         time[DBS_RECORD_LENGHT_TIME+1];
   int32_t      user_id; 
   int32_t      property; 
   uint32_t		parameter_id;
  
} SProductMaster, *SProductMasterPtr;           /* Database struct. PRODUCT_MASTERS.DBO */

typedef struct _SDBSMasterList          
{
	void*				pdbs;
	SProductMasterPtr	ProductMaster;      /* Database table PRODUCT_MASTERS */ 
  
	int32_t  ProductMasterSize; 
  	int32_t  _Allocated;
	bool_t	 sort;

	/* Database Connections Fnc */
	SElExceptionPtr (*ProductMasterRead)(struct _SDBSMasterList* me, int32_t branch_id); 
	SElExceptionPtr (*ProductMasterSave)(struct _SDBSMasterList* me, int32_t branch_id);  
	SElExceptionPtr (*ProductMasterDelHist)(struct _SDBSMasterList* me, int32_t branch_id); 

	/* Product Master Edit Fnc */ 
	SElExceptionPtr (*ProductMasterInsert)(struct _SDBSMasterList* me, SProductMaster master);  
	SElExceptionPtr (*ProductMasterEdit)(struct _SDBSMasterList* me, SProductMaster master, SProductMaster masterold);
	SElExceptionPtr (*ProductMasterDel)(struct _SDBSMasterList* me, SProductMaster master);
	SElExceptionPtr (*ProductMasterGroupDel)(struct _SDBSMasterList* me, SProductMaster master);  
	SElExceptionPtr (*ProductMasterCopy)(struct _SDBSMasterList* me, int32_t pidSrc[], int32_t pidTrg[], int32_t pidSize); 
	SElExceptionPtr (*ProductMasterSort)(struct _SDBSMasterList* me); 
   
	SElExceptionPtr (*ProductMasterGetId)(struct _SDBSMasterList* me, int32_t product_id, const char* master_code, int32_t* master_id); 
	SElExceptionPtr (*ProductMasterGetCode)(struct _SDBSMasterList* me, int32_t product_id, char** master, int32_t* masterSize);
	SElExceptionPtr (*ProductMasterGetBarCodes)(struct _SDBSMasterList* me, int32_t product_id, int32_t* count, char** master);

} SDBSMasterList, *SDBSMasterListPtr;

SElExceptionPtr dbsmasterlist_new(SDBSMasterListPtr* pDBSMasterListPtr, void* pDBS);
SElExceptionPtr dbsmasterlist_delete(SDBSMasterListPtr* pDBSMasterListPtr);

/* DBS_ERROR */
#define DBS_PRODUCT_MASTER_ERROR                    -1000000

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__DBS_MASTERLIST_H__)
