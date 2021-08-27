
#ifndef		__EL_PLUGIN_H__
#define		__EL_PLUGIN_H__

#include <eltypes.h>
#include "el_pluginargs.h"
#include "el_plugininfo.h"
#include "el_pluginserver.h"

//-----------------------------------------------------------------------------------------------------------------------
///	@brief		Macro for exporting SPL functions.
//-----------------------------------------------------------------------------------------------------------------------
#if defined(WINDOWS)

	#define PLUGIN_API		__declspec( dllexport )

#elif defined(UNIX)

	#define PLUGIN_API		extern "C"

#else

	#define PLUGIN_API

#endif

//-----------------------------------------------------------------------------------------------------------------------
///	@brief		Macro for exporting SPL's default functions.
//-----------------------------------------------------------------------------------------------------------------------
#if defined(WINDOWS)
	#define DEFINE_PLUGIN_EXPORTS()\
		PLUGIN_API SElExceptionPtr _FUNCC pluginInitialize( SPluginArgsPtr a_pPluginArgs );\
		PLUGIN_API SElExceptionPtr _FUNCC pluginRun( SPluginArgsPtr a_pPluginArgs, const char* uuid );\
		PLUGIN_API SElExceptionPtr _FUNCC pluginLeave(SPluginArgsPtr a_pPluginArgs, const char* uuid );\
		PLUGIN_API SElExceptionPtr _FUNCC pluginGetInfo( SPluginInfoPtr* pInfo );\
		PLUGIN_API SElExceptionPtr _FUNCC pluginGetAttribute( uint32_t attrID, uint32_t size, void* value )

	#define DEFINE_PLUGIN_DLLMAIN()							\
		BOOL APIENTRY DllMain( HANDLE a_hModule,				\
							   DWORD  a_dwReasonForCall,		\
							   LPVOID a_lpReserved )			\
		{														\
			switch( a_dwReasonForCall )							\
			{													\
				case DLL_PROCESS_ATTACH:						\
				case DLL_THREAD_ATTACH:							\
				case DLL_THREAD_DETACH:							\
				case DLL_PROCESS_DETACH:						\
					break;										\
			}													\
																\
			return TRUE;										\
		}

#else 

	#define DEFINE_PLUGIN_DLLMAIN()
	#define DEFINE_PLUGIN_EXPORTS()

#endif

//-----------------------------------------------------------------------------------------------------------------------
///	@brief		Macro for exporting SPL's default functions.
//-----------------------------------------------------------------------------------------------------------------------
#define DEFINE_PLUGIN_INFO( a_lBuildNumber,				\
							a_lMajorVersion,			\
							a_lMinorVersion,			\
							a_bPublicArgInfos,		\
							a_strName,						\
							a_strVendor,					\
							a_strDescription,			\
							a_strInfo,						\
							a_strHomepage,				\
							a_strEmail,						\
							a_strUUID,            \
                            a_strLocation )				\
\
	SPluginInfoData gs_InfoData =  {\
		{ a_lMajorVersion, a_lMinorVersion, a_lBuildNumber, a_bPublicArgInfos},\
			a_strName,\
			a_strVendor,\
			a_strDescription,\
			a_strInfo,\
			a_strHomepage,\
			a_strEmail,\
			a_strUUID,\
			a_strLocation,\
			__DATE__,\
			__TIME__\
				}

//-----------------------------------------------------------------------------------------------------------------------
///	@brief		Macro for setting the plugin-side arguments for the "splInitialize" function.
//-----------------------------------------------------------------------------------------------------------------------
#define SET_PLUGIN_INIT_ARGUMENT( ppi, a_strArgumentName, a_strArgumentDescription )		\
																						\
//	ppi->SetArgument( PluginFunction_Init, a_strArgumentName, a_strArgumentDescription );										

//-----------------------------------------------------------------------------------------------------------------------
///	@brief		Macro for setting the plugin-side arguments for the "splRun" function.
//-----------------------------------------------------------------------------------------------------------------------
#define SET_PLUGIN_RUN_ARGUMENT( ppi, a_strArgumentName, a_strArgumentDescription )		\
																						\
//	ppi->SetArgument( PluginFunction_Run, a_strArgumentName, a_strArgumentDescription );

//-----------------------------------------------------------------------------------------------------------------------
///	@brief		Macro for setting the plugin-side arguments for the "splShutdown" function.
//-----------------------------------------------------------------------------------------------------------------------
#define SET_PLUGIN_SHUTDOWN_ARGUMENT( ppi, a_strArgumentName, a_strArgumentDescription )		\
																							\
//	ppi->SetArgument( PluginFunction_Shutdown, a_strArgumentName, a_strArgumentDescription );

//-----------------------------------------------------------------------------------------------------------------------
///	@brief		Macro for implementing a default "splGetInfo" function.
//-----------------------------------------------------------------------------------------------------------------------
#define PLUGIN_IMPLEMENT_PLUGIN_GETINFO()								\
																	\
PLUGIN_API SPluginInfoPtr PLUGIN_GETINFO_NAME_CODE( void ) \
{ \
	return PluginInfo_new( &gs_InfoData, NULL); \
}

//-----------------------------------------------------------------------------------------------------------------------
///	@brief		Macro for implementing a default "splInitialize" function.
//-----------------------------------------------------------------------------------------------------------------------
#define PLUGIN_IMPLEMENT_PLUGIN_INITIALIZE()									\
																			\
	PLUGIN_API int32_t PLUGIN_INIT_NAME_CODE( SPluginArgs* a_pPluginArgs )	\
	{																		\
		return 0;														\
	}

//-----------------------------------------------------------------------------------------------------------------------
///	@brief		Macro for implementing a default "splShutdown" function.
//-----------------------------------------------------------------------------------------------------------------------
#define PLUGIN_IMPLEMENT_PLUGIN_SHUTDOWN()											\
																				\
	PLUGIN_API int32_t PLUGIN_SHUTDOWN_NAME_CODE( SPluginArgs* a_pPluginArgs )	\
	{																			\
		return 0;															\
	}

#endif	//	__EL_PLUGIN_H__
