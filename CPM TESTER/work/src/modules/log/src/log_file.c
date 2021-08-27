
#include <log/log_file.h>
#include <log/log_utils.h>
#include <elerror.h> /* malloc */
#include <stdlib.h> /* malloc */
#include <stdio.h>
#include <string.h>
#include <time.h> /* time */

#define MAX_FORMAT_STRING       64
#define MAX_TIME_STRING         128
#define	LOG_FILE_MAX_PATH_NAME		1024

#define CHECKFILE( fCal) \
    if ( error = (fCal), error < 0 ) goto Error; else

typedef struct {
    FILE*       stream;				/*!< stream handle to log file */
    char		FileName[LOG_FILE_MAX_PATH_NAME+1];		/*!< Full path of the log file */
    uint32_t	mask;				/*!< Mask */
    char        lineFormat[MAX_FORMAT_STRING];
    char        timeFormat[MAX_FORMAT_STRING];
    uint8_t     append;				/*!< Append or truncate lig file, wghen instance is created */
} SFileConfiguration;

enum {
    LOG_STDOUT,
    LOG_STDERR
};

#define DEFAULT_LINE_FORMAT  "%{ident} [%{priority}] %{timestamp} %{message}"
#define DEFAULT_TIME_FORMAT  "%b %d %H:%M:%S"

static int32_t _FUNCC LogFileLog(
    struct _SLogSession* pLog,
    uint32_t     priority,
    const char* fmt,
    ...
);
static int32_t LogFileClose(
    struct _SLogSession* pLog
);
static int32_t LogFileFlush(
    struct _SLogSession* pLog
);
static int32_t LogFileSetDefaultConfiguration(
    SFileConfiguration*  pConf
);
static int32_t LogFileParseConfiguration(
    SFileConfiguration*  pConf,
    const char*             pConfStr
);
static int32_t LogFileGetAttribute(
    struct _SLogSession*	pLog,
    uint32_t				attributeID,
    void*					pValue
);


/*****************************************************************************
 * Exported functions definition *********************************************
 *****************************************************************************/
/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "log_file_factory"
/*!
 * Attempts to return a concrete Log instance of file type.
 *
 * \param pLog  Pointer to session which will be created.
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
int32_t log_file_factory(
    SLogSessionPtr* pLog,
    const char* name,
    const char* ident,
    const char* pConf,
    uint32_t    priority
)
{
    int32_t                 error = LOG_ERROR_SUCCESS;
    SLogSessionPtr          psession = *pLog;
    SFileConfiguration*     pConfiguration = NULL;

    if ( psession == NULL ) {
        psession = (SLogSessionPtr) malloc(sizeof(SLogSession));
        CHECKALLOC( psession);
        memset( psession, 0, sizeof(SLogSession));
    }  /* if ( psession == NULL ) */
    
    pConfiguration = (SFileConfiguration*) malloc( sizeof(SFileConfiguration));
    CHECKALLOC( pConfiguration);
	CHECKERR( LogFileParseConfiguration(pConfiguration, pConf));
    
	pConfiguration->stream = fopen(name, (pConfiguration->append) ? "a+" :"w+");
	strncpy( pConfiguration->FileName, name, LOG_FILE_MAX_PATH_NAME);
    
    psession->_Log            = LogFileLog;
    psession->_Close          = LogFileClose;
    psession->_GetAttribute   = LogFileGetAttribute;
    psession->_Flush          = LogFileFlush;
    psession->pConfiguration = (void *)pConfiguration;

Error:
    PRINTERR( error);
    if ( error <  0 ) {
        free( psession);
        free( pConfiguration);
        psession = NULL;
    }
    *pLog = psession;
    return error;
} /* log_file_factory */


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LogFileLog"
static int32_t _FUNCC LogFileLog(
    struct _SLogSession* pLog,
    uint32_t     priority,
    const char* fmt,
    ...
)
{
    int32_t     error = LOG_ERROR_SUCCESS;
    int32_t     output;
    time_t      ltime;
    struct tm *today;
    char tmpbuf[MAX_TIME_STRING];
    SFileConfiguration*  pconf = (SFileConfiguration*)(pLog->pConfiguration);
    const char*             pformat = pconf->lineFormat;
    char                    character;
    va_list argptr;
                
	if ( pconf->stream == NULL )
	{
		CHECKERR( -1);
	}

	va_start( argptr, fmt );     /* Initialize variable arguments. */
	
    while ((output = WriteToOutput(&pformat, &character)) != -1 ) {
        switch ( output )
        {
            case LOG_OUTPUT_TIMESTAMP:
            {
                time( &ltime );
                today = localtime( &ltime );

                /* Use strftime to build a customized time string. */
                strftime( tmpbuf, MAX_TIME_STRING, pconf->timeFormat, today );
                CHECKFILE( fprintf(pconf->stream, tmpbuf));
            }
            break;
            case LOG_OUTPUT_IDENT:
                CHECKFILE( fprintf(pconf->stream, pLog->ident));
                break;
            case LOG_OUTPUT_PRIORITY:
                CHECKFILE( fprintf(pconf->stream, LogUtilsTranslatePriority(priority)));
                break;
            case LOG_OUTPUT_MESSGAE:
                CHECKFILE( vfprintf(pconf->stream,fmt,argptr));
                break;
            case LOG_OUTPUT_CHARACTER:
                CHECKFILE( fprintf(pconf->stream, "%c", character));
                break;
            
        } /* switch ( output ) */
    } /* while ((output = WriteToOutput()) != -1 ) */
    CHECKFILE( fprintf(pconf->stream,"\n"));
    if ( priority <= ELOG_ERR )
    {
	fflush(pconf->stream);
	}
Error:
    PRINTERR( error);	
    return error;
} /* LogFileLog */


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LogFileFlush"
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
 *  None\n
 *
 */
static int32_t LogFileFlush(
    struct _SLogSession* pLog
)
{
	int32_t     error = 0;
	SFileConfiguration*  pconf = (SFileConfiguration*)(pLog->pConfiguration);
	fflush(pconf->stream);

/* Error: */
    PRINTERR( error);	
    return error;
}


/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LogFileClose"
static int32_t LogFileClose(
    struct _SLogSession* pLog
)
{
    if ( pLog->pConfiguration != NULL ) {
    	SFileConfiguration*  pconf = (SFileConfiguration*)(pLog->pConfiguration);
        if ( pconf->stream != NULL) {
            fclose(pconf->stream);
            pconf->stream = NULL;
        } /* if ( pLog->conf->stream ) */
        free(pLog->pConfiguration);
        pLog->pConfiguration = NULL;
    } /* if ( session->conf != NULL ) */

    return LOG_ERROR_SUCCESS;
} /* LogFileClose */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LogFileSetDefaultConfiguration"
static int32_t LogFileSetDefaultConfiguration(
    SFileConfiguration*  pConf
)
{
    int32_t error = 0;
   
    memset(pConf, 0, sizeof(SFileConfiguration));
    strcpy(pConf->lineFormat, DEFAULT_LINE_FORMAT);
    strcpy(pConf->timeFormat, DEFAULT_TIME_FORMAT);
    pConf->append = 1;

    PRINTERR( error);
    return error;
} /* LogFileSetDefaultConfiguration */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LogFileParseConfiguration"
static int32_t LogFileParseConfiguration(
    SFileConfiguration*  pConf,
    const char*             pConfStr
)
{
    int32_t error = 0;

    CHECKERR( LogFileSetDefaultConfiguration( pConf));
    
Error:
    PRINTERR( error);
    return error;
} /* LogFileParseConfiguration */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "LogFileGetAttribute"
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
static int32_t LogFileGetAttribute(
    struct _SLogSession*	pLog,
    uint32_t				attributeID,
    void*					pValue
)
{
	int32_t	error = 0;
	SFileConfiguration*  pconf = (SFileConfiguration*)(pLog->pConfiguration);
	switch ( attributeID )
	{
	case LOG_ATTR_LOG_FILE_NAME:
		strncpy((char*)pValue, pconf->FileName, LOG_FILE_MAX_PATH_NAME);
		break;
	case LOG_ATTR_LOG_FILE_NAME_PTR:
		pValue = (void*)((const char*)(pconf->FileName));
		break;
	default:
		error = LOG_ERROR_UNKNOWN_ATTRIBUTE;
		break;
	}
	return error;	
}
