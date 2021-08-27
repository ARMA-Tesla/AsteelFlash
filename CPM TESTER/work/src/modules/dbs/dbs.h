#if !defined(__DBS_H__)
#define __DBS_H__
 
#if defined (_PRODUCT_LINE) || \
	defined(_PRODUCT_LINE_NEW)
	#define _NO_MASTER
	#define _NO_TEST
	#define _NO_USER
#endif

#include <exception/Exception.h>
#include "dbs_productlist.h"
#include "dbs_paramlist.h"
#ifndef _NO_MASTER
	#include "dbs_masterlist.h"
#endif
#ifndef _NO_TEST
	#include "dbs_testlist.h"
	#include "dbs_testparamlist.h"
#endif
#ifndef _NO_USER
	#include "dbs_userlist.h"
#endif
#include "dbs_testerlist.h"
#include "dbs_export.h"
#include "dbs_storage.h"
#include "dbs_kosu.h"
#include "dbs_lineprocess.h"
#include "dbs_linestep.h"
#include "dbs_lineparam.h"
#include "dbs_lineexport.h"

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif	

#define TESTER_ID	1

enum _EDBSValueType{
    E_DVT_INT32,
    E_DVT_REAL64,
    E_DVT_STRING,
	E_DVT_TEXT
} EDBSValueType;

/* DBS structure */
typedef struct _SDBS           
{
	void*		pdbs;		
	void*		podbc;

	SDBSTesterListPtr      	TesterList;
#ifndef _NO_USER
	SDBSUserListPtr         UserList;
#endif
	SDBSProductListPtr      ProductList;
	SDBSParamListPtr      	ProductParameterList;
#ifndef _NO_MASTER
	SDBSMasterListPtr     	ProductMasterList;
#endif
#ifndef _NO_TEST
	SDBSTestListPtr         TestList;
	SDBSTestParamListPtr   	TestParameterList;
#endif
	SDBSExportPtr         	Export;
	SDBSStoragePtr         	Storage;
	SDBSKOSUPtr            	Kosu;
	/* Line */
	SDBSLineProcessListPtr	LineProcessList;
	SDBSLineStepListPtr		LineStepList;
	SDBSLineParamListPtr	LineParamList;
	SDBSLineExportPtr		LineExport;

	/* Memory Allocation */
	SElExceptionPtr (*UsedMemory)(struct _SDBS* me, uint32_t* size);

	/* Free Memory */
	char* (*CharAlloc)(char** value, size_t size); /* realloc(); */
	char* (*CharCopy)(char* value);	/* calloc(); strcpy() */
	SElExceptionPtr (*Free)(void* instance); /* free(); */
	
	/* lock */
	SElExceptionPtr (*fncLock)(struct _SDBS* me);
	SElExceptionPtr (*fncUnLock)(struct _SDBS* me); 
	
	/* Database Connections Fnc */
	SElExceptionPtr (*Initialize)(struct _SDBS* me, const char* xml_config);
	SElExceptionPtr (*CheckSchema)(struct _SDBS* me);
	SElExceptionPtr (*Read)(struct _SDBS* me);
	SElExceptionPtr (*ReadLine)(struct _SDBS* me);
	SElExceptionPtr (*Save)(struct _SDBS* me);
	SElExceptionPtr (*SaveLine)(struct _SDBS* me);
	SElExceptionPtr (*DelHist)(struct _SDBS* me);
	SElExceptionPtr (*GetUser)(struct _SDBS* me, char* user);
	SElExceptionPtr (*Lock)(struct _SDBS* me, const char* lock_name);
	SElExceptionPtr (*Unlock)(struct _SDBS* me, const char* lock_name);

	/* change branch_id ~ a start product_id for selected tester_id (only for root user)*/
	SElExceptionPtr (*ChangeBranchId)(struct _SDBS* me, int32_t branch_id, bool_t* reloaded);

/* PRIVAT: */
	bool_t				data_changed;
	bool_t				data_changed_line;
	int32_t				TesterProductId;  
	int32_t				_branch_id;
	int32_t				_tester_id;
	CRITICAL_SECTION	_Lock;
	HANDLE				_hThread; /* prevent connection timeout */
	HANDLE				_hEventStop;	
	void*				LogApi;
	uint32_t			LogChannel;
	char				_odbc[256];
	char				_user[256];
	char				_password[256];
	bool_t				is_connected;

} SDBS, *SDBSPtr;

SElExceptionPtr dbs_new(SDBSPtr* pDBSPtr);
SElExceptionPtr dbs_delete(SDBSPtr* pDBSPtr);

/* DBS_ERROR */
#define DBS_ERROR                           -1000000
#define DBS_ERROR_NOT_VALID_PRODUCT_ID      -1000001
#define DBS_ERROR_NOT_VALID_USER			-1000002 
#define DBS_ERROR_DUPLICATE_ITEM			-1000003 
#define DBS_ERROR_DBS_IS_LOCKED				-1000004
#define DBS_ERROR_SCHEMA_NOT_EXIST			-1000010
#define DBS_ERROR_TABLE_NOT_EXIST			-1000011
#define DBS_ERROR_COLUMN_NOT_EXIST			-1000012

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__DBS_H__)
