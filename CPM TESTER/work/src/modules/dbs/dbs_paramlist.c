#include "dbs.h"
#include "dbs_paramlist.h"
#include <stdio.h>
#include <elstring.h>
#include <odbc/elodbc.h>
#include "dbs_define_local.h"

#define LOCK_STR		"product_parameters"

static SElExceptionPtr param_ProductParameterRead(struct _SDBSParamList* me, int32_t branch_id);
static SElExceptionPtr param_ProductParameterSave(struct _SDBSParamList* me, int32_t branch_id);  
static SElExceptionPtr param_ProductParameterDelHist(struct _SDBSParamList* me, int32_t branch_id);

static SElExceptionPtr param_ProductParameterEdit(struct _SDBSParamList* me,SProductParameter parameter);
static SElExceptionPtr param_ProductParameterInsert(struct _SDBSParamList* me,SProductParameter parameter);
static SElExceptionPtr param_ProductParameterDel(struct _SDBSParamList* me, SProductParameter parameter);
static SElExceptionPtr param_ProductParamCopy(struct _SDBSParamList* me, int32_t pidSrc[], int32_t pidTrg[], int32_t pidSize); 

static SElExceptionPtr param_ProductParameterSort(struct _SDBSParamList* me);

static SElExceptionPtr param_ProductParamSetVType(struct _SDBSParamList* me, SProductParameterPtr parameter);

static SElExceptionPtr param_Allocate(struct _SDBSParamList* me, int32_t size);
static SElExceptionPtr param_CheckAlloc(struct _SDBSParamList* me);

static SElExceptionPtr param_ReplaceCRLF(char* multiline_text, char** fomated_text);
static SElExceptionPtr param_PlaceCRLF(char* fomated_text, char** multiline_text);

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbsparamlist_new"
SElExceptionPtr dbsparamlist_new(SDBSParamListPtr* pDBSParListPtr, void* pDBS)
{
	SElExceptionPtr    pexception = NULL;
	SDBSParamListPtr	me = NULL;

	me = calloc(1,sizeof(SDBSParamList));
	EXCCHECKALLOC(me);

	if(pDBSParListPtr) *pDBSParListPtr = me;
	
	me->ProductParamRead = param_ProductParameterRead;
	me->ProductParamSave = param_ProductParameterSave;
	me->ProductParamDelHist = param_ProductParameterDelHist;

	me->ProductParamEdit	= param_ProductParameterEdit;
	me->ProductParamInsert = param_ProductParameterInsert; 
	me->ProductParamDel	= param_ProductParameterDel; 
	me->ProductParamCopy = param_ProductParamCopy;
	me->ProductParamSort	= param_ProductParameterSort;  
	
	me->ProductParamSetVType = param_ProductParamSetVType;

	me->pdbs = pDBS;
	me->sort = TRUE;

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbsparamlist_delete"
SElExceptionPtr dbsparamlist_delete(SDBSParamListPtr* pDBSParListPtr)
{
	int	i;
	
	if (pDBSParListPtr && *pDBSParListPtr)
	{
		SDBSParamListPtr	me = *pDBSParListPtr;

		for(i=0; i<me->_Allocated; i++)
		{
			if(me->ProductParameter[i].description)
			{
				free(me->ProductParameter[i].description);
				me->ProductParameter[i].description = NULL;
			}
			if(me->ProductParameter[i].text)
			{
				free(me->ProductParameter[i].text);
				me->ProductParameter[i].text = NULL;
			}
		}
		
		if(me->ProductParameter)
		{
			free(me->ProductParameter);
			me->ProductParameter = NULL;
		}

		free(*pDBSParListPtr);
		*pDBSParListPtr = NULL;
	}

/* Error: */
	return NULL;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "param_ProductParameterRead"
static SElExceptionPtr param_ProductParameterRead(struct _SDBSParamList* me, int32_t branch_id)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL; 
	int32_t			count, index;
	char			sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr		podbc = PDBS->podbc;
	size_t			desc_len, text_len; 
	
	LOCK;
	
	/* lock database tester.product_parameters */
	PDBS_LOCK(LOCK_STR);

	/* select items */
	sprintf(sql, "SELECT pp.product_id,pp.name,pp.description,pp.value,pp.time,pp.user_id,pp.property,pp.vtype,pp.text,pp.parameter_id,pp.tester_id " 
				 "FROM tester.product p INNER JOIN tester.product_parameters pp "
				 "ON p.product_id=pp.product_id "
				 "WHERE p.branch_id=%d AND (pp.tester_id=0 OR pp.tester_id=%d) "
				 "ORDER BY pp.name, pp.product_id",
				 branch_id,
				 PDBS_TESTER_ID);
	EXCCHECK( podbc->ExecSQL( podbc, sql));
	EXCCHECK( podbc->GetRows(podbc, &count));

	/* memory allocation */
	EXCCHECK( param_Allocate(me, count));

	CHECKERR( podbc->Fetch( podbc, TRUE));
	
	for(index=0;index<count;index++)
	{
		SProductParameterPtr	pparameter = me->ProductParameter;
		
		pparameter[index].product_id	= *((long*)podbc->GetFieldValuePtr(podbc, 1));
		strncpy(pparameter[index].name, (char*)podbc->GetFieldValuePtr(podbc, 2), DBS_RECORD_LENGHT_NAME);
		
		if(pparameter[index].description!=NULL)
		{
			free(pparameter[index].description);
			pparameter[index].description = NULL;
		}
		
		desc_len = strlen((char*)podbc->GetFieldValuePtr(podbc, 3));
		if(desc_len)
		{
			pparameter[index].description = (char*)calloc(desc_len+1, sizeof(char));
			EXCCHECKALLOC(pparameter[index].description); 
			strcpy(pparameter[index].description, (char*)podbc->GetFieldValuePtr(podbc, 3));
		}
		strncpy(pparameter[index].value, (char*)podbc->GetFieldValuePtr(podbc, 4), DBS_RECORD_LENGHT_VALUE_EXT);
		strncpy(pparameter[index].time, (char*)podbc->GetFieldValuePtr(podbc, 5), DBS_RECORD_LENGHT_TIME);
		pparameter[index].user_id	= *((long*)podbc->GetFieldValuePtr(podbc, 6));
		pparameter[index].property	= *((short*)podbc->GetFieldValuePtr(podbc, 7));
		pparameter[index].vtype		= *((short*)podbc->GetFieldValuePtr(podbc, 8));

		if(pparameter[index].text!=NULL)
		{
			free(pparameter[index].text);
			pparameter[index].text = NULL;
		}
		if(pparameter[index].vtype==E_DVT_TEXT)
		{
			text_len = strlen((char*)podbc->GetFieldValuePtr(podbc, 9));
			if(text_len)
			{
				pparameter[index].text = (char*)calloc(text_len+1, sizeof(char));
				EXCCHECKALLOC(pparameter[index].text); 
				strcpy(pparameter[index].text, (char*)podbc->GetFieldValuePtr(podbc, 9));
			}
		}
		pparameter[index].parameter_id	= *((unsigned long*)podbc->GetFieldValuePtr(podbc, 10));
		pparameter[index].tester_id		= *((unsigned long*)podbc->GetFieldValuePtr(podbc, 11));

		CHECKERR( podbc->MoveNext(podbc));
	}
	
	me->ProductParameterSize = index;
	
	EXCCHECK( podbc->Cancel(podbc));
	
	EXCCHECK( me->ProductParamSort(me) );        
	
Error:
	PDBS_UNLOCK(LOCK_STR);
	UNLOCK;
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "param_ProductParameterSave"
static SElExceptionPtr param_ProductParameterSave(struct _SDBSParamList* me, int32_t branch_id)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	int				i, j;
	char			sql[SQL_COMMAND_LENGHT];  
	SOdbcPtr		podbc = PDBS->podbc;
	SELStringPtr   	pelstring = NULL;
	
	LOCK;

	/* lock database tester.product_parameters */
	PDBS_LOCK(LOCK_STR);

	if(me->ProductParameterSize)
	{
		uint32_t last_parameter_id;

		/* get last test_id */
		EXCCHECK( podbc->ExecSQL( podbc, "SELECT MAX(pp.parameter_id) FROM tester.product_parameters pp"));
		CHECKERR( podbc->Fetch( podbc, TRUE));
		if(error==ELODBC_SQL_WARN_NO_DATA_FOUND)
			last_parameter_id = 0;
		else
			last_parameter_id = *((unsigned long*)podbc->GetFieldValuePtr(podbc, 1));

		EXCCHECK( podbc->Cancel(podbc));

		EXCCHECK( elstring_new(NULL, &pelstring));
		for(i=0; i<me->ProductParameterSize;)
		{	
			EXCCHECK(pelstring->Empty(pelstring) );   

			if(me->ProductParameter[i].parameter_id==0)
				me->ProductParameter[i].parameter_id = ++last_parameter_id;

			sprintf(sql, "REPLACE INTO tester.product_parameters "
						 "(product_id,name,description,value,time,user_id,property,vtype,text,parameter_id,tester_id) "
						 "VALUES (%d,'%s',_UTF8'%s','%s','%s',%d,%d,%d,%s%s%s,%d,%d)", 
						 me->ProductParameter[i].product_id,
						 me->ProductParameter[i].name,
						 GET_STRING(me->ProductParameter[i].description),
						 me->ProductParameter[i].value,
						 me->ProductParameter[i].time,
						 me->ProductParameter[i].user_id,
						 me->ProductParameter[i].property,
						 me->ProductParameter[i].vtype,
						 (me->ProductParameter[i].text==NULL)?"":"'",
						 (me->ProductParameter[i].text==NULL)?"NULL":me->ProductParameter[i].text,
						 (me->ProductParameter[i].text==NULL)?"":"'",
						 me->ProductParameter[i].parameter_id,
						 me->ProductParameter[i].tester_id);
	
			EXCCHECK(pelstring->Append(pelstring, sql));
			
			i++;
			j = 0;
			
			for(; i<me->ProductParameterSize; i++)
			{
				if(++j>SQL_COMMAND_NB_OF_INSERTED_ROW)
					break;

				if(me->ProductParameter[i].parameter_id==0)
					me->ProductParameter[i].parameter_id = ++last_parameter_id;

				sprintf(sql, ",(%d,'%s',_UTF8'%s','%s','%s',%d,%d,%d,%s%s%s,%d,%d)", 
							 me->ProductParameter[i].product_id,
							 me->ProductParameter[i].name,
							 GET_STRING(me->ProductParameter[i].description),
							 me->ProductParameter[i].value,
							 me->ProductParameter[i].time,
							 me->ProductParameter[i].user_id,
							 me->ProductParameter[i].property,
							 me->ProductParameter[i].vtype,
							 (me->ProductParameter[i].text==NULL)?"":"'",
							 (me->ProductParameter[i].text==NULL)?"NULL":me->ProductParameter[i].text,
							 (me->ProductParameter[i].text==NULL)?"":"'",
							 me->ProductParameter[i].parameter_id,
							 me->ProductParameter[i].tester_id);
		
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
#define __FUNC__ "param_ProductParameterDelHist"
static SElExceptionPtr param_ProductParameterDelHist(struct _SDBSParamList* me, int32_t branch_id)
{
	SElExceptionPtr	pexception = NULL; 
	SOdbcPtr		podbc = PDBS->podbc;
	char			sql[SQL_COMMAND_LENGHT];  
	
	LOCK;

	sprintf(sql, "DELETE FROM tester.product_parameters "
				 "WHERE (product_parameters.property&%d)=0 AND product_parameters.product_id IN "
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
/* sort algorithm for product parameter structure array 
 * priority: 	1) name
 *				2) PROPERTY_VALID
 *				3) product_id
 */
int compare_ProductParameter ( const void* a, const void* b ) 
{
	SProductParameterPtr aa, bb;
	
	aa = (SProductParameterPtr)a;
	bb = (SProductParameterPtr)b;
	
	if( 0!=strcmp(aa->name, bb->name) )
	{
		return ( strcmp(aa->name, bb->name) );
	}
	else if( (aa->property&PROPERTY_VALID) != (bb->property&PROPERTY_VALID) )
	{
		return ( (bb->property&PROPERTY_VALID) - (aa->property&PROPERTY_VALID) );
	}
	else if( aa->product_id != bb->product_id)
	{
		return ( aa->product_id - bb->product_id );  
	}
	else
	{
		return ( aa->tester_id - bb->tester_id);
	}
}		

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "param_Insert"
static SElExceptionPtr param_Insert(struct _SDBSParamList* me, SProductParameter parameter)
{
	SElExceptionPtr	pexception = NULL; 
	
	/* check free space */
	EXCCHECK( param_CheckAlloc(me) );

	if(me->ProductParameter[me->ProductParameterSize].description)
		free(me->ProductParameter[me->ProductParameterSize].description);
	
	/* complete record */
	parameter.parameter_id = 0;

	me->ProductParameter[me->ProductParameterSize++] = parameter;

	if(me->sort)  
		qsort(me->ProductParameter, me->ProductParameterSize, sizeof(SProductParameter), compare_ProductParameter);
	
	PDBS->data_changed	= TRUE; 
	
Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "param_ProductParameterSort"
static SElExceptionPtr param_ProductParameterSort(struct _SDBSParamList* me)
{
	SElExceptionPtr	pexception = NULL; 
	
	qsort(me->ProductParameter, me->ProductParameterSize, sizeof(SProductParameter), compare_ProductParameter);	
	
/* Error: */
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
/* PRODUCT PARAMETER SECTION *************************************************/   
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "param_ProductParameterEdit"
static SElExceptionPtr param_ProductParameterEdit(
	struct _SDBSParamList* me, 
	SProductParameter parameter
)
{
	SElExceptionPtr	pexception = NULL; 
	int				i;

	if(0==parameter.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");
	
	if(parameter.tester_id>0)
		parameter.tester_id = PDBS_TESTER_ID;

	/* edit selected parameter */
	for(i=0; i<me->ProductParameterSize; i++)
	{
		if( (me->ProductParameter[i].property&PROPERTY_VALID)>0
			&& me->ProductParameter[i].product_id == parameter.product_id
			&& me->ProductParameter[i].tester_id == parameter.tester_id
			&& 0 == strcmp(me->ProductParameter[i].name, parameter.name) )
		{
			me->ProductParameter[i].property ^= PROPERTY_VALID;
			break;
		}
	}

	/* set new record */     
	parameter.property = PROPERTY_EDITED|PROPERTY_VALID;
	DATETIME_TO_STRING(parameter.time);

	EXCCHECK( param_Insert(me, parameter) );       	
	
Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "param_ProductParameterInsert"
static SElExceptionPtr param_ProductParameterInsert(
	struct _SDBSParamList* me, 
	SProductParameter parameter
)
{
	SElExceptionPtr	pexception = NULL; 
	int				i;
	
	if(0==parameter.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");

	if(parameter.tester_id>0)
		parameter.tester_id = 0;
	
	/* check duplication */
	for(i=0; i<me->ProductParameterSize; i++)
	{
		if(me->ProductParameter[i].product_id == parameter.product_id
			&& (me->ProductParameter[i].property&PROPERTY_VALID)>0
			&& 0==strcmp(me->ProductParameter[i].name, parameter.name))
		{
			EXCTHROW( DBS_ERROR_DUPLICATE_ITEM, "Duplicate item!");
		}
	}
	
	/* set new record */     
	parameter.property = PROPERTY_CREATED|PROPERTY_VALID;
	DATETIME_TO_STRING(parameter.time);

	EXCCHECK( param_Insert(me, parameter) ); 
	
Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "param_ProductParameterDel"
static SElExceptionPtr param_ProductParameterDel(
   struct _SDBSParamList* me, 
   SProductParameter parameter
)
{
	SElExceptionPtr	pexception = NULL; 
	int				i, j;
	int				pid[1024];
	int				pidSize = 1024;
   
	if(0==parameter.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");
   
	/* search downtree products id */
	EXCCHECK( PPRODUCT->ProductGetIdTreeDown(PDBS->ProductList, parameter.product_id, pid, &pidSize) );
   
	/* set current parameters not valid */
	for(j=0; j<pidSize; j++)
	{
		for(i=0; i<me->ProductParameterSize;i++)
		{
			if( (me->ProductParameter[i].property&PROPERTY_VALID)>0
				&& me->ProductParameter[i].product_id == pid[j]
				&& ((parameter.tester_id==0)? TRUE:(me->ProductParameter[i].tester_id==parameter.tester_id))
				&& 0==strcmp(me->ProductParameter[i].name, parameter.name) )
			{
				me->ProductParameter[i].property ^= PROPERTY_VALID;
				strncpy(parameter.value, me->ProductParameter[i].value, DBS_RECORD_LENGHT_VALUE_EXT);
				parameter.product_id = me->ProductParameter[i].product_id; 
				parameter.description = PDBS_COPY(parameter.description);
				break;
			}
		}
      
		if(i<me->ProductParameterSize)
		{
			/* set new record */     
			parameter.property = PROPERTY_DELETED;
			DATETIME_TO_STRING(parameter.time);

			EXCCHECK( param_Insert(me, parameter) ); 
		}
	}
   
Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "param_ProductParamCopy"
static SElExceptionPtr param_ProductParamCopy(
	struct _SDBSParamList* me, 
	int32_t pidSrc[], 
	int32_t pidTrg[], 
	int32_t pidSize
)
{
	SElExceptionPtr	pexception = NULL; 
	int32_t			i, j;
	int32_t			sproduct_id, tproduct_id;
	SProductParameter	parameter = {0};
		
	/* disable sorting algorithm because of program speed */ 
	me->sort = FALSE;
	
	for(i=0; i<pidSize; i++)
	{
		sproduct_id = pidSrc[i];
		tproduct_id = pidTrg[i];
	
		for(j=0;j<me->ProductParameterSize; j++)
		{
			if( (me->ProductParameter[j].property&PROPERTY_VALID)
				&& me->ProductParameter[j].product_id == sproduct_id)
			{
				parameter = me->ProductParameter[j];
				parameter.product_id = tproduct_id;
				parameter.description = PDBS_COPY(parameter.description);
				parameter.text = PDBS_COPY(parameter.text);
				EXCCHECK( me->ProductParamInsert(me, parameter));    
			}
		}
	}
	
	me->sort = TRUE;
	EXCCHECK( me->ProductParamSort(me));

Error:
	me->sort = TRUE;
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "param_ProductParamSetVType"
static SElExceptionPtr param_ProductParamSetVType(
	struct _SDBSParamList* me, 
	SProductParameterPtr pparameter
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
		for(i=0; i<me->ProductParameterSize;i++)
		{
			if( (me->ProductParameter[i].property&PROPERTY_VALID)>0
			 	 && me->ProductParameter[i].product_id == pid[j]
				 && 0==strcmp(me->ProductParameter[i].name,pparameter->name) )
			{
				pparameter->vtype = me->ProductParameter[i].vtype;
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
/* MEMORY ALLOCATION FNC *****************************************************/ 
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "param_Allocate"
static SElExceptionPtr param_Allocate(struct _SDBSParamList* me, int32_t size)
{
	SElExceptionPtr    pexception = NULL; 
	
	if(me->_Allocated == 0)
	{
		me->ProductParameter = calloc(size+MEMORY_ALLOCATION_RESERVE, sizeof(SProductParameter));
		EXCCHECKALLOC(me->ProductParameter);
		me->_Allocated = size+MEMORY_ALLOCATION_RESERVE;
	}
	else if(size>me->_Allocated)  
	{
		me->ProductParameter = realloc(me->ProductParameter, (size+MEMORY_ALLOCATION_RESERVE)*sizeof(SProductParameter));
		EXCCHECKALLOC(me->ProductParameter);
		memset( me->ProductParameter+me->_Allocated, 
				0, 
				(size+MEMORY_ALLOCATION_RESERVE-me->_Allocated)*sizeof(SProductParameter));
		me->_Allocated = size+MEMORY_ALLOCATION_RESERVE;
	}
	
Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "param_CheckAlloc"
static SElExceptionPtr param_CheckAlloc(struct _SDBSParamList* me)
{
	SElExceptionPtr	pexception = NULL; 
	
	if(me->_Allocated - me->ProductParameterSize < MEMORY_ALLOCATION_MIN )
	{
		me->ProductParameter = realloc( me->ProductParameter, 
										(me->_Allocated+MEMORY_ALLOCATION_RESERVE)*sizeof(SProductParameter));
		EXCCHECKALLOC(me->ProductParameter);
		/* clear memory */
		memset( me->ProductParameter + me->_Allocated,
				0,
				MEMORY_ALLOCATION_RESERVE*sizeof(SProductParameter));
		me->_Allocated += MEMORY_ALLOCATION_RESERVE;
	}
			
Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "param_RemoveCRLF"
static SElExceptionPtr param_ReplaceCRLF(char* multiline_text, char** fomated_text)
{
	SElExceptionPtr	pexception = NULL; 
	char*			pdata = NULL;
	size_t			length = 0, length2 = 0, count = 0;

	if(multiline_text!=NULL && fomated_text!=NULL)
	{
		length = strlen(multiline_text);
		
		pdata = strchr(multiline_text, '\n');
		while(pdata!=NULL)
		{
			count++;
			pdata = strchr(pdata+1, '\n');	
		};

		length2 = length + (count*strlen("@[CR]"));
		*fomated_text = (char*)realloc(*fomated_text, (length2+1)*sizeof(char));
		EXCCHECKALLOC(*fomated_text);
		memset(*fomated_text, 0, (length2+1)*sizeof(char));

		strcpy(*fomated_text, multiline_text);

		count = 0;
		pdata = strchr(*fomated_text, '\n');
		while(pdata!=NULL)
		{
			memmove(pdata+strlen("@[CR]")-1, pdata, strlen(*fomated_text)-(pdata-*fomated_text));
			memcpy(pdata, "@[CR]", strlen("@[CR]"));
			count++;
			pdata = strchr(pdata+1, '\n');	
		};
	}

Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "param_PlaceCRLF"
static SElExceptionPtr param_PlaceCRLF(char* fomated_text, char** multiline_text)
{
	SElExceptionPtr	pexception = NULL; 
	char*			pdata = NULL;
	size_t			length = 0, count = 0;

	if(multiline_text!=NULL && fomated_text!=NULL)
	{
		length = strlen(fomated_text);
		
		pdata = strstr(fomated_text, "@[CR]");
		while(pdata!=NULL)
		{
			count++;
			pdata = strstr(pdata+1, "@[CR]");	
		};

		*multiline_text = (char*)realloc(*multiline_text, (length+1)*sizeof(char));
		EXCCHECKALLOC(*multiline_text);
		memset(*multiline_text, 0, (length+1)*sizeof(char));

		strcpy(*multiline_text, fomated_text);

		count = 0;
		pdata = strstr(*multiline_text, "@[CR]");
		while(pdata!=NULL)
		{
			memmove(pdata+strlen("\n"), pdata+strlen("@[CR]"), strlen(*multiline_text)-(pdata-*multiline_text));
			memcpy(pdata, "\n", strlen("\n"));
			count++;
			pdata = strstr(pdata+1, "@[CR]");	
		};
	}

Error:
	EXCRETHROW( pexception);  
}
