#include <modules/module.h>
#include <eventfactory/EventFactory.h>
#include <utility.h>
#include <guid.h>
#include <log/log.h>
#include "tester.h"
#include "asynctmr.h"
#include <ansi_c.h>
#include <multithread/multithread.h>

static SElEventFactoryPtr	gs_EventFactory = NULL;
static STesterPtr 	    	gs_pTester = NULL;
static HANDLE				gs_hThread = NULL;
static HANDLE				hEventStop = NULL;
static int          		g_timerId = 0;
static int          		g_timerId2 = 0;


static SElExceptionPtr RegisterEventFunction(void);
int CVICALLBACK Timer_Jig (int reserved, int theTimerId, int event,
                                 void *callbackData, int eventData1,
                                 int eventData2);

int CVICALLBACK Timer_Table (int reserved, int theTimerId, int event,
                                 void *callbackData, int eventData1,
                                 int eventData2);



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
			strncpy((char*)value, MODULE_TESTER, size);
			break;
		case MODULE_ATTR_MODULE_NAME:
			strncpy((char*)value, "Tester Module", size);
			break;
		case MODULE_ATTR_MODULE_DESCRIPTION:
			strncpy((char*)value, "Module for tester control", size);
			break;
		case MODULE_ATTR_REQUIRED_MODULES:	
			strncpy((char*)value, MODULE_LOG";"MODULE_SENSORS, size);
			break;
		case MODULE_ATTR_BUILD_DATETIME:
			strncpy((char*)value, __DATE__" "__TIME__, size);
			break;
	}
	return NULL;
}

#undef __FUNC__
#define __FUNC__ "{Tester}::LoadElcomModule"
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

	EXCCHECK( tester_new( &gs_pTester));
	EXCCHECK( pParam->SetArg( pParam, "TesterModule", ARG_PTR, (void*) gs_pTester));

	EXCCHECK( pParam->GetArg( pParam,
							 MODULE_LOG,
							 sizeof(void*),
							 &(gs_pTester->LogApi)));

	gs_pTester->LogChannel = E_LOG_CHANNEL_TESTER;

	EXCCHECKCVI( GetModuleDir (__CVIUserHInst, module_dir));
	sprintf(file_name, "%s\\tester.xml", module_dir);
	EXCCHECK( gs_pTester->Initialize(gs_pTester, file_name));

	EXCCHECK( pParam->GetArg( pParam,
							 "EventFactory",
							 sizeof(SElEventFactoryPtr),
							 &gs_EventFactory));
	
#if 0    /* formerly excluded lines */
	EXCCHECK( pParam->GetArg( pParam,
							 "axisModule",
							 sizeof(gs_pTester->axissAPI),
							 &(gs_pTester->axissAPI)));	
#endif   /* formerly excluded lines */
		
	EXCCHECK( RegisterEventFunction());
	
Error:                       
	*pInstance = gs_pTester;
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{Tester}::InitializeElcomModule"
SElExceptionPtr	_FUNCC InitializeElcomModule( void* pInstance, const char* configuration, SPluginArgsPtr pParam)
{
	SElExceptionPtr	pexception = NULL; 
	
	EXCCHECK( pParam->GetArg( pParam,
							  "SensorModule",
							  sizeof(gs_pTester->sensorAPI),
							  &(gs_pTester->sensorAPI)));

	EXCCHECK( pParam->GetArg( pParam,
							 "EventFactory",
							 sizeof(gs_pTester->eventAPI),
							 &(gs_pTester->eventAPI)));
	

	EXCCHECK( gs_pTester->RegisterSensorCallbacks( gs_pTester));
	
	if ( gs_EventFactory )
	{
		EXCCHECK( gs_EventFactory->RegisterEvent(gs_EventFactory, EVNT_JIG_ITEM_INSERTED));
		EXCCHECK( gs_EventFactory->RegisterEvent(gs_EventFactory, EVNT_JIG_ITEM_RELEASED));
		
		EXCCHECK( gs_EventFactory->RegisterEvent(gs_EventFactory, EVNT_JIG_ITEM_LOCKED_STATION_10));
		EXCCHECK( gs_EventFactory->RegisterEvent(gs_EventFactory, EVNT_JIG_ITEM_UNLOCKED_STATION_10));
		
		EXCCHECK( gs_EventFactory->RegisterEvent(gs_EventFactory, EVNT_JIG_ITEM_PLUGGED_STATION_20));
		EXCCHECK( gs_EventFactory->RegisterEvent(gs_EventFactory, EVNT_JIG_ITEM_UNPLUGGED_STATION_20));
		
        EXCCHECK( gs_EventFactory->RegisterEvent(gs_EventFactory, EVNT_JIG_ITEM_VERTICAL_PLUGGED_STATION_30));
		EXCCHECK( gs_EventFactory->RegisterEvent(gs_EventFactory, EVNT_JIG_ITEM_VERTICAL_UNPLUGGED_STATION_30));
		
		EXCCHECK( gs_EventFactory->RegisterEvent(gs_EventFactory, EVNT_JIG_ITEM_PLUGGED_STATION_30));
		EXCCHECK( gs_EventFactory->RegisterEvent(gs_EventFactory, EVNT_JIG_ITEM_UNPLUGGED_STATION_30));
		
		EXCCHECK( gs_EventFactory->RegisterEvent(gs_EventFactory, EVNT_JIG_ITEM_PLUGGED_STATION_40));
		EXCCHECK( gs_EventFactory->RegisterEvent(gs_EventFactory, EVNT_JIG_ITEM_UNPLUGGED_STATION_40));
		
		EXCCHECK( gs_EventFactory->RegisterEvent(gs_EventFactory, EVNT_JIG_ITEM_PLUGGED_STATION_50));
		EXCCHECK( gs_EventFactory->RegisterEvent(gs_EventFactory, EVNT_JIG_ITEM_UNPLUGGED_STATION_50));
		
		EXCCHECK( gs_EventFactory->RegisterEvent(gs_EventFactory, EVNT_JIG_ITEM_OPEN_GRIPPER));
		EXCCHECK( gs_EventFactory->RegisterEvent(gs_EventFactory, EVNT_JIG_ITEM_CLOSE_GRIPPER));
		//-----
		EXCCHECK( gs_EventFactory->RegisterEvent(gs_EventFactory, EVNT_JIG_ITEM_ACTUATOR_X_REAR));
		EXCCHECK( gs_EventFactory->RegisterEvent(gs_EventFactory, EVNT_JIG_ITEM_ACTUATOR_X_FRONT));
		
		EXCCHECK( gs_EventFactory->RegisterEvent(gs_EventFactory, EVNT_JIG_ITEM_ACTUATOR_Z_UP));
		EXCCHECK( gs_EventFactory->RegisterEvent(gs_EventFactory, EVNT_JIG_ITEM_ACTUATOR_Z_DOWN));
		
		
        //--------
		EXCCHECK( gs_EventFactory->RegisterEvent(gs_EventFactory, EVNT_RED_BOX_EMPTY));

		EXCCHECK( gs_EventFactory->RegisterEvent(gs_EventFactory, EVNT_TESTER_ROTATION_FINISHED));
		EXCCHECK( gs_EventFactory->RegisterEvent(gs_EventFactory, EVNT_TESTER_CONTINUE_ROTATION));
		EXCCHECK( gs_EventFactory->RegisterEvent(gs_EventFactory, EVNT_JIG_CYCLE_START));
		EXCCHECK( gs_EventFactory->RegisterEvent(gs_EventFactory, EVNT_JIG_NEW));
	   	EXCCHECK( gs_EventFactory->RegisterEvent(gs_EventFactory, EVNT_JIG_START));
		
	 
	}

    g_timerId = NewAsyncTimer (0.01, -1, 1, Timer_Jig, (void*)gs_pTester);
    g_timerId2 = NewAsyncTimer (0.01, -1, 1, Timer_Table, (void*)gs_pTester);

	gs_pTester->SetSensor(gs_pTester, "LIGHT_TESTER", 1);

Error:
	EXCRETHROW( pexception);   
}

#undef __FUNC__
#define __FUNC__ "{Tester}::UnloadElcomModule"
SElExceptionPtr	_FUNCC UnloadElcomModule( void* pInstance)
{
	SElExceptionPtr    pexception = NULL;        
	
	DiscardAsyncTimer (g_timerId);
	DiscardAsyncTimer (g_timerId2);
	EXCCHECK( tester_delete(&gs_pTester)); 
	
	
Error:
	EXCRETHROW( pexception);   
}

static int32_t		gs_key_count = 0;
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "FunctionCallback"
SElExceptionPtr FunctionCallback( const char* eventName, void* pData)
{
	SElExceptionPtr	pexception = NULL;

	if(pData)
	{
		const char* key = ((SElEventDataPtr)pData)->GetStringPtr(pData);   

		if( 0==strcmp( key, "F1_KEY"))
		{
			gs_key_count++;

			if( gs_key_count>=2)
			{
				EXCCHECK( gs_pTester->SetSensor(gs_pTester, "MASK_REAR", 0));
				EXCCHECK( gs_pTester->SetSensor(gs_pTester, "MASK_FRONT", 1));
				gs_key_count = 0;
			}
		}
		else
		{
			gs_key_count = 0;
		}
	}

Error:                       
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
		                                   "EVNT_GUI_KEY_PRESSED",
		                                   FunctionCallback));	  
   }
   
Error:
   EXCRETHROW(pexception);
}   /* RegisterEventFunction */

#undef  __FUNC__
#define __FUNC__  "Timer_Jig"
int CVICALLBACK Timer_Jig (int reserved, int theTimerId, int event,
                                 void *callbackData, int eventData1,
                                 int eventData2)
{                                                   
	SElExceptionPtr	pexception = NULL;
	STester* me = (STesterPtr) callbackData; 

    /* execute JigThread to refresh inputs */
	EXCCHECK(JigThread(me));
	
Error:
	if (pexception)
	{
	EXCDISPLAY(pexception);
	DiscardAsyncTimer (g_timerId);
	}
	return 0;
}

#undef  __FUNC__
#define __FUNC__  "Timer_Table"
int CVICALLBACK Timer_Table (int reserved, int theTimerId, int event,
                                 void *callbackData, int eventData1,
                                 int eventData2)
{                                                   
	SElExceptionPtr	pexception = NULL;
	STester* me = (STesterPtr) callbackData; 

    /* execute TableThread to refresh inputs */
	EXCCHECK(TableThread(me));
	
Error:
	if (pexception)
	{
	EXCDISPLAY(pexception);
	DiscardAsyncTimer (g_timerId2);
	}
	return 0;
}


