/*============================================================================*/
/*                        L a b W i n d o w s / C V I                         */
/*----------------------------------------------------------------------------*/
/*    Copyright (c) National Instruments 1987-1999.  All Rights Reserved.     */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Title:       regexpr.c                                                     */
/* Purpose:     provides functions for using regular expressions to find      */
/*              patterns in text.                                             */
/*                                                                            */
/* Warning:     The functions in this file are NOT multithread-safe.          */
/*                                                                            */
/*============================================================================*/



#include <cvirte.h>
//#include <ansi_c.h>
#if defined(_NI_mswin32_) && _NI_mswin32_
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#elif defined(_NI_linux_) && _NI_linux_
#include <pthread.h>
#endif
#include <utility.h>
#include "toolbox.h"
#include "regexpr.h"


/***************************************************************************/
/**************** regular expression declarations   ************************/
/***************************************************************************/


enum {
   NOT_IN_RANGE,
   RANGE_BEGIN,    /* one char */
   RANGE_CONTINUE  /* '-' */
};

enum {
    /* a negative error code, defined in regexpr.h, is returned in case of an error */
    reNone = 0,
    reSuccess = 1
};

enum tokenType {
    tokEitherOr,      /* | */
    tokZeroOrMore,    /* * */
    tokOneOrMore,     /* + */
    tokZeroOrOne,     /* ? */
    tokAnyChar,       /* . */
    tokBegLine,       /* ^ */
    tokEndLine,       /* $ */
    tokBegGroup,      /* { */
    tokEndGroup,      /* } */
    tokBegSet,        /* [ */
    tokEndSet,        /* ] */
    tokNot,           /* ~ */
    tokRange,
    tokChar
};
typedef enum tokenType RegExprTokenType;

typedef struct charRange CharRange;
struct charRange {
    int lowerLimit;
    int upperLimit;
};
#define CharRangeSize sizeof (struct charRange)

typedef struct token Token;
struct token {
    int type;
    union {
        int ch;
        CharRange range;
    } u;
};
#define TokenSize sizeof (struct token)


enum regExprType {
    reChar,
    reEitherOr,
    reZeroOrOne,
    reZeroOrMore,
    reOneOrMore,
    reGroup,
    reSet,
    reAnyChar,
    reBegLine,
    reEndLine,
    reInvalidType
};
typedef enum regExprType RegExprType;


typedef struct regExprCharDef RegExprCharDef;
struct regExprCharDef {
    int ch1;     /* if caseSens, original value;  else forced to upper case */
    int ch2;     /* if caseSens, original value;  else forced to lower case */
};

typedef struct regExprNode *RegExprNodePtr;

typedef struct regExprEitherOr RegExprEitherOr;
struct regExprEitherOr {
    RegExprNodePtr left;
    RegExprNodePtr right;
};

typedef struct regExprNode RegExprNode;
struct regExprNode {
    RegExprType type;
    RegExprNodePtr next;        /* concatenation */
    unsigned char *firstCharVec;  /* for first node, sets and non-leaf nodes;  do chars manually;  other assumed to match all  */
    int maxNumChars;
    int minNumChars;
    union {
        RegExprCharDef charDef;
        RegExprNodePtr rep;
        RegExprNodePtr group;
        RegExprEitherOr eitherOr;
    } u;
};
#define RegExprNodeSize sizeof (struct regExprNode)

typedef struct RegExprErrorInfo {
    int errorChar;
    int errorNumber;
    char buffer[100];
} RegExprErrorInfo;

static Token *gTokenList;
static int gNumTokens;
static int gCurrToken;
static int gCaseSens;

static int  GetTokens (const char *, int);
static RegExprTokenType TokenTypeFromChar (int);
static int  GetHexVal (const char *, int *);
static void AddToSet (int, int *);
static void AddChar (int);
static int  REG (RegExprNodePtr *);
static int  CXT (RegExprNodePtr *);
static int  CX1 (RegExprNodePtr, RegExprNodePtr *);
static int  OPR (RegExprNodePtr, RegExprNodePtr *);
static int  TRM (RegExprNodePtr *);
static int  GRP (RegExprNodePtr *);
static int  SET (RegExprNodePtr *);
static int  TokenTypeMatch (int);
static RegExprNodePtr AllocRegExprNode (int);
static int  MatchRegExpr (RegExprNodePtr node, const char *text, int txtLen,
                          int col, int stretchCol, int matchPolicy,
                          int *nextCol);
static void RemoveRegExpr (RegExprNodePtr *regExpr);
static int  GenerateFirstCharVecs (RegExprNodePtr node);
static void AnalyzeRegExprFirstChar (RegExprNodePtr node, unsigned char vec[]);
static int  MightMatchCurrTextPos (RegExprNodePtr node, const char *text,
                                      int txtLen, int col);
static void GenerateMaxNumCharCount (RegExprNodePtr node);
static void CalculateMaxNumChars (RegExprNodePtr origNode);
static void GenerateMinNumCharCount (RegExprNodePtr node);
static void CalculateMinNumChars (RegExprNodePtr origNode);

static int  RecordRegExprError (int error, int ch);

static long NumBytesInCharAtLoc (const char *loc);
static long ColOfNextChar (const char *text, long col);
static long ColOfPrevChar (const char *text, long col);

/***************************************************************************/
/********************** multi-thread safety ********************************/
/***************************************************************************/

#define     THREAD_SLEEP_TIME    10L
static int  gThreadLocalHandle  = 0;
static int  gLockHandle         = 0;

static void CVIANSI UninitializeMultithreading(void)
{
    if (!CVIRTEHasBeenDetached())
        {
        if (gLockHandle)
            {
            CmtDiscardLock(gLockHandle);
            gLockHandle = 0;
            }
        if (gThreadLocalHandle)
            {
            CmtDiscardThreadLocalVar(gThreadLocalHandle);
            gThreadLocalHandle = 0;
            }
        }
}

static int InitializeMultithreading(void)
{
#if defined(_NI_linux_) && _NI_linux_
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
#endif
    static int gInitDone = 0;   /* Lock has already been initialized */
    static int sInitNow = 0;    /* Lock is currently being initialized. */
    int error = 0, prev = 0;

    /* If the locks have never been initialized, do so now. */
    if (!gInitDone)
        {
#if defined(_NI_linux_) && _NI_linux_
        if (sInitNow == 1)
            prev = sInitNow;
        else
            {
            pthread_mutex_lock (&mutex);
            prev = sInitNow;
            sInitNow = 1;
            pthread_mutex_unlock (&mutex);
            }
#else
        prev = InterlockedExchange(&sInitNow, 1);
#endif
        if (prev)                                   /* If it is initialized or is in the process of initialing. */
            {
            while (!gInitDone)          /* Wait until initializing the locks finishes   */
#if defined(_NI_linux_) && _NI_linux_
                {
                struct timespec sleepTime;
                sleepTime.tv_sec = 0;
                sleepTime.tv_nsec = THREAD_SLEEP_TIME * 1000000;
                nanosleep (&sleepTime, NULL);
                }
#else
                Sleep(THREAD_SLEEP_TIME);
#endif
            }
        else
            {
            errChk(CmtNewLock(0, 0, &gLockHandle));
            errChk(CmtNewThreadLocalVar(sizeof(RegExprErrorInfo), 0, 0, 0,
                &gThreadLocalHandle));
            atexit(UninitializeMultithreading);
            gInitDone = 1;
            }
        }

Error:
    if (error < 0)
        UninitializeMultithreading();
    return error;
}

static int GetLock(void)
{
    int error = 0;

    errChk(InitializeMultithreading());
    errChk(CmtGetLock(gLockHandle));

Error:
    return error < 0 ? RegExprErr_OutOfMemory : RegExprErr_NoErr;
}

static int ReleaseLock(void)
{
    int error = 0;

    if (gLockHandle)
        {
        errChk(CmtReleaseLock(gLockHandle));
        }
Error:
    return error < 0 ? RegExprErr_OutOfMemory : RegExprErr_NoErr;
}

static RegExprErrorInfo * GetThreadErrorInfo(void)
{
    int error;
    RegExprErrorInfo * errorInfo = 0;

    errChk(InitializeMultithreading());
    errChk(CmtGetThreadLocalVar(gThreadLocalHandle, &errorInfo));

Error:
    return error < 0 ? 0 : errorInfo;
}

/***************************************************************************/
/********************** regular expression functions ***********************/
/***************************************************************************/

/*
 * REG --> CXT REG | e        concatenation of complex terms
 *
 * CXT --> TRM CX1
 * CX1 --> OPR CX1 | e
 *
 * OPR -->  * | + | ? | '|' TRM
 *
 * TRM -->  GRP | SET | char | . | ^ | $
 *
 * GRP --> { CXT GR1 }
 * GR1 --> CXT GR1 | e     collapsed into GRP()
 * SET --> [ SE1 SE2 ]                             * multibyte chars NOT allowed in SET *
 * SE1 --> ~ | e           collapsed into SET()
 * SE2 --> range SE3       collapsed into SET()
 * SE3 --> range | e       collapsed into SET()
*/

int CVIFUNC RegExpr_Parse (const char *text, int caseSensitive, int *handle)
{
    RegExprNodePtr regExpr = NULL;
    int len;
    int error = UIENoError, locked = 0;
    RegExprErrorInfo * errorInfo = 0;

    errChk (GetLock ());
    locked = 1;
    nullChk (errorInfo = GetThreadErrorInfo ());
    errorInfo->errorNumber = 0;
    gTokenList = NULL;
    gCurrToken = 0;
    gCaseSens = caseSensitive;

    if (text == NULL)
        errChk (RecordRegExprError (RegExprErr_NullInputString, 0));
    len = strlen (text);
    if (len == 0)
        errChk (RecordRegExprError (RegExprErr_EmptyInputString, 0));

    nullChk ((gTokenList = calloc (len+1, TokenSize)));
    errChk (GetTokens (text, len));

    errChk (REG(&regExpr));
    errChk (GenerateFirstCharVecs(regExpr));

    if (regExpr->firstCharVec == NULL)   /* make sure first node has a char vec */
        {
        nullChk (regExpr->firstCharVec = calloc (256, 1));
        AnalyzeRegExprFirstChar (regExpr, regExpr->firstCharVec);
        }

    GenerateMaxNumCharCount (regExpr);
    GenerateMinNumCharCount (regExpr);

Error:
    free (gTokenList);
    if (error < 0)
        RemoveRegExpr (&regExpr);     /* sets regExpr to NULL */
    else
        *handle = (int)regExpr;
    if (locked)
        ReleaseLock ();
    return error < 0 ? error : 0;
}

static int GetTokens (const char *buf, int bufLen)
{
    int error = UIENoError;
    int rangeStat, groupDepth=0;
    int hexVal;
    int ch;
    char *pos, *posOfBegSet, *limitPos;
    int inSet=FALSE;

    gNumTokens = 0;
    limitPos = (char*)buf + bufLen;
    for (pos=(char*)buf; pos < limitPos; CmbStrInc(pos))
        {
        switch (ch = CmbGetC(pos))
            {
            case '|':
            case '*':
            case '+':
            case '?':
            case '.':
            case '^':
            case '$':
                if (inSet)
                    AddToSet (ch, &rangeStat);
                else
                    gTokenList[gNumTokens++].type = TokenTypeFromChar (ch);
                break;
            case '{':
                if (inSet)
                    AddToSet (ch, &rangeStat);
                else
                    {
                    gTokenList[gNumTokens++].type = tokBegGroup;
                    groupDepth++;
                    }
                break;
            case '}':
                if (inSet)
                    AddToSet (ch, &rangeStat);
                else if (groupDepth > 0)
                    {
                    gTokenList[gNumTokens++].type = tokEndGroup;
                    groupDepth--;
                    }
                else
                    errChk (RecordRegExprError (RegExprErr_UnmatchedChar, ch));
                break;
            case '[':
                if (inSet)
                    errChk (RecordRegExprError (RegExprErr_InvalidCharInRange, ch));
                else
                    {
                    gTokenList[gNumTokens++].type = tokBegSet;
                    inSet = TRUE;
                    posOfBegSet = pos;
                    rangeStat = NOT_IN_RANGE;
                    }
                break;
            case ']':   /* must be preceded by '\' to be included in range */
                if (inSet)
                    if (rangeStat == RANGE_CONTINUE)
                        errChk (RecordRegExprError (RegExprErr_InvalidCharInRange, ch));
                    else
                        {
                        gTokenList[gNumTokens++].type = tokEndSet;
                        inSet = FALSE;
                        }
                else
                    errChk (RecordRegExprError (RegExprErr_UnmatchedChar, ch));
                break;
            case '~':
                if (inSet)
                    if (pos == posOfBegSet + 1)
                        gTokenList[gNumTokens++].type = tokNot;
                    else
                        AddToSet (ch, &rangeStat);
                else
                    AddChar (ch);
                break;
            case '-':
                if (inSet)
                    if (rangeStat == RANGE_BEGIN)
                        rangeStat = RANGE_CONTINUE;
                    else
                        errChk (RecordRegExprError (RegExprErr_InvalidCharInRange, ch));
                else
                    AddChar (ch);
                break;
            case '\\':
                if (++pos >= limitPos)
                    errChk (RecordRegExprError (RegExprErr_EndWithBackslash, 0));
                ch = CmbGetC(pos);
                if ((ch == 't') || (ch == 'T'))
                    ch = '\t';
                else if ((ch == 'x') || (ch == 'X'))
                    if ((pos+2 < buf+bufLen) && GetHexVal (pos+1, &hexVal))
                        {
                        ch = hexVal;
                        pos += 2;
                        }
                    else
                        errChk (RecordRegExprError (RegExprErr_InvalidHexValue, 0));
                /* FALL THRU !!! */
                /* FALL THRU !!! */
                /* FALL THRU !!! */
            default:
                if (inSet)
                    AddToSet (ch, &rangeStat);
                else
                    AddChar (ch);
                break;
            }
        } /* end for-loop */

    if (inSet)
        errChk (RecordRegExprError (RegExprErr_UnmatchedChar, '['));
    if (groupDepth > 0)
        errChk (RecordRegExprError (RegExprErr_UnmatchedChar, '{'));

Error:
    return error;
}

static RegExprTokenType TokenTypeFromChar (int ch)
{
    switch (ch)
        {
        case '|':
            return tokEitherOr;
        case '*':
            return tokZeroOrMore;
        case '+':
            return tokOneOrMore;
        case '?':
            return tokZeroOrOne;
        case '.':
            return tokAnyChar;
        case '^':
            return tokBegLine;
        case '$':
            return tokEndLine;
        default:
            return -1;
        }
}

static int GetHexVal (const char *buf, int *hexVal)   /* requires exactly two hex chars -- don't bother with multibyte chars here -- too obscure */
{
    int ch[2];
    int i, c;

    for (i=0; i < 2; i++)
        {
        c = toupper(buf[i]);
        if ((c >= '0') && (c <= '9'))
            ch[i] = c - '0';
        else if ((c >= 'A') && (c <= 'F'))
            ch[i] = c - 'A' + 10;
        else
            return FALSE;
        }

    *hexVal = (ch[0] << 4) + ch[1];
    return TRUE;
}

static void AddToSet (int ch, int *rangeStat)
{
    Token *tok = &gTokenList[gNumTokens];    /* point to next available token slot */

    switch (*rangeStat)
        {
        case NOT_IN_RANGE:
        case RANGE_BEGIN:    /* prev range was single char only; start new range */
            tok->type = tokRange;
            tok->u.range.lowerLimit = ch;
            tok->u.range.upperLimit = ch;
            *rangeStat = RANGE_BEGIN;
            gNumTokens++;
            break;
        case RANGE_CONTINUE:
            --tok;        /* *n was already incremented for beginning of range */
            tok->u.range.upperLimit = ch;
            *rangeStat = NOT_IN_RANGE;
            break;
        }
}

static void AddChar (int ch)
{
    Token *tok = &gTokenList[gNumTokens];    /* point to next available token slot */

    tok->type = tokChar;
    tok->u.ch = ch;
    gNumTokens++;
}

static int REG (RegExprNodePtr *regExpr)    /* *regExpr must be NULL on entry */
{
    RegExprNodePtr *nextNodeAddr = regExpr;
    RegExprNodePtr newNode;
    int status;

    while ((status = CXT (&newNode)) == reSuccess)
        {
        *nextNodeAddr = newNode;
        nextNodeAddr = &newNode->next;
        }

    if (status < 0)
        return status;
    else
        return reSuccess;
}

static int CXT (RegExprNodePtr *theNode)
{
    RegExprNodePtr leftNode, parentNode;
    int status;
    int error = UIENoError;

    errChk (TRM (&leftNode));       /* if reNone, let CX1 return reNone */
                                    /* or report a specific error       */

    errChk (CX1 (leftNode, &parentNode));
                                    /* CX1 deletes all nodes if fails */

    if (parentNode == NULL)         /* leftNode was NULL, and */
        status = reNone;            /* there was no operator  */
    else
        {
        *theNode = parentNode;
        status = reSuccess;
        }

Error:
    if (error < 0)
        return error;
    else
        return status;
}

static int CX1 (RegExprNodePtr leftNode, RegExprNodePtr *parentNode)
{
    RegExprNodePtr newNode;
    int status;

    while ((status = OPR (leftNode, &newNode)) == reSuccess)
        leftNode = newNode;

    if (status < 0)
        {
        RemoveRegExpr (&leftNode);   /* delete the entire list */
        return status;
        }
    else
        {
        *parentNode = leftNode;            /* status == reNone */
        return reSuccess;
        }
}

static int OPR (RegExprNodePtr leftNode, RegExprNodePtr *parentNode)
{
    RegExprNodePtr node=NULL;
    int status=reSuccess;
    int error = UIENoError;

    if (gCurrToken >= gNumTokens)
        {
        status = reNone;
        goto Error;
        }

    switch (gTokenList[gCurrToken].type)
        {
        case tokZeroOrMore:
            if (leftNode == NULL)
                errChk (RecordRegExprError (RegExprErr_OperatorOnEmptyPattern, '*'));
            gCurrToken++;
            nullChk (node = AllocRegExprNode (reZeroOrMore));
            node->u.rep = leftNode;
            break;
        case tokOneOrMore:
            if (leftNode == NULL)
                errChk (RecordRegExprError (RegExprErr_OperatorOnEmptyPattern, '+'));
            gCurrToken++;
            nullChk (node = AllocRegExprNode (reOneOrMore));
            node->u.rep = leftNode;
            break;
        case tokZeroOrOne:
            if (leftNode == NULL)
                errChk (RecordRegExprError (RegExprErr_OperatorOnEmptyPattern, '?'));
            gCurrToken++;
            nullChk (node = AllocRegExprNode (reZeroOrOne));
            node->u.rep = leftNode;
            break;
        case tokEitherOr:
            if (leftNode == NULL)
                errChk (RecordRegExprError (RegExprErr_EmptyLeftSide, 0));
            gCurrToken++;
            nullChk (node = AllocRegExprNode (reEitherOr));

            errChk (status = TRM (&node->u.eitherOr.right));
            if (status == reNone)
                errChk (RecordRegExprError (RegExprErr_EmptyRightSide, 0));

            node->u.eitherOr.left = leftNode;
            break;
        default:
            status = reNone;
            break;
        }

Error:
    if (error < 0)
        {
        free (node);
        return error;
        }
    else
        {
        *parentNode = node;
        return status;
        }
}

static int TRM (RegExprNodePtr *theNode)
{
    RegExprNodePtr node=NULL;
    int ch;
    int status=reSuccess;
    int error = UIENoError;

    if (gCurrToken >= gNumTokens)
        {
        status = reNone;
        goto Error;
        }

    switch (gTokenList[gCurrToken].type)
        {
        case tokBegGroup:
            gCurrToken++;
            errChk (GRP(&node));
            break;
        case tokBegSet:
            gCurrToken++;
            errChk (SET (&node));
            break;
        case tokChar:
            nullChk (node = AllocRegExprNode (reChar));
            ch = gTokenList[gCurrToken].u.ch;
            node->u.charDef.ch1 = (gCaseSens || !CmbIsSingleC(ch)) ? ch : toupper(ch);
            node->u.charDef.ch2 = (gCaseSens || !CmbIsSingleC(ch)) ? ch : tolower(ch);
            gCurrToken++;
            break;
        case tokAnyChar:
            gCurrToken++;
            nullChk (node = AllocRegExprNode (reAnyChar));
            break;
        case tokBegLine:
            gCurrToken++;
            nullChk (node = AllocRegExprNode (reBegLine));
            break;
        case tokEndLine:
            gCurrToken++;
            nullChk (node = AllocRegExprNode (reEndLine));
            break;
        default:
            status = reNone;
            break;
        }

Error:
    if (error < 0)
        {
        free (node);
        return error;
        }
    else
        {
        *theNode = node;
        return status;
        }
}

static int GRP (RegExprNodePtr *theNode)
{
    RegExprNodePtr groupNode=NULL, newNode, *nextNodeAddr;
    int count=0;
    int status=reSuccess;
    int error = UIENoError;

    nullChk (groupNode = AllocRegExprNode (reGroup));

    nextNodeAddr = &groupNode->u.group;
    while ((status = CXT (&newNode)) == reSuccess)
        {
        *nextNodeAddr = newNode;
        nextNodeAddr = &newNode->next;
        count++;
        }
    errChk (status);
    if (count == 0)
        errChk (RecordRegExprError (RegExprErr_EmptyGroup, 0));

    TokenTypeMatch (tokEndGroup);
    gCurrToken++;

Error:
    if (error < 0)
        {
        RemoveRegExpr (&groupNode);
        return error;
        }
    else
        {
        *theNode = groupNode;
        return reSuccess;
        }
}

static int SET (RegExprNodePtr *theNode)              /* multibyte chars not allowed in SETs */
{
    RegExprNodePtr node=NULL;
    unsigned char *vec = NULL;
    CharRange range;
    int not, count=0, i,j;
    int error = UIENoError;

    nullChk (vec = calloc (256, 1));

    not = TokenTypeMatch (tokNot);
    if (not)
        gCurrToken++;

    while (TokenTypeMatch (tokRange))
        {
        count++;
        range = gTokenList[gCurrToken++].u.range;
        if (range.upperLimit > 0xFF)
            errChk (RecordRegExprError (RegExprErr_MultibyteCharsNotAllowedInRange, 0));
        if (range.lowerLimit > range.upperLimit)
            errChk (RecordRegExprError (RegExprErr_InvalidRange, 0));
        for (i=range.lowerLimit; i <= range.upperLimit; i++)
            vec[i] = TRUE;
        }
    if (count == 0)
        errChk (RecordRegExprError (RegExprErr_EmptySet, 0));

    TokenTypeMatch (tokEndSet) ;
    gCurrToken++;

    if (!gCaseSens)
        for (i='A',j='a'; i <= 'Z'; i++,j++)
            if (vec[i] || vec[j])
                vec[i] = vec[j] = TRUE;

    if (not)
        for (i=0; i < 256; i++)
            vec[i] = !vec[i];

    nullChk (node = AllocRegExprNode (reSet));
    node->firstCharVec = vec;

Error:
    if (error < 0)
        {
        free (vec);
        free (node);
        return error;
        }
    else
        {
        *theNode = node;
        return reSuccess;
        }
}

static int TokenTypeMatch (int type)
{
    if (gCurrToken >= gNumTokens)
        return FALSE;
    else
        return (gTokenList[gCurrToken].type == type);
}

static RegExprNodePtr AllocRegExprNode (int type)
{
    RegExprNodePtr p;

    p = calloc (1, RegExprNodeSize);
    if (p)
        p->type = type;
    return p;
}

static int GenerateFirstCharVecs (RegExprNodePtr node)
{
    int allocVec;
    int error = UIENoError;

    for ( ; node != NULL; node=node->next)
        {
        switch (node->type)
            {
            case reEitherOr:
                errChk (GenerateFirstCharVecs (node->u.eitherOr.left));
                errChk (GenerateFirstCharVecs (node->u.eitherOr.right));
                allocVec = TRUE;
                break;
            case reZeroOrOne:
            case reZeroOrMore:
            case reOneOrMore:
            case reGroup:   /* u.rep is same as u.group */
                errChk (GenerateFirstCharVecs (node->u.rep))
                allocVec = TRUE;
                break;
            default:
                allocVec = FALSE;
                break;
            }

        if (allocVec)
            {
            nullChk (node->firstCharVec = calloc(256, 1));
            AnalyzeRegExprFirstChar (node, node->firstCharVec);
            }
        }

Error:
    return error;
}

static void AnalyzeRegExprFirstChar (   /* analyze beginning of reg expr */
RegExprNodePtr node,                    /* so that we can know what the  */
unsigned char vec[])                            /* first char must be            */
{
    int i, type;

    type = (node == NULL) ? reInvalidType : node->type;
    switch (type) {
        case reChar:
            vec[CmbFirstByteOfChar(node->u.charDef.ch1)] = TRUE;
            vec[CmbFirstByteOfChar(node->u.charDef.ch2)] = TRUE;
            break;
        case reEitherOr:
            AnalyzeRegExprFirstChar (node->u.eitherOr.left, vec);
            AnalyzeRegExprFirstChar (node->u.eitherOr.right, vec);
            break;
        case reZeroOrOne:
        case reZeroOrMore:
            AnalyzeRegExprFirstChar (node->u.rep, vec);
            AnalyzeRegExprFirstChar (node->next, vec);
            break;
        case reOneOrMore:
        case reGroup:   /* u.rep is same as u.group */
            AnalyzeRegExprFirstChar (node->u.rep, vec);
            break;
        case reSet:
            for (i=0; i < 256; i++)
                if (node->firstCharVec[i])
                    vec[i] = TRUE;
            break;
        case reBegLine:
            AnalyzeRegExprFirstChar (node->next, vec);
            break;
        case reAnyChar:
        case reEndLine:
        case reInvalidType:
            memset (vec, TRUE, 256);
            break;
    }
}

void CVIFUNC RegExpr_Destroy (int handle)
{
    int error, tmpHandle = handle, locked = 0;

    errChk (GetLock ());
    locked = 1;
    RemoveRegExpr ((RegExprNodePtr *)&tmpHandle);

Error:
    if (locked)
        ReleaseLock();
}

static void RemoveRegExpr (RegExprNodePtr *regExpr)
{
    RegExprNodePtr node, nextNode;

    for (node=*regExpr; node; node=nextNode) {
        nextNode = node->next;
        switch (node->type) {
            case reEitherOr:
                RemoveRegExpr (&node->u.eitherOr.left);
                RemoveRegExpr (&node->u.eitherOr.right);
                break;
            case reZeroOrOne:
            case reZeroOrMore:
            case reOneOrMore:
            case reGroup:     /* u.rep is same as u.group */
                RemoveRegExpr (&node->u.rep);
                break;
        }
        free (node->firstCharVec);
        free (node);
    }
    *regExpr = NULL;
}

static int MatchText (int handle, const char *text, int txtLen,
    int firstCol, int stretchCol, int matchPolicy, int *matchLen)
{
    RegExprNodePtr node = (RegExprNodePtr)handle;
    int retval;

    if (txtLen < 0)
        txtLen = strlen(text);

    if ((txtLen > 0) && !node->firstCharVec[(unsigned char)text[firstCol]])
        retval = RegExpr_NoMatch;
    else
        retval = MatchRegExpr (node, text, txtLen, firstCol, stretchCol,
            matchPolicy, matchLen);

    if (retval != RegExpr_Match)
        *matchLen = -1;
    return retval;
}

int CVIFUNC RegExpr_MatchText (int handle, const char *text, int txtLen,
                       int matchPolicy, int *matchLen)
{
    RegExprNodePtr node = (RegExprNodePtr)handle;
    int retval = RegExpr_NoMatch, error, locked = 0;

    errChk (GetLock ());
    locked = 1;

    if (txtLen < 0)
        txtLen = strlen(text);

    if ((txtLen > 0) && !node->firstCharVec[(unsigned char)text[0]])
        retval = RegExpr_NoMatch;
    else
        retval = MatchRegExpr (node, text, txtLen, 0, 0, matchPolicy,
                               matchLen);

    if (retval != RegExpr_Match)
        *matchLen = -1;

Error:
    if (locked)
        ReleaseLock();
    return retval;
}

static int MatchRegExpr (
RegExprNodePtr node,
const char *text,
int txtLen,
int col,        /* col to start at */
int stretchCol, /* want to match cols at least up thru stretchCol-1 */
int matchPolicy,
int *nextCol)   /* if RegExpr_Match, return the next col;  might be less than stretchCol */
{
    int minReps, maxReps;
    int tryCol, begNextCol, repCol, foundNext;
    int stepCol, leftStepCol, rightStepCol;
    int leftPossible, rightPossible;
    int leftNotOnLine, rightNotOnLine;
    int newStretchCol;
    int status;
    int ch;
    int nextColToRet = -1;

    for ( ; node != NULL; node=node->next) {
        switch (node->type) {
            case reChar:
                if (col >= txtLen)
                    return RegExpr_NoMatch;
                ch = CmbGetC (&text[col]);
                col += CmbGetNumBytesInChar (ch);
                if ((ch != node->u.charDef.ch1) && (ch != node->u.charDef.ch2))
                    return RegExpr_NoMatch;
                break;
            case reEitherOr:
                /*************************************************************/
                /* Find first occurrence of "node->next", and set begNextCol */
                /* to its location.  Then try to find the left or right alt  */
                /* ending precisely at begNextCol.  If so, we have a match.  */
                /* If at least one occurrence was found after begNextCol,    */
                /* try to find a later occurrence of "node->next".           */
                /* Otherwise, return RegExpr_NoMatch.                              */
                /*************************************************************/
                if (!MightMatchCurrTextPos (node, text, txtLen, col))
                    return RegExpr_NoMatch;
                leftPossible = rightPossible = TRUE;
                leftNotOnLine = rightNotOnLine = FALSE;
                tryCol = col;                  /* Try to find "node->next" starting at tryCol. */
                while (tryCol <= txtLen) {     /* Allow tryCol to be txtLen, so can match '$'. */
                    begNextCol = tryCol;
                    if (node->next == NULL) {
                        foundNext = TRUE;
                        if (begNextCol < stretchCol)
                            begNextCol = stretchCol;
                        *nextCol = begNextCol;
                    }
                    else {
                        foundNext = FALSE;
                        while (begNextCol <= txtLen) {
                            if (MightMatchCurrTextPos (node->next, text, txtLen, begNextCol)) {
                                status = MatchRegExpr (node->next, text, txtLen,
                                                       begNextCol, stretchCol,
                                                       matchPolicy, nextCol);
                                if (status == RegExpr_NotInText)
                                    break;
                                if ((status == RegExpr_Match) && (*nextCol >= stretchCol)) {
                                    foundNext = TRUE;
                                    break;
                                }
                            }
                            begNextCol = ColOfNextChar (text, begNextCol);
                        }
                    }
                    if (!foundNext)
                        return (tryCol == col) ? RegExpr_NotInText : RegExpr_NoMatch;
                    tryCol = ColOfNextChar (text, begNextCol);   /* may be needed later */
                    if (leftPossible)
                        switch (MatchRegExpr (node->u.eitherOr.left, text,
                                              txtLen, col, begNextCol,
                                              matchPolicy, &leftStepCol)) {
                            case RegExpr_Match:
                                if (leftStepCol == begNextCol)
                                    return RegExpr_Match;
                                if (leftStepCol < begNextCol)
                                    leftPossible = FALSE;
                                break;
                            case RegExpr_NoMatch:
                                leftPossible = FALSE;
                                break;
                            case RegExpr_NotInText:
                                leftNotOnLine = TRUE;
                                leftPossible = FALSE;
                                break;
                        }
                    if (rightPossible)
                        switch (MatchRegExpr (node->u.eitherOr.right, text,
                                              txtLen, col, begNextCol,
                                              matchPolicy, &rightStepCol)) {
                            case RegExpr_Match:
                                if (rightStepCol == begNextCol)
                                    return RegExpr_Match;
                                if (rightStepCol < begNextCol)
                                    rightPossible = FALSE;
                                break;
                            case RegExpr_NoMatch:
                                rightPossible = FALSE;
                                break;
                            case RegExpr_NotInText:
                                rightNotOnLine = TRUE;
                                rightPossible = FALSE;
                                break;
                        }
                    if (leftNotOnLine && rightNotOnLine)
                        return RegExpr_NotInText;
                    if (!leftPossible && !rightPossible)
                        return RegExpr_NoMatch;
                }
                return RegExpr_NoMatch;
            case reZeroOrOne:
                minReps = 0;
                maxReps = 1;
                goto repStart;
            case reZeroOrMore:
                minReps = 0;
                maxReps = 0x7fff;
                goto repStart;
            case reOneOrMore:
                minReps = 1;
                maxReps = 0x7fff;
                goto repStart;
            case reGroup:       /***************************************/
                minReps = 1;    /* NOTE:  node->u.group is the same as */
                maxReps = 1;    /*        node->u.rep, since they are  */
                goto repStart;  /*        in the same union.           */
                /*************************************************************/
                /* Find first occurrence of "node->next", and set begNextCol */
                /* to its location.  Then try to find successive occurrences */
                /* of "node->u.rep" ending precisely at begNextCol.  If so,  */
                /* we have a match.  If the occurrences were found but end   */
                /* after begNextCol, try to find a later occurrence of       */
                /* "node->next".  If the occurrences of "node->u.rep" did    */
                /* not make it all the way to begNextCol, return RegExpr_NoMatch.  */
                /*************************************************************/
repStart:
                if (!MightMatchCurrTextPos (node, text, txtLen, col))
                    return RegExpr_NoMatch;
                tryCol = col + node->minNumChars; /* Try to find "node->next" starting at tryCol. */
                while (tryCol <= txtLen) {        /* Allow tryCol to be txtLen, so can match '$'. */
                    begNextCol = tryCol;
                    if (node->next == NULL) {
                        foundNext = TRUE;
                        if (begNextCol < stretchCol)
                            begNextCol = stretchCol;
                        *nextCol = begNextCol;
                    }
                    else {
                        foundNext = FALSE;
                        while (begNextCol <= txtLen) {
                            if (MightMatchCurrTextPos (node->next, text, txtLen, begNextCol)) {
                                status = MatchRegExpr (node->next, text, txtLen, begNextCol,
                                                       stretchCol, matchPolicy, nextCol);
                                if (status == RegExpr_NotInText)
                                    break;
                                if ((status == RegExpr_Match) && (*nextCol >= stretchCol)) {
                                    foundNext = TRUE;
                                    break;
                                }
                            }
                            begNextCol = ColOfNextChar (text, begNextCol);
                        }
                        if (!foundNext)
                            if (tryCol == col)
                                return RegExpr_NotInText;
                            else
                                goto repEnd;
                    }

                    tryCol = ColOfNextChar (text, begNextCol);   /* may be needed later */
                    /*******************************************************/
                    /* Match "node->u.rep" occurrences until cannot match  */
                    /* or until at or beyond where "node->next" was found. */
                    /* (Take care of case when 0 reps are allowed and we   */
                    /* have no chars between col and begNextCol).          */
                    /*******************************************************/
                    if ((minReps == 0) && (begNextCol == col)) {
                        nextColToRet = *nextCol;    /* keep record of this and loop around */
                        if (matchPolicy == RegExpr_MatchSmallestNumChars)
                            goto repEnd;
                    }
                    else if (maxReps == 1) {
                        status = MatchRegExpr (node->u.rep, text, txtLen,
                                               col, begNextCol,
                                               matchPolicy, &stepCol);
                        switch (status) {
                            case RegExpr_Match:
                                if (stepCol == begNextCol) {   /* exact match */
                                    nextColToRet = *nextCol;
                                    if (matchPolicy == RegExpr_MatchSmallestNumChars)
                                        goto repEnd;
                                }
                                else if (stepCol < begNextCol)  /* no chance since only 1 rep */
                                    goto repEnd;
                                break;       /* was beyond begNextCol;  try again with later occurrence of node->next */
                            case RegExpr_NoMatch:  /* cannot be found starting at col and */
                                goto repEnd;       /* stretching to begNextCol or beyond  */
                            case RegExpr_NotInText:
                                if (minReps == 0)
                                    goto repEnd;
                                else
                                    return RegExpr_NotInText;
                        }
                    }
                    else {
                        for (repCol=col; repCol <= begNextCol; repCol=stepCol) {
                            /**************************************************************/
                            /* Try to find the largest possible match up to begNextCol.   */
                            /* Allow for matches that go past begNextCol;  when that      */
                            /* happens, we want to drop out of this loop and try          */
                            /* to find a later occurrence of node->next.                  */
                            /**************************************************************/
                            if (node->maxNumChars >= 0)      /* there is a max */
                                newStretchCol = Min (begNextCol, repCol+node->maxNumChars);
                            else
                                newStretchCol = begNextCol;
                            status = RegExpr_NoMatch;
                            while (newStretchCol >= repCol) {
                                status = MatchRegExpr (node->u.rep, text, txtLen,
                                                       repCol, newStretchCol,
                                                       matchPolicy, &stepCol);
                                if (status == RegExpr_Match)
                                    break;
                                if (status == RegExpr_NotInText)
                                    break;
                                newStretchCol = ColOfPrevChar (text, newStretchCol);
                            }
                            switch (status) {
                                case RegExpr_Match:
                                    if (stepCol == begNextCol) {  /* exact match */
                                        nextColToRet = *nextCol;
                                        if (matchPolicy == RegExpr_MatchSmallestNumChars || stepCol == repCol)
                                            goto repEnd;
                                    }
                                    else if (stepCol == repCol)   /* no progress */
                                        goto repEnd;
                                    break;   /* ok; go to next repCol; if stepCol>begNextCol, will drop out of loop and try to find later occurrence of node->next */
                                case RegExpr_NoMatch:
                                    goto repEnd;
                                case RegExpr_NotInText:
                                    if (minReps == 0)
                                        goto repEnd;
                                    else
                                        return RegExpr_NotInText;
                            }
                        }
                    }
                }
repEnd:
                if (nextColToRet >= 0) {
                    *nextCol = nextColToRet;
                    return RegExpr_Match;
                }
                return RegExpr_NoMatch;
            case reSet:
                /*************************************************************************************************/
                /* NOTE: Although multibyte chars are not allowed in sets (i.e., ranges) in regular expressions, */
                /*       a multibyte character can match a set if the 'not' (~) operator is used in the set.     */
                /*       Given that, just looking at the first byte is sufficient.                               */
                /*************************************************************************************************/
                if (col >= txtLen)
                    return RegExpr_NoMatch;
                if (!node->firstCharVec[(unsigned char)text[col]])
                    return RegExpr_NoMatch;
                col = ColOfNextChar (text, col);
                break;
            case reAnyChar:
                if (col >= txtLen)
                    return RegExpr_NoMatch;
                col = ColOfNextChar (text, col);;
                break;
            case reBegLine:
                if (col > 0)
                    return RegExpr_NoMatch;
                break;
            case reEndLine:
                if (col < txtLen)
                    return RegExpr_NoMatch;
                break;
        }
    }
    *nextCol = col;
    return RegExpr_Match;
}

static int MightMatchCurrTextPos (
RegExprNodePtr node,
const char *text,
int txtLen,
int col)
{
    int ch, firstByte;

    if (node == NULL)
        return TRUE;
    else {
        if (node->type == reChar) {
            ch = (col < txtLen) ? CmbGetC(&text[col]) : 0;
            return (ch == node->u.charDef.ch1) || (ch == node->u.charDef.ch2);
        }
        else if (node->firstCharVec) {
            firstByte = (col < txtLen) ? (unsigned char)text[col] : 0;
            return node->firstCharVec[firstByte];
        }
        else
            return TRUE;
    }
}

static void GenerateMaxNumCharCount (RegExprNodePtr node)
{
    for (; node; node=node->next) {
        switch (node->type) {
            case reEitherOr:
                GenerateMaxNumCharCount (node->u.eitherOr.left);
                GenerateMaxNumCharCount (node->u.eitherOr.right);
                break;
            case reZeroOrOne:
            case reZeroOrMore:
            case reOneOrMore:
            case reGroup:    /* u.rep is same as u.group */
                GenerateMaxNumCharCount (node->u.rep);
                break;
        }
        CalculateMaxNumChars (node);
    }
}

static void CalculateMaxNumChars (RegExprNodePtr node)
{
    node->maxNumChars = 0;    /* just to be sure */

    switch (node->type) {
        case reChar:
            node->maxNumChars = CmbGetNumBytesInChar(node->u.charDef.ch1);
            break;
        case reAnyChar:
            node->maxNumChars = OnMBSystem() ? 2 : 1;
            break;
        case reSet:
            node->maxNumChars = 1;  /* multibyte chars not allowed in sets */
            break;
        case reEitherOr:
            if ((node->u.eitherOr.left->maxNumChars < 0)
                        || (node->u.eitherOr.right->maxNumChars < 0))
                node->maxNumChars = -1;
            else
                node->maxNumChars = Max (node->u.eitherOr.left->maxNumChars,
                                         node->u.eitherOr.right->maxNumChars);
            break;
        case reOneOrMore:
        case reZeroOrMore:
            node->maxNumChars = -1;
            break;
        case reZeroOrOne:
            if (node->u.rep->maxNumChars < 0)
                node->maxNumChars = -1;
            else
                node->maxNumChars = node->u.rep->maxNumChars;
            break;
        case reGroup:    /* u.rep is same as u.group */
            {
                RegExprNodePtr p;
                for (p=node->u.group; p; p=p->next) {
                    if (p->maxNumChars < 0) {
                        node->maxNumChars = -1;
                        break;
                    }
                    else
                        node->maxNumChars += p->maxNumChars;
                }
            }
            break;
    }
}

static void GenerateMinNumCharCount (RegExprNodePtr node)
{
    for (; node; node=node->next) {
        switch (node->type) {
            case reEitherOr:
                GenerateMinNumCharCount (node->u.eitherOr.left);
                GenerateMinNumCharCount (node->u.eitherOr.right);
                break;
            case reZeroOrOne:
            case reZeroOrMore:
            case reOneOrMore:
            case reGroup:    /* u.rep is same as u.group */
                GenerateMinNumCharCount (node->u.rep);
                break;
        }
        CalculateMinNumChars (node);
    }
}

static void CalculateMinNumChars (RegExprNodePtr node)
{
    node->minNumChars = 0;    /* just to be sure */

    switch (node->type)
        {
        case reChar:
            node->minNumChars = CmbGetNumBytesInChar(node->u.charDef.ch1);
            break;
        case reAnyChar:
            node->minNumChars = 1;
            break;
        case reSet:
            node->minNumChars = 1;
            break;
        case reEitherOr:
            node->minNumChars = Min (node->u.eitherOr.left->minNumChars,
                                     node->u.eitherOr.right->minNumChars);
            break;
        case reZeroOrOne:
        case reZeroOrMore:
            node->minNumChars = 0;
            break;
        case reOneOrMore:
            node->minNumChars = node->u.rep->minNumChars;
            break;
        case reGroup:    /* u.rep is same as u.group */
            {
            RegExprNodePtr p;
            for (p=node->u.group; p; p=p->next)
                node->minNumChars += p->minNumChars;
            }
            break;
        }
}

static int RecordRegExprError (int error, int ch)
{
    RegExprErrorInfo * errorInfo = GetThreadErrorInfo ();

    assert (errorInfo);
    if (errorInfo)
        {
        errorInfo->errorNumber = error;
        errorInfo->errorChar = ch;
        }
    return error;
}

typedef struct
{
    char *beginLine;
    char *endLine;
}
tLineDesc;

static int GetLines (const char *text, int textLen, tLineDesc **lines, int *numLines)
{
    int     error = UIENoError;
    int     nLines = 0;
    char    *pc = 0, *beginNextLine = 0;
    tLineDesc *pLine;

    *lines = NULL;
    *numLines = 0;

    pc = (char*)text;
    while (pc - text < textLen)
    {
        unsigned int currentChar = *pc;

        pc = _mbsinc(pc);

        if(currentChar == '\r' || currentChar == '\n')
        {
            nLines++;
            if(pc - text < textLen)
            {
                unsigned int nextChar = *pc;
                // look for "\r\n", "\n\r", "\r", or "\n"
                if(nextChar == '\r' || nextChar == '\n')
                {
                    if(nextChar != currentChar)
                        // skip this character too (i.e. "\r\n" or "\n\r")
                        pc = _mbsinc (pc);
                }
            }
        }
    }
    // Always one additional line than indicated by \r or \n
    // because last line never ends in \r or \n.
    nLines++;

    nullChk (*lines = (tLineDesc *) malloc (sizeof (tLineDesc) * nLines));

    pc = (char*)text;
    beginNextLine = pc;
    while (pc - text < textLen)
    {
        unsigned int currentChar = *pc;

        pc = _mbsinc(pc);

        if(currentChar == '\r' || currentChar == '\n')
        {
            pLine = (*lines)+(*numLines);
            (*numLines)++;

            pLine->beginLine = beginNextLine;
            pLine->endLine = pc - 1; // account for fact that we incremented already

            if(pc - text < textLen)
            {
                unsigned int nextChar = *pc;
                // look for "\r\n", "\n\r", "\r", or "\n"
                if(nextChar == '\r' || nextChar == '\n')
                {
                    if(nextChar != currentChar)
                        // skip this character too (i.e. "\r\n" or "\n\r")
                        pc = _mbsinc (pc);
                }
            }

            // save beginning of next line
            beginNextLine = pc;
        }
    }
    // Always one additional line than indicated by \r or \n
    // because last line never ends in \r or \n.
    pLine = (*lines)+(*numLines);
    (*numLines)++;

    pLine->beginLine = beginNextLine;
    pLine->endLine = pc;

Error:

    if (error < 0)
    {
        if(*lines)
            free(*lines);
        *lines = NULL;
        *numLines = 0;
    }

    return error;
}

int CVIFUNC RegExpr_FindPatternInText (const char *regExpr, int caseSens,
    const char *text, int txtLen, int direction, int matchPolicy,
    int *matched, int *matchCol, int *matchLen)
{
    int handle, incr, startCol, currCol, colsLeft;
    int tempLen = 0, lineLen, nextCol = 0;
    int handleIsValid=FALSE;
    int error = UIENoError;
    char *pcLineBeg = NULL, *pcLineEnd = NULL;
    tLineDesc *lines = NULL;
    int numLines, currLine, linesLeft = 0;
    int numBytesInChar;

    /* Not locking this function as it does not use any global data */

    *matched = 0;

    if (txtLen < 0)
        txtLen = strlen(text);

    errChk (RegExpr_Parse (regExpr, caseSens, &handle));
    handleIsValid = TRUE;

    errChk (GetLines (text, txtLen, &lines, &numLines));

    if (direction == RegExpr_SearchBackwards)
    {
        currLine = numLines - 1;
        incr = -1;
    }
    else
    {
        currLine = 0;
        incr = 1;
    }

    for (linesLeft = numLines; linesLeft > 0; linesLeft--, currLine += incr)
    {
        pcLineBeg = lines[currLine].beginLine;
        pcLineEnd = lines[currLine].endLine;
        lineLen = (int)(pcLineEnd - pcLineBeg);

        colsLeft = lineLen;

        if (direction == RegExpr_SearchBackwards) {   /* optimization */
            startCol = ColOfPrevChar (pcLineBeg, colsLeft);
            switch (RegExpr_MatchText (handle, pcLineBeg, lineLen, matchPolicy, matchLen)) {
                case RegExpr_Match:
                    break;          /* might be another match later in line */
                case RegExpr_NoMatch:
                    colsLeft -= _ismbblead(pcLineBeg[0]) ? 2 : 1; /* don't bother with first col again */
                    break;
                case RegExpr_NotInText:
                    continue;      /* this is a big time saver! */
            }
        } else {   /* default to forwards if anything else */
            startCol = 0;
        }

        /* If searching backwards and asked to find the largest number of chars */
        if ((direction == RegExpr_SearchBackwards) && (matchPolicy == RegExpr_MatchLargestNumChars))
        {
            for (currCol = startCol; colsLeft > 0; )
            {
                switch (MatchText (handle, pcLineBeg, lineLen, currCol,
                 currCol, matchPolicy, &nextCol))
                {
                    case RegExpr_Match:
                        /* If the last byte of a previous match is not equal to the last byte */
                        /* in the current match, leave and use previous match             */
                        if ((*matched) && ((pcLineBeg-text+nextCol) != (*matchCol+*matchLen)) )
                            goto Done;
                        *matched = TRUE;
                        *matchCol = currCol+pcLineBeg-text;
                        *matchLen = nextCol - currCol;
                        break;
                    default:
                        /* otherwise if a previous match was found, leave */
                        if (*matched)
                            goto Done;
                        break;
                }
                numBytesInChar = NumBytesInCharAtLoc (&pcLineBeg[currCol]);
                colsLeft -= numBytesInChar;
                currCol = ColOfPrevChar (pcLineBeg, currCol);
            }

            // Match might have been found but not gone to Done because of search
            // backwards/match largest optimization. Make sure we goto Done here
            // if a match was found rather than loop to the next line.
            if (*matched)
                goto Done;
        }
        else
        {
            for (currCol = startCol; colsLeft > 0; )
            {
                switch (MatchText (handle, pcLineBeg, lineLen, currCol, currCol,
                 matchPolicy, &nextCol))
                {
                    case RegExpr_Match:
                        *matched = TRUE;
                        *matchCol = currCol+pcLineBeg-text;
                        *matchLen = nextCol - currCol;
                        goto Done;
                        /*break;*/
                    case RegExpr_NoMatch:
                        break;
                    case RegExpr_NotInText:  /* optimization */
                        if (direction == RegExpr_SearchForwards)
                        {
                            colsLeft = 0;
                            continue; // done with this line. exit for loop.
                        }
                        break;
                }
                numBytesInChar = NumBytesInCharAtLoc (&pcLineBeg[currCol]);
                colsLeft -= numBytesInChar;
                if (direction == RegExpr_SearchBackwards)
                    currCol = ColOfPrevChar (pcLineBeg, currCol);
                else
                    currCol += numBytesInChar;
            }
        }
    }
Done:
Error:
    if (lines)
        free (lines);
    if (handleIsValid)
        RegExpr_Destroy (handle);
    if (!*matched)
    {
        *matchCol = -1;
        *matchLen = -1;
    }
    return error;
}

char * CVIFUNC RegExpr_GetErrorString (int error)
{
    switch (error)
        {
        case RegExprErr_OutOfMemory:
            return "Out of memory";
        case RegExprErr_UnmatchedChar:
            return "Unmatched character";
        case RegExprErr_InvalidCharInRange:
            return "Invalid character in range";
        case RegExprErr_EndWithBackslash:
            return "Cannot end with '\\\\'";
        case RegExprErr_InvalidHexValue:
            return "Invalid hex value following '\\\\x'";
        case RegExprErr_OperatorOnEmptyPattern:
            return "Operator applied to empty pattern";
        case RegExprErr_EmptyLeftSide:
            return "Left side of '|' is empty";
        case RegExprErr_EmptyRightSide:
            return "Right side of '|' is empty";
        case RegExprErr_EmptyGroup:
            return "Empty group";
        case RegExprErr_InvalidRange:
            return "Invalid range";
        case RegExprErr_EmptySet:
            return "Empty set";
        case RegExprErr_EmptyInputString:
            return "Empty input string";
        case RegExprErr_NullInputString:
            return "NULL input string";
        case RegExprErr_MultibyteCharsNotAllowedInRange:
            return "Multibyte characters not allowed in range";
        default:
            if (error < 0)
                return "Unknown error";
            else
                return "No error";
        }
}


char * CVIFUNC RegExpr_GetErrorElaboration (void)
{
    RegExprErrorInfo * errorInfo = GetThreadErrorInfo ();

    if (errorInfo == 0)
        return "Could not get thread error information";

    switch (errorInfo->errorNumber)
        {
        case RegExprErr_UnmatchedChar:
            sprintf (errorInfo->buffer, "Unmatched '%c'", errorInfo->errorChar);
            return errorInfo->buffer;
        case RegExprErr_InvalidCharInRange:
            sprintf (errorInfo->buffer, "Invalid character ('%c') in range",
                errorInfo->errorChar);
            return errorInfo->buffer;
        case RegExprErr_OperatorOnEmptyPattern:
            sprintf (errorInfo->buffer, "'%c' applied to empty pattern",
                errorInfo->errorChar);
            return errorInfo->buffer;
        default:
            return RegExpr_GetErrorString(errorInfo->errorNumber);
        }
}

static long NumBytesInCharAtLoc (const char *loc)
{
    if (CmbIsLeadByte (*loc))
        return 2;
    else
        return 1;
}

static long ColOfNextChar (const char *text, long col)
{
    return col + NumBytesInCharAtLoc (&text[col]);
}

static long ColOfPrevChar (const char *text, long col)
{
    char *p;

    p = CmbStrPrev (text, &text[col]);
    if (p)
        return p - text;
    else
        return -1;
}

