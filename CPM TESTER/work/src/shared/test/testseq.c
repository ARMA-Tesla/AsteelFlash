#include <test/testseq.h>
#include <utility.h>
#include <formatio.h>
#include <userint.h>
#include <exception/Exception.h>
#include <elstring.h>
#include <stdio.h>
#include "Log_Asteel.h"



#define         NOK   			0
#define         OK    			1

#define         NO  	 		0
#define         YES    			1

#define         MAX_STATIONS    8

 typedef enum _EStation
{
	STATION_20 = 0,
	STATION_30,
	STATION_40,
	STATION_50,
	STATION_60,
	STATION_10,
	STATION_MAX,
		
} EStation;
	
extern HWND		g_hWnd;

SElExceptionPtr GetTestDescription(
	STestParamPtr	ppar,
	int			process,
	int			step,
	char*		name,
	char*		desc
);

static SElExceptionPtr Load(
        struct _STestSequence* pTestSeq,
        const char* path,
		SObjListPtr pModules,
		SElEventFactoryPtr pEventFactory
);

static SElExceptionPtr UnLoad(
        struct _STestSequence* pTestSeq
);

static SElExceptionPtr SetTestActive(
		struct _STestSequence* me,
		int    process_nb,
		const char* TestName,
		int32_t TestStep
);

static SElExceptionPtr UsageValidation(
        struct _STestSequence* pTestSeq,
		STestParamPtr pTestParam
);
static SElExceptionPtr RunSelectedStation(
        struct _STestSequence* pTestSeq,
		STestParamPtr	pReserved,
		bool_t fault_ignore,
		int32_t	Station
);
static SElExceptionPtr RunAll(
        struct _STestSequence* pTestSeq,
		STestParamPtr	pReserved,
		bool_t fault_ignore
);
void SaveXmlResultFile(
		STestParamPtr pTestParam, 
		bool_t failed
);
static SElExceptionPtr RunSelected(
		struct _STestSequence* me, 
		STestParamPtr	pReserved,
		int32_t		Station,
		const char* test_name
);
static SElExceptionPtr GetSeqActive(
        struct _STestSequence* me,
	    int* UsedSeq
);	
static SElExceptionPtr IniPanel(
        struct _STestSequence* me,
		const char* panelPictureLeft,
		const char* panelPictureRight,
		bool_t reset
);
static SElExceptionPtr SetPanelDescription(
        struct _STestSequence* me,
        int32_t     control,
		const char* description
);
static SElExceptionPtr SetPanelWarning(
        struct _STestSequence* me,
        int32_t     control,
		bool_t status
);
static SElExceptionPtr SetPanelStationStatus(
        struct _STestSequence* me,
        int32_t     station,
		bool_t status
);
static SElExceptionPtr SetPanelTime(
        struct _STestSequence* me,
        int32_t     station,
		int32_t  	time
);
static SElExceptionPtr SetPanelFault(
        struct _STestSequence* me,
        int32_t     station,  
		int32_t     station_origin,
		char* test_description,
		bool_t 		status
);
static SElExceptionPtr SetTestPanelReferences(
        struct _STestSequence* me,
		char* RefValeo,
	    char* RefCustomer
);
static SElExceptionPtr GetPalettStatus(
        struct _STestSequence* me,
		 int32_t	palettID,
		 int32_t	*status
);
static SElExceptionPtr SetPanelMode(
		struct _STestSequence* me, 
		int32_t mode
);
static SElExceptionPtr RunStation(
        struct _STestSequence* pTestSeq,
		STestParamPtr	pReserved,
		bool_t fault_ignore,
		int32_t	Station_RnR, 
		int32_t Repeat_RnR
);
static SElExceptionPtr RunStationMaster(
        struct _STestSequence* pTestSeq,
		STestParamPtr	pReserved,
		bool_t fault_ignore,
		int32_t	Station_Master 
);

static SElExceptionPtr TestSequenceHelp(
		struct _STestSequence* me, 
		const char* test_name, 
		SELStringPtr pHelp
);

static SElExceptionPtr StepStart(
		struct _STestSequence* me, 
		STestParamPtr pTestParam, 
		bool_t fault_ignore
);
static SElExceptionPtr StepNext(
		struct _STestSequence* me, 
		bool_t* last_step
); 

static SElExceptionPtr StepRepeat(
		struct _STestSequence* me
); 

static SElExceptionPtr StepBack(
		struct _STestSequence* me
); 

static SElExceptionPtr StepStop(
		struct _STestSequence* me
);

static bool_t SequenceActive(
		struct _STestSequence* me,
		int*   UsedSeq
);




int compare(const void *a, const void *b) 
{
	STestPtr	test1 = *(STestPtr*)a;
	STestPtr	test2 = *(STestPtr*)b;
	
	return (int)(test1->_Step - test2->_Step);
}
/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "testseq_new"
SElExceptionPtr testseq_new(
	STestSequencePtr*  pTestSequence
)
{
    SElExceptionPtr pexception = NULL;

    *pTestSequence = (STestSequencePtr)calloc(1, sizeof(STestSequence));
    EXCCHECKALLOC( *pTestSequence);    
    (*pTestSequence)->Load = Load;
	(*pTestSequence)->UnLoad = UnLoad;
	(*pTestSequence)->SetTestActive = SetTestActive;
	(*pTestSequence)->UsageValidation = UsageValidation;
	(*pTestSequence)->RunSelectedStation = RunSelectedStation;
	(*pTestSequence)->RunAll = RunAll;
	(*pTestSequence)->RunSelected = RunSelected;
#if 0    /* formerly excluded lines */
	(*pTestSequence)->RunStation = RunStation;
	(*pTestSequence)->RunStationMaster = RunStationMaster;
	(*pTestSequence)->TestSequenceHelp = TestSequenceHelp;
	(*pTestSequence)->StepStart = StepStart;
	(*pTestSequence)->StepNext = StepNext;
	(*pTestSequence)->StepRepeat = StepRepeat;
	(*pTestSequence)->StepBack = StepBack;
	(*pTestSequence)->StepStop = StepStop;
	(*pTestSequence)->SequenceActive = SequenceActive;
#endif   /* formerly excluded lines */
	(*pTestSequence)->IniPanel = IniPanel;
	(*pTestSequence)->SetPanelDescription = SetPanelDescription;
	(*pTestSequence)->SetPanelStationStatus = SetPanelStationStatus;
	(*pTestSequence)->SetPanelWarning = SetPanelWarning;
	(*pTestSequence)->SetTestPanelReferences = SetTestPanelReferences;
	(*pTestSequence)->GetPalettStatus = GetPalettStatus;
	(*pTestSequence)->SetPanelMode = SetPanelMode;
	(*pTestSequence)->SetPanelTime = SetPanelTime;
	(*pTestSequence)->SetPanelFault = SetPanelFault;
	(*pTestSequence)->GetSeqActive = GetSeqActive;

	
Error:
    if ( pexception )
    {
        testseq_delete(pTestSequence);   
    } 
    EXCRETHROW( pexception);
} /* testseq_new */

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "testseq_delete"
SElExceptionPtr testseq_delete(
	STestSequencePtr* pTestSeq
)
{
	SElExceptionPtr pexception = NULL;

	if (pTestSeq && *pTestSeq)
	{
        free((*pTestSeq)->_pTests);
        free(*pTestSeq);
        *pTestSeq = NULL;
	} /* testseq_delete */

//Error:
	EXCRETHROW( pexception);
} /* testseq_delete */

#define SEQ_CALLOC_RESERVE 		12
#define TEST_CALLOC_RESERVE 	32

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "STestSequence::Load"
static SElExceptionPtr Load(
        struct _STestSequence* pTestSeq,
        const char* path,
		SObjListPtr pModules,
		SElEventFactoryPtr pEventFactory
)
{
	SElExceptionPtr pexception = NULL;
	int32_t         i, j, count, countSeq;
	STestSeq		   pprocess;
	STestListItemPtr   pitem;
	GetTestFunctionFcePtr_t 	GetTestFunction = NULL;
	GetTestSeqFunctionFcePtr_t 	GetTestSeqFunction = NULL;
	TestFce_t				pfce = NULL;
	STestPtr*				ptest = NULL;
   	STestSeqPtr*			ptestseq = NULL;
   	
    pTestSeq->_hLibrary = LoadLibrary(path);
    if ( pTestSeq->_hLibrary == (HANDLE)NULL )
    {
		char	buffer[1024];
		int		err = GetLastError();
		
		sprintf(buffer, "Load library failed: \"%s\"", path);
        EXCTHROW( EL_ERROR_OS_SPECIFIC, buffer);
    }
	
	pTestSeq->_IniTestPanel = (IniTestPanel_t) GetProcAddress( pTestSeq->_hLibrary, "IniTestPanel"); 
	pTestSeq->_SetTestPanelDescription = (SetTestPanelDescription_t) GetProcAddress( pTestSeq->_hLibrary, "SetTestPanelDescription");
	pTestSeq->_SetPanelWarning = (SetPanelWarning_t) GetProcAddress( pTestSeq->_hLibrary, "SetPanelWarning");
	pTestSeq->_SetTestPanelReferences = (SetTestPanelReferences_t) GetProcAddress( pTestSeq->_hLibrary, "SetTestPanelReferences");
	pTestSeq->_SetTestPanelMode = (SetTestPanelMode_t) GetProcAddress( pTestSeq->_hLibrary, "SetTestPanelMode");
	pTestSeq->_CloseTestPanel = (CloseTestPanel_t) GetProcAddress( pTestSeq->_hLibrary, "CloseTestPanel"); 
	pTestSeq->_SetPanelStationStatus = (SetPanelStationStatus_t) GetProcAddress( pTestSeq->_hLibrary, "SetPanelStationStatus");
	pTestSeq->_SetTestPanelTime = (SetTestPanelTime_t) GetProcAddress( pTestSeq->_hLibrary, "SetTestPanelTime");
	pTestSeq->_SetTestPanelFault = (SetTestPanelFault_t) GetProcAddress( pTestSeq->_hLibrary, "SetTestPanelFault");
	pTestSeq->_GetPalettStatus = (GetPalettStatus_t) GetProcAddress( pTestSeq->_hLibrary, "GetPalettStatus");
	pTestSeq->_GetTestSeqActive = (GetSeqActive_t) GetProcAddress( pTestSeq->_hLibrary, "GetTestSeqActive"); 
	pTestSeq->_GetTestSeqStopFail = (GetSeqStopFail_t) GetProcAddress( pTestSeq->_hLibrary, "GetTestSeqStopFail"); 
	
	/* Init, Cleanup process */
	GetTestSeqFunction = (GetTestSeqFunctionFcePtr_t) GetProcAddress( pTestSeq->_hLibrary, "GetTestSeqFunction");  
	EXCCHECK( GetTestSeqFunction( -1, NULL, &countSeq));
	pTestSeq->_TestSeqAlloc = (uint32_t)countSeq + SEQ_CALLOC_RESERVE;
    pTestSeq->_TestSeqCount = (uint32_t)countSeq;
	
	pTestSeq->_pTestsSeq = (STestSeqPtr*)calloc( countSeq + SEQ_CALLOC_RESERVE, sizeof(STestSeqPtr));
	EXCCHECKALLOC( pTestSeq->_pTestsSeq);
	pTestSeq->_TestAlloc = calloc( countSeq + SEQ_CALLOC_RESERVE, sizeof(uint32_t));
    EXCCHECKALLOC( pTestSeq->_TestAlloc);
	pTestSeq->_TestCount = calloc( countSeq , sizeof(uint32_t));
    EXCCHECKALLOC( pTestSeq->_TestCount);
	pTestSeq->_TestSeqUsed = calloc( countSeq , sizeof(uint32_t));
    EXCCHECKALLOC( pTestSeq->_TestSeqUsed);
	pTestSeq->_TestSeqStopFail = calloc( countSeq , sizeof(uint32_t));
    EXCCHECKALLOC( pTestSeq->_TestSeqStopFail);

	ptestseq = &(pTestSeq->_pTestsSeq[0]);

	pTestSeq->_pTests = (STestPtr**)calloc( countSeq, sizeof(STestPtr*));

    for (j=0; j<countSeq; j++, ptestseq++)		//j = station 
    {
        EXCCHECK( GetTestSeqFunction( j, &pprocess, NULL));
		
	   *ptestseq = (STestSeqPtr)calloc(1, sizeof(STestSeq));
    	EXCCHECKALLOC( *ptestseq);
	
		(*ptestseq)->InitName = pprocess.InitName;
		(*ptestseq)->CleanupName = pprocess.CleanupName;
		(*ptestseq)->WaitName = pprocess.WaitName;

		pfce = (TestFce_t) GetProcAddress( pTestSeq->_hLibrary, pprocess.InitName);
		(*ptestseq)->Init = pfce;
	
		pfce = (TestFce_t) GetProcAddress( pTestSeq->_hLibrary, pprocess.CleanupName);
		(*ptestseq)->Cleanup = pfce;
		
		pfce = (TestFce_t) GetProcAddress( pTestSeq->_hLibrary, pprocess.WaitName);
		(*ptestseq)->Wait = pfce;
			
		/* Help, Check, Init, Run, Cleanup steps */
    	GetTestFunction = (GetTestFunctionFcePtr_t) GetProcAddress( pTestSeq->_hLibrary, "GetTestFunction");
		count = 0;
    	EXCCHECK( GetTestFunction(j, -1, NULL, &count));
       
    	pTestSeq->_pTests[j] = (STestPtr*)calloc( count + TEST_CALLOC_RESERVE, sizeof(STestPtr));
    	EXCCHECKALLOC( pTestSeq->_pTests[j]);
		pTestSeq->_TestAlloc[j] = (uint32_t)count + TEST_CALLOC_RESERVE;
    	pTestSeq->_TestCount[j] = (uint32_t)count;
		pTestSeq->_TestSeqUsed[j] = 1; //par défaut Sequence active
			
    	ptest = &(pTestSeq->_pTests[j][0]);

    	for ( i = 0; i < count; i++, ptest++ )   // i = test step
    	{
        	EXCCHECK( GetTestFunction(j,  i, &pitem, NULL));
        	EXCCHECK( test_new( ptest));
		
			(*ptest)->test.name = pitem->name;
			(*ptest)->test.HelpName = pitem->HelpName; 
			(*ptest)->test.CheckName = pitem->CheckName; 
			(*ptest)->test.InitName = pitem->InitName; 
			(*ptest)->test.RunName = pitem->RunName; 
			(*ptest)->test.CleanupName = pitem->CleanupName; 
		
        	pfce = (TestFce_t) GetProcAddress( pTestSeq->_hLibrary, pitem->HelpName);
        	EXCCHECK( (*ptest)->SetFunctionPtr( *ptest,
        								 TEST_FCE_TYPE_HELP,
										 pitem->HelpName,
        								 pfce));
        	pfce = (TestFce_t) GetProcAddress( pTestSeq->_hLibrary, pitem->CheckName);
        	EXCCHECK( (*ptest)->SetFunctionPtr( *ptest,
        								 TEST_FCE_TYPE_CHECK,
										 pitem->CheckName,
        								 pfce));
        	pfce = (TestFce_t) GetProcAddress( pTestSeq->_hLibrary, pitem->InitName);
        	EXCCHECK( (*ptest)->SetFunctionPtr( *ptest,
        								 TEST_FCE_TYPE_INIT,
										 pitem->InitName,
        								 pfce));
        	pfce = (TestFce_t) GetProcAddress( pTestSeq->_hLibrary, pitem->RunName);
        	EXCCHECK( (*ptest)->SetFunctionPtr( *ptest,
        								 TEST_FCE_TYPE_RUN,
										 pitem->RunName,
        								 pfce));
			pfce = (TestFce_t) GetProcAddress( pTestSeq->_hLibrary, pitem->CleanupName);
        	EXCCHECK( (*ptest)->SetFunctionPtr( *ptest,
										 TEST_FCE_TYPE_CLEAN,
										 pitem->CleanupName,
										 pfce));
		}
	
	}


Error:
	EXCRETHROW( pexception);
} /* STestSequence::Load */

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "STestSequence::UnLoad"
static SElExceptionPtr UnLoad(
        struct _STestSequence* pTestSeq
)
{
	SElExceptionPtr pexception = NULL;
	uint32_t        i = 0;
	uint32_t        j = 0;
	STestPtr*		ptest = NULL;

	
	if (pTestSeq)
	{
		for(i=0; i<pTestSeq->_TestSeqCount; i++)
			{ 
			ptest = &(pTestSeq->_pTests[i][0]);
			
				for(j=0; j<pTestSeq->_TestCount[i]; j++, ptest++)
	   			 {
	      		  EXCCHECK( test_delete(ptest));
	    		 }
				free( pTestSeq->_pTests[i]);
				pTestSeq->_pTests[i] = NULL;
				pTestSeq->_TestAlloc[i] = 0;
    			pTestSeq->_TestCount[i] = 0;
				pTestSeq->_pTestsSeq[i] = NULL;
			}
			
		pTestSeq->_TestSeqAlloc = 0;
    	pTestSeq->_TestSeqCount = 0;
		pTestSeq->_pTestsSeq = NULL;

		//EXCCHECK( pTestSeq->_CloseTestPanel());
		
		if ( pTestSeq->_hLibrary )
		{
			pTestSeq->_hLibrary = NULL;
			pTestSeq->_IniTestPanel = NULL;
			pTestSeq->_SetTestPanelDescription = NULL;
			pTestSeq->_SetPanelWarning = NULL;
			pTestSeq->_SetTestPanelMode = NULL;
			pTestSeq->_CloseTestPanel = NULL;
			pTestSeq->_SetTestPanelTime = NULL; 
			pTestSeq->_GetTestSeqActive = NULL;
			pTestSeq->_GetTestSeqStopFail = NULL; 
		}
	}
	
Error:
	EXCRETHROW( pexception);
} /* STestSequence::UnLoad */

/*----------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "STestSequence::SetTestActive"
static SElExceptionPtr SetTestActive(
		struct _STestSequence* pTestSeq,
		int    process_nb,
		const char* TestName,
		int32_t TestStep
)
{
	SElExceptionPtr pexception = NULL;
	STestPtr*		ptest = &(pTestSeq->_pTests[process_nb][0]); 
	STestPtr		ptest_new = NULL;
	uint32_t        i = 0;  
	
	if ( ptest )
	{
		for( i = 0; i < pTestSeq->_TestCount[process_nb]; i++, ptest++ )
	    {
	        if(0==strcmp((*ptest)->test.name, TestName))
			{
				if((*ptest)->_IsUsed)  //appel plus que 1 fois test dans la séquence de base
				{
					/* create copy */
					EXCCHECK( (*ptest)->CloneSelf(*ptest, &ptest_new));
					ptest_new->_Step = TestStep;
					ptest_new->_Process = process_nb;

					if(pTestSeq->_TestCount[process_nb]<pTestSeq->_TestAlloc[process_nb])
					{
						pTestSeq->_pTests[process_nb][pTestSeq->_TestCount[process_nb]] = ptest_new;
						pTestSeq->_TestCount[process_nb]++;
					}
					else
					{
						pTestSeq->_pTests[process_nb] = realloc( pTestSeq->_pTests[process_nb], 
																(pTestSeq->_TestAlloc[process_nb] + TEST_CALLOC_RESERVE)*sizeof(STestPtr));
						EXCCHECKALLOC( pTestSeq->_pTests[process_nb]);
						pTestSeq->_TestAlloc[process_nb] += TEST_CALLOC_RESERVE;
						
						pTestSeq->_pTests[process_nb][pTestSeq->_TestCount[process_nb]] = ptest_new;
						pTestSeq->_TestCount[process_nb]++;
					}
				}
				else
				{
					(*ptest)->_IsUsed = TRUE;
					(*ptest)->_Step = TestStep;
					(*ptest)->_Process = process_nb;

				}
				break;
			}
	    }
		if(i == pTestSeq->_TestCount[process_nb])
		{
			char serror[512];

			sprintf(serror, "Testsequence, test not found: %s", TestName);
			EXCTHROW(ERROR_TESTSEQUENCE_TEST_NOT_FOUND, serror);
		}
	}
	
Error:
	EXCRETHROW( pexception);	
}

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "STestSequence::UsageValidation"
static SElExceptionPtr UsageValidation(
        struct _STestSequence* pTestSeq,
		STestParamPtr pTestParam
)
{
	SElExceptionPtr pexception = NULL;
	uint32_t        i = 0;
	uint32_t        j = 0;
	STestPtr*		ptest = NULL;
	SELStringPtr	pstring = NULL;

    if(pTestSeq)
	{
		EXCCHECK( elstring_new(NULL, &pstring)); 

		for(i=0; i<pTestSeq->_TestSeqCount; i++)
		{ 
			ptest = &(pTestSeq->_pTests[i][0]);
			
			for(j=0; j<pTestSeq->_TestCount[i]; j++, ptest++)
	   		 {
				if((*ptest)->_IsUsed)
				{
					pTestParam->process = (*ptest)->_Process;
					pTestParam->step = (*ptest)->_Step;

		        	pexception = (*ptest)->test.Check(pTestParam);
				
					(*ptest)->_CanBeUsed = (pexception==NULL);
				
					if(pexception)
					{
						if(pexception->GetErrorCode(pexception)==TEST_ERROR_NOT_POSIBLE_TEST)
						{	
						EXCCHECK(pstring->Format(pstring, "%s: %s\n", (*ptest)->test.name, pexception->GetErrorMsgPtr(pexception)));
						EXCDELETE(pexception);
						}
						/*
						* If the exception is different from TEST_ERROR_NOT_VALID_TESTER_CONFIGURATION
						* function is abborted.
						*/
						EXCCHECK( pexception); 
					}
				}
			 }
		}
	}

	if (pstring!= NULL && pstring->Size(pstring)>0)
	{
		EXCTHROW( TEST_ERROR_NOT_VALID_TESTER_CONFIGURATION, pstring->GetPtr(pstring)); 
	}

Error:
	elstring_delete( &pstring);
	EXCRETHROW( pexception);	
} /* STestSequence::UsageValidation */


/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "STestSequence::RunSelectedStation"
static SElExceptionPtr RunSelectedStation(
        struct _STestSequence* pTestSeq,
		STestParamPtr	pTestParam,
		bool_t fault_ignore,
		int32_t	process_nb
)
{
	SElExceptionPtr pexception = NULL;
	SElExceptionPtr pexc_table = NULL;
	SElExceptionPtr pexc_cleanup = NULL;
	uint32_t        i = 0;
	uint32_t        j = 0;
	STestPtr*		ptest = &(pTestSeq->_pTests[process_nb][0]);
	STestSeqPtr		ptestseq = pTestSeq->_pTestsSeq[process_nb];
	HANDLE			handle = NULL;
	bool_t			cleanup = FALSE;
	int 			Test, SubTest;
	STestParam		TestParam = {0};
	char            desc[256];
	
	
	pTestParam->process = process_nb;
		
	if(ptestseq)
		EXCCHECK( (ptestseq->Init)(pTestParam));
		
	qsort(pTestSeq->_pTests[process_nb], pTestSeq->_TestCount[process_nb], sizeof(STestPtr), compare);
	
    if (ptest)
	{
		for(i=0; i<pTestSeq->_TestCount[process_nb]; i++, ptest++)
	   	{
			if  ((*ptest)->_IsUsed)
			{
				EXCCHECK( (*ptest)->Run( *ptest, pTestParam, &handle, NULL ));
			}
		}
	}

	if(ptestseq)
		EXCCHECK( (ptestseq->Cleanup)(pTestParam));
		
Error:
	if(ptestseq)  
	{
		pTestParam->pexception = pexception;
		pexc_cleanup = (ptestseq->Cleanup)(pTestParam);
		EXCDELETE( pexc_cleanup);
	}
	EXCRETHROW( pexception);
}

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "STestSequence::RunAll"
static SElExceptionPtr RunAll(
        struct _STestSequence* pTestSeq,
		STestParamPtr	pTestParam,
		bool_t fault_ignore
)
{
	SElExceptionPtr pexception = NULL;
	SElExceptionPtr pexc_test = NULL;
	SElExceptionPtr pexc_test_printer = NULL;
	SElExceptionPtr pexc_cleanup = NULL;
	uint32_t        i = 0;
	uint32_t        j = 0;
	STestPtr*		ptest =  &(pTestSeq->_pTests[0][0]);
	STestSeqPtr		ptestseq = pTestSeq->_pTestsSeq[0];
	HANDLE			handle = NULL;
	bool_t			cleanup = FALSE;
	char			test_name[256] = ""; 
	char			test_description[256] = "unknown"; 	
	char			msg[512] = ""; 
	
	
	pTestParam->pexception = NULL;

 	 if (ptest)
		{
			//printf("%d\n",pTestSeq->_TestSeqCount);
			for(j=0; j<pTestSeq->_TestSeqCount; j++)
			{
				ptestseq = pTestSeq->_pTestsSeq[j];
				pTestParam->process = j;
				ptest = &(pTestSeq->_pTests[j][0]);
				qsort(pTestSeq->_pTests[j], pTestSeq->_TestCount[j], sizeof(STestPtr), compare);

				/* Init Seq */
				StatusStations[j] = BUSY; 
				EXCDISPLAY( (ptestseq->Init)(pTestParam));
		
				/* Run Steps */
				if (pTestParam->Skip_All_Test == FALSE) 
				{
					if (pTestSeq->_TestSeqUsed[j] == 1)  
					{	
						if (pexc_test == NULL)  /* NOTE ! condition pexc_test == NULL pour ne pas exécuter next station if FAIL */
						{
							for(i=0; i<pTestSeq->_TestCount[j]; i++, ptest++)
		  		 			{
								if (pexc_test == NULL)   /* NOTE ! condition pexc_test == NULL pour ne pas exécuter next step in station if FAIL */
								{
									if  ((*ptest)->_IsUsed)
									{
										StatusStations[j] = BUSY; 
										pexc_test =  (*ptest)->Run( *ptest, pTestParam, &handle, NULL);
										pTestParam->pexception = pexc_test;
										if (pexc_test) 
										{
											GetTestDescription(pTestParam, pTestParam->process, pTestParam->step, test_name, test_description);
											sprintf (pTestParam->Name_Fault, "%s - '%s'", test_name, test_description);
											if (pTestSeq->_TestSeqStopFail[j] == 1)
											{
												sprintf (msg, "test_name = %s \r\n test_description = %s",test_name, test_description); 
												MessagePopup("TEST FAIL", msg);
											}
										}
									}
								}
							
							}
						}
						
					}
				}
				
				/* Cleanup Seq */
				cleanup = TRUE;
				(ptestseq->Cleanup)(pTestParam);
				
				/* Wait Seq */
				StatusStations[j] = NOT_BUSY; 
			    (ptestseq->Wait)(pTestParam);
			}
		
		}
	
Error:
	if(ptestseq && !cleanup)  
	{

		pexc_cleanup = (ptestseq->Cleanup)(pTestParam);
		EXCDELETE( pexc_cleanup);
	}
	EXCRETHROW( pexc_test);
}

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "STestSequence::RunSelected"
static SElExceptionPtr RunSelected(
        struct _STestSequence* pTestSeq,
		STestParamPtr pTestParam,
		int32_t	process_nb,
		const char* test_name
)
{
	SElExceptionPtr pexception = NULL;
	uint32_t        i = 0;
	STestPtr*		ptest = &(pTestSeq->_pTests[process_nb][0]);
	STestSeqPtr		ptestseq = pTestSeq->_pTestsSeq[process_nb];
	STestParam		TestParam = {0};
	
    if(ptest)
	{
		for(i=0; i<pTestSeq->_TestCount[process_nb]; i++, ptest++)
	    {
			if(0==strcmp(test_name, (*ptest)->test.name))
			{
				STestParamPtr ptestparam = (pTestParam!=NULL)? pTestParam:&TestParam;

				EXCCHECK( (*ptest)->test.Init( ptestparam));
				EXCCHECK( (*ptest)->test.Run( ptestparam));
				EXCCHECK( (*ptest)->test.Cleanup( ptestparam));
				break;
			}
	    }
	}

Error:
	EXCRETHROW( pexception);	
} 

/*----------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "STestSequence::SequenceActive"
static bool_t SequenceActive(struct _STestSequence* me, int*  UsedSeq) 
{
	return me->_StepTestActive;
}

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "STestSequence::GetSeqActive"
static SElExceptionPtr GetSeqActive(
        struct _STestSequence* pTestSeq,
		int* SeqUsed
)
{
	SElExceptionPtr pexception = NULL;

	if(pTestSeq->_GetTestSeqActive)
		EXCCHECK( pTestSeq->_GetTestSeqActive(SeqUsed)); 

Error:
	EXCRETHROW( pexception);
}
 
/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "STestSequence::GetSeqStopFail"
static SElExceptionPtr GetSeqStopFail(
        struct _STestSequence* pTestSeq,
		int* SeqStopFail
)
{
	SElExceptionPtr pexception = NULL;

	if(pTestSeq->_GetTestSeqStopFail)
		EXCCHECK( pTestSeq->_GetTestSeqStopFail(SeqStopFail)); 

Error:
	EXCRETHROW( pexception);
}

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "STestSequence::IniPanel"
static SElExceptionPtr IniPanel(
        struct _STestSequence* pTestSeq,
		const char* picture_left,
		const char* picture_right,
		bool_t reset
)
{
	SElExceptionPtr pexception = NULL;

	if(pTestSeq->_IniTestPanel)
		EXCCHECK( pTestSeq->_IniTestPanel(picture_left, picture_left, reset)); 

Error:
	EXCRETHROW( pexception);
}

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "STestSequence::SetPanelDescription"
static SElExceptionPtr SetPanelDescription(
        struct _STestSequence* pTestSeq,
        int32_t     control,
		const char* description
)
{
	SElExceptionPtr pexception = NULL;

	if(pTestSeq->_SetTestPanelDescription)
		EXCCHECK( pTestSeq->_SetTestPanelDescription(control, description)); 

Error:
	EXCRETHROW( pexception);
}

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "STestSequence::SetPanelFault"
static SElExceptionPtr SetPanelFault(
        struct _STestSequence* pTestSeq,
    	int32_t     station,
		int32_t     station_origin,
		char*       test_description,
		bool_t      status
)
{
	SElExceptionPtr pexception = NULL;

	if(pTestSeq->_SetTestPanelFault)
		EXCCHECK( pTestSeq->_SetTestPanelFault(station, station_origin, test_description, status)); 

Error:
	EXCRETHROW( pexception);
}
/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "STestSequence::SetPanelTime"
static SElExceptionPtr SetPanelTime(
        struct _STestSequence* pTestSeq,
    	int32_t     station,
		int32_t     time
)
{
	SElExceptionPtr pexception = NULL;

	if(pTestSeq->_SetTestPanelTime)
		EXCCHECK( pTestSeq->_SetTestPanelTime(station, time)); 

Error:
	EXCRETHROW( pexception);
}

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "STestSequence::SetPanelWarning"
static SElExceptionPtr SetPanelWarning(
        struct _STestSequence* pTestSeq,
        int32_t     control,
		bool_t status
)
{
	SElExceptionPtr pexception = NULL;

	if(pTestSeq->SetPanelWarning)
		EXCCHECK( pTestSeq->_SetPanelWarning(control, status)); 

Error:
	EXCRETHROW( pexception);
}

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "STestSequence::SetPanelStationStatus"
static SElExceptionPtr SetPanelStationStatus(
        struct _STestSequence* pTestSeq,
        int32_t     control,
		bool_t status
)
{
	SElExceptionPtr pexception = NULL;

	if(pTestSeq->SetPanelStationStatus && control)
		EXCCHECK( pTestSeq->_SetPanelStationStatus(control, status)); 

Error:
	EXCRETHROW( pexception);
}

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "STestSequence::GetPalettStatus"
static SElExceptionPtr GetPalettStatus(
        struct _STestSequence* pTestSeq,
        int32_t     control,
        int32_t     *status
)
{
	SElExceptionPtr pexception = NULL;

	if(pTestSeq->GetPalettStatus && control)
		EXCCHECK( pTestSeq->_GetPalettStatus(control, status)); 

Error:
	EXCRETHROW( pexception);
}
/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "STestSequence::SetTestPanelReferences"
static SElExceptionPtr SetTestPanelReferences(
        struct _STestSequence* pTestSeq,
    char* 	RefValeo,
    char* 	RefCustomer
)
{
	SElExceptionPtr pexception = NULL;

	/*if(pTestSeq->SetTestPanelReferences)
		EXCCHECK( pTestSeq->_SetTestPanelReferences(RefValeo, RefCustomer)); */

Error:
	EXCRETHROW( pexception);
}
/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "STestSequence::SetPanelMode"
static SElExceptionPtr SetPanelMode(
		struct _STestSequence* pTestSeq, 
		int32_t mode
)
{
	SElExceptionPtr pexception = NULL;

	if(pTestSeq->_SetTestPanelMode)
		EXCCHECK( pTestSeq->_SetTestPanelMode(mode)); 

Error:
	EXCRETHROW( pexception);
}


#if 0    /* formerly excluded lines */
/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "STestSequence::RunStation"
static SElExceptionPtr RunStation(
        struct _STestSequence* pTestSeq,
		STestParamPtr	pTestParam,
		bool_t fault_ignore,
		int32_t	Station_RnR,
		int32_t	Repeat_RnR
)
{
	SElExceptionPtr pexception = NULL;
	SElExceptionPtr pexc_table = NULL;
	uint32_t        i = 0;
	uint32_t        j = 0;
	STestPtr*		ptest = pTestSeq->_pTests;
	STestSeqPtr		ptestseq = pTestSeq->_pTestsSeq;
	HANDLE			handle = NULL;
	bool_t			cleanup = FALSE;
	int 			Test, SubTest;
	int 			First_TestCount_RnR, End_TestCount_RnR;
	
	
	
	qsort(pTestSeq->_pTests, pTestSeq->_TestCount, sizeof(STestPtr), compare);
	
	if(ptestseq)
		EXCCHECK( (ptestseq->Init)(pTestParam));
	
	
	pTestParam->RnR_counter = 0;
	
    if (ptest)
	{
		for(i=0; i<pTestSeq->_TestCount; i++, ptest++)
	   	{
			sscanf((*ptest)->test.name, "%d.%d", &Test, &SubTest);
			//printf ("(*ptest)->test.name = %s\r\n", (*ptest)->test.name);
			
			//printf ("test name : %s | test Used = %d\r\n", (*ptest)->test.name, (*ptest)->_IsUsed);
			
			if  ((*ptest)->_IsUsed)
			{
				/* test RnR */
				if (Station_RnR == Test/10 - 1)
				{
					StatusStations[(Test/10)-1] = BUSY;
					
					if (SubTest == 90) //rotation
						{
							WriteLine (pTestParam->pTicket_CSV, pTestParam->line_CSV, MAX_LENGTH_CSV-1);
							memset(pTestParam->line_CSV, 0X00, MAX_LENGTH_CSV);

							End_TestCount_RnR = i+1; /* decide rotate or not the table when arrived to end test station RnR*/

							if (pTestParam->RnR_counter == Repeat_RnR) /* if all Repeatitions terminated Rotate Table*/
								{
								CloseFile (pTestParam->pTicket_CSV);
								StatusStations[(Test/10)-1] = NOT_BUSY; //station becaome not busy because wait rotation and all test are finished
								pexc_table = ( (*ptest)->Run( *ptest, pTestParam, &handle, NULL ));
									if (pexc_table)
										{
					  					EXCCHECK(pexc_table);
										}
								}
					
							else  /* if Repeatitions not terminated back to first test don't rotate table */
								{
					 			for (j=First_TestCount_RnR; j<End_TestCount_RnR;j++)
									{
									ptest--;
										i--;
									}
								pTestParam->RnR_counter++;
								
								/* unplug and unlock panel */
								pexc_table = pTestSeq->RunSelected(pTestSeq, pTestParam, "60.85");
								}
						}
					
					else if (Station_RnR == 0 && i == pTestSeq->_TestCount-1) //no  SubTest == 90 in Station 10
						{
							/* Run Last Test in station 10 */
							if (pTestParam->RnR_counter == 0 && (*ptest)->_IsUsed)
									{
									pTestParam->RnR_counter += 1; 
									First_TestCount_RnR = i;
									}
					
							if ((*ptest)->_IsUsed)
									StatusStations[(Test/10)-1] = BUSY; 
								pexception = ( (*ptest)->Run( *ptest, pTestParam, &handle, NULL ));
								pTestParam->pexception = pexception;	

							WriteLine (pTestParam->pTicket_CSV, pTestParam->line_CSV, MAX_LENGTH_CSV-1);
							memset(pTestParam->line_CSV, 0X00, MAX_LENGTH_CSV);

							End_TestCount_RnR = i+1; /* decide rotate or not the table when arrived to end test station RnR*/

							if (pTestParam->RnR_counter == Repeat_RnR) /* if all Repeatitions terminated Rotate Table*/
								{
								CloseFile (pTestParam->pTicket_CSV);
								StatusStations[(Test/10)-1] = NOT_BUSY; 
								}
					
							else  /* if Repeatitions not terminated back to first test don't rotate table */
								{
					 			for (j=First_TestCount_RnR; j<End_TestCount_RnR;j++)
									{
									ptest--;
										i--;
									}
								pTestParam->RnR_counter++; 	
								}
						}
				
					else /* station to Run R&R*/
							{
							if (pTestParam->RnR_counter == 0 && (*ptest)->_IsUsed)
									{
									pTestParam->RnR_counter += 1; 
									First_TestCount_RnR = i;
									}
								if ((*ptest)->_IsUsed)
									StatusStations[(Test/10)-1] = BUSY; //busy
								pexception = ( (*ptest)->Run( *ptest, pTestParam, &handle, NULL ));
								pTestParam->pexception = pexception;	
								}	
	
				}
				
				
				/* NO test RnR */	
				else  
				{
					if (SubTest == 90)
					{
					StatusStations[(Test/10)-1] = NOT_BUSY;
					pexc_table = ( (*ptest)->Run( *ptest, pTestParam, &handle, NULL ));
						if (pexc_table)
						{
					  	EXCCHECK(pexc_table);
						}
					}
				}
			}
			
		}
	
		EXCCHECK(pexception);
	}
	

	if(ptestseq)  
	{
		cleanup = TRUE;
		EXCCHECK( (pTestSeq->_pTestsSeq->Cleanup)(pTestParam));
	}

Error:
	if(ptestseq && !cleanup)  
	{
		SElExceptionPtr pexc_cleanup = NULL;
		pexc_cleanup = (pTestSeq->_pTestsSeq->Cleanup)(pTestParam);
		EXCDELETE( pexc_cleanup);
	}
	EXCRETHROW( pexception);
}
#endif   /* formerly excluded lines */


#if 0    /* formerly excluded lines */
/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "STestSequence::RunStationMaster"
static SElExceptionPtr RunStationMaster(
        struct _STestSequence* pTestSeq,
		STestParamPtr	pTestParam,
		bool_t fault_ignore,
		int32_t	Station_Master
)
{
	SElExceptionPtr pexception = NULL;
	SElExceptionPtr pexc_table = NULL;
	uint32_t        i = 0;
	uint32_t        j = 0;
	STestPtr*		ptest = pTestSeq->_pTests;
	STestSeqPtr		ptestseq = pTestSeq->_pTestsSeq;
	HANDLE			handle = NULL;
	bool_t			cleanup = FALSE;
	int 			Test, SubTest; 
	int				actual_station = 0;
	
	
	
	qsort(pTestSeq->_pTests, pTestSeq->_TestCount, sizeof(STestPtr), compare);
	
	if(ptestseq)
		EXCCHECK( (ptestseq->Init)(pTestParam));
	
	
    if (ptest)
	{
		for(i=0; i<pTestSeq->_TestCount; i++, ptest++)
	   	{
			sscanf((*ptest)->test.name, "%d.%d", &Test, &SubTest);
			
			if (actual_station != Test)
				{
					pTestSeq->_SetTestPanelFault((Test/10)-1, Test, NULL, OK);
				}
			
			if  ((*ptest)->_IsUsed)
			{
				
				/* test Master in this Station */
				if (Station_Master -1 == Test/10 - 1)
				{
					if (SubTest == 90)
					{
					StatusStations[(Test/10)-1] = NOT_BUSY; 
					pexception = ( (*ptest)->Run( *ptest, pTestParam, &handle, NULL ));
					if (fault_ignore)
						EXCDELETE(pexception);
					pTestParam->pexception = pexception;
					}
					
					else 
					{
					StatusStations[(Test/10)-1] = BUSY; //busy
					if ((*ptest)->_IsUsed)
						pexception = ( (*ptest)->Run( *ptest, pTestParam, &handle, NULL ));
						if (fault_ignore)
							EXCDELETE(pexception);
						pTestParam->pexception = pexception;
					}
			
				}	
			
				/* don't test Master in this Station  Only Run 90*/	
				else  
				{
					if (SubTest == 90)
					{
					StatusStations[(Test/10)-1] = NOT_BUSY;
					pexc_table = ( (*ptest)->Run( *ptest, pTestParam, &handle, NULL ));
						if (pexc_table)
						{
					  	EXCCHECK(pexc_table);
						}
					}
				}
			}
			
		}
	
		EXCCHECK(pexception);
	}
	

	if(ptestseq)  
	{
		cleanup = TRUE;
		EXCCHECK( (pTestSeq->_pTestsSeq->Cleanup)(pTestParam));
	}

Error:
	if(ptestseq && !cleanup)  
	{
		SElExceptionPtr pexc_cleanup = NULL;
		pexc_cleanup = (pTestSeq->_pTestsSeq->Cleanup)(pTestParam);
		EXCDELETE( pexc_cleanup);
	}
	EXCRETHROW( pexception);
}
#endif   /* formerly excluded lines */

/*----------------------------------------------------------------------------*/ 
/*  <testsequnce>
 *		<test>
 *			<help></help>
 *			<parameter>
 *				<option/>
 *				<name/>
 *				<type/>
 *				<help/>
 *			</parameter>
 *		</test>
 *		...
 *	</testsequence>
 */
#if 0    /* formerly excluded lines */
#undef __FUNC__
#define __FUNC__ "STestSequence::TestSequenceHelp"
static SElExceptionPtr TestSequenceHelp(
		struct _STestSequence* pTestSeq, 
		const char* test_name, 
		SELStringPtr pHelp
)
{
	SElExceptionPtr pexception = NULL;
	uint32_t        i = 0;
	STestPtr*		ptest = pTestSeq->_pTests;
	STestSeqPtr		ptestseq = pTestSeq->_pTestsSeq;
	STestParam		TestParam = {0};
	
    if(pHelp && ptest)
	{
		TestParam.pHelp = pHelp;

		if(test_name && test_name[0]!='\0') /* help for selected test */
		{
			for(i=0; i<pTestSeq->_TestCount; i++, ptest++)
			{
				if(0==strcmp(test_name, (*ptest)->test.name))
				{
					EXCCHECK( pHelp->Format(pHelp, "<test name='%s'>\n", (*ptest)->test.name));
					EXCCHECK( (*ptest)->test.Help(&TestParam));
					EXCCHECK( pHelp->Append(pHelp, "</test>\n"));
					break;
				}
			}
		}
		else /* help for whole testsequence */
		{
			EXCCHECK( pHelp->Append(pHelp, "<testsequence>\n"));

			for(i=0; i<pTestSeq->_TestCount; i++, ptest++)
			{
				EXCCHECK( pHelp->Format(pHelp, "<test name='%s'>\n", (*ptest)->test.name));
				EXCCHECK( (*ptest)->test.Help(&TestParam));
				EXCCHECK( pHelp->Append(pHelp, "</test>\n"));
			}

			EXCCHECK( pHelp->Append(pHelp, "</testsequence>"));
		}
	}

Error:
	EXCRETHROW( pexception);	
} 
#endif   /* formerly excluded lines */

#if 0    /* formerly excluded lines */
/*----------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "STestSequence::StepStart"
static SElExceptionPtr StepStart(struct _STestSequence* pTestSeq, STestParamPtr pTestParam, bool_t fault_ignore)
{
	SElExceptionPtr pexception = NULL;
	STestSeqPtr		ptestseq = pTestSeq->_pTestsSeq;
	
	pTestSeq->_StepTestParameter = pTestParam;
	pTestSeq->_StepTestActive = TRUE;
	pTestSeq->_StepTestActual = 0;
	pTestSeq->_StepIgnoreFault = fault_ignore;
	
	qsort(pTestSeq->_pTests, pTestSeq->_TestCount, sizeof(STestPtr), compare); 
	
	if(ptestseq)
		EXCCHECK( (ptestseq->Init)(pTestParam));
	
Error:
	EXCRETHROW( pexception);	
}
#endif   /* formerly excluded lines */

#if 0    /* formerly excluded lines */
/*----------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "STestSequence::StepNext"
static SElExceptionPtr StepNext(struct _STestSequence* pTestSeq, bool_t* last_step)
{
	SElExceptionPtr pexception = NULL;
	STestPtr*		ptest = pTestSeq->_pTests; 
	uint32_t		i;
	HANDLE			handle = NULL;  
	bool_t 			used = FALSE;
	
	if(pTestSeq->_StepTestActive)
	{
		if (ptest)
		{
			for(i=pTestSeq->_StepTestActual, ptest+=pTestSeq->_StepTestActual; i<pTestSeq->_TestCount; i++, ptest++ )
		    {
				pexception = ( (*ptest)->Run( *ptest, pTestSeq->_StepTestParameter, &handle, &used ));
				
				if(!pTestSeq->_StepIgnoreFault)
				{
					EXCCHECK(pexception);
				}
				
				if(used)
				{	
					pTestSeq->_StepTestActual = i + 1;	
					break;
				}
		    }
			if(pTestSeq->_StepTestActual==pTestSeq->_TestCount)
			{
				if(last_step) *last_step = TRUE;
				EXCCHECK(pTestSeq->StepStop(pTestSeq));
			}
		}
	}
	
Error:
	EXCRETHROW( pexception);	
}
#endif   /* formerly excluded lines */

#if 0    /* formerly excluded lines */
/*----------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "STestSequence::StepBack"
static SElExceptionPtr StepBack(struct _STestSequence* pTestSeq)
{
	SElExceptionPtr pexception = NULL;
	STestPtr*		ptest = pTestSeq->_pTests; 
	uint32_t		i;
	HANDLE			handle = NULL;  
	bool_t 			used = FALSE;
	
	if(pTestSeq->_StepTestActive)
	{
		if (ptest && pTestSeq->_StepTestActual>1)
		{
			pTestSeq->_StepTestActual -= 2;

			for(i=pTestSeq->_StepTestActual, ptest+=pTestSeq->_StepTestActual; i<pTestSeq->_TestCount; i++, ptest++ )
		    {
				pexception = ( (*ptest)->Run( *ptest, pTestSeq->_StepTestParameter, &handle, &used ));
				
				if(!pTestSeq->_StepIgnoreFault)
				{
					EXCCHECK(pexception);
				}
				
				if(used)
				{	
					pTestSeq->_StepTestActual = i + 1;	
					break;
				}
		    }
		}
	}
	
Error:
	EXCRETHROW( pexception);	
}
#endif   /* formerly excluded lines */


#if 0    /* formerly excluded lines */
/*----------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "STestSequence::StepRepeat"
static SElExceptionPtr StepRepeat(struct _STestSequence* pTestSeq)
{
	SElExceptionPtr pexception = NULL;
	STestPtr*		ptest = pTestSeq->_pTests; 
	uint32_t		i;
	HANDLE			handle = NULL;  
	bool_t 			used = FALSE;
	
	if(pTestSeq->_StepTestActive && pTestSeq->_StepTestActual>0)
	{
		if (ptest)
		{
			pTestSeq->_StepTestActual--;
				
			for(i=pTestSeq->_StepTestActual, ptest+=pTestSeq->_StepTestActual; i<pTestSeq->_TestCount; i++, ptest++ )
		    {
				pexception = ( (*ptest)->Run( *ptest, pTestSeq->_StepTestParameter, &handle, &used ));
				
				if(!pTestSeq->_StepIgnoreFault)
				{
					EXCCHECK(pexception);
				}
				
				if(used)
				{	
					pTestSeq->_StepTestActual = i + 1;	
					break;
				}
		    }
		}
	}
	
Error:
	EXCRETHROW( pexception);	
}
#endif   /* formerly excluded lines */

#if 0    /* formerly excluded lines */
/*----------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "STestSequence::StepStop"
static SElExceptionPtr StepStop(struct _STestSequence* pTestSeq)
{
	SElExceptionPtr pexception = NULL;
	STestSeqPtr		ptestseq = pTestSeq->_pTestsSeq;
	HRESULT         hr = 0;   
	VBOOL         	is_xml_success = 1; 
	
	if(pTestSeq->_StepTestActive)
	{
		if(ptestseq)  
			EXCCHECK((pTestSeq->_pTestsSeq->Cleanup)(pTestSeq->_StepTestParameter));
	
		/* save test file */
		SaveXmlResultFile(pTestSeq->_StepTestParameter, 0);
	
		pTestSeq->_StepTestParameter = NULL;
		pTestSeq->_StepTestActive = FALSE;
		pTestSeq->_StepTestActual = 0;
		pTestSeq->_StepIgnoreFault = FALSE;
	}

Error:
	EXCRETHROW( pexception);	
}
#endif   /* formerly excluded lines */

/*----------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "SaveXmlResultFile"
void SaveXmlResultFile(STestParamPtr pTestParam, bool_t failed)
{
	SElExceptionPtr pexception = NULL;
	HRESULT			hr;
	char			pfile_xml[256];
	VARIANT			pfile_name_var = CA_VariantEmpty();

	if(pTestParam && pTestParam->pTicket)
	{
		sprintf(pfile_xml, "%s\\%s_%s_%s.xml",
					pTestParam->TicketDirPath,
					pTestParam->Production_Serial,
					(failed)? "FAULT":"OK",
					pTestParam->Time);
		
		strcpy(pTestParam->TicketFilePath, pfile_xml);
		EXCCHECKCOM( CA_VariantSetCString(&pfile_name_var, pfile_xml));
		EXCCHECKCOM( MSXML_IXMLDOMDocumentsave (pTestParam->pTicket, NULL, pfile_name_var));
		EXCCHECKCOM( CA_DiscardObjHandle(pTestParam->pTicket));
		pTestParam->pTicket = 0;
	}

Error:
	CA_VariantClear(&pfile_name_var);
	EXCDISPLAY(pexception);
}

/*----------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "LogAsteelFile"
void LogAsteelFile(STestParamPtr pTestParam, bool_t failed)
{
	SElExceptionPtr pexception = NULL;
	HRESULT			hr;
	char			pfile_xml[256];
	char			reference[32];
	char			serial[128];
	char			result[32];
	char			date_time[128];
	char			mesures[4100];
	VARIANT			pfile_name_var = CA_VariantEmpty();

	//if(pTestParam && pTestParam->pTicket)
	//{

		SetBreakOnLibraryErrors (0);
		SetBreakOnProtectionErrors(0);
		sprintf(pfile_xml, "%s\\%s_%s_%s.xml",
					pTestParam->TicketDirPath,
					pTestParam->Production_Serial,
					(failed)? "FAULT":"OK",
					pTestParam->Time);
		
		//sprintf(pfile_xml,"c:\\application\\data\\production\\2021.02.03\\204600177_OK_12.10.40.xml" );
		ExtractXMLInformations(pfile_xml,reference,serial,result,date_time);
		Fill_Log_Asteel_Mesures(pfile_xml,mesures);
		Sleep(200);
	    Fct_Save_Result (reference,serial,result,date_time,mesures );

	//}

Error:
	CA_VariantClear(&pfile_name_var);
	EXCDISPLAY(pexception);
}

/*----------------------------------------------------------------------------*/ 
#undef __FUNC__
#define __FUNC__ "InsertionMesures_DB"
void InsertionMesures_DB(STestParamPtr pTestParam, bool_t failed)
{
	SElExceptionPtr pexception = NULL;
	HRESULT			hr;
	char			pfile_xml[256];
	char			nom_mesure[256];
	char			mesure[128];
	char			result[32];
	char			min[32];
	char			max[32];
	
	VARIANT			pfile_name_var = CA_VariantEmpty();

	//if(pTestParam && pTestParam->pTicket)
	//{
		sprintf(pfile_xml, "%s\\%s_%s_%s.xml",
					pTestParam->TicketDirPath,
					pTestParam->Production_Serial,
					(failed)? "FAULT":"OK",
					pTestParam->Time);
		
		
		Fill_DBO_Asteel_Mesures (pfile_xml,nom_mesure,mesure,min,max,result);
		
		
	//}

Error:
	CA_VariantClear(&pfile_name_var);
	EXCDISPLAY(pexception);
}
