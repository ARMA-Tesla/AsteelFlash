/****************************************************************************
 *                          M O D U L E
 *---------------------------------------------------------------------------
 *    Copyright   2005-2006 ELCOM, a.s.  All Rights Reserved.        
 *---------------------------------------------------------------------------
 *                                                                          
 * Title:       elmodule.h                                                    
 * Purpose:     ELCOM MODULE interface
 ****************************************************************************/

#include <modules/elmodule.h>
#include <modules/module.h>
#include <elstring.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <elBase.h>
#include <elLog.h>

#define     MODULE_DEFAULT_REQUIRE_MODULE_LENGTH        1024UL

typedef struct _SModuleItem
{
    SELStringPtr    FullPath;
    char            moduleID[MODULE_MAX_ID_LENGTH+1];
    char            moduleVersion[MODULE_MAX_VERSION_LENGTH+1];
	char			moduleName[MODULE_MAX_NAME_LENGTH+1];
	char			moduleDescription[MODULE_MAX_DESCRIPTION_LENGTH+1];
	char			moduleBuildDatetime[MODULE_MAX_BUILD_DATETIME_LENGTH+1];
    SELStringPtr    RequiredModules;
    bool_t          isOK;
	HMODULE         hmodule;
	void*			pInstance;
} SModuleItem, *SModuleItemPtr;

typedef	struct _SModuleVersion {
	int a;
	int b;
	int c;
	int d;
} SModuleVersion, *SModuleVersionPtr;

#define MODULE_VERSION_REQUIRED_EQUAL 		(1UL << 1)
#define MODULE_VERSION_REQUIRED_LESS 		(1UL << 2)
#define MODULE_VERSION_REQUIRED_GREATER		(1UL << 3)


typedef SElExceptionPtr	(_FUNCC *GetModuleAttributeFcePtr_t)( uint32_t  attrID, uint32_t size, void* value);
typedef SElExceptionPtr	(_FUNCC *LoadElcomModuleFcePtr_t)( const char* configuration, void** pInstance, SPluginArgsPtr	pParam);
typedef SElExceptionPtr	(_FUNCC *InitializeElcomModuleFcePtr_t)( void* pInstance, const char* configuration, SPluginArgsPtr	pParam);
typedef SElExceptionPtr	(_FUNCC *UnloadElcomModuleFcePtr_t)( void* pInstance);


static SElExceptionPtr (*_LogFnc)(const char* message);

#define LOG(message) \
	if(_LogFnc) \
		EXCCHECK(_LogFnc(message))     

static SModuleItemPtr  gs_pModules = NULL;
static uint32_t		   gs_ModuleCount = 0;
/*****************************************************************************
 * Static not-exported functions declaration
 ******************************************************************************/ 
static SElExceptionPtr GetNumberOfModules(
    const char* modulePath,
    uint32_t* count
);
static SElExceptionPtr LoadVersionOfModules(
    const char*     modulePath,
    uint32_t        allocCount,
    SModuleItemPtr  pModuleItems,
    uint32_t*       count
);
static SElExceptionPtr ClenupVersionModule(
    SModuleItemPtr  pModuleItems,
    uint32_t        count
);
int CompareVersionOfModules(const void *element1, const void *element2);
static SElExceptionPtr CheckModuleValidity(
    SModuleItemPtr  pModuleItems,
    uint32_t        count
);
static SElExceptionPtr CompareVersions(
	const char*	currentVersion,
	const char*	requiredVersion,
	bool_t*		isValid
);
static SElExceptionPtr CompareTwoVersions(
	uint32_t	compareFunction,
	SModuleVersionPtr	pVersion1,
	SModuleVersionPtr	pVersion2,
	bool_t*				pIsValid
);
static SElExceptionPtr ParseVersions(
	const char*	versionString,
	SModuleVersionPtr	pVersion,
	uint32_t*			pCompareFunction
);
static SElExceptionPtr SplitVersionRange(
	const char*	pVersionString,
	char		version1[],
	char		version2[],
	uint32_t	bufferSize
);
static bool_t IsVersionRange(
	const char* pVersionSring
);
static SElExceptionPtr RemoveRequiredModuleFromString(
	SELStringPtr	pString,
	const char*		pdata
);
static SElExceptionPtr Sort(
	SModuleItemPtr pModules, 
	uint32_t count
);
/*****************************************************************************
 *
 ******************************************************************************/ 
#undef __FUNC__
#define __FUNC__ "LoadAllModules"
SElExceptionPtr	LoadAllModules(
	const char* modulePath,
	SObjListPtr* pModuleList,
	SPluginArgsPtr	pParam
)
{
    SElExceptionPtr pexception = NULL;
    SModuleItemPtr  pitem = NULL;
    uint32_t        count, i;
    LoadElcomModuleFcePtr_t 		LoadElcomModule = NULL;
    InitializeElcomModuleFcePtr_t   InitializeElcomModule = NULL;
	char			message[1024];
    
    EXCCHECK( GetNumberOfModules( modulePath, &count));
    if ( gs_pModules )
    {
		ClenupVersionModule( gs_pModules, gs_ModuleCount);
		objlist_delete( pModuleList);
    }
    
	if(count>0)
	{
    	EXCCHECK( objlist_new( pModuleList));
    
    	gs_pModules = calloc(count, sizeof(SModuleItem));
    	EXCCHECKALLOC( gs_pModules);
		
		EXCCHECK( LoadVersionOfModules( modulePath, count, gs_pModules, &count));
	    gs_ModuleCount = count; 

		qsort (gs_pModules, count, sizeof(SModuleItem), CompareVersionOfModules);
	    
		EXCCHECK( Sort(gs_pModules, count));

		EXCCHECK( CheckModuleValidity( gs_pModules, count));
		
		pitem = gs_pModules;
		for ( i = 0; i < count; i++, pitem++)
		{
			LoadElcomModule = (LoadElcomModuleFcePtr_t)GetProcAddress(
									pitem->hmodule,
									"LoadElcomModule");
			if ( LoadElcomModule != NULL )
			{
				sprintf(message, "Load Module: %s", pitem->moduleName);
				LOG( message);
				EXCCHECK( LoadElcomModule( NULL, &(pitem->pInstance), pParam));
			}
	        
			EXCCHECK( (*pModuleList)->Add( *pModuleList,
        									pitem->moduleID,
        									pitem->pInstance));
		}

		pitem = gs_pModules;
		for ( i = 0; i < count; i++, pitem++)
		{
			InitializeElcomModule = (InitializeElcomModuleFcePtr_t)GetProcAddress(
									pitem->hmodule,
									"InitializeElcomModule");
			if ( InitializeElcomModule != NULL )
			{
				EXCCHECK( InitializeElcomModule( pitem->pInstance, NULL, pParam));
			}
		}
	}
	else
	{
		gs_ModuleCount = 0;
		if(pModuleList) *pModuleList = NULL;
	}

Error:
    EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__ "UnLoadAllModules"
SElExceptionPtr	UnLoadAllModules(
	SObjListPtr* pModuleList
)
{
	SElExceptionPtr pexception = NULL;
	
	EXCCHECK( ClenupVersionModule( gs_pModules, gs_ModuleCount));
	EXCCHECK( objlist_delete(pModuleList));
	free(gs_pModules);
	gs_pModules = NULL;
	gs_ModuleCount = 0;
	
Error:
    EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "GetModulesCount"
int32_t GetModulesCount(
	void
)
{
	return gs_ModuleCount;
}

#undef __FUNC__
#define __FUNC__ "GetModuleInfo"
SElExceptionPtr GetModuleInfo(
	int32_t index,
	uint32_t attribute,
	uint32_t size,
	void* value	
)
{
	SElExceptionPtr pexception = NULL;
	
	if((uint32_t)index<gs_ModuleCount)
	{
		switch(attribute)
		{
			case MODULE_ATTR_VERSION_STRING:
				strncpy((char*)value, gs_pModules[index].moduleVersion, size);
				break;
			case MODULE_ATTR_MODULE_ID:
				strncpy((char*)value, gs_pModules[index].moduleID, size);
				break;
			case MODULE_ATTR_MODULE_NAME:
				strncpy((char*)value, gs_pModules[index].moduleName, size);
				break;
			case MODULE_ATTR_MODULE_DESCRIPTION:
				strncpy((char*)value, gs_pModules[index].moduleDescription, size);
				break;
			case MODULE_ATTR_REQUIRED_MODULES:
				{
					const char* pRequiredModules = NULL;
					
					if(gs_pModules[index].RequiredModules)
						pRequiredModules = gs_pModules[index].RequiredModules->GetPtr(gs_pModules[index].RequiredModules);

					strncpy((char*)value, (pRequiredModules)? pRequiredModules:"", size);
				}
				break;
			case MODULE_ATTR_BUILD_DATETIME:
				strncpy((char*)value, gs_pModules[index].moduleBuildDatetime, size);
				break;
		}
	}
	else
	{
		EXCTHROW(-1, "Not Valid Module Index!");
	}

Error:
    EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "SetLogFnc"
SElExceptionPtr SetLogFnc( 
	SElExceptionPtr (*_ImportedLogFnc)(const char* message)
)
{
	SElExceptionPtr pexception = NULL;    
	
	_LogFnc = _ImportedLogFnc;
	
Error:
    EXCRETHROW( pexception);	
}

/*****************************************************************************
 * Static not-exported functions definition
 ******************************************************************************/ 
#undef __FUNC__
#define __FUNC__ "GetNumberOfModules"
static SElExceptionPtr GetNumberOfModules(
    const char* modulePath,
    uint32_t* count
)
{
    SElExceptionPtr pexception = NULL;
    SELStringPtr    pstring    = NULL;
    size_t          length     = (modulePath) ? strlen(modulePath) : 0;
    HANDLE			hDirectory = 0;
    WIN32_FIND_DATA FindData;
    
	if(count) *count = 0;

    EXCCHECK( elstring_new(modulePath,&pstring));
    if ( length && modulePath[length-1] != '\\' )
    {
        EXCCHECK( pstring->Append(pstring, "\\"));
    }
    EXCCHECK( pstring->Append(pstring, "*.dll"));

    hDirectory = FindFirstFile( pstring->GetPtr(pstring), &FindData);
	if( hDirectory != INVALID_HANDLE_VALUE )
	{
		do
		{
			*count += 1;
		} while( FindNextFile( hDirectory, &FindData ) );   
    }
	
Error:
    elstring_delete( &pstring);
    if( hDirectory) FindClose( hDirectory );
    EXCRETHROW( pexception); 
} /* GetNumberOfModules */

#undef __FUNC__
#define __FUNC__ "LoadVersionOfModules"
static SElExceptionPtr LoadVersionOfModules(
    const char*     modulePath,
    uint32_t        allocCount,
    SModuleItemPtr  pModuleItems,
    uint32_t*       count
)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
    SModuleItemPtr  pitem = pModuleItems;
    SELStringPtr    pstring = NULL;
    size_t          length = (modulePath) ? strlen(modulePath) : 0;
    HANDLE			hDirectory = 0;
    bool_t          append_on_end = FALSE;
    WIN32_FIND_DATA FindData;
    HMODULE         hmodule = NULL;
    char            required_modules[MODULE_DEFAULT_REQUIRE_MODULE_LENGTH];
    GetModuleAttributeFcePtr_t GetModuleAttribute;
    
	// elLogD("%s(): start",__FUNC__);

	LOG( "Load Modules Libraries ...");

	// elLogD("%s(): elstring_new() ...",__FUNC__);

	EXCCHECK( elstring_new(modulePath,&pstring));
    if ( length && modulePath[length-1] != '\\' )
    {
        append_on_end = TRUE;
        EXCCHECK( pstring->Append(pstring, "\\"));
    }
    EXCCHECK( pstring->Append(pstring, "*.dll"));

	// elLogD("%s(): FindFirstFile() ...",__FUNC__);

    hDirectory = FindFirstFile( pstring->GetPtr(pstring), &FindData);
	if( hDirectory == INVALID_HANDLE_VALUE )
	{
        EXCTHROW(EL_ERROR_OS_SPECIFIC, "FindFirstFileEx failed");
    }

	// elLogD("%s(): do { ... } while",__FUNC__);

	*count = 0;
	do
	{
		HANDLE	hmodule0;
		char	module_path0[1024+1];
		char	module_path[1024+1];
		char	tmp[1024+1];
		

        EXCCHECK( elstring_new(modulePath,&(pitem->FullPath)));
        if ( append_on_end )
        {
            EXCCHECK( (pitem->FullPath)->Append(pitem->FullPath, "\\"));
        }
        EXCCHECK( (pitem->FullPath)->Append(pitem->FullPath, FindData.cFileName) );
        OutputDebugString((pitem->FullPath)->GetPtr(pitem->FullPath));

		hmodule0 = GetModuleHandle(FindData.cFileName);
		if ( hmodule0 )
			GetModuleFileName(hmodule0,module_path0,sizeof(module_path0));

		hmodule = LoadLibrary( (pitem->FullPath)->GetPtr(pitem->FullPath));
        if ( hmodule == NULL )
        {
			char message[1024];

			sprintf(message, "Load library failed: \"%s\"", FindData.cFileName); 
            EXCTHROW( EL_ERROR_OS_SPECIFIC, message);
        }
        
		if ( hmodule0 )
		{
			GetModuleFileName(hmodule,module_path,sizeof(module_path));
			if ( strcmp(module_path0,module_path) )
			{
				char	str[1024+1];

				sprintf(str,"%s: module already loaded from %s !",
					module_path,module_path0);
				EXCTHROW(-1,str);
			}
		}
		
        GetModuleAttribute = (GetModuleAttributeFcePtr_t)GetProcAddress(
                                hmodule,
                                "GetModuleAttribute");
        if ( GetModuleAttribute != NULL )
        {
			pitem->hmodule = hmodule;

            EXCCHECK( GetModuleAttribute( MODULE_ATTR_MODULE_ID,
                                          MODULE_MAX_ID_LENGTH+1,
                                          pitem->moduleID));
            EXCCHECK( GetModuleAttribute( MODULE_ATTR_VERSION_STRING,
                                          MODULE_MAX_VERSION_LENGTH+1,
                                          pitem->moduleVersion));
			EXCCHECK( GetModuleAttribute( MODULE_ATTR_MODULE_NAME,
                                          MODULE_MAX_NAME_LENGTH+1,
                                          pitem->moduleName));
			EXCCHECK( GetModuleAttribute( MODULE_ATTR_MODULE_DESCRIPTION,
                                          MODULE_MAX_DESCRIPTION_LENGTH+1,
                                          pitem->moduleDescription));
			EXCCHECK( GetModuleAttribute( MODULE_ATTR_BUILD_DATETIME,
                                          MODULE_MAX_BUILD_DATETIME_LENGTH+1,
                                          pitem->moduleBuildDatetime));	
            EXCCHECK( GetModuleAttribute( MODULE_ATTR_REQUIRED_MODULES,
                                          MODULE_DEFAULT_REQUIRE_MODULE_LENGTH,
                                          required_modules)); 

            EXCCHECK( elstring_new( required_modules, &(pitem->RequiredModules)));
           
            *count += 1;
            pitem++;        
        }
		else
		{
			EXCCHECKWIN( FreeLibrary( hmodule));
			hmodule = NULL;
			EXCCHECK( elstring_delete( &pitem->FullPath));
		}
				
        if (*count >= allocCount)
        {
            break;
        }
			
	} while( FindNextFile( hDirectory, &FindData ) );
	
Error:
	elstring_delete( &pstring);
    if ( hDirectory ) FindClose( hDirectory );
    EXCRETHROW( pexception);
} /* LoadVersionOfModules */


#undef __FUNC__
#define __FUNC__ "ClenupVersionModule"
static SElExceptionPtr ClenupVersionModule(
    SModuleItemPtr  pModuleItems,
    uint32_t        count
)
{
    SElExceptionPtr	pexception = NULL;
	int32_t			error = 0;
    SModuleItemPtr  pitem = pModuleItems;
    int32_t         i;
    UnloadElcomModuleFcePtr_t	UnloadElcomModule = NULL;
	char			message[512];
    
    for( i = (int32_t)gs_ModuleCount; --i >= 0; )
    {
        pitem = (pModuleItems + i);
        if ( pitem->hmodule )
        {
			sprintf(message, "Unload Module: %s", pitem->moduleName);
			LOG( message);
				
	        UnloadElcomModule = (UnloadElcomModuleFcePtr_t)GetProcAddress(
	                                pitem->hmodule,
	                                "UnloadElcomModule");
	        if ( UnloadElcomModule != NULL )
	        {
				EXCCHECK( UnloadElcomModule( pitem->pInstance));
				pitem->pInstance = NULL;
			}
			EXCCHECKWIN( FreeLibrary( pitem->hmodule));
			pitem->hmodule = NULL;
        }
        EXCCHECK( elstring_delete(&(pitem->FullPath)));
        EXCCHECK( elstring_delete(&(pitem->RequiredModules)));
    }
Error:
    EXCRETHROW(pexception);
}

#undef __FUNC__
#define __FUNC__ "CheckModuleValidity"
static SElExceptionPtr CheckModuleValidity(
    SModuleItemPtr  pModuleItems,
    uint32_t        count
)
{
	SElExceptionPtr	pexception = NULL;
	uint32_t			i, j	= 0;
	SModuleItemPtr  pitem  = pModuleItems;
	SModuleItemPtr  pactual = pModuleItems;
	SELStringPtr    pstring    = NULL;
	SELStringPtr    pexception_message  = NULL;
	SELStringPtr    pversion_string     = NULL;
	const char*		prequired_module_id = NULL;
	bool_t			is_version_required;
	const char* pversion = NULL;
	
	EXCCHECK( elstring_new( "", &pexception_message));
	
	for( i = 0; i < count; i++, pactual++)
	{
		if ( (pactual->RequiredModules)->Size(pactual->RequiredModules) )
		{
			EXCCHECK( elstring_new( (pactual->RequiredModules)->GetPtr(pactual->RequiredModules),
								    &pstring));
			pitem  = pModuleItems;
			for( j = 0; j < i; j++, pitem++)
			{
				prequired_module_id = strstr(pstring->GetPtr(pstring), pitem->moduleID);
				if ( prequired_module_id )
				{
					is_version_required = FALSE;
					pversion = strchr(prequired_module_id, '[');
					if ( pversion )
					{
						const char* pend = strchr(prequired_module_id, ';');
						is_version_required = TRUE;
						if ( pend && pend < pversion )
						{
							is_version_required = FALSE;
						}
					}
				/*
				 * TODO: Continue
				 */
				 	if ( is_version_required )
				 	{
						 bool_t	is_valid;
						 const char *pbegin = ++pversion;
						 const char *pend = strchr(pbegin, ']');;
						 EXCCHECK( elstring_new( pbegin, &pversion_string));
						
						 if ( pend )
						 {
						    EXCCHECK( pversion_string->Remove( pversion_string,
						    								   ELSTRING_END,
						    								   (int32_t) strlen(pend)));
						 }
						 else
						 {
						 	EXCCHECK( pversion_string->Remove(pversion_string, ELSTRING_END, 1));
						 }
				 		 EXCCHECK( CompareVersions( pitem->moduleVersion,
				 		                            pversion_string->GetPtr(pversion_string),
				 		                            &is_valid));
						 if ( is_valid == FALSE )
						 {
							EXCCHECK( pexception_message->Format(pexception_message,
																 "Module: %s\n"
																 "Required version: %s\n"
																 "Actual version: %s\n",
																 pitem->moduleID,
																 pversion_string->GetPtr(pversion_string),
																 pitem->moduleVersion
																 ));
						 
						 }
				 		 EXCCHECK( elstring_delete(&pversion_string));
				 	}
				 	
				 	EXCCHECK(RemoveRequiredModuleFromString( pstring, pitem->moduleID));
				}
			}
			if ( pstring->Size(pstring) > 0 )
			{
				EXCCHECK( pexception_message->Format(pexception_message,
													 "Module Name: %s\n"
													 "Module Id: %s\n"
													 "Required Modules: %s\n",
													 pitem->moduleName,
													 pitem->moduleID,
													 pstring->GetPtr(pstring)));
			}
			
			EXCCHECK( elstring_delete(&pstring));
		}
	}
Error:
	if ( pexception == NULL && pexception_message->Size(pexception_message) > 0 )
	{
		EXCTHROW( MODULE_ERR_MODULE_DEPENDENCY,
				  pexception_message->GetPtr( pexception_message));
	}
	elstring_delete(&pstring);
	elstring_delete(&pversion_string); 
	elstring_delete(&pexception_message);
	EXCRETHROW( pexception);
} /* CheckModuleValidity */

#undef __FUNC__
#define __FUNC__ "RemoveRequiredModuleFromString"
static SElExceptionPtr RemoveRequiredModuleFromString(
	SELStringPtr	pString,
	const char*		pdata
)
{
	SElExceptionPtr	pexception = NULL;
	const char*		pstring = pString->GetPtr(pString);
	const char*		pend	= NULL;
	uint32_t		offset  = 0;
	uint32_t		length  = 0;
	
	pend = strstr(pstring, pdata);
	if ( pend == NULL )
	{
		EXCTHROW( MODULE_ERR_MODULE_STRING_NOT_FOUND, "Module String not found.");
	}
	
	offset = (uint32_t)(pend - pstring);
	pend = strchr(pend, ';');
	length = (pend) ? (((uint32_t)(pend-pstring)-offset)+1) : (uint32_t)strlen(pstring+offset);
	EXCCHECK( pString->Remove(pString, offset,length));

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "CompareVersions"

/*!
 * @param[in] currentVersion	version string in format x.x.x.x, the first
 								number is required.
   @param[in] requiredVersion	list of required versions. the versions are
                                separated by semicolon ',' and with compare prefix <=>
                                If there is any range you must use chaacter '-'
                                example: "=1.0" exact version 1.0.0.0
                                          "=1.0,=2.0" exact version 1.0 and 2.0
                                          "1.0" all version greater as 1.0 and version 1.0
                                          ">1.0-<2.0" version greater as 1.0 and less as 2.0
*/
/* 
 * version:  3.0
 * required: >=1.0,<2.0,>=4.0 
 */
static SElExceptionPtr CompareVersions(
	const char*	currentVersion,
	const char*	requiredVersion,
	bool_t*		isValid
)
{
	SElExceptionPtr	pexception = NULL;
	SModuleVersion	v1, v2;
	const char* prequired = requiredVersion;
	bool_t		is_valid = FALSE;
	uint32_t	cmp_fce;
	
	memset( &v1, 0, sizeof(SModuleVersion));
	memset( &v2, 0, sizeof(SModuleVersion));
	
	EXCCHECK( ParseVersions( currentVersion, &v1, NULL));
	
	for ( ; prequired && is_valid == FALSE; prequired = strchr(prequired, ',') )
	{
		if ( *prequired == ',' ) prequired++;
		
		if ( IsVersionRange(prequired))
		{
			char	tmp_v1[16];
			char	tmp_v2[16];
			EXCCHECK( SplitVersionRange( prequired, tmp_v1, tmp_v2, sizeof(tmp_v1)));
			EXCCHECK( ParseVersions( tmp_v2, &v2, &cmp_fce));
			EXCCHECK( CompareTwoVersions( cmp_fce, &v1, &v2, &is_valid));
			if ( is_valid )
			{
				EXCCHECK( ParseVersions( tmp_v2, &v2, &cmp_fce));
				EXCCHECK( CompareTwoVersions( cmp_fce, &v1, &v2, &is_valid));
			}
		}
		else
		{
			EXCCHECK( ParseVersions( prequired, &v2, &cmp_fce));
			EXCCHECK( CompareTwoVersions( cmp_fce, &v1, &v2, &is_valid));
		}
	}

Error:
	*isValid = is_valid;
	EXCRETHROW( pexception);
} /* CompareVersions */


#undef __FUNC__
#define __FUNC__ "CompareTwoVersions"
static SElExceptionPtr CompareTwoVersions(
	uint32_t	compareFunction,
	SModuleVersionPtr	pVersion1,
	SModuleVersionPtr	pVersion2,
	bool_t*				pIsValid
)
{
	SElExceptionPtr	pexception = NULL;
	
	if ( compareFunction & MODULE_VERSION_REQUIRED_EQUAL )
	{
		if ( pVersion1->a == pVersion2->a && pVersion1->b == pVersion2->b && pVersion1->c==pVersion2->c)
		{
			*pIsValid = TRUE; goto Error;
		}
	}
	compareFunction &= ~MODULE_VERSION_REQUIRED_EQUAL; 
	if ( compareFunction )
	{
		switch (compareFunction)
		{
		case MODULE_VERSION_REQUIRED_LESS:
			if ( pVersion1->a < pVersion2->a)
			{
				*pIsValid = TRUE; break;
			}
			else
			if ( pVersion1->a > pVersion2->a)
			{
				*pIsValid = FALSE; break;
			}
			
			if ( pVersion1->b < pVersion2->b)
			{
				*pIsValid = TRUE; break;
			}
			else
			if ( pVersion1->b > pVersion2->b)
			{
				*pIsValid = FALSE; break;
			}
			if ( pVersion1->c < pVersion2->c)
			{
				*pIsValid = TRUE; break;
			}
			break;
		case MODULE_VERSION_REQUIRED_GREATER:
			if ( pVersion1->a < pVersion2->a)
			{
				*pIsValid = FALSE; break;
			}
			else
			if ( pVersion1->a > pVersion2->a)
			{
				*pIsValid = TRUE; break;
			}
			
			if ( pVersion1->b < pVersion2->b)
			{
				*pIsValid = FALSE; break;
			}
			else
			if ( pVersion1->b > pVersion2->b)
			{
				*pIsValid = TRUE; break;
			}
			if ( pVersion1->c > pVersion2->c)
			{
				*pIsValid = TRUE; break;
			}
			break;
		case (MODULE_VERSION_REQUIRED_LESS | MODULE_VERSION_REQUIRED_GREATER):
			if ( pVersion1->a == pVersion2->a && pVersion1->b == pVersion2->b && pVersion1->c==pVersion2->c)
			{
				*pIsValid = FALSE; break;
			}
			*pIsValid = TRUE;
			break;
		default:
			break;

		}
	}
Error:
	EXCRETHROW( pexception);
}


/*---------------------------------------------------------------------------*/
#define CHECKDOT( pchar) \
	if ( *pchar == '\0' || *pchar == ',' || *pchar == ']') goto Error;\
	else if (*pchar != '.' ) {\
		EXCTHROW( MODULE_ERR_INVALID_VERSION,\
				  "Version must contains numbers and dots."); \
	} else 


#undef __FUNC__
#define __FUNC__ "ParseVersions"
static SElExceptionPtr ParseVersions(
	const char*	versionString,
	SModuleVersionPtr	pVersion,
	uint32_t*			pCompareFunction
)
{
	SElExceptionPtr	pexception = NULL;
	const char*	pversion_string = versionString;
	char*		pnext = NULL;

	if ( pCompareFunction )
	{
		int	i;
		uint32_t	cmp_fce;
		*pCompareFunction = 0;
		for( i = 2; i > 0; i--, pversion_string++)
		{
			switch(*pversion_string)
			{
				case '=': cmp_fce = MODULE_VERSION_REQUIRED_EQUAL;   break;
				case '<': cmp_fce = MODULE_VERSION_REQUIRED_LESS;    break;
				case '>': cmp_fce = MODULE_VERSION_REQUIRED_GREATER; break;
				default: cmp_fce = 0; i = 0; break;
			}
			if ( cmp_fce == 0 ) break;
		   *pCompareFunction |= cmp_fce;
		} /* for( i = 2; i > 0; i--, pversion_string++) */
	} /* if ( pCompareFunction ) */
	pVersion->a = strtol( pversion_string, &pnext, 10);
	CHECKDOT( pnext);
	pVersion->b = strtol( ++pnext, &pnext, 10);
	CHECKDOT( pnext);
	pVersion->c = strtol( ++pnext, &pnext, 10);
	CHECKDOT( pnext);
	pVersion->d = strtol( ++pnext, &pnext, 10);
	CHECKDOT( pnext);

Error:
	if ( pCompareFunction && *pCompareFunction == 0 )
	{
		*pCompareFunction |= MODULE_VERSION_REQUIRED_EQUAL;
		*pCompareFunction |= MODULE_VERSION_REQUIRED_GREATER;
	}
	EXCRETHROW( pexception);
} /* ParseVersions */


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "SplitVersionRange"
static SElExceptionPtr SplitVersionRange(
	const char*	pVersionString,
	char		version1[],
	char		version2[],
	uint32_t	bufferSize
)
{
	SElExceptionPtr	pexception = NULL;
	const char	*pend = strchr(pVersionString, ',');
	const char	*prange = strchr(pVersionString, '-');
	uint32_t	s1, s2;
		
	if ( prange == NULL || prange > pend )
	{
		EXCTHROW( MODULE_ERR_NOT_VERSION_RANGE, "Not valid version Range");	
	}
	
	s1 = (uint32_t) (prange-pVersionString);
	s2 = (uint32_t) strlen(++prange);
	
	strncpy( version1, pVersionString, bufferSize);
	strncpy( version2, prange, bufferSize);
	
	if ( s1 >= bufferSize || s2 >= bufferSize )
	{
		EXCTHROW( ( s1 > s2 ) ? s1 : s2, "Small Buffer Size");
	}

Error:
	EXCRETHROW( pexception);
} /* SplitVersionRange */


/*---------------------------------------------------------------------------*/
/*!
 * \brief Function checks if the specified required version is version range.
 * @param[in] pVersionSring  required version string
 * @return TRUE if the first version of\p pVersionString is range of versions.
 */
#undef __FUNC__
#define __FUNC__ "IsVersionRange"
static bool_t IsVersionRange(
	const char* pVersionSring
)
{
	const char* prange = strchr(pVersionSring, '-');

	if ( prange )
	{
		const char* pend = strchr(pVersionSring, ',');
		if ( pend )
		{
			return ( prange < pend ) ? TRUE : FALSE;
		}
		return TRUE;
	}
	return FALSE;
} /* IsVersionRange */

#undef __FUNC__
#define __FUNC__ "CompareVersionOfModules"
int CompareVersionOfModules(
	const void *element1,
	const void *element2
)
{
	const char*	pitem1 = (((SModuleItemPtr)element1)->RequiredModules)->GetPtr(((SModuleItemPtr)element1)->RequiredModules);
	const char*	pitem2 = (((SModuleItemPtr)element2)->RequiredModules)->GetPtr(((SModuleItemPtr)element2)->RequiredModules);
	
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
		 if ( strstr( pitem2, ((SModuleItemPtr)element1)->moduleID) )
		 {
			return -1;
		 }
		 else
		 if ( strstr( pitem1, ((SModuleItemPtr)element2)->moduleID) )
		 {
		    return 1;
		 }
	}

	return 0;
} /* CompareVersionOfModules */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Sort" 
/* for solving of multidependencies */
static SElExceptionPtr Sort(
	SModuleItemPtr pModules, 
	uint32_t count
)
{
	SElExceptionPtr	pexception = NULL;
	SModuleItemPtr	pdata = pModules;
	SModuleItemPtr	pdata2 = NULL;
	SModuleItem		pdata_tmp = {0};
	int32_t			i, j, i_tmp = -1, err_cnt = 0;
	SELStringPtr	pelstring = NULL;

	EXCCHECK( elstring_new(NULL, &pelstring));

	/* modules without dependencies to the top */
	for(i=0; i<count; i++)
	{
		const char* ids = pdata->RequiredModules->GetPtr(pdata->RequiredModules);
		
		/* find first module with dependencies */
		if(i_tmp<0 && *ids)
		{
			pdata_tmp = *pdata;
			i_tmp = i;
		}
		else if(i_tmp>=0 && !*ids)
		{
			pModules[i_tmp] = *pdata;
			pModules[i] = pdata_tmp;

			i = i_tmp;
			pdata = &pModules[i_tmp];

			i_tmp = -1;
		}
		pdata++;
	};

	/* sort modules with dependencies */
	for(i=i_tmp, pdata=&pModules[i_tmp]; i<count; i++)
	{
		const char* ids = pdata->RequiredModules->GetPtr(pdata->RequiredModules);
		
		EXCCHECK( pelstring->Empty(pelstring));
		EXCCHECK( pelstring->Append(pelstring, ids));

		pdata2 = pModules;
		for(j=0; j<i; j++, pdata2++)
		{
			const char* id = pdata2->moduleID;
			
			if(strstr(ids, id))
				EXCCHECK( RemoveRequiredModuleFromString(pelstring, id));
		}

		/* there is not required module in front, move module to the end of the list */
		if(pelstring->Size(pelstring))
		{
			pdata_tmp = *pdata;
			
			memmove(pdata, pdata+1, sizeof(SModuleItem)*(count-i-1));
			pModules[count-1] = pdata_tmp;

			if(++err_cnt>=(count-i)) /* some module is missing */
				break;

			i--;
		}
		else
		{
			pdata++;
			err_cnt = 0;
		}
	}

Error:
	EXCRETHROW( pexception);
}
