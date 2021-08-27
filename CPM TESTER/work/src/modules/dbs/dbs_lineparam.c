#include "dbs.h"
#include "dbs_lineparam.h"
#include <stdio.h>
#include <elstring.h>
#include <odbc/elodbc.h>
#include "dbs_define_local.h"

#define LOCK_STR		"line_param"

static SElExceptionPtr param_LineParamRead(struct _SDBSLineParamList* me, int32_t branch_id); 		
static SElExceptionPtr param_LineParamSave(struct _SDBSLineParamList* me, int32_t branch_id);	

static SElExceptionPtr param_LineParamInsert(struct _SDBSLineParamList* me, SLineParam parameter);
static SElExceptionPtr param_LineParamEdit(struct _SDBSLineParamList* me, SLineParam parameter);
static SElExceptionPtr param_LineParamDel(struct _SDBSLineParamList* me, SLineParam parameter);
static SElExceptionPtr param_LineParamCopy(struct _SDBSLineParamList* me, int32_t pidSrc[], int32_t pidTrg[], int32_t pidSize);

static SElExceptionPtr param_CheckAlloc(struct _SDBSLineParamList* me);
static SElExceptionPtr param_Allocate(struct _SDBSLineParamList* me, int32_t size);

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbslineparamlist_new"
SElExceptionPtr dbslineparamlist_new(SDBSLineParamListPtr* pDBSLineParamListPtr, void* pDBS)
{
	SElExceptionPtr			pexception = NULL;
	SDBSLineParamListPtr	me = NULL;

	me = calloc(1,sizeof(SDBSLineParamList));
	EXCCHECKALLOC(me);
	
	*pDBSLineParamListPtr = me;
	
	me->LineParamRead	= param_LineParamRead;
	me->LineParamSave	= param_LineParamSave;

	me->LineParamInsert = param_LineParamInsert;
	me->LineParamEdit = param_LineParamEdit;
	me->LineParamDel = param_LineParamDel;
	me->LineParamCopy = param_LineParamCopy;
	
	me->pdbs = pDBS;
	me->sort = TRUE;

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbslineparamlist_delete"
SElExceptionPtr dbslineparamlist_delete(SDBSLineParamListPtr* pDBSLineParamListPtr)
{
	if (pDBSLineParamListPtr && *pDBSLineParamListPtr)
	{
		SDBSLineParamListPtr	me = *pDBSLineParamListPtr;
		
		if(me->LineParam)
		{
			free(me->LineParam);
			me->LineParam = NULL;
		}

		free(*pDBSLineParamListPtr);
		*pDBSLineParamListPtr = NULL;
	}

/* Error: */
	return NULL;
}

/*---------------------------------------------------------------------------*/  
#undef __FUNC__
#define __FUNC__ "param_LineParamRead"
static SElExceptionPtr param_LineParamRead(struct _SDBSLineParamList* me, int32_t branch_id)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL; 
	int32_t			count, index;
	char			sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr		podbc = PDBS->podbc; 
	
	LOCK;
	
	/* lock database tester.line_parameters */
	PDBS_LOCK(LOCK_STR);

	/* select items */
	sprintf(sql, "SELECT lp.product_id,lp.fnc_nb,lp.name,lp.value,lp.parameter_id,lp.property,lp.time,lp.user_id " 
				 "FROM tester.product p INNER JOIN tester.line_parameters lp "
				 "ON p.product_id=lp.product_id "
				 "WHERE p.branch_id=%d "
				 "ORDER BY lp.fnc_nb,lp.name,lp.product_id", 
				 branch_id);
	EXCCHECK( podbc->ExecSQL(podbc, sql));
	EXCCHECK( podbc->GetRows(podbc, &count));

	/* memory allocation */
	EXCCHECK( param_Allocate(me, count));

	CHECKERR( podbc->Fetch( podbc, TRUE));
	
	for(index=0;index<count;index++)
	{
		SLineParamPtr pparam = me->LineParam;
	
		pparam[index].product_id = *((long*)podbc->GetFieldValuePtr(podbc, 1));
		pparam[index].fnc_nb = *((long*)podbc->GetFieldValuePtr(podbc, 2));
		strcpy(pparam[index].name, (char*)podbc->GetFieldValuePtr(podbc, 3));
		strcpy(pparam[index].value, (char*)podbc->GetFieldValuePtr(podbc, 4));
		pparam[index].parameter_id	= *((long*)podbc->GetFieldValuePtr(podbc, 5));
		pparam[index].property 	= *((short*)podbc->GetFieldValuePtr(podbc, 6));
		strcpy(pparam[index].time, (char*)podbc->GetFieldValuePtr(podbc, 7));
		pparam[index].user_id	= *((long*)podbc->GetFieldValuePtr(podbc, 8));

		CHECKERR( podbc->MoveNext(podbc));
	}
	
	me->LineParamSize = index;
	
	EXCCHECK( podbc->Cancel(podbc));
	
Error:
	PDBS_UNLOCK(LOCK_STR);
	UNLOCK;
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "param_LineParamSave"
static SElExceptionPtr param_LineParamSave(struct _SDBSLineParamList* me, int32_t branch_id)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL;
	int				i, j;
	char			sql[SQL_COMMAND_LENGHT] = "";  
	SOdbcPtr		podbc = PDBS->podbc;
	SELStringPtr   	pelstring = NULL;

	LOCK;

	/* lock database tester.test_steps */
	PDBS_LOCK(LOCK_STR);

	if(me->LineParam)
	{
		uint32_t last_param_id;

		/* get last test_id */
		EXCCHECK( podbc->ExecSQL( podbc, "SELECT MAX(lp.parameter_id) FROM tester.line_parameters lp"));
		CHECKERR( podbc->Fetch( podbc, TRUE));
		if(error==ELODBC_SQL_WARN_NO_DATA_FOUND)
			last_param_id = 0;
		else
			last_param_id = *((unsigned long*)podbc->GetFieldValuePtr(podbc, 1));

		EXCCHECK( podbc->Cancel(podbc));
	
		EXCCHECK( elstring_new(NULL, &pelstring));
		for(i=0; i<me->LineParamSize;)
		{	
			EXCCHECK(pelstring->Empty(pelstring) );
	
			if(me->LineParam[i].parameter_id==0)
				me->LineParam[i].parameter_id = ++last_param_id;

			sprintf(sql, "REPLACE INTO tester.line_parameters "
						 "(product_id,fnc_nb,name,value,parameter_id,property,time,user_id) "
						 "VALUES (%d,%d,\"%s\",\"%s\",%d,%d,\"%s\",%d)", 
						 me->LineParam[i].product_id,
						 me->LineParam[i].fnc_nb,
						 me->LineParam[i].name,			
						 me->LineParam[i].value,
						 me->LineParam[i].parameter_id,
						 me->LineParam[i].property,
						 me->LineParam[i].time,
						 me->LineParam[i].user_id);
	
			EXCCHECK(pelstring->Append(pelstring, sql) );
	
			i++;
			j = 0;
			
			for(; i<me->LineParamSize; i++)
			{
				if(++j>SQL_COMMAND_NB_OF_INSERTED_ROW)
					break;
			
				if(me->LineParam[i].parameter_id==0)
					me->LineParam[i].parameter_id = ++last_param_id;

				sprintf(sql, ",(%d,%d,\"%s\",_UTF8\"%s\",%d,%d,\"%s\",%d)", 
							 me->LineParam[i].product_id,
							 me->LineParam[i].fnc_nb,
							 me->LineParam[i].name,			
							 me->LineParam[i].value,
							 me->LineParam[i].parameter_id,
							 me->LineParam[i].property,
							 me->LineParam[i].time,
							 me->LineParam[i].user_id);
		
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
/* sort algorithm for test parameter structure array 
 * priority: 	1) test_nb
 *				2) PROPERTY_VALID
 *				3) name
 *				4) product_id
 */
int compare_LineParam ( const void* a, const void* b ) 
{
	SLineParamPtr aa, bb;
	
	aa = (SLineParamPtr)a;
	bb = (SLineParamPtr)b;
	
	if(aa->fnc_nb != bb->fnc_nb)
	{
		return ( aa->fnc_nb - bb->fnc_nb );
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
static SElExceptionPtr param_Insert(struct _SDBSLineParamList* me, SLineParam parameter)
{
	SElExceptionPtr	pexception = NULL; 
	
	/* check free space */
	EXCCHECK( param_CheckAlloc(me) );
	
	/* complete record */
	parameter.parameter_id = 0;
	DATETIME_TO_STRING(parameter.time);

	me->LineParam[me->LineParamSize++] = parameter;

	if(me->sort)  
		qsort(me->LineParam, me->LineParamSize, sizeof(SLineParam), compare_LineParam);
	
	PDBS->data_changed_line	= TRUE; 
	
Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "param_Sort"
static SElExceptionPtr param_Sort(struct _SDBSLineParamList* me)
{
	qsort(me->LineParam, me->LineParamSize, sizeof(SLineParam), compare_LineParam);
	
	return NULL; 
}

/*---------------------------------------------------------------------------*/
/* PRODUCT PARAMETER SECTION *************************************************/   
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "param_LineParamInsert"
static SElExceptionPtr param_LineParamInsert(
	struct _SDBSLineParamList* me, 
	SLineParam parameter
)
{
	SElExceptionPtr	pexception = NULL; 
	int				i;
	
	if(0==parameter.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");

	/* check duplication */
	for(i=0; i<me->LineParamSize; i++)
	{
		if(me->LineParam[i].product_id == parameter.product_id
			&& me->LineParam[i].fnc_nb == parameter.fnc_nb
			&& (me->LineParam[i].property&PROPERTY_VALID)>0
			&& 0==strcmp(me->LineParam[i].name, parameter.name))
		{
			EXCTHROW( DBS_ERROR_DUPLICATE_ITEM, "Duplicate item!");
		}
	}
	
	/* set new record */     
	parameter.property = PROPERTY_VALID;

	EXCCHECK( param_Insert(me, parameter)); 
	
Error:
	EXCRETHROW(pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "param_LineParamEdit"
static SElExceptionPtr param_LineParamEdit(
	struct _SDBSLineParamList* me, 
	SLineParam parameter
)
{
	SElExceptionPtr	pexception = NULL; 
	int				i;
		
	if(0==parameter.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");

	/* edit selected parameter */
	for(i=0; i<me->LineParamSize; i++)
	{
		if( (me->LineParam[i].property&PROPERTY_VALID)>0
			&& me->LineParam[i].fnc_nb == parameter.fnc_nb
			&& me->LineParam[i].product_id == parameter.product_id
			&& 0 == strcmp(me->LineParam[i].name, parameter.name))
		{
			me->LineParam[i].property ^= PROPERTY_VALID;   
			break;
		}
	}
	
	/* set new record */     
	parameter.property = PROPERTY_VALID;

	EXCCHECK( param_Insert(me, parameter));    	
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "param_LineParamDel"
static SElExceptionPtr param_LineParamDel(
	struct _SDBSLineParamList* me, 
	SLineParam parameter
)
{
	SElExceptionPtr	pexception = NULL; 
	int				i, j;
	int				pid[1024];
	int				pidSize = 1024;
	
	if(0==parameter.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");

	/* search downtree products id */
	EXCCHECK( PPRODUCT->ProductGetIdTreeDown(PPRODUCT, parameter.product_id, pid, &pidSize));
	
	for(j=0; j<pidSize; j++)
	{
		/* set current parameters not valid */  
		for(i=0; i<me->LineParamSize;i++)
		{
			if( (me->LineParam[i].property&PROPERTY_VALID)>0
				&& me->LineParam[i].fnc_nb==parameter.fnc_nb
				&& me->LineParam[i].product_id == pid[j]
				&& 0==strcmp(me->LineParam[i].name, parameter.name))
			{
				me->LineParam[i].property ^= PROPERTY_VALID;
				break;
			}
		}
	}

	PDBS->data_changed_line	= TRUE;
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/  
#undef __FUNC__
#define __FUNC__ "param_LineParamCopy"
static SElExceptionPtr param_LineParamCopy(
	struct _SDBSLineParamList* me, 
	int32_t pidSrc[], 
	int32_t pidTrg[], 
	int32_t pidSize
)
{
	SElExceptionPtr	pexception = NULL;   
	SLineParamPtr	pparamnew = NULL;
	SLineParamPtr   pdata = NULL;
	int32_t			paramSize, i, j, k;   
	
	/* disable sorting algorithm because of program speed */ 
	me->sort = FALSE;
	
	/* precount */
	for(i=0, paramSize = 0; i<me->LineParamSize; i++) 
	{
		for(j=0; j<pidSize; j++)
		{
			if( (me->LineParam[i].property&PROPERTY_VALID)>0
				&&pidSrc[j] == me->LineParam[i].product_id)
			{
				paramSize++;
				break;
			}
		}
	}
	
	pparamnew = calloc(paramSize, sizeof(SLineParam));		
	pdata = pparamnew;
	
	for(i=0; i<me->LineParamSize; i++) 
	{	
		for(j=0; j<pidSize; j++)
		{
			if((me->LineParam[i].property&PROPERTY_VALID)>0
				&& pidSrc[j] == me->LineParam[i].product_id)
			{
				*pdata = me->LineParam[i];
				pdata->fnc_nb++;
				
				for(k=0; k<pidSize; k++)
				{
					if(pidSrc[k] == me->LineParam[i].product_id)
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
	param_Allocate(me, me->LineParamSize+paramSize);

	for(i=0; i<paramSize; i++)
	{
		EXCCHECK( param_Insert(me, pparamnew[i])); 
	}

	me->sort = TRUE;
	EXCCHECK( param_Sort(me)); 

Error:
	me->sort = TRUE;
	free(pparamnew);
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/  
#undef __FUNC__
#define __FUNC__ "param_Allocate"
static SElExceptionPtr param_Allocate(struct _SDBSLineParamList* me, int32_t size) 
{
	SElExceptionPtr    pexception = NULL;   

	if(me->_Allocated == 0)
	{
		me->LineParam = calloc(size+MEMORY_ALLOCATION_RESERVE, sizeof(SLineParam)); 
		EXCCHECKALLOC(me->LineParam);
		me->_Allocated = size+MEMORY_ALLOCATION_RESERVE;
	}
	else if(size>me->_Allocated)   
	{
		me->LineParam = realloc(me->LineParam, (size+MEMORY_ALLOCATION_RESERVE)*sizeof(SLineParam)); 
		EXCCHECKALLOC(me->LineParam);
		memset( me->LineParam + me->_Allocated, 
				0, 
				(size+MEMORY_ALLOCATION_RESERVE - me->_Allocated)*sizeof(SLineParam));
		me->_Allocated = size+MEMORY_ALLOCATION_RESERVE;
	}
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "param_CheckAlloc"
static SElExceptionPtr param_CheckAlloc(struct _SDBSLineParamList* me)
{
	SElExceptionPtr    pexception = NULL;   
	
	if(me->_Allocated - me->LineParamSize < MEMORY_ALLOCATION_MIN )
	{
		me->LineParam = realloc( me->LineParam, 
							(me->_Allocated + MEMORY_ALLOCATION_RESERVE)*sizeof(SLineParam));
		EXCCHECKALLOC(me->LineParam);
		/* clear memory */
		memset( me->LineParam + me->_Allocated,
				0,
				MEMORY_ALLOCATION_RESERVE*sizeof(SLineParam));
		me->_Allocated += MEMORY_ALLOCATION_RESERVE;
	}
		
Error:
	EXCRETHROW( pexception); 
}

