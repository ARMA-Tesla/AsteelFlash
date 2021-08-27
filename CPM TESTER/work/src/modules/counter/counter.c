/****************************************************************************
 *                        COUNTER
 *---------------------------------------------------------------------------
 *    Copyright   2005 ELCOM, a.s.  All Rights Reserved.        
 *---------------------------------------------------------------------------
 *                                                                          
 * Title:       counter.h                                                    
 * Purpose:     HW Counter interface
 * Version:     1.0  
 ****************************************************************************/
#include <NIDAQmx.h>
#include "counter.h"
#include "msxmldom.h"
#include <ansi_c.h>
#include <utility.h>
#include <guid.h>

#define MAX_LENGTH_SENSOR_LOGICAL_NAME				64
#define MAX_LENGTH_SENSOR_PHYSICAL_NAME				64

typedef enum _ETaskType
{
	ETT_NONE = 0,
	ETT_FREQ,
	ETT_DO,
	ETT_FREQ_IN,
	ETT_DUTY_IN
} ETaskType;

typedef struct _STaskConfig
{
	char			name[MAX_LENGTH_SENSOR_LOGICAL_NAME+1];
	char			physical[MAX_LENGTH_SENSOR_PHYSICAL_NAME+1];
	char			physical_line[MAX_LENGTH_SENSOR_PHYSICAL_NAME+1];
	TaskHandle		th;
	ETaskType		type;
} STaskConfig,	*STaskConfigPtr;

static SElExceptionPtr counterOpenWithConfiguration(
	SHWCounterPtr	me,
	const char*		filename
);
static SElExceptionPtr counterGenerateFreq( 
	SHWCounterPtr me,
	const char* name,
	double freq,
	double duty
);
static SElExceptionPtr counterMeasFreq( 
	SHWCounterPtr me,
	const char* name,
	double* freq
);
static SElExceptionPtr counterMeasPulseWidth( 
	SHWCounterPtr me,
	const char* name,
	double* duty
);

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "hwcounter_new"
SElExceptionPtr	hwcounter_new(SHWCounterPtr* pCounter)
{
	SElExceptionPtr    pexception = NULL;

	*pCounter = (SHWCounterPtr)calloc( 1, sizeof(SHWCounter));
	EXCCHECKALLOC( *pCounter);

	(*pCounter)->OpenWithConfiguration = counterOpenWithConfiguration;
	(*pCounter)->GenerateFreq	= counterGenerateFreq;
	(*pCounter)->MeasFreq		= counterMeasFreq;
	(*pCounter)->MeasPulseWidth	= counterMeasPulseWidth;
	
	InitializeCriticalSection( &((*pCounter)->_Lock));

Error:
	EXCRETHROW(pexception);	
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "hwcounter_delete"
SElExceptionPtr	hwcounter_delete(SHWCounterPtr* pCounter)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;

	if ( pCounter && *pCounter )
	{
		SHWCounterPtr	me = *pCounter;
		STaskConfigPtr	ptask_config = (STaskConfigPtr)me->_TaskConfig;
		int				i;

		for(i=0; i<me->_TaskConfigSize; i++, ptask_config++)
		{
			if(ptask_config->th)
			{
				EXCCHECKDAQMX( DAQmxClearTask (ptask_config->th));
				ptask_config->th = 0;
				ptask_config->type = ETT_NONE;
			}
		}

		free(me->_TaskConfig);
		me->_TaskConfig = NULL;
		me->_TaskConfigSize = 0;

		DeleteCriticalSection(&me->_Lock);

		free(*pCounter);
		*pCounter = NULL;
	}	

Error:
	EXCRETHROW( pexception);	
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "counterOpenWithConfiguration"
static SElExceptionPtr counterOpenWithConfiguration(
	SHWCounterPtr	me,
	const char*		fileName
)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	HRESULT         hr = 0;
	CAObjHandle     pxml  = 0;
	CAObjHandle     pnode = 0, plist = 0;
	CAObjHandle     pparameter = 0;
	CAObjHandle     pitem = 0;
	const char*     pfile_name = (fileName) ? fileName : "\\counter.xml";
	VBOOL         	is_xml_success;
	VARIANT         pfile_name_var = CA_VariantEmpty();
	char*         	ptext= NULL;
	char			path[512];
	int				i;
	
	EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml));
	EXCCHECKCVI( CA_VariantSetCString(  &pfile_name_var, pfile_name));
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (pxml, NULL, pfile_name_var, &is_xml_success));
	
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//module[@id='"MODULE_COUNTER"']", &pnode) );

	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectNodes (pnode, NULL, "parameters/tasks/task", &plist));   
	if(plist)
	{
		STaskConfigPtr	ptask_config = NULL;
		long			length = 0;	
	
		EXCCHECKMSXML( MSXML_IXMLDOMNodeListGetlength (plist, NULL, &length));
		
		me->_TaskConfigSize = length;
		me->_TaskConfig = calloc( length, sizeof(STaskConfig));
		ptask_config = (STaskConfigPtr)me->_TaskConfig;
	
		EXCCHECKMSXML( MSXML_IXMLDOMNodeListreset (plist, NULL));
		
		for( i=0; i<length; i++, ptask_config++)
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
						strncpy(ptask_config->name, ptext, MAX_LENGTH_SENSOR_LOGICAL_NAME);	 
						CA_FreeMemory(ptext);
						ptext=NULL;
					}
					EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
					pparameter = 0;
				}
				EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pitem, NULL, "@physical", &pparameter));
				if(pparameter)
				{
					EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
					if(ptext)
					{
						strncpy(ptask_config->physical, ptext, MAX_LENGTH_SENSOR_PHYSICAL_NAME);	 
						CA_FreeMemory(ptext);
						ptext=NULL;
					}
					EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
					pparameter = 0;
				}
				EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pitem, NULL, "@physical_line", &pparameter));
				if(pparameter)
				{
					EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
					if(ptext)
					{
						strncpy(ptask_config->physical_line, ptext, MAX_LENGTH_SENSOR_PHYSICAL_NAME);	 
						CA_FreeMemory(ptext);
						ptext=NULL;
					}
					EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
					pparameter = 0;
				}
			}
		}

		ptask_config = (STaskConfigPtr)me->_TaskConfig;
		for( i=0; i<me->_TaskConfigSize; i++, ptask_config++)
		{
			EXCCHECKDAQMX( DAQmxCreateTask( ptask_config->name, &(ptask_config->th)));	
		}
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
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "counterLock"
static SElExceptionPtr counterLock(
	SHWCounterPtr	pCounter
)
{
	EnterCriticalSection( &(pCounter->_Lock));
	return NULL;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "counterUnlock"
static SElExceptionPtr counterUnlock(
	SHWCounterPtr	pCounter
)
{
	LeaveCriticalSection( &(pCounter->_Lock));
	return NULL;	
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "counterGenerateFreq"
static SElExceptionPtr counterGenerateFreq( 
	SHWCounterPtr me,
	const char* name,
	double freq,
	double duty
)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	STaskConfigPtr	ptask_config = (STaskConfigPtr)me->_TaskConfig;
	int				i;

	EXCCHECK( counterLock(me));

	for(i=0; i<me->_TaskConfigSize; i++, ptask_config++)
	{
		if(0==strcmp(ptask_config->name, name))
			break;
	}
	
	if(i==me->_TaskConfigSize)
		EXCTHROW(ERROR_COUNTER_LOGICAL_NAME_NOT_EXIST, "Logical name does not exist!");
	
	if(ptask_config->th)
	{
		EXCCHECKDAQMX( DAQmxStopTask (ptask_config->th));

		if(freq>0.0)
		{
			/* TODO: if freq == 0.0 or 100.0 use physical_line for DO */

			if(duty<0.1) duty = 0.1;
			if(duty>99.9) duty = 99.9;
			duty /= 100.0;

			if(ptask_config->type!=ETT_FREQ)
			{
				if(ptask_config->type!=ETT_NONE)
				{
					EXCCHECKDAQMX( DAQmxStopTask (ptask_config->th));
					EXCCHECKDAQMX( DAQmxClearTask (ptask_config->th));
					ptask_config->th = 0;
					EXCCHECKDAQMX( DAQmxCreateTask (ptask_config->name, &ptask_config->th));
					ptask_config->type = ETT_NONE;
				}

				if(ptask_config->type==ETT_NONE)
				{
					EXCCHECKDAQMX( DAQmxCreateCOPulseChanFreq(ptask_config->th,
															ptask_config->physical,
															ptask_config->name,
															DAQmx_Val_Hz,
															DAQmx_Val_Low,
															0.0,
															(float64)freq,
															(float64)duty));

					EXCCHECKDAQMX( DAQmxCfgImplicitTiming(ptask_config->th, 
															DAQmx_Val_ContSamps, 
															1000));

					ptask_config->type = ETT_FREQ;
				}
			}
			else /* if(ptask_config->type==ETT_FREQ) */
			{
				EXCCHECKDAQMX( DAQmxSetChanAttribute(ptask_config->th, 
													ptask_config->physical, 
													DAQmx_CO_Pulse_Freq, 
													freq));

				EXCCHECKDAQMX( DAQmxSetChanAttribute(ptask_config->th, 
													ptask_config->physical, 
													DAQmx_CO_Pulse_DutyCyc, 
													duty));
			}
			EXCCHECKDAQMX( DAQmxStartTask (ptask_config->th));
		}
	}

Error:
	counterUnlock(me);
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "counterMeasFreq"
static SElExceptionPtr counterMeasFreq( 
	SHWCounterPtr me,
	const char* name,
	double* freq
)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	STaskConfigPtr	ptask_config = (STaskConfigPtr)me->_TaskConfig;
	int				i;
	float64			value;
	int				stop_task = 0;

	if(freq)
		*freq = 0.0;

	EXCCHECK( counterLock(me));

	for(i=0; i<me->_TaskConfigSize; i++, ptask_config++)
	{
		if(0==strcmp(ptask_config->name, name))
			break;
	}
	
	if(i==me->_TaskConfigSize)
		EXCTHROW(ERROR_COUNTER_LOGICAL_NAME_NOT_EXIST, "Logical name does not exist!");
	
	if(ptask_config->th)
	{
		if(ptask_config->type!=ETT_FREQ_IN)
		{
			if(ptask_config->type!=ETT_NONE)
			{
				EXCCHECKDAQMX( DAQmxStopTask (ptask_config->th));
				EXCCHECKDAQMX( DAQmxClearTask (ptask_config->th));
				ptask_config->th = 0;
				EXCCHECKDAQMX( DAQmxCreateTask (ptask_config->name, &ptask_config->th));
				ptask_config->type = ETT_NONE;
			}

			if(ptask_config->type==ETT_NONE)
			{
				EXCCHECKDAQMX( DAQmxCreateCIFreqChan(ptask_config->th,
														ptask_config->physical,
														ptask_config->name,
														2.0, /* min */
														5000.0, /* max */
														DAQmx_Val_Hz, 
														DAQmx_Val_Falling, 
														DAQmx_Val_LowFreq1Ctr, 
														0.001, 
														4, 
														""));

				ptask_config->type = ETT_FREQ_IN;
			}
		}
		
		EXCCHECKDAQMX( DAQmxStartTask (ptask_config->th));
		stop_task = 1;
		EXCCHECKDAQMX( DAQmxReadCounterScalarF64 (ptask_config->th, 2.0, &value, 0));
		
		if(freq) *freq = (double)value;
	}

Error:
	if ( stop_task )
		DAQmxStopTask (ptask_config->th);
	counterUnlock(me);
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "counterMeasPulseWidth"
static SElExceptionPtr counterMeasPulseWidth( 
	SHWCounterPtr me,
	const char* name,
	double* duty
)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	STaskConfigPtr	ptask_config = (STaskConfigPtr)me->_TaskConfig;
	int				i;
	float64			value;

	if(duty)
		*duty = 0.0;

	EXCCHECK( counterLock(me));

	for(i=0; i<me->_TaskConfigSize; i++, ptask_config++)
	{
		if(0==strcmp(ptask_config->name, name))
			break;
	}
	
	if(i==me->_TaskConfigSize)
		EXCTHROW(ERROR_COUNTER_LOGICAL_NAME_NOT_EXIST, "Logical name does not exist!");
	
	if(ptask_config->th)
	{
		if(ptask_config->type!=ETT_DUTY_IN)
		{
			if(ptask_config->type!=ETT_NONE)
			{
				EXCCHECKDAQMX( DAQmxStopTask (ptask_config->th));
				EXCCHECKDAQMX( DAQmxClearTask (ptask_config->th));
				ptask_config->th = 0;
				EXCCHECKDAQMX( DAQmxCreateTask (ptask_config->name, &ptask_config->th));
				ptask_config->type = ETT_NONE;
			}

			if(ptask_config->type==ETT_NONE)
			{
				EXCCHECKDAQMX( DAQmxCreateCIPulseWidthChan(ptask_config->th,
														ptask_config->physical,
														ptask_config->name,
														0.00001, 
														0.1,
														DAQmx_Val_Seconds, 
														DAQmx_Val_Rising,
														""));

				ptask_config->type = ETT_DUTY_IN;
			}
		}
		EXCCHECKDAQMX( DAQmxStartTask (ptask_config->th));
		EXCCHECKDAQMX( DAQmxReadCounterScalarF64 (ptask_config->th, 2.0, &value, 0));
		EXCCHECKDAQMX( DAQmxStopTask (ptask_config->th));
		if(duty) *duty = (double)value;
	}

Error:
	counterUnlock(me);
	EXCRETHROW(pexception);
}
