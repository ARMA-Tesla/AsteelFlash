/****************************************************************************
 *                          M O D U L E
 *---------------------------------------------------------------------------
 *    Copyright   2005-2006 ELCOM, a.s.  All Rights Reserved.        
 *---------------------------------------------------------------------------
 *                                                                          
 * Title:       elmodule.h                                                    
 * Purpose:     ELCOM MODULE interface
 ****************************************************************************/

#if !defined(__MODULE_H__)
#define __MODULE_H__

#include <exception/Exception.h>
#include <plugins/el_pluginargs.h>

#if defined(WINDOWS)
	#define MODULE_API		__declspec( dllexport )
#elif defined(UNIX)
	#define MODULE_API		extern "C"
#else
	#define MODULE_API
#endif

#if defined(__cplusplus) || defined(__cplusplus__)
	extern "C" {
#endif

#define MODULE_ATTR_VERSION_STRING            (10UL)
#define MODULE_ATTR_MODULE_ID                 (20UL)
#define MODULE_ATTR_MODULE_NAME               (21UL)
#define MODULE_ATTR_MODULE_DESCRIPTION        (22UL)
#define MODULE_ATTR_REQUIRED_MODULES          (30UL)
#define MODULE_ATTR_BUILD_DATETIME			  (31UL)


#define MODULE_MAX_ID_LENGTH                42  /* TODO: change to less as 39 bytes and fix stability */
#define MODULE_MAX_VERSION_LENGTH           32
#define MODULE_MAX_NAME_LENGTH				64
#define MODULE_MAX_DESCRIPTION_LENGTH		128
#define MODULE_MAX_BUILD_DATETIME_LENGTH	64

MODULE_API SElExceptionPtr	_FUNCC GetModuleAttribute( uint32_t  attrID, uint32_t size, void* value);
MODULE_API SElExceptionPtr	_FUNCC LoadElcomModule( const char* configuration, void** pInstance, SPluginArgsPtr	pParam);
MODULE_API SElExceptionPtr	_FUNCC InitializeElcomModule( void* pInstance, const char* configuration, SPluginArgsPtr	pParam);
MODULE_API SElExceptionPtr	_FUNCC UnloadElcomModule( void* pInstance);

#if defined(__cplusplus) || defined(__cplusplus__)
	}
#endif

#endif  /* __MODULE_H__ */
