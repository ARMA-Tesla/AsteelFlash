/****************************************************************************
 *                 T E S T E R     M E A S U R E M E N T
 *---------------------------------------------------------------------------
 *    Copyright  © 2005 ELCOM, a.s.  All Rights Reserved.        
 *---------------------------------------------------------------------------
 *                                                                          
 * Title:       tester_measurement.h                                                    
 * Purpose:     Tester measurement GUI. This GUI displays last and previous
 *              data reports.
 ****************************************************************************/

#if !defined(__REPORT_MEASUREMENT_H__)
#define __REPORT_MEASUREMENT_H__


#include <eltypes.h>

#if defined(__cplusplus) || defined(__cplusplus__)
	extern "C" {
#endif

#include <exception/Exception.h>
		
typedef enum _EReportMode 
{
	MODE_PRODUCTION = 0,
	MODE_QUALITY,
	MODE_EXPERTISE
} EReportMode;

typedef struct _SDefaut
{
	int32_t		row;
	int32_t		count;
	int32_t		id;
	char		name[256];
	char		step[256];
	char		mesure[256];
} SDefaut, *SDefautPtr;

/*! Display Panel with last measurement report */
SElExceptionPtr	IniReportPanel(int panelHandle);

#if defined(__cplusplus) || defined(__cplusplus__)
	}
#endif

#endif /* __REPORT_MEASUREMENT_H__ */



	

