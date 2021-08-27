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
#include "msxmldom.h" 
#include <ansi_c.h>


SElExceptionPtr CalendarCallback(int panel, int day, int month, int year);

#define PATH_PRODUCTION 				".\\data\\production\\%04d.%02d.%02d\\"

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
#undef __FUNC__
#define __FUNC__ "ExtractXMLData"
SElExceptionPtr ExtractXMLData(const char* pfile_xml,int *id, char *name, char *step, char *mesure)
{
	int32_t				error = 0;
	SElExceptionPtr    	pexception = NULL; 
	HRESULT         	hr = 0;
	CAObjHandle     	pxml = 0; 
	CAObjHandle     	pnode = 0;
	VARIANT         	pfile_name_var = CA_VariantEmpty();
	VBOOL            	is_xml_success = 1;
	char         		path[256];
	CAObjHandle     	pparameter = 0;
	CAObjHandle     	pparameter1 = 0;
	CAObjHandle     	pparameter2 = 0;
	CAObjHandle     	pparameter3 = 0;
	CAObjHandle     	pparameter4 = 0;
	CAObjHandle     	pparameter5 = 0;
	CAObjHandle     	pparameter6 = 0;

	char*           	ptext = NULL;
	int            		i = 0;
	int            		j = 0;
	int            		k = 0;
	int 				error_found;

/* configure test sequence from XML */
EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml));
EXCCHECKCOM( CA_VariantSetCString(&pfile_name_var, pfile_xml));
EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (pxml, NULL, pfile_name_var, &is_xml_success));
EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//testseq", &pnode));

error_found = FALSE;

do
{
	sprintf(path, "process[@stage='%d']",++k); 
	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter));
	if(pparameter)
	{
		if (error_found) break; // not necessary see others tests
		i=0;
		do
		{
			sprintf(path, "process[@stage='%d']/test[@step='%d']/failed",k, ++i); 
			EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter));
			if(pparameter)
			{
				EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
				if(ptext && *ptext)
				{
					if (strcmp(ptext, "1") == 0) /* step Failed */
					{
						error_found = TRUE;
						
						sprintf(path, "process[@stage='%d']/test[@step='%d']/name",k, i); 
						EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter1));
						if(pparameter1)
						{
							EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter1, NULL, &ptext));
							if(ptext && *ptext)
							{
							sprintf (name, "%s",ptext);
							CA_FreeMemory(ptext);
							ptext=NULL;
							}
							EXCCHECKCVI( CA_DiscardObjHandle(pparameter1));
							pparameter1 = 0;
						}
					
						sprintf(path, "process[@stage='%d']/test[@step='%d']/description",k , i); 
						EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter2));
						if(pparameter2)
						{
							EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter2, NULL, &ptext));
							if(ptext && *ptext)
							{
							sprintf (step, "%s",ptext);
							CA_FreeMemory(ptext);
							ptext=NULL;
							}
						 
							EXCCHECKCVI( CA_DiscardObjHandle(pparameter2));
							pparameter2 = 0;
						}	
					
						do{
								sprintf(path, "process[@stage='%d']/test[@step='%d']/measure[@step='%d']/failed",k , i, ++j); 
								EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter3));
								if(pparameter3)  /* mesure exist */
								{
									EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter3, NULL, &ptext));
									if(ptext && *ptext)
									{
										if (strcmp(ptext, "1") == 0) /* mesure Failed */
										{
						    				sprintf(path, "process[@stage='%d']/test[@step='%d']/measure[@step='%d']/limit/id",k , i, j);  /* Get ID of Mesure Failed */
				 		    				EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter4));
			      							if(pparameter4)
											{
												EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter4, NULL, &ptext));
												if(ptext && *ptext)
												{
													*id= atoi(ptext);
									 				CA_FreeMemory(ptext);
						     						ptext=NULL;
												}
												EXCCHECKCVI( CA_DiscardObjHandle(pparameter4));
				    							pparameter4 = 0;	
											}
							
						   					sprintf(path, "process[@stage='%d']/test[@step='%d']/measure[@step='%d']/config",k , i, j); /* Get config of Mesure Failed */ 
				 		   					EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter5));
			      							if(pparameter5)
											{
												EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter5, NULL, &ptext));
												if(ptext && *ptext)
												{
													sprintf (mesure, "%s",ptext);
													CA_FreeMemory(ptext);
						     						ptext=NULL;
												}
							
												EXCCHECKCVI( CA_DiscardObjHandle(pparameter5));
				    							pparameter5 = 0;
											}
								
											break;
										}
						
										CA_FreeMemory(ptext);
										ptext=NULL;
									}
							
									EXCCHECKCVI( CA_DiscardObjHandle(pparameter3));
									pparameter3 = 0;
								}
				
								else /* no measure*/
								{
									sprintf(path, "process[@stage='%d']/test[@step='%d']/id",k, i); 
									EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter6));
									if(pparameter6)
									{
										EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter6, NULL, &ptext));
										if(ptext && *ptext)
										{
											*id= atoi(ptext);
											sprintf (mesure, "none");
											CA_FreeMemory(ptext);
											ptext=NULL;
											break;
										}
										EXCCHECKCVI( CA_DiscardObjHandle(pparameter6));
										pparameter6 = 0;
									}
								}
						
							}while(TRUE);	
			
						break;	
					}	
					
					CA_FreeMemory(ptext);
					ptext=NULL;
				}
	
				EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
				pparameter = 0;
			}
	
			else break;
		
		}while(TRUE);
		
		EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
		pparameter = 0;
	}
	
	else break;
		
}while(TRUE);


Error:
	CA_VariantClear(&pfile_name_var);
	if (ptext) CA_FreeMemory(ptext);
	if (pparameter) CA_DiscardObjHandle(pparameter);
	if (pparameter1) CA_DiscardObjHandle(pparameter1);
	if (pparameter2) CA_DiscardObjHandle(pparameter2);
	if (pparameter3) CA_DiscardObjHandle(pparameter3);
	if (pparameter4) CA_DiscardObjHandle(pparameter4);
	if (pparameter5) CA_DiscardObjHandle(pparameter5);
	if (pparameter6) CA_DiscardObjHandle(pparameter6);
	if (pnode) CA_DiscardObjHandle(pnode);
	if (pxml) CA_DiscardObjHandle(pxml);
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef	__FUNC__
#define	__FUNC__ "DisplayFiles"
static SElExceptionPtr DisplayFiles(int panel, int control, int day, int month, int year, EReportMode ReportMode)
{
	SElExceptionPtr pexception = NULL;    
	int32_t         error = 0,
		            iFlag_found;
	int            	bad = 0; 
	int				good = 0;
	int	            i = 1;

	
	WIN32_FIND_DATA FindData;  
	HANDLE 			hDirectory; 
	char			file_filter[256],
					path[256],
					buffer[256],
					*pBarcode = NULL,
					*pResult = NULL,
					*pTime = NULL;
	char            message[128];
	char			FileDirectory[512];
	SDefautPtr		pDefaut[100];
	int id, count,rows;
	char name[256];
	char step[256]; 
	char mesure[256]; 
	int id_found = FALSE;
	int IdCount = 0;
	int panelMessage;
	char pathname[MAX_PATHNAME_LEN];
	char pathfile[MAX_PATHNAME_LEN];
	int	 top_defauts = 15;
	
	GetCtrlVal(panel, PANEL_TOP_DEFAUTS, &top_defauts);
	
	EXCCHECKCVI( DeleteTableRows (panel, PANEL_FILELIST, 1, -1));
	SetCtrlVal(panel, PANEL_BAD, "0");
	SetCtrlVal(panel, PANEL_GOOD, "0");
	ProcessSystemEvents ();
	ProcessDrawEvents ();

	SetCtrlAttribute (panel, PANEL_MESSAGE, ATTR_VISIBLE, 1);
	ProcessSystemEvents ();
	ProcessDrawEvents ();
	
	sprintf(path, PATH_PRODUCTION, year, month, day);
	sprintf(file_filter, PATH_PRODUCTION"*.xml", year, month, day); 
	
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
				if(0==strcmp(pResult,"FAULT"))
				{
					sprintf(path, PATH_PRODUCTION, year, month, day);
					sprintf(FileDirectory, "%s%s", path, FindData.cFileName);
					EXCCHECK(ExtractXMLData(FileDirectory, &id, name, step, mesure));
					
					id_found = FALSE;
					
					for (i=0; i<IdCount; i++)    /* recherche si id exste */
						{
							if (pDefaut[i]->id == id)
							{
								pDefaut[i]->count++;
								id_found = TRUE;
								break;
							}
						}
							
					if (id_found == FALSE)   /* nouveau id */ 
						{
							pDefaut[i] = calloc(1, sizeof(struct _SDefaut));	
							pDefaut[i]->id = id;
							pDefaut[i]->count = 1;
							sprintf (pDefaut[i]->name, "%s", name);
							sprintf (pDefaut[i]->step, "%s", step);
							sprintf (pDefaut[i]->mesure, "%s", mesure);
							IdCount++;
						}	
				   bad++; /* incrementation total défauts */ 
				}
				
				else if(0==strcmp(pResult,"OK"))
				{
				good++;	
				}
			}
		
		} while( FindNextFile( hDirectory, &FindData ) );
		
		FindClose( hDirectory );
	}
		 
	
	SetCtrlAttribute (panel, PANEL_MESSAGE, ATTR_VISIBLE, 0);
	ProcessSystemEvents ();
	ProcessDrawEvents ();
	
	if (bad>0 || good>0)
	{
		if ( IdCount>0 )
		{
		for (i=0; i<IdCount; i++)    /* recherche si id exste */
		{
		
		EXCCHECKCVI( InsertTableRows (panel, PANEL_FILELIST, -1, 1, VAL_CELL_NUMERIC));
		
		SetTableCellAttribute (panel, PANEL_FILELIST, MakePoint( 1, i+1), ATTR_DATA_TYPE, VAL_INTEGER);
		SetTableCellAttribute (panel, PANEL_FILELIST, MakePoint( 2, i+1), ATTR_CELL_TYPE, VAL_CELL_STRING); 
		SetTableCellAttribute (panel, PANEL_FILELIST, MakePoint( 3, i+1), ATTR_CELL_TYPE, VAL_CELL_STRING); 
		SetTableCellAttribute (panel, PANEL_FILELIST, MakePoint( 4, i+1), ATTR_CELL_TYPE, VAL_CELL_STRING); 
		SetTableCellAttribute (panel, PANEL_FILELIST, MakePoint( 5, i+1), ATTR_DATA_TYPE, VAL_INTEGER);	
		
		SetTableCellVal( panel, PANEL_FILELIST, MakePoint( 1, i+1), pDefaut[i]->id); 
		SetTableCellVal( panel, PANEL_FILELIST, MakePoint( 2, i+1), pDefaut[i]->name); 
		SetTableCellVal( panel, PANEL_FILELIST, MakePoint( 3, i+1), pDefaut[i]->step); 
		SetTableCellVal( panel, PANEL_FILELIST, MakePoint( 4, i+1), pDefaut[i]->mesure); 
	   	SetTableCellVal( panel, PANEL_FILELIST, MakePoint( 5, i+1), pDefaut[i]->count); 
		ProcessSystemEvents ();
		ProcessDrawEvents ();
		}
	
		EXCCHECKCVI( SortTableCells (panel, PANEL_FILELIST, VAL_TABLE_ENTIRE_RANGE, VAL_COLUMN_MAJOR, 5, 1, 0, 0));
		if (top_defauts != -1)
		{
			if (IdCount > top_defauts)
				EXCCHECKCVI( DeleteTableRows (panel, PANEL_FILELIST, top_defauts, -1));
		}
		ProcessSystemEvents ();
		ProcessDrawEvents ();
		}
	
		sprintf (message, "%d", bad);
		SetCtrlVal(panel, PANEL_BAD, message);
		sprintf (message, "%d", good);
		SetCtrlVal(panel, PANEL_GOOD, message);
		ProcessSystemEvents ();
		ProcessDrawEvents ();
	}

	else 
		MessagePopup ("Extraction des données", "pas des datas!");
Error:
	for (i=0; i<IdCount; i++)    /* recherche si id exste */
	{
		if (pDefaut[i])	free(pDefaut[i]);	
	}
	EXCRETHROW( pexception);
} 

