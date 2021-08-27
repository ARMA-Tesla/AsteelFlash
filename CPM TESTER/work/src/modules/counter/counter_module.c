#include <modules/module.h>
#include <counter/counter.h>
#include <eventfactory/EventFactory.h>
#include <utility.h>
#include <guid.h>

static SElEventFactoryPtr	gs_EventFactory = NULL;
static SHWCounterPtr    	gs_pCounter = NULL;

int __stdcall DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
		{
		case DLL_PROCESS_ATTACH:
#if (!defined(_CVI_)&&defined(_DEBUG))
			//_CrtSetBreakAlloc(74);
#endif
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
#define __FUNC__ "GetModuleAttribute"
SElExceptionPtr	_FUNCC GetModuleAttribute( uint32_t  attrID, uint32_t size, void* value)
{
	switch ( attrID )
	{
		case MODULE_ATTR_VERSION_STRING:
			strncpy((char*)value, "2.1", size);
			break;
		case MODULE_ATTR_MODULE_ID:
			strncpy((char*)value, MODULE_COUNTER, size);
			break;
		case MODULE_ATTR_MODULE_NAME:
			strncpy((char*)value, "Counter Module", size);
			break;
		case MODULE_ATTR_MODULE_DESCRIPTION:
			strncpy((char*)value, "Module for card counter communication", size);
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
#define __FUNC__ "{Sensors}::LoadElcomModule"
SElExceptionPtr	_FUNCC LoadElcomModule(
	const char* configuration,
	void** pInstance,
	SPluginArgsPtr	pParam
)
{
	SElExceptionPtr	pexception = NULL;

	EXCCHECK( pParam->GetArg( pParam,
							  "WindowHandle",
							  sizeof(hWnd),
							  &hWnd));

	EXCCHECK( hwcounter_new(&gs_pCounter));
	
Error:                       
	*pInstance = gs_pCounter;
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{Sensors}::InitializeElcomModule"
SElExceptionPtr	_FUNCC InitializeElcomModule( void* pInstance, const char* configuration, SPluginArgsPtr	pParam)
{
	SElExceptionPtr	pexception = NULL;
	char			module_dir[2048] = "";
	int32_t			error = 0;
	
	EXCCHECKCVI( GetModuleDir (__CVIUserHInst, module_dir));
	strcat(module_dir, "\\counter.xml");
	EXCCHECK( gs_pCounter->OpenWithConfiguration( gs_pCounter, module_dir));
	
Error:
	EXCRETHROW( pexception);   
}

#undef __FUNC__
#define __FUNC__ "{Sensors}::UnloadElcomModule"
SElExceptionPtr	_FUNCC UnloadElcomModule( void* pInstance)
{
	SElExceptionPtr    pexception = NULL;        
	
	EXCCHECK( hwcounter_delete(&gs_pCounter));    
	
Error:
	EXCRETHROW( pexception);   
}
