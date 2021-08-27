/******************************************************************************
 *  Log.h
 ******************************************************************************/

#if !defined(__LOG_H__) 
#define __LOG_H__

#include <eltypes.h>

 /* Priority identifiers */
#define LOG_EMERG				(1UL << 0)  /*!< System is unusable */
#define LOG_ALERT				(1UL << 1)  /*!< Immediate action required */
#define LOG_CRIT				(1UL << 2)  /*!< Critical conditions */
#define LOG_ERR					(1UL << 3)  /*!< Error conditions */
#define LOG_WARNING				(1UL << 4)  /*!< Warning conditions */
#define LOG_NOTICE				(1UL << 5)  /*!< Normal but significant */
#define LOG_INFO				(1UL << 6)  /*!< Informational */
#define LOG_DEBUG				(1UL << 7)  /*!< Debug-level messages */
#define LOG_COMMUNICATION		(1UL << 8)  /*!< Communication messages */
#define LOG_TRACE				(1UL << 9)  /*!< Trace function messages */
#define LOG_NUMBER_OF_PRIORITIES		10
#define LOG_ALL					(0xFFFFFFFFUL)  /*! All messages */
#define LOG_NONE				(0x0UL)     /*!< No message */

#ifdef DEFINE_LOG_STRING_CONST
static char* SLogPriority[LOG_NUMBER_OF_PRIORITIES] =
{
	"EMERG",
	"ALERT",
	"CRIT",
	"ERR",
	"WARNING",
	"NOTICE",
	"INFO",
	"DEBUG",
	"COMMUNICATION",
	"TRACE"
};
#endif

/* Output identifiers */ 
typedef enum {
	E_LOG_OUTPUT_NONE = 0,
    E_LOG_OUTPUT_CONSOLE,
    E_LOG_OUTPUT_FILE,
    E_LOG_OUTPUT_DEBUGER,
	E_NUMBER_OF_OUTPUTS
}ELogOutput;

#ifdef DEFINE_LOG_STRING_CONST
static char* SLogOutput[E_NUMBER_OF_OUTPUTS] =
{
	"NONE",
	"CONSOLE",
	"FILE",
	"DEBUGER"
};
#endif

/* Channel identifiers */
typedef enum {
	E_LOG_CHANNEL_NONE = 0,
	E_LOG_CHANNEL_SYSTEM,
	E_LOG_CHANNEL_BOX,
	E_LOG_CHANNEL_CAMERA,
	E_LOG_CHANNEL_CAN,
	E_LOG_CHANNEL_COUNTER,
	E_LOG_CHANNEL_DBS,
	E_LOG_CHANNEL_IBUTTON,
	E_LOG_CHANNEL_IVIBRIDGE,
	E_LOG_CHANNEL_JIG,
	E_LOG_CHANNEL_LABELS,
	E_LOG_CHANNEL_ODBC,
	E_LOG_CHANNEL_PRINTER,
	E_LOG_CHANNEL_SENSORS,
	E_LOG_CHANNEL_SERCOMM,
	E_LOG_CHANNEL_TESTER,
	E_LOG_CHANNEL_USERS,
	E_LOG_CHANNEL_VLINE,
	E_LOG_CHANNEL_TOOL_AIRPUMP,
	E_LOG_CHANNEL_TOOL_BANNER,
	E_LOG_CHANNEL_TOOL_BARCODE,
	E_LOG_CHANNEL_TOOL_GREASE,
	E_LOG_CHANNEL_TOOL_IONIZER,
	E_LOG_CHANNEL_TOOL_LIFT,
	E_LOG_CHANNEL_TOOL_MPD,
	E_LOG_CHANNEL_LINE_CTRL,
	E_LOG_CHANNEL_SLEDGE,
	E_LOG_CHANNEL_LINE_IO,
	E_LOG_CHANNEL_TOOL_WAIT,
	E_LOG_CHANNEL_TOOL_SCREWDRIVER,
	E_LOG_CHANNEL_TOOL_BOXGROUP,
	E_LOG_CHANNEL_TOOL_DETECTION,
	E_LOG_CHANNEL_TOOL_PCBPREPARE,
	E_LOG_CHANNEL_TOOL_GREASE2,
	E_LOG_CHANNEL_TOOL_GREASE3,
	/* ... */
	E_NUMBER_OF_CHANNELS
} ELogChannel;

#ifdef DEFINE_LOG_STRING_CONST
static char* SLogChannel[E_NUMBER_OF_CHANNELS] =
{
	"NONE",
	"SYSTEM",
	"BOX",
	"CAMERA",
	"CAN",
	"COUNTER",
	"DBS",
	"IBUTTON",
	"IVIBRIDGE",
	"JIG",
	"LABELS",
	"ODBC",
	"PRINTER",
	"SENSORS",
	"SERCOMM",
	"TESTER",
	"USERS",
	"VLINE",
	"TOOL AIRPUMP",
	"TOOL BANNER",
	"TOOL BARCODE",
	"TOOL GREASE",
	"TOOL IONIZER",
	"TOOL LIFT",
	"TOOL MPD",
	"LINE CTRL",
	"SLEDGE",
	"LINE IO",
	"TOOL WAIT",
	"TOOL SCREWDRIVER",
	"TOOL BOXGROUP",
	"TOOL DETECTION",
	"TOOL PCBPREPARE",
	"TOOL GREASE2"
};
#endif

typedef void (_FUNCC *LogFce_t)(void* pLog, ELogChannel channel, uint32_t priority, const char* message, ...);

#if defined (_CVI_) || _MSC_VER < 1400
	/* fake function */
	void LOG(uint32_t priority, const char* message, ...);  
#else
	/* LOG(ERR, "Error: %d", -1) */
	#define LOG(priority, mesage, ...) \
		if(me->LogApi) { \
			void** vLogFnc = me->LogApi; \
			LogFce_t LogFnc = (LogFce_t)*vLogFnc; \
			LogFnc(me->LogApi, me->LogChannel, LOG_##priority, mesage, __VA_ARGS__); }
#endif

#endif /* __LOG_H__ */
