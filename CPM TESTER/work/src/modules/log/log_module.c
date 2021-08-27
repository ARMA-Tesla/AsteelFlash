#include <modules/module.h>
#include <eventfactory/EventFactory.h>
#include <utility.h>
#include <guid.h>
#include "log_struct.h"

static SLogPtr		gs_pLog = NULL;

int __stdcall DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		if (InitCVIRTE (hinstDLL, 0, 0) == 0)
			return 0;	  /* out of memory */
		break;
	case DLL_PROCESS_DETACH:
		CloseCVIRTE ();
#if !defined(_CVI_) && defined(_DEBUG) && defined(DUMP_MEMORY_LEAKS)
			_CrtDumpMemoryLeaks();
#endif
		break;
	}
	
	return 1;
}


#undef __FUNC__
#define __FUNC__ "{Log}::GetModuleAttribute"
SElExceptionPtr	_FUNCC GetModuleAttribute( uint32_t  attrID, uint32_t size, void* value)
{
	switch ( attrID )
	{
		case MODULE_ATTR_VERSION_STRING:
			strncpy((char*)value, "1.0", size);
			break;
		case MODULE_ATTR_MODULE_ID:
			strncpy((char*)value, MODULE_LOG, size);
			break;
		case MODULE_ATTR_MODULE_NAME:
			strncpy((char*)value, "Log Module", size);
			break;
		case MODULE_ATTR_MODULE_DESCRIPTION:
			strncpy((char*)value, "Module for log messages", size);
			break;
		case MODULE_ATTR_REQUIRED_MODULES:
			strncpy((char*)value, "", size);
			break;
		case MODULE_ATTR_BUILD_DATETIME:
			strncpy((char*)value, __DATE__" "__TIME__, size);
			break;
	}
	return NULL;
}

#undef __FUNC__
#define __FUNC__ "{Log}::LoadElcomModule"
SElExceptionPtr	_FUNCC LoadElcomModule(
	const char* configuration,
	void** pInstance,
	SPluginArgsPtr	pParam
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	char			module_dir[512];
	char			file_name[1024];

	EXCCHECK( pParam->GetArg( pParam,
							  "WindowHandle",
							  sizeof(hWnd),
							  &hWnd));

	EXCCHECK( log_new( &gs_pLog));
	EXCCHECK( pParam->SetArg( pParam, MODULE_LOG, ARG_PTR, (void*) gs_pLog));

	EXCCHECKCVI( GetModuleDir (__CVIUserHInst, module_dir));
	sprintf(file_name, "%s\\log.xml", module_dir);
	EXCCHECK( gs_pLog->Initialize(gs_pLog, file_name));
	
Error:                       
	*pInstance = gs_pLog;
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{Log}::InitializeElcomModule"
SElExceptionPtr	_FUNCC InitializeElcomModule( void* pInstance, const char* configuration, SPluginArgsPtr pParam)
{
	SElExceptionPtr	pexception = NULL;
	SLogPtr			p2log = (SLogPtr) pInstance;

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Log}::UnloadElcomModule"
SElExceptionPtr	_FUNCC UnloadElcomModule( void* pInstance)
{
	SElExceptionPtr	pexception = NULL;
	SLogPtr			p2log = (SLogPtr) pInstance;

	EXCCHECK( log_delete(&gs_pLog));

Error:
	EXCRETHROW( pexception);
}