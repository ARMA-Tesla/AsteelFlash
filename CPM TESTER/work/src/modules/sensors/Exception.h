/*! \file Exception.h
 *  \brief Elcom Exception class
 *  \author Zdenek Rykala <zdenek.rykala@elcom.cz>
 *  \version 1.0
 *  \date    1999-2006
 */

#if !defined(__ELCOM_EXCEPTION_H__)
#define __ELCOM_EXCEPTION_H__

#include <eltypes.h>
#include <elerror.h>
 
#if defined(__cplusplus)
    extern "C" {
#endif

#define ELEXCEPTION_ALLOC_MESSAGE_QUANTUM       32
#define ELEXCEPTION_ALLOC_PATH_QUANTUM          64

typedef struct _SElException
{
    /**************************************************************************
     * Public Members
     **************************************************************************/      
     void (*SetError)( struct _SElException* pException, int32_t error, const char* pMessage);
     int32_t (*GetErrorCode)( struct _SElException* pException );
     const char* (*GetErrorMsgPtr)( struct _SElException* pException );
     const char* (*GetCallPathPtr)( struct _SElException* pException );
	 void	  (*DestroySelf)(struct _SElException* pException);
     struct _SElException* (*AppendPath)(struct _SElException* pException, const char* fceName);
     void (*Display)(struct _SElException* pException);
     void (*Log)(struct _SElException* pException);

    /**************************************************************************
     * Private Members
     **************************************************************************/
    bool_t    _is_static; /*!< TRUE if the exption is static, otherwise FALSE */
    int32_t   _error_code; /*!< Error code */
    char*     _p_message; /*!< Humman readable exception message */
    uint32_t  _p_alloc_message;
    char*     _p_callpath; /*!< Humman readable exception message */
    uint32_t  _p_alloc_path;
	const char*     _file;
	long	  _line;
} SElException, *SElExceptionPtr;

SElExceptionPtr elexception_new(int32_t errorCode, const char* messsage, const char* file, long line);
/* EXCDELETE( pexception ) */

extern void* hWnd;

void excdisplay(SElExceptionPtr pException);
void excdispstr(SElExceptionPtr pException,char *str,int str_size);
void elexception_log_set(void (*fn_log)(SElExceptionPtr pException));

#define EXCDISPLAY( fCal)									\
	if( pexception = (fCal), pexception!=NULL ) {			\
		pexception->AppendPath(pexception, __FUNC__);		\
		if ( pexception->Log )								\
			pexception->Log(pexception);					\
		if ( pexception->Display )							\
			pexception->Display(pexception);				\
		EXCDELETE(pexception); }							\
	else

#define EXCDBGSTR( fCal)													\
	if( pexception = (fCal), pexception!=NULL) {							\
		char serror[512];													\
		pexception->AppendPath(pexception, __FUNC__);						\
		snprintf(serror, sizeof(serror),									\
						"_UT_Exception: Nb: %d; Message: %s; Call: %s\n",	\
						pexception->GetErrorCode(pexception),				\
						pexception->GetErrorMsgPtr(pexception),				\
						pexception->GetCallPathPtr(pexception));			\
		EXCDELETE(pexception); }											\
	else

#define EXCDELETE( pexception) \
	if( pexception) { \
		pexception->DestroySelf(pexception); \
		pexception = NULL; } \
	else

#define EXCCHECK( fCal) \
	if ( pexception = (fCal),  pexception != NULL ) \
		goto Error; \
	else

#define EXCTHROW( error, message) \
	if ( pexception = elexception_new( error, message, __FILE__, __LINE__), pexception ) \
		goto Error; \
	else

#define EXCRETHROW( exception) \
	return ( exception ) ? exception->AppendPath(exception, __FUNC__) : NULL

/* log exception but do not display it: */
#define EXCRETHROW_NO_DISP(exception)					\
	return (exception)									\
		? (												\
			exception->Display = NULL,					\
			exception->AppendPath(exception,__FUNC__)	\
		)												\
		: NULL

#define EXCCHECKALLOC( pmem) \
	if ( pmem == NULL ) { \
		pexception = elexception_new(EL_ERROR_LOW_MEMORY, NULL, __FILE__, __LINE__); \
		goto Error; } \
	else

#define EXCIGNORE( error) \
	if (pexception && pexception->GetErrorCode(pexception) == error) {\
        pexception->DestroySelf(pexception); \
		pexception = NULL; } \
	else

#define EXCCHECKCVI( fCal) \
	if ( error = (fCal), error < 0 ) \
		EXCTHROW( error, "Call:  "#fCal); \
	else

#define EXCCHECKCAN( fCal) \
	if ( error = (fCal), error < 0 ) {\
		char message[256]; \
		char buffer[512]; \
		ncStatusToString( error, sizeof(message)-1, message); \
		sprintf_s(buffer, sizeof(buffer), "Message:  %s\n\nCall: %s", message, #fCal); \
		EXCTHROW( error, buffer); } \
	else

#define EXCCHECKWIN( fCal) \
	if ( error = (int32_t)(fCal), error == 0 ) \
		EXCTHROW( EL_ERROR_OS_SPECIFIC, "Call:  "#fCal); \
	else

#define EXCCHECKCOM( fCal) \
	if ( hr = (fCal), FAILED(hr)) \
		EXCTHROW( EL_ERROR_WIN_COM, "Call:  "#fCal); \
	else 

#define EXCCHECKMSXML(fCal) EXCCHECKCOM(fCal)

#ifdef _CVI_
#define EXCCHECKMSXMLLOAD(fCal) \
	is_xml_success = VARIANT_TRUE; \
	if ( hr = (fCal), FAILED(hr)) \
		EXCTHROW( EL_ERROR_WIN_COM, "Call:  "#fCal); \
	else if ( is_xml_success == VARIANT_FALSE ) { \
		char message[512]; \
		char* buffer = NULL; \
		CA_VariantGetCString(&pfile_name_var, &buffer); \
		sprintf(message, "Message:  XML File not loaded: %s", buffer); \
		EXCTHROW( EL_ERROR_MSXML, message); \
		CA_FreeMemory(buffer); } \
	else 
#else
#define EXCCHECKMSXMLLOAD(fCal) \
	is_xml_success = VARIANT_TRUE; \
	if ( hr = (fCal), FAILED(hr)) \
		EXCTHROW( EL_ERROR_WIN_COM, "Call:  "#fCal); \
	else if ( is_xml_success == VARIANT_FALSE ) { \
		char message[512]; \
		char* buffer = NULL; \
		CA_VariantGetCString(&pfile_name_var, &buffer); \
		sprintf_s(message, sizeof(message), "Message:  XML File not loaded: %s", buffer); \
		EXCTHROW( EL_ERROR_MSXML, message); \
		CA_FreeMemory(buffer); } \
	else 
#endif

#define EXCCHECKIMAQ(fCal) \
	if ( error = (fCal), error <= 0 ) {\
		char* message; \
		char buffer[512]; \
		if ( error == 0 ) { error = imaqGetLastError(); } \
		message = imaqGetErrorText (error); \
		sprintf(buffer, "Message:  %s\n\nCall:  %s", message, #fCal); \
		EXCTHROW( error, buffer); \
		imaqDispose(message); } \
	else

#define EXCCHECKIMAQ1394(fCal) \
	if ( error = (fCal), error != IMG1394_ERR_GOOD ) { \
		char message[256]; \
		char buffer[512]; \
		imaq1394ShowError( error, message, sizeof(message)-1); \
		sprintf(buffer, "Message:  %s\n\nCall: %s", message, #fCal); \
		EXCTHROW( error, buffer); } \
	else

#define EXCCHECKIMAQDX(fCal) \
	if( error = (fCal), error != IMAQdxErrorSuccess) { \
		char message[256]; \
		char buffer[512]; \
		IMAQdxGetErrorString( error, message, sizeof(message)-1); \
		sprintf(buffer, "Message:  %s\n\nCall: %s", message, #fCal); \
		EXCTHROW( error, buffer); } \
	else

#define EXCCHECKSQL( fCal) \
	if ( sql_ret = (fCal), !SQL_SUCCEEDED(sql_ret) ) { \
		if(sql_ret==SQL_NO_DATA) \
			EXCTHROW( ELODBC_SQL_WARN_NO_DATA_FOUND, "Call:  "#fCal); \
		else  \
			EXCTHROW( EL_ERROR_SQL, "Call:  "#fCal); } \
	else   

#define EXCCHECKDAQMX(fCal) \
	if ( error = (fCal), error < 0 ) { \
		char message[256]; \
		char buffer[512]; \
		typedef daveResultSet StResult; \
		StResult *Result; \
		daveGetErrorOfResult(Result, error); \
		sprintf(buffer, "Message:  %s\n\nCall: %s", Result->results->bytes, #fCal); \
		EXCTHROW( error, buffer); } \
	else

#define IVI_CHECK_ERR(error) \
	if(!pexception && error<0) { \
		int32_t error_ivi = error; \
		error = 0; \
		EXCTHROW( error_ivi, "IVI Driver fault"); }

#define EXCCHECKIVI(prefix, ivi_session, fCal) \
	if (error = (fCal), (error = (error < 0) ? error : VI_SUCCESS)) { \
		char message[512]; \
		char buffer[1024]; \
		prefix##_error_message(ivi_session, error, message); \
		sprintf(buffer, "IVI '%s' Driver fault\nMessage:  %s\n\nCall: %s", #prefix, message, #fCal); \
		EXCTHROW( error, buffer); } \
	else

#if defined(__cplusplus)
    } /* extern "C" */
#endif

#endif /* __ELCOM_EXCEPTION_H__ */
