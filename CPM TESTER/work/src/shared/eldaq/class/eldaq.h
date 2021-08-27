#include <cvidef.h>

/****************************************************************************
 * Title:       eldaq.h                                                    
 * Purpose:     eldaq Class declarations.                        
 ****************************************************************************/

#ifndef ELDAQ_HEADER
#define ELDAQ_HEADER

#include <ivi.h>

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/****************************************************************************
 *----------------- Instrument Driver Revision Information -----------------*
 ****************************************************************************/

#define ELDAQ_MAJOR_VERSION                (3L)
#define ELDAQ_MINOR_VERSION                (0L)

#define ELDAQ_CLASS_SPEC_MAJOR_VERSION     (3L)
#define ELDAQ_CLASS_SPEC_MINOR_VERSION     (0L)

#define ELDAQ_DRIVER_VENDOR                "ELCOM, a.s."
#define ELDAQ_DRIVER_DESCRIPTION           "eldaq Class Driver"

/****************************************************************************
 *------------------------------ Useful Macros -----------------------------*
 ****************************************************************************/
    /*- Defined values for maxTime parameter to the Measure, Read -*/
    /*- and Fetch functions -*/
#define ELDAQ_VAL_MAX_TIME_INFINITE    IVI_VAL_MAX_TIME_INFINITE
#define ELDAQ_VAL_MAX_TIME_IMMEDIATE   IVI_VAL_MAX_TIME_IMMEDIATE

/****************************************************************************
 *--------------------- eldaq Class Attribute Defines ---------------------*
 ****************************************************************************/

    /*- IVI Inherent Attributes -*/

        /*- User Options -*/
#define ELDAQ_ATTR_CACHE                     IVI_ATTR_CACHE                       /* ViBoolean */
#define ELDAQ_ATTR_RANGE_CHECK               IVI_ATTR_RANGE_CHECK                 /* ViBoolean */
#define ELDAQ_ATTR_QUERY_INSTRUMENT_STATUS   IVI_ATTR_QUERY_INSTRUMENT_STATUS     /* ViBoolean */
#define ELDAQ_ATTR_RECORD_COERCIONS          IVI_ATTR_RECORD_COERCIONS            /* ViBoolean */
#define ELDAQ_ATTR_SIMULATE                  IVI_ATTR_SIMULATE                    /* ViBoolean */
#define ELDAQ_ATTR_INTERCHANGE_CHECK         IVI_ATTR_INTERCHANGE_CHECK           /* ViBoolean */
#define ELDAQ_ATTR_SPY                       IVI_ATTR_SPY                         /* ViBoolean */
#define ELDAQ_ATTR_USE_SPECIFIC_SIMULATION   IVI_ATTR_USE_SPECIFIC_SIMULATION     /* ViBoolean */

        /*- Instrument Capabilities -*/
#define ELDAQ_ATTR_GROUP_CAPABILITIES        IVI_ATTR_GROUP_CAPABILITIES          /* ViString, read-only */
#define ELDAQ_ATTR_FUNCTION_CAPABILITIES     IVI_ATTR_FUNCTION_CAPABILITIES       /* ViString, read-only */

        /*- Class Driver Information -*/    
#define ELDAQ_ATTR_CLASS_DRIVER_PREFIX                     IVI_ATTR_CLASS_DRIVER_PREFIX         /* ViString, read-only */
#define ELDAQ_ATTR_CLASS_DRIVER_VENDOR                     IVI_ATTR_CLASS_DRIVER_VENDOR         /* ViString, read-only */
#define ELDAQ_ATTR_CLASS_DRIVER_DESCRIPTION                IVI_ATTR_CLASS_DRIVER_DESCRIPTION    /* ViString, read-only */
#define ELDAQ_ATTR_CLASS_DRIVER_CLASS_SPEC_MAJOR_VERSION   IVI_ATTR_CLASS_DRIVER_CLASS_SPEC_MAJOR_VERSION  /* ViInt32,  read-only */
#define ELDAQ_ATTR_CLASS_DRIVER_CLASS_SPEC_MINOR_VERSION   IVI_ATTR_CLASS_DRIVER_CLASS_SPEC_MINOR_VERSION  /* ViInt32,  read-only */

        /*- Specific Driver Information -*/    
#define ELDAQ_ATTR_SPECIFIC_DRIVER_PREFIX                      IVI_ATTR_SPECIFIC_DRIVER_PREFIX      /* ViString, read-only */
#define ELDAQ_ATTR_SPECIFIC_DRIVER_LOCATOR                     IVI_ATTR_SPECIFIC_DRIVER_LOCATOR     /* ViString, read-only */
#define ELDAQ_ATTR_IO_RESOURCE_DESCRIPTOR                      IVI_ATTR_IO_RESOURCE_DESCRIPTOR      /* ViString, read-only */
#define ELDAQ_ATTR_LOGICAL_NAME                                IVI_ATTR_LOGICAL_NAME                /* ViString, read-only */
#define ELDAQ_ATTR_SPECIFIC_DRIVER_VENDOR                      IVI_ATTR_SPECIFIC_DRIVER_VENDOR      /* ViString, read-only */
#define ELDAQ_ATTR_SPECIFIC_DRIVER_DESCRIPTION                 IVI_ATTR_SPECIFIC_DRIVER_DESCRIPTION /* ViString, read-only */
#define ELDAQ_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION    IVI_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION  /* ViInt32,  read-only */
#define ELDAQ_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION    IVI_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION  /* ViInt32,  read-only */

        /*- Instrument Information -*/    
#define ELDAQ_ATTR_INSTRUMENT_FIRMWARE_REVISION     IVI_ATTR_INSTRUMENT_FIRMWARE_REVISION /* ViString, read-only */
#define ELDAQ_ATTR_INSTRUMENT_MANUFACTURER          IVI_ATTR_INSTRUMENT_MANUFACTURER      /* ViString, read-only */
#define ELDAQ_ATTR_INSTRUMENT_MODEL                 IVI_ATTR_INSTRUMENT_MODEL             /* ViString, read-only */
#define ELDAQ_ATTR_SUPPORTED_INSTRUMENT_MODELS      IVI_ATTR_SUPPORTED_INSTRUMENT_MODELS

        /*- Version Information -*/
#define ELDAQ_ATTR_CLASS_DRIVER_REVISION            IVI_ATTR_CLASS_DRIVER_REVISION              /* ViString, read-only */

#define ELDAQ_ATTR_SPECIFIC_DRIVER_REVISION         IVI_ATTR_SPECIFIC_DRIVER_REVISION             /* ViString, read-only */

        /*- Driver Setup Information -*/
#define ELDAQ_ATTR_DRIVER_SETUP              IVI_ATTR_DRIVER_SETUP                /* ViString */
	
#define ELDAQ_ATTR_CALIBRATION_FILE_NAME  (IVI_CLASS_PUBLIC_ATTR_BASE + 1L)		/* ViString (Read Only) */
	
	

        
        /* Instrument specific attribute value definitions */
#define ELDAQ_VAL_MEASUREMENT_RMS		1
#define ELDAQ_VAL_MEASUREMENT_MIN		2
#define ELDAQ_VAL_MEASUREMENT_MAX		3
#define ELDAQ_VAL_MEASUREMENT_MEAN		4

/****************************************************************************
 *----------------- eldaq Class Attribute Value Defines -------------------*
 ****************************************************************************/


/****************************************************************************
 *--------- eldaq Class Instrument Driver Function Declarations -*
 ****************************************************************************/
    /*- Required VXIplug&play Functions -*/
ViStatus _VI_FUNC eldaq_init (ViRsrc logicalName, 
                               ViBoolean idQuery, 
                               ViBoolean resetDevice, 
                               ViSession *vi);

ViStatus _VI_FUNC eldaq_close (ViSession vi);

ViStatus _VI_FUNC eldaq_reset (ViSession vi);

ViStatus _VI_FUNC eldaq_self_test (ViSession vi, 
                                    ViInt16 *selfTestResult, 
                                    ViChar selfTestMessage[]);

ViStatus _VI_FUNC eldaq_error_query (ViSession vi, 
                                      ViInt32 *errorCode, 
                                      ViChar errorMessage[]);

ViStatus _VI_FUNC eldaq_revision_query (ViSession vi, 
                                         ViChar driverRev[], 
                                         ViChar instrRev[]);

ViStatus _VI_FUNC eldaq_error_message (ViSession vi, 
                                        ViStatus statusCode, 
                                        ViChar message[]);

    /*- Utility Functions -*/
ViStatus _VI_FUNC eldaq_InvalidateAllAttributes (ViSession vi);

ViStatus _VI_FUNC eldaq_ResetWithDefaults (ViSession vi);

ViStatus _VI_FUNC eldaq_Disable (ViSession vi);

    /*- Required IVI Functions -*/
ViStatus _VI_FUNC eldaq_InitWithOptions (ViRsrc logicalName, 
                                          ViBoolean IDQuery,
                                          ViBoolean resetDevice, 
                                          ViConstString optionString, 
                                          ViSession *vi);

    /*- Set, Get, and Check Attribute Functions -*/
ViStatus _VI_FUNC eldaq_GetAttributeViInt32 (ViSession vi, 
                                              ViConstString channelName,
                                              ViAttr attributeId,
                                              ViInt32 *value);

ViStatus _VI_FUNC eldaq_SetAttributeViInt32 (ViSession vi, 
                                              ViConstString channelName,
                                              ViAttr attributeId,
                                              ViInt32 value);

ViStatus _VI_FUNC eldaq_CheckAttributeViInt32 (ViSession vi, 
                                                ViConstString channelName,
                                                ViAttr attributeId,
                                                ViInt32 value);

ViStatus _VI_FUNC eldaq_GetAttributeViReal64 (ViSession vi, 
                                               ViConstString channelName, 
                                               ViAttr attributeId, 
                                               ViReal64 *value);

ViStatus _VI_FUNC eldaq_SetAttributeViReal64 (ViSession vi, 
                                               ViConstString channelName, 
                                               ViAttr attributeId, 
                                               ViReal64 value);

ViStatus _VI_FUNC eldaq_CheckAttributeViReal64 (ViSession vi, 
                                                 ViConstString channelName, 
                                                 ViAttr attributeId, 
                                                 ViReal64 value);

ViStatus _VI_FUNC eldaq_GetAttributeViString (ViSession vi, 
                                               ViConstString channelName, 
                                               ViAttr attributeId, 
                                               ViInt32 bufferSize, 
                                               ViChar value[]);

ViStatus _VI_FUNC eldaq_SetAttributeViString (ViSession vi, 
                                               ViConstString channelName, 
                                               ViAttr attributeId, 
                                               ViConstString value);

ViStatus _VI_FUNC eldaq_CheckAttributeViString (ViSession vi, 
                                                 ViConstString channelName, 
                                                 ViAttr attributeId, 
                                                 ViConstString value); 


ViStatus _VI_FUNC eldaq_GetAttributeViBoolean (ViSession vi, 
                                                ViConstString channelName, 
                                                ViAttr attributeId, 
                                                ViBoolean *value);

ViStatus _VI_FUNC eldaq_SetAttributeViBoolean (ViSession vi, 
                                                ViConstString channelName, 
                                                ViAttr attributeId, 
                                                ViBoolean value);

ViStatus _VI_FUNC eldaq_CheckAttributeViBoolean (ViSession vi, 
                                                  ViConstString channelName, 
                                                  ViAttr attributeId, 
                                                  ViBoolean value);

ViStatus _VI_FUNC eldaq_GetAttributeViSession (ViSession vi, 
                                                ViConstString channelName, 
                                                ViAttr attributeId, 
                                                ViSession *value);

ViStatus _VI_FUNC eldaq_SetAttributeViSession (ViSession vi, 
                                                ViConstString channelName, 
                                                ViAttr attributeId, 
                                                ViSession value);

ViStatus _VI_FUNC eldaq_CheckAttributeViSession (ViSession vi, 
                                                  ViConstString channelName, 
                                                  ViAttr attributeId, 
                                                  ViSession value);

    /*- Lock and Unlock Functions -*/
ViStatus _VI_FUNC eldaq_LockSession (ViSession vi, 
                                      ViBoolean *callerHasLock);

ViStatus _VI_FUNC eldaq_UnlockSession (ViSession vi, 
                                        ViBoolean *callerHasLock);

    /*- Error Information Functions -*/
ViStatus _VI_FUNC eldaq_GetError (ViSession vi, 
                                   ViStatus *errorCode, 
                                   ViInt32 bufferSize, 
                                   ViChar description[]);

ViStatus _VI_FUNC eldaq_ClearError (ViSession vi);

    /*- Interchangeability Checking Functions -*/
ViStatus _VI_FUNC eldaq_GetNextInterchangeWarning (ViSession vi, 
                                                    ViInt32 bufferSize,
                                                    ViChar warning[]);

ViStatus _VI_FUNC eldaq_ClearInterchangeWarnings (ViSession vi);
ViStatus _VI_FUNC eldaq_ResetInterchangeCheck (ViSession vi);

ViStatus _VI_FUNC eldaq_GetNextCoercionRecord (ViSession vi,
                                                ViInt32 bufferSize, 
                                                ViChar record[]);

   /*- Class Driver Functions (not exported by specific driver) -*/
ViStatus _VI_FUNC eldaq_GetSpecificDriverCHandle (ViSession vi,
                                                   ViSession* specificDriverCHandle);

ViStatus _VI_FUNC eldaq_GetSpecificDriverIUnknownPtr (ViSession vi,
                                                       void* specificDriverIUnknownPtr);

    /*- eldaq Functions -*/
ViStatus _VI_FUNC eldaq_ConfigureMeasurement (ViSession vi,
                                              ViChar _VI_FAR measuredChannels[],
                                              ViInt32 sampleRate,
                                              ViReal64 measuredIntervals);

ViStatus _VI_FUNC eldaq_StartMeasurement (ViSession vi);

ViStatus _VI_FUNC eldaq_StopMeasurement (ViSession vi);

ViStatus _VI_FUNC eldaq_ReadMeasurement (ViSession vi, ViChar _VI_FAR channelName[],
										 void * _VI_FAR scaleInfo,
                                         ViInt32 measurementType, ViPReal64 value);

ViStatus _VI_FUNC eldaq_ReadWaveform (ViSession vi, ViChar _VI_FAR channelName[],
                                      ViChar _VI_FAR scale[], ViInt32 arraySize,
                                      ViReal64 _VI_FAR data[], ViPReal64 startTime,
                                      ViPReal64 xIncrement);

ViStatus _VI_FUNC eldaq_WriteDigitalLine (ViSession vi,
                                          ViChar digitalChannels[],
										  ViInt32 status);

ViStatus _VI_FUNC eldaq_GetChannelScale (ViSession vi,
                                         ViChar channelName[],
                                         ViConstString scaleName,
                                         void *_pScaleInfo);

ViStatus _VI_FUNC eldaq_ReadForce (ViSession vi, ViChar channels[],
                                    ViChar clockSource[], ViReal64 min,
                                    ViReal64 max, ViInt32 sampsPerChan,
                                    ViInt32 rate, double data[]);

ViStatus _VI_FUNC eldaq_LoadCalibration(
	ViSession		vi,
	ViConstString	calibrationFile
);


                                                
/****************************************************************************
 *----------------- eldaq Class Error And Completion Codes ----------------*
 ****************************************************************************/
    
#define ELDAQ_ERROR_MEASUREMENT_IS_RUNNING          (IVI_CLASS_ERROR_BASE + 1)
#define ELDAQ_ERROR_MEASUREMENT_IS_NOT_CONFIGURED   (IVI_CLASS_ERROR_BASE + 2)
#define ELDAQ_ERROR_LINEAR_CALIBRATION_CONFIG		(IVI_CLASS_ERROR_BASE + 3)

#define ELDAQ_ERROR_NOT_SPECIFIED_CALIBRATION_TYPE  (IVI_CLASS_ERROR_BASE + 4)
#define ELDAQ_ERROR_NOT_SUPPORTED_CALIBRATION_TYPE  (IVI_CLASS_ERROR_BASE + 5)


#define ELDAQ_ERRMSG_MEASUREMENT_IS_RUNNING         "Measurement is running"
#define ELDAQ_ERRMSG_MEASUREMENT_IS_NOT_CONFIGURED         "Measurement is not configured"
#define ELDAQ_ERRMSG_LINEAR_CALIBRATION_CONFIG         "Linera configuration error"
#define ELDAQ_ERRMSG_NOT_SPECIFIED_CALIBRATION_TYPE         "Not specified calibration"
#define ELDAQ_ERRMSG_NOT_SUPPORTED_CALIBRATION_TYPE         "Not supported calibration"

#define ELDAQ_ERROR_CODES_AND_MSGS \
	{ELDAQ_ERROR_MEASUREMENT_IS_RUNNING, ELDAQ_ERRMSG_MEASUREMENT_IS_RUNNING},\
	{ELDAQ_ERROR_MEASUREMENT_IS_NOT_CONFIGURED, ELDAQ_ERRMSG_MEASUREMENT_IS_NOT_CONFIGURED},\
	{ELDAQ_ERROR_LINEAR_CALIBRATION_CONFIG, ELDAQ_ERRMSG_LINEAR_CALIBRATION_CONFIG},\
	{ELDAQ_ERROR_NOT_SPECIFIED_CALIBRATION_TYPE, ELDAQ_ERRMSG_NOT_SPECIFIED_CALIBRATION_TYPE},\
	{ELDAQ_ERROR_NOT_SUPPORTED_CALIBRATION_TYPE, ELDAQ_ERRMSG_NOT_SUPPORTED_CALIBRATION_TYPE},\
	{ELDAQ_ERROR_MEASUREMENT_IS_RUNNING, ELDAQ_ERRMSG_MEASUREMENT_IS_RUNNING},\
	{VI_NULL, VI_NULL}

/****************************************************************************
 *---------------------------- End Include File ----------------------------*
 ****************************************************************************/

#if defined(__cplusplus) || defined(__cplusplus__)
}                                                    
#endif

#endif /* ELDAQ_HEADER */

