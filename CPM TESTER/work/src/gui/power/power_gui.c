#include <exception/exception.h>
#include <power/class/powers.h>
#include <labels/tester_labels.h>
#include <eventfactory/EventFactory.h>
#include "power_gui_plugin.h"
#include <ansi_c.h>
#include "power_gui.h"

#define NB_OF_PANELS		1 
#define NAME_LENGTH			128
#define MAX_GROUPS			30

/*GLOBALS*/
static SObjListPtr     		gs_Modules = NULL;
static SElEventFactoryPtr   gs_EventFactory = NULL;  
static int					gs_panel[NB_OF_PANELS+1];
static SPluginInfoPtr		gs_pPI = NULL; 



void Activate_Controls(int handle)
{
	int group = 0;
	int i;

	SetCtrlAttribute (handle, POWER_INITIALIZE, ATTR_DIMMED, TRUE);
	SetCtrlAttribute (handle, POWER_CLOSE, ATTR_DIMMED, FALSE);
	
	SetCtrlAttribute (handle, POWER_CONFIG_CURRENT_LIMIT, ATTR_DIMMED, FALSE);
	SetCtrlAttribute (handle, POWER_LED_CONFIG_CUR_LIMIT, ATTR_DIMMED, FALSE);
	SetCtrlAttribute (handle, POWER_CURRENT_LIMIT, ATTR_DIMMED, FALSE);
		
	SetCtrlAttribute (handle, POWER_CONFIG_OUT, ATTR_DIMMED, FALSE);	
	SetCtrlAttribute (handle, POWER_LED_CONFIG_OUT, ATTR_DIMMED, FALSE);
	SetCtrlAttribute (handle, POWER_VOLTAGE, ATTR_DIMMED, FALSE);
	
	SetCtrlAttribute (handle, POWER_ENABLE_OUT, ATTR_DIMMED, FALSE);	
	SetCtrlAttribute (handle, POWER_OUTPUT, ATTR_DIMMED, FALSE);	
	SetCtrlAttribute (handle, POWER_LED_OUTPUT, ATTR_DIMMED, FALSE);	
		
	SetCtrlAttribute (handle, POWER_OVP, ATTR_DIMMED, FALSE);
	SetCtrlAttribute (handle, POWER_LED_OVP, ATTR_DIMMED, FALSE);	
	SetCtrlAttribute (handle, POWER_OVP_LIMIT, ATTR_DIMMED, FALSE);
	
	SetCtrlAttribute (handle, POWER_OCP, ATTR_DIMMED, FALSE);
	SetCtrlAttribute (handle, POWER_LED_OCP, ATTR_DIMMED, FALSE);	
	SetCtrlAttribute (handle, POWER_OCP_LIMIT, ATTR_DIMMED, FALSE);
}

void Desactivate_Controls(int handle)
{
	int group = 0;
	int i;

	
	SetCtrlAttribute (handle, POWER_INITIALIZE, ATTR_DIMMED, FALSE);
	SetCtrlAttribute (handle, POWER_CLOSE, ATTR_DIMMED, TRUE);
	
	SetCtrlAttribute (handle, POWER_CONFIG_CURRENT_LIMIT, ATTR_DIMMED, TRUE);
	SetCtrlAttribute (handle, POWER_LED_CONFIG_CUR_LIMIT, ATTR_DIMMED, TRUE);
	SetCtrlAttribute (handle, POWER_CURRENT_LIMIT, ATTR_DIMMED, TRUE);
		
	SetCtrlAttribute (handle, POWER_CONFIG_OUT, ATTR_DIMMED, TRUE);	
	SetCtrlAttribute (handle, POWER_LED_CONFIG_OUT, ATTR_DIMMED, TRUE);
	SetCtrlAttribute (handle, POWER_VOLTAGE, ATTR_DIMMED, TRUE);
	
	SetCtrlAttribute (handle, POWER_ENABLE_OUT, ATTR_DIMMED, TRUE);	
	SetCtrlAttribute (handle, POWER_OUTPUT, ATTR_DIMMED, TRUE);	
	SetCtrlAttribute (handle, POWER_LED_OUTPUT, ATTR_DIMMED, TRUE);	
		
	SetCtrlAttribute (handle, POWER_OVP, ATTR_DIMMED, TRUE);
	SetCtrlAttribute (handle, POWER_LED_OVP, ATTR_DIMMED, TRUE);	
	SetCtrlAttribute (handle, POWER_OVP_LIMIT, ATTR_DIMMED, TRUE);
	
	SetCtrlAttribute (handle, POWER_OCP, ATTR_DIMMED, TRUE);
	SetCtrlAttribute (handle, POWER_LED_OCP, ATTR_DIMMED, TRUE);	
	SetCtrlAttribute (handle, POWER_OCP_LIMIT, ATTR_DIMMED, TRUE);
}	
		
#if defined(_CVI_DLL_) || \
	defined(_WINDLL)
/*---------------------------------------------------------------------------*/
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
			//_CrtSetBreakAlloc(61);
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
 /*---------------------------------------------------------------------------*/
int __stdcall DllEntryPoint(
   HINSTANCE hinstDLL,
   DWORD fdwReason,
   LPVOID lpvReserved
)
{
   /* Included for compatibility with Borland */

    return DllMain (hinstDLL, fdwReason, lpvReserved);
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Power Plugin}::Initialize"
PLUGIN_API SElExceptionPtr PLUGIN_INIT_NAME_CODE( SPluginArgsPtr a_pPluginArgs )
{
	SElExceptionPtr pexception = NULL;
	int32_t         error = 0;
	int      		main_panel;

	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
			                        "CardPanel",
			                        sizeof(main_panel),
			                        &main_panel));

	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
									 "WindowHandle",
									 sizeof(hWnd),
									 &hWnd));

	EXCCHECKCVI( gs_panel[POWER] = LoadPanelEx(main_panel,"power_gui.uir", POWER, __CVIUserHInst) );

	EXCCHECKCVI( SetPanelAttribute (gs_panel[POWER], ATTR_TOP, 0));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[POWER], ATTR_LEFT, 0));

Error:
   	EXCRETHROW( pexception);
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Power Plugin}::Run"
PLUGIN_API SElExceptionPtr PLUGIN_RUN_NAME_CODE(SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
	SElExceptionPtr	pexception = NULL;  
	int32_t			error = 0;
	 
	if( 0==strcmp( uuid, PLUGIN_POWER))
	{
		EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
	                        			"Modules",
	                        			sizeof(SObjListPtr),
	                        			&gs_Modules));
		
		EXCCHECKCVI( DisplayPanel(gs_panel[POWER]));
	}

Error:   
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Power Plugin}::Leave"
PLUGIN_API SElExceptionPtr PLUGIN_LEAVE_NAME_CODE( SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;

	EXCCHECKCVI( HidePanel(gs_panel[POWER]));

Error:   
	EXCRETHROW( pexception);

}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Power Plugin}::Shutdown"
PLUGIN_API SElExceptionPtr PLUGIN_SHUTDOWN_NAME_CODE( SPluginArgsPtr a_pPluginArgs)
{
   	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;

	if(gs_panel[POWER])
	EXCCHECKCVI( DiscardPanel( gs_panel[POWER]));

Error:
	EXCRETHROW( pexception);
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Power Plugin}::Info"
PLUGIN_API SElExceptionPtr PLUGIN_GETINFO_NAME_CODE( SPluginInfoPtr* a_pInfo)
{
   	SElExceptionPtr	pexception = NULL;  
	
	if (a_pInfo) EXCCHECK( (*a_pInfo)->SetInfoData(*a_pInfo, &gs_InfoData));
	
Error:	
	EXCRETHROW( pexception);
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Power Plugin}::Attribute"
PLUGIN_API SElExceptionPtr PLUGIN_GETATTR_NAME_CODE( uint32_t  attrID, uint32_t size, void* value)
{
   return NULL;
}

#endif



int CVICALLBACK OnPower (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr	pexception = NULL;
	int32_t	 error = 0;
	double  Current_Limit;
	double  voltage;
	double  OVP_Value;
	double  OCP_Value;
	int 	output;			
	
	switch (event)
	{
		case EVENT_COMMIT:
		{	
			SPowersPtr	ppowers = (SPowersPtr)gs_Modules->Get(gs_Modules, MODULE_POWERS);  
			SPowerPtr		ppower = NULL;
							
			
			EXCCHECK( ppowers->GetPowerByIndex(ppowers, 0, &ppower));
			
			
			switch (control)
			{
				

				case POWER_INITIALIZE :

					pexception = ppower->Initialize(ppower);
					if(pexception==NULL) 
					{
						EXCCHECKCVI( SetCtrlAttribute(panel, POWER_LED_INITIALIZE,ATTR_FRAME_COLOR, VAL_GREEN));
						Activate_Controls (panel);
						ProcessSystemEvents ();
					}
					else
					{
						EXCDISPLAY( pexception);   
						EXCCHECKCVI( SetCtrlAttribute(panel, POWER_LED_INITIALIZE,ATTR_FRAME_COLOR, VAL_RED));
					}				
				
				break;
				
				
				case POWER_CLOSE :
	
					pexception = ppower->Disconnect(ppower);
					EXCCHECKCVI( SetCtrlAttribute(panel, POWER_LED_INITIALIZE, ATTR_FRAME_COLOR, VAL_WHITE));	
					Desactivate_Controls (panel);
					ProcessSystemEvents ();
		
				break;
					

				case POWER_CONFIG_CURRENT_LIMIT :
									
					GetCtrlVal(panel, POWER_CURRENT_LIMIT, &Current_Limit);
					pexception = ppower->ConfigureCurrentLimit(ppower, Current_Limit);	
					if(pexception==NULL) 
					{
						EXCCHECKCVI( SetCtrlAttribute(panel, POWER_LED_CONFIG_CUR_LIMIT, ATTR_FRAME_COLOR, VAL_GREEN));
						ProcessSystemEvents ();
					}
					else
					{
						EXCDISPLAY( pexception);   
						EXCCHECKCVI( SetCtrlAttribute(panel, POWER_LED_CONFIG_CUR_LIMIT, ATTR_FRAME_COLOR, VAL_RED));
					}
				break;	
	
				case POWER_CONFIG_OUT :
									
					GetCtrlVal(panel, POWER_VOLTAGE, &voltage);
					pexception = ppower->ConfigureOutput(ppower, voltage);	
					if(pexception==NULL) 
					{
						EXCCHECKCVI( SetCtrlAttribute(panel, POWER_LED_CONFIG_OUT, ATTR_FRAME_COLOR, VAL_GREEN));
						ProcessSystemEvents ();
					}
					else
					{
						EXCDISPLAY( pexception);   
						EXCCHECKCVI( SetCtrlAttribute(panel, POWER_LED_CONFIG_OUT, ATTR_FRAME_COLOR, VAL_RED));
					}
				break;		
						
				case POWER_OVP :
									
					GetCtrlVal(panel, POWER_OVP_LIMIT, &OVP_Value);
					pexception = ppower->ConfigureOVP(ppower, OVP_Value);	
					if(pexception==NULL) 
					{
						EXCCHECKCVI( SetCtrlAttribute(panel, POWER_LED_OVP, ATTR_FRAME_COLOR, VAL_GREEN));
						ProcessSystemEvents ();
					}
					else
					{
						EXCDISPLAY( pexception);   
						EXCCHECKCVI( SetCtrlAttribute(panel, POWER_LED_OVP, ATTR_FRAME_COLOR, VAL_RED));
					}
				break;		
							
				case POWER_OCP :
									
					GetCtrlVal(panel, POWER_OCP_LIMIT, &OCP_Value);
					pexception = ppower->ConfigureOCP(ppower, OCP_Value);	
					if(pexception==NULL) 
					{
						EXCCHECKCVI( SetCtrlAttribute(panel, POWER_LED_OCP, ATTR_FRAME_COLOR, VAL_GREEN));
						ProcessSystemEvents ();
					}
					else
					{
						EXCDISPLAY( pexception);   
						EXCCHECKCVI( SetCtrlAttribute(panel, POWER_LED_OCP, ATTR_FRAME_COLOR, VAL_RED));
					}
				break;	
								
				case POWER_ENABLE_OUT :
									
					GetCtrlVal(panel, POWER_OUTPUT, &output);
					pexception = ppower->EnableOutput(ppower, output);	
					if(pexception==NULL) 
					{
						if (output == TRUE)
							EXCCHECKCVI( SetCtrlAttribute(panel, POWER_LED_OUTPUT, ATTR_FRAME_COLOR, VAL_GREEN));
						else 
							EXCCHECKCVI( SetCtrlAttribute(panel, POWER_LED_OUTPUT, ATTR_FRAME_COLOR, VAL_WHITE));	
						ProcessSystemEvents ();
					}
					else
					{
						EXCDISPLAY( pexception);  
						EXCCHECKCVI( SetCtrlAttribute(panel, POWER_LED_OUTPUT, ATTR_FRAME_COLOR, VAL_RED));
					}
				break;	
			}
		}
		break;
	}

Error:
	EXCDISPLAY( pexception);
	return 0;
}