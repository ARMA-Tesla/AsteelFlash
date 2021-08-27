/*============================================================================*/
/*                        L a b W i n d o w s / C V I                         */
/*----------------------------------------------------------------------------*/
/*    Copyright (c) National Instruments 1987-1999.  All Rights Reserved.     */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Title:       mbsupp.h                                                      */
/* Purpose:     Include file for ANSI Standard C Library support of functions */
/*              to manipulate strings and array of multibyte characters       */
/*                                                                            */
/*============================================================================*/

#ifndef _MBSUPP
#define _MBSUPP

#include "cvidef.h"
#include "cvirte.h"

#ifdef __cplusplus
    extern "C" {
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef _SIZE_T_DEFINED
#define _SIZE_T_DEFINED
typedef unsigned int size_t;
#endif

/* Error code for mbsicmp, mbsnicmp if uppercase conversion fails. */
#define _NLSCMPERROR    2147483647

/* Special return value for getmbcp to signal that a single byte   */
/* code page is currently in use.                                  */
#define _MB_CP_SBCS             0

/* Byte types returned by mbsbtype.                                */
#define _MBC_SINGLE     0
#define _MBC_LEAD       1
#define _MBC_TRAIL      2
#define _MBC_ILLEGAL   -1

#define _getmbcp        cviprefix(_getmbcp)
#define _ismbblead      cviprefix(_ismbblead)
#define _mbsbtype       cviprefix(_mbsbtype)
#define _mbsdec         cviprefix(_mbsdec)
#define _mbsinc         cviprefix(_mbsinc)
#define _mbscmp         cviprefix(_mbscmp)
#define _mbsicmp        cviprefix(_mbsicmp)
#define _mbsnbcmp       cviprefix(_mbsnbcmp)
#define _mbsnbicmp      cviprefix(_mbsnbicmp)
#define _mbscat         cviprefix(_mbscat)
#define _mbsnbcat       cviprefix(_mbsnbcat)
#define _mbscpy         cviprefix(_mbscpy)
#define _mbsnbcpy       cviprefix(_mbsnbcpy)
#define _mbsstr         cviprefix(_mbsstr)
#define _mbschr         cviprefix(_mbschr)
#define _mbsrchr        cviprefix(_mbsrchr)
#define _mbstok         cviprefix(_mbstok)
#define _mbspbrk        cviprefix(_mbspbrk)
#define _mbslen         cviprefix(_mbslen)
#define _mbscspn        cviprefix(_mbscspn)
#define _mbsspn         cviprefix(_mbsspn)

extern int  __CVI_SystemCodePage;   /* same as _getmbcp() */

#define OnMBSystem()    (__CVI_SystemCodePage != _MB_CP_SBCS)

int    CVIANSI _getmbcp(void);
int    CVIANSI _ismbblead(unsigned int);
int    CVIANSI _mbsbtype(const unsigned char *, size_t);
unsigned char * CVIANSI _mbsdec(const unsigned char *, const unsigned char *);
unsigned char * CVIANSI _mbsinc(const unsigned char *);
int    CVIANSI _mbscmp(const unsigned char *, const unsigned char *);
int    CVIANSI _mbsicmp(const unsigned char *, const unsigned char *);
int    CVIANSI _mbsnbcmp(const unsigned char *, const unsigned char *, size_t);
int    CVIANSI _mbsnbicmp(const unsigned char *, const unsigned char *, size_t);
unsigned char * CVIANSI _mbscat(unsigned char *, const unsigned char *);
unsigned char * CVIANSI _mbsnbcat(unsigned char *, const unsigned char *, size_t);
unsigned char * CVIANSI _mbscpy(unsigned char *, const unsigned char *);
unsigned char * CVIANSI _mbsnbcpy(unsigned char *, const unsigned char *, size_t);
unsigned char * CVIANSI _mbsstr(const unsigned char *, const unsigned char *);
unsigned char * CVIANSI _mbschr(const unsigned char *, unsigned int); 
unsigned char * CVIANSI _mbsrchr(const unsigned char *, unsigned int);
unsigned char * CVIANSI _mbstok(unsigned char *, const unsigned char *);
unsigned char * CVIANSI _mbspbrk(const unsigned char *, const unsigned char *);
size_t CVIANSI _mbslen(const unsigned char *);
size_t CVIANSI _mbscspn(const unsigned char *, const unsigned char *);
size_t CVIANSI _mbsspn(const unsigned char *, const unsigned char *);

#ifdef __cplusplus
    }
#endif

#endif


