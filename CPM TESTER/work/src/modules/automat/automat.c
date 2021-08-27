#include <userint.h>
#include <labels/tester_labels.h>
#include <test/test.h>
#include <test/eltest.h>
#include <multithread/multithread.h>
#include <language/language.h>
#include <utility.h>
#include <stdio.h>
#include <dbs/dbs.h>
#include <box/box.h>
#include <tester/tester.h>
#include <printer/class/printers.h>
#include <traca/class/tracas.h>
#include "automat.h"
#include "automat_fnc.h"
#include "automat_plugin.h" 
//#include "hpe364xa.h"
#include "bp98xx.h"
#include "asynctmr.h"
#include <rs232.h>
#include <formatio.h>
#include "toolbox.h"
#include <eldaq/class/eldaq.h>


#define READ_ANY					0xFFFFFFFF	
#define _START_AUTOMAT_INIT_ALL	  	0 /* 0 no init */
#define NB_OF_PANEL					1  
#define NUMBER_OF_STATIONS			6
#define OK							0 
#define NOK							1 
#define RUN							2 
#define FREE						3 
#define UNLOCK_DELAY				500
#define TIMEOUT						5000
#define COLDMARKING_TIMEOUT			6000
#define ROTATION_TIMEOUT			7000
#define TIME_OUT_PRINTER		    60000 //1min
#define AXIS_X  					0
#define COLOR_GREEN  				0
#define COLOR_RED  					1
#define COLOR_ORANGE  				2
#define COLOR_OFF	  				3	
#define COLOR_GREEN_ORANGE 			4
#define COLOR_ORANGE_OFF			5
#define	COM_BARCODE_PALETT			1

#define MAX_FIELDS					20
#define MAX_LENGTH					128	

enum {
	EVNT_STOP_THREAD = 0,
	EVNT_GUI_PRODUCTION,
	EVNT_GUI_QUALITY_RELEASE,
	EVNT_GUI_QUALITY_RETEST,
	EVNT_GUI_QUALITY_MASTER,
	EVNT_GUI_EXPERTISE_STEP_MODE,
	EVNT_GUI_EXPERTISE_GO_MODE,
	EVNT_GUI_EXPERTISE_SEQUENCE_CREATED,
	EVNT_GUI_EXPERTISE_RNR,
	EVNT_GUI_PRODUCT_SELECTED,
	EVNT_TEST_SEQUENCE_RUN,
	EVNT_EXPERTISE_STEP_MODE_NEXT,
	EVNT_EXPERTISE_STEP_MODE_REPEAT,
	EVNT_EXPERTISE_STEP_MODE_BACK,
	EVNT_EXPERTISE_STEP_MODE_STOP,
	EVNT_GUI_KEY_ESC_PRESSED,
	EVNT_RESET_PRODUCT,
	EVNT_JIG_ITEM_INSERTED_,
	EVNT_JIG_ITEM_RELEASED_,
	EVNT_JIG_CYCLE_START_,
	EVNT_JIG_NEW_,
	EVNT_JIG_START_,
	/* event count */
	EVENT_HANDLE_NB
};

int						gs_panel[NB_OF_PANEL + 1];
static HANDLE			gs_EventHandle[EVENT_HANDLE_NB];
SElEventFactoryPtr		gs_EventFactory = NULL;  
static SObjListPtr 		gs_Modules = NULL; 
static SLabelListPtr	gs_pLabels = NULL;
static ViSession		gs_DAQ = 0;
static SDBSPtr			gs_pDbs = NULL;
static SBoxPtr			gs_pBox = NULL;
static STesterPtr		gs_pTester = NULL;
static SPrintersPtr		gs_pPrinters = NULL;
static STracasPtr		gs_pTracas = NULL;
static SaxissPtr		gs_paxiss = NULL;
static SCamerasPtr		gs_pcameras = NULL;
static STestParamPtr	gs_pTID; 
static SAutomat			gs_Automat;
static HANDLE			gs_hThread = NULL;
static HANDLE			hEventStop = NULL;
static HANDLE			gs_hThread2 = NULL;
static HANDLE			hEventStop2 = NULL;
bool_t					b_loop_stop2 = FALSE;

SLanguagePtr			gs_pLanguage = NULL; 
int 					gs_Table_Rotation;
int32_t					Reset = FALSE;
static HANDLE 			gs_hEventStop = NULL;


int 					gs_PalettID;
int 					gs_Product_Checked;
int						g_timerId;
int 					Autorisation_Thread;
		
int32_t					gs_RnR_Station;
int32_t					gs_RnR_Repeat;
char 					gs_RnR_Palett[10+1]; 
char 					gs_RnR_Piece[10+1];

int32_t                 TimeStation[8];

#define MAX_MASTER_PARTS	50

int 					number_of_master_parts;
int 					Actual_master_part = 0;
int 					Finished_master_parts = 0;
int						NOK_Master_Parts = 0;
char					gs_barcode_Master[128];
char					MastersBarcodes[MAX_MASTER_PARTS][512];
char					MastersDescription[MAX_MASTER_PARTS][512];
char					MastersName[MAX_MASTER_PARTS][512];
int 					MastersStations[MAX_MASTER_PARTS];
int						Status [MAX_MASTER_PARTS];


#define TIMEOUT_INIT() 	clock_t	_start_clock = clock()

#define TIMEOUT_CHECK_CONVOYER(timeout) \
	if ( ((clock() - _start_clock_convoyer) / (double)CLOCKS_PER_SEC) > ((double)timeout/1000.0) ) { EXCTHROW( TEST_ERR_TIMEOUT, "Timeout Convoyer"); } else

#define TIMEOUT_CHECK(timeout) \
	if ( ((clock() - _start_clock) / (double)CLOCKS_PER_SEC) > ((double)timeout/1000.0) ) { EXCTHROW( TEST_ERR_TIMEOUT, "Timeout"); } else

#define TIMEOUT_PRINTER_CHECK(timeout) \
	if ( ((clock() - _start_clock) / (double)CLOCKS_PER_SEC) > ((double)timeout/1000.0) ) { EXCTHROW( TEST_ERR_TIMEOUT, "Timeout Impriomante!"); } else
	
#define PANEL_MSG(control, msg) \
	do{ \
		SetCtrlVal(gs_panel[AUTOMAT], control, msg); \
		SetCtrlAttribute(gs_panel[AUTOMAT], control, ATTR_FRAME_COLOR, VAL_BLACK); \
	}while(0)

#define PANEL_RESULT(result) \
	do{ \
		EXCCHECK( gs_Automat.pTestSeq->SetPanelDescription(gs_Automat.pTestSeq, PANEL_RESULT_MSG, result)); \
	}while(0)

#define LED_OPERATOR(color) \
	do{ \
		LED_Operator(color); \
	}while(0)

#define PANEL_TABLE_WARNING(control , status) \
	do{ \
		if(gs_Automat.pTestSeq) \
			EXCCHECK( gs_Automat.pTestSeq->SetPanelWarning(gs_Automat.pTestSeq, control, status)); \
	}while(0)

#define PANEL_STATION_STATUS(station , status) \
	do{ \
		if(gs_Automat.pTestSeq) \
			EXCCHECK( gs_Automat.pTestSeq->SetPanelStationStatus(gs_Automat.pTestSeq, station, status)); \
	}while(0)

#define PANEL_STATION_FAULT(station, station_origin, test_description, status) \
	do{ \
		if(gs_Automat.pTestSeq) \
			EXCCHECK( gs_Automat.pTestSeq->SetPanelFault(gs_Automat.pTestSeq, station, station_origin, test_description, status)); \
	}while(0)

#define PANEL_STATION_TIME(station , time) \
	do{ \
		if(gs_Automat.pTestSeq) \
			gs_Automat.pTestSeq->SetPanelTime(gs_Automat.pTestSeq, station, time); \
	}while(0)

#define PANEL_SET_REFERENCES(Refvaleo, Refcustomer) \
	do{ \
		if(gs_Automat.pTestSeq) \
			EXCCHECK( gs_Automat.pTestSeq->SetTestPanelReferences(gs_Automat.pTestSeq, Refvaleo, Refcustomer)); \
	}while(0)

#define PANEL_GET_PALETT_STATUS(PalettID, status) \
	do{ \
		if(gs_Automat.pTestSeq) \
			EXCCHECK( gs_Automat.pTestSeq->GetPalettStatus(gs_Automat.pTestSeq, PalettID, status)); \
	}while(0)

#define PANEL_MODE(mode) \
	do{ \
		if(gs_Automat.pTestSeq) \
			EXCCHECK( gs_Automat.pTestSeq->SetPanelMode(gs_Automat.pTestSeq, E_TSPM_##mode)); \
	}while(0)

#define CHECK_ERR( is_Error) \
	if(is_Error!=0) goto Error; \
	else
 
#define CHECK_ERR2( is_Error) \
	if(is_Error2!=0) goto Error2; \
	else

#define CHECK_ERR3( is_Error) \
	if(is_Error!=0) goto Error3; \
	else

#define CHECK_ERR4( is_Error) \
	if(is_Error!=0) goto Error4; \
	else

#define CHECK_ERR5( is_Error) \
	if(is_Error!=0) goto Error5; \
	else

#define INSERT_EMPTY_BOX() \
	EXCCHECK( gs_pBox->SetPanelsInBox(gs_pBox, gs_pBox->_PanelsInBox))

static SElExceptionPtr JigPanelLock(int32_t* pError);
static SElExceptionPtr JigPanelUnlock(int32_t* pError);
static SElExceptionPtr JigPanelInsert(int32_t* pError);
static SElExceptionPtr JigPanelInsert2(int32_t* pError);
static SElExceptionPtr JigPanelRelease(struct _SSledge* me, bool_t fault);
static SElExceptionPtr JigPanelRelease2(struct _SSledge* me, bool_t fault);
static SElExceptionPtr ProcessEventMasterSequenceRun(void);
static SElExceptionPtr ProcessEventGuiProduction(void);
static SElExceptionPtr PanelBridage(int timeout);
static SElExceptionPtr PanelDebridage(int timeout);
static SElExceptionPtr PanelPresence(int timeout);
static SElExceptionPtr PanelWaitForRemove(void);
static SElExceptionPtr PanelChecked(int timeout);
static void CVICALLBACK PostDeferredDisplayPanel (void *callbackData){ DisplayPanel(gs_panel[AUTOMAT]); }
static void CVICALLBACK PostDeferredHidePanel (void *callbackData){ HidePanel(gs_panel[AUTOMAT]); }
static DWORD WINAPI Thread_Sledge(LPVOID param);
static SElExceptionPtr Activate(struct _SSledge* me);
static SElExceptionPtr Deactivate(struct _SSledge* me);


CRITICAL_SECTION   _Lock;
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "fncLock"
static SElExceptionPtr fncLock(void)
{
   EnterCriticalSection( &_Lock);
   return NULL;
} 

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "fncUnlock"
static SElExceptionPtr fncUnlock(void)
{
   LeaveCriticalSection( &_Lock);
   return NULL;
} 
/*---------------------------------------------------------------------------*/
int GetPalettNumber (void)
{
	
	int PalletID;
	
	if (gs_PalettID == 6)
		PalletID = 1;
	else 
		PalletID = gs_PalettID + 1;
	
	return PalletID;
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LED_OPERATOR"
SElExceptionPtr LED_Operator(int color)
{
SElExceptionPtr pexception = NULL;

switch (color)
	{
		case COLOR_GREEN :
			gs_pTester->SetSensor(gs_pTester, "VERRINE_GREEN", TRUE);
			gs_pTester->SetSensor(gs_pTester, "VERRINE_ORANGE", FALSE);
			gs_pTester->SetSensor(gs_pTester, "VERRINE_RED", FALSE);
		break;
		
		
		case COLOR_GREEN_ORANGE :
			gs_pTester->SetSensor(gs_pTester, "VERRINE_GREEN", TRUE);
			gs_pTester->SetSensor(gs_pTester, "VERRINE_ORANGE", TRUE);
			gs_pTester->SetSensor(gs_pTester, "VERRINE_RED", FALSE);
		break;
		
		case COLOR_ORANGE_OFF :
			gs_pTester->SetSensor(gs_pTester, "VERRINE_GREEN", TRUE);
			gs_pTester->SetSensor(gs_pTester, "VERRINE_ORANGE", FALSE);
			gs_pTester->SetSensor(gs_pTester, "VERRINE_RED", FALSE);
		break;

		case COLOR_RED :
			gs_pTester->SetSensor(gs_pTester, "VERRINE_GREEN", FALSE);
			gs_pTester->SetSensor(gs_pTester, "VERRINE_RED", TRUE);
			gs_pTester->SetSensor(gs_pTester, "VERRINE_ORANGE", FALSE);
		break;

		case COLOR_ORANGE :
			gs_pTester->SetSensor(gs_pTester, "VERRINE_ORANGE", TRUE);
			gs_pTester->SetSensor(gs_pTester, "VERRINE_GREEN", FALSE);
			gs_pTester->SetSensor(gs_pTester, "VERRINE_RED", FALSE);
		break;

		case COLOR_OFF :
			gs_pTester->SetSensor(gs_pTester, "VERRINE_GREEN", FALSE);
			gs_pTester->SetSensor(gs_pTester, "VERRINE_RED", FALSE);
			gs_pTester->SetSensor(gs_pTester, "VERRINE_ORANGE", FALSE);
		break;
}	
Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/			
#undef __FUNC__
#define __FUNC__ "CheckAxisPosition"
SElExceptionPtr CheckAxisPosition( SaxisPtr paxis, int axis, double position, double timeout)
{
	SElExceptionPtr pexception = NULL;
	double Currentposition;
	int   status;

	
    TIMEOUT_INIT();
	do{
	paxis->CurrentPosition(paxis, axis, &Currentposition);
	if (Currentposition < position + 0.1 && Currentposition > position - 0.1)
			break;
        TIMEOUT_CHECK(timeout);
    }
    while(TRUE);
	
	
Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::GetPalettID"
static SElExceptionPtr GetPalettID(int *palettID)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	int received, stringsize;
	char send_data[3+1];
	char buffer [128];
	int  pallet = 0;
	int  cnt;
	
	*palettID = 0;
	
	OpenComConfig (COM_BARCODE_PALETT, "", 115200, 0, 8, 1, 512, 512);
	
	send_data[0] = 22;
	send_data[1] ='T';
	send_data[2] = 13;
	send_data[3] = 0X00;
	
	cnt = 0;
	do {
		memset (buffer, 0x00, 128);
		stringsize = StringLength (send_data);
    	ComWrt (COM_BARCODE_PALETT, send_data, stringsize);
		ReturnRS232Err ();
	
		Sleep(1500);
	
		received  = GetInQLen (COM_BARCODE_PALETT);
		if (received == 2)
		{
			//printf ("received = %d \r\n", received);
			ComRd (COM_BARCODE_PALETT, buffer, received);
			sscanf (buffer, "P%d", &pallet);
			*palettID = pallet;
			//printf ("*palettID = %d \r\n", *palettID);
		}

	if (pallet == 0)
		 cnt++;
			 
	}while ( cnt < 5 && pallet == 0);

Error:
	CloseCom (COM_BARCODE_PALETT);
	EXCRETHROW( pexception);  
}	

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::ExtractDoubleBarcode"
static SElExceptionPtr ExtractDoubleBarcode(char *Buffer, char *Barcode1, char *Barcode2)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
    int i, Index1, Index2;
	
	Index1 = FindPattern (Buffer, 0, -1, "\r", 0, 0);
	
	for(i=0; i<Index1;i++)
	{
		if(Buffer[i] >= 48)
		{
			CopyString (Barcode1, 0, Buffer, i, Index1-i);		
			break;
		}
	}
	
	Index2 = FindPattern (Buffer, Index1+1, -1, "\r", 0, 0);
	
	for(i=Index1+1; i<Index2;i++)
	{
		if(Buffer[i] >= 48)
		{
			CopyString (Barcode2, 0, Buffer, i, Index2-i);		
			break;
		}
	}
		
Error:
	EXCRETHROW( pexception);  
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::DisplayWait"
SElExceptionPtr DisplayWait (void)
{
SElExceptionPtr	pexception = NULL;
int32_t			error = 0;
static int step = 0;

switch (step)
	{
	case 0 :	
		PANEL_MSG( AUTOMAT_HELP2, _("Attente Fin des Tests de toutes Les Stations"));
		LED_OPERATOR(COLOR_GREEN_ORANGE);
		step ++;
		Sleep (200);
	break;
	
	case 1 :
		PANEL_MSG( AUTOMAT_HELP2, _(""));
		LED_OPERATOR(COLOR_ORANGE_OFF);
		step ++;																									       
		Sleep (300);
	break;
	
	case 2 :	
		step = 0;
		LED_OPERATOR(COLOR_ORANGE_OFF);
	break;
	}

Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__  "Timer_Wait"
int CVICALLBACK Timer_Wait (int reserved, int theTimerId, int event,
                                 void *callbackData, int eventData1,
                                 int eventData2)
{                                                   
	SElExceptionPtr	pexception = NULL;

	DisplayWait();
	
Error:
	if (pexception)
	{
		EXCDISPLAY(pexception);
		DiscardAsyncTimer (g_timerId);
	}
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__  "Wait_To_Empty_Convoyer"
int CVICALLBACK Wait_To_Empty_Convoyer ()

{
	int32_t ProductOutConvoyer;
	while (TRUE)
	{
		gs_pTester->GetSensor(gs_pTester, "SENSOR_CONV_OUT", &ProductOutConvoyer);
		if(!ProductOutConvoyer)
			break;
		Sleep(100);
	}
	return 0;
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::Init_PWM_Generator"
SElExceptionPtr Init_PWM_Generator (void)
{
	SElExceptionPtr	pexception = NULL;
	char sys_error[128];
	
    TEST_USE(GENERATOR);
    INITIALIZE_GENERATOR (0);
	pgenerator->Connect(pgenerator);
	pgenerator->SystemError(pgenerator, sys_error);	
	pgenerator->ClearError(pgenerator);
	pgenerator->SetFonction(pgenerator,50.0,2.5,1.25,50.0);	
	pgenerator->SetOutput(pgenerator,1);
	
Error:
	EXCRETHROW( pexception);
}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::Init_BK_PRECISION_WS40_WS50"
SElExceptionPtr Init_BK_PRECISION_WS40_WS50 (void)
{
	ViSession vi_BK;
	SElExceptionPtr	pexception = NULL;
	char sys_error[128];
	char msg[250];
	int ErrorBK;
    ViChar ErrorMessage[250];
	ViRsrc BKresourceName    = "ASRL4::INSTR";
	ViInt32 Readsize;
	
	 ErrorBK = bp98xx_Initialize (BKresourceName, VI_TRUE, VI_TRUE, BAUDRATE_VAL6_19200, &vi_BK);
		 if (ErrorBK < VI_SUCCESS)
		 {
			
			 
			 ViChar ErrorMessage[250];
			 bp98xx_GetLastErrorMsg (vi_BK, 255, ErrorMessage, &Readsize);
			 sprintf(msg, "Function Error : bp98xx_Initialize \r\n  message error : %s\n", ErrorMessage);
			 EXCTHROW( -1, msg);
		 }
		 ErrorBK =bp98xx_Reset (vi_BK);
		 Sleep(50);
		 ErrorBK = bp98xx_ConfigureCurrentProtectionLimits (vi_BK, 1.0, 1.3, CURRENTPROTECTIONDELAY_VAL0_DELAY, PEAKCURRENTPROTECTIONDELAY_VAL0_DELAY);
		 if (ErrorBK < VI_SUCCESS)
		 {
			 bp98xx_GetLastErrorMsg (vi_BK, 255, ErrorMessage, &Readsize);
			 sprintf(msg, "Function Error : bp98xx_ConfigureCurrentProtectionLimits \r\n  message error : %s\n", ErrorMessage);
			 EXCTHROW( -1, msg);
		 }
		 ErrorBK = bp98xx_ConfigureOutput (vi_BK, 230.0, 50.0);
		 if (ErrorBK < VI_SUCCESS)
		 {
			 bp98xx_GetLastErrorMsg (vi_BK, 255, ErrorMessage, &Readsize);
			 sprintf(msg, "Function Error : bp98xx_ConfigureOutput \r\n  message error : %s\n", ErrorMessage);
			 EXCTHROW( -1, msg);
		 }
		 Sleep(50);
		 ErrorBK = bp98xx_EnableOutput (vi_BK, VI_TRUE);
		 if (ErrorBK < VI_SUCCESS)
		 {

			 bp98xx_GetLastErrorMsg (vi_BK, 255, ErrorMessage, &Readsize);

			 sprintf(msg, "Function Error : bp98xx_EnableOutput \r\n  message error : %s\n", ErrorMessage);
			 EXCTHROW( -1, msg);
		 }
		 Sleep(100);
	
   
	
Error:
	EXCRETHROW( pexception);
}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::Init_Alim_keysight"
SElExceptionPtr Init_Alim_keysight (void)
{
ViSession vi_E3642A;
ViRsrc    E3642AresourceName   = "ASRL17::INSTR";
SElExceptionPtr	pexception = NULL;
ViInt32   ErrorE3642A,
		  ErrorCode;;
ViChar    ErrorMessage[256];
char      msg[512];

  // Initiate the keysight and create a session ---------------------------//
		ErrorE3642A = hpe364xa_init(E3642AresourceName,VI_FALSE,VI_TRUE,&vi_E3642A);
		if (ErrorE3642A < VI_SUCCESS)
		{
			
			hpe364xa_GetError (vi_E3642A, &ErrorCode, 255, ErrorMessage);
			sprintf(msg, "Function Error :E3642A_InitWithOptions \r\n Error: %d, message error : %s\n", ErrorCode, ErrorMessage);
			EXCTHROW( -1, msg);
		}

		hpe364xa_ClearErrorInfo(vi_E3642A);
		ErrorE3642A = hpe364xa_ConfigureOutputEnabled (vi_E3642A, "1", VI_FALSE);
		if (ErrorE3642A < VI_SUCCESS)
		{
			hpe364xa_GetError (vi_E3642A, &ErrorCode, 255, ErrorMessage);
			sprintf(msg, "Function Error : AgE3642A_ConfigureOutputEnabled OFF \r\n Error: %d, message error : %s\n", ErrorCode, ErrorMessage);
			EXCTHROW( -1, msg);
		}
		//Sleep(200);
		ErrorE3642A = hpe364xa_ConfigureOutputRange   (vi_E3642A, "1", HPE364XA_VAL_RANGE_VOLTAGE, 20.0);
		if (ErrorE3642A < VI_SUCCESS)
		{
			
			hpe364xa_GetError (vi_E3642A, &ErrorCode, 255, ErrorMessage);
			sprintf(msg, "Function Error : AgE3642A_ConfigureOutputRange \r\n Error: %d, message error : %s\n", ErrorCode, ErrorMessage);
			EXCTHROW( -1, msg);
		}
	//	Sleep(200);
		ErrorE3642A = hpe364xa_ConfigureCurrentLimit  (vi_E3642A, "1", HPE364XA_VAL_CURRENT_REGULATE, 0.8);
		if (ErrorE3642A < VI_SUCCESS)
		{
			
			hpe364xa_GetError (vi_E3642A, &ErrorCode, 255, ErrorMessage);
			sprintf(msg, "Function Error : AgE3642A_ConfigureCurrentLimit \r\n Error: %d, message error : %s\n", ErrorCode, ErrorMessage);
			EXCTHROW( -1, msg);
		}
 
		ErrorE3642A = hpe364xa_ConfigureVoltageLevel (vi_E3642A, "1", 12.04);
		
		if (ErrorE3642A < VI_SUCCESS)
		{
			hpe364xa_GetError (vi_E3642A, &ErrorCode, 255, ErrorMessage);
			sprintf(msg, "Function Error : AgE3642A_ConfigureVoltageLevel \r\n Error: %d, message error : %s\n", ErrorCode, ErrorMessage);
			EXCTHROW( -1, msg);
		}
      // Sleep(200);
        ErrorE3642A = hpe364xa_ConfigureOutputEnabled (vi_E3642A, "1", VI_TRUE); 
		if (ErrorE3642A < VI_SUCCESS)
		{
			hpe364xa_GetError (vi_E3642A, &ErrorCode, 255, ErrorMessage);
			sprintf(msg, "Function Error : AgE3642A_ConfigureOutputEnabled \r\n Error: %d, message error : %s\n", ErrorCode, ErrorMessage);
			EXCTHROW( -1, msg);
		}	
Error:
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::DisplayClignotant"
SElExceptionPtr DisplayClignotant (char* message)
{
SElExceptionPtr	pexception = NULL;
int32_t			error = 0;
static int step;
static int fois;

switch (step)
	{
	case 0 :	
		PANEL_MSG( AUTOMAT_HELP2, _(message));
		Sleep (10);
		fois ++;
	    if (fois == 10)
		{
			fois = 0;
			step ++;
		}
	break;
	
	case 1 :
		PANEL_MSG( AUTOMAT_HELP2, _(""));
		Sleep (10);
		fois ++;
	    if (fois == 10)
			step = 0;
	break;
	}

Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::ResetStations"
static SElExceptionPtr ResetStations(void)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	int32_t			is_Error = 0;
	SaxisPtr		paxis;
	int             i;
	int32_t			insert;
	int 			status;
	int 			status_micro_cam, barrier, ProductOutConvoyer, ProductInConvoyer;
	double 			Currentposition;
	int32_t			position_0;
	int32_t 		alarm;
	clock_t	_start_clock_convoyer;
						   
	if (Reset == FALSE)
	{
	Reset = TRUE;
	
	/* Disconnect relais WS20 VAC*/
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_2B",0));
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_2C",0));
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_2D",0));
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_2E",0));
	/* Disconnect relais WS30 VAC*/
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_FTA1",                0)); 
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_FTA2",                0)); 
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_FTA3",                0));
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_FTA4",                0));
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_DBUS_Relay_3F", 			 0));
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_3G", 				 0));
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_3H", 				 0));
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_3I", 				 0));
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_3L", 				 0));
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_3M", 				 0));
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_3N", 				 0));
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_3O", 				 0));
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_3P", 				 0));
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_3Q_3Q2",			     0));
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_AC_PowerSupply_Relay_3R",   0));
	/* Disconnect relais WS40 VAC*/
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_4A",                0)); 
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_4B",                0));  
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_4S",                0)); 
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_4T",                0)); 
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_4U",                0)); 
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_4V_4V2",            0));  
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_4Z",                0)); 
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_4W",                0)); 
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_DBUS_Relay_4R",           0)); 
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_PowerSupply_Relay_4H",    0)); 
	/* Disconnect relais WS50 VAC*/
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_5C",                0)); 
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_5D",                0)); 
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_5E",                0)); 
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_5F",                0)); 
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_5I",                0)); 
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_5L",                0));
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_5M",                0));
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_Relay_5N",                0));
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_PowerSupply_Relay_5G",    0));
	EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_DBUS_Relay_5H",           0));
	/* unplug all stations */
	EXCCHECK( gs_pTester->VerticalUnplugStation30(gs_pTester, 5000));
	EXCCHECK( gs_pTester->UnPlugPanelStation20(gs_pTester,    5000));
	EXCCHECK( gs_pTester->UnplugPanelStation30(gs_pTester,    5000));
	EXCCHECK( gs_pTester->UnplugPanelStation40(gs_pTester,    5000));
    EXCCHECK( gs_pTester->UnplugPanelStation50(gs_pTester,    5000));
 
	EXCCHECK( gs_pTester->GetSensor(gs_pTester, "MICRO_CAM", &status_micro_cam));

	/* move one step if a part exist in convoyer */
	EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_CONV_OUT",&ProductOutConvoyer ))
	if (!ProductOutConvoyer)
	{
		EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_CONV_IN",&ProductInConvoyer ))
		if (ProductInConvoyer)
			{
				_start_clock_convoyer = clock();
				do{
					EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_CONV_IN",&ProductInConvoyer ))
					EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_CONVEYOR", TRUE));
					if(ProductInConvoyer==0)
						{
							EXCCHECK( gs_pTester->SetSensor(gs_pTester, "CMD_CONVEYOR", FALSE));
							break;
						}
					/* ajouter un timeout */
					TIMEOUT_CHECK_CONVOYER(10000);
				}while (TRUE);
			}
	}
	EXCCHECK( gs_pTester->GetSensor(gs_pTester, "MICRO_CAM", &status_micro_cam));		
	if (status_micro_cam == 1)
	{
		/* Home station 60*/
		EXCCHECK( gs_pTester->HomePickPlacePanelStation60(gs_pTester, 10000));
	}
		
	ProcessDrawEvents ();
	
	Reset = FALSE;
	}

Error:
	gs_pTester->SetSensor(gs_pTester, "CMD_CONVEYOR", FALSE);
	Reset = FALSE;
	EXCRETHROW( pexception);  
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::ProcessEventGuiResetProduct"
static SElExceptionPtr ProcessEventGuiResetProduct(void)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	char			test_form_xml[MAX_PATHNAME_LEN]; 
	int32_t			is_Error = 0;
	int             i;
	int32_t			insert,
					insert1,
					insert2,
					insert3;
	int 	status;
	int status_micro_cam, barrier;
	double Currentposition;
	int32_t			position_0;  
	
	
	//EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_PRESENCE_PRODUCT_STATION_10", &insert));
	EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_1_PRESENCE_PRODUCT_STATION_10", &insert1));
	EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_2_PRESENCE_PRODUCT_STATION_10", &insert2));
	EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_3_PRESENCE_PRODUCT_STATION_10", &insert3));
    insert=insert1||insert2 ||insert3;
	EXCCHECK( ResetStations());

	if(gs_Automat.shift_id==0)
		{
		PANEL_MSG( AUTOMAT_HELP2, _("Selectionner Une Equipe"));  
		PANEL_MSG( AUTOMAT_HELP1, "");
		}
	
	else if(gs_Automat.masterpart)
		{
		PANEL_MSG( AUTOMAT_HELP2, _("Insérer une piéce type"));
		}
	
	else 
	{

			if (gs_Automat.EmtyTable == TRUE) //mode vidage 
				{
				if (gs_Automat.ChangeVersion == FALSE) //End Version
						PANEL_MSG( AUTOMAT_HELP1, _("MODE VIDAGE PLATEAU"));

				else if (gs_Automat.ChangeVersion == TRUE) //Change Version
					PANEL_MSG( AUTOMAT_HELP1, _("Changement de Référence (Vidage Plateau)")); 

				if (insert1 ||insert2 ||insert3 )
					PANEL_MSG( AUTOMAT_HELP2, _("Enlever Le produit"));
				
				else if (!insert1 && !insert2 && !insert3 )
					PANEL_MSG( AUTOMAT_HELP2, _("Appuer Sur Start Cycle"));
				}
		
			else 	  //mode production normal
				{
					
				PANEL_MSG( AUTOMAT_HELP1, _("MODE PRODUCTION"));
		
				if (!insert1 && !insert2 && !insert3)
					PANEL_MSG( AUTOMAT_HELP2, _("Insérer Un Produit"));
				else if (insert1 && insert2 && insert3 )
					PANEL_MSG( AUTOMAT_HELP2, _("Appuyer Sur Start Cycle"));
				}
	}

Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::ProcessEventGuiProductSelected"
static SElExceptionPtr ProcessEventGuiProductSelected(bool_t set_panel, bool_t set_box, bool_t set_master)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	char			test_form_xml[MAX_PATHNAME_LEN]; 
	int32_t			is_Error = 0;
	SElEventDataPtr pdata = NULL;
	int32_t			full;
	int32_t			closed;
	int32_t			finished;
	int32_t			erreur;
	char     		data[256];
	
	
	if(gs_EventFactory && set_panel)
	{
		/* set panel to default */
		EXCCHECK( eleventdata_new(&pdata)); 
		EXCCHECK( pdata->SetStringPtr(pdata, "panel;2"));
		EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_GUI_PANEL", pdata));
	}

	if(gs_Automat.master_active)
		{

			PANEL_MSG( AUTOMAT_HELP1, "Mode Passage Piéces Types");
			ProcessDrawEvents ();
			ProcessSystemEvents ();
			
			/* Load first master barcode */
			EXCCHECK( ProcessEventMasterSequenceRun ());
		}

	else 
	{
		/* set mode */

		gs_Automat.Mode = AUTOMAT_PRODUCTION;
		gs_Automat.TestSeqMode = TESTSEQ_PRODUCTION;

		EXCCHECK(  ProcessEventGuiProduction ());
	
		EXCCHECKCVI( GetProjectDir(test_form_xml));
		strcat(test_form_xml, PATH_TEST_FILE_FORM_XML);
	
		EXCCHECK( LoadTestSequence(test_form_xml, gs_Automat.pTestSeq, gs_Modules, TRUE, &is_Error));
		CHECK_ERR( is_Error);
		//PANEL_SET_REFERENCES(gs_Automat.pTestSeq->ProductPartNumber, gs_Automat.pTestSeq->PSA_FPN);
	}

	gs_Automat.ChangeVersion = FALSE;
	PANEL_MODE( DEFAULT_MODE);

Error:
	eleventdata_delete(&pdata);
	EXCRETHROW( pexception);  
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ReadMasterInformations"
SElExceptionPtr ReadMasterInformations(const char* pfile_xml, char *description, char *name, char *station)
{
	int32_t				error = 0;
	SElExceptionPtr    	pexception = NULL; 
	HRESULT         	hr = 0;
	CAObjHandle     	pxml = 0; 
	CAObjHandle     	pnode = 0;
	VARIANT         	pfile_name_var = CA_VariantEmpty();
	VBOOL            	is_xml_success = 1;
	char*           	ptext = NULL;
	
	/* configure test sequence from XML */
	EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml));
	EXCCHECKCOM( CA_VariantSetCString(&pfile_name_var, pfile_xml));
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (pxml, NULL, pfile_name_var, &is_xml_success));
	
	/* description */
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode( pxml, NULL, "/testseq/product/description", &pnode));
	if(pnode)
	{
	EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pnode, NULL, &ptext));
		if(ptext && *ptext)
			{
			sprintf (description, "%s", ptext);
			CA_FreeMemory(ptext);
			ptext=NULL;
			}
		EXCCHECKCVI( CA_DiscardObjHandle(pnode));
		pnode = 0;
	}
	/* name */
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode( pxml, NULL, "/testseq/product/name", &pnode));
	if(pnode)
	{
	EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pnode, NULL, &ptext));
		if(ptext && *ptext)
			{
			sprintf (name, "%s", ptext);
			CA_FreeMemory(ptext);
			ptext=NULL;
			}
		EXCCHECKCVI( CA_DiscardObjHandle(pnode));
		pnode = 0;
	}
	
	/* station */
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode( pxml, NULL, "/testseq/product/station", &pnode));
	if(pnode)
	{
	EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pnode, NULL, &ptext));
		if(ptext && *ptext)
			{
			sprintf (station, "%s", ptext);
			CA_FreeMemory(ptext);
			ptext=NULL;
			}
		EXCCHECKCVI( CA_DiscardObjHandle(pnode));
		pnode = 0;
	}

Error:
	CA_VariantClear(&pfile_name_var);
	if (ptext) CA_FreeMemory(ptext);
	if (pnode) CA_DiscardObjHandle(pnode);
	if (pxml) CA_DiscardObjHandle(pxml);
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::GetMasters"
static SElExceptionPtr GetMasters(void)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	char testmaster_form_xml[MAX_PATHNAME_LEN];
	int32_t			master_id;
	int32_t			is_Error = 0;
	SElEventDataPtr pdata = NULL;
	char			Msg[128]; 
	char**			Barcodes = NULL;
	char			description[512]; 
	char			name[512]; 
	char			station[128]; 
	char 			pfile_xml[512];
	int 			i;
	int             iStation;
	
	if(gs_pDbs) 
		{
		/* get the number of master parts in this verion */
		Barcodes = (char**)calloc(512, sizeof(char*));
		
		EXCCHECK( gs_pDbs->ProductMasterList->ProductMasterGetBarCodes(gs_pDbs->ProductMasterList, gs_pDbs->TesterProductId,&number_of_master_parts, Barcodes));
	
		if (number_of_master_parts > 0)
			{
			for (i=0; i<number_of_master_parts; i++)
				{
				/*Read Masters description*/
				sprintf (pfile_xml, ".//Masters//%s.XML", Barcodes[i]);
				memset (name, 0x00, 512);
				memset (description, 0x00, 512);
				EXCCHECK(ReadMasterInformations(pfile_xml, description, name, station));
				sprintf (MastersBarcodes[i], "%s", Barcodes[i]);
				sprintf (MastersDescription[i], "%s", description);
				MastersStations[i] = atoi(station);
				}
			}
			gs_Automat.TestSeqMode = TESTSEQ_MASTERPART;
		}
				
Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::RunMasterSequence"
static SElExceptionPtr RunMasterSequence(bool_t Load_Params)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	char 			test_form_xml[MAX_PATHNAME_LEN];
	int32_t			master_id;
	int32_t			is_Error = 0;
	SElEventDataPtr pdata = NULL;
	char			Msg[128]; 
	
		if (number_of_master_parts > 0)
			{
			/* init Finished Master parts for new tour */
			Finished_master_parts = 0;	
			
			/* Start with first index of master part */
			Actual_master_part = 0;

			if (Load_Params)
			{
			EXCCHECKCVI( GetProjectDir(test_form_xml));
			strcat(test_form_xml, PATH_TEST_FILE_FORM_XML); 

			/* load testsequence */
			EXCCHECK( LoadTestSequence(test_form_xml, gs_Automat.pTestSeq, gs_Modules, FALSE, &is_Error)); 
			}
		
			/* Display */
			sprintf (Msg, "Insérer La Piéce Type : '%s'", MastersDescription[Actual_master_part]);	
			PANEL_MSG( AUTOMAT_HELP2, _(Msg));
			
			gs_Automat.TestSeqMode = TESTSEQ_MASTERPART;
			}
				
Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::ProcessEventGuiProduction"
static SElExceptionPtr ProcessEventGuiProduction(void)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	char			test_form_xml[MAX_PATHNAME_LEN];
	int32_t			is_Error = 0;
	int32_t			full;
	int32_t			closed;
	int32_t			finished;
	int32_t			erreur;
	int32_t			insert1, insert2,insert3;

	if (gs_Automat.Mode != AUTOMAT_MASTERPART && gs_Automat.EmtyTable == FALSE)
	{
	/* set mode */
	gs_Automat.Mode = AUTOMAT_PRODUCTION;
	
	/* load testsequence for production */
	if(gs_Automat.TestSeqMode!=TESTSEQ_PRODUCTION)
	{
		EXCCHECKCVI( GetProjectDir(test_form_xml));
		strcat(test_form_xml, PATH_TEST_FILE_FORM_XML); 
	
		EXCCHECK( LoadTestSequence(test_form_xml, gs_Automat.pTestSeq, gs_Modules, TRUE, &is_Error));
		CHECK_ERR( is_Error);
		
		gs_Automat.TestSeqMode = TESTSEQ_PRODUCTION;
		//PANEL_SET_REFERENCES(gs_Automat.pTestSeq->ProductPartNumber, gs_Automat.pTestSeq->PSA_FPN);
	}
	
	gs_Automat.shift_id = 1; //TEST
	
	if(gs_Automat.shift_id==0)
	{
		PANEL_MSG( AUTOMAT_HELP2, _("Selectionner une equipe"));  
		PANEL_MSG( AUTOMAT_HELP1, "");
		ProcessDrawEvents ();
	}

	else
	{
		PANEL_MSG( AUTOMAT_HELP1, _("MODE PRODUCTION"));
		ProcessDrawEvents ();

		//PANEL_SET_REFERENCES(gs_Automat.pTestSeq->ProductPartNumber, gs_Automat.pTestSeq->PSA_FPN);
		
	   if(gs_Automat.Mode==AUTOMAT_PRODUCTION)
		{
			if (gs_pTester)
			{
				EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_1_PRESENCE_PRODUCT_STATION_10", &insert1));
				EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_2_PRESENCE_PRODUCT_STATION_10", &insert2));
				EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_3_PRESENCE_PRODUCT_STATION_10", &insert3));
				
				if (!insert1 || !insert2 ||!insert2)
					PANEL_MSG( AUTOMAT_HELP2, _("Insérer Un Produit"));
				else if (insert1 && insert2 && insert2)
					PANEL_MSG( AUTOMAT_HELP2, _("Appuyer sur Start Cycle"));
			}
		}

	}
	PANEL_MODE( DEFAULT_MODE);
	}
	
Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::ProcessEventMasterSequenceRun"
static SElExceptionPtr ProcessEventMasterSequenceRun(void)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	char			test_form_xml[MAX_PATHNAME_LEN]; 
	int32_t			is_Error = 0;

	EXCCHECK( GetMasters ());

	if (number_of_master_parts == 0) 
		{
		/* reload production testsequence */
		gs_Automat.Mode = AUTOMAT_PRODUCTION;
			
		gs_Automat.TestSeqMode = TESTSEQ_PRODUCTION;
			
		gs_Automat.masterpart = FALSE;
			
		EXCCHECK(  ProcessEventGuiProduction ());
			
		EXCCHECKCVI( GetProjectDir(test_form_xml));
		strcat(test_form_xml, PATH_TEST_FILE_FORM_XML);
	
		EXCCHECK( LoadTestSequence(test_form_xml, gs_Automat.pTestSeq, gs_Modules, TRUE, &is_Error));
		CHECK_ERR( is_Error);
			
		//PANEL_SET_REFERENCES(gs_Automat.pTestSeq->ProductPartNumber, gs_Automat.pTestSeq->PSA_FPN);
		}
			
	else 
		{
		gs_Automat.masterpart = TRUE;
		
		/* set mode passage piece type avant production */
		gs_Automat.Mode = AUTOMAT_MASTERPART;
				
		/* Load first master barcode */
		EXCCHECK( RunMasterSequence (TRUE));
		}

Error:
	EXCRETHROW( pexception);  
}	

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::ProcessEventGuiExpertiseGoMode"
static SElExceptionPtr ProcessEventGuiExpertiseGoMode(void)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;

	/* set mode */
	gs_Automat.Mode = AUTOMAT_EXPERTISE_GO_MODE;
	
	if(gs_Automat.TestSeqMode==TESTSEQ_PRODUCTION)
	{
		PANEL_MSG( AUTOMAT_HELP1, _("Expertise, Go Mode"));
	}
	else if(gs_Automat.TestSeqMode==TESTSEQ_EXPERTISE)
	{
		PANEL_MSG( AUTOMAT_HELP1, _("Expertise Sequence, Go Mode"));
	}
	PANEL_MSG( AUTOMAT_HELP2, _("Insérer Un Produit"));
	PANEL_MODE( DEFAULT_MODE);
				
Error:
	EXCRETHROW( pexception);  
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::ProcessEventGuiExpertiseSequenceCreated"
static SElExceptionPtr ProcessEventGuiExpertiseSequenceCreated(void)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	char			testexp_form_xml[MAX_PATHNAME_LEN]; 
	int32_t			is_Error = 0;

	
	/* set mode */
	gs_Automat.Mode = AUTOMAT_EXPERTISE_SEQUENCE;
		
#if 0    /* formerly excluded lines */
	if (gs_Automat.Mode == AUTOMAT_EXPERTISE_SEQUENCE)
		{
			PromptPopup ("Informations R&R", "Ajouter Numero Piéce", gs_RnR_Piece, 10);	
		}
#endif   /* formerly excluded lines */
			
	PANEL_MSG( AUTOMAT_HELP1, _("Expertise R&R"));  
	PANEL_MSG( AUTOMAT_HELP2, _("Insert Panel"));
	PANEL_MODE( DEFAULT_MODE);
				
Error:
	EXCRETHROW( pexception);  
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::ProcessEventGuiQualityRetest"
static SElExceptionPtr ProcessEventGuiQualityRetest(void)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	char			test_form_xml[MAX_PATHNAME_LEN];
	int32_t			is_Error = 0;
   	int32_t			insert1, insert2,insert3;

	/* set mode */
	gs_Automat.Mode = AUTOMAT_QUALITY_RETEST;
	
	if(gs_Automat.shift_id==0)
		{
			PANEL_MSG( AUTOMAT_HELP2, _("Selectionner une equipe"));  
			PANEL_MSG( AUTOMAT_HELP1, "");
		}
	else 
	{
	PANEL_MSG( AUTOMAT_HELP1, _("Quality Retest"));
	
	EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_1_PRESENCE_PRODUCT_STATION_10", &insert1));
	EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_1_PRESENCE_PRODUCT_STATION_10", &insert2));
	EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_1_PRESENCE_PRODUCT_STATION_10", &insert3));
	
		if (gs_Automat.EmtyTable == TRUE) //mode vidage 
		{
			if (insert1)
				PANEL_MSG( AUTOMAT_HELP2, _("Enlever le produit"));
			else if (!insert1)
				PANEL_MSG( AUTOMAT_HELP2, _("Appuyer sur Start Cycle"));
		}
		
		else 	  //mode production normal
		{	
			if (!insert1)
				PANEL_MSG( AUTOMAT_HELP2, _("Insérer Un Produit"));
			else if (insert1)
				PANEL_MSG( AUTOMAT_HELP2, _("Appuyer sur Start Cycle"));
		}
	}

Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::ProcessEventGuiQualityMaster"
static SElExceptionPtr ProcessEventGuiQualityMaster(void)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	char			test_form_xml[MAX_PATHNAME_LEN]; 
	int32_t			is_Error = 0;

	/* set mode */
	gs_Automat.Mode = AUTOMAT_QUALITY_MASTER;
	
	/* load testsequence for production */
	if(gs_Automat.TestSeqMode!=TESTSEQ_PRODUCTION)
	{   
		EXCCHECKCVI( GetProjectDir(test_form_xml));
		strcat(test_form_xml, PATH_TEST_FILE_FORM_XML);
		
		EXCCHECK( LoadTestSequence(test_form_xml, gs_Automat.pTestSeq, gs_Modules, TRUE, &is_Error));
		CHECK_ERR( is_Error);
		
		gs_Automat.TestSeqMode = TESTSEQ_PRODUCTION;
	}
	
	PANEL_MSG( AUTOMAT_HELP1, _("Quality Master"));
	PANEL_MSG( AUTOMAT_HELP2, _("Insérer Un Produit"));
	PANEL_MODE( DEFAULT_MODE);
				
Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::ProcessEventTestSequenceRun"
static SElExceptionPtr ProcessEventTestSequenceRun(struct _SSledge* me)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	char			form_xml[MAX_PATHNAME_LEN];        
	
		EXCCHECKCVI( GetProjectDir(form_xml));
	
		strcat(form_xml, PATH_TEST_FILE_FORM_XML); 
		
		EXCCHECK( RunTestSequence(form_xml, me, &gs_Automat, gs_EventFactory)); 
		
				
Error:
	EXCRETHROW( pexception);  
}

void GetFieldDataFromName (int count, char  FieldName[MAX_FIELDS][MAX_LENGTH], char FieldData[MAX_FIELDS][MAX_LENGTH], char* SerachedNameField, char* FoundDataField)
{
			
	int i;
	
	for (i=0; i < count; i++)
		{
		if (!strcmp(FieldName[i], SerachedNameField))
			{
				sprintf(FoundDataField, FieldData[i]);
				break;
			}
			else 
			{
				sprintf(FoundDataField, "");
			}
		}
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "JigPanelBarcode"
static SElExceptionPtr JigPanelBarcode(struct _SSledge* me, int32_t* pError)
{
	SElExceptionPtr	pexception = NULL;
	STracaPtr		ptraca = NULL;
	int32_t			error = 0;
	char            buffer[128];
	char            barcode[128] = "none";
	char            barcode_SMM[128] = "none";
	char			BoardVer_sFin[8+1] = "";
	char			BoardVer_pFin[8+1] = "";
	int				id_Board_sFin;
	int     		id_Board_pFin;
	int				valide, active;
	char			ReferenceProduct[20+1] = "";
	int             result;
	char 			FieldName[MAX_FIELDS][MAX_LENGTH];
	char 			FieldData[MAX_FIELDS][MAX_LENGTH];
	int             count;
	int             i;
	char			msg[512] = "";
	int   			result_SMM;
	char 			Num_Serie_SMM[29+1];

	
		EXCCHECK( gs_pTracas->GetTracaByIndex(gs_pTracas, 0, &ptraca));   //  traca
					
		/* lecture avec lecteur code à barre */
		PANEL_MSG( AUTOMAT_HELP2, _("('Lecture DataMatrix FCT12')"));
		pexception = gs_pTester->GetItemCode(gs_pTester, 0, 0, 512, buffer, 3000);	
		if (pexception)
		{
			PANEL_MSG( AUTOMAT_HELP2, _("('Probleme Lecture DataMatrix !')"));
			if(pError) *pError = -1;
			CHECK_ERR( *pError);
		}
		
		PANEL_MSG( AUTOMAT_HELP2, _("('Lecture DataMatrix OK')"));
		
		
		/* enlever le \r\n */
		for(i= 0 ; i < strlen(buffer); i++)
		{
			if (buffer[i] == 0x0D)
				break;	
			else 
				barcode[i] = buffer[i];
		}

		/* vérifierF longueur = 20 */	
		if (strlen(barcode) != 20)
		{
			PANEL_MSG( AUTOMAT_HELP2, _("Longueur Code à barre erroné"));
			if(pError) *pError = -1;
			CHECK_ERR( *pError);
		}		

		sprintf(me->Num_Serie_sFin, barcode);
		
		PANEL_MSG( AUTOMAT_HELP2, _("('Vérification  DataMatrix OK')"));
		/* extraire board ver */
		sscanf(barcode, "SKR_%s", buffer);
		CopyBytes(BoardVer_sFin, 0 , buffer, 0, 7);
		CopyBytes(me->Serial_Board_Ver_sFin, 0 , buffer, 7, 9);//YYWW12345


		/* verifier si board_ver_sFin est active */
		EXCCHECK( ptraca->Get_BoardVer_sFin_Valide(ptraca, BoardVer_sFin, &id_Board_sFin, &valide));
		if(!valide)
			{
				PANEL_MSG( AUTOMAT_HELP2, _("Board_Ver non valide"));
				if(pError) *pError = -1;
				CHECK_ERR( *pError);
			}
			
		/* verifier si board_ver_sFin est active et valide */
		EXCCHECK( ptraca->Get_BoardVer_sFin_Active(ptraca, id_Board_sFin, &valide, &active));
		if(!valide)
			{
				PANEL_MSG( AUTOMAT_HELP2, _("Board_Ver non valide"));
				if(pError) *pError = -1;
				CHECK_ERR( *pError);
			}
		if(!active)
			{
				PANEL_MSG( AUTOMAT_HELP2, _("Board_Ver non active"));
				if(pError) *pError = -1;
				CHECK_ERR( *pError);
			}
			
		/* extraire id_board_ver_sFin + code asteel_flash*/
		EXCCHECK( ptraca->Get_Reference_Product(ptraca, id_Board_sFin, &id_Board_pFin, ReferenceProduct));

		/* vérifier si la version sélectionner dans interface soft est coreespond à la code asteel flash avtive+valide */
		if(strcmp (ReferenceProduct, gs_Automat.pTestSeq->Code_AsteelFlash))
			{
				sprintf(msg, "référence selectionné %s '(référence active %s)'", gs_Automat.pTestSeq->Code_AsteelFlash, ReferenceProduct);
				PANEL_MSG( AUTOMAT_HELP2, msg);
				if(pError) *pError = -1;
				CHECK_ERR( *pError);
			}
			
		/* extraire board_ver_pFin */
		EXCCHECK( ptraca->Get_BoardVer_pFin(ptraca, id_Board_pFin, BoardVer_pFin));
		if (strlen(BoardVer_pFin) == 0 || id_Board_pFin == 0)
		{
			sprintf(msg, "id_Board_pFin non trouvé '(id_Board_pFin = %d)'", id_Board_pFin);
			PANEL_MSG( AUTOMAT_HELP2, msg);
			if(pError) *pError = -1;
			CHECK_ERR( *pError);		
		}
		
		me->id_Board_pFin = id_Board_pFin;
		sprintf(me->Board_Ver_pFin, BoardVer_pFin);
		
		/* vérifier si Last Movement TEST est OK */
		EXCCHECK( ptraca->Get_Last_Mouvement(ptraca, barcode, gs_Automat.Last_Mvt, &result));
		if(!result)
			{
				sprintf(msg, "Produit non Testé Bon au poste %s !", gs_Automat.Last_Mvt);
				PANEL_MSG( AUTOMAT_HELP2, msg);
				if(pError) *pError = -1;
				CHECK_ERR( *pError);
			}
			
		PANEL_MSG( AUTOMAT_HELP2, _("('Last Mouvement OK')"));
		
		/* extraire Field Label datas */
	/*	EXCCHECK( ptraca->Get_FieldLabel(ptraca, id_Board_pFin, &count, FieldName, FieldData));
		me->pFieldLabel = calloc(1, sizeof(SFieldLabel));
		EXCCHECKALLOC(me->pFieldLabel);
		GetFieldDataFromName (count, FieldName,FieldData, "SAPMN", me->pFieldLabel->SAPMN);
		if (strlen (me->pFieldLabel->SAPMN) == 0)
		{
			sprintf(msg, "Field data non trouvé '(Label %s)'", "SAPMN");
			PANEL_MSG( AUTOMAT_HELP2, msg);
			if(pError) *pError = -1;
			CHECK_ERR( *pError);		
		}
		GetFieldDataFromName (count, FieldName,FieldData, "HW-ID", me->pFieldLabel->HWID);
		if (strlen (me->pFieldLabel->HWID) == 0)
		{
			sprintf(msg, "Field data non trouvé '(Label %s)'", "HW-ID");
			PANEL_MSG( AUTOMAT_HELP2, msg);
			if(pError) *pError = -1;
			CHECK_ERR( *pError);		
		}
		GetFieldDataFromName (count, FieldName,FieldData, "SAP supplier", me->pFieldLabel->SAP_Supplier);
		if (strlen (me->pFieldLabel->SAP_Supplier) == 0)
		{
			sprintf(msg, "Field data non trouvé '(Label %s)'", "SAP supplier");
			PANEL_MSG( AUTOMAT_HELP2, msg);
			if(pError) *pError = -1;
			CHECK_ERR( *pError);		
		}
		GetFieldDataFromName (count, FieldName,FieldData, "Suppliers series No", me->pFieldLabel->Supplier_Series);
		if (strlen (me->pFieldLabel->Supplier_Series) == 0)
		{
			sprintf(msg, "Field data non trouvé '(Label %s)'", "Suppliers series No");
			PANEL_MSG( AUTOMAT_HELP2, msg);
			if(pError) *pError = -1;
			CHECK_ERR( *pError);		
		}
		GetFieldDataFromName (count, FieldName,FieldData, "VDE", me->pFieldLabel->VDE_Version);
		if (strlen (me->pFieldLabel->VDE_Version) == 0)
		{
			sprintf(msg, "Field data non trouvé '(Label %s)'", "VDE");
			PANEL_MSG( AUTOMAT_HELP2, msg);
			if(pError) *pError = -1;
			CHECK_ERR( *pError);		
		}
		GetFieldDataFromName (count, FieldName,FieldData, "temperature", me->pFieldLabel->Temperature);
		if (strlen (me->pFieldLabel->Temperature) == 0)
		{
			sprintf(msg, "Field data non trouvé '(Label %s)'", "temperature");
			PANEL_MSG( AUTOMAT_HELP2, msg);
			if(pError) *pError = -1;
			CHECK_ERR( *pError);		
		}
		GetFieldDataFromName (count, FieldName,FieldData, "Voltage", me->pFieldLabel->Nominal_Voltage);
		if (strlen (me->pFieldLabel->Nominal_Voltage) == 0)
		{
			sprintf(msg, "Field data non trouvé '(Label %s)'", "Voltage");
			PANEL_MSG( AUTOMAT_HELP2, msg);
			if(pError) *pError = -1;
			CHECK_ERR( *pError);		
		}
		GetFieldDataFromName (count, FieldName,FieldData, "Nominal power", me->pFieldLabel->Nominal_Power);
		if (strlen (me->pFieldLabel->Nominal_Power) == 0)
		{
			sprintf(msg, "Field data non trouvé '(Label %s)'", "Nominal power");
			PANEL_MSG( AUTOMAT_HELP2, msg);
			if(pError) *pError = -1;
			CHECK_ERR( *pError);		
		}
*/
		
		/* Extraire data SMM */
			/*strncpy(barcode_SMM, barcode,20);
			EXCCHECK( ptraca->Test_SMM(ptraca, BoardVer_pFin, &result_SMM));
			if(result_SMM==0)
				sprintf (me->SMM, "00000000000000000000000000000");
			else if(result_SMM==1)
				{
					EXCCHECK( ptraca->Get_SMM_SN(ptraca, barcode_SMM, Num_Serie_SMM));
					sprintf (me->SMM,Num_Serie_SMM);
				}*/

		PANEL_MSG( AUTOMAT_HELP2, _("('Lecture data base des données OK')"));
Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::ProcessEventJigItemCycleStarted"   
static  SElExceptionPtr ProcessEventJigItemCycleStarted(struct _SSledge* me)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	bool_t			failed = FALSE;
	int32_t			full;
	int32_t			closed;
	int32_t			finished;
	int32_t			erreur;
	int32_t			barrier;
	int32_t			i = 0; 
	int32_t			MicroCam;
	int32_t			insert1, insert2,ProductInConvoyer,ProductOutConvoyer;
    int32_t 		status_micro_cam;
	char 			Barcode[10+1];

	int32_t		    status = -1;
	int32_t         insert;
	int32_t   		AI_LEFT,AI_CENTRAL,AI_RIGHT;
	HANDLE  		hThreadPalett;
	char            Msg[128];
	
	
	
		LED_OPERATOR(COLOR_GREEN);
		//check If station 60  Is Working 
		if( StatusStations[4] == BUSY || PickAndPlace == 1)
		{
		 PANEL_MSG( AUTOMAT_HELP2, "Wait BUSY STATIONS  !");
		 SetCtrlAttribute(gs_panel[AUTOMAT], AUTOMAT_HELP2, ATTR_FRAME_COLOR, VAL_YELLOW);
		 CHECK_ERR5( -1);
			
		}
		
		
		gs_pTester->GetSensor(gs_pTester, "SENSOR_CONV_IN", &ProductInConvoyer);
		if(ProductInConvoyer && StatusStations[STATION_60] == NOT_BUSY )
		{
			PANEL_MSG( AUTOMAT_HELP2, "A part exist in the convoyer : Remove Part !");
			SetCtrlAttribute(gs_panel[AUTOMAT], AUTOMAT_HELP2, ATTR_FRAME_COLOR, VAL_RED);
		    CHECK_ERR5( -1);
		}
		//check convoyer 
		gs_pTester->GetSensor(gs_pTester, "SENSOR_CONV_OUT", &ProductOutConvoyer);
		if(ProductOutConvoyer)
		{
			PANEL_MSG( AUTOMAT_HELP2, "CONVOYER is FULL PLEASE EMPTY IT !");
			SetCtrlAttribute(gs_panel[AUTOMAT], AUTOMAT_HELP2, ATTR_FRAME_COLOR, VAL_RED);
			Wait_To_Empty_Convoyer();
			PANEL_MSG( AUTOMAT_HELP2, "");
		    CHECK_ERR5( -1);
		}
		
		//check product insertion 
		gs_pTester->GetSensorAI(gs_pTester, "SENSOR_AI_1_PRODUCT_STATION_10", &AI_LEFT);
		gs_pTester->GetSensorAI(gs_pTester, "SENSOR_AI_2_PRODUCT_STATION_10", &AI_CENTRAL);
		gs_pTester->GetSensorAI(gs_pTester, "SENSOR_AI_3_PRODUCT_STATION_10", &AI_RIGHT);
		AI_LEFT    = (int)AI_LEFT/32;
		AI_CENTRAL = (int)AI_CENTRAL/32;
		AI_RIGHT   = (int)AI_RIGHT/32;

		if(me->Skip == FALSE)
		{

			if(!strcmp ("YES", gs_Automat.pTestSeq->SMB_CHECK))
			{
				if((AI_LEFT<310)||(AI_LEFT>314))//+/-580 IF SMB EXIST
				{
					PANEL_MSG( AUTOMAT_HELP2, _("incorrectly LEFT inserted product !!! Please check SMB module"));
					PANEL_MSG( AUTOMAT_HELP1, "");
					CHECK_ERR5( -1);

				}
			}

			else
			{
				if((AI_LEFT<290)||(AI_LEFT>297))
				{
					PANEL_MSG( AUTOMAT_HELP2, _("incorrectly LEFT inserted product:This Reference is without SMB"));
					PANEL_MSG( AUTOMAT_HELP1, "");
					CHECK_ERR5( -1);

				}

			}

			if((AI_CENTRAL<291)||(AI_CENTRAL>295))
			{
				PANEL_MSG( AUTOMAT_HELP2, _("incorrectly CENTRAL inserted product"));
				PANEL_MSG( AUTOMAT_HELP1, "");
				CHECK_ERR5( -1);

			}

			if((AI_RIGHT<293)||(AI_RIGHT>297))
			{
				PANEL_MSG( AUTOMAT_HELP2, _("incorrectly RIGHT inserted product"));
				PANEL_MSG( AUTOMAT_HELP1, "");
				CHECK_ERR5( -1);

			}
		}
		if(me->Skip == FALSE)
		{
			/* read Barcode */
			EXCCHECK( JigPanelBarcode(me, &error));  
			CHECK_ERR4(error);
		}
	
#if 0    /* formerly excluded lines */
	    if(gs_Automat.Mode==AUTOMAT_MASTERPART && me->Skip == FALSE)
			{
				/* check if scanned barcode exist in the list of master parts of this version */
				if (strcmp(gs_barcode_Master, MastersBarcodes[Actual_master_part]))
				{
				//gs_pTester->UnlockPanelStation10(gs_pTester, 2000);
				sprintf (Msg, "Insérer La Piéce Type : '%s'", MastersDescription[Actual_master_part]);	
				PANEL_MSG( AUTOMAT_HELP2, _(Msg));
				goto Error2; 
				}
			}
#endif   /* formerly excluded lines */
		   	
		PANEL_MSG( AUTOMAT_HELP2, _("Reading ID Palette"));
		
		sprintf(me->barcode, "no_barcode");
		
		do{
			gs_pTester->GetSensor(gs_pTester, "INFRARED_BARRIER", &barrier);			
			if (StatusStations[STATION_60] == 1 &&
				StatusStations[STATION_20] == NOT_BUSY && 
				StatusStations[STATION_30] == NOT_BUSY &&
				StatusStations[STATION_40] == NOT_BUSY &&
				StatusStations[STATION_50] == NOT_BUSY &&
				StatusStations[STATION_60] == NOT_BUSY &&
			    barrier)
				{
				break;
				}
		
			if (!g_timerId)
			    g_timerId = NewAsyncTimer (0.2, -1, 1, Timer_Wait, NULL);
		
		}while (barrier);
		
		DiscardAsyncTimer (g_timerId);
		g_timerId = 0;
		
		if (!barrier)  //re push Start Cycle Button
		{
			PANEL_MSG( AUTOMAT_HELP2, _("Press Start Cycle : barrière  Franchi"));
			//LED_OPERATOR(COLOR_OFF);
			goto Error; 
		}
		
		/* Rotate_Table */

		pexception = JigPanelInsert(&error);

		gs_pTester->GetSensor(gs_pTester, "MICRO_CAM", &MicroCam);
		gs_pTester->GetSensor(gs_pTester, "INFRARED_BARRIER", &barrier);
		
		if (MicroCam == 0)  // table pas dans position pas error => franchi barriere par operateur
			{
			PANEL_MSG( AUTOMAT_HELP2, _("Appuyer sur Start Cycle : Plateau pas en position indexé"));
			do {
				gs_pTester->GetSensor(gs_pTester, "MICRO_CAM", &MicroCam);
				if (MicroCam == 1)
					  break;
			}while (TRUE);
					
			}
		
		else   //check if error table rotation
			{
			CHECK_ERR( error);
			}
		
		/* for next product	*/
		gs_Product_Checked = 0;
		
		/* affect palett Number to product */
		sprintf (me->PalletID, "%d", GetPalettNumber());
		
		/*parameters RnR */
		me->Palett_RnR = atoi(me->PalletID);	
		me->Part_RnR = atoi(gs_RnR_Piece);	
		me->Station_RnR = gs_RnR_Station;
		me->Repeat_RnR = gs_RnR_Repeat;		
			
		Autorisation_Thread = 1;
		
		gs_Automat.isTableEmpty = FALSE;

#if 0    /* formerly excluded lines */					
		if(gs_Automat.Mode == AUTOMAT_MASTERPART && me->Skip == FALSE)
		{
			/* affect Actual_Master_Part to this Thread */
			me->Actual_Master_Part = Actual_master_part;
			me->Station_Master = MastersStations[Actual_master_part];
			sprintf (me->barcode_master_Part ,"%s\0", MastersBarcodes[Actual_master_part]);

			/* increment next master part */
			Actual_master_part++;
		
			/* if list of master parts are finished to empty table */
			if (Actual_master_part == number_of_master_parts)
			{
			PANEL_MSG( AUTOMAT_HELP2, _("Push Start Cycle"));
			
			gs_Automat.EmtyTable = TRUE;
			gs_Table_Rotation = 0;
			}
			
			/* if list of master parts are not yet finished  */
			else 
			{
				if (MastersBarcodes[Actual_master_part] != NULL)
				{
				sprintf (Msg, "Insérer La Piéce Type : '%s'", MastersDescription[Actual_master_part]);	
				PANEL_MSG( AUTOMAT_HELP2, _(Msg));
				}		
			}
		}
				
		//unfreeze panel if Mode R&R
		if(gs_Automat.Mode == AUTOMAT_EXPERTISE_SEQUENCE && gs_EventFactory) 
			gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_GUI_PANEL_UNLOCK", NULL);
		
		if(gs_Automat.Mode != AUTOMAT_MASTERPART)
			{
				if (StatusStations[STATION_10] == NOT_BUSY)
					PANEL_MSG( AUTOMAT_HELP2, _("Appuyer sur Start Cycle"));
			}
		
#endif   /* formerly excluded lines */
		/* Debridage  produit n+1 */	

			
		/* run test sequence */

		pexception = ProcessEventTestSequenceRun(me);
		if(  pexception==NULL)
		{
			failed = FALSE;
		}
		
		else if(pexception)
		{
			EXCDBGSTR( pexception);
			failed = TRUE;
		}
				

		if (me->pTestParam->Skip_All_Test == FALSE)
		{
			if(gs_Automat.Mode == AUTOMAT_MASTERPART)
			{
				pexception =  JigPanelRelease2(me, failed);
				if(pexception==NULL)
				goto Error2; 
			}
	
			else 
			{
			pexception =  JigPanelRelease(me, failed);
			if(pexception==NULL)
				goto Error2; 
	
			}
		}


Error:
	EXCCHECK( gs_pTester->GetSensor(gs_pTester, "MICRO_CAM", &status_micro_cam));
	if(gs_Automat.shift_id && status_micro_cam)
	{
	PANEL_MSG( AUTOMAT_HELP2, _("Appuyer sur Start Cycle"));
	}
	Autorisation_Thread = 1;
	StatusStations[STATION_10] = NOT_BUSY;
	EXCRETHROW( pexception);
	
Error2:
	StatusStations[STATION_10] = NOT_BUSY;
	Autorisation_Thread = 1;
	EXCRETHROW( pexception); 
	
Error4:
	//PanelDebridage (3000);
	StatusStations[STATION_10] = NOT_BUSY;
	Autorisation_Thread = 1;
	EXCRETHROW_NO_DISP( pexception);
	
Error5:
	Autorisation_Thread = 1;
	StatusStations[STATION_10] = NOT_BUSY;
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::ProcessEventJigItemEmpty"
static SElExceptionPtr ProcessEventJigItemEmpty(void)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	bool_t			failed = FALSE;
	int32_t			full;
	int32_t			closed;
	int32_t			finished;
	int32_t			erreur;
	int32_t			barrier;
	int32_t			i = 0; 
	int32_t			MicroCam;
	int32_t			insert;

	if( !gs_Automat.disabled)
	{
		/* Wait all test Finish */
		PANEL_MSG( AUTOMAT_HELP2, _("Rotation Plateau"));  
		
		do{
			gs_pTester->GetSensor(gs_pTester, "INFRARED_BARRIER_OUT", &barrier);
			
			if (StatusStations[STATION_20] == NOT_BUSY && 
				StatusStations[STATION_30] == NOT_BUSY &&
				StatusStations[STATION_40] == NOT_BUSY &&
				StatusStations[STATION_50] == NOT_BUSY &&
				StatusStations[STATION_60] == NOT_BUSY &&
			    barrier)
				{
				break;
				}

			if (!g_timerId)
			    g_timerId = NewAsyncTimer (0.2, -1, 1, Timer_Wait, NULL);
		
		}while (barrier);
		
		DiscardAsyncTimer (g_timerId);
		g_timerId = 0;
		
		if (!barrier)  //re push Start Cycle Buttom
		{
			PANEL_MSG( AUTOMAT_HELP2, _("Appuyer sur Start Cycle : Barriére Franchi"));
			//LED_OPERATOR(COLOR_OFF);
			goto Error; 
		}
		
		/* Rotate_Table */
		pexception = JigPanelInsert2(&error);
		
		gs_pTester->GetSensor(gs_pTester, "MICRO_CAM", &MicroCam);
		gs_pTester->GetSensor(gs_pTester, "INFRARED_BARRIER", &barrier);
		
		if (!MicroCam)  // table pas dans position pas error => franchi barriere par operateur
			{
			PANEL_MSG( AUTOMAT_HELP2, _("Appuyer sur Start Cycle : Plateau pas en position indexé"));
			do {
				gs_pTester->GetSensor(gs_pTester, "MICRO_CAM", &MicroCam);
				if (MicroCam == 1)
					  break;
			}while (TRUE);
			}
	
		else if (MicroCam && !barrier)  // table pas encore bougé => franchi barriere par operateur
			{
			PANEL_MSG( AUTOMAT_HELP2, _("Appuyer sur Start Cycle"));
			EXCTHROW(-1, "rotation Fault"); 			
			}
		else if (MicroCam && pexception)  // table pas ready
			{
			PANEL_MSG( AUTOMAT_HELP2, _("Appuyer sur Start Cycle"));
			EXCTHROW(-1, "rotation Fault"); 			
			}
	

#if 0    /* formerly excluded lines */
		/* connect others */
		pexception = gs_pTester->PlugPanelStation60(gs_pTester, 0);
		
		/* connect 10 */
		do {
			pexception = gs_pTester->PlugPanelStation10(gs_pTester, 3000);
				if (pexception == NULL)
					  break;
		}while (TRUE);
		
		Sleep (200);
#endif   /* formerly excluded lines */
		
	}
		
Error:
	Autorisation_Thread = 1;
	EXCRETHROW_NO_DISP( pexception);	
	
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::ProcessEventJigItemReleased"
static SElExceptionPtr ProcessEventJigItemReleased(void)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	int32_t			full;
	int32_t			closed;
	int32_t			finished;
	int32_t			erreur;
	
	PANEL_RESULT("");
	//LED_OPERATOR (COLOR_OFF);

	if( gs_Automat.Mode!=AUTOMAT_MASTERPART)
	{
	if(gs_Automat.shift_id==0)
	{
		PANEL_MSG( AUTOMAT_HELP2, _("Selectionner une equipe"));  
		PANEL_MSG( AUTOMAT_HELP1, "");
	}
	  
	 if (StatusStations[STATION_10] == NOT_BUSY)
		{
			if (gs_Automat.EmtyTable == TRUE)
				{
				//LED_OPERATOR (COLOR_OFF);
				PANEL_STATION_STATUS(STATION_10,FREE);
				PANEL_MSG( AUTOMAT_HELP2, _("Appuyer sur Start Cycle"));
				}
			else
				{
				//LED_OPERATOR (COLOR_OFF);
				PANEL_STATION_STATUS(STATION_10, FREE);
				PANEL_MSG( AUTOMAT_HELP2, _("Insérer Un Produit"));
				gs_Product_Checked = FALSE;
				//LED_OPERATOR(COLOR_OFF);
				}
		}

	}

Error:
	EXCRETHROW( pexception);  
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::ProcessEventJigItemInserted"
static SElExceptionPtr ProcessEventJigItemInserted(void)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	int32_t			full;
	int32_t			closed;
	int32_t			finished;
	int32_t			erreur;
	bool_t			is_locked = FALSE;
	char            buffer [512];
	
	
	if(gs_Automat.shift_id==0)
	{
		PANEL_MSG( AUTOMAT_HELP2, _("Selectionner une equipe"));  
		PANEL_MSG( AUTOMAT_HELP1, "");
	}
	
	else 
		{ 	
			PANEL_STATION_STATUS(STATION_10, RUN);
		
			if (gs_Automat.EmtyTable == TRUE)
				{
					Sleep(1000); //wait for change Flag  StatusStations[STATION_10]
					if (StatusStations[STATION_10] == NOT_BUSY)
					PANEL_MSG( AUTOMAT_HELP2, _("Enlever le produit"));
				}
		
			else 
				{
				Sleep(1000); //to confirm that StatusStations[STATION_10] = NOT_BUSY 
				
			    if (StatusStations[STATION_10] == NOT_BUSY && Autorisation_Thread == 1 && !gs_Automat.disabled)
					{
#if 0    /* formerly excluded lines */
						gs_Product_Checked = 0;
						
						pexception = gs_pTester->GetItemCode(gs_pTester, 0, 0, 512, buffer, 3000);	
						if (pexception)
							{
								//LED_OPERATOR(COLOR_RED);
								PANEL_MSG( AUTOMAT_HELP2, _("('Probléme Lecture Barcode!')"));
								EXCCHECK( pexception);
							}
							
						pexception = PanelBridage (10000);
						if (pexception)
							{
								//LED_OPERATOR(COLOR_RED);
								PANEL_MSG( AUTOMAT_HELP2, _("('Probléme bridage produit!')"));
								EXCCHECK( pexception);
							}
						
						//LED_OPERATOR(COLOR_ORANGE);
				
						PANEL_MSG( AUTOMAT_HELP2, _("Attente Lecture barre code"));
						if (gs_Automat.CHECK_PCB_active)
							{
								/*pexception = CheckNormalMode( gs_pcan, 3000);
								if (pexception)
								{
									//LED_OPERATOR(COLOR_RED);
									PANEL_MSG( AUTOMAT_HELP2, _("('Probléme Communication CAN')"));
									EXCCHECK( pexception);
								}*/
							}
					PANEL_STATION_STATUS(STATION_10,RUN);
					//LED_OPERATOR(COLOR_GREEN);
#endif   /* formerly excluded lines */
					PANEL_MSG( AUTOMAT_HELP2, _("Appuyer sur Start Cycle"));
					gs_Product_Checked = 1;
				}
			}
		
		}

Error: 
	if (pexception)
	{
	//attente barriere + debridage 	
	//PanelDebridage (3000);
	gs_Product_Checked = 0;
	}
	return 0;  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "sledge_new"
SElExceptionPtr	sledge_new(SSledgePtr* pSledge)
{
	SElExceptionPtr	pexception = NULL;
	SSledgePtr		me = NULL;

	me = calloc(1, sizeof(SSledge));
	EXCCHECKALLOC( me);

	me->Activate = Activate;
	me->Deactivate = Deactivate;

	InitializeCriticalSection(&me->_Lock);

	if(pSledge) *pSledge = me;

Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "sledge_delete"
SElExceptionPtr	sledge_delete(SSledgePtr* pSledge)
{
	SElExceptionPtr	pexception = NULL;

	if(pSledge && *pSledge)
	{
		SSledgePtr	me = *pSledge;

		DeleteCriticalSection(&me->_Lock);

		free(*pSledge);
		*pSledge = NULL;
	}

Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "Activate"
static SElExceptionPtr Activate(struct _SSledge* me)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	int32_t			status;

	//printf("Activate\r\n");
	/* start thread */
	EXCCHECK( ThreadStart(Thread_Sledge, me, &me->_hThread, NULL));  

	
Error:   
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "Deactivate"
static SElExceptionPtr Deactivate(struct _SSledge* me)
{
	SElExceptionPtr pexception = NULL;

	EXCCHECK( ThreadStop(me->_hThread, me->_hEventStop));

Error:   
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::ProcessEndVerion"
static SElExceptionPtr ProcessEndVerion(void)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;

	 pexception  = ProcessEventJigItemEmpty();
	 
	 if (pexception == NULL && !gs_Automat.disabled)
		{
			gs_Table_Rotation++;
		
			if (gs_Table_Rotation == NUMBER_OF_STATIONS)
				{
				PANEL_STATION_FAULT(STATION_10 , STATION_10*10, NULL, 1);
				PANEL_STATION_STATUS(STATION_10,FREE);
				ProcessDrawEvents ();
				gs_Automat.isTableEmpty = TRUE;
				gs_Automat.EmtyTable = FALSE;
				PANEL_MSG( AUTOMAT_HELP2, _(""));
				/*Start Normal production Mode */
				EXCDISPLAY( ProcessEventGuiProduction());
				}
		
			else 
			{
				PANEL_STATION_FAULT(gs_Table_Rotation , gs_Table_Rotation*10, NULL, 1);
				PANEL_STATION_STATUS(gs_Table_Rotation,FREE);
			}
		}

Error:
	EXCRETHROW( pexception);  
}

 /*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::DisplayVersionList"
static SElExceptionPtr DisplayVersionList(void)
{
	SElExceptionPtr	pexception = NULL;
	SElEventDataPtr pdata = NULL;
	int32_t			error = 0;
	
	//plugin product Run
	EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_GUI_MENU_HIDE", pdata));	
	EXCCHECK( eleventdata_new(&pdata));
	ProcessSystemEvents ();
	ProcessDrawEvents ();
	EXCCHECK( pdata->SetStringPtr(pdata, PLUGIN_PRODUCT));
	EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_PLUGIN_RUN", pdata));
	EXCCHECK( eleventdata_new(&pdata)); 
	EXCCHECK( pdata->SetStringPtr(pdata, "product"));
	EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_GUI_PANEL", pdata));
	
 Error:
	EXCRETHROW( pexception);  
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::ProcessChangeVerion"
static SElExceptionPtr ProcessChangeVerion(void)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
 
	pexception  = ProcessEventJigItemEmpty();
	 
 	if (pexception == NULL && !gs_Automat.disabled)
		{
		gs_Table_Rotation++;
		if (gs_Table_Rotation == NUMBER_OF_STATIONS)
			{
				PANEL_STATION_FAULT(STATION_10 , STATION_10*10, NULL, 1);
				PANEL_STATION_STATUS(STATION_10,FREE);
				EXCDISPLAY( DisplayVersionList());
				PANEL_RESULT("");
				gs_Automat.isTableEmpty = TRUE;
				gs_Automat.EmtyTable = FALSE;
			}
	
		else 
		{
			PANEL_STATION_FAULT(gs_Table_Rotation , gs_Table_Rotation*10, NULL, 1);
			PANEL_STATION_STATUS(gs_Table_Rotation,FREE);
		}
		}

Error:
	EXCRETHROW( pexception);  
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::ProcessMasterParts"
static SElExceptionPtr ProcessMasterParts(void)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
 
	pexception  = ProcessEventJigItemEmpty();
	 
 	if (pexception == NULL && !gs_Automat.disabled)
		{
		gs_Table_Rotation++;
		if (gs_Table_Rotation == NUMBER_OF_STATIONS)
			{
				PANEL_STATION_FAULT(STATION_10 , STATION_10*10, NULL, 1);
				PANEL_STATION_STATUS(STATION_10,FREE);
				EXCDISPLAY( ProcessEventMasterSequenceRun());
				PANEL_RESULT("");
				gs_Automat.isTableEmpty = TRUE;
				gs_Automat.EmtyTable = FALSE;
			}
	
		else 
		{
			PANEL_STATION_FAULT(gs_Table_Rotation , gs_Table_Rotation*10, NULL, 1);
			PANEL_STATION_STATUS(gs_Table_Rotation,FREE);
		}
		
		}

Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::ProcessEmptyTable"
static SElExceptionPtr ProcessEmptyTable(void)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	int32_t			full;
	int32_t			closed;
	int32_t			finished;
	int32_t			erreur;
	char			Msg[128]; 
	int32_t			insert1,insert2,insert3;

	
	/* Check if product in pallett */
	EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_1_PRESENCE_PRODUCT_STATION_10", &insert1));
	EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_2_PRESENCE_PRODUCT_STATION_10", &insert2));
	EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_3_PRESENCE_PRODUCT_STATION_10", &insert3));
	
	if (!insert1 && !insert2 && !insert3 )
	{
		if (gs_Automat.ChangeVersion == FALSE) //End Version
			EXCDISPLAY( ProcessEndVerion());
		else if (gs_Automat.ChangeVersion == TRUE) //Change Version  
	   		EXCDISPLAY( ProcessChangeVerion());
		else if (gs_Automat.MasterPart == TRUE) //Master Parts  
	   		EXCDISPLAY( ProcessMasterParts());
	}

	else if (StatusStations[STATION_10] == NOT_BUSY)
	{
		//PANEL_MSG( AUTOMAT_HELP2, _("Débridage Produit"));
		//JigPanelUnlock(&error);
		StatusStations[STATION_10] = NOT_BUSY;
		Autorisation_Thread = 1;
		PANEL_MSG( AUTOMAT_HELP2, _("Enlever Le Produit"));
	}
			
Error:
	EXCRETHROW( pexception);  
}
/*---------------------------------------------------------------------------*/
int Pushed_Start_Skip_Palett (int timeout)
{
	clock_t _start_test_clock;
	double  time = 0;
	int  	status;
	
	
	_start_test_clock =  clock();
	
	Sleep(1000);
	
	
	do {
		gs_pTester->GetSensor(gs_pTester, "BUTTON_START", &status);//BUTTON_START
		if (status == 0)
			break;
		//printf("status BUTTON_NOK %d \n",status);
		time = (double)(clock()-_start_test_clock);
		
		if (time >= (timeout - 1000))	
			break;
			
		}while (time < timeout);
	
	if (time >= (timeout - 1000))	
		return 1;
	else 
		return 0;
	
}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
int Pushed_Start_Without_Palett ()
{

        int status=0;
		gs_pTester->GetSensor(gs_pTester, "BUTTON_START", &status);//BUTTON_START
		if (status == 1)
			Sleep(300);
		gs_pTester->GetSensor(gs_pTester, "BUTTON_START", &status);//BUTTON_START

		return status;
	
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::ProcessJig"
static SElExceptionPtr ProcessJig(void)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	int Status_Palett = 1;
	SSledgePtr	pSledge = NULL;
	int         insert1,insert2,insert3;
	char 		msg[128];

	SetCtrlVal(gs_panel[AUTOMAT], AUTOMAT_LED_AUTORISATION, Autorisation_Thread);
	SetCtrlVal(gs_panel[AUTOMAT], AUTOMAT_LED_STATUS_10, StatusStations[STATION_10]);
   	if (StatusStations[STATION_10] == NOT_BUSY && Autorisation_Thread == 1 && !gs_Automat.disabled)
		{
		  if (gs_Automat.EmtyTable == FALSE )
			  {
				/* check if palett is desactivated */
				PANEL_GET_PALETT_STATUS (GetPalettNumber(), &Status_Palett);
	
				Status_Palett = 1;
				
				if (Status_Palett)
				{
		  			EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_1_PRESENCE_PRODUCT_STATION_10", &insert1));
					EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_2_PRESENCE_PRODUCT_STATION_10", &insert2));
					EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_3_PRESENCE_PRODUCT_STATION_10", &insert3));


					if ( insert1 == 1 && insert2 == 1 && insert3== 1)  /* passage avec produit */
					{
					Autorisation_Thread = 0;
				  	EXCCHECK( sledge_new(&pSledge)); //creation de nouvelle structure pSledge

				  	EXCCHECK( pSledge->Activate(pSledge)); //lancement d'un nouveau thread de pSledge 
					}
							
					else 
					{
                        Sleep(100);
						//if (Pushed_Start_Skip_Palett(3000) == TRUE) /* Operateur demande de passer palette vide */
						if(Pushed_Start_Without_Palett() == TRUE && StatusStations[STATION_60] == 1)
						{
							Autorisation_Thread = 0;
			  				EXCCHECK( sledge_new(&pSledge)); //creation de nouvelle structure pSledge
							pSledge->Skip = TRUE; //skipp all test	
		  					EXCCHECK( pSledge->Activate(pSledge)); //lancement d'un nouveau thread de pSledge 
						}
						else
						{
		    				Autorisation_Thread = 1;		
						}
					}
				}
					
				else 
				{
	  				EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_1_PRESENCE_PRODUCT_STATION_10", &insert1));
					EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_2_PRESENCE_PRODUCT_STATION_10", &insert2));
					EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_3_PRESENCE_PRODUCT_STATION_10", &insert3));
					
					if ( insert1 == 0 && insert2 == 0 && insert3 == 0 )  /* passage avec palette vide */
					{				
					Autorisation_Thread = 0;
			  		EXCCHECK( sledge_new(&pSledge)); //creation de nouvelle structure pSledge
					pSledge->Skip = TRUE; //skipp all test	
		  			EXCCHECK( pSledge->Activate(pSledge)); //lancement d'un nouveau thread de pSledge 
					}
				
					else 
					{
				//	gs_pTester->UnlockPanelStation10(gs_pTester, 2000);
					sprintf (msg, "Enlever le Produit Palette 'P%d' est Désactivé", GetPalettNumber());
					PANEL_MSG( AUTOMAT_HELP2, msg);
		    		Autorisation_Thread = 1;	
					}
				}	
			  }	
		else 
		 	{
			Autorisation_Thread = 0;
	 	 	 EXCDISPLAY( ProcessEmptyTable());	  
			}								   
		}
	
Error:
	EXCRETHROW( pexception);  
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Thread_Automat"
static DWORD WINAPI Thread_Automat(LPVOID param)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	DWORD 			result;
	bool_t			b_loop_stop = FALSE;
	char			msg[512];
	bool_t			is_locked = FALSE;
	SSledgePtr		pSledge = NULL;
	int             insert;
	
	do
	{
	result = WaitForMultipleObjects(EVENT_HANDLE_NB, gs_EventHandle, FALSE, 10000);
		
		EXCCHECK( fncLock());
		is_locked = TRUE;  
		
		switch(result)
		{	
			case WAIT_OBJECT_0 + EVNT_GUI_PRODUCT_SELECTED:
				EXCDISPLAY( ProcessEventGuiProductSelected(TRUE, TRUE, TRUE));
				break;
			case WAIT_OBJECT_0 + EVNT_RESET_PRODUCT:
				EXCDISPLAY( ProcessEventGuiResetProduct());
				break;
			case WAIT_OBJECT_0 + EVNT_GUI_PRODUCTION:
			//	printf("WAIT_OBJECT_0 + EVNT_GUI_PRODUCTION\r\n");
				EXCDISPLAY( ProcessEventGuiProduction());
				break;
			case WAIT_OBJECT_0 + EVNT_GUI_QUALITY_RETEST:
				EXCDISPLAY( ProcessEventGuiQualityRetest());
				break;
			case WAIT_OBJECT_0 + EVNT_GUI_QUALITY_MASTER:
				EXCDISPLAY( ProcessEventGuiQualityMaster());
				break;
			case WAIT_OBJECT_0 + EVNT_GUI_EXPERTISE_GO_MODE:
				EXCDISPLAY( ProcessEventGuiExpertiseGoMode());
				break;
			case WAIT_OBJECT_0 + EVNT_GUI_EXPERTISE_SEQUENCE_CREATED:
				EXCDISPLAY( ProcessEventGuiExpertiseSequenceCreated());
				break;
			case WAIT_OBJECT_0 + EVNT_JIG_CYCLE_START_:
			//	printf("WAIT_OBJECT_0 + EVNT_JIG_CYCLE_START_\r\n");
				EXCDISPLAY( ProcessJig());
			break;
			case WAIT_OBJECT_0 + EVNT_STOP_THREAD:
				{
				MessagePopup ("THREAD", "EVNT_STOP_THREAD");
				b_loop_stop = TRUE;
				}
				break;
			case WAIT_TIMEOUT:
				break;
			case WAIT_FAILED:
				EXCTHROW(-2, "Thread_Automat Fault");
				break;
		}
		
		fncUnlock();
		is_locked = FALSE;

	}while(!b_loop_stop);

Error:
	if( is_locked) fncUnlock();
	EXCDISPLAY(pexception);
	ExitThread(THREAD_EXIT_CODE);
	return 0;
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Thread_Sledge"
static DWORD WINAPI Thread_Sledge(LPVOID param)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	DWORD 			result;
	bool_t			b_loop_stop = FALSE;
	char			barcode[512];
	bool_t			is_locked = FALSE;
	SSledgePtr		pSledge = (SSledgePtr) param;
	SSledgePtr		me = (SSledgePtr) param;//Added

	//printf("Thread_Sledge\r\n");	
	EXCDISPLAY( ProcessEventJigItemCycleStarted(pSledge)); 
	
	/* free pointer pSledge */
	EXCCHECK( sledge_delete(&pSledge));
	
	/* terminate thread */
	if(me->_hThread)
	{
		CloseHandle(me->_hThread);
		me->_hThread = NULL;
	}
	
Error:
	EXCDISPLAY(pexception);
	ExitThread(THREAD_EXIT_CODE);
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__											   
#define __FUNC__ "JigPanelLabel"
SElExceptionPtr JigPanelLabel(struct _SSledge* me, int32_t* pError)  //karim 
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	bool_t			isCorrect = FALSE;
	int32_t			master_id;
	char            Buffer[512]= "";
	char            barcode1[32]= "";
	char            barcode2[32]= "";
   
   
	StatusStations[STATION_10] = BUSY;
		
	/* Read Label Barcode */
	EXCCHECK( gs_pTester->GetItemCode(gs_pTester, 0, 0, 512, Buffer, 3000));    //skipper pour desactiver code à barre 
	ExtractDoubleBarcode(Buffer, barcode1, barcode2);
	sprintf(me->barcode, "%s", barcode1);
   //sprintf(me->barcode, "%s", "00000");	  //activer pour desactiver code à barre

Error:  
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "JigPanelLabelFail"
SElExceptionPtr JigPanelLabelFail(struct _SSledge* me, int32_t* pError)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	bool_t			isCorrect = FALSE;
	int32_t			master_id;
	char            Buffer[512]= "";
	int             index;	
	SPrinterPtr		printer;
	int 			iStatus;
	clock_t _start_clock;
	double   time;	
	int		StatusOnline;
	int		Interpreter;
	int		AmmountLabels;
	char	Error[128] = "";
	char	sAmmount[128] = "";
    int     status_barrier;
   
   
		StatusStations[STATION_10] = BUSY;
		
		gs_pPrinters->GetPrinterByIndex(gs_pPrinters, 0, &printer);
		
		/* Electovalve AIR PRINTER */
		EXCCHECK( gs_pTester->SetSensor(gs_pTester, "PRINTER_AIR_VALVE", 1));
		Sleep(100);
	
		_start_clock =  clock();
		
		do{
			/* Operator confirm print */
			PANEL_MSG( AUTOMAT_HELP2, _("APPUI START CYCLE POUR IMPRESSION"));
			do{
				EXCCHECK( gs_pTester->GetSensor(gs_pTester, "INFRARED_BARRIER", &status_barrier));
 				if (status_barrier)
				break;
		  	}while (TRUE);
		
			/* make reset Printer */
			PANEL_MSG( AUTOMAT_HELP2, _("Reset Buffer imprimante"));
			printer->Reset(printer);
			
			Sleep(1000);
			
			/* Check if Printer Reset */
			printer->Status(printer, &StatusOnline, Error, &AmmountLabels, &Interpreter);
			if (StatusOnline == 1 && strcmp (Error, "No error") == 0 && Interpreter == 0)		
			  break;
	
		TIMEOUT_PRINTER_CHECK (TIME_OUT_PRINTER);//100s sinon erreur imprimante
			
		}while (TRUE);

		_start_clock =  clock();
		
		do{
			/* Operator confirm print */
			PANEL_MSG( AUTOMAT_HELP2, _("APPUI START CYCLE POUR IMPRESSION"));
			do{
				EXCCHECK( gs_pTester->GetSensor(gs_pTester, "INFRARED_BARRIER", &status_barrier));
 				if (status_barrier)
				break;
		  	}while (TRUE);
		
			/* Check if Printer Reset */
			PANEL_MSG( AUTOMAT_HELP2, _("Activation Applicateur Imprimante"));
			pexception = gs_pTester->ReadyPrint(gs_pTester, 6000);
			if (pexception == NULL)		
			  break;
	
		TIMEOUT_PRINTER_CHECK (TIME_OUT_PRINTER);//100s sinon erreur imprimante
			
		}while (TRUE);
				
		/* send label NOK */
		PANEL_MSG( AUTOMAT_HELP2, _("Envoi Etiquette Mauvaise à l'imprimante"));
		EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_TEST_SEQUENCE_END", me->pData));
		
		/* Check if data already sent to Printer */
		_start_clock =  clock();
		do{
		Sleep(300);
		printer->Status(printer, &StatusOnline, Error, &AmmountLabels, &Interpreter);
		if (StatusOnline == 1 && Interpreter == 1)		
		  break;
		TIMEOUT_CHECK (5000);
		}while (TRUE);

		_start_clock =  clock();
		
		do{
			EXCCHECK( gs_pTester->GetSensor(gs_pTester, "INFRARED_BARRIER", &status_barrier));
			if (!status_barrier)
				{
				/* Operator confirm print */
				PANEL_MSG( AUTOMAT_HELP2, _("APPUI START CYCLE POUR IMPRESSION"));
					do{
						EXCCHECK( gs_pTester->GetSensor(gs_pTester, "INFRARED_BARRIER", &status_barrier));
 						if (status_barrier)
							break;
					  }while (TRUE);
				}
			
			/* make Ready Printer */
			PANEL_MSG( AUTOMAT_HELP2, _("Activation Applicateur Imprimante"));
			gs_pTester->ReadyPrint(gs_pTester, 6000);
			
			/* Print label */
			PANEL_MSG( AUTOMAT_HELP2, _("Cycle Impression En Cours"));
			pexception = gs_pTester->CyclePrint(gs_pTester, 7000);
			if (pexception == NULL)		
			  break;
		
		TIMEOUT_PRINTER_CHECK (TIME_OUT_PRINTER);//100s sinon erreur imprimante
			
		}while (TRUE);

Error:  
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "GetNewSerial"
SElExceptionPtr GetNewSerial(int* counter, int32_t* pError)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	EIdShift		shift_id = 0;
	bool_t			isOpen = FALSE;
	

		EXCCHECK( gs_pDbs->Storage->IsShiftOpen(gs_pDbs->Storage, &isOpen, &shift_id));
		if(isOpen)
		{
			pexception = gs_pDbs->Storage->GetProductCounter(gs_pDbs->Storage, gs_pDbs->TesterProductId, counter);
			if(pexception)
			{	
			EXCDELETE(pexception);
			if(pError) *pError = -1;
			CHECK_ERR( *pError);
			}
		}

Error:
	EXCRETHROW( pexception);  
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "JigPanelUnlock"
static SElExceptionPtr JigPanelUnlock(int32_t* pError)
{
	SElExceptionPtr		pexception = NULL;
	int32_t				error = 0;
	bool_t			    barrier, MicroCam;
	
				
		pexception = gs_pTester->UnlockPanelStation10(gs_pTester, TIMEOUT);
		if(pexception)
		{
		EXCDELETE(pexception);
		if(pError) *pError = -1;
		CHECK_ERR( *pError);
		}
			
Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
//#undef __FUNC__
//#define __FUNC__ "JigPanelLock"
//static SElExceptionPtr JigPanelLock(int32_t* pError)
//{
//	SElExceptionPtr		pexception = NULL;
//	int32_t				error = 0;
//	bool_t			    barrier;
//	
//		
//		PANEL_MSG( AUTOMAT_HELP2, _("Lock Panel"));
//		
//				
//		pexception = gs_pTester->LockPanelStation10(gs_pTester, TIMEOUT);
//		if(pexception)
//		{	
//		EXCDELETE(pexception);
//		if(pError) *pError = -1;
//		CHECK_ERR( *pError);
//		}
//			
//Error:
//	EXCRETHROW( pexception);  
//}
///*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "JigPanelInsert"
static SElExceptionPtr JigPanelInsert(int32_t* pError)
{
	SElExceptionPtr		pexception = NULL;
	int32_t				error = 0;

		gs_pTester->SetSensor(gs_pTester, "INDICATOR_OK", TRUE);
		PANEL_MSG( AUTOMAT_HELP2, _("Rotation Plateau"));
		pexception = gs_pTester->TableRotate(gs_pTester, ROTATION_TIMEOUT, TRUE);
		if(pexception)
		{	
		EXCDELETE(pexception);
		if(pError) *pError = -1;
		CHECK_ERR( *pError);
		}
			
Error:
	gs_pTester->SetSensor(gs_pTester, "INDICATOR_OK", FALSE);
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "JigPanelInsert2"
static SElExceptionPtr JigPanelInsert2(int32_t* pError)
{
	SElExceptionPtr		pexception = NULL;
	int32_t				error = 0;

		gs_pTester->SetSensor(gs_pTester, "INDICATOR_OK", TRUE);

		EXCCHECK(gs_pTester->TableRotate(gs_pTester, ROTATION_TIMEOUT, TRUE));
			
Error:
	gs_pTester->SetSensor(gs_pTester, "INDICATOR_OK", FALSE);
	EXCRETHROW( pexception);  
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PanelWaitForRemove"
static SElExceptionPtr PanelWaitForRemove(void)
{
	SElExceptionPtr		pexception = NULL;
	int32_t				error = 0;
    bool_t              insert1,insert2, status_barrier;
    bool_t              plug, unplug;
	
	
#if 0    /* formerly excluded lines */
	do{
		StatusStations[STATION_10] = BUSY;
		
		EXCCHECK( gs_pTester->GetSensor(gs_pTester, "INFRARED_BARRIER_OUT", &status_barrier));
		//printf("PanelWaitForRemove : status_barrier = %d\r\n", status_barrier);
		if (status_barrier)
			break;
	
	}while (TRUE);
#endif   /* formerly excluded lines */
	 		
	
	//gs_pTester->UnlockPanelStation10(gs_pTester, 3000);

	
	do{
		StatusStations[STATION_10] = BUSY;

		EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR__1_PRESENCE_PRODUCT_STATION_10", &insert1));
		EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR__2_PRESENCE_PRODUCT_STATION_10", &insert2));
		//printf("PanelWaitForRemove : insert = %d\r\n", insert);
		if (insert1 == FALSE && insert2 == FALSE )
			break;
		
	}while (TRUE);
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PanelChecked"
static SElExceptionPtr PanelChecked(int timeout)
{
	SElExceptionPtr		pexception = NULL;
	int32_t				error = 0;

	TIMEOUT_INIT(); 		
	do{
		
		if (gs_Product_Checked)
			break;
		
		TIMEOUT_CHECK(timeout);

	}while (TRUE);
				
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PanelBridage"
static SElExceptionPtr PanelBridage(int timeout)
{
	SElExceptionPtr		pexception = NULL;
	int32_t				error = 0;
    bool_t              insert, status_barrier;
    bool_t              gs_lock_station_10,
						gs_unlock_station_10,
						gs_unlock_left_station_10,
						gs_unlock_right_station_10;

	
	TIMEOUT_INIT(); 
		
	// retour vérin 
	do{
		EXCCHECK( gs_pTester->GetSensor(gs_pTester, "INFRARED_BARRIER_OUT", &status_barrier));
 		if (status_barrier)
			gs_pTester->LockPanelStation10(gs_pTester, 0);
	
		
		/* Check if panel locked STATION 10 */ 
		EXCCHECK( gs_pTester->GetSensor(gs_pTester, "IS_PANEL_LOCKED_STATION_10", &gs_lock_station_10));
		EXCCHECK( gs_pTester->GetSensor(gs_pTester, "IS_PANEL_UNLOCKED_STATION_10", &gs_unlock_station_10));

		if (gs_lock_station_10 && !gs_unlock_station_10)
			break;
		
		TIMEOUT_CHECK(timeout);

	}while (TRUE);
		
		
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PanelPresence"
static SElExceptionPtr PanelPresence(int timeout)
{
	SElExceptionPtr		pexception = NULL;
	int32_t				error = 0;
    bool_t              insert, status_barrier;
	
	TIMEOUT_INIT(); 

	//Presence produit OK
	do{
		EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_PRESENCE_PRODUCT_STATION_10", &insert));
		ProcessDrawEvents ();

		TIMEOUT_CHECK(timeout);

	}while (insert == FALSE);
		
		
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
//#undef __FUNC__
//#define __FUNC__ "PanelDebridage"
//static SElExceptionPtr PanelDebridage(int timeout)
//{
//	SElExceptionPtr		pexception = NULL;
//	int32_t				error = 0;
//    bool_t              insert, status_barrier;
//    bool_t              gs_lock_station_10,
//						gs_unlock_station_10;

//	TIMEOUT_INIT(); 

//	// Debridage OK
//	do{
//		EXCCHECK( gs_pTester->GetSensor(gs_pTester, "INFRARED_BARRIER_OUT", &status_barrier));
// 		//if (status_barrier)
//		//	gs_pTester->UnlockPanelStation10(gs_pTester, 0);
//		//
//		///* Check if panel locked STATION 10 */ 
//		//EXCCHECK( gs_pTester->GetSensor(gs_pTester, "IS_PANEL_LOCKED_STATION_10", &gs_lock_station_10));
//		//EXCCHECK( gs_pTester->GetSensor(gs_pTester, "IS_PANEL_UNLOCKED_STATION_10", &gs_unlock_station_10));

//		//if (!gs_lock_station_10 && gs_unlock_station_10 )
//		//	break;
//		
//		TIMEOUT_CHECK(timeout);

//	}while (TRUE);
//		
//Error:
//	EXCRETHROW( pexception); 
//}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "JigPanelRelease"
static SElExceptionPtr JigPanelRelease(struct _SSledge* me, bool_t fault)
{
	SElExceptionPtr		pexception = NULL;
	STracaPtr			ptraca = NULL;
	int32_t				error = 0;
	char 				msg[512] = "";
	
	
	//EXCCHECK( gs_pTracas->GetTracaByIndex(gs_pTracas, 0, &ptraca));
		
	StatusStations[STATION_10] = BUSY;
	
	if(fault)   //Print FAIL Label & put panel in the Red BOX
	{

			PANEL_STATION_STATUS(STATION_10,RUN);
			StatusStations[STATION_10] = BUSY;
			PANEL_RESULT("NOK");
			//LED_OPERATOR (COLOR_RED);
			PANEL_STATION_STATUS(STATION_10,NOK);
			sprintf (msg, "DEFAUT:(%s)", me->pTestParam->Name_Fault);
			PANEL_MSG( AUTOMAT_HELP2, msg);
			Sleep(200);
			EXCCHECK( PanelWaitForRemove());
			PANEL_MSG( AUTOMAT_HELP2, _("PIECE EJECTE"));
			//LED_OPERATOR (COLOR_RED);
			StatusStations[STATION_10] = NOT_BUSY;
	}

	else 						   
	{	
			StatusStations[STATION_10] = BUSY;
			PANEL_RESULT("OK");
			PANEL_STATION_STATUS(STATION_10, OK);
			//LED_OPERATOR(COLOR_GREEN);
			PANEL_MSG( AUTOMAT_HELP2, _("Evacuer La Piece Bonne"));
			Sleep(200);
		   	EXCCHECK( PanelWaitForRemove());
			PANEL_MSG( AUTOMAT_HELP2, _(""));
			//LED_OPERATOR(COLOR_OFF);
			StatusStations[STATION_10] = NOT_BUSY;
	}

Error:
	StatusStations[STATION_10] = NOT_BUSY;
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "JigPanelRelease2"
static SElExceptionPtr JigPanelRelease2(struct _SSledge* me, bool_t fault)
{
	SElExceptionPtr		pexception = NULL;
	int32_t				error = 0;
	double   			time;	
	char      			msg[256];
	int                 i;
	
	
	StatusStations[STATION_10] = BUSY;
	
	if(fault)   //Print FAIL Label & put panel in the Red BOX
	{

		PANEL_STATION_STATUS(STATION_10,RUN);
		StatusStations[STATION_10] = BUSY;
		PANEL_RESULT("NOK");
		//LED_OPERATOR (COLOR_RED);
		PANEL_STATION_STATUS(STATION_10,NOK);
		sprintf (msg, "DEFAUT:'%s'", me->pTestParam->Name_Fault);
		PANEL_MSG( AUTOMAT_HELP2, msg);
		EXCCHECK( PanelWaitForRemove());
		PANEL_MSG( AUTOMAT_HELP2, _(""));
		//LED_OPERATOR (COLOR_RED);
		StatusStations[STATION_10] = NOT_BUSY;
	}

	else 
	{	
		PANEL_STATION_STATUS(STATION_10,RUN);
		PANEL_RESULT("OK");
		PANEL_STATION_STATUS(STATION_10,OK);
		PANEL_MSG( AUTOMAT_HELP2, _("Evacuer La Piece Type Bonne"));
		//LED_OPERATOR(COLOR_GREEN);
	 	EXCCHECK( PanelWaitForRemove());
		PANEL_MSG( AUTOMAT_HELP2, _(""));
		//LED_OPERATOR(COLOR_OFF);
		StatusStations[STATION_10] = NOT_BUSY;
	}

	/* next master part */
	Finished_master_parts++;
	
	Status [me->Actual_Master_Part] = fault;

	/* if Last Finished Master Part */
	if (Finished_master_parts == number_of_master_parts) //the tour is finished
		{
		NOK_Master_Parts = 0;
			
		/* check if all masters parts are OK */	
		for (i=0; i<number_of_master_parts; i++)
			{
				if (Status [i] == TRUE )
					{
					sprintf (MastersBarcodes[NOK_Master_Parts], "%s", MastersBarcodes[i]);//shift register	
					sprintf (MastersDescription[NOK_Master_Parts], "%s", MastersDescription[i]);//shift register
					MastersStations[NOK_Master_Parts] = MastersStations[i]; 
					NOK_Master_Parts++;
					}
			}
		    
		/* if all master parts are OK : GO production */
		if (NOK_Master_Parts == 0)
			{
			/* reload production testsequence */
			gs_Automat.Mode = AUTOMAT_PRODUCTION;
			
			/* Disable master parts mode */
			gs_Automat.masterpart = FALSE;
				
			/* Table not empty */
			gs_Automat.isTableEmpty = FALSE;
			gs_Automat.EmtyTable = FALSE;
		
			PANEL_MSG( AUTOMAT_HELP1, _("MODE PRODUCTION"));	
			EXCCHECK( ProcessEventGuiProduction());  
			StatusStations[STATION_10] = NOT_BUSY;
			}
			
		else 
			{
			int ok = 0;
					
			/* Table not empty */
			gs_Automat.isTableEmpty = FALSE;
			gs_Automat.EmtyTable = FALSE;
			number_of_master_parts = NOK_Master_Parts; //new number of master parts to test	
			PANEL_MSG( AUTOMAT_HELP2, _("Voulez Vous Retester Piéce Type NOK Master?"));	
			EXCCHECK( gs_pTester->OkNokCheck(gs_pTester, 10000, &ok));
			if (!ok)
				{
				/* reload production testsequence */
				gs_Automat.Mode = AUTOMAT_PRODUCTION;
			
				/* Disable master parts mode */
				gs_Automat.masterpart = FALSE;
				PANEL_MSG( AUTOMAT_HELP1, _("MODE PRODUCTION"));
				EXCCHECK( ProcessEventGuiProduction());
				}
				else if (ok)
				{
					number_of_master_parts = NOK_Master_Parts; //new number of master parts to test	
					EXCCHECK( RunMasterSequence (FALSE));
				}
			}
		}
	
Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::GetEventHandle"
static SElExceptionPtr GetEventHandle(void)
{
	SElExceptionPtr	pexception = NULL;
   
	if ( gs_EventFactory )
	{
		EXCCHECK( gs_EventFactory->GetEvent(gs_EventFactory,
		                                   "EVNT_GUI_PRODUCTION",
		                                   &gs_EventHandle[EVNT_GUI_PRODUCTION]));
		EXCCHECK( gs_EventFactory->GetEvent(gs_EventFactory,
		                                   "EVNT_GUI_QUALITY_RELEASE",
		                                   &gs_EventHandle[EVNT_GUI_QUALITY_RELEASE]));
		EXCCHECK( gs_EventFactory->GetEvent(gs_EventFactory,
		                                   "EVNT_GUI_QUALITY_RETEST",
		                                   &gs_EventHandle[EVNT_GUI_QUALITY_RETEST]));
		EXCCHECK( gs_EventFactory->GetEvent(gs_EventFactory,
		                                   "EVNT_GUI_QUALITY_MASTER",
		                                   &gs_EventHandle[EVNT_GUI_QUALITY_MASTER]));
		EXCCHECK( gs_EventFactory->GetEvent(gs_EventFactory,
		                                   "EVNT_GUI_EXPERTISE_STEP_MODE",
		                                   &gs_EventHandle[EVNT_GUI_EXPERTISE_STEP_MODE]));
		EXCCHECK( gs_EventFactory->GetEvent(gs_EventFactory,
		                                   "EVNT_GUI_EXPERTISE_GO_MODE",
		                                   &gs_EventHandle[EVNT_GUI_EXPERTISE_GO_MODE]));
		EXCCHECK( gs_EventFactory->GetEvent(gs_EventFactory,
		                                   "EVNT_GUI_EXPERTISE_SEQUENCE_CREATED",
		                                   &gs_EventHandle[EVNT_GUI_EXPERTISE_SEQUENCE_CREATED]));
		EXCCHECK( gs_EventFactory->GetEvent(gs_EventFactory,
		                                   "EVNT_GUI_EXPERTISE_RNR",
		                                   &gs_EventHandle[EVNT_GUI_EXPERTISE_RNR]));
		EXCCHECK( gs_EventFactory->GetEvent(gs_EventFactory,
		                                   "EVNT_TEST_SEQUENCE_RUN",
		                                   &gs_EventHandle[EVNT_TEST_SEQUENCE_RUN]));
		EXCCHECK( gs_EventFactory->GetEvent(gs_EventFactory,
		                                   "EVNT_GUI_PRODUCT_SELECTED",
		                                   &gs_EventHandle[EVNT_GUI_PRODUCT_SELECTED]));
		EXCCHECK( gs_EventFactory->GetEvent(gs_EventFactory,
		                                   "EVNT_EXPERTISE_STEP_MODE_NEXT",
		                                   &gs_EventHandle[EVNT_EXPERTISE_STEP_MODE_NEXT]));
		EXCCHECK( gs_EventFactory->GetEvent(gs_EventFactory,
		                                   "EVNT_EXPERTISE_STEP_MODE_REPEAT",
		                                   &gs_EventHandle[EVNT_EXPERTISE_STEP_MODE_REPEAT]));
		EXCCHECK( gs_EventFactory->GetEvent(gs_EventFactory,
		                                   "EVNT_EXPERTISE_STEP_MODE_BACK",
		                                   &gs_EventHandle[EVNT_EXPERTISE_STEP_MODE_BACK]));
		EXCCHECK( gs_EventFactory->GetEvent(gs_EventFactory,
		                                   "EVNT_EXPERTISE_STEP_MODE_STOP",
		                                   &gs_EventHandle[EVNT_EXPERTISE_STEP_MODE_STOP]));
		EXCCHECK( gs_EventFactory->GetEvent(gs_EventFactory,
		                                   "EVNT_GUI_KEY_ESC_PRESSED",
		                                   &gs_EventHandle[EVNT_GUI_KEY_ESC_PRESSED]));
		EXCCHECK( gs_EventFactory->GetEvent(gs_EventFactory,
		                                   "EVNT_RESET_PRODUCT",
		                                   &gs_EventHandle[EVNT_RESET_PRODUCT]));
		EXCCHECK( gs_EventFactory->GetEvent(gs_EventFactory,
		                                   "EVNT_JIG_ITEM_INSERTED",
		                                   &gs_EventHandle[EVNT_JIG_ITEM_INSERTED_]));
		EXCCHECK( gs_EventFactory->GetEvent(gs_EventFactory,
		                                   "EVNT_JIG_ITEM_RELEASED",
		                                   &gs_EventHandle[EVNT_JIG_ITEM_RELEASED_]));
		EXCCHECK( gs_EventFactory->GetEvent(gs_EventFactory,
		                                   "EVNT_JIG_CYCLE_START",
		                                   &gs_EventHandle[EVNT_JIG_CYCLE_START_]));
	   	EXCCHECK( gs_EventFactory->GetEvent(gs_EventFactory,
		                                   "EVNT_JIG_NEW",
		                                   &gs_EventHandle[EVNT_JIG_NEW_]));
		EXCCHECK( gs_EventFactory->GetEvent(gs_EventFactory,
		                                   "EVNT_JIG_START",
		                                   &gs_EventHandle[EVNT_JIG_START_]));
	}
    
Error:
	EXCRETHROW(pexception);
}  

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "EventFunctionCallback"
static SElExceptionPtr EventFunctionCallback( const char* eventName, void* pData)
{
	SElExceptionPtr	pexception = NULL;
	int i;
	
	EXCCHECK( fncLock());

	if(0==strcmp(eventName, "EVNT_TESTER_ROTATION_FINISHED") )
	{
	for(i=0; i< NB_OF_STATIONS ;i++)
			{
			TimeStation[i] = 0;
			PANEL_STATION_TIME (i, TimeStation[i]);
			}
	//GetPalettID(&gs_PalettID);
	}
	else if(0==strcmp(eventName, "EVNT_JIG_ITEM_RELEASED"))
	{
		EXCCHECK( ProcessEventJigItemReleased()); 
	}
	else if(0==strcmp(eventName, "EVNT_JIG_ITEM_INSERTED"))
	{
		EXCCHECK( ProcessEventJigItemInserted()); 
	}

Error:   
	fncUnlock();
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "EventFunctionPluginCallback"
static SElExceptionPtr EventFunctionPluginCallback( const char* eventName, void* pData)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL;
	const char*		uuid = NULL;
	int32_t			full;
	int32_t			closed;
	int32_t			finished;
	int32_t			erreur;
	int32_t			insert1, insert2,insert3;
	
	EXCCHECK( fncLock());

	if( pData)
		uuid = ((SElEventDataPtr)pData)->GetStringPtr(pData);

	if(0==strcmp(eventName, "EVNT_GUI_TEST"))
	{
	EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, EVNT_JIG_CYCLE_START, NULL));
	}
		
	if(0==strcmp(eventName, "EVNT_PLUGIN_RUN"))
	{
		LED_Operator(COLOR_ORANGE);
		if( uuid &&( 0==strcmp(uuid, PLUGIN_MAINTENANCE)
					 || 0==strcmp(uuid, PLUGIN_REPORT) 
					 || 0==strcmp(uuid, PLUGIN_STATISTIC)
					 || 0==strcmp(uuid, PLUGIN_PRODUCT) ))
		{
			EXCCHECKCVI( PostDeferredCall(PostDeferredHidePanel, NULL));
			gs_Automat.disabled = TRUE;
		}
	}

	else if(0==strcmp(eventName, "EVNT_GUI_SHIFT"))
	{
		gs_Automat.shift_id = strtol(uuid, NULL, 10);

		if(gs_Automat.shift_id!=0)
		{
			if (!gs_Automat.MasterPart)
			{
				PANEL_MSG( AUTOMAT_HELP1, _("MODE PRODUCTION")); 

				EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_1_PRESENCE_PRODUCT_STATION_10", &insert1));
				EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_2_PRESENCE_PRODUCT_STATION_10", &insert2));
				EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_3_PRESENCE_PRODUCT_STATION_10", &insert3));
				if (gs_Automat.EmtyTable == TRUE) //mode vidage 
					{
					if (insert1 && insert2 && insert3)
						PANEL_MSG( AUTOMAT_HELP2, _("Enlever le produit"));
					else if (!insert1 && !insert2 && !insert3)
						PANEL_MSG( AUTOMAT_HELP2, _("Appuyer sur Start Cycle"));
					}
		
				else //mode production normal
					{	
					if (!insert1 && !insert2 && !insert3)
						PANEL_MSG( AUTOMAT_HELP2, _("Insérer Un Produit"));
					else if (insert1)
						PANEL_MSG( AUTOMAT_HELP2, _("Appuyer sur Start Cycle"));
					}
			}
			else 
			PANEL_MSG( AUTOMAT_HELP1, "Mode Passage Piéces Types");

		}
	
	}
	
	else if(0==strcmp(eventName, "EVNT_GUI_END_VERSION"))
	{

		if (gs_Automat.EmtyTable==FALSE && gs_Automat.isTableEmpty == FALSE && gs_Automat.shift_id!=0)
		{
			if(gs_Automat.Mode!=AUTOMAT_QUALITY_MASTER)
			{
			PANEL_MSG( AUTOMAT_HELP1, _("MODE VIDAGE PLATEAU"));
			}
		
			/* Check if product in pallett */
				EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_1_PRESENCE_PRODUCT_STATION_10", &insert1));
				EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_2_PRESENCE_PRODUCT_STATION_10", &insert2));
				EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_3_PRESENCE_PRODUCT_STATION_10", &insert3));
			if (!insert1 && !insert2 && !insert3 )
			{	
			PANEL_MSG( AUTOMAT_HELP2, _("Appuyer sur Start Cycle"));
			}
			else 
			{
			PANEL_MSG( AUTOMAT_HELP2, _("Enlever le produit"));
			}
			
			gs_Automat.EmtyTable = TRUE;
			gs_Table_Rotation = 0;
		}
	}

	else if(0==strcmp(eventName, "EVNT_GUI_CHANGE_VERSION"))
	{
	  if (!gs_Automat.EMPTY_TABLE_active)	
		gs_Automat.isTableEmpty = TRUE;
			
	 if (gs_Automat.isTableEmpty == TRUE)  
		{
		EXCDISPLAY( DisplayVersionList());
		}
		
	 if(gs_Automat.isTableEmpty == FALSE && gs_Automat.EmtyTable == FALSE && gs_Automat.shift_id!=0)
		{
			//End Actual Version
			PANEL_MSG( AUTOMAT_HELP1, _("Changement de Référence (Vidage Plateau)")); 
			/* Check if product in pallett */
			    EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_1_PRESENCE_PRODUCT_STATION_10", &insert1));
				EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_2_PRESENCE_PRODUCT_STATION_10", &insert2));
				EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_3_PRESENCE_PRODUCT_STATION_10", &insert3));
			if (!insert1 && !insert2 && !insert3)
			{	
			PANEL_MSG( AUTOMAT_HELP2, _("Appuyer sur Start Cycle"));
			}
			else 
			{
			PANEL_MSG( AUTOMAT_HELP2, _("Enlever le produit"));
			}
			
			gs_Automat.EmtyTable = TRUE;
			gs_Automat.ChangeVersion = TRUE;
			gs_Table_Rotation = 0;
		}

	}
	
	else if(0==strcmp(eventName, "EVNT_TEST_SEQUENCE_MASTER"))
	{
#if 0    /* formerly excluded lines */
							
		PANEL_MSG( AUTOMAT_HELP1, _("Mode Passage Piéces Types")); 

		EXCDISPLAY( ProcessEventMasterSequenceRun());

								
		 if (gs_Automat.shift_id!=0 && gs_Automat.master_active && gs_Automat.Mode != AUTOMAT_MASTERPART)
		 {
			 if (gs_Automat.isTableEmpty == TRUE) /* Table is free  */  
				{
						//Start Master Part
						PANEL_MSG( AUTOMAT_HELP1, _("Mode Passage Piéces Types")); 
						
						EXCDISPLAY( ProcessEventMasterSequenceRun());
				}
		
			 else 
				 {
					 if(gs_Automat.EmtyTable == FALSE)/* not yet started free Table */
					 {  		
						 //End Actual Version
						 PANEL_MSG( AUTOMAT_HELP1, _("Mode Passage Piéces Types"));

						 /* Check if product in pallett */
						 EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_1_PRESENCE_PRODUCT_STATION_10", &insert1));
						 EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_2_PRESENCE_PRODUCT_STATION_10", &insert2));
						 EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_3_PRESENCE_PRODUCT_STATION_10", &insert3));
						 if(!insert1 && !insert2 && !insert3)
							 PANEL_MSG( AUTOMAT_HELP2, _("Appuyer sur Start Cycle"));
						 else
							 PANEL_MSG( AUTOMAT_HELP2, _("Enlever le produit"));

						 gs_Table_Rotation = 0;
						 gs_Automat.EmtyTable = TRUE;
						 gs_Automat.MasterPart = TRUE;
					 }
					else   /* started but not free Table */
					{  		
						//End Actual Version
						PANEL_MSG( AUTOMAT_HELP1, _("Mode Passage Piéces Types"));

						/* Check if product in pallett */
						EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_1_PRESENCE_PRODUCT_STATION_10", &insert1));
						EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_2_PRESENCE_PRODUCT_STATION_10", &insert2));
						EXCCHECK( gs_pTester->GetSensor(gs_pTester, "SENSOR_3_PRESENCE_PRODUCT_STATION_10", &insert3));
						if (!insert1 && !insert2 && !insert3)
							PANEL_MSG( AUTOMAT_HELP2, _("Appuyer sur Start Cycle"));
						else
							PANEL_MSG( AUTOMAT_HELP2, _("Enlever le produit"));

						gs_Automat.MasterPart = TRUE;
					}
			
				 }
	
		 }
#endif   /* formerly excluded lines */
	}
		
	else if(0==strcmp(eventName, "EVNT_PAUSE_STARTED"))
	{
		EXCCHECKCVI( PostDeferredCall(PostDeferredHidePanel, NULL));
		gs_Automat.disabled = TRUE;

		gs_Automat.pause = TRUE;

		EXCCHECK( ProcessEventGuiProductSelected(TRUE, FALSE, FALSE));
		if( gs_EventFactory)
		{
			EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_PLUGIN_LEAVE", NULL));
		}
	}

	else if(0==strcmp(eventName, "EVNT_PAUSE_STOPED"))
	{
		gs_Automat.pause = FALSE;

		EXCCHECK( ProcessEventGuiProductSelected(TRUE, FALSE, FALSE));
		if( gs_EventFactory)
		{
			EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_PLUGIN_LEAVE", NULL));
		}

		EXCCHECKCVI( PostDeferredCall(PostDeferredDisplayPanel, NULL));
		gs_Automat.disabled = FALSE;
	}

	else if(0==strcmp(eventName, "EVNT_GUI_PRODUCTION")
		  || 0==strcmp(eventName, "EVNT_GUI_PRODUCT_SELECTED"))
	{
		LED_OPERATOR(COLOR_GREEN);
		if(!gs_Automat.pause)
		{
			EXCCHECKCVI( PostDeferredCall(PostDeferredDisplayPanel, NULL));
			gs_Automat.disabled = FALSE;
		}
		else
		{
			EXCCHECKCVI( PostDeferredCall(PostDeferredHidePanel, NULL));
			gs_Automat.disabled = TRUE;
		}
	}

	else if( 0==strcmp(eventName, "EVNT_GUI_EXPERTISE_STEP_MODE")
		  || 0==strcmp(eventName, "EVNT_GUI_EXPERTISE_GO_MODE")
		  || 0==strcmp(eventName, "EVNT_GUI_EXPERTISE_RNR")
		  || 0==strcmp(eventName, "EVNT_GUI_EXPERTISE_SEQUENCE_CREATED")
		  || 0==strcmp(eventName, "EVNT_GUI_QUALITY_MASTER")
		  || 0==strcmp(eventName, "EVNT_GUI_QUALITY_RETEST"))
	{
		EXCCHECKCVI( PostDeferredCall(PostDeferredDisplayPanel, NULL));
		gs_Automat.disabled = FALSE;
		
		if (0==strcmp(eventName, "EVNT_GUI_EXPERTISE_SEQUENCE_CREATED")) 
		{
			/* Get data [Station+Repeat] for R&R  */ 
			const char* buffer = ((SElEventDataPtr)pData)->GetStringPtr(pData);   

			char	data[10];
			char*	pdata = NULL;
			char*	pdata2 = NULL;
			

			strcpy_s(data, sizeof(data), buffer);
			
			pdata = data;				/* tester mode */
			pdata2 = strchr(pdata, ';');
			if (pdata2 != NULL)			/* xml path */
			{
				*pdata2 = '\0';
				pdata2++;
			}
			
			gs_RnR_Station = strtol(pdata, NULL, 10);
			gs_RnR_Repeat = strtol(pdata2, NULL, 10);
			
		}

	}
	
Error:   
	fncUnlock();
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
														 "EVNT_TESTER_ROTATION_FINISHED",
														 EventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_JIG_LOADED",
														 EventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_JIG_UNLOADED",
														 EventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_JIG_ITEM_RELEASED",
														 EventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_JIG_ITEM_INSERTED",
														 EventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_PLUGIN_RUN",
														 EventFunctionPluginCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_GUI_SHIFT",
														 EventFunctionPluginCallback));	
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_GUI_TEST",
														 EventFunctionPluginCallback));	
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_PAUSE_STARTED",
														 EventFunctionPluginCallback));	
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_PAUSE_STOPED",
														 EventFunctionPluginCallback));	
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_GUI_PRODUCTION",
														 EventFunctionPluginCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_GUI_EXPERTISE_STEP_MODE",
														 EventFunctionPluginCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_GUI_EXPERTISE_GO_MODE",
														 EventFunctionPluginCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_GUI_EXPERTISE_SEQUENCE_CREATED",
														 EventFunctionPluginCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_GUI_EXPERTISE_RNR",
														 EventFunctionPluginCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_GUI_QUALITY_MASTER",
														 EventFunctionPluginCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_GUI_QUALITY_RETEST",
														 EventFunctionPluginCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_GUI_PRODUCT_SELECTED",
														 EventFunctionPluginCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_BOX_CHANGE",
														 EventFunctionPluginCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_BOX_INSERTED",
														 EventFunctionPluginCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_RED_BOX_CLOSED",
														 EventFunctionPluginCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_RED_BOX_EMPTY",
														 EventFunctionPluginCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_PRINTER_READY",
														 EventFunctionPluginCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_TESTER_BUTTON_PRESSED_LABEL",
														 EventFunctionPluginCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_GUI_END_VERSION",
														 EventFunctionPluginCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_GUI_CHANGE_VERSION",
														 EventFunctionPluginCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_TEST_SEQUENCE_MASTER",
														 EventFunctionPluginCallback));	
		
	}

Error:
	EXCRETHROW(pexception);
}	/* RegisterEventFunction */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LoadConfiguration"
static SElExceptionPtr LoadConfiguration(
   const char*   fileName
)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	HRESULT         hr = 0;
	CAObjHandle     pxml  = 0;
	CAObjHandle     pnode = 0;
	CAObjHandle     pparameter = 0;   
	const char*     pfile_name = (fileName) ? fileName : "\\automat.xml";
	VBOOL         	is_xml_success = 1;
	VARIANT         pfile_name_var = CA_VariantEmpty();
	char*         	ptext= NULL;
	char			path[MAX_PATHNAME_LEN ];  

	EXCCHECKCVI( GetModuleDir (__CVIUserHInst, path));  
	strcat(path, pfile_name);
	
	EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml));
	EXCCHECKCVI( CA_VariantSetCString(  &pfile_name_var, path));
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (pxml, NULL, pfile_name_var, &is_xml_success));
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//plugin[@id='"PLUGIN_AUTOMAT"']", &pnode));   
	
	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "settings/master_panel", &pparameter));	
	if(pparameter!=0)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
		if(ptext)
		{
			if(0==strcmp(ptext, "TRUE"))
				gs_Automat.master_active = TRUE;

			CA_FreeMemory(ptext);
			ptext=NULL;
		}

		EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
		pparameter = 0;
	}

	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "settings/retest_quality", &pparameter));	
	if(pparameter!=0)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
		if(ptext)
		{
			if(0==strcmp(ptext, "TRUE"))
				gs_Automat.retest_active = TRUE;

			CA_FreeMemory(ptext);
			ptext=NULL;
		}

		EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
		pparameter = 0;
	}

	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "settings/box", &pparameter));	
	if(pparameter!=0)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
		if(ptext)
		{
			if(0==strcmp(ptext, "TRUE"))
				gs_Automat.box_active = TRUE;

			CA_FreeMemory(ptext);
			ptext=NULL;
		}

		EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
		pparameter = 0;
	}
	
    EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "settings/CIM", &pparameter));	
	if(pparameter!=0)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
		if(ptext)
		{
			if(0==strcmp(ptext, "TRUE"))
				gs_Automat.CIM_PLUS_active = TRUE;

			CA_FreeMemory(ptext);
			ptext=NULL;
		}

		EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
		pparameter = 0;
	}

	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "settings/LASER", &pparameter));	
	if(pparameter!=0)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
		if(ptext)
		{
			if(0==strcmp(ptext, "TRUE"))
				gs_Automat.LASER_active = TRUE;

			CA_FreeMemory(ptext);
			ptext=NULL;
		}

		EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
		pparameter = 0;
	}
	
	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "settings/PRINTER", &pparameter));	
	if(pparameter!=0)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
		if(ptext)
		{
			if(0==strcmp(ptext, "TRUE"))
				gs_Automat.PRINTER_active = TRUE;

			CA_FreeMemory(ptext);
			ptext=NULL;
		}

		EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
		pparameter = 0;
	}
	
	
    EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "settings/LAST_MVT", &pparameter));	
	if(pparameter!=0)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
		if(ptext)
		{
			sprintf(gs_Automat.Last_Mvt, ptext);
			CA_FreeMemory(ptext);
			ptext=NULL;
		}

		EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
		pparameter = 0;
	}
	
	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "settings/Changer_Ref_Avec_Vidage", &pparameter));	
	if(pparameter!=0)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
		if(ptext)
		{
			if(0==strcmp(ptext, "TRUE"))
				gs_Automat.EMPTY_TABLE_active = TRUE;
			 else 
				gs_Automat.EMPTY_TABLE_active = FALSE;
			
			CA_FreeMemory(ptext);
			ptext=NULL;
		}

		EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
		pparameter = 0;
	}

	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "settings/panel_release", &pparameter));	
	if(pparameter!=0)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
		if(ptext)
		{
			if(0==strcmp(ptext, "QUALITY"))
				gs_Automat.release_mode = E_PRM_QUALITY;
			else if(0==strcmp(ptext, "NOK"))
				gs_Automat.release_mode = E_PRM_NOK;
			else
				gs_Automat.release_mode = E_PRM_FALSE;

			CA_FreeMemory(ptext);
			ptext=NULL;
		}

		EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
		pparameter = 0;
	}

	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "settings/printer_label", &pparameter));	
	if(pparameter!=0)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
		if(ptext)
		{
			if(0==strcmp(ptext, "TRUE"))
				gs_Automat.label_active = TRUE;

			CA_FreeMemory(ptext);
			ptext=NULL;
		}

		EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
		pparameter = 0;
	}

Error:
	CA_VariantClear(&pfile_name_var);
	if (pnode) CA_DiscardObjHandle(pnode);
	if (pxml) CA_DiscardObjHandle(pxml);
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
			//_CrtSetBreakAlloc(77);
#endif
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
#define __FUNC__ "{Automat Plugin}::Initialize"
PLUGIN_API SElExceptionPtr PLUGIN_INIT_NAME_CODE( SPluginArgsPtr a_pPluginArgs )
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	const char*		pconfiguration_file = NULL;
	int32_t			main_panel = 0;
	int				monitor_id, top, height;  

	InitializeCriticalSection( &_Lock);

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
	
	EXCCHECKCVI( gs_panel[AUTOMAT] = LoadPanelEx (main_panel, "automat.uir", AUTOMAT, __CVIUserHInst));

	EXCCHECK( a_pPluginArgs->SetArg(a_pPluginArgs, "AutomatPanel", ARG_INT32, &gs_panel[PANEL]));  

	EXCCHECKCVI( GetPanelAttribute( main_panel, ATTR_TOP, &top));
    EXCCHECKCVI( GetPanelAttribute( main_panel, ATTR_HEIGHT, &height));
	
	EXCCHECKCVI( SetPanelAttribute (gs_panel[AUTOMAT], ATTR_TOP, 0));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[AUTOMAT], ATTR_LEFT, 0));

	
	EXCCHECKCVI(SetPanelAttribute (gs_panel[PANEL], ATTR_SCALE_CONTENTS_ON_RESIZE, TRUE));

	
	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
									 "EventFactory",
									 sizeof(SElEventFactoryPtr),
									 &gs_EventFactory));
	
	if(gs_Modules)
	{
		gs_pLabels = (SLabelListPtr)gs_Modules->Get(gs_Modules, MODULE_LABELS);
		gs_pDbs = (SDBSPtr)gs_Modules->Get(gs_Modules, MODULE_DBS);
		gs_pBox = (SBoxPtr)gs_Modules->Get(gs_Modules, MODULE_BOX);
		gs_pTester = (STesterPtr)gs_Modules->Get(gs_Modules, MODULE_TESTER);
		gs_pPrinters = gs_Modules->Get(gs_Modules, MODULE_PRINTERS);
		gs_paxiss = gs_Modules->Get(gs_Modules, MODULE_AXIS);
		gs_pcameras = gs_Modules->Get(gs_Modules, MODULE_CAMERA);
		gs_pTracas = gs_Modules->Get(gs_Modules, MODULE_TRACAS);
	}

Error:	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::Run"
PLUGIN_API SElExceptionPtr PLUGIN_RUN_NAME_CODE( SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	int i;
	SPrinterPtr		printer;
	STracaPtr		ptraca = NULL;
	
	
	
	EXCCHECK( testseq_new(&gs_Automat.pTestSeq));
	EXCCHECK( a_pPluginArgs->SetArg(a_pPluginArgs, "TestSequence", ARG_PTR, gs_Automat.pTestSeq));  
	EXCCHECK( LoadConfiguration(NULL));

	EXCCHECK( GetEventHandle()); 
	EXCCHECK( RegisterEventFunction());

	EXCCHECKCVI( DisplayPanel(gs_panel[AUTOMAT]));

	gs_EventHandle[EVNT_STOP_THREAD] = CreateEvent( NULL, 1, 0, NULL);
	if (gs_EventHandle[EVNT_STOP_THREAD] == NULL)
		EXCTHROW(-1, "Create event failed");

	for(i=0; i< NB_OF_STATIONS ;i++)
	{
		StatusStations[i] = NOT_BUSY;
		TimeStation[i] = 0.0;
	}
	
	Autorisation_Thread = 1;
		
	gs_Product_Checked = FALSE;
	
	
	/* Read ID First Pallett */
	//GetPalettID(&gs_PalettID);
	
		/*--- Start Printer  ----*/
	if (gs_Automat.PRINTER_active)
	{
		gs_pPrinters->GetPrinterByIndex(gs_pPrinters, 0, &printer);
		if (printer)
		EXCCHECK(printer->Connect (printer));
	}
	/*--- Start alim 12 VDC  ----*///anwar
	EXCCHECK( Init_BK_PRECISION_WS40_WS50());
	EXCCHECK( Init_Alim_keysight());
	EXCCHECK( Init_PWM_Generator());
	/* Connecter à la base Traca */
	if (gs_pTracas)
	{
	EXCCHECK( gs_pTracas->GetTracaByIndex(gs_pTracas, 0, &ptraca));
	EXCCHECK(  ptraca->Connect(ptraca));	
	}
	
	/* Reset all actuators to don't move down */
	EXCCHECK(ResetStations());//anwar

	EXCCHECK( ThreadStart(Thread_Automat, NULL, &gs_hThread, &hEventStop)); 
	
	//EXCCHECK( ThreadStart(Thread_Time, NULL, &gs_hThread2, &hEventStop2)); 
	
	gs_Automat.EmtyTable = FALSE;
			
	if(gs_Automat.master_active)
	{
		gs_Automat.MasterPart = TRUE;
		
		EXCDISPLAY( ProcessEventMasterSequenceRun());
		
		PANEL_MSG( AUTOMAT_HELP1, "Mode Passage Piéces Types");
		ProcessDrawEvents ();
		ProcessSystemEvents ();
	}

	EXCCHECK( ProcessEventGuiProduction());	
	LED_Operator(COLOR_GREEN);

Error:	
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::Leave"
PLUGIN_API SElExceptionPtr PLUGIN_LEAVE_NAME_CODE( SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
 
					
#if 0    /* formerly excluded lines */
	PulseEvent(gs_EventHandle[EVNT_STOP_THREAD]); 
#endif   /* formerly excluded lines */
	
Error:	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::Shutdown"
PLUGIN_API SElExceptionPtr PLUGIN_SHUTDOWN_NAME_CODE( SPluginArgsPtr a_pPluginArgs)
{
   	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
   
	b_loop_stop2 = TRUE;
	Sleep(1100);
	EXCCHECK( ThreadStop( &gs_hThread, &gs_EventHandle[EVNT_STOP_THREAD]));
	EXCCHECK( gs_Automat.pTestSeq->UnLoad(gs_Automat.pTestSeq));  
	EXCCHECK( testseq_delete(&gs_Automat.pTestSeq)); 
	if(gs_panel[AUTOMAT])
		EXCCHECKCVI( DiscardPanel(gs_panel[AUTOMAT]));

	DeleteCriticalSection( &_Lock);

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::Info"
PLUGIN_API SElExceptionPtr PLUGIN_GETINFO_NAME_CODE( SPluginInfoPtr* a_pInfo)
{
	SElExceptionPtr	pexception = NULL;  
	
	if(a_pInfo) EXCCHECK( (*a_pInfo)->SetInfoData(*a_pInfo, &gs_InfoData));
	
Error:	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::Attribute"
PLUGIN_API SElExceptionPtr PLUGIN_GETATTR_NAME_CODE( uint32_t  attrID, uint32_t size, void* value)
{
	return NULL;
}

#endif 

////int Fill_DBO_Asteel_Mesures (const char* pfile_xml,char* nom_mesure, char* mesure_value,char* mesure_min,char* mesure_max,char* mesure_result)
////{

//#undef __FUNC__											   
//#define __FUNC__ "Fill_DBO_Asteel_Mesures"
//SElExceptionPtr Fill_DBO_Asteel_Mesures(struct _SSledge* me, int32_t* pError)  //karim 
//{
//	SElExceptionPtr	pexception = NULL;
//	int32_t			error = 0;

//	HRESULT         	hr = 0;
//	CAObjHandle     	pxml = 0; 
//	CAObjHandle     	pnode = 0;
//	VARIANT         	pfile_name_var = CA_VariantEmpty();
//	VBOOL            	is_xml_success = 1;
//	char         		path[256];
//	CAObjHandle     	pparameter = 0;
//	CAObjHandle     	pparameter_process_name = 0;
//	CAObjHandle     	pparameter_step_name = 0;
//	CAObjHandle     	pparameter_mesure_config = 0;
//	CAObjHandle     	pparameter_mesure_value = 0;
//	CAObjHandle     	pparameter_mesure_min = 0;
//	CAObjHandle     	pparameter_mesure_max = 0;
//	CAObjHandle     	pparameter_mesure_result = 0;

//	char*           	ptext = NULL;
//	char*           	Step_Description = NULL;
//	char				Serial[128];
//	int            		i = 0;
//	int            		j = 0;
//	int            		k = 0;
//	char 				sTrame[256];
//	char 				Message[256];
//	int                 Status;
//	int 				Failed  = 0;
//	int 				Id_Test = 0;


//	/* Read from XML test Steps */

//	//gs_pTracas->GetTracaByIndex(gs_pTracas, 0, &ptraca);

//	
//	MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml);
//	CA_VariantSetCString(&pfile_name_var, pfile_xml);
//	MSXML_IXMLDOMDocumentload (pxml, NULL, pfile_name_var, &is_xml_success);
//	MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//testseq", &pnode);		
//	
//	
//	/* barcode */
//	MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "product//barcode", &pparameter);
//	if(pparameter)
//		{
//			MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext);
//			if(ptext && *ptext)
//			{
//			sprintf(Serial, ptext);
//			CA_FreeMemory(ptext);
//			ptext=NULL;
//			}
//						 
//		CA_DiscardObjHandle(pparameter);
//		pparameter = 0; 
//		}
//	    /*** Extract IDtest from DBO Test ***/
//		 EXCCHECK( ptraca->Get_Id_Test_Produit(ptraca,Serial,"",&Id_Test));	
//	
//	
//	do
//	{
//		sprintf(path, "process[@stage='%d']/name", ++i); 
//		MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter_process_name);
//		if(pparameter_process_name)//process
//			{			
//				do
//				{
//					sprintf(path, "process[@stage='%d']/test[@step='%d']/name", i, ++j); 
//					MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter_step_name);
//					if(pparameter_step_name)//step
//					{
//						do
//						{   


//							sprintf(path, "process[@stage='%d']/test[@step='%d']/measure[@step='%d']/config", i, j, ++k);
//							MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter_mesure_config);
//							
//							sprintf(path, "process[@stage='%d']/test[@step='%d']/measure[@step='%d']/value", i, j, k);
//							MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter_mesure_value);
//							
//							sprintf(path, "process[@stage='%d']/test[@step='%d']/measure[@step='%d']/limit/value1", i, j, k);
//							MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter_mesure_min);
//							
//							sprintf(path, "process[@stage='%d']/test[@step='%d']/measure[@step='%d']/limit/value2", i, j, k);
//							MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter_mesure_max);
//							
//							sprintf(path, "process[@stage='%d']/test[@step='%d']/measure[@step='%d']/failed", i, j, k);
//							MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter_mesure_result);
//							
//							if (pparameter_mesure_config)
//							{



//								MSXML_IXMLDOMNodeGettext (pparameter_mesure_config, NULL, &ptext);

//								if(ptext && *ptext)   //nom mesure
//								{
//									strcpy(nom_mesure, ptext);
//									CA_FreeMemory(ptext);
//									ptext=NULL;
//								}

//								
//								MSXML_IXMLDOMNodeGettext (pparameter_mesure_value, NULL, &ptext);
//								if(ptext && *ptext)
//								{
//									
//									strcpy(mesure_value, ptext);
//									CA_FreeMemory(ptext);
//									ptext=NULL;
//								}
//								
//								MSXML_IXMLDOMNodeGettext (pparameter_mesure_min, NULL, &ptext);
//								if(ptext && *ptext)
//								{
//									strcpy(mesure_min, ptext);
//									CA_FreeMemory(ptext);
//									ptext=NULL;
//								}
//								
//								MSXML_IXMLDOMNodeGettext (pparameter_mesure_max, NULL, &ptext);
//								if(ptext && *ptext)
//								{
//									strcpy(mesure_max, ptext);
//									CA_FreeMemory(ptext);
//									ptext=NULL;
//								}
//								
//								MSXML_IXMLDOMNodeGettext (pparameter_mesure_result, NULL, &ptext);
//								if(ptext && *ptext)
//								{
//									strcpy(mesure_result, ptext);
//									if(0 == atoi(ptext))
//									{
//										Failed = 0;
//									}
//									else
//									{
//										Failed = 1;
//										EXCCHECK( ptraca->Set_Measurement(ptraca,Id_Test,nom_mesure,atof(mesure_min),atof(mesure_max),atof(mesure_value),atoi(mesure_result)));
//										break;
//									}
//									CA_FreeMemory(ptext);
//									ptext=NULL;
//								}
//								
//								CA_DiscardObjHandle(pparameter_mesure_config);
//								CA_DiscardObjHandle(pparameter_mesure_value);
//								CA_DiscardObjHandle(pparameter_mesure_min);
//								CA_DiscardObjHandle(pparameter_mesure_max);
//								CA_DiscardObjHandle(pparameter_mesure_result);
//								//------------------------------------------
//								/*** Extract IDtest from DBO Test ***/
//		                       // EXCCHECK( ptraca->Get_Id_Test_Produit(ptraca,Serial,"",&Id_Test));
//								/***Insert Mesure to DBO MESURE ***/

//								EXCCHECK( ptraca->Set_Measurement(ptraca,Id_Test,nom_mesure,atof(mesure_min),atof(mesure_max),atof(mesure_value),atoi(mesure_result)));
//								//-----------------------------------------
//								pparameter_mesure_config = 0;
//								pparameter_mesure_value=0;
//								pparameter_mesure_min = 0;
//								pparameter_mesure_max=0;
//								pparameter_mesure_result=0;
//							}
//							else//mesure
//							{
//								k=0;
//								break;
//							}

//						}
//						while(TRUE);

//						CA_DiscardObjHandle(pparameter_step_name);
//						CA_DiscardObjHandle(pparameter_mesure_config);
//						CA_DiscardObjHandle(pparameter_mesure_value);
//						CA_DiscardObjHandle(pparameter_mesure_min);
//						CA_DiscardObjHandle(pparameter_mesure_max);
//						CA_DiscardObjHandle(pparameter_mesure_result);
//						pparameter_step_name = 0;
//						if(Failed)
//							break;
//					}
//					else//step
//					{
//						j=0;
//						break;
//					}
//		
//				}while(TRUE);
//				
//				CA_DiscardObjHandle(pparameter_process_name);
//				pparameter_process_name = 0;
//				if(Failed)
//						break;
//			}
//		else
//		//	{

//				break;
//		//	}
//	
//	}while(TRUE);
//	

//	if (ptext) CA_FreeMemory(ptext);
//	if (pparameter_process_name) CA_DiscardObjHandle(pparameter_process_name);
//	if (pparameter_step_name) CA_DiscardObjHandle(pparameter_step_name);
//	if (pparameter_mesure_config) CA_DiscardObjHandle(pparameter_mesure_config);
//	if (pparameter_mesure_value) CA_DiscardObjHandle(pparameter_mesure_value);
//	if (pparameter_mesure_min) CA_DiscardObjHandle(pparameter_mesure_min);
//	if (pparameter_mesure_max) CA_DiscardObjHandle(pparameter_mesure_max);
//	if (pparameter_mesure_result) CA_DiscardObjHandle(pparameter_mesure_result);


//	if (pnode) CA_DiscardObjHandle(pnode);
//	if (pxml) CA_DiscardObjHandle(pxml);
//	
//	Error:  
//	EXCRETHROW( pexception);  
//}


