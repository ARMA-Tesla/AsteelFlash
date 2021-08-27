/*============================================================================*/
/*                        L a b W i n d o w s / C V I                         */
/*----------------------------------------------------------------------------*/
/*    Copyright (c) National Instruments 1987-1999.  All Rights Reserved.     */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Title:       formatio.h                                                    */
/* Purpose:     Include file for LabWindows formatted I/O library.            */
/*                                                                            */
/*============================================================================*/

#ifndef _FORMATIO_H
#define _FORMATIO_H

/* This header must be included after any Windows SDK header or any header */
/* that directly or indirectly includes a Windows SDK header.              */
/* Some of the function names conflict with Windows SDK functions.         */
#ifndef INCLUDE_AFTER_WINDOWS_H
#define INCLUDE_AFTER_WINDOWS_H
#endif /* INCLUDE_AFTER_WINDOWS_H */

#include "cvidef.h"
#include "cvirte.h"

#ifdef __cplusplus
    extern "C" {
#endif

typedef enum {
    FmtIONoErr          = 0,
    FmtIONoFileErr      = 1,
    FmtIOGenErr         = 2,
    FmtIOBadHandleErr   = 3,
    FmtIOInsuffMemErr   = 4,
    FmtIOFileExistsErr  = 5,    /* not used */
    FmtIOAccessErr      = 6,
    FmtIOInvalArgErr    = 7,
    FmtIOMaxFilesErr    = 8,
    FmtIODiskFullErr    = 9,
    FmtIONameTooLongErr = 10,
    _unusedFmtIOErrType = 0x7fffffff    /* makes this type equivalent to int */

} FmtIOErrType;

extern int CVIFUNC_C    FmtOut(const char *, ...);
extern int CVIFUNC_C    Fmt(void *, const char *, ...);
extern int CVIFUNC_C    FmtFile(int, const char *, ...);
extern int CVIFUNC_C    ScanIn(const char *, ...);
extern int CVIFUNC_C    Scan(void *, const char *, ...);
extern int CVIFUNC_C    ScanFile(int, const char *, ...);
extern int CVIFUNC      NumFmtdBytes(void);
extern int CVIFUNC      GetFmtErrNdx(void);

#ifdef _NI_mswin32_
extern int CVIFUNC      cviprefix(CloseFile)(int handle);
extern int CVIFUNC      cviprefix(OpenFile)(const char *filename,int rd_wrt, int action, int bin_ascii);
extern int CVIFUNC      cviprefix(ReadFile)(int handle, char *buf, int count);
extern int CVIFUNC      cviprefix(WriteFile)(int handle, const char *buf, int count);
extern int CVIFUNC      cviprefix(SetCommitMode)(int commit);
#define CloseFile       cviprefix(CloseFile)
#define OpenFile        cviprefix(OpenFile)
#define ReadFile        cviprefix(ReadFile)
#define WriteFile       cviprefix(WriteFile)
#define SetCommitMode   cviprefix(SetCommitMode)
#else
extern int CVIFUNC      CloseFile(int handle);
extern int CVIFUNC      OpenFile(const char *filename,int rd_wrt, int action, int bin_ascii);
extern int CVIFUNC      ReadFile(int handle, char *buf, int count);
extern int CVIFUNC      WriteFile(int handle, const char *buf, int count);
extern int CVIFUNC      SetCommitMode(int commit);
#endif
 
extern int CVIFUNC      GetFileInfo(const char *filename, long *size);
extern long CVIFUNC     SetFilePtr(int handle, long offset, int origin);
extern int CVIFUNC      CompareBytes(const char* s1, int s1i, const char* s2, int s2i,
                             int n, int case_sens);
extern int CVIFUNC      CompareStrings(const char* s1, int s1i, const char* s2,
                               int s2i, int case_sens);
extern void CVIFUNC     CopyBytes(char* d, int di, const char* s, int si, int n);
extern void CVIFUNC     CopyString(char* d, int di, const char* s, int si, int max_n);
extern int CVIFUNC      FindPattern(const char s[], int si, int n, const char* pattern,
                            int case_sens, int start_from_right);
extern int CVIFUNC      ReadLine(int handle, char line[], int max_n);
extern int CVIFUNC      StringLength(const char* s);
extern void CVIFUNC     StringLowerCase(char* s);
extern void CVIFUNC     StringUpperCase(char* s);
extern int CVIFUNC      WriteLine(int handle, const char line[], int n);
extern void CVIFUNC     FillBytes (char *d, int di, int n, int ch);

extern int CVIFUNC      ArrayToFile (const char *filename, const void *array, int dataType,
                            int numElements, int numGroups, int dataOrder,
                            int fileLayout, int sepStyle, int fieldWidth,
                            int fileType, int fileAction);
extern int CVIFUNC      FileToArray (const char *filename, void *array, int dataType,
                            int numElements, int numGroups, int dataOrder,
                            int fileLayout, int fileType);

extern FmtIOErrType CVIFUNC     GetFmtIOError(void);
extern char * CVIFUNC       GetFmtIOErrorString(FmtIOErrType);

#ifdef _CVI_USE_FUNCS_FOR_VARS_
    extern FmtIOErrType * CVIFUNC   _GetFmtIOError(void);
    #define FmtIOError  (*_GetFmtIOError())
#endif

/*** file modes (for OpenFile): ***/
#define VAL_READ_WRITE 0
#define VAL_READ_ONLY 1
#define VAL_WRITE_ONLY 2

    /*** file actions (for OpenFile, ArrayToFile): ***/
#define VAL_TRUNCATE 0
#define VAL_APPEND 1
#define VAL_OPEN_AS_IS 2

    /*** file types (for OpenFile, ArrayToFile, FileToArray): ***/
#define VAL_BINARY 0
#define VAL_ASCII 1

    /*** data separation styles (for ArrayToFile, FileToArray): ***/
#define VAL_CONST_WIDTH 0
#define VAL_SEP_BY_COMMA 1
#define VAL_SEP_BY_TAB 2

    /*** file layout styles (for ArrayToFile, FileToArray): ***/
#define VAL_GROUPS_AS_COLUMNS 0
#define VAL_GROUPS_AS_ROWS 1

    /*** array data orderings (for ArrayToFile, FileToArray): ***/
#define VAL_GROUPS_TOGETHER 0
#define VAL_DATA_MULTIPLEXED 1

    /*** data types (for ArrayToFile, FileToArray): ***/
#define VAL_CHAR                        0L
#define VAL_INTEGER                     1L
#define VAL_SHORT_INTEGER               2L
#define VAL_FLOAT                       3L
#define VAL_DOUBLE                      4L
#define VAL_STRING                      5L
#define VAL_UNSIGNED_SHORT_INTEGER      6L
#define VAL_UNSIGNED_INTEGER            7L
#define VAL_UNSIGNED_CHAR               8L
#define VAL_NO_TYPE                     9L

#ifdef __cplusplus
    }
#endif

#endif /* ndef _FORMATIO_H */
