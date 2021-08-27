#include <MSComCtrl/MSComCtl2.h>
#include <utility.h> 
#include <stdio.h>
#include <formatio.h>
#include "dbs_global.h"
#include "ring_control.h" 

extern SGlobalPtr		global;
extern int				gs_panel[];
extern int				gs_panelTab[];


static SElExceptionPtr CreateTestLimitIniFile(bool_t to_file, time_t time);
static SElExceptionPtr CreateProductParameterIniFile(bool_t to_file, time_t time); 
static SElExceptionPtr CreateUserListIniFile(bool_t to_file);
static SElExceptionPtr HistoryListGetTime(time_t* time);

extern char* TesterRightsToString(int32_t rights);    

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::PostDeferredDisplayInfo"
void CVICALLBACK PostDeferredDisplayInfo (void *callbackData)
{
	SElExceptionPtr pexception =  NULL;  
	int32_t			error = 0;
	int				top, left; 
	
	//EXCCHECKCVI( GetCtrlAttribute (gs_panel[DBS], DBS_OUTLINE, ATTR_TOP, &top)); 
	//EXCCHECKCVI( GetCtrlAttribute (gs_panel[DBS], DBS_OUTLINE, ATTR_LEFT, &left));
	
	//EXCCHECKCVI( SetPanelAttribute (gs_panelTab[PAGE_FOUR], ATTR_TOP, top));
	//EXCCHECKCVI( SetPanelAttribute (gs_panelTab[PAGE_FOUR], ATTR_LEFT, left)); 
	
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DBS], DBS_BKTREE, ATTR_VISIBLE, FALSE)); 
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DBS], DBS_TEXTBOX, ATTR_VISIBLE, TRUE)); 
	
	EXCCHECKCVI( DisplayPanel (gs_panelTab[PAGE_FOUR]));
		
Error:
	EXCDISPLAY( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::PostDeferredHideInfo"
void CVICALLBACK PostDeferredHideInfo (void *callbackData)
{
	SElExceptionPtr pexception =  NULL;  
	int32_t			error = 0;
	
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DBS], DBS_TEXTBOX, ATTR_VISIBLE, FALSE));  
	EXCCHECKCVI( SetCtrlAttribute (gs_panel[DBS], DBS_BKTREE, ATTR_VISIBLE, TRUE)); 
	
	EXCCHECKCVI( HidePanel (gs_panelTab[PAGE_FOUR])); 
	
Error:
	EXCDISPLAY( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "InfoShow"
SElExceptionPtr InfoShow(void)
{
	int32_t 		error = 0;
	SElExceptionPtr pexception = NULL;
	
    EXCCHECKCVI( PostDeferredCall(PostDeferredDisplayInfo, NULL));

	EXCCHECK( RingGroupAttribute(RING_GROUPS_LOCK));

Error:
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "InfoHide"
SElExceptionPtr InfoHide(void)
{
	int32_t 		error = 0;
	SElExceptionPtr pexception = NULL;
	
	EXCCHECKCVI( PostDeferredCall(PostDeferredHideInfo, NULL));
	
	EXCCHECK( RingGroupAttribute(RING_GROUPS_UNLOCK)); 
	
Error:
	EXCRETHROW(pexception);     
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Database Plugin}::OnIniLimits"
int CVICALLBACK OnIniLimits (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int32_t 		error = 0;
	SElExceptionPtr pexception = NULL; 
	bool_t			value;
	time_t			time;
	
	switch (event)
		{
		case EVENT_COMMIT:
			
			EXCCHECKCVI( GetCtrlVal(panel, TABPANEL_4_TO_FILE, &value));
			EXCCHECK( HistoryListGetTime(&time));
			
			switch(control)
			{
				case TABPANEL_4_INI_LIMIT:
					EXCCHECK( CreateTestLimitIniFile(value, 0) );
					break;
				case TABPANEL_4_INI_PARAMETER:
					EXCCHECK( CreateProductParameterIniFile(value, 0) );
					break;
				case TABPANEL_4_INI_LIMIT_HIST:
					EXCCHECK( CreateTestLimitIniFile(value, time) );
					break;
				case TABPANEL_4_INI_PARAMETER_HIST:
					EXCCHECK( CreateProductParameterIniFile(value, time) );
					break;
				case TABPANEL_4_INI_USER:
					EXCCHECK( CreateUserListIniFile(value) );
					break;
			}		
			break;
		}

Error:	
	EXCDISPLAY( pexception);
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "CreateTestLimitIniFile"
static SElExceptionPtr CreateTestLimitIniFile(bool_t to_file, time_t time)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	SETestPtr		etest = NULL;
	int32_t			etestSize = 0;
	bool_t			valid;
	int32_t			product_id = global->product_id;
	char			message[1024];
	char*         	product_name;
	char         	pathtmp[MAX_PATHNAME_LEN];
	char         	pathini[MAX_PATHNAME_LEN]; 
	FILE*			file = NULL;
	int            	result;
	int            	i, j, size;
	char			desc[DBS_RECORD_LENGHT_DESCRIPTION+1];
	char*			pletter;
	char			path[256]; 
	char*			pdata = NULL;
	int				process_nb = 0;
	SELineProcessPtr pprocess = NULL;
	int32_t			process_size = 0;
	int				process_count = 0;

	EXCCHECK( PPRODUCT->ProductIsValidEndProduct(PPRODUCT, product_id, &valid) );

	if(valid)
	{
		/* create file */
		EXCCHECK( PPRODUCT->ProductGetIdName(PPRODUCT, product_id, &product_name) ); 
		GetProjectDir (pathtmp);
		strcat(pathtmp, "\\limits_");
		strcat(pathtmp, product_name);
		strcat(pathtmp, ".tmp");

		result = DeleteFile (pathtmp);
		file = fopen(pathtmp,"w");
	
		/* <process> */
		for(process_count=0; process_count<process_size; process_count++)
		{
	
		process_nb++;
			
		EXCCHECK( PLINEEXPORT->GetProcessSeq(PLINEEXPORT, product_id, &pprocess, &process_size));

		EXCCHECK(PEXPORT->GetTestSeq(PEXPORT, product_id, pprocess[process_count].process_nb, time, &etest, &etestSize)); 

		EXCCHECK( PPRODUCT->ProductGetIdPath(PPRODUCT, product_id, path, PRODUCT_PATH_LEVEL_1) );   
		
		fprintf(file, "[%s]\n\n", path);
		
		for(i=0; i<etestSize; i++)
		{
			fprintf(file, "[");
			fprintf(file, etest[i].name );
			fprintf(file, "]\n");
			fprintf(file, "description=");
			
			strncpy(desc, GET_STRING(etest[i].description), DBS_RECORD_LENGHT_DESCRIPTION);
			pletter = strchr(desc, 37); /* %=37 */
			if(pletter)
				*pletter = 'p';
			
			fprintf(file, desc); 
			fprintf(file, "\n");
			
			for(j=0; j<etest[i].parameterSize; j++)
			{
				fprintf(file, etest[i].parameter[j].name);
				fprintf(file, "=" );
				fprintf(file, etest[i].parameter[j].value);
				fprintf(file, "\n" );  
			}
			fprintf(file, "\n"); 
		}
		
		}
		fclose(file);
		
		if( 1== GetFileInfo (pathtmp, &size))
		{
			char*	text;
			
			text = calloc(size+1, sizeof(char));
			
			file = fopen(pathtmp,"r");     
			fread (text, sizeof(char), size, file);
			fclose(file);   
			
			ResetTextBox (gs_panel[DBS], DBS_TEXTBOX, text);
			
			free(text);
		}
		
		if(to_file)
		{
			strcpy(pathini, pathtmp);
			pdata = pathini + strlen(pathtmp) - strlen("ini");
			sprintf(pdata, "ini");
			
			CopyFile (pathtmp, pathini);
			sprintf(message, _("File was created.\n%s"), pathini);
			MessageBox((HWND)hWnd, message, _("Message"), MB_OK); 
		}
		
		DeleteFile (pathtmp);
	}
	else
	{
		MessageBox((HWND)hWnd, _("Select Correct EndProduct!"), _("Warning"), MB_OK|MB_ICONWARNING);
	}
	
Error:
	PEXPORT->FreeTestSeq(PEXPORT, &etest, etestSize);
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "CreateProductParameterIniFile"
static SElExceptionPtr CreateProductParameterIniFile(bool_t to_file, time_t time)
{
	int32_t 	error = 0;
	SElExceptionPtr pexception = NULL;
	SEParamPtr	eparam = NULL;
	int32_t		eparamSize = 0;
	bool_t		valid;
	int32_t		product_id = global->product_id;
	char		message[1024];
	char*       product_name;
	char        pathtmp[MAX_PATHNAME_LEN];
	char        pathini[MAX_PATHNAME_LEN]; 
	FILE        *file = NULL;
	int         result;
	int         i, size;
	char		path[256];
	char*		pdata = NULL;   
	int			mindex = 0;
	
	EXCCHECK( PPRODUCT->ProductIsValidEndProduct(PPRODUCT, product_id, &valid) );

	if(valid)
	{
		/* create file */
		EXCCHECK( PPRODUCT->ProductGetIdName(PPRODUCT, product_id, &product_name) ); 
		GetProjectDir (pathtmp);
		strcat(pathtmp, "\\parameters_");
		strcat(pathtmp, product_name);
		strcat(pathtmp, ".tmp");

		result = DeleteFile (pathtmp);
		file = fopen(pathtmp,"w");
		
		EXCCHECK( PEXPORT->GetParameter(PEXPORT, product_id, time, &eparam, &eparamSize));
		EXCCHECK( PPRODUCT->ProductGetIdPath(PPRODUCT, product_id, path, PRODUCT_PATH_LEVEL_1) );
		
		fprintf(file, "[%s]\n\n", path);
		
		for(i=0; i<eparamSize; i++)
		{
			fprintf(file, eparam[i].name );
			fprintf(file, "=");
			if(eparam[i].vtype!=E_DVT_TEXT)
			{
				fprintf(file, eparam[i].value);
			}
			else
			{
				fprintf(file, "\n");
				fprintf(file, eparam[i].text);
			}
			fprintf(file, "\n");
		}
		
		fprintf(file, "\n");
		
		for(i=0; i<PMASTER->ProductMasterSize; i++)
		{
			if( (PMASTER->ProductMaster[i].property&PROPERTY_VALID)>0
				&& PMASTER->ProductMaster[i].product_id == product_id)
			{
				if(mindex==0)
				{
					char* 	name = "none";
					
					if( PMASTER->ProductMaster[i].master_id )
						EXCCHECK( PPRODUCT->ProductGetIdName(PPRODUCT, PMASTER->ProductMaster[i].master_id, &name) ); 	
					
					fprintf(file, "Super Master=%s\n", name);
				}
				
				if( 0!=strcmp(PMASTER->ProductMaster[i].master, "@empty") )
					fprintf(file, "Master%02d=%s\n", ++mindex, PMASTER->ProductMaster[i].master); 	
			}
		}
		
		fclose(file);
		
		if( 1== GetFileInfo (pathtmp, &size))
		{
			char*	text;
			
			text = calloc(size+1, sizeof(char));
			
			file = fopen(pathtmp,"r");     
			fread (text, sizeof(char), size, file);
			fclose(file);   
			
			ResetTextBox (gs_panel[DBS], DBS_TEXTBOX, text);
			
			free(text);
		}
		
		if(to_file)
		{
			strcpy(pathini, pathtmp);
			pdata = pathini + strlen(pathtmp) - strlen("ini");
			sprintf(pdata, "ini");
			
			CopyFile (pathtmp, pathini);
		
			sprintf(message, _("File was created.\n%s"), pathini);
			MessageBox((HWND)hWnd, message, _("Message"), MB_OK);			 
		}
		
		DeleteFile (pathtmp);
	}
	else
	{
		MessageBox((HWND)hWnd, _("Select Correct EndProduct!"), _("Warning"), MB_OK|MB_ICONWARNING);
	}
	
Error:
	PDBS_FREE(eparam);
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "CreateUserListIniFile"
static SElExceptionPtr CreateUserListIniFile(bool_t to_file)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	int32_t 		userSize;
	SEUserPtr 		user = NULL;
	char         	pathtmp[MAX_PATHNAME_LEN];
	char         	pathini[MAX_PATHNAME_LEN]; 
	FILE          	*file = NULL;
	int            	result, i, size; 
	char*			pdata = NULL;  
	char			message[1024];  
	STesterItemPtr	ptester = NULL;
	int32_t			tester_id = global->activeUser.tester_id;

	EXCCHECK( PTESTER->TesterGetSelected(PTESTER, tester_id, &ptester) ); 
	if(tester_id && ptester)
	{
		GetProjectDir (pathtmp);
		strcat(pathtmp, "\\users_");
		strcat(pathtmp, (ptester->description)? ptester->description: "noname");
		strcat(pathtmp, ".tmp");

		result = DeleteFile (pathtmp);
		file = fopen(pathtmp,"w");

		EXCCHECK( PUSER->UserGetTesterList(PUSER, tester_id, &user, &userSize) );	

		fprintf(file, "[%s]\n\n", (ptester->description)? ptester->description: "noname");

		for(i=0; i<userSize; i++)
		{
			char*	rights_tester = TesterRightsToString(user[i].rights_tester);
			
			fprintf(file, "Name=%s; ", user[i].name );
			fprintf(file, "iButton=%s;\n", user[i].ibutton);
			fprintf(file, "Rights=%s;\n", rights_tester);     
			
			fprintf(file, "\n");
			free(rights_tester);
		}

		fprintf(file, "\n");

		fclose(file);

		if( 1== GetFileInfo (pathtmp, &size))
		{
			char*	text;
			
			text = calloc(size+1, sizeof(char));
			
			file = fopen(pathtmp,"r");     
			fread (text, sizeof(char), size, file);
			fclose(file);   
			
			ResetTextBox (gs_panel[DBS], DBS_TEXTBOX, text);
			
			free(text);
		}

		if(to_file)
		{
			strcpy(pathini, pathtmp);
			pdata = pathini + strlen(pathtmp) - strlen("ini");
			sprintf(pdata, "ini");
			
			CopyFile (pathtmp, pathini);

			sprintf(message, _("File was created.\n%s"), pathini);
			MessageBox((HWND)hWnd, message, _("Message"), MB_OK);	
		}

		DeleteFile (pathtmp);
	}
	else
	{
		MessageBox((HWND)hWnd, _("Not Correct Tester Id!"), _("Warning"), MB_OK|MB_ICONWARNING);
	}
	
Error:
	PDBS_FREE(user);
	EXCRETHROW(pexception);
}
	

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "HistoryListGetTime"
static SElExceptionPtr	HistoryListGetTime(time_t* time)
{
	HRESULT			hr;
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL; 
	int16_t			value;
	VARIANT			variant;
	struct tm 		date;

	date.tm_yday = 0;
	date.tm_isdst = 0;

	EXCCHECKCOM( MSComCtl2_IDTPickerGetDay (global->hpicDayHandle, NULL, &variant));
	CA_VariantGetShort (&variant, &value);
	date.tm_mday = value;
	
	EXCCHECKCOM( MSComCtl2_IDTPickerGetMonth (global->hpicDayHandle, NULL, &variant)); 
	CA_VariantGetShort (&variant, &value);
	date.tm_mon = value;
	
	EXCCHECKCOM( MSComCtl2_IDTPickerGetYear(global->hpicDayHandle, NULL, &variant)); 
	CA_VariantGetShort (&variant, &value);
	date.tm_year = value;
	date.tm_year -= 1900;

	EXCCHECKCOM( MSComCtl2_IDTPickerGetDayOfWeek(global->hpicDayHandle, NULL, &variant)); 
	CA_VariantGetShort (&variant, &value);
	date.tm_wday = value;
	
	EXCCHECKCOM( MSComCtl2_IDTPickerGetHour(global->hpicHourHandle, NULL, &variant)); 
	CA_VariantGetShort (&variant, &value);
	date.tm_hour = value;
	
	EXCCHECKCOM( MSComCtl2_IDTPickerGetMinute(global->hpicHourHandle, NULL, &variant)); 
	CA_VariantGetShort (&variant, &value);
	date.tm_min = value; 
	
	EXCCHECKCOM( MSComCtl2_IDTPickerGetSecond(global->hpicHourHandle, NULL, &variant)); 
	CA_VariantGetShort (&variant, &value);
	date.tm_sec = value; 
	
	*time = mktime( &date );
	
Error:
	EXCRETHROW(pexception);
}

