#define DEFINE_LOG_STRING_CONST
#include "log_struct.h"
#include "msxmldom.h"
#include <guid.h>
#include <stdio.h>

#define OUTPUT_DEFAULT_FORMAT "%02d.%02d.%02d %02d.%02d.%02d [%s] %s:%s\n"

static void _FUNCC Log(struct _SLog* me, ELogChannel channel, uint32_t priority, const char* message, ...);
static SElExceptionPtr Initialize(struct _SLog* me, const char* fileName);

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "log_new"
SElExceptionPtr log_new(SLogPtr* pLog)
{
	SElExceptionPtr	pexception = NULL;
	SLogPtr			plog = NULL;
	
	plog = calloc(1, sizeof(SLog));
	EXCCHECKALLOC( plog);
		
	plog->Log = Log;
	plog->Initialize = Initialize;
		
	InitializeCriticalSection(&(plog->_Lock));

	if (pLog)
	{
		*pLog = plog;
		(*pLog)->LogApi = plog;
	}
	
Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "log_delete"
SElExceptionPtr log_delete(SLogPtr* pLog)
{
	SElExceptionPtr	pexception = NULL;
	
	if (pLog && *pLog)
	{
		SLogSessionPtr	plog_session = (*pLog)->_pLogSession;
		int i;

		for(i=0; i<(*pLog)->_pLogSessionSize; i++, plog_session++)
		{
			if(plog_session->file!=NULL)
			{
				FILE *file = (FILE*)plog_session->file;
				fclose(file);
			}
		}

		free((*pLog)->_pLogSession);
		(*pLog)->_pLogSession = NULL;

		DeleteCriticalSection(&((*pLog)->_Lock));

		free(*pLog);
		*pLog = NULL;
	}
	
Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "fncLock"
static SElExceptionPtr fncLock(struct _SLog* me)
{
	EnterCriticalSection( &me->_Lock);    
	return NULL;
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "fncUnLock"
static SElExceptionPtr fncUnLock(struct _SLog* me)
{
	LeaveCriticalSection( &me->_Lock);   
	return NULL;
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "Log"
static void _FUNCC Log(struct _SLog* me, ELogChannel channel, uint32_t priority, const char* message, ...)
{
	int				i;
	va_list			args;
	char*			pmessage = NULL;
	SLogSessionPtr	plog_session = NULL;

	fncLock(me);

	va_start(args, message);

	for(i=0, plog_session=me->_pLogSession; i<me->_pLogSessionSize; i++, plog_session++)
	{
		if(plog_session->channel==channel 
			&& plog_session->enabled==TRUE 
			&& (plog_session->mask&priority)>0)
		{
			uint32_t required_size = 0;
			uint32_t size = strlen(message);
			
			required_size = _vscprintf( message, args);
			if(size==required_size)
			{
				plog_session->Message(plog_session, priority, message);
			}
			else
			{
				pmessage = realloc(pmessage, (required_size+1)*sizeof(char));
				vsprintf(pmessage, message, args);
				plog_session->Message(plog_session, priority, pmessage);
			}
		}
	}

Error:
	va_end(args);
	free(pmessage);
	fncUnLock(me);
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "priority_to_nb"
static uint32_t priority_to_nb(uint32_t priotity)
{
	uint32_t	priority = 0;
	int			i;
	uint32_t	mask = 1;

	for(i=0; i<LOG_NUMBER_OF_PRIORITIES; i++)
	{
		if((mask&priotity)>0)
		{
			priority = i;
			break;
		}
		mask = mask<<1;
	}

	return priority;
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "Message"
static void _FUNCC Message(struct _SLogSession* me, uint32_t priority, const char* message)
{
	char *pmessage = NULL;

	if(me->enabled && (me->mask&priority)>0)
	{
		SYSTEMTIME st;
		
		GetLocalTime(&st);
		
		switch(me->output)
		{
			case E_LOG_OUTPUT_CONSOLE:
				printf(OUTPUT_DEFAULT_FORMAT,  
							st.wYear-2000,
							st.wMonth,
							st.wDay,
							st.wHour,
							st.wMinute,
							st.wSecond,
							SLogChannel[me->channel],
							SLogPriority[priority_to_nb(priority)], 
							message);
				break;
			case E_LOG_OUTPUT_DEBUGER:
				{
					uint32_t required_size = _scprintf(OUTPUT_DEFAULT_FORMAT, 
							st.wYear-2000,
							st.wMonth,
							st.wDay,
							st.wHour,
							st.wMinute,
							st.wSecond,
							SLogChannel[me->channel], 
							SLogPriority[priority_to_nb(priority)], 
							message);
					
					pmessage = realloc(pmessage, (required_size+1)*sizeof(char));

					sprintf(pmessage, OUTPUT_DEFAULT_FORMAT, 
							st.wYear-2000,
							st.wMonth,
							st.wDay,
							st.wHour,
							st.wMinute,
							st.wSecond,
							SLogChannel[me->channel], 
							SLogPriority[priority_to_nb(priority)], 
							message);

					OutputDebugStr(pmessage);
				}
				break;
			case E_LOG_OUTPUT_FILE:
				{
					if(me->file==NULL)
					{
						/* open */
						me->file = (void*)fopen(me->config, "a+");

						/* create if not exist */
						if(me->file==NULL)
							me->file = (void*)fopen(me->config, "w+");
					}
					
					if(me->file!=NULL)
					{
						FILE *file = (FILE*)me->file;

						fprintf(file, OUTPUT_DEFAULT_FORMAT, 
								st.wYear-2000,
								st.wMonth,
								st.wDay,
								st.wHour,
								st.wMinute,
								st.wSecond,
								SLogChannel[me->channel], 
								SLogPriority[priority_to_nb(priority)], 
								message);

						fflush(file);
					}
				}
				break;
		}
	}

Error:
	free(pmessage);
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "channel_from_str"
static ELogChannel channel_from_str(const char* schannel)
{
	uint32_t	channel = 0;
	int			i;

	for(i=0; i<E_NUMBER_OF_CHANNELS; i++)
	{
		if(0==strcmp(schannel, SLogChannel[i]))
		{
			channel = i;
			break;
		}
	}

	return channel;
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "output_from_str"
static ELogOutput output_from_str(const char* soutput)
{
	uint32_t	output = 0;
	int			i;

	for(i=0; i<E_NUMBER_OF_OUTPUTS; i++)
	{
		if(0==strcmp(soutput, SLogOutput[i]))
		{
			output = i;
			break;
		}
	}

	return output;
}

/*----------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "Initialize"
static SElExceptionPtr Initialize(
	struct _SLog*	me,
	const char*		fileName
)
{
	SElExceptionPtr pexception = NULL;
	int32_t			error = 0;
	HRESULT         hr = 0;
	CAObjHandle     pxml  = 0;
	CAObjHandle     pnode  = 0;
	CAObjHandle     plist = 0;
	CAObjHandle     pparameter = 0;
	CAObjHandle     pitem = 0;
	const char*     pfile_name = (fileName) ? fileName : "\\log.xml";
	VBOOL         	is_xml_success;
	VARIANT         pfile_name_var = CA_VariantEmpty();
	char*         	ptext= NULL;
	char			path[512];
	int32_t			i;

	EXCCHECKMSXML( MSXML_NewDOMDocumentIXMLDOMDocument (NULL, 1, LOCALE_NEUTRAL, 0, &pxml));
	EXCCHECKCVI( CA_VariantSetCString(  &pfile_name_var, pfile_name));
	EXCCHECKMSXMLLOAD( MSXML_IXMLDOMDocumentload (pxml, NULL, pfile_name_var, &is_xml_success));	
	EXCCHECKMSXML( MSXML_IXMLDOMDocumentselectSingleNode (pxml, NULL, "//module[@id='"MODULE_LOG"']", &pnode));   	
	EXCCHECKMSXML( MSXML_IXMLDOMNodeselectNodes (pnode, NULL, "listener", &plist));   
	if(plist)
	{
		SLogSessionPtr plog_session = NULL;
		long length = 0;	
	
		EXCCHECKMSXML( MSXML_IXMLDOMNodeListGetlength (plist, NULL, &length));
		
		me->_pLogSessionSize = length;
		me->_pLogSession = calloc(length, sizeof(SLogSession));

		EXCCHECKMSXML( MSXML_IXMLDOMNodeListreset (plist, NULL));
		
		for(i=0, plog_session=me->_pLogSession; i<length; i++, plog_session++)
		{
			/* init fnc */
			plog_session->Message = Message;

			EXCCHECKMSXML( MSXML_IXMLDOMNodeListnextNode (plist, NULL, &pitem));
			if(pitem)
			{
				EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pitem, NULL, "@name", &pparameter));
				if(pparameter)
				{
					EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
					if(ptext)
					{
						strcpy(plog_session->listener, ptext);
						CA_FreeMemory(ptext);
						ptext=NULL;
					}
					EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
					pparameter = 0;
				}
				EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pitem, NULL, "channel", &pparameter));
				if(pparameter)
				{
					EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
					if(ptext)
					{
						plog_session->channel = channel_from_str(ptext);
						CA_FreeMemory(ptext);
						ptext=NULL;
					}
					EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
					pparameter = 0;
				}
				EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pitem, NULL, "output", &pparameter));
				if(pparameter)
				{
					EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
					if(ptext)
					{
						plog_session->output = output_from_str(ptext);
						CA_FreeMemory(ptext);
						ptext=NULL;
					}
					EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
					pparameter = 0;
				}
				EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pitem, NULL, "enabled", &pparameter));
				if(pparameter)
				{
					EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
					if(ptext)
					{
						plog_session->enabled = (0==strcmp(ptext, "TRUE"));
						CA_FreeMemory(ptext);
						ptext=NULL;
					}
					EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
					pparameter = 0;
				}
				EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pitem, NULL, "mask", &pparameter));
				if(pparameter)
				{
					EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
					if(ptext)
					{
						plog_session->mask = strtoul(ptext, NULL, 2);
						CA_FreeMemory(ptext);
						ptext=NULL;
					}
					EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
					pparameter = 0;
				}
				EXCCHECKMSXML( MSXML_IXMLDOMNodeselectSingleNode (pitem, NULL, "config", &pparameter));
				if(pparameter)
				{
					EXCCHECKMSXML( MSXML_IXMLDOMNodeGettext (pparameter, NULL, &ptext));
					if(ptext)
					{
						strcpy(plog_session->config, ptext);
						CA_FreeMemory(ptext);
						ptext=NULL;
					}
					EXCCHECKCVI( CA_DiscardObjHandle(pparameter));
					pparameter = 0;
				}
				CA_DiscardObjHandle( pitem ); 
				pitem = 0; 
			}
		}
	}

Error:
	CA_FreeMemory(ptext);
	CA_VariantClear(&pfile_name_var);
	if (plist) CA_DiscardObjHandle(plist);
	if (pitem) CA_DiscardObjHandle(pitem);
	if (pparameter) CA_DiscardObjHandle(pparameter);
	if (pnode) CA_DiscardObjHandle(pnode);
	if (pxml) CA_DiscardObjHandle(pxml);
	EXCRETHROW( pexception);		
} /* sensorsLoadConfiguration */	