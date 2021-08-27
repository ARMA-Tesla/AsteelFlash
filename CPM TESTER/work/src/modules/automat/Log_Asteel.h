/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

  
/*******************************************************************************
*
*                           Local Prototypes functions
*
*******************************************************************************/
int Fct_Save_Result(char* Reference, char* Serial, char* Result,char* date_time,char* mesures  );
int spi_OuverturePlusEnteteFichier(char tp_FileName[MAX_PATHNAME_LEN],char* Reference, char* Serial,char* date_time, int *pp_FileHandle );
int spi_Ajout_Result(int *pp_FileHandle );                                   
int spi_Fin_Fichier(int vp_PassFail, int *pp_FileHandle);    
int MoveExistingDataFiles (char *vp_char_PathName);                   
int SetPath(char *vp_char_PathName, int vp_int_Mode); 

int ExtractXMLInformations(const char* pfile_xml, char* Reference, char* Serial, char* Result,char* date_time );
int Fill_Log_Asteel_Mesures (const char* pfile_xml, char* mesures);
int Fill_DBO_Asteel_Mesures (const char* pfile_xml,char* nom_mesure, char* mesure_value,char* mesure_min,char* mesure_max,char* mesure_result);


/*******************************************************************************/

#ifdef __cplusplus
    }
#endif
