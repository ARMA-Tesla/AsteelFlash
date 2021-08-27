

/*----------------------------------------------------------------------------
/// @file		el_pluginargs.h
/// @version	1.0
/// @brief		The plugin argument class.
/// @author		René Stuhr
//----------------------------------------------------------------------------*/

#ifndef		__EL_PLUGINARGS_H__
#define		__EL_PLUGINARGS_H__

#include <eltypes.h>
#include <exception/Exception.h>


#define ARG_INT32       (1L << 0)
#define ARG_UINT32      (1L << 1)
#define ARG_REAL64      (1L << 2)
#define ARG_PTR         (1L << 3)
#define ARG_STR7        (1L << 4)
#define ARG_DSTR        (1L << 5)

//------------------------------------------------------------------------------
///	@brief		The plugin argument class.
///	@ingroup	SPL
//------------------------------------------------------------------------------
typedef struct _SPluginArgs
{

	/*----------------------------------------------------------------------------
	 *	\brief		Sets an argument with the specified name.
	 *--------------------------------------------------------------------------*/
	SElExceptionPtr (*SetArg)( struct _SPluginArgs* me,
                     const char* argName,
                     uint32_t argType,
                     void* a_pvArg );

	/*----------------------------------------------------------------------------
	 *	\brief		Returns the number of arguments.
	 *--------------------------------------------------------------------------*/
	uint32_t (*GetCount)( struct _SPluginArgs* me );// const;

	/*---------------------------------------------------------------------------
	 *	@brief		Gets the argument with the specified name.
	 *--------------------------------------------------------------------------*/
	SElExceptionPtr (*GetArg)( struct _SPluginArgs* me,
                     const char* argName,
                     uint32_t argSize,
                     void* a_pvArg);
	int32_t (*DeleteArg)( struct _SPluginArgs* me,
                     const char* argName);

	/*---------------------------------------------------------------------------
	 *--------------------------------------------------------------------------*/
	SElExceptionPtr (*Clear)( struct _SPluginArgs* me );

	//----------------------------------------------------------------------------
	//***************************     ATTRIBUTES     *****************************
	//----------------------------------------------------------------------------

  void* _pArguments;
  uint32_t  _AllocCount;
  uint32_t  _UsedCount;
} SPluginArgs, *SPluginArgsPtr;

SElExceptionPtr pluginargs_new(SPluginArgsPtr* pParams);
SElExceptionPtr pluginargs_delete(SPluginArgsPtr* pArgs);


#define PLUGINARG_ERR_INVALID_ARGUMENT_NAME		-1111

#endif	// End __EL_PLUGINARGS_H__
