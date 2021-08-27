
#include <log.h>
#include <ansi_c.h>
#include <log/log_console.h>
#include <log/log_file.h>
#include <elerror.h>
#include <math.h>	/* log10 */
#if defined(_DEBUG)
#include <stdio.h>  /* PRINTERR */
#endif
#if defined(WINDOWS)
#include <log/log_debuger.h>
#endif

#if !defined(LOG_SIGNATURE_SIZE)
/*!
 * Define size of the maximum id in session factory. This ID should contains
 * full information from log_factory() and log_singelton() functions 
 * formated in string
 */  
#define     LOG_SIGNATURE_SIZE         1024
#endif /* LOG_SIGNATURE_SIZE */

#define LOG_ALLOC_QUANTUM               16
#define LOG_ALLOC_LISTENERS_QUANTUM     4

/*!
 * This macro is used for safe format of string into buffer.
 * 
 * \param form_str  format string
 * \param  param    parameter which will be formated
 * \param nbAdChar  number of aditional characters in format string
 * \param bufferSize  size of buffer, when pbuffer variable has been
 *                    initialized.
 *                     
 * \pre Defined and initialized pointer \p pbuffer on buffer 
 * \pre declared and initialized varaible \p required type of int32_t
 */  
#define SAFE_STR_FORMAT( form_str, param, nbAdChar, bufferSize ) \
  do { \
    required += (nbAdChar + (int32_t)strlen( param));  \
    if ( required < bufferSize ) {\
      pbuffer += sprintf( pbuffer, form_str, param);\
    }\
  } while( 0 )

/*!
 * This macro is used for safe format of int into buffer.
 * 
 * \param form_str  format string
 * \param  param    parameter which will be formated
 * \param nbAdChar  number of aditional characters in format string
 * \param bufferSize  size of buffer, when pbuffer variable has been
 *                    initialized.
 *                     
 * \pre Defined and initialized pointer \p pbuffer on buffer 
 * \pre declared and initialized varaible \p required type of int32_t
 */  
#define SAFE_INT_FORMAT( form_str, param, nbAdChar, bufferSize ) \
  do { \
    required += nbAdChar + ( abs(param) > 0 ) ? ((int)log10(param)) + 1 : 1 ;\
    if ((param) < 0 ) required += 1; \
    if ( required < bufferSize ) {\
      pbuffer += sprintf( pbuffer, form_str, param);\
    }\
  } while( 0 )
  
  


typedef struct _SLogFactorySessionEntry {
  char              signature[LOG_SIGNATURE_SIZE+1];  /*!< Specifies type of the session */
  SLogSessionPtr    session;          /*!< Session for given ID */
} SLogFactorySessionEntry, *SLogFactorySessionEntryPtr;

typedef struct _SLogFactorySession {
  CRITICAL_SECTION	Lock;    /*!< The lock of the session */
  uint32_t          AllocCount;
  uint32_t          Count;
  SLogFactorySessionEntryPtr  sessions;
} SLogFactorySession, *SLogFactorySessionPtr;

static SLogFactorySession       gs_LogSessionList;
static SLogFactorySessionPtr    gs_pLogSessionList = NULL;

/*****************************************************************************
 * Static no-exported functions declaration **********************************
 *****************************************************************************/
static int32_t LogFactoryLock(void);
static int32_t LogFactoryUnlock(void);
static int32_t LogLock(
    struct _SLogSession*	pLog
);
static int32_t LogUnlock(
    struct _SLogSession*	pLog
);
static int32_t _FUNCC LogLog(
    struct _SLogSession* pLog,
    uint32_t     priority,
    const char* fmt,
    ...
);
static int32_t LogMessage(
    struct _SLogSession* pLog,
    const char* message,
    uint32_t    priority
);
static int32_t LogClose(
    struct _SLogSession* pLog
);
static SLogFactorySession* LogGetSessionListPtr(void);
static int32_t  LogInitialize(void);
static int32_t BuildSignature(
    int32_t     handler,
    const char* name,
    const char* ident,
    const char* conf,
    uint32_t    level,
    char*       buffer,
    int32_t     bufferSize
);
static SLogSessionPtr GetSingeltonSessionEntry(const char* signature);
static int32_t  LogRegisterSession(
    const char* signature,
    SLogSessionPtr session
);
static int32_t LogSetMask(
    struct _SLogSession*  pLog,
    uint32_t    mask
);
static int32_t LogMaskUpTo(
    struct _SLogSession* pLog,
    uint32_t    priority
);
static int32_t LogGetFreeObcerver(
    struct _SLogSession*  pLog,
    SLogObserverPtr*    pObcerver
);
static int32_t LogAttach(
    struct _SLogSession*  pLog,
    LogCallbackPtr callaback,
    uint32_t mask
);
static int32_t LogGetAttribute(
    struct _SLogSession*  pLog,
    uint32_t	attributeID,
    void*		pValue
);
static int32_t	LogFlush(
	 struct _SLogSession*  pLog
);

/*****************************************************************************
 * Exported functions definition *********************************************
 *****************************************************************************/
 /*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "log_factory"
 /*!
 * Attempts to return a concrete Log instance of type \p handler.
 *
 * \param handler   The type of concrete Log subclass to return.
 *                  Attempt to dynamically include the code for
 *                  this subclass. Currently, valid values are
 *                  \li \c LOG_HANDLER_CONSOLE
 *                  \li \c LOG_HANDLER_FILE
 *                  \li \c LOG_HANDLER_DEBUGER
 *
 * \param name      The name of the actually log file, table, or
 *                  other specific store to use. Defaults to an
 *                  empty string, with which the subclass will
 *                  attempt to do something intelligent.
 *
 * \param ident     The identity reported to the log system.
 *
 * \param conf      A string containing any additional configuration
 *                  information that a subclass might need.
 *
 * \param level     Log messages up to and including this level.
 *
 * \return The handle of a newly created concrete Log instance, or NULL on
 *         an error.
 */
SLogSessionPtr log_factory(
    int32_t     handler,
    const char* name,
    const char* ident,
    const char* conf,
    uint32_t    priority
)
{
    int32_t         error = LOG_ERROR_SUCCESS;
    SLogSessionPtr  psession = NULL;

    LogFactoryLock();
    switch ( handler )
    {
        case LOG_HANDLER_CONSOLE:
            CHECKERR( log_console_factory( &psession,
                                          name,
                                          ident,
                                          conf,
                                          priority));
            break;
        case LOG_HANDLER_FILE:
            CHECKERR( log_file_factory( &psession,
                                         name,
                                         ident,
                                         conf,
                                         priority));
			break;
#if defined(WINDOWS)
        case LOG_HANDLER_DEBUGER:
            CHECKERR( log_debuger_factory( &psession,
                                           name,
                                           ident,
                                           conf,
                                           priority));
			break;
#endif
    } /* switch ( handler ) */
    
	if ( psession != NULL )
	{
		InitializeCriticalSection(&(psession->Lock));
		psession->SetMask  = LogSetMask;
		psession->Message  = LogMessage;
		psession->Log      = LogLog;
		psession->Close    = LogClose;
		psession->attach   = LogAttach;
		psession->MaskUpTo = LogMaskUpTo;
		psession->GetAttribute= LogGetAttribute;
		psession->MaskUpTo( psession, priority);
		psession->Flush    = LogFlush;
		if ( psession->ident ) {
			strncpy( psession->ident, ident, MAX_IDENT_SIZE );
		}
	} /* if ( handle ) */

  LogFactoryUnlock();

Error:
    PRINTERR( error);
    return psession;
} /* log_factory */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "log_delete"
/*!
 * This function close and destoys open log. It should be used only for instanecs
 * created by log_factory() function.
 *
 * \param pLog
 * Pointer to instance created by log_factory() function.
 *
 * \return
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 */
int32_t	log_delete(
	SLogSessionPtr*	pLog
)
{
	if (*pLog)
	{
		(*pLog)->Close( *pLog);
		free((*pLog)->pListeners);
		(*pLog)->pListeners = NULL;
		DeleteCriticalSection(&((*pLog)->Lock));
		free( *pLog);
		*pLog = NULL;
	}
	
	return 0;
} /* log_delete */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "log_singelton"
/*!
 * Attempts to return a reference to a concrete Log instance of type
 * \p handler, only creating a new instance if no log instance with the same
 * parameters currently exists.
 *
 * You should use this if there are multiple places you might create a
 * logger, you don't want to create multiple loggers, and you don't want to
 * check for the existance of one each time. The singleton pattern does all
 * the checking work for you.
 *
 * \param handler   The type of concrete Log subclass to return.
 *                  Attempt to dynamically include the code for
 *                  this subclass. Currently, valid values are
 *                  \li \c LOG_HANDLER_CONSOLE
 *                  \li \c LOG_HANDLER_FILE
 *                  \li \c LOG_HANDLER_DEBUGER
 *
 * \param name      The name of the actually log file, table, or
 *                  other specific store to use. Defaults to an
 *                  empty string, with which the subclass will
 *                  attempt to do something intelligent.
 *
 * \param ident     The identity reported to the log system.
 *
 * \param conf      A string containing any additional configuration
 *                  information that a subclass might need.
 *
 * \param level     Log messages up to and including this level.
 *
 * \return The handle of a newly created concrete Log instance, or NULL on
 *         an error.
 */
SLogSessionPtr log_singelton(
    int32_t     handler,
    const char* name,
    const char* ident,
    const char* conf,
    uint32_t    level
)
{
    int32_t         error = LOG_ERROR_SUCCESS;
    SLogSessionPtr  psession = NULL;
    char            signature[LOG_SIGNATURE_SIZE];

    LogFactoryLock();
  
    CHECKERR( BuildSignature( handler,
                              name,
                              ident,
                              conf,
                              level,
                              signature,
                              sizeof(signature)));

	psession = GetSingeltonSessionEntry( signature);    
	if ( psession == NULL )
	{
		psession = log_factory(handler, name, ident, conf , level);
		if ( psession != 0 ) {
			CHECKERR( LogRegisterSession( signature,
			                              psession));
		} /* if ( handle != 0 ) */
	} /* if ( handle == 0 ) */

Error:
    LogFactoryUnlock();    
    PRINTERR( error);
    return psession;
} /* log_singelton */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "log_destroy"
/*!
 * This function close all open logs. It should be used when
 * application terminated.
 *
 * \return
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 */
int32_t log_destroy(void)
{
    SLogFactorySession* plog_session_list = LogGetSessionListPtr();    
    SLogFactorySessionEntryPtr  psession = NULL;
    uint32_t i;
    
    LogFactoryLock();
    
    psession = plog_session_list->sessions;
    for(i = 0; i < plog_session_list->Count; i++, psession++) {
		if ( psession->session != 0 ) {
			psession->session->Close( psession->session);
		}
    } /* for(i = 0; i < LOG_SESSION_COUNT; i++) */

	memset(plog_session_list->sessions, 0,
           sizeof(SLogFactorySessionEntry) * plog_session_list->Count);

    LogFactoryUnlock();
    DeleteCriticalSection(&(plog_session_list->Lock));

    memset(&gs_LogSessionList, 0, sizeof(gs_LogSessionList));
    gs_pLogSessionList = NULL;
    return LOG_ERROR_SUCCESS;
} /* log_destroy */


/*****************************************************************************
 * Static no-exported functions **********************************************
 *****************************************************************************/
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LogFactoryLock"
/*!
 * This function is used to obtain a lock on the global Log session resource.
 *
 * \return	A value indicating whether an error occurred.
 *          A negative error code indicates function failure.		
 */
static int32_t LogFactoryLock(void)
{
	
#if 1	
	EnterCriticalSection(&(LogGetSessionListPtr()->Lock));
#else
	{ char mmm[256] = "";
	while ( TryEnterCriticalSection(&(LogGetSessionListPtr()->Lock)) == 0 )
	{
		sprintf(mmm,"ERROR: LOCKED "__FUNC__" [%d]................................\n", (int)GetCurrentThreadId());
		OutputDebugString(mmm);
		Sleep(10);
	}
	}
#endif
  	
	PRINTERR( 0);
	return LOG_ERROR_SUCCESS;
} /* LogFactoryLock */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LogFactoryUnlock"
/**
 * Relinquishes a lock to the resource associated with the global Log session.
 *
 * \return	A value indicating whether an error occurred.
 *          A negative error code indicates function failure.		
 *
 * \warning Call this function only when the resource is locked.
 *          See LogFactoryLock().
 */
static int32_t LogFactoryUnlock(void)
{

	LeaveCriticalSection(&(LogGetSessionListPtr()->Lock));

	PRINTERR( 0);
	return LOG_ERROR_SUCCESS;
} /* LogFactoryUnlock */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LogLock"
/**
 * This function is used to obtain a lock on the specified resource.
 *
 * \param	pLog	Pointer to resource which should be locked
 *
 * \return	A value indicating whether an error occurred.
 *          A negative error code indicates function failure.		
 */
static int32_t LogLock(
    struct _SLogSession*	pLog
)
{
	int32_t	error = LOG_ERROR_SUCCESS;

	if ( pLog == NULL )
	{
		CHECKERR( -1); /* TODO: change it */
	}
#if 1	
	EnterCriticalSection(&(pLog->Lock));
#else
	{ char mmm[256] = "";
	while ( TryEnterCriticalSection(&(pLog->Lock)) == 0 )
	{
		sprintf(mmm,"ERROR: LOCKED "__FUNC__" [%d]................................\n", (int)GetCurrentThreadId());
		OutputDebugString(mmm);
		Sleep(10);
	}
	}
#endif

Error:	
	PRINTERR( error);
	return error;
} /* LogLock */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LogUnlock"
/**
 * Relinquishes a lock to the resource associated with the given session.
 *
 * \param	pLog	Pointer to resource which should be unlocked
 *
 * \return	A value indicating whether an error occurred.
 *          A negative error code indicates function failure.		
 *
 * \warning Call this function only when the resource is locked.
 *          See LogLock().
 */
static int32_t LogUnlock(
    struct _SLogSession*	pLog
)
{
	int32_t	error = LOG_ERROR_SUCCESS;

	if ( pLog == NULL )
	{
		CHECKERR( -1); /* TODO: change it */
	}
	LeaveCriticalSection(&(pLog->Lock));

Error:	
	PRINTERR( error);
	return error;
} /* LogUnlock */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LogLog"
static int32_t _FUNCC LogLog(
    struct _SLogSession* pLog,
    uint32_t     priority,
    const char* fmt,
    ...
)
{
	int32_t            error     = LOG_ERROR_SUCCESS;
	SLogObserverPtr    pobserver = NULL;
	uint32_t			i = 0;
	va_list args;

    CHECKERR( LogLock( pLog));
    
    va_start( args, fmt );
    

    if ( IS_MASKED( pLog->m_MASK, priority) ) {
        pLog->_Log(pLog, priority, fmt, args);
    }
    
    pobserver = pLog->pListeners;
    
    for ( i = 0; pobserver && i < pLog->ListenersCount; pobserver++, i++)
    {
        if ( pobserver->Callback && IS_MASKED(pobserver->mask, priority))
        {
			
            pobserver->Callback(priority, pLog->ident, "TODO");
        }
    }

Error:	
    LogUnlock( pLog);
	PRINTERR( error);
	return error;
} /* LogLog2 */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LogMessage"
static int32_t LogMessage(
    struct _SLogSession* pLog,
    const char* message,
    uint32_t    priority
)
{
	int32_t            error = LOG_ERROR_SUCCESS;
	SLogObserverPtr    pobserver = NULL;
	uint32_t			i;

    CHECKERR( LogLock( pLog));
    
    if (IS_MASKED( pLog->m_MASK, priority)) {
        pLog->_Log(pLog, priority, message);
    }
    
    pobserver = pLog->pListeners;
    
    for ( i = 0; pobserver && i < pLog->ListenersCount; pobserver++, i++)
    {
        if ( pobserver->Callback && IS_MASKED(pobserver->mask, priority))
        {
			pobserver->Callback(priority, pLog->ident, message);
        }
    }

Error:	
    LogUnlock( pLog);
	PRINTERR( error);
	return error;
} /* LogMessage */
    


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LogClose"
static int32_t LogClose(
    struct _SLogSession* pLog
)
{
	int32_t	error = LOG_ERROR_SUCCESS;

    LogLock( pLog);
    pLog->_Close( pLog);
    LogUnlock( pLog);
    
    if ( pLog->pConfiguration ) {
        free( pLog->pConfiguration);
    }

	PRINTERR( error);
	return error;

} /* LogClose */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LogGetSessionListPtr"
/*!
 * Returns pointer on the Log Session Factory
 *
 * \return The session of a Log Session Factory, or NULL on an error.
 */
static SLogFactorySession* LogGetSessionListPtr(void) {
    if ( gs_pLogSessionList != NULL ) {
        return gs_pLogSessionList;
    } /* gs_pLogSessionList != NULL */
    
    LogInitialize();
    gs_pLogSessionList = &gs_LogSessionList;

    return gs_pLogSessionList;
} /* LogGetSessionListPtr */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LogInitialize"
/*!
 * Initielizes Log Session factory.
 * 
 * \return The handle of a newly created concrete Log instance, or NULL on
 *         an error. 
 */  
static int32_t LogInitialize(void)
{
    int32_t error = LOG_ERROR_SUCCESS;

    memset(&gs_LogSessionList, 0, sizeof(gs_LogSessionList));
    InitializeCriticalSection(&gs_LogSessionList.Lock);

    PRINTERR( error);
    return error;
} /* LogInitialize */


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "BuildSignature"
/*!
 * This function builds signature from parameters which has been passsed into
 * buffer. 
 * 
 * \return
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  If the current value of the return buffer, including the terminating NUL
 *  byte, is larger than the size you indicate in the Buffer Size parameter,
 *  the function copies characters up to last valid section, places
 *  an ASCII NUL byte at the end of the buffer, and returns the buffer size you
 *  must pass to get the entire value.  For example, if the value is "123456"
 *  and the Buffer Size is 4, the function places "123" into the buffer
 *  and returns 7.
 */ 
static int32_t BuildSignature(
    int32_t     handler,
    const char* name,
    const char* ident,
    const char* conf,
    uint32_t    level,
    char*       buffer,
    int32_t     bufferSize
)
{
	int32_t			error = LOG_ERROR_SUCCESS;
	char*          pbuffer = buffer;
    int32_t        required = 0;
    static char*   s_signatures[] = { "",
                                      "CONSOLE",
                                      "FILE",
                                      "DEBUGER"};
    if (handler < LOG_HANDLER_CONSOLE || handler > ARRAY_SIZE(s_signatures)) {
        CHECKERR( LOG_ERROR_INVALID_HANDLER);
    } /* if (handler<LOG_HANDLER_CONSOLE||handler>ARRAY_SIZE(s_signatures)) */
    
    SAFE_STR_FORMAT( "%s", s_signatures[handler], 0, bufferSize);
    SAFE_STR_FORMAT( "-%s", (( name ) ? name  : "NULL"), 1, bufferSize);
    SAFE_STR_FORMAT( "-%s", (( ident) ? ident : "NULL"), 1, bufferSize);
    SAFE_STR_FORMAT( "-%s", (( conf ) ? conf  : "NULL"), 1, bufferSize);
    SAFE_INT_FORMAT( "-%d", (int)level, 1, bufferSize);

Error:
  if ( required > bufferSize ) {
    return bufferSize;
  }
  PRINTERR( error);
  return error;
} /* BuildSignature */


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "GetSingeltonSessionEntry"
/*!
 * TODO
 * 
 *
 * \warning Call this function only when the resource is locked.
 *          See LogFactoryLock().   
 */ 
static SLogSessionPtr GetSingeltonSessionEntry(
    const char* signature
)
{
	SLogFactorySession*  plog_session_list = LogGetSessionListPtr();
	uint32_t i;
	
    for (i = 0; i < plog_session_list->Count; i++) {
        if ( plog_session_list->sessions[i].session != 0 && 
             strcmp(plog_session_list->sessions[i].signature, signature) == 0 )
		{
            return (plog_session_list->sessions[i].session);
        } /* if ( plog_session_list->sessions[i].handler == 0 ) */
    } /* for (...; i < LOG_SESSION_COUNT; ... ) */
    return NULL;
} /* GetSingeltonSessionEntry */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LogRegisterSession"
/*!
 * This function registers the session under singature in secion factory.
 * 
 * \param   signature   Signature of the session
 * \param   session     Session of new LOG instance described in signature
 * 
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.
 *  
 * \warning Call this function only when the resource is locked.
 *          See LogFactoryLock().  
 */ 
static int32_t  LogRegisterSession(
    const char* signature,
    SLogSessionPtr session
)
{
    int32_t             error = LOG_ERROR_SUCCESS;
    SLogFactorySession*    plog_session_list = LogGetSessionListPtr(); 
    
    if ( plog_session_list->AllocCount <= plog_session_list->Count ) {
        int32_t	alloc_count = plog_session_list->AllocCount + LOG_ALLOC_QUANTUM;
		SLogFactorySessionEntryPtr pnew_session = malloc( sizeof(SLogFactorySessionEntry) * alloc_count);
        
        if ( pnew_session == NULL )
		{
            CHECKERR( EL_ERROR_LOW_MEMORY);
        }
		memset( pnew_session, 0, sizeof(SLogFactorySessionEntry) * alloc_count);
        memcpy( pnew_session,  plog_session_list->sessions, plog_session_list->AllocCount);
        
        free( plog_session_list->sessions);
        plog_session_list->sessions = pnew_session;
		plog_session_list->AllocCount = alloc_count;
    }
    
	strncpy( plog_session_list->sessions[plog_session_list->Count].signature,
             signature,
             LOG_SIGNATURE_SIZE);
    plog_session_list->sessions[plog_session_list->Count].session = session;
	plog_session_list->Count += 1;
    
Error:    
    return error;
} /* LogRegisterSession */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LogSetMask"
/*!
 * Set the level mask for the current Log instance.
 *
 * \param pLog  	A handle of the log container.
 * \param mask      A bitwise mask of log levels.
 *                  Valid values are:
 *                  \li \c ELOG_EMERG
 *                  \li \c ELOG_ALERT
 *                  \li \c ELOG_CRIT
 *                  \li \c ELOG_ERR
 *                  \li \c ELOG_WARNING
 *                  \li \c ELOG_NOTICE
 *                  \li \c ELOG_INFO
 *                  \li \c ELOG_DEBUG
 *                  You can use bitwise or for example
 *                  ELOG_EMERG |  ELOG_INFO will write on output
 *                  only emergency and info messages.
 *
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 * -------------------------------\n
 * 0                      Success\n
 * Positive Values        Warnings\n
 * Negative Values        Errors\n
 */
static int32_t LogSetMask(
    struct _SLogSession*  pLog,
    uint32_t    mask
)
{
    int32_t             error = LOG_ERROR_SUCCESS;
    
    CHECKERR( LogLock( pLog));

    pLog->m_MASK = mask;

 Error:
	PRINTERR( error);
	LogUnlock( pLog);
	return error;
} /* LogSetMask */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LogMaskUpTo"
/*!
 * TODO
 *
 * \param pLog  	A handle of the log container.
 * \param priority  mask of log levels.
 *                  Valid values are:
 *                  \li \c ELOG_EMERG
 *                  \li \c ELOG_ALERT
 *                  \li \c ELOG_CRIT
 *                  \li \c ELOG_ERR
 *                  \li \c ELOG_WARNING
 *                  \li \c ELOG_NOTICE
 *                  \li \c ELOG_INFO
 *                  \li \c ELOG_DEBUG
 *
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 * -------------------------------\n
 * 0                      Success\n
 * Positive Values        Warnings\n
 * Negative Values        Errors\n
 */
static int32_t LogMaskUpTo(
	struct _SLogSession*  pLog,
    uint32_t    priority
)
{
    int32_t             error = LOG_ERROR_SUCCESS;
    uint32_t            mask = 0;
    uint32_t            i = 0;

    for( i = 0; (1UL << i) <= priority; i++)    
        mask |= (1 << i);
    
    CHECKERR( LogSetMask(pLog, mask));

 Error:
	PRINTERR( error);
	return error;
} /* LogMasUpTu */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LogGetFreeObcerver"
/*!
 * TODO
 *
 * \param pLog  	A handle of the log container.
 * \param pObcerver Returns pointer to first observer
 *
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 * -------------------------------\n
 * 0                      Success\n
 * Positive Values        Warnings\n
 * Negative Values        Errors\n
 */
static int32_t LogGetFreeObcerver(
    struct _SLogSession*  pLog,
    SLogObserverPtr*    pObserver
)
{
    int32_t         error = 0;
    SLogObserverPtr pobserver = pLog->pListeners;
	uint32_t		i;
    
    for ( i = 0; pobserver && pobserver->Callback && i < pLog->ListenersCount; pobserver++, i++)
        ; /* Do nothing */

    if ( i >= pLog->ListenersCount ) {
        SLogObserverPtr pobserver_new = NULL;
        
        pobserver = NULL;

        pobserver_new = (SLogObserverPtr)malloc( sizeof(SLogObserver) * (pLog->ListenersCount + LOG_ALLOC_LISTENERS_QUANTUM));
        CHECKALLOC( pobserver_new);
		memset(pobserver_new, 0, sizeof(SLogObserver) * (pLog->ListenersCount + LOG_ALLOC_LISTENERS_QUANTUM));
        if ( pLog->pListeners ) {
            memcpy( pobserver_new, pLog->pListeners,
                    sizeof(SLogObserver) * pLog->ListenersCount);
            free( pLog->pListeners);
        }
        pLog->pListeners = pobserver_new;
        pobserver = &(pLog->pListeners[pLog->ListenersCount]);
        pLog->ListenersCount += LOG_ALLOC_LISTENERS_QUANTUM;
    } /* if ( i >= pLog->ListenersCount ) */
    
    
Error:
    PRINTERR( error);
    *pObserver = pobserver;
    return error;
} /* LogGetFreeObcerver */


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LogAttach"
/*!
 * TODO
 *
 * \param pLog  	A handle of the log container.
 * \param pObcerver Returns pointer to first observer
 *
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 * -------------------------------\n
 * 0                      Success\n
 * Positive Values        Warnings\n
 * Negative Values        Errors\n
 */
static int32_t LogAttach(
    struct _SLogSession*  pLog,
    LogCallbackPtr callback,
    uint32_t mask
)
{
    int32_t         error = 0;
    SLogObserverPtr pobserver = NULL;
    
    CHECKERR( LogLock( pLog));
    
    CHECKERR( LogGetFreeObcerver( pLog, &pobserver));
    pobserver->Callback = callback;
    pobserver->mask = mask;

    LogUnlock( pLog);
    
Error:
    PRINTERR( error);
    return error;
} /* LogAttach */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LogGetAttribute"
/*!
 * Queries the specified attribute for a given object.
 *
 * \param pLog
 * Pointer to instance of the iButton object cretaed by ibutton_new() function.
 *
 * \param attributeID
 * Specifies the attribute whose value you want to obtain.\n
 * Attributes are:\n
 * LOG_ATTR_LOG_FILE_NAME		-	Full log file name. datat type: char[1024]
 * LOG_ATTR_LOG_FILE_NAME_PTR	-	Full log file name. datat type: const char*
 *
 * \param pValue
 * The value of the specified attribute. See \p attributeID help for data type.
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
 *  LOG_ERROR_UNKNOWN_ATTRIBUTE - Unknown attribute\n\n
 *  ERRORS:\n
 *  None	
 *
 */
static int32_t LogGetAttribute(
    struct _SLogSession*  pLog,
    uint32_t	attributeID,
    void*		pValue
)
{
	int32_t	error = 0;

	if ( pLog->_GetAttribute )
	{
		CHECKERR( pLog->_GetAttribute( pLog, attributeID, pValue));
	} /* if ( pLog->_GetAttribute ) */

Error:
	return error;
} /* LogGetAttribute */


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LogFlush"
/*!
 * This function flushes the Log buffer of the specified stream.
 *
 * \param pLog
 * Pointer to instance of the iButton object cretaed by ibutton_new() function.
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
 *  None\n
 *  ERRORS:\n
 *  None	
 *
 */
static int32_t	LogFlush(
	 struct _SLogSession*  pLog
)
{
	int32_t	error = 0;

	LogLock( pLog);
	if ( pLog->_Flush != NULL)
	{
		pLog->_Flush( pLog);
	}

/* Error: */
    LogUnlock( pLog);
	PRINTERR( error);
	return error;

} /* LogFlush */

