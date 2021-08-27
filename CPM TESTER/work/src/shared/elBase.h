/*
 *  <elBase.h> - Base general definitions.
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
 *		Created/extracted from <ads.h> and other header files.
 * 
 *
 * Supported OS:
 * -------------
 *
 *	OS							Manifest constant(s)
 *	-------------------------------------------------------------------------
 *  SCO							SCO, SCO4, SCO5, SCO41, SCO42, SCO500, SCO502,
 *								SCO504
 *	ULTRIX						ULTRIX
 *	OSF/1 (DIGITAL UNIX)		OSF1
 *	SunOS						SUN, SUN4, SUN5
 *	Windows 3.11, 95/98/Me, NT	WIN
  *	Windows 2000/XP/2003/Vista	WIN
 *	Precise/MQX					MQX
 */

#if /* (WIN32 || _WIN32 || _WINDOWS) && */ _MSC_VER >= 1000
#pragma once
#endif 

#if !defined(_EL_BASE_H)
#define _EL_BASE_H

#if _MSC_VER < 1400 && !__BCOPT__ && !_CVI_  /* Borland C/C++: Unknown preprocessor directive: 'ident' */
#if 1
#ident "@(#) elBase.h 0.1 2007-03-05"
#else
#endif
#endif

#include <elTypes.h>

#if !defined(_MFC_VER)		/* !MFC (!<afx.h>) */
/*
 * <winsock2.h> pøed <windows.h>, aby se z nìj nevzal <winsock.h>, ve kterém
 * napø. chybí konstanty 'SD_{SEND|RECEIVE|BOTH}' ('shutdown()'):
 */
#if defined(_MSC_VER)
/*
 * It seems that warning 4201 ("nameless struct/union"), unlike e.g. 4115,
 * cannot be disabled outside of (e.g. here) the include file that is causing
 * the warning (e.g. "C:\program files\microsoft visual studio\vc98\include\qos.h").
 * So the include file itself (e.g. "...\qos.h") has been modified to include
 * the '#pragma warning(disable|default:...)' lines.
 */
#pragma warning(disable : 4115 4201)
#pragma warning(disable : 4996)	/* "The POSIX name for this item is deprecated" */
#pragma warning(disable : 4102)	/* "'...' : unreferenced label" */
#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif
#if !_CVI_ && _MSC_VER <= 1200
#include <winsock2.h>
#endif
#include <windows.h>   		/* pro vsechny Windows aplikace	   */
#if defined(_MSC_VER )
#pragma warning(default : 4115 4201)
#endif
#endif	/* !_MFC_VER */
#include <winsvc.h>			/* Windows services */
#if !_CVI_
#include <tchar.h>
#endif
#include <stdio.h>


#define EL_ST		1	/* string terminator (space for '\0') */

#define EL_FLAG(x)	(1 << (x))

#if !defined(MAX)
#	define MAX(a,b)			( (a) > (b) ? (a) : (b) )
#endif
#if !defined(MIN)
#	define MIN(a,b)			( (a) < (b) ? (a) : (b) )
#endif
#if !defined(ABS)
#	define ABS(n)			( (n) < 0 ? -(n) : (n) )
#endif
#if !defined(SGN)
#	define SGN(n)			( (n) < 0 ? -1 : +1 )
#endif
#if !defined(CTRL)
#	define CTRL(c)			((c) & 0x1F )
#endif

#define EL_KiB				1024
#define EL_KB				EL_KiB
#define EL_MiB				((ulong)EL_KiB * EL_KiB)
#define EL_MB				EL_MiB
#define EL_GiB				((ulong)EL_MiB * EL_KiB)
#define EL_GB				EL_GiB

#if defined(EL_DLL_EXPORTS)
#define EL_DLL_EXPORT		__declspec( dllexport )
#else
#define EL_DLL_EXPORT
#endif
#define EL_DLL_API		EL_DLL_EXPORT
#if defined(EL_DLL_IMPORTS)
#define EL_DLL_IMPORT		__declspec( dllimport )
#else
#define EL_DLL_IMPORT
#endif

/*
 * '__P(args)' makro pro transparentni prototypy funkci (EL_OLD_PROTOTYPES...)
 * Pouziti:		char *fn __P((char *fmt,...));
 */
#if !defined(__P)
#	if EL_OLD_PROTOTYPES
#		define	__P(args)	()
#	else
#		define	__P(args)	args
#	endif
#endif

#if WIN
/* 'STDIN_...' - e.g. for 'isatty()': */
#define STDIN_FILENO			0
#define STDOUT_FILENO			1
#define STDERR_FILENO			2
#endif

#define EL_T_SEC				1L
#define EL_T_MIN				(60 * EL_T_SEC)
#define EL_T_HOUR				(60 * EL_T_MIN)
#define EL_T_DAY				(24 * EL_T_HOUR)

/*
 * No UNICODE on UNIX and in CVI at the moment:
 */
#if UNIX || _CVI_ || CYGWIN
#define TCHAR		char
#define TUCHAR		uchar
#define _TCHAR		char
#define _TUCHAR		uchar
#define _T(ch)		ch

#define _tprintf	printf
#define _ftprintf	fprintf
#define _fputtc		fputc
#define _fputts		fputs
#define _fgetts		fgets
#define _istspace	isspace
#define _tcreat		creat
#define _tcscat		strcat
#define _tcschr		strchr
#define _tcsrchr	strrchr
#define _tcslen		strlen
#define _tcscmp		strcmp
#define _tcsncmp	strncmp
#define _tcscpy		strcpy
#define _tcsdup		strdup
#define _tcsicmp	stricmp
#define _tcsnicmp	strnicmp
#define _tcscat		strcat
#define _tcscat		strcat
#define _tcsncat	strncat
#define _tcsncpy	strncpy
#define _tcspbrk	strpbrk
#define _tcsstr		strstr
#define _tfopen		fopen
#define _tfclose	fclose
#define _tgetenv	getenv
#define _ttoi		atoi
#define _ttol		atol
#define _stprintf	sprintf
#define _stscanf	sscanf
#define _vstprintf	vsprintf
#define _istdigit	isdigit
#define _tcsftime	strftime
#define _tmain		main
#endif

/*
 * Number of elements in an array:
 */
#define EL_ARRAY_CNT(array)		(int)( sizeof(array) / sizeof(array[0]) )

/*
 * 'EL_STCMBR_{OFFSET|SIZE}()': STruCture MemBeR's offset & size.
 */
#if WIN && _MSC_VER >= 1400
#if 1
#define	EL_STCMBR_OFFSET(stc_type,member)	\
		(int) ( (char *)&(((stc_type *)NULL)->member) - (char *)NULL )
#else
#define	EL_STCMBR_OFFSET(stc_type,member)	(intptr_t)&(((stc_type *)NULL)->member)
#endif
#else
#define	EL_STCMBR_OFFSET(stc_type,member)	(int)&(((stc_type *)NULL)->member)
#endif
#define	EL_STCMBR_SIZE(stc_type,member)		(int)(sizeof ((stc_type *)NULL)->member)
#if defined(offsetof)
#	undef	offsetof
#endif
#define	offsetof	EL_STCMBR_OFFSET

#if 0
/*
 * Constant conditional expression for MSVC 6.0 compiler warning level 3:
 *
 * The constant expression causes the following warning to be reported
 * when warning level is set to 4:
 *
 *	warning C4127: conditional expression is constant
 */
#define EL_CONST_COND_1		1
#define EL_CONST_COND_0		0
#else
/*
 * Non-constant conditional expression for MSVC 6.0 compiler warning level 4:
 */
#if !defined(EL_BASE_MODULE)
EL_DLL_IMPORT extern int g_el_const_cond_1;
EL_DLL_IMPORT extern int g_el_const_cond_0;
#endif
#define EL_CONST_COND_1		g_el_const_cond_1
#define EL_CONST_COND_0		g_el_const_cond_0
#endif

#endif 	/* _EL_BASE_H */
