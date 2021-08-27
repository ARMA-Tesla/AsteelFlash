#if !defined(__DBS_TESTERLIST_H__)
#define __DBS_TESTERLIST_H__

#include "dbs_define.h"
#include <exception/exception.h>

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

typedef struct _STesterItem
{
	int32_t      tester_id;
	int32_t      product_id;
	char*        description; 

} STesterItem, *STesterItemPtr;     /* Database struct. PRODUCT_PARAMETERS.DBO */

typedef struct _SDBSTesterList          
{
	void*			pdbs;
	STesterItemPtr	Tester;   /* Database table PRODUCT_PARAMETERS */

	int32_t			TesterSize;

	/* Database Connections Fnc */
	SElExceptionPtr (*TesterRead)(struct _SDBSTesterList* me); 

	/* Tester Fnc */ 
	SElExceptionPtr (*TesterGet)(struct _SDBSTesterList* me, STesterItemPtr* tester);  
	SElExceptionPtr (*TesterGetSelected)(struct _SDBSTesterList* me, int32_t tester_id, STesterItemPtr* tester);  

} SDBSTesterList, *SDBSTesterListPtr;

SElExceptionPtr dbstesterlist_new(SDBSTesterListPtr* pDBSTesterListPtr, void* pDBS);
SElExceptionPtr dbstesterlist_delete(SDBSTesterListPtr* pDBSTesterListPtr);

/* DBS_ERROR */
#define DBS_TESTER_ERROR		-1000000

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__DBS_TESTERLIST_H__)
