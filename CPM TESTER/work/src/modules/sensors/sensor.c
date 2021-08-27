
/****************************************************************************
 *                         D I O     S E N S O R S
 *---------------------------------------------------------------------------
 *    Copyright   2005 ELCOM, a.s.  All Rights Reserved.        
 *---------------------------------------------------------------------------
 *                                                                          
 * Title:       sensor.h                                                    
 * Purpose:     DIO Sensor interface
 * Version:     1.0  
 ****************************************************************************/
#include "sensors.h"
#include "sensors_gui.h"
#include "msxmldom.h"
#include <stdio.h>
#include <userint.h>
#include <utility.h>
#include <guid.h>
#include <multithread/multithread.h> 
#include <ansi_c.h>
#include "appIO.h"
#include "applicomIO.h"

#define MAX_LENGTH_SENSOR_LOGICAL_NAME				64
#define MAX_LENGTH_SENSOR_PHYSICAL_NAME				64

/* Paramètres Profibus */
#define CARTE_PROFIBUS 	1 
#define ADDRESS_PLC     3 
#define ADDRESS_PV1 	4
#define ADDRESS_PV2 	5
#define ADDRESS_PV3 	6



#define NUMBER_MODULE_DI 	 	   2 
#define BYTES_PER_MODULE_DI 	   4 
#define FIRST_MODULE_POSITION_DI   3 

#define NUMBER_MODULE_AI 	 	   1 
#define WORDS_PER_MODULE_AI 	   8 
#define FIRST_MODULE_POSITION_AI   5 

#define NUMBER_MODULE_D0 	 	   2 
#define BYTES_PER_MODULE_DO 	   4 
#define FIRST_MODULE_POSITION_DO   1

#define NUMBER_MODULE_PV1 	 	   1 
#define BYTES_PER_MODULE_PV1 	   4 
#define FIRST_MODULE_POSITION_PV1  1 

#define NUMBER_MODULE_PV2 	 	   1 
#define BYTES_PER_MODULE_PV2 	   4 
#define FIRST_MODULE_POSITION_PV2  1 

#define NUMBER_MODULE_PV3 	 	   1 
#define BYTES_PER_MODULE_PV3 	   4 
#define FIRST_MODULE_POSITION_PV3  1 


typedef struct _SLogicNames
{
	char		name[MAX_LENGTH_SENSOR_LOGICAL_NAME+1];
	char		physical[MAX_LENGTH_SENSOR_PHYSICAL_NAME+1];
	int			TaskIndex;
	int			ValueIndex;
} SLogicNames, *SLogicNamesPtr;

typedef struct _SFnc
{
	char					name[MAX_LENGTH_SENSOR_LOGICAL_NAME+1];
	SensorCallbackFnc_t*	fCalls;
	void**					pData;
	unsigned char			count;
} SFnc, *SFncPtr;

typedef enum _ETaskType
{
	ETT_NONE = 0,
	ETT_DI,
	ETT_DO,
	PV1_DO,
	PV2_DO,
	PV3_DO,
	ETT_CI,
	ETT_AI
} ETaskType;

typedef struct _STaskConfig
{
	char			name[MAX_LENGTH_SENSOR_LOGICAL_NAME+1];
	unsigned char *			values;
	uint32_t				valuesAI[8];
	uint32_t		value; /* task 32bit value */
	SFncPtr			fncs; /* lines callbacks */
	unsigned long	count;
	SFnc			fnc; /* task callbacks */
	ETaskType		type;
} STaskConfig,	*STaskConfigPtr;

typedef struct _SSensorsConfig
{
	bool_t			IsSimulation;
	int				TaskCount;
	uint32_t		LoopTime;
	STaskConfigPtr	pTasks;
	int				NamesCount;
	SLogicNamesPtr	pNames;
} SSensorsConfig, *SSensorsConfigPtr;

typedef struct _SEventThreadData{
	SDIOSensorsPtr	SDIOSensors;
    int32_t			TaskPos;
	int32_t			ValuePos;
} SEventThreadData, *SEventThreadDataPtr;

static SElExceptionPtr sensorsOpenWithConfiguration(
	SDIOSensorsPtr	SDIOSensors,
	const char*		filename
);
static SElExceptionPtr sensorsClose(
	SDIOSensorsPtr	SDIOSensors
);
static SElExceptionPtr sensorsLock(
	SDIOSensorsPtr	pSensors
);
static SElExceptionPtr sensorsUnLock(
	SDIOSensorsPtr	pSensors
);
static SElExceptionPtr sensorsGetState(
	SDIOSensorsPtr	pSensors,
	const char*		channelName,
	uint32_t*		state
);
static SElExceptionPtr sensorsGetAI(
	SDIOSensorsPtr	pSensors,
	const char*		channelName,
	uint32_t*		value
);
static SElExceptionPtr sensorsSetState(
	SDIOSensorsPtr	pSensors,
	const char*		channelName,
	uint32_t		state
);
static SElExceptionPtr RegisterCallback( 
	struct _SDIOSensors* me,
	const char* name,
	SensorCallbackFnc_t pFnc,
	void* pData
);
static SElExceptionPtr sensorsLoadConfiguration(
	const char*			fileName,
	SSensorsConfigPtr	pConfig
);
static SElExceptionPtr sensorsCreateTasks(
	SSensorsConfigPtr	pConfig
);
static SElExceptionPtr sensorsCleanupTasks(
	SSensorsConfigPtr	pConfig
);
static SElExceptionPtr sensorsCleanupLogicalNames(
	SSensorsConfigPtr	pConfig
);

int CompareTasks(const void *element1, const void *element2);
int CompareLogicalNames(const void *element1, const void *element2);
	
static int gs_sim_test_inputs[] = {
	PANEL_IN1_2,
	PANEL_IN2_2,
	PANEL_IN3_2,
	PANEL_IN4_2,
	PANEL_IN5_2,
	PANEL_IN6_2,
	PANEL_IN7_2,
	PANEL_IN8_2,
	PANEL_IN9_2,
	PANEL_IN10_2,
	PANEL_IN11_2,
	PANEL_IN12_2,
	PANEL_IN13_2,
	PANEL_IN14_2,
	PANEL_IN15_2,
	PANEL_IN16_2,
	PANEL_IN17_2,
	PANEL_IN18_2,
	PANEL_IN19_2,
	PANEL_IN20_2,
	PANEL_IN21_2,
	PANEL_IN22_2,
	PANEL_IN23_2,
	PANEL_IN24_2,
	PANEL_IN25_2,
	PANEL_IN26_2,
	PANEL_IN27_2,
	PANEL_IN28_2,
	PANEL_IN29_2,
	PANEL_IN30_2,
	PANEL_IN31_2,
	PANEL_IN32_2,
	PANEL_IN33_2,
	PANEL_IN34_2,
	PANEL_IN35_2,
	PANEL_IN36_2,
	PANEL_IN37_2,
	PANEL_IN38_2,
	PANEL_IN39_2,
	PANEL_IN40_2,
	PANEL_IN41_2,
	PANEL_IN42_2,
	PANEL_IN43_2,
	PANEL_IN44_2,
	PANEL_IN45_2,
	PANEL_IN46_2,
	PANEL_IN47_2,
	PANEL_IN48_2,
	PANEL_IN49_2,
	PANEL_IN50_2,
	PANEL_IN51_2,
	PANEL_IN52_2,
	PANEL_IN53_2,
	PANEL_IN54_2,
	PANEL_IN55_2,
	PANEL_IN56_2,
	PANEL_IN57_2,
	PANEL_IN58_2,
	PANEL_IN59_2,
	PANEL_IN60_2,
	PANEL_IN61_2,
	PANEL_IN62_2,
	PANEL_IN63_2,
	PANEL_IN64_2,
	PANEL_IN65_2,
	PANEL_IN66_2,
	PANEL_IN67_2,
	PANEL_IN68_2,
	PANEL_IN69_2,
	PANEL_IN70_2,
	PANEL_IN71_2,
	PANEL_IN72_2,
	PANEL_IN73_2,
	PANEL_IN74_2,
	PANEL_IN75_2,
	PANEL_IN76_2,
	PANEL_IN77_2,
	PANEL_IN78_2,
	PANEL_IN79_2,
	PANEL_IN80_2,
	PANEL_IN81_2,
	PANEL_IN82_2,
	PANEL_IN83_2,
	PANEL_IN84_2,
	PANEL_IN85_2,
	PANEL_IN86_2,
	PANEL_IN87_2,
	PANEL_IN88_2,
	PANEL_IN89_2,
	PANEL_IN90_2,
	PANEL_IN91_2,
	PANEL_IN92_2,
	PANEL_IN93_2,
	PANEL_IN94_2,
	PANEL_IN95_2,
	PANEL_IN96_2};

static int gs_sim_test_outputs[] = {
	PANEL_OUT1_2,
	PANEL_OUT2_2,
	PANEL_OUT3_2,
	PANEL_OUT4_2,
	PANEL_OUT5_2,
	PANEL_OUT6_2,
	PANEL_OUT7_2,
	PANEL_OUT8_2,
	PANEL_OUT9_2,
	PANEL_OUT10_2,
	PANEL_OUT11_2,
	PANEL_OUT12_2,
	PANEL_OUT13_2,
	PANEL_OUT14_2,
	PANEL_OUT15_2,
	PANEL_OUT16_2,
	PANEL_OUT17_2,
	PANEL_OUT18_2,
	PANEL_OUT19_2,
	PANEL_OUT20_2,
	PANEL_OUT21_2,
	PANEL_OUT22_2,
	PANEL_OUT23_2,
	PANEL_OUT24_2,
	PANEL_OUT25_2,
	PANEL_OUT26_2,
	PANEL_OUT27_2,
	PANEL_OUT28_2,
	PANEL_OUT29_2,
	PANEL_OUT30_2,
	PANEL_OUT31_2,
	PANEL_OUT32_2};

static int gs_sim_test_cpv[] = {
	PANEL_PV1_2,
	PANEL_PV2_2,
	PANEL_PV3_2,
	PANEL_PV4_2,
	PANEL_PV5_2,
	PANEL_PV6_2,
	PANEL_PV7_2,
	PANEL_PV8_2,
	PANEL_PV9_2,
	PANEL_PV10_2,
	PANEL_PV11_2,
	PANEL_PV12_2,
	PANEL_PV13_2,
	PANEL_PV14_2,
	PANEL_PV15_2,
	PANEL_PV16_2,
	PANEL_PV17_2,
	PANEL_PV18_2,
	PANEL_PV19_2,
	PANEL_PV20_2,
	PANEL_PV21_2,
	PANEL_PV22_2,
	PANEL_PV23_2,
	PANEL_PV24_2,
	PANEL_PV25_2,
	PANEL_PV26_2,
	PANEL_PV27_2,
	PANEL_PV28_2,
	PANEL_PV29_2,
	PANEL_PV30_2,
	PANEL_PV31_2,
	PANEL_PV32_2,
	PANEL_PV33_2,
	PANEL_PV34_2,
	PANEL_PV35_2,
	PANEL_PV36_2,
	PANEL_PV37_2,
	PANEL_PV38_2,
	PANEL_PV39_2,
	PANEL_PV40_2,
	PANEL_PV41_2,
	PANEL_PV42_2,
	PANEL_PV43_2,
	PANEL_PV44_2,
	PANEL_PV45_2,
	PANEL_PV46_2,
	PANEL_PV47_2,
	PANEL_PV48_2};

/******************************************************************************
 * Second thread function
 ******************************************************************************/
static DWORD WINAPI Thread_Sensor(LPVOID param);

/******************************************************************************
 * Callbacks thread function
 ******************************************************************************/
#if defined(_CVI_)
	int CVICALLBACK EventThreadFunc (void *functionData);
#else
	unsigned __cdecl EventThreadFunc( void* pArguments );
#endif

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "diosenosrs_new"
SElExceptionPtr	diosenosrs_new( SDIOSensorsPtr* pSensors)
{
	SElExceptionPtr    pexception = NULL;

	*pSensors = (SDIOSensorsPtr)calloc( 1, sizeof(SDIOSensors));
	EXCCHECKALLOC( *pSensors);

	(*pSensors)->OpenWithConfiguration = sensorsOpenWithConfiguration;
	(*pSensors)->Close           = sensorsClose;
	(*pSensors)->GetState        = sensorsGetState;
	(*pSensors)->GetAI        	 = sensorsGetAI;
	(*pSensors)->SetState        = sensorsSetState;
	(*pSensors)->RegisterCallback = RegisterCallback;
	
	InitializeCriticalSection( &((*pSensors)->_Lock));

#if defined(_CVI_)
	{	/* inner block */
		int	cmt_status = CmtNewThreadPool( 5, &(*pSensors)->_pool_handle);
	    if ( cmt_status < 0 )
	    {
			char	message[256] = "";

			CmtGetErrorMessage ( cmt_status, message);
			EXCTHROW( EL_ERROR_OS_SPECIFIC, message );
	    }
    }	/* inner block */
#endif

Error:
	EXCRETHROW( pexception);	
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "diosenosrs_delete"
SElExceptionPtr	diosenosrs_delete( SDIOSensorsPtr* pSensors)
{
	SElExceptionPtr    pexception = NULL;

	if ( pSensors && *pSensors )
	{
		EXCCHECK( (*pSensors)->Close( *pSensors));
		DeleteCriticalSection( &((*pSensors)->_Lock));
		free( *pSensors);
		*pSensors = NULL;
	}	

Error:
	EXCRETHROW( pexception);	
}


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "sensorsOpenWithConfiguration"
static SElExceptionPtr sensorsOpenWithConfiguration(
	SDIOSensorsPtr	pSensors,
	const char*		filename
)
{
	SElExceptionPtr		pexception = NULL;
	SSensorsConfigPtr	pconfig = NULL;
	char				module_dir[2048] = "";
	int32_t				error = 0,
						monitor_id = 0,
						monitor_id2 = 0,
						left = 0;

	if ( pSensors->_pConfig )
	{
		EXCCHECK( pSensors->Close( pSensors));
	}
	pSensors->_pConfig = calloc(1, sizeof(SSensorsConfig));
	pconfig = (SSensorsConfigPtr) pSensors->_pConfig;
	
	EXCCHECK( sensorsLoadConfiguration( ( filename && *filename != '\0') ? filename : "sensor.xml", pSensors->_pConfig));
	EXCCHECK( sensorsCreateTasks( pSensors->_pConfig));

	if ( pconfig->IsSimulation )
	{
		EXCCHECKCVI( pSensors->_panel = LoadPanelEx( 0, "sensors_gui.uir", PANEL, __CVIUserHInst)); 	
		EXCCHECKCVI( DisplayPanel( pSensors->_panel));
#if 0    /* formerly excluded lines */
		EXCCHECKCVI( GetSystemAttribute( ATTR_PRIMARY_MONITOR, &monitor_id));
		EXCCHECKCVI( GetMonitorAttribute (monitor_id, ATTR_NEXT_MONITOR, &monitor_id2));
		if(monitor_id2)
		{
			EXCCHECKCVI( GetMonitorAttribute (monitor_id2, ATTR_LEFT, &left));
			EXCCHECKCVI( SetPanelAttribute (pSensors->_panel, ATTR_LEFT, left));
		}
#endif   /* formerly excluded lines */
		pSensors->_IsSimulation = TRUE;
	}

	EXCCHECK( ThreadStart( Thread_Sensor, pSensors, &pSensors->_hThread, &pSensors->_hEventStop));

Error:
	EXCRETHROW( pexception);
} /* LoadPluginConfiguration */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "sensorsLock"
static SElExceptionPtr sensorsLock(
	SDIOSensorsPtr	pSensors
)
{
	EnterCriticalSection( &(pSensors->_Lock));
	return NULL;
} /* sensorsLock */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "sensorsUnLock"
static SElExceptionPtr sensorsUnLock(
	SDIOSensorsPtr	pSensors
)
{
	LeaveCriticalSection( &(pSensors->_Lock));
	return NULL;	
} /* sensorsUnLock */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "sensorsClose"
static SElExceptionPtr sensorsClose(
	SDIOSensorsPtr	pSensors
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;

	if ( pSensors->_pConfig )
	{
		EXCCHECK( ThreadStop( &(pSensors->_hThread), &(pSensors->_hEventStop)));

		EXCCHECK( sensorsLock( pSensors)); 

		EXCCHECK( sensorsCleanupTasks( pSensors->_pConfig));
		EXCCHECK( sensorsCleanupLogicalNames( pSensors->_pConfig));
		
		free( pSensors->_pConfig);
		pSensors->_pConfig = NULL;

		if ( pSensors->_panel )
		{
			EXCCHECKCVI( HidePanel(pSensors->_panel));
			EXCCHECKCVI( DiscardPanel( pSensors->_panel));
			pSensors->_panel = 0;
		}
	}

Error:
	sensorsUnLock( pSensors);
	EXCRETHROW( pexception);	
} /* sensorsClose */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "sensorsSetState"
static SElExceptionPtr sensorsSetState(
	SDIOSensorsPtr	pSensors,
	const char*		repCapName,
	uint32_t		state
)
{
	SElExceptionPtr		pexception = NULL;
	SSensorsConfigPtr	pConfig = NULL; 
	SLogicNames	  		tmp;
	SLogicNamesPtr		plogic_name = NULL;
	STaskConfig			tmp2;
	STaskConfigPtr		ptask_config = NULL;
	
	EXCCHECK( sensorsLock( pSensors)); 
	
	pConfig = (SSensorsConfigPtr)(pSensors->_pConfig);	
	if( pConfig)
	{
		strcpy( tmp.name, repCapName);
		plogic_name = bsearch( &tmp, pConfig->pNames, pConfig->NamesCount, sizeof(SLogicNames), CompareLogicalNames);

		if ( plogic_name) /* line name */
		{
			pConfig->pTasks[plogic_name->TaskIndex].values[plogic_name->ValueIndex] = (state>0);
		}
		else /* task name */
		{
			strcpy( tmp2.name, repCapName);
			ptask_config = bsearch( &tmp2, pConfig->pTasks,	pConfig->TaskCount,	sizeof(STaskConfig), CompareTasks);	

			if( ptask_config)
			{
				if( ptask_config->type==ETT_DO)
					ptask_config->value = state;
			}
			else if( ptask_config)
			{
				if( ptask_config->type==PV1_DO)
					ptask_config->value = state;
			}
			else if( ptask_config)
			{
				if( ptask_config->type==PV2_DO)
					ptask_config->value = state;
			}
			
			else if( ptask_config)
			{
				if( ptask_config->type==PV3_DO)
					ptask_config->value = state;
			}
			else
			{
				char buffer[512];

				sprintf( buffer, "Logical name does not exist: %s", repCapName); 
				EXCTHROW( ERROR_SENSORS_LOGICAL_NAME_NOT_EXIST, "Logical name does not exist!");
			}
		}
	}

Error:
	sensorsUnLock( pSensors);
	EXCRETHROW( pexception);		
} /* sensorsSetState */


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "sensorsGetState"
static SElExceptionPtr sensorsGetState(
	SDIOSensorsPtr	pSensors,
	const char*		repCapName,
	uint32_t*		state
)
{
	SElExceptionPtr		pexception = NULL;
	SSensorsConfigPtr	pConfig = NULL; 
	SLogicNames	  		tmp;
	SLogicNamesPtr		plogic_name = NULL;
	STaskConfig			tmp2;
	STaskConfigPtr		ptask_config = NULL;
	
	EXCCHECK( sensorsLock( pSensors));

	pConfig = (SSensorsConfigPtr)(pSensors->_pConfig);	
	if( pConfig)
	{
		strcpy( tmp.name, repCapName);
		plogic_name = bsearch( &tmp, pConfig->pNames, pConfig->NamesCount, sizeof(SLogicNames), CompareLogicalNames);

		if ( plogic_name) /* line name */
		{
			if(state) *state = pConfig->pTasks[plogic_name->TaskIndex].values[plogic_name->ValueIndex];
		}
		else /* task name */
		{
			strcpy( tmp2.name, repCapName);
			ptask_config = bsearch( &tmp2, pConfig->pTasks,	pConfig->TaskCount,	sizeof(STaskConfig), CompareTasks);	

			if( ptask_config)
			{
				if(state) *state = ptask_config->value;
			}
			else
			{
				char buffer[512];

				sprintf( buffer, "Logical name does not exist: %s", repCapName); 
				EXCTHROW( ERROR_SENSORS_LOGICAL_NAME_NOT_EXIST, buffer);
			}
		}
	}
	
Error:
	sensorsUnLock( pSensors);
	EXCRETHROW( pexception);		
} /* sensorsGetState */	


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "sensorsGetAI"
static SElExceptionPtr sensorsGetAI(
	SDIOSensorsPtr	pSensors,
	const char*		repCapName,
	uint32_t*		value
)
{
	SElExceptionPtr		pexception = NULL;
	SSensorsConfigPtr	pConfig = NULL; 
	SLogicNames	  		tmp;
	SLogicNamesPtr		plogic_name = NULL;
	STaskConfig			tmp2;
	STaskConfigPtr		ptask_config = NULL;
	
	EXCCHECK( sensorsLock( pSensors));

	pConfig = (SSensorsConfigPtr)(pSensors->_pConfig);	
	if( pConfig)
	{
		strcpy( tmp.name, repCapName);
		plogic_name = bsearch( &tmp, pConfig->pNames, pConfig->NamesCount, sizeof(SLogicNames), CompareLogicalNames);

		if ( plogic_name) /* line name */
		{
			if(value) *value = pConfig->pTasks[plogic_name->TaskIndex].valuesAI[plogic_name->ValueIndex];
		}
		else /* task name */
		{
				char buffer[512];

				sprintf( buffer, "Logical name does not exist: %s", repCapName); 
				EXCTHROW( ERROR_SENSORS_LOGICAL_NAME_NOT_EXIST, buffer);
		}
	}
	
Error:
	sensorsUnLock( pSensors);
	EXCRETHROW( pexception);		
} /* sensorsGetAI */
/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "RegisterCallback"
static SElExceptionPtr RegisterCallback( 
	struct _SDIOSensors* pSensors,
	const char* repCapName,
	SensorCallbackFnc_t pFnc,
	void* pData
)
{
	SElExceptionPtr		pexception = NULL;
	SSensorsConfigPtr	pConfig = ((SSensorsConfigPtr)(pSensors->_pConfig)); 
	SLogicNames	  		tmp;
	SLogicNamesPtr		plogic_name = NULL;
	STaskConfig			tmp2;
	STaskConfigPtr		ptask_config = NULL;
	
	if( pFnc)
	{
		EXCCHECK( sensorsLock( pSensors));
		
		strcpy( tmp.name, repCapName);
		plogic_name = bsearch( &tmp, pConfig->pNames, pConfig->NamesCount, sizeof(SLogicNames), CompareLogicalNames);

		if ( plogic_name) /* line name */
		{
			int32_t count = pConfig->pTasks[plogic_name->TaskIndex].fncs[plogic_name->ValueIndex].count;

			pConfig->pTasks[plogic_name->TaskIndex].fncs[plogic_name->ValueIndex].fCalls = 
					(SensorCallbackFnc_t*)realloc( pConfig->pTasks[plogic_name->TaskIndex].fncs[plogic_name->ValueIndex].fCalls,
					(count+1)*sizeof(SensorCallbackFnc_t));

			pConfig->pTasks[plogic_name->TaskIndex].fncs[plogic_name->ValueIndex].pData = 
					(void*)realloc( pConfig->pTasks[plogic_name->TaskIndex].fncs[plogic_name->ValueIndex].pData,
					(count+1)*sizeof(void*));
			
			strncpy( pConfig->pTasks[plogic_name->TaskIndex].fncs[plogic_name->ValueIndex].name, repCapName, MAX_LENGTH_SENSOR_LOGICAL_NAME); 
			pConfig->pTasks[plogic_name->TaskIndex].fncs[plogic_name->ValueIndex].fCalls[count] = pFnc;
			pConfig->pTasks[plogic_name->TaskIndex].fncs[plogic_name->ValueIndex].pData[count] = pData;
			pConfig->pTasks[plogic_name->TaskIndex].fncs[plogic_name->ValueIndex].count++;
		}
		else /* task name */
		{
			strcpy( tmp2.name, repCapName);
			ptask_config = bsearch( &tmp2, pConfig->pTasks,	pConfig->TaskCount,	sizeof(STaskConfig), CompareTasks);	

			if( ptask_config)
			{
				int32_t count = ptask_config->fnc.count;

				ptask_config->fnc.fCalls = (SensorCallbackFnc_t*)realloc(ptask_config->fnc.fCalls, (count+1)*sizeof(SensorCallbackFnc_t));
				ptask_config->fnc.pData = (void*)realloc(ptask_config->fnc.pData, (count+1)*sizeof(void*));

				strncpy( ptask_config->fnc.name, repCapName, MAX_LENGTH_SENSOR_LOGICAL_NAME);
				ptask_config->fnc.fCalls[count] = pFnc;
				ptask_config->fnc.pData[count] = pData;
				ptask_config->fnc.count++;
			}
			else
			{
				char buffer[512];

				sprintf( buffer, "Logical name does not exist: %s", repCapName); 
				EXCTHROW( ERROR_SENSORS_LOGICAL_NAME_NOT_EXIST, buffer);
			}
		}
	}
	
Error:
	sensorsUnLock( pSensors);
	EXCRETHROW( pexception);				
}

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "sensorsLoadConfiguration"
static SElExceptionPtr sensorsLoadConfiguration(
	const char*			fileName,
	SSensorsConfigPtr	pConfig
)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	HRESULT         hr = 0;
	char         	_message[512];
	CAObjHandle     pxml  = 0;
	CAObjHandle     pnode = 0, plist = 0, plist2 = 0;
	CAObjHandle     pparameter = 0;
	CAObjHandle     pitem = 0, pitem2 = 0;
	const char*     pfile_name = (fileName) ? fileName : "\\sensor.xml";
	VBOOL         	is_xml_success = 1;
	VARIANT         pfile_name_var = CA_VariantEmpty();
	char*         	ptext= NULL;
	char			path[512];
	int32_t			i, j;

	memset(pConfig, 0, sizeof(SSensorsConfig));
	pConfig->LoopTime = 25;
	
	EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml));
	EXCCHECKCVI( CA_VariantSetCString(  &pfile_name_var, pfile_name));
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (pxml, NULL, pfile_name_var, &is_xml_success));
	
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//module[@id='"MODULE_SENSORS"']", &pnode) );   
	
	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "parameters/parameter[@name='simulation']", &pparameter));   
	if ( pparameter )
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
		if(ptext)
		{
			pConfig->IsSimulation = (stricmp(ptext,"true") == 0); 
		}
		EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
		pparameter = 0;
		CA_FreeMemory(ptext);
		ptext=NULL;
	}	
	
	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectNodes (pnode, NULL, "parameters/tasks/task", &plist));   
	if(plist)
	{
		STaskConfigPtr  pTask_config = NULL;
		SLogicNamesPtr	plogic_names = NULL;
		long			length = 0, length2 = 0;	
	
		EXCCHECKMSXML( MSXML_IXMLDOMNodeListGetlength (plist, NULL, &length));
		
		pConfig->TaskCount = length;
		pConfig->pTasks = (STaskConfigPtr) calloc( length, sizeof(STaskConfig));
		pTask_config = pConfig->pTasks;
	
		EXCCHECKMSXML( MSXML_IXMLDOMNodeListreset (plist, NULL));
		
		for( i=0; i<length; i++)
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeListnextNode (plist, NULL, &pitem));
			if(pitem)
			{
				EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pitem, NULL, "@name", &pparameter));
				if(pparameter)
				{
					EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
					if(ptext)
					{
						strncpy(pTask_config->name, ptext, MAX_LENGTH_SENSOR_LOGICAL_NAME);	 
					}
					EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
					pparameter = 0;
					CA_FreeMemory(ptext);
					ptext=NULL;
				}

				EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pitem, NULL, "@type", &pparameter));
				if(pparameter)
				{
					size_t length , length_ok=3;
					EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
					length = strlen(ptext) ;
					if(ptext)
					{
						if( 0==strcmp(ptext, "DI"))
						{
							pTask_config->type = ETT_DI;
						}
						else if( 0==strcmp(ptext, "DO"))
						{
							pTask_config->type = ETT_DO;
						}
						else if( 0==strcmp (ptext, "PV1"))
						{
							pTask_config->type = PV1_DO;
						}
						else if( 0==strcmp(ptext, "PV2"))
						{
							pTask_config->type = PV2_DO;
						}
						else if( 0==strcmp(ptext, "PV3"))
						{
							pTask_config->type = PV3_DO;
						}
						else if( 0==strcmp(ptext, "CI"))
						{
							pTask_config->type = ETT_CI;
						}
						else if( 0==strcmp(ptext, "AI"))
						{
							pTask_config->type = ETT_AI;
						}
					}
					EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
					pparameter = 0;
					CA_FreeMemory(ptext);
					ptext=NULL;
				}
				pTask_config++;  
				CA_DiscardObjHandle( pitem ); 
				pitem = 0; 
			}
		}

		qsort( pConfig->pTasks, pConfig->TaskCount, sizeof(STaskConfig), CompareTasks);
		pTask_config = pConfig->pTasks;
		
		for( i=0; i<length; i++)
		{
			sprintf( path, "parameters/tasks/task[@name='%s']", pTask_config->name);
			EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pitem)); 
			if( pitem)
			{
				EXCCHECKMSXML( MSXML_IXMLDOMNodeselectNodes (pitem, NULL, "line", &plist2));
				EXCCHECKMSXML( MSXML_IXMLDOMNodeListGetlength (plist2, NULL, &length2));
				pTask_config->count = length2;

				pTask_config->values = (unsigned char*)calloc( pTask_config->count, sizeof(unsigned char));
				pTask_config->fncs = (SFncPtr)calloc( pTask_config->count, sizeof( SFnc));
				pConfig->pNames = (SLogicNamesPtr)realloc( pConfig->pNames, (pConfig->NamesCount + pTask_config->count) * sizeof(SLogicNames));
	
				plogic_names = pConfig->pNames + pConfig->NamesCount;
				pConfig->NamesCount += pTask_config->count;
				memset( plogic_names, 0, pTask_config->count * sizeof(SLogicNames) ); 

				EXCCHECKMSXML( MSXML_IXMLDOMNodeListreset (plist2, NULL));

				for( j=0; j<length2; j++, plogic_names++)
				{
					EXCCHECKMSXML( MSXML_IXMLDOMNodeListnextNode (plist2, NULL, &pitem2));
					if( pitem2)
					{
						EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pitem2, NULL, "@name", &pparameter));
						if(pparameter)
						{
							EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
							if(ptext)
							{
								strncpy(plogic_names->name, ptext, MAX_LENGTH_SENSOR_LOGICAL_NAME);	 
							}
							EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
							pparameter = 0;
							CA_FreeMemory(ptext);
							ptext=NULL;
						}	

						EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pitem2, NULL, "@physical", &pparameter));
						if(pparameter)
						{
							EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
							if(ptext)
							{
								strncpy(plogic_names->physical, ptext, MAX_LENGTH_SENSOR_LOGICAL_NAME);	 
							}
							EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
							pparameter = 0;
							CA_FreeMemory(ptext);
							ptext=NULL;
						}	

						CA_DiscardObjHandle( pitem2); 
						pitem2 = 0; 
					}
			
					plogic_names->TaskIndex  = i;
					plogic_names->ValueIndex = j;
				}

				pTask_config++;  
				CA_DiscardObjHandle( pitem ); 
				pitem = 0; 
			}
		}
	
		qsort( pConfig->pNames, pConfig->NamesCount, sizeof(SLogicNames), CompareLogicalNames);
	}

Error:
	CA_FreeMemory(ptext);
	CA_VariantClear(&pfile_name_var);
	if (plist) CA_DiscardObjHandle(plist);
	if (pitem) CA_DiscardObjHandle(pitem);
	if (pparameter) CA_DiscardObjHandle(pparameter);
	if (pnode) CA_DiscardObjHandle(pnode);
	if (pxml) CA_DiscardObjHandle(pxml);
	EXCRETHROW( pexception);		
} /* sensorsLoadConfiguration */	

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "sensorsCreateTasks"
static SElExceptionPtr sensorsCreateTasks(
	SSensorsConfigPtr	pConfig
)
{
	SElExceptionPtr		pexception = NULL;
	unsigned long		error = 0;
	STaskConfigPtr		pTask_config = pConfig->pTasks;
	SLogicNamesPtr		plogic_names = pConfig->pNames;
 	short status = 0;
 	short 	statusShort = 0;
	int					i,j;

	if( !pConfig->IsSimulation)
	{
	IO_Exit (CARTE_PROFIBUS, &status);
	IO_Init (CARTE_PROFIBUS, &statusShort);
	}
	
Error:
	EXCRETHROW( pexception);		
} /* sensorsLoadTasks */

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "sensorsCleanupTasks"
static SElExceptionPtr sensorsCleanupTasks(
	SSensorsConfigPtr	pConfig
)
{
	SElExceptionPtr		pexception = NULL;
	unsigned long				error = 0;
	STaskConfigPtr		pTask_config = pConfig->pTasks;
	int					i, j;
	short status = 0;
	
	if ( pConfig->pTasks )
	{
		for ( i=0; i<pConfig->TaskCount; i++, pTask_config++)	
		{
			if( !pConfig->IsSimulation)
			{
				IO_Exit (CARTE_PROFIBUS, &status);
			}

			for(j=0; j<pTask_config->count; j++)
			{
				free( pTask_config->fncs[j].fCalls);
				pTask_config->fncs[j].fCalls = NULL;

				free( pTask_config->fncs[j].pData);
				pTask_config->fncs[j].pData = NULL;
			}
			free( pTask_config->fncs);
			pTask_config->fncs = NULL;

			free( pTask_config->fnc.fCalls);
			pTask_config->fnc.fCalls = NULL;

			free( pTask_config->fnc.pData);
			pTask_config->fnc.pData = NULL;

			free( pTask_config->values);
			pTask_config->values = NULL;
		}

		pConfig->TaskCount = 0;
		free( pConfig->pTasks);
		pConfig->pTasks = NULL;
	}

Error:
	EXCRETHROW( pexception);
}

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "sensorsCleanupLogicalNames"
static SElExceptionPtr sensorsCleanupLogicalNames(
	SSensorsConfigPtr	pConfig
)
{
	SElExceptionPtr		pexception = NULL;

	if ( pConfig->pNames )
	{
		free( pConfig->pNames);
		pConfig->pNames = NULL;
		pConfig->NamesCount = 0;
	}

Error:
	EXCRETHROW( pexception);
}

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "CompareTasks"
int CompareTasks(const void *element1, const void *element2)
{
	STaskConfigPtr	t1 = (STaskConfigPtr)element1;
	STaskConfigPtr	t2 = (STaskConfigPtr)element2;
	return strcmp(t1->name, t2->name);
}  /* CompareTasks */

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "CompareLogicalNames"
int CompareLogicalNames(const void *element1, const void *element2)
{
	SLogicNamesPtr	t1 = (SLogicNamesPtr)element1;
	SLogicNamesPtr	t2 = (SLogicNamesPtr)element2;
	return strcmp(t1->name, t2->name);
} /* CompareLogicalNames */

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "CheckDIChanges"
static SElExceptionPtr CheckDIChanges(
	SDIOSensorsPtr pSensors,
	STaskConfigPtr pTasks, 
	unsigned char** pold_data, 
	int32_t TaskCount
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			i, j;
	int				cmt_status;

	for(i=0; i<TaskCount; i++)
	{
		if( 0!=memcmp(pold_data[i], pTasks[i].values, pTasks[i].count*sizeof(unsigned char)))
		{
			SEventThreadDataPtr	pthread_data = NULL;

			/* process tasks callbacks */
			if( pTasks[i].fnc.count!=0)
			{
				pthread_data = (SEventThreadDataPtr)calloc(1, sizeof(SEventThreadData)); 
				EXCCHECKALLOC( pthread_data);

				pthread_data->SDIOSensors = pSensors;
				pthread_data->TaskPos = i;
				pthread_data->ValuePos = -1;
#if defined(_CVI_)		
					cmt_status = CmtScheduleThreadPoolFunction(
									   pSensors->_pool_handle,
									   EventThreadFunc,
									   pthread_data,
									   NULL);					
#else
					_beginthread( EventThreadFunc, 0, pthread_data);
#endif
			}

			/* process lines callbacks */
			for(j=0; j<pTasks[i].count; j++)
			{
				if( pTasks[i].fncs[j].count!=0 && pold_data[i][j] != pTasks[i].values[j])
				{
					pthread_data = (SEventThreadDataPtr)calloc(1, sizeof(SEventThreadData)); 
					EXCCHECKALLOC( pthread_data);

					pthread_data->SDIOSensors = pSensors;
					pthread_data->TaskPos = i;
					pthread_data->ValuePos = j;
#if defined(_CVI_)		
					cmt_status = CmtScheduleThreadPoolFunction(
									   pSensors->_pool_handle,
									   EventThreadFunc,
									   pthread_data,
									   NULL);					
#else
					_beginthread( EventThreadFunc, 0, pthread_data);
#endif
				}
			}
		}

		/* actualize old data */
		memcpy(pold_data[i], pTasks[i].values, pTasks[i].count*sizeof(unsigned char));
		
	}

Error:
	EXCRETHROW( pexception);
}

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Thread_Sensor"
static DWORD WINAPI Thread_Sensor(LPVOID param)
{
	SElExceptionPtr		pexception = NULL;
	SDIOSensorsPtr		pSensors = (SDIOSensorsPtr) param;
	SSensorsConfigPtr	pConfig  = (SSensorsConfigPtr)pSensors->_pConfig;
	DWORD				state;
	int32_t				error;
	uint32_t			jig_code = 0;
	int					i,j,x,y;
	bool_t				is_locked;
	unsigned char **				pold_data = NULL;
	short Status = 0;
	SLogicNamesPtr		plogic_names = pConfig->pNames;

	//CmtSetCurrentThreadPriority (THREAD_PRIORITY_TIME_CRITICAL);

	/*************************************************
	 * allocate buffer for prevous data
	 *************************************************/
	pold_data = (uint8_t**)calloc( pConfig->TaskCount, sizeof(uint8_t*));
	for( i = 0; i < pConfig->TaskCount; i++)
	{
		pold_data[i] = (uint8_t*)calloc( pConfig->pTasks[i].count, sizeof(uint8_t));
	}

	while(TRUE)
	{
		if (WaitForSingleObject(pSensors->_hEventStop, 1) == WAIT_OBJECT_0)
		{
			for(i=0; i<pConfig->TaskCount; i++)
			{
				free( pold_data[i]);
				pold_data[i] = 0;
			}
			free( pold_data);
			pold_data = NULL;

			if(is_locked)
			{
				sensorsUnLock(pSensors);
				is_locked = FALSE;
			}

			ExitThread(THREAD_EXIT_CODE);
		}

		EXCCHECK( sensorsLock( pSensors)); 
		is_locked = TRUE;

		if ( pConfig->IsSimulation)
		{
			STaskConfig			tmp;
			STaskConfigPtr		ptask_config = NULL;


			strcpy( tmp.name, "PLC_MOD3_P0_DI");
			ptask_config = bsearch( &tmp, pConfig->pTasks, pConfig->TaskCount, sizeof(STaskConfig), CompareTasks);	
			if( ptask_config)
			{
				ptask_config->value = 0;
				for ( i=0; i<ptask_config->count && i<sizeof(gs_sim_test_inputs)/sizeof(gs_sim_test_inputs[0]); i++)
				{
					EXCCHECKCVI( GetCtrlVal( pSensors->_panel, gs_sim_test_inputs[i], &x));
					ptask_config->values[i] = (unsigned char)x;
					ptask_config->value += (uint32_t)(ptask_config->values[i] << i);
				}
			}
		
			strcpy( tmp.name, "PLC_MOD3_P1_DI");
			ptask_config = bsearch( &tmp, pConfig->pTasks, pConfig->TaskCount, sizeof(STaskConfig), CompareTasks);	
			if( ptask_config)
			{
				ptask_config->value = 0;
				for ( i=0; i<ptask_config->count && i<sizeof(gs_sim_test_inputs)/sizeof(gs_sim_test_inputs[0]); i++)
				{
					EXCCHECKCVI( GetCtrlVal( pSensors->_panel, gs_sim_test_inputs[i+8], &x));
					ptask_config->values[i] = (unsigned char)x;
					ptask_config->value += (uint32_t)(ptask_config->values[i] << i);
				}
			}
	
			strcpy( tmp.name, "PLC_MOD3_P2_DI");
			ptask_config = bsearch( &tmp, pConfig->pTasks, pConfig->TaskCount, sizeof(STaskConfig), CompareTasks);	
			if( ptask_config)
			{
				ptask_config->value = 0;
				for ( i=0; i<ptask_config->count && i<sizeof(gs_sim_test_inputs)/sizeof(gs_sim_test_inputs[0]); i++)
				{
					EXCCHECKCVI( GetCtrlVal( pSensors->_panel, gs_sim_test_inputs[i+16], &x));
					ptask_config->values[i] = (unsigned char)x;
					ptask_config->value += (uint32_t)(ptask_config->values[i] << i);
				}
			}
			
			strcpy( tmp.name, "PLC_MOD3_P3_DI");
			ptask_config = bsearch( &tmp, pConfig->pTasks, pConfig->TaskCount, sizeof(STaskConfig), CompareTasks);	
			if( ptask_config)
			{
				ptask_config->value = 0;
				for ( i=0; i<ptask_config->count && i<sizeof(gs_sim_test_inputs)/sizeof(gs_sim_test_inputs[0]); i++)
				{
					EXCCHECKCVI( GetCtrlVal( pSensors->_panel, gs_sim_test_inputs[i+24], &x));
					ptask_config->values[i] = (unsigned char)x;
					ptask_config->value += (uint32_t)(ptask_config->values[i] << i);
				}
			}
			
			strcpy( tmp.name, "PLC_MOD4_P0_DI");
			ptask_config = bsearch( &tmp, pConfig->pTasks, pConfig->TaskCount, sizeof(STaskConfig), CompareTasks);	
			if( ptask_config)
			{
				ptask_config->value = 0;
				for ( i=0; i<ptask_config->count && i<sizeof(gs_sim_test_inputs)/sizeof(gs_sim_test_inputs[0]); i++)
				{
					EXCCHECKCVI( GetCtrlVal( pSensors->_panel, gs_sim_test_inputs[i+32], &x));
					ptask_config->values[i] = (unsigned char)x;
					ptask_config->value += (uint32_t)(ptask_config->values[i] << i);
				}
			}
			
			strcpy( tmp.name, "PLC_MOD4_P1_DI");
			ptask_config = bsearch( &tmp, pConfig->pTasks, pConfig->TaskCount, sizeof(STaskConfig), CompareTasks);	
			if( ptask_config)
			{
				ptask_config->value = 0;
				for ( i=0; i<ptask_config->count && i<sizeof(gs_sim_test_inputs)/sizeof(gs_sim_test_inputs[0]); i++)
				{
					EXCCHECKCVI( GetCtrlVal( pSensors->_panel, gs_sim_test_inputs[i+40], &x));
					ptask_config->values[i] = (unsigned char)x;
					ptask_config->value += (uint32_t)(ptask_config->values[i] << i);
				}
			}
			
			strcpy( tmp.name, "PLC_MOD4_P2_DI");
			ptask_config = bsearch( &tmp, pConfig->pTasks, pConfig->TaskCount, sizeof(STaskConfig), CompareTasks);	
			if( ptask_config)
			{
				ptask_config->value = 0;
				for ( i=0; i<ptask_config->count && i<sizeof(gs_sim_test_inputs)/sizeof(gs_sim_test_inputs[0]); i++)
				{
					EXCCHECKCVI( GetCtrlVal( pSensors->_panel, gs_sim_test_inputs[i+48], &x));
					ptask_config->values[i] = (unsigned char)x;
					ptask_config->value += (uint32_t)(ptask_config->values[i] << i);
				}
			}
			
			strcpy( tmp.name, "PLC_MOD4_P3_DI");
			ptask_config = bsearch( &tmp, pConfig->pTasks, pConfig->TaskCount, sizeof(STaskConfig), CompareTasks);	
			if( ptask_config)
			{
				ptask_config->value = 0;
				for ( i=0; i<ptask_config->count && i<sizeof(gs_sim_test_inputs)/sizeof(gs_sim_test_inputs[0]); i++)
				{
					EXCCHECKCVI( GetCtrlVal( pSensors->_panel, gs_sim_test_inputs[i+56], &x));
					ptask_config->values[i] = (unsigned char)x;
					ptask_config->value += (uint32_t)(ptask_config->values[i] << i);
				}
			}
			strcpy( tmp.name, "PLC_MOD5_P0_DI");
			ptask_config = bsearch( &tmp, pConfig->pTasks, pConfig->TaskCount, sizeof(STaskConfig), CompareTasks);	
			if( ptask_config)
			{
				ptask_config->value = 0;
				for ( i=0; i<ptask_config->count && i<sizeof(gs_sim_test_inputs)/sizeof(gs_sim_test_inputs[0]); i++)
				{
					EXCCHECKCVI( GetCtrlVal( pSensors->_panel, gs_sim_test_inputs[i+64], &x));
					ptask_config->values[i] = (unsigned char)x;
					ptask_config->value += (uint32_t)(ptask_config->values[i] << i);
				}
			}
			
			strcpy( tmp.name, "PLC_MOD5_P1_DI");
			ptask_config = bsearch( &tmp, pConfig->pTasks, pConfig->TaskCount, sizeof(STaskConfig), CompareTasks);	
			if( ptask_config)
			{
				ptask_config->value = 0;
				for ( i=0; i<ptask_config->count && i<sizeof(gs_sim_test_inputs)/sizeof(gs_sim_test_inputs[0]); i++)
				{
					EXCCHECKCVI( GetCtrlVal( pSensors->_panel, gs_sim_test_inputs[i+72], &x));
					ptask_config->values[i] = (unsigned char)x;
					ptask_config->value += (uint32_t)(ptask_config->values[i] << i);
				}
			}
			
			strcpy( tmp.name, "PLC_MOD5_P2_DI");
			ptask_config = bsearch( &tmp, pConfig->pTasks, pConfig->TaskCount, sizeof(STaskConfig), CompareTasks);	
			if( ptask_config)
			{
				ptask_config->value = 0;
				for ( i=0; i<ptask_config->count && i<sizeof(gs_sim_test_inputs)/sizeof(gs_sim_test_inputs[0]); i++)
				{
					EXCCHECKCVI( GetCtrlVal( pSensors->_panel, gs_sim_test_inputs[i+80], &x));
					ptask_config->values[i] = (unsigned char)x;
					ptask_config->value += (uint32_t)(ptask_config->values[i] << i);
				}
			}
			
			strcpy( tmp.name, "PLC_MOD5_P3_DI");
			ptask_config = bsearch( &tmp, pConfig->pTasks, pConfig->TaskCount, sizeof(STaskConfig), CompareTasks);	
			if( ptask_config)
			{
				ptask_config->value = 0;
				for ( i=0; i<ptask_config->count && i<sizeof(gs_sim_test_inputs)/sizeof(gs_sim_test_inputs[0]); i++)
				{
					EXCCHECKCVI( GetCtrlVal( pSensors->_panel, gs_sim_test_inputs[i+88], &x));
					ptask_config->values[i] = (unsigned char)x;
					ptask_config->value += (uint32_t)(ptask_config->values[i] << i);
				}
			}
			
			strcpy( tmp.name, "PLC_MOD1_P0_DO");
			ptask_config = bsearch( &tmp, pConfig->pTasks, pConfig->TaskCount, sizeof(STaskConfig), CompareTasks);	
			if( ptask_config)
			{
				ptask_config->value = 0;
				for ( i=0; i<ptask_config->count && i<sizeof(gs_sim_test_inputs)/sizeof(gs_sim_test_inputs[0]); i++)
				{
					x = (int)(ptask_config->values[i]);
					EXCCHECKCVI( SetCtrlVal( pSensors->_panel, gs_sim_test_outputs[i], x));
					ptask_config->value += (uint32_t)(ptask_config->values[i] << i);
				}
			}

			strcpy( tmp.name, "PLC_MOD1_P1_DO");
			ptask_config = bsearch( &tmp, pConfig->pTasks, pConfig->TaskCount, sizeof(STaskConfig), CompareTasks);	
			if( ptask_config)
			{
				ptask_config->value = 0;
				for ( i=0; i<ptask_config->count && i<sizeof(gs_sim_test_inputs)/sizeof(gs_sim_test_inputs[0]); i++)
				{
					x = (int)(ptask_config->values[i]);
					EXCCHECKCVI( SetCtrlVal( pSensors->_panel, gs_sim_test_outputs[i+8], x));
					ptask_config->value += (uint32_t)(ptask_config->values[i] << i);
				}
			}
	
			strcpy( tmp.name, "PLC_MOD1_P2_DO");
			ptask_config = bsearch( &tmp, pConfig->pTasks, pConfig->TaskCount, sizeof(STaskConfig), CompareTasks);	
			if( ptask_config)
			{
				ptask_config->value = 0;
				for ( i=0; i<ptask_config->count && i<sizeof(gs_sim_test_inputs)/sizeof(gs_sim_test_inputs[0]); i++)
				{
					x = (int)(ptask_config->values[i]);
					EXCCHECKCVI( SetCtrlVal( pSensors->_panel, gs_sim_test_outputs[i+16], x));
					ptask_config->value += (uint32_t)(ptask_config->values[i] << i);
				}
			}
			
			strcpy( tmp.name, "PLC_MOD1_P3_DO");
			ptask_config = bsearch( &tmp, pConfig->pTasks, pConfig->TaskCount, sizeof(STaskConfig), CompareTasks);	
			if( ptask_config)
			{
				ptask_config->value = 0;
				for ( i=0; i<ptask_config->count && i<sizeof(gs_sim_test_inputs)/sizeof(gs_sim_test_inputs[0]); i++)
				{
					x = (int)(ptask_config->values[i]);
					EXCCHECKCVI( SetCtrlVal( pSensors->_panel, gs_sim_test_outputs[i+24], x));
					ptask_config->value += (uint32_t)(ptask_config->values[i] << i);
				}
			}
			
			strcpy( tmp.name, "PV1_MOD1_P1_DO");
			ptask_config = bsearch( &tmp, pConfig->pTasks, pConfig->TaskCount, sizeof(STaskConfig), CompareTasks);	
			if( ptask_config)
			{
				ptask_config->value = 0;
				for ( i=0; i<ptask_config->count && i<sizeof(gs_sim_test_inputs)/sizeof(gs_sim_test_inputs[0]); i++)
				{
					x = (int)(ptask_config->values[i]);
					EXCCHECKCVI( SetCtrlVal( pSensors->_panel, gs_sim_test_cpv[i], x));
					ptask_config->value += (uint32_t)(ptask_config->values[i] << i);
				}
			}
			strcpy( tmp.name, "PV1_MOD1_P2_DO");
			ptask_config = bsearch( &tmp, pConfig->pTasks, pConfig->TaskCount, sizeof(STaskConfig), CompareTasks);	
			if( ptask_config)
			{
				ptask_config->value = 0;
				for ( i=0; i<ptask_config->count && i<sizeof(gs_sim_test_inputs)/sizeof(gs_sim_test_inputs[0]); i++)
				{
					x = (int)(ptask_config->values[i]);
					EXCCHECKCVI( SetCtrlVal( pSensors->_panel, gs_sim_test_cpv[i+8], x));
					ptask_config->value += (uint32_t)(ptask_config->values[i] << i);
				}
			}
			strcpy( tmp.name, "PV1_MOD1_P3_DO");
			ptask_config = bsearch( &tmp, pConfig->pTasks, pConfig->TaskCount, sizeof(STaskConfig), CompareTasks);	
			if( ptask_config)
			{
				ptask_config->value = 0;
				for ( i=0; i<ptask_config->count && i<sizeof(gs_sim_test_inputs)/sizeof(gs_sim_test_inputs[0]); i++)
				{
					x = (int)(ptask_config->values[i]);
					EXCCHECKCVI( SetCtrlVal( pSensors->_panel, gs_sim_test_cpv[i+16], x));
					ptask_config->value += (uint32_t)(ptask_config->values[i] << i);
				}
			}
			strcpy( tmp.name, "PV1_MOD1_P4_DO");
			ptask_config = bsearch( &tmp, pConfig->pTasks, pConfig->TaskCount, sizeof(STaskConfig), CompareTasks);	
			if( ptask_config)
			{
				ptask_config->value = 0;
				for ( i=0; i<ptask_config->count && i<sizeof(gs_sim_test_inputs)/sizeof(gs_sim_test_inputs[0]); i++)
				{
					x = (int)(ptask_config->values[i]);
					EXCCHECKCVI( SetCtrlVal( pSensors->_panel, gs_sim_test_cpv[i+24], x));
					ptask_config->value += (uint32_t)(ptask_config->values[i] << i);
				}
			}
			strcpy( tmp.name, "PV2_MOD1_P1_DO");
			ptask_config = bsearch( &tmp, pConfig->pTasks, pConfig->TaskCount, sizeof(STaskConfig), CompareTasks);	
			if( ptask_config)
			{
				ptask_config->value = 0;
				for ( i=0; i<ptask_config->count && i<sizeof(gs_sim_test_inputs)/sizeof(gs_sim_test_inputs[0]); i++)
				{
					x = (int)(ptask_config->values[i]);
					EXCCHECKCVI( SetCtrlVal( pSensors->_panel, gs_sim_test_cpv[i+32], x));
					ptask_config->value += (uint32_t)(ptask_config->values[i] << i);
				}
			}
			
			strcpy( tmp.name, "PV2_MOD1_P2_DO");
			ptask_config = bsearch( &tmp, pConfig->pTasks, pConfig->TaskCount, sizeof(STaskConfig), CompareTasks);	
			if( ptask_config)
			{
				ptask_config->value = 0;
				for ( i=0; i<ptask_config->count && i<sizeof(gs_sim_test_inputs)/sizeof(gs_sim_test_inputs[0]); i++)
				{
					x = (int)(ptask_config->values[i]);
					EXCCHECKCVI( SetCtrlVal( pSensors->_panel, gs_sim_test_cpv[i+40], x));
					ptask_config->value += (uint32_t)(ptask_config->values[i] << i);
				}
			}
			
			strcpy( tmp.name, "JIGINSERTED");
			ptask_config = bsearch( &tmp, pConfig->pTasks, pConfig->TaskCount, sizeof(STaskConfig), CompareTasks);	
			if( ptask_config)
			{
				ptask_config->value = 0;
				EXCCHECKCVI( GetCtrlVal( pSensors->_panel, PANEL_JIG_CODE, &jig_code));
				for( i=0; i<ptask_config->count; i++)
				{
					ptask_config->values[i] = ( (1<<i)&jig_code)? 1:0;
					ptask_config->value += (uint32_t)(ptask_config->values[i] << i);
				}
			}
		}
	
		else 	
		{   
			BYTE TabReadByte [NUMBER_MODULE_DI * BYTES_PER_MODULE_DI] = {0}; /* PLC DI 12 Bytes */
			WORD TabReadWord [NUMBER_MODULE_AI * WORDS_PER_MODULE_AI] = {0,0,0,0,0,0,0,0}; /* PLC AI 8 Words */
			BYTE TabWriteByte [NUMBER_MODULE_D0 * BYTES_PER_MODULE_DO]; /* PLC D0 4 Bytes */
			BYTE TabWriteBytePV1 [NUMBER_MODULE_PV1 * BYTES_PER_MODULE_PV1]; /* PV1 D0 4 Bytes */
			BYTE TabWriteBytePV2 [NUMBER_MODULE_PV2 * BYTES_PER_MODULE_PV2]; /* PV2 D0 4 Bytes */
			BYTE TabWriteBytePV3 [NUMBER_MODULE_PV3 * BYTES_PER_MODULE_PV3]; /* PV2 D0 4 Bytes */

			int Bit;
			int Module,Port,Line;
			int Old_Byte = -1;
			int Initialize_Byte = 0;
			uint32_t analog;
					 
			EXCCHECKCPLC( IO_RefreshInput (CARTE_PROFIBUS, &Status));
			EXCCHECKCPLC( IO_ReadIByte (CARTE_PROFIBUS, ADDRESS_PLC, 0, NUMBER_MODULE_DI * BYTES_PER_MODULE_DI, TabReadByte, &Status));
			EXCCHECKCPLC( IO_ReadIWord (CARTE_PROFIBUS, ADDRESS_PLC, 8, NUMBER_MODULE_AI * WORDS_PER_MODULE_AI, TabReadWord, &Status)); //8 offset aprés 2 modules inputs 8 bytes

			plogic_names = pConfig->pNames;
			
			for( i = 0; i < pConfig->TaskCount; i++)
			{
			if (i > 0)
				plogic_names++;
			
				if( pConfig->pTasks[plogic_names->TaskIndex].type == ETT_DI)
				{
					for( j=0; j<pConfig->pTasks[plogic_names->TaskIndex].count; j++)
					{   
						if (j > 0)
							plogic_names++;
						sscanf(plogic_names->physical, "MOD%d/P%d.%d", &Module, &Port, &Line);
						pConfig->pTasks[plogic_names->TaskIndex].value = (uint32_t)TabReadByte[((Module-FIRST_MODULE_POSITION_DI)*BYTES_PER_MODULE_DI)+Port];
						Bit = (pConfig->pTasks[plogic_names->TaskIndex].value >> j) & 1;
						pConfig->pTasks[plogic_names->TaskIndex].values[j] = Bit;
					}
				}
				
				if( pConfig->pTasks[plogic_names->TaskIndex].type == ETT_AI)
				{
					for( j=0; j<pConfig->pTasks[plogic_names->TaskIndex].count; j++)
					{   
						if (j > 0)
							plogic_names++;
						sscanf(plogic_names->physical, "MOD%d/P%d.%d", &Module, &Port, &Line);
						analog = (uint32_t)TabReadWord[((Module-FIRST_MODULE_POSITION_AI)*WORDS_PER_MODULE_AI)+j];
						pConfig->pTasks[plogic_names->TaskIndex].valuesAI[j] = analog;
					}
				}
			
				else if( pConfig->pTasks[plogic_names->TaskIndex].type == ETT_DO)
				{
					for( j=0; j<pConfig->pTasks[plogic_names->TaskIndex].count; j++)
					{   
						if (j > 0)
							plogic_names++;
						if (j==0)
						{
							sscanf(plogic_names->physical, "MOD%d/P%d.%d", &Module, &Port, &Line);
							TabWriteByte[((Module-FIRST_MODULE_POSITION_DO)*BYTES_PER_MODULE_DO)+Port] = pConfig->pTasks[plogic_names->TaskIndex].values[j] * pow(2,j);
						}	
						else
						{
						sscanf(plogic_names->physical, "MOD%d/P%d.%d", &Module, &Port, &Line);
						TabWriteByte[((Module-FIRST_MODULE_POSITION_DO)*BYTES_PER_MODULE_DO)+Port] = TabWriteByte[((Module-FIRST_MODULE_POSITION_DO)*BYTES_PER_MODULE_DO)+Port] + (pConfig->pTasks[plogic_names->TaskIndex].values[j] * pow(2,j));
						pConfig->pTasks[plogic_names->TaskIndex].value = (uint32_t)TabWriteByte[((Module-FIRST_MODULE_POSITION_DO)*BYTES_PER_MODULE_DO)+Port];
						}
					}
				}
			
				else if( pConfig->pTasks[plogic_names->TaskIndex].type == PV1_DO)
				{
					for( j=0; j<pConfig->pTasks[plogic_names->TaskIndex].count; j++)
					{   
						if (sscanf(plogic_names->physical, "EV%d.A", &Port) < 0)
							sscanf(plogic_names->physical, "EV%d.B", &Port);
						
						if (Old_Byte != (Port-1)/4) /* check if we will move to next byte */
							Initialize_Byte = 0;
						Old_Byte =  (Port-1)/4 ;
						
						if (j > 0) /* in first Itération don't increment because already incremented last one */
							plogic_names++;
						
						if (Initialize_Byte == 0) /* initialize TabWriteByteCPV with the first value */
						{
							TabWriteBytePV1[(Port-1)/4] = pConfig->pTasks[plogic_names->TaskIndex].values[j] * pow(2,j);
							Initialize_Byte = 1;
						}	
						else
						{
						TabWriteBytePV1[(Port-1)/4] = TabWriteBytePV1[(Port-1)/4] + (pConfig->pTasks[plogic_names->TaskIndex].values[j] * pow(2,j));
						pConfig->pTasks[plogic_names->TaskIndex].value = (uint32_t)TabWriteBytePV1[(Port-1)/4];
						}
					}
					
				}
	
				else if( pConfig->pTasks[plogic_names->TaskIndex].type == PV2_DO)
				{
					for( j=0; j<pConfig->pTasks[plogic_names->TaskIndex].count; j++)
					{   
						if (sscanf(plogic_names->physical, "EV%d.A", &Port) < 0)
							sscanf(plogic_names->physical, "EV%d.B", &Port);
						
						if (Old_Byte != (Port-1)/4) /* check if we will move to next byte */
							Initialize_Byte = 0;
						Old_Byte =  (Port-1)/4 ;
						
						if (j > 0) /* in first Itération don't increment because already incremented last one */
							plogic_names++;
						
						if (Initialize_Byte == 0) /* initialize TabWriteByteCPV with the first value */
						{
							TabWriteBytePV2[((Port-1-16)/4)] = pConfig->pTasks[plogic_names->TaskIndex].values[j] * pow(2,j);
							Initialize_Byte = 1;
						}	
						else
						{
						TabWriteBytePV2[(Port-1-16)/4] = TabWriteBytePV2[((Port-1-16)/4)] + (pConfig->pTasks[plogic_names->TaskIndex].values[j] * pow(2,j));
						pConfig->pTasks[plogic_names->TaskIndex].value = (uint32_t)TabWriteBytePV2[(Port-1-16)/4];
						}
					}
					
				}	
	
				else if( pConfig->pTasks[plogic_names->TaskIndex].type == PV3_DO)
				{
					for( j=0; j<pConfig->pTasks[plogic_names->TaskIndex].count; j++)
					{   
						if (sscanf(plogic_names->physical, "EV%d.A", &Port) < 0)
							sscanf(plogic_names->physical, "EV%d.B", &Port);
						
						if (Old_Byte != (Port-1)/4) /* check if we will move to next byte */
							Initialize_Byte = 0;
						Old_Byte =  (Port-1)/4 ;
						
						if (j > 0) /* in first Itération don't increment because already incremented last one */
							plogic_names++;
						
						if (Initialize_Byte == 0) /* initialize TabWriteByteCPV with the first value */
						{
							TabWriteBytePV3[((Port-1-32)/4)] = pConfig->pTasks[plogic_names->TaskIndex].values[j] * pow(2,j);
							Initialize_Byte = 1;
						}	
						else
						{
						TabWriteBytePV3[(Port-1-32)/4] = TabWriteBytePV3[((Port-1-32)/4)] + (pConfig->pTasks[plogic_names->TaskIndex].values[j] * pow(2,j));
						pConfig->pTasks[plogic_names->TaskIndex].value = (uint32_t)TabWriteBytePV3[(Port-1-32)/4];
						}
					}
					
				}					
		
			}
			EXCCHECKCPLC( IO_WriteQByte (CARTE_PROFIBUS, ADDRESS_PLC, 0, NUMBER_MODULE_D0 * BYTES_PER_MODULE_DO, TabWriteByte, &Status));
			EXCCHECKCPLC( IO_WriteQByte (CARTE_PROFIBUS, ADDRESS_PV1, 0, NUMBER_MODULE_PV1 * BYTES_PER_MODULE_PV1, TabWriteBytePV1, &Status));
			EXCCHECKCPLC( IO_WriteQByte (CARTE_PROFIBUS, ADDRESS_PV2, 0, NUMBER_MODULE_PV2 * BYTES_PER_MODULE_PV2, TabWriteBytePV2, &Status));
			EXCCHECKCPLC( IO_WriteQByte (CARTE_PROFIBUS, ADDRESS_PV3, 0, NUMBER_MODULE_PV3 * BYTES_PER_MODULE_PV3, TabWriteBytePV3, &Status));
			EXCCHECKCPLC( IO_RefreshOutput (CARTE_PROFIBUS, &Status));
		}

		/* check DI changes */
		EXCCHECK( CheckDIChanges(pSensors, pConfig->pTasks, pold_data, pConfig->TaskCount));

		sensorsUnLock(pSensors); 
		is_locked = FALSE;
	};
	
Error:
	if(is_locked) sensorsUnLock(pSensors); 
	ExitThread(THREAD_ERROR_CODE);
	EXCDISPLAY(pexception);
	return 0;

}

/*----------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "EventThreadFunc"
#if defined(_CVI_)
	int CVICALLBACK EventThreadFunc (void *pArguments)
#else
	unsigned __cdecl EventThreadFunc( void* pArguments )
#endif
{
	SElExceptionPtr		pexception = NULL;
	SDIOSensorsPtr		pSensors = ((SEventThreadDataPtr)pArguments)->SDIOSensors;
	int32_t				TaskPos = ((SEventThreadDataPtr)pArguments)->TaskPos;
	int32_t				ValuePos = ((SEventThreadDataPtr)pArguments)->ValuePos;
	SSensorsConfigPtr	pConfig  = (SSensorsConfigPtr)pSensors->_pConfig;
	int32_t				count = 0;
	SensorCallbackFnc_t	*pfunction = NULL;
	int					i;

	if( ValuePos<0)
	{	
		int32_t				count = pConfig->pTasks[TaskPos].fnc.count;
		SensorCallbackFnc_t	*pfunction = pConfig->pTasks[TaskPos].fnc.fCalls;

		for(i=0; i<count; i++, pfunction++)
		{
			EXCDISPLAY( (*pfunction)( pConfig->pTasks[TaskPos].fnc.name, 
									  pConfig->pTasks[TaskPos].value, 
									  pConfig->pTasks[TaskPos].fnc.pData[i]));	
		}	
	}
	else
	{
		int32_t				count = pConfig->pTasks[TaskPos].fncs[ValuePos].count;
		SensorCallbackFnc_t	*pfunction = pConfig->pTasks[TaskPos].fncs[ValuePos].fCalls;
		
		for(i=0; i<count; i++, pfunction++)
		{
			EXCDISPLAY( (*pfunction)( pConfig->pTasks[TaskPos].fncs[ValuePos].name, 
									  (uint32_t)pConfig->pTasks[TaskPos].values[ValuePos],
									  pConfig->pTasks[TaskPos].fncs[ValuePos].pData[i]));	
		}	
	}

	free( (SEventThreadDataPtr)pArguments);
	(SEventThreadDataPtr)pArguments = NULL;

#if defined(_CVI_)
	CmtExitThreadPoolThread (0);
#else
    _endthread( 0 );
#endif
    return 0;
}	/* EventThreadFunc */
