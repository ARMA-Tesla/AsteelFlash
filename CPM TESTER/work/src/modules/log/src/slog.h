/******************************************************************************
 *  slog.h
 *
 *  Author: $Author: pisalak $
 *
 * Revision History:
 * $Log: slog.h,v $
 * Revision 1.2  2005/11/02 09:57:44  pisalak
 * *** empty log message ***
 *
 * Revision 1.1  2005/08/10 13:34:30  rykalaz
 * Added
 *
 *
 ******************************************************************************/

#if !defined(__SLOG_H__) 
#define __SLOG_H__

#include <eltypes.h>
#include <stdarg.h>

#ifdef __cplusplus
    extern "C" {
#endif 

/*
 * LOG levels
 */ 
#define ELOG_EMERG    (1UL << 0)
#define ELOG_ALERT    (1UL << 1)
#define ELOG_CRIT     (1UL << 2)
#define ELOG_ERR      (1UL << 3)
#define ELOG_WARNING  (1UL << 4)
#define ELOG_NOTICE   (1UL << 5)
#define ELOG_INFO     (1UL << 6)
#define ELOG_DEBUG    (1UL << 7)
#define ELOG_ALL      (0xFFFFFFFFUL)


#define LOG_ERROR_SUCCESS                  (0)



#define SLOG_BEGIN(lev, fmt)    slog_msg(lev, fmt
#define SLOG_P(parameter)       ,parameter
#define SLOG_END()              );

#define SLOG_EMERG( message)      slog_msg(ELOG_EMERG, message)
#define SLOG_ALERT( message)    slog_msg(ELOG_ALERT, message)
#define SLOG_CRIT( message)     slog_msg(ELOG_CRIT, message)
#define SLOG_ERR( message)      slog_msg(ELOG_ERR, message)
#define SLOG_WARNING( message)  slog_msg(ELOG_WARNING, message)
#define SLOG_NOTICE( message)   slog_msg(ELOG_NOTICE, message)
#define SLOG_INFO( message)     slog_msg(ELOG_INFO, message)
#define SLOG_DEBUG( message)    slog_msg(ELOG_DEBUG, message)


int32_t slog_initialize(void);
int32_t slog_destroy(void);

int32_t _FUNCC slog_msg(
    int32_t     priority,
    const char* fmt,
    ...
);

int32_t slog_mask(
    uint32_t    mask
);

#ifdef __cplusplus
    }
#endif 
#endif /* __LOG_H__ */
