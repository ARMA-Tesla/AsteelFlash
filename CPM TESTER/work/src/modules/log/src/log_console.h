/******************************************************************************
 *
 ******************************************************************************/

#if !defined(__LOG_CONSOLE_H__) 
#define __LOG_CONSOLE_H__

#include <log.h>

int32_t log_console_factory(
    SLogSessionPtr* session,
    const char* name,
    const char* ident,
    const char* conf,
    uint32_t    priority
);

#endif /* __LOG_CONSOLE_H__ */
