#if !defined(__TESTER_H__)
#define __TESTER_H__

#include <eltypes.h>
#include <exception/Exception.h>
#include <windows.h>
#include <sensors/sensors.h>
#include <axis/class/axis.h>
#include <eventfactory/EventFactory.h>
#include <tester/barcodereader/BarCodeReader.h>

#if defined(__cplusplus) || defined(__cplusplus__)
	extern "C" {
#endif

#define EVNT_TESTER_BUTTON_PRESSED_OK						"EVNT_TESTER_BUTTON_PRESSED_OK"
#define EVNT_TESTER_BUTTON_PRESSED_NOK						"EVNT_TESTER_BUTTON_PRESSED_NOK"
#define EVNT_TESTER_BUTTON_PRESSED_MENU						"EVNT_TESTER_BUTTON_PRESSED_MENU"
#define EVNT_TESTER_BUTTON_PRESSED_LABEL					"EVNT_TESTER_BUTTON_PRESSED_LABEL"
#define EVNT_TESTER_BUTTON_PRESSED_DOWNTIME					"EVNT_TESTER_BUTTON_PRESSED_DOWNTIME"
#define EVNT_CONTINUE										"EVNT_CONTINUE"
#define EVNT_TESTER_ROTATION_FINISHED						"EVNT_TESTER_ROTATION_FINISHED"
#define EVNT_TESTER_CONTINUE_ROTATION						"EVNT_TESTER_CONTINUE_ROTATION"
#define EVNT_JIG_CYCLE_START								"EVNT_JIG_CYCLE_START"
#define EVNT_JIG_NEW										"EVNT_JIG_NEW"

#define EVNT_JIG_ITEM_INSERTED								"EVNT_JIG_ITEM_INSERTED"
#define EVNT_JIG_ITEM_RELEASED								"EVNT_JIG_ITEM_RELEASED"
#define EVNT_JIG_ITEM_PLUGGED_STATION_10					"EVNT_JIG_ITEM_PLUGGED_STATION_10"
#define EVNT_JIG_ITEM_UNPLUGGED_STATION_10					"EVNT_JIG_ITEM_UNPLUGGED_STATION_10"
#define EVNT_JIG_ITEM_LOCKED_STATION_10						"EVNT_JIG_ITEM_LOCKED_STATION_10"
#define EVNT_JIG_ITEM_UNLOCKED_STATION_10					"EVNT_JIG_ITEM_UNLOCKED_STATION_10"
		
#define EVNT_JIG_ITEM_PLUGGED_STATION_30					"EVNT_JIG_ITEM_PLUGGED_STATION_30"
#define EVNT_JIG_ITEM_UNPLUGGED_STATION_30					"EVNT_JIG_ITEM_UNPLUGGED_STATION_30"
#define	EVNT_JIG_ITEM_VERTICAL_PLUGGED_STATION_30           "EVNT_JIG_ITEM_VERTICAL_PLUGGED_STATION_30"
#define	EVNT_JIG_ITEM_VERTICAL_UNPLUGGED_STATION_30         "EVNT_JIG_ITEM_VERTICAL_UNPLUGGED_STATION_30"
#define EVNT_JIG_ITEM_PLUGGED_STATION_40					"EVNT_JIG_ITEM_PLUGGED_STATION_40"
#define EVNT_JIG_ITEM_UNPLUGGED_STATION_40					"EVNT_JIG_ITEM_UNPLUGGED_STATION_40"
#define EVNT_JIG_ITEM_PLUGGED_STATION_50		            "EVNT_JIG_ITEM_PLUGGED_STATION_50"
#define EVNT_JIG_ITEM_UNPLUGGED_STATION_50					"EVNT_JIG_ITEM_UNPLUGGED_STATION_50"
#define EVNT_JIG_ITEM_LOCKED_STATION_30						"EVNT_JIG_ITEM_LOCKED_STATION_30"
#define EVNT_JIG_ITEM_UNLOCKED_STATION_30					"EVNT_JIG_ITEM_UNLOCKED_STATION_30"
#define EVNT_CYLINDER_GRIPPER_CENTRAGE_BLOCK_STATION_30		"EVNT_CYLINDER_GRIPPER_CENTRAGE_BLOCK_STATION_30"
#define EVNT_CYLINDER_GRIPPER_CENTRAGE_UNBLOCK_STATION_30	"EVNT_CYLINDER_GRIPPER_CENTRAGE_UNBLOCK_STATION_30"
#define EVNT_CYLINDER_CENTRAGE_FRONT_STATION_30  			"EVNT_CYLINDER_CENTRAGE_FRONT_STATION_30"
#define EVNT_CYLINDER_CENTRAGE_REAR_STATION_30  			"EVNT_CYLINDER_CENTRAGE_REAR_STATION_30"
#define EVNT_CYLINDER_AIRBAG_UP_STATION_30  				"EVNT_CYLINDER_AIRBAG_UP_STATION_30"
#define EVNT_CYLINDER_AIRBAG_DOWN_STATION_30   				"EVNT_CYLINDER_AIRBAG_DOWN_STATION_30"

//#define EVNT_JIG_ITEM_PLUGGED_STATION_60					"EVNT_JIG_ITEM_PLUGGED_STATION_60"
//#define EVNT_JIG_ITEM_UNPLUGGED_STATION_60					"EVNT_JIG_ITEM_UNPLUGGED_STATION_60"
//#define EVNT_CYLINDER_GRIPPER_CENTRAGE_BLOCK_STATION_60		"EVNT_CYLINDER_GRIPPER_CENTRAGE_BLOCK_STATION_60"
//#define EVNT_CYLINDER_GRIPPER_CENTRAGE_UNBLOCK_STATION_60	"EVNT_CYLINDER_GRIPPER_CENTRAGE_UNBLOCK_STATION_60"
//#define EVNT_CYLINDER_CENTRAGE_FRONT_STATION_60  			"EVNT_CYLINDER_CENTRAGE_FRONT_STATION_60"
//#define EVNT_CYLINDER_CENTRAGE_REAR_STATION_60  			"EVNT_CYLINDER_CENTRAGE_REAR_STATION_60"
//#define EVNT_JIG_ITEM_CYLYNDER_ROTATION_UP_STATION_60  		"EVNT_JIG_ITEM_CYLYNDER_ROTATION_UP_STATION_60"
//#define EVNT_JIG_ITEM_CYLYNDER_ROTATION_DOWN_STATION_60   	"EVNT_JIG_ITEM_CYLYNDER_ROTATION_DOWN_STATION_60"
#define EVNT_JIG_ITEM_OPEN_GRIPPER					     	"EVNT_JIG_ITEM_OPEN_GRIPPER"
#define EVNT_JIG_ITEM_CLOSE_GRIPPER					        "EVNT_JIG_ITEM_CLOSE_GRIPPER"
#define EVNT_JIG_ITEM_ACTUATOR_X_REAR					    "EVNT_JIG_ITEM_ACTUATOR_X_REAR"
#define EVNT_JIG_ITEM_ACTUATOR_X_FRONT					    "EVNT_JIG_ITEM_ACTUATOR_X_FRONT"	
#define EVNT_JIG_ITEM_ACTUATOR_Z_UP					        "EVNT_JIG_ITEM_ACTUATOR_Z_UP"
#define EVNT_JIG_ITEM_ACTUATOR_Z_DOWN					    "EVNT_JIG_ITEM_ACTUATOR_Z_DOWN"

#define EVNT_JIG_ITEM_PLUGGED_STATION_20					"EVNT_JIG_ITEM_PLUGGED_STATION_20"
#define EVNT_JIG_ITEM_UNPLUGGED_STATION_20					"EVNT_JIG_ITEM_UNPLUGGED_STATION_20"
		
#define EVNT_JIG_ITEM_PLUGGED_STATION_80					"EVNT_JIG_ITEM_PLUGGED_STATION_80"
#define EVNT_JIG_ITEM_UNPLUGGED_STATION_80					"EVNT_JIG_ITEM_UNPLUGGED_STATION_80"		
#define EVNT_JIG_ITEM_LOCKED_STATION_80						"EVNT_JIG_ITEM_LOCKED_STATION_80"
#define EVNT_JIG_ITEM_UNLOCKED_STATION_80					"EVNT_JIG_ITEM_UNLOCKED_STATION_80"

#define EVNT_JIG_ITEM_CYLYNDER_ROTATION_UP_STATION_80  		"EVNT_JIG_ITEM_CYLYNDER_ROTATION_UP_STATION_80"
#define EVNT_JIG_ITEM_CYLYNDER_ROTATION_DOWN_STATION_80   	"EVNT_JIG_ITEM_CYLYNDER_ROTATION_DOWN_STATION_80"
#define EVNT_JIG_ITEM_CYLYNDER_SYSTEM_LEV_UP_STATION_80  	"EVNT_JIG_ITEM_CYLYNDER_SYSTEM_LEV_UP_STATION_80"
#define EVNT_JIG_ITEM_CYLYNDER_SYSTEM_LEV_DOWN_STATION_80   "EVNT_JIG_ITEM_CYLYNDER_SYSTEM_LEV_DOWN_STATION_80"	
		
#define EVNT_JIG_ITEM_PRINTER_FRONT   						"EVNT_JIG_ITEM_PRINTER_FRONT"	
#define EVNT_JIG_ITEM_PRINTER_REAR   						"EVNT_JIG_ITEM_PRINTER_REAR"	

#define EVNT_RED_BOX_EMPTY									"EVNT_RED_BOX_EMPTY"
#define EVNT_JIG_START										"EVNT_JIG_START"
		

			
		
typedef struct _SBCR
{
	SBarCodeReaderPtr	reader;
	SElExceptionPtr		(*bcr_new)( SBarCodeReaderPtr* pBcr);
	SElExceptionPtr		(*bcr_delete)( SBarCodeReaderPtr* pBcr);
	HANDLE				dllHandle;
} SBCR, *SBCRPtr;
		
typedef struct _STester
{
	SElExceptionPtr (*Initialize)(struct _STester*	me, const char*	fileName);
	SElExceptionPtr (*RegisterSensorCallbacks)( struct _STester* me);
	
	SElExceptionPtr (*TableRotate)(struct _STester* me, int32_t timeout, bool_t wait_end);
	SElExceptionPtr (*GetStationStatus)(struct _STester* me, char status[9]);
	SElExceptionPtr (*WaitRotationFinished)(struct _STester* me, int32_t timeout);
	SElExceptionPtr (*OkNokCheck)(struct _STester* me, int32_t timeout, bool_t* ok);
	SElExceptionPtr (*NokCheck)(struct _STester* me, int32_t timeout);
	SElExceptionPtr (*InitializeStations)(struct _STester* me, int32_t station, int32_t timeout);
	SElExceptionPtr (*SetSensor)(struct _STester* me, const char* sensorId, uint32_t value);
	SElExceptionPtr (*GetSensor)(struct _STester* me, const char* sensorId, uint32_t* value);
	SElExceptionPtr (*GetSensorAI)(struct _STester* me, const char* sensorId, uint32_t* value);
	SElExceptionPtr (*SetSensorCallback)(struct _STester* me, const char* sensorId, SensorCallbackFnc_t fCall, void* pData);
	SElExceptionPtr (*GetItemCode)(struct _STester* me, const char* product, int32_t index, long bufferSize, char* buffer,int32_t timeout);
	
	
	SElExceptionPtr (*LockPanelStation10)(struct _STester* me, int32_t timeout);
	SElExceptionPtr (*UnlockPanelStation10)(struct _STester* me, int32_t timeout);
	SElExceptionPtr (*PlugPanelStation20)(struct _STester* me, int32_t timeout);
	SElExceptionPtr (*UnPlugPanelStation20)(struct _STester* me, int32_t timeout);
	SElExceptionPtr (*PlugPanelStation30)(struct _STester* me, int32_t timeout);
	SElExceptionPtr (*UnplugPanelStation30)(struct _STester* me, int32_t timeout);
	SElExceptionPtr (*VerticalPlugStation30)(struct _STester* me, int32_t timeout);
    SElExceptionPtr (*VerticalUnplugStation30)(struct _STester* me, int32_t timeout);
	SElExceptionPtr (*PlugPanelStation40)(struct _STester* me, int32_t timeout);
	SElExceptionPtr (*UnplugPanelStation40)(struct _STester* me, int32_t timeout);
	SElExceptionPtr (*PlugPanelStation50)(struct _STester* me, int32_t timeout);
	SElExceptionPtr (*UnplugPanelStation50)(struct _STester* me, int32_t timeout);
	SElExceptionPtr (*ReadyPrint)(struct _STester* me, int32_t timeout);
	SElExceptionPtr (*CyclePrint)(struct _STester* me, int32_t timeout);
	SElExceptionPtr (*OPEN_GRIPPER)(struct _STester* me, int32_t timeout);
	SElExceptionPtr (*CLOSE_GRIPPER)(struct _STester* me, int32_t timeout);
	SElExceptionPtr (*ACTUATOR_X_REAR)(struct _STester* me, int32_t timeout);
	SElExceptionPtr (*ACTUATOR_X_FRONT)(struct _STester* me, int32_t timeout);
	SElExceptionPtr (*ACTUATOR_Z_UP)(struct _STester* me, int32_t timeout);
	SElExceptionPtr (*ACTUATOR_Z_DOWN)(struct _STester* me, int32_t timeout);
	SElExceptionPtr (*PickPlacePanelStation60)(struct _STester* me, int32_t timeout);
	SElExceptionPtr (*HomePickPlacePanelStation60)(struct _STester* me, int32_t timeout);
	SElExceptionPtr (*PickPrintPanelStation60)(struct _STester* me, int32_t timeout);
    SElExceptionPtr (*ResetPickPrintPanelStation60)(struct _STester* me, int32_t timeout);
/*******************************************
 * PRIVATE
 *******************************************/
	SDIOSensorsPtr		sensorAPI;
	SaxisPtr			paxis;
	SaxissPtr			axissAPI;
	SElEventFactoryPtr	eventAPI;
	void*				pLogicalNames;
	int32_t				LogicalCounts;
	char				path[512]; /* xml config path */
	HANDLE				hThread;
	HANDLE				hEventStop;
	void*				LogApi;
	uint32_t			LogChannel;
	SBCRPtr				pBarCodeReader;
	int32_t				BCRCounts;
	
} STester, *STesterPtr;

SElExceptionPtr	tester_new(STesterPtr* pTester);
SElExceptionPtr	tester_delete(STesterPtr* pTester);
SElExceptionPtr JigThread(struct _STester* me);
SElExceptionPtr TableThread(struct _STester* me);

#define TESTER_ERROR_INVALID_PARAMETER		-10001
#define TESTER_ERROR_TIMEOUT				-10002
#define TESTER_ERROR_STILL_ACTIVE			-10003
#define TESTER_ERROR_WAIT_TIMEOUT			-10004
#define TESTER_ERROR_LOAD_LIBRARY			-10005
#define TESTER_ERROR_PICK_PLACE				-10006
#define TESTER_LASER_ERROR					-10007




#define ERROR_PUSH_1						-10010
#define ERROR_PUSH_2						-10011
#define ERROR_PUSH_3						-10012
#define ERROR_PUSH_4						-10013
#define ERROR_PUSH_5						-10014
#define ERROR_PUSH_6						-10015
#define ERROR_PUSH_7						-10016
#define ERROR_PUSH_8						-10017


#if defined(__cplusplus) || defined(__cplusplus__)
	}
#endif

#endif /* __TESTER_H__*/ 
