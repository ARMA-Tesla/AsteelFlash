#include <windows.h>
#include <userint.h>
#include "Exception.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h> 

void* hWnd;

#pragma warning( push )
#pragma warning(disable: 4996)

/******************************************************************************
 * Statis function declaration exported as class methods
 ******************************************************************************/
static void SetError(
    struct _SElException* pException,
    int32_t error,
    const char* pMessage
);
static int32_t GetErrorCode(
    struct _SElException* pException
);
static const char* GetErrorMsgPtr(
    struct _SElException* pException
);
static const char* GetCallPathPtr(
    struct _SElException* pException
);
static struct _SElException* AppendPath(
    struct _SElException* pException,
    const char* fceName
);
static void DestroySelf(
	struct _SElException* pException
);

static char*	gs_empty_String = "";
static SElException gs_LowMemoryException =
{
    SetError,
    GetErrorCode,
    GetErrorMsgPtr,
    GetCallPathPtr,
	DestroySelf,
    AppendPath,
    excdisplay,
    NULL,
    TRUE,
    EL_ERROR_LOW_MEMORY,
    "Low Memory detected", 
	0,
    NULL, 
	0,
	"",
	0
};

void (*gs_fn_log)(SElExceptionPtr pException);

/******************************************************************************
 * Exported function definition
 ******************************************************************************/
#undef __FUNC__
#define __FUNC__ "elexception_new"
SElExceptionPtr elexception_new(
    int32_t     errorCode,
    const char* messsage,
	const char* file, 
	long		line
)
{
    SElExceptionPtr pexception = NULL;

    if ( errorCode != EL_ERROR_LOW_MEMORY )
    {
        pexception = (SElExceptionPtr)calloc(1, sizeof(SElException));
        EXCCHECKALLOC( pexception);
        pexception->SetError = SetError;
        pexception->GetErrorCode = GetErrorCode;
        pexception->GetErrorMsgPtr = GetErrorMsgPtr;
        pexception->GetCallPathPtr = GetCallPathPtr;
        pexception->AppendPath = AppendPath;
		pexception->DestroySelf = DestroySelf;
		pexception->Display = excdisplay;
		pexception->Log = gs_fn_log;

		pexception->_file = file;
		pexception->_line = line;
        
        pexception->SetError( pexception, errorCode, messsage);
    }
    else
    {
        pexception = &gs_LowMemoryException;
    }
    
Error:
    return pexception;
}

#undef __FUNC__
#define __FUNC__ "elexception_delete"
void elexception_delete(
    SElExceptionPtr* pException
)
{
    if ( *pException )
    {
        if ( (*pException)->_is_static == FALSE)
        { 
            free((*pException)->_p_message);
            (*pException)->_p_message = NULL;
            free((*pException)->_p_callpath);
            (*pException)->_p_callpath = NULL;
            free( *pException);
        }
        *pException = NULL;            
    }

    return;
}

#undef __FUNC__
#define __FUNC__ "elexception_log_set"
void elexception_log_set(void (*fn_log)(SElExceptionPtr pException))
{
	gs_fn_log = fn_log;
}

#undef __FUNC__
#define __FUNC__ "excdispstr"
void excdispstr(SElExceptionPtr pException,char *str,int str_size)
{
	(void) str_size;

	if (  pException )
	{
		int32_t	error_code = pException->GetErrorCode(pException);
		char*	pend = str;
		
		pend += sprintf(pend, "Exception:  %d\n", error_code);
		if ( error_code == EL_ERROR_OS_SPECIFIC )
		{
			LPVOID buf;

  			if ( FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
  								NULL,
  								GetLastError(),
  								MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
  								(LPTSTR)&buf,
  								0, NULL))
			{
				pend += sprintf( pend, "Message:  %s\n", (const char*)buf);
				LocalFree(buf);
			}
		}
		
		pend += sprintf( pend, "%s\n", pException->GetErrorMsgPtr(pException)); /* "Call:  %s" */
		pend += sprintf( pend, "Path:  %s\n", pException->GetCallPathPtr(pException));
		pend += sprintf( pend, "File:  %s\n", pException->_file);
		pend += sprintf( pend, "Line:  %d", pException->_line);
	}
}

#undef __FUNC__
#define __FUNC__ "excdisplay"
void excdisplay(SElExceptionPtr pException)
{
	if (  pException )
	{
		char	message[2048] = "";
		
		excdispstr(pException,message,sizeof(message));
#if 0	/* VALEO assy line: see 'line_ctrl_el_exception_log()' */
		elLog("exception:",__FUNC__);	
		elLog("%s",message);
#endif
		//MessageBox((HWND)hWnd, message, "Exception", MB_OK|MB_ICONERROR);
		MessagePopup ("Exception", message); 

	}
}

/******************************************************************************
 * Statis function definition exported as class methods
 ******************************************************************************/
/*----------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "SElEventFactory::SetError" 
static void SetError(
    struct _SElException* pException,
    int32_t error,
    const char* pMessage
)
{
    pException->_error_code = error;
    
    if ( pException->_is_static == FALSE)
    {
        size_t    length = (pMessage) ? strlen(pMessage) + 1 : 0;
        if ( pException->_p_alloc_message < length )
        {
            pException->_p_message = realloc( pException->_p_message, sizeof(char) * (( length / ELEXCEPTION_ALLOC_MESSAGE_QUANTUM ) + 1) * ELEXCEPTION_ALLOC_MESSAGE_QUANTUM);
            pException->_p_alloc_message = (uint32_t)((( length / ELEXCEPTION_ALLOC_MESSAGE_QUANTUM ) + 1) * ELEXCEPTION_ALLOC_MESSAGE_QUANTUM); 
            /*
             * ToDo: what happen wen low memory ?
             */          
        }
        if ( length && pException->_p_message )
        {
            memcpy( pException->_p_message, pMessage, length);
        }
    }
} /* SElEventFactory::SetError */

/*----------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "SElEventFactory::GetErrorCode"
static int32_t GetErrorCode(
    struct _SElException* pException
)
{
    return pException->_error_code;
} /* SElEventFactory::GetErrorCode */

/*----------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "SElEventFactory::GetErrorMsgPtr"
static const char* GetErrorMsgPtr(
    struct _SElException* pException
)
{
    return (pException->_p_message) ? pException->_p_message : gs_empty_String;
} /* SElEventFactory::GetErrorMsgPtr */

/*----------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "SElEventFactory::GetCallPathPtr"
static const char* GetCallPathPtr(
    struct _SElException* pException
)
{
    return (pException->_p_callpath) ? pException->_p_callpath : gs_empty_String;
} /* SElEventFactory::GetCallPathPtr */

/*----------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "SElEventFactory::DestroySelf"
static void DestroySelf(
	struct _SElException* pException
)
{
	elexception_delete(&pException);
}

/*----------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "SElEventFactory::AppendPath"
static struct _SElException* AppendPath(
    struct _SElException* pException,
    const char* fceName
)
{
    if ( pException->_is_static == FALSE )
    {
        size_t    length = (fceName) ? strlen(fceName) + 2 : 0;
        size_t    used   = (pException->_p_callpath) ? strlen(pException->_p_callpath) : 0;
        
        if ( pException->_p_alloc_path < (used + length + 1) )
        {
            uint32_t    required_size = (uint32_t)(used + length + 1);

            required_size = (( required_size / ELEXCEPTION_ALLOC_PATH_QUANTUM ) + 1) * ELEXCEPTION_ALLOC_PATH_QUANTUM;
            pException->_p_callpath = realloc( pException->_p_callpath, sizeof(char) * required_size);
            pException->_p_alloc_path = required_size; 
            /*
             * ToDo: what happen wen low memory ?
             */          
        }
        if ( used && length && pException->_p_callpath )
        {
            memmove(pException->_p_callpath + length, pException->_p_callpath, used + 1);
        }
        if ( length )
        {
            const char* pspliter = "->";
            
            memcpy(pException->_p_callpath, fceName, length - ((used) ? 2 : 1) );
            if ( used )
            {
            	memcpy(pException->_p_callpath+ (length-2), pspliter, 2);
            }
        }
    }
    
    return pException;
} /* SElEventFactory::AppendPath */

#pragma warning( pop )
