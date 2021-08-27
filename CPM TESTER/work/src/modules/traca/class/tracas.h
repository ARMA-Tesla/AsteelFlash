#ifndef _TRACAS_H_
#define _TRACAS_H_

#include <windows.h>
#include <exception/Exception.h>

#define MAX_TRACA_NAME			128
#define MAX_BUFFER_SIZE				20

#define TRACA_CONNECTION_FAILED				-1
#define TRACA_SEND_FAILED						-1
#define TRACA_RESET_FAILED					-1
#define TRACA_READ_STATUS_FAILED				-1

#define MAX_FIELDS				20
#define MAX_LENGTH				128
 
typedef struct _Traca
{
	int                 Handle_Traca;
	char				ConnetionString[1024];
	char*				tracaName;
	char*				optionString;
	WORD				port;
	WORD				baudRate;
	CRITICAL_SECTION	lock;
	int32_t				status;
	int32_t				readMode;
	char				readBuffer[MAX_BUFFER_SIZE];
	HANDLE				specLib;

	
	SElExceptionPtr (*Open)(struct _Traca* me,const char*	resource, const char* optionString);
	SElExceptionPtr (*Close)(struct _Traca* me);
	SElExceptionPtr (*Connect)(struct _Traca* me);
	SElExceptionPtr (*Disconnect)(struct _Traca* me);
	SElExceptionPtr (*Get_BoardVer_sFin_Valide)(struct _Traca* me, char *board_Ver_sFin, int* id_board_sFin, bool_t *valide);
	SElExceptionPtr (*Get_BoardVer_sFin_Active)(struct _Traca* me, int id_board_sFin,  bool_t *valide, bool_t *active);
	SElExceptionPtr (*Get_Reference_Product)(struct _Traca* me, int id_board_sFin, int *id_board_pFin, char *ProductRefernce);
	SElExceptionPtr (*Get_BoardVer_pFin)(struct _Traca* me, int id_board_pFin, char *board_Ver_pFin);
	SElExceptionPtr (*Get_FieldLabel)(struct _Traca* me, int id_board_pFin, int* count, char FieldName[MAX_FIELDS][MAX_LENGTH], char FieldData[MAX_FIELDS][MAX_LENGTH]);
	SElExceptionPtr (*Get_Last_Mouvement)(struct _Traca* me, char *Num_Serie, char *TypeTest, bool_t *result);
	SElExceptionPtr (*Set_Last_Mouvement)(struct _Traca* me, char *Num_Serie, int id_board, char *DateDebut, char *DateFin, char *Id_Machine, char *TypeTest, bool_t result);
	SElExceptionPtr (*Set_Association)(struct _Traca* me, char *Num_Serie_sFin, char *Num_Serie_pFin, char *Num_Serie_Client, char *date_association);
	SElExceptionPtr (*Test_SMM)(struct _Traca* me, char *Board_Ver_pFin, bool_t *result);
	SElExceptionPtr (*Get_SMM_SN)(struct _Traca* me, char *Num_Serie_sFin, char *Num_Serie_SMM);
	SElExceptionPtr (*Get_Id_Test_Produit)(struct _Traca* me, char *Num_Serie_pFin,  char *Date_Debut, int* Id_Test_Produit);
	SElExceptionPtr (*Set_Measurement)(struct _Traca* me, int Id_Test_Produit, char *Nom_Mesure ,float Low_Limit, float High_Limit, float Mesure,bool_t Result);	
		
	/* Private */
	SElExceptionPtr (*_ConnectSpec)(struct _Traca* me);
	SElExceptionPtr (*_DisconnectSpec)(struct _Traca* me);
	SElExceptionPtr (*_Get_BoardVer_sFin_ValideSpec)(struct _Traca* me, char *board_Ver_sFin, int* id_board_sFin, bool_t *valide);
	SElExceptionPtr (*_Get_BoardVer_sFin_ActiveSpec)(struct _Traca* me, int id_board_sFin,  bool_t *valide, bool_t *active);
	SElExceptionPtr (*_Get_Reference_ProductSpec)(struct _Traca* me, int id_board_sFin, int *id_board_pFin, char *ProductRefernce);
	SElExceptionPtr (*_Get_BoardVer_pFinSpec)(struct _Traca* me, int id_board_pFin, char *board_Ver_pFin);
	SElExceptionPtr (*_Get_FieldLabelSpec)(struct _Traca* me, int id_board_pFin, int* count, char FieldName[MAX_FIELDS][MAX_LENGTH], char FieldData[MAX_FIELDS][MAX_LENGTH]);
	SElExceptionPtr (*_Get_Last_MouvementSpec)(struct _Traca* me, char *Num_Serie, char *TypeTest, bool_t *result);
	SElExceptionPtr (*_Set_Last_MouvementSpec)(struct _Traca* me, char *Num_Serie, int id_board, char *DateDebut, char *DateFin, char *Id_Machine, char *TypeTest, bool_t result);
	SElExceptionPtr (*_Set_AssociationSpec)(struct _Traca* me, char *Num_Serie_sFin, char *Num_Serie_pFin, char *Num_Serie_Client, char *date_association);
	SElExceptionPtr (*_Test_SMMSpec)(struct _Traca* me, char *Board_Ver_pFin, bool_t *result);
	SElExceptionPtr (*_Get_SMM_SNSpec)(struct _Traca* me, char *Num_Serie_sFin, char *Num_Serie_SMM);
	SElExceptionPtr (*_Get_Id_Test_ProduitSpec)(struct _Traca* me, char *Num_Serie_pFin,  char *Date_Debut, int* Id_Test_Produit);
	SElExceptionPtr (*_Set_MeasurementSpec)(struct _Traca* me, int Id_Test_Produit, char *Nom_Mesure ,float Low_Limit, float High_Limit, float Mesure,bool_t Result);
	
} STraca, *STracaPtr;


typedef struct _Tracas
{
	STracaPtr*	tracas;
	int32_t			numTracas;
	int32_t			currentIndex;
	
	SElExceptionPtr (*GetTracaByName)(
		struct _Tracas*	me,
		const char*			tracaName,
		STracaPtr*		traca
	);
	SElExceptionPtr (*GetTracaByIndex)(
		struct _Tracas*	me,
		int32_t				index,
		STracaPtr*		traca
	);
	SElExceptionPtr (*GetCurrentTraca)(
		struct _Tracas*	me,
		STracaPtr*		traca
	);
	SElExceptionPtr (*SetCurrentTracaByName)(
		struct _Tracas*	me,
		const char*			tracaName
	);
	SElExceptionPtr (*GetFirstTraca)(
		struct _Tracas*	me,
		STracaPtr*		traca
	);
	SElExceptionPtr (*GetNextTraca)(
		struct _Tracas*	me,
		STracaPtr*		traca
	);
	
	SElExceptionPtr (*InitTracas)(struct _Tracas* me);
	SElExceptionPtr (*CloseTracas)(struct _Tracas* me);
	
} STracas, *STracasPtr;

SElExceptionPtr tracas_new(STracasPtr* tracas);
SElExceptionPtr tracas_delete(STracasPtr* tracas);

SElExceptionPtr traca_new(
	STracaPtr*	traca,
	const char*		name,
	const char*		optionString
);
SElExceptionPtr traca_delete(STracaPtr* traca);

#endif /* _TRACAS_H_ */
