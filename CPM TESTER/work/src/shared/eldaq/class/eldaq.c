#include <userint.h>

/*****************************************************************************
 *  eldaq Class Driver                               
 *  Original Release: 2. kvìtna 2007                                  
 *  By: Zdenek Rykala, ELCOM, a.s.                              
 *      PH. 558 279 900   Fax 558 279 901                              
 *                                                                           
 *  Modification History:                                                    
 *                                                                           
 *       2. kvìtna 2007 - Class Driver Created.                  
 *                                                                           
 *****************************************************************************/

#include <unknwn.h>
#include <vpptype.h>
#include "stdio.h"
#include "eldaq.h"

/****************************************************************************
 *  Global Definitions
 ****************************************************************************/
#define CLASS_NAME      "eldaq"

static  IviStringValueTable ClassErrorMessagesTable = 
    {
    ELDAQ_ERROR_CODES_AND_MSGS,
    {VI_NULL, VI_NULL}
    };

/****************************************************************************
 * Utility Routine Declarations (Non-exported functions)
 ****************************************************************************/
static ViStatus InitAttributes (ViSession vi);

/****************************************************************************
 * Callback Declarations (Non-Exported)
 ****************************************************************************/

static ViStatus _VI_FUNC FuncCapabilities_ReadCallback (ViSession vi, 
                                                        ViSession io, 
                                                        ViConstString channelName, 
                                                        ViAttr attributeId, 
                                                        const ViConstString value);

static ViStatus _VI_FUNC ClassRevision_ReadCallback (ViSession vi, 
                                                     ViSession io, 
                                                     ViConstString channelName, 
                                                     ViAttr attributeId, 
                                                     const ViConstString value);
                                                     
/****************************************************************************
 *  Inherent Function Names, Tables, and Typedefs
 ****************************************************************************/

/*- Function Enumerations -*/
enum
    {
    kIviInit,
    kIviClose,
    kinit,
    kInitWithOptions,
    kclose,
    kreset,
    kself_test,
    kerror_query,
    kerror_message,
    krevision_query,
    kInvalidateAllAttributes,
    kResetWithDefaults,
    kDisable,
    kGetError,
    kClearError,
    kGetErrorInfo,
    kClearErrorInfo,
    kLockSession,
    kUnlockSession,
    kGetNextCoercionRecord,
    kGetNextInterchangeWarning,
    kSimulationInit,
    kSimulationClose,
    kGetAttributeViInt32,
    kGetAttributeViReal64,
    kGetAttributeViString,
    kGetAttributeViBoolean,
    kGetAttributeViSession,
    kSetAttributeViInt32,
    kSetAttributeViReal64,
    kSetAttributeViString,
    kSetAttributeViBoolean,
    kSetAttributeViSession,
    kCheckAttributeViInt32,
    kCheckAttributeViReal64,
    kCheckAttributeViString,
    kCheckAttributeViBoolean,
    kCheckAttributeViSession,
    kResetInterchangeCheck,
    kClearInterchangeWarnings,
    kGetSpecificDriverCHandle,
    kGetSpecificDriverIUnknownPtr,
    kInitSimStatus,
    kNumInherentFunctions
};

/*- Function Names -*/
#define INHERENT_FUNCTION_NAMES \
    "IviInit",                  \
    "IviClose",                 \
    "init",                     \
    "InitWithOptions",          \
    "close",                    \
    "reset",                    \
    "self_test",                \
    "error_query",              \
    "error_message",            \
    "revision_query",           \
    "InvalidateAllAttributes",  \
    "ResetWithDefaults",        \
    "Disable",                  \
    "GetError",                 \
    "ClearError",               \
    "GetErrorInfo",             \
    "ClearErrorInfo",           \
    "LockSession",              \
    "UnlockSession",            \
    "GetNextCoercionRecord",    \
    "GetNextInterchangeWarning",\
    "SimulationInit",           \
    "SimulationClose",          \
    "GetAttributeViInt32",      \
    "GetAttributeViReal64",     \
    "GetAttributeViString",     \
    "GetAttributeViBoolean",    \
    "GetAttributeViSession",    \
    "SetAttributeViInt32",      \
    "SetAttributeViReal64",     \
    "SetAttributeViString",     \
    "SetAttributeViBoolean",    \
    "SetAttributeViSession",    \
    "CheckAttributeViInt32",    \
    "CheckAttributeViReal64",   \
    "CheckAttributeViString",   \
    "CheckAttributeViBoolean",  \
    "CheckAttributeViSession",  \
    "ResetInterchangeCheck",    \
    "ClearInterchangeWarnings", \
    "GetSpecificDriverCHandle",    \
    "GetSpecificDriverIUnknownPtr", \
    "InitSimStatus"
    
/*- Function Enum / Name Table -*/
#define INHERENT_FUNCTION_TABLE(name) \
    {kinit,                             #name"_init"},                      \
    {kInitWithOptions,                  #name"_InitWithOptions"},           \
    {kclose,                            #name"_close"},                     \
    {kreset,                            #name"_reset"},                     \
    {kself_test,                        #name"_self_test"},                 \
    {kerror_query,                      #name"_error_query"},               \
    {kerror_message,                    #name"_error_message"},             \
    {krevision_query,                   #name"_revision_query"},            \
    {kInvalidateAllAttributes,          #name"_InvalidateAllAttributes"},   \
    {kResetWithDefaults,                #name"_ResetWithDefaults"},         \
    {kDisable,                          #name"_Disable"},                   \
    {kGetError,                         #name"_GetError"},                  \
    {kClearError,                       #name"_ClearError"},                \
    {kGetErrorInfo,                     #name"_GetErrorInfo"},              \
    {kClearErrorInfo,                   #name"_ClearErrorInfo"},            \
    {kLockSession,                      #name"_LockSession"},               \
    {kUnlockSession,                    #name"_UnlockSession"},             \
    {kGetNextCoercionRecord,            #name"_GetNextCoercionRecord"},     \
    {kGetAttributeViInt32,              #name"_GetAttributeViInt32"},       \
    {kGetAttributeViReal64,             #name"_GetAttributeViReal64"},      \
    {kGetAttributeViString,             #name"_GetAttributeViString"},      \
    {kGetAttributeViBoolean,            #name"_GetAttributeViBoolean"},     \
    {kGetAttributeViSession,            #name"_GetAttributeViSession"},     \
    {kSetAttributeViInt32,              #name"_SetAttributeViInt32"},       \
    {kSetAttributeViReal64,             #name"_SetAttributeViReal64"},      \
    {kSetAttributeViString,             #name"_SetAttributeViString"},      \
    {kSetAttributeViBoolean,            #name"_SetAttributeViBoolean"},     \
    {kSetAttributeViSession,            #name"_SetAttributeViSession"},     \
    {kCheckAttributeViInt32,            #name"_CheckAttributeViInt32"},     \
    {kCheckAttributeViReal64,           #name"_CheckAttributeViReal64"},    \
    {kCheckAttributeViString,           #name"_CheckAttributeViString"},    \
    {kCheckAttributeViBoolean,          #name"_CheckAttributeViBoolean"},   \
    {kCheckAttributeViSession,          #name"_CheckAttributeViSession"},   \
    {kResetInterchangeCheck,            #name"_ResetInterchangeCheck"},     \
    {kClearInterchangeWarnings,         #name"_ClearInterchangeWarnings"},  \
    {kGetSpecificDriverCHandle,         #name"_GetSpecificDriverCHandle"},  \
    {kGetSpecificDriverIUnknownPtr,     #name"_GetSpecificDriverIUnknownPtr"}

/*- Function Pointer Typedefs -*/
typedef ViStatus (_VI_FUNC *IviInitFuncPtr)(ViRsrc resourceName, 
                                            ViBoolean IDQuery,
                                            ViBoolean resetDevice, 
                                            ViSession vi);

typedef ViStatus (_VI_FUNC *initFuncPtr)(ViRsrc resourceName, 
                                         ViBoolean IDQuery,
                                         ViBoolean resetDevice, 
                                         ViSession *vi);

typedef ViStatus (_VI_FUNC *InitWithOptionsFuncPtr)(ViRsrc resourceName, 
                                                    ViBoolean IDQuery,
                                                    ViBoolean resetDevice, 
                                                    ViConstString optionString,
                                                    ViSession *vi);

typedef ViStatus (_VI_FUNC *closeFuncPtr)(ViSession vi);

typedef ViStatus (_VI_FUNC *IviCloseFuncPtr)(ViSession vi);

typedef ViStatus (_VI_FUNC *resetFuncPtr)(ViSession vi);

typedef ViStatus (_VI_FUNC *self_testFuncPtr)(ViSession vi, 
                                              ViInt16 * selfTestResult,
                                              ViChar selfTestMessage[]);

typedef ViStatus (_VI_FUNC *error_queryFuncPtr)(ViSession vi,
                                                ViInt32  *errorCode, 
                                                ViChar errorMessage[]);

typedef ViStatus (_VI_FUNC *error_messageFuncPtr)(ViSession vi, 
                                                  ViStatus statusCode, 
                                                  ViChar message[]);

typedef ViStatus (_VI_FUNC *revision_queryFuncPtr)(ViSession vi, 
                                                   ViChar driverRev[], 
                                                   ViChar instrRev[]);

typedef ViStatus (_VI_FUNC *InvalidateAllAttributesFuncPtr)(ViSession vi);

typedef ViStatus (_VI_FUNC *ResetWithDefaultsFuncPtr)(ViSession vi);

typedef ViStatus (_VI_FUNC *DisableFuncPtr)(ViSession vi);

typedef ViStatus (_VI_FUNC *GetAttributeViInt32FuncPtr)(ViSession vi, 
                                                        ViConstString channelName, 
                                                        ViAttr attributeId, 
                                                        ViInt32 *value);

typedef ViStatus (_VI_FUNC *GetAttributeViReal64FuncPtr)(ViSession vi, 
                                                         ViConstString channelName, 
                                                         ViAttr attributeId, 
                                                         ViReal64 *value);

typedef ViStatus (_VI_FUNC *GetAttributeViStringFuncPtr)(ViSession vi, 
                                                         ViConstString channelName,
                                                         ViAttr attributeId, 
                                                         ViInt32 bufSize, 
                                                         ViChar value[]);

typedef ViStatus (_VI_FUNC *GetAttributeViBooleanFuncPtr)(ViSession vi, 
                                                          ViConstString channelName, 
                                                          ViAttr attributeId, 
                                                          ViBoolean *value);

typedef ViStatus (_VI_FUNC *GetAttributeViSessionFuncPtr)(ViSession vi, 
                                                          ViConstString channelName, 
                                                          ViAttr attributeId, 
                                                          ViSession *value);

typedef ViStatus (_VI_FUNC *SetAttributeViInt32FuncPtr)(ViSession vi, 
                                                        ViConstString channelName, 
                                                        ViAttr attributeId, 
                                                        ViInt32 value);

typedef ViStatus (_VI_FUNC *SetAttributeViReal64FuncPtr)(ViSession vi, 
                                                         ViConstString channelName, 
                                                         ViAttr attributeId, 
                                                         ViReal64 value);

typedef ViStatus (_VI_FUNC *SetAttributeViStringFuncPtr)(ViSession vi, 
                                                         ViConstString channelName, 
                                                         ViAttr attributeId, 
                                                         ViConstString value);

typedef ViStatus (_VI_FUNC *SetAttributeViBooleanFuncPtr)(ViSession vi, 
                                                          ViConstString channelName, 
                                                          ViAttr attributeId, 
                                                          ViBoolean value);

typedef ViStatus (_VI_FUNC *SetAttributeViSessionFuncPtr)(ViSession vi, 
                                                          ViConstString channelName, 
                                                          ViAttr attributeId, 
                                                          ViSession value);

typedef ViStatus (_VI_FUNC *CheckAttributeViInt32FuncPtr)(ViSession vi, 
                                                          ViConstString channelName, 
                                                          ViAttr attributeId, 
                                                          ViInt32 value);

typedef ViStatus (_VI_FUNC *CheckAttributeViReal64FuncPtr)(ViSession vi, 
                                                           ViConstString channelName, 
                                                           ViAttr attributeId, 
                                                           ViReal64 value);

typedef ViStatus (_VI_FUNC *CheckAttributeViStringFuncPtr)(ViSession vi, 
                                                           ViConstString channelName, 
                                                           ViAttr attributeId, 
                                                           ViConstString value);

typedef ViStatus (_VI_FUNC *CheckAttributeViBooleanFuncPtr)(ViSession vi, 
                                                            ViConstString channelName, 
                                                            ViAttr attributeId, 
                                                            ViBoolean value);

typedef ViStatus (_VI_FUNC *CheckAttributeViSessionFuncPtr)(ViSession vi, 
                                                            ViConstString channelName, 
                                                            ViAttr attributeId, 
                                                            ViSession value);

typedef ViStatus (_VI_FUNC *GetErrorFuncPtr)(ViSession vi, 
                                             ViStatus *errorCode, 
                                             ViInt32 bufferSize, 
                                             ViChar description[]);

typedef ViStatus (_VI_FUNC *ClearErrorFuncPtr)(ViSession vi);

typedef ViStatus (_VI_FUNC *GetErrorInfoFuncPtr)(ViSession vi, 
                                                 ViStatus *primaryError, 
                                                 ViStatus *secondaryError, 
                                                 ViChar errorElaboration[256]);

typedef ViStatus (_VI_FUNC *ClearErrorInfoFuncPtr)(ViSession vi);

typedef ViStatus (_VI_FUNC *LockSessionFuncPtr)(ViSession vi, 
                                                ViBoolean *callerHasLock);

typedef ViStatus (_VI_FUNC *UnlockSessionFuncPtr)(ViSession vi,
                                                  ViBoolean *callerHasLock);

typedef ViStatus (_VI_FUNC *GetNextCoercionRecordFuncPtr)(ViSession vi, 
                                                          ViInt32 bufferSize, \
                                                          ViChar recordBuf[]);

typedef ViStatus (_VI_FUNC *GetNextInterchangeWarningFuncPtr)(ViSession vi, 
                                                              ViInt32 bufferSize, 
                                                              ViChar buffer[]);

typedef ViStatus (_VI_FUNC *ResetInterchangeCheckFuncPtr)(ViSession vi);

typedef ViStatus (_VI_FUNC *ClearInterchangeWarningsFuncPtr)(ViSession vi);
                                           
typedef ViStatus (_VI_FUNC *GetSpecificDriverCHandleFuncPtr)(ViSession vi, 
                                                             ViSession* specificHandle);

typedef ViStatus (_VI_FUNC *GetSpecificDriverIUnknownPtrFuncPtr)(ViSession vi,
                                                                 IUnknown **specificDriverIUnknownPtr);

typedef ViStatus (_VI_FUNC *InitSimStatusFuncPtr) (ViSession vi);
typedef ViStatus (_VI_FUNC *SimulationInitFuncPtr) (ViSession vi, 
                                                    ViConstString simulationSessionName, 
                                                    ViSession* sessionToSimulate);
typedef ViStatus (_VI_FUNC *SimulationCloseFuncPtr) (ViSession vi);

/****************************************************************************
 *  eldaq Function Names, Tables, and Typedefs
 ****************************************************************************/
enum
    {
	kConfigureMeasurement = kNumInherentFunctions,
	kWriteDigitalLine,
	kStartMeasurement,
	kStopMeasurement,
	kReadForce,
	kReadMeasurement,
	kGetChannelScale,
	kReadWaveform,
	kLoadCalibration,
    kNumFunctions
    };

static char *functionNames[kNumFunctions + 1] =
    {
    INHERENT_FUNCTION_NAMES,
	"ConfigureMeasurement",
	"WriteDigitalLine",
	"StartMeasurement",
	"StopMeasurement",
	"ReadForce",
	"ReadMeasurement",
	"GetChannelScale",
	"ReadWaveform",
	"LoadCalibration",
    VI_NULL
    };

static IviStringValueTable funcTable =
    {
    INHERENT_FUNCTION_TABLE(eldaq),
	{kConfigureMeasurement,		"eldaq_ConfigureMeasurement"},
	{kWriteDigitalLine,			"eldaq_WriteDigitalLine"},		
	{kStartMeasurement,		"eldaq_StartMeasurement"},		
	{kStopMeasurement,		"eldaq_StopMeasurement"},
	{kReadForce,				"eldaq_ReadForce"},		
	{kReadMeasurement,		"eldaq_ReadMeasurement"},
	{kGetChannelScale,		"eldaq_GetChannelScale"},
	{kReadWaveform,		"eldaq_ReadWaveform"},		
	{kLoadCalibration,		"eldaq_LoadCalibration"},		
    {VI_NULL, VI_NULL}
    };

/*- Function Pointer Typedefs -*/
typedef ViStatus (_VI_FUNC *ConfigureMeasurementFuncPtr) (ViSession vi,
                                              ViChar _VI_FAR measuredChannels[],
                                              ViInt32 sampleRate,
                                              ViReal64 measuredIntervals);

typedef ViStatus (_VI_FUNC *WriteDigitalLineFuncPtr) (ViSession vi,
                                              ViChar _VI_FAR Channels[],
											  ViInt32 status); 

typedef ViStatus (_VI_FUNC *GetChannelScaleFuncPtr) (ViSession vi,
                                              ViChar _VI_FAR channelName[],
                                              ViConstString _VI_FAR scaleName,
											  void* _VI_FAR _pScaleInfo);

typedef ViStatus (_VI_FUNC *ReadForceFuncPtr) (ViSession vi, ViChar channels[],
                                    ViChar clockSource[], ViReal64 min,
                                    ViReal64 max, ViInt32 sampsPerChan,
                                    ViInt32 rate, double data[]);


typedef ViStatus (_VI_FUNC *StartMeasurementFuncPtr) (ViSession vi);

typedef ViStatus (_VI_FUNC *StopMeasurementFuncPtr) (ViSession vi);

typedef ViStatus (_VI_FUNC *ReadMeasurementFuncPtr) (ViSession vi, 
										ViChar _VI_FAR channelName[],
										 void *scaleInfo,
                                         ViInt32 measType, 
										 ViPReal64 value);

typedef ViStatus (_VI_FUNC *ReadWaveformFuncPtr) (ViSession vi, ViChar _VI_FAR channelName[],
                                      ViChar _VI_FAR scale[], ViInt32 arraySize,
                                      ViReal64 _VI_FAR data[], ViPReal64 startTime,
                                      ViPReal64 xIncrement);
typedef ViStatus (_VI_FUNC *LoadCalibrationFuncPtr) (ViSession vi, ViConstString calibFile);



/****************************************************************************
 *  Macros and Functions for Class Simulation Support 
 ****************************************************************************/

/* These defined values are used for class simulation support */
#define DEFAULT_SIMULATION_DRIVER       ""
#define ELDAQ_SIMULATE_BEGIN(f)        IVI_SIMULATE_BEGIN(f, eldaq, ELDAQ)

#define SIMULATE_BEGIN(f)                                                           \
    {                                                                               \
        if (Ivi_Simulating(vi) && !Ivi_UseSpecificSimulation(vi))                   \
        {                                                                           \
            ViStatus  warning = error;                                              \
            ViSession simVi = 0;                                                    \
            f##FuncPtr simFunc = 0;                                                 \
            checkErr (ClassUtil_GetSimulationDriverInfo (vi, DEFAULT_SIMULATION_DRIVER, #f, &simVi, (ViAddr*)&simFunc, functionNames));           \
            if (simVi && simFunc) {                                                 \
                error = simFunc(simVi

#define SIMULATE_END()                                          \
        );}                                                     \
        if (error != VI_SUCCESS)                                \
            {                                                   \
            Ivi_SetErrorInfo(vi, VI_FALSE, error, 0, VI_NULL);  \
            if (error < VI_SUCCESS) goto Error;                 \
            }                                                   \
        if (warning != VI_SUCCESS)                              \
            error = warning;                                    \
        }                                                       \
    }
#define SIMULATE_ARG(a) ,a

//  Each class use the diffenent get attribute function.

#define IVI_SIMULATE_BEGIN(f, classLow, classUp)                                                    \
    {                                                                                   \
        ViBoolean simulationEnabled = 0;                                                \
        checkErr (ClassUtil_GetAttributeViBooleanFromSpecificDriver (vi, VI_NULL, classUp##_ATTR_SIMULATE, &simulationEnabled)); \
        if (simulationEnabled && !Ivi_UseSpecificSimulation(vi))                        \
            {                                                                           \
            ViStatus  warning = error;                                                  \
            ViSession simVi = 0;                                                        \
            f##FuncPtr simFunc = 0;                                                     \
            checkErr (ClassUtil_GetSimulationDriverInfo (vi, DEFAULT_SIMULATION_DRIVER, #f, &simVi, (ViAddr*)&simFunc, functionNames));           \
            if (simVi && simFunc) {                                                     \
                error = simFunc(simVi

/****************************************************************************
 *  Utility Macros
 ****************************************************************************/

/*- Locking Macros -*/
#define LOCK() error =  Ivi_LockSession(vi, &callerHasLock);
#define UNLOCK() Ivi_UnlockSession(vi, &callerHasLock);
/*- End Locking Macros -*/

/*- Specific Driver Call Macros -*/
#define DECLARE(f)             \
    ViStatus  error = VI_SUCCESS;       \
    ViBoolean callerHasLock = VI_FALSE; \
    f##FuncPtr functionPtr = VI_NULL;

#define DECLARE_NO_SPECIFIC_CALL() \
    ViStatus  error = VI_SUCCESS;           \
    ViBoolean callerHasLock = VI_FALSE;     \

#define DECLARE_NO_LOCK(f)     \
    ViStatus  error = VI_SUCCESS;       \
    f##FuncPtr functionPtr = VI_NULL;
    
#define DECLARE_INIT(f)        \
    ViStatus  error = VI_SUCCESS;       \
    f##FuncPtr functionPtr = VI_NULL;   \
    ViChar resourceName[256] = "";
    
#define RETURN()   \
    return error;
    
#define SPECIFIC_CALL_BEGIN(f)                                              \
    {                                                                       \
    checkErr( Ivi_GetFunctionPtr (vi, k##f, (ViAddr *)&functionPtr));       \
    error =  (*functionPtr)(Ivi_SpecificDriverSession(vi)
    
#define SPECIFIC_CALL_INIT_BEGIN(f,l)                                       \
    {                                                                       \
    checkErr( Ivi_GetFunctionPtr (vi, k##f, (ViAddr *)&functionPtr));       \
    error =  (*functionPtr)( (ViRsrc) l
    
#define SPECIFIC_CALL_END()                                     \
        );                                                      \
        if (error < VI_SUCCESS) goto Error;                     \
    }
            
#define SPECIFIC_CALL_ARG(a) ,a
/*- End Specific Driver Call Macros -*/

/*- Discard a function error -*/
#define noErr(f)    if (f < VI_SUCCESS) {Ivi_ClearErrorInfo(vi);}

/****************************************************************************
 *  ClassUtil Functions
 ****************************************************************************/

/*- Function Prototypes -*/
typedef ViStatus (*InitAttributesFuncPtr)(ViSession vi);
    
ViStatus ClassUtil_TestNewSession(ViSession* newVi, ViStatus secondError);

void ClassUtil_BuildRevision(ViString revString, 
                             ViChar* className, 
                             ViInt32 classMajor, 
                             ViInt32 classMinor);

ViStatus ClassUtil_GetCoercionRecord (ViSession vi, 
                                      ViInt32 bufferSize, 
                                      ViChar recordBuf[]);

ViStatus ClassUtil_GetNextInterchangeWarning (ViSession vi,
                                              ViInt32 bufferSize,
                                              ViChar warnString[]);

ViStatus ClassUtil_ClearInterchangeWarnings (ViSession vi);

ViStatus ClassUtil_ResetInterchangeCheck (ViSession vi);

ViStatus ClassUtil_GetError (ViSession vi, 
                             ViStatus *errorCode, 
                             ViInt32 bufferSize, 
                             ViChar description[],
                             IviStringValueTable errorMessageTable);

ViStatus ClassUtil_ClearError (ViSession vi);

ViStatus ClassUtil_error_message (ViSession vi,
                                  ViStatus statusCode,
                                  ViChar message[],
                                  IviStringValueTable ClassErrorMessagesTable);

ViBoolean ClassUtil_IsClassAttribute (ViInt32 attributeId);


ViStatus ClassUtil_GetSimulationDriverInfo   (ViSession vi, 
                                              ViConstString defaultSoftwareModule,
                                              ViConstString functionName,
                                              ViSession* simVi,
                                              ViAddr* functionPtr,
                                              ViString functionNames[]);

ViStatus ClassUtil_CreateNewSession (ViConstString logicalName,
                                     ViConstString optionString,
                                     ViBoolean idQuery, 
                                     ViBoolean resetDevice,
                                     ViSession* newSession,
                                     ViConstString className,
                                     ViString functionNames[],
                                     InitAttributesFuncPtr InitAttributes);

ViStatus ClassUtil_DisposeSession (ViSession vi);
ViStatus ClassUtil_GetAttributeViBooleanFromSpecificDriver (ViSession vi, 
                                                            ViConstString channelName, 
                                                            ViAttr attributeId, 
                                                            ViBoolean *value);


#define CLASSUTIL_DISPOSE_ON_ERROR()                                                    \
    if (error < VI_SUCCESS)                                                             \
    {                                                                                   \
        ClassUtil_DisposeSession (vi);                                                  \
        Ivi_Dispose (vi);                                                               \
        *newSession = VI_NULL;                                                          \
    }                                                                                   \

/****************************************************************************
 * Build Dll Main function
 ****************************************************************************/
#if 0
int __stdcall DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
        {
        case DLL_PROCESS_ATTACH:
            if (InitCVIRTE (hinstDLL, 0, 0) == 0)
                return 0;     /* out of memory */
            break;
        case DLL_PROCESS_DETACH:
            CloseCVIRTE ();
            break;
        }
    
    return 1;
}
#endif

/**************************************************************************** 
 *  User-callable Functions                                                 * 
 ****************************************************************************/

/**************************************************************************** 
 *  Function: eldaq_init                                                
 *  Purpose:  Open an instrument session, queries the instrument ID, and      
 *            initializes the instrument to a known state.                    
 ****************************************************************************/
ViStatus _VI_FUNC eldaq_init (ViRsrc logicalName, 
                               ViBoolean idQuery, 
                               ViBoolean resetDevice, 
                               ViSession* newSession)
{					   
    ViStatus  error = VI_SUCCESS;
    ViSession vi = VI_NULL;

    checkErr( ClassUtil_TestNewSession(newSession, VI_ERROR_PARAMETER5));
    checkErr( ClassUtil_CreateNewSession(logicalName, "", 
                               idQuery, resetDevice, newSession,
                               CLASS_NAME, functionNames,
                               InitAttributes));
    vi = *newSession;

    ELDAQ_SIMULATE_BEGIN(InitSimStatus)
    SIMULATE_END()

Error:
    CLASSUTIL_DISPOSE_ON_ERROR()
    RETURN()
}

/**************************************************************************** 
 *  Function: eldaq_InitWithOptions                                     
 *  Purpose:  Open an instrument session, queries the instrument ID, and      
 *            initializes the instrument to a known state.                    
 ****************************************************************************/
ViStatus _VI_FUNC eldaq_InitWithOptions (ViRsrc logicalName, 
                                          ViBoolean idQuery, 
                                          ViBoolean resetDevice,
                                          ViConstString optionString, 
                                          ViSession* newSession)
{
    ViStatus  error = VI_SUCCESS;
    ViSession vi = VI_NULL;

    checkErr( ClassUtil_TestNewSession(newSession, VI_ERROR_PARAMETER5));
    checkErr( ClassUtil_CreateNewSession(logicalName, optionString, 
                               idQuery, resetDevice, newSession,
                               CLASS_NAME, functionNames,
                               InitAttributes));
    vi = *newSession;

    ELDAQ_SIMULATE_BEGIN(InitSimStatus)
    SIMULATE_END()

Error:
    CLASSUTIL_DISPOSE_ON_ERROR()

    RETURN()
}

/**************************************************************************** 
 *  Function: eldaq_close                                                
 *  Purpose:  Close the instrument session.                                   
 ****************************************************************************/
ViStatus _VI_FUNC eldaq_close (ViSession vi)
{
    DECLARE_NO_SPECIFIC_CALL()

    LOCK()
    
    viCheckErr( Ivi_UnlockSession_Class (Ivi_SpecificDriverSession(vi), NULL));
    viCheckErr( ClassUtil_DisposeSession (vi));

Error:
    
    Ivi_UnlockSession_Class (vi, &callerHasLock);
    Ivi_Dispose(vi);
    RETURN()
}

/**************************************************************************** 
 *  Function: eldaq_reset                                                
 *  Purpose:  Reset the instrument.                                           
 *            If the reset function is not supported by the instrument,       
 *            this function returns the warning VI_WARN_NSUP_RESET.           
 ****************************************************************************/
ViStatus _VI_FUNC eldaq_reset (ViSession vi)
{
    DECLARE(reset)

    LOCK()
    
    SPECIFIC_CALL_BEGIN(reset)
    SPECIFIC_CALL_END()

    ELDAQ_SIMULATE_BEGIN(reset)
    SIMULATE_END()

Error:
    UNLOCK()
    RETURN()
}

/**************************************************************************** 
 *  Function: eldaq_self_test                                            
 *  Purpose:  Execute the instrument self-test and return the result.         
 *            If the self test function is not supported by the instrument,   
 *            this function returns the warning VI_WARN_NSUP_SELF_TEST.       
 ****************************************************************************/
ViStatus _VI_FUNC eldaq_self_test (ViSession vi, ViInt16* testResult, ViChar testMessage[])
{
    DECLARE(self_test)

    LOCK()

    SPECIFIC_CALL_BEGIN(self_test)
        SPECIFIC_CALL_ARG(testResult)
        SPECIFIC_CALL_ARG(testMessage)
    SPECIFIC_CALL_END()

    ELDAQ_SIMULATE_BEGIN(self_test)
        SIMULATE_ARG(testResult)
        SIMULATE_ARG(testMessage)
    SIMULATE_END()

Error:
    UNLOCK()
    RETURN()
}

/**************************************************************************** 
 *  Function: eldaq_error_query                                          
 *  Purpose:  Query the instrument error queue, and return the result.        
 *            If the error query is not supported by the instrument, this     
 *            function returns the warning VI_WARN_NSUP_ERROR_QUERY.          
 ****************************************************************************/
ViStatus _VI_FUNC eldaq_error_query (ViSession vi, ViInt32 *errorCode, ViChar errorMessage[])
{
    DECLARE(error_query)

    LOCK()

    SPECIFIC_CALL_BEGIN(error_query)
        SPECIFIC_CALL_ARG(errorCode)
        SPECIFIC_CALL_ARG(errorMessage)
    SPECIFIC_CALL_END()

    ELDAQ_SIMULATE_BEGIN(error_query)
        SIMULATE_ARG(errorCode)
        SIMULATE_ARG(errorMessage)
    SIMULATE_END()

Error:
    UNLOCK()
    RETURN()
}

/**************************************************************************** 
 *  Function: eldaq_error_message                                        
 *  Purpose:  Translate the error return value from the instrument into a     
 *            user-readable string.                                           
 ****************************************************************************/
ViStatus _VI_FUNC eldaq_error_message (ViSession vi, ViStatus statusCode, ViChar message[])
{
    DECLARE_NO_SPECIFIC_CALL()
    
    if (vi) 
        {
        LOCK()
        }
        
    checkErr( ClassUtil_error_message (vi, statusCode, message, ClassErrorMessagesTable));

    if (vi)
        {
        ELDAQ_SIMULATE_BEGIN(error_message)
            SIMULATE_ARG(statusCode)
            SIMULATE_ARG(message)
        SIMULATE_END()
        }
        
Error:
    if (vi)
        {
        UNLOCK()
        }
    RETURN()
}

/**************************************************************************** 
 *  Function: eldaq_revision_query                                       
 *  Purpose:  Return the driver and instrument revisions.                     
 *            If the revision query function is not supported by the          
 *            instrument, this function returns the warning                   
 *            VI_WARN_NSUP_REV_QUERY.                                         
 ****************************************************************************/
ViStatus _VI_FUNC eldaq_revision_query (ViSession vi, ViChar driverRev[], ViChar instrRev[])
{
    DECLARE(revision_query)

    LOCK()

    SPECIFIC_CALL_BEGIN(revision_query)
        SPECIFIC_CALL_ARG(driverRev)
        SPECIFIC_CALL_ARG(instrRev)
    SPECIFIC_CALL_END()

    ELDAQ_SIMULATE_BEGIN(revision_query)
        SIMULATE_ARG(driverRev)
        SIMULATE_ARG(instrRev)
    SIMULATE_END()

Error:

    UNLOCK()
    RETURN()
}

/****************************************************************************
 *  Function: eldaq_InvalidateAllAttributes
 *  Purpose:  This function invalidates the cached value of all attributes.
 ****************************************************************************/
ViStatus _VI_FUNC eldaq_InvalidateAllAttributes (ViSession vi)
{
    DECLARE(InvalidateAllAttributes)

    LOCK()

    SPECIFIC_CALL_BEGIN(InvalidateAllAttributes)
    SPECIFIC_CALL_END()

    ELDAQ_SIMULATE_BEGIN(InvalidateAllAttributes)
    SIMULATE_END()

Error:

    UNLOCK()
    RETURN()
}

/*****************************************************************************
 * Function: eldaq_ResetWithDefaults
 * Purpose:  This function resets the instrument and applies default settings
 *           from the IVI Configuration Store based on the logical name
 *           from which the session was created.
 *****************************************************************************/
ViStatus _VI_FUNC eldaq_ResetWithDefaults (ViSession vi)
{
    DECLARE(ResetWithDefaults)

    LOCK()

    SPECIFIC_CALL_BEGIN(ResetWithDefaults)
    SPECIFIC_CALL_END()

    ELDAQ_SIMULATE_BEGIN(ResetWithDefaults)
    SIMULATE_END()

Error:

    UNLOCK()
    RETURN()
}

/**************************************************************************** 
 *  Function: eldaq_Disable
 *  Purpose:  This function places the instrument in a quiescent state as 
 *            quickly as possible.
 ****************************************************************************/
ViStatus _VI_FUNC eldaq_Disable (ViSession vi)
{
    DECLARE(Disable)

    LOCK()

    SPECIFIC_CALL_BEGIN(Disable)
    SPECIFIC_CALL_END()

    ELDAQ_SIMULATE_BEGIN(Disable)
    SIMULATE_END()

Error:
    UNLOCK()
    RETURN()
}

/****************************************************************************
 *  Function: GetAttribute<type> Functions
 *  Purpose:  These functions are provided for the instrument driver user
 *            to get attribute values.  There are versions for ViInt32,
 *            ViReal64, ViString, ViBoolean, and ViSession attributes.
 ****************************************************************************/
ViStatus _VI_FUNC eldaq_GetAttributeViInt32 (ViSession vi, 
                                                ViConstString channelName, 
                                                ViAttr attributeId, 
                                                ViInt32 *value)
{
    DECLARE(GetAttributeViInt32)
    
    LOCK()

    if (ClassUtil_IsClassAttribute(attributeId))
    {
        checkErr (Ivi_GetAttributeViInt32(vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, value));
    }
    else
    {
        SPECIFIC_CALL_BEGIN(GetAttributeViInt32)
            SPECIFIC_CALL_ARG(channelName)
            SPECIFIC_CALL_ARG(attributeId)
            SPECIFIC_CALL_ARG(value)
        SPECIFIC_CALL_END ()
    }


Error:
    UNLOCK()
    RETURN()
}

ViStatus _VI_FUNC eldaq_GetAttributeViReal64 (ViSession vi, 
                                                 ViConstString channelName, 
                                                 ViAttr attributeId, 
                                                 ViReal64 *value)
{
    DECLARE(GetAttributeViReal64)
    
    LOCK()

    if (ClassUtil_IsClassAttribute(attributeId))
    {
        checkErr (Ivi_GetAttributeViReal64(vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, value));
    }
    else
    {
        SPECIFIC_CALL_BEGIN(GetAttributeViReal64)
            SPECIFIC_CALL_ARG(channelName)
            SPECIFIC_CALL_ARG(attributeId)
            SPECIFIC_CALL_ARG(value)
        SPECIFIC_CALL_END ()
    }


Error:

    UNLOCK()
    RETURN()
}

ViStatus _VI_FUNC eldaq_GetAttributeViString (ViSession vi, 
                                                 ViConstString channelName, 
                                                 ViAttr attributeId, 
                                                 ViInt32 bufSize, 
                                                 ViChar value[])
{
    DECLARE(GetAttributeViString)
    
    LOCK()

    if (ClassUtil_IsClassAttribute(attributeId))
    {
        /* Do not use checkErr here as checkErr will discard actual size value */
        error = Ivi_GetAttributeViString(vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, bufSize, value);
        if (error)
            goto Error; /* Either error or actual size will be returned */
    }
    else
    {
        SPECIFIC_CALL_BEGIN(GetAttributeViString)
            SPECIFIC_CALL_ARG(channelName)
            SPECIFIC_CALL_ARG(attributeId)
            SPECIFIC_CALL_ARG(bufSize)
            SPECIFIC_CALL_ARG(value)
        SPECIFIC_CALL_END ()
    }


Error:
    UNLOCK()
    RETURN()
}

ViStatus _VI_FUNC eldaq_GetAttributeViBoolean (ViSession vi, 
                                                  ViConstString channelName, 
                                                  ViAttr attributeId, 
                                                  ViBoolean *value)
{
    DECLARE(GetAttributeViBoolean)
    
    LOCK()

    if (ClassUtil_IsClassAttribute(attributeId))
    {
        checkErr (Ivi_GetAttributeViBoolean(vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, value));
    }
    else
    {
        SPECIFIC_CALL_BEGIN(GetAttributeViBoolean)
            SPECIFIC_CALL_ARG(channelName)
            SPECIFIC_CALL_ARG(attributeId)
            SPECIFIC_CALL_ARG(value)
        SPECIFIC_CALL_END ()
    }


Error:
    UNLOCK()
    RETURN()
}

ViStatus _VI_FUNC eldaq_GetAttributeViSession (ViSession vi, 
                                                  ViConstString channelName, 
                                                  ViAttr attributeId, 
                                                  ViSession *value)
{
    DECLARE(GetAttributeViSession)
    
    LOCK()

    if (ClassUtil_IsClassAttribute(attributeId))
    {
        checkErr (Ivi_GetAttributeViSession(vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, value));
    }
    else
    {
        SPECIFIC_CALL_BEGIN(GetAttributeViSession)
            SPECIFIC_CALL_ARG(channelName)
            SPECIFIC_CALL_ARG(attributeId)
            SPECIFIC_CALL_ARG(value)
        SPECIFIC_CALL_END ()
    }


Error:
    UNLOCK()
    RETURN()
}

/****************************************************************************
 *  Function: SetAttribute<type> Functions
 *  Purpose:  These functions are provided for the instrument driver user
 *            to set attribute values.  There are versions for ViInt32,
 *            ViReal64, ViString, ViBoolean, and ViSession attributes.
 ****************************************************************************/
ViStatus _VI_FUNC eldaq_SetAttributeViInt32 (ViSession vi, 
                                                ViConstString channelName, 
                                                ViAttr attributeId, 
                                                ViInt32 value)
{
    DECLARE(SetAttributeViInt32)
    
    LOCK()

    if (ClassUtil_IsClassAttribute(attributeId))
    {
        checkErr (Ivi_SetAttributeViInt32(vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, value));
    }
    else
    {
        SPECIFIC_CALL_BEGIN(SetAttributeViInt32)
            SPECIFIC_CALL_ARG(channelName)
            SPECIFIC_CALL_ARG(attributeId)
            SPECIFIC_CALL_ARG(value)
        SPECIFIC_CALL_END ()
    }


Error:
    UNLOCK()
    RETURN()
}

ViStatus _VI_FUNC eldaq_SetAttributeViReal64 (ViSession vi, 
                                                 ViConstString channelName, 
                                                 ViAttr attributeId, 
                                                 ViReal64 value)
{
    DECLARE(SetAttributeViReal64)
    
    LOCK()

    if (ClassUtil_IsClassAttribute(attributeId))
    {
        checkErr (Ivi_SetAttributeViReal64(vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, value));
    }
    else
    {
        SPECIFIC_CALL_BEGIN(SetAttributeViReal64)
            SPECIFIC_CALL_ARG(channelName)
            SPECIFIC_CALL_ARG(attributeId)
            SPECIFIC_CALL_ARG(value)
        SPECIFIC_CALL_END ()
    }


Error:
    UNLOCK()
    RETURN()
}

ViStatus _VI_FUNC eldaq_SetAttributeViString (ViSession vi, 
                                                 ViConstString channelName, 
                                                 ViAttr attributeId, 
                                                 ViConstString value)
{
    DECLARE(SetAttributeViString)
    
    LOCK()

    if (ClassUtil_IsClassAttribute(attributeId))
    {
        checkErr (Ivi_SetAttributeViString(vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, value));
    }
    else
    {
        SPECIFIC_CALL_BEGIN(SetAttributeViString)
            SPECIFIC_CALL_ARG(channelName)
            SPECIFIC_CALL_ARG(attributeId)
            SPECIFIC_CALL_ARG(value)
        SPECIFIC_CALL_END ()
    }


Error:
    UNLOCK()
    RETURN()
}

ViStatus _VI_FUNC eldaq_SetAttributeViBoolean (ViSession vi, 
                                                  ViConstString channelName, 
                                                  ViAttr attributeId, 
                                                  ViBoolean value)
{
    DECLARE(SetAttributeViBoolean)
    
    LOCK()

    if (ClassUtil_IsClassAttribute(attributeId))
    {
        checkErr (Ivi_SetAttributeViBoolean(vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, value));
    }
    else
    {
        SPECIFIC_CALL_BEGIN(SetAttributeViBoolean)
            SPECIFIC_CALL_ARG(channelName)
            SPECIFIC_CALL_ARG(attributeId)
            SPECIFIC_CALL_ARG(value)
        SPECIFIC_CALL_END ()
    }


Error:
    UNLOCK()
    RETURN()
}

ViStatus _VI_FUNC eldaq_SetAttributeViSession (ViSession vi, 
                                                  ViConstString channelName, 
                                                  ViAttr attributeId, 
                                                  ViSession value)
{
    DECLARE(SetAttributeViSession)
    
    LOCK()

    if (ClassUtil_IsClassAttribute(attributeId))
    {
        checkErr (Ivi_SetAttributeViSession(vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, value));
    }
    else
    {
        SPECIFIC_CALL_BEGIN(SetAttributeViSession)
            SPECIFIC_CALL_ARG(channelName)
            SPECIFIC_CALL_ARG(attributeId)
            SPECIFIC_CALL_ARG(value)
        SPECIFIC_CALL_END ()
    }

Error:
    UNLOCK()
    RETURN()
}

/****************************************************************************
 *  Function: CheckAttribute<type> Functions
 *  Purpose:  These functions are provided for the instrument driver user
 *            to check attribute values.
 ****************************************************************************/
ViStatus _VI_FUNC eldaq_CheckAttributeViInt32 (ViSession vi, 
                                                  ViConstString channelName, 
                                                  ViAttr attributeId, 
                                                  ViInt32 value)
{
    DECLARE(CheckAttributeViInt32)
    
    LOCK()

    if (ClassUtil_IsClassAttribute(attributeId))
    {
        checkErr (Ivi_CheckAttributeViInt32(vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, value));
    }
    else
    {
        SPECIFIC_CALL_BEGIN(CheckAttributeViInt32)
            SPECIFIC_CALL_ARG(channelName)
            SPECIFIC_CALL_ARG(attributeId)
            SPECIFIC_CALL_ARG(value)
        SPECIFIC_CALL_END ()
    }


Error:
    UNLOCK()
    RETURN()
}

ViStatus _VI_FUNC eldaq_CheckAttributeViReal64 (ViSession vi, 
                                                   ViConstString channelName, 
                                                   ViAttr attributeId, 
                                                   ViReal64 value)
{
    DECLARE(CheckAttributeViReal64)
    
    LOCK()

    if (ClassUtil_IsClassAttribute(attributeId))
    {
        checkErr (Ivi_CheckAttributeViReal64(vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, value));
    }
    else
    {
        SPECIFIC_CALL_BEGIN(CheckAttributeViReal64)
            SPECIFIC_CALL_ARG(channelName)
            SPECIFIC_CALL_ARG(attributeId)
            SPECIFIC_CALL_ARG(value)
        SPECIFIC_CALL_END ()
    }


Error:
    UNLOCK()
    RETURN()
}

ViStatus _VI_FUNC eldaq_CheckAttributeViString (ViSession vi, ViConstString channelName, ViAttr attributeId, ViConstString value)
{
    DECLARE(CheckAttributeViString)
    
    LOCK()

    if (ClassUtil_IsClassAttribute(attributeId))
    {
        checkErr (Ivi_CheckAttributeViString(vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, value));
    }
    else
    {
        SPECIFIC_CALL_BEGIN(CheckAttributeViString)
            SPECIFIC_CALL_ARG(channelName)
            SPECIFIC_CALL_ARG(attributeId)
            SPECIFIC_CALL_ARG(value)
        SPECIFIC_CALL_END ()
    }

Error:
    UNLOCK()
    RETURN()
}

ViStatus _VI_FUNC eldaq_CheckAttributeViBoolean (ViSession vi, ViConstString channelName, ViAttr attributeId, ViBoolean value)
{
    DECLARE(CheckAttributeViBoolean)
    
    LOCK()

    if (ClassUtil_IsClassAttribute(attributeId))
    {
        checkErr (Ivi_CheckAttributeViBoolean(vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, value));
    }
    else
    {
        SPECIFIC_CALL_BEGIN(CheckAttributeViBoolean)
            SPECIFIC_CALL_ARG(channelName)
            SPECIFIC_CALL_ARG(attributeId)
            SPECIFIC_CALL_ARG(value)
        SPECIFIC_CALL_END ()
    }

Error:
    UNLOCK()
    RETURN()
}

ViStatus _VI_FUNC eldaq_CheckAttributeViSession (ViSession vi, ViConstString channelName, ViAttr attributeId, ViSession value)
{
    DECLARE(CheckAttributeViSession)
    
    LOCK()

    if (ClassUtil_IsClassAttribute(attributeId))
    {
        checkErr (Ivi_CheckAttributeViSession(vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, value));
    }
    else
    {
        SPECIFIC_CALL_BEGIN(CheckAttributeViSession)
            SPECIFIC_CALL_ARG(channelName)
            SPECIFIC_CALL_ARG(attributeId)
            SPECIFIC_CALL_ARG(value)
        SPECIFIC_CALL_END ()
    }

Error:
    UNLOCK()
    RETURN()
}

/**************************************************************************** 
 *  Functions: LockSession
 *  Purpose:   This function is provided to guarantee exclusive access     
 *             to an instrument.                                              
 ****************************************************************************/
ViStatus _VI_FUNC eldaq_LockSession (ViSession vi, 
                                        ViBoolean *callerHasLock)
{
    DECLARE_NO_LOCK(LockSession)

    checkErr( Ivi_LockSession(vi, callerHasLock));

Error:
    RETURN()
}

/**************************************************************************** 
 *  Functions: UnlockSession
 *  Purpose:   This function is provided to guarantee exclusive access     
 *             to an instrument.                                              
 ****************************************************************************/
ViStatus _VI_FUNC eldaq_UnlockSession (ViSession vi, 
                                          ViBoolean *callerHasLock)
{
    DECLARE_NO_LOCK(UnlockSession)

    checkErr( Ivi_UnlockSession (vi, callerHasLock));

Error:
    RETURN()
}

/**************************************************************************** 
 *  Function: eldaq_GetError 
 *  Purpose:  This function is provided for the instrument driver user     
 *            to Get the error information associated with an IVI    
 *            session.                                                        
 ****************************************************************************/
ViStatus  _VI_FUNC eldaq_GetError (ViSession vi, 
                                      ViStatus *errorCode, 
                                      ViInt32 bufferSize, 
                                      ViChar description[])
{
    DECLARE_NO_SPECIFIC_CALL()

    if (vi)
    {
        LOCK()
    }        

    /* Do not use checkErr here as checkErr will discard actual size value */
    error = ClassUtil_GetError(vi, errorCode,bufferSize, description, ClassErrorMessagesTable);
    if (error)  /* Either error or actual size will be returned */
        goto Error;

Error:
    if (vi)
    {
        UNLOCK()
    }
    RETURN()
}

/**************************************************************************** 
 *  Function: eldaq_ClearError
 *  Purpose:  This function is provided for the instrument driver user     
 *            to Clear the error information associated with an IVI    
 *            session.                                                        
 ****************************************************************************/
ViStatus  _VI_FUNC eldaq_ClearError (ViSession vi)
{
    DECLARE(ClearError)

    if (vi)
    {
        LOCK()
    }
        
    checkErr( ClassUtil_ClearError (vi));
        
Error:
    if (vi)
    {
        UNLOCK()
    }
    RETURN()
}

/**************************************************************************** 
 *  Function: GetErrorInfo 
 *  Purpose:  This function is provided for the instrument driver user     
 *            to Get the error information associated with an IVI    
 *            session.                                                        
 ****************************************************************************/
ViStatus  _VI_FUNC eldaq_GetErrorInfo (ViSession vi, 
                                          ViStatus *primaryError, 
                                          ViStatus *secondaryError, 
                                          ViChar errorElaboration[IVI_MAX_MESSAGE_BUF_SIZE])
{
    DECLARE(GetErrorInfo)
    
    if (vi) 
        {
        LOCK()
        
        SPECIFIC_CALL_BEGIN(GetErrorInfo)
            SPECIFIC_CALL_ARG(primaryError)
            SPECIFIC_CALL_ARG(secondaryError)
            SPECIFIC_CALL_ARG(errorElaboration)
        SPECIFIC_CALL_END()
        }
    else
        {
        checkErr (Ivi_GetErrorInfo (vi, primaryError, secondaryError, errorElaboration));
        }
    
Error:
    if (vi) 
        {
        UNLOCK()
        }
    RETURN()
}

/**************************************************************************** 
 *  Function: ClearErrorInfo
 *  Purpose:  This function is provided for the instrument driver user     
 *            to Clear the error information associated with an IVI    
 *            session.                                                        
 ****************************************************************************/
ViStatus  _VI_FUNC eldaq_ClearErrorInfo (ViSession vi)
{
    DECLARE(ClearErrorInfo)

    if (vi)
        {
        LOCK()
        
        SPECIFIC_CALL_BEGIN(ClearErrorInfo)
        SPECIFIC_CALL_END()
        }
    else
        {
        checkErr (Ivi_ClearErrorInfo (vi));
        }
        
Error:
    if (vi)
        {
        UNLOCK()
        }
    RETURN()
}

/**************************************************************************** 
 *  Function: GetNextCoercionRecord                               
 *  Purpose:  Get the next coercion record on the queue.                      
 ****************************************************************************/
ViStatus _VI_FUNC eldaq_GetNextCoercionRecord (ViSession vi, 
                                                  ViInt32 bufferSize, 
                                                  ViChar recordBuf[])
{
    DECLARE_NO_SPECIFIC_CALL()
    
    LOCK()

    /* Do not use checkErr here as checkErr will discard actual size value */
    error = ClassUtil_GetCoercionRecord (vi, bufferSize, recordBuf);
    if (error)  /* Either error or actual size will be returned */
        goto Error;

Error:
    UNLOCK()
    RETURN()
}

/**************************************************************************** 
 *  Function: GetSpecificDriverCHandle                               
 *  Purpose:  Returns the session of the specific driver.                      
 ****************************************************************************/
ViStatus _VI_FUNC eldaq_GetSpecificDriverCHandle (ViSession vi,
                                                   ViSession* specificHandle)
{
    DECLARE_NO_SPECIFIC_CALL()
    
    LOCK()

    if (specificHandle) {
        *specificHandle = Ivi_SpecificDriverSession(vi);
    }

Error:
    UNLOCK()
    RETURN()
}

/**************************************************************************** 
 *  Function: GetSpecificDriverIUnknownPtr
 *  Purpose:  Get the IUnknown Pointer for the underlying COM component.
 ****************************************************************************/
ViStatus _VI_FUNC eldaq_GetSpecificDriverIUnknownPtr (ViSession vi, void* ref)
{
    DECLARE(GetSpecificDriverIUnknownPtr)
    IUnknown** specificDriverIUnknownPtr = ref;
    
    LOCK()

    if (specificDriverIUnknownPtr) {
        *specificDriverIUnknownPtr = VI_NULL;
        error = Ivi_GetFunctionPtr (vi, kGetSpecificDriverIUnknownPtr, (ViAddr *)&functionPtr);
        if (functionPtr == VI_NULL)
        {
            Ivi_ClearErrorInfo(vi);
        }
        else
        {
            error =  functionPtr(Ivi_SpecificDriverSession(vi), specificDriverIUnknownPtr);
            if (error < VI_SUCCESS) goto Error;
        }
    }

Error:
    UNLOCK()
    RETURN()
}

/**************************************************************************** 
 *  Function: GetNextInterchangeWarning                            
 *  Purpose:  Get the next interchangeability warning on the queue.           
 ****************************************************************************/
ViStatus _VI_FUNC eldaq_GetNextInterchangeWarning (ViSession vi, 
                                                      ViInt32 bufferSize, 
                                                      ViChar warnString[])
{
    DECLARE_NO_SPECIFIC_CALL()
    
    LOCK()

    /* Do not use checkErr here as checkErr will discard actual size value */
    error = ClassUtil_GetNextInterchangeWarning(vi, bufferSize, warnString);
    if (error)  /* Either error or actual size will be returned */
        goto Error;

Error:
    UNLOCK()
    RETURN()
}

/**************************************************************************** 
 *  Function: ResetInterchangeCheck                            
 *  Purpose:  Set all attributes as not set by user.           
 ****************************************************************************/
ViStatus _VI_FUNC eldaq_ResetInterchangeCheck (ViSession vi)
{
    DECLARE_NO_SPECIFIC_CALL()
    
    LOCK()

    checkErr( ClassUtil_ResetInterchangeCheck (vi));

Error:
    UNLOCK()
    RETURN()
}

/**************************************************************************** 
 *  Function: ClearInterchangeWarnings                            
 *  Purpose:  Clears the Queue of Interchange Warnings.
 ****************************************************************************/
ViStatus _VI_FUNC eldaq_ClearInterchangeWarnings (ViSession vi)
{
    DECLARE_NO_SPECIFIC_CALL()
    
    LOCK()

    checkErr( ClassUtil_ClearInterchangeWarnings (vi));

Error:
    UNLOCK()
    RETURN()
}

/****************************************************************************
 *  Class-specific Function Definitions
 ****************************************************************************/
/****************************************************************************
 *  Function: eldaq_ConfigureMeasurement
 *  Purpose: 
 ****************************************************************************/
ViStatus _VI_FUNC eldaq_ConfigureMeasurement (ViSession vi,
                                              ViChar _VI_FAR measuredChannels[],
                                              ViInt32 sampleRate,
                                              ViReal64 measuredIntervals)
{
	DECLARE(ConfigureMeasurement)

	LOCK()

	SPECIFIC_CALL_BEGIN(ConfigureMeasurement)
		SPECIFIC_CALL_ARG(measuredChannels)
		SPECIFIC_CALL_ARG(sampleRate)
		SPECIFIC_CALL_ARG(measuredIntervals)
	SPECIFIC_CALL_END()

	ELDAQ_SIMULATE_BEGIN(ConfigureMeasurement)
		SIMULATE_ARG(measuredChannels)
		SIMULATE_ARG(sampleRate)
		SIMULATE_ARG(measuredIntervals)

	SIMULATE_END()

Error:
	UNLOCK()
	RETURN()
}

/****************************************************************************
 *  Function: eldaq_StartMeasurement
 *  Purpose: 
 ****************************************************************************/
ViStatus _VI_FUNC eldaq_StartMeasurement (ViSession vi)
{
	DECLARE(StartMeasurement)

	LOCK()

	SPECIFIC_CALL_BEGIN(StartMeasurement)
	SPECIFIC_CALL_END()

	ELDAQ_SIMULATE_BEGIN(StartMeasurement)

	SIMULATE_END()

Error:
	UNLOCK()
	RETURN()
}

/****************************************************************************
 *  Function: eldaq_StopMeasurement
 *  Purpose: 
 ****************************************************************************/
ViStatus _VI_FUNC eldaq_StopMeasurement (ViSession vi)
{
	DECLARE(StopMeasurement)

	LOCK()

	SPECIFIC_CALL_BEGIN(StopMeasurement)
	SPECIFIC_CALL_END()

	ELDAQ_SIMULATE_BEGIN(StopMeasurement)

	SIMULATE_END()

Error:
	UNLOCK()
	RETURN()
}

/****************************************************************************
 *  Function: eldaq_ReadMeasurement
 *  Purpose: 
 ****************************************************************************/
ViStatus _VI_FUNC eldaq_ReadMeasurement (ViSession vi, ViChar _VI_FAR channelName[],
										 void * _VI_FAR scaleInfo,
                                         ViInt32 measurementType, ViPReal64 value)
{
	DECLARE(ReadMeasurement)

	LOCK()

	SPECIFIC_CALL_BEGIN(ReadMeasurement)
		SPECIFIC_CALL_ARG(channelName)
		SPECIFIC_CALL_ARG(scaleInfo)
		SPECIFIC_CALL_ARG(measurementType)
		SPECIFIC_CALL_ARG(value)
	SPECIFIC_CALL_END()

Error:
	UNLOCK()
	RETURN()
}

/****************************************************************************
 *  Function: eldaq_ReadWaveform
 *  Purpose: 
 ****************************************************************************/
ViStatus _VI_FUNC eldaq_ReadWaveform (ViSession vi, ViChar _VI_FAR channelName[],
                                      ViChar _VI_FAR scale[], ViInt32 arraySize,
                                      ViReal64 _VI_FAR data[], ViPReal64 startTime,
                                      ViPReal64 xIncrement)
{
	DECLARE(ReadWaveform)

	LOCK()

	SPECIFIC_CALL_BEGIN(ReadWaveform)
		SPECIFIC_CALL_ARG(channelName)
		SPECIFIC_CALL_ARG(scale)
		SPECIFIC_CALL_ARG(arraySize)
		SPECIFIC_CALL_ARG(data)
		SPECIFIC_CALL_ARG(startTime)
		SPECIFIC_CALL_ARG(xIncrement)
	SPECIFIC_CALL_END()

	ELDAQ_SIMULATE_BEGIN(ReadWaveform)
		SIMULATE_ARG(channelName)
		SIMULATE_ARG(scale)
		SIMULATE_ARG(arraySize)
		SIMULATE_ARG(data)
		SIMULATE_ARG(startTime)
		SIMULATE_ARG(xIncrement)

	SIMULATE_END()

Error:
	UNLOCK()
	RETURN()
}

ViStatus _VI_FUNC eldaq_ReadForce (ViSession vi, ViChar channels[],
                                    ViChar clockSource[], ViReal64 min,
                                    ViReal64 max, ViInt32 sampsPerChan,
                                    ViInt32 rate, double data[])

{
	DECLARE(ReadForce)

	LOCK()

	SPECIFIC_CALL_BEGIN(ReadForce)
		SPECIFIC_CALL_ARG(channels)
		SPECIFIC_CALL_ARG(clockSource)
		SPECIFIC_CALL_ARG(min)
		SPECIFIC_CALL_ARG(max)
		SPECIFIC_CALL_ARG(sampsPerChan)
		SPECIFIC_CALL_ARG(rate)
		SPECIFIC_CALL_ARG(data)
	SPECIFIC_CALL_END()

	ELDAQ_SIMULATE_BEGIN(ReadForce)
		SIMULATE_ARG(channels)
		SIMULATE_ARG(clockSource)
		SIMULATE_ARG(min)
		SIMULATE_ARG(max)
		SIMULATE_ARG(sampsPerChan)
		SIMULATE_ARG(rate)
		SIMULATE_ARG(data)
	SIMULATE_END()

Error:
	UNLOCK()
	RETURN()
}

ViStatus _VI_FUNC eldaq_WriteDigitalLine (ViSession vi,
                                          ViChar digitalChannels[],
										  ViInt32 status)
{
	DECLARE(WriteDigitalLine)

	LOCK()

	SPECIFIC_CALL_BEGIN(WriteDigitalLine)
		SPECIFIC_CALL_ARG(digitalChannels)
		SPECIFIC_CALL_ARG(status)
	SPECIFIC_CALL_END()

	ELDAQ_SIMULATE_BEGIN(WriteDigitalLine)
		SIMULATE_ARG(digitalChannels)
		SIMULATE_ARG(status)
	SIMULATE_END()

Error:
	UNLOCK()
	RETURN()
}
ViStatus _VI_FUNC eldaq_GetChannelScale (ViSession vi,
                                         ViChar channelName[],
                                         ViConstString scaleName,
                                         void *_pScaleInfo)
{
	DECLARE(GetChannelScale)

	LOCK()

	SPECIFIC_CALL_BEGIN(GetChannelScale)
		SPECIFIC_CALL_ARG(channelName)
		SPECIFIC_CALL_ARG(scaleName)
		SPECIFIC_CALL_ARG(_pScaleInfo)
	SPECIFIC_CALL_END()

	ELDAQ_SIMULATE_BEGIN(GetChannelScale)
		SIMULATE_ARG(channelName)
		SIMULATE_ARG(scaleName)
		SIMULATE_ARG(_pScaleInfo)
	SIMULATE_END()

Error:
	UNLOCK()
	RETURN()    
}
/****************************************************************************
 *  Function: eldaq_LoadCalibration
 *  Purpose: 
 ****************************************************************************/
ViStatus _VI_FUNC eldaq_LoadCalibration (ViSession vi, ViConstString calibrationFile)
{
	DECLARE(LoadCalibration)

	LOCK()

	SPECIFIC_CALL_BEGIN(LoadCalibration)
		SPECIFIC_CALL_ARG(calibrationFile)
	SPECIFIC_CALL_END()

	ELDAQ_SIMULATE_BEGIN(LoadCalibration)
		SIMULATE_ARG(calibrationFile)
	SIMULATE_END()

Error:
	UNLOCK()
	RETURN()
}




/**************************************************************************** 
 *  Capability Callback Functions                                           * 
 ****************************************************************************/
    /*----------------------------------------------------------------------*/
    /*  Function and Attribute Capability Callbacks                         */
    /*----------------------------------------------------------------------*/
static ViStatus _VI_FUNC FuncCapabilities_ReadCallback (ViSession vi, 
                                                        ViSession io, 
                                                        ViConstString channelName, 
                                                        ViAttr attributeId, 
                                                        const ViConstString value)
{
    ViStatus error = VI_SUCCESS;
    ViChar   newValue[4096] = "";

    checkErr( Ivi_ClassFunctionCapabilities (vi, funcTable, newValue, VI_NULL));
    checkErr( Ivi_SetValInStringCallback (vi, attributeId, newValue));
Error:
    return error;
}

/***************************************************************************** 
 *  Function: eldaqClassRevision_ReadCallback                                
 *  Purpose:  Read callback for ELDAQ_ATTR_CLASS_REVISION          
 *****************************************************************************/
static ViStatus _VI_FUNC ClassRevision_ReadCallback (ViSession vi, 
                                                     ViSession io, 
                                                     ViConstString channelName, 
                                                     ViAttr attributeId, 
                                                     const ViConstString value)
{
    ViStatus error = VI_NULL;
    ViChar   newValue[256];

    ClassUtil_BuildRevision(newValue, 
                  CLASS_NAME, 
                  ELDAQ_MAJOR_VERSION, 
                  ELDAQ_MINOR_VERSION);
    checkErr( Ivi_SetValInStringCallback (vi, attributeId, newValue));
Error:
    return error;
}

/**************************************************************************** 
 *  Utility Routines                                                        * 
 ****************************************************************************/
/**************************************************************************** 
 *  Function: InitAttributes                                       
 *  Purpose:  Initialize attributes that are required to be set by the        
 *            class driver and add private attributes needed by the class.    
 ****************************************************************************/
static ViStatus InitAttributes (ViSession vi)
{
    ViStatus error = VI_SUCCESS;

        /* Attributes required to be set by the class */
    checkErr( Ivi_SetAttrReadCallbackViString (vi, ELDAQ_ATTR_FUNCTION_CAPABILITIES,
                                               FuncCapabilities_ReadCallback));
    
    checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL,
                                       IVI_ATTR_CLASS_DRIVER_MAJOR_VERSION, 
                                       0, ELDAQ_MAJOR_VERSION));
    checkErr( Ivi_SetAttributeViInt32 (vi, VI_NULL,
                                       IVI_ATTR_CLASS_DRIVER_MINOR_VERSION, 
                                       0, ELDAQ_MINOR_VERSION));
    checkErr( Ivi_SetAttributeViString (vi, VI_NULL, 
                                       ELDAQ_ATTR_CLASS_DRIVER_VENDOR, 
                                       0, ELDAQ_DRIVER_VENDOR));
    checkErr( Ivi_SetAttributeViString (vi, VI_NULL, 
                                       ELDAQ_ATTR_CLASS_DRIVER_DESCRIPTION, 
                                       0, ELDAQ_DRIVER_DESCRIPTION));
    checkErr( Ivi_SetAttrReadCallbackViString (vi, ELDAQ_ATTR_CLASS_DRIVER_REVISION, 
                                               ClassRevision_ReadCallback));

Error:
    return error;
}

/**************************************************************************** 
 *  ClassUtil Routines                                                        * 
 ****************************************************************************/

#define version(f, g) f+(g/1000.0)

/**************************************************************************** 
 *  Function: ClassUtil_TestNewSession                                  
 *  Purpose:  If a new session pointer is null, this function returns 
 *            an appropriate error.
 ****************************************************************************/
ViStatus ClassUtil_TestNewSession(ViSession* newVi, ViStatus secondError)
{
    if (newVi == VI_NULL)
            {
            Ivi_SetErrorInfo (VI_NULL, VI_FALSE, IVI_ERROR_INVALID_PARAMETER,
                              secondError, "Null address for Instrument Handle");
            return IVI_ERROR_INVALID_PARAMETER;
            }
        *newVi = VI_NULL;
        return VI_SUCCESS;
}

/**************************************************************************** 
 *  Function: ClassUtil_BuildRevision
 *  Purpose:  Builds a class driver's revision string, depending on 
 ****************************************************************************/
void ClassUtil_BuildRevision(ViString revString, 
                   ViChar* className, 
                   ViInt32 classMajor, 
                   ViInt32 classMinor)
{                               
    sprintf (revString,
             "Class: %s %.2f, Compiler: %s %3.2f, Components: IVIEngine %.2f, VISA-Spec %.2f",
             className, version(classMajor, classMinor), 
             IVI_COMPILER_NAME, IVI_COMPILER_VER_NUM,
             version(IVI_ENGINE_MAJOR_VERSION, IVI_ENGINE_MINOR_VERSION),
             Ivi_ConvertVISAVer(VI_SPEC_VERSION));
    return;
}

/**************************************************************************** 
 *  Function: ClassUtil_GetCoercionRecord
 *  Purpose:  Gets the coercion record from the specific driver. This utility
 *            function is to be used both by the class driver GetCoercionRecord
 *            function and the Spy support function BuildCoercionRecord
 ****************************************************************************/
ViStatus ClassUtil_GetCoercionRecord (ViSession vi, 
                                      ViInt32 bufferSize, 
                                      ViChar recordBuf[])
{
    DECLARE_NO_LOCK(GetNextCoercionRecord)

    SPECIFIC_CALL_BEGIN(GetNextCoercionRecord)
        SPECIFIC_CALL_ARG(bufferSize)
        SPECIFIC_CALL_ARG(recordBuf)
    SPECIFIC_CALL_END()

Error:
    RETURN()
}

/****************************************************************************
 *  Function: ClassUtil_GetNextInterchangeWarning
 *  Purpose:  Gets the next interchange warning from the soecific driver.
 *            This utility function is to be used both by the class drivers
 *            GetNextInterchangeWarning function and the Spy support function
 *            BuildIntchWarning
 ****************************************************************************/
ViStatus ClassUtil_GetNextInterchangeWarning (ViSession vi,
                                              ViInt32 bufferSize,
                                              ViChar warnString[])
{
    DECLARE_NO_LOCK(GetNextInterchangeWarning)
    
    if (warnString != NULL)
        warnString[0] = '\0';

    SPECIFIC_CALL_BEGIN(GetNextInterchangeWarning)
        SPECIFIC_CALL_ARG(bufferSize)
        SPECIFIC_CALL_ARG(warnString)
    SPECIFIC_CALL_END()

    if ((error == VI_SUCCESS) && ((warnString == NULL) || (warnString[0] == '\0')))
    {
        /* Do not use checkErr here as Ivi_GetNextInterchangeCheckString might return actual buffer size if the input buffer size is 0 */
        error = Ivi_GetNextInterchangeCheckString(vi, bufferSize, warnString);
        if (error)
            goto Error; /* Either error or actual size will be returned */
    }

Error:
    RETURN()
}

/****************************************************************************
 *  Function: ClassUtil_ClearInterchangeWarnings
 *  Purpose:  Clears interchange warnings for the specific driver and class
 *            driver.
 ****************************************************************************/
ViStatus ClassUtil_ClearInterchangeWarnings (ViSession vi)
{
    DECLARE_NO_LOCK(ClearInterchangeWarnings)

    SPECIFIC_CALL_BEGIN(ClearInterchangeWarnings)
    SPECIFIC_CALL_END()

    checkErr( Ivi_ClearInterchangeWarnings(vi));

Error:
    RETURN()
}

/****************************************************************************
 *  Function: ClassUtil_ResetInterchangeCheck
 *  Purpose:  Resets interchange checking for the specific driver and class
 *            driver.
 ****************************************************************************/
ViStatus ClassUtil_ResetInterchangeCheck (ViSession vi)
{
    DECLARE_NO_LOCK(ResetInterchangeCheck)

    SPECIFIC_CALL_BEGIN(ResetInterchangeCheck)
    SPECIFIC_CALL_END()

    checkErr( Ivi_ResetInterchangeCheck(vi));

Error:
    RETURN()
}

/**************************************************************************** 
 *  Function: ClassUtil_GetError 
 *  Purpose:  This function is provided for the instrument driver user     
 *            to Get the error information associated with an IVI    
 *            session.                                                        
 ****************************************************************************/
ViStatus  ClassUtil_GetError (ViSession vi, 
                              ViStatus *errorCode, 
                              ViInt32 bufferSize, 
                              ViChar description[],
                              IviStringValueTable errorMessageTable)
{
    ViSession   specific = VI_NULL;

    DECLARE_NO_LOCK(GetError)
    
    if (vi != VI_NULL)
    {
        SPECIFIC_CALL_BEGIN(GetError)
            SPECIFIC_CALL_ARG(errorCode)
            SPECIFIC_CALL_ARG(bufferSize)
            SPECIFIC_CALL_ARG(description)
        SPECIFIC_CALL_END()
    }
    else
        *errorCode = VI_SUCCESS;

    // If no specific driver error occurred, then get the class driver error
    if (*errorCode == VI_SUCCESS)
    {
        ViInt32 primaryError = VI_SUCCESS, 
                secondaryError = VI_SUCCESS,
                actualSize = 0;
        ViChar  errorElaboration[256] = "",
                errorMessage[IVI_MAX_MESSAGE_BUF_SIZE] = "",
                *appendPoint = errorMessage;

        /* Test for nulls and acquire error data */
        if (bufferSize != 0)
            {
            if (errorCode == VI_NULL)
                {
                viCheckParm( IVI_ERROR_INVALID_PARAMETER, 2, "Null address for Error");
                }
            if (description == VI_NULL)
                {
                viCheckParm( IVI_ERROR_INVALID_PARAMETER, 4, "Null address for Description");
                }
            checkErr( Ivi_GetErrorInfo (vi, &primaryError, &secondaryError, errorElaboration));
            }
        else
            {
            checkErr( Ivi_GetAttributeViString(vi, VI_NULL, IVI_ATTR_ERROR_ELABORATION, 0, 256, errorElaboration));
            checkErr( Ivi_GetAttributeViInt32(vi, VI_NULL, IVI_ATTR_SECONDARY_ERROR, 0, &secondaryError));
            checkErr( Ivi_GetAttributeViInt32(vi, VI_NULL, IVI_ATTR_PRIMARY_ERROR, 0, &primaryError));
            }
        
        /* Format data */
        if (primaryError != VI_SUCCESS)
            {
            ViChar msg[256] = "";
            checkErr( Ivi_GetClassDriverStatusDesc (vi, primaryError, msg, errorMessageTable));
            appendPoint += sprintf(appendPoint, "Primary Error: (Hex 0x%08X) %s", primaryError, msg);
            }
    
        if (secondaryError != VI_SUCCESS)
            {
            ViChar msg[256] = "";
            checkErr( Ivi_GetClassDriverStatusDesc (vi, primaryError, msg, errorMessageTable));
            appendPoint += sprintf(appendPoint, "\nSecondary Error: (Hex 0x%08X) %s", secondaryError, msg);
            }
    
        if (errorElaboration[0])
            {
            sprintf(appendPoint, "\nElaboration: %s", errorElaboration);
            }
    
        actualSize = strlen(errorMessage) + 1;
        
        /* Prepare return values */
        if (bufferSize == 0)
            {
            error = actualSize;
            }
        else 
            {
            if (bufferSize > 0)
                {
                if (actualSize > bufferSize)
                    {
                    error = actualSize;
                    actualSize = bufferSize;
                    }
                }
            memcpy(description, errorMessage, actualSize-1);
            description[actualSize-1] = '\0';
            }
        
        if (errorCode) 
            {
            *errorCode = primaryError;
            }
    }
    
Error:
    RETURN()
}

/**************************************************************************** 
 *  Function: ClassUtil_ClearError 
 *  Purpose:  This function is provided for the instrument driver user     
 *            to clear the error queue on the session
 ****************************************************************************/
ViStatus  ClassUtil_ClearError (ViSession vi)
{
    DECLARE_NO_LOCK(ClearError)
 
    if (vi != VI_NULL)
    {
        SPECIFIC_CALL_BEGIN(ClearError)
        SPECIFIC_CALL_END()
    }
    
    checkErr( Ivi_ClearErrorInfo (vi));

Error:
    RETURN()
}

/****************************************************************************/          
/*  Function: ClassUtil_error_message                                       */          
/*  Purpose:  Handles error message handling for class & specific drivers   */
/****************************************************************************/          
ViStatus ClassUtil_error_message (ViSession vi,
                                  ViStatus statusCode,
                                  ViChar message[],
                                  IviStringValueTable ClassErrorMessagesTable)
{
    DECLARE_NO_LOCK(error_message)
    message[0]='\0';

    if (vi == VI_NULL)
        checkErr( Ivi_GetClassDriverStatusDesc (VI_NULL, statusCode, message, ClassErrorMessagesTable));
    else
    {
        SPECIFIC_CALL_BEGIN(error_message)
            SPECIFIC_CALL_ARG(statusCode)
            SPECIFIC_CALL_ARG(message)
        SPECIFIC_CALL_END()
    }

    if (message[0] == '\0')
        checkErr( Ivi_GetClassDriverStatusDesc (vi, statusCode, message, ClassErrorMessagesTable));

Error:
    RETURN()
}

/****************************************************************************/          
/*  Function: ClassUtil_IsClassAttribute                                    */          
/*  Purpose:  Returns if the specified attribute ID is a class attribute    */
/****************************************************************************/          
static ViInt32  classAttrIds[] = {
    IVI_ATTR_CLASS_DRIVER_PREFIX,
    IVI_ATTR_CLASS_DRIVER_VENDOR,
    IVI_ATTR_CLASS_DRIVER_DESCRIPTION,
    IVI_ATTR_CLASS_DRIVER_CLASS_SPEC_MAJOR_VERSION,
    IVI_ATTR_CLASS_DRIVER_CLASS_SPEC_MINOR_VERSION,
    IVI_ATTR_CLASS_DRIVER_REVISION,
    IVI_ATTR_SPECIFIC_DRIVER_LOCATOR,
    IVI_ATTR_CLASS_DRIVER_MAJOR_VERSION,
    IVI_ATTR_CLASS_DRIVER_MINOR_VERSION,
    IVI_ATTR_SPY,
    IVI_ATTR_FUNCTION_CAPABILITIES,
    IVI_ATTR_USE_SPECIFIC_SIMULATION,       // Add this attribute because the IVI Engine treat this attribute as a class attribute.
    VI_NULL};

ViBoolean ClassUtil_IsClassAttribute (ViInt32 attributeId)
{
    ViInt32     i = 0;

    for (i=0; classAttrIds[i]!=VI_NULL; i++)
    {
        if (attributeId == classAttrIds[i])
            return VI_TRUE;
    }    

    return VI_FALSE;
}

/****************************************************************************/          
/*  Function: GetSimulationDriverVi                                         */          
/*  Purpose:  Loads simulation driver if not already loaded.                */          
/*            Call this only if simulating in class.                        */          
/****************************************************************************/          
ViStatus ClassUtil_GetSimulationDriverInfo (
    ViSession vi, 
    ViConstString defaultSoftwareModule,
    ViConstString functionName,
    ViSession* simVi,
    ViAddr* functionPtr,
    ViString functionNames[])
{                                                                                       
    ViStatus error = VI_SUCCESS;                                                        
    
    viCheckErr( Ivi_LoadSimulationDriver(vi, defaultSoftwareModule, functionNames));
    viCheckErr( Ivi_GetSimulationSession(vi, simVi));
    
    Ivi_GetSimulationDriverFunctionPtr(vi, functionName, functionPtr);
    /* The error returned by this function will be ignored.  When there is an error,
       the "functionPtr" will be NULL.  And then, the class driver will only call the
       specific simulation dirver for this function and ignore the class simulation
       driver.  See CAR 2ZN43UCE.                                                     */

Error:                                                                                  
    if (error < VI_SUCCESS) {
        *simVi = 0;
        *functionPtr = 0;
    }
    return error;
}

/**************************************************************************** 
 *  Function: ClassUtil_CreateNewSession                                              
 *  Purpose:  Call the specific driver's initialize function.                 
 ****************************************************************************/
ViStatus ClassUtil_CreateNewSession (ViConstString logicalName,
                                     ViConstString optionString,
                                     ViBoolean idQuery, 
                                     ViBoolean resetDevice,
                                     ViSession* newSession,
                                     ViConstString className,
                                     ViString functionNames[],
                                     InitAttributesFuncPtr InitAttributes)
{
    DECLARE_INIT(InitWithOptions)
    ViSession   vi = VI_NULL,
                specificVi = VI_NULL;
    
    /* Instantiate the class driver portion of the session */
    checkErr( Ivi_ClassDriverNew (logicalName, className, functionNames, &vi));
    checkErr( InitAttributes (vi));
    
    /* Instantiate the specific driver portion of the session */
    SPECIFIC_CALL_INIT_BEGIN(InitWithOptions, logicalName)
        SPECIFIC_CALL_ARG(idQuery)
        SPECIFIC_CALL_ARG(resetDevice)
        SPECIFIC_CALL_ARG(optionString)
        SPECIFIC_CALL_ARG(&specificVi)
    SPECIFIC_CALL_END()
    
    checkErr( Ivi_SetSpecificDriverSession(vi, specificVi));

Error:
    if (error < VI_SUCCESS)
        {
        ClassUtil_DisposeSession (vi);
        Ivi_Dispose (vi);
        }
    else if (newSession)
        {
        *newSession = vi;
        }
    
    RETURN()
}

/**************************************************************************** 
 *  Function: ClassUtil_DisposeSession                                             
 *  Purpose:  Perform all driver clean-up operations except closing the       
 *            IVI session.                                                    
 ****************************************************************************/
ViStatus ClassUtil_DisposeSession (ViSession vi)
{
    DECLARE_NO_LOCK(close)
    
    /* Dispose of the specific driver session */
    SPECIFIC_CALL_BEGIN(close)
    SPECIFIC_CALL_END()

    /* Invalidate the class driver session reference to the specific driver session */
    Ivi_SetSpecificDriverSession (vi, 0);

    /* Dispose of the simulation session */
    Ivi_CloseSimulationSession(vi);

Error:
    RETURN()
}

/**************************************************************************** 
 *  Function: ClassUtil_GetAttributeViBooleanFromSpecificDriver                                             
 *  Purpose:  Gets ViBoolean attribute value from specific driver.
 ****************************************************************************/
ViStatus ClassUtil_GetAttributeViBooleanFromSpecificDriver (ViSession vi, 
                                                            ViConstString channelName, 
                                                            ViAttr attributeId, 
                                                            ViBoolean *value)
{
    DECLARE_NO_LOCK(GetAttributeViBoolean)
    
    SPECIFIC_CALL_BEGIN(GetAttributeViBoolean)
            SPECIFIC_CALL_ARG(channelName)
            SPECIFIC_CALL_ARG(attributeId)
            SPECIFIC_CALL_ARG(value)
    SPECIFIC_CALL_END () 

Error:
    RETURN()
}
