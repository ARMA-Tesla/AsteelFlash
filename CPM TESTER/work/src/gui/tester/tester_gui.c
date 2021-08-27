#include <userint.h>
#include "tester_gui.h"
#include <msxmldom.h>
#include <eventfactory/EventFactory.h> 
#include <multithread/multithread.h> 
#include <utility.h>
#include <guid.h>
#include <tester/tester.h>
#include <axis/class/axis.h>
#include <language/language.h>
#include <bklib/bklib2.h>
#include "tester_gui_plugin.h"
#include <ansi_c.h>
#include "trans_language.h"
#include <test/testseq.h>
#include "toolbox.h"

#define NUMBER_OF_PANELS 	1+1
#define NUMBER_OF_TABS		7
#define LINE_NAME_LENGTH	32
#define TIMEOUT 			2000
#define MAX_STATIONS 		6

#define VAL_VERT 			0
#define VAL_ROUGE 			1
#define VAL_GRIS 			2


typedef enum _EPage
{
	PAGE_ONE = 0,
	PAGE_TWO,
	PAGE_THREE,
	PAGE_FOUR,
	PAGE_FIVE,
	PAGE_SIX,
	PAGE_SEVEN,
} EPage;

typedef enum _EMode
{
	MODE_TESTER = 0,
	MODE_STATION_1,
	MODE_STATION_2,
	MODE_STATION_3,
	MODE_STATION_4,
	MODE_STATION_5,
	MODE_STATION_6,
} EMode;

enum EThreadId
{
	THREAD_TESTER = 0,
	THREAD_STATION_1,
	THREAD_STATION_2,
	THREAD_STATION_3,
	THREAD_STATION_4,
	THREAD_STATION_5,
	THREAD_STATION_6,
	NB_OF_THREAD
};

static int gs_status[] = {
	TABPANEL_LED_1,
	TABPANEL_LED_2,
	TABPANEL_LED_3,
	TABPANEL_LED_4,
	TABPANEL_LED_5,
	TABPANEL_LED_6
};

static int gs_TabPanel[] = {
	TABPANEL_1_BKTREE,
	TABPANEL_2_BKTREE,
	TABPANEL_3_BKTREE,
	TABPANEL_4_BKTREE,
	TABPANEL_5_BKTREE,
	TABPANEL_6_BKTREE
};

typedef struct _SLine
{
	char		channel[LINE_NAME_LENGTH+1];
	char		keyNode[LINE_NAME_LENGTH+1];
	uint32_t	state;
} SLine, *SLinePtr;

static EMode				gs_mode;
static int					gs_Sation;
static int					gs_Thread_Id;
static int					gs_panel[NUMBER_OF_PANELS+1];
int							gs_panelTab[NUMBER_OF_TABS+1];
SLanguagePtr 				gs_pLanguage;
static SElEventFactoryPtr   gs_EventFactory = NULL;  
static SObjListPtr          gs_Modules = NULL; 
static STesterPtr			gs_pTester = NULL;
static SDIOSensorsPtr		gs_pSensors = NULL;
static SaxissPtr			gs_paxiss = NULL;
static CAObjHandle			gs_hTreeTester;
static SPluginArgsPtr		gs_PluginArgs = NULL;
static STestSequencePtr		gs_TestSequence = NULL;
static CAObjHandle			gs_hTreeStation[MAX_STATIONS];
static CAObjHandle			gs_hImageList;
static HANDLE				gs_hThread[NB_OF_THREAD];
static HANDLE				gs_hEventStop[NB_OF_THREAD];
static SLinePtr				gs_pInputTester = NULL;
static SLinePtr				gs_pOutputTester = NULL;
static SLinePtr				gs_pInputStation[MAX_STATIONS];
static SLinePtr				gs_pOutputStation[MAX_STATIONS];

static DWORD WINAPI ThreadProcTester(LPVOID param);
static DWORD WINAPI ThreadProcStation(LPVOID param);
static SElExceptionPtr Tab_Change( int page);

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "OnFncStation1"
int CVICALLBACK OnFncStation1 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr 	pexception = NULL;
	int32_t				error = 0;
	char				buffer[512] = "";
	int					position = 0;
	STestParamPtr		pTestParam = NULL;
	bool_t          	microCam, presence;
 	HANDLE  			hThread;
	uint32_t 			valueAI_1,valueAI_2,valueAI_3;
	
	SetWaitCursor (1);

	switch (event)
		{
		case EVENT_COMMIT:
		
			switch(control)
			{
				
				case TABPANEL_1_READ_ANALOG_SENSOR:
				
					pexception = gs_pTester->GetSensorAI(gs_pTester, "SENSOR_AI_1_PRODUCT_STATION_10", &valueAI_1);
					pexception = gs_pTester->GetSensorAI(gs_pTester, "SENSOR_AI_2_PRODUCT_STATION_10", &valueAI_2);
					pexception = gs_pTester->GetSensorAI(gs_pTester, "SENSOR_AI_3_PRODUCT_STATION_10", &valueAI_3);
					valueAI_1 = (int)valueAI_1/32;
					valueAI_2 = (int)valueAI_2/32;
					valueAI_3 = (int)valueAI_3/32;
					SetCtrlVal(panel,TABPANEL_1_ANALOG_SENSOR_AI1, valueAI_1);
					SetCtrlVal(panel,TABPANEL_1_ANALOG_SENSOR_AI2, valueAI_2);
					SetCtrlVal(panel,TABPANEL_1_ANALOG_SENSOR_AI3, valueAI_3);
					
				break;					

				
				case TABPANEL_1_BARCODE:
						
					DeleteTextBoxLines (panel, TABPANEL_1_CODE_STRING, 0, -1);
					
					if (gs_pTester->pBarCodeReader)
						pexception = gs_pTester->GetItemCode(gs_pTester, 0, 0, 512, buffer, TIMEOUT);
					
					if(pexception==NULL) 
					{
						EXCCHECKCVI( SetCtrlVal(panel, TABPANEL_1_CODE_STRING, buffer));
					}
					else
					{
						EXCDISPLAY( pexception); 
						EXCCHECKCVI( SetCtrlVal(panel, TABPANEL_1_CODE_STRING, "#error"));
					}
					
				break;
				

			}
		break;
}
	
Error:
	SetWaitCursor (0);
	if (pTestParam != NULL)
		free(pTestParam);
	EXCDISPLAY( pexception);
	return 0;
}

#undef __FUNC__
#define __FUNC__ "OnFncStation2"
int CVICALLBACK OnFncStation2 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int Etat;
	
	SElExceptionPtr 	pexception = NULL;
	int32_t				error = 0;
	char				buffer[512];
	int					position = 0;
	int                 push;
	int 				program = 0;
	STestParamPtr		pTestParam = NULL;
	bool_t          	microCam, presence;
	
	SetWaitCursor (1);

	switch (event)
	{
		case EVENT_COMMIT:

			switch(control)
			{
				case TABPANEL_2_PLUG:
				
					pexception = gs_pTester->PlugPanelStation20(gs_pTester, 4000);
					if(pexception==NULL) 
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_PLUG, VAL_VERT));
						ProcessSystemEvents ();
					}
					else
					{

						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_PLUG, VAL_ROUGE));
						EXCDISPLAY( pexception);   
					}
					EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_UNPLUG, VAL_GRIS));
					
				break;
				
				case TABPANEL_2_UNPLUG:
				
					pexception = gs_pTester->UnPlugPanelStation20(gs_pTester, 4000);
					if(pexception==NULL) 
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_UNPLUG, VAL_VERT));
						ProcessSystemEvents ();
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_UNPLUG, VAL_ROUGE));
						EXCDISPLAY( pexception);  
					}
					EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_PLUG, VAL_GRIS));
					
				break;
				
				
				case TABPANEL_2_CMD_RELAY_2B:
				    GetCtrlVal(panel, TABPANEL_2_CMD_RELAY_2B, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_2B",Etat);
					if(pexception==NULL) 
					{  
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_2B, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_2B, VAL_GRIS));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
											
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_2B, VAL_ROUGE));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_DESACTIV_230, VAL_GRIS));
					
				break;
						
				case TABPANEL_2_CMD_RELAY_2C:
				    GetCtrlVal(panel, TABPANEL_2_CMD_RELAY_2C, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_2C",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_2C, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_2C, VAL_GRIS));	
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_2C, VAL_ROUGE));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
				
				case TABPANEL_2_CMD_RELAY_2D:
				    GetCtrlVal(panel, TABPANEL_2_CMD_RELAY_2D, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_2D",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_2D, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_2D, VAL_GRIS));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_2D, VAL_ROUGE));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
				
				case TABPANEL_2_CMD_RELAY_2E:
				    GetCtrlVal(panel, TABPANEL_2_CMD_RELAY_2E, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_2E",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_2E, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_2E, VAL_GRIS));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_2E, VAL_ROUGE));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
				case TABPANEL_2_CMD_RELAY_2B_2C:
				    GetCtrlVal(panel, TABPANEL_2_CMD_RELAY_2B_2C, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_2D",0);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_2E",0);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_2B",Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_2C",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_2C_2B, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_2C_2B, VAL_GRIS));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_2C_2B, VAL_ROUGE));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
			case TABPANEL_2_CMD_RELAY_2B_2D:
				    GetCtrlVal(panel, TABPANEL_2_CMD_RELAY_2B_2D, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_2C",0);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_2E",0);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_2B",Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_2D",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_2B_2D, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_2B_2D, VAL_GRIS));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_2B_2D, VAL_ROUGE));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
				case TABPANEL_2_CMD_RELAY_2E_2D:
				    GetCtrlVal(panel, TABPANEL_2_CMD_RELAY_2E_2D, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_2C",0);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_2B",0);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_2E",Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_2D",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_2E_2D, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_2E_2D, VAL_GRIS));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_2E_2D, VAL_ROUGE));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
					
							
				case TABPANEL_2_DISCONNECT_ALL:
				
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_2B",FALSE);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_2C",FALSE);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_2D",FALSE);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_2E",FALSE);
					if(pexception==NULL) 
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_DISCONNECT_ALL, VAL_VERT));
						ProcessSystemEvents ();
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_DISCONNECT_ALL, VAL_ROUGE));
						EXCDISPLAY( pexception);  
					}	
					
					EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_2B, VAL_GRIS));
					EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_2C, VAL_GRIS));
					EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_2D, VAL_GRIS));
					EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_2_LED_2E, VAL_GRIS));
					
				break;		
					
			}
		break;
	}
			
Error:
	SetWaitCursor (0);
	if (pTestParam != NULL)
		free(pTestParam);
	EXCDISPLAY( pexception);
	return 0;
}

#undef __FUNC__
#define __FUNC__ "OnFncStation3"
int CVICALLBACK OnFncStation3 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int Etat;
	
	SElExceptionPtr 	pexception = NULL;
	int32_t				error = 0;
	char				buffer[512];
	int					position = 0;
	int                 push;
	STestParamPtr		pTestParam = NULL;
	bool_t          	microCam, presence;
	
	SetWaitCursor (1);

	switch (event)
	{
		case EVENT_COMMIT:

			switch(control)
			{
				
			case TABPANEL_3_PLUG:
				
					pexception = gs_pTester->PlugPanelStation30(gs_pTester, 4000);
					if(pexception==NULL) 
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_PLUG, VAL_VERT));
						ProcessSystemEvents ();
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_PLUG, VAL_ROUGE));
						EXCDISPLAY( pexception);  						
					}
					EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_UNPLUG, VAL_GRIS));					
					
			break;
				
				case TABPANEL_3_UNPLUG:
				
					pexception = gs_pTester->UnplugPanelStation30(gs_pTester, 4000);
					if(pexception==NULL) 
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_UNPLUG, VAL_VERT));
						ProcessSystemEvents ();
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_UNPLUG, VAL_ROUGE));
						EXCDISPLAY( pexception);  
					}
					EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_PLUG, VAL_GRIS));
					
			break;			
			
				case TABPANEL_3_V_PLUG:
				    pexception = gs_pTester->VerticalPlugStation30(gs_pTester, 4000);
					//pexception = gs_pTester->SetSensor(gs_pTester,"CYLINDER_PLUG_V_STATION_30",TRUE);
					//pexception = gs_pTester->SetSensor(gs_pTester,"CYLINDER_UNPLUG_V_STATION_30",FALSE);
					
					if(pexception==NULL) 
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_V_PLUG, VAL_VERT));
						ProcessSystemEvents ();
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_V_PLUG, VAL_ROUGE));
						EXCDISPLAY( pexception);  
					}
					EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_V_UNPLUG, VAL_GRIS));
					
				break;
						
				case TABPANEL_3_V_UNPLUG:
					
				    pexception = gs_pTester->VerticalUnplugStation30(gs_pTester, 4000);
					//pexception = gs_pTester->SetSensor(gs_pTester,"CYLINDER_UNPLUG_V_STATION_30",TRUE);
					//pexception = gs_pTester->SetSensor(gs_pTester,"CYLINDER_PLUG_V_STATION_30",FALSE);
					if(pexception==NULL) 
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_V_UNPLUG, VAL_VERT));
						ProcessSystemEvents ();
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_V_UNPLUG, VAL_ROUGE));
						EXCDISPLAY( pexception);  
					}
					EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_V_PLUG, VAL_GRIS));
					
				break;
				
				
				case TABPANEL_3_CMD_RELAY_FTA1:
				    GetCtrlVal(panel, TABPANEL_3_CMD_RELAY_FTA1, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_FTA1",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_FTA1, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_FTA1, VAL_GRIS));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_FTA1, VAL_ROUGE));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
				case TABPANEL_3_CMD_RELAY_FTA2:
				    GetCtrlVal(panel, TABPANEL_3_CMD_RELAY_FTA2, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_FTA2",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_FTA2, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_FTA2, VAL_GRIS));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_FTA2, VAL_ROUGE));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
				case TABPANEL_3_CMD_RELAY_FTA3:
				    GetCtrlVal(panel, TABPANEL_3_CMD_RELAY_FTA3, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_FTA3",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_FTA3, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_FTA3, VAL_GRIS));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_FTA3, VAL_ROUGE));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
				case TABPANEL_3_CMD_RELAY_FTA4:
				    GetCtrlVal(panel, TABPANEL_3_CMD_RELAY_FTA4, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_FTA4",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_FTA4, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_FTA4, VAL_GRIS));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_FTA4, VAL_ROUGE));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
				
				case TABPANEL_3_CMD_RELAY_DBUS_3F:
				    GetCtrlVal(panel, TABPANEL_3_CMD_RELAY_DBUS_3F, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_DBUS_Relay_3F",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3F, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3F, VAL_GRIS));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3F, VAL_ROUGE));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
				case TABPANEL_3_CMD_RELAY_3G:
				    GetCtrlVal(panel, TABPANEL_3_CMD_RELAY_3G, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_3G",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3G, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3G, VAL_GRIS));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3G, VAL_ROUGE));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
				
				case TABPANEL_3_CMD_RELAY_3H:
				    GetCtrlVal(panel, TABPANEL_3_CMD_RELAY_3H, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_3H",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3H, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3H, VAL_GRIS));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3H, VAL_ROUGE));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
				//3I
				case TABPANEL_3_CMD_RELAY_3I:
				    GetCtrlVal(panel, TABPANEL_3_CMD_RELAY_3I, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_3I",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3I, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3I, VAL_GRIS));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3I, VAL_ROUGE));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
				//3L
				case TABPANEL_3_CMD_RELAY_3L:
				    GetCtrlVal(panel, TABPANEL_3_CMD_RELAY_3L, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_3L",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3L, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3L, VAL_GRIS));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3L, VAL_ROUGE));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
				//3M
				case TABPANEL_3_CMD_RELAY_3M:
				    GetCtrlVal(panel, TABPANEL_3_CMD_RELAY_3M, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_3M",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3M, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3M, VAL_GRIS));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3M, VAL_ROUGE));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
				//3N
				case TABPANEL_3_CMD_RELAY_3N:
				    GetCtrlVal(panel, TABPANEL_3_CMD_RELAY_3N, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_3N",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3N, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3N, VAL_GRIS));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3N, VAL_ROUGE));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
				//3O
				case TABPANEL_3_CMD_RELAY_3O:
				    GetCtrlVal(panel, TABPANEL_3_CMD_RELAY_3O, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_3O",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3O, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3O, VAL_GRIS));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3O, VAL_ROUGE));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
				//3P
				case TABPANEL_3_CMD_RELAY_3P:
				    GetCtrlVal(panel, TABPANEL_3_CMD_RELAY_3P, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_3P",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3P, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3P, VAL_GRIS));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3P, VAL_ROUGE));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
				//3Q
				case TABPANEL_3_CMD_RELAY_3Q_3Q2:
				    GetCtrlVal(panel, TABPANEL_3_CMD_RELAY_3Q_3Q2, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_3Q_3Q2",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3Q, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3Q, VAL_GRIS));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3Q, VAL_ROUGE));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
				//3R
				case TABPANEL_3_CMD_RELAY_AC_PSupply:
				    GetCtrlVal(panel, TABPANEL_3_CMD_RELAY_AC_PSupply, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_AC_PowerSupply_Relay_3R",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3R, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3R, VAL_GRIS));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_3R, VAL_ROUGE));
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
				
				//--------------------------------
				case TABPANEL_3_DISCONNECT_ALL:
				
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_FTA1",FALSE);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_FTA1",FALSE);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_FTA1",FALSE);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_FTA1",FALSE);
					if(pexception==NULL) 
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_VERT));
						ProcessSystemEvents ();
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_ROUGE));
						EXCDISPLAY( pexception);  
					}	
					
					EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_FTA1, VAL_GRIS));
					EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_FTA2, VAL_GRIS));
					EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_FTA3, VAL_GRIS));
					EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_FTA4, VAL_GRIS));
					
				break;		
				
			}
		break;
	}
			
Error:
	SetWaitCursor (0);
	if (pTestParam != NULL)
		free(pTestParam);
	EXCDISPLAY( pexception);
	return 0;
}
				
#undef __FUNC__
#define __FUNC__ "OnFncStation4"
int CVICALLBACK OnFncStation4 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int Etat;
	
	SElExceptionPtr 	pexception = NULL;
	int32_t				error = 0;
	char				buffer[512];
	int					position = 0;
	int                 push;
	STestParamPtr		pTestParam = NULL;
	bool_t          	microCam, presence;
	
	SetWaitCursor (1);

	switch (event)
	{
		case EVENT_COMMIT:

			switch(control)
			{
				
			case TABPANEL_4_PLUG:
				


					pexception = gs_pTester->PlugPanelStation40(gs_pTester, 4000);
					if(pexception==NULL) 
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_PLUG, VAL_VERT));
						ProcessSystemEvents ();						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_PLUG, VAL_ROUGE));
						EXCDISPLAY( pexception);  						
					}
					EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_UNPLUG, VAL_GRIS));	
					
			break;
				
				case TABPANEL_4_UNPLUG:
				
					pexception = gs_pTester->UnplugPanelStation40(gs_pTester, 4000);
					if(pexception==NULL) 
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_UNPLUG, VAL_VERT));
						ProcessSystemEvents ();
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_UNPLUG, VAL_ROUGE));
						EXCDISPLAY( pexception);  
					}
					EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_PLUG, VAL_GRIS));
					
				break;	
			//----------------------
			case TABPANEL_4_CMD_RELAY_4A:
				    GetCtrlVal(panel, TABPANEL_4_CMD_RELAY_4A, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_4A",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4A, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4A, VAL_GRIS));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4A, VAL_ROUGE));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
				
				case TABPANEL_4_CMD_RELAY_4B:
				    GetCtrlVal(panel, TABPANEL_4_CMD_RELAY_4B, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_4B",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4B, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4B, VAL_GRIS));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4B, VAL_ROUGE));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
				
				case TABPANEL_4_CMD_RELAY_4H:
				    GetCtrlVal(panel, TABPANEL_4_CMD_RELAY_4H, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_PowerSupply_Relay_4H",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4H, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4H, VAL_GRIS));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4H, VAL_ROUGE));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
				
				case TABPANEL_4_CMD_RELAY_4S:
				    GetCtrlVal(panel, TABPANEL_4_CMD_RELAY_4S, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_4S",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4S, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4S, VAL_GRIS));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4S, VAL_ROUGE));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
				case TABPANEL_4_CMD_RELAY_4T:
				    GetCtrlVal(panel, TABPANEL_4_CMD_RELAY_4T, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_4T",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4T, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4T, VAL_GRIS));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4T, VAL_ROUGE));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
				
				
				case TABPANEL_4_CMD_RELAY_4U:
				    GetCtrlVal(panel, TABPANEL_4_CMD_RELAY_4U, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_4U",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4U, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4U, VAL_GRIS));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4U, VAL_ROUGE));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
				
				case TABPANEL_4_CMD_RELAY_4V:
				    GetCtrlVal(panel, TABPANEL_4_CMD_RELAY_4V, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_4V_4V2",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4V, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4V, VAL_GRIS));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4V, VAL_ROUGE));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
				
				case TABPANEL_4_CMD_RELAY_4Z:
				    GetCtrlVal(panel, TABPANEL_4_CMD_RELAY_4Z, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_4Z",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4Z, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4Z, VAL_GRIS));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4Z, VAL_ROUGE));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
				case TABPANEL_4_CMD_RELAY_4W:
				    GetCtrlVal(panel, TABPANEL_4_CMD_RELAY_4W, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_4W",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4W, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4W, VAL_GRIS));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4W, VAL_ROUGE));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
				
				case TABPANEL_4_CMD_RELAY_4R:
				    GetCtrlVal(panel, TABPANEL_4_CMD_RELAY_4R, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_DBUS_Relay_4R",Etat);
					if(pexception==NULL) 
					{
						if(Etat==1)
					 	EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4R, VAL_VERT));
						else
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4R, VAL_GRIS));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();
						
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_4_LED_4R, VAL_ROUGE));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);  
					}
					
					
				break;
				
				
			//----------------------
				
						
				
			}
		break;
	}
			
Error:
	SetWaitCursor (0);
	if (pTestParam != NULL)
		free(pTestParam);
	EXCDISPLAY( pexception);
	return 0;
}

#undef __FUNC__
#define __FUNC__ "OnFncStation5"
int CVICALLBACK OnFncStation5 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int Etat;
	
	SElExceptionPtr 	pexception = NULL;
	int32_t				error = 0;
	char				buffer[512];
	int					position = 0;
	int                 push;
	STestParamPtr		pTestParam = NULL;
	bool_t          	microCam, presence;
	int                 output0,
		                output1,
		                output2,
		                output3;
	SetWaitCursor (1);

	switch (event)
	{
		case EVENT_COMMIT:

			switch(control)
			{

				case TABPANEL_5_PLUG:



					pexception = gs_pTester->PlugPanelStation50(gs_pTester, 4000);
					if(pexception==NULL)
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_PLUG, VAL_VERT));
						ProcessSystemEvents ();
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_PLUG, VAL_ROUGE));
						EXCDISPLAY( pexception);
					}
					EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_UNPLUG, VAL_GRIS));

					break;

				case TABPANEL_5_UNPLUG:

					pexception = gs_pTester->UnplugPanelStation50(gs_pTester, 4000);
					if(pexception==NULL)
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_UNPLUG, VAL_VERT));
						ProcessSystemEvents ();
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_UNPLUG, VAL_ROUGE));
						EXCDISPLAY( pexception);
					}
					EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_PLUG, VAL_GRIS));

					break;

				case TABPANEL_5_CMD_RELAY_5C:
					GetCtrlVal(panel, TABPANEL_5_CMD_RELAY_5C, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_5C",Etat);
					if(pexception==NULL)
					{
						if(Etat==1)
							EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_5C, VAL_VERT));
						else
							EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_5C, VAL_GRIS));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();

					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_5C, VAL_ROUGE));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);
					}


					break;
				case TABPANEL_5_CMD_RELAY_5D:
					GetCtrlVal(panel, TABPANEL_5_CMD_RELAY_5D, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_5D",Etat);
					if(pexception==NULL)
					{
						if(Etat==1)
							EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_5D, VAL_VERT));
						else
							EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_5D, VAL_GRIS));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();

					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_5D, VAL_ROUGE));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);
					}


					break;
				case TABPANEL_5_CMD_RELAY_5E:
					GetCtrlVal(panel, TABPANEL_5_CMD_RELAY_5E, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_5E",Etat);
					if(pexception==NULL)
					{
						if(Etat==1)
							EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_5E, VAL_VERT));
						else
							EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_5E, VAL_GRIS));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();

					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_5E, VAL_ROUGE));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);
					}


					break;

				case TABPANEL_5_CMD_RELAY_5F:
					GetCtrlVal(panel, TABPANEL_5_CMD_RELAY_5F, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_5F",Etat);
					if(pexception==NULL)
					{
						if(Etat==1)
							EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_5F, VAL_VERT));
						else
							EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_5F, VAL_GRIS));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();

					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_5F, VAL_ROUGE));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);
					}


					break;
					//-----------
				case TABPANEL_5_CMD_RELAY_5I:
					GetCtrlVal(panel, TABPANEL_5_CMD_RELAY_5I, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_5I",Etat);
					if(pexception==NULL)
					{
						if(Etat==1)
							EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_5I, VAL_VERT));
						else
							EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_5I, VAL_GRIS));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();

					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_5I, VAL_ROUGE));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);
					}


					break;
				case TABPANEL_5_CMD_RELAY_5M:
					GetCtrlVal(panel, TABPANEL_5_CMD_RELAY_5M, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_5M",Etat);
					if(pexception==NULL)
					{
						if(Etat==1)
							EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_5M, VAL_VERT));
						else
							EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_5M, VAL_GRIS));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();

					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_5M, VAL_ROUGE));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);
					}


					break;
				case TABPANEL_5_CMD_RELAY_5N:
					GetCtrlVal(panel, TABPANEL_5_CMD_RELAY_5N, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_5N",Etat);
					if(pexception==NULL)
					{
						if(Etat==1)
							EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_5N, VAL_VERT));
						else
							EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_5N, VAL_GRIS));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();

					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_5N, VAL_ROUGE));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);
					}


					break;
				case TABPANEL_5_CMD_RELAY_5G:
					GetCtrlVal(panel, TABPANEL_5_CMD_RELAY_5G, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_PowerSupply_Relay_5G",Etat);
					if(pexception==NULL)
					{
						if(Etat==1)
							EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_5G, VAL_VERT));
						else
							EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_5G, VAL_GRIS));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();

					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_5G, VAL_ROUGE));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);
					}


					break;

				case TABPANEL_5_CMD_RELAY_5H:
					GetCtrlVal(panel, TABPANEL_5_CMD_RELAY_5H, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_DBUS_Relay_5H",Etat);
					if(pexception==NULL)
					{
						if(Etat==1)
							EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_5H, VAL_VERT));
						else
							EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_5H, VAL_GRIS));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();

					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_5H, VAL_ROUGE));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);
					}


					break;

				case TABPANEL_5_CMD_RELAY_4T:
					GetCtrlVal(panel, TABPANEL_5_CMD_RELAY_4T, &Etat);
					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_Relay_4T",Etat);
					if(pexception==NULL)
					{
						if(Etat==1)
							EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_4T, VAL_VERT));
						else
							EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_4T, VAL_GRIS));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						ProcessSystemEvents ();

					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_5_LED_4T, VAL_ROUGE));
						//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_3_LED_DISCONNECT_ALL, VAL_GRIS));
						EXCDISPLAY( pexception);
					}


					break;


			}
			break;
	}

Error:
	SetWaitCursor (0);
	if (pTestParam != NULL)
		free(pTestParam);
	EXCDISPLAY( pexception);
	return 0;
}

#undef __FUNC__
#define __FUNC__ "OnFncStation6"
int CVICALLBACK OnFncStation6 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	SElExceptionPtr 	pexception = NULL;
	int32_t				error = 0;
	char				buffer[512];
	int					position = 0;
	int                 push;
	
	int                 output0,
		                output1,
		                output2,
		                output3;
	STestParamPtr		pTestParam = NULL;
	bool_t          	microCam, presence;
	
	SetWaitCursor (1);

	switch (event)
	{
		case EVENT_COMMIT:

			switch(control)
			{
					
					
				case TABPANEL_6_OPEN_GRIPPER:

					pexception = gs_pTester->OPEN_GRIPPER(gs_pTester, 5000);
					if(pexception==NULL)
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_GRIPPER_OPEN, VAL_VERT));
						ProcessSystemEvents ();
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_GRIPPER_OPEN, VAL_ROUGE));
						EXCDISPLAY( pexception);
					}
					EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_GRIPPER_CLOSED, VAL_GRIS));

					break;
					
					case TABPANEL_6_CLOSE_GRIPPER:

					pexception = gs_pTester->CLOSE_GRIPPER(gs_pTester, 4000);
					if(pexception==NULL)
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_GRIPPER_CLOSED, VAL_VERT));
						ProcessSystemEvents ();
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_GRIPPER_CLOSED, VAL_ROUGE));
						EXCDISPLAY( pexception);
					}
					EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_GRIPPER_OPEN, VAL_GRIS));

					break;
					
					case TABPANEL_6_ACTUATOR_X_REAR:

					pexception = gs_pTester->ACTUATOR_X_REAR(gs_pTester, 5000);
					if(pexception==NULL)
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_X_REAR_LED, VAL_VERT));
						ProcessSystemEvents ();
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_X_REAR_LED, VAL_ROUGE));
						EXCDISPLAY( pexception);
					}
					EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_X_FRONT_LED, VAL_GRIS));

					break;
					
					case TABPANEL_6_ACTUATOR_X_FRONT:

					pexception = gs_pTester->ACTUATOR_X_FRONT(gs_pTester, 5000);
					if(pexception==NULL)
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_X_FRONT_LED, VAL_VERT));
						ProcessSystemEvents ();
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_X_FRONT_LED, VAL_ROUGE));
						EXCDISPLAY( pexception);
					}
					EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_X_REAR_LED, VAL_GRIS));

					break;
					
					case TABPANEL_6_ACTUATOR_Z_UP:

					pexception = gs_pTester->ACTUATOR_Z_UP(gs_pTester, 4000);
					if(pexception==NULL)
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_Z_UP_LED, VAL_VERT));
						ProcessSystemEvents ();
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_Z_UP_LED, VAL_ROUGE));
						EXCDISPLAY( pexception);
					}
					EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_Z_DOWN_LED, VAL_GRIS));

					break;
					case TABPANEL_6_ACTUATOR_Z_DOWN:

					pexception = gs_pTester->ACTUATOR_Z_DOWN(gs_pTester, 4000);
					if(pexception==NULL)
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_Z_DOWN_LED, VAL_VERT));
						ProcessSystemEvents ();
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_Z_DOWN_LED, VAL_ROUGE));
						EXCDISPLAY( pexception);
					}
					EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_Z_UP_LED, VAL_GRIS));

					break;
					
					case TABPANEL_6_CMD_CONVOYER:

					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_CONVEYOR",1);
					if(pexception==NULL)
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_CONVOYER, VAL_VERT));
						ProcessSystemEvents ();
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_CONVOYER, VAL_ROUGE));
						EXCDISPLAY( pexception);
					}
                        EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_CONVOYER_STOP, VAL_GRIS));

					break;
					case TABPANEL_6_STOP_CONVOYER:

					pexception = gs_pTester->SetSensor(gs_pTester, "CMD_CONVEYOR",0);
					if(pexception==NULL)
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_CONVOYER_STOP, VAL_VERT));
						ProcessSystemEvents ();
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_CONVOYER_STOP, VAL_ROUGE));
						EXCDISPLAY( pexception);
					}
                    EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_CONVOYER, VAL_GRIS));

					break;
					
				
				
				case TABPANEL_6_PRINT:

					EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_READY, VAL_GRIS));

					pexception = gs_pTester->CyclePrint(gs_pTester, 6000);

					if(pexception==NULL)
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_PRINT, VAL_VERT));
						ProcessSystemEvents ();
					}
					else
					{
						EXCDISPLAY( pexception);
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_PRINT, VAL_ROUGE));
					}

					break;

				case TABPANEL_6_READY:

					EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_PRINT, VAL_GRIS));

					pexception = gs_pTester->ReadyPrint(gs_pTester, 2000);

					if(pexception==NULL)
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_READY, VAL_VERT));
						ProcessSystemEvents ();
					}
					else
					{
						EXCDISPLAY( pexception);
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_READY, VAL_ROUGE));
					}

					break;

				case TABPANEL_6_BARCODE:

					DeleteTextBoxLines (panel, TABPANEL_6_CODE_STRING, 0, -1);

					if (gs_pTester->pBarCodeReader)
						pexception = gs_pTester->GetItemCode(gs_pTester, 0, 1, 512, buffer, TIMEOUT);

					if(pexception==NULL)
					{
						EXCCHECKCVI( SetCtrlVal(panel, TABPANEL_6_CODE_STRING, buffer));
					}
					else
					{
						EXCDISPLAY( pexception);
						EXCCHECKCVI( SetCtrlVal(panel, TABPANEL_6_CODE_STRING, "#error"));
					}

					break;


					//------


				case TABPANEL_6_PICK_PLACE:

					pexception = gs_pTester->PickPlacePanelStation60(gs_pTester, 20000);
					if(pexception==NULL)
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_PICK_PLACE, VAL_VERT));
						ProcessSystemEvents ();
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_PICK_PLACE, VAL_ROUGE));
						EXCDISPLAY( pexception);
					}
					EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_HOME_PICK_PLACE, VAL_GRIS));

					break;

				case TABPANEL_6_HOME_PICK_PLACE:

					pexception = gs_pTester->HomePickPlacePanelStation60(gs_pTester, 20000);
					if(pexception==NULL)
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_HOME_PICK_PLACE, VAL_VERT));
						ProcessSystemEvents ();
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_HOME_PICK_PLACE, VAL_ROUGE));
						EXCDISPLAY( pexception);
					}
					EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_PICK_PLACE, VAL_GRIS));

					break;
					
				case TABPANEL_6_PICK_PRINT:

					pexception = gs_pTester->PickPrintPanelStation60(gs_pTester, 200000);
					if(pexception==NULL)
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_PICK_PRINT, VAL_VERT));
						ProcessSystemEvents ();
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_PICK_PRINT, VAL_ROUGE));
						EXCDISPLAY( pexception);
					}
					//EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_HOME_PICK_PLACE, VAL_GRIS));

					break;
				case TABPANEL_6_PICK_READ:

					pexception = gs_pTester->PickPrintPanelStation60(gs_pTester, 20000);
					if(pexception==NULL)
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_PICK_PRINT, VAL_VERT));
						ProcessSystemEvents ();
						pexception = gs_pTester->ACTUATOR_X_REAR(gs_pTester, 20000);
					}
					
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_PICK_PRINT, VAL_ROUGE));
						EXCDISPLAY( pexception);
					}
					pexception = gs_pTester->ACTUATOR_X_REAR(gs_pTester, 20000);
					if(pexception==NULL)
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_PICK_READ, VAL_VERT));
						ProcessSystemEvents ();
					}
					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_LED_PICK_READ, VAL_ROUGE));
						EXCDISPLAY( pexception);
					}
					

					break;

				/*case TABPANEL_6_HOME_PICK_READ:

					pexception = gs_pTester->ACTUATOR_X_FRONT(gs_pTester, 20000);
					if(pexception==NULL)
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_X_FRONT_LED, VAL_VERT));
						ProcessSystemEvents ();
					}

					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_X_FRONT_LED, VAL_ROUGE));
						EXCDISPLAY( pexception);
					}
					
					pexception = gs_pTester->ACTUATOR_Z_DOWN(gs_pTester, 20000);
					if(pexception==NULL)
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_Z_DOWN_LED, VAL_VERT));
						ProcessSystemEvents ();
					}

					else
					{
						EXCCHECKCVI( SetCtrlVal(panel,TABPANEL_6_Z_DOWN_LED, VAL_ROUGE));
						EXCDISPLAY( pexception);
					}
					
					
					

					break;
*/
				
			}
		break;
	}
			
Error:
	SetWaitCursor (0);
	if (pTestParam != NULL)
		free(pTestParam);
	EXCDISPLAY( pexception);
	return 0;
}

#undef __FUNC__
#define __FUNC__ "OnFncTester"
int CVICALLBACK OnFncTester (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int Response_popup = 0;
	SElExceptionPtr 	pexception = NULL;
	int32_t				error = 0;
	int					position = 0;
	int                 Check, Confirm;
	int 				i;
	
	switch (event)
		{
		case EVENT_COMMIT:

			switch(control)
			{
				case TABPANEL_ROTATE_TABLE:
					
					Check = TRUE;
					if (Check == TRUE)
					{
					Confirm =ConfirmPopup ("Attention Risque de Collision!", "Rotation Plateau Merci de vérifier les conditions visuellement, Continuer?");
					if (Confirm == 1)
						pexception = gs_pTester->TableRotate(gs_pTester, 10000, TRUE);
					}
					
					else 
					{
						pexception = gs_pTester->TableRotate(gs_pTester, 10000, FALSE);
					}
					
					if(pexception==NULL) 
						{
						EXCCHECKCVI( SetCtrlAttribute(panel, TABPANEL_TABLE_LED,ATTR_FRAME_COLOR, VAL_GREEN));
						}
				
					else
						{
						EXCDISPLAY( pexception);   
						EXCCHECKCVI( SetCtrlAttribute(panel, TABPANEL_TABLE_LED,ATTR_FRAME_COLOR, VAL_RED));
						}
				
				break;

				case TABPANEL_HOME_STATIONS:

					for (i=0; i < MAX_STATIONS; i++)
					{
						pexception = gs_pTester->InitializeStations(gs_pTester, i, 5000);
						if(pexception==NULL) 
							EXCCHECKCVI( SetCtrlAttribute(panel, TABPANEL_LED_HOME_STATIONS,ATTR_FRAME_COLOR, VAL_GREEN));
						else
							{
							EXCDISPLAY( pexception);   
							EXCCHECKCVI( SetCtrlAttribute(panel, TABPANEL_LED_HOME_STATIONS,ATTR_FRAME_COLOR, VAL_RED));
							break;
							}
					}
				
	
				break;

				
			}
			
			break;
		}
	
Error:
	EXCDISPLAY( pexception);
	return 0;
}


#define BKTREE_HEADER "root|DIO^w400$id|ID^w120$desc|Description^w628$state|^w24\n"
#define OUTPUT_STR			"output"
#define INPUT_STR			"input"

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ClearTree"
static SElExceptionPtr ClearTree(CAObjHandle hTree)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL;
	HRESULT         hr = 0;
	short			AXvalue;

	EXCCHECKCOM( BKLib2__DBKTreeFromString ( hTree, NULL, BKTREE_HEADER, "$", "\n", &AXvalue));
	
Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SetTree"
static SElExceptionPtr SetTree(
	const char* fileXml, 
	const char* GUID, 
	CAObjHandle hTree, 
	SLinePtr* pInput,
	SLinePtr* pOutput
)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL;
	HRESULT         hr = 0;
	char            _message[512];
	CAObjHandle     pxml  = 0;
	CAObjHandle     pnode = 0;
	CAObjHandle     pnode2 = 0;
	CAObjHandle     plist = 0; 
	CAObjHandle     pparameter = 0;
	const char*     pfile_xml = (fileXml) ? fileXml : "../modules/tester.xml";
	VBOOL           is_xml_success;
	VARIANT         pfile_name_var = CA_VariantEmpty();
	char*           ptext = NULL;
	int32_t			length, i, j;
	SELStringPtr 	pelstring = NULL;
	short			AXvalue;
	char			path[512];
	ERRORINFO		errorInfo;

	EXCCHECK( elstring_new( BKTREE_HEADER, &pelstring));
	
	/* configure test sequence from XML */
	EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml));
	EXCCHECKCVI( CA_VariantSetCString(  &pfile_name_var, pfile_xml));
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (pxml, &errorInfo, pfile_name_var, &is_xml_success));

	sprintf(path, "//module[@id='%s']/Sensors", GUID);
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, path, &pnode));	
	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectNodes (pnode, NULL, "Sensor[@type='DI']", &plist));
	EXCCHECKMSXML( MSXML_IXMLDOMNodeListGetlength (plist, NULL, &length));
	EXCCHECKMSXML( MSXML_IXMLDOMNodeListreset (plist, NULL));

	if(pInput && *pInput)
	{
		free( *pInput);
		*pInput = NULL;
	}
	if(length>0)
	{
		if(pInput) *pInput = calloc(length+1, sizeof(SLine));
	}

	EXCCHECK( pelstring->Append(pelstring, "|"INPUT_STR"|Inputs\n"));

	for( i=0; i<length; i++)
	{
		EXCCHECK( pelstring->Format(pelstring, ""INPUT_STR"|%d|", i));    
		if(pInput) sprintf((*pInput)[i].keyNode, "%d", i);

		EXCCHECKMSXML( MSXML_IXMLDOMNodeListnextNode (plist, NULL, &pnode2));
		
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode2, NULL, "@name", &pparameter));  
		if ( pparameter )
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
			
			EXCCHECK( pelstring->Append(pelstring, ptext));
			EXCCHECK( pelstring->Append(pelstring, "$"));
				
			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
		
		if ( pnode2 )
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pnode2, NULL, &ptext));
			
			EXCCHECK( pelstring->Append(pelstring, ptext));
			EXCCHECK( pelstring->Append(pelstring, "$"));
		
			if(pInput) strncpy((*pInput)[i].channel, ptext, LINE_NAME_LENGTH);

			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
		
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode2, NULL, "@description", &pparameter));  
		if ( pparameter )
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
			
			EXCCHECK( pelstring->Append(pelstring, ptext));
			EXCCHECK( pelstring->Append(pelstring, "$"));
			
			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
			CA_FreeMemory(ptext);
			ptext=NULL;
		}

		EXCCHECK( pelstring->Append(pelstring, "^i0\n"));
		
		if (pnode2) CA_DiscardObjHandle(pnode2);
	}
	
	if (plist) CA_DiscardObjHandle(plist);

	
	
	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectNodes (pnode, NULL, "Sensor[@type='DO']", &plist));
	EXCCHECKMSXML( MSXML_IXMLDOMNodeListGetlength (plist, NULL, &length));
	EXCCHECKMSXML( MSXML_IXMLDOMNodeListreset (plist, NULL));
	
	if(pOutput && *pOutput)
	{
		free( *pOutput);
		*pOutput = NULL;
	}
	if(length>0)
	{
		if(pOutput) *pOutput = calloc(length+1, sizeof(SLine));
	}

	EXCCHECK( pelstring->Append(pelstring, "|"OUTPUT_STR"|Outputs\n"));

	for( j=0; j<length; j++)
	{
		EXCCHECK( pelstring->Format(pelstring, ""OUTPUT_STR"|%d|", i+j)); 
		if(pOutput) sprintf((*pOutput)[j].keyNode, "%d", i+j);
		
		EXCCHECKMSXML( MSXML_IXMLDOMNodeListnextNode (plist, NULL, &pnode2));
		
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode2, NULL, "@name", &pparameter));  
		if ( pparameter )
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
			
			EXCCHECK( pelstring->Append(pelstring, ptext));
			EXCCHECK( pelstring->Append(pelstring, "$"));
				
			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
		
		if ( pnode2 )
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pnode2, NULL, &ptext));
			
			EXCCHECK( pelstring->Append(pelstring, ptext));
			EXCCHECK( pelstring->Append(pelstring, "$"));
		
			if(pOutput) strncpy((*pOutput)[j].channel, ptext, LINE_NAME_LENGTH);

			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
		
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode2, NULL, "@description", &pparameter));  
		if ( pparameter )
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
			
			EXCCHECK( pelstring->Append(pelstring, ptext));
			EXCCHECK( pelstring->Append(pelstring, "$"));
			
			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
	
		EXCCHECK( pelstring->Append(pelstring, "^i0^d0\n"));
		
		if (pnode2) CA_DiscardObjHandle(pnode2);
	}
	if (plist) CA_DiscardObjHandle(plist);

	EXCCHECKCOM( BKLib2__DBKTreeFromString ( hTree, NULL, pelstring->GetPtr(pelstring), "$", "\n", &AXvalue));
	EXCCHECKCOM( BKLib2__DBKTreeSetEnableDragAndDrop ( hTree, NULL, VFALSE));
	EXCCHECKCOM( BKLib2__DBKTreeExpandLevel ( hTree, NULL, 2, &AXvalue));
	
Error:
	elstring_delete( &pelstring);
	CA_FreeMemory(ptext);
	CA_VariantClear(&pfile_name_var);
	if (pparameter) CA_DiscardObjHandle(pparameter);
	if (pnode) CA_DiscardObjHandle(pnode);
	if (pxml) CA_DiscardObjHandle(pxml);  
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SetTreeStations"
static SElExceptionPtr SetTreeStations(
	const char* fileXml, 
	const char* GUID, 
	CAObjHandle hTree, 
	SLinePtr* pInput,
	SLinePtr* pOutput,
	int Station
)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL;
	HRESULT         hr = 0;
	char            _message[512];
	CAObjHandle     pxml  = 0;
	CAObjHandle     pnode = 0;
	CAObjHandle     pnode2 = 0;
	CAObjHandle     plist = 0; 
	CAObjHandle     pparameter = 0;
	const char*     pfile_xml = (fileXml) ? fileXml : "../modules/tester.xml";
	VBOOL           is_xml_success;
	VARIANT         pfile_name_var = CA_VariantEmpty();
	char*           ptext = NULL;
	int32_t			length, i, j;
	SELStringPtr 	pelstring = NULL;
	short			AXvalue;
	char			path[512];
	ERRORINFO		errorInfo;

	EXCCHECK( elstring_new( BKTREE_HEADER, &pelstring));
	
	/* configure test sequence from XML */
	EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml));
	EXCCHECKCVI( CA_VariantSetCString(  &pfile_name_var, pfile_xml));
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (pxml, &errorInfo, pfile_name_var, &is_xml_success));

	sprintf(path, "//module[@id='%s']/Station%d", GUID,Station+1);
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, path, &pnode));	
	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectNodes (pnode, NULL, "Sensor[@type='DI']", &plist));
	EXCCHECKMSXML( MSXML_IXMLDOMNodeListGetlength (plist, NULL, &length));
	EXCCHECKMSXML( MSXML_IXMLDOMNodeListreset (plist, NULL));

	if(pInput && *pInput)
	{
		free( *pInput);
		*pInput = NULL;
	}
	if(length>0)
	{
		if(pInput) *pInput = calloc(length+1, sizeof(SLine));
	}

	EXCCHECK( pelstring->Append(pelstring, "|"INPUT_STR"|Inputs\n"));

	for( i=0; i<length; i++)
	{
		EXCCHECK( pelstring->Format(pelstring, ""INPUT_STR"|%d|", i));    
		if(pInput) sprintf((*pInput)[i].keyNode, "%d", i);

		EXCCHECKMSXML( MSXML_IXMLDOMNodeListnextNode (plist, NULL, &pnode2));
		
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode2, NULL, "@name", &pparameter));  
		if ( pparameter )
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
			
			EXCCHECK( pelstring->Append(pelstring, ptext));
			EXCCHECK( pelstring->Append(pelstring, "$"));
				
			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
		
		if ( pnode2 )
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pnode2, NULL, &ptext));
			
			EXCCHECK( pelstring->Append(pelstring, ptext));
			EXCCHECK( pelstring->Append(pelstring, "$"));
		
			if(pInput) strncpy((*pInput)[i].channel, ptext, LINE_NAME_LENGTH);

			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
		
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode2, NULL, "@description", &pparameter));  
		if ( pparameter )
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
			
			EXCCHECK( pelstring->Append(pelstring, ptext));
			EXCCHECK( pelstring->Append(pelstring, "$"));
			
			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
			CA_FreeMemory(ptext);
			ptext=NULL;
		}

		EXCCHECK( pelstring->Append(pelstring, "^i0\n"));
		
		if (pnode2) CA_DiscardObjHandle(pnode2);
	}
	if (plist) CA_DiscardObjHandle(plist);
	
	
	
	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectNodes (pnode, NULL, "Sensor[@type='DO']", &plist));
	EXCCHECKMSXML( MSXML_IXMLDOMNodeListGetlength (plist, NULL, &length));
	EXCCHECKMSXML( MSXML_IXMLDOMNodeListreset (plist, NULL));
	
	if(pOutput && *pOutput)
	{
		free( *pOutput);
		*pOutput = NULL;
	}
	if(length>0)
	{
		if(pOutput) *pOutput = calloc(length+1, sizeof(SLine));
	}

	EXCCHECK( pelstring->Append(pelstring, "|"OUTPUT_STR"|Outputs\n"));

	for( j=0; j<length; j++)
	{
		EXCCHECK( pelstring->Format(pelstring, ""OUTPUT_STR"|%d|", i+j)); 
		if(pOutput) sprintf((*pOutput)[j].keyNode, "%d", i+j);
		
		EXCCHECKMSXML( MSXML_IXMLDOMNodeListnextNode (plist, NULL, &pnode2));
		
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode2, NULL, "@name", &pparameter));  
		if ( pparameter )
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
			
			EXCCHECK( pelstring->Append(pelstring, ptext));
			EXCCHECK( pelstring->Append(pelstring, "$"));
				
			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
		
		if ( pnode2 )
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pnode2, NULL, &ptext));
			
			EXCCHECK( pelstring->Append(pelstring, ptext));
			EXCCHECK( pelstring->Append(pelstring, "$"));
		
			if(pOutput) strncpy((*pOutput)[j].channel, ptext, LINE_NAME_LENGTH);

			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
		
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode2, NULL, "@description", &pparameter));  
		if ( pparameter )
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
			
			EXCCHECK( pelstring->Append(pelstring, ptext));
			EXCCHECK( pelstring->Append(pelstring, "$"));
			
			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
	
		EXCCHECK( pelstring->Append(pelstring, "^i0^d0\n"));
		
		if (pnode2) CA_DiscardObjHandle(pnode2);
	}
	if (plist) CA_DiscardObjHandle(plist);

	
	EXCCHECKCOM( BKLib2__DBKTreeFromString ( hTree, NULL, pelstring->GetPtr(pelstring), "$", "\n", &AXvalue));
	EXCCHECKCOM( BKLib2__DBKTreeSetEnableDragAndDrop ( hTree, NULL, VFALSE));
	EXCCHECKCOM( BKLib2__DBKTreeExpandLevel ( hTree, NULL, 2, &AXvalue));
	
Error:
	elstring_delete( &pelstring);
	CA_FreeMemory(ptext);
	CA_VariantClear(&pfile_name_var);
	if (pparameter) CA_DiscardObjHandle(pparameter);
	if (pnode) CA_DiscardObjHandle(pnode);
	if (pxml) CA_DiscardObjHandle(pxml);  
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "GetOutputStates"
static SElExceptionPtr GetOutputStates(SLinePtr pOutput)
{
	HRESULT			hr;
	SElExceptionPtr	pexception = NULL;
	uint32_t		state = 0;
	int32_t			i;

	if(pOutput)
	{
		i = 0;
		while(0 != strlen(pOutput[i].keyNode))
		{
			EXCCHECK( gs_pSensors->GetState(gs_pSensors, pOutput[i].channel, &state));
			
			
			EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemImage(gs_hTreeTester, 
														 NULL,  
														 pOutput[i].keyNode, 
														 "state", state));
			
			EXCCHECKCOM(BKLib2__DBKTreeSetNodeItemBackColor (gs_hTreeTester, NULL, pOutput[i].keyNode, "state",
															state == 1? 0x00FF00L : 0xFFFFFFL)); /* to select between green and white*/ //HOUSSEM

			EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemData(gs_hTreeTester, 
														 NULL, 
														 pOutput[i].keyNode, 
														 "state",  state));
			i++;
		};
	}

Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "GetOutputStatesStation"
static SElExceptionPtr GetOutputStatesStation(SLinePtr pOutput)
{
	HRESULT			hr;
	SElExceptionPtr	pexception = NULL;
	uint32_t		state = 0;
	int32_t			i;

	if(pOutput)
	{
		i = 0;
		while(0 != strlen(pOutput[i].keyNode))
		{
			EXCCHECK( gs_pSensors->GetState(gs_pSensors, pOutput[i].channel, &state));
			
			EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemImage(gs_hTreeStation[gs_Sation], 
														 NULL,  
														 pOutput[i].keyNode, 
														 "state", 
														 state));
			
			EXCCHECKCOM(BKLib2__DBKTreeSetNodeItemBackColor (gs_hTreeStation[gs_Sation], NULL, pOutput[i].keyNode, "state",
															state == 1? 0x00FF00L : 0xFFFFFFL)); /* to select between green and white*/ //HOUSSEM

			EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemData(gs_hTreeStation[gs_Sation], 
														 NULL, 
														 pOutput[i].keyNode, 
														 "state", 
														 state));
			i++;
		};
	}

Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "BkTreeNodeClickTester"
static HRESULT CVICALLBACK BkTreeNodeClickTester (
   CAObjHandle caServerObjHandle,
    void *caCallbackData,
    char *keyNode
)
{
	SElExceptionPtr pexception =  NULL; 
   	int32_t      	error = 0;
	HRESULT			hr;
	char*			parentNode = NULL;
	char*			id = NULL;
	int32_t			state;


    EXCCHECKCOM( BKLib2__DBKTreeGetParentNode(gs_hTreeTester, NULL, keyNode, &parentNode));

	if( 0==strcmp(parentNode, OUTPUT_STR))
	{
		BKLib2Type_OLE_COLOR color ; 
		
		/* set output */
		EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemData(gs_hTreeTester, NULL, keyNode, "state", &state));
		EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemText(gs_hTreeTester, NULL, keyNode, "id", &id));
		
		EXCCHECK( gs_pSensors->SetState(gs_pSensors, id, !state));
		EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemData(gs_hTreeTester, NULL, keyNode, "state", !state));
		EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemImage(gs_hTreeTester, NULL, keyNode, "state", !state));
		EXCCHECKCOM(BKLib2__DBKTreeSetNodeItemBackColor (gs_hTreeTester, NULL, keyNode, "state",
														 state == 0? 0x00FF00L : 0xFFFFFFL)); 
	}

Error: 
	EXCDISPLAY( pexception);
   	return 0;
}														

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "BkTreeNodeClickStation"
static HRESULT CVICALLBACK BkTreeNodeClickStation (
   CAObjHandle caServerObjHandle,
    void *caCallbackData,
    char *keyNode
)
{
	SElExceptionPtr pexception =  NULL; 
   	int32_t      	error = 0;
	HRESULT			hr;
	char*			parentNode = NULL;
	char*			id = NULL;
	int32_t			state;
	int station  =  (int)caCallbackData ;
		
    EXCCHECKCOM( BKLib2__DBKTreeGetParentNode(gs_hTreeStation[station], NULL, keyNode, &parentNode));

	if( 0==strcmp(parentNode, OUTPUT_STR))
	{
		/* set output */
		EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemData(gs_hTreeStation[station], NULL, keyNode, "state", &state));
		EXCCHECKCOM( BKLib2__DBKTreeGetNodeItemText(gs_hTreeStation[station], NULL, keyNode, "id", &id));

		EXCCHECK( gs_pSensors->SetState(gs_pSensors, id, !state));
		EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemData(gs_hTreeStation[station], NULL, keyNode, "state", !state));
		EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemImage(gs_hTreeStation[station], NULL, keyNode, "state", !state));
		EXCCHECKCOM(BKLib2__DBKTreeSetNodeItemBackColor (gs_hTreeStation[station], NULL, keyNode, "state",
														 state == 0? 0x00FF00L : 0xFFFFFFL)); 
	}

Error: 
	EXCDISPLAY( pexception);
   	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PostDeferredDisplayStation"
void CVICALLBACK PostDeferredDisplayStation (void *callbackData) 
{ 
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	int station  =  (int)callbackData ;

	EXCCHECK( GetOutputStatesStation( gs_pOutputStation[station]));

Error:
	EXCDISPLAY(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PostDeferredDisplayPanel"
void CVICALLBACK PostDeferredDisplayPanel (void *callbackData) 
{ 
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;

	EXCCHECK( GetOutputStates(gs_pOutputTester));

Error:
	EXCDISPLAY(pexception);
}

/*---------------------------------------------------------------------------*/
void CVICALLBACK PostDeferredHidePanel (void *callbackData) 
{ 
	int *panel = (int*)callbackData;
	if(*panel)
		HidePanel(*panel);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ShowPanel"
static SElExceptionPtr ShowPanel(void)
{
	HRESULT			hr;
	int32_t         error = 0;
   	SElExceptionPtr pexception = NULL;
	int32_t			visible;
	int32_t         page;
	
	
	EXCCHECKCVI( GetPanelAttribute( gs_panel[TESTER], ATTR_VISIBLE, &visible));
	if(!visible)
	{
		EXCCHECKCVI( DisplayPanel(gs_panel[TESTER]));
		GetActiveTabPage (gs_panel[TESTER], TESTER_TAB, &page);
		EXCCHECK(Tab_Change(page));
		/*
		EXCCHECKCOM( BKLib2__DBKTreeSetSelectedItem (gs_hTreeTester, NULL, INPUT_STR));
		EXCCHECK( GetOutputStates(gs_pOutputTester));
		EXCCHECK( ThreadStart( ThreadProcTester, gs_pInputTester, &gs_hThread[THREAD_TESTER], &gs_hEventStop[THREAD_TESTER]));   //ARAIBIA
		*/
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
	int				visible;
	int				i;

	EXCCHECKCVI( GetPanelAttribute (gs_panel[TESTER], ATTR_VISIBLE, &visible));
	if(visible)
	{
	  	EXCCHECKCVI( HidePanel(gs_panel[TESTER])); 

		for(i=0;i<NB_OF_THREAD;i++)
			EXCCHECK( ThreadStop(&gs_hThread[i], &gs_hEventStop[i]));
	}
	
Error:      
   EXCRETHROW( pexception);
}


int CVICALLBACK Timer_Printer (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	static bool_t printer_input1;
	static bool_t printer_input2;
	static bool_t printer_input3;
	static bool_t printer_input4;
	static bool_t printer_input5;
	
	switch (event)
	{
		case EVENT_TIMER_TICK:

		if (gs_pTester)
		{	
			gs_pTester->GetSensor(gs_pTester, "PRINTER_INPUT1", &printer_input1);
			gs_pTester->GetSensor(gs_pTester, "PRINTER_INPUT2", &printer_input2);
			gs_pTester->GetSensor(gs_pTester, "PRINTER_INPUT3", &printer_input3);
			gs_pTester->GetSensor(gs_pTester, "PRINTER_INPUT4", &printer_input4);
			gs_pTester->GetSensor(gs_pTester, "PRINTER_INPUT5", &printer_input5);
		}
		
		
			
			break;
	}
	return 0;
}


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PostDeferredCallBkTreeTester"
void CVICALLBACK PostDeferredCallBkTreeTester(void *callbackData)
{
	SElExceptionPtr	pexception = NULL;
	HRESULT			hr;
	int32_t			i;
	uint32_t		state;
	
	if(gs_pInputTester)
	{
		i = 0;
		while(0 != strlen(gs_pInputTester[i].keyNode))
		{
		BKLib2__DBKTreeSetNodeItemImage(gs_hTreeTester, 
														 NULL,  
														 gs_pInputTester[i].keyNode, 
														 "state",gs_pInputTester[i].state);
		
		/* to select between green and white*/ //HOUSSEM 	
		EXCCHECKCOM(BKLib2__DBKTreeSetNodeItemBackColor (gs_hTreeTester, NULL, gs_pInputTester[i].keyNode, "state",
														  gs_pInputTester[i].state == 1? 0x00FF00L : 0xFFFFFFL)); 
			i++;
		};
	}

Error:
	EXCDISPLAY( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ThreadProcTester"
static DWORD WINAPI ThreadProcTester(LPVOID param)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	SLinePtr		pInput = (SLinePtr)param;
	int32_t			i = 0;
	uint32_t		state;
	bool_t			data_changed = FALSE;
	char            status[6+1] ;
	bool_t			Is_Ready = FALSE;

	memset (status, 0x00, 7);
	
	while ( TRUE)
	{
		if (WaitForSingleObject(gs_hEventStop[THREAD_TESTER], 500) == WAIT_OBJECT_0)
		{
			ExitThread(THREAD_EXIT_CODE);
		}

		i = 0;
		if (pInput)
		{
			while(0 != strlen(pInput[i].keyNode))
			{
				EXCCHECK( gs_pSensors->GetState(gs_pSensors, pInput[i].channel, &state));

				if(pInput[i].state != state)
				{
					pInput[i].state = state;
					data_changed = TRUE;
				}
			i++;
			};
		}

		if( data_changed)
		{
			EXCCHECKCVI( PostDeferredCall( PostDeferredCallBkTreeTester, NULL));
			data_changed = FALSE;
		}
			
		if( gs_pTester)
			gs_pTester->GetStationStatus(gs_pTester, status);
		
		for (i=0;i<MAX_STATIONS;i++)
		{
		Is_Ready = (int)status[i];
		SetCtrlVal (gs_panelTab[PAGE_ONE], gs_status[i], Is_Ready == 1 ? 0 : 1);
		}
	};

Error:
	EXCDISPLAY( pexception);
	ExitThread(0);
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__													  
#define __FUNC__ "PostDeferredCallBkTreeStation"
void CVICALLBACK PostDeferredCallBkTreeStation(void *callbackData)
{
	SElExceptionPtr	pexception = NULL;
	HRESULT			hr;
	int32_t			i;
	uint32_t		state;
	
	if(gs_pInputStation[gs_Sation])
	{
		i = 0;
		while(0 != strlen(gs_pInputStation[gs_Sation][i].keyNode))
		{
		EXCCHECKCOM( BKLib2__DBKTreeSetNodeItemImage(gs_hTreeStation[gs_Sation], 
														 NULL,  
														 gs_pInputStation[gs_Sation][i].keyNode, 
														 "state",gs_pInputStation[gs_Sation][i].state));
		
		/* to select between green and white*/ //HOUSSEM 	
		EXCCHECKCOM(BKLib2__DBKTreeSetNodeItemBackColor (gs_hTreeStation[gs_Sation], NULL,  gs_pInputStation[gs_Sation][i].keyNode, "state",
														  gs_pInputStation[gs_Sation][i].state == 1? 0x00FF00L : 0xFFFFFFL));
		i++;
		};
	}

Error:
	EXCDISPLAY( pexception);
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ThreadProcStation"
static DWORD WINAPI ThreadProcStation(LPVOID param)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	SLinePtr		pInput = (SLinePtr)param;
	int32_t			i = 0;
	uint32_t		state;
	HRESULT			hr;
	bool_t			data_changed = FALSE;
	
	while ( TRUE)
	{
		if (WaitForSingleObject(gs_hEventStop[gs_Thread_Id], 500) == WAIT_OBJECT_0)
		{
			ExitThread(THREAD_EXIT_CODE);
		}
		
		i = 0;
		while(0 != strlen(pInput[i].keyNode))
		{
			EXCCHECK( gs_pSensors->GetState(gs_pSensors, pInput[i].channel, &state));

			if(pInput[i].state != state)
			{
				pInput[i].state = state;
				data_changed = TRUE;
			}
			i++;
		};

		if( data_changed)
		{
			EXCCHECKCVI( PostDeferredCall( PostDeferredCallBkTreeStation, NULL));
			data_changed = FALSE;
		}
	};

Error:
	EXCDISPLAY( pexception);
	ExitThread(0);
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ChangeMode"
static SElExceptionPtr ChangeMode(EMode mode)
{
	int32_t         error = 0;
   	SElExceptionPtr pexception = NULL;
	
	switch(gs_mode)
	{
		case MODE_TESTER:
			EXCCHECK( ThreadStop(&gs_hThread[THREAD_TESTER], &gs_hEventStop[THREAD_TESTER]));
			break;
		case MODE_STATION_1:
			EXCCHECK( ThreadStop(&gs_hThread[THREAD_STATION_1], &gs_hEventStop[THREAD_STATION_1]));
			break;
		case MODE_STATION_2:
			EXCCHECK( ThreadStop(&gs_hThread[THREAD_STATION_2], &gs_hEventStop[THREAD_STATION_2]));
			break;
		case MODE_STATION_3:
			EXCCHECK( ThreadStop(&gs_hThread[THREAD_STATION_3], &gs_hEventStop[THREAD_STATION_3]));
			break;
		case MODE_STATION_4:
			EXCCHECK( ThreadStop(&gs_hThread[THREAD_STATION_4], &gs_hEventStop[THREAD_STATION_4]));
			break;
		case MODE_STATION_5:
			EXCCHECK( ThreadStop(&gs_hThread[THREAD_STATION_5], &gs_hEventStop[THREAD_STATION_5]));
			break;
		case MODE_STATION_6:
			EXCCHECK( ThreadStop(&gs_hThread[THREAD_STATION_6], &gs_hEventStop[THREAD_STATION_6]));
			break;
	}

	switch(mode)
	{
		case MODE_TESTER:
			EXCCHECK( ThreadStart( ThreadProcTester, gs_pInputTester, &gs_hThread[THREAD_TESTER], &gs_hEventStop[THREAD_TESTER])); 
		break;

		case MODE_STATION_1:
			gs_Sation = 0;
			gs_Thread_Id = THREAD_STATION_1;
			if(gs_pInputStation[gs_Sation])
			EXCCHECK( ThreadStart( ThreadProcStation, gs_pInputStation[gs_Sation], &gs_hThread[THREAD_STATION_1], &gs_hEventStop[THREAD_STATION_1]));
			EXCCHECKCVI( PostDeferredCall( PostDeferredDisplayStation, (void*)0)); 
		break;
		
		case MODE_STATION_2:
			gs_Sation = 1;
			gs_Thread_Id = THREAD_STATION_2;
			if(gs_pInputStation[gs_Sation])
			EXCCHECK( ThreadStart( ThreadProcStation, gs_pInputStation[gs_Sation], &gs_hThread[THREAD_STATION_2], &gs_hEventStop[THREAD_STATION_2]));
			EXCCHECKCVI( PostDeferredCall( PostDeferredDisplayStation, (void*)1)); 
		break;
			
		case MODE_STATION_3:
			gs_Sation = 2;
			gs_Thread_Id = THREAD_STATION_3;
			if(gs_pInputStation[gs_Sation])
			EXCCHECK( ThreadStart( ThreadProcStation, gs_pInputStation[gs_Sation], &gs_hThread[THREAD_STATION_3], &gs_hEventStop[THREAD_STATION_3]));
			EXCCHECKCVI( PostDeferredCall( PostDeferredDisplayStation, (void*)2)); 
		break;
			
		case MODE_STATION_4:
			gs_Sation = 3;
			gs_Thread_Id = THREAD_STATION_4;
			if(gs_pInputStation[gs_Sation])
			EXCCHECK( ThreadStart( ThreadProcStation, gs_pInputStation[gs_Sation], &gs_hThread[THREAD_STATION_4], &gs_hEventStop[THREAD_STATION_4]));
			EXCCHECKCVI( PostDeferredCall( PostDeferredDisplayStation, (void*)3)); 
		break;
			
		case MODE_STATION_5:
			gs_Sation = 4;
			gs_Thread_Id = THREAD_STATION_5;
			if(gs_pInputStation[gs_Sation])
			EXCCHECK( ThreadStart( ThreadProcStation, gs_pInputStation[gs_Sation], &gs_hThread[THREAD_STATION_5], &gs_hEventStop[THREAD_STATION_5]));
			EXCCHECKCVI( PostDeferredCall( PostDeferredDisplayStation, (void*)4)); 
		break;
			
		case MODE_STATION_6:
			gs_Sation = 5;
			gs_Thread_Id = THREAD_STATION_6;
			if(gs_pInputStation[gs_Sation])
			EXCCHECK( ThreadStart( ThreadProcStation, gs_pInputStation[gs_Sation], &gs_hThread[THREAD_STATION_6], &gs_hEventStop[THREAD_STATION_6]));
			EXCCHECKCVI( PostDeferredCall( PostDeferredDisplayStation, (void*)5)); 
		break;
	
	}
		
	gs_mode = mode;

Error:      
   EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Tab_Change"
static SElExceptionPtr Tab_Change( int page)
{
	int32_t          error = 0;
	SElExceptionPtr  pexception = NULL;



	if(page == PAGE_ONE)
		EXCCHECK(ChangeMode(MODE_TESTER));

	else if (page == PAGE_TWO)
		EXCCHECK(ChangeMode(MODE_STATION_1));	

	else if(page == PAGE_THREE)
		EXCCHECK(ChangeMode(MODE_STATION_2));	

	else if(page == PAGE_FOUR)
		EXCCHECK(ChangeMode(MODE_STATION_3));
	
	else if(page == PAGE_FIVE)
		EXCCHECK(ChangeMode(MODE_STATION_4));
				
	else if(page == PAGE_SIX)
		EXCCHECK(ChangeMode(MODE_STATION_5));
			
	else if(page == PAGE_SEVEN)
		EXCCHECK(ChangeMode(MODE_STATION_6));
	

Error:      
   EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
int CVICALLBACK Cb_ChangeTab (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr	pexception = NULL;  
	int 	page;
	
	switch (event)
	{
		case EVENT_ACTIVE_TAB_CHANGE:
			
			GetActiveTabPage (panel, control, &page);
			EXCCHECK(Tab_Change(page));
			
		break;
	}

Error:
   	EXCDISPLAY( pexception);
   	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "EventFunctionCallback"
static SElExceptionPtr EventFunctionCallback( const char* eventName, void* pData)
{
	int32_t          error = 0;
	SElExceptionPtr  pexception = NULL;

	if(0==strcmp(eventName, "EVNT_GUI_TESTER"))
	{
		EXCCHECK(ChangeMode(MODE_TESTER));
	}

Error:      
   EXCRETHROW( pexception);
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "RegisterEventFunction"
static SElExceptionPtr RegisterEventFunction(void)
{
   int            error = 0;
   SElExceptionPtr   pexception = NULL;
   
   if ( gs_EventFactory )
   {
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
                                         "EVNT_GUI_TESTER",
                                         EventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
                                         "EVNT_GUI_TESTER_PANEL",
                                         EventFunctionCallback));
   }
   
Error:
   EXCRETHROW(pexception);
}   /* RegisterEventFunction */

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
	int            		monitor_id, top, height,width;  
	HRESULT				hr;
    int                 station;
	
	gs_PluginArgs = a_pPluginArgs;

   	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
                            "MaintenancePanel",
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
	
	EXCCHECK( LoadPanelExWithLanguage(main_panel, "tester_gui.uir", TESTER, TESTERlang, &gs_panel[TESTER])); 
	EXCCHECK( a_pPluginArgs->SetArg(a_pPluginArgs, "TesterPanel", ARG_INT32, &gs_panel[TESTER])); 

	EXCCHECKCVI(SetPanelAttribute (gs_panel[TESTER], ATTR_SCALE_CONTENTS_ON_RESIZE, TRUE));	
	
	GetPanelHandleFromTabPage (gs_panel[TESTER], TESTER_TAB, PAGE_ONE, &gs_panelTab[PAGE_ONE]);
	EXCCHECKCVI( GetObjHandleFromActiveXCtrl (gs_panelTab[PAGE_ONE], TABPANEL_BKTREE, &gs_hTreeTester) );
	EXCCHECKCOM( BKLib2__DBKTreeEventsRegOnNodeClick (gs_hTreeTester, BkTreeNodeClickTester, NULL, 1, NULL));

	//EXCCHECKCVI( GetObjHandleFromActiveXCtrl (gs_panel[TESTER], TESTER_IMAGELIST, &gs_hImageList) );
	//EXCCHECKCOM( BKLib2__DBKTreeSetImageList (gs_hTreeTester, NULL, gs_hImageList));
	//EXCCHECKCOM( BKLib2__DBKTreeSetAutoImages (gs_hTreeTester, NULL, VFALSE));
	EXCCHECKCOM( BKLib2__DBKTreeSetEnableSelection (gs_hTreeTester, NULL, VTRUE));

	for (station=0; station < MAX_STATIONS; station++)
	{		
		GetPanelHandleFromTabPage (gs_panel[TESTER], TESTER_TAB, station+1, &gs_panelTab[station+1]);
		EXCCHECKCVI(SetPanelAttribute (gs_panelTab[station+1], ATTR_SCALE_CONTENTS_ON_RESIZE, TRUE));	
		EXCCHECKCVI( GetObjHandleFromActiveXCtrl (gs_panelTab[station+1], gs_TabPanel[station], &gs_hTreeStation[station]));
		EXCCHECKCOM( BKLib2__DBKTreeEventsRegOnNodeClick (gs_hTreeStation[station], BkTreeNodeClickStation, (void*)station, station+1, NULL));
		//EXCCHECKCOM( BKLib2__DBKTreeSetImageList (gs_hTreeStation[station], NULL, gs_hImageList));
		//XCCHECKCOM( BKLib2__DBKTreeSetAutoImages (gs_hTreeStation[station], NULL, VFALSE));
		EXCCHECKCOM( BKLib2__DBKTreeSetEnableSelection (gs_hTreeStation[station], NULL, VTRUE));
	}

	EXCCHECKCVI( GetSystemAttribute( ATTR_PRIMARY_MONITOR, &monitor_id));
    EXCCHECKCVI( GetMonitorAttribute (monitor_id, ATTR_TOP, &top));
    EXCCHECKCVI( GetMonitorAttribute( monitor_id, ATTR_HEIGHT, &height));
	EXCCHECKCVI( GetMonitorAttribute( monitor_id, ATTR_WIDTH, &width)); 
   
	EXCCHECKCVI( SetPanelAttribute (gs_panel[TESTER], ATTR_TOP, 0));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[TESTER], ATTR_LEFT, 0));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[TESTER], ATTR_HEIGHT, height-78));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[TESTER], ATTR_WIDTH, width));

	
	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
	                        "EventFactory",
	                        sizeof(SElEventFactoryPtr),
	                        &gs_EventFactory));

	if(gs_Modules)
	{
		char uuid[65];

		gs_pTester = (STesterPtr)gs_Modules->Get(gs_Modules, MODULE_TESTER); 
		gs_pSensors = (SDIOSensorsPtr)gs_Modules->Get(gs_Modules, MODULE_SENSORS);
		gs_paxiss = (SaxissPtr)gs_Modules->Get(gs_Modules, MODULE_AXIS);  

		if(gs_pTester)
		{
			EXCCHECK(SetTree(gs_pTester->path, MODULE_TESTER, gs_hTreeTester, &gs_pInputTester, &gs_pOutputTester));
			for (station=0; station < MAX_STATIONS; station++)
				EXCCHECK(SetTreeStations(gs_pTester->path, MODULE_TESTER, gs_hTreeStation[station], &gs_pInputStation[station], &gs_pOutputStation[station], station));
		}
	}
	
	gs_mode = -1;

	EXCCHECK( RegisterEventFunction());  

Error:   
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{Tester Plugin}::Run"
PLUGIN_API SElExceptionPtr PLUGIN_RUN_NAME_CODE( SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	int32_t			page = 0;
	
	
	if( 0==strcmp( uuid, PLUGIN_TESTER))
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

