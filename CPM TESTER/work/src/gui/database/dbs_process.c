#include <bklib/BKLib2.h>
#include "dbs_global.h"
#include <dbs/dbs.h>
#include <elstring.h>
#include <stdio.h>
#include <msxmldom.h>
#include "toolbox.h"
#include "dbs_global.h"
#include "dbs_process.h"
#include "ring_control.h"
#include "resource.h"
#include "dbs_test_edit.h"
#include "dbs_test.h"

#define	_STEP_NEW_WITH_PARAM	1 /* automatically generate parameters with a new step */

extern SGlobalPtr				global;
extern int						gs_panel[];

#define BKTREE_HEADER0			"root|Process/Step^w200¦edit|E^w40¦name|Test/Parameter^w200¦desc|Description^w400¦value|Value^w150¦path|Path^w150¦info|I^w100\n"
#define BKTREE_HEADER1			"|0|Processes^i0^r1\n"

#define BKTREE_MENU_ROOT		_("Nouveau Station|Liste des Produits 	>")
#define BKTREE_MENU_PROCESS		_("Nouveau Station Avant|Nouveau Station Aprés|Effacer Station|Nouveau Test|Liste Des Tests		>")
//#define BKTREE_MENU_TEST		_("Nouveau Test avant|Nouveau Test aprés|Editer Test|Skipper Test|Annuler Skip|Effacer Test|Déplacer Test")
#define BKTREE_MENU_TEST		_("Nouveau Test avant|Nouveau Test aprés|Editer Test|Skipper Test|Annuler Skip|Effacer Test")
#define BKTREE_MENU_PARAMETER   _("Nouveau Paramètre |Editer Paramètre |Effacer Paramètre |Paramètres De Test 	>")

/* menu depth */
typedef enum _EBkTreeLevel
{
	BKTREE_LEVEL_ROOT = -1,
	BKTREE_LEVEL_NONE,
	BKTREE_LEVEL_PROCESS,
	BKTREE_LEVEL_TEST,
	BKTREE_LEVEL_PARAMETER //pas utilisé 
} EBkTreeLevel;

/* root menu */
typedef enum _EBkTreeMenuRoot
{
	BKTREE_MENU_ROOT_NEW_PROCESS = 0,
	BKTREE_MENU_ROOT_PRODUCT_VIEW
} _EBkTreeMenuProcess;

/* process menu */
typedef enum _EBkTreeMenuProces
{
	BKTREE_MENU_PROCESS_NEW_BEFORE = 0,
	BKTREE_MENU_PROCESS_NEW_AFTER,
	BKTREE_MENU_PROCESS_DEL,
	BKTREE_MENU_PROCESS_NEW_STEP,
	BKTREE_MENU_PRODUCT_TESTVIEW,
} _EBkTreeMenuProces;

/* test menu */
typedef enum _EBkTreeMenuTest
{
	BKTREE_MENU_TEST_INSERT_BEFORE = 0,
	BKTREE_MENU_TEST_INSERT_AFTER,
	BKTREE_MENU_TEST_EDIT,
	BKTREE_MENU_TEST_SKIP,
	BKTREE_MENU_TEST_REMOVE_SKIP,
	BKTREE_MENU_TEST_DEL,
	BKTREE_MENU_TEST_MOVE,
} EBkTreeMenuTest;

/* parameter menu */ 
typedef enum _EBkTreeMenuParameter
{
	BKTREE_MENU_PARAMETR_NEW = 0,
	BKTREE_MENU_PARAMETR_EDIT,
	BKTREE_MENU_PARAMETR_DEL,
	BKTREE_MENU_PARAMETR_VIEW
} EBkTreeMenuParameter;

/* editable items parametrs */
#define TREE_EDIT_NO	0
#define TREE_EDIT_DEL	(1 << 0)
#define TREE_EDIT_SKIP	(1 << 1)

static int32_t      gs_product_id;
static int32_t      gs_test_nb;
static int32_t      gs_process_nb;


extern SElExceptionPtr ChangeMode (int32_t mode); 
static SElExceptionPtr ProcessTreeString(SELStringPtr pelstring);
static SElExceptionPtr ProcessTreeInfoString(int32_t process_nb, char* info, int32_t infoSize);
static SElExceptionPtr StepTreeInfoString(int32_t step_nb, char* info, int32_t infoSize);
static SElExceptionPtr StepTreeSkip(int32_t step_nb, int32_t* skip);
static SElExceptionPtr ProcessTreeSkip(int32_t process_nb, int32_t* skip);
static SElExceptionPtr ParamTreeInfoString(int32_t step_nb, char* name, char* info, int32_t infoSize);
static SElExceptionPtr ParamTreeVisible(int32_t fnc_nb, char* name, int32_t product_id,	int32_t* visible);
static SElExceptionPtr InsertTestDialog_New(STestStep test, char* sname);
static SElExceptionPtr Process_InsertTestDialog(STestStep test, char* sname);
static SElExceptionPtr Process_EditTestDialog(STestStep test, char* sname);
static SElExceptionPtr Process_SkipTestDialog(STestStep	test, int32_t editable);
static SElExceptionPtr Process_RemoveSkipTestDialog(STestStep test, int32_t editable);
static SElExceptionPtr Process_DelTestDialog(STestStep	test, int32_t editable);


static int CVICALLBACK OnExitInsert (int panel, int control, int event,
      void *callbackData, int eventData1, int eventData2);
static int CVICALLBACK OnExitEdit (int panel, int control, int event,
      void *callbackData, int eventData1, int eventData2);

static SElExceptionPtr TestStepTreeString(
	SELStringPtr	pelstring, 
	int32_t 		process_nb, 
	int32_t 		process_index, 
	int32_t* 		ProductId, 
	int32_t 		ProductIdSize
	);
static SElExceptionPtr TestTreeInfoString(int32_t test_nb, char* info, int32_t infoSize);
static SElExceptionPtr TestTreeSkip(int32_t test_nb, int32_t* skip);
static SElExceptionPtr TestParameterTreeString(
			SELStringPtr pelstring,
			int32_t test_nb, 
			int32_t test_index,
			int32_t* ProductId, 
			int32_t ProductIdSize);
static SElExceptionPtr TestTreeParameterInfoString(int32_t test_nb, char* name, char* info, int32_t infoSize);
static SElExceptionPtr TestTreeParameterVisible(
			int32_t test_nb, 
			char* name,
			int32_t	product_id,
			int32_t* visible);
/*---------------------------------------------------------------------------*/  
static int GetLevelBkTree(char *keyNode, int *a, int *b)
{
int	level = 0;
	 
*a = 0; 
*b = 0;
	
	sscanf(keyNode, "0_%d_%d", a, b);
	
	if (*a == 0 && *b == 0)
		level = -1;
	else if (*a>0 && *b == 0)
		level = 1;
	else if (*a>0 && *b>0)
		level = 2;
	else 
		level = 0;
	
return  level;
}
/*---------------------------------------------------------------------------*/  
#undef __FUNC__
#define __FUNC__ "BkTreeProcessNodeClick"
SElExceptionPtr BkTreeProcessNodeClick (char *keyNode)
{
	SElExceptionPtr pexception = NULL; 
	HRESULT			hr;
	short int		AXvalue;
	char*			pedit = NULL;
	int32_t 		editable = 0;
	int32_t			level, a, b, c;

	//level = sscanf(keyNode, "0_%d_%d_%d", &a, &b, &c);
	level = GetLevelBkTree(keyNode, &a, &b); 

	if(level==BKTREE_LEVEL_ROOT)
	{
		EXCCHECKCOM( BKLib2__DBKTreeSetPopupMenu(global->treeHandle, NULL, BKTREE_MENU_ROOT, &AXvalue));
	}
	else
	{
		EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemText (global->treeHandle, NULL, keyNode, "edit", &pedit));
		sscanf(pedit, "%d", &editable);
		if(editable)
		{
			if(level==BKTREE_LEVEL_PROCESS)
				EXCCHECKCOM( BKLib2__DBKTreeSetPopupMenu(global->treeHandle, NULL, BKTREE_MENU_PROCESS, &AXvalue));
			if(level==BKTREE_LEVEL_TEST)
				EXCCHECKCOM( BKLib2__DBKTreeSetPopupMenu( global->treeHandle, NULL, BKTREE_MENU_TEST, &AXvalue));
			else if(level==BKTREE_LEVEL_PARAMETER)
				EXCCHECKCOM( BKLib2__DBKTreeSetPopupMenu( global->treeHandle, NULL, BKTREE_MENU_PARAMETER, &AXvalue));
		}
		else
		{
			EXCCHECKCOM( BKLib2__DBKTreeSetPopupMenu(global->treeHandle, NULL, NULL, &AXvalue));
		}
	}

Error:
	CA_FreeMemory(pedit); 
	EXCRETHROW( pexception);       
}

/*---------------------------------------------------------------------------*/  
#undef __FUNC__
#define __FUNC__ "BkTreeProcessPopupMenuCommand"
SElExceptionPtr BkTreeProcessPopupMenuCommand (char *keyNode, char *keyCol, long menuItem)
{
	SElExceptionPtr pexception = NULL; 
	int32_t			level, a=0, b=0, c=0, index;
	HRESULT			hr = 0;
	char*			sname;
   	int32_t			stepNumber = -1;
	int32_t			editable;
	char*			sedit;


	EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemData (global->treeHandle, NULL, keyNode, "root", &index));
   	EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemText (global->treeHandle, NULL, keyNode, "root", &sname));
   	EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemText (global->treeHandle, NULL, keyNode, "edit", &sedit));
	
	sscanf(sedit, "%d", &editable);
	//level = sscanf(keyNode, "0_%d_%d_%d", &a, &b, &c);
	level = GetLevelBkTree(keyNode, &a, &b); 

	if(level==BKTREE_LEVEL_ROOT)
	{
		switch(menuItem)
		{
			case BKTREE_MENU_ROOT_NEW_PROCESS:
				{
					char nkeyNode[32];
					char firstNode[32];
					short exist;

					sprintf(nkeyNode, "0_1_new");
					EXCCHECKCOM( BKLib2__DBKTreeAddNode(global->treeHandle, NULL, "0", nkeyNode, "?", NULL));

					sprintf(firstNode, "0_1");
					EXCCHECKCOM( BKLib2__DBKTreeIsNodeExist(global->treeHandle, NULL, firstNode, &exist));
					if(exist)
						EXCCHECKCOM( BKLib2__DBKTreeMoveNode(global->treeHandle, NULL, nkeyNode, firstNode, TCTV_PREV, NULL));

					EXCCHECKCOM( BKLib2__DBKTreeSetSelectedItem(global->treeHandle, NULL, nkeyNode));
				}
				break;
			case BKTREE_MENU_ROOT_PRODUCT_VIEW:
				EXCCHECK( ChangeMode(EDIT_PRODUCT));
				break;
		}
	}

	else if(level==BKTREE_LEVEL_PROCESS)
	{
		SLineProcess process = {0};

		switch(menuItem)
		{
			case BKTREE_MENU_PROCESS_NEW_BEFORE:
			case BKTREE_MENU_PROCESS_NEW_AFTER:
				{
					char nkeyNode[32];

					sprintf(nkeyNode, "0_%d_new", a + (menuItem==BKTREE_MENU_PROCESS_NEW_AFTER));
					EXCCHECKCOM( BKLib2__DBKTreeAddNode(global->treeHandle, NULL, "0", nkeyNode, "?", NULL));
					EXCCHECKCOM( BKLib2__DBKTreeMoveNode(global->treeHandle, NULL, nkeyNode, keyNode, 
												(menuItem==BKTREE_MENU_PROCESS_NEW_BEFORE)? TCTV_PREV:TCTV_NEXT, NULL));
					EXCCHECKCOM( BKLib2__DBKTreeSetSelectedItem(global->treeHandle, NULL, nkeyNode));
				}
				break;
			case BKTREE_MENU_PROCESS_DEL:
				if(index>0)
				{
					process = PPROCESS->LineProcess[index-1];
				
					if(process.product_id!=global->product_id)
						EXCTHROW(-1, _("Delete process from a definition level of the product!"));

					process.user_id = global->activeUser.user_id;
					EXCCHECK( PPROCESS->LineProcessDel(PPROCESS, process));
				}
				EXCCHECK(TreeProcessIni());
				break;
			case BKTREE_MENU_PROCESS_NEW_STEP:
				{
					
					STestStep test = {0}; 

					process = PPROCESS->LineProcess[index-1];

					test.product_id = global->product_id;
					test.test_nb = 1;
					test.process_nb = process.process_nb;
					gs_process_nb = process.process_nb;
					
					CHECK_RIGHTS_TEST( EXCCHECK( Process_InsertTestDialog(test, sname))); 

				}
				break;
					
			case BKTREE_MENU_PRODUCT_TESTVIEW:
				{
				process = PPROCESS->LineProcess[index-1];
				global->process_nb = process.process_nb;
				sprintf(global->process_name, "%s", process.name);
				EXCCHECK( ChangeMode(EDIT_TEST));
				}
				break;
		}

	}
	
	else if(level== BKTREE_LEVEL_TEST)   //Mode process
	{
	 	STestStep test = {0}; 

		EXCCHECKCOM( BKLib2__DBKTreeGetNodeData (global->treeHandle, NULL, keyNode, &index));

		if(index)
			test = PTEST->Test[--index]; 

		global->test_nb = 0;
		
		switch(menuItem)
		{
			case BKTREE_MENU_TEST_INSERT_BEFORE:
				CHECK_RIGHTS_TEST( EXCCHECK( Process_InsertTestDialog(test, sname))); 
				break;
			case BKTREE_MENU_TEST_INSERT_AFTER:
				stepNumber++;
				test.test_nb++;
				CHECK_RIGHTS_TEST( EXCCHECK( Process_InsertTestDialog(test, sname))); 
				break;
			case BKTREE_MENU_TEST_EDIT:
				CHECK_RIGHTS_TEST( EXCCHECK( Process_EditTestDialog(test, sname))); 
				break;
			case BKTREE_MENU_TEST_SKIP:
				CHECK_RIGHTS_TEST( EXCCHECK( Process_SkipTestDialog(test, editable))); 
				break;
			case BKTREE_MENU_TEST_REMOVE_SKIP:
				CHECK_RIGHTS_TEST( EXCCHECK( Process_RemoveSkipTestDialog(test, editable))); 
				break;
			case BKTREE_MENU_TEST_DEL:
				CHECK_RIGHTS_TEST( EXCCHECK( Process_DelTestDialog(test, editable))); 
				break;
		}
		
	}
	

Error:
	EXCRETHROW( pexception);       
}

/*---------------------------------------------------------------------------*/  
#undef __FUNC__
#define __FUNC__ "BkTreeProcessModify"
SElExceptionPtr BkTreeProcessModify (char *keyNode, char *keyCol, long  modifyType)
{
	SElExceptionPtr pexception = NULL; 
	int32_t			level, a=0, b=0, c=0, index;
	HRESULT			hr = 0;
	char*			ptext = NULL;

	EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemData (global->treeHandle, NULL, keyNode, "root", &index));
	EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemText(global->treeHandle, NULL, keyNode, keyCol, &ptext));

	//level = sscanf(keyNode, "0_%d_%d_%d", &a, &b, &c);
	level = GetLevelBkTree(keyNode, &a, &b); 

	if(level==BKTREE_LEVEL_PROCESS)
	{
		SLineProcess process = {0};

		if(index>0)
		{
			process = PPROCESS->LineProcess[index-1];
			
			if(ptext && *ptext)
			{
				if(0==strcmp(keyCol, "value") || 0==strcmp(keyCol, "desc"))
				{
					strcpy(process.description, ptext);
					process.user_id = global->activeUser.user_id;
					EXCCHECK( PPROCESS->LineProcessEdit(PPROCESS, process));
				}
			}
			else if(0==strcmp(keyCol, "value")) /* empty field, return old value */
			{
				EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemText(global->treeHandle, NULL, keyNode, keyCol, process.description, NULL));
			}
		}
		else /* new process */
		{
			char *pname = NULL, *pdescription = NULL, *nextNode = NULL, *prevNode = NULL;
			bool_t last_node = FALSE;

			EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemText(global->treeHandle, NULL, keyNode, "root", &pname));
			EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemText(global->treeHandle, NULL, keyNode, "Description", &pdescription));

			/* determine actual position - process_nb */
			EXCCHECKCOM( BKLib2__DBKTreeGetNodeNext(global->treeHandle, NULL, keyNode, TCTV_NEXT, &nextNode));
			EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemData (global->treeHandle, NULL, nextNode, "root", &index));

			if(index<0) /* last node */
			{
				EXCCHECKCOM( BKLib2__DBKTreeGetNodeNext(global->treeHandle, NULL, keyNode, TCTV_PREV, &prevNode));
				EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemData (global->treeHandle, NULL, prevNode, "root", &index));
				last_node = TRUE;
			}

			strcpy(process.name, pname);
			strcpy(process.description, pdescription);
			process.product_id = global->product_id;
			process.user_id = global->activeUser.user_id;
			
			if(index>0)
			{
				process.process_nb = PPROCESS->LineProcess[index-1].process_nb + (last_node);
				EXCCHECK( PPROCESS->LineProcessInsert(PPROCESS, process));
			}
			else if(index==-1)
			{
				process.process_nb = PPROCESS->LineProcessSize;
				EXCCHECK( PPROCESS->LineProcessInsert(PPROCESS, process));
			}

			EXCCHECK( TreeProcessIni());
		}
	}

	else if(level==BKTREE_LEVEL_TEST)
	{
		
		
	}


Error:
	CA_FreeMemory(ptext); 
	EXCRETHROW( pexception);       
}

/*---------------------------------------------------------------------------*/  
#undef __FUNC__
#define __FUNC__ "TreeProcessIni"
SElExceptionPtr TreeProcessIni(void)
{
	SElExceptionPtr pexception = NULL;   
	int32_t			error = 0;
	SELStringPtr	pelstring = global->pelstring;
	HRESULT			hr;
	short int		AXvalue, exist, isExpanded;
	int32_t			a=0, b=0, c=0, found, i=1, imglistHandle;
	char			*selectedNode = NULL;
	char			actNode[32];

	EXCCHECK( pelstring->Empty(pelstring));

	EXCCHECKCOM( BKLib2__DBKTreeSetRedraw(global->treeHandle, NULL, 0, NULL));
	EXCCHECKCOM( BKLib2__DBKTreeGetSelectedItem(global->treeHandle, NULL, &selectedNode));
	EXCCHECKCOM( BKLib2__DBKTreeIsExpanded(global->treeHandle, NULL, selectedNode, &isExpanded));

	EXCCHECK( ProcessTreeString(pelstring));

	EXCCHECKCOM( BKLib2__DBKTreeFromString (global->treeHandle, NULL, pelstring->GetPtr(pelstring), "¦", "\n", &AXvalue));
	EXCCHECKCOM( BKLib2__DBKTreeSetEditable(global->treeHandle, NULL, TRUE));
	EXCCHECKCOM( BKLib2__DBKTreeSetEnableDragAndDrop (global->treeHandle, NULL, VFALSE));
	EXCCHECKCOM( BKLib2__DBKTreeSetEnableEventModify (global->treeHandle, NULL, VTRUE));

	EXCCHECKCVI( GetObjHandleFromActiveXCtrl (gs_panel[DBS], DBS_IMAGELIST1, &imglistHandle));
	EXCCHECKCOM( BKLib2__DBKTreeSetImageList (global->treeHandle, NULL, imglistHandle));
	EXCCHECKCOM( BKLib2__DBKTreeSetAutoImages (global->treeHandle, NULL, VFALSE));

	//found = sscanf(selectedNode, "0_%d_%d_%d", &a, &b, &c);
	found = GetLevelBkTree(selectedNode, &a, &b); 

	/* collapse all */
	do
	{
		sprintf(actNode, "0_%d", i++);
		EXCCHECKCOM( BKLib2__DBKTreeIsNodeExist(global->treeHandle, NULL, actNode, &exist));
		EXCCHECKCOM( BKLib2__DBKTreeCollapse (global->treeHandle, NULL, actNode, &AXvalue));
	} while(exist);

	/* select old node */
	if(found>=BKTREE_LEVEL_PROCESS)
	{
		sprintf(actNode, "0_%d", a);
		EXCCHECKCOM( BKLib2__DBKTreeIsNodeExist(global->treeHandle, NULL, actNode, &exist));
		if(exist)
		{
			EXCCHECKCOM( BKLib2__DBKTreeSetSelectedItem(global->treeHandle, NULL, actNode));
			if(found>BKTREE_LEVEL_PROCESS || (found==BKTREE_LEVEL_PROCESS && isExpanded))
				EXCCHECKCOM( BKLib2__DBKTreeExpand(global->treeHandle, NULL, actNode, &AXvalue));
		}
	}
	if(found>=BKTREE_LEVEL_TEST)
	{
		sprintf(actNode, "0_%d_%d", a, b);
		EXCCHECKCOM( BKLib2__DBKTreeIsNodeExist(global->treeHandle, NULL, actNode, &exist));
		if(exist)
		{
			EXCCHECKCOM( BKLib2__DBKTreeSetSelectedItem(global->treeHandle, NULL, actNode));
			if(found>BKTREE_LEVEL_TEST || (found==BKTREE_LEVEL_TEST && isExpanded))
				EXCCHECKCOM( BKLib2__DBKTreeExpand(global->treeHandle, NULL, actNode, &AXvalue));
		}
	}
	
#if 0    /* formerly excluded lines */

	if(found>=BKTREE_LEVEL_PARAMETER)
	{
		sprintf(actNode, "0_%d_%d_%d", a, b, c);
		EXCCHECKCOM( BKLib2__DBKTreeIsNodeExist(global->treeHandle, NULL, actNode, &exist));
		if(exist)
			EXCCHECKCOM( BKLib2__DBKTreeSetSelectedItem(global->treeHandle, NULL, actNode));
	}
#endif   /* formerly excluded lines */

	EXCCHECKCOM( BKLib2__DBKTreeSetRedraw(global->treeHandle, NULL, 1, NULL));
	EXCCHECKCOM( BKLib2__DBKTreeSetScrollBarVisible(global->treeHandle, NULL, 0, 1));
	EXCCHECKCOM( BKLib2__DBKTreeEnsureVisible(global->treeHandle, NULL, selectedNode));

	EXCCHECKCVI( SetCtrlAttribute(gs_panel[DBS], TABPANEL_1_XML_BCK, ATTR_VISIBLE, TRUE));
	EXCCHECKCVI( SetCtrlAttribute(gs_panel[DBS], TABPANEL_1_CSV_OPEN, ATTR_VISIBLE, TRUE));
	EXCCHECKCVI( SetCtrlAttribute(gs_panel[DBS], TABPANEL_1_CSV_FILE, ATTR_VISIBLE, TRUE));
	EXCCHECKCVI( SetCtrlAttribute(gs_panel[DBS], TABPANEL_1_CSV_IMPORT, ATTR_VISIBLE, TRUE));

Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/  
#undef __FUNC__
#define __FUNC__ "TreeProcessClose"
SElExceptionPtr TreeProcessClose(void)
{
	SElExceptionPtr pexception = NULL;   
	int32_t			error = 0;

	EXCCHECKCVI( SetCtrlAttribute(gs_panel[DBS], TABPANEL_1_XML_BCK, ATTR_VISIBLE, FALSE));
	EXCCHECKCVI( SetCtrlAttribute(gs_panel[DBS], TABPANEL_1_CSV_OPEN, ATTR_VISIBLE, FALSE));
	EXCCHECKCVI( SetCtrlAttribute(gs_panel[DBS], TABPANEL_1_CSV_FILE, ATTR_VISIBLE, FALSE));
	EXCCHECKCVI( SetCtrlAttribute(gs_panel[DBS], TABPANEL_1_CSV_IMPORT, ATTR_VISIBLE, FALSE));

Error:
	EXCRETHROW( pexception);
}

#define TREE_STRUCTURE_SIZE		4096
#define TREE_INFO_SIZE			256
#define TREE_PATH_SIZE			256
#define TREE_ROW_SIZE 			2048 + TREE_INFO_SIZE + TREE_PATH_SIZE



/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "ProcessTreeString"
static SElExceptionPtr ProcessTreeString(
	SELStringPtr pelstring
)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL; 
	int				process_index = 1, i, j;
	int32_t			ProductId[TREE_STRUCTURE_SIZE] = {0};
	int32_t			ProductIdSize = TREE_STRUCTURE_SIZE;
	int32_t			editable = 0;
	int32_t			used_process = -1;
	char			path[TREE_PATH_SIZE+1] = "";
	char			info[TREE_INFO_SIZE+1] = "";
	char			row[TREE_ROW_SIZE+1] = "";
	int32_t			skip = 0;
	char*			keyNode;

	EXCCHECK( pelstring->Append(pelstring, BKTREE_HEADER0));
	EXCCHECK( pelstring->Append(pelstring, BKTREE_HEADER1));
	
	if(global->product_id!=0)
	{
		/* select specified product tree branch */
		EXCCHECK( PPRODUCT->ProductGetIdTreeUpDown(
									PPRODUCT,
									global->product_id, 	
									ProductId, 
									&ProductIdSize));
	}
	else
	{
		ProductIdSize = 0;
	}

	/* number of steps */ 
	for(i=0; i<PPROCESS->LineProcessSize; i++)   			
	{
		/* visible only specified tests */
		for(j=ProductIdSize-1; j>=0; j--)   			
		{
			if(ProductId[j] == PPROCESS->LineProcess[i].product_id 
				&& (PPROCESS->LineProcess[i].property&PROPERTY_VALID)>0
				&& (used_process<0 || used_process!=PPROCESS->LineProcess[i].process_nb)) /* unvisible skiped tests */
			{
				/* editable: delete test; skip test*/
				if(ProductId[j]==global->product_id)
					editable |= TREE_EDIT_DEL;
				if(ProductId[j]<global->product_id)
					editable |= TREE_EDIT_SKIP;

				EXCCHECK( PPRODUCT->ProductGetIdPath(PPRODUCT, PPROCESS->LineProcess[i].product_id,path, PRODUCT_PATH_LEVEL_2));		
				EXCCHECK( ProcessTreeInfoString(PPROCESS->LineProcess[i].process_nb, info, TREE_INFO_SIZE) );
				EXCCHECK( ProcessTreeSkip(PPROCESS->LineProcess[i].process_nb, &skip) );
				
				sprintf(row, "0|0_%d|%s%s^r1^i0^c0000%s^d%d¦%d¦¦%s^r0d¦¦%s^r1¦%s^r1\n",   
						process_index, 
						PPROCESS->LineProcess[i].name,
						(skip)? " [skip]":"",
						(editable)? "FF":"00",
						(long)(i+1), 
						editable,	
						PPROCESS->LineProcess[i].description,   
						path,
						info);	
			
	
				EXCCHECK( pelstring->Append(pelstring, row));
				
				if(editable)
				{
					
					/* add steps */ 
					EXCCHECK( TestStepTreeString(pelstring, 
											  	  PPROCESS->LineProcess[i].process_nb, 
												  process_index, 
										          ProductId, 
										          ProductIdSize) ); 
				}
			
				process_index++;
				used_process = PPROCESS->LineProcess[i].process_nb;
				break;
			}
		}
		editable = 0;
	}

Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "TestStepTreeString"
static SElExceptionPtr TestStepTreeString(
	SELStringPtr	pelstring, 
	int32_t 		process_nb, 
	int32_t 		process_index, 
	int32_t* 		ProductId, 
	int32_t 		ProductIdSize
)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL; 
	int				test_index = 1, i, j;
	int32_t			editable = 0;
	int32_t			used_test = 0;
	char			path[256] = "";
	char			info[TREE_INFO_SIZE] = "";
	char			row[TREE_ROW_SIZE] = "";
	int32_t			skip = 0;

	/* number of steps */ 
	for(i=0; i<PTEST->TestSize; i++)   			
	{
		if( PTEST->Test[i].process_nb == process_nb )
		{
		/* visible only specified tests */
		for(j=ProductIdSize-1; j>=0; j--)   			
		{
			/* editable: delete test; skip test*/
			if ( ProductId[j] == global->product_id )
				editable |= TREE_EDIT_DEL;
			if ( ProductId[j] < global->product_id )
				editable |= TREE_EDIT_SKIP;
			
			if ( ProductId[j] == PTEST->Test[i].product_id && (PTEST->Test[i].property&PROPERTY_VALID)>0 )
			{
				/* unvisible skiped tests */
				if( used_test == 0 || used_test != PTEST->Test[i].test_nb ) 
				{
					EXCCHECK( PPRODUCT->ProductGetIdPath(PPRODUCT, PTEST->Test[i].product_id,path, PRODUCT_PATH_LEVEL_3) );
					EXCCHECK( TestTreeInfoString(PTEST->Test[i].test_nb, info, TREE_INFO_SIZE) );
					EXCCHECK( TestTreeSkip(PTEST->Test[i].test_nb, &skip) );
						   	
					sprintf(row, "0_%d|0_%d_%d|Step%d%s^r1^i0^c0000%s^d%d¦%d¦%s¦%s^r0¦¦%s^r1¦%s^r1\n",   
							process_index,																   
							process_index,
							test_index,
							test_index,
							(skip)? " [skip]":"",
							(editable)? "FF":"00",
							(long)(i+1), 
							editable,							
							PTEST->Test[i].name,			
							GET_STRING(PTEST->Test[i].description),   
							path,
							info) ;	
				
					EXCCHECK( pelstring->Append(pelstring, row) );
					
#if 0    /* formerly excluded lines */
					/* add parameters */ 
					EXCCHECK( TestParameterTreeString(pelstring, 
												  PTEST->Test[i].test_nb, 
												  test_index, 
										          ProductId, 
										          ProductIdSize) ); 
#endif   /* formerly excluded lines */
				
					test_index++;
					used_test = PTEST->Test[i].test_nb;
					break;
				}
			}
		}
		editable = 0;
		}
	}

Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/   
#undef __FUNC__
#define __FUNC__ "ProcessTreeInfoString"
static SElExceptionPtr ProcessTreeInfoString(int32_t process_nb, char* info, int32_t infoSize)
{
	SElExceptionPtr pexception = NULL; 
	int32_t			error = 0;
	int				i;
	SLineProcessPtr		pprocess = PPROCESS->LineProcess;
	SProductTypesPtr	pproduct = PPRODUCT->Product;
	int32_t			products_id[1024] = {0};
	int32_t			products_idSize = 0;
	char			path[128]="";
	
	strcpy(info, "");
	
	/* search tests witch equal test_nb, output product_id*/
	for(i=0; i<PPROCESS->LineProcessSize; i++) 
	{
		if(pprocess[i].process_nb == process_nb 
			&& (pprocess[i].property&PROPERTY_VALID)>0 
			&& 0==strcmp(pprocess[i].name, "@skip"))
		{
			products_id[products_idSize++] = pprocess[i].product_id;		
		}
		
		if(pprocess[i].process_nb > process_nb)
			break;
	}
	
	/* determine product_id name and print to info */
	if(products_idSize>0)
	{
		strcat(info, "Skip: "); 
		
		for(i=0; i<products_idSize; i++)
		{
			EXCCHECK( PPRODUCT->ProductGetIdPath(PPRODUCT, products_id[i], path, PRODUCT_PATH_LEVEL_4));
			
			if( (int)(strlen(info) + strlen(path) + strlen("; ...^i3"))>=infoSize)
			{	
				strcat(info, "...");
				break;
			}
			strcat(info, path);
			strcat(info, "; ");
		}
		strcat(info, "^i3");  /* insert picture index */          
	}

Error:
	EXCRETHROW( pexception);     
}

/*---------------------------------------------------------------------------*/  
#undef __FUNC__
#define __FUNC__ "ProcessTreeSkip"
static SElExceptionPtr ProcessTreeSkip(int32_t process_nb, int32_t* skip)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	int				i, j; 
	SLineProcessPtr		pprocess = PPROCESS->LineProcess;
	SProductTypesPtr	pproduct = PPRODUCT->Product;
	int32_t			products_id[1024] = {0};
	int32_t			products_idSize = 0;
	int32_t			path[DBS_NUMBER_OF_GROUPS+1];
	int32_t			pathSize = DBS_NUMBER_OF_GROUPS+1;
	
	*skip = 0;
	
	/* determine product id path */
	EXCCHECK( PPRODUCT->ProductGetIdTreeUp(PPRODUCT, global->product_id, path, &pathSize));    
	
	/* search tests witch equal test_nb, output product_id*/
	for(i=0; i<PPROCESS->LineProcessSize; i++) 
	{
		if(pprocess[i].process_nb == process_nb 
			&& (pprocess[i].property&PROPERTY_VALID)>0
			&& 0==strcmp(pprocess[i].name, "@skip"))
		{
			products_id[products_idSize++] = pprocess[i].product_id;		
		}
		
		if(pprocess[i].process_nb > process_nb)
			break;
	}
	
	/* is test skip? */
	for(i=0; i<products_idSize; i++)
	{
		for(j=0; j<pathSize; j++)
		{
			if(path[j] == products_id[i])
			{
				*skip = 1;
				break;
			}
		}
		
		if(*skip)
			break;
	}
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ParamTreeInfoString"
static SElExceptionPtr ParamTreeInfoString(int32_t step_nb, char* name, char* info, int32_t infoSize)
{
	SElExceptionPtr pexception = NULL; 
	int32_t			error = 0;
	int				i;
	SLineParamPtr		plineparameter = PLINEPARAM->LineParam;
	SProductTypesPtr	pproduct = PPRODUCT->Product;
	int32_t			products_id[1024] = {0};
	int32_t			products_idSize = 0;
	char			path[128]="";
	char*			pvalue[1024] = {""};
	
	strcpy(info, "");
	
	/* search parameters witch equal test_nb, output product_id*/
	for(i=0; i<PLINEPARAM->LineParamSize; i++) 
	{
		if(plineparameter[i].fnc_nb == step_nb 
			&& (plineparameter[i].property&PROPERTY_VALID)>0 
			&& 0==strcmp(plineparameter[i].name, name))
		{
			pvalue[products_idSize] = plineparameter[i].value;
			products_id[products_idSize++] = plineparameter[i].product_id;
		}
		
		if(plineparameter[i].fnc_nb > step_nb)
			break;
	}
	
	/* determine product_id name and print to info */
	if(products_idSize>1)
	{
		strcat(info, "Changed: "); 
		
		for(i=1; i<products_idSize; i++)
		{
			EXCCHECK( PPRODUCT->ProductGetIdPath(PPRODUCT, products_id[i], path, PRODUCT_PATH_LEVEL_4));
			
			if( (int)(strlen(info) + strlen(path) + strlen(pvalue[i]) + strlen(":; ...^i3"))>=infoSize)
			{	
				strcat(info, "...");
				break;
			}
			strcat(info, path);
			strcat(info, ":");
			strcat(info, pvalue[i]);  /* visible parametr value */
			strcat(info, "; ");
		}
		strcat(info, "^i3");   /* insert picture index */           
	}

Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ParamTreeVisible"
static SElExceptionPtr ParamTreeVisible(
	int32_t fnc_nb, 
	char* name, 
	int32_t product_id,
	int32_t* visible
) 
{
	SElExceptionPtr pexception = NULL;
	int				i, j;
	SProductTypesPtr	pproduct = PPRODUCT->Product; 
	SLineParamPtr		plineparam	= PLINEPARAM->LineParam;
	int32_t			path[DBS_NUMBER_OF_GROUPS+1];
	int32_t			pathSize = DBS_NUMBER_OF_GROUPS+1;
	int32_t			products_id[1024] = {0};
	int32_t			products_idSize = 0;
	int32_t			actual_id = 0;
	char*			pvalue[1024];
	
	if(visible) *visible = 0;
	
	/* determine product id path */ 
	EXCCHECK( PPRODUCT->ProductGetIdTreeUp(PPRODUCT, global->product_id, path, &pathSize));

	/* search parameters witch equal test_nb, output product_id*/
	for(i=0; i<PLINEPARAM->LineParamSize; i++) 
	{
		if(plineparam[i].fnc_nb == fnc_nb 
			&& (plineparam[i].property&PROPERTY_VALID)>0 
			&& 0==strcmp(plineparam[i].name, name))
		{
			pvalue[products_idSize] = plineparam[i].value;
			products_id[products_idSize++] = plineparam[i].product_id;
		}
		
		if(plineparam[i].fnc_nb > fnc_nb)
			break;
	}
	
	/* select actual parametr */
	for(i=0; i<pathSize; i++)
	{
		for(j=0; j<products_idSize; j++)
		{
			if(products_id[j] == path[i])
			{
				actual_id = products_id[j];
				break;
			}
		}
		if(actual_id)
			break;
	}
	
	if(actual_id == product_id)
		if(visible) *visible = 1;
	
Error:
	EXCRETHROW( pexception);    
}

/*---------------------------------------------------------------------------*/    
#undef __FUNC__
#define __FUNC__ "TestParameterTreeString"
static SElExceptionPtr TestParameterTreeString(
	SELStringPtr	pelstring, 
	int32_t 		test_nb, 
	int32_t 		test_index, 
	int32_t* 		ProductId, 
	int32_t 		ProductIdSize
)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL; 
	int				parameter_index = 1, p_index = 1, l_index = 1, i, j;
	int32_t			editable = 0;
	char			path[128] = "";
	char			info[TREE_INFO_SIZE] = "";
	char			row[TREE_ROW_SIZE] = "";
	int32_t			visible = 0;
	bool_t			is_limit = FALSE;
	
	/* number of parameters */
	for(i=0; i<PTESTPARAM->TestParameterSize; i++)   				
	{
		if( PTESTPARAM->TestParameter[i].test_nb == test_nb )
		{
			/* visible only specified parameters */ 
			for(j=ProductIdSize-1; j>=0; j--)   			
			{
				/* editable */
				if ( ProductId[j] == global->product_id )
					editable |= TREE_EDIT_PARAMETR;
				if ( ProductId[j] < global->product_id )
					editable |= TREE_EDIT_PARAMETR_INSERT;
				
				if ( ProductId[j] == PTESTPARAM->TestParameter[i].product_id 
						&& (PTESTPARAM->TestParameter[i].property&PROPERTY_VALID)>0 )
				{
			   		/* unvisible duplicate parameters, select actual one */
			   		EXCCHECK( TestTreeParameterVisible(PTESTPARAM->TestParameter[i].test_nb, 
			   								PTESTPARAM->TestParameter[i].name,
			   								PTESTPARAM->TestParameter[i].product_id,
			   								&visible));
			   		
					is_limit = (strncmp(PTESTPARAM->TestParameter[i].name, "limit", strlen("limit")) == 0);
						
			   		if( visible )
			   		{
				   		EXCCHECK( PPRODUCT->ProductGetIdPath(PPRODUCT, PTESTPARAM->TestParameter[i].product_id, path, PRODUCT_PATH_LEVEL_3) );
				   		EXCCHECK( TestTreeParameterInfoString(PTESTPARAM->TestParameter[i].test_nb, 
				   											 PTESTPARAM->TestParameter[i].name, 
				   											 info, TREE_INFO_SIZE) );
			   		
						sprintf(row, "1_%d|1_%d_%d|%s%d^i2^c0000%s^d%d¦%d¦%s¦%s¦%s¦%s¦%s\n",	
								test_index,
								test_index,
								parameter_index,
								(is_limit)? "Limit" : "Parameter",
								(is_limit)? l_index++ : p_index++, 
								(editable)? "FF":"00",
								(long)(i+1),
								editable,							
								PTESTPARAM->TestParameter[i].name,		
								GET_STRING(PTESTPARAM->TestParameter[i].description), 
								PTESTPARAM->TestParameter[i].value,		
								path,
								info);
				
						EXCCHECK( pelstring->Append(pelstring, row) );  
						
						parameter_index++;
					}
				 }
			}
			editable = 0;
		}
	}

Error:
	EXCRETHROW( pexception); 
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "TestTreeParameterVisible"
static SElExceptionPtr TestTreeParameterVisible(
	int32_t test_nb, 
	char* name, 
	int32_t product_id,
	int32_t* visible) 
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	int				i, j;
	
	SProductTypesPtr	pproduct = PPRODUCT->Product; 
	STestParameterPtr	pparameter	= PTESTPARAM->TestParameter;
	
	int32_t		path[DBS_NUMBER_OF_GROUPS+1];
	int32_t		pathSize = DBS_NUMBER_OF_GROUPS+1;
	int32_t		products_id[128] = {0};
	int32_t		products_idSize = 0;
	int32_t		actual_id = 0;
	char*		pvalue[128];
	
	if(visible) *visible = 0;
	
	/* determine product id path */ 
	EXCCHECK( PPRODUCT->ProductGetIdTreeUp(PPRODUCT, global->product_id, path, &pathSize) );

	/* search parameters witch equal test_nb, output product_id*/
	for(i=0; i<PTESTPARAM->TestParameterSize; i++) 
	{
		if(pparameter[i].test_nb == test_nb 
			&& (pparameter[i].property&PROPERTY_VALID)>0 
			&& 0==strcmp(pparameter[i].name, name))
		{
			pvalue[products_idSize] = pparameter[i].value;
			products_id[products_idSize++] = pparameter[i].product_id;
		}
		
		if(pparameter[i].test_nb > test_nb)
			break;
	}
	
	/* select actual parametr */
	for(i=0; i<pathSize; i++)
	{
		for(j=0; j<products_idSize; j++)
		{
			if(products_id[j] == path[i])
			{
				actual_id = products_id[j];
				break;
			}
		}
		if(actual_id)
			break;
	}
	
	if(actual_id == product_id)
		if(visible) *visible = 1;
	
Error:
	EXCRETHROW( pexception);    
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "TestTreeParameterInfoString"
static SElExceptionPtr TestTreeParameterInfoString(int32_t test_nb, char* name, char* info, int32_t infoSize)
{
	SElExceptionPtr pexception = NULL; 
	int32_t			error = 0;
	int				i;
	STestParameterPtr	pparameter = PTESTPARAM->TestParameter;
	SProductTypesPtr	pproduct = PPRODUCT->Product;
	int32_t			products_id[128] = {0};
	int32_t			products_idSize = 0;
	char			path[128]="";
	char*			pvalue[128] = {""};
	
	strcpy(info, "");
	
	/* search parameters witch equal test_nb, output product_id*/
	for(i=0; i<PTESTPARAM->TestParameterSize; i++) 
	{
		if(pparameter[i].test_nb == test_nb 
			&& (pparameter[i].property&PROPERTY_VALID)>0 
			&& 0==strcmp(pparameter[i].name, name))
		{
			pvalue[products_idSize] = pparameter[i].value;
			products_id[products_idSize++] = pparameter[i].product_id;
		}
		
		if(pparameter[i].test_nb > test_nb)
			break;
	}
	
	/* determine product_id name and print to info */
	if(products_idSize>1)
	{
		strcat(info, "Changed: "); 
		
		for(i=1; i<products_idSize; i++)
		{
			EXCCHECK( PPRODUCT->ProductGetIdPath(PPRODUCT, products_id[i], path, PRODUCT_PATH_LEVEL_4));
			
			if((int)(strlen(info) + strlen(path) + strlen(pvalue[i]) + strlen(":; ...^i3"))>=infoSize)
			{	
				strcat(info, "...");
				break;
			}
			strcat(info, path);
			strcat(info, ":");
			strcat(info, pvalue[i]);  /* visible parametr value */
			strcat(info, "; ");
		}
		strcat(info, "^i3");   /* insert picture index */           
	}

Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/   
#undef __FUNC__
#define __FUNC__ "TestTreeInfoString"
static SElExceptionPtr TestTreeInfoString(int32_t test_nb, char* info, int32_t infoSize)
{
	SElExceptionPtr pexception = NULL; 
	int32_t			error = 0;
	int				i;
	STestStepPtr	pstep = PTEST->Test;
	SProductTypesPtr	pproduct = PPRODUCT->Product;
	int32_t			products_id[128] = {0};
	int32_t			products_idSize = 0;
	char			path[128]="";
	
	strcpy(info, "");
	
	/* search tests witch equal test_nb, output product_id*/
	for(i=0; i<PTEST->TestSize; i++) 
	{
		if(pstep[i].test_nb == test_nb 
			&& (pstep[i].property&PROPERTY_VALID)>0 
			&& 0==strcmp(pstep[i].name, "@skip"))
		{
			products_id[products_idSize++] = pstep[i].product_id;		
		}
		
		if(pstep[i].test_nb > test_nb)
			break;
	}
	
	/* determine product_id name and print to info */
	if(products_idSize>0)
	{
		strcat(info, "Skip: "); 
		
		for(i=0; i<products_idSize; i++)
		{
			EXCCHECK( PPRODUCT->ProductGetIdPath(PPRODUCT, products_id[i], path, PRODUCT_PATH_LEVEL_4));
			
			if( (int)(strlen(info) + strlen(path) + strlen("; ...^i3"))>=infoSize)
			{	
				strcat(info, "...");
				break;
			}
			strcat(info, path);
			strcat(info, "; ");
		}
		strcat(info, "^i3");  /* insert picture index */          
	}

Error:
	EXCRETHROW( pexception);     
}

/*---------------------------------------------------------------------------*/  
#undef __FUNC__
#define __FUNC__ "TestTreeSkip"
static SElExceptionPtr TestTreeSkip(int32_t test_nb, int32_t* skip)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	int				i, j; 
	STestStepPtr	pstep = PTEST->Test;
	SProductTypesPtr	pproduct = PPRODUCT->Product;
	int32_t			products_id[128] = {0};
	int32_t			products_idSize = 0;
	int32_t			path[DBS_NUMBER_OF_GROUPS+1];
	int32_t			pathSize = DBS_NUMBER_OF_GROUPS+1;
	
	*skip = 0;
	
	/* determine product id path */
	EXCCHECK( PPRODUCT->ProductGetIdTreeUp(PPRODUCT, global->product_id, path, &pathSize) );    
	
	/* search tests witch equal test_nb, output product_id*/
	for(i=0; i<PTEST->TestSize; i++) 
	{
		if(pstep[i].test_nb == test_nb 
			&& (pstep[i].property&PROPERTY_VALID)>0 
			&& 0==strcmp(pstep[i].name, "@skip"))
		{
			products_id[products_idSize++] = pstep[i].product_id;		
		}
		
		if(pstep[i].test_nb > test_nb)
			break;
	}
	
	/* is test skip? */
	for(i=0; i<products_idSize; i++)
	{
		for(j=0; j<pathSize; j++)
		{
			if(path[j] == products_id[i])
			{
				*skip = 1;
				break;
			}
		}
		
		if(*skip)
			break;
	}
	

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "TranslateBKTreeBinCode"
static int TranslateBKTreeBinCode (char *buffer)
{
    // Conversion table for "non-printable" characters used in BKTree
    char    *BinaryCodesTbl[] = {"~#01~", "~#02~", "~#03~", "~#04~", "~#05~",
                                 "~#06~", "~#07~", "~#08~", "~#09~", "~#10~",
                                 "~#11~", "~#12~", "~#13~", 0};

    short   idx = 0;
    char    *p2code;

    // Search and replace BKTree binary character tags
    if (buffer)
    {
        for (idx = 0; BinaryCodesTbl[idx]; idx++)
            for (;p2code = strstr (buffer, BinaryCodesTbl[idx]);)
            {
                p2code[0] = idx + 1; // Set bin code
                memmove (p2code + 1, p2code + strlen(BinaryCodesTbl[idx]), strlen(p2code));
            }
    }

    return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Process_InsertTestDialog"
static SElExceptionPtr Process_InsertTestDialog(STestStep test, char* sname)
{
	SElExceptionPtr pexception = NULL; 
	int32_t      error = 0;
	int32_t      top = 100, left = 30; 

	/* ini global variable */
	gs_product_id = test.product_id;
	gs_test_nb = test.test_nb;
 	gs_process_nb = test.process_nb;

	EXCCHECKCVI( SetCtrlVal(gs_panel[TEST], TEST_STEP, sname));
	EXCCHECKCVI( SetCtrlVal(gs_panel[TEST], TEST_NAME, ""));
	EXCCHECKCVI( SetCtrlVal(gs_panel[TEST], TEST_DESCRIPT, ""));

	EXCCHECKCVI( SetPanelAttribute (gs_panel[TEST], ATTR_TITLE, _("Insert New Test")));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[TEST], TEST_EXIT_OK, ATTR_CALLBACK_FUNCTION_POINTER, OnExitInsert));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[TEST], TEST_EXIT_CANCEL, ATTR_CALLBACK_FUNCTION_POINTER, OnExitInsert));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[TEST], TEST_STEP, ATTR_LABEL_TEXT, _("Step")));   
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[TEST], TEST_NAME, ATTR_NO_EDIT_TEXT, 0));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[TEST], TEST_NAME, ATTR_TEXT_BGCOLOR, 0x00FFFFFF)); 

	EXCCHECKCVI( SetPanelPos (gs_panel[TEST], top, left));  
	EXCCHECKCVI( InstallPopup(gs_panel[TEST]));

Error:
   EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Process_EditTestDialog"
static SElExceptionPtr Process_EditTestDialog(STestStep test, char* sname)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	int32_t			top = 100, left = 30; 

	/* ini global variable */
	gs_product_id = test.product_id;
	gs_test_nb = test.test_nb;
	gs_process_nb = test.process_nb;

	EXCCHECKCVI( SetCtrlVal(gs_panel[TEST], TEST_STEP, sname));
	EXCCHECKCVI( SetCtrlVal(gs_panel[TEST], TEST_NAME, test.name));
	EXCCHECKCVI( SetCtrlVal(gs_panel[TEST], TEST_DESCRIPT, GET_STRING(test.description)));

	EXCCHECKCVI( SetPanelAttribute (gs_panel[TEST], ATTR_TITLE, _("Edit Current Test")));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[TEST], TEST_EXIT_OK, ATTR_CALLBACK_FUNCTION_POINTER, OnExitEdit));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[TEST], TEST_EXIT_CANCEL, ATTR_CALLBACK_FUNCTION_POINTER, OnExitEdit));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[TEST], TEST_STEP, ATTR_LABEL_TEXT, _("Step")));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[TEST], TEST_NAME, ATTR_NO_EDIT_TEXT, 0)); 
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[TEST], TEST_NAME, ATTR_TEXT_BGCOLOR, 0x00FFFFFF)); 

	EXCCHECKCVI( SetPanelPos (gs_panel[TEST], top, left));    
	EXCCHECKCVI( InstallPopup(gs_panel[TEST]));

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Process_SkipTestDialog"
static SElExceptionPtr Process_SkipTestDialog(STestStep	test, int32_t editable)
{
	SElExceptionPtr	pexception = NULL;

	if(editable & TREE_EDIT_DEL)
	{
		if(IDOK==MessageBox((HWND)hWnd, _("Skip selected test?"), _("Skip Dialog"), MB_OKCANCEL|MB_ICONQUESTION))
		{
			test.product_id = global->product_id;
			test.user_id = global->activeUser.user_id;
			EXCCHECK( PTEST->TestSkip(PTEST, test));
			EXCCHECK( TreeProcessIni());
		}
	}

Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Process_RemoveSkipTestDialog"
static SElExceptionPtr Process_RemoveSkipTestDialog(STestStep test, int32_t editable)
{
	SElExceptionPtr pexception = NULL;

	if(IDOK==MessageBox((HWND)hWnd, _("Remove skip for selected test?"), _("Remove Skip Dialog"), MB_OKCANCEL|MB_ICONQUESTION))
	{
		test.product_id = global->product_id;
		test.user_id = global->activeUser.user_id;
		EXCCHECK( PTEST->TestRemoveSkip(PTEST, test) );
		EXCCHECK( TreeProcessIni()); 
	}

Error:
	EXCRETHROW( pexception);      
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Process_DelTestDialog"
static SElExceptionPtr Process_DelTestDialog(STestStep	test, int32_t editable)
{
	SElExceptionPtr	pexception = NULL;

	if(editable & TREE_EDIT_DEL)
	{
		if(editable & TREE_EDIT_SKIP)
		{
			EXCTHROW(-1, _("Delete test from the oreginal level!"));
		}
		else
		{
			if(IDOK==MessageBox((HWND)hWnd, _("Delete selected test?\n - all skips will be removed\n - child parameters will be removed"), _("Delete Dialog"), MB_OKCANCEL|MB_ICONQUESTION))
			{
				test.user_id = global->activeUser.user_id;
				EXCCHECK( PTEST->TestDel(PTEST, test));
				EXCCHECK( TreeProcessIni()); 
			}
		}
	}

Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::OnExitInsert"
static int CVICALLBACK OnExitInsert (int panel, int control, int event,
      void *callbackData, int eventData1, int eventData2)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL;
	STestStep		test = {0};
	double			timer;
   
	switch (event)
	{
		case EVENT_COMMIT:
			if(control == TEST_EXIT_OK)
			{
				TIMER_START;
	                  
				test.product_id = global->product_id;
				test.test_nb = gs_test_nb;
				test.process_nb = gs_process_nb;
				
				EXCCHECKCVI( GetCtrlVal(gs_panel[TEST], TEST_NAME, test.name));
				CHECK_VALUE( test.name);

				EXCCHECKCVI( GetCtrlVal(gs_panel[TEST], TEST_DESCRIPT, PDBS_ALLOC(&test.description, DBS_RECORD_LENGHT_DESCRIPTION)));
				test.user_id = global->activeUser.user_id;
	            
				EXCCHECK( PTEST->TestInsert(PTEST, test));
				EXCCHECK( TreeProcessIni());
				TIMER_STOP(timer);
			}
			EXCCHECKCVI( RemovePopup(gs_panel[TEST]));  
			break;
	}

Error:
	EXCDISPLAY( pexception);
	return 0;
}


/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::OnExitEdit"
static int CVICALLBACK OnExitEdit (int panel, int control, int event,
      void *callbackData, int eventData1, int eventData2)
{
   int32_t      	error = 0;
   SElExceptionPtr 	pexception = NULL;
   STestStep    	test = {0};
   double			timer;   

   switch (event)
      {
      case EVENT_COMMIT:
         if(control == TEST_EXIT_OK)
         {
            TIMER_START;
            
			test.product_id = gs_product_id;
            test.test_nb = gs_test_nb;
       		test.process_nb = gs_process_nb;

            EXCCHECKCVI( GetCtrlVal(gs_panel[TEST], TEST_NAME, test.name));
            CHECK_VALUE( test.name);

            EXCCHECKCVI( GetCtrlVal(gs_panel[TEST], TEST_DESCRIPT, PDBS_ALLOC(&test.description, DBS_RECORD_LENGHT_DESCRIPTION)));
            test.user_id = global->activeUser.user_id;
            
            EXCCHECK( PTEST->TestEdit(PTEST, test));
            EXCCHECK( TreeProcessIni());
            TIMER_STOP(timer);
         }
         EXCCHECKCVI( RemovePopup(gs_panel[TEST]));  
         break;
      }

Error:
   EXCDISPLAY( pexception); 
   return 0;
}
