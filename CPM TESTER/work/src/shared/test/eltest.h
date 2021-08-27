#if !defined(__EL_TEST_H__)
#define __EL_TEST_H__

#include <windows.h>
#include <exception/Exception.h>
#include <time.h>
#include <elstring.h>
#include "msxmldom.h" 

#ifndef EXCLUDE_TEST_MAKROS
#include <guid.h>
#include <objlist/objlist.h> 
#include <tester/tester.h>
#include <counter/counter.h>
#include <ivi.h>
#include <iviswtch.h>
#include <IviDCPwr.h>
#include <IviDmm.h>
#include <eldaq/class/eldaq.h>
#include <printer/class/printers.h>
#include <Camera/class/cameras.h>
#include <labels/tester_labels.h>
#include <sonometer/class/sonometers.h>
#include <dielectrimeter/class/dielectrimeters.h>
#include <generator/class/generators.h>
#include <dbs/dbs.h>
#include <dbus/dbus.h>
#include <traca/class/tracas.h>

#endif

#define		MAX_LENGTH_CSV	   2000


#if defined(WINDOWS)
	#define TEST_API		__declspec( dllexport )
#elif defined(UNIX)
	#define TEST_API		extern "C"
#else
	#define TEST_API
#endif

#ifdef __cplusplus
    extern "C" {
#endif 

		
int Lock_Lecture_Trame_CAN;

typedef SElExceptionPtr (_FUNCC *TestFce_t)(void *TestPtr);

typedef struct _STestSeq {
    const char*     InitName;
    const char*     CleanupName;
    const char*     WaitName;
    TestFce_t       Init;   /*!< Function which initialize test */
    TestFce_t       Cleanup;/*!< Function which cleanups test */
    TestFce_t       Wait;/*!< Function which cleanups test */
} STestSeq, *STestSeqPtr;

typedef struct _STestListItem {
    const char*     name;   /*!< Test name */
    const char*     HelpName;
    const char*     CheckName;
    const char*     InitName;
    const char*     RunName;
    const char*     CleanupName;
    TestFce_t         Help;   /*!< Function whitch returns description of the test */
    TestFce_t         Check;  /*!< Function which checks the all test resources */
    TestFce_t         Init;   /*!< Function which initialize test */
    TestFce_t         Run;    /*!< function which launch test */
    TestFce_t         Cleanup;/*!< Function which cleanups test */
} STestListItem, *STestListItemPtr;

typedef struct _SFieldLabel {
	char				SAPMN[10+1];
	char				HWID[45+1];
	char				SAP_Supplier[10+1];
	char				Supplier_Series[9+1];
	char				VDE_Version[12+1];
	char				Temperature[4+1];
	char				Nominal_Voltage[20+1];
	char				Nominal_Power[6+1];
} SFieldLabel, *SFieldLabelPtr;

typedef struct _STestParam {
	SFieldLabelPtr		pFieldLabel;
	uint32_t			process;
	uint32_t			step;
	CAObjHandle			pTicket;
	int					pTicket_CSV;
	int                 RnR_counter;
	char				csv_dir[MAX_DIRNAME_LEN];
	char				line_CSV[MAX_LENGTH_CSV];
	SELStringPtr		pHelp;
	int32_t				shift_id;
	int32_t				fault_continue;
	int32_t				Test_RnR;
	int32_t				Retested;
	int32_t				Write_Tracability_MMS;
	int32_t				Skip_All_Test;
	int32_t				Check_DTC;
	int32_t				Autorisation_retest;
	int32_t             Origin_Fault;
	char				Name_Fault[256];
	char				Barcode[256];
	int					palette;
	char				Time[32];
	char				Date[32];
	char				Time_BSH[32];
	char				Date_BSH[32];
	char				Date_NVRAM[32];
	char				TicketDirPath[MAX_PATHNAME_LEN];
	char				TicketFilePath[MAX_PATHNAME_LEN]; /* output parameter */
	char                Production_Serial[10+1];
	char     	        Board_Ver_pFin[7+1];
	char     			Serial_Board_Ver_sFin[9+1];//ANWAR
	char                Num_Serie_sFin[20+1];
	char                Num_Serie_pFin[20+1];
	char                half_Num_Serie_pFin[20+1];
	char		        SMM[29+1];
	int                 id_Board_pFin;

	SElExceptionPtr 	pexception;
} STestParam, *STestParamPtr;

typedef SElExceptionPtr (_FUNCC *IniTestPanel_t)(const char* pictureLeft, const char* pictureRight, bool_t reset);
typedef SElExceptionPtr (_FUNCC *SetTestPanelDescription_t)(int32_t control, const char* description);
typedef SElExceptionPtr (_FUNCC *SetPanelWarning_t)(int32_t control,  bool_t status);
typedef SElExceptionPtr (_FUNCC *SetTestPanelReferences_t)(char* RefValeo, char* RefCustomer);
typedef SElExceptionPtr (_FUNCC *SetTestPanelMode_t)(int32_t mode);
typedef SElExceptionPtr	(_FUNCC *GetTestFunctionFcePtr_t)(int32_t process, int32_t index, STestListItemPtr* pTest, int32_t* count);
typedef SElExceptionPtr	(_FUNCC *GetTestSeqFunctionFcePtr_t)(int32_t index, STestSeq* pTestSeq, int32_t* count); 
typedef SElExceptionPtr (_FUNCC *CloseTestPanel_t)(void);
typedef SElExceptionPtr (_FUNCC *SetPanelStationStatus_t)(int32_t station,  bool_t status);
typedef SElExceptionPtr (_FUNCC *SetTestPanelTime_t)(int32_t station,  int32_t time);
typedef SElExceptionPtr (_FUNCC *SetTestPanelFault_t)(int32_t station, int32_t station_origin, char* description, bool_t status);
typedef SElExceptionPtr (_FUNCC *GetPalettStatus_t)(int32_t palettID, int32_t *status);
typedef SElExceptionPtr (_FUNCC *GetSeqActive_t)(int* SeqUsed);
typedef SElExceptionPtr (_FUNCC *GetSeqStopFail_t)(int* SeqStopFail);

void SaveXmlBarcode(STestParamPtr pTestParam);
void SaveXmlResultFileFail(STestParamPtr pTestParam);

enum {
    TEST_FCE_TYPE_HELP = 1,
    TEST_FCE_TYPE_CHECK,
    TEST_FCE_TYPE_INIT,
    TEST_FCE_TYPE_RUN,
    TEST_FCE_TYPE_CLEAN
};
/*!
 * Specifies test information
 */ 
typedef struct _STest 
{
    SElExceptionPtr (*SetFunctionPtr)(
        struct _STest* me,
        int32_t     fceType,
		const char*		fceName,
        TestFce_t   pFce
    );
    SElExceptionPtr (*Run)(
    	struct _STest* pTest,
    	STestParamPtr pParams,
    	HANDLE* pHandle,
		bool_t* used 
	);
	SElExceptionPtr (*CloneSelf)(
    	struct _STest* pTest,
    	struct _STest** pTestClone 
	);

    int32_t     _LoopCount;
    bool_t      _IsUsed;
	bool_t		_CanBeUsed;
	int32_t 	_Process;
	int32_t 	_Step;
    STestListItem   test;
} STestObj, *STestPtr;

SElExceptionPtr test_new( STestPtr* pTest);
SElExceptionPtr test_delete( STestPtr* pTest);
void Debug (char* message);

#define TEST_ERROR_NOT_VALID_TESTER_CONFIGURATION    (-2222)
#define TEST_ERR_UNKNOWN_FCE_TYPE					-11111
#define TEST_ERR_UNKNOWN_FCE_PTR					-11112
#define TEST_ERR_TIMEOUT							-11113
#define TEST_ERR_VALUE_OUT_OF_LIMIT					-11114
#define TEST_ERROR_NOT_POSIBLE_TEST					-11115
#define TEST_ERROR_UNKNOWN_CONFIG_VARIABLE			-11116
#define TEST_ERROR_UNKNOWN_VALUE					-11117
#define TEST_ERROR_BUTTON_NOK						-11118
#define TEST_ERROR_CAMERA_SET_PARAMETER				-11119
#define TEST_ERROR_CAMERA_RESPONSE_TIMEOUT			-11120


#ifndef EXCLUDE_TEST_MAKROS

#define CAMERA_PICTO	0
#define CAMERA_LED		1


SElExceptionPtr QueryParameterReal64(
	STestParamPtr	ppar,
	int			process,
	int			step,
	const char* name,
	double*	pValue
);
SElExceptionPtr QueryParameterUInt32(
	STestParamPtr	ppar,
	int			process,
	int			step,
	const char* name,
	uint32_t*	pValue
);
SElExceptionPtr QueryParameterInt32(
	STestParamPtr	ppar,
	int			process,
	int			step,
	const char* name,
	int32_t*	pValue
);
SElExceptionPtr QueryParameterString(
	STestParamPtr	ppar,
	int			process,
	int			step,
	const char* name,
	char		buffer[],
	int32_t		bufferSize
);
SElExceptionPtr SetParameterString(
	STestParamPtr	ppar,
	int			process,
	int			step,
	const char* name,
	const char* buffer
);
SElExceptionPtr SetParameterInt32(
	STestParamPtr	ppar,
	int			process,
	int			step,
	const char* name,
	int32_t		pValue
);
SElExceptionPtr SetParameterUInt32(
	STestParamPtr	ppar,
	int			process,
	int			step,
	const char* name,
	uint32_t		pValue
);
SElExceptionPtr SetParameterReal64(
	STestParamPtr	ppar,
	int			process,
	int			step,
	const char* name,
	double		pValue
);
SElExceptionPtr CreateNewLimit(
	STestParamPtr	ppar,
	int			process,
	int			step,
	int			index,
	char*		limit_str,
	const char*	description
);
SElExceptionPtr CreateNewLimitEx(
	STestParamPtr	ppar,
	int			process,
	int			step,
	int			index,
	char*		min,
	char*		max,
	const char*	description
);
SElExceptionPtr GetLimitConfiguration(
	STestParamPtr	ppar,
	int			process,
	int			step,
	int			index,
	char		buffer[],
	int32_t		bufferSize
);
bool_t LimitExist(
	STestParamPtr	ppar,
	int			process,
	int			step,
	int			index
);
SElExceptionPtr CheckLimitSTRING(
	STestParamPtr	ppar,
	int			process,
	int			step,
	int			index,
	char*		value
);
SElExceptionPtr CheckLimitINT32(
	STestParamPtr	ppar,
	int			process,
	int			step,
	int			index,
	int			value
);
SElExceptionPtr CheckLimitUINT32(
	STestParamPtr	ppar,
	int			process,
	int			step,
	int			index,
	uint32_t	value
);
SElExceptionPtr CheckLimitREAL64(
	STestParamPtr	ppar,
	int			process,
	int			step,
	int			index,
	double		value
);

SElExceptionPtr GetTestDescription(
	STestParamPtr	ppar,
	int			process,
	int			step,
	char*		name,
	char*		desc
);

SElExceptionPtr GetSubTest(
	STestParamPtr	ppar,
	int			process,
	int			step,
	char*   	subtest 
);

SElExceptionPtr GetTestResult(
	STestParamPtr	ppar,
	int32_t*		result
);

SElExceptionPtr SetTestResult(
	STestParamPtr	ppar,
	int			process,
	int			step,
	int			index, 
	SElExceptionPtr	pexception
);

SElExceptionPtr SetTestSequenceResult(
	STestParamPtr	ppar,
	int			mode,
	SElExceptionPtr pexception
);

SElExceptionPtr SetTestSequenceDuration(
	STestParamPtr	ppar,
	int			mode,
	double 		duration
);

SElExceptionPtr SetTestUsed(
	STestParamPtr	ppar,
	int			process,
	int			step
);

SElExceptionPtr SetTestDuration(
	STestParamPtr	ppar,
	int			process,
	int			step,
	double 		duration
);

SElExceptionPtr SetSubTestDuration(
	STestParamPtr	ppar,
	int			process,
	int			step,
	int			index,
	double 		duration
);

SElExceptionPtr SetTestSequenceTime(
	STestParamPtr	ppar,
	double 		duration
);

SElExceptionPtr GetTestSequenceBarcode(
	STestParamPtr	ppar,
	char* barcode,
	uint32_t barcode_size
);

SElExceptionPtr SetTestValue(
	STestParamPtr		ppar,
	int					process,
	int					step,
	const char*			name,
	const char*			value
);

SElExceptionPtr SetTestSequenceValue(
	STestParamPtr		ppar,
	int					mode,
	const char*			name,
	const char*			value
);

SElExceptionPtr GetResultREAL64formXMLString(
	SELStringPtr	result, 
	const char*		path, 
	double*			value
);
SElExceptionPtr GetResultSTRINGformXMLString(
	SELStringPtr		result, 
	const char*			path,
	char*				buffer
);

SElExceptionPtr CameraSetParam(
	SCamerasPtr			pcameras,
	int32_t 			step,
	int32_t				timeout
);
SElExceptionPtr CameraGetdata(
	STesterPtr			ptester,
	SCamerasPtr			pcameras,
	int32_t 			step,
	int32_t				timeout,
	char				*data
);

#define DEFINE_GLOBAL_VARIABLE() \
	static clock_t			_start_testseq_clock; \
	static clock_t			_start_test_clock; \
	extern int				gs_TestPanel; \
	extern SObjListPtr 		gs_Modules; \
	static bool_t			_kline_init = FALSE

#define MAX_AXIS_RESSOURCES	   		1														 
#define MAX_MOTOR_RESSOURCES	    2														  
#define MAX_LOAD_RESSOURCES	    	1
	
#define TEST_USE( resource)		TEST_USE_##resource
#define TEST_USE_LIN			TEST_USE_CAN
#define TEST_USE_CAN			SCANListPtr			 pcan = (SCANListPtr)gs_Modules->Get(gs_Modules, MODULE_CAN)
#define TEST_USE_DBUS			SDBUSListPtr		 pdbus = (SDBUSListPtr)gs_Modules->Get(gs_Modules, MODULE_DBUS)
#define TEST_USE_KLINE			SVLinePtr			 pvline = (SVLinePtr)gs_Modules->Get(gs_Modules, MODULE_VLINE)
#define TEST_USE_TESTER			STesterPtr		  	 ptester = (STesterPtr)gs_Modules->Get(gs_Modules, MODULE_TESTER)
#define TEST_USE_CAMERA			SCamerasPtr			 pcameras = (SCamerasPtr)gs_Modules->Get(gs_Modules, MODULE_CAMERA)
#define TEST_USE_JIG			SJigPtr				 pjig = (SJigPtr)gs_Modules->Get(gs_Modules, MODULE_JIG_CLASS)
#define TEST_USE_COUNTER		SHWCounterPtr		 pcounter = (SHWCounterPtr)gs_Modules->Get(gs_Modules, MODULE_COUNTER)
#define TEST_USE_SENSORS		SDIOSensorsPtr		 psensors = (SDIOSensorsPtr)gs_Modules->Get(gs_Modules, MODULE_SENSORS)
#define TEST_USE_PRINTERS		SPrintersPtr		 pprinters = (SPrintersPtr)gs_Modules->Get(gs_Modules, MODULE_PRINTERS); SPrinterPtr	printer
#define TEST_USE_LABELS			SLabelListPtr		 plabels = (SLabelListPtr)gs_Modules->Get(gs_Modules, MODULE_LABELS)
#define TEST_USE_DBS			SDBSPtr			     pdbs = (SDBSPtr)gs_Modules->Get(gs_Modules, MODULE_DBS)
#define TEST_USE_AXIS			SaxissPtr		     paxiss = (SaxissPtr)gs_Modules->Get(gs_Modules, MODULE_AXIS); SaxisPtr	paxis[MAX_AXIS_RESSOURCES] 
#define TEST_USE_MOTOR			SmotorsPtr		     pmotors = (SmotorsPtr)gs_Modules->Get(gs_Modules, MODULE_MOTOR); SmotorPtr	pmotor[MAX_MOTOR_RESSOURCES] 
#define TEST_USE_LOAD			SLoadsPtr		     pLoads = (SLoadsPtr)gs_Modules->Get(gs_Modules, MODULE_LOADS); SLoadPtr	pload[MAX_LOAD_RESSOURCES] 
#define TEST_USE_SONOMETER		SSonometersPtr	     pSonometers = (SSonometersPtr)gs_Modules->Get(gs_Modules, MODULE_SONOMETERS); SSonometerPtr	psonometer
#define TEST_USE_DIELECTRIMETER SDielectrimetersPtr	 pDielectrimeters = (SDielectrimetersPtr)gs_Modules->Get(gs_Modules, MODULE_DIELECTRIMETERS); SDielectrimeterPtr	pdielectrimeter
#define TEST_USE_GENERATOR      SGeneratorsPtr	     pGenerators = (SGeneratorsPtr)gs_Modules->Get(gs_Modules, MODULE_GENERATORS); SGeneratorPtr	pgenerator
#define TEST_USE_TRACA			STracasPtr			 pTracas = (STracasPtr)gs_Modules->Get(gs_Modules, MODULE_TRACAS);	STracaPtr	ptraca

#define VARIABLE( dtype, name) VARIABLE_##dtype(name)
#define VARIABLE_REAL64(name)	double name
#define VARIABLE_INT32(name)	int32_t name
#define VARIABLE_UINT32(name)	uint32_t name
#define STRING(name, size)		char name[size+1]=""

#define INITIALIZE_PRINTER(resource) \
		EXCCHECK(pprinters->GetPrinterByIndex(pprinters, resource, &printer)); 

#define INITIALIZE_AXIS(resource) \
		EXCCHECK(paxiss->GetaxisByIndex(paxiss, resource, &paxis[resource])); 

#define INITIALIZE_MOTOR(resource) \
		EXCCHECK( pmotors->GetmotorByIndex(pmotors, resource, &pmotor[resource])); 

#define INITIALIZE_LOAD(resource) \
		EXCCHECK( pLoads->GetLoadByIndex(pLoads, resource, &pload[resource]));

#define INITIALIZE_SONOMETER(resource) \
		EXCCHECK(pSonometers->GetSonometerByIndex(pSonometers, resource, &psonometer));

#define INITIALIZE_DIELECTRIMETER(resource) \
		EXCCHECK(pDielectrimeters->GetDielectrimeterByIndex(pDielectrimeters, resource, &pdielectrimeter));

#define INITIALIZE_GENERATOR(resource) \
		EXCCHECK(pGenerators->GetGeneratorByIndex(pGenerators, resource, &pgenerator));

#define INITIALIZE_TRACA(resource) \
		EXCCHECK(pTracas->GetTracaByIndex(pTracas, resource, &ptraca));

#define TESTER_INIT() \
	ptester->InitializeStations(ptester, station, TIMEOUT_DEFAULT)

  #define MOTOR_INIT(resource) \
	EXCCHECK( pmotor[resource]->_InitializeSpec(pmotor[resource])) 

#define MOTOR_STOP(resource) \
	EXCCHECK( pmotor[resource]->EnableMotor(pmotor[resource], 0)) 

#define MOTOR_INIT_ANGLE(resource) \
	EXCCHECK( pmotor[resource]->AlarmReset(pmotor[resource], 0)) 

#define MOTOR_ROTATE(resource, angle, speed) \
	EXCCHECK( pmotor[resource]->MoveMotorPosition(pmotor[resource], 0, angle, speed)); 

#define WAIT_TABLE_ROTATION_FINISHED(timeout) \
		EXCCHECK( ptester->WaitRotationFinished(ptester, timeout))

#define TESTER_OKNOK(timeout) \
	do{	\
		bool_t ok; \
		EXCCHECK( ptester->OkNokCheck(ptester, timeout,&ok)); \
		if(!ok) EXCTHROW(TEST_ERROR_BUTTON_NOK, "Button NOK pressed!" ); \
	}while(0)

#define TESTER_BEEP()  \
	EXCCHECK( ptester->TesterBeep(ptester,100))



/* TEST INIT */
#define TESTINIT_BEGIN() \
	char				_test_name[256] = ""; \
	char				_test_description[256] = "unknown"; \
	clock_t				_start_clock = clock(); \
	SElExceptionPtr     pexception = NULL; \
	int32_t				_test_index = 1; \
	int32_t				error = 0; \
	char                _subtest[256] = ""; \
	int32_t				station = ((STestParamPtr)pTID)->process;


#define TESTINIT_END() \
	TEST_DESCRIPTION(_test_name, _test_description); \
	DISPLAY_TEST(station, _test_name, _test_description); \
	TEST_USED(); \
	_start_test_clock = _start_clock; \
	if(pexception) DISPLAY_TEST_RESULT(station, (double)(clock()-_start_test_clock)/(double)CLOCKS_PER_SEC); \
	if(pexception) TEST_DURATION((double)(clock()-_start_test_clock)/(double)CLOCKS_PER_SEC); \
	TEST_FAILED( pexception ); \
	return  pexception


/* TEST */
#define TEST_BEGIN() \
    SElExceptionPtr     pexception = NULL; \
	int32_t				error = 0; \
	clock_t				_clock = clock(); \
	int32_t				_test_index = 1; \
	int32_t				station = ((STestParamPtr)pTID)->process

#define TEST_END() \
	if(pexception) DISPLAY_TEST_RESULT(station, (double)(clock()-_start_test_clock)/(double)CLOCKS_PER_SEC); \
	if(pexception) TEST_DURATION((double)(clock()-_start_test_clock)/(double)CLOCKS_PER_SEC); \
	TEST_FAILED( pexception ); \
    return  pexception
    
#define HELP_BEGIN() \
    SElExceptionPtr pexception = NULL

#define REQUIRE_END() } else error = error

#define TESTREGUIRE_END() \
	return  pexception
	

/* TEST CLEANUP */
#define TESTCLEANUP_BEGIN() \
	SElExceptionPtr     pexception = NULL; \
	int32_t				_test_index = 1; \
	int32_t				error = 0; \
	int32_t				station = ((STestParamPtr)pTID)->process
    
#define TESTCLEANUP_END() \
	TEST_DURATION((double)(clock()-_start_test_clock)/(double)CLOCKS_PER_SEC); \
	DISPLAY_TEST_RESULT(station, (double)(clock()-_start_test_clock)/(double)CLOCKS_PER_SEC); \
	TEST_FAILED( pexception ); \
	return  pexception


/* TEST SEQUENCE INIT */
#define TESTSEQUENCEINIT_BEGIN() \
	SElExceptionPtr     pexception = NULL; \
	clock_t				_start_clock = clock(); \
	int32_t				error = 0; \
	int32_t				station = ((STestParamPtr)pTID)->process

#define TESTSEQUENCEINIT_END() \
	IVI_CHECK_ERR(error); \
	_start_testseq_clock = clock(); \
	TESTSEQUENCE_INIT_DURATION((double)(clock()-_start_clock)/(double)CLOCKS_PER_SEC); \
	TESTSEQUENCE_INIT_FAILED(pexception); \
	return  pexception

/* TEST SEQUENCE CLEANUP */
#define TESTSEQUENCECLEANUP_BEGIN() \
	SElExceptionPtr     pexception = NULL; \
	clock_t				_start_clock = clock(); \
	int32_t				error = 0; \
	int32_t				station = ((STestParamPtr)pTID)->process

#define TESTSEQUENCECLEANUP_END() \
	_kline_init = FALSE; \
	DISPLAY_TESTSEQUENCE_DURATION((double)(clock()-_start_testseq_clock)/(double)CLOCKS_PER_SEC); \
	TESTSEQUENCE_DURATION((double)(clock()-_start_testseq_clock)/(double)CLOCKS_PER_SEC); \
	TESTSEQUENCE_CLEANUP_DURATION((double)(clock()-_start_clock)/(double)CLOCKS_PER_SEC); \
	TESTSEQUENCE_CLEANUP_FAILED(pexception); \
	return  pexception

/* TEST SEQUENCE WAIT */
#define TESTSEQUENCEWAIT_BEGIN() \
	SElExceptionPtr     pexception = NULL; \
	clock_t				_start_clock = clock(); \
	int32_t				error = 0; \
	int32_t				station = ((STestParamPtr)pTID)->process

#define TESTSEQUENCEWAIT_END() \
	return  pexception


#define HELP_TEST(help) \
	EXCCHECK(((STestParamPtr)pTID)->pHelp->Format(((STestParamPtr)pTID)->pHelp, \
		"	<help>%s</help>\n", help))

#define HELP_PARAMETER(option, name, type, help) HELP_PARAMETER_##option(name, type, help) /* required, name, value type, help */

#define HELP_PARAMETER_OPTIONAL(name, type, help) /* required, name, value type, help */ \
	EXCCHECK(((STestParamPtr)pTID)->pHelp->Format(((STestParamPtr)pTID)->pHelp, \
		"	<parameter>\n" \
		"		<option>Optional</option>\n" \
		"		<name>%s</name>\n" \
		"		<type>%s</type>\n" \
		"		<help>%s</help>\n" \
		"	</parameter>\n", name, type, help))

#define HELP_PARAMETER_REQUIRED(name, type, help) /* required, name, value type, help */ \
	EXCCHECK(((STestParamPtr)pTID)->pHelp->Format(((STestParamPtr)pTID)->pHelp, \
		"	<parameter>\n" \
		"		<option>Required</option>\n" \
		"		<name>%s</name>\n" \
		"		<type>%s</type>\n" \
		"		<help>%s</help>\n" \
		"	</parameter>\n", name, type, help))

#define HELP_END() \
    return  pexception
    
#define TESTREGUIRE_BEGIN() \
	SElExceptionPtr     pexception = NULL;\
	ViStatus	error = VI_SUCCESS

	
#define REQUIRE( res)  REQUIRE_##res	
	
#define REQUIRE_KLINE if (!error) { \
	SVLinePtr pvline = (SVLinePtr)gs_Modules->Get(gs_Modules, MODULE_VLINE);\
	if ( pvline == NULL ) EXCTHROW( TEST_ERROR_NOT_POSIBLE_TEST, _("K-line Module is missing!"))

#define REQUIRE_LIN REQUIRE_CAN

#define REQUIRE_CAN if (!error) { \
	SCANPtr pcan = (SCANPtr)gs_Modules->Get(gs_Modules, MODULE_CAN);\
	if ( pcan == NULL ) EXCTHROW( TEST_ERROR_NOT_POSIBLE_TEST, _("CAN Module is missing!"))

#define REQUIRE_DBUS if (!error) { \
	SDBUSPtr pdbus = (SDBUSPtr)gs_Modules->Get(gs_Modules, MODULE_DBUS);\
	if ( pdbus == NULL ) EXCTHROW( TEST_ERROR_NOT_POSIBLE_TEST, _("DBUS Module is missing!"))
	
#define REQUIRE_TESTER if (!error) { \
	STesterPtr ptester = (STesterPtr)gs_Modules->Get(gs_Modules, MODULE_TESTER);\
	if ( ptester == NULL ) EXCTHROW( TEST_ERROR_NOT_POSIBLE_TEST, _("Tester Module is missing!"))

#define REQUIRE_AXIS if (!error) { \
	SaxissPtr	paxiss = (SaxissPtr)gs_Modules->Get(gs_Modules, MODULE_AXIS);\
	if ( paxiss == NULL ) EXCTHROW( TEST_ERROR_NOT_POSIBLE_TEST, _("Axis Module is missing!"))

#define REQUIRE_MOTOR if (!error) { \
	SaxissPtr	paxiss = (SaxissPtr)gs_Modules->Get(gs_Modules, MODULE_MOTOR);\
	if ( paxiss == NULL ) EXCTHROW( TEST_ERROR_NOT_POSIBLE_TEST, _("Axis Module is missing!"))

#define REQUIRE_CAMERA if (!error) { \
	SIMAQPtr pcamera = (SIMAQPtr)gs_Modules->Get(gs_Modules, MODULE_CAMERA);\
	if ( pcamera == NULL ) EXCTHROW( TEST_ERROR_NOT_POSIBLE_TEST, _("Camera Module is missing!"))

#define REQUIRE_PRINTERS if (!error) { \
	SPrintersPtr pprinters = (SPrintersPtr)gs_Modules->Get(gs_Modules, MODULE_PRINTERS);\
	if ( pprinters == NULL ) EXCTHROW( TEST_ERROR_NOT_POSIBLE_TEST, _("Printers Module is missing!"))


#define FCE_HELP( prefix, nb) \
    TEST_API SElExceptionPtr _FUNCC prefix##_##nb##_help(void* pTID, int32_t mode)
    
#define FCE_CHECK( prefix, nb) \
    TEST_API SElExceptionPtr _FUNCC prefix##_##nb##_check(void* pTID, int32_t mode)

#define FCE_INIT( prefix, nb) \
    TEST_API SElExceptionPtr _FUNCC prefix##_##nb##_init(void* pTID, int32_t mode)

#define FCE_TEST( prefix, nb) \
    TEST_API SElExceptionPtr _FUNCC prefix##_##nb##_test(void* pTID, int32_t mode)
    
#define FCE_CLEANUP( prefix, nb) \
    TEST_API SElExceptionPtr _FUNCC prefix##_##nb##_cleanup(void* pTID, int32_t mode)


#define ADD_TI( name, prefix, nb) { name,\
									#prefix"_"#nb"_help",\
									#prefix"_"#nb"_check",\
									#prefix"_"#nb"_init",\
									#prefix"_"#nb"_test",\
									#prefix"_"#nb"_cleanup",\
									NULL,\
									NULL,\
									NULL,\
									NULL,\
									NULL }

#define DEFINE_TEST_FUNCTIONS( prefix, nb) \
	FCE_HELP( prefix, nb);  \
	FCE_CHECK( prefix, nb);  \
	FCE_INIT( prefix, nb);  \
	FCE_TEST( prefix, nb);  \
	FCE_CLEANUP( prefix, nb)


#define FCE_TESTSEQUENCE_INIT( prefix) \
    TEST_API SElExceptionPtr _FUNCC prefix##_init(void* pTID, int32_t mode)

#define FCE_TESTSEQUENCE_CLEANUP( prefix) \
    TEST_API SElExceptionPtr _FUNCC prefix##_cleanup(void* pTID, int32_t mode)

#define FCE_TESTSEQUENCE_WAIT( prefix) \
    TEST_API SElExceptionPtr _FUNCC prefix##_wait(void* pTID, int32_t mode)

#define ADD_TSI( prefix) { #prefix"_init",\
						   #prefix"_cleanup",\
						   #prefix"_wait",\
						    NULL}

#define DEFINE_TESTSEQUENCE_FUNCTIONS( prefix) \
	FCE_TESTSEQUENCE_INIT(prefix); \
	FCE_TESTSEQUENCE_CLEANUP(prefix); \
	FCE_TESTSEQUENCE_WAIT(prefix)

#define ADD_TSI_LAST_ENTRY { NULL,NULL,NULL,NULL}
#define ADD_TI_LAST_ENTRY { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}


#define PARAM_REAL64_REQUIRED( name, var)\
	EXCCHECK( QueryParameterReal64(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, name, var))
#define PARAM_INT32_REQUIRED( name, var)\
	EXCCHECK( QueryParameterInt32(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, name, var))
#define PARAM_UINT32_REQUIRED( name, var)\
	EXCCHECK( QueryParameterUInt32(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, name, var))
#define PARAM_STRING_REQUIRED( name, buffer) \
	EXCCHECK( QueryParameterString(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, name, buffer, sizeof(buffer)-1))


#define PARAM_UINT32_OPTIONAL( name, var, dvalue) \
	do { \
		*(var) = dvalue;\
		pexception = QueryParameterUInt32(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, name, var); \
		if (pexception) EXCDELETE( pexception); \
		/* EXCIGNORE( )  */ \
	} while (0);

#define PARAM_INT32_OPTIONAL( name, var, dvalue) \
	do { \
		*(var) = dvalue;\
		pexception = QueryParameterInt32(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, name, var); \
		if (pexception) EXCDELETE( pexception); \
		/* EXCIGNORE( )  */ \
	} while (0);

#define PARAM_REAL64_OPTIONAL( name, var, dvalue) \
	do { \
		*(var) = dvalue;\
		pexception = QueryParameterReal64(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, name, var); \
		if (pexception) EXCDELETE( pexception); \
		/* EXCIGNORE( )  */ \
	} while (0);

#define PARAM_STRING_OPTIONAL( name, buffer, dvalue) \
	do { \
		strcpy(buffer, dvalue); \
		pexception = QueryParameterString(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, name, buffer, sizeof(buffer)-1); \
		if (pexception) EXCDELETE( pexception); \
		/* EXCIGNORE( )  */ \
	} while (0);

#define RESULT_XMLSTRING_GET(type, result, path, value) \
	EXCCHECK( GetResult##type##formXMLString(result, path, value))

#define LIMIT_CREATE(limit_str, description) \
	EXCCHECK( CreateNewLimit(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, _test_index, limit_str, description))

#define LIMIT_CREATE_EX(min, max, description) \
	EXCCHECK( CreateNewLimitEx(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, _test_index, min, max, description))

#define LIMIT_CONFIGURATION(buffer) \
	EXCCHECK( GetLimitConfiguration(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, _test_index, buffer, sizeof(buffer)))

#define LIMIT_EXIST() \
	LimitExist(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, _test_index)

#define LIMIT_CHECK( type, var) \
	LIMIT_CHECK_EXT(type, var, pexception); \
	if (((STestParamPtr)pTID)->Test_RnR != TRUE && ((STestParamPtr)pTID)->fault_continue != TRUE) EXCCHECK( pexception);

#define CHECK_FOR_LIMIT(type, var) \
	pexception = CheckLimit##type(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, _test_index, var); \
	if (pexception) EXCDELETE(pexception); \
	else {_clock = clock(); _test_index++; break; }

#define LIMIT_CHECK_EXT( type, var, pexception ) \
	pexception = CheckLimit##type(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, _test_index, var); \
	_clock = clock(); \
	_test_index++

#define LIMIT_SET(type, var) \
	do { \
	SElExceptionPtr pexc_tmp = NULL; \
	pexc_tmp = CheckLimit##type(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, _test_index, var); \
	if (pexc_tmp) EXCDELETE(pexc_tmp); \
	_test_index++; \
	}while(0)

#define TEST_VALUE_SET( name, value) \
	SetTestValue(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, name, value)

#define TEST_DESCRIPTION( name, desc ) \
	GetTestDescription(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, name, desc)

#define TEST_SUB_TEST(SubTest) \
	GetSubTest(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, SubTest)

#define TEST_USED( ) \
	SetTestUsed(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step)

#define TEST_FAILED( pexception ) \
	SetTestResult(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, --_test_index, pexception)

#define TESTSEQUENCE_INIT_FAILED( pexception ) \
	SetTestSequenceResult(pTID, 0, pexception)

#define TESTSEQUENCE_CLEANUP_FAILED( pexception ) \
	SetTestSequenceResult(pTID, 1, pexception)

#define TESTSEQUENCE_INIT_DURATION( duration ) \
	SetTestSequenceDuration(pTID, 0, duration)

#define TESTSEQUENCE_CLEANUP_DURATION( duration ) \
	SetTestSequenceDuration(pTID, 1, duration)

#define TEST_DURATION( duration ) \
	SetTestDuration(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, duration)

#define SUBTEST_DURATION( index, duration ) \
	EXCCHECK( SetSubTestDuration(pTID, ((STestParamPtr)pTID)->process, ((STestParamPtr)pTID)->step, index, duration))

#define TESTSEQUENCE_DURATION( duration ) \
	SetTestSequenceTime(pTID, duration)

#define TESTSEQUENCE_BARCODE( barcode, size) \
	GetTestSequenceBarcode(pTID, barcode, size)

#endif /* EXCLUDE_TEST_MAKROS */

#ifdef __cplusplus
    }
#endif 

#endif /* __EL_TEST_H__ */
