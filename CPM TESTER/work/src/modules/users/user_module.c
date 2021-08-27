
#include "user.h"
#include <modules/module.h>
#include <eventfactory/EventFactory.h>
#include <guid.h>
#include <cvirte.h>

static SElEventFactoryPtr	gs_EventFactory = NULL;
static SUserListPtr 	    gs_pUserList = NULL;

static SElExceptionPtr RegisterEventFunction(void);

static SElExceptionPtr UserLoginCallback( const char* eventName, void* reserved);
static SElExceptionPtr UserLogoutCallback( const char* eventName, void* reserved); 
static SElExceptionPtr UserIbuttonCallback( const char* eventName, void* reserved);

static SElExceptionPtr	callUserLogin(void);
static SElExceptionPtr	callUserLogout(void);
static SElExceptionPtr	callOperatorLog(int32_t user_id);

int __stdcall DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
		{
		case DLL_PROCESS_ATTACH:
#if (!defined(_CVI_)&&defined(_DEBUG))
			//_CrtSetBreakAlloc(61);
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
			strncpy((char*)value, MODULE_USER, size);
			break;
		case MODULE_ATTR_MODULE_NAME:
			strncpy((char*)value, "User login module", size);
			break;
		case MODULE_ATTR_MODULE_DESCRIPTION:
			strncpy((char*)value, "Register logged user", size);
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
#define __FUNC__ "{User}::LoadElcomModule"
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

	EXCCHECK( userlist_new( &gs_pUserList));

	/* set callback functions */
	gs_pUserList->_OperatorLogin = callOperatorLog;
	gs_pUserList->_OperatorLogout = callOperatorLog;
	gs_pUserList->_UserLogin = callUserLogin;
	gs_pUserList->_UserLogout = callUserLogout;
	
	EXCCHECK( pParam->GetArg( pParam,
							 "EventFactory",
							 sizeof(SElEventFactoryPtr),
							 &gs_EventFactory));
							 
	EXCCHECK( RegisterEventFunction());

Error:                       
	*pInstance = gs_pUserList;
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{User}::InitializeElcomModule"
SElExceptionPtr	_FUNCC InitializeElcomModule( void* pInstance, const char* configuration, SPluginArgsPtr	pParam)
{
	return NULL;
}

#undef __FUNC__
#define __FUNC__ "{User}::UnloadElcomModule"
SElExceptionPtr	_FUNCC UnloadElcomModule( void* pInstance)
{
	SElExceptionPtr    pexception = NULL;    
	
	EXCCHECK( userlist_delete( &gs_pUserList)); 
	
Error:  
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "UserLoginCallback"
static SElExceptionPtr UserLoginCallback( const char* eventName, void* pData)
{
	SElExceptionPtr pexception = NULL;
	SElEventDataPtr	pdata = (SElEventDataPtr)pData; 

	if ( gs_pUserList)
	{
		EXCCHECK( gs_pUserList->LoginUser(gs_pUserList, pdata->GetStringPtr(pdata)));			
	}

Error:                       
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "UserLogoutCallback"
static SElExceptionPtr UserLogoutCallback( const char* eventName, void* pData)
{
	SElExceptionPtr pexception = NULL;
	SElEventDataPtr	pdata = (SElEventDataPtr)pData; 

	if ( gs_pUserList)
	{
		EXCCHECK( gs_pUserList->LogoutUser(gs_pUserList));
	}

Error:                       
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "UserIbuttonCallback"
static SElExceptionPtr UserIbuttonCallback( const char* eventName, void* pData)
{
	SElExceptionPtr pexception = NULL;
	SElEventDataPtr	pdata = (SElEventDataPtr)pData; 

	if ( gs_pUserList)
	{
		EXCCHECK( gs_pUserList->LoginUser(gs_pUserList, pdata->GetStringPtr(pdata)));			
	}

Error:                       
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "callUserLogin"
static SElExceptionPtr	callUserLogin(void)
{
	SElExceptionPtr pexception = NULL;

	if( gs_EventFactory)
		EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_USER_LOGIN_VALIDATED", NULL));

Error:                       
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "callUserLogout"
static SElExceptionPtr	callUserLogout(void)
{
	SElExceptionPtr pexception = NULL;
	
	if( gs_EventFactory)
		EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_USER_LOGOUT_VALIDATED", NULL));

Error:                       
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "callOperatorLog"
static SElExceptionPtr	callOperatorLog(int32_t user_id)
{
	SElExceptionPtr pexception = NULL;
	SElEventDataPtr	pdata = NULL;

	if( gs_EventFactory)
	{
		char	buffer[32];

		EXCCHECK( eleventdata_new(&pdata));
		sprintf(buffer, "%d", user_id);
		EXCCHECK( pdata->SetStringPtr(pdata, buffer));

		/* event for module_dbs: storage and kosu */
		EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_OPERATOR_LOGIN", pdata));
	}

Error:              
	eleventdata_delete(&pdata);
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "RegisterEventFunction"
static SElExceptionPtr RegisterEventFunction(void)
{
   SElExceptionPtr   pexception = NULL;
   
  if ( gs_EventFactory )
	{
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory, 
														 "EVNT_USER_LOGIN", 
														 UserLoginCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory, 
														 "EVNT_USER_LOGOUT", 
														 UserLogoutCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory, 
														 "EVNT_USER_IBUTTON", 
														 UserIbuttonCallback));
	}
   
Error:
   EXCRETHROW(pexception);
}   /* RegisterEventFunction */