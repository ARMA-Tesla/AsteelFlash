
#include <ansi_c.h>
#include <NIDAQmx.h>
/*****************************************************************************
 *  ELCOM-VPC Power Switch Instrument Driver                               
 *  LabWindows/CVI Instrument Driver                                     
 *  Original Release: 27. èervence 2007                                  
 *  By: Zdenek Rykala, ELCOM, a.s.                              
 *      PH. +420 775 265 821   Fax +420 775 265 821                              
 *                                                                           
 *  Modification History:                                                    
 *                                                                           
 *       27. èervence 2007 - Instrument Driver Created.                  
 *                                                                           
 *****************************************************************************/

#include <string.h>
#include <stdio.h>  
#include <formatio.h>
#include <utility.h>
#include <ctype.h>
#include "elvpc3a.h"


#define checkDAQmx( fCal) checkErr( fCal)

/*****************************************************************************
 *--------------------- Hidden Attribute Declarations -----------------------*
 *****************************************************************************/

#define ELVPC3A_ATTR_OPC_TIMEOUT      (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 1L)       /* ViInt32 */
#define ELVPC3A_ATTR_USE_SPECIFIC_SIMULATION  (IVI_INHERENT_ATTR_BASE + 23L)  /* ViBoolean */
#define ELVPC3A_ATTR_VISA_RM_SESSION  (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 5L)   /* ViSession */
#define ELVPC3A_ATTR_OPC_CALLBACK     (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 6L)   /* ViAddr */
#define ELVPC3A_ATTR_CHECK_STATUS_CALLBACK    (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 7L)   /* ViAddr */
#define ELVPC3A_ATTR_USER_INTERCHANGE_CHECK_CALLBACK  (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 8L)   /* ViAddr */
#define ELVPC3A_ATTR_CHANNEL_MATRIX                   (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 101L)  /* ViAddr */
#define ELVPC3A_ATTR_ORIGINAL_SOURCE_CHANNEL_INDEX    (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 102L)  /* ViInt32, channel-based */
#define ELVPC3A_ATTR_CONFIGURATION_CHANNEL_INDEX      (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 103L)  /* ViInt32, channel-based */
#define ELVPC3A_ATTR_IS_MUX_CHANNEL                   (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 104L)  /* ViBoolean, channel-based */
#define ELVPC3A_ATTR_MAXIMUM_DEPTH                    (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 105L)  /* ViInt32 */
#define ELVPC3A_ATTR_SETTLING_MOMENT                  (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 106L)  /* ViReal64 */ 

#define ELVPC3A_ATTR_RELAYS                           (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 110L)  /* ViInt32 */ 
#define ELVPC3A_ATTR_RELAY_COUNTER                    (IVI_SPECIFIC_PRIVATE_ATTR_BASE + 111L)  /* ViAddr */

/*****************************************************************************
 *---------------------------- Useful Macros --------------------------------*
 *****************************************************************************/
typedef	struct _SRelTable
	{
		const char*	channel1;
		const char*	channel2;
		const char*	port0;
		const char*	port1;
	} SRelTable, *SRelTablePtr;


static SRelTable gs_RelTable[] = {    
	{"CH1A",	"CH1B", 	"port1/line7", "port0/line23"},
	{"C1A",		"C1B",		"port1/line6", "port0/line22"},
	
	{"CH2A",	"CH2B", 	"port2/line6", "port0/line30"},
	{"C2A",		"C2B",		"port1/line2", "port0/line18"},
	
	{"CH3A",	"CH3B", 	"port2/line7", "port0/line31"},
	{"C3A",		"C3B",		"port2/line3", "port0/line27"},
	
	{"CH4A",	"CH4B",		"port2/line2", "port0/line26"},
	{"C4A",		"C4B", 		"port1/line5", "port0/line21"},
	
	{"REV3A",	"REV3B",	"port1/line0", "port0/line16"},
	
	{"REV4A",	"REV4B",	"port1/line1", "port0/line17"},
	{NULL,		NULL,		NULL,			NULL}
};




/*- I/O buffer size -----------------------------------------------------*/
#define BUFFER_SIZE                             512L        

#define ELVPC3A_IO_SESSION_TYPE   IVI_VAL_VISA_SESSION_TYPE



    /*- List of channels passed to the Ivi_BuildChannelTable function -------*/ 
#define CHANNEL_LIST                            "CH1A,CH1B,C1A,C1B,"\
												"CH2A,CH2B,C2A,C2B,"\
												"CH3A,CH3B,C3A,C3B,"\
												"CH4A,CH4B,C4A,C4B,"\
												"REV3A,REV3B,REV4A,REV4B"

    /* Defined values for the flags in the channel matrix */
#define ELVPC3A_VAL_CHANNELS_DIRECTLY_CONNECTED   (1<<0)      /*  The two channels are connected without any      */
                                                               /*  interceding configuration channels.             */
                                                               /*  Either of the two channels may be a             */
                                                               /*  configuration channel.                          */
                                                                            
#define ELVPC3A_VAL_CHANNELS_EXPLICITLY_CONNECTED (1<<1)      /*  The user has connected the two channels         */
                                                               /*  explicitly by calling either elvpc3a_Connect   */
                                                               /*  or elvpc3a_SetPath.  Neither channel can be    */
                                                               /*  a configuration channel.                        */

#define ELVPC3A_VAL_DIRECT_CONNECTION_IMPOSSIBLE  (1<<2)      /*  These channels can never be directly conected   */

#define ELVPC3A_VAL_RESERVED_FOR_RECURSION        (1<<3)      /*  This flag is always set on the first column. It */
                                                               /*  is temporarily set for a channel before        */
                                                               /*  entering the recursive call. This prevents the */
                                                               /*  channel from being used in the recursive call. */
                                                                            

    /* Defined macros for accessing flags in the channel matrix */

#define elvpc3a_ChannelsAreDirectlyConnected(ch1, ch2)                               \
                        ((connections[ch1][ch2] & ELVPC3A_VAL_CHANNELS_DIRECTLY_CONNECTED) != 0)
#define elvpc3a_ChannelsAreExplicitlyConnected(ch1, ch2)                               \
                        ((connections[ch1][ch2] & ELVPC3A_VAL_CHANNELS_EXPLICITLY_CONNECTED) != 0)
#define elvpc3a_DirectConnectionIsPossible(ch1, ch2)                                     \
                        ((connections[ch1][ch2] & ELVPC3A_VAL_DIRECT_CONNECTION_IMPOSSIBLE) == 0)
#define elvpc3a_RecursionOnChannelIsAllowed(ch)                          \
                        ((connections[ch][1] & ELVPC3A_VAL_RESERVED_FOR_RECURSION) == 0)

#define elvpc3a_MarkChannelsAsDirectlyConnected(ch1, ch2)                            \
                        {                                                            \
                        connections[ch1][ch2] |= ELVPC3A_VAL_CHANNELS_DIRECTLY_CONNECTED;\
                        connections[ch2][ch1] |= ELVPC3A_VAL_CHANNELS_DIRECTLY_CONNECTED;\
                        }
#define elvpc3a_MarkChannelsAsExplicitlyConnected(ch1, ch2)                            \
                        {                                                            \
                        connections[ch1][ch2] |= ELVPC3A_VAL_CHANNELS_EXPLICITLY_CONNECTED;\
                        connections[ch2][ch1] |= ELVPC3A_VAL_CHANNELS_EXPLICITLY_CONNECTED;\
                        }
#define elvpc3a_PreventRecursionOnChannel(ch)                          \
                        {                                                               \
                        connections[ch][1] |= ELVPC3A_VAL_RESERVED_FOR_RECURSION;\
                        }

#define elvpc3a_MarkChannelsAsNotDirectlyConnected(ch1, ch2)                                \
                        {                                                   \
                        connections[ch1][ch2] &= ~ELVPC3A_VAL_CHANNELS_DIRECTLY_CONNECTED;\
                        connections[ch2][ch1] &= ~ELVPC3A_VAL_CHANNELS_DIRECTLY_CONNECTED;\
                        }
#define elvpc3a_MarkChannelsAsNotExplicitlyConnected(ch1, ch2)                                \
                        {                                                   \
                        connections[ch1][ch2] &= ~ELVPC3A_VAL_CHANNELS_EXPLICITLY_CONNECTED;\
                        connections[ch2][ch1] &= ~ELVPC3A_VAL_CHANNELS_EXPLICITLY_CONNECTED;\
                        }
#define elvpc3a_AllowRecursionOnChannel(ch)                          \
                        {                                                               \
                        connections[ch][1] &= ~ELVPC3A_VAL_RESERVED_FOR_RECURSION;\
                        }


/*****************************************************************************
 *-------------- Utility Function Declarations (Non-Exported) ---------------*
 *****************************************************************************/
static ViStatus elvpc3a_IviInit (ViRsrc resourceName, ViBoolean IDQuery, 
                                     ViBoolean reset, ViSession vi);
static ViStatus elvpc3a_IviClose (ViSession vi);   

static ViStatus elvpc3a_InitAttributes (ViSession vi);
static ViStatus elvpc3a_DefaultInstrSetup (ViSession openInstrSession);
static ViStatus elvpc3a_CheckStatus (ViSession vi);
static ViStatus elvpc3a_SourceChannelConflict (ViSession vi, ViConstString channel1, 
                                                ViConstString channel2,
                                                ViBoolean *hasConflict);
static ViStatus elvpc3a_VerifySourceChannelConflict (ViSession vi, 
                                                      ViConstString channel1, 
                                                      ViConstString channel2);
static ViStatus elvpc3a_IsEitherAConfigChannel (ViSession vi, 
                                                 ViConstString channel1, 
                                                 ViConstString channel2,
                                                 ViBoolean *atLeastOneIsAConfigChannel);
static ViStatus elvpc3a_VerifyIsEitherAConfigChannel (ViSession vi, 
                                                       ViConstString channel1, 
                                                       ViConstString channel2);
static ViStatus elvpc3a_IsUnconnectedConfigChannel (ViSession vi, 
                                                     ViConstString channel,
                                                     ViBoolean *isUnconnectedConfigChannel);
static ViStatus elvpc3a_GetConfigChannelInfo (ViSession vi, 
                                               ViConstString channel,
                                               ViBoolean *isConfigChannel,
                                               ViInt32 *ndxConnectedToChannel);
static ViStatus elvpc3a_GetPossiblePath (ViSession vi, 
                                          ViConstString channel1,
                                          ViConstString channel2, 
                                          ViInt32 maxDepth,
                                          ViString *path);
static ViStatus elvpc3a_GetPossiblePathRecursively (ViSession vi, 
                                                     ViInt32 ndxChannel1, 
                                                     ViInt32 ndxChannel2, 
                                                     ViInt32 **connections, 
                                                     ViInt32 numChannels, 
                                                     ViString *path, 
                                                     ViInt32 oldSize,
                                                     ViInt32 maxRemainingSteps,
                                                     ViInt32 *numSteps);
static ViStatus elvpc3a_GetExistingPath (ViSession vi, 
                                          ViConstString channel1,
                                          ViConstString channel2, 
                                          ViString *path);
static ViStatus elvpc3a_GetExistingPathRecursively (ViSession vi, 
                                                     ViInt32 ndxChannel1, 
                                                     ViInt32 ndxChannel2, 
                                                     ViInt32 **connections, 
                                                     ViInt32 numChannels, 
                                                     ViString *path, 
                                                     ViInt32 oldSize);
static ViStatus elvpc3a_GetFirstAndLastChannelInPath (ViSession vi, 
                                                       ViConstString path,
                                                       ViChar firstChannel[],
                                                       ViChar lastChannel[]);
static ViString elvpc3a_FormatPathLegErrorElab (ViConstString channel1,
                                                 ViConstString channel2,
                                                 ViChar elabBuf[]);
static ViStatus elvpc3a_VerifyNotConnectedMux (ViSession vi, 
                                                ViConstString channel);
static ViStatus elvpc3a_VerifyPath (ViSession vi, ViConstString path);
static ViStatus elvpc3a_VerifyPathRecursively (ViSession vi, 
                                                ViConstString path,
                                                ViInt32 offset,
                                                ViInt32 ndxPrevLast,
                                                ViInt32 **connections);
static ViStatus elvpc3a_ConnectPathOnInstrument (ViSession vi, ViSession io,
                                                  ViConstString path);
static ViStatus elvpc3a_DisconnectPathOnInstrument (ViSession vi, 
                                                     ViSession io,
                                                     ViConstString path);
static ViStatus elvpc3a_IsConnectedMuxChannel (ViSession vi, 
                                                ViConstString channel,
                                                ViBoolean *muxChannelConnected);
static ViStatus elvpc3a_MarkPathAsConnected (ViSession vi, ViConstString path);
static ViStatus elvpc3a_MarkPathAsDisconnected (ViSession vi, ViConstString path);
static ViStatus elvpc3a_MarkAllAsDisconnected (ViSession vi);
static ViStatus elvpc3a_MarkOriginalSourceRecursively (ViSession vi, 
                                                        ViInt32 ndxChannel, 
                                                        ViInt32 originalIndex, 
                                                        ViInt32 numChannels, 
                                                        ViInt32 **connections);
static ViStatus elvpc3a_RecalculateSourceInformation (ViSession vi,
                                                       ViInt32 **connections,
                                                       ViInt32 ndxTheSource);
static ViStatus elvpc3a_MarkConnectionAlongPath (ViSession vi, ViConstString channel1, 
                                                  ViConstString channel2);
static ViStatus elvpc3a_DeleteConnectionAlongPath (ViSession vi, ViConstString channel1, 
                                                    ViConstString channel2);
static ViStatus elvpc3a_IsPathSupported (ViSession vi, ViInt32 ndxChannel1, 
                                          ViInt32 ndxChannel2, 
                                          ViInt32 **connections, 
                                          ViInt32 maxDepth,
                                          ViBoolean *result);
static ViStatus elvpc3a_IsPathAvailable (ViSession vi, ViInt32 ndxChannel1, 
                                          ViInt32 ndxChannel2, 
                                          ViInt32 **connections, 
                                          ViInt32 maxDepth,
                                          ViBoolean *result);
static ViStatus elvpc3a_IsPathAvailableRecursively (ViSession vi, 
                                                     ViInt32 ndxChannel1, 
                                                     ViInt32 ndxChannel2, 
                                                     ViInt32 **connections, 
                                                     ViInt32 numChannels,
                                                     ViInt32 maxDepth,
                                                     ViBoolean *result);
static ViStatus elvpc3a_ImplicitConnectionExists (ViSession vi,
                                                   ViConstString channel1,
                                                   ViConstString channel2, 
                                                   ViBoolean *connectionExists);

static void elvpc3a_CalculatePathLegSize (ViConstString channel1, 
                                           ViConstString channel2,
                                           ViInt32 *legSize);
static void elvpc3a_CleanPath (ViChar string[]);

/*****************************************************************************
 *----------------- Callback Declarations (Non-Exported) --------------------*
 *****************************************************************************/


    /*- Attribute callbacks -------------------------------------------------*/

static ViStatus _VI_FUNC elvpc3aAttrDriverRevision_ReadCallback (ViSession vi,
                                                                  ViSession io, 
                                                                  ViConstString channelName,
                                                                  ViAttr attributeId, 
                                                                  const ViConstString cacheValue);
static ViStatus _VI_FUNC elvpc3aAttrInstrumentManufacturer_ReadCallback (ViSession vi, 
                                                                          ViSession io,
                                                                          ViConstString channelName, 
                                                                          ViAttr attributeId,
                                                                          const ViConstString cacheValue);
static ViStatus _VI_FUNC elvpc3aAttrInstrumentModel_ReadCallback (ViSession vi, 
                                                                   ViSession io,
                                                                   ViConstString channelName, 
                                                                   ViAttr attributeId,
                                                                   const ViConstString cacheValue);

static ViStatus _VI_FUNC elvpc3aAttrIsDebounced_ReadCallback (ViSession vi,
                                                               ViSession io,
                                                               ViConstString channelName,
                                                               ViAttr attributeId,
                                                               ViBoolean *value);
static ViStatus _VI_FUNC elvpc3aAttrOriginalSourceChannelIndex_ReadCallback (ViSession vi,
                                                                              ViSession io,
                                                                              ViConstString channelName,
                                                                              ViAttr attributeId,
                                                                              ViInt32 *value);

/*****************************************************************************
 *------------ User-Callable Functions (Exportable Functions) ---------------*
 *****************************************************************************/

/*****************************************************************************
 * Function: elvpc3a_init   
 * Purpose:  VXIplug&play required function.  Calls the   
 *           elvpc3a_InitWithOptions function.   
 *****************************************************************************/
ViStatus _VI_FUNC elvpc3a_init (ViRsrc resourceName, ViBoolean IDQuery,
                                 ViBoolean resetDevice, ViSession *newVi)
{   
    ViStatus    error = VI_SUCCESS;

    if (newVi == VI_NULL)
        {
        Ivi_SetErrorInfo (VI_NULL, VI_FALSE, IVI_ERROR_INVALID_PARAMETER, 
                          VI_ERROR_PARAMETER4, "Null address for Instrument Handle");
        checkErr( IVI_ERROR_INVALID_PARAMETER);
        }

    checkErr( elvpc3a_InitWithOptions (resourceName, IDQuery, resetDevice, 
                                        "", newVi));

Error:  
    return error;
}

/*****************************************************************************
 * Function: elvpc3a_InitWithOptions                                       
 * Purpose:  This function creates a new IVI session and calls the 
 *           IviInit function.                                     
 *****************************************************************************/
ViStatus _VI_FUNC elvpc3a_InitWithOptions(
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
    checkErr( Ivi_SpecificDriverNew ("elvpc3a", newOptionString, &vi));  
    if (!isLogicalName)
    {
        ViInt32 oldFlag = 0;
        
        checkErr (Ivi_GetAttributeFlags (vi, IVI_ATTR_IO_RESOURCE_DESCRIPTOR, &oldFlag));
        checkErr (Ivi_SetAttributeFlags (vi, IVI_ATTR_IO_RESOURCE_DESCRIPTOR, oldFlag & 0xfffb | 0x0010));
        checkErr (Ivi_SetAttributeViString (vi, "", IVI_ATTR_IO_RESOURCE_DESCRIPTOR, 0, newResourceName));
    }
        /* init the driver */
    checkErr( elvpc3a_IviInit (newResourceName, IDQuery, resetDevice, vi)); 
    if (isLogicalName)
        checkErr( Ivi_ApplyDefaultSetup (vi));
    *newVi = vi;
    
Error:
    if (error < VI_SUCCESS) 
        Ivi_Dispose (vi);
        
    return error;
}

/*****************************************************************************
 * Function: elvpc3a_close                                                           
 * Purpose:  This function closes the instrument.                            
 *
 *           Note:  This function must unlock the session before calling
 *           Ivi_Dispose.
 *****************************************************************************/
ViStatus _VI_FUNC elvpc3a_close (ViSession vi)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Ivi_LockSession (vi, VI_NULL));
    
    checkErr( elvpc3a_IviClose (vi));

Error:    
    Ivi_UnlockSession (vi, VI_NULL);
    Ivi_Dispose (vi);  

    return error;
}

/*****************************************************************************
 * Function: elvpc3a_reset                                                         
 * Purpose:  This function resets the instrument.                          
 *****************************************************************************/
ViStatus _VI_FUNC elvpc3a_reset (ViSession vi)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Ivi_LockSession (vi, VI_NULL));

	
	checkErr( elvpc3a_DefaultInstrSetup (vi));
	
	if ( error == VI_SUCCESS )
	    viCheckWarn( VI_WARN_NSUP_RESET);

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: elvpc3a_ResetWithDefaults
 * Purpose:  This function resets the instrument and applies default settings
 *           from the IVI Configuration Store based on the logical name
 *           from which the session was created.
 *****************************************************************************/
ViStatus _VI_FUNC elvpc3a_ResetWithDefaults (ViSession vi)
{
    ViStatus error = VI_SUCCESS;
    
    checkErr( Ivi_LockSession (vi, VI_NULL));
    checkErr( elvpc3a_reset(vi));
    checkErr( Ivi_ApplyDefaultSetup(vi));
    
Error:
    Ivi_UnlockSession(vi, VI_NULL);
    return error;
}

/**************************************************************************** 
 *  Function: elvpc3a_Disable
 *  Purpose:  This function places the instrument in a quiescent state as 
 *            quickly as possible.
 ****************************************************************************/
ViStatus _VI_FUNC elvpc3a_Disable (ViSession vi)
{
    ViStatus error = VI_SUCCESS;
    
    checkErr( Ivi_LockSession (vi, VI_NULL));
    
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: elvpc3a_self_test                                                       
 * Purpose:  This function executes the instrument self-test and returns the 
 *           result.                                                         
 *****************************************************************************/
ViStatus _VI_FUNC elvpc3a_self_test (ViSession vi, ViInt16 *testResult, 
                                      ViChar testMessage[])
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Ivi_LockSession (vi, VI_NULL));

    if (testResult == VI_NULL)
        viCheckParm( IVI_ERROR_INVALID_PARAMETER, 2, "Null address for Test Result");
    if (testMessage == VI_NULL)
        viCheckParm( IVI_ERROR_INVALID_PARAMETER, 3, "Null address for Test Message");

	viCheckWarn( VI_WARN_NSUP_SELF_TEST);

Error:
    Ivi_UnlockSession(vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: elvpc3a_revision_query                                                  
 * Purpose:  This function returns the driver and instrument revisions.      
 *****************************************************************************/
ViStatus _VI_FUNC elvpc3a_revision_query (ViSession vi, ViChar driverRev[], 
                                           ViChar instrRev[])
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Ivi_LockSession (vi, VI_NULL));

    if (driverRev == VI_NULL)
        viCheckParm( IVI_ERROR_INVALID_PARAMETER, 2, "Null address for Driver Revision");
    if (instrRev == VI_NULL)
        viCheckParm( IVI_ERROR_INVALID_PARAMETER, 3, "Null address for Instrument Revision");

    checkErr( Ivi_GetAttributeViString (vi, VI_NULL, ELVPC3A_ATTR_SPECIFIC_DRIVER_REVISION, 
                                        0, 256, driverRev));
    checkErr( Ivi_GetAttributeViString (vi, "", ELVPC3A_ATTR_INSTRUMENT_FIRMWARE_REVISION, 
                                        0, 256, instrRev));
    checkErr( elvpc3a_CheckStatus (vi));
	viCheckWarn( VI_WARN_NSUP_REV_QUERY);

Error:    
    Ivi_UnlockSession(vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: elvpc3a_error_query                                                     
 * Purpose:  This function queries the instrument error queue and returns   
 *           the result.                                                     
 *****************************************************************************/
ViStatus _VI_FUNC elvpc3a_error_query (ViSession vi, ViInt32 *errCode, 
                                        ViChar errMessage[])
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Ivi_LockSession (vi, VI_NULL));
    
    if (errCode == VI_NULL)
        viCheckParm( IVI_ERROR_INVALID_PARAMETER, 2, "Null address for Error Code");
    if (errMessage == VI_NULL)
        viCheckParm( IVI_ERROR_INVALID_PARAMETER, 3, "Null address for Error Message");

	viCheckWarn( VI_WARN_NSUP_ERROR_QUERY);

Error:
    Ivi_UnlockSession(vi, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: elvpc3a_error_message                                                  
 * Purpose:  This function translates the error codes returned by this       
 *           instrument driver into user-readable strings.  
 *
 *           Note:  The caller can pass VI_NULL for the vi parameter.  This 
 *           is useful if one of the init functions fail.
 *****************************************************************************/
ViStatus _VI_FUNC elvpc3a_error_message (ViSession vi, ViStatus errorCode,
                                          ViChar errorMessage[256])
{
    ViStatus    error = VI_SUCCESS;
    
    static      IviStringValueTable errorTable = 
        {
			IVISWTCH_ERROR_CODES_AND_MSGS,
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
 * Function: elvpc3a_InvalidateAllAttributes
 * Purpose:  This function invalidates the cached value of all attributes.
 *****************************************************************************/
ViStatus _VI_FUNC elvpc3a_InvalidateAllAttributes (ViSession vi)
{
    return Ivi_InvalidateAllAttributes(vi);
}

    /*- Obsolete Error Handling Functions ----------------------------*/
ViStatus _VI_FUNC elvpc3a_GetErrorInfo (ViSession vi, ViStatus *primaryError, 
                                         ViStatus *secondaryError, ViChar errorElaboration[256])  
{                                                                                                           
    return Ivi_GetErrorInfo(vi, primaryError, secondaryError, errorElaboration);                                
}                                                                                                           
ViStatus _VI_FUNC elvpc3a_ClearErrorInfo (ViSession vi)                                                        
{                                                                                                           
    return Ivi_ClearErrorInfo (vi);                                                                             
}

/*****************************************************************************
 * Function: elvpc3a_GetError and elvpc3a_ClearError Functions                       
 * Purpose:  These functions enable the instrument driver user to  
 *           get or clear the error information the driver associates with the
 *           IVI session.                                                        
 *****************************************************************************/
ViStatus _VI_FUNC elvpc3a_GetError (ViSession vi, 
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
        checkErr( elvpc3a_error_message (vi, primary, msg));
        appendPoint += sprintf(appendPoint, "Primary Error: (Hex 0x%08X) %s\n", primary, msg);
        }
    
    if (secondary != VI_SUCCESS)
        {
        ViChar msg[256] = "";
        checkErr( elvpc3a_error_message (vi, secondary, msg));
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

ViStatus _VI_FUNC elvpc3a_ClearError (ViSession vi)                                                        
{                                                                                                           
    return Ivi_ClearErrorInfo (vi);                                                                             
}

/*****************************************************************************
 * Function: elvpc3a_GetNextCoercionRecord                        
 * Purpose:  This function enables the instrument driver user to obtain
 *           the coercion information associated with the IVI session.                                                              
 *           This function retrieves and clears the oldest instance in which 
 *           the instrument driver coerced a value the instrument driver user
 *           specified to another value.                     
 *****************************************************************************/
ViStatus _VI_FUNC elvpc3a_GetNextCoercionRecord (ViSession vi,
                                                  ViInt32 bufferSize,
                                                  ViChar  record[])
{
    return Ivi_GetNextCoercionString (vi, bufferSize, record);
}

/**************************************************************************** 
 *  Function: elvpc3a_GetNextInterchangeWarning,
 *            elvpc3a_ResetInterchangeCheck, and
 *            elvpc3a_ClearInterchangeWarnings
 *  Purpose:  These functions allow the user to retrieve interchangeability
 *            warnings, reset the driver's interchangeability checking 
 *            state, and clear all previously logged interchangeability warnings.
 ****************************************************************************/
ViStatus _VI_FUNC elvpc3a_GetNextInterchangeWarning (ViSession vi, 
                                                      ViInt32 bufferSize, 
                                                      ViChar warnString[])
{
    return Ivi_GetNextInterchangeCheckString (vi, bufferSize, warnString);
}

ViStatus _VI_FUNC elvpc3a_ResetInterchangeCheck (ViSession vi)
{
    return Ivi_ResetInterchangeCheck (vi);
}

ViStatus _VI_FUNC elvpc3a_ClearInterchangeWarnings (ViSession vi)
{
    return Ivi_ClearInterchangeWarnings (vi);
}

/*****************************************************************************
 * Function: elvpc3a_LockSession and elvpc3a_UnlockSession Functions                        
 * Purpose:  These functions enable the instrument driver user to lock the 
 *           session around a sequence of driver calls during which other
 *           execution threads must not disturb the instrument state.
 *                                                                          
 *           NOTE:  The callerHasLock parameter must be a local variable 
 *           initialized to VI_FALSE and passed by reference, or you can pass 
 *           VI_NULL.                     
 *****************************************************************************/
ViStatus _VI_FUNC elvpc3a_LockSession (ViSession vi, ViBoolean *callerHasLock)  
{                                              
    return Ivi_LockSession(vi,callerHasLock);      
}                                              
ViStatus _VI_FUNC elvpc3a_UnlockSession (ViSession vi, ViBoolean *callerHasLock) 
{                                              
    return Ivi_UnlockSession(vi,callerHasLock);    
}   

/**************************************************************************** 
 *  Function: elvpc3a_GetChannelName
 *  Purpose:  This function returns the highest-level channel name that
 *            corresponds to the specific driver channel string that is in
 *            the channel table at an index you specify.
 ****************************************************************************/
ViStatus _VI_FUNC elvpc3a_GetChannelName (ViSession vi, ViInt32 index,
                                           ViInt32 bufferSize,
                                           ViChar name[])
{
    ViStatus         error = VI_SUCCESS;
    ViConstString    channelName;
    ViInt32          actualSize = 0;
    ViInt32			 channelCount;

    checkErr( Ivi_LockSession (vi, VI_NULL));
    
    /* Check Parameters */
    if (bufferSize != 0)
        {
        if (name == VI_NULL)
            {
            viCheckParm( IVI_ERROR_INVALID_PARAMETER, 4, "Null address for Channel Name");
            }
        }
        
    checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, IVI_ATTR_CHANNEL_COUNT, 0, &channelCount));
    if ( (index<1) || (index>channelCount) )
        viCheckParm( IVI_ERROR_INVALID_PARAMETER, 2, "Channel Index");
    
    /* Get channel name */
    viCheckParm( Ivi_GetNthChannelString (vi, index, &channelName), 2, "Index");
    checkErr( Ivi_GetUserChannelName (vi, channelName, &channelName));
    
    actualSize = strlen(channelName) + 1;
    
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
        memcpy(name, channelName, actualSize-1);
        name[actualSize-1] = '\0';
        }

Error:    
    Ivi_UnlockSession(vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_Connect
 *  Purpose:  This function connects channels 1 and 2.  The driver 
 *            calculates the shortest path between the two channels.
 *            If a path is not available, the function returns one of the 
 *            following errors:
 *            - ELVPC3A_ERROR_EXPLICIT_CONNECTION_EXISTS, if the two channels 
 *              are already explicitly connected.
 *            - ELVPC3A_ERROR_IS_CONFIGURATION_CHANNEL, if a channel is a
 *              configuration channel.  The error elaboration tells you which 
 *              channel is a configuration channel.
 *            - ELVPC3A_ERROR_ATTEMPT_TO_CONNECT_SOURCES, if both channels are
 *              connected to a different source.  The error elaboration tells 
 *              you to which source channel channels 1 and 2 are connected.
 *            - ELVPC3A_ERROR_RSRC_IN_USE, if a channel is a connected common
 *              multiplexer channel.
 *            - ELVPC3A_ERROR_CANNOT_CONNECT_TO_ITSELF, if the two channels
 *              are identical.
 *            - ELVPC3A_ERROR_PATH_NOT_FOUND, if the driver cannot find a 
 *              path between the two channels.
 *            The user can call the elvpc3a_GetPath function to obtain the path 
 *            the driver calculates.  The user can call the elvpc3a_SetPath 
 *            function to use an explicit path to make a connection.
 *****************************************************************************/
ViStatus _VI_FUNC elvpc3a_Connect (ViSession vi, ViConstString channel1,
                                    ViConstString channel2)
{
    ViStatus    error = VI_SUCCESS;
    ViString    path = VI_NULL;
    ViInt32     maxDepth;
    
    checkErr( Ivi_LockSession (vi, VI_NULL));

    if (Ivi_RangeChecking (vi))
        {
        checkErr( elvpc3a_VerifyIsEitherAConfigChannel (vi, channel1, 
                                                         channel2));
        checkErr( elvpc3a_VerifySourceChannelConflict (vi, channel1, 
                                                        channel2));
        }

    checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ELVPC3A_ATTR_MAXIMUM_DEPTH,
                                       0, &maxDepth));
    checkErr( elvpc3a_GetPossiblePath (vi, channel1, channel2, maxDepth, 
                                                &path));

    if (!Ivi_Simulating (vi))
        {
        ViSession   io = Ivi_IOSession (vi);
        
        checkErr( Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        checkErr( elvpc3a_ConnectPathOnInstrument (vi, io, path));
        }

    checkErr( elvpc3a_MarkPathAsConnected (vi, path));

    checkErr( elvpc3a_CheckStatus (vi));

	
#if 0
	{
		
		ViInt32 numChannels, i, j;
    	ViInt32 **table;
		

	
		const char* ll[] = {
		"",
		"CH1A",
		"CH1B",
		"C1A",
		"C1B",
		"CH2A",
		"CH2B",
		"C2A",
		"C2B",
		"CH3A",
		"CH3B",
		"C3A",
		"C3B",
		"CH4A",
		"CH4B",
		"C4A",
		"C4B",
		"REV3A",
		"REV3B",
		"REV4A",
		"REV4B"	
		};
		
			    checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, 
                                       ELVPC3A_ATTR_CHANNEL_COUNT, 
                                       0, &numChannels));
			
				checkErr( Ivi_GetAttributeViAddr (vi, VI_NULL, 
                                      ELVPC3A_ATTR_CHANNEL_MATRIX, 
                                      0, (void**)&table));	
	
	printf("\t|");	
	for (i=1; i<=numChannels; i++)
	{
		printf("%s|", ll[i]);	
	}
	printf("\n");
	for (i=1; i<=numChannels; i++)
	{
        printf("%s\t|", ll[i]);	
		for (j=1; j<=numChannels; j++)
		{
			
			if (table[i][j] == ELVPC3A_VAL_DIRECT_CONNECTION_IMPOSSIBLE)
			{
				printf(" X  |");
			}
			else
			{
				printf(" %d  |", (int)table[i][j]); 	
			}
		}
		printf("\n");
	}
	}
#endif	
	
	
	
Error:
    if (path != VI_NULL)
        Ivi_Free (vi, path);
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/***************************************************************************** 
 *  Function: elvpc3a_Disconnect
 *  Purpose:  This function disconnects the two channels. If no path
 *            exists between the two channels, the function returns the 
 *            ELVPC3A_ERROR_NO_SUCH_PATH error. You can verify the 
 *            validity and the current status of a path by calling the 
 *            elvpc3a_CanConnect function.
 *****************************************************************************/
ViStatus _VI_FUNC elvpc3a_Disconnect (ViSession vi, ViConstString channel1,
                                       ViConstString channel2)
{
    ViStatus    error = VI_SUCCESS;
    ViString    path = VI_NULL;
    
    checkErr( Ivi_LockSession (vi, VI_NULL));

    checkErr( elvpc3a_GetExistingPath (vi, channel1, channel2, &path));
    if (!Ivi_Simulating (vi))
	{
        ViSession   io = Ivi_IOSession (vi);
        
        checkErr( Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        checkErr( elvpc3a_DisconnectPathOnInstrument (vi, io, path));
	}

    checkErr( elvpc3a_MarkPathAsDisconnected (vi, path));

    checkErr( elvpc3a_CheckStatus (vi));

	// viCheckWarn( ELVPC3A_WARN_PATH_REMAINS);

Error:
    if (path != VI_NULL)
        Ivi_Free (vi, path);
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_DisconnectAll
 *  Purpose:  This function disconnects all currently existing paths.  This
 *            does not reset the switch card.  If the card is not capable of 
 *            removing all paths and always leaves at least one connected,
 *            the function returns the ELVPC3A_WARN_PATH_REMAINS warning.
 *****************************************************************************/
ViStatus _VI_FUNC elvpc3a_DisconnectAll (ViSession vi)
{
    ViStatus    	error = VI_SUCCESS;
	ViInt32			i = 0;
	ViInt32			port = 0;
	ViConstString   pvpc = NULL;
    
    checkErr( Ivi_LockSession (vi, VI_NULL));

	checkErr( Ivi_SetAttributeViInt32( vi, VI_NULL, ELVPC3A_ATTR_RELAYS, 0, 0));
	
    checkErr( elvpc3a_MarkAllAsDisconnected (vi));

    checkErr( elvpc3a_CheckStatus (vi));
#if 0
    viCheckWarn( ELVPC3A_WARN_PATH_REMAINS);
#endif

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_GetPath
 *  Purpose:  The user can create a path between two channels with the
 *            elvpc3a_Connect function, which calculates the shortest 
 *            path between the two channels.  The user can call this function 
 *            to obtain the actual path.  If no path currently exists between 
 *            the two channels, this function returns the 
 *            ELVPC3A_ERROR_NO_SUCH_PATH error. 
 *            It is possible for the user to inadvertently connect two channels 
 *            without making an explicit path between them.  In this case, 
 *            this function returns the ELVPC3A_ERROR_NO_SUCH_PATH error. 
 *****************************************************************************/
ViStatus _VI_FUNC elvpc3a_GetPath (ViSession vi, ViConstString channel1,
                                    ViConstString channel2, ViInt32 bufSize,
                                    ViChar pathList[])
{
    ViStatus    error = VI_SUCCESS;
    ViString    path = VI_NULL;
    ViInt32     pathLength;    
    
    checkErr( Ivi_LockSession (vi, VI_NULL));
    
    if (pathList == VI_NULL && bufSize != 0)
        viCheckParm( IVI_ERROR_INVALID_PARAMETER, 4, 
                     "Null address for Path List");
    
    checkErr( elvpc3a_GetExistingPath (vi, channel1, channel2, &path));
    pathLength = strlen (path) + 1;
    if (bufSize == 0)
        error = pathLength;
    else
        {
        strncpy (pathList, path, bufSize-1);
        pathList[bufSize-1] = '\0';
        if (pathLength > bufSize)
            error = pathLength;
        else
            error = VI_SUCCESS;
        }
    
Error:
    if (path != VI_NULL)
        Ivi_Free (vi, path);
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_SetPath
 *  Purpose:  This function connects two external channels using a channel 
 *            list. This list specifies the switch channels that form a unique 
 *            path between two external channels.  In case of failure, this 
 *            function provides the primary error, secondary error, and error 
 *            elaboration that describes the conditions under which the error 
 *            occurred:
 *            - ELVPC3A_ERROR_INVALID_SWITCH_PATH if the syntax of the path 
 *              string is not valid.
 *            - ELVPC3A_ERROR_EXPLICIT_CONNECTION_EXISTS, if a path already 
 *              exists between the two channels.
 *            - ELVPC3A_ERROR_ATTEMPT_TO_CONNECT_SOURCES, if setting this path
 *              would connect two sources;
 *            - ELVPC3A_ERROR_IS_CONFIGURATION_CHANNEL, if either of the channels
 *              at the ends of the path is a configuration channel.
 *            - ELVPC3A_ERROR_RSRC_IN_USE, if any of the channels in the path
 *              are already connected.
 *            - ELVPC3A_ERROR_CHANNELS_ALREADY_CONNECTED, if any of the channels 
 *              in the path are already connected.
 *****************************************************************************/
ViStatus _VI_FUNC elvpc3a_SetPath (ViSession vi, ViConstString pathList)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      *cleanPath = VI_NULL;
    
    checkErr( Ivi_LockSession (vi, VI_NULL));
    if (pathList == VI_NULL)
        viCheckParm( IVI_ERROR_INVALID_PARAMETER, 2, 
                     "Null passed for the Path List.");

    checkErr( Ivi_Alloc (vi, strlen (pathList) + 1, (ViAddr*)&cleanPath));
    strcpy (cleanPath, pathList);
    elvpc3a_CleanPath (cleanPath);

    if (Ivi_RangeChecking (vi))
        {
        checkErr( elvpc3a_VerifyPath (vi, cleanPath));
        }
        
    if (!Ivi_Simulating (vi))                   /* call this only when locked */
        {
        ViSession   io = Ivi_IOSession (vi);    /* call this only when locked */

        checkErr( Ivi_SetNeedToCheckStatus (vi, VI_TRUE));
        checkErr( elvpc3a_ConnectPathOnInstrument (vi, io, cleanPath));
        }

    checkErr( elvpc3a_MarkPathAsConnected (vi, cleanPath));
    checkErr( elvpc3a_CheckStatus (vi));
    
Error:
    if (cleanPath != VI_NULL)
        Ivi_Free (vi, cleanPath);
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_CanConnect
 *  Purpose:  This function tells the user whether the switch module is 
 *            capable of connecting the two channels. Possible
 *            values for the pathCapabilityRef output parameter are:
 *            - ELVPC3A_VAL_PATH_UNSUPPORTED indicates that the 
 *              driver cannot create the path.
 *            - ELVPC3A_VAL_PATH_AVAILABLE indicates that the 
 *              driver can create the path at this time.
 *            - ELVPC3A_VAL_RSRC_IN_USE indicates that the path 
 *              is valid, but the driver cannot create the path in the switch 
 *              module until the user releases another configuration channel by
 *              destroying some other path.
 *            - ELVPC3A_VAL_PATH_EXISTS indicates that the 
 *              channels are already connected.
 *            - ELVPC3A_VAL_SOURCE_CONFLICT indicates that 
 *              connecting these two channels would connect two source channels 
 *              eiher explicitly or implicitly.
 *            - ELVPC3A_VAL_CHANNEL_NOT_AVAILABLE indicates 
 *              that one of the two channels is a configuration channel.
 *            This function returns the ELVPC3A_WARN_IMPLICIT_CONNECTION_EXISTS
 *            warning if the path between the channels is not available, the
 *            channels are not explicitly connected, but the implicit 
 *            connection exists between them.
 *****************************************************************************/
ViStatus _VI_FUNC elvpc3a_CanConnect (ViSession vi, ViConstString channel1,
                                       ViConstString channel2,
                                       ViInt32 *pathCapabilityRef)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     ndxChannel1, ndxChannel2, **connections;
    ViBoolean   pathAvailable, muxChannelConnected, confChanConflict;
    ViBoolean   explicitlyConnected, pathSupported, sourceConflict;
    ViBoolean   connectionExists;
    ViInt32     pathCapability;
    ViInt32     maxDepth;        
        
    checkErr( Ivi_LockSession (vi, VI_NULL));
    checkErr( Ivi_GetChannelIndex (vi, channel1, &ndxChannel1));
    checkErr( Ivi_GetChannelIndex (vi, channel2, &ndxChannel2));
    if (ndxChannel1 == ndxChannel2)
        pathCapability = ELVPC3A_VAL_PATH_UNSUPPORTED;
    else
        {
        checkErr( Ivi_GetAttributeViAddr (vi, VI_NULL, 
                                          ELVPC3A_ATTR_CHANNEL_MATRIX, 
                                          0, (ViAddr*)&connections));

        explicitlyConnected = elvpc3a_ChannelsAreExplicitlyConnected( ndxChannel1, ndxChannel2);
        if (explicitlyConnected)
            pathCapability = ELVPC3A_VAL_PATH_EXISTS;
        else
            {
            checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ELVPC3A_ATTR_MAXIMUM_DEPTH,
                                               0, &maxDepth));
    
            checkErr( elvpc3a_IsPathSupported (vi, ndxChannel1, ndxChannel2, 
                                                connections, maxDepth, 
                                                &pathSupported));
            if (!pathSupported)
                pathCapability = ELVPC3A_VAL_PATH_UNSUPPORTED;
            else
                {
                checkErr( elvpc3a_SourceChannelConflict (vi, channel1, channel2, 
                                                          &sourceConflict));
                if (sourceConflict)
                    pathCapability = ELVPC3A_VAL_SOURCE_CONFLICT;
                else 
                    {
                    checkErr( elvpc3a_IsEitherAConfigChannel (vi, channel1,
                                                               channel2,
                                                               &confChanConflict));
                    if (confChanConflict)
                        pathCapability = ELVPC3A_VAL_CHANNEL_NOT_AVAILABLE;
                    else
                        {
                        checkErr( elvpc3a_IsPathAvailable (vi, ndxChannel1, 
                                                            ndxChannel2, 
                                                            connections, 
                                                            maxDepth,
                                                            &pathAvailable));
                        checkErr( elvpc3a_IsConnectedMuxChannel (vi, channel2,
                                                                  &muxChannelConnected));
                        checkErr( elvpc3a_ImplicitConnectionExists (vi, 
                                                                     channel1,
                                                                     channel2,
                                                                     &connectionExists));
                        if (connectionExists)
                            viCheckWarn( ELVPC3A_WARN_IMPLICIT_CONNECTION_EXISTS);
                        if (pathAvailable && !muxChannelConnected)
                            pathCapability = ELVPC3A_VAL_PATH_AVAILABLE;
                        else
                            pathCapability = ELVPC3A_VAL_RSRC_IN_USE;
                        }
                    }
                }
            }
        }
Error:
    *pathCapabilityRef = pathCapability;
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_IsDebounced
 *  Purpose:  This function returns the state of the switch module. It
 *            indicates if all the paths that you created have settled.
 *****************************************************************************/
ViStatus _VI_FUNC elvpc3a_IsDebounced (ViSession vi, ViBoolean *isDebounced)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Ivi_LockSession (vi, VI_NULL));

    checkErr( Ivi_GetAttributeViBoolean (vi, VI_NULL, 
                                         ELVPC3A_ATTR_IS_DEBOUNCED, 
                                         0, isDebounced));

Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_WaitForDebounce
 *  Purpose:  This function returns after all the paths that you create have
 *            settled.  If the maximum time you specify with the Maximum Time
 *            parameter elapses before the switch settles, this function 
 *            returns a timeout error.  
 *            NOTE: If the simluation is enabled, 
 *            ELVPC3A_ATTR_IS_DEBOUNCED always returns VI_TRUE with
 *            no delay.
 *****************************************************************************/
ViStatus _VI_FUNC elvpc3a_WaitForDebounce (ViSession vi, ViInt32 maxMilliseconds)
{
    ViStatus    error = VI_SUCCESS;
    ViBoolean   debounced = VI_FALSE;
    ViReal64    startTime, maxSeconds;
    
    checkErr( Ivi_LockSession (vi, VI_NULL));
    startTime = Timer ();
    maxSeconds = (ViReal64)maxMilliseconds / 1000.0;
    
    while (!debounced && (Timer () < (startTime + maxSeconds)))
        {
        checkErr( Ivi_GetAttributeViBoolean (vi, VI_NULL, 
                                             ELVPC3A_ATTR_IS_DEBOUNCED, 
                                             0, &debounced));
        }
    if (!debounced)
        viCheckErr( ELVPC3A_ERROR_MAX_TIME_EXCEEDED);
    
Error:
    Ivi_UnlockSession (vi, VI_NULL);
    return error;
}


/*****************************************************************************
 * Function: elvpc3a_GetAttribute<type> Functions                                    
 * Purpose:  These functions enable the instrument driver user to get 
 *           attribute values directly.  There are typesafe versions for 
 *           ViInt32, ViReal64, ViString, ViBoolean, and ViSession attributes.                                         
 *****************************************************************************/
ViStatus _VI_FUNC elvpc3a_GetAttributeViInt32 (ViSession vi, ViConstString channelName, 
                                                ViAttr attributeId, ViInt32 *value)
{                                                                                                           
    return Ivi_GetAttributeViInt32 (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                    value);
}                                                                                                           
ViStatus _VI_FUNC elvpc3a_GetAttributeViReal64 (ViSession vi, ViConstString channelName, 
                                                 ViAttr attributeId, ViReal64 *value)
{                                                                                                           
    return Ivi_GetAttributeViReal64 (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                     value);
}                                                                                                           
ViStatus _VI_FUNC elvpc3a_GetAttributeViString (ViSession vi, ViConstString channelName, 
                                                 ViAttr attributeId, ViInt32 bufSize, 
                                                 ViChar value[]) 
{   
    return Ivi_GetAttributeViString (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                     bufSize, value);
}   
ViStatus _VI_FUNC elvpc3a_GetAttributeViBoolean (ViSession vi, ViConstString channelName, 
                                                  ViAttr attributeId, ViBoolean *value)
{                                                                                                           
    return Ivi_GetAttributeViBoolean (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                      value);
}                                                                                                           
ViStatus _VI_FUNC elvpc3a_GetAttributeViSession (ViSession vi, ViConstString channelName, 
                                                  ViAttr attributeId, ViSession *value)
{                                                                                                           
    return Ivi_GetAttributeViSession (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                      value);
}                                                                                                           

/*****************************************************************************
 * Function: elvpc3a_SetAttribute<type> Functions                                    
 * Purpose:  These functions enable the instrument driver user to set 
 *           attribute values directly.  There are typesafe versions for 
 *           ViInt32, ViReal64, ViString, ViBoolean, and ViSession datatypes.                                         
 *****************************************************************************/
ViStatus _VI_FUNC elvpc3a_SetAttributeViInt32 (ViSession vi, ViConstString channelName, 
                                                ViAttr attributeId, ViInt32 value)
{                                                                                                           
    return Ivi_SetAttributeViInt32 (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                    value);
}                                                                                                           
ViStatus _VI_FUNC elvpc3a_SetAttributeViReal64 (ViSession vi, ViConstString channelName, 
                                                 ViAttr attributeId, ViReal64 value)
{                                                                                                           
    return Ivi_SetAttributeViReal64 (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                     value);
}                                                                                                           
ViStatus _VI_FUNC elvpc3a_SetAttributeViString (ViSession vi, ViConstString channelName, 
                                                 ViAttr attributeId, ViConstString value) 
{   
    return Ivi_SetAttributeViString (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                     value);
}   
ViStatus _VI_FUNC elvpc3a_SetAttributeViBoolean (ViSession vi, ViConstString channelName, 
                                                  ViAttr attributeId, ViBoolean value)
{                                                                                                           
    return Ivi_SetAttributeViBoolean (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                      value);
}                                                                                                           
ViStatus _VI_FUNC elvpc3a_SetAttributeViSession (ViSession vi, ViConstString channelName, 
                                                  ViAttr attributeId, ViSession value)
{                                                                                                           
    return Ivi_SetAttributeViSession (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                      value);
}                                                                                                           

/*****************************************************************************
 * Function: elvpc3a_CheckAttribute<type> Functions                                  
 * Purpose:  These functions enable the instrument driver user to check  
 *           attribute values directly.  These functions check the value you
 *           specify even if you set the ELVPC3A_ATTR_RANGE_CHECK 
 *           attribute to VI_FALSE.  There are typesafe versions for ViInt32, 
 *           ViReal64, ViString, ViBoolean, and ViSession datatypes.                         
 *****************************************************************************/
ViStatus _VI_FUNC elvpc3a_CheckAttributeViInt32 (ViSession vi, ViConstString channelName, 
                                                  ViAttr attributeId, ViInt32 value)
{                                                                                                           
    return Ivi_CheckAttributeViInt32 (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                      value);
}
ViStatus _VI_FUNC elvpc3a_CheckAttributeViReal64 (ViSession vi, ViConstString channelName, 
                                                   ViAttr attributeId, ViReal64 value)
{                                                                                                           
    return Ivi_CheckAttributeViReal64 (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                       value);
}                                                                                                           
ViStatus _VI_FUNC elvpc3a_CheckAttributeViString (ViSession vi, ViConstString channelName, 
                                                   ViAttr attributeId, ViConstString value)  
{   
    return Ivi_CheckAttributeViString (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                       value);
}   
ViStatus _VI_FUNC elvpc3a_CheckAttributeViBoolean (ViSession vi, ViConstString channelName, 
                                                    ViAttr attributeId, ViBoolean value)
{                                                                                                           
    return Ivi_CheckAttributeViBoolean (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                        value);
}                                                                                                           
ViStatus _VI_FUNC elvpc3a_CheckAttributeViSession (ViSession vi, ViConstString channelName, 
                                                    ViAttr attributeId, ViSession value)
{                                                                                                           
    return Ivi_CheckAttributeViSession (vi, channelName, attributeId, IVI_VAL_DIRECT_USER_CALL, 
                                        value);
}                                                                                                           


/*****************************************************************************
 *-------------------- Utility Functions (Not Exported) ---------------------*
 *****************************************************************************/

/*****************************************************************************
 *  Function: elvpc3a_GetPossiblePath
 *  Purpose:  This function creates a string containing the shortest path 
 *            between the channels channel1 and channel2. The caller is 
 *            responsible for freeing the pathRef variable.
 *            The caller should already verify that channels 1 and 2 are not
 *            configuration channels and that they are not connected to 
 *            different source channels.
 *****************************************************************************/
static ViStatus elvpc3a_GetPossiblePath (ViSession vi, ViConstString channel1,
                                          ViConstString channel2, 
                                          ViInt32 maxDepth,
                                          ViString *pathRef)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     ndxChannel1, ndxChannel2, numChannels;
    ViInt32     **connections;
    ViInt32     steps = 0;
    ViBoolean   isConnectedMuxChannel;
    ViString    path = VI_NULL;

    checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, ELVPC3A_ATTR_CHANNEL_COUNT,
                                       0, &numChannels));
                                       
    checkErr( Ivi_GetChannelIndex (vi, channel1, &ndxChannel1));
    checkErr( Ivi_GetChannelIndex (vi, channel2, &ndxChannel2));
    if (ndxChannel1 == ndxChannel2) 
        viCheckErrElab( ELVPC3A_ERROR_CANNOT_CONNECT_TO_ITSELF, channel1);
        
    checkErr( Ivi_GetAttributeViAddr (vi, VI_NULL, ELVPC3A_ATTR_CHANNEL_MATRIX,
                                      0, (ViAddr*)&connections));
    if (elvpc3a_ChannelsAreExplicitlyConnected( ndxChannel1, ndxChannel2))
        viCheckErr( ELVPC3A_ERROR_EXPLICIT_CONNECTION_EXISTS);
        
    /*
        Mux channels can be external channels or configuration channels,
        and can be connected to only one channel.  The same
        is true for any type of configuration channel, but some 
        non-mux external channels can be connected to multiple channels.
        If the external channels 1 and 2 are mux channels, they must
        not be connected.  
        NOTE:  elvpc3a_GetPossiblePathRecursively makes sure
        that no configuration channels along the path are already 
        connected.                                                                                                 
    */
    checkErr( elvpc3a_IsConnectedMuxChannel (vi, channel1, &isConnectedMuxChannel));
    if (isConnectedMuxChannel)
        viCheckErrElab( ELVPC3A_ERROR_RSRC_IN_USE, channel1);
    
    checkErr( elvpc3a_IsConnectedMuxChannel (vi, channel2, &isConnectedMuxChannel));
    if (isConnectedMuxChannel)
        viCheckErrElab( ELVPC3A_ERROR_RSRC_IN_USE, channel2);
    
    checkErr( elvpc3a_GetPossiblePathRecursively (vi, ndxChannel1, ndxChannel2, 
                                                   connections, numChannels, 
                                                   &path, 0, maxDepth, 
                                                   &steps));
    if (path == VI_NULL)
        viCheckErr( ELVPC3A_ERROR_PATH_NOT_FOUND);
        
Error:
    *pathRef = path;
    return error;
}


/*****************************************************************************
 *  Function: elvpc3a_GetPossiblePathRecursively
 *  Purpose:  This function creates the string containing the shortest path 
 *            between the channels with indices ndxChannel1 and ndxChannel2. 
 *            The caller should already verify that neither channel 1 nor 2 
 *            is a configuration channel or a connected mux common channel.
 *            Each invocation of this function makes one connection step
 *            and calls itself recursively to make additional steps if 
 *            necessary.
 *            If the caller has already found a path and is trying to find a 
 *            better one, then the maxDepth parameter represents the
 *            maximum number of connection steps that we can have between 
 *            ndxChannel1 and ndxChannel2 while still producing a shorter full
 *            path.   If the caller has not yet found a path then the maxDepth 
 *            parameter represents the maximum depth to which it is reasonable 
 *            to search. If maxDepth is zero or negative, the function does not 
 *            consider a maximum number of connection steps.  
 *            If this function finds a path, it returns 1 and sets the 
 *            *actualDepth parameter to the number of connection steps in the 
 *            path.  
 *            If the function cannot find a path, it returns 0 and does not
 *            change the value in the *actualDepth parameter.
 *****************************************************************************/
static ViStatus elvpc3a_GetPossiblePathRecursively (ViSession vi, 
                                                     ViInt32 ndxChannel1, 
                                                     ViInt32 ndxChannel2, 
                                                     ViInt32 **connections, 
                                                     ViInt32 numChannels, 
                                                     ViString *pathRef,
                                                     ViInt32 oldSize,
                                                     ViInt32 maxDepth,
                                                     ViInt32 *actualDepth)
{
    ViStatus        error = VI_SUCCESS;
    ViConstString   channel1;        /* channel string for ndxChannel1 */
    ViInt32         ndxChannelToTry; /* Holds index of channel through which */
                                     /* we try to connect channels 1 and 2.  */
    ViInt32         actualRemainingSteps; /* the number of steps made after    */
                                          /* this invocation in the smallest   */
                                          /* path found so far.                */
    ViBoolean       foundAtLeastOnePath = VI_FALSE;
    ViString        path = *pathRef;

    
    checkErr( Ivi_GetNthChannelString (vi, ndxChannel1, &channel1));
    
    if (elvpc3a_DirectConnectionIsPossible (ndxChannel1, ndxChannel2))  
        {
        ViInt32         legSize;
        ViConstString   channel2;
        
        checkErr( Ivi_GetNthChannelString (vi, ndxChannel2, &channel2));
        elvpc3a_CalculatePathLegSize (channel1, channel2, &legSize);
        if (path != VI_NULL)
            checkErr( Ivi_Free (vi, path));
        checkErr( Ivi_Alloc (vi, oldSize + legSize, (ViAddr*)&path));
        sprintf (path + oldSize, "%s->%s", channel1, channel2);
        actualRemainingSteps = 0;
        foundAtLeastOnePath = VI_TRUE;
        }
        
    else if (maxDepth == 1)              /* Since the channels cannot be directly */
        {                                /* connected, we cannot find a path with */
        foundAtLeastOnePath = VI_FALSE;  /* only 1 step. (Remember that there is  */
        }                                /* no maximum, if maxDepth <= 0.)        */
        
    else    /* use recursion to find rest of path */                                  
        { 
        ViInt32         maxRemainingSteps; /* number of steps allowed after the       */
                                           /* connection made in this invocation;     */
                                           /* can get smaller as we find better paths */
        /* 
            Set maxRemainingSteps to 1 less than the maxDepth because maxRemainingSteps
            does not include the connection step made in this invocation.
            If the caller has not yet found a path, this is a negative number. 
        */
        maxRemainingSteps = maxDepth - 1; 
    
        /*  
            Channel 1 is the reference channel.  Loop through all channels to 
            find one to which channel 1 can be connected.  For such channels, 
            call this function recursively to find the rest of the path to 
            channel 2, and use the path if it is the shortest one.
        */
        for (ndxChannelToTry=1; ndxChannelToTry <= numChannels; ndxChannelToTry++)
            {
            if (elvpc3a_DirectConnectionIsPossible(ndxChannel1, ndxChannelToTry)
                     && 
                elvpc3a_RecursionOnChannelIsAllowed( ndxChannelToTry))
                {
                ViConstString   channelToTry;
                ViBoolean       isUnconnectedConfigChannel;
                
                /* 
                    For there to be a path from channelToTry to channel2, 
                    channelToTry must be an unconnected configuration channel
                    or an unconnected mux common channel.  It must also not 
                    have been used in this current recursion stack.
                */
            
                checkErr( Ivi_GetNthChannelString (vi, ndxChannelToTry, 
                                                   &channelToTry));
                checkErr( elvpc3a_IsUnconnectedConfigChannel (vi, channelToTry,
                                                             &isUnconnectedConfigChannel));

                if (isUnconnectedConfigChannel) 
                    {
                    ViStatus    tempStatus;
                    ViInt32     legSize;

                    /* 
                        Call this function recursively.  
                        Subtract 1 from remaining steps because we are trying
                        to find a shorter path.  If we have not found a path
                        maxRemainingSteps is already negative.
                    */
                    elvpc3a_PreventRecursionOnChannel( ndxChannelToTry);
                    elvpc3a_CalculatePathLegSize (channel1, channelToTry, &legSize);
                    tempStatus = 
                        elvpc3a_GetPossiblePathRecursively (vi, ndxChannelToTry, 
                                                             ndxChannel2, 
                                                             connections, numChannels, 
                                                             &path, oldSize + legSize,
                                                             maxRemainingSteps,
                                                             &actualRemainingSteps);
                    elvpc3a_AllowRecursionOnChannel( ndxChannelToTry);
                    viCheckErr( tempStatus);   

                    /*
                        If a better path was found, place the leg from channel1  
                        to the index channelToTry just before the found path.
                    */
                    if (tempStatus == 1)    /* found a better path */
                        {
                        ViInt32 numBytesAdded;
                    
                        /*  Populate the path with the current depth of channels */
                        numBytesAdded = sprintf (path+oldSize, "%s->%s", channel1, channelToTry);
                        path[oldSize+numBytesAdded] = ',';
                        
                        /*  Try to find shorter path in the next iteration */
                        foundAtLeastOnePath = VI_TRUE;
                        maxRemainingSteps = actualRemainingSteps - 1;
                        if (actualRemainingSteps == 1)  
                            goto Error;    /* cannot do better than this */
                        }
                    }
                }
            }
        } 
        
Error:
    /*  If something failed, and we have the allocated memory, clean it up */
    if (error < VI_SUCCESS) 
        {
        if (path != VI_NULL)
            {
            Ivi_Free (vi, path);
            path = VI_NULL;
            }
        }
    else   /* no error */
        {
        if (foundAtLeastOnePath)
            {
            *actualDepth = actualRemainingSteps + 1;  /* include the current depth */
            error = 1;
            }
        else
            {
            error = 0;
            }
        }
    *pathRef = path;
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_IsUnconnectedConfigChannel
 *  Purpose:  This function determines whether a channel is a configuration
 *            channel, and if so, that it is not currently connected.
 *****************************************************************************/
static ViStatus elvpc3a_IsUnconnectedConfigChannel (ViSession vi, 
                                                     ViConstString channel,
                                                     ViBoolean *isUnconnectedConfigChannel)
{ 
    ViStatus    error = VI_SUCCESS;
    ViBoolean   isConfigChannel = VI_FALSE;
    ViInt32     ndxConnectedToChannel = ELVPC3A_VAL_NONE;
    
    checkErr( elvpc3a_GetConfigChannelInfo (vi, channel, &isConfigChannel,
                                             &ndxConnectedToChannel));

Error:
    *isUnconnectedConfigChannel = (isConfigChannel 
                                    && 
                                  (ndxConnectedToChannel == ELVPC3A_VAL_NONE));
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_GetConfigChannelInfo
 *  Purpose:  This function determines whether a channel is a configuration
 *            channel and the index of the channel it is connected to, if any.
 *            If the channel is not a configuration channel, this function 
 *            sets the *isConfigChannelRef parameter to VI_FALSE and the
 *            *ndxConnectedToChannelRef parameter to ELVPC3A_VAL_NONE.
 *            If the channel is a configuration channel, this function sets 
 *            the *isConfigChannelRef parameter to VI_TRUE and places
 *            the index of a channel it connects to into the 
 *            *ndxConnectedToChannelRef parameter.  If the channel is not 
 *            currently connected, this function sets the 
 *            *ndxConnectedToChannelRef parameter to ELVPC3A_VAL_NONE.
 *            You may pass VI_NULL for either output parameter.
 *****************************************************************************/
static ViStatus elvpc3a_GetConfigChannelInfo (ViSession vi, 
                                               ViConstString channel,
                                               ViBoolean *isConfigChannelRef,
                                               ViInt32 *ndxConnectedToChannelRef)
{ 
    ViStatus    error = VI_SUCCESS;
    ViBoolean   isConfigChannel = VI_FALSE;
    ViInt32     ndxConnectedToChannel = ELVPC3A_VAL_NONE;
    
    checkErr( Ivi_GetAttributeViBoolean (vi, channel,
                                         ELVPC3A_ATTR_IS_CONFIGURATION_CHANNEL,
                                         0, &isConfigChannel));
    if (isConfigChannel)
        {                    
        checkErr( Ivi_GetAttributeViInt32 (vi, channel,
                                           ELVPC3A_ATTR_CONFIGURATION_CHANNEL_INDEX,
                                           0, &ndxConnectedToChannel));
        }                                           

Error:
    if (ndxConnectedToChannelRef != VI_NULL)
        *ndxConnectedToChannelRef = ndxConnectedToChannel;
    if (isConfigChannelRef != VI_NULL)
        *isConfigChannelRef = isConfigChannel;
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_ConnectPathOnInstrument
 *  Purpose:  This function performs I/O to close switches on 
 *            the instrument, based on a path.
 *            The caller must pass a correctly formatted path.
 *****************************************************************************/
static ViStatus elvpc3a_ConnectPathOnInstrument (ViSession vi, ViSession io,
                                                  ViConstString path)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     index, legSize;
    ViChar      channel1[BUFFER_SIZE], channel2[BUFFER_SIZE];
	ViInt32		i, output = 0;
	ViInt32*	counter = NULL;

    checkErr( Ivi_GetAttributeViInt32( vi, VI_NULL, ELVPC3A_ATTR_RELAYS, 0, &output));
	checkErr( Ivi_GetAttributeViAddr( vi, VI_NULL, ELVPC3A_ATTR_RELAY_COUNTER, 0, (ViAddr*)&counter));
	
	
	
	index = 0;
    while (sscanf (path+index, "%[^-]->%[^,]", channel1, channel2) == 2)
	{
		for( i = 0; gs_RelTable[i].channel1; i++)
		{
			if ( strcmp(channel1, gs_RelTable[i].channel1) == 0 || 
				 strcmp(channel1, gs_RelTable[i].channel2) == 0 )
			{
#if 0
				if ( strlen(channel1) == 3 )
				{
					if (( output & ( 1 << i ) ) != 0 )  counter[i] = counter[i] + 1;
					
					output &= ~( 1 << i );	
				}
				else
#endif
				{
					if (( output & ( 1 << i ) ) == 0 )  counter[i] = counter[i] + 1; 
					
					output |= ( 1 << i );
				}
			}
		}
     
        elvpc3a_CalculatePathLegSize (channel1, channel2, &legSize);
        index += (legSize - 1);          /* position at comma or NUL byte */
        if (path[index] == '\0')
            break;
        else
            index++;
	}
	
	checkErr( Ivi_SetAttributeViInt32( vi, VI_NULL, ELVPC3A_ATTR_RELAYS, 0, output));
        
Error:
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_DisconnectPathOnInstrument
 *  Purpose:  This function performs I/O to open switches on 
 *            the instrument based on a path.
 *            The caller must pass a correctly formatted path.
 *****************************************************************************/
static ViStatus elvpc3a_DisconnectPathOnInstrument (ViSession vi, ViSession io,
                                                     ViConstString path)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     index, legSize;
    ViChar      channel1[BUFFER_SIZE], channel2[BUFFER_SIZE];
	ViInt32		i, output = 0;

    checkErr( Ivi_GetAttributeViInt32( vi, VI_NULL, ELVPC3A_ATTR_RELAYS, 0, &output));

    index = 0;
    while (sscanf (path+index, "%[^-]->%[^,]", channel1, channel2) == 2)
	{
    	for( i = 0; gs_RelTable[i].channel1; i++)
		{
			if ( strcmp(channel1, gs_RelTable[i].channel1) == 0 || 
				 strcmp(channel1, gs_RelTable[i].channel2) == 0 )
			{
#if 0
				if ( strlen(channel1) == 3 )
				{
					output |= ( 1 << i );
				}
				else
#endif
				{
					output &= ~( 1 << i );	
				}
			}
		}

     
        elvpc3a_CalculatePathLegSize (channel1, channel2, &legSize);
        index += (legSize - 1);          /* position at comma or NUL byte */
        if (path[index] == '\0')
            break;
        else
            index++;
	}    
    checkErr( Ivi_SetAttributeViInt32( vi, VI_NULL, ELVPC3A_ATTR_RELAYS, 0, output));
	
Error:
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_MarkPathAsConnected
 *  Purpose:  This function updates the channel matrix to reflect changes that
 *            result from creating a new path.
 *            This function does not validate the path parameter.
 *            The caller must pass a correctly formatted path.
 *            NOTE: Even though this function checks for errors, it cannot fail
 *            as long as range checking is on and the calling function checks
 *            for errors before calling this function.  Thus, we do not have
 *            to try to clean up after a failure in the middle of the function.
 *****************************************************************************/
static ViStatus elvpc3a_MarkPathAsConnected (ViSession vi, ViConstString path)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      channel1[BUFFER_SIZE], channel2[BUFFER_SIZE];
    ViInt32     index, legSize, ndxFirst, ndxLast;
    ViInt32     **connections;
    ViReal64    currentTime, settlingTime, maxSettlingTime, oldSettlingMoment;
    
    currentTime = Timer ();
    maxSettlingTime = 0.0;
    
    index = 0;
    while (sscanf (path+index, "%[^-]->%[^,]", channel1, channel2) == 2)
        {
        checkErr( Ivi_GetAttributeViReal64 (vi, channel1, 
                                            ELVPC3A_ATTR_SETTLING_TIME,
                                            0, &settlingTime));
        if (settlingTime > maxSettlingTime)
            maxSettlingTime = settlingTime;

        checkErr( elvpc3a_MarkConnectionAlongPath (vi, channel1, channel2));
        elvpc3a_CalculatePathLegSize (channel1, channel2, &legSize);
        index += (legSize - 1);          /* position at comma or NUL byte */
        if (path[index] == '\0')
            break;
        else
            index++;
        }    

    checkErr( Ivi_GetAttributeViReal64 (vi, channel2,
                                        ELVPC3A_ATTR_SETTLING_TIME,
                                        0, &settlingTime));
    if (settlingTime > maxSettlingTime)
        maxSettlingTime = settlingTime;

    checkErr( Ivi_GetAttributeViReal64 (vi, VI_NULL,
                                        ELVPC3A_ATTR_SETTLING_MOMENT,
                                        0, &oldSettlingMoment));
    if ((currentTime+maxSettlingTime) > oldSettlingMoment)
        checkErr( Ivi_SetAttributeViReal64 (vi, VI_NULL,
                                            ELVPC3A_ATTR_SETTLING_MOMENT,
                                            0, currentTime+maxSettlingTime));

    checkErr( Ivi_GetChannelIndex (vi, channel2, &ndxLast));
    sscanf (path, "%[^-]", channel1);
    checkErr( Ivi_GetChannelIndex (vi, channel1, &ndxFirst));
    checkErr( Ivi_GetAttributeViAddr (vi, VI_NULL, 
                                      ELVPC3A_ATTR_CHANNEL_MATRIX,
                                      0, (ViAddr*)&connections));
    elvpc3a_MarkChannelsAsExplicitlyConnected( ndxFirst, ndxLast);

Error:
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_MarkPathAsDisconnected
 *  Purpose:  This function updates the channel matrix to reflect changes that
 *            result from destroying an existing path.
 *            This function does not validate the path parameter.
 *            The caller must pass a correctly formatted path.
 *            NOTE: Even though this function checks for errors, it cannot fail
 *            as long as range checking is on and the calling function checks
 *            for errors before calling this function.  Thus, we do not have
 *            to try to clean up after a failure in the middle of the function.
 *****************************************************************************/
static ViStatus elvpc3a_MarkPathAsDisconnected (ViSession vi, ViConstString path)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      channel1[BUFFER_SIZE], channel2[BUFFER_SIZE];
    ViInt32     index, legSize, ndxFirst, ndxLast, ndxSource1;
    ViInt32     **connections;
    
    sscanf (path, "%[^-]", channel1);
    checkErr( Ivi_GetChannelIndex (vi, channel1, &ndxFirst));
    checkErr( Ivi_GetAttributeViInt32 (vi, channel1,
                                       ELVPC3A_ATTR_ORIGINAL_SOURCE_CHANNEL_INDEX,
                                       0, &ndxSource1));
    index = 0;
    while (sscanf (path+index, "%[^-]->%[^,]", channel1, channel2) == 2)
        {
        checkErr( elvpc3a_DeleteConnectionAlongPath (vi, channel1, channel2));
        elvpc3a_CalculatePathLegSize (channel1, channel2, &legSize);
        index += (legSize - 1);          /* position at comma or NUL byte */
        if (path[index] == '\0')
            break;
        else
            index++;
        }    
    checkErr( Ivi_GetChannelIndex (vi, channel2, &ndxLast));
    checkErr( Ivi_GetAttributeViAddr (vi, VI_NULL, 
                                      ELVPC3A_ATTR_CHANNEL_MATRIX,
                                      0, (ViAddr*)&connections));
    /* 
        Because the channels were connected, they must have the same value for the 
        ELVPC3A_ATTR_ORIGINAL_SOURCE_CHANNEL_INDEX attribute.  If that value is 
        something other than ELVPC3A_VAL_NONE, recalculate the information for 
        that source channel throughout the entire matrix.
    */    
    if (ndxSource1 != ELVPC3A_VAL_NONE)
        checkErr( elvpc3a_RecalculateSourceInformation (vi, connections,
                                                         ndxSource1));
    elvpc3a_MarkChannelsAsNotExplicitlyConnected( ndxFirst, ndxLast);

Error:
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_MarkAllAsDisconnected
 *  Purpose:  This function updates the channel matrix to reflect changes that
 *            result from calling the elvpc3a_DisconnectAll function.  
 *            NOTE: Even though this function checks for errors, it cannot fail
 *            as long as range checking is on and the calling function checks
 *            for errors before calling this function.  Thus, we do not have
 *            to try to clean up after a failure in the middle of the function.
 *****************************************************************************/
static ViStatus elvpc3a_MarkAllAsDisconnected (ViSession vi)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     i, j, numChannels;
    ViInt32     **connections;
    
    checkErr( Ivi_GetAttributeViAddr (vi, VI_NULL, 
                                      ELVPC3A_ATTR_CHANNEL_MATRIX,
                                      0, (ViAddr*)&connections));
    checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, 
                                       ELVPC3A_ATTR_CHANNEL_COUNT,
                                       0, &numChannels));
    for (i = 1; i < numChannels; i++)
        for (j = i; j <= numChannels; j++)
            {
            elvpc3a_MarkChannelsAsNotDirectlyConnected( i, j);
            elvpc3a_MarkChannelsAsNotExplicitlyConnected( i, j);
            }

    for (i = 1; i <= numChannels; i++)
    {
        ViString channel = VI_NULL;
        ViBoolean isSource = VI_FALSE;

        checkErr( Ivi_GetNthChannelString (vi, i, &channel)); 
        checkErr( Ivi_GetAttributeViBoolean (vi, channel, ELVPC3A_ATTR_IS_SOURCE_CHANNEL, 0, &isSource));
        checkErr( Ivi_SetAttributeViInt32 (vi, channel, ELVPC3A_ATTR_ORIGINAL_SOURCE_CHANNEL_INDEX, 0, isSource?i:ELVPC3A_VAL_NONE));
        checkErr( Ivi_SetAttributeViInt32 (vi, channel, ELVPC3A_ATTR_CONFIGURATION_CHANNEL_INDEX, 0, ELVPC3A_VAL_NONE));
    }

Error:
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_SourceChannelConflict
 *  Purpose:  This function returns the connectibility status of two channels
 *            with respect to their connection to a source channel.  This 
 *            function sets the *hasConflictRef parameter to VI_TRUE if both 
 *            channels are connected to a different source channel.
 *****************************************************************************/
static ViStatus elvpc3a_SourceChannelConflict (ViSession vi, ViConstString channel1, 
                                                ViConstString channel2,
                                                ViBoolean *hasConflictRef)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     ndxSource1, ndxSource2;
    ViBoolean   hasConflict;
    
    checkErr( Ivi_GetAttributeViInt32 (vi, channel1, 
                                       ELVPC3A_ATTR_ORIGINAL_SOURCE_CHANNEL_INDEX, 
                                       0, &ndxSource1));
    checkErr( Ivi_GetAttributeViInt32 (vi, channel2, 
                                       ELVPC3A_ATTR_ORIGINAL_SOURCE_CHANNEL_INDEX, 
                                       0, &ndxSource2));
             
    hasConflict = (ndxSource1 != ndxSource2) 
                    && (ndxSource1 != ELVPC3A_VAL_NONE) 
                    && (ndxSource2 != ELVPC3A_VAL_NONE);

Error:
    *hasConflictRef = hasConflict;
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_VerifySourceChannelConflict
 *  Purpose:  This function returns the ELVPC3A_ERROR_ATTEMPT_TO_CONNECT_SOURCES
 *            error if both channels are connected to a different source 
 *            channel.
 *****************************************************************************/
static ViStatus elvpc3a_VerifySourceChannelConflict (ViSession vi, 
                                                      ViConstString channel1, 
                                                      ViConstString channel2)
{
    ViStatus    error = VI_SUCCESS;
    ViBoolean   hasConflict;
    ViInt32     ndxSource1, ndxSource2;
    ViString    source1, source2;
    ViChar      elabBuf[IVI_MAX_MESSAGE_BUF_SIZE];
    
    checkErr( Ivi_GetAttributeViInt32 (vi, channel1, 
                                       ELVPC3A_ATTR_ORIGINAL_SOURCE_CHANNEL_INDEX, 
                                       0, &ndxSource1));
    checkErr( Ivi_GetAttributeViInt32 (vi, channel2, 
                                       ELVPC3A_ATTR_ORIGINAL_SOURCE_CHANNEL_INDEX, 
                                       0, &ndxSource2));
             
    hasConflict = (ndxSource1 != ndxSource2) 
                    && (ndxSource1 != ELVPC3A_VAL_NONE) 
                    && (ndxSource2 != ELVPC3A_VAL_NONE);
    if (hasConflict)
        {
        Ivi_GetNthChannelString (vi, ndxSource1, &source1); /* cannot fail */
        Ivi_GetNthChannelString (vi, ndxSource2, &source2); /* cannot fail */
        sprintf (elabBuf, "Channels %.50s and %.50s "
                          "connected to sources %.50s and %.50s ", 
                          channel1, channel2, source1, source2);
        viCheckErrElab( ELVPC3A_ERROR_ATTEMPT_TO_CONNECT_SOURCES, elabBuf);
        }
        
Error:
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_IsEitherAConfigChannel
 *  Purpose:  This function sets the *atLeastOneIsAConfigChannelRef parameter
 *            to VI_TRUE if either channel is a configuration channel. You 
 *            cannot connect an external signal to a configuration channel.
 *****************************************************************************/
static ViStatus elvpc3a_IsEitherAConfigChannel (ViSession vi, 
                                                 ViConstString channel1, 
                                                 ViConstString channel2,
                                                 ViBoolean *atLeastOneIsAConfigChannelRef)
{
    ViStatus    error = VI_SUCCESS;
    ViBoolean   atLeastOneIsAConfigChannel = VI_TRUE;
    
    checkErr( Ivi_GetAttributeViBoolean (vi, channel1, 
                                         ELVPC3A_ATTR_IS_CONFIGURATION_CHANNEL, 
                                         0, &atLeastOneIsAConfigChannel));
    if (!atLeastOneIsAConfigChannel)
        checkErr( Ivi_GetAttributeViBoolean (vi, channel2, 
                                             ELVPC3A_ATTR_IS_CONFIGURATION_CHANNEL, 
                                             0, &atLeastOneIsAConfigChannel));

Error:
    *atLeastOneIsAConfigChannelRef = atLeastOneIsAConfigChannel;
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_VerifyIsEitherAConfigChannel
 *  Purpose:  This function returns the ELVPC3A_ERROR_IS_CONFIGURATION_CHANNEL
 *            error if either channel1 or channel2 is a configuration channel. 
 *            You cannot connect an external signal to a configuration channel.
 *****************************************************************************/
static ViStatus elvpc3a_VerifyIsEitherAConfigChannel (ViSession vi, 
                                                   ViConstString channel1, 
                                                   ViConstString channel2)
{
    ViStatus    error = VI_SUCCESS;
    ViBoolean   isConfigChannel = VI_TRUE;
    
    checkErr( Ivi_GetAttributeViBoolean (vi, channel1, 
                                         ELVPC3A_ATTR_IS_CONFIGURATION_CHANNEL, 
                                         0, &isConfigChannel));
    if (isConfigChannel)
        viCheckErrElab (ELVPC3A_ERROR_IS_CONFIGURATION_CHANNEL, channel1);
    checkErr( Ivi_GetAttributeViBoolean (vi, channel2, 
                                         ELVPC3A_ATTR_IS_CONFIGURATION_CHANNEL, 
                                         0, &isConfigChannel));
    if (isConfigChannel)
        viCheckErrElab (ELVPC3A_ERROR_IS_CONFIGURATION_CHANNEL, channel2);

Error:
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_MarkConnectionAlongPath
 *  Purpose:  This function updates the channel matrix and all relevant 
 *            attributes to indicate that a direct connection exists between 
 *            two channels along a path.                       
 *            NOTE: The caller must call this function only when all the
 *            configuration channels in the path were previously unconnected.
 *            NOTE: The caller must not call this function when the two
 *            channels are connected to different sources.
 *            NOTE: Even though this function checks for errors, it cannot fail
 *            as long as range checking is on and the calling function checks
 *            for errors before calling this function.  Thus, we do not have
 *            to try to clean up after a failure in the middle of the function.
 *****************************************************************************/
static ViStatus elvpc3a_MarkConnectionAlongPath (ViSession vi, 
                                                  ViConstString channel1, 
                                                  ViConstString channel2)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     **connections;
    ViInt32     ndxChannel1, ndxChannel2;
    ViInt32     ndxSource1, ndxSource2;
    ViBoolean   channel2IsConfig;
    
    checkErr( Ivi_GetChannelIndex (vi, channel1, &ndxChannel1));
    checkErr( Ivi_GetChannelIndex (vi, channel2, &ndxChannel2));
    checkErr( Ivi_GetAttributeViAddr (vi, VI_NULL, 
                                      ELVPC3A_ATTR_CHANNEL_MATRIX, 
                                      0, (ViAddr*)&connections));

    /* 
        When a configuration channel is in a path, it is physically connected
        to two other channels.  However, we record only one of these channels 
        in the ELVPC3A_ATTR_CONFIGURATION_CHANNEL_INDEX attribute.  When 
        this function is repeatedly called to record the connections in a path, 
        we mark channel2 (if it is a configuration channel) as connected to 
        channel1.  Thus, channel1 (if it is a configuration channel) has 
        already been marked as connected to the previous channel.)   
    */
    checkErr( Ivi_GetAttributeViBoolean (vi, channel2, 
                                         ELVPC3A_ATTR_IS_CONFIGURATION_CHANNEL,
                                         0, &channel2IsConfig));
    if (channel2IsConfig)
        {
        checkErr( Ivi_SetAttributeViInt32 (vi, channel2,
                                           ELVPC3A_ATTR_CONFIGURATION_CHANNEL_INDEX,
                                           0, ndxChannel1));
        }

    elvpc3a_MarkChannelsAsDirectlyConnected( ndxChannel1, ndxChannel2);


    
    /* 
        If exactly one channel is connected to a source, then we
        call elvpc3a_MarkOriginalSourceRecursively to 
        mark the other channel (and all of the channels to which it is
        connected) as connected to the source.
    */
    checkErr( Ivi_GetAttributeViInt32 (vi, channel1,
                                       ELVPC3A_ATTR_ORIGINAL_SOURCE_CHANNEL_INDEX,
                                       0, &ndxSource1));
    checkErr( Ivi_GetAttributeViInt32 (vi, channel2,
                                       ELVPC3A_ATTR_ORIGINAL_SOURCE_CHANNEL_INDEX,
                                       0, &ndxSource2));
    if (ndxSource1 != ndxSource2)
        {
        ViInt32     numChannels;
        
        checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, 
                                           ELVPC3A_ATTR_CHANNEL_COUNT, 
                                           0, &numChannels));
                                           
        if (ndxSource1 != ELVPC3A_VAL_NONE)
            {
            checkErr( elvpc3a_MarkOriginalSourceRecursively (vi, ndxChannel1, ndxSource1,
                                                              numChannels, connections));
            }
        else 
            {
            checkErr( elvpc3a_MarkOriginalSourceRecursively (vi, ndxChannel2, ndxSource2,
                                                              numChannels, connections));
            }
        }
        
Error:
    return error;
}
   
   
/*****************************************************************************
 *  Function: elvpc3a_DeleteConnectionAlongPath
 *  Purpose:  This function updates the channel matrix and all relevant 
 *            attributes to indicate that a direct connection no longer exists 
 *            between two channels along a path.                       
 *            NOTE: Even though this function checks for errors, it cannot fail
 *            as long as range checking is on and the calling function checks
 *            for errors before calling this function.  Thus, we do not have
 *            to try to clean up after a failure in the middle of the function.
 *            NOTE: The caller must call this function repeatedly along the
 *            path, using the same direction along the path with which the
 *            path was created.
 *****************************************************************************/
static ViStatus elvpc3a_DeleteConnectionAlongPath (ViSession vi, 
                                                    ViConstString channel1, 
                                                    ViConstString channel2)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     ndxChannel1, ndxChannel2;
    ViInt32     **connections;
    ViBoolean   channel2IsConfig;
    
    checkErr( Ivi_GetChannelIndex (vi, channel1, &ndxChannel1));
    checkErr( Ivi_GetChannelIndex (vi, channel2, &ndxChannel2));
    checkErr( Ivi_GetAttributeViAddr (vi, VI_NULL, 
                                      ELVPC3A_ATTR_CHANNEL_MATRIX, 
                                      0, (ViAddr*)&connections));
                                      
    elvpc3a_MarkChannelsAsNotDirectlyConnected (ndxChannel1, ndxChannel2);
    
    /* 
        When a configuration channel is in a path, it is connected
        physically to two other channels.  However, we record only one of
        these channels in the ELVPC3A_ATTR_CONFIGURATION_CHANNEL_INDEX
        attribute.  When this function is called, channel2 (if it is a 
        configuration channel) is already marked as connected to channel1, so 
        mark channel2 as not being connected. 
    */
    checkErr( Ivi_GetAttributeViBoolean (vi, channel2, 
                                         ELVPC3A_ATTR_IS_CONFIGURATION_CHANNEL,
                                         0, &channel2IsConfig));
    if (channel2IsConfig)
        {
        checkErr( Ivi_SetAttributeViInt32 (vi, channel2,
                                           ELVPC3A_ATTR_CONFIGURATION_CHANNEL_INDEX,
                                           0, ELVPC3A_VAL_NONE));
        }

Error:
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_RecalculateSourceInformation
 *  Purpose:  This function marks all channels that you connect to a specific 
 *            source as connected to that source. Call this function after 
 *            disconnecting a channel that is connected to the source.  
 *            Doing so ensures that only the channels still connected to the 
 *            source are marked as conected to the source.
 *            NOTE: Even though this function checks for errors, it cannot fail
 *            as long as range checking is on and the calling function checks
 *            for errors before calling this function.  Thus, we do not have
 *            to try to clean up after a failure in the middle of the function.
 *****************************************************************************/
static ViStatus elvpc3a_RecalculateSourceInformation (ViSession vi, 
                                                       ViInt32 **connections,
                                                       ViInt32 ndxTheSource)
{
    ViStatus        error = VI_SUCCESS;
    ViInt32         i, numChannels;
    
    checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, 
                                       ELVPC3A_ATTR_CHANNEL_COUNT, 
                                       0, &numChannels));
    /*  
        Because some of the channels previously connected to the source channel
        might no longer be connected to it, unmark all such channels and remark 
        the channels starting from the source channel.
    */
        
    for (i=1; i <= numChannels; i++)
        {
        ViConstString   channel;
        ViInt32         tmpSrc;
    
        if (i != ndxTheSource)
            {
            checkErr( Ivi_GetNthChannelString (vi, i, &channel));
            checkErr( Ivi_GetAttributeViInt32 (vi, channel, 
                                               ELVPC3A_ATTR_ORIGINAL_SOURCE_CHANNEL_INDEX, 
                                               0, &tmpSrc));
            if (tmpSrc == ndxTheSource)
                checkErr( Ivi_SetAttributeViInt32 (vi, channel, 
                                                   ELVPC3A_ATTR_ORIGINAL_SOURCE_CHANNEL_INDEX, 
                                                   0, ELVPC3A_VAL_NONE));
            }
        }
        
    /*
        Now re-mark the channels connected to the source.
    */
    checkErr( elvpc3a_MarkOriginalSourceRecursively (vi, ndxTheSource, ndxTheSource, 
                                                      numChannels, connections));
                                                      
Error:
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_MarkOriginalSourceRecursively
 *  Purpose:  This function marks all channels you connect to a source as
 *            connected to that source.  It recursively searches all
 *            existing connections and updates the 
 *            ELVPC3A_ATTR_ORIGINAL_SOURCE_CHANNEL_INDEX to point to the
 *            original source channel.
 *            NOTE: Even though this function checks for errors, it cannot fail
 *            as long as range checking is on and the calling function checks
 *            for errors before calling this function.  Thus, we do not have
 *            to try to clean up after a failure in the middle of the function.
 *****************************************************************************/
static ViStatus elvpc3a_MarkOriginalSourceRecursively (ViSession vi, 
                                                        ViInt32 ndxFirstChannel,
                                                        ViInt32 originalSourceIndex, 
                                                        ViInt32 numChannels, 
                                                        ViInt32 **connections)
{
    ViStatus        error = VI_SUCCESS;
    ViInt32         ndxOtherChannel;
    ViConstString   otherChannel;
    
    /*
        This function searches for all channels that are directly connected 
        to the first channel, marks them as connected to the original source,
        and calls itself recursively on those channels.
        If a channel is already marked as connected to the source, it is not
        necessary to call the function recursively on it.  
        NOTE:  Any channel connected to the first channel cannot be connected
        to any other source.
    */
    
    for (ndxOtherChannel=1;  ndxOtherChannel <= numChannels; ndxOtherChannel++)
        {
        if (elvpc3a_ChannelsAreDirectlyConnected( ndxFirstChannel, ndxOtherChannel))
            {
            ViInt32 tmpSrc;
            
            checkErr( Ivi_GetNthChannelString (vi, ndxOtherChannel, &otherChannel));
            checkErr( Ivi_GetAttributeViInt32 (vi, otherChannel, 
                                               ELVPC3A_ATTR_ORIGINAL_SOURCE_CHANNEL_INDEX, 
                                               0, &tmpSrc));
            if (tmpSrc == ELVPC3A_VAL_NONE)
                {
                checkErr( Ivi_SetAttributeViInt32 (vi, otherChannel, 
                                                   ELVPC3A_ATTR_ORIGINAL_SOURCE_CHANNEL_INDEX, 
                                                   0, originalSourceIndex));
                checkErr( elvpc3a_MarkOriginalSourceRecursively (vi, 
                                                                  ndxOtherChannel, 
                                                                  originalSourceIndex, 
                                                                  numChannels, 
                                                                  connections));
                }
            }
        }
Error:
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_GetExistingPath
 *  Purpose:  This function creates the string containing the existing path 
 *            between channels 1 and 2.  The caller is responsible for freeing 
 *            the pathRef variable.  If the path does not exist, the function 
 *            returns the ELVPC3A_ERROR_NO_SUCH_PATH error.
 *****************************************************************************/
static ViStatus elvpc3a_GetExistingPath (ViSession vi, ViConstString channel1,
                                          ViConstString channel2, 
                                          ViString *pathRef)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     ndxChannel1, ndxChannel2, numChannels;
    ViInt32     **connections;
    ViString    path;

    path = VI_NULL;
    checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL,
                                       ELVPC3A_ATTR_CHANNEL_COUNT,
                                       0, &numChannels));
    checkErr( Ivi_GetChannelIndex (vi, channel1, &ndxChannel1));
    checkErr( Ivi_GetChannelIndex (vi, channel2, &ndxChannel2));
    checkErr( Ivi_GetAttributeViAddr (vi, VI_NULL, 
                                      ELVPC3A_ATTR_CHANNEL_MATRIX, 
                                      0, (ViAddr*)&connections));
                                      
    if (!elvpc3a_ChannelsAreExplicitlyConnected( ndxChannel1, ndxChannel2))
        viCheckErr( ELVPC3A_ERROR_NO_SUCH_PATH);

    /*
        This driver stores the path connections in a unidirectional manner, 
        using the ELVPC3A_ATTR_CONFIGURATION_CHANNEL_INDEX attribute.
        Each configuration channel can have only one value for the attribute.  
        Thus, the connections can point in only one direction.  When creating
        a path, the driver always chooses one direction.
        Consequently, we first try to find an existing path from channel1 to 
        channel2.  If there is no path in that direction, we try to find an
        existing path from channel2 to channel1.
    */
    checkErr( elvpc3a_GetExistingPathRecursively (vi, ndxChannel1, ndxChannel2,
                                                   connections, numChannels,
                                                   &path, 0));
    if (path == VI_NULL)
        checkErr( elvpc3a_GetExistingPathRecursively (vi, ndxChannel2, ndxChannel1,
                                                       connections, numChannels,
                                                       &path, 0));
    if (path == VI_NULL)
        viCheckErr( ELVPC3A_ERROR_NO_SUCH_PATH);
        
Error:
    *pathRef = path;
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_GetExistingPathRecursively
 *  Purpose:  This function creates a string containing the existing path 
 *            between the channels with indices ndxChannel1 and ndxChannel2. 
 *            If this function finds a path, it sets the *pathRef parameter
 *            to contain the path.  Otherwise, it does not change the 
 *            *pathRef parameter.
 *****************************************************************************/
static ViStatus elvpc3a_GetExistingPathRecursively (ViSession vi, 
                                                     ViInt32 ndxChannel1, 
                                                     ViInt32 ndxChannel2, 
                                                     ViInt32 **connections, 
                                                     ViInt32 numChannels, 
                                                     ViString *pathRef, 
                                                     ViInt32 oldSize)
{
    ViStatus        error = VI_SUCCESS;
    ViInt32         ndxChannelToTry, legSize;
    ViConstString   channel1, vName1;
    ViBoolean       pathFound = VI_FALSE;
    ViString        path = VI_NULL;
    
    checkErr( Ivi_GetNthChannelString (vi, ndxChannel1, &channel1));
    checkErr( Ivi_GetUserChannelName (vi, channel1, &vName1));
    
    /*
        If channel1 is directly connected to channel2, we have found the end of
        the path.
    */
    if (elvpc3a_ChannelsAreDirectlyConnected( ndxChannel1, ndxChannel2))
        {
        ViConstString   channel2, vName2;
        
        checkErr( Ivi_GetNthChannelString (vi, ndxChannel2, &channel2));
        checkErr( Ivi_GetUserChannelName (vi, channel2, &vName2));

        elvpc3a_CalculatePathLegSize (vName1, vName2, &legSize);
        if (path != VI_NULL)
            checkErr( Ivi_Free (vi, path));
        checkErr( Ivi_Alloc (vi, oldSize + legSize, (ViAddr*)&path));
        sprintf (path + oldSize, "%s->%s", vName1, vName2);
        }
        
    /*
        If channel1 is not directly connected to channel2 (that is, path is 
        still VI_NULL), then we must try each configuration channel that is
        directly connected to channel1 to see if it is connected to channel2.
    */
    for (ndxChannelToTry=1; 
                (path == VI_NULL) && (ndxChannelToTry <= numChannels); 
                            ndxChannelToTry++)
        {
        if (elvpc3a_ChannelsAreDirectlyConnected( ndxChannel1, ndxChannelToTry))
            {
            ViConstString   channelToTry, vNameToTry;
            ViInt32         ndxConnectedToChannel;
    
            checkErr( Ivi_GetNthChannelString (vi, ndxChannelToTry, &channelToTry));
            checkErr( Ivi_GetUserChannelName (vi, channelToTry, &vNameToTry));
            
            checkErr( elvpc3a_GetConfigChannelInfo (vi, channelToTry,
                                                     VI_NULL,
                                                     &ndxConnectedToChannel));
            if (ndxConnectedToChannel == ndxChannel1)
                {
                elvpc3a_CalculatePathLegSize (vName1, vNameToTry, &legSize);
                checkErr( elvpc3a_GetExistingPathRecursively (vi, ndxChannelToTry, ndxChannel2, 
                                                               connections, numChannels, 
                                                               &path, 
                                                               oldSize + legSize));
                                                            
                if (path != VI_NULL)
                    {
                    ViInt32 numBytesAdded;

                    numBytesAdded = sprintf (path+oldSize, "%s->%s", vName1, vNameToTry);
                    path[oldSize+numBytesAdded] = ',';
                    goto Error;     /* we have found the path !!! */
                    }
                }
            }
        }

Error:
    if (path != VI_NULL)
        *pathRef = path;
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_IsPathSupported
 *  Purpose:  This function verifies that this switch module can connect 
 *            channels with indices ndxChannel1 and ndxChannel2.  This
 *            function does not determine if the current configuration 
 *            of the module allows for this connection.
 *****************************************************************************/
static ViStatus elvpc3a_IsPathSupported (ViSession vi, ViInt32 ndxChannel1, 
                                          ViInt32 ndxChannel2, 
                                          ViInt32 **connections,
                                          ViInt32 maxDepth,
                                          ViBoolean *pathSupportedRef)
{
    ViStatus        error = VI_SUCCESS;
    ViBoolean       foundSupportedPath = VI_FALSE;
    
    if (elvpc3a_DirectConnectionIsPossible( ndxChannel1, ndxChannel2))
        {
        foundSupportedPath = VI_TRUE;
        }
    else if (maxDepth == 1)   /* 0 or negative means "no maximum" */
        {
        foundSupportedPath = VI_FALSE;
        }
    else
        {
        ViInt32         ndxChannelToTry, numChannels;
        
        checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, 
                                           ELVPC3A_ATTR_CHANNEL_COUNT, 
                                           0, &numChannels));
        for (ndxChannelToTry=1; 
                    !foundSupportedPath && (ndxChannelToTry<=numChannels); 
                            ndxChannelToTry++)
            {
            if (elvpc3a_DirectConnectionIsPossible( ndxChannel1, ndxChannelToTry)
                    && 
                elvpc3a_RecursionOnChannelIsAllowed( ndxChannelToTry))
                {
                ViConstString   channelToTry;
                ViBoolean       isConfigChannel;
                
                checkErr( Ivi_GetNthChannelString (vi, ndxChannelToTry, &channelToTry));
                checkErr( Ivi_GetAttributeViBoolean (vi, channelToTry,
                                                     ELVPC3A_ATTR_IS_CONFIGURATION_CHANNEL,
                                                     0, &isConfigChannel));
                if (isConfigChannel)
                    {
                    ViStatus tempStatus;
                    
                    elvpc3a_PreventRecursionOnChannel( ndxChannelToTry);
                    tempStatus = elvpc3a_IsPathSupported (vi, ndxChannelToTry, 
                                                           ndxChannel2, 
                                                           connections,
                                                           maxDepth-1, 
                                                           &foundSupportedPath);
                    elvpc3a_AllowRecursionOnChannel( ndxChannelToTry);
                    viCheckErr(tempStatus);
                    }
                }
            } 
        }
        
Error:
    *pathSupportedRef = foundSupportedPath;
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_IsPathAvailable
 *  Purpose:  This function verifies that the switch can connect the channels
 *            with indices ndxChannel1 and ndxChannel2 given the current
 *            configuration of the switch module.
 *            The caller should already verify that channels 1 and 2 are not
 *            configuration channels, that they are not connected to 
 *            different source channels, and that no explicit connection
 *            between the two channels already exists.
 *****************************************************************************/
static ViStatus elvpc3a_IsPathAvailable (ViSession vi, ViInt32 ndxChannel1, 
                                          ViInt32 ndxChannel2, 
                                          ViInt32 **connections,
                                          ViInt32 maxDepth,
                                          ViBoolean *pathAvailableRef)
{
    ViStatus        error = VI_SUCCESS;
    ViBoolean       channel1IsConnectedMux, channel2IsConnectedMux;
    ViBoolean       foundAvailablePath = VI_FALSE;
    ViConstString   channel1, channel2;
    
    /* Check if the first channel is a connected mux channel */
    checkErr( Ivi_GetNthChannelString (vi, ndxChannel1, &channel1));
    checkErr( Ivi_GetNthChannelString (vi, ndxChannel2, &channel2));

    /*
        Mux channels can be external channels or configuration channels.
        Mux channels can be connected to only one channel.  The same
        is true for any type of configuration channel, but some 
        non-mux external channels can be connected to multiple channels.
        If the external channels 1 and 2 are mux channels, they must
        not be connected.  
        NOTE:  elvpc3a_IsPathAvailableRecursively makes sure
        that no configuration channels along the path are already 
        connected.  
    */
    checkErr( elvpc3a_IsConnectedMuxChannel (vi, channel1, 
                                              &channel1IsConnectedMux));
    checkErr( elvpc3a_IsConnectedMuxChannel (vi, channel2, 
                                              &channel2IsConnectedMux));
    
    if (!channel1IsConnectedMux && !channel2IsConnectedMux)
        {  
        ViInt32 numChannels;                   
        ViInt32 **connections;
        
        checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, 
                                               ELVPC3A_ATTR_CHANNEL_COUNT, 
                                               0, &numChannels));
                                               
        checkErr( Ivi_GetAttributeViAddr (vi, VI_NULL, ELVPC3A_ATTR_CHANNEL_MATRIX,
                                          0, (ViAddr*)&connections));
                                          
        checkErr( elvpc3a_IsPathAvailableRecursively (vi, ndxChannel1,
                                                       ndxChannel2, 
                                                       connections, 
                                                       numChannels,
                                                       maxDepth,
                                                       &foundAvailablePath));
        }
            
Error:
    *pathAvailableRef = foundAvailablePath;
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_IsPathAvailableRecursively
 *  Purpose:  This function verifies that the switch can connect the channels
 *            with indices ndxChannel1 and ndxChannel2 given the current
 *            configuration of the switch module.
 *            The caller should already verify that neither channels 1 nor 2 
 *            is connected configuration channel or a connected mux common 
 *            channel.
 *            Each invocation of this function checks one connection step
 *            and calls itself recursively to checks additional steps if 
 *            necessary.
 *****************************************************************************/
static ViStatus elvpc3a_IsPathAvailableRecursively (ViSession vi, ViInt32 ndxChannel1, 
                                          ViInt32 ndxChannel2, 
                                          ViInt32 **connections,
                                          ViInt32 numChannels,
                                          ViInt32 maxDepth,
                                          ViBoolean *pathAvailableRef)
{
    ViStatus        error = VI_SUCCESS;
    ViInt32         ndxChannelToTry;
    ViBoolean       foundAvailablePath = VI_FALSE;
    
    if (elvpc3a_DirectConnectionIsPossible( ndxChannel1, ndxChannel2))
        {
        foundAvailablePath = VI_TRUE;
        }

    else if (maxDepth == 1)             /* Since the channels cannot be directly */
        {                               /* connected, we cannot find a path with */
        foundAvailablePath = VI_FALSE;  /* only 1 step. (Remember that there is  */
        }                               /* no maximum, if maxDepth <= 0.)  */
        
    else    /* use recursion to find rest of path */                                  
        {
        for (ndxChannelToTry=1; 
                    !foundAvailablePath && (ndxChannelToTry <= numChannels); 
                            ndxChannelToTry++)
            {
            if (elvpc3a_DirectConnectionIsPossible( ndxChannel1, ndxChannelToTry)
                    && 
                elvpc3a_RecursionOnChannelIsAllowed( ndxChannelToTry))
                {
                ViConstString   channelToTry;
                ViBoolean       isUnconnectedConfigChannel;

                checkErr( Ivi_GetNthChannelString (vi, ndxChannelToTry, &channelToTry));
                
                checkErr( elvpc3a_IsUnconnectedConfigChannel (vi, channelToTry,
                                                             &isUnconnectedConfigChannel));
                                                             
                if (isUnconnectedConfigChannel)
                    {
                    ViStatus tempStatus;
                    
                    elvpc3a_PreventRecursionOnChannel( ndxChannelToTry);
                    tempStatus = elvpc3a_IsPathAvailableRecursively (vi, ndxChannelToTry, 
                                                                      ndxChannel2, 
                                                                      connections, 
                                                                      numChannels,
                                                                      maxDepth - 1,
                                                                      &foundAvailablePath);
                    elvpc3a_AllowRecursionOnChannel( ndxChannelToTry);
                    viCheckErr(tempStatus);
                    }
                }
            }
        }        
        
Error:
    *pathAvailableRef = foundAvailablePath;
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_VerifyPath
 *  Purpose:  This function verifies that the switch module can close all the
 *            connections in an explicit path.  
 *            The caller must have removed whitespaces and trailing commas.
 *            The function returns the following errors:
 *            - ELVPC3A_ERROR_INVALID_SWITCH_PATH if the syntax of the path 
 *              string is not valid.
 *            - ELVPC3A_ERROR_EXPLICIT_CONNECTION_EXISTS, if a path already 
 *              exists between the two channels.
 *            - ELVPC3A_ERROR_ATTEMPT_TO_CONNECT_SOURCES, if setting this path
 *              would connect two sources;
 *            - ELVPC3A_ERROR_IS_CONFIGURATION_CHANNEL, if either of the channels
 *              at the ends of the path is a configuration channel.
 *            - ELVPC3A_ERROR_RSRC_IN_USE, if any of the channels in the path
 *              are already connected.
 *            - ELVPC3A_ERROR_CHANNELS_ALREADY_CONNECTED, if any of the 
 *              channels in the path are already connected.
 *****************************************************************************/
static ViStatus elvpc3a_VerifyPath (ViSession vi, ViConstString path)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      firstChannel[BUFFER_SIZE], lastChannel[BUFFER_SIZE];
    ViChar      elabBuf[IVI_MAX_MESSAGE_BUF_SIZE];
    ViInt32     ndxFirst, ndxLast;
    ViInt32     **connections;
    
    checkErr( Ivi_GetAttributeViAddr (vi, VI_NULL,  
                                      ELVPC3A_ATTR_CHANNEL_MATRIX,
                                      0, (ViAddr*)&connections));
    checkErr( elvpc3a_GetFirstAndLastChannelInPath (vi, path,
                                                     firstChannel, 
                                                     lastChannel));

    checkErr( Ivi_GetChannelIndex (vi, firstChannel, &ndxFirst));
    checkErr( Ivi_GetChannelIndex (vi, lastChannel, &ndxLast));

    if (elvpc3a_ChannelsAreExplicitlyConnected( ndxFirst, ndxLast))
        {
        sprintf (elabBuf, "Channels %.80s and %.80s", firstChannel, 
                 lastChannel);
        viCheckErrElab( ELVPC3A_ERROR_EXPLICIT_CONNECTION_EXISTS, elabBuf);
        }
        
    if (ndxFirst == ndxLast)
        viCheckErrElab( ELVPC3A_ERROR_CANNOT_CONNECT_TO_ITSELF, firstChannel);
    
    checkErr( elvpc3a_VerifySourceChannelConflict (vi, firstChannel, 
                                                    lastChannel));
    checkErr( elvpc3a_VerifyIsEitherAConfigChannel (vi, firstChannel, 
                                                     lastChannel));
    checkErr( elvpc3a_VerifyNotConnectedMux (vi, firstChannel));
    checkErr( elvpc3a_VerifyNotConnectedMux (vi, lastChannel));
    checkErr( elvpc3a_VerifyPathRecursively (vi, path, 0, 0, 
                                              connections));
        
Error:
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_VerifyNotConnectedMux
 *  Purpose:  This function verifies that the common multiplexer channel
 *            is not marked as connected.
 *            If the channel is indeed a mux channel that is marked as 
 *            connected, the function returns the primary error
 *            ELVPC3A_ERROR_INVALID_SWITCH_PATH, and sets the
 *            secondary error to ELVPC3A_ERROR_RSRC_IN_USE.
 *****************************************************************************/
static ViStatus elvpc3a_VerifyNotConnectedMux (ViSession vi, 
                                                ViConstString channel)
{
    ViStatus    error = VI_SUCCESS;
    ViStatus    secondaryError = VI_SUCCESS;
    ViChar      elabBuf[IVI_MAX_MESSAGE_BUF_SIZE];
    ViString    errorElaboration = VI_NULL;
    ViBoolean   isConnectedMuxChannel = VI_FALSE;

    checkErr( elvpc3a_IsConnectedMuxChannel (vi, channel, 
                                              &isConnectedMuxChannel));
    if (isConnectedMuxChannel)
        {
        secondaryError = ELVPC3A_ERROR_RSRC_IN_USE;
        sprintf (elabBuf, "%.80s", channel);
        errorElaboration = elabBuf;
        viCheckErr( ELVPC3A_ERROR_INVALID_SWITCH_PATH);
        }
Error:
    if (error < VI_SUCCESS)
        Ivi_SetErrorInfo (vi, VI_FALSE, error, secondaryError, errorElaboration);
    return error;
}
        
/*****************************************************************************
 *  Function: elvpc3a_VerifyPathRecursively
 *  Purpose:  This function verifies that the module can close all the
 *            connections in the path before attempting to close the switches
 *            on the instrument.
 *            The caller must have removed whitespaces and trailing commas from
 *            the path string.  The caller must validate the path syntax and 
 *            each channel name in the path.
 *****************************************************************************/
static ViStatus elvpc3a_VerifyPathRecursively (ViSession vi, 
                                                ViConstString path,
                                                ViInt32 offset,
                                                ViInt32 ndxPrevLast,
                                                ViInt32 **connections)
{
    ViStatus    error = VI_SUCCESS;
    ViStatus    secondaryError = VI_SUCCESS;
    ViString    errorElaboration = VI_NULL;
    ViChar      channel1[BUFFER_SIZE], channel2[BUFFER_SIZE];
    ViChar      elabBuf[IVI_MAX_MESSAGE_BUF_SIZE];
    ViInt32     ndxChannel1, ndxChannel2, ndxConnectedToChannel;
    
    ViInt32     legSize, newOffset;
    ViBoolean   isConfigChannel, lastLeg = VI_FALSE;
    
    sscanf (path + offset, "%[^-]->%[^,]", channel1, channel2);
    
    elvpc3a_CalculatePathLegSize (channel1, channel2, &legSize);
    newOffset = offset + legSize - 1;
    if (path[newOffset] == '\0')
        lastLeg = VI_TRUE;
    else
        newOffset++;
        
    Ivi_GetChannelIndex (vi, channel1, &ndxChannel1);   /* already validated */
    Ivi_GetChannelIndex (vi, channel2, &ndxChannel2);   /* already validated */
    
    if (!elvpc3a_RecursionOnChannelIsAllowed( ndxChannel2))
        {
        secondaryError = ELVPC3A_ERROR_CHANNEL_DUPLICATED_IN_PATH;
        errorElaboration = channel2;
        viCheckErr( ELVPC3A_ERROR_INVALID_SWITCH_PATH);
        }
    /*
        Verify that first channel of this leg is the same as 
        the last channel of the previous leg, if any.
    */
    if (ndxPrevLast != 0)
        {
        if (ndxChannel1 != ndxPrevLast)
            {
            secondaryError = ELVPC3A_ERROR_DISCONTINUOUS_PATH;
            sprintf (elabBuf, "Discontinuity before %.80s->%.80s", channel1, channel2);
            errorElaboration = elabBuf;
            viCheckErr( ELVPC3A_ERROR_INVALID_SWITCH_PATH);
            }
        }

    /* 
        Verify that both channels in the leg are not the same,
        that they are not directly connected, and that they
        can be directly connected.
    */
    if (ndxChannel1 == ndxChannel2)
        {
        secondaryError = ELVPC3A_ERROR_CHANNEL_DUPLICATED_IN_LEG;
        errorElaboration = channel1;
        viCheckErr( ELVPC3A_ERROR_INVALID_SWITCH_PATH);
        }

    if (elvpc3a_ChannelsAreDirectlyConnected( ndxChannel1, ndxChannel2))
        {
        secondaryError = ELVPC3A_ERROR_CHANNELS_ALREADY_CONNECTED;
        errorElaboration = elvpc3a_FormatPathLegErrorElab (channel1, channel2,
                                                            elabBuf);
        viCheckErr( ELVPC3A_ERROR_INVALID_SWITCH_PATH);
        }

    if (!elvpc3a_DirectConnectionIsPossible( ndxChannel1, ndxChannel2 ))
        {
        secondaryError = ELVPC3A_ERROR_CANNOT_CONNECT_DIRECTLY;
        errorElaboration = elvpc3a_FormatPathLegErrorElab (channel1, channel2,
                                                            elabBuf);
        viCheckErr( ELVPC3A_ERROR_INVALID_SWITCH_PATH);
        }

    if (!lastLeg)
        {
        checkErr( elvpc3a_GetConfigChannelInfo (vi, channel2,
                                                 &isConfigChannel,
                                                 &ndxConnectedToChannel));
        if (!isConfigChannel)
            {
            secondaryError = ELVPC3A_ERROR_NOT_A_CONFIGURATION_CHANNEL;
            errorElaboration = channel2;
            viCheckErr( ELVPC3A_ERROR_INVALID_SWITCH_PATH);
            }
        if (ndxConnectedToChannel != ELVPC3A_VAL_NONE)
            {
            secondaryError = ELVPC3A_ERROR_RSRC_IN_USE;
            errorElaboration = channel2;
            viCheckErr( ELVPC3A_ERROR_INVALID_SWITCH_PATH);
            }
            
        elvpc3a_PreventRecursionOnChannel( ndxChannel2);
        error =  elvpc3a_VerifyPathRecursively (vi, path, newOffset, 
                                                 ndxChannel2, connections);
        elvpc3a_AllowRecursionOnChannel( ndxChannel2);
        }
        
Error:
    if (error <  VI_SUCCESS)
        Ivi_SetErrorInfo (vi, VI_FALSE, error, secondaryError, errorElaboration);
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_CalculatePathLegSize
 *  Purpose:  Calculates the number of bytes, including the terminating byte,
 *            in the path string portion that represents the connection from
 *            channel1 to channel2.
 *****************************************************************************/
static void elvpc3a_CalculatePathLegSize (ViConstString channel1, 
                                           ViConstString channel2,
                                           ViInt32 *legSize)
{
    /*
        Add 2 for the arrow ('->') and 1 for the ASCII NUL byte or comma
    */
    *legSize = strlen(channel1) + strlen(channel2) + 3;   
}

/*****************************************************************************
 *  Function: elvpc3a_FormatPathLegErrorElab
 *  Purpose:  Formats two channel strings that make up a path leg into an
 *            error elaboration buffer, and returns a pointer to the buffer.
 *****************************************************************************/
static ViString elvpc3a_FormatPathLegErrorElab (ViConstString channel1,
                                                 ViConstString channel2,
                                                 ViChar elabBuf[IVI_MAX_MESSAGE_BUF_SIZE])
{                                                     
     sprintf (elabBuf, "%.80s->%.80s", channel1, channel2);
     return elabBuf;
} 
 
/*****************************************************************************
 *  Function: elvpc3a_CleanPath
 *  Purpose:  This function removes the white spaces from the string
 *            and removes trailing commas.
 *****************************************************************************/
static void elvpc3a_CleanPath (ViChar string[])
{
    ViChar      *p1, *p2;
    ViInt32     i;

    p1 = p2 = string;
    do
        {                    
        if (!isspace (*p1))
            *p2++ = *p1;
        } while (*p1++);
    
    for (i=strlen(string)-1; i >= 0; i--)
        if (string[i] == ',')
            string[i] = '\0';
        else
            break;
}

/*****************************************************************************
 *  Function: elvpc3a_IsConnectedMuxChannel
 *  Purpose:  This function verifies that you are not trying to connect a 
 *            channel to a mux common without previously marking the common
 *            channel as available by attempting to disconnect it.
 *****************************************************************************/
static ViStatus elvpc3a_IsConnectedMuxChannel (ViSession vi, 
                                                ViConstString channel,
                                                ViBoolean *isConnectedMuxChannel)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     ndxCommon;
    ViBoolean   commonChannel, connected = VI_FALSE;
    ViInt32     i, numChannels;
    ViInt32     **connections;
    
    checkErr( Ivi_GetAttributeViAddr (vi, VI_NULL,
                                      ELVPC3A_ATTR_CHANNEL_MATRIX,
                                      0, (ViAddr*)&connections));
    checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, 
                                       ELVPC3A_ATTR_CHANNEL_COUNT,
                                       0, &numChannels));
    checkErr( Ivi_GetAttributeViBoolean (vi, channel,
                                         ELVPC3A_ATTR_IS_MUX_CHANNEL,
                                         0, &commonChannel));
    if (commonChannel)
        {
        checkErr( Ivi_GetChannelIndex (vi, channel, &ndxCommon));
        for (i=1; !connected && (i <= numChannels); i++)
            connected = elvpc3a_ChannelsAreExplicitlyConnected( ndxCommon, i);
        }                

Error:
    *isConnectedMuxChannel = connected;
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_GetFirstAndLastChannelInPath
 *  Purpose:  Extracts the first and last channel strings that appear in a 
 *            path.  The caller must have already removed all white spaces
 *            and trailing commas from the path.
 *            This function returns the fllowing errors:
 *            - ELVPC3A_ERROR_INVALID_SWITCH_PATH if the path string does not
 *              contain a valid path.  
 *            If the function returns this primary error, the following are
 *            the possible secondary errors:
 *            - ELVPC3A_ERROR_EMPTY_SWITCH_PATH, if the path string is empty
 *            - ELVPC3A_ERROR_LEG_MISSING_FIRST_CHANNEL, if a leg in the path
 *              does not contain the first channel
 *            - ELVPC3A_ERROR_LEG_MISSING_SECOND_CHANNEL, if a leg in the path
 *              does not contain 
 *****************************************************************************/
static ViStatus elvpc3a_GetFirstAndLastChannelInPath (ViSession vi, 
                                                       ViConstString path,
                                                       ViChar firstChannel[],
                                                       ViChar lastChannel[])
{
    ViStatus    error = VI_SUCCESS;
    ViStatus    secondaryError = VI_SUCCESS;
    ViString    errorElaboration = VI_NULL;
    ViInt32     numFilled, index, legSize;
    ViChar      channel1[BUFFER_SIZE], channel2[BUFFER_SIZE];
    
    firstChannel[0] = '\0';
    lastChannel[0] = '\0';
    if (path[0] == '\0')
        {
        secondaryError = ELVPC3A_ERROR_EMPTY_SWITCH_PATH;
        viCheckErr( ELVPC3A_ERROR_INVALID_SWITCH_PATH);
        }
        
    index = 0;
    while ((numFilled = sscanf (path+index, "%[^-]->%[^,]", channel1, channel2)) == 2)
        {
        if ((secondaryError = Ivi_CoerceChannelName (vi, channel1, VI_NULL)) < VI_SUCCESS)
            {
            errorElaboration = channel1;
            viCheckErr( ELVPC3A_ERROR_INVALID_SWITCH_PATH);
            }
        if ((secondaryError = Ivi_CoerceChannelName (vi, channel2, VI_NULL)) < VI_SUCCESS)
            {
            errorElaboration = channel2;
            viCheckErr( ELVPC3A_ERROR_INVALID_SWITCH_PATH);
            }

        elvpc3a_CalculatePathLegSize (channel1, channel2, &legSize);
        if (firstChannel[0] == '\0')
            strcpy (firstChannel, channel1);
        strcpy (lastChannel, channel2);
        
        index += (legSize - 1);          /* position at comma or NUL byte */
        if (path[index] == '\0')
            break;
        else
            index++;
        }

        
    if (numFilled == 0)   
        {
        if ((path[index] == '-') || (path[index] == '>') || (path[index] == ','))
            {
            secondaryError = ELVPC3A_ERROR_LEG_MISSING_FIRST_CHANNEL;
            viCheckErr( ELVPC3A_ERROR_INVALID_SWITCH_PATH);
            }
        else
            {
            secondaryError = ELVPC3A_ERROR_LEG_MISSING_SECOND_CHANNEL;
            viCheckErr( ELVPC3A_ERROR_INVALID_SWITCH_PATH);
            }
        }
    else if (numFilled != 2)   /* 1 or -1 */
        {
        secondaryError = ELVPC3A_ERROR_LEG_MISSING_SECOND_CHANNEL;
        viCheckErr( ELVPC3A_ERROR_INVALID_SWITCH_PATH);
        }

Error:
    if (error <  VI_SUCCESS)
        Ivi_SetErrorInfo (vi, VI_FALSE, error, secondaryError, errorElaboration);
    return error;
}

/*****************************************************************************
 *  Function: elvpc3a_ImplicitConnectionExists 
 *  Purpose:  This function determines whether two channels are implicitly
 *            connected.  The function assumes that channels 1 and 2 can
 *            be connected.  The function sets the connectionExists parameter 
 *            to VI_TRUE only if both of the following conditions are met:
 *            (1) channels 1 and 2 are not explicitly connected
 *            (2) channels 1 and 2 are connected as a result of explicit
 *                connections between other channels.
 *****************************************************************************/
static ViStatus elvpc3a_ImplicitConnectionExists (ViSession vi,
                                                   ViConstString channel1,
                                                   ViConstString channel2, 
                                                   ViBoolean *connectionExistsRef)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     ndxSource1, ndxSource2;
    ViBoolean   connectionExists = VI_FALSE;
    
    checkErr( Ivi_GetAttributeViInt32 (vi, channel1, 
                                       ELVPC3A_ATTR_ORIGINAL_SOURCE_CHANNEL_INDEX,
                                       0, &ndxSource1));
    checkErr( Ivi_GetAttributeViInt32 (vi, channel2, 
                                       ELVPC3A_ATTR_ORIGINAL_SOURCE_CHANNEL_INDEX,
                                       0, &ndxSource2));
    /*
        If the source indices for channels 1 and 2 are different, it
        is guaranteed that there is no connection between them.
    */
    if (ndxSource1 == ndxSource2)
        {
        /*
            If channels 1 and 2 are both connected to the same source channel, 
            then they are implicitly connected.
        */
        if (ndxSource1 != ELVPC3A_VAL_NONE)
            connectionExists = VI_TRUE;
        else     /* neither channel connected to a source channel */
            {
            ViInt32 ndxChannel1, numChannels, i;
            ViInt32 **connections;
        
            checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL,
                                               ELVPC3A_ATTR_CHANNEL_COUNT,
                                               0, &numChannels));
            checkErr( Ivi_GetAttributeViAddr (vi, VI_NULL,
                                              ELVPC3A_ATTR_CHANNEL_MATRIX,
                                              0, (ViAddr*)&connections));
            checkErr( Ivi_GetChannelIndex (vi, channel1, &ndxChannel1));

            /*
                Neither channel is connected to a source channel; therefore,
                neither is a source channel.  To see if channels 1 and 2 are 
                connected, mark channel 1 as connected to a source channel
                with index ndxChannel1 (effectively marking channel 1 as
                the source channel), and mark all channels connected to channel
                1 as connected to source with an index ndxChannel1.
                If channel 2, which originally was not connected to a source,
                is marked as connected to a source, then there is an implicit 
                connection between channels 1 and 2.
            */   
            Ivi_SetAttributeViInt32 (vi, channel1,
                                     ELVPC3A_ATTR_ORIGINAL_SOURCE_CHANNEL_INDEX,
                                     0, ndxChannel1);   /* cannot fail */
            error = elvpc3a_MarkOriginalSourceRecursively (vi, ndxChannel1, 
                                                            ndxChannel1,
                                                            numChannels, 
                                                            connections);

            Ivi_GetAttributeViInt32 (vi, channel2, 
                                     ELVPC3A_ATTR_ORIGINAL_SOURCE_CHANNEL_INDEX,
                                     0, &ndxSource2);    /* cannot fail */
            if (ndxSource2 == ndxChannel1)
                connectionExists = VI_TRUE;
                
            /*
                Clean up the switch information on all the channels marked
                as connected to ndxSource1.
            */
            for (i=1; i <= numChannels; i++)
                {
                ViString    iChannel;

                Ivi_GetNthChannelString (vi, i, &iChannel);
                Ivi_GetAttributeViInt32 (vi, iChannel,
                                         ELVPC3A_ATTR_ORIGINAL_SOURCE_CHANNEL_INDEX,
                                         0, &ndxSource1);
                if (ndxSource1 == ndxChannel1)
                    {
                    Ivi_SetAttributeViInt32 (vi, iChannel,
                                             ELVPC3A_ATTR_ORIGINAL_SOURCE_CHANNEL_INDEX,
                                             0, ELVPC3A_VAL_NONE);
                    }
                }
            viCheckErr( error);
            }
        }
        
Error:
    *connectionExistsRef = connectionExists;
    return error;
}


/*****************************************************************************
 * Function: elvpc3a_IviInit                                                       
 * Purpose:  This function is called by elvpc3a_InitWithOptions  
 *           or by an IVI class driver.  This function initializes the I/O 
 *           interface, optionally resets the device, optionally performs an
 *           ID query, and sends a default setup to the instrument.                
 *****************************************************************************/
static ViStatus elvpc3a_IviInit (ViRsrc resourceName, ViBoolean IDQuery,
                                    ViBoolean reset, ViSession vi)
{
    ViStatus    error = VI_SUCCESS;
	TaskHandle	th = 0;
	ViInt32		port  = 0;
	ViChar		buffer[BUFFER_SIZE] = "";
	ViChar*		pend = NULL;
    
	checkErr( Ivi_GetAttributeViString (vi, VI_NULL, IVI_ATTR_DRIVER_SETUP, 0, sizeof(buffer), buffer));
	if (( pend = strstr(buffer, "port:")) != NULL )
	{
		pend += strlen("port:");
		for( ; pend && *pend && isspace( *pend); pend++) ;
		port = atoi(pend);
	}
	
	checkErr( Ivi_BuildChannelTable (vi, CHANNEL_LIST, VI_FALSE, VI_NULL));

     /* Add attributes */
    checkErr( elvpc3a_InitAttributes (vi));

    if (!Ivi_Simulating(vi))
	{
        ViSession   rmSession = VI_NULL;
		int			i;
		char		task_list[4096] = "";
		char*		pend = task_list;

        /* Open instrument session */
        checkErr( Ivi_GetAttributeViSession (vi, VI_NULL, IVI_ATTR_VISA_RM_SESSION, 0,
                                             &rmSession)); 
        
		checkDAQmx( DAQmxCreateTask( "PowerVPCBoard", &th));
		for( i = 0; gs_RelTable[i].channel1; i++)
		{
			pend += sprintf( pend, "%s/%s,", resourceName, (port) ? gs_RelTable[i].port1 : gs_RelTable[i].port0);
		}
		*--pend = '\0';
		checkDAQmx( DAQmxCreateDOChan (th, task_list, "", DAQmx_Val_ChanPerLine));
		checkDAQmx( DAQmxStartTask(th));
		
        /* io session owned by driver now */
        checkErr( Ivi_SetAttributeViSession (vi, VI_NULL, IVI_ATTR_IO_SESSION, 0, th));  

	}
        
    /*- Reset instrument ----------------------------------------------------*/
    if (reset) 
        checkErr( elvpc3a_reset (vi));
    else  /*- Send Default Instrument Setup ---------------------------------*/
        checkErr( elvpc3a_DefaultInstrSetup (vi));
	
	/*- Identification Query ------------------------------------------------*/

    checkErr( elvpc3a_CheckStatus (vi));
	
	if (reset && error == VI_SUCCESS )
	    viCheckWarn( VI_WARN_NSUP_RESET);

Error:
    if (error < VI_SUCCESS)
        {
        if (!Ivi_Simulating (vi) & th)
            DAQmxClearTask( th);
        }
    return error;
}

/*****************************************************************************
 * Function: elvpc3a_IviClose                                                        
 * Purpose:  This function performs all of the drivers clean-up operations   
 *           except for closing the IVI session.  This function is called by 
 *           elvpc3a_close or by an IVI class driver. 
 *
 *           Note:  This function must close the I/O session and set 
 *           IVI_ATTR_IO_SESSION to 0.
 *****************************************************************************/
static ViStatus elvpc3a_IviClose (ViSession vi)
{
    ViStatus error = VI_SUCCESS;
    TaskHandle th = 0;

        /* Do not lock here.  The caller manages the lock. */

    checkErr( Ivi_GetAttributeViSession (vi, VI_NULL, IVI_ATTR_IO_SESSION, 0, &th));
	if ( th )
	{
		checkErr( elvpc3a_DisconnectAll(vi));
		checkDAQmx( DAQmxStopTask(th));
	}

Error:
    Ivi_SetAttributeViSession (vi, VI_NULL, IVI_ATTR_IO_SESSION, 0, VI_NULL);
    if (th)                                                      
    {
        DAQmxClearTask( th);
	}
    return error;   
}

/*****************************************************************************
 * Function: elvpc3a_CheckStatus                                                 
 * Purpose:  This function checks the status of the instrument to detect 
 *           whether the instrument has encountered an error.  This function  
 *           is called at the end of most exported driver functions.  If the    
 *           instrument reports an error, this function returns      
 *           IVI_ERROR_INSTRUMENT_SPECIFIC.  The user can set the 
 *           ELVPC3A_ATTR_QUERY_INSTRUMENT_STATUS attribute to VI_FALSE to disable this 
 *           check and increase execution speed.                                   
 *
 *           Note:  Call this function only when the session is locked.
 *****************************************************************************/
static ViStatus elvpc3a_CheckStatus (ViSession vi)
{
    ViStatus    error = VI_SUCCESS;

    if (Ivi_QueryInstrStatus (vi) && Ivi_NeedToCheckStatus (vi) && !Ivi_Simulating (vi))
    {
        checkErr( Ivi_SetNeedToCheckStatus (vi, VI_FALSE));
	}
        
Error:
    return error;
}


/*****************************************************************************
 * Function: elvpc3a_DefaultInstrSetup                                               
 * Purpose:  This function sends a default setup to the instrument.  The    
 *           elvpc3a_reset function calls this function.  The 
 *           elvpc3a_IviInit function calls this function when the
 *           user passes VI_FALSE for the reset parameter.  This function is 
 *           useful for configuring settings that other instrument driver 
 *           functions require.    
 *
 *           Note:  Call this function only when the session is locked.
 *****************************************************************************/
static ViStatus elvpc3a_DefaultInstrSetup (ViSession vi)
{
    ViStatus    error = VI_SUCCESS;
	ViConstString	name = VI_NULL;
        
    /* Invalidate all attributes */
    checkErr( Ivi_InvalidateAllAttributes (vi));
    
    {
    ViInt32 numChannels, i, j;
    ViInt32 **table;
	ViInt32 *counter;    

    checkErr( Ivi_GetAttributeViInt32 (vi, VI_NULL, 
                                       ELVPC3A_ATTR_CHANNEL_COUNT, 
                                       0, &numChannels));
	

    checkErr( Ivi_GetAttributeViAddr (vi, VI_NULL, 
                                      ELVPC3A_ATTR_RELAY_COUNTER, 
                                      0, (void*)&counter));
	if ( counter == VI_NULL )
	{
		checkErr( Ivi_Alloc (vi, (numChannels + 1)*sizeof(ViInt32), (ViAddr*)&counter));	
		memset(counter, 0, (numChannels + 1)*sizeof(ViInt32));
    	checkErr( Ivi_SetAttributeViAddr(vi, VI_NULL, 
                                      		 ELVPC3A_ATTR_RELAY_COUNTER, 
                                      		0, counter));
		
	}
    
	checkErr( Ivi_GetAttributeViAddr (vi, VI_NULL, 
                                      ELVPC3A_ATTR_CHANNEL_MATRIX, 
                                      0, (void**)&table));
    if (table != VI_NULL)
        {
        for (i=1; i<=numChannels; i++)
            {
            checkErr( Ivi_Free (vi, table[i]));
            }
        checkErr( Ivi_Free (vi, table));
        }
    checkErr( Ivi_Alloc (vi, (numChannels + 1)*sizeof(*table), (ViAddr*)&table));
    for (i=1; i<=numChannels; i++)
        checkErr( Ivi_Alloc (vi, (numChannels + 1)*sizeof(ViInt32), (ViAddr*)&table[i]));

	for (i=1; i<=numChannels; i++)
	{
        for (j=1; j<=numChannels; j++)
        {
            if ( (i%2 == 1) && j == (i+1))
			{
#if 0
				checkErr( Ivi_GetNthChannelString (vi, j, &name));
				table[i][j] = ( strlen(name) == 3) ? (ELVPC3A_VAL_CHANNELS_EXPLICITLY_CONNECTED | ELVPC3A_VAL_CHANNELS_DIRECTLY_CONNECTED) : 0;
#else
				table[i][j] = 0;
#endif
			}
			else
			if ( (i%2 == 0) && j == (i-1))
			{
#if 0
				checkErr( Ivi_GetNthChannelString (vi, j, &name));
				table[i][j] = ( strlen(name) == 3) ? (ELVPC3A_VAL_CHANNELS_EXPLICITLY_CONNECTED | ELVPC3A_VAL_CHANNELS_DIRECTLY_CONNECTED ) : 0;
#else
				table[i][j] = 0;
#endif
			}
			else
				table[i][j] = ELVPC3A_VAL_DIRECT_CONNECTION_IMPOSSIBLE;
        }
	}

	
    /*=CHANGE:===============================================================*
        Modify this example code to create the matrix structure that reflects
        the structure of your instrument. This square matrix has one row and 
        one column per channel.  Populate this matrix in such a way so that 
        you mark what direct connections are physically possible on your 
        instrument.
        
        For example, if there is a switch that you can open or close to 
        connect channels R3 and C4, and these channels have indices 3 and 8,
        then put a zero in the table[3][8] and table[8][3] locations.
        If, however, you cannot connect channels C2 and C3 by closing a 
        single switch on the instrument, then you should put a
        ELVPC3A_VAL_DIRECT_CONNECTION_IMPOSSIBLE in locations
        table[5][6] and table[6][5].
        Table rows and columns are 1-based.
        
        If you have a matrix switch with rows R1, R2, R3, and R4, and columns
        C1, C2, C3, and C4, you need to populate your connections table like this:
        
                table[i][j]:
                
                1A 1B 2A 2B 3A 3B 4A 4B
               -------------------------
           1A | x  0  x  x  x  x  x  x  1
           1B | 0  x  x  x  x  x  x  x  2
           2A | x  x  x  0  x  x  x  x  3
           2B | x  x  0  x  x  x  x  x  4
           3A | x  x  x  x  x  0  x  x  5
           3B | x  x  x  x  0  x  x  x  6
           4A | x  x  x  x  x  x  x  0  7
           4B | x  x  x  x  x  x  0  x  8
    
    

     
     *============================================================END=CHANGE=*/   

    checkErr( Ivi_SetAttributeViAddr( vi, VI_NULL, 
                                      ELVPC3A_ATTR_CHANNEL_MATRIX, 
                                      0, table));
    }
    checkErr( Ivi_SetAttributeViInt32( vi, VI_NULL, 
                                      ELVPC3A_ATTR_RELAYS, 
                                      0, 0));
Error:
    return error;
}

/*****************************************************************************
 *----------------- Attribute Range Tables and Callbacks --------------------*
 *****************************************************************************/
    
/*- ELVPC3A_ATTR_SPECIFIC_DRIVER_REVISION -*/

static ViStatus _VI_FUNC elvpc3aAttrDriverRevision_ReadCallback (ViSession vi, 
                                                                  ViSession io,
                                                                  ViConstString channelName, 
                                                                  ViAttr attributeId,
                                                                  const ViConstString cacheValue)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      driverRevision[256];
    
    
    sprintf (driverRevision, 
             "Driver: elvpc3a %.2f, Compiler: %s %3.2f, "
             "Components: IVIEngine %.2f, VISA-Spec %.2f",
             ELVPC3A_MAJOR_VERSION + ELVPC3A_MINOR_VERSION/1000.0, 
             IVI_COMPILER_NAME, IVI_COMPILER_VER_NUM, 
             IVI_ENGINE_MAJOR_VERSION + IVI_ENGINE_MINOR_VERSION/1000.0, 
             Ivi_ConvertVISAVer(VI_SPEC_VERSION));

    checkErr( Ivi_SetValInStringCallback (vi, attributeId, driverRevision));    
Error:
    return error;
}



/*- ELVPC3A_ATTR_INSTRUMENT_MANUFACTURER -*/

static ViStatus _VI_FUNC elvpc3aAttrInstrumentManufacturer_ReadCallback (ViSession vi, 
                                                                          ViSession io,
                                                                          ViConstString channelName, 
                                                                          ViAttr attributeId,
                                                                          const ViConstString cacheValue)
{
    ViStatus    error = VI_SUCCESS;
  
    checkErr( Ivi_SetValInStringCallback (vi, attributeId, "ELCOM"));
    
Error:
    return error;
}

/*- ELVPC3A_ATTR_INSTRUMENT_MODEL -*/
static ViStatus _VI_FUNC elvpc3aAttrInstrumentModel_ReadCallback (ViSession vi, 
                                                                   ViSession io,
                                                                   ViConstString channelName, 
                                                                   ViAttr attributeId,
                                                                   const ViConstString cacheValue)
{
    ViStatus    error = VI_SUCCESS;
  
    checkErr( Ivi_SetValInStringCallback (vi, attributeId, "VPC Power Switch for Valeo"));
    
Error:
    return error;
}
    

/*- ELVPC3A_ATTR_IS_DEBOUNCED -*/
static ViStatus _VI_FUNC elvpc3aAttrIsDebounced_ReadCallback (ViSession vi,
                                                                   ViSession io,
                                                                   ViConstString channelName,
                                                                   ViAttr attributeId,
                                                                   ViBoolean *value)
{
    ViStatus    error = VI_SUCCESS;
    ViReal64    currentTime, settlingMoment;

    checkErr( Ivi_GetAttributeViReal64 (vi, VI_NULL,
                                        ELVPC3A_ATTR_SETTLING_MOMENT,
                                        0, &settlingMoment));
    currentTime = Timer ();
    *value = (currentTime >= settlingMoment);

Error:
    return error;
}

/*- ELVPC3A_ATTR_RELAYS -*/
static ViStatus _VI_FUNC elvpc3aAttrRelays_WriteCallback(
	ViSession		vi,
	ViSession		io,
	ViConstString	channelName,
	ViAttr			attributeId,
	ViInt32			value
)
{
    ViStatus    error = VI_SUCCESS;
	uInt8		data[64];
	int			i;
	int32		write;
	
	memset( data, 0, sizeof(data));
	for( i =0; i < sizeof(value) * 8; i++)
	{
		data[i] = (value & (1<<i)) ? 1 : 0;	
	}

	checkDAQmx( DAQmxWriteDigitalLines( io, 1, 1, 1.0, DAQmx_Val_GroupByChannel, data, &write, 0));
	
Error:
    return error;
}

/*- ELVPC3A_ATTR_ORIGINAL_SOURCE_CHANNEL_INDEX -*/
static ViStatus _VI_FUNC elvpc3aAttrOriginalSourceChannelIndex_ReadCallback (ViSession vi,
                                                                                      ViSession io,
                                                                                      ViConstString channelName,
                                                                                      ViAttr attributeId,
                                                                                      ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;
    ViBoolean   source;
    ViInt32     index = ELVPC3A_VAL_NONE;
    
    checkErr( Ivi_GetAttributeViBoolean (vi, channelName, 
                                         ELVPC3A_ATTR_IS_SOURCE_CHANNEL, 
                                         0, &source));
    if (source)
        checkErr( Ivi_GetChannelIndex (vi, channelName, &index));

Error:
   *value = index;
    return error;
}


/*****************************************************************************
 * Function: elvpc3a_InitAttributes                                                  
 * Purpose:  This function adds attributes to the IVI session, initializes   
 *           instrument attributes, and sets attribute invalidation          
 *           dependencies.                                                   
 *****************************************************************************/
static ViStatus elvpc3a_InitAttributes (ViSession vi)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     flags = 0;

    checkErr( Ivi_SetAttributeViInt32 (vi, "", ELVPC3A_ATTR_SPECIFIC_DRIVER_MAJOR_VERSION,
                                       0, ELVPC3A_MAJOR_VERSION));
    checkErr( Ivi_SetAttributeViInt32 (vi, "", ELVPC3A_ATTR_SPECIFIC_DRIVER_MINOR_VERSION,
                                       0, ELVPC3A_MINOR_VERSION));
    checkErr( Ivi_SetAttributeViString (vi, "", ELVPC3A_ATTR_IO_SESSION_TYPE,
                                        0, ELVPC3A_IO_SESSION_TYPE));
    checkErr( Ivi_SetAttrReadCallbackViString (vi, ELVPC3A_ATTR_SPECIFIC_DRIVER_REVISION,
                                               elvpc3aAttrDriverRevision_ReadCallback));
    checkErr( Ivi_SetAttributeViInt32 (vi, "", ELVPC3A_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION,
                                       0, ELVPC3A_CLASS_SPEC_MAJOR_VERSION));
    checkErr( Ivi_SetAttributeViInt32 (vi, "", ELVPC3A_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION,
                                       0, ELVPC3A_CLASS_SPEC_MINOR_VERSION));
    checkErr( Ivi_SetAttributeViString (vi, "", ELVPC3A_ATTR_SUPPORTED_INSTRUMENT_MODELS,
                                        0, ELVPC3A_SUPPORTED_INSTRUMENT_MODELS));



	checkErr( Ivi_GetAttributeFlags (vi, ELVPC3A_ATTR_INSTRUMENT_MANUFACTURER, &flags));
	checkErr( Ivi_SetAttributeFlags (vi, ELVPC3A_ATTR_INSTRUMENT_MANUFACTURER, 
	                                 flags | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION));
	checkErr( Ivi_SetAttrReadCallbackViString (vi, ELVPC3A_ATTR_INSTRUMENT_MANUFACTURER,
	                                           elvpc3aAttrInstrumentManufacturer_ReadCallback));

	checkErr( Ivi_GetAttributeFlags (vi, ELVPC3A_ATTR_INSTRUMENT_MODEL, &flags));
	checkErr( Ivi_SetAttributeFlags (vi, ELVPC3A_ATTR_INSTRUMENT_MODEL, 
	                                 flags | IVI_VAL_USE_CALLBACKS_FOR_SIMULATION));
	checkErr( Ivi_SetAttrReadCallbackViString (vi, ELVPC3A_ATTR_INSTRUMENT_MODEL,
	                                           elvpc3aAttrInstrumentModel_ReadCallback));

    checkErr( Ivi_SetAttributeViString (vi, "", ELVPC3A_ATTR_SPECIFIC_DRIVER_VENDOR,
                                        0, ELVPC3A_DRIVER_VENDOR));
    checkErr( Ivi_SetAttributeViString (vi, "", ELVPC3A_ATTR_SPECIFIC_DRIVER_DESCRIPTION,
                                        0, ELVPC3A_DRIVER_DESCRIPTION));
    checkErr( Ivi_SetAttributeViAddr (vi, VI_NULL, IVI_ATTR_OPC_CALLBACK, 0,
                                      VI_NULL));
    checkErr( Ivi_SetAttributeViAddr (vi, VI_NULL, IVI_ATTR_CHECK_STATUS_CALLBACK, 0,
                                      VI_NULL));


    /*=CHANGE:===============================================================*
        Set the group capabilities attribute, Remove the extension groups
        this driver does not support, and add additional code to dynamically
        determine how the presence of specific instrument options may affect
        this attribute.
     *============================================================END=CHANGE=*/
        
	checkErr( Ivi_SetAttributeViString (vi, "", ELVPC3A_ATTR_GROUP_CAPABILITIES, 0,
	                                    "IviSwtchBase"));
    
        /*- Add instrument-specific attributes ------------------------------*/            
	checkErr( Ivi_AddAttributeViBoolean (vi, ELVPC3A_ATTR_IS_SOURCE_CHANNEL,
	                                     "ELVPC3A_ATTR_IS_SOURCE_CHANNEL",
	                                     VI_FALSE, IVI_VAL_MULTI_CHANNEL | IVI_VAL_ALWAYS_CACHE,
	                                     VI_NULL, VI_NULL));
	checkErr( Ivi_AddAttributeViBoolean (vi, ELVPC3A_ATTR_IS_CONFIGURATION_CHANNEL,
	                                     "ELVPC3A_ATTR_IS_CONFIGURATION_CHANNEL",
	                                     VI_FALSE, IVI_VAL_MULTI_CHANNEL | IVI_VAL_ALWAYS_CACHE,
	                                     VI_NULL, VI_NULL));
	checkErr( Ivi_AddAttributeViBoolean (vi, ELVPC3A_ATTR_IS_DEBOUNCED,
	                                     "ELVPC3A_ATTR_IS_DEBOUNCED",
	                                     VI_TRUE, IVI_VAL_NEVER_CACHE | IVI_VAL_NOT_USER_WRITABLE |
	                                     IVI_VAL_USE_CALLBACKS_FOR_SIMULATION,
	                                     elvpc3aAttrIsDebounced_ReadCallback,
	                                     VI_NULL));
	
	/*=CHANGE:====================================================*
	   The following attributes are read-only attributes that
	   reflect the characteristic of an instrument. You can obtain
	   this information from the specification pages of an
	   instrument manual. In the following calls to Ivi_AddAttribute
	   function you must replace the argument for default value with 
	   the value from the instrument manual.
	   
	   If your instrument is capable of changing any of these
	   attributes, generate the read callback for the attribute to 
	   determine the attribute value.
	 *=================================================END=CHANGE=*/
	checkErr( Ivi_AddAttributeViReal64 (vi, ELVPC3A_ATTR_SETTLING_TIME,
	                                    "ELVPC3A_ATTR_SETTLING_TIME",
	                                    0, IVI_VAL_MULTI_CHANNEL|IVI_VAL_NOT_USER_WRITABLE,
	                                    VI_NULL, VI_NULL, VI_NULL, 0));
	checkErr( Ivi_AddAttributeViReal64 (vi, ELVPC3A_ATTR_BANDWIDTH,
	                                    "ELVPC3A_ATTR_BANDWIDTH",
	                                    0, IVI_VAL_NOT_USER_WRITABLE | IVI_VAL_MULTI_CHANNEL,
	                                    VI_NULL, VI_NULL, VI_NULL, 0));
	checkErr( Ivi_AddAttributeViReal64 (vi, ELVPC3A_ATTR_MAX_DC_VOLTAGE,
	                                    "ELVPC3A_ATTR_MAX_DC_VOLTAGE",
	                                    0, IVI_VAL_NOT_USER_WRITABLE | IVI_VAL_MULTI_CHANNEL,
	                                    VI_NULL, VI_NULL, VI_NULL, 0));
	checkErr( Ivi_AddAttributeViReal64 (vi, ELVPC3A_ATTR_MAX_AC_VOLTAGE,
	                                    "ELVPC3A_ATTR_MAX_AC_VOLTAGE",
	                                    0, IVI_VAL_NOT_USER_WRITABLE | IVI_VAL_MULTI_CHANNEL,
	                                    VI_NULL, VI_NULL, VI_NULL, 0));
	checkErr( Ivi_AddAttributeViReal64 (vi, ELVPC3A_ATTR_MAX_SWITCHING_AC_CURRENT,
	                                    "ELVPC3A_ATTR_MAX_SWITCHING_AC_CURRENT",
	                                    0, IVI_VAL_NOT_USER_WRITABLE | IVI_VAL_MULTI_CHANNEL,
	                                    VI_NULL, VI_NULL, VI_NULL, 0));
	checkErr( Ivi_AddAttributeViReal64 (vi, ELVPC3A_ATTR_MAX_SWITCHING_DC_CURRENT,
	                                    "ELVPC3A_ATTR_MAX_SWITCHING_DC_CURRENT",
	                                    0, IVI_VAL_NOT_USER_WRITABLE | IVI_VAL_MULTI_CHANNEL,
	                                    VI_NULL, VI_NULL, VI_NULL, 0));
	checkErr( Ivi_AddAttributeViReal64 (vi, ELVPC3A_ATTR_MAX_CARRY_AC_CURRENT,
	                                    "ELVPC3A_ATTR_MAX_CARRY_AC_CURRENT",
	                                    0, IVI_VAL_NOT_USER_WRITABLE | IVI_VAL_MULTI_CHANNEL,
	                                    VI_NULL, VI_NULL, VI_NULL, 0));
	checkErr( Ivi_AddAttributeViReal64 (vi, ELVPC3A_ATTR_MAX_CARRY_DC_CURRENT,
	                                    "ELVPC3A_ATTR_MAX_CARRY_DC_CURRENT",
	                                    0, IVI_VAL_NOT_USER_WRITABLE | IVI_VAL_MULTI_CHANNEL,
	                                    VI_NULL, VI_NULL, VI_NULL, 0));
	checkErr( Ivi_AddAttributeViReal64 (vi, ELVPC3A_ATTR_MAX_SWITCHING_AC_POWER,
	                                    "ELVPC3A_ATTR_MAX_SWITCHING_AC_POWER",
	                                    0, IVI_VAL_NOT_USER_WRITABLE | IVI_VAL_MULTI_CHANNEL,
	                                    VI_NULL, VI_NULL, VI_NULL, 0));
	checkErr( Ivi_AddAttributeViReal64 (vi, ELVPC3A_ATTR_MAX_SWITCHING_DC_POWER,
	                                    "ELVPC3A_ATTR_MAX_SWITCHING_DC_POWER",
	                                    0, IVI_VAL_NOT_USER_WRITABLE | IVI_VAL_MULTI_CHANNEL,
	                                    VI_NULL, VI_NULL, VI_NULL, 0));
	checkErr( Ivi_AddAttributeViReal64 (vi, ELVPC3A_ATTR_MAX_CARRY_AC_POWER,
	                                    "ELVPC3A_ATTR_MAX_CARRY_AC_POWER",
	                                    0, IVI_VAL_NOT_USER_WRITABLE | IVI_VAL_MULTI_CHANNEL,
	                                    VI_NULL, VI_NULL, VI_NULL, 0));
	checkErr( Ivi_AddAttributeViReal64 (vi, ELVPC3A_ATTR_MAX_CARRY_DC_POWER,
	                                    "ELVPC3A_ATTR_MAX_CARRY_DC_POWER",
	                                    0, IVI_VAL_NOT_USER_WRITABLE | IVI_VAL_MULTI_CHANNEL,
	                                    VI_NULL, VI_NULL, VI_NULL, 0));
	checkErr( Ivi_AddAttributeViReal64 (vi, ELVPC3A_ATTR_CHARACTERISTIC_IMPEDANCE,
	                                    "ELVPC3A_ATTR_CHARACTERISTIC_IMPEDANCE",
	                                    0, IVI_VAL_NOT_USER_WRITABLE | IVI_VAL_MULTI_CHANNEL,
	                                    VI_NULL, VI_NULL, VI_NULL, 0));
	/*=CHANGE:====================================================*
	   The end of read-only attributes that need the change in 
	   the default value argument.
	 *=================================================END=CHANGE=*/
	checkErr( Ivi_AddAttributeViInt32 (vi, ELVPC3A_ATTR_NUM_OF_ROWS,
	                                   "ELVPC3A_ATTR_NUM_OF_ROWS",
	                                   10, IVI_VAL_NOT_USER_WRITABLE,
	                                   VI_NULL,
	                                   VI_NULL, VI_NULL));
	checkErr( Ivi_AddAttributeViInt32 (vi, ELVPC3A_ATTR_NUM_OF_COLUMNS,
	                                   "ELVPC3A_ATTR_NUM_OF_COLUMNS",
	                                   1, IVI_VAL_NOT_USER_WRITABLE,
	                                   VI_NULL,
	                                   VI_NULL, VI_NULL));
	checkErr( Ivi_AddAttributeViInt32 (vi, ELVPC3A_ATTR_WIRE_MODE,
	                                   "ELVPC3A_ATTR_WIRE_MODE",
	                                   1, IVI_VAL_NOT_USER_WRITABLE | IVI_VAL_MULTI_CHANNEL,
	                                   VI_NULL, VI_NULL, VI_NULL));
	checkErr( Ivi_AddAttributeViInt32 (vi, ELVPC3A_ATTR_ORIGINAL_SOURCE_CHANNEL_INDEX,
	                                   "ELVPC3A_ATTR_ORIGINAL_SOURCE_CHANNEL_INDEX",
	                                   ELVPC3A_VAL_NONE, IVI_VAL_HIDDEN | 
	                                   IVI_VAL_MULTI_CHANNEL | IVI_VAL_ALWAYS_CACHE | 
	                                   IVI_VAL_USE_CALLBACKS_FOR_SIMULATION,
	                                   elvpc3aAttrOriginalSourceChannelIndex_ReadCallback, 
	                                   VI_NULL, VI_NULL));
	checkErr( Ivi_AddAttributeViInt32 (vi, ELVPC3A_ATTR_CONFIGURATION_CHANNEL_INDEX,
	                                   "ELVPC3A_ATTR_CONFIGURATION_CHANNEL_INDEX",
	                                   ELVPC3A_VAL_NONE, IVI_VAL_HIDDEN | 
	                                   IVI_VAL_MULTI_CHANNEL | IVI_VAL_ALWAYS_CACHE | 
	                                   IVI_VAL_USE_CALLBACKS_FOR_SIMULATION,
	                                   VI_NULL, VI_NULL, VI_NULL));
	checkErr( Ivi_AddAttributeViAddr (vi, ELVPC3A_ATTR_CHANNEL_MATRIX,
	                                  "ELVPC3A_ATTR_CHANNEL_MATRIX",
	                                  VI_NULL, IVI_VAL_HIDDEN,
	                                  VI_NULL, VI_NULL));
	/*- ELVPC3A_ATTR_RELAY_COUNTER -*/
	checkErr( Ivi_AddAttributeViAddr (vi, ELVPC3A_ATTR_RELAY_COUNTER,
	                                  "ELVPC3A_ATTR_RELAY_COUNTER",
	                                  VI_NULL, IVI_VAL_HIDDEN,
	                                  VI_NULL, VI_NULL));
	
	checkErr (Ivi_AddAttributeViBoolean (vi, ELVPC3A_ATTR_IS_MUX_CHANNEL,
	                                     "ELVPC3A_ATTR_IS_MUX_CHANNEL", VI_FALSE,
	                                     IVI_VAL_MULTI_CHANNEL | IVI_VAL_HIDDEN,
	                                     VI_NULL, VI_NULL));
	checkErr (Ivi_AddAttributeViInt32 (vi, ELVPC3A_ATTR_MAXIMUM_DEPTH,
	                                   "ELVPC3A_ATTR_MAXIMUM_DEPTH", 0,
	                                   IVI_VAL_HIDDEN, VI_NULL, VI_NULL, VI_NULL));
	checkErr (Ivi_AddAttributeViReal64 (vi, ELVPC3A_ATTR_SETTLING_MOMENT,
	                                    "ELVPC3A_ATTR_SETTLING_MOMENT", 0.0,
	                                    IVI_VAL_HIDDEN, VI_NULL, 
	                                    VI_NULL, VI_NULL, 0));
	
	
    checkErr( Ivi_AddAttributeViInt32 (vi, ELVPC3A_ATTR_OPC_TIMEOUT,
                                       "ELVPC3A_ATTR_OPC_TIMEOUT",
                                       5000, IVI_VAL_HIDDEN | IVI_VAL_DONT_CHECK_STATUS,
                                       VI_NULL, VI_NULL, VI_NULL)); 
    checkErr( Ivi_AddAttributeViInt32 (vi,
                                       ELVPC3A_ATTR_VISA_RM_SESSION,
                                       "ELVPC3A_ATTR_VISA_RM_SESSION",
                                       VI_NULL,
                                       IVI_VAL_HIDDEN | IVI_VAL_DONT_CHECK_STATUS,
                                       VI_NULL, VI_NULL, VI_NULL));
    checkErr( Ivi_AddAttributeViInt32 (vi, ELVPC3A_ATTR_OPC_CALLBACK,
                                       "ELVPC3A_ATTR_OPC_CALLBACK",
                                       VI_NULL,
                                       IVI_VAL_HIDDEN | IVI_VAL_DONT_CHECK_STATUS,
                                       VI_NULL, VI_NULL, VI_NULL));
    checkErr( Ivi_AddAttributeViInt32 (vi,
                                       ELVPC3A_ATTR_CHECK_STATUS_CALLBACK,
                                       "ELVPC3A_ATTR_CHECK_STATUS_CALLBACK",
                                       VI_NULL,
                                       IVI_VAL_HIDDEN | IVI_VAL_DONT_CHECK_STATUS,
                                       VI_NULL, VI_NULL, VI_NULL));
    checkErr( Ivi_AddAttributeViInt32 (vi,
                                       ELVPC3A_ATTR_USER_INTERCHANGE_CHECK_CALLBACK,
                                       "ELVPC3A_ATTR_USER_INTERCHANGE_CHECK_CALLBACK",
                                       VI_NULL,
                                       IVI_VAL_HIDDEN | IVI_VAL_DONT_CHECK_STATUS,
                                       VI_NULL, VI_NULL, VI_NULL));

	checkErr (Ivi_AddAttributeViInt32( vi,
									   ELVPC3A_ATTR_RELAYS,
									   "ELVPC3A_ATTR_RELAYS",
									   0, IVI_VAL_HIDDEN,
									   VI_NULL,
									   elvpc3aAttrRelays_WriteCallback,
									   VI_NULL));

        /*- Setup attribute invalidations -----------------------------------*/            
	/*- ELVPC3A_ATTR_IS_SOURCE_CHANNEL Invalidations -*/
	checkErr( Ivi_AddAttributeInvalidation (vi, ELVPC3A_ATTR_IS_SOURCE_CHANNEL, 
	                                        ELVPC3A_ATTR_ORIGINAL_SOURCE_CHANNEL_INDEX, 
	                                        VI_TRUE));

    /*=CHANGE:===============================================================*
       
       Set attribute dependencies by calling the additional 
       Ivi_AddAttributeInvalidation functions here.  Remove the dependencies
       that do not apply to your instrument by deleting the calls to
       Ivi_AddAttributeInvalidation.
       When you initially add an attribute, it applies to all channels / repeated
       capability instances.  If you want it to apply to only a subset, call the 
       Ivi_RestrictAttrToChannels or Ivi_RestrictAttrToInstances function.

     *============================================================END=CHANGE=*/

Error:
    return error;
}

/*****************************************************************************
 *------------------- End Instrument Driver Source Code ---------------------*
 *****************************************************************************/
