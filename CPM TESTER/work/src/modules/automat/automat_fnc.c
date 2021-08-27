#include "msxmldom.h" 
#include <toolbox.h>
#include <test/eltest.h>
#include <utility.h>  
#include <multithread/multithread.h>
#include <formatio.h>
#include <guid.h>
#include <language/language.h>
#include "automat.h" 
#include "automat_fnc.h"


#define TIME_OUT_PRINTER		    60000 //1min

#define TIMEOUT_PRINTER_CHECK(timeout) \
	if ( ((clock() - _start_clock) / (double)CLOCKS_PER_SEC) > ((double)timeout/1000.0) ) { EXCTHROW( TEST_ERR_TIMEOUT, "Timeout Impriomante!"); } else

#define TIMEOUT_CAMERA		  2000
#define MAX_STEPS 			  200
#define MAX_MESURES 		  20
#define MAX_CHARACTERS		  128

extern 		int					gs_panel[];
extern 		SElEventFactoryPtr	gs_EventFactory;

static 		HANDLE gs_hEventStop = NULL;
int   		Loop = TRUE;
static 		SCamerasPtr		gs_pcameras = NULL;
static		SCamerasPtr		pcameras = NULL;
int 		Steps;
int			Mesures[MAX_STEPS];
int			Used_Step_Test[MAX_STEPS];
int			ID_Step_Test[MAX_STEPS];
int  		Failed_Step_Test[MAX_STEPS];
char		Desc_Step_Test[MAX_STEPS][MAX_CHARACTERS];
int 		Failed_Mesure_Test[MAX_STEPS][MAX_MESURES];
char		Value_Mesure_Test[MAX_STEPS][MAX_MESURES][MAX_CHARACTERS]; 
char 		Desc_Mesure_Test[MAX_STEPS][MAX_MESURES][MAX_CHARACTERS];

int 		Steps_Master;
int			Mesures_Master[MAX_STEPS];
int			Used_Step_Test_Master[MAX_STEPS];
int			ID_Step_Test_Master[MAX_STEPS];
int			Failed_Step_Test_Master[MAX_STEPS];
char		Desc_Step_Test_Master[MAX_STEPS][MAX_CHARACTERS];
int			Failed_Mesure_Test_Master[MAX_STEPS][MAX_MESURES];
char		Value_Mesure_Test_Master[MAX_STEPS][MAX_MESURES][MAX_CHARACTERS]; 
char 		Desc_Mesure_Test_Master[MAX_STEPS][MAX_MESURES][MAX_CHARACTERS];

/*---------------------------------------------------------------------------*/
#define ERROR_SHOW() \
	if(pexception) { \
		if( pexception->GetErrorCode(pexception)<0) /* error */ { \
			SetCtrlVal( gs_panel[AUTOMAT], AUTOMAT_MSG, _("Load test sequence fault!")); \
			SetCtrlVal( gs_panel[AUTOMAT], AUTOMAT_ERROR, pexception->GetErrorMsgPtr(pexception)); } \
		else /* message */ { \
			SetCtrlVal( gs_panel[AUTOMAT], AUTOMAT_MSG, pexception->GetErrorMsgPtr(pexception)); } \
		if(Error) *Error = pexception->GetErrorCode(pexception); \
		EXCDELETE( pexception); } \
	else { \
		SetCtrlVal( gs_panel[AUTOMAT], AUTOMAT_MSG, ""); \
		SetCtrlVal( gs_panel[AUTOMAT], AUTOMAT_ERROR, ""); } 


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ReadMasterResults"
SElExceptionPtr ReadMasterResults(const char* pfile_xml)
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
	char*           	ptext = NULL;
	int            		i = 0;
	int            		j = 0;
	
	/* configure test sequence from XML */
	EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml));
	EXCCHECKCOM( CA_VariantSetCString(&pfile_name_var, pfile_xml));
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (pxml, NULL, pfile_name_var, &is_xml_success));

	
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//testseq", &pnode));
	do
	{
		sprintf(path, "test[@step='%d']/failed", ++i); 
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter));
		if(pparameter)
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
			if(ptext && *ptext)
			{
				Failed_Step_Test_Master[i] = atoi(ptext);
	
				sprintf(path, "test[@step='%d']/id", i); 
				EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter));
				if(pparameter)
					{
					 EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
					if(ptext && *ptext)
						{
						ID_Step_Test_Master[i] = atoi(ptext);
						CA_FreeMemory(ptext);
						ptext=NULL;
						}
					EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
					pparameter = 0;
					}
				
								
				sprintf(path, "test[@step='%d']/used", i); 
				EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter));
				if(pparameter)
					{
					 EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
					if(ptext && *ptext)
						{
						Used_Step_Test_Master[i] = atoi(ptext);
						CA_FreeMemory(ptext);
						ptext=NULL;
						}
					EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
					pparameter = 0;
					}
			
				sprintf(path, "test[@step='%d']/description", i); 
				EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter));
				if(pparameter)
					{
					 EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
					if(ptext && *ptext)
						{
						sprintf (Desc_Step_Test_Master[i], "%s", ptext);
						CA_FreeMemory(ptext);
						ptext=NULL;
						}
					EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
					pparameter = 0;
					}
				
				do
				{
				sprintf(path, "test[@step='%d']/measure[@step='%d']/failed", i, ++j); 
				EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter));
				if(pparameter)
					{
					EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
					if(ptext && *ptext)
						{
						Failed_Mesure_Test_Master[i][j] = atoi(ptext);
						CA_FreeMemory(ptext);
						ptext=NULL;
						}
					EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
					pparameter = 0;
					}
					
				sprintf(path, "test[@step='%d']/measure[@step='%d']/config", i, j); 
				EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter));
				if(pparameter)
					{
					EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
					if(ptext && *ptext)
						{
						sprintf (Desc_Mesure_Test_Master[i][j], "%s", ptext);
						CA_FreeMemory(ptext);
						ptext=NULL;
						}
					EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
					pparameter = 0;
					}
		
				sprintf(path, "test[@step='%d']/measure[@step='%d']/value", i, j); 
				EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter));
				if(pparameter)
					{
					EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
					if(ptext && *ptext)
						{
						sprintf (Value_Mesure_Test_Master[i][j], "%s", ptext);
						CA_FreeMemory(ptext);
						ptext=NULL;
						}
					EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
					pparameter = 0;
					}
			
				else
					{
					Mesures_Master[i] = j;//Number of measures for step i  
					j=0;
					break;
					}
		
				}while(TRUE);
				
				CA_FreeMemory(ptext);
				ptext=NULL;
			}
			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
		}
		else
		{
			Steps_Master = i;//number of test steps
			break;
		}
		
	}while(TRUE);
	

Error:
	CA_VariantClear(&pfile_name_var);
	if (ptext) CA_FreeMemory(ptext);
	if (pparameter) CA_DiscardObjHandle(pparameter);
	if (pnode) CA_DiscardObjHandle(pnode);
	if (pxml) CA_DiscardObjHandle(pxml);
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ReadTestedResults"
SElExceptionPtr ReadTestedResults(	CAObjHandle pxml)
{
	int32_t				error = 0;
	SElExceptionPtr    	pexception = NULL; 
	HRESULT         	hr = 0;
	CAObjHandle     	pnode = 0;
	VARIANT         	pfile_name_var = CA_VariantEmpty();
	VBOOL            	is_xml_success = 1;
	char         		path[256];
	CAObjHandle     	pparameter = 0;
	char*           	ptext = NULL;
	int            		i = 0;
	int            		j = 0;
	
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//testseq", &pnode));
	do
	{
		sprintf(path, "test[@step='%d']/failed", ++i); 
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter));
		if(pparameter)
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
			if(ptext && *ptext)
			{
				Failed_Step_Test[i] = atoi(ptext);
	
				sprintf(path, "test[@step='%d']/id", i); 
				EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter));
				if(pparameter)
					{
					 EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
					if(ptext && *ptext)
						{
						ID_Step_Test[i] = atoi(ptext);
						CA_FreeMemory(ptext);
						ptext=NULL;
						}
					EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
					pparameter = 0;
					}
				
								
				sprintf(path, "test[@step='%d']/used", i); 
				EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter));
				if(pparameter)
					{
					 EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
					if(ptext && *ptext)
						{
						Used_Step_Test[i] = atoi(ptext);
						CA_FreeMemory(ptext);
						ptext=NULL;
						}
					EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
					pparameter = 0;
					}
			
				sprintf(path, "test[@step='%d']/description", i); 
				EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter));
				if(pparameter)
					{
					 EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
					if(ptext && *ptext)
						{
						sprintf (Desc_Step_Test[i], "%s", ptext);
						CA_FreeMemory(ptext);
						ptext=NULL;
						}
					EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
					pparameter = 0;
					}
				
				do
				{
				sprintf(path, "test[@step='%d']/measure[@step='%d']/failed", i, ++j); 
				EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter));
				if(pparameter)
					{
					EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
					if(ptext && *ptext)
						{
						Failed_Mesure_Test[i][j] = atoi(ptext);
						CA_FreeMemory(ptext);
						ptext=NULL;
						}
					EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
					pparameter = 0;
					}
					
				sprintf(path, "test[@step='%d']/measure[@step='%d']/config", i, j); 
				EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter));
				if(pparameter)
					{
					EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
					if(ptext && *ptext)
						{
						sprintf (Desc_Mesure_Test[i][j], "%s", ptext);
						CA_FreeMemory(ptext);
						ptext=NULL;
						}
					EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
					pparameter = 0;
					}
						
				sprintf(path, "test[@step='%d']/measure[@step='%d']/value", i, j); 
				EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter));
				if(pparameter)
					{
					EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
					if(ptext && *ptext)
						{
						sprintf (Value_Mesure_Test[i][j], "%s", ptext);
						CA_FreeMemory(ptext);
						ptext=NULL;
						}
					EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
					pparameter = 0;
					}
			
				else
					{
					Mesures[i] = j;//Number of measures for step i  
					j=0;
					break;
					}
		
				}while(TRUE);
				
				CA_FreeMemory(ptext);
				ptext=NULL;
			}
			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
		}
		else
		{
			Steps = i;//number of test steps
			break;
		}
		
	}while(TRUE);


Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "CompareXML"
SElExceptionPtr CompareXML(char *barcode_Master, CAObjHandle pxml, char* Error)
{
int32_t			   	error = 0;
SElExceptionPtr    	pexception = NULL; 
char 				pfile_xml[512];
int 				i,j,k;
char 				Error_Msg[256];
	
	/*Read Masters Results*/
	sprintf (pfile_xml, ".//Masters//%s.XML", barcode_Master);
	EXCCHECK(ReadMasterResults(pfile_xml));
			
	/* Read Tested Results*/
	EXCCHECK(ReadTestedResults(pxml));
		
	/*compare XML files */
	for(i=1; i<Steps; i++)//producion version
		{
		for(j=1; j<Steps_Master; j++) //
			{
			if (ID_Step_Test[i] == ID_Step_Test_Master[j])
				break;	
			}

			if (Failed_Step_Test_Master[j] != 2 && Used_Step_Test[i] == 1)  /* Used_Step_Test = 1 not skipped for this ref */
			{
				if (Mesures[i] > 0)  /* il ya des mesures sous step i */
				{
					for (k=1; k<Mesures[i]; k++)
					{
						if (Failed_Mesure_Test_Master[j][k] == 0 || Failed_Mesure_Test_Master[j][k] == 1)/* comparer par Failed */ 
						{
							
							if (Failed_Mesure_Test[i][k] != Failed_Mesure_Test_Master[j][k])
							{
							sprintf (Error_Msg,"Resultat : %s = %d '(Attendu  : %s = %d)'", Desc_Mesure_Test[i][k], 
																											 Failed_Mesure_Test[i][k], 
																						 					 Desc_Mesure_Test_Master[j][k], 
																						 					 Failed_Mesure_Test_Master[j][k]);
							sprintf (Error, "%s", Error_Msg);
							EXCTHROW( -1, "Master Part NOK");
							}
						}
				
						else if (Failed_Mesure_Test_Master[j][k] == 3)/* comparer par Value exp CAMERA test*/ 
						{
								 
							if (strcmp (Value_Mesure_Test[i][k] , Value_Mesure_Test_Master[j][k]))
							{
							sprintf (Error_Msg,"Valeur : %s = %s '(Attendu : %s = %s)'", Desc_Mesure_Test[i][k], 
																						 			   Value_Mesure_Test[i][k], 
																						 			   Desc_Mesure_Test_Master[j][k], 
																						 			   Value_Mesure_Test_Master[j][k]);
							sprintf (Error, "%s", Error_Msg);
							EXCTHROW( -1, "Master Part NOK");
							}		 
								 
						}
				
						else if (Failed_Mesure_Test_Master[j][k] == 2)
						{
						/* nothing */	
						}
					
					}
				}
			
				else  /* pas des mesures sous step i */
				{
					if (Failed_Step_Test[i] != Failed_Step_Test_Master[j])
					{
					sprintf (Error_Msg,"Step : %s = %d '(Attendu : %s = %d)'", Desc_Step_Test[i], 
																			   Failed_Step_Test[i], 
																			   Desc_Step_Test_Master[j],
																			   Failed_Step_Test_Master[j]);
					sprintf (Error, "%s", Error_Msg);
					EXCTHROW( -1, "Master Part NOK");
					}
				}
			}
			
		}

Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Fill_CSV_Steps_Label"
SElExceptionPtr Fill_CSV_Steps_Label (CAObjHandle pxml, int pfile_csv, int Station)
{
	int32_t				error = 0;
	SElExceptionPtr    	pexception = NULL; 
	HRESULT         	hr = 0;
	//CAObjHandle     	pxml = 0; 
	CAObjHandle     	pnode = 0;
	VBOOL            	is_xml_success = 1;
	char         		path[256];
	char         		line[MAX_LENGTH_CSV];
	CAObjHandle     	pparameter = 0;
	char*           	ptext = NULL;
	char*           	Step_Description = NULL;
	int            		i = 0;
	int            		j = 0;
	int 				Test, SubTest;

	
		/* initialize line */
		memset(line, 0X00, MAX_LENGTH_CSV);
		sprintf (line, "");

		/* Read from XML test Steps */
		EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//testseq", &pnode));
	
		do
		{
		sprintf(path, "test[@step='%d']/name", ++i); 
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter));
		if(pparameter)
			{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
			if(ptext && *ptext)
				{
				sscanf(ptext, "%d.%d", &Test, &SubTest);
			
				if (Station == Test/10 - 1) /*check if station to Run RnR */
					{
					sprintf(path, "test[@step='%d']/description", i); 
					EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter));
					if(pparameter)
						{
							EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &Step_Description))

							do
							{
							sprintf(path, "test[@step='%d']/measure[@step='%d']/config", i, ++j); 
							EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter));
							if(pparameter)
								{
								EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
								if(ptext && *ptext)
									{
										if (j==1)
										{
										/* add description in CSV file Line 1*/
										strcat(line, Step_Description);
										strcat(line, ";");
										CA_FreeMemory(Step_Description);
										Step_Description=NULL;
										}
										else 
										{
										/* add comma [;] */
										strcat(line, "              ;");
										CA_FreeMemory(ptext);
										ptext=NULL;
										}
									}
								
								EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
								pparameter = 0;
								}
							
							else
								{
									if (Station == 0)	  //only for station 10 because no measurement exists
									{
									/* add description in CSV file Line 1*/
									strcat(line, Step_Description);
									strcat(line, ";");
									CA_FreeMemory(Step_Description);
									Step_Description=NULL;
									}
								j=0;
								break;
								}
							
							}while(TRUE);
		
						EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
						pparameter = 0;
						}
					}
				CA_FreeMemory(ptext);
				ptext=NULL;
				}
		EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
		pparameter = 0;
			}
	
		else
		{
		//write line 
		WriteLine (pfile_csv, line, MAX_LENGTH_CSV-1);
		//CloseFile (pfile_csv);
		break;
		}
		
	}while(TRUE);

Error:
	if (ptext) CA_FreeMemory(ptext);
	if (pparameter) CA_DiscardObjHandle(pparameter);
	if (pnode) CA_DiscardObjHandle(pnode);
	//if (pxml) CA_DiscardObjHandle(pxml);
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Fill_CSV_Mesures_Label"
SElExceptionPtr Fill_CSV_Mesures_Label (CAObjHandle pxml, int pfile_csv, int Station)
{
	int32_t				error = 0;
	SElExceptionPtr    	pexception = NULL; 
	HRESULT         	hr = 0;
	char         		line[MAX_LENGTH_CSV];
	char         		min[32];
	char         		max[32];
	CAObjHandle     	pnode = 0;
	VBOOL            	is_xml_success = 1;
	char         		path[256];
	CAObjHandle     	pparameter = 0;
	char*           	ptext = NULL;
	char*           	Step_Description = NULL;
	int            		i = 0;
	int            		j = 0;
	int 				Test, SubTest;

	
		/* initialize line */
		memset(line, 0X00, MAX_LENGTH_CSV);
		sprintf (line, "");
											
		/* Read from XML test Steps */
		EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//testseq", &pnode));
	
		do
		{
		sprintf(path, "test[@step='%d']/name", ++i); 
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter));
		if(pparameter)
			{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
			if(ptext && *ptext)
				{
				sscanf(ptext, "%d.%d", &Test, &SubTest);
			
				if (Station == Test/10 - 1) /*check if station to Run RnR */
					{
					sprintf(path, "test[@step='%d']/description", i); 
					EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter));
					if(pparameter)
						{
							do
							{
								sprintf(path, "test[@step='%d']/measure[@step='%d']/config", i, ++j); 
								EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter));
								if(pparameter)
								{
									EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
									if(ptext && *ptext)
										{
											/* add Mesures Config Line 2 */
											strcat(line, ptext);
											CA_FreeMemory(ptext);
											ptext=NULL;
										}
									EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
									pparameter = 0;
								}
							
								sprintf(path, "test[@step='%d']/measure[@step='%d']/limit/value1", i, j);
								EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter));
								if(pparameter)
								{
									EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
									if(ptext && *ptext)
										{
											/* add min */
											sprintf (min,"[%s - ", ptext);
											strcat(line, min);
											CA_FreeMemory(ptext);
											ptext=NULL;
										}
									EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
									pparameter = 0;
								}	
							
								sprintf(path, "test[@step='%d']/measure[@step='%d']/limit/value2", i, j);
								EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter));
								if(pparameter)
								{
									EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
									if(ptext && *ptext)
										{
											/* add max */
											sprintf (max,"%s]", ptext);
											strcat(line, max);
											strcat(line, ";");
											CA_FreeMemory(ptext);
											ptext=NULL;
										}
									EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
									pparameter = 0;
								}	
							
								else
								{
									if (Station == 0)	  //only for station 10 because no measurement exists
									{
									/* add comma [;] */
									strcat(line, "              ;");
									}
									j=0;
									break;
								}
							
							}while(TRUE);
		
						EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
						pparameter = 0;
						}
					}
				CA_FreeMemory(ptext);
				ptext=NULL;
				}
		EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
		pparameter = 0;
			}
	
		else
		{
		/* write line */ 
		WriteLine (pfile_csv, line, MAX_LENGTH_CSV-1);
		//CloseFile (pfile_csv);
		break;
		}
		
	}while(TRUE);

Error:
	if (ptext) CA_FreeMemory(ptext);
	if (pparameter) CA_DiscardObjHandle(pparameter);
	if (pnode) CA_DiscardObjHandle(pnode);
	//if (pxml) CA_DiscardObjHandle(pxml);
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LoadTestSequence"
SElExceptionPtr LoadTestSequence(
	const char* fileXml, 
	STestSequencePtr pTestSeq, 
	SObjListPtr pModules,
	bool_t		reset_panel,
	int32_t*	Error
)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL;
	HRESULT         hr = 0;
	CAObjHandle     pxml  = 0;
	CAObjHandle     pnode = 0;
	CAObjHandle     pparameter = 0;
	CAObjHandle     pparameter1 = 0;
	CAObjHandle     pparameter2 = 0;
	const char*     pfile_xml = (fileXml) ? fileXml : "test_form.xml";
	VBOOL           is_xml_success;
	VARIANT         pfile_name_var = CA_VariantEmpty();
	char*           ptext = NULL;
	char*           ptext2 = NULL;
	char         	path[256];
	int            	i = 0;
	int            	j = 0;
	int32_t 		step=0;
	STestParam		TestParam = {0};  
	SElEventDataPtr	pdata = NULL;
	char			testdll_path[MAX_PATHNAME_LEN] = "";
	char			picture_path_Left[MAX_PATHNAME_LEN] = "";
	char			picture_path_Right[MAX_PATHNAME_LEN] = "";
	char			buffer[512];
	HANDLE			hThread = NULL;
	int32_t			jig_code = 0, 
					jig_required = 0;
	
	


	
	EXCCHECKCVI( ProcessSystemEvents());

	ERROR_SHOW();  

	EXCCHECK( eleventdata_new(&pdata));	

	/* unload old test panel */
	EXCCHECK( pTestSeq->UnLoad(pTestSeq));

	/* configure test sequence from XML */
	EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml));
	EXCCHECKCOM( CA_VariantSetCString(&pfile_name_var, pfile_xml));
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (pxml, NULL, pfile_name_var, &is_xml_success));
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//testseq", &pnode));
		

	/* set panel info */
		
			
	if(gs_EventFactory && reset_panel)
	{
		
	

		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "global/parameter[name='Code_AsteelFlash']/value", &pparameter));
		if(pparameter)
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
			if(ptext)
			{
				sprintf(buffer, "PANEL_MSG_CODE_AsteelFlash:%s", ptext);
				EXCCHECK( pdata->SetStringPtr(pdata, buffer));
				EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_GUI_INFO_SET", pdata));
				CA_FreeMemory(ptext);
				ptext=NULL;
			}
			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
		}


		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "global/parameter[name='Code_AsteelFlash']/value", &pparameter));
		if ( pparameter )
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
			if(ptext && *ptext)
			{
				sprintf(pTestSeq->Code_AsteelFlash, ptext);
				CA_FreeMemory(ptext);
				ptext=NULL;
			}

			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
		}
		
	
	}
		/* cHECK SMB  */
    EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "global/parameter[name='SMB']/value", &pparameter));		
	if ( pparameter )
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
		if(ptext && *ptext)
		{
			sprintf(pTestSeq->SMB_CHECK, ptext);
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
		
		EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
		pparameter = 0;
	}
	
		/* Carte SMB */
	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "global/parameter[name='SMB']/value", &pparameter));		
	if ( pparameter )
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
		if(ptext && *ptext)
		{
			sprintf(buffer, "PANEL_MSG_SMM:%s", ptext);
			EXCCHECK( pdata->SetStringPtr(pdata, buffer));
			EXCCHECK( gs_EventFactory->PulseEvent(gs_EventFactory, "EVNT_GUI_INFO_SET", pdata));
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
		
		EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
		pparameter = 0;
	}

	/* Product Customer Ref */
	
	
	
	 //Camera Ref */

/*	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "global/parameter[name='CodeFichierJob']/value", &pparameter));		
	if ( pparameter )
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
		if(ptext && *ptext)
		{

			step = atoi(ptext);
			if(step!=100)
			CameraSetRef(pcameras, step, TIMEOUT_CAMERA);
			else
			CameraSetRef(pcameras, 0, TIMEOUT_CAMERA);			
			ptext=NULL;
		}
		
		EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
		pparameter = 0;
	}*/	

	/* test dll name */
	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "global/parameter[name='ProductTestFile']/value", &pparameter));		
	if ( pparameter )
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
		if(ptext && *ptext)
		{
			EXCCHECKCVI( GetProjectDir(testdll_path)); 
			strcat(testdll_path, "\\plugins\\");
			strcat(testdll_path, ptext);
			
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
		
		EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
		pparameter = 0;
	}
	else
	{
		EXCTHROW(ERROR_AUTOMAT_NO_LIBRARY_NAME, _("Library name does not specified in xml file!"));
	}
			
	/* load new test sequence from DLL */ 
	EXCCHECK( pTestSeq->Load(pTestSeq, testdll_path, pModules, gs_EventFactory));

	do
	{
		sprintf(path, "process [@stage='%d']", ++i);
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter1));
		if(pparameter1)
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter1, NULL, &ptext));
			if(ptext && *ptext)
			{
				do
				{
					sprintf(path, "process [@stage='%d']/test[@step='%d']/name", i, ++j); 
					EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter2));
					if(pparameter2)
					{
						EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter2, NULL, &ptext));
						if(ptext && *ptext)
						{
							EXCCHECK(pTestSeq->SetTestActive(pTestSeq, i-1, ptext, j));
							
							CA_FreeMemory(ptext);
							ptext=NULL;
						}
						EXCCHECKCVI( CA_DiscardObjHandle(pparameter2));
						pparameter2 = 0;
					}
					else
					{
						j=0;
						break;
					}
		
				}while(TRUE);	
				
				
			CA_FreeMemory(ptext);
			ptext=NULL;
			}
		
			EXCCHECKCVI( CA_DiscardObjHandle(pparameter1));
			pparameter1 = 0;
		}
		
		else
		{
			break;
		}
		
	}while(TRUE);
		
			
//#if 0    /* formerly excluded lines */
	/* check validation */
	TestParam.pTicket = pxml;
	
	EXCCHECK( pTestSeq->UsageValidation(pTestSeq, &TestParam));  	

	/* load picture product Left */  
	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "global/parameter[name='ProductPicture']/value", &pparameter));
	if ( pparameter )
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
		if(ptext && *ptext)
		{
			EXCCHECKCVI( GetProjectDir(picture_path_Left)); 
			strcat(picture_path_Left, "\\images\\");
			strcat(picture_path_Left, ptext);
			
			CA_FreeMemory(ptext);
			ptext=NULL;
		}		
		EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
		pparameter = 0;
	}
	
//#endif   /* formerly excluded lines */	
	/* load test panel */  
	EXCCHECK( pTestSeq->IniPanel(pTestSeq, picture_path_Left, picture_path_Right, reset_panel));


Error:
	eleventdata_delete(&pdata);
	CA_VariantClear(&pfile_name_var);
	if (ptext) CA_FreeMemory(ptext);
	if (pparameter) CA_DiscardObjHandle(pparameter);
	if (pnode) CA_DiscardObjHandle(pnode);
	if (pxml) CA_DiscardObjHandle(pxml);
	if (pexception) ERROR_SHOW();
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "CheckDirectory"
static SElExceptionPtr CheckDirectory(const char* directory)
{
	int32_t			   	error = 0;
	SElExceptionPtr    	pexception = NULL; 
	int					size;

	EXCCHECKCVI( FileExists (directory, &size));
	if(size==-1)
	{
		/* create directory */	
		EXCCHECKCVI( MakeDir(directory));
	}
	
Error:
	EXCRETHROW( pexception);
}
void copyTestOption(struct _SSledge* me)
{
	
	me->pTestParam->palette = atoi(me->PalletID);
	me->pTestSeq->LockTest = me->_Lock;
	me->pTestParam->fault_continue = FALSE;
	me->pTestParam->Autorisation_retest = FALSE;
	me->pTestParam->Skip_All_Test = me->Skip;
}
void copyFieldData(struct _SSledge* me)
{
	memcpy(me->pTestParam->pFieldLabel, me->pFieldLabel, sizeof(SFieldLabel));
	sprintf(me->pTestParam->SMM, me->SMM);	
	sprintf(me->pTestParam->Board_Ver_pFin, me->Board_Ver_pFin);
	sprintf(me->pTestParam->Serial_Board_Ver_sFin, me->Serial_Board_Ver_sFin);
	sprintf(me->pTestParam->Num_Serie_sFin, me->Num_Serie_sFin);
	me->pTestParam->id_Board_pFin = me->id_Board_pFin;
	
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "RunTestSequence"
SElExceptionPtr RunTestSequence(
	const char* fileFormXml, 
	struct _SSledge* me, 
	SAutomatPtr pAutomat, 
	SElEventFactoryPtr pEventFactory
)
{
	int32_t			   	error = 0;
	SElExceptionPtr    	pexception = NULL, 
						ptest_exception = NULL,
						pPrinter_exception = NULL;
	EAutomatMode 		AutomatMode = pAutomat->Mode;
	HRESULT         	hr = 0;
	CAObjHandle     	pxml = 0; 
	CAObjHandle     	pnode = 0; 
	const char*     	pfile_form_xml = (fileFormXml)? fileFormXml : "test_form.xml";
	VBOOL            	is_xml_success = 1;
	VARIANT         	pfile_name_var = CA_VariantEmpty();
	char				project_dir[MAX_PATHNAME_LEN],
						result_dir[MAX_PATHNAME_LEN] = "",
						csv_dir[MAX_PATHNAME_LEN],
						fileName[MAX_PATHNAME_LEN],
						sdate_time[256],
						buffer[2048] = "",
						AutomatModeStr[AUTOMAT_MODE_SIZE][32] = AUTOMAT_MODE_STR;
	SYSTEMTIME			st;
	bool_t				is_locked = FALSE;
	int                 serial;
	clock_t 			_start_clock;
	int                UsedSeq[NB_OF_STATIONS];
	int                StopFailSeq[NB_OF_STATIONS];
	int                station;
	
	char               data[128];
	
	me->pTestSeq = calloc(1, sizeof(STestSequence));
	EXCCHECKALLOC( me->pTestSeq);
	me->pTestParam = calloc(1, sizeof(STestParam));
	EXCCHECKALLOC( me->pTestParam);
	
	memcpy(me->pTestSeq, pAutomat->pTestSeq,sizeof(STestSequence));
	memcpy(me->pTestParam, &pAutomat->TestParam,sizeof(STestParam));

	me->pTestParam->Origin_Fault = -1;
	
	/* check destination directory */
	GetLocalTime(&st);
	EXCCHECKCVI(GetProjectDir(project_dir));
	switch(AutomatMode)
	{
		case AUTOMAT_PRODUCTION:
		{	
			sprintf(result_dir, PATH_PRODUCTION, project_dir, st.wYear, st.wMonth, st.wDay);
		}	
			break;
		case AUTOMAT_QUALITY_MASTER:
		case AUTOMAT_QUALITY_RETEST:
		case AUTOMAT_MASTERPART:
			sprintf(result_dir, PATH_QUALITY, project_dir, st.wYear, st.wMonth, st.wDay);
			break;
		case AUTOMAT_EXPERTISE_GO_MODE:
		case AUTOMAT_EXPERTISE_STEP_MODE:
			sprintf(result_dir, PATH_EXPERTISE, project_dir, st.wYear, st.wMonth, st.wDay);
			break;
		case AUTOMAT_EXPERTISE_SEQUENCE:
		case AUTOMAT_EXPERTISE_RNR_STUDY:
			sprintf(result_dir, PATH_RNR, project_dir, st.wYear, st.wMonth, st.wDay);
			break;
	}
	EXCCHECK( CheckDirectory(result_dir)); 

	/* open xml file */
	EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml));
	EXCCHECKCOM( CA_VariantSetCString(&pfile_name_var, pfile_form_xml));
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (pxml, NULL, pfile_name_var, &is_xml_success));

	/* mode */
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode( pxml, NULL, "/testseq/product/mode", &pnode));
	if(pnode)
	{
		char smode[AUTOMAT_MODE_SIZE][32] = AUTOMAT_MODE_DESCRIPTION_STR;

		EXCCHECKMSXML( MSXML_IXMLDOMNodeSettext( pnode, NULL, smode[AutomatMode]));

		EXCCHECKCVI( CA_DiscardObjHandle(pnode));
		pnode = 0;
	}
	/* date_time */
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode( pxml, NULL, "/testseq/product/date_time", &pnode));
	if(pnode)
	{
		sprintf(sdate_time, DATE_TIME_FORMAT, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);      
		EXCCHECKMSXML( MSXML_IXMLDOMNodeSettext( pnode, NULL, sdate_time));

		EXCCHECKCVI( CA_DiscardObjHandle(pnode));
		pnode = 0;
	}
	/* PalletID */
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode( pxml, NULL, "/testseq/product/PalettID", &pnode));
	if(pnode)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeSettext( pnode, NULL, me->PalletID));

		EXCCHECKCVI( CA_DiscardObjHandle(pnode));
		pnode = 0;
	}

		/* if Mode R&R create head of file CSV from balise config in test_form.xml */
	if (AutomatMode == AUTOMAT_EXPERTISE_SEQUENCE)
	{
	sprintf(csv_dir, "%s\\%02d.%02d.%02d", result_dir, st.wHour, st.wMinute, st.wSecond);
	sprintf(me->pTestParam->csv_dir, "%s", csv_dir);
	EXCCHECK( CheckDirectory(csv_dir)); 
	sprintf(fileName, "%s\\STATION_%02d_PALETT_%d_PIECE_%d.csv", csv_dir, 
															   10*(me->Station_RnR+1), 
															   me->Palett_RnR, 
															   me->Part_RnR);
	
	me->pTestParam->pTicket_CSV = OpenFile (fileName, VAL_READ_WRITE, VAL_OPEN_AS_IS, VAL_ASCII);
	EXCCHECK(Fill_CSV_Steps_Label(pxml, me->pTestParam->pTicket_CSV, me->Station_RnR));
	EXCCHECK(Fill_CSV_Mesures_Label(pxml, me->pTestParam->pTicket_CSV, me->Station_RnR));
	me->pTestParam->Test_RnR = TRUE;
	memset(me->pTestParam->line_CSV, 0X00, MAX_LENGTH_CSV);
	}
	
	/* prepare test parameter data */
	me->pTestParam->pTicket = pxml;
	me->pTestParam->shift_id = pAutomat->shift_id;
	sprintf(me->pTestParam->Time, "%02d.%02d.%02d", st.wHour, st.wMinute, st.wSecond);
	sprintf(me->pTestParam->Date, "%04d.%02d.%02d", st.wYear, st.wMonth, st.wDay);
	sprintf(me->pTestParam->TicketDirPath, result_dir);
	
	EXCCHECK( eleventdata_new(&(me->pData)));

	switch(AutomatMode)
	{
		case AUTOMAT_PRODUCTION:
		case AUTOMAT_QUALITY_MASTER:
		case AUTOMAT_QUALITY_RETEST:
		case AUTOMAT_EXPERTISE_RNR_STUDY:
			
			if (pAutomat->Mode == AUTOMAT_QUALITY_RETEST)
				me->pTestParam->Autorisation_retest = TRUE;

			sprintf(me->pTestParam->Production_Serial, "");
			
			/* copy test options to pTestParam */
			copyTestOption(me);
			
			if (me->pTestParam->Skip_All_Test == FALSE)
			{
			sprintf(me->pTestParam->Board_Ver_pFin, me->Board_Ver_pFin);
			sprintf(me->pTestParam->Serial_Board_Ver_sFin, me->Serial_Board_Ver_sFin);
			sprintf(me->pTestParam->Num_Serie_sFin, me->Num_Serie_sFin);
	        me->pTestParam->id_Board_pFin = me->id_Board_pFin;
	
			sprintf(me->pTestParam->Production_Serial, "%s", me->Serial_Board_Ver_sFin);
			sprintf(me->pTestParam->Date_BSH, "%02d%02d%02d",  st.wDay, st.wMonth, st.wYear - 2000);
			sprintf(me->pTestParam->Time_BSH, "%02d%02d",  st.wHour, st.wMinute);
			}
		
			me->pTestSeq->_GetTestSeqActive(UsedSeq);
			for(station=0; station<NB_OF_STATIONS; station++)
				me->pTestSeq->_TestSeqUsed[station] = UsedSeq[station];
			
			me->pTestSeq->_GetTestSeqStopFail(StopFailSeq);
			for(station=0; station<NB_OF_STATIONS; station++)
				me->pTestSeq->_TestSeqStopFail[station] = StopFailSeq[station];
			
			ptest_exception = me->pTestSeq->RunAll(me->pTestSeq, me->pTestParam, FALSE);
			//ptest_exception = elexception_new( -1, "Error", __FILE__, __LINE__);

			StatusStations[STATION_10] = BUSY; 
			
			if (me->pTestParam->Skip_All_Test == FALSE)
			{
			sprintf(me->barcode, "%s\0", "");
			
			/* Event Label Print OK and save XML report with barode */ 
			if (ptest_exception==NULL )
			{
			sprintf (me->barcode, me->pTestParam->Num_Serie_pFin);
			/* Barcodes save XML */
			EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode( pxml, NULL, "/testseq/product/barcode", &pnode));
			if(pnode)
			{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeSettext( pnode, NULL, (me->barcode)? me->barcode:"no_barcode"));
			EXCCHECKCVI( CA_DiscardObjHandle(pnode));
			pnode = 0;
			}
			SaveXmlResultFile(me->pTestParam, 0);
			Delay(0.7);
			//LogAsteelFile(me->pTestParam, 0); /* fichier type .000*/
			InsertionMesures_DB(me->pTestParam, 0);
			}
		
			/* Get new serial number and save XML report with serial number */ 
			else
			{
			sprintf(me->barcode, me->pTestParam->Num_Serie_pFin);
			/* Barcode save XML */
			EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode( pxml, NULL, "/testseq/product/barcode", &pnode));
			if(pnode)
			{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeSettext( pnode, NULL, (me->barcode)? me->barcode:"no_barcode"));
			EXCCHECKCVI( CA_DiscardObjHandle(pnode));
			pnode = 0;
			}
			SaveXmlResultFile(me->pTestParam, 1);
			Delay(0.7);
			//LogAsteelFile(me->pTestParam, 1); /* fichier type .000*/
			InsertionMesures_DB(me->pTestParam, 1);
			
			}
			
			/* path XML for dtatabase */ 
			sprintf(buffer, "%s;%s;%s", AutomatModeStr[AutomatMode], me->pTestParam->TicketFilePath, (ptest_exception==NULL)? "OK":"FAULT");
			EXCCHECK( me->pData->SetStringPtr(me->pData, buffer));

			/* Save good status in datatbase  |||| this function will be called later for Label Fail */ 
			EXCCHECK( pEventFactory->PulseEvent(pEventFactory, "EVNT_TEST_SEQUENCE_END", me->pData));
			}
			RemovePopup (1);//anwar

		break;
		
		case AUTOMAT_MASTERPART:
			
			me->pTestSeq->LockTest = me->_Lock;
			me->pTestParam->fault_continue = TRUE;
			me->pTestParam->Skip_All_Test = me->Skip;

			sprintf(me->pTestParam->Production_Serial, "%s", me->barcode_master_Part);
			sprintf(me->barcode, "%s\0", me->barcode_master_Part);
	
			if (me->Station_Master == 0 || me->pTestParam->Skip_All_Test == TRUE)
			ptest_exception = me->pTestSeq->RunAll(me->pTestSeq, me->pTestParam, FALSE);
			else 
			ptest_exception = me->pTestSeq->RunStationMaster(me->pTestSeq, me->pTestParam, FALSE, me->Station_Master);

			StatusStations[STATION_10] = BUSY; 
			
			/*compare XML files */
			ptest_exception = CompareXML(me->barcode_master_Part, pxml, me->pTestParam->Name_Fault); 
			
			/* Barcode save XML */
			EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode( pxml, NULL, "/testseq/product/barcode", &pnode));
			if(pnode)
			{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeSettext( pnode, NULL, (me->barcode)? me->barcode:"none"));
			EXCCHECKCVI( CA_DiscardObjHandle(pnode));
			pnode = 0;
			}
	
			if (ptest_exception==NULL)
				SaveXmlResultFile(me->pTestParam, 0);
			else if (ptest_exception)
				SaveXmlResultFile(me->pTestParam, 1);
			
		break;
			
		case AUTOMAT_EXPERTISE_GO_MODE:
		
			me->pTestParam->fault_continue = TRUE;
			
			if (pAutomat->Mode == AUTOMAT_QUALITY_RETEST)
				me->pTestParam->Autorisation_retest = TRUE;

			sprintf(me->pTestParam->Production_Serial, "");
			
			/* copy test options to pTestParam */
			copyTestOption(me);
			
			if (me->pTestParam->Skip_All_Test == FALSE)
			{
			sprintf(me->pTestParam->Board_Ver_pFin, me->Board_Ver_pFin);
			sprintf(me->pTestParam->Serial_Board_Ver_sFin, me->Serial_Board_Ver_sFin);
			sprintf(me->pTestParam->Num_Serie_sFin, me->Num_Serie_sFin);
	        me->pTestParam->id_Board_pFin = me->id_Board_pFin;
	
			sprintf(me->pTestParam->Production_Serial, "%s", me->Serial_Board_Ver_sFin);
			sprintf(me->pTestParam->Date_BSH, "%02d%02d%02d",  st.wDay, st.wMonth, st.wYear - 2000);
			sprintf(me->pTestParam->Time_BSH, "%02d%02d",  st.wHour, st.wMinute);
			}
		
			me->pTestSeq->_GetTestSeqActive(UsedSeq);
			for(station=0; station<NB_OF_STATIONS; station++)
				me->pTestSeq->_TestSeqUsed[station] = UsedSeq[station];
			
			me->pTestSeq->_GetTestSeqStopFail(StopFailSeq);
			for(station=0; station<NB_OF_STATIONS; station++)
				me->pTestSeq->_TestSeqStopFail[station] = StopFailSeq[station];
			
			ptest_exception = me->pTestSeq->RunAll(me->pTestSeq, me->pTestParam, TRUE);
			//ptest_exception = elexception_new( -1, "Error", __FILE__, __LINE__);

			StatusStations[STATION_10] = BUSY; 
			
			if (me->pTestParam->Skip_All_Test == FALSE)
			{
			sprintf(me->barcode, "%s\0", "");
			
			/* Event Label Print OK and save XML report with barode */ 
			if (ptest_exception==NULL )
			{
			sprintf (me->barcode, me->pTestParam->Num_Serie_pFin);
			/* Barcodes save XML */
			EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode( pxml, NULL, "/testseq/product/barcode", &pnode));
			if(pnode)
			{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeSettext( pnode, NULL, (me->barcode)? me->barcode:"no_barcode"));
			EXCCHECKCVI( CA_DiscardObjHandle(pnode));
			pnode = 0;
			}
			SaveXmlResultFile(me->pTestParam, 0);
			Delay(0.7);
			//LogAsteelFile(me->pTestParam, 0); /* fichier type .000*/
			InsertionMesures_DB(me->pTestParam, 0);
			}
		
			/* Get new serial number and save XML report with serial number */ 
			else
			{
			sprintf(me->barcode, me->pTestParam->Num_Serie_pFin);
			/* Barcode save XML */
			EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode( pxml, NULL, "/testseq/product/barcode", &pnode));
			if(pnode)
			{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeSettext( pnode, NULL, (me->barcode)? me->barcode:"no_barcode"));
			EXCCHECKCVI( CA_DiscardObjHandle(pnode));
			pnode = 0;
			}
			SaveXmlResultFile(me->pTestParam, 1);
			Delay(0.7);
			//LogAsteelFile(me->pTestParam, 1); /* fichier type .000*/
			InsertionMesures_DB(me->pTestParam, 1);
			
			}
			
			/* path XML for dtatabase */ 
			sprintf(buffer, "%s;%s;%s", AutomatModeStr[AutomatMode], me->pTestParam->TicketFilePath, (ptest_exception==NULL)? "OK":"FAULT");
			EXCCHECK( me->pData->SetStringPtr(me->pData, buffer));

			/* Save good status in datatbase  |||| this function will be called later for Label Fail */ 
			EXCCHECK( pEventFactory->PulseEvent(pEventFactory, "EVNT_TEST_SEQUENCE_END", me->pData));
			}
			RemovePopup (1);//anwar
			
			break;
			
		case AUTOMAT_EXPERTISE_STEP_MODE:
			ptest_exception = me->pTestSeq->StepStart(me->pTestSeq, me->pTestParam, TRUE);  
		break;
		
			
		case AUTOMAT_EXPERTISE_SEQUENCE:
			
			me->pTestSeq->LockTest = me->_Lock;
			me->pTestParam->fault_continue = FALSE;
			
			ptest_exception = me->pTestSeq->RunStation(me->pTestSeq, me->pTestParam, FALSE, me->Station_RnR, me->Repeat_RnR);
		
			StatusStations[STATION_10] = FALSE;
			sprintf(me->barcode, "RnR%02d_%02d_%02d", st.wHour, st.wMinute, st.wSecond);
			
			/* Event Label Print OK and save XML report with barode */ 
			if (ptest_exception==NULL )
			{
			sprintf(me->pTestParam->Barcode, "%s\0", me->barcode);
			
			/* Barcodes save XML */
			EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode( pxml, NULL, "/testseq/product/barcode", &pnode));
			if(pnode)
			{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeSettext( pnode, NULL, (me->barcode)? me->barcode:"no_barcode"));
			EXCCHECKCVI( CA_DiscardObjHandle(pnode));
			pnode = 0;
			}
			SaveXmlResultFile(me->pTestParam, 0);
			}
		
			/* Get new serial number and save XML report with serial number */ 
			if (ptest_exception)
			{
			sprintf(me->pTestParam->Barcode, "%s\0", me->barcode);

			/* Barcode save XML */
			EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode( pxml, NULL, "/testseq/product/barcode", &pnode));
			if(pnode)
			{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeSettext( pnode, NULL, (me->barcode)? me->barcode:"no_barcode"));
			EXCCHECKCVI( CA_DiscardObjHandle(pnode));
			pnode = 0;
			}
			SaveXmlResultFile(me->pTestParam, 1);
			}
			
			/* path XML for dtatabase */ 
			sprintf(buffer, "%s;%s;%s", AutomatModeStr[AutomatMode], me->pTestParam->TicketFilePath, (ptest_exception==NULL)? "OK":"FAULT");
			EXCCHECK( me->pData->SetStringPtr(me->pData, buffer));

			/* Save good status in datatbase and print Fail Label if Fail */ 
			if (ptest_exception==NULL)
				EXCCHECK( pEventFactory->PulseEvent(pEventFactory, "EVNT_TEST_SEQUENCE_END", me->pData));
			
		break;
	}	

	EXCCHECK(ptest_exception);
	
Error:
	if (pexception) //exception in lasering barcode
	{
	SaveXmlResultFile(me->pTestParam, 1);
	}
	if( is_locked) LeaveCriticalSection( &(me->_Lock));
	CA_VariantClear(&pfile_name_var);
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "{Automat Plugin}::StepTestSequence"
SElExceptionPtr StepTestSequence(
	const char* fileXml, 
	SAutomatPtr pAutomat, 
	SElEventFactoryPtr pEventFactory, 
	int step_mode,
	bool_t* is_active,
	bool_t* last_step
)
{
	SElExceptionPtr		pexception = NULL;
	int32_t				error = 0;
	STestSequencePtr 	pTestSeq = pAutomat->pTestSeq;
	bool_t				b_last_step = FALSE; 
	SElEventDataPtr 	pData = NULL;  
	char				buffer[1024]= ""; 
	
	if(pTestSeq && pTestSeq->SequenceActive(pTestSeq))
	{
		if(is_active) *is_active = TRUE;
		if(last_step) *last_step = FALSE;

		switch(step_mode)
		{
			case STEP_SEQUENCE_REPEAT:
				EXCCHECK( pTestSeq->StepRepeat(pTestSeq));   
				break;
			case STEP_SEQUENCE_NEXT:
				EXCCHECK( pTestSeq->StepNext(pTestSeq, &b_last_step));
				if(last_step) *last_step = b_last_step;
				if(b_last_step)
				{
					if(pEventFactory)
					{
						sprintf(buffer, "EXPERTISE_STEP_MODE;%s;", fileXml); 
						EXCCHECK( eleventdata_new(&pData));
						EXCCHECK( pData->SetStringPtr(pData, buffer));
						EXCCHECK( pEventFactory->PulseEvent(pEventFactory, "EVNT_TEST_SEQUENCE_END", pData));
					}					
				}
				break;		
			case STEP_SEQUENCE_BACK:
				EXCCHECK( pTestSeq->StepBack(pTestSeq));   
				break;
			case STEP_SEQUENCE_CANCEL:
				EXCCHECK( pTestSeq->StepStop(pTestSeq)); 
			
				if(pEventFactory)
				{
					sprintf(buffer, "EXPERTISE_STEP_MODE;%s;", fileXml); 
					EXCCHECK( eleventdata_new(&pData));
					EXCCHECK( pData->SetStringPtr(pData, buffer));
					EXCCHECK( pEventFactory->PulseEvent(pEventFactory, "EVNT_TEST_SEQUENCE_END", pData));
				}
				break;
		}
	}
	else
	{
		if(is_active) *is_active = FALSE;
		if(last_step) *last_step = FALSE;
	}
				
Error:
	eleventdata_delete(&pData);    
	EXCRETHROW( pexception);  
}
