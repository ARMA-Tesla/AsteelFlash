#include <bklib/BKLib2.h> 
#include <toolbox.h>
#include "dbs_global.h"
#include "dbs_test.h"
#include "dbs_product.h"
#include "dbs_product_param.h" 
#include "dbs_product_param_edit.h" 
#include "dbs_product_master_edit.h"  
#include "ring_control.h"

#define BKTREE_HEADER	"root|Parametres^w200¦desc|Description^w60"

/* menu depth */
typedef enum _EBkTreeLevel
{
	BKTREE_LEVEL_ROOT = 1,
	BKTREE_LEVEL_PARAMETER
} EBkTreeLevel;

/* level root */
typedef enum _EBkTreeMenu
{
	BKTREE_MENU_ROOT_NEW_PARAMETER = 0,
	BKTREE_MENU_ROOT_PRODUCTVIEW
} EBkTreeMenu;

#define BKTREE_MENU_ROOT_MASTER_PRODUCTVIEW		0  

/* level parameter */
typedef enum _EBkTreeMenuProduct
{
	BKTREE_MENU_PRODUCT_PARAM_EDIT = 0,
	BKTREE_MENU_PRODUCT_PARAM_DEL,
	BKTREE_MENU_PRODUCT_PARAM_PRODUCTVIEW
}EBkTreeMenuProduct;

typedef enum _EBkTreeMenuMaster
{
	BKTREE_MENU_PRODUCT_MASTER_NEW = 0,
	BKTREE_MENU_PRODUCT_MASTER_DEL,
	BKTREE_MENU_PRODUCT_MASTER_PRODUCTVIEW
} EBkTreeMenuMaster;

extern SGlobalPtr		global;
extern int				gs_panel[];

static char				gs_keyNode[32];

static SElExceptionPtr ProductParamTreeString(void);
static SElExceptionPtr TestTreeItemLevel(char* keyNode, int32_t* level);
static SElExceptionPtr SetValueIntoTable(SELStringPtr pelstring, 
								 SProductParameter parameter, 
								 int32_t* product_id, 
								 int32_t product_idSize);
static SElExceptionPtr TreeGetUpperLevelString(const char* keyNode, char* upperlevel);

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "TreeProductParamIni"
SElExceptionPtr TreeProductParamIni(void)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	HRESULT			hr;
	char			config[512] = "";
	int				imglistHandle;
	
	global->editMode = EDIT_PRODUCT_PARAM;
	memset(global->keyNode[EDIT_PRODUCT_PARAM], 0, sizeof(global->keyNode[EDIT_PRODUCT_PARAM]));
	
	EXCCHECKCVI( GetObjHandleFromActiveXCtrl (gs_panel[DBS], DBS_IMAGELIST3, &imglistHandle));
	EXCCHECKCOM( BKLib2__DBKTreeSetImageList (global->treeHandle, NULL, imglistHandle));
	EXCCHECKCOM( BKLib2__DBKTreeSetAutoImages (global->treeHandle, NULL, VFALSE));

	EXCCHECK( PPARAM->ProductParamSort(PPARAM) ); 
	#ifndef _NO_MASTER
	EXCCHECK( PMASTER->ProductMasterSort(PMASTER) ); 
	#endif
	EXCCHECK( RingGroupAttribute( RING_GROUPS_LOCK) );
	EXCCHECK( TreeProductParamSet( PRODUCT_PARAM_RELOAD ) );
	EXCCHECK( TreeProductParamSet( PRODUCT_PARAM_SET_ACTIVE_NODE ) ); 
	
Error:
	EXCRETHROW( pexception);      
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "TreeProductParamSet"
SElExceptionPtr TreeProductParamSet(int32_t mode)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL; 
	HRESULT			hr;
	short int		AXvalue;
	char*			keyNode = NULL;
	
	switch(mode)
		{
		case PRODUCT_PARAM_RELOAD:
			EXCCHECKCOM( BKLib2__DBKTreeGetSelectedItem (global->treeHandle, NULL, &keyNode));  
			EXCCHECK( ProductParamTreeString());
			EXCCHECKCOM( BKLib2__DBKTreeFromString (global->treeHandle, NULL, (global->pelstring)->GetPtr(global->pelstring), "¦", "\n", &AXvalue));
			EXCCHECKCOM( BKLib2__DBKTreeSetEditable (global->treeHandle, NULL, VTRUE));  
			EXCCHECKCOM( BKLib2__DBKTreeSetEnableDragAndDrop (global->treeHandle, NULL, VFALSE)); 
			EXCCHECKCOM( BKLib2__DBKTreeSetEnableEventModify (global->treeHandle, NULL, VTRUE));
			EXCCHECKCOM( BKLib2__DBKTreeSetSelectedItem (global->treeHandle, NULL, keyNode));
			EXCCHECKCOM( BKLib2__DBKTreeEnsureVisible (global->treeHandle, NULL, keyNode));
			break;
		 case PRODUCT_PARAM_SET_ACTIVE_NODE:
			if( 0==strlen(global->keyNode[EDIT_PRODUCT_PARAM]) )
			{
				EXCCHECKCOM( BKLib2__DBKTreeSetSelectedItem (global->treeHandle, NULL, "parameter")); 
				EXCCHECKCOM( BKLib2__DBKTreeEnsureVisible (global->treeHandle, NULL, "parameter"));
			}
			else
				EXCCHECKCOM( BKLib2__DBKTreeSetSelectedItem (global->treeHandle, NULL, global->keyNode[EDIT_PRODUCT_PARAM])); 
			break;
		}
	
Error:
	if(keyNode) CA_FreeMemory( keyNode );     	
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/  
#undef __FUNC__
#define __FUNC__ "TreeProductParamClose"
SElExceptionPtr TreeProductParamClose(void)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	
	EXCCHECK( PPARAM->ProductParamSort(PPARAM) );
	#ifndef _NO_MASTER
	EXCCHECK( PMASTER->ProductMasterSort(PMASTER) );
	#endif
	EXCCHECK( RingGroupAttribute(RING_GROUPS_UNLOCK) );
	
Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "BkTreeProductParamNodeClick"
SElExceptionPtr BkTreeProductParamNodeClick (char *keyNode)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	HRESULT			hr;
	int32_t			level;
	short int		AXvalue;
	char*			parentNode = NULL;
	
	/* set active name */
	strcpy(global->keyNode[EDIT_PRODUCT_PARAM], keyNode);

	EXCCHECK( TestTreeItemLevel(keyNode, &level) );
	
	if( level==BKTREE_LEVEL_PARAMETER )
	{
		EXCCHECKCOM( BKLib2__DBKTreeGetParentNode (global->treeHandle, NULL, keyNode, &parentNode));           
		
		if(0==strcmp(parentNode, "parameter"))
		{
			EXCCHECKCOM( BKLib2__DBKTreeSetPopupMenu( global->treeHandle, 
													  NULL, 
													  _("Edit|Delete|<Product View>"), 
													  &AXvalue)); 
		}
		else if(0==strcmp(parentNode, "master"))
		{
			EXCCHECKCOM( BKLib2__DBKTreeSetPopupMenu( global->treeHandle, 
													  NULL, 
													  _("New|Delete|<Product View>"), 
													  &AXvalue)); 
		}
	}
	else
	{
		if(0==strcmp(keyNode, "parameter"))
		{
			EXCCHECKCOM( BKLib2__DBKTreeSetPopupMenu( global->treeHandle, 
													  NULL, 
													  _("New|<Product View>"), 
													  &AXvalue));
		}
		else if(0==strcmp(keyNode, "master"))
		{
			EXCCHECKCOM( BKLib2__DBKTreeSetPopupMenu( global->treeHandle, 
													  NULL, 
													  _("<Product View>"), 
													  &AXvalue));
		}
	}

Error:
	if(parentNode) CA_FreeMemory( parentNode ); 
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "BkTreeProductParamPopupMenuCommand"
SElExceptionPtr BkTreeProductParamPopupMenuCommand (char *keyNode,char *keyCol,long  menuItem)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	HRESULT			hr;
	int32_t			level;
	char*			parentNode = NULL;
	SProductParameter	parameter = {0};
	SProductMaster		master = {0}; 
	bool_t			free_description = FALSE;
	
	EXCCHECK( TestTreeItemLevel(keyNode, &level) );	    
	
	if(level==BKTREE_LEVEL_PARAMETER)
		EXCCHECKCOM( BKLib2__DBKTreeGetParentNode (global->treeHandle, NULL, keyNode, &parentNode));
	
	/* set pointer to parameter */
	if( level==BKTREE_LEVEL_PARAMETER /* PARAMETER PART */ 
		&& 0==strcmp(parentNode, "parameter") 
		&& 0!=strcmp(keyCol, "desc") 
		&& 0!=strcmp(keyCol, "root") )
	{
		long 		index;  
		
		EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemData (global->treeHandle, NULL, keyNode, keyCol, &index));
		if( index )
		{
			index -= 1;
			parameter = PPARAM->ProductParameter[index];
		}
		else  /* value does not exist, create one */
		{
			char*		name;
			char*		description;
		
			sscanf(keyCol, "%d",&parameter.product_id);	
			
			EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemText (global->treeHandle, NULL, keyNode, "root", &name));
			EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemText (global->treeHandle, NULL, keyNode, "desc", &description));  
			strncpy(parameter.name, name, DBS_RECORD_LENGHT_NAME);
			strcpy(PDBS_ALLOC(&parameter.description, DBS_RECORD_LENGHT_DESCRIPTION), description);
			free_description = (parameter.description!=NULL);
			strncpy(parameter.value, "@new", DBS_RECORD_LENGHT_VALUE_EXT); 
			
			EXCCHECK(PPARAM->ProductParamSetVType(PPARAM, &parameter));
			
			CA_FreeMemory( name ); 
			CA_FreeMemory( description ); 
		}
	}
	else if( level==BKTREE_LEVEL_PARAMETER /* MASTER PART */ 
		&& 0==strcmp(parentNode, "master") 
		&& 0!=strcmp(keyCol, "desc") 
		&& 0!=strcmp(keyCol, "root") )
	{
		long 		index;  
		char*		text;
		
		EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemData (global->treeHandle, NULL, keyNode, keyCol, &index));
		EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemText (global->treeHandle, NULL, keyNode, keyCol, &text));
		
		if( 0==strcmp(keyNode, "1_0") )
		{
			if( index || 0==strcmp(text, "none") )
			{	
				sscanf(keyCol, "%d",&master.product_id);
				master.master_id = index;
			}
			else
			{
				sscanf(keyCol, "%d",&master.product_id);  
				strcpy(master.master, "@newmaster");  
			}
		}
		else
		{
			if( index )
			{
				index -= 1;
				master = PMASTER->ProductMaster[index];
			}
			else
			{
				sscanf(keyCol, "%d",&master.product_id);
				strcpy(master.master, "@new"); 
			}
		}
		
		CA_FreeMemory( text );     
	}
	else 
	{
		char*  	nextCol;
		
		EXCCHECKCOM( BKLib2__DBKTreeGetColumnNext (global->treeHandle, NULL, "desc", &nextCol));
		
		sscanf(nextCol, "%d", &parameter.product_id); 
		sscanf(nextCol, "%d", &master.product_id);   
		
		strcpy(parameter.name, "@none");
		strcpy(master.master, "@none");
		
		CA_FreeMemory( nextCol ); 
	}
	
	if(level==BKTREE_LEVEL_PARAMETER)					 
	{
		if(0==strcmp(parentNode, "parameter"))
			switch(menuItem)
			{																  
				case BKTREE_MENU_PRODUCT_PARAM_EDIT:
					CHECK_RIGHTS_PARAMETER( EXCCHECK( EditProductParamDialog(parameter) ) );
					break;
				case BKTREE_MENU_PRODUCT_PARAM_DEL:
					CHECK_RIGHTS_PARAMETER( EXCCHECK( DelProductParamDialog(parameter, keyNode, keyCol) ) ); 
					break;
				case BKTREE_MENU_PRODUCT_PARAM_PRODUCTVIEW:
					EXCCHECK( ChangeMode(EDIT_PRODUCT) );
					break;
			}
		if(0==strcmp(parentNode, "master"))  
			switch(menuItem)
			{
				case BKTREE_MENU_PRODUCT_MASTER_NEW:
					CHECK_RIGHTS_PARAMETER( EXCCHECK( NewProductMasterDialog(master) ) );
					break;
				case BKTREE_MENU_PRODUCT_MASTER_DEL:
					CHECK_RIGHTS_PARAMETER( EXCCHECK( DelProductMasterDialog(master, keyNode, keyCol) ) );
					break;
				case BKTREE_MENU_PRODUCT_MASTER_PRODUCTVIEW:
					EXCCHECK( ChangeMode(EDIT_PRODUCT) );
					break;
			}
	}
	
	if(level==BKTREE_LEVEL_ROOT)
	{
		if(0==strcmp(keyNode, "parameter"))  
			switch(menuItem)
			{				
				case BKTREE_MENU_ROOT_NEW_PARAMETER:
					CHECK_RIGHTS_PARAMETER( EXCCHECK( InsertProductParamDialog(parameter) ) );
					break;
				case BKTREE_MENU_ROOT_PRODUCTVIEW:
					EXCCHECK( ChangeMode(EDIT_PRODUCT) );
					break;
			}
		if(0==strcmp(keyNode, "master"))
			switch(menuItem)
			{				
				case BKTREE_MENU_ROOT_MASTER_PRODUCTVIEW:
					EXCCHECK( ChangeMode(EDIT_PRODUCT) );
					break;
			}
	}
	
Error:
	if(free_description) PDBS_FREE(parameter.description);
	if(parentNode) CA_FreeMemory( parentNode ); 
	EXCRETHROW( pexception);    
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "BkTreeProductParamModify"
SElExceptionPtr BkTreeProductParamModify(char *keyNode, char *keyCol, long modifyType)
{
	int32_t 		error = 0;
	SElExceptionPtr pexception = NULL;
	HRESULT			hr;
	int32_t			level; 
	short int		AXvalue;
	char*			name = NULL;
	char*      		value = NULL;
	char*			description = NULL;
	char*			parentNode = NULL;  
	SProductParameter		parameter = {0};  
	SProductMaster			master = {0};  
	SProductMaster			masterold = {0};  
	
	EXCCHECK( TestTreeItemLevel(keyNode, &level) );	
	
	if(level==BKTREE_LEVEL_PARAMETER)
	{
		EXCCHECKCOM( BKLib2__DBKTreeGetParentNode (global->treeHandle, NULL, keyNode, &parentNode)); 
		
		if(0==strcmp(parentNode, "parameter"))   
		{
			long 		index;

			/* disable sorting algorithm */
			PPARAM->sort = FALSE;
		
			EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemText (global->treeHandle, 
														 NULL, 
														 keyNode, 
														 keyCol, 
														 &value));
			CHECK_VALUE( value);
		
			EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemData (global->treeHandle, NULL, keyNode, keyCol, &index));
		
			if( index )
			{
				index -= 1;
				parameter = PPARAM->ProductParameter[index]; 
				strncpy(parameter.value, value, DBS_RECORD_LENGHT_VALUE_EXT);
				EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemText (global->treeHandle, 
															 NULL, 
															 keyNode, 
															 keyCol, 
															 parameter.value,
															 &AXvalue));
				parameter.description = PDBS_COPY(parameter.description);
				parameter.user_id = global->activeUser.user_id;
				EXCCHECK( PPARAM->ProductParamEdit(PPARAM, parameter) ); 
				EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemData ( global->treeHandle, 
															  NULL, 
															  keyNode, 
															  keyCol, 
															  (long)(PPARAM->ProductParameterSize)));
			}
			else  /* value does not exist, create one */
			{
				sscanf(keyCol, "%d",&parameter.product_id);	
				EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemText (global->treeHandle, NULL, keyNode, "root", &name));
				EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemText (global->treeHandle, NULL, keyNode, "desc", &description));  
				strncpy(parameter.name, name, DBS_RECORD_LENGHT_NAME);
				strcpy(PDBS_ALLOC(&parameter.description, DBS_RECORD_LENGHT_DESCRIPTION), description);
				
				EXCCHECK(PPARAM->ProductParamSetVType(PPARAM, &parameter)); 
				
				parameter.user_id = global->activeUser.user_id;
				strncpy(parameter.value, value, DBS_RECORD_LENGHT_VALUE_EXT); 
			
				EXCCHECK( PPARAM->ProductParamInsert(PPARAM, parameter) );  
				EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemData ( global->treeHandle, 
															  NULL, 
															  keyNode, 
															  keyCol, 
															  (long)(PPARAM->ProductParameterSize)));
			
				CA_FreeMemory(name); 
				name = NULL;
				CA_FreeMemory(description); 
				description = NULL;
			}
			CA_FreeMemory(value);
			value = NULL;
		}
		if(0==strcmp(parentNode, "master"))     
		{
			long 		index;

			/* disable sorting algorithm */
			PMASTER->sort = FALSE;
			
			EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemText (global->treeHandle, NULL, keyNode, keyCol, &value));
			CHECK_VALUE( value);
		
			EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemData (global->treeHandle, NULL, keyNode, keyCol, &index));
		
			if( index )
			{
				index -= 1;
				master = PMASTER->ProductMaster[index]; 
				masterold = PMASTER->ProductMaster[index]; 
				strncpy(master.master, value, DBS_RECORD_LENGHT_VALUE);
				master.user_id = global->activeUser.user_id; 
				EXCCHECK( PMASTER->ProductMasterEdit(PMASTER, master, masterold) ); 
				EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemData ( global->treeHandle, NULL, keyNode, keyCol, 
												(long)(PMASTER->ProductMasterSize)));
				
				if(0==strcmp(masterold.master, "@empty"))
				{
					int 		val1=0, val2=0;
					char		nextNode[32]="";
					short int	exist = FALSE;	
				
					sscanf(keyNode, "%d_%d", &val1, &val2);
					sprintf(nextNode, "%d_%d", val1, val2+1);
					
					EXCCHECKCOM( BKLib2__DBKTreeIsNodeExist (global->treeHandle, NULL, nextNode, &exist));    
					if(exist)
						EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemEditable (global->treeHandle, NULL, nextNode, keyCol, TRUE));    	
				}
			}
			else  /* value does not exist, create one */
			{
				long 	master_id = 0;
				char*	master_text = NULL;
				
				sscanf(keyCol, "%d",&master.product_id);	
				strncpy(master.master, value, DBS_RECORD_LENGHT_VALUE); 
				master.user_id = global->activeUser.user_id; 
			
				EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemText (global->treeHandle, NULL, "1_0", keyCol, &master_text)); 
				if( 0==strlen(master_text) )
				{
					MessageBox((HWND)hWnd, _("Master Id not defined!"), _("Warning"), MB_OK|MB_ICONWARNING);
					EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemText (global->treeHandle, NULL, keyNode, keyCol, "", &AXvalue));  
				}
				else
				{
					int 		val1=0, val2=0;
					char		nextNode[32]="";
					short int	exist = FALSE;
				
					EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemData (global->treeHandle, NULL, "1_0", keyCol, &master_id));
					master.master_id = master_id;
				
					EXCCHECK( PMASTER->ProductMasterInsert(PMASTER, master) );  
					EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemData ( global->treeHandle, NULL, keyNode, keyCol, 
												(long)(PMASTER->ProductMasterSize)));
					
					sscanf(keyNode, "%d_%d", &val1, &val2);
					sprintf(nextNode, "%d_%d", val1, val2+1);
			
					EXCCHECKCOM( BKLib2__DBKTreeIsNodeExist (global->treeHandle, NULL, nextNode, &exist));
					if(!exist)
					{
						char	master_text2[32];
						char*	nextCol = NULL;
						
						sprintf(master_text2, "Master %02d", val2+1); 
						EXCCHECKCOM( BKLib2__DBKTreeAddNode (global->treeHandle, NULL, "master", nextNode, master_text2, &AXvalue));
						EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemImage (global->treeHandle, NULL, nextNode, "root", 1));
						
						EXCCHECKCOM( BKLib2__DBKTreeGetColumnNext (global->treeHandle, NULL, "desc", &nextCol));
						while(0!=strlen(nextCol))
						{
							char* lastCol = nextCol;
							EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemEditable (global->treeHandle, NULL, nextNode, nextCol, FALSE));
							EXCCHECKCOM( BKLib2__DBKTreeGetColumnNext (global->treeHandle, NULL, lastCol, &nextCol)); 	
							CA_FreeMemory(lastCol); 
							lastCol = NULL;
						}
						CA_FreeMemory(nextCol);
						nextCol = NULL;
					}
						
					EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemEditable (global->treeHandle, NULL, nextNode, keyCol, TRUE));
				}
				
				CA_FreeMemory(master_text);
				master_text = NULL;
			}
		 
			CA_FreeMemory(value);
			value = NULL;
		}
	}
		
Error:
	if(parentNode) CA_FreeMemory( parentNode ); 

	PPARAM->sort = TRUE;
	PMASTER->sort = TRUE; 

	if(pexception!=NULL)
	{
		TreeProductParamSet(PRODUCT_PARAM_RELOAD);
	}
	
	EXCRETHROW( pexception);   
}

#define TREE_STRUCTURE_SIZE 1024
/*---------------------------------------------------------------------------*/   
#undef __FUNC__
#define __FUNC__ "ProductParamTreeString"
static SElExceptionPtr ProductParamTreeString(void)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL; 
	SELStringPtr	pelstring = global->pelstring;
	SProductTypesPtr	pproduct = PPRODUCT->Product;
	int				i, j, k;  	
	char			path[64] = "";
	int32_t			pid[128] = {0};
	int32_t			pidSize = 0;
	int32_t			index = 0;   
	char*			used_name[512] = {""};
	bool_t			exist;
	char*			name = "";
	int32_t			code;
	int32_t			count;
	bool_t			found;
	char			row[2048]= {""};
	bool_t			last_found[512];
	
	SELStringPtr   	pvalue = NULL;
	
	EXCCHECK(elstring_new(NULL, &pvalue));
	
	/* search product_id */
	pid[pidSize++] = global->product_id;
	for(i=0; i<PPRODUCT->ProductSize; i++)
	{
		if( (pproduct[i].property&PROPERTY_VALID)>0 
			&& global->product_id == pproduct[i].parent_id)
			 pid[pidSize++] = pproduct[i].product_id;
	}
	
	/* create header */
	EXCCHECK( pelstring->Empty(pelstring) );
	EXCCHECK( pelstring->Append(pelstring, BKTREE_HEADER) ); 
	
	for(i=0; i<pidSize; i++)
	{
			EXCCHECK( PPRODUCT->ProductGetIdPath(PPRODUCT, pid[i], path, PRODUCT_PATH_LEVEL_3) );
			sprintf(row, "¦%d|%s^w150", /* value, pointer to value(unvisible) */ 
								pid[i],
								path);
			
			pelstring->Append(pelstring, row);  
	}
#if 1	
	/* problem with bktree */
	EXCCHECK( pelstring->Append(pelstring,"¦empty|^w10" ) ); 
#endif
	EXCCHECK( pelstring->Append(pelstring,"\n" ) );
	
/* create parameters table */
	EXCCHECK( pelstring->Append(pelstring, "|parameter|Parameters^i0^r1¦^r1") ); 
	for(i=0; i<pidSize; i++)
	{
		EXCCHECK( pelstring->Append(pelstring, "¦^r1") );
	}
	EXCCHECK( pelstring->Append(pelstring,"\n" ) );     
	
	for(i=0; i<PPARAM->ProductParameterSize; i++)
	{
		for(j=0; j<pidSize; j++)
		{
			if( (PPARAM->ProductParameter[i].property&PROPERTY_VALID)>0 
				&& pid[j] == PPARAM->ProductParameter[i].product_id)
			{
				/* do not repeat the same parameter name */
				exist = FALSE;
				for(k=0; k<index; k++)
				{
					if( 0==strcmp(used_name[k], PPARAM->ProductParameter[i].name) )
						exist = TRUE;	
				}
				
				if( !exist ) 
				{
					EXCCHECK( SetValueIntoTable(pvalue, 
												PPARAM->ProductParameter[i],
												pid, 
												pidSize) ); 
				
					EXCCHECK( pelstring->Format(pelstring, "parameter|0_%d|%s^i1^r1¦%s^r1¦%s\n", 
											++index,
											PPARAM->ProductParameter[i].name,
											GET_STRING(PPARAM->ProductParameter[i].description),
											pvalue->GetPtr(pvalue)
											) );
		
					used_name[index-1] = PPARAM->ProductParameter[i].name;
				}
			}
		}
	}

/* create masters table */
	EXCCHECK( pelstring->Append(pelstring, "|master|Masters^i0^r1¦^r1") ); 
	for(i=0; i<pidSize; i++)
	{
		EXCCHECK( pelstring->Append(pelstring, "¦^r1") );
	}
	EXCCHECK( pelstring->Append(pelstring,"\n" ) ); 
	
	/* Master Id */
	EXCCHECK( pelstring->Append(pelstring, "master|1_0|Master Id^i1^r1¦^r1¦") );
	for(i=0; i<pidSize; i++)
	{
		for(j=0; j<PMASTER->ProductMasterSize; j++)
		{
			if( (PMASTER->ProductMaster[j].property&PROPERTY_VALID)>0 
				&& pid[i] == PMASTER->ProductMaster[j].product_id)
			{
				EXCCHECK( PPRODUCT->ProductGetIdName(PPRODUCT, PMASTER->ProductMaster[j].master_id, &name) );
				if( 0==strcmp(name, "root") )
					name = "none";
				break;
			}
		}
		if(j<PMASTER->ProductMasterSize)
		{
			if( i!=pidSize-1)
				sprintf(row, "%s^r1^d%d¦", 
						name,
						PMASTER->ProductMaster[j].master_id);  	
			if( i==pidSize-1)  /* last column without ¦ */
				sprintf(row, "%s^r1^d%d¦^r1", 
						name,
						PMASTER->ProductMaster[j].master_id); 
			
			pelstring->Append(pelstring, row);
		}
		else /* no master id */
		{
			if( i!=pidSize-1)
				EXCCHECK( pelstring->Append(pelstring, "^r1¦") );  	
			if( i==pidSize-1) /* last column without ¦ */ 
				EXCCHECK( pelstring->Append(pelstring, "^r1¦^r1") ); 
		}
	}
	EXCCHECK( pelstring->Append(pelstring,"\n") );
	
	/* Master codes */
	code=0;
	index=1;
	memset(last_found, TRUE, 512);
	
	do
	{
		sprintf(row, "master|1_%d|Master %02d^i1^r1¦^r1¦", index, index);
		EXCCHECK( pelstring->Append(pelstring,row) );   
		
		index++;
		found=FALSE;
		
		for(i=0; i<pidSize; i++)
		{
			count = 0;
			
			for(j=0; j<PMASTER->ProductMasterSize; j++)
			{
				if( (PMASTER->ProductMaster[j].property&PROPERTY_VALID)>0 
					&& pid[i] == PMASTER->ProductMaster[j].product_id)
				{
					if(code==count)
					{
						found=TRUE;
						break;
					}
					count++;
				}
			}
			if(j<PMASTER->ProductMasterSize)
			{								   
				if( i!=pidSize-1)
					sprintf(row, 
							CHECK_RIGHTS_PARAMETER2("%s^r0^d%d¦","%s^r1^d%d¦"), 
							(0==strcmp(PMASTER->ProductMaster[j].master, "@empty"))? "empty":PMASTER->ProductMaster[j].master,
							(long)(j+1));  	
				if( i==pidSize-1) /* last column without ¦ */
					sprintf(row, 
							CHECK_RIGHTS_PARAMETER2("%s^r0^d%d¦^r1","%s^r1^d%d¦^r1"), 
							(0==strcmp(PMASTER->ProductMaster[j].master, "@empty"))? "empty":PMASTER->ProductMaster[j].master,
							(long)(j+1)); 

				pelstring->Append(pelstring, row);    
			}
			else /* no master code */
			{
				if( i!=pidSize-1)
					EXCCHECK( pelstring->Append(pelstring, 
										CHECK_RIGHTS_PARAMETER2(
											(last_found[i]==TRUE)? "^r0¦":"^r1¦",
											"^r1¦")) );  	
				if( i==pidSize-1) /* last column without ¦ */
					EXCCHECK( pelstring->Append(pelstring, 
										CHECK_RIGHTS_PARAMETER2(
											(last_found[i]==TRUE)? "^r0¦^r1":"^r1¦^r1",
											"^r1¦^r1")) ); 
			}
			
			last_found[i] = ( (j<PMASTER->ProductMasterSize) && 0!=strcmp(PMASTER->ProductMaster[j].master, "@empty") ); 
		}
		EXCCHECK( pelstring->Append(pelstring,"\n" ) );
		
		code++;
	
	}while(found);
	
Error:
	elstring_delete(&pvalue);          
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "TestTreeItemLevel"
static SElExceptionPtr	TestTreeItemLevel(char* keyNode, int32_t* level)
{
	int32_t 		error = 0;
	SElExceptionPtr pexception = NULL;    
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
	SProductParameter parameter,
	int32_t* pid, 
	int32_t pidSize
)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL; 
	int 			i, j;
	char			svalue[512];

	EXCCHECK( pvalue->Empty(pvalue) );
	
	for(i=0; i<pidSize; i++)
	{
		for(j=PPARAM->ProductParameterSize-1; j>=0; j--)
		{
			if( (PPARAM->ProductParameter[j].property&PROPERTY_VALID)>0
				&& PPARAM->ProductParameter[j].product_id == pid[i]
				&& 0==strcmp(PPARAM->ProductParameter[j].name, parameter.name))
			{
				sprintf(svalue, "%s^d%d", 
						(PPARAM->ProductParameter[j].vtype==E_DVT_TEXT)? "#Multiline Text":PPARAM->ProductParameter[j].value, 
						(long)(j+1));
				EXCCHECK( pvalue->Append(pvalue, svalue) );  
				break;
			}
		}
		/* no value */
		if( i!=pidSize-1)  
			EXCCHECK( pvalue->Append(pvalue, CHECK_RIGHTS_PARAMETER2("^r0¦", "^r1¦") ) );
	
		if( i==pidSize-1)  /* last column without ¦ */
			EXCCHECK( pvalue->Append(pvalue, CHECK_RIGHTS_PARAMETER2("^r0¦^r1", "^r1¦^r1") ) );
	}

Error:
	EXCRETHROW( pexception);  
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "TreeGetUpperLevelString"
static SElExceptionPtr TreeGetUpperLevelString(const char* keyNode, char* upperlevel)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL; 
	int				i;
	char*			result = NULL;
	char			keynode[32];
	char			number[8];
	int32_t			tree[DBS_NUMBER_OF_GROUPS] = {0}; 
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
