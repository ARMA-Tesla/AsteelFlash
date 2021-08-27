#include <msxmldom2.h>
#include <utility.h>
#include <eventfactory/EventFactory.h>
#include <stdio.h>
#include <language/language.h>
#include <dbus/dbus.h>
#include <multithread/multithread.h>
#include <guid.h>
#include "dbusGUI_plugin.h"
#include <ansi_c.h>
#include "dbusGUI.h"
#include <userint.h>


#define TIMEOUT_DBUS			400
#define TIMEOUT_DBUS_DEFAULT	2000

#define NUMBER_OF_PANELS		1


SLanguagePtr 				gs_pLanguage = NULL;
void*						gs_hWnd;
static int					gs_panel[NUMBER_OF_PANELS+1];
static SElEventFactoryPtr   gs_EventFactory = NULL;
static SObjListPtr          gs_Modules = NULL;
static SDBUSListPtr			gs_dbus = NULL;

static HANDLE				gs_hThread = NULL;
static HANDLE				gs_hEventStop = NULL;


/* language support pattern */
static SLanguageItem	gs_dbusGUILang[] = {
   	P(DBUSGUI,                	_noop("DBUS Communication")),
   	{0 , NULL, 0}
};

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "OnFncSend"
int CVICALLBACK OnFncSend (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{

   	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	char    		UDA_name[128] = "";
	int 			target_adress;
	char    		MessageID[128] = "";
	char    		MessageData[128] = "";
	
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel, DBUSGUI_UDA_LIST, UDA_name);
			GetCtrlVal(panel, DBUSGUI_SEND_TARGET_ADRESS, &target_adress);
			GetCtrlVal(panel, DBUSGUI_SEND_MSG_ID, MessageID);
			GetCtrlVal(panel, DBUSGUI_SEND_MSG_DATA, MessageData);
				
			EXCCHECK( gs_dbus->WriteLowLevel(gs_dbus, UDA_name, target_adress, MessageID, MessageData));
			
			break;
	}

Error:
	EXCDISPLAY( pexception);
	return 0;
}

int CVICALLBACK OnFncReceive (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	char    		UDA_name[128] = "";
	int 			target_adress;
	char    		MessageID[128] = "";
	char    		MessageData[128] = "";
	
	switch (event)
	{
		case EVENT_COMMIT:

			GetCtrlVal(panel, DBUSGUI_UDA_LIST, UDA_name);
			GetCtrlVal(panel, DBUSGUI_RECEIVE_TARGET_ADRESS, &target_adress);

			EXCCHECK( gs_dbus->ReadLowLevel(gs_dbus, UDA_name, target_adress, MessageID, MessageData));
			
			SetCtrlVal(panel, DBUSGUI_RECEIVE_MSG_ID, MessageID);
			SetCtrlVal(panel, DBUSGUI_RECEIVE_MSG_DATA, MessageData);

			break;
	}
Error:
	EXCDISPLAY( pexception);
	return 0;
}



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
		//_CrtSetBreakAlloc(156);
#endif
         if (InitCVIRTE (hinstDLL, 0, 0) == 0)      
            return 0;   /* out of memory */
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

#undef __FUNC__
#define __FUNC__ "{DbusGUI Plugin}::Initialize"
PLUGIN_API SElExceptionPtr PLUGIN_INIT_NAME_CODE( SPluginArgsPtr a_pPluginArgs )
{
	SElExceptionPtr  	pexception = NULL;    
	int32_t         	error = 0,
						main_panel = 0;
	uint32_t			i = 0;
	const char*			channel = NULL;
   
   	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
                            "CardPanel",
                            sizeof(main_panel),
                            &main_panel));

	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
							 "WindowHandle",
							 sizeof(gs_hWnd),
							 &gs_hWnd));

	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
	                        "Modules",
	                        sizeof(SObjListPtr),
	                        &gs_Modules));

	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
	                        "EventFactory",
	                        sizeof(SElEventFactoryPtr),
	                        &gs_EventFactory));

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
	
	EXCCHECK( LoadPanelExWithLanguage(main_panel, "dbusGUI.uir", DBUSGUI, gs_dbusGUILang, &gs_panel[DBUSGUI]));
   
	EXCCHECKCVI( SetPanelAttribute (gs_panel[DBUSGUI], ATTR_TOP, 0));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[DBUSGUI], ATTR_LEFT, 0));

	EXCCHECKCVI( ResetTextBox(gs_panel[DBUSGUI], DBUSGUI_COMM_LIST, ""));

	EXCCHECKCVI( ClearListCtrl( gs_panel[DBUSGUI], DBUSGUI_UDA_LIST));

	if(gs_Modules)
	{
		gs_dbus = (SDBUSListPtr) gs_Modules->Get(gs_Modules, MODULE_DBUS);

		if(gs_dbus)
		{
			for (i = 0; i < gs_dbus->_udaList->GetCount(gs_dbus->_udaList); i++)
			{
				channel = gs_dbus->_udaList->GetNameOnIndex( gs_dbus->_udaList, i);
				EXCCHECKCVI( InsertListItem( gs_panel[DBUSGUI], DBUSGUI_UDA_LIST, -1, channel, channel));
			}
		}
	}
	
Error:   
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{DbusGUI Plugin}::Run"
PLUGIN_API SElExceptionPtr PLUGIN_RUN_NAME_CODE( SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
	SElExceptionPtr	pexception = NULL;
   	int32_t         error = 0;
 
	if(0 == strcmp(uuid, PLUGIN_DBUSGUI))
	{
		EXCCHECKCVI( DisplayPanel(gs_panel[DBUSGUI])); 
	}
				
Error:   
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{DbusGUI Plugin}::Leave"
PLUGIN_API SElExceptionPtr PLUGIN_LEAVE_NAME_CODE( SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
   	SElExceptionPtr	pexception = NULL;
   	int32_t         error = 0;
	
	EXCCHECKCVI( HidePanel(gs_panel[DBUSGUI])); 

Error:   
   	EXCRETHROW( pexception);

}

#undef __FUNC__
#define __FUNC__ "{DbusGUI Plugin}::Shutdown"
PLUGIN_API SElExceptionPtr PLUGIN_SHUTDOWN_NAME_CODE( SPluginArgsPtr a_pPluginArgs)
{
   	SElExceptionPtr	pexception = NULL;
   	int32_t         error = 0;

	if(gs_panel[DBUSGUI])
		EXCCHECKCVI( DiscardPanel(gs_panel[DBUSGUI]));

Error:   
   EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{DbusGUI Plugin}::Info"
PLUGIN_API SElExceptionPtr PLUGIN_GETINFO_NAME_CODE( SPluginInfoPtr* a_pInfo)
{
	SElExceptionPtr	pexception = NULL;  
	
	if(a_pInfo) EXCCHECK( (*a_pInfo)->SetInfoData(*a_pInfo, &gs_InfoData));
	
Error:	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{DbusGUI Plugin}::Attribute"
PLUGIN_API SElExceptionPtr PLUGIN_GETATTR_NAME_CODE( uint32_t  attrID, uint32_t size, void* value)
{
   return NULL;
}

#endif 

#pragma warning( pop)


int CVICALLBACK OnFncTest (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	char    		UDA_name[128] = "";
	int 			target_adress;
	char    		MessageID[128] = "";
	char    		MessageData[128] = "";
	switch (event)
	{
		case EVENT_COMMIT:

			switch (control)
			{
				case DBUSGUI_PWM_X8_4:	
					GetCtrlVal(panel, DBUSGUI_UDA_LIST, UDA_name);
					EXCCHECK( gs_dbus->ReadLowLevel(gs_dbus, UDA_name, 0x1B, "", MessageData));	
				case DBUSGUI_OFFSET:	
					GetCtrlVal(panel, DBUSGUI_UDA_LIST, UDA_name);
					EXCCHECK( gs_dbus->WriteLowLevel(gs_dbus, UDA_name, 0x1B, "7071", MessageData));	
					
					
			}
			
			
			break;
	}
	Error:
	EXCDISPLAY( pexception);
	return 0;
	return 0;
}
