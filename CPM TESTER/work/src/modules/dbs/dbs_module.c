#include <windows.h>
#include <stdio.h>
#include <cvirte.h>
#include <modules/module.h>
#include <eventfactory/EventFactory.h>
#include <guid.h>
#include <log/log.h>
#include "dbs.h"
#if defined(_PRODUCT_LINE)
#include <elBase.h>
#include <elInit.h>
#include "line_ctrl.h"
#endif

const char* HISTORY = \
"[2007-11-23] rT 1.1\n"
" * Fixed sql commands for a test statistic.\n"
"[2007-12-04] rT 2.0\n"
" * Created functions for line sequence editing.\n"
"[2008-01-16] rT 2.1\n"
" * Created functions for test moving.\n";

static SElEventFactoryPtr	gs_EventFactory = NULL;
static SDBSPtr 	    		gs_pDbs = NULL;

static SElExceptionPtr RegisterEventFunction(void);
static SElExceptionPtr TerminateStorage(void);

#if defined(_PRODUCT_LINE)
#undef MODULE_LOG
#define MODULE_LOG ""
#endif

int __stdcall DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
		{
		case DLL_PROCESS_ATTACH:
#if (!defined(_CVI_)&&defined(_DEBUG))
			//_CrtSetBreakAlloc(960);
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
			strncpy((char*)value, MODULE_DBS, size);
			break;
		case MODULE_ATTR_MODULE_NAME:
			strncpy((char*)value, "Database module", size);
			break;
		case MODULE_ATTR_MODULE_DESCRIPTION:
			strncpy((char*)value, "Interface for database MySQL", size);
			break;
		case MODULE_ATTR_REQUIRED_MODULES:	
			strncpy((char*)value, MODULE_LOG, size);
			break;
		case MODULE_ATTR_BUILD_DATETIME:
			strncpy((char*)value, __DATE__" "__TIME__, size);
			break;
	}
	return NULL;
}

#undef __FUNC__
#define __FUNC__ "{Database}::LoadElcomModule"
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

	EXCCHECK( pParam->GetArg( pParam,
							  "EventFactory",
							  sizeof(SElEventFactoryPtr),
							  &gs_EventFactory));

	EXCCHECK( dbs_new(&gs_pDbs));

#if defined(_PRODUCT_LINE)
	elexception_log_set(line_ctrl_el_exception_log);
#else
	EXCCHECK( pParam->GetArg( pParam,
							 MODULE_LOG,
							 sizeof(gs_pDbs->LogApi),
							 &(gs_pDbs->LogApi)));
#endif

	EXCCHECK( gs_pDbs->Initialize(gs_pDbs, NULL));

#if defined(_PRODUCT_LINE) || \
	defined(_PRODUCT_LINE_NEW)

#else
	EXCCHECK( RegisterEventFunction());
	//EXCCHECK( TerminateStorage());
#endif

	EXCCHECK( pParam->SetArg( pParam, "DatabaseModule", ARG_PTR, (void*) gs_pDbs));
	
Error:                       
	*pInstance = gs_pDbs;
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{Database}::InitializeElcomModule"
SElExceptionPtr	_FUNCC InitializeElcomModule( void* pInstance, const char* configuration, SPluginArgsPtr	pParam)
{
#if defined(_PRODUCT_LINE)
	/*
	 * 'el...()' routines startup:
	 */
	elStartup();

	/*
	 * Open diagnostic output:
	 */
	line_ctrl_log_startup();
	elLog("%s(): %s: log initialized",__FUNC__,__FILE__);
	elLog("%s(): %s: thread ID: %lu",__FUNC__,__FILE__,GetCurrentThreadId());
#if 0
	elLogFlush(NULL);
#endif
	elexception_log_set(line_ctrl_el_exception_log);
#endif

	return NULL;
}

#undef __FUNC__
#define __FUNC__ "{Database}::UnloadElcomModule"
SElExceptionPtr	_FUNCC UnloadElcomModule( void* pInstance)
{
	SElExceptionPtr	pexception = NULL;

#if defined(_PRODUCT_LINE) || \
	defined(_PRODUCT_LINE_NEW)

#else
	if(gs_pDbs)
	{
		bool_t				shift_open;   
		SDBSStoragePtr		pStorage = gs_pDbs->Storage; 
		
		if(pStorage)
		{
			/* stop actual shift */
			/*
			EXCCHECK( pStorage->IsShiftOpen(pStorage, &shift_open, NULL));
			if(shift_open)
			{
				EXCCHECK(pStorage->StopShift(pStorage));
			}  */
		}
	}
#endif
	EXCCHECK( dbs_delete(&gs_pDbs));
	
Error:
	EXCRETHROW( pexception);   
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Module}::TerminateStorage"
static SElExceptionPtr TerminateStorage( void)
{
	SElExceptionPtr    	pexception = NULL;
	SDBSStoragePtr		pStorage = (SDBSStoragePtr)gs_pDbs->Storage;
	
	EXCCHECK(pStorage->TerminateShift(pStorage));
	EXCCHECK(pStorage->TerminatePause(pStorage));
	EXCCHECK(pStorage->TerminateOperator(pStorage)); 
	
Error:                       
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Module}::ShiftFunctionCallback"
static SElExceptionPtr ShiftFunctionCallback( const char* eventName, void* pData)
{
	SElExceptionPtr    	pexception = NULL;
	SDBSStoragePtr		pStorage = gs_pDbs->Storage;
	bool_t				shift_open;
	EIdShift			Last_shift_id;
	SElEventDataPtr		pdata = NULL;
	char				info[128];
	
	EXCCHECK( pStorage->IsShiftOpen(pStorage, &shift_open, &Last_shift_id));
	
	if(!shift_open) // no opened shift in the database
	{
		if(0==strcmp(eventName, "EVNT_GUI_SHIFT"))
		{
			const char* buffer = ((SElEventDataPtr)pData)->GetStringPtr(pData);
			EIdShift shift_id;

			shift_id = strtoul(buffer, NULL, 10);

			if(shift_id>0 && shift_id<ID_SHIFT_COUNT)
			{
			EXCCHECK( pStorage->StartShift(pStorage, shift_id));
			sprintf(info, "Equipe %d", shift_id);
		
			/* start master part */
			if(gs_EventFactory)
				EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_TEST_SEQUENCE_MASTER", NULL));
			}
			else
			{
			sprintf(info, "none");
			}
		}
		else if(0==strcmp(eventName, "EVNT_GUI_RESET_COUNTER"))
		{
			const char* buffer = ((SElEventDataPtr)pData)->GetStringPtr(pData);
			EIdShift shift_id;

			shift_id = strtoul(buffer, NULL, 10);

			if(shift_id>0 && shift_id<ID_SHIFT_COUNT)
			{
			EXCCHECK( pStorage->StartShift(pStorage, shift_id));
			sprintf(info, "Equipe %d", shift_id);
		
			/* start master part */
			if(gs_EventFactory)
				EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_TEST_SEQUENCE_MASTER", NULL));
			}
			else
			{
			sprintf(info, "none");
			}
		}
	}
	
	else if(shift_open) // opened shift in the database
	{
		if(0==strcmp(eventName, "EVNT_GUI_SHIFT"))
		{
			const char* buffer = ((SElEventDataPtr)pData)->GetStringPtr(pData);
			EIdShift shift_id;

			shift_id = strtoul(buffer, NULL, 10);

			if(shift_id>0 && shift_id<ID_SHIFT_COUNT )
			{
				if (Last_shift_id != shift_id) //selection of another shift
				{
					EXCCHECK(pStorage->StopShift(pStorage));
					EXCCHECK(pStorage->LogoutOperators(pStorage));
					EXCCHECK( pStorage->StartShift(pStorage, shift_id));
					sprintf(info, "Equipe %d", shift_id);
					
					/* start master part */
					if(gs_EventFactory)
						EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_TEST_SEQUENCE_MASTER", NULL));
				}
				else
				{
				sprintf(info, "Equipe %d", shift_id);
				}
			}
			else
			{
			sprintf(info, "none");
			}
		}
	
		else if(0==strcmp(eventName, "EVNT_GUI_RESET_COUNTER"))
		{
			EXCCHECK( pStorage->IsShiftOpen(pStorage, &shift_open, &Last_shift_id));
			EXCCHECK(pStorage->StopShift(pStorage));
			EXCCHECK(pStorage->LogoutOperators(pStorage));
			EXCCHECK( pStorage->StartShift(pStorage, Last_shift_id));
			sprintf(info, "Equipe %d", Last_shift_id);
			/* start master part */
			if(gs_EventFactory)
				EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_TEST_SEQUENCE_MASTER", NULL));
		}
	}
	
	/* update kosu */
	if(gs_EventFactory)
		EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_KOSU_UPDATE", NULL));

	/* updata info panel */
	if(gs_EventFactory)
	{
		char buffer[256];

		EXCCHECK( eleventdata_new(&pdata));	
		sprintf(buffer, "PANEL_MSG_SHIFT:%s", info);
		EXCCHECK( pdata->SetStringPtr(pdata, buffer));
		EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_GUI_INFO_SET", pdata));
	}
	
	
Error:         
	eleventdata_delete(&pdata);
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Module}::PauseFunctionCallback"
static SElExceptionPtr PauseFunctionCallback( const char* eventName, void* pData)
{
	SElExceptionPtr	pexception = NULL;
	SDBSStoragePtr	pStorage = gs_pDbs->Storage;
	bool_t			pause_opened, 
					shift_opened;
	static bool_t	pause_defined = FALSE;
	
	if(0==strcmp(eventName, "EVNT_PAUSE") || /* shortcut CTRL+S */
		0==strcmp(eventName, "EVNT_TESTER_BUTTON_PRESSED_DOWNTIME") )
	{
		EXCCHECK( pStorage->IsShiftOpen(pStorage, &shift_opened, NULL));  
		if(shift_opened)
		{
			EXCCHECK( pStorage->IsPauseOpen(pStorage, &pause_opened));  
			if(!pause_opened)
			{
				EXCCHECK( pStorage->StartPause(pStorage));
				EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_PAUSE_STARTED", NULL));   
			}
			else if(pause_defined)
			{
				EXCCHECK( pStorage->StopPause(pStorage));
				EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_PAUSE_STOPED", NULL)); 
				pause_defined = FALSE;
			}
		}
	}
	else if(0==strcmp(eventName, "EVNT_PAUSE_SELECTED") && pData)
	{
		int 			pause_id;
		const char* 	buffer = ((SElEventDataPtr)pData)->GetStringPtr(pData);  
		
		pause_id = strtol(buffer, NULL, 10);
		
		EXCCHECK( pStorage->IsPauseOpen(pStorage, &pause_opened));  
		if(pause_id && pause_opened)
		{
			EXCCHECK( pStorage->DefinePause(pStorage, pause_id));
			pause_defined = TRUE;
		}
	}
	
	/* update kosu */
	EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_KOSU_UPDATE", NULL));   
	
Error:                       
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Module}::OperatorFunctionCallback"
static SElExceptionPtr OperatorFunctionCallback( const char* eventName, void* pData)
{
	SElExceptionPtr	pexception = NULL;
	SDBSStoragePtr	pStorage = gs_pDbs->Storage;
	uint32_t		user_id = 0;
	bool_t			isLogged, 
					shift_opened; 
	char*			endPointer = NULL;

	if(0==strcmp(eventName, "EVNT_OPERATOR_LOGIN"))
	{
		EXCCHECK( pStorage->IsShiftOpen(pStorage, &shift_opened, NULL));  
		if(shift_opened)
		{
			if(pData)
			{
				const char* buffer = ((SElEventDataPtr)pData)->GetStringPtr(pData);
				
				user_id = strtol(buffer, &endPointer, 10);
				
				if( 0==strcmp(endPointer, "") && user_id>0)
				{
					EXCCHECK( pStorage->IsOperatorLoged(pStorage, user_id, &isLogged));  
					if(!isLogged)
					{
						EXCCHECK( pStorage->LoginOperator(pStorage, user_id));
					}
					else
					{
						EXCCHECK( pStorage->LogoutOperator(pStorage, user_id));
					}
				}
			}
		}
	}
	
	/* update kosu */
	EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_KOSU_UPDATE", NULL));   
	
Error:                       
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Module}::ProductFunctionCallback"
static SElExceptionPtr ProductFunctionCallback( const char* eventName, void* pData)
{
	SElExceptionPtr	pexception = NULL;
	SDBSStoragePtr	pStorage = gs_pDbs->Storage;
	int				id = 0;
	
	if(pData)
	{
		const char* buffer = ((SElEventDataPtr)pData)->GetStringPtr(pData);   
		
		if(strlen(buffer)>0)
		{
			char	data[1024];
			char*	pdata = NULL;
			char*	pdata2 = NULL;
			char*	pdata3 = NULL;
			
			strcpy(data, buffer);
			
			pdata = strtok(data, ";");  /* tester mode */
			pdata2 = strtok(NULL, ";"); /* xml path */
			pdata3 = strtok(NULL, ";"); /* test_result */
			
			if(pdata && 0==strcmp(pdata, "PRODUCTION"))
				id = ID_MODE_PRODUCTION;
			else if(pdata && 0==strcmp(pdata, "QUALITY_RETEST")) 
				id = ID_MODE_QUALITY_RETEST; 
			else if(pdata && 0==strcmp(pdata, "QUALITY_MASTER")) 
				id = ID_MODE_QUALITY_MASTER; 
			else if(pdata && 0==strcmp(pdata, "EXPERTISE_STEP_MODE")) 
				id = ID_MODE_EXPERTISE_STEP_MODE; 
			else if(pdata && 0==strcmp(pdata, "EXPERTISE_GO_MODE")) 
				id = ID_MODE_EXPERTISE_GO_MODE; 
			else if(pdata && 0==strcmp(pdata, "EXPERTISE_SEQUENCE")) 
				id = ID_MODE_EXPERTISE_SEQUENCE; 
			else if(pdata && 0==strcmp(pdata, "MASTERPART")) 
				id = ID_MODE_MASTERPART; 
			else
				id = 0;
			
			if(id)
				EXCCHECK( pStorage->AddProduct(pStorage, pdata2, id));
		}
	}
	
	/* update kosu */
	if(id==ID_MODE_PRODUCTION || id==ID_MODE_QUALITY_RETEST)
		EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_KOSU_UPDATE", NULL));   
	
Error:                       
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Module}::RegisterEventFunction"
static SElExceptionPtr RegisterEventFunction(void)
{
   SElExceptionPtr	pexception = NULL;
   
   if ( gs_EventFactory )
   {
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
		                                   "EVNT_GUI_SHIFT",
		                                   ShiftFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
		                                   "EVNT_TEST_SEQUENCE_END",
		                                   ProductFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
		                                   "EVNT_PAUSE",
		                                   PauseFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
		                                   "EVNT_TESTER_BUTTON_PRESSED_DOWNTIME",
		                                   PauseFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
		                                   "EVNT_PAUSE_SELECTED",
		                                   PauseFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
		                                   "EVNT_OPERATOR_LOGIN",
		                                   OperatorFunctionCallback)); 
   }
   
Error:
   EXCRETHROW(pexception);
}
