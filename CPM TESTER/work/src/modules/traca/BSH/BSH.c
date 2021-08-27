#include "cvi_db.h"
#include <cvintwrk.h>
#include <userint.h>
#include <tcpsupp.h>
#include "BSH.h"
#include <stdio.h>
#include <formatio.h>
#include <utility.h>

#pragma warning( push)
#pragma warning( disable: 4100 4127)

#define TRACA_TIMEOUT 	5000

#undef __FUNC__
#define __FUNC__	"{BSH}::Connect"
SElExceptionPtr Connect(STracaPtr me)
{

SElExceptionPtr	pexception = NULL;

		me->Handle_Traca = DBConnect (me->ConnetionString);
		if (me->Handle_Traca <= 0)
			EXCTHROW(-1, "Erreur Connection Base des données Asteel Flash");

Error:
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__	"{BSH}::Disconnect"
SElExceptionPtr Disconnect(STracaPtr me)
{

SElExceptionPtr	pexception = NULL;

	DBDisconnect (me->Handle_Traca);


Error:
	EXCRETHROW( pexception);
}
#undef __FUNC__
#define __FUNC__	"{BSH}::Get_BoardVer_sFin_Valide"
SElExceptionPtr Get_BoardVer_sFin_Valide(STracaPtr me, char *board_Ver_sFin, int* id_board_sFin, bool_t *valide)
{
	SElExceptionPtr	pexception = NULL;
    unsigned short valide_local = 0;
	int id_board_sfin_local;
	unsigned short bit_valide = 0;
	char chaine[512];
	int hstmt;
	
	strcat(board_Ver_sFin,"%");
	sprintf(chaine,"SELECT [Valide] FROM [Board] WHERE [Board_Ver] like '%s'", board_Ver_sFin);
	hstmt = DBActivateSQL(me->Handle_Traca,chaine);
	DBGetColShort(hstmt,1, &bit_valide);
	valide_local = bit_valide >> 15;
    
	sprintf(chaine,"SELECT [Id] FROM [Board] WHERE [Board_Ver] like '%s'", board_Ver_sFin);
	hstmt = DBActivateSQL(me->Handle_Traca,chaine);
	DBGetColInt(hstmt,1,&id_board_sfin_local);
	
	*id_board_sFin = id_board_sfin_local ;
	*valide = valide_local ;

Error:
	EXCRETHROW( pexception);
}
#undef __FUNC__
#define __FUNC__	"{BSH}::Get_BoardVer_sFin_Active"
SElExceptionPtr Get_BoardVer_sFin_Active(STracaPtr me, int id_board_sFin, bool_t *valide, bool_t *active)
{
	SElExceptionPtr	pexception = NULL;
	int valide_local = 0;
	int active_local = 0;
	unsigned short bit_valide;
	unsigned short bit_active;
	int hstmt;
	char chaine[512];
	int ret;
	int i;
	
	sprintf(chaine,"SELECT [Valide],[Active] FROM [Config_Ligne] WHERE [Id_RefPCB_Assembled] = '%d'AND [Valide]= 1 AND [Active]=1 ", id_board_sFin);
	hstmt = DBActivateSQL(me->Handle_Traca,chaine);
	ret=DBNumberOfRecords (hstmt);
	for (i=1; i<(ret+1);i++)
	{
		DBGetColShort(hstmt,1,&bit_valide);
		valide_local = bit_valide >> 15;
		DBGetColShort(hstmt,2,&bit_active);
		active_local = bit_active >> 15;
		
		if (valide_local && active_local)
			break;
		else 
			DBFetchNext (hstmt);
	}

	*valide = valide_local;
	*active = active_local;

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{BSH}::Get_Reference_Product"
SElExceptionPtr Get_Reference_Product(STracaPtr me,  int id_board_sFin, int *id_board_pFin, char *ProductRefernce)
{
	SElExceptionPtr	pexception = NULL;
	int id_board_pfin_local;
	char product_reference_local[20+1];
	int hstmt;
	char chaine[512];
	
	sprintf(chaine,"SELECT [Id_RefProrduitFini]   FROM [Config_Ligne] WHERE [Id_RefPCB_Assembled]= '%d' AND [Valide] = 'true' AND [Active] = 'true'", id_board_sFin);
	hstmt = DBActivateSQL(me->Handle_Traca,chaine);
	DBGetColInt(hstmt,1,&id_board_pfin_local);
	*id_board_pFin = id_board_pfin_local;
	sprintf(chaine,"SELECT [Code_AsteelFlash]   FROM [Board] WHERE [Id] = '%d' ", id_board_pfin_local);
	hstmt = DBActivateSQL(me->Handle_Traca,chaine);
	DBGetColCharBuffer (hstmt, 1, product_reference_local, 40, "");
	sprintf(ProductRefernce, "%s", product_reference_local);

Error:
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__	"{BSH}::Get_BoardVer_pFin"
SElExceptionPtr Get_BoardVer_pFin(STracaPtr me, int id_board_pFin,  char *Board_Ver_pFin)
{
	SElExceptionPtr	pexception = NULL;
	char board_pfin_local[512];
	char chaine[512];
	int  hstmt;
	
	
	sprintf(chaine,"SELECT [Board_Ver]   FROM [Board] WHERE [Id]= '%d'   ",id_board_pFin);
    hstmt = DBActivateSQL (me->Handle_Traca, chaine);
	DBGetColCharBuffer (hstmt, 1, board_pfin_local, 40, "");

	sprintf(Board_Ver_pFin, "%s", board_pfin_local);
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{BSH}::Get_FieldLabel"
SElExceptionPtr Get_FieldLabel(STracaPtr me,  int id_board_pFin,  int *count ,char FieldName[MAX_FIELDS][MAX_LENGTH] ,char FieldData[MAX_FIELDS][MAX_LENGTH])
{
	SElExceptionPtr	pexception = NULL;
	int count_local = 0;
	char board_pfin_local;
	char fieldname_local[128];
	char fielddata_local[512];
	char chaine[512];
	int hstmt;
	int idlabel;
	int ret;
	int i;
	
	sprintf(chaine,"SELECT [Id_Label]   FROM [Board] WHERE [Id]= '%d'   ",id_board_pFin);
    hstmt = DBActivateSQL (me->Handle_Traca, chaine);
	DBGetColInt (hstmt,1, &idlabel);
	
	sprintf(chaine,"SELECT *   FROM [FieldLabel] WHERE [Id_Label]= '%d'   ",idlabel);
	hstmt = DBActivateSQL (me->Handle_Traca, chaine);
	ret=DBNumberOfRecords (hstmt);

	count_local = 0;
	for (i=1; i<(ret+1);i++)
	   {
		   sprintf(chaine,"SELECT [FieldName],[FieldData]   FROM [FieldLabel] WHERE [Id_Label]= '%d' AND [OrderLabel]='%d'   ",idlabel , i);
		   hstmt = DBActivateSQL (me->Handle_Traca, chaine);
		   DBGetColCharBuffer (hstmt, 1, fieldname_local, 46, "");
		   DBGetColCharBuffer (hstmt, 2, fielddata_local, 46, "");
		   sprintf(FieldName[i-1], fieldname_local); 
	       sprintf(FieldData[i-1], fielddata_local); 
		   count_local ++;
	   }	
	*count = count_local;


Error:
	EXCRETHROW( pexception);
}



#undef __FUNC__
#define __FUNC__	"{BSH}::Get_Last_Mouvement"
SElExceptionPtr Get_Last_Mouvement(STracaPtr me, char *Num_Serie, char *TypeTest , bool_t *result)
{
	SElExceptionPtr	pexception = NULL;
	unsigned short bit;
	int result_local;
	char chaine[512];
	int hstmt;
	char msg[1024];
		
	strcat(Num_Serie,"%");
	strcat(TypeTest,"%");
	sprintf(chaine, "SELECT top(1)[Result] FROM [Test] WHERE [Num_Serie] like '%s' AND [TypeTest] like '%s'  ORDER BY Id DESC",Num_Serie, TypeTest);
	hstmt = DBActivateSQL (me->Handle_Traca, chaine);
	if (hstmt <= 0)
	{
		sprintf (msg, "Get_Last_Mouvement Erreur requete %s", chaine);
		EXCTHROW(-1, msg);
	}
	
	DBGetColShort (hstmt, 1, &bit);
    result_local = bit >> 15; 
	
	*result = result_local;
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{BSH}::Set_Last_Mouvement"
SElExceptionPtr Set_Last_Mouvement(STracaPtr me, char *Num_Serie, int id_board_pfin ,char *DateDebut, char *DateFin, char *Id_Machine, char *TypeTest, bool_t result)
{
	SElExceptionPtr	pexception = NULL;
	char chaine[512];
	int hstmt;
	char msg[1024];
		
	sprintf (chaine, "INSERT INTO TEST (Id_Board,Num_Serie,Id_Machine,DateDebut,DateFin,Result,TypeTest) VALUES ('%d','%s','%s','%s', NULL,'%d','%s')",id_board_pfin,Num_Serie,Id_Machine,DateDebut,result,TypeTest);
	//printf ("Set_Last_Mouvement : \r\n %s", chaine);
	hstmt = DBActivateSQL (me->Handle_Traca,chaine);
	//hstmt = DBActivateSQL (me->Handle_Traca, chaine);
	if (hstmt <= 0)
	{
		sprintf (msg, "Set_Last_Mouvement Erreur requete %s", chaine);
		EXCTHROW(-1, msg);
	}
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{BSH}::Set_Association"
SElExceptionPtr Set_Association(STracaPtr me,  char *Num_Serie_sFin, char *Num_Serie_pFin, char *Num_Serie_Client, char *date_association)
{
	SElExceptionPtr	pexception = NULL;
	char chaine[512];
	int hstmt;
	char msg[1024];
		
	sprintf(chaine, "INSERT INTO Association (SN_PCBA,SN_PFIN,SN_Client,Date_Association) VALUES ('%s','%s','%s','%s')",Num_Serie_sFin , Num_Serie_pFin , Num_Serie_Client ,date_association);
	//printf ("Set_Association : \r\n %s", chaine);
	hstmt = DBActivateSQL (me->Handle_Traca, chaine);
	if (hstmt <= 0)
	{
		sprintf (msg, "Set_Association Erreur requete %s", chaine);
		EXCTHROW(-1, msg);
	}

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{BSH}::Test_SMM"
SElExceptionPtr Test_SMM(STracaPtr me,  char* Board_Ver_pFin ,bool_t *Subassembly)
{
	SElExceptionPtr	pexception = NULL;
	unsigned short bit;
	int result_local;
	char chaine[512];
	int hstmt;
	char msg[1024];
	int  status;
		
	strcat(Board_Ver_pFin,"%");
	
	//Under this Board Ver pFin there is many variants : When the variant with SMM ( [SubAssembly] = 1) || When the variant without SMM ( [SubAssembly] = 0 )
	//sprintf(chaine, "SELECT [SubAssembly] FROM [Board] WHERE [Board_Ver] like '%s' AND [SubAssembly] = 1 ",Board_Ver_pFin); 
	
	
	//Under this Board Ver pFin there is just one variant
	sprintf(chaine, "SELECT [SubAssembly] FROM [Board] WHERE [Board_Ver] like '%s'",Board_Ver_pFin);
	
	
	hstmt = DBActivateSQL (me->Handle_Traca, chaine);
	if (hstmt <= 0)
	{
		sprintf (msg, "Test_SMM Erreur requete %s", chaine);
		EXCTHROW(-1, msg);
	}
	status = DBGetColShort (hstmt, 1, &bit);
	if (status != DB_SUCCESS)
	{
		sprintf (msg, "Get_SMM_SN Erreur requete %s", chaine);
		EXCTHROW(-1, msg);
	}
    result_local = bit >> 15; 
	
	*Subassembly = result_local;
	
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{BSH}::Get_SMM_SN"
SElExceptionPtr Get_SMM_SN(STracaPtr me,  char* Num_Serie_sFin ,char* Num_Serie_SMM)
{
	SElExceptionPtr	pexception = NULL;
	
	int id;
	char chaine[512];
	int hstmt;
	char msg[1024];
	int  status;
	int  ret;
	
	//strcat(Num_Serie_sFin, "%");
	
	sprintf(chaine, "SELECT TOP (1) Association.SN_PFIN, Details_Associations.Num_Serie "
"FROM     Association INNER JOIN "
                 "Associations ON Association.SN_PCBA = Associations.Num_Serie INNER JOIN "
                  "Details_Associations ON Associations.Id = Details_Associations.Id_Associations "
"WHERE  (Association.SN_PFIN = N'%s') "
"ORDER BY Association.Id DESC",Num_Serie_sFin);

	hstmt = DBActivateSQL (me->Handle_Traca, chaine);
	if (hstmt <= 0)
	{
		sprintf (msg, "Get_SMM_SN Erreur requete %s", chaine);
		EXCTHROW(-1, msg);
	}
	ret=DBNumberOfRecords (hstmt);
    DBGetColCharBuffer (hstmt, 2, Num_Serie_SMM, 40, "");
	
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{BSH}::Get_Id_Test_Produit"
SElExceptionPtr Get_Id_Test_Produit(STracaPtr me, char *Num_Serie_pFin,  char *Date_Debut, int* Id_Test_Produit)
{
	SElExceptionPtr	pexception = NULL;
	int Id_Test_Produit_local;
	char chaine[512];
	int  hstmt;
	char Date_Debut_1[512];
	
	
	//sprintf(chaine,"SELECT Id FROM Test WHERE Num_Serie= '%s' AND DateDebut='2020-12-16 13:19:18.000' ",Num_Serie_pFin);
	//sprintf(chaine,"SELECT Id FROM Test WHERE Num_Serie= '%s' AND DateDebut='16/12/2020 13:19:18' ",Num_Serie_pFin);	
	sprintf(chaine,"SELECT TOP (1) Test.Id FROM Test WHERE (Test.Num_Serie) = '%s' ",Num_Serie_pFin);	
    hstmt = DBActivateSQL (me->Handle_Traca, chaine);
	//DBGetColCharBuffer (hstmt, 1, Date_Debut_1, 40, "");
	DBGetColInt(hstmt,1,&Id_Test_Produit_local);
	*Id_Test_Produit = Id_Test_Produit_local ;
	
	//printf ("Get_Id_Test_Produit requete : \r\n  %s \r\n and Id_Test_Produit_local = \r\n %d \r\n", chaine,Id_Test_Produit);
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{BSH}::Set_Measurement"
SElExceptionPtr Set_Measurement(STracaPtr me, int Id_Test_Produit, char *Nom_Mesure ,float Low_Limit, float High_Limit, float Mesure,bool_t Result)
{
	SElExceptionPtr	pexception = NULL;
	char chaine[512];
	int hstmt;
	char msg[1024];
	

	sprintf (chaine, "INSERT INTO MESURES (Id_Test,Nom_Mesure,Low_Limit,High_Limit,Mesure,Result) VALUES ('%d','%s','%f','%f','%f','%d')",Id_Test_Produit,Nom_Mesure ,Low_Limit,High_Limit,Mesure,Result);	
	//hstmt = DBActivateSQL (me->Handle_Traca,chaine);
		for(int i=0;i<100;i++)
		{
			hstmt = DBActivateSQL (me->Handle_Traca, chaine);
	//printf ("Set_Measurement requete : \r\n %s \r\n HSTMT : \r\n %d \r\n", chaine,hstmt);	
		}
	if (hstmt <= 0)
	{
		sprintf (msg, "Set_Measurement Erreur requete \r\n  %s \r\n", chaine);
		EXCTHROW(-1, msg);
	}
Error:
	EXCRETHROW( pexception);
}


#pragma warning( pop)
