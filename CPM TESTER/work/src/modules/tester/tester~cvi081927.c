#include "tester.h"
#include "msxmldom.h"
#include <guid.h>
#include <time.h>
#include <utility.h>
#include <sdk/mmsystem.h>
#include <multithread/multithread.h>
#include "asynctmr.h"
#include <userint.h>
#include <ansi_c.h>

CRITICAL_SECTION   _Lock;


#define MAX_LENGTH_SENSOR_LOGICAL_NAME				64

#define TIMEOUT_INIT() 	clock_t	_start_clock = clock();

#define TIMEOUT_CHECK(timeout) \
	if ( timeout>0 && ((clock() - _start_clock) / (double)CLOCKS_PER_SEC) > (timeout/1000.0) ) { EXCTHROW( TESTER_ERROR_TIMEOUT, "Timeout"); } else

#define TIMEOUT_PRINTER_CHECK(timeout) \
	if ( ((clock() - _start_clock) / (double)CLOCKS_PER_SEC) > (timeout/1000.0) ) { EXCTHROW( -1, "Cycle print error"); } else

#define NUMBER_OF_STATIONS	  6+1

#define AXIS_X  0

#define PASS  1
#define FAIL  0

#define MODE_REMOVE_KEY  0
#define MODE_INSERT_KEY  1

#define TIME_AIR  		200


	
static int  	g_timerId3 = 0;


typedef enum _EStation
{
	STATION_10 = 0,
	STATION_20,
	STATION_30,
	STATION_40,
	STATION_50,
	STATION_60,
} EStation;


	
static int32_t	gs_Light_Tester = 0;


static int32_t	gs_inserted = 0;
static int32_t	gs_released = 0;
static int32_t	gs_plugged_station_10 = 0;
static int32_t	gs_unplugged_station_10 = 0;
static int32_t	gs_locked_station_10 = 0;
static int32_t	gs_unlocked_station_10 = 0;

static int32_t	gs_plugged_station_30 = 0;
static int32_t	gs_unplugged_station_30 = 0;
static int32_t	gs_locked_station_30 = 0;
static int32_t	gs_unlocked_station_30 = 0;
static int32_t	gs_cylinder_centrage_is_front_30 = 0;
static int32_t	gs_cylinder_centrage_is_rear_30 = 0;
static int32_t	gs_cylinder_gripper_centrage_is_blocked_30 = 0;
static int32_t	gs_cylinder_gripper_centrage_is_unblocked_30 = 0;
static int32_t	gs_cylinder_airbag_is_up_30 = 0;
static int32_t	gs_cylinder_airbag_is_down_30 = 0;

static int32_t	gs_plugged_station_60 = 0;
static int32_t	gs_unplugged_station_60 = 0;
static int32_t	gs_cylinder_rotation_is_up_60 = 0;
static int32_t	gs_cylinder_rotation_is_down_60 = 0;
static int32_t	gs_cylinder_centrage_is_front_60 = 0;
static int32_t	gs_cylinder_centrage_is_rear_60 = 0;
static int32_t	gs_cylinder_gripper_centrage_is_blocked_60 = 0;
static int32_t	gs_cylinder_gripper_centrage_is_unblocked_60 = 0;
static int32_t	gs_locked_station_60 = 0;
static int32_t	gs_unlocked_station_60 = 0;

static int32_t	gs_plugged_station_70 = 0;
static int32_t	gs_unplugged_station_70 = 0;
static int32_t	gs_locked_station_70 = 0;
static int32_t	gs_unlocked_station_70 = 0;
static int32_t	gs_mission_running_station_70 = 0;


static int32_t	gs_plugged_station_80 = 0;
static int32_t	gs_unplugged_station_80 = 0;
static int32_t	gs_locked_station_80 = 0;
static int32_t	gs_unlocked_station_80 = 0;
static int32_t	gs_cylinder_centrage_is_rear_80 = 0;
static int32_t	gs_cylinder_rotation_is_up_80 = 0;
static int32_t	gs_cylinder_rotation_is_down_station_80 = 0;
static int32_t	gs_cylinder_system_lev_is_up_80 = 0;
static int32_t	gs_cylinder_system_lev_is_down_80 = 0;	


//Get only the status of this sensors at the plug state	//HOUSSEM
static int32_t is_panel_locked_left_station_10 =0 ;
static int32_t is_panel_locked_right_station_10 =0 ;
static int32_t is_panel_unlocked_left_station_10 =0 ;
static int32_t is_panel_unlocked_right_station_10 =0;

static int32_t is_panel_locked_left_station_60 =0 ;
static int32_t is_panel_locked_right_station_60 =0 ;
static int32_t is_panel_unlocked_left_station_60 =0 ;
static int32_t is_panel_unlocked_right_station_60 =0 ;

static int32_t is_panel_locked_left_station_70 =0 ;
static int32_t is_panel_locked_right_station_70 =0 ;
static int32_t is_panel_unlocked_left_station_70 =0 ;
static int32_t is_panel_unlocked_right_station_70 =0 ;

static int32_t is_panel_locked_left_station_80 =0 ;
static int32_t is_panel_locked_right_station_80 =0 ;
static int32_t is_panel_unlocked_left_station_80 =0 ;
static int32_t is_panel_unlocked_right_station_80 =0 ;

static int32_t gs_unglug_command_station_10 =0 ;
static int32_t gs_unglug_command_station_60 =0 ;
static int32_t gs_unglug_command_station_70 =0 ;
static int32_t gs_unglug_command_station_80 =0 ;


				
static int32_t	gs_table_started_rotation = -1;
static int32_t	gs_table_finished_rotation = -1;

char Table_Ready[NUMBER_OF_STATIONS+1];

typedef enum _EStep
{
	CHECK_CYLINDER_LOCKED = 0,
	CHECK_CYLINDER_UNPLUGGED,
	CHECK_TABLE_POITION,
	CHECK_TABLE_IN_ROTATION,
	CHECK_IF_ROTATION_FINIHED,
	ROTATION_FINIHED_OK,
} EStep;

typedef enum _EStepRobotInit
{
	ROBOT_STEP_0 = 0,
	ROBOT_STEP_1,
	ROBOT_STEP_2,
	ROBOT_STEP_3,
	ROBOT_STEP_4,
	ROBOT_STEP_END,
} EStepRobotInit;

typedef struct _SLogicName
{
	char		aliasName[MAX_LENGTH_SENSOR_LOGICAL_NAME+1];
	char		logicalName[MAX_LENGTH_SENSOR_LOGICAL_NAME+1];
} SLogicName, *SLogicNamePtr;

typedef struct _SLogicNameList
{
	SLogicNamePtr	pLigicNames;
	int32_t			ListCount;
} SLogicNameList, *SLogicNameListPtr;

int CVICALLBACK Timer_Robot (int reserved, int theTimerId, int event,
                                 void *callbackData, int eventData1,
                                 int eventData2);
/* Tester */
static SElExceptionPtr Initialize(struct _STester*	me, const char*	fileName);
static SElExceptionPtr RegisterSensorCallbacks( struct _STester* me);
static SElExceptionPtr TableRotate(struct _STester* me, int32_t timeout, bool_t wait_end);
static SElExceptionPtr GetStationStatus(struct _STester* me, char status[9]);
static SElExceptionPtr WaitRotationFinished(struct _STester* me, int32_t timeout);
static SElExceptionPtr OkNokCheck(struct _STester* me, int32_t timeout, bool_t* ok);
static SElExceptionPtr NokCheck(struct _STester* me, int32_t timeout);
static SElExceptionPtr SetSensor(struct _STester* me, const char* sensorId, uint32_t value);
static SElExceptionPtr GetSensor(struct _STester* me, const char* sensorId, uint32_t* value);
static SElExceptionPtr SetSensorCallback(struct _STester* me, const char* sensorId, SensorCallbackFnc_t fCall, void* pData);
static SElExceptionPtr CallbackFncButtonOk(const char* name, uint32_t state, void* pData);
static SElExceptionPtr CallbackFncButtonNok(const char* name, uint32_t state, void* pData);
static SElExceptionPtr CallbackFncButtonMenu(const char* name, uint32_t state, void* pData);
static SElExceptionPtr CallbackFncButtonLabel(const char* name, uint32_t state, void* pData);
static SElExceptionPtr CallbackFncButtonDowntime(const char* name, uint32_t state, void* pData);
static SElExceptionPtr CallbackFncAbis(const char* name, uint32_t state, void* pData);
static SElExceptionPtr CallbackFncMissionRunning(const char* name, uint32_t state, void* pData);
static SElExceptionPtr CallbackFncButtonStart(const char* name, uint32_t state, void* pData);
static SElExceptionPtr CallbackFncJigNew(const char* name, uint32_t state, void* pData);
static SElExceptionPtr CallbackFncRotation(const char* name, uint32_t state, void* pData);
static SElExceptionPtr CallbackFncRotationStarted(const char* name, uint32_t state, void* pData);
static SElExceptionPtr CallbackFncButtonTotalStop(const char* name, uint32_t state, void* pData);
static SElExceptionPtr CallbackFncRedBoxEmpty(const char* name, uint32_t state, void* pData);
static SElExceptionPtr InitializeStations(struct _STester* me, int32_t station, int32_t timeout);
/* Station 10 */
static SElExceptionPtr PlugPanelStation10(struct _STester* me, int32_t timeout);
static SElExceptionPtr UnplugPanelStation10(struct _STester* me, int32_t timeout);
static SElExceptionPtr LockPanelStation10(struct _STester* me, int32_t timeout);
static SElExceptionPtr UnlockPanelStation10(struct _STester* me, int32_t timeout);
static SElExceptionPtr ReadyPrint(struct _STester* me, int32_t timeout);
static SElExceptionPtr CyclePrint(struct _STester* me, int32_t timeout);

/* Station 20 */
//Empty
/* Station 30 */
static SElExceptionPtr PlugPanelStation30(struct _STester* me, int32_t timeout);
static SElExceptionPtr UnplugPanelStation30(struct _STester* me, int32_t timeout);
static SElExceptionPtr LockPanelStation30(struct _STester* me, int32_t timeout);
static SElExceptionPtr UnlockPanelStation30(struct _STester* me, int32_t timeout);
static SElExceptionPtr MoveCylinderGripperCentrageBlockStation30(struct _STester* me, int32_t timeout);
static SElExceptionPtr MoveCylinderGripperCentrageUnblockStation30(struct _STester* me, int32_t timeout);
static SElExceptionPtr MoveCylinderCentrageFrontStation30(struct _STester* me, int32_t timeout);
static SElExceptionPtr MoveCylinderCentrageRearStation30(struct _STester* me, int32_t timeout);
static SElExceptionPtr MoveCylinderAirbagUpStation30(struct _STester* me, int32_t timeout);
static SElExceptionPtr MoveCylinderAirbagDownStation30(struct _STester* me, int32_t timeout);
/* Station 40 */
//Empty
/* Station 50 */
//Empty
/* Station 60 */
static SElExceptionPtr PlugPanelStation60(struct _STester* me, int32_t timeout);
static SElExceptionPtr UnplugPanelStation60(struct _STester* me, int32_t timeout);
static SElExceptionPtr MoveCylinderGripperCentrageBlockStation60(struct _STester* me, int32_t timeout);
static SElExceptionPtr MoveCylinderGripperCentrageUnblockStation60(struct _STester* me, int32_t timeout);
static SElExceptionPtr MoveCylinderCentrageFrontStation60(struct _STester* me, int32_t timeout);
static SElExceptionPtr MoveCylinderCentrageRearStation60(struct _STester* me, int32_t timeout);
static SElExceptionPtr MoveCylinderRotationUpStation60(struct _STester* me, int32_t timeout);
static SElExceptionPtr MoveCylinderRotationDownStation60(struct _STester* me, int32_t timeout);
static SElExceptionPtr LockPanelStation60(struct _STester* me, int32_t timeout);
static SElExceptionPtr UnlockPanelStation60(struct _STester* me, int32_t timeout);
static SElExceptionPtr RemoveInsertKeyStation60(struct _STester* me, int32_t timeout, bool_t wait_end , int32_t Mode);
static SElExceptionPtr ResetPickPlaceStation60(struct _STester* me, int32_t timeout, bool_t wait_end);
/* Station 70 */
static SElExceptionPtr PlugPanelStation70(struct _STester* me, int32_t timeout);
static SElExceptionPtr UnplugPanelStation70(struct _STester* me, int32_t timeout);
static SElExceptionPtr LockPanelStation70(struct _STester* me, int32_t timeout);
static SElExceptionPtr UnlockPanelStation70(struct _STester* me, int32_t timeout);
static SElExceptionPtr IlluminationStation70(struct _STester* me, int32_t timeout);
static SElExceptionPtr RobotInitStation70(struct _STester* me, int32_t timeout);
static SElExceptionPtr RobotSelectProgramStation70(struct _STester* me, int32_t program, int32_t timeout);
static SElExceptionPtr RobotStartMissionStation70(struct _STester* me, int32_t timeout);
static SElExceptionPtr RobotEndMissionStation70(struct _STester* me, int32_t timeout);
static SElExceptionPtr RobotEndTestMissionStation70(struct _STester* me, int32_t timeout);
static SElExceptionPtr RobotHomeStation70(struct _STester* me, int32_t timeout);


/* Station 80 */
static SElExceptionPtr PlugPanelStation80(struct _STester* me, int32_t timeout);
static SElExceptionPtr UnplugPanelStation80(struct _STester* me, int32_t timeout);
static SElExceptionPtr LockPanelStation80(struct _STester* me, int32_t timeout);
static SElExceptionPtr UnlockPanelStation80(struct _STester* me, int32_t timeout);
static SElExceptionPtr RemoveInsertKeyStation80(struct _STester* me, int32_t timeout, bool_t wait_end , int32_t Mode);
static SElExceptionPtr ResetPickPlaceStation80(struct _STester* me, int32_t timeout, bool_t wait_end);
static SElExceptionPtr MoveCylinderGripperCentrageBlockStation80(struct _STester* me, int32_t timeout);
static SElExceptionPtr MoveCylinderGripperCentrageUnblockStation80(struct _STester* me, int32_t timeout);
static SElExceptionPtr MoveCylinderRotationUpStation80(struct _STester* me, int32_t timeout);
static SElExceptionPtr MoveCylinderRotationDownStation80(struct _STester* me, int32_t timeout);
static SElExceptionPtr MoveCylinderSystemLevDownStation80(struct _STester* me, int32_t timeout);
static SElExceptionPtr MoveCylinderSystemLevUpStation80(struct _STester* me, int32_t timeout);
static SElExceptionPtr TestChecker_Station80(struct _STester* me, int32_t *output0, int32_t *output1, int32_t *output2, int32_t *output3);



static SElExceptionPtr GetItemCode(struct _STester* me, const char* product, int32_t index, long bufferSize, char* buffer,int32_t timeout);


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
#undef  __FUNC__
#define __FUNC__ "tester_new"
 SElExceptionPtr tester_new(STesterPtr*	pTester)
{
	SElExceptionPtr    	pexception = NULL;
	int32_t				error = 0;

	*pTester = calloc( 1, sizeof(STester));
	EXCCHECKALLOC( *pTester);

	/* Tester */
	(*pTester)->Initialize = Initialize;
	(*pTester)->SetSensor = SetSensor;
	(*pTester)->GetSensor = GetSensor;
	(*pTester)->TableRotate = TableRotate;
	(*pTester)->GetStationStatus = GetStationStatus;
	(*pTester)->InitializeStations = InitializeStations;
	(*pTester)->WaitRotationFinished = WaitRotationFinished;
	(*pTester)->OkNokCheck = OkNokCheck;
	(*pTester)->NokCheck = NokCheck;
	(*pTester)->RegisterSensorCallbacks = RegisterSensorCallbacks;
	(*pTester)->SetSensorCallback = SetSensorCallback;
	/* Station 10 */
	(*pTester)->PlugPanelStation10 = PlugPanelStation10;
	(*pTester)->UnplugPanelStation10 = UnplugPanelStation10;
	(*pTester)->LockPanelStation10 = LockPanelStation10;
	(*pTester)->UnlockPanelStation10 = UnlockPanelStation10;
	(*pTester)->ReadyPrint = ReadyPrint;
	(*pTester)->CyclePrint = CyclePrint;

	/* Station 20 */
	 //empty
	/* Station 30 */
	(*pTester)->PlugPanelStation30 = PlugPanelStation30;
	(*pTester)->UnplugPanelStation30 = UnplugPanelStation30;
	(*pTester)->LockPanelStation30 = LockPanelStation30;
	(*pTester)->UnlockPanelStation30 = UnlockPanelStation30;
	(*pTester)->MoveCylinderGripperCentrageBlockStation30 = MoveCylinderGripperCentrageBlockStation30;
	(*pTester)->MoveCylinderGripperCentrageUnblockStation30 = MoveCylinderGripperCentrageUnblockStation30;
	(*pTester)->MoveCylinderCentrageFrontStation30 = MoveCylinderCentrageFrontStation30;
	(*pTester)->MoveCylinderCentrageRearStation30 = MoveCylinderCentrageRearStation30;
	(*pTester)->MoveCylinderAirbagUpStation30 = MoveCylinderAirbagUpStation30;
	(*pTester)->MoveCylinderAirbagDownStation30 = MoveCylinderAirbagDownStation30;
	/* Station 40 */
	//Empty
	/* Station 50 */
	//Empty
	/* Station 60 */
	(*pTester)->PlugPanelStation60 = PlugPanelStation60;
	(*pTester)->UnplugPanelStation60 = UnplugPanelStation60;
	(*pTester)->MoveCylinderGripperCentrageBlockStation60 = MoveCylinderGripperCentrageBlockStation60;
	(*pTester)->MoveCylinderGripperCentrageUnblockStation60 = MoveCylinderGripperCentrageUnblockStation60;
	(*pTester)->MoveCylinderCentrageFrontStation60 = MoveCylinderCentrageFrontStation60;
	(*pTester)->MoveCylinderCentrageRearStation60 = MoveCylinderCentrageRearStation60;
	(*pTester)->MoveCylinderRotationUpStation60 = MoveCylinderRotationUpStation60;
	(*pTester)->MoveCylinderRotationDownStation60 = MoveCylinderRotationDownStation60;
	(*pTester)->LockPanelStation60 = LockPanelStation60;
	(*pTester)->UnlockPanelStation60 = UnlockPanelStation60;
	(*pTester)->RemoveInsertKeyStation60 = RemoveInsertKeyStation60 ;
	(*pTester)->ResetPickPlaceStation60 = ResetPickPlaceStation60; 
	/* Station 70 */
	(*pTester)->PlugPanelStation70 = PlugPanelStation70;
	(*pTester)->UnplugPanelStation70 = UnplugPanelStation70;
	(*pTester)->LockPanelStation70 = LockPanelStation70;
	(*pTester)->UnlockPanelStation70 = UnlockPanelStation70;
	(*pTester)->IlluminationStation70 = IlluminationStation70;
	(*pTester)->RobotInitStation70 = RobotInitStation70;
	(*pTester)->RobotSelectProgramStation70 = RobotSelectProgramStation70;
	(*pTester)->RobotStartMissionStation70 = RobotStartMissionStation70;
	(*pTester)->RobotHomeStation70 = RobotHomeStation70;
	(*pTester)->RobotEndMissionStation70 = RobotEndMissionStation70;
	(*pTester)->RobotEndTestMissionStation70 = RobotEndTestMissionStation70;
	/* Station 80 */
	(*pTester)->PlugPanelStation80 = PlugPanelStation80;
	(*pTester)->UnplugPanelStation80 = UnplugPanelStation80;
	(*pTester)->RemoveInsertKeyStation80 = RemoveInsertKeyStation80 ;
	(*pTester)->ResetPickPlaceStation80 = ResetPickPlaceStation80; 
	(*pTester)->LockPanelStation80 = LockPanelStation80;
	(*pTester)->UnlockPanelStation80 = UnlockPanelStation80;
	(*pTester)->MoveCylinderRotationUpStation80 =	MoveCylinderRotationUpStation80;
	(*pTester)->MoveCylinderRotationDownStation80 =	MoveCylinderRotationDownStation80;
	(*pTester)->MoveCylinderSystemLevDownStation80 =	MoveCylinderSystemLevDownStation80;
	(*pTester)->MoveCylinderSystemLevUpStation80 =	MoveCylinderSystemLevUpStation80;
	(*pTester)->TestChecker_Station80 =	TestChecker_Station80;

	(*pTester)->GetItemCode = GetItemCode;


	
Error:
	EXCRETHROW( pexception); 
}/* vline_new */

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "tester_delete"
SElExceptionPtr	tester_delete(STesterPtr* pTester)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	int32_t			status;

	if ( pTester && *pTester )
	{
		(*pTester)->eventAPI = NULL;
		(*pTester)->sensorAPI = NULL;

		free( (*pTester)->pLogicalNames);
		(*pTester)->pLogicalNames = NULL;
		(*pTester)->LogicalCounts = 0;
		
		free( *pTester);
		*pTester = NULL;
	}

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"Tester::RegisterSensorCallbacks"
static SElExceptionPtr RegisterSensorCallbacks( struct _STester* me)
{
	SElExceptionPtr	pexception = NULL;

	EXCCHECK( me->SetSensorCallback(me, "BUTTON_OK", CallbackFncButtonOk, me));
	EXCCHECK( me->SetSensorCallback(me, "BUTTON_NOK", CallbackFncButtonNok, me));
	EXCCHECK( me->SetSensorCallback(me, "BUTTON_MENU", CallbackFncButtonMenu, me));
	//EXCCHECK( me->SetSensorCallback(me, "BUTTON_LABEL", CallbackFncButtonLabel, me));
	//EXCCHECK( me->SetSensorCallback(me, "BUTTON_DOWNTIME", CallbackFncButtonDowntime, me));
	EXCCHECK( me->SetSensorCallback(me, "BUTTON_START", CallbackFncButtonStart, me));

	EXCCHECK( me->SetSensorCallback(me, "MICRO_CAM", CallbackFncRotation, me));
	EXCCHECK( me->SetSensorCallback(me, "BUTTON_TOTAL_STOP", CallbackFncButtonTotalStop, me));
	EXCCHECK( me->SetSensorCallback(me, "SELECTEUR_ABIS", CallbackFncAbis, me));
	EXCCHECK( me->SetSensorCallback(me, "MISSION_RUNNING", CallbackFncMissionRunning, me));
	//EXCCHECK( me->SetSensorCallback(me, "SENSOR_RED_BOX_FULL", CallbackFncRedBoxEmpty, me));

Error:
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
static int CompareFnc(const void* v1, const void* v2)
{
	SLogicNamePtr	value1 = (SLogicNamePtr) v1;
	SLogicNamePtr	value2 = (SLogicNamePtr) v2;
	
	return strcmp(value1->aliasName, value2->aliasName);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"Tester::Initialize"
static SElExceptionPtr Initialize
(
	struct _STester*	me,
	const char*		fileName
)
{
	SElExceptionPtr	pexception = NULL;
	CAObjHandle		xml_doc = 0;
	CAObjHandle		sensors_list = 0;
	CAObjHandle		sensor = 0;
	CAObjHandle		alias = 0;
	VBOOL			is_xml_success = 1;
	char*			alias_name = NULL;
	char*			logical_name = NULL;
	const char*     pfile_name = (fileName && fileName[0] != '\0') ? fileName : "modules\\tester.xml";
	char			module_dir[512];
	VARIANT         pfile_name_var = CA_VariantEmpty();
	SLogicNamePtr	plogical_list = NULL;
	int32_t			count = 0,
					error = 0;
	HRESULT			hr;
	char			sNode[512];
	int             iStation;
	CAObjHandle		bcr_list = 0;
	CAObjHandle		barcode = 0;
	CAObjHandle		dll_name = 0;
	char*			dll_name_str = NULL;
	char			directory_path[FILE_NAME_SIZE] = "";
	char			bcr_dll_file[FILE_NAME_SIZE] = "";
	char			message[2048] = "";
	char*			config_xml = NULL;
	int size;
	
	
	
	InitializeCriticalSection( &_Lock);

	strcpy( me->path, pfile_name);
	EXCCHECKCOM( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &xml_doc));
	EXCCHECKCVI( CA_VariantSetCString(  &pfile_name_var, pfile_name));
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (xml_doc, NULL, pfile_name_var, &is_xml_success));

	/* Tester */
	sprintf(sNode, "//module[@id='"MODULE_TESTER"']/Sensors/Sensor");
	EXCCHECKCOM( MSXML_IXMLDOMDocumentselectNodes (xml_doc, NULL, sNode, &sensors_list));
	EXCCHECKCOM( MSXML_IXMLDOMNodeListreset (sensors_list, NULL));
	EXCCHECKCOM( MSXML_IXMLDOMNodeListnextNode (sensors_list, NULL, &sensor));
	
	while (sensor != 0)
	{
		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(sensor, NULL, "@name", &alias));
		EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (alias, NULL, &alias_name));
		EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (sensor, NULL, &logical_name));
		
		plogical_list = realloc(plogical_list, (count + 1) * sizeof(SLogicName));
		EXCCHECKALLOC( plogical_list);
		memset(plogical_list + count, 0, sizeof(SLogicName));
		strncpy((plogical_list + count)->aliasName, alias_name, MAX_LENGTH_SENSOR_LOGICAL_NAME);
		strncpy((plogical_list + count)->logicalName, logical_name, MAX_LENGTH_SENSOR_LOGICAL_NAME);
		count++;
		
		CA_FreeMemory(alias_name); alias_name = NULL;
		CA_FreeMemory(logical_name); logical_name = NULL;
		CA_DiscardObjHandle(sensor); sensor = 0;
		
		EXCCHECKCOM( MSXML_IXMLDOMNodeListnextNode (sensors_list, NULL, &sensor));
	}
	/* Stations */
	for (iStation = 1 ; iStation < NUMBER_OF_STATIONS ; iStation++)
	{
	sprintf(sNode, "//module[@id='"MODULE_TESTER"']/Station%d/Sensor", iStation);
	EXCCHECKCOM( MSXML_IXMLDOMDocumentselectNodes (xml_doc, NULL,sNode,&sensors_list));
	EXCCHECKCOM( MSXML_IXMLDOMNodeListreset (sensors_list, NULL));
	EXCCHECKCOM( MSXML_IXMLDOMNodeListnextNode (sensors_list, NULL, &sensor));
	
	while (sensor != 0)
	{
		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(sensor, NULL, "@name", &alias));
		EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (alias, NULL, &alias_name));
		EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (sensor, NULL, &logical_name));
		
		plogical_list = realloc(plogical_list, (count + 1) * sizeof(SLogicName));
		EXCCHECKALLOC( plogical_list);
		memset(plogical_list + count, 0, sizeof(SLogicName));
		strncpy((plogical_list + count)->aliasName, alias_name, MAX_LENGTH_SENSOR_LOGICAL_NAME);
		strncpy((plogical_list + count)->logicalName, logical_name, MAX_LENGTH_SENSOR_LOGICAL_NAME);
		count++;
		
		CA_FreeMemory(alias_name); alias_name = NULL;
		CA_FreeMemory(logical_name); logical_name = NULL;
		CA_DiscardObjHandle(sensor); sensor = 0;
		
		EXCCHECKCOM( MSXML_IXMLDOMNodeListnextNode (sensors_list, NULL, &sensor));
	}
	}
	
	qsort(plogical_list, count, sizeof(SLogicName), CompareFnc);
	
	me->pLogicalNames = (void*) plogical_list;
	me->LogicalCounts = count;
	
/* TODO: load barcode drivers */   //skipped HOUSSEM
	EXCCHECKCOM( MSXML_IXMLDOMDocumentselectNodes (xml_doc, NULL,
						"//module[@id='"MODULE_TESTER"']/BarCodeReaders/BarCodeReader",
						&bcr_list));
	if (bcr_list)
	{
		EXCCHECKCOM( MSXML_IXMLDOMNodeListreset (bcr_list, NULL));
		EXCCHECKCOM( MSXML_IXMLDOMNodeListnextNode (bcr_list, NULL, &barcode));
		while (barcode)
		{
			SBCRPtr	last_bcr = NULL;
			
			me->pBarCodeReader = realloc(me->pBarCodeReader, (me->BCRCounts + 1) * sizeof(SBCR));
			EXCCHECKALLOC(me->pBarCodeReader);
			last_bcr = me->pBarCodeReader + (me->BCRCounts);
			memset(last_bcr, 0, sizeof(SBCR));
			me->BCRCounts++;
			
			EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(barcode, NULL, "Driver/@name", &dll_name));
			if (dll_name != 0)
			{
				EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (dll_name, NULL, &dll_name_str));
				EXCCHECKCVI( GetModuleDir (__CVIUserHInst, directory_path));
				sprintf(bcr_dll_file, "%s\\%s", directory_path, dll_name_str);
				last_bcr->dllHandle = LoadLibrary(bcr_dll_file);
				if (last_bcr->dllHandle == NULL)
				{
					sprintf(message, "Unable to load barcode reader driver\n(%s)", bcr_dll_file);
					EXCTHROW( TESTER_ERROR_LOAD_LIBRARY, message);
				}
				
				last_bcr->bcr_new = (SElExceptionPtr (*)(SBarCodeReaderPtr*))
											GetProcAddress(last_bcr->dllHandle,
														   "bcr_new");
				last_bcr->bcr_delete = (SElExceptionPtr (*)(SBarCodeReaderPtr*))
											GetProcAddress(last_bcr->dllHandle,
														   "bcr_delete");
				
				EXCCHECKCOM( MSXML_IXMLDOMNodeGetxml (barcode, NULL, &config_xml));
			
				if (last_bcr->bcr_new)
				{
					last_bcr->bcr_new(&(last_bcr->reader));
					EXCCHECK( (last_bcr->reader)->Initialize(last_bcr->reader, config_xml));
					last_bcr->reader->eventFactory = me->eventAPI;
				}
				
				CA_FreeMemory(dll_name_str); dll_name_str = NULL;
				CA_FreeMemory(config_xml); config_xml = NULL;
				CA_DiscardObjHandle(dll_name); dll_name = 0;
			}
			CA_DiscardObjHandle(barcode); barcode = 0;
			EXCCHECKCOM( MSXML_IXMLDOMNodeListnextNode (bcr_list, NULL, &barcode));
		}   
	}	 				  
	
Error:
	if (alias) CA_DiscardObjHandle(alias);
	if (sensor) CA_DiscardObjHandle(sensor);
	if (sensors_list) CA_DiscardObjHandle(sensors_list);
	if (xml_doc) CA_DiscardObjHandle(xml_doc);
	if (alias_name) CA_FreeMemory(alias_name);
	if (logical_name) CA_FreeMemory(logical_name);
	CA_VariantClear(&pfile_name_var);
	
	if (pexception)
	{
		free(plogical_list);
		plogical_list = NULL;
		me->pLogicalNames = NULL;
		me->LogicalCounts = 0;
	}
	
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"Tester::TableRotate"
static SElExceptionPtr TableRotate(struct _STester* me, int32_t timeout, bool_t wait_end)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
		uint32_t		step = 0;
	
	bool_t			status_barrier = FALSE,
					status_micro_cam = FALSE,
					is_table_rotation_finished = FALSE;
	
	uint32_t		status_cylinder_airbag_up_station_30,
					status_cylinder_airbag_down_station_30,
					status_cylinder_centrage_front_station_30,
					status_cylinder_centrage_rear_station_30,
					status_cylinder_rotation_up_station_60,
					status_cylinder_rotation_down_station_60,
					status_cylinder_centrage_front_station_60,
					status_cylinder_centrage_rear_station_60,
					is_cylinder_y_rear_station_60,
					is_cylinder_y_front_station_60,
					is_cylinder_z_up_station_60,
					is_cylinder_z_down_station_60,
					status_cylinder_rotation_up_station_80,
					status_cylinder_rotation_down_station_80,
					status_cylinder_system_lev_up_station_80,
					status_cylinder_system_lev_down_station_80,
					is_cylinder_z_up_station_80,
					is_cylinder_z_down_station_80,
					is_cylinder_x_rear_station_80,
					is_cylinder_x_front_station_80,
					is_cylinder_y_rear_station_80,
					is_cylinder_y_front_station_80;

	bool_t			printer_up = FALSE;
	
	bool_t			sfae_bit = FALSE,
					home_bit = FALSE,
					mission_running;
			
	bool_t			is_station_10_ready = FALSE,
					is_station_20_ready = FALSE,
					is_station_30_ready = FALSE,
					is_station_40_ready = FALSE,
					is_station_50_ready = FALSE,
					is_station_60_ready = FALSE,
					is_station_70_ready = FALSE,
					is_station_80_ready = FALSE;

	
			/* STATION 10 */
			EXCCHECK( me->GetSensor(me, "PRINTER_INPUT2", &printer_up));
			if(printer_up)
			{
				is_station_10_ready = TRUE;
			}
			else 
				is_station_10_ready = FALSE;
	
			 //add printer
			/* STATION 20 */
			is_station_20_ready = TRUE;
			
			/* STATION 30 */
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_AIRBAG_UP_STATION_30", &status_cylinder_airbag_up_station_30));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_AIRBAG_DOWN_STATION_30", &status_cylinder_airbag_down_station_30));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_CENTRAGE_FRONT_STATION_30", &status_cylinder_centrage_front_station_30));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_CENTRAGE_REAR_STATION_30", &status_cylinder_centrage_rear_station_30));
			if(status_cylinder_airbag_up_station_30 && !status_cylinder_airbag_down_station_30 &&
				status_cylinder_centrage_rear_station_30 && !status_cylinder_centrage_front_station_30)
			{
				is_station_30_ready = TRUE;
			}
			else 
				is_station_30_ready = FALSE;
			/* STATION 40 */
			is_station_40_ready = TRUE;
			/* STATION 50 */
			is_station_50_ready = TRUE;
			/* STATION 60 */
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Y_FRONT_STATION_60", &is_cylinder_y_front_station_60));
	  		EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Y_REAR_STATION_60", &is_cylinder_y_rear_station_60));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60", &is_cylinder_z_up_station_60));
	  		EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_60", &is_cylinder_z_down_station_60));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_ROTATION_UP_STATION_60", &status_cylinder_rotation_up_station_60));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_ROTATION_DOWN_STATION_60", &status_cylinder_rotation_down_station_60));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_CENTRAGE_FRONT_STATION_60", &status_cylinder_centrage_front_station_60));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_CENTRAGE_REAR_STATION_60", &status_cylinder_centrage_rear_station_60));
			if( is_cylinder_y_rear_station_60 && !is_cylinder_y_front_station_60  &&
				is_cylinder_z_up_station_60   && !is_cylinder_z_down_station_60   &&
				status_cylinder_rotation_up_station_60 && !status_cylinder_rotation_down_station_60 &&
				status_cylinder_centrage_rear_station_60 && !status_cylinder_centrage_front_station_60)
			{
				is_station_60_ready = TRUE;
			}
			else 
				is_station_60_ready = FALSE;

			/* STATION 70 */
			EXCCHECK( me->GetSensor(me, "ROBOT_IN_FREE_AREA", &sfae_bit));
			EXCCHECK( me->GetSensor(me, "HOME_POSITION", &home_bit));
			EXCCHECK( me->GetSensor(me, "MISSION_RUNNING", &mission_running));
			if(sfae_bit == TRUE && home_bit == TRUE && mission_running == FALSE)
			{
				is_station_70_ready = TRUE;
			}
			else 
				is_station_70_ready = FALSE;
			
			/* STATION 80 */
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_ROTATION_UP_STATION_80", &status_cylinder_rotation_up_station_80));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_ROTATION_DOWN_STATION_80", &status_cylinder_rotation_down_station_80));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_SYSTEM_LEV_UP_Station_80", &status_cylinder_system_lev_up_station_80));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_SYSTEM_LEV_DOWN_Station_80", &status_cylinder_system_lev_down_station_80));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Y_FRONT_STATION_80", &is_cylinder_y_front_station_80));
	  		EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Y_REAR_STATION_80", &is_cylinder_y_rear_station_80));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_80", &is_cylinder_z_up_station_80));
	  		EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_80", &is_cylinder_z_down_station_80));
			if( is_cylinder_z_up_station_80   && !is_cylinder_z_down_station_80	 &&
				is_cylinder_y_rear_station_80 && !is_cylinder_y_front_station_80 &&
			    status_cylinder_rotation_up_station_80 && !status_cylinder_rotation_down_station_80 &&
			    status_cylinder_system_lev_up_station_80 && !status_cylinder_system_lev_down_station_80)
				{
				is_station_80_ready = TRUE;
				}
			else 
				is_station_80_ready = FALSE;
			
		
	if(is_station_10_ready &&
	   is_station_20_ready &&
	   is_station_30_ready &&
	   is_station_40_ready && 
	   is_station_50_ready && 
	   is_station_60_ready && 
	   is_station_70_ready && 
	   is_station_80_ready) 
	{
		
	TIMEOUT_INIT();
	step = CHECK_CYLINDER_LOCKED;
	
	while( TRUE)
		{
		
		/* Freeza all cylinder to move during rotation */
		if (step >= CHECK_TABLE_POITION)
		{
		me->SetSensor(me, "CYLINDER_PLUG_STATION_10", FALSE);
		me->SetSensor(me, "CYLINDER_PLUG_STATION_30", FALSE);
		me->SetSensor(me, "CYLINDER_PLUG_STATION_60", FALSE);
		me->SetSensor(me, "CYLINDER_PLUG_STATION_70", FALSE);
		me->SetSensor(me, "CYLINDER_PLUG_STATION_80", FALSE);
		
		me->SetSensor(me, "CYLINDER_GRIPPER_CENTRAGE_BLOCK_STATION_30", FALSE);
		me->SetSensor(me, "CYLINDER_AIRBAG_DOWN_STATION_30", FALSE);
		me->SetSensor(me, "CYLINDER_GRIPPER_CENTRAGE_BLOCK_STATION_60", FALSE);
		me->SetSensor(me, "CYLINDER_ROTATION_DOWN_STATION_60", FALSE);
		me->SetSensor(me, "CYLINDER_ROTATION_DOWN_STATION_80", FALSE);
		me->SetSensor(me, "CYLINDER_SYSTEM_LEV_DOWN_STATION_80", FALSE);
		me->SetSensor(me, "PRINTER_AIR_VALVE", FALSE); //Printer protection
		
		me->SetSensor(me, "CYLINDER_UNPLUG_STATION_10", TRUE);
		me->SetSensor(me, "CYLINDER_UNPLUG_STATION_30", TRUE);
		me->SetSensor(me, "CYLINDER_UNPLUG_STATION_60", TRUE);
		me->SetSensor(me, "CYLINDER_UNPLUG_STATION_70", TRUE);
		me->SetSensor(me, "CYLINDER_UNPLUG_STATION_80", TRUE);
		
		me->SetSensor(me, "CYLINDER_GRIPPER_CENTRAGE_UNBLOCK_STATION_30", TRUE);
		me->SetSensor(me, "CYLINDER_AIRBAG_UP_STATION_30", TRUE);
		me->SetSensor(me, "CYLINDER_GRIPPER_CENTRAGE_UNBLOCK_STATION_60", TRUE);
		me->SetSensor(me, "CYLINDER_ROTATION_UP_STATION_60", TRUE);
		me->SetSensor(me, "CYLINDER_ROTATION_UP_STATION_80", TRUE);
		me->SetSensor(me, "CYLINDER_SYSTEM_LEV_UP_STATION_80", TRUE);
		}
		
		  EXCCHECK( me->GetSensor(me, "INFRARED_BARRIER", &status_barrier));
		  if (status_barrier == 0)
		  {
  	 	  Sleep(1000);
	   	  EXCCHECK( me->GetSensor(me, "MICRO_CAM", &status_micro_cam));
	   	  if(status_micro_cam == 0)// in not indexed poition 
				EXCTHROW( TESTER_ERROR_TIMEOUT, "Plateau pas en position");
		  else  if(status_micro_cam == 1 && (step == CHECK_CYLINDER_LOCKED || step == CHECK_CYLINDER_UNPLUGGED))// in not indexed poition 
				EXCTHROW( TESTER_ERROR_TIMEOUT, "Erreur démarrage Plateau");
		  }
	  
		/* si aprés passage barriére un véin sorte il doit vérifier un autre fois is_table_ready should be = 0 */
		else if (!is_table_rotation_finished)
		{
			switch  (step)
			{
				
			 case  CHECK_CYLINDER_LOCKED : /* check if cylinders are locked */
				   		
				  	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &status_micro_cam));

				 	if(status_micro_cam == 1 && wait_end == TRUE)// in indexed poition 
					{
					me->PlugPanelStation30(me, 0);
					me->PlugPanelStation60(me, 0);
					me->PlugPanelStation70(me, 0);
					me->PlugPanelStation80(me, 0);
					me->PlugPanelStation10(me, 2000);
						
					me->LockPanelStation30(me, 0);
					me->LockPanelStation60(me, 0);
					me->LockPanelStation70(me, 0);
					me->LockPanelStation80(me, 0);
					me->LockPanelStation10(me, 2000);
					
						if(gs_locked_station_10 &&
						   gs_locked_station_30 &&	
						   gs_locked_station_60 &&	
						   gs_locked_station_70 &&	
						   gs_locked_station_80) 
							{
							me->SetSensor(me, "CYLINDER_LOCK_STATION_10", FALSE);
							me->SetSensor(me, "CYLINDER_LOCK_STATION_30", FALSE);
							me->SetSensor(me, "CYLINDER_LOCK_STATION_60", FALSE);
							me->SetSensor(me, "CYLINDER_LOCK_STATION_70", FALSE);
							me->SetSensor(me, "CYLINDER_LOCK_STATION_80", FALSE);
							me->SetSensor(me, "CYLINDER_UNLOCK_STATION_10", FALSE);
							me->SetSensor(me, "CYLINDER_UNLOCK_STATION_30", FALSE);
							me->SetSensor(me, "CYLINDER_UNLOCK_STATION_60", FALSE);
							me->SetSensor(me, "CYLINDER_UNLOCK_STATION_70", FALSE);
							me->SetSensor(me, "CYLINDER_UNLOCK_STATION_80", FALSE);
				 			me->UnplugPanelStation10(me, 0);
							me->UnplugPanelStation30(me, 0);
							me->UnplugPanelStation60(me, 0);
							me->UnplugPanelStation70(me, 0);
				 			me->UnplugPanelStation80(me, 0);
							step ++;
							}
					}
				
					else 
					{
						step++;
					}
		  
				break;
			
			  case  CHECK_CYLINDER_UNPLUGGED : /* check if cylinders are unplugged */
				   		
				 	me->UnplugPanelStation10(me, 0);
					me->UnplugPanelStation30(me, 0);
					me->UnplugPanelStation60(me, 0);
					me->UnplugPanelStation70(me, 0);
				 	me->UnplugPanelStation80(me, 0);
					
					if(gs_unplugged_station_10 && 
					   gs_unplugged_station_30 && 
					   gs_unplugged_station_60 && 
					   gs_unplugged_station_70 && 
					   gs_unplugged_station_80) 
							{
							step ++;
							}

		  	 break;
			
			 
			 case  CHECK_TABLE_POITION : /* check the table in wich position and start rotation */
				   	
				   		EXCCHECK( me->GetSensor(me, "MICRO_CAM", &status_micro_cam));
				   
						if(status_micro_cam == 1 && gs_unplugged_station_10)// in indexed poition 
							{
				 				EXCCHECK( me->SetSensor(me, "START_ROTATION", 1));
								EXCCHECK( me->SetSensor(me, "FORCAGE_ROTATION", 1));
								step ++;		
				 			}  
				 		else if(status_micro_cam == 0 && gs_unplugged_station_10)// in not indexed poition 
							{
								EXCCHECK( me->SetSensor(me, "FORCAGE_ROTATION", 1));// old START_ROTATION = 0 HOUSSEM
								step ++;		
				 			}
			break;
			
			
			case  CHECK_TABLE_IN_ROTATION :
				   		
				   EXCCHECK( me->GetSensor(me, "MICRO_CAM", &status_micro_cam));
					if(status_micro_cam == 0)// in rotation now
					{
						EXCCHECK( me->SetSensor(me, "START_ROTATION", 0));//  old FORCAGE_ROTATION  = 0 HOUSSEM
						step ++;
					}
			break;
			
			
			case  CHECK_IF_ROTATION_FINIHED :
				   		
				   EXCCHECK( me->GetSensor(me, "MICRO_CAM", &status_micro_cam));
						if(status_micro_cam == 1)// rotation finished 
							{
				 				EXCCHECK( me->SetSensor(me, "FORCAGE_ROTATION", 0));
								EXCCHECK( me->SetSensor(me, "START_ROTATION", 0));
								step ++;		
				 			}  
			break;
				
			case  ROTATION_FINIHED_OK :
					
				 is_table_rotation_finished = TRUE;
		
			break;
				
			}
	
		}
		
		else if (is_table_rotation_finished) break;

		TIMEOUT_CHECK( timeout);
		}

	}
	
	else EXCTHROW( TESTER_ERROR_TIMEOUT, "Conditions Pour Rotation Plateau pas prets");

Error:
	me->SetSensor(me, "FORCAGE_ROTATION", 0);
	me->SetSensor(me, "START_ROTATION", 0);
	EXCRETHROW(pexception);
}
//*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"Tester::GetStationStatus"
static SElExceptionPtr GetStationStatus(struct _STester* me, char Table_Ready[9])
{
	SElExceptionPtr	pexception = NULL;

		uint32_t	is_panel_plugged_station_10,
					is_panel_unplugged_station_10,
					is_panel_plugged_station_30,
					is_panel_unplugged_station_30,				
					status_cylinder_airbag_up_station_30,
					status_cylinder_airbag_down_station_30,
					status_cylinder_centrage_front_station_30,
					status_cylinder_centrage_rear_station_30 ,
					status_cylinder_rotation_up_station_60,
					status_cylinder_rotation_down_station_60,
					status_cylinder_centrage_front_station_60,
					status_cylinder_centrage_rear_station_60 ,
					is_cylinder_y_rear_station_60,
					is_cylinder_y_front_station_60,
					is_cylinder_z_up_station_60,
					is_cylinder_z_down_station_60,
					is_panel_plugged_station_60,
					is_panel_unplugged_station_60,
					is_panel_plugged_station_70,
					is_panel_unplugged_station_70,
					is_panel_plugged_station_80,
					is_panel_unplugged_station_80,
					status_cylinder_rotation_up_station_80,
					status_cylinder_rotation_down_station_80,
					status_cylinder_system_lev_up_station_80,
					status_cylinder_system_lev_down_station_80,
					is_cylinder_z_up_station_80,
					is_cylinder_z_down_station_80,
					is_cylinder_x_rear_station_80,
					is_cylinder_x_front_station_80,
					is_cylinder_y_rear_station_80,
					is_cylinder_y_front_station_80;

	bool_t			safe_bit = FALSE,
					home_bit = FALSE,
					mission_running;
	
	bool_t			printer_up = FALSE;
	
			
	bool_t			is_station_10_ready = FALSE,
					is_station_20_ready = FALSE,
					is_station_30_ready = FALSE,
					is_station_40_ready = FALSE,
					is_station_50_ready = FALSE,
					is_station_60_ready = FALSE,
					is_station_70_ready = FALSE,
					is_station_80_ready = FALSE;

	


			/* STATION 10 */
			EXCCHECK( me->GetSensor(me, "PRINTER_INPUT2", &printer_up));
			if(printer_up)
			{
				is_station_10_ready = TRUE;
			}
			else 
				is_station_10_ready = FALSE;

			
			/* STATION 20 */
			is_station_20_ready = TRUE;
		
			
			/* STATION 30 */
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_AIRBAG_UP_STATION_30", &status_cylinder_airbag_up_station_30));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_AIRBAG_DOWN_STATION_30", &status_cylinder_airbag_down_station_30));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_CENTRAGE_FRONT_STATION_30", &status_cylinder_centrage_front_station_30));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_CENTRAGE_REAR_STATION_30", &status_cylinder_centrage_rear_station_30));
			if(status_cylinder_airbag_up_station_30 && !status_cylinder_airbag_down_station_30 &&
				status_cylinder_centrage_rear_station_30 && !status_cylinder_centrage_front_station_30)
			{
				is_station_30_ready = TRUE;
			}
			else 
				is_station_30_ready = FALSE;
			
			/* STATION 40 */
			is_station_40_ready = TRUE;
			/* STATION 50 */
			is_station_50_ready = TRUE;
			/* STATION 60 */
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Y_FRONT_STATION_60", &is_cylinder_y_front_station_60));
	  		EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Y_REAR_STATION_60", &is_cylinder_y_rear_station_60));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60", &is_cylinder_z_up_station_60));
	  		EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_60", &is_cylinder_z_down_station_60));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_ROTATION_UP_STATION_60", &status_cylinder_rotation_up_station_60));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_ROTATION_DOWN_STATION_60", &status_cylinder_rotation_down_station_60));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_CENTRAGE_FRONT_STATION_60", &status_cylinder_centrage_front_station_60));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_CENTRAGE_REAR_STATION_60", &status_cylinder_centrage_rear_station_60));
			if( is_cylinder_y_rear_station_60 && !is_cylinder_y_front_station_60  &&
				is_cylinder_z_up_station_60   && !is_cylinder_z_down_station_60   &&
				status_cylinder_rotation_up_station_60 && !status_cylinder_rotation_down_station_60 &&
				status_cylinder_centrage_rear_station_60 && !status_cylinder_centrage_front_station_60)
			{
				is_station_60_ready = TRUE;
			}
			else 
				is_station_60_ready = FALSE;

			/* STATION 70 */
			EXCCHECK( me->GetSensor(me, "ROBOT_IN_FREE_AREA", &safe_bit));
			EXCCHECK( me->GetSensor(me, "HOME_POSITION", &home_bit));
			EXCCHECK( me->GetSensor(me, "MISSION_RUNNING", &mission_running));
			if(safe_bit == TRUE && home_bit == TRUE && mission_running == FALSE)
			{
				is_station_70_ready = TRUE;
			}
			else 
				is_station_70_ready = FALSE;
			
			/* STATION 80 */
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_ROTATION_UP_STATION_80", &status_cylinder_rotation_up_station_80));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_ROTATION_DOWN_STATION_80", &status_cylinder_rotation_down_station_80));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_SYSTEM_LEV_UP_Station_80", &status_cylinder_system_lev_up_station_80));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_SYSTEM_LEV_DOWN_Station_80", &status_cylinder_system_lev_down_station_80));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Y_FRONT_STATION_80", &is_cylinder_y_front_station_80));
	  		EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Y_REAR_STATION_80", &is_cylinder_y_rear_station_80));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_80", &is_cylinder_z_up_station_80));
	  		EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_80", &is_cylinder_z_down_station_80));
			if( is_cylinder_z_up_station_80   && !is_cylinder_z_down_station_80	 &&
				is_cylinder_y_rear_station_80 && !is_cylinder_y_front_station_80 &&
			    status_cylinder_rotation_up_station_80 && !status_cylinder_rotation_down_station_80 &&
			    status_cylinder_system_lev_up_station_80 && !status_cylinder_system_lev_down_station_80)
				{
				is_station_80_ready = TRUE;
				}
			else 
				is_station_80_ready = FALSE;
			

					
			Table_Ready[0] = is_station_10_ready;
			Table_Ready[1] = is_station_20_ready;
			Table_Ready[2] = is_station_30_ready;
			Table_Ready[3] = is_station_40_ready;
			Table_Ready[4] = is_station_50_ready;
			Table_Ready[5] = is_station_60_ready;
			Table_Ready[6] = is_station_70_ready;
			Table_Ready[7] = is_station_80_ready;
			Table_Ready[8] = 0X00;
		
Error:
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"Tester::OkNokCheck"
static SElExceptionPtr OkNokCheck(struct _STester* me, int32_t timeout, bool_t* ok)
{
	SElExceptionPtr	pexception = NULL;
	uint32_t		button_ok,
					button_nok;

	int red = 0;
	int green = 1;
	
	TIMEOUT_INIT();
	
	if(ok) *ok = FALSE;


	
	while(TRUE)
	{
		EXCCHECK( me->GetSensor(me, "BUTTON_OK", &button_ok));
		EXCCHECK( me->GetSensor(me, "BUTTON_NOK", &button_nok));
	
		if (red == 0)
		{
			EXCCHECK( me->SetSensor(me, "INDICATOR_NOK", 1));
			red = 1;
		}
		else  if (red == 1)
		{
			EXCCHECK( me->SetSensor(me, "INDICATOR_NOK", 0));
			red = 0;
		}
		Sleep(120);
		EXCCHECK( me->GetSensor(me, "BUTTON_OK", &button_ok));
		EXCCHECK( me->GetSensor(me, "BUTTON_NOK", &button_nok));
		
		if (green == 0)
		{
			EXCCHECK( me->SetSensor(me, "INDICATOR_OK", 1));
			green = 1;
		}
		else  if (green == 1)
		{
			EXCCHECK( me->SetSensor(me, "INDICATOR_OK", 0));
			green = 0;
		}
		Sleep(10);
		EXCCHECK( me->GetSensor(me, "BUTTON_OK", &button_ok));
		EXCCHECK( me->GetSensor(me, "BUTTON_NOK", &button_nok));
		
		if(button_ok && !button_nok)
		{
			if(ok) *ok = TRUE;
			break;
		}
		else if(!button_ok && button_nok)
		{
			if(ok) *ok = FALSE;
			break;
		}
		
		TIMEOUT_CHECK( timeout);
	};

Error:
	EXCIGNORE( TESTER_ERROR_TIMEOUT);
	me->SetSensor(me, "INDICATOR_OK", 0);
	me->SetSensor(me, "INDICATOR_NOK", 0);
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"Tester::NokCheck"
static SElExceptionPtr NokCheck(struct _STester* me, int32_t timeout)
{
	SElExceptionPtr	pexception = NULL;
	uint32_t		button_nok;

	TIMEOUT_INIT();

	EXCCHECK( me->SetSensor(me, "INDICATOR_NOK", 1));

	while(TRUE)
	{
		EXCCHECK( me->GetSensor(me, "BUTTON_NOK", &button_nok));
		
		if(button_nok)
			break;

		if(timeout>=0)
			TIMEOUT_CHECK( timeout);

		Sleep(10);
	};

Error:
	EXCIGNORE( TESTER_ERROR_TIMEOUT);
	me->SetSensor(me, "INDICATOR_NOK", 0);
	EXCRETHROW(pexception);
}

typedef struct _SBeepThreadData{
	struct _STester*	me;
	int32_t				time;
} SBeepThreadData, *SBeepThreadDataPtr;

/*----------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "BeepThreadFunc"
#if defined(_CVI_)
	static DWORD WINAPI BeepThreadFunc (LPVOID param)
#else
	unsigned __cdecl BeepThreadFunc( void* pArguments )
#endif
{
	SElExceptionPtr		pexception = NULL;
	SBeepThreadDataPtr	pthread_data = (SBeepThreadDataPtr)param;
	
	EXCCHECK( pthread_data->me->SetSensor(pthread_data->me, "TESTER_BEEP", 1)); 

	timeBeginPeriod(1);
	Sleep(pthread_data->time); 
	timeEndPeriod(1);

	EXCCHECK( pthread_data->me->SetSensor(pthread_data->me, "TESTER_BEEP", 0));
	
Error:
	free( pthread_data);
	EXCDBGSTR( pexception);
#if defined(_CVI_)
	CmtExitThreadPoolThread (0);
#else
    _endthread( 0 );
#endif
    return 0;
}	/* BeepThreadFunc */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"Tester::SetSensor"
static SElExceptionPtr SetSensor
(
	struct _STester* me,
	const char*		sensorId,
	uint32_t		value
)
{
	SElExceptionPtr	pexception = NULL;
	char*			plogic_name = "";
	SLogicName		tmp;
	SLogicNamePtr	palias_pair = NULL;
	char*			plogical_name = NULL;
	
	/* convert sensorId to logical name in tester */
	strcpy( tmp.aliasName, sensorId);
	
	palias_pair = bsearch( &tmp,
						   me->pLogicalNames,
						   me->LogicalCounts,
						   sizeof(SLogicName),
						   CompareFnc);

	plogical_name = (palias_pair) ? palias_pair->logicalName : (char*) sensorId;

	if( me->sensorAPI)
		EXCCHECK( me->sensorAPI->SetState(me->sensorAPI, plogical_name, value));

Error:
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"Tester::SetSensorCallback"
static SElExceptionPtr SetSensorCallback(
	struct _STester* me, 
	const char* sensorId, 
	SensorCallbackFnc_t fCall,
	void* pData
)
{
	SElExceptionPtr	pexception = NULL;
	char*			plogic_name = "";
	SLogicName		tmp;
	SLogicNamePtr	palias_pair = NULL;
	char*			plogical_name = NULL;
	
	/* convert sensorId to logical name in tester */
	strcpy( tmp.aliasName, sensorId);
	
	palias_pair = bsearch( &tmp,
						   me->pLogicalNames,
						   me->LogicalCounts,
						   sizeof(SLogicName),
						   CompareFnc);

	plogical_name = (palias_pair) ? palias_pair->logicalName : (char*) sensorId;

	if( me->sensorAPI)
		EXCCHECK( me->sensorAPI->RegisterCallback( me->sensorAPI, plogical_name, fCall, pData));

Error:
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"Tester::GetSensor"
static SElExceptionPtr GetSensor
(
	struct _STester*	me,
	const char*		sensorId,
	uint32_t*		value
)
{
	SElExceptionPtr	pexception = NULL;
	SLogicName		tmp;
	SLogicNamePtr	palias_pair = NULL;
	char*			plogical_name = NULL;
	
	/* TODO: convert sensorId to logical name in tester */
	strcpy( tmp.aliasName, sensorId);
	
	palias_pair = bsearch( &tmp,
						   me->pLogicalNames,
						   me->LogicalCounts,
						   sizeof(SLogicName),
						   CompareFnc);

	if (value == NULL)
	{
		EXCTHROW(TESTER_ERROR_INVALID_PARAMETER, "Parameter value is NULL");
	}

	plogical_name = (palias_pair)? palias_pair->logicalName : (char*) sensorId;
	
	if( me->sensorAPI)
		EXCCHECK( me->sensorAPI->GetState(me->sensorAPI, plogical_name, value));

Error:
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"CallbackFncButtonOk"
static SElExceptionPtr CallbackFncButtonOk(const char* name, uint32_t state, void* pData)
{
	SElExceptionPtr	pexception = NULL;
	STesterPtr		me = (STesterPtr)pData;

	if(state==1 && me->eventAPI)
		EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_TESTER_BUTTON_PRESSED_OK, NULL));

	if(state==1)
	{
		HWND hwnd_excp = NULL;
		HWND hwnd_exit = NULL;
	
		/* confirm excption */
		hwnd_excp = FindWindow(NULL, "Exception");
		if(hwnd_excp)
		{
			SendMessage(hwnd_excp, WM_COMMAND, IDYES, BN_CLICKED);
		}
		else
		{
			/* confirm exit */
			hwnd_exit = FindWindow(NULL, "Exit");
			if(hwnd_exit)
				SendMessage(hwnd_exit, WM_COMMAND, IDYES, BN_CLICKED);
		}
	}

Error:
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"CallbackFncButtonNok"
static SElExceptionPtr CallbackFncButtonNok(const char* name, uint32_t state, void* pData)
{
	SElExceptionPtr	pexception = NULL;
	STesterPtr		me = (STesterPtr)pData;
	int 						status, status_barrier;
	clock_t 					_start_test_clock;
	double   					time;
	static double   			time_push = 1500;

	if( state==1)
	{
		_start_test_clock =  clock();
		do {
			EXCCHECK( me->GetSensor(me, "BUTTON_NOK", &status));
	    	time = (double)(clock()-_start_test_clock);
			}while (status == 1 && time < 300);	
		
		EXCCHECK( me->GetSensor(me, "BUTTON_NOK", &status));

	if (status == 0 && time <= 300) //pulse
		{
		if( me->eventAPI )
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_TESTER_BUTTON_PRESSED_NOK, NULL));
	
		HWND hwnd_exit = NULL;
		/* storno exit */
		hwnd_exit = FindWindow(NULL, "Exit");
		if(hwnd_exit)
			SendMessage(hwnd_exit, WM_COMMAND, IDNO, BN_CLICKED);
	
		}

	else if (status == 1) //appui long
		{
			_start_test_clock =  clock();
		do {
			EXCCHECK( me->GetSensor(me, "INFRARED_BARRIER", &status_barrier));
			EXCCHECK( me->GetSensor(me, "BUTTON_NOK", &status));
		      if (status_barrier)
				EXCCHECK( me->SetSensor(me, "INDICATOR_NOK", 1));
			  else 
				EXCCHECK( me->SetSensor(me, "INDICATOR_NOK", 0));
 
	    	time = (double)(clock()-_start_test_clock);
			
			}while (status == 1 && time < time_push);
	
		EXCCHECK( me->SetSensor(me, "INDICATOR_NOK", 0));
		
		 if (status == 1 && time >= time_push && status_barrier) //Reset Station 1
			{
			EXCCHECK( me->SetSensor(me, "INDICATOR_NOK", 1));
		   	EXCCHECK( me->PlugPanelStation10(me, 5000));
			EXCCHECK( me->UnlockPanelStation10(me, 5000));
			EXCCHECK( me->SetSensor(me, "INDICATOR_NOK", 0));
			}
		}

	}
Error:
	EXCCHECK( me->SetSensor(me, "INDICATOR_NOK", 0));
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"CallbackFncButtonMenu"
static SElExceptionPtr CallbackFncButtonMenu(const char* name, uint32_t state, void* pData)
{
	SElExceptionPtr	pexception = NULL;
	STesterPtr		me = (STesterPtr)pData;
	int 						status;
	clock_t 					_start_test_clock;
	double   					time;
	static double   			time_push = 1500;
	
	
#if 0    /* formerly excluded lines */
	if( state==1)
	{
		_start_test_clock =  clock();
		do {
			EXCCHECK( me->GetSensor(me, "BUTTON_MENU", &status));
	    	time = (double)(clock()-_start_test_clock);
			}while (status == 1 && time < 300);	
		
		EXCCHECK( me->GetSensor(me, "BUTTON_MENU", &status));

	if (status == 0 && time <= 300) //pulse
		{
		if( state==1 && me->eventAPI)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_TESTER_BUTTON_PRESSED_MENU, NULL));
		}

	else if (status == 1) //appui long
		{
			_start_test_clock =  clock();
		do {
			EXCCHECK( me->GetSensor(me, "BUTTON_MENU", &status));
	    	time = (double)(clock()-_start_test_clock);
			}while (status == 1 && time < time_push);
	
		 if (status == 1 && time >= time_push ) //Reset Station 1
			{
			if (gs_Light_Tester == 0)
			{
				EXCCHECK( me->SetSensor(me, "LIGHT_TESTER", 1));
				gs_Light_Tester = 1;
			}
			else if (gs_Light_Tester == 1)
			{					   
				EXCCHECK( me->SetSensor(me, "LIGHT_TESTER", 0));
				gs_Light_Tester = 0;
			}
			}
		}

	}
#endif   /* formerly excluded lines */
	
	if( state==1 && me->eventAPI)
		EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_TESTER_BUTTON_PRESSED_MENU, NULL));
	
	
Error:
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"CallbackFncButtonLabel"
static SElExceptionPtr CallbackFncButtonLabel(const char* name, uint32_t state, void* pData)
{
	SElExceptionPtr	pexception = NULL;
	STesterPtr		me = (STesterPtr)pData;

	if( state==1 && me->eventAPI)
		EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_TESTER_BUTTON_PRESSED_LABEL, NULL));

Error:
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"CallbackFncButtonDowntime"
static SElExceptionPtr CallbackFncButtonDowntime(const char* name, uint32_t state, void* pData)
{
	SElExceptionPtr	pexception = NULL;
	STesterPtr		me = (STesterPtr)pData;

	if( state==1 && me->eventAPI)
		EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_TESTER_BUTTON_PRESSED_DOWNTIME, NULL));

Error:
	EXCRETHROW(pexception);
}

#undef __FUNC__
#define __FUNC__	"CallbackFncButtonStart"
static SElExceptionPtr CallbackFncButtonStart(const char* name, uint32_t state, void* pData)
{
	SElExceptionPtr	pexception = NULL;
	STesterPtr		me = (STesterPtr)pData;
	bool_t          status_micro_cam, status_barrier;
	bool_t          sensor_plug_station_10, sensor_unplug_station_10;
	bool_t          insert;
	
	
	/* check where is the table */
	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &status_micro_cam));
	EXCCHECK( me->GetSensor(me, "INFRARED_BARRIER", &status_barrier));

	if (status_micro_cam == 0)
		me->TableRotate(me, 6000,TRUE);
	
		EXCCHECK( me->GetSensor(me, "INFRARED_BARRIER", &status_barrier));

	if(status_barrier == 1 && status_micro_cam == 1)
		{
		EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_CYCLE_START, NULL));
		}
	
Error:
	EXCRETHROW(pexception);
}


#undef __FUNC__
#define __FUNC__	"CallbackFncRedBoxEmpty"
static SElExceptionPtr CallbackFncRedBoxEmpty(const char* name, uint32_t state, void* pData)
{
	SElExceptionPtr	pexception = NULL;
	STesterPtr		me = (STesterPtr)pData;
	
	if( state==0 && me->eventAPI)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_RED_BOX_EMPTY, NULL));
	
Error:
	EXCRETHROW(pexception);
}
	
#undef __FUNC__
#define __FUNC__	"CallbackFncRotation"
static SElExceptionPtr CallbackFncRotation(const char* name, uint32_t state, void* pData)
{
	SElExceptionPtr	pexception = NULL;
	STesterPtr		me = (STesterPtr)pData;
	bool_t          status_micro_cam;

	if( state==1 && me->eventAPI)  // state==1 : TABLE IN POSITION
	{
		EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_TESTER_ROTATION_FINISHED, NULL));
	}
	
Error:
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"CallbackFncButtonTotalStop"
static SElExceptionPtr CallbackFncButtonTotalStop(const char* name, uint32_t state, void* pData)
{
	SElExceptionPtr	pexception = NULL;
	STesterPtr		me = (STesterPtr)pData;

	//EXCCHECK( me->SetSensor(me, "INDICATOR_START", state));	

Error:
	EXCRETHROW(pexception);
}

#undef __FUNC__
#define __FUNC__	"JigThread"
SElExceptionPtr JigThread(struct _STester* me)
{
	SElExceptionPtr	pexception = NULL;
	bool_t			microCam = 0;
	int32_t			insert = 0,
					gs_plug_station_10,
					gs_unplug_station_10,
					gs_lock_left_station_10,
					gs_unlock_left_station_10,
					gs_lock_right_station_10,
					gs_unlock_right_station_10,
					sensor_locked_station_10,
					 
					gs_plug_station_30,
					gs_unplug_station_30,
					gs_lock_left_station_30,
					gs_unlock_left_station_30,
					gs_lock_right_station_30,
					gs_unlock_right_station_30,
					gs_cylinder_airbag_up_station_30 = 0,
					gs_cylinder_airbag_down_station_30 = 0,
					gs_cylinder_centrage_front_station_30,
					gs_cylinder_centrage_rear_station_30,
					gs_cylinder_gripper_centrage_block_station_30,
					gs_cylinder_gripper_centrage_unblock_station_30,
					
					gs_plug_station_60,
					gs_unplug_station_60,
					gs_cylinder_rotation_up_station_60 = 0,
					gs_cylinder_rotation_down_station_60 = 0,
					gs_lock_left_station_60,
					gs_unlock_left_station_60,
					gs_lock_right_station_60,
					gs_unlock_right_station_60,
					gs_cylinder_centrage_front_station_60,
					gs_cylinder_centrage_rear_station_60,
					gs_cylinder_gripper_centrage_block_station_60,
					gs_cylinder_gripper_centrage_unblock_station_60,
					
					gs_plug_station_70,
					gs_unplug_station_70,
					gs_lock_left_station_70,
					gs_unlock_left_station_70,
					gs_lock_right_station_70,
					gs_unlock_right_station_70,
					gs_plug_station_80,
					gs_unplug_station_80,
					gs_lock_left_station_80,
					gs_unlock_left_station_80,
					gs_lock_right_station_80,
					gs_unlock_right_station_80,
					gs_cylinder_rotation_up_station_80 = 0,
					gs_cylinder_rotation_down_station_80 = 0,
					gs_cylinder_system_lev_up_station_80 = 0,
					gs_cylinder_system_lev_down_station_80 = 0,
					gs_barcode_front,
					gs_barcode_rear;
	
	
	/* Check if pannel is inserted STATION 10*/ 
	EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_STATION_10", &gs_plug_station_10));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_STATION_10", &gs_unplug_station_10));
	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	if (gs_plug_station_10 && !gs_unplug_station_10 && microCam)
	{
		EXCCHECK( me->GetSensor(me, "SENSOR_PRESENCE_PRODUCT_STATION_10", &insert));
		if (insert)
		{
			if (me->eventAPI && !gs_inserted )
			{
				Sleep(200); //confirm product inserted
				EXCCHECK( me->GetSensor(me, "SENSOR_PRESENCE_PRODUCT_STATION_10", &insert));
				if (insert)
				{
					EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_INSERTED, NULL));
					gs_inserted = 1;
					gs_released = 0;
				}
			}
		}
	
		else if (!insert)
		{

			if (me->eventAPI && !gs_released)
			{
				Sleep(200); //confirm product removed
				EXCCHECK( me->GetSensor(me, "SENSOR_PRESENCE_PRODUCT_STATION_10", &insert));
				if (!insert)
				{
					EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_RELEASED, NULL));
					gs_released = 1;
					gs_inserted = 0;
				}
			}
		}	
			
	}	
		
	/* Check if panel plugged STATION 10 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_STATION_10", &gs_plug_station_10));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_STATION_10", &gs_unplug_station_10));
	if (!gs_unplug_station_10 && gs_plug_station_10)
	{
		if (me->eventAPI && !gs_plugged_station_10)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_PLUGGED_STATION_10, NULL));
		
		gs_plugged_station_10 = 1;
	}
	else
	{
		gs_plugged_station_10 = 0;
	}
	
	/* Check if panel unplugged STATION 10 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_STATION_10", &gs_plug_station_10));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_STATION_10", &gs_unplug_station_10));
	if (gs_unplug_station_10 && !gs_plug_station_10)
	{
		if (me->eventAPI && !gs_unplugged_station_10)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_UNPLUGGED_STATION_10, NULL));
		
		gs_unplugged_station_10 = 1;
	}
	else
	{
		gs_unplugged_station_10 = 0;
	}
	
	/* Check if panel locked STATION 10 */ 
	EXCCHECK( me->GetSensor(me, "IS_PANEL_LOCKED_LEFT_STATION_10", &gs_lock_left_station_10));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_LOCKED_RIGHT_STATION_10", &gs_lock_right_station_10));

	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNLOCKED_LEFT_STATION_10", &gs_unlock_left_station_10));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNLOCKED_RIGHT_STATION_10", &gs_unlock_right_station_10));

	
	if ((!gs_unlock_left_station_10 && gs_lock_left_station_10) && (!gs_unlock_right_station_10 && gs_lock_right_station_10))
	{
	if (me->eventAPI && !gs_locked_station_10)
		EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_LOCKED_STATION_10, NULL));
		gs_locked_station_10 = 1;
	}
	else
	{
		gs_locked_station_10 = 0;
	}

	
	/* Check if panel unlocked station 10 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_LOCKED_LEFT_STATION_10", &gs_lock_left_station_10));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_LOCKED_RIGHT_STATION_10", &gs_lock_right_station_10));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNLOCKED_LEFT_STATION_10", &gs_unlock_left_station_10));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNLOCKED_RIGHT_STATION_10", &gs_unlock_right_station_10));

	
	//Check Left Unlocking Cylinder
	if (gs_unlock_left_station_10 && !gs_lock_left_station_10 & gs_unlock_right_station_10 && !gs_lock_right_station_10 )  
	{
		if (me->eventAPI && !gs_unlocked_station_10)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_UNLOCKED_STATION_10, NULL));
		
		gs_unlocked_station_10 = 1;
	
	}
	else													  
	{
		gs_unlocked_station_10 = 0;
	}
	
	 	/* Check if panel plugged STATION 30 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_STATION_30", &gs_plug_station_30));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_STATION_30", &gs_unplug_station_30));
	if (!gs_unplug_station_30 && gs_plug_station_30)
	{
		if (me->eventAPI && !gs_plugged_station_30)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_PLUGGED_STATION_30, NULL));
		
		gs_plugged_station_30 = 1;
	}
	else
	{
		gs_plugged_station_30 = 0;
	}
	
	/* Check if panel unplugged STATION 30 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_STATION_30", &gs_plug_station_30));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_STATION_30", &gs_unplug_station_30));
	if (gs_unplug_station_30 && !gs_plug_station_30)
	{
		if (me->eventAPI && !gs_unplugged_station_30)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_UNPLUGGED_STATION_30, NULL));
		
		gs_unplugged_station_30 = 1;
	}
	else
	{
		gs_unplugged_station_30 = 0;
	}
	
	/* Check if panel locked station 30 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_LOCKED_LEFT_STATION_30", &gs_lock_left_station_30));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_LOCKED_RIGHT_STATION_30", &gs_lock_right_station_30));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNLOCKED_LEFT_STATION_30", &gs_unlock_left_station_30));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNLOCKED_RIGHT_STATION_30", &gs_unlock_right_station_30));

	if (!gs_unlock_left_station_30 && gs_lock_left_station_30 &&!gs_unlock_right_station_30 && gs_lock_right_station_30) 
	{
	if (me->eventAPI && !gs_locked_station_30)
		EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_LOCKED_STATION_30, NULL));
		gs_locked_station_30 = 1;
	}
	else
	{
		gs_locked_station_30 = 0;
	}
	
	/* Check if panel unlocked station 30 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_LOCKED_LEFT_STATION_30", &gs_lock_left_station_30));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_LOCKED_RIGHT_STATION_30", &gs_lock_right_station_30));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNLOCKED_LEFT_STATION_30", &gs_unlock_left_station_30));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNLOCKED_RIGHT_STATION_30", &gs_unlock_right_station_30));

	if (gs_unlock_left_station_30 && !gs_lock_left_station_30 && gs_unlock_right_station_30 && !gs_lock_right_station_30) 
	{
		if (me->eventAPI && !gs_unlocked_station_30)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_UNLOCKED_STATION_30, NULL));
		
		gs_unlocked_station_30 = 1;
	
	}
	else													  
	{
		gs_unlocked_station_30 = 0;
	}
	
	/* Check if Cylinder_Centrage is Front Station 30 */
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_CENTRAGE_FRONT_STATION_30", &gs_cylinder_centrage_front_station_30));
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_CENTRAGE_REAR_STATION_30", &gs_cylinder_centrage_rear_station_30));
	if (gs_cylinder_centrage_front_station_30 && !gs_cylinder_centrage_rear_station_30)
	{
	if (me->eventAPI && !gs_cylinder_centrage_is_front_30)
		EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_CYLINDER_CENTRAGE_FRONT_STATION_30, NULL));
		gs_cylinder_centrage_is_front_30 = 1;
	}
	else
	{
		gs_cylinder_centrage_is_front_30 = 0;
	}
	
	/* Check if Cylinder_Centrage is REAR Station 30 */
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_CENTRAGE_FRONT_STATION_30", &gs_cylinder_centrage_front_station_30));
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_CENTRAGE_REAR_STATION_30", &gs_cylinder_centrage_rear_station_30));
	if (!gs_cylinder_centrage_front_station_30 && gs_cylinder_centrage_rear_station_30)
	{
	if (me->eventAPI && !gs_cylinder_centrage_is_rear_30)
		EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_CYLINDER_CENTRAGE_REAR_STATION_30, NULL));
		gs_cylinder_centrage_is_rear_30 = 1;
	}
	else
	{
		gs_cylinder_centrage_is_rear_30 = 0;
	}
	
	/* Check if Cylinder_GripperCentrage is blocked Station 30 */
	EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CENTRAGE_BLOCK_STATION_30", &gs_cylinder_gripper_centrage_block_station_30));
	EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CENTRAGE_UNBLOCK_STATION_30", &gs_cylinder_gripper_centrage_unblock_station_30));
	if (gs_cylinder_gripper_centrage_block_station_30 && !gs_cylinder_gripper_centrage_unblock_station_30)
	{
	if (me->eventAPI && !gs_cylinder_gripper_centrage_is_blocked_30)
		EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_CYLINDER_GRIPPER_CENTRAGE_BLOCK_STATION_30, NULL));
		gs_cylinder_gripper_centrage_is_blocked_30 = 1;
	}
	else
	{
		gs_cylinder_gripper_centrage_is_blocked_30 = 0;
	}
	
	
	/* Check if Cylinder_GripperCentrage is unblocked Station 30 */
	EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CENTRAGE_BLOCK_STATION_30", &gs_cylinder_gripper_centrage_block_station_30));
	EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CENTRAGE_UNBLOCK_STATION_30", &gs_cylinder_gripper_centrage_unblock_station_30));
	if (!gs_cylinder_gripper_centrage_block_station_30 && gs_cylinder_gripper_centrage_unblock_station_30)
	{
	if (me->eventAPI && !gs_cylinder_gripper_centrage_is_unblocked_30)
		EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_CYLINDER_GRIPPER_CENTRAGE_UNBLOCK_STATION_30, NULL));
		gs_cylinder_gripper_centrage_is_unblocked_30 = 1;
	}
	else
	{
		gs_cylinder_gripper_centrage_is_unblocked_30 = 0;
	}

	/* Check if Cylinder_Rotation is Up 30 */
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_AIRBAG_UP_STATION_30", &gs_cylinder_airbag_up_station_30));
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_AIRBAG_DOWN_STATION_30", &gs_cylinder_airbag_down_station_30));
	if (gs_cylinder_airbag_up_station_30 && !gs_cylinder_airbag_down_station_30)
	{
	if (me->eventAPI && !gs_cylinder_airbag_is_up_30)
		EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_CYLINDER_AIRBAG_UP_STATION_30, NULL));
		gs_cylinder_airbag_is_up_30 = 1;
	}
	else
	{
		gs_cylinder_airbag_is_up_30 = 0;
	}
	
	/* Check if Cylinder_Rotation is Down 30 */
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_AIRBAG_UP_STATION_30", &gs_cylinder_airbag_up_station_30));
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_AIRBAG_DOWN_STATION_30", &gs_cylinder_airbag_down_station_30));
	if (gs_cylinder_airbag_down_station_30 && !gs_cylinder_airbag_up_station_30)
	{
	if (me->eventAPI && !gs_cylinder_airbag_is_down_30)
		EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_CYLINDER_AIRBAG_DOWN_STATION_30, NULL));
		gs_cylinder_airbag_is_down_30 = 1;
	}
	else
	{
		gs_cylinder_airbag_is_down_30 = 0;
	}


   	/* Check if panel plugged STATION 60 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_STATION_60", &gs_plug_station_60));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_STATION_60", &gs_unplug_station_60));
	if (!gs_unplug_station_60 && gs_plug_station_60)
	{
		if (me->eventAPI && !gs_plugged_station_60)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_PLUGGED_STATION_60, NULL));
		
		gs_plugged_station_60 = 1;
	}
	else
	{
		gs_plugged_station_60 = 0;
	}
	/* Check if panel unplugged STATION 60 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_STATION_60", &gs_plug_station_60));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_STATION_60", &gs_unplug_station_60));
	if (gs_unplug_station_60 && !gs_plug_station_60)
	{
		if (me->eventAPI && !gs_unplugged_station_60)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_UNPLUGGED_STATION_60, NULL));
		
		gs_unplugged_station_60 = 1;
	}
	else
	{
		gs_unplugged_station_60 = 0;
	}
	
		/* Check if Cylinder_Centrage is Front Station 60 */
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_CENTRAGE_FRONT_STATION_60", &gs_cylinder_centrage_front_station_60));
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_CENTRAGE_REAR_STATION_60", &gs_cylinder_centrage_rear_station_60));
	if (gs_cylinder_centrage_front_station_60 && !gs_cylinder_centrage_rear_station_60)
	{
	if (me->eventAPI && !gs_cylinder_centrage_is_front_60)
		EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_CYLINDER_CENTRAGE_FRONT_STATION_60, NULL));
		gs_cylinder_centrage_is_front_60 = 1;
	}
	else
	{
		gs_cylinder_centrage_is_front_60 = 0;
	}
	
			/* Check if Cylinder_Centrage is REAR Station 60 */
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_CENTRAGE_FRONT_STATION_60", &gs_cylinder_centrage_front_station_60));
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_CENTRAGE_REAR_STATION_60", &gs_cylinder_centrage_rear_station_60));
	if (!gs_cylinder_centrage_front_station_60 && gs_cylinder_centrage_rear_station_60)
	{
	if (me->eventAPI && !gs_cylinder_centrage_is_rear_60)
		EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_CYLINDER_CENTRAGE_REAR_STATION_60, NULL));
		gs_cylinder_centrage_is_rear_60 = 1;
	}
	else
	{
		gs_cylinder_centrage_is_rear_60 = 0;
	}
	
			/* Check if Cylinder_GripperCentrage is blocked Station 60 */
	EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CENTRAGE_BLOCK_STATION_60", &gs_cylinder_gripper_centrage_block_station_60));
	EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CENTRAGE_UNBLOCK_STATION_60", &gs_cylinder_gripper_centrage_unblock_station_60));
	if (gs_cylinder_gripper_centrage_block_station_60 && !gs_cylinder_gripper_centrage_unblock_station_60)
	{
	if (me->eventAPI && !gs_cylinder_gripper_centrage_is_blocked_60)
		EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_CYLINDER_GRIPPER_CENTRAGE_BLOCK_STATION_60, NULL));
		gs_cylinder_gripper_centrage_is_blocked_60 = 1;
	}
	else
	{
		gs_cylinder_gripper_centrage_is_blocked_60 = 0;
	}
	
	
	/* Check if Cylinder_GripperCentrage is unblocked Station 60 */
	EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CENTRAGE_BLOCK_STATION_60", &gs_cylinder_gripper_centrage_block_station_60));
	EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CENTRAGE_UNBLOCK_STATION_60", &gs_cylinder_gripper_centrage_unblock_station_60));
	if (!gs_cylinder_gripper_centrage_block_station_60 && gs_cylinder_gripper_centrage_unblock_station_60)
	{
	if (me->eventAPI && !gs_cylinder_gripper_centrage_is_unblocked_60)
		EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_CYLINDER_GRIPPER_CENTRAGE_UNBLOCK_STATION_60, NULL));
		gs_cylinder_gripper_centrage_is_unblocked_60 = 1;
	}
	else
	{
		gs_cylinder_gripper_centrage_is_unblocked_60 = 0;
	}
	
	
	/* Check if Cylinder_Rotation is Up 60 */
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_ROTATION_UP_STATION_60", &gs_cylinder_rotation_up_station_60));
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_ROTATION_DOWN_STATION_60", &gs_cylinder_rotation_down_station_60));
	if (gs_cylinder_rotation_up_station_60 && !gs_cylinder_rotation_down_station_60)
	{
	if (me->eventAPI && !gs_cylinder_rotation_is_up_60)
		EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_CYLYNDER_ROTATION_UP_STATION_60, NULL));
		gs_cylinder_rotation_is_up_60 = 1;
	}
	else
	{
		gs_cylinder_rotation_is_up_60 = 0;
	}
		/* Check if Cylinder_Rotation isDown 60 */
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_ROTATION_UP_STATION_60", &gs_cylinder_rotation_up_station_60));
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_ROTATION_DOWN_STATION_60", &gs_cylinder_rotation_down_station_60));
	if (gs_cylinder_rotation_down_station_60 && !gs_cylinder_rotation_up_station_60)
	{
	if (me->eventAPI && !gs_cylinder_rotation_is_down_60)
		EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_CYLYNDER_ROTATION_DOWN_STATION_60, NULL));
		gs_cylinder_rotation_is_down_60 = 1;
	}
	else
	{
		gs_cylinder_rotation_is_down_60 = 0;
	}
	

	
/* Check if panel locked station 60 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_LOCKED_LEFT_STATION_60", &gs_lock_left_station_60));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_LOCKED_RIGHT_STATION_60", &gs_lock_right_station_60));

	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNLOCKED_LEFT_STATION_60", &gs_unlock_left_station_60));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNLOCKED_RIGHT_STATION_60", &gs_unlock_right_station_60));

	
	if (!gs_unlock_left_station_60 && gs_lock_left_station_60 &&!gs_unlock_right_station_60 && gs_lock_right_station_60) 
	{
	if (me->eventAPI && !gs_locked_station_60)
		EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_LOCKED_STATION_60, NULL));
		gs_locked_station_60 = 1;
	}
	else
	{
		gs_locked_station_60 = 0;
	}
	
	
	/* Check if panel unlocked station 60 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_LOCKED_LEFT_STATION_60", &gs_lock_left_station_60));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_LOCKED_RIGHT_STATION_60", &gs_lock_right_station_60));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNLOCKED_LEFT_STATION_60", &gs_unlock_left_station_60));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNLOCKED_RIGHT_STATION_60", &gs_unlock_right_station_60));

	
	if (gs_unlock_left_station_60 && !gs_lock_left_station_60 && gs_unlock_right_station_60 && !gs_lock_right_station_60) 
	{
		if (me->eventAPI && !gs_unlocked_station_60)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_UNLOCKED_STATION_60, NULL));
		
		gs_unlocked_station_60 = 1;
	
	}
	else													  
	{
		gs_unlocked_station_60 = 0;
	}
	
	/* Check if panel plugged station 70 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_STATION_70", &gs_plug_station_70));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_STATION_70", &gs_unplug_station_70));
	if (!gs_unplug_station_70 && gs_plug_station_70)
	{
		if (me->eventAPI && !gs_plugged_station_70)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_PLUGGED_STATION_70, NULL));
		
		gs_plugged_station_70 = 1;
	}
	else
	{
		gs_plugged_station_70 = 0;
	}
	/* Check if panel unplugged station 70 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_STATION_70", &gs_plug_station_70));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_STATION_70", &gs_unplug_station_70));
	if (gs_unplug_station_70 && !gs_plug_station_70)
	{
		if (me->eventAPI && !gs_unplugged_station_70)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_UNPLUGGED_STATION_70, NULL));
		
		gs_unplugged_station_70 = 1;
	}
	else
	{
		gs_unplugged_station_70 = 0;
	}
	
	
	/* Check if panel locked station 70 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_LOCKED_LEFT_STATION_70", &gs_lock_left_station_70));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_LOCKED_RIGHT_STATION_70", &gs_lock_right_station_70));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNLOCKED_LEFT_STATION_70", &gs_unlock_left_station_70));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNLOCKED_RIGHT_STATION_70", &gs_unlock_right_station_70));

	if (!gs_unlock_left_station_70 && gs_lock_left_station_70 && !gs_unlock_right_station_70 && gs_lock_right_station_70)
	{
	if (me->eventAPI && !gs_locked_station_70)
		EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_LOCKED_STATION_70, NULL));
		gs_locked_station_70 = 1;
	}
	else
	{
		gs_locked_station_70 = 0;
	}
	

	/* Check if panel unlocked station 70 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_LOCKED_LEFT_STATION_70", &gs_lock_left_station_70));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_LOCKED_RIGHT_STATION_70", &gs_lock_right_station_70));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNLOCKED_LEFT_STATION_70", &gs_unlock_left_station_70));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNLOCKED_RIGHT_STATION_70", &gs_unlock_right_station_70));

	
	if (gs_unlock_left_station_70 && !gs_lock_left_station_70 && gs_unlock_right_station_70 && !gs_lock_right_station_70)
	{
		if (me->eventAPI && !gs_unlocked_station_70)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_UNLOCKED_STATION_70, NULL));
		
		gs_unlocked_station_70 = 1;
	
	}
	else													  
	{
		gs_unlocked_station_70 = 0;
	}
		
	 /* Check if panel plugged STATION 80 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_STATION_80", &gs_plug_station_80));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_STATION_80", &gs_unplug_station_80));
	if (!gs_unplug_station_80 && gs_plug_station_80)
	{
		if (me->eventAPI && !gs_plugged_station_80)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_PLUGGED_STATION_80, NULL));
		
		gs_plugged_station_80 = 1;
	}
	else
	{
		gs_plugged_station_80 = 0;
	}
	/* Check if panel unplugged STATION 80 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_STATION_80", &gs_plug_station_80));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_STATION_80", &gs_unplug_station_80));
	if (gs_unplug_station_80 && !gs_plug_station_80)
	{
		if (me->eventAPI && !gs_unplugged_station_80)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_UNPLUGGED_STATION_80, NULL));
		
		gs_unplugged_station_80 = 1;
	}
	else
	{
		gs_unplugged_station_80 = 0;
	}
	
	
	/* Check if panel locked station 80 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_LOCKED_LEFT_STATION_80", &gs_lock_left_station_80));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_LOCKED_RIGHT_STATION_80", &gs_lock_right_station_80));

	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNLOCKED_LEFT_STATION_80", &gs_unlock_left_station_80));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNLOCKED_RIGHT_STATION_80", &gs_unlock_right_station_80));

	
	if (!gs_unlock_left_station_80 && gs_lock_left_station_80 && !gs_unlock_right_station_80 && gs_lock_right_station_80)
	{
	if (me->eventAPI && !gs_locked_station_80)
		EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_LOCKED_STATION_80, NULL));
		gs_locked_station_80 = 1;
	}
	else
	{
		gs_locked_station_80 = 0;
	}
	/* Check if panel unlocked station 80 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_LOCKED_LEFT_STATION_80", &gs_lock_left_station_80));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_LOCKED_RIGHT_STATION_80", &gs_lock_right_station_80));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNLOCKED_LEFT_STATION_80", &gs_unlock_left_station_80));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNLOCKED_RIGHT_STATION_80", &gs_unlock_right_station_80));
	if (gs_unlock_left_station_80 && !gs_lock_left_station_80 && gs_unlock_right_station_80 && !gs_lock_right_station_80)
	{
		if (me->eventAPI && !gs_unlocked_station_80)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_UNLOCKED_STATION_80, NULL));
		
		gs_unlocked_station_80 = 1;
	
	}
	else													  
	{
		gs_unlocked_station_80 = 0;
	}
	
	/* Check if Cylinder_RotationUp 80 */
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_ROTATION_UP_STATION_80", &gs_cylinder_rotation_up_station_80));
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_ROTATION_DOWN_STATION_80", &gs_cylinder_rotation_down_station_80));
	if (gs_cylinder_rotation_up_station_80 && !gs_cylinder_rotation_down_station_80)
	{
	if (me->eventAPI && !gs_cylinder_rotation_is_up_80)
		EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_CYLYNDER_ROTATION_UP_STATION_80, NULL));
		gs_cylinder_rotation_is_up_80 = 1;
	}
	else
	{
		gs_cylinder_rotation_is_up_80 = 0;
	}
		/* Check if Cylinder_RotationDown 80 */
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_ROTATION_UP_STATION_80", &gs_cylinder_rotation_up_station_80));
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_ROTATION_DOWN_STATION_80", &gs_cylinder_rotation_down_station_80));
	if (gs_cylinder_rotation_down_station_80 && !gs_cylinder_rotation_up_station_80)
	{
	if (me->eventAPI && !gs_cylinder_rotation_is_down_station_80)
		EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_CYLYNDER_ROTATION_DOWN_STATION_80, NULL));
		gs_cylinder_rotation_is_down_station_80 = 1;
	}
	else
	{
		gs_cylinder_rotation_is_down_station_80 = 0;
	}
	
	/* Check if Cylinder_RotationSystemLeverUp 80 */
	
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_SYSTEM_LEV_UP_Station_80", &gs_cylinder_system_lev_up_station_80));
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_SYSTEM_LEV_DOWN_Station_80", &gs_cylinder_system_lev_down_station_80));
	if (gs_cylinder_system_lev_up_station_80 && !gs_cylinder_system_lev_down_station_80)
	{
	if (me->eventAPI && !gs_cylinder_system_lev_is_up_80)
		EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_CYLYNDER_SYSTEM_LEV_UP_STATION_80, NULL));
		gs_cylinder_system_lev_is_up_80 = 1;
	}
	else
	{
		gs_cylinder_system_lev_is_up_80 = 0;
	}

	/* Check if Cylinder_RotationSystemLeverDown 80 */
	
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_SYSTEM_LEV_UP_Station_80", &gs_cylinder_system_lev_up_station_80));
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_SYSTEM_LEV_DOWN_Station_80", &gs_cylinder_system_lev_down_station_80));
	if (!gs_cylinder_system_lev_up_station_80 && gs_cylinder_system_lev_down_station_80)
	{
	if (me->eventAPI && !gs_cylinder_system_lev_is_down_80)
		EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_CYLYNDER_SYSTEM_LEV_DOWN_STATION_80, NULL));
		gs_cylinder_system_lev_is_down_80 = 1;
	}
	else
	{
		gs_cylinder_system_lev_is_down_80 = 0;
	}
	


Error:
	EXCRETHROW(pexception);
}

#undef __FUNC__
#define __FUNC__	"TableThread"
SElExceptionPtr TableThread(struct _STester* me)
{
	SElExceptionPtr	pexception = NULL;
		bool_t			status_micro_cam = FALSE,
						is_table_rotation_finished = FALSE;
	
	
		uint32_t	is_panel_plugged_station_10,
					is_panel_unplugged_station_10,
					
					status_cylinder_airbag_up_station_30,
					status_cylinder_airbag_down_station_30,
					status_cylinder_centrage_front_station_30,
					status_cylinder_centrage_rear_station_30 ,
					is_panel_plugged_station_30,
					is_panel_unplugged_station_30,

					status_cylinder_rotation_up_station_60,
					status_cylinder_rotation_down_station_60,
					status_cylinder_centrage_front_station_60,
					status_cylinder_centrage_rear_station_60 ,
					is_cylinder_y_rear_station_60,
					is_cylinder_y_front_station_60,
					is_cylinder_z_up_station_60,
					is_cylinder_z_down_station_60,
					is_panel_plugged_station_60,
					is_panel_unplugged_station_60,
					
					is_panel_plugged_station_70,
					is_panel_unplugged_station_70,
					
					is_panel_plugged_station_80,
					is_panel_unplugged_station_80,
					status_cylinder_rotation_up_station_80,
					status_cylinder_rotation_down_station_80,
					status_cylinder_system_lev_up_station_80,
					status_cylinder_system_lev_down_station_80,
					is_cylinder_z_up_station_80,
					is_cylinder_z_down_station_80,
					is_cylinder_x_rear_station_80,
					is_cylinder_x_front_station_80,
					is_cylinder_y_rear_station_80,
					is_cylinder_y_front_station_80;

	bool_t			safe_bit = FALSE,
					home_bit = FALSE,
					mission_running;
			
	bool_t			printer_up;
	
	bool_t			is_station_10_ready = FALSE,
					is_station_20_ready = FALSE,
					is_station_30_ready = FALSE,
					is_station_40_ready = FALSE,
					is_station_50_ready = FALSE,
					is_station_60_ready = FALSE,
					is_station_70_ready = FALSE,
					is_station_80_ready = FALSE;


		   	/* STATION 10 */
			EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_STATION_10", &is_panel_plugged_station_10));
			EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_STATION_10", &is_panel_unplugged_station_10));
			EXCCHECK( me->GetSensor(me, "PRINTER_INPUT2", &printer_up));
			if( !is_panel_plugged_station_10 && is_panel_unplugged_station_10 && printer_up)
			{
				is_station_10_ready = TRUE;
			}
			else 
				is_station_10_ready = FALSE;

			
			/* STATION 20 */
			is_station_20_ready = TRUE;
			
			/* STATION 30 */
			EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_STATION_30", &is_panel_plugged_station_30));
			EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_STATION_30", &is_panel_unplugged_station_30));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_AIRBAG_UP_STATION_30", &status_cylinder_airbag_up_station_30));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_AIRBAG_DOWN_STATION_30", &status_cylinder_airbag_down_station_30));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_CENTRAGE_FRONT_STATION_30", &status_cylinder_centrage_front_station_30));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_CENTRAGE_REAR_STATION_30", &status_cylinder_centrage_rear_station_30));
			if( !is_panel_plugged_station_30 && is_panel_unplugged_station_30 &&
				status_cylinder_airbag_up_station_30 && !status_cylinder_airbag_down_station_30 &&
				status_cylinder_centrage_rear_station_30 && !status_cylinder_centrage_front_station_30)
			{
				is_station_30_ready = TRUE;
			}
			else 
				is_station_30_ready = FALSE;
			
			/* STATION 40 */
			is_station_40_ready = TRUE;
		
			/* STATION 50 */
			is_station_50_ready = TRUE;
		
		
			/* STATION 60 */
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Y_FRONT_STATION_60", &is_cylinder_y_front_station_60));
	  		EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Y_REAR_STATION_60", &is_cylinder_y_rear_station_60));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60", &is_cylinder_z_up_station_60));
	  		EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_60", &is_cylinder_z_down_station_60));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_ROTATION_UP_STATION_60", &status_cylinder_rotation_up_station_60));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_ROTATION_DOWN_STATION_60", &status_cylinder_rotation_down_station_60));
			EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_STATION_60", &is_panel_plugged_station_60));
			EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_STATION_60", &is_panel_unplugged_station_60));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_CENTRAGE_FRONT_STATION_60", &status_cylinder_centrage_front_station_60));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_CENTRAGE_REAR_STATION_60", &status_cylinder_centrage_rear_station_60));
			if( is_cylinder_y_rear_station_60 && !is_cylinder_y_front_station_60  &&
				is_cylinder_z_up_station_60   && !is_cylinder_z_down_station_60   &&
				status_cylinder_rotation_up_station_60 && !status_cylinder_rotation_down_station_60 &&
				status_cylinder_centrage_rear_station_60 && !status_cylinder_centrage_front_station_60 && 
				is_panel_unplugged_station_60 && !is_panel_plugged_station_60
				)
			{
				is_station_60_ready = TRUE;
			}
			else 
				is_station_60_ready = FALSE;

			
			/* STATION 70 */
			EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_STATION_70", &is_panel_plugged_station_70));
			EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_STATION_70", &is_panel_unplugged_station_70));
			EXCCHECK( me->GetSensor(me, "ROBOT_IN_FREE_AREA", &safe_bit));
			EXCCHECK( me->GetSensor(me, "MISSION_RUNNING", &mission_running));
			EXCCHECK( me->GetSensor(me, "HOME_POSITION", &home_bit));
			if(is_panel_unplugged_station_70 && !is_panel_plugged_station_70 && 
			   safe_bit == TRUE && home_bit == TRUE && mission_running == FALSE)
			{
				is_station_70_ready = TRUE;
			}
			else 
				is_station_70_ready = FALSE;
			
			
			/* STATION 80 */
			EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_STATION_80", &is_panel_plugged_station_80));
			EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_STATION_80", &is_panel_unplugged_station_80));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_ROTATION_UP_STATION_80", &status_cylinder_rotation_up_station_80));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_ROTATION_DOWN_STATION_80", &status_cylinder_rotation_down_station_80));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_SYSTEM_LEV_UP_Station_80", &status_cylinder_system_lev_up_station_80));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_SYSTEM_LEV_DOWN_Station_80", &status_cylinder_system_lev_down_station_80));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_FRONT_STATION_80", &is_cylinder_x_front_station_80));
	  		EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_REAR_STATION_80", &is_cylinder_x_rear_station_80));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Y_FRONT_STATION_80", &is_cylinder_y_front_station_80));
	  		EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Y_REAR_STATION_80", &is_cylinder_y_rear_station_80));
			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_80", &is_cylinder_z_up_station_80));
	  		EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_80", &is_cylinder_z_down_station_80));
			if( is_cylinder_z_up_station_80   && !is_cylinder_z_down_station_80	 &&
				is_cylinder_y_rear_station_80 && !is_cylinder_y_front_station_80 &&
			   is_panel_unplugged_station_80 && !is_panel_plugged_station_80 &&
			   status_cylinder_rotation_up_station_80 && !status_cylinder_rotation_down_station_80 &&
			   status_cylinder_system_lev_up_station_80 && !status_cylinder_system_lev_down_station_80)
				{
				is_station_80_ready = TRUE;
				}
			else 
				is_station_80_ready = FALSE;
			
	if(!is_station_10_ready || 
	   !is_station_20_ready || 
	   !is_station_30_ready || 
	   !is_station_40_ready || 
	   !is_station_50_ready || 
	   !is_station_60_ready || 
	   !is_station_70_ready ||
	   !is_station_80_ready)
		{
		me->SetSensor(me, "FORCAGE_ROTATION", 0);
		me->SetSensor(me, "START_ROTATION", 0); 	
		}
	
Error:
	EXCRETHROW(pexception);
}


#undef __FUNC__
#define __FUNC__	"RobotThread"
SElExceptionPtr RobotThread(struct _STester* me)
{
	SElExceptionPtr	pexception = NULL;
	bool_t				input = FALSE;
	

	/* task to check STOP_MESS */
	EXCCHECK( me->GetSensor(me, "COLLECTIVE_FAULT", &input)); /* Read :STOP MESS  */ 
    if (input == TRUE)
		EXCCHECK( me->SetSensor(me, "ERROR_ACK", TRUE));  /* CONF_MESS = FALSE  */

	
Error:
	EXCRETHROW(pexception);
}

/*Plug Panel station 10 */
#undef __FUNC__
#define __FUNC__	"PlugPanelStation10"
static SElExceptionPtr PlugPanelStation10
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;
	
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_PLUGGED_STATION_10, &hevent));

	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	if (microCam)
	{
	EXCCHECK( me->SetSensor(me, "CYLINDER_PLUG_STATION_10", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_UNPLUG_STATION_10", 0));

	gs_unglug_command_station_10 = 0;   /*Unplug flag station 10*/
  
	}
	
	if (timeout)
	{
		if(gs_plugged_station_10) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_plugged_station_10) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

/*Unplug Panel station 10 */
#undef __FUNC__
#define __FUNC__	"UnplugPanelStation10"
static SElExceptionPtr UnplugPanelStation10
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;

	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_UNPLUGGED_STATION_10, &hevent));

	EXCCHECK( me->SetSensor(me, "CYLINDER_UNPLUG_STATION_10", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_PLUG_STATION_10", 0));

	gs_unglug_command_station_10 = 1;   /*Unplug flag station 10*/

	if (timeout)
	{
		if(gs_unplugged_station_10) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_unplugged_station_10) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

/* Lock Panel station 10 */
#undef __FUNC__
#define __FUNC__	"LockPanelStation10"
static SElExceptionPtr LockPanelStation10
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;
	int32_t			sensor_locked = FALSE;

	
   	EXCCHECK( me->PlugPanelStation10(me, 5000));

	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_LOCKED_STATION_10, &hevent));

	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	if (microCam)
	{
	EXCCHECK( me->SetSensor(me, "CYLINDER_LOCK_STATION_10", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_UNLOCK_STATION_10", 0));
	}
	if (timeout)
	{
		if(gs_locked_station_10) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_locked_station_10) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}


/* Unlock Panel station 10 */
#undef __FUNC__
#define __FUNC__	"UnlockPanelStation10"
static SElExceptionPtr UnlockPanelStation10
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;

	
  	EXCCHECK( me->PlugPanelStation10(me, 5000));

	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_UNLOCKED_STATION_10, &hevent));
	

	EXCCHECK( me->SetSensor(me, "CYLINDER_UNLOCK_STATION_10", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_LOCK_STATION_10", 0));

	if (timeout)
	{
		if(gs_unlocked_station_10) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_unlocked_station_10) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

/*Ready Cycle print */
#undef __FUNC__
#define __FUNC__	"ReadyPrint"
static SElExceptionPtr ReadyPrint
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	static HANDLE			hevent;
	DWORD 					result;

	bool_t				printer_in_position = FALSE, 
						printer_input1 = FALSE,
						printer_input2 = FALSE,
						printer_input3 = FALSE,
						printer_input4 = FALSE,
						printer_input5 = FALSE,
						microCam;
	
	
	//wait cycle_print_finished OFF
	TIMEOUT_INIT();
	do{
		me->GetSensor(me, "PRINTER_INPUT1", &printer_input1);
		me->GetSensor(me, "PRINTER_INPUT2", &printer_input2);
		me->GetSensor(me, "PRINTER_INPUT3", &printer_input3);
		me->GetSensor(me, "PRINTER_INPUT4", &printer_input4);
		me->GetSensor(me, "PRINTER_INPUT5", &printer_input5);
		me->GetSensor(me, "MICRO_CAM", &microCam);
		
	if(!printer_input1 && 
	   printer_input2 && 
	   printer_input3 && 
	   printer_input4 &&
	   printer_input5)
		{
		/* SART = 0 */
		me->SetSensor(me, "START_PRINT", 0);
		break;
		}
	
	else if (microCam)
	{
	/* Electovalve AIR PRINTER */
	EXCCHECK( me->SetSensor(me, "PRINTER_AIR_VALVE", 1));
	Sleep(100);
	
	/* Reset Errors */
	EXCCHECK( me->SetSensor(me, "PRINTER_OUT2", 1));
	Sleep(100);
	}

	TIMEOUT_PRINTER_CHECK(timeout);
	
	}while(TRUE);

	
Error:
	me->SetSensor(me, "PRINTER_OUT2", 0);
	EXCRETHROW(pexception);
}

/*Start Cycle print */
#undef __FUNC__
#define __FUNC__	"CyclePrint"
static SElExceptionPtr CyclePrint
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	static HANDLE			hevent;
	DWORD 					result;

	bool_t				printer_in_position = FALSE, 
						printer_input2 = FALSE,
						microCam;
	
	
	EXCCHECK( me->GetSensor(me, "PRINTER_IN_POSITION", &printer_in_position));
	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	
	if(microCam && printer_in_position)
	{
	   /* PRINTER_START */
		EXCCHECK( me->SetSensor(me, "PRINTER_START", 1));
		Sleep(200);
		EXCCHECK( me->SetSensor(me, "PRINTER_START", 0));
	
		TIMEOUT_INIT();

			//wait cycle_print_finished OFF
  			do{
			me->GetSensor(me, "MICRO_CAM", &microCam);
				if(microCam == 0) EXCCHECK( me->SetSensor(me, "PRINTER_AIR_VALVE", 0));
	  			me->GetSensor(me, "PRINTER_INPUT2", &printer_input2);
	   			if (!printer_input2)
					break;
        		TIMEOUT_PRINTER_CHECK(timeout);
    		}while(TRUE);
		
			
			//wait cycle_print_finished ON
  			do{
				me->GetSensor(me, "MICRO_CAM", &microCam);
				if(microCam == 0) EXCCHECK( me->SetSensor(me, "PRINTER_AIR_VALVE", 0));
	  			me->GetSensor(me, "PRINTER_INPUT2", &printer_input2);
	   			if (printer_input2)
					break;
        		TIMEOUT_PRINTER_CHECK(timeout);
    		}while(TRUE);
		}
	
	else 
	{
	EXCTHROW( TESTER_ERROR_TIMEOUT, "Cycle Print Error");
	}
	
	
Error:
	me->SetSensor(me, "START_PRINT", 0);
	//me->SetSensor(me, "PRINTER_AIR_VALVE", 0);
	EXCRETHROW(pexception);
}

/*Plug Panel station 30 */
#undef __FUNC__
#define __FUNC__	"PlugPanelStation30"
static SElExceptionPtr PlugPanelStation30
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;
	
	
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_PLUGGED_STATION_30, &hevent));

	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	if (microCam)
	{
	EXCCHECK( me->SetSensor(me, "CYLINDER_PLUG_STATION_30", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_UNPLUG_STATION_30", 0));
	}
	
	
	if (timeout)
	{
		if(gs_plugged_station_30) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_plugged_station_30) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

/*Unplug Panel station 30 */
#undef __FUNC__
#define __FUNC__	"UnplugPanelStation30"
static SElExceptionPtr UnplugPanelStation30
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;

	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_UNPLUGGED_STATION_30, &hevent));

	EXCCHECK( me->SetSensor(me, "CYLINDER_UNPLUG_STATION_30", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_PLUG_STATION_30", 0));
	
	if (timeout)
	{
		if(gs_unplugged_station_30) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_unplugged_station_30) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

 /* Lock Panel station 30 */
#undef __FUNC__
#define __FUNC__	"LockPanelStation30"
static SElExceptionPtr LockPanelStation30
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;
	int32_t			sensor_locked = FALSE;

	
	EXCCHECK( me->PlugPanelStation30(me, 5000));

	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_LOCKED_STATION_30, &hevent));

	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	if (microCam)
	{
	EXCCHECK( me->SetSensor(me, "CYLINDER_LOCK_STATION_30", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_UNLOCK_STATION_30", 0));
	}
	
	if (timeout)
	{
		if(gs_locked_station_30) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_locked_station_30) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "EVNT_JIG_ITEM_LOCKED_STATION_30");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

/* Unlock Panel station 30 */
#undef __FUNC__
#define __FUNC__	"UnlockPanelStation30"
static SElExceptionPtr UnlockPanelStation30
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;

	
	EXCCHECK( me->PlugPanelStation30(me, 5000));
	
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_UNLOCKED_STATION_30, &hevent));
	
	EXCCHECK( me->SetSensor(me, "CYLINDER_UNLOCK_STATION_30", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_LOCK_STATION_30", 0));
	if (timeout)
	{
		if(gs_unlocked_station_30) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_unlocked_station_30) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

/*MoveCylinderCentrageFront Station30 */
#undef __FUNC__
#define __FUNC__	"MoveCylinderCentrageFrontStation30"
static SElExceptionPtr MoveCylinderCentrageFrontStation30
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam,sensor_Pos_0;

	
	/* check if pince unbloccked before move front */
	if (!gs_cylinder_gripper_centrage_is_unblocked_30 && gs_cylinder_gripper_centrage_is_blocked_30)
	{
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_CYLINDER_GRIPPER_CENTRAGE_UNBLOCK_STATION_30, &hevent));

	EXCCHECK( me->SetSensor(me, "CYLINDER_GRIPPER_CENTRAGE_BLOCK_STATION_30", 0));
	EXCCHECK( me->SetSensor(me, "CYLINDER_GRIPPER_CENTRAGE_UNBLOCK_STATION_30", 1));

	if (timeout)
	{
		if(gs_cylinder_gripper_centrage_is_unblocked_30) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_cylinder_gripper_centrage_is_unblocked_30) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "EVNT_CYLINDER_GRIPPER_CENTRAGE_UNBLOCK_STATION_30");
		}
	}
	}
	
	/* move front */
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_CYLINDER_CENTRAGE_FRONT_STATION_30, &hevent));
	
	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));

   	if (microCam && gs_cylinder_gripper_centrage_is_unblocked_30 && !gs_cylinder_gripper_centrage_is_blocked_30)
	{
	EXCCHECK( me->SetSensor(me, "CYLINDER_CENTRAGE_FRONT_STATION_30", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_CENTRAGE_REAR_STATION_30", 0));
	}
	
	if (timeout)
	{
		if(gs_cylinder_centrage_is_front_30) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_cylinder_centrage_is_front_30) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "EVNT_CYLINDER_CENTRAGE_FRONT_STATION_30");
		}
	}

	/* check if pince blocked after move front */
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_CYLINDER_GRIPPER_CENTRAGE_BLOCK_STATION_30, &hevent));

	EXCCHECK( me->SetSensor(me, "CYLINDER_GRIPPER_CENTRAGE_BLOCK_STATION_30", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_GRIPPER_CENTRAGE_UNBLOCK_STATION_30", 0));

	if (timeout)
	{
		if(gs_cylinder_gripper_centrage_is_blocked_30) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_cylinder_gripper_centrage_is_blocked_30) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "EVNT_CYLINDER_GRIPPER_CENTRAGE_BLOCK_STATION_30");
		}
	}

Error:
	EXCRETHROW(pexception);
}

/*MoveCylinderCentrageRear Station30 */
#undef __FUNC__
#define __FUNC__	"MoveCylinderCentrageRearStation30"
static SElExceptionPtr MoveCylinderCentrageRearStation30
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;

	
	/* check if pince unbloccked before move rear */
	if (!gs_cylinder_gripper_centrage_is_unblocked_30 && gs_cylinder_gripper_centrage_is_blocked_30)
	{
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_CYLINDER_GRIPPER_CENTRAGE_UNBLOCK_STATION_30, &hevent));

	EXCCHECK( me->SetSensor(me, "CYLINDER_GRIPPER_CENTRAGE_UNBLOCK_STATION_30", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_GRIPPER_CENTRAGE_BLOCK_STATION_30", 0));

	if (timeout)
	{
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_cylinder_gripper_centrage_is_unblocked_30) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "EVNT_CYLINDER_GRIPPER_CENTRAGE_UNBLOCK_STATION_30");
		}
	}
	}

	Sleep(300);

	/* move rear */
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_CYLINDER_CENTRAGE_REAR_STATION_30, &hevent));
  
	if (gs_cylinder_gripper_centrage_is_unblocked_30 && !gs_cylinder_gripper_centrage_is_blocked_30)
	{
		EXCCHECK( me->SetSensor(me, "CYLINDER_CENTRAGE_FRONT_STATION_30", 0));
		EXCCHECK( me->SetSensor(me, "CYLINDER_CENTRAGE_REAR_STATION_30", 1));
	}
	if (timeout)
	{
		if(gs_cylinder_centrage_is_rear_30) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_cylinder_centrage_is_rear_30) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "EVNT_CYLINDER_CENTRAGE_REAR_STATION_30");
		}
	}
	
	
Error:
	EXCRETHROW(pexception);
}



/*MoveCylinderGripperCentrageBlock Station30 */
#undef __FUNC__
#define __FUNC__	"MoveCylinderGripperCentrageBlockStation30"
static SElExceptionPtr MoveCylinderGripperCentrageBlockStation30
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;

	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_CYLINDER_GRIPPER_CENTRAGE_BLOCK_STATION_30, &hevent));

	EXCCHECK( me->SetSensor(me, "CYLINDER_GRIPPER_CENTRAGE_BLOCK_STATION_30", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_GRIPPER_CENTRAGE_UNBLOCK_STATION_30", 0));

	if (timeout)
	{
		if(gs_cylinder_gripper_centrage_is_blocked_30) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_cylinder_gripper_centrage_is_blocked_30) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

/*MoveCylinderGripperCentrageUnblock Station30 */
#undef __FUNC__
#define __FUNC__	"MoveCylinderGripperCentrageUnblockStation30"
static SElExceptionPtr MoveCylinderGripperCentrageUnblockStation30
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;

	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_CYLINDER_GRIPPER_CENTRAGE_UNBLOCK_STATION_30, &hevent));

	EXCCHECK( me->SetSensor(me, "CYLINDER_GRIPPER_CENTRAGE_BLOCK_STATION_30", 0));
	EXCCHECK( me->SetSensor(me, "CYLINDER_GRIPPER_CENTRAGE_UNBLOCK_STATION_30", 1));

	if (timeout)
	{
		if(gs_cylinder_gripper_centrage_is_unblocked_30) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_cylinder_gripper_centrage_is_unblocked_30) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

/*MoveCylinderAirbagUp_Station30 */
#undef __FUNC__
#define __FUNC__	"MoveCylinderAirbagUpStation30"
static SElExceptionPtr MoveCylinderAirbagUpStation30
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;

	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_CYLINDER_AIRBAG_UP_STATION_30, &hevent));

	EXCCHECK( me->SetSensor(me, "CYLINDER_AIRBAG_DOWN_STATION_30", 1));
	Sleep(TIME_AIR);
	EXCCHECK( me->SetSensor(me, "CYLINDER_AIRBAG_UP_STATION_30", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_AIRBAG_DOWN_STATION_30", 0));

	if (timeout)
	{
		if(gs_cylinder_airbag_is_up_30) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_cylinder_airbag_is_up_30) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

/*MoveCylinderAirbagDownStation30 */
#undef __FUNC__
#define __FUNC__	"MoveCylinderAirbagDownStation30"
static SElExceptionPtr MoveCylinderAirbagDownStation30
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t 		microCam;

	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_CYLINDER_AIRBAG_DOWN_STATION_30, &hevent));
	
	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	if (microCam)
	{
	EXCCHECK( me->SetSensor(me, "CYLINDER_AIRBAG_UP_STATION_30", 0));
	EXCCHECK( me->SetSensor(me, "CYLINDER_AIRBAG_DOWN_STATION_30", 1));
	}
	if (timeout)
	{
		if(gs_cylinder_airbag_is_down_30) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_cylinder_airbag_is_down_30) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

/*Plug Panel station 60 */
#undef __FUNC__
#define __FUNC__	"PlugPanelStation60"
static SElExceptionPtr PlugPanelStation60
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;
	
	
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_PLUGGED_STATION_60, &hevent));

	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	if (microCam)
	{
	EXCCHECK( me->SetSensor(me, "CYLINDER_PLUG_STATION_60", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_UNPLUG_STATION_60", 0));
	
	gs_unglug_command_station_60 = 0;   /*Unplug flag station 60*/

	}
	
	
	if (timeout)
	{
		if(gs_plugged_station_60) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_plugged_station_60) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

/*Unplug Panel station 60 */
#undef __FUNC__
#define __FUNC__	"UnplugPanelStation60"
static SElExceptionPtr UnplugPanelStation60
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;

	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_UNPLUGGED_STATION_60, &hevent));

	EXCCHECK( me->SetSensor(me, "CYLINDER_UNPLUG_STATION_60", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_PLUG_STATION_60", 0));
	
	gs_unglug_command_station_60 = 1;   /*Unplug flag station 60*/
	
	if (timeout)
	{
		if(gs_unplugged_station_60) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_unplugged_station_60) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

 /* Lock Panel station 60 */
#undef __FUNC__
#define __FUNC__	"LockPanelStation60"
static SElExceptionPtr LockPanelStation60
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;
	int32_t			sensor_locked = FALSE;

	
	EXCCHECK( me->PlugPanelStation60(me, 5000));

	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_LOCKED_STATION_60, &hevent));

	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	if (microCam)
	{
	EXCCHECK( me->SetSensor(me, "CYLINDER_LOCK_STATION_60", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_UNLOCK_STATION_60", 0));
	}
	
	if (timeout)
	{
		if(gs_locked_station_60) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_locked_station_60) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "EVNT_JIG_ITEM_LOCKED_STATION_60");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

/* Unlock Panel station 60 */
#undef __FUNC__
#define __FUNC__	"UnlockPanelStation60"
static SElExceptionPtr UnlockPanelStation60
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;

	
	EXCCHECK( me->PlugPanelStation60(me, 5000));
	
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_UNLOCKED_STATION_60, &hevent));
	
	EXCCHECK( me->SetSensor(me, "CYLINDER_UNLOCK_STATION_60", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_LOCK_STATION_60", 0));
	if (timeout)
	{
		if(gs_unlocked_station_60) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_unlocked_station_60) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

/*MoveCylinderCentrageFront Station60 */
#undef __FUNC__
#define __FUNC__	"MoveCylinderCentrageFrontStation60"
static SElExceptionPtr MoveCylinderCentrageFrontStation60
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam,sensor_Pos_0,
					cylinder_gripper_block_station_60, 
					cylinder_gripper_unblock_station_60;
	
		
	/* check if Motor In Position 0 */
	EXCCHECK( me->GetSensor(me, "SENSOR_MOTOR_POS_0_STATION_60", &sensor_Pos_0));
	if (sensor_Pos_0 == 0)
		EXCTHROW( TESTER_ERROR_TIMEOUT, "Motor Station 60 not in Position 0");
	
	/* check if pince unbloccked before move front */
	EXCCHECK( me->SetSensor(me, "CYLINDER_GRIPPER_CENTRAGE_BLOCK_STATION_60", 0));
	EXCCHECK( me->SetSensor(me, "CYLINDER_GRIPPER_CENTRAGE_UNBLOCK_STATION_60", 1));

	/* move front */
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_CYLINDER_CENTRAGE_FRONT_STATION_60, &hevent));
	
	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CENTRAGE_BLOCK_STATION_60", &cylinder_gripper_block_station_60));
	EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CENTRAGE_UNBLOCK_STATION_60", &cylinder_gripper_unblock_station_60));
   	if (microCam && cylinder_gripper_unblock_station_60 && !cylinder_gripper_block_station_60)
	{
	EXCCHECK( me->SetSensor(me, "CYLINDER_CENTRAGE_FRONT_STATION_60", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_CENTRAGE_REAR_STATION_60", 0));
	}
	
	if (timeout)
	{
		if(gs_cylinder_centrage_is_front_60) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_cylinder_centrage_is_front_60) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "EVNT_CYLINDER_CENTRAGE_FRONT_STATION_60");
		}
	}

	/* check if pince bloccked after move front */
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_CYLINDER_GRIPPER_CENTRAGE_BLOCK_STATION_60, &hevent));

	EXCCHECK( me->SetSensor(me, "CYLINDER_GRIPPER_CENTRAGE_BLOCK_STATION_60", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_GRIPPER_CENTRAGE_UNBLOCK_STATION_60", 0));

	if (timeout)
	{
		if(gs_cylinder_gripper_centrage_is_blocked_60) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_cylinder_gripper_centrage_is_blocked_60) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "EVNT_CYLINDER_GRIPPER_CENTRAGE_BLOCK_STATION_60");
		}
	}

Error:
	EXCRETHROW(pexception);
}

/*MoveCylinderCentrageRear Station60 */
#undef __FUNC__
#define __FUNC__	"MoveCylinderCentrageRearStation60"
static SElExceptionPtr MoveCylinderCentrageRearStation60
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			cylinder_gripper_block_station_60, 
					cylinder_gripper_unblock_station_60;
	
	TIMEOUT_INIT();
	
	EXCCHECK( me->SetSensor(me, "CYLINDER_GRIPPER_CENTRAGE_UNBLOCK_STATION_60", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_GRIPPER_CENTRAGE_BLOCK_STATION_60", 0));
	do
	{
	Sleep(100);
	EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CENTRAGE_BLOCK_STATION_60", &cylinder_gripper_block_station_60));
	EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CENTRAGE_UNBLOCK_STATION_60", &cylinder_gripper_unblock_station_60));
	if (cylinder_gripper_unblock_station_60 && !cylinder_gripper_block_station_60)
		break;
	
	TIMEOUT_CHECK( 2000);
	
	}while (TRUE);
	
	/* move rear */
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_CYLINDER_CENTRAGE_REAR_STATION_60, &hevent));
  
	EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CENTRAGE_BLOCK_STATION_60", &cylinder_gripper_block_station_60));
	EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CENTRAGE_UNBLOCK_STATION_60", &cylinder_gripper_unblock_station_60));
	if (cylinder_gripper_unblock_station_60 && !cylinder_gripper_block_station_60)
	{
		EXCCHECK( me->SetSensor(me, "CYLINDER_CENTRAGE_REAR_STATION_60", 1));
		EXCCHECK( me->SetSensor(me, "CYLINDER_CENTRAGE_FRONT_STATION_60", 0));
	}

	if (timeout)
	{
		if(gs_cylinder_centrage_is_rear_60) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_cylinder_centrage_is_rear_60) goto Error;
				EXCTHROW( TESTER_ERROR_TIMEOUT, "EVNT_CYLINDER_CENTRAGE_REAR_STATION_60");
		}
	}
	
	
Error:
	EXCRETHROW(pexception);
}



/*MoveCylinderGripperCentrageBlock Station60 */
#undef __FUNC__
#define __FUNC__	"MoveCylinderGripperCentrageBlockStation60"
static SElExceptionPtr MoveCylinderGripperCentrageBlockStation60
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;

	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_CYLINDER_GRIPPER_CENTRAGE_BLOCK_STATION_60, &hevent));

	EXCCHECK( me->SetSensor(me, "CYLINDER_GRIPPER_CENTRAGE_BLOCK_STATION_60", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_GRIPPER_CENTRAGE_UNBLOCK_STATION_60", 0));

	if (timeout)
	{
		if(gs_cylinder_gripper_centrage_is_blocked_60) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_cylinder_gripper_centrage_is_blocked_60) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

/*MoveCylinderGripperCentrageUnblock Station60 */
#undef __FUNC__
#define __FUNC__	"MoveCylinderGripperCentrageUnblockStation60"
static SElExceptionPtr MoveCylinderGripperCentrageUnblockStation60
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;

	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_CYLINDER_GRIPPER_CENTRAGE_UNBLOCK_STATION_60, &hevent));

	EXCCHECK( me->SetSensor(me, "CYLINDER_GRIPPER_CENTRAGE_BLOCK_STATION_60", 0));
	EXCCHECK( me->SetSensor(me, "CYLINDER_GRIPPER_CENTRAGE_UNBLOCK_STATION_60", 1));

	if (timeout)
	{
		if(gs_cylinder_gripper_centrage_is_unblocked_60) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_cylinder_gripper_centrage_is_unblocked_60) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

/*MoveCylinderRotationUp_Station60 */
#undef __FUNC__
#define __FUNC__	"MoveCylinderRotationUpStation60"
static SElExceptionPtr MoveCylinderRotationUpStation60
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;

	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_CYLYNDER_ROTATION_UP_STATION_60, &hevent));

	EXCCHECK( me->SetSensor(me, "CYLINDER_ROTATION_DOWN_STATION_60", 1));
	Sleep(TIME_AIR);
	EXCCHECK( me->SetSensor(me, "CYLINDER_ROTATION_UP_STATION_60", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_ROTATION_DOWN_STATION_60", 0));

	if (timeout)
	{
		if(gs_cylinder_rotation_is_up_60) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_cylinder_rotation_is_up_60) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

/*MoveCylinderRotationDownStation60 */
#undef __FUNC__
#define __FUNC__	"MoveCylinderRotationDownStation60"
static SElExceptionPtr MoveCylinderRotationDownStation60
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t 		microCam;

	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_CYLYNDER_ROTATION_DOWN_STATION_60, &hevent));
	
	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	if (microCam)
	{
	EXCCHECK( me->SetSensor(me, "CYLINDER_ROTATION_UP_STATION_60", 0));
	EXCCHECK( me->SetSensor(me, "CYLINDER_ROTATION_DOWN_STATION_60", 1));
	}
	if (timeout)
	{
		if(gs_cylinder_rotation_is_down_60) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_cylinder_rotation_is_down_60) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}			  

/* RemoveInsertKeyStation60 */
#undef __FUNC__
#define __FUNC__	"Tester::RemoveInsertKeyStation60"
static SElExceptionPtr RemoveInsertKeyStation60(struct _STester* me, int32_t timeout, bool_t wait_end, int32_t Mode)
{
	SElExceptionPtr		pexception = NULL;
	int32_t				error = 0;
	bool_t				is_hand_block = FALSE,
						is_hand_unblock = FALSE,
						is_cylinder_z_up = FALSE,
						is_cylinder_z_down = FALSE,
						is_cylinder_y_rear = FALSE,
						is_cylinder_y_front = FALSE,
						is_panel_unlocked_Left = FALSE,
						is_panel_locked_Left = FALSE,
						is_panel_unlocked_Right = FALSE,
						is_panel_locked_Right = FALSE,
						presence_key = FALSE;
	int32_t				microCam;
	
	bool_t				cylinder_y_front,
						cylinder_y_rear,
						cylinder_z_up,
						cylinder_z_down,
						hand_block,
						hand_unblock;

	int32_t				step;
	bool_t			    is_product_took = 0;
	
		
	if (Mode==MODE_REMOVE_KEY)
 		EXCCHECK( me->ResetPickPlaceStation60(me, timeout, TRUE )); 

	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60", &is_cylinder_z_up));
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_60", &is_cylinder_z_down));
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Y_REAR_STATION_60", &is_cylinder_y_rear));
    EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Y_FRONT_STATION_60", &is_cylinder_y_front));
	EXCCHECK( me->GetSensor(me, "SENSOR_PRESENCE_KEY_STATION_60", &presence_key));
	
	if((microCam && 
		is_cylinder_z_up && !is_cylinder_z_down && 
		is_cylinder_y_rear && !is_cylinder_y_front))
	{
	TIMEOUT_INIT();
	step = INIT;
	
	while( TRUE)
	{
		if (!is_product_took)
		{
			
			switch  (step)
			{
			case  INIT :
								  
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60", &cylinder_z_up));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_60", &cylinder_z_down));
					EXCCHECK( me->GetSensor(me, "SENSOR_BLOCK_STATION_60", &hand_block));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_UNBLOCK_STATION_60", &hand_unblock));
					EXCCHECK( me->GetSensor(me, "SENSOR_PRESENCE_KEY_STATION_60", &presence_key));
				
					if (Mode==MODE_REMOVE_KEY)
					{
						EXCCHECK( me->SetSensor(me, "CYLINDER_UNBLOCK_STATION_60", 1));
				   		EXCCHECK( me->SetSensor(me, "CYLINDER_BLOCK_STATION_60", 0));
					}
				
					if(!cylinder_z_up && cylinder_z_down) 
							{
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_UP_STATION_60", 1));
							Sleep(10);
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_DOWN_STATION_60", 0));
				 			}  
				   else if ((cylinder_z_up && !cylinder_z_down && Mode==MODE_INSERT_KEY && !presence_key) || 
					   	   (cylinder_z_up && !cylinder_z_down && Mode==MODE_REMOVE_KEY))
							{
							step = MOVE_TO_PALETT;		
				 			} 
		
			break;
			
			case  MOVE_TO_PALETT : 
				   		
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Y_FRONT_STATION_60", &cylinder_y_front));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Y_REAR_STATION_60", &cylinder_y_rear));
				   
				   if(!cylinder_y_front && cylinder_y_rear) 
							{
							EXCCHECK( me->SetSensor(me, "CYLINDER_Y_REAR_STATION_60", 0));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Y_FRONT_STATION_60", 1));
				 			}  
				   else if (cylinder_y_front && !cylinder_y_rear) 
							{
								step = MOVE_TO_PANEL ;	 		
							} 
			break;
			
			case  MOVE_TO_PANEL : 
				   		
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60", &cylinder_z_up));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_60", &cylinder_z_down));
				   
				   if(!cylinder_z_down && cylinder_z_up) 
							{
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_DOWN_STATION_60", 1));
							Sleep(100);
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_UP_STATION_60", 0));
				 			}  
				   else if (cylinder_z_down && !cylinder_z_up) 
							{
								if (Mode==MODE_REMOVE_KEY)
									step= TAKE_ON_KEY ;
								else if (Mode == MODE_INSERT_KEY) 
									step= TAKE_OFF_KEY ;		
				 			} 
				   
			break;											 
																
			case  TAKE_ON_KEY: 
				   		
	  				EXCCHECK( me->GetSensor(me, "SENSOR_BLOCK_STATION_60", &hand_block));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_UNBLOCK_STATION_60", &hand_unblock));
					EXCCHECK( me->GetSensor(me, "SENSOR_PRESENCE_KEY_STATION_60", &presence_key));
				  
				   //if(!hand_block && hand_unblock) 
				   if(hand_unblock) 
							{
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_BLOCK_STATION_60", 1));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_UNBLOCK_STATION_60", 0));
				 			}  
				   else if(!hand_unblock) 
				   			{
							//Sleep(1000);
							//Sleep(500);
							step= PICK_UP_KEY ;  
				 			} 
			break;
			
			case  TAKE_OFF_KEY: 
				
	  				EXCCHECK( me->GetSensor(me, "SENSOR_BLOCK_STATION_60", &hand_block));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_UNBLOCK_STATION_60", &hand_unblock));
				
					if(!hand_unblock) 
							{
							EXCCHECK( me->SetSensor(me, "CYLINDER_UNBLOCK_STATION_60", 1));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_BLOCK_STATION_60", 0));
				 			}  
				   //else if(hand_unblock && !hand_block ) 
				   else if(hand_unblock && !hand_block ) 
				   			{
								step= PICK_UP_KEY ;  
							}
			break;
			
			case  PICK_UP_KEY : 
				   		
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60", &cylinder_z_up));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_60", &cylinder_z_down));

				   if(!cylinder_z_up && cylinder_z_down) 
							{
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_UP_STATION_60", 1));
							Sleep(10);
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_DOWN_STATION_60", 0));
				 			}  
				   else if (cylinder_z_up && !cylinder_z_down)
							{
							step = MOVE_TO_HOME;		
				 			} 
			break;
			
			case  MOVE_TO_HOME : 
				   		
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Y_FRONT_STATION_60", &cylinder_y_front));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Y_REAR_STATION_60", &cylinder_y_rear));
				    EXCCHECK( me->GetSensor(me, "SENSOR_PRESENCE_KEY_STATION_60", &presence_key));
					
					if(!cylinder_y_rear && cylinder_y_front) 
							{
							EXCCHECK( me->SetSensor(me, "CYLINDER_Y_REAR_STATION_60", 1));
							Sleep(10);
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Y_FRONT_STATION_60", 0));
							}  
				   else if ((cylinder_y_rear && !cylinder_y_front && Mode==MODE_INSERT_KEY ) ||
					   		(cylinder_y_rear && !cylinder_y_front && Mode==MODE_REMOVE_KEY ) )
							{
							step = REMOVE_PLACE_FINIHED ;		
				 			} 
			break;


			case  REMOVE_PLACE_FINIHED :
				 is_product_took = TRUE;
			break;
				
			}
		}

		else break;
		
		TIMEOUT_CHECK( timeout);
		}	

	}

	else 
	{
 		EXCCHECK( me->ResetPickPlaceStation60(me, timeout, TRUE )); 
		EXCTHROW( TESTER_ERROR_TIMEOUT, "Conditions pour Insertion/Enlevement Clé Rouge sont pas pret");
	}
		
	
Error:
	EXCRETHROW(pexception);
}
 
/* PickPlace station 60 */
#undef __FUNC__
#define __FUNC__	"Tester::ResetPickPlaceStation60"
static SElExceptionPtr ResetPickPlaceStation60(struct _STester* me, int32_t timeout, bool_t wait_end)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	uint32_t		step = TAKE_OFF_KEY;
	bool_t			is_product_took = FALSE;
	bool_t			cylinder_y_front,
					cylinder_y_rear,
					cylinder_z_up,
					cylinder_z_down,
					hand_block,
					hand_unblock;
	
	TIMEOUT_INIT();
	
	while( TRUE)
	{
		if (!is_product_took)
		{
			switch  (step)
			{
			case  TAKE_OFF_KEY: 
				
					EXCCHECK( me->SetSensor(me, "CYLINDER_UNBLOCK_STATION_60", 1));
				   	EXCCHECK( me->SetSensor(me, "CYLINDER_BLOCK_STATION_60", 0));
	  		
					EXCCHECK( me->GetSensor(me, "SENSOR_BLOCK_STATION_60", &hand_block));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_UNBLOCK_STATION_60", &hand_unblock));

					if(hand_unblock && !hand_block) 
				   			{
							step = PICK_UP_KEY;		
				 			}

			break;
			
			case  PICK_UP_KEY : 
					
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60", &cylinder_z_up));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_60", &cylinder_z_down));
				   
				   if((!cylinder_z_up && cylinder_z_down) || (!cylinder_z_up && !cylinder_z_down))
							{
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_UP_STATION_60", 1));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_DOWN_STATION_60", 0));
				 			}  
				   else if (cylinder_z_up && !cylinder_z_down) 
							{
							step = MOVE_TO_HOME;		
				 			} 
			break;
			
			case  MOVE_TO_HOME : 
				   		
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Y_FRONT_STATION_60", &cylinder_y_front));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Y_REAR_STATION_60", &cylinder_y_rear));
				   
				   if( (!cylinder_y_rear && !cylinder_y_front) || (!cylinder_y_rear && cylinder_y_front)) 
							{
							EXCCHECK( me->SetSensor(me, "CYLINDER_Y_REAR_STATION_60", 1));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Y_FRONT_STATION_60", 0));
				 			}  
				   else if (cylinder_y_rear && !cylinder_y_front) 
							{
							step  = REMOVE_PLACE_FINIHED;		
				 			} 
			break;
			
			case  REMOVE_PLACE_FINIHED :
				
				 is_product_took = TRUE;
				 
			break;
				
			}
		}

		else break;
		
		TIMEOUT_CHECK( timeout);
	}
	
Error:
	EXCRETHROW(pexception);
}

/*Plug Panel station 70 */
#undef __FUNC__
#define __FUNC__	"PlugPanelStation70"
static SElExceptionPtr PlugPanelStation70
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;
	
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_PLUGGED_STATION_70, &hevent));

	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	if (microCam)
	{
	gs_unglug_command_station_70 = 0;   /*Unplug flag station 70*/

	EXCCHECK( me->SetSensor(me, "CYLINDER_PLUG_STATION_70", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_UNPLUG_STATION_70", 0));

	}
	if (timeout)
	{
		if(gs_plugged_station_70) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_plugged_station_70) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

/*Plug Panel station 70 */
#undef __FUNC__
#define __FUNC__	"UnplugPanelStation70"
static SElExceptionPtr UnplugPanelStation70
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;

	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_UNPLUGGED_STATION_70, &hevent));

	EXCCHECK( me->SetSensor(me, "CYLINDER_UNPLUG_STATION_70", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_PLUG_STATION_70", 0));
	
	gs_unglug_command_station_70 = 1;   /*Unplug flag station 70*/


	if (timeout)
	{
		if(gs_unplugged_station_70) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_unplugged_station_70) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

/*Plug Panel station 70 */
#undef __FUNC__
#define __FUNC__	"IlluminationStation70"
static SElExceptionPtr IlluminationStation70
(
	struct _STester* me,
	int32_t			status
)
{
	SElExceptionPtr	pexception = NULL;

	EXCCHECK( me->SetSensor(me, "LIGHT", status));
	
Error:
	EXCRETHROW(pexception);
}


 /* Lock Panel station 70 */
#undef __FUNC__
#define __FUNC__	"LockPanelStation70"
static SElExceptionPtr LockPanelStation70
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;
	int32_t			sensor_locked = FALSE;

  	EXCCHECK( me->PlugPanelStation70(me, 5000));

	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_LOCKED_STATION_70, &hevent));

  	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	if (microCam)
	{
	EXCCHECK( me->SetSensor(me, "CYLINDER_LOCK_STATION_70", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_UNLOCK_STATION_70", 0));
	}
	if (timeout)
	{
		if(gs_locked_station_70) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_locked_station_70) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

/* Unlock Panel station 70 */
#undef __FUNC__
#define __FUNC__	"UnlockPanelStation70"
static SElExceptionPtr UnlockPanelStation70
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;

	
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_UNLOCKED_STATION_70, &hevent));
	
	EXCCHECK( me->SetSensor(me, "CYLINDER_UNLOCK_STATION_70", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_LOCK_STATION_70", 0));
	if (timeout)
	{
		if(gs_unlocked_station_70) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_unlocked_station_70) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

#undef  __FUNC__
#define __FUNC__  "Timer_Robot"
int CVICALLBACK Timer_Robot (int reserved, int theTimerId, int event,
                                 void *callbackData, int eventData1,
                                 int eventData2)
{                                                   
	SElExceptionPtr	pexception = NULL;
	STester* me = (STesterPtr) callbackData; 

    /* execute RobotThread to refresh inputs */
	EXCCHECK(RobotThread(me));
	
Error:
	if (pexception)
	{
	EXCDISPLAY(pexception);
	DiscardAsyncTimer (g_timerId3);
	}
	return 0;
}

 /* Init Robot station 70 */
#undef __FUNC__
#define __FUNC__	"RobotInitStation70"
static SElExceptionPtr RobotInitStation70
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t input;
	int32_t step;
	bool_t Flag_Init__Robot_OK =0;
	
	TIMEOUT_INIT();
	step = ROBOT_STEP_1;
	
	//printf ("ROBOT_STEP_1 = TRUE\r\n");

	DiscardAsyncTimer (g_timerId3);
	
	EXCCHECK( me->SetSensor(me, "PROGRAM_START", FALSE)); 
	EXCCHECK( me->SetSensor(me, "MOVE_ENABLE", FALSE));  
	EXCCHECK( me->SetSensor(me, "ERROR_ACK", FALSE));  
	EXCCHECK( me->SetSensor(me, "DRIVER_OFF", FALSE));  
	EXCCHECK( me->SetSensor(me, "DRIVER_ON", FALSE));  
	EXCCHECK( me->SetSensor(me, "START_MOVEMENT", FALSE));  
	EXCCHECK( me->SetSensor(me, "PROGRAM_CODE_BIT0", FALSE));  
	EXCCHECK( me->SetSensor(me, "PROGRAM_CODE_BIT1", FALSE));  
	EXCCHECK( me->SetSensor(me, "PROGRAM_CODE_BIT2", FALSE));  
   	EXCCHECK( me->SetSensor(me, "PROGRAM_CODE_BIT3", FALSE));  
	EXCCHECK( me->SetSensor(me, "PROGRAM_CODE_BIT4", FALSE));  
	EXCCHECK( me->SetSensor(me, "HOMING_REQ", FALSE));  
	EXCCHECK( me->SetSensor(me, "CAMERA_END", FALSE));  
 	EXCCHECK( me->SetSensor(me, "TEST_CAMERA_END", FALSE));  

	
	EXCCHECK( me->SetSensor(me, "HOMING_REQ", TRUE));
   	Sleep(200);
	EXCCHECK( me->SetSensor(me, "HOMING_REQ", FALSE));

	while( TRUE)
	{
			switch  (step)
			{
			case  ROBOT_STEP_1 :
					//printf ("send : MOVE_ENABLE = TRUE\r\n");
					//printf ("send : DRIVER_OFF = TRUE\r\n");
					EXCCHECK( me->SetSensor(me, "MOVE_ENABLE", TRUE));  /* MOVE_ENABLE = TRUE  */  
					EXCCHECK( me->SetSensor(me, "DRIVER_OFF", TRUE));   /* DRIVER_OFF = TRUE  */  
					EXCCHECK( me->GetSensor(me, "ALARM_STOP", &input)); /* Read : ALM STOP  */
					//printf ("receive : ALARM_STOP = %d\r\n",input);
					 if (input == TRUE)
						{
						step++;	
						//printf ("ROBOT_STEP_2 = TRUE\r\n");

				 		} 
			break;
			
			case  ROBOT_STEP_2 :
					//printf ("send : DRIVER_ON = TRUE\r\n");
					EXCCHECK( me->SetSensor(me, "DRIVER_ON", TRUE));  /* DRIVER_ON = TRUE  */
					EXCCHECK( me->GetSensor(me, "DRIVERS_READY", &input)); /* Read :PERY RDY  */ 
					//printf ("Read : DRIVERS_READY [PERY RDY] = %d\r\n",input);

					 if (input == TRUE)
						{
						step++;	
						//printf ("ROBOT_STEP_3 = TRUE\r\n");
				 		} 
			break;
			
			case  ROBOT_STEP_3 :

					Sleep(300);
					//printf ("send : DRIVER_OFF = FALSE\r\n");
					//printf ("send : CONF_MESS = TRUE\r\n");
					EXCCHECK( me->SetSensor(me, "DRIVER_ON", FALSE));  /* DRIVER_OFF = FALSE  */  
					EXCCHECK( me->SetSensor(me, "ERROR_ACK", TRUE));  /* CONF_MESS = TRUE  */
					EXCCHECK( me->GetSensor(me, "COLLECTIVE_FAULT", &input)); /* Read :STOP MESS  */ 
					//printf ("Read : COLLECTIVE_FAULT [STOP MESS] = %d\r\n",input);
					 if (input == FALSE)
						{
						step++;	
						//printf ("ROBOT_STEP_4 = TRUE\r\n");
				 		} 
			break;

			case  ROBOT_STEP_4 :

					Sleep(300);
					//printf ("send : CONF_MESS = FALSE\r\n");
					//printf ("send : PROGRAM_START = TRUE\r\n");
					EXCCHECK( me->SetSensor(me, "ERROR_ACK", FALSE));  /* CONF_MESS = FALSE  */
					EXCCHECK( me->SetSensor(me, "PROGRAM_START", TRUE));  /* PROGRAM_START = TRUE  */
					EXCCHECK( me->GetSensor(me, "PROGRAM_ACTIVE", &input));  /* Read : PROGRAM_ACTIVE  */ 
					//printf ("Read : PROGRAM_ACTIVE = %d\r\n",input);
					if (input == TRUE)
						{
						step++;
						//printf ("ROBOT_STEP_END = TRUE\r\n");
				 		} 
			break; 
			

			case  ROBOT_STEP_END :
					Sleep(0);
					EXCCHECK( me->SetSensor(me, "PROGRAM_START", FALSE));  /* PROGRAM_START = FALSE  */
					Flag_Init__Robot_OK = TRUE;
			break;
		
			}
			if (Flag_Init__Robot_OK == TRUE)
			{
				/*Send program by default */
				EXCCHECK( me->SetSensor(me, "PROGRAM_CODE_BIT0", TRUE));
				EXCCHECK( me->SetSensor(me, "PROGRAM_CODE_BIT1", FALSE));
				EXCCHECK( me->SetSensor(me, "PROGRAM_CODE_BIT2", FALSE));
				EXCCHECK( me->SetSensor(me, "PROGRAM_CODE_BIT3", FALSE));
				EXCCHECK( me->SetSensor(me, "PROGRAM_CODE_BIT4", FALSE));
			 	g_timerId3 = NewAsyncTimer (0.1, -1, 1, Timer_Robot, (void*)me);
			 	break;
			}
			
			TIMEOUT_CHECK( timeout);
			Sleep(200);
	}
	

Error:
	EXCRETHROW(pexception);	
}

/* Robot Select Program Station 70 */
#undef __FUNC__
#define __FUNC__	"RobotSelectProgramStation70"
static SElExceptionPtr RobotSelectProgramStation70
(
	struct _STester* me,
	int32_t			program,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int             last_program;
	bool_t     		bit0,
					bit1,
					bit2,
					bit3,
					bit4;
	
	bool_t     		Last_bit0,
					Last_bit1,
					Last_bit2,
					Last_bit3,
					Last_bit4;
	
	//convert  program to binary
	bit0 = (program >> 0) & 1;
	bit1 = (program >> 1) & 1;
	bit2 = (program >> 2) & 1;
	bit3 = (program >> 3) & 1;
	bit4 = (program >> 4) & 1;
	
	EXCCHECK( me->GetSensor(me, "LAST_PROGRAM_CODE_BIT0", &Last_bit0));
	EXCCHECK( me->GetSensor(me, "LAST_PROGRAM_CODE_BIT1", &Last_bit1));
	EXCCHECK( me->GetSensor(me, "LAST_PROGRAM_CODE_BIT2", &Last_bit2));
	EXCCHECK( me->GetSensor(me, "LAST_PROGRAM_CODE_BIT3", &Last_bit3));
	EXCCHECK( me->GetSensor(me, "LAST_PROGRAM_CODE_BIT4", &Last_bit4));
	last_program =  Last_bit0*1 + Last_bit1*2 + Last_bit2*4 +Last_bit3*8 + Last_bit4*16;
	
	/* !!!CAN CRASH ROBOT or CAMERA : be careful this code check if there is mouvement from side to side to go the safety point*/
	
	if (last_program !=31 &&	   //last is safe central point
		(program/10 > last_program/10 || //move left to right lever
		 last_program/10 > program/10 ||//move right to left lever
		 last_program == 0))    //last is Home position
	{
	// call central safety point program
	EXCCHECK( me->SetSensor(me, "PROGRAM_CODE_BIT0", 1));
	EXCCHECK( me->SetSensor(me, "PROGRAM_CODE_BIT1", 1));
	EXCCHECK( me->SetSensor(me, "PROGRAM_CODE_BIT2", 1));
	EXCCHECK( me->SetSensor(me, "PROGRAM_CODE_BIT3", 1));
	EXCCHECK( me->SetSensor(me, "PROGRAM_CODE_BIT4", 1));
	Sleep(200); 

	//start mission central safety point
	EXCCHECK( me->RobotEndMissionStation70(me, 10000));
	
	//start mission central safety point
	EXCCHECK( me->RobotStartMissionStation70(me, 10000));
	
	
	// call new program  IF misison program safety finished
	EXCCHECK( me->SetSensor(me, "PROGRAM_CODE_BIT0", bit0));
	EXCCHECK( me->SetSensor(me, "PROGRAM_CODE_BIT1", bit1));
	EXCCHECK( me->SetSensor(me, "PROGRAM_CODE_BIT2", bit2));
	EXCCHECK( me->SetSensor(me, "PROGRAM_CODE_BIT3", bit3));
	EXCCHECK( me->SetSensor(me, "PROGRAM_CODE_BIT4", bit4));
	}

	else 
	{
	EXCCHECK( me->SetSensor(me, "PROGRAM_CODE_BIT0", bit0));
	EXCCHECK( me->SetSensor(me, "PROGRAM_CODE_BIT1", bit1));
	EXCCHECK( me->SetSensor(me, "PROGRAM_CODE_BIT2", bit2));
	EXCCHECK( me->SetSensor(me, "PROGRAM_CODE_BIT3", bit3));
	EXCCHECK( me->SetSensor(me, "PROGRAM_CODE_BIT4", bit4));
	}
	
Error:
	EXCRETHROW(pexception);
}

/* Robot Start MissionStation 70 */
#undef __FUNC__
#define __FUNC__	"RobotStartMissionStation70"
static SElExceptionPtr RobotStartMissionStation70
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	bool_t			input;
	bool_t			microCam;
	bool_t			mission_running;
	clock_t	_start_clock = clock();
	
	mission_running = 0;
	
	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	if (microCam)
	{

	do
	{
	EXCCHECK( me->GetSensor(me, "MISSION_RUNNING", &mission_running));
	//printf ("RobotStartMissionStation70 : expect [mission_running = FALSE] mission_running = %d\r\n", mission_running);
	Sleep(50);
	TIMEOUT_CHECK( timeout);
	}while (mission_running == TRUE);
	
	gs_mission_running_station_70 = FALSE;
	
	//printf ("send : START_MOVEMENT = TRUE\r\n");
	EXCCHECK( me->SetSensor(me, "START_MOVEMENT", TRUE));
	Sleep(200);
	//printf ("send : PROGRAM_START = TRUE\r\n");
	EXCCHECK( me->SetSensor(me, "PROGRAM_START", TRUE));
	Sleep(200);
	EXCCHECK( me->SetSensor(me, "PROGRAM_START", FALSE));

	Sleep(100);

	EXCCHECK( me->SetSensor(me, "PROGRAM_START", FALSE));  /* PROGRAM_START = FALSE  */
	Sleep(100);
	EXCCHECK( me->SetSensor(me, "PROGRAM_START", TRUE));  /* PROGRAM_START = TRUE  */

	do
	{
	//printf ("RobotStartMissionStation70 : expect [gs_mission_running_station_70 = TRUE] gs_mission_running_station_70 = %d\r\n", mission_running);
	Sleep(50);
	TIMEOUT_CHECK( timeout);
	}while (gs_mission_running_station_70 == FALSE);
	
	//printf ("send : PROGRAM_START = FALSE\r\n");
	
	EXCCHECK( me->SetSensor(me, "START_MOVEMENT", FALSE));
	
	
	//EXCCHECK( me->RobotEndMissionStation70(me, 5000)); 
	}

Error:
	me->SetSensor(me, "START_MOVEMENT", FALSE);
	EXCRETHROW(pexception);
}

/* Robot End MissionStation 70 */
#undef __FUNC__
#define __FUNC__	"RobotEndMissionStation70"
static SElExceptionPtr RobotEndMissionStation70
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	bool_t			input;
	
	//printf ("RobotEndMissionStation70 : CAMERA_END = TRUE\r\n");
	EXCCHECK( me->SetSensor(me, "CAMERA_END", TRUE));
	
	TIMEOUT_INIT();
	do
	{
	EXCCHECK( me->GetSensor(me, "MISSION_RUNNING", &input));
	//printf ("RobotEndMissionStation70 : expect [input = FALSE] input = %d\r\n", input);
	Sleep(50);
	TIMEOUT_CHECK( timeout);
	}while (input == TRUE);
	
Error:
	me->SetSensor(me, "CAMERA_END", FALSE);
	EXCRETHROW(pexception);
}

/* Robot End Test Station 70 */
#undef __FUNC__
#define __FUNC__	"RobotEndTestMissionStation70"
static SElExceptionPtr RobotEndTestMissionStation70
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	bool_t			input;
	

	EXCCHECK( me->SetSensor(me, "TEST_CAMERA_END", TRUE));
	EXCCHECK( me->SetSensor(me, "CAMERA_END", FALSE));
	
	TIMEOUT_INIT();
	do
	{
	EXCCHECK( me->GetSensor(me, "MISSION_RUNNING", &input));
	//printf ("receive : MISSION_RUNNING = %d\r\n", input);
	TIMEOUT_CHECK( timeout);
	Sleep(100);
	}while (input == TRUE);
	
	EXCCHECK( me->SetSensor(me, "TEST_CAMERA_END", FALSE));

Error:
	me->SetSensor(me, "TEST_CAMERA_END", FALSE);
	EXCRETHROW(pexception);
}

/* Robot Home Station 70 */
#undef __FUNC__
#define __FUNC__	"RobotHomeStation70"
static SElExceptionPtr RobotHomeStation70
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	bool_t			home_position,
					homing_in_progress;
	
	//printf ("send : HOMING_REQ = TRUE\r\n");
	EXCCHECK( me->RobotEndMissionStation70(me, 5000));
	EXCCHECK( me->SetSensor(me, "HOMING_REQ", TRUE));

	TIMEOUT_INIT();
	do
	{
	Sleep(100);
	EXCCHECK( me->GetSensor(me, "HOMING_IN_PROGRESS", &homing_in_progress));
	//printf ("receive : HOMING_IN_PROGRESS = %d\r\n", homing_in_progress);
	TIMEOUT_CHECK( timeout);
	}while (homing_in_progress == FALSE);
	
	EXCCHECK( me->SetSensor(me, "HOMING_REQ", FALSE));

	do
	{
	Sleep(100);
	EXCCHECK( me->GetSensor(me, "HOME_POSITION", &home_position));
	EXCCHECK( me->GetSensor(me, "HOMING_IN_PROGRESS", &homing_in_progress));
	//printf ("receive : HOME_POSITION = %d\r\n", home_position);
	//printf ("receive : HOMING_IN_PROGRESS = %d\r\n", homing_in_progress);
	TIMEOUT_CHECK( timeout);
	}while (home_position == FALSE || 
		    homing_in_progress == TRUE);
	

Error:
	me->SetSensor(me, "HOMING_REQ", FALSE);
	EXCRETHROW(pexception);
}


	
/*Plug Panel station 80 */
#undef __FUNC__
#define __FUNC__	"PlugPanelStation80"
static SElExceptionPtr PlugPanelStation80
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;
	
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_PLUGGED_STATION_80, &hevent));

	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	if (microCam)
	{
	EXCCHECK( me->SetSensor(me, "CYLINDER_PLUG_STATION_80", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_UNPLUG_STATION_80", 0));
	
	gs_unglug_command_station_80 = 0;   /*Unplug flag station 80*/

	}
	
	if (timeout)
	{
		if(gs_plugged_station_80) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_plugged_station_80) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

/*Unplug Panel station 80 */
#undef __FUNC__
#define __FUNC__	"UnplugPanelStation80"
static SElExceptionPtr UnplugPanelStation80
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;

	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_UNPLUGGED_STATION_80, &hevent));

	EXCCHECK( me->SetSensor(me, "CYLINDER_UNPLUG_STATION_80", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_PLUG_STATION_80", 0));
	
	gs_unglug_command_station_80 = 1;   /*Unplug flag station 80*/


	if (timeout)
	{
		if(gs_unplugged_station_80) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_unplugged_station_80) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}


 /* Lock Panel Left station 80 */
#undef __FUNC__
#define __FUNC__	"LockPanelStation80"
static SElExceptionPtr LockPanelStation80
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;
	int32_t			sensor_locked = FALSE;

	EXCCHECK( me->PlugPanelStation80(me, 5000));

	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_LOCKED_STATION_80, &hevent));

	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	if (microCam)
	{
	EXCCHECK( me->SetSensor(me, "CYLINDER_LOCK_STATION_80", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_UNLOCK_STATION_80", 0));
	}
	if (timeout)
	{
		if(gs_locked_station_80) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_locked_station_80) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

/* Unlock Panel station 80 */
#undef __FUNC__
#define __FUNC__	"UnlockPanelStation80"
static SElExceptionPtr UnlockPanelStation80
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;

	
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_UNLOCKED_STATION_80, &hevent));
	

	EXCCHECK( me->SetSensor(me, "CYLINDER_UNLOCK_STATION_80", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_LOCK_STATION_80", 0));
	if (timeout)
	{
		if(gs_unlocked_station_80) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_unlocked_station_80) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}
/* RemoveInsertKeyStation80 */
#undef __FUNC__
#define __FUNC__	"Tester::RemoveInsertKeyStation80"
static SElExceptionPtr RemoveInsertKeyStation80(struct _STester* me, int32_t timeout, bool_t wait_end , int32_t Mode)
{
	SElExceptionPtr		pexception = NULL;
	int32_t				error = 0;
	bool_t				is_hand_block = FALSE,
						is_hand_unblock = FALSE,
						is_cylinder_z_up = FALSE,
						is_cylinder_z_down = FALSE,
						is_cylinder_x_rear = FALSE,
						is_cylinder_x_front = FALSE,
						is_cylinder_y_rear = FALSE,
						is_cylinder_y_front = FALSE,
						is_panel_unlocked_Left = FALSE,
						is_panel_locked_Left = FALSE,
						is_panel_unlocked_Right = FALSE,
						is_panel_locked_Right = FALSE,
						presence_key = FALSE;
	int32_t				microCam;
	
	int32_t				step = INIT;
	bool_t				is_product_took = FALSE;
	bool_t				cylinder_x_front,
						cylinder_x_rear,
						cylinder_y_front,
						cylinder_y_rear,
						cylinder_z_up,
						cylinder_z_down,
						hand_block,
						hand_unblock;
	
	
	if (Mode==MODE_REMOVE_KEY)
 		EXCCHECK( me->ResetPickPlaceStation80(me, timeout, TRUE ));

      EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	  EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_80", &is_cylinder_z_up));
	  EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_80", &is_cylinder_z_down));
	  EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Y_REAR_STATION_80", &is_cylinder_y_rear));
	  EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Y_FRONT_STATION_80", &is_cylinder_y_front));
 	  EXCCHECK( me->GetSensor(me, "IS_PANEL_LOCKED_LEFT_STATION_80", &is_panel_locked_Left));
	  EXCCHECK( me->GetSensor(me, "IS_PANEL_UNLOCKED_LEFT_STATION_80", &is_panel_unlocked_Left));
	  EXCCHECK( me->GetSensor(me, "IS_PANEL_LOCKED_RIGHT_STATION_80", &is_panel_locked_Right));
	  EXCCHECK( me->GetSensor(me, "IS_PANEL_UNLOCKED_RIGHT_STATION_80", &is_panel_unlocked_Right));
	  EXCCHECK( me->GetSensor(me, "SENSOR_PRESENCE_KEY_STATION_80", &presence_key));

	if(microCam && 
	   is_cylinder_z_up && !is_cylinder_z_down &&
	   is_cylinder_y_rear && !is_cylinder_y_front &&
	   !presence_key)  //Verify if the key is absent
		 
	{
	TIMEOUT_INIT();
	step = INIT;
	
	while( TRUE)
	{
		if (!is_product_took)
		{
			switch  (step)
			{
			case  INIT :
								  
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_80", &cylinder_z_up));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_80", &cylinder_z_down));
					
					EXCCHECK( me->GetSensor(me, "SENSOR_BLOCK_STATION_80", &hand_block));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_UNBLOCK_STATION_80", &hand_unblock));
					EXCCHECK( me->GetSensor(me, "SENSOR_PRESENCE_KEY_STATION_80", &presence_key));

					if (Mode==MODE_REMOVE_KEY)
					{
							EXCCHECK( me->SetSensor(me, "CYLINDER_UNBLOCK_STATION_80", 1));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_BLOCK_STATION_80", 0));
					}
						
					if(!cylinder_z_up && cylinder_z_down) 
							{
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_UP_STATION_80", 1));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_DOWN_STATION_80", 0));
				 			}  
				   else if (cylinder_z_up && !cylinder_z_down && !presence_key) 
							{
							step = MOVE_TO_PALETT;		
				 			} 
			break;
			
			case  MOVE_TO_PALETT : 
				   		
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Y_FRONT_STATION_80", &cylinder_y_front));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Y_REAR_STATION_80", &cylinder_y_rear));
				   
				   if(!cylinder_y_front && cylinder_y_rear) 
							{
							EXCCHECK( me->SetSensor(me, "CYLINDER_Y_REAR_STATION_80", 0));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Y_FRONT_STATION_80", 1));
				 			}  
				   else if (cylinder_y_front && !cylinder_y_rear) 
							{
								if (Mode==MODE_REMOVE_KEY)
									step= MOVE_TO_PANEL ;
								else if (Mode == MODE_INSERT_KEY) 
									step= MOVE_TO_SECOND_POSITION ;								 		
							} 
			break;
			
			case  MOVE_TO_PANEL : 
				   		
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_80", &cylinder_z_up));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_80", &cylinder_z_down));
				   
				   if(!cylinder_z_down && cylinder_z_up) 
							{
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_DOWN_STATION_80", 1));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_UP_STATION_80", 0));
				 			}  
				   else if (cylinder_z_down && !cylinder_z_up) 
							{
								if (Mode==MODE_REMOVE_KEY)
									step= TAKE_ON_KEY ;
								else if (Mode == MODE_INSERT_KEY) 
									step= TAKE_OFF_KEY ;		
				 			} 
			  
				   
			break;											 
																
			case  TAKE_ON_KEY: 
				   		
	  				EXCCHECK( me->GetSensor(me, "SENSOR_BLOCK_STATION_80", &hand_block));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_UNBLOCK_STATION_80", &hand_unblock));

				   if(!hand_block && hand_unblock) 
							{
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_BLOCK_STATION_80", 1));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_UNBLOCK_STATION_80", 0));
				 			}  
				   else if(hand_block && !hand_unblock) 
				   			{
							Sleep(300);
							step= PICK_UP_KEY ;  
		
				 			} 
			break;
			
			case  MOVE_TO_SECOND_POSITION : 
				   		
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_FRONT_STATION_80", &cylinder_x_front));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_REAR_STATION_80", &cylinder_x_rear));
				   
				   if(!cylinder_x_front && cylinder_x_rear) 
							{
							EXCCHECK( me->SetSensor(me, "CYLINDER_X_REAR_STATION_80", 0));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_X_FRONT_STATION_80", 1));
				 			}  
				   else if (cylinder_x_front && !cylinder_x_rear) 
							{
								step = MOVE_TO_PANEL;	 		
							} 
			break;
			
			case  TAKE_OFF_KEY: 
				
	  				EXCCHECK( me->GetSensor(me, "SENSOR_BLOCK_STATION_80", &hand_block));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_UNBLOCK_STATION_80", &hand_unblock));
					
				   if(!hand_unblock && hand_block) 
							{
							EXCCHECK( me->SetSensor(me, "CYLINDER_UNBLOCK_STATION_80", 1));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_BLOCK_STATION_80", 0));
				 			}  
				   else if (hand_unblock && !hand_block)
				   			{
								step= PICK_UP_KEY ;  
							}
			break;
			
			case  PICK_UP_KEY : 
				   		
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_80", &cylinder_z_up));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_80", &cylinder_z_down));

				   
				   if(!cylinder_z_up && cylinder_z_down) 
							{
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_UP_STATION_80", 1));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_DOWN_STATION_80", 0));
				 			}  
				   else if (cylinder_z_up && !cylinder_z_down) 
							{
							 step = MOVE_TO_PRIMARY_POSITION;		
				 			} 
			break;
		
			case  MOVE_TO_PRIMARY_POSITION : 
				   		
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Y_FRONT_STATION_80", &cylinder_y_front));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Y_REAR_STATION_80", &cylinder_y_rear));

				   
				   if(!cylinder_y_rear && cylinder_y_front) 
							{
							EXCCHECK( me->SetSensor(me, "CYLINDER_Y_REAR_STATION_80", 1));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Y_FRONT_STATION_80", 0));
				 			}  
				   else if (cylinder_y_rear && !cylinder_y_front)
							{
							step = MOVE_TO_HOME;		
				 			} 
			break;
			
			
			case  MOVE_TO_HOME : 
				   		
				   	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_FRONT_STATION_80", &cylinder_x_front));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_REAR_STATION_80", &cylinder_x_rear));
					EXCCHECK( me->GetSensor(me, "SENSOR_PRESENCE_KEY_STATION_80", &presence_key));

				   if(cylinder_x_front && !cylinder_x_rear) 
							{
							EXCCHECK( me->SetSensor(me, "CYLINDER_X_REAR_STATION_80", 1));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_X_FRONT_STATION_80", 0));
				 			}  
				   else if ((!cylinder_x_front && cylinder_x_rear) && 
					   		((presence_key && Mode==MODE_INSERT_KEY)|| (Mode==MODE_REMOVE_KEY)))  //There's only a sensor in the customer position 
							{
								step = REMOVE_PLACE_FINIHED;
								
							} 
			break;

			case  REMOVE_PLACE_FINIHED :
				 is_product_took = TRUE;
			break;
			}
		}

		else break;
	
		TIMEOUT_CHECK( timeout);
	}
	}

	else EXCTHROW( TESTER_ERROR_TIMEOUT, "Conditions pour Insertion/Enlevement Clé Rouge sont pas pret");
	

Error:
	EXCRETHROW(pexception);
}

/* PickPlace station 80 */
#undef __FUNC__
#define __FUNC__	"Tester::ResetPickPlaceStation80"
static SElExceptionPtr ResetPickPlaceStation80(struct _STester* me, int32_t timeout, bool_t wait_end)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	uint32_t		step = TAKE_OFF_KEY;
	bool_t			is_product_took = FALSE;
	bool_t			cylinder_x_front,
					cylinder_x_rear,
					cylinder_y_front,
					cylinder_y_rear,
					cylinder_z_up,
					cylinder_z_down,
					hand_block,
					hand_unblock;

	TIMEOUT_INIT();
	
	while( TRUE)
	{
		if (!is_product_took)
		{
			switch  (step)
			{
			case  TAKE_OFF_KEY: 
				
					EXCCHECK( me->SetSensor(me, "CYLINDER_UNBLOCK_STATION_80", 1));
				   	EXCCHECK( me->SetSensor(me, "CYLINDER_BLOCK_STATION_80", 0));
					
	  				EXCCHECK( me->GetSensor(me, "SENSOR_BLOCK_STATION_80", &hand_block));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_UNBLOCK_STATION_80", &hand_unblock));

				    if(hand_unblock && !hand_block) 
				   			{
							step = PICK_UP_KEY;		
				 			}
			break;
			
			case  PICK_UP_KEY : 
				   	
		   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_UP_STATION_80", 1));
				  	EXCCHECK( me->SetSensor(me, "CYLINDER_Z_DOWN_STATION_80", 0));
					
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_80", &cylinder_z_up));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_80", &cylinder_z_down));
				   
				   if((!cylinder_z_up && cylinder_z_down) || (!cylinder_z_up && !cylinder_z_down))
							{
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_UP_STATION_80", 1));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_DOWN_STATION_80", 0));
				 			}  
				   else if (cylinder_z_up && !cylinder_z_down) 
							{
							step = MOVE_TO_HOME;		
				 			} 
			break;
			
			case  MOVE_TO_HOME : 
				   		
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Y_FRONT_STATION_80", &cylinder_y_front));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Y_REAR_STATION_80", &cylinder_y_rear));
				   
				   if( (!cylinder_y_rear && !cylinder_y_front) || (!cylinder_y_rear && cylinder_y_front)) 
							{
							EXCCHECK( me->SetSensor(me, "CYLINDER_Y_REAR_STATION_80", 1));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Y_FRONT_STATION_80", 0));
				 			}  
				   else if (cylinder_y_rear && !cylinder_y_front) 
							{
							step  = MOVE_TO_PRIMARY_POSITION;		
				 			} 
			break;
			
			case  MOVE_TO_PRIMARY_POSITION : 
				   		
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_FRONT_STATION_80", &cylinder_x_front));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_REAR_STATION_80", &cylinder_x_rear));
				   
				   if( (!cylinder_x_rear && !cylinder_x_front) || (!cylinder_x_rear && cylinder_x_front)) 
							{
							EXCCHECK( me->SetSensor(me, "CYLINDER_X_REAR_STATION_80", 1));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_X_FRONT_STATION_80", 0));
				 			}  
				   else if (cylinder_x_rear && !cylinder_x_front) 
							{
							step  = REMOVE_PLACE_FINIHED; 		
				 			} 
			break;
			
			case  REMOVE_PLACE_FINIHED :
				
				 is_product_took = TRUE;
				 
			break;
				
			}
		}

		else break;
		
		TIMEOUT_CHECK( timeout);
	}

Error:
	EXCRETHROW(pexception);
}

/*MoveCylinderRotationUp_Station80 */
#undef __FUNC__
#define __FUNC__	"MoveCylinderRotationUpStation80"
static SElExceptionPtr MoveCylinderRotationUpStation80
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;

	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_CYLYNDER_ROTATION_UP_STATION_80, &hevent));

	EXCCHECK( me->SetSensor(me, "CYLINDER_ROTATION_DOWN_STATION_80", 1));
	Sleep(TIME_AIR);
	EXCCHECK( me->SetSensor(me, "CYLINDER_ROTATION_UP_STATION_80", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_ROTATION_DOWN_STATION_80", 0));

	if (timeout)
	{
		if(gs_cylinder_rotation_is_up_80) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_cylinder_rotation_is_up_80) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}

/*MoveCylinderRotationDownStation80 */
#undef __FUNC__
#define __FUNC__	"MoveCylinderRotationDownStation80"
static SElExceptionPtr MoveCylinderRotationDownStation80
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;
	
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_CYLYNDER_ROTATION_DOWN_STATION_80, &hevent));

	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	if (microCam)
	{
	EXCCHECK( me->SetSensor(me, "CYLINDER_ROTATION_UP_STATION_80", 0));
	EXCCHECK( me->SetSensor(me, "CYLINDER_ROTATION_DOWN_STATION_80", 1));
	}
	if (timeout)
	{
		if(gs_cylinder_rotation_is_down_station_80) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_cylinder_rotation_is_down_station_80) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}			  

/*MoveCylinderSystemLevDownStation80 */
#undef __FUNC__
#define __FUNC__	"MoveCylinderSystemLevDownStation80"
static SElExceptionPtr MoveCylinderSystemLevDownStation80
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;
	double			position_axis;
   	SaxisPtr		paxis = NULL; 

	
	EXCCHECK( me->axissAPI->GetaxisByIndex(me->axissAPI, 0, &paxis));
	EXCCHECK( paxis->CurrentPosition(paxis, AXIS_X, &position_axis));
	if (gs_locked_station_80)
	{
	if(position_axis < 51.0 && position_axis > 49.0)
	{
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_CYLYNDER_SYSTEM_LEV_DOWN_STATION_80, &hevent));

	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	if (microCam)
	{
	EXCCHECK( me->SetSensor(me, "CYLINDER_SYSTEM_LEV_UP_STATION_80", 0));
	EXCCHECK( me->SetSensor(me, "CYLINDER_SYSTEM_LEV_DOWN_STATION_80", 1));
	}
	if (timeout)
	{
		if(gs_cylinder_system_lev_is_down_80) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_cylinder_system_lev_is_down_80) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	}
	else 
		EXCTHROW( TESTER_ERROR_TIMEOUT, "MoveCylinderSystemLevDownStation80 : Error Position axis");
	}
	else 
		EXCTHROW( TESTER_ERROR_TIMEOUT, "MoveCylinderSystemLevDownStation80 : Error cylinder unlocked");
	
	
	
Error:
	EXCRETHROW(pexception);
}			  


/*MoveCylinderSystemLevUpStation80 */
#undef __FUNC__
#define __FUNC__	"MoveCylinderSystemLevUpStation80"
static SElExceptionPtr MoveCylinderSystemLevUpStation80
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	double			position_axis;
   	SaxisPtr		paxis = NULL; 
	

	EXCCHECK( me->axissAPI->GetaxisByIndex(me->axissAPI, 0, &paxis));
	EXCCHECK( paxis->SetPosition(paxis, AXIS_X, 50.0, 100.0));
	
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_CYLYNDER_SYSTEM_LEV_UP_STATION_80, &hevent));

	EXCCHECK( me->SetSensor(me, "CYLINDER_SYSTEM_LEV_DOWN_STATION_80", 1));
	Sleep(TIME_AIR);
	EXCCHECK( me->SetSensor(me, "CYLINDER_SYSTEM_LEV_UP_STATION_80", 1));
	EXCCHECK( me->SetSensor(me, "CYLINDER_SYSTEM_LEV_DOWN_STATION_80", 0));

	if (timeout)
	{
		if(gs_cylinder_system_lev_is_up_80) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_cylinder_system_lev_is_up_80) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}			  

/*Test Checker station 70 */
#undef __FUNC__
#define __FUNC__	"TestChecker_Station80"
static SElExceptionPtr TestChecker_Station80
(
	struct _STester* me,
	int32_t			*output0,
	int32_t			*output1,
	int32_t			*output2,
	int32_t			*output3


)
{
	SElExceptionPtr	pexception = NULL;
	*output0=-1;
	*output1=-1;
	*output2=-1;
	*output3=-1;

	EXCCHECK( me->SetSensor(me, "KEYENCE_OUT1", 1));
	Sleep(200);
	EXCCHECK( me->SetSensor(me, "KEYENCE_OUT1", 0));
	Sleep(500);
   	EXCCHECK( me->GetSensor(me, "KEYENCE_INPUT1", output0));
   	EXCCHECK( me->GetSensor(me, "KEYENCE_INPUT2", output1));
   	EXCCHECK( me->GetSensor(me, "KEYENCE_INPUT3", output2));
   	EXCCHECK( me->GetSensor(me, "KEYENCE_INPUT4", output3));

Error:
	me->SetSensor(me, "KEYENCE_OUT1", 0);
	EXCRETHROW(pexception);
}

/* Initialize Stations */
#undef __FUNC__
#define __FUNC__	"InitializeStations"
static SElExceptionPtr InitializeStations
(
	struct _STester* me,
	int32_t			station,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	HANDLE			hevent = NULL;
	SaxisPtr		paxis = NULL; 
	bool_t			presence_key;
	bool_t			sensor_Pos_0, hand_unblock;
	
	
	switch (station)
	{
	case STATION_30 :
	EXCCHECK( me->MoveCylinderAirbagUpStation30(me, timeout));
	EXCCHECK( me->MoveCylinderCentrageRearStation30(me, timeout));
	EXCCHECK( me->LockPanelStation30(me, timeout));
	break;
	
	case STATION_60 :
	EXCCHECK( me->GetSensor(me, "SENSOR_UNBLOCK_STATION_60", &hand_unblock)); //chech if exist key
	EXCCHECK( me->GetSensor(me, "SENSOR_MOTOR_POS_0_STATION_60", &sensor_Pos_0));
	if (sensor_Pos_0 && !hand_unblock)
	EXCCHECK( me->RemoveInsertKeyStation60(me, timeout, TRUE , MODE_INSERT_KEY)); 
	else 
	EXCCHECK( me->ResetPickPlaceStation60(me, timeout, TRUE)); 
	
	EXCCHECK( me->MoveCylinderRotationUpStation60(me, timeout));
	EXCCHECK( me->MoveCylinderCentrageRearStation60(me, timeout));
	EXCCHECK( me->LockPanelStation60(me, timeout));
	break;

	case STATION_70 :
	//EXCCHECK( me->LockPanelStation70(me, timeout)); TEMPO
	EXCCHECK( me->RobotHomeStation70(me, timeout));
	EXCCHECK( me->IlluminationStation70(me, 0));
	break;

	case STATION_80 :
	EXCCHECK( me->ResetPickPlaceStation80(me, timeout, TRUE));
	EXCCHECK( me->MoveCylinderRotationUpStation80(me, timeout));
	EXCCHECK( me->MoveCylinderSystemLevUpStation80(me, timeout));
	EXCCHECK( me->LockPanelStation80(me, timeout));
	break;
	}
	
Error:
	EXCDISPLAY(pexception);
	return 0;
}

/* Wait for table rotation */
#undef __FUNC__
#define __FUNC__	"WaitRotationFinished"
static SElExceptionPtr WaitRotationFinished
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	HANDLE			hevent = NULL;
	HANDLE			hevent2 = NULL;
	bool_t   		microCam;

		EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_TESTER_ROTATION_FINISHED, &hevent));
		EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_PLUGGED_STATION_10, &hevent2));
		
		if (timeout)
		{
			if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
			{
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
			}
		}
		if (timeout)
		{
			if (WaitForSingleObject( hevent2, timeout) == WAIT_TIMEOUT)
			{
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
			}
		}

Error:
	EXCRETHROW(pexception);
}

/* Get barcode indentification of panel */
#undef __FUNC__
#define __FUNC__	"GetItemCode"
static SElExceptionPtr GetItemCode
(
	struct _STester* me,
	const char*		product,
	int32_t			index,
	long			bufferSize,
	char*			buffer,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			barcode_event = 0;
	SBCRPtr	last_bcr = NULL;

	if (me->pBarCodeReader->reader)
	{
		last_bcr = me->pBarCodeReader + (index);
#if 0    /* formerly excluded lines */
		EXCCHECK( (last_bcr->reader)->StartReading(last_bcr->reader));
		Sleep(800);
#endif   /* formerly excluded lines */
		EXCCHECK( (last_bcr->reader)->GetBarCode(last_bcr->reader, buffer, bufferSize));
#if 0    /* formerly excluded lines */
		Sleep(100);
		EXCCHECK( (last_bcr->reader)->StopReading(last_bcr->reader));
#endif   /* formerly excluded lines */
	}

Error:
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"CallbackFncAbis"
static SElExceptionPtr CallbackFncAbis(const char* name, uint32_t state, void* pData)
{
	SElExceptionPtr	pexception = NULL;
	STesterPtr		me = (STesterPtr)pData;

	if( state==1 && me->eventAPI)
	{
		EXCCHECK( me->SetSensor(me, "VEROUILLAGE_ABIS", 1));

	}
	else if( state==0 && me->eventAPI)
	{
		EXCCHECK( me->SetSensor(me, "VEROUILLAGE_ABIS", 0));

	}

Error:
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"CallbackFncMissionRunning"
static SElExceptionPtr CallbackFncMissionRunning(const char* name, uint32_t state, void* pData)
{
	SElExceptionPtr	pexception = NULL;
	STesterPtr		me = (STesterPtr)pData;

	if( state==1 && me->eventAPI)
	{
		//printf("CallbackFncMissionRunning : gs_mission_running_station_70 = TRUE\r\n");
		gs_mission_running_station_70 = TRUE;
	}

Error:
	EXCRETHROW(pexception);
}

