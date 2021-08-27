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
#include "station10.h"
#include "station20.h"
#include "station30.h"
#include "station40.h"
#include "station50.h"
#include "station60.h"
#include "rs232.h"
CRITICAL_SECTION   _Lock;


#define MAX_LENGTH_SENSOR_LOGICAL_NAME				64

#define TIMEOUT_INIT() 	clock_t	_start_clock = clock();

#define TIMEOUT_CHECK(timeout) \
	if ( timeout>0 && ((clock() - _start_clock) / (double)CLOCKS_PER_SEC) > (timeout/1000.0) ) { EXCTHROW( TESTER_ERROR_TIMEOUT, "Timeout"); } else


#define TIMEOUT_CHECK_START(timeout) \
	if ( timeout>0 && ((clock() - _start_clock) / (double)CLOCKS_PER_SEC) > (timeout/1000.0) ) { EXCTHROW( TESTER_ERROR_TIMEOUT, "Timeout START MISSION"); } else


#define TIMEOUT_CHECK_END(timeout) \
	if ( timeout>0 && ((clock() - _start_clock) / (double)CLOCKS_PER_SEC) > (timeout/1000.0) ) { EXCTHROW( TESTER_ERROR_TIMEOUT, "Timeout END MISSION"); } else



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
	STATION_20=0,
	STATION_30,
	STATION_40,
	STATION_50,
	STATION_60,
	STATION_10,
} EStation;



static int32_t	gs_inserted = 0;
static int32_t	gs_released = 0;

static int32_t	gs_table_started_rotation = -1;
static int32_t	gs_table_finished_rotation = -1;

char Table_Ready[NUMBER_OF_STATIONS+1];

typedef enum _EStep
{
	CHECK_TABLE_POITION = 0,
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
static SElExceptionPtr GetSensorAI(struct _STester* me, const char* sensorId, uint32_t* value);
static SElExceptionPtr SetSensorCallback(struct _STester* me, const char* sensorId, SensorCallbackFnc_t fCall, void* pData);
static SElExceptionPtr CallbackFncButtonOk(const char* name, uint32_t state, void* pData);
static SElExceptionPtr CallbackFncButtonNok(const char* name, uint32_t state, void* pData);
static SElExceptionPtr CallbackFncButtonMenu(const char* name, uint32_t state, void* pData);
static SElExceptionPtr CallbackFncButtonLabel(const char* name, uint32_t state, void* pData);
static SElExceptionPtr CallbackFncButtonDowntime(const char* name, uint32_t state, void* pData);
static SElExceptionPtr CallbackFncMissionRunning(const char* name, uint32_t state, void* pData);
static SElExceptionPtr CallbackFncButtonStart(const char* name, uint32_t state, void* pData);
static SElExceptionPtr CallbackFncJigNew(const char* name, uint32_t state, void* pData);
static SElExceptionPtr CallbackFncRotation(const char* name, uint32_t state, void* pData);
static SElExceptionPtr CallbackFncRotationStarted(const char* name, uint32_t state, void* pData);
static SElExceptionPtr CallbackFncButtonTotalStop(const char* name, uint32_t state, void* pData);
static SElExceptionPtr CallbackFncRedBoxEmpty(const char* name, uint32_t state, void* pData);
static SElExceptionPtr InitializeStations(struct _STester* me, int32_t station, int32_t timeout);
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
	(*pTester)->GetSensorAI = GetSensorAI;
	(*pTester)->TableRotate = TableRotate;
	(*pTester)->GetStationStatus = GetStationStatus;
	(*pTester)->InitializeStations = InitializeStations;
	(*pTester)->WaitRotationFinished = WaitRotationFinished;
	(*pTester)->OkNokCheck = OkNokCheck;
	(*pTester)->NokCheck = NokCheck;
	(*pTester)->RegisterSensorCallbacks = RegisterSensorCallbacks;
	(*pTester)->SetSensorCallback = SetSensorCallback;

	/* Station 10 */
	//(*pTester)->LockPanelStation10 = LockPanelStation10;
	//(*pTester)->UnlockPanelStation10 = UnlockPanelStation10;

	/* Station 20 */
	(*pTester)->PlugPanelStation20 = PlugPanelStation20;
	(*pTester)->UnPlugPanelStation20 = UnPlugPanelStation20;

	/* Station 30 */
	(*pTester)->PlugPanelStation30 = PlugPanelStation30;
	(*pTester)->UnplugPanelStation30 = UnplugPanelStation30;

	(*pTester)->VerticalPlugStation30 =VerticalPlugStation30;//Tastatore actuatore
	(*pTester)->VerticalUnplugStation30 = VerticalUnplugStation30;

	/* Station 40 */
	(*pTester)->PlugPanelStation40 = PlugPanelStation40;
	(*pTester)->UnplugPanelStation40 = UnplugPanelStation40;

	/* Station 50 */
	(*pTester)->PlugPanelStation50 = PlugPanelStation50;
	(*pTester)->UnplugPanelStation50 = UnplugPanelStation50;
	(*pTester)->ReadyPrint = ReadyPrint;
	(*pTester)->CyclePrint = CyclePrint;

	/* Station 60 */
	(*pTester)->CLOSE_GRIPPER = CLOSE_GRIPPER;
	(*pTester)->OPEN_GRIPPER = OPEN_GRIPPER;
	(*pTester)->ACTUATOR_X_REAR = ACTUATOR_X_REAR;
	(*pTester)->ACTUATOR_X_FRONT = ACTUATOR_X_FRONT;
	(*pTester)->ACTUATOR_Z_UP = ACTUATOR_Z_UP;
	(*pTester)->ACTUATOR_Z_DOWN = ACTUATOR_Z_DOWN;
	
	
	
	(*pTester)->PickPlacePanelStation60 = PickPlacePanelStation60;
	(*pTester)->HomePickPlacePanelStation60 = HomePickPlacePanelStation60;

	(*pTester)->PickPrintPanelStation60 = PickPrintPanelStation60;
	(*pTester)->ResetPickPrintPanelStation60 = ResetPickPrintPanelStation60;
	
	(*pTester)->GetItemCode = GetItemCode;//read barcode WS60



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
	//EXCCHECK( me->SetSensorCallback(me, "INFRARED_BARRIER", CallbackFncButtonStart, me));

	EXCCHECK( me->SetSensorCallback(me, "MICRO_CAM", CallbackFncRotation, me));
	EXCCHECK( me->SetSensorCallback(me, "BUTTON_TOTAL_STOP", CallbackFncButtonTotalStop, me));
	//EXCCHECK( me->SetSensorCallback(me, "MISSION_RUNNING", CallbackFncMissionRunning, me));
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

		CA_FreeMemory(alias_name);
		alias_name = NULL;
		CA_FreeMemory(logical_name);
		logical_name = NULL;
		CA_DiscardObjHandle(sensor);
		sensor = 0;

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

			CA_FreeMemory(alias_name);
			alias_name = NULL;
			CA_FreeMemory(logical_name);
			logical_name = NULL;
			CA_DiscardObjHandle(sensor);
			sensor = 0;

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

				CA_FreeMemory(dll_name_str);
				dll_name_str = NULL;
				CA_FreeMemory(config_xml);
				config_xml = NULL;
				CA_DiscardObjHandle(dll_name);
				dll_name = 0;
			}
			CA_DiscardObjHandle(barcode);
			barcode = 0;
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
	int             Confirm;

	bool_t			status_barrier = FALSE,
					status_micro_cam = FALSE,
					is_table_rotation_finished = FALSE;

	char			is_station_10_ready = FALSE,
					is_station_20_ready = FALSE,
					is_station_30_ready = FALSE,
					is_station_40_ready = FALSE,
					is_station_50_ready = FALSE,
					is_station_60_ready = FALSE;

	char            status[6+1] ;


	me->GetStationStatus(me, status);

	is_station_10_ready = status[0];
	is_station_20_ready = status[1];
	is_station_30_ready = status[2];
	is_station_40_ready = status[3];
	is_station_50_ready = status[4];
	is_station_60_ready = status[5];

	if(     is_station_10_ready &&
			is_station_20_ready &&
			is_station_30_ready &&
			is_station_40_ready &&
			is_station_50_ready &&
			is_station_60_ready)
	{

		TIMEOUT_INIT();
		step = CHECK_TABLE_POITION;

		while( TRUE)
		{

			/* Freeza all cylinder to move during rotation */
			if (step >= CHECK_TABLE_POITION && step < ROTATION_FINIHED_OK)
			{
				//me->SetSensor(me, "CYLINDER_UNLOCK_STATION_10", FALSE);
				me->SetSensor(me, "CYLINDER_PLUG_STATION_20",   FALSE);
				me->SetSensor(me, "CYLINDER_PLUG_STATION_30",   FALSE);
				me->SetSensor(me, "CYLINDER_PLUG_V_STATION_30", FALSE);
				me->SetSensor(me, "CYLINDER_PLUG_STATION_40",   FALSE);
				me->SetSensor(me, "CYLINDER_PLUG_STATION_50",   FALSE);
				//me->SetSensor(me, "CYLINDER_UNLOCK_STATION_60", FALSE);

				//me->SetSensor(me, "CYLINDER_LOCK_STATION_10",  TRUE);
				me->SetSensor(me, "CYLINDER_UNPLUG_STATION_20",  TRUE);
				me->SetSensor(me, "CYLINDER_UNPLUG_STATION_30",  TRUE);
				me->SetSensor(me, "CYLINDER_UNPLUG_V_STATION_30",TRUE);
				me->SetSensor(me, "CYLINDER_UNPLUG_STATION_40",  TRUE);
				me->SetSensor(me, "CYLINDER_UNPLUG_STATION_50",  TRUE);
				//me->SetSensor(me, "CYLINDER_LOCK_STATION_60",  TRUE);

				/* Electovalve AIR PRINTER */
				/*EXCCHECK( me->SetSensor(me, "PRINTER_AIR_VALVE_ACTIVATE", FALSE));
				EXCCHECK( me->SetSensor(me, "PRINTER_AIR_VALVE_DESACTIVATE", TRUE));*/
			}

			EXCCHECK( me->GetSensor(me, "INFRARED_BARRIER", &status_barrier));
			if (status_barrier == 0)
			{
				Sleep(1000);
				EXCCHECK( me->GetSensor(me, "MICRO_CAM", &status_micro_cam));
				if(status_micro_cam == 0)// in not indexed poition
					EXCTHROW( TESTER_ERROR_TIMEOUT, "Plateau pas en position");
				else  if(status_micro_cam == 1)// in indexed poition
					EXCTHROW( TESTER_ERROR_TIMEOUT, "Erreur démarrage Plateau");
			}

			/* si aprés passage barriére un véin sort? il doit vérifier une autre fois is_table_ready should be = 0 */
			else if (!is_table_rotation_finished)
			{
				switch  (step)
				{
					case  CHECK_TABLE_POITION : /* check the table in wich position and start rotation */

						//Confirm = ConfirmPopup ("Attention Risque de Collision!", "Rotation Table Start!!, Confirmer?");
						Confirm = 1;
						if (Confirm == 1)
						{
							EXCCHECK( me->GetSensor(me, "MICRO_CAM", &status_micro_cam));

							if(status_micro_cam == 1 )// in indexed poition
							{
								EXCCHECK( me->SetSensor(me, "START_ROTATION", 1));
								EXCCHECK( me->SetSensor(me, "FORCAGE_ROTATION", 1));
								step ++;
							}
							else if(status_micro_cam == 0 )// in not indexed poition
							{
								EXCCHECK( me->SetSensor(me, "START_ROTATION", 1)); //F
								step ++;
							}
						}
						else step =  ROTATION_FINIHED_OK;
						break;


					case  CHECK_TABLE_IN_ROTATION :

						EXCCHECK( me->GetSensor(me, "MICRO_CAM", &status_micro_cam));
						if(status_micro_cam == 0)// in rotation now
						{
							EXCCHECK( me->SetSensor(me, "FORCAGE_ROTATION", 0));
							step ++;
						}
						break;


					case  CHECK_IF_ROTATION_FINIHED :

						EXCCHECK( me->GetSensor(me, "MICRO_CAM", &status_micro_cam));
						if(status_micro_cam == 1)// rotation finished
						{
							EXCCHECK( me->SetSensor(me, "START_ROTATION", 0));
							EXCCHECK( me->SetSensor(me, "FORCAGE_ROTATION", 0));
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

	else EXCTHROW( TESTER_ERROR_TIMEOUT, "vérifier les Conditions Rotation Plateau!");

Error:
	me->SetSensor(me, "FORCAGE_ROTATION", 0);
	me->SetSensor(me, "START_ROTATION", 0);
	EXCRETHROW(pexception);
}
//*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__	"Tester::GetStationStatus"
static SElExceptionPtr GetStationStatus(struct _STester* me, char Table_Ready[6+1])
{
	SElExceptionPtr	pexception = NULL;

	uint32_t		status_cylinder_plugged_station_20,
					status_cylinder_unplugged_station_20,

					status_cylinder_plugged_station_30,
					status_cylinder_unplugged_station_30,
					status_cylinder_V_plugged_station_30,
					status_cylinder_V_unplugged_station_30,

					status_cylinder_plugged_station_40,
					status_cylinder_unplugged_station_40,

					status_cylinder_plugged_station_50,
					status_cylinder_unplugged_station_50,
					status_cylinder_Z_Up,
					status_cylinder_Z_Down,
					status_cylinder_X_Rear,
					status_cylinder_X_Front,

     				isApplicatorFront;


	bool_t			home_bit = FALSE,
					mission_running;

	bool_t			printer_up = FALSE;
	


	char 			is_station_10_ready = FALSE,
					is_station_20_ready = FALSE,
					is_station_30_ready = FALSE,
					is_station_40_ready = FALSE,
					is_station_50_ready = FALSE,
					is_station_60_ready = FALSE;



	/* STATION 10 */

	is_station_10_ready = TRUE;

	/* STATION 20 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_STATION_20", &status_cylinder_plugged_station_20));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_STATION_20", &status_cylinder_unplugged_station_20));
	if( !status_cylinder_plugged_station_20 && status_cylinder_unplugged_station_20)
	{
		is_station_20_ready = TRUE;
	}
	else
		is_station_20_ready = FALSE;


	/* STATION 30 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_STATION_30", &status_cylinder_plugged_station_30));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_STATION_30", &status_cylinder_unplugged_station_30));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_V_STATION_30", &status_cylinder_V_plugged_station_30));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_V_STATION_30", &status_cylinder_V_unplugged_station_30));

	if( !status_cylinder_plugged_station_30 && status_cylinder_unplugged_station_30 && !status_cylinder_V_plugged_station_30 && status_cylinder_V_unplugged_station_30)
	{
		is_station_30_ready = TRUE;
	}
	else
		is_station_30_ready = FALSE;

	/* STATION 40 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_STATION_40", &status_cylinder_plugged_station_40));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_STATION_40", &status_cylinder_unplugged_station_40));
	if( !status_cylinder_plugged_station_40 && status_cylinder_unplugged_station_40)
	{
		is_station_40_ready = TRUE;
	}
	else
		is_station_40_ready = FALSE;


	/* STATION 50 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_STATION_50", &status_cylinder_plugged_station_50));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_STATION_50", &status_cylinder_unplugged_station_50));
	if( !status_cylinder_plugged_station_50 && status_cylinder_unplugged_station_50)
	{
		is_station_50_ready = TRUE;
	}
	else
		is_station_50_ready = FALSE;

	/* STATION 60 */
	
	EXCCHECK( me->GetSensor(me, "SENSOR_APPLICATOR_BACKWARD", &isApplicatorFront));
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60", &status_cylinder_Z_Up));
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_60", &status_cylinder_Z_Down));
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_REAR_STATION_60", &status_cylinder_X_Rear));
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_FRONT_STATION_60", &status_cylinder_X_Front));
	//printf(" APPLICATOR %d\n",isApplicatorFront);
	//printf("z up %d\n",status_cylinder_Z_Up);
	//printf("x rea %d\n",status_cylinder_X_Rear);
	//printf("x front %d\n",status_cylinder_X_Front);
	if(!isApplicatorFront && status_cylinder_Z_Up && !status_cylinder_Z_Down  )//*&& status_cylinder_X_Rear && !status_cylinder_X_Front*/
	{
	    is_station_60_ready = TRUE;	
	}
	else
		is_station_60_ready = FALSE;

#if 0      /* formerly excluded lines */
	EXCCHECK( me->GetSensor(me, "PRINTER_INPUT2", &printer_up));
	if(printer_up)
	{
		is_station_60_ready = TRUE;
	}
	else
		is_station_60_ready = FALSE;
#endif     /* formerly excluded lines */


	Table_Ready[0] = is_station_10_ready;
	Table_Ready[1] = is_station_20_ready;
	Table_Ready[2] = is_station_30_ready;
	Table_Ready[3] = is_station_40_ready;
	Table_Ready[4] = is_station_50_ready;
	Table_Ready[5] = is_station_60_ready;
	Table_Ready[6] = 0X00;

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

typedef struct _SBeepThreadData
{
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
	uint32_t*			value
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
#define __FUNC__	"Tester::GetSensorAI"
static SElExceptionPtr GetSensorAI
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
		EXCCHECK( me->sensorAPI->GetAI(me->sensorAPI, plogical_name, value));

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
		do
		{
			EXCCHECK( me->GetSensor(me, "BUTTON_NOK", &status));
			time = (double)(clock()-_start_test_clock);
		}
		while (status == 1 && time < 300);

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
			do
			{
				EXCCHECK( me->GetSensor(me, "INFRARED_BARRIER", &status_barrier));
				EXCCHECK( me->GetSensor(me, "BUTTON_NOK", &status));
				if (status_barrier)
					EXCCHECK( me->SetSensor(me, "INDICATOR_NOK", 1));
				else
					EXCCHECK( me->SetSensor(me, "INDICATOR_NOK", 0));

				time = (double)(clock()-_start_test_clock);

			}
			while (status == 1 && time < time_push);

			EXCCHECK( me->SetSensor(me, "INDICATOR_NOK", 0));

			if (status == 1 && time >= time_push && status_barrier) //Reset Station 1
			{
				EXCCHECK( me->SetSensor(me, "INDICATOR_NOK", 1));
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
		do
		{
			EXCCHECK( me->GetSensor(me, "BUTTON_MENU", &status));
			time = (double)(clock()-_start_test_clock);
		}
		while (status == 1 && time < 300);

		EXCCHECK( me->GetSensor(me, "BUTTON_MENU", &status));

		if (status == 0 && time <= 300) //pulse
		{
			if( state==1 && me->eventAPI)
				EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_TESTER_BUTTON_PRESSED_MENU, NULL));
		}

		else if (status == 1) //appui long
		{
			_start_test_clock =  clock();
			do
			{
				EXCCHECK( me->GetSensor(me, "BUTTON_MENU", &status));
				time = (double)(clock()-_start_test_clock);
			}
			while (status == 1 && time < time_push);

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
	bool_t          insert1 = 0;
	bool_t          insert2 = 0;

	/* check where is the table */
	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &status_micro_cam));

	if (status_micro_cam == 0)
		me->TableRotate(me, 6000,TRUE);

	EXCCHECK( me->GetSensor(me, "INFRARED_BARRIER", &status_barrier));//INFRARED_BARRIER_OUT

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

	    EXCCHECK( me->SetSensor(me, "VERRINE_ORANGE", !state));
		EXCCHECK( me->SetSensor(me, "VEROUILLAGE_ABIS_DROITE", !state));
		EXCCHECK( me->SetSensor(me, "VEROUILLAGE_ABIS_GAUCHE", !state));
		EXCCHECK( me->SetSensor(me, "VEROUILLAGE_ABIS_ARRIERE", !state));

Error:
	EXCRETHROW(pexception);
}

#undef __FUNC__
#define __FUNC__	"JigThread"
SElExceptionPtr JigThread(struct _STester* me)
{
	SElExceptionPtr	pexception = NULL;
	bool_t			microCam = 0;
	int32_t			insert1 = 0,
					insert2 = 0,
					insert3 = 0,
					gs_plug_station_20,
					gs_unplug_station_20,

					gs_plug_station_30,
					gs_unplug_station_30,

					gs_vertical_plug_station_30,
					gs_vertical_unplug_station_30,

					gs_plug_station_40,
					gs_unplug_station_40,

					gs_plug_station_50,
					gs_unplug_station_50,

					gs_printer_front = 0,
					gs_printer_rear = 0;



	/* Check if panel inserted or released STATION 10 */
	EXCCHECK( me->GetSensor(me, "SENSOR_1_PRESENCE_PRODUCT_STATION_10", &insert1));
	EXCCHECK( me->GetSensor(me, "SENSOR_2_PRESENCE_PRODUCT_STATION_10", &insert2));
	EXCCHECK( me->GetSensor(me, "SENSOR_3_PRESENCE_PRODUCT_STATION_10", &insert3));

	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	if (insert1 && insert2 && insert3 && microCam)
	{
		if (me->eventAPI && !gs_inserted)
		{
			Sleep(1000); //confirm product inserted
			/* Check if panel inserted or released STATION 10 */
			EXCCHECK( me->GetSensor(me, "SENSOR_1_PRESENCE_PRODUCT_STATION_10", &insert1));
			EXCCHECK( me->GetSensor(me, "SENSOR_2_PRESENCE_PRODUCT_STATION_10", &insert2));
			EXCCHECK( me->GetSensor(me, "SENSOR_3_PRESENCE_PRODUCT_STATION_10", &insert3));
			EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
			if (insert1 && insert2 && insert3 && microCam)
			{
				EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_INSERTED, NULL));
				gs_inserted = 1;
				gs_released = 0;
			}
		}
	}

	else if ((!insert1 || !insert2 || !insert3) && microCam)
	{

		if (me->eventAPI && !gs_released)
		{
			Sleep(1000); //confirm product removed
			/* Check if pannel is removed STATION 10*/
			EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
			EXCCHECK( me->GetSensor(me, "SENSOR_1_PRESENCE_PRODUCT_STATION_10", &insert1));
			EXCCHECK( me->GetSensor(me, "SENSOR_2_PRESENCE_PRODUCT_STATION_10", &insert2));
			EXCCHECK( me->GetSensor(me, "SENSOR_3_PRESENCE_PRODUCT_STATION_10", &insert3));
			if ((!insert1 || !insert2 || !insert3) && microCam)
			{
				EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_RELEASED, NULL));
				gs_released = 1;
				gs_inserted = 0;
			}
		}
	}

	/* Check if panel plugged station 20 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_STATION_20", &gs_plug_station_20));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_STATION_20", &gs_unplug_station_20));
	if (!gs_unplug_station_20 && gs_plug_station_20)
	{
		if (me->eventAPI && !gs_plugged_station_20)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_PLUGGED_STATION_20, NULL));

		gs_plugged_station_20 = 1;
	}
	else
	{
		gs_plugged_station_20 = 0;
	}

	/* Check if panel unplugged station 20 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_STATION_20", &gs_plug_station_20));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_STATION_20", &gs_unplug_station_20));
	if (gs_unplug_station_20 && !gs_plug_station_20)
	{
		if (me->eventAPI && !gs_unplugged_station_20)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_UNPLUGGED_STATION_20, NULL));

		gs_unplugged_station_20 = 1;
	}
	else
	{
		gs_unplugged_station_20 = 0;
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



	/* Check if vertical actuator plugged STATION 30 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_V_STATION_30", &gs_vertical_plug_station_30));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_V_STATION_30", &gs_vertical_unplug_station_30));
	if (!gs_vertical_unplug_station_30 && gs_vertical_plug_station_30)
	{
		if (me->eventAPI && !gs_vertical_plugged_station_30)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_VERTICAL_PLUGGED_STATION_30, NULL));

		gs_vertical_plugged_station_30 = 1;
	}
	else
	{
		gs_vertical_plugged_station_30 = 0;
	}

	/* Check if vertical actuator is unplugged STATION 30 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_V_STATION_30", &gs_vertical_plug_station_30));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_V_STATION_30", &gs_vertical_unplug_station_30));
	if (gs_vertical_unplug_station_30 && !gs_vertical_plug_station_30)
	{
		if (me->eventAPI && !gs_vertical_unplugged_station_30)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_VERTICAL_UNPLUGGED_STATION_30, NULL));

		gs_vertical_unplugged_station_30 = 1;
	}
	else
	{
		gs_vertical_unplugged_station_30 = 0;
	}


	/* Check if panel plugged STATION 40 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_STATION_40", &gs_plug_station_40));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_STATION_40", &gs_unplug_station_40));
	if (!gs_unplug_station_40 && gs_plug_station_40)
	{
		if (me->eventAPI && !gs_plugged_station_40)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_PLUGGED_STATION_40, NULL));

		gs_plugged_station_40 = 1;
	}
	else
	{
		gs_plugged_station_40 = 0;
	}

	/* Check if panel unplugged STATION 40 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_STATION_40", &gs_plug_station_40));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_STATION_40", &gs_unplug_station_40));
	if (gs_unplug_station_40 && !gs_plug_station_40)
	{
		if (me->eventAPI && !gs_unplugged_station_40)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_UNPLUGGED_STATION_40, NULL));

		gs_unplugged_station_40 = 1;
	}
	else
	{
		gs_unplugged_station_40 = 0;
	}

	/* Check if panel plugged STATION 50 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_STATION_50", &gs_plug_station_50));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_STATION_50", &gs_unplug_station_50));
	if (!gs_unplug_station_50 && gs_plug_station_50)
	{
		if (me->eventAPI && !gs_plugged_station_50)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_PLUGGED_STATION_50, NULL));

		gs_plugged_station_50 = 1;
	}
	else
	{
		gs_plugged_station_50 = 0;
	}

	/* Check if panel unplugged STATION 50 */
	EXCCHECK( me->GetSensor(me, "IS_PANEL_PLUGGED_STATION_50", &gs_plug_station_50));
	EXCCHECK( me->GetSensor(me, "IS_PANEL_UNPLUGGED_STATION_50", &gs_unplug_station_50));
	if (gs_unplug_station_50 && !gs_plug_station_50)
	{
		if (me->eventAPI && !gs_unplugged_station_50)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_UNPLUGGED_STATION_50, NULL));

		gs_unplugged_station_50 = 1;
	}
	else
	{
		gs_unplugged_station_50 = 0;
	}


	/* Check if gripper open  station 60 */
	EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_OPENED_STATION_60", &gs_open_gripper));
	EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CLOSED_PRODUCT_STATION_60", &gs_close_gripper));
	if (!gs_close_gripper && gs_open_gripper )
	{
		if (me->eventAPI && !gs_open_gripper)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_OPEN_GRIPPER, NULL));
		gs_open_gripper = 1;
	}
	else
	{
		gs_open_gripper = 0;
	}


	/* Check if gripper closed station 60 */
	EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CLOSED_PRODUCT_STATION_60", &gs_close_gripper));
	EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_OPENED_STATION_60", &gs_open_gripper));

	if (gs_close_gripper && !gs_open_gripper)
	{
		if (me->eventAPI && !gs_close_gripper)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_CLOSE_GRIPPER, NULL));

		gs_close_gripper = 1;

	}
	else
	{
		gs_close_gripper = 0;
	}
	
	/* Check if X rear  station 60 */
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_REAR_STATION_60", &gs_x_rear));
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_FRONT_STATION_60", &gs_x_front));

	if (gs_x_rear && !gs_x_front)
	{
		if (me->eventAPI && !gs_x_rear)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_ACTUATOR_X_REAR, NULL));

		gs_x_rear = 1;

	}
	else
	{
		gs_x_rear = 0;
	}
	/* Check if X front  station 60 */
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_REAR_STATION_60", &gs_x_rear));
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_FRONT_STATION_60", &gs_x_front));

	if (gs_x_front && !gs_x_rear)
	{
		if (me->eventAPI && !gs_x_front)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_ACTUATOR_X_FRONT, NULL));

		gs_x_front = 1;

	}
	else
	{
		gs_x_front = 0;
	}
/* Check if Z UP  station 60 */
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60", &gs_z_up));
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_60", &gs_z_down));

	if (gs_z_up && !gs_z_down)
	{
		if (me->eventAPI && !gs_z_up)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_ACTUATOR_Z_UP, NULL));

		gs_z_up = 1;

	}
	else
	{
		gs_z_up = 0;
	}

	/* Check if Z DOWN  station 60 */
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60", &gs_z_up));
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_60", &gs_z_down));

	if (gs_z_down && !gs_z_up)
	{
		if (me->eventAPI && !gs_z_down)
			EXCCHECK( me->eventAPI->PulseEvent(me->eventAPI, EVNT_JIG_ITEM_ACTUATOR_Z_DOWN, NULL));

		gs_z_down = 1;

	}
	else
	{
		gs_z_down = 0;
	}

Error:
	EXCRETHROW(pexception);
}

#undef __FUNC__
#define __FUNC__	"TableThread"
SElExceptionPtr TableThread(struct _STester* me)
{
	SElExceptionPtr	pexception = NULL;
	
	bool_t			is_station_10_ready = FALSE,
					is_station_20_ready = FALSE,
					is_station_30_ready = FALSE,
					is_station_40_ready = FALSE,
					is_station_50_ready = FALSE,
					is_station_60_ready = FALSE;



	char            status[6+1] ;


	me->GetStationStatus(me, status);

	is_station_10_ready = status[0];
	is_station_20_ready = status[1];
	is_station_30_ready = status[2];
	is_station_40_ready = status[3];
	is_station_50_ready = status[4];
	is_station_60_ready = status[5];

	if(     !is_station_10_ready ||
			!is_station_20_ready ||
			!is_station_30_ready ||
			!is_station_40_ready ||
			!is_station_50_ready ||
			!is_station_60_ready)
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
		case STATION_20 :
			EXCCHECK( me->SetSensor(me, "CMD_Relay_2B", FALSE)); //dielectric test: DC power connection
			EXCCHECK( me->SetSensor(me, "CMD_Relay_2C", FALSE));
			EXCCHECK( me->SetSensor(me, "CMD_Relay_2D", FALSE));
			EXCCHECK( me->SetSensor(me, "CMD_Relay_2E", FALSE));
			EXCCHECK( me->UnPlugPanelStation20(me, 5000));
			break;

		case STATION_30 :
			EXCCHECK( me->SetSensor(me, "CMD_Relay_FTA1",    FALSE));		 //digital Input TEST RELAY
			EXCCHECK( me->SetSensor(me, "CMD_Relay_FTA2",    FALSE));		 //digital Input TEST RELAY
			EXCCHECK( me->SetSensor(me, "CMD_Relay_FTA3",    FALSE));        //digital Input TEST RELAY
			EXCCHECK( me->SetSensor(me, "CMD_Relay_FTA4",    FALSE));        //digital Input TEST RELAY
			EXCCHECK( me->SetSensor(me, "CMD_DBUS_Relay_3F", FALSE));        //DBUS activation RELAY
			EXCCHECK( me->SetSensor(me, "CMD_Relay_3G", 	 FALSE));        //12 VDC measurement RELAY
			EXCCHECK( me->SetSensor(me, "CMD_Relay_3H", 	 FALSE));        //12 VDC measurement RELAY
			EXCCHECK( me->SetSensor(me, "CMD_Relay_3I",      FALSE));        //12 VDC measurement RELAY
			EXCCHECK( me->SetSensor(me, "CMD_Relay_3L",      FALSE));        //PWM Feedback Input X8.7
			EXCCHECK( me->SetSensor(me, "CMD_Relay_3M",      FALSE));        //PWM Feedback Input X8.4
			EXCCHECK( me->SetSensor(me, "CMD_Relay_3N",      FALSE));        //PWM Feedback Input X9.4
			EXCCHECK( me->SetSensor(me, "CMD_Relay_3O",      FALSE));        //5 VDC measurement RELAY
			EXCCHECK( me->SetSensor(me, "CMD_Relay_3P",      FALSE));        //5 VDC measurement RELAY
			EXCCHECK( me->SetSensor(me, "CMD_Relay_3Q_3Q2",  FALSE));        //Check externaltemperature sensor input RELAY
			EXCCHECK( me->SetSensor(me, "CMD_PowerSupply_Relay_3R", FALSE)); // ~ POWER SUPPLY activation RELAY
			//EXCCHECK( me->VerticalUnplugStation30(me,  3000));
			EXCCHECK( me->UnplugPanelStation30(me,  5000));
			
			break;

		case STATION_40 :
			EXCCHECK( me->SetSensor(me, "CMD_Relay_4A",     		FALSE));
			EXCCHECK( me->SetSensor(me, "CMD_Relay_4B",     		FALSE));
			EXCCHECK( me->SetSensor(me, "CMD_PowerSupply_Relay_4H", FALSE));
			EXCCHECK( me->SetSensor(me, "CMD_Relay_4S",    			FALSE));
			EXCCHECK( me->SetSensor(me, "CMD_Relay_4T",     	    FALSE));
			EXCCHECK( me->SetSensor(me, "CMD_Relay_4U",    		    FALSE));
			EXCCHECK( me->SetSensor(me, "CMD_Relay_4V_4V2",		    FALSE));
			EXCCHECK( me->SetSensor(me, "CMD_Relay_4Z",     		FALSE));
			EXCCHECK( me->SetSensor(me, "CMD_Relay_4W",    			FALSE));
			EXCCHECK( me->UnplugPanelStation40(me, 5000));
			break;
			
		case STATION_50 :
			EXCCHECK( me->SetSensor(me, "CMD_Relay_5C",     		FALSE));
			EXCCHECK( me->SetSensor(me, "CMD_Relay_5D",     		FALSE));
			EXCCHECK( me->SetSensor(me, "CMD_Relay_5E",             FALSE));
			EXCCHECK( me->SetSensor(me, "CMD_Relay_5F",    			FALSE));
			EXCCHECK( me->UnplugPanelStation50(me,  5000));
			break;
			
			
			case STATION_60 :
            //EXCCHECK( me->HomePickPlacePanelStation60(me,  6000));
			break;



	}

Error:
	EXCRETHROW(pexception);
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
	if (timeout)
	{
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
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
		EXCCHECK( (last_bcr->reader)->StartReading(last_bcr->reader));
		//printf("START READING \n");
		Sleep(800);
		EXCCHECK( (last_bcr->reader)->GetBarCode(last_bcr->reader, buffer, bufferSize));
		Sleep(100);
		//printf("GetBarCode \n");
		EXCCHECK( (last_bcr->reader)->StopReading(last_bcr->reader));
		//printf("StopReading \n");
	}

Error:
	EXCRETHROW(pexception);
}