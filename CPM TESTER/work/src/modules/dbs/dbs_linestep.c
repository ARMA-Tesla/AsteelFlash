#include "dbs.h"
#include "dbs_linestep.h"
#include <stdio.h>
#include <elstring.h>
#include <odbc/elodbc.h>
#include "dbs_define_local.h"

#define LOCK_STR		"line_step"

static SElExceptionPtr step_LineStepRead(struct _SDBSLineStepList* me, int32_t branch_id); 		
static SElExceptionPtr step_LineStepSave(struct _SDBSLineStepList* me, int32_t branch_id);	

static SElExceptionPtr step_LineStepInsert(struct _SDBSLineStepList* me, SLineStep step);
static SElExceptionPtr step_LineStepEdit(struct _SDBSLineStepList* me, SLineStep step);
static SElExceptionPtr step_LineStepDel(struct _SDBSLineStepList* me, SLineStep step);  
static SElExceptionPtr step_LineStepSkip(struct _SDBSLineStepList* me, SLineStep step); 
static SElExceptionPtr step_LineStepSkipRemove(struct _SDBSLineStepList* me, SLineStep step);
static SElExceptionPtr step_LineStepCopy(struct _SDBSLineStepList* me, int32_t pidSrc[], int32_t pidTrg[], int32_t pidSize, int32_t processCpy[], int32_t processSize);

static SElExceptionPtr step_Sort(struct _SDBSLineStepList* me);
static SElExceptionPtr step_CheckAlloc(struct _SDBSLineStepList* me);
static SElExceptionPtr step_Allocate(struct _SDBSLineStepList* me, int32_t size);

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbslinesteplist_new"
SElExceptionPtr dbslinesteplist_new(SDBSLineStepListPtr* pDBSLineStepListPtr, void* pDBS)
{
	SElExceptionPtr			pexception = NULL;
	SDBSLineStepListPtr	me = NULL;

	me = calloc(1,sizeof(SDBSLineStepList));
	EXCCHECKALLOC(me);
	
	*pDBSLineStepListPtr = me;
	
	me->LineStepRead	= step_LineStepRead;
	me->LineStepSave	= step_LineStepSave;

	me->LineStepInsert = step_LineStepInsert;
	me->LineStepEdit = step_LineStepEdit;
	me->LineStepDel = step_LineStepDel;
	me->LineStepSkip = step_LineStepSkip;
	me->LineStepSkipRemove = step_LineStepSkipRemove;
	me->LineStepCopy = step_LineStepCopy;

	me->pdbs = pDBS;
	me->sort = TRUE;

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbslinesteplist_delete"
SElExceptionPtr dbslinesteplist_delete(SDBSLineStepListPtr* pDBSLineStepListPtr)
{
	if (pDBSLineStepListPtr && *pDBSLineStepListPtr)
	{
		SDBSLineStepListPtr	me = *pDBSLineStepListPtr;
		
		if(me->LineStep)
		{
			free(me->LineStep);
			me->LineStep = NULL;
		}

		free(*pDBSLineStepListPtr);
		*pDBSLineStepListPtr = NULL;
	}

/* Error: */
	return NULL;
}

/*---------------------------------------------------------------------------*/  
#undef __FUNC__
#define __FUNC__ "step_LineStepRead"
static SElExceptionPtr step_LineStepRead(struct _SDBSLineStepList* me, int32_t branch_id)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL; 
	int32_t			count, index;
	char			sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr		podbc = PDBS->podbc;
	
	LOCK;

	/* lock database tester.line_steps */
	PDBS_LOCK(LOCK_STR);
	
	/* select items */
	sprintf(sql, "SELECT ls.product_id,ls.process_nb,ls.fnc_nb,ls.name,ls.fnc_id,ls.property,ls.time,ls.user_id " 
				 "FROM tester.product p INNER JOIN tester.line_steps ls "
				 "ON p.product_id=ls.product_id "
				 "WHERE p.branch_id=%d "
				 "ORDER BY ls.process_nb,ls.fnc_nb,ls.product_id ", 
				 branch_id);
	EXCCHECK( podbc->ExecSQL( podbc, sql));
	EXCCHECK( podbc->GetRows(podbc, &count));

	/* memory allocation */
	EXCCHECK( step_Allocate(me, count));

	CHECKERR( podbc->Fetch( podbc, TRUE));
	
	for(index=0;index<count;index++)
	{
		SLineStepPtr		pstep = me->LineStep;
	
		pstep[index].product_id = *((long*)podbc->GetFieldValuePtr(podbc, 1));
		pstep[index].process_nb = *((long*)podbc->GetFieldValuePtr(podbc, 2));
		pstep[index].fnc_nb = *((long*)podbc->GetFieldValuePtr(podbc, 3));
		strcpy(pstep[index].name, (char*)podbc->GetFieldValuePtr(podbc, 4));
		pstep[index].fnc_id	= *((long*)podbc->GetFieldValuePtr(podbc, 5));
		pstep[index].property 	= *((short*)podbc->GetFieldValuePtr(podbc, 6));
		strcpy(pstep[index].time, (char*)podbc->GetFieldValuePtr(podbc, 7));
		pstep[index].user_id	= *((long*)podbc->GetFieldValuePtr(podbc, 8));

		CHECKERR( podbc->MoveNext(podbc));
	}
	
	me->LineStepSize = index;
	
	EXCCHECK( podbc->Cancel(podbc));

	/* sort because of @skip */
	EXCCHECK( step_Sort(me));
	
Error:
	PDBS_UNLOCK(LOCK_STR);
	UNLOCK;
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "step_LineStepSave"
static SElExceptionPtr step_LineStepSave(struct _SDBSLineStepList* me, int32_t branch_id)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL;
	int				i, j;
	char			sql[SQL_COMMAND_LENGHT] = "";  
	SOdbcPtr		podbc = PDBS->podbc;
	SELStringPtr   	pelstring = NULL;

	LOCK;

	/* lock database tester.line_steps */
	PDBS_LOCK(LOCK_STR);

	if(me->LineStep)
	{
		uint32_t last_fnc_id;

		/* get last test_id */
		EXCCHECK( podbc->ExecSQL( podbc, "SELECT MAX(ls.fnc_id) FROM tester.line_steps ls"));
		CHECKERR( podbc->Fetch( podbc, TRUE));
		if(error==ELODBC_SQL_WARN_NO_DATA_FOUND)
			last_fnc_id = 0;
		else
			last_fnc_id = *((unsigned long*)podbc->GetFieldValuePtr(podbc, 1));

		EXCCHECK( podbc->Cancel(podbc));
	
		EXCCHECK( elstring_new(NULL, &pelstring));
		for(i=0; i<me->LineStepSize;)
		{	
			EXCCHECK(pelstring->Empty(pelstring) );
	
			if(me->LineStep[i].fnc_id==0)
				me->LineStep[i].fnc_id = ++last_fnc_id;

			sprintf(sql, "REPLACE INTO tester.line_steps "
						 "(product_id,process_nb,fnc_nb,name,fnc_id,property,time,user_id) "
						 "VALUES (%d,%d,%d,'%s',%d,%d,'%s',%d)", 
						 me->LineStep[i].product_id,
						 me->LineStep[i].process_nb,
						 me->LineStep[i].fnc_nb,
						 me->LineStep[i].name,		
						 me->LineStep[i].fnc_id,
						 me->LineStep[i].property,
						 me->LineStep[i].time,
						 me->LineStep[i].user_id);
	
			EXCCHECK(pelstring->Append(pelstring, sql) );
	
			i++;
			j = 0;
			
			for(; i<me->LineStepSize; i++)
			{
				if(++j>SQL_COMMAND_NB_OF_INSERTED_ROW)
					break;
			
				if(me->LineStep[i].fnc_id==0)
					me->LineStep[i].fnc_id = ++last_fnc_id;

				sprintf(sql, ",(%d,%d,%d,'%s',%d,%d,'%s',%d)", 
							 me->LineStep[i].product_id,
							 me->LineStep[i].process_nb,
							 me->LineStep[i].fnc_nb,
							 me->LineStep[i].name,		
							 me->LineStep[i].fnc_id,
							 me->LineStep[i].property,
							 me->LineStep[i].time,
							 me->LineStep[i].user_id);
		
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

/* sort algorithm for test structure array 
 * priority: 	1) process_nb
 *				2) PROPERTY_VALID
 *				3) product_id
 *				4) name (@skip to end)
 */
int compare_Step ( const void* a, const void* b ) 
{
	SLineStepPtr aa, bb;
	
	aa = (SLineStepPtr)a;
	bb = (SLineStepPtr)b;
	
	if(aa->process_nb != bb->process_nb)
	{
		return (aa->process_nb - bb->process_nb);
	}
	else if(aa->fnc_nb != bb->fnc_nb)
	{
		return (aa->fnc_nb - bb->fnc_nb);
	}
	else if((aa->property&PROPERTY_VALID) != (bb->property&PROPERTY_VALID))
	{
		return ((bb->property&PROPERTY_VALID) - (aa->property&PROPERTY_VALID));
	}
	else if(aa->product_id != bb->product_id)
	{
		return (aa->product_id - bb->product_id);
	}
	else
	{
		if(0==strcmp(aa->name, "@skip"))
			return 1;
		else if(0==strcmp(bb->name, "@skip"))
			return -1;
		else
			return 0;
	}
}		

/*---------------------------------------------------------------------------*/
int compare_Step2 ( const void* a, const void* b ) 
{
	SLineStepPtr aa, bb;
	
	aa = (SLineStepPtr)a;
	bb = (SLineStepPtr)b;
	
	return (aa->fnc_nb - bb->fnc_nb);
}	

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "step_Insert"
static SElExceptionPtr step_Insert(struct _SDBSLineStepList* me, SLineStep step)
{ 
	SElExceptionPtr	pexception = NULL; 
	
	/* check free space */
	EXCCHECK( step_CheckAlloc(me));

	/* complete record */
	step.fnc_id = 0;
	DATETIME_TO_STRING(step.time);

	me->LineStep[me->LineStepSize++] = step;
	
	if(me->sort)
		qsort(me->LineStep, me->LineStepSize, sizeof(SLineStep), compare_Step); 
	
	PDBS->data_changed_line	= TRUE;  
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "step_Sort"
static SElExceptionPtr step_Sort(struct _SDBSLineStepList* me)
{
	qsort(me->LineStep, me->LineStepSize, sizeof(SLineStep), compare_Step); 
	
/* Error: */
	return NULL; 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "step_Sort2"
static SElExceptionPtr step_Sort2(struct _SDBSLineStepList* me)
{
	qsort(me->LineStep, me->LineStepSize, sizeof(SLineStep), compare_Step2); 
	
/* Error: */
	return NULL; 
}

/*---------------------------------------------------------------------------*/
/* PROCESS SECTION **************************************************************/
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "step_LineStepInsert"
static SElExceptionPtr step_LineStepInsert(
	struct _SDBSLineStepList* me, 
	SLineStep step)
{
	SElExceptionPtr	pexception = NULL; 
	int				i;
		
	if(0==step.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");

	/* reindex all steps */
	for(i=0; i<me->LineStepSize; i++)
	{
		if( me->LineStep[i].fnc_nb >= step.fnc_nb)
			me->LineStep[i].fnc_nb++;
	}
	
	/* reindex all parameters */
	for(i=0; i<PLINEPARAM->LineParamSize; i++)
	{
		if( PLINEPARAM->LineParam[i].fnc_nb >= step.fnc_nb)
			PLINEPARAM->LineParam[i].fnc_nb++;
	}
	
	/* set new record */
	step.property = PROPERTY_VALID;
	
	EXCCHECK( step_Insert(me, step) );  
		
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "step_LineStepEdit"
static SElExceptionPtr step_LineStepEdit(
	struct _SDBSLineStepList* me, 
	SLineStep step)
{
	SElExceptionPtr	pexception = NULL; 
	int				i;
	
	if(0==step.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");

	/* set actual not valid */
	for(i=0; i<me->LineStepSize; i++)
	{
		if( (me->LineStep[i].property&PROPERTY_VALID)>0
			&& me->LineStep[i].fnc_nb == step.fnc_nb
			&& 0!=strcmp(me->LineStep[i].name, "@skip") )
		{
			me->LineStep[i].property ^= PROPERTY_VALID;
			break;
		}
	}
	
	/* set new record */
	step.property = PROPERTY_VALID;
	
	EXCCHECK( step_Insert(me, step) );  
		
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "step_LineStepDel"
static SElExceptionPtr step_LineStepDel(
	struct _SDBSLineStepList* me, 
	SLineStep step
)
{
	SElExceptionPtr	pexception = NULL; 
	int				i;
		
	if(0==step.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");

	/* set current steps and all skips not valid */
	for(i=0; i<me->LineStepSize; i++)
	{
		if( (me->LineStep[i].property&PROPERTY_VALID)>0
			&& me->LineStep[i].fnc_nb == step.fnc_nb)
		{
			me->LineStep[i].property ^= PROPERTY_VALID; 
		}	
	}
	
	/* set current test parameters not valid without log*/ 
	for(i=0; i<PLINEPARAM->LineParamSize; i++)
	{
		if( (PLINEPARAM->LineParam[i].property&PROPERTY_VALID)>0
			&& PLINEPARAM->LineParam[i].fnc_nb == step.fnc_nb)
		{
			PLINEPARAM->LineParam[i].property ^= PROPERTY_VALID;
		}	
	}

	PDBS->data_changed_line	= TRUE;  

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "step_LineStepSkip"
static SElExceptionPtr step_LineStepSkip(
	struct _SDBSLineStepList* me, 
	SLineStep step
)
{
	SElExceptionPtr	pexception = NULL; 
	int				i;
		
	if(0==step.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");

	/* check duplication */
	/* TODO: check for parent duplication too */
	for(i=0; i<me->LineStepSize; i++)
	{
		if( (me->LineStep[i].property&PROPERTY_VALID)>0
			&& me->LineStep[i].fnc_nb == step.fnc_nb
			&& me->LineStep[i].product_id == step.product_id
			&& 0==strcmp(me->LineStep[i].name, "@skip"))
		{
			EXCTHROW(DBS_ERROR_DUPLICATE_ITEM, "Duplicate item!");
		}
	}
	
	/* set new record */ 
	step.property = PROPERTY_VALID;
	strcpy(step.name, "@skip");
	
	EXCCHECK( step_Insert(me, step));
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "step_LineStepSkipRemove"
static SElExceptionPtr step_LineStepSkipRemove(
	struct _SDBSLineStepList* me, 
	SLineStep step
)
{
	SElExceptionPtr	pexception = NULL; 
	int				i; 
	
	if(0==step.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");

	/* set current skip not valid */
	for(i=0; i<me->LineStepSize; i++)
	{
		if( (me->LineStep[i].property&PROPERTY_VALID)>0 
			&& me->LineStep[i].fnc_nb == step.fnc_nb
			&& me->LineStep[i].product_id == step.product_id
			&& 0==strcmp(me->LineStep[i].name, "@skip") )
		{
			me->LineStep[i].property ^= PROPERTY_VALID;
			break;
		}	
	}
	
	PDBS->data_changed_line	= TRUE;  

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/  
#undef __FUNC__
#define __FUNC__ "step_LineStepCopy"
static SElExceptionPtr step_LineStepCopy(
	struct _SDBSLineStepList* me, 
	int32_t pidSrc[], 
	int32_t pidTrg[], 
	int32_t pidSize,
	int32_t processCpy[], 
	int32_t processSize
)
{
	SElExceptionPtr	pexception = NULL;   
	SLineStepPtr 	pstepnew = NULL;
	SLineStepPtr 	pdata = NULL;
	int32_t			next_fnc_nb = 0;
	int32_t			stepSize = 0;
	int32_t			shift = 0;
	int32_t			step_index = 0;
	int32_t			i, j, k;
	int32_t			*fncCpy = NULL;
	int32_t			fncSize = 0;

	/* disable sorting algorithm because of program speed */ 
	me->sort = FALSE;
	
	/* precount */
	for(i=0; i<me->LineStepSize; i++)
	{
		for(j=0; j<pidSize; j++)
		{
			if( (me->LineStep[i].property&PROPERTY_VALID)>0
				&&pidSrc[j] == me->LineStep[i].product_id )
			{
				stepSize++;	
				break;
			}
		}
	}
	
	pstepnew = calloc(stepSize, sizeof(SLineStep));
	fncCpy = calloc(stepSize, sizeof(int32_t));
	pdata = pstepnew;
	
	/* sort by fnc_nb */
	step_Sort2(me);

	/* shift the steps */
	for(i=0; i<me->LineStepSize; i++)
	{
		/* shift steps */
		for(j=step_index; j<PLINEPARAM->LineParamSize; j++) 
		{
			if(PLINEPARAM->LineParam[j].fnc_nb==me->LineStep[i].fnc_nb)
			{
				PLINEPARAM->LineParam[j].fnc_nb += shift;
			}
			else if(PLINEPARAM->LineParam[j].fnc_nb>me->LineStep[i].fnc_nb) 
			{
				step_index = j;
				break;
			}
		}
	
		/* shift step */
		me->LineStep[i].fnc_nb += shift;
		
		for(j=0; j<pidSize; j++)
		{
			if( (me->LineStep[i].property&PROPERTY_VALID)>0
				&& pidSrc[j] == me->LineStep[i].product_id)
			{
				*pdata = me->LineStep[i];

				/* shift only when oreginal was shifted */
				for(k=0; k<processSize; k++)
				{
					if(processCpy[k]==pdata->process_nb)
						break;
				}
				if(k<processSize)
					pdata->process_nb++;

				/* next fnc_nb */
				for(k=i+1; k<me->LineStepSize; k++)
				{
					if( (me->LineStep[k].property&PROPERTY_VALID)>0)
					{
						next_fnc_nb = me->LineStep[k].fnc_nb + shift;	
						break;
					}
				}
				
				if(next_fnc_nb==0 || me->LineStep[i].fnc_nb!=next_fnc_nb)
					shift++; 

				if(0!=strcmp(me->LineStep[i].name, "@skip"))
				{
					fncCpy[fncSize++] = pdata->fnc_nb;
					pdata->fnc_nb++;
				}
				
				for(k=0; k<pidSize; k++)
				{
					if(pidSrc[k] == me->LineStep[i].product_id)
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
	step_Allocate(me, me->LineStepSize+stepSize);

	for(i=0; i<stepSize; i++)
	{
		/* shift @skip */
		for(j=0; j<fncSize; j++)
		{
			if(fncCpy[j]==pstepnew[i].fnc_nb && 0==strcmp(pstepnew[i].name, "@skip"))
				break;
		}
		if(j<fncSize)
			pstepnew[i].fnc_nb++;

		EXCCHECK( step_Insert(me, pstepnew[i])); 
	}

	me->sort = TRUE;
	EXCCHECK( step_Sort(me));

Error:
	me->sort = TRUE;    
	free(pstepnew);
	free(fncCpy);
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/  
#undef __FUNC__
#define __FUNC__ "step_Allocate"
static SElExceptionPtr step_Allocate(struct _SDBSLineStepList* me, int32_t size) 
{
	SElExceptionPtr    pexception = NULL;   

	if(me->_Allocated == 0)
	{
		me->LineStep = calloc(size+MEMORY_ALLOCATION_RESERVE, sizeof(SLineStep)); 
		EXCCHECKALLOC(me->LineStep);
		me->_Allocated = size+MEMORY_ALLOCATION_RESERVE;
	}
	else if(size>me->_Allocated)   
	{
		me->LineStep = realloc(me->LineStep, (size+MEMORY_ALLOCATION_RESERVE)*sizeof(SLineStep)); 
		EXCCHECKALLOC(me->LineStep);
		memset( me->LineStep + me->_Allocated, 
				0, 
				(size+MEMORY_ALLOCATION_RESERVE - me->_Allocated)*sizeof(SLineStep));
		me->_Allocated = size+MEMORY_ALLOCATION_RESERVE;
	}
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "step_CheckAlloc"
static SElExceptionPtr step_CheckAlloc(struct _SDBSLineStepList* me)
{
	SElExceptionPtr    pexception = NULL;   
	
	if(me->_Allocated - me->LineStepSize < MEMORY_ALLOCATION_MIN )
	{
		me->LineStep = realloc( me->LineStep, 
							(me->_Allocated + MEMORY_ALLOCATION_RESERVE)*sizeof(SLineStep));
		EXCCHECKALLOC(me->LineStep);
		/* clear memory */
		memset( me->LineStep + me->_Allocated,
				0,
				MEMORY_ALLOCATION_RESERVE*sizeof(SLineStep));
		me->_Allocated += MEMORY_ALLOCATION_RESERVE;
	}
		
Error:
	EXCRETHROW( pexception); 
}

