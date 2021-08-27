
#ifndef _TRACA_SPECIFIC_BSH_
#define _TRACA_SPECIFIC_BSH_

#include <traca/class/tracas.h>

#if defined(WINDOWS)
	#define TRACA_API		__declspec( dllexport )
#elif defined(UNIX)
	#define TRACA_API		extern "C"
#else
	#define TRACA_API
#endif

#define MAX_FIELDS 20
#define MAX_LENGTH 128

TRACA_API SElExceptionPtr _FUNCC Connect(STracaPtr me);
TRACA_API SElExceptionPtr _FUNCC Disconnect(STracaPtr me);
TRACA_API SElExceptionPtr _FUNCC Get_BoardVer_sFin_Valide(STracaPtr me, char *board_Ver_sFin, int* id_board_sFin, bool_t *valide);
TRACA_API SElExceptionPtr _FUNCC Get_BoardVer_sFin_Active(STracaPtr me, int id_board_sFin, bool_t *valide, bool_t *active);
TRACA_API SElExceptionPtr _FUNCC Get_Reference_Product(STracaPtr me, int id_board_sFin, int *id_board_pFin, char *ProductRefernce);
TRACA_API SElExceptionPtr _FUNCC Get_BoardVer_pFin(STracaPtr me, int id_board_pFin, char *board_Ver_pFin);
TRACA_API SElExceptionPtr _FUNCC Get_FieldLabel(STracaPtr me, int id_board_pFin, int* count, char FieldName[MAX_FIELDS][MAX_LENGTH] ,char FieldData[MAX_FIELDS][MAX_LENGTH]);
TRACA_API SElExceptionPtr _FUNCC Get_Last_Mouvement(STracaPtr me, char *Num_Serie, char *TypeTest, bool_t *result);
TRACA_API SElExceptionPtr _FUNCC Set_Last_Mouvement(STracaPtr me, char *Num_Serie, int id_board_pFin, char *DateDebut, char *DateFin, char *Id_Machine, char *TypeTest, bool_t result);
TRACA_API SElExceptionPtr _FUNCC Set_Association(STracaPtr me, char *Num_Serie_sFin, char *Num_Serie_pFin, char *Num_Serie_Client, char *date_association);
TRACA_API SElExceptionPtr _FUNCC Test_SMM(STracaPtr me, char* Board_Ver_pFin ,bool_t *subassembly);
TRACA_API SElExceptionPtr _FUNCC Get_SMM_SN(STracaPtr me, char* Num_Serie_sFin , char* Num_Serie_SMM);
TRACA_API SElExceptionPtr _FUNCC Get_Id_Test_Produit(STracaPtr me, char *Num_Serie_pFin,  char *Date_Debut, int* Id_Test_Produit);
TRACA_API SElExceptionPtr _FUNCC Set_Measurement(STracaPtr me, int Id_Test_Produit, char *Nom_Mesure ,float Low_Limit, float High_Limit, float Mesure,bool_t Result);

#endif /* _TRACA_SPECIFIC_BSH_ */
