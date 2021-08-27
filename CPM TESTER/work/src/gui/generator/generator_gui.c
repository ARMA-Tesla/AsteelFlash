#include <exception/exception.h>
#include <generator/class/generators.h>
#include <labels/tester_labels.h>
#include <eventfactory/EventFactory.h>
#include "generator_gui_plugin.h"
#include <ansi_c.h>
#include "generator_gui.h"

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

	SetCtrlAttribute (handle, GEN_CONNECT, ATTR_DIMMED, TRUE);
	
	SetCtrlAttribute (handle, GEN_DISCONNECT, ATTR_DIMMED, FALSE);
	SetCtrlAttribute (handle, GEN_SEND_TEXT, ATTR_DIMMED, FALSE);
	SetCtrlAttribute (handle, GEN_READ_TEXT, ATTR_DIMMED, FALSE);
	SetCtrlAttribute (handle, GEN_SEND, ATTR_DIMMED, FALSE);
	SetCtrlAttribute (handle, GEN_READ, ATTR_DIMMED, FALSE);
	
	SetCtrlAttribute (handle, GEN_MODE_CONT, ATTR_DIMMED, FALSE);
	SetCtrlAttribute (handle, GEN_LED_MODE_CONT, ATTR_DIMMED, FALSE);
	
	SetCtrlAttribute (handle, GEN_SET_OUTPUT, ATTR_DIMMED, FALSE);	
	SetCtrlAttribute (handle, GEN_OUTPUT, ATTR_DIMMED, FALSE);
		
	SetCtrlAttribute (handle, GEN_SET_FUNCTION, ATTR_DIMMED, FALSE);	
	SetCtrlAttribute (handle, GEN_LED_SET_FUNCTION, ATTR_DIMMED, FALSE);
	SetCtrlAttribute (handle, GEN_SET_FREQUENCY, ATTR_DIMMED, FALSE);
	SetCtrlAttribute (handle, GEN_SET_AMPLITUDE, ATTR_DIMMED, FALSE);
	SetCtrlAttribute (handle, GEN_SET_OFFSET, ATTR_DIMMED, FALSE);
	SetCtrlAttribute (handle, GEN_SET_DUTY, ATTR_DIMMED, FALSE);
			
	SetCtrlAttribute (handle, GEN_SYSTEM_ERROR, ATTR_DIMMED, FALSE);
	SetCtrlAttribute (handle, GEN_CLEAR_ERROR, ATTR_DIMMED, FALSE);	
	SetCtrlAttribute (handle, GEN_ERROR, ATTR_DIMMED, FALSE);
}

void Desactivate_Controls(int handle)
{
	int group = 0;
	int i;

	
	SetCtrlAttribute (handle, GEN_CONNECT, ATTR_DIMMED, FALSE);
	
	SetCtrlAttribute (handle, GEN_DISCONNECT, ATTR_DIMMED, TRUE);
	SetCtrlAttribute (handle, GEN_SEND_TEXT, ATTR_DIMMED, TRUE);
	SetCtrlAttribute (handle, GEN_READ_TEXT, ATTR_DIMMED, TRUE);
	SetCtrlAttribute (handle, GEN_SEND, ATTR_DIMMED, TRUE);
	SetCtrlAttribute (handle, GEN_READ, ATTR_DIMMED, TRUE);
	
	SetCtrlAttribute (handle, GEN_MODE_CONT, ATTR_DIMMED, TRUE);
	SetCtrlAttribute (handle, GEN_LED_MODE_CONT, ATTR_DIMMED, TRUE);
	
	SetCtrlAttribute (handle, GEN_SET_OUTPUT, ATTR_DIMMED, TRUE);	
	SetCtrlAttribute (handle, GEN_OUTPUT, ATTR_DIMMED, TRUE);
	
	SetCtrlAttribute (handle, GEN_SET_FUNCTION, ATTR_DIMMED, TRUE);	
	SetCtrlAttribute (handle, GEN_LED_SET_FUNCTION, ATTR_DIMMED, TRUE);
	SetCtrlAttribute (handle, GEN_SET_FREQUENCY, ATTR_DIMMED, TRUE);
	SetCtrlAttribute (handle, GEN_SET_AMPLITUDE, ATTR_DIMMED, TRUE);
	SetCtrlAttribute (handle, GEN_SET_OFFSET, ATTR_DIMMED, TRUE);
	SetCtrlAttribute (handle, GEN_SET_DUTY, ATTR_DIMMED, TRUE);
	
	SetCtrlAttribute (handle, GEN_SYSTEM_ERROR, ATTR_DIMMED, TRUE);
	SetCtrlAttribute (handle, GEN_CLEAR_ERROR, ATTR_DIMMED, TRUE);	
	SetCtrlAttribute (handle, GEN_ERROR, ATTR_DIMMED, TRUE);
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
#define __FUNC__ "{Generator Plugin}::Initialize"
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

	EXCCHECKCVI( gs_panel[GEN] = LoadPanelEx(main_panel,"generator_gui.uir", GEN, __CVIUserHInst) );

	EXCCHECKCVI( SetPanelAttribute (gs_panel[GEN], ATTR_TOP, 0));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[GEN], ATTR_LEFT, 0));

Error:
   	EXCRETHROW( pexception);
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Generator Plugin}::Run"
PLUGIN_API SElExceptionPtr PLUGIN_RUN_NAME_CODE(SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
	SElExceptionPtr	pexception = NULL;  
	int32_t			error = 0;
	 
	if( 0==strcmp( uuid, PLUGIN_GENERATOR))
	{
		EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
	                        			"Modules",
	                        			sizeof(SObjListPtr),
	                        			&gs_Modules));
		
		EXCCHECKCVI( DisplayPanel(gs_panel[GEN]));
	}

Error:   
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Generator Plugin}::Leave"
PLUGIN_API SElExceptionPtr PLUGIN_LEAVE_NAME_CODE( SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;

	EXCCHECKCVI( HidePanel(gs_panel[GEN]));

Error:   
	EXCRETHROW( pexception);

}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Generator Plugin}::Shutdown"
PLUGIN_API SElExceptionPtr PLUGIN_SHUTDOWN_NAME_CODE( SPluginArgsPtr a_pPluginArgs)
{
   	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;

	if(gs_panel[GEN])
	EXCCHECKCVI( DiscardPanel( gs_panel[GEN]));

Error:
	EXCRETHROW( pexception);
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Generator Plugin}::Info"
PLUGIN_API SElExceptionPtr PLUGIN_GETINFO_NAME_CODE( SPluginInfoPtr* a_pInfo)
{
   	SElExceptionPtr	pexception = NULL;  
	
	if (a_pInfo) EXCCHECK( (*a_pInfo)->SetInfoData(*a_pInfo, &gs_InfoData));
	
Error:	
	EXCRETHROW( pexception);
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Generator Plugin}::Attribute"
PLUGIN_API SElExceptionPtr PLUGIN_GETATTR_NAME_CODE( uint32_t  attrID, uint32_t size, void* value)
{
   return NULL;
}

#endif



int CVICALLBACK OnGen (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr	pexception = NULL;
	int32_t	 error = 0;
	char	comand[1024] = "";
	char	data[1024] = "";
	char	log[128] = "";
	int 	output;			
	double  frequency;
	double  amplitude;
	double  offset;
	double  duty;
	char	sys_error[128] = "";
	
	switch (event)
	{
		case EVENT_COMMIT:
		{	
			SGeneratorsPtr	pgenerators = (SGeneratorsPtr)gs_Modules->Get(gs_Modules, MODULE_GENERATORS);  
			SGeneratorPtr		pgenerator = NULL;
							
			
			EXCCHECK( pgenerators->GetGeneratorByIndex(pgenerators, 0, &pgenerator));
			
			
			switch (control)
			{
				
				case GEN_CONNECT :

					pexception = pgenerator->Connect(pgenerator);
					if(pexception==NULL) 
					{
						EXCCHECKCVI( SetCtrlAttribute(panel, GEN_LED_CONNECT,ATTR_FRAME_COLOR, VAL_GREEN));
						Activate_Controls (panel);
						ProcessSystemEvents ();
					}
					else
					{
						EXCDISPLAY( pexception);   
						EXCCHECKCVI( SetCtrlAttribute(panel, GEN_LED_CONNECT,ATTR_FRAME_COLOR, VAL_RED));
					}				
				
				break;
				
				
				case GEN_DISCONNECT :
							

					pexception = pgenerator->Disconnect(pgenerator);
					EXCCHECKCVI( SetCtrlAttribute(panel, GEN_LED_CONNECT,ATTR_FRAME_COLOR, VAL_WHITE));	
					Desactivate_Controls (panel);
					ProcessSystemEvents ();
		
				break;
					
				
				case GEN_SEND :
								
					GetCtrlVal(panel, GEN_SEND_TEXT, comand);
					EXCCHECK( pgenerator->Write(pgenerator, comand));			
				
				break;
				
				
				case GEN_READ :

					EXCCHECK( pgenerator->Read(pgenerator, data));	
					SetCtrlVal(panel, GEN_READ_TEXT, data);		
				
				break;	
				
				case GEN_MODE_CONT :
					
					pexception = pgenerator->SetModeContinuous(pgenerator);	
					if(pexception==NULL) 
					{
						EXCCHECKCVI( SetCtrlAttribute(panel, GEN_LED_MODE_CONT, ATTR_FRAME_COLOR, VAL_GREEN));
						ProcessSystemEvents ();
					}
					else
					{
						EXCDISPLAY( pexception);   
						EXCCHECKCVI( SetCtrlAttribute(panel, GEN_LED_MODE_CONT, ATTR_FRAME_COLOR, VAL_RED));
					}
				break;	
	
				
				case GEN_SET_OUTPUT :
					
					GetCtrlVal(panel, GEN_OUTPUT, &output);
					pexception = pgenerator->SetOutput(pgenerator, output);	
					if(pexception==NULL) 
					{
						EXCCHECKCVI( SetCtrlAttribute(panel, GEN_LED_OUTPUT, ATTR_FRAME_COLOR, VAL_GREEN));
						ProcessSystemEvents ();
					}
					else
					{
						EXCDISPLAY( pexception);   
						EXCCHECKCVI( SetCtrlAttribute(panel, GEN_LED_OUTPUT, ATTR_FRAME_COLOR, VAL_RED));
					}
				break;	
				
				case GEN_SET_FUNCTION :
				
					GetCtrlVal(panel, GEN_SET_FREQUENCY, &frequency);
					GetCtrlVal(panel, GEN_SET_AMPLITUDE, &amplitude);
					GetCtrlVal(panel, GEN_SET_OFFSET, &offset);
					GetCtrlVal(panel, GEN_SET_DUTY, &duty);
					pexception = pgenerator->SetFonction(pgenerator, frequency, amplitude, offset, duty);	
					if(pexception==NULL) 
					{
						EXCCHECKCVI( SetCtrlAttribute(panel, GEN_LED_SET_FUNCTION, ATTR_FRAME_COLOR, VAL_GREEN));
						ProcessSystemEvents ();
					}
					else
					{
						EXCDISPLAY( pexception);   
						EXCCHECKCVI( SetCtrlAttribute(panel, GEN_LED_SET_FUNCTION, ATTR_FRAME_COLOR, VAL_RED));
					}
				break;	
				
				case GEN_SYSTEM_ERROR :
				
					DefaultCtrl(panel, GEN_ERROR);
					pexception = pgenerator->SystemError(pgenerator, sys_error);	
					if(pexception==NULL) 
					{
						SetCtrlVal(panel, GEN_ERROR, sys_error);
						ProcessSystemEvents ();
					}
					else
					{
						EXCDISPLAY( pexception);   
					}
					
				break;	
				
				case GEN_CLEAR_ERROR:
				
					pexception = pgenerator->ClearError(pgenerator);	
					if(pexception) 
					{
						EXCDISPLAY( pexception);   
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