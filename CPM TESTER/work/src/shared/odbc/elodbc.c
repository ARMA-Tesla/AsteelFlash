/****************************************************************************
 *                              O D B C     
 *---------------------------------------------------------------------------
 *    Copyright  @ 2005 ELCOM, a.s.  All Rights Reserved.        
 *---------------------------------------------------------------------------
 *                                                                          
 * Title:       elodbc.h                                                    
 * Purpose:     ODBC library
 ****************************************************************************/
#include <ansi_c.h>
#include "elodbc.h"
#include <time.h>
#include <log/log.h>
#if defined(_PRODUCT_LINE)
#include "line_ctrl.h"
#endif
	
#define CHECKSQL(fCal) if (sql_ret = (fCal), !SQL_SUCCEEDED(sql_ret)) { error = (sql_ret == SQL_NO_DATA ) ? ELODBC_SQL_WARN_NO_DATA_FOUND : ELODBC_SQL_ERROR; goto Error; } else
		
#define MYSQL_TINYINT			-6
#define MYSQL_TEXT				-1	/* text tinytext longtext mediumtext */
#define MYSQL_BIGINT			-5  /* bigint */
#define MYSQL_BLOB				-4  /* tinyblob blob mediumblob longblob */

/*****************************************************************************
 *
 *****************************************************************************/ 
static SElExceptionPtr elOdbcOpen(struct _SOdbc* pOdbc, LPCTSTR szDSN, LPCTSTR szUser, LPCTSTR szPass);
static SElExceptionPtr elOdbcSetQueryTimeout(struct _SOdbc* pOdbc, int32_t timeOut);
static SElExceptionPtr elOdbcClose(struct _SOdbc* pOdbc);

static SElExceptionPtr elOdbcFreeBindColumns(struct _SOdbc* pOdbc);
static SElExceptionPtr elOdbcGetRows(struct _SOdbc* pOdbc, int32_t* pCount);
static SElExceptionPtr elOdbcGetCols(struct _SOdbc* pOdbc, int32_t* pCount);
static SElExceptionPtr elOdbcGetColName(struct _SOdbc* pOdbc, int32_t col, LPTSTR pBuffer, uint32_t bufferSize);
static SElExceptionPtr elOdbcGetColLen(struct _SOdbc* pOdbc, int32_t col, int32_t* pLen);
static SElExceptionPtr elOdbcGetColType(struct _SOdbc* pOdbc, int32_t col, int32_t* pType);
static SElExceptionPtr elOdbcExecSQL(struct _SOdbc* pOdbc);
static SElExceptionPtr elOdbcExecSQL2(struct _SOdbc* pOdbc, LPCTSTR szSQL);
static SElExceptionPtr elOdbcOnSetOptions(struct _SOdbc* pOdbc);
static SElExceptionPtr elOdbcCancel(struct _SOdbc* pOdbc);
static SElExceptionPtr elOdbcPrepareSQL(struct _SOdbc* pOdbc, LPCTSTR szSQL);
static int32_t elOdbcFetch(struct _SOdbc*	pOdbc, bool_t bBindColumns);
static int32_t elOdbcMoveNext(struct _SOdbc* pOdbc);
static SElExceptionPtr elOdbcBindColumns(struct _SOdbc* pOdbc);
static SElExceptionPtr elOdbcBindParam(
	struct _SOdbc* pOdbc,
	int32_t   paramNo, 
	int32_t   paramIo, 
	int32_t   paramValueType,
	int32_t   paramType,
	int32_t   columnSize,
	int32_t   decimalDigits,
	void*   lpBuffer,
	uint32_t   bufferSize,
	int32_t  *lpIndicator
);
static SElExceptionPtr elOdbcBindColumn(
	struct _SOdbc* pOdbc,
	int32_t   colNo, 
	int32_t   colType,
	void*  lpBuffer,
	uint32_t  bufferSize,
	int32_t  *lpIndicator
);
static SElExceptionPtr elOdbcGetFieldLen(struct _SOdbc* pOdbc, int32_t col, uint32_t* pLen);
static SElExceptionPtr elOdbcGetFieldValue(struct _SOdbc* pOdbc, int32_t col, LPVOID lpValue, uint32_t dwValueSize);
static void* elOdbcGetFieldValuePtr(struct _SOdbc* pOdbc, int32_t col);
static SElExceptionPtr elOdbcGetErrorMessage(struct _SOdbc* pOdbc, int32_t bufferSize, LPTSTR buffer);        

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "odbc_new"
SElExceptionPtr odbc_new(SOdbcPtr* pOdbc)
{
	SElExceptionPtr	pexception = NULL;
	SOdbcPtr		me = NULL;

	me = calloc( 1, sizeof(SOdbc));
	EXCCHECKALLOC( me);

	*pOdbc = me;
	
	me->_henv        	= SQL_NULL_HENV;
	me->_hdbc        	= SQL_NULL_HDBC;
	me->_hstmt       	= SQL_NULL_HSTMT;
	me->_ppData      	= NULL;
	me->_pIndicators 	= NULL;
	
	me->Open            = elOdbcOpen;
	me->Close           = elOdbcClose;
	me->PrepareSQL      = elOdbcPrepareSQL;
	me->Exec            = elOdbcExecSQL;
	me->ExecSQL         = elOdbcExecSQL2;
	me->Cancel          = elOdbcCancel;
	me->GetErrorMessage = elOdbcGetErrorMessage;
	me->SetQueryTimeout = elOdbcSetQueryTimeout;
	me->GetRows         = elOdbcGetRows;
	me->GetCols         = elOdbcGetCols;
	me->GetColName      = elOdbcGetColName;
	me->GetColLen       = elOdbcGetColLen;
	me->GetColType      = elOdbcGetColType;
	me->BindParam       = elOdbcBindParam;
	me->BindColumn      = elOdbcBindColumn;
	me->BindColumns     = elOdbcBindColumns;
	me->Fetch		   	= elOdbcFetch;
	me->MoveNext        = elOdbcMoveNext;
	me->GetFieldLen     = elOdbcGetFieldLen;
    me->GetFieldValue   = elOdbcGetFieldValue;
    me->GetFieldValuePtr= elOdbcGetFieldValuePtr;

	me->LogChannel = E_LOG_CHANNEL_ODBC;

Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "odbc_delete"
SElExceptionPtr	odbc_delete(SOdbcPtr* pOdbc)
{
	if ( pOdbc && *pOdbc )
	{
		(*pOdbc)->Close( *pOdbc);
		free( *pOdbc);
		*pOdbc = NULL;
	}

/* Error: */
	return 0;
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elOdbcOpen"
static SElExceptionPtr elOdbcOpen(
  struct _SOdbc* me,
  LPCTSTR        szDSN,
  LPCTSTR        szUser,
  LPCTSTR        szPass
)
{
	SElExceptionPtr	pexception = NULL;
	SQLRETURN 		sql_ret;

	/* Allocate an ODBC enviroment handle (Error : SQLAllocHandle:Env) */
	EXCCHECKSQL( SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &(me->_henv)));

	/* Set Attributes */
	EXCCHECKSQL( SQLSetEnvAttr(me->_henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER));

	/* Allocate an ODBC connection handle (Error : SQLAllocHandle:Dbc) */
	EXCCHECKSQL( SQLAllocHandle(SQL_HANDLE_DBC, me->_henv, &(me->_hdbc)));

	/* Connection to SQL Server (Error : SQLConnect) */
	EXCCHECKSQL( SQLConnect( me->_hdbc,
                        (SQLTCHAR *) szDSN,
                        SQL_NTS,
                        (SQLTCHAR *) szUser,
                        SQL_NTS,
                        (SQLTCHAR *) szPass,
                        SQL_NTS));


	/* Process data after successful connection (SQLAllocHandle:Stmt )*/
  	EXCCHECKSQL( SQLAllocHandle(SQL_HANDLE_STMT, me->_hdbc, &(me->_hstmt)));

	me->_time = time(NULL);

Error:
	//LOG(INFO, "Connection %s (DSN:%s, User:%s)", (pexception==NULL)? "succed":"failed",szDSN, szUser);
	EXCRETHROW( pexception); 
} 

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elOdbcClose"
static SElExceptionPtr elOdbcClose(
  struct _SOdbc* pOdbc
)
{
	elOdbcFreeBindColumns(pOdbc);

	if (pOdbc->_hstmt != SQL_NULL_HSTMT)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, pOdbc->_hstmt);
	}

	if (pOdbc->_hdbc != SQL_NULL_HDBC)
	{
		SQLDisconnect(pOdbc->_hdbc);
		SQLFreeHandle(SQL_HANDLE_DBC, pOdbc->_hdbc);
	}

	if (pOdbc->_henv != SQL_NULL_HENV)
	{
		SQLFreeHandle(SQL_HANDLE_ENV, pOdbc->_henv);
	}

	pOdbc->_hstmt = SQL_NULL_HSTMT;
	pOdbc->_hdbc  = SQL_NULL_HDBC;
	pOdbc->_henv  = SQL_NULL_HENV;

	return NULL;
}


/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elOdbcBindColumns"
static SElExceptionPtr elOdbcBindColumns(
	struct _SOdbc* pOdbc
)
{
	SElExceptionPtr	pexception = NULL;
	SQLRETURN		sql_ret;
	SQLSMALLINT 	sql_type;
	long 			col, cols;
    
	elOdbcFreeBindColumns( pOdbc);
	
	EXCCHECK( pOdbc->GetCols(pOdbc, &cols));

	// Allocate memory for DataBinding
	pOdbc->_cols        = cols;
	pOdbc->_ppData      = (LPBYTE *) calloc( cols, sizeof(LPBYTE));
	pOdbc->_pIndicators = (SQLINTEGER *) calloc( cols, sizeof(SQLINTEGER));

	for(col = 0; col < cols; col++)
	{
		int32_t col_len, col_type;
		
		EXCCHECK( pOdbc->GetColLen(pOdbc, col+1, &col_len));
		EXCCHECK( pOdbc->GetColType(pOdbc, col+1, &col_type));
		
		switch( col_type )
		{
			case MYSQL_BIGINT:
			case MYSQL_BLOB:  
				sql_type =SQL_C_BINARY;
				break;
			case SQL_CHAR:    
				col_len++; /* zero termination */    
				sql_type =  SQL_C_CHAR;
				break;
			case MYSQL_TINYINT:
				sql_type = SQL_C_TINYINT; 
				break;
			case SQL_DECIMAL: 
			case SQL_INTEGER: 
				sql_type = SQL_C_LONG;
				break;
			case SQL_SMALLINT: 
				sql_type = SQL_C_SHORT;
				break;
			case SQL_FLOAT:  
				sql_type = SQL_C_FLOAT;
				break;
			case SQL_REAL:  
				sql_type = SQL_C_FLOAT;
				break;
			case SQL_DOUBLE: 
				sql_type = SQL_C_DOUBLE;
				break;
			case SQL_DATE:
				sql_type = SQL_C_TIMESTAMP;
				break;
			case SQL_TIME:
				sql_type = SQL_C_TIME;
				break;
			case SQL_TIMESTAMP:
				sql_type = SQL_C_TIMESTAMP;
				break;
			case SQL_VARCHAR:
			case MYSQL_TEXT: 
				col_len++;  /* zero termination */
				sql_type = SQL_C_CHAR;
				break;
#if (ODBCVER >= 0x0300)
			case SQL_NUMERIC: 
				sql_type = SQL_C_LONG;
				break;
			case SQL_TYPE_DATE:
				sql_type = SQL_C_DATE;
				break;
			case SQL_TYPE_TIME: 
				sql_type = SQL_C_TIME;
				break;
			case SQL_TYPE_TIMESTAMP:
				col_len  = 20;
				sql_type = SQL_C_BINARY;
				break;
#endif
			default:
				sql_type = col_type;
				break;
		}
		
		if ( col_len )
			pOdbc->_ppData[col] = (LPBYTE) calloc(col_len, sizeof( BYTE ));

		EXCCHECKSQL( SQLBindCol( pOdbc->_hstmt,
							  (SQLSMALLINT) col+1,
							  sql_type,
							  pOdbc->_ppData[col],
							  col_len,
							  &(pOdbc->_pIndicators[col])));
	}	

Error:
    EXCRETHROW( pexception); 
}


/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elOdbcFreeBindColumns"
static SElExceptionPtr elOdbcFreeBindColumns(
	struct _SOdbc* pOdbc
)
{
	if (pOdbc->_ppData != NULL)
	{
		uint32_t col; 

		for(col = 0; col < pOdbc->_cols; col++)
		{
			free(pOdbc->_ppData[col]);
		}
		free(pOdbc->_ppData);
		pOdbc->_ppData = NULL;
		pOdbc->_cols = 0;
	}

	if (pOdbc->_pIndicators != NULL)
		free(pOdbc->_pIndicators);

	pOdbc->_ppData      = NULL;
	pOdbc->_pIndicators = NULL;
	
	return NULL;
} 

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elOdbcGetCols"
static SElExceptionPtr elOdbcGetCols(
	struct _SOdbc* pOdbc,
	int32_t*       pCount
)
{
    SQLSMALLINT cols  = 0;

	SQLNumResultCols(pOdbc->_hstmt, &cols);

	*pCount = (int32_t)cols;
	
	return NULL;	
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elOdbcGetRows"
static SElExceptionPtr elOdbcGetRows(
	struct _SOdbc* pOdbc,
	int32_t*       pCount
)
{
	SQLINTEGER rows  = 0;

	SQLRowCount(pOdbc->_hstmt, &rows);

	*pCount = (int32_t)rows;
	
	return NULL;	
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elOdbcGetColName"
static SElExceptionPtr elOdbcGetColName(
	struct _SOdbc* pOdbc,
    int32_t         col,
    LPTSTR          pBuffer,
    uint32_t        bufferSize
)
{
	SElExceptionPtr	pexception = NULL;
	SQLSMALLINT cb_attr;
	SQLINTEGER  num_attr;
	SQLRETURN	sql_ret;    

	memset((LPVOID)pBuffer, 0, sizeof(TCHAR) * bufferSize);

	EXCCHECKSQL( SQLColAttribute( pOdbc->_hstmt,
							  (SQLSMALLINT) col,
							  SQL_DESC_NAME,
							  (SQLTCHAR *) pBuffer,
							  (SQLSMALLINT)bufferSize,
							  &cb_attr,
							  &num_attr));
Error:
	EXCRETHROW( pexception);	
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elOdbcGetColLen"
static SElExceptionPtr elOdbcGetColLen(
	struct _SOdbc* pOdbc,
    int32_t         col,
    int32_t*        pLen
)
{
	SElExceptionPtr	pexception = NULL;
	SQLINTEGER  num_attr;
	SQLRETURN	sql_ret;

	EXCCHECKSQL( SQLColAttribute(pOdbc->_hstmt,
							  (SQLSMALLINT) col,
							  SQL_DESC_OCTET_LENGTH,
							  NULL,
							  0,
							  NULL,
							  &num_attr));

Error:
	*pLen = (int32_t) num_attr;
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elOdbcGetColType"
static SElExceptionPtr elOdbcGetColType(
	struct _SOdbc* pOdbc,
    int32_t         col,
    int32_t*        pType
)
{
	SElExceptionPtr	pexception = NULL;
	SQLINTEGER  num_attr;
	SQLRETURN	sql_ret;

	EXCCHECKSQL( SQLColAttribute(pOdbc->_hstmt,
							  (SQLSMALLINT) col,
							  SQL_DESC_TYPE,
							  NULL,
							  0,
							  NULL,
							  &num_attr));
	
Error:
    *pType = (int32_t) num_attr;
    EXCRETHROW( pexception);   
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elOdbcBindParam"
static SElExceptionPtr elOdbcBindParam(
	struct _SOdbc* pOdbc,
	int32_t   paramNo, 
	int32_t   paramIo, 
	int32_t   paramValueType,
	int32_t   paramType,
	int32_t   columnSize,
	int32_t   decimalDigits,
	void*   lpBuffer,
	uint32_t   bufferSize,
	int32_t  *lpIndicator
)
{
	SElExceptionPtr	pexception = NULL;
	SQLRETURN	sql_ret;

	EXCCHECKSQL( SQLBindParameter( pOdbc->_hstmt,
				 (SQLUSMALLINT) paramNo,
				 (SQLSMALLINT)  paramIo,
				 (SQLSMALLINT)  paramValueType,
				 (SQLSMALLINT)  paramType,
				 (SQLUINTEGER)  columnSize,
				 (SQLSMALLINT)  decimalDigits,
				 (SQLPOINTER)   lpBuffer,
				 (SQLINTEGER)   bufferSize,
				 (SQLINTEGER *) lpIndicator));
	
Error:
    EXCRETHROW( pexception); 
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elOdbcBindColumn"
static SElExceptionPtr elOdbcBindColumn(
	struct _SOdbc* pOdbc,
	int32_t   colNo, 
	int32_t   colType,
	void*     lpBuffer,
	uint32_t  bufferSize,
	int32_t  *lpIndicator
)
{
	SElExceptionPtr	pexception = NULL;
	SQLRETURN	sql_ret;

	EXCCHECKSQL( SQLBindCol( pOdbc->_hstmt,
						   (SQLSMALLINT) colNo,
						   (SQLSMALLINT) colType,
						   (SQLPOINTER ) lpBuffer,
						   (SQLINTEGER ) bufferSize,
						   (SQLLEN *)    lpIndicator));

Error:
    EXCRETHROW( pexception);
}


/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elOdbcPrepareSQL"
static SElExceptionPtr elOdbcPrepareSQL(
	struct _SOdbc* pOdbc,
	LPCTSTR szSQL
)
{
	SElExceptionPtr	pexception = NULL;
	SQLRETURN	sql_ret;

	elOdbcOnSetOptions( pOdbc);

	EXCCHECKSQL( SQLPrepare(pOdbc->_hstmt, (SQLTCHAR *) szSQL, SQL_NTS));

Error:
	EXCRETHROW( pexception);   
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elOdbcExecSQL"
static SElExceptionPtr elOdbcExecSQL(
	struct _SOdbc* pOdbc
)
{
	SElExceptionPtr	pexception = NULL;
	SQLRETURN		sql_ret;

	EXCCHECKSQL( SQLExecute(pOdbc->_hstmt));
	
Error:
	EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elOdbcOnSetOptions"
static SElExceptionPtr elOdbcOnSetOptions(
	struct _SOdbc* pOdbc
)
{
	//TCHAR sz_options[] = _T("SET QUOTED_IDENTIFIER OFF");

	SQLExecDirect(pOdbc->_hstmt, (SQLTCHAR *) "SET QUOTED_IDENTIFIER OFF", SQL_NTS); 
	SQLFreeStmt(pOdbc->_hstmt, SQL_RESET_PARAMS);
	
	return NULL;
}


/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elOdbcCancel"
static SElExceptionPtr elOdbcCancel(
	struct _SOdbc* pOdbc
)
{
	SQLFreeStmt( pOdbc->_hstmt, SQL_CLOSE);
	
	elOdbcFreeBindColumns(pOdbc);
	
	return NULL;
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elOdbcExecSQL2"
static SElExceptionPtr elOdbcExecSQL2(
	struct _SOdbc* me,
	LPCTSTR szSQL
)
{
	

#if defined(_PRODUCT_LINE)
	line_ctrl_t	*line = line_ctrl_ptr();
#endif
	SElExceptionPtr	pexception = NULL;
	SQLRETURN		sql_ret;

	//LOG(COMMUNICATION, "%s", szSQL);

	elOdbcOnSetOptions(me);

#if 0	/* exception log/display test: */
	if ( 0 || line->run && line->process && !line->startup )
		EXCCHECKSQL( SQLExecDirect(me->_hstmt, (SQLTCHAR *) "select test from test;", SQL_NTS));
#endif
	EXCCHECKSQL( SQLExecDirect(me->_hstmt, (SQLTCHAR *) szSQL, SQL_NTS));
	
	me->_time = time(NULL);

Error:
    if (pexception) printf(szSQL);
#if 1 && defined(_PRODUCT_LINE)
	if ( line->run && line->process && !line->startup )
		EXCRETHROW_NO_DISP(pexception); 
	else
#endif

	EXCRETHROW(pexception); 
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elOdbcSetQueryTimeout"
static SElExceptionPtr elOdbcSetQueryTimeout(
	struct _SOdbc* pOdbc,
	int32_t		   timeOut
)
{
	SElExceptionPtr	pexception = NULL;      
	SQLRETURN 		sql_ret;

	EXCCHECKSQL( SQLSetStmtAttr( pOdbc->_hstmt, SQL_ATTR_QUERY_TIMEOUT, &timeOut, SQL_IS_INTEGER));

Error:
	EXCRETHROW( pexception);   
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elOdbcFetch"
static int32_t elOdbcFetch(
	struct _SOdbc*	pOdbc,
	bool_t			bBindColumns /*= TRUE*/
)
{
	SElExceptionPtr	pexception = NULL;
	int32_t		error = 0;
	SQLRETURN	sql_ret;

	if ( bBindColumns )
	{
		EXCCHECK( pOdbc->BindColumns(pOdbc));
	}

	sql_ret = SQLFetch( pOdbc->_hstmt);
	
	if (sql_ret == SQL_NO_DATA_FOUND)
	{
		error = ELODBC_SQL_WARN_NO_DATA_FOUND;
	}
	else
	{
		CHECKSQL( sql_ret);
	}

Error:
	return error;
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elOdbcGetFieldLen"
static SElExceptionPtr elOdbcGetFieldLen(
   struct _SOdbc*   pOdbc,
   int32_t         col,
   uint32_t*       pLen
)
{
	SElExceptionPtr	pexception = NULL;  
    uint32_t    	cols;
    
    EXCCHECK( pOdbc->GetCols( pOdbc, &cols));

   if (col < 1 || cols < (uint32_t)col)
        EXCTHROW( -1, "SQL_ERROR_COLUMN_INDEX");

   *pLen = (pOdbc->_pIndicators[col-1] == SQL_NULL_DATA) ? 0 : pOdbc->_pIndicators[col-1];

Error:
    EXCRETHROW( pexception);  
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elOdbcGetFieldValue"
static SElExceptionPtr elOdbcGetFieldValue(
   struct _SOdbc*   pOdbc,
   int32_t         col,
   LPVOID          lpValue,
    uint32_t        dwValueSize
)
{
	SElExceptionPtr	pexception = NULL; 
   	uint32_t    	len;
    uint32_t    	cols;
    
    EXCCHECK( pOdbc->GetCols( pOdbc, &cols));

   	if (col < 1 || cols < (uint32_t)col)
        EXCTHROW( -1, "SQL_ERROR_COLUMN_INDEX");

   	EXCCHECK( pOdbc->GetFieldLen( pOdbc, col, &len));

    memcpy(lpValue, pOdbc->_ppData[col-1], (dwValueSize > len ) ? len : dwValueSize);
    
Error:
    EXCRETHROW( pexception);
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elOdbcGetFieldValuePtr"
static void* elOdbcGetFieldValuePtr(
   struct _SOdbc*   pOdbc,
   int32_t         col
)
{
	SElExceptionPtr	pexception = NULL;
    uint32_t    	cols;
    
    EXCCHECK( pOdbc->GetCols( pOdbc, &cols));
   
  	if (col < 1 || cols < (uint32_t)col)
        EXCTHROW( -1, "SQL_ERROR_COLUMN_INDEX"); 

Error:
   return (pexception!=NULL) ? NULL : (LPVOID) (pOdbc->_ppData[col-1]);
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elOdbcGetErrorMessage"
static SElExceptionPtr elOdbcGetErrorMessage(
	struct _SOdbc* pOdbc,
	int32_t	bufferSize,
	LPTSTR	buffer
)
{
    SQLTCHAR    sz_error[SQL_MAX_MESSAGE_LENGTH + 1];
    SQLSMALLINT cb_error;
    SQLSMALLINT rec = 1;

    if ( bufferSize && buffer )
		buffer[0] = '\0';

	SQLGetDiagField( SQL_HANDLE_STMT, 
						     pOdbc->_hstmt,
						     rec++,
						     SQL_DIAG_MESSAGE_TEXT,
						     sz_error,
						     SQL_MAX_MESSAGE_LENGTH, 
						     &cb_error);
	
	strncpy( buffer,sz_error, bufferSize);
	
	return NULL;
}

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "elOdbcMoveNext"
static int32_t elOdbcMoveNext(
	struct _SOdbc* pOdbc
)
{
	int32_t		error = 0;
	SQLRETURN	sql_ret;

	sql_ret = SQLFetch(pOdbc->_hstmt);

	if (sql_ret == SQL_NO_DATA_FOUND)
	{
		error = ELODBC_SQL_WARN_NO_DATA_FOUND;
	}
	else
	{
		CHECKSQL( sql_ret);	
	}

Error:
	return error;	
}
