#include <ansi_c.h>
#include "EA210_utils.h"
#include "toolbox.h"
#include <formatio.h>
#include <multithread/multithread.h>
#include "test_panel.h"
#include "EA210_Test_WS10.h"
#include <automat/automat.h>

#define TIMEOUT_CONFIRM_OP		  10000
#define RETEST_FONC				  3


#define TESTER_LOCK_PANEL_STATION_10(timeout) \
	EXCCHECK( ptester->LockPanelStation10(ptester, timeout))

#define TESTER_UNLOCK_PANEL_STATION_10(timeout) \
	EXCCHECK( ptester->UnlockPanelStation10(ptester, timeout))

#define TESTER_PLUG_PANEL_STATION_10(timeout) \
	EXCCHECK( ptester->PlugPanelStation10(ptester, timeout))

#define TESTER_UNPLUG_PANEL_STATION_10(timeout) \
	EXCCHECK( ptester->UnplugPanelStation10(ptester, timeout))

DEFINE_GLOBAL_VARIABLE();

extern STestListItem gs_TestList_Station_10[] = {
	
	
	ADD_TI_LAST_ENTRY
};


/*********************************************************************************
 * Function Testsequence Init
 *********************************************************************************/
FCE_TESTSEQUENCE_INIT(STATION_10)
{
	TESTSEQUENCEINIT_BEGIN();

	PANEL_INI();
		
Error:
	TESTSEQUENCEINIT_END();
}

/*********************************************************************************
 * Function Testsequence Cleanup
 *********************************************************************************/
FCE_TESTSEQUENCE_CLEANUP(STATION_10)
{
	TESTSEQUENCECLEANUP_BEGIN(); 
	TEST_USE(TESTER);


Error:
	TESTSEQUENCECLEANUP_END();
}

/*********************************************************************************
 * Function Testsequence Wait
 *********************************************************************************/
FCE_TESTSEQUENCE_WAIT(STATION_10)
{
	TESTSEQUENCEWAIT_BEGIN(); 

Error:
	TESTSEQUENCEWAIT_END();
}
