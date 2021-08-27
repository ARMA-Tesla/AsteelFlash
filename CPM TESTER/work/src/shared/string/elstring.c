/****************************************************************************
 *                            E L S T R I N G     
 *---------------------------------------------------------------------------
 *    Copyright  @ 2005 ELCOM, a.s.  All Rights Reserved.        
 *---------------------------------------------------------------------------
 *                                                                          
 * Title:       elstring.c                                                    
 * Purpose:     ELCOM String class
 * Version:     2.0  
 ****************************************************************************/
#include <elstring.h>
#include <exception/exception.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#pragma warning( push )
#pragma warning(disable: 4996)

static const struct {
	unsigned short charcode;
	const char *entity;
	int entitylen;
} gs_basic_html_entities[] = {
	{ '"',	"&quot;",	6 },
	{ '\'',	"&#039;",	6 },
	{ '\'',	"&#39;",	5 },
	{ '<',	"&lt;",		4 },
	{ '>',	"&gt;",		4 },
	{ 0, NULL, 0}
};


static SElExceptionPtr	elstringPreAlloc(struct _SELString* me, uint32_t newSize);
static SElExceptionPtr	elstringReserveSpace(struct _SELString* me, uint32_t newSize);
static SElExceptionPtr	elstringDuplicate(struct _SELString* me, const char *str);
static int32_t	elstringSize(struct _SELString* me);
static SElExceptionPtr	elstringAppend(struct _SELString* me, const char *str);
static SElExceptionPtr	elstringRemove( struct _SELString* pString, uint32_t	offset, int32_t		count);
static SElExceptionPtr	elstringEmpty(struct _SELString* me);
static SElExceptionPtr  elstringHTMLSpecialCharsDecode(struct _SELString* me);
static SElExceptionPtr	_FUNCC elstringFormat(struct _SELString* me, const char *fstr, ...);
static const char* elstringGetPtr(struct _SELString* me);
static SElExceptionPtr elstringGetBuffer(struct _SELString* me, char* buffer, int32_t bufferSize);
static SElExceptionPtr elstringReplace(struct _SELString* pString, const char* expression, const char* str);

/* CVI or MSVC++ 6.0 or earlier: */
#if defined (_CVI_) || _MSC_VER <= 1200
static FILE*	gs_tmp_file = NULL;
static uint32_t _vscprintf(
   	  const char *format,
      va_list argptr 
);
#endif


/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elstring_new"
/*!
 * \brief
 * This function creates a new instance.
 * 
 * \param pError 
 * Returns a value indicating whether an error occurred.
 * A negative error code indicates function failure.	
 * 
 * \return 
 * Pointer to new instance of the String object. If the function fails, NULL
 * pointer is returned
 *
 * This function creates a new instance of the String object.
 */
SElExceptionPtr elstring_new(
	const char*	pStr,
	SELStringPtr*	pString
)
{
	SElExceptionPtr    pexception = NULL;

	*pString = (SELStringPtr)calloc( 1, sizeof(SELString));
	EXCCHECKALLOC( *pString);
	(*pString)->Duplicate = elstringDuplicate;
	(*pString)->Size = elstringSize;
	(*pString)->Append = elstringAppend;
	(*pString)->Remove = elstringRemove;
	(*pString)->Format = elstringFormat;
	(*pString)->Empty = elstringEmpty;
	(*pString)->GetPtr = elstringGetPtr;
	(*pString)->GetStringBuffer = elstringGetBuffer;
	(*pString)->ReserveSpace = elstringReserveSpace; 
	(*pString)->HTMLSpecialCharsDecode = elstringHTMLSpecialCharsDecode;
	(*pString)->Replace = elstringReplace;

	(*pString)->_p_data = (*pString)->_s_data;
	EXCCHECK( (*pString)->Append(*pString, pStr));
	
Error:
	if ( pexception )
	{
		elstring_delete( pString);
	}
	EXCRETHROW( pexception); 
} /* elstring_new */


/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elstring_delete"
/*!
 * \brief
 * This function destroys specified instance.
 * 
 * \param pString 
 * Pointer to instance of the String object cretaed by elstring_new() function.
 * 
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  None\n\n
 *  ERRORS:\n
 *  None\n 
 * 
 * This function destroys specified instance and release all resources.
 */
SElExceptionPtr	elstring_delete(
	SELStringPtr*	pString
)
{
	if ( pString && *pString )
	{
		(*pString)->Empty( *pString);
		if ( (*pString)->_alloc_size )
		{
			free( (*pString)->_p_data );
			(*pString)->_p_data     = NULL;
			(*pString)->_alloc_size =  0;
		}
		free(*pString);
		*pString = NULL;
	}

/* Error: */
	return NULL;
} /* elstring_delete */



/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elstringReserveSpace"
/*!
 * \brief
 * This function reserve required space for apend data.
 * 
 * \param pString 
 * Pointer to instance of the String object cretaed by elstring_new() function.
 *
 * \param requiredSize
 * Required sapce. object allocate memory for curent size + \p requiredSize.
 * 
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  None\n\n
 *  ERRORS:\n
 *  EL_ERROR_LOW_MEMORY - Low memory\n 
 * 
 * This function reserve required space for apend data. Use this function,
 * if you appand a lot of pieces, which requires reallocation of memory.
 */
static SElExceptionPtr	elstringReserveSpace(
	struct _SELString* pString,
	uint32_t requiredSize
)
{
	SElExceptionPtr    pexception = NULL;
	uint32_t           required_size = pString->_used_size + requiredSize + 1; 
    
    EXCCHECK( elstringPreAlloc( pString, required_size));

Error: 
	EXCRETHROW( pexception);
} /* elstringReserveSpace */


/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elstringDuplicate"
/*!
 * \brief
 * This function duplicates string.
 * 
 * \param pString 
 * Pointer to instance of the String object cretaed by elstring_new() function.
 *
 * \param str
 * String which is duplicated into \p pString object. 
 * 
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  None\n\n
 *  ERRORS:\n
 *  EL_ERROR_LOW_MEMORY - Low memory\n 
 * 
 * This function duplicates string.
 */
static SElExceptionPtr	elstringDuplicate(
	struct _SELString* pString,
	const char *str
)
{
	SElExceptionPtr    pexception = NULL;
	
	EXCCHECK( pString->Empty( pString));
	EXCCHECK( pString->Append( pString, str));

Error:
    EXCRETHROW( pexception);
} /* elstringDuplicate */


/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elstringSize"
/*!
 * \brief
 * This function returns string length.
 * 
 * \param pString 
 * Pointer to instance of the String object cretaed by elstring_new() function.
 * 
 * \return 
 *  Length of the string.
 * 
 * This function returns size of string in \p pString object.
 */
static int32_t	elstringSize(
	struct _SELString* pString
)
{
	return (pString->_used_size) ? (pString->_used_size - 1) : 0;
} /* elstringSize */


/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elstringAppend"
/*!
 * \brief
 * This function appends string into \p pString object.
 * 
 * \param pString 
 * Pointer to instance of the String object cretaed by elstring_new() function.
 *
 * \param str
 * The C-string to be appended.
 * 
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  None\n\n
 *  ERRORS:\n
 *  EL_ERROR_LOW_MEMORY - Low memory\n 
 * 
 * This function appends string on the end of \p pString object.
 */
static SElExceptionPtr	elstringAppend(
	struct _SELString* pString,
	const char *str
)
{
    SElExceptionPtr    pexception = NULL;
	uint32_t	      required_size;

	if ( str )
	{
		required_size = pString->_used_size + (uint32_t)strlen(str) + 1;
		
		if (( required_size > MAX_ELSTRING_STATIC_LENGTH ) &&
		    ( required_size > pString->_alloc_size ))
		{
			uint32_t	new_size =  ((required_size / ELSTRING_ALLOC_STEP) + 1) * ELSTRING_ALLOC_STEP;
			if ( pString->_alloc_size )
				pString->_p_data = realloc(pString->_p_data, new_size);
			else
			{
				pString->_p_data = malloc(new_size);
				memcpy( pString->_p_data, pString->_s_data, pString->_used_size);
			}
		
		  	EXCCHECKALLOC( pString->_p_data);
		  	pString->_alloc_size = new_size;
		}
		strcpy( pString->_p_data + (( pString->_used_size ) ? pString->_used_size - 1 : 0) , str);
		if ( pString->_used_size > 0 )
			required_size -= 1;
		
		pString->_used_size = required_size;
	}

Error:
    EXCRETHROW( pexception);
} /* elstringAppend */

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elstringReplace"
static SElExceptionPtr elstringReplace(
	struct _SELString* pString, 
	const char* expression, 
	const char* str
)
{
	SElExceptionPtr	pexception = NULL;
	uint32_t		required_size;

	if( str && expression && *expression )
	{
		char* pdata = strstr(pString->_p_data, expression);

		if(pdata)
		{
			required_size = pString->_used_size + (uint32_t)strlen(str) - (uint32_t)strlen(expression) + 1;

			if (( required_size > MAX_ELSTRING_STATIC_LENGTH ) &&
				( required_size > pString->_alloc_size ))
			{
				uint32_t	new_size =  ((required_size / ELSTRING_ALLOC_STEP) + 1) * ELSTRING_ALLOC_STEP;
				if ( pString->_alloc_size )
					pString->_p_data = realloc(pString->_p_data, new_size);
				else
				{
					pString->_p_data = malloc(new_size);
					memcpy( pString->_p_data, pString->_s_data, pString->_used_size);
				}
			
		  		EXCCHECKALLOC( pString->_p_data);
		  		pString->_alloc_size = new_size;
			}

			memmove( pdata + (uint32_t)strlen(str), 
					 pdata + (uint32_t)strlen(expression), 
					 pString->_used_size - (pdata-pString->_p_data) - (uint32_t)strlen(expression));
			
			memcpy( pdata, str, strlen(str));

			if ( pString->_used_size > 0 )
				required_size -= 1;
			
			pString->_used_size = required_size;
		}
	}

Error:
    EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elstringRemove"
/*!
 * \brief
 * This function removes characters from string
 * 
 * \param pString 
 * Pointer to instance of the String object cretaed by elstring_new() function.
 *
 * \param offset
 * Specifies index, where is first character which will be remoced
 * * ELSTRING_BEGIN - characters are removed from begin
 * * ELSTRING_END	- characters are removed from end
 *
 * \param count
 * number of characters, which will be removed
 * 
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  None\n\n
 *  ERRORS:\n
 *  EL_ERROR_LOW_MEMORY - Low memory\n 
 * 
 * This function removes characters from string
 */
static SElExceptionPtr	elstringRemove(
	struct _SELString* pString,
	uint32_t	offset,
	int32_t		count
)
{
    SElExceptionPtr    pexception = NULL;
	uint32_t	size = pString->Size(pString); 
	
	if ( offset < size ||
		 offset == ELSTRING_END )
	{
		if ( offset ==  ELSTRING_END )
		{
			pString->_used_size = ((int32_t)( pString->_used_size - 1 ) > count) ? pString->_used_size - count : 0;
			pString->_p_data[pString->_used_size - 1] = '\0';
		}
		else
		{
			if (( offset + count ) > size ) 
			{
				pString->_used_size	= offset;
				pString->_p_data[pString->_used_size] = '\0';
			}
			else
			{
				/*
				 * "test for remove"
				 *  		 ^
				 *           +------ offset = 9, length 6
				 * "test for "
				 */
				
				memmove( pString->_p_data + offset, pString->_p_data + offset + count, size - ( offset + count ) + 1);
				pString->_used_size	-= count;
			}

		}
	}

/* Error: */
	if ( pString->_p_data )
	{
		pString->_p_data[pString->_used_size] = '\0';
	}
    EXCRETHROW( pexception);
} /* elstringRemove */

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elstringFormat"
/*!
 * \brief
 * This function formats and appends string into \p pString object.
 * 
 * \param pString 
 * Pointer to instance of the String object cretaed by elstring_new() function.
 *
 * \param fstr
 * Format-control string
 * 
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  None\n\n
 *  ERRORS:\n
 *  EL_ERROR_LOW_MEMORY - Low memory\n 
 * 
 * This function formats and appends string on the end of \p pString object.
 */
static SElExceptionPtr	_FUNCC elstringFormat(struct _SELString* pString, const char *fstr, ...)
{
    SElExceptionPtr    pexception = NULL;
	va_list ap;	
	uint32_t	required_size = 0;
	bool_t		was_empty = FALSE;
												 
		va_start(ap, fstr);
		
		required_size = _vscprintf( fstr, ap);  //strlen
		was_empty = ( pString->_used_size == 0 );
		EXCCHECK( pString->ReserveSpace( pString, required_size));
		pString->_used_size += vsprintf(pString->_p_data + ((pString->_used_size ) ? (pString->_used_size-1): 0 ), fstr, ap);
		
		if ( was_empty )
		{
			pString->_used_size += 1;
		}
		va_end(ap);

Error:
    EXCRETHROW( pexception);
} /* elstringFormat */


/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elstringEmpty"
/*!
 * \brief
 * This function deletes contains of the \p pString object
 * 
 * \param pString 
 * Pointer to instance of the String object cretaed by elstring_new() function.
 *
 * \param fstr
 * Format-control string
 * 
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  None\n\n
 *  ERRORS:\n
 *  EL_ERROR_LOW_MEMORY - Low memory\n 
 * 
 * This function deletes contains of the \p pString object
 */
static SElExceptionPtr	elstringEmpty(
	struct _SELString* pString
)
{
	
#if 0
	if ( pString->_alloc_size )
	{
		free(pString->_p_data);	
		pString->_alloc_size = 0;
		pString->_p_data = pString->_s_data;
	}
#else
	pString->_used_size = 0;
	pString->_p_data[0] = 0;
#endif
	return NULL;
} /* elstringEmpty */



/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elstringGetPtr"
/*!
 * \brief
 * This function deletes contains of the \p pString object
 * 
 * \param pString 
 * Pointer to instance of the String object cretaed by elstring_new() function.
 *
 * \param fstr
 * Format-control string
 * 
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  None\n\n
 *  ERRORS:\n
 *  EL_ERROR_LOW_MEMORY - Low memory\n 
 * 
 * This function deletes contains of the \p pString object
 */
static const char* elstringGetPtr(struct _SELString* pString)
{
	return pString->_p_data;
}


/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elstringHTMLSpecialCharsDecode"
/*!
 * \brief
 * This function deletes contains of the \p pString object
 * 
 * \param pString 
 * Pointer to instance of the String object cretaed by elstring_new() function.
 *
 * \param fstr
 * Format-control string
 * 
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  None\n\n
 *  ERRORS:\n
 *  EL_ERROR_LOW_MEMORY - Low memory\n 
 * 
 * This function deletes contains of the \p pString object
 */
static SElExceptionPtr  elstringHTMLSpecialCharsDecode(struct _SELString* pString)
{
    SElExceptionPtr    pexception = NULL;
	char*		pstart = pString->_p_data;
	char*		pbegin = pString->_p_data;
	int			i, replace_amp;
	/*
	 * replace all & to &amp;
	 */
	while ( (pbegin = strchr( pbegin, '&')) != NULL )
	{
		replace_amp = 1;
		if ( *(pbegin+1) == '#' )
		{
			char*		pend;
			long		value = strtol(pbegin+1, &pend, 16);

			(void) value;
			replace_amp = (*pend != ';');
		}
		else if ( strncmp(pbegin, "&amp;",5 ) != 0 )
		{
			for( i = 0; gs_basic_html_entities[i].entity; i++)
			{
				if ( strcmp(pbegin, gs_basic_html_entities[i].entity) == 0 )
					break;
			}
			
			replace_amp = ( gs_basic_html_entities[i].entity == NULL );
		}
		else
		{
			replace_amp = 0;
		}
		
		if ( replace_amp )
		{
			int	character = (int)(pbegin - pstart);
			EXCCHECK( pString->ReserveSpace( pString, 4));
			pstart = pString->_p_data;
			pbegin = pstart + character;
			memmove(pbegin + 4, pbegin, strlen(pbegin)+1);
			memcpy(pbegin+1, "amp;", 4);
			pString->_used_size += 4;
		}
		pbegin++;
	}
	
	
	for( i = 0; gs_basic_html_entities[i].entity; i++)
	{
		pbegin = pstart;
		while ( (pbegin = strchr( pbegin, gs_basic_html_entities[i].charcode)) != NULL )
		{
			int	character = (int)(pbegin - pstart);
			EXCCHECK( pString->ReserveSpace( pString, gs_basic_html_entities[i].entitylen-1));
			pstart = pString->_p_data;
			pbegin = pstart + character;
			memmove(pbegin + gs_basic_html_entities[i].entitylen -1, pbegin, strlen(pbegin)+1);
			memcpy(pbegin, gs_basic_html_entities[i].entity, gs_basic_html_entities[i].entitylen);
			pString->_used_size += gs_basic_html_entities[i].entitylen - 1;
			pbegin++;
		}
	}
	
	

Error:
    EXCRETHROW( pexception);
} /* elstringHTMLSpecialCharsDecode */


/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elstringPreAlloc"
/*!
 * \brief
 * This function realocate size of the intrnal buffer.
 * 
 * \param pString 
 * Pointer to instance of the String object cretaed by elstring_new() function.
 *
 * \param newSize
 * New size of internal buffer
 * 
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  None\n\n
 *  ERRORS:\n
 *  EL_ERROR_LOW_MEMORY - Low memory\n 
 * 
 * This function realocate size of the intrnal buffer.
 */
static SElExceptionPtr	elstringPreAlloc(
	struct _SELString* pString,
	uint32_t newSize
)
{
    SElExceptionPtr    pexception = NULL;

	if (( newSize > MAX_ELSTRING_STATIC_LENGTH ) &&
		( newSize > pString->_alloc_size ))
	{
		if ( pString->_alloc_size )
			pString->_p_data = realloc(pString->_p_data, newSize);
		else
		{
			pString->_p_data = malloc(newSize);
			if ( pString->_used_size )
			{
			memcpy( pString->_p_data, pString->_s_data, pString->_used_size);
		}
			else
			{
				pString->_p_data[0] = '\0';
		}
		}
	  	EXCCHECKALLOC( pString->_p_data);
	  	pString->_alloc_size = newSize;
	}
	
Error:
    EXCRETHROW( pexception);
} /* elstringPreAlloc */


/* CVI or MSVC++ 6.0 or earlier: */
#if defined (_CVI_) || _MSC_VER <= 1200
/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "_vscprintf"
/*!
 * \brief
 * Returns the number of characters in the formatted string using a pointer
 * to a list of arguments.
 * 
 * \param format 
 * Format-control string.
 *
 * \param argptr
 * Pointer to list of arguments.
 * 
 * \return 
 *  Returns the number of characters that would be generated if the string
 *  pointed to by the list of arguments was printed or sent to a file
 *  or buffer using the specified formatting codes. 
 */
static uint32_t _vscprintf(
   	  const char *format,
      va_list argptr 
)
{
	 int	size;
	 if ( gs_tmp_file == NULL )
	 	gs_tmp_file = tmpfile();
	 size = vfprintf( gs_tmp_file, format, argptr ) + 1;
	 fseek (gs_tmp_file, 0, SEEK_SET);
	 return (uint32_t)size; 
}
#endif

static SElExceptionPtr elstringGetBuffer(struct _SELString* me, char* buffer, int32_t bufferSize)
{
	SElExceptionPtr    pexception = NULL;  
	
	if (buffer == NULL && bufferSize == 0)
	{
		EXCTHROW( me->_used_size, "Required Size");
	}
	
	if ( (uint32_t)bufferSize < me->_used_size)
	{
		strncpy(buffer, me->_p_data, bufferSize - 1);
		buffer[bufferSize - 1] = '\0';
	}
	else
	{
		strcpy(buffer, me->_p_data);
	}

Error:
	EXCRETHROW( pexception); 
}

#pragma warning( pop )
