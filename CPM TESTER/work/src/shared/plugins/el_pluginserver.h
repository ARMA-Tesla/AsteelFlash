#ifndef		__EL_PLUGINSERVER_H__
#define		__EL_PLUGINSERVER_H__

#include <eltypes.h>
#include "el_plugininfo.h"
#include "el_pluginargs.h"
#include <exception/Exception.h>

#if defined(_cplusplus)
	extern "C" {
#endif

typedef void* UserData_t;

typedef struct _SPluginServer
{
	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Returns the current number of loaded plugins.
	//-----------------------------------------------------------------------------------------------------------------------
	int32_t (*GetLoadedPlugins)( struct _SPluginServer *me);

	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Returns information about a plugin from internal plugin info map.
	/// @remarks	Index is 0-based.
	///	@param		a_lIndex		[in] The plugin's index.
	///	@retval		A slcPluginInfo structure containing the plugin's information.
	/// @sa			slcPluginInfo
	//-----------------------------------------------------------------------------------------------------------------------
	SPluginInfoPtr (*GetSafedPluginInfo)( struct _SPluginServer *me, uint32_t index );

	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Returns information about a plugin, use this to safe the info to info map.
	/// @remarks	Index is 0-based.
	///	@param		a_lIndex		[in] The plugin's index.
	///	@retval		A slcPluginInfo structure containing the plugin's information.
	/// @sa			slcPluginInfo
	//-----------------------------------------------------------------------------------------------------------------------
	SElExceptionPtr (*GetPluginInfo)( struct _SPluginServer *me, uint32_t a_lIndex, SPluginInfoPtr* pInfo);

	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Loads all plugins with a given wildcard in a specified path.
	///	@param		a_strPath		[in] Valid path for searching the plugins.
	///	@param		a_strWildcard	[in] File wildcard (e.g. *.*, *.dll) for the plugins.
	///	@param		a_pPluginArgs	[in] Pointer to arguments for initializing all plugins after they have been loaded.
	///	@return		Number of successfully loaded plugins.
	//  @remarks	This function is pure virtual and must be overwritten!
	//-----------------------------------------------------------------------------------------------------------------------
	SElExceptionPtr (*LoadAllPlugins)( struct _SPluginServer *me, const char* a_strPath, const char*	a_strWildcard, SPluginArgsPtr pPluginArgs);

	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Loads a plugin with the specified file name.
	///	@param		a_strFileName	[in] The plugin's file name.
	///	@param		a_pPluginArgs	[in] Pointer to plugin's start function arguments.
	///	@return		The method returns one of the following values:
	///	@retval		-2 | Plugin was not found.
	///	@retval		-1 | Plugin's initialize function was not found.
	///	@retval		 0 | Error while executing the intialisation function.
	///	@retval		 1 | Plugin was successfully loaded.
	//  @remarks	This function is pure virtual and must be overwritten!
	//-----------------------------------------------------------------------------------------------------------------------
	SElExceptionPtr (*LoadPlugin)( struct _SPluginServer *me, const char* a_strFileName, SPluginArgs* _pPluginArgs);

	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Unloads a plugin with the given index (0-based).
	///	@param		a_lIndex		[in] The plugin's index.
	///	@param		a_pPluginArgs	[in] Pointer to plugin's shutdown function arguments.
	///	@return		The method returns one of the following values:
	///	@retval		true	| The plugin was unloaded successfully.
	///	@retval		false	| Could not unload plugin.
	//  @remarks	This function is pure virtual and must be overwritten!
	//-----------------------------------------------------------------------------------------------------------------------
	SElExceptionPtr (*UnloadPlugin)( struct _SPluginServer *me,	uint32_t index, SPluginArgs* pPluginArgs);

	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Unloads all plugins.
	///	@param		a_pPluginArgs	[in] Pointer to arguments for shutting down all plugins after they have been unloaded.
	//  @remarks	This function is pure virtual and must be overwritten!
	//-----------------------------------------------------------------------------------------------------------------------
	SElExceptionPtr (*UnloadAllPlugins)( struct _SPluginServer *me,	SPluginArgsPtr pPluginArgs);

	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Init, Run or Leave a plugin with the given index (0-based) and arguments.
	///	@param		a_lIndex		[in] The plugin's index.
	///	@param		a_pPluginArgs	[in] Pointer to plugin's run function arguments.
	///	@return		The method returns one of the following values:
	///	@retval		true	| The plugin execution was successful.
	///	@retval		false	| An error occured while executing the plugin.
	//  @remarks	This function is pure virtual and must be overwritten!
	//-----------------------------------------------------------------------------------------------------------------------
	SElExceptionPtr (*InitializePlugin)( struct _SPluginServer *me, uint32_t Index, SPluginArgs* pPluginArgs );
	SElExceptionPtr (*RunPlugin)( struct _SPluginServer *me, uint32_t Index, SPluginArgs* pPluginArgs, const char* uuid );
	SElExceptionPtr (*LeavePlugin)( struct _SPluginServer *me, uint32_t Index, SPluginArgs* pPluginArgs, const char* uuid );

	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Init, Run or Leave all plugins with the given arguments.
	///	@param		a_pPluginArgs	[in] Pointer to plugin's run function arguments.
	///	@return		Number of successfully executed plugins.
	//  @remarks	This function is pure virtual and must be overwritten!
	//-----------------------------------------------------------------------------------------------------------------------
	SElExceptionPtr (*InitializeAllPlugins)( struct _SPluginServer *me, SPluginArgs* pPluginArgs );
	SElExceptionPtr (*RunAllPlugins)( struct _SPluginServer *me, SPluginArgs* pPluginArgs );
	SElExceptionPtr (*LeaveAllPlugins)( struct _SPluginServer *me, SPluginArgs* pPluginArgs );

	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Runs or Leave a selected plugin and dependent plugins with the given arguments.
	///	@param		a_pPluginArgs	[in] Pointer to plugin's run function arguments.
	///	@return		Number of successfully executed plugins.
	//  @remarks	This function is pure virtual and must be overwritten!
	//-----------------------------------------------------------------------------------------------------------------------
	SElExceptionPtr (*RunSelectedPlugin)( struct _SPluginServer *me, const char* uuid, SPluginArgs* pPluginArgs );
	SElExceptionPtr (*LeaveSelectedPlugin)( struct _SPluginServer *me, const char* uuid, SPluginArgs* pPluginArgs );

#if 0
	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Unloads all plugins and looks for new plugins in the given path.
	///	@param		a_strPath				[in] Valid path for searching the plugins.
	///	@param		a_strWildcard			[in] File wildcard (e.g. *.*, *.dll) for the plugins.
	///	@param		a_pPluginShutdownArgs	[in] Pointer to arguments for shutting down all plugins before they will be unloaded.
	///	@param		a_pPluginInitArgs		[in] Pointer to arguments for initializing all plugins after they have been loaded.
	///	@return		Number of successfully loaded plugins.
	//  @remarks	This function is pure virtual and must be overwritten!
	//-----------------------------------------------------------------------------------------------------------------------
	int32_t (*Refresh)( struct _SPluginServer *me,
                        const char*		    a_strPath, 
						const char*		    a_strWildcard, 
						SPluginArgsPtr		a_pPluginShutdownArgs, 
						SPluginArgsPtr		a_pPluginInitArgs);

	int32_t (*SetUserData)( struct _SPluginServer *me,
						    uint32_t index,
						    void*    pUserData);
	int32_t (*GetUserData)( struct _SPluginServer *me,
						    uint32_t index,
						    void**   pUserData);
#endif
	uint32_t (*GetCount)( struct _SPluginServer *me);
	
	SElExceptionPtr (*SetLogFnc)( struct _SPluginServer* me,
								  SElExceptionPtr (*_LogFnc)(const char* message));
	//
	//-----------------------------------------------------------------------------------------------------------------------
	//*************************************************     ATTRIBUTES     **************************************************
	//-----------------------------------------------------------------------------------------------------------------------
	//
  uint32_t        _UsedCount;
  uint32_t        _AllocCount;
  void*           _pIds;
  UserData_t*	  _pUserData;
  SElExceptionPtr (*_LogFnc)(const char* message);
} SPluginServer, *SPluginServerPtr;

SElExceptionPtr PluginServer_new( SPluginServerPtr* pPluginServer );
SElExceptionPtr PluginServer_delete( SPluginServerPtr* pServer );

#define 	PLUGIN_ERR_FILE_NAME_MISSING		-11111
#define		PLUGIN_ERR_INDEX_OUT_OF_RANGE		-22222
#define		PLUGIN_ERR_MISSING_GETINFO_FUNCTION	-33333


#define		PLUGIN_INIT_NAME			"pluginInitialize"					///< The initialisation function's name.
#define		PLUGIN_INIT_NAME_CODE		 pluginInitialize					///< The initialisation function's name for internal macros.
#define		PLUGIN_RUN_NAME				"pluginRun"						///< The run function's name.
#define		PLUGIN_RUN_NAME_CODE		 pluginRun							///< The run function's name for internal macros.
#define		PLUGIN_LEAVE_NAME			"pluginLeave"						///< The run function's name.
#define		PLUGIN_LEAVE_NAME_CODE		 pluginLeave							///< The run function's name for internal macros.
#define		PLUGIN_GETINFO_NAME		    "pluginGetInfo"					///< The information function's name.
#define		PLUGIN_GETINFO_NAME_CODE	 pluginGetInfo						///< The information function's name for internal macros.
#define		PLUGIN_GETATTR_NAME    		"pluginGetAttribute"					///< The information function's name.
#define		PLUGIN_GETATTR_NAME_CODE	 pluginGetAttribute

//-----------------------------------------------------------------------------------------------------------------------
///	@brief		The plugin's initialisation function.
//-----------------------------------------------------------------------------------------------------------------------
typedef SElExceptionPtr (_FUNCC *PLUGIN_INIT_FUNC) ( SPluginArgsPtr );

//-----------------------------------------------------------------------------------------------------------------------
///	@brief		The plugin's run function.
//-----------------------------------------------------------------------------------------------------------------------
typedef SElExceptionPtr (_FUNCC *PLUGIN_RUN_FUNC )( SPluginArgsPtr, const char* );


//-----------------------------------------------------------------------------------------------------------------------
///	@brief		The plugin's leave function.
//-----------------------------------------------------------------------------------------------------------------------
typedef SElExceptionPtr (_FUNCC *PLUGIN_LEAVE_FUNC) ( SPluginArgsPtr, const char* );


//-----------------------------------------------------------------------------------------------------------------------
///	@brief		The plugin's shutdown function.
//-----------------------------------------------------------------------------------------------------------------------
typedef SElExceptionPtr (_FUNCC *PLUGIN_SHUTDOWN_FUNC )( SPluginArgsPtr );


//-----------------------------------------------------------------------------------------------------------------------
///	@brief		The plugin's information function.
//-----------------------------------------------------------------------------------------------------------------------
typedef SElExceptionPtr (_FUNCC *PLUGIN_INFO_FUNC )( SPluginInfoPtr* );


//-----------------------------------------------------------------------------------------------------------------------
///	@brief		The plugin's information function.
//-----------------------------------------------------------------------------------------------------------------------
typedef SElExceptionPtr (_FUNCC *PLUGIN_GETATTR_FUNC )( uint32_t  attrID, uint32_t size, void* value);



#if defined(_cplusplus)
	}
#endif

#endif	//	__SPL_PLUGINSERVER_H__
