#include "extcode.h"
#pragma pack(push)
#pragma pack(1)

#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
	uint32_t BaudRate;
} Cluster;

/*!
 * Performs an instrument error query before terminating the software 
 * connection to the instrument.
 */
int32_t __cdecl BKPrecision9800Series_Close(uintptr_t *VISAResourceName);
/*!
 * Establishes communication with the instrument and optionally performs an 
 * instrument identification query and/or an instrument reset.  It also places 
 * the instrument in a default state needed for other instrument driver 
 * operations.  Therefore, call this VI before calling other instrument driver 
 * VIs for this instrument.  Generally, you need to call the <b>Initialize</b> 
 * VI only once at the beginning of an application.
 */
int32_t __cdecl BKPrecision9800Series_Initialize(uintptr_t *VISAResourceName, 
	LVBoolean IDQueryTrue, LVBoolean ResetTrue, Cluster *SerialConfiguration, 
	uintptr_t *VISAResourceNameOut);
/*!
 * Returns the actual voltage, current, power level, and frequency at the 
 * output.
 */
int32_t __cdecl BKPrecision9800Series_MeasureOutput(
	uintptr_t *VISAResourceName, double *OutputFrequencyHz, 
	double *ActualPowerLevelW, double *ActualCurrentLevelA, 
	double *ActualVoltageLevelV, uintptr_t *VISAResourceNameOut);
/*!
 * Configures AC output voltage level and frequency of the power supply.
 */
int32_t __cdecl BKPrecision9800Series_ConfigureOutput(
	uintptr_t *VISAResourceName, double VoltageLevel00V, double Frequency50Hz, 
	uintptr_t *VISAResourceNameOut);
/*!
 * Configures the output current (RMS) and peak current protection limits and 
 * their respective protection delay mode.
 */
int32_t __cdecl BKPrecision9800Series_ConfigureCurrentProtectionLimits(
	uintptr_t *VISAResourceName, double RMSCurrentLimit3A, 
	double PeakCurrentLimit12A, uint16_t CurrentProtectionDelay0Delay, 
	uint16_t PeakCurrentProtectionDelay0Delay, uintptr_t *VISAResourceNameOut);
/*!
 * Enables or Disables the output.
 */
int32_t __cdecl BKPrecision9800Series_EnableOutput(
	uintptr_t *VISAResourceName, LVBoolean EnableOutputFFalse, 
	uintptr_t *VISAResourceNameOut);

MgErr __cdecl LVDLLStatus(char *errStr, int errStrLen, void *module);

void __cdecl SetExecuteVIsInPrivateExecutionSystem(Bool32 value);

#ifdef __cplusplus
} // extern "C"
#endif

#pragma pack(pop)

