#include <ansi_c.h>
#include "EA210_utils.h"
#include "toolbox.h"
#include <formatio.h>
#include <multithread/multithread.h>
#include <advanlys.h>
#include "test_panel.h"
#include "EA210_Test_WS20.h"

double time_station_20;

DEFINE_GLOBAL_VARIABLE();

#define TESTER_PLUG_PANEL_STATION_20(timeout) \
	EXCCHECK( ptester->PlugPanelStation20(ptester, timeout))
	
#define TESTER_UNPLUG_PANEL_STATION_20(timeout) \
	EXCCHECK( ptester->UnPlugPanelStation20(ptester, timeout))

#define TESTER_CONNECT_RELAIS_TEST_0_TO_3750V() \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_2B", TRUE)) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_2C", TRUE)) \ 
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_2D", FALSE)) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_2E", FALSE)) 

#define TESTER_CONNECT_RELAIS_TEST_0_TO_1750V() \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_2C", FALSE)) \ 
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_2E", FALSE)) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_2B", TRUE)) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_2D", TRUE)) 


#define TESTER_CONNECT_RELAIS_TEST_0_TO_750V() \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_2B", FALSE)) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_2C", FALSE)) \ 
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_2D", TRUE)) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_2E", TRUE)) 

#define TESTER_DISCONNECT_ALL() \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_2B", FALSE)) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_2C", FALSE)) \ 
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_2D", FALSE)) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_Relay_2E", FALSE)) 


STestListItem gs_TestList_Station_20[] = {

	/* STATION 20 */
	ADD_TI( "20.0.0", EA210, 20_0_0),	/* TESTER : Connect Product */
	ADD_TI( "20.0.1", EA210, 20_0_1),	/* TESTER : Disconnect Product */		
	ADD_TI( "20.1.0", EA210, 20_1_1),	/* safety Tester : PCB- isolation voltage test */
	ADD_TI( "20.1.1", EA210, 20_1_2),	/* safety Tester : High voltage test FE – N / L */
	ADD_TI( "20.1.2", EA210, 20_1_3),	/* safety Tester : High voltage test FE – SELV */
	

		
	ADD_TI_LAST_ENTRY
};


	
int CVICALLBACK OnTimerWS20 (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{	
	char            stime[32];
	
	switch (event)
	{
		case EVENT_TIMER_TICK:
			
			time_station_20 = time_station_20 + 0.1;
			
			sprintf (stime, "(%0.1fs)", time_station_20);
			SetCtrlVal( gs_TestPanel, PANEL_TIME_WS20, stime); 

			break;
	}
	return 0;
}
/*********************************************************************************
 * Function Testsequence Init
 *********************************************************************************/
FCE_TESTSEQUENCE_INIT(STATION_20)
{
	TESTSEQUENCEINIT_BEGIN();
	TEST_USE(TESTER);
	TEST_USE(DIELECTRIMETER);	
	time_station_20 = 0.0;
	
	/* Open connection */
	//pdielectrimeter->Connect(pdielectrimeter);
	
	SetCtrlAttribute (gs_TestPanel, PANEL_TIMER_WS20, ATTR_ENABLED, TRUE);

	PANEL_INI();
	
	TESTER_DISCONNECT_ALL();
	Sleep(1000);
	//printf("init WS20 \n\r ");	
	
Error:
	if (pexception) SetCtrlAttribute (gs_TestPanel, PANEL_TIMER_WS20,ATTR_ENABLED, FALSE);
	TESTSEQUENCEINIT_END();
}

/*********************************************************************************
 * Function Testsequence Cleanup
 *********************************************************************************/
FCE_TESTSEQUENCE_CLEANUP(STATION_20)
{
	
	TESTSEQUENCECLEANUP_BEGIN(); 
	TEST_USE(TESTER);
	TESTER_INIT ();
	TEST_USE(DIELECTRIMETER);
	TESTER_DISCONNECT_ALL()

	/* Close connection */ 
	//pdielectrimeter->Disconnect(pdielectrimeter);
	//printf("cleanup WS20 \n\r ");
Error:
	
	SetCtrlAttribute (gs_TestPanel, PANEL_TIMER_WS20, ATTR_ENABLED, FALSE);
	DISPLAY_TESTSEQ_RESULT ();
	TESTSEQUENCECLEANUP_END();

	
}

/*********************************************************************************
 * Function Testsequence Wait
 *********************************************************************************/
FCE_TESTSEQUENCE_WAIT(STATION_20)
{
	TESTSEQUENCEWAIT_BEGIN(); 
	TEST_USE(TESTER);

	WAIT_TABLE_ROTATION_FINISHED(TIMEOUT_TABLE);
	
Error:
	TESTSEQUENCEWAIT_END();
}

/*********************************************************************************
 * Function 20_0_0
*********************************************************************************/

FCE_HELP(EA210, 20_0_0)
{
    HELP_BEGIN();
	HELP_TEST( "TESTER : Connect product");

Error:
    HELP_END();
}

FCE_CHECK(EA210, 20_0_0)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE( TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 20_0_0)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 20_0_0)
{
    TEST_BEGIN();
	TEST_USE(TESTER);
	Sleep(500);
	TESTER_PLUG_PANEL_STATION_20(5000);
	Sleep(700);
	TESTER_CONNECT_RELAIS_TEST_0_TO_3750V();
	Sleep(700);

Error:
    TEST_END();
}

FCE_CLEANUP(EA210, 20_0_0)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}


/*********************************************************************************
 * Function 20_0_1
*********************************************************************************/

FCE_HELP(EA210, 20_0_1)
{
    HELP_BEGIN();
	HELP_TEST( "TESTER : Deconnection produit");
Error:
    HELP_END();
}

FCE_CHECK(EA210, 20_0_1)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE( TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 20_0_1)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 20_0_1)
{
    TEST_BEGIN();
	TEST_USE(TESTER);
	
	TESTER_UNPLUG_PANEL_STATION_20(TIMEOUT_DEFAULT);


Error:
    TEST_END();
}

FCE_CLEANUP(EA210, 20_0_1)
{
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 20_1_1
*********************************************************************************/

FCE_HELP(EA210, 20_1_1)
{
    HELP_BEGIN();
	HELP_TEST( "RS-PRO IIT200 : PCB- isolation voltage test ");
Error:
    HELP_END();
}

FCE_CHECK(EA210, 20_1_1)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE( TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 20_1_1)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	//TESTER_CONNECT_RELAIS_TEST_0_TO_3750V();
	Sleep(400);
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 20_1_1)
{
    TEST_BEGIN();
	TEST_USE(TESTER);
	TEST_USE(DIELECTRIMETER);
	char sys_error[1024];
	double voltage ;
	double current ;
	int    result ;
	double time;
	//clock_t 		_start_test_clock;
	//printf("----------station 20 test en cours------------ \n");
	INITIALIZE_DIELECTRIMETER (0);
	
	/* Open connection */
	pdielectrimeter->Connect(pdielectrimeter);
	
	/* Read Errors */
	pdielectrimeter->SystemError(pdielectrimeter, sys_error);
	
	/* Rest Errors */
	pdielectrimeter->ClearError(pdielectrimeter);
	
	/* Mode Manu */
	pdielectrimeter->SetModeManual(pdielectrimeter);
		Sleep(100);
	/* Select Step */
	pdielectrimeter->SetStep(pdielectrimeter,1);
	
	/* Output ON */
		Sleep(200);
	
	pdielectrimeter->SetOutput(pdielectrimeter,1);
	Sleep(2000);

	/* Measurement */ 
	pdielectrimeter->TestResult(pdielectrimeter, &result, &voltage, &current);
	pdielectrimeter->SetOutput(pdielectrimeter,0);
	Sleep(100);
	LIMIT_CHECK(REAL64, current);	


Error:
	
	/* Close connection */ 
	pdielectrimeter->Disconnect(pdielectrimeter);
    TEST_END();
}

FCE_CLEANUP(EA210, 20_1_1)
{
    TESTCLEANUP_BEGIN();
	TEST_USE(TESTER);
	TESTER_DISCONNECT_ALL();
    
Error:    
    TESTCLEANUP_END();
}
/*********************************************************************************
 * Function 20_1_2
*********************************************************************************/

FCE_HELP(EA210, 20_1_2)
{
    HELP_BEGIN();
	HELP_TEST( "RS-PRO IIT200 : High voltage test FE – N / L ");
Error:
    HELP_END();
}

FCE_CHECK(EA210, 20_1_2)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE( TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 20_1_2)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	Sleep(100);
	TESTER_CONNECT_RELAIS_TEST_0_TO_1750V();
	Sleep(200);
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 20_1_2)
{
    TEST_BEGIN();
	TEST_USE(TESTER);
	TEST_USE(DIELECTRIMETER);
	char sys_error[1024];
	double voltage ;
	double current ;
	int result ;
	
	INITIALIZE_DIELECTRIMETER (0);
	/* Open connection */
	pdielectrimeter->Connect(pdielectrimeter);
	
	/* Read Errors */
	pdielectrimeter->SystemError(pdielectrimeter, sys_error);
	
	/* Rest Errors */
	pdielectrimeter->ClearError(pdielectrimeter);
	
	/* Mode Manu */
	pdielectrimeter->SetModeManual(pdielectrimeter);
	
	/* Select Step */
	pdielectrimeter->SetStep(pdielectrimeter,3);
	
	/* Output ON */
	pdielectrimeter->SetOutput(pdielectrimeter,1);
	Sleep(2000);
	/* Measurement */ 
	pdielectrimeter->TestResult(pdielectrimeter, &result, &voltage, &current);
	pdielectrimeter->SetOutput(pdielectrimeter,0);
	
	LIMIT_CHECK(REAL64, current );	
	 
	

Error:
	
	/* Close connection */ 
	pdielectrimeter->Disconnect(pdielectrimeter);
    TEST_END();
}

FCE_CLEANUP(EA210, 20_1_2)
{
    TESTCLEANUP_BEGIN();
	TEST_USE(TESTER);
	TESTER_DISCONNECT_ALL();
    
Error:    
    TESTCLEANUP_END();
}
/*********************************************************************************
 * Function 20_1_3
*********************************************************************************/

FCE_HELP(EA210, 20_1_3)
{
    HELP_BEGIN();
	HELP_TEST( "RS-PRO IIT200 : High voltage test FE – SELV ");
Error:
    HELP_END();
}

FCE_CHECK(EA210, 20_1_3)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE( TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 20_1_3)									
{
    TESTINIT_BEGIN();
	TEST_USE(TESTER);
	TESTER_CONNECT_RELAIS_TEST_0_TO_750V()
	Sleep(100);
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 20_1_3)
{

    TEST_BEGIN();
	TEST_USE(TESTER);
	TEST_USE(DIELECTRIMETER);
	char sys_error[1024];
	double voltage ;
	double current ;
	int result ;
	
	INITIALIZE_DIELECTRIMETER (0);
	
	/* Open connection */
	pdielectrimeter->Connect(pdielectrimeter);
	
	/* Read Errors */
	pdielectrimeter->SystemError(pdielectrimeter, sys_error);
	
	/* Rest Errors */
	pdielectrimeter->ClearError(pdielectrimeter);
	
	/* Mode Manu */
	pdielectrimeter->SetModeManual(pdielectrimeter);
	
	/* Select Step */
	pdielectrimeter->SetStep(pdielectrimeter,5);
	
	/* Output ON */
	pdielectrimeter->SetOutput(pdielectrimeter,1);
	Sleep(2000);
	/* Measurement */ 
	pdielectrimeter->TestResult(pdielectrimeter, &result, &voltage, &current);
	pdielectrimeter->SetOutput(pdielectrimeter,0);
	
	LIMIT_CHECK(REAL64, current );	
	 

Error:
	
	/* Close connection */ 
	pdielectrimeter->Disconnect(pdielectrimeter);
    TEST_END();
}

FCE_CLEANUP(EA210, 20_1_3)
{
    TESTCLEANUP_BEGIN();
	TEST_USE(TESTER);
	Sleep(200);
	TESTER_DISCONNECT_ALL();
    
Error:    
    TESTCLEANUP_END();
}
