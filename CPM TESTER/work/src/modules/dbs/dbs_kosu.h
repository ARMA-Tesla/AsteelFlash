/*!
 * KOSU Calculator interface.
 * 
 * Copyright (c) 2005 by Zdenek Rykala, ELCOM, a.s.
 */

#if !defined(__DBS_KOSU_H__)
#define __DBS_KOSU_H__

#include "interval.h"
#include <exception/exception.h> 
#include <windows.h>

#if !defined(KOSU_MAX_USERS)
/*! 
 * Defines maximum login in one time. It specifies
 * how many KOSU settings can be stored. The default Value is 8.
 * It means that max. 8 KOSU section can be in kosu.ini
 */
#define KOSU_MAX_USERS			8
#endif

#if !defined(KOSU_MAX_ALARMS)
/*! 
 * Defines maximum defined allarms for KOSU.
 */
#define KOSU_MAX_ALARMS			4
#endif
#if !defined(KOSU_MAX_PANEL_ID)
/*! 
 * Defines maximum length of panel identification.
 */
#define KOSU_MAX_PANEL_ID			16
#endif

#if !defined(KOSU_USER_ID_SIZE)
/*! 
 * Defines size of user id identification. It is usually 8 bytes
 * 64bit i Button ID.
 */
#define KOSU_USER_ID_SIZE			8
#endif

/*!
 * Defines allocation quantum for product. 
 * When new product is inserted into KOSU calculatoe, it's stored
 * in specified KOSU instance. If the storage is full, the storage
 * will be reallocated with this quantum. This eliminates
 * memory allocation.
 */
#define	ALLOC_COUNT				100

/*****************************************************************************
 * KOSU ERRORS
 *****************************************************************************/

/*! \struct _SKOSUPause
 * 
 * Specifies the schedule brake, like lunch, cofee break, etc.
 * which employer adopt.
 * These breaks are in specified time min:hour and the time of the
 * break is specified in minutes.
 *
 * \typedef SKOSUPause
 * Specifies the schedule brake. \see _SKOSUPause
 */
typedef struct _SDBSKOSUPause {
	uint8_t		hour;	/*!< time of the begin of the brake hour:min */
	uint8_t		min;	/*!< time of the begin of the brake hour:min */
	uint16_t	delay;	/*!< the length of the break in minutes */
	struct _SDBSKOSUPause* pNext;
} SDBSKOSUPause, *SDBSKOSUPausePtr;

/*! \struct _SKOSUItem
 *
 * Specifies KOSU settings for specified user count.
 * For every specified number of operators, can be set different KOSU.
 * KOSU means, time between two successfully tested items. This time
 * is specified in seconds.
 *
 * \typedef SKOSUItem
 * Specifies KOSU settings for specified user count. \see _SKOSUItem
 */
typedef struct _SDBSKOSUItem {
	uint8_t		UserCount; /*!< Specifies number of operators */
	uint32_t	KOSU;	   /*!< Required KOSU for specified number of operators. */
} SDBSKOSUItem, *SDBSKOSUItemPtr;

typedef struct _SDBSKOSUAlarmItem {
	uint32_t	alarms[KOSU_MAX_ALARMS+1];
} SDBSKOSUAlarmItem, *SDBSKOSUAlarmItemPtr;

/*! \struct _SKOSU
 * KOSU caluclator and production storage. 
 *
 * \typedef SKOSU
 * KOSU caluclator and production storage. \see _SKOSU
 */
typedef struct _SDBSKOSU {
	void*			pdbs;

	SDBSKOSUItem		KOSUSettings[KOSU_MAX_USERS+1]; /*!< Spcifies KOSU settings. */
	SDBSKOSUAlarmItem	KOSUAlarms[KOSU_MAX_USERS+1]; /*!< Spcifies KOSU alarms settings. */
	SDBSKOSUPause*	    pPause; /*!< Specifies all scheduled brakes */
	SDBSKOSUPause*	    pShiftEnd; /*!< Specifies ends of schifts */

	SElExceptionPtr (*Initialize)(struct _SDBSKOSU* me, const char* xml_config);

	SElExceptionPtr (*AddKOSU)( struct _SDBSKOSU* me, uint8_t UserCount, uint32_t KOSU); /*!< \see kosuAddKosu */
	SElExceptionPtr	(*AddPause)(struct _SDBSKOSU* me, uint8_t hours, uint8_t min, uint16_t delay);    /*!< \see kosuAddPause */
	SElExceptionPtr	(*AddEndShift)(struct _SDBSKOSU* me, uint8_t hours, uint8_t min);   /*!< \see kosuAddEndShift */
	SElExceptionPtr (*SetAlarm)( struct _SDBSKOSU* me, uint32_t users, uint32_t alarm, uint32_t kosu);  /*!< \see kosuSetAlarm */
	
    SElExceptionPtr (*GetKOSU)( struct _SDBSKOSU* me,
								uint32_t* pPANEL_actual_good, 
							    uint32_t* pPANEL_actual_bad,
							    uint32_t* pPANEL_actual_retest_good,
							    uint32_t* pPANEL_actual_retest_bad,
							    uint32_t* pPANEL_required, 
							    uint32_t* pPANEL_END_actual,
							    uint32_t* pPANEL_END_required, 
							    uint32_t* pKOSU_actual,
							    uint32_t* pKOSU_required,  
							    uint32_t* pUSER_COUNT,  
							    uint32_t* pPAUSE_TIME);  /*!< \see kosuGetKOSU */
	
	SElExceptionPtr (*GetAvgKOSU)( struct _SDBSKOSU* me, time_t t1, time_t t2, uint32_t* pAvg, uint32_t* pPause);
	SElExceptionPtr (*GetShiftRange)( struct _SDBSKOSU* me, time_t* start,  time_t* stop); /*!< \see kosuGetLastShiftRange */
	SElExceptionPtr (*GetAlarmInInterval)(struct _SDBSKOSU* me, int32_t alarm, time_t t1, time_t t2, SIntervalResultPtr* pInterval);
	SElExceptionPtr	(*GetMaxAlarm)(struct _SDBSKOSU* me, int32_t* max_alarm);
	SElExceptionPtr (*GetRequiredKOSUAlarm)(struct _SDBSKOSU* me, uint32_t alarm, uint32_t UserCount, uint32_t* pKOSUValue);

	CRITICAL_SECTION	cs; /*!< Locks/Unlocks critical sesction */	

} SDBSKOSU, *SDBSKOSUPtr;

SElExceptionPtr dbskosu_new(SDBSKOSUPtr* pKOSU, void* pDBS);
SElExceptionPtr dbskosu_delete(SDBSKOSUPtr* pKOSU);

#define DBS_KOSU_ERROR_MAX_USER_COUNT_EXCEED	-100001
#define DBS_KOSU_ERROR_MAX_ALARM_COUNT_EXCEED	-100002
#define DBS_KOSU_ERROR_TOO_MANY_KOSU_SETTINGS	-100003
#define DBS_KOSU_ERROR_KOSU_NOT_AVAILABLE		-100004

#endif /* __DBS_KOSU_H__ */
