#include "msxmldom.h"
#include <cvirte.h>
#include <modules/module.h>
#include <guid.h>
#include <exception/Exception.h>
#include <eventfactory/EventFactory.h> 
#include <utility.h>
#include "ibutton.h"

static SIButtonPtr    		gs_pIBUTTON = NULL;
static SElEventFactoryPtr	gs_EventFactory; 

static SElExceptionPtr	ibutton_InitializeConfiguration(struct _SIButton* pIButton, const char* pConfigStore);
static SElExceptionPtr _FUNCC iButtonReceivedCallback(struct _SIButton* me, const char* ibutton);

int __stdcall DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
		{
		case DLL_PROCESS_ATTACH:
			if (InitCVIRTE (hinstDLL, 0, 0) == 0)
				return 0;	  /* out of memory */
			break;
		case DLL_PROCESS_DETACH:
			CloseCVIRTE ();
#if !defined(_CVI_) && defined(_DEBUG) && defined(DUMP_MEMORY_LEAKS)
			_CrtDumpMemoryLeaks();
#endif
			break;
		}
	
	return 1;
}


#undef __FUNC__
#define __FUNC__ "GetModuleAttribute"
SElExceptionPtr	_FUNCC GetModuleAttribute( uint32_t  attrID, uint32_t size, void* value)
{
	switch ( attrID )
	{
		case MODULE_ATTR_VERSION_STRING:
			strncpy((char*)value, "1.0", size);
			break;
		case MODULE_ATTR_MODULE_ID:
			strncpy((char*)value, MODULE_IBUTTON, size);
			break;
		case MODULE_ATTR_MODULE_NAME:
			strncpy((char*)value, "iButton Module", size);
			break;
		case MODULE_ATTR_MODULE_DESCRIPTION:
			strncpy((char*)value, "Module for iDallas chip", size);
			break;
		case MODULE_ATTR_REQUIRED_MODULES:	
			strncpy((char*)value, "", size);
			break;
		case MODULE_ATTR_BUILD_DATETIME:
			strncpy((char*)value, __DATE__" "__TIME__, size);
			break;
	}
	return NULL;
}

#undef __FUNC__
#define __FUNC__ "{IButton}::LoadElcomModule"
SElExceptionPtr	_FUNCC LoadElcomModule(
	const char* configuration,
	void** pInstance,
	SPluginArgsPtr	pParam
)
{
	SElExceptionPtr	pexception = NULL;

	EXCCHECK( pParam->GetArg( pParam,
							  "WindowHandle",
							  sizeof(hWnd),
							  &hWnd));

	EXCCHECK( pParam->GetArg( pParam,
							  "EventFactory",
							  sizeof(SElEventFactoryPtr),
							  &gs_EventFactory));

	EXCCHECK( ibutton_new( &gs_pIBUTTON));
	
Error:                       
	*pInstance = gs_pIBUTTON;
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{IButton}::InitializeElcomModule"
SElExceptionPtr	_FUNCC InitializeElcomModule( void* pInstance, const char* configuration, SPluginArgsPtr	pParam)
{
	SElExceptionPtr    pexception = NULL; 

	EXCCHECK( ibutton_InitializeConfiguration( gs_pIBUTTON, NULL));
	EXCCHECK( gs_pIBUTTON->RegisterCallbackFnc( gs_pIBUTTON, iButtonReceivedCallback));
	EXCCHECK( gs_pIBUTTON->Run( gs_pIBUTTON));
	
Error:
	EXCRETHROW( pexception);   
}

#undef __FUNC__
#define __FUNC__ "{IButton}::UnloadElcomModule"
SElExceptionPtr	_FUNCC UnloadElcomModule( void* pInstance)
{
	SElExceptionPtr    pexception = NULL;        
	
	EXCCHECK( ibutton_delete(&gs_pIBUTTON));
	
Error:
	EXCRETHROW( pexception);   
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ibutton_InitializeConfiguration"
/*!
 * \brief
 * This function loads XML configuration for commands, etc.
 * 
 * \param pIVIBridge
 * Pointer to instance of the IVI Bridge object cretaed by ivibridge_new() function.
 * 
 * \param pConfigStore path to configuration file of IVI Bridge module. If the 
 *                     path is empty default file is "ivibridge.xml"
 * 
 * \return 
 *  Returns an exception. If the function pass NULL pointer is returned.
 */
static SElExceptionPtr	ibutton_InitializeConfiguration(
	struct _SIButton*		pIButton,
	const char*				pConfigStore
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	HRESULT         hr = 0;
	char         	_message[512];
	CAObjHandle     pxml  = 0;
	CAObjHandle     pnode = 0;
	const char*     pfile_name = (pConfigStore) ? pConfigStore : "\\ibutton.xml";
	VBOOL         	is_xml_success = 1;
	VARIANT         pfile_name_var = CA_VariantEmpty();
	char*         	ptext= NULL;
	char			path[MAX_PATHNAME_LEN]; 

	EXCCHECKCVI( GetModuleDir (__CVIUserHInst, path));  
	strcat(path, pfile_name);
	
	EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml));
	EXCCHECKCVI( CA_VariantSetCString(&pfile_name_var, path));
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (pxml, NULL, pfile_name_var, &is_xml_success));
	
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//module[@id='"MODULE_IBUTTON"']", &pnode) );   
	if ( pnode == 0 )
	{
		char message[2048] = "";
		sprintf( message, "Configuration for module '"MODULE_IBUTTON"' in file %s is missing.", pfile_name);
		EXCTHROW( IBUTTON_ERROR_CONFIGURATION_MISSING, message);
	}
	CA_DiscardObjHandle(pnode);
	pnode = 0;

	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//module[@id='"MODULE_IBUTTON"']/command/read", &pnode) );   
	if(pnode)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext( pnode, NULL, &ptext));
		strncpy( pIButton->_readCmd, ptext, IBUTTON_CMD_SIZE);
		CA_FreeMemory(ptext); 
		ptext = NULL;
		CA_DiscardObjHandle(pnode);
		pnode = 0;
	}
	
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//module[@id='"MODULE_IBUTTON"']/command/reset", &pnode) );   
	if(pnode)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext( pnode, NULL, &ptext));
		strncpy( pIButton->_resetCmd, ptext, IBUTTON_CMD_SIZE);
		CA_FreeMemory(ptext); 
		ptext = NULL;
		CA_DiscardObjHandle(pnode);
		pnode = 0;
	}
	
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//module[@id='"MODULE_IBUTTON"']/resource/name", &pnode) );
	if(pnode)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext( pnode, NULL, &ptext));
		if ( strnicmp( ptext, "com", 3 ) == 0 )
			pIButton->COM.port = atoi( ptext + 3);

		CA_FreeMemory(ptext); 
		ptext = NULL;
		CA_DiscardObjHandle(pnode);
		pnode = 0;
	}

Error:
	CA_FreeMemory(ptext);
	CA_VariantClear(&pfile_name_var);
	if (pnode) CA_DiscardObjHandle(pnode);
	if (pxml) CA_DiscardObjHandle(pxml);
	EXCRETHROW( pexception);
} /* ivibridgeInitializeConfiguration */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "iButtonReceivedCallback"
static SElExceptionPtr _FUNCC iButtonReceivedCallback(struct _SIButton* me, const char* ibutton)
{
	SElExceptionPtr	pexception = NULL;
	SElEventDataPtr	pevent_data = NULL;
	
	if ( gs_EventFactory )
	{
		EXCCHECK( eleventdata_new(&pevent_data));
		EXCCHECK( pevent_data->SetStringPtr(pevent_data, ibutton)); 
		EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_IBUTTON", pevent_data));	
	}

Error:
	eleventdata_delete(&pevent_data); 
	EXCRETHROW(pexception);
}
