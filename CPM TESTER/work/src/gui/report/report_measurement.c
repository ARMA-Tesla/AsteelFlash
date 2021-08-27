/****************************************************************************
 *                 T E S T E R     M E A S U R E M E N T
 *---------------------------------------------------------------------------
 *    Copyright  © 2005 ELCOM, a.s.  All Rights Reserved.        
 *---------------------------------------------------------------------------
 *                                                                          
 * Title:       tester_measurement.c                                                    
 * Purpose:     Tester measurement GUI. This GUI displays last and previous
 *              data reports.
 ****************************************************************************/

#define	 _WIN32_WINNT 0x0400     
#include <windows.h> 
#include <time.h>
#include <stdio.h>
#include <toolbox.h>
#include <webviewer/shdocvw.h>
#include <language/language.h>
#include "trans_language.h"
#include "report_measurement.h"
#include "calendar.h"

SElExceptionPtr CalendarCallback(int panel, int day, int month, int year);

#define STYLESHEET_FILE					"xml2html.xsl"
#define STYLESHEET_PATH					".\\data\\xslt\\"
#define PATH_PRODUCTION 				".\\data\\production\\%04d.%02d.%02d\\"
#define PATH_QUALITY	 				".\\data\\quality\\%04d.%02d.%02d\\"
#define PATH_EXPERTISE	 				".\\data\\expertise\\%04d.%02d.%02d\\"

struct _SReportData
{
	int day;
	int month;
	int year;
} SReportData;

static int				gs_IEobj = 0;
extern EReportMode		gv_ReportMode;

static SElExceptionPtr	SetFileListCtrlAttributes(int panel, int control);
static int PrevMonthDays(int month, int year);
static SElExceptionPtr DisplayFiles(int panel, int control, int day, int month, int year, EReportMode ReportMode);
static SElExceptionPtr CreateTmpFile(const char* xmlFile, const char* xsltFile, char tmpFileOutput[], int32_t tmpBufferSize);
static SElExceptionPtr DisplayReport(int panel, int	tableControl, int row);

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "CalendarCallback"
SElExceptionPtr CalendarCallback(int panel, int day, int month, int year)
{
	SElExceptionPtr pexception = NULL; 
	int32_t			error = 0;
	int		ReportMode = 0;

	SReportData.day = day;
	SReportData.month = month;
	SReportData.year = year;

	EXCCHECKCVI( GetCtrlVal( panel, PANEL_MODE, &ReportMode));
	EXCCHECK( DisplayFiles( panel, PANEL_FILELIST, day, month, year, ReportMode));

Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "IniReportPanel"
SElExceptionPtr	IniReportPanel(int panelHandle)
{
	SElExceptionPtr pexception = NULL; 
	int32_t			error = 0;

	EXCCHECKCVI( SetCtrlVal(panelHandle, PANEL_MODE, gv_ReportMode));	

	EXCCHECK( SetFileListCtrlAttributes( panelHandle, PANEL_FILELIST));
	EXCCHECKCVI( GetObjHandleFromActiveXCtrl( panelHandle, PANEL_WEBBROWSER, &gs_IEobj));
	EXCCHECK( InitializeCal( panelHandle, 
							 PANEL_MONTH_DISPLAY, 
							 PANEL_CAL_DISPLAY, 
							 PANEL_MONTH_NEXT,
							 PANEL_MONTH_PREV,
							 -1, -1));

Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef	__FUNC__
#define	__FUNC__ "OnMode"
int CVICALLBACK OnMode(int panel, int control, int event,
	void* callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr pexception = NULL;  
	int32_t			error;
	EReportMode		ReportMode = 0;

	switch (event)
	{
		case EVENT_VAL_CHANGED:
			EXCCHECKCVI( GetCtrlVal( panel, PANEL_MODE, &ReportMode));
			EXCCHECK( DisplayFiles( panel, PANEL_FILELIST, SReportData.day, SReportData.month, SReportData.year, ReportMode));
			break;
	}
	
Error:
	EXCDISPLAY( pexception);
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef	__FUNC__
#define	__FUNC__ "OnFileList"
int CVICALLBACK OnFileList(int panel, int control, int event,
	void* callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr pexception = NULL;  
	int32_t			error;
	Point 			cell;
	
	switch (event)
	{
		case EVENT_LEFT_CLICK:
			EXCCHECKCVI( GetTableCellFromPoint (panel, control, MakePoint(eventData2, eventData1), &cell));
			
			if(cell.x && cell.y)
				EXCCHECK( DisplayReport( panel, control, cell.y));
			break;
	}
	
Error:
	EXCDISPLAY( pexception);
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef	__FUNC__
#define	__FUNC__ "OnLMBack"
int CVICALLBACK OnLMBack (int panel, int control, int event,
	void* callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr pexception = NULL;  
	int32_t			error;
	
	switch (event)
	{
		case EVENT_COMMIT:
			EXCCHECKCVI( HidePanel(panel));
		
			if ( gs_IEobj != 0 )
			{
				EXCCHECKCVI( CA_DiscardObjHandle (gs_IEobj));
				gs_IEobj = 0;
			}
			break;
	}
	
Error:
	EXCDISPLAY( pexception);
	return 0;
} 

/*---------------------------------------------------------------------------*/
#undef	__FUNC__
#define	__FUNC__ "SetFileListCtrlAttributes"
static SElExceptionPtr	SetFileListCtrlAttributes(int panel, int control)
{
	SElExceptionPtr pexception = NULL;    
	int32_t			error = 0;
	
	EXCCHECKCVI( SetCtrlAttribute (panel, control, ATTR_NO_EDIT_TEXT, 1));
	EXCCHECKCVI( SetCtrlAttribute (panel, control, ATTR_ENABLE_COLUMN_SIZING, 0));
	EXCCHECKCVI( SetCtrlAttribute (panel, control, ATTR_ENABLE_ROW_SIZING, 0));

	EXCCHECKCVI( HideBuiltInCtrlMenuItem( panel, control, -2));
	EXCCHECKCVI( HideBuiltInCtrlMenuItem( panel, control, -3));
	EXCCHECKCVI( HideBuiltInCtrlMenuItem( panel, control, -4));

	EXCCHECKCVI( SetTableColumnAttribute (panel, control, 1, ATTR_COLUMN_WIDTH, 193));
	EXCCHECKCVI( SetCtrlAttribute (panel, control, ATTR_NUM_VISIBLE_COLUMNS, 1));	

Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef	__FUNC__
#define	__FUNC__ "DisplayFiles"
static SElExceptionPtr DisplayFiles(int panel, int control, int day, int month, int year, EReportMode ReportMode)
{
	SElExceptionPtr pexception = NULL;    
	int32_t         error = 0;
	int            	row = 1;
	WIN32_FIND_DATA FindData;  
	HANDLE 			hDirectory; 
	char			file_filter[256],
					path[256],
					buffer[256],
					*pBarcode = NULL,
					*pResult = NULL,
					*pTime = NULL;
	
	EXCCHECKCVI( DeleteTableRows (panel, control, 1, -1));

	switch(ReportMode)
	{
		case MODE_PRODUCTION:	
			sprintf(path, PATH_PRODUCTION, year, month, day);
			sprintf(file_filter, PATH_PRODUCTION"*.xml", year, month, day); 
			break;
		case MODE_QUALITY:	
			sprintf(path, PATH_QUALITY, year, month, day);
			sprintf(file_filter, PATH_QUALITY"*.xml", year, month, day); 
			break;
		case MODE_EXPERTISE:	
			sprintf(path, PATH_EXPERTISE, year, month, day);
			sprintf(file_filter, PATH_EXPERTISE"*.xml", year, month, day); 
			break;
	}
	
	hDirectory = FindFirstFileEx( file_filter,
                                  FindExInfoStandard,
                                  &FindData,
                                  FindExSearchNameMatch,
                                  NULL, 0 );
	
	if( hDirectory != INVALID_HANDLE_VALUE )
	{
		do
    	{
			/* parse file name */
			strcpy(buffer, FindData.cFileName);
			pBarcode = strtok(buffer, "_");
			pResult = strtok(NULL, "_");
			pTime = strtok(NULL, "_");

			if(pBarcode && *pBarcode && pResult && *pResult && pTime && *pTime)
			{
				EXCCHECKCVI( InsertTableRows (panel, control, -1, 1, VAL_CELL_STRING));

				if(0==strcmp(pResult,"FAULT"))
					EXCCHECKCVI(SetTableCellAttribute (panel, control, MakePoint (1, row), ATTR_TEXT_COLOR, VAL_RED));

				EXCCHECKCVI( SetTableCellVal( panel, control, MakePoint( 1, row), pBarcode)); 
				EXCCHECKCVI( SetTableCellVal( panel, control, MakePoint( 2, row), FindData.cFileName)); 
				EXCCHECKCVI( SetTableCellVal( panel, control, MakePoint( 3, row), path)); 
				EXCCHECKCVI( SetTableCellVal( panel, control, MakePoint( 4, row), pTime)); 

				row++;
			}	
		} while( FindNextFile( hDirectory, &FindData ) );
		
		FindClose( hDirectory );
	}

	if(row>1)
	{
		EXCCHECKCVI( SetActiveTableCell (panel, control, MakePoint(1,1)));
		EXCCHECKCVI( SortTableCells (panel, control, VAL_TABLE_ENTIRE_RANGE, VAL_COLUMN_MAJOR, 4, 1, 0, 0));
		EXCCHECK( DisplayReport( panel, control, 1));
	}
	
Error:	
	EXCRETHROW( pexception);
} 

/*---------------------------------------------------------------------------*/
#undef	__FUNC__
#define	__FUNC__ "CreateTmpFile"
static SElExceptionPtr CreateTmpFile(
	const char*	xmlFile,
	const char* xsltFile,
	char	tmpFileOutput[],
	int32_t	tmpBufferSize
)
{
	SElExceptionPtr pexception = NULL;       
	int32_t			error = 0;
	char			xml_file_out[MAX_PATH] = "";
	char			xsl_file[MAX_PATH] = "";
	char			buffer[1024*10];
	FILE*			input = NULL;
	FILE*			output = NULL;
	FILE*			xslt = NULL;
	char			imgFiles[MAX_PATH];
	char			imgFile[MAX_PATH];
	char			*pdata = NULL;

	/* copy img file */
	strcpy(imgFiles, xmlFile);
	pdata = strstr(imgFiles, "\\\\");

	if(pdata)
	{
		char imgFileTarget[MAX_PATH] = "";
		HANDLE hFile = 0;
		WIN32_FIND_DATA FindData;

		/* delete old files */
		hFile = FindFirstFile( STYLESHEET_PATH"*.png", &FindData);
		if(hFile!=INVALID_HANDLE_VALUE)
		{
			do
			{
				sprintf(imgFileTarget, STYLESHEET_PATH"%s", FindData.cFileName);
				//EXCCHECKWIN( DeleteFile(imgFileTarget));	
				DeleteFile(imgFileTarget);	
			} while( FindNextFile( hFile, &FindData));
		}

		/* copy new image files */
		pdata = strstr(pdata, "_"); 
		if(pdata)
			strcpy(pdata+1, "*.png"); // ...path...\\barcode_*.png

		hFile = FindFirstFile( imgFiles, &FindData);
		if(hFile!=INVALID_HANDLE_VALUE)
		{
			strcpy(imgFile, xmlFile);
			pdata = strstr(imgFile, "\\\\");

			do
			{
				sprintf(imgFileTarget, STYLESHEET_PATH"%s", FindData.cFileName);
				strcpy(pdata+1, FindData.cFileName);
				//EXCCHECKWIN( CopyFile(imgFile, imgFileTarget));	
				CopyFile(imgFile, imgFileTarget);	
			} while( FindNextFile( hFile, &FindData));
		}				
	}

	sprintf(xsl_file, STYLESHEET_PATH"%s", xsltFile);
	sprintf(xml_file_out, STYLESHEET_PATH"tmp.xml");
	
	if ((input = fopen(xmlFile, "r")) == NULL)
	{
		EXCTHROW( -4, "xml file not opened");
	}
	if ((output = fopen(xml_file_out, "w")) == NULL)
	{
		EXCTHROW( -5, "tmp file not opened!");   
	}

	fprintf( output,"<?xml-stylesheet type=\"text/xsl\" href=\"%s\" ?>\n", xsltFile);

	while (!feof(input))
	{
		size_t	read = 0;
		
		read = fread(buffer, 1, sizeof(buffer), input);
		fwrite(buffer, sizeof(char), read, output);
	}

Error:
	strncpy(tmpFileOutput, xml_file_out, tmpBufferSize);
	if(input) fclose(input);
	if(output) fclose(output);
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef	__FUNC__
#define	__FUNC__ "DisplayReport"
static SElExceptionPtr DisplayReport(
	int panel,
	int	tableControl,
	int row
)
{
	SElExceptionPtr pexception = NULL;       
	int32_t			error = 0;
	HRESULT			hr;
	char			file_name[MAX_PATH]= "";
	char			dir_name[MAX_PATH] = "";
	char			tmp_path_name[MAX_PATH] = "";
    int 			width;
	
	EXCCHECKCVI( GetTableCellVal(panel, tableControl, MakePoint(2, row), file_name));
	EXCCHECKCVI( GetTableCellVal(panel, tableControl, MakePoint(3, row), dir_name));
	
	strcat(dir_name, "\\");
	strcat(dir_name, file_name);
	EXCCHECK( CreateTmpFile( dir_name, STYLESHEET_FILE, tmp_path_name, sizeof(tmp_path_name))); 

	GetCurrentDirectory( sizeof(file_name), file_name);
	strcat(file_name, "\\");
	strcat(file_name, tmp_path_name);
	EXCCHECKCOM( WebViewer_IWebBrowser2Navigate(gs_IEobj, 
				 NULL, file_name, 
				 CA_VariantNULL(), CA_VariantNULL(), 
				 CA_VariantNULL(), CA_VariantNULL()));
#if 0    /* formerly excluded lines */
	
	EXCCHECKCVI( GetPanelAttribute (panel, ATTR_WIDTH, &width));

	WebViewer_IWebBrowser2SetWidth (gs_IEobj, NULL, width);
#endif   /* formerly excluded lines */

Error:
	EXCRETHROW( pexception);
}
