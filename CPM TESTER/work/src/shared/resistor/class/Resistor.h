/****************************************************************************
 * Title:       Resistor.h                                                    
 * Purpose:     Resistor Class declarations.                        
 ****************************************************************************/

#ifndef RESISTOR_HEADER
#define RESISTOR_HEADER

#include <ivi.h>

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/****************************************************************************
 *----------------- Instrument Driver Revision Information -----------------*
 ****************************************************************************/

#define RESISTOR_MAJOR_VERSION                (3L)
#define RESISTOR_MINOR_VERSION                (0L)

#define RESISTOR_CLASS_SPEC_MAJOR_VERSION     (3L)
#define RESISTOR_CLASS_SPEC_MINOR_VERSION     (0L)

#define RESISTOR_DRIVER_VENDOR                "ELCOM, a.s."
#define RESISTOR_DRIVER_DESCRIPTION           "Resistor Class Driver"

/****************************************************************************
 *------------------------------ Useful Macros -----------------------------*
 ****************************************************************************/
        /*=CHANGE:===============================================================
           Remove these definitions if your class does not have a Measure, Read,
           or Fetch function that does not contain a maxTime parameter
         *============================================================END=CHANGE=*/
    /*- Defined values for maxTime parameter to the Measure, Read -*/
    /*- and Fetch functions -*/
#define RESISTOR_VAL_MAX_TIME_INFINITE    IVI_VAL_MAX_TIME_INFINITE
#define RESISTOR_VAL_MAX_TIME_IMMEDIATE   IVI_VAL_MAX_TIME_IMMEDIATE

/****************************************************************************
 *--------------------- Resistor Class Attribute Defines ---------------------*
 ****************************************************************************/

    /*- IVI Inherent Attributes -*/

        /*- User Options -*/
#define RESISTOR_ATTR_CACHE                     IVI_ATTR_CACHE                       /* ViBoolean */
#define RESISTOR_ATTR_RANGE_CHECK               IVI_ATTR_RANGE_CHECK                 /* ViBoolean */
#define RESISTOR_ATTR_QUERY_INSTRUMENT_STATUS   IVI_ATTR_QUERY_INSTRUMENT_STATUS     /* ViBoolean */
#define RESISTOR_ATTR_RECORD_COERCIONS          IVI_ATTR_RECORD_COERCIONS            /* ViBoolean */
#define RESISTOR_ATTR_SIMULATE                  IVI_ATTR_SIMULATE                    /* ViBoolean */
#define RESISTOR_ATTR_INTERCHANGE_CHECK         IVI_ATTR_INTERCHANGE_CHECK           /* ViBoolean */
#define RESISTOR_ATTR_SPY                       IVI_ATTR_SPY                         /* ViBoolean */
#define RESISTOR_ATTR_USE_SPECIFIC_SIMULATION   IVI_ATTR_USE_SPECIFIC_SIMULATION     /* ViBoolean */

        /*- Instrument Capabilities -*/
#define RESISTOR_ATTR_GROUP_CAPABILITIES        IVI_ATTR_GROUP_CAPABILITIES          /* ViString, read-only */
#define RESISTOR_ATTR_FUNCTION_CAPABILITIES     IVI_ATTR_FUNCTION_CAPABILITIES       /* ViString, read-only */

        /*- Class Driver Information -*/    
#define RESISTOR_ATTR_CLASS_DRIVER_PREFIX                     IVI_ATTR_CLASS_DRIVER_PREFIX         /* ViString, read-only */
#define RESISTOR_ATTR_CLASS_DRIVER_VENDOR                     IVI_ATTR_CLASS_DRIVER_VENDOR         /* ViString, read-only */
#define RESISTOR_ATTR_CLASS_DRIVER_DESCRIPTION                IVI_ATTR_CLASS_DRIVER_DESCRIPTION    /* ViString, read-only */
#define RESISTOR_ATTR_CLASS_DRIVER_CLASS_SPEC_MAJOR_VERSION   IVI_ATTR_CLASS_DRIVER_CLASS_SPEC_MAJOR_VERSION  /* ViInt32,  read-only */
#define RESISTOR_ATTR_CLASS_DRIVER_CLASS_SPEC_MINOR_VERSION   IVI_ATTR_CLASS_DRIVER_CLASS_SPEC_MINOR_VERSION  /* ViInt32,  read-only */

        /*- Specific Driver Information -*/    
#define RESISTOR_ATTR_SPECIFIC_DRIVER_PREFIX                      IVI_ATTR_SPECIFIC_DRIVER_PREFIX      /* ViString, read-only */
#define RESISTOR_ATTR_SPECIFIC_DRIVER_LOCATOR                     IVI_ATTR_SPECIFIC_DRIVER_LOCATOR     /* ViString, read-only */
#define RESISTOR_ATTR_IO_RESOURCE_DESCRIPTOR                      IVI_ATTR_IO_RESOURCE_DESCRIPTOR      /* ViString, read-only */
#define RESISTOR_ATTR_LOGICAL_NAME                                IVI_ATTR_LOGICAL_NAME                /* ViString, read-only */
#define RESISTOR_ATTR_SPECIFIC_DRIVER_VENDOR                      IVI_ATTR_SPECIFIC_DRIVER_VENDOR      /* ViString, read-only */
#define RESISTOR_ATTR_SPECIFIC_DRIVER_DESCRIPTION                 IVI_ATTR_SPECIFIC_DRIVER_DESCRIPTION /* ViString, read-only */
#define RESISTOR_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION    IVI_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION  /* ViInt32,  read-only */
#define RESISTOR_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION    IVI_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION  /* ViInt32,  read-only */

        /*- Instrument Information -*/    
#define RESISTOR_ATTR_INSTRUMENT_FIRMWARE_REVISION     IVI_ATTR_INSTRUMENT_FIRMWARE_REVISION /* ViString, read-only */
#define RESISTOR_ATTR_INSTRUMENT_MANUFACTURER          IVI_ATTR_INSTRUMENT_MANUFACTURER      /* ViString, read-only */
#define RESISTOR_ATTR_INSTRUMENT_MODEL                 IVI_ATTR_INSTRUMENT_MODEL             /* ViString, read-only */
#define RESISTOR_ATTR_SUPPORTED_INSTRUMENT_MODELS      IVI_ATTR_SUPPORTED_INSTRUMENT_MODELS

        /*- Version Information -*/
#define RESISTOR_ATTR_CLASS_DRIVER_REVISION            IVI_ATTR_CLASS_DRIVER_REVISION              /* ViString, read-only */

#define RESISTOR_ATTR_SPECIFIC_DRIVER_REVISION         IVI_ATTR_SPECIFIC_DRIVER_REVISION             /* ViString, read-only */

        /*- Driver Setup Information -*/
#define RESISTOR_ATTR_DRIVER_SETUP              IVI_ATTR_DRIVER_SETUP                /* ViString */

#define RESISTOR_ATTR_ZERO_OFFSET				   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 3L)    /* ViReal64, channel-based */
#define RESISTOR_ATTR_RESISTANCE				   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 4L)    /* ViReal64, channel-based */


        /*=CHANGE:===============================================================
           Add attribute ID definitions for your class-defined attributes here
           starting at IVI_CLASS_PUBLIC_ATTR_BASE. Examples:

    //- Resistor Fundamental Attributes -//
#define RESISTOR_ATTR_FUNCTION                  (IVI_CLASS_PUBLIC_ATTR_BASE + 1L)    
#define RESISTOR_ATTR_RANGE                     (IVI_CLASS_PUBLIC_ATTR_BASE + 2L)    
#define RESISTOR_ATTR_RESOLUTION_ABSOLUTE       (IVI_CLASS_PUBLIC_ATTR_BASE + 8L)    

    //- ResistorAcMeasurement Attributes -/
#define RESISTOR_ATTR_AC_MIN_FREQ               (IVI_CLASS_PUBLIC_ATTR_BASE + 6L)   
#define RESISTOR_ATTR_AC_MAX_FREQ               (IVI_CLASS_PUBLIC_ATTR_BASE + 7L) 

        /*============================================================END=CHANGE=*/

/****************************************************************************
 *----------------- Resistor Class Attribute Value Defines -------------------*
 ****************************************************************************/

        /*=CHANGE:===============================================================
           Add attribute value definitions for your class-defined attributes here.
           Examples:

    //- Defined values for attribute RESISTOR_ATTR_FUNCTION -//
#define RESISTOR_VAL_DC_VOLTS                     (1L)
#define RESISTOR_VAL_AC_VOLTS                     (2L)
#define RESISTOR_VAL_4_WIRE_RES                   (101L)
#define RESISTOR_VAL_AC_PLUS_DC_VOLTS             (106L)
#define RESISTOR_VAL_AC_PLUS_DC_CURRENT           (107L)

    //- Defined values for attribute RESISTOR_ATTR_RANGE -//
#define RESISTOR_VAL_AUTO_RANGE_ON                (-1.0)
#define RESISTOR_VAL_AUTO_RANGE_OFF               (-2.0)
#define RESISTOR_VAL_AUTO_RANGE_ONCE              (-3.0)

        /*============================================================END=CHANGE=*/


/****************************************************************************
 *--------- Resistor Class Instrument Driver Function Declarations -*
 ****************************************************************************/
    /*- Required VXIplug&play Functions -*/
ViStatus _VI_FUNC Resistor_init (ViRsrc logicalName, 
                               ViBoolean idQuery, 
                               ViBoolean resetDevice, 
                               ViSession *vi);

ViStatus _VI_FUNC Resistor_close (ViSession vi);

ViStatus _VI_FUNC Resistor_reset (ViSession vi);

ViStatus _VI_FUNC Resistor_self_test (ViSession vi, 
                                    ViInt16 *selfTestResult, 
                                    ViChar selfTestMessage[]);

ViStatus _VI_FUNC Resistor_error_query (ViSession vi, 
                                      ViInt32 *errorCode, 
                                      ViChar errorMessage[]);

ViStatus _VI_FUNC Resistor_revision_query (ViSession vi, 
                                         ViChar driverRev[], 
                                         ViChar instrRev[]);

ViStatus _VI_FUNC Resistor_error_message (ViSession vi, 
                                        ViStatus statusCode, 
                                        ViChar message[]);

    /*- Utility Functions -*/
ViStatus _VI_FUNC Resistor_InvalidateAllAttributes (ViSession vi);

ViStatus _VI_FUNC Resistor_ResetWithDefaults (ViSession vi);

ViStatus _VI_FUNC Resistor_Disable (ViSession vi);

    /*- Required IVI Functions -*/
ViStatus _VI_FUNC Resistor_InitWithOptions (ViRsrc logicalName, 
                                          ViBoolean IDQuery,
                                          ViBoolean resetDevice, 
                                          ViConstString optionString, 
                                          ViSession *vi);

    /*- Set, Get, and Check Attribute Functions -*/
ViStatus _VI_FUNC Resistor_GetAttributeViInt32 (ViSession vi, 
                                              ViConstString channelName,
                                              ViAttr attributeId,
                                              ViInt32 *value);

ViStatus _VI_FUNC Resistor_SetAttributeViInt32 (ViSession vi, 
                                              ViConstString channelName,
                                              ViAttr attributeId,
                                              ViInt32 value);

ViStatus _VI_FUNC Resistor_CheckAttributeViInt32 (ViSession vi, 
                                                ViConstString channelName,
                                                ViAttr attributeId,
                                                ViInt32 value);

ViStatus _VI_FUNC Resistor_GetAttributeViReal64 (ViSession vi, 
                                               ViConstString channelName, 
                                               ViAttr attributeId, 
                                               ViReal64 *value);

ViStatus _VI_FUNC Resistor_SetAttributeViReal64 (ViSession vi, 
                                               ViConstString channelName, 
                                               ViAttr attributeId, 
                                               ViReal64 value);

ViStatus _VI_FUNC Resistor_CheckAttributeViReal64 (ViSession vi, 
                                                 ViConstString channelName, 
                                                 ViAttr attributeId, 
                                                 ViReal64 value);

ViStatus _VI_FUNC Resistor_GetAttributeViString (ViSession vi, 
                                               ViConstString channelName, 
                                               ViAttr attributeId, 
                                               ViInt32 bufferSize, 
                                               ViChar value[]);

ViStatus _VI_FUNC Resistor_SetAttributeViString (ViSession vi, 
                                               ViConstString channelName, 
                                               ViAttr attributeId, 
                                               ViConstString value);

ViStatus _VI_FUNC Resistor_CheckAttributeViString (ViSession vi, 
                                                 ViConstString channelName, 
                                                 ViAttr attributeId, 
                                                 ViConstString value); 


ViStatus _VI_FUNC Resistor_GetAttributeViBoolean (ViSession vi, 
                                                ViConstString channelName, 
                                                ViAttr attributeId, 
                                                ViBoolean *value);

ViStatus _VI_FUNC Resistor_SetAttributeViBoolean (ViSession vi, 
                                                ViConstString channelName, 
                                                ViAttr attributeId, 
                                                ViBoolean value);

ViStatus _VI_FUNC Resistor_CheckAttributeViBoolean (ViSession vi, 
                                                  ViConstString channelName, 
                                                  ViAttr attributeId, 
                                                  ViBoolean value);

ViStatus _VI_FUNC Resistor_GetAttributeViSession (ViSession vi, 
                                                ViConstString channelName, 
                                                ViAttr attributeId, 
                                                ViSession *value);

ViStatus _VI_FUNC Resistor_SetAttributeViSession (ViSession vi, 
                                                ViConstString channelName, 
                                                ViAttr attributeId, 
                                                ViSession value);

ViStatus _VI_FUNC Resistor_CheckAttributeViSession (ViSession vi, 
                                                  ViConstString channelName, 
                                                  ViAttr attributeId, 
                                                  ViSession value);

    /*- Lock and Unlock Functions -*/
ViStatus _VI_FUNC Resistor_LockSession (ViSession vi, 
                                      ViBoolean *callerHasLock);

ViStatus _VI_FUNC Resistor_UnlockSession (ViSession vi, 
                                        ViBoolean *callerHasLock);

    /*- Error Information Functions -*/
ViStatus _VI_FUNC Resistor_GetError (ViSession vi, 
                                   ViStatus *errorCode, 
                                   ViInt32 bufferSize, 
                                   ViChar description[]);

ViStatus _VI_FUNC Resistor_ClearError (ViSession vi);

    /*- Interchangeability Checking Functions -*/
ViStatus _VI_FUNC Resistor_GetNextInterchangeWarning (ViSession vi, 
                                                    ViInt32 bufferSize,
                                                    ViChar warning[]);

ViStatus _VI_FUNC Resistor_ClearInterchangeWarnings (ViSession vi);
ViStatus _VI_FUNC Resistor_ResetInterchangeCheck (ViSession vi);

ViStatus _VI_FUNC Resistor_GetNextCoercionRecord (ViSession vi,
                                                ViInt32 bufferSize, 
                                                ViChar record[]);

   /*- Class Driver Functions (not exported by specific driver) -*/
ViStatus _VI_FUNC Resistor_GetSpecificDriverCHandle (ViSession vi,
                                                   ViSession* specificDriverCHandle);

ViStatus _VI_FUNC Resistor_GetSpecificDriverIUnknownPtr (ViSession vi,
                                                       void* specificDriverIUnknownPtr);

    /*- Resistor Functions -*/
ViStatus _VI_FUNC Resistor_ConfigureOutputResistance (ViSession vi,
                                                      ViChar _VI_FAR channelName[],
                                                      ViReal64 resistance);

ViStatus _VI_FUNC Resistor_CalibrateOutputResistance (ViSession vi,
                                                      ViChar _VI_FAR channelName[],
                                                      ViReal64 outputResistance,
                                                      ViInt32 calibrationType);

ViStatus _VI_FUNC Resistor_ReadCalibration (ViSession vi,
                                            ViChar _VI_FAR channelName[]);

ViStatus _VI_FUNC Resistor_WriteCalibration (ViSession vi,
                                             ViChar _VI_FAR channelName[]);

ViStatus _VI_FUNC Resistor_WriteInstrData (ViSession vi,
                                           ViChar _VI_FAR writeBuffer[]);

ViStatus _VI_FUNC Resistor_ReadInstrData (ViSession vi,
                                          ViInt32 number_ofBytesToRead,
                                          ViChar _VI_FAR readBuffer[],
                                          ViPInt32 numBytesRead);


                                                
/****************************************************************************
 *----------------- Resistor Class Error And Completion Codes ----------------*
 ****************************************************************************/
    /*=CHANGE:==============================================================*
        Add class-defined error and warning codes and messages here starting at
        IVI_CLASS_WARN_BASE and IVI_CLASS_ERROR_BASE.  Example:

#define RESISTOR_WARN_OVER_RANGE              (IVI_CLASS_WARN_BASE  + 1)

#define RESISTOR_ERROR_TRIGGER_NOT_SOFTWARE   (IVI_CROSS_CLASS_ERROR_BASE + 1)
#define RESISTOR_ERROR_MAX_TIME_EXCEEDED      (IVI_CLASS_ERROR_BASE + 3)

#define RESISTOR_WRNMSG_OVER_RANGE            "Over range."

#define RESISTOR_ERRMSG_MAX_TIME_EXCEEDED     "Max Time exceeded before "\
                                            "operation completed."
#define RESISTOR_ERRMSG_TRIGGER_NOT_SOFTWARE  "The trigger source is not "\
                                            "software trigger."
     *===========================================================END=CHANGE=*/

#define RESISTOR_ERROR_CODES_AND_MSGS      {VI_NULL, VI_NULL}

    /*=CHANGE:==============================================================*
        Add error and warning codes and messages to 
        RESISTOR_ERROR_CODES_AND_MSGS and remove the above line.
        Example:

#define RESISTOR_ERROR_CODES_AND_MSGS                                         \
        {RESISTOR_WARN_OVER_RANGE,            RESISTOR_WRNMSG_OVER_RANGE},          \
        {RESISTOR_ERROR_MAX_TIME_EXCEEDED,    RESISTOR_ERRMSG_MAX_TIME_EXCEEDED},    \
        {RESISTOR_ERROR_TRIGGER_NOT_SOFTWARE, RESISTOR_ERRMSG_TRIGGER_NOT_SOFTWARE} 
     *===========================================================END=CHANGE=*/


/****************************************************************************
 *---------------------------- End Include File ----------------------------*
 ****************************************************************************/

#if defined(__cplusplus) || defined(__cplusplus__)
}                                                    
#endif

#endif /* RESISTOR_HEADER */

