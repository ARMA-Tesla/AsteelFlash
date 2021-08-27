#include <userint.h>
#include <time.h> 
#include <test/eltest.h>
#include <sdk/mmsystem.h>
#include <test/test_helper_functions.h>
#include <toolbox.h>
#include "EA210_utils.h" 
#include <multithread/multithread.h>
#include "asynctmr.h"
#include <analysis.h>
#include <rs232.h>
#include <formatio.h>
#include <exception/exception.h>
#include "test_panel.h"

#define AXIS_X  				 0


			 
extern SObjListPtr 	gs_Modules;



#define TIMEOUT_AXIS_INIT() 	clock_t	_start_clock = clock();

#define TIMEOUT_AXIS_POSITION_CHECK(timeout) \
	if ( ((clock() - _start_clock) / (double)CLOCKS_PER_SEC) > (timeout/1000.0) ) { EXCTHROW( -1, "Axis position error"); } else


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SaveGraphPNG"
void SaveGraphPNG(int PanelHandle, int ControleHandle, char *PathNamePNG)
{
	int BitmapId;
	
	GetCtrlDisplayBitmap ( PanelHandle, ControleHandle, 1, &BitmapId);
	SaveBitmapToPNGFile (BitmapId, PathNamePNG); 
	DiscardBitmap (BitmapId);
	
}
	
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "GetNewSerial"
SElExceptionPtr GetNewSerial(SDBSPtr pdbs, int* counter)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	EIdShift		shift_id = 0;
	bool_t			isOpen = FALSE;
	

		EXCCHECK( pdbs->Storage->IsShiftOpen(pdbs->Storage, &isOpen, &shift_id));
		
		if(isOpen)
		{
			EXCCHECK( pdbs->Storage->GetProductCounter(pdbs->Storage, pdbs->TesterProductId, counter));
		}

Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Write_Traca_MMS_NOK"
int Write_Traca_MMS_NOK (int Station, STestParamPtr pTestParam)
{
	SElExceptionPtr pexception = NULL;
	char 			part_number[128];
	char 			lineID[128];
	int 			Year,Month,Day;

	void* pTID = pTestParam;
	
	
Error:
	return 0;
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "CheckAxisPosition"
SElExceptionPtr CheckAxisPosition( SaxisPtr paxis, int axis, double position, double timeout)
{
	SElExceptionPtr pexception = NULL;
	double Currentposition;
	int   status;

	
    TIMEOUT_INIT();
	do{
	paxis->CurrentPosition(paxis, axis, &Currentposition);
	if (Currentposition < position + 0.1 && Currentposition > position - 0.1)
			break;
        TIMEOUT_CHECK(timeout);
    }
    while(TRUE);
	
	
Error:
	EXCRETHROW( pexception);
}


/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "MoveAxis"
SElExceptionPtr MoveAxis( SaxisPtr paxis, int axis, double target_position, double speed)
{
	SElExceptionPtr pexception = NULL;
	double position, Last_position;
	double timeout;

	paxis->EnableAxis(paxis, axis); 
	
	/* Read last postion */
	paxis->CurrentPosition(paxis, axis, &Last_position); 

	/* calculate timeout to go target Angle */
	timeout = ((speed/fabs(target_position-Last_position)) * 1000 ) + 1000;
	
	/* Move axis to target postion */
	paxis->SetPosition(paxis, axis, target_position, speed);
	paxis->SetPosition(paxis, axis, target_position, speed);

    TIMEOUT_AXIS_INIT();
	do{
	paxis->CurrentPosition(paxis, axis, &position); 

	if (position < target_position + 0.1 && position > target_position - 0.1)
			break;
      
	TIMEOUT_AXIS_POSITION_CHECK(timeout);
    }
    while(TRUE);
	
	
Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "CheckDirectory"
SElExceptionPtr CheckDirectory(const char* directory)
{
	int32_t			   	error = 0;
	SElExceptionPtr    	pexception = NULL; 
	int					size;

	EXCCHECKCVI( FileExists (directory, &size));
	if(size==-1)
	{
		/* create directory */	
		EXCCHECKCVI( MakeDir(directory));
	}
	
Error:
	EXCRETHROW( pexception);
}
