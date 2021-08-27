/******************************************************************************
 *
 ******************************************************************************/

#if !defined(__LOG_DEBUGER_H__) 
#define __LOG_DEBUGER_H__

#include <log.h>

int32_t log_debuger_factory(
    SLogSessionPtr* session,
    const char* name,
    const char* ident,
    const char* conf,
    uint32_t    level
);

#endif /* __LOG_DEBUGER_H__ */
