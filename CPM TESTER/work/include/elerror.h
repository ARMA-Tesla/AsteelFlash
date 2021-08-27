/*!
 * ELCOM Error codes and macros
 * 
 * Copyright (c) 2005 by ELCOM,a.s., 2005
 */

#if !defined(__ELCOM_ERROR_H__)
#define __ELCOM_ERROR_H__

#define	EL_ERROR_GENERIC		(-1L)
#define EL_ERROR_OS_SPECIFIC	(-2L)
#define EL_ERROR_NULL_POINTER	(-3L)
#define EL_ERROR_CVIAUTO		(-4L)
#define EL_ERROR_WIN_COM        (-5L)
#define EL_ERROR_MSXML			(-6L)
#define EL_ERROR_CVIGUI			(-7L)
#define EL_ERROR_SQL			(-8L)
#define	EL_ERROR_LOW_MEMORY		(-100L)


#define EL_ERROR_NOT_NUMERIC_VALUE   (-222L)
		 

#define EL_ERROR_APP_OFFSET		(-100000L)
/*
 * Application specific errors can start with offset Application erros
 */

/*!
 * This macro is used for error checking. If the fuction fCal returns an
 * error, macro skip the all code on Error label. if the function fCal
 * return success (0), execution continues on next line.
 *
 * \warning	Function which is using this macro must have defined \p error
 * variable as \p int32_t
 */
#define CHECKERR( fCal) if (error = (fCal), error < 0) goto Error; else

/*!
 * This macro is used for checking after malloc() or calloc() function call.
 * If the pointer pVal is NULL pointer, macro skips the all code on Error label.
 * If the pointer pVal is not zero, execution continues on next line.
 *
 * \warning	Function which is using this macro must have defined \p error
 * variable as \p int32_t
 */
#define CHECKALLOC( pVal) if (NULL == (pVal)) { error = EL_ERROR_LOW_MEMORY; goto Error; }else


/*!
 * This macro is used for error checking of Windows function. If the fuction
 * fCal returns an error, macro skip the all code on Error label. Macro
 * translate WIN specific error tp EL_ERROR_OS_SPECIFIC.
 * if the function fCal  return success (0), execution continues on next line.
 *
 * \warning	Function which is using this macro must have defined \p error
 * variable as \p int32_t and \p win_errro as BOOL
 */

#define CHECKWIN( fCal) \
	if (win_error = (fCal), win_error == 0 ) { \
		error = EL_ERROR_OS_SPECIFIC;\
		goto Error;\
	} else

#if defined(_DEBUG)
/*!
 * This macro is defined for _DEBUG and should be placed before return in all functions.
 * In _DEBUG mode, it prints function name and error codes. In release mode is empty.
 */
#define PRINTERR( error) \
	if (error) printf(__FUNC__"... [ERROR] %d\n", error); else printf(__FUNC__"... [SUCCESS]\n")
#else
#define PRINTERR( error)
#endif





#if 0
#define CHECKWIN( fCal, errCode) \
			if (win_error = (fCal), win_error == 0 ) { \
				utility_LogWinError(__FILE__, __LINE__);\
				error = errCode; goto Error;\
			} else
#endif

#endif /* __ELCOM_UTILITY_H__ */
