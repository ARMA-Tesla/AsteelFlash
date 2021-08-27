/****************************************************************************
 *                       Pickering Resistor Cards                            
 *                                                                          
 * Title:    pires.h                                        
 * Purpose:  Pickering Resistor Cards                                        
 *           instrument driver declarations.                                
 *                                                                          
 ****************************************************************************/

#ifndef __PIRES_HEADER
#define __PIRES_HEADER

#include <ivi.h>

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/****************************************************************************
 *----------------- Instrument Driver Revision Information -----------------*
 ****************************************************************************/
#define PIRES_MAJOR_VERSION                1     /* Instrument driver major version   */
#define PIRES_MINOR_VERSION                0     /* Instrument driver minor version   */
                                                                
#define PIRES_CLASS_SPEC_MAJOR_VERSION     2     /* Class specification major version */
#define PIRES_CLASS_SPEC_MINOR_VERSION     0     /* Class specification minor version */

#define PIRES_SUPPORTED_INSTRUMENT_MODELS  "40-295-021,40-295-121"
#define PIRES_DRIVER_VENDOR                "ELCOM, a.s."
#define PIRES_DRIVER_DESCRIPTION           "Pickering Resistor cards"
                    



/**************************************************************************** 
 *------------------------------ Useful Macros -----------------------------* 
 ****************************************************************************/



/**************************************************************************** 
 *---------------------------- Attribute Defines ---------------------------* 
 ****************************************************************************/

    /*- IVI Inherent Instrument Attributes ---------------------------------*/    

        /* User Options */
#define PIRES_ATTR_RANGE_CHECK                   IVI_ATTR_RANGE_CHECK                    /* ViBoolean */
#define PIRES_ATTR_QUERY_INSTRUMENT_STATUS       IVI_ATTR_QUERY_INSTRUMENT_STATUS        /* ViBoolean */
#define PIRES_ATTR_CACHE                         IVI_ATTR_CACHE                          /* ViBoolean */
#define PIRES_ATTR_SIMULATE                      IVI_ATTR_SIMULATE                       /* ViBoolean */
#define PIRES_ATTR_RECORD_COERCIONS              IVI_ATTR_RECORD_COERCIONS               /* ViBoolean */
#define PIRES_ATTR_INTERCHANGE_CHECK             IVI_ATTR_INTERCHANGE_CHECK              /* ViBoolean */
        
        /* Driver Information  */
#define PIRES_ATTR_SPECIFIC_DRIVER_PREFIX        IVI_ATTR_SPECIFIC_DRIVER_PREFIX         /* ViString, read-only  */
#define PIRES_ATTR_SUPPORTED_INSTRUMENT_MODELS   IVI_ATTR_SUPPORTED_INSTRUMENT_MODELS    /* ViString, read-only  */
#define PIRES_ATTR_GROUP_CAPABILITIES            IVI_ATTR_GROUP_CAPABILITIES             /* ViString, read-only  */
#define PIRES_ATTR_INSTRUMENT_MANUFACTURER       IVI_ATTR_INSTRUMENT_MANUFACTURER        /* ViString, read-only  */
#define PIRES_ATTR_INSTRUMENT_MODEL              IVI_ATTR_INSTRUMENT_MODEL               /* ViString, read-only  */
#define PIRES_ATTR_INSTRUMENT_FIRMWARE_REVISION  IVI_ATTR_INSTRUMENT_FIRMWARE_REVISION              /* ViString, read-only  */
#define PIRES_ATTR_SPECIFIC_DRIVER_REVISION      IVI_ATTR_SPECIFIC_DRIVER_REVISION       /* ViString, read-only  */
#define PIRES_ATTR_SPECIFIC_DRIVER_VENDOR        IVI_ATTR_SPECIFIC_DRIVER_VENDOR         /* ViString, read-only  */
#define PIRES_ATTR_SPECIFIC_DRIVER_DESCRIPTION   IVI_ATTR_SPECIFIC_DRIVER_DESCRIPTION    /* ViString, read-only  */
#define PIRES_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION IVI_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION /* ViInt32, read-only */
#define PIRES_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION IVI_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION /* ViInt32, read-only */

        /* Advanced Session Information */
#define PIRES_ATTR_LOGICAL_NAME                  IVI_ATTR_LOGICAL_NAME                   /* ViString, read-only  */
#define PIRES_ATTR_IO_RESOURCE_DESCRIPTOR        IVI_ATTR_IO_RESOURCE_DESCRIPTOR         /* ViString, read-only  */
#define PIRES_ATTR_DRIVER_SETUP                  IVI_ATTR_DRIVER_SETUP                   /* ViString, read-only  */        
    /*- Instrument-Specific Attributes -------------------------------------*/

#define PIRES_ATTR_ZERO_OFFSET				   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 3L)    /* ViReal64, channel-based */
#define PIRES_ATTR_RESISTANCE				   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 4L)    /* ViReal64, channel-based */

    /*- Obsolete Inherent Instrument Attributes ----------------------------*/
        // Driver Information 
#define PIRES_ATTR_SPECIFIC_DRIVER_MAJOR_VERSION IVI_ATTR_SPECIFIC_DRIVER_MAJOR_VERSION  // ViInt32,  read-only  
#define PIRES_ATTR_SPECIFIC_DRIVER_MINOR_VERSION IVI_ATTR_SPECIFIC_DRIVER_MINOR_VERSION  // ViInt32,  read-only  

        // Error Info
#define PIRES_ATTR_PRIMARY_ERROR                 IVI_ATTR_PRIMARY_ERROR                  // ViInt32 
#define PIRES_ATTR_SECONDARY_ERROR               IVI_ATTR_SECONDARY_ERROR                // ViInt32
#define PIRES_ATTR_ERROR_ELABORATION             IVI_ATTR_ERROR_ELABORATION              // ViString

        // Advanced Session Information
#define PIRES_ATTR_IO_SESSION_TYPE               IVI_ATTR_IO_SESSION_TYPE                // ViString, read-only  
#define PIRES_ATTR_IO_SESSION                    IVI_ATTR_IO_SESSION                     // ViSession, read-only

     // Obsolete Attribute Identifiers for Renamed Attributes
#define PIRES_ATTR_NUM_CHANNELS             	IVI_ATTR_CHANNEL_COUNT
#define PIRES_ATTR_QUERY_INSTR_STATUS       	PIRES_ATTR_QUERY_INSTRUMENT_STATUS            
#define PIRES_ATTR_RESOURCE_DESCRIPTOR      	PIRES_ATTR_IO_RESOURCE_DESCRIPTOR
	
#define PIRES_VAL_USE_ZEROING					0
#define PIRES_VAL_DONT_USE_ZEROING				1


/**************************************************************************** 
 *------------------------ Attribute Value Defines -------------------------* 
 ****************************************************************************/

        /* Instrument specific attribute value definitions */


/**************************************************************************** 
 *---------------- Instrument Driver Function Declarations -----------------* 
 ****************************************************************************/

    /*- Init and Close Functions -------------------------------------------*/
ViStatus _VI_FUNC  pires_init (ViRsrc resourceName, ViBoolean IDQuery,
                                  ViBoolean resetDevice, ViSession *vi);
ViStatus _VI_FUNC  pires_InitWithOptions (ViRsrc resourceName, ViBoolean IDQuery,
                                             ViBoolean resetDevice, ViConstString optionString, 
                                             ViSession *newVi);
ViStatus _VI_FUNC  pires_close (ViSession vi);   

    /*- Coercion Info Functions --------------------------------------------*/
ViStatus _VI_FUNC  pires_GetNextCoercionRecord (ViSession vi,
                                                   ViInt32 bufferSize,
                                                   ViChar record[]);
    
    /*- Locking Functions --------------------------------------------------*/
ViStatus _VI_FUNC  pires_LockSession (ViSession vi, ViBoolean *callerHasLock);   
ViStatus _VI_FUNC  pires_UnlockSession (ViSession vi, ViBoolean *callerHasLock);


ViStatus _VI_FUNC pires_ConfigureOutputResistance(
	ViSession		vi, 
	ViConstString	channelName,
	ViReal64		resistance
);
ViStatus _VI_FUNC pires_CalibrateOutputResistance(
	ViSession		vi, 
	ViConstString	channelName,
	ViReal64		resistance,
	ViInt32			calibType
);
ViStatus _VI_FUNC pires_ReadCalibration(
	ViSession		vi, 
	ViConstString	channelName
);
ViStatus _VI_FUNC pires_WriteCalibration(
	ViSession		vi, 
	ViConstString	channelName
);

	/*=CHANGE:===============================================================*
           Add instrument-specific public function declarations here.                           
         *============================================================END=CHANGE=*/
    
    /*- Error Functions ----------------------------------------------------*/
ViStatus _VI_FUNC  pires_error_query (ViSession vi, ViInt32 *errorCode,
                                         ViChar errorMessage[]);

ViStatus _VI_FUNC  pires_GetError (ViSession vi, ViStatus *code, 
                                      ViInt32 bufferSize, ViChar description[]);
ViStatus _VI_FUNC  pires_ClearError (ViSession vi);

ViStatus _VI_FUNC  pires_error_message (ViSession vi, ViStatus errorCode,
                                           ViChar errorMessage[256]);
    
    /*- Obsolete Error Handling Functions ----------------------------*/
ViStatus _VI_FUNC  pires_GetErrorInfo (ViSession vi, ViStatus *primaryError, 
                                          ViStatus *secondaryError, 
                                          ViChar errorElaboration[256]);
ViStatus _VI_FUNC  pires_ClearErrorInfo (ViSession vi);

    /*- Interchangeability Checking Functions ------------------------------*/
ViStatus _VI_FUNC pires_GetNextInterchangeWarning (ViSession vi, 
                                                      ViInt32 bufferSize, 
                                                      ViChar warnString[]);
ViStatus _VI_FUNC pires_ResetInterchangeCheck (ViSession vi);
ViStatus _VI_FUNC pires_ClearInterchangeWarnings (ViSession vi);

    /*- Utility Functions --------------------------------------------------*/
ViStatus _VI_FUNC pires_InvalidateAllAttributes (ViSession vi);
ViStatus _VI_FUNC  pires_reset (ViSession vi);
ViStatus _VI_FUNC  pires_ResetWithDefaults (ViSession vi);
ViStatus _VI_FUNC  pires_self_test (ViSession vi, ViInt16 *selfTestResult,
                                       ViChar selfTestMessage[]);
ViStatus _VI_FUNC  pires_revision_query (ViSession vi, 
                                            ViChar instrumentDriverRevision[],
                                            ViChar firmwareRevision[]);
ViStatus _VI_FUNC pires_Disable (ViSession vi);

    /*- Set, Get, and Check Attribute Functions ----------------------------*/
ViStatus _VI_FUNC  pires_GetAttributeViInt32 (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 *value);
ViStatus _VI_FUNC  pires_GetAttributeViReal64 (ViSession vi, ViConstString channelName, ViAttr attribute, ViReal64 *value);
ViStatus _VI_FUNC  pires_GetAttributeViString (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 bufSize, ViChar value[]); 
ViStatus _VI_FUNC  pires_GetAttributeViSession (ViSession vi, ViConstString channelName, ViAttr attribute, ViSession *value);
ViStatus _VI_FUNC  pires_GetAttributeViBoolean (ViSession vi, ViConstString channelName, ViAttr attribute, ViBoolean *value);

ViStatus _VI_FUNC  pires_SetAttributeViInt32 (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 value);
ViStatus _VI_FUNC  pires_SetAttributeViReal64 (ViSession vi, ViConstString channelName, ViAttr attribute, ViReal64 value);
ViStatus _VI_FUNC  pires_SetAttributeViString (ViSession vi, ViConstString channelName, ViAttr attribute, ViConstString value); 
ViStatus _VI_FUNC  pires_SetAttributeViSession (ViSession vi, ViConstString channelName, ViAttr attribute, ViSession value);
ViStatus _VI_FUNC  pires_SetAttributeViBoolean (ViSession vi, ViConstString channelName, ViAttr attribute, ViBoolean value);

ViStatus _VI_FUNC  pires_CheckAttributeViInt32 (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 value);
ViStatus _VI_FUNC  pires_CheckAttributeViReal64 (ViSession vi, ViConstString channelName, ViAttr attribute, ViReal64 value);
ViStatus _VI_FUNC  pires_CheckAttributeViString (ViSession vi, ViConstString channelName, ViAttr attribute, ViConstString value); 
ViStatus _VI_FUNC  pires_CheckAttributeViSession (ViSession vi, ViConstString channelName, ViAttr attribute, ViSession value);
ViStatus _VI_FUNC  pires_CheckAttributeViBoolean (ViSession vi, ViConstString channelName, ViAttr attribute, ViBoolean value);

/****************************************************************************
 *------------------------ Error And Completion Codes ----------------------*
 ****************************************************************************/
    /*=CHANGE:==============================================================*
        Add instrument-specific error codes here starting at
        IVI_SPECIFIC_ERROR_BASE.  Example:

#define PIRES_ERROR_NAME  (IVI_SPECIFIC_ERROR_BASE + 1)
     *===========================================================END=CHANGE=*/
#define PIRES_ERROR_PILPXI			(IVI_SPECIFIC_ERROR_BASE + 1)
#define PIRES_ERROR_COMPLEX_PATH	(IVI_SPECIFIC_ERROR_BASE + 2)

/**************************************************************************** 
 *---------------------------- End Include File ----------------------------* 
 ****************************************************************************/
#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif
#endif /* __PIRES_HEADER */




