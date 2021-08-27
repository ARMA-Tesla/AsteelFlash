#include "dbs.h"
#include "dbs_lineprocess.h"
#include <stdio.h>
#include <elstring.h>
#include <odbc/elodbc.h>
#include "dbs_define_local.h"

#define LOCK_STR		"line_process"

static SElExceptionPtr process_LineProcessRead(struct _SDBSLineProcessList* me, int32_t branch_id); 		
static SElExceptionPtr process_LineProcessSave(struct _SDBSLineProcessList* me, int32_t branch_id);	

static SElExceptionPtr process_LineProcessInsert(struct _SDBSLineProcessList* me, SLineProcess process);
static SElExceptionPtr process_LineProcessDel(struct _SDBSLineProcessList* me, SLineProcess process); 
static SElExceptionPtr process_LineProcessEdit(struct _SDBSLineProcessList* me, SLineProcess process); 
static SElExceptionPtr process_LineProcessCopy(struct _SDBSLineProcessList* me, int32_t pidSrc[], int32_t pidTrg[], int32_t pidSize, int32_t ProcessCpy[], int32_t *ProcessSize);

static SElExceptionPtr process_CheckAlloc(struct _SDBSLineProcessList* me);
static SElExceptionPtr process_Allocate(struct _SDBSLineProcessList* me, int32_t size);

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbslineprocesslist_new"
SElExceptionPtr dbslineprocesslist_new(SDBSLineProcessListPtr* pDBSLineProcessListPtr, void* pDBS)
{
	SElExceptionPtr			pexception = NULL;
	SDBSLineProcessListPtr	me = NULL;

	me = calloc(1,sizeof(SDBSLineProcessList));
	EXCCHECKALLOC(me);
	
	*pDBSLineProcessListPtr = me;
	
	me->LineProcessRead	= process_LineProcessRead;
	me->LineProcessSave	= process_LineProcessSave;

	me->LineProcessInsert = process_LineProcessInsert;
	me->LineProcessDel = process_LineProcessDel;
	me->LineProcessEdit = process_LineProcessEdit;
	me->LineProcessCopy = process_LineProcessCopy;
	
	me->pdbs = pDBS;
	me->sort = TRUE;

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbslineprocesslist_delete"
SElExceptionPtr dbslineprocesslist_delete(SDBSLineProcessListPtr* pDBSLineProcessListPtr)
{	
	if (pDBSLineProcessListPtr && *pDBSLineProcessListPtr)
	{
		SDBSLineProcessListPtr	me = *pDBSLineProcessListPtr;
		
		if(me->LineProcess)
		{
			free(me->LineProcess);
			me->LineProcess = NULL;
		}

		free(*pDBSLineProcessListPtr);
		*pDBSLineProcessListPtr = NULL;
	}

/* Error: */
	return NULL;
}

/*---------------------------------------------------------------------------*/  
#undef __FUNC__
#define __FUNC__ "process_LineProcessRead"
static SElExceptionPtr process_LineProcessRead(struct _SDBSLineProcessList* me, int32_t branch_id)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL; 
	int32_t			count, index;
	char			sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr		podbc = PDBS->podbc;
	
	LOCK;

	/* lock database tester.test_steps */
	PDBS_LOCK(LOCK_STR);
	
	/* select items */
	sprintf(sql, "SELECT lp.product_id,lp.process_nb,lp.name,lp.description,lp.process_id,lp.property,lp.time,lp.user_id " 
				 "FROM tester.product p INNER JOIN tester.line_process lp "
				 "ON p.product_id=lp.product_id "
				 "WHERE p.branch_id=%d "
				 "ORDER BY lp.process_nb, lp.product_id", 
				 branch_id);
	EXCCHECK( podbc->ExecSQL( podbc, sql));
	EXCCHECK( podbc->GetRows(podbc, &count));

	/* memory allocation */
	EXCCHECK( process_Allocate(me, count));

	CHECKERR( podbc->Fetch( podbc, TRUE));
	
	for(index=0;index<count;index++)
	{
		SLineProcessPtr		pprocess = me->LineProcess;
	
		pprocess[index].product_id = *((long*)podbc->GetFieldValuePtr(podbc, 1));
		pprocess[index].process_nb = *((long*)podbc->GetFieldValuePtr(podbc, 2));
		strcpy(pprocess[index].name, (char*)podbc->GetFieldValuePtr(podbc, 3));
		strcpy(pprocess[index].description, (char*)podbc->GetFieldValuePtr(podbc, 4));
		pprocess[index].process_id	= *((long*)podbc->GetFieldValuePtr(podbc, 5));
		pprocess[index].property 	= *((short*)podbc->GetFieldValuePtr(podbc, 6));
		strcpy(pprocess[index].time, (char*)podbc->GetFieldValuePtr(podbc, 7));
		pprocess[index].user_id	= *((long*)podbc->GetFieldValuePtr(podbc, 8));

		CHECKERR( podbc->MoveNext(podbc));
	}
	
	me->LineProcessSize = index;
	
	EXCCHECK( podbc->Cancel(podbc));
	
Error:
	PDBS_UNLOCK(LOCK_STR);
	UNLOCK;
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "process_LineProcessSave"
static SElExceptionPtr process_LineProcessSave(struct _SDBSLineProcessList* me, int32_t branch_id)
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

	if(me->LineProcess)
	{
		uint32_t last_process_id;

		/* get last test_id */
		EXCCHECK( podbc->ExecSQL( podbc, "SELECT MAX(lp.process_id) FROM tester.line_process lp"));
		CHECKERR( podbc->Fetch( podbc, TRUE));
		if(error==ELODBC_SQL_WARN_NO_DATA_FOUND)
			last_process_id = 0;
		else
			last_process_id = *((unsigned long*)podbc->GetFieldValuePtr(podbc, 1));

		EXCCHECK( podbc->Cancel(podbc));
	
		EXCCHECK( elstring_new(NULL, &pelstring));
		for(i=0; i<me->LineProcessSize;)
		{	
			EXCCHECK(pelstring->Empty(pelstring) );
	
			if(me->LineProcess[i].process_id==0)
				me->LineProcess[i].process_id = ++last_process_id;

			sprintf(sql, "REPLACE INTO tester.line_process "
						 "(product_id,process_nb,name,description,process_id,property,time,user_id) "
						 "VALUES (%d,%d,'%s','%s',%d,%d,'%s',%d)", 
						 me->LineProcess[i].product_id,
						 me->LineProcess[i].process_nb,
						 me->LineProcess[i].name,			
						 me->LineProcess[i].description,
						 me->LineProcess[i].process_id,
						 me->LineProcess[i].property,
						 me->LineProcess[i].time,
						 me->LineProcess[i].user_id);
	
			EXCCHECK(pelstring->Append(pelstring, sql) );
	
			i++;
			j = 0;
			
			for(; i<me->LineProcessSize; i++)
			{
				if(++j>SQL_COMMAND_NB_OF_INSERTED_ROW)
					break;
			
				if(me->LineProcess[i].process_id==0)
					me->LineProcess[i].process_id = ++last_process_id;

				sprintf(sql, ",(%d,%d,'%s','%s',%d,%d,'%s',%d)", 
							 me->LineProcess[i].product_id,
							 me->LineProcess[i].process_nb,
							 me->LineProcess[i].name,			
							 me->LineProcess[i].description,
							 me->LineProcess[i].process_id,
							 me->LineProcess[i].property,
							 me->LineProcess[i].time,
							 me->LineProcess[i].user_id);
		
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
 */
int compare_Process ( const void* a, const void* b ) 
{
	SLineProcessPtr aa, bb;
	
	aa = (SLineProcessPtr)a;
	bb = (SLineProcessPtr)b;
	
	if(aa->process_nb != bb->process_nb)
	{
		return ( aa->process_nb - bb->process_nb );
	}
	else if( (aa->property&PROPERTY_VALID) != (bb->property&PROPERTY_VALID) )
	{
		return ( (bb->property&PROPERTY_VALID) - (aa->property&PROPERTY_VALID) );
	}
	else 
	{
		return ( aa->product_id - bb->product_id );
	}
}		

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "process_Insert"
static SElExceptionPtr process_Insert(struct _SDBSLineProcessList* me, SLineProcess process)
{ 
	SElExceptionPtr	pexception = NULL; 
	
	/* check free space */
	EXCCHECK( process_CheckAlloc(me) );

	/* complete record */
	process.process_id = 0;
	DATETIME_TO_STRING(process.time);

	me->LineProcess[me->LineProcessSize++] = process;
	
	if(me->sort)
		qsort(me->LineProcess, me->LineProcessSize, sizeof(SLineProcess), compare_Process); 
	
	PDBS->data_changed_line	= TRUE;  
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "process_Sort"
static SElExceptionPtr process_Sort(struct _SDBSLineProcessList* me)
{
	qsort(me->LineProcess, me->LineProcessSize, sizeof(SLineProcess), compare_Process); 

	return NULL; 
}

/*---------------------------------------------------------------------------*/
/* PROCESS SECTION **************************************************************/
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "process_LineProcessInsert"
static SElExceptionPtr process_LineProcessInsert(
	struct _SDBSLineProcessList* me, 
	SLineProcess process)
{
	SElExceptionPtr	pexception = NULL; 
	int				i;
		
	if(0==process.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");

	/* reindex all process */
	for(i=0; i<me->LineProcessSize; i++)
	{
		if( me->LineProcess[i].process_nb >= process.process_nb)
			me->LineProcess[i].process_nb++;
	}
	
	/* reindex all steps */
	for(i=0; i<PSTEP->LineStepSize; i++)
	{
		if( PSTEP->LineStep[i].process_nb >= process.process_nb)
			PSTEP->LineStep[i].process_nb++;
	}
	
	/* set new record */
	process.property = PROPERTY_VALID;
	
	EXCCHECK( process_Insert(me, process) );  
		
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "process_LineProcessEdit"
static SElExceptionPtr process_LineProcessEdit(
	struct _SDBSLineProcessList* me, 
	SLineProcess process)
{
	SElExceptionPtr	pexception = NULL; 
	int				i;
		
	if(0==process.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");

	for(i=0; i<me->LineProcessSize; i++)
	{
		if( (me->LineProcess[i].property&PROPERTY_VALID)>0
			&& me->LineProcess[i].process_nb == process.process_nb)
		{
			strcpy(me->LineProcess[i].description, process.description);
			break;
		}	
	}
		
	PDBS->data_changed_line	= TRUE;  

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "process_LineProcessCopy"
static SElExceptionPtr process_LineProcessCopy(
	struct _SDBSLineProcessList* me, 
	int32_t pidSrc[], 
	int32_t pidTrg[], 
	int32_t pidSize,
	int32_t	ProcessCpy[],
	int32_t	*ProcessSize
)
{
	SElExceptionPtr	pexception = NULL; 
	SLineProcessPtr 	pprocessnew = NULL;
	SLineProcessPtr 	pdata = NULL;
	int32_t			next_process_nb = 0;
	int32_t			processSize = 0;
	int32_t			shift = 0;
	int32_t			process_index = 0;
	int32_t			i, j, k;

	/* disable sorting algorithm because of program speed */ 
	me->sort = FALSE;
	
	/* precount */
	for(i=0; i<me->LineProcessSize; i++)
	{
		for(j=0; j<pidSize; j++)
		{
			if( (me->LineProcess[i].property&PROPERTY_VALID)>0
				&&pidSrc[j] == me->LineProcess[i].product_id )
			{
				processSize++;	
				break;
			}
		}
	}
	
	pprocessnew = calloc(processSize, sizeof(SLineProcess));
	pdata = pprocessnew;
	
	/* shift the processes */
	for(i=0; i<me->LineProcessSize; i++)
	{
		/* shift steps */
		for(j=process_index; j<PSTEP->LineStepSize; j++) 
		{
			if(PSTEP->LineStep[j].process_nb==me->LineProcess[i].process_nb)
			{
				PSTEP->LineStep[j].process_nb += shift;
			}
			else if(PSTEP->LineStep[j].process_nb>me->LineProcess[i].process_nb) 
			{
				process_index = j;
				break;
			}
		}
	
		/* shift process */
		me->LineProcess[i].process_nb += shift;
		
		for(j=0; j<pidSize; j++)
		{
			if( (me->LineProcess[i].property&PROPERTY_VALID)>0
				&&pidSrc[j] == me->LineProcess[i].product_id )
			{
				*pdata = me->LineProcess[i];
				ProcessCpy[(*ProcessSize)++] = pdata->process_nb;
				pdata->process_nb++;
				
				/* next test_nb */
				for(k=i+1; k<me->LineProcessSize; k++)
				{
					if( (me->LineProcess[k].property&PROPERTY_VALID)>0 )
					{
						next_process_nb = me->LineProcess[k].process_nb + shift;	
						break;
					}
				}
				
				if(next_process_nb==0 || me->LineProcess[i].process_nb!=next_process_nb)
					shift++; 
				
				for(k=0; k<pidSize; k++)
				{
					if(pidSrc[k] == me->LineProcess[i].product_id)
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
	
	for(i=0; i<processSize; i++)
	{
		EXCCHECK( process_Insert(me, pprocessnew[i])); 
	}

	me->sort = TRUE;
	EXCCHECK( process_Sort(me));

Error:
	me->sort = TRUE;    
	free(pprocessnew);
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "process_LineProcessDel"
static SElExceptionPtr process_LineProcessDel(
	struct _SDBSLineProcessList* me, 
	SLineProcess process
)
{
	SElExceptionPtr	pexception = NULL; 
	int				i, j;
		
	if(0==process.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");

	/* set current steps and all skips not valid */
	for(i=0; i<me->LineProcessSize; i++)
	{
		if( (me->LineProcess[i].property&PROPERTY_VALID)>0
			&& me->LineProcess[i].process_nb == process.process_nb)
		{
			me->LineProcess[i].property ^= PROPERTY_VALID; 
		}	
	}
	
	/* set current process steps not valid without log*/ 
	for(i=0; i<PSTEP->LineStepSize; i++)
	{
		if( (PSTEP->LineStep[i].property&PROPERTY_VALID)>0
			&& PSTEP->LineStep[i].process_nb == process.process_nb)
		{
			PSTEP->LineStep[i].property ^= PROPERTY_VALID;

			for(j=0; j<PLINEPARAM->LineParamSize; j++)
			{
				if( (PLINEPARAM->LineParam[j].property&PROPERTY_VALID)>0
					&& PLINEPARAM->LineParam[j].fnc_nb == PSTEP->LineStep[i].fnc_nb)
				{
					PLINEPARAM->LineParam[j].property ^= PROPERTY_VALID;
				}
			}
		}	
	}

	PDBS->data_changed_line	= TRUE;

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/  
#undef __FUNC__
#define __FUNC__ "process_Allocate"
static SElExceptionPtr process_Allocate(struct _SDBSLineProcessList* me, int32_t size) 
{
	SElExceptionPtr    pexception = NULL;   

	if(me->_Allocated == 0)
	{
		me->LineProcess = calloc(size+MEMORY_ALLOCATION_RESERVE, sizeof(SLineProcess)); 
		EXCCHECKALLOC(me->LineProcess);
		me->_Allocated = size+MEMORY_ALLOCATION_RESERVE;
	}
	else if(size>me->_Allocated)   
	{
		me->LineProcess = realloc(me->LineProcess, (size+MEMORY_ALLOCATION_RESERVE)*sizeof(SLineProcess)); 
		EXCCHECKALLOC(me->LineProcess);
		memset( me->LineProcess + me->_Allocated, 
				0, 
				(size+MEMORY_ALLOCATION_RESERVE - me->_Allocated)*sizeof(SLineProcess));
		me->_Allocated = size+MEMORY_ALLOCATION_RESERVE;
	}
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "process_CheckAlloc"
static SElExceptionPtr process_CheckAlloc(struct _SDBSLineProcessList* me)
{
	SElExceptionPtr    pexception = NULL;   
	
	if(me->_Allocated - me->LineProcessSize < MEMORY_ALLOCATION_MIN )
	{
		me->LineProcess = realloc( me->LineProcess, 
							(me->_Allocated + MEMORY_ALLOCATION_RESERVE)*sizeof(SLineProcess));
		EXCCHECKALLOC(me->LineProcess);
		/* clear memory */
		memset( me->LineProcess + me->_Allocated,
				0,
				MEMORY_ALLOCATION_RESERVE*sizeof(SLineProcess));
		me->_Allocated += MEMORY_ALLOCATION_RESERVE;
	}
		
Error:
	EXCRETHROW( pexception); 
}

