#if !defined(__ELTYPES_H__)
#define __ELTYPES_H__

#include <os.h>

#if defined(UNIX)
#include <stdint.h>
#else
#include <stdint.h>
#endif

#if WIN
#include <stdlib.h>		/* 'size_t' */
#endif
#include <stdarg.h>		/* 'va_list' */

typedef int	bool_t;
#if (!defined(TRUE))
#define TRUE (1)
#endif
#if (!defined(FALSE))
#define FALSE (0)
#endif

#if !defined(bool) && !defined(_CURSES_H) && !defined(__cplusplus)
typedef unsigned char bool;
#endif

#if WIN && (defined(_MSC_VER) && !MINGW)
typedef unsigned int	uint;
#endif
#if WIN && ( (defined(_MSC_VER) && !MINGW) || defined(_CVI_) )
typedef __int64				int64_t;
typedef unsigned __int64	uint64_t;
#endif

#if !(SUN || SCO5 || DJGPP || LINUX)
typedef unsigned short	ushort;
#endif	/* !(SUN || SCO5 || DJGPP || LINUX) */
#if !(SUN || SCO5 || LINUX) || SUN4
typedef unsigned long	ulong;
#endif	/* !(SUN || SCO5 || LINUX) || SUN4 */

typedef unsigned char	uchar;
typedef unsigned char	elByte_t;
typedef int				elInt_t;
typedef int8_t			elInt8_t;
typedef uint8_t			elUInt8_t;
typedef int16_t			elInt16_t;
typedef uint16_t		elUInt16_t;
typedef int32_t			elInt32_t;
typedef uint32_t		elUInt32_t;
typedef elUInt32_t		elErrId_t;
typedef int64_t			elInt64_t;
typedef uint64_t		elUInt64_t;

#if _CVI_ || _MSC_VER <= 1200
#define strcpy_s(dst,size,src)	( (void) (size), strcpy(dst,src) )
#endif

#endif /* __ELTYPES_H__ */
