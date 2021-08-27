/*
 *  <elBigInt.h> - Definitions for module "elBigInt.c".
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
 *	2007-08-05	Radomir Tomis (Radomir.Tomis@dvi.elcom.cz)
 *		Created from "lib/LGInt.h" and "lib/LGIntStr.h".
 */

#if _MSC_VER >= 1000
#pragma once
#endif 

#if !defined(_EL_BIG_INT_H)
#define _EL_BIG_INT_H

#include <elBase.h>

/*
 * 'elBigInt_t'  - Big signed integer data type.
 * 'elBigUInt_t' - Big unsigned integer data type.
 *
 * 'WIN && MQX' may not use '__int64', because MQX provided 'printf()' routines
 * do not support "%I64"...
 */
#if WIN && !MQX

/*
 * Signed type 'elBigInt_t' using 'int64_t', because of unsupported conversion
 * from 'unsigned __int64' to 'double':
 */
typedef __int64						elBigInt_t;		/* signed type */
typedef unsigned __int64			elBigUInt_t;	/* unsigned type */
#define EL_BIG_INT_SIZEOF			8
#define EL_BIG_INT_FMTSZPFX			"I64"			/* format size prefix */
#define EL_BIG_UINT2double(val)		(double)(__int64)(val)

#elif defined(__GNUC__)

typedef long long					elBigInt_t;		/* signed type */
typedef unsigned long long			elBigUInt_t;	/* unsigned type */
#define EL_BIG_INT_SIZEOF			8
#define EL_BIG_INT_FMTSZPFX			"ll"			/* format size prefix */
#define EL_BIG_INT_CONST(val)		val##ll
#define EL_BIG_UINT_CONST(val)		val##ull
#define EL_BIG_UINT2double(val)		(double)(val)

#elif UNIX || MQX

typedef long						elBigInt_t;		/* signed type */
typedef unsigned long				elBigUInt_t;	/* unsigned type */
#define EL_BIG_INT_SIZEOF			4
#define EL_BIG_INT_FMTSZPFX			"l"				/* format size prefix */
#define EL_BIG_UINT2double(val)		(double)(val)

#endif

#define EL_BIG_UINT_SIZEOF	EL_BIG_INT_SIZEOF

/*
 * 'EL_BIG_INT_MAX_DECIMAL_STRLEN'
 * 'EL_BIG_INT_MAX_DECIMAL_STRLEN_STR'
 *	- max. string length for decimal representation of max. 'elBigInt_t' value
 * 'EL_BIG_UINT_MAX_DECIMAL_STRLEN'
 * 'EL_BIG_UINT_MAX_DECIMAL_STRLEN_STR'
 *	- max. string length for decimal representation of max. 'elBigUInt_t' value
 * 'EL_BIG_INT_MAX_DECIMAL_STRLEN_WSEP'
 * 'EL_BIG_INT_MAX_DECIMAL_STRLEN_WSEP_STR'
 *	- ='EL_BIG_INT_MAX_DECIMAL_STRLEN' 'w'ith thousand 'sep'arators
 * 'EL_BIG_UINT_MAX_DECIMAL_STRLEN_WSEP'
 * 'EL_BIG_UINT_MAX_DECIMAL_STRLEN_WSEP_STR'
 *	- ='EL_BIG_UINT_MAX_DECIMAL_STRLEN' 'w'ith thousand 'sep'arators
 */
#if EL_BIG_INT_SIZEOF == 4

/* "-2147483648" (-2^31): */
#define EL_BIG_INT_MAX_DECIMAL_STRLEN			11
#define EL_BIG_INT_MAX_DECIMAL_STRLEN_STR		"11"
/* "-2'147'483'648" (-2^31): */
#define EL_BIG_INT_MAX_DECIMAL_STRLEN_WSEP		14
#define EL_BIG_INT_MAX_DECIMAL_STRLEN_WSEP_STR	"14"
/* "-1Gi+1023Mi+1023Ki+1023" (-2^31-1): */
#define EL_BIG_INT_MAX_MMULT_STRLEN				23
#define EL_BIG_INT_MAX_MMULT_STRLEN_STR			"23"

/* "4294967295" (2^32-1): */
#define EL_BIG_UINT_MAX_DECIMAL_STRLEN			10
#define EL_BIG_UINT_MAX_DECIMAL_STRLEN_STR		"10"
/* "4'294'967'295" (2^32-1): */
#define EL_BIG_UINT_MAX_DECIMAL_STRLEN_WSEP		13
#define EL_BIG_UINT_MAX_DECIMAL_STRLEN_WSEP_STR	"13"
/* "3Gi+1023Mi+1023Ki+1023" (2^32-1): */
#define EL_BIG_UINT_MAX_MMULT_STRLEN			22
#define EL_BIG_UINT_MAX_MMULT_STRLEN_STR		"22"

#elif EL_BIG_INT_SIZEOF == 8

/* "-9223372036854775808" (-2^63): */
#define EL_BIG_INT_MAX_DECIMAL_STRLEN			20
#define EL_BIG_INT_MAX_DECIMAL_STRLEN_STR		"20"
/* "-9'223'372'036'854'775'808" (-2^63): */
#define EL_BIG_INT_MAX_DECIMAL_STRLEN_WSEP		26
#define EL_BIG_INT_MAX_DECIMAL_STRLEN_WSEP_STR	"26"
/* "-7Ei+1023Pi+1023Ti+1023Gi+1023Mi+1023Ki+1023" (-2^63-1): */
#define EL_BIG_INT_MAX_MMULT_STRLEN				44
#define EL_BIG_INT_MAX_MMULT_STRLEN_STR			"44"

/* "18446744073709551615" (2^64-1): */
#define EL_BIG_UINT_MAX_DECIMAL_STRLEN			20
#define EL_BIG_UINT_MAX_DECIMAL_STRLEN_STR		"20"
/* "18'446'744'073'709'551'615" (2^64-1): */
#define EL_BIG_UINT_MAX_DECIMAL_STRLEN_WSEP		26
#define EL_BIG_UINT_MAX_DECIMAL_STRLEN_WSEP_STR	"26"
/* "17Ei+1023Pi+1023Ti+1023Gi+1023Mi+1023Ki+1023" (2^64-1): */
#define EL_BIG_UINT_MAX_MMULT_STRLEN			44
#define EL_BIG_UINT_MAX_MMULT_STRLEN_STR		"44"

#else
#error Unsupported 'EL_BIG_INT_SIZEOF'
#endif

/*
 * 'EL_BIG_INT_STR_HEXPFX_DOLLAR_SIGN':
 *
 * This constant determines whether hexadecimal prefix "$" can be used to
 * introduce hexadecimal numbers as well as "0x" prefix.
 */
#define EL_BIG_INT_STR_HEXPFX_DOLLAR_SIGN		1

/*
 * 'EL_BIG_INT_STR_BINPFX':
 *
 * This constant determines whether binary prefix "&" can be used to introduce
 * binary numbers (e.g. "&1111" = 15).
 */
#define EL_BIG_INT_STR_BINPFX		1

#ifdef __cplusplus
extern "C" {			/* C Plus Plus function bindings */
#endif

EL_DLL_API	char	*elBigIntVal2StrTSep(elBigUInt_t val);
EL_DLL_API	char	*elBigIntVal2StrMMult(elBigUInt_t val,bool binary);
EL_DLL_API	char	*elBigIntVal2StrMMultOpt(char *fmt,elBigUInt_t val,bool binary);
EL_DLL_API	bool	 elBigIntStr2ValMMult(char *str,elBigUInt_t *pval,char **pstrend);
EL_DLL_API	bool	 elBigIntStr2ValSMult(char *str,elBigUInt_t *pval,char **pstrend);
EL_DLL_API	double	 elBigIntVal2RValSMult(elBigUInt_t val,bool binary,char **mult_sym);
EL_DLL_API	char	*elBigIntVal2RStrSMult(elBigUInt_t val,bool binary,int prec);
EL_DLL_API	bool	 elBigIntMult(int mult_idx,char *mult_sym,char *mult_name,elBigInt_t *pval);

#ifdef __cplusplus
}						/* C Plus Plus function bindings */
#endif

#endif	/* _EL_BIG_INT_H */