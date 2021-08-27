#include <ansi_c.h>
#include "EA210_utils.h"
#include "toolbox.h"
#include <formatio.h>
#include <multithread/multithread.h>
#include "bp98xx.h"
#include "hpe364xa.h"
#include "AgE36xx.h"
#include <advanlys.h>
#include "Ag34450.h"
#include "test_panel.h"
#include "EA210_Test_WS30.h"

double time_station_30;

ViInt32   ErrorDMM,
		  ErrorE3642A,
		  ErrorBK;
ViRsrc    BKresourceName       = "ASRL1::INSTR";
ViRsrc    E3642AresourceName   = "ASRL17::INSTR";
ViRsrc    AG34450AresourceName = "USB0::0x2A8D::0xB318::MY59320036::INSTR";
ViChar    channel[50];
ViSession vi_DMM 		   = 0;
ViSession vi_E3642A        = 0;
ViSession vi_BK;
ViReal64  time_ms   	   = 5000;   

#define UDA_NAME_STATION_30 	"UDA (USB-SN: UDA-03)"


DEFINE_GLOBAL_VARIABLE();

/* Connexion Verin Horizontale*/

#define TESTER_PLUG_PANEL_STATION_30(timeout) \
	EXCCHECK( ptester->PlugPanelStation30(ptester, timeout))

#define TESTER_UNPLUG_PANEL_STATION_30(timeout) \
	EXCCHECK( ptester->UnplugPanelStation30(ptester, timeout))

/* Connexion Verin Verticale */

#define TESTER_VERTICAL_PLUG_PANEL_STATION_30(timeout) \
	EXCCHECK( ptester->VerticalPlugStation30(ptester, timeout))

#define TESTER_VERTICAL_UNPLUG_PANEL_STATION_30(timeout) \
	EXCCHECK( ptester->VerticalUnplugStation30(ptester, timeout))

#define TESTER_DISCONNECT_ALL_STATION_30(timeout) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_FTA1",              FALSE)) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_FTA2",              FALSE)) \ 
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_FTA3",              FALSE)) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_FTA4",              FALSE)) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_DBUS_Relay_3F", 			 FALSE)) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3G", 				 FALSE)) \ 
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3H", 				 FALSE)) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3I", 				 FALSE)) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3L", 				 FALSE)) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3M", 				 FALSE)) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3N", 				 FALSE)) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3O", 				 FALSE)) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3P", 				 FALSE)) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3Q_3Q2",			 FALSE)) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_AC_PowerSupply_Relay_3R", FALSE)) 


#define CONNECT_UDA_RELAY(timeout) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_DBUS_Relay_3F", TRUE))
#define DISCONNECT_UDA_RELAY(timeout) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_DBUS_Relay_3F", FALSE))


STestListItem gs_TestList_Station_30[] = {

	/* STATION 30 */
	ADD_TI( "30.0.0",  EA210, 30_0_0),		/* TESTER : Disconnect Product */
	ADD_TI( "30.0.1",  EA210, 30_0_1),		/* TESTER : Disconnect Product */
	ADD_TI( "30.0.2",  EA210, 30_0_2),		/* TESTER : HW TEST MODE DBUS */

		
	ADD_TI( "30.1.1",  EA210, 30_1_1),		/* TEST : OFFSET Error Correction */
	ADD_TI( "30.1.2",  EA210, 30_1_2),		/* TEST : Get offset correction value */
	
	ADD_TI( "30.2.1",  EA210, 30_2_1),		/* TEST : Switch on supply voltage */
	ADD_TI( "30.2.2",  EA210, 30_2_2),		/* TEST : Check current consumption SMPS 90 VAC */
	ADD_TI( "30.2.3",  EA210, 30_2_3),		/* TEST : Check current consumption SMPS 260 VAC */
	ADD_TI( "30.2.4",  EA210, 30_2_4),		/* TEST : Measurement of output voltage 12VDC with simulated loads */
	ADD_TI( "30.2.5",  EA210, 30_2_5),		/* TEST : Measurement of output voltage 5VDC */
	
	ADD_TI( "30.3.1",  EA210, 30_3_1),		/* TEST : Change HW Test Mode */
	ADD_TI( "30.3.2",  EA210, 30_3_2),		/* TEST : Check SM status */
	ADD_TI( "30.3.3",  EA210, 30_3_3),		/* TEST : Check tracing ID of SMB */
	ADD_TI( "30.3.4",  EA210, 30_3_4),		/* TEST : Standby Mode Activation of SMB */
	
	ADD_TI( "30.4.1",  EA210, 30_4_1),		/* TEST : Calibrate line voltage measurement 130VDC */
	ADD_TI( "30.4.2",  EA210, 30_4_2),		/* TEST : Check correction factor */
	ADD_TI( "30.4.3",  EA210, 30_4_3),		/* TEST : Calibrate line voltage measurement 170VDC */
	ADD_TI( "30.4.4",  EA210, 30_4_2),		/* TEST : Check correction factor */	
	ADD_TI( "30.4.5",  EA210, 30_4_5),		/* TEST : Calibrate line voltage measurement 230VDC */	
	ADD_TI( "30.4.6",  EA210, 30_4_2),		/* TEST : Check correction factor */	
	ADD_TI( "30.4.7",  EA210, 30_4_7),		/* TEST : Check corrected line voltage */	
	ADD_TI( "30.4.8",  EA210, 30_4_1),		/* TEST : Calibrate line voltage measurement 130VDC */	
	ADD_TI( "30.4.9",  EA210, 30_4_2),		/* TEST : Check correction factor */	
	ADD_TI( "30.4.10", EA210, 30_4_3),		/* TEST : Calibrate line voltage measurement 170VDC */	
	ADD_TI( "30.4.11", EA210, 30_4_2),		/* TEST : Check correction factor */	
	ADD_TI( "30.4.12", EA210, 30_4_5),		/* TEST : Calibrate line voltage measurement 230VDC */	
	ADD_TI( "30.4.13", EA210, 30_4_2),		/* TEST : Check correction factor */	
	ADD_TI( "30.4.14", EA210, 30_4_7),		/* TEST : Check corrected line voltage */
	
	ADD_TI( "30.5.1",  EA210, 30_5_1),		/* TEST : Check external temperature sensor inputs */	
	
	ADD_TI( "30.6.1",  EA210, 30_6_1),		/* TEST : Check digital input X10.2 */	
	ADD_TI( "30.6.2",  EA210, 30_6_2),		/* TEST : Check digital input X10.3 */	
	ADD_TI( "30.6.3",  EA210, 30_6_3),		/* TEST : Check magnetic door open sensor */
	ADD_TI( "30.6.4",  EA210, 30_6_4),		/* TEST : Check magnetic door closed sensor */	
	ADD_TI( "30.6.5",  EA210, 30_6_5),		/* TEST : Test of PWM Feedback Input X8.4 */	
	ADD_TI( "30.6.6",  EA210, 30_6_6),		/* TEST : Test of PWM Feedback Input X8.7*/
	ADD_TI( "30.6.7",  EA210, 30_6_7),		/* TEST : Test of PWM Feedback Input X9.4*/
	
	ADD_TI_LAST_ENTRY
};
//-------------------------------------------------------------------------------
int CVICALLBACK OnTimerWS30 (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{	
	char            stime[32];
	
	switch (event)
	{
		case EVENT_TIMER_TICK:
			
			time_station_30 = time_station_30 + 0.1;
			
			sprintf (stime, "(%0.1fs)", time_station_30);
			SetCtrlVal( gs_TestPanel, PANEL_TIME_WS30, stime); 

			break;
	}
	return 0;
}
ViSession vi_E3642A;

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::Init_Alim_keysight"
SElExceptionPtr Init_Alim_keysight (void)
{

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
	
		//Sleep(200);
		ErrorE3642A = hpe364xa_ConfigureCurrentLimit  (vi_E3642A, "1", HPE364XA_VAL_CURRENT_REGULATE, 0.6);
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

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::Close_Alim_keysight"
SElExceptionPtr Close_Alim_keysight (void)
{

SElExceptionPtr	pexception = NULL;
ViInt32   ErrorE3642A,
		  ErrorCode;;
ViChar    ErrorMessage[256];
char      msg[512];

  		// Initiate the keysight and create a session ---------------------------//
		ErrorE3642A = hpe364xa_close(vi_E3642A);
		if (ErrorE3642A < VI_SUCCESS)
		{
			
			hpe364xa_GetError (vi_E3642A, &ErrorCode, 255, ErrorMessage);
			sprintf(msg, "Function Error :hpe364xa_Close \r\n Error: %d, message error : %s\n", ErrorCode, ErrorMessage);
			EXCTHROW( -1, msg);
		}
Error:
	EXCRETHROW( pexception);
}
/*********************************************************************************
 * Function Testsequence Init
 *********************************************************************************/
FCE_TESTSEQUENCE_INIT(STATION_30)
{
	TESTSEQUENCEINIT_BEGIN();
	PANEL_INI();
	time_station_30 = 0.0;
	TEST_USE(TESTER);
	TESTER_DISCONNECT_ALL_STATION_30();
	SetCtrlAttribute (gs_TestPanel, PANEL_TIMER_WS30,ATTR_ENABLED, TRUE);
	
Error:
	if (pexception) 	SetCtrlAttribute (gs_TestPanel, PANEL_TIMER_WS30,ATTR_ENABLED, FALSE);
	TESTSEQUENCEINIT_END();
}

/*********************************************************************************
 * Function Testsequence Cleanup
 *********************************************************************************/
FCE_TESTSEQUENCE_CLEANUP(STATION_30)
{
	bp98xx_Close(vi_BK);
	Close_Alim_keysight();
	TESTSEQUENCECLEANUP_BEGIN(); 
	SetCtrlAttribute (gs_TestPanel, PANEL_TIMER_WS30, ATTR_ENABLED, FALSE);
	TEST_USE(TESTER);
	TESTER_DISCONNECT_ALL_STATION_30();
	TESTER_VERTICAL_UNPLUG_PANEL_STATION_30(3000);
	TESTER_UNPLUG_PANEL_STATION_30(5000);//TIMEOUT_DEFAULT
	//TESTER_INIT ();
	DISPLAY_TESTSEQ_RESULT ();

	
Error:
	TESTSEQUENCECLEANUP_END();
}

/*********************************************************************************
 * Function Testsequence Wait
 *********************************************************************************/
FCE_TESTSEQUENCE_WAIT(STATION_30)
{
	TESTSEQUENCEWAIT_BEGIN(); 
	TEST_USE(TESTER);

	WAIT_TABLE_ROTATION_FINISHED(TIMEOUT_TABLE);
	
Error:
	TESTSEQUENCEWAIT_END();
}


/*********************************************************************************
 * Function 30_0_1
*********************************************************************************/

FCE_HELP(EA210, 30_0_0)
{
    HELP_BEGIN();
	HELP_TEST( " TESTER : connect Product & init instruments");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 30_0_0)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE( TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 30_0_0)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 30_0_0)
{
	TEST_BEGIN();
	TEST_USE(TESTER);
	char sys_error[128];
	char   msg[256];
	ViInt32 ErrorCode;
	ViChar ErrorMessage[50];
	
	pexception = ptester->PlugPanelStation30(ptester, 5000);
	if (pexception)
	   TESTER_PLUG_PANEL_STATION_30(5000);
	
	EXCCHECK( Init_Alim_keysight());
	CONNECT_UDA_RELAY();
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3M", TRUE));       //step 6.5
	/*--Initiate the keysight and create a session */
	ErrorDMM = Ag34450_init (AG34450AresourceName, VI_FALSE, VI_FALSE, &vi_DMM);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34450_GetError(vi_DMM, &ErrorCode, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34450_init \r\n Error: %d, message error : %s\n", ErrorCode, ErrorMessage);
		EXCTHROW( -1, msg);
	}



Error:
	TEST_END();
}

FCE_CLEANUP(EA210, 30_0_0)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 30_0_1
*********************************************************************************/

FCE_HELP(EA210, 30_0_1)
{
    HELP_BEGIN();
	HELP_TEST( " TESTER : TESTER_UNPLUG_PANEL_STATION_30 ");

Error:
    HELP_END();
}

FCE_CHECK(EA210, 30_0_1)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 30_0_1)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 30_0_1)
{
    TEST_BEGIN();
	TEST_USE(TESTER);
    TESTER_VERTICAL_UNPLUG_PANEL_STATION_30(3000);
	TESTER_UNPLUG_PANEL_STATION_30(5000);//TIMEOUT_DEFAULT

Error:
    TEST_END();
}

FCE_CLEANUP(EA210, 30_0_1)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 30_0_2
*********************************************************************************/

FCE_HELP(EA210, 30_0_2)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : DBUS ENTER HARDWARE TEST MODE");

Error:
    HELP_END();
}

FCE_CHECK(EA210, 30_0_2)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE( TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 30_0_2)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 30_0_2)
{
	TEST_BEGIN();
	TEST_USE(TESTER);
	TEST_USE(DBUS);
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	int cnt=0;
	int MessageID_OK = 0;
	//Start HW test Mode
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_30));

	do
	{
		cnt++;
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_30, 0x1B, "7000", "23C0");
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_30, 0xC0, MessageID, MessageData);

		if (!strcmp(MessageID, "7100"))
			MessageID_OK = 1;

		if (MessageID_OK)
			break;

	}
	while (cnt<40);


	if (strcmp(MessageID, "7100"))
	{
		LIMIT_CREATE("7100", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}


Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_30));
	TEST_END();
}

FCE_CLEANUP(EA210, 30_0_2)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 30_1_1
*********************************************************************************/

FCE_HELP(EA210, 30_1_1)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : OFFSET Error Correction");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 30_1_1)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 30_1_1)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 30_1_1)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
    //12VDC ON ----
	ViInt32 index;
    ViReal64 voltage; 
	ViStatus ErrorCode;
	ViChar ErrorMessage[50];
	char   msg[256];
	
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_30));
	// Set offset correction value	
	cnt = 0;
	do{
		cnt++;
		
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_30, 0x1B, "7071", "");
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_30, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7171"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt==40) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7171"))
	{
		LIMIT_CREATE("7171", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}

Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_30));
    TEST_END();
}

FCE_CLEANUP(EA210, 30_1_1)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 30_1_2
*********************************************************************************/

FCE_HELP(EA210, 30_1_2)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Get offset correction value");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 30_1_2)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 30_1_2)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 30_1_2)
{   TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	STRING(byte1, 4+1);	
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	
	
	//Get offset correction value
	
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_30));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_30, 0x1B, "7025", "00");//step 6.5
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_30));
	Sleep(500);			
	do{
		cnt++;
		
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_30, 0x1B, "7072", "");
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_30, 0xC0, MessageID, MessageData);
		
		
		if ((!strcmp(MessageID, "7172")))
		{
			CopyString (byte,0,MessageData,4,2);
			sscanf(byte, "%02X", &valeur);
			if(valeur==1)
				MessageID_OK = 1;
		}
		if (MessageID_OK)
			break;
		
		if (cnt ==40) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7172"))
	{
		LIMIT_CREATE("7172", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	/* Offset Correction value	*/
	CopyString (byte1,0,MessageData,0,4);
	sscanf(byte1, "%04X", &valeur);
	//LIMIT_CHECK(INT32, valeur);	
	
	/* Check result of Set Line Offset Correction value */
	CopyString (byte,0,MessageData,4,2);
	sscanf(byte, "%02X", &valeur);
	LIMIT_CHECK(INT32, valeur);
	

	//KEYSIGHT : Power off electronic
	
	
	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_30));
    TEST_END();
}

FCE_CLEANUP(EA210, 30_1_2)
{
    TESTCLEANUP_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_DBUS_Relay_3F", FALSE));
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 30_2_1
*********************************************************************************/

FCE_HELP(EA210, 30_2_1)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Init VAC supply voltage ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 30_2_1)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 30_2_1)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 30_2_1)
{   TEST_BEGIN();
	TEST_USE(TESTER);

	
	ViReal64 ActualPower;
    ViReal64 ActualVoltage;
    ViReal64 ActualCurrent;
    ViReal64 Freq;
	ViInt32 Readsize;
	ViChar ErrorMessage[512];
	char   msg[512];

    EXCCHECK( ptester->SetSensor(ptester, "CMD_AC_PowerSupply_Relay_3R", TRUE));
	
	
	/*- Initiate the BK and create a session ---------------------------*/
	ErrorBK = bp98xx_Initialize (BKresourceName, VI_TRUE, VI_TRUE, BAUDRATE_VAL6_19200, &vi_BK);
	if (ErrorBK < VI_SUCCESS)
	{	Sleep(200);
		ErrorBK = bp98xx_Initialize (BKresourceName, VI_TRUE, VI_TRUE, BAUDRATE_VAL6_19200, &vi_BK);
		if (ErrorBK < VI_SUCCESS)
		{
			Sleep(200);
			ErrorBK = bp98xx_Initialize (BKresourceName, VI_TRUE, VI_TRUE, BAUDRATE_VAL6_19200, &vi_BK);
			if (ErrorBK < VI_SUCCESS)
			{
				bp98xx_GetLastErrorMsg (vi_BK, 255, ErrorMessage, &Readsize);
				sprintf(msg, "Function Error : bp98xx_Initialize \r\n  message error : %s\n", ErrorMessage);
				EXCTHROW( -1, msg);
			}
		}
	}
    ErrorBK = bp98xx_Reset (vi_BK);
	Sleep(50);
	ErrorBK = bp98xx_ConfigureCurrentProtectionLimits (vi_BK, 0.2, 1, CURRENTPROTECTIONDELAY_VAL0_DELAY, PEAKCURRENTPROTECTIONDELAY_VAL0_DELAY);
	if (ErrorBK < VI_SUCCESS)
	{
		bp98xx_GetLastErrorMsg (vi_BK, 255, ErrorMessage, &Readsize);
		sprintf(msg, "Function Error : bp98xx_ConfigureCurrentProtectionLimits \r\n  message error : %s\n", ErrorMessage);
		EXCTHROW( -1, msg);
	}
	
	ErrorBK = bp98xx_ConfigureOutput (vi_BK, 90.0, 50.0);
	if (ErrorBK < VI_SUCCESS)
	{				
		bp98xx_GetLastErrorMsg (vi_BK, 255, ErrorMessage, &Readsize);
		sprintf(msg, "Function Error : bp98xx_ConfigureOutput \r\n  message error : %s\n", ErrorMessage);
		EXCTHROW( -1, msg);
	}
	ErrorBK = bp98xx_EnableOutput (vi_BK, VI_TRUE);
	if (ErrorBK < VI_SUCCESS)
	{
		bp98xx_GetLastErrorMsg (vi_BK, 255, ErrorMessage, &Readsize);
		sprintf(msg, "Function Error : bp98xx_EnableOutput \r\n  message error : %s\n", ErrorMessage);
		EXCTHROW( -1, msg);
	}
	Sleep(1000);

	
Error:

    TEST_END();
}

FCE_CLEANUP(EA210, 30_2_1)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 30_2_2
*********************************************************************************/

FCE_HELP(EA210, 30_2_2)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Check current consumption SMPS 90 VAC");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 30_2_2)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 30_2_2)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 30_2_2)
{   TEST_BEGIN();
    TEST_USE(TESTER);
	
	ViReal64 ActualPower;
	ViReal64 ActualCurrent;
	ViReal64 ActualVoltage;
	ViReal64 Freq;
	ViInt32 Readsize;
	char   msg[256];
	ViChar ErrorMessage[50];
	int    cnt = 0;
	
	//mesure current seulement 90VAC 260VAC)
	do{
		Sleep (200);
		cnt++;
		_test_index = 1;
			
		EXCDELETE(pexception);
			
		bp98xx_MeasureOutput (vi_BK, &Freq, &ActualVoltage, &ActualCurrent, &ActualPower);
		ActualCurrent = ActualCurrent*1000;
		//printf ("30_2_2 : ActualCurrent = %f\r\n", ActualCurrent);
		LIMIT_CHECK_EXT(REAL64, ActualCurrent, pexception);
		
		if (pexception && cnt==20)
			{
				EXCCHECK(pexception);
			}
	//printf ("30_2_2 : cnt = %d\r\n", cnt);
	}while(pexception && cnt<20);

Error:

    TEST_END();
}

FCE_CLEANUP(EA210, 30_2_2)
{
    TESTCLEANUP_BEGIN();    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 30_2_3
*********************************************************************************/

FCE_HELP(EA210, 30_2_3)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Check current consumption SMPS 260 VAC");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 30_2_3)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 30_2_3)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 30_2_3)
{   TEST_BEGIN();
	TEST_USE(TESTER);
	ViReal64 ActualPower;
	ViReal64 ActualCurrent;
	ViReal64 ActualVoltage;
	ViReal64 Freq;
	ViInt32 Readsize;
	char   msg[256];
	ViChar ErrorMessage[50];
	int    cnt = 0;
		
	//BK : Power on electronic with 90VAC and Measurment of current
	ErrorBK = bp98xx_ConfigureOutput (vi_BK, 260.0, 50.0);
	if (ErrorBK < VI_SUCCESS)
	{		
		bp98xx_GetLastErrorMsg (vi_BK, 255, ErrorMessage, &Readsize);
		sprintf(msg, "Function Error : bp98xx_ConfigureOutput \r\n , message error : %s\n", ErrorMessage);
		EXCTHROW( -1, msg);
	}
	
	//mesure seulement 90VAC 260VAC)
	do{
		Sleep (200);
		cnt++;
		_test_index = 1;
			
		EXCDELETE(pexception);

		bp98xx_MeasureOutput (vi_BK, &Freq, &ActualVoltage, &ActualCurrent, &ActualPower);
		ActualCurrent = ActualCurrent*1000;
		//printf ("30_2_3 : ActualCurrent = %f\r\n", ActualCurrent);
		LIMIT_CHECK_EXT(REAL64, ActualCurrent, pexception);
		
		if (pexception && cnt==40)
			{
				EXCCHECK(pexception);
			}
	//printf ("30_2_3 : cnt = %d\r\n", cnt);
	}while(pexception && cnt<40);



	  
	//mesure seulement 90VAC 260VAC)
	
Error:

    TEST_END();
}

FCE_CLEANUP(EA210, 30_2_3)
{
    TESTCLEANUP_BEGIN();
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 30_2_4
*********************************************************************************/

FCE_HELP(EA210, 30_2_4)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Measurement of output voltage 12VDC with simulated loads");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 30_2_4)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 30_2_4)									
{
    TESTINIT_BEGIN();
	char   msg[256];
	ViInt32 ErrorCode;
	ViChar ErrorMessage[50];
	ViInt32 Readsize;
	/*BK 230 VAC*/
	ErrorBK = bp98xx_ConfigureOutput (vi_BK, 230.0, 50.0);
	if (ErrorBK < VI_SUCCESS)
	{		
		bp98xx_GetLastErrorMsg (vi_BK, 255, ErrorMessage, &Readsize);
		sprintf(msg, "Function Error : bp98xx_ConfigureOutput \r\n  message error : %s\n", ErrorMessage);
		EXCTHROW( -1, msg);
	}
	bp98xx_EnableOutput (vi_BK, VI_TRUE);
		Sleep(500);
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 30_2_4)
{   TEST_BEGIN();
	TEST_USE(TESTER);
	ViInt32 Readsize;
	char   msg[256];
	ViInt32 ErrorCode;
	ViChar ErrorMessage[50];
	SElExceptionPtr	pexc_first = NULL;
	VARIABLE(UINT32, cnt);
    ViReal64  voltage_0 = 0.000;//no load
	ViReal64  voltage_12 = 0.000;//12 ohm
	ViReal64  voltage_100 = 0.000;//100 ohm

    EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3Q_3Q2", TRUE));	//step 5.1
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3M", TRUE));       //step 6.5
	//Connect relays of test measurement output voltage with load 12VDC 0 Ohm
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3H", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3I", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3G", TRUE));


	//AG3450A : Measurement of voltage
	//Configure the meter to 10V range and fast mode (least resolution)
	//ErrorDMM = Ag34450_DCVoltageConfigure(vi_DMM, 100, AG34450_VAL_RESOLUTION_MEDIUM);
	//if (ErrorDMM < VI_SUCCESS)
	//{
	//	Ag34450_GetError(vi_DMM, &ErrorCode, 255, ErrorMessage);
	//	sprintf(msg, "Function Error : Ag34450_DCVoltageConfigure \r\n Error: %d, message error : %s\n", ErrorCode, ErrorMessage);
	//	EXCTHROW( -1, msg);
	//}

	///*- Read DC voltage ---------------------------------------------------*/
	//ErrorDMM = Ag34450_Read (vi_DMM, 10000, &voltage_0);
	//if (ErrorDMM < VI_SUCCESS)
	//{
	//	Ag34450_GetError(vi_DMM, &ErrorCode, 255, ErrorMessage);
	//	sprintf(msg, "Function Error : Ag34450_Read \r\n Error: %d, message error : %s\n", ErrorCode, ErrorMessage);
	//	EXCTHROW( -1, msg);
	//}
	ErrorDMM = Ag34450_DCVoltageMeasure (vi_DMM, 100, AG34450_VAL_RESOLUTION_MEDIUM, &voltage_0);
	if (ErrorDMM < VI_SUCCESS)
	{
		ErrorDMM = Ag34450_DCVoltageMeasure (vi_DMM, 100, AG34450_VAL_RESOLUTION_MEDIUM, &voltage_0);
		if (ErrorDMM < VI_SUCCESS)
		{
			Ag34450_GetError(vi_DMM, &ErrorCode, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34450_Read \r\n Error: %d, message error : %s\n", ErrorCode, ErrorMessage);
			EXCTHROW( -1, msg);
		}
	}
	LIMIT_CHECK(REAL64, voltage_0);

	//Connect relays of test measurement output voltage with load 12VDC 12 Ohm
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3H", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3I", TRUE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3G", TRUE));
	Sleep(300);
	//KEYSIGHT : Meaurment of voltage

	/*- Read DC voltage ---------------------------------------------------*/
	/*ErrorDMM = Ag34450_Read (vi_DMM, 8000, &voltage_12);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34450_GetError(vi_DMM, &ErrorCode, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34450_Read \r\n Error: %d, message error : %s\n", ErrorCode, ErrorMessage);
		EXCTHROW( -1, msg);
	}*/
	ErrorDMM = Ag34450_DCVoltageMeasure (vi_DMM, 100, AG34450_VAL_RESOLUTION_MEDIUM, &voltage_12);
	if (ErrorDMM < VI_SUCCESS)
	{
		ErrorDMM = Ag34450_DCVoltageMeasure (vi_DMM, 100, AG34450_VAL_RESOLUTION_MEDIUM, &voltage_12);
		if (ErrorDMM < VI_SUCCESS)
		{
			Ag34450_GetError(vi_DMM, &ErrorCode, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34450_Read \r\n Error: %d, message error : %s\n", ErrorCode, ErrorMessage);
			EXCTHROW( -1, msg);
		}
	}

	LIMIT_CHECK(REAL64, voltage_12);

	//Connect relays of test measurement output voltage with load 12VDC 100 Ohm
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3I", FALSE)); 		
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3G", TRUE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3H", TRUE));
	//KEYSIGHT : Measurement of voltage
	Sleep(300);
	/*- Read DC voltage ---------------------------------------------------*/
	//ErrorDMM = Ag34450_Read (vi_DMM, 8000, &voltage_100);
	//if (ErrorDMM < VI_SUCCESS)
	//{
	//	Ag34450_GetError(vi_DMM, &ErrorCode, 255, ErrorMessage);
	//	sprintf(msg, "Function Error : Ag34450_Read \r\n Error: %d, message error : %s\n", ErrorCode, ErrorMessage);
	//	EXCTHROW( -1, msg);
	//}
	ErrorDMM = Ag34450_DCVoltageMeasure (vi_DMM, 100, AG34450_VAL_RESOLUTION_MEDIUM, &voltage_100);
	if (ErrorDMM < VI_SUCCESS)
	{
		ErrorDMM = Ag34450_DCVoltageMeasure (vi_DMM, 100, AG34450_VAL_RESOLUTION_MEDIUM, &voltage_100);
		if (ErrorDMM < VI_SUCCESS)
		{
			Ag34450_GetError(vi_DMM, &ErrorCode, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34450_Read \r\n Error: %d, message error : %s\n", ErrorCode, ErrorMessage);
			EXCTHROW( -1, msg);
		}
	}

	LIMIT_CHECK(REAL64, voltage_100);

	//Disconnect all relays of test measurement output voltage with load 12VDC
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3G", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3H", FALSE));	
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3I", FALSE)); 
	
Error:

    TEST_END();
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3G", FALSE)); 
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3H", FALSE));	
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3I", FALSE));
 
	
}

FCE_CLEANUP(EA210, 30_2_4)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 30_2_5
*********************************************************************************/

FCE_HELP(EA210, 30_2_5)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Measurement of output voltage 5VDC");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 30_2_5)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 30_2_5)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 30_2_5)
{   TEST_BEGIN();
	TEST_USE(TESTER);
	ViStatus ErrorCode;
	ViInt32 Readsize;
	ViReal64 voltage_0=0.000;
	ViReal64 voltage_250=0.000;
	ViChar ErrorMessage[50];
	SElExceptionPtr	pexc_first = NULL;
	char   msg[256];
	int cnt=0;
 
	//Connect relays of test measurement output voltage with load 5VDC 0 Ohm
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3O", TRUE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3P", FALSE));
	Sleep(100);
	//Configure the meter to 20v range and fast mode (least resolution)
	ErrorDMM = Ag34450_DCVoltageConfigure(vi_DMM, 100, AG34450_VAL_RESOLUTION_MEDIUM);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34450_GetError(vi_DMM, &ErrorCode, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34450_DCVoltageConfigure \r\n Error: %d, message error : %s\n", ErrorCode, ErrorMessage);
		EXCTHROW( -1, msg);
	}

	Sleep(200);
	/*- Read DC voltage ---------------------------------------------------*/
	ErrorDMM = Ag34450_Read (vi_DMM, 8000, &voltage_0);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34450_GetError(vi_DMM, &ErrorCode, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34450_Read \r\n Error: %d, message error : %s\n", ErrorCode, ErrorMessage);
		EXCTHROW( -1, msg);
	}

	LIMIT_CHECK(REAL64, voltage_0);
	Sleep(200);
	//Connect relays of test measurement output voltage with load 5VDC 250 Ohm
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3O", TRUE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3P", TRUE));

	//KEYSIGHT : Meaurment of voltage

	/*- Read DC voltage ---------------------------------------------------*/
	ErrorDMM = Ag34450_Read (vi_DMM, 8000, &voltage_250);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34450_GetError(vi_DMM, &ErrorCode, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34450_Read \r\n Error: %d, message error : %s\n", ErrorCode, ErrorMessage);
		EXCTHROW( -1, msg);
	}

	LIMIT_CHECK(REAL64, voltage_250);

	//Disconnect all relays of test measurement output voltage with load 12VDC
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3O", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3P", FALSE)); 
	
Error:

    TEST_END();
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3O", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3P", FALSE));
}

FCE_CLEANUP(EA210, 30_2_5)
{
    TESTCLEANUP_BEGIN();  
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 30_3_1
*********************************************************************************/

FCE_HELP(EA210, 30_3_1)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : ENTER HW Test Mode 230 VAC");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 30_3_1)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 30_3_1)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 30_3_1)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
    ViChar ErrorMessage[50];
	char   msg[256];
	ViInt32 Readsize;
	
    EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_30));	
	// Connect UDA
	EXCCHECK( ptester->SetSensor(ptester, "CMD_DBUS_Relay_3F", TRUE));
	Sleep(100);

	// Change HW Test mode	
	do{
		cnt++;
		
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_30, 0x1B, "7000", "23C0");
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_30, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7100"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 40) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7100"))
	{
		LIMIT_CREATE("7100", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	/*if (strcmp(MessageData, "00000000"))//00000000
	{
		LIMIT_CREATE( "00000000", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}*/

		
	

Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_30));
    TEST_END();
}

FCE_CLEANUP(EA210, 30_3_1)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 30_3_2
*********************************************************************************/

FCE_HELP(EA210, 30_3_2)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Check SM status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 30_3_2)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 30_3_2)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 30_3_2)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_30));
	Sleep(200);
	
	do{
		cnt++;
		
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_30, 0xA0, "F50E", "C0");
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_30, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "F50F"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 40) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "F50F"))
	{
		LIMIT_CREATE("F50F", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, ""))
	{
		LIMIT_CREATE( "", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}

		
	

Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_30));
    TEST_END();
}

FCE_CLEANUP(EA210, 30_3_2)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 30_3_3
*********************************************************************************/

FCE_HELP(EA210, 30_3_3)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Check tracing ID of SMB ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 30_3_3)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 30_3_3)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 30_3_3)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;

	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_30));
	
	do{
		cnt++;
		
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_30, 0xA0, "600D", "C0");//A00D
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_30, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "610D"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt > 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "610D"))
	{
		LIMIT_CREATE("610D", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	/*if (strcmp(MessageData, "003830303131363230373730303030343430333335303030303639363031"))
	{
		LIMIT_CREATE( "003830303131363230373730303030343430333335303030303632373734", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}*/

		
	

Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_30));
    TEST_END();
}

FCE_CLEANUP(EA210, 30_3_3)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 30_3_4
*********************************************************************************/

FCE_HELP(EA210, 30_3_4)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Standby Mode Activation of SMB ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 30_3_4)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 30_3_4)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 30_3_4)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;

    EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_30));
	
	do{
		cnt++;
		
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_30, 0xA0, "600C", "C0");
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_30, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "610C"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "610C"))
	{
		LIMIT_CREATE("610C", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "00"))
	{
		LIMIT_CREATE( "00", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}

		
	

Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_30));
    TEST_END();
}

FCE_CLEANUP(EA210, 30_3_4)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 30_4_1
*********************************************************************************/

FCE_HELP(EA210, 30_4_1)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Calibrate line voltage measurement 230VDC ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 30_4_1)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 30_4_1)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 30_4_1)
{
	char voltchar[10];
	char outputchar[50];
	char value[10];
	ViReal64 ActualPower;
	ViReal64 ActualCurrent;
	ViReal64 ActualVoltage;
	ViReal64 Freq;

	TEST_BEGIN();
	TEST_USE(DBUS);
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	ViChar ErrorMessage[256];
	ViInt32 Readsize;
	int cnt=0;
	int MessageID_OK = 0;
	char   msg[256];
    int val;
	//BK : Power on electronic with 130VAC
	ErrorBK = bp98xx_ConfigureOutput (vi_BK, 230.0, 50.0);
	if (ErrorBK < VI_SUCCESS)
	{		
		
		
		bp98xx_GetLastErrorMsg (vi_BK, 255, ErrorMessage, &Readsize);
		sprintf(msg, "Function Error : bp98xx_ConfigureOutput \r\n  message error : %s\n", ErrorMessage);
		EXCTHROW( -1, msg);
	}
	 EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_30));
	 Sleep(100);
#if 0    /* formerly excluded lines */
     /*bp98xx_MeasureOutput (vi_BK, &Freq, &ActualVoltage, &ActualCurrent, &ActualPower);
	 sprintf(value, "%5f\n", ActualVoltage*100);
	 CopyString(voltchar,0,value,0,5);
	 val=atoi(voltchar);
	 sprintf(value, "%5x\n", val);*/
#endif   /* formerly excluded lines */

	do{
		cnt++;
		
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_30, 0x1B, "7073","59D8" );//
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_30, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7173"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7173"))
	{
		LIMIT_CREATE("7173", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "59D8"))
	{
		LIMIT_CREATE( value, "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}

Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_30));
    TEST_END();
	//bp98xx_EnableOutput (vi_BK, VI_FALSE);
}

FCE_CLEANUP(EA210, 30_4_1)
{
    TESTCLEANUP_BEGIN();
	Sleep(100);
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 30_4_2
*********************************************************************************/

FCE_HELP(EA210, 30_4_2)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Check correction factor 230 V");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 30_4_2)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 30_4_2)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 30_4_2)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	STRING(byte1,4+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;

    //Sleep(1000);
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_30));	
	do{
		cnt++;
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_30, 0x1B, "7074", "");
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_30, 0xC0, MessageID, MessageData);
		//----------------------------------
		CopyString (byte,0,MessageData,4,2);
	    sscanf(byte, "%02X", &valeur);
		
		//----------------------------------
		//if (!strcmp(MessageID, "7174"))
			if (valeur==1)
			      MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 40) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7174"))
	{
		LIMIT_CREATE("7174", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	//Correction factor	
	CopyString (byte1,0,MessageData,0,4);
	sscanf(byte1, "%04X", &valeur);
	//LIMIT_CHECK(INT32, valeur);	
	
	//Check result of Set Line Circuit Linear Factor value	
	CopyString (byte,0,MessageData,4,2);
	sscanf(byte, "%02X", &valeur);
	LIMIT_CHECK(INT32, valeur );
	  

Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_30));
    TEST_END();
}

FCE_CLEANUP(EA210, 30_4_2)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 30_4_3
*********************************************************************************/

FCE_HELP(EA210, 30_4_3)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Check corrected line voltage 230 VAC ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 30_4_3)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 30_4_3)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 30_4_3)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	STRING(byte1, 4+1);	
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK=0;
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_30));
				
	do{
		cnt++;
		
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_30, 0x1B, "7070", "");
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_30, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7170"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7170"))
	{
		LIMIT_CREATE("7170", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	//Line voltage
	CopyString (byte1,0,MessageData,0,4);
	
	sscanf(byte1, "%04X", &valeur);
	LIMIT_CHECK(INT32, valeur );	
	  
	//Frequency
	CopyString (byte,0,MessageData,4,2);
	sscanf(byte, "%02X", &valeur);
	LIMIT_CHECK(INT32, valeur );
      
	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_30));
    TEST_END();
	//bp98xx_EnableOutput (vi_BK, VI_FALSE);
}

FCE_CLEANUP(EA210, 30_4_3)
{
    TESTCLEANUP_BEGIN();
   // bp98xx_EnableOutput (vi_BK, VI_FALSE);
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 30_4_5
*********************************************************************************/

FCE_HELP(EA210, 30_4_5)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Calibrate line voltage measurement 230VDC ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 30_4_5)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 30_4_5)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 30_4_5)
{
	ViReal64 ActualPower;
	ViReal64 ActualCurrent;
	ViReal64 ActualVoltage;
	ViReal64 Freq;

	TEST_BEGIN();
	TEST_USE(TESTER);
	TEST_USE(DBUS);
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	ViInt32 Readsize;
	ViChar ErrorMessage[256];
	char   msg[512];

	//BK : Power on electronic with 230VAC
		ErrorBK = bp98xx_ConfigureOutput (vi_BK, 230.0, 50.0);
	if (ErrorBK < VI_SUCCESS)
	{		
		bp98xx_GetLastErrorMsg (vi_BK, 255, ErrorMessage, &Readsize);
		sprintf(msg, "Function Error : bp98xx_ConfigureOutput \r\n  message error : %s\n", ErrorMessage);
		EXCTHROW( -1, msg);
	}
	
	Sleep(1000);
    
	// Calibrate line voltage measurement 230V 50Hz
	
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_30));	
	do{
		cnt++;
		
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_30, 0x1B, "7073", "59D800000000");
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_30, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7173"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7173"))
	{
		LIMIT_CREATE("7173", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "59D8"))
	{
		LIMIT_CREATE( "59D8", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}

Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_30));
    TEST_END();
	//bp98xx_EnableOutput (vi_BK, VI_FALSE);
}

FCE_CLEANUP(EA210, 30_4_5)
{
    TESTCLEANUP_BEGIN();
	//bp98xx_EnableOutput (vi_BK, VI_FALSE);
    
Error:    
    TESTCLEANUP_END();
}


/*********************************************************************************
 * Function 30_4_7
*********************************************************************************/

FCE_HELP(EA210, 30_4_7)
{
    HELP_BEGIN();
	HELP_TEST( " DBUS : Check corrected line voltage ");

Error:
    HELP_END();
}

FCE_CHECK(EA210, 30_4_7)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 30_4_7)									
{
   	TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 30_4_7)
{
    TEST_BEGIN();
	TEST_USE(DBUS);
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	STRING(byte1, 4+1);	
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK=0;
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_30));
				
	do{
		cnt++;
		
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_30, 0x1B, "7070", "");
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_30, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7170"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7170"))
	{
		LIMIT_CREATE("7170", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	//Line voltage
	CopyString (byte1,0,MessageData,0,4);
	
	sscanf(byte1, "%04X", &valeur);
	LIMIT_CHECK(INT32, valeur );
	  
	
	//Frequency
	CopyString (byte,0,MessageData,4,2);
	sscanf(byte, "%02X", &valeur);
	LIMIT_CHECK(INT32, valeur );
      
	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_30));
    TEST_END();
}

FCE_CLEANUP(EA210, 30_4_7)
{	
    TESTCLEANUP_BEGIN();

    
Error:    
    TESTCLEANUP_END();
}
/*********************************************************************************
 * Function 30_5_1
*********************************************************************************/

FCE_HELP(EA210, 30_5_1)
{
    HELP_BEGIN();
	HELP_TEST( " DBUS : Check external temperature sensor inputs ");

Error:
    HELP_END();
}

FCE_CHECK(EA210, 30_5_1)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 30_5_1)									
{
   	TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 30_5_1)
{
    TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 4+1);	
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK=0;
	ViInt32 Readsize;
	ViChar ErrorMessage[256];
	char   msg[512];

	
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3Q_3Q2", TRUE));	
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_30));
		Sleep(500);			
	do{
		cnt++;
		
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_30, 0x1B, "7051", "");
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_30, 0xC0, MessageID, MessageData);
		
		if (strcmp(MessageID, "7151")==0)
			MessageID_OK = 1;
	
		if (MessageID_OK==1)
			break;
		/*CopyString (byte,0,MessageData,0,4);
	    sscanf(byte, "%04X", &valeur);
		printf("VAL %d =%d \n",cnt,valeur);*/
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7151"))
	{
		LIMIT_CREATE("7151", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	//Analog Input X11.1
	CopyString (byte,0,MessageData,0,4);
	sscanf(byte, "%04X", &valeur);
	LIMIT_CHECK(INT32, valeur );	
	  
	
	//Analog Input X11.2
	CopyString (byte,0,MessageData,4,4);
	sscanf(byte, "%04X", &valeur);
	LIMIT_CHECK(INT32, valeur );	
	  
	
	//Analog Input X11.3
	CopyString (byte,0,MessageData,8,4);
	sscanf(byte, "%04X", &valeur);
	LIMIT_CHECK(INT32, valeur );	
	  
	//Analog Input X11.4
	CopyString (byte,0,MessageData,12,4);
	sscanf(byte, "%04X", &valeur);
	LIMIT_CHECK(INT32, valeur );	
	  
	//Analog Input X11.5
	CopyString (byte,0,MessageData,20,4);
	sscanf(byte, "%04X", &valeur);
	LIMIT_CHECK(INT32, valeur );	
	  
	
	//Analog Input Ambient temperature sensor
	CopyString (byte,0,MessageData,16,4);
	sscanf(byte, "%04X", &valeur);
	LIMIT_CHECK(INT32, valeur );
	  	
	
	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_30));
    TEST_END();
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3Q_3Q2", FALSE));	
	//bp98xx_EnableOutput (vi_BK, VI_FALSE);
}

FCE_CLEANUP(EA210, 30_5_1)
{	
    TESTCLEANUP_BEGIN();
	TEST_USE(TESTER);
	//bp98xx_EnableOutput (vi_BK, VI_FALSE);
	//EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3Q_3Q2", FALSE));	
	

    
Error:    
    TESTCLEANUP_END();
}
/*********************************************************************************
 * Function 30_6_1
*********************************************************************************/

FCE_HELP(EA210, 30_6_1)
{
    HELP_BEGIN();
	HELP_TEST( " DBUS : Check digital input X10.2 ");

Error:
    HELP_END();
}

FCE_CHECK(EA210, 30_6_1)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(DBUS);
	//REQUIRE(TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 30_6_1)									
{
   	TESTINIT_BEGIN();
	TEST_USE(TESTER);
	ViInt32 Readsize;
	char   msg[512];
	ViChar ErrorMessage[50];
	
	//Connect relays of test digital inputs X10.2
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_FTA2", FALSE));	
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_FTA3", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_FTA4", TRUE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_FTA1", TRUE));	
	//BK : Power on electronic with 230VAC
	/*ErrorBK = bp98xx_ConfigureOutput (vi_BK, 230.0, 50.0);
	if (ErrorBK < VI_SUCCESS)
	{		
		
		bp98xx_GetLastErrorMsg (vi_BK, 255, ErrorMessage, &Readsize);
		sprintf(msg, "Function Error : bp98xx_ConfigureOutput \r\n  message error : %s\n", ErrorMessage);
		EXCTHROW( -1, msg);
	}
	Sleep(200);
	ErrorBK = bp98xx_EnableOutput (vi_BK, VI_TRUE);
	if (ErrorBK < VI_SUCCESS)
	{

		bp98xx_GetLastErrorMsg (vi_BK, 255, ErrorMessage, &Readsize);

		sprintf(msg, "Function Error : bp98xx_EnableOutput \r\n  message error : %s\n", ErrorMessage);
		EXCTHROW( -1, msg);
	}*/
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 30_6_1)
{
    TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);	
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK=0;	
	
	//Check digital input X10.2
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_30));
		
	do{
		cnt++;
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_30, 0x1B, "7060", "");
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_30, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7160"))
			if (!strcmp(MessageData, "05"))
			     MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 20) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7160"))
	{
		LIMIT_CREATE("7160", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	/* Check which Input is active "01" */
	CopyString (byte,0,MessageData,0,2);
	sscanf(byte, "%02X", &valeur);
	LIMIT_CHECK(INT32, valeur );	
	  	
	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_30));
    TEST_END();
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_FTA2", FALSE));	
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_FTA3", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_FTA4", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_FTA1", FALSE));	
}

FCE_CLEANUP(EA210, 30_6_1)
{	
    TESTCLEANUP_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_FTA2", FALSE));	
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_FTA3", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_FTA4", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_FTA1", FALSE));

Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 30_6_2
*********************************************************************************/

FCE_HELP(EA210, 30_6_2)
{
    HELP_BEGIN();
	HELP_TEST( " DBUS : Check digital input X10.3 ");

Error:
    HELP_END();
}

FCE_CHECK(EA210, 30_6_2)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(DBUS);
	//REQUIRE(TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 30_6_2)									
{
   	TESTINIT_BEGIN();
	ViInt32 Readsize;
	char   msg[512];
	ViChar ErrorMessage[50];
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 30_6_2)
{
    TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);	
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK=0;
	
	//Connect relays of test digital inputs X10.3
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_FTA1", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_FTA4", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_FTA2", TRUE));	
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_FTA3", TRUE));
	
	
	//Check digital input X10.3
	
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_30));
		
	do{
		cnt++;
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_30, 0x1B, "7060", "");
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_30, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7160"))
			if (!strcmp(MessageData, "06"))
			    MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 20) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7160"))
	{
		LIMIT_CREATE("7160", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	/* Check which Input is active "02" */
	CopyString (byte,0,MessageData,0,2);
	sscanf(byte, "%02X", &valeur);
	LIMIT_CHECK(INT32, valeur );	
	  	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_30));
    TEST_END();

}

FCE_CLEANUP(EA210, 30_6_2)
{	
    TESTCLEANUP_BEGIN();
	TEST_USE(TESTER);
    //TESTER_VERTICAL_UNPLUG_PANEL_STATION_30(TIMEOUT_DEFAULT);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_FTA1", FALSE));	
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_FTA2", FALSE));	
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_FTA3", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_FTA4", FALSE));

    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
* Function 30_6_3
*********************************************************************************/

FCE_HELP(EA210, 30_6_3)
{
    HELP_BEGIN();
	HELP_TEST( " DBUS : Check magnetic door open  sensor ");

Error:
    HELP_END();
}

FCE_CHECK(EA210, 30_6_3)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(DBUS);
	//REQUIRE(TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 30_6_3)									
{
   	TESTINIT_BEGIN();
	TEST_USE(TESTER);
	ViInt32 Readsize;
	ViChar ErrorMessage[50];
	char   msg[512];
	
	//Disactivate magnetic field cmd EV
	TESTER_VERTICAL_UNPLUG_PANEL_STATION_30(TIMEOUT_DEFAULT);
	Sleep(300);
	
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 30_6_3)
{
    TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);	
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK=0;
	
	//Check magnetic door closed sensor
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_30));
				
	do{
		cnt++;
		
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_30, 0x1B, "7060", "");
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_30, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7160"))
			//if (!strcmp(MessageData, "3"))
			    MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7160"))
	{
		LIMIT_CREATE("7160", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	/* Check which Input is active 03 */
	CopyString (byte,0,MessageData,0,2);
	sscanf(byte, "%02X", &valeur);
	LIMIT_CHECK(INT32, valeur );	
	  	
	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_30));
    TEST_END();
}

FCE_CLEANUP(EA210, 30_6_3)
{	
    TESTCLEANUP_BEGIN();

    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 30_6_4
*********************************************************************************/

FCE_HELP(EA210, 30_6_4)
{
    HELP_BEGIN();
	HELP_TEST( " DBUS : Check magnetic door closed sensor ");

Error:
    HELP_END();
}

FCE_CHECK(EA210, 30_6_4)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(DBUS);
	//REQUIRE(TESTER);	
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 30_6_4)									
{
   	TESTINIT_BEGIN();
	TEST_USE(TESTER);
	ViInt32 Readsize;
	ViChar ErrorMessage[50];
	char   msg[512];
	
	//magnetic field cmd EV
	TESTER_VERTICAL_PLUG_PANEL_STATION_30(TIMEOUT_DEFAULT);
	Sleep(400);

Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 30_6_4)
{
    TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);	
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK=0;

	//Check of magnetic door closed sensor
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_30));
				
	do{
		cnt++;
		
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_30, 0x1B, "7060", "");
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_30, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7160"))
			if (!strcmp(MessageData, "03"))
				MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 40) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7160"))
	{
		LIMIT_CREATE("7160", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	//Check which Input is active
	CopyString (byte,0,MessageData,0,2);
	sscanf(byte, "%02X", &valeur);
	LIMIT_CHECK(INT32, valeur );	
	  	
	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_30));
    TEST_END();
}

FCE_CLEANUP(EA210, 30_6_4)
{	
    TESTCLEANUP_BEGIN();
	TEST_USE(TESTER);
	ptester->VerticalUnplugStation30(ptester, 5000);
Error:    
    TESTCLEANUP_END();
}


/*********************************************************************************
 * Function 30_6_5
*********************************************************************************/

FCE_HELP(EA210, 30_6_5)
{
    HELP_BEGIN();
	HELP_TEST( " DBUS : Test of PWM Feedback Input X8.4 ");

Error:
    HELP_END();
}

FCE_CHECK(EA210, 30_6_5)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(DBUS);
	//REQUIRE(TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 30_6_5)									
{
   	TESTINIT_BEGIN();
	//TEST_USE(TESTER);
	//ViInt32 Readsize;
	//char   msg[512];
	//ViChar ErrorMessage[50];
	//EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3L", FALSE)); 	
	//EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3N", FALSE));
	//EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3M", TRUE));
	//EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3M", TRUE));
	//BK : Power on electronic with 230VAC
	/*ErrorBK = bp98xx_ConfigureOutput (vi_BK, 230.0, 50.0);
	if (ErrorBK < VI_SUCCESS)
	{		
		
		bp98xx_GetLastErrorMsg (vi_BK, 255, ErrorMessage, &Readsize);
		sprintf(msg, "Function Error : bp98xx_ConfigureOutput \r\n  message error : %s\n", ErrorMessage);
		EXCTHROW( -1, msg);
	}
	Sleep(100);
	ErrorBK = bp98xx_EnableOutput (vi_BK, VI_TRUE);
	if (ErrorBK < VI_SUCCESS)
	{

		bp98xx_GetLastErrorMsg (vi_BK, 255, ErrorMessage, &Readsize);

		sprintf(msg, "Function Error : bp98xx_EnableOutput \r\n  message error : %s\n", ErrorMessage);
		EXCTHROW( -1, msg);
	}*/
	
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 30_6_5)
{
    TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	TEST_USE(GENERATOR);
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	STRING(byte1, 4+1);	
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK=0;
	char sys_error[128];
	
	//Test of PWM Feedback Input X8.4
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_30));
	Sleep(100);
	cnt=0;
	do{
		cnt++;
		
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_30, 0x1B, "7025", "00");
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_30, 0xC0, MessageID, MessageData);
		if (!strcmp(MessageID, "7125"))
		{
			CopyString (byte1,0,MessageData,0,4);
			sscanf(byte1, "%04X", &valeur);
		 	 if (valeur>1490)
			   	MessageID_OK = 1;
		}
		if (MessageID_OK)
			break;
		
	}while (cnt<20);
	

	if (strcmp(MessageID, "7125"))
	{
		LIMIT_CREATE("7125", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	//Check which feedback input is active :
	//CopyString (byte,0,MessageData,0,2);
	//sscanf(byte, "%02X", &valeur);
	//LIMIT_CHECK(INT32, valeur);	
	
	//Check the feedback input value :
	CopyString (byte1,0,MessageData,0,4);
	sscanf(byte1, "%04X", &valeur);
	LIMIT_CHECK(INT32, valeur );		
	  	
	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_30));
	TEST_END();
	
}

FCE_CLEANUP(EA210, 30_6_5)
{	
    TESTCLEANUP_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3L", FALSE)); 	
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3N", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3M", FALSE));

    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 30_6_6
*********************************************************************************/

FCE_HELP(EA210, 30_6_6)
{
    HELP_BEGIN();
	HELP_TEST( " DBUS : Test of PWM Feedback Input X8.7 ");

Error:
    HELP_END();
}

FCE_CHECK(EA210, 30_6_6)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(DBUS);
	//REQUIRE(TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 30_6_6)									
{
   	TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 30_6_6)
{
    TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	TEST_USE(GENERATOR);
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	STRING(byte1, 4+1);	
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK=0;
	char sys_error[128];
	//Connect relays of test PWM Feedback Input X8.7
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3M", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3N", FALSE)); 	
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3L", TRUE));		
	
	Sleep(200);
	
	//Test of PWM Feedback Input X8.4
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_30));
				
	do{
		cnt++;
		
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_30, 0x1B, "7025", "01");
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_30, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7125"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7125"))
	{
		LIMIT_CREATE("7125", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	//Check which feedback input is active :
	/*CopyString (byte,0,MessageData,0,2);
	sscanf(byte, "%02X", &valeur);
	LIMIT_CHECK(INT32, valeur);	*/
	
	//Check the feedback input value :
	CopyString (byte1,0,MessageData,0,4);
	sscanf(byte1, "%04X", &valeur);
	LIMIT_CHECK_EXT(INT32, valeur,pexception);		
	EXCDELETE(pexception);	  	
	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_30));
    TEST_END();
}

FCE_CLEANUP(EA210, 30_6_6)
{	
    TESTCLEANUP_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3L", FALSE)); 	
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3N", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3M", FALSE));
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 30_6_7
*********************************************************************************/

FCE_HELP(EA210, 30_6_7)
{
    HELP_BEGIN();
	HELP_TEST( " DBUS : Test of PWM Feedback Input X9.4 ");

Error:
    HELP_END();
}

FCE_CHECK(EA210, 30_6_7)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(DBUS);
	//REQUIRE(TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 30_6_7)									
{
   	TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 30_6_7)
{
    TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	TEST_USE(GENERATOR);
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 4+1);
	STRING(byte1, 4+1);	
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK=0;
	char sys_error[128];
	
	//Connect relays of test PWM Feedback Input X9.4
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3L", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3M", FALSE)); 	
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3N", TRUE));
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_30));
				
	do{
		cnt++;
		
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_30, 0x1B, "7025", "02");
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_30, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7125"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7125"))
	{
		LIMIT_CREATE("7125", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	//Check which feedback input is active :
	/*CopyString (byte,0,MessageData,0,2);
	sscanf(byte, "%02X", &valeur);
	LIMIT_CHECK(INT32, valeur);	*/
	
	//Check the feedback input value :
	CopyString (byte1,0,MessageData,0,4);
	sscanf(byte1, "%04X", &valeur);
	LIMIT_CHECK_EXT(INT32, valeur,pexception);
	EXCDELETE(pexception);	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_30));
    TEST_END();
}

FCE_CLEANUP(EA210, 30_6_7)
{	
    TESTCLEANUP_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3L", FALSE)); 	
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3N", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_3M", FALSE));
    
Error:    
    TESTCLEANUP_END();
}
void string2hexString(char* input, char* output)
{
    int loop;
    int i; 
    
    i=0;
    loop=0;
    
    while(input[loop] != '\0')
    {
        sprintf((char*)(output+i),"%02X", input[loop]);
        loop+=1;
        i+=2;
    }
    //insert NULL at the end of the output string
    output[i++] = '\0';
}
