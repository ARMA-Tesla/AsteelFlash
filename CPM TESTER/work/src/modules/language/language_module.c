#include <modules/module.h>
#include <eventfactory/EventFactory.h>
#include <utility.h>
#include <guid.h>
#include "lang_support.h"

static SElEventFactoryPtr	gs_EventFactory = NULL;
static SLanguagePtr			gs_pLang;

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
#define __FUNC__ "GetModuleAttribute"
SElExceptionPtr	_FUNCC GetModuleAttribute( uint32_t  attrID, uint32_t size, void* value)
{
	switch ( attrID )
	{
		case MODULE_ATTR_VERSION_STRING:
			strncpy((char*)value, "1.0", size);
			break;
		case MODULE_ATTR_MODULE_ID:
			strncpy((char*)value, MODULE_LANGUAGE, size);
			break;
		case MODULE_ATTR_MODULE_NAME:
			strncpy((char*)value, "Language module", size);
			break;
		case MODULE_ATTR_MODULE_DESCRIPTION:
			strncpy((char*)value, "Interface for multi-language support", size);
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
#define __FUNC__ "{Language}::LoadElcomModule"
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

	EXCCHECK( language_new(&gs_pLang));
	
Error:                       
	*pInstance = gs_pLang;
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{Language}::InitializeElcomModule"
SElExceptionPtr	_FUNCC InitializeElcomModule( void* pInstance, const char* configuration, SPluginArgsPtr	pParam)
{
	return NULL;
}

#undef __FUNC__
#define __FUNC__ "{Language}::UnloadElcomModule"
SElExceptionPtr	_FUNCC UnloadElcomModule( void* pInstance)
{
	int32_t			   	error = 0;
	SElExceptionPtr    	pexception = NULL;

	EXCCHECK( language_delete(&gs_pLang));

Error:
	EXCRETHROW( pexception);   
}