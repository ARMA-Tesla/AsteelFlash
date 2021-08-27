#if !defined(__DBS_LINEPROCESS_H__)
#define __DBS_LINEPROCESS_H__

#include "dbs_define.h"
#include <exception/exception.h> 

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

typedef struct _SLineProcess
{
   int32_t		product_id;
   int32_t		process_nb;
   char			name[DBS_RECORD_LENGHT_NAME+1];
   char			description[DBS_RECORD_LENGHT_NAME+1]; 
   int32_t		property;
   uint32_t		process_id;
   char         time[DBS_RECORD_LENGHT_TIME+1]; 
   int32_t      user_id;

} SLineProcess, *SLineProcessPtr;            /* Database struct. LINE_PROCESS.DBO */

typedef struct _SDBSLineProcessList          
{
	void*				pdbs;
	SLineProcessPtr		LineProcess;      /* Database table LINE_PROCESS */  
  
	int32_t		LineProcessSize;
  	int32_t		_Allocated;
	bool_t		sort;

	/* Database Connections Fnc */
	SElExceptionPtr (*LineProcessRead)(struct _SDBSLineProcessList* me, int32_t branch_id); 
	SElExceptionPtr (*LineProcessSave)(struct _SDBSLineProcessList* me, int32_t branch_id);

	/* Process Edit Fnc */
	SElExceptionPtr (*LineProcessInsert)(struct _SDBSLineProcessList* me, SLineProcess process);
	SElExceptionPtr (*LineProcessEdit)(struct _SDBSLineProcessList* me, SLineProcess process);
	SElExceptionPtr (*LineProcessDel)(struct _SDBSLineProcessList* me, SLineProcess process); 
	SElExceptionPtr (*LineProcessCopy)(struct _SDBSLineProcessList* me, int32_t pidSrc[], int32_t pidTrg[], int32_t pidSize, int32_t ProcessCpy[], int32_t *ProcessSize);

} SDBSLineProcessList, *SDBSLineProcessListPtr;

SElExceptionPtr dbslineprocesslist_new(SDBSLineProcessListPtr* pDBSLineProcessListPtr, void* pDBS);
SElExceptionPtr dbslineprocesslist_delete(SDBSLineProcessListPtr* pDBSLineProcessListPtr);

/* DBS_LINE_PROCESS_ERROR */
#define DBS_LINE_PROCESS_ERROR                    -1000000

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__DBS_LINEPROCESS_H__)
