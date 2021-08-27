#if !defined(__ELODBC_H__)
#define __ELODBC_H__

#ifndef _WCHAR_T_DEFINED
#define _WCHAR_T_DEFINED
#define _WCHAR_T
typedef unsigned short wchar_t;
typedef unsigned short sdk_wchar_t;
#endif

#include <eltypes.h>
#include <windows.h>
#include <sql.h>
#include <time.h>
#include <sqlext.h>
#include <exception/Exception.h> 

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

typedef struct _SOdbc
{
	SElExceptionPtr (*Open)(struct _SOdbc* me, LPCTSTR szDSN, LPCTSTR szUser, LPCTSTR szPass);	
	SElExceptionPtr (*SetQueryTimeout)(struct _SOdbc* me, int32_t timeOut); 
	SElExceptionPtr (*Close)(struct _SOdbc* me);
	
	SElExceptionPtr (*PrepareSQL)(struct _SOdbc* me, LPCTSTR szSQL);
	SElExceptionPtr (*Exec)(struct _SOdbc* me);
	SElExceptionPtr (*ExecSQL)(struct _SOdbc* me, LPCTSTR szSQL);
	SElExceptionPtr (*Cancel)(struct _SOdbc* me);   
	
	int32_t (*Fetch)(struct _SOdbc* me, bool_t bBindColumns);
	int32_t (*MoveNext)(struct _SOdbc* me);

	SElExceptionPtr (*GetParams)(struct _SOdbc* me);
	SElExceptionPtr (*GetRows)(struct _SOdbc* me, int32_t* pCount);
	SElExceptionPtr (*GetCols)(struct _SOdbc* me, int32_t* pCount);
	SElExceptionPtr (*GetColName)(struct _SOdbc* me, int32_t col, LPTSTR lpszName, uint32_t dwName);
	SElExceptionPtr (*GetColLen)(struct _SOdbc	* me, int32_t col, int32_t* pLength);
	SElExceptionPtr (*GetColType)(struct _SOdbc* me, int32_t col, int32_t* pType);

	SElExceptionPtr (*BindParam)(struct _SOdbc* me,int32_t paramNo, int32_t paramIo, int32_t paramValueType, int32_t paramType, int32_t columnSize, int32_t decimalDigits, void* lpBuffer, uint32_t bufferSize, int32_t *lpIndicator);
	SElExceptionPtr (*BindColumn)(struct _SOdbc* me,int32_t colNo, int32_t colType, void* lpBuffer, uint32_t bufferSize, int32_t *lpIndicator);
	SElExceptionPtr (*BindColumns)(struct _SOdbc* me);

	SElExceptionPtr (*GetFieldLen)(struct _SOdbc* me,int32_t col,uint32_t* pLength);
   	SElExceptionPtr (*GetFieldValue)(struct _SOdbc* me,int32_t col, LPVOID lpValue, uint32_t dwValueSize);
   	void* (*GetFieldValuePtr)(struct _SOdbc* me,int32_t col);

	SElExceptionPtr (*GetErrorMessage)(struct _SOdbc* me, int32_t bufferSize, LPTSTR buffer);  
	
	SQLHENV     _henv;
	SQLHDBC     _hdbc;
	SQLHSTMT    _hstmt;
	uint32_t	_cols;
	LPBYTE     *_ppData;
	SQLINTEGER *_pIndicators;
	time_t		_time; /* last execution time */

	void*				LogApi;
	uint32_t			LogChannel;
	
} SOdbc, *SOdbcPtr;

SElExceptionPtr odbc_new(SOdbcPtr* pOdbc);
SElExceptionPtr	odbc_delete(SOdbcPtr* pOdbc);

#define		ELODBC_SQL_ERROR				-233333
#define     ELODBC_SQL_WARN_NO_DATA_FOUND  	2434

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__ELODBC_H__)
