#include <userint.h>
#include <exception/Exception.h>
#include <plugins/el_pluginserver.h>
#include <eventfactory/EventFactory.h>
#include <modules/elmodule.h>
#include <modules/module.h>
#include <bklib/bklib2.h>
#include <language/language.h>
#include <utility.h> 
#include <msxmldom.h>
#include <ansi_c.h>
#include <time.h>
#include <guid.h>
#include "application.h"

#define NB_OF_PANELS		3
static int					gs_panel[NB_OF_PANELS+1];
static bool_t				gs_consoleActive = FALSE;
static SObjListPtr 			gs_Modules = NULL;
static SPluginArgsPtr		gs_PluginParam = NULL;
static SElEventFactoryPtr	gs_EventFactory = NULL;
static SPluginServerPtr		gs_PluginServer = NULL;
SLanguagePtr				gs_pLanguage = NULL;
static SELStringPtr			gs_version = NULL;

static SElExceptionPtr SetPanelToFullScreen(int panel);
static SElExceptionPtr RegisterEventFunction(void);
static SElExceptionPtr RegisterEvent(const char* file_name);
static SElExceptionPtr PluginInfo(SELStringPtr* version);
static SElExceptionPtr eventLogFnc(const char* message);
static SElExceptionPtr messageLogFnc(const char* message); 
static SElExceptionPtr PluginRun(const char* eventName, void* pData);
static SElExceptionPtr PluginLeave(const char* eventName, void* pData);
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "WinMain"
int __stdcall WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
					   LPSTR lpszCmdLine, int nCmdShow)
{
	SElExceptionPtr		pexception = NULL;
	int32_t				error = 0;
	int  instance;
	
	
#if (!defined(_CVI_)&&defined(_DEBUG))
    _CrtSetBreakAlloc(56);
#endif

	if (InitCVIRTE (hInstance, 0, 0) == 0)
		return -1;    /* out of memory */

	
	CheckForDuplicateAppInstance (DO_NOT_ACTIVATE_OTHER_INSTANCE, &instance);
		if (instance)
			return -1;    /* instance running */

	
	EXCCHECKCVI( gs_panel[PANEL] = LoadPanel (0, 	 			"application.uir", PANEL));
	EXCCHECKCVI( gs_panel[INFO]  = LoadPanel (gs_panel[PANEL], 	"application.uir", INFO));  
	EXCCHECKCVI( gs_panel[LOG]	 = LoadPanel (0, 				"application.uir", LOG));    
	
	//EXCCHECKCVI( GetPanelAttribute (gs_panel[PANEL], ATTR_SYSTEM_WINDOW_HANDLE, &hWnd)); 

	EXCCHECKCVI( DisplayPanel (gs_panel[LOG]));   
	
	EXCCHECK( eleventfactory_new( &gs_EventFactory));  
	EXCCHECK( RegisterEvent(NULL) ); 
	EXCCHECK( gs_EventFactory->SetLogFnc(gs_EventFactory, eventLogFnc)); 
	
	EXCCHECK( pluginargs_new( &gs_PluginParam));  
	EXCCHECK( gs_PluginParam->SetArg(gs_PluginParam, "EventFactory", ARG_PTR, gs_EventFactory));  
	EXCCHECK( gs_PluginParam->SetArg(gs_PluginParam, "MainPanel", ARG_INT32, &gs_panel[PANEL]));
	EXCCHECK( gs_PluginParam->SetArg(gs_PluginParam, "WindowHandle", ARG_INT32, &hWnd));

	EXCCHECK( SetLogFnc( messageLogFnc));
	EXCCHECK( LoadAllModules( ".\\modules\\", &gs_Modules, gs_PluginParam));
	EXCCHECK( gs_PluginParam->SetArg(gs_PluginParam, "Modules", ARG_PTR, gs_Modules));

	/* language ini */
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
	EXCCHECK( PluginServer_new( &gs_PluginServer ));
	EXCCHECK( gs_PluginServer->SetLogFnc(gs_PluginServer, messageLogFnc));
	EXCCHECK( gs_PluginParam->SetArg(gs_PluginParam, "PluginServer", ARG_PTR, gs_PluginServer)); 
	
	EXCCHECK( gs_PluginServer->LoadAllPlugins(gs_PluginServer, ".\\plugins\\", "*.dll", gs_PluginParam));
	EXCCHECK( gs_PluginServer->LoadAllPlugins(gs_PluginServer, ".\\maintenance\\", "*.dll", gs_PluginParam));
	
	EXCCHECK( RegisterEventFunction() );
	
	EXCCHECK( gs_PluginServer->InitializeAllPlugins( gs_PluginServer, gs_PluginParam));	
	EXCCHECK( gs_PluginServer->RunAllPlugins(gs_PluginServer, gs_PluginParam));
	
	EXCCHECK( PluginInfo(&gs_version));
	EXCCHECK( gs_PluginParam->SetArg(gs_PluginParam, "Version", ARG_PTR, gs_version)); 
	
	EXCCHECKCVI( HidePanel (gs_panel[LOG]));  
	
	EXCCHECK( SetPanelToFullScreen(gs_panel[PANEL]));  

	EXCCHECKCVI( DisplayPanel (gs_panel[PANEL]));

	EXCCHECKCVI( RunUserInterface ());

	EXCCHECKCVI( HidePanel (gs_panel[PANEL]));
	
	EXCCHECKCVI( DisplayPanel (gs_panel[LOG]));  
	 
	EXCCHECK( PluginServer_delete( &gs_PluginServer));
	EXCCHECK( pluginargs_delete( &gs_PluginParam));
	EXCCHECK( UnLoadAllModules( &gs_Modules));
	EXCCHECK( eleventfactory_delete( &gs_EventFactory));
	
	EXCCHECKCVI( HidePanel (gs_panel[LOG])); 


Error:
	EXCDISPLAY( pexception);
	
	if(gs_consoleActive)
		FreeConsole();

	elstring_delete(&gs_version);
	//DiscardPanel( gs_panel[PANEL] );

#if !defined(_CVI_) && defined(_DEBUG) && defined(DUMP_MEMORY_LEAKS)
   _CrtDumpMemoryLeaks();
#endif

	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SetPaneltoFullScreen"
/*!
 Sets panel to the maximum size of the primary monitor.
 @param[in]  panel  Handle of the panel, which is dispalyed on primary monitor
 					in full size.
 @return	NULL is function is success or an exception.
 */
static SElExceptionPtr SetPanelToFullScreen(int panel)
{
	int				error = 0;
	SElExceptionPtr	pexception = NULL;
	int				monitor_id,
					width,
					height;
	int				top,
					left;

	EXCCHECKCVI( GetSystemAttribute( ATTR_PRIMARY_MONITOR, &monitor_id));
	EXCCHECKCVI( GetMonitorAttribute (monitor_id, ATTR_TOP, &top));
	EXCCHECKCVI( GetMonitorAttribute (monitor_id, ATTR_LEFT, &left));
	EXCCHECKCVI( GetMonitorAttribute( monitor_id, ATTR_WIDTH, &width));
	EXCCHECKCVI( GetMonitorAttribute( monitor_id, ATTR_HEIGHT, &height));
	
	EXCCHECKCVI( SetPanelPos(panel, top, left));
	EXCCHECKCVI( SetPanelSize(panel, height, width));
	
	//EXCCHECKCVI( SetCtrlAttribute (panel, PANEL_LINE2, ATTR_HEIGHT, height));
	//EXCCHECKCVI( SetCtrlAttribute (panel, PANEL_LINE1, ATTR_WIDTH, width));

	/* adjust background to resulution 1024x768 */
	/*if( width==1024)
	{
		EXCCHECKCVI( SetCtrlVal( panel, PANEL_PICTURERING, 1));
		
		EXCCHECKCVI( SetCtrlAttribute (panel, PANEL_LINE2, ATTR_WIDTH, 127));
		EXCCHECKCVI( SetCtrlAttribute (panel, PANEL_LINE1, ATTR_HEIGHT, 46));
	}*/
	
/*	EXCCHECKCVI( DisplayImageFile (panel, PANEL_PICTURE, "template.jpg"));
	EXCCHECKCVI( SetCtrlAttribute (panel, PANEL_PICTURE, ATTR_HEIGHT, height));
	EXCCHECKCVI( SetCtrlAttribute (panel, PANEL_PICTURE, ATTR_WIDTH, width));*/
	

	
Error:
	EXCRETHROW( pexception);
}	/* SetPaneltoFullScreen */

#define CHECK_KEYPRESS_INTERVAL(time) \
	if ( ((clock() - _last_clock) / (double)CLOCKS_PER_SEC) < (time/1000.0) ) { goto Error; } else _last_clock = clock();  

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "OnAppPanel"
int CVICALLBACK OnAppPanel (
	int panel,
	int event,
	void *callbackData,
	int eventData1,
	int eventData2
)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	SElEventDataPtr pdata = NULL;
	static time_t	_last_clock;
	
	switch (event)
	{
		case EVENT_KEYPRESS:
			
			CHECK_KEYPRESS_INTERVAL(300);
			
			EXCCHECK( eleventdata_new(&pdata));
			
			switch(eventData1)
			{
				case VAL_F1_VKEY:
					EXCCHECK(pdata->SetStringPtr(pdata, "F1_KEY"));
					EXCCHECK(gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_GUI_KEY_PRESSED", pdata)); 
					break;
				case VAL_F2_VKEY:
					EXCCHECK(pdata->SetStringPtr(pdata, "F2_KEY"));
					EXCCHECK(gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_GUI_KEY_PRESSED", pdata)); 
					break;
				case VAL_F3_VKEY:
					EXCCHECK(pdata->SetStringPtr(pdata, "F3_KEY"));
					EXCCHECK(gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_GUI_KEY_PRESSED", pdata)); 
					break;
				case VAL_F4_VKEY:
					EXCCHECK(pdata->SetStringPtr(pdata, "F4_KEY"));
					EXCCHECK(gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_GUI_KEY_PRESSED", pdata)); 
					break;
				case VAL_F5_VKEY:
					EXCCHECK(pdata->SetStringPtr(pdata, "F5_KEY"));
					EXCCHECK(gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_GUI_KEY_PRESSED", pdata)); 
					break;
				case VAL_F6_VKEY:
					EXCCHECK(pdata->SetStringPtr(pdata, "F6_KEY"));
					EXCCHECK(gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_GUI_KEY_PRESSED", pdata)); 
					break;
				case VAL_F7_VKEY:
					EXCCHECK(pdata->SetStringPtr(pdata, "F7_KEY"));
					EXCCHECK(gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_GUI_KEY_PRESSED", pdata)); 
					break;
				case VAL_F8_VKEY:
					EXCCHECK(pdata->SetStringPtr(pdata, "F8_KEY"));
					EXCCHECK(gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_GUI_KEY_PRESSED", pdata)); 
					break;
				case VAL_F9_VKEY:
					EXCCHECK(pdata->SetStringPtr(pdata, "F9_KEY"));
					EXCCHECK(gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_GUI_KEY_PRESSED", pdata)); 
					break;
				case VAL_F10_VKEY:
					EXCCHECK(pdata->SetStringPtr(pdata, "F10_KEY"));
					EXCCHECK(gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_GUI_KEY_PRESSED", pdata)); 
					break;
				case VAL_F11_VKEY:
					EXCCHECK(pdata->SetStringPtr(pdata, "F11_KEY"));
					EXCCHECK(gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_GUI_KEY_PRESSED", pdata)); 
					break;
				case VAL_F12_VKEY:
					EXCCHECK(pdata->SetStringPtr(pdata, "F12_KEY"));
					EXCCHECK(gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_GUI_KEY_PRESSED", pdata)); 
					break;
				case VAL_MENUKEY_MODIFIER | 's':
				case VAL_MENUKEY_MODIFIER | 'S': 
					EXCCHECK(pdata->SetStringPtr(pdata, "CTRL_S_KEY"));
					EXCCHECK(gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_GUI_KEY_PRESSED", pdata)); 
					break;
				case VAL_MENUKEY_MODIFIER | 'm':
				case VAL_MENUKEY_MODIFIER | 'M': 
					EXCCHECK(pdata->SetStringPtr(pdata, "CTRL_M_KEY"));
					EXCCHECK(gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_GUI_KEY_PRESSED", pdata)); 
					break;
				case VAL_MENUKEY_MODIFIER | 'p':
				case VAL_MENUKEY_MODIFIER | 'P': 
					EXCCHECK(gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_PAUSE", NULL)); 
					break;
				case VAL_MENUKEY_MODIFIER | 't':
				case VAL_MENUKEY_MODIFIER | 'T': 
					EXCCHECK(gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_TEST_SEQUENCE_RUN", NULL)); 
					break;
				case VAL_MENUKEY_MODIFIER | 'i':
				case VAL_MENUKEY_MODIFIER | 'I': 
					{
						int32_t visible;
					
						EXCCHECKCVI( GetPanelAttribute(gs_panel[INFO], ATTR_VISIBLE, &visible));
						if(!visible)
							EXCCHECKCVI( InstallPopup(gs_panel[INFO])); 
					}
					break;
				case VAL_ESC_VKEY:
					EXCCHECK(gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_GUI_KEY_ESC_PRESSED", NULL)); 
					break;
			}
			break;
		case EVENT_GOT_FOCUS:
			break;
		case EVENT_LOST_FOCUS:
			break;
		case EVENT_CLOSE:
			QuitUserInterface(0);
			break;
	
	}
	
Error:
	EXCDISPLAY( pexception);
	eleventdata_delete(&pdata);
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "QuitUserInterfaceCallback"
void CVICALLBACK QuitUserInterfaceCallback (void *callbackData)
{
	int32_t			   	error = 0;
	SElExceptionPtr    	pexception = NULL;

	if(gs_EventFactory)
		EXCCHECK(gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_QUIT_USER_INTERFACE", NULL)); 

	EXCCHECKCVI(ProcessSystemEvents());
	EXCCHECKCVI(QuitUserInterface(0));
	
Error:                       
	;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "MenuQuitCallback"
SElExceptionPtr MenuQuitCallback( const char* eventName, void* pData)
{
	int32_t			   	error = 0;
	SElExceptionPtr    	pexception = NULL;
	
	if(IDYES == MessageBox((HWND)hWnd, _("Quit application?"), "Exit", MB_YESNO|MB_ICONQUESTION)) /* do not translate Exit! */
		EXCCHECKCVI( PostDeferredCall(QuitUserInterfaceCallback, NULL));
	
Error:                       
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "RegisterEventFunction"
static SElExceptionPtr RegisterEventFunction(void)
{
	int				error = 0;
	SElExceptionPtr	pexception = NULL;
	
	if ( gs_EventFactory )
	{
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_GUI_QUIT",
														 MenuQuitCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory, 
														 "EVNT_PLUGIN_RUN", 
														 PluginRun));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory, 
														 "EVNT_PLUGIN_LEAVE", 
														 PluginLeave));
	}

Error:
	EXCRETHROW(pexception);
}	/* RegisterEventFunction */

#define TIMEOUT_INIT() 	clock_t	_start_clock = clock()

#define TIMEOUT_CHECK(timeout) \
	if ( ((clock() - _start_clock) / (double)CLOCKS_PER_SEC) > ((double)timeout/1000.0) ) { break; } else

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "RegisterEvent"
static SElExceptionPtr RegisterEvent(
   const char*   fileName
)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	HRESULT         hr = 0;
	CAObjHandle     pxml  = 0;
	CAObjHandle     pnode = 0;
	CAObjHandle     pparameter = 0;
	CAObjHandle     pparameter_old = 0;
	const char*     pfile_name = (fileName) ? fileName : "application.xml";
	VBOOL         	is_xml_success = 1;
	VARIANT         pfile_name_var = CA_VariantEmpty();
	char*         	ptext= NULL, *pend;

	EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml));
	EXCCHECKCVI( CA_VariantSetCString(  &pfile_name_var, pfile_name));
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (pxml, NULL, pfile_name_var, &is_xml_success));
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//events", &pnode));   	
	if(pnode)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGetfirstChild (pnode, NULL, &pparameter));
		while(pparameter)
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
			if(ptext)
			{
	 			pexception = gs_EventFactory->RegisterEvent(gs_EventFactory, ptext);
				EXCIGNORE( ELEVENT_WARN_ALREADY_EXIST);
				EXCCHECK( pexception);

				CA_FreeMemory(ptext);
				ptext=NULL;
			}
			pparameter_old = pparameter;
			MSXML_IXMLDOMNodeGetnextSibling (pparameter, NULL, &pparameter);

			EXCCHECKCVI( CA_DiscardObjHandle(pparameter_old));
			pparameter_old = 0;	
		};
	}

	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//console", &pnode)); 
	if(pnode)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pnode, NULL, &ptext));
		if(ptext)
		{
 			if(0==strcmp(ptext, "TRUE"))
			{
				HMENU hmenu;
				HWND hwnd = NULL;
				TIMEOUT_INIT();

				/* ignore CTRL+C */
				EXCCHECKWIN( SetConsoleCtrlHandler(NULL, TRUE));

				gs_consoleActive = TRUE;
				EXCCHECKWIN( AllocConsole());
				EXCCHECKWIN( SetConsoleTitle("LOG Console"));

				while(hwnd==NULL)
				{
					Sleep(100);

					hwnd = FindWindow(NULL, "LOG Console");
					TIMEOUT_CHECK(2000);
				}
				
				if(hwnd)
				{
					EXCCHECKWIN( ShowWindow(hwnd, SW_MINIMIZE));		

					hmenu = GetSystemMenu(hwnd, 0);
					/* remove close button */
					if(hmenu)
						EXCCHECKWIN( RemoveMenu( hmenu, 6, MF_BYPOSITION));
				}
			}
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
		EXCCHECKCVI( CA_DiscardObjHandle(pnode));
		pnode = 0;	
	}
	 
Error:
	CA_FreeMemory(ptext);
	CA_VariantClear(&pfile_name_var);
	if (pparameter) CA_DiscardObjHandle(pparameter);
	if (pparameter_old) CA_DiscardObjHandle(pparameter_old);
	if (pnode) CA_DiscardObjHandle(pnode);
	if (pxml) CA_DiscardObjHandle(pxml);
	EXCRETHROW( pexception);
} /* RegisterEvent */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PluginInfo"
static SElExceptionPtr PluginInfo(SELStringPtr* pversion)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	HRESULT			hr;
	SPluginInfoPtr 	pInfo = NULL;
	char			name[256],
					version[256],
					vendor[256],
					desc[256],
					info[256],
					www[256],
					email[256],
					uuid[256],
					datetime[256],
					row[2048];
	int				i, k;
	CAObjHandle		treeHandle; 
	SELStringPtr	pelstring = NULL;
	short int		AXvalue;
	
	EXCCHECK( elstring_new("", &pelstring));
	EXCCHECK( pelstring->Append(pelstring, "root|Name^w105$version|Version^w50$build|Build^w120$desc|Description^w150$uuid|UUID^w240$vendor|Vendor^w80$www|HomePage^w100$email|Email^w150$info|Info^w50\n" ) );
	EXCCHECK( pelstring->Append(pelstring, "|root|Info\n" ) ); 
	EXCCHECK( pelstring->Append(pelstring, "root|plugin|Plugins\n" ) ); 
	
	EXCCHECK( elstring_new("<version>\n", pversion));

	for(i=0; i<gs_PluginServer->GetCount(gs_PluginServer); i++)
	{
		EXCCHECK( gs_PluginServer->GetPluginInfo(gs_PluginServer, i, &pInfo));  
	
		if(pInfo)
		{
			EXCCHECK( pInfo->GetName(pInfo, name, 256));
			EXCCHECK( pInfo->GetVersion(pInfo, version, 256));    
			EXCCHECK( pInfo->GetVendor(pInfo, vendor, 256)); 
			EXCCHECK( pInfo->GetDescription(pInfo, desc, 256)); 
			EXCCHECK( pInfo->GetInfo(pInfo, info, 256)); 
			EXCCHECK( pInfo->GetHomepage(pInfo, www, 256)); 
			EXCCHECK( pInfo->GetEmail(pInfo, email, 256)); 
			EXCCHECK( pInfo->GetUUID(pInfo, uuid, 256));
			EXCCHECK( pInfo->GetBuildDateTime(pInfo, datetime, 256));

			sprintf(row, "plugin|%d|%s$%s$%s$%s$%s$%s$%s$%s$%s\n", 
						 i,
						 name,
						 version,
						 datetime,
						 desc,
						 uuid,
						 vendor,
						 www,
						 email,
						 info
						 );
		
			EXCCHECK( pelstring->Append(pelstring, row) );	

			sprintf(row,	"	<item type='plugin'>\n"
							"		<name>%s</name>\n"
							"		<version>%s</version>\n"
							"		<datetime>%s</datetime>\n"
							"		<uuid>%s</uuid>\n"
							"	</item>\n",
							name,
							version,
							datetime,
							uuid);

			EXCCHECK((*pversion)->Append(*pversion, row));
		}
	}

	EXCCHECK( pelstring->Append(pelstring, "root|modules|Modules\n" ) ); 

	for(k=0; k<GetModulesCount(); k++)
	{
		EXCCHECK( GetModuleInfo(k, MODULE_ATTR_MODULE_NAME, 256, name));
		EXCCHECK( GetModuleInfo(k, MODULE_ATTR_VERSION_STRING, 256, version));
		EXCCHECK( GetModuleInfo(k, MODULE_ATTR_BUILD_DATETIME, 256, datetime));
		EXCCHECK( GetModuleInfo(k, MODULE_ATTR_MODULE_DESCRIPTION, 256, desc));
		EXCCHECK( GetModuleInfo(k, MODULE_ATTR_MODULE_ID, 256, uuid));			
		EXCCHECK( GetModuleInfo(k, MODULE_ATTR_REQUIRED_MODULES, 256, info));

		sprintf(row, "modules|%d|%s$%s$%s$%s$%s$%s$%s$%s$%s\n", 
						 i+k,
						 name,
						 version,
						 datetime,
						 desc,
						 uuid,
						 "",
						 "",
						 "",
						 info
						 );
		
		EXCCHECK( pelstring->Append(pelstring, row) );	

		sprintf(row,	"	<item type='module'>\n"
						"		<name>%s</name>\n"
						"		<version>%s</version>\n"
						"		<datetime>%s</datetime>\n"
						"		<uuid>%s</uuid>\n"
						"	</item>\n",
						name,
						version,
						datetime,
						uuid);

		EXCCHECK((*pversion)->Append(*pversion, row));
	}

	EXCCHECK((*pversion)->Append(*pversion, "</version>"));

	EXCCHECKCVI( GetObjHandleFromActiveXCtrl (gs_panel[INFO], INFO_BKTREE, &treeHandle) ); 
	EXCCHECKCOM( BKLib2__DBKTreeFromString (treeHandle, NULL, pelstring->GetPtr(pelstring), "$", "\n", &AXvalue)); 
	EXCCHECKCOM( BKLib2__DBKTreeExpand (treeHandle, NULL, "root", &AXvalue));
	EXCCHECKCOM( BKLib2__DBKTreeExpand (treeHandle, NULL, "plugin", &AXvalue));  
	EXCCHECKCOM( BKLib2__DBKTreeExpand (treeHandle, NULL, "maintenance", &AXvalue));
	EXCCHECKCOM( BKLib2__DBKTreeExpand (treeHandle, NULL, "modules", &AXvalue));
	
Error:
	elstring_delete(&pelstring); 
	EXCRETHROW( pexception);
} 

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "OnInfoExit"
int CVICALLBACK OnInfoExit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr pexception = NULL;  
	int32_t			error = 0;
		
	switch (event)
	{
		case EVENT_COMMIT:
			EXCCHECKCVI( RemovePopup (0));
			break;
	}
		
Error:
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "eventLogFnc"
static SElExceptionPtr eventLogFnc(const char* message)
{
	SElExceptionPtr pexception = NULL;  
	int32_t			error = 0;	
	char			buffer[1024];
	
	sprintf(buffer, "_UT_EventFactory: %s\n", message);
	
	OutputDebugString(buffer);
	
Error:
	EXCRETHROW( pexception);   
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "messageLogFnc"
static SElExceptionPtr messageLogFnc(const char* message)
{
	SElExceptionPtr pexception = NULL;  
	int32_t			error = 0;	
	int32_t			index;	

	EXCCHECKCVI( SetCtrlVal (gs_panel[LOG], LOG_MESSAGE, message));
	
Error:
	EXCRETHROW( pexception);   	
}

typedef struct _SPostDeferredCallData
{
	char*					uuid;
	SPluginServerPtr		pPluginServer;
	SPluginArgsPtr			pPluginParam;
} SPostDeferredCallData, *SPostDeferredCallDataPtr;
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PluginRunPostDeferred"
void CVICALLBACK PluginRunPostDeferred(void *callbackData)
{
	SElExceptionPtr pexception = NULL; 
	SPostDeferredCallDataPtr	pdata = (SPostDeferredCallDataPtr)callbackData;

	if( pdata->uuid==NULL)
	{
		EXCDISPLAY( pdata->pPluginServer->RunAllPlugins(pdata->pPluginServer, pdata->pPluginParam));
	}
	else
	{
		EXCDISPLAY( pdata->pPluginServer->RunSelectedPlugin(pdata->pPluginServer, pdata->uuid, pdata->pPluginParam));
	}	

	free(pdata->uuid);
	free(pdata);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PluginRun"
static SElExceptionPtr PluginRun(const char* eventName, void* pData)
{
	SElExceptionPtr pexception = NULL;  
	int32_t			error = 0;
	const char*		uuid = ((SElEventDataPtr)pData)->GetStringPtr(pData);
	SPostDeferredCallDataPtr	pdata = NULL;

	pdata = calloc(1, sizeof(SPostDeferredCallData));
	
	if(uuid!=NULL && uuid[0]!='\0')
	{
		pdata->uuid = (char*)calloc( strlen(uuid)+1, sizeof(char));
		strcpy( pdata->uuid, uuid);
	}

	pdata->pPluginServer = gs_PluginServer;
	pdata->pPluginParam = gs_PluginParam;

	strcpy( pdata->uuid, uuid);
	EXCCHECKCVI( PostDeferredCall( PluginRunPostDeferred, pdata));
	
Error:
	EXCRETHROW( pexception);   	
}	

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PluginLeavePostDeferred"
void CVICALLBACK PluginLeavePostDeferred(void *callbackData)
{
	SElExceptionPtr pexception = NULL; 
	SPostDeferredCallDataPtr	pdata = (SPostDeferredCallDataPtr)callbackData;

	if( pdata->uuid==NULL)
	{
		EXCDISPLAY( pdata->pPluginServer->LeaveAllPlugins(pdata->pPluginServer, pdata->pPluginParam));
	}
	else
	{
		EXCDISPLAY( pdata->pPluginServer->LeaveSelectedPlugin(pdata->pPluginServer, pdata->uuid, pdata->pPluginParam));
	}	

	free(pdata->uuid);
	free(pdata);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PluginLeave"
static SElExceptionPtr PluginLeave(const char* eventName, void* pData)
{
	SElExceptionPtr pexception = NULL;  
	int32_t			error = 0;
	const char*		uuid = ((SElEventDataPtr)pData)->GetStringPtr(pData);
	SPostDeferredCallDataPtr	pdata = NULL;

	pdata = calloc(1, sizeof(SPostDeferredCallData));
	
	if(uuid!=NULL && uuid[0]!='\0')
	{
		pdata->uuid = (char*)calloc( strlen(uuid)+1, sizeof(char));
		strcpy( pdata->uuid, uuid);
	}

	pdata->pPluginServer = gs_PluginServer;
	pdata->pPluginParam = gs_PluginParam;

	
	EXCCHECKCVI( PostDeferredCall( PluginLeavePostDeferred, pdata));

Error:
	EXCRETHROW( pexception);   	
}	
