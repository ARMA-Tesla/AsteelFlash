#include "msxmldom.h"
#include <modules/module.h>
#include <eventfactory/EventFactory.h>
#include <guid.h>
#include <utility.h>
#include "dbus.h"
#include <stdio.h>

static SElEventFactoryPtr	gs_EventFactory = NULL;
static SDBUSListPtr			gs_pDBUS = NULL;

static SElExceptionPtr LoadWithConfiguration(const char* fileName, SDBUSListPtr* pDBUS);

int __stdcall DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
#if (!defined(_CVI_)&&defined(_DEBUG))
			//_CrtSetBreakAlloc(235);
#endif
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
#define __FUNC__ "{DBUS}::GetModuleAttribute"
SElExceptionPtr	_FUNCC GetModuleAttribute( uint32_t  attrID, uint32_t size, void* value)
{
	switch ( attrID )
	{
		case MODULE_ATTR_VERSION_STRING:
			strncpy((char*)value, "1.0", size);
			break;
		case MODULE_ATTR_MODULE_ID:
			strncpy((char*)value, MODULE_DBUS, size);
			break;
		case MODULE_ATTR_MODULE_NAME:
			strncpy((char*)value, "DBUS Module", size);
			break;
		case MODULE_ATTR_MODULE_DESCRIPTION:
			strncpy((char*)value, "Communication module for DBUS", size);
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
#define __FUNC__ "{DBUS}::LoadModule"
SElExceptionPtr	_FUNCC LoadElcomModule(
	const char* configuration,
	void** pInstance,
	SPluginArgsPtr	pParam
)
{
	SElExceptionPtr	pexception = NULL;
	const char*     libary_name = ".\\modules\\DBusDrv.dll";
	
	EXCCHECK( pParam->GetArg( pParam,
							  "WindowHandle",
							  sizeof(hWnd),
							  &hWnd));
	
	EXCCHECK( LoadLibrary_DBUS(libary_name));

	EXCCHECK( LoadWithConfiguration(NULL, &gs_pDBUS));

Error:                       
	*pInstance = gs_pDBUS;
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{DBUS}::InitializeModule"
SElExceptionPtr	_FUNCC InitializeElcomModule( void* pInstance, const char* configuration, SPluginArgsPtr pParam)
{
	SElExceptionPtr	pexception = NULL;

	EXCCHECK( pParam->GetArg( pParam,
							  "EventFactory",
							  sizeof(SElEventFactoryPtr),
							  &gs_EventFactory));

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{DBUS}::UnloadModule"
SElExceptionPtr	_FUNCC UnloadElcomModule( void* pInstance)
{
	SElExceptionPtr	pexception = NULL;
	
	EXCCHECK( gs_pDBUS->Close(gs_pDBUS, NULL));
	EXCCHECK( dbuslist_delete(&gs_pDBUS));

Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LoadWithConfiguration"
static SElExceptionPtr LoadWithConfiguration(
   const char*	fileName,
   SDBUSListPtr*	pDBUS
)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	HRESULT         hr = 0;
	CAObjHandle     pxml  = 0;
	CAObjHandle     pconf_list = 0;
	CAObjHandle     pconfiguration = 0;
	CAObjHandle     pparameter = 0;   
	const char*     pfile_name = (fileName) ? fileName : "\\dbus.xml";
	VBOOL         	is_xml_success = 1;
	VARIANT         pfile_name_var = CA_VariantEmpty();
	char			path[MAX_PATHNAME_LEN]; 
	char*			UDA_name = NULL;
	char*			portCommPartner = NULL;
	char*			PortFlags = NULL;

	EXCCHECKCVI( GetModuleDir (__CVIUserHInst, path));  
	strcat(path, pfile_name);

	EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml));
	EXCCHECKCVI( CA_VariantSetCString(&pfile_name_var, path));
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (pxml, NULL, pfile_name_var, &is_xml_success));
	
	EXCCHECK( dbuslist_new(pDBUS));
	
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectNodes (pxml, NULL,
												     "//module[@id='"MODULE_DBUS"']/settings/configure",
													 &pconf_list));
	
	if(pconf_list != 0)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeListreset(pconf_list, NULL));
		EXCCHECKMSXML( MSXML_IXMLDOMNodeListnextNode(pconf_list, NULL, &pconfiguration));
		
		while (pconfiguration)
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode(pconfiguration, NULL, "@UDA", &pparameter));
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext(pparameter, NULL, &UDA_name));
			CA_DiscardObjHandle(pparameter);
			pparameter = 0;
			
			EXCCHECK( (*pDBUS)->AddChannel(*pDBUS, UDA_name));
			
			EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode(pconfiguration, NULL, "portCommPartner", &pparameter));
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext(pparameter, NULL, &portCommPartner));
			CA_DiscardObjHandle(pparameter);
			pparameter = 0;
			
			EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode(pconfiguration, NULL, "PortFlags", &pparameter));
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext(pparameter, NULL, &PortFlags));
			CA_DiscardObjHandle(pparameter);
			pparameter = 0;

			EXCCHECK( (*pDBUS)->Open(*pDBUS, UDA_name, portCommPartner, PortFlags));
			if (UDA_name)
			{
				CA_FreeMemory(UDA_name); UDA_name = NULL;
			}
			CA_DiscardObjHandle(pconfiguration); pconfiguration = 0;
			EXCCHECKMSXML( MSXML_IXMLDOMNodeListnextNode(pconf_list, NULL, &pconfiguration));
		}

		CA_DiscardObjHandle(pconf_list); pconf_list = 0;
	}

Error:
	CA_VariantClear(&pfile_name_var);
	if (portCommPartner) CA_FreeMemory(portCommPartner);
	if (PortFlags) CA_FreeMemory(PortFlags);
	if (UDA_name) CA_FreeMemory(UDA_name);
	if (pparameter) CA_DiscardObjHandle(pparameter);
	if (pconf_list) CA_DiscardObjHandle(pconf_list);
	if (pconfiguration) CA_DiscardObjHandle(pconfiguration);
	if (pxml) CA_DiscardObjHandle(pxml);
	EXCRETHROW( pexception);
}
