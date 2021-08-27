#include "msxmldom.h"
#include <eventfactory/EventFactory.h> 
#include <utility.h>
#include <formatio.h>
#include <time.h>
#include <dbs/dbs.h>
#include <guid.h>
#include "product.h"
#include "product_fnc.h"
#include "product_plugin.h"
#include <ansi_c.h>

#define PANEL_TOP		(80 + 754)
#define PANEL_LEFT		11

#define PRODUCT_TEST_PATH			"\\plugins\\test_form.xml"
#define EVENT_NEXT_ITEM				0x123456

#define NB_OF_PANEL 		2  
int							gs_panel[NB_OF_PANEL + 1];  
int32_t    					gs_ActiveProductId;
static SDBSPtr    			gs_pDbs;
static SDBSProductListPtr	gs_pProductList; 
static time_t				gs_last_display_clock = 0;
static SElEventFactoryPtr	gs_EventFactory = NULL;  
static SObjListPtr 			gs_Modules = NULL;  
static SPluginArgsPtr		gs_PluginParam = NULL;

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "AppendVersionInfo"
static SElExceptionPtr AppendVersionInfo(const char* xmlForm)
{
	int32_t			error = 0;
	SElExceptionPtr pexception =  NULL;
	SELStringPtr	pversion = NULL;
	HRESULT			hr = S_OK;
	VARIANT_BOOL	is_xml_success = 1;
	VARIANT         pfile_name_var = CA_VariantEmpty();
	CAObjHandle		pxml = 0;
	CAObjHandle		pnode = 0;
	CAObjHandle		pnode2 = 0;
	CAObjHandle		pnode3 = 0;
	CAObjHandle		pxml_form = 0;

	EXCCHECK( gs_PluginParam->GetArg( gs_PluginParam,
									 "Version",
									 sizeof(pversion),
									 &pversion));

	if(pversion)
	{
		EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml));
		EXCCHECKMSXML( MSXML_IXMLDOMDocumentloadXML (pxml, NULL, pversion->GetPtr(pversion), &is_xml_success));
		EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode(pxml, NULL, "//version", &pnode));

		EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml_form));
		EXCCHECKCVI( CA_VariantSetCString(&pfile_name_var, xmlForm));
		EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (pxml_form, NULL, pfile_name_var, &is_xml_success));
		EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml_form, NULL, "//testseq", &pnode2));

		EXCCHECKMSXML( MSXML_IXMLDOMNodeappendChild(pnode2, NULL, pnode, &pnode3));

		EXCCHECKMSXML( MSXML_IXMLDOMDocumentsave(pxml_form, NULL, pfile_name_var));
	}

Error:
	CA_VariantClear(&pfile_name_var);
	if(pnode3) CA_DiscardObjHandle(pnode3);
	if(pnode2) CA_DiscardObjHandle(pnode2);
	if(pnode) CA_DiscardObjHandle(pnode);
	if(pxml) CA_DiscardObjHandle(pxml);
	if(pxml_form) CA_DiscardObjHandle(pxml_form);
	EXCRETHROW( pexception); 	
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ProductSelected"
static SElExceptionPtr ProductSelected(int32_t product_id, bool_t reset)
{
	int32_t			error = 0;
	SElExceptionPtr pexception =  NULL;
	char			path[MAX_PATHNAME_LEN];
	SElEventDataPtr pdata = NULL;
	
	if(gs_pDbs)
	{
		EXCCHECKCVI( GetProjectDir(path));
		strcat(path, PRODUCT_TEST_PATH); 
		
		EXCCHECK( gs_pDbs->Export->GetTestSeqXML(gs_pDbs->Export, product_id, path));
		
		/* append version to test form */
		EXCCHECK( AppendVersionInfo(path));

		if(!reset)
		{
			if(gs_EventFactory)
			{			
				EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_GUI_PRODUCT_SELECTED", NULL));	

				EXCCHECK( eleventdata_new(&pdata));
				EXCCHECK( pdata->SetStringPtr(pdata, PLUGIN_PRODUCT));
				EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_PLUGIN_LEAVE", pdata));
			}
			gs_pDbs->TesterProductId = product_id;
		}
		else
		{
			if(gs_EventFactory)
				EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_RESET_PRODUCT", NULL));	
		}
	}
	
Error: 
	eleventdata_delete(&pdata);
	EXCRETHROW( pexception); 	
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "OnList"
int CVICALLBACK OnList (int panel, int control, int event,
      void *callbackData, int eventData1, int eventData2)
{
	int32_t      	error = 0;
	SElExceptionPtr	pexception = NULL; 
	int32_t        	product_id, index, count;
	bool_t      	child;
	int32_t      	parent_id; 
   
	switch (event)
	{
  		case EVENT_VAL_CHANGED:
			EXCCHECKCVI( GetCtrlIndex (panel, control, &index));
			EXCCHECKCVI( GetValueFromIndex (panel, control, index, &product_id));
			
			gs_ActiveProductId = product_id;
			
			EXCCHECK( SetProductPath( gs_pProductList, product_id ) );
			EXCCHECK( SetProductPicture( gs_pProductList, product_id ) );
			break;
  		case EVENT_COMMIT:
			EXCCHECKCVI( GetCtrlIndex (panel, control, &index));
			EXCCHECKCVI( GetValueFromIndex (panel, control, index, &product_id));
			
			gs_ActiveProductId = product_id;
			
			if(index==0)
			{
				EXCCHECK( SetProductLevelBackward( gs_pProductList, product_id) );
			}
			else
			{ 
				EXCCHECK( ChildExist( gs_pProductList, product_id, &child, &parent_id) );
				
				if(!child)
				{
					EXCCHECK( ProductSelected(product_id, FALSE));
				}
				else
				{
					EXCCHECK( SetProductLevelForward( gs_pProductList, product_id) );
				}
			}
			break;
		case EVENT_NEXT_ITEM:
			EXCCHECKCVI( GetCtrlIndex (panel, control, &index));
			EXCCHECKCVI( GetNumListItems( panel, control, &count));
			
			index++;
			if( index>=count)
			{
				EXCCHECKCVI( SetCtrlIndex (panel, control, 0));
			}
			else
			{
				EXCCHECKCVI( SetCtrlIndex (panel, control, index));
			}

			OnList( panel, control, EVENT_VAL_CHANGED, NULL, 0, 0);

			break;
	}

Error:
	EXCDISPLAY( pexception);
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "GetLastProductId"
static SElExceptionPtr GetLastProductId(const char* fileXml, int32_t* product_id, int32_t* branch_id)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL;
	HRESULT         hr = 0;
	char            _message[512];
	CAObjHandle     pxml  = 0;
	CAObjHandle     pnode = 0;
	CAObjHandle     pparameter = 0;
	const char*     pfile_xml = (fileXml) ? fileXml : "./plugins/test_form.xml";
	VBOOL           is_xml_success;
	VARIANT         pfile_name_var = CA_VariantEmpty();
	char*           ptext = NULL;
	
	/* configure test sequence from XML */
	EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml));
	EXCCHECKCVI( CA_VariantSetCString(  &pfile_name_var, pfile_xml));
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (pxml, NULL, pfile_name_var, &is_xml_success));
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//testseq", &pnode));
		
	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "product/id", &pparameter));	
	if ( pparameter )
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
		
		if(product_id) *product_id = strtol(ptext, NULL, 10);
					
		EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
		pparameter = 0;
		CA_FreeMemory(ptext);
		ptext=NULL;
	}

	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "product/branch_id", &pparameter));	
	if ( pparameter )
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
		
		if(branch_id) *branch_id = strtol(ptext, NULL, 10);
					
		EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
		pparameter = 0;
		CA_FreeMemory(ptext);
		ptext=NULL;
	}

Error:
	CA_FreeMemory(ptext);
	CA_VariantClear(&pfile_name_var);
	if (pparameter) CA_DiscardObjHandle(pparameter);
	if (pnode) CA_DiscardObjHandle(pnode);
	if (pxml) CA_DiscardObjHandle(pxml);  
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SetLastProduct"
static SElExceptionPtr SetLastProduct(void)
{
	int32_t			error = 0;
	SElExceptionPtr pexception =  NULL;
	int32_t			product_id = 0;
	int32_t			branch_id = 0;

	if(gs_pDbs && gs_pDbs->TesterProductId==0)
	{
		pexception = GetLastProductId( NULL, &product_id, &branch_id);

		if( pexception==NULL)
		{
			EXCCHECK( gs_pDbs->ChangeBranchId(gs_pDbs, branch_id, NULL));
			gs_pDbs->TesterProductId = product_id;
		}
		else
		{
			EXCDELETE( pexception);
		}
	}
	
Error:
	EXCRETHROW( pexception); 	
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ProductDialogShow"
static SElExceptionPtr ProductDialogShow(void)
{
	int32_t			error = 0;
	SElExceptionPtr pexception = NULL;
	
	gs_last_display_clock = clock();

	if(gs_pDbs)
	{
		if(gs_pDbs->TesterProductId==0)
		{
			EXCCHECK( gs_pProductList->ProductGetIdFirstValid(gs_pProductList, &gs_pDbs->TesterProductId));
		}
		else
		{
			bool_t	valid = FALSE;

			EXCCHECK( gs_pProductList->ProductIsValidProduct(gs_pProductList, gs_pDbs->TesterProductId, &valid));
			if(!valid)
			{
				EXCCHECK( gs_pProductList->ProductGetIdFirstValid(gs_pProductList, &gs_pDbs->TesterProductId));
			}
		}

		EXCCHECK( SetProductPath( gs_pProductList, gs_pDbs->TesterProductId ));
		EXCCHECK( SetProductPicture( gs_pProductList, gs_pDbs->TesterProductId ));
		EXCCHECK( SetProductList( gs_pProductList, gs_pDbs->TesterProductId ));
	}

	EXCCHECKCVI( DisplayPanel(gs_panel[PRODUCT]));

Error:
	EXCRETHROW( pexception); 	
}

#define CHECK_DISPLAY_INTERVAL(time) \
	if ( gs_last_display_clock>0 && ((clock() - gs_last_display_clock) / (double)CLOCKS_PER_SEC) < (time/1000.0) ) goto Error; else  

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "EventFunctionCallback"
SElExceptionPtr EventFunctionCallback( const char* eventName, void* pData)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL;
	int32_t			visible;
	SElEventDataPtr pdata = NULL;
	
	if( 0==strcmp(eventName, "EVNT_TESTER_BUTTON_PRESSED_NOK"))
	{
		;
	}
	else if( 0==strcmp(eventName, "EVNT_TESTER_BUTTON_PRESSED_OK"))
	{
		EXCCHECKCVI( GetPanelAttribute(gs_panel[PRODUCT], ATTR_VISIBLE, &visible));

		if( visible)
		{
			CHECK_DISPLAY_INTERVAL(500);
			OnList( gs_panel[PRODUCT], PRODUCT_LIST, EVENT_COMMIT, NULL, 0, 0);
		}
	}
	else if( 0==strcmp(eventName, "EVNT_TESTER_BUTTON_PRESSED_MENU"))
	{
		EXCCHECKCVI( GetPanelAttribute(gs_panel[PRODUCT], ATTR_VISIBLE, &visible));

		if( visible)
			OnList( gs_panel[PRODUCT], PRODUCT_LIST, EVENT_NEXT_ITEM, NULL, 0, 0);
	}
	else if( 0==strcmp(eventName, "EVNT_RESET"))
	{
		if(gs_pDbs->TesterProductId>0)
		{
			bool_t	valid = FALSE;

			EXCCHECK( gs_pProductList->ProductIsValidProduct(gs_pProductList, gs_pDbs->TesterProductId, &valid));
			if(valid)
				EXCCHECK( ProductSelected(gs_pDbs->TesterProductId, TRUE));
		}
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
	SElExceptionPtr	pexception = NULL;
   
	if ( gs_EventFactory )
	{
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
		                                   "EVNT_TESTER_BUTTON_PRESSED_OK",
		                                   EventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
		                                   "EVNT_TESTER_BUTTON_PRESSED_NOK",
		                                   EventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
		                                   "EVNT_TESTER_BUTTON_PRESSED_MENU",
		                                   EventFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
		                                   "EVNT_RESET",
		                                   EventFunctionCallback));  
	}
   
Error:
	EXCRETHROW(pexception);
}   /* RegisterEventFunction */

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
#define __FUNC__ "{Product Plugin}::Initialize"
PLUGIN_API SElExceptionPtr PLUGIN_INIT_NAME_CODE( SPluginArgsPtr a_pPluginArgs )
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	const char*		pconfiguration_file = NULL;
	int32_t			main_panel = 0;
	int				monitor_id, top, left, height, width;  
	
	gs_PluginParam = a_pPluginArgs;

	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
									 "MainPanel",
									 sizeof(main_panel),
									 &main_panel));

	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
									 "WindowHandle",
									 sizeof(hWnd),
									 &hWnd));


	EXCCHECKCVI( gs_panel[PRODUCT] = LoadPanelEx (main_panel, "product.uir", PRODUCT, __CVIUserHInst));
	
		
	EXCCHECKCVI( GetPanelAttribute (main_panel, ATTR_TOP, &top));
	EXCCHECKCVI( GetPanelAttribute (main_panel, ATTR_HEIGHT, &height));

	EXCCHECKCVI( SetPanelAttribute (gs_panel[PRODUCT], ATTR_TOP, 0));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[PRODUCT], ATTR_LEFT, 0));
	EXCCHECKCVI( SetPanelAttribute (gs_panel[PRODUCT], ATTR_HEIGHT, height - 65));

	
	EXCCHECKCVI(SetPanelAttribute (gs_panel[PRODUCT], ATTR_SCALE_CONTENTS_ON_RESIZE, TRUE));
	
	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
									 "EventFactory",
									 sizeof(SElEventFactoryPtr),
									 &gs_EventFactory));
	
	EXCCHECK( a_pPluginArgs->GetArg( a_pPluginArgs,
									 "Modules",
									 sizeof(SObjListPtr),
									 &gs_Modules));
	
	if(gs_Modules)
	{
		gs_pDbs = (SDBSPtr)gs_Modules->Get(gs_Modules, MODULE_DBS);
		if(gs_pDbs)
		{
			gs_pProductList = gs_pDbs->ProductList;
		}
		else
		{
			EXCTHROW( -1, "DBS Module Missing");
		}
	}

	EXCCHECK( RegisterEventFunction());  
	EXCCHECK( SetLastProduct());

Error:	
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{Product Plugin}::Run"
PLUGIN_API SElExceptionPtr PLUGIN_RUN_NAME_CODE( SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
	SElExceptionPtr	pexception = NULL;
	
	if( 0==strcmp( uuid, PLUGIN_PRODUCT))
	{
		EXCCHECK( ProductDialogShow());
	}
				
Error:	
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{Product Plugin}::Leave"
PLUGIN_API SElExceptionPtr PLUGIN_LEAVE_NAME_CODE( SPluginArgsPtr a_pPluginArgs, const char* uuid)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	
	EXCCHECKCVI( HidePanel(gs_panel[PRODUCT]));
	gs_last_display_clock = 0;

Error:	
	EXCRETHROW( pexception);

}

#undef __FUNC__
#define __FUNC__ "{Product Plugin}::Shutdown"
PLUGIN_API SElExceptionPtr PLUGIN_SHUTDOWN_NAME_CODE( SPluginArgsPtr a_pPluginArgs)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;

	if( gs_panel[PRODUCT])
		EXCCHECKCVI( DiscardPanel( gs_panel[PRODUCT]));
	
Error:	
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{Product Plugin}::Info"
PLUGIN_API SElExceptionPtr PLUGIN_GETINFO_NAME_CODE( SPluginInfoPtr* a_pInfo)
{
	SElExceptionPtr	pexception = NULL;  
	
	if(a_pInfo) EXCCHECK( (*a_pInfo)->SetInfoData(*a_pInfo, &gs_InfoData));
	
Error:	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Product Plugin}::Attribute"
PLUGIN_API SElExceptionPtr PLUGIN_GETATTR_NAME_CODE( uint32_t  attrID, uint32_t size, void* value)
{
	return NULL;
}

#endif 


