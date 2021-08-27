
#include "printers.h"
#include <cvirte.h>
#include <modules/module.h>
#include <guid.h>

static SPrintersPtr		gs_pPrinters = NULL;

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
#define __FUNC__ "{Printers}::GetModuleAttribute"
SElExceptionPtr	_FUNCC GetModuleAttribute( uint32_t  attrID, uint32_t size, void* value)
{
	switch ( attrID )
	{
		case MODULE_ATTR_VERSION_STRING:
			strncpy((char*)value, "1.0", size);
			break;
		case MODULE_ATTR_MODULE_ID:
			strncpy((char*)value, MODULE_PRINTERS, size);
			break;
		case MODULE_ATTR_MODULE_NAME:
			strncpy((char*)value, "Printers Module", size);
			break;
		case MODULE_ATTR_MODULE_DESCRIPTION:
			strncpy((char*)value, "Printers module for handling tester printers", size);
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
#define __FUNC__ "{Printers}::LoadElcomModule"
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

	EXCCHECK( printers_new( &gs_pPrinters));

	EXCCHECK( pParam->SetArg( pParam, "PrintersModule", ARG_PTR, (void*) gs_pPrinters));
	
Error:                       
	*pInstance = gs_pPrinters;
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{Printers}::InitializeElcomModule"
SElExceptionPtr	_FUNCC InitializeElcomModule( void* pInstance, const char* configuration, SPluginArgsPtr	pParam)
{
	SElExceptionPtr	pexception = NULL;
	SPrintersPtr	p2printers = (SPrintersPtr) pInstance;

	EXCCHECK( gs_pPrinters->InitPrinters(gs_pPrinters));

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Printers}::UnloadElcomModule"
SElExceptionPtr	_FUNCC UnloadElcomModule( void* pInstance)
{
	SElExceptionPtr	pexception = NULL;
	SPrintersPtr	p2printers = (SPrintersPtr) pInstance;

	EXCCHECK( printers_delete(&gs_pPrinters));

Error:
	EXCRETHROW( pexception);
}

