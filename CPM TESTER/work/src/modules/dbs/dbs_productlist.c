#include "dbs.h"
#include "dbs_productlist.h"
#include <stdio.h>
#include <elstring.h>
#include <odbc/elodbc.h>
#include "dbs_define_local.h"

#define LOCK_STR		"product_types"
#define LOCK_STR2		"product"
#define LOCK_STR3		"product_counter"

static SElExceptionPtr product_ProductRead(struct _SDBSProductList* me, int32_t branch_id);
static SElExceptionPtr product_ProductSave(struct _SDBSProductList* me, int32_t branch_id);

static SElExceptionPtr product_ProductNew(struct _SDBSProductList* me, SProductTypesPtr pproduct);
static SElExceptionPtr product_ProductEdit(struct _SDBSProductList* me, SProductTypes pproduct);  
#if !defined(_NO_TEST) || !defined(_NO_MASTER)
	static SElExceptionPtr product_ProductCopyStruct(struct _SDBSProductList* me, SProductTypes source, SProductTypes target, int32_t mode);
	static SElExceptionPtr product_ProductDel(struct _SDBSProductList* me, SProductTypes pproduct); 
#endif

static SElExceptionPtr product_GetProductIdTreeUp(struct _SDBSProductList* me, int32_t start_pid, int32_t* pid, int32_t* pidSize);
static SElExceptionPtr product_GetProductIdTreeDown(struct _SDBSProductList* me, int32_t start_pid, int32_t* pid, int32_t* pidSize);
static SElExceptionPtr product_GetProductIdTreeUpDown(struct _SDBSProductList* me, int32_t start_pid, int32_t* pid, int32_t* pidSize);  
static SElExceptionPtr product_GetProductIdName(struct _SDBSProductList* me, int32_t pid, char** name);
static SElExceptionPtr product_GetProductIdDescription(struct _SDBSProductList* me, int32_t pid, char** description);
static SElExceptionPtr product_GetProductIdPath(struct _SDBSProductList* me, int32_t pid, char* path, int32_t level); 
static SElExceptionPtr product_GetProductIdFromPath(struct _SDBSProductList* me, char* path, int32_t* pid);      
static SElExceptionPtr product_GetProductIdFirstValid(struct _SDBSProductList* me, int32_t* pid);  
static SElExceptionPtr product_ProductIsValidEndProduct(struct _SDBSProductList* me, int32_t product_id, bool_t* valid);
static SElExceptionPtr product_ProductIsValidProduct(struct _SDBSProductList* me, int32_t product_id, bool_t* valid);

static SElExceptionPtr product_CheckAlloc(struct _SDBSProductList* me);
static SElExceptionPtr product_Allocate(struct _SDBSProductList* me, int32_t size);

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbsproductlist_new"
SElExceptionPtr dbsproductlist_new(SDBSProductListPtr* pDBSProductListPtr, void* pDBS)
{
	SElExceptionPtr    pexception = NULL;
	SDBSProductListPtr	me = NULL;

	me = calloc(1,sizeof(SDBSProductList));
	EXCCHECKALLOC(me);

	if(pDBSProductListPtr) *pDBSProductListPtr = me;
	
	me->ProductRead		= product_ProductRead;
	me->ProductSave		= product_ProductSave;

	me->ProductNew		= product_ProductNew;
#if !defined(_NO_TEST) || !defined(_NO_MASTER)
	me->ProductCopy		= product_ProductCopyStruct;
	me->ProductDel		= product_ProductDel;
#endif
	me->ProductEdit		= product_ProductEdit; 

	me->ProductGetIdTreeDown 	= product_GetProductIdTreeDown;
	me->ProductGetIdTreeUp 		= product_GetProductIdTreeUp;
	me->ProductGetIdTreeUpDown	= product_GetProductIdTreeUpDown;
	me->ProductGetIdName		= product_GetProductIdName;
	me->ProductGetIdDescription	= product_GetProductIdDescription;
	me->ProductGetIdPath		= product_GetProductIdPath;
	me->ProductGetIdFromPath 	= product_GetProductIdFromPath;
	me->ProductGetIdFirstValid	= product_GetProductIdFirstValid;
	me->ProductIsValidEndProduct = product_ProductIsValidEndProduct;
	me->ProductIsValidProduct	= product_ProductIsValidProduct;

	me->pdbs = pDBS;
	me->sort = TRUE;

Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbsproductlist_delete"
SElExceptionPtr dbsproductlist_delete(SDBSProductListPtr* pDBSProductListPtr)
{
	int	i;
	
	if (pDBSProductListPtr && *pDBSProductListPtr)
	{
		SDBSProductListPtr	me = *pDBSProductListPtr;

		for(i=0; i<me->_Allocated; i++)
		{
			if(me->Product[i].description)
			{
				free(me->Product[i].description);
				me->Product[i].description = NULL;
			}
		}
		
		if(me->Product)
		{
			free(me->Product);
			me->Product = NULL;
		}

		free(*pDBSProductListPtr);
		*pDBSProductListPtr = NULL;
	}

/* Error: */
	return NULL;
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "product_ProductRead"
static SElExceptionPtr product_ProductRead(struct _SDBSProductList* me, int32_t branch_id)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL; 
	int32_t			count, index;
	char			sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr		podbc = PDBS->podbc;
	size_t			desc_len;

	LOCK;

	/* lock database tester.product */
	PDBS_LOCK(LOCK_STR);
	
	/* select items */														
	sprintf(sql, "SELECT p.product_id,p.parent_id,pt.name,pt.description,pt.time,pt.user_id,pt.property,pt.parameter_id " 
		 		 "FROM tester.product p INNER JOIN tester.product_types pt "
				 "ON p.product_id=pt.product_id "
				 "WHERE p.parent_id=0 OR p.branch_id=%d "
				 "ORDER BY p.product_id",
				 branch_id);
	
	EXCCHECK( podbc->ExecSQL( podbc, sql));
	EXCCHECK( podbc->GetRows(podbc, &count));

	/* memory allocation */
	EXCCHECK( product_Allocate(me, count));

	CHECKERR( podbc->Fetch(podbc, TRUE));

	for(index=0;index<count;index++)
	{
		SProductTypesPtr pproduct = me->Product;

		pproduct[index].product_id = *((long*)podbc->GetFieldValuePtr(podbc, 1)); 
		pproduct[index].parent_id = *((long*)podbc->GetFieldValuePtr(podbc, 2));
		strncpy(pproduct[index].name, (char*)podbc->GetFieldValuePtr(podbc, 3), DBS_RECORD_LENGHT_NAME);

		if(pproduct[index].description!=NULL)
		{
			free(pproduct[index].description);
			pproduct[index].description = NULL;
		}

		desc_len = strlen((char*)podbc->GetFieldValuePtr(podbc, 4));
		if(desc_len)
		{
			pproduct[index].description = calloc(desc_len+1, sizeof(char));
			strncpy(pproduct[index].description, (char*)podbc->GetFieldValuePtr(podbc, 4), desc_len);
		}
		//strncpy(pproduct[index].time, (char*)podbc->GetFieldValuePtr(podbc, 5), DBS_RECORD_LENGHT_TIME);  //TEST
		sprintf(pproduct[index].time, (char*)podbc->GetFieldValuePtr(podbc, 5));
		pproduct[index].user_id = *((long*)podbc->GetFieldValuePtr(podbc, 6));
		pproduct[index].property = *((short*)podbc->GetFieldValuePtr(podbc, 7));
		pproduct[index].parameter_id = *((long*)podbc->GetFieldValuePtr(podbc, 8));
 
		CHECKERR( podbc->MoveNext(podbc));
	}
	
	me->ProductSize = index;
	
	EXCCHECK( podbc->Cancel(podbc));

Error:
	PDBS_UNLOCK(LOCK_STR);
	UNLOCK;
	EXCRETHROW( pexception);     
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "product_ProductSave"
static SElExceptionPtr product_ProductSave(struct _SDBSProductList* me, int32_t branch_id)
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

	if(me->ProductSize)
	{	
		uint32_t last_param_id;

		/* get last test_id */
		EXCCHECK( podbc->ExecSQL( podbc, "SELECT MAX(pt.parameter_id) FROM tester.product_types pt"));
		CHECKERR( podbc->Fetch( podbc, TRUE));
		if(error==ELODBC_SQL_WARN_NO_DATA_FOUND)
			last_param_id = 0;
		else
			last_param_id = *((unsigned long*)podbc->GetFieldValuePtr(podbc, 1));

		EXCCHECK( podbc->Cancel(podbc));

		EXCCHECK( elstring_new(NULL, &pelstring));
		for(i=0; i<me->ProductSize;)
		{	
			EXCCHECK(pelstring->Empty(pelstring));

			if(me->Product[i].parameter_id==0)
				me->Product[i].parameter_id = ++last_param_id;

			sprintf(sql, "REPLACE INTO tester.product_types "
						 "(product_id,name,description,time,user_id,property,parameter_id) "
						 "VALUES (%d,'%s',_UTF8'%s','%s',%d,%d,%d)", 
						 me->Product[i].product_id,
						 me->Product[i].name, 
						 GET_STRING(me->Product[i].description),
						 me->Product[i].time,
						 me->Product[i].user_id,
						 me->Product[i].property,
						 me->Product[i].parameter_id);
	
			EXCCHECK(pelstring->Append(pelstring, sql) );
		
			i++;
			j = 0;
			
			for(; i<me->ProductSize; i++)
			{
				if(++j>SQL_COMMAND_NB_OF_INSERTED_ROW)
					break;
				
				if(me->Product[i].parameter_id==0)
					me->Product[i].parameter_id = ++last_param_id;
							
				sprintf(sql, ",(%d,'%s',_UTF8'%s','%s',%d,%d,%d)", 
							 me->Product[i].product_id,
							 me->Product[i].name, 
							 GET_STRING(me->Product[i].description),
							 me->Product[i].time,
							 me->Product[i].user_id,
							 me->Product[i].property,
							 me->Product[i].parameter_id);
		
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

/* sort algorithm for product structure array 
 * priority: 	1) product_id
 *				2) PROPERTY_VALID
 */
int compare_Product (const void* a, const void* b) 
{
	SProductTypesPtr aa, bb;
	
	aa = (SProductTypesPtr)a;
	bb = (SProductTypesPtr)b;
	
	if(aa->product_id != bb->product_id)
	{
		return (aa->product_id - bb->product_id);
	}
	else
	{
		return ((bb->property&PROPERTY_VALID) - (aa->property&PROPERTY_VALID));
	}
}		

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "product_Insert"
static SElExceptionPtr product_Insert(struct _SDBSProductList* me, SProductTypes product)
{ 
	SElExceptionPtr	pexception = NULL; 
	
	/* check free space */
	EXCCHECK( product_CheckAlloc(me));
	
	if(me->Product[me->ProductSize].description)
		free(me->Product[me->ProductSize].description);

	/* complete record */
	DATETIME_TO_STRING(product.time);
	product.parameter_id = 0;

	me->Product[me->ProductSize++] = product;
	
	if(me->sort)
		qsort(me->Product, me->ProductSize, sizeof(SProductTypes), compare_Product); 
	
	PDBS->data_changed	= TRUE;  
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "product_Sort"
static SElExceptionPtr product_Sort(struct _SDBSProductList* me)
{
	qsort(me->Product, me->ProductSize, sizeof(SProductTypes), compare_Product); 
	
	return NULL; 
}

/*---------------------------------------------------------------------------*/     
/* PRODUCT SECTION ***********************************************************/
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "product_ProductNew"
static SElExceptionPtr product_ProductNew(
	struct _SDBSProductList* me, 
	SProductTypesPtr pproduct
)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL; 
	char			sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr		podbc = PDBS->podbc;
	int32_t			last_product_id = 0;
	SProductTypes	product = *pproduct;

	if(0==pproduct->user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");

	LOCK;

	/* lock database tester.product */
	PDBS_LOCK(LOCK_STR2);

	sprintf(sql, "SELECT MAX(product_id) FROM tester.product");
	EXCCHECK( podbc->ExecSQL( podbc, sql));
	CHECKERR( podbc->Fetch( podbc, TRUE));
	last_product_id	= *((short*)podbc->GetFieldValuePtr(podbc, 1));
	EXCCHECK( podbc->Cancel(podbc));
	
	product.product_id = ++last_product_id;
	pproduct->product_id = product.product_id;
	
	sprintf(sql, "INSERT INTO tester.product "
				 "(product_id,parent_id,branch_id) "
				 "VALUES (%d,%d,%d)", 
				 pproduct->product_id,
				 pproduct->parent_id,
				 (pproduct->parent_id==0)? pproduct->product_id : PDBS->_branch_id);
	
	EXCCHECK( podbc->ExecSQL( podbc, sql)); 
	EXCCHECK( podbc->Cancel(podbc));
	
	/* set new record */
	product.property |= PROPERTY_CREATED|PROPERTY_VALID;
	
	EXCCHECK( product_Insert(me, product));  

Error:
	PDBS_UNLOCK(LOCK_STR2);
	UNLOCK;
	EXCRETHROW( pexception);     
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "product_ProductEdit"
static SElExceptionPtr product_ProductEdit(
	struct _SDBSProductList* me, 
	SProductTypes product
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			i;
	
	if(0==product.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");

	/* set actual not valid */
	for(i=0; i<me->ProductSize; i++)
	{
		if( (me->Product[i].property&PROPERTY_VALID)>0
			&& me->Product[i].product_id == product.product_id)
		{
			me->Product[i].property ^= PROPERTY_VALID;	
			break;
		}
	}
	
  	/* set new record */     
	product.property |= PROPERTY_EDITED|PROPERTY_VALID;

	EXCCHECK( product_Insert(me, product)); 	

Error:
	EXCRETHROW( pexception);     
}
#if !defined(_NO_TEST) || !defined(_NO_MASTER)
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "product_ProductDel"
static SElExceptionPtr product_ProductDel(
	struct _SDBSProductList* me, 
	SProductTypes product
)
{
	SElExceptionPtr	pexception = NULL; 
	int				i, j;
	int				pid[1024];
	int				pidSize = 1024;
	bool_t			data_changed_line = FALSE;
	
	if(0==product.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");
	
	for(i=0; i<me->ProductSize; i++)
	{
		if( (me->Product[i].property&PROPERTY_VALID)>0
			&& me->Product[i].product_id == product.product_id)
		{
			me->Product[i].property ^= PROPERTY_VALID;
			product.description = PDBS_COPY(product.description);
		}
	}

	/* set new record */     
	product.property = PROPERTY_DELETED; 
	
	EXCCHECK( product_Insert(me, product)); 

	/* search downtree products id */
	EXCCHECK( me->ProductGetIdTreeDown(me, product.product_id, pid, &pidSize));

	for(j=0; j<pidSize; j++)
	{
		for(i=0; i<me->ProductSize; i++)
		{
			if( (me->Product[i].property&PROPERTY_VALID)>0
				&& me->Product[i].product_id == pid[j])
			{
				me->Product[i].property ^= PROPERTY_VALID;
			}
		}

		/* set current product parameters not valid without log*/ 
		for(i=0; i<PPARAM->ProductParameterSize; i++)
		{
			if( (PPARAM->ProductParameter[i].property&PROPERTY_VALID)>0
				&& PPARAM->ProductParameter[i].product_id == pid[j] )
			{
				PPARAM->ProductParameter[i].property ^= PROPERTY_VALID;
			}	
		}
		/* set current product masters not valid without log*/ 
		for(i=0; i<PMASTER->ProductMasterSize; i++)
		{
			if( (PMASTER->ProductMaster[i].property&PROPERTY_VALID)>0
				&& PMASTER->ProductMaster[i].product_id == pid[j] )
			{
				PMASTER->ProductMaster[i].property ^= PROPERTY_VALID;
			}	
		}
		/* set current tests not valid without log*/ 
		for(i=0; i<PTEST->TestSize; i++)
		{
			if( (PTEST->Test[i].property&PROPERTY_VALID)>0
				&& PTEST->Test[i].product_id == pid[j] )
			{
				PTEST->Test[i].property ^= PROPERTY_VALID;
			}	
		}
		/* set current test parameters not valid without log*/ 
		for(i=0; i<PTESTPARAM->TestParameterSize; i++)
		{
			if( (PTESTPARAM->TestParameter[i].property&PROPERTY_VALID)>0
				&& PTESTPARAM->TestParameter[i].product_id == pid[j] )
			{
				PTESTPARAM->TestParameter[i].property ^= PROPERTY_VALID;
			}	
		}
		/* set current line process not valid without log*/ 
		for(i=0; i<PPROCESS->LineProcessSize; i++)
		{
			if( (PPROCESS->LineProcess[i].property&PROPERTY_VALID)>0
				&& PPROCESS->LineProcess[i].product_id == pid[j] )
			{
				PPROCESS->LineProcess[i].property ^= PROPERTY_VALID;
				data_changed_line = TRUE;
			}	
		}
		/* set current line step not valid without log*/ 
		for(i=0; i<PSTEP->LineStepSize; i++)
		{
			if( (PSTEP->LineStep[i].property&PROPERTY_VALID)>0
				&& PSTEP->LineStep[i].product_id == pid[j] )
			{
				PSTEP->LineStep[i].property ^= PROPERTY_VALID;
				data_changed_line = TRUE;
			}	
		}
		/* set current line step not valid without log*/ 
		for(i=0; i<PLINEPARAM->LineParamSize; i++)
		{
			if( (PLINEPARAM->LineParam[i].property&PROPERTY_VALID)>0
				&& PLINEPARAM->LineParam[i].product_id == pid[j] )
			{
				PLINEPARAM->LineParam[i].property ^= PROPERTY_VALID;
				data_changed_line = TRUE;
			}	
		}
	}

	if(data_changed_line)
		PDBS->data_changed_line = TRUE;  
	
Error:
	EXCRETHROW( pexception);     
}

/*---------------------------------------------------------------------------*/
int compare_pid ( const void* a, const void* b ) 
{
	int32_t		aa = *(int32_t*)a;
	int32_t		bb = *(int32_t*)b;
	
	return (aa - bb);
}		

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "product_ProductCopy"
static SElExceptionPtr product_ProductCopy(
	struct _SDBSProductList* me, 
	int32_t pidSrc[], 
	int32_t pidTrg[], 
	int32_t pidSize,
	int32_t user_id
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	char			sql[SQL_COMMAND_LENGHT] = "";  
	SOdbcPtr		podbc = PDBS->podbc;
	int32_t			last_product_id;
	SELStringPtr   	pelstring = NULL;
	int32_t			i, j, index;
	SProductTypesPtr	pprodTrg = NULL;

	LOCK;

	/* lock database tester.product */
	PDBS_LOCK(LOCK_STR2);

	pprodTrg = calloc(pidSize, sizeof(SProductTypes)); 
	EXCCHECKALLOC(pprodTrg);

	/* get last product_id from DB*/
	sprintf(sql, "SELECT MAX(product_id) FROM tester.product");
	EXCCHECK( podbc->ExecSQL( podbc, sql));
	CHECKERR( podbc->Fetch( podbc, TRUE));
	last_product_id	= *((short*)podbc->GetFieldValuePtr(podbc, 1));
	EXCCHECK( podbc->Cancel(podbc));

	for(i=0, index=1; i<me->ProductSize; i++)
	{
		for(j=0; j<pidSize; j++)
		{
			if( (me->Product[i].property&PROPERTY_VALID)>0
				&& me->Product[i].product_id!=pidSrc[0]
				&& me->Product[i].product_id==pidSrc[j]) 
			{
				if(pidSize==index)
					EXCTHROW(-1, "It shouldn´t happen!");

				pprodTrg[index] = me->Product[i];
				pprodTrg[index].product_id = ++last_product_id; 
				pidTrg[index] = pprodTrg[index].product_id;
				index++;
				break;
			}
		}
	}

	/* sort src */
	qsort(pidSrc, pidSize, sizeof(int32_t), compare_pid); 

	/* determine new parent_id */
	for(i=0; i<pidSize; i++)
	{
		for(j=1; j<pidSize; j++)
		{
			if(pprodTrg[j].parent_id==pidSrc[i])
				pprodTrg[j].parent_id = pidTrg[i];	
		}
	}
		
	EXCCHECK( elstring_new(NULL, &pelstring)); 

	for(i=1; i<pidSize; i++)
	{
		if(i==1)
		{
			sprintf(sql, "INSERT INTO tester.product "
						 "(product_id,parent_id,branch_id) "
						 "VALUES (%d,%d,%d)", 
						pprodTrg[i].product_id,
						pprodTrg[i].parent_id,
						PDBS->_branch_id);
		}
		else
		{
			sprintf(sql, ",(%d,%d,%d)", 
						pprodTrg[i].product_id,
						pprodTrg[i].parent_id,
						PDBS->_branch_id);
		}
		EXCCHECK(pelstring->Append(pelstring, sql));
	}
	EXCCHECK( podbc->ExecSQL( podbc, pelstring->GetPtr(pelstring))); 
	EXCCHECK( podbc->Cancel(podbc));

	pelstring->Empty(pelstring);  
	
	for(i=1; i<pidSize; i++)
	{
		/* set new record */
		pprodTrg[i].user_id = user_id; 
		
		if((pprodTrg[i].property&PROPERTY_VISIBLE)>0)
			pprodTrg[i].property = PROPERTY_VISIBLE;

		pprodTrg[i].property |= PROPERTY_CREATED|PROPERTY_VALID;
		pprodTrg[i].description = PDBS_COPY( pprodTrg[i].description);

		EXCCHECK( product_Insert(me, pprodTrg[i]));
	}

Error:
	elstring_delete(&pelstring);
	free(pprodTrg);
	PDBS_UNLOCK(LOCK_STR2);
	UNLOCK;
	EXCRETHROW( pexception);     
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "product_ProductCounterCopy"
static SElExceptionPtr product_ProductCounterCopy(
	struct _SDBSProductList* me, 
	int32_t pidSrc[], 
	int32_t pidTrg[], 
	int32_t pidSize,
	int32_t user_id
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	char			sql[SQL_COMMAND_LENGHT] = "";  
	SOdbcPtr		podbc = PDBS->podbc;
	int32_t			i, count;

	LOCK;

	/* lock database tester.product_counter */
	PDBS_LOCK(LOCK_STR3);

	for(i=0; i<pidSize; i++)
	{
		sprintf(sql, "SELECT pc.product_id FROM tester.product_counter pc "
					 "WHERE pc.product_id=%d ",
					 pidSrc[i]);
		EXCCHECK( podbc->ExecSQL( podbc, sql));
		CHECKERR( podbc->Fetch( podbc, TRUE));
		EXCCHECK( podbc->GetRows(podbc, &count));
		EXCCHECK( podbc->Cancel(podbc));

		if(count>0) /* counter exist */
		{
			sprintf(sql, "REPLACE INTO tester.product_counter "
						 "(product_id,count,time) "
						 "VALUES (%d, 0, NULL) ",
						 pidTrg[i]);
			EXCCHECK( podbc->ExecSQL( podbc, sql));
			EXCCHECK( podbc->Cancel(podbc));
		}
	}

Error:
	PDBS_UNLOCK(LOCK_STR3);
	UNLOCK;
	EXCRETHROW( pexception);     
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "product_ProductCopyStruct"
static SElExceptionPtr product_ProductCopyStruct(
	struct _SDBSProductList* me, 
	SProductTypes source, 
	SProductTypes target, 
	int32_t mode
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			pidSrc[1024];
	int32_t			pidTrg[1024];
	int32_t			pidSize = 1024;
	int32_t			pidSize2 = 1024;
	bool_t			create_product_tree = TRUE;
	int32_t			i;
	char			*nameSrc = NULL, *nameTrg = NULL;

	if(0==target.user_id)
		EXCTHROW(DBS_ERROR_NOT_VALID_USER, "Not valid user!");

	/* search downtree products id */
	EXCCHECK( me->ProductGetIdTreeDown(me, source.product_id, pidSrc, &pidSize));
	pidTrg[0] = target.product_id;

	/* search downtree target products id */
	EXCCHECK( me->ProductGetIdTreeDown(me, pidTrg[0], pidTrg, &pidSize2));
	
	/* check target product tree */
	if(pidSize2>1 && pidSize2==pidSize)
	{
		for(i=1; i<pidSize; i++)
		{
			EXCCHECK( me->ProductGetIdName(me, pidSrc[i], &nameSrc));
			EXCCHECK( me->ProductGetIdName(me, pidTrg[i], &nameTrg));
			if(0!=strcmp(nameSrc, nameTrg))
				break;
		}
		if(i==pidSize)
			create_product_tree = FALSE;
		else
			EXCTHROW(-1, "Incompatibile target product tree!");
	}

	if((mode&PRODUCT_CP_PRODUCT)>0)
	{
		/* create new product ids, return target ids */
		if(create_product_tree)
		{
			EXCCHECK( product_ProductCopy(me, pidSrc, pidTrg, pidSize, target.user_id)); 
			EXCCHECK( product_ProductCounterCopy(me, pidSrc, pidTrg, pidSize, target.user_id));
		}
	
		/* create new product parameters */
		if((mode&PRODUCT_CP_PRODUCT_PARAMETER)>0)    
			EXCCHECK( PPARAM->ProductParamCopy(PPARAM, pidSrc, pidTrg, pidSize));

		/* create new product master */
		if((mode&PRODUCT_CP_PRODUCT_MASTER)>0)    
			EXCCHECK( PMASTER->ProductMasterCopy(PMASTER, pidSrc, pidTrg, pidSize));

		/* create new test steps */		
		if((mode&PRODUCT_CP_TEST)>0)
		{
			EXCCHECK( PTEST->TestCopy(PTEST, pidSrc, pidTrg, pidSize));

			/* create new test parameters */		
			if((mode&PRODUCT_CP_TEST_PARAMETER)>0)    
				EXCCHECK( PTESTPARAM->TestParamCopy(PTESTPARAM, pidSrc, pidTrg, pidSize));
		}

		if((mode&PRODUCT_CP_LINE_PROCESS)>0)
		{
			int32_t processCpy[128];
			int32_t	processSize = 0;

			EXCCHECK( PPROCESS->LineProcessCopy(PPROCESS, pidSrc, pidTrg, pidSize, processCpy, &processSize));

			if((mode&PRODUCT_CP_LINE_STEP)>0)
			{
				EXCCHECK( PSTEP->LineStepCopy(PSTEP, pidSrc, pidTrg, pidSize, processCpy, processSize));

				if((mode&PRODUCT_CP_LINE_PARAMETER)>0)
					EXCCHECK( PLINEPARAM->LineParamCopy(PLINEPARAM, pidSrc, pidTrg, pidSize));
			}
		}
	}
		
Error:
	EXCRETHROW( pexception);   
}
#endif
/*---------------------------------------------------------------------------*/
/* PRODUCT ID SEARCH SECTION *************************************************/   
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "product_GetProductIdTreeUp"
static SElExceptionPtr product_GetProductIdTreeUp(
	struct _SDBSProductList* me, 
	int32_t start_pid, 
	int32_t* pid, 
	int32_t* pidSize
)  
{
	SElExceptionPtr pexception = NULL;
	int				i, index = 0;
	int32_t			product_id;
	SProductTypesPtr	pproduct = me->Product;
	int32_t			allocated_size = *pidSize;
	
	memset(pid, 0, sizeof(pid[0])*allocated_size);
	
	product_id = start_pid;

	if(product_id!=0)
	{
		/* is product id valid? */  
		for(i=0; i<me->ProductSize; i++)
		{
			if( (pproduct[i].property&PROPERTY_VALID)>0 
				&& pproduct[i].product_id == product_id)
			{
			 	break;
			}
		}
		if(i == me->ProductSize)
			EXCTHROW(DBS_ERROR_NOT_VALID_PRODUCT_ID, "Not valid product ID!");
	
		/* determine product id path */
		do
		{
			for(i=0; i<me->ProductSize; i++)
			{
				if( (pproduct[i].property&PROPERTY_VALID)>0 
					&& pproduct[i].product_id == product_id)
				{
					/* check allocated size */
					if(allocated_size == index)
						EXCTHROW(-1, "Not enough allocated space!");
				
					product_id = pproduct[i].parent_id;
					pid[index++] = pproduct[i].product_id;
					break;
				}
			}
		} while(product_id>0);
	
		*pidSize = index;
	}
	else
	{
		*pidSize = 0;
	}

Error:
	EXCRETHROW( pexception);   
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "product_GetProductIdTreeDown"
static SElExceptionPtr product_GetProductIdTreeDown(
	struct _SDBSProductList* me, 
	int32_t start_pid, 
	int32_t* pid, 
	int32_t* pidSize
)  
{
	SElExceptionPtr pexception = NULL;   
	int				i, index, pidIndex = 0;
	int32_t			product_id;
	int32_t			allocated_size = *pidSize;
	
	memset(pid, 0, sizeof(pid[0])*allocated_size);
	
	pid[pidIndex++] = start_pid;
	
	product_id = start_pid;
	index = 0;
	/* determine product id path */
	do
	{
		for(i=0; i<me->ProductSize; i++)
		{
			if( (me->Product[i].property&PROPERTY_VALID)>0 
				&& me->Product[i].parent_id == pid[index])
			{
				/* check allocated size */
				if(allocated_size == pidIndex)
					EXCTHROW(-1, "Not enough allocated space!");
			
				pid[pidIndex++] = me->Product[i].product_id;	
			}
		}
		index++;
		
	} while(pid[index]!=0);
	
	*pidSize = pidIndex;

Error:
	EXCRETHROW( pexception);   
}

#define TREE_STRUCTURE_SIZE 1024
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "product_GetProductIdTreeUpDown"
static SElExceptionPtr product_GetProductIdTreeUpDown(
	struct _SDBSProductList* me, 
	int32_t start_pid, 
	int32_t* pid, 
	int32_t* pidSize
)  
{
	SElExceptionPtr pexception = NULL;   
	int32_t			allocated_size = *pidSize;
	int32_t			data[TREE_STRUCTURE_SIZE];
	int32_t			dataSize = TREE_STRUCTURE_SIZE;
	int				i, j;
	int32_t*		ppid = pid;
	
	memset(pid, 0, sizeof(pid[0])*allocated_size);  
	
	EXCCHECK( me->ProductGetIdTreeUp(me, start_pid, data, &dataSize) );  
	
	for(i=0; i<dataSize-1; i++)
	{
		/* check allocated size */
		if(allocated_size <= i)
			EXCTHROW(-1, "Not enough allocated space!");
		
		*ppid++ = data[dataSize-1-i];
	}
	
	dataSize = TREE_STRUCTURE_SIZE;
	
	EXCCHECK( me->ProductGetIdTreeDown(me, start_pid, data, &dataSize) );
	
	for(j=0; j<dataSize; j++)
	{
		/* check allocated size */
		if(allocated_size <= i+j)
			EXCTHROW(-1, "Not enough allocated space!");
	
	   	*ppid++ = data[j];
	}

	*pidSize = i+j;
	
Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "product_GetProductIdName"
static SElExceptionPtr product_GetProductIdName(
	struct _SDBSProductList* me, 
	int32_t pid, 
	char** name
)
{
	SElExceptionPtr pexception = NULL;   
	int				i;
	
	for(i=0; i<me->ProductSize; i++)
	{
		if( (me->Product[i].property&PROPERTY_VALID)>0 
			&& me->Product[i].product_id == pid)
		{
			*name = me->Product[i].name; 
			break;
		}
	}
	if(i==me->ProductSize)  
	{
		for(i=0; i<me->ProductSize; i++)
		{
			if( (me->Product[i].property&PROPERTY_VALID)==0
				&& (me->Product[i].property&PROPERTY_CREATED)>0 
				&& me->Product[i].product_id == pid)
			{
				*name = me->Product[i].name; 
				break;
			}
		}
	}
	if(i==me->ProductSize)
	{
		for(i=0; i<me->ProductSize; i++)
		{
			if( (me->Product[i].property&PROPERTY_VALID)==0
				&& me->Product[i].product_id == pid)
			{
				*name = me->Product[i].name; 
				break;
			}
		}
	}
	if(i==me->ProductSize)
		*name = "root";
	
/* Error: */
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "product_GetProductIdDescription"
static SElExceptionPtr product_GetProductIdDescription(
	struct _SDBSProductList* me, 
	int32_t pid, 
	char** description
)
{
	SElExceptionPtr pexception = NULL;   
	int				i;
	
	for(i=0; i<me->ProductSize; i++)
	{
		if( (me->Product[i].property&PROPERTY_VALID)>0 
			&& me->Product[i].product_id == pid)
		{
			*description = me->Product[i].description; 
			break;
		}
	}
	if(i==me->ProductSize)  
	{
		for(i=0; i<me->ProductSize; i++)
		{
			if( (me->Product[i].property&PROPERTY_VALID)==0
				&& (me->Product[i].property&PROPERTY_CREATED)>0 
				&& me->Product[i].product_id == pid)
			{
				*description = me->Product[i].description; 
				break;
			}
		}
	}
	if(i==me->ProductSize)
	{
		for(i=0; i<me->ProductSize; i++)
		{
			if( (me->Product[i].property&PROPERTY_VALID)==0
				&& me->Product[i].product_id == pid)
			{
				*description = me->Product[i].description; 
				break;
			}
		}
	}
	if(i==me->ProductSize)
		*description = NULL;
	
/* Error: */
	EXCRETHROW( pexception);
}

#define NUMBER_OF_GROUPS 	5
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "product_GetProductIdPath"
static SElExceptionPtr product_GetProductIdPath(
	struct _SDBSProductList* me, 
	int32_t product_id,
	char* 	path,
	int32_t level
)
{
	SElExceptionPtr pexception = NULL;   
	int				i;
	char*			name[NUMBER_OF_GROUPS];
	int32_t			nameSize = 0;
	char*			ppath = path;

	/* path string 'PL3\SOP1\IHKS\IHKS09\1.0' */
	/* tree level   1  \2   \3   \4     \5    */
	
	/* determine product id path */ 
	do
	{
		for(i=0; i<me->ProductSize; i++)
		{
			if( (me->Product[i].property&PROPERTY_VALID)>0
				&& me->Product[i].product_id == product_id )
			{
				product_id = me->Product[i].parent_id;
				name[nameSize++] = me->Product[i].name;
				break;
			}
		}
		if(i==me->ProductSize)
		{
			for(i=0; i<me->ProductSize; i++)
			{
				if( (me->Product[i].property&PROPERTY_VALID)==0
					&& (me->Product[i].property&PROPERTY_DELETED)>0  
					&& me->Product[i].product_id == product_id)
				{
					product_id = me->Product[i].parent_id;
					name[nameSize++] = me->Product[i].name;
					break;
				}
			}
		}
		if(i==me->ProductSize)
		{
			for(i=0; i<me->ProductSize; i++)
			{
				if( (me->Product[i].property&PROPERTY_VALID)==0
					&& me->Product[i].product_id == product_id)
				{
					product_id = me->Product[i].parent_id;
					name[nameSize++] = me->Product[i].name;
					break;
				}
			}
		}
		if(i==me->ProductSize)
		{
			*ppath = '\0';
			EXCTHROW(-1, "Any product_id found!\nPossibly from other product branch.");
		}

	} while(product_id>0); 
	
	for(i=nameSize-level; i>=0; i--)  
	{
		ppath += sprintf(ppath, "%s\\", name[i]);
		if(i==0)
			sprintf(--ppath, "\0");
	}
	if(nameSize<level 
		&& nameSize!=0)
	{
		ppath += sprintf(ppath, "%s", name[0]);
	}
	if(nameSize==0)
	{
		sprintf(ppath, "none"); 
	}
	
Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "product_GetProductIdFromPath"
static SElExceptionPtr product_GetProductIdFromPath(
	struct _SDBSProductList* me, 
		char* path, 
		int32_t* pid
)
{
	SElExceptionPtr pexception = NULL;   
	char*			name[NUMBER_OF_GROUPS+1];
	int32_t			nameSize = 0;
	int32_t			product_id = 0;
	int				i, j;
	bool_t			found = FALSE;
	
	/* path string 'PL3\SOP1\IHKS\IHKS09\1.0' */
	name[nameSize++] = strtok(path, "\\"); 
	while(name[nameSize-1] != NULL){
		name[nameSize++] = strtok(NULL, "\\"); 		
	} 
	
	for(i=0; i<nameSize-1; i++)
	{
		for(j=0; j<me->ProductSize; j++)
		{
			if( (me->Product[j].property&PROPERTY_VALID)>0 
				&& 0==strcmp(me->Product[j].name, name[i]) 
				&& me->Product[j].parent_id == product_id )
			{
				product_id = me->Product[j].product_id;
				break;
			}
		}
		if( me->ProductSize == j)
		{
			found = FALSE;
			break;
		}
		else
		{
			found = TRUE;
		}
	}
	if(found)
		*pid = product_id;
	if(!found)
		*pid = 0;
	
/* Error: */
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/  
#undef __FUNC__
#define __FUNC__ "product_GetProductIdFirstValid"
static SElExceptionPtr product_GetProductIdFirstValid(
	struct _SDBSProductList* me, 
	int32_t* pid
)
{
	SElExceptionPtr pexception = NULL;   
	int				i;
	bool_t			valid;
	
	if(pid) *pid = 0;
		
	for(i=0; i<me->ProductSize; i++)
	{   
		if( (me->Product[i].property&PROPERTY_VALID)>0 
			&&(me->Product[i].property&PROPERTY_VISIBLE)>0)
		{
			EXCCHECK( me->ProductIsValidEndProduct(me, me->Product[i].product_id, &valid) );
			if(valid)
			{
				if(pid) *pid =  me->Product[i].product_id;
				break;
			}
		}
	}
	
Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "product_ProductIsValidEndProduct"
static SElExceptionPtr product_ProductIsValidEndProduct(
	struct _SDBSProductList* me, 
	int32_t	product_id,
	bool_t* valid
)
{
	SElExceptionPtr pexception = NULL;   
	int				i;

	if(valid) *valid = FALSE;

	/* if there are not the valid child items, product is end product, exclude root products */
	for(i=0; i<me->ProductSize; i++)
	{   
		if( (me->Product[i].property&PROPERTY_VALID)>0 
			&&me->Product[i].product_id==product_id)
		{
			break;
		}
	}

	if(i==me->ProductSize || me->Product[i].parent_id==0)
	{
		if(valid) *valid = FALSE;
	}
	else
	{
		for(i=0; i<me->ProductSize; i++)
		{   
			if( (me->Product[i].property&PROPERTY_VALID)>0 
				&&me->Product[i].parent_id==product_id)
			{
				break;
			}
		}

		if(i==me->ProductSize)
			if(valid) *valid = TRUE;	
	}

/* Error: */
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "product_ProductIsValidProduct"
static SElExceptionPtr product_ProductIsValidProduct(
	struct _SDBSProductList* me, 
	int32_t	product_id,
	bool_t* valid
)
{
	SElExceptionPtr pexception = NULL;   
	int				i;

	if(valid) *valid = FALSE;

	/* if there are not the valid child items, product is end product */
	for(i=0; i<me->ProductSize; i++)
	{   
		if( (me->Product[i].property&PROPERTY_VALID)>0 
			&&me->Product[i].product_id==product_id)
		{
			break;
		}
	}

	if(i!=me->ProductSize)
		if(valid) *valid = TRUE;	

/* Error: */
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/  
#undef __FUNC__
#define __FUNC__ "product_Allocate"
static SElExceptionPtr product_Allocate(struct _SDBSProductList* me, int32_t size) 
{
	SElExceptionPtr	pexception = NULL;   

	if(me->_Allocated == 0)
	{
		me->Product = calloc(size+MEMORY_ALLOCATION_RESERVE, sizeof(SProductTypes)); 
		EXCCHECKALLOC(me->Product);
		me->_Allocated = size+MEMORY_ALLOCATION_RESERVE;
	}
	else if(size>me->_Allocated)   
	{
		me->Product = realloc(me->Product, (size+MEMORY_ALLOCATION_RESERVE)*sizeof(SProductTypes)); 
		EXCCHECKALLOC(me->Product);
		memset( me->Product + me->_Allocated, 
				0, 
				(size+MEMORY_ALLOCATION_RESERVE - me->_Allocated)*sizeof(SProductTypes));
		me->_Allocated = size+MEMORY_ALLOCATION_RESERVE;
	}
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "product_CheckAlloc"
static SElExceptionPtr product_CheckAlloc(struct _SDBSProductList* me)
{
	SElExceptionPtr	pexception = NULL;   
	
	if(me->_Allocated - me->ProductSize < MEMORY_ALLOCATION_MIN )
	{
		me->Product = realloc( me->Product, 
							(me->_Allocated + MEMORY_ALLOCATION_RESERVE)*sizeof(SProductTypes));
		EXCCHECKALLOC(me->Product);
		/* clear memory */
		memset( me->Product + me->_Allocated,
				0,
				MEMORY_ALLOCATION_RESERVE*sizeof(SProductTypes));
		me->_Allocated += MEMORY_ALLOCATION_RESERVE;
	}
		
Error:
	EXCRETHROW( pexception); 
}
