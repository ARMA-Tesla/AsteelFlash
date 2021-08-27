/******************************************************************************
 *  Log.h
 *
 *  Author: $Author: rykalaz $
 *
 * Revision History:
 * $Log: log.h,v $
 * Revision 1.4  2006/01/08 22:05:42  rykalaz
 * Added check for  ARRAYSIZE
 *
 * Revision 1.3  2005/11/09 09:24:50  pisalak
 * Added function log_delete
 * Added Flush method
 *
 * Revision 1.2  2005/11/02 09:57:44  pisalak
 * *** empty log message ***
 *
 *
 ******************************************************************************/

#if !defined(__LOG_H__) 
#define __LOG_H__

#include <eltypes.h>
#include <stdarg.h>
#if defined(WINDOWS)
#include <windows.h>
#else
#include <pthread.h>
#endif

/*
 * LOG levels
 */ 
#define ELOG_EMERG    (1UL << 0)  /*!< System is unusable */
#define ELOG_ALERT    (1UL << 1)  /*!< Immediate action required */
#define ELOG_CRIT     (1UL << 2)  /*!< Critical conditions */
#define ELOG_ERR      (1UL << 3)  /*!< Error conditions */
#define ELOG_WARNING  (1UL << 4)  /*!< Warning conditions */
#define ELOG_NOTICE   (1UL << 5)  /*!< Normal but significant */
#define ELOG_INFO     (1UL << 6)  /*!< Informational */
#define ELOG_DEBUG    (1UL << 7)  /*!< Debug-level messages */
#define ELOG_ALL      (0xFFFFFFFFUL)  /*! All messages */
#define ELOG_NONE     (0x0UL)     /*!< No message */

#if !defined( LOG_MAX_MESSAGE_LENGTH)
#define LOG_MAX_MESSAGE_LENGTH		4096
#endif /* LOG_MAX_MESSAGE_LENGTH */

#define MAX_IDENT_SIZE			16

#define IS_MASKED( mask, priority)  ((mask) & (priority))

typedef int32_t (_FUNCC *LogCallbackPtr)( uint32_t  priority, const char* ident, const char* message);


typedef struct _SLogObserver{
	uint32_t    mask;
	LogCallbackPtr		Callback; /*!< This callback is called log session is logged with specified masked priority */
} SLogObserver, *SLogObserverPtr;

typedef struct _SLogSession{
  CRITICAL_SECTION	Lock;        /*!< The lock of the session */
  uint32_t          m_MASK;     /*!< The bitmask of allowed log levels. */
  void*             pConfiguration; /*!< Session Configurations */
  SLogObserverPtr   pListeners; /*!< Holds all Log_observer objects that wish to be notified of new messages. */
  uint32_t          ListenersCount; /*!< Number of allocated listeners */
  char				ident[MAX_IDENT_SIZE];

  int32_t (*Message)(struct _SLogSession* me, const char* pMessage, uint32_t  priority);
  int32_t (_FUNCC *Log)(struct _SLogSession* me, uint32_t  priority, const char* fmt, ...);
  int32_t (*Close)(struct _SLogSession* me);

  
  int32_t (*SetMask)(struct _SLogSession* me, uint32_t  mask);
  int32_t (*MaskUpTo)(struct _SLogSession* me, uint32_t  priority);
  int32_t (*attach)(struct _SLogSession* me,
                     int32_t (_FUNCC *Callback)( uint32_t  priority, const char* ident, const char* message), uint32_t mask);
	int32_t (*GetAttribute)(struct _SLogSession* me, uint32_t  attribute, void*	pValue);
	int32_t	(*Flush)(struct _SLogSession* me);
  
/*
 * Private
 */
	int32_t (_FUNCC *_Log)(struct _SLogSession* me, uint32_t  priority, const char* fmt, ...);
	int32_t (*_GetAttribute)(struct _SLogSession* me, uint32_t  attribute, void*	pValue);
	int32_t (*_Close)(struct _SLogSession* me);
	int32_t (*_Flush)(struct _SLogSession* me);
  
} SLogSession, *SLogSessionPtr;


/*
 * LOG handlers
 */ 
enum {
    LOG_HANDLER_CONSOLE = 1,
    LOG_HANDLER_FILE,
    LOG_HANDLER_DEBUGER
};

SLogSessionPtr log_factory(
    int32_t     handler,
    const char* name,
    const char* ident,
    const char* conf,
    uint32_t    level
);

SLogSessionPtr log_singelton(
    int32_t     handler,
    const char* name,
    const char* ident,
    const char* conf,
    uint32_t    level
);

int32_t log_destroy(void);
int32_t	log_delete(SLogSessionPtr* pLog);

#define LOG_ATTR_LOG_FILE_NAME				1001
#define LOG_ATTR_LOG_FILE_NAME_PTR			1002



#define LOG_ERROR_SUCCESS                  (0)
#define LOG_ERROR_INVALID_HANDLER          (-1000000)
#define LOG_ERROR_UNKNOWN_ATTRIBUTE		   (-1000001)

#if !defined(ARRAY_SIZE)
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))
#endif

#endif /* __LOG_H__ */
