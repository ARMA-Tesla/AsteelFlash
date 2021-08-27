/*
 *  "elErrWsa.c" - Windows Socket API error handling routines.
 *
 *	(c) 2007 ELCOM, a.s.
 *		All Rights Reserved.
 *
 *	The information  in this file  is provided for the exclusive use of
 *	the  licensees  of  ELCOM, a.s.  Such users have the right  to use,
 *	modify,  and incorporate this code into other products for purposes
 *	authorized  by the license  agreement  provided they  include  this
 *	notice  and the associated copyright notice  with any such product.
 *	The information in this file  is provided "AS IS" without warranty.
 *
 *
 * Revision History:
 * -----------------
 *	2007-03-05	Radomir Tomis (Radomir.Tomis@dvi.elcom.cz)
 *		Created.
 */

#if 1
static char _ident[] = "@(#) elErrWsa.c 0.1 2007-03-05";
#else
#endif

#include <elBase.h>
#include <elErr.h>
#include <elLog.h>
#include <elErrWin.h>
#include <elErrWsa.h>

static struct {
	int num;
	char *sym, *str;
} gs_wsa_error[] = {
	-1,					"WSA",				NULL,

	WSAEACCES,			"EACCES",
						"Permission denied",
	WSAEADDRINUSE,		"EADDRINUSE",
						"Address already in use",
	WSAEADDRNOTAVAIL,	"EADDRNOTAVAIL",
						"Cannot assign requested address",
	WSAEAFNOSUPPORT,	"EAFNOSUPPORT",
						"Address family not supported by protocol family",
	WSAEALREADY,		"EALREADY",
						"Operation already in progress",
	WSAECONNABORTED,	"ECONNABORTED",
						"Software caused connection abort",
	WSAECONNREFUSED,	"ECONNREFUSED",
						"Connection refused",
	WSAECONNRESET,		"ECONNRESET",
						"Connection reset by peer",
	WSAEDESTADDRREQ,	"EDESTADDRREQ",
						"Destination address required",
	WSAEFAULT,			"EFAULT",
						"Bad address",
	WSAEHOSTDOWN,		"EHOSTDOWN",
						"Host is down",
	WSAEHOSTUNREACH,	"EHOSTUNREACH",
						"No route to host",
	WSAEINPROGRESS,		"EINPROGRESS",
						"Operation now in progress",
	WSAEINTR,			"EINTR",
						"Interrupted function call",
	WSAEINVAL,			"EINVAL",
						"Invalid argument",
	WSAEISCONN,			"EISCONN",
						"Socket is already connected",
	WSAEMFILE,			"EMFILE",
						"Too many open files",
	WSAEMSGSIZE,		"EMSGSIZE",
						"Message too long",
	WSAENETDOWN,		"ENETDOWN",
						"Network is down",
	WSAENETRESET,		"ENETRESET",
						"Network dropped connection on reset",
	WSAENETUNREACH,		"ENETUNREACH",
						"Network is unreachable",
	WSAENOBUFS,			"ENOBUFS",
						"No buffer space available",
	WSAENOPROTOOPT,		"ENOPROTOOPT",
						"Bad protocol option",
	WSAENOTCONN,		"ENOTCONN",
						"Socket is not connected",
	WSAENOTSOCK,		"ENOTSOCK",
						"Socket operation on non-socket",
	WSAEOPNOTSUPP,		"EOPNOTSUPP",
						"Operation not supported",
	WSAEPFNOSUPPORT,	"EPFNOSUPPORT",
						"Protocol family not supported",
	WSAEPROCLIM,		"EPROCLIM",
						"Too many processes",
	WSAEPROTONOSUPPORT,	"EPROTONOSUPPORT",
						"Protocol not supported",
	WSAEPROTOTYPE,		"EPROTOTYPE",
						"Protocol wrong type for socket",
	WSAESHUTDOWN,		"ESHUTDOWN",
						"Cannot send after socket shutdown",
	WSAESOCKTNOSUPPORT,	"ESOCKTNOSUPPORT",
						"Socket type not supported",
	WSAETIMEDOUT,		"ETIMEDOUT",
						"Connection timed out",
	WSAEWOULDBLOCK,		"EWOULDBLOCK",
						"Resource temporarily unavailable",
	WSAHOST_NOT_FOUND,	"HOST_NOT_FOUND",
						"Host not found",
#if 1 && _MSC_VER < 1400	/* <winsock2.h> */
	WSA_INVALID_HANDLE,	"_INVALID_HANDLE",
						"Specified event object handle is invalid",
	WSA_INVALID_PARAMETER,	"_INVALID_PARAMETER",
						"One or more parameters are invalid",
	WSAEINVALIDPROCTABLE,	"EINVALIDPROCTABLE",
						"Invalid procedure table from service provider",
	WSAEINVALIDPROVIDER,"EINVALIDPROVIDER",
						"Invalid service provider version number",
	WSA_IO_PENDING,		"_IO_PENDING",
						"Overlapped operations will complete later",
	WSA_IO_INCOMPLETE,	"_IO_INCOMPLETE",
						"Overlapped I/O event object not in signaled state",
	WSA_NOT_ENOUGH_MEMORY,	"_NOT_ENOUGH_MEMORY",
						"Insufficient memory available",
#endif
	WSANOTINITIALISED,	"NOTINITIALISED",
						"Successful WSAStartup not yet performed",
	WSANO_DATA,			"NO_DATA",
						"Valid name, no data record of requested type",
	WSANO_RECOVERY,		"NO_RECOVERY",
						"This is a non-recoverable error",
#if 1	/* <winsock2.h> */
	WSAEPROVIDERFAILEDINIT,	"EPROVIDERFAILEDINIT",
						"Unable to initialize a service provider",
	WSASYSCALLFAILURE,	"SYSCALLFAILURE",
						"System call failure",
#endif
	WSASYSNOTREADY,		"SYSNOTREADY",
						"Network subsystem is unavailable",
	WSATRY_AGAIN,		"TRY_AGAIN",
						"Non-authoritative host not found",
	WSAVERNOTSUPPORTED,	"VERNOTSUPPORTED",
						"WINSOCK.DLL version out of range",
	WSAEDISCON,			"EDISCON",
						"Graceful shutdown in progress",
#if 1 && _MSC_VER < 1400	/* <winsock2.h> */
	WSA_OPERATION_ABORTED,	"_OPERATION_ABORTED",
						"Overlapped operation aborted",
#endif
};
#define WSA_ERRORs	EL_ARRAY_CNT(gs_wsa_error)


/*
 * Max. délka symbolického oznaèení chyby:
 *
 * ("ERROR_REQUIRES_INTERACTIVE_WINDOWSTATION" = 40)
 */
#define WIN_ERR_SYM_MAXLEN		50

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elErrWsaStartup"
EL_DLL_API elErr_t *elErrWsaStartup(void)
{
	EL_ERR_VAR;

	;

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elErrWsaCleanup"
EL_DLL_API elErr_t *elErrWsaCleanup(void)
{
	EL_ERR_VAR;

	;

	return err;
}

/******************************************************************************/
/*
 * 'elErrWsaSymStr()':
 *
 * Vrací symbolické (zkrácené) oznaèení ("WSA...") pro danou chybu typu
 * 'WSAGetLastError()' v prostøedí Windows Socket API.
 */
#undef  __FUNC__
#define __FUNC__	"elErrWsaSymStr"
EL_DLL_API TCHAR *elErrWsaSymStr(int errnum)
{
#define ERR_STR_LEN_MAX		64
	elStr_t			*errsym;
#if defined(UNICODE)
	wchar_t			ustr[30+ST];
#endif
	register TCHAR	*prefix;
	register int	i;

	errsym = elStrTmpIdxGet(EL_STR_IDX_ERR_WSA1);
	if ( elStrAlloc(errsym,ERR_STR_LEN_MAX+EL_ST) )
	{
		return
			_T("<")
			_T(__FUNC__) _T("(): cannot obtain temporary string")
			_T(">");
	}

	for(i = 0, prefix = _T(""); i < WSA_ERRORs; i++)
	{
		if ( gs_wsa_error[i].num == errnum )
		{
			errsym->Printf(errsym,_T("%s%s"),prefix,gs_wsa_error[i].sym);
			return errsym->ptr;
		}
		if ( gs_wsa_error[i].num == -1 )
		{
#if defined(UNICODE)
#pragma message("elErrWsaSymStr(): UNICODE version not implemented yet")
			ansi2unicode_str(gs_wsa_error[i].sym,ustr);
			prefix = ustr;
#else
			prefix = gs_wsa_error[i].sym;
#endif
		}
	}
	/*
	 * Nìkteré WinSock funkce (napø. 'gethostname()') mohou vracet i standardní
	 * chybové kódy Win32 API funkcí typu 'GetLastError()':
	 */
	if ( elErrWinHasStr(errnum) )
		return elErrWinSymStr(errnum);
	else
	{
		errsym->Printf(errsym,_T("WSA_ERROR_\?\?\? - %d"),errnum);
		return errsym->ptr;
	}

#undef ERR_STR_LEN_MAX
}

/******************************************************************************/
/*
 * 'elErrWsaSymNum()':
 *
 * Vrací hodnotu chyby typu 'WSAGetLastError()' (WinSock API) pro dané
 * symbolické (zkrácené) oznaèení ("WSA...") chyby typu 'WSAGetLastError()'
 * (WinSock API) (opak 'elErrWsaSymStr()').
 */
#undef  __FUNC__
#define __FUNC__	"elErrWsaSymNum"
EL_DLL_API int elErrWsaSymNum(TCHAR *errsym)
{
#if defined(UNICODE)
	wchar_t ustr[128+ST];
#endif
	register TCHAR *prefix;
	register int i, plen;

	for(i = 0, prefix = _T(""), plen = 0; i < WSA_ERRORs; i++)
	{
		if ( gs_wsa_error[i].num == -1 )
		{
#if defined(UNICODE)
#pragma message("elErrWsaSymNum(): UNICODE version not implemented yet")
			ansi2unicode_str(gs_wsa_error[i].str,ustr);
			plen = (int) _tcslen(prefix = ustr);
#else
			plen = (int) _tcslen(prefix = gs_wsa_error[i].str);
#endif
			continue;
		}
		/*
		 * Symbolické oznaèení mùže být s nebo bez prefixu
		 * ("WSAECONNREFUSED" nebo "ECONNREFUSED"):
		 */
#if defined(UNICODE)
		ansi2unicode_str(gs_wsa_error[i].sym,ustr);
		if ( !_tcscmp(ustr,errsym) ||
			!_tcsncmp(prefix,errsym,plen) &&
			!_tcscmp(ustr,errsym+plen) )
#else
		if ( !strcmp(gs_wsa_error[i].sym,errsym) ||
			!strncmp(prefix,errsym,plen) &&
			!strcmp(gs_wsa_error[i].sym,errsym+plen) )
#endif
			return gs_wsa_error[i].num;
	}
	return -1;
}

/******************************************************************************/
/*
 * 'elErrWsaStr()':
 *
 * Vrací øetìzec popisující danou chybu typu 'WSAGetLastError()' v prostøedí
 * Windows Socket API.
 */
#undef  __FUNC__
#define __FUNC__	"elErrWsaStr"
EL_DLL_API TCHAR *elErrWsaStr(int errnum)
{
#define ERR_STR_LEN_MAX		256
	elStr_t			*errstr;
	register TCHAR	*str;
	register int	i;

	errstr = elStrTmpIdxGet(EL_STR_IDX_ERR_WSA2);
	if ( elStrAlloc(errstr,ERR_STR_LEN_MAX+EL_ST) )
	{
		return
			_T("<")
			_T(__FUNC__) _T("(): cannot obtain temporary string")
			_T(">");
	}

	for(i = 0; i < WSA_ERRORs; i++)
		if ( gs_wsa_error[i].num == errnum )
			break;
	if ( i < WSA_ERRORs )
#if defined(UNICODE)
#pragma message("elErrWsaStr(): UNICODE version not implemented yet")
	{
		wchar_t ustr[256+ST];
		
		ansi2unicode_str(gs_wsa_error[i].str,ustr);
		str = ustr;
	}
#else
		str = gs_wsa_error[i].str;
#endif
	else
	{
		/*
		 * Nìkteré WinSock funkce (napø. 'gethostname()') mohou vracet i
		 * standardní chybové kódy Win32 API funkcí typu 'GetLastError()':
		 */
		if ( elErrWinHasStr(errnum) )
			return elErrWinStr(errnum);
		str = NULL;
	}
	errstr->Printf(errstr,_T("%s <%s/%d>"),str ? str : _T("\?\?\?"),
		elErrWsaSymStr(errnum),errnum);
	return errstr->ptr;

#undef ERR_STR_LEN_MAX
}

/******************************************************************************/
/*
 * 'elErrWsaGetLastErrorStr()':
 *
 * Vrací øetìzec popisující poslední chybu pøi volání funkce Windows Socket API.
 */
#undef  __FUNC__
#define __FUNC__	"elErrWsaGetLastErrorStr"
EL_DLL_API TCHAR *elErrWsaGetLastErrorStr(void)
{
	return elErrWsaStr(WSAGetLastError());
}

/******************************************************************************/
