#define	 _WIN32_WINNT 0x0400     
#include <windows.h> 
#include <cviauto.h>
#include "msxmldom.h"
#include <ansi_c.h>
#include <cvirte.h>		
#include "Log_Asteel.h"
#include <formatio.h>
#include <toolbox.h>
#include "cvi_db.h"
#include <exception/Exception.h>
#include <utility.h>
#include <stdio.h>
#include <formatio.h>
#include "toolbox.h"

//---------------

int Fill_Log_Asteel_Mesures (const char* pfile_xml, char* mesures)
{
	HRESULT         	hr = 0;
	CAObjHandle     	pxml = 0; 
	CAObjHandle     	pnode = 0;
	VARIANT         	pfile_name_var = CA_VariantEmpty();
	VBOOL            	is_xml_success = 1;
	char         		min[32];
	char         		max[32];
	char         		path[256];
	CAObjHandle     	pparameter_process_name = 0;
	CAObjHandle     	pparameter_step_name = 0;
	CAObjHandle     	pparameter_mesure_config = 0;
	CAObjHandle     	pparameter_mesure_value = 0;
	CAObjHandle     	pparameter_mesure_min = 0;
	CAObjHandle     	pparameter_mesure_max = 0;
	CAObjHandle     	pparameter_mesure_result = 0;

	char*           	ptext = NULL;
	char*           	Step_Description = NULL;
	int            		i = 0;
	int            		j = 0;
	int            		k = 0;
	char 				sTrame[256];
	char 				Message[256];
	int                 Status;
	int 				Failed=0;	
	/* Read from XML test Steps */
	MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml);
	CA_VariantSetCString(&pfile_name_var, pfile_xml);
	MSXML_IXMLDOMDocumentload (pxml, NULL, pfile_name_var, &is_xml_success);
	MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//testseq", &pnode);		
		
	do
	{
		sprintf(path, "process[@stage='%d']/name", ++i); 
		MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter_process_name);
		if(pparameter_process_name)//process
		{
			do
			{
				sprintf(path, "process[@stage='%d']/test[@step='%d']/name", i, ++j);
				MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter_step_name);
				if(pparameter_step_name)//step
				{
					do
					{


						sprintf(path, "process[@stage='%d']/test[@step='%d']/measure[@step='%d']/config", i, j, ++k);
						MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter_mesure_config);

						sprintf(path, "process[@stage='%d']/test[@step='%d']/measure[@step='%d']/value", i, j, k);
						MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter_mesure_value);

						sprintf(path, "process[@stage='%d']/test[@step='%d']/measure[@step='%d']/limit/value1", i, j, k);
						MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter_mesure_min);

						sprintf(path, "process[@stage='%d']/test[@step='%d']/measure[@step='%d']/limit/value2", i, j, k);
						MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter_mesure_max);

						sprintf(path, "process[@stage='%d']/test[@step='%d']/measure[@step='%d']/failed", i, j, k);
						MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter_mesure_result);

						if (pparameter_mesure_config)
						{

							MSXML_IXMLDOMNodeGettext (pparameter_step_name, NULL, &ptext);

							if(ptext && *ptext)   //step_name
							{
								strcat(mesures, ptext);
								strcat(mesures, "=");
								CA_FreeMemory(ptext);
								ptext=NULL;
							}


							MSXML_IXMLDOMNodeGettext (pparameter_mesure_value, NULL, &ptext);
							if(ptext && *ptext)
							{
								char tl_Tampon[128]="";

								Fmt (tl_Tampon,"%s[a]<%f[p3];", atof(ptext));
								strcat(mesures, tl_Tampon);
								CA_FreeMemory(ptext);
								ptext=NULL;
							}

							MSXML_IXMLDOMNodeGettext (pparameter_mesure_min, NULL, &ptext);
							if(ptext && *ptext)
							{
								strcat(mesures, ptext);
								strcat(mesures, ";");
								CA_FreeMemory(ptext);
								ptext=NULL;
							}

							MSXML_IXMLDOMNodeGettext (pparameter_mesure_max, NULL, &ptext);
							if(ptext && *ptext)
							{
								strcat(mesures, ptext);
								strcat(mesures, ";GELE;");
								CA_FreeMemory(ptext);
								ptext=NULL;
							}

							MSXML_IXMLDOMNodeGettext (pparameter_mesure_result, NULL, &ptext);
							if(ptext && *ptext)
							{
								if(0 == atoi(ptext))
								{
									strcat(mesures, "P\n");
									Failed = 0;
								}
								else
								{
									strcat(mesures, "F\n");
									Failed = 1;
									break;
								}
								CA_FreeMemory(ptext);
								ptext=NULL;
							}

							CA_DiscardObjHandle(pparameter_mesure_config);
							CA_DiscardObjHandle(pparameter_mesure_value);
							CA_DiscardObjHandle(pparameter_mesure_min);
							CA_DiscardObjHandle(pparameter_mesure_max);
							CA_DiscardObjHandle(pparameter_mesure_result);



							pparameter_mesure_config = 0;
							pparameter_mesure_value=0;
							pparameter_mesure_min = 0;
							pparameter_mesure_max=0;
							pparameter_mesure_result=0;
						}
						else//mesure
						{
							k=0;
							break;
						}

					}
					while(TRUE);

					
					CA_DiscardObjHandle(pparameter_mesure_config);
					CA_DiscardObjHandle(pparameter_mesure_value);
					CA_DiscardObjHandle(pparameter_mesure_min);
					CA_DiscardObjHandle(pparameter_mesure_max);
					CA_DiscardObjHandle(pparameter_mesure_result);
					CA_DiscardObjHandle(pparameter_step_name);
					pparameter_step_name = 0;
					if(Failed)
						break;
				}
				else//step
				{
					j=0;
					break;
				}

			}
			while(TRUE);
			
			CA_DiscardObjHandle(pparameter_process_name);
			pparameter_process_name = 0;
			if(Failed)
				break;
		}
		else
		//	{

				break;
		//	}
	
	}while(TRUE);
	

	if (ptext) CA_FreeMemory(ptext);
	if (pparameter_process_name) CA_DiscardObjHandle(pparameter_process_name);
	if (pparameter_step_name) CA_DiscardObjHandle(pparameter_step_name);
	if (pparameter_mesure_config) CA_DiscardObjHandle(pparameter_mesure_config);
	if (pparameter_mesure_value) CA_DiscardObjHandle(pparameter_mesure_value);
	if (pparameter_mesure_min) CA_DiscardObjHandle(pparameter_mesure_min);
	if (pparameter_mesure_max) CA_DiscardObjHandle(pparameter_mesure_max);
	if (pparameter_mesure_result) CA_DiscardObjHandle(pparameter_mesure_result);


	if (pnode) CA_DiscardObjHandle(pnode);
	if (pxml) CA_DiscardObjHandle(pxml);
	return 0;
}



int Fill_DBO_Asteel_Mesures (const char* pfile_xml,char* nom_mesure, char* mesure_value,char* mesure_min,char* mesure_max,char* mesure_result)
{
	char chaine[512]="";
	char chaine1[512]="";
	HRESULT         	hr = 0;
	CAObjHandle     	pxml = 0; 
	CAObjHandle     	pnode = 0;
	VARIANT         	pfile_name_var = CA_VariantEmpty();
	VBOOL            	is_xml_success = 1;
	char         		path[256];
	CAObjHandle     	pparameter = 0;
	CAObjHandle     	pparameter_process_name = 0;
	CAObjHandle     	pparameter_step_name = 0;
	CAObjHandle     	pparameter_mesure_config = 0;
	CAObjHandle     	pparameter_mesure_value = 0;
	CAObjHandle     	pparameter_mesure_min = 0;
	CAObjHandle     	pparameter_mesure_max = 0;
	CAObjHandle     	pparameter_mesure_result = 0;

	char*           	ptext = NULL;
	char*           	Step_Description = NULL;
	char				Serial[128]="";
	int            		i = 0;
	int            		j = 0;
	int            		k = 0;
	char 				sTrame[256];
	char 				Message[256];
	int                 Status;
	int 				Failed  = 0;
	int 				Id_Test = 0;
	SElExceptionPtr	pexception = NULL;

	int hstmt;
	int  connectionHandle = 0;
	/* Read from XML test Steps */
	
	connectionHandle = DBConnect ("Provider=SQLNCLI11;Password=hp3070;Persist Security Info=True;User ID=testict;Initial Catalog=BSH;Data Source='10.200.0.145'");

	MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml);
	CA_VariantSetCString(&pfile_name_var, pfile_xml);
	MSXML_IXMLDOMDocumentload (pxml, NULL, pfile_name_var, &is_xml_success);
	MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//testseq", &pnode);		
	
	
	/* barcode */
	MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "product//barcode", &pparameter);
	if(pparameter)
		{
			MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext);
			if(ptext && *ptext)
			{
			sprintf(Serial, ptext);
			CA_FreeMemory(ptext);
			ptext=NULL;
			}
						 
		CA_DiscardObjHandle(pparameter);
		pparameter = 0; 
		}
	    /*** Extract IDtest from DBO Test ***/
		 //EXCCHECK( ptraca->Get_Id_Test_Produit(ptraca,Serial,"",&Id_Test));	
		 sprintf(chaine,"SELECT TOP (1) Test.Id FROM Test WHERE (Test.Num_Serie) = '%s' AND typeTest='FCT' AND Id_Machine='TESTER_FCT12' ORDER BY ID DESC ",Serial);
		// printf("Serial = %s \n",Serial);
         hstmt = DBActivateSQL (connectionHandle, chaine);
		 DBGetColInt(hstmt,1,&Id_Test);
		// printf("Id_Test = %d \n",Id_Test);
	
	do
	{
		sprintf(path, "process[@stage='%d']/name", ++i); 
		MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter_process_name);
		if(pparameter_process_name)//process
			{			
				do
				{
					sprintf(path, "process[@stage='%d']/test[@step='%d']/name", i, ++j); 
					MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter_step_name);
					if(pparameter_step_name)//step
					{
						do
						{   


							sprintf(path, "process[@stage='%d']/test[@step='%d']/measure[@step='%d']/config", i, j, ++k);
							MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter_mesure_config);
							
							sprintf(path, "process[@stage='%d']/test[@step='%d']/measure[@step='%d']/value", i, j, k);
							MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter_mesure_value);
							
							sprintf(path, "process[@stage='%d']/test[@step='%d']/measure[@step='%d']/limit/value1", i, j, k);
							MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter_mesure_min);
							
							sprintf(path, "process[@stage='%d']/test[@step='%d']/measure[@step='%d']/limit/value2", i, j, k);
							MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter_mesure_max);
							
							sprintf(path, "process[@stage='%d']/test[@step='%d']/measure[@step='%d']/failed", i, j, k);
							MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, path, &pparameter_mesure_result);
							
							if (pparameter_mesure_config)
							{



								MSXML_IXMLDOMNodeGettext (pparameter_mesure_config, NULL, &ptext);

								if(ptext && *ptext)   //nom mesure
								{
									strcpy(nom_mesure, ptext);
									CA_FreeMemory(ptext);
									ptext=NULL;
								}

								
								MSXML_IXMLDOMNodeGettext (pparameter_mesure_value, NULL, &ptext);
								if(ptext && *ptext)
								{
									
									strcpy(mesure_value, ptext);
									CA_FreeMemory(ptext);
									ptext=NULL;
								}
								
								MSXML_IXMLDOMNodeGettext (pparameter_mesure_min, NULL, &ptext);
								if(ptext && *ptext)
								{
									strcpy(mesure_min, ptext);
									CA_FreeMemory(ptext);
									ptext=NULL;
								}
								
								MSXML_IXMLDOMNodeGettext (pparameter_mesure_max, NULL, &ptext);
								if(ptext && *ptext)
								{
									strcpy(mesure_max, ptext);
									CA_FreeMemory(ptext);
									ptext=NULL;
								}
								
								MSXML_IXMLDOMNodeGettext (pparameter_mesure_result, NULL, &ptext);
								if(ptext && *ptext)
								{
									strcpy(mesure_result, ptext);
									if(0 == atoi(ptext))
									{
										Failed = 0;
									}
									else
									{
										Failed = 1;
										//EXCCHECK( ptraca->Set_Measurement(ptraca,Id_Test,nom_mesure,atof(mesure_min),atof(mesure_max),atof(mesure_value),atoi(mesure_result)));
										sprintf (chaine1, "INSERT INTO MESURES (Id_Test,Nom_Mesure,Low_Limit,High_Limit,Mesure,Result) VALUES ('%d','%s','%f','%f','%f','%d')",Id_Test,nom_mesure ,atof(mesure_min),atof(mesure_max),atof(mesure_value),0);	
										hstmt = DBActivateSQL (connectionHandle, chaine1);
										break;
									}
									CA_FreeMemory(ptext);
									ptext=NULL;
								}
								
								CA_DiscardObjHandle(pparameter_mesure_config);
								CA_DiscardObjHandle(pparameter_mesure_value);
								CA_DiscardObjHandle(pparameter_mesure_min);
								CA_DiscardObjHandle(pparameter_mesure_max);
								CA_DiscardObjHandle(pparameter_mesure_result);

								/***Insert Mesure to DBO MESURE ***/

								sprintf (chaine1, "INSERT INTO MESURES (Id_Test,Nom_Mesure,Low_Limit,High_Limit,Mesure,Result) VALUES ('%d','%s','%f','%f','%f','%d')",Id_Test,nom_mesure ,atof(mesure_min),atof(mesure_max),atof(mesure_value),!Failed);	
								hstmt = DBActivateSQL (connectionHandle, chaine1);
								//-----------------------------------------
								pparameter_mesure_config = 0;
								pparameter_mesure_value=0;
								pparameter_mesure_min = 0;
								pparameter_mesure_max=0;
								pparameter_mesure_result=0;
							}
							else//mesure
							{
								k=0;
								break;
							}

						}
						while(TRUE);

						CA_DiscardObjHandle(pparameter_step_name);
						CA_DiscardObjHandle(pparameter_mesure_config);
						CA_DiscardObjHandle(pparameter_mesure_value);
						CA_DiscardObjHandle(pparameter_mesure_min);
						CA_DiscardObjHandle(pparameter_mesure_max);
						CA_DiscardObjHandle(pparameter_mesure_result);
						pparameter_step_name = 0;
						if(Failed)
							break;
					}
					else//step
					{
						j=0;
						break;
					}
		
				}while(TRUE);
				
				CA_DiscardObjHandle(pparameter_process_name);
				pparameter_process_name = 0;
				if(Failed)
						break;
			}
		else
		//	{

				break;
		//	}
	
	}while(TRUE);
	

	if (ptext) CA_FreeMemory(ptext);
	if (pparameter_process_name) CA_DiscardObjHandle(pparameter_process_name);
	if (pparameter_step_name) CA_DiscardObjHandle(pparameter_step_name);
	if (pparameter_mesure_config) CA_DiscardObjHandle(pparameter_mesure_config);
	if (pparameter_mesure_value) CA_DiscardObjHandle(pparameter_mesure_value);
	if (pparameter_mesure_min) CA_DiscardObjHandle(pparameter_mesure_min);
	if (pparameter_mesure_max) CA_DiscardObjHandle(pparameter_mesure_max);
	if (pparameter_mesure_result) CA_DiscardObjHandle(pparameter_mesure_result);


	if (pnode) CA_DiscardObjHandle(pnode);
	if (pxml) CA_DiscardObjHandle(pxml);
	
	Error:
	DBDisconnect(connectionHandle);
	//EXCRETHROW( pexception); 
	return 0;
}





int ExtractXMLInformations(const char* pfile_xml, char* Reference, char* Serial, char* Result,char* date_time )
{
	HRESULT         	hr = 0;
	CAObjHandle     	pxml = 0; 
	CAObjHandle     	pnode = 0;
	VARIANT         	pfile_name_var = CA_VariantEmpty();
	VBOOL            	is_xml_success = 1;
	char         		path[256];
	CAObjHandle     	pparameter = 0;
	char*           	ptext = NULL;

	
	/* configure test sequence from XML */
	MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml);
	CA_VariantSetCString(&pfile_name_var, pfile_xml);
	MSXML_IXMLDOMDocumentload (pxml, NULL, pfile_name_var, &is_xml_success);
	MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//testseq", &pnode);
	
	/* référence */
	MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "product//name", &pparameter);
	if(pparameter)
		{
			MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext);
			if(ptext && *ptext)
			{
			sprintf(Reference, ptext);
			CA_FreeMemory(ptext);
			ptext=NULL;
			}
						 
		CA_DiscardObjHandle(pparameter);
		pparameter = 0; 
		}
	
	/* barcode */
	MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "product//barcode", &pparameter);
	if(pparameter)
		{
			MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext);
			if(ptext && *ptext)
			{
			sprintf(Serial, ptext);
			CA_FreeMemory(ptext);
			ptext=NULL;
			}
						 
		CA_DiscardObjHandle(pparameter);
		pparameter = 0; 
		}
		
	/* result */
	MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "product//failed", &pparameter);
	if(pparameter)
		{
			MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext);
			if(ptext && *ptext)
			{
			sprintf(Result, ptext);
			CA_FreeMemory(ptext);
			ptext=NULL;
			}
						 
		CA_DiscardObjHandle(pparameter);
		pparameter = 0; 
		}
		
		/* date time */
	MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "product//date_time", &pparameter);
	if(pparameter)
		{
			MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext);
			if(ptext && *ptext)
			{
			sprintf(date_time, ptext);
			CA_FreeMemory(ptext);
			ptext=NULL;
			}
						 
		CA_DiscardObjHandle(pparameter);
		pparameter = 0; 
		}
		
		
		
	CA_VariantClear(&pfile_name_var);
	if (ptext) CA_FreeMemory(ptext);
	if (pparameter) CA_DiscardObjHandle(pparameter);
	if (pnode) CA_DiscardObjHandle(pnode);
	if (pxml) CA_DiscardObjHandle(pxml);
	return 0;	
}

//------------------------------------------------------------------------------------------------------------------
int SetPath (char *vp_char_PathName,int vp_int_Mode) /* 1 : création - 0 : vérification */
{
  char directory [100], repertoire [50], chemin_repertoire [150];
  int status = 0;

  DisableBreakOnLibraryErrors ();
  (vp_char_PathName[0]>90) ? (status = SetDrive (vp_char_PathName[0] - 'a')) :(status = SetDrive (vp_char_PathName[0] - 'A'));
  if (status != 0) 
  	{	

    vp_char_PathName[0] = 'C';		   // si réseau non accessible sauvegarde sous c:\
	
    status = SetDrive (vp_char_PathName[0] - 'A');
    if (status != 0) 
    	  return -100;
    }
  
  status = SetDir ("\\");

  Fmt (chemin_repertoire, "");
  CopyString (directory, 0, vp_char_PathName, 2, -1);

  do 
  {

    Fmt (repertoire, "%s<\\%s[i1t92t0]", directory);
    Fmt (chemin_repertoire, "%s[a]<%s", repertoire);
    CopyString (directory, 0, directory, StringLength (repertoire), -1);

    if (SetDir (chemin_repertoire) != 0)
    {
      if (vp_int_Mode == 1)
      {
        status = MakeDir (chemin_repertoire);

        if (status != 0) return -101;
      }
      else return -102;
    } 
    else status = 0;
  } while ((status == 0) && (StringLength (directory) > 1));


  return 0;
}
/*****************************************************************************/
/*  Nom de fonction : MoveExistingDataFiles                             	*/
/*                                                                           */
/*  Description :                                                            */
/*                                                                           */
/*  Remarques :                                                              */
/*                                                                           */
/*****************************************************************************/
int MoveExistingDataFiles (char *vp_char_PathName)
{
 
	int vl_int_TransfertStatus = 0;
	char tl_char_SearchCriteria [MAX_PATHNAME_LEN]= "";
	char tl_char_LocalFileName  [MAX_PATHNAME_LEN]= "";
	char tl_char_RemoteFileName [MAX_PATHNAME_LEN]= "";
	char tl_char_SourceDirectory[MAX_PATHNAME_LEN]= "";
	char tl_char_TargetDirectory[MAX_PATHNAME_LEN]= "";

	char tl_char_FileExtension[8];
	int vl_int_FileExtension = 0;
	int vl_Result;  
	int error = 0;

vl_Result = FindPattern (vp_char_PathName, 0, StringLength (vp_char_PathName), "Pass", 0, 0);
if (vl_Result > 0)
	CopyString(tl_char_TargetDirectory, 0 ,vp_char_PathName, 0, vl_Result);  
else
	{
	vl_Result = FindPattern (vp_char_PathName, 0, StringLength (vp_char_PathName), "Fail", 0, 0);
	if (vl_Result > 0)
		CopyString(tl_char_TargetDirectory, 0 ,vp_char_PathName, 0, vl_Result);  
	else
		{
		error = -1000;
		goto Error;
		}
	}

CopyString(tl_char_SourceDirectory, 0 ,tl_char_TargetDirectory, 0, StringLength (tl_char_TargetDirectory));                 
Fmt(tl_char_SourceDirectory, "%s[a]<Pass\\");
tl_char_SourceDirectory[0] = 'C';
Fmt(tl_char_TargetDirectory, "%s[a]<Pass\\");  

/* transert des fichiers bon */
/* creation de l'arborescence */                       
if( SetPath (tl_char_TargetDirectory, 0) != 0)
	SetPath (tl_char_TargetDirectory, 1);
/* */
DisableBreakOnLibraryErrors ();
if (SetDir(tl_char_SourceDirectory) != 0);
	{
	error = -1;
	goto Error;
	}
EnableBreakOnLibraryErrors();      
/* */
sprintf (tl_char_SearchCriteria, "*.*");
      
/* retourne '0' si un fichier a été localisé */
/* tl_char_LocalFileName : nom de base + extension du fichier localisé */
vl_int_TransfertStatus = GetFirstFile (tl_char_SearchCriteria, 1, 0, 0, 0, 0, 0, tl_char_LocalFileName);

while (vl_int_TransfertStatus == 0) /* fichier localisé ? */
	{
    /* formattage du chemin complet pour la copie vers le réseau */
     Fmt (tl_char_RemoteFileName, "%s<%s\\%s", tl_char_TargetDirectory, tl_char_LocalFileName);
    
    /* validation du déplacement d'un fichier */
    /* retourne -9 si le fichier distant existe déja */
    /* fichiers bon => écraser le fichier */
    vl_int_TransfertStatus = RenameFile (tl_char_LocalFileName, tl_char_RemoteFileName);
    if (vl_int_TransfertStatus == -9)
        {
          DeleteFile (tl_char_RemoteFileName);
          RenameFile (tl_char_LocalFileName, tl_char_RemoteFileName);
        }
        
    // rechercher le fichier suivant
    vl_int_TransfertStatus = GetNextFile (tl_char_LocalFileName);
    }

    /* transert des fichiers mauvais */
    /* */
vl_Result = FindPattern (tl_char_TargetDirectory, 0, StringLength (tl_char_TargetDirectory), "Pass", 0, 0);
if (vl_Result > 0)
	CopyString(tl_char_TargetDirectory, 0 ,tl_char_TargetDirectory, 0, vl_Result);   
Fmt(tl_char_TargetDirectory, "%s[a]<Fail\\"); 

vl_Result = FindPattern (tl_char_SourceDirectory, 0, StringLength (tl_char_SourceDirectory), "Pass", 0, 0);
if (vl_Result > 0)
	CopyString(tl_char_SourceDirectory, 0 ,tl_char_SourceDirectory, 0, vl_Result);   
Fmt(tl_char_SourceDirectory, "%s[a]<Fail\\");     

/* creation de l'arborescence */                       
if( SetPath (tl_char_TargetDirectory, 0) != 0)
	SetPath (tl_char_TargetDirectory, 1);
/* */
SetDir(tl_char_SourceDirectory);
      
/* */
sprintf (tl_char_SearchCriteria, "*.*");
      
/* retourne '0' si un fichier a été localisé */
/* tl_char_LocalFileName : nom de base + extension du fichier localisé */
vl_int_TransfertStatus = GetFirstFile (tl_char_SearchCriteria, 
                                             1, 0, 0, 0, 0, 0, 
                                             tl_char_LocalFileName);

while (vl_int_TransfertStatus == 0)
	{
    /* formattage du chemin complet pour la copie vers le réseau */
    Fmt (tl_char_RemoteFileName, "%s<%s\\%s", tl_char_TargetDirectory, tl_char_LocalFileName);
    
    /* validation du déplacement d'un fichier */
    vl_int_TransfertStatus = RenameFile (tl_char_LocalFileName, tl_char_RemoteFileName);
    DisableBreakOnLibraryErrors ();       
    while (vl_int_TransfertStatus == -9)
        {
        /* lecture de l'extension du fichier distant */
        /* conversion en entier */
        Scan (tl_char_RemoteFileName, "%s>%s[dt46].%i", &vl_int_FileExtension);
        Fmt (tl_char_FileExtension, "%s<%i[w3p0]", ++vl_int_FileExtension);         
          
        /* mise à jour du nom distant */
        CopyBytes (tl_char_RemoteFileName, 
                     strlen(tl_char_RemoteFileName)-strlen(tl_char_FileExtension), tl_char_FileExtension, 0, strlen(tl_char_FileExtension));
          
        /* nouvelle tentative de copie */
        vl_int_TransfertStatus = RenameFile (tl_char_LocalFileName, tl_char_RemoteFileName);
        }
        EnableBreakOnLibraryErrors ();  
        // rechercher le fichier suivant
        vl_int_TransfertStatus = GetNextFile (tl_char_LocalFileName);
     } 
   
 
  
Error:
return error;

}

 int spi_OuverturePlusEnteteFichier(char tp_FileName[MAX_PATHNAME_LEN],char* Reference, char* Serial,char* date_time, int *pp_FileHandle )
{
#if 0    /* formerly excluded lines */

#endif   /* formerly excluded lines */
int     vl_Mois;
int     vl_Jour;
int     vl_Annee;



int     vl_Status=0;
char    tl_Tampon [4100]= "";
char    tl_Chaine [128]= "";
char    tl_FileName [128]= "";  
int vl_Result;

int error=0;

vl_Result = FindPattern (tp_FileName, 0, strlen(tp_FileName), "\\", 0, 1);
if (vl_Result > 0)
	{
	CopyString (tl_Chaine, 0, tp_FileName, 0, vl_Result);
#if 0    /* formerly excluded lines */
	if( SetPath (tl_Chaine, 0) != 0)
#endif   /* formerly excluded lines */
    SetPath (tl_Chaine, 1);
    CopyString (tl_FileName, 0, tl_Chaine, 0, vl_Result); 
    CopyString (tl_FileName, vl_Result, tp_FileName, vl_Result, strlen(tp_FileName));       
	}

// Open file        				  
*pp_FileHandle = OpenFile (tl_FileName, VAL_WRITE_ONLY, 2, VAL_ASCII);

if(*pp_FileHandle <0)
  { // File error
    return (*pp_FileHandle);
  }

// Makes header of file to save 
Fmt (tl_Tampon,"%s<[PARAMETTERS];;;;\n");


CopyString (tl_Chaine, 0, Reference, 0, strlen (Reference));  
Fmt (tl_Tampon,"%s[a]<Device_Type=%s;;;;\n", tl_Chaine);


CopyString (tl_Chaine, 0, Serial, 15, 5);
Fmt (tl_Tampon,"%s[a]<Serial_Number=%s;;;;\n",tl_Chaine); 
Fmt (tl_Tampon,"%s[a]<LOT=%s;;;;\n", "");
CopyString (tl_Chaine, 0, Serial, 0, strlen (Serial)); 
Fmt (tl_Tampon,"%s[a]<BARCODE=%s;;;;\n",tl_Chaine); 
Fmt (tl_Tampon,"%s[a]<Language=10;;;;\n");  
Fmt (tl_Tampon,"%s[a]<Meas_Type=STD;;;;\n");
Fmt (tl_Tampon,"%s[a]<Meas_Date=%s;;;;\n", DateStr ()); 

CopyString (tl_Chaine, 0, "FCT", 0, strlen ("FCT"));        
Fmt (tl_Tampon,"%s[a]<TEST=%s;;;;\n", tl_Chaine); 

CopyString (tl_Chaine, 0, "2.1.1", 0, strlen ("2.1.1"));   
Fmt (tl_Tampon,"%s[a]<PROGRAM=%s;;;;\n", tl_Chaine); 

Fmt (tl_Tampon,"%s[a]<PANEL=1/1;;;;\n"); 

CopyString (tl_Chaine, 0, "", 0, strlen ("")); 
Fmt (tl_Tampon,"%s[a]<OPERATOR=%s;;;;\n", tl_Chaine); 

CopyString (tl_Chaine, 0, "FCT12", 0, strlen ("FCT12")); 
Fmt (tl_Tampon,"%s[a]<WORKSTATION=%s;;;;\n", tl_Chaine); 

CopyString (tl_Chaine, 0, date_time, 0, strlen (date_time));  
//GetSystemDate (&vl_Mois, &vl_Jour, &vl_Annee);
//Fmt (tl_Tampon,"%s[a]<STARTTIME=%i[p0w2] %i[p0w2] %i %s\n",vl_Jour, vl_Mois, vl_Annee, tl_Chaine);
Fmt (tl_Tampon,"%s[a]<ENDTEST;;;;\n");          

Fmt (tl_Tampon,"%s[a]<STARTTIME=%s;;;;\n", tl_Chaine); 

Fmt (tl_Tampon,"%s[a]<USERDATA=NONE;;;;\n;;;;\n"); 

Fmt (tl_Tampon,"%s[a]<[SYSTEM TEST];;;;\n;;;;\n"); 
Fmt (tl_Tampon,"%s[a]<[EXTRA DATA];;;;\n;;;;\n");
Fmt (tl_Tampon,"%s[a]<[RESULTS];;;;\n");   


// Save header file 
vl_Status = WriteFile (*pp_FileHandle, tl_Tampon, strlen(tl_Tampon));

return vl_Status;


Error:

return error;


}


int spi_Fin_Fichier(int vp_PassFail, int *pp_FileHandle )
{

int		vl_Status;
int     vl_Mois;
int     vl_Jour;
int     vl_Annee;
int     vl_Secondes;
int		vl_Minutes;
int 	vl_Heures;
char    tl_Tampon [9999]="";  


// Makes End of file to save 
Fmt (tl_Tampon,"%s<;;;;\n");

Fmt (tl_Tampon,"%s<\n[EOF];;;;\n");
if (vp_PassFail == 0)
	Fmt (tl_Tampon,"%s[a]<STATUS=PASS;;;;\n");    
else
	Fmt (tl_Tampon,"%s[a]<STATUS=FAIL;;;;\n");    
GetSystemDate (&vl_Mois, &vl_Jour, &vl_Annee);
GetSystemTime (&vl_Heures, &vl_Minutes, &vl_Secondes);
Fmt (tl_Tampon,"%s[a]<ENDTIME=%i[p0w2] %i[p0w2] %i %i[p0w2]:%i[p0w2]:%i[p0w2];;;;\n",vl_Jour, vl_Mois, vl_Annee, vl_Heures, vl_Minutes, vl_Secondes);
Fmt (tl_Tampon,"%s[a]<ENDTEST;;;;\n");          

// Save Footer file 
vl_Status = WriteFile (*pp_FileHandle, tl_Tampon, strlen(tl_Tampon));

return vl_Status;
}


 int Fct_Save_Result(char* Reference, char* Serial, char* Result,char* date_time,char* mesures )

{
char tg_Disk[512]="c:\\";

int vl_Status;
int error;

//ERRORINFO errorInfo;                        // Info of error with API
//CAObjHandle HandleUserName;                 // User nane object for API
//CAObjHandle HandleEngine;                   // Engine object for API
//CAObjHandle HandleStep;                     // Step object for API
                // Interactive Mode Flag
char tl_Part_Num[40];  						// Numéro de produit 
char tl_Serial_Num[40];    					// Numero de série

    					// For manipulation                 

char tl_Path_File[512] = "";
char tl_Type_fichier[512] = "";
char tl_Nom_fichier[512] = "";
int vl_Index;
int vl_Nbre_fichiers = 0;
char tl_FinalFile [512];
char tl_Extension [4];
int vl_File_Handle = 0;
int vl_Result;  


SetBreakOnLibraryErrors (0);
SetBreakOnProtectionErrors(0);


// Création du chemin de sauvegarde + nom de fichier
CopyString (tl_Path_File, 0, tg_Disk, 0, strlen (tg_Disk));   
CopyString (tl_Part_Num, 0,Reference, 0, strlen (Reference)); 
CopyString (tl_Serial_Num, 0, Serial, 0, strlen (Serial)); 

if  (atoi(Result) == 0)
{
	Fmt (tl_Path_File,"%s<%sResults\\%s\\Pass\\P1%s", tl_Path_File, tl_Part_Num, tl_Serial_Num);
	Fmt (tl_Path_File,"%s<%s.000",tl_Path_File);
}
else
{
	Fmt (tl_Path_File,"%s<%sResults\\%s\\Fail\\P1%s", tl_Path_File, tl_Part_Num, tl_Serial_Num);
	// Verify non-existant file ?
	if(!strlen(tl_Path_File)) goto Error;
	Fmt (tl_Type_fichier, "%s<%s.*", tl_Path_File);
	vl_Status = GetFirstFile (tl_Type_fichier, 1, 0, 0, 0, 0, 0, tl_Nom_fichier);
	// First file ?
	if(!vl_Status)
	{
		// File is already exist
		do
		{
			vl_Nbre_fichiers++;
			vl_Result = GetNextFile (tl_Nom_fichier);
		}
		while (!vl_Result);

		Fmt (tl_Extension, "%s[w3]<%i", vl_Nbre_fichiers);
		while (tl_Extension [2] == ' ')
		{
			for (vl_Index=2; vl_Index>0; vl_Index--)
				tl_Extension [vl_Index] = tl_Extension [vl_Index-1];
			tl_Extension [0] = '0';
		}
		Fmt (tl_FinalFile, "%s<%s.%s", tl_Path_File, tl_Extension);
		strcpy(tl_Path_File, tl_FinalFile);
	}
	else
	{
		// First file
		Fmt (tl_Path_File,"%s<%s.000",tl_Path_File);
	}
}


//// Création du fichier avec entête
spi_OuverturePlusEnteteFichier( tl_Path_File,Reference,Serial,date_time, &vl_File_Handle);

//// Récupération des résultas
Fmt (mesures, "%s[a]<%s",";;;;");

WriteFile (vl_File_Handle, mesures, strlen(mesures)); 

//// Fin de fichier
spi_Fin_Fichier(atoi(Result), &vl_File_Handle);   

CloseFile(vl_File_Handle);

//if (tl_Path_File[0] != 'C' && tl_Path_File[0] != 'c') 
//	MoveExistingDataFiles(tl_Path_File);				 // Transfert sur le réseau des fichiers sous C     

Error:
#if 0    /* formerly excluded lines */

#endif   /* formerly excluded lines */

return 0;

}

