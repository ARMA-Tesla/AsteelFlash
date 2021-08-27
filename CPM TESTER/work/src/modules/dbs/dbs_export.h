#if !defined(__DBS_EXPORT_H__)
#define __DBS_EXPORT_H__

#include "dbs_define.h"
#include <exception/exception.h>  
#include <time.h>

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/* output structures for tester parametrization */
typedef struct _SEPartDescript
{
   int32_t      product_id;
   char         ProductOrder[DBS_RECORD_LENGHT_VALUE+1];
   char         ProductPartNumber[DBS_RECORD_LENGHT_VALUE+1];
   char			ProductName[DBS_RECORD_LENGHT_VALUE+1];
} SEPartDescript, *SEPartDescriptPtr;

typedef struct _SEParam
{
	char		name[DBS_RECORD_LENGHT_NAME+1];
	char		value[DBS_RECORD_LENGHT_VALUE_EXT+1];
	char*		text;
	char		description[DBS_RECORD_LENGHT_DESCRIPTION+1];
	uint32_t	parameter_id;
	int32_t		vtype;
} SEParam, *SEParamPtr;

typedef struct _SETest
{
	char			name[DBS_RECORD_LENGHT_NAME+1];
	char			description[DBS_RECORD_LENGHT_DESCRIPTION+1];
	int32_t			test_nb;
	uint32_t		test_id;

	int32_t			parameterSize;  
	SEParamPtr	parameter;
} SETest, *SETestPtr;

typedef struct _SEProduct
{
	char			name[DBS_RECORD_LENGHT_NAME+1];
	int32_t			product_id;

	int32_t			testSize;
	SETestPtr		test;
} SEProduct, *SEProductPtr;

typedef struct _SDBSExport          
{
	void*			pdbs;

	/* fnc */
	SElExceptionPtr (*GetParameter)(struct _SDBSExport* me, 
			int32_t product_id, time_t time, SEParamPtr* param, int32_t* paramSize); 
	SElExceptionPtr (*GetSpecParameter)(struct _SDBSExport* me, 
			int32_t product_id, const char* name, char* value, int32_t valueSize);
	SElExceptionPtr (*FreeParameter)(struct _SDBSExport* me, SEParamPtr* param, int32_t paramSize); 
#ifndef _NO_TEST
	SElExceptionPtr (*GetTestSeq)(struct _SDBSExport* me, 
			int32_t product_id, int32_t	process_nb, time_t time, SETestPtr* testseq, int32_t* testseqSize);   
	SElExceptionPtr (*FreeTestSeq)(struct _SDBSExport* me, SETestPtr* ttestseq, int32_t ttestseqSize);
#endif
	SElExceptionPtr (*GetTestSeqXML)(struct _SDBSExport* me,
			int32_t product_id, const char* path);
	SElExceptionPtr (*GetProductDescription)(struct _SDBSExport* me, 
			SEPartDescriptPtr* pdesc, int32_t* pdescSize); 
	SElExceptionPtr (*GetProductIdFromParameter)(struct _SDBSExport* me, 
			char* name, char* value, int32_t* product_id); 

} SDBSExport, *SDBSExportPtr;

SElExceptionPtr dbsexport_new(SDBSExportPtr* pDBSExportPtr, void* pDBS);
SElExceptionPtr dbsexport_delete(SDBSExportPtr* pDBSExportPtr);

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__DBS_EXPORT_H__)
