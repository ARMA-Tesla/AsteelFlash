#include "tracas.h"
#include <string.h>
#include <stdio.h>
#include <cviauto.h>
#include "msxmldom.h"
#include <utility.h>

#define DISCARD_HANDLE(handle) \
	if (handle) \
	{ \
		CA_DiscardObjHandle(handle); \
		handle = 0; \
	}

#define CA_FREE_MEMORY(mem) \
	if (mem) \
	{ \
		CA_FreeMemory(mem); \
		mem = NULL; \
	}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

static SElExceptionPtr traca_open(
		struct _Traca*	me,
		const char*			resource,
		const char*			optionString
);
static SElExceptionPtr traca_close(
		struct _Traca* me
);
static SElExceptionPtr traca_connect(STracaPtr me);
static SElExceptionPtr traca_disconnect(STracaPtr me);
static SElExceptionPtr traca_Get_BoardVer_sFin_Valide(STracaPtr me, char *board_Ver_sFin, int* id_board_sFin, bool_t *valide);
static SElExceptionPtr traca_Get_BoardVer_sFin_Active(STracaPtr me, int id_board_sFin, bool_t *valide, bool_t *active);
static SElExceptionPtr traca_Get_Reference_Product(STracaPtr me, int id_board_sFin, int *id_board_pFin, char *ProductRefernce);
static SElExceptionPtr traca_Get_BoardVer_pFin(STracaPtr me, int id_board_pFin, char *board_Ver_pFin);
static SElExceptionPtr traca_Get_FieldLabel(STracaPtr me, int id_board_pFin, int* count, char FieldName[MAX_FIELDS][MAX_LENGTH], char FieldData[MAX_FIELDS][MAX_LENGTH]);
static SElExceptionPtr traca_Get_Last_Mouvement(STracaPtr me, char *Num_Serie, char *TypeTest, bool_t *result);
static SElExceptionPtr traca_Set_Last_Mouvement(STracaPtr me, char *Num_Serie, int id_board, char *DateDebut, char *DateFin, char *Id_Machine, char *TypeTest, bool_t result);
static SElExceptionPtr traca_Set_Association(STracaPtr me, char *Num_Serie_sFin, char *Num_Serie_pFin, char *Num_Serie_Client, char *date_association);
static SElExceptionPtr traca_Test_SMM(STracaPtr me, char *Board_Ver_pFin, bool_t *result);
static SElExceptionPtr traca_Get_SMM_SN(STracaPtr me, char *Num_Serie_sFin, char *Num_Serie_SMM);
/*****************************************************************************/

static SElExceptionPtr tracas_GetTracaByName(
	struct _Tracas*	me,
	const char*			tracaName,
	STracaPtr*		traca
);
static SElExceptionPtr tracas_GetTracaByIndex(
	struct _Tracas*	me,
	int32_t				index,
	STracaPtr*		traca
);
static SElExceptionPtr tracas_GetCurrentTraca(
	struct _Tracas*	me,
	STracaPtr*		traca
);
static SElExceptionPtr tracas_SetCurrentTracaByName(
	struct _Tracas*	me,
	const char*			tracaName
);
static SElExceptionPtr tracas_GetFirstTraca(
	struct _Tracas*	me,
	STracaPtr*		traca
);
static SElExceptionPtr tracas_GetNextTraca(
	struct _Tracas*	me,
	STracaPtr*		traca
);
	
static SElExceptionPtr tracas_InitTracas(struct _Tracas* me);
static SElExceptionPtr tracas_CloseTracas(struct _Tracas* me);

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "traca_new"
SElExceptionPtr traca_new(
	STracaPtr*	traca,
	const char*		name,
	const char*		optionString
)
{
	SElExceptionPtr	pexception = NULL;
	STracaPtr		pnew_traca = NULL;
	
	pnew_traca = calloc(1, sizeof(STraca));
	EXCCHECKALLOC( pnew_traca);
	InitializeCriticalSection(&(pnew_traca->lock));
	
	pnew_traca->tracaName = calloc(strlen(name) + 1, sizeof(char));
	EXCCHECKALLOC( pnew_traca->tracaName);
	strcpy(pnew_traca->tracaName, name);
	
	pnew_traca->Open						= traca_open;
	pnew_traca->Close						= traca_close;
	pnew_traca->Connect						= traca_connect;
	pnew_traca->Disconnect					= traca_disconnect;
	pnew_traca->Get_BoardVer_sFin_Valide	= traca_Get_BoardVer_sFin_Valide;
	pnew_traca->Get_BoardVer_sFin_Active	= traca_Get_BoardVer_sFin_Active;
	pnew_traca->Get_Reference_Product		= traca_Get_Reference_Product;
	pnew_traca->Get_BoardVer_pFin			= traca_Get_BoardVer_pFin;
	pnew_traca->Get_FieldLabel				= traca_Get_FieldLabel;
	pnew_traca->Get_Last_Mouvement			= traca_Get_Last_Mouvement;
	pnew_traca->Set_Last_Mouvement			= traca_Set_Last_Mouvement;
	pnew_traca->Set_Association				= traca_Set_Association;
	pnew_traca->Test_SMM					= traca_Test_SMM;
	pnew_traca->Get_SMM_SN					= traca_Get_SMM_SN;

	if (traca)
		*traca = pnew_traca;
	
Error:
	if (pexception && pnew_traca)
		free(pnew_traca);
	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "traca_delete"
SElExceptionPtr traca_delete(STracaPtr* traca)
{
	SElExceptionPtr	pexception = NULL;
	
	if (traca && *traca)
	{
		(*traca)->Close(*traca);
		
		if ((*traca)->tracaName)
			free((*traca)->tracaName);
		
		if ((*traca)->optionString)
			free((*traca)->optionString);
		
		DeleteCriticalSection( &((*traca)->lock) );
		
		free(*traca);
		*traca = NULL;
	}
	
Error:
	EXCRETHROW( pexception);
}

/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "tracas_new"
SElExceptionPtr tracas_new(STracasPtr*	tracas)
{
	SElExceptionPtr	pexception = NULL;
	STracasPtr	pnew_tracas = NULL;
	
	pnew_tracas = calloc(1, sizeof(STracas));
	EXCCHECKALLOC( pnew_tracas);
	
	pnew_tracas->tracas = NULL;
	pnew_tracas->numTracas = 0;
	pnew_tracas->currentIndex = -1;
	
	pnew_tracas->GetTracaByName			= tracas_GetTracaByName;
	pnew_tracas->GetTracaByIndex		= tracas_GetTracaByIndex;
	pnew_tracas->GetCurrentTraca		= tracas_GetCurrentTraca;
	pnew_tracas->SetCurrentTracaByName	= tracas_SetCurrentTracaByName;
	pnew_tracas->GetFirstTraca			= tracas_GetFirstTraca;
	pnew_tracas->GetNextTraca			= tracas_GetNextTraca;
	pnew_tracas->InitTracas				= tracas_InitTracas;
	pnew_tracas->CloseTracas			= tracas_CloseTracas;
	
	if (tracas)
		*tracas = pnew_tracas;
	
Error:
	if (pexception && pnew_tracas)
		free(pnew_tracas);
	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "tracas_delete"
SElExceptionPtr tracas_delete(STracasPtr* tracas)
{
	SElExceptionPtr	pexception = NULL;
	
	if (tracas && *tracas)
	{
		EXCCHECK( (*tracas)->CloseTracas(*tracas));
		
		free(*tracas);
		*tracas = NULL;
	}
	
Error:
	EXCRETHROW( pexception);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "{Traca}::Open"
static SElExceptionPtr traca_open(
		struct _Traca*	me,
		const char*			resource,
		const char*			optionString
)
{
	SElExceptionPtr	pexception = NULL;
	HRESULT			hr = S_OK;
	VARIANT_BOOL	is_xml_success = 1;
	CAObjHandle		document = 0;
	CAObjHandle		xml_configuration = 0;
	CAObjHandle		xml_node = 0;
	char*			xml_type = NULL;
	char*			xml_value = NULL;
	char			message[1024] = "";
	
	
	me->optionString = calloc(strlen(optionString) + 1, sizeof(char));
	EXCCHECKALLOC( me->optionString);
	strcpy(me->optionString, optionString);
	
	EXCCHECKCOM( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &document));
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentloadXML (document, NULL, optionString, &is_xml_success));
	
	EXCCHECKCOM( MSXML_IXMLDOMDocumentselectSingleNode(document, NULL,
													   "/Traca/Configuration",
													   &xml_configuration));
	if (xml_configuration == 0)
		EXCTHROW( -1, "Missing Configuration Node");
	
	/* Load Specific DLL functions */
	EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(xml_configuration, NULL, "@specificDll", &xml_node));
	if (xml_node == 0)
		EXCTHROW( -3, "Missing Traca Specific Library");
	
	EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(xml_node, NULL, &xml_value));
	me->specLib = LoadLibrary(xml_value);
	
	if (me->specLib == 0)
	{
		sprintf(message, "Traca Library (%s) Loading Failed", xml_value);
		EXCTHROW( -4, message);
	}
	
	me->_ConnectSpec	= (SElExceptionPtr (*)(STracaPtr))
								GetProcAddress(me->specLib, "Connect");
	
	me->_DisconnectSpec	= (SElExceptionPtr (*)(STracaPtr))
								GetProcAddress(me->specLib, "Disconnect");
	
	me->_Get_BoardVer_sFin_ValideSpec		= (SElExceptionPtr (*)(STracaPtr, char *, int* , bool_t *))
													GetProcAddress(me->specLib, "Get_BoardVer_sFin_Valide");
	
	me->_Get_BoardVer_sFin_ActiveSpec		= (SElExceptionPtr (*)(STracaPtr, int , bool_t *, bool_t *))
													GetProcAddress(me->specLib, "Get_BoardVer_sFin_Active");
	
	me->_Get_Reference_ProductSpec			= (SElExceptionPtr (*)(STracaPtr, int , int *, char *))
													GetProcAddress(me->specLib, "Get_Reference_Product");
	
	me->_Get_BoardVer_pFinSpec				= (SElExceptionPtr (*)(STracaPtr, int, char *))
													GetProcAddress(me->specLib, "Get_BoardVer_pFin");
	
	me->_Get_FieldLabelSpec					= (SElExceptionPtr (*)(STracaPtr, int, int*, char **, char **))
													GetProcAddress(me->specLib, "Get_FieldLabel");
	
	me->_Get_Last_MouvementSpec				= (SElExceptionPtr (*)(STracaPtr, char *, char *, bool_t *))
													GetProcAddress(me->specLib, "Get_Last_Mouvement");
		
	me->_Set_Last_MouvementSpec				= (SElExceptionPtr (*)(STracaPtr, char *, int , char *, char *, char *, char *, bool_t))
													GetProcAddress(me->specLib, "Set_Last_Mouvement");

	me->_Set_AssociationSpec				= (SElExceptionPtr (*)(STracaPtr, char * , char *, char *, char *))
													GetProcAddress(me->specLib, "Set_Association");
	
	me->_Test_SMMSpec						= (SElExceptionPtr (*)(STracaPtr, char * , bool_t *))
													GetProcAddress(me->specLib, "Test_SMM");
	
	me->_Get_SMM_SNSpec						= (SElExceptionPtr (*)(STracaPtr, char * , char *))
													GetProcAddress(me->specLib, "Get_SMM_SN");

	CA_FREE_MEMORY(xml_value);
	DISCARD_HANDLE(xml_node);
	
	/* Get String Connection */
	EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(xml_configuration, NULL,
												   "ConnectionString", &xml_node));
	
	if (xml_node == 0)
		EXCTHROW( -5, "Manque Chaine de Connection dans le fichier XML!");
	
	EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(xml_node, NULL, &xml_value));
	sprintf (me->ConnetionString, xml_value);
	//EXCCHECK( me->_ConnectSpec (me));
	CA_FREE_MEMORY(xml_value);
			
	DISCARD_HANDLE(xml_node);

Error:

	if (pexception)
	{
		if (me->specLib)
		{
			FreeLibrary(me->specLib);
			me->specLib = 0;
		}
		
		me->_ConnectSpec	= NULL;
		me->_DisconnectSpec	= NULL;
		me->_Get_BoardVer_sFin_ValideSpec	= NULL;
		me->_Get_BoardVer_sFin_ActiveSpec	= NULL;
		me->_Get_Reference_ProductSpec	= NULL;
		me->_Get_BoardVer_pFinSpec	= NULL;
		me->_Get_FieldLabelSpec	= NULL;
		me->_Get_Last_MouvementSpec	= NULL;
		me->_Set_Last_MouvementSpec	= NULL;	
		me->_Set_AssociationSpec	= NULL;	
		me->_Test_SMMSpec	= NULL;	
		me->_Get_SMM_SNSpec	= NULL;	
	}
		
	DISCARD_HANDLE(xml_node);
	CA_FREE_MEMORY(xml_value);
	CA_FREE_MEMORY(xml_type);
	DISCARD_HANDLE(document);
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Traca}::Close"
static SElExceptionPtr traca_close(struct _Traca* me)
{
	SElExceptionPtr	pexception = NULL;
	
		me->_ConnectSpec	= NULL;
		me->_DisconnectSpec	= NULL;
		me->_Get_BoardVer_sFin_ValideSpec	= NULL;
		me->_Get_BoardVer_sFin_ActiveSpec	= NULL;
		me->_Get_Reference_ProductSpec	= NULL;
		me->_Get_BoardVer_pFinSpec	= NULL;
		me->_Get_FieldLabelSpec	= NULL;
		me->_Get_Last_MouvementSpec	= NULL;
		me->_Set_Last_MouvementSpec	= NULL;
		me->_Set_AssociationSpec	= NULL;	
		me->_Test_SMMSpec	= NULL;	
		me->_Get_SMM_SNSpec	= NULL;	
Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "tracaLock"
static SElExceptionPtr tracaLock(STracaPtr me)
{
	EnterCriticalSection( &me->lock);    
	return NULL;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "tracaUnLock"
static SElExceptionPtr tracaUnLock(STracaPtr me)
{
	LeaveCriticalSection( &me->lock);   
	return NULL;
}
#undef __FUNC__
#define __FUNC__ "{Traca}::traca_connect"
static SElExceptionPtr traca_connect(STracaPtr me)
{
	SElExceptionPtr	pexception = NULL;
	
	tracaLock(me);

	if (me->_ConnectSpec)
		EXCCHECK( me->_ConnectSpec(me));

Error:
	tracaUnLock(me);
	EXCRETHROW( pexception);
}
#undef __FUNC__
#define __FUNC__ "{Traca}::traca_disconnect"
static SElExceptionPtr traca_disconnect(STracaPtr me)
{
	SElExceptionPtr	pexception = NULL;
	
	tracaLock(me);

	if (me->_DisconnectSpec)
		EXCCHECK( me->_DisconnectSpec(me));

Error:
	tracaUnLock(me);
	EXCRETHROW( pexception);
}
#undef __FUNC__
#define __FUNC__ "{Traca}::traca_Get_BoardVer_sFin_Valide"
static SElExceptionPtr traca_Get_BoardVer_sFin_Valide(STracaPtr me, char *board_Ver_sFin, int* id_board_sFin, bool_t *valide)
{
	SElExceptionPtr	pexception = NULL;
	
	tracaLock(me);

	if (me->_Get_BoardVer_sFin_ValideSpec)
		EXCCHECK( me->_Get_BoardVer_sFin_ValideSpec(me, board_Ver_sFin, id_board_sFin, valide));

Error:
	tracaUnLock(me);
	EXCRETHROW( pexception);
}
#undef __FUNC__
#define __FUNC__ "{Traca}::traca_Get_BoardVer_sFin_Active"
static SElExceptionPtr traca_Get_BoardVer_sFin_Active(STracaPtr me, int id_board_sFin, bool_t *valide, bool_t *active)
{
	SElExceptionPtr	pexception = NULL;
	tracaLock(me);

	if (me->_Get_BoardVer_sFin_ActiveSpec)
		EXCCHECK( me->_Get_BoardVer_sFin_ActiveSpec(me, id_board_sFin, valide, active));
	
Error:
	tracaUnLock(me);
	EXCRETHROW( pexception);
}
#undef __FUNC__
#define __FUNC__ "{Traca}::traca_Get_Reference_Product"
static SElExceptionPtr traca_Get_Reference_Product(STracaPtr me, int id_board_sFin, int *id_board_pFin, char *ProductRefernce)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_Get_Reference_ProductSpec)
		EXCCHECK( me->_Get_Reference_ProductSpec(me, id_board_sFin, id_board_pFin, ProductRefernce));
	
Error:
	EXCRETHROW( pexception);
}
#undef __FUNC__
#define __FUNC__ "{Traca}::traca_Get_BoardVer_pFin"
static SElExceptionPtr traca_Get_BoardVer_pFin(STracaPtr me, int id_board_pFin, char *board_Ver_pFin)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_Get_BoardVer_pFinSpec)
		EXCCHECK( me->_Get_BoardVer_pFinSpec(me, id_board_pFin, board_Ver_pFin));
	
Error:
	EXCRETHROW( pexception);
}
#undef __FUNC__
#define __FUNC__ "{Traca}::traca_Get_FieldLabel"
static SElExceptionPtr traca_Get_FieldLabel(STracaPtr me, int id_board_pFin, int* count, char FieldName[MAX_FIELDS][MAX_LENGTH], char FieldData[MAX_FIELDS][MAX_LENGTH])
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_Get_FieldLabelSpec)
		EXCCHECK( me->_Get_FieldLabelSpec(me, id_board_pFin, count, FieldName, FieldData));
	
Error:
	EXCRETHROW( pexception);
}
#undef __FUNC__
#define __FUNC__ "{Traca}::traca_Get_Last_Mouvement"
static SElExceptionPtr traca_Get_Last_Mouvement(STracaPtr me, char *Num_Serie, char *TypeTest, bool_t *result)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_Get_Last_MouvementSpec)
		EXCCHECK( me->_Get_Last_MouvementSpec(me, Num_Serie, TypeTest, result));
	
Error:
	EXCRETHROW( pexception);
}
#undef __FUNC__
#define __FUNC__ "{Traca}::traca_Set_Last_Mouvement"
static SElExceptionPtr traca_Set_Last_Mouvement(STracaPtr me, char *Num_Serie, int id_board, char *DateDebut, char *DateFin, char *Id_Machine, char *TypeTest, bool_t result)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_Set_Last_MouvementSpec)
		EXCCHECK( me->_Set_Last_MouvementSpec(me, Num_Serie, id_board, DateDebut, DateFin, Id_Machine, TypeTest, result));
	
Error:
	EXCRETHROW( pexception);
}
#undef __FUNC__
#define __FUNC__ "{Traca}::traca_Set_Association"
static SElExceptionPtr traca_Set_Association(STracaPtr me, char *Num_Serie_sFin, char *Num_Serie_pFin, char *Num_Serie_Client, char *date_association)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_Set_AssociationSpec)
		EXCCHECK( me->_Set_AssociationSpec(me, Num_Serie_sFin, Num_Serie_pFin, Num_Serie_Client, date_association));
	
Error:
	EXCRETHROW( pexception);
}
#undef __FUNC__
#define __FUNC__ "{Traca}::traca_Test_SMM"
static SElExceptionPtr traca_Test_SMM(STracaPtr me, char *Board_Ver_pFin, bool_t *result)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_Test_SMMSpec)
		EXCCHECK( me->_Test_SMMSpec(me, Board_Ver_pFin, result));
	
Error:
	EXCRETHROW( pexception);
}
#undef __FUNC__
#define __FUNC__ "{Traca}::traca_Get_SMM_SN"
static SElExceptionPtr traca_Get_SMM_SN(STracaPtr me, char *Num_Serie_sFin, char *Num_Serie_SMM)
{
	SElExceptionPtr	pexception = NULL;
	

	if (me->_Get_SMM_SNSpec)
		EXCCHECK( me->_Get_SMM_SNSpec(me, Num_Serie_sFin, Num_Serie_SMM));
	
Error:
	EXCRETHROW( pexception);
}
/*****************************************************************************/

#undef __FUNC__
#define __FUNC__ "{Tracas}::GetTracaByName"
static SElExceptionPtr tracas_GetTracaByName(
	struct _Tracas*	me,
	const char*			tracaName,
	STracaPtr*		traca
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			i = 0;
	STracaPtr		found_traca = NULL;
	char			message[1024] = "";
	
	for (i = 0; i < me->numTracas; i++)
	{
		if (!strcmp(me->tracas[i]->tracaName, tracaName))
		{
			found_traca = me->tracas[i];
			break;
		}
	}
	
	if (traca)
		*traca = found_traca;
	
	if (found_traca == NULL)
	{
		sprintf(message, "Traca Name (%s) Not Found", tracaName);
		EXCTHROW( -12, message);
	}
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Tracas}::GetTracaByIndex"
static SElExceptionPtr tracas_GetTracaByIndex(
	struct _Tracas*	me,
	int32_t				index,
	STracaPtr*		traca
)
{
	SElExceptionPtr	pexception = NULL;
	
	if (index < me->numTracas && index >= 0)
	{
		if (traca)
			*traca = me->tracas[index];
	}
	else
	{
		if (traca)
			*traca = NULL;
		
		EXCTHROW( -13, "Index Out of Range");
	}
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Tracas}::GetCurrentTraca"
static SElExceptionPtr tracas_GetCurrentTraca(
	struct _Tracas*	me,
	STracaPtr*		traca
)
{
	SElExceptionPtr	pexception = NULL;
	
	if (me->currentIndex >= 0 && me->numTracas > 0)
	{
		if (traca)
			*traca = me->tracas[me->currentIndex];
	}
	else
	{
		if (traca)
			*traca = NULL;
	}

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Tracas}::SetCurrentTracaByName"
static SElExceptionPtr tracas_SetCurrentTracaByName(
	struct _Tracas*	me,
	const char*			tracaName
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			i = 0;
	STracaPtr		traca = NULL;
	char			message[1024] = "";

	for (i = 0; i < me->numTracas; i++)
	{
		if (!strcmp(me->tracas[i]->tracaName, tracaName))
		{
			me->currentIndex = i;
			break;
		}
	}
	
	if (i >= me->numTracas)
	{
		sprintf(message, "Traca Name (%s) Not Found", tracaName);
		EXCTHROW( -12, message);
	}

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Tracas}::GetFirstTraca"
static SElExceptionPtr tracas_GetFirstTraca(
	struct _Tracas*	me,
	STracaPtr*		traca
)
{
	SElExceptionPtr	pexception = NULL;
	
	me->currentIndex = 0;

	if(me->numTracas > 0)
	{
		if (traca)
			*traca = me->tracas[0];
	}
	else
	{
		if (traca)
			*traca = NULL;
	}

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Tracas}::GetNextTraca"
static SElExceptionPtr tracas_GetNextTraca(
	struct _Tracas*	me,
	STracaPtr*		traca
)
{
	SElExceptionPtr	pexception = NULL;

	if (++(me->currentIndex) >= me->numTracas)
	{
		me->currentIndex = me->numTracas - 1;
		if (traca)
			*traca = NULL;
	}
	else
	{
		if (traca)
			*traca = me->tracas[me->currentIndex];
	}
	
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Tracas}::InitTracas"
static SElExceptionPtr tracas_InitTracas(struct _Tracas* me)
{
	SElExceptionPtr	pexception = NULL;
	HRESULT			hr = S_OK;
	VARIANT_BOOL	is_xml_success = 1;
	VARIANT			file;
	CAObjHandle		document = 0;
	CAObjHandle		traca = 0;
	CAObjHandle		traca_name_node = 0;
	CAObjHandle		traca_list = 0;
	char*			xml_value = NULL;
	char*			traca_xml = NULL;
	
	EXCCHECKCOM( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &document));
	
	CA_VariantSetCString (&file, "modules/tracas.xml");
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentload(document, NULL, file, &is_xml_success));
	
	EXCCHECKCOM( MSXML_IXMLDOMDocumentselectNodes(document, NULL, "/Tracas/Traca", &traca_list));
	EXCCHECKCOM( MSXML_IXMLDOMNodeListreset (traca_list, NULL));
	
	EXCCHECKCOM( MSXML_IXMLDOMNodeListnextNode(traca_list, NULL, &traca));
	while (traca)
	{
		EXCCHECKCOM( MSXML_IXMLDOMNodeselectSingleNode(traca, NULL, "@name", &traca_name_node));
		EXCCHECKCOM( MSXML_IXMLDOMNodeGettext(traca_name_node, NULL, &xml_value));
		EXCCHECKCOM( MSXML_IXMLDOMNodeGetxml(traca, NULL, &traca_xml));
		
		me->tracas = realloc( me->tracas, (me->numTracas + 1) * sizeof(STracaPtr));
		EXCCHECKALLOC( me->tracas);
		
		EXCCHECK( traca_new( &me->tracas[me->numTracas], xml_value, NULL));
		EXCCHECK( me->tracas[me->numTracas]->Open(me->tracas[me->numTracas], NULL, traca_xml));
		me->numTracas++;
		
		CA_FREE_MEMORY(xml_value);
		CA_FREE_MEMORY(traca_xml);
		DISCARD_HANDLE(traca_name_node);
		DISCARD_HANDLE(traca);
		
		EXCCHECKCOM( MSXML_IXMLDOMNodeListnextNode(traca_list, NULL, &traca));
	}
	
Error:
	CA_FREE_MEMORY(xml_value);
	CA_FREE_MEMORY(traca_xml);
	DISCARD_HANDLE(traca_name_node);
	DISCARD_HANDLE(traca);
	DISCARD_HANDLE(traca_list);
	DISCARD_HANDLE(document);
	
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "{Tracas}::CloseTracas"
static SElExceptionPtr tracas_CloseTracas(struct _Tracas* me)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			i = 0;
	
	for (i = 0; i < me->numTracas; i++)
	{
		EXCCHECK( traca_delete(&(me->tracas[i])));
	}
	free(me->tracas);
	me->tracas = NULL;
	me->numTracas = 0;
	
Error:
	EXCRETHROW( pexception);
}
