
#ifndef		__EL_PLUGININFO_H__
#define		__EL_PLUGININFO_H__

#include <eltypes.h>
#include <exception/Exception.h>

	enum 
	{
		PluginFunction_Init = 0,
		PluginFunction_Run,
		PluginFunction_Shutdown
	} EPluginFunction;

typedef struct _SPluginInfoData
{
	unsigned char	version[4];
	const char*		Name;
	const char*		Vendor;
	const char*		Description;
	const char*		Info;
	const char*		www;
	const char*		email;
	const char*		uuid;
	const char*		target;
	const char*		build_date;
	const char*		build_time;
} SPluginInfoData, *SPluginInfoDataPtr;

//---------------------------------------------------------------------------------------------------------------------------
///	@brief		The plugin information class.
///	@ingroup	SPL
//---------------------------------------------------------------------------------------------------------------------------
typedef struct _SPluginInfo
{
	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Set info data.
	//-----------------------------------------------------------------------------------------------------------------------
	SElExceptionPtr (*SetInfoData)( struct _SPluginInfo* me, SPluginInfoDataPtr pInfo); 
	
	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Clears the structure.
	//-----------------------------------------------------------------------------------------------------------------------
	SElExceptionPtr (*Clear)( struct _SPluginInfo* me ); 

	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Returns the argument count of the specified function.
	///	@param		a_enuFunc	[in] Function to get the argument count from.
	///	@return		Number of arguments of the specified function.
	//-----------------------------------------------------------------------------------------------------------------------
	SElExceptionPtr (*GetArgCount)( struct _SPluginInfo* me, int32_t* a_enuFunc );

	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Returns an argument's description.
	///	@param		a_enuFunc	[in] Function to get the argument description from.
	///	@param		a_lIndex	[in] Index (0-based) of argument.
	///	@return		The argument's description.
	//-----------------------------------------------------------------------------------------------------------------------
	SElExceptionPtr (*GetArgDescription)( struct _SPluginInfo* me, int32_t a_enuFunc, const long a_lIndex, char* buffer, int32_t bufferSize );

	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Returns an argument's name.
	///	@param		a_enuFunc	[in] Function to get the argument description from.
	///	@param		a_lIndex	[in] Index (0-based) of argument.
	///	@return		The argument's name.
	//-----------------------------------------------------------------------------------------------------------------------
	SElExceptionPtr (*GetArgName)( struct _SPluginInfo* me, int32_t a_enuFunc, const long a_lIndex, char* buffer, int32_t bufferSize );

	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Returns the plugin's name.
	///	@return		The plugin's name.
	//-----------------------------------------------------------------------------------------------------------------------
	SElExceptionPtr (*GetName)( struct _SPluginInfo* me, char* buffer, int32_t bufferSize );

	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Returns the plugin's vendor.
	///	@return		The plugin's vendor.
	//-----------------------------------------------------------------------------------------------------------------------
	SElExceptionPtr (*GetVendor)( struct _SPluginInfo* me, char* buffer, int32_t bufferSize );
  
	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Returns the plugin's information string.
	///	@return		The plugin's information string.
	//-----------------------------------------------------------------------------------------------------------------------
	SElExceptionPtr (*GetInfo)( struct _SPluginInfo* me, char* buffer, int32_t bufferSize );

	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Returns the plugin's description.
	///	@return		The plugin's description.
	//-----------------------------------------------------------------------------------------------------------------------
	SElExceptionPtr (*GetDescription)( struct _SPluginInfo* me, char* buffer, int32_t bufferSize );

	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Returns the plugin's homepage.
	///	@return		The plugin's homepage.
	//-----------------------------------------------------------------------------------------------------------------------
	SElExceptionPtr (*GetHomepage)( struct _SPluginInfo* me, char* buffer, int32_t bufferSize );

	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Returns the plugin's e-mail address.
	///	@return		The plugin's e-mail address.
	//-----------------------------------------------------------------------------------------------------------------------
	SElExceptionPtr (*GetEmail)( struct _SPluginInfo* me, char* buffer, int32_t bufferSize );

	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Returns the plugin's major version (e.g. 1.xx).
	///	@return		The plugin's major version.
	//-----------------------------------------------------------------------------------------------------------------------
	SElExceptionPtr (*GetMajorVersion)( struct _SPluginInfo* me, char* buffer, int32_t bufferSize );

	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Returns the plugin's minor version (e.g. x.12).
	///	@return		The plugin's minor version.
	//-----------------------------------------------------------------------------------------------------------------------
	SElExceptionPtr (*GetMinorVersion)( struct _SPluginInfo* me, char* buffer, int32_t bufferSize );

	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Returns the plugin's build version.
	///	@return		The plugin's build version.
	//-----------------------------------------------------------------------------------------------------------------------
	SElExceptionPtr (*GetBuildVersion)( struct _SPluginInfo* me, char* buffer, int32_t bufferSize );
	
	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Returns the plugin's version (build.major.minor).
	///	@return		The plugin's build version.
	//-----------------------------------------------------------------------------------------------------------------------
	SElExceptionPtr (*GetVersion)( struct _SPluginInfo* me, char* buffer, int32_t bufferSize );

	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Returns the plugin's UUID.
	///	@return		The plugin's UUID.
	//-----------------------------------------------------------------------------------------------------------------------
	SElExceptionPtr (*GetUUID)( struct _SPluginInfo* me, char* buffer, int32_t bufferSize );

	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Returns the plugin's target.
	///	@return		The plugin's target.
	//-----------------------------------------------------------------------------------------------------------------------
	SElExceptionPtr (*GetTarget)( struct _SPluginInfo* me, char* buffer, int32_t bufferSize );

	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Reports whether the plugin has public arguments or not.
	///	@return		The method returns one of the following values:
	///	@retval		true	| The plugin has public arguments.
	///	@retval		false	| The plugin has no public arguments.
	//-----------------------------------------------------------------------------------------------------------------------
	SElExceptionPtr (*HasPublicArgs)( struct _SPluginInfo* me, bool_t *hasArg );

	//-----------------------------------------------------------------------------------------------------------------------
	///	@brief		Returns the plugin's build date and time.
	///	@return		The plugin's build version.
	//-----------------------------------------------------------------------------------------------------------------------
	SElExceptionPtr (*GetBuildDateTime)( struct _SPluginInfo* me, char* buffer, int32_t bufferSize );

	SPluginInfoDataPtr		_pInfoData;
} SPluginInfo, *SPluginInfoPtr;

SElExceptionPtr	PluginInfo_new(SPluginInfoDataPtr pData, SPluginInfoPtr* pInfo);
SElExceptionPtr	PluginInfo_delete(SPluginInfoPtr*	pPluginInfo);

#endif	//	__SPL_PLUGININFO_H__
