#if !defined(__AUTOMAT_FNC_H__)
#define __AUTOMAT_FNC_H__

#include <test/testseq.h> 
#include <eventfactory/EventFactory.h>
#include <exception/Exception.h> 

#define MAX_CHECKERS	4
#define MAX_OUTPUTS		4


#define PATH_PRODUCTION					"%s\\data\\production\\%04d.%02d.%02d"
#define PATH_QUALITY					"%s\\data\\quality\\%04d.%02d.%02d"
#define PATH_EXPERTISE					"%s\\data\\expertise\\%04d.%02d.%02d"
#define PATH_RNR						"%s\\data\\expertise\\%04d.%02d.%02d"

#define PATH_TEST_FILE_FORM_XML			"\\plugins\\test_form.xml" 
#define PATH_TESTEXP_FILE_FORM_XML		"\\plugins\\testexp_form.xml" 
#define PATH_TESTMASTER_FILE_FORM_XML	"\\plugins\\testmaster_form.xml" 

#define DATE_TIME_FORMAT				"%02d-%02d-%02d %02d:%02d:%02d"

#define STEP_SEQUENCE_REPEAT	0
#define STEP_SEQUENCE_NEXT		1
#define STEP_SEQUENCE_BACK		2
#define STEP_SEQUENCE_CANCEL	3

enum {
	STATION_20 = 0,
	STATION_30,
	STATION_40,
	STATION_50,
	STATION_60,
	STATION_10,
};

typedef enum _EAutomatMode
{
	AUTOMAT_NONE 	= 0,
	AUTOMAT_PRODUCTION,
	AUTOMAT_QUALITY_RETEST,
	AUTOMAT_QUALITY_MASTER,
	AUTOMAT_EXPERTISE_STEP_MODE,
	AUTOMAT_EXPERTISE_GO_MODE,
	AUTOMAT_EXPERTISE_SEQUENCE,
	AUTOMAT_EXPERTISE_RNR_STUDY,
	AUTOMAT_MASTERPART,
	/* size */
	AUTOMAT_MODE_SIZE
} EAutomatMode;

#define AUTOMAT_MODE_STR \
{ \
	"NONE", \
	"PRODUCTION", \
	"QUALITY_RETEST", \
	"QUALITY_MASTER", \
	"EXPERTISE_STEP_MODE", \
	"EXPERTISE_GO_MODE", \
	"EXPERTISE_SEQUENCE", \
	"EXPERTISE_RNR_STUDY", \
	"MASTERPART" \
}

#define AUTOMAT_MODE_DESCRIPTION_STR \
{ \
	"none", \
	"production", \
	"quality retest", \
	"quality master", \
	"expertise", \
	"expertise", \
	"expertise", \
	"expertise", \
	"masterpart" \
}

typedef enum _ETestSeqMode
{
	TESTSEQ_NONE = 0,
	TESTSEQ_PRODUCTION,
	TESTSEQ_EXPERTISE,
	TESTSEQ_MASTERPART
} ETestSeqMode;

typedef enum _EPanelReleaseMode
{
	E_PRM_FALSE = 0,
	E_PRM_NOK,		/* NOK button */
	E_PRM_QUALITY	/* from quality mode */
} EPanelReleaseMode;

#define BARCODE_BUFFER_SIZE		512

typedef struct _SSledge
{
	SFieldLabelPtr		pFieldLabel;
	HANDLE				_hThread;
	HANDLE				_hEventStop;
	CRITICAL_SECTION	_Lock;
	char				barcode[BARCODE_BUFFER_SIZE+1];
	char				PalletID[32];
	STestSequencePtr	pTestSeq;
	STestParamPtr		pTestParam;
	SElEventDataPtr 	pData; 
	int32_t				Station_RnR;
	int32_t				Repeat_RnR;
	int32_t             Palett_RnR;
	int32_t             Part_RnR;
	int32_t             Station_Master;
	uint32_t			Actual_Master_Part;
	int32_t             Skip;
	char				barcode_master_Part[BARCODE_BUFFER_SIZE+1];
	char				name_master_Part[BARCODE_BUFFER_SIZE+1];
	char				SMM[29+1];
	char     			Num_Serie_sFin[20+1];
	char     			Serial_Board_Ver_sFin[9+1];
	char     			Board_Ver_pFin[7+1];
	int                 id_Board_pFin;

		
	/* process sequence functions */
	SElExceptionPtr	(*Activate)(struct _SSledge* me);
	SElExceptionPtr	(*Deactivate)(struct _SSledge* me);
	
} SSledge, *SSledgePtr;

typedef struct _SAutomat
{
	EAutomatMode		Mode;
	ETestSeqMode		TestSeqMode;   
	STestSequencePtr	pTestSeq;
	STestParam			TestParam;
	int32_t				shift_id;
	bool_t				disabled;
	bool_t				masterpart;
	bool_t				pause;
	bool_t				box;
	bool_t				EmtyTable;
	bool_t				isTableEmpty;
	bool_t				ChangeVersion;
	bool_t				MasterPart;

/* settings from xml */
	bool_t				retest_active;
	bool_t				master_active;
	bool_t				box_active;
	bool_t				CIM_PLUS_active;
	bool_t				PRINTER_active;
	bool_t				LASER_active;
	bool_t				CHECK_PCB_active;
	bool_t				EMPTY_TABLE_active;
	bool_t				label_active;
	char				active_printer[1024];
	char				Last_Mvt[128];
	
	EPanelReleaseMode	release_mode;
} SAutomat, *SAutomatPtr;

SElExceptionPtr LoadTestSequence(
	const char* fileXml, 
	STestSequencePtr pTestSeq, 
	SObjListPtr pModules,
	bool_t		reset_panel,
	int32_t*	error
);
SElExceptionPtr RunTestSequence(
	const char* fileFormXml,
    struct _SSledge* me,
	SAutomatPtr pAutomat, 
	SElEventFactoryPtr pEventFactory
);
SElExceptionPtr StepTestSequence(
	const char* fileXml, 
	SAutomatPtr pAutomat, 
	SElEventFactoryPtr pEventFactory, 
	int step_mode,
	bool_t* is_active,
	bool_t* last_step
);

SElExceptionPtr JigPanelLabel(struct _SSledge* me, int32_t* pError);
SElExceptionPtr GetNewSerial(int *counter, int32_t* pError);
DWORD WINAPI Thread_WaitAnimation(LPVOID param);
SElExceptionPtr CloseAnimation(HANDLE* hThread, HANDLE* hStopEvent);



#define MSG_AUTOMAT							100001

#define ERROR_AUTOMAT_NO_LIBRARY_NAME		-100001 
#define ERROR_AUTOMAT_NO_XML_FILE			-100002
#define ERROR_AUTOMAT_NO_JIG_CODE			-100003
#define ERROR_AUTOMAT_TABLE_BUSY			-100004


#endif /* __AUTOMAT_FNC_H__ */

