/****************************************************************************
 *                            E L S T R I N G     
 *---------------------------------------------------------------------------
 *    Copyright  @ 2005 ELCOM, a.s.  All Rights Reserved.        
 *---------------------------------------------------------------------------
 *                                                                          
 * Title:       elstring.h                                                    
 * Purpose:     ELCOM String class
 * Version:     2.0 
 ****************************************************************************/

#if !defined(__ELSTRING_H__)
#define __ELSTRING_H__

#include <eltypes.h>
#include <exception/Exception.h>

#if !defined(MAX_ELSTRING_STATIC_LENGTH)
#define MAX_ELSTRING_STATIC_LENGTH		1024*30
#endif
#define ELSTRING_ALLOC_STEP				256

#define ELSTRING_BEGIN					(0UL)
#define ELSTRING_END					(0xFFFFFFFFUL)

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

typedef struct _SELString {

	SElExceptionPtr	(*ReserveSpace)(
        struct _SELString* pString,
        uint32_t size
    );
	SElExceptionPtr	(_FUNCC *Format)(
        struct _SELString* pString,
        const char* fstr,
        ...
    );
	SElExceptionPtr	(*Duplicate)(
        struct _SELString* pString,
        const char *str
    );
	int32_t	(*Size)(struct _SELString* pString);
	SElExceptionPtr	(*Empty)(struct _SELString* pString);
	SElExceptionPtr (*Append)(struct _SELString* pString, const char *str);
	SElExceptionPtr (*Remove)(struct _SELString* pString, uint32_t	offset, int32_t count);
	SElExceptionPtr (*HTMLSpecialCharsDecode)(struct _SELString* pString);
	const char* (*GetPtr)(struct _SELString* pString);
	SElExceptionPtr	(*GetStringBuffer)(struct _SELString* pString, char* buffer, int32_t bufferSize);
	SElExceptionPtr (*Replace)(struct _SELString* pString, const char* expression, const char* str);

    /**************************************************************************
     * PRIVATE    
     **************************************************************************/     
	char		_s_data[MAX_ELSTRING_STATIC_LENGTH];
	char*		_p_data;
	uint32_t 	_alloc_size;
	uint32_t 	_used_size;
} SELString, *SELStringPtr;

SElExceptionPtr elstring_new(
    const char*     pStr,
    SELStringPtr*	pString
);
SElExceptionPtr	elstring_delete(
	SELStringPtr*	pString
);


#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif /* __ELSTRING_H__ */
