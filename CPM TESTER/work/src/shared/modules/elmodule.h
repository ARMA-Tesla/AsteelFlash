/****************************************************************************
 *                          M O D U L E
 *---------------------------------------------------------------------------
 *    Copyright   2005-2006 ELCOM, a.s.  All Rights Reserved.        
 *---------------------------------------------------------------------------
 *                                                                          
 * Title:       elmodule.h                                                    
 * Purpose:     ELCOM MODULE interface
 ****************************************************************************/

#if !defined(__EL_MODULE_H__)
#define __EL_MODULE_H__

#include <exception/Exception.h>
#include <objlist/objlist.h>
#include <plugins/el_pluginargs.h>

#if defined(__cplusplus) || defined(__cplusplus__)
	extern "C" {
#endif

SElExceptionPtr LoadAllModules(
	const char*		modulePath,
	SObjListPtr*	pModuleList,
	SPluginArgsPtr	pParam
);

SElExceptionPtr UnLoadAllModules(
	SObjListPtr* pModuleList
);

int32_t GetModulesCount(void);

SElExceptionPtr GetModuleInfo(
	int32_t index,
	uint32_t attribute,
	uint32_t size,
	void* value	
);

SElExceptionPtr SetLogFnc( 
	SElExceptionPtr (*_LogFnc)(const char* message)
);

#define MODULE_ERR_INVALID_VERSION			-11111
#define MODULE_ERR_NOT_VERSION_RANGE		-11112
#define MODULE_ERR_MODULE_STRING_NOT_FOUND	-11113
#define MODULE_ERR_MODULE_DEPENDENCY		-11114

#if defined(__cplusplus) || defined(__cplusplus__)
	}
#endif

#endif  /* __EL_MODULE_H__ */
