/*!
 * Log File adapter for all subsystems.
 * 
 * Copyright (c) 2005 by Zdenek Rykala, ELCOM, a.s.
 */
#include <eltypes.h>

#if !defined(__TESTER_LOG_ADAPTER_H__)
#define __TESTER_LOG_ADAPTER_H__

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/*****************************************************************************
 * SYSTEM LOG CALLBACK *******************************************************
 *****************************************************************************/
 int32_t _FUNCC TesterSystemLogCallback( uint32_t  priority, const char* ident, const char* message);

/******************************************************
 * System Callbacks
 ******************************************************/
int32_t _FUNCC TesterKOSULog(uint32_t  priority, const char* message);
int32_t _FUNCC TesteriButtonLog(uint32_t  priority, const char* message);
int32_t _FUNCC TesterPrinterLog(uint32_t  priority, const char* message);
int32_t _FUNCC TesterReaderLog(uint32_t  priority, const char* message);
int32_t _FUNCC TesterOPCLog(uint32_t  priority, const char* message);
int32_t _FUNCC TesterFrameworkLog(uint32_t  priority, const char* message);
int32_t _FUNCC TesterStorageLog(uint32_t  priority, const char* message);
int32_t _FUNCC TesterVLineLog(uint32_t  priority, const char* message);
int32_t _FUNCC TesterCANLog(uint32_t  priority, const char* message);
int32_t _FUNCC TesterVANLog(uint32_t  priority, const char* message);


#if defined(__cplusplus) || defined(__cplusplus__)
	}
#endif

#endif /* __TESTER_LOG_ADAPTER_H__ */
