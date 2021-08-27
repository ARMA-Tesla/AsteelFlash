#include <ansi_c.h>
#include "EA210_utils.h"
#include "toolbox.h"
#include <formatio.h>
#include <multithread/multithread.h>
#include "Ag34450.h"
#include "Ag34970.h"
#include <rs232.h>
#include "bp98xx.h"
#include <advanlys.h>
#include "test_panel.h"
#include "EA210_Test_WS40.h"



double time_station_40;

ViInt32   ErrorDMM,
		  ErrorE3642A,
		  ErrorBK;
ViRsrc    BKresourceNameWs40    ="ASRL4::INSTR";
ViRsrc    AG34470AresourceName  ="ASRL2::INSTR";
ViChar    channel[50];
ViSession vi_AG34470A ;
ViSession vi_BK_WS40;
ViBoolean idQuery_WS40          = VI_TRUE;
ViBoolean reset_WS40            = VI_TRUE;
ViStatus  error_WS40            = VI_SUCCESS;
ViReal64  range_WS40            = 0.20;
ViReal64  resolution_WS40       = 5.50; 
ViReal64  time_ms_WS40   	    = 5000;   
ViInt32   ErrorDMM;

#define UDA_NAME_STATION_40 	"UDA (USB-SN: UDA-04)"

#define CHECK_ANSWER_UDA  1 	

DEFINE_GLOBAL_VARIABLE();

/* Connexion Verin */

#define TESTER_PLUG_PANEL_STATION_40(timeout) \
	EXCCHECK( ptester->PlugPanelStation40(ptester, timeout))

#define TESTER_UNPLUG_PANEL_STATION_40(timeout) \
	EXCCHECK( ptester->UnplugPanelStation40(ptester, timeout))

/* Connexion UDA*/
#define CONNECT_UDA_RELAY(timeout) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_DBUS_Relay_4R", TRUE))
#define DISCONNECT_UDA_RELAY(timeout) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_DBUS_Relay_4R", FALSE))

#define TESTER_DISCONNECT_ALL_STATION_40(timeout) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4A",                FALSE)) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4B",                FALSE)) \ 
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4S",                FALSE)) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4U",                FALSE)) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4V_4V2",            FALSE)) \ 
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4Z",                FALSE)) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4W",                FALSE)) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_DBUS_Relay_4R",           FALSE)) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_PowerSupply_Relay_4H",    FALSE)) ;
	//EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4T",                FALSE)) \ // MOVED TO WS 50 

STestListItem gs_TestList_Station_40[] = {

	/* STATION 40 */
	ADD_TI( "40.0.0",  EA210, 40_0_0),		/* TEST : PLug Product*/
	ADD_TI( "40.0.1",  EA210, 40_0_1),		/* TEST : unPLug Product */
	ADD_TI( "40.0.2",  EA210, 40_0_2),		/* TEST : DBUS ENTER HARDWARE TEST MODE */	
	
	ADD_TI( "40.3.2",  EA210, 40_3_2),		/* TEST : SMB STATION 30 */
	ADD_TI( "40.3.3",  EA210, 40_3_3),		/* TEST : SMB STATION 30 */
	ADD_TI( "40.3.4",  EA210, 40_3_4),		/* TEST : SMB STATION 30 */
	
	ADD_TI( "40.7.1",  EA210, 40_7_1),		/* TEST : Test of DC Switch output X8.8 on status */
	ADD_TI( "40.7.2",  EA210, 40_7_2),		/* TEST : Test of DC Switch output X8.8 off status */
	ADD_TI( "40.7.3",  EA210, 40_7_3),		/* TEST : Test of DC Switch output X8.9 on status */
	ADD_TI( "40.7.4",  EA210, 40_7_4),		/* TEST : Test of DC Switch output X8.9 off status */
	ADD_TI( "40.7.5",  EA210, 40_7_5),		/* TEST : Test of Switch FAN Outputs X8.3 on status */
	ADD_TI( "40.7.6",  EA210, 40_7_6),		/* TEST : Test of Switch FAN Outputs X8.3 off status */
	ADD_TI( "40.7.7",  EA210, 40_7_7),		/* TEST : Test of Switch FAN Outputs X8.6 on status */
	ADD_TI( "40.7.8",  EA210, 40_7_8),		/* TEST : Test of Switch FAN Outputs X8.6 off status */
	ADD_TI( "40.7.9",  EA210, 40_7_9),		/* TEST : Test of Switch FAN Outputs X9.3 on status */
	ADD_TI( "40.7.10", EA210, 40_7_10),		/* TEST : Test of Switch FAN Outputs X9.3 off status */
	ADD_TI( "40.7.11", EA210, 40_7_11),		/* TEST : Test of PWM FAN Outputs X8.2 on status */
	ADD_TI( "40.7.12", EA210, 40_7_12),		/* TEST : Test of PWM FAN outputs X8.2 off status */
	ADD_TI( "40.7.13", EA210, 40_7_13),		/* TEST : Test of PWM FAN outputs X8.5 on status */
	ADD_TI( "40.7.14", EA210, 40_7_14),		/* TEST : Test of PWM FAN outputs X8.5 off status */
	ADD_TI( "40.7.15", EA210, 40_7_15),		/* TEST : Test of PWM FAN outputs X9.2 on status */
	ADD_TI( "40.7.16", EA210, 40_7_16),		/* TEST : Test of PWM FAN outputs X9.2 off status */
	ADD_TI( "40.7.17", EA210, 40_7_17),		/* TEST : Test of PWM FAN outputs X8.7 on status */
	ADD_TI( "40.7.18", EA210, 40_7_18),		/* TEST : Test of PWM FAN outputs X8.7 off status */
	ADD_TI( "40.7.19", EA210, 40_7_19),		/* TEST : Test of constant current LED outputs status on */
	ADD_TI( "40.7.20", EA210, 40_7_20),		/* TEST : Test of constant current LED  outputs status  off */
	ADD_TI( "40.7.21", EA210, 40_7_21),		/* TEST : Test of Stepper Driver Output */
	ADD_TI( "40.7.22", EA210, 40_7_22),		/* TEST : Test of Stepper Driver Output */
	ADD_TI( "40.7.23", EA210, 40_7_23),		/* TEST : Test of Stepper Driver Output */
	ADD_TI( "40.7.24", EA210, 40_7_24),		/* TEST : Test of Stepper Driver Output */
	ADD_TI( "40.7.25", EA210, 40_7_25),		/* TEST : Test of Stepper Driver Output */
	ADD_TI( "40.7.26", EA210, 40_7_26),		/* TEST : Test of PFM Output */
	ADD_TI( "40.7.27", EA210, 40_7_27),		/* TEST : Test of PFM Output */
	ADD_TI( "40.7.28", EA210, 40_7_28),		/* TEST : Test of PFM Output */

    // Test of digital outputs AC	
	ADD_TI( "40.8.1",  EA210, 40_8_1),		/* TEST : Test of relay output X2.1 on status */
	ADD_TI( "40.8.2",  EA210, 40_8_2),		/* TEST : Test of relay output X2.1 off status */	
	ADD_TI( "40.8.3",  EA210, 40_8_3),		/* TEST : Test of relay output X2.2 on status */	
	ADD_TI( "40.8.4",  EA210, 40_8_4),		/* TEST : Test of relay output X2.2 off status */
	ADD_TI( "40.8.5",  EA210, 40_8_5),		/* TEST : Test of relay output X2.3 on status */
	ADD_TI( "40.8.6",  EA210, 40_8_6),		/* TEST : Test of relay output X2.3 off status */
	ADD_TI( "40.8.7",  EA210, 40_8_7),		/* TEST : Check triac output X3.2 on status */
	ADD_TI( "40.8.8",  EA210, 40_8_8),		/* TEST : Check triac output X3.2 off status */
	ADD_TI( "40.8.9",  EA210, 40_8_9),		/* TEST : Check triac output X3.3 on status */
	ADD_TI( "40.8.10", EA210, 40_8_10),		/* TEST : Check triac output X3.3 off status */
	ADD_TI( "40.8.11", EA210, 40_8_11),		/* TEST : Check triac output X3.4 on status */
	ADD_TI( "40.8.12", EA210, 40_8_12),		/* TEST : Check triac output X3.4 off status */
	ADD_TI( "40.8.13", EA210, 40_8_13),		/* TEST : Check triac output X3.5 on status */
	ADD_TI( "40.8.14", EA210, 40_8_14),		/* TEST : Check triac output X3.5 off status */
	ADD_TI_LAST_ENTRY
};
int CVICALLBACK OnTimerWS40 (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{	
	char            stime[32];
	
	switch (event)
	{
		case EVENT_TIMER_TICK:
			
			time_station_40 = time_station_40 + 0.1;
			
			sprintf (stime, "(%0.1fs)", time_station_40);
			SetCtrlVal( gs_TestPanel, PANEL_TIME_WS40, stime); 

			break;
	}
	return 0;
}

/*********************************************************************************
 * Function Testsequence Init
 *********************************************************************************/
FCE_TESTSEQUENCE_INIT(STATION_40)
{
	//Sleep(100);
	TESTSEQUENCEINIT_BEGIN();
	time_station_40 = 0.0;
	
	SetCtrlAttribute (gs_TestPanel, PANEL_TIMER_WS40, ATTR_ENABLED, TRUE);
	TEST_USE(TESTER);
	PANEL_INI();
	TESTER_DISCONNECT_ALL_STATION_40();
	Sleep(100);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_PowerSupply_Relay_4H",    TRUE)); 
	EXCCHECK( ptester->SetSensor(ptester, "CMD_DBUS_Relay_4R", TRUE));
	
	
		//printf("init WS40 \n\r ");

Error:
	if (pexception) 	SetCtrlAttribute (gs_TestPanel, PANEL_TIMER_WS40,ATTR_ENABLED, FALSE);
	TESTSEQUENCEINIT_END();
}


/*********************************************************************************
 * Function Testsequence Cleanup
 *********************************************************************************/
FCE_TESTSEQUENCE_CLEANUP(STATION_40)
{
	TESTSEQUENCECLEANUP_BEGIN(); 
	TEST_USE(TESTER);
	
	TESTER_DISCONNECT_ALL_STATION_40();
	TESTER_UNPLUG_PANEL_STATION_40(5000);
	Ag34970_ClearError (vi_AG34470A);
	Ag34970_reset(vi_AG34470A);


			//printf("cleanup WS40 \n\r ");
Error:
	SetCtrlAttribute (gs_TestPanel, PANEL_TIMER_WS40, ATTR_ENABLED, FALSE);
	DISPLAY_TESTSEQ_RESULT ();	
	TESTSEQUENCECLEANUP_END();	
}

/*********************************************************************************
 * Function Testsequence Wait
 *********************************************************************************/
FCE_TESTSEQUENCE_WAIT(STATION_40)
{
	TESTSEQUENCEWAIT_BEGIN(); 
	TEST_USE(TESTER);

	WAIT_TABLE_ROTATION_FINISHED(TIMEOUT_TABLE);
	
Error:
	TESTSEQUENCEWAIT_END();
}
/*********************************************************************************
 * Function 40_0_0
*********************************************************************************/

FCE_HELP(EA210, 40_0_0)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : PLUG PRODUCT & init instrument");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_0_0)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_0_0)									
{
    TESTINIT_BEGIN();	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_0_0)
{
	TEST_BEGIN();
	TEST_USE(TESTER);
	ViChar ErrorMessage[512];
	char   msg[512];
	ViInt32 Readsize;
	
	pexception = ptester->PlugPanelStation40(ptester, 5000);
	if (pexception)
	{
		TESTER_PLUG_PANEL_STATION_40(5000);
	}
	EXCCHECK( ptester->SetSensor(ptester, "CMD_PowerSupply_Relay_4H",    FALSE)) ;

	ErrorDMM = Ag34970_init (AG34470AresourceName, VI_FALSE, VI_TRUE,&vi_AG34470A );
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34470_init \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
	Ag34970_ClearError (vi_AG34470A);
	Sleep(100);
Error:
	TEST_END();
}

FCE_CLEANUP(EA210, 40_0_0)
{
    TESTCLEANUP_BEGIN();
Error:    
    TESTCLEANUP_END();
}
/*********************************************************************************
 * Function 40_0_1
*********************************************************************************/

FCE_HELP(EA210, 40_0_1)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : UNPLUG PRODUCT");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_0_1)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_0_1)									
{
    TESTINIT_BEGIN();	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_0_1)
{
	TEST_BEGIN();
	TEST_USE(TESTER);
	
	pexception = ptester->UnplugPanelStation40(ptester, 5000);
	if (pexception)
	{
       TESTER_UNPLUG_PANEL_STATION_40(5000);
	}

Error:
	TEST_END();
}

FCE_CLEANUP(EA210, 40_0_1)
{
    TESTCLEANUP_BEGIN();
Error:    
    TESTCLEANUP_END();
}

//
/*********************************************************************************
 * Function 40_0_2
*********************************************************************************/

FCE_HELP(EA210, 40_0_2)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : DBUS ENTER HARDWARE TEST MODE");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_0_2)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_0_2)									
{
    TESTINIT_BEGIN();	
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_DBUS_Relay_4R", TRUE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_PowerSupply_Relay_4H",    TRUE)) ;
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_0_2)
{
	TEST_BEGIN();
	TEST_USE(TESTER);
	TEST_USE(DBUS);
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	int cnt=0;
	int MessageID_OK = 0;
	//Start HW test Mode
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));

	do
	{
		cnt++;
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7000", "23C0");
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);

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
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
	TEST_END();
}

FCE_CLEANUP(EA210, 40_0_2)
{
    TESTCLEANUP_BEGIN();
Error:    
    TESTCLEANUP_END();
}
/*********************************************************************************
 * Function 40_7_1
*********************************************************************************/

FCE_HELP(EA210, 40_7_1)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of DC Switch output X8.8 on status");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_7_1)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_7_1)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4V_4V2", TRUE))
	
	Sleep(200);
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_7_1)
{
	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViConstString ChannelName="203";

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	ViChar ErrorMessage[255];
	char msg[255]  ;


 	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	
	// Test of DC Switch output X8.8 on status
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7010", "01");
	Sleep(200);
	
	cnt = 0;
	do{
		cnt++;
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7010", "01");
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7110"))
			 MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
	}while (cnt<40);
	
	if (strcmp(MessageData, "01"))
	   	 Sleep(200);
			
	if (strcmp(MessageID, "7110"))
	{
		LIMIT_CREATE("7110", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	/*if (strcmp(MessageData, "01"))
	{
		LIMIT_CREATE( "01", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}*/

	  ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, ChannelName, 300, 0.001, 1, &voltageRead, &buffersize);
	   if (ErrorDMM < VI_SUCCESS)
		{
			Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
			EXCTHROW( -1, msg);
		}
		
	LIMIT_CHECK(REAL64, voltageRead);
		

Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
	TEST_END();
}

FCE_CLEANUP(EA210, 40_7_1)
{
    TESTCLEANUP_BEGIN();
	TEST_USE(TESTER);
    EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4B", TRUE))//step 7.19 
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4Z", TRUE))//step7.11
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4U", TRUE))//step 7.26 7.27 7.28
Error:    
    TESTCLEANUP_END();
}


/*********************************************************************************
 * Function 40_7_2
*********************************************************************************/

FCE_HELP(EA210, 40_7_2)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of DC Switch output X8.8 off status");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_7_2)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_7_2)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4V_4V2", TRUE));
	Sleep(200);
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_7_2)
{
	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	
	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViConstString ChannelName="203";
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	ViChar ErrorMessage[255];
	char msg[255]  ;
 	
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	
	// Test of DC Switch output X8.8 off status
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7080", "001388");//step 7.27 
	Sleep(70);
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7010", "00");	
	Sleep(100);
	
#if CHECK_ANSWER_UDA
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7110"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7110"))
	{
		LIMIT_CREATE("7110", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "00"))
	{
		LIMIT_CREATE( "00", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */

	  ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, ChannelName, 100, 0.1, 1, &voltageRead, &buffersize);
	 
	  if (ErrorDMM < VI_SUCCESS)
	  {
		  ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, ChannelName, 100, 0.1, 1, &voltageRead, &buffersize);
		  if (ErrorDMM < VI_SUCCESS)
		  {
			  Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
			  sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
			  EXCTHROW( -1, msg);
		  }
	  }
		LIMIT_CHECK(REAL64, voltageRead);
		
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_7_2)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_7_3
*********************************************************************************/

FCE_HELP(EA210, 40_7_3)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of DC Switch output X8.9 off status");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_7_3)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_7_3)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4V_4V2", TRUE))
	Sleep(200);
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_7_3)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViConstString ChannelName="202";
	ViChar ErrorMessage[255];
	char msg[255]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	
 
	// Test of DC Switch output X8.9 on status
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7010", "02");
	Sleep(100);
	
#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7010", "02");
		Sleep(200);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7110"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 20) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7110"))
	{
		LIMIT_CREATE("7110", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "02"))
	{
		LIMIT_CREATE( "02", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */
	
	 ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, ChannelName, 100, 0.1, 1, &voltageRead, &buffersize);
	 
	  if (ErrorDMM < VI_SUCCESS)
	  {
		  ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, ChannelName, 100, 0.1, 1, &voltageRead, &buffersize);
		  if (ErrorDMM < VI_SUCCESS)
		  {
			  Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
			  sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
			  EXCTHROW( -1, msg);
		  }
	  }
		LIMIT_CHECK(REAL64, voltageRead);
			

Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_7_3)
{
    TESTCLEANUP_BEGIN();
	TEST_USE(TESTER);
    EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4B", TRUE))//step 7.19 
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4Z", TRUE))//step7.11
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4U", TRUE))//step 7.26 7.27 7.28    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_7_4
*********************************************************************************/

FCE_HELP(EA210, 40_7_4)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of DC Switch output X8.9 off status");
	Sleep(200);
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_7_4)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_7_4)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4V_4V2", TRUE))
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_7_4)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViConstString ChannelName="202";
	ViChar ErrorMessage[255];
	char msg[255]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	
 
	// Test of DC Switch output X8.9 off status
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7010", "00");
	Sleep(100);

#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7110"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7110"))
	{
		LIMIT_CREATE("7110", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "00"))
	{
		LIMIT_CREATE( "00", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */

	 ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, ChannelName, 100, 0.1, 1, &voltageRead, &buffersize);
	 
	  if (ErrorDMM < VI_SUCCESS)
	  {
		  ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, ChannelName, 100, 0.1, 1, &voltageRead, &buffersize);
		  if (ErrorDMM < VI_SUCCESS)
		  {
			  Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
			  sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
			  EXCTHROW( -1, msg);
		  }
	  }
		LIMIT_CHECK(REAL64, voltageRead);
		
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_7_4)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_7_5
*********************************************************************************/

FCE_HELP(EA210, 40_7_5)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of Switch FAN Outputs X8.3 on status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_7_5)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_7_5)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4V_4V2", TRUE))
	Sleep(200);
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_7_5)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViConstString ChannelName="205";
	ViChar ErrorMessage[255];
	char msg[255]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	
 
	// Test of Switch FAN Outputs X8.3 on status
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7024", "01");
	Sleep(100);
#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7124"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7124"))
	{
		LIMIT_CREATE("7124", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "01"))
	{
		LIMIT_CREATE( "01", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */
	 
	ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, ChannelName, 100, 0.1, 1, &voltageRead, &buffersize);
	 
	  if (ErrorDMM < VI_SUCCESS)
	  {
		  ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, ChannelName, 100, 0.1, 1, &voltageRead, &buffersize);
		  if (ErrorDMM < VI_SUCCESS)
		  {
			  Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
			  sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
			  EXCTHROW( -1, msg);
		  }
	  }
		LIMIT_CHECK(REAL64, voltageRead);
		
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_7_5)
{
    TESTCLEANUP_BEGIN();
	TEST_USE(TESTER);
    EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4B", TRUE))//step 7.19 
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4Z", TRUE))//step7.11
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4U", TRUE))//step 7.26 7.27 7.28     
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_7_6
*********************************************************************************/

FCE_HELP(EA210, 40_7_6)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of Switch FAN Outputs X8.3 off status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_7_6)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_7_6)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4V_4V2", TRUE))
	Sleep(200);
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_7_6)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViConstString ChannelName="205";
	ViChar ErrorMessage[255];
	char msg[255]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	
 
	// Test of Switch FAN Outputs X8.3 off status
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7020", "0032");// step 7.11
	Sleep(20);
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7020", "0132");// step 7.13
	Sleep(20);
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7020", "0232");// step 7.15
	Sleep(20);
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7020", "0332");// step 7.17
	Sleep(70);
	
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7024", "00");
	Sleep(100);
	
#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(100);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7124"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 20) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7124"))
	{
		LIMIT_CREATE("7124", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "00"))
	{
		LIMIT_CREATE( "00", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */
	
	 ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, ChannelName, 100, 0.1, 1, &voltageRead, &buffersize);
	 
	 if (ErrorDMM < VI_SUCCESS)
	 {
		 ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, ChannelName, 100, 0.1, 1, &voltageRead, &buffersize);
		 if (ErrorDMM < VI_SUCCESS)
		 {
			 ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, ChannelName, 100, 0.1, 1, &voltageRead, &buffersize);
			 if (ErrorDMM < VI_SUCCESS)
			 {
				 Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
				 sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
				 EXCTHROW( -1, msg);
			 }
		 }
	 }
		LIMIT_CHECK(REAL64, voltageRead);
		

Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_7_6)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_7_7
*********************************************************************************/

FCE_HELP(EA210, 40_7_7)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of Switch FAN Outputs X8.6 on status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_7_7)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_7_7)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4V_4V2", TRUE))
	Sleep(200);
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_7_7)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViConstString ChannelName="204";
	ViChar ErrorMessage[255];
	char msg[255]  ;
	
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	
 
	// Test of Switch FAN Outputs X8.6 on status
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7024", "02");
	Sleep(100);
	
#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7124"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7124"))
	{
		LIMIT_CREATE("7124", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "02"))
	{
		LIMIT_CREATE( "02", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */
	
    ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, ChannelName, 10.0, 0.1, 1, &voltageRead, &buffersize);
	   if (ErrorDMM < VI_SUCCESS)
		{
			Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
			EXCTHROW( -1, msg);
		}
		LIMIT_CHECK(REAL64, voltageRead);
		

Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_7_7)
{
    TESTCLEANUP_BEGIN();
    	TEST_USE(TESTER);
    EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4B", TRUE))//step 7.19 
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4Z", TRUE))//step7.11
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4U", TRUE))//step 7.26 7.27 7.28  
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_7_8
*********************************************************************************/

FCE_HELP(EA210, 40_7_8)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of Switch FAN Outputs X8.6 off status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_7_8)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_7_8)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4V_4V2", TRUE));
	Sleep(200);
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_7_8)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViConstString ChannelName="204";
	ViChar ErrorMessage[255];
	char msg[255]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	
 
	// Test of Switch FAN Outputs X8.6 off status
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7024", "00");
	Sleep(100);

#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7124"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7124"))
	{
		LIMIT_CREATE("7124", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "00"))
	{
		LIMIT_CREATE( "00", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */
	
	ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, ChannelName, 100, 0.1, 1, &voltageRead, &buffersize);
	   if (ErrorDMM < VI_SUCCESS)
		{
			Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
			EXCTHROW( -1, msg);
		}
		LIMIT_CHECK(REAL64, voltageRead);
		

Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_7_8)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_7_9
*********************************************************************************/

FCE_HELP(EA210, 40_7_9)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of Switch FAN Outputs X9.3 on status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_7_9)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_7_9)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4V_4V2", TRUE))
Error:    
    TESTINIT_END();
	
}

FCE_TEST(EA210, 40_7_9)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	
	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViConstString ChannelName="201";
	ViChar ErrorMessage[255];
	char msg[255]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	
 
	// Test of Switch FAN Outputs X9.3 on status
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7024", "04");
	Sleep(100);
	
#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7124"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7124"))
	{
		LIMIT_CREATE("7124", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "04"))
	{
		LIMIT_CREATE( "04", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */
	 
 	ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, ChannelName, 100, 0.0001, 1, &voltageRead, &buffersize);
	   if (ErrorDMM < VI_SUCCESS)
		{
			Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
			EXCTHROW( -1, msg);
		}
		LIMIT_CHECK_EXT(REAL64, voltageRead,pexception);
		EXCDELETE(pexception);
		
		

Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_7_9)
{
    TESTCLEANUP_BEGIN();
   	TEST_USE(TESTER);
    EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4B", TRUE))//step 7.19 
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4Z", TRUE))//step7.11
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4U", TRUE))//step 7.26 7.27 7.28   
Error:    
    TESTCLEANUP_END();
}



/*********************************************************************************
 * Function 40_7_10
*********************************************************************************/

FCE_HELP(EA210, 40_7_10)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of Switch FAN Outputs X9.3 off status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_7_10)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_7_10)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4V_4V2", TRUE))
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_7_10)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	
	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViConstString ChannelName="201";
	ViChar ErrorMessage[255];
	char msg[255]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	
 
	// Test of Switch FAN Outputs X9.3 off status
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7024", "00");
	Sleep(100);

#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7124"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7124"))
	{
		LIMIT_CREATE("7124", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "00"))
	{
		LIMIT_CREATE( "00", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */
	  
	ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, ChannelName, 100.0, 0.1, 1, &voltageRead, &buffersize);
	   if (ErrorDMM < VI_SUCCESS)
		{
			Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
			EXCTHROW( -1, msg);
		}
		LIMIT_CHECK_EXT(REAL64, voltageRead,pexception);
		EXCDELETE(pexception);
		

Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_7_10)
{
    TESTCLEANUP_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4V_4V2", FALSE))
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_7_11 //4Z CHANNEL 09 
*********************************************************************************/

FCE_HELP(EA210, 40_7_11)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of PWM FAN Outputs X8.2 on status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_7_11)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_7_11)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4V_4V2", FALSE))
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4Z", TRUE))
	Sleep(100);
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_7_11)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 FreqRead;
	ViConstString ChannelName="209";//209
	ViChar ErrorMessage[255];
	char msg[255]  ;
	
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 4+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	
 
	// Test of PWM FAN Outputs X8.2 on status 
#if 0    /* formerly excluded lines */
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	do{
		cnt++;
		
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7020", "0032");//32 voir step 7.6
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7120"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7120"))
	{
		LIMIT_CREATE("7120", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "0032"))
	{
		LIMIT_CREATE( "0032", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */

	
	 // Sleep(100);
	  ErrorDMM= Ag34970_MeasureFreqMeasure (vi_AG34470A, ChannelName, 30000, -102, 1, &FreqRead, &buffersize);
	   if (ErrorDMM < VI_SUCCESS)
		{
			Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34970_MeasureFreqMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
			EXCTHROW( -1, msg);
		}
		
		LIMIT_CHECK(REAL64, FreqRead/1000);
				 

Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_7_11)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_7_12
*********************************************************************************/

FCE_HELP(EA210, 40_7_12)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of PWM FAN outputs X8.2 off status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_7_12)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_7_12)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4Z", TRUE));
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_7_12)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	
	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViConstString ChannelName="209";
	ViChar ErrorMessage[255];
	char msg[255]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	
 
	// Test of PWM FAN outputs X8.2 off status
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	//Sleep(100);//TEST 
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7020", "0064");//32
	Sleep(100);

#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7120"))//7120
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7120"))
	{
		LIMIT_CREATE("7120", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */
	
	cnt=0;
	do
	{
		cnt++;
		ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, ChannelName, 100.0, 0.1, 1, &voltageRead, &buffersize);
		if (ErrorDMM < VI_SUCCESS)
		{
			Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
			EXCTHROW( -1, msg);
		}
		Sleep(100);
		if(voltageRead>=10)
			break;
		//printf("voltage =%f\n",voltageRead);
		if (cnt == 10)
			break;
	}
	while (TRUE);
		LIMIT_CHECK(REAL64, voltageRead);
		
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_7_12)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_7_13
*********************************************************************************/

FCE_HELP(EA210, 40_7_13)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of PWM FAN Outputs X8.5 on status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_7_13)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_7_13)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4Z", TRUE));
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_7_13)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 FreqRead;
	ViConstString ChannelName="208";
	ViChar ErrorMessage[255];
	char msg[255]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 4+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	
 
	// TEST : Test of PWM FAN Outputs X8.5 on status
	/*EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));

	do{
		cnt++;
		
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7020", "0132");
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7120"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7120"))
	{
		LIMIT_CREATE("7120", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "0132"))
	{
		LIMIT_CREATE( "0132", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
 */

	
	cnt=0;
	do
	{
		cnt++;
		ErrorDMM= Ag34970_MeasureFreqMeasure (vi_AG34470A, ChannelName, 30000, 1000, 1, &FreqRead, &buffersize);
		if (FreqRead != 0)
			break;
		if (cnt == 10)
			break;
		Sleep(50);
	}
	while(TRUE);
		
		
		LIMIT_CHECK(REAL64, FreqRead/1000);
				
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_7_13)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_7_14
*********************************************************************************/

FCE_HELP(EA210, 40_7_14)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of PWM FAN outputs X8.5 off status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_7_14)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_7_14)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4Z", TRUE));
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_7_14)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViConstString ChannelName="208";
	ViChar ErrorMessage[255];
	char msg[255]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	
 
	// Test of PWM FAN outputs X8.5 off status
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7020", "0164");
	Sleep(100);
	
#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(200);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7120"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
#endif   /* formerly excluded lines */
	

	/*if (strcmp(MessageID, "7121"))
	{
		LIMIT_CREATE("7120", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "01"))
	{
		LIMIT_CREATE( "01", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}*/
  
	  ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, ChannelName, 100.0, 0.01, 1, &voltageRead, &buffersize);
	   if (ErrorDMM < VI_SUCCESS)
		{
			Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
			EXCTHROW( -1, msg);
		}
		
		LIMIT_CHECK(REAL64, voltageRead);
		


Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_7_14)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}


/*********************************************************************************
 * Function 40_7_15
*********************************************************************************/

FCE_HELP(EA210, 40_7_15)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of PWM FAN Outputs X9.2 on status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_7_15)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_7_15)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4Z", TRUE));
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_7_15)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 FreqRead;
	ViConstString ChannelName="210";
	ViChar ErrorMessage[255];
	char msg[255]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	
 
	// TEST : Test of PWM FAN Outputs X9.2 on on status
	/*EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	do{
		cnt++;
		
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7020", "0232");
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7120"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7120"))
	{
		LIMIT_CREATE("7120", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "0232"))
	{
		LIMIT_CREATE( "0232", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	 */
	  cnt=0;
	do
	{
		cnt++;
		ErrorDMM= Ag34970_MeasureFreqMeasure (vi_AG34470A, ChannelName, 30000, 1000, 1, &FreqRead, &buffersize);
		if (FreqRead != 0)
			break;
		if (cnt == 10)
			break;
		Sleep(50);
	}
	while(TRUE);
	   if (ErrorDMM < VI_SUCCESS)
		{
			Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34970_MeasureFreqMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
			EXCTHROW( -1, msg);
		}
		
		LIMIT_CHECK_EXT(REAL64, FreqRead/1000,pexception);
		EXCDELETE(pexception);
				

Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_7_15)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_7_16
*********************************************************************************/

FCE_HELP(EA210, 40_7_16)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of PWM FAN outputs X9.2 off status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_7_16)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_7_16)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4Z", TRUE));
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_7_16)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViConstString ChannelName="210";
	ViChar ErrorMessage[255];
	char msg[255]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	
 
	// Test of PWM FAN outputs X9.2 off status
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7021", "02");
	Sleep(100);
		
#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		if (!strcmp(MessageID, "7121"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7121"))
	{
		LIMIT_CREATE("7120", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "02"))
	{
		LIMIT_CREATE( "02", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */
    
	//measure output VOLTAGE 
	 	Sleep(100);
	  ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, ChannelName, 100.0, 0.01, 1, &voltageRead, &buffersize);
	   if (ErrorDMM < VI_SUCCESS)
		{
			Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
			EXCTHROW( -1, msg);
		}
		LIMIT_CHECK_EXT(REAL64, voltageRead,pexception);
		EXCDELETE(pexception);	
		
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_7_16)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_7_17
*********************************************************************************/

FCE_HELP(EA210, 40_7_17)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of PWM FAN Outputs X8.7 on status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_7_17)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_7_17)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	//EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4Z", TRUE));
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_7_17)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 FreqRead;
	ViConstString ChannelName="207";
	ViChar ErrorMessage[255];
	char msg[255]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	
 
	// TEST : Test of PWM FAN Outputs X8.7 on on status
#if 0    /* formerly excluded lines */
	/*EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	do{
		cnt++;
		
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7020", "0332");
		Sleep(50);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7120"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7120"))
	{
		LIMIT_CREATE("7120", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "0332"))
	{
		LIMIT_CREATE( "0332", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}*/
#endif   /* formerly excluded lines */

	  ErrorDMM= Ag34970_MeasureFreqMeasure (vi_AG34470A, ChannelName, 30000, 1000, 1, &FreqRead, &buffersize);
	   if (ErrorDMM < VI_SUCCESS)
		{
			Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34970_MeasureFreqMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
			EXCTHROW( -1, msg);
		}
		
		LIMIT_CHECK(REAL64, FreqRead/1000);
				

Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_7_17)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_7_18
*********************************************************************************/

FCE_HELP(EA210, 40_7_18)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of PWM FAN outputs X8.7 off status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_7_18)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_7_18)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4Z", TRUE));
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_7_18)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViConstString ChannelName="207";
	ViChar ErrorMessage[255];
	char msg[255]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	
 
	// Test of PWM FAN outputs X8.7 off status
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7020", "0364");
	Sleep(100);
		
#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
	    
	   /* pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7020", "0364");
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);*/
		
		if (!strcmp(MessageID, "7120"))//7121
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
#endif   /* formerly excluded lines */
	

	/*if (strcmp(MessageID, "7120"))//
	{
		LIMIT_CREATE("7120", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "03"))
	{
		LIMIT_CREATE( "03", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
   Sleep(200);*/
  // cnt=0;
   
	   //measure output signal at connector
	  /* ErrorDMM= Ag34970_VoltageDCVoltageConfigure (vi_AG34470A, ChannelName, 100.0, 0.1);
	   if (ErrorDMM < VI_SUCCESS)
	   {
		   Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		   sprintf(msg, "Function Error : Ag34970_VoltageDCVoltageConfigure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		   EXCTHROW( -1, msg);
	   }
	   Sleep(200);
	   Ag34970_SystemWaitForOperationComplete(vi_AG34470A,5000);*/
	   ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, ChannelName, 100.0, 0.1, 1, &voltageRead, &buffersize);
	   if (ErrorDMM < VI_SUCCESS)
	   {
		   Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		   sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		   EXCTHROW( -1, msg);
	   }
	  
		LIMIT_CHECK(REAL64, voltageRead);
		
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4Z", FALSE));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_7_18)
{
    TESTCLEANUP_BEGIN();
    TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4Z", FALSE));
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_7_19
*********************************************************************************/

FCE_HELP(EA210, 40_7_19)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of constant current LED outputs status on ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_7_19)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_7_19)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4Z", FALSE));
	//EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4B", TRUE));
	Sleep(100);
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_7_19)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 voltageRead[2];
	ViReal64 voltageRead2;
	ViConstString ChannelName="213";
	ViChar ErrorMessage[255];
	char msg[255]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	
 
	// TEST : Test of constant current LED outputs status on
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7042", "64");//32
	Sleep(100);
		
#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7142"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7142"))
	{
		LIMIT_CREATE("7142", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "64"))
	{
		LIMIT_CREATE( "64", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */

	//measure output signal at connector
	/*  ErrorDMM= Ag34970_VoltageDCVoltageConfigure (vi_AG34470A, "213,214", 100.0, 0.00008);
	   if (ErrorDMM < VI_SUCCESS)
		{
			Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34970_VoltageDCVoltageConfigure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
			EXCTHROW( -1, msg);
		}
		//ErrorDMM=Ag34970_VoltageDCVoltageSetNPLC (vi_AG34470A,213, 20);
	    ErrorDMM=Ag34970_SystemWaitForOperationComplete(vi_AG34470A,5000);
		 if (ErrorDMM < VI_SUCCESS)
		{
			Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34970_SystemWaitForOperationComplete \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
			EXCTHROW( -1, msg);
		}*/
			//Sleep(50);
			ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, "213,214", 300, 0.0008, 2, voltageRead, &buffersize);//0.00008
			if (ErrorDMM < VI_SUCCESS)
			{
				Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
				sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
				EXCTHROW( -1, msg);
			}
			
		LIMIT_CHECK(REAL64, voltageRead[0]);
				
		LIMIT_CHECK(REAL64, voltageRead[1]);
				

Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_7_19)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_7_20
*********************************************************************************/

FCE_HELP(EA210, 40_7_20)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of constant current LED outputs status off ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_7_20)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_7_20)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_7_20)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 voltageRead[2];
	ViChar ErrorMessage[255];
	char msg[255]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	
 
	// TEST : Test of constant current LED outputs status off
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7043", "");
	Sleep(100);
		
#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7143"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7143"))
	{
		LIMIT_CREATE("7143", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */


	//measure output signal at connector
	//ErrorDMM= Ag34970_VoltageDCVoltageConfigure (vi_AG34470A, "213,214", 100.0, 0.00008);
	//if (ErrorDMM < VI_SUCCESS)
	//{
	//	Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
	//	sprintf(msg, "Function Error : Ag34970_VoltageDCVoltageConfigure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
	//	EXCTHROW( -1, msg);
	//}

	//ErrorDMM=Ag34970_SystemWaitForOperationComplete(vi_AG34470A,5000);
	ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, "213,214", 100.0, 0.0008, 2, voltageRead, &buffersize);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
	LIMIT_CHECK(REAL64, abs(voltageRead[0]));
		
	LIMIT_CHECK(REAL64, abs(voltageRead[1]));
	
	
	/*//measure output signal at connector
	ErrorDMM= Ag34970_VoltageDCVoltageConfigure (vi_AG34470A, "214", 100.0, 0.00008);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_VoltageDCVoltageConfigure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
    ErrorDMM=Ag34970_SystemWaitForOperationComplete(vi_AG34470A,5000);
	Sleep(300);
	ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, "214", 100.0, 0.00008, 1, &voltageRead, &buffersize);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
	LIMIT_CHECK(REAL64, voltageRead);*/

Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_7_20)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_7_21
*********************************************************************************/

FCE_HELP(EA210, 40_7_21)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : TEST : Test of Stepper Driver Output ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_7_21)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_7_21)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
   //Switch Relays X9.1 X9.2 X9.3 X9.4
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4W", TRUE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4A", TRUE));
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_7_21)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	
	ViInt32 buffersize;
	ViReal64 voltageReadList[4];
	ViChar ErrorMessage[255];
	char msg[255]  ;
	
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	
 
	//	TEST : Test of Stepper Driver Output
	//pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7080", "000000");//STEP 7.28 0 RPM
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7033", "010000");
	Sleep(100);
		
#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7133"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7133"))
	{
		LIMIT_CREATE("7133", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "010000"))
	{
		LIMIT_CREATE( "010000", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */
	
    Sleep(200);//100
	//Measure output signal at connector X10.4 and X9.1
	//ErrorDMM= Ag34970_VoltageDCVoltageConfigure (vi_AG34470A, "212,210,201,211", 100.0, 0.1);
	//if (ErrorDMM < VI_SUCCESS)
	//{
	//	Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
	//	sprintf(msg, "Function Error : Ag34970_VoltageDCVoltageConfigure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
	//	EXCTHROW( -1, msg);
	//}
    //ErrorDMM=Ag34970_SystemWaitForOperationComplete(vi_AG34470A,5000);
	//Sleep(200);
	ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, "212,210,201,211", 100.0, 0.01, 4, voltageReadList, &buffersize);

	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
	LIMIT_CHECK(REAL64, voltageReadList[0]);
	
	LIMIT_CHECK(REAL64, voltageReadList[1]);
	
	LIMIT_CHECK(REAL64, voltageReadList[2]);
	
	LIMIT_CHECK(REAL64, voltageReadList[3]);
	
	
	/*Sleep(100);
	//Measure output signal at connector X10.4 and X9.2
	ErrorDMM= Ag34970_VoltageDCVoltageConfigure (vi_AG34470A, "210", 100.0, 0.1);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_VoltageDCVoltageConfigure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
    ErrorDMM=Ag34970_SystemWaitForOperationComplete(vi_AG34470A,5000);
	Sleep(300);
	ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, "210", 100.0, 0.1, 1, &voltageRead, &buffersize);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
	LIMIT_CHECK(REAL64, voltageRead);
    Sleep(100);
		//Measure output signal at connector X10.4 and X9.3
	ErrorDMM= Ag34970_VoltageDCVoltageConfigure (vi_AG34470A, "201", 100.0, 0.1);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_VoltageDCVoltageConfigure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
    ErrorDMM=Ag34970_SystemWaitForOperationComplete(vi_AG34470A,5000);
	Sleep(300);
	ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, "201", 100.0, 0.1, 1, &voltageRead, &buffersize);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
	LIMIT_CHECK(REAL64, voltageRead);

	Sleep(100);	
	
	//Measure output signal at connector X10.4 and X9.4
	ErrorDMM= Ag34970_VoltageDCVoltageConfigure (vi_AG34470A, "211", 100.0, 0.1);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_VoltageDCVoltageConfigure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
    ErrorDMM=Ag34970_SystemWaitForOperationComplete(vi_AG34470A,5000);
	Sleep(300);
	ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, "211", 100.0, 0.1, 1, &voltageRead, &buffersize);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
	LIMIT_CHECK(REAL64, voltageRead);*/
	
	
	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_7_21)
{
    TESTCLEANUP_BEGIN();
	
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_7_22
*********************************************************************************/

FCE_HELP(EA210, 40_7_22)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of Stepper Driver Output ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_7_22)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_7_22)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_7_22)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 voltageReadList[4];
	ViChar ErrorMessage[255];
	char msg[255]  ;
	
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	
 
	//	TEST : Test of Stepper Driver Output
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7033", "010100");
	Sleep(100);
		
#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(100);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7133"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7133"))
	{
		LIMIT_CREATE("7133", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "010100"))
	{
		LIMIT_CREATE( "010100", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */

    Sleep(300);
	
	
	//Measure output signal at connector X10.4 and X9.1
	//ErrorDMM= Ag34970_VoltageDCVoltageConfigure (vi_AG34470A, "212,210,201,211", 100.0, 0.1);
	//if (ErrorDMM < VI_SUCCESS)
	//{
	//	Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
	//	sprintf(msg, "Function Error : Ag34970_VoltageDCVoltageConfigure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
	//	EXCTHROW( -1, msg);
	//}
    //ErrorDMM=Ag34970_SystemWaitForOperationComplete(vi_AG34470A,5000);
	//Sleep(200);
	ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, "212,210,201,211", 100.0, 0.01, 4, voltageReadList, &buffersize);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
	
	  if((voltageReadList[1])> 12.5)
	 {
    Sleep(200);	 
	ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, "212,210,201,211", 100.0, 0.001, 4, voltageReadList, &buffersize); 
	 }
	LIMIT_CHECK(REAL64, voltageReadList[3]);
		
	LIMIT_CHECK(REAL64, voltageReadList[1]);
	
	LIMIT_CHECK(REAL64, voltageReadList[0]);
	
	LIMIT_CHECK(REAL64, voltageReadList[2]);
	
   /* Sleep(100);
	
	
	//Measure output signal at connector X10.4 and X9.2
	ErrorDMM= Ag34970_VoltageDCVoltageConfigure (vi_AG34470A, "210", 100.0, 0.1);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_VoltageDCVoltageConfigure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
    ErrorDMM=Ag34970_SystemWaitForOperationComplete(vi_AG34470A,5000);
	Sleep(300);
	ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, "210", 100.0, 0.1, 1, &voltageRead, &buffersize);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
	LIMIT_CHECK(REAL64, voltageRead);
	
    Sleep(100);
	
	
	//Measure output signal at connector X10.4 and X9.3
	ErrorDMM= Ag34970_VoltageDCVoltageConfigure (vi_AG34470A, "201", 100.0, 0.1);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_VoltageDCVoltageConfigure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
    ErrorDMM=Ag34970_SystemWaitForOperationComplete(vi_AG34470A,5000);
	Sleep(300);
	ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, "201", 100.0, 0.1, 1, &voltageRead, &buffersize);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
	LIMIT_CHECK(REAL64, voltageRead);

    Sleep(100);
	
	
	//Measure output signal at connector X10.4 and X9.4
	ErrorDMM= Ag34970_VoltageDCVoltageConfigure (vi_AG34470A, "211", 100.0, 0.1);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_VoltageDCVoltageConfigure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
    ErrorDMM=Ag34970_SystemWaitForOperationComplete(vi_AG34470A,5000);
	Sleep(300);
	ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, "211", 100.0, 0.1, 1, &voltageRead, &buffersize);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
	LIMIT_CHECK(REAL64, voltageRead);
*/
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_7_22)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_7_23
*********************************************************************************/

FCE_HELP(EA210, 40_7_23)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of Stepper Driver Output ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_7_23)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_7_23)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_7_23)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	
	ViInt32 buffersize;
	ViReal64 voltageReadList[4];
	ViChar ErrorMessage[255];
	char msg[255]  ;
	
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	
 
	//	TEST : Test of Stepper Driver Output
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7033", "010001");
	Sleep(100);
		
#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(200);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7133"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 20) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7133"))
	{
		LIMIT_CREATE("7133", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "010001"))
	{
		LIMIT_CREATE( "010001", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */

    Sleep(200);
	
	
	//Measure output signal at connector X10.4 and X9.1
	/*ErrorDMM= Ag34970_VoltageDCVoltageConfigure (vi_AG34470A, "212,210,201,211", 100.0, 0.1);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_VoltageDCVoltageConfigure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
    ErrorDMM=Ag34970_SystemWaitForOperationComplete(vi_AG34470A,5000);
	Sleep(200);*/
	ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, "212,210,201,211", 100.0, 0.01, 4, voltageReadList, &buffersize);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
     if((voltageReadList[1])>12.5)
	 {
    Sleep(200);	 
	ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, "212,210,201,211", 100.0, 0.001, 4, voltageReadList, &buffersize); 
	 }
	LIMIT_CHECK(REAL64, voltageReadList[3]);
	
	LIMIT_CHECK(REAL64, voltageReadList[1]);
	
	LIMIT_CHECK(REAL64, voltageReadList[0]);
	
	LIMIT_CHECK(REAL64, voltageReadList[2]);
	
   

Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_7_23)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_7_24
*********************************************************************************/

FCE_HELP(EA210, 40_7_24)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of Stepper Driver Output ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_7_24)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_7_24)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_7_24)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	
	ViInt32 buffersize;
	ViReal64 voltageReadList[4];
	ViChar ErrorMessage[255];
	char msg[255]  ;
		
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	
 
	//	TEST : Test of Stepper Driver Output
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7033", "011000");
	Sleep(100);		
		
#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7133"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7133"))
	{
		LIMIT_CREATE("7133", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "011000"))
	{
		LIMIT_CREATE( "011000", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */

    Sleep(200);
	
	
	//Measure output signal at connector X10.4 and X9.1
	//ErrorDMM= Ag34970_VoltageDCVoltageConfigure (vi_AG34470A, "212,210,201,211", 100.0, 0.1);
	//if (ErrorDMM < VI_SUCCESS)
	//{
	//	Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
	//	sprintf(msg, "Function Error : Ag34970_VoltageDCVoltageConfigure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
	//	EXCTHROW( -1, msg);
	//}
    //ErrorDMM=Ag34970_SystemWaitForOperationComplete(vi_AG34470A,5000);
	//Sleep(300);
	ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, "212,210,201,211", 100.0, 0.01, 4, voltageReadList, &buffersize);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
	  if((voltageReadList[0])>12.5)
	 {
    Sleep(200);	 
	ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, "212,210,201,211", 100.0, 0.001, 4, voltageReadList, &buffersize); 
	 }
	LIMIT_CHECK(REAL64, voltageReadList[3]);
	
	LIMIT_CHECK(REAL64, voltageReadList[1]);
	
	LIMIT_CHECK(REAL64, voltageReadList[0]);
	
	LIMIT_CHECK(REAL64, voltageReadList[2]);
	
 
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_7_24)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_7_25
*********************************************************************************/

FCE_HELP(EA210, 40_7_25)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of Stepper Driver Output ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_7_25)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_7_25)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_7_25)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);

	ViInt32 buffersize;
	ViReal64 voltageReadList[4];
	ViChar ErrorMessage[255];
	char msg[255]  ;
		
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	
 
	//	TEST : Test of Stepper Driver Output
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7033", "010010");
	Sleep(100);
		
#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(200);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7133"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7133"))
	{
		LIMIT_CREATE("7133", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "010010"))
	{
		LIMIT_CREATE( "010010", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */

    Sleep(200);
	ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, "212,210,201,211", 100.0, 0.01, 4, voltageReadList, &buffersize);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
	LIMIT_CHECK(REAL64, voltageReadList[3]);
	
	LIMIT_CHECK(REAL64, voltageReadList[1]);
	
	LIMIT_CHECK(REAL64, voltageReadList[0]);
	
	LIMIT_CHECK(REAL64, voltageReadList[2]);
	
	

Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_7_25)
{
    TESTCLEANUP_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4A", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4W", FALSE));
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_7_26
*********************************************************************************/

FCE_HELP(EA210, 40_7_26)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of PFM Output ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_7_26)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_7_26)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	//EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4U", TRUE));
	//Sleep(200);
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_7_26)
{
	ViReal64 NPLC;
	
	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);

	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViChar ErrorMessage[255];
	char msg[255]  ;
	
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	ViReal64 FreqRead;
 
	//	TEST : Test of PFM Output
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7080", "0001F4");	
	Sleep(100);
		
#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7180"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7180"))
	{
		LIMIT_CREATE("7180", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "0001F4"))
	{
		LIMIT_CREATE( "0001F4", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */

	//Switch Relays X4.2 and X4.3
	//Sleep(100);
//	Ag34970_VoltageSetAutoResistance (vi_AG34470A,206, VI_TRUE);
//	ErrorDMM= Ag34970_VoltageDCVoltageConfigure (vi_AG34470A, "206", 100.0, 0.00008);
//	if (ErrorDMM < VI_SUCCESS)
//	{
//		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
//		sprintf(msg, "Function Error : Ag34970_VoltageDCVoltageConfigure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
//		EXCTHROW( -1, msg);
//	}
   //
//	Sleep(100);
//	ErrorDMM=Ag34970_VoltageDCVoltageSetNPLC (vi_AG34470A,  206, 20);
//	ErrorDMM=Ag34970_SystemWaitForOperationComplete(vi_AG34470A,5000);

	ErrorDMM= Ag34970_MeasureDCVoltMeasure (vi_AG34470A, "206", 100.0, 0.00008, 1, &voltageRead, &buffersize);//0.001
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
	LIMIT_CHECK(REAL64, voltageRead);
	
	//ADD FREQ MEASUREMENT -------------------------------------------------------
		  ErrorDMM= Ag34970_MeasureFreqMeasure (vi_AG34470A, "206", 100, 10, 1, &FreqRead, &buffersize);
	   if (ErrorDMM < VI_SUCCESS)
		{
			Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34970_MeasureFreqMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
			EXCTHROW( -1, msg);
		}
		
		LIMIT_CHECK(REAL64, FreqRead);
		
	//-------------------------------------------------------
	

Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_7_26)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_7_27
*********************************************************************************/

FCE_HELP(EA210, 40_7_27)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of PFM Output ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_7_27)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_7_27)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4U", TRUE))	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_7_27)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);

	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViChar ErrorMessage[255];
	char msg[255]  ;
	
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	ViReal64 FreqRead;
 
	//	TEST : Test of PFM Output
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7080", "001388");
	Sleep(70);
#if 0    /* formerly excluded lines */
	do{
		cnt++;
		
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7080", "001388");
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7180"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7180"))
	{
		LIMIT_CREATE("7180", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "001388"))
	{
		LIMIT_CREATE( "001388", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	Sleep(300);
#endif   /* formerly excluded lines */

	ErrorDMM = Ag34970_MeasureDCVoltMeasure (vi_AG34470A, "206", 300.0, 0.001, 1, &voltageRead, &buffersize);
	//ErrorDMM = Ag34970_MeasureDCVoltMeasure (vi_AG34470A, "206", -105, -101, 1, &voltageRead, &buffersize);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
	LIMIT_CHECK(REAL64, voltageRead);
		
	//ADD FREQ MEASUREMENT -------------------------------------------------------
		  ErrorDMM = Ag34970_MeasureFreqMeasure (vi_AG34470A, "206", 1000, -103, 1, &FreqRead, &buffersize);
	   if (ErrorDMM < VI_SUCCESS)
		{
			Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34970_MeasureFreqMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
			EXCTHROW( -1, msg);
		}
		
		LIMIT_CHECK(REAL64, FreqRead);
				
	//-------------------------------------------------------

Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_7_27)
{
    TESTCLEANUP_BEGIN();
	TEST_USE(DBUS);
	pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7033", "010000");
    
Error:    
    TESTCLEANUP_END();
}


/*********************************************************************************
 * Function 40_7_28
*********************************************************************************/

FCE_HELP(EA210, 40_7_28)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of PFM Output ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_7_28)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_7_28)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_7_28)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	
	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViChar ErrorMessage[255];
	char msg[255]  ;
	
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	ViReal64 FreqRead;
 
	//	TEST : Test of PFM Output
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7080", "000000");
	
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7180"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7180"))
	{
		LIMIT_CREATE("7180", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "000000"))
	{
		LIMIT_CREATE( "000000", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}

	ErrorDMM = Ag34970_MeasureDCVoltMeasure (vi_AG34470A, "206", 100, 0.1, 1, &voltageRead, &buffersize);//0.01
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_MeasureDCVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
	LIMIT_CHECK(REAL64, voltageRead);
		
	//ADD FREQ MEASUREMENT -------------------------------------------------------

		  ErrorDMM = Ag34970_MeasureFreqMeasure (vi_AG34470A, "206", 100, 10, 1, &FreqRead, &buffersize);
	   if (ErrorDMM < VI_SUCCESS)
		{
			Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34970_MeasureFreqMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
			EXCTHROW( -1, msg);
		}

		//FreqRead=0;
		LIMIT_CHECK(REAL64, FreqRead);
	    		
	//-------------------------------------------------------

Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_7_28)
{
    TESTCLEANUP_BEGIN();
	TEST_USE(TESTER);
	//Ag34970_close(vi_AG34470A);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4U", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4S", TRUE));
    
Error:    
    TESTCLEANUP_END();
}

//----------------------------------------------------------------------------------------------------------------------------
/*********************************************************************************
 * Function 40_8_1
*********************************************************************************/

FCE_HELP(EA210, 40_8_1)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of relay output X2.1 on status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_8_1)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_8_1)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4S", TRUE));
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_8_1)
{
	ViInt32 count[50];
	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViChar ErrorMessage[255];
	char msg[512]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	int cnt=0;
	int MessageID_OK = 0;

 
	//	TEST : Test of relay output X2.1 on status
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
    pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7090", "0001");
	Sleep(200);
	
#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	
	do{
		cnt++;
        Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);

		if (!strcmp(MessageID, "7190"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7190"))
	{
		LIMIT_CREATE("7190", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "0001"))
	{
		LIMIT_CREATE( "0001", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */

	//Sleep(300); //OLD
	Sleep(1000); //ARAIBIA
	//Measure output signal at connector X2.1 Ag34970_VoltageACVoltageSetBandwidth (vi_AG34470A,101,20);

	ErrorDMM= Ag34970_MeasureACVoltMeasure (vi_AG34470A, "101", 300, 0.1, 1, &voltageRead, &buffersize);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_MeasureACVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
	
	LIMIT_CHECK(REAL64, voltageRead);
		
	
	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_8_1)
{
    TESTCLEANUP_BEGIN();
	TEST_USE(DBUS);
	
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_8_2
*********************************************************************************/

FCE_HELP(EA210, 40_8_2)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of relay output X2.1 off status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_8_2)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_8_2)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
    //Ag34970_VoltageACVoltageConfigure (vi_AG34470A, "101", 300, 0.001);
	//Ag34970_VoltageACVoltageSetBandwidth (vi_AG34470A, 101,20);
   //  Ag34970_VoltageACVoltageSetAutoRange (vi_AG34470A,101, VI_TRUE);
	// Ag34970_VoltageACVoltageSetRange (vi_AG34470A,101 , 300);

	// Ag34970_RouteSetAutoDelay (vi_AG34470A,101 , VI_TRUE);
	// Ag34970_RouteSetDelay (vi_AG34470A,101,0.5);

	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4S", TRUE));
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_8_2)
{
	ViChar voltage[50];

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViChar ErrorMessage[255];
	char msg[512]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	char command[128];
	int size=0;
	char BufferRead[128];
	STRING(byte1, 15+1);
 
	//	TEST : Test of relay output X2.1 off status
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7090", "0000");
	Sleep(100);
	
#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7190"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7190"))
	{
		LIMIT_CREATE("7190", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "0000"))
	{
		LIMIT_CREATE( "0000", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */
	

	
		ErrorDMM= Ag34970_MeasureACVoltMeasure (vi_AG34470A, "101", 300, 0.001, 1, &voltageRead, &buffersize);//300.0, 0.001
	//ErrorDMM = Ag34970_ScanRead (vi_AG34470A, 15, voltage);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_MeasureACVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
	//voltageRead=atof(voltage);
	
	//offset
	voltageRead=voltageRead+3;
	//limite check
	LIMIT_CHECK(REAL64, voltageRead);
		
	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
	TEST_END();
}

FCE_CLEANUP(EA210, 40_8_2)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_8_3
*********************************************************************************/

FCE_HELP(EA210, 40_8_3)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of relay output X2.2 on status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_8_3)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_8_3)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	//EXCCHECK( ptester->SetSensor(ptester, "CMD_DBUS_Relay_4R",           TRUE));
	//EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4S", FALSE));
	//Sleep(200);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4S", TRUE));
	//EXCCHECK( ptester->SetSensor(ptester, "CMD_PowerSupply_Relay_4H",    TRUE)) ;
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_8_3)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViChar ErrorMessage[255];
	char msg[512]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	char command[128];
	int size=0;
	char BufferRead[128];
	STRING(byte1, 15+1);
	
 
	//	TEST : Test of relay output X2.2 on status

	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7090", "0101");
	Sleep(100);

#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7190"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7190"))
	{
		LIMIT_CREATE("7190", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "0101"))
	{
		LIMIT_CREATE( "0101", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */
	
	ErrorDMM= Ag34970_MeasureACVoltMeasure (vi_AG34470A, "102", 300, 0.001,1, &voltageRead, &buffersize);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_MeasureACVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
	LIMIT_CHECK(REAL64, voltageRead);
		
	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_8_3)
{
    TESTCLEANUP_BEGIN();
	
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_8_4
*********************************************************************************/

FCE_HELP(EA210, 40_8_4)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of relay output X2.2 off status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_8_4)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_8_4)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	//	EXCCHECK( ptester->SetSensor(ptester, "CMD_DBUS_Relay_4R",           TRUE));
	//EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4S", FALSE));
	//Sleep(200);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4S", TRUE));
	//EXCCHECK( ptester->SetSensor(ptester, "CMD_PowerSupply_Relay_4H",    TRUE)) ;
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_8_4)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViChar ErrorMessage[255];
	char msg[512]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	int MessageData_OK=0;
	char command[128];
	int size=0;
	char BufferRead[128];
	STRING(byte1, 15+1);
	
 
	//	TEST : Test of relay output X2.2 off status
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7090", "0100");
	Sleep(100);

#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do
	{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);

		if (!strcmp(MessageID, "7190"))
			MessageID_OK = 1;

		if (MessageID_OK)
			break;

		if (cnt == 10)
			break;

	}
	while (TRUE);

	if (strcmp(MessageData, "0100"))
		Sleep(200);


	if (strcmp(MessageID, "7190"))
	{
		LIMIT_CREATE("7190", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "0100"))
	{
		LIMIT_CREATE( "0100", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */

    //Sleep(100);
	ErrorDMM= Ag34970_MeasureACVoltMeasure (vi_AG34470A, "102", 300, 0.001, 1, &voltageRead, &buffersize);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_MeasureACVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
	//offset
	voltageRead=voltageRead+3;
	//limite check
	LIMIT_CHECK(REAL64, voltageRead);
		
	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_8_4)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_8_5
*********************************************************************************/

FCE_HELP(EA210, 40_8_5)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of relay output X2.3 on status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_8_5)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_8_5)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	//EXCCHECK( ptester->SetSensor(ptester, "CMD_DBUS_Relay_4R",           TRUE));
	//EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4S", FALSE));
	//Sleep(200);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4S", TRUE));
	//EXCCHECK( ptester->SetSensor(ptester, "CMD_PowerSupply_Relay_4H",    TRUE)) ;
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_8_5)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViChar ErrorMessage[255];
	char msg[512]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	char command[128];
	int size=0;
	char BufferRead[128];
	STRING(byte1, 15+1);
	
 
	//	TEST : Test of relay output X2.3 on status
    
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7090", "0201");
	Sleep(100);

#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "7190"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7190"))
	{
		LIMIT_CREATE("7190", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "0201"))
	{
		LIMIT_CREATE( "0201", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */
		Sleep(1000);
	//ErrorDMM= Ag34970_MeasureACVoltMeasure (vi_AG34470A, "103", 300, 0.001, 1, &voltageRead, &buffersize);
	//Sleep(200);
	ErrorDMM= Ag34970_MeasureACVoltMeasure (vi_AG34470A, "103", 300, 0.001, 1, &voltageRead, &buffersize);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_MeasureACVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
	LIMIT_CHECK(REAL64, voltageRead);
		
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_8_5)
{
    TESTCLEANUP_BEGIN();
	
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_8_6
*********************************************************************************/

FCE_HELP(EA210, 40_8_6)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Test of relay output X2.3 off status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_8_6)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_8_6)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	char command[128];
	//EXCCHECK( ptester->SetSensor(ptester, "CMD_DBUS_Relay_4R",           TRUE));
	//EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4S", FALSE));
	//Sleep(200);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4S", TRUE));
	//EXCCHECK( ptester->SetSensor(ptester, "CMD_PowerSupply_Relay_4H",    TRUE)) ;
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_8_6)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViChar ErrorMessage[255];
	char msg[512]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	char command[128];
	int size=0;
	char BufferRead[128];
	STRING(byte1, 17+1);
	
 
	//	TEST : Test of relay output X2.3 off status
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "7090", "0200");//0x7090 0x02 0x01
	Sleep(100);
	
#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);

		//Sleep(200);
		if (!strcmp(MessageID, "7190"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt ==10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "7190"))
	{
		LIMIT_CREATE("7190", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "0200"))
	{
		LIMIT_CREATE( "0200", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */
	
	//Sleep(2000); //OLD 
	Sleep(500); //OLD ARAIBIA
	ErrorDMM= Ag34970_MeasureACVoltMeasure (vi_AG34470A, "103", 300, 0.001, 1, &voltageRead, &buffersize);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_MeasureACVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
	
	//offset demande par nizar
	voltageRead=voltageRead+3;
	//limite check	
	LIMIT_CHECK(REAL64, voltageRead);
	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_8_6)
{
    TESTCLEANUP_BEGIN();
	
    
Error:    
    TESTCLEANUP_END();
}




/*********************************************************************************
 * Function 40_8_7
*********************************************************************************/

FCE_HELP(EA210, 40_8_7)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Check triac output X3.2 on status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_8_7)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_8_7)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4T", TRUE));
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_8_7)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViChar ErrorMessage[255];
	char msg[512]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	char command[128];
	int size=0;
	char BufferRead[128];
	STRING(byte1, 15+1);
	
 
	//	TEST : Check triac output X3.2 on status
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "70B0", "0003");
	Sleep(100);

#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "71B0"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "71B0"))
	{
		LIMIT_CREATE("71B0", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "0003"))
	{
		LIMIT_CREATE( "0003", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */

	ErrorDMM= Ag34970_MeasureACVoltMeasure (vi_AG34470A, "104", 300, 0.001, 1, &voltageRead, &buffersize);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_MeasureACVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
	LIMIT_CHECK(REAL64, voltageRead);
	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_8_7)
{
    TESTCLEANUP_BEGIN();
	//TEST_USE(TESTER);
    //EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4T", FALSE));
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_8_8
*********************************************************************************/

FCE_HELP(EA210, 40_8_8)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Check triac output X3.2 off status ");
	
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_8_8)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_8_8)									
{
    ViChar charread[50];
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4T", TRUE));
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_8_8)
{
	ViReal64 voltage_0;
	ViChar voltagechar[50];

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViChar ErrorMessage[255];
	char msg[512]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	char command[128];
	int size=0;
	char BufferRead[128];
	STRING(byte1, 17+1);
	clock_t TIME0,TIME1;
  
 	TIME0=clock();
	//	TEST : Check triac output X3.2 off status
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "70B0", "0004");
	Sleep(100);
	
#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "71B0"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		if (cnt == 10) 
			break; 
		
		}while (TRUE);
	

	if (strcmp(MessageID, "71B0"))
	{
		LIMIT_CREATE("71B0", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "0004"))
	{
		LIMIT_CREATE( "0004", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */
	
	cnt=0;

	/*do
	{   cnt++;*/
		ErrorDMM= Ag34970_MeasureACVoltMeasure (vi_AG34470A, "104", 300,0.001, 1, &voltageRead, &buffersize);
		if (ErrorDMM < VI_SUCCESS)
		{
			Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34970_MeasureACVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
			EXCTHROW( -1, msg);
		}
		//printf("voltage =   %f\n  ",voltageRead);
		/*if(voltageRead<2)
			break;
	}
	while (cnt<2);*/
		
	/*printf("DBUS TIME=%d\n",clock()-TIME0);
	TIME1=clock();
	//Cls();
	cnt=0;
	do
	{
		cnt++;
		ErrorDMM = Ag34970_ScanRead (vi_AG34470A, 15, voltagechar);
		Sleep(5);

		printf("%s\n",voltagechar);
		if(atof(voltagechar)<1.9)
			break;

	}
	while (cnt<10);
	printf("\nREAD TIME=%d\n",clock()-TIME1);

	
	voltageRead=atof(voltagechar);*/
	
	LIMIT_CHECK(REAL64, voltageRead);
	
	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_8_8)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_8_9
*********************************************************************************/

FCE_HELP(EA210, 40_8_9)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Check triac output X3.3 on status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_8_9)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_8_9)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_8_9)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViChar ErrorMessage[255];
	char msg[512]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	char command[128];
	int size=0;
	char BufferRead[128];
	STRING(byte1, 15+1);
	
	
 
	//	TEST : Check triac output X3.3 on status
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "70B0", "0103");
	Sleep(100);
	
#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "71B0"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "71B0"))
	{
		LIMIT_CREATE("71B0", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "0103"))
	{
		LIMIT_CREATE( "0103", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */
	
	ErrorDMM= Ag34970_MeasureACVoltMeasure (vi_AG34470A, "105", 300, 0.001, 1, &voltageRead, &buffersize);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_MeasureACVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
	LIMIT_CHECK(REAL64, voltageRead);
	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_8_9)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_8_10
*********************************************************************************/

FCE_HELP(EA210, 40_8_10)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Check triac output X3.3 off status ");
	//Sleep(200);
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_8_10)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_8_10)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_8_10)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViChar ErrorMessage[255];
	char msg[512]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	char command[128];
	int size=0;
	char BufferRead[128];
	STRING(byte1, 15+1);
	
 
	//	TEST : Check triac output X3.3 off status
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "70B0", "0104");
	Sleep(100);
	
#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "71B0"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "71B0"))
	{
		LIMIT_CREATE("71B0", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "0104"))
	{
		LIMIT_CREATE( "0104", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */
#if 0    /* formerly excluded lines */
	
#endif   /* formerly excluded lines */
    	
	cnt=0;

	/*do
	{
		cnt++;*/
		ErrorDMM= Ag34970_MeasureACVoltMeasure (vi_AG34470A, "105", 300,0.001, 1, &voltageRead, &buffersize);
		if (ErrorDMM < VI_SUCCESS)
		{
			Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34970_MeasureACVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
			EXCTHROW( -1, msg);
		}
		//printf("voltage =   %f\n  ",voltageRead);
		/*if(voltageRead<2)
			break;
	}
	while (cnt<2);*/
	LIMIT_CHECK(REAL64, voltageRead);
	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_8_10)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}


/*********************************************************************************
 * Function 40_8_11
*********************************************************************************/

FCE_HELP(EA210, 40_8_11)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Check triac output X3.4 on status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_8_11)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_8_11)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	char command[128];

	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_8_11)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViChar ErrorMessage[255];
	char msg[512]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	char command[128];
	int size=0;
	char BufferRead[128];
	STRING(byte1, 15+1);
	
 
	//	TEST : Check triac output X3.4 on status
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "70B0", "0203");
	Sleep(100);
	
#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "71B0"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "71B0"))
	{
		LIMIT_CREATE("71B0", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "0203"))
	{
		LIMIT_CREATE( "0203", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */

	ErrorDMM= Ag34970_MeasureACVoltMeasure (vi_AG34470A, "106", 300, 0.001, 1, &voltageRead, &buffersize);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_MeasureACVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
	LIMIT_CHECK(REAL64, voltageRead);
	
	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_8_11)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_8_12
*********************************************************************************/

FCE_HELP(EA210, 40_8_12)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Check triac output X3.4 off status ");
	//Sleep(200);
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_8_12)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_8_12)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);	
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_8_12)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViChar ErrorMessage[255];
	char msg[512]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	char command[128];
	int size=0;
	char BufferRead[128];
	STRING(byte1, 15+1);
	
 
	//	TEST : Check triac output X3.4 off status
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "70B0", "0204");
	Sleep(100);

#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "71B0"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "71B0"))
	{
		LIMIT_CREATE("71B0", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "0204"))
	{
		LIMIT_CREATE( "0204", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */

	cnt=0;
	do
	{   cnt++;
		ErrorDMM= Ag34970_MeasureACVoltMeasure (vi_AG34470A, "106", 300,0.001, 1, &voltageRead, &buffersize);
		if (ErrorDMM < VI_SUCCESS)
		{
			Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34970_MeasureACVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
			EXCTHROW( -1, msg);
		}
		//printf("voltage =   %f\n  ",voltageRead);
		if(voltageRead<2)
			break;
	}
	while (cnt<20);
	
	LIMIT_CHECK(REAL64, voltageRead);
    
	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_8_12)
{
    TESTCLEANUP_BEGIN();

    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_8_13
*********************************************************************************/

FCE_HELP(EA210, 40_8_13)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Check triac output X3.5 on status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_8_13)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_8_13)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	char command[128];
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_8_13)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViChar ErrorMessage[255];
	char msg[512]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	char command[128];
	int size=0;
	char BufferRead[128];
	STRING(byte1, 15+1);
	
 
	//	TEST : Check triac output X3.5 on status
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
    pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "70B0", "0303");
	Sleep(100);
		
#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "71B0"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "71B0"))
	{
		LIMIT_CREATE("71B0", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "0303"))
	{
		LIMIT_CREATE( "0303", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */

	ErrorDMM= Ag34970_MeasureACVoltMeasure (vi_AG34470A, "107", 300, 0.001, 1, &voltageRead, &buffersize);
	if (ErrorDMM < VI_SUCCESS)
	{
		Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
		sprintf(msg, "Function Error : Ag34970_MeasureACVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
		EXCTHROW( -1, msg);
	}
	
	LIMIT_CHECK(REAL64, voltageRead);
    
	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_8_13)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_8_14
*********************************************************************************/

FCE_HELP(EA210, 40_8_14)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Check triac output X3.5 off status ");
	//Sleep(200);
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_8_14)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_8_14)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_8_14)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	TEST_USE(TESTER);
	ViInt32 buffersize;
	ViReal64 voltageRead;
	ViChar ErrorMessage[255];
	char msg[512]  ;
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	char command[128];
	int size=0;
	char BufferRead[128];
	STRING(byte1, 15+1);
	
 
	//	TEST : Check triac output X3.5 off status
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0x1B, "70B0", "0304");
	Sleep(100);

#if CHECK_ANSWER_UDA    /* formerly excluded lines */
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "71B0"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		if (cnt == 10) 
			break; 
		
	}while (TRUE);
	

	if (strcmp(MessageID, "71B0"))
	{
		LIMIT_CREATE("71B0", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "0304"))
	{
		LIMIT_CREATE( "0304", "MessageData");
		LIMIT_SET(STRING, MessageData);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
#endif   /* formerly excluded lines */

	cnt=0;
	do
	{   cnt++;
		ErrorDMM= Ag34970_MeasureACVoltMeasure (vi_AG34470A, "107", 300,0.001, 1, &voltageRead, &buffersize);
		if (ErrorDMM < VI_SUCCESS)
		{
			Ag34970_GetError (vi_AG34470A, &error_WS40, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34970_MeasureACVoltMeasure \r\n Error: %d, message error : %s\n", error_WS40, ErrorMessage);
			EXCTHROW( -1, msg);
		}
		//printf("voltage =   %f\n  ",voltageRead);
		if(voltageRead<2)
			break;
	}
	while (cnt<20);
	LIMIT_CHECK(REAL64, voltageRead);
	
	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_8_14)
{
    TESTCLEANUP_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4T", FALSE));
    
Error:    
    TESTCLEANUP_END();
}

//-----------------test NSMB STATION 30 -------------------------------------------
/*********************************************************************************
 * Function 40_3_2
*********************************************************************************/

FCE_HELP(EA210, 40_3_2)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Check SM status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_3_2)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_3_2)									
{
    TESTINIT_BEGIN();
	Sleep(300);
	
Error:    
    TESTINIT_END();
}
FCE_TEST(EA210, 40_3_2)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;
	
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	//Sleep(200);

	
	do{
		cnt++;
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0xA0, "F50E", "C0");
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
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
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_3_2)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_3_3
*********************************************************************************/

FCE_HELP(EA210, 40_3_3)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Check tracing ID of SMB ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_3_3)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_3_3)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_3_3)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;

	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0xA0, "600D", "C0");//A00D
	
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
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
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_3_3)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 40_3_4
*********************************************************************************/

FCE_HELP(EA210, 40_3_4)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Standby Mode Activation of SMB ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 40_3_4)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 40_3_4)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 40_3_4)
{

	TEST_BEGIN();
	TEST_USE(DBUS);
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	STRING(byte, 2+1);
	VARIABLE(INT32, valeur);
	int cnt=0;
	int MessageID_OK = 0;

    EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_40));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_40, 0xA0, "600C", "C0");
	
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_40, 0xC0, MessageID, MessageData);
		
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
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_40));
    TEST_END();
}

FCE_CLEANUP(EA210, 40_3_4)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

