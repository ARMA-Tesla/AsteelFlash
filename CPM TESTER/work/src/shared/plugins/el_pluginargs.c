//---------------------------------------------------------------------------------------------------------------------------
/// @file		spl_pluginargs.cpp
/// @version	1.0
/// @author		René Stuhr
//---------------------------------------------------------------------------------------------------------------------------

#include	"el_pluginargs.h"
#include	<string.h>
#include	<elerror.h>

#define ARG_VALUE_NAME_LENGTH	38 /* {uuid} */

typedef struct _SArgValue{
  char        name[ARG_VALUE_NAME_LENGTH+1];
  uint32_t    vType;
  union {
      int32_t   i32;
      uint32_t  u32;
      double    r64;
      char      s[8];
      void*     ptr;
  } val;
} SArgValue, *SArgValuePtr;


static SElExceptionPtr elPluginArgsSetArg(
  struct _SPluginArgs* pArg,
  const char* argName,
  uint32_t    argType,
  void*       a_pvArg
);
static SArgValuePtr elPluginArgsGetValue(
  struct _SPluginArgs* pArg,
  const char* argName
);
static SArgValuePtr elPluginArgsClearValue(
  SArgValuePtr pValue
);
static uint32_t elPluginArgsGetCount(
  struct _SPluginArgs* me
);
static SElExceptionPtr elPluginArgsGetArg(
  struct _SPluginArgs* pArg,
  const char* argName,
  uint32_t    argSize,
  void*       a_pvArg
);
static SElExceptionPtr elPluginArgsClear(
  struct _SPluginArgs* pArg
);
static int32_t elPluginArgsDeleteArg(
  struct _SPluginArgs* pArg,
  const char* argName
);


#undef __FUNC__
#define __FUNC__ "pluginargs_new"
SElExceptionPtr pluginargs_new(SPluginArgsPtr *pParam)
{
  SElExceptionPtr   pexception = NULL;
  
  
  *pParam = calloc(1, sizeof(SPluginArgs));
  EXCCHECKALLOC( *pParam);
  
  (*pParam)->SetArg   = elPluginArgsSetArg;
  (*pParam)->DeleteArg= elPluginArgsDeleteArg;
  (*pParam)->GetCount = elPluginArgsGetCount;
  (*pParam)->GetArg   = elPluginArgsGetArg;
  (*pParam)->Clear    = elPluginArgsClear;
  
Error:
  if ( pexception )
  {
		pluginargs_delete( pParam);
  }
  EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "pluginargs_delete"
SElExceptionPtr pluginargs_delete(SPluginArgsPtr* pArgs)
{
  if ( pArgs && *pArgs )
  {
    (*pArgs)->Clear(*pArgs);
    free( *pArgs);
    *pArgs = NULL;
  }
  
  return NULL;
}

#undef __FUNC__
#define __FUNC__ "elPluginArgsSetArg"
static SElExceptionPtr elPluginArgsSetArg(
  struct _SPluginArgs* pArg,
  const char* argName,
  uint32_t    argType,
  void*       a_pvArg
)
{
  SElExceptionPtr   pexception = NULL;
  SArgValuePtr parg_value = NULL;
  
  if ( argName == NULL || strlen(argName) == 0 )
  {
    EXCTHROW( PLUGINARG_ERR_INVALID_ARGUMENT_NAME, "Empty argument name.");
  }
  
  parg_value = elPluginArgsGetValue( pArg, argName);
  
  if ( parg_value )
  {
		elPluginArgsClearValue( parg_value);
  }
  else
  {
    if ( pArg->_UsedCount == pArg->_AllocCount )
    {
        pArg->_pArguments = realloc (pArg->_pArguments, ( pArg->_AllocCount + 5 ) * sizeof(SArgValue));
        EXCCHECKALLOC(pArg->_pArguments);
        memset( ((SArgValuePtr)pArg->_pArguments)+pArg->_AllocCount, 0, 5*sizeof(SArgValue));
        pArg->_AllocCount += 5;        
    }
    parg_value = ((SArgValuePtr)pArg->_pArguments)+pArg->_UsedCount;
    pArg->_UsedCount++;
  }
  
  strncpy(parg_value->name, argName, ARG_VALUE_NAME_LENGTH);
  parg_value->vType = argType; 
  switch ( argType )
  {
    case ARG_INT32:
      parg_value->val.i32 = *((int32_t*)a_pvArg);
      break;
    case ARG_UINT32:
      parg_value->val.u32 = *((uint32_t*)a_pvArg);
      break;
    case ARG_REAL64:
      parg_value->val.r64 = *((double*)a_pvArg);
      break;
    case ARG_PTR:
      parg_value->val.ptr = a_pvArg;
      break;
    case ARG_STR7:
      strncpy(parg_value->val.s, (const char*)a_pvArg, 7);
      break;
    case ARG_DSTR:
      {
        if ( (const char*)a_pvArg )
        {
			size_t length = strlen((const char*)a_pvArg);
			parg_value->val.ptr = malloc(length+1);
			strcpy(parg_value->val.ptr, (const char*)a_pvArg);
		}
		else
			parg_value->val.ptr = NULL;
      }
      break;
  }
Error:
	EXCRETHROW( pexception);
}

/*-----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "elPluginArgsGetCount"
static uint32_t elPluginArgsGetCount(
  struct _SPluginArgs* pArg
) 
{
	return pArg->_UsedCount;
}


/*-----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "elPluginArgsDeleteArg"
static int32_t elPluginArgsDeleteArg(
  struct _SPluginArgs* pArg,
  const char* argName
)
{
  int32_t   error = 0;
  SArgValuePtr  pvalue = (SArgValuePtr)(pArg->_pArguments);
  uint32_t      i, count = pArg->_UsedCount;
  bool_t		is_removed = FALSE;
  
  if ( argName == NULL || strlen(argName) == 0 )
  {
    CHECKERR( -1);
  }
  
  for(i = 0; i < count; i++, pvalue++)
  {
    if (strcmp(pvalue->name, argName) == 0 )
	{
		elPluginArgsClearValue( pvalue);
		memmove( pvalue, pvalue + 1, sizeof(SArgValue) * (count - 1 - i));
		pArg->_UsedCount -= 1;
		is_removed = TRUE;
		break;
	}
  }
  
  if ( is_removed )
  {
		SArgValuePtr  pvalue = (SArgValuePtr)(pArg->_pArguments);
		pvalue = pvalue + (count - 1);
		memset( pvalue, 0, sizeof(SArgValue));
  }

  
Error:
	return error;
}

/*-----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "elPluginArgsGetArg"
static SElExceptionPtr elPluginArgsGetArg(
  struct _SPluginArgs* pArg,
  const char* argName,
  uint32_t    argSize,
  void*       a_pvArg
)
{
  SElExceptionPtr   pexception = NULL;
  SArgValuePtr		parg_value = NULL;
  
  parg_value = elPluginArgsGetValue( pArg, argName);
  
  if ( !parg_value )
  {
    EXCTHROW( PLUGINARG_ERR_INVALID_ARGUMENT_NAME, "Empty argument name.");
  }

  switch ( parg_value->vType )
  {
    case ARG_INT32:
      *((int32_t*)a_pvArg) = parg_value->val.i32;
      break;
    case ARG_UINT32:
      *((uint32_t*)a_pvArg) = parg_value->val.u32;
      break;
    case ARG_REAL64:
      *((double*)a_pvArg) = parg_value->val.r64;
      break;
    case ARG_PTR:
	  *((void**)a_pvArg) = parg_value->val.ptr;
      break;
    case ARG_STR7:
    case ARG_DSTR:
	  *((void**)a_pvArg) = parg_value->val.ptr;
      break;
  }
Error:
	EXCRETHROW( pexception);
} /* elPluginArgsGetArg */

/*-----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "elPluginArgsClear"
static SElExceptionPtr elPluginArgsClear(
  struct _SPluginArgs* pArg
)
{
	SElExceptionPtr pexception = NULL;
	
	int32_t lArguments  = pArg->_UsedCount;
	pArg->_UsedCount  = 0;
	pArg->_AllocCount = 0;
	free( pArg->_pArguments);
	pArg->_pArguments = NULL;
/* Error: */
	EXCRETHROW( pexception);
} /* elPluginArgsClear */


/*-----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "elPluginArgsGetValue"
static SArgValuePtr elPluginArgsGetValue(
  struct _SPluginArgs* pArg,
  const char* argName
)
{
  
  SArgValuePtr  pvalue = (SArgValuePtr)(pArg->_pArguments);
  uint32_t      i, count = pArg->_UsedCount;
  
  if ( argName == NULL || strlen(argName) == 0 )
  {
    return NULL;
  }
  
  for(i = 0; i < count; i++, pvalue++)
  {
    if (strcmp(pvalue->name, argName) == 0 )
      return pvalue;
  }
  
  return NULL;
} /* elPluginArgsGetValue */

/*-----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "elPluginArgsClearValue"
static SArgValuePtr elPluginArgsClearValue(
  SArgValuePtr pValue
)
{
  if ( pValue )
  {
    if (pValue->vType == ARG_DSTR )
    {
        free(pValue->val.ptr);
        pValue->val.ptr = NULL;
    }
  }
	return pValue;
}
