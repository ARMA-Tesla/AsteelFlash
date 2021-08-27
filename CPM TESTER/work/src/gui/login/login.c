#include <msxmldom.h>
#include <eventfactory/EventFactory.h>
#include <utility.h>
#include <dbs/dbs.h>
#include <guid.h>
#include <language/language.h>
#include "login_plugin.h"
#include <ansi_c.h>
#include "trans_language.h"

#define MAX_PASSWORD_LENGTH		32

typedef enum _EUserStatus
{
	US_UNKNOWN = 0,
	US_LOGIN,
	US_LOGOUT
}EUserStatus;

#define USER_DATA_LENGHT	512

typedef struct _SLoginConf
{
	bool_t	is_password;
	bool_t	is_username;
	bool_t	is_userlist;
	uint8_t	password_length;
	uint8_t	username_length;
	uint32_t list_size;
	SEUserPtr	list;
	EUserStatus	user_status;
	int32_t		user_active;
} SLoginConf, *SLoginConfPtr;

static SLoginConf	gs_configuration = 
{
	TRUE,
	TRUE,
	FALSE,
	MAX_PASSWORD_LENGTH,
	MAX_PASSWORD_LENGTH,
	0,
	NULL,
	US_LOGOUT,
	0
};

static int					gs_panel;  
static char					gs_password[MAX_PASSWORD_LENGTH+1] = "";
static SElEventFactoryPtr	gs_EventFactory = NULL;
static SObjListPtr          gs_Modules = NULL;        
static char*				gs_pEnd = gs_password;
static SDBSPtr				gs_pDbs = NULL;
SLanguagePtr				gs_pLanguage = NULL;

static SElExceptionPtr LoadPluginConfiguration(const char*	fileName, SLoginConfPtr pConfig);
static SElExceptionPtr CleanupPluginConfiguration(SLoginConfPtr	pConfig);
static SElExceptionPtr ConfigurePlugin(SLoginConfPtr pConfig);

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Login Plugin}::PostDeferredHidePanel"
void CVICALLBACK PostDeferredHidePanel (void *callbackData) 
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	int32_t			visible;

	EXCCHECKCVI( GetPanelAttribute(gs_panel, ATTR_VISIBLE, &visible));
	if(visible)
		EXCCHECKCVI( RemovePopup(0));

Error:
	EXCDISPLAY( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Login Plugin}::DisplayLoginDialog"
static SElExceptionPtr DisplayLoginDialog(
	void
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	
	EXCCHECKCVI( SetCtrlVal (gs_panel, PANEL_LOGIN, ""));
	
	memset(&gs_password, 0, sizeof(gs_password));
	gs_pEnd = gs_password;
	
	EXCCHECKCVI( SetCtrlVal(gs_panel, PANEL_PASSWORD, ""));	
	EXCCHECKCVI( SetCtrlVal(gs_panel, PANEL_LOGINLIST, 0));	 
	EXCCHECKCVI( InstallPopup(gs_panel));   
	
Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Login Plugin}::CloseLoginDialog"
static SElExceptionPtr CloseLoginDialog(
	void
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	
	EXCCHECKCVI( PostDeferredCall(PostDeferredHidePanel, NULL)); 
	
Error:
	EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Login Plugin}::OnPassword"
int CVICALLBACK OnPassword (
	int panel,
	int control,
	int event,
	void *callbackData,
	int eventData1,
	int eventData2
)
{
	int32_t			error = 0;
	char			stars[MAX_PASSWORD_LENGTH+1] = "";
	SElExceptionPtr	pexception = NULL;
	SElEventDataPtr	pevent_data = NULL;

	switch (event)
	{
		case EVENT_COMMIT:
		case EVENT_KEYPRESS:
			
			EXCCHECKCVI( GetCtrlVal(panel, control, stars));
			
			if ( eventData1 >= ' ' &&  eventData1 <= '~' )
			{
				if (( gs_pEnd - gs_password ) == MAX_PASSWORD_LENGTH )  
					return (-1);
				
				*gs_pEnd++ = eventData1;
				*gs_pEnd = '\0';
				strcat(stars, "*");
			}
			else if ( eventData1 == VAL_BACKSPACE_VKEY)
			{
				if ( gs_pEnd > gs_password )
				{
					*--gs_pEnd = '\0';
				}
				memset( stars, 0, sizeof(stars));
				memset( stars, '*', gs_pEnd - gs_password);
			}
			else if ( eventData1 == VAL_ENTER_VKEY)
			{
	
				if ( gs_configuration.is_userlist )
				{
					int		i;
					
					EXCCHECKCVI( GetCtrlVal(panel, PANEL_LOGINLIST, &i)); 
					
					if( i!=0 && 0==strcmp( gs_configuration.list[i-1].password, gs_password))
					{
					   	if ( gs_EventFactory )
						{
							char	buffer[512];
							
							sprintf(buffer, "%d;%s;%d;%d;%d", 
											gs_configuration.list[i-1].user_id,
											gs_configuration.list[i-1].name,
											gs_configuration.list[i-1].tester_id,
											gs_configuration.list[i-1].rights_tester,
											gs_configuration.list[i-1].rights_dbs); 
							EXCCHECK( eleventdata_new(&pevent_data));
							EXCCHECK( pevent_data->SetStringPtr(pevent_data, buffer));
							EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_USER_LOGIN", pevent_data));
							EXCCHECK( eleventdata_delete(&pevent_data));
							EXCCHECK( CloseLoginDialog());
							gs_configuration.user_active = i-1;
						}
					}
					else if(i==0) /* none item */
					{
						EXCCHECKCVI( MessagePopup(_("Login Fault"), _("Select correct user!")));
					}
					else
					{
						EXCCHECKCVI( MessagePopup(_("Login Fault"), _("Incorect password!")));
					}
				}
				else if ( gs_configuration.is_username )
				{
					char	user_name[256];
					int		i;
					
					EXCCHECKCVI( GetCtrlVal(panel, PANEL_LOGIN, user_name)); 
					
					for(i=0; i<gs_configuration.list_size; i++)
					{
						if(0==strcmp(gs_configuration.list[i].name, user_name))
							break;
					}
					
					if(i==gs_configuration.list_size)
					{
						EXCCHECKCVI( MessagePopup(_("Login Fault"), _("Incorect name or user not registered!")));
					}
					else if(0==strcmp( gs_configuration.list[i].password, gs_password))    
					{
						if ( gs_EventFactory )
						{
							char	buffer[512];
							
							sprintf(buffer, "%d;%s;%d;%d;%d", 
											gs_configuration.list[i].user_id,
											gs_configuration.list[i].name,
											gs_configuration.list[i].tester_id,
											gs_configuration.list[i].rights_tester,
											gs_configuration.list[i].rights_dbs); 
							EXCCHECK( eleventdata_new(&pevent_data));
							EXCCHECK( pevent_data->SetStringPtr(pevent_data, buffer));
							EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_USER_LOGIN", pevent_data));
							EXCCHECK( eleventdata_delete(&pevent_data));
							EXCCHECK( CloseLoginDialog());
							gs_configuration.user_active = i;
						}
					}
					else
					{
						EXCCHECKCVI( MessagePopup(_("Login Fault"), _("Incorect password!")));
					}
				}
				
			}
			else if ( eventData1 == VAL_TAB_VKEY )
			{
				if ( gs_configuration.is_userlist )
				{
					int		i, count;
					
					EXCCHECKCVI( GetNumListItems (panel, PANEL_LOGINLIST, &count));
					EXCCHECKCVI( GetCtrlVal(panel, PANEL_LOGINLIST, &i));
					
					if(++i==count)
						i=0;
						
					EXCCHECKCVI( SetCtrlVal(panel, PANEL_LOGINLIST, i));   
				}
				return (-1);
			}
			else if ( eventData1 == VAL_ESC_VKEY )
			{
				EXCCHECK( CloseLoginDialog()); 
			}
			else
			{
				return (-1); 
			}
			
			EXCCHECKCVI( SetCtrlVal(panel, control, stars)); 
			EXCCHECKCVI( SetCtrlAttribute (panel, control, ATTR_TEXT_SELECTION_START, gs_pEnd - gs_password));
			return (-1); 
	}


Error:
	eleventdata_delete(&pevent_data);
	EXCDISPLAY( pexception);
	return 0;
}

/*---------------------------------------------------------------------------*/
int CVICALLBACK OnLoginList (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	SElExceptionPtr	pexception = NULL;      
	int32_t			error = 0;
	
	switch (event)
		{
		case EVENT_COMMIT:
			
			memset(&gs_password, 0, sizeof(gs_password));
			gs_pEnd = gs_password;
	
			EXCCHECKCVI( SetCtrlVal(gs_panel, PANEL_PASSWORD, ""));	
			
			EXCCHECKCVI( SetActiveCtrl (gs_panel, PANEL_PASSWORD));
			
			break;
		}
	
Error:
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Login Plugin}::LoadPluginConfiguration"
static SElExceptionPtr LoadPluginConfiguration(
	const char*	fileName,
	SLoginConfPtr	pConfig
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	HRESULT			hr = 0;
	char			__message[512];
	CAObjHandle		pxml  = 0;
	CAObjHandle		pnode = 0;
	CAObjHandle		pparameter = 0;
	CAObjHandle		pitem = 0;
	CAObjHandle		pattribute = 0;
	const char*		pfile_name = (fileName) ? fileName : "plugins/login.xml";
	VBOOL			is_xml_success = 1;
	VARIANT			pfile_name_var = CA_VariantEmpty();
	char*			ptext= NULL;

	EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml));
	EXCCHECKCVI( CA_VariantSetCString(  &pfile_name_var, pfile_name));
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (pxml, NULL, pfile_name_var, &is_xml_success));
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//plugin[@id='"PLUGIN_LOGIN"']", &pnode));	
	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "parameters/parameter[@name='password_required']", &pparameter));   	
	if ( pparameter )
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
		pConfig->is_password = (stricmp(ptext,"true") == 0);
		CA_DiscardObjHandle(pparameter);
		pparameter = 0;
		CA_FreeMemory(ptext);
		ptext=NULL;
		
	}
	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "parameters/parameter[@name='user_required']", &pparameter));
	if ( pparameter )
	{
		
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
		pConfig->is_username = (stricmp(ptext,"true") == 0);
		pConfig->is_userlist = !pConfig->is_username;
		EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
		pparameter = 0;
		CA_FreeMemory(ptext);
		ptext=NULL;
	}
	
Error:
	CA_FreeMemory(ptext);
	CA_VariantClear(&pfile_name_var);
	if (pattribute) CA_DiscardObjHandle(pattribute);
	if (pitem) CA_DiscardObjHandle(pitem);
	if (pparameter) CA_DiscardObjHandle(pparameter);
	if (pnode) CA_DiscardObjHandle(pnode);
	if (pxml) CA_DiscardObjHandle(pxml);
	EXCRETHROW( pexception);
} /* LoadPluginConfiguration */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Login Plugin}::ConfigurePlugin"
static SElExceptionPtr ConfigurePlugin(
	SLoginConfPtr	pConfig
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	uint32_t		i = 0;
	SEUserPtr		pitem;
	
	if(gs_pDbs)
		EXCCHECK( gs_pDbs->UserList->UserGetList(gs_pDbs->UserList, &gs_configuration.list, &gs_configuration.list_size));

	EXCCHECKCVI( SetCtrlAttribute (gs_panel, PANEL_PASSWORD, ATTR_MAX_ENTRY_LENGTH, pConfig->password_length));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel, PANEL_PASSWORD, ATTR_MAX_ENTRY_LENGTH, pConfig->username_length));

	EXCCHECKCVI( SetCtrlAttribute (gs_panel, PANEL_PASSWORD, ATTR_VISIBLE, pConfig->is_password));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel, PANEL_LOGIN, ATTR_VISIBLE, pConfig->is_username));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel, PANEL_LOGINLIST, ATTR_VISIBLE, pConfig->is_userlist));

	EXCCHECKCVI( DeleteListItem (gs_panel, PANEL_LOGINLIST, 0, -1));

	if(pConfig->is_userlist)
	{
		EXCCHECKCVI( InsertListItem (gs_panel, PANEL_LOGINLIST, -1, "Selectionner", i)); 
		
		pitem = pConfig->list;
		
		for( i=0; i<pConfig->list_size; i++, pitem++)
		{
			if(0!=strlen(pitem->password) )
				EXCCHECKCVI( InsertListItem (gs_panel, PANEL_LOGINLIST, -1, pitem->name, i+1));
		}
	}

Error:	
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Login Plugin}::CleanupPluginConfiguration"
static SElExceptionPtr CleanupPluginConfiguration(
	SLoginConfPtr	pConfig
)
{
	if(gs_pDbs)
	{
		gs_pDbs->Free( pConfig->list );
		pConfig->list = NULL;
	}
	
	pConfig->list_size = 0;
	
	return NULL;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "iButtonLog"
static SElExceptionPtr iButtonLog(const char* ibutton)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	int				i;
	SElEventDataPtr	pevent_data = NULL;

	if ( ibutton[0]!='\0' && gs_EventFactory )
	{
		for(i=0; i<gs_configuration.list_size; i++)
		{
			if( 0==strcmp(gs_configuration.list[i].ibutton, ibutton))
				break;
		}

		if(gs_configuration.list_size==i)
		{
			EXCTHROW( -1, _("Ibutton is not registered!"));
		}
		else
		{
			char	buffer[512];
			
			sprintf(buffer, "%d;%s;%d;%d;%d", 
							gs_configuration.list[i].user_id,
							gs_configuration.list[i].name,
							gs_configuration.list[i].tester_id,
							gs_configuration.list[i].rights_tester,
							gs_configuration.list[i].rights_dbs); 
			EXCCHECK( eleventdata_new(&pevent_data));
			EXCCHECK( pevent_data->SetStringPtr(pevent_data, buffer));
			EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_USER_IBUTTON", pevent_data));
			EXCCHECK( eleventdata_delete(&pevent_data));
			EXCCHECK( CloseLoginDialog());
			gs_configuration.user_active = i;
		}
	}	

Error:                       
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "EventFunctionCallback"
static SElExceptionPtr EventFunctionCallback( const char* eventName, void* pData)
{
	int32_t			   	error = 0;
	SElExceptionPtr    	pexception = NULL;
	SElEventDataPtr		pdata = NULL;

	if(0==strcmp(eventName, "EVNT_IBUTTON"))
	{
		const char* ibutton = ((SElEventDataPtr)pData)->GetStringPtr(pData);

		EXCCHECK( iButtonLog(ibutton));
	}
	else if(0==strcmp(eventName, "EVNT_USER_LOGIN_VALIDATED"))
	{
		gs_configuration.user_status = US_LOGIN;	

		/* updata info panel */
		if(gs_EventFactory)
		{
			char buffer[256];

			EXCCHECK( eleventdata_new(&pdata));	
			sprintf(buffer, "PANEL_MSG_ADMIN:%s", gs_configuration.list[gs_configuration.user_active].name);
			EXCCHECK( pdata->SetStringPtr(pdata, buffer));
			EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_GUI_INFO_SET", pdata));
		}	
	}
	else if(0==strcmp(eventName, "EVNT_USER_LOGOUT_VALIDATED"))
	{
		gs_configuration.user_status = US_LOGOUT;	

		/* updata info panel */
		if(gs_EventFactory)
		{
			EXCCHECK( eleventdata_new(&pdata));	
			EXCCHECK( pdata->SetStringPtr(pdata, "PANEL_MSG_ADMIN:log out"));
			EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_GUI_INFO_SET", pdata));

			/* set production mode */
			EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_PLUGIN_LEAVE", NULL));
			EXCCHECK( pdata->SetStringPtr(pdata, "panel;2"));
			EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_GUI_PANEL", pdata));
			EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_GUI_PRODUCTION", NULL));
		}	
	}
	else if(0==strcmp(eventName, "EVNT_RESET"))
	{
		EXCCHECK( CleanupPluginConfiguration( &gs_configuration));
		EXCCHECK( ConfigurePlugin( &gs_configuration));
	}
	
Error:   
	eleventdata_delete(&pdata);
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "RegisterEventFunction"
static SElExceptionPtr RegisterEventFunction(void)
{
	int				error = 0;
	SElExceptionPtr	pexception = NULL;
	
	if ( gs_EventFactory )
	{
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_IBUTTON",
														 EventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_USER_LOGIN_VALIDATED",
														 EventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_USER_LOGOUT_VALIDATED",
														 EventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
														 "EVNT_RESET",
														 EventFunctionCallback));
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
#define __FUNC__ "{Login Plugin}::Initialize"
PLUGIN_API SElExceptionPtr PLUGIN_INIT_NAME_CODE( SPluginArgsPtr a_pPluginArgs )
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	const char*		pconfiguration_file = NULL;
	int32_t			main_panel = 0;
	int				monitor_id, top;       

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
	
	EXCCHECK( LoadPanelExWithLanguage(main_panel, "login.uir", PANEL,LOGINlang, &gs_panel));
	EXCCHECKCVI( SetCtrlAttribute (gs_panel, PANEL_PASSWORD, ATTR_MAX_ENTRY_LENGTH, MAX_PASSWORD_LENGTH));
	
	EXCCHECKCVI( GetSystemAttribute( ATTR_PRIMARY_MONITOR, &monitor_id));
    EXCCHECKCVI( GetMonitorAttribute (monitor_id, ATTR_TOP, &top));

	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
									 "EventFactory",
									 sizeof(SElEventFactoryPtr),
									 &gs_EventFactory));
	
	if(gs_Modules)
		gs_pDbs = (SDBSPtr)gs_Modules->Get(gs_Modules, MODULE_DBS); 

	EXCCHECK( CleanupPluginConfiguration( &gs_configuration));
	EXCCHECK( LoadPluginConfiguration(NULL, &gs_configuration));
	EXCCHECK( ConfigurePlugin( &gs_configuration));
	
	EXCCHECK( RegisterEventFunction());

Error:	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Login Plugin}::Run"
PLUGIN_API SElExceptionPtr PLUGIN_RUN_NAME_CODE( SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;

	if( 0==strcmp( uuid, PLUGIN_LOGIN))
	{
		/* check if user logged */
		if(gs_configuration.user_status == US_LOGIN)
		{
			EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_USER_LOGOUT", NULL));				
		}
		else
		{
			EXCCHECK( DisplayLoginDialog());
		}
	}
	else if( 0==strcmp( uuid, ""))
	{
		/* CVI freezing problem with first run */
		EXCCHECKCVI( InstallPopup(gs_panel));
		EXCCHECKCVI( RemovePopup(0));
	}

Error:	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Login Plugin}::Leave"
PLUGIN_API SElExceptionPtr PLUGIN_LEAVE_NAME_CODE( SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;

	EXCCHECKCVI( HidePanel( gs_panel));

Error:	
	EXCRETHROW( pexception);

}

#undef __FUNC__
#define __FUNC__ "{Login Plugin}::Shutdown"
PLUGIN_API SElExceptionPtr PLUGIN_SHUTDOWN_NAME_CODE( SPluginArgsPtr a_pPluginArgs)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;

	EXCCHECK( CleanupPluginConfiguration( &gs_configuration));
	EXCCHECKCVI( DiscardPanel( gs_panel));

Error:	
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{Login Plugin}::Info"
PLUGIN_API SElExceptionPtr PLUGIN_GETINFO_NAME_CODE( SPluginInfoPtr* a_pInfo)
{
	SElExceptionPtr	pexception = NULL;  
	
	if(a_pInfo) EXCCHECK( (*a_pInfo)->SetInfoData(*a_pInfo, &gs_InfoData));
	
Error:	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Login Plugin}::Attribute"
PLUGIN_API SElExceptionPtr PLUGIN_GETATTR_NAME_CODE( uint32_t  attrID, uint32_t size, void* value)
{
	return NULL;
}

#endif

