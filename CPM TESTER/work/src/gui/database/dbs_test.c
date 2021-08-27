#include <bklib/BKLib2.h> 
#include "dbs_global.h"
#include <stdio.h>
#include "dbs_test.h"
#include "dbs_product.h"
#include "dbs_test_param.h"
#include "dbs_test_edit.h"
#include "ring_control.h"

#define BKTREE_HEADER0 _("root|Step^w200¦edit|E^w40¦nom|Test/paramètre^w250¦desc|Description^w250¦valeur|valeur^w250¦chemin|Chemin^w200¦info|I^w22\n")
#define BKTREE_HEADER1 "|1|Steps^i0^r0\n"

#define BKTREE_MENU_ROOT _("Nouveau Test|Liste des Process   >")
#define BKTREE_MENU_TEST _("Nouveau paramètre|Nouveau Test avant|Nouveau Test aprés|Editer Test|Skipper Test|Annuler Skip|Effacer Test|Déplacer Test|Paramètres 	>|Process 	>")
#define BKTREE_MENU_PARAMETER _("Nouveau paramètre |Editer Paramètre |Effacer Paramètre |Paramètres De Test   >")

/* colors */
#define BKTREE_COLOR_SELECT				0x0000FF
#define BKTREE_COLOR_NOT_SELECT			0xFF0000 

/* menu depth */
typedef enum _EBkTreeLevel
{
	BKTREE_LEVEL_ROOT = -1,
	BKTREE_LEVEL_NONE,
	BKTREE_LEVEL_TEST,
	BKTREE_LEVEL_PARAMETER
} EBkTreeLevel;

/* test menu */
typedef enum _EBkTreeMenuTest
{
	BKTREE_MENU_TEST_NEW_LIMIT = 0,
	BKTREE_MENU_TEST_INSERT_BEFORE,
	BKTREE_MENU_TEST_INSERT_AFTER,
	BKTREE_MENU_TEST_EDIT,
	BKTREE_MENU_TEST_SKIP,
	BKTREE_MENU_TEST_REMOVE_SKIP,
	BKTREE_MENU_TEST_DEL,
	BKTREE_MENU_TEST_MOVE,
	BKTREE_MENU_TEST_PARAMETERVIEW,
	BKTREE_MENU_TEST_PROCESSVIEW
} EBkTreeMenuTest;

/* parameter menu */ 
typedef enum _EBkTreeMenuParameter
{
	BKTREE_MENU_PARAMETR_NEW = 0,
	BKTREE_MENU_PARAMETR_EDIT,
	BKTREE_MENU_PARAMETR_DEL,
	BKTREE_MENU_PARAMETR_VIEW
} EBkTreeMenuParameter;

/* root menu */
typedef enum _EBkTreeMenuRoot
{
	BKTREE_MENU_ROOT_NEW_TEST = 0,
	BKTREE_MENU_ROOT_PROCESSVIEW
} EBkTreeMenuRoot;

extern SGlobalPtr		global;
extern int				gs_panel[];

struct _STestMove
{
	bool_t		active;
	char		keyNode[32];
	int32_t		pos;
	STestStep	test;
	uint32_t	last_mode;
} gs_STestMove;

static SElExceptionPtr TestStepTreeString(void);
static SElExceptionPtr TestParameterTreeString(
			SELStringPtr pelstring,
			int32_t test_nb, 
			int32_t test_index,
			int32_t* ProductId, 
			int32_t ProductIdSize); 
static SElExceptionPtr TestTreeInfoString(int32_t test_nb, char* info, int32_t infoSize);
static SElExceptionPtr TestTreeParameterInfoString(int32_t test_nb, char* name, char* info, int32_t infoSize);
static SElExceptionPtr TestTreeParameterVisible(
			int32_t test_nb, 
			char* name,
			int32_t	product_id,
			int32_t* visible);
static SElExceptionPtr TestTreeSkip(int32_t test_nb, int32_t* skip);
static SElExceptionPtr TreeGetUpperLevelString(const char* keyNode, char* upperlevel);
static SElExceptionPtr TreeSetSelectedItem(char* keyNode);
static SElExceptionPtr GetSelectTestNb(char* keyNode);
static SElExceptionPtr MoveTestModeExit(void);

/*---------------------------------------------------------------------------*/  
static int GetLevelBkTree(char *keyNode, int *a, int *b)
{
int	level = 0;
	 
*a = 0; 
*b = 0;
	
	sscanf(keyNode, "1_%d_%d", a, b);
	
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
#define __FUNC__ "TreeStepsIni"
SElExceptionPtr TreeStepsIni(void)
{
	SElExceptionPtr pexception = NULL;   
	int32_t			error = 0;
	HRESULT			hr;
	short			AXvalue, isExpanded, exist;
	int				imglistHandle, found, a, b, i=1;
	char			actNode[32];
	
	EXCCHECKCOM( BKLib2__DBKTreeSetRedraw(global->treeHandle, NULL, 0, NULL));
	EXCCHECKCOM( BKLib2__DBKTreeIsExpanded(global->treeHandle, NULL, global->keyNode[EDIT_TEST], &isExpanded));

	EXCCHECK( TestStepTreeString());
	
	EXCCHECKCOM( BKLib2__DBKTreeFromString (global->treeHandle, NULL, (global->pelstring)->GetPtr(global->pelstring), "¦", "\n", &AXvalue));
	EXCCHECKCOM( BKLib2__DBKTreeSetEditable(global->treeHandle, NULL, FALSE));
	EXCCHECKCOM( BKLib2__DBKTreeSetEnableDragAndDrop (global->treeHandle, NULL, VFALSE));

	EXCCHECKCVI( GetObjHandleFromActiveXCtrl (gs_panel[DBS], DBS_IMAGELIST1, &imglistHandle));
	EXCCHECKCOM( BKLib2__DBKTreeSetImageList (global->treeHandle, NULL, imglistHandle));
	EXCCHECKCOM( BKLib2__DBKTreeSetAutoImages (global->treeHandle, NULL, VFALSE));
	 
	//found = sscanf(global->keyNode[EDIT_TEST], "1_%d_%d", &a, &b); //ARA
	found = GetLevelBkTree(global->keyNode[EDIT_TEST], &a, &b); 
	/* collapse all */
	do
	{
		sprintf(actNode, "1_%d", i++);
		EXCCHECKCOM( BKLib2__DBKTreeIsNodeExist(global->treeHandle, NULL, actNode, &exist));
		EXCCHECKCOM( BKLib2__DBKTreeCollapse (global->treeHandle, NULL, actNode, &AXvalue));
	} while(exist);

	/* select old node */
	if(found>=BKTREE_LEVEL_TEST)
	{
		sprintf(actNode, "1_%d", a);
		EXCCHECKCOM( BKLib2__DBKTreeIsNodeExist(global->treeHandle, NULL, actNode, &exist));
		if(exist)
		{
			EXCCHECKCOM( BKLib2__DBKTreeSetSelectedItem(global->treeHandle, NULL, actNode));
			if(found>BKTREE_LEVEL_TEST || (found==BKTREE_LEVEL_TEST && isExpanded))
				EXCCHECKCOM( BKLib2__DBKTreeExpand(global->treeHandle, NULL, actNode, &AXvalue));
		}
	}
	if(found>=BKTREE_LEVEL_PARAMETER)
	{
		sprintf(actNode, "1_%d_%d", a, b);
		EXCCHECKCOM( BKLib2__DBKTreeIsNodeExist(global->treeHandle, NULL, actNode, &exist));
		if(exist)
			EXCCHECKCOM( BKLib2__DBKTreeSetSelectedItem(global->treeHandle, NULL, actNode));
	}

	EXCCHECKCOM( BKLib2__DBKTreeSetRedraw(global->treeHandle, NULL, 1, NULL));
	EXCCHECKCOM( BKLib2__DBKTreeSetScrollBarVisible(global->treeHandle, NULL, 0, 1));
	EXCCHECKCOM( BKLib2__DBKTreeEnsureVisible(global->treeHandle, NULL, global->keyNode[EDIT_TEST]));

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/  
#undef __FUNC__
#define __FUNC__ "TreeStepsClose"
SElExceptionPtr TreeStepsClose(void)
{
	SElExceptionPtr pexception = NULL;   

	EXCCHECK( MoveTestModeExit());
	
Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/  
#undef __FUNC__
#define __FUNC__ "BkTreeStepsNodeClick"
SElExceptionPtr BkTreeStepsNodeClick (char *keyNode)
{
	SElExceptionPtr pexception = NULL; 
	HRESULT			hr;
	short int		AXvalue;
	char*			pedit = NULL;
	int32_t 		editable = 0;
	int32_t			a,b,level;

	/* set active name */
	strcpy(global->keyNode[EDIT_TEST], keyNode);
	
	level = GetLevelBkTree (keyNode, &a, &b);

	if(gs_STestMove.active)
	{
		EXCCHECKCOM( BKLib2__DBKTreeSetPopupMenu (global->treeHandle, NULL, NULL, &AXvalue));
		EXCCHECKCOM( BKLib2__DBKTreeSetSelectedItem(global->treeHandle, NULL, gs_STestMove.keyNode));
		EXCCHECKCOM( BKLib2__DBKTreeEnsureVisible(global->treeHandle, NULL, gs_STestMove.keyNode));
	}
	
	else if(level==BKTREE_LEVEL_ROOT)
	{
		EXCCHECKCOM( BKLib2__DBKTreeSetPopupMenu(global->treeHandle, NULL, BKTREE_MENU_ROOT, &AXvalue));
	}

	else
	{
		EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemText (global->treeHandle, NULL, keyNode, "edit", &pedit)); 
		sscanf(pedit, "%d", &editable);
		if(editable)
		{
			if(level==BKTREE_LEVEL_TEST)
				EXCCHECKCOM( BKLib2__DBKTreeSetPopupMenu( global->treeHandle, NULL, BKTREE_MENU_TEST, &AXvalue));
			else if(level==BKTREE_LEVEL_PARAMETER)
				EXCCHECKCOM( BKLib2__DBKTreeSetPopupMenu( global->treeHandle, NULL, BKTREE_MENU_PARAMETER, &AXvalue));
		}
		else
		{
			EXCCHECKCOM( BKLib2__DBKTreeSetPopupMenu (global->treeHandle, NULL, NULL, &AXvalue));
		}
	}

Error:
	CA_FreeMemory(pedit); 
	EXCRETHROW( pexception);       
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "MoveTestMode"
SElExceptionPtr MoveTestMode(STestStep test, int32_t editable, const char* keyNode)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	HRESULT			hr;
	static bool_t	help = FALSE;
	
	if(editable)
	{
		if(editable & TREE_EDIT_SKIP)
			EXCTHROW(-1, _("Move test from the oreginal level!"));

		if(!help)
		{
			MessageBox((HWND)hWnd, _("Move mode help:\n\n [Arrow Up] - move test up\n [Arrow Down] - move test down\n [Esc] - escape without exit\n [Enter] - save changes"), 
					_("Move Dialog"), MB_OK|MB_ICONQUESTION);
			help = TRUE;
		}
		
		gs_STestMove.active = TRUE;
		strcpy(gs_STestMove.keyNode, keyNode);
		gs_STestMove.pos = 0;
		gs_STestMove.test = test;
		gs_STestMove.last_mode = 0;

		EXCCHECK( RingGroupAttribute(RING_GROUPS_LOCK));

		EXCCHECKCOM( BKLib2__DBKTreeSetBackTextColor(global->treeHandle, NULL, 0x00e7c3bd));
		EXCCHECKCVI( SetActiveCtrl(gs_panel[DBS], DBS_BKTREE));
	}

Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SaveTestPosition"
static SElExceptionPtr SaveTestPosition(void)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;

	if(gs_STestMove.pos)
	{
		HRESULT hr;
		long index;
		char keyNodeTrg[32];
		int32_t a;
		short exist;

		sscanf(gs_STestMove.keyNode, "1_%d", &a);
		sprintf(keyNodeTrg, "1_%d", a + gs_STestMove.pos);

		EXCCHECKCOM( BKLib2__DBKTreeIsNodeExist(global->treeHandle, NULL, keyNodeTrg, &exist));
		if(exist)
		{
			EXCCHECKCOM( BKLib2__DBKTreeGetNodeData (global->treeHandle, NULL, keyNodeTrg, &index));

			if(index)
			{
				STestStep testTrg = {0};
				
				testTrg = PTEST->Test[--index];
				gs_STestMove.test.user_id = global->activeUser.user_id;

				EXCCHECK(PTEST->TestMove(PTEST, gs_STestMove.test, (gs_STestMove.last_mode==TCTV_PREV)? TSM_PREV:TSM_NEXT, testTrg.test_nb));
			}
		}
	}

Error: 
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "MoveTestModeExit"
static SElExceptionPtr MoveTestModeExit(void)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;

	if(gs_STestMove.active)
	{
		char keyNode[32];
		HRESULT hr;
		int32_t a; 

		EXCCHECK( TreeStepsIni());
		EXCCHECKCVI( SetActiveCtrl(gs_panel[DBS], DBS_BKTREE));

		sscanf(gs_STestMove.keyNode, "1_%d", &a);
		
		if(gs_STestMove.pos<0 && gs_STestMove.last_mode==TCTV_NEXT)
			sprintf(keyNode, "1_%d", a+gs_STestMove.pos+1);
		else if(gs_STestMove.pos>0 && gs_STestMove.last_mode==TCTV_PREV)
			sprintf(keyNode, "1_%d", a+gs_STestMove.pos-1);
		else
			sprintf(keyNode, "1_%d", a+gs_STestMove.pos);

		EXCCHECKCOM( BKLib2__DBKTreeSetSelectedItem(global->treeHandle, NULL, keyNode));
		EXCCHECKCOM( BKLib2__DBKTreeEnsureVisible(global->treeHandle, NULL, keyNode));

		gs_STestMove.active = FALSE;
		strcpy(gs_STestMove.keyNode, "");
		gs_STestMove.pos = 0;

		EXCCHECK( RingGroupAttribute(RING_GROUPS_UNLOCK));
	}

Error: 
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "BkTreeStepMove"
SElExceptionPtr BkTreeStepMove(int32_t key_id, bool_t *activated)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;

	if(gs_STestMove.active)
	{
		HRESULT hr;
		int32_t level, a, b; 
		char keyNodeTrg[32];
		short exist;

		//level = sscanf(gs_STestMove.keyNode, "1_%d", &a);  //ARA
		level = GetLevelBkTree(gs_STestMove.keyNode, &a, &b);
		

		switch(key_id)
		{
			case VAL_UP_ARROW_VKEY:
				gs_STestMove.pos--;

				if(gs_STestMove.pos==0)
					sprintf(keyNodeTrg, "1_%d", a+1);
				else
					sprintf(keyNodeTrg, "1_%d", a+((gs_STestMove.last_mode==TCTV_NEXT)? ++gs_STestMove.pos:gs_STestMove.pos));

				EXCCHECKCOM( BKLib2__DBKTreeIsNodeExist(global->treeHandle, NULL, keyNodeTrg, &exist));
				if(exist)
				{
					EXCCHECKCOM( BKLib2__DBKTreeMoveNode(global->treeHandle, NULL, gs_STestMove.keyNode, keyNodeTrg, TCTV_PREV, NULL));
					gs_STestMove.last_mode = TCTV_PREV;
					EXCCHECKCOM( BKLib2__DBKTreeSetSelectedItem(global->treeHandle, NULL, gs_STestMove.keyNode));
					EXCCHECKCOM( BKLib2__DBKTreeEnsureVisible(global->treeHandle, NULL, gs_STestMove.keyNode));
				}
				else
					gs_STestMove.pos++;

				if(activated) *activated = TRUE;

				break;
			case VAL_DOWN_ARROW_VKEY:
				gs_STestMove.pos++;

				if(gs_STestMove.pos==0)
					sprintf(keyNodeTrg, "1_%d", a-1);
				else
					sprintf(keyNodeTrg, "1_%d", a+((gs_STestMove.last_mode==TCTV_PREV)? --gs_STestMove.pos:gs_STestMove.pos));

				EXCCHECKCOM( BKLib2__DBKTreeIsNodeExist(global->treeHandle, NULL, keyNodeTrg, &exist));
				if(exist)
				{
					EXCCHECKCOM( BKLib2__DBKTreeMoveNode(global->treeHandle, NULL, gs_STestMove.keyNode, keyNodeTrg, TCTV_NEXT, NULL));
					gs_STestMove.last_mode = TCTV_NEXT;
					EXCCHECKCOM( BKLib2__DBKTreeSetSelectedItem(global->treeHandle, NULL, gs_STestMove.keyNode));
					EXCCHECKCOM( BKLib2__DBKTreeEnsureVisible(global->treeHandle, NULL, gs_STestMove.keyNode));
				}
				else
					gs_STestMove.pos--;

				if(activated) *activated = TRUE;

				break;
			case VAL_ENTER_VKEY:
				EXCCHECK( SaveTestPosition());
			case VAL_ESC_VKEY:
				EXCCHECK( MoveTestModeExit());
				break;
		}
	}

Error: 
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "BkTreeStepsPopupMenuCommand"
SElExceptionPtr BkTreeStepsPopupMenuCommand (char *keyNode,char *keyCol,long  menuItem)
{
	SElExceptionPtr pexception = NULL;  
	int32_t			error = 0;
	HRESULT			hr;
	char*			sname;
	char*			sedit;
	long 			index = 0;
	int32_t			stepNumber = -1;
	int32_t			parameterNumber = -1; 
	int32_t			level, a, b; 
	int32_t			editable;
	
	EXCCHECKCOM( BKLib2__DBKTreeGetNodeData (global->treeHandle, NULL, keyNode, &index));
	EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemText (global->treeHandle, NULL, keyNode, "root", &sname));
	EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemText (global->treeHandle, NULL, keyNode, "edit", &sedit));
	
	sscanf(sedit, "%d", &editable);
	//level = sscanf(keyNode, "1_%d_%d", &a, &b); 
	level = GetLevelBkTree(keyNode, &a, &b); 

 	if(level==BKTREE_LEVEL_TEST) /* test menu */ 
	{	
		STestStep test = {0}; 

		if(index)
			test = PTEST->Test[--index]; 

		global->test_nb = 0;
		
		switch(menuItem)
		{
			case BKTREE_MENU_TEST_NEW_LIMIT:
				CHECK_RIGHTS_PARAMETER( EXCCHECK( NewParameterDialog(test, sname))); 
				break;
			case BKTREE_MENU_TEST_INSERT_BEFORE:
				CHECK_RIGHTS_TEST( EXCCHECK( InsertTestDialog(test, sname))); 
				break;
			case BKTREE_MENU_TEST_INSERT_AFTER:
				stepNumber++;
				test.test_nb++;
				CHECK_RIGHTS_TEST( EXCCHECK( InsertTestDialog(test, sname))); 
				break;
			case BKTREE_MENU_TEST_EDIT:
				CHECK_RIGHTS_TEST( EXCCHECK( EditTestDialog(test, sname))); 
				break;
			case BKTREE_MENU_TEST_SKIP:
				CHECK_RIGHTS_TEST( EXCCHECK( SkipTestDialog(test, editable))); 
				break;
			case BKTREE_MENU_TEST_REMOVE_SKIP:
				CHECK_RIGHTS_TEST( EXCCHECK( RemoveSkipTestDialog(test, editable))); 
				break;
			case BKTREE_MENU_TEST_DEL:
				CHECK_RIGHTS_TEST( EXCCHECK( DelTestDialog(test, editable))); 
				break;
			case BKTREE_MENU_TEST_MOVE:
				CHECK_RIGHTS_TEST( EXCCHECK( MoveTestMode(test, editable, keyNode))); 
				break;
			case BKTREE_MENU_TEST_PARAMETERVIEW:
				EXCCHECK( GetSelectTestNb(keyNode));      
				EXCCHECK( ChangeMode(EDIT_TEST_PARAM));
				break;
			case BKTREE_MENU_TEST_PROCESSVIEW:
				EXCCHECK( ChangeMode(EDIT_LINE ));
				break;
		}
	}

	else if(level==BKTREE_LEVEL_PARAMETER) /* parametr menu */ 
	{
		STestParameter parameter = {0}; 
		
		if(index)
			parameter = PTESTPARAM->TestParameter[--index];

		global->test_nb = parameter.test_nb; 

		switch(menuItem)
		{
			case BKTREE_MENU_PARAMETR_NEW:
				{
					STestStep test = {0}; 

					test.product_id = global->product_id;
					test.test_nb = global->test_nb;

					CHECK_RIGHTS_PARAMETER( EXCCHECK( NewParameterDialog(test, sname))); 
				}
				break;
			case BKTREE_MENU_PARAMETR_EDIT:
				CHECK_RIGHTS_PARAMETER( EXCCHECK( EditParameterDialog(parameter, sname, editable))); 
				break;
			case BKTREE_MENU_PARAMETR_DEL:
				CHECK_RIGHTS_PARAMETER( EXCCHECK( DelParameterDialog(parameter, editable))); 
				break;
			case BKTREE_MENU_PARAMETR_VIEW:
				EXCCHECK( ChangeMode(EDIT_TEST_PARAM));
				break;
		}   
	}

	else if(level==BKTREE_LEVEL_ROOT) /* product menu */ 
	{
		STestStep test = {0}; 

		test.product_id = global->product_id;
		test.test_nb = 1;

		switch(menuItem)
		{	
			case BKTREE_MENU_ROOT_NEW_TEST:
				CHECK_RIGHTS_TEST( EXCCHECK( InsertTestDialog(test, "Step1"))); 
				break;
			case BKTREE_MENU_ROOT_PROCESSVIEW:
				EXCCHECK( ChangeMode(EDIT_LINE)); 
				break;
		}
	}

Error:
	CA_FreeMemory( sname );
	CA_FreeMemory( sedit );
	EXCRETHROW( pexception);   
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "BkTreeStepsInsert"
SElExceptionPtr BkTreeStepsInsert(void)
{
	SElExceptionPtr pexception = NULL; 
	int32_t			error = 0;
	HRESULT			hr;
	char*			keyNode;
	int32_t			level, a, b;  
	BKLib2Type_OLE_COLOR color;
	
	EXCCHECKCOM( BKLib2__DBKTreeGetSelectedItem(global->treeHandle, NULL, &keyNode));
	
	//level = sscanf(keyNode, "1_%d_%d", &a, &b);
	level = GetLevelBkTree(keyNode, &a, &b);
	
	/* test menu */ 
	if(level==BKTREE_LEVEL_TEST) 
	{
		char	keyNext[32];
		short	exist;
		int		val1, val2;
		
		EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemTextColor (global->treeHandle, NULL, keyNode, "root", &color));
		if(color==BKTREE_COLOR_NOT_SELECT)
			EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemTextColor (global->treeHandle, NULL, keyNode, "root", BKTREE_COLOR_SELECT));  	
		else if(color==BKTREE_COLOR_SELECT)
			EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemTextColor (global->treeHandle, NULL, keyNode, "root", BKTREE_COLOR_NOT_SELECT));  	
		
		sscanf(keyNode, "%d_%d", &val1, &val2);
		sprintf(keyNext, "%d_%d", val1, val2+1); 
		EXCCHECKCOM( BKLib2__DBKTreeIsNodeExist (global->treeHandle, NULL, keyNext, &exist));
		
		if(exist)
			EXCCHECKCOM( BKLib2__DBKTreeSetSelectedItem (global->treeHandle, NULL, keyNext));
	}
	
Error:
	CA_FreeMemory( keyNode ); 
	EXCRETHROW( pexception); 
}

#define TREE_STRUCTURE_SIZE		4096
#define TREE_INFO_SIZE			256
#define TREE_ROW_SIZE 			1024 + TREE_INFO_SIZE
/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "TestStepTreeString"
static SElExceptionPtr TestStepTreeString(void)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL; 
	int				test_index = 1, i, j;
	int32_t			ProductId[TREE_STRUCTURE_SIZE] = {0};
	int32_t			ProductIdSize = TREE_STRUCTURE_SIZE;
	int32_t			editable = 0;
	int32_t			used_test = 0;
	char			path[256] = "";
	char			info[TREE_INFO_SIZE] = "";
	char			row[TREE_ROW_SIZE] = "";
	int32_t			skip = 0;
	SELStringPtr	pelstring = global->pelstring;
	
	EXCCHECK( pelstring->ReserveSpace(pelstring, 20000) );
	EXCCHECK( pelstring->Empty(pelstring) );
	EXCCHECK( pelstring->Append(pelstring, BKTREE_HEADER0) );
	
	sprintf(row, "|1|%s^i0^r0\n",  global->process_name) ;	 //name of process
					
	EXCCHECK( pelstring->Append(pelstring, row) ); 
	
	if(global->product_id != 0)
	{
		/* select specified product tree branch */
		EXCCHECK( PPRODUCT->ProductGetIdTreeUpDown(
									PPRODUCT,
									global->product_id, 	
									ProductId, 
									&ProductIdSize) );
	}
	else
	{
		ProductIdSize = 0;
	}

/* number of steps */ 
for(i=0; i<PTEST->TestSize; i++)   			
	{
	if(PTEST->Test[i].process_nb == global->process_nb)
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
					
					sprintf(row, "1|1_%d|Step%d%s^i1^c0000%s^d%d¦%d¦%s¦%s¦¦%s¦%s\n",   
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
					
					/* add parameters */ 
					EXCCHECK( TestParameterTreeString(pelstring, 
												  PTEST->Test[i].test_nb, 
												  test_index, 
										          ProductId, 
										          ProductIdSize) ); 
				
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
#define __FUNC__ "TreeGetUpperLevelString"
static SElExceptionPtr TreeGetUpperLevelString(const char* keyNode, char* upperlevel)
{
	SElExceptionPtr pexception = NULL; 
	int32_t			error = 0;
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

#define BKTREE_ROW_SHIFT 	20
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "TreeSetSelectedItem"
static SElExceptionPtr TreeSetSelectedItem(char* keyNode)
{
	SElExceptionPtr pexception = NULL;
	int32_t  		error = 0;	
	HRESULT			hr;
	int32_t			l1=0, l2=0, l3=0;
	char	 		skeynode[32];
	short int		exist;
	int				shift = BKTREE_ROW_SHIFT;

	/* shift the bktree. result is that the selected item is in the middle of the bktree list*/
	EXCCHECKCOM( BKLib2__DBKTreeSetSelectedItem (global->treeHandle, NULL, keyNode));
	sscanf(keyNode, "%d_%d_%d", &l1, &l2, &l3); 
	
	if(l3==0)	/* test level */
	{
		do
		{
			sprintf(skeynode, "%d_%d", l1, l2+shift);
			EXCCHECKCOM( BKLib2__DBKTreeIsNodeExist (global->treeHandle, NULL, skeynode, &exist));
			shift--;
		}while(!exist && (shift >= 0));
		EXCCHECKCOM( BKLib2__DBKTreeEnsureVisible (global->treeHandle, NULL, skeynode));
	}
	else		/* parameter level */
	{
		EXCCHECKCOM( BKLib2__DBKTreeEnsureVisible (global->treeHandle, NULL, keyNode));
	}
	

Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "GetSelectTestNb"
static SElExceptionPtr	GetSelectTestNb(char* keyNode)
{
	SElExceptionPtr pexception = NULL;  
	int32_t			error = 0;
	HRESULT			hr;
	short			exist;
	int32_t			i = 1;
	char			node[32];
	long			index;
	STestStep		test = {0};
	BKLib2Type_OLE_COLOR 	color;  
	
	global->test_nb_insSize = 0;
	
	do
	{
		sprintf(node, "1_%d", i++);
		EXCCHECKCOM( BKLib2__DBKTreeIsNodeExist (global->treeHandle, NULL, node, &exist));
		if(!exist)
			continue;
		
		EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemTextColor (global->treeHandle, NULL, node, "root", &color));        
		if(color==BKTREE_COLOR_SELECT)
		{
			EXCCHECKCOM( BKLib2__DBKTreeGetNodeData (global->treeHandle, NULL, node, &index));
			if(index)
			{
				index -= 1;
				test = PTEST->Test[index];
			
				if(global->test_nb_insSize == BKTREE_MAX_SELECTED_ITEM)
					break;
				
				global->test_nb_ins[global->test_nb_insSize++] = test.test_nb;	
			}
		}
			
	}while(exist);
	
	if(global->test_nb_insSize == 0)
	{
		EXCCHECKCOM( BKLib2__DBKTreeGetNodeData (global->treeHandle, NULL, keyNode, &index)); 
		if(index)
		{
			index -= 1;
			test = PTEST->Test[index];
			global->test_nb_ins[global->test_nb_insSize++] = test.test_nb;
		}
	}
	
Error:
	EXCRETHROW( pexception);   
}
