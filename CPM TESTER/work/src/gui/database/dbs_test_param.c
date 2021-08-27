#include <bklib/BKLib2.h> 
#include <toolbox.h>
#include "dbs_global.h"
#include "dbs_test.h"
#include "dbs_product.h"
#include "dbs_test_param.h" 
#include "dbs_test_param_edit.h"  
#include "ring_control.h"

#define BKTREE_HEADER0  "root|Parameter^w130¦desc|Description^w350\n"
#define BKTREE_HEADER1	"root|Parameter^w130¦desc|Description^w350"

#define BKTREE_MENU_ROOT _("New|<Test View>")
#define BKTREE_MENU_PARAMETER _("Edit|Delete|<Test View>")

/* menu depth */
typedef enum _EBkTreeLevel
{
	BKTREE_LEVEL_ROOT = 1,
	BKTREE_LEVEL_PARAMETER
} EBkTreeLevel;

/* level root */
typedef enum _EBkTreeMenuRoot
{
	BKTREE_MENU_ROOT_NEW_PARAMETER = 0,
	BKTREE_MENU_ROOT_STEPSVIEW
} EBkTreeMenuRoot;

/* level parameter */
typedef enum _EBkTreeMenuParameter
{
	BKTREE_MENU_PARAMETER_EDIT = 0,
	BKTREE_MENU_PARAMETER_DEL,
	BKTREE_MENU_PARAMETER_STEPSVIEW
} EBkTreeMenuParameter;

extern SGlobalPtr		global;
extern int				gs_panel[];

static char				gs_keyNode[32];

static SElExceptionPtr ParametersTreeString(void);
static SElExceptionPtr TestTreeItemLevel(char* keyNode, int32_t* level);
static SElExceptionPtr SetValueIntoTable(SELStringPtr pvalue, STestParameter parameter, int32_t* product_id, int32_t product_idSize);
static SElExceptionPtr TreeGetUpperLevelString(const char* keyNode, char* upperlevel);   

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "TreeParametersIni"
SElExceptionPtr TreeParametersIni(void)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	HRESULT			hr;
	char			config[512] = "";
	int				imglistHandle;
	
	global->editMode = EDIT_TEST_PARAM;
	memset(global->keyNode[EDIT_TEST_PARAM], 0, sizeof(global->keyNode[EDIT_TEST_PARAM]));   
	
	EXCCHECKCVI( GetObjHandleFromActiveXCtrl (gs_panel[DBS], DBS_IMAGELIST3, &imglistHandle));
	EXCCHECKCOM( BKLib2__DBKTreeSetImageList (global->treeHandle, NULL, imglistHandle));
	EXCCHECKCOM( BKLib2__DBKTreeSetAutoImages (global->treeHandle, NULL, VFALSE));

	EXCCHECK( PTESTPARAM->TestParamSort(PTESTPARAM) );  
	
	EXCCHECK( RingGroupAttribute(RING_GROUPS_LOCK) );
	EXCCHECK( TreeParametersSet( TEST_PARAM_RELOAD ) );
	EXCCHECK( TreeParametersSet( TEST_PARAM_SET_ACTIVE_NODE ) ); 
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "TreeParametersSet"
SElExceptionPtr TreeParametersSet(int32_t mode)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	HRESULT			hr;
	short int		AXvalue;
	char*			keyNode = NULL;;
	
	switch(mode)
	{
		case TEST_PARAM_RELOAD:
			EXCCHECKCOM( BKLib2__DBKTreeGetSelectedItem (global->treeHandle, NULL, &keyNode));  
			EXCCHECK( ParametersTreeString());
			EXCCHECKCOM( BKLib2__DBKTreeFromString (global->treeHandle, NULL, (global->pelstring)->GetPtr(global->pelstring), "¦", "\n", &AXvalue));
			EXCCHECKCOM( BKLib2__DBKTreeSetEditable (global->treeHandle, NULL, VTRUE));  
			EXCCHECKCOM( BKLib2__DBKTreeSetEnableDragAndDrop (global->treeHandle, NULL, VFALSE)); 
			EXCCHECKCOM( BKLib2__DBKTreeSetEnableEventModify (global->treeHandle, NULL, VTRUE));
			EXCCHECKCOM( BKLib2__DBKTreeSetSelectedItem (global->treeHandle, NULL, keyNode)); 
			EXCCHECKCOM( BKLib2__DBKTreeEnsureVisible (global->treeHandle, NULL, keyNode));
			break;
		 case TEST_PARAM_SET_ACTIVE_NODE:
			if( 0==strlen(global->keyNode[EDIT_TEST_PARAM]) )
			{
				EXCCHECKCOM( BKLib2__DBKTreeSetSelectedItem (global->treeHandle, NULL, "0")); 
				EXCCHECKCOM( BKLib2__DBKTreeEnsureVisible (global->treeHandle, NULL, "0"));
			}
			else
			{
				EXCCHECKCOM( BKLib2__DBKTreeSetSelectedItem (global->treeHandle, NULL, global->keyNode[EDIT_TEST_PARAM])); 
			}
			break;
	}
	
Error:
	if(keyNode) CA_FreeMemory( keyNode );
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/   
#undef __FUNC__
#define __FUNC__ "TreeParametersClose"
SElExceptionPtr TreeParametersClose(void)
{
	SElExceptionPtr pexception = NULL;
	
	EXCCHECK( PTESTPARAM->TestParamSort(PTESTPARAM) ); 
	EXCCHECK( RingGroupAttribute(RING_GROUPS_UNLOCK) );
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/  
#undef __FUNC__
#define __FUNC__ "BkTreeParametersNodeClick"
SElExceptionPtr BkTreeParametersNodeClick (char *keyNode)
{
	SElExceptionPtr pexception = NULL;
	HRESULT			hr;
	int32_t			level;
	short int		AXvalue;
	
	/* set active name */
	strcpy(global->keyNode[EDIT_TEST_PARAM], keyNode);
	
	EXCCHECK( TestTreeItemLevel(keyNode, &level) );
	switch(level)
	{
		case BKTREE_LEVEL_PARAMETER:
			EXCCHECKCOM( BKLib2__DBKTreeSetPopupMenu( global->treeHandle, 
													  NULL, 
													  BKTREE_MENU_PARAMETER, 
													  &AXvalue));  
			break;
		case BKTREE_LEVEL_ROOT:
			EXCCHECKCOM( BKLib2__DBKTreeSetPopupMenu( global->treeHandle, 
													  NULL, 
													  BKTREE_MENU_ROOT, 
													  &AXvalue));
			break;
	}

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "BkTreeParametersPopupMenuCommand"
SElExceptionPtr BkTreeParametersPopupMenuCommand (char *keyNode,char *keyCol,long  menuItem)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	int32_t			level;
	int				l1, l2;
	char			testnb_node[32];
	HRESULT			hr;
	STestParameter	parameter = {0};
	bool_t			free_description = FALSE;
	
	EXCCHECK( TestTreeItemLevel(keyNode, &level) );	  
	
	/* read pointer from parameter */
	if( 0 != strcmp(keyCol, "root") 
		&& 0 != strcmp(keyCol, "desc")
		&& level==BKTREE_LEVEL_PARAMETER )
	{
		long		index;   
		
		EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemData (global->treeHandle, NULL, keyNode, keyCol, &index));
		if( index )
		{
			index -= 1;
			parameter = PTESTPARAM->TestParameter[index];
		}
		else  /* value does not exist, create one */
		{
			char*	name;
			char*	description;
		
			sscanf(keyCol, "%d",&parameter.product_id);	
			
			sscanf(keyNode, "%d_%d", &l1, &l2);			
			sprintf(testnb_node, "%d", l1);
			EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemData (global->treeHandle, NULL, testnb_node, "root", &parameter.test_nb)); 
			
			EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemText (global->treeHandle, NULL, keyNode, "root", &name));
			EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemText (global->treeHandle, NULL, keyNode, "desc", &description)); 
			strncpy(parameter.name, name, DBS_RECORD_LENGHT_NAME);
			strcpy(PDBS_ALLOC(&parameter.description, DBS_RECORD_LENGHT_DESCRIPTION), description);
			free_description = (parameter.description!=NULL);
			strncpy(parameter.value, "@new", DBS_RECORD_LENGHT_VALUE_EXT); 
			
			EXCCHECK(PTESTPARAM->TestParamSetVType(PTESTPARAM, &parameter)); 
			
			CA_FreeMemory( name ); 
			CA_FreeMemory( description ); 
		}
	}
	else
	{
		char*  	nextCol;
		
		sscanf(keyNode, "%d_%d", &l1, &l2);			
		sprintf(testnb_node, "%d", l1);
		EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemData (global->treeHandle, NULL, testnb_node, "root", &parameter.test_nb)); 
		
		EXCCHECKCOM( BKLib2__DBKTreeGetColumnNext (global->treeHandle, NULL, "desc", &nextCol));
		sscanf(nextCol, "%d", &parameter.product_id); 
		
		strcpy(parameter.name, "@none");
		
		CA_FreeMemory( nextCol ); 
	}
	
	if(level==BKTREE_LEVEL_PARAMETER)
	{
		switch(menuItem)
		{																  
			case BKTREE_MENU_PARAMETER_EDIT:
				CHECK_RIGHTS_PARAMETER( EXCCHECK( EditParameterDialog2(parameter) ) );
				break;
			case BKTREE_MENU_PARAMETER_DEL:
				CHECK_RIGHTS_PARAMETER( EXCCHECK( DelParameterDialog2(parameter, keyNode, keyCol) ) ); 
				break;
			case BKTREE_MENU_PARAMETER_STEPSVIEW:
				EXCCHECK( ChangeMode(EDIT_TEST) );
				break;
		}
	}
	else if(level==BKTREE_LEVEL_ROOT)
	{	
		switch(menuItem)
		{																  
			case BKTREE_MENU_ROOT_NEW_PARAMETER:
				CHECK_RIGHTS_PARAMETER( EXCCHECK( InsertParameterDialog2(parameter) ) );
				break;
			case BKTREE_MENU_ROOT_STEPSVIEW:
				EXCCHECK( ChangeMode(EDIT_TEST) );
				break;
		}
	}
	
Error:    
	if(free_description) PDBS_FREE(parameter.description);
	EXCRETHROW( pexception);   
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "BkTreeParametersModify"
SElExceptionPtr BkTreeParametersModify(char *keyNode, char *keyCol, long modifyType)
{
	int32_t 		error = 0;
	SElExceptionPtr pexception = NULL;
	HRESULT			hr;
	int32_t			level; 
	char*			name;
	char*      		value;
	char*			description;
	int				l1, l2;
	char			testnb_node[32];
	STestParameter	parameter = {0}; 
	
	/* disable sorting algorithm */
	PTESTPARAM->sort = FALSE;

	EXCCHECK( TestTreeItemLevel(keyNode, &level) );	
	
	if(level==BKTREE_LEVEL_PARAMETER)
	{
		long 		index; 
		
		EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemText (global->treeHandle, NULL, keyNode, keyCol, &value));
		CHECK_VALUE( value);
		
		EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemData (global->treeHandle, NULL, keyNode, keyCol, &index));
		
		if( index )
		{
			index -= 1;
			parameter = PTESTPARAM->TestParameter[index]; 
			strncpy(parameter.value, value, DBS_RECORD_LENGHT_VALUE_EXT);
			parameter.description = PDBS_COPY(parameter.description); 
			parameter.user_id = global->activeUser.user_id;
			EXCCHECK( PTESTPARAM->TestParamEdit(PTESTPARAM, parameter) ); 
			EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemData ( global->treeHandle, NULL, keyNode, keyCol, 
											(long)PTESTPARAM->TestParameterSize));
		}
		else  /* value does not exist, create one */
		{
			sscanf(keyCol, "%d",&parameter.product_id);	
			
			sscanf(keyNode, "%d_%d", &l1, &l2);			
			sprintf(testnb_node, "%d", l1);
			EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemData (global->treeHandle, NULL, testnb_node, "root", &parameter.test_nb));
			
			EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemText (global->treeHandle, NULL, keyNode, "root", &name));
			EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemText (global->treeHandle, NULL, keyNode, "desc", &description)); 
			strncpy(parameter.name, name, DBS_RECORD_LENGHT_NAME);
			strcpy(PDBS_ALLOC(&parameter.description, DBS_RECORD_LENGHT_DESCRIPTION), description);
			
			EXCCHECK(PTESTPARAM->TestParamSetVType(PTESTPARAM, &parameter)); 
			
			parameter.user_id = global->activeUser.user_id;
			strncpy(parameter.value, value, DBS_RECORD_LENGHT_VALUE_EXT); 
			
			EXCCHECK( PTESTPARAM->TestParamEditInsert(PTESTPARAM, parameter) );  
			EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemData ( global->treeHandle, NULL, keyNode, keyCol, 
											(long)PTESTPARAM->TestParameterSize));
			
			CA_FreeMemory( name ); 
			CA_FreeMemory( description ); 
		}
		 
		CA_FreeMemory( value );  
	}
		
Error: 
	PTESTPARAM->sort = TRUE; 
	if(pexception!=NULL)  
	{
		TreeParametersSet(TEST_PARAM_RELOAD);
	}
	EXCRETHROW( pexception);       
}


/*---------------------------------------------------------------------------*/
int compare_pid ( const void* a, const void* b ) 
{
	return ( *((int32_t*)a) - *((int32_t*)b) );
}

#define TREE_STRUCTURE_SIZE 1024
/*---------------------------------------------------------------------------*/  
#undef __FUNC__
#define __FUNC__ "ParametersTreeString"
static SElExceptionPtr ParametersTreeString(void)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL; 
	SELStringPtr	pelstring = global->pelstring; 
	SProductTypesPtr	pproduct = PPRODUCT->Product; 
	int				i, j, z;                          
	int32_t			index = 0;
	char			path[256] = "";
	int32_t			pid[256] = {0};
	int32_t			pidSize = 0;
	char*			used_name[256] = {""};
	bool_t			exist;
	char*			test_name;
	char			row[2048];
	
	SELStringPtr   	pvalue = NULL;
	
	EXCCHECK (elstring_new(NULL, &pvalue));

/* if parameter was selected */
	if(global->test_nb != 0)
	{
		for(i=0; i<PTEST->TestSize; i++)
		{
			if( PTEST->Test[i].test_nb == global->test_nb
			    && (PTEST->Test[i].property&PROPERTY_VALID)>0)
			{	
				pid[pidSize++] = PTEST->Test[i].product_id;
				break;
			}
		}
		for(i=0; i<PPRODUCT->ProductSize; i++)
		{
			if( (pproduct[i].property&PROPERTY_VALID)>0
				&& pid[0] == pproduct[i].parent_id)
				 pid[pidSize++] = pproduct[i].product_id;
		}
	}

/* if tests were selected */
	if(global->test_nb == 0)  
	{
		for(i=0; i<PTEST->TestSize; i++)
		{
			if( PTEST->Test[i].test_nb == global->test_nb_ins[0]
			    && (PTEST->Test[i].property&PROPERTY_VALID)>0)
			{	
				pid[pidSize++] = PTEST->Test[i].product_id;
				break;
			}
		}
		for(i=0; i<PPRODUCT->ProductSize; i++)
		{
			if( (pproduct[i].property&PROPERTY_VALID)>0
				&& pid[0] == pproduct[i].parent_id)
				 pid[pidSize++] = pproduct[i].product_id;
		}
	}
	
	/* create header */
	EXCCHECK( pelstring->Empty(pelstring) );
	EXCCHECK( pelstring->Append(pelstring, BKTREE_HEADER1) ); 
	
	for(i=0; i<pidSize; i++)
	{
		EXCCHECK( PPRODUCT->ProductGetIdPath(PPRODUCT, pid[i], path, PRODUCT_PATH_LEVEL_3) );
		sprintf(row, "¦%d|%s^w150", /* value, pointer to value(unvisible) */ 
							pid[i],
							path );
		
		EXCCHECK( pelstring->Append(pelstring,row) );  
	}
	EXCCHECK( pelstring->Append(pelstring,"\n" ) );
	
/* if parameter was selected */
	if(global->test_nb != 0)
	{
		/* create test header */ 
		EXCCHECK( PTEST->TestGetName(PTEST, global->test_nb, &test_name) ); 
		EXCCHECK( pelstring->Format(pelstring, "|0|%s^i0^r1^d%d¦^r1", test_name, global->test_nb) ); 
		for(i=0; i<pidSize; i++)
		{
			EXCCHECK( pelstring->Append(pelstring, "¦^r1") );
		}
		EXCCHECK( pelstring->Append(pelstring,"\n" ) );    
	
		for(i=0; i<PTESTPARAM->TestParameterSize; i++)
		{
			if( PTESTPARAM->TestParameter[i].test_nb == global->test_nb
			    && (PTESTPARAM->TestParameter[i].property&PROPERTY_VALID)>0 )
			{
				/* do not repeat the same parameter name */
				exist = FALSE;
				for(j=0; j<index; j++)
				{
					if( 0==strcmp(used_name[j], PTESTPARAM->TestParameter[i].name) )
						exist = TRUE;	
				}
				
				if( !exist ) 
				{
					EXCCHECK( SetValueIntoTable(pvalue, 
												PTESTPARAM->TestParameter[i],
												pid, 
												pidSize) ); 
			
					EXCCHECK( pelstring->Format(pelstring, "0|0_%d|%s^i1^r1¦%s^r1¦%s\n", 
											++index,
											PTESTPARAM->TestParameter[i].name,
											GET_STRING(PTESTPARAM->TestParameter[i].description),
											pvalue->GetPtr(pvalue)
											) );
			
					used_name[index-1] = PTESTPARAM->TestParameter[i].name;
				}
			}
		}
	}
	
/* if tests were selected */
	if(global->test_nb == 0)  
	{
		for(z=0; z<global->test_nb_insSize; z++)
		{
			index = 0;	
			/* create test header */
			EXCCHECK( PTEST->TestGetName(PTEST, global->test_nb_ins[z], &test_name) ); 
			EXCCHECK( pelstring->Format(pelstring, "|%d|%s^i0^r1^d%d¦^r1", z, test_name, global->test_nb_ins[z]) ); 
			for(i=0; i<pidSize; i++)
			{
				EXCCHECK( pelstring->Append(pelstring, "¦^r1") );
			}
			EXCCHECK( pelstring->Append(pelstring,"\n" ) );    
	
			for(i=0; i<PTESTPARAM->TestParameterSize; i++)
			{
				if( PTESTPARAM->TestParameter[i].test_nb == global->test_nb_ins[z]
				    && (PTESTPARAM->TestParameter[i].property&PROPERTY_VALID)>0 )
				{
					/* do not repeat the same parameter name */
					exist = FALSE;
					for(j=0; j<index; j++)
					{
						if( 0==strcmp(used_name[j], PTESTPARAM->TestParameter[i].name) )
							exist = TRUE;	
					}
				
					if( !exist ) 
					{
						EXCCHECK( SetValueIntoTable(pvalue, 
													PTESTPARAM->TestParameter[i],
													pid, 
													pidSize) ); 
			
						EXCCHECK( pelstring->Format(pelstring, "%d|%d_%d|%s^i1^r1¦%s^r1¦%s\n", 
												z,
												z,
												++index,
												PTESTPARAM->TestParameter[i].name,
												GET_STRING(PTESTPARAM->TestParameter[i].description),
												pvalue->GetPtr(pvalue)
												) );
			
						used_name[index-1] = PTESTPARAM->TestParameter[i].name;
					}
				}
			}
		}
	}
	
Error:
	elstring_delete(&pvalue);    
	EXCRETHROW( pexception);       
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "TestTreeItemLevel"
static SElExceptionPtr	TestTreeItemLevel(char* keyNode, int32_t* level)
{
	SElExceptionPtr pexception = NULL;    
	int32_t 		error = 0;
	char*			result = NULL;
	char			keynode[32] = "";
	
	*level = 0;
	
	strcpy(keynode, keyNode);
	result = strtok(keynode, "_");			
	while( result != NULL)
	{
		*level = *level + 1;
		result = strtok(NULL, "_");
	}
	
/* Error: */
	EXCRETHROW( pexception);       
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SetValueIntoTable"
static SElExceptionPtr SetValueIntoTable(
	SELStringPtr   	pvalue, 
	STestParameter parameter,
	int32_t* product_id, 
	int32_t product_idSize
)
{
	int32_t		error = 0;
	SElExceptionPtr pexception = NULL; 
	int 		i, j;
	char		svalue[256];
	
	EXCCHECK( pvalue->Empty(pvalue) );

	for(i=0; i<product_idSize; i++)
	{
		for(j=0; j<PTESTPARAM->TestParameterSize; j++)
		{
			if( (PTESTPARAM->TestParameter[j].property&PROPERTY_VALID)>0
				&& PTESTPARAM->TestParameter[j].test_nb == parameter.test_nb 
				&& PTESTPARAM->TestParameter[j].product_id == product_id[i]
				&& 0==strcmp(PTESTPARAM->TestParameter[j].name, parameter.name))
			{
				sprintf(svalue, "%s^d%d", PTESTPARAM->TestParameter[j].value, (long)(j+1));
				EXCCHECK( pvalue->Append(pvalue, svalue) );
				break;
			}
		}
		/* no value */
		if( i!=product_idSize-1)
		{
			EXCCHECK( pvalue->Append(pvalue, CHECK_RIGHTS_PARAMETER2("^r0¦", "^r1¦") ) );
		}
		else
		{
			EXCCHECK( pvalue->Append(pvalue, CHECK_RIGHTS_PARAMETER2("^r0", "^r1") ) );
		}
	}

Error:
	EXCRETHROW( pexception);       
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "TreeGetUpperLevelString"
static SElExceptionPtr TreeGetUpperLevelString(const char* keyNode, char* upperlevel)
{
	SElExceptionPtr pexception = NULL;  
	int32_t			error = 0;
	int				i;
	char*			result = NULL;
	char			keynode[32];
	char			number[8];
	int32_t			tree[DBS_NUMBER_OF_GROUPS]={0}; 
	int32_t			treeSize = 0;
	
	sprintf(keynode, "%s", keyNode);
	strcpy(upperlevel, "");
	
	result = strtok(keynode, "_");
	while(result != NULL)
	{
		sscanf(result, "%d", &tree[treeSize++]);
		result = strtok(NULL, "_");
	}
	
	for(i=0; i<treeSize-1; i++)
	{
		sprintf(number, "%d", tree[i]);
		strcat(upperlevel, number);
		if(i!=treeSize-2)
			strcat(upperlevel, "_");
	}

/* Error: */
	EXCRETHROW( pexception);       
}
