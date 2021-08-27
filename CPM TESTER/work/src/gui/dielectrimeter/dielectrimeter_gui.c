#include <exception/exception.h>
#include <dielectrimeter/class/dielectrimeters.h>
#include <labels/tester_labels.h>
#include <eventfactory/EventFactory.h>
#include "dielectrimeter_gui_plugin.h"
#include <utility.h>
#include <ansi_c.h>
#include "dielectrimeter_gui.h"

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

	SetCtrlAttribute (handle, DIELECT_CONNECT, ATTR_DIMMED, TRUE);
	
	SetCtrlAttribute (handle, DIELECT_DISCONNECT, ATTR_DIMMED, FALSE);
	SetCtrlAttribute (handle, DIELECT_SEND_TEXT, ATTR_DIMMED, FALSE);
	SetCtrlAttribute (handle, DIELECT_READ_TEXT, ATTR_DIMMED, FALSE);
	SetCtrlAttribute (handle, DIELECT_SEND, ATTR_DIMMED, FALSE);
	SetCtrlAttribute (handle, DIELECT_READ, ATTR_DIMMED, FALSE);
	
	SetCtrlAttribute (handle, DIELECT_MODE_MAN, ATTR_DIMMED, FALSE);
	SetCtrlAttribute (handle, DIELECT_LED_MODE_MAN, ATTR_DIMMED, FALSE);
	
	SetCtrlAttribute (handle, DIELECT_SET_STEP, ATTR_DIMMED, FALSE);
	SetCtrlAttribute (handle, DIELECT_STEP, ATTR_DIMMED, FALSE);	
	
	SetCtrlAttribute (handle, DIELECT_SET_OUTPUT, ATTR_DIMMED, FALSE);	
	SetCtrlAttribute (handle, DIELECT_OUTPUT, ATTR_DIMMED, FALSE);
		
	SetCtrlAttribute (handle, DIELECT_TEST_RESULT, ATTR_DIMMED, FALSE);	
	SetCtrlAttribute (handle, DIELECT_LED_TEST_RESULT, ATTR_DIMMED, FALSE);
	SetCtrlAttribute (handle, DIELECT_TEST_VOLTAGE, ATTR_DIMMED, FALSE);
	SetCtrlAttribute (handle, DIELECT_TEST_CURRENT, ATTR_DIMMED, FALSE);
	
	SetCtrlAttribute (handle, DIELECT_SYSTEM_ERROR, ATTR_DIMMED, FALSE);
	SetCtrlAttribute (handle, DIELECT_CLEAR_ERROR, ATTR_DIMMED, FALSE);	
	SetCtrlAttribute (handle, DIELECT_ERROR, ATTR_DIMMED, FALSE);
}

void Desactivate_Controls(int handle)
{
	int group = 0;
	int i;

	
	SetCtrlAttribute (handle, DIELECT_CONNECT, ATTR_DIMMED, FALSE);
	
	SetCtrlAttribute (handle, DIELECT_DISCONNECT, ATTR_DIMMED, TRUE);
	SetCtrlAttribute (handle, DIELECT_SEND_TEXT, ATTR_DIMMED, TRUE);
	SetCtrlAttribute (handle, DIELECT_READ_TEXT, ATTR_DIMMED, TRUE);
	SetCtrlAttribute (handle, DIELECT_SEND, ATTR_DIMMED, TRUE);
	SetCtrlAttribute (handle, DIELECT_READ, ATTR_DIMMED, TRUE);
	
	SetCtrlAttribute (handle, DIELECT_MODE_MAN, ATTR_DIMMED, TRUE);
	SetCtrlAttribute (handle, DIELECT_LED_MODE_MAN, ATTR_DIMMED, TRUE);
	
	SetCtrlAttribute (handle, DIELECT_SET_STEP, ATTR_DIMMED, TRUE);
	SetCtrlAttribute (handle, DIELECT_STEP, ATTR_DIMMED, TRUE);	
	
	SetCtrlAttribute (handle, DIELECT_SET_OUTPUT, ATTR_DIMMED, TRUE);	
	SetCtrlAttribute (handle, DIELECT_OUTPUT, ATTR_DIMMED, TRUE);
		
	SetCtrlAttribute (handle, DIELECT_TEST_RESULT, ATTR_DIMMED, TRUE);	
	SetCtrlAttribute (handle, DIELECT_LED_TEST_RESULT, ATTR_DIMMED, TRUE);
	SetCtrlAttribute (handle, DIELECT_TEST_VOLTAGE, ATTR_DIMMED, TRUE);
	SetCtrlAttribute (handle, DIELECT_TEST_CURRENT, ATTR_DIMMED, TRUE);
	
	SetCtrlAttribute (handle, DIELECT_SYSTEM_ERROR, ATTR_DIMMED, TRUE);
	SetCtrlAttribute (handle, DIELECT_CLEAR_ERROR, ATTR_DIMMED, TRUE);	
	SetCtrlAttribute (handle, DIELECT_ERROR, ATTR_DIMMED, TRUE);
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
#define __FUNC__ "{Dielectrimeter Plugin}::Initialize"
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

	EXCCHECKCVI( gs_panel[DIELECT] = LoadPanelEx(main_panel,"dielectrimeter_gui.uir", DIELECT, __CVIUserHInst) );

	EXCCHECKCVI( SetPanelAttribute (gs_panel[DIELECT], ATTR_TOP, 0));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[DIELECT], ATTR_LEFT, 0));

Error:
   	EXCRETHROW( pexception);
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Dielectrimeter Plugin}::Run"
PLUGIN_API SElExceptionPtr PLUGIN_RUN_NAME_CODE(SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
	SElExceptionPtr	pexception = NULL;  
	int32_t			error = 0;
	 
	if( 0==strcmp( uuid, PLUGIN_DIELECTRIMETER))
	{
		EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
	                        			"Modules",
	                        			sizeof(SObjListPtr),
	                        			&gs_Modules));
		
		EXCCHECKCVI( DisplayPanel(gs_panel[DIELECT]));
	}

Error:   
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Dielectrimeter Plugin}::Leave"
PLUGIN_API SElExceptionPtr PLUGIN_LEAVE_NAME_CODE( SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;

	EXCCHECKCVI( HidePanel(gs_panel[DIELECT]));

Error:   
	EXCRETHROW( pexception);

}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Dielectrimeter Plugin}::Shutdown"
PLUGIN_API SElExceptionPtr PLUGIN_SHUTDOWN_NAME_CODE( SPluginArgsPtr a_pPluginArgs)
{
   	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;

	if(gs_panel[DIELECT])
	EXCCHECKCVI( DiscardPanel( gs_panel[DIELECT]));

Error:
	EXCRETHROW( pexception);
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Dielectrimeter Plugin}::Info"
PLUGIN_API SElExceptionPtr PLUGIN_GETINFO_NAME_CODE( SPluginInfoPtr* a_pInfo)
{
   	SElExceptionPtr	pexception = NULL;  
	
	if (a_pInfo) EXCCHECK( (*a_pInfo)->SetInfoData(*a_pInfo, &gs_InfoData));
	
Error:	
	EXCRETHROW( pexception);
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Dielectrimeter Plugin}::Attribute"
PLUGIN_API SElExceptionPtr PLUGIN_GETATTR_NAME_CODE( uint32_t  attrID, uint32_t size, void* value)
{
   return NULL;
}

#endif



int CVICALLBACK OnDielectric (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr	pexception = NULL;
	int32_t	 error = 0;
	char	comand[1024] = "";
	char	data[1024] = "";
	char	log[128] = "";
	int 	step;			
	int 	output;			
	int 	result;
	double  voltage;
	double  current;
	char	sys_error[128] = "";
	
	switch (event)
	{
		case EVENT_COMMIT:
		{	
			SDielectrimetersPtr	pdielectrimeters = (SDielectrimetersPtr)gs_Modules->Get(gs_Modules, MODULE_DIELECTRIMETERS);  
			SDielectrimeterPtr		pdielectrimeter = NULL;
							
			
			EXCCHECK( pdielectrimeters->GetDielectrimeterByIndex(pdielectrimeters, 0, &pdielectrimeter));
			
			
			switch (control)
			{
				
				case DIELECT_CONNECT :

					pexception = pdielectrimeter->Connect(pdielectrimeter);
					if(pexception==NULL) 
					{
						EXCCHECKCVI( SetCtrlAttribute(panel, DIELECT_LED_CONNECT,ATTR_FRAME_COLOR, VAL_GREEN));
						Activate_Controls (panel);
						ProcessSystemEvents ();
					}
					else
					{
						EXCDISPLAY( pexception);   
						EXCCHECKCVI( SetCtrlAttribute(panel, DIELECT_LED_CONNECT,ATTR_FRAME_COLOR, VAL_RED));
					}				
				
				break;
				
				
				case DIELECT_DISCONNECT :
							

					pexception = pdielectrimeter->Disconnect(pdielectrimeter);
					EXCCHECKCVI( SetCtrlAttribute(panel, DIELECT_LED_CONNECT,ATTR_FRAME_COLOR, VAL_WHITE));	
					Desactivate_Controls (panel);
					ProcessSystemEvents ();
		
				break;
					
				
				case DIELECT_SEND :
								
					GetCtrlVal(panel, DIELECT_SEND_TEXT, comand);
					EXCCHECK( pdielectrimeter->Write(pdielectrimeter, comand));			
				
				break;
				
				
				case DIELECT_READ :

					EXCCHECK( pdielectrimeter->Read(pdielectrimeter, data));	
					SetCtrlVal(panel, DIELECT_READ_TEXT, data);		
				
				break;	
				
				case DIELECT_MODE_MAN :
					
					pexception = pdielectrimeter->SetModeManual(pdielectrimeter);	
					if(pexception==NULL) 
					{
						EXCCHECKCVI( SetCtrlAttribute(panel, DIELECT_LED_MODE_MAN, ATTR_FRAME_COLOR, VAL_GREEN));
						ProcessSystemEvents ();
					}
					else
					{
						EXCDISPLAY( pexception);   
						EXCCHECKCVI( SetCtrlAttribute(panel, DIELECT_LED_MODE_MAN, ATTR_FRAME_COLOR, VAL_RED));
					}
				break;	
							
				case DIELECT_SET_STEP :
					
					GetCtrlVal(panel, DIELECT_STEP, &step);
					pexception = pdielectrimeter->SetStep(pdielectrimeter, step);	
					if(pexception==NULL) 
					{
						EXCCHECKCVI( SetCtrlAttribute(panel, DIELECT_LED_STEP, ATTR_FRAME_COLOR, VAL_GREEN));
						ProcessSystemEvents ();
					}
					else
					{
						EXCDISPLAY( pexception);   
						EXCCHECKCVI( SetCtrlAttribute(panel, DIELECT_LED_STEP, ATTR_FRAME_COLOR, VAL_RED));
					}
					
				break;		
				
				case DIELECT_SET_OUTPUT :
					
					GetCtrlVal(panel, DIELECT_OUTPUT, &output);
					pexception = pdielectrimeter->SetOutput(pdielectrimeter, output);	
					if(pexception==NULL) 
					{
						EXCCHECKCVI( SetCtrlAttribute(panel, DIELECT_LED_OUTPUT, ATTR_FRAME_COLOR, VAL_GREEN));
						Delay(1);
						EXCCHECK( pdielectrimeter->Read(pdielectrimeter, data));	
					    SetCtrlVal(panel, DIELECT_READ_TEXT, data);
						ProcessSystemEvents ();
					}
					else
					{
						Delay(1);
						EXCCHECK( pdielectrimeter->Read(pdielectrimeter, data));	
					    SetCtrlVal(panel, DIELECT_READ_TEXT, data);
						EXCDISPLAY( pexception);   
						EXCCHECKCVI( SetCtrlAttribute(panel, DIELECT_LED_OUTPUT, ATTR_FRAME_COLOR, VAL_RED));
					}
					   
				break;	
				
				case DIELECT_TEST_RESULT :
				
					pexception = pdielectrimeter->TestResult(pdielectrimeter, &result, &voltage, &current);	
					if(pexception==NULL) 
					{
						EXCCHECKCVI( SetCtrlAttribute(panel, DIELECT_LED_TEST_RESULT, ATTR_FRAME_COLOR, result==1? VAL_GREEN:VAL_RED ));
						SetCtrlVal(panel, DIELECT_TEST_VOLTAGE, voltage);
						SetCtrlVal(panel, DIELECT_TEST_CURRENT, current);
						EXCCHECK( pdielectrimeter->Read(pdielectrimeter, data));	
					    SetCtrlVal(panel, DIELECT_READ_TEXT, data);
						ProcessSystemEvents ();
					}
					else
					{
						EXCDISPLAY( pexception);   
						EXCCHECKCVI( SetCtrlAttribute(panel, DIELECT_LED_OUTPUT, ATTR_FRAME_COLOR, VAL_RED));
					}
				break;	
				
				case DIELECT_SYSTEM_ERROR :
				
					DefaultCtrl(panel, DIELECT_ERROR);
					pexception = pdielectrimeter->SystemError(pdielectrimeter, sys_error);	
					if(pexception==NULL) 
					{
						SetCtrlVal(panel, DIELECT_ERROR, sys_error);
						ProcessSystemEvents ();
					}
					else
					{
						EXCDISPLAY( pexception);   
					}
					
				break;	
				
				case DIELECT_CLEAR_ERROR:
				
					pexception = pdielectrimeter->ClearError(pdielectrimeter);	
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