#if !defined(__EA210_UTILS_H__)
#define __EA210_UTILS_H__

#include <test/eltest.h>
#include <eltypes.h>
#include <elerror.h>
#include <utility.h>
#include "c:\work\src\shared\cvi\include\NIIMAQdx.h"
#include <stdlib.h>
//#include <nidaqmx.h>

#ifdef __cplusplus
    extern "C" {
#endif 
		
		
#define MAX_STATIONS			8
		
#define TIMEOUT_DEFAULT			10000
#define TIMEOUT_TABLE			INFINITE
#define TIMEOUT_AXIS			10000
		
int gs_alarm_over_torque_60;

#define TIMEOUT_INIT() 	clock_t	_start_clock = clock()

#define TIMEOUT_CHECK(timeout) \
	if ( ((clock() - _start_clock) / (double)CLOCKS_PER_SEC) > ((double)timeout/1000.0) ) { EXCTHROW( TEST_ERR_TIMEOUT, "Timeout"); } else

#define TIMEOUT_CHECK_DTC(timeout) \
	if ( ((clock() - _start_clock) / (double)CLOCKS_PER_SEC) > ((double)timeout/1000.0) ) { EXCTHROW( -1, "Error check DTC"); } else


enum {
	CAL_TYPE_LINEAR = 1
};


int Write_Traca_MMS_NOK (int Station, STestParamPtr pTestParam);


void SaveGraphPNG(
	int PanelHandle, 
	int ControleHandle, 
	char *PathNamePNG
);

SElExceptionPtr MoveAxis( SaxisPtr paxis, int axis, double target_position, double speed);

SElExceptionPtr CheckAxisPosition( 
	SaxisPtr paxis, 
	int axis, 
	double position, 
	double timeout
);


#if defined(__cplusplus)
    } /* extern "C" */
#endif

#endif /* __EA210_UTILS_H__ */
