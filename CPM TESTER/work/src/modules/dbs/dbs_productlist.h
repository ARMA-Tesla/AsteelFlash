#if !defined(__DBS_PRODUCTLIST_H__)
#define __DBS_PRODUCTLIST_H__

#include "dbs_define.h"
#include <exception/exception.h>

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

#define PRODUCT_PATH_LEVEL_1         1       
#define PRODUCT_PATH_LEVEL_2         2    
#define PRODUCT_PATH_LEVEL_3         3
#define PRODUCT_PATH_LEVEL_4         4 
   
#define PRODUCT_CP_PRODUCT				(1<<0)
#define PRODUCT_CP_PRODUCT_PARAMETER	(1<<1) 
#define PRODUCT_CP_PRODUCT_MASTER		(1<<2)       
#define PRODUCT_CP_TEST					(1<<3) 
#define PRODUCT_CP_TEST_PARAMETER		(1<<4) 
#define PRODUCT_CP_LINE_PROCESS			(1<<5) 
#define PRODUCT_CP_LINE_STEP			(1<<6) 
#define PRODUCT_CP_LINE_PARAMETER		(1<<7) 

typedef struct _SProductTypes
{
   int32_t      product_id;
   int32_t      parent_id;
   char         name[DBS_RECORD_LENGHT_NAME+1]; 
   char*        description; 
   char         time[DBS_RECORD_LENGHT_TIME+1];
   int32_t      user_id; 
   int32_t      property;
   int32_t		parameter_id;
} SProductTypes, *SProductTypesPtr; /* Database struct. PRODUCT_TYPES.DBO */

typedef struct _SDBSProductList          
{
	void*				pdbs;
	SProductTypesPtr    Product; /* Database table PRODUCT and PRODUCT_TYPES */

	int32_t		ProductSize;
	int32_t		_Allocated;
	bool_t		sort;

	/* Database Connections Fnc */
	SElExceptionPtr (*ProductRead)(struct _SDBSProductList* me, int32_t branch_id); 
	SElExceptionPtr (*ProductSave)(struct _SDBSProductList* me, int32_t branch_id); 

	/* Products Edit Fnc */
	SElExceptionPtr (*ProductNew)(struct _SDBSProductList* me, SProductTypesPtr pproduct);
	SElExceptionPtr (*ProductEdit)(struct _SDBSProductList* me, SProductTypes product); 
#if !defined(_NO_TEST) || !defined(_NO_MASTER)
	SElExceptionPtr (*ProductCopy)(struct _SDBSProductList* me, SProductTypes source, SProductTypes target, int32_t mode);	
	SElExceptionPtr (*ProductDel)(struct _SDBSProductList* me, SProductTypes product);
#endif

	SElExceptionPtr (*ProductGetIdTreeDown)(struct _SDBSProductList* me, int32_t start_pid, int32_t* pid, int32_t* pidSize);
	SElExceptionPtr (*ProductGetIdTreeUp)(struct _SDBSProductList* me, int32_t start_pid, int32_t* pid, int32_t* pidSize);
	SElExceptionPtr (*ProductGetIdTreeUpDown)(struct _SDBSProductList* me, int32_t start_pid, int32_t* pid, int32_t* pidSize);
	SElExceptionPtr (*ProductGetIdName)(struct _SDBSProductList* me, int32_t pid, char** name); 
	SElExceptionPtr (*ProductGetIdDescription)(struct _SDBSProductList* me, int32_t pid, char** description); 
	SElExceptionPtr (*ProductGetIdPath)(struct _SDBSProductList* me, int32_t pid, char* path, int32_t level);   
	SElExceptionPtr (*ProductGetIdFromPath)(struct _SDBSProductList* me, char* path, int32_t* pid);
	SElExceptionPtr (*ProductGetIdFirstValid)(struct _SDBSProductList* me, int32_t* pid);
	SElExceptionPtr (*ProductIsValidEndProduct)(struct _SDBSProductList* me, int32_t pid, bool_t* valid);
	SElExceptionPtr (*ProductIsValidProduct)(struct _SDBSProductList* me, int32_t pid, bool_t* valid);

} SDBSProductList, *SDBSProductListPtr;

SElExceptionPtr dbsproductlist_new(SDBSProductListPtr* pDBSProductListPtr, void* pDBS);
SElExceptionPtr dbsproductlist_delete(SDBSProductListPtr* pDBSProductListPtr);

/* DBS_ERROR */
#define DBS_PRODUCT_ERROR                    -1000000

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__DBS_PRODUCTLIST_H__)
