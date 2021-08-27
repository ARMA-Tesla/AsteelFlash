#include <bklib/BKLib2.h>
#include <utility.h>
#include <toolbox.h>
#include "dbs_global.h"
#include "dbs_product_param.h"							     

extern SGlobalPtr		global;  
extern int				gs_panel[]; 

static int32_t			gs_product_id;

static int CVICALLBACK OnExitEdit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2);
static int CVICALLBACK OnExitNew (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2);
static int CVICALLBACK OnExitEditInsert (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2);

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::OnVtype"
int  CVICALLBACK OnVtype(int panel, int control, int event, 
   void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr pexception =  NULL;  
	int32_t      	error = 0;
	int32_t			vtype = 0;

	switch (event)
	{
		case EVENT_COMMIT:
			EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_VTYPE, &vtype));
			EXCCHECKCVI( SetPanelAttribute(gs_panel[PARAM], ATTR_HEIGHT, (vtype==E_DVT_TEXT)? PARAMETER_DIALOG_HIGH_WITH_TEXT:PARAMETER_DIALOG_HIGH));
			break;
	}

Error:
	EXCDISPLAY( pexception);
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "EditProductParamDialog"
SElExceptionPtr EditProductParamDialog(SProductParameter parameter)
{
	int32_t		error = 0;
	SElExceptionPtr pexception = NULL;
	int32_t		top = 100, left = 30; 
	char*		path; 

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
	
	gs_product_id = parameter.product_id;
	EXCCHECK( PPRODUCT->ProductGetIdName(PPRODUCT, parameter.product_id, &path) );
	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_NB, path));

	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_NAME, parameter.name));
	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_DESCRIPT, GET_STRING(parameter.description)));
	
	EXCCHECKCVI( SetPanelAttribute (gs_panel[PARAM], ATTR_TITLE, _("Edit Current Parameter")));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[PARAM], PARAM_NB, ATTR_LABEL_TEXT, _("Product")));
	
	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_VTYPE, parameter.vtype));  
	EXCCHECKCVI( SetCtrlAttribute(gs_panel[PARAM], PARAM_VTYPE, ATTR_VISIBLE, TRUE));
	EXCCHECKCVI( SetPanelAttribute(gs_panel[PARAM], ATTR_HEIGHT, (parameter.vtype==E_DVT_TEXT)? PARAMETER_DIALOG_HIGH_WITH_TEXT:PARAMETER_DIALOG_HIGH));
	EXCCHECKCVI( ResetTextBox(gs_panel[PARAM], PARAM_TEXTBOX, GET_STRING(parameter.text)));
	
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[PARAM], PARAM_NAME, ATTR_NO_EDIT_TEXT, 1));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[PARAM], PARAM_NAME, ATTR_TEXT_BGCOLOR, 0x00D4D0C8));
	
	EXCCHECKCVI( SetPanelPos (gs_panel[PARAM], top, left));
	EXCCHECKCVI( InstallPopup(gs_panel[PARAM]));

Error:
	EXCRETHROW( pexception);   
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "InsertProductParamDialog"
SElExceptionPtr InsertProductParamDialog(SProductParameter parameter)
{
	int32_t		error = 0;
	SElExceptionPtr pexception = NULL;
	int32_t		top = 100, left = 30; 
	char*		path;  

	gs_product_id = parameter.product_id;
	EXCCHECK( PPRODUCT->ProductGetIdName(PPRODUCT, parameter.product_id, &path) );
	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_NB, path));

	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_NAME, ""));
	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_DESCRIPT, GET_STRING(parameter.description)));
	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_VALUE, parameter.value));
	
	EXCCHECKCVI( SetPanelAttribute (gs_panel[PARAM], ATTR_TITLE, _("New Parameter")));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[PARAM], PARAM_NB, ATTR_LABEL_TEXT, _("Product")));
	
	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_VTYPE, E_DVT_STRING)); 
	EXCCHECKCVI( SetCtrlAttribute(gs_panel[PARAM], PARAM_VTYPE, ATTR_VISIBLE, TRUE)); 
	EXCCHECKCVI( SetPanelAttribute(gs_panel[PARAM], ATTR_HEIGHT, PARAMETER_DIALOG_HIGH));
	EXCCHECKCVI( ResetTextBox(gs_panel[PARAM], PARAM_TEXTBOX, ""));

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
#define __FUNC__ "DelProductParamDialog"
SElExceptionPtr DelProductParamDialog(	
		SProductParameter	parameter,
		const char* keyNode,
		const char* keyCol
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
		goto Error;
	}
	
	if(IDOK==MessageBox((HWND)hWnd, _("Delete selected parametr?\n - child parameters will be removed"), _("Delete Dialog"), MB_OKCANCEL|MB_ICONQUESTION))
	{
		char* 		nextCol;
		short int	AXvalue;
		
		parameter.user_id = global->activeUser.user_id;
		EXCCHECKCOM( BKLib2__DBKTreeGetColumnNext (global->treeHandle, NULL, "desc", &nextCol));
		
		/* reload */  
		if( 0==strcmp(nextCol, keyCol) )
		{
			EXCCHECK( PPARAM->ProductParamDel(PPARAM, parameter) );   
			EXCCHECK( TreeProductParamSet(PRODUCT_PARAM_RELOAD) );
		}
		else
		{		
			/* disable sorting algorithm */
			PPARAM->sort = FALSE;
			
			EXCCHECK( PPARAM->ProductParamDel(PPARAM, parameter) );   
			
			EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemText (global->treeHandle, NULL, keyNode, keyCol, "", &AXvalue));
			EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemData (global->treeHandle, NULL, keyNode, keyCol, 0));
		}
		
		CA_FreeMemory( nextCol );
	}
	
Error:
	PPARAM->sort = TRUE;
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
	SProductParameter	parameter = {0};
	char			text[5000] = "";

	switch (event)
		{
		case EVENT_COMMIT:
			
			if(control == PARAM_EXIT_OK)
			{
				parameter.product_id = gs_product_id;
				EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_NAME, parameter.name));
				EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_VTYPE, &parameter.vtype)); 
				if(parameter.vtype==E_DVT_TEXT)
				{
					EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_TEXTBOX, text));
					PDBS_ALLOC(&parameter.text, strlen(text));
					strcpy(PDBS_ALLOC(&parameter.text, strlen(text)), text);
					CHECK_VALUE( text);
				}
				else
				{
					EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_VALUE, parameter.value));
					CHECK_VALUE( parameter.value);
				}

				EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_TESTER, &parameter.tester_id));

				EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_DESCRIPT, PDBS_ALLOC(&parameter.description, DBS_RECORD_LENGHT_DESCRIPTION)));
				parameter.user_id = global->activeUser.user_id;
				
				EXCCHECK( PPARAM->ProductParamEdit(PPARAM, parameter));
				EXCCHECK( TreeProductParamSet(PRODUCT_PARAM_RELOAD));
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
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	SProductParameter	parameter = {0};
	char			text[5000] = "";

	switch (event)
		{
		case EVENT_COMMIT:
			
			if(control == PARAM_EXIT_OK)
			{
				parameter.product_id = gs_product_id; 
				EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_NAME, parameter.name));
				CHECK_VALUE( parameter.name);

				EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_VTYPE, &parameter.vtype)); 
				if(parameter.vtype==E_DVT_TEXT)
				{
					EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_TEXTBOX, text));
					PDBS_ALLOC(&parameter.text, strlen(text));
					strcpy(PDBS_ALLOC(&parameter.text, strlen(text)), text);
					CHECK_VALUE( text);
				}
				else
				{
					EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_VALUE, parameter.value));
					CHECK_VALUE( parameter.value);
				}

				EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_DESCRIPT, PDBS_ALLOC(&parameter.description, DBS_RECORD_LENGHT_DESCRIPTION)));
				parameter.user_id = global->activeUser.user_id;
				
				EXCCHECK( PPARAM->ProductParamInsert(PPARAM, parameter));
				EXCCHECK( TreeProductParamSet(PRODUCT_PARAM_RELOAD));
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
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	SProductParameter	parameter = {0};
	char			text[5000] = "";

	switch (event)
		{
		case EVENT_COMMIT:
			
			if(control == PARAM_EXIT_OK)
			{
				parameter.product_id = gs_product_id; 
				EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_NAME, parameter.name));

				EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_VTYPE, &parameter.vtype)); 
				if(parameter.vtype==E_DVT_TEXT)
				{
					EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_TEXTBOX, text));
					PDBS_ALLOC(&parameter.text, strlen(text));
					strcpy(PDBS_ALLOC(&parameter.text, strlen(text)), text);
					CHECK_VALUE( text);
				}
				else
				{
					EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_VALUE, parameter.value));
					CHECK_VALUE( parameter.value);
				}

				EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_DESCRIPT, PDBS_ALLOC(&parameter.description, DBS_RECORD_LENGHT_DESCRIPTION)));
				parameter.user_id = global->activeUser.user_id;  
				
				EXCCHECK( PPARAM->ProductParamInsert(PPARAM, parameter) );
				EXCCHECK( TreeProductParamSet(PRODUCT_PARAM_RELOAD) );
			}
			EXCCHECKCVI( RemovePopup(gs_panel[PARAM]));  
			break;
		}

Error:
	EXCDISPLAY( pexception);
	return 0;
}

