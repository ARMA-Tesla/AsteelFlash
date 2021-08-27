#include <slog.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#define	MAX_STRING_LENGTH		256
/*!
 * Defines max size of priority name + 3 character which means "[] "
 */ 
#define MAX_PRIORITY_LENGTH     16

static const char* GetPriorityString(uint32_t priority);
static int32_t	LogIsMasked( uint32_t priority);

int32_t slog_initialize(void)
{
    return LOG_ERROR_SUCCESS;
} /* slog_initialize */

/**
 * This function close all open logs. It should be used when
 * application terminated.
 * @return The handle of a newly created concrete Log instance, or 0 on an error.
 */
int32_t slog_destroy(void)
{
    return LOG_ERROR_SUCCESS;
} /* slog_destroy */

/*!
 * Formats and writes a message to log system.
 *
 * @param priority	The priority of the message.  Valid
 *                  values are:
 *                  \li \c ELOG_EMERG
 *                  \li \c ELOG_ALERT
 *                  \li \c ELOG_CRIT
 *                  \li \c ELOG_ERR
 *                  \li \c ELOG_WARNING
 *                  \li \c ELOG_NOTICE
 *                  \li \c ELOG_INFO
 *                  \li \c ELOG_DEBUG
 *
 * @param fmt		Contains the format string that specifies how subsequent
 *                  arguments are converted for output.  If insufficient arguments
 *                  exist for the format, the behavior is undefined.  If the format
 *                  is exhausted while arguments remain, the excess arguments are
 *                  evaluated but are otherwise ignored.
 *                  
 *                  Note:  Refer to documentation of this ANSI C Standard Library
 *                  function for further information on these formatting strings.
 *
 * @param ...		This specifies one or more arguments that are to be converted
 *                  and written to the standard output.  If there are multiple
 *                  arguments, they must be separated by commas.
 *
 * @return ???
 */
int32_t _FUNCC slog_msg(
    int32_t     priority,
    const char* fmt,
    ...
)
{
    char	tmp_message[MAX_STRING_LENGTH] = "";
    char	message[MAX_STRING_LENGTH] = "";
    char*   pdynamic_message = NULL;
    char*	pmessage = message;
    va_list args;


    va_start( args, fmt );
    
    if ( LogIsMasked( priority) ) {
#if 0        
		int len;
        len = _vscprintf( fmt, args ) /* _vscprintf doesn't count */
                                    + 1; /* terminating '\0' */
        if ( (len + MAX_PRIORITY_LENGTH) > MAX_STRING_LENGTH ) {
            pdynamic_message = malloc((len+MAX_PRIORITY_LENGTH)*sizeof(char) );
            pmessage         = pdynamic_message;
            /*
             * TODO: Check NULL pointer
             */             
        }
#endif
        pmessage += sprintf( pmessage,
                             "[%s] ",
                             GetPriorityString(priority));
        pmessage += vsprintf( pmessage, 
                              fmt,
                              args);
		*pmessage++ = '\n';
		*pmessage++ = '\0';
       pmessage = ( pdynamic_message ) ? pdynamic_message : message;
	   OutputDebugString(pmessage);
    } /* if ( LogIsMasked( pConfiguration->mask, priority) ) */


    free( pdynamic_message);
    return LOG_ERROR_SUCCESS;
}

/**
 * Set the level mask for the current Log instance.
 *
 * @param handle	A handle of the log container.
 * @param mask      A bitwise mask of log levels.
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
 * @return ???
 */
int32_t log_mask(
    uint32_t    mask
)
{
    return LOG_ERROR_SUCCESS;
}


/*****************************************************************************
 * Static no-exported functions **********************************************
 *****************************************************************************/
static const char* GetPriorityString(uint32_t priority) {
    static char*    def_string = "";
    static char*    s_priority_str[] = {
        "EMERG",
        "ALERT",
        "CRIT",
        "ERROR",
        "WARNING",
        "NOTICE",
        "INFO",
        "DEBUG",
        NULL
    };
    int	i;
    
    for( i = 0; s_priority_str[i]; i++ ) {
        if ( priority & (1 << i)) {
            return s_priority_str[i];
        } /* if ( priority & (1 << i)) */
    }
    
    return def_string;
}

static int32_t	LogIsMasked( uint32_t priority) {
	return 1;
}
