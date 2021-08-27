#include "el_plugininfo.h"
#include <stdlib.h>
#include <stdio.h>
#include <elerror.h>
#include <string.h>

/*---------------------------------------------------------------------------*/
SElExceptionPtr Clear( struct _SPluginInfo* me )
{
	return NULL;
}

/*---------------------------------------------------------------------------*/
SElExceptionPtr GetArgCount( struct _SPluginInfo* me, int32_t* a_enuFunc )
{
	return NULL;
}

/*---------------------------------------------------------------------------*/
SElExceptionPtr GetArgDescription( struct _SPluginInfo* me, int32_t a_enuFunc, const long a_lIndex, char* buffer, int32_t bufferSize )
{
	return NULL;
}

/*---------------------------------------------------------------------------*/
SElExceptionPtr GetArgName( struct _SPluginInfo* me, int32_t a_enuFunc, const long a_lIndex, char* buffer, int32_t bufferSize )
{
	return NULL;
}

/*---------------------------------------------------------------------------*/
SElExceptionPtr GetName( struct _SPluginInfo* me, char* buffer, int32_t bufferSize )
{
	if(me->_pInfoData)
	{
		int len = (int) strlen(me->_pInfoData->Name)+1;
		strncpy(buffer, me->_pInfoData->Name, (bufferSize<len)? bufferSize : len); 
	}
	else
	{
		memset(buffer, 0, bufferSize);
	}
	return NULL;
}

/*---------------------------------------------------------------------------*/
SElExceptionPtr GetVendor( struct _SPluginInfo* me, char* buffer, int32_t bufferSize )
{
	if(me->_pInfoData)
	{
		int len = (int) strlen(me->_pInfoData->Vendor)+1;
		strncpy(buffer, me->_pInfoData->Vendor, (bufferSize<len)? bufferSize : len); 
	}
	else
	{
		memset(buffer, 0, bufferSize);
	}
	return NULL;
}

/*---------------------------------------------------------------------------*/
SElExceptionPtr GetInfo( struct _SPluginInfo* me, char* buffer, int32_t bufferSize )
{
	if(me->_pInfoData)
	{
		int len = (int) strlen(me->_pInfoData->Info)+1;
		strncpy(buffer, me->_pInfoData->Info, (bufferSize<len)? bufferSize : len); 
	}
	else
	{
		memset(buffer, 0, bufferSize);
	}
	return NULL;
}

/*---------------------------------------------------------------------------*/
SElExceptionPtr GetDescription( struct _SPluginInfo* me, char* buffer, int32_t bufferSize )
{
	if(me->_pInfoData)
	{
		int len = (int) strlen(me->_pInfoData->Description)+1;
		strncpy(buffer, me->_pInfoData->Description, (bufferSize<len)? bufferSize : len); 
	}
	else
	{
		memset(buffer, 0, bufferSize);
	}
	return NULL;
}

/*---------------------------------------------------------------------------*/
SElExceptionPtr GetHomepage( struct _SPluginInfo* me, char* buffer, int32_t bufferSize )
{
	if(me->_pInfoData)
	{
		int len = (int) strlen(me->_pInfoData->www)+1;
		strncpy(buffer, me->_pInfoData->www, (bufferSize<len)? bufferSize : len); 
	}
	else
	{
		memset(buffer, 0, bufferSize);
	}
	return NULL;
}

/*---------------------------------------------------------------------------*/
SElExceptionPtr GetEmail( struct _SPluginInfo* me, char* buffer, int32_t bufferSize )
{
	if(me->_pInfoData)
	{
		int len = (int) strlen(me->_pInfoData->email)+1;
		strncpy(buffer, me->_pInfoData->email, (bufferSize<len)? bufferSize : len); 
	}
	else
	{
		memset(buffer, 0, bufferSize);
	}
	return NULL;
}

/*---------------------------------------------------------------------------*/
SElExceptionPtr GetMajorVersion( struct _SPluginInfo* me, char* buffer, int32_t bufferSize )
{
	if(me->_pInfoData)
	{
		sprintf(buffer, "%d", me->_pInfoData->version[0]);	
	}
	else
	{
		memset(buffer, 0, bufferSize);  
	}
	return NULL;
}

/*---------------------------------------------------------------------------*/
SElExceptionPtr GetMinorVersion( struct _SPluginInfo* me, char* buffer, int32_t bufferSize )
{
	if(me->_pInfoData)
	{
		sprintf(buffer, "%d", me->_pInfoData->version[1]);	
	}
	else
	{
		memset(buffer, 0, bufferSize);  
	}
	return NULL;
}

/*---------------------------------------------------------------------------*/
SElExceptionPtr GetBuildVersion( struct _SPluginInfo* me, char* buffer, int32_t bufferSize )
{
	if(me->_pInfoData)
	{
		sprintf(buffer, "%d", me->_pInfoData->version[2]);	
	}
	else
	{
		memset(buffer, 0, bufferSize);  
	}
	return NULL;
}

/*---------------------------------------------------------------------------*/
SElExceptionPtr GetVersion( struct _SPluginInfo* me, char* buffer, int32_t bufferSize )
{
	if(me->_pInfoData)
	{
		sprintf(buffer, "%d.%d.%d", me->_pInfoData->version[0],
									me->_pInfoData->version[1],
									me->_pInfoData->version[2]);	
	}
	else
	{
		memset(buffer, 0, bufferSize);  
	}
	return NULL;
}

/*---------------------------------------------------------------------------*/
SElExceptionPtr GetUUID( struct _SPluginInfo* me, char* buffer, int32_t bufferSize )
{
	if(me->_pInfoData)
	{
		int len = (int) strlen(me->_pInfoData->uuid)+1;
		strncpy(buffer, me->_pInfoData->uuid, (bufferSize<len)? bufferSize : len); 
	}
	else
	{
		memset(buffer, 0, bufferSize);
	}
	return NULL;
}

/*---------------------------------------------------------------------------*/
SElExceptionPtr GetTarget( struct _SPluginInfo* me, char* buffer, int32_t bufferSize )
{
	if(me->_pInfoData)
	{
		int len = (int) strlen(me->_pInfoData->target)+1;
		strncpy(buffer, me->_pInfoData->target, (bufferSize<len)? bufferSize : len); 
	}
	else
	{
		memset(buffer, 0, bufferSize);
	}
	return NULL;
}

/*---------------------------------------------------------------------------*/
SElExceptionPtr HasPublicArgs( struct _SPluginInfo* me, bool_t *hasArg )
{
	if(me->_pInfoData)
	{
		if(hasArg) *hasArg = me->_pInfoData->version[3];	
	}
	else
	{
		if(hasArg) *hasArg = FALSE;
	}
	return NULL;
}

/*---------------------------------------------------------------------------*/
SElExceptionPtr GetBuildDateTime( struct _SPluginInfo* me, char* buffer, int32_t bufferSize )
{
	if(me->_pInfoData)
	{
		int len;
		char datetime[1024] = "";

		sprintf(datetime, "%s %s", me->_pInfoData->build_date, me->_pInfoData->build_time);
		len = (int) strlen(datetime)+1;
		strncpy(buffer, datetime, (bufferSize<len)? bufferSize : len); 
	}
	else
	{
		memset(buffer, 0, bufferSize);
	}
	return NULL;
}

/*---------------------------------------------------------------------------*/
SElExceptionPtr SetInfoData( struct _SPluginInfo* me, SPluginInfoDataPtr pInfo)
{
	if(pInfo)
	{
		me->_pInfoData = pInfo;
	}
	return NULL;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PluginInfo_new"
SElExceptionPtr	PluginInfo_new(
  SPluginInfoDataPtr pData,
  SPluginInfoPtr* pInfo
)
{
	SElExceptionPtr	pexception = NULL;

	*pInfo = calloc(1, sizeof(SPluginInfo));
	EXCCHECKALLOC( *pInfo);
	
	if(pData)
		(*pInfo)->_pInfoData 		= pData;
	
	(*pInfo)->Clear				= Clear;
	(*pInfo)->GetArgCount		= GetArgCount;
	(*pInfo)->GetArgDescription = GetArgDescription;
	(*pInfo)->GetArgName		= GetArgName;
	(*pInfo)->GetName 			= GetName;
	(*pInfo)->GetVendor			= GetVendor;
	(*pInfo)->GetInfo			= GetInfo;
	(*pInfo)->GetDescription	= GetDescription;
	(*pInfo)->GetHomepage		= GetHomepage;
	(*pInfo)->GetEmail			= GetEmail;
	(*pInfo)->GetMajorVersion   = GetMajorVersion;
	(*pInfo)->GetMinorVersion   = GetMinorVersion;
	(*pInfo)->GetBuildVersion   = GetBuildVersion;
	(*pInfo)->GetVersion 		= GetVersion;
	(*pInfo)->GetUUID			= GetUUID;
	(*pInfo)->GetTarget			= GetTarget;
	(*pInfo)->HasPublicArgs		= HasPublicArgs;
	(*pInfo)->GetBuildDateTime	= GetBuildDateTime;
	
	(*pInfo)->SetInfoData 		= SetInfoData;
	
Error:
	if ( *pInfo && pexception )
	{
		PluginInfo_delete(pInfo);
	}
	EXCRETHROW(pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "PluginInfo_delete"
SElExceptionPtr	PluginInfo_delete(
	SPluginInfoPtr*	pPluginInfo
)
{
	SElExceptionPtr	pexception = NULL;
	
	if ( pPluginInfo && *pPluginInfo)
	{
		(*pPluginInfo)->_pInfoData = NULL;
		free( *pPluginInfo);
		*pPluginInfo = NULL;
	}

/* Error: */
	EXCRETHROW(pexception);
}
