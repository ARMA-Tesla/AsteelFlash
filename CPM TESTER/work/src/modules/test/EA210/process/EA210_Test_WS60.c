#include <ansi_c.h>
#include "EA210.h"
#include "EA210_utils.h"
#include "toolbox.h"
#include <formatio.h>
#include <multithread/multithread.h>
#include "test_panel.h"
#include "EA210_Test_WS60.h"
#include <exception/Exception.h> 
#include <test/testseq.h>


double time_station_60;

#define TESTER_CONVOYER(status) \
	EXCCHECK( ptester->SetSensor(ptester, "CMD_CONVEYOR", status? 1:0)) \


#define PRINTER_CAB  	0
	
#define TESTER_PRINTER_AIR(status) \
	EXCCHECK( ptester->SetSensor(ptester, "PRINTER_AIR_VALVE_ACTIVATE", status? 1:0)) \
	EXCCHECK( ptester->SetSensor(ptester, "PRINTER_AIR_VALVE_DESACTIVATE", status? 0:1)) \

#define TIMEOUT_INIT_CONVOYER() 	clock_t	_start_clock = clock()
#define TIMEOUT_CHECK_CONVOYER(timeout) \
	if ( ((clock() - _start_clock) / (double)CLOCKS_PER_SEC) > ((double)timeout/1000.0) ) { EXCTHROW( TEST_ERR_TIMEOUT, "Timeout Convoyer"); } else
	
	
#define TIMEOUT_INIT_PRINTER() 	clock_t	_start_clock = clock()
#define TIMEOUT_RESET_BUFFER_PRINTER(timeout) \
	if ( ((clock() - _start_clock) / (double)CLOCKS_PER_SEC) > ((double)timeout/1000.0) ) { EXCTHROW( TEST_ERR_TIMEOUT, "Timeout Reset Buffer"); } else
	
#define TIMEOUT_SEND_BUFFER_PRINTER(timeout) \
	if ( ((clock() - _start_clock) / (double)CLOCKS_PER_SEC) > ((double)timeout/1000.0) ) { EXCTHROW( TEST_ERR_TIMEOUT, "Timeout Envoi Label Buffer"); } else

extern SElEventFactoryPtr	gs_EventFactory;

DEFINE_GLOBAL_VARIABLE();

STestListItem gs_TestList_Station_60[] = {
	
	ADD_TI( "60.00", EA210, 60_00),	/* Reset Buffer Imprimante */
	ADD_TI( "60.10", EA210, 60_10),	/* Initialisation Imprimante */
	ADD_TI( "60.20", EA210, 60_20),	/* Envoi Etiquette Imprimante */
	ADD_TI( "60.30", EA210, 60_30),	/* Pick Panel And Go to Printer */
	ADD_TI( "60.40", EA210, 60_40),	/* Start Cycle Print */
	ADD_TI( "60.50", EA210, 60_50),	/* X move & place Product front DATAMAN */
	ADD_TI( "60.60", EA210, 60_60),	/* Read barcode */
	ADD_TI( "60.70", EA210, 60_70),	/* PLACE PRODUCT TO PANEL */
	
	
	ADD_TI_LAST_ENTRY
};

int CVICALLBACK OnTimerWS60 (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{	
	char            stime[32];
	
	switch (event)
	{
		case EVENT_TIMER_TICK:
			
			time_station_60 = time_station_60 + 0.1;
			
			sprintf (stime, "(%0.1fs)", time_station_60);
			SetCtrlVal( gs_TestPanel, PANEL_TIME_WS60, stime); 

			break;
	}
	return 0;
}
/*********************************************************************************
 * Function Testsequence Init
 *********************************************************************************/
FCE_TESTSEQUENCE_INIT(STATION_60)
{
	TESTSEQUENCEINIT_BEGIN();
	TEST_USE(TESTER);
	StatusStations[4] = BUSY; 
	
	int ProductInConvoyer=0;
    time_station_60 = 0.0;
	SetCtrlAttribute (gs_TestPanel, PANEL_TIMER_WS60, ATTR_ENABLED, TRUE);
	PANEL_INI();
	SetCtrlAttribute(PANEL, PANEL_CONVOYER_MSG, ATTR_VISIBLE, FALSE);
	//
	/* home pick */

	if (((STestParamPtr)pTID)->pexception) //piéce FAIL
	{
		if (((STestParamPtr)pTID)->Origin_Fault != 60)
		{
			do
			{
				EXCCHECK( ptester->GetSensor(ptester, "SENSOR_CONV_IN",&ProductInConvoyer ))
				TESTER_CONVOYER(TRUE);
				if(ProductInConvoyer==0)
				{
					TESTER_CONVOYER(FALSE);
					break;
				}
				/* ajouter un timeout */
				TIMEOUT_CHECK_CONVOYER(10000);
			}
			while (TRUE);
			PickAndPlace = 1;
			Sleep(2000);
			EXCCHECK(ptester->PickPlacePanelStation60(ptester, 30000));
			_start_clock = clock();
			do
			{
				EXCCHECK( ptester->GetSensor(ptester, "SENSOR_CONV_IN",&ProductInConvoyer ))
				TESTER_CONVOYER(TRUE);
				if(ProductInConvoyer==0)
				{
					TESTER_CONVOYER(FALSE);
					break;
				}
				/* ajouter un timeout */
				TIMEOUT_CHECK_CONVOYER(10000);
			}
			while (TRUE);
			PickAndPlace = 0;
		}
	}
	//
	
Error:
	if (pexception) 	SetCtrlAttribute (gs_TestPanel, PANEL_TIMER_WS60,ATTR_ENABLED, FALSE);
	TESTSEQUENCEINIT_END();
}

/*********************************************************************************
 * Function Testsequence Cleanup
 *********************************************************************************/
FCE_TESTSEQUENCE_CLEANUP(STATION_60)
{
	TESTSEQUENCECLEANUP_BEGIN(); 
	SetCtrlAttribute (gs_TestPanel, PANEL_TIMER_WS60, ATTR_ENABLED, FALSE);
	TEST_USE(TESTER);
	TEST_USE(TRACA);
	int ProductInConvoyer=0;
	char   Date_Debut[32+1];
	int    Year, Month, Day, Hour, Minute, Second;
	DISPLAY_TESTSEQ_RESULT ();


	/* home pick */
	if (((STestParamPtr)pTID)->pexception) //piéce FAIL
	{	 
  		/* Insertion mouvement FCT-12 */
		INITIALIZE_TRACA(0);
		sprintf(((STestParamPtr)pTID)->Num_Serie_pFin, "SKR_%s%s",((STestParamPtr)pTID)->Board_Ver_pFin,((STestParamPtr)pTID)->Serial_Board_Ver_sFin);
		sscanf(((STestParamPtr)pTID)->Time, "%02d.%02d.%02d", &Hour, &Minute, &Second);
		sscanf(((STestParamPtr)pTID)->Date, "%04d.%02d.%02d", &Year, &Month, &Day);
		sprintf(Date_Debut, "%04d-%02d-%02d %02d:%02d:%02d.000", Year, Day, Month, Hour, Minute, Second);//2020-22-08 22:00:00.000
		pexception = ptraca->Set_Last_Mouvement(ptraca,((STestParamPtr)pTID)->Num_Serie_pFin,((STestParamPtr)pTID)->id_Board_pFin, Date_Debut, "","TESTER_FCT12", "FCT", (((STestParamPtr)pTID)->pexception == NULL)? 1:0);

		if (pexception)
		{
			((STestParamPtr)pTID)->pexception = pexception;
			printf("erreur set movement \n");
			EXCTHROW (-1, "erreur insertion mouvement !");

		}
			/* association entre sFin et pFin */
		pexception = ptraca->Set_Association(ptraca,((STestParamPtr)pTID)->Num_Serie_sFin, ((STestParamPtr)pTID)->Num_Serie_pFin, "", Date_Debut);
		if (pexception)
		{
			((STestParamPtr)pTID)->pexception = pexception;
			EXCTHROW (-1, "erreur association entre NumSerie sFin et NumSerie pFin !");
		}		
			/* home pick */
		if (((STestParamPtr)pTID)->Origin_Fault == 60)
		{
			_start_clock = clock();
			do{
				EXCCHECK( ptester->GetSensor(ptester, "SENSOR_CONV_IN",&ProductInConvoyer ))
				TESTER_CONVOYER(TRUE); //1 step convoyer
				if(ProductInConvoyer==0)
					{
						TESTER_CONVOYER(FALSE);
						break;
					}
					/* ajouter un timeout */
				TIMEOUT_CHECK_CONVOYER(10000);
			}while (TRUE);
			
			EXCCHECK(ptester->HomePickPlacePanelStation60(ptester, 20000));//if in position printer or barcode put in convoyer and come home
			
			_start_clock = clock();
			do{
				EXCCHECK( ptester->GetSensor(ptester, "SENSOR_CONV_IN",&ProductInConvoyer ))
				TESTER_CONVOYER(TRUE); //1 step convoyer
				if(ProductInConvoyer==0)
					{
						TESTER_CONVOYER(FALSE);
						break;
					}
					/* ajouter un timeout */
				TIMEOUT_CHECK_CONVOYER(10000);
			}while (TRUE);
		}
				
		//else 
		//{
		//	_start_clock = clock();
		//	do{
		//		EXCCHECK( ptester->GetSensor(ptester, "SENSOR_CONV_IN",&ProductInConvoyer ))
		//		TESTER_CONVOYER(TRUE);
		//			if(ProductInConvoyer==0)
		//			{
		//				TESTER_CONVOYER(FALSE);
		//				break;
		//			}
		//			/* ajouter un timeout */
		//			TIMEOUT_CHECK_CONVOYER(10000);
		//	}while (TRUE);
		//	Sleep(2000);
		//	EXCCHECK(ptester->PickPlacePanelStation60(ptester, 30000));
		//	_start_clock = clock();
		//	do{
		//		EXCCHECK( ptester->GetSensor(ptester, "SENSOR_CONV_IN",&ProductInConvoyer ))
		//		TESTER_CONVOYER(TRUE);
		//			if(ProductInConvoyer==0)
		//			{
		//				TESTER_CONVOYER(FALSE);
		//				break;
		//			}
		//			/* ajouter un timeout */
		//			TIMEOUT_CHECK_CONVOYER(10000);
		//	}while (TRUE);
		//}
		
	}
	
	TESTER_INIT ();


Error:
	if (pexception)
	{
		ptester->SetSensor(ptester, "CMD_CONVEYOR", 0);
		SetCtrlAttribute(PANEL, PANEL_CONVOYER_MSG, ATTR_VISIBLE, TRUE);
		SetCtrlVal(gs_Panel[station], PANEL_CONVOYER_MSG, "ERROR CONVOYER !");
		DISPLAY_TESTSEQ_RESULT ();
	}
	StatusStations[4] = NOT_BUSY; 
	TESTSEQUENCECLEANUP_END();
}

/*********************************************************************************
 * Function Testsequence Wait
 *********************************************************************************/
FCE_TESTSEQUENCE_WAIT(STATION_60)
{
	TESTSEQUENCEWAIT_BEGIN(); 
	TEST_USE(TESTER);

	WAIT_TABLE_ROTATION_FINISHED(TIMEOUT_TABLE);
	
Error:
	TESTSEQUENCEWAIT_END();
}
/*********************************************************************************
 * Function 60_00
*********************************************************************************/

FCE_HELP(EA210, 60_00)
{
    HELP_BEGIN();
    HELP_TEST( "Reset buffer Printer" );

Error:
    HELP_END();
}

FCE_CHECK(EA210, 60_00)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE( TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 60_00)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 60_00)
{
    TEST_BEGIN();
	TEST_USE(TESTER);
	TEST_USE(PRINTERS);
	int 			iStatus;
	clock_t 		_start_clock;
	double   		time;	
	int				StatusOnline = -1;
	int				Interpreter = -1;
	int				AmmountLabels = -1;
	char			Error[128] = "";
	char			sAmmount[128] = "";
    int      		status_barrier;
	char			msg[512] = "";
	
	INITIALIZE_PRINTER(PRINTER_CAB);
	
	/* Electovalve AIR PRINTER */
	TESTER_PRINTER_AIR (TRUE);
		
#if 0    /* formerly excluded lines */
	/* Connect */
	pexception = printer->Connect(printer);
	if (pexception)
	{
		MessagePopup("Erreur Imprimante", "Erreur Connection Imprimante");
		EXCCHECK(pexception);
	}
#endif   /* formerly excluded lines */
	
	/* Reset */
	pexception = printer->Reset(printer);
	if (pexception)
	{
		MessagePopup("Erreur Imprimante", "Erreur Reset Buffer Imprimante");
		EXCCHECK(pexception);
	}	
	
	/* make reset Printer */
	_start_clock =  clock();
	do{

		Sleep(200); //TEST old  Sleep(200);
		printer->Status(printer, &StatusOnline, Error, &AmmountLabels, &Interpreter);
		if (StatusOnline == 1 && strcmp (Error, "No error") == 0 && Interpreter == 0)		
		 	 break;
		TIMEOUT_RESET_BUFFER_PRINTER (6000);
	}while (TRUE);
	
	
Error:
	if (pexception)
	{
	sprintf (msg, "Erreur Reset Printer  \r\n StatusOnline = %d \r\n Interpreter = %d \r\n Error = %s \r\n", StatusOnline, Interpreter, Error);
	MessagePopup("Message De Déboguage", msg);
	}
	TEST_END();
}

FCE_CLEANUP(EA210, 60_00)
{																	  
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 60_10
*********************************************************************************/

FCE_HELP(EA210, 60_10)
{
    HELP_BEGIN();
    HELP_TEST( "Tester : Init Printer" );

Error:
    HELP_END();
}

FCE_CHECK(EA210, 60_10)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE( TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 60_10)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 60_10)
{
    TEST_BEGIN();
	TEST_USE(TESTER);

	/* make Ready Printer */
	EXCCHECK(ptester->ReadyPrint(ptester, 6000));	
	
Error:
	TEST_END();
}

FCE_CLEANUP(EA210, 60_10)
{																	  
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 60_20
*********************************************************************************/

FCE_HELP(EA210, 60_20)
{
    HELP_BEGIN();
    HELP_TEST( "Tester : SEND Label to  Printer and Write Tracability" );

Error:
    HELP_END();
}

FCE_CHECK(EA210, 60_20)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE( TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 60_20)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 60_20)
{
    TEST_BEGIN();
	TEST_USE(TESTER);
	TEST_USE(PRINTERS);
	TEST_USE(TRACA);

	int 				iStatus;
	clock_t 			_start_clock;
	double   			time;	
	int					StatusOnline;
	int					Interpreter;
	int					AmmountLabels;
	char				Error[128] = "";
	char				sAmmount[128] = "";
	char            	Buffer[512]= "";
	char   barcode[512];	
	char 				buffer[2048] = "";
	char   Date_Debut[32+1];
	int    Year, Month, Day, Hour, Minute, Second;
   	SElEventDataPtr 	pData; 


	INITIALIZE_PRINTER(PRINTER_CAB);

	INITIALIZE_TRACA(0);
		
		sprintf(((STestParamPtr)pTID)->Num_Serie_pFin, "SKR_%s%s",((STestParamPtr)pTID)->Board_Ver_pFin,((STestParamPtr)pTID)->Serial_Board_Ver_sFin);
		sscanf(((STestParamPtr)pTID)->Time, "%02d.%02d.%02d", &Hour, &Minute, &Second);
		sscanf(((STestParamPtr)pTID)->Date, "%04d.%02d.%02d", &Year, &Month, &Day);
		sprintf(Date_Debut, "%04d-%02d-%02d %02d:%02d:%02d.000", Year, Day, Month, Hour, Minute, Second);//2020-22-08 22:00:00.000
		EXCCHECK( eleventdata_new(&pData));	

	if (((STestParamPtr)pTID)->pexception == NULL)  //PASS	   me->pTestParam->Origin_Fault == STATION_10
	{
		/* send label OK */

	    sprintf(((STestParamPtr)pTID)->half_Num_Serie_pFin, "SKR_%s", ((STestParamPtr)pTID)->Board_Ver_pFin);
		sprintf(Buffer, "%s%s", ((STestParamPtr)pTID)->half_Num_Serie_pFin,((STestParamPtr)pTID)->Serial_Board_Ver_sFin);


		EXCCHECK(pData->SetStringPtr(pData, Buffer));
		EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_TESTER_BUTTON_PRESSED_LABEL", pData));
		

	   /* Insertion mouvement FCT-12 */
		pexception = ptraca->Set_Last_Mouvement(ptraca,((STestParamPtr)pTID)->Num_Serie_pFin,((STestParamPtr)pTID)->id_Board_pFin, Date_Debut, "","TESTER_FCT12", "FCT", (((STestParamPtr)pTID)->pexception == NULL)? 1:0);

		if (pexception)
		{
			((STestParamPtr)pTID)->pexception = pexception;
			//printf("erreur set movement \n");
			EXCTHROW (-1, "erreur insertion mouvement !");

		}
		/* association entre sFin et pFin */
		pexception = ptraca->Set_Association(ptraca,((STestParamPtr)pTID)->Num_Serie_sFin, ((STestParamPtr)pTID)->Num_Serie_pFin, "", Date_Debut);
		if (pexception)
		{
			((STestParamPtr)pTID)->pexception = pexception;
			EXCTHROW (-1, "erreur association entre NumSerie sFin et NumSerie pFin !");
		}		
		
		

		/* Check if data already sent to Printer */
		
		_start_clock =  clock();
		do{
			Sleep(100);//TEST old  Sleep(300);
			printer->Status(printer, &StatusOnline, Error, &AmmountLabels, &Interpreter);
			//printf ("Check if data already sent to Printer ==> StatusOnline = %d | Interpreter = %d | Error = %s \r\n", StatusOnline, Interpreter, Error);
			if (StatusOnline == 1 && Interpreter == 1)		
		 	 break;
			TIMEOUT_SEND_BUFFER_PRINTER (7000);
		}while (TRUE);

	
	}
	
	
	
Error:
	//printer->Close(printer);
	TEST_END();
}

FCE_CLEANUP(EA210, 60_20)
{																	  
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 60_30
*********************************************************************************/

FCE_HELP(EA210, 60_30)
{
    HELP_BEGIN();
    HELP_TEST( "Tester : Pick Panel And Go to Printer " );

Error:
    HELP_END();
}

FCE_CHECK(EA210, 60_30)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE( TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 60_30)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 60_30)
{

    TEST_BEGIN();
	TEST_USE(TESTER);
   
	/* pick & place to print */
	EXCCHECK(ptester->PickPrintPanelStation60(ptester, 10000));
	
	
Error:
	TEST_END();
}

FCE_CLEANUP(EA210, 60_30)
{																	  
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 60_40
*********************************************************************************/

FCE_HELP(EA210, 60_40)
{
    HELP_BEGIN();
    HELP_TEST( "Tester : Start Cycle Print" );

Error:
    HELP_END();
}

FCE_CHECK(EA210, 60_40)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE( TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 60_40)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 60_40)
{

    TEST_BEGIN();
	TEST_USE(TESTER);
	/* make Ready Printer */
	EXCCHECK(ptester->CyclePrint(ptester, 10000));
	Sleep(2000);
	
Error:
	TEST_END();
}

FCE_CLEANUP(EA210, 60_40)
{																	  
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}

/*********************************************************************************
 * Function 60_50
*********************************************************************************/
FCE_HELP(EA210, 60_50)
{
    HELP_BEGIN();
    HELP_TEST( "Tester : X move & place Product front DATAMAN" );

Error:
    HELP_END();
}

FCE_CHECK(EA210, 60_50)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE( TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 60_50)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 60_50)
{

	TEST_BEGIN();
	TEST_USE(TESTER);
	int i,XREAR;

	/* pick & place barcode */
	ptester->ACTUATOR_X_REAR(ptester, 4000);
	Sleep(600);
	EXCCHECK( ptester->GetSensor(ptester, "SENSOR_CYLINDER_X_REAR_STATION_60",&XREAR ))
	if(XREAR==0)
	{
		Sleep(1000);
		EXCCHECK(ptester->ACTUATOR_X_REAR(ptester, 4000));
	}

Error:
	TEST_END();
}

FCE_CLEANUP(EA210, 60_50)
{																	  
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}/*********************************************************************************
 * Function 60_60
*********************************************************************************/

FCE_HELP(EA210, 60_60)
{
    HELP_BEGIN();
    HELP_TEST( "Tester : Read barcode" );

Error:
    HELP_END();
}

FCE_CHECK(EA210, 60_60)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE( TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 60_60)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 60_60)
{

    TEST_BEGIN();
	TEST_USE(TESTER);
    char barcode[512];
    char buffer[512];
	char msg[255];
	int i;

	/* read barcode */
	memset (buffer, 0x00, 512);
	memset (barcode, 0x00, 512);

	ptester->GetItemCode(ptester, 0, 1, 512, buffer, 5000);
	if(strlen(buffer)==0)
		EXCCHECK( ptester->GetItemCode(ptester, 0, 1, 512, buffer, 10000));

	/* enlever le \r\n */
	for(i= 0 ; i < strlen(buffer); i++)
	{
		if (buffer[i] == 0x0D)
			break;
		else
			barcode[i] = buffer[i];
	}
	if (strlen(barcode) != 20)
	{

		sprintf (msg, "erreur longueur barcode = %d",strlen(barcode));
		EXCTHROW( -1, msg);
	}
	
	
Error:
	TEST_END();
}

FCE_CLEANUP(EA210, 60_60)
{																	  
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}
/*********************************************************************************
 * Function 60_70
*********************************************************************************/

FCE_HELP(EA210, 60_70)
{
    HELP_BEGIN();
    HELP_TEST( "Tester : PLACE PRODUCT TO PANEL" );

Error:
    HELP_END();
}

FCE_CHECK(EA210, 60_70)
{
    TESTREGUIRE_BEGIN();
	
	REQUIRE( TESTER);
	REQUIRE_END();
	
Error:
    TESTREGUIRE_END();
}

FCE_INIT(EA210, 60_70)									
{
    TESTINIT_BEGIN();
	
Error:    
    TESTINIT_END();
}

FCE_TEST(EA210, 60_70)
{

    TEST_BEGIN();
	TEST_USE(TESTER);
	EXCCHECK(ptester->ResetPickPrintPanelStation60(ptester, 25000));
	
Error:
	TEST_END();
}

FCE_CLEANUP(EA210, 60_70)
{																	  
    TESTCLEANUP_BEGIN();
    
Error:    
    TESTCLEANUP_END();
}