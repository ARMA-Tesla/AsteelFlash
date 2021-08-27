
#include <log/log_console.h>
#include <log/log_utils.h>
#include <stdlib.h> /* malloc */
#include <elerror.h>
#include <stdio.h>
#include <string.h>
#include <time.h> /* time */

#define MAX_FORMAT_STRING       64
#define MAX_TIME_STRING         128

typedef struct {
    uint32_t	mask;
    uint32_t    stram;
    char        lineFormat[MAX_FORMAT_STRING];
    char        timeFormat[MAX_FORMAT_STRING];
} SConsoleConfiguration;

enum {
    LOG_STDOUT,
    LOG_STDERR
};

#define DEFAULT_LINE_FORMAT  "%{ident} [%{priority}] %{timestamp} %{message}"
#define DEFAULT_TIME_FORMAT  "%b %d %H:%M:%S"

/*****************************************************************************
 * Static no-exported functions declaration **********************************
 *****************************************************************************/
static int32_t LogConsoleSetDefaultConfiguration(
    SConsoleConfiguration*  pConf
);
static int32_t LogConsoleParseConfiguration(
    SConsoleConfiguration*  pConf,
    const char*             pConfStr
);
static int32_t _FUNCC LogConsoleLog(
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
 * Attempts to return a concrete Log instance of console type.
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
int32_t log_console_factory(
    SLogSessionPtr* pSession,
    const char* name,
    const char* ident,
    const char* pConf,
    uint32_t    priority
)
{
	int32_t					error = LOG_ERROR_SUCCESS;
	SLogSessionPtr          psession = *pSession;
    SConsoleConfiguration*  pConfiguration = NULL;

    if ( psession == NULL ) {
        psession = (SLogSessionPtr) malloc(sizeof(SLogSession));
        CHECKALLOC( psession);
        memset( psession, 0, sizeof(SLogSession));
    }  /* if ( psession == NULL ) */
        
    pConfiguration = (SConsoleConfiguration*) malloc( sizeof(SConsoleConfiguration));
    CHECKALLOC( pConfiguration);
    CHECKERR( LogConsoleParseConfiguration(pConfiguration, pConf));
    
    
    psession->_Log            = LogConsoleLog;
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
} /* log_console_factory */


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LogConsoleLog"
static int32_t _FUNCC LogConsoleLog(
    struct _SLogSession* pLog,
    uint32_t     priority,
    const char* fmt,
    ...
)
{
    int32_t     error = LOG_ERROR_SUCCESS;
    int32_t     output;
    int32_t     written, written_count = 0;
    time_t      ltime;
    struct tm *today;
    char tmpbuf[MAX_TIME_STRING];
    SConsoleConfiguration*  pconf   = NULL;
    const char*             pformat = NULL;
    char                    character;
    va_list argptr;
                
    va_start( argptr, fmt );     /* Initialize variable arguments. */

    pconf = (SConsoleConfiguration*)(pLog->pConfiguration);
    pformat = pconf->lineFormat;
    

    while ((output = WriteToOutput(&pformat, &character)) != -1 ) {
        switch ( output )
        {
            case LOG_OUTPUT_TIMESTAMP:
            {
                time( &ltime );
                today = localtime( &ltime );

                /* Use strftime to build a customized time string. */
                strftime( tmpbuf, MAX_TIME_STRING, pconf->timeFormat, today );
                printf(tmpbuf);
            }
            break;
            case LOG_OUTPUT_IDENT:
                written = printf(pLog->ident);
                break;
            case LOG_OUTPUT_PRIORITY:
                printf(LogUtilsTranslatePriority(priority));
                break;
            case LOG_OUTPUT_MESSGAE:
                vprintf(fmt,argptr);
                break;
            case LOG_OUTPUT_CHARACTER:
                printf("%c", character);
                break;
            
        } /* switch ( output ) */
    } /* while ((output = WriteToOutput()) != -1 ) */
    printf("\n");
    
/* Error: */
    PRINTERR( error);
	return error;
} /* LogConsoleLog */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LogConsoleSetDefaultConfiguration"
static int32_t LogConsoleSetDefaultConfiguration(
    SConsoleConfiguration*  pConf
)
{
    int32_t error = 0;
   
    memset(pConf, 0, sizeof(SConsoleConfiguration));
    pConf->stram = LOG_STDOUT;
    strcpy(pConf->lineFormat, DEFAULT_LINE_FORMAT);
    strcpy(pConf->timeFormat, DEFAULT_TIME_FORMAT);

    PRINTERR( error);
    return error;
} /* LogConsoleSetDefaultConfiguration */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LogConsoleParseConfiguration"
static int32_t LogConsoleParseConfiguration(
    SConsoleConfiguration*  pConf,
    const char*             pConfStr
)
{
    int32_t error = 0;

    CHECKERR( LogConsoleSetDefaultConfiguration( pConf));
    
Error:
    PRINTERR( error);
    return error;
} /* LogConsoleParseConfiguration */
