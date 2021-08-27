#include "dbs.h"
#include "dbs_testerlist.h"
#include <stdio.h>
#include <odbc/elodbc.h>
#include "dbs_define_local.h"

static SElExceptionPtr tester_TesterRead(struct _SDBSTesterList* me); 
static SElExceptionPtr tester_TesterGet(struct _SDBSTesterList* me, STesterItemPtr* tester);
static SElExceptionPtr tester_TesterGetSelected(struct _SDBSTesterList* me, int32_t tester_id, STesterItemPtr* tester); 

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbstesterlist_new"
SElExceptionPtr dbstesterlist_new(SDBSTesterListPtr* pDBSTesterListPtr, void* pDBS)
{
	SElExceptionPtr    pexception = NULL;
	SDBSTesterListPtr	me = NULL;

	me = calloc(1,sizeof(SDBSTesterList));
	EXCCHECKALLOC(me);
	
	if(pDBSTesterListPtr) *pDBSTesterListPtr = me;

	me->TesterRead = tester_TesterRead;
	me->TesterGet = tester_TesterGet;
	me->TesterGetSelected = tester_TesterGetSelected;

	me->pdbs = pDBS;

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbstesterlist_delete"
SElExceptionPtr dbstesterlist_delete(SDBSTesterListPtr* pDBSTesterListPtr)
{
	int					i;
	
	if (pDBSTesterListPtr && *pDBSTesterListPtr)
	{
		SDBSTesterListPtr	me = *pDBSTesterListPtr;

		for(i=0; i<me->TesterSize; i++)
		{
			if(me->Tester[i].description)
			{
				free(me->Tester[i].description);
				me->Tester[i].description = NULL;
			}
		}

		if(me->Tester)
		{
			free(me->Tester);
			me->Tester = NULL;
		}

		free(*pDBSTesterListPtr);
		*pDBSTesterListPtr = NULL;
	}

/* Error: */
	return NULL;
}

/*---------------------------------------------------------------------------*/  
#undef __FUNC__
#define __FUNC__ "tester_TesterRead"
static SElExceptionPtr tester_TesterRead(struct _SDBSTesterList* me)
{
	int32_t			error = 0;
	SElExceptionPtr    pexception = NULL;
	int32_t			count, index, i;
	char			sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr		podbc = PDBS->podbc;
	int32_t			continuityErr = 0;
	size_t			desc_len;
	
	LOCK;

	/* clear old data */
	if(me->Tester)
	{
		for(i=0; i<me->TesterSize; i++)
		{
			if(me->Tester[i].description) 
				free(me->Tester[i].description);
		}

		free(me->Tester);
		me->Tester = NULL;
	}
	
	/* select items */														
	sprintf(sql, "SELECT tester.tester_id, tester.product_id, tester.description " 
		 		 "FROM tester.tester "
				 "ORDER BY tester_id");
	EXCCHECK( podbc->ExecSQL( podbc, sql));
	EXCCHECK( podbc->GetRows(podbc, &count));

	/* memory allocation */
	me->Tester = calloc(count, sizeof(STesterItem));

	CHECKERR( podbc->Fetch( podbc, TRUE));
	
	for(index=0;index<count;index++)
	{
		me->Tester[index].tester_id 	= *((long*)podbc->GetFieldValuePtr(podbc, 1)); 
		me->Tester[index].product_id  	= *((long*)podbc->GetFieldValuePtr(podbc, 2));
		desc_len = strlen((char*)podbc->GetFieldValuePtr(podbc, 3));
		if(desc_len)
		{
			me->Tester[index].description = calloc(desc_len+1, sizeof(char));
			strncpy(me->Tester[index].description, (char*)podbc->GetFieldValuePtr(podbc, 3), desc_len);
		}
 
		CHECKERR( podbc->MoveNext(podbc));
	}
	
	me->TesterSize = index;
	
	EXCCHECK( podbc->Cancel(podbc));

Error:
	UNLOCK;
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "tester_TesterGet"
static SElExceptionPtr tester_TesterGet(struct _SDBSTesterList* me, STesterItemPtr* tester)
{
	SElExceptionPtr    pexception = NULL;
	int			i;

	for(i=0; i<me->TesterSize; i++)
	{
		if(me->Tester[i].tester_id == PDBS_TESTER_ID)
		{
			*tester = &me->Tester[i];
			break;
		}
	}

	/* not found */
	if(i==me->TesterSize)
		*tester = NULL;

/* Error: */
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "tester_TesterGetSelected"
static SElExceptionPtr tester_TesterGetSelected(
	struct _SDBSTesterList* me, 
	int32_t tester_id, 
	STesterItemPtr* tester
)
{
	SElExceptionPtr    pexception = NULL;
	int			i;

	for(i=0; i<me->TesterSize; i++)
	{
		if(me->Tester[i].tester_id == tester_id)
		{
			*tester = &me->Tester[i];
			break;
		}
	}

	/* not found */
	if(i==me->TesterSize)
		*tester = NULL;

/* Error: */
	EXCRETHROW( pexception);
}
