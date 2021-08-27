#if !defined(__DBS_LINEEXPORT_H__)
#define __DBS_LINEEXPORT_H__

#include "dbs_define.h"
#include <exception/exception.h>  

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/* output structures for tester parametrization */
typedef struct _SELineParam
{
	char			name[DBS_RECORD_LENGHT_NAME+1];
	char			value[DBS_RECORD_LENGHT_VALUE_EXT+1];
} SELineParam, *SELineParamPtr;

typedef struct _SELineStep
{
	char			name[DBS_RECORD_LENGHT_NAME+1];
	int32_t			fnc_nb;

	int32_t			paramSize;  
	SELineParamPtr	param;
} SELineStep, *SELineStepPtr;

typedef struct _SELineProcess
{
	char			name[DBS_RECORD_LENGHT_NAME+1];
	char			description[DBS_RECORD_LENGHT_NAME+1];
	int32_t			process_nb;

	int32_t			stepSize;
	SELineStepPtr	step;
} SELineProcess, *SELineProcessPtr;

typedef struct _SDBSLineExport          
{
	void*			pdbs;

	/* fnc */
	SElExceptionPtr (*GetProcessSeq)(struct _SDBSLineExport* me, int32_t product_id, SELineProcessPtr* seq, int32_t* seqSize);   
	SElExceptionPtr (*FreeProcessSeq)(struct _SDBSLineExport* me, SELineProcessPtr* seq, int32_t seqSize);

	SElExceptionPtr (*GetProcessSeqXML)(struct _SDBSLineExport* me, int32_t product_id, const char* path);
	SElExceptionPtr (*GetProcessSeqXML2)(struct _SDBSLineExport* me, int32_t product_id, const char* path);

} SDBSLineExport, *SDBSLineExportPtr;

SElExceptionPtr dbslineexport_new(SDBSLineExportPtr* pDBSLineExportPtr, void* pDBS);
SElExceptionPtr dbslineexport_delete(SDBSLineExportPtr* pDBSLineExportPtr);

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__DBS_LINEEXPORT_H__)
