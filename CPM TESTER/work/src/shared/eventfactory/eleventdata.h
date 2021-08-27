
#if !defined(__ELCOM_EVENT_DATA_H__)
#define __ELCOM_EVENT_DATA_H__

#include <elstring.h>

#if defined(__cplusplus)
    extern "C" {
#endif

typedef struct _SElEventData
{
	const char*	(*GetStringPtr)(struct _SElEventData* me);
	SElExceptionPtr	(*SetStringPtr)(struct _SElEventData* me, const char* pData);
	SELStringPtr _pString;
} SElEventData, *SElEventDataPtr;

SElExceptionPtr eleventdata_new(SElEventDataPtr* pEventData);
SElExceptionPtr eleventdata_delete(SElEventDataPtr* pEventData);


#if defined(__cplusplus)
    }
#endif


#endif /* __ELCOM_EVENT_DATA_H__ */
