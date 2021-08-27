/*****************************************************************************
 *  Pickering Resistor Cards  Instrument Driver                               
 *  LabWindows/CVI Instrument Driver                                     
 *  Original Release: 21. bøezna 2007                                  
 *  By: Zdenek Rykala, ELCOM, a.s.                              
 *      PH. +420 558 271 916   Fax +420 558 271 901                              
 *                                                                           
 *  Modification History:                                                    
 *                                                                           
 *       21. bøezna 2007 - Instrument Driver Created.                  
 *                                                                           
 *****************************************************************************/

#include <string.h>
#include <stdio.h>  
#include <WINDOWS.h>  
#include <ansi_c.h>
#include <pilpxi.h>
#include "pires.h"

/*****************************************************************************
 *--------------------- Resistor configuration ------------------------------*
 *****************************************************************************/
typedef struct _SDefaultCardEntry
{
	ViConstString	name;
	ViInt32			cardType;
	ViInt32			releys;
	ViInt32			channels;
} SDefaultCardEntry, *SDefaultCardEntryPtr;

typedef struct _SResConfig
{
	ViInt32		bit;
	ViReal64	DefinedValue;	
	ViReal64	CalibratedValue;
	ViBoolean	HasCalibration;
} SResConfig, *SResConfigPtr;

typedef struct _SChannelConfiguration
{
	ViInt32			ReleyCount;
	ViReal64		ZeroOffset;
	ViReal64		MaxResistance;
	SResConfigPtr	ResDefinition;
} SChannelConfiguration, *SChannelConfigurationPtr;

typedef struct _SResCard
{
	ViInt32		ChannelCount;
	SChannelConfigurationPtr	ChannelDefinition;
} SResCard, *SResCardPtr;

enum
{
	UNKNOWN_RESISTOR_CARD = 0,
	PI_40_295_021_10_8,
	PI_40_295_121_18_8,
	PI_40_295_021_05_12,
	PI_40_295_121_10_12,
	PI_40_295_021_05_16,
	PI_40_295_121_10_16,
	PI_40_295_021_03_24,
	PI_40_295_121_06_24,
} EPickeribCards;

/*****************************************************************************
 *--------------------- Resistor configuration ------------------------------*
 *****************************************************************************/
static SResConfig	gs_DefaultResistors[] = 
{										    /* 2.453 ohms */
	{0,     1.0, 0.0, VI_FALSE},			/* 3.394  => 941 ohms */
	{1,     2.0, 0.0, VI_FALSE},			/* 4.362  =>   1909 */
	{2,     4.0, 0.0, VI_FALSE},			/* 6.347 */
	{3,     8.0, 0.0, VI_FALSE},			/* 10.362 */
	{4,    16.0, 0.0, VI_FALSE},			/* 18.408 */
	{5,    32.0, 0.0, VI_FALSE},			/* 34.395 */
	{6,    64.0, 0.0, VI_FALSE},			/* 66.224 */
	{7,   128.0, 0.0, VI_FALSE},
	{8,   256.0, 0.0, VI_FALSE},
	{9,   512.0, 0.0, VI_FALSE},
	{10,  1024.0, 0.0, VI_FALSE},
	{11,  2048.0, 0.0, VI_FALSE},
	{12,  4096.0, 0.0, VI_FALSE},
	{13,  8192.0, 0.0, VI_FALSE},
	{14, 16384.0, 0.0, VI_FALSE},
	{15, 32768.0, 0.0, VI_FALSE},			/* 32800 */
	{16, 32768.0, 0.0, VI_FALSE},			/* 32800 */
	{17, 32768.0, 0.0, VI_FALSE},			/* 32800 */
	{18, 32768.0, 0.0, VI_FALSE},			/* 32800 */
	{19, 32768.0, 0.0, VI_FALSE},			/* 32800 */
	{20, 32768.0, 0.0, VI_FALSE},			/* 32800 */
	{21, 32768.0, 0.0, VI_FALSE},			/* 32800 */
	{22, 32768.0, 0.0, VI_FALSE},			/* 32800 */
	{23, 32768.0, 0.0, VI_FALSE},			/* 32800 */
};

static SDefaultCardEntry	gs_DefaultCards[] = 
{
	{"40-295-021-10/8",  PI_40_295_021_10_8,  8,  10},
	{"40-295-121-18/8",  PI_40_295_121_18_8,  8,  18},
	{"40-295-021-5/12",  PI_40_295_021_05_12, 12, 5},
	{"40-295-121-10/12", PI_40_295_121_10_12, 12, 10},
	{"40-295-021-5/16",  PI_40_295_021_05_16, 16, 05},
	{"40-295-121-10/16", PI_40_295_121_10_16, 16, 10},
	{"40-295-021-3/24",  PI_40_295_021_03_24, 24, 03},
	{"40-295-121-6/24",  PI_40_295_121_06_24, 24, 06},
	{NULL, -1, -1, -1}
};


/*****************************************************************************
 *--------------------- Hidden Attribute Declarations -----------------------*
 *****************************************************************************/
#define checkPILErr( fCal ) if ( error = (fCal), error != 0 ) { viCheckErr(PIRES_ERROR_PILPXI); }else error=error



#define PIRES_ATTR_OPC_TIMEOUT      			(IVI_SPECIFIC_PRIVATE_ATTR_BASE + 1L)       /* ViInt32 */
#define PIRES_ATTR_USE_SPECIFIC_SIMULATION  	(IVI_INHERENT_ATTR_BASE + 23L)  /* ViBoolean */
#define PIRES_ATTR_VISA_RM_SESSION  			(IVI_SPECIFIC_PRIVATE_ATTR_BASE + 5L)   /* ViSession */
#define PIRES_ATTR_OPC_CALLBACK     			(IVI_SPECIFIC_PRIVATE_ATTR_BASE + 6L)   /* ViAddr */
#define PIRES_ATTR_CHECK_STATUS_CALLBACK   	    (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 7L)   /* ViAddr */
#define PIRES_ATTR_USER_INTERCHANGE_CHECK_CALLBACK  (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 8L)   /* ViAddr */

#define PIRES_ATTR_INSTR_RESISTOR_CONFIGURATION		(IVI_SPECIFIC_PRIVATE_ATTR_BASE + 10L)       /* ViAddr */
#define PIRES_ATTR_INSTR_PATTERN					(IVI_SPECIFIC_PRIVATE_ATTR_BASE + 11L)       /* ViInt32 */

/*****************************************************************************
 *---------------------------- Useful Macros --------------------------------*
 *****************************************************************************/

    /*- I/O buffer size -----------------------------------------------------*/
#define BUFFER_SIZE                             512L        

#define PIRES_IO_SESSION_TYPE   "PILPXI"

    /*- 488.2 Event Status Register (ESR) Bits ------------------------------*/
#define IEEE_488_2_ERROR_MASK              0x3C


/*****************************************************************************
 *-------------- Utility Function Declarations (Non-Exported) ---------------*
 *****************************************************************************/
static ViStatus pires_IviInit (ViRsrc resourceName, ViBoolean IDQuery, 
                                     ViBoolean reset, ViSession vi);
static ViStatus pires_IviClose (ViSession vi);   

static ViStatus pires_InitAttributes(
	ViSession 		vi,
	ViAddr			pConfig
);
static ViStatus pires_DefaultInstrSetup (ViSession openInstrSession);
static ViStatus pires_CheckStatus (ViSession vi);

static ViStatus pires_CreateDefaultResistorDefinition(
	ViSession	vi,
	ViInt32 	type,
	ViAddr*		pconfig,
	ViChar		channelList[256]
);
static ViStatus pires_DestroyConfiguration(
	ViSession	vi,
	ViAddr		pconfig
);
static ViStatus pires_DetectDefaultCard(
	ViSession	vi,
	ViInt32*	pType
);

/*****************************************************************************
 *----------------- Callback Declarations (Non-Exported) --------------------*
 *****************************************************************************/
    /*- Global Session Callbacks --------------------------------------------*/
    
static ViStatus _VI_FUNC pires_CheckStatusCallback (ViSession vi, ViSession io);

/*- Attribute callbacks -------------------------------------------------*/
static ViStatus _VI_FUNC piresAttrDriverRevision_ReadCallback(
	ViSession		vi, 
	ViSession		io,
	ViConstString	channelName, 
	ViAttr			attributeId,
	const ViConstString cacheValue
);
static ViStatus _VI_FUNC piresAttrInstrumentManufacturer_ReadCallback(
	ViSession		vi, 
	ViSession		io,
	ViConstString	channelName, 
	ViAttr			attributeId,
	const ViConstString cacheValue
);
static ViStatus _VI_FUNC piresAttrInstrumentModel_ReadCallback(
	ViSession		vi, 
	ViSession		io,
	ViConstString	channelName, 
	ViAttr			attributeId,
	const ViConstString cacheValue
);
static ViStatus _VI_FUNC piresAttrResistance_CheckCallback(
	ViSession		vi,
	ViConstString	channelName,
	ViAttr 			attributeId,
	ViReal64 		value
);
static ViStatus _VI_FUNC piresAttrResistance_ReadCallback(
	ViSession vi,
	ViSession io,
	ViConstString channelName,
	ViAttr attributeId,
	ViReal64* value
);
static ViStatus _VI_FUNC piresAttrResistance_WriteCallback(
	ViSession vi,
	ViSession io,
	ViConstString channelName,
	ViAttr attributeId,
	ViReal64 value
);
static ViStatus _VI_FUNC piresAttrZeroOffset_ReadCallback(
	ViSession vi,
	ViSession io,
	ViConstString channelName,
	ViAttr attributeId,
	ViReal64* value
);
static ViStatus _VI_FUNC piresAttrZeroOffset_WriteCallback(
	ViSession vi,
	ViSession io,
	ViConstString channelName,
	ViAttr attributeId,
	ViReal64 value
);
static ViStatus _VI_FUNC piresAttrInstrPattern_WriteCallback(
	ViSession		vi,
	ViSession		io,
	ViConstString	channelName,
	ViAttr			attributeId,
	ViInt32			value
);
static ViStatus _VI_FUNC piresAttrInstrPattern_ReadCallback(
	ViSession		vi,
	ViSession		io,
	ViConstString	channelName,
	ViAttr			attributeId,
	ViInt32*		value
);


/*****************************************************************************
 *------------ User-Callable Functions (Exportable Functions) ---------------*
 *****************************************************************************/

/*****************************************************************************
 * Function: pires_init   
 * Purpose:  VXIplug&play required function.  Calls the   
 *           pires_InitWithOptions function.   
 *****************************************************************************/
ViStatus _VI_FUNC pires_init (ViRsrc resourceName, ViBoolean IDQuery,
                                 ViBoolean resetDevice, ViSession *newVi)
{   
    ViStatus    error = VI_SUCCESS;

    if (newVi == VI_NULL)
        {
        Ivi_SetErrorInfo (VI_NULL, VI_FALSE, IVI_ERROR_INVALID_PARAMETER, 
                          VI_ERROR_PARAMETER4, "Null address for Instrument Handle");
        checkErr( IVI_ERROR_INVALID_PARAMETER);
        }

    checkErr( pires_InitWithOptions (resourceName, IDQuery, resetDevice, 
                                        "", newVi));

Error:  
    return error;
}

/*****************************************************************************
 * Function: pires_InitWithOptions                                       
 * Purpose:  This function creates a new IVI session and calls the 
 *           IviInit function.                                     
 *****************************************************************************/
ViStatus _VI_FUNC pires_InitWithOptions(
    ViRsrc          resourceName,
    ViBoolean       IDQuery,
    ViBoolean       resetDevice,
    ViConstString   optionString,
    ViSession      *newVi
)
{   
    ViStatus    error = VI_SUCCESS;
    ViSession   vi = VI_NULL;
    ViChar      newResourceName[IVI_MAX_MESSAGE_BUF_SIZE];
    ViChar      newOptionString[IVI_MAX_MESSAGE_BUF_SIZE];
    ViBoolean   isLogicalName;
    
    if (newVi == VI_NULL)
        {
        Ivi_SetErrorInfo (VI_NULL, VI_FALSE, IVI_ERROR_INVALID_PARAMETER, 
                          VI_ERROR_PARAMETER5, "Null address for Instrument Handle");
        checkErr( IVI_ERROR_INVALID_PARAMETER);
        }

    *newVi = VI_NULL;
    
    checkErr( Ivi_GetInfoFromResourceName( resourceName, (ViString)optionString,
                                           newResourceName,
                                           newOptionString,
                                           &isLogicalName));
    
        /* create a new interchangeable driver */
    checkErr( Ivi_SpecificDriverNew ("pires", newOptionString, &vi));  
    if (!isLogicalName)
    {
        ViInt32 oldFlag = 0;
        
        checkErr (Ivi_GetAttributeFlags (vi, IVI_ATTR_IO_RESOURCE_DESCRIPTOR, &oldFlag));
        checkErr (Ivi_SetAttributeFlags (vi, IVI_ATTR_IO_RESOURCE_DESCRIPTOR, oldFlag & 0xfffb | 0x0010));
        checkErr (Ivi_SetAttributeViString (vi, "", IVI_ATTR_IO_RESOURCE_DESCRIPTOR, 0, newResourceName));
    }
        /* init the driver */
    checkErr( pires_IviInit (newResourceName, IDQuery, resetDevice, vi)); 
    if (isLogicalName)
        checkErr( Ivi_ApplyDefaultSetup (vi));
    *newVi = vi;
    
Error:
    if (error < VI_SUCCESS) 
        Ivi_Dispose (vi);
        
    return error;
}

/*****************************************************************************
 * Function: pires_close                                                           
 * Purpose:  This function closes the instrument.                            
 *
 *           Note:  This function must unlock the session before calling
 *           Ivi_Dispose.
 *****************************************************************************/
ViStatus _VI_FUNC pires_close (ViSession vi)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Ivi_LockSession (vi, VI_NULL));
    
    checkErr( pires_IviClose (vi));

Error:    
    Ivi_UnlockSession (vi, VI_NULL);
    Ivi_Dispose (vi);  
    return error;
}

/*****************************************************************************
 * Function: pires_reset                                                         
 * Purpose:  This function resets the instrument.                          
 *****************************************************************************/
ViStatus _VI_FUNC pires_reset (ViSession vi)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Ivi_LockSession (vi, VI_NULL));

	if (!Ivi_Simulating(vi))                /* call only when locked */         
	{                                                                              
	    ViSession   io = Ivi_IOSession(vi); /* call only when locked */         
	                                                                                   
	    checkErr( Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
	
	    checkPILErr( PIL_ClearCard( io));
	} 
	
	checkErr( pires_DefaultInstrSetup (vi));                                
	
	checkErr( pires_CheckStatus (vi));                                      

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: pires_ResetWithDefaults
 * Purpose:  This function resets the instrument and applies default settings
 *           from the IVI Configuration Store based on the logical name
 *           from which the session was created.
 *****************************************************************************/
ViStatus _VI_FUNC pires_ResetWithDefaults (ViSession vi)
{
    ViStatus error = VI_SUCCESS;
    
    checkErr( Ivi_LockSession (vi, VI_NULL));
    checkErr( pires_reset(vi));
    checkErr( Ivi_ApplyDefaultSetup(vi));
    
Error:
    Ivi_UnlockSession(vi, VI_NULL);
    return error;
}

/**************************************************************************** 
 *  Function: pires_Disable
 *  Purpose:  This function places the instrument in a quiescent state as 
 *            quickly as possible.
 ****************************************************************************/
ViStatus _VI_FUNC pires_Disable (ViSession vi)
{
    ViStatus error = VI_SUCCESS;
    
    checkErr( Ivi_LockSession (vi, VI_NULL));
    checkErr( pires_reset(vi));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: pires_self_test                                                       
 * Purpose:  This function executes the instrument self-test and returns the 
 *           result.                                                         
 *****************************************************************************/
ViStatus _VI_FUNC pires_self_test (ViSession vi, ViInt16 *testResult, 
                                      ViChar testMessage[])
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Ivi_LockSession (vi, VI_NULL));

    if (testResult == VI_NULL)
        viCheckParm( IVI_ERROR_INVALID_PARAMETER, 2, "Null address for Test Result");
    if (testMessage == VI_NULL)
        viCheckParm( IVI_ERROR_INVALID_PARAMETER, 3, "Null address for Test Message");

	if (!Ivi_Simulating(vi))                /* call only when locked */
	    {
	    ViSession   io = Ivi_IOSession(vi); /* call only when locked */
	
	    checkErr( Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
	
	        /*=CHANGE: ==========================================================*
	           Insert code here to execute your instruments self-test.               
	         *===================================================================*/
	    }
	else
	    {
	        /* Simulate Self Test */
	    *testResult = 0;
	    strcpy (testMessage, "No error.");
	    }
	
	checkErr( pires_CheckStatus (vi));

Error:
    Ivi_UnlockSession(vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: pires_revision_query                                                  
 * Purpose:  This function returns the driver and instrument revisions.      
 *****************************************************************************/
ViStatus _VI_FUNC pires_revision_query (ViSession vi, ViChar driverRev[], 
                                           ViChar instrRev[])
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Ivi_LockSession (vi, VI_NULL));

    if (driverRev == VI_NULL)
        viCheckParm( IVI_ERROR_INVALID_PARAMETER, 2, "Null address for Driver Revision");
    if (instrRev == VI_NULL)
        viCheckParm( IVI_ERROR_INVALID_PARAMETER, 3, "Null address for Instrument Revision");

    checkErr( Ivi_GetAttributeViString (vi, VI_NULL, PIRES_ATTR_SPECIFIC_DRIVER_REVISION, 
                                        0, 256, driverRev));
    checkErr( Ivi_GetAttributeViString (vi, "", PIRES_ATTR_INSTRUMENT_FIRMWARE_REVISION, 
                                        0, 256, instrRev));
    checkErr( pires_CheckStatus (vi));

Error:    
    Ivi_UnlockSession(vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: pires_error_query                                                     
 * Purpose:  This function queries the instrument error queue and returns   
 *           the result.                                                     
 *****************************************************************************/
ViStatus _VI_FUNC pires_error_query (ViSession vi, ViInt32 *errCode, 
                                        ViChar errMessage[])
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Ivi_LockSession (vi, VI_NULL));
    
    if (errCode == VI_NULL)
        viCheckParm( IVI_ERROR_INVALID_PARAMETER, 2, "Null address for Error Code");
    if (errMessage == VI_NULL)
        viCheckParm( IVI_ERROR_INVALID_PARAMETER, 3, "Null address for Error Message");

	if (!Ivi_Simulating(vi))                /* call only when locked */
	{
	    ViSession   io = Ivi_IOSession(vi); /* call only when locked */
	
	    checkErr( Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
		*errCode = PIL_Status( io);
		
	}
	else
	{
	        /* Simulate Error Query */
	    *errCode = 0;
	    strcpy (errMessage, "No error.");
	}
	
	checkErr( pires_CheckStatus (vi));

Error:
    Ivi_UnlockSession(vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: pires_error_message                                                  
 * Purpose:  This function translates the error codes returned by this       
 *           instrument driver into user-readable strings.  
 *
 *           Note:  The caller can pass VI_NULL for the vi parameter.  This 
 *           is useful if one of the init functions fail.
 *****************************************************************************/
ViStatus _VI_FUNC pires_error_message (ViSession vi, ViStatus errorCode,
                                          ViChar errorMessage[256])
{
    ViStatus    error = VI_SUCCESS;
    
    static      IviStringValueTable errorTable = 
        {
            {PIRES_ERROR_PILPXI,   		"Specific error of Pilpxi"},  
			{PIRES_ERROR_COMPLEX_PATH,  "Only one resistor can be in path during calibration."},  

			{VI_NULL,                               VI_NULL}
        };
        
    if (vi)
        Ivi_LockSession(vi, VI_NULL);

        /* all VISA and IVI error codes are handled as well as codes in the table */
    if (errorMessage == VI_NULL)
        viCheckParm( IVI_ERROR_INVALID_PARAMETER, 3, "Null address for Error Message");

    checkErr( Ivi_GetSpecificDriverStatusDesc(vi, errorCode, errorMessage, errorTable));

Error:
    if (vi)
        Ivi_UnlockSession(vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: pires_InvalidateAllAttributes
 * Purpose:  This function invalidates the cached value of all attributes.
 *****************************************************************************/
ViStatus _VI_FUNC pires_InvalidateAllAttributes (ViSession vi)
{
    return Ivi_InvalidateAllAttributes(vi);
}

    /*- Obsolete Error Handling Functions ----------------------------*/
ViStatus _VI_FUNC pires_GetErrorInfo (ViSession vi, ViStatus *primaryError, 
                                         ViStatus *secondaryError, ViChar errorElaboration[256])  
{                                                                                                           
    return Ivi_GetErrorInfo(vi, primaryError, secondaryError, errorElaboration);                                
}                                                                                                           
ViStatus _VI_FUNC pires_ClearErrorInfo (ViSession vi)                                                        
{                                                                                                           
    return Ivi_ClearErrorInfo (vi);                                                                             
}


/*****************************************************************************
 * Function: pires_ConfigureOutputResistance
 * Purpose:  This function configures specified resistance on the output
 *****************************************************************************/
ViStatus _VI_FUNC pires_ConfigureOutputResistance(
	ViSession		vi, 
	ViConstString	channelName,
	ViReal64		resistance
)
{
	return Ivi_SetAttributeViReal64( vi,
									channelName,
									PIRES_ATTR_RESISTANCE,
									IVI_VAL_DIRECT_USER_CALL,
									resistance);
}

/*****************************************************************************
 * Function: pires_ConfigureOutputResistance
 * Purpose:  This function calibrate card.
 *****************************************************************************/
ViStatus _VI_FUNC pires_CalibrateOutputResistance(
	ViSession		vi, 
	ViConstString	channelName,
	ViReal64		resistance,
	ViInt32			calibType
)
{
	ViStatus    	error = VI_SUCCESS;
	ViReal64		value = resistance;
	ViConstString	pcoerced = VI_NULL;
	SResCardPtr		pcard = NULL;

	checkErr( Ivi_LockSession (vi, VI_NULL));
	
	viCheckParm( Ivi_CoerceChannelName(vi, channelName, &pcoerced), 2, "Channel Name");
	
	if ( calibType == PIRES_VAL_USE_ZEROING )
	{
		ViReal64		zero = 0.0;
		
		checkErr( Ivi_GetAttributeViReal64( vi, channelName, PIRES_ATTR_ZERO_OFFSET, 0, &zero));
		value -= zero;
	}
	
	checkErr( Ivi_GetAttributeViAddr( vi,
				 					  VI_NULL,
									  PIRES_ATTR_INSTR_RESISTOR_CONFIGURATION,
									  0, (ViAddr*)&pcard));
	
	if (!Ivi_Simulating(vi))                /* call only when locked */         
	{                                                                              
	    ViSession   io = Ivi_IOSession(vi); /* call only when locked */
		int			i, index = atoi(pcoerced);
		ViInt32		pattern = 0;
		SChannelConfigurationPtr	pdefinition = VI_NULL;
		
		checkErr( Ivi_GetAttributeViInt32( vi, channelName, PIRES_ATTR_INSTR_PATTERN, 0, &pattern));
		pattern = ~pattern;
		
		pdefinition = pcard->ChannelDefinition + index;

		for( i = pdefinition->ReleyCount; pattern && --i >=0; )		
		{
			if ( pattern == (1 << pdefinition->ResDefinition[i].bit) )
			{
				DWORD	dvalue = 0.0;
				
				pdefinition->ResDefinition[i].CalibratedValue = value;
				pdefinition->ResDefinition[i].HasCalibration  = VI_TRUE;
				
				*((ViReal32*)&dvalue) = (ViReal32)value;
				break;
			}
		}
		if ( i < 0 )
		{
			viCheckErr( PIRES_ERROR_COMPLEX_PATH);
		}
	}

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: pires_ReadCalibration
 * Purpose:  This function read calibration from card
 *****************************************************************************/
ViStatus _VI_FUNC pires_ReadCalibration(
	ViSession		vi, 
	ViConstString	channelName
)
{
    ViStatus    	error = VI_SUCCESS;
	ViConstString	pcoerced = VI_NULL;
	SResCardPtr		pcard = NULL;
	SChannelConfigurationPtr	pdefinition = VI_NULL;
	

    checkErr( Ivi_LockSession (vi, VI_NULL));
	
	viCheckParm( Ivi_CoerceChannelName(vi, channelName, &pcoerced), 2, "Channel Name");
	
	checkErr( Ivi_GetAttributeViAddr( vi,
				 					  VI_NULL,
									  PIRES_ATTR_INSTR_RESISTOR_CONFIGURATION,
									  0, (ViAddr*)&pcard));

	if (!Ivi_Simulating(vi))                /* call only when locked */         
	{                                                                              
	    ViSession   io = Ivi_IOSession(vi); /* call only when locked */         
		DWORD		dvalue;
		int			i, index = atoi(pcoerced);
		
	                                                                                   
	    checkErr( Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

	    dvalue = 0;
		pdefinition = pcard->ChannelDefinition + index;
		
		for( i = pdefinition->ReleyCount; --i >=0; )
		{
			checkPILErr( PIL_ReadCal( io, index, i, &dvalue));

			if ( dvalue )
			{
				ViReal32	fvalue = *((ViReal32*)&dvalue);

				/* memcpy(&fvalue, &dvalue, sizeof(&dvalue)); */
				pdefinition->ResDefinition[i].CalibratedValue = (ViReal64)fvalue;
				pdefinition->ResDefinition[i].HasCalibration  = VI_TRUE;
			}
		}

	} 
	
	checkErr( pires_CheckStatus (vi));                                      

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: pires_WriteCalibration
 * Purpose:  This function read calibration from card
 *****************************************************************************/
ViStatus _VI_FUNC pires_WriteCalibration(
	ViSession		vi, 
	ViConstString	channelName
)
{
    ViStatus    	error = VI_SUCCESS;
	ViConstString	pcoerced = VI_NULL;
	SResCardPtr		pcard = NULL;
	SChannelConfigurationPtr	pdefinition = VI_NULL;
	

    checkErr( Ivi_LockSession (vi, VI_NULL));
	
	viCheckParm( Ivi_CoerceChannelName(vi, channelName, &pcoerced), 2, "Channel Name");
	
	checkErr( Ivi_GetAttributeViAddr( vi,
				 					  VI_NULL,
									  PIRES_ATTR_INSTR_RESISTOR_CONFIGURATION,
									  0, (ViAddr*)&pcard));

	if (!Ivi_Simulating(vi))                /* call only when locked */         
	{                                                                              
	    ViSession   io = Ivi_IOSession(vi); /* call only when locked */         
		DWORD		dvalue;
		int			i, index = atoi(pcoerced);
		
	                                                                                   
	    checkErr( Ivi_SetNeedToCheckStatus (vi, VI_TRUE));

	    dvalue = 0;
		pdefinition = pcard->ChannelDefinition + index;

		for( i = pdefinition->ReleyCount; --i >=0; )
		{
			
			dvalue =  ( pdefinition->ResDefinition[i].HasCalibration ) ? 
				(*((DWORD*)&(pdefinition->ResDefinition[i].CalibratedValue))) : 0;
				
			checkPILErr( PIL_WriteCal( io, index, i, dvalue));
		}

	} 
	
	checkErr( pires_CheckStatus (vi));                                      

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}


/*****************************************************************************
 * Function: pires_GetError and pires_ClearError Functions                       
 * Purpose:  These functions enable the instrument driver user to  
 *           get or clear the error information the driver associates with the
 *           IVI session.                                                        
 *****************************************************************************/
ViStatus _VI_FUNC pires_GetError (ViSession vi, 
                                     ViStatus *errorCode, 
                                     ViInt32 bufferSize,
                                     ViChar description[])  
{
    ViStatus error = VI_SUCCESS;
    ViStatus primary = VI_SUCCESS,
             secondary = VI_SUCCESS;
    ViChar   elaboration[256] = "";
    ViChar   errorMessage[1024] = "";
    ViChar  *appendPoint = errorMessage;
    ViInt32  actualSize = 0;
    ViBoolean locked = VI_FALSE;

    /* Lock Session */
    if (vi != 0)
        {
        checkErr( Ivi_LockSession(vi, &locked));
        }

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
        checkErr( Ivi_GetErrorInfo (vi, &primary, &secondary, elaboration));
        }

    else
        {
        checkErr( Ivi_GetAttributeViString(vi, VI_NULL, IVI_ATTR_ERROR_ELABORATION, 0, 256, elaboration));
        checkErr( Ivi_GetAttributeViInt32(vi, VI_NULL, IVI_ATTR_SECONDARY_ERROR, 0, &secondary));
        checkErr( Ivi_GetAttributeViInt32(vi, VI_NULL, IVI_ATTR_PRIMARY_ERROR, 0, &primary));
        }
        
    /* Format data */
    if (primary != VI_SUCCESS)
        {
        ViChar msg[256] = "";
        checkErr( pires_error_message (vi, primary, msg));
        appendPoint += sprintf(appendPoint, "Primary Error: (Hex 0x%08X) %s\n", primary, msg);
        }
    
    if (secondary != VI_SUCCESS)
        {
        ViChar msg[256] = "";
        checkErr( pires_error_message (vi, secondary, msg));
        appendPoint += sprintf(appendPoint, "Secondary Error: (Hex 0x%08X) %s\n", secondary, msg);
        }
    
    if (elaboration[0])
        {
        sprintf(appendPoint, "Elaboration: %s", elaboration);
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
        *errorCode = primary;
        }
    
Error:
    /* Unlock Session */
    Ivi_UnlockSession (vi, &locked);
    return error;
}   

ViStatus _VI_FUNC pires_ClearError (ViSession vi)                                                        
{                                                                                                           
    return Ivi_ClearErrorInfo (vi);                                                                             
}

/*****************************************************************************
 * Function: pires_GetNextCoercionRecord                        
 * Purpose:  This function enables the instrument driver user to obtain
 *           the coercion information associated with the IVI session.                                                              
 *           This function retrieves and clears the oldest instance in which 
 *           the instrument driver coerced a value the instrument driver user
 *           specified to another value.                     
 *****************************************************************************/
ViStatus _VI_FUNC pires_GetNextCoercionRecord (ViSession vi,
                                                  ViInt32 bufferSize,
                                                  ViChar  record[])
{
    return Ivi_GetNextCoercionString (vi, bufferSize, record);
}

/**************************************************************************** 
 *  Function: pires_GetNextInterchangeWarning,
 *            pires_ResetInterchangeCheck, and
 *            pires_ClearInterchangeWarnings
 *  Purpose:  These functions allow the user to retrieve interchangeability
 *            warnings, reset the driver's interchangeability checking 
 *            state, and clear all previously logged interchangeability warnings.
 ****************************************************************************/
ViStatus _VI_FUNC pires_GetNextInterchangeWarning (ViSession vi, 
                                                      ViInt32 bufferSize, 
                                                      ViChar warnString[])
{
    return Ivi_GetNextInterchangeCheckString (vi, bufferSize, warnString);
}

ViStatus _VI_FUNC pires_ResetInterchangeCheck (ViSession vi)
{
    return Ivi_ResetInterchangeCheck (vi);
}

ViStatus _VI_FUNC pires_ClearInterchangeWarnings (ViSession vi)
{
    return Ivi_ClearInterchangeWarnings (vi);
}

/*****************************************************************************
 * Function: pires_LockSession and pires_UnlockSession Functions                        
 * Purpose:  These functions enable the instrument driver user to lock the 
 *           session around a sequence of driver calls during which other
 *           execution threads must not disturb the instrument state.
 *                                                                          
 *           NOTE:  The callerHasLock parameter must be a local variable 
 *           initialized to VI_FALSE and passed by reference, or you can pass 
 *           VI_NULL.                     
 *****************************************************************************/
ViStatus _VI_FUNC pires_LockSession (ViSession vi, ViBoolean *callerHasLock)  
{                                              
    return Ivi_LockSession(vi,callerHasLock);      
}                                              
ViStatus _VI_FUNC pires_UnlockSession (ViSession vi, ViBoolean *callerHasLock) 
{                                              
    return Ivi_UnlockSession(vi,callerHasLock);    
}   


/*****************************************************************************
 * Function: pires_GetAttribute<type> Functions                                    
 * Purpose:  These functions enable the instrument driver user to get 
 *           attribute values directly.  There are typesafe versions for 
 *           ViInt32, ViReal64, ViString, ViBoolean, and ViSession attributes.                                         
 *****************************************************************************/
ViStatus _VI_FUNC pires_GetAttributeViInt32(
	ViSession		vi,
	ViConstString	channelName, 
	ViAttr			attributeId,
	ViInt32		   *value
)
{                                                                                                           
    return Ivi_GetAttributeViInt32( vi,
									channelName,
									attributeId,
									IVI_VAL_DIRECT_USER_CALL, 
                                    value);
}                                                                                                           
ViStatus _VI_FUNC pires_GetAttributeViReal64(
	ViSession		vi,
	ViConstString	channelName, 
	ViAttr			attributeId,
	ViReal64	   *value
)
{                                                                                                           
    return Ivi_GetAttributeViReal64( vi,
									 channelName,
									 attributeId,
									 IVI_VAL_DIRECT_USER_CALL, 
                                     value);
}                                                                                                           
ViStatus _VI_FUNC pires_GetAttributeViString(
	ViSession		vi,
	ViConstString	channelName, 
	ViAttr			attributeId,
	ViInt32 		bufSize,
	ViChar		   	value[]
)
{   
    return Ivi_GetAttributeViString( vi,
									 channelName,
									 attributeId,
									 IVI_VAL_DIRECT_USER_CALL, 
                                     bufSize,
									 value);
}   
ViStatus _VI_FUNC pires_GetAttributeViBoolean(
	ViSession		vi,
	ViConstString	channelName, 
	ViAttr			attributeId,
	ViBoolean	   *value
)
{                                                                                                           
    return Ivi_GetAttributeViBoolean( vi,
									  channelName,
									  attributeId,
									  IVI_VAL_DIRECT_USER_CALL, 
                                      value);
}
ViStatus _VI_FUNC pires_GetAttributeViSession(
	ViSession		vi,
	ViConstString	channelName, 
	ViAttr			attributeId,
	ViSession	   *value
)
{                                                                                                           
    return Ivi_GetAttributeViSession( vi,
									  channelName,
									  attributeId,
									  IVI_VAL_DIRECT_USER_CALL, 
                                      value);
}                                                                                                           

/*****************************************************************************
 * Function: pires_SetAttribute<type> Functions                                    
 * Purpose:  These functions enable the instrument driver user to set 
 *           attribute values directly.  There are typesafe versions for 
 *           ViInt32, ViReal64, ViString, ViBoolean, and ViSession datatypes.                                         
 *****************************************************************************/
ViStatus _VI_FUNC pires_SetAttributeViInt32(
	ViSession		vi,
	ViConstString	channelName, 
	ViAttr			attributeId,
	ViInt32		    value
)
{                                                                                                           
    return Ivi_SetAttributeViInt32 (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                    value);
}                                                                                                           
ViStatus _VI_FUNC pires_SetAttributeViReal64(
	ViSession		vi,
	ViConstString	channelName, 
	ViAttr			attributeId,
	ViReal64	    value
)
{                                                                                                           
    return Ivi_SetAttributeViReal64 (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                     value);
}                                                                                                           
ViStatus _VI_FUNC pires_SetAttributeViString(
	ViSession		vi,
	ViConstString	channelName, 
	ViAttr			attributeId,
	ViConstString   value
)
{   
    return Ivi_SetAttributeViString (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                     value);
}   
ViStatus _VI_FUNC pires_SetAttributeViBoolean(
	ViSession		vi,
	ViConstString	channelName, 
	ViAttr			attributeId,
	ViBoolean	    value
)
{                                                                                                           
    return Ivi_SetAttributeViBoolean (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                      value);
}                                                                                                           
ViStatus _VI_FUNC pires_SetAttributeViSession(
	ViSession		vi,
	ViConstString	channelName, 
	ViAttr			attributeId,
	ViSession	    value
)
{                                                                                                           
    return Ivi_SetAttributeViSession (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                      value);
}                                                                                                           

/*****************************************************************************
 * Function: pires_CheckAttribute<type> Functions                                  
 * Purpose:  These functions enable the instrument driver user to check  
 *           attribute values directly.  These functions check the value you
 *           specify even if you set the PIRES_ATTR_RANGE_CHECK 
 *           attribute to VI_FALSE.  There are typesafe versions for ViInt32, 
 *           ViReal64, ViString, ViBoolean, and ViSession datatypes.                         
 *****************************************************************************/
ViStatus _VI_FUNC pires_CheckAttributeViInt32(
	ViSession		vi,
	ViConstString	channelName, 
	ViAttr			attributeId,
	ViInt32		    value
)
{                                                                                                           
    return Ivi_CheckAttributeViInt32( vi,
									  channelName,
									  attributeId,
									  IVI_VAL_DIRECT_USER_CALL, 
                                      value);
}                                                                                                           
ViStatus _VI_FUNC pires_CheckAttributeViReal64(
	ViSession		vi,
	ViConstString	channelName, 
	ViAttr			attributeId,
	ViReal64	    value
)
{                                                                                                           
    return Ivi_CheckAttributeViReal64( vi,
									   channelName,
									   attributeId,
									   IVI_VAL_DIRECT_USER_CALL, 
                                       value);
}                                                                                                           
ViStatus _VI_FUNC pires_CheckAttributeViString(
	ViSession		vi,
	ViConstString	channelName, 
	ViAttr			attributeId,
	ViConstString   value
)
{   
    return Ivi_CheckAttributeViString( vi,
									   channelName,
									   attributeId,
									   IVI_VAL_DIRECT_USER_CALL, 
                                       value);
}   
ViStatus _VI_FUNC pires_CheckAttributeViBoolean(
	ViSession		vi,
	ViConstString	channelName, 
	ViAttr			attributeId,
	ViBoolean	    value
)
{                                                                                                           
    return Ivi_CheckAttributeViBoolean( vi,
										channelName,
										attributeId,
										IVI_VAL_DIRECT_USER_CALL, 
                                      	value);
}                                                                                                           
ViStatus _VI_FUNC pires_CheckAttributeViSession(
	ViSession		vi,
	ViConstString	channelName, 
	ViAttr			attributeId,
	ViSession	    value
)
{                                                                                                           
    return Ivi_CheckAttributeViSession( vi,
										channelName,
										attributeId,
										IVI_VAL_DIRECT_USER_CALL, 
                                      	value);
}

/*****************************************************************************
 *-------------------- Utility Functions (Not Exported) ---------------------*
 *****************************************************************************/


/*****************************************************************************
 * Function: pires_IviInit                                                       
 * Purpose:  This function is called by pires_InitWithOptions  
 *           or by an IVI class driver.  This function initializes the I/O 
 *           interface, optionally resets the device, optionally performs an
 *           ID query, and sends a default setup to the instrument.                
 *****************************************************************************/
static ViStatus pires_IviInit (ViRsrc resourceName, ViBoolean IDQuery,
                                    ViBoolean reset, ViSession vi)
{
    ViStatus    error = VI_SUCCESS;
	ViAddr		pconfig = VI_NULL;
	ViInt32		type = PI_40_295_121_10_16;
	ViChar		channelList[256]="";
#if 0
	ViSession   io = VI_NULL;
#else
	DWORD		io = 0;
#endif
    

     /* Add attributes */
    if (!Ivi_Simulating(vi))
	{
		int	bus, slot;

		sscanf(resourceName, "PXI%d::%d::INSTR", &bus, &slot);
		checkPILErr( PIL_OpenSpecifiedCard( bus, slot, &io));
		checkErr( Ivi_SetAttributeViSession (vi, VI_NULL, IVI_ATTR_IO_SESSION, 0, io));  
		
		checkErr( pires_DetectDefaultCard( vi, &type));
	}
	
	checkErr( pires_CreateDefaultResistorDefinition( vi,
													 type,
													 &pconfig,
													 channelList));
	checkErr( Ivi_BuildChannelTable (vi, channelList, VI_FALSE, VI_NULL));
	checkErr( pires_InitAttributes (vi, pconfig));
        
    /*- Reset instrument ----------------------------------------------------*/
    if (reset) 
        checkErr( pires_reset (vi));
    else  /*- Send Default Instrument Setup ---------------------------------*/
        checkErr( pires_DefaultInstrSetup (vi));
	
	/*- Identification Query ------------------------------------------------*/
	if (IDQuery)
    {
		ViChar	buffer[256]="";

		checkErr( Ivi_GetAttributeViString( vi,
										    VI_NULL,
											PIRES_ATTR_INSTRUMENT_MODEL,
											0,
											sizeof(buffer), buffer));
		if ( !strstr( PIRES_SUPPORTED_INSTRUMENT_MODELS, buffer))
		{
			viCheckErr( VI_ERROR_FAIL_ID_QUERY);
		}
    }

    checkErr( pires_CheckStatus (vi));

Error:
    if (error < VI_SUCCESS)
	{
        if (!Ivi_Simulating (vi) && io)
            PIL_CloseSpecifiedCard( io);
	}
    return error;
}

/*****************************************************************************
 * Function: pires_IviClose                                                        
 * Purpose:  This function performs all of the drivers clean-up operations   
 *           except for closing the IVI session.  This function is called by 
 *           pires_close or by an IVI class driver. 
 *
 *           Note:  This function must close the I/O session and set 
 *           IVI_ATTR_IO_SESSION to 0.
 *****************************************************************************/
static ViStatus pires_IviClose (ViSession vi)
{
    ViStatus error = VI_SUCCESS;
    ViSession io = VI_NULL;
	ViAddr		pconfig = VI_NULL;

        /* Do not lock here.  The caller manages the lock. */

    checkErr( Ivi_GetAttributeViSession (vi, VI_NULL, IVI_ATTR_IO_SESSION, 0, &io));
	
	checkErr( Ivi_GetAttributeViAddr (vi, VI_NULL, PIRES_ATTR_INSTR_RESISTOR_CONFIGURATION, 0, &pconfig));
	checkErr( pires_DestroyConfiguration( vi, pconfig));

Error:
    Ivi_SetAttributeViSession (vi, VI_NULL, IVI_ATTR_IO_SESSION, 0, VI_NULL);
    if(io)                                                      
	{
        PIL_CloseSpecifiedCard( io);
	}
    return error;   
}

/*****************************************************************************
 * Function: pires_CheckStatus                                                 
 * Purpose:  This function checks the status of the instrument to detect 
 *           whether the instrument has encountered an error.  This function  
 *           is called at the end of most exported driver functions.  If the    
 *           instrument reports an error, this function returns      
 *           IVI_ERROR_INSTRUMENT_SPECIFIC.  The user can set the 
 *           PIRES_ATTR_QUERY_INSTRUMENT_STATUS attribute to VI_FALSE to disable this 
 *           check and increase execution speed.                                   
 *
 *           Note:  Call this function only when the session is locked.
 *****************************************************************************/
static ViStatus pires_CheckStatus (ViSession vi)
{
    ViStatus    error = VI_SUCCESS;

    if (Ivi_QueryInstrStatus (vi) && Ivi_NeedToCheckStatus (vi) && !Ivi_Simulating (vi))
	{
		checkErr( pires_CheckStatusCallback (vi, Ivi_IOSession(vi)));
		checkErr( Ivi_SetNeedToCheckStatus (vi, VI_FALSE));
	}
        
Error:
    return error;
}

/*****************************************************************************
 * Function: pires_DefaultInstrSetup                                               
 * Purpose:  This function sends a default setup to the instrument.  The    
 *           pires_reset function calls this function.  The 
 *           pires_IviInit function calls this function when the
 *           user passes VI_FALSE for the reset parameter.  This function is 
 *           useful for configuring settings that other instrument driver 
 *           functions require.    
 *
 *           Note:  Call this function only when the session is locked.
 *****************************************************************************/
static ViStatus pires_DefaultInstrSetup (ViSession vi)
{
    ViStatus    error = VI_SUCCESS;
        
    /* Invalidate all attributes */
    checkErr( Ivi_InvalidateAllAttributes (vi));
    
    if (!Ivi_Simulating(vi))
	{
        ViSession   io = Ivi_IOSession(vi); /* call only when locked */

        checkErr( Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
	}
Error:
    return error;
}


/*****************************************************************************
 *----------------- Attribute Range Tables and Callbacks --------------------*
 *****************************************************************************/

static ViStatus _VI_FUNC pires_CheckStatusCallback (ViSession vi, ViSession io)
{
    ViStatus    error = VI_SUCCESS;
	
	
Error:
    return error;
}
    
/*- PIRES_ATTR_SPECIFIC_DRIVER_REVISION -*/
static ViStatus _VI_FUNC piresAttrDriverRevision_ReadCallback(
	ViSession		vi, 
	ViSession		io,
	ViConstString	channelName, 
	ViAttr			attributeId,
	const ViConstString cacheValue
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      driverRevision[256];
    
    
    sprintf (driverRevision, 
             "Driver: pires %.2f, Compiler: %s %3.2f, "
             "Components: IVIEngine %.2f, PILPXI ver %d",
             PIRES_MAJOR_VERSION + PIRES_MINOR_VERSION/1000.0, 
             IVI_COMPILER_NAME, IVI_COMPILER_VER_NUM, 
             IVI_ENGINE_MAJOR_VERSION + IVI_ENGINE_MINOR_VERSION/1000.0, 
             (int)PIL_Version());

    checkErr( Ivi_SetValInStringCallback (vi, attributeId, driverRevision));    
Error:
    return error;
}


/*- PIRES_ATTR_INSTRUMENT_FIRMWARE_REVISION -*/

static ViStatus _VI_FUNC piresAttrFirmwareRevision_ReadCallback (ViSession vi, 
                                                                    ViSession io,
                                                                    ViConstString channelName, 
                                                                    ViAttr attributeId,
                                                                    const ViConstString cacheValue)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      rdBuffer[BUFFER_SIZE];
	ViChar*		pend = NULL;

	checkPILErr( PIL_CardId(io, rdBuffer));
	

	pend = strchr( rdBuffer, ',');
	if ( pend )
		pend = strchr( ++pend, ',');
	if ( pend )
		memmove(rdBuffer, ++pend, strlen(pend)+1);

    checkErr( Ivi_SetValInStringCallback (vi, attributeId, rdBuffer));
    
Error:
    return error;
}

/*- PIRES_ATTR_INSTRUMENT_MANUFACTURER -*/
static ViStatus _VI_FUNC piresAttrInstrumentManufacturer_ReadCallback(
	ViSession		vi, 
	ViSession		io,
	ViConstString	channelName, 
	ViAttr			attributeId,
	const ViConstString cacheValue
)
{
    ViStatus    error = VI_SUCCESS;
  
    checkErr( Ivi_SetValInStringCallback (vi, attributeId, "Pickering"));
    
Error:
    return error;
}

/*- PIRES_ATTR_INSTRUMENT_MODEL -*/
static ViStatus _VI_FUNC piresAttrInstrumentModel_ReadCallback(
	ViSession		vi, 
	ViSession		io,
	ViConstString	channelName, 
	ViAttr			attributeId,
	const ViConstString cacheValue
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      rdBuffer[BUFFER_SIZE];
	ViChar*		pend = NULL;

	checkPILErr( PIL_CardId(io, rdBuffer));

	pend = strchr( rdBuffer, ',');
	if ( pend ) *pend = '\0';

    checkErr( Ivi_SetValInStringCallback (vi, attributeId, rdBuffer));
    
Error:
    return error;
}
    

/* PIRES_ATTR_RESISTANCE */
static ViStatus _VI_FUNC piresAttrResistance_CheckCallback(
	ViSession		vi,
	ViConstString	channelName,
	ViAttr 			attributeId,
	ViReal64 		value
)
{
	ViStatus  	    error = VI_SUCCESS;
	ViInt32			i, index = atoi(channelName); 
	SChannelConfigurationPtr	pdefinition = VI_NULL;
	SResCardPtr		pcard = NULL;
	ViReal64		offset, max = 0.0;

	checkErr( Ivi_GetAttributeViAddr( vi,
				 					  VI_NULL,
									  PIRES_ATTR_INSTR_RESISTOR_CONFIGURATION,
									  0, (ViAddr*)&pcard));
	checkErr( Ivi_GetAttributeViReal64( vi,
										channelName,
										PIRES_ATTR_ZERO_OFFSET,
										0, &offset));
	
	pdefinition = pcard->ChannelDefinition + index;
	
	for( i = pdefinition->ReleyCount; --i >=0; )
	{
			max += ( pdefinition->ResDefinition[i].HasCalibration ) ?
				pdefinition->ResDefinition[i].CalibratedValue :
				pdefinition->ResDefinition[i].DefinedValue;
	}
	
	max += offset;
	
	if ( value < 0.0 || value > max )
	{
		viCheckErr( IVI_ERROR_INVALID_VALUE);
	}
	
Error:
	return error;
} /* piresAttrResistance_CheckCallback */

static ViStatus _VI_FUNC piresAttrResistance_ReadCallback(
	ViSession vi,
	ViSession io,
	ViConstString channelName,
	ViAttr attributeId,
	ViReal64* value
)
{
	ViStatus  	    error = VI_SUCCESS;
	ViInt32			pattern = 0;
	ViInt32			i, index = atoi(channelName);
	SChannelConfigurationPtr	pdefinition = VI_NULL;
	SResCardPtr		pcard = NULL;
	ViReal64		offset;

	checkErr( Ivi_GetAttributeViInt32( vi,
									   channelName,
									   PIRES_ATTR_INSTR_PATTERN,
									   0, &pattern));
	checkErr( Ivi_GetAttributeViAddr( vi,
				 					  VI_NULL,
									  PIRES_ATTR_INSTR_RESISTOR_CONFIGURATION,
									  0, (ViAddr*)&pcard));
	checkErr( Ivi_GetAttributeViReal64( vi,
										channelName,
										PIRES_ATTR_ZERO_OFFSET,
										0, &offset));

	*value = offset;
	pdefinition = pcard->ChannelDefinition + index;
	pattern = ~pattern;
	for( i = pdefinition->ReleyCount; --i >=0; )
	{
		if (pattern & (1 << i))
		{
			*value += ( pdefinition->ResDefinition[i].HasCalibration ) ?
				pdefinition->ResDefinition[i].CalibratedValue :
				pdefinition->ResDefinition[i].DefinedValue;
		}
	}

Error:
	return error;
}

static ViStatus _VI_FUNC piresAttrResistance_WriteCallback(
	ViSession vi,
	ViSession io,
	ViConstString channelName,
	ViAttr attributeId,
	ViReal64 value
)
{
	ViStatus    error = VI_SUCCESS;
	ViReal64	offset = 0.0;
	ViReal64	value2, resistor, real_value; 
	ViInt32		i, mask = 0;
	SChannelConfigurationPtr	pdefinition;
	ViInt32			index = atoi(channelName);
	SResCardPtr		pcard = NULL;
	ViInt32			greater_pattern = 0;
	ViReal64		greater_value = 0.0;

	checkErr( Ivi_GetAttributeViAddr( vi,
				 					  VI_NULL,
									  PIRES_ATTR_INSTR_RESISTOR_CONFIGURATION,
									  0, (ViAddr*)&pcard));
	checkErr( Ivi_GetAttributeViReal64( vi,
										channelName,
										PIRES_ATTR_ZERO_OFFSET,
										0, &offset));
										
	pdefinition = pcard->ChannelDefinition + index;

	value2      = value - offset;
	real_value = offset;
	for( i = pdefinition->ReleyCount; --i >=0; )
	{
		resistor = ( pdefinition->ResDefinition[i].HasCalibration ) ?
				pdefinition->ResDefinition[i].CalibratedValue :
				pdefinition->ResDefinition[i].DefinedValue;
		if ( value2 >= resistor )
		{
			value2      -= resistor;
			real_value += resistor;
			mask |= ( 1 << i );
		}
		else if ( !greater_pattern && (fabs(value2 - resistor) < (resistor * 0.1)) )
		{
			greater_pattern = mask | ( 1 << i ); 
			greater_value   = real_value + resistor;
		}
	}
	
	if ( fabs(greater_value - value) < fabs(real_value - value) )
	{
		mask = greater_pattern;
	}
	
	checkErr( Ivi_SetAttributeViInt32( vi,
									   channelName,
									   PIRES_ATTR_INSTR_PATTERN,
									   0, ~mask));

Error:
    return error;
}

/*- PIRES_ATTR_ZERO_OFFSET -*/
static IviRangeTableEntry zeroOffsetRangeTableEntries[] =
{
    {0.0, 10.0e6, 0, "", 0},
    {IVI_RANGE_TABLE_LAST_ENTRY}
};
static IviRangeTable zeroOffsetRangeTable =
{
    IVI_VAL_RANGED,
    VI_TRUE,
    VI_TRUE,
    VI_NULL,
    zeroOffsetRangeTableEntries
};

static ViStatus _VI_FUNC piresAttrZeroOffset_ReadCallback(
	ViSession vi,
	ViSession io,
	ViConstString channelName,
	ViAttr attributeId,
	ViReal64* value
)
{
	ViStatus    	error = VI_SUCCESS;	
	ViInt32			index = atoi(channelName);
	SResCardPtr		pcard = NULL;

	checkErr( Ivi_GetAttributeViAddr( vi,
				 					  VI_NULL,
									  PIRES_ATTR_INSTR_RESISTOR_CONFIGURATION,
									  0, (ViAddr*)&pcard));
	
	*value = (pcard->ChannelDefinition + index)->ZeroOffset;

Error:
	return error;
}

static ViStatus _VI_FUNC piresAttrZeroOffset_WriteCallback(
	ViSession vi,
	ViSession io,
	ViConstString channelName,
	ViAttr attributeId,
	ViReal64 value
)
{
	ViStatus    	error = VI_SUCCESS;	
	ViInt32			index = atoi(channelName);
	SResCardPtr		pcard = NULL;

	checkErr( Ivi_GetAttributeViAddr( vi,
				 					  VI_NULL,
									  PIRES_ATTR_INSTR_RESISTOR_CONFIGURATION,
									  0, (ViAddr*)&pcard));
	
	(pcard->ChannelDefinition + index)->ZeroOffset = value;

Error:
	return error;
}

static ViStatus _VI_FUNC piresAttrInstrPattern_WriteCallback(
	ViSession		vi,
	ViSession		io,
	ViConstString	channelName,
	ViAttr			attributeId,
	ViInt32			value
)
{
	ViStatus	error = 0;
	DWORD		pattern[32];
	
	/* Just for sure, unsafe PILPXI function */
	memset( pattern, 0 , sizeof(pattern));
	pattern[0] = value;
	
	checkPILErr( PIL_WriteSub( io, atoi(channelName), pattern));	

	
Error:
	return error;
}

static ViStatus _VI_FUNC piresAttrInstrPattern_ReadCallback(
	ViSession		vi,
	ViSession		io,
	ViConstString	channelName,
	ViAttr			attributeId,
	ViInt32*		value
)
{
	ViStatus	error = 0;
	DWORD		pattern[32];
	
	/* Just for sure, unsafe PILPXI function */
	memset( pattern, 0 , sizeof(pattern));
	pattern[0] = (DWORD)value;
	
	checkPILErr( PIL_ReadSub( io, atoi(channelName), pattern));	
	

	*value = pattern[0];
	
Error:
	return error;	
}

/*****************************************************************************
 * Function: pires_InitAttributes                                                  
 * Purpose:  This function adds attributes to the IVI session, initializes   
 *           instrument attributes, and sets attribute invalidation          
 *           dependencies.                                                   
 *****************************************************************************/
static ViStatus pires_InitAttributes(
	ViSession 		vi,
	ViAddr			pConfig
)
{
    ViStatus    	error = VI_SUCCESS;
    ViInt32     	flags = 0;

    checkErr( Ivi_SetAttributeViInt32 (vi, "", PIRES_ATTR_SPECIFIC_DRIVER_MAJOR_VERSION,
                                       0, PIRES_MAJOR_VERSION));
    checkErr( Ivi_SetAttributeViInt32 (vi, "", PIRES_ATTR_SPECIFIC_DRIVER_MINOR_VERSION,
                                       0, PIRES_MINOR_VERSION));
	checkErr( Ivi_SetAttributeViString (vi, "", PIRES_ATTR_IO_SESSION_TYPE,
                                        0, PIRES_IO_SESSION_TYPE));

    checkErr( Ivi_SetAttrReadCallbackViString (vi, PIRES_ATTR_SPECIFIC_DRIVER_REVISION,
                                               piresAttrDriverRevision_ReadCallback));
    checkErr( Ivi_SetAttributeViInt32 (vi, "", PIRES_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION,
                                       0, PIRES_CLASS_SPEC_MAJOR_VERSION));
    checkErr( Ivi_SetAttributeViInt32 (vi, "", PIRES_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION,
                                       0, PIRES_CLASS_SPEC_MINOR_VERSION));
    checkErr( Ivi_SetAttributeViString (vi, "", PIRES_ATTR_SUPPORTED_INSTRUMENT_MODELS,
                                        0, PIRES_SUPPORTED_INSTRUMENT_MODELS));


    checkErr( Ivi_GetAttributeFlags (vi, PIRES_ATTR_INSTRUMENT_FIRMWARE_REVISION, &flags));
    checkErr( Ivi_SetAttributeFlags (vi, PIRES_ATTR_INSTRUMENT_FIRMWARE_REVISION, 
                                     flags | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION));
    checkErr( Ivi_SetAttrReadCallbackViString (vi, PIRES_ATTR_INSTRUMENT_FIRMWARE_REVISION,
                                               piresAttrFirmwareRevision_ReadCallback));
	checkErr( Ivi_SetAttrReadCallbackViString (vi, PIRES_ATTR_INSTRUMENT_MANUFACTURER,
	                                           piresAttrInstrumentManufacturer_ReadCallback));
	checkErr( Ivi_SetAttrReadCallbackViString (vi, PIRES_ATTR_INSTRUMENT_MODEL,
	                                           piresAttrInstrumentModel_ReadCallback));

    checkErr( Ivi_SetAttributeViString (vi, "", PIRES_ATTR_SPECIFIC_DRIVER_VENDOR,
                                        0, PIRES_DRIVER_VENDOR));
    checkErr( Ivi_SetAttributeViString (vi, "", PIRES_ATTR_SPECIFIC_DRIVER_DESCRIPTION,
                                        0, PIRES_DRIVER_DESCRIPTION));
    checkErr( Ivi_SetAttributeViAddr (vi, VI_NULL, IVI_ATTR_OPC_CALLBACK, 0,
                                      VI_NULL));
    checkErr( Ivi_SetAttributeViAddr (vi, VI_NULL, IVI_ATTR_CHECK_STATUS_CALLBACK, 0,
                                      pires_CheckStatusCallback));
	checkErr( Ivi_SetAttributeViString (vi, "", PIRES_ATTR_GROUP_CAPABILITIES, 0,
	                                    "None"));
    
        /*- Add instrument-specific attributes ------------------------------*/            
	
	
    checkErr( Ivi_AddAttributeViInt32 (vi, PIRES_ATTR_OPC_TIMEOUT,
                                       "PIRES_ATTR_OPC_TIMEOUT",
                                       5000, IVI_VAL_HIDDEN | IVI_VAL_DONT_CHECK_STATUS,
                                       VI_NULL, VI_NULL, VI_NULL)); 
    checkErr( Ivi_AddAttributeViInt32 (vi,
                                       PIRES_ATTR_VISA_RM_SESSION,
                                       "PIRES_ATTR_VISA_RM_SESSION",
                                       VI_NULL,
                                       IVI_VAL_HIDDEN | IVI_VAL_DONT_CHECK_STATUS,
                                       VI_NULL, VI_NULL, VI_NULL));
    checkErr( Ivi_AddAttributeViInt32 (vi, PIRES_ATTR_OPC_CALLBACK,
                                       "PIRES_ATTR_OPC_CALLBACK",
                                       VI_NULL,
                                       IVI_VAL_HIDDEN | IVI_VAL_DONT_CHECK_STATUS,
                                       VI_NULL, VI_NULL, VI_NULL));
    checkErr( Ivi_AddAttributeViInt32 (vi,
                                       PIRES_ATTR_CHECK_STATUS_CALLBACK,
                                       "PIRES_ATTR_CHECK_STATUS_CALLBACK",
                                       VI_NULL,
                                       IVI_VAL_HIDDEN | IVI_VAL_DONT_CHECK_STATUS,
                                       VI_NULL, VI_NULL, VI_NULL));
    checkErr( Ivi_AddAttributeViInt32 (vi,
                                       PIRES_ATTR_USER_INTERCHANGE_CHECK_CALLBACK,
                                       "PIRES_ATTR_USER_INTERCHANGE_CHECK_CALLBACK",
                                       VI_NULL,
                                       IVI_VAL_HIDDEN | IVI_VAL_DONT_CHECK_STATUS,
                                       VI_NULL, VI_NULL, VI_NULL));
	/* PIRES_ATTR_ZERO_OFFSET */
	checkErr( Ivi_AddAttributeViReal64( vi,
										PIRES_ATTR_ZERO_OFFSET,
										"PIRES_ATTR_ZERO_OFFSET",
										0.0,
										IVI_VAL_MULTI_CHANNEL | IVI_VAL_NEVER_CACHE | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION | IVI_VAL_DONT_CHECK_STATUS,
										piresAttrZeroOffset_ReadCallback,
										piresAttrZeroOffset_WriteCallback,
										&zeroOffsetRangeTable,
										0));
	/* PIRES_ATTR_RESISTANCE */
	checkErr( Ivi_AddAttributeViReal64( vi,
										PIRES_ATTR_RESISTANCE,
										"PIRES_ATTR_RESISTANCE",
										32000,
										IVI_VAL_MULTI_CHANNEL | IVI_VAL_NEVER_CACHE | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION | IVI_VAL_DONT_CHECK_STATUS,
										piresAttrResistance_ReadCallback,
										piresAttrResistance_WriteCallback,
										VI_NULL,
										0));
	checkErr( Ivi_SetAttrCheckCallbackViReal64( vi,
											   PIRES_ATTR_RESISTANCE,
	                                           piresAttrResistance_CheckCallback));
	/* PIRES_ATTR_INSTR_PATTERN */
	checkErr( Ivi_AddAttributeViInt32 (vi,
                                       PIRES_ATTR_INSTR_PATTERN,
                                       "PIRES_ATTR_INSTR_PATTERN",
                                       0,
                                       IVI_VAL_HIDDEN | IVI_VAL_MULTI_CHANNEL,
                                       piresAttrInstrPattern_ReadCallback,
									   piresAttrInstrPattern_WriteCallback,
									   VI_NULL));

	checkErr( Ivi_AddAttributeViAddr (vi, PIRES_ATTR_INSTR_RESISTOR_CONFIGURATION,
		                               "PIRES_ATTR_INSTR_RESISTOR_CONFIGURATION", pConfig,
									   IVI_VAL_HIDDEN | IVI_VAL_DONT_CHECK_STATUS | IVI_VAL_ALWAYS_CACHE,
									   VI_NULL, VI_NULL));
                                       
        /*- Setup attribute invalidations -----------------------------------*/

Error:
    return error;
}


static ViStatus pires_CreateDefaultResistorDefinition(
	ViSession	vi,
	ViInt32 	type,
	ViAddr*		pconfig,
	ViChar		channelList[256]
)
{
	ViStatus		error = 0;
	SResCardPtr 	pcard = NULL;
	SResConfigPtr*	pcard_configuration = NULL;
	ViInt32			channel_count = 0;
	ViInt32			reley_count   = 0;
	ViChar*			pchannel = channelList; 
	int				i;
	
	switch (type)
	{
		case PI_40_295_121_10_16:
			channel_count = 10;
			reley_count   = 16;
			break;
		case PI_40_295_021_10_8:
			channel_count = 10;
			reley_count   = 8;
			break;
		case PI_40_295_121_18_8:
			channel_count = 18;
			reley_count   = 8;
			break;
		case PI_40_295_021_05_12:
			channel_count = 5;
			reley_count   = 12;
			break;
		case PI_40_295_121_10_12:
			channel_count = 10;
			reley_count   = 12;
			break;
		case PI_40_295_021_05_16:
			channel_count = 5;
			reley_count   = 16;
			break;
		case PI_40_295_021_03_24:
			channel_count = 3;
			reley_count   = 24;
			break;
		case PI_40_295_121_06_24:
			channel_count = 6;
			reley_count   = 24;
			break;
		default:
			viCheckErrElab( IVI_ERROR_INVALID_VALUE,
						"Not supported resisto card." );
			break;
	}
	
#if 0
	checkErr( Ivi_Alloc( vi,
					     sizeof(SResCard),
						 (ViAddr*)&pcard));
	pcard->ChannelCount =  channel_count;
	checkErr( Ivi_Alloc( vi,
					     sizeof(SChannelConfigurationPtr)*channel_count,
						 (ViAddr*)&(pcard->ChannelDefinition)));	
	memset(pcard->ChannelDefinition, 0, sizeof(SChannelConfiguration)*channel_count);

	for ( i = 0; i < channel_count; i++)
	{
		pchannel += sprintf( pchannel, "%d,", i);
		pcard->ChannelDefinition[i].ReleyCount = reley_count;
		pcard->ChannelDefinition[i].ZeroOffset = 0.0;
		checkErr( Ivi_Alloc( vi,
						     sizeof(SResConfig) * reley_count,
							 (ViAddr*)(&(pcard->ChannelDefinition[i].ResDefinition))));
		memcpy( pcard->ChannelDefinition[i].ResDefinition,
				gs_DefaultResistors,
				sizeof(SResConfig) * reley_count);
	}
	*--pchannel = '\0';
#else
	pcard = (SResCardPtr)calloc( 1, sizeof(SResCard));
	pcard->ChannelCount      = channel_count;
	pcard->ChannelDefinition = calloc(channel_count, sizeof(SChannelConfiguration)); 

	for ( i = 0; i < channel_count; i++)
	{
		pchannel += sprintf( pchannel, "%d,", i);
		pcard->ChannelDefinition[i].ReleyCount    = reley_count;
		pcard->ChannelDefinition[i].ZeroOffset    = 0.0;
		pcard->ChannelDefinition[i].ResDefinition = calloc( reley_count, sizeof(SResConfig)); 
		memcpy( pcard->ChannelDefinition[i].ResDefinition,
				gs_DefaultResistors,
				sizeof(SResConfig) * reley_count);
	}
	*--pchannel = '\0';	
#endif
	
Error:
	if ( error < 0 )
	{
		pires_DestroyConfiguration(vi, pcard);
		pcard = NULL;
	}
	*pconfig = (ViAddr)pcard;
	return error;
}

static ViStatus pires_DestroyConfiguration(
	ViSession	vi,
	ViAddr		pconfig
)
{
	ViStatus	error = 0;
	SResCardPtr	pcard = (SResCardPtr)pconfig;
#if 0
	if ( pcard )
	{
		ViInt32		channel_count = pcard->ChannelCount;
		
		for( ; --channel_count >= 0; )
		{
			if ( pcard->ChannelDefinition[channel_count].ResDefinition )
			{
				checkErr( Ivi_Free( vi, (ViAddr)(pcard->ChannelDefinition[channel_count].ResDefinition)));
				pcard->ChannelDefinition[channel_count].ResDefinition = NULL;
			}
		}
		checkErr( Ivi_Free( vi, (ViAddr)pcard->ChannelDefinition));				
		pcard->ChannelDefinition = NULL;
		checkErr( Ivi_Free( vi, (ViAddr)pcard));
	}
#else
	if ( pcard )
	{
		ViInt32		channel_count = pcard->ChannelCount;
		
		for( ; --channel_count >= 0; )
		{
			if ( pcard->ChannelDefinition[channel_count].ResDefinition )
			{
				free( pcard->ChannelDefinition[channel_count].ResDefinition);
				pcard->ChannelDefinition[channel_count].ResDefinition = NULL;
			}
		}
		free( pcard->ChannelDefinition);
		pcard->ChannelDefinition = NULL;
		free( pcard);
	}
#endif
Error:	
	return error;
}

static ViStatus pires_DetectDefaultCard(
	ViSession	vi,
	ViInt32*	pType
)
{
	ViStatus	error = 0;
	ViInt32		t = UNKNOWN_RESISTOR_CARD;
	if (!Ivi_Simulating(vi))
	{
		DWORD		i,subs, reley, temp, stype, lrelay = 0;
		ViSession	io = Ivi_IOSession(vi);

		checkPILErr( PIL_EnumerateSubs( io, &temp, &subs));
		for( i = 0; ++i <= subs; ) 
		{
			checkPILErr( PIL_SubInfo(io, i, TRUE, &stype, &temp, &reley));
			if ( !lrelay) lrelay = reley;
			else
			if ( lrelay != reley && stype != TYPE_RES)
			{
				reley = -1;
				break;
			}
			
		}
		if ( reley > 0 )
		{
			for( i = 0; gs_DefaultCards[i].name; i++)
			{
				if ( gs_DefaultCards[i].releys == reley &&
					 gs_DefaultCards[i].channels == subs )
				{
					t = gs_DefaultCards[i].cardType;
					break;	
				}
				
			}
		}
	}	

Error:
	*pType = t;
	return error;
}

/*****************************************************************************
 *------------------- End Instrument Driver Source Code ---------------------*
 *****************************************************************************/
