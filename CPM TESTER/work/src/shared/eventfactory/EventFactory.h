/*! \file SElEventFactory
 *  \brief Elcom Event Factory class
 *  \author Zdenek Rykala <zdenek.rykala@elcom.cz>
 *  \version 1.0
 *  \date    1999-2006
 */

#if !defined(__ELCOM_EVENT_FACTORY_H__)
#define __ELCOM_EVENT_FACTORY_H__
 
#if defined(__cplusplus)
    extern "C" {
#endif

#include <objlist/objlist.h>
#include <exception/Exception.h>
#include <windows.h>
#include "eleventdata.h"

/*! Specifies maximum length of the elcom event names */
#define ELEVENT_MAX_NAME_LENGTH     64


#if OBJ_LIST_NAME_LENGTH < ELEVENT_MAX_NAME_LENGTH
#pragma  message("The length of Object List is less as length of event name")
#pragma  message("Hint: Change macro OBJ_LIST_NAME_LENGTH or ELEVENT_MAX_NAME_LENGTH")
#error "Incompatible string sizes"
#endif


typedef SElExceptionPtr (*EventFunction_t)( const char* eventName, void* reserved);


typedef struct _SElEvent
{
    HANDLE  			event;			/*!< Event */
    EventFunction_t		*EventListFce;	/*!< Event Functions */
    uint32_t			fceCount;		/*!< Number of Event Functions */
    char				name[ELEVENT_MAX_NAME_LENGTH+1];
} SElEvent, *SElEventPtr;

typedef struct _SElEventFactory
{
    /**************************************************************************
     * Public Members
     **************************************************************************/      
    SElExceptionPtr (*RegisterEvent)( struct _SElEventFactory* me,
                                      const char* eventName );
	SElExceptionPtr (*RegisterEventFunction)( struct _SElEventFactory* me,
                                      		  const char* eventName,
                                      		  EventFunction_t	EventFcePtr);
    SElExceptionPtr (*GetEvent)( struct _SElEventFactory* me,
							    const char* eventName,
							    HANDLE*    pEvent);

    SElExceptionPtr (*PulseEvent)( struct _SElEventFactory* me,
                                   const char* eventNameList,
                                   SElEventDataPtr pData);
    const char*     (*GetEventName)( struct _SElEventFactory* me,
                                     HANDLE event);
	
	SElExceptionPtr (*SetLogFnc)( struct _SElEventFactory* me,
								  SElExceptionPtr (*_LogFnc)(const char* message));
	
	SElExceptionPtr (*ClearLogFnc)( struct _SElEventFactory* me);   
	
    /**************************************************************************
     * Private Members
     **************************************************************************/
	 SElExceptionPtr (*_LogFnc)(const char* message);
     SObjListPtr	_p_event_list;	/*!< Event List */
#if defined(_CVI_)     
     int			_pool_handle;
#endif
} SElEventFactory, *SElEventFactoryPtr;

SElExceptionPtr eleventfactory_new(SElEventFactoryPtr* pEventFactory);
SElExceptionPtr eleventfactory_delete(SElEventFactoryPtr* pEventFactory);

#define ELEVENT_WARN_ALREADY_EXIST		1000			
#define ELEVENT_ERR_UNKNOWN_EVENT_NAME		-1000

#if defined(__cplusplus)
    } /* extern "C" */
#endif

#endif /* __ELCOM_EVENT_FACTORY_H__ */
