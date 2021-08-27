#include <msxmldom.h>
#include <bklib/bklib2.h>
#include <NIDAQmx.h>
#include <sensors/sensors.h>
#include <exception/exception.h>
#include <eventfactory/EventFactory.h>
#include <multithread/multithread.h>
#include "sensors_gui.h"
#include "sensors_gui_plugin.h"
#include <ansi_c.h>

/*PROTOTYPES*/
static HRESULT CVICALLBACK BkTreeNodeClick (
   CAObjHandle caServerObjHandle,
    void *caCallbackData,
    char *keyNode
);

static DWORD WINAPI Thread_RefreshTree(LPVOID param);

#define NB_OF_PANELS		1 
#define BKTREE_HEADER 		"root|Task/Line Name^w300$physical|Physical Name^w500$state|State^w100\n"

/*GLOBALS*/
static SElEventFactoryPtr   gs_EventFactory = NULL;  
static SObjListPtr          gs_Modules = NULL;
static int					gs_panel[NB_OF_PANELS+1];
SPluginInfoPtr				gs_pPI = NULL; 
static SDIOSensorsPtr 		gs_Sensors;
static CAObjHandle			treeHandle = 0;
static HANDLE				hThread = NULL;
static HANDLE				hStopEvent = NULL;

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "BkTreeNodeClick"
static HRESULT CVICALLBACK BkTreeNodeClick (
   CAObjHandle caServerObjHandle,
    void *caCallbackData,
    char *keyNode
)
{
	SElExceptionPtr pexception =  NULL; 
   	int32_t      	error = 0;
	int				state = 0;
	HRESULT			hr;
	char*			parentNode = NULL;
	char			nodetext[10];
	char			*task_name = NULL,
					*task_type = NULL;

    EXCCHECKCOM( BKLib2__DBKTreeGetParentNode(treeHandle, NULL, keyNode, &parentNode));

	if( parentNode && 0==strncmp(parentNode, "Task", sizeof("Task")-1))
	{
		EXCCHECKCOM(BKLib2__DBKTreeGetNodeItemText (treeHandle, NULL, parentNode, "physical", &task_type));
		if( 0==strcmp(task_type, "Digital Output") || 0==strcmp(task_type, "CPV Output")) 
		{
			EXCCHECKCOM(BKLib2__DBKTreeGetNodeItemText (treeHandle, NULL, keyNode, "root", &task_name));
		
			EXCCHECK(gs_Sensors->GetState(gs_Sensors, task_name, &state));
			EXCCHECK(gs_Sensors->SetState(gs_Sensors, task_name, !state));

			sprintf(nodetext,"%d",(!state));
			EXCCHECKCOM(BKLib2__DBKTreeSetNodeItemText( treeHandle, NULL, keyNode,  "state", nodetext, NULL));
		}
	}

Error: 
	EXCDISPLAY( pexception);
   	return 0;
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "DisplayTree"
static SElExceptionPtr DisplayTree(const char* fileXml, const char* GUID)
{
	SElExceptionPtr pexception = NULL;
	HRESULT			hr;
	int32_t			error = 0;
	VBOOL           is_xml_success;
	VARIANT         pfile_name_var = CA_VariantEmpty();
	char*           ptext = NULL;
	const char*		pfile_xml = (fileXml) ? fileXml : "./modules/sensor.xml";
	CAObjHandle     pxml = 0;
	CAObjHandle     plist = 0;
	CAObjHandle     plist2 = 0;
	CAObjHandle     pnode = 0;
	CAObjHandle     pnode2 = 0;
	CAObjHandle     pnode3 = 0;
	CAObjHandle     pparameter = 0;
	short int		AXvalue;
	int				i, j, k, l, length, length2;
	SELStringPtr	pstring = NULL;
	uint32_t 		state = 0;
	char 			row[1024],
					name[128],
					physical[128],
					path[512];
	
	EXCCHECK( elstring_new(BKTREE_HEADER, &pstring));

	/* configure test sequence from XML */
	EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml));
	EXCCHECKCVI( CA_VariantSetCString(  &pfile_name_var, pfile_xml));
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (pxml, NULL, pfile_name_var, &is_xml_success));

	sprintf(path, "//module[@id='%s']/parameters/tasks", GUID);
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, path, &pnode));	

	/* digital inputs */
	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectNodes (pnode, NULL, "task[@type='DI']", &plist));
	EXCCHECKMSXML( MSXML_IXMLDOMNodeListGetlength (plist, NULL, &length));
	EXCCHECKMSXML( MSXML_IXMLDOMNodeListreset (plist, NULL));

	for( i=0; i<length; i++)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeListnextNode (plist, NULL, &pnode2));
		
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode2, NULL, "@name", &pparameter));  
		if ( pparameter )
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));

			sprintf(row, "|Task%d|%s$Digital Input\n", i, ptext);
			EXCCHECK( pstring->Append( pstring, row));

			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
	
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectNodes (pnode2, NULL, "line", &plist2));
		EXCCHECKMSXML( MSXML_IXMLDOMNodeListGetlength (plist2, NULL, &length2));
		EXCCHECKMSXML( MSXML_IXMLDOMNodeListreset (plist2, NULL));
		for(j=0; j<length2; j++)
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeListnextNode (plist2, NULL, &pnode3));

			name[0] = '\0';
			EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode3, NULL, "@name", &pparameter));  
			if ( pparameter )
			{
				EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
				strncpy(name, ptext, 127);

				EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
				pparameter = 0;
				CA_FreeMemory(ptext);
				ptext=NULL;
			}
			
			physical[0] = '\0';
			EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode3, NULL, "@physical", &pparameter));  
			if ( pparameter )
			{
				EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
				strncpy(physical, ptext, 127);

				EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
				pparameter = 0;
				CA_FreeMemory(ptext);
				ptext=NULL;
			}

			sprintf(row, "Task%d|%d_%d|%s$%s$%d\n",
							i,
							i,
							j,
					  		name,
							physical,
							0);

			EXCCHECK( pstring->Append( pstring, row));
		}
	}

	/* digital outputs */
	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectNodes (pnode, NULL, "task[@type='DO']", &plist));
	EXCCHECKMSXML( MSXML_IXMLDOMNodeListGetlength (plist, NULL, &length));
	EXCCHECKMSXML( MSXML_IXMLDOMNodeListreset (plist, NULL));

	for( k=0; k<length; k++)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeListnextNode (plist, NULL, &pnode2));
		
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode2, NULL, "@name", &pparameter));  
		if ( pparameter )
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));

			sprintf(row, "|Task%d|%s$Digital Output\n", k+i, ptext);
			EXCCHECK( pstring->Append( pstring, row));

			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
	
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectNodes (pnode2, NULL, "line", &plist2));
		EXCCHECKMSXML( MSXML_IXMLDOMNodeListGetlength (plist2, NULL, &length2));
		EXCCHECKMSXML( MSXML_IXMLDOMNodeListreset (plist2, NULL));
		for(j=0; j<length2; j++)
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeListnextNode (plist2, NULL, &pnode3));

			name[0] = '\0';
			EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode3, NULL, "@name", &pparameter));  
			if ( pparameter )
			{
				EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
				strncpy(name, ptext, 127);

				EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
				pparameter = 0;
				CA_FreeMemory(ptext);
				ptext=NULL;
			}
			
			physical[0] = '\0';
			EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode3, NULL, "@physical", &pparameter));  
			if ( pparameter )
			{
				EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
				strncpy(physical, ptext, 127);

				EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
				pparameter = 0;
				CA_FreeMemory(ptext);
				ptext=NULL;
			}

			sprintf(row, "Task%d|%d_%d|%s$%s$%d\n",
							k+i,
							k+i,
							j,
					  		name,
							physical,
							0);

			EXCCHECK( pstring->Append( pstring, row));
		}
	}

	/* CPV outputs */
	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectNodes (pnode, NULL, "task[@type='CPV']", &plist));
	EXCCHECKMSXML( MSXML_IXMLDOMNodeListGetlength (plist, NULL, &length));
	EXCCHECKMSXML( MSXML_IXMLDOMNodeListreset (plist, NULL));

	for( l=0; l<length; l++)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeListnextNode (plist, NULL, &pnode2));
		
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode2, NULL, "@name", &pparameter));  
		if ( pparameter )
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));

			sprintf(row, "|Task%d|%s$CPV Output\n", l+i, ptext);
			EXCCHECK( pstring->Append( pstring, row));

			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
	
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectNodes (pnode2, NULL, "line", &plist2));
		EXCCHECKMSXML( MSXML_IXMLDOMNodeListGetlength (plist2, NULL, &length2));
		EXCCHECKMSXML( MSXML_IXMLDOMNodeListreset (plist2, NULL));
		for(j=0; j<length2; j++)
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeListnextNode (plist2, NULL, &pnode3));

			name[0] = '\0';
			EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode3, NULL, "@name", &pparameter));  
			if ( pparameter )
			{
				EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
				strncpy(name, ptext, 127);

				EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
				pparameter = 0;
				CA_FreeMemory(ptext);
				ptext=NULL;
			}
			
			physical[0] = '\0';
			EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode3, NULL, "@physical", &pparameter));  
			if ( pparameter )
			{
				EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
				strncpy(physical, ptext, 127);

				EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
				pparameter = 0;
				CA_FreeMemory(ptext);
				ptext=NULL;
			}

			sprintf(row, "Task%d|%d_%d|%s$%s$%d\n",
							l+i,
							l+i,
							j,
					  		name,
							physical,
							0);

			EXCCHECK( pstring->Append( pstring, row));
		}
	}
	
	EXCCHECKCOM( BKLib2__DBKTreeFromString (treeHandle, NULL, pstring->GetPtr(pstring), "$", "\n", &AXvalue));
	EXCCHECKCOM( BKLib2__DBKTreeSetEnableDragAndDrop (treeHandle, NULL, VFALSE));
	EXCCHECKCOM( BKLib2__DBKTreeExpandLevel (treeHandle, NULL, 1, &AXvalue));

Error:
	elstring_delete(&pstring);
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "DisplayState"
static void CVICALLBACK DisplayState (void *callbackData)
{
	SElExceptionPtr	pexception = NULL;
	int32_t         error = 0;
	HRESULT			hr;
	int 			i=0, j=0;
	uint32_t		state;
	short 			ax;
	char			keyNode[32],
					str_state[32],
					*line_name = NULL,
					*task_name = NULL;
	
	do
	{
		sprintf(keyNode,"Task%d",i);
		
		CA_FreeMemory(task_name);
		task_name = NULL;
			
		EXCCHECKCOM(BKLib2__DBKTreeGetNodeItemText (treeHandle, NULL, keyNode, "root", &task_name));
		if(strcmp(task_name,"")!=0)
		{
			EXCCHECK(gs_Sensors->GetState(gs_Sensors, task_name, &state));
			sprintf(str_state,"0x%X",state);
			EXCCHECKCOM(BKLib2__DBKTreeSetNodeItemText( treeHandle, NULL, keyNode, "state", str_state, &ax));
			do
			{
				sprintf(keyNode,"%d_%d",i,j);
			
				CA_FreeMemory(line_name);
				line_name = NULL;
				
				EXCCHECKCOM(BKLib2__DBKTreeGetNodeItemText (treeHandle, NULL, keyNode, "root", &line_name));
				if(strcmp(line_name,"")!=0)
				{
					EXCCHECK(gs_Sensors->GetState(gs_Sensors,line_name,&state));
					sprintf(str_state,"%d",state);
					EXCCHECKCOM(BKLib2__DBKTreeSetNodeItemText( treeHandle, NULL, keyNode, "state", str_state, &ax));
					j++;
				}
			}while(strcmp(line_name,"")!=0);
		
			j=0;
			i++;
		}
		
	}while(strcmp(task_name,"")!=0);
		
Error:
	EXCDISPLAY( pexception);
}

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Thread_RefreshTree"
static DWORD WINAPI Thread_RefreshTree(LPVOID param)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error;

	while ( TRUE)
	{
		if (WaitForSingleObject(hStopEvent, 500) == WAIT_OBJECT_0)
		{
			ExitThread(THREAD_EXIT_CODE);
		}
		
		EXCCHECKCVI( PostDeferredCall( DisplayState, NULL));
	};
	
Error:
	EXCDISPLAY( pexception);
	return 0;
}

#if defined(_CVI_DLL_) || \
	defined(_WINDLL)
/*---------------------------------------------------------------------------*/
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
			//_CrtSetBreakAlloc(61);
#endif
         if (InitCVIRTE (hinstDLL, 0, 0) == 0)      
            return 0;   /* out of memory */
         break;
      case DLL_PROCESS_DETACH:
         if (!CVIRTEHasBeenDetached())      /* Do not call CVI functions if cvirte.dll has already been detached. */
         {
            DiscardUIObjectsForDLL ();      /* Discard the panels loaded in InitUIForDLL */
            CloseCVIRTE ();      
         }
#if !defined(_CVI_) && defined(_DEBUG) && defined(DUMP_MEMORY_LEAKS)
			_CrtDumpMemoryLeaks();
#endif
         break;
      }

   return 1;
}
 /*---------------------------------------------------------------------------*/
int __stdcall DllEntryPoint(
   HINSTANCE hinstDLL,
   DWORD fdwReason,
   LPVOID lpvReserved
)
{
   /* Included for compatibility with Borland */

    return DllMain (hinstDLL, fdwReason, lpvReserved);
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Sensors Plugin}::Initialize"
PLUGIN_API SElExceptionPtr PLUGIN_INIT_NAME_CODE( SPluginArgsPtr a_pPluginArgs )
{
	SElExceptionPtr pexception = NULL;
	HRESULT			hr;
	int32_t         error = 0;
	int      		main_panel;

	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
	                        "Modules",
	                        sizeof(SObjListPtr),
	                        &gs_Modules));

	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
									 "WindowHandle",
									 sizeof(hWnd),
									 &hWnd));

	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
	                        "CardPanel",
	                        sizeof(main_panel),
	                        &main_panel));

	EXCCHECKCVI( gs_panel[SENSORS] = LoadPanelEx( main_panel, "sensors_gui.uir", SENSORS, __CVIUserHInst));
	EXCCHECKCVI( GetObjHandleFromActiveXCtrl( gs_panel[SENSORS], SENSORS_BKTREE, &treeHandle));

	EXCCHECKCOM( BKLib2__DBKTreeEventsRegOnNodeClick (treeHandle, BkTreeNodeClick, NULL, 1, NULL));

	EXCCHECKCVI( SetPanelAttribute (gs_panel[SENSORS], ATTR_TOP, 0));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[SENSORS], ATTR_LEFT, 0));
	
	if(gs_Modules)
	{
		gs_Sensors = (SDIOSensorsPtr)gs_Modules->Get(gs_Modules, MODULE_SENSORS);

		if(gs_Sensors)
			EXCCHECK( DisplayTree(NULL, MODULE_SENSORS));
	}
	
Error:
   	EXCRETHROW( pexception);
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Sensors Plugin}::Run"
PLUGIN_API SElExceptionPtr PLUGIN_RUN_NAME_CODE(SPluginArgsPtr a_pPluginArgs, const char* uuid )
{
	HRESULT			hr;
	SElExceptionPtr	pexception = NULL;	
	int32_t			error = 0;
	 
	if( 0==strcmp( uuid, PLUGIN_SENSORS) ||
		0==strcmp( uuid, PLUGIN_CARD))
	{
		EXCCHECKCOM( BKLib2__DBKTreeSetSelectedItem (treeHandle, NULL, "Task0"));
		EXCCHECKCVI( DisplayPanel(gs_panel[SENSORS]));
		EXCCHECK( ThreadStart( Thread_RefreshTree, NULL, &hThread, &hStopEvent));
	}

Error:   
   	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Sensors Plugin}::Leave"
PLUGIN_API SElExceptionPtr PLUGIN_LEAVE_NAME_CODE( SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
   	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
   	
	EXCCHECKCVI( HidePanel(gs_panel[SENSORS]));
	EXCCHECK( ThreadStop(&hThread, &hStopEvent));

Error:   
   	EXCRETHROW( pexception);

}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Sensors Plugin}::Shutdown"
PLUGIN_API SElExceptionPtr PLUGIN_SHUTDOWN_NAME_CODE( SPluginArgsPtr a_pPluginArgs)
{
   	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
   
	EXCCHECK( ThreadStop(&hThread, &hStopEvent));
	EXCCHECKCVI( DiscardPanel( gs_panel[SENSORS]));

Error:
	EXCRETHROW( pexception);
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Sensors Plugin}::Info"
PLUGIN_API SElExceptionPtr PLUGIN_GETINFO_NAME_CODE( SPluginInfoPtr* a_pInfo)
{
   	SElExceptionPtr	pexception = NULL;  
	
	if(a_pInfo) EXCCHECK( (*a_pInfo)->SetInfoData(*a_pInfo, &gs_InfoData));
	
Error:	
	EXCRETHROW( pexception);
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Sensors Plugin}::Attribute"
PLUGIN_API SElExceptionPtr PLUGIN_GETATTR_NAME_CODE( uint32_t  attrID, uint32_t size, void* value)
{
   return NULL;
}

#endif
