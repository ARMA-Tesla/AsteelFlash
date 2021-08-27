#include "msxmldom.h"
#include <guid.h>
#include <utility.h>
#include <time.h>
#include <stdio.h>
#include <log/log.h>
#include <odbc/elodbc.h>
#include <multithread/multithread.h>
#include "dbs.h"
#include "dbs_schema.h"  
#include "dbs_define_local.h"

#define ODBC_QUERY_TIMEOUT	2000

static SElExceptionPtr fncLock(struct _SDBS* me);
static SElExceptionPtr fncUnLock(struct _SDBS* me); 
	
static SElExceptionPtr dbs_Initialize(struct _SDBS* me, const char* xml_config); 
static SElExceptionPtr dbs_CheckSchema(struct _SDBS* me);
static SElExceptionPtr dbs_Read(struct _SDBS* me);
static SElExceptionPtr dbs_ReadLine(struct _SDBS* me);
static SElExceptionPtr dbs_Save(struct _SDBS* me);
static SElExceptionPtr dbs_SaveLine(struct _SDBS* me);
static SElExceptionPtr dbs_DelHist(struct _SDBS* me);
static SElExceptionPtr dbs_GetUser(struct _SDBS* me, char* user);
static SElExceptionPtr dbs_Free(void* instance);
static SElExceptionPtr dbs_UsedMemory(struct _SDBS* me, uint32_t* size); 
static SElExceptionPtr dbs_ChangeBranchId(struct _SDBS* me, int32_t branch_id, bool_t* reload);
static SElExceptionPtr dbs_Lock(struct _SDBS* me, const char* lock_name);
static SElExceptionPtr dbs_Unlock(struct _SDBS* me, const char* lock_name);

static char* dbs_CharCopy(char* value);
static char* dbs_CharAlloc(char** value, size_t size);

static SElExceptionPtr SetCharacter(struct _SDBS* me);
static SElExceptionPtr SetBranchId(struct _SDBS* me);

static DWORD WINAPI Thread_Dbs(LPVOID param);

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbs_new"
SElExceptionPtr	dbs_new(SDBSPtr* pDBSPtr)
{
	SElExceptionPtr    	pexception = NULL;
	SDBSPtr				me = NULL;

	me = calloc(1,sizeof(SDBS));
	EXCCHECKALLOC(me);

	if(pDBSPtr) *pDBSPtr = me;
	
	InitializeCriticalSection( &me->_Lock);    
	
	me->pdbs			= me;
	me->data_changed	= FALSE;
	
   	me->Read 			= dbs_Read;
	me->ReadLine 		= dbs_ReadLine;
	me->Save			= dbs_Save; 
	me->SaveLine		= dbs_SaveLine; 
	me->DelHist			= dbs_DelHist;   
	me->GetUser			= dbs_GetUser; 
	me->UsedMemory		= dbs_UsedMemory;
	me->Free			= dbs_Free;
	me->CharCopy		= dbs_CharCopy;
	me->CharAlloc		= dbs_CharAlloc;
	me->ChangeBranchId	= dbs_ChangeBranchId;
	me->CheckSchema		= dbs_CheckSchema;
	me->Initialize		= dbs_Initialize;
	me->fncLock 		= fncLock;
	me->fncUnLock 		= fncUnLock;
	me->Lock			= dbs_Lock;
	me->Unlock			= dbs_Unlock;

	me->LogChannel		= E_LOG_CHANNEL_DBS;

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbs_delete"
SElExceptionPtr	dbs_delete(SDBSPtr* pDBSPtr)
{
	SElExceptionPtr	pexception = NULL;

	if (pDBSPtr && *pDBSPtr)
	{
		SDBSPtr	me = *pDBSPtr;

		EXCCHECK( ThreadStop(&me->_hThread, &me->_hEventStop));

		if(me->TesterList!=NULL)
		 	EXCCHECK( dbstesterlist_delete(&me->TesterList));   
#ifndef _NO_USER
		if(me->UserList!=NULL)
		 	EXCCHECK( dbsuserlist_delete(&me->UserList));   
#endif
		if(me->ProductList!=NULL)
		 	EXCCHECK( dbsproductlist_delete(&me->ProductList));

		if(me->ProductParameterList!=NULL)
		 	EXCCHECK( dbsparamlist_delete(&me->ProductParameterList));
#ifndef _NO_MASTER
		if(me->ProductMasterList!=NULL)
		 	EXCCHECK( dbsmasterlist_delete(&me->ProductMasterList));
#endif
#ifndef _NO_TEST
		if(me->TestList!=NULL)
		 	EXCCHECK( dbstestlist_delete(&me->TestList));

		if(me->TestParameterList!=NULL)
		 	EXCCHECK( dbstestparamlist_delete(&me->TestParameterList));
#endif
		if(me->Export!=NULL)
		 	EXCCHECK( dbsexport_delete(&me->Export));

		if(me->Storage!=NULL)
		 	EXCCHECK( dbsstorage_delete(&me->Storage));

		if(me->Kosu!=NULL)
		 	EXCCHECK( dbskosu_delete(&me->Kosu));

		if(me->LineProcessList!=NULL)
		 	EXCCHECK( dbslineprocesslist_delete(&me->LineProcessList));
		
		if(me->LineStepList!=NULL)
		 	EXCCHECK( dbslinesteplist_delete(&me->LineStepList));

		if(me->LineParamList!=NULL)
		 	EXCCHECK( dbslineparamlist_delete(&me->LineParamList));

#if 0    /* formerly excluded lines */
		if(me->podbc!=NULL)
		 	EXCCHECK( odbc_delete((SOdbcPtr*)&me->podbc));
#endif   /* formerly excluded lines */

		if(me->LineExport!=NULL)
		 	EXCCHECK( dbslineexport_delete(&me->LineExport));

		DeleteCriticalSection(&(me->_Lock));   
		
		free(*pDBSPtr);
		*pDBSPtr = NULL;
	}

Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
/* DATABASE SECTION **********************************************************/
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "fncLock"
static SElExceptionPtr fncLock(struct _SDBS* me)
{
	EnterCriticalSection( &me->_Lock);    
	return NULL;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "fncUnLock"
static SElExceptionPtr fncUnLock(struct _SDBS* me)
{
	LeaveCriticalSection( &me->_Lock);   
	return NULL;
}

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Thread_Dbs"
static DWORD WINAPI Thread_Dbs(LPVOID param)
{
	SElExceptionPtr	pexception = NULL;
	SDBSPtr			me = (SDBSPtr) param;
	bool_t			is_locked = FALSE;
	SOdbcPtr		podbc = me->podbc; 
	double			dtime = 0.0;

	while(TRUE)
	{
		if (WaitForSingleObject(me->_hEventStop, 60000) == WAIT_OBJECT_0)
		{
			if(is_locked)
			{
				UNLOCK;
				is_locked = FALSE;
			}

			ExitThread(THREAD_EXIT_CODE);
		}

		LOCK; 
		is_locked = TRUE;

		dtime = difftime(time(NULL), podbc->_time);
		if(dtime>120.0)
		{
			pexception = podbc->ExecSQL(podbc, "select curtime()"); 
			if(pexception && pexception->GetErrorCode(pexception)==-8)
			{
				/* reconect */
				EXCDBGSTR( pexception);
				me->is_connected = FALSE;
				
				EXCCHECK( podbc->Close(podbc));
				pexception = podbc->Open(podbc, me->_odbc, me->_user, me->_password);
				if(!pexception)
				{
					EXCCHECK( podbc->SetQueryTimeout(podbc, ODBC_QUERY_TIMEOUT));
					me->is_connected = TRUE;
				}
				else
					EXCDBGSTR( pexception);
			}
			else
			{
				EXCCHECK( pexception);	
				EXCCHECK( podbc->Cancel(podbc));
			}
		}

		UNLOCK;
		is_locked = FALSE;
	};
	
Error:
	if(is_locked) UNLOCK;
	ExitThread(THREAD_ERROR_CODE);
	EXCDISPLAY(pexception);
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbs_Initialize"
static SElExceptionPtr dbs_Initialize(struct _SDBS* me, const char* fileName)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL;
	HRESULT         hr = 0;  
	CAObjHandle     pxml  = 0;
	CAObjHandle     pnode = 0;
	CAObjHandle		pparameter = 0;
	VBOOL           is_xml_success = 1; 
	char*           ptext= NULL;
	const char*     pfile_name = (fileName) ? fileName : "\\dbs.xml";
	VARIANT         pfile_name_var = CA_VariantEmpty();
	char			path[MAX_PATHNAME_LEN ];
	SOdbcPtr		podbc = NULL;
	char			odbc[256] = "valeo";
	char			user[256] = "valeo";
	char			password[256] = "valeo";
	int32_t			tester_id = TESTER_ID;

	EXCCHECKCVI( GetModuleDir (__CVIUserHInst, path));  
	strcat(path, pfile_name);
	
	EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml));
	EXCCHECKCVI( CA_VariantSetCString(  &pfile_name_var, path));
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (pxml, NULL, pfile_name_var, &is_xml_success));
	
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//module[@id='"MODULE_DBS"']", &pnode));   

	if(pnode)
	{
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "odbc", &pparameter));
		if ( pparameter )
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
			
			strncpy(odbc, ptext, 255);
			
			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
		
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "user", &pparameter));
		if ( pparameter )
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
			
			strncpy(user, ptext, 255);
			
			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
		
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "password", &pparameter));
		if ( pparameter )
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
			
			strncpy(password, ptext, 255);
			
			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
		
		EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pnode, NULL, "tester_id", &pparameter));
		if ( pparameter )
		{
			EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
			
			tester_id = strtol(ptext, NULL, 10);
			
			EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
			pparameter = 0;
			CA_FreeMemory(ptext);
			ptext=NULL;
		}
	}
	
	EXCCHECK( odbc_new(&podbc));
	podbc->LogApi = me->LogApi;

	strcpy(me->_odbc, odbc);
	strcpy(me->_user, user);
	strcpy(me->_password, password);

#if defined(_PRODUCT_LINE_NEW)
	pexception = podbc->Open(podbc, odbc, user, password);
	if(!pexception)
	{
		EXCCHECK( podbc->SetQueryTimeout(podbc, ODBC_QUERY_TIMEOUT));
		me->is_connected = TRUE;
	}
	else
		EXCDBGSTR( pexception);
#else
	EXCCHECK( podbc->Open(podbc, odbc, user, password)); //valeo_mysql; valeo_local
	
	me->is_connected = TRUE;
	EXCCHECK( podbc->SetQueryTimeout(podbc, ODBC_QUERY_TIMEOUT));
#endif
	me->podbc = podbc;
	me->_tester_id = tester_id;

	EXCCHECK( dbstesterlist_new(&me->TesterList, me));
#ifndef _NO_USER
	EXCCHECK( dbsuserlist_new(&me->UserList, me));
#endif
	EXCCHECK( dbsproductlist_new(&me->ProductList, me));
	EXCCHECK( dbsparamlist_new(&me->ProductParameterList, me));
#ifndef _NO_MASTER
	EXCCHECK( dbsmasterlist_new(&me->ProductMasterList, me));
#endif
#ifndef _NO_TEST
	EXCCHECK( dbstestlist_new(&me->TestList, me));
	EXCCHECK( dbstestparamlist_new(&me->TestParameterList, me));
#endif
	EXCCHECK( dbsexport_new(&me->Export, me));
	EXCCHECK( dbsstorage_new(&me->Storage, me));
	EXCCHECK( dbskosu_new(&me->Kosu, me));
	EXCCHECK( me->Kosu->Initialize(me->Kosu, pfile_name));
   
	/* line ini */
	EXCCHECK( dbslineprocesslist_new(&me->LineProcessList, me));
	EXCCHECK( dbslinesteplist_new(&me->LineStepList, me));
	EXCCHECK( dbslineparamlist_new(&me->LineParamList, me));
	EXCCHECK( dbslineexport_new(&me->LineExport, me));

	if(me->is_connected)
	{
		/* read data from database */
		EXCCHECK( me->CheckSchema(me));
		EXCCHECK( SetCharacter(me)); 
		EXCCHECK( PTESTER->TesterRead(PTESTER));
		EXCCHECK( SetBranchId(me));
#ifndef _NO_USER
		EXCCHECK( PUSER->UserRead(PUSER)); 
#endif
		EXCCHECK( me->Read(me));
		EXCCHECK( me->ReadLine(me));
	}

	EXCCHECK( ThreadStart(Thread_Dbs, me, &me->_hThread, &me->_hEventStop));

Error:
	if (pxml) CA_DiscardObjHandle(pxml);
	if (pnode) CA_DiscardObjHandle(pnode);
	if (pparameter) CA_DiscardObjHandle(pparameter);
	CA_FreeMemory(ptext);  
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SetCharacter"
static SElExceptionPtr SetCharacter(struct _SDBS* me)
{
	SElExceptionPtr	pexception = NULL;
	SOdbcPtr		podbc = me->podbc; 
	
	LOCK;

	/* set character set */
	EXCCHECK( podbc->ExecSQL(podbc, "SET CHARACTER SET UTF8"));
	EXCCHECK( podbc->ExecSQL(podbc, "SET NAMES UTF8")); 
	EXCCHECK( podbc->Cancel(podbc));

Error:
	UNLOCK;
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbs_Lock"
static SElExceptionPtr dbs_Lock(struct _SDBS* me, const char* lock_name)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
	SOdbcPtr		podbc = me->podbc; 
	char			sql[SQL_COMMAND_LENGHT] = "";
	int32_t			lock = 0;

	if(lock_name && lock_name[0]!='\0')
	{
		sprintf( sql, "SELECT GET_LOCK('tester.%s', 5)", lock_name);
		EXCCHECK( podbc->ExecSQL(podbc, sql)); 
		CHECKERR( podbc->Fetch( podbc, TRUE));
		sscanf(podbc->GetFieldValuePtr(podbc, 1), "%d", &lock);
		EXCCHECK( podbc->Cancel(podbc));

		if(lock==0)
		{
			char	serror[256];

			sprintf(serror, "Database is locked: \"%s\"", lock_name); 
			EXCTHROW( DBS_ERROR_DBS_IS_LOCKED, serror);
		}
	}

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbs_Unlock"
static SElExceptionPtr dbs_Unlock(struct _SDBS* me, const char* lock_name)
{
	SElExceptionPtr	pexception = NULL;
	SOdbcPtr		podbc = me->podbc; 
	char			sql[SQL_COMMAND_LENGHT] = "";

	if(lock_name && lock_name[0]!='\0')
	{
		sprintf( sql, "SELECT RELEASE_LOCK('tester.%s')", lock_name);
		EXCCHECK( podbc->ExecSQL(podbc, sql)); 
		EXCCHECK( podbc->Cancel(podbc));
	}

Error:
	EXCDISPLAY( pexception); 
	return NULL;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SetBranchId"
static SElExceptionPtr SetBranchId(struct _SDBS* me)
{
	SElExceptionPtr	pexception = NULL;
	STesterItemPtr	ptester = NULL;

	EXCCHECK( PTESTER->TesterGet(PTESTER, &ptester));

	if(ptester)
		PDBS->_branch_id = ptester->product_id;

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbs_ChangeBranchId"
static SElExceptionPtr dbs_ChangeBranchId(struct _SDBS* me, int32_t branch_id, bool_t* reload)
{
	SElExceptionPtr	pexception = NULL;

	if(me->_branch_id==branch_id)
	{
		if(reload) *reload = FALSE;
	}
	else
	{
		me->_branch_id = branch_id;
		EXCCHECK( me->Read(me));
		EXCCHECK( me->ReadLine(me));
		if(reload) *reload = TRUE;
	}

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbs_Read"
static SElExceptionPtr dbs_Read(struct _SDBS* me)
{
	SElExceptionPtr	pexception = NULL;

	EXCCHECK( PPRODUCT->ProductRead(PPRODUCT, me->_branch_id) );
	EXCCHECK( PPARAM->ProductParamRead(PPARAM, me->_branch_id) );
#ifndef _NO_MASTER
	EXCCHECK( PMASTER->ProductMasterRead(PMASTER, me->_branch_id) ); 
#endif
#ifndef _NO_TEST
	EXCCHECK( PTEST->TestRead(PTEST, me->_branch_id) ); 
	EXCCHECK( PTESTPARAM->TestParamRead(PTESTPARAM, me->_branch_id) ); 
#endif
	
	me->data_changed = FALSE;

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbs_ReadLine"
static SElExceptionPtr dbs_ReadLine(struct _SDBS* me)
{
	SElExceptionPtr	pexception = NULL;
	
	EXCCHECK( PPROCESS->LineProcessRead(PPROCESS, me->_branch_id)); 
	EXCCHECK( PSTEP->LineStepRead(PSTEP, me->_branch_id)); 
	EXCCHECK( PLINEPARAM->LineParamRead(PLINEPARAM, me->_branch_id)); 
	
	me->data_changed_line = FALSE;

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbs_Save"
static SElExceptionPtr dbs_Save(struct _SDBS* me) 
{
	SElExceptionPtr	pexception = NULL;

	EXCCHECK( PPRODUCT->ProductSave(PPRODUCT, me->_branch_id));
	EXCCHECK( PPARAM->ProductParamSave(PPARAM, me->_branch_id));
#ifndef _NO_MASTER
	EXCCHECK( PMASTER->ProductMasterSave(PMASTER, me->_branch_id));
#endif
#ifndef _NO_TEST
	EXCCHECK( PTEST->TestSave(PTEST, me->_branch_id) );
	EXCCHECK( PTESTPARAM->TestParamSave(PTESTPARAM, me->_branch_id));
#endif
	me->data_changed = FALSE; 
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbs_SaveLine"
static SElExceptionPtr dbs_SaveLine(struct _SDBS* me) 
{
	SElExceptionPtr	pexception = NULL;

	EXCCHECK( PPROCESS->LineProcessSave(PPROCESS, me->_branch_id));
	EXCCHECK( PSTEP->LineStepSave(PSTEP, me->_branch_id));
	EXCCHECK( PLINEPARAM->LineParamSave(PLINEPARAM, me->_branch_id));

	me->data_changed_line = FALSE; 
	
Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbs_DelHist"
static SElExceptionPtr dbs_DelHist(struct _SDBS* me)
{
	SElExceptionPtr	pexception = NULL;

	EXCCHECK( PPARAM->ProductParamDelHist(PPARAM, me->_branch_id));
#ifndef _NO_MASTER
	EXCCHECK( PMASTER->ProductMasterDelHist(PMASTER, me->_branch_id));
#endif
#ifndef _NO_TEST
	EXCCHECK( PTEST->TestDelHist(PTEST, me->_branch_id));
	EXCCHECK( PTESTPARAM->TestParamDelHist(PTESTPARAM, me->_branch_id));
#endif

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbs_CheckSchema"
static SElExceptionPtr dbs_CheckSchema(struct _SDBS* me)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL;
	SOdbcPtr		podbc = me->podbc; 
	char*			table;
	int32_t			index = 0, 
					index2 = 0;
	char			sql[SQL_COMMAND_LENGHT+1];

	/* SHOW TABLES, DESCRIBE table */

	/* get used schema */
	EXCCHECK( podbc->ExecSQL(podbc, "SHOW TABLES"));
	CHECKERR( podbc->Fetch( podbc, TRUE)); 
	
	/* tables do not exist */
	if(error==ELODBC_SQL_WARN_NO_DATA_FOUND)
		EXCTHROW(DBS_ERROR_SCHEMA_NOT_EXIST, "Database Check Error, Schema Not Exist");

	do
	{
		table = (char*)podbc->GetFieldValuePtr(podbc, 1);

		index = 0;
		while(SValeoSchemaPtr->table[index].name!=NULL)
		{
			if(0==strcmp(SValeoSchemaPtr->table[index].name, table))
			{
				SValeoSchemaPtr->table[index].check = TRUE;
				break;
			}
			index++;
		};

	}while(podbc->MoveNext(podbc)==0);

	EXCCHECK( podbc->Cancel(podbc));

	index = 0;
	while(SValeoSchemaPtr->table[index].name!=NULL)
	{
		if(SValeoSchemaPtr->table[index].check==TRUE)
		{
			char*	column;

			sprintf(sql, "DESCRIBE %s", SValeoSchemaPtr->table[index].name); 
			EXCCHECK( podbc->ExecSQL(podbc, sql));
			CHECKERR( podbc->Fetch( podbc, TRUE)); 	

			if(error==ELODBC_SQL_WARN_NO_DATA_FOUND) 
			{
				char serror[512];

				sprintf(serror, "No columns in the table: %s", SValeoSchemaPtr->table[index].name);
				EXCTHROW(DBS_ERROR_COLUMN_NOT_EXIST, serror);
			}

			do
			{
				column = (char*)podbc->GetFieldValuePtr(podbc, 1);

				index2 = 0;
				while(SValeoSchemaPtr->table[index].column[index2].name!=NULL)
				{
					if(0==strcmp(SValeoSchemaPtr->table[index].column[index2].name, column))
					{
						SValeoSchemaPtr->table[index].column[index2].check = TRUE;
						break;
					}
					index2++;
				};
			}while(podbc->MoveNext(podbc)==0);

			EXCCHECK( podbc->Cancel(podbc));
		}
		else
		{
			char serror[512];

			sprintf(serror, "Database table does not exist: %s", SValeoSchemaPtr->table[index].name);
			EXCTHROW(DBS_ERROR_TABLE_NOT_EXIST, serror);
		}

		index++;
	};

	index = 0;
	while(SValeoSchemaPtr->table[index].name!=NULL)
	{
		index2 = 0;
		while(SValeoSchemaPtr->table[index].column[index2].name!=NULL)
		{
			if(SValeoSchemaPtr->table[index].column[index2].check == FALSE)
			{
				char serror[512];

				sprintf(serror, "Database column does not exist: %s\\%s", 
								SValeoSchemaPtr->table[index].name,
								SValeoSchemaPtr->table[index].column[index2].name);
				EXCTHROW(DBS_ERROR_COLUMN_NOT_EXIST, serror);
			}
			index2++;
		};

		index++;
	};

	EXCCHECK( podbc->Cancel(podbc));

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbs_GetUser"
static SElExceptionPtr dbs_GetUser(struct _SDBS* me, char* user)
{
	int32_t			error = 0;
	SElExceptionPtr	pexception = NULL;
	SOdbcPtr		podbc = me->podbc; 
	
	/* get user */
	EXCCHECK( podbc->ExecSQL(podbc, "SELECT USER()"));
	CHECKERR( podbc->Fetch( podbc, TRUE)); 
	strcpy(user, (char*)podbc->GetFieldValuePtr(podbc, 1));  
	EXCCHECK( podbc->Cancel(podbc));

Error:
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbs_UsedMemory"
static SElExceptionPtr dbs_UsedMemory(struct _SDBS* me, uint32_t* size)
{
	SElExceptionPtr	pexception = NULL;

	if(size)
		*size = PPRODUCT->ProductSize*sizeof(SProductTypes)
#ifndef _NO_MASTER
				+ PMASTER->_Allocated*sizeof(SProductMaster) 
#endif
			    + PPARAM->_Allocated*sizeof(SProductParameter) 
#ifndef _NO_TEST
				+ PTEST->_Allocated*sizeof(STestStep) 
				+ PTESTPARAM->_Allocated*sizeof(STestParameter)
#endif
				;
			
/* Error: */
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbs_Free"
static SElExceptionPtr dbs_Free(void* pInstance)
{
	SElExceptionPtr	pexception = NULL;

	if(pInstance)
	{
		free(pInstance);
		pInstance = NULL;
	}

/* Error: */
	EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbs_CharCopy"
static char* dbs_CharCopy(char* value)	
{
	if(value)
	{
		char* 	pdata = NULL;
		pdata =  calloc(strlen(value)+1, sizeof(char));
		strcpy(pdata, value);
		return pdata;
	}
	
	return NULL; 
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "dbs_CharAlloc"
static char* dbs_CharAlloc(char** value, size_t size)	
{
	if(value)
	{
		*value = realloc(*value, (size+1)*sizeof(char)); 
		memset(*value, 0, (size+1)*sizeof(char));
		
		return *value;
	}
	else
	{
		return NULL;
	}
}
