//---------------------------------------------------------------------------------------------------------------------------
/// @file		spl_pluginserver_win32.cpp
/// @version	1.0
/// @author		Andreas Loeffler
//---------------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <eltypes.h>
#include <elerror.h>
#define _WIN32_WINNT 0x0400
#include <windows.h>
#include <string.h>
//#include <tchar.h>
#include "el_pluginserver.h"

#define LOG(message) \
	if(pServer->_LogFnc) \
		EXCCHECK(pServer->_LogFnc(message))     

typedef struct _SPluginEntry{
	HANDLE			handle;
	char			name[64];
	SPluginInfoPtr	pInfo;
	
	PLUGIN_INIT_FUNC		pInitFce;
	PLUGIN_RUN_FUNC			pRunFce;
	PLUGIN_LEAVE_FUNC		pLeaveFce;
	PLUGIN_SHUTDOWN_FUNC	pShutdownFce;
	PLUGIN_INFO_FUNC		pInfoFce;
} SPluginEntry, *SPluginEntryPtr;

static SElExceptionPtr PluginServerLoadAllPlugins(
  struct _SPluginServer* pServer,
  const char* strPath,
  const char* strWildcard, 
  SPluginArgs* pPluginArgs
);
static SElExceptionPtr PluginServerLoadPlugin(
  struct _SPluginServer* pServer,
  const char* strFileName,
  SPluginArgs* pPluginArgs
);
static SElExceptionPtr PluginServerGetPluginInfo( 
  struct _SPluginServer* pServer,
  uint32_t Index,
  SPluginInfoPtr* pInfo
);
static SElExceptionPtr PluginServerUnloadPlugin(
	struct _SPluginServer *pServer,
	uint32_t		index,
	SPluginArgsPtr	pPluginArgs
);
static SElExceptionPtr PluginServerUnloadAllPlugins(
	struct _SPluginServer*	pServer,
	SPluginArgsPtr	pPluginArgs
);
static SElExceptionPtr PluginServerInitializePlugin( 
	struct _SPluginServer *me, 
	uint32_t Index, 
	SPluginArgs* pPluginArgs 
);
static SElExceptionPtr PluginServerInitializeAllPlugins( 
	struct _SPluginServer *me, 
	SPluginArgs* pPluginArgs 
);
static SElExceptionPtr PluginServerRunPlugin(
	struct _SPluginServer *pServer,
	uint32_t index,
	SPluginArgsPtr pPluginArgs,
	const char* uuid
);
static SElExceptionPtr PluginServerRunAllPlugins(
	struct _SPluginServer *pServer,
	SPluginArgsPtr	pPluginArgs
);
static SElExceptionPtr PluginServerLeavePlugin(
	struct _SPluginServer *pServer,
	uint32_t index,
	SPluginArgsPtr pPluginArgs,
	const char* uuid
);
static SElExceptionPtr PluginServerLeaveAllPlugins(
	struct _SPluginServer *pServer,
	SPluginArgsPtr	pPluginArgs
);
static SElExceptionPtr PluginServerRunSelectedPlugin( 
	struct _SPluginServer *me, 
	const char* uuid, 
	SPluginArgs* pPluginArgs 
);
static SElExceptionPtr PluginServerLeaveSelectedPlugin( 
	struct _SPluginServer *me, 
	const char* uuid, 
	SPluginArgs* pPluginArgs 
);
#if 0  
static int32_t PluginServerSetUserData(
	struct _SPluginServer *pServer,
	uint32_t index,
	void*    pUserData
);
static int32_t PluginServerGetUserData(
	struct _SPluginServer *pServer,
	uint32_t index,
	void**   pUserData
);
#endif
static uint32_t PluginServerGetCount(
	struct _SPluginServer *pServer
);

static int ComparePluginsDependency(const void *element1, const void *element2);
static SElExceptionPtr PluginSetLogFnc( struct _SPluginServer* me, SElExceptionPtr (*_LogFnc)(const char* message));
static SElExceptionPtr AddPluginHandle(struct _SPluginServer* pServer, HANDLE h);
static SElExceptionPtr RemovePluginHandle(struct _SPluginServer* pServer, uint32_t index);

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PluginServer_new"
SElExceptionPtr PluginServer_new( SPluginServerPtr* pPluginServer )
{
	SElExceptionPtr	 pexception = 0;

	*pPluginServer = calloc(1, sizeof( SPluginServer));
	EXCCHECKALLOC( *pPluginServer);

	(*pPluginServer)->LoadPlugin     = PluginServerLoadPlugin;
	(*pPluginServer)->LoadAllPlugins = PluginServerLoadAllPlugins;
	(*pPluginServer)->GetPluginInfo  = PluginServerGetPluginInfo;

	(*pPluginServer)->UnloadPlugin   = PluginServerUnloadPlugin;
	(*pPluginServer)->UnloadAllPlugins= PluginServerUnloadAllPlugins;

	(*pPluginServer)->InitializePlugin	= PluginServerInitializePlugin;
	(*pPluginServer)->InitializeAllPlugins = PluginServerInitializeAllPlugins;
	(*pPluginServer)->RunPlugin			= PluginServerRunPlugin;
	(*pPluginServer)->RunAllPlugins		= PluginServerRunAllPlugins;
	(*pPluginServer)->LeavePlugin		= PluginServerLeavePlugin;
	(*pPluginServer)->LeaveAllPlugins	= PluginServerLeaveAllPlugins;
	(*pPluginServer)->RunSelectedPlugin = PluginServerRunSelectedPlugin;
	(*pPluginServer)->LeaveSelectedPlugin = PluginServerLeaveSelectedPlugin;
	
#if 0 
	(*pPluginServer)->SetUserData    = PluginServerSetUserData;
	(*pPluginServer)->GetUserData    = PluginServerGetUserData;
#endif
	(*pPluginServer)->GetCount	    = PluginServerGetCount;
	(*pPluginServer)->SetLogFnc 	= PluginSetLogFnc;


Error:
	EXCRETHROW(pexception);
} /* PluginServer_new */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PluginServer_delete"
SElExceptionPtr PluginServer_delete( SPluginServerPtr* pServer )
{
	SElExceptionPtr	 pexception = 0;
	if (pServer && *pServer)
	{
		EXCCHECK( (*pServer)->UnloadAllPlugins(*pServer, NULL));
		free((*pServer)->_pIds);
		free((*pServer)->_pUserData);
		free(*pServer);
		*pServer = NULL;
	}

Error:
	EXCRETHROW(pexception);
} /* PluginServer_delete */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PluginServerLoadAllPlugins"
static SElExceptionPtr PluginServerLoadAllPlugins(
  struct _SPluginServer* pServer,
  const char*		         strPath,
  const char*		         strWildcard, 
  SPluginArgs*     			 pPluginArgs
)
{
#define CHANGE_CURRENT_DIRECTORY	0
#if !CHANGE_CURRENT_DIRECTORY
	int				i;
	bool			strPath_with_dirsep;
#endif
	SElExceptionPtr	pexception = 0;
	int32_t			error = 0;
	char			buffer[1024] = "";
	WIN32_FIND_DATA FindData;
	const char*     str_wildcard = strWildcard;
	HANDLE 			hDirectory;

	if( str_wildcard == NULL || strlen(str_wildcard)==0 )
		str_wildcard = "*.dll";

#if CHANGE_CURRENT_DIRECTORY
	GetCurrentDirectory( sizeof(buffer), buffer);
	SetCurrentDirectory( strPath );
	
#if 0
	{
	char			buffer[1024] = "";
	GetCurrentDirectory( sizeof(buffer), buffer);
	OutputDebugString(buffer);
	}
#endif

	hDirectory = FindFirstFileEx( str_wildcard,
                                  FindExInfoStandard,
                                  &FindData,
                                  FindExSearchNameMatch,
                                  NULL, 0 );

#else

	//i = (int) _tcslen(strPath) - 1;
	i = (int) strlen(strPath) - 1;
	strPath_with_dirsep = (bool) (strPath[i] == '/' || strPath[i] == '\\');
	strcpy(buffer,strPath);
	if ( !strPath_with_dirsep )
		strcat(buffer,"/");
	strcat(buffer,str_wildcard);
	hDirectory = FindFirstFileEx( buffer,
                                  FindExInfoStandard,
                                  &FindData,
                                  FindExSearchNameMatch,
                                  NULL, 0 );
	
#endif

	if( hDirectory != INVALID_HANDLE_VALUE )
	{
		do
    	{
			if( ( strcmp( FindData.cFileName, "." ) != 0 ) &&
				( strcmp( FindData.cFileName, ".." ) != 0 ) )
			{
#if CHANGE_CURRENT_DIRECTORY
				EXCCHECK( PluginServerLoadPlugin(pServer,FindData.cFileName,pPluginArgs) );
#else
				strcpy(buffer,strPath);
				if ( !strPath_with_dirsep )
					strcat(buffer,"/");
				strcat(buffer,FindData.cFileName);
				EXCCHECK( PluginServerLoadPlugin(pServer,buffer,pPluginArgs) );
#endif
			}	
		} while( FindNextFile( hDirectory, &FindData ) );
		
		EXCCHECKWIN( FindClose(hDirectory));
	}
	
Error:
#if CHANGE_CURRENT_DIRECTORY
	SetCurrentDirectory( buffer );
#endif
	EXCRETHROW(pexception);
#undef	CHANGE_CURRENT_DIRECTORY
} /* PluginServerLoadAllPlugins */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PluginServerLoadPlugin"
static SElExceptionPtr		PluginServerLoadPlugin(
  struct _SPluginServer*	pServer,
  const char*            	strFileName,
  SPluginArgs*           	pPluginArgs
)
{
	int32_t				error = 0;
	SElExceptionPtr	 	pexception = 0;
	HMODULE				hModule = NULL;
	PLUGIN_INFO_FUNC	pInfoFunc = NULL;
	SPluginInfoPtr		pInfo = NULL;
	char				message[512],
						name[256];
	
	if( strFileName == NULL || strlen(strFileName)== 0 )
	{
		EXCTHROW( PLUGIN_ERR_FILE_NAME_MISSING, "Missing file name");
	}

	hModule = LoadLibrary( strFileName );

	if(hModule)
	{
		pInfoFunc = (PLUGIN_INFO_FUNC)GetProcAddress( (HINSTANCE)hModule, PLUGIN_GETINFO_NAME);
		if(pInfoFunc)
		{
			EXCCHECK( AddPluginHandle(pServer, hModule));
			
			/* plugin info */
			EXCCHECK( pServer->GetPluginInfo( pServer, pServer->_UsedCount-1, &pInfo));

			EXCCHECK( pInfo->GetName(pInfo, name, 255));
			sprintf(message, "Load Plugin: %s", name);
			LOG( message);
		}
		else /* unload */
		{
			EXCCHECKWIN( FreeLibrary( hModule));
			hModule = NULL;
		}
	}
	else
	{
		char str[256+1];
		
		sprintf(str,"Load Library Fault: %s",strFileName);
		EXCTHROW(EL_ERROR_OS_SPECIFIC, str);
	}

Error:
	EXCRETHROW(pexception);
} /* PluginServerLoadPlugin */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PluginServerGetPluginInfo"
static SElExceptionPtr PluginServerGetPluginInfo( 
  struct _SPluginServer* pServer,
  uint32_t Index,
  SPluginInfoPtr* pInfo
)
{
	SElExceptionPtr	pexception = 0;
	SPluginEntryPtr	pitem  = (SPluginEntryPtr)pServer->_pIds;

	if( Index >= pServer->_UsedCount )
	{
		EXCTHROW( PLUGIN_ERR_INDEX_OUT_OF_RANGE, "Index is out of range");
	}
 
	pitem += Index;

	if( pitem->pInfo == NULL )
	{
		SPluginInfoPtr	pinfo = NULL;

		if ( NULL == pitem->pInfoFce )
		{
			pitem->pInfoFce = (PLUGIN_INFO_FUNC)GetProcAddress( pitem->handle, PLUGIN_GETINFO_NAME);
		}
		if( NULL == pitem->pInfoFce )
		{
			EXCTHROW( PLUGIN_ERR_MISSING_GETINFO_FUNCTION, "GetInfo function is missing");
		}

		EXCCHECK( PluginInfo_new( NULL, pInfo)); 
		
		EXCCHECK( pitem->pInfoFce( pInfo));
		
		pitem->pInfo = *pInfo;
	}
	else
	{
		*pInfo = pitem->pInfo;
	}
	
Error:
	EXCRETHROW(pexception);
} /* PluginServerGetPluginInfo */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PluginServerUnloadPlugin"
static SElExceptionPtr PluginServerUnloadPlugin(
	struct _SPluginServer *pServer,
	uint32_t		index,
	SPluginArgsPtr	pPluginArgs
)
{
	int32_t				error = 0;
	SElExceptionPtr	 	pexception = 0; 
	SPluginEntryPtr		pitem  = (SPluginEntryPtr)pServer->_pIds;
	char				message[512],
						name[256];

	if( index >= pServer->_UsedCount )
	{
		EXCTHROW( PLUGIN_ERR_INDEX_OUT_OF_RANGE, __FUNC__": Index is out of range");
	}

	pitem += index;
	
	if( pitem->handle )
	{
		EXCCHECK( pitem->pInfo->GetName(pitem->pInfo, name, 256));
		sprintf(message, "Unload Plugin: %s", name);
		LOG( message);

		/* free plugin info */
		EXCCHECK( PluginInfo_delete( &(pitem->pInfo))); 
		/* UNLOAD LIBRARY */
		EXCCHECKWIN( FreeLibrary( pitem->handle));
		pitem->handle = NULL;
		/* REMOVE LIBRARY FROM ID-MAP AND INFO-MAP */
		EXCCHECK( RemovePluginHandle(pServer, index));
	}

Error:
	EXCRETHROW(pexception);
} /* PluginServerUnloadPlugin */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PluginServerUnloadAllPlugins"
static SElExceptionPtr PluginServerUnloadAllPlugins(
	struct _SPluginServer*	pServer,
	SPluginArgsPtr	pPluginArgs
)
{
	SElExceptionPtr	pexception = 0; 
	int32_t			l;
	uint32_t		lCount = pServer->_UsedCount;
	
	for(l=lCount-1; l>=0;  l--)
	{
		EXCCHECK( pServer->UnloadPlugin( pServer, l, pPluginArgs));
	}

Error:
	EXCRETHROW(pexception);
} /* PluginServerUnloadAllPlugins */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PluginServerInitializePlugin"
static SElExceptionPtr PluginServerInitializePlugin(
	struct _SPluginServer *pServer,
	uint32_t		index,
	SPluginArgsPtr	pPluginArgs
)
{
	SElExceptionPtr	pexception = 0; 
	SPluginEntryPtr	pitem  = (SPluginEntryPtr)pServer->_pIds;

	if( index >= pServer->_UsedCount )
	{
		EXCTHROW( PLUGIN_ERR_INDEX_OUT_OF_RANGE, "Index is out of range");
	}

	pitem += index;
	if( pitem->handle != NULL )
	{
		if ( pitem->pInitFce == NULL )
		{
			pitem->pInitFce = (PLUGIN_INIT_FUNC)GetProcAddress( pitem->handle, PLUGIN_INIT_NAME);
		}
		if( NULL != pitem->pInitFce )
		{
			EXCCHECK( pitem->pInitFce( pPluginArgs));
		}
	}

Error:
	EXCRETHROW(pexception);
} /* PluginServerInitializePlugin */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "sort_tree_dependency"
static SElExceptionPtr sort_tree_dependency(
	SPluginEntryPtr pitem,
	uint32_t		size
)
{
	SElExceptionPtr	pexception = 0;
	uint32_t		i = 0, j;
	SPluginEntry	item = {0};

	/* sort plugins */
	qsort(pitem, size, sizeof(SPluginEntry), ComparePluginsDependency);

	while(i<size)
	{
		if((pitem+i)->pInfo->_pInfoData->target[0]!='\0')
		{
			for(j=0; j<size; j++)
			{
				if(0==strcmp((pitem+j)->pInfo->_pInfoData->uuid, (pitem+i)->pInfo->_pInfoData->target))
					break;
			}

			if(j==size)
			{
				char serror[256];

				sprintf(serror, "Target plugin does not exist: %s", (pitem+i)->pInfo->_pInfoData->target);
				EXCTHROW(-1, serror);
			}
			else if(i<j)
			{
				memcpy(&item, (pitem+i), sizeof(SPluginEntry));
				memcpy((pitem+i), (pitem+j), sizeof(SPluginEntry));
				memcpy((pitem+j), &item, sizeof(SPluginEntry));
				i=0;
			}
		}
		i++;
	}

Error:
	EXCRETHROW(pexception);
}
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PluginServerInitializeAllPlugins"
static SElExceptionPtr PluginServerInitializeAllPlugins(
	struct _SPluginServer *pServer,
	SPluginArgsPtr	pPluginArgs
)
{
	SElExceptionPtr	pexception = 0;
	uint32_t		l, lCount = pServer->_UsedCount;

	LOG( "Plugins Initialize");

	EXCCHECK( sort_tree_dependency((SPluginEntryPtr)pServer->_pIds, lCount));

	for(l=0; l<lCount; l++)
	{
		EXCDISPLAY( pServer->InitializePlugin( pServer, l, pPluginArgs));
	}

Error:
	EXCRETHROW(pexception);
} /* PluginServerInitializeAllPlugins */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PluginServerRunPlugin"
static SElExceptionPtr PluginServerRunPlugin(
	struct _SPluginServer *pServer,
	uint32_t		index,
	SPluginArgsPtr	pPluginArgs,
	const char* uuid
)
{
	SElExceptionPtr	pexception = 0; 
	SPluginEntryPtr	pitem  = (SPluginEntryPtr)pServer->_pIds;

	if( index >= pServer->_UsedCount )
	{
		EXCTHROW( PLUGIN_ERR_INDEX_OUT_OF_RANGE, "Index is out of range" );
	}

	pitem += index;
	if( pitem->handle != NULL )
	{
		if ( pitem->pRunFce == NULL )
		{
			pitem->pRunFce =( PLUGIN_RUN_FUNC)GetProcAddress(pitem->handle, PLUGIN_RUN_NAME);
		}
		if( NULL != pitem->pRunFce )
		{
			EXCCHECK( pitem->pRunFce( pPluginArgs, uuid));
		}
	}

Error:
	EXCRETHROW(pexception);
} /* PluginServerRunPlugin */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PluginServerRunAllPlugins"
static SElExceptionPtr PluginServerRunAllPlugins(
	struct _SPluginServer *pServer,
	SPluginArgsPtr	pPluginArgs
)
{
	SElExceptionPtr	pexception = 0;
	uint32_t		l, lCount = pServer->_UsedCount;
	
	LOG( "Plugins Run");

	for( l = 0; l < lCount;  l++ )
	{
		EXCDISPLAY( pServer->RunPlugin( pServer, l, pPluginArgs, ""));
	}

Error:
	EXCRETHROW(pexception);
} /* PluginServerRunAllPlugins */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PluginServerLeavePlugin"
static SElExceptionPtr PluginServerLeavePlugin(
	struct _SPluginServer *pServer,
	uint32_t		index,
	SPluginArgsPtr	pPluginArgs,
	const char* uuid
)
{
	SElExceptionPtr	pexception = 0; 
	SPluginEntryPtr	pitem  = (SPluginEntryPtr)pServer->_pIds;

	if( index >= pServer->_UsedCount )
	{
		EXCTHROW( PLUGIN_ERR_INDEX_OUT_OF_RANGE, "Index is out of range" );
	}

	pitem += index;
	if( pitem->handle != NULL )
	{
		if ( pitem->pLeaveFce == NULL )
		{
			pitem->pLeaveFce =(PLUGIN_LEAVE_FUNC)GetProcAddress( pitem->handle, PLUGIN_LEAVE_NAME);
		}
		if( NULL != pitem->pLeaveFce )
		{
			EXCCHECK( pitem->pLeaveFce( pPluginArgs, uuid));
		}
	}

Error:
	EXCRETHROW(pexception);
} /* PluginServerLeavePlugin */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PluginServerLeaveAllPlugins"
static SElExceptionPtr PluginServerLeaveAllPlugins(
	struct _SPluginServer *pServer,
	SPluginArgsPtr	pPluginArgs
)
{
	SElExceptionPtr	pexception = 0;
	int32_t			l; 
	uint32_t		lCount = pServer->_UsedCount;
	
	for(l=lCount-1; l>=0; l--)
	{
		EXCDISPLAY( pServer->LeavePlugin( pServer, l, pPluginArgs, ""));
	}

Error:
	EXCRETHROW(pexception);
} /* PluginServerLeaveAllPlugins */

typedef struct _SDependency
{
	const char*		uuid;
	const char*		name;
	uint32_t		pos;
} SDependency, *SDependencyPtr;

#define MAX_DEPENDENCY_NB		64

static SDependency	gs_SDep[MAX_DEPENDENCY_NB];
static int32_t		gs_SDepNb = 0;

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PluginServerRunSelectedPlugin"
static SElExceptionPtr PluginServerRunSelectedPlugin( 
	struct _SPluginServer *pServer, 
	const char* uuid, 
	SPluginArgs* pPluginArgs 
)
{
	SElExceptionPtr	pexception = 0;
	uint32_t		l, lCount = pServer->_UsedCount;
	SPluginEntryPtr	pitem  = (SPluginEntryPtr)pServer->_pIds;	
	int32_t			SDepIndex = 0;
	int32_t			index = 0;

	/* leave old plugin if it is not parent plugin*/
	for(l=0; l<(uint32_t)gs_SDepNb; l++)
	{
		if( 0==strcmp(uuid, gs_SDep[l].uuid))
		{
			break;
		}
	}
	if(l>0 && l==gs_SDepNb)
	{
		EXCCHECK( pServer->LeaveSelectedPlugin(pServer, gs_SDep[0].uuid, pPluginArgs));
	}

	/* search new plugin */
	for(l=0; l<lCount; l++, pitem++)
	{
		if( 0==strcmp(pitem->pInfo->_pInfoData->uuid, uuid))
		{
			break;
		}
	}

	/* search dependencies */
	if( l==lCount)
	{
		EXCTHROW( -1, "Plugin UUID does not found!");
	}
	else
	{
		memset(gs_SDep, 0, sizeof(SDependency)*MAX_DEPENDENCY_NB); 
		gs_SDep[SDepIndex].pos = l;
		gs_SDep[SDepIndex].uuid = pitem->pInfo->_pInfoData->uuid;
		gs_SDep[SDepIndex].name = pitem->pInfo->_pInfoData->Name;
		SDepIndex++;

		do
		{
			pitem  = (SPluginEntryPtr)pServer->_pIds;

			for(l=0; l<lCount; l++, pitem++)
			{
				if( 0==strcmp(pitem->pInfo->_pInfoData->target, gs_SDep[index].uuid))
				{
					if(SDepIndex >= MAX_DEPENDENCY_NB)
						break;

					gs_SDep[SDepIndex].pos = l;
					gs_SDep[SDepIndex].uuid = pitem->pInfo->_pInfoData->uuid;
					gs_SDep[SDepIndex].name = pitem->pInfo->_pInfoData->Name;
					SDepIndex++;
				}
			}
			index++;

		}while( gs_SDep[index].uuid!=NULL);

		/* run founded dependencies */
		for(l=0; l<(uint32_t)SDepIndex; l++)
		{
			EXCDISPLAY( pServer->RunPlugin( pServer, gs_SDep[l].pos, pPluginArgs, uuid));
		}
		gs_SDepNb = SDepIndex;
	}

Error:
	EXCRETHROW(pexception);
} /* PluginServerRunSelectedPlugin */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PluginServerLeaveSelectedPlugin"
static SElExceptionPtr PluginServerLeaveSelectedPlugin( 
	struct _SPluginServer *pServer, 
	const char* uuid, 
	SPluginArgs* pPluginArgs 
)
{
	SElExceptionPtr	pexception = 0;
	int32_t			l; 
	uint32_t		lCount = pServer->_UsedCount;
	SPluginEntryPtr	pitem  = (SPluginEntryPtr)pServer->_pIds;
	int32_t			SDepIndex = 0;
	int32_t			index = 0;
	
	for(l=0; l<(int32_t)lCount; l++, pitem++)
	{
		if( 0==strcmp(pitem->pInfo->_pInfoData->uuid, uuid))
		{
			break;
		}
	}

	/* leave dependencies */
	if( l==lCount)
	{
		EXCTHROW( -1, "Plugin UUID does not found!");
	}
	else
	{
		memset(gs_SDep, 0, sizeof(SDependency)*MAX_DEPENDENCY_NB); 
		gs_SDep[SDepIndex].pos = l;
		gs_SDep[SDepIndex].uuid = uuid;
		gs_SDep[SDepIndex].name = pitem->pInfo->_pInfoData->Name;
		SDepIndex++;

		do
		{
			pitem  = (SPluginEntryPtr)pServer->_pIds;

			for(l=0; l<(int32_t)lCount; l++, pitem++)
			{
				if( 0==strcmp(pitem->pInfo->_pInfoData->target, gs_SDep[index].uuid))
				{
					if(SDepIndex >= MAX_DEPENDENCY_NB)
						break;

					gs_SDep[SDepIndex].pos = l;
					gs_SDep[SDepIndex].uuid = pitem->pInfo->_pInfoData->uuid;
					gs_SDep[SDepIndex].name = pitem->pInfo->_pInfoData->Name;
					SDepIndex++;
				}
			}
			index++;

		}while( gs_SDep[index].uuid!=NULL);

		/* leave founded dependencies */
		for(l=SDepIndex-1; l>=0; l--)
		{
			EXCDISPLAY( pServer->LeavePlugin( pServer, gs_SDep[l].pos, pPluginArgs, uuid));
		}
		gs_SDepNb = 0;
	}

Error:
	EXCRETHROW(pexception);
} /* PluginServerLeaveSelectedPlugin */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PluginServerSetUserData"
static SElExceptionPtr PluginServerSetUserData(
	struct _SPluginServer *pServer,
	uint32_t index,
	void*    pUserData
)
{
	SElExceptionPtr	pexception = 0;
	
	if( index >= pServer->_UsedCount )
	{
		EXCTHROW( PLUGIN_ERR_INDEX_OUT_OF_RANGE, "Index is out of range");
	}
	
	pServer->_pUserData[index] = pUserData;
	
Error:
	EXCRETHROW(pexception);
} /* PluginServerSetUserData */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PluginServerGetUserData"
static SElExceptionPtr PluginServerGetUserData(
	struct _SPluginServer *pServer,
	uint32_t index,
	void**   pUserData
)
{
	SElExceptionPtr	 	pexception = 0;
	
	if( index >= pServer->_UsedCount )
	{
		EXCTHROW( PLUGIN_ERR_INDEX_OUT_OF_RANGE, "Index is out of range");
	}
	
	*pUserData = pServer->_pUserData[index];

Error:
	EXCRETHROW(pexception);
} /* PluginServerGetUserData */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PluginServerGetCount"
static uint32_t PluginServerGetCount(
	struct _SPluginServer *pServer
)
{
	return pServer->_UsedCount;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PluginSetLogFnc"
static SElExceptionPtr PluginSetLogFnc( 
	struct _SPluginServer* me, 
	SElExceptionPtr (*_LogFnc)(const char* message)
)
{
	SElExceptionPtr	 	pexception = NULL;
	
	me->_LogFnc = _LogFnc;
	
Error:
	EXCRETHROW(pexception);	
}

#if 0
/*---------------------------------------------------------------------------*/
long slcPluginServer::Refresh( const string&	    a_strPath, 
							   const string&	    a_strWildcard, 
							   const slcPluginArgs* a_pPluginShutdownArgs, 
							   const slcPluginArgs*	a_pPluginInitArgs )
{
	this->UnloadAllPlugins( a_pPluginShutdownArgs );

	//
	// Lock this object for exclusive use.
	// Explicitly usage.
	//
	slcThreadLock threadLock( this );

	threadLock.Lock();
	this->Clear();
	threadLock.Unlock();

	return( this->LoadAllPlugins( a_strPath, a_strWildcard, a_pPluginInitArgs ) ); 
}
#endif

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "AddPluginHandle"
static SElExceptionPtr AddPluginHandle(struct _SPluginServer* pServer, HANDLE h)
{
	SElExceptionPtr	pexception = 0;
	SPluginEntryPtr	pitem = NULL;

	if(pServer->_UsedCount == pServer->_AllocCount)
	{
		pServer->_pIds = realloc(pServer->_pIds, (pServer->_AllocCount + 5) * sizeof(SPluginEntry));
		EXCCHECKALLOC(pServer->_pIds);
		pitem = ((SPluginEntryPtr)pServer->_pIds);
		
		pServer->_pUserData = realloc(pServer->_pUserData, (pServer->_AllocCount + 5) * sizeof(UserData_t));
		EXCCHECKALLOC(pServer->_pUserData);

		memset( pitem + pServer->_AllocCount, 0, 5 * sizeof(SPluginEntry));
		memset( pServer->_pUserData + pServer->_AllocCount, 0, 5 * sizeof(UserData_t));
		pServer->_AllocCount += 5;
	}
	else
	{
		pitem = ((SPluginEntryPtr)pServer->_pIds); 
	}

	pitem += pServer->_UsedCount;
	pitem->handle = h;
	pServer->_UsedCount += 1;

Error:
	EXCRETHROW(pexception);
} /* AddPluginHandle */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "RemovePluginHandle"
static SElExceptionPtr RemovePluginHandle(struct _SPluginServer* pServer, uint32_t index)
{
	SElExceptionPtr	pexception = 0;
	uint32_t		count = pServer->_UsedCount;

	if(index < count)
	{
		SPluginEntryPtr		pitem  = (SPluginEntryPtr)pServer->_pIds;

		pitem += index;

		pServer->_UsedCount -= 1;

		if( index < (count-1))
		{
			memmove( pitem, pitem+1, (count-index)*sizeof(SPluginEntry)); 
			memmove( pServer->_pUserData+index, pServer->_pUserData+index+1, (count-index)*sizeof(UserData_t));
		}

		memset( pServer->_pUserData + pServer->_UsedCount, 0, sizeof(UserData_t));
		memset( ((SPluginEntryPtr)pServer->_pIds) + pServer->_UsedCount, 0, sizeof(SPluginEntry));
	}

Error:
	EXCRETHROW(pexception);
} /* RemovePluginHandle */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "ComparePluginsDependency"
static int ComparePluginsDependency(
	const void *element1,
	const void *element2
)
{
	const char*	pitem1 = ((SPluginEntryPtr)element1)->pInfo->_pInfoData->target;
	const char*	pitem2 = ((SPluginEntryPtr)element2)->pInfo->_pInfoData->target;
	
	if ( pitem1[0] == '\0' && pitem2[0] == '\0' )
	{
		return 0;	
	}
	else
	if ( pitem1[0] == '\0' && pitem2[0] != '\0' )
	{
		return -1;
	}
	else
	if ( pitem1[0] != '\0' && pitem2[0] == '\0' )
	{
		return 1;
	}
	else
	{
		if ( 0==strcmp( pitem2, ((SPluginEntryPtr)element1)->pInfo->_pInfoData->uuid))
		{
			return -1;
		}
		else if ( 0==strcmp( pitem1, ((SPluginEntryPtr)element2)->pInfo->_pInfoData->uuid))
		{
			return 1;
		}
	}

	return 0;
} /* ComparePluginsDependency */
