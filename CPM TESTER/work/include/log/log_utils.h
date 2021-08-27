/******************************************************************************
 *
 ******************************************************************************/

#if !defined(__LOG_UTILS_H__) 
#define __LOG_UTILS_H__

#include <log.h>

uint32_t LogUPTO(uint32_t priority);
uint8_t LogIsMasked(uint32_t mask, uint32_t priority);
const char* LogUtilsTranslatePriority(
	uint32_t	priority
);


int32_t WriteToOutput(
    const char** pFormatString,
    char*   character
);

enum {
    LOG_OUTPUT_TIMESTAMP,
    LOG_OUTPUT_IDENT,
    LOG_OUTPUT_PRIORITY,
    LOG_OUTPUT_MESSGAE,
    LOG_OUTPUT_CHARACTER
} ELogOutputsParameters;

#endif /* __LOG_UTILS_H__ */
