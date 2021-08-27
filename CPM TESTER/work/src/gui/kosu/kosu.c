#include <userint.h>
#include "kosu.h"
#include <dbs/dbs.h> 
#include <objlist/objlist.h>
#include <eventfactory/EventFactory.h> 
#include <language/language.h>
#include <utility.h> 
#include <guid.h>
#include "kosu_plugin.h" 
#include "toolbox.h"
#include <formatio.h>
#include <ansi_c.h>
#include "trans_language.h"
#include "piechart.h"
#include <math.h>  // <cmath> en C++

#define NB_OF_PANEL 		1
static int					gs_panel[NB_OF_PANEL + 1];
static bool_t				gs_pause_active;
static SElEventFactoryPtr	gs_EventFactory = NULL; 
static SObjListPtr 			gs_Modules = NULL;  
static SDBSPtr				gs_pDbs = NULL;
static SDBSKOSUPtr			gs_pKosu = NULL;
SLanguagePtr				gs_pLanguage = NULL; 

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "OnResetCounter"
int CVICALLBACK OnResetCounter (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr	pexception = NULL;   
	int32_t			error = 0;
	int32_t			Selected = 0;

	switch (event)
	{																					
		case EVENT_COMMIT:

		
		Selected = ConfirmPopup ("Reset Counter!", "Confirm Reset");
			
		if (Selected == 1)
		{
		/* reset counter */
		if(gs_EventFactory)
			EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_GUI_RESET_COUNTER", NULL));
		}
		
		break;
	}
Error:
	return 0;
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "OnTimerTick"
int CVICALLBACK OnTimerTick (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr	pexception = NULL;   
	int32_t			error = 0;
	double			start_time = 0.0,
					time = 0.0;
	char			time_string[10] = "";
	
	switch (event)
	{
		case EVENT_TIMER_TICK:

			if(gs_pause_active)
			{
				EXCCHECKCVI( GetCtrlVal(panel, PANEL_PAUSE_START, &start_time));
		
				time = Timer() - start_time;
		
				sprintf( time_string, "%02d:%02d", (int32_t) (time / 60.0),
						(int32_t) (fmod(time, 60)));
		
				//EXCCHECKCVI( SetCtrlVal(panel, PANEL_PAUSE_TIME, time_string));
			}
		
			break;
	}
	
Error:
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "TimeToString"
static void TimeToString(uint32_t itime, char* stime)
{
   int32_t	error = 0;
   int		h = 0,m = 0,s = 0;  
   
   h =  itime / 3600;
   m = (itime - 3600*h) / 60;
   s = (itime - 3600*h - m*60);
   
   if ( h > 0 )
      sprintf( stime, "%d:%02d:%02d", h, m, s); 
   else
      sprintf( stime, "%02d:%02d", m, s); 
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PieChartUpdate"
void PieChartUpdate( int Poucent_PASS, int Poucent_FAIL)
{
    pieInfo  pieAttributes;
    piePiece pieChart;
    int      i;
    static int pieChartID;
	
	 /* Initialize Pie Attribute structure */
    pieAttributes.title = malloc (100);
    pieAttributes.top = 90;
    pieAttributes.left = 0;
    pieAttributes.width = 160;
    pieAttributes.height = 190;
    Fmt (pieAttributes.title, "");
    
    /* Initialize Pie Chart structure */
    pieChart.numPieces = 2;
    pieChart.legendInfo = malloc (pieChart.numPieces * sizeof(ssize_t));
    for (i = 0; i < pieChart.numPieces; i++)
        pieChart.legendInfo[i] = malloc (50);
        
    /* Initialize the Pie Chart legend info */  
    Fmt(pieChart.legendInfo[0], "PASS");
    Fmt(pieChart.legendInfo[1], "FAIL");
    
    /* Now add some pieces to the chart */
    pieChart.piecePercent = malloc (sizeof(int) * pieChart.numPieces);
    pieChart.colors = malloc (sizeof(int) * pieChart.numPieces);
    pieChart.piecePercent[0] = Poucent_PASS;
    pieChart.piecePercent[1] = Poucent_FAIL;
    pieChart.colors[0] = VAL_GREEN;
    pieChart.colors[1] = VAL_RED;
    
    /* Draw Pie Chart on Canvas Control using CreatePie Function */    
    pieChartID = CreatePie (gs_panel[PANEL], pieAttributes, pieChart, VAL_LEGENDON);
 	SetCtrlAttribute (gs_panel[PANEL], pieChartID, ATTR_SMOOTH_UPDATE, 1);//Anwar
	DelayWithEventProcessing(0.1);//Anwar
    
    SetCtrlAttribute (gs_panel[PANEL], pieChartID, ATTR_ZPLANE_POSITION, 1000);

	

  /* Now free all resources and return */
    free (pieAttributes.title);
    for (i = 0; i < pieChart.numPieces; i++)
       free (pieChart.legendInfo[i]);
    free (pieChart.legendInfo);
    free (pieChart.piecePercent);
    free (pieChart.colors);




}	
	
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "KosuUpdate"
SElExceptionPtr KosuUpdate( void)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL;
	uint32_t		PANEL_actual_total; 
	uint32_t		PANEL_actual_good; 
    uint32_t		PANEL_actual_bad;
    uint32_t		PANEL_actual_retest_good;
    uint32_t		PANEL_actual_retest_bad;
    uint32_t		PANEL_required; 
    uint32_t		PANEL_END_actual;
    uint32_t		PANEL_END_required; 
    uint32_t		KOSU_actual;
    uint32_t		KOSU_required;  
    uint32_t		USER_count;  
    uint32_t		PAUSE_time;
	char      		buffer[128];
	double          fPoucent_PASS;
	double          fPoucent_FAIL;
	int 			iPoucent_PASS;
	int 			iPoucent_FAIL;
	
	
	
	if(gs_pKosu)
	{
		 pexception = gs_pKosu->GetKOSU( gs_pKosu,
					                    &PANEL_actual_good,
									    &PANEL_actual_bad,
									    &PANEL_actual_retest_good,
									    &PANEL_actual_retest_bad,
									    &PANEL_required, 
									    &PANEL_END_actual,
									    &PANEL_END_required, 
									    &KOSU_actual,
									    &KOSU_required,  
									    &USER_count,  
									    &PAUSE_time);  
		
		if(pexception==NULL)
		{

			PANEL_actual_total = PANEL_actual_good + PANEL_actual_bad;
			
			sprintf(buffer, "%d", PANEL_actual_good);
			EXCCHECKCVI( SetCtrlVal(gs_panel[PANEL], PANEL_NUM_GOOD, buffer)); 
			
			sprintf(buffer, "%d", PANEL_actual_bad);        
			EXCCHECKCVI( SetCtrlVal(gs_panel[PANEL], PANEL_NUM_BAD, buffer));  
			
			sprintf(buffer, "%d", PANEL_actual_total);        
			EXCCHECKCVI( SetCtrlVal(gs_panel[PANEL], PANEL_NUM_TOTAL, buffer));
			
			//sprintf(buffer, "%d", PANEL_actual_retest_good);
			//EXCCHECKCVI( SetCtrlVal(gs_panel[PANEL], PANEL_NUM_GOOD_RETEST, buffer)); 
			
			//sprintf(buffer, "%d", PANEL_actual_retest_bad);        
			//EXCCHECKCVI( SetCtrlVal(gs_panel[PANEL], PANEL_NUM_BAD_RETEST, buffer));  
			
		   // sprintf(buffer, "%d", USER_count);    
			//EXCCHECKCVI( SetCtrlVal(gs_panel[PANEL], PANEL_NUM_OPERATOR, buffer));  
			
		   // sprintf(buffer, "%d", PANEL_required);  
	 		//EXCCHECKCVI( SetCtrlVal(gs_panel[PANEL], PANEL_NUM_PANEL, buffer)); 
			
		   // sprintf(buffer, "%d", PANEL_actual_good);   
			//EXCCHECKCVI( SetCtrlVal(gs_panel[PANEL], PANEL_NUM_PANEL_2, buffer)); 
			
			//sprintf(buffer, "%d", PANEL_END_required);
			//EXCCHECKCVI( SetCtrlVal(gs_panel[PANEL], PANEL_NUM_END, buffer)); 
			
		  //  sprintf(buffer, "%d", PANEL_END_actual);   
			//EXCCHECKCVI( SetCtrlVal(gs_panel[PANEL], PANEL_NUM_END_2, buffer)); 
		
		//	sprintf(buffer, "%d", KOSU_required);
			//EXCCHECKCVI( SetCtrlVal(gs_panel[PANEL], PANEL_KOSU_TIME, buffer)); 
			 
			//sprintf(buffer, "%d", KOSU_actual);
			//EXCCHECKCVI( SetCtrlVal(gs_panel[PANEL], PANEL_KOSU_TIME_2, buffer)); 
			
			//TimeToString(PAUSE_time, buffer);
			//EXCCHECKCVI( SetCtrlVal(gs_panel[PANEL], PANEL_PAUSE_TIME, buffer));
			

			if (PANEL_actual_total != 0)
			{		
			
            fPoucent_PASS = (double)PANEL_actual_good/ (double)PANEL_actual_total;
            fPoucent_FAIL = (double)PANEL_actual_bad/ (double)PANEL_actual_total;


			fPoucent_PASS = RoundRealToNearestInteger(fPoucent_PASS *100);
			fPoucent_FAIL = RoundRealToNearestInteger(fPoucent_FAIL *100);
			iPoucent_PASS = (int)fPoucent_PASS;
			iPoucent_FAIL = (int)fPoucent_FAIL;
			
			PieChartUpdate (iPoucent_PASS, iPoucent_FAIL);
			}
			
		}
		else
		{
			EXCDBGSTR( pexception);	
		}
	}
	
Error:                       
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "EventFunctionCallback"
SElExceptionPtr EventFunctionCallback( const char* eventName, void* pData)
{
	int32_t			   	error = 0;
	SElExceptionPtr    	pexception = NULL;

	if(0==strcmp(eventName, "EVNT_PAUSE_STARTED"))
	{
		gs_pause_active = TRUE;
		//EXCCHECKCVI( SetCtrlVal(gs_panel[PANEL], PANEL_PAUSE_TIME, "00:00"));   
		EXCCHECKCVI( SetCtrlVal(gs_panel[PANEL], PANEL_PAUSE_START, Timer()-1));
		EXCCHECKCVI( SetCtrlAttribute( gs_panel[PANEL], PANEL_PAUSE_PICT, ATTR_VISIBLE, TRUE)); 

		/* hide operators */
		//EXCCHECKCVI( SetCtrlAttribute( gs_panel[PANEL], PANEL_BCKPICT_4, ATTR_VISIBLE, FALSE)); 
		//EXCCHECKCVI( SetCtrlAttribute( gs_panel[PANEL], PANEL_NUM_OPERATOR, ATTR_VISIBLE, FALSE)); 
		//EXCCHECKCVI( SetCtrlAttribute( gs_panel[PANEL], PANEL_MSG_OPERATOR, ATTR_VISIBLE, FALSE)); 
	}
	else if(0==strcmp(eventName, "EVNT_PAUSE_STOPED"))
	{
		gs_pause_active = FALSE;  
		EXCCHECKCVI( SetCtrlAttribute( gs_panel[PANEL], PANEL_PAUSE_PICT, ATTR_VISIBLE, FALSE)); 

		/* visible operators */
		//EXCCHECKCVI( SetCtrlAttribute( gs_panel[PANEL], PANEL_BCKPICT_4, ATTR_VISIBLE, TRUE)); 
		//EXCCHECKCVI( SetCtrlAttribute( gs_panel[PANEL], PANEL_NUM_OPERATOR, ATTR_VISIBLE, TRUE)); 
		//EXCCHECKCVI( SetCtrlAttribute( gs_panel[PANEL], PANEL_MSG_OPERATOR, ATTR_VISIBLE, TRUE)); 
	}
	else if(0==strcmp(eventName, "EVNT_KOSU_UPDATE"))
	{
		EXCCHECK( KosuUpdate());

	    EXCCHECKCVI( DisplayPanel(gs_panel[PANEL]));  
	}
	
Error:                       
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "RegisterEventFunction"
static SElExceptionPtr RegisterEventFunction(void)
{
	SElExceptionPtr	pexception = NULL;
	
	if ( gs_EventFactory )
	{
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_PAUSE_STARTED",
														 EventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_PAUSE_STOPED",
														 EventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_KOSU_UPDATE",
														 EventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_GUI_RESET_COUNTER",
														 EventFunctionCallback));
	}

Error:
	EXCRETHROW(pexception);
}	/* RegisterEventFunction */

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
			if (InitCVIRTE (hinstDLL, 0, 0) == 0)		
				return 0;	/* out of memory */
			break;
		case DLL_PROCESS_DETACH:
			if (!CVIRTEHasBeenDetached())		/* Do not call CVI functions if cvirte.dll has already been detached. */
			{
				DiscardUIObjectsForDLL ();		/* Discard the panels loaded in InitUIForDLL */
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
#define __FUNC__ "{Kosu Plugin}::Initialize"
PLUGIN_API SElExceptionPtr PLUGIN_INIT_NAME_CODE( SPluginArgsPtr a_pPluginArgs )
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	const char*		pconfiguration_file = NULL;
	int32_t			main_panel = 0;
	int				monitor_id, top, height, width;  

	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
									 "MainPanel",
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
	}
	
	EXCCHECK( LoadPanelExWithLanguage(main_panel,"kosu.uir", PANEL, KOSUlang, &gs_panel[PANEL]));
	
		
	EXCCHECKCVI( GetPanelAttribute (main_panel, ATTR_TOP, &top));
	EXCCHECKCVI( GetPanelAttribute (main_panel, ATTR_HEIGHT, &height));
	EXCCHECKCVI( GetPanelAttribute (main_panel, ATTR_WIDTH, &width));

	EXCCHECKCVI( SetPanelAttribute (gs_panel[PANEL], ATTR_TOP, 580));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[PANEL], ATTR_LEFT, 1395));
	
	EXCCHECKCVI(SetPanelAttribute (gs_panel[PANEL], ATTR_SCALE_CONTENTS_ON_RESIZE, TRUE));
	
	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
									 "EventFactory",
									 sizeof(SElEventFactoryPtr),
									 &gs_EventFactory));

	if(gs_Modules)
	{
		gs_pDbs = (SDBSPtr)gs_Modules->Get(gs_Modules, MODULE_DBS); 
		if(gs_pDbs)
		{
			if(gs_pDbs) gs_pKosu = gs_pDbs->Kosu;
		}
	}
	
	EXCCHECK( RegisterEventFunction());

Error:	
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{Kosu Plugin}::Run"
PLUGIN_API SElExceptionPtr PLUGIN_RUN_NAME_CODE( SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;

	EXCCHECK( KosuUpdate());
	EXCCHECKCVI( DisplayPanel(gs_panel[PANEL]));  
		
Error:	
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{Kosu Plugin}::Leave"
PLUGIN_API SElExceptionPtr PLUGIN_LEAVE_NAME_CODE( SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	
Error:	
	EXCRETHROW( pexception);

}

#undef __FUNC__
#define __FUNC__ "{Kosu Plugin}::Shutdown"
PLUGIN_API SElExceptionPtr PLUGIN_SHUTDOWN_NAME_CODE( SPluginArgsPtr a_pPluginArgs)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;

	if ( gs_panel[PANEL] )
		DiscardPanel(gs_panel[PANEL]);
	
Error:	
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{Kosu Plugin}::Info"
PLUGIN_API SElExceptionPtr PLUGIN_GETINFO_NAME_CODE( SPluginInfoPtr* a_pInfo)
{
	SElExceptionPtr	pexception = NULL;  
	
	if(a_pInfo) EXCCHECK( (*a_pInfo)->SetInfoData(*a_pInfo, &gs_InfoData));
	
Error:	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Kosu Plugin}::Attribute"
PLUGIN_API SElExceptionPtr PLUGIN_GETATTR_NAME_CODE( uint32_t  attrID, uint32_t size, void* value)
{
	return NULL;
}

#endif 
