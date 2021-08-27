
#include "powers.h"
#include <cvirte.h>
#include <modules/module.h>
#include <guid.h>

static SPowersPtr		gs_pPowers = NULL;

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
#define __FUNC__ "{Powers}::GetModuleAttribute"
SElExceptionPtr	_FUNCC GetModuleAttribute( uint32_t  attrID, uint32_t size, void* value)
{
	switch ( attrID )
	{
		case MODULE_ATTR_VERSION_STRING:
			strncpy((char*)value, "1.0", size);
			break;
		case MODULE_ATTR_MODULE_ID:
			strncpy((char*)value, MODULE_POWERS, size);
			break;
		case MODULE_ATTR_MODULE_NAME:
			strncpy((char*)value, "Powers Module", size);
			break;
		case MODULE_ATTR_MODULE_DESCRIPTION:
			strncpy((char*)value, "Powers module for handling tester powers", size);
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
#define __FUNC__ "{Powers}::LoadElcomModule"
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

	EXCCHECK( powers_new( &gs_pPowers));

	EXCCHECK( pParam->SetArg( pParam, "PowersModule", ARG_PTR, (void*) gs_pPowers));
	
Error:                       
	*pInstance = gs_pPowers;
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{Powers}::InitializeElcomModule"
SElExceptionPtr	_FUNCC InitializeElcomModule( void* pInstance, const char* configuration, SPluginArgsPtr	pParam)
{
	SElExceptionPtr	pexception = NULL;
	SPowersPtr	p2powers = (SPowersPtr) pInstance;

	EXCCHECK( gs_pPowers->InitPowers(gs_pPowers));

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Powers}::UnloadElcomModule"
SElExceptionPtr	_FUNCC UnloadElcomModule( void* pInstance)
{
	SElExceptionPtr	pexception = NULL;
	SPowersPtr	p2powers = (SPowersPtr) pInstance;

	EXCCHECK( powers_delete(&gs_pPowers));

Error:
	EXCRETHROW( pexception);
}

