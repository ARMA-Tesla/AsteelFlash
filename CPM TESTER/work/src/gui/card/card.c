#include <eventfactory/EventFactory.h> 
#include <guid.h>
#include "card_plugin.h"
#include "card.h" 

#define NUMBER_OF_PANELS 	1

static int					gs_panel[NUMBER_OF_PANELS+1];
static SElEventFactoryPtr   gs_EventFactory = NULL;  

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ShowPanel"
static SElExceptionPtr ShowPanel(void)
{
	int32_t         error = 0;
   	SElExceptionPtr pexception = NULL;
	int32_t			visible;
	
	EXCCHECKCVI( GetPanelAttribute( gs_panel[CARD], ATTR_VISIBLE, &visible));
	if(!visible)
	{
		EXCCHECKCVI( DisplayPanel(gs_panel[CARD])); 
	}
		
Error:      
   EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ClosePanel"
static SElExceptionPtr ClosePanel(void)          
{
	int32_t			error = 0;
   	SElExceptionPtr	pexception = NULL;
	int32_t			visible;
	
	EXCCHECKCVI( GetPanelAttribute( gs_panel[CARD], ATTR_VISIBLE, &visible));
	if(visible)
	{
		EXCCHECKCVI( HidePanel(gs_panel[CARD])); 
	}

Error:      
	EXCRETHROW( pexception);
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
#define __FUNC__ "{Tester Plugin}::Initialize"
PLUGIN_API SElExceptionPtr PLUGIN_INIT_NAME_CODE( SPluginArgsPtr a_pPluginArgs )
{
	SElExceptionPtr  	pexception = NULL;    
	int32_t         	error = 0;
	int32_t         	main_panel = 0;
	HRESULT				hr;
   
	int32_t			monitor_number =0;
	int32_t			monitor_id = 0,
					monitor_id2 = 0,
					monitor_id3 = 0;
	int32_t         top, left, 
					width, height;
	
   	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
                            "MaintenancePanel",
                            sizeof(main_panel),
                            &main_panel));

	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
									 "WindowHandle",
									 sizeof(hWnd),
									 &hWnd));
	
	EXCCHECKCVI( gs_panel[CARD] = LoadPanelEx(main_panel, "card.uir", CARD, __CVIUserHInst)); 

	EXCCHECK( a_pPluginArgs->SetArg(a_pPluginArgs, "CardPanel", ARG_INT32, &gs_panel[CARD])); 
	
	EXCCHECKCVI( SetPanelAttribute (gs_panel[CARD], ATTR_TOP, 0));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[CARD], ATTR_LEFT, 0));
	
	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
	                        "EventFactory",
	                        sizeof(SElEventFactoryPtr),
	                        &gs_EventFactory));

Error:   
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{Tester Plugin}::Run"
PLUGIN_API SElExceptionPtr PLUGIN_RUN_NAME_CODE( SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
    
	if( 0==strcmp( uuid, PLUGIN_CARD))
	{
		EXCCHECK( ShowPanel());
	}
				
Error:   
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{Tester Plugin}::Leave"
PLUGIN_API SElExceptionPtr PLUGIN_LEAVE_NAME_CODE( SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
   	SElExceptionPtr	pexception = NULL;
   
	EXCCHECK( ClosePanel());

Error:   
   	EXCRETHROW( pexception);

}

#undef __FUNC__
#define __FUNC__ "{Testerr Plugin}::Shutdown"
PLUGIN_API SElExceptionPtr PLUGIN_SHUTDOWN_NAME_CODE( SPluginArgsPtr a_pPluginArgs)
{
   	SElExceptionPtr   pexception = NULL;
   	int32_t         error = 0;

	if(gs_panel[CARD])
		EXCCHECKCVI( DiscardPanel(gs_panel[CARD]));

Error:   
   EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{Tester Plugin}::Info"
PLUGIN_API SElExceptionPtr PLUGIN_GETINFO_NAME_CODE( SPluginInfoPtr* a_pInfo)
{
	SElExceptionPtr	pexception = NULL;  
	
	if(a_pInfo) EXCCHECK( (*a_pInfo)->SetInfoData(*a_pInfo, &gs_InfoData));
	
Error:	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Tester Plugin}::Attribute"
PLUGIN_API SElExceptionPtr PLUGIN_GETATTR_NAME_CODE( uint32_t  attrID, uint32_t size, void* value)
{
   return NULL;
}

#endif 

