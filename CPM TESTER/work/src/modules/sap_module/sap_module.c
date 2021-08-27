#include "msxmldom.h"
#include <cvirte.h>
#include <modules/module.h>
#include <guid.h>
#include <exception/Exception.h>
#include <eventfactory/EventFactory.h> 
#include <utility.h>
#include "sap_barcode.h"
#include "sap_converter.h"
#include <regexpr.h>
#include <dbs/dbs.h>
#include <odbc/elodbc.h>

//#include <ansi_c.h>

#define SAPCONVERTER_ERROR_CONFIGURATION_MISSING -5512145


#define MAX_PATTERN_SIZE	64
typedef struct  _SSBCConfig
{
    char       spattern[MAX_PATTERN_SIZE+1];
    char       bpattern[MAX_PATTERN_SIZE+1];
	bool_t	   append_nl;	/*!< Append new line on the end */
} SSBCConfig, *SSBCConfigPtr;

#define	ERROR_SMALL_INTERNAL_BUFFER  -555
#define ERROR_NOT_VALID_PATTERN	-666


static SHBarcodeReaderPtr    gs_pReader = NULL;
static SSAPConverterPtr		 gs_pSAP = NULL;
static SSBCConfigPtr		 gs_pConfig = NULL;
static int					 gs_Count = 0;
static SElEventFactoryPtr	gs_EventFactory; 
static SDBSPtr 	    		gs_pDbs = NULL;

static SElExceptionPtr	sapconverter_InitializeConfiguration(
	SHBarcodeReaderPtr		pReader,
	SSAPConverterPtr		pSAP,
	const char*    				pConfigStore
);
static SElExceptionPtr _FUNCC barcodeReceivedCallback(struct _SHBarcodeReader* me, const char* ibutton);
static SElExceptionPtr EventFunctionCallback( const char* eventName, void* pData);
static SElExceptionPtr RegisterEventFunction(void);
static SElExceptionPtr GetStatus (SDBSPtr pDBS, const char*  barcode, int *pStatus);

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
			strncpy((char*)value, MODULE_SAP_CONVERTER, size);
			break;
		case MODULE_ATTR_MODULE_NAME:
			strncpy((char*)value, "SAP Barcode Converter Module", size);
			break;
		case MODULE_ATTR_MODULE_DESCRIPTION:
			strncpy((char*)value, "Module for SAP link", size);
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
#define __FUNC__ "{SAPConverter}::LoadElcomModule"
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
	

	EXCCHECK( handbr_new( &gs_pReader));
	EXCCHECK( sapconverter_new( &gs_pSAP));
	
Error:                       
	*pInstance = gs_pSAP;
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{SAPConverter}::InitializeElcomModule"
SElExceptionPtr	_FUNCC InitializeElcomModule( void* pInstance, const char* configuration, SPluginArgsPtr	pParam)
{
	SElExceptionPtr    pexception = NULL; 

	EXCCHECK( sapconverter_InitializeConfiguration( gs_pReader, 
													gs_pSAP,
													NULL));
	if ( gs_pReader && gs_pSAP )
	{
	EXCCHECK( gs_pReader->RegisterCallbackFnc( (struct _SHBarcodeReader*)gs_pReader, (BarcodeReaderCallbackPtr)barcodeReceivedCallback));
	EXCCHECK( gs_pReader->Run( gs_pReader));

	EXCCHECK(  gs_pSAP->Open( gs_pSAP));
	EXCCHECK(  gs_pSAP->Start( gs_pSAP));

	EXCCHECK( pParam->GetArg( pParam,
							  "DatabaseModule",
							  sizeof(SDBSPtr),
							  &(gs_pDbs)));
		EXCCHECK( pParam->SetArg( pParam,
								  MODULE_SAP_CONVERTER,
								  ARG_PTR, (void*) gs_pSAP));
	}
Error:
	EXCRETHROW( pexception);   
}

#undef __FUNC__
#define __FUNC__ "{SAPConverter}::UnloadElcomModule"
SElExceptionPtr	_FUNCC UnloadElcomModule( void* pInstance)
{
	SElExceptionPtr    pexception = NULL;        
	
	EXCCHECK( handbr_delete(&gs_pReader));
	//EXCCHECK(  gs_pSAP->Stop( gs_pSAP));
	EXCCHECK(  gs_pSAP->Close( gs_pSAP));
	EXCCHECK( sapconverter_delete(&gs_pSAP));
	free( gs_pConfig); gs_pConfig = NULL; gs_Count = 0;
	
Error:
	EXCRETHROW( pexception);   
}



/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LoadReaderConfiguration"
static SElExceptionPtr LoadReaderConfiguration(
	CAObjHandle			pXML,
	SHBarcodeReaderPtr	pReader  
)
{
    SElExceptionPtr    pexception = NULL;
    CAObjHandle     	list = 0;
    CAObjHandle     	item = 0;
    CAObjHandle     	childs = 0;
    CAObjHandle     	child_item = 0;
//    SSBCConfigPtr   	pconfig = NULL;
    long            	count;
    char*           	pdestination = NULL;
    char*           	ptext = NULL;
	HRESULT				hr = 0;
	long				child_count;
	VBOOL				is_ok;
	ERRORINFO			ei;
	VARIANT				va_str = CA_VariantEmpty();
	int					port;

	
    
	EXCCHECKCOM( MSXML_IXMLDOMDocumentselectNodes( pXML,
                                                   NULL,
                                                            "//module[@id='"MODULE_SAP_CONVERTER"']/reader",
                                                            &list));

	EXCCHECKCOM( MSXML_IXMLDOMNodeListGetlength( list, &ei, &count));
  
    
  if ( count )
  {
        EXCCHECKCOM( MSXML_IXMLDOMNodeListGetitem( list, NULL,  0, &item));
        EXCCHECKCOM( MSXML_IXMLDOMNodeGetchildNodes( item, NULL, &childs));
        
        EXCCHECKCOM( MSXML_IXMLDOMNodeListGetlength( childs, NULL, &child_count));
        for ( ; --child_count >= 0; )
        {
            EXCCHECKCOM( MSXML_IXMLDOMNodeListGetitem( childs, NULL, child_count, &child_item));
            EXCCHECKCOM( MSXML_IXMLDOMNodeGetnodeName(child_item,  NULL,  &ptext));
            if ( strcmp(ptext, "port") == 0 )
            {
                CA_FreeMemory( ptext); ptext = NULL;
				EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(child_item,  NULL,  &ptext));
				if ( *ptext == 'C' ) sscanf(ptext, "COM%d", &port);
				else port = atoi( ptext);
				pReader->COM.port	= port;
            }
			else
			if ( strcmp(ptext, "protocol") == 0 )
			{
				CA_FreeMemory( ptext); ptext = NULL;
				EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(child_item,  NULL,  &ptext));
				pReader->is_usb	= (strcmp( ptext, "USB" ) == 0 );

			}
            CA_FreeMemory( ptext); ptext = NULL;
            CA_DiscardObjHandle( child_item); child_item = 0;
        }    
        
        CA_DiscardObjHandle( childs); childs = 0;
        CA_DiscardObjHandle( item); item = 0;
    }
    
    

Error:
    CA_VariantClear( &va_str);
    if ( ptext ) CA_FreeMemory( ptext);
    if ( child_item ) CA_DiscardObjHandle( child_item);
    if ( childs ) CA_DiscardObjHandle( childs);
    if ( item ) CA_DiscardObjHandle( item);
    if ( list ) CA_DiscardObjHandle( list);
    EXCRETHROW( pexception);
}

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LoadConverterConfiguration"
static SElExceptionPtr LoadConverterConfiguration(
	CAObjHandle         pXML,
	SSAPConverterPtr	pSAP
)
{
    SElExceptionPtr    pexception = NULL;
    CAObjHandle     	list = 0;
    CAObjHandle     	item = 0;
    CAObjHandle     	childs = 0;
    CAObjHandle     	child_item = 0;
    long            	count;
    char*           	pdestination = NULL;
    char*           	ptext = NULL;
	HRESULT				hr = 0;
	int					port;
	long				child_count;
	VBOOL				is_ok;
	VARIANT				va_str = CA_VariantEmpty();
    
    EXCCHECKCOM( MSXML_IXMLDOMDocumentselectNodes( pXML,
                                                            NULL,
                                                            "//module[@id='"MODULE_SAP_CONVERTER"']/converter",
                                                            &list));

	EXCCHECKCOM( MSXML_IXMLDOMNodeListGetlength( list, NULL, &count));
  
    
  if ( count )
  {
        EXCCHECKCOM( MSXML_IXMLDOMNodeListGetitem( list, NULL,  0, &item));
        EXCCHECKCOM( MSXML_IXMLDOMNodeGetchildNodes( item, NULL, &childs));
        
        EXCCHECKCOM( MSXML_IXMLDOMNodeListGetlength( childs, NULL, &child_count));
        for ( ; --child_count >= 0; )
        {
            EXCCHECKCOM( MSXML_IXMLDOMNodeListGetitem( childs, NULL, child_count, &child_item));
            EXCCHECKCOM( MSXML_IXMLDOMNodeGetnodeName(child_item,  NULL,  &ptext));
            if ( strcmp(ptext, "port") == 0 )
            {
                CA_FreeMemory( ptext); ptext = NULL;
				EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(child_item,  NULL,  &ptext));
				if ( *ptext == 'C' ) sscanf(ptext, "COM%d", &port);
				else port = atoi( ptext);
				pSAP->COM.port	= port;

            }
            CA_FreeMemory( ptext); ptext = NULL;
            CA_DiscardObjHandle( child_item); child_item = 0;
        }    
        
        CA_DiscardObjHandle( childs); childs = 0;
        CA_DiscardObjHandle( item); item = 0;
    }
    
    

Error:
    CA_VariantClear( &va_str);
    if ( ptext ) CA_FreeMemory( ptext);
    if ( child_item ) CA_DiscardObjHandle( child_item);
    if ( childs ) CA_DiscardObjHandle( childs);
    if ( item ) CA_DiscardObjHandle( item);
    if ( list ) CA_DiscardObjHandle( list);
    EXCRETHROW( pexception);
}

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LoadConfiguration"
static SElExceptionPtr LoadConfiguration(
                                CAObjHandle pXML,
                                SSBCConfigPtr* pConfig,
                                int     *pCount
                              )
{
    SElExceptionPtr    pexception = NULL;
    CAObjHandle     list = 0;
    CAObjHandle     item = 0;
    CAObjHandle     childs = 0;
    CAObjHandle     child_item = 0;
    SSBCConfigPtr   pconfig = NULL;
    long            count;
    char*           pdestination = NULL;
    char*           ptext = NULL;
	HRESULT			hr = 0;
	long			child_count;
	VBOOL				is_ok;
	VARIANT				va_str = CA_VariantEmpty();
    
    EXCCHECKCOM( MSXML_IXMLDOMDocumentselectNodes( pXML,
                                                            NULL,
                                                            "//module[@id='"MODULE_SAP_CONVERTER"']/barcode",
                                                            &list));
    EXCCHECKCOM( MSXML_IXMLDOMNodeListGetlength( list, NULL, &count));
    
    pconfig = calloc( count, sizeof(SSBCConfig));
	*pCount  = count;    
    
    for ( ; --count >= 0; )
    {
        EXCCHECKCOM( MSXML_IXMLDOMNodeListGetitem( list, NULL, count, &item));
        EXCCHECKCOM( MSXML_IXMLDOMNodeGetchildNodes( item, NULL, &childs));
        
        EXCCHECKCOM( MSXML_IXMLDOMNodeListGetlength( childs, NULL, &child_count));
        for ( ; --child_count >= 0; )
        {
            EXCCHECKCOM( MSXML_IXMLDOMNodeListGetitem( childs, NULL, child_count, &child_item));
            EXCCHECKCOM( MSXML_IXMLDOMNodeGetnodeName(child_item,  NULL,  &ptext));
            if ( strcmp(ptext, "pattern") == 0 )
            {
				CA_FreeMemory( ptext); ptext = NULL;
				EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(child_item,  NULL,  &ptext));
				strncpy(pconfig[count].spattern, ptext, MAX_PATTERN_SIZE);
            }
            else
            if ( strcmp(ptext, "conversion") == 0 )
            {
                CA_FreeMemory( ptext); ptext = NULL;
				EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(child_item,  NULL,  &ptext));
				strncpy(pconfig[count].bpattern, ptext, MAX_PATTERN_SIZE);
            }
			else
            if ( strcmp(ptext, "newline") == 0 )
            {
                CA_FreeMemory( ptext); ptext = NULL;
				EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(child_item,  NULL,  &ptext));
				pconfig[count].append_nl = (strcmp( ptext, "true" ) == 0 );
            }
            
            CA_FreeMemory( ptext); ptext = NULL;
            CA_DiscardObjHandle( child_item); child_item = 0;
            
        }    
        
        CA_DiscardObjHandle( childs); childs = 0;
        CA_DiscardObjHandle( item); item = 0;
    }
    
    

Error:
	 CA_VariantClear( &va_str);
	if ( ptext ) CA_FreeMemory( ptext);
    if ( child_item ) CA_DiscardObjHandle( child_item);
    if ( childs ) CA_DiscardObjHandle( childs);
    if ( item ) CA_DiscardObjHandle( item);
    if ( list ) CA_DiscardObjHandle( list);
    *pConfig = pconfig;
    EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "sapconverter_InitializeConfiguration"
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
static SElExceptionPtr	sapconverter_InitializeConfiguration(
	SHBarcodeReaderPtr		pReader,
	SSAPConverterPtr		pSAP,
	const char*    				pConfigStore
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	HRESULT         hr = 0;
	char         	_message[512];
	CAObjHandle     pxml  = 0;
	CAObjHandle     pnode = 0;
	const char*     pfile_name = (pConfigStore) ? pConfigStore : "\\sap.xml";
	VBOOL         	is_xml_success = 1;
	VARIANT         pfile_name_var = CA_VariantEmpty();
	char*         	ptext= NULL;
	char			path[MAX_PATHNAME_LEN]; 

	EXCCHECKCVI( GetModuleDir (__CVIUserHInst, path));  
	strcat(path, pfile_name);
	
	EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml));
	EXCCHECKCVI( CA_VariantSetCString(&pfile_name_var, path));
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (pxml, NULL, pfile_name_var, &is_xml_success));
	
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//module[@id='"MODULE_SAP_CONVERTER"']", &pnode) );   
	if ( pnode == 0 )
	{
		char message[2048] = "";
		sprintf( message, "Configuration for module '"MODULE_SAP_CONVERTER"' in file %s is missing.", pfile_name);
		EXCTHROW( SAPCONVERTER_ERROR_CONFIGURATION_MISSING, message);
	}
	CA_DiscardObjHandle(pnode);
	pnode = 0;
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//module[@id='"MODULE_SAP_CONVERTER"']//enabled", &pnode) );
	if ( pnode != 0 )
	{
		EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(pnode,  NULL,  &ptext));
		if ( stricmp( ptext, "false") == 0 )
		{
			EXCCHECK( handbr_delete( &gs_pReader));
			EXCCHECK( sapconverter_delete( &gs_pSAP));
			goto Error;
		}
		CA_FreeMemory( ptext); ptext = NULL;
		CA_DiscardObjHandle(pnode);
	}



	EXCCHECK( LoadReaderConfiguration( pxml, pReader));
	EXCCHECK( LoadConverterConfiguration( pxml, pSAP));
	EXCCHECK( LoadConfiguration( pxml, &gs_pConfig, &gs_Count));
	EXCCHECK( RegisterEventFunction());


Error:
	CA_FreeMemory(ptext);
	CA_VariantClear(&pfile_name_var);
	if (pnode) CA_DiscardObjHandle(pnode);
	if (pxml) CA_DiscardObjHandle(pxml);
	EXCRETHROW( pexception);
} /* ivibridgeInitializeConfiguration */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "barcodeReceivedCallback"
static SElExceptionPtr _FUNCC barcodeReceivedCallback(struct _SHBarcodeReader* me, const char* barcode)
{
	SElExceptionPtr	pexception = NULL;
	SElEventDataPtr	pevent_data = NULL;
	
	if ( gs_EventFactory )
	{
		EXCCHECK( eleventdata_new(&pevent_data));
		EXCCHECK( pevent_data->SetStringPtr(pevent_data, barcode)); 
		EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_SAP_CODE_READ", pevent_data));	
	}

Error:
	eleventdata_delete(&pevent_data); 
	EXCRETHROW(pexception);
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
														 "EVNT_SAP_CODE_READ",
														 EventFunctionCallback));
	}

Error:
	EXCRETHROW(pexception);
}	/* RegisterEventFunction */

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "FindConfiguration"
static SElExceptionPtr FindConfiguration(
    SSBCConfigPtr pconfig,
    int           count,
    const char* barcode,
    SSBCConfigPtr* pbcditem
)
{
    SElExceptionPtr    pexception = NULL;
    int             found = 0;
    int             i, pos, length;

    for( i = 0;  i < count; i++)
    {
        
        RegExpr_FindPatternInText( pconfig[i].spattern,
                                   1,
                                   barcode,
                                   -1,
                                   RegExpr_SearchForwards,
                                   RegExpr_MatchSmallestNumChars,
                                   &found,
                                   &pos,
                                   &length);
                                   
        if ( found )
            break;
    }
    
    *pbcditem = ( found ) ? (pconfig+i) : NULL;
        
Error:
    EXCRETHROW( pexception);
}


/*---------------------------------------------------------------------------*/
#define SKP_WS( p) \
    for ( ; *p && isspace(*p); ) p++ 

#undef __FUNC__
#define __FUNC__ "TranslateBarcode"
static SElExceptionPtr TranslateBarcode(
    const char* barcode,
    const char* pattern,
    char buffer[],
    uint32_t bufferSize
)
{
    SElExceptionPtr    pexception = NULL;
    uint32_t        barcode_size = strlen(barcode);
    uint32_t        pattern_size = strlen(barcode);
    const char*     ppattern = pattern; 
    char            output[256] = "";
    char*           pend = output;
    int             used = 0;
    long            si, se;
    
    if (ppattern && *ppattern)
    {
        for ( ; *ppattern; ppattern++ )
        {
            if ( used == sizeof(output)-1)
                EXCTHROW( ERROR_SMALL_INTERNAL_BUFFER, "Small static buffer in TranslateBarcode.");
                
            if ( *ppattern == '[' )
            {
                ppattern++; /* skip [ */
				do {
					se = 0;
                    SKP_WS( ppattern);
                    
                    if ( !isdigit(*ppattern) )
                        EXCTHROW( ERROR_NOT_VALID_PATTERN, "Not Valid pattern");
                        
                    si = strtol(ppattern, (char **)&ppattern, 10);
                    SKP_WS( ppattern);
                    if ( *ppattern == '-')
                    {
                        ppattern++;
                        SKP_WS( ppattern);
                        se = strtol(ppattern, (char **)&ppattern, 10);
                        SKP_WS( ppattern);
                    }
                    *pend++ = barcode[si];
                    used++;                    
                    if ( se > 0 )
                    {
                        for ( si++; si <= se; si++, used++)
                        {
                            if ( used == sizeof(output)-1)
                                EXCTHROW( ERROR_SMALL_INTERNAL_BUFFER, "Small static buffer in TranslateBarcode.");
                            *pend++ = barcode[si];
                        }                            
                    }
                    if ( *ppattern == ',') ppattern++;
                } while ( *ppattern != ']' );
            }
            else
            {
                if ( *ppattern == '\\' )
				{
					switch (*++ppattern)
					{
						case '\\':
                *pend++ = *ppattern;
                used++;                
							break;
						case 'n':
							*pend++ = '\n';
							used++;
							break;
						case 't':
							*pend++ = '\t';
							used++;
							break;
						case 'r':
							*pend++ = '\r';
							used++;
							break;
						case '\'':
							*pend++ = '\'';
							used++;
							break;

					}
				}
				else
				{
                *pend++ = *ppattern;
                used++;                
            }                
            }                
        }
    }
    else
    {
        strncpy(output, barcode, sizeof(output));   
    }
    
Error:
	strncpy(buffer, output, bufferSize);   
    EXCRETHROW( pexception);
} /* TranslateBarcode */


/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "GetStatus"
static SElExceptionPtr GetStatus (SDBSPtr pDBS, const char*  barcode, int *pStatus)
{
	SElExceptionPtr	pexception = NULL;    
	int	error = 0, status = 0, i;
	char			sql[SQL_COMMAND_LENGHT] = "";
	SOdbcPtr		podbc = pDBS->podbc;
	char			newBarcode[SQL_COMMAND_LENGHT] = "";

	EXCCHECK( pDBS->fncLock(pDBS));

	memset(newBarcode, 0, SQL_COMMAND_LENGHT);

	for(i = 0; i < strlen(barcode); i++)
	{
		if (barcode[i] == 10 || barcode[i] == 13)
			break;
		newBarcode[i] = barcode[i];
	}
	
	if (i < strlen(barcode))
		newBarcode[i] = '\0';

	sprintf( sql, "select p.failed from product_status p where p.barcode='%s' order by p.time desc LIMIT 1", newBarcode);
	EXCCHECK( podbc->ExecSQL( podbc, sql));
	CHECKERR( podbc->Fetch(podbc, TRUE));
	
	if ( error != ELODBC_SQL_WARN_NO_DATA_FOUND )
	{
		const unsigned char* pdata = NULL;
		
		pdata = podbc->GetFieldValuePtr(podbc, 1);
		status = ( pdata && *pdata ); /* 0 ok, 1 failed */
	}
  
	EXCCHECK( podbc->Cancel(podbc));

Error:
	pDBS->fncUnLock(pDBS);
	*pStatus = ( error != 0 ) ? -1 : status;
	EXCRETHROW( pexception);  
}


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "EventFunctionCallback"
static SElExceptionPtr EventFunctionCallback( const char* eventName, void* pData)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL;
	SElEventDataPtr	pevent_data = NULL;
	SSBCConfigPtr	pitem = NULL;
	char			buffer[MAX_PATTERN_SIZE+1] = "";
	char			message[MAX_PATTERN_SIZE+1] = "";
	const char* barcode = ((SElEventDataPtr)pData)->GetStringPtr(pData);
	int			status;
	
	EXCCHECK( FindConfiguration( gs_pConfig, gs_Count, barcode, &pitem));

	if ( !pitem )
	{
			sprintf(message, "0;%s", barcode);
			EXCCHECK( eleventdata_new(&pevent_data));
			EXCCHECK( pevent_data->SetStringPtr(pevent_data, message)); 
			EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_SAP_CODE_CHECK", pevent_data));	
	}
	else
	{
		EXCCHECK( TranslateBarcode( barcode, pitem->bpattern, buffer, sizeof(buffer)));
	
#if 1
	if ( pitem && pitem->spattern[0] )
	{
		EXCCHECK( GetStatus( gs_pDbs, barcode, &status));
	}
	else
	{
		status = 0;
	}
	
	if ( status == 0 )
	{
		if ( pitem->append_nl )		/*	Add new line if required */
		{
			int i = strlen(buffer) - 1;
			if ( i > 0 )
			{
				for ( ; !isalnum(buffer[i]); i-- ) 
					;
				buffer[++i] = 10;
				buffer[++i] = '\0';
			}
		}

		EXCCHECK( gs_pSAP->Write( gs_pSAP, buffer));

		EXCCHECK( eleventdata_new(&pevent_data));
		EXCCHECK( pevent_data->SetStringPtr(pevent_data, buffer)); 
		EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_SAP_CODE_TRANSLATED", pevent_data));	
	}
	else
	{
		if ( gs_EventFactory )
		{
			sprintf(message, "%s;%s", (status < 0) ? "-1" : "1", barcode);
			EXCCHECK( eleventdata_new(&pevent_data));
			EXCCHECK( pevent_data->SetStringPtr(pevent_data, message)); 
			EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_SAP_CODE_CHECK", pevent_data));	
		}
	}
#else
	MessagePopup("", buffer);
#endif
	}

Error:
	eleventdata_delete(&pevent_data); 
	EXCRETHROW(pexception);
} 
