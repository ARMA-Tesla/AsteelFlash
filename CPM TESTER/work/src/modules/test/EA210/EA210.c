#include <ansi_c.h>
#include "EA210.h"
#include "EA210_utils.h"
#include <test\test_helper_functions.h>
#include "toolbox.h"
#include <formatio.h>
#include <multithread/multithread.h>
#include <analysis.h>

const char* HISTORY_INFO = \
"[01-07-2019] ARAIBIA MOHAMED: version 2.00\
 * Création."; 



STestSeq gs_TestSeq[] = { 
	 
	ADD_TSI(STATION_20),
	ADD_TSI(STATION_30),
	ADD_TSI(STATION_40),
	ADD_TSI(STATION_50),
	ADD_TSI(STATION_60),
	ADD_TSI(STATION_10),

	ADD_TSI_LAST_ENTRY
};


extern 	STestListItem	gs_TestList_Station_10[];
extern 	STestListItem 	gs_TestList_Station_20[];
extern 	STestListItem 	gs_TestList_Station_30[];
extern 	STestListItem 	gs_TestList_Station_40[];
extern 	STestListItem 	gs_TestList_Station_50[];
extern 	STestListItem 	gs_TestList_Station_60[];


STestListItemPtr gs_TestList[STATION_MAX] = {
	gs_TestList_Station_20, 
	gs_TestList_Station_30, 
	gs_TestList_Station_40,
	gs_TestList_Station_50,
	gs_TestList_Station_60,
	gs_TestList_Station_10,
};























