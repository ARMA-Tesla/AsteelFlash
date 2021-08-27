#include "extcode.h"
#pragma pack(push)
#pragma pack(1)

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Performs an instrument error query before terminating the software 
 * connection to the instrument.
 */
void __cdecl BKPrecisionPVSSeries_Close(uintptr_t *VISAResourceNameOut);
/*!
 * Establishes communication with the instrument and optionally performs an 
 * instrument identification query and/or an instrument reset.  It also places 
 * the instrument in a default state needed for other instrument driver 
 * operations.  Therefore, call this VI before calling other instrument driver 
 * VIs for this instrument.  Generally, you need to call the Initialize VI 
 * only once at the beginning of an application.
 */
int32_t __cdecl BKPrecisionPVSSeries_Initialize(char VISAResourceName[], 
	int32_t IDQueryTrue, int32_t ResetTrue, uint32_t BaudRate, 
	uintptr_t *VISAResourceNameOut);
/*!
 * Enables or Disables the output. 
 */
void __cdecl BKPrecisionPVSSeries_EnableOutput(
	uintptr_t *VISAResourceNameOut, int32_t Address, 
	uint16_t UnitControlOperation, int32_t EnableOutput);
/*!
 * Configures the current limit.  Power supply (power supplies for multi unit 
 * control) will automatically switch to constant current (CC) mode when limit 
 * is reached. CurrentLimit (0.02A (default)), Adress 
 * (1=default),UnitControlOperation (0=Single(default),1=Single Chain, 2=All 
 * Chain), 
 */
void __cdecl BKPrecisionPVSSeries_ConfigureCurrentLimit(
	uintptr_t *VISAResourceNameOut, double CurrentLimit, int32_t Address, 
	uint16_t UnitControlOperation);
/*!
 * Configures over current protection (OCP) of the power supply or power 
 * supplies (multi unit control).  Specify the current limit and whether to 
 * enable or disable OCP. CurrentLimit (5.1A(Default)),, Adress (1=default), 
 * UnitControlOperation(0=single (default), 1=single Chain, 2=Chain All).
 */
void __cdecl BKPrecisionPVSSeries_ConfigureOCP(
	uintptr_t *VISAResourceNameOut, double CurrentLimit, int32_t Address, 
	uint16_t UnitControlOperation, int32_t EnableOCP);
/*!
 * Configures output voltage level, and enables or disables the output of the 
 * power supply or power supplies (multi unit control). Voltage Level 
 * (5.0V=default), Adress(1=default), 
 * UnitControlOperation(0=Single(Default),1=Single Chain, 2= All Chain)
 */
void __cdecl BKPrecisionPVSSeries_ConfigureOutput(
	uintptr_t *VISAResourceNameOut, double VoltageLevel, int32_t Address, 
	uint16_t UnitControlOperation, int32_t EnableOutput);
/*!
 * Configures over voltage protection (OVP) of the power supply or multiple 
 * supplies (multi unit control).  Specify the voltage limit and whether to 
 * enable or disable OVP. VoltageLimit (660V=Default), Adress(1=Default), 
 * UnitControlOperation(0=Single,1=Single Chain,2=All Chain).
 */
void __cdecl BKPrecisionPVSSeries_ConfigureOVP(
	uintptr_t *VISAResourceNameOut, double VoltageLimit, int32_t Address, 
	uint16_t UnitControlOperation, int32_t EnableOVP);

MgErr __cdecl LVDLLStatus(char *errStr, int errStrLen, void *module);

void __cdecl SetExecuteVIsInPrivateExecutionSystem(Bool32 value);

#ifdef __cplusplus
} // extern "C"
#endif

#pragma pack(pop)

