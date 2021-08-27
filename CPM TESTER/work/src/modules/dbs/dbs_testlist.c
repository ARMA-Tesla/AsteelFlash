#include "dbs.h"
#include "dbs_testlist.h"
#include <stdio.h>
#include <elstring.h>
#include <odbc/elodbc.h>
#include "dbs_define_local.h"

#define LOCK_STR		"test_steps"

static SElExceptionPtr test_TestRead(struct _SDBSTestList* me, int32_t branch_id); 		
static SElExceptionPtr test_TestSave(struct _SDBSTestList* me, int32_t branch_id);	
static SElExceptionPtr test_TestDelHist(struct _SDBSTestList* me, int32_t branch_id);

static SElExceptionPtr test_TestInsert(struct _SDBSTestList* me, STestStep test);						   
static SElExceptionPtr test_TestEdit(struct _SDBSTestList* me, STestStep test); 
static SElExceptionPtr test_TestDel(struct _SDBSTestList* me, STestStep test);  
static SElExceptionPtr test_TestSkip(struct _SDBSTestList* me, STestStep test); 
static SElExceptionPtr test_TestRemoveSkip(struct _SDBSTestList* me, STestStep test);
static SElExceptionPtr test_TestSort(struct _SDBSTestList* me);
static SElExceptionPtr test_TestCopy(struct _SDBSTestList* me, int32_t pidSrc[], int32_t pidTrg[], int32_t pidSize);
static SElExceptionPtr test_TestMove(struct _SDBSTestList* me, STestStep test, ETestStepMove pos, int32_t test_nb);

static SElExceptionPtr test_TestGetName(struct _SDBSTestList* me, int32_t	test_nb, char** name);

static SElExceptionPtr test_TestSort(struct _SDBSTestList* me);   
static SElExceptionPtr test_Insert(struct _SDBSTestList* me, STestStep test);
static SElExceptionPtr test_CheckAlloc(struct _SDBSTestList* me);
static SElExceptionPtr test_Allocate(struct _SDBSTestList* me, int32_t size);

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbstestlist_new"
SElExceptionPtr dbstestlist_new(SDBSTestListPtr* pDBSTestListPtr, void* pDBS)
{
	SElExceptionPtr	pexception = NULL;
	SDBSTestListPtr	me = NULL;

	me = calloc(1,sizeof(SDBSTestList));
	EXCCHECKALLOC(me);
	
	*pDBSTestListPtr = me;
	
	me->TestRead	= test_TestRead;
	me->TestSave	= test_TestSave;
	me->TestDelHist = test_TestDelHist;

	me->TestInsert 		= test_TestInsert;
	me->TestEdit	 	= test_TestEdit; 
	me->TestDel	 		= test_TestDel; 
	me->TestSkip	 	= test_TestSkip; 
	me->TestRemoveSkip	= test_TestRemoveSkip; 
	me->TestSort		= test_TestSort;
	me->TestCopy		= test_TestCopy;
	me->TestMove		= test_TestMove;

	me->TestGetName		= test_TestGetName;
	
	me->pdbs = pDBS;
	me->sort = TRUE;

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbstestlist_delete"
SElExceptionPtr dbstestlist_delete(SDBSTestListPtr* pDBSTestListPtr)
{
	int	i;
	
	if (pDBSTestListPtr && *pDBSTestListPtr)
	{
		SDBSTestListPtr	me = *pDBSTestListPtr;

		for(i=0; i<me->_Allocated; i++)
		{
			if(me->Test[i].description)
			{
				free(me->Test[i].description);
				me->Test[i].description = NULL;
			}
		}
		
		if(me->Test)
		{
			free(me->Test);
			me->Test = NULL;
		}

		free(*pDBSTestListPtr);
		*pDBSTestListPtr = NULL;
	}

/* Error: */
	return NULL;
}

/*---------------------------------------------------------------------------*/  
#undef __FUNC__
#define __FUNC__ "test_TestRead"
static SElExceptionPtr test_TestRead(struct _SDBSTestList* me, int32_t branch_id)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL; 
	int32_t			count, index;
	char			sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr		podbc = PDBS->podbc;
	size_t			desc_len;  
	
	LOCK;

	/* lock database tester.test_steps */
	PDBS_LOCK(LOCK_STR);
	
	/* select items */
	sprintf(sql, "SELECT ts.product_id,ts.process_nb,ts.test_nb,ts.name,ts.description,ts.time,ts.user_id,ts.property,ts.test_id " 
				 "FROM tester.product p INNER JOIN tester.test_steps ts "
				 "ON p.product_id=ts.product_id "
				 "WHERE p.branch_id=%d "
				 "ORDER BY ts.process_nb, ts.test_nb, ts.product_id", 
				 branch_id);
	EXCCHECK( podbc->ExecSQL(podbc, sql));
	EXCCHECK( podbc->GetRows(podbc, &count));

	/* memory allocation */
	EXCCHECK( test_Allocate(me, count));

	CHECKERR( podbc->Fetch(podbc, TRUE));
	
	for(index=0;index<count;index++)
	{
		STestStepPtr ptest = me->Test;
	
		ptest[index].product_id = *((long*)podbc->GetFieldValuePtr(podbc, 1));
		ptest[index].process_nb = *((long*)podbc->GetFieldValuePtr(podbc, 2));
		ptest[index].test_nb    = *((long*)podbc->GetFieldValuePtr(podbc, 3));
		strcpy(ptest[index].name, (char*)podbc->GetFieldValuePtr(podbc, 4));
		
		if(ptest[index].description!=NULL)
		{
			free(ptest[index].description);
			ptest[index].description = NULL;
		}
		
		desc_len = strlen((char*)podbc->GetFieldValuePtr(podbc, 5));
		if(desc_len)
		{
			ptest[index].description = calloc(desc_len+1, sizeof(char));
			strcpy(ptest[index].description, (char*)podbc->GetFieldValuePtr(podbc, 5));
		}
		strcpy(ptest[index].time, (char*)podbc->GetFieldValuePtr(podbc, 6));
		ptest[index].user_id	= *((long*)podbc->GetFieldValuePtr(podbc, 7));
		ptest[index].property 	= *((short*)podbc->GetFieldValuePtr(podbc, 8));
		ptest[index].test_id 	= *((unsigned long*)podbc->GetFieldValuePtr(podbc, 9));

		CHECKERR( podbc->MoveNext(podbc));
	}
	
	me->TestSize = index;
	
	EXCCHECK( podbc->Cancel(podbc));

	/* sort because of @skip */
	EXCCHECK( me->TestSort(me));
	
Error:
	PDBS_UNLOCK(LOCK_STR);
	UNLOCK;
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "test_TestSave"
static SElExceptionPtr test_TestSave(struct _SDBSTestList* me, int32_t branch_id)
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

	if(me->TestSize)
	{
		uint32_t last_test_id;

		/* get last test_id */
		EXCCHECK( podbc->ExecSQL( podbc, "SELECT MAX(ts.test_id) FROM tester.test_steps ts"));
		CHECKERR( podbc->Fetch( podbc, TRUE));
		if(error==ELODBC_SQL_WARN_NO_DATA_FOUND)
			last_test_id = 0;
		else
			last_test_id = *((unsigned long*)podbc->GetFieldValuePtr(podbc, 1));

		EXCCHECK( podbc->Cancel(podbc));
	
		EXCCHECK( elstring_new(NULL, &pelstring));
		for(i=0; i<me->TestSize;)
		{	
			EXCCHECK(pelstring->Empty(pelstring));
	
			if(me->Test[i].test_id==0)
				me->Test[i].test_id = ++last_test_id;

			sprintf(sql, "REPLACE INTO tester.test_steps "
						 "(product_id,process_nb,test_nb,name,description,time,user_id,property,test_id) "
						 "VALUES (%d,%d,%d,'%s',_UTF8'%s','%s',%d,%d,%d)", 
						 me->Test[i].product_id,
						 me->Test[i].process_nb,
						 me->Test[i].test_nb,
						 me->Test[i].name,			
						 GET_STRING(me->Test[i].description), 
						 me->Test[i].time,
						 me->Test[i].user_id,
						 me->Test[i].property,
						 me->Test[i].test_id);
	
			EXCCHECK(pelstring->Append(pelstring, sql) );
		
			i++;
			j = 0;
			
			for(; i<me->TestSize; i++)
			{
				if(++j>SQL_COMMAND_NB_OF_INSERTED_ROW)
					break;
			
				if(me->Test[i].test_id==0)
					me->Test[i].test_id = ++last_test_id;

				sprintf(sql, ",(%d,%d,%d,'%s',_UTF8'%s','%s',%d,%d,%d)", 
							 me->Test[i].product_id,
							 me->Test[i].process_nb,
							 me->Test[i].test_nb,
							 me->Test[i].name,			
							 GET_STRING(me->Test[i].description), 
							 me->Test[i].time,
							 me->Test[i].user_id,
							 me->Test[i].property,
							 me->Test[i].test_id);
		
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
#define __FUNC__ "test_TestDelHist"
static SElExceptionPtr test_TestDelHist(struct _SDBSTestList* me, int32_t branch_id)
{
	SElExceptionPtr	pexception = NULL; 
	SOdbcPtr		podbc = PDBS->podbc;
	char			sql[SQL_COMMAND_LENGHT];  
	
	LOCK;

	sprintf(sql, "DELETE FROM tester.test_steps "
				 "WHERE (test_steps.property&%d)=0 AND test_steps.product_id IN "
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

/* sort algorithm for test structure array 
 * priority: 	1) test_nb
 *				2) PROPERTY_VALID
 *				3) product_id
 */
int compare_Test ( const void* a, const void* b ) 
{
	STestStepPtr aa, bb;
	
	aa = (STestStepPtr)a;
	bb = (STestStepPtr)b;
	
	if(aa->test_nb != bb->test_nb)
	{
		return ( aa->test_nb - bb->test_nb );
	}
	else if( (aa->property&PROPERTY_VALID) != (bb->property&PROPERTY_VALID) )
	{
		return ( (bb->property&PROPERTY_VALID) - (aa->property&PROPERTY_VALID) );
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
#undef __FUNC__
#define __FUNC__ "test_Insert"
static SElExceptionPtr test_Insert(struct _SDBSTestList* me, STestStep test)
{ 
	SElExceptionPtr	pexception = NULL; 
	
	/* check free space */
	EXCCHECK( test_CheckAlloc(me) );
	
	if(me->Test[me->TestSize].description)
		free(me->Test[me->TestSize].description);

	/* complete record */
	DATETIME_TO_STRING(test.time);
	test.test_id = 0;

	me->Test[me->TestSize++] = test;
	
	if(me->sort)
		qsort(me->Test, me->TestSize, sizeof(STestStep), compare_Test); 
	
	PDBS->data_changed	= TRUE;  
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "test_TestSort"
static SElExceptionPtr test_TestSort(
	struct _SDBSTestList* me
)
{
	SElExceptionPtr    pexception = NULL; 
	
	qsort(me->Test, me->TestSize, sizeof(STestStep), compare_Test); 
	
/* Error: */
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
/* TEST SECTION **************************************************************/
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "test_TestInsert"
static SElExceptionPtr test_TestInsert(
	struct _SDBSTestList* me, 
	STestStep test)
{
	SElExceptionPtr	pexception = NULL; 
	int				i;
	
	if(0==test.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");

	
	/* reindex all tests */
	for(i=0; i<me->TestSize; i++)
	{
		if( me->Test[i].test_nb >= test.test_nb )
			me->Test[i].test_nb++;
	}
	
	/* reindex all parameters */
	for(i=0; i<PTESTPARAM->TestParameterSize; i++)
	{
		if( PTESTPARAM->TestParameter[i].test_nb >= test.test_nb )
			PTESTPARAM->TestParameter[i].test_nb++;
	}
	
	/* set new record */
	test.property = PROPERTY_CREATED|PROPERTY_VALID;
	
	EXCCHECK( test_Insert(me, test));  
		
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "test_TestEdit"
static SElExceptionPtr test_TestEdit(
	struct _SDBSTestList* me, 
	STestStep test)
{
	SElExceptionPtr	pexception = NULL; 
	int				i;
	
	if(0==test.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");

	/* set actual not valid */
	for(i=0; i<me->TestSize; i++)
	{
		if( (me->Test[i].property&PROPERTY_VALID)>0
			&& me->Test[i].test_nb == test.test_nb
			&& 0!=strcmp(me->Test[i].name, "@skip") )
		{
			me->Test[i].property ^= PROPERTY_VALID;	
			break;
		}
	}
	
  	/* set new record */     
	test.property = PROPERTY_EDITED|PROPERTY_VALID;

	EXCCHECK( test_Insert(me, test)); 	
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "test_TestDel"
static SElExceptionPtr test_TestDel(
	struct _SDBSTestList* me, 
	STestStep test
)
{
	SElExceptionPtr	pexception = NULL; 
	int				i;

	if(0==test.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");
		
	/* set current tests and all skips not valid */
	for(i=0; i<me->TestSize; i++)
	{
		if( (me->Test[i].property&PROPERTY_VALID)>0
			&& me->Test[i].test_nb == test.test_nb )
		{
			me->Test[i].property ^= PROPERTY_VALID; 
		}	
	}
	
	/* set new record */     
	test.description = PDBS_COPY(test.description);
	test.property = PROPERTY_DELETED;
	
	EXCCHECK( test_Insert(me, test)); 
	
	/* set current test parameters not valid without log*/ 
	for(i=0; i<PTESTPARAM->TestParameterSize; i++)
	{
		if( (PTESTPARAM->TestParameter[i].property&PROPERTY_VALID)>0
			&& PTESTPARAM->TestParameter[i].test_nb == test.test_nb )
		{
			PTESTPARAM->TestParameter[i].property ^= PROPERTY_VALID;
		}	
	}

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "test_TestMove"
static SElExceptionPtr test_TestMove(
	struct _SDBSTestList* me, 
	STestStep test, 
	ETestStepMove pos, 
	int32_t test_nb
)
{
	SElExceptionPtr	pexception = NULL; 
	int				i;
	int32_t			test_nb_original = test.test_nb;
	bool_t			sort_param = FALSE;

	if(0==test.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");

	/* insert new test */
	test.test_nb = (pos==TSM_PREV)? test_nb:test_nb+1;
	test.description = PDBS_COPY(test.description);
	EXCCHECK( me->TestInsert(me, test));

	if(test_nb_original>test.test_nb)
		test_nb_original++;

	/* delete old test */
	for(i=0; i<me->TestSize; i++)
	{
		if( (me->Test[i].property&PROPERTY_VALID)>0
			&& me->Test[i].test_nb == test_nb_original)
		{
			if(0==strcmp(me->Test[i].name, "@skip"))
				me->Test[i].test_nb = test.test_nb; 
			else
				me->Test[i].property ^= PROPERTY_VALID;
		}	
	}
	   
	test.test_nb = test_nb_original;
	test.description = PDBS_COPY(test.description);
	test.property = PROPERTY_MOVED;

	EXCCHECK( test_Insert(me, test)); 

	/* reindex old test parameters */
	for(i=0; i<PTESTPARAM->TestParameterSize; i++)
	{
		if( (PTESTPARAM->TestParameter[i].property&PROPERTY_VALID)>0
			&& PTESTPARAM->TestParameter[i].test_nb == test_nb_original)
		{
			PTESTPARAM->TestParameter[i].test_nb = (pos==TSM_PREV)? test_nb:test_nb+1;
			sort_param = TRUE;
		}	
	}

	if(sort_param)
		EXCCHECK( PTESTPARAM->TestParamSort(PTESTPARAM));

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "test_TestSkip"
static SElExceptionPtr test_TestSkip(
	struct _SDBSTestList* me, 
	STestStep test
)
{
	SElExceptionPtr	pexception = NULL; 
	int				i;

	if(0==test.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");
		
	/* check duplication */
	for(i=0; i<me->TestSize; i++)
	{
		if( (me->Test[i].property&PROPERTY_VALID)>0
			&& me->Test[i].test_nb == test.test_nb
			&& me->Test[i].product_id == test.product_id
			&& 0==strcmp(me->Test[i].name, "@skip"))
		{
			EXCTHROW(DBS_ERROR_DUPLICATE_ITEM, "Duplicate item!");
		}
	}
	
	/* set new record */ 
	test.property = PROPERTY_CREATED|PROPERTY_VALID;
	strcpy(test.name, "@skip");
	test.description = NULL;
	
	EXCCHECK( test_Insert(me, test) );
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "test_TestRemoveSkip"
static SElExceptionPtr test_TestRemoveSkip(
	struct _SDBSTestList* me, 
	STestStep test
)
{
	SElExceptionPtr	pexception = NULL; 
	int				i; 
	
	if(0==test.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");
	
	/* set current skip not valid */
	for(i=0; i<me->TestSize; i++)
	{
		if( (me->Test[i].property&PROPERTY_VALID)>0 
			&& me->Test[i].test_nb == test.test_nb
			&& me->Test[i].product_id == test.product_id
			&& 0==strcmp(me->Test[i].name, "@skip") )
		{
			me->Test[i].property ^= PROPERTY_VALID;
			break;
		}	
	}

	if(i==me->TestSize)
	{
		/* item was not found */	
	}
	else
	{
		/* set new record */
		test.property = PROPERTY_DELETED;
		strcpy(test.name, "@skip");
		test.description = NULL;
		
		EXCCHECK( test_Insert(me, test) );
	}
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "test_TestCopy"
static SElExceptionPtr test_TestCopy(
	struct _SDBSTestList* me, 
	int32_t pidSrc[], 
	int32_t pidTrg[], 
	int32_t pidSize
)
{
	SElExceptionPtr	pexception = NULL;
	STestStepPtr 	ptestnew = NULL;
	STestStepPtr 	pd_ptestnew = NULL;
	int32_t			next_test_nb = 0;
	int32_t			testSize = 0;
	int32_t			shift = 0;
	int32_t			test_index = 0;
	int32_t			i, j, k;

	/* disable sorting algorithm because of program speed */ 
	me->sort = FALSE;
	
	/* precount */
	for(i=0; i<me->TestSize; i++)
	{
		for(j=0; j<pidSize; j++)
		{
			if( (me->Test[i].property&PROPERTY_VALID)>0
				&&pidSrc[j] == me->Test[i].product_id )
			{
				testSize++;	
				break;
			}
		}
	}
	
	ptestnew = calloc(testSize, sizeof(STestStep));
	pd_ptestnew = ptestnew;
	
	/* shift the tests */
	for(i=0; i<me->TestSize; i++)
	{
		/* shift test parameters */
		for(j=test_index; j<PTESTPARAM->TestParameterSize; j++) 
		{
			if(PTESTPARAM->TestParameter[j].test_nb==me->Test[i].test_nb)
			{
				PTESTPARAM->TestParameter[j].test_nb += shift;
			}
			else if(PTESTPARAM->TestParameter[j].test_nb>me->Test[i].test_nb) 
			{
				test_index = j;
				break;
			}
		}
	
		/* shift test */
		me->Test[i].test_nb += shift;
		
		for(j=0; j<pidSize; j++)
		{
			if( (me->Test[i].property&PROPERTY_VALID)>0
				&&pidSrc[j] == me->Test[i].product_id )
			{
				*pd_ptestnew = me->Test[i];
				pd_ptestnew->test_nb++;
				
				/* next test_nb */
				for(k=i+1; k<me->TestSize; k++)
				{
					if( (me->Test[k].property&PROPERTY_VALID)>0 )
					{
						next_test_nb = me->Test[k].test_nb + shift;	
						break;
					}
				}
				
				if(next_test_nb==0 || me->Test[i].test_nb!=next_test_nb)
					shift++; 
				
				for(k=0; k<pidSize; k++)
				{
					if(pidSrc[k] == me->Test[i].product_id)
					{
						pd_ptestnew->product_id = pidTrg[k];
						break;
					}
				}
				
				pd_ptestnew++;
				break;
			}
		}
	}

	/* preallocate memory for a faster process */
	test_Allocate(me, me->TestSize+testSize);
	
	for(i=0; i<testSize; i++)
	{
		ptestnew[i].description = PDBS_COPY( ptestnew[i].description);
		EXCCHECK( test_Insert(me, ptestnew[i])); 
	}

	me->sort = TRUE;
	EXCCHECK( PTEST->TestSort(me));

Error:
	me->sort = TRUE;    
	free(ptestnew);
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "test_TestGetName"
static SElExceptionPtr test_TestGetName(
	struct _SDBSTestList* me, 
	int32_t	test_nb, 
	char** name
)
{
	SElExceptionPtr	pexception = NULL; 
	int				i;
	
	for(i=0; i<me->TestSize; i++)
	{
		if( me->Test[i].test_nb == test_nb )
		{
			*name = me->Test[i].name;
			break;
		}
	}
	
/* Error: */
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/  
#undef __FUNC__
#define __FUNC__ "test_Allocate"
static SElExceptionPtr test_Allocate(struct _SDBSTestList* me, int32_t size) 
{
	SElExceptionPtr    pexception = NULL;   

	if(me->_Allocated == 0)
	{
		me->Test = calloc(size+MEMORY_ALLOCATION_RESERVE, sizeof(STestStep)); 
		EXCCHECKALLOC(me->Test);
		me->_Allocated = size+MEMORY_ALLOCATION_RESERVE;
	}
	else if(size>me->_Allocated)   
	{
		me->Test = realloc(me->Test, (size+MEMORY_ALLOCATION_RESERVE)*sizeof(STestStep)); 
		EXCCHECKALLOC(me->Test);
		memset( me->Test + me->_Allocated, 
				0, 
				(size+MEMORY_ALLOCATION_RESERVE - me->_Allocated)*sizeof(STestStep));
		me->_Allocated = size+MEMORY_ALLOCATION_RESERVE;
	}
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "test_CheckAlloc"
static SElExceptionPtr test_CheckAlloc(struct _SDBSTestList* me)
{
	SElExceptionPtr    pexception = NULL;   
	
	if(me->_Allocated - me->TestSize < MEMORY_ALLOCATION_MIN )
	{
		me->Test = realloc( me->Test, 
							(me->_Allocated + MEMORY_ALLOCATION_RESERVE)*sizeof(STestStep));
		EXCCHECKALLOC(me->Test);
		/* clear memory */
		memset( me->Test + me->_Allocated,
				0,
				MEMORY_ALLOCATION_RESERVE*sizeof(STestStep));
		me->_Allocated += MEMORY_ALLOCATION_RESERVE;
	}
		
Error:
	EXCRETHROW( pexception); 
}

