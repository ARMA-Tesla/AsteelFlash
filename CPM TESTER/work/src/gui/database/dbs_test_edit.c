#include "dbs_global.h"
#include "dbs_test.h"   
#include <toolbox.h>

extern SGlobalPtr   global;
extern int			gs_panel[];

static int32_t      gs_product_id;
static int32_t      gs_test_nb;
static int32_t      gs_editable;

static int32_t		gs_process_nb;


static int CVICALLBACK OnExitInsert (int panel, int control, int event,
      void *callbackData, int eventData1, int eventData2);
static int CVICALLBACK OnExitEdit (int panel, int control, int event,
      void *callbackData, int eventData1, int eventData2);
static int CVICALLBACK OnExitNewParameter (int panel, int control, int event,
      void *callbackData, int eventData1, int eventData2);
static int CVICALLBACK OnExitEditParameter (int panel, int control, int event,
      void *callbackData, int eventData1, int eventData2);

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "InsertTestDialog"
SElExceptionPtr InsertTestDialog(STestStep test, char* sname)
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
#define __FUNC__ "EditTestDialog"
SElExceptionPtr EditTestDialog(STestStep test, char* sname)
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
#define __FUNC__ "EditParameterDialog"
SElExceptionPtr EditParameterDialog(STestParameter parameter, char* pname, int32_t editable)
{
	SElExceptionPtr pexception = NULL; 
	int32_t			error = 0;  
	int32_t			top = 100, left = 30; 

	/* ini global variable */     
	gs_editable = editable; 
	gs_product_id = parameter.product_id;
	gs_test_nb = parameter.test_nb;

	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_NB, pname));
	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_NAME, parameter.name));
	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_DESCRIPT, GET_STRING(parameter.description)));
	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_VALUE, parameter.value));

	EXCCHECKCVI( SetPanelAttribute (gs_panel[PARAM], ATTR_TITLE, _("Edit Current Parameter")));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[PARAM], PARAM_EXIT_OK, ATTR_CALLBACK_FUNCTION_POINTER, OnExitEditParameter));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[PARAM], PARAM_EXIT_CANCEL, ATTR_CALLBACK_FUNCTION_POINTER, OnExitEditParameter));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[PARAM], PARAM_NB, ATTR_LABEL_TEXT, _("Parameter")));

	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_VTYPE, parameter.vtype));
	EXCCHECKCVI( SetCtrlAttribute(gs_panel[PARAM], PARAM_VTYPE, ATTR_VISIBLE, FALSE)); 
	EXCCHECKCVI( SetPanelAttribute(gs_panel[PARAM], ATTR_HEIGHT, (parameter.vtype==E_DVT_TEXT)? PARAMETER_DIALOG_HIGH_WITH_TEXT:PARAMETER_DIALOG_HIGH));

	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_TESTER, (parameter.tester_id!=0))); 
	EXCCHECKCVI( SetCtrlAttribute(gs_panel[PARAM], PARAM_TESTER, ATTR_VISIBLE, TRUE));

	EXCCHECKCVI( SetCtrlAttribute (gs_panel[PARAM], PARAM_NAME, ATTR_NO_EDIT_TEXT, 1));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[PARAM], PARAM_NAME, ATTR_TEXT_BGCOLOR, 0x00D4D0C8));

	EXCCHECKCVI( SetPanelPos (gs_panel[PARAM], top, left));
	EXCCHECKCVI( InstallPopup(gs_panel[PARAM]));

Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "NewParameterDialog"
SElExceptionPtr NewParameterDialog(STestStep test, char* sname)
{
	SElExceptionPtr pexception = NULL; 
	int32_t			error = 0;
	int32_t			top = 100, left = 30; 

	/* ini global variable */
	gs_product_id = test.product_id;
	gs_test_nb = test.test_nb;

	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_NB, sname));
	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_NAME, ""));
	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_DESCRIPT, ""));
	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_VALUE, ""));

	EXCCHECKCVI( SetPanelAttribute (gs_panel[PARAM], ATTR_TITLE, _("Insert New Parameter")));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[PARAM], PARAM_EXIT_OK, ATTR_CALLBACK_FUNCTION_POINTER, OnExitNewParameter));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[PARAM], PARAM_EXIT_CANCEL, ATTR_CALLBACK_FUNCTION_POINTER, OnExitNewParameter));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[PARAM], PARAM_NB, ATTR_LABEL_TEXT, _("Step")));

	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_VTYPE, E_DVT_STRING));
	EXCCHECKCVI( SetCtrlAttribute(gs_panel[PARAM], PARAM_VTYPE, ATTR_VISIBLE, FALSE)); 
	EXCCHECKCVI( SetPanelAttribute(gs_panel[PARAM], ATTR_HEIGHT, PARAMETER_DIALOG_HIGH));

	EXCCHECKCVI( SetCtrlVal(gs_panel[PARAM], PARAM_TESTER, 0)); 
	EXCCHECKCVI( SetCtrlAttribute(gs_panel[PARAM], PARAM_TESTER, ATTR_VISIBLE, FALSE));

	EXCCHECKCVI( SetCtrlAttribute (gs_panel[PARAM], PARAM_NAME, ATTR_NO_EDIT_TEXT, 0));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[PARAM], PARAM_NAME, ATTR_TEXT_BGCOLOR, 0x00FFFFFF));

	EXCCHECKCVI( SetPanelPos (gs_panel[PARAM], top, left));
	EXCCHECKCVI( InstallPopup(gs_panel[PARAM]));

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SkipTestDialog"
SElExceptionPtr SkipTestDialog(STestStep	test, int32_t editable)
{
	SElExceptionPtr	pexception = NULL;

	if(editable & TREE_EDIT_DEL)
	{
		if(IDOK==MessageBox((HWND)hWnd, _("Skip selected test?"), _("Skip Dialog"), MB_OKCANCEL|MB_ICONQUESTION))
		{
			test.product_id = global->product_id;
			test.user_id = global->activeUser.user_id;
			EXCCHECK( PTEST->TestSkip(PTEST, test));
			EXCCHECK( TreeStepsIni());
		}
	}

Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "DelTestDialog"
SElExceptionPtr DelTestDialog(STestStep	test, int32_t editable)
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
				EXCCHECK( TreeStepsIni()); 
			}
		}
	}

Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "DelTestDialog"
SElExceptionPtr RemoveSkipTestDialog(STestStep test, int32_t editable)
{
	SElExceptionPtr pexception = NULL;

	if(IDOK==MessageBox((HWND)hWnd, _("Remove skip for selected test?"), _("Remove Skip Dialog"), MB_OKCANCEL|MB_ICONQUESTION))
	{
		test.product_id = global->product_id;
		test.user_id = global->activeUser.user_id;
		EXCCHECK( PTEST->TestRemoveSkip(PTEST, test) );
		EXCCHECK( TreeStepsIni()); 
	}

Error:
	EXCRETHROW( pexception);      
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "DelParameterDialog"
SElExceptionPtr DelParameterDialog(STestParameter parameter, int32_t editable)
{
	int32_t      	error = 0;
	SElExceptionPtr 	pexception = NULL;
   
	if( editable & TREE_EDIT_PARAMETR )
	{
		if( editable & TREE_EDIT_PARAMETR_INSERT )
		{
			MessageBox((HWND)hWnd, _("Delete parameter from upper level"), _("Warning"), MB_OK|MB_ICONWARNING);
		}
		else
		{
			if(IDOK==MessageBox((HWND)hWnd, _("Delete selected parametr?\n - if this is parameter specification, only this specification will be removed\n - if this is main parameter, this parameter and all specifications will be removed"), _("Delete Dialog"), MB_OKCANCEL|MB_ICONQUESTION))
			{
				parameter.product_id = global->product_id;
				parameter.user_id = global->activeUser.user_id;
				EXCCHECK( PTESTPARAM->TestParamDel(PTESTPARAM, parameter) );
				EXCCHECK( TreeStepsIni()); 
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
				EXCCHECK( TreeStepsIni());
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
            EXCCHECK( TreeStepsIni());
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
#define __FUNC__ "{Database Plugin}::OnExitEditParameter"
static int CVICALLBACK OnExitEditParameter (int panel, int control, int event,
      void *callbackData, int eventData1, int eventData2)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	STestParameter	parameter = {0};
	double			timer;
   
	switch (event)
	{
		case EVENT_COMMIT:
			if(control == PARAM_EXIT_OK)
			{   
				TIMER_START;  
	            
				parameter.product_id = global->product_id;
				parameter.test_nb = gs_test_nb;  
	            
				EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_NAME, parameter.name));
				CHECK_VALUE( parameter.name);   

				EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_VALUE, parameter.value));
				CHECK_VALUE( parameter.value);

				EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_DESCRIPT, PDBS_ALLOC(&parameter.description, DBS_RECORD_LENGHT_DESCRIPTION)));
				parameter.user_id = global->activeUser.user_id;
				EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_VTYPE, &parameter.vtype)); 
	            
				if(gs_editable & TREE_EDIT_PARAMETR)
				{
					if(gs_editable & TREE_EDIT_PARAMETR_INSERT)
					{
						EXCCHECK( PTESTPARAM->TestParamEditInsert(PTESTPARAM, parameter) );
					}
					else
					{
						EXCCHECK( PTESTPARAM->TestParamEdit(PTESTPARAM, parameter) ); 
					}
				}
				EXCCHECK( TreeStepsIni());
				TIMER_STOP(timer);
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
#define __FUNC__ "{Database Plugin}::OnExitNewParameter"
static int CVICALLBACK OnExitNewParameter (int panel, int control, int event,
      void *callbackData, int eventData1, int eventData2)
{
	int32_t          error = 0;
	SElExceptionPtr pexception = NULL;
	STestParameter   parameter = {0};
	double           timer;  

	switch (event)
	{
	case EVENT_COMMIT:
		if(control == PARAM_EXIT_OK)
		{
			TIMER_START;
			parameter.product_id = gs_product_id;
			parameter.test_nb = gs_test_nb;

			EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_NAME, parameter.name));
			CHECK_VALUE( parameter.name);

			EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_VALUE, parameter.value));
			CHECK_VALUE( parameter.value);

			EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_DESCRIPT, PDBS_ALLOC(&parameter.description, DBS_RECORD_LENGHT_DESCRIPTION)));
			parameter.user_id = global->activeUser.user_id;
			EXCCHECKCVI( GetCtrlVal(gs_panel[PARAM], PARAM_VTYPE, &parameter.vtype));

			EXCCHECK( PTESTPARAM->TestParamInsert(PTESTPARAM, parameter));
			EXCCHECK( TreeStepsIni()); 
			TIMER_STOP(timer);
		}
		EXCCHECKCVI( RemovePopup(gs_panel[PARAM]));  
		break;
	}

Error:   
   	EXCDISPLAY( pexception); 
   	return 0;
}

