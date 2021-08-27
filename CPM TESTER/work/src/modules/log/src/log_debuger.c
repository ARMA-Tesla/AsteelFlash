
#include <log/log_debuger.h>
#include <log/log_utils.h>
#include <elerror.h> /* malloc */
#include <stdlib.h> /* malloc */
#include <stdio.h>
#include <time.h> /* time */

#define MAX_FORMAT_STRING       64
#define MAX_TIME_STRING         128

typedef struct {
	uint32_t	mask;
    char        lineFormat[MAX_FORMAT_STRING];
    char        timeFormat[MAX_FORMAT_STRING];
} SDebugerConfiguration;

enum {
    LOG_STDOUT,
    LOG_STDERR
};

#define DEFAULT_LINE_FORMAT  "%{ident} [%{priority}] %{message}"
#define DEFAULT_TIME_FORMAT  "%b %d %H:%M:%S"

static int32_t LogDebugerParseConfiguration(
    SDebugerConfiguration*  pConf,
    const char*             pConfStr
);
static int32_t LogDebugerSetDefaultConfiguration(
    SDebugerConfiguration*  pConf
);
static int32_t _FUNCC LogDebugerLog(
    struct _SLogSession* pLog,
    uint32_t     priority,
    const char* fmt,
    ...
);


/*****************************************************************************
 * Exported functions definition *********************************************
 *****************************************************************************/
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "log_console_factory"
/*!
 * Attempts to return a concrete Log instance of debuger type.
 *
 * \param pSession  Pointer to session which will be created.
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
int32_t log_debuger_factory(
    SLogSessionPtr* pSession,
    const char* name,
    const char* ident,
    const char* pConf,
    uint32_t    priority
)
{
    int32_t                 error = LOG_ERROR_SUCCESS;
    SLogSessionPtr          psession = *pSession;
    SDebugerConfiguration*  pConfiguration = NULL;

    if ( psession == NULL ) {
        psession = (SLogSessionPtr) malloc(sizeof(SLogSession));
        CHECKALLOC( psession);
        memset( psession, 0, sizeof(SLogSession));
    }  /* if ( psession == NULL ) */
        
    pConfiguration = (SDebugerConfiguration*) malloc( sizeof(SDebugerConfiguration));
    CHECKALLOC( pConfiguration);
    CHECKERR( LogDebugerParseConfiguration(pConfiguration, pConf));
    
    
    psession->_Log            = LogDebugerLog;
    psession->_Close          = NULL;
    psession->pConfiguration = (void *)pConfiguration;

Error:
    PRINTERR( error);
    if ( error <  0 ) {
        free( psession);
        free( pConfiguration);
        psession = NULL;
    }
    *pSession = psession;
    return error;
} /* log_debuger_factory */


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LogDebugerLog"
static int32_t _FUNCC LogDebugerLog(
    struct _SLogSession* pLog,
    uint32_t     priority,
    const char* fmt,
    ...
)
{
    int32_t error = LOG_ERROR_SUCCESS;
    char	message[LOG_MAX_MESSAGE_LENGTH];
    char*	pmessage = message;
    int32_t output;
    SDebugerConfiguration*  pconf = (SDebugerConfiguration*)(pLog->pConfiguration);
    const char*             pformat = pconf->lineFormat;
    char                    character;
    va_list argptr;
                
    va_start( argptr, fmt );     /* Initialize variable arguments. */

    while ((output = WriteToOutput(&pformat, &character)) != -1 ) {
        switch ( output )
        {
            case LOG_OUTPUT_TIMESTAMP:
            {
            	time_t      ltime;
    			struct tm *today;
    			char tmpbuf[MAX_TIME_STRING];
			
                time( &ltime );
                today = localtime( &ltime );

                /* Use strftime to build a customized time string. */
                strftime( tmpbuf, MAX_TIME_STRING, pconf->timeFormat, today );
                pmessage += sprintf(pmessage, tmpbuf);
            }
            break;
            case LOG_OUTPUT_IDENT:
                pmessage += sprintf(pmessage, pLog->ident);
                break;
            case LOG_OUTPUT_PRIORITY:
                pmessage += sprintf(pmessage, LogUtilsTranslatePriority(priority));
                break;
            case LOG_OUTPUT_MESSGAE:
                pmessage += vsprintf(pmessage, fmt,argptr);
                break;
            case LOG_OUTPUT_CHARACTER:
                *pmessage++ = character;
                break;
            
        } /* switch ( output ) */
    } /* while ((output = WriteToOutput()) != -1 ) */
	pmessage += sprintf(pmessage, "\r\n");
	OutputDebugString(message);
    
/* Error: */
    PRINTERR( error);
	return error;
} /* LogDebugerLog */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LogDebugerSetDefaultConfiguration"
static int32_t LogDebugerSetDefaultConfiguration(
    SDebugerConfiguration*  pConf
)
{
    int32_t error = 0;
   
    memset(pConf, 0, sizeof(SDebugerConfiguration));
    strcpy(pConf->lineFormat, DEFAULT_LINE_FORMAT);
    strcpy(pConf->timeFormat, DEFAULT_TIME_FORMAT);

    PRINTERR( error);
    return error;
} /* LogDebugerSetDefaultConfiguration */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LogDebugerParseConfiguration"
static int32_t LogDebugerParseConfiguration(
    SDebugerConfiguration*  pConf,
    const char*             pConfStr
)
{
    int32_t error = 0;

    CHECKERR( LogDebugerSetDefaultConfiguration( pConf));
    
Error:
    PRINTERR( error);
    return error;
} /* LogDebugerParseConfiguration */
