#if !defined(__TEST_PANEL_H__)
#define __TEST_PANEL_H__

#include <test/eltest.h>
#include "test.h"
#include <eltypes.h>
#include <elerror.h>
#include <language/language.h>
#include <userint.h> 

#define OK			 	 0
#define NOK			  	 1
#define RUN			  	 2
#define FREE			 3
 


typedef enum _EStation
{
	STATION_20 = 0,
	STATION_30,
	STATION_40,
	STATION_50,
	STATION_60,
	STATION_10,
	STATION_MAX,
		
} EStation;

int		gs_Panel[STATION_MAX];  
int     gs_TestPanelLevers;


static int panelSeqUsed [STATION_MAX] = 
	{
		TEST_20_ACTIVE, TEST_30_ACTIVE, TEST_40_ACTIVE,
		TEST_50_ACTIVE, TEST_60_ACTIVE, TEST_10_ACTIVE
	};

static int panelSeqGoFail [STATION_MAX] = 
	{
		TEST_20_GO_FAIL, TEST_30_GO_FAIL, TEST_40_GO_FAIL,
		TEST_50_GO_FAIL, TEST_60_GO_FAIL, TEST_10_GO_FAIL
	};

static int ControlTest [STATION_MAX] = 
	{
		TEST_20_TEST20, TEST_30_TEST30, TEST_40_TEST40,
		TEST_50_TEST50, TEST_60_TEST60, TEST_10_TEST10
	};

static int ControlReTest [STATION_MAX] = 
	{
		TEST_20_RETEST, TEST_30_RETEST, TEST_40_RETEST,
		TEST_50_RETEST, TEST_60_RETEST, TEST_10_RETEST
	};
 
static int ControlStopReTest [STATION_MAX] = 
	{
		TEST_20_STOP_RETEST, TEST_30_STOP_RETEST, TEST_40_STOP_RETEST,
		TEST_50_STOP_RETEST, TEST_60_STOP_RETEST, TEST_10_STOP_RETEST
	};

static int ControlReTestTotal [STATION_MAX] = 
	{
		TEST_20_RETEST_TOTAL, TEST_30_RETEST_TOTAL, TEST_40_RETEST_TOTAL,
		TEST_50_RETEST_TOTAL, TEST_60_RETEST_TOTAL, TEST_10_RETEST_TOTAL
	};

static int ControlReTestPASS [STATION_MAX] = 
	{
		TEST_20_RETEST_PASS, TEST_30_RETEST_PASS, TEST_40_RETEST_PASS,
		TEST_50_RETEST_PASS, TEST_60_RETEST_PASS, TEST_10_RETEST_PASS
	};

static int ControlReTestFAIL [STATION_MAX] = 
	{
		TEST_20_RETEST_FAIL, TEST_30_RETEST_FAIL, TEST_40_RETEST_FAIL,
		TEST_50_RETEST_FAIL, TEST_60_RETEST_FAIL, TEST_10_RETEST_FAIL
	};

static int PanelTable [STATION_MAX] = 
	{
		TEST_20_TABLE, 	TEST_30_TABLE, 	TEST_40_TABLE,
		TEST_50_TABLE, 	TEST_60_TABLE, 	TEST_10_TABLE
	};

static int PanelTableState [STATION_MAX] = 
	{
		TEST_20_TABLE_STATE, 	TEST_30_TABLE_STATE, 	TEST_40_TABLE_STATE,
		TEST_50_TABLE_STATE, 	TEST_60_TABLE_STATE, 	TEST_10_TABLE_STATE
	};

static int panelStatusStation [STATION_MAX] = 
	{
		PANEL_STATE_STATION_20, PANEL_STATE_STATION_30, PANEL_STATE_STATION_40, 
		PANEL_STATE_STATION_50, PANEL_STATE_STATION_60, PANEL_STATE_STATION_10, 
	};

static int panelPlatettStation [STATION_MAX] = 
	{
		PANEL_PALETT_STATION_20, PANEL_PALETT_STATION_30, PANEL_PALETT_STATION_40, 
		PANEL_PALETT_STATION_50, PANEL_PALETT_STATION_60, PANEL_PALETT_STATION_10, 
	};

static int panelSEQStation [STATION_MAX] = 
	{
		PANEL_SEQ_STATION_20, PANEL_SEQ_STATION_30, PANEL_SEQ_STATION_40, 
		PANEL_SEQ_STATION_50, PANEL_SEQ_STATION_60, PANEL_SEQ_STATION_10, 
	};

static int panelFaultStation [STATION_MAX] = 
	{
		PANEL_FAULT_STATION_20,	PANEL_FAULT_STATION_30,	PANEL_FAULT_STATION_40, 
		PANEL_FAULT_STATION_50,	PANEL_FAULT_STATION_60,	PANEL_FAULT_STATION_10
	};

static int panelPlaettCheck [STATION_MAX] = 
	{
		PANEL_CHECK_P1,	PANEL_CHECK_P2,	PANEL_CHECK_P3,
		PANEL_CHECK_P4,	PANEL_CHECK_P5,	PANEL_CHECK_P6
	};
	
TEST_API SElExceptionPtr _FUNCC IniTestPanel(const char* picture_left, const char* picture_right, bool_t reset);
TEST_API SElExceptionPtr _FUNCC CloseTestPanel(void); 
TEST_API SElExceptionPtr _FUNCC SetTestPanelDescription(int32_t control, const char* description);
TEST_API SElExceptionPtr _FUNCC SetPanelWarning(int32_t control,  bool_t status);
TEST_API SElExceptionPtr _FUNCC SetPanelStationStatus(int32_t station,  bool_t status);
TEST_API SElExceptionPtr _FUNCC SetTestPanelReferences(char* RefValeo,  char* RefCustomer);
TEST_API SElExceptionPtr _FUNCC SetTestPanelMode(int32_t mode);
TEST_API SElExceptionPtr _FUNCC SetTestPanelTime(int32_t station,  int32_t time);
TEST_API SElExceptionPtr _FUNCC SetTestPanelFault(int32_t station, int32_t station_origin, char* test_description, bool_t status);
TEST_API SElExceptionPtr _FUNCC GetPalettStatus(int32_t palettID, int32_t *status);
TEST_API SElExceptionPtr _FUNCC GetTestSeqActive(int* SeqUed);
TEST_API SElExceptionPtr _FUNCC GetTestSeqStopFail(int* SeqUed);

TEST_API SElExceptionPtr _FUNCC GetTestFunction(int32_t process, int32_t index, STestListItemPtr* pTest, int32_t* count);
TEST_API SElExceptionPtr _FUNCC GetTestSeqFunction(int32_t index, STestSeq* pTestSeq, int32_t* count);

void SetAnnotationAttribute_ALL(int Panel_Handle, int Control_ID, int Annotation_Index);
int GetStationNumber(int32_t station);

#define COLOR_GRID  0x00347B

#define TEST_HELP(help) \
	EXCCHECKCVI( SetCtrlVal(gs_TestPanel, PANEL_HELP2, help)) \

#define TEST_HELP_OPERATOR_RIGHT(help) \
	EXCCHECKCVI( SetCtrlVal(gs_TestPanelLevers, PANEL_OP_HELP_RIGHT, help)) \
	SetCtrlAttribute (gs_TestPanelLevers, PANEL_OP_RESULT_RIGHT, ATTR_TEXT_BGCOLOR, VAL_WHITE); \
	SetCtrlVal( gs_TestPanelLevers, PANEL_OP_RESULT_RIGHT, ""); \


#define TEST_HELP_RESULT_RIGHT(help) \
	char _buffer[256]; \
	sprintf (_buffer , "DEFAUT : %s", help); \
	EXCCHECKCVI( SetCtrlVal(gs_TestPanelLevers, PANEL_OP_HELP_RIGHT, _buffer)); \
	SetCtrlVal( gs_TestPanelLevers, PANEL_OP_RESULT_RIGHT, "NOK"); \
	SetCtrlAttribute (gs_TestPanelLevers, PANEL_OP_RESULT_RIGHT, ATTR_TEXT_BGCOLOR, VAL_RED); \


#define TEST_HELP_RESULT_LEFT(help) \
	char _buffer[256]; \
	sprintf (_buffer , "DEFAUT : %s", help); \
	EXCCHECKCVI( SetCtrlVal(gs_TestPanelLevers, PANEL_OP_HELP_LEFT, _buffer)); \
	SetCtrlVal( gs_TestPanelLevers, PANEL_OP_RESULT_LEFT, "NOK"); \
	SetCtrlAttribute (gs_TestPanelLevers, PANEL_OP_RESULT_LEFT, ATTR_TEXT_BGCOLOR, VAL_RED); \


#define TEST_HELP_OPERATOR_LEFT(help) \
	EXCCHECKCVI( SetCtrlVal(gs_TestPanelLevers, PANEL_OP_HELP_LEFT, help)) \

#define CONVERT_GRAPH_TO_PNG_STATION_60(path) \
	SaveGraphPNG (gs_TestPanel4,PANEL_4, path); \

#define ACTIVATE_TIMER(status) \
	SetCtrlAttribute (gs_TestPanelLevers, PANEL_OP_TIMER_LED, ATTR_ENABLED, status); \
	
#define DISPLAY_TEST( station, name, msg ) \
	do { \
		int	rows = 1; char _buffer[256] = "Test: "; \
		strcat(_buffer, msg); \
		InsertTableRows (gs_Panel[station], PanelTable[station], -1, 1, VAL_CELL_STRING); \
		GetNumTableRows (gs_Panel[station], PanelTable[station], &rows);\
		SetTableRowAttribute (gs_Panel[station], PanelTable[station], rows, ATTR_SIZE_MODE, VAL_USE_EXPLICIT_SIZE); \
		SetTableRowAttribute (gs_Panel[station], PanelTable[station], rows, ATTR_ROW_HEIGHT, 25); \
		SetTableCellAttribute (gs_Panel[station], PanelTable[station], MakePoint(1, rows), ATTR_HORIZONTAL_GRID_COLOR, COLOR_GRID); \
		SetTableCellAttribute (gs_Panel[station], PanelTable[station], MakePoint(1, rows), ATTR_VERTICAL_GRID_COLOR, COLOR_GRID); \
		SetTableCellAttribute (gs_Panel[station], PanelTable[station], MakePoint(1, rows), ATTR_CELL_JUSTIFY, VAL_CENTER_LEFT_JUSTIFIED); \
		SetTableCellAttribute (gs_Panel[station], PanelTable[station], MakePoint(1, rows), ATTR_CTRL_VAL, name); \
		SetTableCellAttribute (gs_Panel[station], PanelTable[station], MakePoint(2, rows), ATTR_HORIZONTAL_GRID_COLOR, COLOR_GRID); \
		SetTableCellAttribute (gs_Panel[station], PanelTable[station], MakePoint(2, rows), ATTR_VERTICAL_GRID_COLOR, COLOR_GRID); \
		SetTableCellAttribute (gs_Panel[station], PanelTable[station], MakePoint(2, rows), ATTR_CELL_JUSTIFY, VAL_CENTER_LEFT_JUSTIFIED); \
		SetTableCellAttribute (gs_Panel[station], PanelTable[station], MakePoint(2, rows), ATTR_CTRL_VAL, msg); \
		SetActiveTableCell (gs_Panel[station], PanelTable[station], MakePoint(1, rows)); \
	} while (0)
	
													
#define DISPLAY_TEST_RESULT(station, duration ) \
	do { \
		int	rows = 1, bid; \
		GetNumTableRows (gs_Panel[station], PanelTable[station], &rows);\
		GetCtrlBitmap (gs_Panel[station], PanelTableState[station], (pexception==NULL)? 0:1, &bid); \
		SetTableCellAttribute (gs_Panel[station], PanelTable[station], MakePoint(3, rows), ATTR_HORIZONTAL_GRID_COLOR, COLOR_GRID); \
		SetTableCellAttribute (gs_Panel[station], PanelTable[station], MakePoint(3, rows), ATTR_VERTICAL_GRID_COLOR, COLOR_GRID); \
		SetTableCellAttribute (gs_Panel[station], PanelTable[station], MakePoint(3, rows), ATTR_CELL_TYPE, VAL_CELL_NUMERIC); \
		SetTableCellAttribute (gs_Panel[station], PanelTable[station], MakePoint(3, rows), ATTR_CELL_JUSTIFY, VAL_CENTER_CENTER_JUSTIFIED); \
		SetTableCellAttribute (gs_Panel[station], PanelTable[station], MakePoint(3, rows), ATTR_CTRL_VAL, duration); \
		SetTableCellAttribute (gs_Panel[station], PanelTable[station], MakePoint(4, rows), ATTR_HORIZONTAL_GRID_COLOR, COLOR_GRID); \
		SetTableCellAttribute (gs_Panel[station], PanelTable[station], MakePoint(4, rows), ATTR_VERTICAL_GRID_COLOR, COLOR_GRID); \
	    SetTableCellAttribute (gs_Panel[station], PanelTable[station], MakePoint(4, rows), ATTR_CELL_TYPE, VAL_CELL_PICTURE); \
		SetTableCellAttribute (gs_Panel[station], PanelTable[station], MakePoint(4, rows), ATTR_FIT_MODE, VAL_PICT_CENTER); \
		SetTableCellAttribute (gs_Panel[station], PanelTable[station], MakePoint(4, rows), ATTR_CELL_JUSTIFY, VAL_CENTER_CENTER_JUSTIFIED); \
		SetTableCellAttribute (gs_Panel[station], PanelTable[station], MakePoint(4, rows), ATTR_CTRL_VAL, bid); \
		DiscardBitmap(bid); \
	} while (0)


#define DISPLAY_TESTSEQ_RESULT() \
	do { \
		char msg[32]; \
		if(((STestParamPtr)pTID)->Skip_All_Test==TRUE)  SetCtrlIndex( gs_TestPanel, panelStatusStation[station], FREE); \
		else { if(((STestParamPtr)pTID)->pexception==NULL)  SetCtrlIndex( gs_TestPanel, panelStatusStation[station], OK); \
		else SetCtrlIndex( gs_TestPanel, panelStatusStation[station], NOK); }\
		EXCCHECKCVI( SetCtrlAttribute(gs_Panel[station], ControlTest [station], ATTR_DIMMED, 0)); \
		if(((STestParamPtr)pTID)->Origin_Fault == -1 && ((STestParamPtr)pTID)->pexception)  ((STestParamPtr)pTID)->Origin_Fault = GetStationNumber(station); \
		SetTestPanelFault (station, ((STestParamPtr)pTID)->Origin_Fault, NULL, (((STestParamPtr)pTID)->pexception == NULL)? TRUE : FALSE); \
	} while (0)


	
#define DISPLAY_TEST_FUNC_RIGHT( msg ) \
	do { \
		int	rows = 1; char _buffer[256] = "Test: "; \
		strcat(_buffer, msg); \
		InsertTableRows (gs_TestPanelLevers, PANEL_OP_TABLE, -1, 1, VAL_CELL_STRING); \
		GetNumTableRows (gs_TestPanelLevers, PANEL_OP_TABLE, &rows);\
		SetTableRowAttribute (gs_TestPanelLevers, PANEL_OP_TABLE, rows, ATTR_SIZE_MODE, VAL_USE_EXPLICIT_SIZE); \
		SetTableRowAttribute (gs_TestPanelLevers, PANEL_OP_TABLE, rows, ATTR_ROW_HEIGHT, 25); \
		SetTableCellAttribute (gs_TestPanelLevers, PANEL_OP_TABLE, MakePoint(1, rows), ATTR_HORIZONTAL_GRID_COLOR, COLOR_GRID); \
		SetTableCellAttribute (gs_TestPanelLevers, PANEL_OP_TABLE, MakePoint(1, rows), ATTR_VERTICAL_GRID_COLOR, COLOR_GRID); \
		SetTableCellAttribute (gs_TestPanelLevers, PANEL_OP_TABLE, MakePoint(1, rows), ATTR_CELL_JUSTIFY, VAL_CENTER_LEFT_JUSTIFIED); \
		SetTableCellAttribute (gs_TestPanelLevers, PANEL_OP_TABLE, MakePoint(1, rows), ATTR_CTRL_VAL, msg); \
		SetActiveTableCell (gs_TestPanelLevers, PANEL_OP_TABLE, MakePoint(1, rows)); \
	} while (0)

#define DISPLAY_TEST_RESULT_FUNC_RIGHT() \
	do { \
		int	rows = 1, bid; \
		GetNumTableRows (gs_TestPanelLevers, PANEL_OP_TABLE, &rows);\
		GetCtrlBitmap (gs_TestPanelLevers, PANEL_OP_TABLE_STATE, (pexception==NULL)? 0:1, &bid); \
		SetTableCellAttribute (gs_TestPanelLevers, PANEL_OP_TABLE, MakePoint(2, rows), ATTR_HORIZONTAL_GRID_COLOR, COLOR_GRID); \
		SetTableCellAttribute (gs_TestPanelLevers, PANEL_OP_TABLE, MakePoint(2, rows), ATTR_VERTICAL_GRID_COLOR, COLOR_GRID); \
	    SetTableCellAttribute (gs_TestPanelLevers, PANEL_OP_TABLE, MakePoint(2, rows), ATTR_CELL_TYPE, VAL_CELL_PICTURE); \
		SetTableCellAttribute (gs_TestPanelLevers, PANEL_OP_TABLE, MakePoint(2, rows), ATTR_FIT_MODE, VAL_PICT_CENTER); \
		SetTableCellAttribute (gs_TestPanelLevers, PANEL_OP_TABLE, MakePoint(2, rows), ATTR_CELL_JUSTIFY, VAL_CENTER_CENTER_JUSTIFIED); \
		SetTableCellAttribute (gs_TestPanelLevers, PANEL_OP_TABLE, MakePoint(2, rows), ATTR_CTRL_VAL, bid); \
		DiscardBitmap(bid); \
		if(pexception!=NULL) SetCtrlIndex( gs_TestPanel, PANEL_ACTION, 0); \
	} while (0)

#define DISPLAY_TEST_FUNC_LEFT( msg ) \
	do { \
		int	rows = 1; char _buffer[256] = "Test: "; \
		strcat(_buffer, msg); \
		InsertTableRows (gs_TestPanelLevers, PANEL_OP_TABLE_2, -1, 1, VAL_CELL_STRING); \
		GetNumTableRows (gs_TestPanelLevers, PANEL_OP_TABLE_2, &rows);\
		SetTableRowAttribute (gs_TestPanelLevers, PANEL_OP_TABLE_2, rows, ATTR_SIZE_MODE, VAL_USE_EXPLICIT_SIZE); \
		SetTableRowAttribute (gs_TestPanelLevers, PANEL_OP_TABLE_2, rows, ATTR_ROW_HEIGHT, 25); \
		SetTableCellAttribute (gs_TestPanelLevers, PANEL_OP_TABLE_2, MakePoint(1, rows), ATTR_HORIZONTAL_GRID_COLOR, COLOR_GRID); \
		SetTableCellAttribute (gs_TestPanelLevers, PANEL_OP_TABLE_2, MakePoint(1, rows), ATTR_VERTICAL_GRID_COLOR, COLOR_GRID); \
		SetTableCellAttribute (gs_TestPanelLevers, PANEL_OP_TABLE_2, MakePoint(1, rows), ATTR_CELL_JUSTIFY, VAL_CENTER_LEFT_JUSTIFIED); \
		SetTableCellAttribute (gs_TestPanelLevers, PANEL_OP_TABLE_2, MakePoint(1, rows), ATTR_CTRL_VAL, msg); \
		SetActiveTableCell (gs_TestPanelLevers, PANEL_OP_TABLE_2, MakePoint(1, rows)); \
	} while (0)

#define DISPLAY_TEST_RESULT_FUNC_LEFT() \
	do { \
		int	rows = 1, bid; \
		GetNumTableRows (gs_TestPanelLevers, PANEL_OP_TABLE_2, &rows);\
		GetCtrlBitmap (gs_TestPanelLevers, PANEL_OP_TABLE_STATE, (pexception==NULL)? 0:1, &bid); \
		SetTableCellAttribute (gs_TestPanelLevers, PANEL_OP_TABLE_2, MakePoint(2, rows), ATTR_HORIZONTAL_GRID_COLOR, COLOR_GRID); \
		SetTableCellAttribute (gs_TestPanelLevers, PANEL_OP_TABLE_2, MakePoint(2, rows), ATTR_VERTICAL_GRID_COLOR, COLOR_GRID); \
	    SetTableCellAttribute (gs_TestPanelLevers, PANEL_OP_TABLE_2, MakePoint(2, rows), ATTR_CELL_TYPE, VAL_CELL_PICTURE); \
		SetTableCellAttribute (gs_TestPanelLevers, PANEL_OP_TABLE_2, MakePoint(2, rows), ATTR_FIT_MODE, VAL_PICT_CENTER); \
		SetTableCellAttribute (gs_TestPanelLevers, PANEL_OP_TABLE_2, MakePoint(2, rows), ATTR_CELL_JUSTIFY, VAL_CENTER_CENTER_JUSTIFIED); \
		SetTableCellAttribute (gs_TestPanelLevers, PANEL_OP_TABLE_2, MakePoint(2, rows), ATTR_CTRL_VAL, bid); \
		DiscardBitmap(bid); \
		if(pexception!=NULL) SetCtrlIndex( gs_TestPanel, PANEL_ACTION, 0); \
	} while (0)

#define DISPLAY_TESTSEQUENCE_DURATION(time) \
	do { \
		char buffer[256]; \
		sprintf(buffer, "%.1f s", time); \
		SetTableCellVal(gs_TestPanel, PANEL_TABLE, MakePoint(3, station+1), buffer); \
	} while (0)

#define PANEL_INI() \
	do{ \
	if(((STestParamPtr)pTID)->pexception!=NULL)  SetCtrlIndex( gs_TestPanel, panelStatusStation[station], NOK); \
	else {SetCtrlIndex( gs_TestPanel, panelStatusStation[station], RUN); \
	SetCtrlAttribute (gs_TestPanel, panelFaultStation[station], ATTR_VISIBLE, FALSE);} \
	EXCCHECKCVI( SetCtrlAttribute(gs_Panel[station], ControlTest [station], ATTR_DIMMED, 1)); \
	EXCCHECKCVI( DeleteTableRows (gs_Panel[station], PanelTable[station], 1, -1)); \
	ProcessDrawEvents (); \
	} while(0)


#define PANEL_CLEAN() \
	do { \
		int index; \
		EXCCHECKCVI( GetCtrlIndex( gs_TestPanel, PANEL_ACTION, &index)); \
		if(index==2) SetCtrlIndex( gs_TestPanel, PANEL_ACTION, 1); \
		EXCCHECKCVI( SetCtrlAttribute(gs_TestPanel, PANEL_FORWARD, ATTR_DIMMED, TRUE)); \
		EXCCHECKCVI( SetCtrlAttribute(gs_TestPanel, PANEL_BACKWARD, ATTR_DIMMED, TRUE)); \
		EXCCHECKCVI( SetCtrlAttribute(gs_TestPanel, PANEL_REPEAT, ATTR_DIMMED, TRUE)); \
		EXCCHECKCVI( SetCtrlAttribute(gs_TestPanel, PANEL_STOP, ATTR_DIMMED, TRUE)); \
	} while (0)

#define START_SEND_DATAS_CAN(mode) \
	do { \
		EXCCHECKCVI( SetCtrlAttribute (gs_TestPanel,PANEL_TIMER_50 , ATTR_ENABLED, TRUE)); \
		EXCCHECKCVI( SetCtrlAttribute (gs_TestPanel,PANEL_TIMER_100 , ATTR_ENABLED, TRUE)); \
		EXCCHECKCVI( SetCtrlAttribute (gs_TestPanel,PANEL_TIMER_200 , ATTR_ENABLED, TRUE)); \
		EXCCHECKCVI( SetCtrlAttribute (gs_TestPanel,PANEL_TIMER_500 , ATTR_ENABLED, TRUE)); \
		EXCCHECKCVI( SetCtrlAttribute (gs_TestPanel,PANEL_TIMER_1000 , ATTR_ENABLED, TRUE)); \
	} while (0) 

#endif
