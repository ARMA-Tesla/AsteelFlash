#include "dbs.h"
#include "dbs_masterlist.h"
#include <elstring.h>
#include <stdio.h>
#include <odbc/elodbc.h>
#include "dbs_define_local.h"

#define LOCK_STR		"product_masters"

static SElExceptionPtr master_ProductMasterRead(struct _SDBSMasterList* me, int32_t branch_id);
static SElExceptionPtr master_ProductMasterSave(struct _SDBSMasterList* me, int32_t branch_id); 
static SElExceptionPtr master_ProductMasterDelHist(struct _SDBSMasterList* me, int32_t branch_id); 

static SElExceptionPtr master_ProductMasterEdit(struct _SDBSMasterList* me,SProductMaster master,SProductMaster masterold);
static SElExceptionPtr master_ProductMasterInsert(struct _SDBSMasterList* me,SProductMaster master);
static SElExceptionPtr master_ProductMasterDel(struct _SDBSMasterList* me, SProductMaster master);
static SElExceptionPtr master_ProductMasterGroupDel(struct _SDBSMasterList* me, SProductMaster master);
static SElExceptionPtr master_ProductMasterGetId(struct _SDBSMasterList* me, int32_t product_id, const char* master_code, int32_t* master_id); 
static SElExceptionPtr master_ProductMasterCopy(struct _SDBSMasterList* me, int32_t pidSrc[], int32_t pidTrg[], int32_t pidSize);

static SElExceptionPtr master_ProductMasterGetCode(struct _SDBSMasterList* me, int32_t product_id, char** master, int32_t* masterSize);
static SElExceptionPtr master_ProductMasterGetBarCodes(struct _SDBSMasterList* me, int32_t product_id, int32_t* count, char** master);


static SElExceptionPtr master_ProductMasterSort(struct _SDBSMasterList* me);
static SElExceptionPtr master_Allocate(struct _SDBSMasterList* me, int32_t size);
static SElExceptionPtr master_CheckAlloc(struct _SDBSMasterList* me);

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbsmasterlist_new"
SElExceptionPtr dbsmasterlist_new(SDBSMasterListPtr* pDBSMasterListPtr, void* pDBS)
{
	SElExceptionPtr    pexception = NULL;
	SDBSMasterListPtr	me = NULL;

	me = calloc(1,sizeof(SDBSMasterList));
	EXCCHECKALLOC(me);
	
	if(pDBSMasterListPtr) *pDBSMasterListPtr = me;

	me->ProductMasterRead = master_ProductMasterRead;
	me->ProductMasterSave = master_ProductMasterSave;
	me->ProductMasterDelHist = master_ProductMasterDelHist;

	me->ProductMasterEdit	= master_ProductMasterEdit;
	me->ProductMasterInsert = master_ProductMasterInsert; 
	me->ProductMasterDel	= master_ProductMasterDel; 
	me->ProductMasterGroupDel	= master_ProductMasterGroupDel;	
	me->ProductMasterSort	= master_ProductMasterSort; 
	me->ProductMasterGetId = master_ProductMasterGetId;
	me->ProductMasterGetCode = master_ProductMasterGetCode;
	me->ProductMasterCopy = master_ProductMasterCopy;
	me->ProductMasterGetBarCodes = master_ProductMasterGetBarCodes;

	me->pdbs = pDBS;
	me->sort = TRUE;

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbsmasterlist_delete"
SElExceptionPtr dbsmasterlist_delete(SDBSMasterListPtr* pDBSMasterListPtr)
{
	if (pDBSMasterListPtr && *pDBSMasterListPtr)
	{
		SDBSMasterListPtr	me = *pDBSMasterListPtr;

		if(me->ProductMaster)
		{
			free(me->ProductMaster);
			me->ProductMaster = NULL;
		}

		free(*pDBSMasterListPtr);
		*pDBSMasterListPtr= NULL;
	}

/* Error: */
	return NULL;
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "master_ProductMasterRead"
static SElExceptionPtr master_ProductMasterRead(struct _SDBSMasterList* me, int32_t branch_id)
{
	int32_t			error = 0;
	SElExceptionPtr    pexception = NULL;    
	int32_t			count, index;
	char			sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr		podbc = PDBS->podbc;
	
	LOCK;
	
	/* lock database tester.product_masters */
	PDBS_LOCK(LOCK_STR);

	/* select items */
	sprintf(sql, "SELECT pm.product_id,pm.master_id,pm.master,pm.time,pm.user_id,pm.property,pm.parameter_id " 
				 "FROM tester.product p INNER JOIN tester.product_masters pm "
				 "ON p.product_id=pm.product_id "
				 "WHERE p.branch_id=%d "
				 "ORDER BY pm.product_id, pm.master",
				 branch_id);
	EXCCHECK( podbc->ExecSQL( podbc, sql));
	EXCCHECK( podbc->GetRows(podbc, &count));

	/* memory allocation */
	EXCCHECK( master_Allocate(me, count));

	CHECKERR( podbc->Fetch( podbc, TRUE));
	
	for(index=0;index<count;index++)
	{
		SProductMasterPtr	pmaster = me->ProductMaster;
		
		pmaster[index].product_id	= *((long*)podbc->GetFieldValuePtr(podbc, 1));
		pmaster[index].master_id	= *((long*)podbc->GetFieldValuePtr(podbc, 2));
		strncpy(pmaster[index].master, (char*)podbc->GetFieldValuePtr(podbc, 3), DBS_RECORD_LENGHT_VALUE);
		strncpy(pmaster[index].time, (char*)podbc->GetFieldValuePtr(podbc, 4), DBS_RECORD_LENGHT_TIME);
		pmaster[index].user_id		= *((long*)podbc->GetFieldValuePtr(podbc, 5));
		pmaster[index].property		= *((short*)podbc->GetFieldValuePtr(podbc, 6));
		pmaster[index].parameter_id	= *((unsigned long*)podbc->GetFieldValuePtr(podbc, 7));
		
		CHECKERR( podbc->MoveNext(podbc));
	}
	
	me->ProductMasterSize = index;
	
	EXCCHECK( podbc->Cancel(podbc));
	
	EXCCHECK( me->ProductMasterSort(me) );   
	
Error:
	PDBS_UNLOCK(LOCK_STR);
	UNLOCK;
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "master_ProductMasterSave"
static SElExceptionPtr master_ProductMasterSave(struct _SDBSMasterList* me, int32_t branch_id)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL;
	int				i, j;
	char			sql[SQL_COMMAND_LENGHT];  
	SOdbcPtr		podbc = PDBS->podbc;
	SELStringPtr   	pelstring = NULL;
	
	LOCK;

	/* lock database tester.product_masters */
	PDBS_LOCK(LOCK_STR);

	if(me->ProductMasterSize)
	{
		uint32_t last_parameter_id;

		/* get last test_id */
		EXCCHECK( podbc->ExecSQL( podbc, "SELECT MAX(pm.parameter_id) FROM tester.product_masters pm"));
		CHECKERR( podbc->Fetch( podbc, TRUE));
		if(error==ELODBC_SQL_WARN_NO_DATA_FOUND)
			last_parameter_id = 0;
		else
			last_parameter_id = *((unsigned long*)podbc->GetFieldValuePtr(podbc, 1));

		EXCCHECK( podbc->Cancel(podbc));
	
		EXCCHECK( elstring_new(NULL, &pelstring) );
		for(i=0; i<me->ProductMasterSize;)
		{	
			EXCCHECK(pelstring->Empty(pelstring) );   
	
			if(me->ProductMaster[i].parameter_id==0)
				me->ProductMaster[i].parameter_id = ++last_parameter_id;

			sprintf(sql, "REPLACE INTO tester.product_masters "
						 "(product_id,master_id,master,time,user_id,property,parameter_id) "
						 "VALUES (%d,%d,'%s','%s',%d,%d,%d)", 
						 me->ProductMaster[i].product_id,
						 me->ProductMaster[i].master_id, 
						 me->ProductMaster[i].master,
						 me->ProductMaster[i].time,
						 me->ProductMaster[i].user_id,
						 me->ProductMaster[i].property,
						 me->ProductMaster[i].parameter_id);
	
			EXCCHECK(pelstring->Append(pelstring, sql));
		
			i++;
			j = 0;
			
			for(; i<me->ProductMasterSize; i++)
			{
				if(++j>SQL_COMMAND_NB_OF_INSERTED_ROW)
					break;
			
				if(me->ProductMaster[i].parameter_id==0)
					me->ProductMaster[i].parameter_id = ++last_parameter_id;

				sprintf(sql, ",(%d,%d,'%s','%s',%d,%d,%d)", 
							 me->ProductMaster[i].product_id,
							 me->ProductMaster[i].master_id, 
							 me->ProductMaster[i].master,
							 me->ProductMaster[i].time,
							 me->ProductMaster[i].user_id,
							 me->ProductMaster[i].property,
							 me->ProductMaster[i].parameter_id);
		
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
#define __FUNC__ "master_ProductMasterDelHist"
static SElExceptionPtr master_ProductMasterDelHist(struct _SDBSMasterList* me, int32_t branch_id)
{
	SElExceptionPtr	pexception = NULL;    
	SOdbcPtr		podbc = PDBS->podbc;
	char			sql[SQL_COMMAND_LENGHT];  
	
	LOCK;

	sprintf(sql, "DELETE FROM tester.product_masters "
				 "WHERE (product_masters.property&%d)=0 AND product_masters.product_id IN "
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
/* sort algorithm for product master structure array 
 * priority: 	1) product_id
 *				2) PROPERTY_VALID
 *				3) master
 */
int compare_ProductMaster ( const void* a, const void* b ) 
{
	SProductMasterPtr aa, bb;
	
	aa = (SProductMasterPtr)a;
	bb = (SProductMasterPtr)b;
	
	if( aa->product_id - bb->product_id )
	{
		return ( aa->product_id - bb->product_id );
	}
	else if( (aa->property&PROPERTY_VALID) != (bb->property&PROPERTY_VALID) )
	{
		return ( (bb->property&PROPERTY_VALID) - (aa->property&PROPERTY_VALID) );
	}
	else
	{
		return ( strcmp(aa->master, bb->master) );  
	}
}		

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "master_Insert"
static SElExceptionPtr master_Insert(struct _SDBSMasterList* me, SProductMaster master)
{
	SElExceptionPtr	pexception = NULL;    
	
	/* check free space */
	EXCCHECK( master_CheckAlloc(me) );
	
	/* complete record */
	master.parameter_id = 0;

	me->ProductMaster[me->ProductMasterSize++] = master;

	if(me->sort)  
		qsort(me->ProductMaster, me->ProductMasterSize, sizeof(SProductMaster), compare_ProductMaster);
	
	PDBS->data_changed	= TRUE; 
	
Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "master_ProductMasterSort"
static SElExceptionPtr master_ProductMasterSort(struct _SDBSMasterList* me)
{
	SElExceptionPtr	pexception = NULL;    
	
	qsort(me->ProductMaster, me->ProductMasterSize, sizeof(SProductMaster), compare_ProductMaster);	
	
/* Error: */
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
/* PRODUCT MASTER SECTION ****************************************************/   
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "master_ProductMasterEdit"
static SElExceptionPtr master_ProductMasterEdit(
	struct _SDBSMasterList* me, 
	SProductMaster master,
	SProductMaster masterold
)
{
	SElExceptionPtr	pexception = NULL;    

	EXCCHECK( me->ProductMasterDel(me, masterold) );
	EXCCHECK( me->ProductMasterInsert(me, master) );
	
Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "master_ProductMasterInsert"
static SElExceptionPtr master_ProductMasterInsert(
	struct _SDBSMasterList* me, 
	SProductMaster master
)
{
	SElExceptionPtr	pexception = NULL;    
	int				i;
	SProductMaster 	masterold = {0};
	
	masterold = master;

	if(0==master.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");
	
	/* unknown user */
	if( master.user_id <0 )
		master.user_id = 0;
	
	/* check duplication */
	for(i=0; i<me->ProductMasterSize; i++)
	{
		if((me->ProductMaster[i].property&PROPERTY_VALID)>0 
			&& me->ProductMaster[i].product_id == master.product_id
			&& 0==strcmp(me->ProductMaster[i].master, master.master))
		{
			EXCTHROW(-1, "Master Code is already registred!");
		}
	}
	
	/* unknown master id */
	if( master.master_id <0 )
	{
		for(i=0; i<me->ProductMasterSize; i++) 
		{
			if((me->ProductMaster[i].property&PROPERTY_VALID)>0 
			&& me->ProductMaster[i].product_id == master.product_id )
			{
				master.master_id = me->ProductMaster[i].master_id;
				break;
			}
		}
		/* not found */
		if( i==me->ProductMasterSize )
			master.master_id = 0;	
	}

	for(i=0; i<me->ProductMasterSize; i++)
	{
		if((me->ProductMaster[i].property&PROPERTY_VALID)>0 
			&& me->ProductMaster[i].product_id == master.product_id 
			&& 0==strcmp(me->ProductMaster[i].master, "@empty"))
		{
			masterold.master_id=master.master_id;
			strcpy(masterold.master, "@empty");
			EXCCHECK( me->ProductMasterDel(me, masterold) );   
			break;
		}
	}
	
	/* set new record */     
	master.property = PROPERTY_CREATED|PROPERTY_VALID;
	DATETIME_TO_STRING(master.time);

	EXCCHECK( master_Insert(me, master) ); 
	
Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "master_ProductMasterDel"
static SElExceptionPtr master_ProductMasterDel(
   struct _SDBSMasterList* me, 
   SProductMaster master
)
{
	SElExceptionPtr	pexception = NULL;    
	int				i, count = 0;
   
	if(0==master.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");

	/* unknown user */
	if( master.user_id <0 )
		master.user_id = 0;
	
    for(i=0; i<me->ProductMasterSize;i++)
	{
		if( (me->ProductMaster[i].property&PROPERTY_VALID)>0
		 	&& me->ProductMaster[i].product_id == master.product_id )
		{
			count++;
		}
	}
		
	for(i=0; i<me->ProductMasterSize;i++)
	{
		if( (me->ProductMaster[i].property&PROPERTY_VALID)>0
		 	&& me->ProductMaster[i].product_id == master.product_id
			&& 0==strcmp(me->ProductMaster[i].master, master.master))
		{
			/* set new record */     
			master.property = PROPERTY_DELETED;
			DATETIME_TO_STRING(master.time);	
		
			me->ProductMaster[i].property ^= PROPERTY_VALID;
			EXCCHECK( master_Insert(me, master) );	
		
			if(count!=1)
				break;
			else if( 0!=strcmp(master.master, "@empty") ) 
			{
				strcpy(master.master, "@empty");
				master.property = PROPERTY_CREATED|PROPERTY_VALID;
				EXCCHECK( master_Insert(me, master) ); 
				break;
			}
		}
	}

Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "master_ProductMasterGroupDel"
static SElExceptionPtr master_ProductMasterGroupDel(
   struct _SDBSMasterList* me, 
   SProductMaster master
)
{
	SElExceptionPtr	pexception = NULL;    
	int				i;
   
   	if(0==master.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");
   
    /* set new record */     
	master.property = PROPERTY_DELETED;
	DATETIME_TO_STRING(master.time);
	
	for(i=0; i<me->ProductMasterSize;i++)
	{
		if( (me->ProductMaster[i].property&PROPERTY_VALID)>0
		 	&& me->ProductMaster[i].product_id == master.product_id )
		{
			me->ProductMaster[i].property ^= PROPERTY_VALID;
			strncpy(master.master, me->ProductMaster[i].master, DBS_RECORD_LENGHT_VALUE);
			EXCCHECK( master_Insert(me, master) );   
			i=0;
		}
	}

Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "master_ProductMasterCopy"
static SElExceptionPtr master_ProductMasterCopy(
	struct _SDBSMasterList* me, 
	int32_t pidSrc[], 
	int32_t pidTrg[], 
	int32_t pidSize
)
{
	SElExceptionPtr	pexception = NULL;   
	SProductMaster	master = {0};
	int32_t			i, j, k;
	int32_t			sproduct_id, tproduct_id;
		
	/* disable sorting algorithm because of program speed */ 
	me->sort = FALSE;
	
	for(i=0; i<pidSize; i++)
	{
		sproduct_id = pidSrc[i];
		tproduct_id = pidTrg[i];
	
		for(j=0;j<me->ProductMasterSize; j++)
		{
			if( (me->ProductMaster[j].property&PROPERTY_VALID)
				&& me->ProductMaster[j].product_id == sproduct_id)
			{
				master = me->ProductMaster[j];
				master.product_id = tproduct_id;
				
				/* set master_id */
				for(k=0; k<pidSize; k++)
				{
					if( pidSrc[k] == me->ProductMaster[j].master_id)
					{
						master.master_id = pidTrg[k];
						break;
					}
				}
				
				/* unknown user */
				if(master.user_id==0)
					master.user_id = -1;
				
				EXCCHECK( me->ProductMasterInsert(me, master) ); 
			}
		}
	}
	
	me->sort = TRUE;
	EXCCHECK( me->ProductMasterSort(me) );

Error:
	me->sort = TRUE;
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "master_ProductMasterGetId"
static SElExceptionPtr master_ProductMasterGetId(
	struct _SDBSMasterList* me, 
	int32_t product_id, 
	const char* master_code, 
	int32_t* master_id
)
{
	SElExceptionPtr	pexception = NULL;    
	int				i;
	bool_t			run = TRUE;

	*master_id = 0;
		
	do
	{
		for(i=0; i<me->ProductMasterSize; i++)
		{
			if( (me->ProductMaster[i].property&PROPERTY_VALID)>0
			 	&& me->ProductMaster[i].product_id == product_id 
				&& 0==strcmp(me->ProductMaster[i].master, master_code) )
			{	
				run = FALSE;
				*master_id = product_id;
				break;
			}
		}
		
		/* master code not found */
		if( i==me->ProductMasterSize )
		{
			for(i=0; i<me->ProductMasterSize; i++)
			{
				if( (me->ProductMaster[i].property&PROPERTY_VALID)>0
				 	&& me->ProductMaster[i].product_id == product_id 
					&& me->ProductMaster[i].master_id != 0 )
				{	
					product_id = me->ProductMaster[i].master_id;
					break;
				}
			}
			
			/* master id not found */ 
			if( i==me->ProductMasterSize )
			{
				run = FALSE;	
			}
		}
		
	} while(run);
		
/* Error: */
	EXCRETHROW( pexception);  
}

#define MASTER_CODE_SIZE  512
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "master_ProductMasterGetCode"
static SElExceptionPtr master_ProductMasterGetCode(
	struct _SDBSMasterList* me, 
	int32_t product_id, 
	char** master, 
	int32_t* masterSize
)
{
	SElExceptionPtr	pexception = NULL;    
	int				i;
	int32_t			allocSize = *masterSize;
	int32_t			index = 0;
	char*			codes[MASTER_CODE_SIZE];
	
	for(i=0; i<me->ProductMasterSize; i++)
	{
		if( (me->ProductMaster[i].property&PROPERTY_VALID)>0
		 	&& me->ProductMaster[i].product_id == product_id )
		{	
			if(	index<allocSize && index<MASTER_CODE_SIZE )	
				codes[index++] = me->ProductMaster[i].master;
		}
	}
	
	*masterSize = index;
	
	if (index>0)
	memcpy(master, codes, index*sizeof(char*));
	
/* Error: */
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "master_ProductMasterGetBarCodes"
static SElExceptionPtr master_ProductMasterGetBarCodes(
	struct _SDBSMasterList* me, 
	int32_t product_id,
	int32_t* count,
	char** master
)
{
	SElExceptionPtr	pexception = NULL;    
	int				i,j;
	int32_t			index = 0;
	char*			codes[5];
	int32_t			pid[5];
	int32_t			pidSize = 5;

	/* select specified product tree branch */
	EXCCHECK( PPRODUCT->ProductGetIdTreeUp(
									PPRODUCT,
									product_id, 	
									pid, 
									&pidSize));
	
	*count = 0;

	//printf("product_id=%d\r\n",product_id);
	
		for(i=0; i<me->ProductMasterSize; i++)
		{
			for(j=0; j<pidSize; j++)
			{
			if( (me->ProductMaster[i].property&PROPERTY_VALID)>0
			 	&& me->ProductMaster[i].product_id == pid[j])
			{	
			if(index<MASTER_CODE_SIZE )	
				{
				codes[index++] = me->ProductMaster[i].master;
				//printf("codes[index++]=%s\r\n", me->ProductMaster[i].master);
				}
			}
			}
		}
	*count = index;
	//printf("*count=%d\r\n",*count);
	if (index>0)
	memcpy(master, codes, index*sizeof(char*));

		
Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "master_Allocate"
static SElExceptionPtr master_Allocate(struct _SDBSMasterList* me, int32_t size)
{
	SElExceptionPtr    pexception = NULL;    
	
	if(me->_Allocated == 0)
	{
		me->ProductMaster = calloc(size+MEMORY_ALLOCATION_RESERVE, sizeof(SProductMaster));
		EXCCHECKALLOC(me->ProductMaster);
		me->_Allocated = size+MEMORY_ALLOCATION_RESERVE;
	}
	else if(size>me->_Allocated)  
	{
		me->ProductMaster = realloc(me->ProductMaster, (size+MEMORY_ALLOCATION_RESERVE)*sizeof(SProductMaster));
		EXCCHECKALLOC(me->ProductMaster);
		memset( me->ProductMaster + me->_Allocated, 
				0, 
				(size+MEMORY_ALLOCATION_RESERVE - me->_Allocated)*sizeof(SProductMaster));
		me->_Allocated = size+MEMORY_ALLOCATION_RESERVE;
	}
	
Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "master_CheckAlloc"
static SElExceptionPtr master_CheckAlloc(struct _SDBSMasterList* me)
{
	SElExceptionPtr    pexception = NULL;    
	
	if(me->_Allocated - me->ProductMasterSize < MEMORY_ALLOCATION_MIN )
	{
		me->ProductMaster = realloc( me->ProductMaster, 
									 (me->_Allocated+MEMORY_ALLOCATION_RESERVE)*sizeof(SProductMaster));
		EXCCHECKALLOC(me->ProductMaster);
		/* clear memory */
		memset( me->ProductMaster + me->_Allocated,
				0,
				MEMORY_ALLOCATION_RESERVE*sizeof(SProductMaster));
		me->_Allocated += MEMORY_ALLOCATION_RESERVE;
	}
			
Error:
	EXCRETHROW( pexception);  
}
