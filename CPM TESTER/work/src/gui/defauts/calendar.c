#define	 _WIN32_WINNT 0x0400     
#include <windows.h> 
#include <ansi_c.h>
#include <language/language.h>
#include <time.h>
#include <userint.h>
#include <iso\iso8601.h>
#include "calendar.h"

#define EVENT_DATE_SELECTED             1002
#define VAL_DEFAULT_COLOR				0x00347B 

static int MONTH_NEXT = 0;
static int MONTH_PREV = 0;
static int MONTH_DISPLAY = 0;
static int CAL_DISPLAY = 0;

extern SElExceptionPtr CalendarCallback(int panel, int day, int month, int year);

static char* gs_days[7] = { _noop("Mo"),
							_noop("Tu"),
							_noop("We"),
							_noop("Th"),
							_noop("Fr"),
							_noop("Sa"),
							_noop("Su") };

static char* gs_months[12] = { _noop("January"),
							   _noop("February"),
							   _noop("March"),
							   _noop("April"),
							   _noop("May"),
							   _noop("June"),
							   _noop("July"),
							   _noop("August"),
							   _noop("September"),
							   _noop("October"),
							   _noop("November"),
							   _noop("December")};

enum {JAN=1, FEB, MAR, APR, MAY, JUN,
      JUL, AUG, SEP, OCT, NOV, DEC};

static int gs_DisplayedMonth = -1;
static int gs_DisplayedYear  = -1;

static SElExceptionPtr	SetCalendarCtrlAttributes(int panel, int control);

/*---------------------------------------------------------------------------*/
#undef	__FUNC__
#define	__FUNC__ "SetFileListCtrlAttributes"
static int PrevMonthDays(int month, int year)
{
	if ( month > 1 )
	{
		return iso8601_calculate_day_of_month( month-1, year); 
	}
	else
	{
		return iso8601_calculate_day_of_month( 12, year-1);
	}
}

/*---------------------------------------------------------------------------*/
#undef	__FUNC__
#define	__FUNC__ "InitializeCal"
SElExceptionPtr	InitializeCal(
	int panel, int labelCtrl, 
	int calCtrl, int monthNext, 
	int monthPrev,
	int sMonth, int sYear)
{
	SElExceptionPtr pexception = NULL;   
	int32_t			error = 0;
	char 			label[256]="";
	int				month,
					year;
	int				aday = 1;
	int				mdays, pmdays, sday, i,col, color;
	unsigned char	d;
	
	MONTH_NEXT = monthNext;
	MONTH_PREV = monthPrev;
	MONTH_DISPLAY = labelCtrl;
	CAL_DISPLAY = calCtrl;

	EXCCHECK( SetCalendarCtrlAttributes( panel, CAL_DISPLAY));

	/*
	 * If the month and year is not specified
	 * get current date
	 */
	if ( sMonth < 0 || gs_DisplayedMonth < 0 || gs_DisplayedYear < 0 )
	{
		time_t	act_time;
		struct tm*	ptm = NULL;

		time(&act_time);
		ptm = localtime(&act_time);
	
		month = ptm->tm_mon + 1;
		year = ptm->tm_year + 1900;
		aday = ptm->tm_mday;
	}
	else
	{
		month = sMonth;
		year = sYear;
	}

	sprintf( label, "%s %d", gettext(gs_months[month-1]), year);
	EXCCHECKCVI( SetCtrlVal( panel, labelCtrl, label)); 
	
	mdays  = iso8601_calculate_day_of_month( month, year); 
	pmdays = PrevMonthDays( month, year); 
	sday   = iso8601_calculate_day_of_week( 1, month, year);
	
	col = ( sday == 0 ) ? 7 : sday;
	i = ( col == 1 ) ? 3 : 2;
	color = VAL_DEFAULT_COLOR;
	
	for( d = 1; !(i == 7 && col ==8); d++, col++ )
	{
		if ( col > 7 )
		{
			col = 1; i++;
		}
		if (d > mdays )
		{
			d = 1;
			color = VAL_PANEL_GRAY;
		}
		EXCCHECKCVI( SetTableCellVal (panel, calCtrl, MakePoint (col, i), d));
		EXCCHECKCVI( SetTableCellAttribute (panel, calCtrl, MakePoint (col, i), ATTR_TEXT_COLOR, color));
		EXCCHECKCVI( SetTableCellAttribute (panel, calCtrl, MakePoint (col, i), ATTR_TEXT_COLOR, color));
		if ( d == aday && color == VAL_DEFAULT_COLOR )
		{
			EXCCHECKCVI( SetTableSelection (panel, calCtrl, MakeRect(col,i,1,1)));
			EXCCHECKCVI( SetActiveTableCell (panel, calCtrl,MakePoint( col, i)));
		}
	} 
	
	col = ( sday == 0 ) ? 7 : ( sday == 1 ) ? 8 : sday;
	for ( d = pmdays; --col > 0; d--)
	{
		EXCCHECKCVI( SetTableCellVal (panel, calCtrl, MakePoint (col, 2), d));
		EXCCHECKCVI( SetTableCellAttribute (panel, calCtrl, MakePoint (col, 2), ATTR_TEXT_COLOR, VAL_PANEL_GRAY));
	} 
	
	gs_DisplayedMonth = month;
	gs_DisplayedYear = year;

	//EXCCHECK( CalendarCallback( panel, (int)aday, month, year));

Error:
	EXCRETHROW( pexception);
} 


/*---------------------------------------------------------------------------*/
#undef	__FUNC__
#define	__FUNC__ "SetCalendarCtrlAttributes"
static SElExceptionPtr	SetCalendarCtrlAttributes(int panel, int control)
{
	SElExceptionPtr pexception = NULL; 
	int32_t			error = 0;   
	int 			i;

	EXCCHECKCVI( SetCtrlAttribute (panel, control, ATTR_NO_EDIT_TEXT, 1));
	EXCCHECKCVI( SetCtrlAttribute (panel, control, ATTR_ENABLE_COLUMN_SIZING, 0));
	EXCCHECKCVI( SetCtrlAttribute (panel, control, ATTR_ENABLE_ROW_SIZING, 0));
	
	EXCCHECKCVI( HideBuiltInCtrlMenuItem( panel, control, -2));
	EXCCHECKCVI( HideBuiltInCtrlMenuItem( panel, control, -3));
	EXCCHECKCVI( HideBuiltInCtrlMenuItem( panel, control, -4));

	for ( i = 1; i <= sizeof(gs_days) / sizeof(gs_days[0]); i++)
	{
		EXCCHECKCVI( SetTableCellVal (panel, control, MakePoint (i, 1), gettext(gs_days[i-1])));
	}

Error:
	EXCRETHROW( pexception);
} 

/*---------------------------------------------------------------------------*/
#undef	__FUNC__
#define	__FUNC__ "OnMonthChange"
int CVICALLBACK OnMonthChange(int panel, int control, int event,
	void* callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr pexception = NULL;      

	switch (event)
	{
		case EVENT_COMMIT:
			if ( control == MONTH_NEXT )
			{
				EXCCHECK( InitializeCal( panel,
										 MONTH_DISPLAY,
										 CAL_DISPLAY,
										 MONTH_NEXT,
										 MONTH_PREV,
										 (gs_DisplayedMonth < 12 ) ? (gs_DisplayedMonth+1) : 1,
										 (gs_DisplayedMonth < 12 ) ? gs_DisplayedYear : gs_DisplayedYear+1));
			}
			else if ( control == MONTH_PREV )
			{
				EXCCHECK( InitializeCal( panel,
										 MONTH_DISPLAY,
										 CAL_DISPLAY,
										 MONTH_NEXT,
										 MONTH_PREV,
										 (gs_DisplayedMonth > 1 ) ? (gs_DisplayedMonth-1) : 12,
										 (gs_DisplayedMonth > 1 ) ? gs_DisplayedYear : gs_DisplayedYear-1)); 
			}
			break;
	}
	
Error:
	EXCDISPLAY( pexception);
	return 0;
} 

/*---------------------------------------------------------------------------*/
#undef	__FUNC__
#define	__FUNC__ "OnCalDisplay"
int CVICALLBACK OnCalDisplay(int panel, int control, int event,
	void* callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	Point			cell;
	static Point	cell_last;

	switch (event)
	{
		case EVENT_LEFT_DOUBLE_CLICK:
		case EVENT_KEYPRESS:
			break;
		case EVENT_SELECTION_CHANGE:
			
			if(cell_last.x && cell_last.y)
			{
				EXCCHECKCVI( SetActiveTableCell (panel, control, cell_last)); 
				{
					Rect sel;
					sel.left   = cell_last.x;
					sel.top    = cell_last.y; 
					sel.height = 1;
					sel.width  = 1;
					EXCCHECKCVI( SetTableSelection (panel, control, sel));
				}
			}
			break;
		case EVENT_LEFT_CLICK:
			/*
			* User select a date in the calendar control, we should detect psoition
			* because control does not selected position now.
			*/
			EXCCHECKCVI( GetTableCellFromPoint (panel, control, MakePoint (eventData2, eventData1), &cell));

			if(cell.x && cell.y>1)
			{	
				EXCCHECKCVI( SetActiveTableCell (panel, control, cell));
				{
					Rect sel;
					sel.left   = cell.x;
					sel.top    = cell.y; 
					sel.height = 1;
					sel.width  = 1;
					EXCCHECKCVI( SetTableSelection (panel, control, sel));
				}
				cell_last = cell;

				EXCCHECKCVI( OnCalDisplay (panel, control, EVENT_DATE_SELECTED, NULL, cell.y, cell.x));
			}
			break;
		case EVENT_DATE_SELECTED:
			/*
			 * New date has been selected
			 */
			 {
			 	unsigned char	day = 0;
			 	int 			month, year;
			 	int				color;
			 	
			 	EXCCHECKCVI( GetTableCellVal (panel, control, MakePoint (eventData2, eventData1), &day));
			 	EXCCHECKCVI( GetTableCellAttribute (panel, control, MakePoint (eventData2, eventData1), ATTR_TEXT_COLOR, &color));
			 	
			 	if ( color == VAL_DEFAULT_COLOR )
			 	{
		 			month = gs_DisplayedMonth;
		 			year  = gs_DisplayedYear;
			 	}
			 	else
			 	{
					if (  eventData1 == 2 ) /* a second row, a month before */
					{
						month = (gs_DisplayedMonth > 1 ) ? (gs_DisplayedMonth-1) : 12;
						year  = (gs_DisplayedMonth > 1 ) ? gs_DisplayedYear : gs_DisplayedYear-1; 
					}
					else /* a last row, a next month */
					{
					 	month = (gs_DisplayedMonth < 12 ) ? (gs_DisplayedMonth+1) : 1;
						year  = (gs_DisplayedMonth < 12 ) ? gs_DisplayedYear : gs_DisplayedYear+1; 
					}
			 	
			 	}
				ProcessSystemEvents ();
				ProcessDrawEvents ();
			 	EXCCHECK( CalendarCallback( panel, (int)day, month, year));
			 }			 
			break;
	}
	
Error:
	EXCDISPLAY( pexception);
	return 0;
} 
