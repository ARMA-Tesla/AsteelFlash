/*****************************************************************************
 *
 *  testseq.h
 *
 *  Author: $Author: rykalaz $
 *
 *  $Id: testseq.h,v 1.6 2006/04/28 07:14:51 rykalaz Exp $
 *
 *****************************************************************************/
#if !defined(__EL_TESTSEQ_H__)
#define __EL_TESTSEQ_H__

#include <windows.h>
#include <eltypes.h>
#include <objlist/objlist.h> 
#include <eventfactory/EventFactory.h>
#define EXCLUDE_TEST_MAKROS
#include <test/eltest.h>
#include "Log_Asteel.h"

#ifdef __cplusplus
    extern "C" {
#endif 

/*!
 * Specifies test information
 */ 

#define NB_OF_STATIONS 		6
#define NOT_BUSY 			1
#define BUSY 				0
		
int32_t StatusStations[NB_OF_STATIONS];
int  PickAndPlace ;

typedef enum _ETestSequencePanelMode
{
	E_TSPM_DEFAULT_MODE = 0,
	E_TSPM_STEP_BY_STEP_MODE
} ETestSequencePanelMode;

typedef struct _STestSequence {

    SElExceptionPtr (*Load)(struct _STestSequence* me, const char* path, SObjListPtr pModules, SElEventFactoryPtr pEventFactory);
	SElExceptionPtr (*UnLoad)(struct _STestSequence* me); 
	
	/* test panel fnc*/
	SElExceptionPtr (*IniPanel)(struct _STestSequence* me, const char* panelPictureLeft, const char* panelPictureRight, bool_t reset);
	SElExceptionPtr (*SetPanelDescription)(struct _STestSequence* me, int32_t control, const char* description);
	SElExceptionPtr (*SetPanelWarning)(struct _STestSequence* me, int32_t control, bool_t status);
	SElExceptionPtr (*SetPanelStationStatus)(struct _STestSequence* me, int32_t station, bool_t status);
	SElExceptionPtr (*SetTestPanelReferences)(struct _STestSequence* me, char* RefValeo, char* RefCustomer);
	SElExceptionPtr (*SetPanelMode)(struct _STestSequence* me, int32_t mode);
   	SElExceptionPtr (*SetPanelTime)(struct _STestSequence* me, int32_t station, int32_t time);
	SElExceptionPtr (*SetPanelFault)(struct _STestSequence* me, int32_t station, int32_t station_origin, char* test_description, bool_t status);
	SElExceptionPtr (*GetPalettStatus)(struct _STestSequence* me, int32_t palettID, int32_t *status);
   	SElExceptionPtr (*GetSeqActive)(struct _STestSequence* me, int* UsedSeq);
   	SElExceptionPtr (*GetSeqStopFail)(struct _STestSequence* me, int* SeqStopFail);

	/* testsequence */
	SElExceptionPtr (*RunAll)(struct _STestSequence* me, STestParamPtr pTestParam, bool_t fault_ignore);
	SElExceptionPtr (*RunStation)(struct _STestSequence* me, STestParamPtr pTestParam, bool_t fault_ignore, int32_t	Station_RnR, int32_t Repeat_RnR);
	SElExceptionPtr (*RunSelectedStation)(struct _STestSequence* me, STestParamPtr pTestParam, bool_t fault_ignore, int32_t	Station);
	SElExceptionPtr (*RunSelected)(struct _STestSequence* me, STestParamPtr pTestParam,  int32_t Station, const char* test_name);
	SElExceptionPtr (*RunStationMaster)(struct _STestSequence* me, STestParamPtr pTestParam, bool_t fault_ignore, int32_t Station_Master);
	SElExceptionPtr (*StepStart)(struct _STestSequence* me, STestParamPtr pTestParam, bool_t fault_ignore);
	SElExceptionPtr (*StepNext)(struct _STestSequence* me, bool_t* last_step); 
	SElExceptionPtr (*StepRepeat)(struct _STestSequence* me); 
	SElExceptionPtr (*StepBack)(struct _STestSequence* me); 
	SElExceptionPtr (*StepStop)(struct _STestSequence* me);
	bool_t (*SequenceActive)(struct _STestSequence* me);

	/* maintenance */
	SElExceptionPtr (*TestSequenceHelp)(struct _STestSequence* me, const char* test_name, SELStringPtr pHelp);

	/* validation */
	SElExceptionPtr (*UsageValidation)(struct _STestSequence* me, STestParamPtr pTestParam);     
	SElExceptionPtr (*SetTestActive)(struct _STestSequence* me, int    process_nb, const char* TestName, int32_t TestStep);

 	HANDLE       _hLibrary;   /*!< Handle to Library */
	STestSeqPtr* _pTestsSeq;  /*!< Ini, Cleanup fnc*/
	uint32_t	 _TestSeqAlloc;
    uint32_t     _TestSeqCount;  /*!< Number of Test Seq in _pTestsSeqs array */
    STestPtr**   _pTests;     /*!< All loaded tests */
	uint32_t*	 _TestAlloc;
    uint32_t*    _TestCount;  /*!< Number of Test in _pTest array */
	uint32_t*	 _TestSeqUsed;
	uint32_t*	 _TestSeqStopFail;

	CRITICAL_SECTION LockTest; /*!< Lock shared variable */

	/* step by step mode */
	uint32_t			_StepTestActual;
	STestParamPtr		_StepTestParameter;
	bool_t				_StepTestActive;
	bool_t				_StepIgnoreFault;
	char				Code_AsteelFlash[17+1];
	char				SMB_CHECK[3+1];

	IniTestPanel_t 				_IniTestPanel;
	SetTestPanelDescription_t	_SetTestPanelDescription;
	SetPanelWarning_t			_SetPanelWarning;
	SetTestPanelReferences_t	_SetTestPanelReferences;
	SetTestPanelMode_t			_SetTestPanelMode;
	CloseTestPanel_t 			_CloseTestPanel;
	SetPanelStationStatus_t		_SetPanelStationStatus;
	SetTestPanelTime_t			_SetTestPanelTime;
	SetTestPanelFault_t			_SetTestPanelFault;
	GetPalettStatus_t			_GetPalettStatus;
	GetSeqActive_t				_GetTestSeqActive;
	GetSeqStopFail_t			_GetTestSeqStopFail;

} STestSequence, *STestSequencePtr;

SElExceptionPtr testseq_new(
	STestSequencePtr*  pTestSequence
);
SElExceptionPtr testseq_delete(
    STestSequencePtr* pTestSeq
);

void SaveXmlResultFile(STestParamPtr pTestParam, bool_t failed);
void LogAsteelFile(STestParamPtr pTestParam, bool_t failed);
void InsertionMesures_DB(STestParamPtr pTestParam, bool_t failed);


#define ERROR_TESTSEQUENCE_TEST_NOT_FOUND        	-100001     

#ifdef __cplusplus
    }
#endif 

#endif /* __EL_TESTSEQ_H__ */
