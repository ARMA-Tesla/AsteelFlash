#include <userint.h>
#include <cvidef.h>

#ifndef DAQMXIOCTRL_HEADER
#define DAQMXIOCTRL_HEADER

#ifdef __cplusplus
	extern "C" {
#endif

#ifndef kOutOfMemory
#define kOutOfMemory                        12
#endif

#ifdef _CVI_
#pragma EnableLibraryRuntimeChecking
#endif // _CVI_

#ifndef errChk
#define errChk(functionCall) if( (error=(functionCall))<0 ) goto Error; else
#endif

#ifndef nullChk
#define nullChk(variable) if( (variable)==NULL ) {error=-kOutOfMemory; goto Error;} else
#endif

typedef enum /* Error codes from -10001 to -10006 */
	{
	NIDAQmx_IOCtrl_FailedToLoadDLL			= -10001,
	NIDAQmx_IOCtrl_CouldNotFindFunction		= -10002,
	NIDAQmx_IOCtrl_IncorrectCtrlType		= -10003,
	NIDAQmx_IOCtrl_InvalidAttribute			= -10004,
	NIDAQmx_IOCtrl_NULLpassed				= -10005,
	NIDAQmx_IOCtrl_CannotReadFromTarget		= -10006,
	} NIDAQmx_IOCtrl_Error;


// NIDAQmx I/O Control Attributes
#define NIDAQmx_IOCtrl_ChanType						1
#define NIDAQmx_IOCtrl_Limit_To_Device				2
#define NIDAQmx_IOCtrl_Limit_Device					3
#define NIDAQmx_IOCtrl_Limit_Device_Length			4
#define NIDAQmx_IOCtrl_PhysChanExternal				5
#define NIDAQmx_IOCtrl_PhysChanInternal				6
#define NIDAQmx_IOCtrl_TerminalAdvanced				7
#define NIDAQmx_IOCtrl_Target						8
#define NIDAQmx_IOCtrl_Target_Length				9
#define NIDAQmx_IOCtrl_Visible						100
#define NIDAQmx_IOCtrl_Dimmed						101


// NIDAQmx I/O Control Attribute Values

// Values for NIDAQmx_IOCtrl_ChanType for Global Channels
#define NIDAQmx_IOCtrl_Val_VirtualChanIOTypeAI		(1<<0)
#define NIDAQmx_IOCtrl_Val_VirtualChanIOTypeAO		(1<<1)
#define NIDAQmx_IOCtrl_Val_VirtualChanIOTypeDI		(1<<2)
#define NIDAQmx_IOCtrl_Val_VirtualChanIOTypeDO		(1<<3)
#define NIDAQmx_IOCtrl_Val_VirtualChanIOTypeCI		(1<<4)
#define NIDAQmx_IOCtrl_Val_VirtualChanIOTypeCO		(1<<5)

// Values for NIDAQmx_IOCtrl_ChanType for Physical Channels
#define NIDAQmx_IOCtrl_Val_PhysChanIOTypeAI			(1<<0)
#define NIDAQmx_IOCtrl_Val_PhysChanIOTypeAO			(1<<1)
#define NIDAQmx_IOCtrl_Val_PhysChanIOTypeDI_Line	(1<<2)
#define NIDAQmx_IOCtrl_Val_PhysChanIOTypeDI_Port	(1<<3)
#define NIDAQmx_IOCtrl_Val_PhysChanIOTypeDI			(NIDAQmx_IOCtrl_Val_PhysChanIOTypeDI_Line|NIDAQmx_IOCtrl_Val_PhysChanIOTypeDI_Port)
#define NIDAQmx_IOCtrl_Val_PhysChanIOTypeDO_Line	(1<<4)
#define NIDAQmx_IOCtrl_Val_PhysChanIOTypeDO_Port	(1<<5)
#define NIDAQmx_IOCtrl_Val_PhysChanIOTypeDO			(NIDAQmx_IOCtrl_Val_PhysChanIOTypeDO_Line|NIDAQmx_IOCtrl_Val_PhysChanIOTypeDO_Port)
#define NIDAQmx_IOCtrl_Val_PhysChanIOTypeCI			(1<<6)
#define NIDAQmx_IOCtrl_Val_PhysChanIOTypeCO			(1<<7)



// NIDAQmx I/O Control Functions
int CVIFUNC NIDAQmx_NewTaskCtrl               (int panel, int ctrl, int multi_select);
int CVIFUNC NIDAQmx_NewChannelCtrl            (int panel, int ctrl, int multi_select);
int CVIFUNC NIDAQmx_NewChannelAICtrl          (int panel, int ctrl, int multi_select);
int CVIFUNC NIDAQmx_NewChannelAOCtrl          (int panel, int ctrl, int multi_select);
int CVIFUNC NIDAQmx_NewChannelDICtrl          (int panel, int ctrl, int multi_select);
int CVIFUNC NIDAQmx_NewChannelDOCtrl          (int panel, int ctrl, int multi_select);
int CVIFUNC NIDAQmx_NewChannelCICtrl          (int panel, int ctrl, int multi_select);
int CVIFUNC NIDAQmx_NewChannelCOCtrl          (int panel, int ctrl, int multi_select);
int CVIFUNC NIDAQmx_NewScaleCtrl              (int panel, int ctrl, int multi_select);
int CVIFUNC NIDAQmx_NewDeviceCtrl             (int panel, int ctrl, int multi_select);
int CVIFUNC NIDAQmx_NewTerminalCtrl           (int panel, int ctrl, int multi_select);
int CVIFUNC NIDAQmx_NewPhysChanAICtrl         (int panel, int ctrl, int multi_select);
int CVIFUNC NIDAQmx_NewPhysChanAOCtrl         (int panel, int ctrl, int multi_select);
int CVIFUNC NIDAQmx_NewPhysChanDICtrl         (int panel, int ctrl, int multi_select);
int CVIFUNC NIDAQmx_NewPhysChanDILineCtrl     (int panel, int ctrl, int multi_select);
int CVIFUNC NIDAQmx_NewPhysChanDIPortCtrl     (int panel, int ctrl, int multi_select);
int CVIFUNC NIDAQmx_NewPhysChanDOCtrl         (int panel, int ctrl, int multi_select);
int CVIFUNC NIDAQmx_NewPhysChanDOLineCtrl     (int panel, int ctrl, int multi_select);
int CVIFUNC NIDAQmx_NewPhysChanDOPortCtrl     (int panel, int ctrl, int multi_select);
int CVIFUNC NIDAQmx_NewPhysChanCICtrl         (int panel, int ctrl, int multi_select);
int CVIFUNC NIDAQmx_NewPhysChanCOCtrl         (int panel, int ctrl, int multi_select);
int CVIFUNC NIDAQmx_NewSwitchCtrl             (int panel, int ctrl, int multi_select);
int CVIFUNC NIDAQmx_NewRelayCtrl              (int panel, int ctrl, int multi_select);

int CVIFUNC NIDAQmx_RevertIOCtrl              (int panel, int ctrl);
int CVIFUNC NIDAQmx_DiscardIOCtrl             (int panel, int ctrl);

int CVIFUNC_C NIDAQmx_SetChannelCtrlAttribute (int panel, int ctrl, int attribute, ...);
int CVIFUNC   NIDAQmx_GetChannelCtrlAttribute (int panel, int ctrl, int attribute, void *value);
int CVIFUNC_C NIDAQmx_SetTerminalCtrlAttribute(int panel, int ctrl, int attribute, ...);
int CVIFUNC   NIDAQmx_GetTerminalCtrlAttribute(int panel, int ctrl, int attribute, void *value);
int CVIFUNC_C NIDAQmx_SetPhysChanCtrlAttribute(int panel, int ctrl, int attribute, ...);
int CVIFUNC   NIDAQmx_GetPhysChanCtrlAttribute(int panel, int ctrl, int attribute, void *value);
int CVIFUNC_C NIDAQmx_SetSwitchCtrlAttribute  (int panel, int ctrl, int attribute, ...);
int CVIFUNC   NIDAQmx_GetSwitchCtrlAttribute  (int panel, int ctrl, int attribute, void *value);
int CVIFUNC_C NIDAQmx_SetRelayCtrlAttribute   (int panel, int ctrl, int attribute, ...);
int CVIFUNC   NIDAQmx_GetRelayCtrlAttribute   (int panel, int ctrl, int attribute, void *value);
int CVIFUNC_C NIDAQmx_SetTaskCtrlAttribute    (int panel, int ctrl, int attribute, ...);
int CVIFUNC   NIDAQmx_GetTaskCtrlAttribute    (int panel, int ctrl, int attribute, void *value);
int CVIFUNC_C NIDAQmx_SetScaleCtrlAttribute   (int panel, int ctrl, int attribute, ...);
int CVIFUNC   NIDAQmx_GetScaleCtrlAttribute   (int panel, int ctrl, int attribute, void *value);
int CVIFUNC_C NIDAQmx_SetDeviceCtrlAttribute  (int panel, int ctrl, int attribute, ...);
int CVIFUNC   NIDAQmx_GetDeviceCtrlAttribute  (int panel, int ctrl, int attribute, void *value);

#ifdef __cplusplus
	}
#endif

#endif  /* DAQMXIOCTRL_HEADER */

