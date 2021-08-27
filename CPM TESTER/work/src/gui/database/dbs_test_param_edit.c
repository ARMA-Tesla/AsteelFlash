#include <bklib/BKLib2.h>
#include <utility.h>
#include <toolbox.h>
#include "dbs_global.h"
#include "dbs_test_param.h"							   
  
extern SGlobalPtr		global;  
extern int				gs_panel[]; 

static int32_t			gs_product_id;
static int32_t			gs_test_nb;

static int CVICALLBACK OnExitEdit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2);
static int CVICALLBACK OnExitNew (int panel, int control, int event,
      void *callbackData, int eventData1, int eventData2);
static int CVICALLBACK OnExitEditInsert (int panel, int control, int event,
      void *callbackData, int eventData1, int eventData2);

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "EditParameterDialog2"
SElExceptionPtr EditParameterDialog2(STestParameter	parameter)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	int32_t			top = 100, left = 30; 
	char*			path; 

	/* check editable parameter */
	if( 0 == strcmp(parameter.name, "@none") )
	{	
		MessageBox((HWND)hWnd, _("Not editable cell"), _("Warning"), MB_OK|MB_ICONWARNING);
		goto Error;
	}
	if( 0 == strcmp(parameter.value,"@new") )
	{   
		/* clear name */
		EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_VALUE, "")); 
		EXCCHECKCVI( SetCtrlAttribute (gs_panel[PARAM], PARAM_EXIT_OK, ATTR_CALLBACK_FUNCTION_POINTER, OnExitEditInsert));
		EXCCHECKCVI( SetCtrlAttribute (gs_panel[PARAM], PARAM_EXIT_CANCEL, ATTR_CALLBACK_FUNCTION_POINTER, OnExitEditInsert));

		EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_TESTER, 0)); 
		EXCCHECKCVI( SetCtrlAttribute(gs_panel[PARAM], PARAM_TESTER, ATTR_VISIBLE, FALSE));
	}
	else
	{
		EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_VALUE, parameter.value)); 
		EXCCHECKCVI( SetCtrlAttribute (gs_panel[PARAM], PARAM_EXIT_OK, ATTR_CALLBACK_FUNCTION_POINTER, OnExitEdit));
		EXCCHECKCVI( SetCtrlAttribute (gs_panel[PARAM], PARAM_EXIT_CANCEL, ATTR_CALLBACK_FUNCTION_POINTER, OnExitEdit));

		EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_TESTER, (parameter.tester_id!=0))); 
		EXCCHECKCVI( SetCtrlAttribute(gs_panel[PARAM], PARAM_TESTER, ATTR_VISIBLE, TRUE));
	}
	
	gs_test_nb = parameter.test_nb;
	gs_product_id = parameter.product_id;
	EXCCHECK( PPRODUCT->ProductGetIdName(PPRODUCT, parameter.product_id, &path) );   
	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_NB, path));

	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_NAME, parameter.name));
	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_DESCRIPT, GET_STRING(parameter.description)));
	
	EXCCHECKCVI( SetPanelAttribute (gs_panel[PARAM], ATTR_TITLE, _("Edit Current Parameter")));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[PARAM], PARAM_NB, ATTR_LABEL_TEXT, _("Product")));
	
	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_VTYPE, parameter.vtype)); 
	EXCCHECKCVI( SetCtrlAttribute(gs_panel[PARAM], PARAM_VTYPE, ATTR_VISIBLE, FALSE)); 
	EXCCHECKCVI( SetPanelAttribute(gs_panel[PARAM], ATTR_HEIGHT, (parameter.vtype==E_DVT_TEXT)? PARAMETER_DIALOG_HIGH_WITH_TEXT:PARAMETER_DIALOG_HIGH));
	
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[PARAM], PARAM_NAME, ATTR_NO_EDIT_TEXT, 1));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[PARAM], PARAM_NAME, ATTR_TEXT_BGCOLOR, 0x00D4D0C8));
	
	EXCCHECKCVI( SetPanelPos (gs_panel[PARAM], top, left));
	EXCCHECKCVI( InstallPopup(gs_panel[PARAM]));

Error:
	EXCRETHROW( pexception);   
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "InsertParameterDialog2"
SElExceptionPtr InsertParameterDialog2(STestParameter parameter)
{
   	int32_t     	error = 0;
	SElExceptionPtr pexception = NULL;
   	int32_t     	top = 100, left = 30; 
   	char*			path; 

	gs_test_nb = parameter.test_nb;
	gs_product_id = parameter.product_id;
	EXCCHECK( PPRODUCT->ProductGetIdName(PPRODUCT, parameter.product_id, &path) );   
	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_NB, path));

	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_NAME, ""));
	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_DESCRIPT, GET_STRING(parameter.description)));
	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_VALUE, parameter.value));

	EXCCHECKCVI( SetPanelAttribute (gs_panel[PARAM], ATTR_TITLE, _("New Parameter")));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[PARAM], PARAM_NB, ATTR_LABEL_TEXT, _("Product")));

	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_VTYPE, E_DVT_STRING)); 
	EXCCHECKCVI( SetCtrlAttribute(gs_panel[PARAM], PARAM_VTYPE, ATTR_VISIBLE, FALSE)); 
	EXCCHECKCVI( SetPanelAttribute(gs_panel[PARAM], ATTR_HEIGHT, PARAMETER_DIALOG_HIGH));

	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_TESTER, 0)); 
	EXCCHECKCVI( SetCtrlAttribute(gs_panel[PARAM], PARAM_TESTER, ATTR_VISIBLE, FALSE));

	EXCCHECKCVI( SetCtrlAttribute (gs_panel[PARAM], PARAM_NAME, ATTR_NO_EDIT_TEXT, 0));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[PARAM], PARAM_NAME, ATTR_TEXT_BGCOLOR, VAL_WHITE));
	
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[PARAM], PARAM_EXIT_OK, ATTR_CALLBACK_FUNCTION_POINTER, OnExitNew));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[PARAM], PARAM_EXIT_CANCEL, ATTR_CALLBACK_FUNCTION_POINTER, OnExitNew)); 

	EXCCHECKCVI( SetPanelPos (gs_panel[PARAM], top, left));
	EXCCHECKCVI( InstallPopup(gs_panel[PARAM]));

Error:
   EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "DelParameterDialog2"
SElExceptionPtr DelParameterDialog2(	
		STestParameter	parameter,
		const char* 	keyNode,
		const char* 	keyCol
)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	HRESULT			hr;
	
	/* check editable parameter */
	if( 0 == strcmp(parameter.value, "@new") 
		|| 0 == strcmp(parameter.name, "@none") )
	{	
		MessageBox((HWND)hWnd, _("Not editable cell"), _("Warning"), MB_OK|MB_ICONWARNING);
	}
	else if(IDOK==MessageBox((HWND)hWnd, _("Delete selected parametr?\n - child parameters will be removed"), _("Delete Dialog"), MB_OKCANCEL|MB_ICONQUESTION))
	{
		char* 		nextCol;
		short int	AXvalue;
		
		parameter.user_id = global->activeUser.user_id;
		EXCCHECKCOM( BKLib2__DBKTreeGetColumnNext (global->treeHandle, NULL, "desc", &nextCol));
		
		/* reload */
		if( 0==strcmp(nextCol, keyCol) )
		{
			EXCCHECK( PTESTPARAM->TestParamDel(PTESTPARAM, parameter) ); 
			EXCCHECK( TreeParametersSet(TEST_PARAM_RELOAD) );
		}
		else
		{		
			/* disable sorting algorithm */
			PTESTPARAM->sort = FALSE;
		
			EXCCHECK( PTESTPARAM->TestParamDel(PTESTPARAM, parameter) ); 
			
			EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemText (global->treeHandle, NULL, keyNode, keyCol, "", &AXvalue));
			EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemData (global->treeHandle, NULL, keyNode, keyCol, 0));
		}
		
		CA_FreeMemory(nextCol);
	}
	 
Error:
	PTESTPARAM->sort = TRUE;
	EXCRETHROW( pexception);    
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::OnExitEdit"
static int CVICALLBACK OnExitEdit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	STestParameter	parameter = {0};

	switch (event)
		{
		case EVENT_COMMIT:
			
			if(control == PARAM_EXIT_OK)
			{
				parameter.test_nb = gs_test_nb;
				parameter.product_id = gs_product_id;
				EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_NAME, parameter.name));
				CHECK_VALUE( parameter.name);

				EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_VALUE, parameter.value));
				CHECK_VALUE( parameter.value); 

				EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_DESCRIPT, PDBS_ALLOC(&parameter.description, DBS_RECORD_LENGHT_DESCRIPTION)));
				parameter.user_id = global->activeUser.user_id;
				EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_VTYPE, &parameter.vtype)); 
				
				EXCCHECK( PTESTPARAM->TestParamEdit(PTESTPARAM, parameter) );
			
				EXCCHECK( TreeParametersSet(TEST_PARAM_RELOAD) );
			}
			EXCCHECKCVI( RemovePopup(gs_panel[PARAM]));  
			break;
		}

Error:
    EXCDISPLAY( pexception); 
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::OnExitNew"
static int CVICALLBACK OnExitNew (int panel, int control, int event,
      void *callbackData, int eventData1, int eventData2)
{
   int32_t      	error = 0;
   SElExceptionPtr 	pexception = NULL;
   STestParameter   parameter = {0};
	   
   switch (event)
      {
      case EVENT_COMMIT:
         
		if(control == PARAM_EXIT_OK)
		{
			parameter.test_nb = gs_test_nb;
			parameter.product_id = gs_product_id;
			EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_NAME, parameter.name));
			CHECK_VALUE( parameter.name);

			EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_VALUE, parameter.value));
			CHECK_VALUE( parameter.value);

			EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_DESCRIPT, PDBS_ALLOC(&parameter.description, DBS_RECORD_LENGHT_DESCRIPTION)));
			parameter.user_id = global->activeUser.user_id;
			EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_VTYPE, &parameter.vtype));  
			
			EXCCHECK( PTESTPARAM->TestParamInsert(PTESTPARAM, parameter) );
		
			EXCCHECK( TreeParametersSet(TEST_PARAM_RELOAD) ); 
		}
		EXCCHECKCVI( RemovePopup(gs_panel[PARAM]));  
		break;
	}

Error:
    EXCDISPLAY( pexception); 
   	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::OnExitEditInsert"
static int CVICALLBACK OnExitEditInsert (int panel, int control, int event,
      void *callbackData, int eventData1, int eventData2)
{
   int32_t      	error = 0;
   SElExceptionPtr 	pexception = NULL;
   STestParameter   parameter = {0};

   switch (event)
      {
      case EVENT_COMMIT:
         
         if(control == PARAM_EXIT_OK)
         {
			parameter.test_nb = gs_test_nb;
			parameter.product_id = gs_product_id;
            EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_NAME, parameter.name));
			CHECK_VALUE( parameter.name);

            EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_VALUE, parameter.value));
            CHECK_VALUE( parameter.value);

            EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_DESCRIPT, PDBS_ALLOC(&parameter.description, DBS_RECORD_LENGHT_DESCRIPTION)));
			parameter.user_id = global->activeUser.user_id;
			EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_VTYPE, &parameter.vtype));  
			
            EXCCHECK( PTESTPARAM->TestParamEditInsert(PTESTPARAM, parameter) );
         
			EXCCHECK( TreeParametersSet(TEST_PARAM_RELOAD) ); 
         }
         EXCCHECKCVI( RemovePopup(gs_panel[PARAM]));  
         break;
      }

Error: 
   	EXCDISPLAY( pexception); 
   	return 0;
}

