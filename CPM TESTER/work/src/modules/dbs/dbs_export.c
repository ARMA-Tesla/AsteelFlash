#include "dbs.h"
#include "dbs_export.h"
#include <elstring.h>
#include <stdio.h>
#include <time.h>
#include <odbc/elodbc.h>
#include "dbs_define_local.h"

#define DBS_NUMBER_OF_GROUPS         	5

/*-----------------------------------------------------------------------*/
static SElExceptionPtr export_GetParameter(
	struct _SDBSExport* me, 
	int32_t product_id, 
	time_t time, 
	SEParamPtr* param, 
	int32_t* paramSize
); 
static SElExceptionPtr export_GetSpecParameter(
	struct _SDBSExport* me, 
	int32_t product_id, 
	const char* name, 
	char* value,
	int32_t size
);
static SElExceptionPtr export_FreeParameter(
	struct _SDBSExport* me, 
	SEParamPtr* param, 
	int32_t paramSize
);
	
#ifndef _NO_TEST
static SElExceptionPtr export_GetTestSeq(
	struct _SDBSExport* me, 
	int32_t product_id, 
	int32_t	process_nb,
	time_t time, 
	SETestPtr*	testseq, 
	int32_t* testseqSize
);   
static SElExceptionPtr export_FreeTestSeq(
	struct _SDBSExport* me, 
	SETestPtr* testseq, 
	int32_t testseqSize
);
#endif
static SElExceptionPtr export_GetTestSeqXML(
	struct _SDBSExport* me, 
	int32_t product_id,
	const char* path
);
static SElExceptionPtr export_GetProductDescription(
	struct _SDBSExport* me, 
	SEPartDescriptPtr* pdesc, 	
	int32_t* pdescSize
);
static SElExceptionPtr export_GetProductIdFromParameter(
	struct _SDBSExport* me, 
	char* 		sname, 
	char* 		svalue, 
	int32_t* 	product_id
);
/*-----------------------------------------------------------------------*/
static SElExceptionPtr export_fncGetParameter(
	struct _SDBSExport* me, 
	int32_t product_id, 
	SEParamPtr* param, 
	int32_t* paramSize
);  
static SElExceptionPtr export_fncGetParameterHist(
	struct _SDBSExport* me, 
	int32_t product_id, 
	time_t time, 
	SEParamPtr* 	param, 
	int32_t* paramSize
); 
#ifndef _NO_TEST
static SElExceptionPtr export_fncGetTestSeq(
	struct _SDBSExport* me, 
	int32_t product_id, 
	int32_t	process_nb, 
	SETestPtr* testseq, 
	int32_t* testseqSize
); 
static SElExceptionPtr export_fncGetTestSeqHist(
	struct _SDBSExport* me, 
	int32_t product_id, 
	time_t time, 
	SETestPtr* testseq, 
	int32_t* testseqSize
);  
	static SElExceptionPtr export_fncTestParameter(
	struct _SDBSExport* me, 
	int32_t product_id, 
	SETestPtr seqtest, 
	int32_t* pid, 
	int32_t pidSize
);
static SElExceptionPtr export_fncTestParameterHist(
	struct _SDBSExport* me, 
	int32_t product_id, 
	time_t time,
	SETestPtr seqtest,
	int32_t* pid, int32_t pidSize
);
#endif
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbsexport_new"
SElExceptionPtr dbsexport_new(SDBSExportPtr* pDBSExportPtr, void* pDBS)
{
	SElExceptionPtr    pexception = NULL; 
	SDBSExportPtr	me = NULL;

	me = calloc(1,sizeof(SDBSExport));
	EXCCHECKALLOC(me);
	
	if(pDBSExportPtr) *pDBSExportPtr = me;

	me->GetParameter 	= export_GetParameter;
	me->GetSpecParameter = export_GetSpecParameter;
	me->FreeParameter	= export_FreeParameter;
#ifndef _NO_TEST
	me->GetTestSeq 		= export_GetTestSeq;
	me->FreeTestSeq 	= export_FreeTestSeq; 
#endif
	me->GetTestSeqXML	= export_GetTestSeqXML;
	me->GetProductIdFromParameter = export_GetProductIdFromParameter; 
	me->GetProductDescription = export_GetProductDescription; 

	me->pdbs = pDBS;

Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbsexport_delete"
SElExceptionPtr dbsexport_delete(SDBSExportPtr* pDBSExportPtr)
{
	if (pDBSExportPtr && *pDBSExportPtr)
	{
		free(*pDBSExportPtr);
		*pDBSExportPtr = NULL;
	}

/* Error: */
	return NULL;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "export_GetProductDescription"
static SElExceptionPtr export_GetProductDescription(
	struct _SDBSExport* me, 
	SEPartDescriptPtr* pdesc, 
	int32_t* pdescSize
)
{
	int32_t				error = 0;
	SElExceptionPtr    	pexception = NULL;
	SOdbcPtr			podbc = PDBS->podbc;
	char				sql[SQL_COMMAND_LENGHT];  	
	int32_t				count, index;                    
	SEPartDescriptPtr 	partdesc = NULL; 
	int32_t 			partdescSize;

	LOCK;

	/* count items */
	sprintf(sql, "SELECT COUNT(pp.product_id) "
				 "FROM tester.product_parameters pp "
				 "WHERE pp.name='ProductName' "
				 "AND (pp.property&%d)>0 "
				 "AND pp.value!='#' "
				 "ORDER BY pp.product_id",
				 PROPERTY_VALID);
	
	EXCCHECK( podbc->ExecSQL( podbc, sql));
	CHECKERR( podbc->Fetch( podbc, TRUE));
	sscanf(podbc->GetFieldValuePtr(podbc, 1), "%d", &count);
	EXCCHECK( podbc->Cancel(podbc));
	
	/* memory allocation */
	partdesc = calloc(count, sizeof(SEPartDescript));
	partdescSize = count;
	
	/* select item name */
	sprintf(sql, "SELECT pp.product_id,pp.value,pp2.value,pp3.value "
				 "FROM product_parameters pp INNER JOIN product_parameters pp2 INNER JOIN product_parameters pp3 "
				 "WHERE pp.name='ProductOrder' "
				 "AND pp2.name='ProductPartNumber' "
				 "AND pp3.name='ProductName' "
				 "AND pp.product_id=pp2.product_id AND pp2.product_id=pp3.product_id "
				 "AND (pp.property&%d)>0 " 			
				 "AND (pp2.property&%d)>0 " 			
				 "AND (pp3.property&%d)>0 " 			
				 "AND pp.value!='#' "
				 "ORDER BY pp.product_id",
				 PROPERTY_VALID,
				 PROPERTY_VALID,
				 PROPERTY_VALID);
	
	EXCCHECK( podbc->ExecSQL( podbc, sql));
	CHECKERR( podbc->Fetch( podbc, TRUE));
	
	for(index=0;index<count;index++)
	{
		partdesc[index].product_id = *((short*)podbc->GetFieldValuePtr(podbc, 1));
		strncpy(partdesc[index].ProductOrder, (char*)podbc->GetFieldValuePtr(podbc, 2), DBS_RECORD_LENGHT_VALUE);
		strncpy(partdesc[index].ProductPartNumber, (char*)podbc->GetFieldValuePtr(podbc, 3), DBS_RECORD_LENGHT_VALUE);
		strncpy(partdesc[index].ProductName, (char*)podbc->GetFieldValuePtr(podbc, 4), DBS_RECORD_LENGHT_VALUE);
		CHECKERR( podbc->MoveNext(podbc));
	}
	
	EXCCHECK( podbc->Cancel(podbc));
	
	*pdesc = partdesc;
	*pdescSize = partdescSize;
	
Error:
	UNLOCK;
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "export_GetProductIdFromParameter"
static SElExceptionPtr export_GetProductIdFromParameter(
	struct _SDBSExport* me, 
	char* 		sname, 
	char* 		svalue, 
	int32_t* 	product_id
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			productId = 0;
	bool_t			validId;
	int				i, j, k;
	int32_t			count = 0;
	int32_t			pid[32];
	int32_t			pidSize = 32;
	
	*product_id = 0;
		
	for(i=0; i<PPARAM->ProductParameterSize; i++)  
	{
		if( (PPARAM->ProductParameter[i].property&PROPERTY_VALID)>0
			&& 0 == strcmp(PPARAM->ProductParameter[i].name, sname)
			&& 0 == strcmp(PPARAM->ProductParameter[i].value, svalue) )
		{
			productId = PPARAM->ProductParameter[i].product_id;
			
			/* test product_id visiblity in tree structure */
			EXCCHECK( PPRODUCT->ProductGetIdTreeUp(PPRODUCT, productId, pid, &pidSize) );
			
			validId = FALSE; 
			for(k=0; k<pidSize; k++)
			{
				for(j=0; j<PPRODUCT->ProductSize; j++)
				{
					if( (PPRODUCT->Product[j].property&PROPERTY_VALID)>0
						&& (PPRODUCT->Product[j].property&PROPERTY_VISIBLE)>0
						&& PPRODUCT->Product[j].product_id == pid[k] )
					{
						validId = TRUE;
						break;
					}
				}
				if(j==PPRODUCT->ProductSize)
				{
					validId = FALSE;
					break;
				}
			}
			
			if(validId)
			{
				*product_id	= productId;
				count++;
			}
		}
	}
	
	if(count>1)
		*product_id = -1;	
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "export_GetParameter"
static SElExceptionPtr export_GetParameter(
	struct _SDBSExport* me, 
	int32_t 		product_id, 
	time_t 			time,
	SEParamPtr* 	param, 
	int32_t* 		paramSize
)
{
	SElExceptionPtr    pexception = NULL;

	if(!time)
	{
		EXCCHECK( PPARAM->ProductParamSort(PPARAM) );
		EXCCHECK( export_fncGetParameter(me, product_id, param, paramSize) );
	}
	else
		EXCCHECK( export_fncGetParameterHist(me, product_id, time, param, paramSize) ); 
		
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "export_FreeParameter"
static SElExceptionPtr export_FreeParameter(
	struct _SDBSExport* me, 
	SEParamPtr* param, 
	int32_t paramSize
)
{
	SElExceptionPtr	pexception = NULL;   	
	int32_t			i;

	if(param)
	{
		for(i=0; i<paramSize; i++)
		{
			free((*param)[i].text);
		}

		free(*param);	
		*param = NULL;
	}

/* Error: */
	EXCRETHROW( pexception); 
}

#ifndef _NO_TEST
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "export_GetTestSeq"
static SElExceptionPtr export_GetTestSeq(
	struct _SDBSExport* me, 
	int32_t 	product_id,
	int32_t		process_nb,
	time_t		time,
	SETestPtr*	testseq, 
	int32_t* 	testseqSize
)
{												 

	SElExceptionPtr    pexception = NULL;   	
	
	if(!time) 
	{
		EXCCHECK( PTESTPARAM->TestParamSort(PTESTPARAM) );
		EXCCHECK( export_fncGetTestSeq(me, product_id, process_nb, testseq, testseqSize) );
	}
	else
		EXCCHECK( export_fncGetTestSeqHist(me, product_id, time, testseq, testseqSize) );  
		
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "export_FreeTestSeq"
static SElExceptionPtr export_FreeTestSeq(
	struct _SDBSExport* me,
	SETestPtr*	testseq, 
	int32_t		testseqSize
)
{
	SElExceptionPtr    pexception = NULL;
	SETestPtr 	ptestseq = NULL;
	int			i;
	
	if(testseq && *testseq)
	{
	   	ptestseq = *testseq;
 		for(i=0; i<testseqSize; i++)
		{
			if(ptestseq[i].parameter)
			{
				free(ptestseq[i].parameter);
				ptestseq[i].parameter = NULL;
				ptestseq[i].parameterSize = 0;
			}
		}
		free(ptestseq);
		ptestseq = NULL;
	}

/* Error: */
	EXCRETHROW( pexception); 
}
#endif

/*---------------------------------------------------------------------------*/
/* TESTER FNC ****************************************************************/ 
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "export_fncGetParameter"
static SElExceptionPtr export_fncGetParameter(
	struct _SDBSExport* me, 
	int32_t		product_id, 
	SEParamPtr* eparam, 
	int32_t*	eparamSize
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			pid[DBS_NUMBER_OF_GROUPS];
	int32_t			pidSize = DBS_NUMBER_OF_GROUPS;
	int				i, j;
	int				param_count = 0; 
	SEParamPtr		param = NULL;  
	
	/* select specified product tree branch */
	EXCCHECK( PPRODUCT->ProductGetIdTreeUp(
									PPRODUCT,
									product_id, 	
									pid, 
									&pidSize) );
	
	/* precount parameters */
	for(i=0; i<PPARAM->ProductParameterSize; i++)   			
	{
		for(j=pidSize-1; j>=0; j--)
		{
			if( (PPARAM->ProductParameter[i].property&PROPERTY_VALID)>0
				&& PPARAM->ProductParameter[i].product_id == pid[j] )
			{
				param_count++;
				break;
			}
		}
	}
	
	if(param_count==0)
	{
		if(eparam) *eparam = NULL;
		if(eparamSize) *eparamSize = 0;
		goto Error;
	}

	param = calloc(param_count, sizeof(SEParam));
	
	/* list parameters */
	param_count = 0;
	for(i=0; i<PPARAM->ProductParameterSize; i++)   			
	{
		for(j=0; j<pidSize; j++)
		{
			if( (PPARAM->ProductParameter[i].property&PROPERTY_VALID)>0
				&& PPARAM->ProductParameter[i].product_id == pid[j] )
			{
				if(	param_count>0
					&& 0==strcmp(PPARAM->ProductParameter[i].name, param[param_count-1].name))
				{
					param_count--;
				}
				strncpy(param[param_count].name, PPARAM->ProductParameter[i].name, DBS_RECORD_LENGHT_NAME);
				strncpy(param[param_count].value, PPARAM->ProductParameter[i].value, DBS_RECORD_LENGHT_VALUE_EXT);
				strncpy(param[param_count].description, GET_STRING(PPARAM->ProductParameter[i].description), DBS_RECORD_LENGHT_DESCRIPTION);
				param[param_count].vtype = PPARAM->ProductParameter[i].vtype;
				if(param[param_count].text!=NULL)
				{
					free(param[param_count].text);
					param[param_count].text = NULL;
				}
				if(param[param_count].vtype==E_DVT_TEXT)
				{
					size_t length = strlen(GET_STRING(PPARAM->ProductParameter[i].text));
					param[param_count].text = (char*)calloc(length+1, sizeof(char));
					strncpy(param[param_count].text, GET_STRING(PPARAM->ProductParameter[i].text), length);
				}
				param_count++;
				break;
			}
		}

	}
	
	if(eparamSize) *eparamSize = param_count;
	if(eparam) *eparam = param;
		
Error:
	EXCRETHROW( pexception); 
}
#ifndef _NO_TEST
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "export_fncGetTestSeq"
static SElExceptionPtr export_fncGetTestSeq(
	struct _SDBSExport* me, 
	int32_t		product_id, 
	int32_t		process_nb, 
	SETestPtr*	etestseq, 
	int32_t*	etestseqSize
)   
{
	SElExceptionPtr	pexception = NULL;    
	int32_t			pid[DBS_NUMBER_OF_GROUPS];
	int32_t			pidSize = DBS_NUMBER_OF_GROUPS;
	int				i, j;
	int				test_count = 0;
	SETestPtr		pseqtest = NULL;
	
	/* select specified product tree branch */
	EXCCHECK( PPRODUCT->ProductGetIdTreeUp(
									PPRODUCT,
									product_id, 	
									pid, 
									&pidSize) );
	
	/* precount tests */ 
	for(i=0; i<PTEST->TestSize; i++)   			
	{
		if( PTEST->Test[i].process_nb == process_nb )
		{
			for(j=pidSize-1; j>=0; j--)
			{
				if( (PTEST->Test[i].property&PROPERTY_VALID)>0
					&&pid[j]==PTEST->Test[i].product_id)
				{
				test_count++;
				break;
				}
			}
		}
	}

	if(test_count==0)
	{
		if(etestseq) *etestseq = NULL;
		if(etestseqSize) *etestseqSize = 0;
		goto Error;
	}

	pseqtest = (SETestPtr)calloc(test_count, sizeof(SETest));
	
	/* list tests */ 
	test_count = 0;
	for(i=0; i<PTEST->TestSize; i++)   			
	{
		if( PTEST->Test[i].process_nb == process_nb )
		{
			for(j=pidSize-1; j>=0; j--)
			{
				if(	(PTEST->Test[i].property&PROPERTY_VALID)>0								    
				&& pid[j]==PTEST->Test[i].product_id )
				{
					if(	test_count>0
					&& 0==strcmp(PTEST->Test[i].name, "@skip")
					&& PTEST->Test[i].test_nb == pseqtest[test_count-1].test_nb )
					{
						test_count--;
					}
					else
					{
					strncpy(pseqtest[test_count].name, PTEST->Test[i].name, DBS_RECORD_LENGHT_NAME);
					strncpy(pseqtest[test_count].description, GET_STRING(PTEST->Test[i].description), DBS_RECORD_LENGHT_DESCRIPTION);
					pseqtest[test_count].test_nb = PTEST->Test[i].test_nb;
					pseqtest[test_count].test_id = PTEST->Test[i].test_id;
					
						if(pseqtest[test_count].parameter)
						{					
						free(pseqtest[test_count].parameter);
						pseqtest[test_count].parameter = NULL;
						pseqtest[test_count].parameterSize = 0;
						}

					EXCCHECK(export_fncTestParameter(me, 
													product_id, 
													&pseqtest[test_count], 
													pid, 
													pidSize) );
					test_count++; 
					}
				break;
				}
			}
		}
	}
	
	if(etestseqSize) *etestseqSize = test_count;
	if(etestseq) *etestseq = pseqtest;
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "export_fncTestParameter"
static SElExceptionPtr export_fncTestParameter(
	struct _SDBSExport* me,
	int32_t		product_id, 
	SETestPtr	pseqtest, 
	int32_t*	pid, 
	int32_t		pidSize
)
{
	SElExceptionPtr	pexception = NULL;    
	int				i, j;
	int				param_count = 0;
	SEParamPtr		param = NULL; 
	
	/* precount parameters */
	for(i=0; i<PTESTPARAM->TestParameterSize; i++)   			
	{
		if( (PTESTPARAM->TestParameter[i].property&PROPERTY_VALID)>0
			&& pseqtest->test_nb == PTESTPARAM->TestParameter[i].test_nb)
		{
			for(j=pidSize-1; j>=0; j--)
			{
				if(	pid[j]==PTESTPARAM->TestParameter[i].product_id )
				{
					param_count++;
					break;
				}
			}
		}
	}
	
	if(param_count==0)
	{
		if(pseqtest) pseqtest->parameter = NULL;
		if(pseqtest) pseqtest->parameterSize = 0;
		goto Error;
	}

	param = (SEParamPtr)calloc(param_count, sizeof(SEParam));
	
	/* list parameters */
	param_count = 0;
	for(i=0; i<PTESTPARAM->TestParameterSize; i++)   			
	{
		if( (PTESTPARAM->TestParameter[i].property&PROPERTY_VALID)>0
			&& pseqtest->test_nb == PTESTPARAM->TestParameter[i].test_nb)
		{
			for(j=pidSize-1; j>=0; j--)
			{
				if(	pid[j]==PTESTPARAM->TestParameter[i].product_id )
				{
					if(	param_count>0
						&& 0==strcmp(PTESTPARAM->TestParameter[i].name, 
									 param[param_count-1].name ) )
					{
						param_count--;
					}
					strncpy(param[param_count].name, PTESTPARAM->TestParameter[i].name, DBS_RECORD_LENGHT_NAME);
					strncpy(param[param_count].value, PTESTPARAM->TestParameter[i].value, DBS_RECORD_LENGHT_VALUE_EXT);
					strncpy(param[param_count].description, GET_STRING(PTESTPARAM->TestParameter[i].description), DBS_RECORD_LENGHT_DESCRIPTION);
					param[param_count].parameter_id = PTESTPARAM->TestParameter[i].parameter_id;
					param[param_count].vtype = PTESTPARAM->TestParameter[i].vtype;
					param_count++;
					break;
				}
			}
		}
	}
	
	if(pseqtest) pseqtest->parameterSize = param_count;
	if(pseqtest) pseqtest->parameter = param;
		
Error:
	EXCRETHROW( pexception); 
}
#endif
/*---------------------------------------------------------------------------*/
/****** Get Parameters from History ******************************************/
/*---------------------------------------------------------------------------*/
static time_t GetTimeInt(char* time)
{
   struct tm    date;
   
   sscanf(time, "%d-%02d-%02d %02d:%02d:%02d", 
				&date.tm_year, 
				&date.tm_mon, 
				&date.tm_mday, 
				&date.tm_hour, 
				&date.tm_min, 
				&date.tm_sec);

   date.tm_year -= 1900;
   date.tm_isdst = 0;
   date.tm_wday = 0;
   date.tm_yday = 0;

/* Error: */
   return mktime( &date );
}
/*---------------------------------------------------------------------------*/
/* sort algorithm for product parameter structure array 
 * priority: 	1) name
 *				deleted 2) PROPERTY_VALID
 *				3) product_id
 *				4) time - its important for history data
 */
static int compare_ProductParameter ( const void* a, const void* b ) 
{
	SProductParameterPtr aa, bb;
	
	aa = (SProductParameterPtr)a;
	bb = (SProductParameterPtr)b;
	
	if( 0!=strcmp(aa->name, bb->name) )
	{
		return ( strcmp(aa->name, bb->name) );
	}
	else if( aa->product_id != bb->product_id ) 
	{
		return ( aa->product_id - bb->product_id );  
	}
	else
	{
		return (int)( GetTimeInt(aa->time) - GetTimeInt(bb->time) );
	}
}
#ifndef _NO_TEST
/*---------------------------------------------------------------------------*/
/* sort algorithm for test structure array 
 * priority: 	1) test_nb
 *				deleted 2) PROPERTY_VALID
 *				3) product_id   
 *				4) time - its important for history data    
 */
static int compare_Test ( const void* a, const void* b ) 
{
	STestStepPtr aa, bb;
	
	aa = (STestStepPtr)a;
	bb = (STestStepPtr)b;
	
	if(aa->test_nb != bb->test_nb)
	{
		return ( aa->test_nb - bb->test_nb );
	}
	else if( aa->product_id != bb->product_id )
	{
		return ( aa->product_id - bb->product_id );
	}
	else
	{
		return (int)( GetTimeInt(aa->time) - GetTimeInt(bb->time) );
	}
	
}		

/*---------------------------------------------------------------------------*/
/* sort algorithm for test parameter structure array 
 * priority: 	1) test_nb
 *				deleted 2) PROPERTY_VALID
 *				3) name
 *				4) product_id
 *				5) time - its important for history data    
 */
static int compare_TestParameter ( const void* a, const void* b ) 
{
	STestParameterPtr aa, bb;
	
	aa = (STestParameterPtr)a;
	bb = (STestParameterPtr)b;
	
	if(aa->test_nb != bb->test_nb)
	{
		return ( aa->test_nb - bb->test_nb );
	}
	else if ( 0!=strcmp(aa->name, bb->name) )
	{
		return ( strcmp(aa->name, bb->name) );
	}
	else if( aa->product_id != bb->product_id )
	{
		return ( aa->product_id - bb->product_id );
	}
	else
	{
		return (int)( GetTimeInt(aa->time) - GetTimeInt(bb->time) );
	}
	
}		
#endif
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "export_fncGetParameterHist"
static SElExceptionPtr export_fncGetParameterHist(
	struct _SDBSExport* me, 
	int32_t		product_id, 
	time_t		time, 
	SEParamPtr* eparam, 
	int32_t*	eparamSize
)
{
	SElExceptionPtr	pexception = NULL;    
	int32_t			pid[DBS_NUMBER_OF_GROUPS];
	int32_t			pidSize = DBS_NUMBER_OF_GROUPS;
	int				i, j;
	int				index = 0;
	int				param_count = 0, paramsrc_count = 0;
	SEParamPtr		param = NULL; 
	SProductParameterPtr	pproductparam_src = NULL;    
	
	/* select specified product tree branch */
	EXCCHECK( PPRODUCT->ProductGetIdTreeUp(
									PPRODUCT,
									product_id, 	
									pid, 
									&pidSize) );
	
	/* precount parameters */
	for(i=0; i<PPARAM->ProductParameterSize; i++)   			
	{
		for(j=pidSize-1; j>=0; j--)
		{
			if( PPARAM->ProductParameter[i].product_id == pid[j] 
				&& time >= GetTimeInt(PPARAM->ProductParameter[i].time) )   
			{
				paramsrc_count++;
				break;
			}
		}
	}
	
	if(paramsrc_count==0) /* nothing to do */
	{
		if(eparamSize) *eparamSize	= 0;
		if(eparam) *eparam = NULL;
		goto Error;
	}
	
	/* create data source */
	pproductparam_src = calloc(paramsrc_count, sizeof(SProductParameter));
	
	for(i=0; i<PPARAM->ProductParameterSize; i++)   			
	{
		for(j=pidSize-1; j>=0; j--)
		{
			if( PPARAM->ProductParameter[i].product_id == pid[j] 
				&& time >= GetTimeInt(PPARAM->ProductParameter[i].time) )   
			{
				pproductparam_src[index++] = PPARAM->ProductParameter[i];
				break;
			}
		}
	}
	qsort(pproductparam_src, paramsrc_count, sizeof(SProductParameter), compare_ProductParameter); 
		
	param = calloc(paramsrc_count, sizeof(SEParam));
	
	/* list parameters */
	param_count = 0;
	for(i=0; i<paramsrc_count; i++)   			
	{
		for(j=pidSize-1; j>=0; j--)
		{
			if( pproductparam_src[i].product_id == pid[j] )
			{
				if(	param_count>0
					&& 0==strcmp(pproductparam_src[i].name, 
								 param[param_count-1].name) )
				{
					param_count--;
				}
				if( (pproductparam_src[i].property&PROPERTY_DELETED)==0 )
				{	
					strncpy(param[param_count].name, pproductparam_src[i].name, DBS_RECORD_LENGHT_NAME);
					strncpy(param[param_count].value, pproductparam_src[i].value, DBS_RECORD_LENGHT_VALUE_EXT);
					strncpy(param[param_count].description, GET_STRING(pproductparam_src[i].description), DBS_RECORD_LENGHT_DESCRIPTION);
					param[param_count].vtype = pproductparam_src[i].vtype;
					if(param[param_count].text!=NULL)
					{
						free(param[param_count].text);
						param[param_count].text = NULL;
					}
					if(param[param_count].vtype==E_DVT_TEXT)
					{
						size_t length = strlen(GET_STRING(pproductparam_src[i].text));
						param[param_count].text = (char*)calloc(length+1, sizeof(char));
						strncpy(param[param_count].text, GET_STRING(pproductparam_src[i].text), length);
					}
					param_count++;
				}
				break;
			}
		}
	}
	
	if(eparamSize) *eparamSize	= param_count;
	if(eparam) *eparam = param;
		
Error:
	if(pproductparam_src) free(pproductparam_src);
	EXCRETHROW( pexception); 
}

#ifndef _NO_TEST
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "export_fncGetTestSeqHist"
static SElExceptionPtr export_fncGetTestSeqHist(
	struct _SDBSExport* me, 
	int32_t		product_id, 
	time_t		time, 
	SETestPtr*	etestseq, 
	int32_t*	etestseqSize
)   
{
	SElExceptionPtr	pexception = NULL;    
	int32_t			pid[DBS_NUMBER_OF_GROUPS];
	int32_t			pidSize = DBS_NUMBER_OF_GROUPS;
	int				i, j;
	int				index = 0;
	int				test_count = 0, testsrc_count = 0;
	SETestPtr		pseqtest = NULL;
	STestStepPtr	ptest_src = NULL;
	bool_t			ignored_test_nb = 0;
	
	/* select specified product tree branch */
	EXCCHECK( PPRODUCT->ProductGetIdTreeUp(
									PPRODUCT,
									product_id, 	
									pid, 
									&pidSize) );
	
	/* precount tests */ 
	for(i=0; i<PTEST->TestSize; i++)   			
	{
		for(j=pidSize-1; j>=0; j--)
		{
			if( pid[j]==PTEST->Test[i].product_id
				&& time >= GetTimeInt(PTEST->Test[i].time) )
			{
				testsrc_count++;
				break;
			}
		}
	}

	if(testsrc_count==0) /* nothing to do */
	{
		if(etestseqSize) *etestseqSize = 0;
		if(etestseq) *etestseq = NULL;
		goto Error;
	}

	/* create data source */ 
	ptest_src = calloc(testsrc_count, sizeof(STestStep)); 

	for(i=0; i<PTEST->TestSize; i++)   			
	{
		for(j=pidSize-1; j>=0; j--)
		{
			if( pid[j]==PTEST->Test[i].product_id
				&& time >= GetTimeInt(PTEST->Test[i].time))
			{
				ptest_src[index++] = PTEST->Test[i];  
				break;
			}
		}
	}
	qsort(ptest_src, testsrc_count, sizeof(STestStep), compare_Test);     
	
	pseqtest = calloc(testsrc_count, sizeof(SETest));
	
	/* list tests */ 
	test_count = 0;
	for(i=0; i<testsrc_count; i++)   			
	{
		for(j=0; j<pidSize; j++)
		{
			if(	ptest_src[i].product_id == pid[j] )
			{
				if(	test_count>0	//test deleted
					&& (ptest_src[i].property&PROPERTY_DELETED)>0
					&& 0!=strcmp(ptest_src[i].name, "@skip")
					&& ptest_src[i].test_nb == pseqtest[test_count-1].test_nb)
				{
					test_count--;
					ignored_test_nb = ptest_src[i].test_nb;
				}
				if(ignored_test_nb!=ptest_src[i].test_nb)
				{
					if(	test_count>0    //test skip created
						&& (ptest_src[i].property&PROPERTY_DELETED)==0
						&& 0==strcmp(ptest_src[i].name, "@skip")
						&& ptest_src[i].test_nb == pseqtest[test_count-1].test_nb)
					{
						test_count--;
						break;
					}
					if( test_count>0   //test skip deleted
						&& (ptest_src[i].property&PROPERTY_DELETED)>0 
						&& 0==strcmp(ptest_src[i].name, "@skip") 
						&& ptest_src[i].test_nb == pseqtest[test_count].test_nb)
					{
						test_count++; 
						break;
					}
					if(	test_count>0	//test edited
						&& (ptest_src[i].property&PROPERTY_EDITED)>0
						&& ptest_src[i].test_nb == pseqtest[test_count-1].test_nb)
					{
						test_count--;
					}
					if( (ptest_src[i].property&PROPERTY_DELETED)==0 
						&& 0!=strcmp(ptest_src[i].name, "@skip") )      
					{
						strncpy(pseqtest[test_count].name, ptest_src[i].name, DBS_RECORD_LENGHT_NAME);
						strncpy(pseqtest[test_count].description, GET_STRING(ptest_src[i].description), DBS_RECORD_LENGHT_DESCRIPTION);
						pseqtest[test_count].test_nb = ptest_src[i].test_nb;
						
						if(pseqtest[test_count].parameter)
						{
							free(pseqtest[test_count].parameter);
							pseqtest[test_count].parameter = NULL;
							pseqtest[test_count].parameterSize = 0;
						}
						
						EXCCHECK(export_fncTestParameterHist(me, 
															product_id, 
															time, 
															&pseqtest[test_count], 
															pid, 
															pidSize) );
						test_count++;
					}
				}
				break;
			}
		}
	}
	
	if(etestseqSize) *etestseqSize = test_count;
	if(etestseq) *etestseq = pseqtest;
	
Error:
	if(ptest_src) free(ptest_src);
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "export_fncTestParameterHist"
static SElExceptionPtr export_fncTestParameterHist(
	struct _SDBSExport* me,
	int32_t		product_id,
	time_t		time,
	SETestPtr	pseqtest,
	int32_t*	pid, 
	int32_t		pidSize
)
{
	SElExceptionPtr	pexception = NULL;    
	int				i, j;
	int				index = 0;  
	int				param_count = 0;
	int				testparamsrc_count = 0;
	SEParamPtr		pseqparam = NULL; 
	SEParamPtr		paramhist = NULL;
	STestParameterPtr	ptestparam_src = NULL;  
	int32_t			paramhist_count, last_product_id;
	
	/* precount parameters */
	for(i=0; i<PTESTPARAM->TestParameterSize; i++)   			
	{
		if( pseqtest->test_nb == PTESTPARAM->TestParameter[i].test_nb
			&& time >= GetTimeInt(PTESTPARAM->TestParameter[i].time) )
		{
			for(j=0; j<pidSize; j++)
			{
				if(	pid[j]==PTESTPARAM->TestParameter[i].product_id )
				{
					testparamsrc_count++;
				}
			}
		}
	}
	
	if(testparamsrc_count==0) /* nothing to do */
	{
		if(pseqtest) pseqtest->parameterSize = 0;
		if(pseqtest) pseqtest->parameter = NULL;
		goto Error;
	}

	/* create data source */ 
	ptestparam_src = calloc(testparamsrc_count, sizeof(STestParameter)); 

	for(i=0; i<PTESTPARAM->TestParameterSize; i++)   			
	{
		if( pseqtest->test_nb == PTESTPARAM->TestParameter[i].test_nb
			&& time >= GetTimeInt(PTESTPARAM->TestParameter[i].time) )
		{
			for(j=0; j<pidSize; j++)
			{
				if(	pid[j]==PTESTPARAM->TestParameter[i].product_id )
				{
					ptestparam_src[index++] = PTESTPARAM->TestParameter[i];
				}
			}
		}
	}
	qsort(ptestparam_src, testparamsrc_count, sizeof(STestParameter), compare_TestParameter);     
	
	pseqparam = calloc(testparamsrc_count, sizeof(SEParam));
	paramhist = calloc(pidSize, sizeof(SEParam));
	
	/* list parameters */
	param_count = 0;
	paramhist_count = -1;
	last_product_id = 0;
	for(i=0; i<testparamsrc_count; i++)   			
	{
		if( pseqtest->test_nb == ptestparam_src[i].test_nb
			&& time >= GetTimeInt(ptestparam_src[i].time) )
		{
			for(j=0; j<pidSize; j++)
			{
				if(	pid[j]==ptestparam_src[i].product_id )
				{
					if(	param_count>0
						&& 0==strcmp(ptestparam_src[i].name, pseqparam[param_count-1].name ) )
					{
						param_count--;
					}
					else
					{
						paramhist_count = -1;
						last_product_id = 0;
					}

					if( last_product_id!=ptestparam_src[i].product_id )
						paramhist_count++;	
					
					strncpy(paramhist[paramhist_count].name, ptestparam_src[i].name, DBS_RECORD_LENGHT_NAME);
					strncpy(paramhist[paramhist_count].value, ptestparam_src[i].value, DBS_RECORD_LENGHT_VALUE_EXT);
					strncpy(paramhist[paramhist_count].description, GET_STRING(ptestparam_src[i].description), DBS_RECORD_LENGHT_DESCRIPTION);
					paramhist[paramhist_count].vtype = ptestparam_src[i].vtype;
					
					if( (ptestparam_src[i].property&PROPERTY_DELETED)==0 )
					{	
						strncpy(pseqparam[param_count].name, ptestparam_src[i].name, DBS_RECORD_LENGHT_NAME);
						strncpy(pseqparam[param_count].value, ptestparam_src[i].value, DBS_RECORD_LENGHT_VALUE_EXT);
						strncpy(pseqparam[param_count].description, GET_STRING(ptestparam_src[i].description), DBS_RECORD_LENGHT_DESCRIPTION);
						pseqparam[param_count].vtype = ptestparam_src[i].vtype;
						param_count++;
					}
					else if(paramhist_count>0) 
					{
						/* give old value if exist */
						strncpy(pseqparam[param_count].name, paramhist[paramhist_count-1].name, DBS_RECORD_LENGHT_NAME);
						strncpy(pseqparam[param_count].value, paramhist[paramhist_count-1].value, DBS_RECORD_LENGHT_VALUE_EXT);
						strncpy(pseqparam[param_count].description, GET_STRING(paramhist[paramhist_count-1].description), DBS_RECORD_LENGHT_DESCRIPTION);
						pseqparam[param_count].vtype = ptestparam_src[i].vtype;
						param_count++; 
					}
					
					last_product_id = ptestparam_src[i].product_id;
				}
			}
		}
	}

	if(pseqtest) pseqtest->parameterSize = param_count;
	if(pseqtest) pseqtest->parameter = pseqparam;
		
Error:
	if(ptestparam_src) free(ptestparam_src);
	if(paramhist) free(paramhist);
	EXCRETHROW( pexception); 
}
#endif
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "export_GetTestSeqXML"
static SElExceptionPtr export_GetTestSeqXML(
	struct _SDBSExport* me,
	int32_t product_id,
	const char* path
)
{
	SElExceptionPtr	pexception = NULL;
	SEParamPtr		param = NULL;
	int32_t			paramSize;
	SELStringPtr	elstring = NULL;
	int				i;
	char			row[2048];
	char			*label = NULL, *pdata = NULL;
	FILE			*file = NULL;
	int				step_nb = 0;
	int				process_nb = 0;
	
#ifndef _NO_TEST
	SELineProcessPtr pprocess = NULL;
	SETestPtr		testseq = NULL;
	int32_t			process_size = 0;
	int32_t			testseqSize = 0;
	int32_t			status, limit_nb, j;
	int				process_count = 0;
#endif 

	EXCCHECK( elstring_new("", &elstring));

	EXCCHECK(elstring->Append(elstring, "<testseq>\n") );
/* <product> */
	EXCCHECK(elstring->Append(elstring, "	<product>\n") );
	
	sprintf(row, "		<id>%d</id>\n", product_id);
	EXCCHECK(elstring->Append(elstring, row) );

	sprintf(row, "		<branch_id>%d</branch_id>\n", PDBS->_branch_id);
	EXCCHECK(elstring->Append(elstring, row) );
	
	EXCCHECK(PPRODUCT->ProductGetIdName(PPRODUCT, product_id, &label));
	sprintf(row, "		<name>%s</name>\n", label);
	EXCCHECK(elstring->Append(elstring, row) );

	EXCCHECK(PPRODUCT->ProductGetIdDescription(PPRODUCT, product_id, &label));
	sprintf(row, "		<description>%s</description>\n", GET_STRING(label));
	EXCCHECK(elstring->Append(elstring, row) );
#ifndef _NO_TEST	
	EXCCHECK(elstring->Append(elstring, "		<mode></mode>\n") ); 
	EXCCHECK(elstring->Append(elstring, "		<date_time></date_time>\n") ); 
	EXCCHECK(elstring->Append(elstring, "		<PalettID></PalettID>\n") ); 
	EXCCHECK(elstring->Append(elstring, "		<barcode></barcode>\n") ); 
	EXCCHECK(elstring->Append(elstring, "		<duration></duration>\n") );
	EXCCHECK(elstring->Append(elstring, "		<failed>0</failed>\n") ); 
#endif
	EXCCHECK(elstring->Append(elstring, "	</product>\n") );
/* <global> */
	EXCCHECK(elstring->Append(elstring, "	<global>\n") );
	EXCCHECK(me->GetParameter(me, product_id, 0, &param, &paramSize));

	for(i=0; i<paramSize; i++)
	{
		if(param[i].vtype!=E_DVT_TEXT)
		{
			EXCCHECK(elstring->Append(elstring, "		<parameter>\n") );
			
			sprintf(row, "			<id>0</id>\n");
			EXCCHECK(elstring->Append(elstring, row) );
			sprintf(row, "			<name>%s</name>\n", param[i].name);
			EXCCHECK(elstring->Append(elstring, row) );
			sprintf(row, "			<description>%s</description>\n", GET_STRING(param[i].description));
			EXCCHECK(elstring->Append(elstring, row) );
			
			sprintf(row, "			<value type=\"%s\">%s</value>\n", 
						GET_VTYPE_STRING(param[i].vtype), 
						param[i].value);

			EXCCHECK(elstring->Append(elstring, row));

			EXCCHECK(elstring->Append(elstring, "		</parameter>\n") );
		}
	}
	EXCCHECK(elstring->Append(elstring, "	</global>\n") );

#ifndef _NO_TEST
	
	EXCCHECK( PLINEEXPORT->GetProcessSeq(PLINEEXPORT, product_id, &pprocess, &process_size));
	
	/* <process> */
	for(process_count=0; process_count<process_size; process_count++)
	{
	
	process_nb++;

	sprintf(row, "	<process stage=\"%d\">\n", process_nb);
	EXCCHECK(elstring->Append(elstring, row) );
	sprintf(row, "		<name>%s</name>\n", pprocess[process_count].name);
	EXCCHECK(elstring->Append(elstring, row) );
	sprintf(row, "		<description>%s</description>\n", pprocess[process_count].description);
	EXCCHECK(elstring->Append(elstring, row) );
	EXCCHECK(elstring->Append(elstring, "	<init>\n") );
	EXCCHECK(elstring->Append(elstring, "		<failed>0</failed>\n") ); 
	EXCCHECK(elstring->Append(elstring, "		<duration></duration>\n") ); 
	EXCCHECK(elstring->Append(elstring, "	</init>\n") );

	
	EXCCHECK(me->GetTestSeq(me, product_id, pprocess[process_count].process_nb, 0, &testseq, &testseqSize)); 

	step_nb = 0;
	
	/* <test> */
	for(i=0; i<testseqSize; i++)
	{
		step_nb++;
		
		sprintf(row, "	<test step=\"%d\">\n", step_nb);
		EXCCHECK(elstring->Append(elstring, row) );
		sprintf(row, "		<id>%d</id>\n", testseq[i].test_id);
		EXCCHECK(elstring->Append(elstring, row) );
		sprintf(row, "		<name>%s</name>\n", testseq[i].name);
		EXCCHECK(elstring->Append(elstring, row) );
		sprintf(row, "		<description>%s</description>\n", GET_STRING(testseq[i].description));
		EXCCHECK(elstring->Append(elstring, row) );
		
		EXCCHECK(elstring->Append(elstring, "		<failed>0</failed>\n") );
		EXCCHECK(elstring->Append(elstring, "		<duration></duration>\n") ); 
		EXCCHECK(elstring->Append(elstring, "		<used>0</used>\n") );

		for(j=0; j<testseq[i].parameterSize; j++)
		{
			/* <parameter> */
			if(strncmp(testseq[i].parameter[j].name, "Limit", sizeof("limit")-1)!=0)
			{
				EXCCHECK(elstring->Append(elstring, "		<parameter>\n") );
		
				sprintf(row, "			<id>%d</id>\n", testseq[i].parameter[j].parameter_id);
				EXCCHECK(elstring->Append(elstring, row) );
				sprintf(row, "			<name>%s</name>\n", testseq[i].parameter[j].name);
				EXCCHECK(elstring->Append(elstring, row) );
				sprintf(row, "			<description>%s</description>\n", GET_STRING(testseq[i].parameter[j].description));
				EXCCHECK(elstring->Append(elstring, row) );
				sprintf(row, "			<value type=\"%s\">%s</value>\n", GET_VTYPE_STRING(testseq[i].parameter[j].vtype), testseq[i].parameter[j].value);
				EXCCHECK(elstring->Append(elstring, row) );

				EXCCHECK(elstring->Append(elstring, "		</parameter>\n") );	
			}
		}
	
		for(j=0; j<testseq[i].parameterSize; j++)
		{
			/* <limit> */
			if(strncmp(testseq[i].parameter[j].name, "Limit", sizeof("limit")-1)==0)
			{
				status = sscanf(testseq[i].parameter[j].name, "Limit%02d", &limit_nb);

				if(status==-1)
					limit_nb = 1;	
				
				sprintf(row, "		<measure step=\"%d\">\n", limit_nb);
				EXCCHECK(elstring->Append(elstring, row) );
				sprintf(row, "			<config>%s</config>\n", GET_STRING(testseq[i].parameter[j].description));
				EXCCHECK(elstring->Append(elstring, row) );
				sprintf(row, "			<value type=\"\"></value>\n");
				EXCCHECK(elstring->Append(elstring, row) );
				
				EXCCHECK(elstring->Append(elstring, "			<failed>0</failed>\n") );  
				EXCCHECK(elstring->Append(elstring, "			<duration></duration>\n") ); 
				EXCCHECK(elstring->Append(elstring, "			<unit></unit>\n") ); 
				EXCCHECK(elstring->Append(elstring, "			<used>0</used>\n") );  

				EXCCHECK(elstring->Append(elstring, "			<limit>\n") );
				
				sprintf(row, "				<id>%d</id>\n", testseq[i].parameter[j].parameter_id);
				EXCCHECK(elstring->Append(elstring, row) );
				sprintf(row, "				<name>%s</name>\n", testseq[i].parameter[j].name);
				EXCCHECK(elstring->Append(elstring, row) );

				pdata = strtok(testseq[i].parameter[j].value, ";");
				if(pdata)
				{
					sprintf(row, "				<value1>%s</value1>\n", pdata);
					EXCCHECK(elstring->Append(elstring, row) );
					
					pdata = strtok(NULL, ";");
					if(pdata)
					{
						sprintf(row, "				<value2>%s</value2>\n", pdata);
						EXCCHECK(elstring->Append(elstring, row) );
					}
				}

				EXCCHECK(elstring->Append(elstring, "			</limit>\n") );

				EXCCHECK(elstring->Append(elstring, "		</measure>\n") );	
			}
		}

		EXCCHECK(elstring->Append(elstring, "	</test>\n") );	
	}

	

	EXCCHECK(elstring->Append(elstring, "	<cleanup>\n") );
	EXCCHECK(elstring->Append(elstring, "		<failed>0</failed>\n") ); 
	EXCCHECK(elstring->Append(elstring, "		<duration></duration>\n") ); 
	EXCCHECK(elstring->Append(elstring, "	</cleanup>\n") );
	
	EXCCHECK(elstring->Append(elstring, "	</process>\n") );	
	}
	
	
#endif
	EXCCHECK(elstring->Append(elstring, "</testseq>\n") );

	if(path=='\0')
		file = fopen("c:\\test_form.xml","w");
	else
		file = fopen(path,"w");
	
	if(file)
	{
		fwrite(elstring->GetPtr(elstring), 1, elstring->Size(elstring), file);
		fclose(file);
	}
	else
	{
		EXCTHROW( -1, "Create file line_form.xml Failed");
	}

Error:
	elstring_delete(&elstring);
	me->FreeParameter(me, &param, paramSize);
#ifndef _NO_TEST
	me->FreeTestSeq(me, &testseq, testseqSize);
#endif
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "export_GetSpecParameter"
static SElExceptionPtr export_GetSpecParameter(
	struct _SDBSExport* me, 
	int32_t product_id, 
	const char* name, 
	char* value,
	int32_t size
)
{
	SElExceptionPtr	pexception = NULL;
	SEParamPtr		param = NULL;
	int32_t			paramSize;
	int				i;
	
	if(value) *value = '\0';

	EXCCHECK(me->GetParameter(me, product_id, 0, &param, &paramSize));

	for(i=0; i<paramSize; i++)
	{
		if(0==strcmp(param[i].name, name))
		{
			strncpy(value, param[i].value, size);
			break;
		}
	}

Error:
	me->FreeParameter(me, &param, paramSize);
	EXCRETHROW( pexception); 
}
