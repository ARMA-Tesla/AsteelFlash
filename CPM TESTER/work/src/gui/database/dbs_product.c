#include <bklib/BKLib2.h> 
#include "toolbox.h"
#include <formatio.h>
#include "dbs_global.h"
#include <stdio.h>
#include "dbs_product.h"
#include "dbs_product_edit.h"  
#include "dbs_product_param.h" 
#include "dbs_test.h"
#include "dbs_test_param.h"
#include "ring_control.h"

#define BKTREE_HEADER0  _("root|produits^w300¦desc|Description^w855¦vis0|V^w30\n")
#define BKTREE_HEADER1	"|root|Produits^i0\n"

#define BKTREE_MENU_PRODUCT _("Nouveau|Copier|Editer|Effacer|Parametres|Process 	>")
#define BKTREE_MENU_ROOT _("Nouveau|Process 	>")


typedef enum _EBkTreeMenuProduct
{
	BKTREE_MENU_PRODUCT_NEW	= 0,
	BKTREE_MENU_PRODUCT_COPYSTRUCT,
	BKTREE_MENU_PRODUCT_EDIT,
	BKTREE_MENU_PRODUCT_DEL,
	BKTREE_MENU_PRODUCT_PARAMVIEW,
	BKTREE_MENU_PRODUCT_LINEVIEW
} EBkTreeMenuProduct;

typedef enum _EBkTreeMenuRoot
{
	BKTREE_MENU_ROOT_PRODUCT_NEW = 0,
	BKTREE_MENU_ROOT_PRODUCT_LINEVIEW
} EBkTreeMenuRoot;

extern SGlobalPtr		global;
extern int				gs_panel[];

char					gs_file_name[512] = "";

static SElExceptionPtr ProductTreeString(void);

/*---------------------------------------------------------------------------*/  
#undef __FUNC__
#define __FUNC__ "TreeProductIni"
SElExceptionPtr TreeProductIni(void)
{
	SElExceptionPtr pexception =  NULL;  
	int32_t			error = 0;
	HRESULT			hr;
	CAObjHandle		imglistHandle;
	short			AXvalue;
	
	EXCCHECKCOM( BKLib2__DBKTreeSetRedraw(global->treeHandle, NULL, 0, NULL));

	EXCCHECK( ProductTreeString());

	EXCCHECKCOM( BKLib2__DBKTreeFromString (global->treeHandle, NULL, (global->pelstring)->GetPtr(global->pelstring), "¦", "\n", &AXvalue));
	EXCCHECKCOM( BKLib2__DBKTreeSetEditable(global->treeHandle, NULL, FALSE));
	EXCCHECKCOM( BKLib2__DBKTreeSetEnableDragAndDrop (global->treeHandle, NULL, VFALSE));

	EXCCHECKCVI( GetObjHandleFromActiveXCtrl (gs_panel[DBS], DBS_IMAGELIST2, &imglistHandle));
	EXCCHECKCOM( BKLib2__DBKTreeSetImageList (global->treeHandle, NULL, imglistHandle));
	EXCCHECKCOM( BKLib2__DBKTreeSetAutoImages (global->treeHandle, NULL, VFALSE));
	
	if(global->product_id==0)
	{
		EXCCHECKCOM( BKLib2__DBKTreeExpand (global->treeHandle, NULL, "root", &AXvalue));	
		EXCCHECKCOM( BKLib2__DBKTreeSetSelectedItem (global->treeHandle, NULL, "root"));
	}
	else
	{
		int32_t pid[1024];
		int32_t pidSize = 1024, i;
		char keynode[32];

		EXCCHECK( PPRODUCT->ProductGetIdTreeUp(PPRODUCT, global->product_id, pid, &pidSize) );
		for(i=pidSize; i>=0; i--)
		{
			memset(keynode, 0, sizeof(keynode));
			sprintf(keynode, "%d", pid[i]);
			EXCCHECKCOM( BKLib2__DBKTreeExpand (global->treeHandle, NULL, keynode, &AXvalue));
			EXCCHECKCOM( BKLib2__DBKTreeSetSelectedItem (global->treeHandle, NULL, keynode));
		}
	}

	EXCCHECKCOM( BKLib2__DBKTreeSetRedraw(global->treeHandle, NULL, 1, NULL));
	EXCCHECKCOM( BKLib2__DBKTreeSetScrollBarVisible(global->treeHandle, NULL, 0, 1));

	EXCCHECK( RingGroupAttribute(RING_GROUPS_VISIBLE)); 
	
Error:
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "TreeProductClose"
SElExceptionPtr TreeProductClose(void)
{
	SElExceptionPtr pexception =  NULL; 
	
/* Error: */
	EXCRETHROW(pexception); 
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "BkTreeProductNodeClick"
SElExceptionPtr BkTreeProductNodeClick (char *keyNode)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL;
	HRESULT			hr;
	short int		AXvalue; 
	int32_t			product_id;
	char*			parentNode = NULL;
	bool_t			reloaded;

	if( 0!=strcmp(keyNode, "root"))
	{
		sscanf(keyNode, "%d", &product_id);
		global->product_id = product_id; 
		
		EXCCHECKCOM( BKLib2__DBKTreeSetPopupMenu( global->treeHandle, 
												  NULL,
												  BKTREE_MENU_PRODUCT, 
												  &AXvalue)); 

		EXCCHECKCOM( BKLib2__DBKTreeGetParentNode(global->treeHandle, NULL, keyNode, &parentNode));
		if( 0==strcmp(parentNode, "root"))
		{
			/* after close this plugin return previous branch_id */
			if(global->branch_id==0)
				global->branch_id = PDBS->_branch_id;

			EXCCHECK( PDBS->ChangeBranchId(PDBS, product_id, &reloaded));
			if(reloaded)
			{
				EXCCHECK( TreeProductIni());
			}

			EXCCHECK( RingGroupAttribute( RING_GROUPS_RELOAD));
		}
		
		global->set_product_id = product_id;
		EXCCHECK( RingGroupAttribute( RING_GROUPS_SET_PATH));

		CA_FreeMemory(parentNode);
	}
	else
	{
		global->set_product_id = 0;
		EXCCHECK( RingGroupAttribute(RING_GROUPS_SET_INI_PATH));
		
		EXCCHECKCOM( BKLib2__DBKTreeSetPopupMenu( global->treeHandle, 
												  NULL, 
												  BKTREE_MENU_ROOT, 
												  &AXvalue)); 
	}
	
Error:
	EXCRETHROW(pexception);   
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "BkTreeProductPopupMenuCommand"
SElExceptionPtr BkTreeProductPopupMenuCommand (char *keyNode,char *keyCol,long  menuItem)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL; 
	HRESULT			hr;
	long			index;
	SProductTypes	product = {0};
	
	EXCCHECKCOM( BKLib2__DBKTreeGetNodeData (global->treeHandle, NULL, keyNode, &index)); 
	if(index)
	{
		index -= 1;
		product = PPRODUCT->Product[index];
	}

	if(0!=strcmp(keyNode, "root"))
	{
		switch(menuItem)
		{																  
			case BKTREE_MENU_PRODUCT_NEW:
				CHECK_RIGHTS_PRODUCT( EXCCHECK(NewProductDialog(product)));
				break;
			case BKTREE_MENU_PRODUCT_COPYSTRUCT:
				CHECK_RIGHTS_PRODUCT( EXCCHECK(CopyProductDialog(product)));
				break;
			case BKTREE_MENU_PRODUCT_EDIT:
				CHECK_RIGHTS_PRODUCT( EXCCHECK(EditProductDialog(product)));
				break;
			case BKTREE_MENU_PRODUCT_DEL:
				CHECK_RIGHTS_PRODUCT( EXCCHECK(DelProductDialog(product)));
				break;
			case BKTREE_MENU_PRODUCT_PARAMVIEW:
				EXCCHECK( ChangeMode(EDIT_PRODUCT_PARAM));
				break;
#if 0    /* formerly excluded lines */
			case BKTREE_MENU_PRODUCT_TESTVIEW:
				EXCCHECK( ChangeMode(EDIT_TEST));
				break;
#endif   /* formerly excluded lines */
			case BKTREE_MENU_PRODUCT_LINEVIEW:
				EXCCHECK( ChangeMode(EDIT_LINE));
				break;
		}
	}
	else
	{
		switch(menuItem)
		{																  
			case BKTREE_MENU_ROOT_PRODUCT_NEW:
				CHECK_RIGHTS_ROOT( EXCCHECK(NewProductDialog(product)));
				break;
#if 0    /* formerly excluded lines */
			case BKTREE_MENU_ROOT_PRODUCT_TESTVIEW:
				EXCCHECK( ChangeMode(EDIT_TEST));
				break;
#endif   /* formerly excluded lines */
			case BKTREE_MENU_ROOT_PRODUCT_LINEVIEW:
				EXCCHECK( ChangeMode(EDIT_LINE));
				break;
		}
	}

Error: 
	EXCRETHROW(pexception);
}

#define TREE_STRUCTURE_SIZE 4096
#define TREE_ROW_SIZE 		1024
/*---------------------------------------------------------------------------*/  
#undef __FUNC__
#define __FUNC__ "ProductTreeString"
static SElExceptionPtr ProductTreeString(void)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL; 
	int				i, j;
	SProductTypesPtr		pproduct = PPRODUCT->Product;
	SELStringPtr	pelstring = global->pelstring;
	int32_t			pidSize = TREE_STRUCTURE_SIZE;
	int32_t			index = 0;
	int32_t			product_id[32];
	int32_t			level = 0;
	int32_t			new_value = 0;
	char			row[TREE_ROW_SIZE] = "";    
	int32_t			treeIndex = 0;
	STesterItemPtr	ptester = NULL;
	int32_t			branch_id =	(global->branch_id)? global->branch_id:PDBS->_branch_id;
	
	memset(product_id, 0, sizeof(product_id));
	
	EXCCHECK( pelstring->Empty(pelstring) );
	EXCCHECK( pelstring->Append(pelstring, BKTREE_HEADER0) );
	EXCCHECK( pelstring->Append(pelstring, BKTREE_HEADER1) );
	
	if(global->activeUser.tester_id!=0)
		EXCCHECK( PTESTER->TesterGetSelected(PTESTER, global->activeUser.tester_id, &ptester) );

	for(i=0; i<PPRODUCT->ProductSize; i++)
	{
		if( (pproduct[i].property&PROPERTY_VALID)>0
			&& pproduct[i].parent_id == 0
			&& (global->activeUser.tester_id==0 /* root user */
			|| (ptester && (ptester->product_id==pproduct[i].product_id || branch_id==pproduct[i].product_id)))) /* common user */
		{
			EXCCHECK( pelstring->Format(pelstring, "root|%d|%s^i1^d%d¦%s\n",
								pproduct[i].product_id,
								pproduct[i].name,
								(long)(i+1),
								GET_STRING(pproduct[i].description) ) );
			
			/* lower levels */
			product_id[level++] = pproduct[i].product_id;
			do
			{
				for(j=0; j<PPRODUCT->ProductSize; j++)
				{
					if( (pproduct[j].property&PROPERTY_VALID)>0 
						&& pproduct[j].parent_id == product_id[level-1])
					{
						
						if(new_value==1 || product_id[level] == 0)  
						{
							product_id[level++] = pproduct[j].product_id;
				
							/* add row to bktree string */
							sprintf(row, "%d|%d|%s^i%d^d%d¦%s¦%d^r0\n",
										 pproduct[j].parent_id,
										 pproduct[j].product_id,
										 pproduct[j].name,
										 level,
										 (long)(j+1),
										 GET_STRING(pproduct[j].description),
										 (pproduct[j].property&PROPERTY_VISIBLE)>0
										 ); 
							EXCCHECK( pelstring->Append(pelstring, row) );  
							break;
						}
						/* do not repeat values */
						if(product_id[level] == pproduct[j].product_id)
							new_value = 1;
					}
				}
				/* parent_id was not found */
				if(j==PPRODUCT->ProductSize)
				{
					product_id[level] = 0;
					level--;
					new_value = 0;
				}	
				
			}while(level>0);	
		}
	}
	
Error:
	EXCRETHROW(pexception);    
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "{Line Editor Plugin}::OnOpenCsvFile"
int  CVICALLBACK OnOpenCsvFile(int panel, int control, int event, 
   void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr pexception =  NULL;  
	int32_t      	error = 0;

	
	switch (event)
	{
		case EVENT_COMMIT:

			if(FileSelectPopup ("", "*.csv", "", _("Selectionner un fichier d'importation"), VAL_LOAD_BUTTON, 0, 1, 1, 0, gs_file_name))
			{
				EXCCHECKCVI( SetCtrlVal(gs_panel[DBS], TABPANEL_1_CSV_FILE, gs_file_name));
			}
			break;
	}
   
Error:  
	EXCDISPLAY( pexception);
   	return 0;   
}

/*---------------------------------------------------------------------------*/ 
/*
VOLKSWAGEN ; POLO ; CONFORTLINE
VOLKSWAGEN ; POLO ; TRENDLINE
VOLKSWAGEN ; POLO ; HIGHLINE

PEUGEOT ; 307 ; HIGHLINE 
PEUGEOT ; POLO ; TRENDLINE  
*/

#undef __FUNC__
#define __FUNC__ "{Line Editor Plugin}::OnImportCsvFile"
int  CVICALLBACK OnImportCsvFile(int panel, int control, int event, 
   void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr pexception =  NULL;  
	int32_t      	error = 0;
   	char			buffer[512] = "";
	char			product_name[128] = "";
	char			last_product_name[128] = "";
	char			line_path[256] = "";
	char			path[256] = "";
	int             HandleFileCSV;
	int             value;
	SProductTypes	product = {0};
	int 			product_id = 0;
	int 			id_parent = 0;
	int             index_line = 0;
	int             last_index_line = 0;
	int             isValid;
	int             type;
	int				Next_Line_Type_Path_Param = 0;   //0 : path //1 : param

	char			parameter_name[128] = "";
	SProductParameter	parameter = {0};
   	char			item[512] = "";
	int End;
		
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlVal (panel, TABPANEL_1_TYPE, &type);
			
			SetCtrlAttribute (panel, TABPANEL_1_MESSAGE, ATTR_VISIBLE, 1);
			
			switch (type)
			{
				case 0 : // produits
				{
				/* extraire les datas à sauvegarder dans la base ligne par ligne */
				 HandleFileCSV = OpenFile (gs_file_name, VAL_READ_ONLY, VAL_OPEN_AS_IS, VAL_ASCII);

				 while (ReadLine (HandleFileCSV, buffer, -1) > 0)
				 {
				  	sprintf (line_path, ""); 
					index_line = 0;
					id_parent = 0;
					
					while (index_line >= 0)
					{
						index_line = FindPattern (buffer, last_index_line, -1, ";", 0, 0);
						
						if (index_line >= 0)
				 		CopyString (product_name, 0, buffer, last_index_line, index_line - last_index_line);
						else 
						CopyString (product_name, 0, buffer, last_index_line, -1); //last item
						
			     		sprintf(last_product_name, "%s", product_name);
				 		last_index_line = index_line + 1;
						strcat(line_path,product_name);	   
						
						
				 		/* vérifier si existe dans la BDD sous le id_parent (on peut trouver confortline dans plusieurs sous famille) */
				  		sprintf(path, line_path); 
						EXCCHECK( PPRODUCT->ProductGetIdFromPath(PPRODUCT, path, &product_id));
				  		EXCCHECK( PPRODUCT->ProductIsValidProduct(PPRODUCT, product_id, &isValid));
						
						strcat(line_path, "\\"); 

				  		if (product_id && isValid) /*parent exist*/
				  		{
						id_parent = product_id; /*save as parent for next level*/ 
				  		}
				 	 	else /* last */
				  		{
						/* affecter les valeurs */
						product.parent_id = id_parent;
						CHECK_VALUE(product_name);
				    	sprintf(product.name, product_name); //pas de description
						PDBS_ALLOC(&product.description, DBS_RECORD_LENGHT_DESCRIPTION);
						sprintf(product.description, ""); //pas de description
						value = 1; //visible
						if(value) product.property |= PROPERTY_VISIBLE;
						product.user_id = global->activeUser.user_id;
						EXCCHECK( PPRODUCT->ProductNew(PPRODUCT, &product)); // ajouter un nouveau product_id
						global->product_id = product.product_id;
						id_parent = product.product_id;
						//EXCCHECK(PDBS->Save(PDBS));
				  		}
					}
				 }
				  
				 						
				 EXCCHECK( TreeProductIni());
	
				 if(IDOK==MessageBox((HWND)hWnd, _("enregistrer produits dans la base des données?"), _("enregistrer"), MB_OKCANCEL|MB_ICONQUESTION))
					EXCCHECK(PDBS->Save(PDBS));

				}
				break;

				
				case 2 : // données paramétres
				{
			
				/* extraire les datas à sauvegarder dans la base ligne par ligne */
				 HandleFileCSV = OpenFile (gs_file_name, VAL_READ_ONLY, VAL_OPEN_AS_IS, VAL_ASCII);

				 while (ReadLine (HandleFileCSV, buffer, -1) > 0)
				 {

				  	sprintf (line_path, ""); 
					index_line = 0;
					product_id = 0;
					last_index_line = 0;
					
					while (index_line >= 0)
					{
					index_line = FindPattern (buffer, last_index_line, -1, ";", 0, 0);
					
					if (index_line >= 0)
				 	CopyString (item, 0, buffer, last_index_line, index_line - last_index_line);
					else 
					CopyString (item, 0, buffer, last_index_line, -1); //last item
					
					if (FindPattern (item, 0, -1, "#", 0, 0) >= 0)  //parameter name
					{
					/* Get product_id from path */
				  	sprintf(path, line_path); 
					EXCCHECK( PPRODUCT->ProductGetIdFromPath(PPRODUCT, path, &product_id));
					CopyString (parameter_name, 0, item, 1, -1); //first #
					last_index_line = index_line + 1;
					}
				
					else if (product_id > 0 && strlen(item) > 0 ) //parameter value to insert
					{
					parameter.product_id = product_id;
					sprintf(parameter.name, parameter_name); //name parameter
					parameter.vtype = 2; // string	
					sprintf(parameter.value, item); //name parameter
					PDBS_ALLOC(&product.description, DBS_RECORD_LENGHT_DESCRIPTION);
					sprintf(product.description, ""); //pas de description
					parameter.user_id = global->activeUser.user_id;
					global->product_id = product_id;
				   				
					last_index_line = index_line + 1;

					pexception = PPARAM->ProductParamInsert(PPARAM, parameter);
					/*if (pexception == NULL)
						{
						EXCCHECK( TreeProductParamSet(PRODUCT_PARAM_RELOAD));
						}*/
					}	
						
					else if (index_line >= 0)//path
					{
				 	last_index_line = index_line + 1;
					strcat(line_path, item);	   
					strcat(line_path, "\\"); 	
					}

				 }
					
				}
			
				EXCCHECK( TreeProductParamSet(PRODUCT_PARAM_RELOAD));

				 if(IDOK==MessageBox((HWND)hWnd, _("enregistrer paramétres dans la base des données?"), _("enregistrer"), MB_OKCANCEL|MB_ICONQUESTION))
					EXCCHECK(PDBS->Save(PDBS));

				}
				break;

			}
			
			SetCtrlAttribute (panel, TABPANEL_1_MESSAGE, ATTR_VISIBLE, 0);

		break;
	}
   
Error:  
	EXCDISPLAY( pexception);
   	return 0;   
}
