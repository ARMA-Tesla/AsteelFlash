#include <ansi_c.h>
#include "EA210_utils.h"
#include "toolbox.h"
#include <formatio.h>
#include <multithread/multithread.h>
#include <rs232.h>
#include "Ag34450.h"
#include <advanlys.h>
#include "test_panel.h"
#include "EA210_Test_WS50.h"

double time_station_50;

ViInt32   ErrorAG34450;
ViRsrc    AG34450AresourceNameWS50 = "USB0::0x2A8D::0xB318::MY59300128::INSTR";
ViSession vi_Ag34450_WS50=0;
ViStatus  ErrorCode ;
int ErrorPVS=1;
ViChar    ErrorMessage[512];
ViStatus  error_WS50               = VI_SUCCESS;
char msg[512];


#define UDA_NAME_STATION_50 	"UDA (USB-SN: UDA-05)"
DEFINE_GLOBAL_VARIABLE();

/* Connexion Verin */

#define TESTER_PLUG_PANEL_STATION_50(timeout) \
	EXCCHECK( ptester->PlugPanelStation50(ptester, timeout))

#define TESTER_UNPLUG_PANEL_STATION_50(timeout) \
	EXCCHECK( ptester->UnplugPanelStation50(ptester, timeout))

/* Deconnexion tous les relais*/

#define TESTER_DISCONNECT_ALL_STATION_50(timeout) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5C",                FALSE)) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5D",                FALSE)) \ 
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5E",                FALSE)) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5F",                FALSE)) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5I",              	 FALSE)) \  
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5L",              	 FALSE)) \  
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5M",                FALSE)) \  
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5N",                FALSE)) \  
	EXCCHECK( ptester->SetSensor(ptester, "CMD_DBUS_Relay_5H",           FALSE)) \  
	EXCCHECK( ptester->SetSensor(ptester, "CMD_PowerSupply_Relay_5G",    FALSE)) \  
    EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4T",                FALSE)); //MOVED FROM WS40 TO WS 5O REALY TRIAC ACTIVATION

STestListItem gs_TestList_Station_50[] = {

	/* STATION 50 */
	ADD_TI( "50.0.0",   EA210, 50_0_0 ),		/* TEST : PLUG PRODUCT*/
	ADD_TI( "50.0.1",   EA210, 50_0_1 ),		/* TEST : UNPLUG PRODUCT*/
	ADD_TI( "50.0.2",   EA210, 50_0_2 ),		/* TEST : HW TEST MODE DBUS*/
	ADD_TI( "50.8.7",   EA210, 50_8_7 ),		/* TEST : TRIAC X3.2 ON*/
	ADD_TI( "50.8.8",   EA210, 50_8_8 ),		/* TEST : TRIAC X3.2 OFF*/
	ADD_TI( "50.8.9",   EA210, 50_8_9 ),		/* TEST : TRIAC X3.3 ON*/
	ADD_TI( "50.8.10",  EA210, 50_8_10),		/* TEST : TRIAC X3.3 OFF*/
	ADD_TI( "50.8.11",  EA210, 50_8_11),		/* TEST : TRIAC X3.4 ON*/
	ADD_TI( "50.8.12",  EA210, 50_8_12),		/* TEST : TRIAC X3.4 OFF*/
	ADD_TI( "50.8.13",  EA210, 50_8_13),		/* TEST : TRIAC X3.5 ON*/
	ADD_TI( "50.8.14",  EA210, 50_8_14),		/* TEST : TRIAC X3.5 OFF*/
	ADD_TI( "50.10.1",  EA210, 50_10_1),		/* TEST : X capacitor discharge Control + ON*/
	ADD_TI( "50.10.2",  EA210, 50_10_2),		/* TEST : X capacitor discharge Control + OFF*/
	ADD_TI( "50.10.3",  EA210, 50_10_3),		/* TEST : X capacitor discharge Control - ON*/
	ADD_TI( "50.10.4",  EA210, 50_10_4),		/* TEST : X capacitor discharge Control - OFF*/
	ADD_TI( "50.11.1",  EA210, 50_11_1),		/* TEST : Power Off */

	ADD_TI_LAST_ENTRY
};

int CVICALLBACK OnTimerWS50 (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{	
	char            stime[32];
	
	switch (event)
	{
		case EVENT_TIMER_TICK:
			
			time_station_50 = time_station_50 + 0.1;
			
			sprintf (stime, "(%0.1fs)", time_station_50);
			SetCtrlVal( gs_TestPanel, PANEL_TIME_WS50, stime); 

			break;
	}
	return 0;
}

/*********************************************************************************
 * Function Testsequence Init
 *********************************************************************************/
FCE_TESTSEQUENCE_INIT(STATION_50)
{  
	    char command[50];
	    char BufferRead[50];
		
		char msg[128];
		
	    TESTSEQUENCEINIT_BEGIN();
		time_station_50 = 0.0;
	
		SetCtrlAttribute (gs_TestPanel, PANEL_TIMER_WS50, ATTR_ENABLED, TRUE);
		
	    TEST_USE(TESTER);
	    PANEL_INI();
		TESTER_DISCONNECT_ALL_STATION_50();
		//EXCCHECK( ptester->SetSensor(ptester, "CMD_DBUS_Relay_5H",       FALSE));//DBUS
		
	
			ErrorPVS = OpenComConfig (6, "COM6",57600, 0, 8, 1, 512, 512);
			if (ErrorPVS < 0)
			{
				sprintf(msg, "Function Error : Open COM6 PVS FAIL!! \r\n");
				EXCTHROW( -1, msg);
			}
			FlushInQ(6);
			FlushOutQ(6);
			sprintf (command,"*RST\n");
			ErrorPVS=ComWrt (6,command , strlen (command));
			if (ErrorPVS < 0)
			{
				sprintf(msg, "Function Error : Reset PVS FAIL!! \r\n");
				EXCTHROW( -1, msg);
			}
			sprintf (command,"CURRent 1\n");//3A
			ErrorPVS=ComWrt (6,command , strlen (command));
			if (ErrorPVS < 0)
			{
				sprintf(msg, "Function Error : PVS set Current FAIL!! \r\n");
				EXCTHROW( -1, msg);
			}

			sprintf (command,"VOLTage 340\n");
			ErrorPVS = ComWrt (6,command , strlen (command));
			if (ErrorPVS < 0)
			{
				sprintf(msg, "Function Error : PVS set Voltage FAIL!! \r\n");
				EXCTHROW( -1, msg);
			}
			sprintf (command,"OCP 1\n");
			ErrorPVS = ComWrt (6,command , strlen (command));
			if (ErrorPVS < VI_SUCCESS)
			{
				sprintf(msg, "Function Error : PVS set Over Current Protection FAIL!! \r\n");
				EXCTHROW( -1, msg);
			}
			sprintf (command,"OISET 2\n");
			ErrorPVS = ComWrt (6,command , strlen (command));
			if (ErrorPVS < 0)
			{
				sprintf(msg, "Function Error : PVS set Over Current Protection value FAIL!! \r\n");
				EXCTHROW( -1, msg);
			}
			sprintf (command,"OVP 1\n");
			ErrorPVS = ComWrt (6,command , strlen (command));
			if (ErrorPVS < 0)
			{
				sprintf(msg, "Function Error : PVS set Over Voltage Protection FAIL!! \r\n");
				EXCTHROW( -1, msg);
			}
			sprintf (command,"OVSET 400\n");
			ErrorPVS = ComWrt (6,command , strlen (command));
			if (ErrorPVS < 0)
			{
				sprintf(msg, "Function Error : PVS set Over Voltage Protection value FAIL!! \r\n");
				EXCTHROW( -1, msg);
			}

#if 0    /* formerly excluded lines */
			sprintf (command,"VOLTage?\n");
			ErrorPVS = ComWrt (6,command , strlen (command));
			ErrorPVS = ComRd (6, BufferRead, 10);
#endif   /* formerly excluded lines */
     
		// Initiate the keysight and create a session ---------------------------//
		//if(vi_Ag34450_WS50 ==0)
		//{
		ErrorAG34450 = Ag34450_init (AG34450AresourceNameWS50, 0, 1, &vi_Ag34450_WS50);
		//printf("agilent init1 \n");
		if (ErrorAG34450 < VI_SUCCESS)
		{
			ErrorAG34450 = Ag34450_init (AG34450AresourceNameWS50, 0, 1, &vi_Ag34450_WS50);
			//printf("agilent init2 \n");
			if (ErrorAG34450 < VI_SUCCESS)
			{
				Ag34450_GetError(vi_Ag34450_WS50, &ErrorCode, 255, ErrorMessage);
				sprintf(msg, "Function Error : Ag34450_init \r\n Error: %d, message error : %s\n", ErrorCode, ErrorMessage);
				EXCTHROW( -1, msg);
			}
		}
		Ag34450_ClearError(vi_Ag34450_WS50);
		Sleep(100);
			
		//}
		//printf("init WS50 \n\r ");

Error:
	if (pexception) 	
	SetCtrlAttribute (gs_TestPanel, PANEL_TIMER_WS50,ATTR_ENABLED, FALSE);
	TESTSEQUENCEINIT_END();
}

/*********************************************************************************
 * Function Testsequence Cleanup
 *********************************************************************************/
FCE_TESTSEQUENCE_CLEANUP(STATION_50)
{
	
		TESTSEQUENCECLEANUP_BEGIN();
		TEST_USE(TESTER);
		
        Ag34450_reset (vi_Ag34450_WS50);
		Ag34450_close (vi_Ag34450_WS50);
		TESTER_DISCONNECT_ALL_STATION_50();
	    TESTER_UNPLUG_PANEL_STATION_50(5000);
		//TESTER_INIT ();
		//printf("agilent init1 \n");
		ErrorPVS = CloseCom (6);
			if (ErrorPVS < 0)
			{
				sprintf(msg, "Function Error : Close COM6 PVS FAIL!! \r\n");
				EXCTHROW( -1, msg);
			}
		
		//printf("cleanup WS50 \n\r ");	
Error:
	SetCtrlAttribute (gs_TestPanel, PANEL_TIMER_WS50, ATTR_ENABLED, FALSE);
	DISPLAY_TESTSEQ_RESULT ();
	TESTSEQUENCECLEANUP_END();

}

/*********************************************************************************
 * Function Testsequence Wait
 *********************************************************************************/
FCE_TESTSEQUENCE_WAIT(STATION_50)
{
	TESTSEQUENCEWAIT_BEGIN(); 
	TEST_USE(TESTER);

	WAIT_TABLE_ROTATION_FINISHED(TIMEOUT_TABLE);
	
Error:
	TESTSEQUENCEWAIT_END();
}



//
/*********************************************************************************
 * Function 
50_0_0
*********************************************************************************/

FCE_HELP(EA210, 50_0_0)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : PLUG PRODUCT ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 50_0_0)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 50_0_0)									
{
    TESTINIT_BEGIN();	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 50_0_0)
{
	TEST_BEGIN();
	TEST_USE(TESTER);
	ViChar ErrorMessage[512];
	char   msg[512];
	ViInt32 Readsize;

	pexception = ptester->PlugPanelStation50(ptester, 5000);
	if (pexception)
	{
		TESTER_PLUG_PANEL_STATION_50(5000);
	}
	EXCCHECK( ptester->SetSensor(ptester, "CMD_PowerSupply_Relay_5G",FALSE));//POWER SUPPLY
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_4T",            TRUE));//CONNECT RESISTANCES
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5I",            TRUE));

	Sleep(200);
Error:
	TEST_END();
}

FCE_CLEANUP(EA210, 50_0_0)
{
    TESTCLEANUP_BEGIN();
Error:    
    TESTCLEANUP_END();
}
/*********************************************************************************
 * Function 50_0_1
*********************************************************************************/

FCE_HELP(EA210, 50_0_1)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : PLUG PRODUCT & init instrument");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 50_0_1)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 50_0_1)									
{
    TESTINIT_BEGIN();	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 50_0_1)
{
	TEST_BEGIN();
	TEST_USE(TESTER);
	TEST_USE(DBUS);

	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_50, 0x1B, "70B0", "0004");
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_50, 0x1B, "70B0", "0104");
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_50, 0x1B, "70B0", "0204");
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_50, 0x1B, "70B0", "0304");

	TESTER_DISCONNECT_ALL_STATION_50();
	TESTER_UNPLUG_PANEL_STATION_50(5000);

Error:
	TEST_END();
}

FCE_CLEANUP(EA210, 50_0_1)
{
    TESTCLEANUP_BEGIN();
Error:    
    TESTCLEANUP_END();
}
/*********************************************************************************
 * Function 50_0_2
*********************************************************************************/

FCE_HELP(EA210, 50_0_2)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : DBUS ENTER HARDWARE TEST MODE");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 50_0_2)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 50_0_2)									
{
    TESTINIT_BEGIN();	
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_PowerSupply_Relay_5G",TRUE));//POWER SUPPLY
	EXCCHECK( ptester->SetSensor(ptester, "CMD_DBUS_Relay_5H",       TRUE));//DBUS
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 50_0_2)
{
	TEST_BEGIN();
	TEST_USE(TESTER);
	TEST_USE(DBUS);
	STRING(MessageID, 128);
	STRING(MessageData, 128);
	int cnt=0;
	int MessageID_OK = 0;
	//Start HW test Mode
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_50));

	do
	{
		cnt++;
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_50, 0x1B, "7000", "23C0");
		Sleep(200);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_50, 0xC0, MessageID, MessageData);

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
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_50));
	TEST_END();
}

FCE_CLEANUP(EA210, 50_0_2)
{
    TESTCLEANUP_BEGIN();
Error:    
    TESTCLEANUP_END();
}




/*********************************************************************************
 * Function 50_10_1
*********************************************************************************/

FCE_HELP(EA210, 50_10_1)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : X capacitor discharge CONTROL + ON");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 50_10_1)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 50_10_1)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	TESTER_DISCONNECT_ALL_STATION_50();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 50_10_1)
{   TEST_BEGIN();
	
	TEST_USE(TESTER);
	ViInt32 Readsize;	
	ViChar ErrorMessage[50];
	SElExceptionPtr	pexc_first = NULL;
	VARIABLE(UINT32, cnt);
    ViReal64  voltage = 0.000;
	ViInt32 ErrorCode;
	cnt = 0;
	int ErrorPVS;
	char command[128],
		 msg[256];
	
	
		EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5C", TRUE));	
		EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5F", TRUE));
		
		/*OUTPUT 340VDC ON*/
		Sleep(100);
		sprintf (command,"OUT 1\n");
	    ErrorPVS = ComWrt (6,command , strlen (command));
		if (ErrorPVS < 0)
		{						
			sprintf(msg, "Function Error : PVS set Output Enable FAIL!! \r\n");
			EXCTHROW( -1, msg);
		}
		/*CONFIG DMM DC MEASUREMENT */
		ErrorAG34450 = Ag34450_DCVoltageConfigure(vi_Ag34450_WS50, 100, AG34450_VAL_RESOLUTION_MEDIUM);
		if (ErrorAG34450 < VI_SUCCESS)
		{
			Ag34450_GetError(vi_Ag34450_WS50, &ErrorCode, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34450_DCVoltageConfigure \r\n Error: %d, message error : %s\n", ErrorCode, ErrorMessage);
			EXCTHROW( -1, msg);
		}
		Sleep(200);	
		 
    	/*Read DC voltage 20.4 VDc*/		
		do 
		{
			cnt++;
			EXCDELETE(pexc_first);
			ErrorAG34450 = Ag34450_Read (vi_Ag34450_WS50, 8000, &voltage);
			if (ErrorAG34450 < VI_SUCCESS)
			{
				Ag34450_GetError(vi_Ag34450_WS50, &ErrorCode, 255, ErrorMessage);
				sprintf(msg, "Function Error : Ag34450_Read \r\n Error: %d, message error : %s\n", ErrorCode, ErrorMessage);
				EXCTHROW( -1, msg);
			}
	
			LIMIT_CHECK_EXT(REAL64, (float)voltage/0.06, pexception);//20.4VDC correspond à 340 VDC 
			if(!pexc_first) 
			pexc_first = pexception;
			if(pexc_first)
			Sleep(100);
		
		}while(pexc_first && cnt < 10);
			//LIMIT_CHECK(REAL64, voltage);	
    					
Error:

    TEST_END();
	/*EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5C", FALSE)); 
	  EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5D", FALSE));	
	  EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5E", FALSE));*/
	
}

FCE_CLEANUP(EA210, 50_10_1)
{
    TESTCLEANUP_BEGIN(); 
  
Error:    
    TESTCLEANUP_END();
}
/*********************************************************************************
 * Function 50_10_2
*********************************************************************************/

FCE_HELP(EA210, 50_10_2)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : X capacitor discharge Control + OFF ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 50_10_2)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 50_10_2)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 50_10_2)
{   TEST_BEGIN();
	char command[128];
	TEST_USE(TESTER);
	ViInt32 Readsize;
	char   msg[256];
	ViChar ErrorMessage[50];
	SElExceptionPtr	pexc_first = NULL;
	VARIABLE(UINT32, cnt);
    ViReal64  voltage = 0.000;
	ViInt32 ErrorCode;
	cnt = 0;
	int ErrorPVS;
	
		/*“Control +” off*/
		/*OUTPUT 340VDC OFF*/
		sprintf (command,"OUT 0\n");
	    ErrorPVS = ComWrt (6,command , strlen (command));
		if (ErrorPVS < 0)
		{						
			sprintf(msg, "Function Error : PVS set Output Enable FAIL!! \r\n");
			EXCTHROW( -1, msg);
		}
		EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5C", FALSE));
	    Sleep(1000);	//DO NOT CHANGE TIME 1s
		/*Read DC voltage 2.04 correspond à 34 VDC*/
		cnt = 0;
		do 
		{
			cnt++;
			EXCDELETE(pexc_first);
			ErrorAG34450 = Ag34450_Read (vi_Ag34450_WS50, 8000, &voltage);
			if (ErrorAG34450 < VI_SUCCESS)
			{
				Ag34450_GetError(vi_Ag34450_WS50, &ErrorCode, 255, ErrorMessage);
				sprintf(msg, "Function Error : Ag34450_Read \r\n Error: %d, message error : %s\n", ErrorCode, ErrorMessage);
				EXCTHROW( -1, msg);
			}
	       //voltage=2;
			LIMIT_CHECK_EXT(REAL64, ((float)voltage/0.06), pexception);
			if(!pexc_first) 
			pexc_first = pexception;
			if(pexc_first)
			Sleep(100);
		
		}while(pexc_first && cnt< 5);
		//LIMIT_CHECK(REAL64, voltage);
		/*OUTPUT 340VDC OFF*/ 	
		sprintf (command,"OUT 0\n");
	    ErrorPVS = ComWrt (6,command , strlen (command));
		if (ErrorPVS < 0)
		{						
			sprintf(msg, "Function Error : PVS set Output Enable FAIL!! \r\n");
			EXCTHROW( -1, msg);
		}
		EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5F", FALSE));
		EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5D", FALSE));
		EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5C", FALSE));
		EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5F", FALSE));
		Sleep(200);
		//Discharge the PCB
		EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5E", TRUE)); 
		Sleep(500);
		EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5E", FALSE));
		Sleep(200);
    			
		
Error:

    TEST_END();
	/*EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5C", FALSE)); 
	  EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5D", FALSE));	
	  EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5E", FALSE));*/
	
}

FCE_CLEANUP(EA210, 50_10_2)
{
    TESTCLEANUP_BEGIN(); 
  
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 50_11_1
*********************************************************************************/

FCE_HELP(EA210, 50_11_1)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Switch off ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 50_11_1)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 50_11_1)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 50_11_1)
{   TEST_BEGIN();
	TEST_USE(TESTER);	
		//Disconnect all relays of test 
		EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5C",FALSE));
		EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5D",FALSE));
		EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5E",FALSE));
		EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5F",FALSE));
		
	/*----------------------------- Disconnect cylinder ----------------------------------------------*/
		EXCCHECK( ptester->UnplugPanelStation50(ptester, 6000));		
Error:
    TEST_END();
}

FCE_CLEANUP(EA210, 50_11_1)
{
    TESTCLEANUP_BEGIN(); 
    
Error:    
    TESTCLEANUP_END();
}
/*********************************************************************************
 * Function 50_10_3
*********************************************************************************/

FCE_HELP(EA210, 50_10_3)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : X capacitor discharge Control -");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 50_10_3)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 50_10_3)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5F", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5D", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5C", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5F", FALSE)); 
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 50_10_3)
{   TEST_BEGIN();
	char command[128];
	char   msg[256];
	TEST_USE(TESTER);
	ViInt32 Readsize;
	ViChar ErrorMessage[50];
	SElExceptionPtr	pexc_first = NULL;
	VARIABLE(UINT32, cnt);
    ViReal64  voltage = 0.000;
	ViInt32 ErrorCode;
	cnt = 0;
	int ErrorPVS;	
	
	   /* Switch Relays*/
		EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5C", FALSE)); 		
		EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5D", TRUE)); 		
		EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5F", TRUE)); 
		Sleep(200);
	    /*OUTPUT 340VDC ON*/
		sprintf (command,"OUT 1\n");
	    ErrorPVS = ComWrt (6,command , strlen (command));
		if (ErrorPVS < 0)
		{						
			sprintf(msg, "Function Error : PVS set Output Enable FAIL!! \r\n");
			EXCTHROW( -1, msg);
		}
		Sleep(500);
		
			/*CONFIG DMM DC MEASUREMENT */
		ErrorAG34450 = Ag34450_DCVoltageConfigure(vi_Ag34450_WS50, 100, AG34450_VAL_RESOLUTION_MEDIUM);
		if (ErrorAG34450 < VI_SUCCESS)
		{
			Ag34450_GetError(vi_Ag34450_WS50, &ErrorCode, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34450_DCVoltageConfigure \r\n Error: %d, message error : %s\n", ErrorCode, ErrorMessage);
			EXCTHROW( -1, msg);
		}
		Sleep(200);	
		//Read DC voltage 
		cnt = 0;
		do 
		{
			cnt++;
			EXCDELETE(pexc_first);
			ErrorAG34450 = Ag34450_Read (vi_Ag34450_WS50, 5000, &voltage);
			if (ErrorAG34450 < VI_SUCCESS)
			{
				Ag34450_GetError(vi_Ag34450_WS50, &ErrorCode, 255, ErrorMessage);
				sprintf(msg, "Function Error : Ag34450_Read \r\n Error: %d, message error : %s\n", ErrorCode, ErrorMessage);
				EXCTHROW( -1, msg);
			}
	
			LIMIT_CHECK_EXT(REAL64, -(float)voltage/0.06, pexception);
			if(!pexc_first) 
			pexc_first = pexception;
			if(pexc_first)
			Sleep(200);
		
		}while(pexc_first && cnt< 5);
		
Error:

    TEST_END();
	/*EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5C", FALSE)); 
	  EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5D", FALSE));	
	  EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5E", FALSE));*/
	//Ag34450_close (vi_Ag34450_WS50);
	
}

FCE_CLEANUP(EA210, 50_10_3)
{
    TESTCLEANUP_BEGIN(); 
	//Ag34450_close (vi_Ag34450_WS50);
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 50_10_4
*********************************************************************************/

FCE_HELP(EA210, 50_10_4)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : X capacitor discharge Control -");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 50_10_4)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE(TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 50_10_4)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5F", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5D", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5C", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5F", FALSE)); 
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 50_10_4)
{   TEST_BEGIN();
	char command[128];
	char   msg[256];
	TEST_USE(TESTER);
	ViInt32 Readsize;
	ViChar ErrorMessage[50];
	SElExceptionPtr	pexc_first = NULL;
	VARIABLE(UINT32, cnt);
    ViReal64  voltage = 0.000;
	ViInt32 ErrorCode;
	cnt = 0;
	int ErrorPVS;	
	
	    /*“Control -” off	*/
		sprintf (command,"OUT 0\n");
	    ErrorPVS = ComWrt (6,command , strlen (command));
		if (ErrorPVS < 0)
		{						
			sprintf(msg, "Function Error : PVS set Output Enable FAIL!! \r\n");
			EXCTHROW( -1, msg);
		}
		EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5D", FALSE));
		Sleep(1000);// do not change time

		cnt = 0;
		do 
		{
			cnt++;
			EXCDELETE(pexc_first);
			ErrorAG34450 = Ag34450_Read (vi_Ag34450_WS50, 5000, &voltage);
			if (ErrorAG34450 < VI_SUCCESS)
			{
				Ag34450_GetError(vi_Ag34450_WS50, &ErrorCode, 255, ErrorMessage);
				sprintf(msg, "Function Error : Ag34450_Read \r\n Error: %d, message error : %s\n", ErrorCode, ErrorMessage);
				EXCTHROW( -1, msg);
			}
	
			LIMIT_CHECK_EXT(REAL64, -(float)voltage/0.06, pexception);
			if(!pexc_first) 
			pexc_first = pexception;
			if(pexc_first)
			Sleep(100);
		
		}while(pexc_first && cnt< 5);
		sprintf (command,"OUT 0\n");
	    ErrorPVS = ComWrt (6,command , strlen (command));
		if (ErrorPVS < 0)
		{						
			sprintf(msg, "Function Error : PVS set Output Enable FAIL!! \r\n");
			EXCTHROW( -1, msg);
		}
		EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5F", FALSE));
		EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5D", FALSE));
		EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5C", FALSE));
		Sleep(300);	
		//Discharge the PCB
		EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5E", TRUE)); 	
		Sleep(300);
		EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5E", FALSE)); 			
		
Error:

    TEST_END();
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5C", FALSE)); 
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5D", FALSE));	
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5E", FALSE));
	
}

FCE_CLEANUP(EA210, 50_10_4)
{
    TESTCLEANUP_BEGIN(); 
Error:    
    TESTCLEANUP_END();
}
/*-----------------
 		TRIAC TESTS
        ----------------------*/

/*********************************************************************************
 * Function 50_8_7
*********************************************************************************/

FCE_HELP(EA210, 50_8_7)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Check triac output X3.2 on status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 50_8_7)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 50_8_7)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_PowerSupply_Relay_5G",TRUE));//POWER SUPPLY
	EXCCHECK( ptester->SetSensor(ptester, "CMD_DBUS_Relay_5H",       TRUE));//DBUS
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5M",            FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5I",            TRUE));//TRIAC X3.2 MOD1 P0.5
	Sleep(300);//500
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 50_8_7)
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
	cnt=0;
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_50));
	//Sleep(300);//TEST
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_50, 0x1B, "70B0", "0003");
	
	do{
		cnt++;
		Sleep(100);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_50, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "71B0"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
	}while (cnt<40);
	

	if (strcmp(MessageID, "71B0"))
	{
		LIMIT_CREATE("71B0", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "0003"))
		Sleep(200);//400

	ErrorAG34450 = Ag34450_ACVoltageMeasure (vi_Ag34450_WS50, 750,AG34450_VAL_RESOLUTION_MEDIUM , &voltageRead);
	if (ErrorAG34450 < VI_SUCCESS)
	{		Sleep(200);//400
		Ag34450_reset (vi_Ag34450_WS50);
		ErrorAG34450 = Ag34450_ACVoltageMeasure (vi_Ag34450_WS50, 750,AG34450_VAL_RESOLUTION_MEDIUM , &voltageRead);
		if (ErrorAG34450 < VI_SUCCESS)
		{
			Ag34450_GetError (vi_Ag34450_WS50, &error_WS50, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34450_ACVoltageMeasure \r\n Error: %d, message error : %s\n", error_WS50, ErrorMessage);
			EXCTHROW( -1, msg);
		}
	}
	

	LIMIT_CHECK(REAL64, voltageRead);

Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_50));
    TEST_END();
}

FCE_CLEANUP(EA210, 50_8_7)
{
    TESTCLEANUP_BEGIN();
	TEST_USE(TESTER);
    EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5I", FALSE));
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 50_8_8
*********************************************************************************/

FCE_HELP(EA210, 50_8_8)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Check triac output X3.2 off status ");
	
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 50_8_8)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 50_8_8)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_PowerSupply_Relay_5G",TRUE));//POWER SUPPLY
	EXCCHECK( ptester->SetSensor(ptester, "CMD_DBUS_Relay_5H",       TRUE));//DBUS
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5M", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5N", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5L", FALSE));
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5I", TRUE)); //X3.2
	Sleep(500);//500
	//printf("TEST : Check triac output X3.2 off status init \n");
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 50_8_8)
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

 
	//	TEST : Check triac output X3.2 off status
	
		EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_50));
#if 0    /* formerly excluded lines */
	do{
		cnt++;
		
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_50, 0x1B, "70B0", "0004");
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_50, 0xC0, MessageID, MessageData);
		
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
	
	Ag34450_ACVoltageConfigure (vi_Ag34450_WS50, 100, AG34450_VAL_RESOLUTION_MEDIUM);
	Sleep(200);
	//ErrorAG34450 = Ag34450_Read (vi_Ag34450_WS50, 8000, &voltageRead);
	ErrorAG34450 = Ag34450_ACVoltageMeasure (vi_Ag34450_WS50, 100,AG34450_VAL_RESOLUTION_MEDIUM , &voltageRead);
	if (ErrorAG34450 < VI_SUCCESS)
	{   
		Ag34450_ClearError (vi_Ag34450_WS50);
	    ErrorAG34450 = Ag34450_ACVoltageMeasure (vi_Ag34450_WS50, 100,AG34450_VAL_RESOLUTION_MEDIUM  , &voltageRead);
		if (ErrorAG34450 < VI_SUCCESS)
		{
			Ag34450_GetError (vi_Ag34450_WS50, &error_WS50, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34450_Read \r\n Error: %d, message error : %s\n", error_WS50, ErrorMessage);
			EXCTHROW( -1, msg);
		}
	}

	LIMIT_CHECK(REAL64, voltageRead)


	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_50));
    TEST_END();
}

FCE_CLEANUP(EA210, 50_8_8)
{
    TESTCLEANUP_BEGIN();
	TEST_USE(TESTER);
    EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5I", FALSE));
    //printf("TEST : Check triac output X3.2 off status cleanup \n");
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 50_8_9
*********************************************************************************/

FCE_HELP(EA210, 50_8_9)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Check triac output X3.3 on status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 50_8_9)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 50_8_9)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
    EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5L", TRUE));
	Sleep(400);
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 50_8_9)
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
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_50));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_50, 0x1B, "70B0", "0103");
	
	do{
		cnt++;
		Sleep(100);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_50, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "71B0"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		
		
	}while (cnt<20);
	

	if (strcmp(MessageID, "71B0"))
	{
		LIMIT_CREATE("71B0", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "WRONG MessageID");
	}
	
	if (strcmp(MessageData, "0103"))
	{
		LIMIT_CREATE("0103", "MessageData");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "WRONG MessageData");
		
	}		
	Sleep(300);

	ErrorAG34450 = Ag34450_ACVoltageMeasure (vi_Ag34450_WS50, 750,AG34450_VAL_RESOLUTION_MEDIUM , &voltageRead);
	if (ErrorAG34450 < VI_SUCCESS)
	{
		Ag34450_ClearError (vi_Ag34450_WS50);
		ErrorAG34450 = Ag34450_ACVoltageMeasure (vi_Ag34450_WS50, 750,AG34450_VAL_RESOLUTION_MEDIUM , &voltageRead);
		if (ErrorAG34450 < VI_SUCCESS)
		{
			Ag34450_GetError (vi_Ag34450_WS50, &error_WS50, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34450_ACVoltageMeasure \r\n Error: %d, message error : %s\n", error_WS50, ErrorMessage);
			EXCTHROW( -1, msg);
		}
	}
	
	
	LIMIT_CHECK(REAL64, voltageRead);

Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_50));
    TEST_END();
}

FCE_CLEANUP(EA210, 50_8_9)
{
    TESTCLEANUP_BEGIN();
	TEST_USE(TESTER);
    EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5L", FALSE));
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 50_8_10
*********************************************************************************/

FCE_HELP(EA210, 50_8_10)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Check triac output X3.3 off status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 50_8_10)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 50_8_10)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5I", FALSE));
    EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5L", TRUE));
	Sleep(400);//400
	//printf("TEST : Check triac output X3.3 off status init \n");
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 50_8_10)
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
		EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_50));
#if 0    /* formerly excluded lines */
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_50));
	do{
		cnt++;
		
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_50, 0x1B, "70B0", "0104");
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_50, 0xC0, MessageID, MessageData);
		
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
	//ErrorAG34450 = Ag34450_SystemWaitForOperationComplete (vi_Ag34450_WS50, 3000);
	//ErrorAG34450 = Ag34450_ACVoltageMeasure (vi_Ag34450_WS50, 10,AG34450_VAL_RESOLUTION_MEDIUM , &voltageRead);
	ErrorAG34450 = Ag34450_ACVoltageMeasure (vi_Ag34450_WS50, 100,AG34450_VAL_RESOLUTION_MEDIUM , &voltageRead);
	if (ErrorAG34450 < VI_SUCCESS)
	{	
		Ag34450_ClearError (vi_Ag34450_WS50);
		ErrorAG34450 = Ag34450_ACVoltageMeasure (vi_Ag34450_WS50, 100,AG34450_VAL_RESOLUTION_MEDIUM , &voltageRead);
		if (ErrorAG34450 < VI_SUCCESS)
		{
			Ag34450_GetError (vi_Ag34450_WS50, &error_WS50, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34450_Read \r\n Error: %d, message error : %s\n", error_WS50, ErrorMessage);
			EXCTHROW( -1, msg);
		}
	}
	
	LIMIT_CHECK(REAL64, voltageRead);

Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_50));
    TEST_END();
}

FCE_CLEANUP(EA210, 50_8_10)
{
    TESTCLEANUP_BEGIN();
	TEST_USE(TESTER);
    EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5L", FALSE));
    //printf("TEST : Check triac output X3.3 off status cleanup \n");
Error:    
    TESTCLEANUP_END();
}


/*********************************************************************************
 * Function 50_8_11
*********************************************************************************/

FCE_HELP(EA210, 50_8_11)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Check triac output X3.4 on status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 50_8_11)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 50_8_11)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5L", FALSE));
    EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5M", TRUE));
    Sleep(200);//400
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 50_8_11)
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
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_50));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_50, 0x1B, "70B0", "0203");
	
	do{
		cnt++;
		Sleep(100);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_50, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "71B0"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
	}while (cnt<20);
	

	if (strcmp(MessageID, "71B0"))
	{
		LIMIT_CREATE("71B0", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "0203"))
		Sleep(200);
	
	ErrorAG34450 = Ag34450_ACVoltageMeasure (vi_Ag34450_WS50, 750,AG34450_VAL_RESOLUTION_MEDIUM , &voltageRead);
	if (ErrorAG34450 < VI_SUCCESS)
	{   
		Ag34450_ClearError (vi_Ag34450_WS50);
		ErrorAG34450 = Ag34450_ACVoltageMeasure (vi_Ag34450_WS50, 750,AG34450_VAL_RESOLUTION_MEDIUM , &voltageRead);
		if (ErrorAG34450 < VI_SUCCESS)
		{  
			Ag34450_GetError (vi_Ag34450_WS50, &error_WS50, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34450_ACVoltageMeasure \r\n Error: %d, message error : %s\n", error_WS50, ErrorMessage);
			EXCTHROW( -1, msg);
		}
	}
	
	LIMIT_CHECK(REAL64, voltageRead);

	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_50));
    TEST_END();
}

FCE_CLEANUP(EA210, 50_8_11)
{
    TESTCLEANUP_BEGIN();
	TEST_USE(TESTER);
    EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5M", FALSE));
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 50_8_12
*********************************************************************************/

FCE_HELP(EA210, 50_8_12)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Check triac output X3.4 off status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 50_8_12)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 50_8_12)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5L", FALSE));
    EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5M", TRUE));
	Sleep(300);//300
	//printf("TEST : Check triac output X3.4 off status init \n");
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 50_8_12)
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
		EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_50));
#if 0    /* formerly excluded lines */
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_50));
	do{
		cnt++;
		
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_50, 0x1B, "70B0", "0204");
		Sleep(300);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_50, 0xC0, MessageID, MessageData);
		
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
	
	
	ErrorAG34450 = Ag34450_ACVoltageMeasure (vi_Ag34450_WS50, 100,AG34450_VAL_RESOLUTION_MEDIUM , &voltageRead);
	if (ErrorAG34450 < VI_SUCCESS)
	{	
		Ag34450_ClearError (vi_Ag34450_WS50);
		ErrorAG34450 = Ag34450_ACVoltageMeasure (vi_Ag34450_WS50, 100,AG34450_VAL_RESOLUTION_MEDIUM , &voltageRead);
		if (ErrorAG34450 < VI_SUCCESS)
		{
			Ag34450_GetError (vi_Ag34450_WS50, &error_WS50, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34450_Read \r\n Error: %d, message error : %s\n", error_WS50, ErrorMessage);
			EXCTHROW( -1, msg);
		}
	}
	
	LIMIT_CHECK(REAL64, voltageRead);

	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_50));
    TEST_END();
}

FCE_CLEANUP(EA210, 50_8_12)
{
    TESTCLEANUP_BEGIN();
	TEST_USE(TESTER);
    EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5M", FALSE));
	//printf("TEST : Check triac output X3.4 off status cleanup \n");
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 50_8_13
*********************************************************************************/

FCE_HELP(EA210, 50_8_13)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Check triac output X3.5 on status ");
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 50_8_13)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 50_8_13)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
    EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5N", TRUE));
	Sleep(200);//300
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 50_8_13)
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
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_50));
	pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_50, 0x1B, "70B0", "0303");
	
	do{
		cnt++;
		Sleep(70);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_50, 0xC0, MessageID, MessageData);
		
		if (!strcmp(MessageID, "71B0"))
			MessageID_OK = 1;
	
		if (MessageID_OK)
			break;
		
		
		
	}while (cnt<20);
	

	if (strcmp(MessageID, "71B0"))
	{
		LIMIT_CREATE("71B0", "MessageID");
		LIMIT_SET(STRING, MessageID);
		EXCTHROW(TEST_ERR_VALUE_OUT_OF_LIMIT, "TEST_ERR_VALUE_OUT_OF_LIMIT");
	}
	
	if (strcmp(MessageData, "0303"))
		Sleep(200);
	
	ErrorAG34450 = Ag34450_ACVoltageMeasure (vi_Ag34450_WS50, 750,AG34450_VAL_RESOLUTION_MEDIUM , &voltageRead);
	if (ErrorAG34450 < VI_SUCCESS)
	{
		Ag34450_ClearError (vi_Ag34450_WS50);
		ErrorAG34450 = Ag34450_ACVoltageMeasure (vi_Ag34450_WS50, 750,AG34450_VAL_RESOLUTION_MEDIUM , &voltageRead);
		if (ErrorAG34450 < VI_SUCCESS)
		{
			Ag34450_GetError (vi_Ag34450_WS50, &error_WS50, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34450_ACVoltageMeasure \r\n Error: %d, message error : %s\n", error_WS50, ErrorMessage);
			EXCTHROW( -1, msg);
		}
	}
	
	
	LIMIT_CHECK(REAL64, voltageRead);

	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_50));
    TEST_END();
}

FCE_CLEANUP(EA210, 50_8_13)
{
    TESTCLEANUP_BEGIN();
	TEST_USE(TESTER);
    EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5N", FALSE));
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 50_8_14
*********************************************************************************/

FCE_HELP(EA210, 50_8_14)
{
    HELP_BEGIN();
	HELP_TEST( " TEST : Check triac output X3.5 off status ");
	//Sleep(200);
	
Error:
    HELP_END();
}

FCE_CHECK(EA210, 50_8_14)
{
    TESTREGUIRE_BEGIN();
	REQUIRE(DBUS);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 50_8_14)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5M", FALSE));
    EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5N", TRUE));
	Sleep(500);//300
	//printf("TEST : Check triac output X3.5 off status init \n");
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 50_8_14)
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
	
 	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_50));
	//	TEST : Check triac output X3.5 off status
#if 0    /* formerly excluded lines */
	EXCCHECK( pdbus->ClearCache(pdbus, UDA_NAME_STATION_50));
	do{
		cnt++;
		
		pexception = pdbus->WriteLowLevel(pdbus, UDA_NAME_STATION_50, 0x1B, "70B0", "0304");
		Sleep(20);
		pexception = pdbus->ReadLowLevel(pdbus, UDA_NAME_STATION_50, 0xC0, MessageID, MessageData);
		
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
	

	ErrorAG34450 = Ag34450_ACVoltageMeasure (vi_Ag34450_WS50, 100,AG34450_VAL_RESOLUTION_MEDIUM , &voltageRead);
	if (ErrorAG34450 < VI_SUCCESS)
	{   
		Ag34450_ClearError (vi_Ag34450_WS50);
	    ErrorAG34450 = Ag34450_ACVoltageMeasure (vi_Ag34450_WS50, 100,AG34450_VAL_RESOLUTION_MEDIUM  , &voltageRead);
		if (ErrorAG34450 < VI_SUCCESS)
		{
			Ag34450_GetError (vi_Ag34450_WS50, &error_WS50, 255, ErrorMessage);
			sprintf(msg, "Function Error : Ag34450_Read \r\n Error: %d, message error : %s\n", error_WS50, ErrorMessage);
			EXCTHROW( -1, msg);
		}
	}
	
	LIMIT_CHECK(REAL64, voltageRead);
	
Error:
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, "Log", pdbus->GetCache(pdbus, UDA_NAME_STATION_50));
    TEST_END();
}

FCE_CLEANUP(EA210, 50_8_14)
{
    TESTCLEANUP_BEGIN();
	TEST_USE(TESTER);
    EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_5N", FALSE));
    //printf("TEST : Check triac output X3.5 off status CLEANUP \n");
Error:    
    TESTCLEANUP_END();
}

