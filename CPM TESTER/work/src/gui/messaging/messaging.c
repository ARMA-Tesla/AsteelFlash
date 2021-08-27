#include <userint.h>
#include "messaging.h"
#include "msxmldom.h"
#include <eventfactory/EventFactory.h>
#include <language/language.h>
#include <utility.h>
#include <tester/tester.h>
#include <box/box.h>
#include <printer/class/printers.h>
#include <time.h>
#include "messaging_plugin.h"
#include <sap_module/sap_converter.h>
#include <ansi_c.h>
#include "trans_language.h"

#define MAX_PANEL_NB		4
static int					gs_panel[MAX_PANEL_NB + 1];  
static SElEventFactoryPtr	gs_EventFactory;
static SObjListPtr          gs_Modules;     
static STesterPtr 	    	gs_pTester = NULL;
static SBoxPtr				gs_pBox = NULL;
static SPrintersPtr			gs_pPrinters = NULL;
SLanguagePtr				gs_pLanguage = NULL;

#define MAX_LENGTH_SENSOR_NAME			64
#define MAX_LENGTH_SENSOR_ERROR			256

#define PANEL_ITEM_NUMBER				4

static int Menu [PANEL_ITEM_NUMBER] = 
	{
		PANEL_ERROR, 	
		PANEL_PRINTER, 	
		PANEL_BOX, 	
		PANEL_SAP
	};

typedef struct _SSError
{
	char		sensor[MAX_LENGTH_SENSOR_NAME+1];
	char		reset[MAX_LENGTH_SENSOR_NAME+1];
	uint32_t	ok_state;
	char		description[MAX_LENGTH_SENSOR_ERROR+1];
} SSError, *SSErrorPtr;

typedef struct _SSErrors
{
	SSErrorPtr	pError;
	uint32_t	count;
} SSErrors;

static SSErrors				gs_Errors = {NULL, 0};

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Highlight_Buttom"
static SElExceptionPtr  Highlight_Buttom (int panel, int control) 
{ 
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	int 			i, selected;
#if 0    /* formerly excluded lines */
	
	for (i=0; i<PANEL_ITEM_NUMBER;i++)
	{
		if (Menu[i] == control)
			EXCCHECKCVI( SetCtrlAttribute (panel, Menu[i], ATTR_PICT_BGCOLOR, 0x555555)); 
		else 
			
			EXCCHECKCVI( SetCtrlAttribute (panel, Menu[i], ATTR_PICT_BGCOLOR, VAL_TRANSPARENT));
	}

#endif   /* formerly excluded lines */
Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "OnErrorExit"
int CVICALLBACK OnErrorExit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int32_t			   	error = 0;
	SElExceptionPtr    	pexception = NULL;
	
	switch (event)
	{
			case EVENT_COMMIT:
				EXCCHECKCVI( RemovePopup( 0));
			break;
	}
	
Error:   
	EXCDISPLAY( pexception);
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "OnError"
int CVICALLBACK OnError (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int32_t			   	error = 0;
	SElExceptionPtr    	pexception = NULL;
	
	switch (event)
	{
#if 0    /* formerly excluded lines */
		case EVENT_MOUSE_POINTER_MOVE :
			 Highlight_Buttom(panel, control);
		break;
#endif   /* formerly excluded lines */
		
		case EVENT_LEFT_CLICK:	
			EXCCHECKCVI( InstallPopup( gs_panel[MSG_ERROR]));
		break;
	}
	
Error:   
	EXCDISPLAY( pexception);
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "OnPrinter"
int CVICALLBACK OnPrinter (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int32_t			   	error = 0;
	SElExceptionPtr    	pexception = NULL;
	
	switch (event)
	{
		case EVENT_MOUSE_POINTER_MOVE :
			 Highlight_Buttom(panel, control);
		break;
		
		case EVENT_LEFT_CLICK:	
			//EXCCHECKCVI( InstallPopup( gs_panel[MSG_ERROR]));
		break;
	}
	
Error:   
	EXCDISPLAY( pexception);
	return 0;
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "OnBox"
int CVICALLBACK OnBox (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int32_t			   	error = 0;
	SElExceptionPtr    	pexception = NULL;
	
	switch (event)
	{
		case EVENT_MOUSE_POINTER_MOVE :
			 Highlight_Buttom(panel, control);
		break;
		
		case EVENT_LEFT_CLICK:	
			//EXCCHECKCVI( InstallPopup( gs_panel[MSG_ERROR]));
		break;
	}
	
Error:   
	EXCDISPLAY( pexception);
	return 0;
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "OnSAP"
int CVICALLBACK OnSAP (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int32_t			   	error = 0;
	SElExceptionPtr    	pexception = NULL;

	switch (event)
	{
		case EVENT_MOUSE_POINTER_MOVE :
			 Highlight_Buttom(panel, control);
		break;
		
		case EVENT_LEFT_CLICK:
			EXCCHECKCVI( InstallPopup( gs_panel[MSG_SAP]));
		break;
	}

Error:   
	EXCDISPLAY( pexception);
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "OnTimer"
int CVICALLBACK OnTimer (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int32_t			   	error = 0;
	SElExceptionPtr    	pexception = NULL;
	
	switch (event)
	{
		case EVENT_TIMER_TICK:
			{
				int value; 
				
				EXCCHECKCVI( GetCtrlVal( gs_panel[PANEL], PANEL_ERROR, &value));
				EXCCHECKCVI( SetCtrlVal( gs_panel[PANEL], PANEL_ERROR, !value));
			}
			break;
	}
	
Error:   
	EXCDISPLAY( pexception);
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "OnTimerPrinter"
int CVICALLBACK OnTimerPrinter (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int32_t			   	error = 0;
	SElExceptionPtr    	pexception = NULL;
	
	switch (event)
	{
		case EVENT_TIMER_TICK:

			
			
			
		break;
	}
	
Error:   
	EXCDISPLAY( pexception);
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "OnSAPErrorTimer"
int CVICALLBACK OnSAPErrorTimer (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr    	pexception = NULL;
	int32_t	error= 0;
	int	value;

	switch (event)
	{
		case EVENT_TIMER_TICK:
			EXCCHECKCVI( GetCtrlVal( panel, SAP_ERROR_TIME, &value));
			EXCCHECKCVI( SetCtrlVal( panel, SAP_ERROR_TIME, --value));
			if ( value <= 0 )
			{
				EXCCHECKCVI( SetCtrlAttribute(panel, SAP_ERROR_TIMER, ATTR_ENABLED, 0));
				EXCCHECKCVI( HidePanel(gs_panel[SAP_ERROR])); 
				EXCCHECKCVI( SetCtrlVal( panel, SAP_ERROR_TIME, 5));
			}
			break;
	}
Error:
	EXCDELETE( pexception);
	return 0;
}


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "OnTimerBox"
int CVICALLBACK OnTimerBox (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int32_t			   	error = 0;
	SElExceptionPtr    	pexception = NULL;
	
	switch (event)
	{
		case EVENT_TIMER_TICK:
			{
				int value; 
				
				EXCCHECKCVI( GetCtrlVal( gs_panel[PANEL], PANEL_BOX, &value));
				EXCCHECKCVI( SetCtrlVal( gs_panel[PANEL], PANEL_BOX, !value));
			}
			break;
	}
	
Error:   
	EXCDISPLAY( pexception);
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "OnTimerTick"
int CVICALLBACK OnTimerTick (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	int				hour, min, sec;
	int				month, day, year;
	char			buffer[256];
	
	switch (event)
	{
		case EVENT_TIMER_TICK:
			EXCCHECKCVI( GetSystemDate(&month, &day, &year));
			EXCCHECKCVI( GetSystemTime(&hour, &min, &sec));
			sprintf(buffer, "%02d:%02d:%02d", hour, min, sec);
			EXCCHECKCVI( SetCtrlVal( panel, PANEL_MSG_TIME, buffer));
			sprintf(buffer, "%02d/%02d/%04d", day, month, year);
			EXCCHECKCVI( SetCtrlVal( panel, PANEL_MSG_DATE, buffer));
			break;
	}
	
Error:                       
	/* EXCDISPLAY( pexception); */
	return 0;
}

#define NB_OF_ROW	16
static int32_t		gs_row = 1;
static int32_t		gs_NbOfActiveErrors = 0;
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "AddErrorMsg"
static SElExceptionPtr AddErrorMsg( SSErrorPtr pError)
{
	int32_t			   	error = 0;
	SElExceptionPtr    	pexception = NULL;
	int					month, day, year, hour, minutes, seconds;
	char				time[16];
	char				date[16];
	int					i;
	
	EXCCHECKCVI( GetSystemDate (&month, &day, &year));
	EXCCHECKCVI( GetSystemTime (&hour, &minutes, &seconds));

	sprintf(time, "%02d:%02d:%02d", hour, minutes, seconds);
	sprintf(date, "%d.%02d.%02d", year, month, day);
					
	EXCCHECKCVI( SetCtrlVal( gs_panel[PANEL], PANEL_ERROR, 1));
	ProcessSystemEvents ();
	ProcessDrawEvents ();					
	EXCCHECKCVI( SetCtrlAttribute( gs_panel[PANEL], PANEL_TIMER, ATTR_ENABLED, TRUE));

	EXCCHECKCVI( InsertTableRows (gs_panel[MSG_ERROR], MSG_ERROR_TABLE, 1, 1, VAL_CELL_STRING));

	/*
	if(gs_row<=NB_OF_ROW)
	{
		EXCCHECKCVI( DeleteTableRows (gs_panel[MSG_ERROR], MSG_ERROR_TABLE, NB_OF_ROW, 1));
	}*/

	EXCCHECKCVI( SetTableCellVal (gs_panel[MSG_ERROR], MSG_ERROR_TABLE, MakePoint (1, 1), date));
	EXCCHECKCVI( SetTableCellVal (gs_panel[MSG_ERROR], MSG_ERROR_TABLE, MakePoint (2, 1), time));
	EXCCHECKCVI( SetTableCellVal (gs_panel[MSG_ERROR], MSG_ERROR_TABLE, MakePoint (3, 1), "Error"));
	EXCCHECKCVI( SetTableCellVal (gs_panel[MSG_ERROR], MSG_ERROR_TABLE, MakePoint (4, 1), pError->description));
	EXCCHECKCVI( SetTableCellVal (gs_panel[MSG_ERROR], MSG_ERROR_TABLE, MakePoint (5, 1), pError->sensor));
	EXCCHECKCVI( SetTableCellVal (gs_panel[MSG_ERROR], MSG_ERROR_TABLE, MakePoint (6, 1), "Active"));

	gs_row++;
	gs_NbOfActiveErrors++;

Error:   
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "DisableErrorMsg"
static SElExceptionPtr DisableErrorMsg( SSErrorPtr pError)
{
	int32_t			   	error = 0;
	SElExceptionPtr    	pexception = NULL;
	int32_t				i;
	char				sensor[MAX_LENGTH_SENSOR_NAME+1];

	for(i=1; i<gs_row; i++)
	{
		EXCCHECKCVI( GetTableCellVal (gs_panel[MSG_ERROR], MSG_ERROR_TABLE, MakePoint (5, i), sensor));

		if( 0==strcmp( sensor, pError->sensor))
		{
		
			DeleteTableRows (gs_panel[MSG_ERROR], MSG_ERROR_TABLE, i, 1);
			
			//EXCCHECKCVI( SetTableCellVal (gs_panel[MSG_ERROR], MSG_ERROR_TABLE, MakePoint (6, i), "Passive"));
			gs_NbOfActiveErrors--;

			if( gs_NbOfActiveErrors==0)
			{
				EXCCHECKCVI( SetCtrlAttribute( gs_panel[PANEL], PANEL_TIMER, ATTR_ENABLED, FALSE));
				EXCCHECKCVI( SetCtrlVal( gs_panel[PANEL], PANEL_ERROR, 0));
				ProcessSystemEvents ();
				ProcessDrawEvents ();
			}
			break;
			
		}
	}

Error:   
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SensorCallbackFnc"
static SElExceptionPtr _FUNCC SensorCallbackFnc(const char* name, uint32_t state, void* pData)
{
	SElExceptionPtr	pexception = NULL;
	SSErrorPtr		pError = (SSErrorPtr)pData; 

	if(pError->ok_state!=state)
	{
		EXCCHECK( AddErrorMsg( pError));
	}
	else
	{
		EXCCHECK( DisableErrorMsg( pError));
	}

Error:   
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "BoxCallbackFnc"
static SElExceptionPtr _FUNCC BoxCallbackFnc(SBoxPtr me)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	char			stmp[256];

	sprintf(stmp, "%d/%d", me->_PanelsActual, me->_PanelsInBox);
	EXCCHECKCVI( SetCtrlVal(gs_panel[PANEL], PANEL_BOX_TXT, stmp)); 

Error:   
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "FirstCheck"
static SElExceptionPtr FirstCheck(void)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			i;
	uint32_t		state;

	for(i=0; i<gs_Errors.count; i++)
	{
		EXCCHECK( gs_pTester->GetSensor(gs_pTester, gs_Errors.pError[i].sensor, &state));

		if(state!=gs_Errors.pError[i].ok_state)
		{
			EXCCHECK( AddErrorMsg( &gs_Errors.pError[i]));	
		}
	}

Error:   
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "RegisterErrors"
static SElExceptionPtr RegisterErrors(void)
{
	SElExceptionPtr	pexception = NULL;
	CAObjHandle		xml_doc = 0;
	CAObjHandle		errors_list = 0;
	CAObjHandle		pparameter = 0;
	CAObjHandle		pnode = 0;
	VBOOL			is_xml_success = 1;
	char*			ptext = NULL;
	VARIANT         pfile_name_var = CA_VariantEmpty();
	int32_t			lenght = 0,
					error = 0;
	HRESULT			hr;
	int32_t			i;

	EXCCHECKCOM( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &xml_doc));
	EXCCHECKCVI( CA_VariantSetCString(  &pfile_name_var, gs_pTester->path));
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (xml_doc, NULL, pfile_name_var, &is_xml_success));

	EXCCHECKCOM( MSXML_IXMLDOMDocumentselectNodes (xml_doc, NULL,
						"//module[@id='"MODULE_TESTER"']/Errors/Error",
						&errors_list));
	EXCCHECKCOM( MSXML_IXMLDOMNodeListreset (errors_list, NULL));
	EXCCHECKCOM( MSXML_IXMLDOMNodeListGetlength( errors_list, NULL, &lenght));
	
	gs_Errors.count = lenght;
	gs_Errors.pError = (SSErrorPtr)calloc(gs_Errors.count, sizeof(SSError));
	EXCCHECKALLOC( gs_Errors.pError);

	for(i=0; i<lenght; i++)
	{
		EXCCHECKCOM( MSXML_IXMLDOMNodeListnextNode (errors_list, NULL, &pnode));
		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(pnode, NULL, "@sensor", &pparameter));
		if( pparameter)
		{
			EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));

			if(ptext)
			{
				strncpy( gs_Errors.pError[i].sensor, ptext, MAX_LENGTH_SENSOR_NAME);
				EXCCHECK( gs_pTester->SetSensorCallback( gs_pTester, ptext, SensorCallbackFnc, &gs_Errors.pError[i]));
				CA_FreeMemory(ptext);
				ptext=NULL;
			}

			EXCCHECKCOM( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
		}

		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(pnode, NULL, "@reset", &pparameter));
		if( pparameter)
		{
			EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));

			if(ptext)
			{
				strncpy( gs_Errors.pError[i].reset, ptext, MAX_LENGTH_SENSOR_NAME);
				CA_FreeMemory(ptext);
				ptext=NULL;
			}

			EXCCHECKCOM( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
		}

		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(pnode, NULL, "@description", &pparameter));
		if( pparameter)
		{
			EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));

			if(ptext)
			{
				strncpy( gs_Errors.pError[i].description, ptext, MAX_LENGTH_SENSOR_ERROR);
				CA_FreeMemory(ptext);
				ptext=NULL;
			}

			EXCCHECKCOM( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
		}

		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(pnode, NULL, "@ok", &pparameter));
		if( pparameter)
		{
			EXCCHECKCOM( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));

			if(ptext)
			{
				gs_Errors.pError[i].ok_state = strtol(ptext, NULL, 10);
				CA_FreeMemory(ptext);
				ptext=NULL;
			}

			EXCCHECKCOM( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
		}

	}
	
Error:
	if (errors_list) CA_DiscardObjHandle(errors_list);
	if (pnode) CA_DiscardObjHandle(pnode);
	if (pparameter) CA_DiscardObjHandle(pparameter);
	if (xml_doc) CA_DiscardObjHandle(xml_doc);
	CA_FreeMemory(ptext);
	CA_VariantClear(&pfile_name_var);
	EXCRETHROW(pexception);
}	/* RegisterErrors */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "EventFunctionCallback"
static SElExceptionPtr EventFunctionCallback( const char* eventName, void* pData)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL;

	const char* setting = ((SElEventDataPtr)pData)->GetStringPtr(pData);
	char* pdata = NULL;
			
	if(0==strncmp(setting, "PANEL_MSG_ADMIN", strlen("PANEL_MSG_ADMIN")))
	{
		pdata = (char*)setting + strlen("PANEL_MSG_ADMIN:");
		EXCCHECKCVI( SetCtrlVal(gs_panel[PANEL], PANEL_MSG_ADMIN, gettext(pdata)));
							
	}
			
	else if(0==strcmp(eventName, "EVNT_BOX_CHANGE"))
	{
		EXCCHECKCVI( SetCtrlAttribute( gs_panel[PANEL], PANEL_TIMER_BOX, ATTR_ENABLED, TRUE));		
	}
	else if(0==strcmp(eventName, "EVNT_BOX_INSERTED"))
	{
		EXCCHECKCVI( SetCtrlAttribute( gs_panel[PANEL], PANEL_TIMER_BOX, ATTR_ENABLED, FALSE));
		EXCCHECKCVI( SetCtrlVal( gs_panel[PANEL], PANEL_BOX, 0));
	}
	else if(0==strcmp(eventName, "EVNT_BATTERY_ON_BATTERY"))
	{
		SSError		new_error;

		strcpy_s(new_error.description, sizeof(new_error.description), _("Tester On Battery"));
		new_error.ok_state = 1;
		strcpy_s(new_error.reset, sizeof(new_error.reset), "");
		strcpy_s(new_error.sensor, sizeof(new_error.sensor), "UPS Battery");

		EXCCHECK( AddErrorMsg( &new_error));
	}
	else if(0==strcmp(eventName, "EVNT_BATTERY_ON_POWER"))
	{
		SSError		new_error;

		strcpy_s(new_error.description, sizeof(new_error.description), _("Tester On Battery"));
		new_error.ok_state = 1;
		strcpy_s(new_error.reset, sizeof(new_error.reset), "");
		strcpy_s(new_error.sensor, sizeof(new_error.sensor), "UPS Battery");

		EXCCHECK( DisableErrorMsg( &new_error));
	}

Error:
	EXCRETHROW(pexception);
} 

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "EventSAPReadFunctionCallback"
static SElExceptionPtr EventSAPReadFunctionCallback( const char* eventName, void* pData)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL;
	const char* pstr_data = ((SElEventDataPtr)pData)->GetStringPtr(pData);

	if(0==strcmp(eventName, "EVNT_SAP_CODE_READ"))
	{
		
		EXCCHECKCVI( SetCtrlVal( gs_panel[MSG_SAP],
								 MSG_SAP_BARCODE,
								 (pstr_data) ? ((const char*)pstr_data) : ""));
		EXCCHECKCVI( SetCtrlVal( gs_panel[MSG_SAP],
								 MSG_SAP_TRANSLATED_BARCODE,
								 ""));
	}
Error:
	EXCRETHROW(pexception);
}


static void CVICALLBACK PostDeferredDisplayError( void *pData)
{
	int32_t	error;
	SElExceptionPtr	pexception = NULL;
	unsigned int	count  = 0;

	if ( pData )
	{
		int status = atoi((const char*)pData);
		const char* pend = strchr( (const char*)pData, ';');
		char	message[1024] = "";

		if ( pend ) pend++;
		EXCCHECKCVI( GetCtrlVal( gs_panel[PANEL], PANEL_SAP_ERROR, &count));
		switch ( status )
		{
		case -1:
				sprintf(message, _("Panel has not tested !!!\n%s"), (pend) ? pend: "");
				count++;
				break;
		case 1:
				sprintf(message, _("Last test of panel failed !!!\n%s"), (pend) ? pend: "");
				count++;
				break;
		case 0:
				sprintf(message, _("Unknown barcode !!!\n%s"), (pend) ? pend: "");
				break;
		}
		EXCCHECKCVI( SetCtrlVal( gs_panel[PANEL], PANEL_SAP_ERROR, count));
		EXCCHECKCVI( SetCtrlVal( gs_panel[SAP_ERROR], SAP_ERROR_STRING, message));
		EXCCHECKCVI( DisplayPanel(gs_panel[SAP_ERROR])); 
		EXCCHECKCVI( SetCtrlVal( gs_panel[SAP_ERROR], SAP_ERROR_TIME, 5));
		EXCCHECKCVI( SetCtrlAttribute( gs_panel[SAP_ERROR], SAP_ERROR_TIMER, ATTR_ENABLED, 1));
	}
	else
	{
		EXCCHECKCVI( SetCtrlAttribute( gs_panel[SAP_ERROR], SAP_ERROR_TIMER, ATTR_ENABLED, 0));
		EXCCHECKCVI( HidePanel(gs_panel[SAP_ERROR])); 
	}

Error:
	free(pData);
	EXCDISPLAY(pexception);
}

static void CVICALLBACK PostDeferredDisplayDebugInfo( void *pData)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL;

	if ( pData )
	{
		int ctrl_id = atoi((const char*)pData);
		const char* pend = strchr( (const char*)pData, ';');

		if ( pend ) pend++;

		EXCCHECKCVI( SetCtrlVal( gs_panel[MSG_SAP],
								 ctrl_id,
								 (pend) ? ((const char*)pend) : ""));
		if ( ctrl_id == MSG_SAP_BARCODE )
		{
			EXCCHECKCVI( SetCtrlVal( gs_panel[MSG_SAP],
									 MSG_SAP_TRANSLATED_BARCODE,
									 ""));
		}
		else
		if ( ctrl_id == MSG_SAP_TRANSLATED_BARCODE )
		{
			PostDeferredDisplayError(NULL);
		}

	}
Error:
	free(pData);
	EXCDISPLAY(pexception);
}



/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "EventSAPFunctionCallback"
static SElExceptionPtr EventSAPFunctionCallback( const char* eventName, void* pData)
{
	int32_t			error = 0;
	char*		pmessage =  NULL;
	const char* pstr_data = ((SElEventDataPtr)pData)->GetStringPtr(pData);

	
	if(0==strcmp(eventName, "EVNT_SAP_CODE_CHECK"))
	{
#if 1
		pmessage = malloc( strlen(pstr_data) + 1);
		strcpy( pmessage, pstr_data);
		PostDeferredCall( PostDeferredDisplayError , (void *)pmessage );
#else
		char	message[1024]="";
		sprintf(message, "Message: %s\n", (pstr_data) ? pstr_data: "");
		OutputDebugStr( message);

#endif
	}
	else if(0==strcmp(eventName, "EVNT_SAP_CODE_TRANSLATED"))
	{
		pmessage = malloc( strlen(pstr_data) + 16);
		sprintf(pmessage, "%d;%s", MSG_SAP_TRANSLATED_BARCODE, pstr_data);
		PostDeferredCall( PostDeferredDisplayDebugInfo , pmessage );
	}
	else if(0==strcmp(eventName, "EVNT_SAP_CODE_READ"))
	{
		pmessage = malloc( strlen(pstr_data) + 16);
		sprintf(pmessage, "%d;%s", MSG_SAP_BARCODE, pstr_data);
		PostDeferredCall( PostDeferredDisplayDebugInfo , pmessage );		
	}

//Error:
//	EXCRETHROW(pexception);
	return NULL;
}


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "RegisterEventFunction"
static SElExceptionPtr RegisterEventFunction(void)
{
	SElExceptionPtr	pexception = NULL;
	
	if ( gs_EventFactory )
	{
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_BOX_CHANGE",
														 EventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_BOX_INSERTED",
														 EventFunctionCallback));

		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_BATTERY_ON_BATTERY",
														 EventFunctionCallback));
		
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_BATTERY_ON_POWER",
														 EventFunctionCallback));
		
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
		                                  				 "EVNT_GUI_INFO_SET",
		                                   				EventFunctionCallback));
#if 1
		
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_SAP_CODE_TRANSLATED",
														 EventSAPFunctionCallback));
#endif
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_SAP_CODE_CHECK",
														 EventSAPFunctionCallback));
#if 1
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_SAP_CODE_READ",
														 EventSAPFunctionCallback));
#endif		
		
	}

Error:
	EXCRETHROW(pexception);
}	/* RegisterEventFunction */

/*---------------------------------------------------------------------------*/
#if defined(_CVI_DLL_) || \
	defined(_WINDLL)
void DiscardUIObjectsForDLL (void)
{
	/* Discard the panels loaded in InitUIForDLL */
}

int __stdcall DllMain(
	HINSTANCE hinstDLL,
	DWORD fdwReason,
	LPVOID lpvReserved
)
{
	switch (fdwReason)
		{
		case DLL_PROCESS_ATTACH:
#if (!defined(_CVI_)&&defined(_DEBUG))
			//_CrtSetBreakAlloc(88);
#endif
			if (InitCVIRTE (hinstDLL, 0, 0) == 0)		
				return 0;	/* out of memory */
			break;
		case DLL_PROCESS_DETACH:
			if (!CVIRTEHasBeenDetached())		/* Do not call CVI functions if cvirte.dll has already been detached. */
				{
				DiscardUIObjectsForDLL ();		/* Discard the panels loaded in InitUIForDLL */
				CloseCVIRTE ();		
				}
#if !defined(_CVI_) && defined(_DEBUG) && defined(DUMP_MEMORY_LEAKS)
			_CrtDumpMemoryLeaks();
#endif
			break;
		}

	return 1;
}

int __stdcall DllEntryPoint(
	HINSTANCE hinstDLL,
	DWORD fdwReason,
	LPVOID lpvReserved
)
{
	/* Included for compatibility with Borland */

	 return DllMain (hinstDLL, fdwReason, lpvReserved);
}

#undef __FUNC__
#define __FUNC__ "{Messaging Plugin}::Initialize"
PLUGIN_API SElExceptionPtr PLUGIN_INIT_NAME_CODE( SPluginArgsPtr a_pPluginArgs )
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	const char*		pconfiguration_file = NULL;
	int32_t			main_panel = 0;
	int				monitor_id, top, height;       
	SSAPConverterPtr	psap = NULL;

	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
									 "MainPanel",
									 sizeof(main_panel),
									 &main_panel));

	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
									 "WindowHandle",
									 sizeof(hWnd),
									 &hWnd));
	
	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
	                        "Modules",
	                        sizeof(SObjListPtr),
	                        &gs_Modules));
	
	/*Gettext*/
	if(gs_Modules)
	{
		gs_pLanguage = (SLanguagePtr)gs_Modules->Get(gs_Modules, MODULE_LANGUAGE);
		if(gs_pLanguage)
		{
			char pathname[MAX_PATHNAME_LEN];
			EXCCHECKCVI( GetProjectDir( pathname));
			strcat(pathname, "\\lang");
			LANGUAGE_PTR->bindtextdomain (LANGUAGE_PTR, PACKAGE, pathname);
			LANGUAGE_PTR->textdomain (LANGUAGE_PTR, PACKAGE);
		}
	}
	
	EXCCHECK( LoadPanelExWithLanguage(main_panel, "messaging.uir", PANEL, PANELlang, &gs_panel[PANEL]));
	EXCCHECK( LoadPanelExWithLanguage(main_panel, "messaging.uir", MSG_ERROR, MSG_ERRORlang, &gs_panel[MSG_ERROR])); 
	EXCCHECK( LoadPanelExWithLanguage(main_panel, "messaging.uir", MSG_SAP, MSG_SAPlang, &gs_panel[MSG_SAP]));
	EXCCHECK( LoadPanelExWithLanguage(main_panel, "messaging.uir", SAP_ERROR, SAP_ERRORlang, &gs_panel[SAP_ERROR]));
	
	if(gs_Modules)
	{
		psap = (SSAPConverterPtr)gs_Modules->Get(gs_Modules, MODULE_SAP_CONVERTER);
	}
	if ( psap == NULL )
	{
		SetCtrlAttribute(gs_panel[PANEL], PANEL_SAP, ATTR_VISIBLE, 0);
		SetCtrlAttribute(gs_panel[PANEL], PANEL_SAP_ERR_LABEL, ATTR_VISIBLE, 0);
		SetCtrlAttribute(gs_panel[PANEL], PANEL_SAP_ERROR, ATTR_VISIBLE, 0);
	}
	
	EXCCHECKCVI( GetPanelAttribute (main_panel, ATTR_TOP, &top));
	EXCCHECKCVI( GetPanelAttribute (main_panel, ATTR_HEIGHT, &height));

	//EXCCHECKCVI( SetPanelAttribute (gs_panel[PANEL], ATTR_TOP, height-65));
	//EXCCHECKCVI( SetPanelAttribute (gs_panel[PANEL], ATTR_LEFT, 65));
	
	EXCCHECKCVI( SetPanelAttribute (gs_panel[PANEL], ATTR_TOP, height-60));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[PANEL], ATTR_LEFT, 50));
	
	EXCCHECKCVI(SetPanelAttribute (gs_panel[PANEL], ATTR_SCALE_CONTENTS_ON_RESIZE, TRUE));
	EXCCHECKCVI(SetPanelAttribute (gs_panel[MSG_ERROR], ATTR_SCALE_CONTENTS_ON_RESIZE, TRUE));

	EXCCHECKCVI( SetPanelAttribute (gs_panel[SAP_ERROR], ATTR_LEFT, height / 2 - 450));

	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
									 "EventFactory",
									 sizeof(SElEventFactoryPtr),
									 &gs_EventFactory));

	EXCCHECK( RegisterEventFunction());
	
	if(gs_Modules)
	{
		/*
		gs_pBox = gs_Modules->Get(gs_Modules, MODULE_BOX);
		if(gs_pBox)
		{
			gs_pBox->_fncBoxCallback = BoxCallbackFnc;
		} */

		gs_pTester = gs_Modules->Get(gs_Modules, MODULE_TESTER);
		if(gs_pTester)
		{
			EXCCHECK( RegisterErrors());
			EXCCHECK( FirstCheck());
		}

		gs_pPrinters = gs_Modules->Get(gs_Modules, MODULE_PRINTERS);
	}

Error:	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Messaging Plugin}::Run"
PLUGIN_API SElExceptionPtr PLUGIN_RUN_NAME_CODE( SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	
	EXCCHECKCVI( DisplayPanel(gs_panel[PANEL])); 

Error:	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Messaging Plugin}::Leave"
PLUGIN_API SElExceptionPtr PLUGIN_LEAVE_NAME_CODE( SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;

Error:	
	EXCRETHROW( pexception);

}

#undef __FUNC__
#define __FUNC__ "{Messaging Plugin}::Shutdown"
PLUGIN_API SElExceptionPtr PLUGIN_SHUTDOWN_NAME_CODE( SPluginArgsPtr a_pPluginArgs)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;

	free( gs_Errors.pError);
	gs_Errors.pError = NULL;

	EXCCHECKCVI( DiscardPanel( gs_panel[MSG_ERROR]));      
	EXCCHECKCVI( DiscardPanel( gs_panel[MSG_SAP]));      
	EXCCHECKCVI( DiscardPanel( gs_panel[SAP_ERROR]));      
	
	EXCCHECKCVI( DiscardPanel( gs_panel[PANEL]));
Error:	
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{Messaging Plugin}::Info"
PLUGIN_API SElExceptionPtr PLUGIN_GETINFO_NAME_CODE( SPluginInfoPtr* a_pInfo)
{
	SElExceptionPtr	pexception = NULL;  
	
	if(a_pInfo) EXCCHECK( (*a_pInfo)->SetInfoData(*a_pInfo, &gs_InfoData));
	
Error:	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Messaging Plugin}::Attribute"
PLUGIN_API SElExceptionPtr PLUGIN_GETATTR_NAME_CODE( uint32_t  attrID, uint32_t size, void* value)
{
	return NULL;
}

#endif

