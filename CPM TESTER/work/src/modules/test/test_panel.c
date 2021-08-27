#include <userint.h>
#include "test.h"
#include "test_panel.h"
#include <utility.h>
#include <test/testseq.h>
#include "test_plugin.h" /* include path in project/properties/aditional include directories */
#include "toolbox.h"
#include <formatio.h>
#include <ansi_c.h>
#include <multithread/multithread.h> 

#pragma warning( push)
#pragma warning( disable: 4996)

extern STestListItemPtr		gs_TestList[STATION_MAX];
extern STestSeq 			gs_TestSeq[];

int							gs_TestPanel = 0;
SObjListPtr 				gs_Modules = NULL;
static STesterPtr			gs_pTester = NULL;
STestSequencePtr			gs_TestSequence = NULL;
SElEventFactoryPtr			gs_EventFactory = NULL;
static SPluginArgsPtr		gs_PluginArgs = NULL;
SLanguagePtr				gs_pLanguage = NULL;
int 						gs_Autorisation;
static int					panelHandle;
/*==================================================================================
						  	 	  DLL MAIN
===================================================================================*/


int __stdcall DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
		
		case DLL_PROCESS_ATTACH:
			if (InitCVIRTE (hinstDLL, 0, 0) == 0)		
				return 0;	//out of memory 
			break;
	
		case DLL_PROCESS_DETACH:
			if (!CVIRTEHasBeenDetached())		//Do not call CVI functions if cvirte.dll has already been detached.
			{
				CloseCVIRTE ();		
			}
			break;
	}

	return 1;
}

int __stdcall DllEntryPoint (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
//Included for compatibility with Borland 

	 return DllMain (hinstDLL, fdwReason, lpvReserved);
}


//Appel de DLL pour Affichier UIR
void DLLEXPORT RunDllUI (void)
{
    ///Call this function from the appropriate place in your code 
    //to load and display startup panels.                       

    panelHandle = LoadPanelEx (0, "Test.uir", PANEL, __CVIUserHInst);
    DisplayPanel (panelHandle);
    RunUserInterface ();
}

#undef __FUNC__
#define __FUNC__ "{Test Plugin}::Initialize"
PLUGIN_API SElExceptionPtr PLUGIN_INIT_NAME_CODE( SPluginArgsPtr a_pPluginArgs )
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	int32_t			main_panel = 0;
	int32_t			monitor_id = 0,
					monitor_id2 = 0;
	int32_t         top, left, 
					width, height;
	
	gs_PluginArgs = a_pPluginArgs;

	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
									 "AutomatPanel",
									 sizeof(main_panel),
									 &main_panel));

	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
									 "WindowHandle",
									 sizeof(hWnd),
									 &hWnd));
	
	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
									 "EventFactory",
									 sizeof(SElEventFactoryPtr),
									 &gs_EventFactory));

	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
	                        "Modules",
	                        sizeof(SObjListPtr),
	                        &gs_Modules));
		
		
	/*Gettext*/
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
		
		gs_pTester = (STesterPtr)gs_Modules->Get(gs_Modules, MODULE_TESTER); 
	}
	
	EXCCHECKCVI( gs_TestPanel = LoadPanelEx( main_panel, "test.uir", PANEL, __CVIUserHInst));
	EXCCHECKCVI( SetPanelAttribute( gs_TestPanel, ATTR_TOP, 70));
	EXCCHECKCVI( SetPanelAttribute( gs_TestPanel, ATTR_LEFT, 0));
	
	EXCCHECKCVI(SetPanelAttribute (gs_TestPanel, ATTR_SCALE_CONTENTS_ON_RESIZE, TRUE));

	EXCCHECKCVI( gs_Panel[STATION_10] = LoadPanelEx( main_panel, "test.uir", TEST_10, __CVIUserHInst));  
	EXCCHECKCVI( SetPanelAttribute( gs_Panel[STATION_10], ATTR_TOP, 190));
	EXCCHECKCVI( SetPanelAttribute( gs_Panel[STATION_10], ATTR_LEFT, 5));
	EXCCHECKCVI(SetPanelAttribute (gs_Panel[STATION_10], ATTR_SCALE_CONTENTS_ON_RESIZE, TRUE));
		
	EXCCHECKCVI( gs_Panel[STATION_20] = LoadPanelEx( main_panel, "test.uir", TEST_20, __CVIUserHInst));  
	EXCCHECKCVI( SetPanelAttribute( gs_Panel[STATION_20], ATTR_TOP, 80));
	EXCCHECKCVI( SetPanelAttribute( gs_Panel[STATION_20], ATTR_LEFT, 5));
	EXCCHECKCVI(SetPanelAttribute (gs_Panel[STATION_20], ATTR_SCALE_CONTENTS_ON_RESIZE, TRUE));
		
	EXCCHECKCVI( gs_Panel[STATION_30] = LoadPanelEx( main_panel, "test.uir", TEST_30, __CVIUserHInst));  
	EXCCHECKCVI( SetPanelAttribute( gs_Panel[STATION_30], ATTR_TOP, 80));
	EXCCHECKCVI( SetPanelAttribute( gs_Panel[STATION_30], ATTR_LEFT, 5));
	EXCCHECKCVI(SetPanelAttribute (gs_Panel[STATION_30], ATTR_SCALE_CONTENTS_ON_RESIZE, TRUE));
		
	EXCCHECKCVI( gs_Panel[STATION_40] = LoadPanelEx( main_panel, "test.uir", TEST_40, __CVIUserHInst));  
	EXCCHECKCVI( SetPanelAttribute( gs_Panel[STATION_40], ATTR_TOP, 80));
	EXCCHECKCVI( SetPanelAttribute( gs_Panel[STATION_40], ATTR_LEFT, 5));
	EXCCHECKCVI(SetPanelAttribute (gs_Panel[STATION_40], ATTR_SCALE_CONTENTS_ON_RESIZE, TRUE));
		
	EXCCHECKCVI( gs_Panel[STATION_50] = LoadPanelEx( main_panel, "test.uir", TEST_50, __CVIUserHInst));  
	EXCCHECKCVI( SetPanelAttribute( gs_Panel[STATION_50], ATTR_TOP, 80));
	EXCCHECKCVI( SetPanelAttribute( gs_Panel[STATION_50], ATTR_LEFT, 5));
	EXCCHECKCVI(SetPanelAttribute (gs_Panel[STATION_50], ATTR_SCALE_CONTENTS_ON_RESIZE, TRUE));
		
	EXCCHECKCVI( gs_Panel[STATION_60] = LoadPanelEx( main_panel, "test.uir", TEST_60, __CVIUserHInst));  
	EXCCHECKCVI( SetPanelAttribute( gs_Panel[STATION_60], ATTR_TOP, 80));
	EXCCHECKCVI( SetPanelAttribute( gs_Panel[STATION_60], ATTR_LEFT, 5));
	EXCCHECKCVI(SetPanelAttribute (gs_Panel[STATION_60], ATTR_SCALE_CONTENTS_ON_RESIZE, TRUE));
	
	gs_Autorisation = 0;

Error:	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Test Plugin}::Run"
PLUGIN_API SElExceptionPtr PLUGIN_RUN_NAME_CODE( SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
	SElExceptionPtr	pexception = NULL;

Error:	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Test Plugin}::Leave"
PLUGIN_API SElExceptionPtr PLUGIN_LEAVE_NAME_CODE( SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
	SElExceptionPtr	pexception = NULL;
	
Error:	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Test Plugin}::Shutdown"
PLUGIN_API SElExceptionPtr PLUGIN_SHUTDOWN_NAME_CODE( SPluginArgsPtr a_pPluginArgs)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	
	if(gs_TestPanel)
	{
		EXCCHECKCVI( DiscardPanel( gs_TestPanel));
		gs_TestPanel = 0;
	}
	
#if 0    /* formerly excluded lines */
	if(gs_TestPanel2)
	{
		EXCCHECKCVI( DiscardPanel( gs_TestPanel2));
		gs_TestPanel2 = 0;
	}
	
	if(gs_TestPanel3)
	{
		EXCCHECKCVI( DiscardPanel( gs_TestPanel3));
		gs_TestPanel3 = 0;
	}

	if(gs_TestPanel4)
	{
		EXCCHECKCVI( DiscardPanel( gs_TestPanel4));
		gs_TestPanel4 = 0;
	}
	
	if(gs_TestPanel5)
	{
		EXCCHECKCVI( DiscardPanel( gs_TestPanel5));
		gs_TestPanel5 = 0;
	}
	
	if(gs_TestPanelLevers)
	{
		EXCCHECKCVI( DiscardPanel( gs_TestPanelLevers));
		gs_TestPanelLevers = 0;
	}
#endif   /* formerly excluded lines */

	free(gs_TestSequence);
		
Error:	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Test Plugin}::Info"
PLUGIN_API SElExceptionPtr PLUGIN_GETINFO_NAME_CODE( SPluginInfoPtr* a_pInfo)
{
	SElExceptionPtr	pexception = NULL;  
	
	if(a_pInfo) EXCCHECK( (*a_pInfo)->SetInfoData(*a_pInfo, &gs_InfoData));
	
Error:	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Test Plugin}::Attribute"
PLUGIN_API SElExceptionPtr PLUGIN_GETATTR_NAME_CODE( uint32_t  attrID, uint32_t size, void* value)
{
	return NULL;
}

//#endif 	 //TEST

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "OnButton"
int  CVICALLBACK OnButton(int panel, int control, int event, 
   void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr pexception =  NULL;  
	int32_t      	error = 0;

	switch (event)
	{
		case EVENT_COMMIT:

			switch(control)
			{
				case PANEL_FORWARD:
					if(gs_EventFactory)
						EXCCHECK(gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_EXPERTISE_STEP_MODE_NEXT", NULL)); 
					break;
				case PANEL_REPEAT:
					if(gs_EventFactory)
						EXCCHECK(gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_EXPERTISE_STEP_MODE_REPEAT", NULL)); 
					break;
				case PANEL_BACKWARD:
					if(gs_EventFactory)
						EXCCHECK(gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_EXPERTISE_STEP_MODE_BACK", NULL)); 
					break;
				case PANEL_STOP:
					if(gs_EventFactory)
						EXCCHECK(gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_EXPERTISE_STEP_MODE_STOP", NULL)); 
					break;
			}
			break;
	}

Error:
	EXCDISPLAY(pexception);
	return 0;
}


 /*---------------------------------------------------------------------------*/
int32_t GetStationNumber(int32_t station)
{
	int iStation;
	
	switch (station)
	{
	case STATION_20 : iStation = 20; break;	
	case STATION_30 : iStation = 30; break;	
	case STATION_40 : iStation = 40; break;	
	case STATION_50 : iStation = 50; break;	
	case STATION_60 : iStation = 60; break;	
	case STATION_10 : iStation = 10; break;	
	default : iStation = 0; break;	 
	}
  
return iStation;
};
	
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "GetTestSeqFunction"
SElExceptionPtr _FUNCC GetTestSeqFunction(
    int32_t         index,
    STestSeq*		pTestSeq,
	int32_t*		test_count
)
{
    static int32_t  count = 0;
	STestSeqPtr		TestSeqPtr;
    SElExceptionPtr pexception = NULL;
    
	for(; gs_TestSeq[count].InitName; count++)
		;

    if( test_count) 
		*test_count = count;
    
    if( pTestSeq && index<count) 
	{
		TestSeqPtr = &(gs_TestSeq[index]);
		*pTestSeq = TestSeqPtr[0];
	}
    
Error:
    EXCRETHROW( pexception);
};

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "GetTestFunction"
SElExceptionPtr _FUNCC GetTestFunction(
    int32_t         process,
    int32_t         index,
    STestListItemPtr*   pTest,
	int32_t*		test_count
)
{
    int32_t  count = 0;
    SElExceptionPtr pexception = NULL;
    
	for(; gs_TestList[process][count].name; count++)
		;

    if( test_count) 
		*test_count = count;
    
    if( pTest && index<count) 
		*pTest = &gs_TestList[process][index];
    
Error:
    EXCRETHROW( pexception);
};

/*---------------------------------------------------------------------------*/

void CVICALLBACK PostDeferredDisplayPanel (void *callbackData) { DisplayPanel(gs_TestPanel);}
void CVICALLBACK PostDeferredHidePanel (void *callbackData) { HidePanel(gs_TestPanel);}

void CVICALLBACK PostDeferredDisplayPanelStation (void *callbackData) 
{
	int station = (int)callbackData; 
	
	DisplayPanel(gs_Panel[station]);
}

void CVICALLBACK PostDeferredHidePanelStation (void *callbackData) 
{
	int panel = (int)callbackData; 
	
	HidePanel(panel);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "GetTestSeqActive"
SElExceptionPtr _FUNCC GetTestSeqActive(int* SeqUsed)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0; 
   	int32_t			station = 0; 
	int             used;
	
	for (station=0; station<STATION_MAX; station++)
	{
		
		EXCCHECKCVI( GetCtrlVal( gs_Panel[station], panelSeqUsed[station], &used));
		SeqUsed[station] = used;
	}

Error:
    EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "GetTestSeqStopFail"
SElExceptionPtr _FUNCC GetTestSeqStopFail(int* SeqStopFail)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0; 
   	int32_t			station = 0; 
	int             StopFail;
	
	for (station=0; station<STATION_MAX; station++)
	{
		
		EXCCHECKCVI( GetCtrlVal( gs_Panel[station], panelSeqGoFail[station], &StopFail));
		SeqStopFail[station] = StopFail;
	}

Error:
    EXCRETHROW( pexception);
}



/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "IniTestPanel"
SElExceptionPtr _FUNCC IniTestPanel(const char* picture_left, const char* picture_right, bool_t reset)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0; 

	if(gs_TestPanel)
	{
#if 0    /* formerly excluded lines */
		if(reset)
		{
			EXCCHECKCVI( SetCtrlVal( gs_TestPanel, PANEL_RESULT_MSG, _("Test Result")));
			EXCCHECKCVI( SetCtrlVal( gs_TestPanel, PANEL_TEXT1, _("Time:"))); 
			EXCCHECKCVI( SetCtrlVal( gs_TestPanel, PANEL_TEXT2, _("S/N:")));
			EXCCHECKCVI( SetCtrlVal( gs_TestPanel, PANEL_TEXT3, _("Test:")));
			EXCCHECKCVI( SetCtrlIndex( gs_TestPanel, PANEL_ACTION, 3)); 
			EXCCHECKCVI( DeleteTableRows( gs_TestPanel, PANEL_TABLE, 1, -1));
		}
#endif   /* formerly excluded lines */
		//EXCCHECKCVI( DisplayImageFile( gs_TestPanel, PANEL_PRODUCT_PICTURE, picture_left));
		EXCCHECKCVI( PostDeferredCall( PostDeferredDisplayPanel, NULL));
	}

Error:
    EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "CloseTestPanel"
SElExceptionPtr _FUNCC CloseTestPanel(void)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0; 

	if(gs_TestPanel)
		EXCCHECKCVI( PostDeferredCall( PostDeferredHidePanel, NULL));

Error:
    EXCRETHROW( pexception);
}

void SetAnnotationAttribute_ALL(int Panel_Handle, int Control_ID, int Annotation_Index)
{
	SetAnnotationAttribute(Panel_Handle, Control_ID, Annotation_Index, ATTR_ANNOTATION_OFFSET_MODE, VAL_TOP_LEFT_OFFSET);
    SetAnnotationAttribute(Panel_Handle, Control_ID, Annotation_Index, ATTR_ANNOTATION_LINE_COLOR, VAL_BLACK);
    SetAnnotationAttribute(Panel_Handle, Control_ID, Annotation_Index, ATTR_ANNOTATION_GLYPH_COLOR, VAL_BLACK);
    SetAnnotationAttribute(Panel_Handle, Control_ID, Annotation_Index, ATTR_ANNOTATION_CAPTION_ALWAYS_IN_VIEW, 1);
    SetAnnotationAttribute(Panel_Handle, Control_ID, Annotation_Index, ATTR_ANNOTATION_CAPTION_BOLD, 1);
    SetAnnotationAttribute(Panel_Handle, Control_ID, Annotation_Index, ATTR_ANNOTATION_CAPTION_BGCOLOR, VAL_WHITE);
	SetAnnotationAttribute(Panel_Handle, Control_ID, Annotation_Index, ATTR_ANNOTATION_CAPTION_COLOR, VAL_BLACK);
}


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SetTestPanelDescription"
SElExceptionPtr _FUNCC SetTestPanelDescription(
    int32_t		control,
    const char*	description
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0; 

	if(gs_TestPanel)
	{
			EXCCHECKCVI( SetCtrlVal( gs_TestPanel, control, description));
	
			if (strcmp("NOK", description) == 0)
				EXCCHECKCVI( SetCtrlAttribute (gs_TestPanel, control, ATTR_TEXT_BGCOLOR, VAL_RED));
	
			else if (strcmp("OK", description) == 0)
				EXCCHECKCVI( SetCtrlAttribute (gs_TestPanel, control, ATTR_TEXT_BGCOLOR, VAL_GREEN));
	
			else 
				EXCCHECKCVI( SetCtrlAttribute (gs_TestPanel, control, ATTR_TEXT_BGCOLOR, VAL_WHITE));
			
	}
	
Error:
    EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SetTestPanelTime"
SElExceptionPtr _FUNCC SetTestPanelTime(
    int32_t     station,
	int32_t     time
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0; 
	char            stime[32];
	
	if(gs_TestPanel)		
	{
		sprintf (stime, "(%ds)", time);
		//EXCCHECKCVI( SetCtrlVal( gs_TestPanel, panelTimeStation[station], stime)); 
	}
	
Error:
    EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SetTestPanelFault"
SElExceptionPtr _FUNCC SetTestPanelFault(
    int32_t     station,
    int32_t     station_origin,
	char*		test_description,
	bool_t      status
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0; 
	char            sfault[32];
	
	if(gs_TestPanel && station_origin > 0)
	{
		sprintf (sfault, "WS%d", station_origin);
		SetCtrlVal( gs_TestPanel, panelFaultStation[station], sfault); 

		if (status == 0)
		{
		SetCtrlVal( gs_TestPanel, panelFaultStation[station], sfault); 
		SetCtrlAttribute (gs_TestPanel, panelFaultStation[station], ATTR_VISIBLE, TRUE);
		}
	
		else 
		{
		SetCtrlAttribute (gs_TestPanel, panelFaultStation[station], ATTR_VISIBLE, FALSE);
		}

	}

	ProcessDrawEvents ();

Error:
    EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SetPanelWarning"
SElExceptionPtr _FUNCC SetPanelWarning(
    int32_t		control,
	bool_t status
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0; 

	/*if(gs_TestPanel)
		EXCCHECKCVI(SetCtrlAttribute (gs_TestPanel, control, ATTR_VISIBLE, status)); */

Error:
    EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SetPanelStationStatus"
SElExceptionPtr _FUNCC SetPanelStationStatus(
    int32_t		station,
	bool_t status
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0; 

	if(gs_TestPanel)
		EXCCHECKCVI(SetCtrlIndex( gs_TestPanel, panelStatusStation[station], status));

Error:
    EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SetTestPanelMode"
SElExceptionPtr _FUNCC SetTestPanelMode(
    int32_t		mode
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0; 

	if(gs_TestPanel)
	{
		switch(mode)
		{
			case E_TSPM_STEP_BY_STEP_MODE:
				EXCCHECKCVI( SetCtrlAttribute(gs_TestPanel, PANEL_RESULT_MSG, ATTR_VISIBLE, FALSE));
				EXCCHECKCVI( SetCtrlAttribute(gs_TestPanel, PANEL_FORWARD, ATTR_VISIBLE, TRUE));
				EXCCHECKCVI( SetCtrlAttribute(gs_TestPanel, PANEL_REPEAT, ATTR_VISIBLE, TRUE));
				EXCCHECKCVI( SetCtrlAttribute(gs_TestPanel, PANEL_BACKWARD, ATTR_VISIBLE, TRUE));
				EXCCHECKCVI( SetCtrlAttribute(gs_TestPanel, PANEL_STOP, ATTR_VISIBLE, TRUE));
				EXCCHECKCVI( SetCtrlAttribute(gs_TestPanel, PANEL_FORWARD, ATTR_DIMMED, TRUE));
				EXCCHECKCVI( SetCtrlAttribute(gs_TestPanel, PANEL_BACKWARD, ATTR_DIMMED, TRUE));
				EXCCHECKCVI( SetCtrlAttribute(gs_TestPanel, PANEL_REPEAT, ATTR_DIMMED, TRUE));
				EXCCHECKCVI( SetCtrlAttribute(gs_TestPanel, PANEL_STOP, ATTR_DIMMED, TRUE));
				break;
			default:
				EXCCHECKCVI( SetCtrlAttribute(gs_TestPanel, PANEL_RESULT_MSG, ATTR_VISIBLE, TRUE));
				EXCCHECKCVI( SetCtrlAttribute(gs_TestPanel, PANEL_FORWARD, ATTR_VISIBLE, FALSE));
				EXCCHECKCVI( SetCtrlAttribute(gs_TestPanel, PANEL_REPEAT, ATTR_VISIBLE, FALSE));
				EXCCHECKCVI( SetCtrlAttribute(gs_TestPanel, PANEL_BACKWARD, ATTR_VISIBLE, FALSE));
				EXCCHECKCVI( SetCtrlAttribute(gs_TestPanel, PANEL_STOP, ATTR_VISIBLE, FALSE));
				//ARAIBIA ajouter autres pour rest panel
				break;
		}
	}

Error:
    EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SetTestPanelReferences"
SElExceptionPtr _FUNCC SetTestPanelReferences(
    char* 	RefValeo,
    char* 	RefCustomer
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0; 

	if(gs_TestPanel)
	{
		//EXCCHECKCVI( DefaultCtrl (gs_TestPanel, PANEL_REF_CUSTOMER));
		//EXCCHECKCVI( SetCtrlVal( gs_TestPanel, PANEL_REF_CUSTOMER, RefCustomer));
		ProcessSystemEvents ();
		ProcessDrawEvents ();
	}

Error:
    EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "GetPalettStatus"
SElExceptionPtr _FUNCC GetPalettStatus(
    int32_t		palettID,
    int32_t		*status
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0; 
	int32_t			State;
		
	if(gs_TestPanel)
	{
		EXCCHECKCVI( GetCtrlVal(gs_TestPanel, panelPlaettCheck[palettID-1], &State));
		*status = State; 
	}
	
Error:
    EXCRETHROW( pexception);
}

/*----------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "SaveXmlResultFile_Manual"
void SaveXmlResultFile_Manual(STestParamPtr pTestParam, int station, bool_t failed)
{
	SElExceptionPtr pexception = NULL;
	HRESULT			hr;
	char			pfile_xml[256];
	VARIANT			pfile_name_var = CA_VariantEmpty();

	if(pTestParam && pTestParam->pTicket)
	{
		
		sprintf(pfile_xml, "%s\\%05d_%s_%s_station_%02d.xml",
					pTestParam->TicketDirPath,
					00000,
					(failed)? "FAULT":"OK",
					pTestParam->Time,
					station);
		
		strcpy(pTestParam->TicketFilePath, pfile_xml);
		EXCCHECKCOM( CA_VariantSetCString(&pfile_name_var, pfile_xml));
		EXCCHECKCOM( MSXML_IXMLDOMDocumentsave (pTestParam->pTicket, NULL, pfile_name_var));
		EXCCHECKCOM( CA_DiscardObjHandle(pTestParam->pTicket));
		pTestParam->pTicket = 0;
	}

Error:
	CA_VariantClear(&pfile_name_var);
	EXCDISPLAY(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "GetTestParameters"
static SElExceptionPtr GetTestParameters(
	SObjListPtr pModules,
	STestSequencePtr pTestSeq, 
	STestParamPtr pTestParam
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	HRESULT         hr = 0;
	CAObjHandle     pxml = 0; 
	CAObjHandle     pnode = 0;
	CAObjHandle     pparameter = 0;
	CAObjHandle     pparameter1 = 0;
	CAObjHandle     pparameter2 = 0;
	char			pfile_form_xml[MAX_PATHNAME_LEN];        
	VBOOL           is_xml_success = 1;
	VARIANT         pfile_name_var = CA_VariantEmpty();
	char*           ptext = NULL;
	char*           ptext2 = NULL;
	char			project_dir[MAX_PATHNAME_LEN],
					result_dir[MAX_PATHNAME_LEN],
					sdate_time[256],
					buffer[2048] = "";
	char			testdll_path[MAX_PATHNAME_LEN] = "";
	SYSTEMTIME		st;
	bool_t			is_locked = FALSE;
	int             serial;
	int				size;
	char         	path[256];
	int            	i = 0;
	int            	j = 0;
	int 			Test, SubTest;
	
	/* check destination directory */
	GetLocalTime(&st);
	EXCCHECKCVI(GetProjectDir(project_dir));

	sprintf(result_dir, "%s\\data\\production\\%04d.%02d.%02d", project_dir, st.wYear, st.wMonth, st.wDay);

	EXCCHECKCVI( FileExists (result_dir, &size));
	
	if(size==-1)
		{
		/* create directory */	
		EXCCHECKCVI( MakeDir(result_dir));
		}
	
	sprintf(pfile_form_xml, "%s\\plugins\\test_form.xml", project_dir);

	/* open xml file */
	EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml));
	EXCCHECKCOM( CA_VariantSetCString(&pfile_name_var, pfile_form_xml));
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (pxml, NULL, pfile_name_var, &is_xml_success));
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//testseq", &pnode));
	
	/* test dll name */
	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "global/parameter[name='ProductTestFile']/value", &pparameter));		
	if ( pparameter )
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
		if(ptext && *ptext)
		{
			EXCCHECKCVI( GetProjectDir(testdll_path)); 
			strcat(testdll_path, "\\plugins\\");
			strcat(testdll_path, ptext);
			
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
		
		EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
		pparameter = 0;
	}
	else
	{
		EXCTHROW(-1, _("Library name does not specified in xml file!"));
	}
	
	/* load new test sequence from DLL */ 
	EXCCHECK( pTestSeq->Load(pTestSeq, testdll_path, pModules, gs_EventFactory));

	do
	{
		sprintf(path, "process [@stage='%d']", ++i);
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter1));
		if(pparameter1)
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter1, NULL, &ptext));
			if(ptext && *ptext)
			{
				do
				{
					sprintf(path, "process [@stage='%d']/test[@step='%d']/name", i, ++j); 
					EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter2));
					if(pparameter2)
					{
						EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter2, NULL, &ptext));
						if(ptext && *ptext)
						{
							EXCCHECK(pTestSeq->SetTestActive(pTestSeq, i-1, ptext, j));
							
							CA_FreeMemory(ptext);
							ptext=NULL;
						}
						EXCCHECKCVI( CA_DiscardObjHandle(pparameter2));
						pparameter2 = 0;
					}
					else
					{
						j=0;
						break;
					}
		
				}while(TRUE);	
				
				
			CA_FreeMemory(ptext);
			ptext=NULL;
			}
		
			EXCCHECKCVI( CA_DiscardObjHandle(pparameter1));
			pparameter1 = 0;
		}
		
		else
		{
			break;
		}
		
	}while(TRUE);
		
			
	/* check validation */
	pTestParam->pTicket = pxml;
	
	EXCCHECK( pTestSeq->UsageValidation(pTestSeq, pTestParam));   	

	/* date_time */
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode( pxml, NULL, "/testseq/product/mode", &pnode));
	if(pnode)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeSettext( pnode, NULL, "Manual"));
		EXCCHECKCVI( CA_DiscardObjHandle(pnode));
		pnode = 0;
	}
	/* date_time */
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode( pxml, NULL, "/testseq/product/date_time", &pnode));
	if(pnode)
	{
		sprintf(sdate_time, "%02d.%02d.%02d %02d.%02d.%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);      
		EXCCHECKMSXML( MSXML_IXMLDOMNodeSettext( pnode, NULL, sdate_time));
		EXCCHECKCVI( CA_DiscardObjHandle(pnode));
		pnode = 0;
	}
	/* prepare test parameter data */
	pTestParam->pTicket = pxml;
	pTestParam->shift_id = 1;
	sprintf(pTestParam->Time, "%02d.%02d.%02d", st.wHour, st.wMinute, st.wSecond);
	sprintf(pTestParam->Date, "%04d.%02d.%02d", st.wYear, st.wMonth, st.wDay);
	sprintf(pTestParam->TicketDirPath, result_dir);
	sprintf(pTestParam->Barcode, "%s\0", "none");
			
Error:
	EXCRETHROW( pexception);  
}

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Thread_Test"
DWORD WINAPI Thread_Test(LPVOID param)
{
	SElExceptionPtr			 	pexception = NULL;
	int32_t						error = 0;
	int							station = (int)param;
	static STestParamPtr		pTestParam = NULL;
	SYSTEMTIME					st;
	int                         process;
	int 						Retest, i;
	int							StopRetest = 0;
	char                        msg[128];
	int                         pass, fail;
	int                         fault_ignore;

	
	if (gs_Autorisation == 0)
	{	
		gs_Autorisation = 1;
		
		pTestParam = calloc(1, sizeof(STestParam));
		EXCCHECKALLOC( pTestParam);


		EXCCHECK( gs_PluginArgs->GetArg(gs_PluginArgs, "TestSequence", ARG_PTR, &gs_TestSequence));
		
		GetCtrlVal(gs_Panel[station], ControlReTest[station], &fault_ignore);
		GetCtrlVal(gs_Panel[station], ControlReTest[station], &Retest);

		pass = 0;
		fail = 0;
		sprintf (msg, "%d", pass);
		SetCtrlVal(gs_Panel[station], ControlReTestPASS[station], msg);
		sprintf (msg, "%d", fail);
		SetCtrlVal(gs_Panel[station], ControlReTestFAIL[station], msg);			
		
		for(i=0; i<Retest; i++)
		{
					
			sprintf (msg, "%d / %d", i+1, Retest);
			SetCtrlVal(gs_Panel[station], ControlReTestTotal[station], msg);
		
			GetCtrlVal( gs_Panel[station], panelSeqGoFail[station], &fault_ignore);

			if(gs_TestSequence)
				EXCCHECK( GetTestParameters(gs_Modules, gs_TestSequence, pTestParam));
					
			GetLocalTime(&st);
	
			sprintf(pTestParam->Time, "%02d.%02d.%02d", st.wHour, st.wMinute, st.wSecond);
							
			if(gs_TestSequence && pTestParam)
			{				
				pexception = gs_TestSequence->RunSelectedStation(gs_TestSequence, pTestParam, fault_ignore, station);
				if (pexception == NULL)	
				{
					pass = pass + 1;
					sprintf (msg, "%d", pass);
					SetCtrlVal(gs_Panel[station], ControlReTestPASS[station], msg);
					SaveXmlResultFile_Manual(pTestParam, station, 0);
				}
				else  
				{   
					fail = fail + 1;
					sprintf (msg, "%d", fail);
					SetCtrlVal(gs_Panel[station], ControlReTestFAIL[station], msg);
					SaveXmlResultFile_Manual(pTestParam, station, 1);
				}
			}
			
			GetCtrlVal(gs_Panel[station], ControlStopReTest[station], &StopRetest);
			if (StopRetest)
				i= Retest;
			
			Sleep(1000);
			
		}
	}
	
	
Error:
	gs_Autorisation = 0;
	//free(pTestParam);
#if 0    /* formerly excluded lines */
	if(pexception)
	{
		EXCDBGSTR(pexception);
		ExitThread(THREAD_ERROR_CODE);
	}
	else
		ExitThread(THREAD_EXIT_CODE);
#endif   /* formerly excluded lines */
return 0;
}

int CVICALLBACK OnDisplay (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0; 
	int 			i;

	switch (event)
	{
		case EVENT_LEFT_CLICK:

				for (i=0; i< STATION_MAX; i++)
				{
					if (control == panelStatusStation[i] || control == panelSEQStation[i])
					{
					EXCCHECKCVI( PostDeferredCall( PostDeferredDisplayPanelStation, (void*)i));	
					}
				}

		break;
	}

	
Error:
	return 0;
}

int CVICALLBACK OnHidePanel (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0; 
	
	switch (event)
	{
		case EVENT_COMMIT:

			EXCCHECKCVI( PostDeferredCall( PostDeferredHidePanelStation, (void*)panel));
			
			break;
	}
	
Error:
	return 0;
}


int CVICALLBACK OnTest (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	SElExceptionPtr 	pexception = NULL;
	int32_t				error = 0;
	 HANDLE  			hThread;
	int    				microCam;

	switch (event)
	{
		case EVENT_COMMIT:

			//EXCCHECK( gs_pTester->GetSensor(gs_pTester, "MICRO_CAM", &microCam));
			microCam = 1;
			
			if (microCam)
				{
						if (panel ==  gs_Panel[STATION_10])  EXCCHECK(ThreadStart(Thread_Test, (void*)STATION_10, &hThread, NULL));
				   else if (panel ==  gs_Panel[STATION_20])  EXCCHECK(ThreadStart(Thread_Test, (void*)STATION_20, &hThread, NULL)); 
				   else if (panel ==  gs_Panel[STATION_30])  EXCCHECK(ThreadStart(Thread_Test, (void*)STATION_30, &hThread, NULL)); 
				   else if (panel ==  gs_Panel[STATION_40])  EXCCHECK(ThreadStart(Thread_Test, (void*)STATION_40, &hThread, NULL)); 
				   else if (panel ==  gs_Panel[STATION_50])  EXCCHECK(ThreadStart(Thread_Test, (void*)STATION_50, &hThread, NULL)); 
				   else if (panel ==  gs_Panel[STATION_60])  EXCCHECK(ThreadStart(Thread_Test, (void*)STATION_60, &hThread, NULL)); 
				}
				
				else 
				{	
					EXCTHROW( -1, "Plateau pas en position!");
				}	
					
		break;
	}

Error:
	EXCDISPLAY( pexception);
	return 0;
}

int CVICALLBACK OnFnc10 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	SElExceptionPtr 	pexception = NULL;
	int32_t				error = 0;
	 HANDLE  			hThread;
	int    				microCam;

	switch (event)
	{
		case EVENT_COMMIT:
			
			
		break;
							
	}
Error:
	EXCDISPLAY( pexception);
	return 0;
}

int CVICALLBACK OnFnc20 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	SElExceptionPtr 	pexception = NULL;
	int32_t				error = 0;
	HANDLE  			hThread;
	int    				microCam = 1;

	switch (event)
	{
		case EVENT_COMMIT:
			
			switch (control)
			{
				case TEST_20_TEST20:
													 
				if (gs_pTester) EXCCHECK( gs_pTester->GetSensor(gs_pTester, "MICRO_CAM", &microCam));
					if (microCam)
						EXCCHECK(ThreadStart(Thread_Test, (void*)STATION_20, &hThread, NULL));			 
					else 
						EXCTHROW( -1, "Plateau pas en position!");
						
				break;	
				
				case TEST_20_EXIT:
													 
					EXCCHECKCVI( PostDeferredCall( PostDeferredHidePanelStation, (void*)panel));
						
				break;	
				
						
				case TEST_20_VIEW_MORE_20 :
		
						int width;
						
						GetPanelAttribute (panel, ATTR_WIDTH, &width);
						if (width < 1400)
						{
						SetPanelAttribute (panel, ATTR_WIDTH, 1400);
						SetCtrlAttribute (panel, control, ATTR_LABEL_TEXT, "Moins");
						}
						else 
						{
						SetPanelAttribute (panel, ATTR_WIDTH, 500);
						SetCtrlAttribute (panel, control, ATTR_LABEL_TEXT, "Plus");					
						}
						
				break;	
				
				
				
				
					
			}
			
		break;
							
	}
Error:
	EXCDISPLAY( pexception);
	return 0;
}

int CVICALLBACK OnFnc30 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	SElExceptionPtr 	pexception = NULL;
	int32_t				error = 0;
	HANDLE  			hThread;
	int    				microCam = 1;

	switch (event)
	{
		case EVENT_COMMIT:
			
			switch (control)
			{
			case TEST_30_TEST30:
													 
				if (gs_pTester) EXCCHECK( gs_pTester->GetSensor(gs_pTester, "MICRO_CAM", &microCam));
					if (microCam)
						EXCCHECK(ThreadStart(Thread_Test, (void*)STATION_30, &hThread, NULL));			 
					else 
						EXCTHROW( -1, "Plateau pas en position!");
						
				break;	
				
				case TEST_30_EXIT:
													 
					EXCCHECKCVI( PostDeferredCall( PostDeferredHidePanelStation, (void*)panel));
						
				break;	
				
						
				case TEST_30_VIEW_MORE_30:
		
						int width;
						
						GetPanelAttribute (panel, ATTR_WIDTH, &width);
						if (width < 1400)
						{
						SetPanelAttribute (panel, ATTR_WIDTH, 1400);
						SetCtrlAttribute (panel, control, ATTR_LABEL_TEXT, "Moins");
						}
						else 
						{
						SetPanelAttribute (panel, ATTR_WIDTH, 500);
						SetCtrlAttribute (panel, control, ATTR_LABEL_TEXT, "Plus");					
						}
						
				break;	
			}	
			
		break;
							
	}
Error:
	EXCDISPLAY( pexception);
	return 0;
}
int CVICALLBACK OnFnc40 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	SElExceptionPtr 	pexception = NULL;
	int32_t				error = 0;
	HANDLE  			hThread;
	int    				microCam = 1;

	switch (event)
	{
		case EVENT_COMMIT:
			
			switch (control)
			{
			case TEST_40_TEST40:
													 
				if (gs_pTester) EXCCHECK( gs_pTester->GetSensor(gs_pTester, "MICRO_CAM", &microCam));
					if (microCam)
						EXCCHECK(ThreadStart(Thread_Test, (void*)STATION_40, &hThread, NULL));			 
					else 
						EXCTHROW( -1, "Plateau pas en position!");
						
				break;	
				
				case TEST_40_EXIT:
													 
					EXCCHECKCVI( PostDeferredCall( PostDeferredHidePanelStation, (void*)panel));
						
				break;	
				
						
				case TEST_40_VIEW_MORE_40:
		
						int width;
						
						GetPanelAttribute (panel, ATTR_WIDTH, &width);
						if (width < 1400)
						{
						SetPanelAttribute (panel, ATTR_WIDTH, 1400);
						SetCtrlAttribute (panel, control, ATTR_LABEL_TEXT, "Moins");
						}
						else 
						{
						SetPanelAttribute (panel, ATTR_WIDTH, 500);
						SetCtrlAttribute (panel, control, ATTR_LABEL_TEXT, "Plus");					
						}
						
				break;	
			}	
			
		break;
							
	}
Error:
	EXCDISPLAY( pexception);
	return 0;
}
int CVICALLBACK OnFnc50 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	SElExceptionPtr 	pexception = NULL;
	int32_t				error = 0;
	HANDLE  			hThread;
	int    				microCam = 0;

	switch (event)
	{
		case EVENT_COMMIT:
					
			case TEST_50_TEST50:
													 
				if (gs_pTester) EXCCHECK( gs_pTester->GetSensor(gs_pTester, "MICRO_CAM", &microCam));
				
					if (microCam)
						EXCCHECK(ThreadStart(Thread_Test, (void*)STATION_50, &hThread, NULL));			 
					else 
						EXCTHROW( -1, "Plateau pas en position!");
						
			break;	
			
		break;
							
	}
Error:
	EXCDISPLAY( pexception);
	return 0;
}
int CVICALLBACK OnFnc60 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	SElExceptionPtr 	pexception = NULL;
	int32_t				error = 0;
	 HANDLE  			hThread;
	int    				microCam=0;

	switch (event)
	{
		case EVENT_COMMIT:
			case TEST_60_TEST60:
													 
				if (gs_pTester) EXCCHECK( gs_pTester->GetSensor(gs_pTester, "MICRO_CAM", &microCam));
				
					if (microCam)
						EXCCHECK(ThreadStart(Thread_Test, (void*)STATION_60, &hThread, NULL));			 
					else 
						EXCTHROW( -1, "Plateau pas en position!");
						
			break;		
	
			
		break;
							
	}
Error:
	EXCDISPLAY( pexception);
	return 0;
}

#pragma warning( pop)





