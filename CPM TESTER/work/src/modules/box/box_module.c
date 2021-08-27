#include "msxmldom.h"
#include <modules/module.h>
#include <eventfactory/EventFactory.h>
#include <utility.h>
#include <guid.h>
#include <box/box.h>
#include <ansi_c.h>

static SElEventFactoryPtr	gs_EventFactory = NULL;
static SBoxPtr 	    		gs_pBox = NULL;

static SElExceptionPtr RegisterEventFunction(void);

int __stdcall DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
		{
		case DLL_PROCESS_ATTACH:
#if (!defined(_CVI_)&&defined(_DEBUG))
			//_CrtSetBreakAlloc(78);
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
			strncpy((char*)value, "1.0", size);
			break;
		case MODULE_ATTR_MODULE_ID:
			strncpy((char*)value, MODULE_BOX, size);
			break;
		case MODULE_ATTR_MODULE_NAME:
			strncpy((char*)value, "Box Module", size);
			break;
		case MODULE_ATTR_MODULE_DESCRIPTION:
			strncpy((char*)value, "Module for box control", size);
			break;
		case MODULE_ATTR_REQUIRED_MODULES:	
			strncpy((char*)value, MODULE_TESTER, size);
			break;
		case MODULE_ATTR_BUILD_DATETIME:
			strncpy((char*)value, __DATE__" "__TIME__, size);
			break;
	}
	return NULL;
}

#undef __FUNC__
#define __FUNC__ "{Box}::LoadElcomModule"
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

	EXCCHECK( box_new( &gs_pBox));

Error:                       
	*pInstance = gs_pBox;
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{Box}::InitializeElcomModule"
SElExceptionPtr	_FUNCC InitializeElcomModule( void* pInstance, const char* configuration, SPluginArgsPtr pParam)
{
	SElExceptionPtr	pexception = NULL; 
	int32_t			error = 0;
	char			module_dir[512];
	char			file_name[1024];
	
	EXCCHECK( pParam->GetArg( pParam,
							  "TesterModule",
							  sizeof(gs_pBox->testerAPI),
							  &(gs_pBox->testerAPI)));

	EXCCHECK( pParam->GetArg( pParam,
							 "EventFactory",
							 sizeof(gs_pBox->eventAPI),
							 &(gs_pBox->eventAPI)));

	EXCCHECKCVI( GetModuleDir (__CVIUserHInst, module_dir));
	sprintf(file_name, "%s\\box.xml", module_dir);
	EXCCHECK( gs_pBox->Initialize(gs_pBox, file_name));

	EXCCHECK( gs_pBox->RegisterSensorCallbacks(gs_pBox));

	EXCCHECK( pParam->GetArg( pParam,
							 "EventFactory",
							 sizeof(gs_EventFactory),
							 &gs_EventFactory));

Error:
	EXCRETHROW( pexception);   
}

#undef __FUNC__
#define __FUNC__ "{Box}::UnloadElcomModule"
SElExceptionPtr	_FUNCC UnloadElcomModule( void* pInstance)
{
	SElExceptionPtr    pexception = NULL;        
	
	EXCCHECK( box_delete(&gs_pBox));    
	
Error:
	EXCRETHROW( pexception);   
}
