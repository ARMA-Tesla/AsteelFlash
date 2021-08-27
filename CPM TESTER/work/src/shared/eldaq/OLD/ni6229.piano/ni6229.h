#include <cvidef.h>

/****************************************************************************
 *                       DAQ Measurement Card                           
 *                                                                          
 * Title:    eldaq.h                                        
 * Purpose:  DAQ Measurement Card                                       
 *           instrument driver declarations.                                
 *                                                                          
 ****************************************************************************/

#ifndef __NI6229_HEADER
#define __NI6229_HEADER

#include <ivi.h>
#include "../class/eldaq.h"

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/****************************************************************************
 *----------------- Instrument Driver Revision Information -----------------*
 ****************************************************************************/
#define NI6229_MAJOR_VERSION                1     /* Instrument driver major version   */
#define NI6229_MINOR_VERSION                0     /* Instrument driver minor version   */
                                                                
#define NI6229_CLASS_SPEC_MAJOR_VERSION     2     /* Class specification major version */
#define NI6229_CLASS_SPEC_MINOR_VERSION     0     /* Class specification minor version */


#define NI6229_SUPPORTED_INSTRUMENT_MODELS  "NI-6361"
#define NI6229_DRIVER_VENDOR                "AE, s.a.s."
#define NI6229_DRIVER_DESCRIPTION           "DAQ IVI Driver"
                    

/**************************************************************************** 
 *------------------------------ Useful Macros -----------------------------* 
 ****************************************************************************/


/**************************************************************************** 
 *---------------------------- Attribute Defines ---------------------------* 
 ****************************************************************************/

    /*- IVI Inherent Instrument Attributes ---------------------------------*/    

        /* User Options */
#define NI6229_ATTR_RANGE_CHECK                   IVI_ATTR_RANGE_CHECK                    /* ViBoolean */
#define NI6229_ATTR_QUERY_INSTRUMENT_STATUS       IVI_ATTR_QUERY_INSTRUMENT_STATUS        /* ViBoolean */
#define NI6229_ATTR_CACHE                         IVI_ATTR_CACHE                          /* ViBoolean */
#define NI6229_ATTR_SIMULATE                      IVI_ATTR_SIMULATE                       /* ViBoolean */
#define NI6229_ATTR_RECORD_COERCIONS              IVI_ATTR_RECORD_COERCIONS               /* ViBoolean */
#define NI6229_ATTR_INTERCHANGE_CHECK             IVI_ATTR_INTERCHANGE_CHECK              /* ViBoolean */
        
        /* Driver Information  */
#define NI6229_ATTR_SPECIFIC_DRIVER_PREFIX        IVI_ATTR_SPECIFIC_DRIVER_PREFIX         /* ViString, read-only  */
#define NI6229_ATTR_SUPPORTED_INSTRUMENT_MODELS   IVI_ATTR_SUPPORTED_INSTRUMENT_MODELS    /* ViString, read-only  */
#define NI6229_ATTR_GROUP_CAPABILITIES            IVI_ATTR_GROUP_CAPABILITIES             /* ViString, read-only  */
#define NI6229_ATTR_INSTRUMENT_MANUFACTURER       IVI_ATTR_INSTRUMENT_MANUFACTURER        /* ViString, read-only  */
#define NI6229_ATTR_INSTRUMENT_MODEL              IVI_ATTR_INSTRUMENT_MODEL               /* ViString, read-only  */
#define NI6229_ATTR_INSTRUMENT_FIRMWARE_REVISION  IVI_ATTR_INSTRUMENT_FIRMWARE_REVISION              /* ViString, read-only  */
#define NI6229_ATTR_SPECIFIC_DRIVER_REVISION      IVI_ATTR_SPECIFIC_DRIVER_REVISION       /* ViString, read-only  */
#define NI6229_ATTR_SPECIFIC_DRIVER_VENDOR        IVI_ATTR_SPECIFIC_DRIVER_VENDOR         /* ViString, read-only  */
#define NI6229_ATTR_SPECIFIC_DRIVER_DESCRIPTION   IVI_ATTR_SPECIFIC_DRIVER_DESCRIPTION    /* ViString, read-only  */
#define NI6229_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION IVI_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION /* ViInt32, read-only */
#define NI6229_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION IVI_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION /* ViInt32, read-only */

        /* Advanced Session Information */
#define NI6229_ATTR_LOGICAL_NAME                  IVI_ATTR_LOGICAL_NAME                   /* ViString, read-only  */
#define NI6229_ATTR_IO_RESOURCE_DESCRIPTOR        IVI_ATTR_IO_RESOURCE_DESCRIPTOR         /* ViString, read-only  */
#define NI6229_ATTR_DRIVER_SETUP                  IVI_ATTR_DRIVER_SETUP                   /* ViString, read-only  */        
    /*- Instrument-Specific Attributes -------------------------------------*/

#define NI6229_ATTR_ID_QUERY_RESPONSE      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1L)     /* ViString (Read Only) */
#define NI6229_ATTR_CALIBRATION_FILE_NAME   ELDAQ_ATTR_CALIBRATION_FILE_NAME		/* ViString (Read Only) */
    /*=CHANGE:===============================================================*
        Add attribute definitions here.                                       
        #define NI6229_ATTR_MY_ATTRIBUTE     (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 2L) 
     *============================================================END=CHANGE=*/

    /*- Obsolete Inherent Instrument Attributes ----------------------------*/
        /* Driver Information  */
#define NI6229_ATTR_SPECIFIC_DRIVER_MAJOR_VERSION IVI_ATTR_SPECIFIC_DRIVER_MAJOR_VERSION  // ViInt32,  read-only  
#define NI6229_ATTR_SPECIFIC_DRIVER_MINOR_VERSION IVI_ATTR_SPECIFIC_DRIVER_MINOR_VERSION  // ViInt32,  read-only  

         /* Error Info */
#define NI6229_ATTR_PRIMARY_ERROR                 IVI_ATTR_PRIMARY_ERROR                  // ViInt32 
#define NI6229_ATTR_SECONDARY_ERROR               IVI_ATTR_SECONDARY_ERROR                // ViInt32
#define NI6229_ATTR_ERROR_ELABORATION             IVI_ATTR_ERROR_ELABORATION              // ViString

        /* Advanced Session Information */
#define NI6229_ATTR_IO_SESSION_TYPE               IVI_ATTR_IO_SESSION_TYPE                // ViString, read-only  
#define NI6229_ATTR_IO_SESSION                    IVI_ATTR_IO_SESSION                     // ViSession, read-only

     /* Obsolete Attribute Identifiers for Renamed Attributes */
#define NI6229_ATTR_NUM_CHANNELS             IVI_ATTR_CHANNEL_COUNT
#define NI6229_ATTR_QUERY_INSTR_STATUS       NI6229_ATTR_QUERY_INSTRUMENT_STATUS            
#define NI6229_ATTR_RESOURCE_DESCRIPTOR      NI6229_ATTR_IO_RESOURCE_DESCRIPTOR


/**************************************************************************** 
 *------------------------ Attribute Value Defines -------------------------* 
 ****************************************************************************/

        /* Instrument specific attribute value definitions */
#define NI6229_VAL_MEASUREMENT_RMS		1
#define NI6229_VAL_MEASUREMENT_MIN		2
#define NI6229_VAL_MEASUREMENT_MAX		3
#define NI6229_VAL_MEASUREMENT_MEAN		4

	
typedef struct _SCalibration
{
	int		calType;
	void*	pCalibData;
} SCalibration, *SCalibrationPtr;
	
/**************************************************************************** 
 *---------------- Instrument Driver Function Declarations -----------------* 
 ****************************************************************************/

    /*- Init and Close Functions -------------------------------------------*/
ViStatus _VI_FUNC  ni6229_init (ViRsrc resourceName, ViBoolean IDQuery,
                                  ViBoolean resetDevice, ViSession *vi);
ViStatus _VI_FUNC  ni6229_InitWithOptions (ViRsrc resourceName, ViBoolean IDQuery,
                                             ViBoolean resetDevice, ViConstString optionString, 
                                             ViSession *newVi);
ViStatus _VI_FUNC  ni6229_close (ViSession vi);   

    /*- Coercion Info Functions --------------------------------------------*/
ViStatus _VI_FUNC  ni6229_GetNextCoercionRecord (ViSession vi,
                                                   ViInt32 bufferSize,
                                                   ViChar record[]);
    
    /*- Locking Functions --------------------------------------------------*/
ViStatus _VI_FUNC  ni6229_LockSession (ViSession vi, ViBoolean *callerHasLock);   
ViStatus _VI_FUNC  ni6229_UnlockSession (ViSession vi, ViBoolean *callerHasLock);


    /*- Error Functions ----------------------------------------------------*/
ViStatus _VI_FUNC  ni6229_error_query (ViSession vi, ViInt32 *errorCode,
                                         ViChar errorMessage[]);

ViStatus _VI_FUNC  ni6229_GetError (ViSession vi, ViStatus *code, 
                                      ViInt32 bufferSize, ViChar description[]);
ViStatus _VI_FUNC  ni6229_ClearError (ViSession vi);

ViStatus _VI_FUNC  ni6229_error_message (ViSession vi, ViStatus errorCode,
                                           ViChar errorMessage[256]);
    
    /*- Interchangeability Checking Functions ------------------------------*/
ViStatus _VI_FUNC ni6229_GetNextInterchangeWarning (ViSession vi, 
                                                      ViInt32 bufferSize, 
                                                      ViChar warnString[]);
ViStatus _VI_FUNC ni6229_ResetInterchangeCheck (ViSession vi);
ViStatus _VI_FUNC ni6229_ClearInterchangeWarnings (ViSession vi);

    /*- Utility Functions --------------------------------------------------*/
ViStatus _VI_FUNC ni6229_InvalidateAllAttributes (ViSession vi);
ViStatus _VI_FUNC  ni6229_reset (ViSession vi);
ViStatus _VI_FUNC  ni6229_ResetWithDefaults (ViSession vi);
ViStatus _VI_FUNC  ni6229_self_test (ViSession vi, ViInt16 *selfTestResult,
                                       ViChar selfTestMessage[]);
ViStatus _VI_FUNC  ni6229_revision_query (ViSession vi, 
                                            ViChar instrumentDriverRevision[],
                                            ViChar firmwareRevision[]);
ViStatus _VI_FUNC ni6229_Disable (ViSession vi);

ViStatus _VI_FUNC ni6229_ConfigureMeasurement( 
	ViSession 		vi,
	ViConstString 	channels,
	ViInt32			sampleRate,
	ViReal64		timeInterval
);

ViStatus _VI_FUNC ni6229_StartMeasurement( 
	ViSession 		vi
);
ViStatus _VI_FUNC ni6229_StopMeasurement( 
	ViSession 		vi
);

    /*- Set, Get, and Check Attribute Functions ----------------------------*/
ViStatus _VI_FUNC  ni6229_GetAttributeViInt32 (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 *value);
ViStatus _VI_FUNC  ni6229_GetAttributeViReal64 (ViSession vi, ViConstString channelName, ViAttr attribute, ViReal64 *value);
ViStatus _VI_FUNC  ni6229_GetAttributeViString (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 bufSize, ViChar value[]); 
ViStatus _VI_FUNC  ni6229_GetAttributeViSession (ViSession vi, ViConstString channelName, ViAttr attribute, ViSession *value);
ViStatus _VI_FUNC  ni6229_GetAttributeViBoolean (ViSession vi, ViConstString channelName, ViAttr attribute, ViBoolean *value);

ViStatus _VI_FUNC  ni6229_SetAttributeViInt32 (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 value);
ViStatus _VI_FUNC  ni6229_SetAttributeViReal64 (ViSession vi, ViConstString channelName, ViAttr attribute, ViReal64 value);
ViStatus _VI_FUNC  ni6229_SetAttributeViString (ViSession vi, ViConstString channelName, ViAttr attribute, ViConstString value); 
ViStatus _VI_FUNC  ni6229_SetAttributeViSession (ViSession vi, ViConstString channelName, ViAttr attribute, ViSession value);
ViStatus _VI_FUNC  ni6229_SetAttributeViBoolean (ViSession vi, ViConstString channelName, ViAttr attribute, ViBoolean value);

ViStatus _VI_FUNC  ni6229_CheckAttributeViInt32 (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 value);
ViStatus _VI_FUNC  ni6229_CheckAttributeViReal64 (ViSession vi, ViConstString channelName, ViAttr attribute, ViReal64 value);
ViStatus _VI_FUNC  ni6229_CheckAttributeViString (ViSession vi, ViConstString channelName, ViAttr attribute, ViConstString value); 
ViStatus _VI_FUNC  ni6229_CheckAttributeViSession (ViSession vi, ViConstString channelName, ViAttr attribute, ViSession value);
ViStatus _VI_FUNC  ni6229_CheckAttributeViBoolean (ViSession vi, ViConstString channelName, ViAttr attribute, ViBoolean value);

ViStatus _VI_FUNC ni6229_ReadForce (ViSession vi, ViChar channels[],
                                    ViChar clockSource[], ViReal64 min,
                                    ViReal64 max, ViInt32 sampsPerChan,
                                    ViInt32 rate, double data[]);

ViStatus _VI_FUNC ni6229_GetChannelScale (ViSession vi,
                                          ViChar channelName[],
                                          ViConstString scaleName,
                                          void *pScaleInfo);

ViStatus _VI_FUNC ni6229_ReadMeasurement (ViSession vi,
                                          ViChar channelName[], 
										  void *scaleInfo,
                                          ViInt32 measType,
                                          ViReal64 *value);

ViStatus _VI_FUNC ni6229_ReadWaveform( 
	ViSession 		vi,
	ViConstString	channelName,
	ViConstString	scaleName,
	ViInt32			arraySize,
	ViReal64		data[],
	ViReal64		*startX,
	ViReal64		*xInc
);

ViStatus _VI_FUNC ni6229_WriteDigitalLine (ViSession instrumentHandle,
                                           ViChar digitalChannels[],
                                           ViInt32 status);

ViStatus _VI_FUNC ni6229_LoadCalibration(
	ViSession		vi,
	ViConstString	calibrationFile
);

/****************************************************************************
 *------------------------ Error And Completion Codes ----------------------*
 ****************************************************************************/

#define NI6229_ERROR_MEASUREMENT_IS_RUNNING  (IVI_SPECIFIC_ERROR_BASE + 1)
#define NI6229_ERROR_MEASUREMENT_IS_NOT_CONFIGURED  (IVI_SPECIFIC_ERROR_BASE + 2)
#define NI6229_ERROR_LINEAR_CALIBRATION_CONFIG		(IVI_SPECIFIC_ERROR_BASE + 3)

#define NI6229_ERROR_NOT_SPECIFIED_CALIBRATION_TYPE (IVI_SPECIFIC_ERROR_BASE + 4)
#define NI6229_ERROR_NOT_SUPPORTED_CALIBRATION_TYPE (IVI_SPECIFIC_ERROR_BASE + 5)
      

/**************************************************************************** 
 *---------------------------- End Include File ----------------------------* 
 ****************************************************************************/
#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif
#endif /* __NI6229_HEADER */




