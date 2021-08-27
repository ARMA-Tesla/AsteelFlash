#include "tester_labels.h"
#include "msxmldom.h"
#include <modules/module.h>
#include <eventfactory/EventFactory.h>
#include <formatio.h>
#include <userint.h>
#include <guid.h>
#include <dbs/dbs.h>
#include <cvinetv.h>
#include <cvirte.h>
#include <stdio.h>
#include "Exception.h"

#pragma warning( push)
#pragma warning( error:4013)
#pragma warning( disable: 4100)

const char* HISTORY = \
"[2007-11-19] KP 1.1\n"
" * Initial Release\n"
"[2008-01-11] KP 1.2\n"
" * Added DLL version informations\n"
" * Removed compiler warnings"
"[2008-07-28] rT 1.3\n"
" * Added MsgFromTester to NetVar\n";

#define DEFAULT_PRINTER		"cab"

#define CVI_NV_TIMEOUT_CONNECT	6000
#define CVI_NV_TIMEOUT_WRITE	100

static SElEventFactoryPtr	gs_EventFactory = NULL;
static SLabelListPtr		gs_pLabels = NULL;
static SPrintersPtr			gs_pPrinters = NULL;
static SDBSPtr 	    		gs_pDbs = NULL;
static CNVSubscriber		gs_netvMsgFromLine = 0;
static CNVWriter			gs_netvMsgFromTester = 0;

static SElExceptionPtr RegisterEventFunction(void);
static SElExceptionPtr NetVarSetup(void);
static SElExceptionPtr NetVarCleanUp(void);

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
#define __FUNC__ "{Labels}::GetModuleAttribute"
SElExceptionPtr	_FUNCC GetModuleAttribute( uint32_t  attrID, uint32_t size, void* value)
{
	switch ( attrID )
	{
		case MODULE_ATTR_VERSION_STRING:
			strncpy((char*)value, "1.3",size);
			break;
		case MODULE_ATTR_MODULE_ID:
			strncpy((char*)value, MODULE_LABELS, size);
			break;
		case MODULE_ATTR_MODULE_NAME:
			strncpy((char*)value, "Labels Module", size);
			break;
		case MODULE_ATTR_MODULE_DESCRIPTION:
			strncpy((char*)value, "Labels module for print labels", size);
			break;
		case MODULE_ATTR_REQUIRED_MODULES:
			//strncpy((char*)value, MODULE_DBS";"MODULE_PRINTERS, size);
			strncpy((char*)value, MODULE_DBS, size);
			break;
		case MODULE_ATTR_BUILD_DATETIME:
			strncpy((char*)value, __DATE__" "__TIME__, size);
			break;
	}
	return NULL;
}

#undef __FUNC__
#define __FUNC__ "{Labels}::LoadElcomModule"
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

	EXCCHECK( labels_new( &gs_pLabels, NULL));
	
Error:                       
	*pInstance = gs_pLabels;
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "{Labels}::InitializeElcomModule"
SElExceptionPtr	_FUNCC InitializeElcomModule( void* pInstance, const char* configuration, SPluginArgsPtr pParam)
{
	SElExceptionPtr	pexception = NULL;
	SPrinterPtr		printer = NULL;

	EXCCHECK( pParam->GetArg( pParam,
							  "EventFactory",
							  sizeof(SElEventFactoryPtr),
							  &gs_EventFactory));

	EXCCHECK( pParam->GetArg( pParam,
							  "DatabaseModule",
							  sizeof(gs_pLabels->pDBS),
							  &(gs_pDbs)));

	gs_pLabels->pDBS = (void*)gs_pDbs;
	
	EXCCHECK( pParam->GetArg( pParam,
							  "PrintersModule",
							  sizeof(SPrintersPtr),
							  &gs_pPrinters)); 

	EXCCHECK( RegisterEventFunction());

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Labels}::UnloadElcomModule"
SElExceptionPtr	_FUNCC UnloadElcomModule( void* pInstance)
{
	SElExceptionPtr	pexception = NULL;
	
	EXCCHECK( NetVarCleanUp());
	EXCCHECK( labels_delete(&gs_pLabels));

Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Labels}::LabelFunctionCallback"
static SElExceptionPtr LabelFunctionCallback( const char* eventName, void* pData)
{
	SElExceptionPtr	pexception = NULL;
	SPrinterPtr		printer = NULL;

	if(0==strcmp(eventName, "EVNT_TESTER_BUTTON_PRESSED_LABEL"))
	{
	EXCCHECK( gs_pPrinters->GetPrinterByIndex(gs_pPrinters, 0, &printer));
	if(printer)
		{
			char		param[5000] = "";
			EIdShift	shift_id = 0;
			bool_t		isOpen = FALSE;
			int         Year,Month,Day;
	
		if(pData)
		{
		const char* buffer = ((SElEventDataPtr)pData)->GetStringPtr(pData);   

		if(strlen(buffer)>0)
			{
			char	data[1024];
			char*	pdata = NULL;
			char*	pdata2 = NULL;
			char*	pdata3 = NULL;
			char*	pdata4 = NULL;
			char*	pdata5 = NULL;
			char*	pdata6 = NULL;
			char*	pdata7 = NULL;
			char*	pdata8 = NULL;
			char*	pdata9 = NULL;
			char*	pdata10 = NULL;
			char*	pdata11 = NULL;
			char*	pdata12 = NULL;
			char 	sHW_ID[20+1] = "";
			
			strcpy_s(data, sizeof(data), buffer);

			/* data */
			pdata = data; 
			//printf("data  %s\n",pdata);
			if (strlen(pdata) == 0)
			{
				EXCTHROW( -1, "Missing Data");
			}/* data */ 
			
			
			//
			///* serial */
			//pdata2 = strchr(pdata, ';');
			//if (pdata2 != NULL)	
			//{
			//	*pdata2 = '\0';
			//	pdata2++;
			//}
			//else
			//{
			//	EXCTHROW( -1, "Missing serial");
			//}/* serial */
			
			
			///* SAPMN */
			//pdata3 = strchr(pdata2, ';');
			//if (pdata3 != NULL)	
			//{
			//	*pdata3 = '\0';
			//	pdata3++;
			//}
			//else
			//{
			//	EXCTHROW( -1, "Missing SAPMN");
			//}/* SAPMN */
			//
			///* HW Identifier */
			//pdata4 = strchr(pdata3, ';');
			//if (pdata4 != NULL)	
			//{
			//	*pdata4 = '\0';
			//	pdata4++;
			//}
			//else
			//{
			//	EXCTHROW( -1, "Missing HW Identifier");
			//}/* HW Identifier */
			//
			///* SAP Supplier */
			//pdata5 = strchr(pdata4, ';');
			//if (pdata5 != NULL)	
			//{
			//	*pdata5 = '\0';
			//	pdata5++;
			//}
			//else
			//{
			//	EXCTHROW( -1, "Missing SAP Supplier");
			//}/* SAP Supplier */
			//
			///* Supplier_Series */
			//pdata6 = strchr(pdata5, ';');
			//if (pdata6 != NULL)	
			//{
			//	*pdata6 = '\0';
			//	pdata6++;
			//}
			//else
			//{
			//	EXCTHROW( -1, "Missing Supplier_Series");
			//}/* Supplier_Series */
			//
			//
			///* VDE_Version */
			//pdata7 = strchr(pdata6, ';');
			//if (pdata7 != NULL)	
			//{
			//	*pdata7 = '\0';
			//	pdata7++;
			//}
			//else
			//{
			//	EXCTHROW( -1, "Missing VDE_Version");
			//}/* VDE_Version */			
			//
			///* Temperature */
			//pdata8 = strchr(pdata7, ';');
			//if (pdata8 != NULL)	
			//{
			//	*pdata8 = '\0';
			//	pdata8++;
			//}
			//else
			//{
			//	EXCTHROW( -1, "Missing Temperature");
			//}/* Temperature */				
			//
			///* Nominal_Voltage */
			//pdata9 = strchr(pdata8, ';');
			//if (pdata9 != NULL)	
			//{
			//	*pdata9 = '\0';
			//	pdata9++;
			//}
			//else
			//{
			//	EXCTHROW( -1, "Missing Nominal_Voltage");
			//}/* Nominal_Voltage */				

			//
			///* Nominal_Power */
			//pdata10 = strchr(pdata9, ';');
			//if (pdata10 != NULL)	
			//{
			//	*pdata10 = '\0';
			//	pdata10++;
			//}
			//else
			//{
			//	EXCTHROW( -1, "Missing Nominal_Power");
			//}/* Nominal_Power */			
			//
			
			///* half_Num_Serie_pFin */
			//pdata11 = strchr(pdata, ';');
			//printf("pdata11  %s\n",pdata11);
			//if (pdata11 != NULL)	
			//{
			//	*pdata11 = '\0';
			//	pdata11++;
			//	printf("pdata11  %s\n",pdata11);
			//}
			//else
			//{
			//	EXCTHROW( -1, "Missing half_Num_Serie_pFin");
			//}/* half_Num_Serie_pFin */		
			//
			///* NUM_SERIE_PFIN */

			//pdata12 = strchr(pdata11, ';');

			//if (pdata12 != NULL)	
			//{
			//	*pdata12 = '\0';
			//	pdata12++;
			//	printf("pdata12  %s\n",pdata12);
			//}
			//else
			//{
			//	EXCTHROW( -1, "Missing SERIAL BOARD VERSION pfin");// YY WW counter piece 
			//}/* NUM_SERIE_PFIN */	
					

			
			
				//sscanf (pdata ,"%04d.%02d.%02d", &Year, &Month, &Day);
				//CopyBytes(sHW_ID, 0, pdata4, 0, 20);
				
				
				sprintf( param, 
								"<LabelParameters>"
									"<NUM_SERIE_PFIN>%s</NUM_SERIE_PFIN>"
								"</LabelParameters>",
								pdata);
			
				EXCCHECK( gs_pLabels->PrintLabel(gs_pLabels, 
												 printer, 
												 "Label:OK", 
												 gs_pDbs->TesterProductId, 
												 param));

			}	
		}
		
		}
	}
	
	else if(0==strcmp(eventName, "EVNT_GUI_PRODUCT_SELECTED")
			|| 0==strcmp(eventName, "EVNT_RESET"))
	{
		EXCCHECK( gs_pLabels->ResetLabels(gs_pLabels));
	}
	else if(0==strcmp(eventName, "EVNT_GUI_KEY_PRESSED"))
	{
		const char* buffer = ((SElEventDataPtr)pData)->GetStringPtr(pData);

		if(buffer && 0==strcmp(buffer, "CTRL_M_KEY"))
		{
			EXCCHECK( gs_pPrinters->GetPrinterByIndex(gs_pPrinters, 0, &printer));
			if(printer)
			{
				EXCCHECK( gs_pLabels->PrintLabel(gs_pLabels, 
												 printer, 
												 "Label:Maintenance", 
												 gs_pDbs->TesterProductId, 
												 NULL));
			}
		}	
	}

Error:         
	EXCRETHROW( pexception);
}

int _FindString(char *Buffer, char *DebutTAG, char *FinTAG, char *StrVal , char *Message)
{
	int index1=0,
	    index2=0;
	
	if(DebutTAG != NULL)
		{
			if((index1=FindPattern (Buffer, 0, -1, DebutTAG, 0, 0))<0)
				{
					sprintf(Message,"none");  
						return -100;
					}
			}
	
	else if (DebutTAG == NULL) 
		{
				index1=0;
				 DebutTAG ="\0";
				}
	
	if((index2 = FindPattern (Buffer, index1, -1, FinTAG, 0, 0))<0)
	{
		sprintf(Message,"none");  
		return -200;
	}
	
	index2=index2-(index1+strlen(DebutTAG));
	index1=index1+strlen(DebutTAG);
	CopyBytes (StrVal,0,Buffer,index1,index2);
	StrVal[index2]='\0';
	
	return index2;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Labels}::GetLabelInfo"
static SElExceptionPtr GetLabelInfo(
	const char* xml_path, 
	char* test_number, 
	char* test_description, 
	char* config,
	char* measured_value, 
	char* hi_limit, 
	char* low_limit,
	char* barcode,
	char* sYear,
	char* sMonth,
	char* sDay,
	char* sHour,
	char* sMin,
	char* sSecond,
	int32_t param_size
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;   	
	HRESULT         hr = 0;
	CAObjHandle     pxml  = 0;
	CAObjHandle     pnode = 0;
	CAObjHandle     pparameter = 0;   
	VBOOL         	is_xml_success = 1;
	VARIANT         pfile_name_var = CA_VariantEmpty();
	char*         	ptext= NULL;
	char 			sTrame[256];
	char 			Message[256];
	int32_t         Year,Month,Day;   
	int32_t         Hour,Min,Second;   
		
	EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml));
	EXCCHECKCVI( CA_VariantSetCString(  &pfile_name_var, xml_path));
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (pxml, NULL, pfile_name_var, &is_xml_success));
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//testseq", &pnode));   
		
	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "product/barcode", &pparameter)); 
		if(pparameter)
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext( pparameter, NULL, &ptext));
			sprintf(barcode, ptext); 
			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
		
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "product/date_time", &pparameter)); 
		if(pparameter)
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext( pparameter, NULL, &ptext));
			
			sscanf (ptext, "%04d-%02d-%02d %02d:%02d:%02d", &Year, &Month, &Day, &Hour, &Min, &Second); //2019-01-21 21:06:00
			sprintf(sYear, "%04d", Year);
			sprintf(sMonth, "%02d", Month);
			sprintf(sDay, "%02d", Day);
				
			sprintf(sHour, "%02d", Hour);
			sprintf(sMin, "%02d", Min);
			sprintf(sSecond, "%02d", Second);
			
			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
	
	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "process/test[failed='1']/name", &pparameter));
	if(pparameter!=0)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
		if(ptext)
		{
			strncpy(test_number, ptext, param_size);
			CA_FreeMemory(ptext);
			ptext=NULL;
		}

		EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
		pparameter = 0;
	}

	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "process/test[failed='1']/description", &pparameter));
	if(pparameter!=0)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
		if(ptext)
		{
			strncpy(test_description, ptext, param_size);
			CA_FreeMemory(ptext);
			ptext=NULL;
		}

		EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
		pparameter = 0;
	}
	
	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "process/test[failed='1']/measure[failed='1']/config", &pparameter));
	if(pparameter!=0)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
		if(ptext)
		{
			/*if (_FindString(ptext,"[","]",sTrame, Message) < 0 )
				sprintf(config, Message);
			else 
				sprintf(config, sTrame); */
			
			sprintf(config, ptext); 
			CA_FreeMemory(ptext);
			ptext=NULL;
		}

		EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
		pparameter = 0;
	}

	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "process/test[failed='1']/measure[failed='1']/value", &pparameter));
	if(pparameter!=0)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
		if(ptext)
		{
			strncpy(measured_value, ptext, param_size);
			CA_FreeMemory(ptext);
			ptext=NULL;
		}

		EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
		pparameter = 0;
	}

	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "process/test[failed='1']/measure[failed='1']/limit/value1", &pparameter));
	if(pparameter!=0)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
		if(ptext)
		{
			strncpy(low_limit, ptext, param_size);
			CA_FreeMemory(ptext);
			ptext=NULL;
		}

		EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
		pparameter = 0;
	}

	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "process/test[failed='1']/measure[failed='1']/limit/value2", &pparameter));
	if(pparameter!=0)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
		if(ptext)
		{
			strncpy(hi_limit, ptext, param_size);
			CA_FreeMemory(ptext);
			ptext=NULL;
		}

		EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
		pparameter = 0;
	}

Error:
	CA_VariantClear(&pfile_name_var);
	if (pparameter) CA_DiscardObjHandle(pparameter);       
	if (pnode) CA_DiscardObjHandle(pnode);
	if (pxml) CA_DiscardObjHandle(pxml);
	EXCRETHROW( pexception);
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Labels}::ProductFunctionCallback"
static SElExceptionPtr ProductFunctionCallback( const char* eventName, void* pData)
{
	SElExceptionPtr	pexception = NULL;
	SPrinterPtr		printer = NULL;

	if(pData)
	{
		const char* buffer = ((SElEventDataPtr)pData)->GetStringPtr(pData);   
		
		EXCCHECK( gs_pPrinters->GetPrinterByIndex(gs_pPrinters, 0, &printer));
		
		EXCCHECK( gs_pLabels->ResetLabels(gs_pLabels));

		if(strlen(buffer)>0 && printer)
		{
			char	data[1024];
			char*	pdata = NULL;
			char*	pdata2 = NULL;
			char*	pdata3 = NULL;
			char*	pdata4 = NULL;

			strcpy_s(data, sizeof(data), buffer);
			
			pdata = data;				/* tester mode */
			pdata2 = strchr(pdata, ';');
			if (pdata2 != NULL)			/* xml path */
			{
				*pdata2 = '\0';
				pdata2++;
			}
			else
			{
				EXCTHROW( -1, "Missing XML Path");
			}
			
			pdata3 = strchr(pdata2, ';');
			if (pdata3 != NULL)			/* test result */
			{
				*pdata3 = '\0';
				pdata3++;
			}
			else
			{
				EXCTHROW( -1, "Missing Test Result");
			}
			
			pdata4 = strchr(pdata3, ';');
			if (pdata4 != NULL)			/* test result */
			{
				*pdata4 = '\0';
				pdata4++;
			}
			else
			{
				//EXCTHROW( -1, "Missing Platett ID");
			}

			if (strlen(pdata) == 0)
			{
				EXCTHROW( -1, "Missing Tester Mode");
			}
			
#if 0    /* formerly excluded lines */
			if(pdata && (0==strcmp(pdata, "PRODUCTION") || 0==strcmp(pdata, "QUALITY_RETEST")))
			{
				if(pdata3 && 0==strcmp(pdata3, "FAULT"))
				{
					char param[1024] = "";
					char test_number[128] = "";
					char test_description[128] = "";
					char config[128] = "";
					char measured_value[128] = "";
					char hi_limit[128] = "";
					char low_limit[128] = "";
					char serial[128] = "";
					char Year[4+1] = "";
					char Month[2+1] = "";
					char Day[2+1] = "";
					char Hour[4+1] = "";
					char Min[2+1] = "";
					char Second[2+1] = "";
					int32_t size = 127;
					

					EXCCHECK( GetLabelInfo(pdata2, test_number, test_description, config, 
										   measured_value, hi_limit, low_limit, serial,
										   Year, Month, Day, Hour, Min, Second, size));

					if (strlen(config) == 0)
						sprintf(config, test_description); 
					 
					sprintf( param,
									"<LabelParameters>\n"
										"<FunctionName>%s</FunctionName>\n"
										"<config>%s</config>\n"
										"<MeasuredValue>%s</MeasuredValue>\n"
										"<LowLimit>%s</LowLimit>\n"
										"<HighLimit>%s</HighLimit>\n"
										"<SerialNumber>%s</SerialNumber>"
										"<PalettId>%s</PalettId>"
										"	<Date>\n"
										"		<Year>%s</Year>\n"
										"		<Month>%s</Month>\n"
										"		<Day>%s</Day>\n"
										"	</Date>\n"
										"	<Time>\n"
										"		<Hours>%s</Hours>\n"
										"		<Minutes>%s</Minutes>\n"
										"		<Seconds>%s</Seconds>\n"
										"	</Time>\n"
									"</LabelParameters>\n",
									test_number,
									config,
									measured_value,
									low_limit,
									hi_limit,
									serial,
									pdata4,
									Year,
									Month,
									Day,
									Hour,
									Min,
									Second);
					
					EXCCHECK( gs_pLabels->PrintLabel(gs_pLabels, 
													 printer, 
													 "Label:NOK", 
													 gs_pDbs->TesterProductId, 
													 param));
				}
			}
#endif   /* formerly excluded lines */
			else if(pdata && 0==strcmp(pdata, "MASTERPART")
				|| 0==strcmp(pdata, "QUALITY_MASTER")) 
			{
				char param[512] = "";
				char result[16] = "empty";
				uint32_t serial = 0;

				if(pdata3 && 0==strcmp(pdata3, "OK"))
				{
					sprintf(result, "OK");
				}
				else if(pdata3 && 0==strcmp(pdata3, "FAULT"))
				{
					sprintf(result, "NOK");
				}

				sprintf( param,
								"<LabelParameters>\n"
									"<TestResultString>%s</TestResultString>\n"
									"<SerialNumber>%d</SerialNumber>\n"
								"</LabelParameters>\n",
								result,
								serial);
				
				EXCCHECK( gs_pLabels->PrintLabel(gs_pLabels, 
												 printer, 
												 "Label:Master", 
												 gs_pDbs->TesterProductId, 
												 param));
			}
#if 0
			else if(0==strcmp(pdata, "QUALITY_MASTER"))
			{
				;
			}
			else if(0==strcmp(pdata, "QUALITY_RETEST")) 
			{
				;
			}
			else if(0==strcmp(pdata, "EXPERTISE_STEP_MODE")) 
			{
				;
			}
			else if(0==strcmp(pdata, "EXPERTISE_GO_MODE")) 
			{
				;
			}
			else if(0==strcmp(pdata, "EXPERTISE_SEQUENCE")) 
			{
				;
			}
			else
			{
				;
			}
#endif
		}
	}

Error:         
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Labels}::RegisterEventFunction"
static SElExceptionPtr RegisterEventFunction(void)
{
   SElExceptionPtr	pexception = NULL;
   
   if ( gs_EventFactory )
   {
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
		                                   "EVNT_TESTER_BUTTON_PRESSED_LABEL",
		                                   LabelFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
		                                   "EVNT_GUI_PRODUCT_SELECTED",
		                                   LabelFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
		                                   "EVNT_RESET",
		                                   LabelFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
		                                   "EVNT_GUI_KEY_PRESSED",
		                                   LabelFunctionCallback));
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
		                                   "EVNT_TEST_SEQUENCE_END",
		                                   ProductFunctionCallback)); 
   }
   
Error:
   EXCRETHROW(pexception);
}   /* RegisterEventFunction */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Labels}::LineMessage"
static SElExceptionPtr LineMessage(const char* line_message)
{
	SElExceptionPtr	pexception = NULL;
	char*			tmp_message = NULL;
	static int32_t	product_id = 0;
	SPrinterPtr		printer = NULL;
	char*			p2start = NULL;
	char*			p2end = NULL;
	int32_t			index = 0,
					size = 0;

	size = strlen(line_message) + 1;
	tmp_message = (char*) calloc( size, sizeof(char));
	strcpy_s(tmp_message, size, line_message);

	p2start = tmp_message;
	p2end = strchr(p2start, ';');
	if (p2end == NULL)
	{
		char serror[256];
		sprintf(serror, "Invalid Message: %s\n", line_message);
		EXCTHROW(-1, serror);
	}
	*p2end++ = '\0';
	index = atoi(p2start);
	p2start = p2end;

	p2end = strchr(p2start, ';');
	if (p2end == NULL)
	{
		char serror[256];
		sprintf(serror, "Invalid Message: %s\n", line_message);
		EXCTHROW(-1, serror);
	}
	*p2end++ = '\0';

	/* Command ID */
	if (!strcmp(p2start, "PRODUCT_LIFE_END_OK"))
	{
		/* Parameters */
		p2start = p2end;

		EXCCHECK( gs_pPrinters->GetPrinterByIndex(gs_pPrinters, 0, &printer));
		if(printer)
		{
			char		param[512] = "";
			uint32_t	counter = 0;
			EIdShift	shift_id = 0;
			bool_t		isOpen = FALSE;

			EXCCHECK( gs_pDbs->Storage->IsShiftOpen(gs_pDbs->Storage, &isOpen, &shift_id));
			if(isOpen)
			{
				product_id = atoi(p2start);

				if (product_id > 0)
				{
					EXCCHECK( gs_pDbs->Storage->GetProductCounter(gs_pDbs->Storage, 
														product_id, 
														&counter));

					sprintf( param,
									"<LabelParameters>"
										"<SerialNumber>%d</SerialNumber>"
										"<ShiftId>%d</ShiftId>"
									"</LabelParameters>", 
									counter, shift_id);

					EXCCHECK( gs_pLabels->PrintLabel(gs_pLabels, 
												   printer,
												   "Label:OK", 
												   product_id, 
												   param));
				}
				else
				{
					char serror[256];
					sprintf(serror, "Product Not Found: '%s'\n", p2start);
					EXCTHROW(-1, serror);
				}
			}
			else
			{
				EXCTHROW(-1, "Shift Not Opened\n");
			}
		}
		else
		{
			EXCTHROW(-1, "Default Printer Not Found\n");
		}
	}
	else
	{
		char serror[512];
		sprintf(serror, "Unknown Command: %s (Msg: %s)\n", p2start, line_message);
		EXCTHROW(-1, serror);
	}

Error:
	if(tmp_message) free(tmp_message);
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Labels}::NetVarFunctionCallback"
static SElExceptionPtr NetVarFunctionCallback( const char* eventName, void* pData)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	CNVData			data = 0;

	if(0==strcmp(eventName, "EVNT_RESET"))
	{
		EXCCHECKCVI( CNVCreateScalarDataValue (&data, CNVString, "EVNT_RESET"));
		EXCCHECKCVI( CNVWrite (gs_netvMsgFromTester, data, CVI_NV_TIMEOUT_WRITE));
	}

Error:      
	if(data) CNVDisposeData(data);
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Labels}::RegisterNetVarEventFunction"
static SElExceptionPtr RegisterNetVarEventFunction(void)
{
   SElExceptionPtr	pexception = NULL;
   
   if ( gs_EventFactory )
   {
		EXCCHECK( gs_EventFactory->RegisterEventFunction(gs_EventFactory,
		                                   "EVNT_RESET",
		                                   NetVarFunctionCallback)); 
   }
   
Error:
   EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Labels}::NetVarDataCallback"
static void CVICALLBACK NetVarDataCallback (void * handle, CNVData data, void * callbackData)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	char*			line_message = NULL;

	EXCCHECKCVI( CNVGetScalarDataValue(data, CNVString, &line_message));

	EXCCHECK( LineMessage(line_message));

Error:
	if (line_message) CNVFreeMemory(line_message);
	CNVDisposeData(data);
	//EXCDBGSTR(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Labels}::NetVarSetup"
static SElExceptionPtr NetVarSetup(void)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	CNVData			data = 0;
	
	EXCCHECKCVI( CNVCreateSubscriber("\\\\localhost\\Tester\\MsgFromLine", NetVarDataCallback, 0, 0, CVI_NV_TIMEOUT_CONNECT, 0, &gs_netvMsgFromLine));
	
	EXCCHECKCVI( CNVCreateWriter("\\\\localhost\\Tester\\MsgFromTester", 0, 0, CVI_NV_TIMEOUT_CONNECT, 0, &gs_netvMsgFromTester));
	EXCCHECKCVI( CNVCreateScalarDataValue (&data, CNVString, ""));
	EXCCHECKCVI( CNVWrite (gs_netvMsgFromTester, data, CVI_NV_TIMEOUT_WRITE));

	EXCCHECK( RegisterNetVarEventFunction());

Error:      
	if(data) CNVDisposeData(data);
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Labels}::NetVarCleanUp"
static SElExceptionPtr NetVarCleanUp(void)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;

	if (gs_netvMsgFromLine)
		EXCCHECKCVI( CNVDispose (gs_netvMsgFromLine));

	if (gs_netvMsgFromTester)
		EXCCHECKCVI( CNVDispose (gs_netvMsgFromTester));

Error:         
	EXCRETHROW( pexception);
}

#pragma warning( pop)
