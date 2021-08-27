/****************************************************************************
 *                       ELCOM-VPC Power Switch                           
 *                                                                          
 * Title:    elvpc3a.h                                        
 * Purpose:  ELCOM-VPC Power Switch                                       
 *           instrument driver declarations.                                
 *                                                                          
 ****************************************************************************/

#ifndef __ELVPC3A_HEADER
#define __ELVPC3A_HEADER

#include <ivi.h>
#include <iviswtch.h>

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/****************************************************************************
 *----------------- Instrument Driver Revision Information -----------------*
 ****************************************************************************/
#define ELVPC3A_MAJOR_VERSION                1     /* Instrument driver major version   */
#define ELVPC3A_MINOR_VERSION                0     /* Instrument driver minor version   */
                                                                
#define ELVPC3A_CLASS_SPEC_MAJOR_VERSION     2     /* Class specification major version */
#define ELVPC3A_CLASS_SPEC_MINOR_VERSION     0     /* Class specification minor version */


        /*=CHANGE:===============================================================
           Add supported instrument models.
           Add your company name.                                                 
           Add useful instrument driver description.                                                 
         *============================================================END=CHANGE=*/
#define ELVPC3A_SUPPORTED_INSTRUMENT_MODELS  "MODEL1,MODEL2,MODEL3,MODEL4"
#define ELVPC3A_DRIVER_VENDOR                "ELCOM"
#define ELVPC3A_DRIVER_DESCRIPTION           "Description"
                    



/**************************************************************************** 
 *------------------------------ Useful Macros -----------------------------* 
 ****************************************************************************/

        /*=CHANGE:===============================================================
           Add useful instrument macros here.                                                 
         *============================================================END=CHANGE=*/

/**************************************************************************** 
 *---------------------------- Attribute Defines ---------------------------* 
 ****************************************************************************/

    /*- IVI Inherent Instrument Attributes ---------------------------------*/    

        /* User Options */
#define ELVPC3A_ATTR_RANGE_CHECK                   IVI_ATTR_RANGE_CHECK                    /* ViBoolean */
#define ELVPC3A_ATTR_QUERY_INSTRUMENT_STATUS       IVI_ATTR_QUERY_INSTRUMENT_STATUS        /* ViBoolean */
#define ELVPC3A_ATTR_CACHE                         IVI_ATTR_CACHE                          /* ViBoolean */
#define ELVPC3A_ATTR_SIMULATE                      IVI_ATTR_SIMULATE                       /* ViBoolean */
#define ELVPC3A_ATTR_RECORD_COERCIONS              IVI_ATTR_RECORD_COERCIONS               /* ViBoolean */
#define ELVPC3A_ATTR_INTERCHANGE_CHECK             IVI_ATTR_INTERCHANGE_CHECK              /* ViBoolean */
        
        /* Driver Information  */
#define ELVPC3A_ATTR_SPECIFIC_DRIVER_PREFIX        IVI_ATTR_SPECIFIC_DRIVER_PREFIX         /* ViString, read-only  */
#define ELVPC3A_ATTR_SUPPORTED_INSTRUMENT_MODELS   IVI_ATTR_SUPPORTED_INSTRUMENT_MODELS    /* ViString, read-only  */
#define ELVPC3A_ATTR_GROUP_CAPABILITIES            IVI_ATTR_GROUP_CAPABILITIES             /* ViString, read-only  */
#define ELVPC3A_ATTR_INSTRUMENT_MANUFACTURER       IVI_ATTR_INSTRUMENT_MANUFACTURER        /* ViString, read-only  */
#define ELVPC3A_ATTR_INSTRUMENT_MODEL              IVI_ATTR_INSTRUMENT_MODEL               /* ViString, read-only  */
#define ELVPC3A_ATTR_INSTRUMENT_FIRMWARE_REVISION  IVI_ATTR_INSTRUMENT_FIRMWARE_REVISION              /* ViString, read-only  */
#define ELVPC3A_ATTR_SPECIFIC_DRIVER_REVISION      IVI_ATTR_SPECIFIC_DRIVER_REVISION       /* ViString, read-only  */
#define ELVPC3A_ATTR_SPECIFIC_DRIVER_VENDOR        IVI_ATTR_SPECIFIC_DRIVER_VENDOR         /* ViString, read-only  */
#define ELVPC3A_ATTR_SPECIFIC_DRIVER_DESCRIPTION   IVI_ATTR_SPECIFIC_DRIVER_DESCRIPTION    /* ViString, read-only  */
#define ELVPC3A_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION IVI_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION /* ViInt32, read-only */
#define ELVPC3A_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION IVI_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION /* ViInt32, read-only */

        /* Advanced Session Information */
#define ELVPC3A_ATTR_LOGICAL_NAME                  IVI_ATTR_LOGICAL_NAME                   /* ViString, read-only  */
#define ELVPC3A_ATTR_IO_RESOURCE_DESCRIPTOR        IVI_ATTR_IO_RESOURCE_DESCRIPTOR         /* ViString, read-only  */
#define ELVPC3A_ATTR_DRIVER_SETUP                  IVI_ATTR_DRIVER_SETUP                   /* ViString, read-only  */        
    /*- Instrument-Specific Attributes -------------------------------------*/
    /*- Configuration Attributes -------------------------------------------*/
#define ELVPC3A_ATTR_IS_SOURCE_CHANNEL          IVISWTCH_ATTR_IS_SOURCE_CHANNEL        /* ViBoolean, Channel-based */
#define ELVPC3A_ATTR_IS_CONFIGURATION_CHANNEL   IVISWTCH_ATTR_IS_CONFIGURATION_CHANNEL /* ViBoolean, Channel-based */

    /*- Status Attributes --------------------------------------------------*/
#define ELVPC3A_ATTR_IS_DEBOUNCED               IVISWTCH_ATTR_IS_DEBOUNCED             /* ViBoolean, Read-only */

    /*- Device Information Attributes --------------------------------------*/
#define ELVPC3A_ATTR_SETTLING_TIME              IVISWTCH_ATTR_SETTLING_TIME            /* ViReal64, Channel-based */
#define ELVPC3A_ATTR_BANDWIDTH                  IVISWTCH_ATTR_BANDWIDTH                /* ViReal64, Channel-based, Read-only */
#define ELVPC3A_ATTR_CHANNEL_COUNT              IVI_ATTR_CHANNEL_COUNT                 /* ViInt32,  read-only  */
#define ELVPC3A_ATTR_MAX_DC_VOLTAGE             IVISWTCH_ATTR_MAX_DC_VOLTAGE           /* ViReal64, Channel-based, Read-only */
#define ELVPC3A_ATTR_MAX_AC_VOLTAGE             IVISWTCH_ATTR_MAX_AC_VOLTAGE           /* ViReal64, Channel-based, Read-only */
#define ELVPC3A_ATTR_MAX_SWITCHING_AC_CURRENT   IVISWTCH_ATTR_MAX_SWITCHING_AC_CURRENT /* ViReal64, Channel-based, Read-only */
#define ELVPC3A_ATTR_MAX_SWITCHING_DC_CURRENT   IVISWTCH_ATTR_MAX_SWITCHING_DC_CURRENT /* ViReal64, Channel-based, Read-only */
#define ELVPC3A_ATTR_MAX_CARRY_AC_CURRENT       IVISWTCH_ATTR_MAX_CARRY_AC_CURRENT     /* ViReal64, Channel-based, Read-only */
#define ELVPC3A_ATTR_MAX_CARRY_DC_CURRENT       IVISWTCH_ATTR_MAX_CARRY_DC_CURRENT     /* ViReal64, Channel-based, Read-only */
#define ELVPC3A_ATTR_MAX_SWITCHING_AC_POWER     IVISWTCH_ATTR_MAX_SWITCHING_AC_POWER   /* ViReal64, Channel-based, Read-only */
#define ELVPC3A_ATTR_MAX_SWITCHING_DC_POWER     IVISWTCH_ATTR_MAX_SWITCHING_DC_POWER   /* ViReal64, Channel-based, Read-only */
#define ELVPC3A_ATTR_MAX_CARRY_AC_POWER         IVISWTCH_ATTR_MAX_CARRY_AC_POWER       /* ViReal64, Channel-based, Read-only */
#define ELVPC3A_ATTR_MAX_CARRY_DC_POWER         IVISWTCH_ATTR_MAX_CARRY_DC_POWER       /* ViReal64, Channel-based, Read-only */
#define ELVPC3A_ATTR_CHARACTERISTIC_IMPEDANCE   IVISWTCH_ATTR_CHARACTERISTIC_IMPEDANCE /* ViReal64, Channel-based, Read-only */
#define ELVPC3A_ATTR_NUM_OF_ROWS                IVISWTCH_ATTR_NUM_OF_ROWS              /* ViInt32, Read-only */
#define ELVPC3A_ATTR_NUM_OF_COLUMNS             IVISWTCH_ATTR_NUM_OF_COLUMNS           /* ViInt32, Read-only */
#define ELVPC3A_ATTR_WIRE_MODE                  IVISWTCH_ATTR_WIRE_MODE                /* ViInt32, Channel-based, Read-only */

    /*=CHANGE:===============================================================*
        Add attribute definitions here.                                       
        #define ELVPC3A_ATTR_MY_ATTRIBUTE     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 2L) 
     *============================================================END=CHANGE=*/

    /*- Obsolete Inherent Instrument Attributes ----------------------------*/

        // Driver Information 
#define ELVPC3A_ATTR_SPECIFIC_DRIVER_MAJOR_VERSION IVI_ATTR_SPECIFIC_DRIVER_MAJOR_VERSION  // ViInt32,  read-only  
#define ELVPC3A_ATTR_SPECIFIC_DRIVER_MINOR_VERSION IVI_ATTR_SPECIFIC_DRIVER_MINOR_VERSION  // ViInt32,  read-only  

        // Error Info
#define ELVPC3A_ATTR_PRIMARY_ERROR                 IVI_ATTR_PRIMARY_ERROR                  // ViInt32 
#define ELVPC3A_ATTR_SECONDARY_ERROR               IVI_ATTR_SECONDARY_ERROR                // ViInt32
#define ELVPC3A_ATTR_ERROR_ELABORATION             IVI_ATTR_ERROR_ELABORATION              // ViString

        // Advanced Session Information
#define ELVPC3A_ATTR_IO_SESSION_TYPE               IVI_ATTR_IO_SESSION_TYPE                // ViString, read-only  
#define ELVPC3A_ATTR_IO_SESSION                    IVI_ATTR_IO_SESSION                     // ViSession, read-only

     // Obsolete Attribute Identifiers for Renamed Attributes
#define ELVPC3A_ATTR_NUM_CHANNELS             IVI_ATTR_CHANNEL_COUNT
#define ELVPC3A_ATTR_QUERY_INSTR_STATUS       ELVPC3A_ATTR_QUERY_INSTRUMENT_STATUS            
#define ELVPC3A_ATTR_RESOURCE_DESCRIPTOR      ELVPC3A_ATTR_IO_RESOURCE_DESCRIPTOR

 


/**************************************************************************** 
 *------------------------ Attribute Value Defines -------------------------* 
 ****************************************************************************/
    /* Defined values for ELVPC3A_ATTR_SCAN_MODE */
#define ELVPC3A_VAL_NONE                        IVISWTCH_VAL_NONE
#define ELVPC3A_VAL_BREAK_BEFORE_MAKE           IVISWTCH_VAL_BREAK_BEFORE_MAKE
#define ELVPC3A_VAL_BREAK_AFTER_MAKE            IVISWTCH_VAL_BREAK_AFTER_MAKE
        /*=CHANGE:==========================================================*
           Add definitions for instrument specific scan modes here.
           Example:

            #define ELVPC3A_VAL_MODE_1   (IVISWTCH_VAL_SCAN_MODE_SPECIFIC_EXT_BASE + 1)
            #define ELVPC3A_VAL_MODE_2   (IVISWTCH_VAL_SCAN_MODE_SPECIFIC_EXT_BASE + 2)
         *=======================================================END=CHANGE=*/

    /* Defined values for ELVPC3A_ATTR_TRIGGER_INPUT */
#define ELVPC3A_VAL_IMMEDIATE                   IVISWTCH_VAL_IMMEDIATE
#define ELVPC3A_VAL_EXTERNAL                    IVISWTCH_VAL_EXTERNAL
#define ELVPC3A_VAL_SOFTWARE_TRIG               IVISWTCH_VAL_SOFTWARE_TRIG
#define ELVPC3A_VAL_TTL0                        IVISWTCH_VAL_TTL0
#define ELVPC3A_VAL_TTL1                        IVISWTCH_VAL_TTL1
#define ELVPC3A_VAL_TTL2                        IVISWTCH_VAL_TTL2
#define ELVPC3A_VAL_TTL3                        IVISWTCH_VAL_TTL3
#define ELVPC3A_VAL_TTL4                        IVISWTCH_VAL_TTL4
#define ELVPC3A_VAL_TTL5                        IVISWTCH_VAL_TTL5
#define ELVPC3A_VAL_TTL6                        IVISWTCH_VAL_TTL6
#define ELVPC3A_VAL_TTL7                        IVISWTCH_VAL_TTL7
#define ELVPC3A_VAL_ECL0                        IVISWTCH_VAL_ECL0
#define ELVPC3A_VAL_ECL1                        IVISWTCH_VAL_ECL1
#define ELVPC3A_VAL_PXI_STAR                    IVISWTCH_VAL_PXI_STAR
#define ELVPC3A_VAL_RTSI_0                      IVISWTCH_VAL_RTSI_0
#define ELVPC3A_VAL_RTSI_1                      IVISWTCH_VAL_RTSI_1
#define ELVPC3A_VAL_RTSI_2                      IVISWTCH_VAL_RTSI_2
#define ELVPC3A_VAL_RTSI_3                      IVISWTCH_VAL_RTSI_3
#define ELVPC3A_VAL_RTSI_4                      IVISWTCH_VAL_RTSI_4
#define ELVPC3A_VAL_RTSI_5                      IVISWTCH_VAL_RTSI_5
#define ELVPC3A_VAL_RTSI_6                      IVISWTCH_VAL_RTSI_6
        /*=CHANGE:==========================================================*
           Add definitions for instrument specific trigger sources here.
           Example:

            #define ELVPC3A_VAL_TRIG_IN_1   (IVISWTCH_VAL_TRIGGER_INPUT_SPECIFIC_EXT_BASE + 1)
            #define ELVPC3A_VAL_TRIG_IN_2   (IVISWTCH_VAL_TRIGGER_INPUT_SPECIFIC_EXT_BASE + 2)
         *=======================================================END=CHANGE=*/

    /* Defined values for ELVPC3A_ATTR_SCAN_ADVANCED_OUTPUT */
#define ELVPC3A_VAL_GPIB_SRQ                    IVISWTCH_VAL_GPIB_SRQ
/* #define ELVPC3A_VAL_NONE                    DEFINED ABOVE */
/* #define ELVPC3A_VAL_EXTERNAL                DEFINED ABOVE */
/* #define ELVPC3A_VAL_TTL0                    DEFINED ABOVE */
/* #define ELVPC3A_VAL_TTL1                    DEFINED ABOVE */
/* #define ELVPC3A_VAL_TTL2                    DEFINED ABOVE */
/* #define ELVPC3A_VAL_TTL3                    DEFINED ABOVE */
/* #define ELVPC3A_VAL_TTL4                    DEFINED ABOVE */
/* #define ELVPC3A_VAL_TTL5                    DEFINED ABOVE */
/* #define ELVPC3A_VAL_TTL6                    DEFINED ABOVE */
/* #define ELVPC3A_VAL_TTL7                    DEFINED ABOVE */
/* #define ELVPC3A_VAL_ECL0                    DEFINED ABOVE */
/* #define ELVPC3A_VAL_ECL1                    DEFINED ABOVE */
/* #define ELVPC3A_VAL_PXI_STAR                DEFINED ABOVE */
/* #define ELVPC3A_VAL_RTSI_0                  DEFINED ABOVE */
/* #define ELVPC3A_VAL_RTSI_1                  DEFINED ABOVE */
/* #define ELVPC3A_VAL_RTSI_2                  DEFINED ABOVE */
/* #define ELVPC3A_VAL_RTSI_3                  DEFINED ABOVE */
/* #define ELVPC3A_VAL_RTSI_4                  DEFINED ABOVE */
/* #define ELVPC3A_VAL_RTSI_5                  DEFINED ABOVE */
/* #define ELVPC3A_VAL_RTSI_6                  DEFINED ABOVE */
        /*=CHANGE:==========================================================*
           Add definitions for instrument specific scanner advanced output
           lines here. Example:

            #define ELVPC3A_VAL_TRIG_OUT_1  (IVISWTCH_VAL_SCAN_ADVANCED_OUTPUT_SPECIFIC_EXT_BASE + 1)
            #define ELVPC3A_VAL_TRIG_OUT_2  (IVISWTCH_VAL_SCAN_ADVANCED_OUTPUT_SPECIFIC_EXT_BASE + 2)
         *=======================================================END=CHANGE=*/

    /* Defined values for elvpc3a_CanConnect path capability parameter */
#define ELVPC3A_VAL_PATH_AVAILABLE              IVISWTCH_VAL_PATH_AVAILABLE
#define ELVPC3A_VAL_PATH_EXISTS                 IVISWTCH_VAL_PATH_EXISTS
#define ELVPC3A_VAL_PATH_UNSUPPORTED            IVISWTCH_VAL_PATH_UNSUPPORTED
#define ELVPC3A_VAL_RSRC_IN_USE                 IVISWTCH_VAL_RSRC_IN_USE
#define ELVPC3A_VAL_SOURCE_CONFLICT             IVISWTCH_VAL_SOURCE_CONFLICT
#define ELVPC3A_VAL_CHANNEL_NOT_AVAILABLE       IVISWTCH_VAL_CHANNEL_NOT_AVAILABLE
        /*=CHANGE:==========================================================*
           Add definitions for instrument specific path capability parameter
           lines here. Example:

            #define ELVPC3A_VAL_ABILITY_1  (IVISWTCH_VAL_CAN_CONNECT_SPECIFIC_EXT_BASE + 1)
            #define ELVPC3A_VAL_ABILITY_2  (IVISWTCH_VAL_CAN_CONNECT_SPECIFIC_EXT_BASE + 2)
         *=======================================================END=CHANGE=*/

        /* Instrument specific attribute value definitions */

    /*=CHANGE:==============================================================*
        Add value defines for instrument specific attributes here.  Example

    // Defined values for attribute ELVPC3A_ATTR_EXAMPLE 

#define ELVPC3A_VAL_EXAMPLE_1             1
#define ELVPC3A_VAL_EXAMPLE_2             2
#define ELVPC3A_VAL_EXAMPLE_3             3
     *===========================================================END=CHANGE=*/

/**************************************************************************** 
 *---------------- Instrument Driver Function Declarations -----------------* 
 ****************************************************************************/

    /*- Init and Close Functions -------------------------------------------*/
ViStatus _VI_FUNC  elvpc3a_init (ViRsrc resourceName, ViBoolean IDQuery,
                                  ViBoolean resetDevice, ViSession *vi);
ViStatus _VI_FUNC  elvpc3a_InitWithOptions (ViRsrc resourceName, ViBoolean IDQuery,
                                             ViBoolean resetDevice, ViConstString optionString, 
                                             ViSession *newVi);
ViStatus _VI_FUNC  elvpc3a_close (ViSession vi);   

    /*- Coercion Info Functions --------------------------------------------*/
ViStatus _VI_FUNC  elvpc3a_GetNextCoercionRecord (ViSession vi,
                                                   ViInt32 bufferSize,
                                                   ViChar record[]);
    
    /*- Locking Functions --------------------------------------------------*/
ViStatus _VI_FUNC  elvpc3a_LockSession (ViSession vi, ViBoolean *callerHasLock);   
ViStatus _VI_FUNC  elvpc3a_UnlockSession (ViSession vi, ViBoolean *callerHasLock);

    /*- Channel Info Functions ---------------------------------------------*/
ViStatus _VI_FUNC elvpc3a_GetChannelName (ViSession vi, ViInt32 index, 
                                   ViInt32 bufferSize, ViChar name[]);
    
    /*- Switch Routing Functions -----------------------------------------*/
ViStatus _VI_FUNC elvpc3a_Connect (ViSession vi,
                                    ViConstString channel1,
                                    ViConstString channel2);
ViStatus _VI_FUNC elvpc3a_Disconnect (ViSession vi,
                                       ViConstString channel1,
                                       ViConstString channel2);
ViStatus _VI_FUNC elvpc3a_DisconnectAll (ViSession vi);
ViStatus _VI_FUNC elvpc3a_GetPath (ViSession vi, ViConstString channel1,
                                    ViConstString channel2, ViInt32 bufSize,
                                    ViChar pathList[]);
ViStatus _VI_FUNC elvpc3a_SetPath (ViSession vi, ViConstString pathList);
ViStatus _VI_FUNC elvpc3a_CanConnect (ViSession vi, ViConstString channel1,
                                       ViConstString channel2,
                                       ViInt32 *pathCapabilityRef);
ViStatus _VI_FUNC elvpc3a_IsDebounced (ViSession vi, ViBoolean *isDebounced);
ViStatus _VI_FUNC elvpc3a_WaitForDebounce (ViSession vi, ViInt32 maxTime);

    
    /*- Error Functions ----------------------------------------------------*/
ViStatus _VI_FUNC  elvpc3a_error_query (ViSession vi, ViInt32 *errorCode,
                                         ViChar errorMessage[]);

ViStatus _VI_FUNC  elvpc3a_GetError (ViSession vi, ViStatus *code, 
                                      ViInt32 bufferSize, ViChar description[]);
ViStatus _VI_FUNC  elvpc3a_ClearError (ViSession vi);

ViStatus _VI_FUNC  elvpc3a_error_message (ViSession vi, ViStatus errorCode,
                                           ViChar errorMessage[256]);
    
    /*- Obsolete Error Handling Functions ----------------------------*/
        /*=CHANGE:=============================================================*
            NOTE TO THE DEVELOPER:  These functions are obsolete and may not be
            supported in future versions of IVI drivers.  
            
            If you DO NOT want this driver to be compliant with NI's Ivi Driver 
            Toolset 1.1, remove these function definitions.

            If you DO want this driver to be compliant with NI's IVI Driver Toolset
            1.1, remove the comments around these functions definitions.

ViStatus _VI_FUNC  elvpc3a_GetErrorInfo (ViSession vi, ViStatus *primaryError, 
                                          ViStatus *secondaryError, 
                                          ViChar errorElaboration[256]);
ViStatus _VI_FUNC  elvpc3a_ClearErrorInfo (ViSession vi);
     *==========================================================END=CHANGE=*/

    /*- Interchangeability Checking Functions ------------------------------*/
ViStatus _VI_FUNC elvpc3a_GetNextInterchangeWarning (ViSession vi, 
                                                      ViInt32 bufferSize, 
                                                      ViChar warnString[]);
ViStatus _VI_FUNC elvpc3a_ResetInterchangeCheck (ViSession vi);
ViStatus _VI_FUNC elvpc3a_ClearInterchangeWarnings (ViSession vi);

    /*- Utility Functions --------------------------------------------------*/
ViStatus _VI_FUNC elvpc3a_InvalidateAllAttributes (ViSession vi);
ViStatus _VI_FUNC  elvpc3a_reset (ViSession vi);
ViStatus _VI_FUNC  elvpc3a_ResetWithDefaults (ViSession vi);
ViStatus _VI_FUNC  elvpc3a_self_test (ViSession vi, ViInt16 *selfTestResult,
                                       ViChar selfTestMessage[]);
ViStatus _VI_FUNC  elvpc3a_revision_query (ViSession vi, 
                                            ViChar instrumentDriverRevision[],
                                            ViChar firmwareRevision[]);
ViStatus _VI_FUNC elvpc3a_Disable (ViSession vi);

    /*- Set, Get, and Check Attribute Functions ----------------------------*/
ViStatus _VI_FUNC  elvpc3a_GetAttributeViInt32 (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 *value);
ViStatus _VI_FUNC  elvpc3a_GetAttributeViReal64 (ViSession vi, ViConstString channelName, ViAttr attribute, ViReal64 *value);
ViStatus _VI_FUNC  elvpc3a_GetAttributeViString (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 bufSize, ViChar value[]); 
ViStatus _VI_FUNC  elvpc3a_GetAttributeViSession (ViSession vi, ViConstString channelName, ViAttr attribute, ViSession *value);
ViStatus _VI_FUNC  elvpc3a_GetAttributeViBoolean (ViSession vi, ViConstString channelName, ViAttr attribute, ViBoolean *value);

ViStatus _VI_FUNC  elvpc3a_SetAttributeViInt32 (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 value);
ViStatus _VI_FUNC  elvpc3a_SetAttributeViReal64 (ViSession vi, ViConstString channelName, ViAttr attribute, ViReal64 value);
ViStatus _VI_FUNC  elvpc3a_SetAttributeViString (ViSession vi, ViConstString channelName, ViAttr attribute, ViConstString value); 
ViStatus _VI_FUNC  elvpc3a_SetAttributeViSession (ViSession vi, ViConstString channelName, ViAttr attribute, ViSession value);
ViStatus _VI_FUNC  elvpc3a_SetAttributeViBoolean (ViSession vi, ViConstString channelName, ViAttr attribute, ViBoolean value);

ViStatus _VI_FUNC  elvpc3a_CheckAttributeViInt32 (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 value);
ViStatus _VI_FUNC  elvpc3a_CheckAttributeViReal64 (ViSession vi, ViConstString channelName, ViAttr attribute, ViReal64 value);
ViStatus _VI_FUNC  elvpc3a_CheckAttributeViString (ViSession vi, ViConstString channelName, ViAttr attribute, ViConstString value); 
ViStatus _VI_FUNC  elvpc3a_CheckAttributeViSession (ViSession vi, ViConstString channelName, ViAttr attribute, ViSession value);
ViStatus _VI_FUNC  elvpc3a_CheckAttributeViBoolean (ViSession vi, ViConstString channelName, ViAttr attribute, ViBoolean value);

/****************************************************************************
 *------------------------ Error And Completion Codes ----------------------*
 ****************************************************************************/
#define ELVPC3A_WARN_PATH_REMAINS                 IVISWTCH_WARN_PATH_REMAINS
#define ELVPC3A_WARN_IMPLICIT_CONNECTION_EXISTS   IVISWTCH_WARN_IMPLICIT_CONNECTION_EXISTS

#define ELVPC3A_ERROR_INVALID_SWITCH_PATH         IVISWTCH_ERROR_INVALID_SWITCH_PATH
#define ELVPC3A_ERROR_INVALID_SCAN_LIST           IVISWTCH_ERROR_INVALID_SCAN_LIST
#define ELVPC3A_ERROR_RSRC_IN_USE                 IVISWTCH_ERROR_RSRC_IN_USE
#define ELVPC3A_ERROR_EMPTY_SCAN_LIST             IVISWTCH_ERROR_EMPTY_SCAN_LIST
#define ELVPC3A_ERROR_EMPTY_SWITCH_PATH           IVISWTCH_ERROR_EMPTY_SWITCH_PATH
#define ELVPC3A_ERROR_SCAN_IN_PROGRESS            IVISWTCH_ERROR_SCAN_IN_PROGRESS
#define ELVPC3A_ERROR_NO_SCAN_IN_PROGRESS         IVISWTCH_ERROR_NO_SCAN_IN_PROGRESS
#define ELVPC3A_ERROR_NO_SUCH_PATH                IVISWTCH_ERROR_NO_SUCH_PATH
#define ELVPC3A_ERROR_IS_CONFIGURATION_CHANNEL    IVISWTCH_ERROR_IS_CONFIGURATION_CHANNEL
#define ELVPC3A_ERROR_NOT_A_CONFIGURATION_CHANNEL IVISWTCH_ERROR_NOT_A_CONFIGURATION_CHANNEL
#define ELVPC3A_ERROR_ATTEMPT_TO_CONNECT_SOURCES  IVISWTCH_ERROR_ATTEMPT_TO_CONNECT_SOURCES
#define ELVPC3A_ERROR_EXPLICIT_CONNECTION_EXISTS  IVISWTCH_ERROR_EXPLICIT_CONNECTION_EXISTS
#define ELVPC3A_ERROR_LEG_MISSING_FIRST_CHANNEL   IVISWTCH_ERROR_LEG_MISSING_FIRST_CHANNEL
#define ELVPC3A_ERROR_LEG_MISSING_SECOND_CHANNEL  IVISWTCH_ERROR_LEG_MISSING_SECOND_CHANNEL
#define ELVPC3A_ERROR_CHANNEL_DUPLICATED_IN_LEG   IVISWTCH_ERROR_CHANNEL_DUPLICATED_IN_LEG
#define ELVPC3A_ERROR_CHANNEL_DUPLICATED_IN_PATH  IVISWTCH_ERROR_CHANNEL_DUPLICATED_IN_PATH
#define ELVPC3A_ERROR_PATH_NOT_FOUND              IVISWTCH_ERROR_PATH_NOT_FOUND
#define ELVPC3A_ERROR_DISCONTINUOUS_PATH          IVISWTCH_ERROR_DISCONTINUOUS_PATH
#define ELVPC3A_ERROR_CANNOT_CONNECT_DIRECTLY     IVISWTCH_ERROR_CANNOT_CONNECT_DIRECTLY
#define ELVPC3A_ERROR_CHANNELS_ALREADY_CONNECTED  IVISWTCH_ERROR_CHANNELS_ALREADY_CONNECTED
#define ELVPC3A_ERROR_CANNOT_CONNECT_TO_ITSELF    IVISWTCH_ERROR_CANNOT_CONNECT_TO_ITSELF
#define ELVPC3A_ERROR_MAX_TIME_EXCEEDED       IVISWTCH_ERROR_MAX_TIME_EXCEEDED

#define ELVPC3A_ERROR_TRIGGER_NOT_SOFTWARE        IVISWTCH_ERROR_TRIGGER_NOT_SOFTWARE
    /*=CHANGE:==============================================================*
        Add instrument-specific error codes here starting at
        IVI_SPECIFIC_ERROR_BASE.  Example:

#define ELVPC3A_ERROR_NAME  (IVI_SPECIFIC_ERROR_BASE + 1)
     *===========================================================END=CHANGE=*/

/**************************************************************************** 
 *---------------------------- End Include File ----------------------------* 
 ****************************************************************************/
#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif
#endif /* __ELVPC3A_HEADER */




