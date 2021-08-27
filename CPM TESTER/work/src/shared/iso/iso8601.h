/****************************************************************************
 *                          I S O 8 6 0 1
 *---------------------------------------------------------------------------
 *    Copyright  © 2005 ELCOM, a.s.  All Rights Reserved.        
 *---------------------------------------------------------------------------
 *                                                                          
 * Title:       iso8601.h                                                    
 * Purpose:     Library used for parsing and scaning date and time strings
 *              acording to ISO 8601 standard
 ****************************************************************************/

#if !defined(__ISO8601_H__)
#define __ISO8601_H__

#include <eltypes.h>

typedef struct _SISO8601Date
{
    int8_t     mday;      /*!< month date 01-28, 01-29, 01-30, 01-31 based on
                                month/year */
    int8_t     month;     /*!< month 01-12 */
    int16_t    year;      /*!< Year in 4 digit (date-century date-year) */
    
    int8_t     week;      /*!< 01-52, 01-53 based on year */
    int8_t     wday;      /*!< 1-7  ; 1 is Monday, 7 is Sunday */    
    int16_t    yday;      /*!< 001-365, 001-366 based on year */
} SISO8601Date, *SISO8601DatePtr;


int32_t iso8601_calculate_day_of_week(
    int32_t day,
    int32_t month,
    int32_t year
);

int32_t iso8601_calculate_day_of_month(
    int32_t month,
    int32_t year
);

bool_t iso8601_leap_year(
	int32_t year
);

bool_t iso8601_calculate_day_of_year(
    int32_t day,
    int32_t month,
    int32_t year
);

int32_t iso8601_parse_day(
    const char*       pDateStr,
    SISO8601Date*     pDate
);


#define ERROR_ISO_8601_DATE_FORMAT      -8601

#endif
