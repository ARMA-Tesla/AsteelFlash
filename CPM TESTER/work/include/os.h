#if !defined(__OS_H__)
#define __OS_H__

/*
 * Set OS according to predefined manifest constants:
 */

#if ultrix
#	define ULTRIX	1
#endif

#if __osf__
#	define OSF1		1
#endif

#if __sun || SUN4 || SUN5
#	define SUN		1
#endif

#if !WIN && defined(__MQX__)
#	define MQX		1
#endif

#if SCO41 || SCO42
#	define SCO4		1
#endif

#if SCO500 || SCO502 || SCO504
#	define SCO5		1
#endif

#if SCO4 || SCO5
#	define SCO		1
#endif

#if defined(__unix) || defined(__linux) || defined(__linux__)
#	define LINUX	1
#endif


#if defined(__unix__) || defined(__unix) || LINUX || SCO || ULTRIX || OSF1 || SUN
#	define _FUNC
#	define _FUNCC
#	define UNIX        1
#elif (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)) && !defined(_NI_mswin16_) && !MQX
#	define _FUNC            __stdcall
#	define _FUNCC           __cdecl
#	define		WINDOWS     1
#	define	WIN			1
#elif defined(_CVI_) && defined(_NI_i386_)
#	define _FUNC            _pascal
#	define _FUNCC
#	define WINDOWS     1
#elif (defined(_WINDOWS) || defined(_Windows)) && !defined(_NI_mswin16_)
#	define _FUNC            _far _pascal _export
#	define _FUNCC           _far _cdecl  _export
#	define WINDOWS     1
#elif (defined(hpux) || defined(__hpux)) && (defined(__cplusplus) || defined(__cplusplus__))
#	define _FUNC
#	define _FUNCC
#	define UNIX        1
#else
#	define _FUNC
#	define _FUNCC
#	define UNIX        1
#endif

#if defined(__GNUC__)
#	define GCC_VERSION (__GNUC__ *10000 + __GNUC_MINOR__ *100 + __GNUC_PATCHLEVEL__)
/* Test for GCC > 3.2.0  #if GCC_VERSION > 30200 */
#endif

#endif /* __OS_H__ */
