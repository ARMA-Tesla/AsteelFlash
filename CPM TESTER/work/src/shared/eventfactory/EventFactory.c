#include "EventFactory.h"
#include <cviauto.h>
#include <utility.h>
#include <stdlib.h>
#include <stdio.h>
#if !defined(_CVI_)
#include <process.h>    /* _beginthread, _endthread */
#else
#include <ansi_c.h>
#endif

#define LOG(message) \
	if(pEventFactory->_LogFnc) \
		pEventFactory->_LogFnc(message)     

typedef struct _SEventThreadData{
	struct _SElEventFactory* pEventFactory;
	SElEventPtr		pevent;
    SElEventDataPtr pData;
} SEventThreadData, *SEventThreadDataPtr;
											
/******************************************************************************
 * Static function declaration exported as class methods
 ******************************************************************************/
static SElExceptionPtr RegisterEvent( 
    struct _SElEventFactory* pEventFactory,
    const char* eventName
);
static  SElExceptionPtr RegisterEventFunction(
	struct _SElEventFactory* me,
	const char* eventName,
	EventFunction_t	EventFcePtr
);
static SElExceptionPtr GetEvent(
    struct _SElEventFactory* pEventFactory,
    const char* eventName,
    HANDLE*    pEvent
);
static SElExceptionPtr EventFactoryPulseEvent(
    struct _SElEventFactory* pEventFactory,
    const char* eventNameList,
    SElEventDataPtr pData
);
static const char* GetEventName(
    struct _SElEventFactory* pEventFactory,
    HANDLE event
);
static SElExceptionPtr SetLogFnc( 
	struct _SElEventFactory* me,
	SElExceptionPtr (*_LogFnc)(const char* message)
);
static SElExceptionPtr ClearLogFnc( 
	struct _SElEventFactory* me
);
static SElExceptionPtr event_delete(
	SElEventPtr* pEvent
);
/******************************************************************************
 * Second thread function
 ******************************************************************************/
#if defined(_CVI_)
	int CVICALLBACK EventFactoryEventThreadFunc (void *functionData);
#else
	unsigned __cdecl EventFactoryEventThreadFunc( void* pArguments );
#endif


/******************************************************************************
 * Exported function definition
 ******************************************************************************/
#undef __FUNC__
#define __FUNC__ "eleventfactory_new"
SElExceptionPtr eleventfactory_new(
    SElEventFactoryPtr* pEventFactory
)
{
    SElExceptionPtr pexception = NULL;

    *pEventFactory = (SElEventFactoryPtr)calloc(1, sizeof(SElEventFactory));
    EXCCHECKALLOC( *pEventFactory);    
    (*pEventFactory)->RegisterEvent = RegisterEvent;
    (*pEventFactory)->RegisterEventFunction = RegisterEventFunction;
    (*pEventFactory)->GetEvent = GetEvent;
    (*pEventFactory)->PulseEvent = EventFactoryPulseEvent;
    (*pEventFactory)->GetEventName = GetEventName;
	(*pEventFactory)->SetLogFnc = SetLogFnc;
	(*pEventFactory)->ClearLogFnc = ClearLogFnc;
    
    EXCCHECK( objlist_new(&((*pEventFactory)->_p_event_list)));
	(*pEventFactory)->_p_event_list->RegisterDestroy((*pEventFactory)->_p_event_list, event_delete);

#if defined(_CVI_)
	{	/* inner block */
		int	cmt_status = CmtNewThreadPool( 5, &(*pEventFactory)->_pool_handle);
	    if ( cmt_status < 0 )
	    {
			char	message[256] = "";

			CmtGetErrorMessage ( cmt_status, message);
			EXCTHROW( EL_ERROR_OS_SPECIFIC, message );
	    }
    }	/* inner block */
#endif


Error:
    EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__ "eleventfactory_delete"
SElExceptionPtr eleventfactory_delete(
    SElEventFactoryPtr* pEventFactory
)
{
    SElExceptionPtr pexception = NULL;
    
    if ( *pEventFactory )
    {
		EXCCHECK( objlist_delete(&((*pEventFactory)->_p_event_list)));
		(*pEventFactory)->_p_event_list = NULL;

        free( *pEventFactory);
        *pEventFactory = NULL;    
    }

Error:
    EXCRETHROW( pexception);
}

/******************************************************************************
 * Static function definition exported as class methods
 ******************************************************************************/
/*----------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "event_delete" 
static SElExceptionPtr event_delete(SElEventPtr* pEvent)
{
    SElExceptionPtr pexception = NULL;
   
	if(pEvent && *pEvent)
	{
		SElEventPtr		pevent = *pEvent;

		CloseHandle(pevent->event);
		pevent->event = NULL;

		if(pevent->EventListFce)
		{
			free(pevent->EventListFce);
			pevent->EventListFce = NULL;
		}

		free(*pEvent);
		*pEvent = NULL;
	}

Error:
	EXCRETHROW( pexception);
}

/*----------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "SElEventFactory::RegisterEvent" 
static SElExceptionPtr RegisterEvent( 
    struct _SElEventFactory* pEventFactory,
    const char* eventName
)
{
    SElExceptionPtr pexception = NULL;
    SElEventPtr		pevent = NULL;
    bool_t			free_event = FALSE;
   
	pevent = (pEventFactory->_p_event_list)->Get( pEventFactory->_p_event_list,
   										          eventName );
   										         
	if ( pevent )
	{
		char	message[256];

		sprintf( message, "Event already exist: %s", eventName);
		EXCTHROW( ELEVENT_WARN_ALREADY_EXIST, message );
	}
	
	free_event = TRUE;
	pevent = (SElEventPtr)calloc(1, sizeof(SElEvent));
	pevent->event = CreateEvent( NULL,
	                             TRUE, /* ManualReset */
	                             FALSE, /* InitialState */
	                             eventName);
	
	strncpy(pevent->name, eventName, ELEVENT_MAX_NAME_LENGTH); 
	
	if ( pevent->event == (HANDLE)NULL)
	{
		EXCTHROW( EL_ERROR_OS_SPECIFIC, "CreateEvent Fails" );
	}
	
	EXCCHECK((pEventFactory->_p_event_list)->Add( pEventFactory->_p_event_list,
   										          eventName,
   										          pevent));

Error:
	if ( pexception && free_event )
	{
		free( pevent);
		pevent = NULL;
	}
	
    EXCRETHROW( pexception);
} /* SElEventFactory::RegisterEvent */

/*----------------------------------------------------------------------------*/
/*!
 * \brief Register event function. The event function is called when event
 *			is occured.
 * \warning	Event function should be easy and fast.
 *
 */
#undef  __FUNC__
#define __FUNC__ "SElEventFactory::RegisterEventFunction" 
static SElExceptionPtr RegisterEventFunction( 
    struct _SElEventFactory* pEventFactory,
    const char* eventName,
    EventFunction_t	EventFcePtr
)
{
    SElExceptionPtr pexception = NULL;
    SElEventPtr		pevent = NULL;
   
	pevent = (pEventFactory->_p_event_list)->Get( pEventFactory->_p_event_list,
   										          eventName );
	if ( pevent == NULL )
	{
		EXCCHECK( pEventFactory->RegisterEvent(pEventFactory, eventName));
		
		pevent = (pEventFactory->_p_event_list)->Get( pEventFactory->_p_event_list,
   										          eventName );
		if ( pevent == NULL )
		{
			EXCTHROW( ELEVENT_ERR_UNKNOWN_EVENT_NAME, "Event doesn't exist!" );
		}
	}
	
	pevent->EventListFce = (EventFunction_t*)realloc(pevent->EventListFce, (pevent->fceCount + 1)*sizeof(EventFunction_t*));
	EXCCHECKALLOC( pevent->EventListFce);    
	
	*(pevent->EventListFce + pevent->fceCount) = EventFcePtr;
	pevent->fceCount += 1;

Error:
    EXCRETHROW( pexception);
} /* SElEventFactory::RegisterEvent */

/*----------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "SElEventFactory::GetEvent"
static SElExceptionPtr GetEvent(
    struct _SElEventFactory* pEventFactory,
    const char* eventName,
    HANDLE*    pEvent
)
{
    SElExceptionPtr pexception = NULL;
    SElEventPtr		pevent = NULL;
    
    pevent = (pEventFactory->_p_event_list)->Get( pEventFactory->_p_event_list,
   										          eventName );

	if ( pevent == NULL )
	{
		EXCCHECK( pEventFactory->RegisterEvent(pEventFactory, eventName));
		
		pevent = (pEventFactory->_p_event_list)->Get( pEventFactory->_p_event_list,
   										          eventName );
		if ( pevent == NULL )
		{
			EXCTHROW( ELEVENT_ERR_UNKNOWN_EVENT_NAME, "Event doesn't exist!" );
		}
	}

	*pEvent = ( pevent ) ? pevent->event : ((HANDLE)NULL);	

Error:
    EXCRETHROW( pexception);
} /* SElEventFactory::GetEvent */


/*----------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "SElEventFactory::EventFactoryPulseEvent"
static SElExceptionPtr EventFactoryPulseEvent(
    struct _SElEventFactory* pEventFactory,
    const char* eventNameList,
    SElEventDataPtr pData
)
{
    SElExceptionPtr pexception = NULL;
    SElEventPtr		pevent = NULL;
    BOOL			state;
	char			message[1024];    
    
    pevent = (pEventFactory->_p_event_list)->Get( pEventFactory->_p_event_list,
   										          eventNameList );
   										          
	if ( pevent == NULL )
	{
		EXCTHROW( ELEVENT_ERR_UNKNOWN_EVENT_NAME, "Unknown Event Name" );
	}
	
	state = PulseEvent( pevent->event);
	if ( state == (BOOL)0)
	{
		EXCTHROW( EL_ERROR_OS_SPECIFIC, "PulseEvent Fails" );
	}

	if ( pevent->fceCount )
	{
		SEventThreadDataPtr		pthread_data = NULL;
		
		pthread_data = (SEventThreadDataPtr)calloc(1, sizeof(SEventThreadData)); 
		EXCCHECKALLOC( pthread_data);
		pthread_data->pevent = pevent;
		pthread_data->pEventFactory = pEventFactory;
		EXCCHECK( eleventdata_new( &(pthread_data->pData)));
		if ( pData )
		{
			EXCCHECK( (pthread_data->pData)->SetStringPtr( pthread_data->pData, 
														   pData->GetStringPtr(pData)));
		}

#if defined(_CVI_)		
		{	/* inner block */
			int	cmt_status = CmtScheduleThreadPoolFunction(
										   pEventFactory->_pool_handle,
										   EventFactoryEventThreadFunc,
										   pthread_data,
										   NULL);
				
		    if ( cmt_status < 0 )
		    {
				char	message[256] = "";

				CmtGetErrorMessage ( cmt_status, message);
				EXCTHROW( EL_ERROR_OS_SPECIFIC, message );
		    }
		}	/* inner block */
					
#else
		_beginthread( EventFactoryEventThreadFunc,
				      0, pthread_data);
#endif
	}

	sprintf(message, "Event Pulsed: %s; %s", pevent->name, (pData)? pData->GetStringPtr(pData) : "");
	LOG( message);

Error:
    EXCRETHROW( pexception);
} /* SElEventFactory::PulseEvent */

/*----------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "SElEventFactory::GetEventName"
static const char* GetEventName(
    struct _SElEventFactory* pEventFactory,
    HANDLE event
)
{
	SObjListEntryPtr	plist = (pEventFactory->_p_event_list)->_pList;
	uint32_t			i, 
						used_count = (pEventFactory->_p_event_list)->_UsedCount;
	SElEventPtr			pevent;

	for ( i = 0; i < used_count; i++, plist++)
	{
		pevent = (SElEventPtr)(plist->pObject);
		if ( pevent->event == event)
		{
			return plist->name;
		}
	}
	

    return NULL;
} /* SElEventFactory::GetEventName */

/*----------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "SetLogFnc"
static SElExceptionPtr SetLogFnc( 
	struct _SElEventFactory* me,
	SElExceptionPtr (*_LogFnc)(const char* message)
)
{
	SElExceptionPtr pexception = NULL;
	
	me->_LogFnc = _LogFnc;
	
Error:
	EXCRETHROW( pexception);
}

/*----------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "ClearLogFnc"
static SElExceptionPtr ClearLogFnc( 
	struct _SElEventFactory* me
)
{
	SElExceptionPtr pexception = NULL;
	
	me->_LogFnc = NULL;
	
Error:
	EXCRETHROW( pexception);
}

/*----------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "EventFactoryEventThreadFunc"
#if defined(_CVI_)
	int CVICALLBACK EventFactoryEventThreadFunc (void *pArguments)
#else
	unsigned __cdecl EventFactoryEventThreadFunc( void* pArguments )
#endif
{
	SElExceptionPtr		pexception = NULL;
	int					i = 0;
	SEventThreadDataPtr	pthread_data = (SEventThreadDataPtr)pArguments;
	EventFunction_t		*pfunction = pthread_data->pevent->EventListFce;
	uint32_t			count = pthread_data->pevent->fceCount;
	struct _SElEventFactory*	pEventFactory = pthread_data->pEventFactory;
	char				message[512];
	
#if 1
	{	/* inner block */
	char	str[1024+1];

	sprintf(str,"%s(): %s: thread ID: %lu\n",__FUNC__,__FILE__,GetCurrentThreadId());
	OutputDebugString(str);
	}	/* inner block */
#endif

	for( i = 0; (uint32_t)i < count; i++, pfunction++)
	{
		EXCDISPLAY(  (*pfunction)(pthread_data->pevent->name, pthread_data->pData));	
	}
	
	sprintf(message, "Event Excited: %s", pthread_data->pevent->name);
	eleventdata_delete(&(pthread_data->pData));
	free( pthread_data);

	LOG( message);

#if defined(_CVI_)
	CmtExitThreadPoolThread (0);
#else
    _endthreadex( 0 );
#endif
    return 0;
}	/* EventFactoryEventThreadFunc */
