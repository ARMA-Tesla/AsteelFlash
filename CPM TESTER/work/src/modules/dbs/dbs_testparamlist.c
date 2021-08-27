#include "dbs.h"
#include "dbs_testparamlist.h"
#include <stdio.h>
#include <elstring.h>
#include <odbc/elodbc.h>
#include "dbs_define_local.h"

#define LOCK_STR		"test_parameters"

static SElExceptionPtr param_TestParameterRead(struct _SDBSTestParamList* me, int32_t branch_id);
static SElExceptionPtr param_TestParameterSave(struct _SDBSTestParamList* me, int32_t branch_id); 
static SElExceptionPtr param_TestParameterDelHist(struct _SDBSTestParamList* me, int32_t branch_id); 

static SElExceptionPtr param_TestParameterSort(struct _SDBSTestParamList* me); 

static SElExceptionPtr param_TestParameterEdit(struct _SDBSTestParamList* me, STestParameter parameter); 
static SElExceptionPtr param_TestParameterEditInsert(struct _SDBSTestParamList* me, STestParameter parameter); 
static SElExceptionPtr param_TestParameterInsert(struct _SDBSTestParamList* me, STestParameter parameter);
static SElExceptionPtr param_TestParameterDel(struct _SDBSTestParamList* me, STestParameter parameter);
static SElExceptionPtr param_TestParamCopy(struct _SDBSTestParamList* me, int32_t pidSrc[], int32_t pidTrg[], int32_t pidSize);

static SElExceptionPtr param_TestParamSetVType(struct _SDBSTestParamList* me, STestParameterPtr parameter); 
static SElExceptionPtr param_TestParamGetFreeLimitNb(struct _SDBSTestParamList* me, STestParameter parameter, int32_t* limit_nb);

static SElExceptionPtr param_Insert(struct _SDBSTestParamList* me, STestParameter parameter); 
static SElExceptionPtr param_Allocate(struct _SDBSTestParamList* me, int32_t size);
static SElExceptionPtr param_CheckAlloc(struct _SDBSTestParamList* me);  

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "dbstestparamlist_new"
SElExceptionPtr dbstestparamlist_new(SDBSTestParamListPtr* pDBSTestParamListPtr, void* pDBS)
{
	SElExceptionPtr    pexception = NULL;    
	SDBSTestParamListPtr me = NULL;

	me = calloc(1,sizeof(SDBSTestParamList));
	EXCCHECKALLOC(me);
	
	if(pDBSTestParamListPtr) *pDBSTestParamListPtr = me;
	
	me->TestParamRead = param_TestParameterRead;
	me->TestParamSave = param_TestParameterSave;
	me->TestParamDelHist = param_TestParameterDelHist;

	me->TestParamEdit	 	= param_TestParameterEdit;
	me->TestParamEditInsert = param_TestParameterEditInsert;
	me->TestParamInsert 	= param_TestParameterInsert; 
	me->TestParamDel	 	= param_TestParameterDel;
	me->TestParamCopy = param_TestParamCopy;
	
	me->TestParamSort     = param_TestParameterSort;
	me->TestParamSetVType = param_TestParamSetVType;
	me->TestParamGetFreeLimitNb = param_TestParamGetFreeLimitNb;
	
	me->pdbs = pDBS;
	me->sort = TRUE;

Error:
	EXCRETHROW( pexception);    
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbstestparamlist_delete"
SElExceptionPtr dbstestparamlist_delete(SDBSTestParamListPtr* pDBSTestParamListPtr)
{
	int		i;
	SDBSTestParamListPtr me = *pDBSTestParamListPtr;
	
	if (me)
	{
		for(i=0; i<me->_Allocated; i++)
		{
			if(me->TestParameter[i].description)
			{
				free(me->TestParameter[i].description);
				me->TestParameter[i].description = NULL;
			}
		}
		
		if(me->TestParameter)
		{
			free(me->TestParameter);
			me->TestParameter = NULL;
		}

		free(me);
		me = NULL;
	}

/* Error: */
	return NULL;
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "param_TestParameterRead"
static SElExceptionPtr param_TestParameterRead(struct _SDBSTestParamList* me, int32_t branch_id)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL; 
	int32_t			count, index;
	char			sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr		podbc = PDBS->podbc;
	size_t			desc_len;   
	
	LOCK;

	/* lock database tester.test_parameters */
	PDBS_LOCK(LOCK_STR);
	
	/* select items */
	sprintf(sql, "SELECT tp.product_id,tp.test_nb,tp.name,tp.description,tp.value,tp.time,tp.user_id,tp.property,tp.parameter_id,tp.vtype,tp.tester_id " 
				 "FROM tester.product p INNER JOIN tester.test_parameters tp "
				 "ON p.product_id=tp.product_id "
				 "WHERE p.branch_id=%d AND (tp.tester_id=0 OR tp.tester_id=%d) "
				 "ORDER BY tp.test_nb,tp.name,tp.product_id", 
				 branch_id,
				 PDBS_TESTER_ID);
	EXCCHECK( podbc->ExecSQL(podbc, sql));
	EXCCHECK( podbc->GetRows(podbc, &count));

	/* memory allocation */
	EXCCHECK( param_Allocate(me, count));

	CHECKERR( podbc->Fetch(podbc, TRUE));
	
	for(index=0;index<count;index++)
	{
		STestParameterPtr pparameter = me->TestParameter; 
			
		pparameter[index].product_id = *((long*)podbc->GetFieldValuePtr(podbc, 1));
		pparameter[index].test_nb	= *((long*)podbc->GetFieldValuePtr(podbc, 2)); 
		strncpy(pparameter[index].name, (char*)podbc->GetFieldValuePtr(podbc, 3), DBS_RECORD_LENGHT_NAME);
		
		if(pparameter[index].description!=NULL)
		{
			free(pparameter[index].description);
			pparameter[index].description = NULL;
		}
		
		desc_len = strlen((char*)podbc->GetFieldValuePtr(podbc, 4));
		if(desc_len)
		{
			pparameter[index].description = calloc(desc_len+1, sizeof(char));
			EXCCHECKALLOC(pparameter[index].description);
			strcpy(pparameter[index].description, (char*)podbc->GetFieldValuePtr(podbc, 4));
		}
		strncpy(pparameter[index].value, (char*)podbc->GetFieldValuePtr(podbc, 5), DBS_RECORD_LENGHT_VALUE_EXT);
		strncpy(pparameter[index].time, (char*)podbc->GetFieldValuePtr(podbc, 6), DBS_RECORD_LENGHT_TIME);
		pparameter[index].user_id	= *((long*)podbc->GetFieldValuePtr(podbc, 7));
		pparameter[index].property	= *((short*)podbc->GetFieldValuePtr(podbc, 8));
		pparameter[index].parameter_id	= *((unsigned long*)podbc->GetFieldValuePtr(podbc, 9));
		pparameter[index].vtype	= *((short*)podbc->GetFieldValuePtr(podbc, 10));
		pparameter[index].tester_id	= *((unsigned long*)podbc->GetFieldValuePtr(podbc, 11));

		CHECKERR( podbc->MoveNext(podbc));
	}
	
	me->TestParameterSize = index;
	
	EXCCHECK( podbc->Cancel(podbc));
	
	EXCCHECK( me->TestParamSort(me)); 

Error:
	PDBS_UNLOCK(LOCK_STR);
	UNLOCK;
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "param_TestParameterSave"
static SElExceptionPtr param_TestParameterSave(struct _SDBSTestParamList* me, int32_t branch_id)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	int				i, j;
	char			sql[SQL_COMMAND_LENGHT];  
	SOdbcPtr		podbc = PDBS->podbc;
	SELStringPtr   	pelstring = NULL;
	
	LOCK;

	/* lock database tester.test_parameters */
	PDBS_LOCK(LOCK_STR);

	if(me->TestParameterSize)
	{
		uint32_t last_parameter_id;

		/* get last parameter_id */
		EXCCHECK( podbc->ExecSQL( podbc, "SELECT MAX(tp.parameter_id) FROM tester.test_parameters tp"));
		CHECKERR( podbc->Fetch( podbc, TRUE));
		if(error==ELODBC_SQL_WARN_NO_DATA_FOUND)
			last_parameter_id = 0;
		else
			last_parameter_id = *((unsigned long*)podbc->GetFieldValuePtr(podbc, 1));

		EXCCHECK( podbc->Cancel(podbc));
		
		EXCCHECK( elstring_new(NULL, &pelstring) );
		for(i=0; i<me->TestParameterSize;)
		{	
			EXCCHECK(pelstring->Empty(pelstring) );    
	
			if(me->TestParameter[i].parameter_id==0)
				me->TestParameter[i].parameter_id = ++last_parameter_id;

			sprintf(sql, "REPLACE INTO tester.test_parameters "
						 "(product_id,test_nb,name,description,value,time,user_id,property,parameter_id,vtype,tester_id) "
						 "VALUES (%d,%d,'%s',_UTF8'%s','%s','%s',%d,%d,%d,%d,%d)", 
						 me->TestParameter[i].product_id,
						 me->TestParameter[i].test_nb,
						 me->TestParameter[i].name,
						 GET_STRING(me->TestParameter[i].description),
						 me->TestParameter[i].value,
						 me->TestParameter[i].time,
						 me->TestParameter[i].user_id,
						 me->TestParameter[i].property,
						 me->TestParameter[i].parameter_id,
						 me->TestParameter[i].vtype,
						 me->TestParameter[i].tester_id);
	
			EXCCHECK(pelstring->Append(pelstring, sql) );
		
			i++;
			j = 0;
			
			for(; i<me->TestParameterSize; i++)
			{
				if(++j>SQL_COMMAND_NB_OF_INSERTED_ROW)
					break;
				
				if(me->TestParameter[i].parameter_id==0)
					me->TestParameter[i].parameter_id = ++last_parameter_id;

				sprintf(sql, ",(%d,%d,'%s',_UTF8'%s','%s','%s',%d,%d,%d,%d,%d)", 
							 me->TestParameter[i].product_id,
							 me->TestParameter[i].test_nb,
							 me->TestParameter[i].name,
							 GET_STRING(me->TestParameter[i].description),
							 me->TestParameter[i].value,
							 me->TestParameter[i].time,
							 me->TestParameter[i].user_id,
							 me->TestParameter[i].property,
							 me->TestParameter[i].parameter_id,
							 me->TestParameter[i].vtype,
							 me->TestParameter[i].tester_id);
	
				EXCCHECK(pelstring->Append(pelstring, sql) );
			}

			EXCCHECK( podbc->ExecSQL( podbc, pelstring->GetPtr(pelstring))); 
			EXCCHECK( podbc->Cancel(podbc));
		} 
	}
	
Error:
	PDBS_UNLOCK(LOCK_STR);
	UNLOCK;
	elstring_delete(&pelstring);        
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "param_TestParameterDelHist"
static SElExceptionPtr param_TestParameterDelHist(struct _SDBSTestParamList* me, int32_t branch_id)
{
	SElExceptionPtr	pexception = NULL; 
	SOdbcPtr		podbc = PDBS->podbc;
	char			sql[SQL_COMMAND_LENGHT];  
	
	LOCK;

	sprintf(sql, "DELETE FROM tester.test_parameters "
				 "WHERE (test_parameters.property&%d)=0 AND test_parameters.product_id IN "
				 "(SELECT p.product_id FROM tester.product p WHERE p.branch_id=%d)", 
				 PROPERTY_VALID,
				 branch_id); 
	EXCCHECK( podbc->ExecSQL( podbc, sql)); 
	EXCCHECK( podbc->Cancel(podbc));
	
Error:
	UNLOCK;
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
/* sort algorithm for test parameter structure array 
 * priority: 	1) test_nb
 *				2) PROPERTY_VALID
 *				3) name
 *				4) product_id
 */
int compare_TestParameter ( const void* a, const void* b ) 
{
	STestParameterPtr aa, bb;
	
	aa = (STestParameterPtr)a;
	bb = (STestParameterPtr)b;
	
	if(aa->test_nb != bb->test_nb)
	{
		return ( aa->test_nb - bb->test_nb );
	}
	else if( (aa->property&PROPERTY_VALID) != (bb->property&PROPERTY_VALID) )
	{
		return ( (bb->property&PROPERTY_VALID) - (aa->property&PROPERTY_VALID) );
	}
	else if ( 0!=strcmp(aa->name, bb->name) )
	{
		return ( strcmp(aa->name, bb->name) );
	}
	else
	{
		return ( aa->product_id - bb->product_id );
	}
}		

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "param_Insert"
static SElExceptionPtr param_Insert(struct _SDBSTestParamList* me, STestParameter parameter)
{
	SElExceptionPtr	pexception = NULL; 
	
	/* check free space */
	EXCCHECK( param_CheckAlloc(me) );
	
	if(me->TestParameter[me->TestParameterSize].description)
		free(me->TestParameter[me->TestParameterSize].description);

	/* complete record */
	DATETIME_TO_STRING(parameter.time);
	parameter.parameter_id = 0;

	me->TestParameter[me->TestParameterSize++] = parameter;

	if(me->sort)  
		qsort(me->TestParameter, me->TestParameterSize, sizeof(STestParameter), compare_TestParameter);
	
	PDBS->data_changed	= TRUE; 
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "param_TestParameterSort"
static SElExceptionPtr param_TestParameterSort(struct _SDBSTestParamList* me)
{
	qsort(me->TestParameter, me->TestParameterSize, sizeof(STestParameter), compare_TestParameter);	
	
	return NULL; 
}

/*---------------------------------------------------------------------------*/
/* TEST PARAMETER SECTION ****************************************************/
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "param_TestParameterInsert"
static SElExceptionPtr param_TestParameterInsert(
	struct _SDBSTestParamList* me, 
	STestParameter parameter
)
{
	SElExceptionPtr	pexception = NULL; 
	int				i;
	
	if(0==parameter.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");
		
	/* check duplication */
	for(i=0; i<me->TestParameterSize; i++)
	{
		if(me->TestParameter[i].test_nb == parameter.test_nb
			&& (me->TestParameter[i].property&PROPERTY_VALID)>0
			&& 0==strcmp(me->TestParameter[i].name, parameter.name))
		{
			EXCTHROW(DBS_ERROR_DUPLICATE_ITEM, "Duplicate item!");
		}
	}
	
	/* set new record */
	parameter.property = PROPERTY_CREATED|PROPERTY_VALID;

	EXCCHECK( param_Insert(me, parameter) );
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "param_TestParameterDel"
static SElExceptionPtr param_TestParameterDel(
	struct _SDBSTestParamList* me, 
	STestParameter parameter
)
{
	SElExceptionPtr	pexception = NULL; 
	int				i, j;
	int				pid[1024];
	int				pidSize = 1024;
	
	if(0==parameter.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");
	
	/* search downtree products id */
	EXCCHECK( PPRODUCT->ProductGetIdTreeDown(PPRODUCT, parameter.product_id,pid, &pidSize) );
	
	for(j=0; j<pidSize; j++)
	{
		/* set current parameters not valid */  
		for(i=0; i<me->TestParameterSize;i++)
		{
			if( (me->TestParameter[i].property&PROPERTY_VALID)>0
				&& me->TestParameter[i].test_nb==parameter.test_nb
				&& me->TestParameter[i].product_id == pid[j]
				&& 0==strcmp(me->TestParameter[i].name,parameter.name) )
			{
				me->TestParameter[i].property ^= PROPERTY_VALID;
				strncpy(parameter.value, me->TestParameter[i].value, DBS_RECORD_LENGHT_VALUE_EXT);
				parameter.product_id = me->TestParameter[i].product_id;
				parameter.description = PDBS_COPY(parameter.description);
				break;
			}
		}
		
		if(i<me->TestParameterSize)
		{
			/* set new record */     
			parameter.property = PROPERTY_DELETED;
			
			EXCCHECK( param_Insert(me, parameter) ); 
		}
	}
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "param_TestParamCopy"
static SElExceptionPtr param_TestParamCopy(
	struct _SDBSTestParamList* me, 
	int32_t pidSrc[], 
	int32_t pidTrg[], 
	int32_t pidSize
)
{
	SElExceptionPtr	pexception = NULL; 
	STestParameterPtr	ptestparamnew = NULL;
	STestParameterPtr   pdata = NULL;
	int32_t			tparamSize, i, j, k;   
	
	/* disable sorting algorithm because of program speed */ 
	me->sort = FALSE;
	
	/* precount */
	for(i=0, tparamSize = 0; i<me->TestParameterSize; i++) 
	{
		for(j=0; j<pidSize; j++)
		{
			if( (me->TestParameter[i].property&PROPERTY_VALID)>0
				&&pidSrc[j] == me->TestParameter[i].product_id)
			{
				tparamSize++;
				break;
			}
		}
	}
	
	ptestparamnew = calloc(tparamSize, sizeof(STestParameter) );		
	pdata = ptestparamnew;
	
	for(i=0; i<me->TestParameterSize; i++) 
	{	
		for(j=0; j<pidSize; j++)
		{
			if((me->TestParameter[i].property&PROPERTY_VALID)>0
				&& pidSrc[j] == me->TestParameter[i].product_id)
			{
				*pdata = me->TestParameter[i];
				pdata->test_nb++;
				
				for(k=0; k<pidSize; k++)
				{
					if(pidSrc[k] == me->TestParameter[i].product_id)
					{
						pdata->product_id = pidTrg[k];
						break;
					}
				}
				pdata++;
				break;
			}
		}
	}
	
	/* preallocate memory for a faster process */
	param_Allocate(me, me->TestParameterSize+tparamSize);

	for(i=0; i<tparamSize; i++)
	{
		ptestparamnew[i].description = PDBS_COPY( ptestparamnew[i].description);
		EXCCHECK( param_Insert(me, ptestparamnew[i])); 
	}

	me->sort = TRUE;
	EXCCHECK( param_TestParameterSort(me)); 

Error:
	me->sort = TRUE;
	free(ptestparamnew);
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "param_TestParameterEdit"
static SElExceptionPtr param_TestParameterEdit(
	struct _SDBSTestParamList* me, 
	STestParameter parameter
)
{
	SElExceptionPtr	pexception = NULL; 
	int				i;

	if(0==parameter.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");
		
	/* edit selected parameter */
	for(i=0; i<me->TestParameterSize; i++)
	{
		if( (me->TestParameter[i].property&PROPERTY_VALID)>0
			&& me->TestParameter[i].test_nb == parameter.test_nb
			&& me->TestParameter[i].product_id == parameter.product_id
			&& 0 == strcmp(me->TestParameter[i].name, parameter.name) )
		{
			me->TestParameter[i].property ^= PROPERTY_VALID;   
			break;
		}
	}
	
	/* set new record */     
	parameter.property = PROPERTY_EDITED|PROPERTY_VALID;

	EXCCHECK( param_Insert(me, parameter) );    	
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "param_TestParameterEditInsert"
static SElExceptionPtr param_TestParameterEditInsert(
	struct _SDBSTestParamList* me, 
	STestParameter parameter
)
{
	SElExceptionPtr	pexception = NULL; 
	int				i;
	
	if(0==parameter.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");
	
	/* check duplication */
	for(i=0; i<me->TestParameterSize; i++)
	{
		if( me->TestParameter[i].product_id == parameter.product_id 
			&& me->TestParameter[i].test_nb == parameter.test_nb
			&& (me->TestParameter[i].property&PROPERTY_VALID)>0
			&& 0==strcmp(me->TestParameter[i].name, parameter.name))
		{
			EXCTHROW(DBS_ERROR_DUPLICATE_ITEM, "Duplicate item!");
		}
	}
	
	/* set new record */     
	parameter.property = PROPERTY_CREATED|PROPERTY_VALID;
	
	EXCCHECK( param_Insert(me, parameter) ); 
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "param_TestParamSetVType"
static SElExceptionPtr param_TestParamSetVType(
	struct _SDBSTestParamList* me, 
	STestParameterPtr pparameter
)
{
	SElExceptionPtr	pexception = NULL; 
	int32_t			pid[128];
	int32_t			pidSize = 128;
	int				i, j;
	bool_t			found = FALSE;
	
	EXCCHECK( PPRODUCT->ProductGetIdTreeUp(PPRODUCT, pparameter->product_id, pid, &pidSize));
	
	for(j=pidSize-1; j>=0; j--)
	{
		for(i=0; i<me->TestParameterSize;i++)
		{
			if( (me->TestParameter[i].property&PROPERTY_VALID)>0
			 	 && me->TestParameter[i].product_id == pid[j]
				 && me->TestParameter[i].test_nb == pparameter->test_nb
				 && 0==strcmp(me->TestParameter[i].name,pparameter->name) )
			{
				pparameter->vtype = me->TestParameter[i].vtype;
				found = TRUE;			
			}
		}
	}
	
	if(!found)
		pparameter->vtype = E_DVT_STRING;
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
static int compare_int32(const void * va, const void * vb)
{
	int32_t		a = *(int32_t*)va;
	int32_t		b = *(int32_t*)vb;
	
	return (int)(a - b);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "param_TestParamGetFreeLimitNb"
static SElExceptionPtr param_TestParamGetFreeLimitNb(
	struct _SDBSTestParamList* me, 
	STestParameter parameter, 
	int32_t* limit_nb
)
{
	SElExceptionPtr    pexception = NULL; 
	int32_t		limit[256];
	int32_t		limitIndex = 0;
	int			i;
	
	for(i=0; i<me->TestParameterSize;i++)
	{
		if( (me->TestParameter[i].property&PROPERTY_VALID)>0
		 	 && me->TestParameter[i].product_id == parameter.product_id
			 && me->TestParameter[i].test_nb == parameter.test_nb
			 && 0==strncmp(me->TestParameter[i].name, "limit", strlen("limit")) )
		{
			sscanf(me->TestParameter[i].name, "limit%02d.", &limit[limitIndex++]);			
		}
	}
	
	qsort(limit, limitIndex, sizeof(int32_t), compare_int32);
	
	if(limitIndex==0) /* no record */
	{
		*limit_nb = 1;
	}
	else
	{
		for(i=0; i<limitIndex; i++)
		{
			if(limit[i]!=i+1)
			{
				*limit_nb = i+1;
				break;
			}
		}
		
		if(i==limitIndex)
			*limit_nb = i+1;
	}
	
/* Error: */
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "param_Allocate"
static SElExceptionPtr param_Allocate(
	struct _SDBSTestParamList* me, 
	int32_t size
)  
{
	SElExceptionPtr    pexception = NULL; 

	if(me->_Allocated == 0)
	{
		me->TestParameter = calloc(size+MEMORY_ALLOCATION_RESERVE, sizeof(STestParameter)); 
		EXCCHECKALLOC(me->TestParameter);
		me->_Allocated = size+MEMORY_ALLOCATION_RESERVE;
	}
	else if(size>me->_Allocated)
	{
		me->TestParameter = realloc(me->TestParameter, (size+MEMORY_ALLOCATION_RESERVE)*sizeof(STestParameter)); 
		EXCCHECKALLOC(me->TestParameter);
		memset( me->TestParameter + me->_Allocated, 
				0, 
				(size+MEMORY_ALLOCATION_RESERVE - me->_Allocated)*sizeof(STestParameter));
		me->_Allocated = size+MEMORY_ALLOCATION_RESERVE;
	}
	
Error:
	EXCRETHROW( pexception);    
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "param_CheckAlloc"
static SElExceptionPtr param_CheckAlloc(struct _SDBSTestParamList* me)
{
	SElExceptionPtr    pexception = NULL;
	
	if(me->_Allocated - me->TestParameterSize < MEMORY_ALLOCATION_MIN )
	{
		me->TestParameter = realloc( me->TestParameter, 
									 (me->_Allocated + MEMORY_ALLOCATION_RESERVE)*sizeof(STestParameter));
		EXCCHECKALLOC(me->TestParameter);
		/* clear memory */
		memset( me->TestParameter + me->_Allocated,
				0,
				MEMORY_ALLOCATION_RESERVE*sizeof(STestParameter));
		me->_Allocated += MEMORY_ALLOCATION_RESERVE;
	}
			
Error:
	EXCRETHROW( pexception);    
}
