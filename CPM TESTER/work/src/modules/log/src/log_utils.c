#include <log/log_utils.h>
#include <string.h>

typedef struct {
	int	value;
	const char*	string;
} SOutputIDs;

static SOutputIDs gs_formatIDs[] = {
    {LOG_OUTPUT_TIMESTAMP,"timestamp"},
    {LOG_OUTPUT_IDENT,    "ident"},
    {LOG_OUTPUT_PRIORITY, "priority"},
    {LOG_OUTPUT_MESSGAE,  "message"},
    {0, NULL}
};


 /**
  * Calculate the log mask for all priorities up to the given priority.
  *
  * @param priority   The maximum priority covered by this mask.
  *
  * @return The calculated log mask.
  */
uint32_t LogUPTO(uint32_t priority)
{
	return (priority > 0 ) ? ((priority - 1) | priority) : 0;
}

/**
 * Check if the given priority is included in the current level mask.
 *
 * @param priority   The priority to check.
 *
 * @return True if the given priority is included in the current
 *                  log mask.
 */
uint8_t LogIsMasked(uint32_t mask, uint32_t priority)
{
	return (uint8_t)((mask & priority)!= 0);
}

int32_t WriteToOutput(
    const char** pFormatString,
    char*   character
)
{
    int32_t     output = LOG_OUTPUT_CHARACTER;
    const char* pbegin = *pFormatString;
    int			i;
    
    if ( *pbegin == 0 ) {
		return -1;
    } /* if ( *pbegin == 0 ) */
    *character = *pbegin;
    if (*pbegin == '%' && *(pbegin+1)=='{') {
        const char* pend =  (const char*)strchr(pbegin, '}');
        if ( pend != NULL ) {
            char    tmpbuffer[32];
            int32_t    count = (int32_t)(pend-(pbegin+2));
            count = (count >= ARRAY_SIZE(tmpbuffer)) ? ARRAY_SIZE(tmpbuffer) - 1 : count;
            memcpy(tmpbuffer, pbegin+2, count);
            tmpbuffer[count] = '\0';
            
            for(i = 0; gs_formatIDs[i].string; i++ ) {
                if (strcmp( gs_formatIDs[i].string, tmpbuffer) == 0 ) {
                   break; 
                } /* if gs_formatids[i] == tmpbuffer */
            } /* for(i = 0; gs_formatids[i]; i++ ) */
            if ( gs_formatIDs[i].string ) {
                pbegin = pend + 1;
                output = gs_formatIDs[i].value;
            }
            else {
                output = LOG_OUTPUT_CHARACTER;
            }
        } /* if ( pend != NULL ) */
    } /* if (*pbegin == '%') */
    else
    {
        pbegin++;
    }

    *pFormatString = pbegin; 
    return output;
} /* WriteToOutput */


typedef struct {
	uint32_t	priority;
	const char*	text;
} SLogPriorityTranslatorEntry;

#define DPE( val) { ELOG_##val, #val }

static SLogPriorityTranslatorEntry gs_LogTransaltor[] = {
	DPE( EMERG ),
	DPE( ALERT ),
	DPE( CRIT ),
	DPE( ERR ),
	DPE( WARNING ),
	DPE( NOTICE ),
	DPE( INFO ),
	DPE( DEBUG ),
	{0, NULL}
};


const char* LogUtilsTranslatePriority(
	uint32_t	priority
)
{
	int i;
	static char* s_unknown = "UNKNOWN";

	for( i = 0; gs_LogTransaltor[i].text; i++)
	{
		if (gs_LogTransaltor[i].priority == priority) {
			return gs_LogTransaltor[i].text;
		}
	}
	return s_unknown;
}