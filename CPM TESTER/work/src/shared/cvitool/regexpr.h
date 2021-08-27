/*============================================================================*/
/*                        L a b W i n d o w s / C V I                         */
/*----------------------------------------------------------------------------*/
/*    Copyright (c) National Instruments 1987-1999.  All Rights Reserved.     */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Title:       regexpr.h                                                     */
/* Purpose:     provides interface for using regular expressions to find      */
/*              patterns in text.                                             */
/*                                                                            */
/* Warning:     The functions declared in this file are NOT multithread-safe. */
/*                                                                            */
/*============================================================================*/

#ifndef _REGEXPR_HEADER_
#define _REGEXPR_HEADER_

#include "toolbox.h"

#ifdef __cplusplus
    extern "C" {
#endif


enum {
    RegExprErr_NoErr = 0,
    RegExprErr_OutOfMemory = UIEOutOfMemory,   /* -12 */
    RegExprErr_UnmatchedChar = -7900,
    RegExprErr_InvalidCharInRange = -7901,
    RegExprErr_EndWithBackslash = -7902,
    RegExprErr_InvalidHexValue = -7903,
    RegExprErr_OperatorOnEmptyPattern = -7904,
    RegExprErr_EmptyLeftSide = -7905,
    RegExprErr_EmptyRightSide = -7906,
    RegExprErr_EmptyGroup = -7907,
    RegExprErr_InvalidRange = -7908,
    RegExprErr_EmptySet = -7909,
    RegExprErr_EmptyInputString = -7910,
    RegExprErr_NullInputString = -7911,
    RegExprErr_MultibyteCharsNotAllowedInRange = -7912
};

enum {  /* return values from RegExpr_MatchTree */
    RegExpr_Match = 1,      /* successful match */
    RegExpr_NoMatch = 0,    /* no match at current startCol */
    RegExpr_NotInText = -1  /* no match at or beyond current startCol */
};

enum {  /* direction parameter to RegExpr_FindPatternInText */
    RegExpr_SearchForwards = 1,  /* any non-zero value is interpreted as "search forwards" */
    RegExpr_SearchBackwards = 0
};

enum {  /* matchPolicy parameter to RegExpr_FindPatternInText and RegExpr_MatchText */
    RegExpr_MatchLargestNumChars = 1,  /* any non-zero value is interpreted as "match max num chars" */
    RegExpr_MatchSmallestNumChars = 0
};


int  CVIFUNC RegExpr_Parse (const char *text, int caseSensitive, int *handle);

int  CVIFUNC RegExpr_MatchText (int handle, const char *text, int txtLen,
                                int matchPolicy, int *matchLen);

int  CVIFUNC RegExpr_FindPatternInText (const char *regExpr, int caseSens,
                                        const char *text, int txtLen, int direction,
                                        int matchPolicy,
                                        int *matched, int *matchPosition,
                                        int *matchLen);

void CVIFUNC RegExpr_Destroy (int handle);

char * CVIFUNC RegExpr_GetErrorString (int error);

char * CVIFUNC RegExpr_GetErrorElaboration (void);

#ifdef __cplusplus
    }
#endif

#endif /* _REGEXPR_HEADER_ */
