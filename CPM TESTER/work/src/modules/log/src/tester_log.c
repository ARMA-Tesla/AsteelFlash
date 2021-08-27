#include "tester_log.h"
#include <global.h>
#include <elerror.h>
#include <stdlib.h>

/*****************************************************************************
 * Static no-exported functions declaration **********************************
 *****************************************************************************/
static int32_t	TesterLogFunction(
	int32_t		LogSubSystem,
	uint32_t	priority,
	const char*	message
);

 /*****************************************************************************
 * Exported functions definition *********************************************
 *****************************************************************************/
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "TesterKOSULog"
/*!
 * This function is used as log callback of KOSU module
 *
 * \param priority
 *  Priority of log message. TODO
 *
 * \param message
 *  message which will be stored into container
 *
  * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  TODO\n\n
 *  ERRORS:\n
 *  TODO\n
 */
int32_t _FUNCC TesterKOSULog(uint32_t  priority, const char* message)
{
	return TesterLogFunction( E_LOG_KOSU, priority, message);
} /* TesterKOSULog */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "TesteriButtonLog"
/*!
 * This function is used as log callback of iButton module
 *
 * \param priority
 *  Priority of log message. TODO
 *
 * \param message
 *  message which will be stored into container
 *
  * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  TODO\n\n
 *  ERRORS:\n
 *  TODO\n
 */
int32_t _FUNCC TesteriButtonLog(uint32_t  priority, const char* message)
{
	return TesterLogFunction( E_LOG_IBUTTON, priority, message);
} /* TesteriButtonLog */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "TesterVLineLog"
/*!
 * This function is used as log callback of valeo line module
 *
 * \param priority
 *  Priority of log message. TODO
 *
 * \param message
 *  message which will be stored into container
 *
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  TODO\n\n
 *  ERRORS:\n
 *  TODO\n
 */
int32_t _FUNCC TesterVLineLog(uint32_t  priority, const char* message)
{
	return TesterLogFunction( E_LOG_VLINE, priority, message);
} /* TesterVLineLog */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "TesterCANLog"
/*!
 * This function is used as log callback of CAN module
 *
 * \param priority
 *  Priority of log message. TODO
 *
 * \param message
 *  message which will be stored into container
 *
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  TODO\n\n
 *  ERRORS:\n
 *  TODO\n
 */
int32_t _FUNCC TesterCANLog(uint32_t  priority, const char* message)
{
	return TesterLogFunction( E_LOG_CAN, priority, message);
} /* TesterCANLog */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "TesterVANLog"
/*!
 * This function is used as log callback of CAN module
 *
 * \param priority
 *  Priority of log message. TODO
 *
 * \param message
 *  message which will be stored into container
 *
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  TODO\n\n
 *  ERRORS:\n
 *  TODO\n
 */
int32_t _FUNCC TesterVANLog(uint32_t  priority, const char* message)
{
	return TesterLogFunction( E_LOG_VAN, priority, message);
} /* TesterCANLog */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "TesterPrinterLog"
/*!
 * This function is used as log callback of Printer module
 *
 * \param priority
 *  Priority of log message. TODO
 *
 * \param message
 *  message which will be stored into container
 *
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  TODO\n\n
 *  ERRORS:\n
 *  TODO\n
 */
int32_t _FUNCC TesterPrinterLog(uint32_t  priority, const char* message)
{
	return TesterLogFunction( E_LOG_PRINTER, priority, message);
} /* TesterPrinterLog */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "TesterReaderLog"
/*!
 * This function is used as log callback of Barcode Reader module
 *
 * \param priority
 *  Priority of log message. TODO
 *
 * \param message
 *  message which will be stored into container
 *
  * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  TODO\n\n
 *  ERRORS:\n
 *  TODO\n
 */
int32_t _FUNCC TesterReaderLog(uint32_t  priority, const char* message)
{
	return TesterLogFunction( E_LOG_READER, priority, message);
} /* TesterReaderLog */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "TesterOPCLog"
/*!
 * This function is used as log callback of OPC module
 *
 * \param priority
 *  Priority of log message. TODO
 *
 * \param message
 *  message which will be stored into container
 *
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  TODO\n\n
 *  ERRORS:\n
 *  TODO\n
 */
int32_t _FUNCC TesterOPCLog(uint32_t  priority, const char* message)
{
	return TesterLogFunction( E_LOG_OPC, priority, message);
} /* TesterOPCLog */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "TesterFrameworkLog"
/*!
 * This function is used as log callback in framework
 *
 * \param priority
 *  Priority of log message. TODO
 *
 * \param message
 *  message which will be stored into container
 *
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  TODO\n\n
 *  ERRORS:\n
 *  TODO\n
 */
int32_t _FUNCC TesterFrameworkLog(uint32_t  priority, const char* message)
{
	return TesterLogFunction( E_LOG_FRAMEWORK, priority, message);
} /* TesterFrameworkLog */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "TesterStorageLog"
/*!
 * This function is used as log callback of storage module
 *
 * \param priority
 *  Priority of log message. TODO
 *
 * \param message
 *  message which will be stored into container
 *
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  TODO\n\n
 *  ERRORS:\n
 *  TODO\n
 */
int32_t _FUNCC TesterStorageLog(uint32_t  priority, const char* message)
{
	return TesterLogFunction( E_LOG_STORAGE, priority, message);
} /* TesterStorageLog */
/*****************************************************************************
 * Static no-exported functions definition ***********************************
 *****************************************************************************/
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "TesterLogFunction"
static int32_t	TesterLogFunction(
	int32_t		LogSubSystem,
	uint32_t	priority,
	const char*	message
)
{
	int32_t		error = 0;
	SGlobalPtr	pglobal = NULL;
	
	GetGlobalLockPtr(&pglobal);
	if ( pglobal->Log[LogSubSystem] )
	{
	CHECKERR( pglobal->Log[LogSubSystem]->Message( pglobal->Log[LogSubSystem], message, priority));
	}
Error:
	ReleaseGlobalLockPtr(&pglobal);
	return error;
} /* TesterLogFunction */

/*****************************************************************************
 * SYSTEM LOG CALLBACK *******************************************************
 *****************************************************************************/
#undef __FUNC__
#define __FUNC__ "TesterSystemLogCallback"
/*!
 * \brief This callback is used for system log.
 *
 * \param priority
 * Priority of the message
 *
 * \param ident
 * Identifier of the message
 *
 * \param message
 * message which is logged
 *
  * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  TODO\n\n
 *  ERRORS:\n
 *  TODO\n
 *
 * This callback is used for system log. every container has attached this
 * observer and when message is logged into container, this function will
 * be called, if the priority of the message is valid for systemlog mask.
 */
int32_t _FUNCC TesterSystemLogCallback(
	 uint32_t  priority,
	 const char* ident,
	 const char* message
	 
)
 {
	int32_t		error = 0;
	SGlobalPtr	pglobal = NULL;
	char		buffer[256] = "";
	char*		pmessage = buffer;
	bool_t		is_dynamic = FALSE;
	
	/*
	 * check that static buffer is enought for
	 * appended ident and message
	 */
	if ((strlen(ident) + strlen(message) + 1) >= sizeof(buffer))
	{
		/*
		 * we need larger buffer, use dynamic buffer
		 */
		is_dynamic = TRUE;
		pmessage = malloc(strlen(ident) + strlen(message)+2);
		CHECKALLOC( pmessage);
		*pmessage = '\0';
	}
	/*
	 * append ident and message
	 */
	strcat(pmessage, ident);
	strcat(pmessage, " ");
	strcat(pmessage, message);

	GetGlobalLockPtr(&pglobal);
	CHECKERR( pglobal->Log[E_LOG_SYSTEM]->Message( pglobal->Log[E_LOG_SYSTEM], pmessage, priority));

Error:
	if ( is_dynamic )
	{
		/*
		 * Buffer is dynamic, remove it
		 */
		free(pmessage);
	}
	ReleaseGlobalLockPtr(&pglobal);
	return error;
} /* TesterSystemLogCallback */
