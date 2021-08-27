#include <userint.h>
#include "database.h"
#include <MSComCtrl/MSComCtl2.h>
#include <pwctrl.h>
#include <utility.h>
#include <stdio.h>
#include <bklib/BKLib2.h>  
#include <exception/exception.h>
#include <eventfactory/EventFactory.h>
#include <guid.h> 
#include <msxmldom.h>
#include "database_plugin.h"
#include "dbs_global.h" 
#include "dbs_history.h" 
#include "dbs_info.h" 
#include "dbs_product.h"
#include "dbs_product_param.h"
#include "dbs_test.h" 
#include "dbs_process.h"
#include "dbs_user.h"
#include "dbs_user_edit.h"
#include "dbs_test_param.h"
#include "ring_control.h"
#include "lang_translation.h" 

const char* HISTORY = \
"[2007-12-11] rT 2.0\n"
" * Created mode for line sequence editing.\n"
"[2008-01-16] rT 2.1\n"
" * Created function for test moving.\n";

#define NUMBER_OF_PANELS	9
#define NUMBER_OF_TABS		4

/* panels positions */
static int32_t					gv_PanelTop = 0;
static int32_t					gv_PanelLeft = 0;
static int32_t					gv_PanelHeight = 0;
static int32_t					gv_PanelWidth = 0;
#define PANEL_TOP				gv_PanelTop
#define PANEL_LEFT				gv_PanelLeft
#define PANEL_HEIGHT			gv_PanelHeight
#define PANEL_WIDTH				gv_PanelWidth

int							gs_panel[NUMBER_OF_PANELS+1];
int							gs_panelTab[NUMBER_OF_TABS+1];


SGlobalPtr					global = NULL;
SLanguagePtr				gs_pLanguage = NULL;   
static SElEventFactoryPtr   gs_EventFactory = NULL;  
static SObjListPtr          gs_Modules = NULL;  
static HINSTANCE			g_hInstance = NULL;

extern HRESULT CVICALLBACK DDTPickerChangeCallback(CAObjHandle caServerObjHandle,
                                                   void *caCallbackData);
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::BkTreeNodeClick"
static HRESULT CVICALLBACK BkTreeNodeClick (CAObjHandle caServerObjHandle,
    void *caCallbackData, char *keyNode)
{
	SElExceptionPtr pexception =  NULL;

	switch(global->editMode)
	{
		case EDIT_PRODUCT:
			EXCCHECK( BkTreeProductNodeClick(keyNode));
			break;
		case EDIT_PRODUCT_PARAM:
			EXCCHECK( BkTreeProductParamNodeClick(keyNode));  
			break;
		case EDIT_TEST:
			EXCCHECK( BkTreeStepsNodeClick(keyNode));
			break;
		case EDIT_TEST_PARAM:
			EXCCHECK( BkTreeParametersNodeClick(keyNode));
			break;
		case EDIT_LINE:
			EXCCHECK( BkTreeProcessNodeClick(keyNode));
			break;
		case EDIT_USER:
			EXCCHECK( BkTreeUserNodeClick(keyNode));
			break;
	}
   
Error: 
	EXCDISPLAY( pexception);
   	return 0;
}
                              
/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::BkTreePopupMenuCommand"
static HRESULT CVICALLBACK BkTreePopupMenuCommand (CAObjHandle caServerObjHandle,
    void *caCallbackData, char *keyNode, char *keyCol, long  menuItem)
{
	SElExceptionPtr pexception =  NULL;
   
	switch(global->editMode)
	{
		case EDIT_PRODUCT:
			EXCCHECK( BkTreeProductPopupMenuCommand(keyNode, keyCol, menuItem));                                                                            
			break;
		case EDIT_PRODUCT_PARAM:
			EXCCHECK( BkTreeProductParamPopupMenuCommand(keyNode, keyCol, menuItem));                                                                            
			break;
		case EDIT_TEST:
			EXCCHECK( BkTreeStepsPopupMenuCommand(keyNode, keyCol, menuItem));
			break;
		case EDIT_TEST_PARAM:
			EXCCHECK( BkTreeParametersPopupMenuCommand(keyNode, keyCol, menuItem));
			break;
		case EDIT_LINE:
			EXCCHECK( BkTreeProcessPopupMenuCommand(keyNode, keyCol, menuItem));
			break;
		case EDIT_USER:  	 
			EXCCHECK( BkTreeUserPopupMenuCommand(keyNode, keyCol, menuItem));
			break;
	}

Error:
	EXCDISPLAY( pexception);
   	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::BkTreeModify"
static HRESULT CVICALLBACK BkTreeModify (CAObjHandle caServerObjHandle,
      void *caCallbackData, char *keyNode, char *keyCol, long  modifyType)
{
	SElExceptionPtr pexception =  NULL;

	switch(global->editMode)
	{
		case EDIT_PRODUCT_PARAM:
			EXCCHECK( BkTreeProductParamModify(keyNode, keyCol, modifyType));                                                                            
			break;
		case EDIT_TEST_PARAM:
			EXCCHECK( BkTreeParametersModify(keyNode, keyCol, modifyType));
			break;
		case EDIT_LINE:
			EXCCHECK( BkTreeProcessModify(keyNode, keyCol, modifyType));
			break;
	}

Error:
	EXCDISPLAY( pexception);
   	return 0;
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::OnGroup"
int  CVICALLBACK OnGroup(int panel, int control, int event, 
   void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr pexception =  NULL;  
	int32_t      	error = 0;
	int32_t      	value;

	switch (event)
	{
		case EVENT_COMMIT:
			EXCCHECKCVI( GetCtrlVal (panel, control, &value));
			/* edit mode */
			switch(global->editMode)
			{   
				case EDIT_PRODUCT:
					EXCCHECK( RingGroupProductSet(control, value)); 
					break;
				case EDIT_TEST:
					EXCCHECK( RingGroupStepSet(control, value)); 
					break;
				case EDIT_LINE:
					EXCCHECK( RingGroupProcessSet(control, value)); 
					break;
			}
			break;
	}
   
Error:  
	EXCDISPLAY( pexception);
   	return 0;   
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::OnDbInsert"
int CVICALLBACK OnDbInsert (int panel, int control, int event,
      void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr	pexception = NULL;  
   
	switch (event)
	{
		case EVENT_COMMIT:
			
			if(IDOK==MessageBox((HWND)hWnd, _("Insert data into database?"), _("Save Changes"), MB_OKCANCEL|MB_ICONQUESTION))
			{
				if(PDBS->data_changed)
				{
					EXCCHECK(PDBS->Save(PDBS));
				}
				
			    if(PDBS->data_changed_line)
				{
					EXCCHECK( PDBS->SaveLine(PDBS));
				}
							
			}
			break;
	}

Error:
   	EXCDISPLAY( pexception);
   	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::OnDbRead"
int CVICALLBACK OnDbRead (int panel, int control, int event,
      void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr	pexception = NULL;  
   
	switch (event)
	{
		case EVENT_COMMIT:
			if(IDOK==MessageBox((HWND)hWnd, _("Read data from database?\nAll changes will be deleted!"), _("Save Changes"), MB_OKCANCEL|MB_ICONQUESTION))
			{
				if(PDBS->data_changed)
					EXCCHECK(PDBS->Read(PDBS));

				if(PDBS->data_changed_line)
					EXCCHECK(PDBS->ReadLine(PDBS));

				global->product_id = PDBS->_branch_id;
				
				global->set_product_id = PDBS->_branch_id;
				EXCCHECK( RingGroupAttribute( RING_GROUPS_SET_PATH));
				
				EXCCHECK(ChangeMode(global->editMode));  
			}
			break;
	}
   
Error:
   	EXCDISPLAY( pexception);
   	return 0;
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::OnDbDeleteHist"
int CVICALLBACK OnDbDeleteHist (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr pexception = NULL; 
	
	switch (event)
	{
		case EVENT_COMMIT:
			if(IDOK==MessageBox((HWND)hWnd, _("Delete parameter history?\n -history data from test_parameters.dbo\n -history data from product_parameters.dbo\n -history data from product_masters.dbo"), _("Delete Parameter History"), MB_OKCANCEL|MB_ICONQUESTION))
			{
				EXCCHECK(PDBS->DelHist(PDBS)); 
			}
			break;
	}
   
Error:
	EXCDISPLAY( pexception);
	return 0;
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "ChangeMode"
SElExceptionPtr ChangeMode (int32_t mode)
{
	SElExceptionPtr pexception = NULL;
	
	/* deinitialize */
	switch(global->editMode)
	{
		case EDIT_PRODUCT:
			EXCCHECK( TreeProductClose());
			break;
		case EDIT_PRODUCT_PARAM:
			EXCCHECK( TreeProductParamClose());  
			break;
		case EDIT_TEST:
			EXCCHECK( TreeStepsClose()); 	
			break;
		case EDIT_TEST_PARAM:
			EXCCHECK( TreeParametersClose()); 	
			break;
		case EDIT_LINE:
			EXCCHECK( TreeProcessClose()); 	
			break;
		case EDIT_USER:
			EXCCHECK( TreeUserClose()); 
			break;
		case DBS_HISTORY:
			EXCCHECK( TreeHistoryClose()); 
			break;
		case DBS_INFO:
			EXCCHECK( InfoHide()); 
			break;
	}
	
	/* initialize */ 
	switch(mode)
	{
		case EDIT_PRODUCT:
			EXCCHECK( TreeProductIni());
			break;
		case EDIT_PRODUCT_PARAM:
			EXCCHECK( TreeProductParamIni());
			break;
		case EDIT_TEST:
			EXCCHECK( TreeStepsIni());
			break;
		case EDIT_TEST_PARAM:
			EXCCHECK( TreeParametersIni()); 	
			break;
		case EDIT_LINE:
			EXCCHECK( TreeProcessIni()); 	
			break;
		case EDIT_USER:
			EXCCHECK( TreeUserIni()); 
			break;
		case DBS_HISTORY:
			EXCCHECK( TreeHistoryIni()); 
			break;
		case DBS_INFO:
			EXCCHECK( InfoShow()); 
			break;	
	}

	global->editMode = mode;
	
Error:
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::OnBKTREE"
int CVICALLBACK OnBKTREE (int panel, int control, int event,
      void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr pexception = NULL; 
	int32_t			error = 0;
	bool_t			activated = FALSE;
   
	switch (event)
	{
		case EVENT_COMMIT:
			break;
		case EVENT_KEYPRESS:
			if(eventData1==VAL_INSERT_VKEY)
			{
				switch(global->editMode)
				{
					case EDIT_TEST:
						EXCCHECK( BkTreeStepsInsert());
						break;
				}
			}
			else if(eventData1==VAL_UP_ARROW_VKEY
					|| eventData1==VAL_DOWN_ARROW_VKEY
					|| eventData1==VAL_ESC_VKEY
					|| eventData1==VAL_ENTER_VKEY)
			{
				switch(global->editMode)
				{
					case EDIT_TEST:
						EXCCHECK( BkTreeStepMove(eventData1, &activated));
						if(activated)
							error = -1;
						break;
				}
			}
			break;
	}
   
Error:
	EXCDISPLAY( pexception);
   	return error;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::OnTimer"
int CVICALLBACK OnTimer (int panel, int control, int event,
      void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			 if(global && PDBS)
			 {
				 SetCtrlAttribute (panel, TABPANEL_1_DB_INSERT, ATTR_DIMMED, (!PDBS->data_changed && !PDBS->data_changed_line));
				 SetCtrlAttribute (panel, TABPANEL_1_DB_READ, ATTR_DIMMED, (!PDBS->data_changed && !PDBS->data_changed_line));
			 }  
			 break;
	}
   
	return 0;
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "OnExit"
static SElExceptionPtr OnExit(void)
{
	SElExceptionPtr pexception = NULL;
   
	if(PDBS->data_changed)
	{
		if(IDYES==MessageBox((HWND)hWnd, _("Save Changes Into Database?"), _("Exit"), MB_YESNO|MB_ICONQUESTION))
		{
			if(PDBS->data_changed)
				EXCCHECK( PDBS->Save(PDBS));
			
			if(PDBS->data_changed_line)
				EXCCHECK( PDBS->SaveLine(PDBS));
		}
		else
		{
			if(PDBS->data_changed)
				EXCCHECK( PDBS->Read(PDBS));

			if(PDBS->data_changed_line)
				EXCCHECK( PDBS->ReadLine(PDBS));
		}
	}

Error:
   EXCRETHROW( pexception);   
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ShowPanel"
SElExceptionPtr ShowPanel(void)
{
	int32_t         error = 0;
   	SElExceptionPtr pexception = NULL;
	int32_t			visible;

	EXCCHECKCVI( GetPanelAttribute( gs_panel[DBS], ATTR_VISIBLE, &visible));
	if(!visible)
	{
		EXCCHECK( ActiveUserSet((SUserListPtr)gs_Modules->Get(gs_Modules, MODULE_USER)));
		
		/* go to root of product */
		global->product_id = 0;
		EXCCHECK( ChangeMode(EDIT_PRODUCT));
		EXCCHECK( RingGroupAttribute(RING_GROUPS_RELOAD));

		EXCCHECKCVI( DisplayPanel(gs_panel[DBS]));		
		EXCCHECKCVI( SetCtrlAttribute(gs_panelTab[PAGE_ONE], TABPANEL_1_TIMER_HRUBCA, ATTR_ENABLED, TRUE));
	}
		
Error:      
   EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ClosePanel"
SElExceptionPtr ClosePanel(void)          
{
	int32_t			error = 0;
   	SElExceptionPtr	pexception = NULL;
	int32_t			visible;
	
	EXCCHECKCVI( GetPanelAttribute( gs_panel[DBS], ATTR_VISIBLE, &visible));
	if(visible)
	{
		/* save or storno changes */     
		EXCCHECK( OnExit());

		/* return previous branch_id if there was a change */
		if(global->branch_id)
		{
			bool_t reload;

			EXCCHECK( PDBS->ChangeBranchId(PDBS, global->branch_id, &reload));
			global->branch_id = 0;
		}

		EXCCHECKCVI( SetCtrlAttribute(gs_panelTab[PAGE_ONE], TABPANEL_1_TIMER_HRUBCA, ATTR_ENABLED, FALSE));
		EXCCHECKCVI( HidePanel(gs_panel[DBS]));
	}
	
Error:      
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ReadConfig"
static SElExceptionPtr ReadConfig(const char* fileXml)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	HRESULT			hr = 0;
	CAObjHandle		pxml  = 0;
	CAObjHandle		pnode = 0;
	CAObjHandle		pparameter = 0;
	const char*		pfile_name = (fileXml)? fileXml:CONFIG_XML_PATH;
	VBOOL			is_xml_success;
	VARIANT			pfile_name_var = CA_VariantEmpty();
	char*			ptext= NULL;

	EXCCHECKCVI( GetProjectDir(global->configxml));
	strcat(global->configxml, pfile_name);

	EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml));
	EXCCHECKCVI( CA_VariantSetCString(  &pfile_name_var, global->configxml));
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (pxml, NULL, pfile_name_var, &is_xml_success));
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//plugin[@id='"PLUGIN_DATABASE"']", &pnode));	
	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "io_file", &pparameter));   	
	if(pparameter)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
		if(ptext && *ptext)
		{
			EXCCHECKCVI( GetProjectDir(global->pathIOxml));
			strcat(global->pathIOxml, ptext);		
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
		CA_DiscardObjHandle(pparameter);
		pparameter = 0;		
	}
	
Error:
	CA_FreeMemory(ptext);
	CA_VariantClear(&pfile_name_var);
	if (pparameter) CA_DiscardObjHandle(pparameter);
	if (pnode) CA_DiscardObjHandle(pnode);
	if (pxml) CA_DiscardObjHandle(pxml);
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
/*#undef __FUNC__
#define __FUNC__ "EventFunctionCallback"
SElExceptionPtr EventFunctionCallback( const char* eventName, void* pData)
{
	SElExceptionPtr	pexception = NULL;
	
	if(0==strcmp(eventName, "EVNT_GUI_DATABASE_PRODUCT"))
	{
		EXCCHECK( ChangeMode(EDIT_PRODUCT));
	}
	else if(0==strcmp(eventName, "EVNT_GUI_DATABASE_USER"))
	{
		EXCCHECK( ChangeMode(EDIT_USER));
	}
	else if(0==strcmp(eventName, "EVNT_GUI_DATABASE_HISTORY"))
	{
		EXCCHECK( ChangeMode(DBS_HISTORY));
	}
	else if(0==strcmp(eventName, "EVNT_GUI_DATABASE_INFO"))
	{
		EXCCHECK( ChangeMode(DBS_INFO));
	}
	
Error:      
   	EXCRETHROW( pexception);
} */

/*---------------------------------------------------------------------------*/
int CVICALLBACK Cb_ChangeTab (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr	pexception = NULL;  
	int 	page;
	
	switch (event)
	{
		case EVENT_ACTIVE_TAB_CHANGE:
			
		  	GetActiveTabPage (panel, control, &page);
	
			if(page == PAGE_ONE)
				EXCCHECK( ChangeMode(EDIT_PRODUCT));

			else if (page == PAGE_TWO)
				EXCCHECK( ChangeMode(EDIT_USER));

			else if(page == PAGE_THREE)
				EXCCHECK( ChangeMode(DBS_HISTORY));

			else if(page == PAGE_FOUR)
				EXCCHECK( ChangeMode(DBS_INFO));
	
		break;
	}

Error:
   	EXCDISPLAY( pexception);
   	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "RegisterEventFunction"
static SElExceptionPtr RegisterEventFunction(void)
{
	SElExceptionPtr	pexception = NULL;
   
	/*if ( gs_EventFactory )
	{
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
		                                   "EVNT_GUI_DATABASE_PRODUCT",
		                                   EventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
		                                   "EVNT_GUI_DATABASE_USER",
		                                   EventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
		                                   "EVNT_GUI_DATABASE_HISTORY",
		                                   EventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
		                                   "EVNT_GUI_DATABASE_INFO",
		                                   EventFunctionCallback));
	  
	} */
   
Error:
   EXCRETHROW(pexception);
}   /* RegisterEventFunction */

/*---------------------------------------------------------------------------*/
#if defined(_CVI_DLL_) || \
	defined(_WINDLL)
void DiscardUIObjectsForDLL (void)
{
   /* Discard the panels loaded in InitUIForDLL */
}

int __stdcall DllMain(
   HINSTANCE hinstDLL,
   DWORD fdwReason,
   LPVOID lpvReserved
)
{
   switch (fdwReason)
      {
      case DLL_PROCESS_ATTACH:
#if (!defined(_CVI_)&&defined(_DEBUG))
			//_CrtSetBreakAlloc(62);
#endif
         if (InitCVIRTE (hinstDLL, 0, 0) == 0)      
            return 0;   /* out of memory */
		 g_hInstance = hinstDLL;
         break;
      case DLL_PROCESS_DETACH:
         if (!CVIRTEHasBeenDetached())      /* Do not call CVI functions if cvirte.dll has already been detached. */
         {
            DiscardUIObjectsForDLL ();      /* Discard the panels loaded in InitUIForDLL */
            CloseCVIRTE ();      
         }
#if !defined(_CVI_) && defined(_DEBUG) && defined(DUMP_MEMORY_LEAKS)
			_CrtDumpMemoryLeaks();
#endif
         break;
      }

   return 1;
}

int __stdcall DllEntryPoint(
   HINSTANCE hinstDLL,
   DWORD fdwReason,
   LPVOID lpvReserved
)
{
   /* Included for compatibility with Borland */

    return DllMain (hinstDLL, fdwReason, lpvReserved);
}

static int passw_old_id_1;
static int passw_old_id_2;

#undef __FUNC__
#define __FUNC__ "{Database Plugin}::Initialize"
PLUGIN_API SElExceptionPtr PLUGIN_INIT_NAME_CODE( SPluginArgsPtr a_pPluginArgs )
{
	SElExceptionPtr	pexception = NULL;
	int32_t         error = 0;
	HRESULT			hr;
	int      		main_panel;
	SDBSUser      	puser = {0}; 
	int				top, height;  
	int             BK_error;
	
	
	global = calloc(1, sizeof(SGlobal)); 
	global->HistoryMerge = TRUE; 
	global->hInstance = g_hInstance;
  
	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
	                        "MaintenancePanel",
	                        sizeof(main_panel),
	                        &main_panel));

	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
									 "WindowHandle",
									 sizeof(hWnd),
									 &hWnd));

	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
	                        "Modules",
	                        sizeof(SObjListPtr),
	                        &gs_Modules));

	/* gettext */
	if(gs_Modules)
	{
		gs_pLanguage = (SLanguagePtr)gs_Modules->Get(gs_Modules, MODULE_LANGUAGE);  
		if(gs_pLanguage)
		{
			char pathname[MAX_PATHNAME_LEN];
			EXCCHECKCVI( GetProjectDir( pathname));
			strcat(pathname, "\\lang");
			LANGUAGE_PTR->bindtextdomain (LANGUAGE_PTR, PACKAGE, pathname);
			LANGUAGE_PTR->textdomain (LANGUAGE_PTR, PACKAGE);
		}
	}

	EXCCHECK( LoadPanelExWithLanguage(main_panel,    "database.uir", DBS, DBSlang, &gs_panel[DBS]) ); 
	EXCCHECK( LoadPanelExWithLanguage(gs_panel[DBS], "database.uir", TEST, TESTlang, &gs_panel[TEST]) ); 
	EXCCHECK( LoadPanelExWithLanguage(gs_panel[DBS], "database.uir", PARAM, PARAMlang, &gs_panel[PARAM]) ); 
	EXCCHECK( LoadPanelExWithLanguage(gs_panel[DBS], "database.uir", PRODUCT, PRODUCTlang, &gs_panel[PRODUCT]) );   
	EXCCHECK( LoadPanelExWithLanguage(gs_panel[DBS], "database.uir", PRODUCT_CP, PRODUCT_CPlang, &gs_panel[PRODUCT_CP]) );
	EXCCHECK( LoadPanelExWithLanguage(gs_panel[DBS], "database.uir", DLG_USER, DLG_USERlang, &gs_panel[DLG_USER]) );
	EXCCHECK( LoadPanelExWithLanguage(gs_panel[DBS], "database.uir", MASTER, MASTERlang, &gs_panel[MASTER]) ); 


	EXCCHECKCVI( SetPanelAttribute (gs_panel[DBS], ATTR_TOP, 0));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[DBS], ATTR_LEFT, 0));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[TEST], ATTR_TOP, 0));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[TEST], ATTR_LEFT, 0));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[PARAM], ATTR_TOP, 0));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[PARAM], ATTR_LEFT, 0));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[PRODUCT], ATTR_TOP, 0));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[PRODUCT], ATTR_LEFT, 0));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[PRODUCT_CP], ATTR_TOP, 0));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[PRODUCT_CP], ATTR_LEFT, 0));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[DLG_USER], ATTR_TOP, 0));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[DLG_USER], ATTR_LEFT, 0));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[MASTER], ATTR_TOP, 0));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[MASTER], ATTR_LEFT, 0));
	
	EXCCHECKCVI(SetPanelAttribute (gs_panel[DBS], ATTR_SCALE_CONTENTS_ON_RESIZE, TRUE));
	EXCCHECKCVI(SetPanelAttribute (gs_panel[TEST], ATTR_SCALE_CONTENTS_ON_RESIZE, TRUE));
	//EXCCHECKCVI(SetPanelAttribute (gs_panel[PARAM], ATTR_SCALE_CONTENTS_ON_RESIZE, TRUE));
	EXCCHECKCVI(SetPanelAttribute (gs_panel[PRODUCT], ATTR_SCALE_CONTENTS_ON_RESIZE, TRUE));
	EXCCHECKCVI(SetPanelAttribute (gs_panel[PRODUCT_CP], ATTR_SCALE_CONTENTS_ON_RESIZE, TRUE));
	EXCCHECKCVI(SetPanelAttribute (gs_panel[DLG_USER], ATTR_SCALE_CONTENTS_ON_RESIZE, TRUE));
	EXCCHECKCVI(SetPanelAttribute (gs_panel[MASTER], ATTR_SCALE_CONTENTS_ON_RESIZE, TRUE));

								
	EXCCHECKCVI( GetObjHandleFromActiveXCtrl (gs_panel[DBS], DBS_BKTREE, &global->treeHandle) );
 
	GetPanelHandleFromTabPage (gs_panel[DBS], DBS_TAB, PAGE_ONE, &gs_panelTab[PAGE_ONE]);
	GetPanelHandleFromTabPage (gs_panel[DBS], DBS_TAB, PAGE_TWO, &gs_panelTab[PAGE_TWO]);
	GetPanelHandleFromTabPage (gs_panel[DBS], DBS_TAB, PAGE_THREE, &gs_panelTab[PAGE_THREE]);
	GetPanelHandleFromTabPage (gs_panel[DBS], DBS_TAB, PAGE_FOUR, &gs_panelTab[PAGE_FOUR]);

	EXCCHECKCVI( SetCtrlAttribute(gs_panelTab[PAGE_ONE], TABPANEL_1_TIMER_HRUBCA, ATTR_ENABLED, FALSE));

	EXCCHECKCVI( GetObjHandleFromActiveXCtrl (gs_panelTab[PAGE_THREE], TABPANEL_3_DTPICKER, &global->pickerHandle) );
	EXCCHECKCVI( GetObjHandleFromActiveXCtrl (gs_panelTab[PAGE_FOUR], TABPANEL_4_DTPICKER_1, &global->hpicDayHandle) );
	EXCCHECKCVI( GetObjHandleFromActiveXCtrl (gs_panelTab[PAGE_FOUR], TABPANEL_4_DTPICKER_2, &global->hpicHourHandle) ); 

	EXCCHECKCOM( BKLib2__DBKTreeEventsRegOnNodeClick (global->treeHandle, BkTreeNodeClick, NULL, 1, NULL));
	EXCCHECKCOM( BKLib2__DBKTreeEventsRegOnPopupMenuCommand (global->treeHandle, BkTreePopupMenuCommand, NULL, 1, NULL));
	EXCCHECKCOM( BKLib2__DBKTreeEventsRegOnModify (global->treeHandle, BkTreeModify, NULL, 1, NULL));

	EXCCHECKCOM( MSComCtl2_DDTPickerEventsRegOnChange (global->pickerHandle, DDTPickerChangeCallback, NULL, 1, NULL));  

	EXCCHECKCVI( PasswordCtrl_ConvertFromString (gs_panel[DLG_USER], DLG_USER_PASSW1));
	EXCCHECKCVI( PasswordCtrl_ConvertFromString (gs_panel[DLG_USER], DLG_USER_PASSW2));

	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
	                        "EventFactory",
	                        sizeof(SElEventFactoryPtr),
	                        &gs_EventFactory));

	if(gs_Modules)
	{
		global->pdbs = (SDBSPtr)gs_Modules->Get(gs_Modules, MODULE_DBS);
		global->ibutton = (SIButtonPtr)gs_Modules->Get(gs_Modules, MODULE_IBUTTON); 

		/*if(global->ibutton)
		{
			EXCCHECK( global->ibutton->RegisterCallbackFnc2(global->ibutton, iButtonReceivedCallback)); //TEST
		}*/
	}

	EXCCHECK( elstring_new(NULL, &global->pelstring));
	EXCCHECK( timer_new(&global->ptimer));
	
	if(gs_Modules)
	{
		SUserListPtr puser_list = (SUserListPtr)gs_Modules->Get(gs_Modules, MODULE_USER);
		
		EXCCHECK( ReadConfig(NULL));
		EXCCHECK( ActiveUserSet(puser_list));   
		EXCCHECK( RingGroupIni()); 
		EXCCHECK( TreeProductIni());
		EXCCHECK( HistoryFilterSetCurrentTime()); 
	}

	EXCCHECK( RegisterEventFunction());
   
Error:
   	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Database Plugin}::Run"
PLUGIN_API SElExceptionPtr PLUGIN_RUN_NAME_CODE(SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
	SElExceptionPtr	pexception = NULL;

	if(0==strcmp( uuid, PLUGIN_DATABASE) && gs_panel[DBS])
	{
#if 0
		if(global->ptestseq==NULL)
		{
			EXCCHECK( a_pPluginArgs->GetArg(a_pPluginArgs, 
									"TestSequence", 
									ARG_PTR, 
									&(global->ptestseq)));
		}
#endif
		EXCCHECK( ShowPanel());
	}
				
Error:   
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{Database Plugin}::Leave"
PLUGIN_API SElExceptionPtr PLUGIN_LEAVE_NAME_CODE( SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
	SElExceptionPtr	pexception = NULL;

	if(gs_panel[DBS])
		EXCCHECK( ClosePanel());

Error:   
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Database Plugin}::Shutdown"
PLUGIN_API SElExceptionPtr PLUGIN_SHUTDOWN_NAME_CODE( SPluginArgsPtr a_pPluginArgs)
{
   	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;

	if(gs_panel[DBS])
		EXCCHECKCVI( DiscardPanel(gs_panel[DBS]));
   
	EXCCHECK( HistoryClear());

	EXCCHECK( timer_delete(&global->ptimer));
	EXCCHECK( elstring_delete(&global->pelstring));  

	free(global);
	global = NULL;

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Database Plugin}::Info"
PLUGIN_API SElExceptionPtr PLUGIN_GETINFO_NAME_CODE( SPluginInfoPtr* a_pInfo)
{
   	SElExceptionPtr	pexception = NULL;  
	
	if(a_pInfo) EXCCHECK( (*a_pInfo)->SetInfoData(*a_pInfo, &gs_InfoData));
	
Error:	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Database Plugin}::Attribute"
PLUGIN_API SElExceptionPtr PLUGIN_GETATTR_NAME_CODE( uint32_t  attrID, uint32_t size, void* value)
{
   return NULL;
}

#endif
