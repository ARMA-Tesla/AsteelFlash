/****************************************************************************
 *                          I S O 8 6 0 1
 *---------------------------------------------------------------------------
 *    Copyright  © 2005 ELCOM, a.s.  All Rights Reserved.        
 *---------------------------------------------------------------------------
 *                                                                          
 * Title:       iso8601.h                                                    
 * Purpose:     Library used for parsing and scaning date and time strings
 *              acording to ISO 8601 standard
 *
 * Resources:   http://www.ietf.org/rfc/rfc3339.txt
 *              http://www.cl.cam.ac.uk/~mgk25/iso-time.html
 ****************************************************************************/

#include <iso/iso8601.h>
#include <string.h>
#include <elerror.h>

#define MAX_WEEKS_PER_YEAR          53  /* 52 */
#define MIN_WEEKS_PER_YEAR          1

#define MAX_DAYS_PER_YEAR          366 /* 365 */
#define MIN_DAYS_PER_YEAR          1

#define MIN_DAYS_PER_WEEK           1
#define MAX_DAYS_PER_WEEK           7

#define MIN_DAYS_PER_MONTH           1
#define MAX_DAYS_PER_MONTH           31


static int gs_MonthLength[] = { 31,   
                                28, /* normal,  leap year 29 */     
                                31,
                                30,
                                31,
                                30,
                                31,
                                31,
                                30,
                                31,
                                30,
                                31};

/******************************************************************************
 * STATIC NOT EXPORTED FUNCTIONS
 ******************************************************************************/
static int32_t ParseYear( const char* pDateStr, int32_t* pYear);
static int32_t ParseMonthDay( const char* pDateStr, int32_t* pDay);
static int32_t ParseWeekDay( const char* pDateStr, int32_t* pDay);
static int32_t ParseYearDay( const char* pDateStr, int32_t* pDay);
static int32_t ParseMonth( const char* pDateStr, int32_t* pMonth);
static int32_t ParseWeek( const char* pDateStr, int32_t* pWeek);

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "iso8601_calculate_day_of_week"
/*!
 * \brief Function calculates the day of the week for dates on or after
 *        0000-03-01. This function is based on Zeller's Congruence [Zeller]
 *
 * \param   day    [IN] day. 
 * \param   month  [IN] month. 
 * \param   year   [IN] year. Must be in 4 digit format (1976 instead of 76)  
 *    
 * \return day of the week
 *         1 - Monday
 *         2 - Tuesday
 *         3 - Wednesday
 *         4 - Thursday
 *         5 - Friday
 *         6 - Saturday
 *         7 - Sunday           
 */
int32_t iso8601_calculate_day_of_week(
    int32_t day,
    int32_t month,
    int32_t year
)
{
  int cent;
  int wday;

  /* adjust months so February is the last one */
  month -= 2;
  if (month < 1) {
     month += 12;
     --year;
  }
  /* split by century */
  cent = year / 100;
  year %= 100;
  wday = (((26 * month - 2) / 10 + day + year
                    + year / 4 + cent / 4 + 5 * cent) % 7);
  return ( wday ) ? wday : 7;
} /* iso8601_day_of_week */

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "iso8601_calculate_day_of_month"
/*!
 * \brief Function returns number of days in specified month, year
 *
 * \param   month  [IN] month. 
 * \param   year   [IN] year. Must be in 4 digit format (1976 instead of 76)  
 *    
 * \return number of days in specified month. -1 for an error.
 */  
int32_t iso8601_calculate_day_of_month(
    int32_t month,
    int32_t year
)
{
	if ( month < 1 || month > 12 )
		return -1;

	if ( month == 2 && iso8601_leap_year( year)) /* February */
    {
		return (gs_MonthLength[month-1] + 1);
    }
    
    return gs_MonthLength[month-1];
} /* iso8601_calculate_day_of_month */


/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "iso8601_leap_year"
/*!
 * \brief Function calculates if a year is a leap year.
 *
 * \param   year    [IN] year. Must be in 4 digit format (1976 instead of 76)
 *    
 * \return non-zero if year is a leap year.  
 */   
bool_t iso8601_leap_year(int32_t year)
{
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
} /* iso8601_leap_year */


/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "iso8601_calculate_day_of_year"
/*!
 * \brief Function calculates date of the year
 *
 * \param   day    [IN] day. 
 * \param   month  [IN] month. 
 * \param   year   [IN] year. Must be in 4 digit format (1976 instead of 76)  
 *    
 * \return date of the year. 1-365, 1-366 based on year  
 */   
bool_t iso8601_calculate_day_of_year(
    int32_t day,
    int32_t month,
    int32_t year
)
{
    int32_t i, wday = day;

    for( i = 1; i < month; i++ )
    {
		wday += iso8601_calculate_day_of_month( i, year); 
    }

    return wday;
} /* iso8601_leap_year */


/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "iso8601_parse_day"
/*!
 * \brief Function parses date from iso format to date structure
 *
 * \param   pdate    [IN] date in ISO 8601 format.
 *                 Following formats are supported:
 *                 "1995"                   - year 1995
 *                 "199502" or "1995-02"    - year 1995, month 2
 *                 "1997-W01" or "1997W01" - year 1997, first week
 *                 "1997-W01-2" or "1997W012" - year 1997, Tuesday in first week    
 *                 "1995-035" or "1995035"    - year 1995 day 35
 * \return non-zero if year is a leap year.  
 */ 
int32_t iso8601_parse_day(
    const char*       pDateStr,
    SISO8601Date*     pDate
)
{
    int32_t         error = 0;
    const char*     pend = pDateStr;
    int             year, month, mday, yday, wday, week;           
    
    year = month = mday = yday = wday = week = -1;
    
    error = ParseYear( pend, &year);
    CHECKERR( error);
    pend += error;

    error = 0;
    switch ( strlen(pend))
    {
        case 2:  /* 199502 */
            error = ParseMonth( pend, &month);
            break;
        case 3:  /* 1995-02 or  1995035 or 1997W01 */
            if (*pend == '-')
                error = ParseMonth( ++pend, &month);
            else
            if (*pend == 'W')
                error = ParseWeek( ++pend, &week);
            else
                error = ParseYearDay( pend, &yday);
            break;
        case 4:  /* 1995-035 or 1997W012 or 1997-W01 */
            if (*pend == '-')
            {
                if ( *++pend == 'W' )
                    error = ParseWeek( ++pend, &week);
                else
                    error = ParseYearDay( pend, &yday);
            }
            else
            if (*pend == 'W')
            {
                error = ParseWeek( ++pend, &week);
                CHECKERR( error);
                pend += error;
                error = ParseWeekDay( pend, &wday);
            }
            break;
        case 6:  /* 1997-W01-2 or 1997-02-08 */
            if (*pend == '-')
            {
                bool_t  is_full_format = (*++pend != 'W');
                if ( is_full_format )
                    error = ParseMonth( pend, &month);
                else
                    error = ParseWeek( ++pend, &week);
                CHECKERR( error);
                pend += error;
                if (*pend++ == '-')
                {
                    if ( is_full_format )
                        error = ParseMonthDay( pend, &mday);
                    else
                        error = ParseWeekDay( pend, &wday);
                }            
            }     
            break;
    }
    CHECKERR( error);
    pend += error;
    
    if (*pend != '\0')
    {
        CHECKERR( ERROR_ISO_8601_DATE_FORMAT );
    }
    
    if ( month > 0 && year != -1 && mday > 0 )
    {
        wday = iso8601_calculate_day_of_week( mday, month, year);
        yday = iso8601_calculate_day_of_year( mday, month, year);
    }

Error:
    pDate->mday   = mday;
    pDate->month  = month;
    pDate->year   = year;
    pDate->week   = week;
    pDate->wday   = wday;
    pDate->yday   = yday;

    return error;
} /* iso8601_parse_day */


/******************************************************************************
 * STATIC NOT EXPORTED FUNCTIONS
 ******************************************************************************/
/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ParseYear"
/*!
 * \brief Function parses year from the string
 *
 * \param   pDateStr [IN] string staring with year (4 DIGIT)
 * \param   pYear    [OUT] value represents year  
 *    
 * \return if the error value is negative. Positive value means number of 
 *         successfully parsed characters.    
 */ 
static int32_t ParseYear(
    const char* pDateStr,
    int32_t*    pYear
)
{
    int32_t     error = 0;
    const char*  pend = pDateStr;
    uint16_t     value;
    int i;
    static uint16_t s_mult[4] = {1000,100,10,1};

    *pYear = 0;    
    for( i = 0; i < 4; i++ )
    {
        value = (uint16_t)(*(pend+i) - '0');
        if ( value > 9 )
        {
            CHECKERR( ERROR_ISO_8601_DATE_FORMAT );
        }
        *pYear += value * s_mult[i]; 
    }

Error:
     return ( error < 0 ) ? error : 4;
} /* ParseYear */

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ParseYearDay"
/*!
 * \brief Function parses day of year from the string
 *
 * \param   pDateStr [IN] string staring with day of year (3 DIGIT)
 * \param   pDay    [OUT] value represents day of year
 *                        ( 001-365, 001-366 based on year )  
 *    
 * \return if the error value is negative. Positive value means number of 
 *         successfully parsed characters.    
 */ 
static int32_t ParseYearDay(
    const char* pDateStr,
    int32_t*    pDay
)
{
    int32_t     error = 0;
    const char* pend = pDateStr;
    int         d = (*pend - '0') * 100 + (*(pend + 1) - '0') *10 + (*(pend + 2) - '0');

    if ( d > MAX_DAYS_PER_YEAR || d < MIN_DAYS_PER_YEAR )
    {
        CHECKERR( ERROR_ISO_8601_DATE_FORMAT );
    }
    
    *pDay = (int32_t)d;

Error:
    return ( error < 0 ) ? error : 3;
} /* ParseYearDay */

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ParseWeekDay"
/*!
 * \brief Function parses day of week from the string
 *
 * \param   pDateStr [IN] string staring with day of week (1 DIGIT)
 * \param   pDay    [OUT] value represents day of week
 *                        ( 1-7  ; 1 is Monday, 7 is Sunday )  
 *    
 * \return if the error value is negative. Positive value means number of 
 *         successfully parsed characters.    
 */ 
static int32_t ParseWeekDay(
    const char* pDateStr,
    int32_t*    pDay
)
{
    int32_t     error = 0;
    const char* pend = pDateStr;
    int         d = (*pend - '0');

    if ( d > MAX_DAYS_PER_WEEK || d < MIN_DAYS_PER_WEEK )
    {
        CHECKERR( ERROR_ISO_8601_DATE_FORMAT );
    }

    *pDay = (int32_t)d;

Error:
    return ( error < 0 ) ? error : 1;
} /* ParseWeekDay */

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ParseMonthDay"
/*!
 * \brief Function parses day of month from the string
 *
 * \param   pDateStr [IN] string staring with day of month (2 DIGIT)
 * \param   pDay    [OUT] value represents day of month
 *                        ( 01-28, 01-29, 01-30, 01-31 based on month/year )  
 *    
 * \return if the error value is negative. Positive value means number of 
 *         successfully parsed characters.    
 */ 
static int32_t ParseMonthDay(
    const char* pDateStr,
    int32_t*    pDay
)
{
    int32_t     error = 0;
    const char* pend = pDateStr;
    int         d = (*pend - '0') * 10 + (*(pend + 1) - '0');

    if ( d > MAX_DAYS_PER_MONTH || d < MIN_DAYS_PER_MONTH )
    {
        CHECKERR( ERROR_ISO_8601_DATE_FORMAT );
    }
    
    *pDay = (int32_t)d;

Error:
    return ( error < 0 ) ? error : 2;
} /* ParseMonthDay */

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ParseMonth"
/*!
 * \brief Function parses day of month from the string
 *
 * \param   pDateStr [IN] string staring with month of year (2 DIGIT)
 * \param   pMonth   [OUT] value represents month (01-12)  
 *    
 * \return if the error value is negative. Positive value means number of 
 *         successfully parsed characters.    
 */ 
static int32_t ParseMonth(
    const char* pDateStr,
    int32_t*    pMonth
)
{
    int32_t     error = 0;
    const char* pend = pDateStr;
    int         d = (*pend - '0') * 10 + (*(pend + 1) - '0');

    if ( d > 12 || d < 1 )
    {
        CHECKERR( ERROR_ISO_8601_DATE_FORMAT );
    }
    
    *pMonth = (int32_t)d;

Error:
    return ( error < 0 ) ? error : 2;
} /* ParseMonth */

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ParseWeek"
/*!
 * \brief Function parses day of month from the string
 *
 * \param   pDateStr [IN] string staring with week of year (2 DIGIT)
 * \param   pWeek   [OUT] value represents week (01-52, 01-53 based on year)  
 *    
 * \return if the error value is negative. Positive value means number of 
 *         successfully parsed characters.    
 */ 
static int32_t ParseWeek(
    const char* pDateStr,
    int32_t*    pWeek
)
{
    int32_t     error = 0;
    const char* pend = pDateStr;
    int         w = (*pend - '0') * 10 + (*(pend + 1) - '0');

    if ( w > MAX_WEEKS_PER_YEAR || w < MIN_WEEKS_PER_YEAR )
    {
        CHECKERR( ERROR_ISO_8601_DATE_FORMAT );
    }
    
    *pWeek = (int32_t)w;

Error:
    return ( error < 0 ) ? error : 2;
} /* ParseWeek */
