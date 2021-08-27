#include <exception/exception.h>
#include <traca/class/tracas.h>
#include <labels/tester_labels.h>
#include <eventfactory/EventFactory.h>
#include "traca_gui_plugin.h"
#include <ansi_c.h>
#include "traca_gui.h"

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
#define __FUNC__ "{Traca Plugin}::Initialize"
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

	EXCCHECKCVI( gs_panel[TRACA] = LoadPanelEx(main_panel,"traca_gui.uir", TRACA, __CVIUserHInst) );

	EXCCHECKCVI( SetPanelAttribute (gs_panel[TRACA], ATTR_TOP, 0));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[TRACA], ATTR_LEFT, 0));

	EXCCHECKCVI(SetPanelAttribute (gs_panel[TRACA], ATTR_SCALE_CONTENTS_ON_RESIZE, TRUE));
		
Error:
   	EXCRETHROW( pexception);
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Traca Plugin}::Run"
PLUGIN_API SElExceptionPtr PLUGIN_RUN_NAME_CODE(SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
	SElExceptionPtr	pexception = NULL;  
	int32_t			error = 0;
	 
	if( 0==strcmp( uuid, PLUGIN_TRACA))
	{
		EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
	                        			"Modules",
	                        			sizeof(SObjListPtr),
	                        			&gs_Modules));
		
		EXCCHECKCVI( DisplayPanel(gs_panel[TRACA]));
	}

Error:   
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Traca Plugin}::Leave"
PLUGIN_API SElExceptionPtr PLUGIN_LEAVE_NAME_CODE( SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;

	EXCCHECKCVI( HidePanel(gs_panel[TRACA]));

Error:   
	EXCRETHROW( pexception);

}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Traca Plugin}::Shutdown"
PLUGIN_API SElExceptionPtr PLUGIN_SHUTDOWN_NAME_CODE( SPluginArgsPtr a_pPluginArgs)
{
   	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;

	if(gs_panel[TRACA])
	EXCCHECKCVI( DiscardPanel( gs_panel[TRACA]));

Error:
	EXCRETHROW( pexception);
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Traca Plugin}::Info"
PLUGIN_API SElExceptionPtr PLUGIN_GETINFO_NAME_CODE( SPluginInfoPtr* a_pInfo)
{
   	SElExceptionPtr	pexception = NULL;  
	
	if (a_pInfo) EXCCHECK( (*a_pInfo)->SetInfoData(*a_pInfo, &gs_InfoData));
	
Error:	
	EXCRETHROW( pexception);
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Traca Plugin}::Attribute"
PLUGIN_API SElExceptionPtr PLUGIN_GETATTR_NAME_CODE( uint32_t  attrID, uint32_t size, void* value)
{
   return NULL;
}

#endif

int CVICALLBACK OnConnection (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr	pexception = NULL; 
 	int32_t				error = 0;
	
	switch (event)
	{
		case EVENT_COMMIT:
		if(gs_Modules)
		{
			STracasPtr	ptracas = (STracasPtr)gs_Modules->Get(gs_Modules, MODULE_TRACAS);  
			STracaPtr		ptraca = NULL;
				
			EXCCHECK( ptracas->GetTracaByIndex(ptracas, 0, &ptraca));
			
			switch (control)
				{
					case TRACA_CONNECT :
						pexception = ptraca->Connect(ptraca);	
						if(pexception==NULL) 
							{
								EXCCHECKCVI( SetCtrlAttribute(panel, TRACA_LED_CONNECT, ATTR_FRAME_COLOR, VAL_GREEN));
								ProcessSystemEvents ();
							}
						else
							{
								EXCDISPLAY( pexception);   					 
								EXCCHECKCVI( SetCtrlAttribute(panel, TRACA_LED_CONNECT,ATTR_FRAME_COLOR, VAL_RED));
							}
					
					break;
				
					case TRACA_DISCONNECT :
						EXCCHECK( ptraca->Disconnect(ptraca));
						EXCCHECKCVI( SetCtrlAttribute(panel, TRACA_LED_CONNECT,ATTR_FRAME_COLOR, VAL_WHITE));
						ProcessSystemEvents ();
					break;
				}
		}			
	
		break;
	}
Error:
	EXCDISPLAY( pexception);
	return 0;
}

int CVICALLBACK OnTraca (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr	pexception = NULL; 
 	int32_t			error = 0;
	char	BoardVer_sFin[10+1] = "";
	char	BoardVer_pFin[10+1] = "";
	int		id_Board_sFin;
	int     id_Board_pFin;
	int		valide, Active;
	char	ReferenceProduct[20+1] = "";
	
	
	switch (event)
	{
		case EVENT_COMMIT:
		if(gs_Modules)
		{
			STracasPtr	ptracas = (STracasPtr)gs_Modules->Get(gs_Modules, MODULE_TRACAS);  
			STracaPtr		ptraca = NULL;
				
			EXCCHECK( ptracas->GetTracaByIndex(ptracas, 0, &ptraca));
			
			switch (control)
				{
					case TRACA_BOARD_VER_SFIN_VALIDE :
						GetCtrlVal (panel, TRACA_BOARD_VER_SFIN, BoardVer_sFin);
						EXCCHECK( ptraca->Get_BoardVer_sFin_Valide(ptraca, BoardVer_sFin, &id_Board_sFin, &valide));
						SetCtrlVal (panel, TRACA_ID_BOARD_SFIN, id_Board_sFin);
						SetCtrlVal (panel, TRACA_LED_VALIDE, valide);
					break;
				
					case TRACA_BOARD_VER_SFIN_ACTIVE :
						GetCtrlVal (panel, TRACA_ID_BOARD_SFIN, &id_Board_sFin);
						EXCCHECK( ptraca->Get_BoardVer_sFin_Active(ptraca, id_Board_sFin, &valide, &Active));
						SetCtrlVal (panel, TRACA_LED_VALIDE_SFIN, valide);
						SetCtrlVal (panel, TRACA_LED_ACTIVE_SFIN, Active);
					break;
								
					case TRACA_REFERENCE_PRODUCT :
						GetCtrlVal (panel, TRACA_ID_BOARD_SFIN, &id_Board_sFin);
						EXCCHECK( ptraca->Get_Reference_Product(ptraca, id_Board_sFin, &id_Board_pFin, ReferenceProduct));
						SetCtrlVal (panel, TRACA_ID_BOARD_PFIN, id_Board_pFin);
						SetCtrlVal (panel, TRACA_PRODUCT_REFERENCE, ReferenceProduct);
					break;
								
					case TRACA_GET_BOARD_VER_PFIN :
						GetCtrlVal (panel, TRACA_ID_BOARD_PFIN, &id_Board_pFin);
						EXCCHECK( ptraca->Get_BoardVer_pFin(ptraca, id_Board_pFin, BoardVer_pFin));
						SetCtrlVal (panel, TRACA_BOARD_VER_PFIN, BoardVer_pFin);
					break;
					
				}
		}			
	
		break;
	}
Error:
	EXCDISPLAY( pexception);
	return 0;
}

int CVICALLBACK OnFieldLabel (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr	pexception = NULL; 
 	int32_t			error = 0;
	char FieldName[MAX_FIELDS][MAX_LENGTH];
	char FieldData[MAX_FIELDS][MAX_LENGTH];
	int		BoardVer_pFin;
	int		count;
	int i, rows;
	
	switch (event)
	{
		case EVENT_COMMIT:
		if(gs_Modules)
		{
			STracasPtr	ptracas = (STracasPtr)gs_Modules->Get(gs_Modules, MODULE_TRACAS);  
			STracaPtr		ptraca = NULL;
				
			EXCCHECK( ptracas->GetTracaByIndex(ptracas, 0, &ptraca));
			
			switch (control)
				{
					case TRACA_FIELD_LABEL :
						DeleteTableRows (panel, TRACA_FIELD_LABEL_DATA, 0, -1);
						GetCtrlVal (panel, TRACA_ID_BOARD_PFIN, &BoardVer_pFin);
						EXCCHECK( ptraca->Get_FieldLabel(ptraca, BoardVer_pFin, &count, FieldName, FieldData));
						for (i=0; i < count; i++)
						{
							InsertTableRows (panel, TRACA_FIELD_LABEL_DATA, -1, 1, VAL_CELL_STRING); 
							GetNumTableRows (panel, TRACA_FIELD_LABEL_DATA, &rows);
							SetTableCellAttribute (panel, TRACA_FIELD_LABEL_DATA, MakePoint(1, rows), ATTR_CTRL_VAL, FieldName[i]); 
							SetTableCellAttribute (panel, TRACA_FIELD_LABEL_DATA, MakePoint(2, rows), ATTR_CTRL_VAL, FieldData[i]); 
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

int CVICALLBACK OnMvt (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr	pexception = NULL; 
 	int32_t			error = 0;
	int		id_Board_sFin;
	int     result;
	char Num_Serie[128];
	char Date_Debut[128];
	char IdMachine[128];
	char TypeTest[128];
	int  id_board_pfin;
	
	switch (event)
	{
		case EVENT_COMMIT:
		if(gs_Modules)
		{
			STracasPtr	ptracas = (STracasPtr)gs_Modules->Get(gs_Modules, MODULE_TRACAS);  
			STracaPtr		ptraca = NULL;
				
			EXCCHECK( ptracas->GetTracaByIndex(ptracas, 0, &ptraca));
			
			switch (control)
				{
					case TRACA_GET_LAST_MVT :
						GetCtrlVal (panel, TRACA_NUM_SERIE_SFIN, Num_Serie);
						GetCtrlVal (panel, TRACA_TYPE_TEST_SFIN, TypeTest);
						EXCCHECK( ptraca->Get_Last_Mouvement(ptraca, Num_Serie, TypeTest, &result));
						SetCtrlVal (panel, TRACA_RESULT_SFIN, result);
					break;
					
							
					case TRACA_SET_LAST_MVT :
						GetCtrlVal (panel, TRACA_NUM_SERIE_PFIN, Num_Serie);
						GetCtrlVal (panel, TRACA_TYPE_TEST_PFIN, TypeTest);
						GetCtrlVal (panel, TRACA_ID_MACHINE_PFIN, IdMachine);
						GetCtrlVal (panel, TRACA_DATE_DEBUT_PFIN, Date_Debut);
						GetCtrlVal (panel, TRACA_RESULT_PFIN, &result);
						GetCtrlVal (panel, TRACA_ID_BOARD_PFIN, &id_board_pfin);
						EXCCHECK( ptraca->Set_Last_Mouvement(ptraca, Num_Serie, id_board_pfin, Date_Debut, "",IdMachine, TypeTest, result));
					break;
				}
		}			
	
		break;
	}
Error:
	EXCDISPLAY( pexception);
	return 0;
}

int CVICALLBACK OnSMM (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr	pexception = NULL; 
 	int32_t			error = 0;
	int		id_Board_sFin;
	int     result;
	char Board_Ver_pFin[128];
	char Num_Serie[128];
	char Num_Serie_SMM[29+1];
	
	switch (event)
	{
		case EVENT_COMMIT:
		if(gs_Modules)
		{
			STracasPtr	ptracas = (STracasPtr)gs_Modules->Get(gs_Modules, MODULE_TRACAS);  
			STracaPtr		ptraca = NULL;
				
			EXCCHECK( ptracas->GetTracaByIndex(ptracas, 0, &ptraca));
			
			switch (control)
				{
					case TRACA_TEST_SMM :
						GetCtrlVal (panel, TRACA_BOARD_VER_PFIN, Board_Ver_pFin);
						EXCCHECK( ptraca->Test_SMM(ptraca, Board_Ver_pFin, &result));
						SetCtrlVal (panel, TRACA_LED_WITH_SMM, result);
					break;
									
					case TRACA_GET_SMM_SN :
						GetCtrlVal (panel, TRACA_NUM_SERIE_SFIN_2, Num_Serie);
						EXCCHECK( ptraca->Get_SMM_SN(ptraca, Num_Serie, Num_Serie_SMM));
						SetCtrlVal (panel, TRACA_NUM_SERIE_SMM, Num_Serie_SMM);
					break;
				}
		}			
	
		break;
	}
Error:
	EXCDISPLAY( pexception);
	return 0;
}



