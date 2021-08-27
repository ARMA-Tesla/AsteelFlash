#include <exception/exception.h>
#include <printer/class/printers.h>
#include <labels/tester_labels.h>
#include <eventfactory/EventFactory.h>
#include "printer_gui_plugin.h"
#include <ansi_c.h>
#include "printer_gui.h"

#define NB_OF_PANELS		1 
#define NAME_LENGTH			128

/*GLOBALS*/
static SObjListPtr     		gs_Modules = NULL;
static SElEventFactoryPtr   gs_EventFactory = NULL;  
static int					gs_panel[NB_OF_PANELS+1];
static SPluginInfoPtr		gs_pPI = NULL; 

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
#define __FUNC__ "{Printer Plugin}::Initialize"
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

	EXCCHECKCVI( gs_panel[PRINTER] = LoadPanelEx(main_panel,"printer_gui.uir", PRINTER, __CVIUserHInst) );

	EXCCHECKCVI( SetPanelAttribute (gs_panel[PRINTER], ATTR_TOP, 0));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[PRINTER], ATTR_LEFT, 0));

Error:
   	EXCRETHROW( pexception);
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Printer Plugin}::Run"
PLUGIN_API SElExceptionPtr PLUGIN_RUN_NAME_CODE(SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
	SElExceptionPtr	pexception = NULL;  
	int32_t			error = 0;
	 
	if( 0==strcmp( uuid, PLUGIN_PRINTER))
	{
		EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
	                        			"Modules",
	                        			sizeof(SObjListPtr),
	                        			&gs_Modules));
		
		EXCCHECKCVI( DisplayPanel(gs_panel[PRINTER]));
	}

Error:   
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Printer Plugin}::Leave"
PLUGIN_API SElExceptionPtr PLUGIN_LEAVE_NAME_CODE( SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;

	EXCCHECKCVI( HidePanel(gs_panel[PRINTER]));

Error:   
	EXCRETHROW( pexception);

}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Printer Plugin}::Shutdown"
PLUGIN_API SElExceptionPtr PLUGIN_SHUTDOWN_NAME_CODE( SPluginArgsPtr a_pPluginArgs)
{
   	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;

	if(gs_panel[PRINTER])
	EXCCHECKCVI( DiscardPanel( gs_panel[PRINTER]));

Error:
	EXCRETHROW( pexception);
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Printer Plugin}::Info"
PLUGIN_API SElExceptionPtr PLUGIN_GETINFO_NAME_CODE( SPluginInfoPtr* a_pInfo)
{
   	SElExceptionPtr	pexception = NULL;  
	
	if (a_pInfo) EXCCHECK( (*a_pInfo)->SetInfoData(*a_pInfo, &gs_InfoData));
	
Error:	
	EXCRETHROW( pexception);
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Printer Plugin}::Attribute"
PLUGIN_API SElExceptionPtr PLUGIN_GETATTR_NAME_CODE( uint32_t  attrID, uint32_t size, void* value)
{
   return NULL;
}

#endif

int CVICALLBACK OnPrint (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr	pexception = NULL;  
	char	buffer[4096] = "";
	char	text[4096] = "";
	int		count;
	
	switch (event)
	{
		case EVENT_COMMIT:
		if(gs_Modules)
		{
			SPrintersPtr	pprinters = (SPrintersPtr)gs_Modules->Get(gs_Modules, MODULE_PRINTERS);  
			SLabelListPtr	plabels = (SLabelListPtr)gs_Modules->Get(gs_Modules, MODULE_LABELS);  
			SPrinterPtr		pprinter = NULL;
			
			GetCtrlAttribute (panel, PRINTER_TEXTBOX, ATTR_STRING_TEXT_LENGTH, &count);
			if ( count >= sizeof(text))
			{
				EXCTHROW( -1, "Small Buffer for printer label");
			}

			GetCtrlVal(panel, PRINTER_TEXTBOX, text);
			EXCCHECK( pprinters->GetPrinterByIndex(pprinters, 0, &pprinter));
			EXCCHECK( pprinter->Write(pprinter, text));
				
#if 0    /* formerly excluded lines */
			if ( pprinters && plabels )
			{
				char*	pend = buffer;

				EXCCHECK( plabels->ParseLabelToBuffer( plabels, text, 0, NULL, buffer, sizeof(buffer)));
				EXCCHECK( pprinters->GetPrinterByIndex(pprinters, 0, &pprinter));

				pend = strchr( pend, '\n');
				while ( pend != NULL )
				{	
					if ( pend > buffer && *(pend-1)!='\r' )
					{
						memmove(pend+1, pend, strlen(pend)+1);
						*pend = '\r';
						pend += 2;
					}
					else if(pend<buffer+sizeof(buffer))
					{
						pend++;
					}
					pend = strchr( pend, '\n');
				};
				
				EXCCHECK( pprinter->Write(pprinter, buffer));
			}
		
		 	else if ( pprinters )
			{

				EXCCHECK( pprinters->GetPrinterByIndex(pprinters, 0, &pprinter));
				EXCCHECK( pprinter->Write(pprinter, text));
			}
#endif   /* formerly excluded lines */

		}			
	
		break;
	}
Error:
	EXCDISPLAY( pexception);
	return 0;
}


int CVICALLBACK OnStatus (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr	pexception = NULL;  
	char	buffer[4096] = "";
	char	text[4096] = "";
	int		count;
	int     Status;
	char	Error[128] = "";
	char	sAmmount[128] = "";
	int		StatusOnline;
	int		AmmountLabels;
	int		Interpreter;
	
	switch (event)
	{
		case EVENT_COMMIT:
		if(gs_Modules)
		{
			SPrintersPtr	pprinters = (SPrintersPtr)gs_Modules->Get(gs_Modules, MODULE_PRINTERS);  
			SLabelListPtr	plabels = (SLabelListPtr)gs_Modules->Get(gs_Modules, MODULE_LABELS);  
			SPrinterPtr		pprinter = NULL;
			
			GetCtrlAttribute (panel, PRINTER_TEXTBOX, ATTR_STRING_TEXT_LENGTH, &count);
			if ( count >= sizeof(text))
			{
				EXCTHROW( -1, "Small Buffer for printer label");
			}

			GetCtrlVal(panel, PRINTER_TEXTBOX, text);
			
			if ( pprinters )
			{
				char*	pend = buffer;

				EXCCHECK( pprinters->GetPrinterByIndex(pprinters, 0, &pprinter));

				EXCCHECK( pprinter->Status(pprinter, &StatusOnline, Error, &AmmountLabels, &Interpreter));
				
				if (StatusOnline == 1)
					SetTableCellVal (panel, PRINTER_TABLE, MakePoint (2, 1), "Online");
				else if (StatusOnline == 0)
					SetTableCellVal (panel, PRINTER_TABLE, MakePoint (2, 1), "Offline");
					
				SetTableCellVal (panel, PRINTER_TABLE, MakePoint (2, 2), Error);
				
				sprintf(sAmmount, "%06d", AmmountLabels);
				SetTableCellVal (panel, PRINTER_TABLE, MakePoint (2, 3), sAmmount);
				
				if (Interpreter == 1)
					SetTableCellVal (panel, PRINTER_TABLE, MakePoint (2, 4), "print job is in process");
				else if (Interpreter == 0)
					SetTableCellVal (panel, PRINTER_TABLE, MakePoint (2, 4), "printer in Standby mode)");
				
				
			}
		}
	break;
	
	}
Error:
	EXCDISPLAY( pexception);
	return 0;
}


int CVICALLBACK OnReset (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr	pexception = NULL;  
	char	buffer[4096] = "";
	char	text[4096] = "";
	int		count;
	int     Status;
	
	switch (event)
	{
		case EVENT_COMMIT:
		if(gs_Modules)
		{
			SPrintersPtr	pprinters = (SPrintersPtr)gs_Modules->Get(gs_Modules, MODULE_PRINTERS);  
			SPrinterPtr		pprinter = NULL;
			
			GetCtrlAttribute (panel, PRINTER_TEXTBOX, ATTR_STRING_TEXT_LENGTH, &count);
			if ( count >= sizeof(text))
			{
				EXCTHROW( -1, "Small Buffer for printer label");
			}

			GetCtrlVal(panel, PRINTER_TEXTBOX, text);
			
			if ( pprinters )
			{
				char*	pend = buffer;

				EXCCHECK( pprinters->GetPrinterByIndex(pprinters, 0, &pprinter));

				EXCCHECK( pprinter->Reset(pprinter));
			}
		}
	break;
	
	}
Error:
	EXCDISPLAY( pexception);
	return 0;
}


	
int CVICALLBACK OnConnect (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr	pexception = NULL;  
	char	buffer[4096] = "";
	char	text[4096] = "";
	int		count;
	int     Status;
	int32_t				error = 0;

	switch (event)
	{
		case EVENT_COMMIT:
		if(gs_Modules)
		{
			SPrintersPtr	pprinters = (SPrintersPtr)gs_Modules->Get(gs_Modules, MODULE_PRINTERS);  
			SPrinterPtr		pprinter = NULL;
			
			GetCtrlAttribute (panel, PRINTER_TEXTBOX, ATTR_STRING_TEXT_LENGTH, &count);
			if ( count >= sizeof(text))
			{
				EXCTHROW( -1, "Small Buffer for printer label");
			}

			GetCtrlVal(panel, PRINTER_TEXTBOX, text);
			
			if ( pprinters )
			{
				char*	pend = buffer;

				EXCCHECK( pprinters->GetPrinterByIndex(pprinters, 0, &pprinter));

				
				pexception = pprinter->Connect(pprinter);
		
				if(pexception==NULL) 
					{
						EXCCHECKCVI( SetCtrlAttribute(panel, PRINTER_LED_CONNECT,ATTR_FRAME_COLOR, VAL_GREEN));
						ProcessSystemEvents ();
					}
					else
					{
						EXCDISPLAY( pexception);   
						EXCCHECKCVI( SetCtrlAttribute(panel, PRINTER_LED_CONNECT,ATTR_FRAME_COLOR, VAL_RED));
					}

			}
		}
		break;
	
	}
Error:
	EXCDISPLAY( pexception);
	return 0;
}
int CVICALLBACK OnDisconnect (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr	pexception = NULL;  
	char	buffer[4096] = "";
	char	text[4096] = "";
	int		count;
	int     Status;
	int32_t	error = 0;
	
	switch (event)
	{
		case EVENT_COMMIT:
		if(gs_Modules)
		{
			SPrintersPtr	pprinters = (SPrintersPtr)gs_Modules->Get(gs_Modules, MODULE_PRINTERS);  
			SPrinterPtr		pprinter = NULL;
			
			GetCtrlAttribute (panel, PRINTER_TEXTBOX, ATTR_STRING_TEXT_LENGTH, &count);
			if ( count >= sizeof(text))
			{
				EXCTHROW( -1, "Small Buffer for printer label");
			}

			GetCtrlVal(panel, PRINTER_TEXTBOX, text);
			
			if ( pprinters )
			{
				char*	pend = buffer;

				EXCCHECK( pprinters->GetPrinterByIndex(pprinters, 0, &pprinter));

				pexception = pprinter->Disconnect(pprinter);
				EXCCHECKCVI( SetCtrlAttribute(panel, PRINTER_LED_CONNECT,ATTR_FRAME_COLOR, VAL_WHITE));
				
		
			}
		}
		break;
	
	}
Error:
	EXCDISPLAY( pexception);
	return 0;
}

