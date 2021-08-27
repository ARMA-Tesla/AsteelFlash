/*============================================================================*/
/*                        L a b W i n d o w s / C V I                         */
/*----------------------------------------------------------------------------*/
/*    Copyright (c) National Instruments 1987-1999.  All Rights Reserved.     */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Title:       toolbox.c                                                     */
/* Purpose:     provides commonly useful functions                            */
/*                                                                            */
/*============================================================================*/

#ifndef HAVE_CVI_RTE
#define HAVE_CVI_RTE    1
#endif


#if HAVE_CVI_RTE
#include <cviauto.h>
#endif
#include <userint.h>
#include <libsupp.h>
#include <errno.h>
#include <float.h>
#if _NI_mswin32_
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <winreg.h>
#endif /* _NI_mswin32 */
#include <utility.h>
#include <formatio.h>
#include "toolbox.h"
#if !HAVE_CVI_RTE
int __CVI_SystemCodePage = 0;
#endif

typedef struct ListStructTag
    {
    int signature;              /* debugging aid */
    int percentIncrease;        /* %of current size to increase by when list is out of space */
    int minNumItemsIncrease;    /* fixed number of items to increase by when list is out of space */
    int listSize;               /* number of items than can fit in the currently allocated memory */
    int itemSize;               /* the size of each item in the list (same for every item) */
    int numItems;               /* number of items currently in the list */
    unsigned char itemList[1];  /* resizable array of list elements */
    } ListStruct;

#define kDefaultAllocationPercentIncrease   10  /* increase list size by 10% every time it is full */
#define kDefaultAllocationminNumItemsIncrease   4   /* always increase list size by 4 items when it is full */

     /* how many items to expand the list by when it becomes full = current listSize (in items) + (hiword percent of list size) +   loword */
#define NUMITEMSPERALLOC(list) Max(((*list)->listSize * ((*list)->percentIncrease + 100)) / 100 , (*list)->minNumItemsIncrease)

#define ITEMPTR(list, item)     &(((char *)(void *)&(*list)->itemList)[ (*(list))->itemSize * (item)])

#define LIST_SIGNATURE          CAT4CHARS('L', 'I', 'S', 'T');

#define MOUSE_EVENTS_SIGNATURE  CAT4CHARS('M', 'E', 'V', 'T');

#define CALLBACK_LINK_SIGNATURE CAT4CHARS('L', 'I', 'N', 'K')
#define THREAD_SLEEP_TIME       10L


typedef struct
    {
    void            *ptr;
    unsigned int    size;
    } HandleRecord;

typedef struct
    {
    int             signature;                  /* debug help, makes this structure identifiable in Dynamic Memory Display */
    char            *type;                      /* copy of type string passed to ChainCtrlCallback(), example "DAQChart", or "DAQNumeric", or "Button With Right Click Help", etc... */
    void            *callbackData;              /* callback data passed in to ChainCtrlCallback */
    CtrlCallbackPtr callback;                   /* callback function passed in to ChainCtrlCallback */
    void            *previousCallbackData;      /* if previously linked, this a ptr to the previous link structure */
    CtrlCallbackPtr previousCallback;           /* if previously linked, this is LinkedCtrlCallback() */
    }   *CtrlCallbackChainLink;

typedef struct
    {
    int             signature;                  /* debug help, makes this structure identifiable in Dynamic Memory Display */
    char            *type;                      /* copy of type string passed to ChainPanelCallback() */
    void            *callbackData;              /* callback data passed in to ChainPanelCallback */
    PanelCallbackPtr callback;                  /* callback function passed in to ChainPanelCallback */
    void            *previousCallbackData;      /* if previously linked, this a ptr to the previous link structure */
    PanelCallbackPtr previousCallback;          /* if previously linked, this is LinkedPanelCallback() */
    }   *PanelCallbackChainLink;

    /*  For adding extra mouse events to a control (see EnableExtendedMouseEvents) */
typedef struct
    {
    int     signature;          /*  debug help, makes this structure identifiable in Dynamic Memory Display */
    int     panel;              /*  owning panel of the ctrl receiving extended mouse events */
    int     ctrl;               /*  the control receiving extended mouse events */
    int     rightButtonDown;    /*  was the right button down the last time we checked? */
    int     leftButtonDown;     /*  was the left button down the last time we checked? */
    int     dispose;            /*  set to TRUE when the control is discarded, causes this struct to be discarded on the next delayed callback */
    int     lastX, lastY;       /*  mouse position the last time we checked */
    double  minPeriod;          /*  min time between mouse events */
    int     enabled;            /*  TRUE if extended mouse events are currently enabled for the control */
    } *MouseEventsInfo;

    /*** Delay Queues ***/

typedef struct  DelayQueueStructTag *DelayQueue;      /* The DelayQueue abstract data type */

typedef struct
    {
    DelayedCallbackFunc callback;
    void                *callbackData;
    double              time;
    } DelayQueueEntry;

typedef struct DelayQueueStructTag
    {
    ListType    actionList; /* sorted by latest to soonest expiration times */
    int         panel;
    int         ownsPanel;
    int         timer;
    } DelayQueueStruct;

#define DISCARD_THREAD_LOCALS_DEFERRED_CALL_TIMEOUT 2000    /* 2 seconds */
    /*  Add thread local variables here */
typedef struct
    {
    unsigned    randomSeed;            /* Used in the random related functions. */
    int         reSeed;                /* Used in the random related functions. */
    double      numJ, numK, numH;      /* Used in the random related functions. */
    long        seedX, seedY, seedZ;   /* Used in the random related functions. */
    char        evtBuf[128];           /* Used in the UILEventString function. */
    DelayQueue  delayQueue;            /* Used in the PostDelayedCall function. */
    int         fontPanel;             /* Used in the GetMetaFontInfo function. */
    int         txtMsg;                /* Used in the GetMetaFontInfo function. */
    int         tabComparePanel;       /* Used in the SortCtrlListByTabOrder function. */
    } ThreadLocalVars;

    /* Add the initial values of the thread local variables here. */
static const ThreadLocalVars initialThreadLocalVarsVals = {0x38589391,      /* randomSeed           */
                                                           TRUE,            /* reSeed               */
                                                           0.0, 0.0, 0.0,   /* numJ, numK, numH     */
                                                           0, 0, 0,         /* seedX, seedY, seedZ  */
                                                           {0},             /* evtBuf               */
                                                           0,               /* delayQueue           */
                                                           0,               /* fontPanel            */
                                                           0,               /* txtMsg               */
                                                           };
typedef enum {
    kChainCallbackLock = 0,
    kExtMouseEventsLock,
    kErrorListLock,
    kNumLocks
    } LockEnum;

    /* Declaration of thread safe variable macros. */
#if HAVE_CVI_RTE
DefineThreadSafeVar (ListType, TsvPanelsWithLinkedCallbacksList);
#endif  // HAVE_CVI_RTE

/******* For SaveBitmapToFile *********/
typedef struct tagRRGGBB{
   unsigned char RR;
   unsigned char GG;
   unsigned char BB;
} RRGGBB;

/******* For PostDeferredCallToThreadAndWait *********/
typedef enum
    {
    kCallNotPosted = 0,
    kCallPosted,
    kCallExecuting,
    kCallExecutionComplete
    } DeferredCallState;

typedef struct tagWaitDeferredCallbackData
    {
    DeferredCallbackPtr userCallbackFuncPtr;
    void                *userCallbackData;
    DeferredCallState   callState;
    int                 callWaitAbandoned;
    } WaitDeferredCallbackData;


/******* For SetCtrlToolTipAttribute *********/

#define kToolTipHelpPanelColor          16777088    // on non-win32
#define kToolTipHelpTextColor           VAL_BLACK   // on non-win32
#define kToolTipHoverDistance           3
#define kDefaultToolTipDelay            0.8         // seconds

typedef enum
    {
    kToolTip_MouseOut,
    kToolTip_MouseIn,
    kToolTip_Shown
    } ToolTipState;

#define kCtrlToolTipId  "CtrlToolTip"

struct SharedTipPanelTag;

typedef struct
    {
    struct SharedTipPanelTag    *tipPanel;
    ToolTipState                state;
    Point                       lastMousePoint;
    char *                      tipText;
    int                         visible;
    int                         panel;          // panel that owns the ctrl
    int                         ctrl;           // control with the tip
    } CtrlToolTip;

#define kSharedTipPanelId "SharedTipPanel"

typedef struct SharedTipPanelTag
    {
    int         disposingPanel; // TRUE iff disposing the panel
    int         topMostPanel;   // The panel that owns the floating tip panel and this structure
    ListType    tipList;        // List of CtrlToolTip pointers
    int         panel;          // floating tip panel
    int         timer;          // tip checking timer
    int         textMsg;        // text msg on floating tip panel
    CtrlToolTip *activeTip;     // CtrlToolTip for currently displayed tip, NULL if no tip is displayed
    } SharedTipPanel;

typedef struct CVINiErrInfo
    {
    int     errorCode;
    char    *errorString;
    } CVINiErrInfo;

 /********************/
 /* Static Functions */
 /********************/

static int      CVICALLBACK ListMemBlockCmp(void *a, void *b, int size);

static void     CVIFUNC FilterDownToCorrectHeapPosition(void *array, int numElements, int elementSize, CompareFunction compareFunction);
static void     CVIFUNC FilterUpToCorrectHeapLocation(void *array, int index, int elementSize, CompareFunction compareFunction);
static void     CVIFUNC BuildHeap(void *array, int numElements, int elementSize, CompareFunction compareFunction);

static int      CVIFUNC SetIntAttributeForCtrlsInCtrlList(int panel, int attribute, int value, int numCtrls, ListType ctrlList);
static int      CVIFUNC IsTBBoundsAttr(int attribute);

static void     CVIFUNC InterleavePartialLastScan(void *list, int itemSize, int numChannels, int numScans, int numItems);
static void     CVIFUNC TransposeDataInPlace(void *list, int itemSize, int numPoints, int numChannels);
static int      CVIFUNC ConvertNumStrToUint (const char *str, unsigned long *n, int *negative);

static int      CVICALLBACK ProgressCancelCallback(int panel, int ctrl, int event, void *callbackData, int eventData1, int eventData2);
static int      CVIFUNC InternationalTimeString(time_t rawTime, char timeString[], int bufferSize);
static int      CVIFUNC InternationalDateString(time_t rawTime, char dateString[], int bufferSize);

static void     CVIANSI ToolBoxAtExitFunction(void);
static double   CVIFUNC GetRand(void);

static int      CVICALLBACK LinkedCtrlCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
static int      CVIFUNC FindCtrlCallbackLink(int panel, int ctrl, const char *type,  CtrlCallbackChainLink *link);
static int      CVIFUNC GetOriginalCtrlCallbackData(int panel, int ctrl, void **callbackData);
static int      CVIFUNC SetOriginalCtrlCallbackData(int panel, int ctrl, void *callbackData);

static void     CVIFUNC InstallObjectDeletionProcedure(void);
static void     CVIANSI ObjectDeletionProcedure(void);

static int      CVIANSI RecordPanelWithLinkedCallback(int panel);
static void     CVIANSI UnrecordPanelWithLinkedCallback(int panel);
static int      CVICALLBACK ComparePanelToPnlWthLnkdCbksRec(void *ptrToItem, void *listItemPtr);
static int      CVIANSI GetPanelWithLinkedCallbackByIndex (int index, int *panel);
static void     CVIANSI DestroyPanelsWithLinkedCallbacksList(void);

static int      CVICALLBACK LinkedPanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2);
static int      CVIFUNC FindPanelCallbackLink(int panel, const char *type,  PanelCallbackChainLink *link);
static int      CVIFUNC GetOriginalPanelCallbackData(int panel, void **callbackData);
static int      CVIFUNC SetOriginalPanelCallbackData(int panel, void *callbackData);

static int      CVIFUNC LastCtrl(int panel);
static int      CVIFUNC NextCtrl(int panel, int ctrl, int wrap);

static void     CVIFUNC MouseEvents_Check(MouseEventsInfo info);
static int      CVICALLBACK MouseEventsCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
static void     CVICALLBACK MouseEvents_CheckCallback(void *callbackData);
static char *   CVIFUNC ToolBox_GetDAQErrorString (short int error);
static char *   CVIFUNC ToolBox_InternalGetDAQErrorString (short int error);
static void     CVIFUNC QSort(char *array, long numElems, long elemSize, CompareFunction cmpFn);

static DelayQueue   CVIFUNC DelayQueueCreate(int panel);
static void     CVIFUNC DelayQueueDispose(DelayQueue delayQueue, int discardUIObjects);
static int      CVIFUNC DelayQueueInsert(DelayQueue delayQueue, DelayedCallbackFunc callback, void *callbackData, double delay);

static int      CVICALLBACK DelayQueueCmpEntriesByTime(void *entry1, void *entry2);
static void     CVIFUNC DelayQueueSetNextInterval(DelayQueue delayQueue);
static int      CVICALLBACK DelayQueueHandler(int panelHandle, int controlID, int event, void *callbackData, int eventData1, int eventData2);

static void     CVIFUNC InitializeMultithreadingIfNecessary(void);
static void     CVIANSI UninitializeMultithreadingProcedure(void);

static void     CVICALLBACK DiscardThreadLocalVarsPtrsRrscCallback (void *threadLocalPtr, int event, void *callbackData, unsigned int threadID);
static void     CVICALLBACK FreeThreadLocalResourcesCallback (void *callbackData);
static int      CVIFUNC NeedToFreeThreadLocalResources (ThreadLocalVars *pThreadLocalVars);
static ThreadLocalVars* CVIFUNC GetThreadLocalVars(void);

static int      CVIFUNC GetLock(LockEnum name);
static int      CVIFUNC ReleaseLock(LockEnum name);
static void     CVIANSI DeleteLocksProcedure(void);

static int      CVICALLBACK CompareCtrlTabOrder (void *ctrl1, void *ctrl2);
static int      CVICALLBACK ComparePanelZPlaneOrder (void *panel1, void *panel2);

static void CVICALLBACK PostDeferredCallToThreadAndWaitCallback (void *callbackData);

static int  CVICALLBACK CtrlToolTip_TimerCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
static int  CVICALLBACK CtrlToolTip_CtrlCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
static int  CtrlToolTip_Show(CtrlToolTip *tip);
static int  CtrlToolTip_Hide(CtrlToolTip *tip);
static CtrlToolTip * CtrlToolTip_Create(int panel, int ctrl);
static void CtrlToolTip_Dispose(CtrlToolTip *tip);
static int  CtrlToolTip_ShouldHide(CtrlToolTip *tip, int *shouldHide);
static int CVICALLBACK CtrlToolTip_CheckTip (CtrlToolTip *tip);

static int  SharedTipPanel_HideTip(SharedTipPanel *tipPanel);
static int  SharedTipPanel_ShowTip(SharedTipPanel *tipPanel, CtrlToolTip *tip);
static void SharedTipPanel_Dispose(SharedTipPanel *tipPanel);
static int  SharedTipPanel_Create(int topMostPanel, SharedTipPanel **tipPanel);
static int  SharedTipPanel_GetTipPanel(int panel, SharedTipPanel **tipPanel);
static int  SharedTipPanel_InstallTip(SharedTipPanel *tipPanel, CtrlToolTip *tip);
static int  SharedTipPanel_RemoveTip(SharedTipPanel *tipPanel, CtrlToolTip *tip);
static CtrlToolTip * SharedTipPanel_GetActiveTip(SharedTipPanel *tipPanel);
static int CVICALLBACK SharedTipPanel_TimerCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
static int CVICALLBACK SharedTipPanel_OwnerPanelCallback (int panelHandle, int event, void *callbackData, int eventData1, int eventData2);

static int  GetToolbarHelpBGColor (void);
static int  GetToolbarHelpTextColor (void);
static int  WinColorToCVIColor(int c);
static int  TopMostParent(int panel, int *topMostParent);
static void CVIFUNC RectPinnedToRect (Rect rect, Rect rectToPin, Rect *pinnedRect);
static int  PointToPanel (int panel, Point point);
static int  PointConvertToGlobal (int childPanel, Point *point);
static int  CVIFUNC PointConvertCoordinates (int topPanel, int childPanel, Point *point, int convertFromChildCoordinates);

static int  CVINiErrParseFilesIntoList (ListType fileList, ListType errorInfoList);
static void CVINiErrDisposeList (ListType *errInfoListPtr);
static int  CVINiErrParseFile (const char *fileName, ListType errInfoList);
static int CVICALLBACK CVINiErrInfoCompareFunc (void *item1, void *item2);

static int       gThreadLocalHandle   = 0;
static int       gLockHandles[kNumLocks] = {{0}, {0}};
static ListType  gCVIErrorList = NULL;
static ListType  gSharedNIErrorList = NULL;

int CVIFUNC ToolboxInit(void)
{
#if !HAVE_CVI_RTE
    int    cp = GetACP();
    CPINFO cpInfo;

    /* get the currently active code page */
    GetCPInfo(cp, &cpInfo);
    __CVI_SystemCodePage = (!cpInfo.LeadByte[0] && !cpInfo.LeadByte[1]) ? _MB_CP_SBCS : cp;
#endif

    return 0;
}
#if HAVE_CVI_RTE
static int CVIFUNC GetLock(LockEnum name)
{
    static int gLockInitDone = 0;       /* Lock has already been initialized */
    static int sLockInitNow = 0;        /* Lock is currently being initialized. */
    int i, prev = 0;

    /* If the locks have never been initialized, do so now. */
    if (!gLockInitDone)
        {
        prev = InterlockedExchange(&sLockInitNow, 1);
        if (prev)                                   /* If it is initialized or is in the process of initialing. */
            {
            while (!gLockInitDone)          /* Wait until initializing the locks finishes   */
                Sleep(THREAD_SLEEP_TIME);
            }
        else
            {
            for (i = 0; i < kNumLocks; i++)
                CmtNewLock(NULL, 0, &gLockHandles[i]);
            atexit(DeleteLocksProcedure);
            gLockInitDone = 1;
            }
        }

    CmtGetLock(gLockHandles[name]);
    return TRUE;
}
#endif  // HAVE_CVI_RTE

#if HAVE_CVI_RTE
static int CVIFUNC ReleaseLock(LockEnum name)
{
    CmtReleaseLock(gLockHandles[name]);
    return FALSE;
}
#endif  // HAVE_CVI_RTE

#if HAVE_CVI_RTE
static void CVIANSI DeleteLocksProcedure(void)
{
    int i;

    if (!CVIRTEHasBeenDetached())
        for (i = 0; i < kNumLocks; i++)
            CmtDiscardLock(gLockHandles[i]);
}
#endif  // HAVE_CVI_RTE

#if HAVE_CVI_RTE
static void CVIFUNC InitializeMultithreadingIfNecessary(void)
{
    static int sMultithreadingInitDone = 0;       /* Multithreading has already been initialized */
    static int sMultithreadingInitNow  = 0;       /* Thread local is currently being initialized. */
    int prev = 0;

    /* Check if already been initialized, if not, do so now. */
    if (!sMultithreadingInitDone)
        {
        prev = InterlockedExchange(&sMultithreadingInitNow, 1);
        if (prev)                                   /* If it is initialized or is in the process of initializing. */
            {
            while (!sMultithreadingInitDone)        /* Wait until the thread initializing the multithreading finishes */
                Sleep(THREAD_SLEEP_TIME);
            }
        else
            {
            CmtNewThreadLocalVar (sizeof(ThreadLocalVars),          /* Create the thread locals. */
                                  (void *)&initialThreadLocalVarsVals,
                                  DiscardThreadLocalVarsPtrsRrscCallback, NULL,
                                  &gThreadLocalHandle);

            InitializeTsvPanelsWithLinkedCallbacksList();           /* Create the thread safe variables. */
            SetTsvPanelsWithLinkedCallbacksList(NULL);

            atexit(UninitializeMultithreadingProcedure);
            sMultithreadingInitDone = 1;
            }
        }
}
#endif  // HAVE_CVI_RTE

#if HAVE_CVI_RTE
static void CVIANSI UninitializeMultithreadingProcedure(void)
{
    /* This function used to call CmtDiscardThreadLocalVar.
       That is a problem:  This may be in a DLL which is being dynamically
       unloaded by a program.  We also added the check for
       CVIRTEHasBeenDetached, which is a new function in CVI 5.0.
    */
    if (!CVIRTEHasBeenDetached())
        {
        if (gThreadLocalHandle)
            CmtDiscardThreadLocalVar (gThreadLocalHandle);

        UninitializeTsvPanelsWithLinkedCallbacksList();
        }
}
#endif  // HAVE_CVI_RTE

#if HAVE_CVI_RTE
static void CVICALLBACK FreeThreadLocalResourcesCallback (void *callbackData)
{
    ThreadLocalVars *pThreadLocalVars = (ThreadLocalVars *)callbackData;

    if (pThreadLocalVars->delayQueue)
        {
        DelayQueueDispose (pThreadLocalVars->delayQueue, TRUE);
        free(pThreadLocalVars->delayQueue);
        pThreadLocalVars->delayQueue = NULL;
        }
    if (pThreadLocalVars->fontPanel > 0)
        {
        DiscardPanel (pThreadLocalVars->fontPanel);
        pThreadLocalVars->fontPanel = 0;
        }
}
#endif  // HAVE_CVI_RTE

static int CVIFUNC NeedToFreeThreadLocalResources (ThreadLocalVars *pThreadLocalVars)
{
    int needToFree = 0;

    if (pThreadLocalVars != NULL)
        needToFree = ((pThreadLocalVars->delayQueue != 0) || (pThreadLocalVars->fontPanel > 0));
    return needToFree;
}

#if HAVE_CVI_RTE
static void CVICALLBACK DiscardThreadLocalVarsPtrsRrscCallback (void *threadLocalPtr, int event, void *callbackData, unsigned int threadID)
{
    ThreadLocalVars *pThreadLocalVars = (ThreadLocalVars *)threadLocalPtr;

    /* We need to free some of the thread local resources in the thread that created them. */
    /* In the case that the discard function was called, we post a deferred call to free   */
    /* the resources in the appropriate thread.  In the other case, the thread is being    */
    /* destroyed, so we know that this calback is being called in the approprate thread.   */
    if (NeedToFreeThreadLocalResources (pThreadLocalVars))
        {
        if (event == EVENT_TLV_DISCARD_FUNCTION_CALLED)
            PostDeferredCallToThreadAndWait (FreeThreadLocalResourcesCallback,
                                             pThreadLocalVars, threadID,
                                             DISCARD_THREAD_LOCALS_DEFERRED_CALL_TIMEOUT);
        else
            FreeThreadLocalResourcesCallback (pThreadLocalVars);
        }

    return;
}
#endif  // HAVE_CVI_RTE

#if HAVE_CVI_RTE
static ThreadLocalVars* CVIFUNC GetThreadLocalVars(void)
{
    ThreadLocalVars *pThreadLocalVars = 0;

    InitializeMultithreadingIfNecessary();

    CmtGetThreadLocalVar (gThreadLocalHandle, &pThreadLocalVars);

    return pThreadLocalVars;
}
#endif  // HAVE_CVI_RTE



/****************
 *   MB STUFF   *
 ****************/


long CmbFirstByteOfChar (long c)
{
    long leadByte;

    leadByte = CmbCharCodeLeadByte(c);
    if (leadByte != 0)
        return leadByte;
    else
        return (c & CmbSingleByteMask);
}

long CVIFUNC CmbNumChars (const char *s)
{
    return _mbslen((void*)(s));
}

long CVIFUNC CmbStrEq(const unsigned char *s1, const unsigned char *s2)
{
    return !strcmp((void*)s1,(void*)s2);
}

long CVIFUNC CmbStrEqI(const unsigned char *s1, const unsigned char *s2)
{
    return !CmbStrICmp((void*)s1,(void*)s2);
}

long CVIFUNC CmbStrEqN(const unsigned char *s1, const unsigned char *s2, long n)
{
    return !strncmp((void*)s1,(void*)s2,n);
}

long CVIFUNC CmbStrEqNI(const unsigned char *s1, const unsigned char *s2, long n)
{
    return !CmbStrNICmp((void*)s1,(void*)s2,n);
}

unsigned char * CVIFUNC CmbStrUpr(unsigned char *s)
{
    unsigned char *p;

    for (p = s; *p != 0; CmbStrInc (p))
        if ((*p >= 'a') && (*p <= 'z'))
            *p = *p + ('A' - 'a');
    return s;
}

unsigned char * CVIFUNC CmbStrLwr(unsigned char *s)
{
    unsigned char *p;

    for (p = s; *p != 0; CmbStrInc (p))
        if ((*p >= 'A') && (*p <= 'Z'))
            *p = *p + ('a' - 'A');
    return s;
}

long CVIFUNC CmbStrByteIs(const unsigned char *buf, long offset, long ch)
    /**************************************************/
    /* Returns TRUE if all the following are true:    */
    /*      offset >= 0                               */
    /*      buf[offset] == ch                         */
    /*      buf[offset] is not a trail byte           */
    /* For example, you can change:                   */
    /*      if (buf[ndx] == '*')                      */
    /* to:                                            */
    /*      if (CmbStrByteIs (buf, ndx, '*'))         */
    /**************************************************/
{
    return offset>=0 && buf[offset]==ch && CmbStrByteType(buf,offset)!=CMB_TRAIL_BYTE;
}

unsigned char * CVIFUNC CmbStrLastChar (const unsigned char *str)
    /**************************************************/
    /* Returns a pointer to the last char of str      */
    /* If str is empty, returns a ptr to the nul      */
    /**************************************************/
{
    unsigned char *ptr;

    Assert (str);

    ptr = CmbStrPrev (str, StringEnd (str));

    if (ptr == NULL)
        ptr = (unsigned char*)str;

    return ptr;
}

unsigned char * CVIFUNC StringEnd(const unsigned char *string)
{
    /* returns a ptr to the null byte in a string */
    return (unsigned char*)string + strlen((const char *)string);
}

/********************************************************************/
    /*  floating point comparison with a tolerance to compensate for the inability to exactly represent some numbers */
    /*  returns -1 if a < b, 0 if a == b, and 1 if a > b */
int CVIFUNC FP_Compare(double a, double b)
{
    if (FP_EQ(a,b))
        return 0;
    else
    if (FP_GT(a,b))
        return 1;
    else
        return -1;
}

/********************************************************************/

    /*  floating point comparison with a tolerance to compensate for the inability to exactly represent some numbers */
int CVIFUNC FP_EQ(double a, double b)       /* is a == b    */
{
    double tmp;

    if(b == 0)
        tmp= a;
    else if (a == 0)
        tmp = b;
    else
        tmp= a/b - 1;

    if(tmp < FP_CompareEpsilon && tmp > -FP_CompareEpsilon)
        if (ABS_VAL(a-b) < FP_CompareEpsilon)
            return TRUE;

    return FALSE;
}

/********************************************************************/

    /*  floating point comparison with a tolerance to compensate for the inability to exactly represent some numbers */
int CVIFUNC FP_GT(double a, double b)       /* is a > b */
{
    double tmp;
    int reply;

    if(b == 0)
        reply= (a > 0);
    else if(a == 0)
        reply= (b < 0);
    else
        {
        tmp= a/b;
        if(tmp > 0 && (ABS_VAL(a-b) <= FP_CompareEpsilon))                      /* both neg, or both pos */
            {
            if(b > 0)
                reply= (tmp - 1 >= FP_CompareEpsilon);
            else
                reply= (tmp - 1 <= -FP_CompareEpsilon);
            }
        else
            reply= a > b;
        }

    return reply;
}

/********************************************************************/

    /*  floating point comparison with a tolerance to compensate for the inability to exactly represent some numbers */
int CVIFUNC FP_GE(double a, double b)       /* is a >= b    */
{
    double tmp;
    int reply;

    if(b == 0)
        reply= (a >= 0);
    else if(a == 0)
        reply= (b <= 0);
    else
        {
        tmp= a/b;
        if(tmp > 0 && (ABS_VAL(a-b) <= FP_CompareEpsilon))                      /* both neg, or both pos */
            {
            if(b > 0)
                reply= (tmp - 1 > -FP_CompareEpsilon);
            else
                reply= (tmp - 1 < FP_CompareEpsilon);
            }
        else
            reply= a > b;
        }

    return reply;
}

/********************************************************************/

    /*  floating point comparison with a tolerance to compensate for the inability to exactly represent some numbers */
int CVIFUNC FP_LT(double a, double b)       /* is a < b */
{
    return FP_GT(b, a);
}

/********************************************************************/

    /*  floating point comparison with a tolerance to compensate for the inability to exactly represent some numbers */
int CVIFUNC FP_LE(double a, double b)       /* is a <= b    */
{
    return FP_GE(b, a);
}

/********************************************************************/

double CVIFUNC Pin(double value, double low, double high)
{
    if (value < low)
        return low;
    else
    if (value > high)
        return high;
    else
        return value;
}

/********************************************************************/
#if HAVE_CVI_RTE
double CVIFUNC Random(double minimum, double maximum)
{
    double  range = maximum - minimum;

    if (range < 0)
        return minimum;
    else
        return minimum + GetRand() * range;
}
#endif  // HAVE_CVI_RTE

/*******************************************************/
#if HAVE_CVI_RTE
void CVIFUNC SetRandomSeed(unsigned long seed)
{
    ThreadLocalVars *pThreadLocalVars = 0;

    pThreadLocalVars = GetThreadLocalVars();

    if (seed)
        pThreadLocalVars->randomSeed = seed;         /* Multithreaded, each thread has its own thread local seed. */
    else
    do
        {
        pThreadLocalVars->randomSeed = clock();
        } while (!pThreadLocalVars->randomSeed);     /* make sure we don't somehow get zero, since that would cause an infinite loop */

    pThreadLocalVars->reSeed = TRUE;
}
#endif  // HAVE_CVI_RTE

/*******************************************************/

static const unsigned char gDoubleInfinityByteArray[sizeof(double)]
    = { 0x7F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static const unsigned char gDoubleNanByteArray[sizeof(double)]
    = { 0x7F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static const unsigned char gDoubleExponentMask[sizeof(double)]
    = { 0x7F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static const unsigned char gDoubleMantissaMask[sizeof(double)]
    = { 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

static double ByteArrayToDouble(const unsigned char *byteArray)
{
    volatile double result = 0.;
    int i;

    // NOTE: the following code only works on x86 processors because of Endianess
    for (i = 0; i < sizeof(double); ++i)
        ((volatile unsigned char*) &result)[sizeof(double)-1-i] = byteArray[i];
    return result;
}

static void CheckDouble(double number, int *mantissa, int *exponent)
{
    int i;
    unsigned char currByte;

    *mantissa = 0;
    *exponent = 1;

    // NOTE: the following code only works on x86 processors because of Endianess
    for (i = 0; i < sizeof(double); ++i)
        {
        currByte = ((unsigned char*) &number)[sizeof(double)-1-i];
        *exponent &= (gDoubleExponentMask[i] == (currByte & gDoubleExponentMask[i]));
        *mantissa |= (currByte & gDoubleMantissaMask[i]);
        }
}

double CVIFUNC PositiveInfinity(void)
{
    return ByteArrayToDouble(gDoubleInfinityByteArray);
}

double CVIFUNC NegativeInfinity(void)
{
    return -PositiveInfinity();
}

double CVIFUNC NotANumber(void)
{
    return ByteArrayToDouble(gDoubleNanByteArray);
}

int CVIFUNC IsNotANumber(double number)
{
    int mantissa, exponent;
    
    CheckDouble(number, &mantissa, &exponent);
    return (mantissa && exponent);
}

// Returns 1 for +ive infinity, -1 for -ive infinity, and 0 otherwise
int CVIFUNC IsInfinity(double number)
{
    int mantissa, exponent;
    
    CheckDouble(number, &mantissa, &exponent);
    return (!mantissa && exponent) ? (number < 0.0 ? -1 : 1) : 0;
}

int CVIFUNC IsFinite(double number)
{
    int mantissa, exponent;
    
    CheckDouble(number, &mantissa, &exponent);
    return !exponent;
}

/*******************************************************/

#if HAVE_CVI_RTE
static double CVIFUNC GetRand(void)
{
    double          randNum;
    ThreadLocalVars *pThreadLocalVars = 0;

    pThreadLocalVars = GetThreadLocalVars();

    if (pThreadLocalVars->reSeed)
        {
        pThreadLocalVars->reSeed = FALSE;
        pThreadLocalVars->seedX = 0;
        }

    while(!pThreadLocalVars->seedX)
        {
        pThreadLocalVars->seedX = pThreadLocalVars->randomSeed & 0x3FFF;
        pThreadLocalVars->seedY = (pThreadLocalVars->seedX * 8191) & 0x3FFF;
        pThreadLocalVars->seedZ = (pThreadLocalVars->seedY * 8191) & 0x3FFF;
        pThreadLocalVars->numJ = 1.0 / 30269.0;
        pThreadLocalVars->numK = 1.0 / 30307.0;
        pThreadLocalVars->numH = 1.0 / 30323.0;
        }

    pThreadLocalVars->seedX = (171 * pThreadLocalVars->seedX) % 30269;
    pThreadLocalVars->seedY = (172 * pThreadLocalVars->seedY) % 30307;
    pThreadLocalVars->seedZ = (170 * pThreadLocalVars->seedZ) % 30323;

    randNum = pThreadLocalVars->seedX * pThreadLocalVars->numJ + pThreadLocalVars->seedY * pThreadLocalVars->numK + pThreadLocalVars->seedZ * pThreadLocalVars->numH;
    randNum -= floor(randNum);
    if(randNum < 0) randNum += 1.0; /* if floor does a Round To Nearest */
    return randNum;
}
#endif  // HAVE_CVI_RTE

/********************************************************************/

void CVIFUNC ConvertArrayType(const void *sourceArray, int sourceDataType, void *targetArray,
                            int targetDataType, int numberOfPoints)
{
    int     i;
    double  temp = 0.;

    for (i=0; i < numberOfPoints; i++)
        {
        switch (sourceDataType)
            {
            case VAL_CHAR:
                temp = ((char *)sourceArray)[i];
                break;
            case VAL_INTEGER:
                temp = ((int *)sourceArray)[i];
                break;
            case VAL_SHORT_INTEGER:
                temp = ((short int *)sourceArray)[i];
                break;
            case VAL_FLOAT:
                temp = ((float *)sourceArray)[i];
                break;
            case VAL_DOUBLE:
                temp = ((double *)sourceArray)[i];
                break;
            case VAL_UNSIGNED_SHORT_INTEGER:
                temp = ((unsigned short *)sourceArray)[i];
                break;
            case VAL_UNSIGNED_INTEGER:
                temp = ((unsigned int *)sourceArray)[i];
                break;
            case VAL_UNSIGNED_CHAR:
                temp = ((unsigned char *)sourceArray)[i];
                break;
            }

        switch (targetDataType)
            {
            case VAL_CHAR:
                ((char *)targetArray)[i] = (char)temp;
                break;
            case VAL_INTEGER:
                ((int *)targetArray)[i] = (int)temp;
                break;
            case VAL_SHORT_INTEGER:
                ((short *)targetArray)[i] = (short)temp;
                break;
            case VAL_FLOAT:
                ((float *)targetArray)[i] = (float)temp;
                break;
            case VAL_DOUBLE:
                ((double *)targetArray)[i] = temp;
                break;
            case VAL_UNSIGNED_SHORT_INTEGER:
                ((unsigned short *)targetArray)[i] = (unsigned short)temp;
                break;
            case VAL_UNSIGNED_INTEGER:
                ((unsigned int *)targetArray)[i] = (unsigned int)temp;
                break;
            case VAL_UNSIGNED_CHAR:
                ((unsigned char *)targetArray)[i] = (unsigned char)temp;
                break;
            }
        }
}

/********************************************************************/

void CVIFUNC DoAssert(int passed, const char *fileName, int line, const char *msg)
{
    extern void  CVIFUNC_C _UPLibReportError(long, long, ...);

    if (!passed)
        {
        char buf[256];

            sprintf(buf, "Assert!\n\n%s\n\nThe error was detected on line %d of file '%s'.\n\nProgram aborting.", msg ? msg : "Programming error detected.", line, fileName);

#if HAVE_CVI_RTE
            MessagePopup("Assert", buf);
#else
            MessageBox(NULL,buf,"Assert",MB_OK);
#endif  // HAVE_CVI_RTE
            abort();
        }
}

/********************************************************************/

int CVIFUNC ShowHtmlHelp(const char szFile[], unsigned int uCommand, void *dwData)
{
    static HINSTANCE    hHtmlHelpModule = NULL;
    static HWND         (WINAPI *pfHtmlHelpA)
                            (HWND hwndCaller,LPCSTR pszFile,UINT uCommand,DWORD dwData) = NULL;
    HWND                status;

    /* Load the Library and get the func ptr, if not already obtained. */
    /* The OCX will be unloaded (if loaded) when the program terminates */
    if (pfHtmlHelpA == NULL)
    {
#if HAVE_CVI_RTE
        CA_InitActiveXThreadStyleForCurrentThread (0, COINIT_APARTMENTTHREADED);
#endif
        if ((hHtmlHelpModule = LoadLibrary(TEXT("Hhctrl.ocx"))) == NULL)
            return ToolErr_ErrorLoadingDLL ;
        if ((pfHtmlHelpA = (void *)GetProcAddress(hHtmlHelpModule, TEXT("HtmlHelpA"))) == NULL)
        {
            FreeLibrary(hHtmlHelpModule);
            return ToolErr_FuncNotFound;
        }
    }
    /* Call the function in the ActiveX control using the function pointer. */
    status = pfHtmlHelpA (NULL, szFile, uCommand, (DWORD) dwData);

    return 0;
}

/********************************************************************/


    /*  Binary search numElements of size elementSize in array for a match
        to the. item. Return the index of the element that matches (0 - numElements - 1).
        If no match is found return the -i-1 where i is the index (0 - numElements)
        where the item should be placed. (*theCmp)(a,b) should return  <0 if a<b,
        0 if a==b, >0 if a>b.

        This function is like the C-Library function bsearch() except that this
        function returns the index where the item should be placed if it is not
        found.
    */
int CVIFUNC BinSearch(const void *array, int numElements, int elementSize, const void *itemPtr, CompareFunction compareFunction)
{
    int low, high, mid, cmp;
    void *arrayItemPtr;

    for(low=0, high= numElements-1, mid=0, cmp= -1; low <= high; )
        {
        mid = (low + high) >> 1;

        arrayItemPtr = (void *) (((char *)array) + (mid*elementSize));
        cmp = compareFunction ? compareFunction((void *)itemPtr, arrayItemPtr)
                              : ListMemBlockCmp((void *)itemPtr, arrayItemPtr, elementSize);
        if (cmp == 0)
            return mid;
        else
        if (cmp < 0)
            high= mid - 1;
        else
            low= mid + 1;
        }
    if (cmp > 0)
        mid++;

    return -mid-1;
}

/********************************************************************/

    /*  Swap numBytes between block1 and block2.
    */
void CVIFUNC SwapBlock(void *block1, void *block2, int numBytes)
{
    if(((int)block1 | (int)block2 | numBytes) & 3)
        {
        char *p, *q;
        char x;
        for(p= (char *)block1, q= (char *)block2; --numBytes >= 0; p++, q++){ x= *p; *p= *q; *q= x; }
        }
    else
        {
        long int    *p, *q, x;
        for(p= (long int*)block1, q= (long int*)block2, numBytes >>= 2; --numBytes >= 0; p++, q++){ x= *p; *p= *q; *q= x; }
        }
}

/********************************************************************/

    /*  returns negative UIL error,  0 if time conversion/formatting problem, or 1 if successful */
#if HAVE_CVI_RTE
int CVIFUNC GetFileCLibTime(const char filePath[], time_t *time)
{
    int         error = UIENoError;
    int         month;
    int         day;
    int         year;
    int         hours;
    int         minutes;
    int         seconds;
    struct tm   fileTM;
    int         savedBole = SetBOLE(FALSE); /* don't break on GetFileDate() errors */

    switch (GetFileDate (filePath, &month, &day, &year))
        {
        case 0:
            break;
        case -1:
            errChk( UIEFileWasNotFound);
            break;
        case -4:
            errChk( UIEOutOfMemory);
            break;
        case -3:
        default:
            errChk( UIEIOError);
            break;
        }

    switch (GetFileTime (filePath, &hours, &minutes, &seconds))
        {
        case 0:
            break;
        case -1:
            errChk( UIEFileWasNotFound);
            break;
        case -4:
            errChk( UIEOutOfMemory);
            break;
        case -3:
        default:
            errChk( UIEIOError);
            break;
        }

    fileTM.tm_sec = seconds;
    fileTM.tm_min = minutes;
    fileTM.tm_hour = hours;
    fileTM.tm_mday = day;
    fileTM.tm_mon = month - 1;
    fileTM.tm_year = year - 1900;
    fileTM.tm_isdst = -1;       /* -1 means "I don't know, you tell me..." */

    *time = mktime(&fileTM);

    return *time != (time_t)-1;

Error:
    SetBOLE(savedBole);
    return error;
}
#endif  // HAVE_CVI_RTE

/********************************************************************/

    /*  returns negative UIL error,  # of characters formatted if success, or 0 if time conversion/formatting problem */
#if HAVE_CVI_RTE
int CVIFUNC InternationalFileTime(const char filePath[], char timeString[], int bufferSize)
{
    time_t  time;
    int     error = UIENoError;
    int     success;

    errChk( success = GetFileCLibTime(filePath, &time));

    if (success)
        return InternationalTimeString(time, timeString, bufferSize);
    else
        return 0;

Error:
    return error;
}
#endif  // HAVE_CVI_RTE

/********************************************************************/

    /*  returns negative UIL error, # of characters formatted if success, or 0 if time conversion/formatting problem */
#if HAVE_CVI_RTE
int CVIFUNC InternationalFileDate(const char filePath[], char dateString[], int bufferSize)
{
    time_t  time;
    int     error = UIENoError;
    int     success;

    errChk( success = GetFileCLibTime(filePath, &time));

    if (success)
        return InternationalDateString(time, dateString, bufferSize);
    else
        return 0;

Error:
    return error;
}
#endif  // HAVE_CVI_RTE

/********************************************************************/

    /*  returns # of bytes formatted if success, or 0 if time conversion/formatting problem */
int CVIFUNC InternationalTime(char timeString[], int bufferSize)
{
    time_t      rawTime;

    time (&rawTime);
    return InternationalTimeString(rawTime, timeString, bufferSize);
}

/********************************************************************/

    /*  returns # of bytes formatted if success, or 0 if time conversion/formatting problem */
int CVIFUNC InternationalDate(char dateString[], int bufferSize)
{
    time_t      rawTime;

    time (&rawTime);
    return InternationalDateString(rawTime, dateString, bufferSize);
}

/********************************************************************/

static int CVIFUNC InternationalTimeString(time_t rawTime, char timeString[], int bufferSize)
{
    char        *oldLocale;
    struct tm   *localTime;
    int         result;

    oldLocale = setlocale (LC_TIME, NULL);
    setlocale (LC_TIME, "");

    localTime = localtime (&rawTime);
    result = strftime (timeString, bufferSize, "%X", localTime);

    setlocale (LC_TIME, oldLocale);
    return result;
}

/********************************************************************/

static int CVIFUNC InternationalDateString(time_t rawTime, char dateString[], int bufferSize)
{
    char        *oldLocale;
    struct tm   *localTime;
    int         result;

    oldLocale = setlocale (LC_TIME, NULL);
    setlocale (LC_TIME, "");

    localTime = localtime (&rawTime);
    result = strftime (dateString, bufferSize, "%x", localTime);

    setlocale (LC_TIME, oldLocale);
    return result;
}

/********************************************************************/

char * CVIFUNC StrDup(const char *stringToDuplicate)
{
    char    *duplicatedString = 0;
    int     size;

    if (stringToDuplicate)
        {
        size = strlen(stringToDuplicate) + 1;
        duplicatedString = (char *)malloc(size);
        if (duplicatedString)
            memcpy(duplicatedString, stringToDuplicate, size);
        }

    return duplicatedString;
}

/**************************************************************************/

    /*  This function adds stringToAdd to the end of *string. */
int CVIFUNC AppendString(char **string, const char *stringToAdd, int lengthToAdd)
{
    int     currLen;
    char    *newString;

    AssertMsg(string, "NULL passed to first parameter of AppendString()");

    if (!stringToAdd)
        return TRUE;

    if (lengthToAdd < 0)
        lengthToAdd = strlen(stringToAdd);
    else
        {
        int index;
                    /* If the stringToAdd contains a terminator, we have to stop there */
        for (index = 0; index < lengthToAdd && stringToAdd[index] != 0; index++);
        lengthToAdd = index;
        }

    if (lengthToAdd == 0)
        return TRUE;

    if (!*string)
        {
        newString = (char *)malloc(lengthToAdd + 1);
        currLen = 0;
        }
    else
        {
        currLen = strlen(*string);
        newString = realloc(*string, currLen + lengthToAdd + 1);
        }

    if (newString)
        {
        memmove(newString + currLen, stringToAdd, lengthToAdd);
        newString[currLen + lengthToAdd] = 0;
        if (CmbStrByteType(newString, currLen + lengthToAdd - 1) == CMB_LEAD_BYTE)
            newString[currLen + lengthToAdd -1] = 0; /* Don't copy just a lead byte */
        *string = newString;
        return TRUE;
        }
    else
        return FALSE;
}

/*****************************************************************************************/

    /*  This function adds stringToPrepend to the beginning of *string. */
int CVIFUNC AddStringPrefix(char **string, const char *stringPrefix, int lengthOfPrefix)
{
    int     currLen;
    char    *newString;

    if (!stringPrefix)
        return TRUE;

    if (lengthOfPrefix < 0)
        lengthOfPrefix = strlen(stringPrefix);
    else
        {
        int index;
                    /* If the stringPrefix contains a terminator, we have to stop there */
        for (index = 0; index < lengthOfPrefix && stringPrefix[index] != 0; index++);
        lengthOfPrefix = index;
        }

    if (stringPrefix == 0)
        return TRUE;

    if (!*string)
        {
        newString = (char *)malloc(lengthOfPrefix + 1);
        currLen = 0;
        }
    else
        {
        currLen = strlen(*string);
        newString = realloc(*string, currLen + lengthOfPrefix + 1);
        }

    if (newString)
        {
        memmove(newString + lengthOfPrefix, newString, currLen);
        memmove(newString, stringPrefix, lengthOfPrefix);
        newString[currLen + lengthOfPrefix] = 0;
        *string = newString;
        return TRUE;
        }
    else
        return FALSE;
}

/*****************************************************************************************/

    /*  This function adds quotes around the string. */
int CVIFUNC AddStringQuotes(char **string)
{
    if (!AppendString(string, "\"", -1))
        return FALSE;
    if (!AddStringPrefix(string, "\"", -1))
        return FALSE;
    return TRUE;
}

/*****************************************************************************************/

/*
   Creates a dynamically allocated copy of a string without
   any leading or trailing white space.
*/
char * CVIFUNC StrDupWithoutSurrWhiteSpace(const char *string)
{
    const char *firstNonWhiteChar = NULL, *lastNonWhiteChar = NULL;
    char *copy = NULL;
    int   len;

    firstNonWhiteChar = SkipWhiteSpace(string);
    lastNonWhiteChar = (const char *)StringEnd ((const unsigned char *)firstNonWhiteChar); /* lastNonWhiteChar will point to the null byte */
    if (lastNonWhiteChar > firstNonWhiteChar)
        {
        /* there are non white chars */
        CmbStrDec(firstNonWhiteChar, (const unsigned char *)lastNonWhiteChar);

        while (isspace ((unsigned char)*lastNonWhiteChar)) /* look backwards for last non white space */
            CmbStrDec (firstNonWhiteChar, (const unsigned char *)lastNonWhiteChar);

        /* lastNonWhiteChar can't be NULL since there are some non white chars */

        if (CmbIsLeadByte(*lastNonWhiteChar))
            len = lastNonWhiteChar - firstNonWhiteChar + 2;     /* an extra one for the second byte of the mb char */
        else
            len = lastNonWhiteChar - firstNonWhiteChar + 1;
        }
    else
        len = 0;

    copy = malloc(len+1);
    if (copy)
        {
        memmove (copy, firstNonWhiteChar, len);
        copy[len] = '\0';
        }

    return copy;
}

/**************************************************************************/

char * CVIFUNC SkipWhiteSpace(const char *s)
{
    if (s)
        while (*s && isspace((unsigned char)*s))
            s++;
    return (char*)s;
}

/**************************************************************************/

char * CVIFUNC SkipNonWhiteSpace(const char *s)
{
    if (s)
        while (*s && !isspace((unsigned char)*s))
            CmbStrInc((const unsigned char *)s);
    return (char*)s;
}

/**************************************************************************/

int CVIFUNC HasNonWhiteSpace (const char *s)
{
    if (s == NULL)
        return FALSE;
    else
        return *SkipWhiteSpace(s) != '\0';
}

/**************************************************************************/

void CVIFUNC StringCopyMax(char *dest, const char *source, int destBufferSize)
{
    int destSize, sourceSize;

#ifndef TB_NO_SAFETY_CHECKS
    AssertMsg(dest, "Null string passed to first parameter of StringCopyMax");
#endif

    if (source && destBufferSize > 0)
        {
        sourceSize = strlen(source) + 1;
        destSize = Min(sourceSize, destBufferSize);
        memmove(dest, source, destSize);
        if (destSize < sourceSize)
            if (CmbStrByteType(source, destSize - 2 /* exclude NULL */) == CMB_LEAD_BYTE)
                dest[destSize - 2] = 0;
            else
                dest[destSize - 1] = 0;     /* add null terminator since we didn't copy it */
        }
}

/***********************************************************************************/

char * CVIFUNC StringInsert(char *destinationString, const char *stringToInsert, int destinationBufferSize)
{
    int destinationLength, lengthOfStringToInsert;

 /* insert stringToInsert before destinationString in destinationString but don't exceed destinationBufferSize in destinationString
    if destinationString is a pointer in a larger string then destinationBufferSize applies only to the section pointed to by destinationString */

#ifndef TB_NO_SAFETY_CHECKS
    AssertMsg(stringToInsert, "NULL string passed to first parameter of StringInsert");
    AssertMsg(destinationString, "NULL string passed to second parameter of StringInsert");
#endif

    destinationLength = strlen(destinationString);
    lengthOfStringToInsert = strlen(stringToInsert);

    if ((destinationLength + lengthOfStringToInsert) > destinationBufferSize-1)
        {
        lengthOfStringToInsert = destinationBufferSize - destinationLength-1;
        if (CmbStrByteType(stringToInsert, lengthOfStringToInsert-1) == CMB_LEAD_BYTE)
            lengthOfStringToInsert--;   /* if insertStringSize is in middle of multi byte char, then don't copy that char */
        }


    if (lengthOfStringToInsert > 0)
        {
        memmove(destinationString + lengthOfStringToInsert, destinationString, destinationLength+1);    /* slide the original string forward, +1 to keep NULL byte */
        memmove(destinationString, stringToInsert, lengthOfStringToInsert);         /* copy new string into the new space in front */
        }

    return destinationString+lengthOfStringToInsert;
}

/**************************************************************************/

int CVIFUNC StrICmp (const char *s1, const char *s2)
{
    return _mbsicmp((void*)(s1), (void*)(s2));
}

/**************************************************************************/

int CVIFUNC StrNICmp (const char *s1, const char *s2, int n)
{
    return _mbsnbicmp((void*)s1,(void*)s2,n);
}

/**************************************************************************/

void CVIFUNC RemoveSurroundingWhiteSpace(char *string)
{
    char *start, *end, *lastNonWS;
    int lengthWithoutLeadingWS;

    if (string)
        {
        start = SkipWhiteSpace(string);
        lengthWithoutLeadingWS = strlen(start);
        memmove (string, start, lengthWithoutLeadingWS);

        lastNonWS = end = string;
        while (end < string+lengthWithoutLeadingWS)
        {
            if (!isspace(*end))
                lastNonWS  = end;
            CmbStrInc((const unsigned char *)end);
        }

        if (end != string)
            CmbStrInc((const unsigned char *)lastNonWS);
        *lastNonWS = 0;
        }
}


/**************************************************************************/

int CVIFUNC StrICmpWithoutSurrWhiteSpace (const char *s1, const char *s2)
{
    int ch1, ch2;

    if (s1 == NULL)
        if (s2 == NULL)
            return 0;
        else
            return -1;
    else
        if (s2 == NULL)
            return 1;
        else
            {
            while (isspace(*s1))
                s1++;
            while (isspace(*s2))
                s2++;

            for (;; CmbStrInc((const unsigned char *)s1),CmbStrInc((const unsigned char *)s2))
                {
                ch1 = CmbGetC(s1);
                if (CmbIsSingleC(ch1))
                    ch1 = toupper(ch1);
                ch2 = CmbGetC(s2);
                if (CmbIsSingleC(ch2))
                    ch2 = toupper(ch2);

                if (ch1 == 0)
                    {
                    while (isspace(*s2))
                        *s2++;
                    if (*s2 == 0)   /* check for trailing whitespace */
                        return 0;
                    else
                        return -1;
                    }
                else
                    {
                    if (ch2 == 0)
                        {
                        while (isspace(*s1))
                            s1++;
                        if (*s1 == 0)  /* check for trailing whitespace */
                            return 0;
                        else
                            return 1;
                        }
                    else
                    if (ch1 != ch2)
                        {
                        if (ch1 < ch2)
                            return -1;
                        else if (ch1 > ch2)
                            return 1;
                        }
                    }
                }
            }
}

/****************************************************************************/

    /*  This function splices list2 into list1 and stores the result in the newly
        allocated string splicedList.  All lists are strings of characters with commas separating
        the items.  The resulting splicedList will have as many items as the longest list.  Each item
        in the result splicedList will be the same as the corresponding item in list1 unless list1
        does not have a corresponding item.  In that case, list2's corresponding item is used.

        Example:
        char    *list1 = "Time,,Pressure, Temperature";
        char    *list2 = "Time, Column 1, Column 2, Column 3, Column 4, Column 5";
        char    *splicedList = 0;

        SpliceCommaSeparatedList(list1, list2, &splicedList);
        printf("Spliced list = %s\n", splicedList);

        This prints: Spliced list = Time, Column 1,Pressure, Temperature, Column 4, Column 5

        This function is used to fill in any missing items in a partially specified list with
        default items.

        The function returns 0 if successful, or -12 (UIEOutOfMemory) if there is not
        enough memory to create the splicedList.
    */
int CVIFUNC SpliceCommaSeparatedList(const char *list1, const char *list2, char **splicedList)
{
    int         error = UIENoError;
    char        *newList = 0;
    const char  *currentList1;
    const char  *currentList2;
    const char  *list1Comma = NULL;
    const char  *list2Comma = NULL;
    int         item1Length;
    int         item2Length;

    currentList1 = list1;
    currentList2 = list2;
    *splicedList = 0;

    while (currentList1 || currentList2)
        {
            /*  Find next header item */
        if (currentList1)
            {
            list1Comma = (const char *)CmbStrChr((const unsigned char *)currentList1, ',');

            if (list1Comma)
                item1Length = list1Comma - currentList1;
            else
                item1Length = strlen(currentList1);
            }
        else
            item1Length = 0;

            /*  Find next default header item */
        if (currentList2)
            {
            list2Comma = (const char *)CmbStrChr((const unsigned char *)currentList2, ',');

            if (list2Comma)
                item2Length = list2Comma - currentList2;
            else
                item2Length = strlen(currentList2);
            }
        else
            item2Length = 0;

        if (item1Length > 0)    /* if there is a header item, use it */
            nullChk( AppendString(&newList, currentList1, item1Length));
        else
        if (item2Length > 0)   /* else, use the default header item, if any */
            nullChk( AppendString(&newList, currentList2, item2Length));

        if (list1Comma || list2Comma)               /* add trailing comma if we aren't on the last item */
            nullChk( AppendString(&newList, ",", -1));


        if (list1Comma) /* skip to next header */
            currentList1 = list1Comma + 1;
        else
            currentList1 = 0;

        if (list2Comma) /* skip to next header */
            currentList2 = list2Comma + 1;
        else
            currentList2 = 0;
        }

    if (!newList)       /* if list1 and list2 had no items to contribute to list3, then make it an empty list */
        nullChk( AppendString(&newList, "", -1));

Error:
    if (error < 0)
        {
        free(newList);
        *splicedList = 0;
        return error;
        }
    else
        {
        *splicedList = newList;
        return 0;
        }
}

/***********************************************************************/

int CVIFUNC StrToInt (const char *str, int *n)
{
    unsigned long   ulval;
    int             negative;

    if (!ConvertNumStrToUint (str, &ulval, &negative))
        return FALSE;
    if (negative)
        {
        if (ulval > 0x80000000L)     /* absolute value is too large */
            return FALSE;
        *n = -(long)ulval;
        }
    else
        {
        if (ulval > 0x7fffffffL)     /* absolute value is too large */
            return FALSE;
        *n = ulval;
        }
    return TRUE;
}

/**************************************************************************/

int CVIFUNC StrToUInt (const char *str, unsigned int *n)
{
    unsigned long   ulval;
    int             negative;

    if (!ConvertNumStrToUint (str, &ulval, &negative))
        return FALSE;
    if (negative)
        return FALSE;
    *n = ulval;
    return TRUE;
}

/***********************************************************************/

static int CVIFUNC ConvertNumStrToUint (const char *s, unsigned long *n, int *negative)
{
    int             ch;
    int             numDigits = 0;
    int             signSeen = FALSE;
    int             endOfNonWhiteSpace = FALSE;
    unsigned long   ulval;

    *negative = FALSE;
    ulval = 0;
    for (; (ch = *s) != '\0'; s++)
        {
        if (endOfNonWhiteSpace)     /* should be all blanks now */
            if (isspace ((unsigned char)ch))
                continue;
            else            /* there was an embedded blank */
                return FALSE;
        if (isspace((unsigned char)ch))
            {
            if (signSeen || numDigits > 0)
                endOfNonWhiteSpace = TRUE;
            continue;
            }
        if ((ch == '+') || (ch == '-'))
            {
            if (signSeen || numDigits > 0)   /* embedded sign */
                return FALSE;
            if (ch == '-')
                *negative = TRUE;
            signSeen = TRUE;
            continue;
            }
        if (isdigit((unsigned char)ch))
            {
            if (((ulval == 429496729L) && (ch > '5')) ||
                                            (ulval > 429496729L))
                return FALSE;     /* would overlflow */
            ulval = (ulval * 10) + (ch - '0');
            numDigits++;
            continue;
            }
        return FALSE;    /* invalid character */
        }
    if (numDigits == 0)
        return FALSE;
    *n = ulval;
    return TRUE;
}

/********************************************************************/

#if HAVE_CVI_RTE
    /*  this is a more convenient Set/Get-BreakOnLibraryErrors() function */
int CVIFUNC SetBOLE(int on)
{
    int wasOn;

    wasOn = GetBreakOnLibraryErrors();

    if (on)
        EnableBreakOnLibraryErrors();
    else
        DisableBreakOnLibraryErrors();

    return wasOn;
}
#endif  // HAVE_CVI_RTE

/********************************************************************/

static void CVIFUNC InterleavePartialLastScan(void *list, int itemSize, int numChannels, int numScans, int numItems)
{
    int     numChannelsLeft;
    int     channel;
    int     destination;
    char    itemBuf[20];    /* should be large enough for one item of any conceivable numeric type */

    Assert (itemSize < 20);

    numChannelsLeft = numItems - numScans * numChannels;
    for (channel = numChannelsLeft - 1; channel >= 0; channel--)
        {
        memmove(itemBuf, (char *)list + (numItems - 1) * itemSize, itemSize);
        destination = (channel + 1) * numScans;
        memmove((char *)list + (destination + 1) * itemSize, (char *)list + destination * itemSize,
                (numItems - destination - 1) * itemSize);
        memmove((char *)list + destination * itemSize, itemBuf, itemSize);
        }
}

/*******************************************************/

static void CVIFUNC TransposeDataInPlace(void *list, int itemSize, int numPoints, int numChannels)
{
    int     scan;
    int     item;
    int     numScans;
    int     numChannelsLeft = numChannels;
    int     *listPtr = list;

    Assert (itemSize < 20);

    numScans = numPoints / numChannels;
    if (numChannels > 1)
        {
        do  {
            for (scan = 1; scan < numScans; scan++)
                {
                item = listPtr[scan*numChannelsLeft];  /* extract relevant channel from start of scan */
                memmove(&listPtr[scan+1], &listPtr[scan], sizeof(int)* (scan*numChannelsLeft - scan));
                listPtr[scan] = item;
                }

            listPtr = listPtr + numScans;
            numChannelsLeft--;
            } while (numChannelsLeft > 1);

        InterleavePartialLastScan(list, itemSize, numChannels, numScans, numPoints);
        }
}

/*******************************************************/

void CVIFUNC TransposeData(void *data, int dataType, int numPoints, int numChannels)
{
    char    *reOrderedData = 0;
    int     dataSize;
    int     numScans;
    int     oldIndex;
    int     newIndex;
    int     scan = 0;
    int     channel = 0;
    int     numScansForThisChannel;
    int     extraChannelsSampled;   /* > 0 if the last scan is incomplete */

    if (numChannels < 2)
        return;

    dataSize = GetDataTypeSize(dataType);
    numScans = numPoints / numChannels;

    extraChannelsSampled = numPoints - numScans * numChannels;

    if (numScans < 2 &&  extraChannelsSampled == 0)
        return;

    if (dataSize > 0 && numPoints > 0)
        {
        reOrderedData = (char *)malloc(numPoints * dataSize);

        if (reOrderedData)
            {
            newIndex = 0;
            for (channel = 0; channel < numChannels; channel++)
                {
                numScansForThisChannel = numScans;
                if (channel < extraChannelsSampled)
                    numScansForThisChannel++;

                for (scan = 0; scan < numScansForThisChannel; scan++)
                    {
                    oldIndex = scan * numChannels + channel;
                    memmove(reOrderedData + newIndex * dataSize, (char *)data + oldIndex * dataSize, dataSize);
                    newIndex++;
                    }
                }
            memmove(data, reOrderedData, numPoints * dataSize);
            }
        else    /* this is slower, but takes no memory */
            TransposeDataInPlace(data, dataSize, numPoints, numChannels);
        }

    free(reOrderedData);
}

/********************************************************************/

    /*  converts from 1-based heap index to zero based array index */
#define HeapItemPtr(array, oneBasedIndex, elementSize)  ((void *)((char *)(array) + (elementSize) * ((oneBasedIndex)-1)))

    /*  Expects the array to be a heap of index - 1 elements.
        the index'th element (1-based) will be inserted into the heap at the correct
        spot, making array a heap of index elements.
    */
static void CVIFUNC FilterUpToCorrectHeapLocation(void *array, int index, int elementSize, CompareFunction compareFunction)
{
    int     parent, cmp;
    void    *indexPtr, *parentPtr;

    for (parent = index >> 1; parent > 0; parent = parent >> 1)
        {
        indexPtr = HeapItemPtr(array, index, elementSize);
        parentPtr = HeapItemPtr(array, parent, elementSize);

        cmp = compareFunction ? compareFunction(indexPtr, parentPtr)
                              : ListMemBlockCmp(indexPtr, parentPtr, elementSize);
        if (cmp <= 0)
            break;  /* element is in correct spot */

        SwapBlock(indexPtr, parentPtr, elementSize);
        index = parent;
        }
}

/**************************************************************************/

    /*  Expects array to be a heap except for the root element (element 1).
        The root element is moved to it correct location so that the array is a heap.
    */
static void CVIFUNC FilterDownToCorrectHeapPosition(void *array, int numElements, int elementSize, CompareFunction compareFunction)
{
    int     leftChild, rightChild, largestChild;
    int     root, cmp;
    void    *rootPtr, *leftChildPtr, *rightChildPtr, *largestChildPtr;

    /* use 1-based indexing inside function for array representation of binary tree */

    for (root = 1;;)
        {
        leftChild = root << 1;
        rightChild = leftChild + 1;

        rootPtr = HeapItemPtr(array, root, elementSize);

        if (leftChild <= numElements)
            leftChildPtr = HeapItemPtr(array, leftChild, elementSize);
        else
            leftChildPtr = 0;

        if (rightChild <= numElements)
            rightChildPtr = HeapItemPtr(array, rightChild, elementSize);
        else
            rightChildPtr = 0;

        if (leftChildPtr == 0)
            break;                  /* no more children, we are done */

        if (rightChildPtr == 0)
            largestChild = leftChild;
        else
            {
            cmp = compareFunction ? compareFunction(leftChildPtr, rightChildPtr)
                                  : ListMemBlockCmp(leftChildPtr, rightChildPtr, elementSize);
            if (cmp >= 0)
                largestChild = leftChild;
            else
                largestChild = rightChild;
            }

        largestChildPtr = HeapItemPtr(array, largestChild, elementSize);

        cmp = compareFunction ? compareFunction(rootPtr, largestChildPtr)
                              : ListMemBlockCmp(rootPtr, largestChildPtr, elementSize);
        if (cmp >= 0)
            break;  /* element is in correct spot */

        SwapBlock(rootPtr, largestChildPtr, elementSize);
        root = largestChild;
        }
}

/**************************************************************************/

static void CVIFUNC BuildHeap(void *array, int numElements, int elementSize, CompareFunction compareFunction)
{
    int     index;

    for (index = 2; index <= numElements; index++)
        FilterUpToCorrectHeapLocation(array, index, elementSize, compareFunction);
}

/**************************************************************************/

void CVIFUNC HeapSort(void *array, int numElements, int elementSize, CompareFunction compareFunction)
{
    int index;

    BuildHeap(array, numElements, elementSize, compareFunction);

    for (index = numElements; index > 1; index--)
        {
            /*  move greatest item for the root to last position that is still part of the heap
                and move the item there to root of the heap.
            */
        SwapBlock(array, HeapItemPtr(array, index, elementSize), elementSize);

            /*  filter the item at the top of the heap into its correct position in
                the now one element smaller heap.
            */
        FilterDownToCorrectHeapPosition(array, index - 1, elementSize, compareFunction);
        }
}

/********************************************************************/

    /* This function provides a stable alternative to quicksort */
void CVIFUNC InsertionSort(void *array, int numElements, int elementSize, CompareFunction compareFunction)
{
    int     start, item, cmp;
    char    *topItem, *bottomItem;

    for (start = 1; start < numElements; start++)
        {
        topItem = (char *)array + start * elementSize;
        bottomItem = topItem - elementSize;

        for (item = start; item > 0; item--)
            {
            cmp = compareFunction ? compareFunction(bottomItem, topItem)
                                  : ListMemBlockCmp(bottomItem, topItem, elementSize);
            if (cmp > 0)
                {
                SwapBlock(bottomItem, topItem, elementSize);
                topItem -= elementSize;
                if (item > 1)
                    bottomItem -= elementSize;
                }
            else
                break;
            }
        }
}

/********************************************************************/

    /* starts a tinny sounding whine on the built PC speaker */
#if HAVE_CVI_RTE
void CVIFUNC StartPCSound(unsigned int frequency)
{
#if defined(_NI_mswin_)  /* _NI_mswin_ is defined automatically when compiling with the Windows version of CVI. */
    outp(0x43, (char)0xb6);         /* prepare timer */

    if (frequency > 0)          /* divide number of cycles per second by the input frequency */
        frequency = (unsigned) (1193180L/frequency);

    outp(0x42, (char)frequency);
    outp(0x42, (char)(frequency >> 8));
    outp(0x61, (char)(inp(0x61) | 0x3));    /* turn on sound with bits 0 and 1 */
#endif /* _NI_mswin_ */
}
#endif  // HAVE_CVI_RTE

/****************************************************************************/

    /* A function you would pay money for after calling StartPCSound(). */
#if HAVE_CVI_RTE
void CVIFUNC StopPCSound(void)
{
#if defined(_NI_mswin_)  /* _NI_mswin_ is defined automatically when compiling with the Windows version of CVI. */
    outp(0x61, (char)(inp(0x61) & ~0x3));   /* turn off sound with bits 0 and 1 */
#endif /* _NI_mswin_ */
}
#endif  // HAVE_CVI_RTE

/********************************************************************/

Handle CVIFUNC NewHandle(unsigned int numBytes)
{
    void            *memPtr = NULL;
    HandleRecord    *hanPtr;

    if (numBytes)
        memPtr = calloc(numBytes, 1);
    hanPtr = (HandleRecord*)calloc(sizeof(HandleRecord), 1);
    if (hanPtr && (memPtr || numBytes == 0))
        {
        hanPtr->ptr = memPtr;
        hanPtr->size = numBytes;
        return (Handle)hanPtr;
        }
    else
        {
        free(memPtr);
        free(hanPtr);
        return NULL;
        }
}

/********************************************************************/

void CVIFUNC DisposeHandle(Handle handle)
{
    if (handle)
        {
        free(*handle);
        free((void *)handle);
        }
}

/********************************************************************/

unsigned int CVIFUNC GetHandleSize(Handle handle)
{
#ifndef TB_NO_SAFETY_CHECKS
    AssertMsg(handle, "Parameter 1 to GetHandleSize was NULL.");
#endif
    return ((HandleRecord *)handle)->size;
}

/********************************************************************/

int CVIFUNC SetHandleSize(Handle handle, unsigned int newSize)
{
    HandleRecord    *hanRecPtr = (HandleRecord *)handle;
    void            *newPtr, *oldPtr;
    unsigned int    oldSize;

#ifndef TB_NO_SAFETY_CHECKS
    AssertMsg(hanRecPtr, "Parameter 1 to SetHandleSize was NULL.");
#endif

    oldPtr = hanRecPtr->ptr;
    oldSize = hanRecPtr->size;
    if (oldSize == newSize)
        return TRUE;

    if (oldPtr == NULL)
        newPtr = malloc(newSize);
    else
        newPtr = realloc(oldPtr, newSize);

    if (newPtr || (newSize == 0))
        {
        hanRecPtr->ptr = newPtr;
        hanRecPtr->size = newSize;
        if (newSize > oldSize)
            memset ((char *)newPtr+oldSize, 0, newSize-oldSize);
        return TRUE;
        }
    else
        return FALSE;
}

/*******************************************************************************/

    /* BEGIN PATCH to 4.0.1 - jag 8-29-96 */
    /*  If numNewItems == 0 then expand the list by the number of items indicated by its allocation policy.
        If numNewItems > 0 then expand the list by exactly the number of items indicated.
        If numNewItems < 0 then expand the list by the absolute value of numNewItems plus the number of items indicated by its allocation policy.
        Returns TRUE for success, FALSE if out of memory
    */
static int CVIFUNC ExpandListSpace (ListType list, int numNewItems)
{
    if (numNewItems == 0)
        numNewItems = NUMITEMSPERALLOC(list);
    else
    if (numNewItems < 0)
        numNewItems = (-numNewItems) + NUMITEMSPERALLOC(list);

    /* END PATCH to 4.0.1 - jag 8-29-96 */
    if (SetHandleSize ((Handle)list,
        sizeof (ListStruct) +
        ((*list)->listSize + numNewItems) * (*list)->itemSize)) {
        (*list)->listSize += numNewItems;
        return TRUE;
        }
    else
        return FALSE;
}

/*******************************/

     /* This function reallocate the list, minus any currently unused portion of its allotted memory. */
void CVIFUNC ListCompact (ListType list)
{
#ifndef TB_NO_SAFETY_CHECKS
    if (!list)
        return;
#endif

    if (!SetHandleSize ((Handle)list,
        sizeof (ListStruct) + (*list)->numItems * (*list)->itemSize))
        return;

    (*list)->listSize = (*list)->numItems;
}

/*******************************/

ListType CVIFUNC ListCreate (int elementSize)
{
    ListType    list;

    list = (ListType)(NewHandle (sizeof(ListStruct)));   /* create empty list */
    if (list)
        {
        (*list)->signature = LIST_SIGNATURE;
        (*list)->numItems = 0;
        (*list)->listSize = 0;
        (*list)->itemSize = elementSize;
        (*list)->percentIncrease = kDefaultAllocationPercentIncrease;
        (*list)->minNumItemsIncrease = kDefaultAllocationminNumItemsIncrease;
        }

    return list;
}

/*******************************/

void CVIFUNC ListSetAllocationPolicy (ListType list, int minItemsPerAlloc, int percentIncreasePerAlloc)
{
#ifndef TB_NO_SAFETY_CHECKS
    if (!list)
        return;
#endif

    (*list)->percentIncrease = percentIncreasePerAlloc;
    (*list)->minNumItemsIncrease = minItemsPerAlloc;
}

/*******************************/

void CVIFUNC ListDispose (ListType list)
{
    DisposeHandle ((Handle)list);
}

/*******************************/

void CVIFUNC ListDisposePtrList(ListType list)
{
    int index;
    int numItems;

    if (list)
        {
        numItems = ListNumItems(list);

        for (index = 1; index <= numItems; index++)
            free(*(void **)ListGetPtrToItem(list, index));

        ListDispose(list);
        }
}

/*******************************/

void  CVIFUNC ListClear (ListType list)  /* keeps memory, resets the number of items to 0 */
{
    if (!list)
        return;
    (*list)->numItems = 0;
}

/*******************************/

ListType CVIFUNC ListCopy (ListType originalList)   /* copy is only as large as necessary */
{
    ListType    tempList = NULL;
    int         numItems;

    if (!originalList)
        return NULL;

    tempList = ListCreate ((*originalList)->itemSize);
    if (tempList) {
        numItems = ListNumItems (originalList);

        if (!SetHandleSize ((Handle)tempList,
            sizeof (ListStruct) + numItems * (*tempList)->itemSize)) {
            ListDispose (tempList);
            return NULL;
            }

        (*tempList)->numItems = (*originalList)->numItems;
        (*tempList)->listSize = (*originalList)->numItems;
        (*tempList)->itemSize = (*originalList)->itemSize;
        (*tempList)->percentIncrease = (*originalList)->percentIncrease;
        (*tempList)->minNumItemsIncrease = (*originalList)->minNumItemsIncrease;

        memcpy (ITEMPTR(tempList,0), ITEMPTR(originalList,0),
            numItems * (*tempList)->itemSize);
        }

    return tempList;
}

/********************************/

int CVIFUNC ListAppend (ListType list1, ListType list2)  /* list1 = list1 + list2 */
{
    int numItemsL1, numItemsL2;

    if (!list2)
        return TRUE;

    if (!list1)
        return FALSE;
    if ((*list1)->itemSize != (*list2)->itemSize)
        return FALSE;

    numItemsL1 = ListNumItems (list1);
    numItemsL2 = ListNumItems (list2);

    if (numItemsL2 == 0)
        return TRUE;

    if (!SetHandleSize ((Handle)list1,
        sizeof (ListStruct) + (numItemsL1 + numItemsL2) * (*list1)->itemSize))
        return FALSE;

    (*list1)->numItems = numItemsL1 + numItemsL2;
    (*list1)->listSize = numItemsL1 + numItemsL2;

    memmove (ITEMPTR(list1,numItemsL1), ITEMPTR(list2,0),
        numItemsL2 * (*list2)->itemSize);

    return TRUE;
}

/*******************************/

    /*  returns TRUE if the item is inserted, returns FALSE if out of memory or
        bad arguments were passed.
    */
int CVIFUNC ListInsertItem (ListType list, const void *ptrToItem, int itemPosition)
{
    return ListInsertItems (list, ptrToItem, itemPosition, 1);
}

/*******************************/

int CVIFUNC ListInsertItems (ListType list, const void *ptrToItems, int firstItemPosition, int numItemsToInsert)
{
    int numItems = (*list)->numItems;

#ifndef TB_NO_SAFETY_CHECKS
    if (!list)
        return FALSE;
    if (numItemsToInsert <= 0)
        return FALSE;
#endif

    if (firstItemPosition == numItems + 1)
        firstItemPosition = END_OF_LIST;
    else
    if (firstItemPosition > numItems)
        return FALSE;

    if ((*list)->numItems >= (*list)->listSize)
        if (!ExpandListSpace (list, -numItemsToInsert)) /* PATCH to 4.0.1 - jag 8-29-96 */
            return FALSE;

    if (firstItemPosition == FRONT_OF_LIST)
        if (numItems == 0)
           firstItemPosition = END_OF_LIST; /* special case for empty list */
        else
           firstItemPosition = 1;

    if (firstItemPosition == END_OF_LIST)  /* add at the end of the list */
        {
        if (ptrToItems)
            memcpy (ITEMPTR(list, numItems), ptrToItems,
                (*list)->itemSize * numItemsToInsert);
        else
            memset (ITEMPTR(list, numItems), 0,
                (*list)->itemSize * numItemsToInsert);

        (*list)->numItems += numItemsToInsert;
        }
    else
        {                                  /* move part of list up to make room for new item */
        memmove (ITEMPTR(list,firstItemPosition-1+numItemsToInsert),
            ITEMPTR(list,firstItemPosition-1),
            (numItems + 1 - firstItemPosition) * (*list)->itemSize);

        if (ptrToItems)
            memmove (ITEMPTR(list,firstItemPosition-1), ptrToItems,
                (*list)->itemSize * numItemsToInsert);
        else
            memset (ITEMPTR(list,firstItemPosition-1), 0,
                (*list)->itemSize * numItemsToInsert);

        (*list)->numItems += numItemsToInsert;
        }

    return TRUE;
}

/*******************************/

int CVIFUNC ListEqual (ListType list1, ListType list2)
{
    if (list1 == list2)
        return TRUE;

    if (list1 == NULL || list2 == NULL)
        return FALSE;

    if ((*list1)->itemSize == (*list1)->itemSize)
        if ((*list1)->numItems == (*list2)->numItems)
            return (memcmp (ITEMPTR(list1,0), ITEMPTR(list2,0),
                (*list1)->itemSize * (*list1)->numItems) == 0);

    return FALSE;
}

/*******************************/

     /* The item pointed to by ptrToItem is copied over the current item at itemPosition */
void CVIFUNC ListReplaceItem (ListType list, const void *ptrToItem, int itemPosition)
{
    ListReplaceItems (list, ptrToItem, itemPosition, 1);
}

/*******************************/

     /* The item pointed to by ptrToItems is copied over the current item at itemPosition */
void CVIFUNC ListReplaceItems (ListType list, const void *ptrToItems, int firstItemPosition, int numItemsToReplace)
{
#ifndef TB_NO_SAFETY_CHECKS
    if (!list || !ptrToItems || numItemsToReplace <= 0)
        return;
    if ((*list)->numItems == 0)
        return;
#endif

    if (firstItemPosition == END_OF_LIST)
        firstItemPosition = (*list)->numItems;
    else
    if (firstItemPosition == FRONT_OF_LIST)
        firstItemPosition = 1;

#ifndef TB_NO_SAFETY_CHECKS
    if (firstItemPosition <= 0  ||
        firstItemPosition + numItemsToReplace - 1 > (*list)->numItems)
        return;
#endif

    memmove (ITEMPTR(list,firstItemPosition-1), ptrToItems,
        (*list)->itemSize * numItemsToReplace);
}

/*******************************/

void CVIFUNC ListRemoveItem (ListType list, void *itemDestination, int itemPosition)
{
    ListRemoveItems (list, itemDestination, itemPosition, 1);
}

/*******************************/

void CVIFUNC ListRemoveItems (ListType list, void *itemsDestination, int firstItemPosition, int numItemsToRemove)
{
    int firstItemAfterChunk, numToMove;

#ifndef TB_NO_SAFETY_CHECKS
    if (!list || numItemsToRemove <= 0 || numItemsToRemove > (*list)->numItems)
        return;
#endif

    if (firstItemPosition == FRONT_OF_LIST)
        firstItemPosition = 1;
    else
    if (firstItemPosition == END_OF_LIST)
        firstItemPosition = (*list)->numItems;

#ifndef TB_NO_SAFETY_CHECKS
    if (firstItemPosition <= 0 ||
        firstItemPosition + numItemsToRemove - 1 > (*list)->numItems)   /* can't remove more than are there */
        return;
#endif

    if (itemsDestination != NULL)
         memcpy (itemsDestination, ITEMPTR(list,firstItemPosition-1),
            (*list)->itemSize * numItemsToRemove);

    firstItemAfterChunk = firstItemPosition + numItemsToRemove;
    numToMove = (*list)->numItems - (firstItemAfterChunk - 1);

    if (numToMove > 0)  /* move part of list down to cover hole left by removed item */
        memmove (ITEMPTR(list,firstItemPosition-1),
                 ITEMPTR(list,firstItemAfterChunk-1),
                 (*list)->itemSize * numToMove);

    (*list)->numItems -= numItemsToRemove;
}

/*******************************/

void CVIFUNC ListGetItem (ListType list, void *itemDestination, int itemPosition)
{
    ListGetItems (list, itemDestination, itemPosition, 1);
}

/*******************************/

void CVIFUNC ListGetItems(ListType list, void *itemsDestination, int firstItemPosition, int numItemsToGet)
{
#ifndef TB_NO_SAFETY_CHECKS
    if (!list || !itemsDestination || numItemsToGet <= 0)
        return;
    if ((*list)->numItems == 0)
        return;
#endif

    if (firstItemPosition == FRONT_OF_LIST)
        firstItemPosition = 1;
    else
    if (firstItemPosition == END_OF_LIST)
        firstItemPosition = (*list)->numItems;

#ifndef TB_NO_SAFETY_CHECKS
    if (firstItemPosition <= 0  ||
        firstItemPosition + numItemsToGet - 1 > (*list)->numItems)
        return;
#endif

    memcpy (itemsDestination, ITEMPTR(list,firstItemPosition-1),
        (*list)->itemSize * numItemsToGet);
}

/*******************************/

    /*  Returns a pointer to the item at itemPosition. returns null if an errors occurred.
    */
void * CVIFUNC ListGetPtrToItem (ListType list, int itemPosition)
{
#ifndef TB_NO_SAFETY_CHECKS
    if (!list)
        return NULL;
    if ((*list)->numItems == 0)
        return NULL;
#endif

    if (itemPosition == FRONT_OF_LIST)
        itemPosition = 1;
    else
    if (itemPosition == END_OF_LIST)
        itemPosition = (*list)->numItems;

#ifndef TB_NO_SAFETY_CHECKS
    if (itemPosition <= 0 || itemPosition > (*list)->numItems)
        return NULL;
#endif

    return ITEMPTR(list,itemPosition-1);
}

/*******************************/

     /* returns a pointer the lists data (abstraction violation for optimization) */
void * CVIFUNC ListGetDataPtr (ListType list)
{
#ifndef TB_NO_SAFETY_CHECKS
    if (!list)
        return NULL;
#endif

    return &((*list)->itemList[0]);
}

/********************************/

int CVIFUNC ListApplyToEach (ListType list, int ascending, ListApplicationFunc funcToApply, void *callbackData)
{
    int result = 0, index;

    if (!list || !funcToApply)
        goto Error;

    if (ascending)
        {
        for (index = 1; index <= ListNumItems (list); index++)
            {
            result  = funcToApply (index, ListGetPtrToItem (list, index),
                callbackData);
            if (result < 0)
                goto Error;
            }
        }
    else
        {
        for (index = ListNumItems (list); index > 0 &&
            index <= ListNumItems (list); index--)
            {
            result  = funcToApply (index, ListGetPtrToItem (list, index),
                callbackData);
            if (result < 0)
                goto Error;
            }
        }

Error:
    return result;
}

/********************************/

int CVIFUNC ListGetItemSize (ListType list)
{
#ifndef TB_NO_SAFETY_CHECKS
    if (!list)
        return 0;
#endif

    return (*list)->itemSize;
}

/********************************/

int CVIFUNC ListNumItems (ListType list)
{
#ifndef TB_NO_SAFETY_CHECKS
    if (!list)
        return 0;
#endif

    return (*list)->numItems;
}

/*******************************/

void CVIFUNC ListRemoveDuplicates (ListType list, CompareFunction compareFunction)
{
    int numItems, index, startIndexForFind, duplicatesIndex;

    numItems = ListNumItems (list);

    for (index = 1; index < numItems; index++)
        {
        startIndexForFind = index + 1;
        while (startIndexForFind <= numItems)
            {
            duplicatesIndex = ListFindItem (list, ListGetPtrToItem (list, index),
                startIndexForFind, compareFunction);
            if (duplicatesIndex > 0)
                {
                ListRemoveItem (list, NULL, duplicatesIndex);
                numItems--;
                startIndexForFind = duplicatesIndex;
                }
             else
                break;
            }
        }
}

/*******************************/

int CVIFUNC ListFindItem (ListType list, const void *ptrToItem, int startingPosition, CompareFunction compareFunction)
{
    int numItems, size, index, cmp;
    void *listItemPtr;

#ifndef TB_NO_SAFETY_CHECKS
    if (!list || !ptrToItem)
        return 0;
#endif

    if ((numItems = (*list)->numItems) == 0)
        return 0;

    size = (*list)->itemSize;

    if (startingPosition == FRONT_OF_LIST)
        startingPosition = 1;
    else
    if (startingPosition == END_OF_LIST)
        startingPosition = numItems;

#ifndef TB_NO_SAFETY_CHECKS
    if (startingPosition <= 0 || startingPosition > (*list)->numItems)
        return 0;
#endif

    for (index = startingPosition; index <= numItems; index++)
        {
        listItemPtr = ITEMPTR(list,index-1);
        cmp = compareFunction ? compareFunction((void *)ptrToItem, listItemPtr)
                              : ListMemBlockCmp((void *)ptrToItem, listItemPtr, size);
        if (cmp == 0)
            return index;
        }

    return 0;
}

/*******************************/

int CVICALLBACK ShortCompare(void *a, void *b)
{
    if (*(short *)a < *(short *)b) return -1;
    if (*(short *)a > *(short *)b) return  1;
    return 0;
}

/*******************************/

int CVICALLBACK IntCompare(void *a, void *b)
{
   if (*(int *)a < *(int *)b) return -1;
   if (*(int *)a > *(int *)b) return  1;
   return 0;
}

/*******************************/

int CVICALLBACK FloatCompare(void *a, void *b)
{
   if (*(float *)a < *(float *)b) return -1;
   if (*(float *)a > *(float *)b) return  1;
   return 0;
}

/*******************************/

int CVICALLBACK DoubleCompare(void *a, void *b)
{
   if (*(double *)a < *(double *)b) return -1;
   if (*(double *)a > *(double *)b) return  1;
   return 0;
}

/*******************************/

int CVICALLBACK CStringCompare(void *a, void *b)
{
    return strcmp(*(char **)a, *(char **)b);
}

/*******************************/

int CVICALLBACK CStringNoCaseCompare(void *a, void *b)
{
    return stricmp(*(char **)a, *(char **)b);
}

/*******************************/

    /*  Used to compare list elements by their raw data contents */
static int CVICALLBACK ListMemBlockCmp(void *a, void *b, int size)
{
    return memcmp(a, b, size);
}

/*******************************/

int CVIFUNC ListBinSearch (ListType list, const void *ptrToItem, CompareFunction compareFunction)
{
    int index;

#ifndef TB_NO_SAFETY_CHECKS
    if (!list || !ptrToItem)
        return 0;
#endif

    index = BinSearch (ITEMPTR(list,0), (int)(*list)->numItems,
        (int)(*list)->itemSize, ptrToItem, compareFunction);

    if (index >= 0)
        index++;        /* lists start from 1 */
    else
        index = 0;      /* item not found */

    return index;
}

/*******************************/

int CVIFUNC ListIsSorted (ListType list, CompareFunction compareFunction)
{
    int numItems, index;
    int cmp, size;
    void *item1Ptr, *item2Ptr;

#ifndef TB_NO_SAFETY_CHECKS
    if (!list)
        return FALSE;
#endif

    if ((numItems = (*list)->numItems) < 2)
        return TRUE;

    size = (*list)->itemSize;

    item2Ptr = ITEMPTR(list,0);   /* prime it */
    for (index = 1; index <= numItems-1; index++)
        {
        item1Ptr = item2Ptr;   /* ITEMPTR(list,index-1) */
        item2Ptr = ITEMPTR(list,index);
        cmp = compareFunction ? compareFunction(item1Ptr, item2Ptr)
                              : ListMemBlockCmp(item1Ptr, item2Ptr, size);
        if (cmp > 0)
            return FALSE;
        }

    return TRUE;
}

/**********************************/

void CVIFUNC ListQuickSort (ListType list, CompareFunction compareFunction)
{

#ifndef TB_NO_SAFETY_CHECKS
    if (!list)
        return;
#endif

    if ((*list)->numItems == 0)
        return;

    QSort (ITEMPTR(list,0), (*list)->numItems, (*list)->itemSize,
           compareFunction);      /* assumes CVICALLBACK and CVIANSI are the same */
}

/*******************************/

void CVIFUNC ListHeapSort (ListType list, CompareFunction compareFunction)
{
#ifndef TB_NO_SAFETY_CHECKS
    if (!list)
        return;
#endif

    if ((*list)->numItems == 0)
        return;

    HeapSort (ITEMPTR(list,0), (*list)->numItems, (*list)->itemSize,
        compareFunction);
}

/*******************************/

void CVIFUNC ListInsertionSort (ListType list, CompareFunction compareFunction)
{
#ifndef TB_NO_SAFETY_CHECKS
    if (!list)
        return;
#endif

    if ((*list)->numItems == 0)
        return;

    InsertionSort (ITEMPTR(list,0), (*list)->numItems, (*list)->itemSize,
        compareFunction);
}

/*******************************/

    /*  returns TRUE if insertion is successful, returns FALSE if out of memory or
        if bad arguments are passed.
    */
int CVIFUNC ListInsertInOrder (ListType list, const void *ptrToItem, CompareFunction compareFunction)
{
    int numItems, InsertPosition;

#ifndef TB_NO_SAFETY_CHECKS
    if (!list || !ptrToItem)
        return FALSE;
#endif

    numItems = ListNumItems (list);

    if (numItems == 0)
        InsertPosition = FRONT_OF_LIST;
    else
        {
        InsertPosition = BinSearch (ITEMPTR(list,0), (int)(*list)->numItems,
            (int)(*list)->itemSize, ptrToItem, compareFunction);
        if (InsertPosition >= 0)
            InsertPosition++;           /* found matching item, list indices start from 1 */
        else
            {
            InsertPosition = -(InsertPosition+1);   /* BinSearch returns insertion point i in the form -i-1, convert to i: */
            InsertPosition++;           /* list indices start from 1 */

            if (InsertPosition > numItems)
                InsertPosition = END_OF_LIST;
            }
        }

    return ListInsertItem (list, ptrToItem, InsertPosition);
}

/**************************************************************************/

    /*  Reserves memory for numItems in the list. If it succeeds then
        numItems items can be inserted without possibility of an
        out of memory error (useful to simplify error recovery in
        complex functions). Returns TRUE if success, FALSE if
        out of memory.
    */
int CVIFUNC ListPreAllocate (ListType list, int numItems)
{
#ifndef TB_NO_SAFETY_CHECKS
    if (!list)
        return FALSE;
#endif

    if ((*list)->listSize - (*list)->numItems < numItems)
        return ExpandListSpace (list,
            numItems - ((*list)->listSize - (*list)->numItems));
    else
        return TRUE;    /* enough items are already pre-allocated */
}

/**************************************************************************/

int CVICALLBACK OutputIntegerItem(FILE *stream, void *ptrToItem)
{
    return fprintf(stream, "%d", *(int *)ptrToItem);
}

/**************************************************************************/

int CVICALLBACK OutputDoubleItem(FILE *stream, void *ptrToItem)
{
    return fprintf(stream, "%f", *(double *)ptrToItem);
}

/**************************************************************************/

int CVICALLBACK OutputShortItem(FILE *stream, void *ptrToItem)
{
    return fprintf(stream, "%d", (int)*(short int *)ptrToItem);
}

/**************************************************************************/

int CVICALLBACK OutputStringItem(FILE *stream, void *ptrToItem)
{
    return fprintf(stream, "%s", *(char **)ptrToItem);
}

/**************************************************************************/

int CVIFUNC OutputList (ListType list, FILE *stream, const char *listPrefix, const char *listSuffix, const char *itemPrefix, const char *itemSuffix, ItemOutputFunction itemOutputFunction)
{
    int index, numItems, totalCharsWritten = 0, charsWritten;

    if (!stream)
        stream = stdout;

    if (!itemOutputFunction)
        return fprintf (stream, "Error, no output function specified\n");

    if (listPrefix)
        if ((charsWritten = fprintf (stream, listPrefix)) < 0)
            return charsWritten;
        else
            totalCharsWritten += charsWritten;

    numItems = ListNumItems (list);

    for (index = 1; index <= numItems; index++) {
        if (itemPrefix)
            if (CmbStrStr ((const unsigned char *)itemPrefix, (const unsigned char *)"%d"))
                if ((charsWritten = fprintf (stream, itemPrefix, index)) < 0)
                    return charsWritten;
                else
                    totalCharsWritten += charsWritten;
            else
                if ((charsWritten = fprintf(stream, itemPrefix)) < 0)
                    return charsWritten;
                else
                    totalCharsWritten += charsWritten;

        if ((charsWritten = itemOutputFunction (stream,
            ListGetPtrToItem (list, index))) < 0)
            return charsWritten;
        else
            totalCharsWritten += charsWritten;

        if (itemSuffix && index != numItems)
            if (CmbStrStr ((const unsigned char *)itemSuffix, (const unsigned char *)"%d"))
                if ((charsWritten = fprintf (stream, itemSuffix, index)) < 0)
                    return charsWritten;
                else
                    totalCharsWritten += charsWritten;
            else
                if ((charsWritten = fprintf (stream, itemSuffix)) < 0)
                    return charsWritten;
                else
                    totalCharsWritten += charsWritten;
        }

    if (listSuffix)
        if (CmbStrStr ((const unsigned char *)listSuffix, (const unsigned char *)"%d"))
            if ((charsWritten = fprintf (stream, listSuffix, numItems)) < 0)
                return charsWritten;
            else
                totalCharsWritten += charsWritten;
        else
            if ((charsWritten = fprintf (stream, listSuffix)) < 0)
                return charsWritten;
            else
                totalCharsWritten += charsWritten;

    return totalCharsWritten;
}

/**************************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC_C AttributeMaxAndMin(int panel, int attribute, int numCtrls, int *max,
                int *ctrlWithMax, int *min, int *ctrlWithMin, ...)
{
    va_list     parmInfo;
    int         error = UIENoError;
    ListType    ctrlList = 0;

    va_start(parmInfo, ctrlWithMin);
    errChk( GetIntListFromParmInfo(parmInfo, numCtrls, &ctrlList));

    errChk( AttrMaxAndMinForList(panel, ctrlList, attribute, max,
                ctrlWithMax, min, ctrlWithMin));

Error:
    ListDispose(ctrlList);
    va_end(parmInfo);
    if (error >= 0)
        return 0;
    else
        return error;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

static int CVIFUNC IsTBBoundsAttr(int attribute)
{
    switch (attribute)
        {
        case ATTR_TB_BOUNDS_TOP:
        case ATTR_TB_BOUNDS_LEFT:
        case ATTR_TB_BOUNDS_HEIGHT:
        case ATTR_TB_BOUNDS_WIDTH:
            return TRUE;
        default:
            return FALSE;
        }
}

/**************************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC AttrMaxAndMinForList(int panel, ListType ctrlList, int attribute, int *max,
                int *ctrlWithMax, int *min, int *ctrlWithMin)
{
    int error = UIENoError;
    int value;
    int ctrl;
    int numCtrls;
    int index;
    int maximum = 0, minimum = 0;
    int maxCtrl = 0, minCtrl = 0;
    
    numCtrls = ListNumItems(ctrlList);
    for (index = 1; index <= numCtrls; index++)
        {
        ListGetItem(ctrlList, &ctrl, index);

        if (IsTBBoundsAttr(attribute))
            errChk( GetCtrlBoundRectAttribute(panel, ctrl, attribute, &value));
        else
            errChk( GetCtrlAttribute(panel, ctrl, attribute, &value));

        if (index == 1)
            {
            maximum = value;
            minimum = value;
            maxCtrl = ctrl;
            minCtrl = ctrl;
            }
        else
            {
            if (value > maximum)
                {
                maximum = value;
                maxCtrl = ctrl;
                }

            if (value < minimum)
                {
                minimum = value;
                minCtrl = ctrl;
                }
            }
        }

    if (max)
        *max = maximum;

    if (min)
        *min = minimum;

    if (ctrlWithMax)
        *ctrlWithMax = maxCtrl;

    if (ctrlWithMin)
        *ctrlWithMin = minCtrl;

Error:
    if (error >= 0)
        return 0;
    else
        return error;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
void CVIFUNC LocalizeNumberString (char *str)
{
    char    decimalSymbol;

    GetSystemAttribute (ATTR_LOCALIZED_DECIMAL_SYMBOL, &decimalSymbol);
    if (decimalSymbol != '.')
        {
        for (; *str; CmbStrInc((const unsigned char *)str))
            if (*str == '.')
                *str = decimalSymbol;
        }
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
void CVIFUNC DelocalizeNumberString (char *str)
{
    char    decimalSymbol;

    GetSystemAttribute (ATTR_LOCALIZED_DECIMAL_SYMBOL, &decimalSymbol);
    if (decimalSymbol != '.')
        {
        for (; *str; CmbStrInc((const unsigned char *)str))
            if (*str == decimalSymbol)
                *str = '.';
        }
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
static int GetToolbarHelpBGColor (void)
{
    static int colorInit = FALSE, gColor;

#if _NI_mswin32_
    if (!colorInit)
    {
        gColor = WinColorToCVIColor (GetSysColor (COLOR_INFOBK));
        colorInit = TRUE;
    }

    return gColor;
#else
    return kToolTipHelpPanelColor;
#endif
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
static int GetToolbarHelpTextColor (void)
{
    static int colorInit = FALSE, gColor;

#if _NI_mswin32_
    if (!colorInit)
    {
        gColor = WinColorToCVIColor (GetSysColor (COLOR_INFOTEXT));
        colorInit = TRUE;
    }

    return gColor;
#else
    return kToolTipHelpTextColor;
#endif
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
#if _NI_mswin32_
static int WinColorToCVIColor(int c)
{
    unsigned char *colorPtr = (unsigned char *)&c;
    unsigned char blue;

    blue = colorPtr[0];
    colorPtr[0] = colorPtr[2];
    colorPtr[2] = blue;
    colorPtr[3] = 0x00;

    return (int)c;
}
#endif
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
static int TopMostParent(int panel, int *topMostParent)
{
    int error = 0;
    int thisParent;

    errChk( GetPanelAttribute(panel, ATTR_PANEL_PARENT, &thisParent));

    if (thisParent)
        return TopMostParent(thisParent, topMostParent);
    else
        *topMostParent = panel;
Error:
    return error;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
    /* Creates a new rectangle by forcing rectToPin to be within rect.  The new rectangle is  */
    /* store in pinnedRect.  If rectToPin is larger than rect, it is pinned to the top and/or */
    /* left of rect but its size is not reduced                                               */
static void CVIFUNC RectPinnedToRect(Rect rect, Rect rectToPin, Rect *pinnedRect)
{
    if (rect.width < rectToPin.width || rect.left > rectToPin.left)
        rectToPin.left = rect.left;
    else
    if (RectRight(rect) < RectRight(rectToPin))
        rectToPin.left = RectRight(rect) - rectToPin.width;

    if (rect.height < rectToPin.height || rect.top > rectToPin.top)
        rectToPin.top = rect.top;
    else
    if (RectBottom(rect) < RectBottom(rectToPin))
        rectToPin.top = RectBottom(rect) - rectToPin.height;

    *pinnedRect = rectToPin;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
static int PointConvertToGlobal(int childPanel, Point *point)
{
    int error = 0, top, left, topOffset, leftOffset;
    int thisParent;

    GetPanelAttribute(childPanel, ATTR_TOP, &top);
    GetPanelAttribute (childPanel, ATTR_VSCROLL_OFFSET, &topOffset);
    GetPanelAttribute(childPanel, ATTR_LEFT, &left);
    GetPanelAttribute (childPanel, ATTR_HSCROLL_OFFSET, &leftOffset);
    
    point->x += left - leftOffset;
    point->y += top - topOffset;

    errChk( GetPanelAttribute(childPanel, ATTR_PANEL_PARENT, &thisParent));

    if (thisParent)
        return PointConvertToGlobal(thisParent, point);

Error:
    return error;
}


static int PointToPanel (int panel, Point point)
{
    ListType    sortedChildren;
    Rect        bounds;
    int         i, numPanels, child, visible, titlebarThickness, frameThickness;

    sortedChildren = ListCreate (sizeof (int));
    GetPanelAttribute (panel, ATTR_NUM_CHILDREN, &numPanels);
    if (numPanels > 0)
    {
        GetPanelAttribute (panel, ATTR_FIRST_CHILD, &child);
        ListInsertItem (sortedChildren, &child, END_OF_LIST);
    }
    for (i = 1; i < numPanels; i++)
    {
        GetPanelAttribute (child, ATTR_NEXT_PANEL, &child);
        ListInsertItem (sortedChildren, &child, END_OF_LIST);
    }
    ListQuickSort (sortedChildren, ComparePanelZPlaneOrder);
    
    for (i = 0; i < numPanels; i++)
    {
        ListGetItem (sortedChildren, &child, i + 1);
        
        GetPanelAttribute (child, ATTR_VISIBLE, &visible);
        if (!visible)
            continue;
        
        GetPanelAttribute (child, ATTR_LEFT, &bounds.left);
        GetPanelAttribute (child, ATTR_TOP, &bounds.top);
        GetPanelAttribute (child, ATTR_WIDTH, &bounds.width);
        GetPanelAttribute (child, ATTR_HEIGHT, &bounds.height);

        GetPanelAttribute (child, ATTR_TITLEBAR_THICKNESS, &titlebarThickness);
        bounds.top -= titlebarThickness;
        bounds.height += titlebarThickness;
        
        GetPanelAttribute (child, ATTR_FRAME_THICKNESS, &frameThickness);
        RectGrow (&bounds, frameThickness, frameThickness);
        
        if (RectContainsPoint (bounds, point))
        {
            PointConvertCoordinates (panel, child, &point, FALSE);
            return PointToPanel (child, point);
        }
    }

    ListDispose (sortedChildren);
    return panel;
}

#endif  // HAVE_CVI_RTE


#if HAVE_CVI_RTE
    /* Converts a point from child to parent coordinates or vice versa.  The child panel does not need to be */
    /* an immediate child of topPanel.                                                                       */
static int CVIFUNC PointConvertCoordinates(int topPanel, int childPanel, Point *point, int convertFromChildCoordinates)
{
    int error = 0;
    int parent;
    int top, left;

    if (topPanel == childPanel)
        return error;

    if (childPanel == 0)    /* not a child of topPanel */
        errChk(UIEValueIsInvalidOrOutOfRange);

    if (convertFromChildCoordinates)
        {
        GetPanelAttribute(childPanel, ATTR_PANEL_PARENT, &parent);
        GetPanelAttribute(childPanel, ATTR_TOP, &top);
        GetPanelAttribute(childPanel, ATTR_LEFT, &left);
        point->x += left;
        point->y += top;

        errChk( PointConvertCoordinates(topPanel, parent, point, convertFromChildCoordinates));
        }
    else
        {
        Point   offset = {0,0};

        errChk( PointConvertCoordinates(topPanel, childPanel, &offset, TRUE));

        point->x -= offset.x;
        point->y -= offset.y;
        }

Error:
    return error;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC SetCtrlToolTipAttribute(int panel, int ctrl, int attribute, ...)
{
    int             error = 0;
    va_list         parmInfo;
    CtrlToolTip *   tip = NULL;
    char *          toolTipText;
    int             toolTipVisible;
    int             discardToolTip = FALSE;

/*  GetLock(kToolBoxFiniteLock);  /* lock when entering via public function */
    va_start(parmInfo, attribute);

    switch (attribute)
        {
        case CTRL_TOOLTIP_ATTR_TEXT:
            toolTipText = va_arg(parmInfo, char *);

            if (toolTipText == NULL || *toolTipText == 0)
                discardToolTip = TRUE;
                
            if (GetChainedCallbackData (panel, ctrl, kCtrlToolTipId, (void **)&tip) < 0)
                {
                if (!discardToolTip)
                    tip = CtrlToolTip_Create(panel, ctrl); /* if the control is not a ToolTip control, make it one */
                else
                    tip = NULL;
                }
                
            if (tip == NULL)
                goto Error;
            if (discardToolTip)
                CtrlToolTip_Dispose(tip);
            else if (strcmp(tip->tipText, toolTipText) != 0) /* has the tip changed? */
                {
                free(tip->tipText);
                nullChk( tip->tipText = StrDup(toolTipText));

                    /* need to rediplay tip with new text? */
                if (SharedTipPanel_GetActiveTip(tip->tipPanel) == tip)
                    {
                    CtrlToolTip_Show(tip);
                    CtrlToolTip_Show(tip);
                    }
                }
            break;
        case CTRL_TOOLTIP_ATTR_ENABLE:
            toolTipVisible = va_arg(parmInfo, int);
                
            if (GetChainedCallbackData (panel, ctrl, kCtrlToolTipId, (void **)&tip) < 0)
                goto Error;
            
            tip->visible = toolTipVisible;
            break;
        default:
            errChk(UIEInvalidAttribute);
        }

Error:
    va_end(parmInfo);
/*  ReleaseLock(kToolBoxFiniteLock); */

    return error;
}

int CVIFUNC GetCtrlToolTipAttribute(int panel, int ctrl, int attribute, void *value)
{
    int             error = 0;
    CtrlToolTip *   tip = NULL;

/*  GetLock(kToolBoxFiniteLock);  /* lock when entering via public function */

    switch (attribute)
        {
        case CTRL_TOOLTIP_ATTR_TEXT:
            if ((error = GetChainedCallbackData (panel, ctrl, kCtrlToolTipId, (void **)&tip)) < 0)
                goto Error;
            strcpy ((char *)value, tip->tipText);
            break;
        case CTRL_TOOLTIP_ATTR_ENABLE:
            if ((error = GetChainedCallbackData (panel, ctrl, kCtrlToolTipId, (void **)&tip)) < 0)
                goto Error;
            *(int *)value = tip->visible;
            break;
        default:
            errChk(UIEInvalidAttribute);
        }

Error:
/*  ReleaseLock(kToolBoxFiniteLock); */

    return error;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
static int CtrlToolTip_ShouldHide(CtrlToolTip *tip, int *shouldHide)
{
    int     error = 0;
    int     topMostPanelActive;
    int     topMostPanelVisible;
    int     panelVisible;
    int     ctrlVisible;
    int     ctrlDisabled;
    int     panelDisabled;
    extern int CVIFUNC_C MenuIsOpen (void); // Hidden UI Library function -- returns true if there is currently a menu open.  
    
    errChk( GetPanelAttribute(tip->tipPanel->topMostPanel, ATTR_VISIBLE, &topMostPanelVisible));
    errChk( GetPanelAttribute(tip->panel, ATTR_VISIBLE, &panelVisible));
    errChk( GetPanelAttribute(tip->panel, ATTR_DIMMED, &panelDisabled));
    errChk( GetPanelAttribute(tip->tipPanel->topMostPanel, ATTR_ACTIVE, &topMostPanelActive));
    errChk( GetCtrlAttribute(tip->panel, tip->ctrl, ATTR_DIMMED, &ctrlDisabled));
    errChk( GetCtrlAttribute(tip->panel, tip->ctrl, ATTR_VISIBLE, &ctrlVisible));
    
    *shouldHide = !*tip->tipText || !tip->visible || MenuIsOpen() || !topMostPanelActive
                || !topMostPanelVisible || !panelVisible || !ctrlVisible
                || ctrlDisabled || panelDisabled;

Error:
    return error;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
static int CtrlToolTip_Show(CtrlToolTip *tip)
{
    int     error = 0;
    int     shouldHide;

    errChk( CtrlToolTip_ShouldHide(tip, &shouldHide));

    if (shouldHide)
        errChk( CtrlToolTip_Hide(tip));
    else
    if (SharedTipPanel_GetActiveTip(tip->tipPanel) != tip)
        errChk( SharedTipPanel_ShowTip(tip->tipPanel, tip));

Error:
    return error;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
static int CtrlToolTip_Hide(CtrlToolTip *tip)
{
    int error = 0;

    if (SharedTipPanel_GetActiveTip(tip->tipPanel) == tip)
        errChk( SharedTipPanel_HideTip(tip->tipPanel));

Error:
    return error;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
    /* the new CtrlToolTip is owned by the control */
static CtrlToolTip * CtrlToolTip_Create(int panel, int ctrl)
{
    int                 error = 0;
    CtrlToolTip *       newTip = NULL;
    SharedTipPanel *    tipPanel;

    errChk( SharedTipPanel_GetTipPanel(panel, &tipPanel));

    nullChk (newTip = calloc(sizeof(CtrlToolTip), 1));
    newTip->panel = panel;
    newTip->ctrl = ctrl;
    newTip->state = kToolTip_MouseOut;
    newTip->tipPanel = tipPanel;
    newTip->visible = TRUE;

    nullChk( newTip->tipText = StrDup(""));

    errChk( SharedTipPanel_InstallTip(tipPanel, newTip));

    errChk( ChainCtrlCallback (panel, ctrl, CtrlToolTip_CtrlCallback, newTip, kCtrlToolTipId));

Error:
    if (error < 0)
        {
        CtrlToolTip_Dispose(newTip);
        newTip = NULL;
        }

    return newTip;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
static void CtrlToolTip_Dispose(CtrlToolTip *tip)
{
    int numTips;
    
    if (tip)
        {
        if (tip->tipPanel)
            SharedTipPanel_RemoveTip(tip->tipPanel, tip);

        numTips = ListNumItems(tip->tipPanel->tipList);
        if (0 == numTips && !tip->tipPanel->disposingPanel)
            SharedTipPanel_Dispose(tip->tipPanel); /* if disposing of last one, then dispose the shared tip too */

        free(tip->tipText);
        free(tip);
        }
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
static int CVICALLBACK CtrlToolTip_CtrlCallback (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
    {
    CtrlToolTip *tip = (CtrlToolTip *)callbackData;

    switch (event)
        {
        case EVENT_VAL_CHANGED:
        case EVENT_KEYPRESS:
        case EVENT_LEFT_CLICK:
        case EVENT_RIGHT_CLICK:
/*  GetLock(kToolBoxFiniteLock); /* lock when entering via callback */
            CtrlToolTip_Hide(tip);
/*  ReleaseLock(kToolBoxFiniteLock); */
            break;
        case EVENT_DISCARD:
/*  GetLock(kToolBoxFiniteLock); /* lock when entering via callback */
            CtrlToolTip_Dispose(tip);
/*  ReleaseLock(kToolBoxFiniteLock); */
            break;
        }

    return 0;
    }
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
static int CVICALLBACK CtrlToolTip_CheckTip (CtrlToolTip *tip)
    {
    int             error = 0;
    int             x,y;
    Rect            ctrlBounds;
    Point           coord, testCoord;
    int             visible;
    CtrlToolTip     *activeTip;
    int             shouldHide = FALSE;
    int             coordPanel, testPanel;

    activeTip = SharedTipPanel_GetActiveTip(tip->tipPanel);

    if (activeTip == tip)
        errChk( CtrlToolTip_ShouldHide(tip, &shouldHide));

    if (shouldHide)
        errChk( CtrlToolTip_Hide(tip));
    else
        {
        errChk( GetPanelAttribute(tip->panel, ATTR_VISIBLE, &visible));

        if (visible) /* can't call GetRelativeMouseState on invisible panel */
            {
            errChk( GetRelativeMouseState (tip->panel, 0, &x, &y, NULL, NULL, NULL));
            errChk( GetCtrlBoundingRect (tip->panel, tip->ctrl, &ctrlBounds.top, &ctrlBounds.left, &ctrlBounds.height, &ctrlBounds.width));

            coord = MakePoint (x, y);
            
            testPanel = tip->tipPanel->topMostPanel;
            testCoord = coord;
            PointConvertCoordinates (testPanel, tip->panel, &testCoord, TRUE);
            coordPanel = PointToPanel (testPanel, testCoord);
            
            if (coordPanel == tip->panel && RectContainsPoint (ctrlBounds, coord))
                {
                if (tip->state == kToolTip_MouseOut)
                    tip->state = kToolTip_MouseIn;
                else
                if (tip->state == kToolTip_MouseIn)
                    {
                    if (ABS_VAL(tip->lastMousePoint.x - x) < kToolTipHoverDistance && ABS_VAL(tip->lastMousePoint.y - y) < kToolTipHoverDistance)
                        {
                        tip->lastMousePoint = MakePoint(x,y);
                        errChk( CtrlToolTip_Show(tip));
                        tip->state = kToolTip_Shown;    /* remember we showed the tip */
                        }
                    }

                tip->lastMousePoint = MakePoint(x,y);
                }
            else
                {
                tip->state = kToolTip_MouseOut;  /* reset state so that moving the mouse back in will display the tip again */
                if (activeTip == tip)
                    errChk( CtrlToolTip_Hide(tip));
                }
            }
        }

Error:
    return error;
    }
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
    /* The shared tooltip data is created and attached to the topMostPanel */
static int SharedTipPanel_Create(int topMostPanel, SharedTipPanel **tipPanel)
{
    int error = 0;

    *tipPanel = NULL;

        /* allocate the SharedTipPanel data structure */
    nullChk( *tipPanel = (SharedTipPanel *)calloc(sizeof(SharedTipPanel), 1));
    (*tipPanel)->activeTip = NULL; // be explicit
    (*tipPanel)->topMostPanel = topMostPanel;

        /* create a list of CtrlToolTip's.  This list will be examined on each timer tick to see if a tip should be shown or hidden */
    nullChk( (*tipPanel)->tipList = ListCreate(sizeof(CtrlToolTip *)));

        /* create the floating tip panel that is shared by all tipCtrls on the topMostPanel and any of its child panels */
    errChk( (*tipPanel)->panel = NewPanel (0, "", -10000, -10000, 10, 10));
    errChk( SetPanelAttribute ((*tipPanel)->panel, ATTR_FLOATING, VAL_FLOAT_APP_ACTIVE));
    errChk( SetPanelAttribute((*tipPanel)->panel, ATTR_TITLEBAR_VISIBLE, FALSE));
    errChk( SetPanelAttribute((*tipPanel)->panel, ATTR_CONFORM_TO_SYSTEM, TRUE));
    errChk( SetPanelAttribute((*tipPanel)->panel, ATTR_BACKCOLOR, GetToolbarHelpBGColor()));
    errChk( SetPanelAttribute((*tipPanel)->panel, ATTR_ACTIVATE_WHEN_CLICKED_ON, FALSE));
    errChk( SetPanelAttribute((*tipPanel)->panel, ATTR_SIZABLE, FALSE));

        /* create the textMsg ctrl that displays the tip */
    errChk( (*tipPanel)->textMsg = NewCtrl ((*tipPanel)->panel, CTRL_TEXT_MSG, "", 0, 0));
    errChk( SetCtrlAttribute((*tipPanel)->panel, (*tipPanel)->textMsg, ATTR_TEXT_BGCOLOR, VAL_TRANSPARENT));
    errChk( SetCtrlAttribute((*tipPanel)->panel, (*tipPanel)->textMsg, ATTR_TEXT_COLOR, GetToolbarHelpTextColor()));

        /* create a timer control to periodically check if tips should be shown or hidden */
    errChk( (*tipPanel)->timer = NewCtrl ((*tipPanel)->panel, CTRL_TIMER, "", -10000, -10000));
    errChk( SetCtrlAttribute((*tipPanel)->panel, (*tipPanel)->timer, ATTR_INTERVAL, kDefaultToolTipDelay / 2.0)); /* divide by two because the mouse must hover for two ticks before we show the tip */
    errChk( ChainCtrlCallback((*tipPanel)->panel, (*tipPanel)->timer, SharedTipPanel_TimerCallback, *tipPanel, kSharedTipPanelId));

        /* hook the topMostPanel callback to destroy the tipPanel structure when the panel is destroyed */
    errChk( ChainPanelCallback(topMostPanel, SharedTipPanel_OwnerPanelCallback, *tipPanel, kSharedTipPanelId));

Error:
    if (error < 0)
        {
        SharedTipPanel_Dispose(*tipPanel);
        *tipPanel = NULL;
        }

    return error;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
    /* Free the SharedTipPanel structure */
static void SharedTipPanel_Dispose(SharedTipPanel *tipPanel)
{
    if (tipPanel)
        {
        int savedState = SetBOLE(FALSE);    // in case the floating tip panel is already discarded because its parent was discarded
        int numItems, index;
        CtrlToolTip **tip = 0;

        tipPanel->disposingPanel = TRUE;
        
        // Discard tips
        numItems = ListNumItems(tipPanel->tipList);
        for (index = numItems; index > 0; index--)
        {
            if ((tip = (CtrlToolTip **) ListGetPtrToItem (tipPanel->tipList, index)) != NULL)
                CtrlToolTip_Dispose(*tip);
        }

        ListDispose(tipPanel->tipList);     // the list items are deleted by the controls that own them
        
        if (tipPanel->panel > 0)
            DiscardPanel(tipPanel->panel);
            
        UnchainPanelCallback(tipPanel->topMostPanel, kSharedTipPanelId);
        SetBOLE(savedState);
        
        // The tip panel is a child panel and is discarded automatically
        free(tipPanel);
        }
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
    /* When the topmost panel that owns the SharedTipPanel structure is discarded, then dispose the SharedTipPanel structure */
static int CVICALLBACK SharedTipPanel_OwnerPanelCallback (int panelHandle, int event, void *callbackData, int eventData1, int eventData2)
{
    SharedTipPanel  *tipPanel = (SharedTipPanel *)callbackData;

    switch (event)
        {
        case EVENT_DISCARD:
            /*  GetLock(kToolBoxFiniteLock); /* lock when entering via callback */
            SharedTipPanel_Dispose(tipPanel);
            /*  ReleaseLock(kToolBoxFiniteLock); */
            break;
        }

    return 0;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
    /* If there is a visible tip, hide it. */
static int SharedTipPanel_HideTip(SharedTipPanel *tipPanel)
{
    int error = 0;

    if (tipPanel->activeTip)
        {
        int savedState = SetBOLE(FALSE); /* if the top level panel is discarded, the floating tip panel might be deleted before the control with the displayed tip is deleted */
        SetPanelAttribute(tipPanel->panel, ATTR_VISIBLE, FALSE);
        tipPanel->activeTip = 0;
        SetBOLE(savedState);
        }

/*Error:*/
    return error;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
    /* Show the floating tip panel with the tip text of the specific tip.  Place the */
    /* panel under where the mouse hovered over the control for the tip              */
static int SharedTipPanel_ShowTip(SharedTipPanel *tipPanel, CtrlToolTip *tip)
{
    int     error = 0;
    int     height;
    int     width;
    Rect    tipRect, screenRect;
    Point   mousePoint;
    int     singleLineHeight;
    int     activePanel;
    
        // hide the floating tip panel so that the changes aren't visible until we are done
    errChk( SetPanelAttribute(tipPanel->panel, ATTR_VISIBLE, FALSE));
    
        // figure out how tall a single line of text is. we'll move the tip down that much
        // to make sure it doesn't obscure any text the mouse might be hovering over
    errChk( SetCtrlVal(tipPanel->panel, tipPanel->textMsg, "SingleLine"));  
    errChk( GetCtrlAttribute(tipPanel->panel, tipPanel->textMsg, ATTR_HEIGHT, &singleLineHeight));
    
        // the text msg will resize to fit the text
    errChk( SetCtrlVal(tipPanel->panel, tipPanel->textMsg, tip->tipText));  
    
        // make the panel fit the text msg
    errChk( GetCtrlAttribute(tipPanel->panel, tipPanel->textMsg, ATTR_HEIGHT, &height));
    errChk( GetCtrlAttribute(tipPanel->panel, tipPanel->textMsg, ATTR_WIDTH, &width));
    errChk( SetPanelAttribute(tipPanel->panel, ATTR_HEIGHT, height));
    errChk( SetPanelAttribute(tipPanel->panel, ATTR_WIDTH, width));

        // get mouse location in screen coordinates
    mousePoint = tip->lastMousePoint;
    errChk( PointConvertToGlobal(tip->panel, &mousePoint));

        // place the tip centered under the mouse
    tipRect = MakeRect(0,0, height, width);
    tipRect.top = mousePoint.y + singleLineHeight;
    tipRect.left = mousePoint.x - tipRect.width / 2;
    
        // force it within the bounds of the screen
    GetScreenSize (&screenRect.height, &screenRect.width);
    screenRect.top = 0;
    screenRect.left = 0;
    RectPinnedToRect(screenRect, tipRect, &tipRect);
    
        // position the floating tip panel
    errChk( SetPanelAttribute (tipPanel->panel, ATTR_TOP, tipRect.top));
    errChk( SetPanelAttribute (tipPanel->panel, ATTR_LEFT, tipRect.left));
    
        // show the floating tip panel
    errChk( activePanel = GetActivePanel());
    errChk( SetPanelAttribute(tipPanel->panel, ATTR_VISIBLE, TRUE));
    errChk( SetActivePanel(activePanel));
    tipPanel->activeTip = tip;

Error:
    return error;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
    /* Add a tip to the list of tips for a panel */
static int SharedTipPanel_InstallTip(SharedTipPanel *tipPanel, CtrlToolTip *tip)
{
    int error = 0;
    int numItems;
    int index;

    if (tip)
        {
        numItems = ListNumItems(tipPanel->tipList);

        for (index = numItems; index > 0; index--)
            if (*(CtrlToolTip **)ListGetPtrToItem (tipPanel->tipList, index) == tip)
                goto Error; /* already installed (should never happen) */

        nullChk( ListInsertItem(tipPanel->tipList, &tip, END_OF_LIST));
        }

Error:
    return error;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
    /* Removes a tip from a tip panel's list of tips and hides the tip if it is displayed. */
    /* this function is called when the tip is disposed                                    */
static int SharedTipPanel_RemoveTip(SharedTipPanel *tipPanel, CtrlToolTip *tip)
{
    int error = 0;
    int numItems;
    int index;
    
    if (tip)
        {
        if (tipPanel->activeTip == tip)
            SharedTipPanel_HideTip(tipPanel);
            
        numItems = ListNumItems(tipPanel->tipList);
        
        for (index = numItems; index > 0; index--)
            if (*(CtrlToolTip **)ListGetPtrToItem (tipPanel->tipList, index) == tip)
                {
                UnchainCtrlCallback(tip->panel, tip->ctrl, kCtrlToolTipId);
                ListRemoveItem(tipPanel->tipList, NULL, index);
                break;
                }
        }       

//Error:
    return error;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
    /* get the shared tip panel.  If it does not exist, it is created and attached to the topmost panel */
static int SharedTipPanel_GetTipPanel(int panel, SharedTipPanel **tipPanel)
{
    int error = 0;
    int topMostPanel;

    *tipPanel = NULL;

    errChk( TopMostParent(panel, &topMostPanel));

    if (GetChainedPanelCallbackData (topMostPanel, kSharedTipPanelId, (void **)tipPanel) < 0)
        errChk( SharedTipPanel_Create(topMostPanel, tipPanel));

Error:
    return error;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
    /* The active tip is the tip that is currently visible.  If NULL, no tip is visible */
static CtrlToolTip * SharedTipPanel_GetActiveTip(SharedTipPanel *tipPanel)
{
    return tipPanel->activeTip;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
    /* For a topmost panel, check if the mouse position should cause the control tooltip to show or hide */
static int CVICALLBACK SharedTipPanel_TimerCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
    {
    int             error = 0;
    SharedTipPanel  *tipPanel = (SharedTipPanel *)callbackData;
    CtrlToolTip     *activeTip;
    int             numItems;
    int             index;
    int             topMostPanelVisible = FALSE;

    switch (event)
        {
        case EVENT_TIMER_TICK:
            numItems = ListNumItems(tipPanel->tipList);

            activeTip = SharedTipPanel_GetActiveTip(tipPanel);

            errChk( GetPanelAttribute(tipPanel->topMostPanel, ATTR_VISIBLE, &topMostPanelVisible));

            if (topMostPanelVisible || activeTip)   /* don't waste the effort if the topmost panel isn't visible */
                {
                /* GetLock(kToolBoxFiniteLock); */

                for (index = 1; index <= numItems; index++)
                    CtrlToolTip_CheckTip (*(CtrlToolTip **)ListGetPtrToItem(tipPanel->tipList, index));

                /* ReleaseLock(kToolBoxFiniteLock); */
                }
            break;
        }

Error:
    return 0;
    }
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC_C SetAttributeForCtrls(int panel, int attribute, int value, int numCtrls, ...)
{
    va_list     parmInfo;
    int         error = UIENoError;
    ListType    ctrlList = 0;

    va_start(parmInfo, numCtrls);
    errChk( GetIntListFromParmInfo(parmInfo, numCtrls, &ctrlList));

    errChk( SetAttributeForList(panel, ctrlList, attribute, value));

Error:
    ListDispose(ctrlList);
    va_end(parmInfo);
    if (error >= 0)
        return 0;
    else
        return error;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC SetAttributeForList(int panel, ListType ctrlList,int attribute, int value)
{
    int     error = UIENoError;
    int     ctrl;
    int     numCtrls;
    int     index;

    numCtrls = ListNumItems(ctrlList);
    for (index = 1; index <= numCtrls; index++)
        {
        ListGetItem(ctrlList, &ctrl, index);

        errChk( SetCtrlAttribute(panel, ctrl, attribute, value));
        }

Error:
    if (error >= 0)
        return 0;
    else
        return error;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC PutLabelOnLeft(int panel, int ctrl, int labelToCtrlGap)
{
    int error = UIENoError;
    int ctrlLeft;
    int labelWidth;

    errChk( GetCtrlAttribute(panel, ctrl, ATTR_LEFT, &ctrlLeft));

    errChk( GetCtrlAttribute(panel, ctrl, ATTR_LABEL_WIDTH, &labelWidth));

    errChk( SetCtrlAttribute(panel, ctrl, ATTR_LABEL_LEFT, ctrlLeft - labelWidth - 1));

    errChk( SetCtrlAttribute(panel, ctrl, ATTR_LABEL_TOP, VAL_AUTO_CENTER));

    errChk( SetCtrlAttribute(panel, ctrl, ATTR_LABEL_LEFT, ctrlLeft - labelWidth - labelToCtrlGap));

Error:
    if (error >= 0)
        return 0;
    else
        return error;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

    /*  pass numCtrls == 0 to pull off arguments until a zero argument is reached */
int CVIFUNC GetIntListFromParmInfo(va_list parmInfo, int numIntegers, ListType *integerList)
{
    int error = UIENoError;
    int integer;
    int numIntegersSoFar = 0;

    nullChk( *integerList = ListCreate(sizeof(int)));

    for (integer = va_arg(parmInfo, int); integer > 1; numIntegersSoFar++, integer = va_arg(parmInfo,int))
        {
        if (numIntegersSoFar >= numIntegers && numIntegers != 0)
            break;

        if (!ListInsertItem(*integerList, &integer, END_OF_LIST))
            errChk( UIEOutOfMemory);
        }

Error:
    if (error < 0)
        {
        ListDispose(*integerList);
        *integerList = 0;
        return error;
        }
    else
        return 0;
}

/**************************************************************************/

    /*  passed numIntegers == 0 to pull off integers until a zero is found */
ListType CVIFUNC_C BuildIntegerList(int numIntegers, ...)
{
    va_list     parmInfo;
    ListType    integerList = 0;

    va_start(parmInfo, numIntegers);
    GetIntListFromParmInfo(parmInfo, numIntegers, &integerList);
    va_end(parmInfo);
    return integerList;
}

/**************************************************************************/

    /*  passed numIntegers == 0 to pull off integers until a zero is found */
    /*  returns non-zero if success, 0 if out of memory */
int CVIFUNC_C AppendToIntegerList(ListType listToAppendTo, int numIntegers, ...)
{
    int         error = UIENoError;
    va_list     parmInfo;
    ListType    integerList = 0;

    va_start(parmInfo, numIntegers);
    errChk( GetIntListFromParmInfo(parmInfo, numIntegers, &integerList));

    nullChk( ListAppend(listToAppendTo, integerList));

Error:
    va_end(parmInfo);
    ListDispose(integerList);
    return (error == 0);
}

/**************************************************************************/

int CVIFUNC ListInsertAfterItem(ListType list, const void *ptrToItem, const void *ptrToItemToInsertAfter, CompareFunction compareFunction)
{
    int index;

    index = ListFindItem(list, ptrToItemToInsertAfter, FRONT_OF_LIST, compareFunction);

    if (index == 0)
        index = END_OF_LIST;
    else
        index++;

    return ListInsertItem(list, ptrToItem, index);
}

/**************************************************************************/

int CVIFUNC ListInsertBeforeItem(ListType list, const void *ptrToItem, const void *ptrToItemToInsertBefore, CompareFunction compareFunction)
{
    int index;

    index = ListFindItem(list, ptrToItemToInsertBefore, FRONT_OF_LIST, compareFunction);

    if (index == 0)
        index = FRONT_OF_LIST;

    return ListInsertItem(list, ptrToItem, index);
}

/**************************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC PutLabelsOnLeftForList(int panel, ListType ctrlList, int labelToCtrlGap)
{
    int     error = UIENoError;
    int     ctrl, index, numCtrls;

    numCtrls = ListNumItems(ctrlList);

    for (index = 1; index <= numCtrls; index++)
        {
        ListGetItem(ctrlList, &ctrl, index);

        errChk( PutLabelOnLeft(panel, ctrl, labelToCtrlGap));
        }

Error:
    if(error >= 0)
        return 0;
    else
        return error;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC_C PutLabelsOnLeft(int panel, int labelToCtrlGap, int numCtrls, ...)
{
    va_list     parmInfo;
    int         error = UIENoError;
    ListType    ctrlList = 0;

    va_start(parmInfo, numCtrls);
    errChk( GetIntListFromParmInfo(parmInfo, numCtrls, &ctrlList));

    errChk( PutLabelsOnLeftForList(panel, ctrlList, labelToCtrlGap));

Error:
    ListDispose(ctrlList);
    va_end(parmInfo);
    if (error >= 0)
        return 0;
    else
        return error;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC GetCtrlBoundRectAttribute(int panel, int ctrl, int attribute, int *value)
{
    int error = UIENoError;
    int top, left, width, height;

    errChk( GetCtrlBoundingRect(panel, ctrl, &top, &left, &height, &width));

    switch (attribute)
        {
        case ATTR_LEFT:
        case ATTR_TB_BOUNDS_LEFT:
            *value = left;
            break;
        case ATTR_TOP:
        case ATTR_TB_BOUNDS_TOP:
            *value = top;
            break;
        case ATTR_WIDTH:
        case ATTR_TB_BOUNDS_WIDTH:
            *value = width;
            break;
        case ATTR_HEIGHT:
        case ATTR_TB_BOUNDS_HEIGHT:
            *value = height;
            break;
        default:
            AssertMsg(0, "Bad attribute passed to GetCtrlBoundRectAttribute()");
            break;
        }

Error:
    return error;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC SetCtrlBoundRectAttribute(int panel, int ctrl, int attribute, int value)
{
    int error = UIENoError;
    int mainCtrlPartPosition;
    int oldPosition;

    errChk( GetCtrlBoundRectAttribute(panel, ctrl, attribute, &oldPosition));
    errChk( GetCtrlAttribute(panel, ctrl, attribute, &mainCtrlPartPosition));

    switch (attribute)
        {
        case ATTR_LEFT:
        case ATTR_TOP:
            errChk( SetCtrlAttribute(panel, ctrl, attribute, (value - oldPosition) + mainCtrlPartPosition));
            break;
        case ATTR_WIDTH:
        case ATTR_HEIGHT:
            errChk( UIEAttributeNotSettable);;
        default:
            AssertMsg(0, "Bad attribute passed to SetCtrlBoundRectAttribute()");
            break;
        }

Error:
    return error;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

    /*  Set direction = 0 for horozontal distribution, set direction = 1 for
        vertical distribution.  Set space = 0 for start to end spacing, set
        spacing = 1 for fixed gap spacing.
    */
#if HAVE_CVI_RTE
int CVIFUNC DistributeCtrlsInList(int panel, ListType ctrlList, int direction, int spacing, int *position, int gap)
{
    int     error = UIENoError;
    int     ctrl;
    int     index;
    int     numCtrls;
    int     positionAttribute, sizeAttribute;
    int     totalSize, size;
    int     ctrlGap;

    if (!position)
        errChk( UIENullPointerPassed);

    numCtrls = ListNumItems(ctrlList);

    if (direction == VAL_TB_VERTICAL_DISTRIBUTION)
        {
        positionAttribute = ATTR_TOP;
        sizeAttribute = ATTR_HEIGHT;
        }
    else
        {
        positionAttribute = ATTR_LEFT;
        sizeAttribute = ATTR_WIDTH;
        }

    if (spacing == VAL_TB_FIXED_GAP_SPACING)
        ctrlGap = gap;
    else
    if (numCtrls < 2)
        ctrlGap = 2;
    else
        {
        totalSize = 0;
        for (index = 1; index <= numCtrls; index++)
            {
            ListGetItem(ctrlList, &ctrl, index);

            errChk( GetCtrlBoundRectAttribute(panel, ctrl, sizeAttribute, &size));

            totalSize = totalSize + size;
            }

        ctrlGap = (gap - totalSize)/ (numCtrls - 1);
        }

    for (index = 1; index <= numCtrls; index++)
        {
        ListGetItem(ctrlList, &ctrl, index);

        errChk( GetCtrlBoundRectAttribute(panel, ctrl, sizeAttribute, &size));

        errChk( SetCtrlBoundRectAttribute(panel, ctrl,  positionAttribute, *position));

        *position += size;
        if (index != numCtrls)
            *position += ctrlGap;
        }

Error:
    if (error >= 0)
        return 0;
    else
        return error;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

    /*  Set direction = 0 for horozontal distribution, set direction = 1 for
        vertical distribution.  Set space = 0 for start to end spacing, set
        spacing = 1 for fixed gap spacing.
    */
#if HAVE_CVI_RTE
int CVIFUNC_C DistributeCtrls(int panel, int direction, int spacing, int *position, int gap, int numCtrls, ...)
{
    va_list     parmInfo;
    int         error = UIENoError;
    ListType    ctrlList = 0;

    va_start(parmInfo, numCtrls);
    errChk( GetIntListFromParmInfo(parmInfo, numCtrls, &ctrlList));

    errChk( DistributeCtrlsInList(panel, ctrlList, direction, spacing, position, gap));

Error:
    ListDispose(ctrlList);
    va_end(parmInfo);
    if (error >= 0)
        return 0;
    else
        return error;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC SetCommonDialogShortcutKeys(int panel, int enterCtrl, int escCtrl, int closeCtrl)
{
    int error = UIENoError;

    if (enterCtrl > 0)
        errChk( SetCtrlAttribute(panel, enterCtrl, ATTR_SHORTCUT_KEY, VAL_ENTER_VKEY));

    if (escCtrl > 0)
        errChk( SetCtrlAttribute(panel, escCtrl, ATTR_SHORTCUT_KEY, VAL_ESC_VKEY));

    if (closeCtrl > 0)
        errChk( SetPanelAttribute(panel, ATTR_CLOSE_CTRL, closeCtrl));

Error:
    if (error >= 0)
        return 0;
    else
        return error;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC SetAttributeToExtremeForList(int panel, ListType ctrlList, int attribute, int whichExtreme)
{
    int     error = UIENoError;
    int     maxValue, minValue, value;

    errChk( AttrMaxAndMinForList(panel, ctrlList, attribute, &maxValue,
                0, &minValue, 0));

    if (whichExtreme == VAL_TB_MAX)
        value = maxValue;
    else
        value = minValue;

    errChk( SetAttributeForList(panel, ctrlList, attribute, value));

Error:
    if (error >= 0)
        return 0;
    else
        return error;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC_C SetCtrlsToAttributeExtreme(int panel, int attribute,int whichExtreme, int numCtrls, ...)
{
    va_list     parmInfo;
    int         error = UIENoError;
    ListType    ctrlList = 0;

    va_start(parmInfo, numCtrls);

    errChk( GetIntListFromParmInfo(parmInfo, numCtrls, &ctrlList));

    errChk( SetAttributeToExtremeForList(panel, ctrlList, attribute, whichExtreme));

Error:
    ListDispose(ctrlList);
    va_end(parmInfo);

    if (error >= 0)
        return 0;
    else
        return error;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

int CVIFUNC GetDataTypeSize(int dataType)
{
    switch (dataType)
        {
        case VAL_CHAR:
        case VAL_UNSIGNED_CHAR:
            return 1;
        case VAL_INTEGER:
        case VAL_UNSIGNED_INTEGER:
        case VAL_FLOAT:
        case VAL_STRING: /* size of a pointer */
            return 4;
        case VAL_UNSIGNED_SHORT_INTEGER:
        case VAL_SHORT_INTEGER:
            return 2;
        case VAL_DOUBLE:
            return 8;
        default:
            AssertMsg(0, "Parameter 1 passed to GetDataTypeSize was not a valid data type.");
            return 0;
        }
}

/**************************************************************************/

double CVIFUNC Pi(void)
{
    return PI;
}

/**************************************************************************/

double CVIFUNC TwoPi(void)
{
    return TWO_PI;
}

/**************************************************************************/

double CVIFUNC HalfPi(void)
{
    return HALF_PI;
}

/**************************************************************************/

double CVIFUNC ThreeHalvesPi(void)
{
    return THREE_HALVES_PI;
}

/**************************************************************************/

double CVIFUNC ReciprocalOfPi(void)
{
    return RECIPROCAL_OF_PI;
}

/**************************************************************************/

double CVIFUNC LnOfPi(void)
{
    return LN_OF_PI;
}

/**************************************************************************/

double CVIFUNC Log10OfPi(void)
{
    return LOG_10_OF_PI;
}

/**************************************************************************/

double CVIFUNC Euler(void)
{
    return EULER;
}

/**************************************************************************/

double CVIFUNC ReciprocalOfEuler(void)
{
    return RECIPROCAL_OF_EULER;
}

/**************************************************************************/

double CVIFUNC Log10OfEuler(void)
{
    return LOG_10_OF_EULER;
}

/**************************************************************************/

double CVIFUNC Ln10(void)
{
    return LN_OF_10;
}

/**************************************************************************/

double CVIFUNC Ln2(void)
{
    return LN_OF_2;
}

/**************************************************************************/

double CVIFUNC PlanckConstant(void)
{
    return PLANCK_CONSTANT;
}

/**************************************************************************/

double CVIFUNC ElementaryCharge(void)
{
    return ELEMENTARY_CHARGE;
}

/**************************************************************************/

double CVIFUNC SpeedOfLight(void)
{
    return SPEED_OF_LIGHT;
}

/**************************************************************************/

double CVIFUNC GravitationalConstant(void)
{
    return GRAVITATIONAL_CONSTANT;
}

/**************************************************************************/

double CVIFUNC AvogadroConstant(void)
{
    return AVOGADRO_CONSTANT;
}

/**************************************************************************/

double CVIFUNC RydbergConstant(void)
{
    return RYDBERG_CONSTANT;
}

/**************************************************************************/

double CVIFUNC MolarGasConstant(void)
{
    return MOLAR_GAS_CONSTANT;
}

/**************************************************************************/

double CVIFUNC DegToRad(double degrees)
{
    return DEG_TO_RAD(degrees);
}

/**************************************************************************/

double CVIFUNC RadToDeg(double radians)
{
    return RAD_TO_DEG(radians);
}

/**************************************************************************/

double CVIFUNC CelsiusToKelvin(double degreesCelsius)
{
    return CELSIUS_TO_KELVIN(degreesCelsius);
}

/**************************************************************************/

double CVIFUNC KelvinToCelsius(double degreesKelvin)
{
    return KELVIN_TO_CELSIUS(degreesKelvin);
}

/**************************************************************************/

double CVIFUNC CelsiusToFahrenheit(double degreesCelsius)
{
    return CELSIUS_TO_FAHRENHEIT(degreesCelsius);
}

/**************************************************************************/

double CVIFUNC FahrenheitToCelsius(double degreesFahrenheit)
{
    return FAHRENHEIT_TO_CELSIUS(degreesFahrenheit);
}

/**************************************************************************/

double CVIFUNC MetersToFeet(double meters)
{
    return METERS_TO_FEET(meters);
}

/**************************************************************************/

double CVIFUNC FeetToMeters(double feet)
{
    return FEET_TO_METERS(feet);
}

/**************************************************************************/

double CVIFUNC KilometersToMiles(double kilometers)
{
    return KILOMETERS_TO_MILES(kilometers);
}

/**************************************************************************/

double CVIFUNC MilesToKilometers(double miles)
{
    return MILES_TO_KILOMETERS(miles);
}

/**************************************************************************/

double CVIFUNC KilogramsToPounds(double kilograms)
{
    return KILOGRAMS_TO_POUNDS(kilograms);
}

/**************************************************************************/

double CVIFUNC PoundsToKilograms(double pounds)
{
    return POUNDS_TO_KILOGRAMS(pounds);
}

/**************************************************************************/

double CVIFUNC LitersToGallons(double liters)
{
    return LITERS_TO_GALLONS(liters);
}

/**************************************************************************/

double CVIFUNC GallonsToLiters(double gallons)
{
    return GALLONS_TO_LITERS(gallons);
}

/**************************************************************************/

#if HAVE_CVI_RTE
static DelayQueue CVIFUNC DelayQueueCreate(int panel)
{
    int         error = UIENoError;
    DelayQueue  newQueue = 0;

    nullChk( newQueue = (DelayQueue)calloc(sizeof(*newQueue), 1));

    if (panel <= 0)
        {
        errChk(panel = NewPanel(0,"", -100, -100, 10, 10));
        newQueue->ownsPanel = TRUE;
        }

    newQueue->panel = panel;

    errChk( newQueue->timer = NewCtrl (panel, CTRL_TIMER, "", -100, -100));
    errChk( InstallCtrlCallback (newQueue->panel, newQueue->timer, DelayQueueHandler, newQueue));
    nullChk( newQueue->actionList = ListCreate(sizeof(DelayQueueEntry)));

Error:
    if (error < 0)
        {
        DelayQueueDispose(newQueue, TRUE);
        free(newQueue);
        return 0;
        }
    else
        return newQueue;
}
#endif  // HAVE_CVI_RTE

/*********************************************************/

#if HAVE_CVI_RTE
static void CVIFUNC DelayQueueDispose(DelayQueue delayQueue, int discardUIObjects)
{
    if (delayQueue)
        {
        ListDispose(delayQueue->actionList);
        delayQueue->actionList = NULL;
        if (discardUIObjects && delayQueue->panel > 0)
            {
            if (delayQueue->ownsPanel)
                DiscardPanel(delayQueue->panel);
            else
            if (delayQueue->timer > 0)
                DiscardCtrl(delayQueue->panel, delayQueue->timer);
            }
        }
}
#endif  // HAVE_CVI_RTE

/*********************************************************/

    /* sort in descending order */
static int CVICALLBACK DelayQueueCmpEntriesByTime(void *entry1, void *entry2)
{
    if (((DelayQueueEntry *)entry1)->time > ((DelayQueueEntry *)entry2)->time)
        return -1;
    else
    if (((DelayQueueEntry *)entry1)->time < ((DelayQueueEntry *)entry2)->time)
        return 1;
    else
        return 0;
}

/*********************************************************/

#if HAVE_CVI_RTE
int CVIFUNC PostDelayedCall(DelayedCallbackFunc callback, void *callbackData, double delay)
{
    int             success = 0;
    ThreadLocalVars *pThreadLocalVars = 0;

    pThreadLocalVars = GetThreadLocalVars();

    if (!pThreadLocalVars->delayQueue)
        pThreadLocalVars->delayQueue = DelayQueueCreate(0);     /* Multithreaded, each thread has its own thread local delay queue. */

    if (pThreadLocalVars->delayQueue)
        success = DelayQueueInsert(pThreadLocalVars->delayQueue, callback, callbackData, delay);

    return success;
}
#endif  // HAVE_CVI_RTE

/*********************************************************/

#if HAVE_CVI_RTE
static int CVIFUNC DelayQueueInsert(DelayQueue delayQueue, DelayedCallbackFunc callback, void *callbackData, double delay)
{
    DelayQueueEntry entry;
    int             result;

    entry.callback = callback;
    entry.callbackData = callbackData;
    entry.time = Timer() + delay;

    result = ListInsertInOrder(delayQueue->actionList, &entry, DelayQueueCmpEntriesByTime);
    DelayQueueSetNextInterval(delayQueue);

    return result;
}
#endif  // HAVE_CVI_RTE

/*********************************************************/

#if HAVE_CVI_RTE
static void CVIFUNC DelayQueueSetNextInterval(DelayQueue delayQueue)
{
    DelayQueueEntry entry;

    if (ListNumItems(delayQueue->actionList) == 0)
        {
        SetCtrlAttribute(delayQueue->panel, delayQueue->timer, ATTR_ENABLED, 0);
        }
    else
        {
        double  interval;

        ListGetItem(delayQueue->actionList, &entry, END_OF_LIST);

        interval = Max(entry.time - Timer(), 0.0);

        SetCtrlAttribute(delayQueue->panel, delayQueue->timer, ATTR_ENABLED, 1);
        SetCtrlAttribute(delayQueue->panel, delayQueue->timer, ATTR_INTERVAL, interval);
        }
}
#endif  // HAVE_CVI_RTE

/*********************************************************/

#if HAVE_CVI_RTE
static int CVICALLBACK DelayQueueHandler(int panelHandle, int controlID, int event,
            void *callbackData, int eventData1, int eventData2)
{
    DelayQueue      delayQueue = (DelayQueue)callbackData;
    double          time;
    DelayQueueEntry entry;

    UNUSED(panelHandle);    /* avoid warning when compiling with external compiler */
    UNUSED(controlID);      /* avoid warning when compiling with external compiler */
    UNUSED(eventData2);     /* avoid warning when compiling with external compiler */

    switch (event)
        {
        case EVENT_DISCARD:
            {
            ThreadLocalVars *pThreadLocalVars = 0;

            DelayQueueDispose(delayQueue, FALSE);
            free(delayQueue);
            pThreadLocalVars = GetThreadLocalVars();
            pThreadLocalVars->delayQueue = NULL;
            }
            break;
        case EVENT_TIMER_TICK:
            time = *(double *)eventData1;
            if (ListNumItems(delayQueue->actionList) > 0)
                {
                ListGetItem(delayQueue->actionList, &entry, END_OF_LIST);
                if (entry.time <= time)
                    {
                    ListRemoveItem(delayQueue->actionList, 0, END_OF_LIST);
                    (entry.callback)(entry.callbackData);
                    }

                DelayQueueSetNextInterval(delayQueue);
                }
            break;
        }

    return 0;
}
#endif  // HAVE_CVI_RTE

/*********************************************************/

    /*  Sizing constants for the color change popup */
#define COLOR_CHANGE_MAX_NUM_LIST_BOX_LINES 10
#define COLOR_CHANGE_MIN_NUM_LIST_BOX_LINES 5
#define COLOR_CHANGE_LIST_BOX_WIDTH         550
#define COLOR_CHANGE_SAMPLE_WIDTH           130
#define V_MARGIN                            8
#define H_MARGIN                            8
#define LABEL_H_SEP                         10
#define CTRL_V_SEP                          8
#define MIN_BUTTON_H_SEP         H_MARGIN

typedef struct
    {
    int                     changed;
    int                     panel;
    int                     listBox;
    int                     colorNumeric;
    int                     sampleStart;
    int                     sampleWidth;
    ListType                colorSelectionList;
    void                    *colorChangeCallbackData;
    ShowColorChangeFunction showColorChangeFunc;
    }   ColorChangeDialog;


static int      CVIFUNC FillColorChangeListBox(ColorChangeDialog *colorChangeDialog);
static int      CVICALLBACK ColorListCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
static void     CVIFUNC GetColorChangeListEntryLabel(ColorChangeEntry *entry, int sampleStart, int sampleWidth, char *labelBuf);
static void     CVIFUNC UpdateColorNumeric(ColorChangeDialog *colorChangeDialog);
static void     CVIFUNC SetColorsToDefaults(ListType colorSelectionList);
static int      CVIFUNC UpdateColorListBoxEntry(ColorChangeDialog *colorChangeDialog, int listIndex);
static int      CVIFUNC UpdateAllColorListBoxEntries(ColorChangeDialog *colorChangeDialog);
static int      CVICALLBACK ColorNumericCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);

static int      CVIFUNC ConformBitmapDataToSystem (unsigned char *bitmap, int *colorTable,
                                           int depth, int rowBytes, int width,
                                           int height, int oldPanelColor,
                                           int newPanelColor, int oldTextColor,
                                           int newTextColor);
static int      CVIFUNC ConformOneBitmapToPanel (int panel, int control, int index,
                                         int bitmap3dColor, int new3dColor,
                                         int bitmapTextColor, int newTextColor);
static int      CVIFUNC ConformOneCtrlBitmapToPanel (int panel, int control, int index,
                                             int bitmap3dColor, int new3dColor,
                                             int bitmapTextColor, int newTextColor);

/**********************************************************************************/

static void CVIFUNC GetColorChangeListEntryLabel(ColorChangeEntry *entry, int sampleStart, int sampleWidth, char *labelBuf)
{
    sprintf(labelBuf, "%s\033p%dl\033fgCCCCCC\033drawfilledbox001CCCCCC\033fg%6.6x\033drawfilledbox%3.3d%6.6x",
            entry->name, sampleStart, VAL_WHITE, sampleWidth, entry->color);
}

/**************************************************************************************/

#if HAVE_CVI_RTE
static int CVIFUNC FillColorChangeListBox(ColorChangeDialog *colorChangeDialog)
{
    int                 error = UIENoError;
    int                 index;
    int                 numItems;
    char                buf[512];
    ColorChangeEntry *entry;

    numItems = ListNumItems(colorChangeDialog->colorSelectionList);

    for (index = 1; index <= numItems; index++)
        {
        entry = (ColorChangeEntry *)ListGetPtrToItem(colorChangeDialog->colorSelectionList, index);
        GetColorChangeListEntryLabel(entry, colorChangeDialog->sampleStart, colorChangeDialog->sampleWidth, buf);
        errChk( InsertListItem(colorChangeDialog->panel, colorChangeDialog->listBox, -1, (char *)buf, index));
        }

Error:
    return error;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************************/

static void CVIFUNC SetColorsToDefaults(ListType colorSelectionList)
{
    int                 index;
    int                 numItems;
    ColorChangeEntry *entry;

    numItems = ListNumItems(colorSelectionList);

    for (index = 1; index <= numItems; index++)
        {
        entry = (ColorChangeEntry *)ListGetPtrToItem(colorSelectionList, index);
        entry->color = entry->defaultColor;
        }
}

/*********************************************************************/

#if HAVE_CVI_RTE
static void CVIFUNC UpdateColorNumeric(ColorChangeDialog *colorChangeDialog)
{
    int                 index;
    ColorChangeEntry *entry;

    GetCtrlVal(colorChangeDialog->panel, colorChangeDialog->listBox, &index);
    entry = (ColorChangeEntry *)ListGetPtrToItem(colorChangeDialog->colorSelectionList, index);
    SetCtrlVal(colorChangeDialog->panel, colorChangeDialog->colorNumeric, entry->color);
}
#endif  // HAVE_CVI_RTE

/*********************************************************************/

#if HAVE_CVI_RTE
static int CVICALLBACK ColorListCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    UNUSED(panel);          /* avoid warning when compiling with external compiler */
    UNUSED(control);        /* avoid warning when compiling with external compiler */
    UNUSED(eventData1);     /* avoid warning when compiling with external compiler */
    UNUSED(eventData2);     /* avoid warning when compiling with external compiler */

    switch (event)
        {
        case EVENT_VAL_CHANGED:
            UpdateColorNumeric((ColorChangeDialog *)callbackData);
            break;
        }

    return 0;
}
#endif  // HAVE_CVI_RTE

/*********************************************************************/

#if HAVE_CVI_RTE
static int CVIFUNC UpdateColorListBoxEntry(ColorChangeDialog *colorChangeDialog, int listIndex)
{
    int                 error = UIENoError;
    char                buf[512];
    ColorChangeEntry    *entry;
    int                 colorIndex;

    GetValueFromIndex(colorChangeDialog->panel, colorChangeDialog->listBox, listIndex, &colorIndex);
    entry = (ColorChangeEntry *)ListGetPtrToItem(colorChangeDialog->colorSelectionList, colorIndex);
    GetColorChangeListEntryLabel(entry, colorChangeDialog->sampleStart, colorChangeDialog->sampleWidth, buf);
    errChk( ReplaceListItem(colorChangeDialog->panel, colorChangeDialog->listBox, listIndex, (char *)buf, colorIndex));

Error:
    return error;
}
#endif  // HAVE_CVI_RTE

/*********************************************************************/

#if HAVE_CVI_RTE
static int CVIFUNC UpdateAllColorListBoxEntries(ColorChangeDialog *colorChangeDialog)
{
    int error = UIENoError;
    int colorIndex;
    int numItems;

    numItems = ListNumItems(colorChangeDialog->colorSelectionList);

    for (colorIndex = 1; colorIndex <= numItems; colorIndex++)
        errChk( UpdateColorListBoxEntry(colorChangeDialog, colorIndex - 1));

Error:
    return error;
}
#endif  // HAVE_CVI_RTE

/*********************************************************************/

#if HAVE_CVI_RTE
static int CVICALLBACK ColorNumericCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    int                 error = UIENoError;
    int                 listIndex, colorIndex;
    int                 color;
    ColorChangeDialog   *colorChangeDialog = (ColorChangeDialog *)callbackData;
    ColorChangeEntry    *entry;


    UNUSED(eventData1);     /* avoid warning when compiling with external compiler */
    UNUSED(eventData2);     /* avoid warning when compiling with external compiler */

    switch (event)
        {
        case EVENT_VAL_CHANGED:
            GetCtrlVal(panel, control, &color);
            GetCtrlIndex(panel, colorChangeDialog->listBox, &listIndex);
            GetValueFromIndex(panel, colorChangeDialog->listBox, listIndex, &colorIndex);
            entry = (ColorChangeEntry *)ListGetPtrToItem(colorChangeDialog->colorSelectionList, colorIndex);
            entry->color = color;
            errChk(UpdateColorListBoxEntry(colorChangeDialog, listIndex));
            break;
        case EVENT_COMMIT:
            colorChangeDialog->changed = TRUE;
            GetCtrlIndex(panel, colorChangeDialog->listBox, &listIndex);
            if (listIndex > -1)
                {
                if (colorChangeDialog->showColorChangeFunc)
                    colorChangeDialog->showColorChangeFunc(colorChangeDialog->colorSelectionList, colorChangeDialog->colorChangeCallbackData, listIndex + 1);
                }
            break;
        }

    return 0;

Error:
    QueueUserEvent(error, 0, 0);
    return 0;
}
#endif  // HAVE_CVI_RTE

/*********************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC ColorChangePopup(const char title[], ListType colorList, int allowDefault, ShowColorChangeFunction showColorChangeFunc, void *colorChangeCallbackData)
{
    int             error = UIENoError;
    int             panel = 0;
    int             okButton, cancelButton, listBox, colorHeadingTxtMsg, sampleHeadingTxtMsg;
    int             colorNumeric, defaultButton;
    int             listBoxWidth = COLOR_CHANGE_LIST_BOX_WIDTH;
    int             sampleWidth = COLOR_CHANGE_SAMPLE_WIDTH;
    int             top, height, width;
    int             leftMargin, rightMargin;
    ColorChangeDialog   colorChangeDialog;
    int             done = FALSE;
    int             evtPanel, evtCtrl;
    int             numListLines;
    ListType        colorListCopy = 0;

    nullChk( colorListCopy = ListCopy(colorList));

    top = V_MARGIN;
    leftMargin = H_MARGIN;
    rightMargin = listBoxWidth + H_MARGIN;

    errChk( panel =                 NewPanel(0, title, 30,30, 200,200));
    errChk( SetPanelAttribute(panel, ATTR_CONFORM_TO_SYSTEM, TRUE));
    errChk( okButton =              NewCtrl(panel, CTRL_SQUARE_COMMAND_BUTTON, "__OK", 10, 10));
    errChk( cancelButton =          NewCtrl(panel, CTRL_SQUARE_COMMAND_BUTTON, "  __Cancel  ", 10, 10));
    errChk( defaultButton =         NewCtrl(panel, CTRL_SQUARE_COMMAND_BUTTON, "__Default", 10, 10));
    errChk( colorNumeric =          NewCtrl(panel, CTRL_COLOR_NUMERIC, "Color:" , 0, 0));
    errChk( colorHeadingTxtMsg =    NewCtrl(panel, CTRL_TEXT_MSG, "Color Name:", top, leftMargin));
    errChk( sampleHeadingTxtMsg =   NewCtrl(panel, CTRL_TEXT_MSG, "Sample:", top, 0));
    errChk( listBox =               NewCtrl(panel, CTRL_LIST, 0, 0,H_MARGIN));

        /* setup listbox heading text messages */
    SetCtrlAttribute(panel, sampleHeadingTxtMsg, ATTR_LEFT, rightMargin - sampleWidth);
    GetCtrlAttribute(panel, colorHeadingTxtMsg, ATTR_HEIGHT, &height);
    top += height + CTRL_V_SEP / 2;

        /* setup listbox */
    InstallCtrlCallback(panel, listBox, ColorListCallback, &colorChangeDialog);
    numListLines = (int)Pin(ListNumItems(colorList), COLOR_CHANGE_MIN_NUM_LIST_BOX_LINES, COLOR_CHANGE_MAX_NUM_LIST_BOX_LINES);
    SetCtrlAttribute(panel, listBox, ATTR_VISIBLE_LINES, numListLines);
    SetCtrlAttribute(panel, listBox, ATTR_TOP, top);
    SetCtrlAttribute(panel, listBox, ATTR_WIDTH, listBoxWidth);
    GetCtrlAttribute(panel, listBox, ATTR_HEIGHT, &height);
    top += height + CTRL_V_SEP;

        /* setup color numeric */
    SetCtrlAttribute(panel, colorNumeric, ATTR_TOP, top);
    SetCtrlAttribute(panel, colorNumeric, ATTR_LABEL_TOP, top);
    GetCtrlAttribute(panel,colorNumeric, ATTR_HEIGHT, &height);
    GetCtrlAttribute(panel,colorNumeric, ATTR_LABEL_WIDTH, &width);
    SetCtrlAttribute(panel,colorNumeric, ATTR_LEFT, width + leftMargin + LABEL_H_SEP);
    SetCtrlAttribute(panel,colorNumeric, ATTR_LABEL_LEFT, leftMargin);
    SetCtrlAttribute(panel, colorNumeric, ATTR_LABEL_TOP, VAL_AUTO_CENTER);
    SetCtrlAttribute(panel, colorNumeric, ATTR_SHOW_TRANSPARENT, FALSE);
    InstallCtrlCallback(panel, colorNumeric, ColorNumericCallback, &colorChangeDialog);

        /* position ok, cancel, and default buttons */
    SetCtrlsToAttributeExtreme (panel, ATTR_WIDTH, VAL_TB_MAX, 3, okButton, cancelButton, defaultButton);
    SetAttributeForCtrls (panel, ATTR_TOP, top, 3, okButton, cancelButton, defaultButton);

    GetCtrlAttribute(panel, okButton, ATTR_WIDTH, &width);  /* buttons are now the same width */

    SetCtrlAttribute(panel,cancelButton, ATTR_LEFT, rightMargin - width);
    SetCtrlAttribute(panel,okButton, ATTR_LEFT, rightMargin - width * 2 - 2 * MIN_BUTTON_H_SEP);
    SetCtrlAttribute(panel,defaultButton, ATTR_LEFT, rightMargin - width * 3 - 4 * MIN_BUTTON_H_SEP);

    GetCtrlAttribute(panel,okButton, ATTR_HEIGHT, &height);
    top += CTRL_V_SEP + height;

        /* position panel */
    top += V_MARGIN;
    SetPanelAttribute(panel, ATTR_HEIGHT, top);
    SetPanelAttribute(panel, ATTR_WIDTH, listBoxWidth + 2 * H_MARGIN);
    SetPanelPos(panel, VAL_AUTO_CENTER, VAL_AUTO_CENTER);
    SetCommonDialogShortcutKeys (panel, okButton, cancelButton, cancelButton);

    if (!allowDefault)
        SetCtrlAttribute(panel, defaultButton, ATTR_VISIBLE, FALSE);

    colorChangeDialog.changed = FALSE;
    colorChangeDialog.panel = panel;
    colorChangeDialog.listBox = listBox;
    colorChangeDialog.colorNumeric = colorNumeric;
    colorChangeDialog.sampleStart = listBoxWidth - LIST_BOX_LEFT_PIXEL_POS - sampleWidth;
    colorChangeDialog.sampleWidth = listBoxWidth - colorChangeDialog.sampleStart - LIST_BOX_FRAME_WIDTH - LIST_BOX_RIGHT_MARGIN - LIST_BOX_LEFT_PIXEL_POS - LIST_BOX_SCROLL_BAR_WIDTH;
    colorChangeDialog.showColorChangeFunc = showColorChangeFunc;
    colorChangeDialog.colorChangeCallbackData = colorChangeCallbackData;
    colorChangeDialog.colorSelectionList = colorList;

    errChk(FillColorChangeListBox(&colorChangeDialog));

    SetActiveCtrl(panel, listBox);
    UpdateColorNumeric(&colorChangeDialog);
    InstallPopup(panel);

    while (!done)
        {
        errChk( GetUserEvent(0, &evtPanel, &evtCtrl));

        if (evtPanel == panel)
            {
            if (evtCtrl == cancelButton)
                {
                if (colorChangeDialog.changed)
                    {
                    ListClear(colorList);
                    nullChk( ListAppend (colorList, colorListCopy));
                    if (showColorChangeFunc)
                        showColorChangeFunc(colorChangeDialog.colorSelectionList, colorChangeDialog.colorChangeCallbackData, -1);
                    }

                colorChangeDialog.changed = FALSE;
                done = TRUE;
                }
            else
            if (evtCtrl == okButton)
                {
                done = TRUE;
                }
            else
            if (evtCtrl == defaultButton)
                {
                colorChangeDialog.changed = TRUE;
                SetColorsToDefaults(colorChangeDialog.colorSelectionList);
                errChk( UpdateAllColorListBoxEntries(&colorChangeDialog));
                UpdateColorNumeric(&colorChangeDialog);
                if (showColorChangeFunc)
                    showColorChangeFunc(colorChangeDialog.colorSelectionList, colorChangeDialog.colorChangeCallbackData, -1);
                }
            }
        }

Error:
    if (panel > 0)
        DiscardPanel(panel);

    ListDispose(colorListCopy);
    if (error < 0)
        return error;
    else
        return colorChangeDialog.changed;
}
#endif  // HAVE_CVI_RTE

/******************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC GetNumericAttributeAsType(int panel, int ctrl, int attribute, int destType, void *value)
{
    char            charVal = 0;
    unsigned char   uCharVal = 0;
    int             intVal = 0;
    unsigned int    uIntVal = 0;
    short           shortIntVal = 0;
    unsigned short  uShortIntVal = 0;
    float           floatVal = 0.f;
    double          doubleVal = 0.;
    int             currType = 0;
    int             error = UIENoError;

    errChk( GetCtrlAttribute(panel, ctrl, ATTR_DATA_TYPE, &currType));

    switch (currType)
        {
        case VAL_CHAR:
            errChk( GetCtrlAttribute(panel, ctrl, attribute, &charVal));
            doubleVal = (double)charVal;
            break;
        case VAL_UNSIGNED_CHAR:
            errChk( GetCtrlAttribute(panel, ctrl, attribute, &uCharVal));
            doubleVal = (double)uCharVal;
            break;
        case VAL_INTEGER:
            errChk( GetCtrlAttribute(panel, ctrl, attribute, &intVal));
            doubleVal = (double)intVal;
            break;
        case VAL_UNSIGNED_INTEGER:
            errChk( GetCtrlAttribute(panel, ctrl, attribute, &uIntVal));
            doubleVal = (double)uIntVal;
            break;
        case VAL_FLOAT:
            errChk( GetCtrlAttribute(panel, ctrl, attribute, &floatVal));
            doubleVal = (double)floatVal;
            break;
        case VAL_UNSIGNED_SHORT_INTEGER:
            errChk( GetCtrlAttribute(panel, ctrl, attribute, &uShortIntVal));
            doubleVal = (double)uShortIntVal;
            break;
        case VAL_SHORT_INTEGER:
            errChk( GetCtrlAttribute(panel, ctrl, attribute, &shortIntVal));
            doubleVal = (double)shortIntVal;
            break;
        case VAL_DOUBLE:
            errChk( GetCtrlAttribute(panel, ctrl, attribute, &doubleVal));
            break;
        default:
            AssertMsg(0, "The type of the control passed to GetNumericAttributeAsType\nwas not a numeric type.");
            break;;
        }

    switch (destType)
        {
        case VAL_CHAR:
            *(char *)value = (char)doubleVal;
            break;
        case VAL_UNSIGNED_CHAR:
            *(unsigned char *)value = (unsigned char)doubleVal;
            break;
        case VAL_INTEGER:
            *(int *)value = (int)doubleVal;
            break;
        case VAL_UNSIGNED_INTEGER:
            *(unsigned int *)value = (unsigned int)doubleVal;
            break;
        case VAL_FLOAT:
            *(float *)value = (float)doubleVal;
            break;
        case VAL_UNSIGNED_SHORT_INTEGER:
            *(unsigned short *)value = (unsigned short)doubleVal;
            break;
        case VAL_SHORT_INTEGER:
            *(short *)value = (short)doubleVal;
            break;
        case VAL_DOUBLE:
            *(double *)value = doubleVal;
        default:
            AssertMsg(0, "Parameter 4 passed to GetNumericAttributeAsType\nwas not a numeric data type.");
            break;;
        }

Error:
    return 0;
}
#endif  // HAVE_CVI_RTE

/*********************************************************/

#if HAVE_CVI_RTE
int CVIFUNC MoveBehind(int panel, int ctrl, int ctrlToMove)
{
    int zOfCtrl;
    int zOfCtrlToMove;
    int error = UIENoError;

    errChk( GetCtrlAttribute(panel, ctrl, ATTR_ZPLANE_POSITION, &zOfCtrl));
    errChk( GetCtrlAttribute(panel, ctrlToMove, ATTR_ZPLANE_POSITION, &zOfCtrlToMove));

    if (zOfCtrl > zOfCtrlToMove)
        errChk( SetCtrlAttribute(panel, ctrlToMove, ATTR_ZPLANE_POSITION, zOfCtrl));
    else
        errChk( SetCtrlAttribute(panel, ctrlToMove, ATTR_ZPLANE_POSITION, zOfCtrl + 1));

Error:
    if (error < 0)
        return error;
    else
        return 0;
}
#endif  // HAVE_CVI_RTE

/**********************************************************/

#if HAVE_CVI_RTE
int CVIFUNC MoveInFront(int panel, int ctrl, int ctrlToMove)
{
    int zOfCtrl;
    int zOfCtrlToMove;
    int error = UIENoError;

    errChk( GetCtrlAttribute(panel, ctrl, ATTR_ZPLANE_POSITION, &zOfCtrl));
    errChk( GetCtrlAttribute(panel, ctrlToMove, ATTR_ZPLANE_POSITION, &zOfCtrlToMove));

    if (zOfCtrl > zOfCtrlToMove)
        errChk( SetCtrlAttribute(panel, ctrlToMove, ATTR_ZPLANE_POSITION, zOfCtrl-1));
    else
        errChk( SetCtrlAttribute(panel, ctrlToMove, ATTR_ZPLANE_POSITION, zOfCtrl));

Error:
    if (error < 0)
        return error;
    else
        return 0;
}
#endif  // HAVE_CVI_RTE

/*********************************************************/

/*  places ctrlToMove after ctrl in the tab order */
#if HAVE_CVI_RTE
int CVIFUNC PlaceAfter(int panel, int ctrl, int ctrlToMove)
{
    int tabPosOfCtrl;
    int tabPosOfCtrlToMove;
    int error = UIENoError;

    errChk( GetCtrlAttribute(panel, ctrl, ATTR_CTRL_TAB_POSITION, &tabPosOfCtrl));
    errChk( GetCtrlAttribute(panel, ctrlToMove, ATTR_CTRL_TAB_POSITION, &tabPosOfCtrlToMove));

    if (tabPosOfCtrl > tabPosOfCtrlToMove)
        errChk( SetCtrlAttribute(panel, ctrlToMove, ATTR_CTRL_TAB_POSITION, tabPosOfCtrl));
    else
        errChk( SetCtrlAttribute(panel, ctrlToMove, ATTR_CTRL_TAB_POSITION, tabPosOfCtrl + 1));

Error:
    if (error < 0)
        return error;
    else
        return 0;
}
#endif  // HAVE_CVI_RTE

/**********************************************************/

    /*  places ctrlToMove before ctrl in the tab order */
#if HAVE_CVI_RTE
int CVIFUNC PlaceBefore(int panel, int ctrl, int ctrlToMove)
{
    int tabPosOfCtrl;
    int tabPosOfCtrlToMove;
    int error = UIENoError;

    errChk( GetCtrlAttribute(panel, ctrl, ATTR_CTRL_TAB_POSITION, &tabPosOfCtrl));
    errChk( GetCtrlAttribute(panel, ctrlToMove, ATTR_CTRL_TAB_POSITION, &tabPosOfCtrlToMove));

    if (tabPosOfCtrl > tabPosOfCtrlToMove)
        errChk( SetCtrlAttribute(panel, ctrlToMove, ATTR_CTRL_TAB_POSITION, tabPosOfCtrl-1));
    else
        errChk( SetCtrlAttribute(panel, ctrlToMove, ATTR_CTRL_TAB_POSITION, tabPosOfCtrl));

Error:
    if (error < 0)
        return error;
    else
        return 0;
}
#endif  // HAVE_CVI_RTE

/***************************************************************************************/

#if HAVE_CVI_RTE
    /* positions one ctrl next to another */
int CVIFUNC PositionCtrlRelativeToCtrl(int panel, int ctrl, int considerLabel,
                    int ctrlToMove, int considerCtrlToMoveLabel, int gap,
                    PositionCtrlSide whichSide, PositionCtrlJustification justification)
{
    int     error = 0;
    Rect    ctrlRect;
    Rect    ctrlToMoveRect;
    int     newTop = 0, newLeft = 0;    /* for ctrlToMove */
    int     currentTop, currentLeft;    /* for ctrlToMove */

    if (considerLabel)
        errChk( GetCtrlBoundingRect(panel, ctrl, &ctrlRect.top, &ctrlRect.left, &ctrlRect.height, &ctrlRect.width));
    else
        {
        errChk( GetCtrlAttribute(panel, ctrl, ATTR_TOP, &ctrlRect.top));
        errChk( GetCtrlAttribute(panel, ctrl, ATTR_LEFT, &ctrlRect.left));
        errChk( GetCtrlAttribute(panel, ctrl, ATTR_HEIGHT, &ctrlRect.height));
        errChk( GetCtrlAttribute(panel, ctrl, ATTR_WIDTH, &ctrlRect.width));
        }

    if (considerCtrlToMoveLabel)
        {
        errChk( GetCtrlBoundingRect(panel, ctrlToMove, &ctrlToMoveRect.top, &ctrlToMoveRect.left, &ctrlToMoveRect.height, &ctrlToMoveRect.width));

        errChk( GetCtrlAttribute(panel, ctrlToMove, ATTR_TOP, &currentTop));
        errChk( GetCtrlAttribute(panel, ctrlToMove, ATTR_LEFT, &currentLeft));
        }
    else
        {
        errChk( GetCtrlAttribute(panel, ctrlToMove, ATTR_TOP, &ctrlToMoveRect.top));
        errChk( GetCtrlAttribute(panel, ctrlToMove, ATTR_LEFT, &ctrlToMoveRect.left));
        errChk( GetCtrlAttribute(panel, ctrlToMove, ATTR_HEIGHT, &ctrlToMoveRect.height));
        errChk( GetCtrlAttribute(panel, ctrlToMove, ATTR_WIDTH, &ctrlToMoveRect.width));
        currentTop = ctrlToMoveRect.top;
        currentLeft = ctrlToMoveRect.left;
        }

        /* compute left */
    switch (whichSide)
        {
        case kPositionCtrlTopLeft:
        case kPositionCtrlCenterLeft:
        case kPositionCtrlBottomLeft:
            newLeft = ctrlRect.left - ctrlToMoveRect.width - gap;
            break;
        case kPositionCtrlTopCenter:
        case kPositionCtrlCenterCenter:
        case kPositionCtrlBottomCenter:
            switch (justification)
                {
                case kPositionCtrlTopOrLeftJustication:
                    newLeft = ctrlRect.left;
                    break;
                case kPositionCtrlCenterJustification:
                    newLeft = ctrlRect.left + (ctrlRect.width - ctrlToMoveRect.width) / 2;
                    break;
                case kPositionCtrlBottomOrRightJustication:
                    newLeft = ctrlRect.left + ctrlRect.width - ctrlToMoveRect.width;
                    break;
                }
            break;
        case kPositionCtrlBottomRight:
        case kPositionCtrlTopRight:
        case kPositionCtrlCenterRight:
            newLeft = ctrlRect.left + ctrlRect.width + gap;
            break;
        }

        /* compute top */
    switch (whichSide)
        {
        case kPositionCtrlTopLeft:
        case kPositionCtrlTopCenter:
        case kPositionCtrlTopRight:
            newTop = ctrlRect.top - ctrlToMoveRect.height - gap;
            break;
        case kPositionCtrlCenterLeft:
        case kPositionCtrlCenterCenter:
        case kPositionCtrlCenterRight:
            switch (justification)
                {
                case kPositionCtrlTopOrLeftJustication:
                    newTop = ctrlRect.top;
                    break;
                case kPositionCtrlCenterJustification:
                    newTop = ctrlRect.top + (ctrlRect.height - ctrlToMoveRect.height) / 2;
                    break;
                case kPositionCtrlBottomOrRightJustication:
                    newTop = ctrlRect.top + ctrlRect.height - ctrlToMoveRect.height;
                    break;
                }
            break;
        case kPositionCtrlBottomLeft:
        case kPositionCtrlBottomCenter:
        case kPositionCtrlBottomRight:
            newTop = ctrlRect.top + ctrlRect.height + gap;
            break;
        }

    if (considerCtrlToMoveLabel) /* we are "moving" the bounding rect */
        {
        newTop = currentTop + (newTop - ctrlToMoveRect.top);
        newLeft = currentLeft + (newLeft - ctrlToMoveRect.left);
        }

    errChk( SetCtrlAttribute(panel, ctrlToMove, ATTR_TOP, newTop));
    errChk( SetCtrlAttribute(panel, ctrlToMove, ATTR_LEFT, newLeft));


Error:
    return error;
}
#endif  // HAVE_CVI_RTE

/***************************************************************************************/

#if HAVE_CVI_RTE
void CVIFUNC SetCtrlRightPosition(int panel, int control, int right)
{
    int width;

    GetCtrlAttribute(panel, control, ATTR_WIDTH, &width);
    SetCtrlAttribute(panel, control, ATTR_LEFT, right - width);
}
#endif  // HAVE_CVI_RTE

/*****************************************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC CenterCtrl(int panel, int ctrl, int top, int left, int height, int width)
{
    int error = UIENoError;
    int cTop, cLeft;        /*  control top, control left */
    int bTop, bLeft;        /*  control bounds top and left */
    int bHeight, bWidth;    /*  control bounds height and width */
    int newBTop, newBLeft;  /*  new control bounds top and left */
    int newTop, newLeft;    /*  new control top and left */

    errChk( GetCtrlBoundingRect(panel, ctrl, &bTop, &bLeft, &bHeight, &bWidth));
    errChk( GetCtrlAttribute(panel, ctrl, ATTR_TOP, &cTop));
    errChk( GetCtrlAttribute(panel, ctrl, ATTR_LEFT, &cLeft));

    newBTop = (height - bHeight) / 2 + top;
    newBLeft = (width - bWidth) / 2 + left;

    newTop = cTop + (newBTop - bTop);
    newLeft = cLeft + (newBLeft - bLeft);

    errChk( SetCtrlAttribute(panel, ctrl, ATTR_TOP, newTop));
    errChk( SetCtrlAttribute(panel, ctrl, ATTR_LEFT, newLeft));

Error:
    return 0;
}
#endif  // HAVE_CVI_RTE

/**********************************************************/

#if HAVE_CVI_RTE
int CVIFUNC SizeRingCtrlToText(int panel, int ring)
{
    int     error  = 0;
    int     index, numItems, maxWidth = 0, width;
    char    *fontName = 0;
    int     fontNameLength;
    int     italic, underLine, strikeOut, pointSize, bold;
    int     style;
    char    label[256];
    int     extraRingWidth = 0;

    errChk( GetCtrlAttribute(panel, ring, ATTR_CTRL_STYLE, &style));

    switch (style)
        {
        case CTRL_RING:
            extraRingWidth = 19;
            break;
        case CTRL_RECESSED_MENU_RING:
            extraRingWidth = 21;
            break;
        default:
            errChk( UIEInvalidControlType);
        }

    errChk( GetCtrlAttribute(panel, ring, ATTR_TEXT_FONT_NAME_LENGTH, &fontNameLength));

    nullChk( fontName = (char *)malloc(fontNameLength + 1));

    errChk( GetCtrlAttribute(panel, ring, ATTR_TEXT_FONT, fontName));
    errChk( GetCtrlAttribute(panel, ring, ATTR_TEXT_ITALIC, &italic));
    errChk( GetCtrlAttribute(panel, ring, ATTR_TEXT_UNDERLINE, &underLine));
    errChk( GetCtrlAttribute(panel, ring, ATTR_TEXT_STRIKEOUT, &strikeOut));
    errChk( GetCtrlAttribute(panel, ring, ATTR_TEXT_POINT_SIZE, &pointSize));
    errChk( GetCtrlAttribute(panel, ring, ATTR_TEXT_BOLD, &bold));

    errChk( CreateMetaFont ("___RingMetaFont", fontName, pointSize, bold, italic, underLine, strikeOut));

    errChk( GetNumListItems (panel, ring, &numItems));

    for (index = 0; index < numItems; index++)
        {
        errChk( GetLabelFromIndex (panel, ring, index, label));
        errChk( GetTextDisplaySize (label, "___RingMetaFont", 0, &width));

        maxWidth = Max(maxWidth, width);
        }

    errChk( SetCtrlAttribute (panel, ring, ATTR_WIDTH, maxWidth + extraRingWidth));

        /*  Since we don't want the user to get back ___RingMetaFont when querying the fontname of their
            controls, we change this metafont to an unreasonable font that no one would ever user (since
            there isn't an explicit DeleteMetaFont() function.
        */
    errChk( CreateMetaFont ("___RingMetaFont", "bogusfont", 6, TRUE, TRUE, TRUE, TRUE));

Error:
    free(fontName);
    return error;
}
#endif  // HAVE_CVI_RTE

/*******************************************************************************/


#define MAX_COLOR_DIFF      15

#define RedComponent(c)     (((c) >> 16) & 0xff)
#define GreenComponent(c)   (((c) >> 8) & 0xff)
#define BlueComponent(c)    ((c) & 0xff)

#define ComputeColorDiff(c1,c2) (abs(RedComponent(c1) - RedComponent(c2)) + \
                                 abs(GreenComponent(c1) - GreenComponent(c2)) + \
                                 abs(BlueComponent(c1) - BlueComponent(c2)))

#if HAVE_CVI_RTE
static int CVIFUNC ConformBitmapDataToSystem (unsigned char *bitmap, int *colorTable,
                                      int depth, int rowBytes, int width,
                                      int height, int oldPanelColor,
                                      int newPanelColor, int oldTextColor,
                                      int newTextColor)
{
    int error = UIENoError;
    int i, j, index, numColors = 0, color, newColor, srcByte;
    int oldPanelHilite, newPanelHilite, oldPanelShadow, newPanelShadow;

    switch (depth)
    {
        case 1:
            numColors = 2;
            break;
        case 4:
            numColors = 16;
            break;
        case 8:
            numColors = 256;
            break;
        case 24:
            numColors = 0;
            break;
        /* Start fix for CAR XCE0AQA */
        case 32:
            numColors = 0;
            break;
        default:
            error = ToolErr_UnRecognizedValue;
            goto Error;
        /* End fix for CAR XCE0AQA */
    }

    errChk (Get3dBorderColors (oldPanelColor, &oldPanelHilite, NULL,
                               &oldPanelShadow, NULL));
    errChk (Get3dBorderColors (newPanelColor, &newPanelHilite, NULL,
                               &newPanelShadow, NULL));

    if (numColors > 0)
    {
        for (index = 0; index < numColors; index++)
        {
            newColor = color = *(colorTable + index);
            if (ComputeColorDiff (color, oldPanelColor) <= MAX_COLOR_DIFF)
                newColor = newPanelColor;
            if (ComputeColorDiff (color, oldPanelHilite) <= MAX_COLOR_DIFF)
                newColor = newPanelHilite;
            if (ComputeColorDiff (color, oldPanelShadow) <= MAX_COLOR_DIFF)
                newColor = newPanelShadow;
            if (ComputeColorDiff (color, oldTextColor) <= MAX_COLOR_DIFF)
                newColor = newTextColor;
            if (newColor != color)
                *(colorTable + index) = newColor;
        }
    }
    else                                    /* depth == 24, no colorTable */
    {
        for (i = 0; i < height ; i++)
        {
            for (j = 0; j < width; j++)
            {
                srcByte = 3 * j + i * rowBytes;
                newColor = color = MakeColor (bitmap[srcByte],
                                              bitmap[srcByte+1],
                                              bitmap[srcByte+2]);
                if (ComputeColorDiff (color, oldPanelColor) <= MAX_COLOR_DIFF)
                    newColor = newPanelColor;
                if (ComputeColorDiff (color, oldPanelHilite) <= MAX_COLOR_DIFF)
                    newColor = newPanelHilite;
                if (ComputeColorDiff (color, oldPanelShadow) <= MAX_COLOR_DIFF)
                    newColor = newPanelShadow;
                if (ComputeColorDiff (color, oldTextColor) <= MAX_COLOR_DIFF)
                    newColor = newTextColor;
                if (newColor != color)
                {
                    bitmap[srcByte] = RedComponent(newColor);
                    bitmap[srcByte+1] = GreenComponent(newColor);
                    bitmap[srcByte+2] = BlueComponent(newColor);
                }
            }
        }
    }

Error:
    return error;
}
#endif  // HAVE_CVI_RTE


#if HAVE_CVI_RTE
static int CVIFUNC ConformOneBitmapToPanel (int panel, int control, int index,
                                    int bitmap3dColor, int new3dColor,
                                    int bitmapTextColor, int newTextColor)
{
    int error = UIENoError;
    int *colorTable = NULL, rowBytes, depth, width, height,
        colorTableSize, bitmapSize, maskSize;
    unsigned char *bitmap = NULL, *mask = NULL;

    errChk (GetImageInfo (panel, control, index, &colorTableSize,
                          &bitmapSize, &maskSize));
    if (colorTableSize)
        nullChk (colorTable = malloc (colorTableSize));
    nullChk (bitmap = malloc (bitmapSize));
    if (maskSize)
        nullChk (mask = malloc (maskSize));
    errChk (GetImageBits (panel, control, index, &rowBytes, &depth,
                          &width, &height, colorTable, bitmap, mask));

    errChk (ConformBitmapDataToSystem (bitmap, colorTable, depth, rowBytes,
                                       width, height, bitmap3dColor, new3dColor,
                                       bitmapTextColor, newTextColor));

    errChk (SetImageBits (panel, control, index, rowBytes, depth,
                          width, height, colorTable, bitmap, mask));

Error :
    if (colorTable)
        free (colorTable);
    if (bitmap)
        free (bitmap);
    if (mask)
        free (mask);

    return error;
}
#endif  // HAVE_CVI_RTE


#if HAVE_CVI_RTE
static int CVIFUNC ConformOneCtrlBitmapToPanel (int panel, int control, int index,
                                        int bitmap3dColor, int new3dColor,
                                        int bitmapTextColor, int newTextColor)
{
    int error = UIENoError;
    int ctrlStyle;
    int numItems, i;

    errChk (GetCtrlAttribute (panel, control, ATTR_CTRL_STYLE, &ctrlStyle));
    switch (ctrlStyle)
    {
        case CTRL_PICTURE_COMMAND_BUTTON:
        case CTRL_PICTURE_TOGGLE_BUTTON:
        case CTRL_PICTURE:
        case CTRL_PICTURE_RING:
            break;
        default:
            errChk (UIEInvalidControlType);
            break;
    }

    if ((ctrlStyle == CTRL_PICTURE_RING) && (index == -1))
    {
        errChk (GetNumListItems (panel, control, &numItems));
        for (i = 0 ; i < numItems ; i++)
        {
            errChk (ConformOneBitmapToPanel (panel, control, i,
                                             bitmap3dColor, new3dColor,
                                             bitmapTextColor, newTextColor));
        }
    }
    else
    {
        errChk (ConformOneBitmapToPanel (panel, control, 0,
                                         bitmap3dColor, new3dColor,
                                         bitmapTextColor, newTextColor));
    }

Error:
    return error;
}
#endif  // HAVE_CVI_RTE


#if HAVE_CVI_RTE
int CVIFUNC ConformCtrlBitmapToPanel (int panel, int control, int index,
                              int bitmap3dColor, int new3dColor,
                              int bitmapTextColor, int newTextColor)
{
    int error = UIENoError;
    int nextCtrl;
    int actual3dColor = new3dColor, actualTextColor = newTextColor;

    if (actual3dColor == -1)
    {
        errChk (GetPanelAttribute (panel, ATTR_BACKCOLOR, &actual3dColor));
    }

    if (actualTextColor == -1)
    {
        errChk (GetPanelAttribute (panel, ATTR_TITLE_COLOR, &actualTextColor));
    }

    if (control == -1)
    {
        errChk (GetPanelAttribute (panel, ATTR_PANEL_FIRST_CTRL, &nextCtrl));

        while (nextCtrl != 0)
        {
            errChk (ConformOneCtrlBitmapToPanel (panel, nextCtrl, index,
                                                 bitmap3dColor, actual3dColor,
                                                 bitmapTextColor, actualTextColor));
            errChk (GetCtrlAttribute (panel, nextCtrl, ATTR_NEXT_CTRL,
                                      &nextCtrl));
        }
    }
    else
    {
        errChk (ConformOneCtrlBitmapToPanel (panel, control, index,
                                             bitmap3dColor, actual3dColor,
                                             bitmapTextColor, actualTextColor));
    }

Error:
    return error;
}
#endif  // HAVE_CVI_RTE

/*******************************************************************************/

#define PROGRESS_BOX_MARGIN         14
#define PROGRESS_BAR_HEIGHT         14
#define PROGRESS_BAR_WIDTH          300
#define PROGRESS_LABEL_CTRL_V_GAP   4

typedef struct
    {
    int panel;
    int progressIndicator;
    int cancelRequested;
    int cancelButton;
    } *ProgressDlg;


#if HAVE_CVI_RTE
void CVIFUNC DiscardProgressDialog(int panel)
{
    int         error = UIENoError;
    ProgressDlg progressDlg = 0;
    int         savedBole;

    savedBole = SetBOLE(FALSE);

    if (panel > 0)
        {
        errChk( GetPanelAttribute(panel, ATTR_CALLBACK_DATA, &progressDlg));
        free(progressDlg);
        DiscardPanel(panel);
        }

Error:
    SetBOLE(savedBole);
}
#endif  // HAVE_CVI_RTE

/*******************************************************************************/

    /*  returns TRUE if the user has tried to cancel the operation */
#if HAVE_CVI_RTE
int CVIFUNC UpdateProgressDialog(int panel, int percentDone, int processEvents)
{
    int         error = UIENoError;
    ProgressDlg progressDlg = 0;
    int         savedBole;
    int         cancelRequested = FALSE;

    savedBole = SetBOLE(FALSE);

    if (panel > 0)
        {
        errChk( GetPanelAttribute(panel, ATTR_CALLBACK_DATA, &progressDlg));

        if (progressDlg)
            {
            percentDone = (int)Pin(percentDone, 0, 100);
            errChk( SetCtrlVal(progressDlg->panel, progressDlg->progressIndicator, percentDone));
            if (processEvents)
                ProcessSystemEvents();
            cancelRequested = progressDlg->cancelRequested;
            if (cancelRequested)
                progressDlg->cancelRequested = FALSE;
            }
        }

Error:
    SetBOLE(savedBole);
    if (error < 0)
        return error;
    else
        return cancelRequested;
}
#endif  // HAVE_CVI_RTE

/*******************************************************************************/

static int CVICALLBACK ProgressCancelCallback(int panel, int ctrl, int event, void *callbackData, int eventData1, int eventData2)
{
    ProgressDlg progressDlg = (ProgressDlg)callbackData;

    UNUSED(panel);
    UNUSED(ctrl);
    UNUSED(eventData1);
    UNUSED(eventData2);

    switch (event)
        {
        case EVENT_COMMIT:
            if (progressDlg)
                progressDlg->cancelRequested = TRUE;
            break;
        }

    return 0;
}

/*******************************************************************************/

    /*  pass 0 for cancelButtonLabel if you don't wan't the dialog to be cancelable.
        pass "" for cancelButtonLabel if you want the dialog to recognized the ESC key but not
        display a cancel button.
    */
#if HAVE_CVI_RTE
int CVIFUNC CreateProgressDialog(const char title[], const char indicatorLabel[], int modal, int indicatorMarkerStyle, const char cancelButtonLabel[])
{
    int         error = UIENoError;
    int         panel = 0;
    ProgressDlg progressDlg = 0;
    int         ctrl;
    int         cancelBtn = 0;
    int         buttonHeight;
    int         buttonWidth;
    int         dlgWidth;
    int         labelHeight;
    int         top, left, height, width;
    int         dlgHeightWithoutCancel;

    dlgWidth = PROGRESS_BAR_WIDTH + 2 * PROGRESS_BOX_MARGIN;

    nullChk( progressDlg = (ProgressDlg)calloc(1, sizeof(*progressDlg)));

    errChk( panel = progressDlg->panel = NewPanel (0, title, 0, 0, 10, dlgWidth));

    errChk( SetPanelAttribute(panel, ATTR_CONFORM_TO_SYSTEM, TRUE));
    errChk( SetPanelAttribute (panel, ATTR_SYSTEM_MENU_VISIBLE, FALSE));
    errChk( SetPanelAttribute(panel, ATTR_CALLBACK_DATA, (void *)progressDlg));

    errChk( ctrl = progressDlg->progressIndicator = NewCtrl(panel, CTRL_NUMERIC_LEVEL_HSLIDE, indicatorLabel, 0, PROGRESS_BOX_MARGIN));

    if (cancelButtonLabel)
        {
        errChk( cancelBtn = progressDlg->cancelButton = NewCtrl(panel, CTRL_SQUARE_COMMAND_BUTTON, cancelButtonLabel, -100, -100));
        errChk( InstallCtrlCallback (panel, cancelBtn, ProgressCancelCallback, (void *)progressDlg));
        SetCtrlAttribute(panel, cancelBtn, ATTR_SHORTCUT_KEY, VAL_ESC_VKEY);
        }

    SetCtrlAttribute(panel, ctrl, ATTR_WIDTH, PROGRESS_BAR_WIDTH);
    SetCtrlAttribute(panel, ctrl, ATTR_HEIGHT, PROGRESS_BAR_HEIGHT);
    SetCtrlAttribute(panel, ctrl, ATTR_DATA_TYPE, VAL_INTEGER);
    SetCtrlAttribute(panel, ctrl, ATTR_MIN_VALUE, 0);
    SetCtrlAttribute(panel, ctrl, ATTR_MAX_VALUE, 100);

    SetCtrlAttribute(panel, ctrl, ATTR_SHOW_DIG_DISP, FALSE);
    SetCtrlAttribute(panel, ctrl, ATTR_SHOW_INCDEC_ARROWS, FALSE);
    SetCtrlAttribute(panel, ctrl, ATTR_MARKER_STYLE, indicatorMarkerStyle);
    if (indicatorMarkerStyle == VAL_NO_MARKERS)
        SetCtrlAttribute (panel, ctrl, ATTR_TICK_STYLE, VAL_NO_TICKS);
    SetCtrlAttribute(panel, ctrl, ATTR_SLIDER_COLOR, VAL_TRANSPARENT);
#if _NI_mswin32_
    SetCtrlAttribute(panel, ctrl, ATTR_FILL_COLOR, WinColorToCVIColor(GetSysColor(COLOR_HIGHLIGHT)));
#endif

    GetCtrlAttribute(panel, ctrl, ATTR_LABEL_HEIGHT, &labelHeight);
    SetCtrlAttribute(panel, ctrl, ATTR_TOP, PROGRESS_BOX_MARGIN + labelHeight + PROGRESS_LABEL_CTRL_V_GAP);

    SetCtrlAttribute(panel, ctrl, ATTR_LABEL_LEFT, PROGRESS_BOX_MARGIN);
    SetCtrlAttribute(panel, ctrl, ATTR_LABEL_TOP, PROGRESS_BOX_MARGIN);

    GetCtrlBoundingRect(panel, ctrl, &top, &left, &height, &width);

    dlgHeightWithoutCancel = height + 2 * PROGRESS_BOX_MARGIN;

    errChk( CenterCtrl(panel, ctrl, 0, 0, dlgHeightWithoutCancel, dlgWidth));

    if (cancelButtonLabel && *cancelButtonLabel)
        {
        GetCtrlAttribute(panel, cancelBtn, ATTR_HEIGHT, &buttonHeight);
        GetCtrlAttribute(panel, cancelBtn, ATTR_WIDTH, &buttonWidth);
        SetCtrlAttribute(panel, cancelBtn, ATTR_TOP, dlgHeightWithoutCancel);
        SetCtrlAttribute(panel, cancelBtn, ATTR_LEFT, dlgWidth / 2 - buttonWidth / 2);  /* center the button */
        SetPanelAttribute(panel, ATTR_HEIGHT, dlgHeightWithoutCancel + PROGRESS_BOX_MARGIN + buttonHeight);
        }
    else
        SetPanelAttribute(panel, ATTR_HEIGHT, height + 2*PROGRESS_BOX_MARGIN);

    SetPanelPos(panel, VAL_AUTO_CENTER, VAL_AUTO_CENTER);

    if (modal)
        InstallPopup(panel);
    else
        DisplayPanel(panel);

Error:
    if (error < 0)
        {
        DiscardProgressDialog(panel);
        return error;
        }
    else
        return panel;
}
#endif  // HAVE_CVI_RTE

/*********************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC GetFileWritability (const char pathName[], int *isWritable)
{
    FILE    *file;
    long        dummy;
    int     error = UIENoError;
    int     oldBreakPolicy = GetBreakOnLibraryErrors();

    DisableBreakOnLibraryErrors ();
    *isWritable = FALSE;
    switch (GetFileInfo (pathName, &dummy))  /* formatio.h */
        {
        case 1:  /* file exists */
            file = fopen (pathName, "r+b");
            if (file)
                {    /* can open it for read/write */
                fclose (file);
                *isWritable = TRUE;
                }
            break;
        case 0:  /* file does not exist */
            file = fopen (pathName, "wb");
            if (file)
                {    /* can create it and open it for writing */
                fclose (file);
                remove (pathName);
                *isWritable = TRUE;
                }
            break;
        case -1: /* too many file handles open */
            errChk (UIETooManyFilesOpen);
            break;
        default: /* should not occur */
            errChk (UIEIOError);
            break;
        }

Error:
    if (oldBreakPolicy)
        EnableBreakOnLibraryErrors ();
    return error;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/
/*
 * Create a temporary file in the same directory as the file identified
 * by pathName.  Use the prefix (which should be 5 characters in length),
 * followed by an index of up to three characters, followed by a dot,
 * followed by the extension (which should be from 1 to 3 characters in
 * length).  Returns the file pointer in tempFile, returns the name of the
 * temporary file in tempPathName.
 */

#define MAX_TEMP_PREFIX_LENGTH  5

#if HAVE_CVI_RTE
int CVIFUNC CreateAndOpenTemporaryFile (const char *pathName, const char *prefix, const char *extension,
                                const char *openModeString,
                                char tempPathName[], FILE **tempFile)

{
    char    driveName[MAX_DRIVENAME_LEN];
    char    dirName[MAX_DIRNAME_LEN];
    char    fileName[MAX_FILENAME_LEN];
    int     foundNameToUse=FALSE;
    long    dummy;
    int     index;
    FILE    *file;
    int     error = UIENoError;
    int     oldBreakPolicy = GetBreakOnLibraryErrors();
    char    prefixBuf[MAX_TEMP_PREFIX_LENGTH + 1];

    DisableBreakOnLibraryErrors ();

    StringCopyMax(prefixBuf, prefix, MAX_TEMP_PREFIX_LENGTH + 1);

    SplitPath (pathName, driveName, dirName, fileName);

    /*****************************************************************/
    /* Try to create a file in the same directory but with the       */
    /* prefix, followed by an index, followed by the extension.      */
    /* Try up to 1000 different indexes.                             */
    /* For each one, determine if a files exists under that name.    */
    /* If so, increment the index and try again.                     */
    /*****************************************************************/
    for (index=0; index < 1000 && !foundNameToUse; index++)
        {
        sprintf (tempPathName, "%s%s%s%1d.%s", driveName, dirName,
                                               prefixBuf, index, extension);
        switch (GetFileInfo (tempPathName, &dummy))  /* formatio.h */
            {
            case 1:  /* file exists */
                break;    /* increment index and try again */
            case 0:  /* file does not exist */
                foundNameToUse = TRUE;
                break;
            case -1: /* too many file handles open */
                errChk (UIETooManyFilesOpen);
                break;
            default: /* should not occur */
                errChk (UIEIOError);
                break;
            }
        }

    if (!foundNameToUse)
        errChk (ToolErr_CouldNotFindUnusedTempFileName);

    file = fopen (tempPathName, openModeString);
    if (file)
        *tempFile = file;
    else
        errChk (ToolErr_CouldNotCreateTempFile);

Error:
    if (oldBreakPolicy)
        EnableBreakOnLibraryErrors ();
    return error;
}
#endif  // HAVE_CVI_RTE

/*********************************************************************/

#if HAVE_CVI_RTE
void CVIFUNC RemoveFileIfExists (const char pathName[])
{
    int oldBreakPolicy = GetBreakOnLibraryErrors();

    DisableBreakOnLibraryErrors();
    remove (pathName);
    if (oldBreakPolicy)
        EnableBreakOnLibraryErrors();
}
#endif  // HAVE_CVI_RTE

/*********************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC DeleteAndRename (const char srcPathName[], const char destPathName[])
{
    int error;
    int oldBreakPolicy = GetBreakOnLibraryErrors();

    DisableBreakOnLibraryErrors();
    RemoveFileIfExists (destPathName);
    switch (RenameFile (srcPathName, destPathName))
        {
        case 0:   /* success */
            error = UIENoError;
            break;
        case -1:    /* src file not found */
            error = UIEFileWasNotFound;
            break;
        case -2:
            error = UIEAccessDenied;
            break;
        case -3:
            error = UIEIOError;
            break;
        case -4:
            error = UIEOutOfMemory;
            break;
        case -5:
            error = UIEBadPathnameMsg;
            break;
        case -6:
            error = UIEAccessDenied;
            break;
        case -8:
            error = UIEDiskFull;
            break;
        default:
            error = UIEIOError;
            break;
        }
    if (oldBreakPolicy)
        EnableBreakOnLibraryErrors();
    return error;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

    /* returns 1 if it exists, 0 if it doesn't, or an UIE error */
#if HAVE_CVI_RTE
int CVIFUNC FileExists(const char *pathName, int *fileSize)
{
    long    size = -1;
    int     result;
    int     savedBOLE = SetBOLE(FALSE);

    switch (GetFileSize (pathName, &size))
        {
        case 0: /* success */
            result = 1;
            break;
        case -1:    /* file not found */
            result = 0;
            break;
        case -3:    /* general I/O error */
            result = UIEIOError;
            break;
        case -4:    /* out of memory  */
            result = UIEOutOfMemory;
            break;
        default:
            result = UIEIOError;
            break;
        }

    SetBOLE(savedBOLE);
    if (fileSize)
        *fileSize = size;
    return result;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

    /*  Retrieves the size of the file in bytes.  Similar to the utility library
        function GetFileSize except that it works from a FILE pointer instead of
        a pathname.  The file must have been opened in binary mode for the results to
        be meaningful.
    */
int CVIFUNC BinaryFileSize(FILE *file, int *size)
{
    int error = UIENoError;
    int currPosition;
    int endPosition;

    currPosition = ftell(file);
    if (currPosition == -1 || errno == 100)
        errChk( UIEIOError);

    if (fseek (file, 0, SEEK_END) != 0)
        errChk( UIEIOError);

    endPosition = ftell(file);
    if (endPosition == -1 || errno == 100)
        errChk( UIEIOError);

    *size = endPosition;

    if (fseek (file, currPosition, SEEK_SET) != 0)  /* restore file position */
        errChk( UIEIOError);

Error:
    return error;
}

/**************************************************************************/

#define SEARCH_BUF_SIZE 128

    /*  returns TRUE if found, or FALSE if not, or a negative error code.
        If the bytesToFind are found, offset is set to how many bytes from the
        initial file position the bytesToFind were located at.
        If the bytesToFind were not found, then offset is set to the distance to
        one byte past the end of the file.
        The position parameter is set to the absolute file position of the
        beginning of the bytes found. If the bytes were not found then position
        is set to one byte past the end of the file.
        Both offset and position are optional and may be NULL.

        The file position is set to the byte after the found string,
        or to one byte past the end of the file.
    */
#if HAVE_CVI_RTE
int CVIFUNC FindBytesInFile(FILE *file, void *bytesToFind, int numBytesToFind, int *offset, int *position)
{
    int     error = UIENoError;
    char    buf[SEARCH_BUF_SIZE];
    char    *bufPtr = NULL;
    char    *unMatched;
    int     numLeft = 0;
    int     numBytesFound;
    int     numExamined = 0;
    int     savedBole = SetBOLE(FALSE);
    int     found = FALSE;
    int     startPosition;
    int     offsetToEndOfRead;

    numBytesFound = 0;
    unMatched = (char *)bytesToFind;

    startPosition = ftell(file);
    if (startPosition == -1 || errno == 100)
        errChk( UIEIOError);

    while (TRUE)
        {
        if (numLeft < 1)
            {
            numLeft  = fread (buf, 1, SEARCH_BUF_SIZE, file);
            if (numLeft == 0)
                break;
            else
                bufPtr = buf;
            }

        numExamined++;
        if  (*bufPtr == *unMatched)
            {
            unMatched++;
            numBytesFound++;
            if (numBytesFound == numBytesToFind)
                break;
            }
        else
            {
            unMatched = (char *)bytesToFind;
            numBytesFound = 0;
            }

        bufPtr++;
        numLeft--;
        }

    if (numBytesFound == numBytesToFind)
        {
        offsetToEndOfRead = numExamined - numBytesToFind;   /* back up to start to the bytes to find */
        found = TRUE;
        }
    else
        offsetToEndOfRead = numExamined;

    if (offset)
        *offset = offsetToEndOfRead;

    if (position)
        *position = startPosition + offsetToEndOfRead;

    if (found)
        {
        if (fseek(file, startPosition + offsetToEndOfRead + numBytesToFind, SEEK_SET) != 0)
            errChk( UIEIOError);
        }

    SetBOLE(savedBole);

Error:
    if (error < 0)
        return error;
    else
        return found;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#define READ_BUF_SIZE 128

    /*  returns 0 if successful, or a negative error code..
        The file offset is set to just past the last byte of the string read from the file .
        pass -1 to maxCharactersToRead to have no limit on the string length

        Use endOfFile to find out if the string was terminated by reaching the end of the file.
        Use maxCharactersRead to find out if the string in the file was longer than maxCharactersToRead.
    */
int CVIFUNC ReadStringFromBinaryFile(FILE *file, char **string, int maxCharactersToRead, int *endOfFile, int *maxCharactersRead)
{
    int     error = UIENoError;
    char    readBuf[READ_BUF_SIZE];
    char    *newString = 0;
    char    *stringEnd = NULL;
    int     numRead;
    int     numLeftToRead;
    int     numToRead;
    long    startingFilePos;
    int     totalBytesRead = 0;

    if (maxCharactersRead)
        *maxCharactersRead = FALSE;
    if (endOfFile)
        *endOfFile = FALSE;

    nullChk( newString = StrDup(""));

    numLeftToRead = maxCharactersToRead;

    startingFilePos = ftell(file);
    if (startingFilePos == -1 || errno == 100)
        errChk( UIEIOError);

    while (TRUE)
        {
        if (maxCharactersToRead < 0)
            numToRead = READ_BUF_SIZE;
        else
            numToRead =  Min(READ_BUF_SIZE, numLeftToRead);

        if (numToRead <= 0)
            {
            if (maxCharactersRead)
                *maxCharactersRead = TRUE;
            break;
            }

        numRead = fread (readBuf, 1, numToRead, file);

        if (numRead == 0)
            {
            if(endOfFile)
                *endOfFile = TRUE;
            break;
            }
        else
        if (maxCharactersToRead >= 0)
            numLeftToRead -= numRead;

        errChk( AppendString(&newString, readBuf, numRead));

        stringEnd = memchr(readBuf, 0, numRead);
        if (stringEnd)
            {
            totalBytesRead += stringEnd - readBuf + 1;  /* +1 for null terminator */
            break;
            }
        else
            totalBytesRead += numRead;
        }

        /* don't let incomplete string end with a lead byte */
    if (!stringEnd && (CmbStrByteType(newString, totalBytesRead-1) == CMB_LEAD_BYTE))
        newString[--totalBytesRead] = 0;

        /* set file position just after last byte read */
    if (fseek (file , startingFilePos + totalBytesRead, SEEK_SET) != 0)
        errChk( UIEIOError);

Error:
    if (error < 0)
        {
        free(newString);
        *string = 0;
        return error;
        }
    else
        {
        *string = newString;
        return 0;
        }
}

/**************************************************************************/

    /*  returns 0 if successful, or a negative error code..
        The file offset is set to just past the last byte of the string read from the file .
        pass -1 to maxCharactersToRead to have no limit on the string length

        Use endOfFile to find out if the string was terminated by reaching the end of the file.
        Use maxCharactersRead to find out if the string in the file was longer than maxCharactersToRead.
    */
#if HAVE_CVI_RTE
int CVIFUNC ReadLineFromBinaryFile(FILE *file, char **string, int maxCharactersToRead, int *endOfFile, int *maxCharactersRead)
{
    int error = UIENoError;
    int startingFilePos;
    int lineLength;
    int lineTerminatorOffset;
    int found;

    startingFilePos = ftell(file);
    if (startingFilePos == -1 || errno == 100)
        errChk( UIEIOError);

    errChk( found = FindBytesInFile(file, LINE_TERMINATOR, LINE_TERMINATOR_LEN, 0, &lineTerminatorOffset));

    if (fseek(file, startingFilePos, SEEK_SET) != 0)
        errChk( UIEIOError);

    if (found)
        {
        lineLength = lineTerminatorOffset - startingFilePos + LINE_TERMINATOR_LEN;
        if (maxCharactersToRead >= 0)
            maxCharactersToRead = Min(maxCharactersToRead, lineLength);
        else
            maxCharactersToRead = lineLength;
        }

    errChk( ReadStringFromBinaryFile(file, string, maxCharactersToRead, endOfFile, maxCharactersRead));

Error:
    if (error < 0)
        return error;
    else
        return 0;
}
#endif  // HAVE_CVI_RTE

/***********************************************************************************/

#define WRITE_BYTE_BUF_SIZE 64

int CVIFUNC WriteByteToFile(FILE *file, char byte, int numTimes)
{
    int     error = UIENoError;
    char    writeBuf[WRITE_BYTE_BUF_SIZE];
    int     numToWrite;
    int     numWritten;

    if (numTimes > 0)
        {
        memset (writeBuf, byte, Min(numTimes, WRITE_BYTE_BUF_SIZE));

        do
            {
            numToWrite = Min(WRITE_BYTE_BUF_SIZE, numTimes);

            numWritten = fwrite (writeBuf, 1, numToWrite, file);
            if (numWritten != numToWrite)
                errChk( UIEDiskFull);

            numTimes -= numWritten;
            } while (numTimes > 0);
        }

Error:
    return error;
}

/**************************************************************************/

char * CVIFUNC SkipDriveName(const char *pathString)
{
#if   defined(_NI_mswin_)
    if (pathString && strlen(pathString) >= 2 && CmbStrByteIs((unsigned char*)pathString, 1, ':'))
        pathString += 2;
#endif

    return (char*)pathString;
}

/**************************************************************************/

int CVIFUNC IsAbsolutePath(const char *pathString)
{
    int absolute = FALSE;
    int fileSepLength = strlen(FILE_SEPARATOR);
    int pathLen;

    if (pathString && *pathString)
        {
        pathString = SkipDriveName(pathString);
        pathLen = strlen(pathString);

        if (pathLen >= fileSepLength && memcmp(pathString, FILE_SEPARATOR, fileSepLength) == 0)
            absolute = TRUE;
        }

    return absolute;
}

/**************************************************************************/

int CVIFUNC WriteStringToFile (FILE *stream, const char *string)
{
    int     error = UIENoError;
    int     len, numWritten;
    char    *newLinePtr;

    while (string)
        {
        newLinePtr = (char *)CmbStrChr((const unsigned char *)string, '\n');

        if (newLinePtr)
            len = newLinePtr - string;
        else
            len = strlen(string);

        if (len > 0)
            {
            numWritten = fwrite (string, 1, len, stream);
            if (numWritten != len)
                errChk( UIEDiskFull);
            }

        if (newLinePtr)
            {
            len = strlen (LINE_TERMINATOR);
            numWritten = fwrite (LINE_TERMINATOR, 1, len, stream);
            if (numWritten != len)
                errChk( UIEDiskFull);

            newLinePtr++;   /* skip over newline */
            }

        string = newLinePtr;
        }

Error:
    return error;
}

/**************************************/
#if HAVE_CVI_RTE
int CVIFUNC SaveBitmapToFile (const char PathName[], int BmpId)
{
    int                     ImageHeight, ImageWidth, RowBytes, PixelDepth, BytesPerLine;
    int                     *ColorTable = NULL, NbBytesColorTable, NbColors;
    int                     NbBytesImage;
    unsigned char           *BitsImage, *TrueBitsImage;
    int                     error;

    int                     i, j;
    RRGGBB                  *Ptr;
    unsigned char           Dummy;
    unsigned char           *PtrSource, *PtrTarget, *PtrDummy;

    FILE                    *Stream;
    BITMAPFILEHEADER        BmpFileHeader;
    BITMAPINFOHEADER        BmpInfoHeader;


    errChk (GetBitmapInfo (BmpId, &NbBytesColorTable, &NbBytesImage, NULL));
    BitsImage = (unsigned char*)malloc(NbBytesImage);

    if (NbBytesColorTable)  /* Pixel Depth <=8 */
        ColorTable = (int*)malloc(NbBytesColorTable);
    errChk (GetBitmapData (BmpId, &RowBytes, &PixelDepth, &ImageWidth, &ImageHeight, ColorTable, BitsImage, NULL));

    NbColors = (int)pow(2,PixelDepth);

    /* Z! Modulo 4 */
    BytesPerLine = 4*((RowBytes+3)/4);

    /* Initialize the fields in the BITMAPINFO structure */
    BmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    BmpInfoHeader.biWidth = ImageWidth;
    BmpInfoHeader.biHeight = ImageHeight; /* I can't use negative number, why ????  */
    BmpInfoHeader.biPlanes = 1;
    BmpInfoHeader.biBitCount = PixelDepth;
    BmpInfoHeader.biCompression = BI_RGB;
    BmpInfoHeader.biSizeImage = BytesPerLine*ImageHeight;
    BmpInfoHeader.biXPelsPerMeter = 0;
    BmpInfoHeader.biYPelsPerMeter = 0;

    if (PixelDepth<24)
        BmpInfoHeader.biClrUsed = NbColors;

    else
        BmpInfoHeader.biClrUsed = 0;

    BmpInfoHeader.biClrImportant = 0; /* All of the device colors are important.  */

    BmpFileHeader.bfType = 0x4d42;  /* Signature 0x42="B" 0x4d="M"   */

    /* Compute the size of the entire file */
    BmpFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + NbBytesColorTable + BmpInfoHeader.biSizeImage;
    BmpFileHeader.bfReserved1 = 0;
    BmpFileHeader.bfReserved2 = 0;

    /* Compute the offset to the array of color indices */
    BmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + NbBytesColorTable;

    /* Open the file */
    Stream = fopen (PathName, "wb+");


    /* Copy the BITMAPFILEHEADER */
    errChk (fwrite ((void*)&BmpFileHeader, sizeof(BITMAPFILEHEADER), 1, Stream));

    /* Copy the BITMAPINFOHEADER */
    errChk (fwrite ((void*)&BmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, Stream));

    /* Copy ColorTable into the file */
    if (PixelDepth<24)
        errChk (fwrite ((void*)ColorTable, sizeof(int), NbColors , Stream));

    /* Create (in memory) the image to be copied on file */
    /* Working that way allow us to save the array with fwrite function */
    TrueBitsImage = (unsigned char*)calloc(BmpInfoHeader.biSizeImage+BytesPerLine,sizeof(unsigned char));


    /* We copy each scan line already filled with 0 */
    PtrSource = BitsImage;
    PtrTarget = TrueBitsImage;

    for (i=0; i<ImageHeight; i++)
    {
        memcpy (PtrTarget, PtrSource, RowBytes);
        PtrTarget+=BytesPerLine;
        PtrSource+=RowBytes;
    }

    /*Since colors must be saved in BBGGRR format instead of RRGGBB ...  */
    if(PixelDepth==24)
        {
        for (j=0; j<ImageHeight; j++)
            {
            Ptr = (RRGGBB*)(TrueBitsImage+j*BytesPerLine);
            for (i=0; i<ImageWidth; i++)
                {
                Dummy = Ptr->RR;
                Ptr->RR = Ptr->BB;
                Ptr->BB = Dummy;
                Ptr++;
                }
            }
        }

    /* I should not do the next few line but I'm not able to use negative value */
    /* for biHeight property so I need to upside down the array by hand ...     */
    /* This code should be remove as soon as I can understand what happen       */
    /* with biHeight                                                            */

    /* PtrSource points to the beginning of the last scan line                  */
    PtrSource = TrueBitsImage + ((ImageHeight-1)*BytesPerLine);

    /* PtrTarget points to the beginning of the first scan line                 */
    PtrTarget = TrueBitsImage;
    PtrDummy = (unsigned char*)malloc(BytesPerLine);
    for (i=0; i<ImageHeight/2; i++)
        {
        memcpy (PtrDummy, PtrTarget, BytesPerLine);
        memcpy (PtrTarget, PtrSource, BytesPerLine);
        memcpy (PtrSource, PtrDummy, BytesPerLine);
        PtrTarget+=BytesPerLine;
        PtrSource-=BytesPerLine;
        }


    /* Copy the array of color indices into the BMP file. */
    errChk (fwrite ((void*)TrueBitsImage, sizeof(unsigned char), BmpInfoHeader.biSizeImage, Stream));

    /* Close the BMP file */
    errChk (fclose (Stream));

Error:

    /*Free memory. */
    free(PtrDummy);
    free(TrueBitsImage);

    if (PixelDepth<24)
        free(ColorTable);

    free(BitsImage);

    return error;
}
#endif  // HAVE_CVI_RTE

/**************************************/

#if HAVE_CVI_RTE
int CVIFUNC SaveCtrlDisplayToFile (int panelHandle, int ctrlId, int include_label,
                                        int height, int width, const char PathName[])
{
    int BmpId;
    int error = UIENoError;
    errChk (GetScaledCtrlDisplayBitmap (panelHandle, ctrlId, include_label, height,
                                          width, &BmpId));

    errChk (SaveBitmapToFile (PathName, BmpId));

Error:
    return error;
}
#endif  // HAVE_CVI_RTE

/**************************************/

#if HAVE_CVI_RTE
int CVIFUNC SavePanelDisplayToFile (int panelHandle, int scope, Rect area,
                                        int height, int width, const char PathName[])
{
    int BmpId;
    int error = UIENoError;
    errChk (GetScaledPanelDisplayBitmap (panelHandle, scope, area, height,
                                           width, &BmpId));

    errChk (SaveBitmapToFile (PathName, BmpId));

Error:
    return error;
}
#endif  // HAVE_CVI_RTE

/**************************************/

void CVIFUNC ChangeFileNameExtension(const char *fileName, char *newFileName, const char *extension)
    /* fileName may have the same buffer as newFileName */
    /* extension should not include the '.' */
{
    char *dest;

    if( !fileName || !newFileName || !extension )
        AssertMsg(0, "Null parameters passed to ChangeFileNameExtension");

    strcpy (newFileName, fileName);
    dest = FindFileDotPosition (newFileName);
    *dest++ = '.';      /* add the '.' in case there was not one already */
    strcpy (dest, extension);
}

/**************************************/

    /* does not preserve the extension, basename should include an extension if needed */
void CVIFUNC ChangeBaseFileName(const char *pathString, char *newPathString, const char *baseName)
{
    int len;

    len = FindFileName(pathString) - pathString;
    memmove(newPathString, pathString, len);
    strcpy (newPathString + len, baseName);
}

/**************************************/

char * CVIFUNC FindFileDotPosition (const char *pathString)
{
 /* This function takes a string and returns a pointer to the last '.' within
    kMaxFileExtension bytes from the end of the string.   If it cannot find a dot, then
    returns a pointer to the terminating nul byte of the string */

    char *p, *prev;

    AssertMsg(pathString, "Null pathString parameter passed to FindFileDotPosition");

    p = FindFileExtension (pathString);
    prev = (char *)CmbStrPrev (pathString, p);
    if ((prev != NULL) && (*prev == '.'))
        return prev;
    else
        return p;
}

/**************************************/

#if defined(_NI_mswin16_)
#define MAX_FILE_EXTENSION_LENGTH   3
#elif defined(_NI_mswin32_)
#define MAX_FILE_EXTENSION_LENGTH   254
#elif defined(_NI_unix_) || defined(_NI_sparc_)
#define MAX_FILE_EXTENSION_LENGTH   253
#elif defined(_NI_mac_)
#define MAX_FILE_EXTENSION_LENGTH   253
#else
#error Undefined platform
#endif

/*  This function takes a string and returns a pointer to the character after
    the last '.' in the string IF the last  '.' is within
    MAX_FILE_EXTENSION_LENGTH characters of the end of the string.
    Else, it returns returns a pointer to the terminating nul byte of the string
*/
char * CVIFUNC FindFileExtension(const char *pathString)
{
    int index;
    char *fileName;
    char *ptr, *terminatorPtr;

    AssertMsg(pathString, "Null pathString parameter passed to FindFileExtension");

    fileName = FindFileName(pathString);

    if ((index = strlen (fileName)) == 0)
        return fileName;

    terminatorPtr = fileName + index;

    ptr = (char *)CmbStrPrev (fileName, terminatorPtr);

    while (ptr && ((terminatorPtr-ptr) <= MAX_FILE_EXTENSION_LENGTH+1))
        {
        if (*ptr == '.' && ptr != fileName) /* if the dot is the first character in filename, */
            return ++ptr;                   /* it really is not an extension */
        CmbStrDec (fileName, (const unsigned char *)ptr);
        }

    return terminatorPtr;
}

/*********************************************/

 /* This function takes a string and returns a pointer to the character after last file separator
    in the string or to the string itself if no separators are found */

char * CVIFUNC FindFileName(const char *pathString)
{
    int         len;
    const char  *p;

    AssertMsg(pathString, "Null pathString parameter passed to FindFileName");

    if ((len = strlen(pathString)) == 0)
        return (char*)pathString;

    p = (const char *)CmbStrPrev(pathString, pathString+len);

    while (p != NULL)
        {
#if   defined(_NI_mswin_)
        if ((*p == *FILE_SEPARATOR) || (*p == ':'))    /* i.e. c:x.c should return x.c on PC */
#else
        if (*p == *FILE_SEPARATOR)
#endif
            return (char*) ++p;
        CmbStrDec (pathString, (const unsigned char *)p);
        }

    return (char*)pathString;
}

/**************************************************************************/

#define CVINIERR_INVALID_INPUT                  -1
#define CVINIERR_OUT_OF_MEMORY                  -2
#define CVINIERR_CANNOT_OPEN_FILE               -3
#define CVINIERR_WRONG_FILE_FORMAT              -4

#define cviNiErrChk(f) if (error = (f), error < 0) goto Error; else

static int CVINiErrParseFile (const char *fileName, ListType errInfoList)
{
    int             error = 0;
    FILE            *file = NULL;
    char            buf[257], *pc = NULL;
    CVINiErrInfo    errInfo;
    
    assert (errInfoList);

    errInfo.errorString = NULL;

    if (!fileName || !errInfoList)
        cviNiErrChk (CVINIERR_INVALID_INPUT);
    
    if (NULL == (file = fopen (fileName, "r")))
        cviNiErrChk (CVINIERR_CANNOT_OPEN_FILE);
            
    pc = fgets (buf, sizeof (buf) - 1, file);
    if (pc && _mbsnbicmp ((const unsigned char *)"<?xml version=\"1.0\">", (unsigned char *)buf, 19) == 0) {
        pc = fgets (buf, sizeof (buf) - 1, file);
        if (pc && _mbsnbcmp ((const unsigned char *)"<nidocument>", (unsigned char *)buf, 12) == 0) {
            pc = fgets (buf, sizeof (buf) - 1, file);
            while (pc && _mbsnbcmp ((const unsigned char *)"</nidocument>", (unsigned char *)buf, 13) != 0) {
                if (pc && _mbsnbcmp ((const unsigned char *)"<nierror code=\"", (unsigned char *)buf, 15) == 0) {
                    // read code field
                    int numread = sscanf (&buf[15], "%d", &errInfo.errorCode);
                    errInfo.errorString = NULL;
                    if (numread <= 0) {
                        cviNiErrChk (CVINIERR_WRONG_FILE_FORMAT);
                    } else {
                        pc = fgets (buf, sizeof (buf) - 1, file);
                        while (pc && _mbsnbcmp ((const unsigned char *)"</nierror>", (unsigned char *)buf, 10) != 0) {
                            RemoveSurroundingWhiteSpace (buf);
                            if (0 == AppendString (&errInfo.errorString, buf, -1))
                                cviNiErrChk (CVINIERR_OUT_OF_MEMORY);
                            pc = fgets (buf, sizeof (buf) - 1, file);
                        }
                        if (pc && _mbsnbcmp ((const unsigned char *)"</nierror>", (unsigned char *)buf, 10) == 0) {
                            if (ListInsertItem (errInfoList, &errInfo, END_OF_LIST) < 0)
                                cviNiErrChk (CVINIERR_OUT_OF_MEMORY);
                            errInfo.errorString = NULL;
                        } else {
                            cviNiErrChk (CVINIERR_WRONG_FILE_FORMAT);
                        }
                    }
                } else if (pc && _mbsnbcmp ((const unsigned char *)"<nicomment>", (unsigned char *)buf, 11) ==0) {
                    // loop and dispose of lines until /nicomment found
                    while (pc && _mbsnbcmp ((const unsigned char *)"</nicomment>", (unsigned char *)buf, 12) != 0) {
                        pc = fgets(buf, sizeof (buf) - 1, file);
                    }
                } else {
                    cviNiErrChk (CVINIERR_WRONG_FILE_FORMAT);
                }
                pc = fgets (buf, sizeof (buf) - 1, file);
            }
        }
    }
Error:
    if (errInfo.errorString)
        free (errInfo.errorString);
    if (file)
        fclose (file);
    return error;
}

static int  CVINiErrParseFilesIntoList (ListType fileList, ListType errorInfoList)
{
    int         error = 0;
    int         i, numFiles;
    
    if (fileList == NULL || errorInfoList == NULL)
        cviNiErrChk (CVINIERR_INVALID_INPUT);
    
    numFiles = ListNumItems (fileList);
    Assert (numFiles > 0);
    for (i = 1; i <= numFiles; ++i) {
        char *file = NULL;
        ListGetItem (fileList, &file, i);
        Assert (file);
        cviNiErrChk (CVINiErrParseFile (file, errorInfoList));
    }

Error:
    return error;
}

static void CVINiErrDisposeList (ListType *errInfoListPtr)
{
    if (errInfoListPtr) {
        if (*errInfoListPtr) {
            int i, numItems = ListNumItems (*errInfoListPtr);
            for (i = 0; i < numItems; ++i) {
                CVINiErrInfo errInfo;
                ListRemoveItem (*errInfoListPtr, &errInfo, END_OF_LIST);
                if (errInfo.errorString)
                    free (errInfo.errorString);
            }
            ListDispose (*errInfoListPtr);
            *errInfoListPtr = NULL;
        }
    }
}

static int CVICALLBACK CVINiErrInfoCompareFunc (void *item1, void *item2)
{
    CVINiErrInfo    *errInfo1 = (CVINiErrInfo*) item1,
                    *errInfo2 = (CVINiErrInfo*) item2;
                    
    return IntCompare (&errInfo1->errorCode, &errInfo2->errorCode);
}

static char * GetCVINiErrString (int errorCode)
{
    int                 locked = 0;
    static const char   *kEnglishFile = "cvirte\\bin\\English\\cvierror.txt";
    static const char   *kJapaneseFile = "cvirte\\bin\\Japanese\\cvierror.txt";
    static const char   *kFrenchFile = "cvirte\\bin\\French\\cvierror.txt";
    static const char   *kGermanFile = "cvirte\\bin\\German\\cvierror.txt";
    static const char   *kOtherFile = "cvirte\\bin\\cvierror.txt";
    
    locked = GetLock (kErrorListLock);
    
    if (gCVIErrorList == NULL)
        {
        HANDLE  hCVIRTE;
        char    cvirtePathBuf[512];
        
        hCVIRTE = GetModuleHandle ("cvirte.dll");
        if (hCVIRTE && 0 != GetModuleFileName (hCVIRTE, cvirtePathBuf, sizeof (cvirtePathBuf)))
            {
            char    *pc = (char *)_mbsrchr ((const unsigned char *)cvirtePathBuf, '\\');
            
            if (pc && NULL != (gCVIErrorList = ListCreate (sizeof (CVINiErrInfo))))
                {
                int primaryLangId = PRIMARYLANGID (LANGIDFROMLCID (GetUserDefaultLCID ()));
                pc = (char *)_mbsinc ((unsigned char *)pc);
                switch (primaryLangId)
                    {
                    case LANG_JAPANESE:     strcpy (pc, kJapaneseFile); break;
                    case LANG_GERMAN:       strcpy (pc, kGermanFile);   break;
                    case LANG_FRENCH:       strcpy (pc, kFrenchFile);   break;
                    default:                
                        primaryLangId = LANG_ENGLISH;
                        strcpy (pc, kEnglishFile);
                        break;
                    }
                if (primaryLangId != LANG_ENGLISH)
                    {
                    if (!FileExists (cvirtePathBuf, NULL))
                        {
                        primaryLangId = LANG_ENGLISH;
                        strcpy (pc, kEnglishFile);
                        }
                    }
                if (!FileExists (cvirtePathBuf, NULL))
                    {
                    strcpy (pc, kOtherFile);
                    }
                if (CVINiErrParseFile (cvirtePathBuf, gCVIErrorList) < 0)
                    {
                    CVINiErrDisposeList (&gCVIErrorList);
                    }
                else
                    {
                    ListQuickSort (gCVIErrorList, CVINiErrInfoCompareFunc);
                    }
                }
            }
            
        if (gCVIErrorList == NULL)
            {
            gCVIErrorList = (void*)-1;
            }
        }
    
    if (locked)
        {
        ReleaseLock (kErrorListLock);
        }

    if (gCVIErrorList != NULL && gCVIErrorList != (void*)-1)
        {
        CVINiErrInfo    errInfo;        
        int             index;
        
        errInfo.errorCode = errorCode;
        index = ListBinSearch (gCVIErrorList, &errInfo, CVINiErrInfoCompareFunc);
        if (index > 0)
            {
            CVINiErrInfo *errInfoPtr = ListGetPtrToItem (gCVIErrorList, index);
            if (errInfoPtr)
                return errInfoPtr->errorString;
            }
        }
    
    return NULL;
}

char * CVIFUNC GetToolboxErrorString(int errorCode)
{
    int     absError = ABS_VAL(errorCode);
    char    *cviNiErrString;        

    if ((absError >= 5000 && absError <= 5299) || (absError >= 6000 && absError <= 6199))
        if (NULL != (cviNiErrString = GetCVINiErrString (errorCode)))
            return cviNiErrString;

    /* OLD CODE */
    switch (-absError)
        {
        case ToolErr_CouldNotOpenFileForReading:
            return  "Could Not Open File For Reading";
        case ToolErr_ErrorReadingFile:
            return "Error Reading From File";
        case ToolErr_CouldNotFindUnusedTempFileName :
            return "Could Not Find Unused Temporary File Name";
        case ToolErr_CouldNotCreateTempFile:
            return "Could Not Create Temporary File";
        case ToolErr_IncorrectFileFormat:
            return "Incorrect File Format";
        case ToolErr_UnexpectedEndOfFile:
            return "Unexpected End Of File";
        case ToolErr_InvalidIntNumber:
            return "Invalid Integer Number";
        case ToolErr_InvalidUIntNumber:
            return "Invalid Unsigned Integer Number";
        case ToolErr_InvalidDoubleNumber:
            return "Invalid Floating Point Number";
        case ToolErr_InvalidBooleanValue:
            return "Invalid Boolean Value";
        case ToolErr_MissingSection:
            return "Missing Section";
        case ToolErr_MissingItem:
            return "Missing Item";
        case ToolErr_UnRecognizedValue:
            return "Unrecognized Value";
        case ToolErr_OperationInProgress:
            return "Could not perform requested action because of an operation currently in progress";
        case ToolErr_TooManyItems:
            return "Too many items";
        case ToolErr_DuplicateItemOrValue:
            return "Duplicate item or value";
        case ToolErr_PanelNotAChildOfCorrectPanel:
            return "Panel is not a child panel of the correct panel";
        case ToolErr_CantOpenKey:
            return "Could not open Registry Key";
        case ToolErr_CantSetKeyValue:
            return "Could not set data for Registry Value";
        case ToolErr_CantAccessKeyValue:
            return "Could not access Registry Value";
        case ToolErr_MissingKeyValue:
            return "Registry Value does not exist";
        case ToolErr_WrongKeyValueType:
            return "Incorrect type for Registry Value";
        case ToolErr_MissingKey:
            return "Registry Key does not exist";
        case ToolErr_OutOfMemory:
            return "Out of memory";
        case ToolErr_InvalidDisplayElement:
            return "Invalid display item";
        case ToolErr_MsgCallbackNotInstalled:
            return "Message-callback not installed";
        case ToolErr_MsgCallbackAlreadyInstalled:
            return "Message-callback already installed.";
        case ToolErr_PanelNotParent:
            return "Panel not top-level.";
        case ToolErr_NoPanelCallback:
            return "Panel does not have a callback function.";
        case ToolErr_DragDropNotEnabled:
            return "Drag-and-Drop not enabled for panel";
        case ToolErr_InvalidMsgCallbackMode:
            return "Invalid message-callback execution-mode";
        case ToolErr_NoMenuForTrayIcon:
            return "No menu for Tray Icon";
        case ToolErr_InvalidTrayIconMenuItem:
            return "Invalid item for Tray Icon popup menu";
        case ToolErr_InvalidTrayIcon:
            return "Invalid Tray Icon";
        case ToolErr_ErrorLoadingIconFileForTray:
            return "Could not load icon from file";
        case ToolErr_ErrorLoadingDLL:
            return "Could not load required system DLL";
        case ToolErr_FuncNotFound:
            return "Required function not found in system DLL";
        case ToolErr_UnknownSystemError:
            return "Unknown system error";
        case ToolErr_FileAssociationNotAvailable:
            return "There is no application associated with the given file name extension";
        case LocalUI_InvalidOrCorruptLangFile:
            return "Invalid or corrupt language file";
        default:
            return "No description available. ";
        }
}

/**************************************************************************/

#if HAVE_CVI_RTE
char * CVIFUNC GetGeneralErrorString(int errorCode)
{
    int                 absError = ABS_VAL(errorCode);
    int                 locked = 0;
    static const char   *kEnglishFile = "English\\*.txt";
    static const char   *kJapaneseFile = "Japanese\\*.txt";
    static const char   *kFrenchFile = "French\\*.txt";
    static const char   *kGermanFile = "German\\*.txt";
    static const char   *kErrorsDirRegLoc = "Software\\National Instruments\\Common\\Installer";
    static const char   *kErrorsDirValueName = "NIERRORSDIR";
    
    locked = GetLock (kErrorListLock);
    
    if (gSharedNIErrorList == NULL)
        {
        unsigned int    size;
        char            errorFilesPathBuf[MAX_PATHNAME_LEN],
                        errorFileBuf[MAX_FILENAME_LEN];
        
        if (0 == RegReadString (REGKEY_HKLM, kErrorsDirRegLoc, kErrorsDirValueName,
         (unsigned char *)errorFilesPathBuf, sizeof (errorFilesPathBuf), &size))
            {
            char    *pc = errorFilesPathBuf + strlen (errorFilesPathBuf);
            
            if (*_mbsdec ((const unsigned char *)errorFilesPathBuf, (unsigned char *)pc) != '\\')
                strcat (errorFilesPathBuf, "\\");
            
            switch (PRIMARYLANGID (LANGIDFROMLCID (GetUserDefaultLCID ())))
                {
                case LANG_JAPANESE: strcat (errorFilesPathBuf, kJapaneseFile);  break;
                case LANG_GERMAN:   strcat (errorFilesPathBuf, kGermanFile);    break;
                case LANG_FRENCH:   strcat (errorFilesPathBuf, kFrenchFile);    break;
                default:            strcat (errorFilesPathBuf, kEnglishFile);   break;
                }
            
            pc = (char *)_mbsrchr ((const unsigned char *)errorFilesPathBuf, '\\');
            pc = (char *)_mbsinc ((unsigned char *)pc);
            if (0 == GetFirstFile (errorFilesPathBuf, 1, 1, 1, 1, 1, 0, errorFileBuf))
                {
                ListType    fileList = NULL;
                int         outOfMemory = 0;
                
                if (NULL != (fileList = ListCreate (sizeof (char*))))
                    {
                    do
                        {
                        char *file = NULL;
                        strcpy (pc, errorFileBuf);
                        file = StrDup (errorFilesPathBuf);
                        if (file == NULL)
                            {
                            outOfMemory = 1;
                            break;
                            }
                        else
                            {
                            if (0 == ListInsertItem (fileList, &file, END_OF_LIST))
                                {
                                free (file);
                                outOfMemory = 1;
                                break;
                                }
                            }
                        } while (0 == GetNextFile (errorFileBuf));
                        
                    if (!outOfMemory 
                     && NULL != (gSharedNIErrorList = ListCreate (sizeof (CVINiErrInfo))))
                        {
                        if (CVINiErrParseFilesIntoList (fileList, gSharedNIErrorList) < 0)
                            {
                            CVINiErrDisposeList (&gSharedNIErrorList);
                            }
                        else
                            {
                            ListQuickSort (gSharedNIErrorList, CVINiErrInfoCompareFunc);
                            }
                        }
                        
                    ListDisposePtrList (fileList);
                    fileList = NULL;
                    }
                }
            }
            
        if (gSharedNIErrorList == NULL)
            {
            gSharedNIErrorList = (void*)-1;
            }
        }
    
    if (locked)
        {
        ReleaseLock (kErrorListLock);
        }

    if (absError < ABS_VAL (UIEErrorLimit))
        return GetUILErrorString(errorCode);
    else
    if ((absError >= 5000 && absError <= 5299) || (absError >= 6000 && absError <= 6199))
        return GetToolboxErrorString(errorCode);
    else
        {
        char *cviNiErrString = GetCVINiErrString(errorCode);
        if (cviNiErrString)
            return cviNiErrString;
        else
#if _NI_mswin16_ || _NI_mswin32_
        if (absError >= 10000 && absError < 11000)
            return ToolBox_GetDAQErrorString((short int)errorCode);
        else
#endif /* _NI_mswin16 || _NI_mswin32_ */
        if (gSharedNIErrorList != NULL && gSharedNIErrorList != (void*)-1)
            {
            CVINiErrInfo    errInfo;        
            int             index;
        
            errInfo.errorCode = errorCode;
            index = ListBinSearch (gSharedNIErrorList, &errInfo, CVINiErrInfoCompareFunc);
            if (index > 0)
                {
                CVINiErrInfo *errInfoPtr = ListGetPtrToItem (gSharedNIErrorList, index);
                if (errInfoPtr)
                    return errInfoPtr->errorString;
                }
            }
        }

        return "No description available. ";
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

static ListType gExistenceList = 0;

    /*  returns non-zero if success, or 0 if out of memory */
#if HAVE_CVI_RTE
int CVIFUNC RegisterExistence(const void *objectPtr)
{
    if (!gExistenceList)
        {
        gExistenceList = ListCreate(sizeof(void *));
        if (gExistenceList)
            {
            atexit(ToolBoxAtExitFunction);
            }
        }

    if (gExistenceList)
        return ListInsertInOrder (gExistenceList, &objectPtr, 0);
    else
        return 0;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

void CVIFUNC DeRegisterExistence(void *objectPtr)
{
    if (gExistenceList && objectPtr)
        {
        int index;

        index = ListBinSearch (gExistenceList, &objectPtr, 0);
        if (index > 0)
            ListRemoveItem (gExistenceList, 0, index);
        }
}

/**************************************************************************/

int CVIFUNC ObjectExists(const void *objectPtr)
{
    if (gExistenceList && objectPtr)
        {
        int index;

        index = ListBinSearch (gExistenceList, &objectPtr, 0);
        return index > 0;
        }
    else
        return FALSE;
}

/**************************************************************************/

    /*  dispose and resources allocated by the programmers toolbox */
#if HAVE_CVI_RTE
static void CVIANSI ToolBoxAtExitFunction(void)
{
    if (gExistenceList)
        {
        /* ListDispose calls free().  If building with CVI, cvirte.dll's free */
        /* is used.  If building with an external compiler, the external      */
        /* compiler's free is used.  We need to avoid calling cvirte.dll      */
        /* if it has already been detached.                                   */
        if (!CVIRTEHasBeenDetached())
            ListDispose(gExistenceList);
        gExistenceList = 0;
        }
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

    /*  Only one link allowed per type.  If already linked with the same type then UIEInvalidControlType
        is returned.  If successfully linked, 0 is returned, otherwise a negative error
        code is returned.

        Currently this is no support for deleting or replacing a link (could be added in future).
        All links are destroyed when the control is discarded.
    */
#if HAVE_CVI_RTE
int CVIFUNC ChainCtrlCallback(int panel, int ctrl, CtrlCallbackPtr newCallback, void *newCallbackData, const char *type)
{
    int                     error = UIENoError;
    CtrlCallbackChainLink   link = 0;
    int                     alreadyLinked = FALSE;
    int                     insertedInList = FALSE;
    int                     locked = FALSE;

    if (!type)
        errChk( UIENullPointerPassed);

    locked = GetLock(kChainCallbackLock);

    errChk( alreadyLinked = FindCtrlCallbackLink(panel, ctrl, type, 0));

    if  (!alreadyLinked)
        {
        InstallObjectDeletionProcedure();
        errChk( RecordPanelWithLinkedCallback(panel));
        insertedInList = TRUE;
        nullChk( link = (CtrlCallbackChainLink)calloc(sizeof(*link),1));
        nullChk( link->type = StrDup(type));
        link->signature = CALLBACK_LINK_SIGNATURE;

            /*  nothing passed this point should fail if the code is working as intended */
        errChk( GetCtrlAttribute(panel, ctrl, ATTR_CALLBACK_FUNCTION_POINTER, &link->previousCallback));
        errChk( GetCtrlAttribute(panel, ctrl, ATTR_CALLBACK_DATA, &link->previousCallbackData));

        link->callback = newCallback;
        link->callbackData = newCallbackData;

        errChk( InstallCtrlCallback(panel, ctrl, LinkedCtrlCallback, (void *)link));
        }
    else
        errChk( UIEInvalidControlType);

Error:
    if (error < 0)
        {
        if (insertedInList)
          UnrecordPanelWithLinkedCallback(panel);
        if (link)
            {
            free(link->type);
            free(link);
            }

        if (locked)
            ReleaseLock(kChainCallbackLock);
        return error;
        }
    else
        {
        if (locked)
            ReleaseLock(kChainCallbackLock);
        return 0;
        }
}
#endif  // HAVE_CVI_RTE

/*************************************************************************/

    /*  returns  UIEInvalidControlType if the control is not linked with the specified type */
#if HAVE_CVI_RTE
int CVIFUNC GetChainedCallbackData(int panel, int ctrl, const char *type, void **callbackData)
{
    int                     error = UIENoError;
    int                     found = FALSE;
    CtrlCallbackChainLink   link;
    int                     locked = FALSE;

    locked = GetLock(kChainCallbackLock);

    if (type && !*type)     /* "" means get original data */
        errChk( GetOriginalCtrlCallbackData(panel, ctrl, callbackData));
    else
        {
        errChk( found = FindCtrlCallbackLink(panel, ctrl, type, &link));

        if (found)
            *callbackData = link->callbackData;
        else
            errChk( UIEInvalidControlType);
        }

Error:
    if (locked)
        ReleaseLock(kChainCallbackLock);
    if (error < 0)
        return error;
    else
        return 0;
}
#endif  // HAVE_CVI_RTE

/*************************************************************************/

    /*  returns  UIEInvalidControlType if the control is not linked with the specified type */
#if HAVE_CVI_RTE
int CVIFUNC SetChainedCallbackData(int panel, int ctrl, const char *type, void *callbackData)
{
    int                     error = UIENoError;
    int                     found = FALSE;
    CtrlCallbackChainLink   link;
    int                     locked = FALSE;

    locked = GetLock(kChainCallbackLock);

    if (type && !*type)     /* "" means set original data */
        errChk( SetOriginalCtrlCallbackData(panel, ctrl, callbackData));
    else
        {
        errChk( found = FindCtrlCallbackLink(panel, ctrl, type, &link));

        if (found)
            link->callbackData = callbackData;
        else
            errChk( UIEInvalidControlType);
        }

Error:
    if (locked)
        ReleaseLock(kChainCallbackLock);
    if (error < 0)
        return error;
    else
        return 0;
}
#endif  // HAVE_CVI_RTE

/*************************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC UnchainCtrlCallback(int panel, int ctrl, const char *type)
{
    int                     error = UIENoError;
    CtrlCallbackPtr         callbackFunc;
    int                     found = FALSE;
    CtrlCallbackChainLink   currLink;
    CtrlCallbackChainLink   prevLink = NULL;
    int                     savedBole = SetBOLE(FALSE);
    int                     locked = FALSE;

    if (!type)
        errChk( UIENullPointerPassed);

    locked = GetLock(kChainCallbackLock);

    errChk( GetCtrlAttribute(panel, ctrl, ATTR_CALLBACK_FUNCTION_POINTER, &callbackFunc));
    errChk( GetCtrlAttribute(panel, ctrl, ATTR_CALLBACK_DATA, &currLink));

    while (!found && currLink && currLink->signature == CALLBACK_LINK_SIGNATURE)
        {
        if (strcmp(currLink->type, type) == 0)
            found = TRUE;
        else
            {
            callbackFunc = currLink->previousCallback;
            prevLink = currLink;
            currLink = currLink->previousCallbackData;
            }
        }

    if (found)
        {
        if (!prevLink)
            {
            errChk( InstallCtrlCallback(panel, ctrl,  currLink->previousCallback, (void *)currLink->previousCallbackData));
            }
        else
            {
            prevLink->previousCallback = currLink->previousCallback;
            prevLink->previousCallbackData = currLink->previousCallbackData;
            }

        free(currLink->type);
        free(currLink);
        UnrecordPanelWithLinkedCallback(panel);
        }

Error:
    if (locked)
        ReleaseLock(kChainCallbackLock);
    SetBOLE(savedBole);
    if (error < 0)
        return error;
    else
        return found;
}
#endif  // HAVE_CVI_RTE

/*************************************************************************/

#if HAVE_CVI_RTE
static int CVIFUNC GetOriginalCtrlCallbackData(int panel, int ctrl, void **callbackData)
{
    int                     error = UIENoError;
    CtrlCallbackPtr         callbackFunc;
    CtrlCallbackChainLink   currLink;

    errChk( GetCtrlAttribute(panel, ctrl, ATTR_CALLBACK_FUNCTION_POINTER, &callbackFunc));
    errChk( GetCtrlAttribute(panel, ctrl, ATTR_CALLBACK_DATA, &currLink));

    while (callbackFunc == LinkedCtrlCallback)
        {
        callbackFunc = currLink->previousCallback;
        currLink = currLink->previousCallbackData;
        }

    *callbackData = currLink;

Error:
    return error;
}
#endif  // HAVE_CVI_RTE

/*************************************************************************/

#if HAVE_CVI_RTE
static int CVIFUNC SetOriginalCtrlCallbackData(int panel, int ctrl, void *callbackData)
{
    int                     error = UIENoError;
    CtrlCallbackPtr         callbackFunc;
    CtrlCallbackChainLink   currLink;

    errChk( GetCtrlAttribute(panel, ctrl, ATTR_CALLBACK_FUNCTION_POINTER, &callbackFunc));
    errChk( GetCtrlAttribute(panel, ctrl, ATTR_CALLBACK_DATA, &currLink));

    while (1)
        {
        callbackFunc = currLink->previousCallback;
        if (callbackFunc == LinkedCtrlCallback)
            currLink = currLink->previousCallbackData;
        else
            {
            currLink->previousCallbackData = callbackData;
            break;
            }
        }

Error:
    return error;
}
#endif  // HAVE_CVI_RTE

/*************************************************************************/

#if HAVE_CVI_RTE
static int CVIFUNC FindCtrlCallbackLink(int panel, int ctrl, const char *type,  CtrlCallbackChainLink *link)
{
    int                     error = UIENoError;
    CtrlCallbackPtr         callbackFunc;
    int                     found = FALSE;
    CtrlCallbackChainLink   currLink;

    if (!type)
        errChk( UIENullPointerPassed);

    errChk( GetCtrlAttribute(panel, ctrl, ATTR_CALLBACK_FUNCTION_POINTER, &callbackFunc));
    errChk( GetCtrlAttribute(panel, ctrl, ATTR_CALLBACK_DATA, &currLink));

    while (!found && (callbackFunc == LinkedCtrlCallback) && currLink)
        {
        if (strcmp(currLink->type, type) == 0)
            {
            found = TRUE;
            if (link)
                *link = currLink;
            }
        else
            {
            callbackFunc = currLink->previousCallback;
            currLink = currLink->previousCallbackData;
            }
        }

Error:
    if (error < 0)
        return error;
    else
        return found;
}
#endif  // HAVE_CVI_RTE

/*************************************************************************/

    /*  When a callback swallows an event by returning a non-zero value, no further callbacks in the
        chain of linked callbacks are called  (with the exception of the EVENT_DISCARD event which is
        alway sent to each callback).  The final return value returned to CVI is TRUE if any ANY of the
        linked callbacks returned TRUE.
    */
#if HAVE_CVI_RTE
static int CVICALLBACK LinkedCtrlCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    CtrlCallbackChainLink   link = (CtrlCallbackChainLink)callbackData;
    int                     returnVal = 0;
    CtrlCallbackPtr         previousCallback = link->previousCallback;
    void                    *previousCallbackData = link->previousCallbackData;

        /*  This is being done as a formality, it is only needed if one of the callback functions is
            "bad" and causes an event to be sent to the control's installed callback as a result of
            receiving the discard message.
        */
    if (event == EVENT_DISCARD) 
        InstallCtrlCallback(panel, control, previousCallback, previousCallbackData);

        /*  Call the callback passed in to ChainCtrlCallback */
    if (link->callback) 
        returnVal =  link->callback(panel, control, event, link->callbackData, eventData1, eventData2);

    if (event == EVENT_DISCARD)   /* can't swallow EVENT_DISCARD */
        returnVal = 0;

    if (returnVal == 0 && previousCallback)
        returnVal = previousCallback(panel, control, event, previousCallbackData, eventData1, eventData2);

    if (event == EVENT_DISCARD)
        {
        free(link->type);
        free(link);
        UnrecordPanelWithLinkedCallback(panel);
        }

    return returnVal;
}
#endif  // HAVE_CVI_RTE

/*****************************************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC CallCtrlCallback(int panel, int ctrl, int event, int eventData1, int eventData2, int *returnValue)
{
    void                *callbackData;
    CtrlCallbackPtr     callback;
    int                 result;
    int                 calledCallback = FALSE;
    int                 error = UIENoError;
    int                 locked = FALSE;

    locked = GetLock(kChainCallbackLock);

    errChk( GetCtrlAttribute(panel, ctrl, ATTR_CALLBACK_FUNCTION_POINTER, &callback));

    if (callback)
        {
        errChk( GetCtrlAttribute(panel, ctrl, ATTR_CALLBACK_DATA, &callbackData));

        /* Remove the lock before calling the callback.  We don't want to halt all
           threads using chain callback functions by holding onto the lock.
        */
        locked = ReleaseLock(kChainCallbackLock);

        result = (*callback)(panel, ctrl, event, callbackData, eventData1, eventData2);
        if (returnValue)
            *returnValue = result;
        calledCallback = TRUE;
        }

Error:
    if (locked)
        ReleaseLock(kChainCallbackLock);
    if (error < 0)
        return error;
    else
        return calledCallback;
}
#endif  // HAVE_CVI_RTE

/*****************************************************************************************/

    /*  When custom controls are deleted by DiscardCtrl() and DiscardPanel(), they have
        a chance to free their associated resources in response to the EVENT_DISCARD
        event.  However, when all remaining UI objects are deleted when the program
        terminates, EVENT_DISCARD is not sent because execution is already terminated.
        This means the memory and other resources allocated for by a custom control will
        not have been freed.  This can be observed in the dynamic memory display after
        exiting a program.  This function causes the deletion of all user interface objects
        to occur while the program is still running so that custom controls can clean up
        after themselves.

        Note: For memory, this is not really a problem, since all unfreed blocks are
        freed before the next time a program is run.  But it confused some users
        to see the dynamic memory display filled up at the end of execution.
    */
#if HAVE_CVI_RTE
static void CVIFUNC InstallObjectDeletionProcedure(void)
{
    static int sInstallDone = 0;       /* This has already been initialized */
    static int sInstallNow  = 0;       /* This is currently being initialized. */
    int prev = 0;

    /* Check if already been initialized, if not, do so now. */
    if (!sInstallDone)
        {
        prev = InterlockedExchange(&sInstallNow, 1);
        if (prev)                                   /* If it is initialized or is in the process of initializing. */
            {
            while (!sInstallDone)                   /* Wait until the initializing finishes */
                Sleep(THREAD_SLEEP_TIME);
            }
        else
            {
            InitializeMultithreadingIfNecessary(); /* Have to make sure that the multithreading atexit()   */
            atexit(ObjectDeletionProcedure);       /* procedure is set before the ObjectDeletionProcedure  */
            sInstallDone = 1;                      /* since atexit works LIFO (last in, first out.)        */
            }
    }
}
#endif  // HAVE_CVI_RTE

#if HAVE_CVI_RTE
static void CVIANSI ObjectDeletionProcedure(void)
{
    /* This function used to call DiscardAllUserInterfaceObjects.
       That is a problem:  This may be in a DLL which is being dynamically
       unloaded by a program that has other UI objects.
       So in CVI 5.0, we changed this to discard only panels for which
       we chained panel or ctrl callbacks.  We also added the check for
       CVIRTEHasBeenDetached, which is a new function in CVI 5.0.
    */
    if (!CVIRTEHasBeenDetached())
        {
        int panel, index, savedBole;

        savedBole = SetBOLE(FALSE);
        index = 1;
        while (GetPanelWithLinkedCallbackByIndex (index, &panel))
            {
            DiscardPanel(panel);
            index++;
            }
        SetBOLE(savedBole);
        DestroyPanelsWithLinkedCallbacksList();
        }
}
#endif  // HAVE_CVI_RTE

/*****************************************************************************************/

typedef struct {
    int panel;
    int refCount;
} PanelWithLinkedCallbackRec;   /* either a CtrlCallback or a PanelCallback */

#if HAVE_CVI_RTE
static int CVIANSI RecordPanelWithLinkedCallback(int panel)
{
    int                         error = UIENoError;
    int                         index;
    PanelWithLinkedCallbackRec  rec, *ptr;
    ListType                    *pPanelsWithLinkedCallbacksList = NULL;

    InitializeMultithreadingIfNecessary();

    pPanelsWithLinkedCallbacksList = GetPointerToTsvPanelsWithLinkedCallbacksList();

    if (*pPanelsWithLinkedCallbacksList == NULL)
        {
        nullChk( *pPanelsWithLinkedCallbacksList
                            = ListCreate(sizeof(PanelWithLinkedCallbackRec)));
        }

    index = ListFindItem(*pPanelsWithLinkedCallbacksList, &panel,
                         FRONT_OF_LIST, ComparePanelToPnlWthLnkdCbksRec);
    if (index > 0)
        {
        ptr = ListGetPtrToItem(*pPanelsWithLinkedCallbacksList, index);
        ptr->refCount++;
        }
    else
        {
        rec.panel = panel;
        rec.refCount = 1;
        if (!ListInsertItem(*pPanelsWithLinkedCallbacksList, &rec, END_OF_LIST))
            errChk( UIEOutOfMemory);
        }

Error:
    if (pPanelsWithLinkedCallbacksList)
        ReleasePointerToTsvPanelsWithLinkedCallbacksList();
    return error;
}
#endif  // HAVE_CVI_RTE

/*****************************************************************************************/

#if HAVE_CVI_RTE
static void CVIANSI UnrecordPanelWithLinkedCallback(int panel)
{
    int                         index;
    PanelWithLinkedCallbackRec  *ptr;
    ListType                    *pPanelsWithLinkedCallbacksList = NULL;

    InitializeMultithreadingIfNecessary();
  
    pPanelsWithLinkedCallbacksList = GetPointerToTsvPanelsWithLinkedCallbacksList();

    if (pPanelsWithLinkedCallbacksList && *pPanelsWithLinkedCallbacksList)
        {
        index = ListFindItem(*pPanelsWithLinkedCallbacksList, &panel,
                             FRONT_OF_LIST, ComparePanelToPnlWthLnkdCbksRec);

        if (index > 0)
            {
            ptr = ListGetPtrToItem(*pPanelsWithLinkedCallbacksList, index);
            ptr->refCount--;
            if (ptr->refCount == 0)
                ListRemoveItem(*pPanelsWithLinkedCallbacksList, NULL, index);
            }
        }

    if (pPanelsWithLinkedCallbacksList)
        ReleasePointerToTsvPanelsWithLinkedCallbacksList();
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

static int CVICALLBACK ComparePanelToPnlWthLnkdCbksRec(void *ptrToItem, void *listItemPtr)
{
    int                         panel = *(int *)ptrToItem;
    PanelWithLinkedCallbackRec  *recPtr = (PanelWithLinkedCallbackRec *)listItemPtr;

    if (panel < recPtr->panel) return -1;
    if (panel > recPtr->panel) return  1;
    return 0;
}

/**************************************************************************/

#if HAVE_CVI_RTE
static int CVIANSI GetPanelWithLinkedCallbackByIndex (int index, int *panel)
{
    PanelWithLinkedCallbackRec *ptr;
    ListType                   *pPanelsWithLinkedCallbacksList = NULL;

    InitializeMultithreadingIfNecessary();

    pPanelsWithLinkedCallbacksList = GetPointerToTsvPanelsWithLinkedCallbacksList();

    if (pPanelsWithLinkedCallbacksList && *pPanelsWithLinkedCallbacksList)
        {
        if ((index >= 1)
                && (index <= ListNumItems(*pPanelsWithLinkedCallbacksList)))
            {
            ptr = ListGetPtrToItem(*pPanelsWithLinkedCallbacksList, index);
            *panel = ptr->panel;
            return TRUE;
            }
        }

    if (pPanelsWithLinkedCallbacksList)
        ReleasePointerToTsvPanelsWithLinkedCallbacksList();
    return FALSE;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
static void CVIANSI DestroyPanelsWithLinkedCallbacksList(void)
{
    ListType *pPanelsWithLinkedCallbacksList = NULL;

    InitializeMultithreadingIfNecessary();

    pPanelsWithLinkedCallbacksList = GetPointerToTsvPanelsWithLinkedCallbacksList();

    if (pPanelsWithLinkedCallbacksList && *pPanelsWithLinkedCallbacksList)
        {
        ListDispose(*pPanelsWithLinkedCallbacksList);
        *pPanelsWithLinkedCallbacksList = NULL;
        }

    if (pPanelsWithLinkedCallbacksList)
        ReleasePointerToTsvPanelsWithLinkedCallbacksList();
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

    /*  Only one link allowed per type.  If already linked with the same type then UIEInvalidControlType
        is returned.  If successfully linked, 0 is returned, otherwise a negative error
        code is returned.

        Currently this is no support for deleting or replacing a link (could be added in future).
        All links are destroyed when the control is discarded.
    */
#if HAVE_CVI_RTE
int CVIFUNC ChainPanelCallback(int panel, PanelCallbackPtr newCallback, void *newCallbackData, const char *type)
{
    int                     error = UIENoError;
    PanelCallbackChainLink  link = 0;
    int                     alreadyLinked = FALSE;
    int                     insertedInList = FALSE;
    int                     locked = FALSE;

    if (!type)
        errChk( UIENullPointerPassed);

    locked = GetLock(kChainCallbackLock);

    errChk( alreadyLinked = FindPanelCallbackLink(panel, type, 0));

    if  (!alreadyLinked)
        {
        InstallObjectDeletionProcedure();
        errChk( RecordPanelWithLinkedCallback(panel));
        insertedInList = TRUE;
        nullChk( link = (PanelCallbackChainLink)calloc(sizeof(*link),1));
        nullChk( link->type = StrDup(type));
        link->signature = CALLBACK_LINK_SIGNATURE;

            /*  nothing passed this point should fail if the code is working as intended */
        errChk( GetPanelAttribute(panel, ATTR_CALLBACK_FUNCTION_POINTER, &link->previousCallback));
        errChk( GetPanelAttribute(panel, ATTR_CALLBACK_DATA, &link->previousCallbackData));

        link->callback = newCallback;
        link->callbackData = newCallbackData;

        errChk( InstallPanelCallback(panel, LinkedPanelCallback, (void *)link));
        }
    else
        errChk( UIEInvalidControlType);

Error:
    if (locked)
        ReleaseLock(kChainCallbackLock);
    if (error < 0)
        {
        if (insertedInList)
            UnrecordPanelWithLinkedCallback(panel);
        if (link)
            {
            free(link->type);
            free(link);
            }
        return error;
        }
    else
        return 0;
}
#endif  // HAVE_CVI_RTE

/*************************************************************************/

    /*  returns  UIEInvalidControlType if the panel is not linked with the specified type */
#if HAVE_CVI_RTE
int CVIFUNC GetChainedPanelCallbackData(int panel, const char *type, void **callbackData)
{
    int                     error = UIENoError;
    int                     found = FALSE;
    PanelCallbackChainLink  link;
    int                     locked = FALSE;

    locked = GetLock(kChainCallbackLock);

    if (type && !*type)  /* "" mean get original callback data */
        errChk( GetOriginalPanelCallbackData(panel, callbackData));
    else
        {
        errChk( found = FindPanelCallbackLink(panel, type, &link));

        if (found)
            *callbackData = link->callbackData;
        else
            errChk( UIEInvalidControlType);
        }

Error:
    if (locked)
        ReleaseLock(kChainCallbackLock);
    if (error < 0)
        return error;
    else
        return 0;
}
#endif  // HAVE_CVI_RTE

/*************************************************************************/

    /*  returns  UIEInvalidControlType if the panel is not linked with the specified type */
#if HAVE_CVI_RTE
int CVIFUNC SetChainedPanelCallbackData(int panel, const char *type, void *callbackData)
{
    int                     error = UIENoError;
    int                     found = FALSE;
    PanelCallbackChainLink  link;
    int                     locked = FALSE;

    locked = GetLock(kChainCallbackLock);

    if (type && !*type)  /* "" mean get original callback data */
        errChk( SetOriginalPanelCallbackData(panel, callbackData));
    else
        {
        errChk( found = FindPanelCallbackLink(panel, type, &link));

        if (found)
            link->callbackData = callbackData;
        else
            errChk( UIEInvalidControlType);
        }

Error:
    if (locked)
        ReleaseLock(kChainCallbackLock);
    if (error < 0)
        return error;
    else
        return 0;
}
#endif  // HAVE_CVI_RTE

/*************************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC UnchainPanelCallback(int panel, const char *type)
{
    int                     error = UIENoError;
    PanelCallbackPtr        callbackFunc;
    int                     found = FALSE;
    PanelCallbackChainLink  currLink;
    PanelCallbackChainLink  prevLink = NULL;
    int                     savedBole = SetBOLE(FALSE);
    int                     locked = FALSE;

    if (!type)
        errChk( UIENullPointerPassed);

    locked = GetLock(kChainCallbackLock);

    errChk( GetPanelAttribute(panel, ATTR_CALLBACK_FUNCTION_POINTER, &callbackFunc));
    errChk( GetPanelAttribute(panel, ATTR_CALLBACK_DATA, &currLink));

    while (!found && currLink && currLink->signature == CALLBACK_LINK_SIGNATURE)
        {
        if (strcmp(currLink->type, type) == 0)
            found = TRUE;
        else
            {
            callbackFunc = currLink->previousCallback;
            prevLink = currLink;
            currLink = currLink->previousCallbackData;
            }
        }

    if (found)
        {
        if (!prevLink)
            {
            errChk( InstallPanelCallback(panel,  currLink->previousCallback, (void *)currLink->previousCallbackData));
            }
        else
            {
            prevLink->previousCallback = currLink->previousCallback;
            prevLink->previousCallbackData = currLink->previousCallbackData;
            }

        free(currLink->type);
        free(currLink);
        UnrecordPanelWithLinkedCallback(panel);
        }

Error:
    if (locked)
        ReleaseLock(kChainCallbackLock);
    SetBOLE(savedBole);
    if (error < 0)
        return error;
    else
        return found;
}
#endif  // HAVE_CVI_RTE

/*************************************************************************/

#if HAVE_CVI_RTE
static int CVIFUNC GetOriginalPanelCallbackData(int panel, void **callbackData)
{
    int                     error = UIENoError;
    PanelCallbackPtr        callbackFunc;
    PanelCallbackChainLink  currLink;

    errChk( GetPanelAttribute(panel, ATTR_CALLBACK_FUNCTION_POINTER, &callbackFunc));
    errChk( GetPanelAttribute(panel, ATTR_CALLBACK_DATA, &currLink));

    while (callbackFunc == LinkedPanelCallback)
        {
        callbackFunc = currLink->previousCallback;
        currLink = currLink->previousCallbackData;
        }

    *callbackData = currLink;

Error:
    return error;
}
#endif  // HAVE_CVI_RTE

/*************************************************************************/

#if HAVE_CVI_RTE
static int CVIFUNC SetOriginalPanelCallbackData(int panel, void *callbackData)
{
    int                     error = UIENoError;
    PanelCallbackPtr        callbackFunc;
    PanelCallbackChainLink  currLink;

    errChk( GetPanelAttribute(panel, ATTR_CALLBACK_FUNCTION_POINTER, &callbackFunc));
    errChk( GetPanelAttribute(panel, ATTR_CALLBACK_DATA, &currLink));

    while (1)
        {
        callbackFunc = currLink->previousCallback;
        if (callbackFunc == LinkedPanelCallback)
            currLink = currLink->previousCallbackData;
        else
            {
            currLink->previousCallbackData = callbackData;
            break;
            }
        }

Error:
    return error;
}
#endif  // HAVE_CVI_RTE

/*************************************************************************/

#if HAVE_CVI_RTE
static int CVIFUNC FindPanelCallbackLink(int panel, const char *type,  PanelCallbackChainLink *link)
{
    int                     error = UIENoError;
    PanelCallbackPtr        callbackFunc;
    int                     found = FALSE;
    PanelCallbackChainLink  currLink;

    if (!type)
        errChk( UIENullPointerPassed);

    errChk( GetPanelAttribute(panel, ATTR_CALLBACK_FUNCTION_POINTER, &callbackFunc));
    errChk( GetPanelAttribute(panel, ATTR_CALLBACK_DATA, &currLink));

    while (!found && (callbackFunc == LinkedPanelCallback)
        && currLink && currLink->signature == CALLBACK_LINK_SIGNATURE)
        {
        if (strcmp(currLink->type, type) == 0)
            {
            found = TRUE;
            if (link)
                *link = currLink;
            }
        else
            {
            callbackFunc = currLink->previousCallback;
            currLink = currLink->previousCallbackData;
            }
        }

Error:
    if (error < 0)
        return error;
    else
        return found;
}
#endif  // HAVE_CVI_RTE

/*************************************************************************/

    /*  When a callback swallows an event by returning a non-zero value, no further callbacks in the
        chain of linked callbacks are called  (with the exception of the EVENT_DISCARD event which is
        alway sent to each callback).  The final return value returned to CVI is TRUE if any ANY of the
        linked callbacks returned TRUE.
    */
#if HAVE_CVI_RTE
static int CVICALLBACK LinkedPanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2)
{
    PanelCallbackChainLink  link = (PanelCallbackChainLink)callbackData;
    int                     returnVal = 0;
    PanelCallbackPtr        previousCallback = link->previousCallback;
    void                    *previousCallbackData = link->previousCallbackData;

        /*  This is being done as a formality, it is only needed if one of the callback functions is
            "bad" and causes an event to be sent to the panel's installed callback as a result of
            receiving the discard message.
        */
    if (event == EVENT_DISCARD)
        InstallPanelCallback(panel, previousCallback, previousCallbackData);

        /*  Call the callback passed in to ChainPanelCallback */
    if (link->callback)
        returnVal =  link->callback(panel, event, link->callbackData, eventData1, eventData2);

    if (event == EVENT_DISCARD)   /* can't swallow EVENT_DISCARD */
        returnVal = 0;

    if (returnVal == 0 && previousCallback)
        returnVal = previousCallback(panel, event, previousCallbackData, eventData1, eventData2);

    if (event == EVENT_DISCARD)
        {
        free(link->type);
        free(link);
        UnrecordPanelWithLinkedCallback(panel);
        }

    return returnVal;
}
#endif  // HAVE_CVI_RTE

/*****************************************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC CallPanelCallback(int panel, int event, int eventData1, int eventData2, int *returnValue)
{
    void                *callbackData;
    PanelCallbackPtr    callback;
    int                 result;
    int                 calledCallback = FALSE;
    int                 error = UIENoError;
    int                 locked = FALSE;

    locked = GetLock(kChainCallbackLock);

    errChk( GetPanelAttribute(panel, ATTR_CALLBACK_FUNCTION_POINTER, &callback));

    if (callback)
        {
        errChk( GetPanelAttribute(panel, ATTR_CALLBACK_DATA, &callbackData));

        /* Remove the lock before calling the callback.  We don't want to halt all
           threads using chain callback functions by holding onto the lock.
        */
        locked = ReleaseLock(kChainCallbackLock);

        result = (*callback)(panel, event, callbackData, eventData1, eventData2);
        if (returnValue)
            *returnValue = result;
        calledCallback = TRUE;
        }

Error:
    if (locked)
        ReleaseLock(kChainCallbackLock);
    if (error < 0)
        return error;
    else
        return calledCallback;
}
#endif  // HAVE_CVI_RTE

/*************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC PreviousCtrl(int panel, int ctrl, int wrap)
{
    int currentCtrl = 0;
    int nextCtrl;

    GetPanelAttribute(panel, ATTR_PANEL_FIRST_CTRL, &currentCtrl);

    if (wrap && ctrl == currentCtrl)
        return LastCtrl(panel);

    while (currentCtrl)
        {
        GetCtrlAttribute(panel, currentCtrl, ATTR_NEXT_CTRL, &nextCtrl);
        if (nextCtrl == ctrl)
            return currentCtrl;
        else
            currentCtrl = nextCtrl;
        }

    return 0;
}
#endif  // HAVE_CVI_RTE

/*************************************************************/

#if HAVE_CVI_RTE
static int  CVIFUNC NextCtrl(int panel, int ctrl, int wrap)
{
    int nextCtrl;

    GetCtrlAttribute(panel, ctrl, ATTR_NEXT_CTRL, &nextCtrl);
    if (wrap && nextCtrl <= 0)
        GetPanelAttribute(panel, ATTR_PANEL_FIRST_CTRL, &nextCtrl);

    return nextCtrl;
}
#endif  // HAVE_CVI_RTE

/*************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC CtrlCanBeTabStop(int panel, int ctrl)
{
    int visible, dimmed, mode, savedBole, error;

    savedBole = SetBOLE (FALSE);
    error = GetCtrlAttribute (panel, ctrl, ATTR_CTRL_MODE, &mode);
    SetBOLE (savedBole);
    if (error < 0)  // control type does not support ATTR_CTRL_MODE: assume that it can be a tab stop
        mode = VAL_HOT;

    GetCtrlAttribute (panel, ctrl, ATTR_VISIBLE, &visible);
    GetCtrlAttribute (panel, ctrl, ATTR_DIMMED, &dimmed);

    return (mode != VAL_INDICATOR && !dimmed && visible) ? TRUE : FALSE;
}
#endif  // HAVE_CVI_RTE

/*************************************************************/

#if HAVE_CVI_RTE
static int CVIFUNC LastCtrl(int panel)
{
    int ctrl;
    int nextCtrl;

    GetPanelAttribute(panel, ATTR_PANEL_FIRST_CTRL, &ctrl);

    while (ctrl)
        {
        GetCtrlAttribute(panel, ctrl, ATTR_NEXT_CTRL, &nextCtrl);
        if (nextCtrl < 0)
            break;
        ctrl = nextCtrl;
        }

    return ctrl;
}
#endif  // HAVE_CVI_RTE

/*************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC NextCtrlInTabOrder(int panel, int ctrl, int forward, int wrap)
{
    int startCtrl = ctrl;

    while (ctrl)
        {
        if (forward)
            ctrl = NextCtrl(panel, ctrl, wrap);
        else
            ctrl = PreviousCtrl(panel, ctrl, wrap);

        if (ctrl == startCtrl)
            return 0;       /* wrapped and found nothing */

        if (ctrl && CtrlCanBeTabStop(panel, ctrl))
            return ctrl;
        }

    return 0;
}
#endif  // HAVE_CVI_RTE

/*************************************************************/

    /*  Returns a boolean indicating whether a control was activated */
#if HAVE_CVI_RTE
int CVIFUNC ActivateFirstCtrl(int panel)
{
    int currentCtrl;
    int controlActivated = FALSE;

    GetPanelAttribute(panel, ATTR_PANEL_FIRST_CTRL, &currentCtrl);
    while (currentCtrl)
        {
        if (CtrlCanBeTabStop(panel, currentCtrl))
            {
            controlActivated = TRUE;
            SetActiveCtrl(panel, currentCtrl);
            break;
            }
        else
            GetCtrlAttribute(panel, currentCtrl, ATTR_NEXT_CTRL, &currentCtrl);
        }

    return controlActivated;
}
#endif  // HAVE_CVI_RTE

/*************************************************************/

    /* discards a control if it exists, but does not cause an error if it doesn't */
#if HAVE_CVI_RTE
void CVIFUNC DiscardCtrlNoBOLE(int panel, int ctrl, int noDiscardEvent)
{
    int savedBole = SetBOLE(FALSE);

    if (ctrl > 0)
        {
        if (noDiscardEvent)
            SetCtrlAttribute(panel, ctrl, ATTR_CALLBACK_FUNCTION_POINTER, 0);
        DiscardCtrl(panel, ctrl);
        }

    SetBOLE(savedBole);
}
#endif  // HAVE_CVI_RTE

/*****************************************************************************************/

    /* discards a panel if it exists, but does not cause an error if it doesn't */
#if HAVE_CVI_RTE
void CVIFUNC DiscardPanelNoBOLE(int panel, int noDiscardEvent)
{
    int savedBole = SetBOLE(FALSE);

    if (panel > 0)
        {
        if (noDiscardEvent)
            SetCtrlAttribute(panel, ATTR_CALLBACK_FUNCTION_POINTER, 0);
        DiscardPanel(panel);
        }

    SetBOLE(savedBole);
}
#endif  // HAVE_CVI_RTE

/*****************************************************************************************/

#if HAVE_CVI_RTE
static void CVIFUNC MouseEvents_Check(MouseEventsInfo info)
{
    int     leftButtonIsDown;
    int     rightButtonIsDown;
    Point   p;
    int     savedBole;
    int     error;

    savedBole = SetBOLE(FALSE);
    error = GetRelativeMouseState (info->panel, 0, &p.x, &p.y, &leftButtonIsDown, &rightButtonIsDown, 0);
    SetBOLE(savedBole);
    if (error < 0)      /* in case of hidden panel error */
        return;

    if (info->lastX  != p.x || info->lastY != p.y)
        {
        info->lastX = p.x;
        info->lastY = p.y;
        CallCtrlCallback (info->panel, info->ctrl, EVENT_MOUSE_MOVE, p.y, p.x, 0);
        }

    if (info->leftButtonDown && !leftButtonIsDown)
        {
        CallCtrlCallback (info->panel, info->ctrl, EVENT_LEFT_MOUSE_UP, p.y, p.x, 0);
        info->leftButtonDown = FALSE;
        }
    if(!info->leftButtonDown && leftButtonIsDown)
        {
        info->leftButtonDown = TRUE;
        }
    if (info->rightButtonDown && !rightButtonIsDown)
        {
        CallCtrlCallback (info->panel, info->ctrl, EVENT_RIGHT_MOUSE_UP, p.y, p.x, 0);
        info->rightButtonDown = FALSE;
        }
    if(!info->rightButtonDown && rightButtonIsDown)
        {
        info->rightButtonDown = TRUE;
        }

}
#endif  // HAVE_CVI_RTE

/*****************************************************************************************/

#if HAVE_CVI_RTE
static void CVICALLBACK MouseEvents_CheckCallback(void *callbackData)
{
    MouseEventsInfo info = (MouseEventsInfo)callbackData;

    if (info->dispose)
        free(info);
    else
        {
        PostDelayedCall (MouseEvents_CheckCallback, info, info->minPeriod);

        if (info->enabled)
            MouseEvents_Check(info);
        }
}
#endif  // HAVE_CVI_RTE

/*****************************************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC DisableExtendedMouseEvents(int panel, int ctrl)
{
    MouseEventsInfo info = 0;
    int             error = UIENoError;
    int             locked = FALSE;

    locked = GetLock(kExtMouseEventsLock);

    errChk( GetChainedCallbackData (panel, ctrl, "MouseEvents", (void **)&info));
    info->enabled = FALSE;
    info->dispose = TRUE;
    errChk( UnchainCtrlCallback(panel, ctrl, "MouseEvents"));

Error:
    if (locked)
        ReleaseLock(kExtMouseEventsLock);
    if (error < 0)
        return error;
    else
        return 0;
}
#endif  // HAVE_CVI_RTE

/*****************************************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC ConvertMouseCoordinates(int panel, int ctrl, int relativeToCtrl, int clipToControl, int *x, int *y)
{
    Point   p;
    int     top, left;
    Rect    r;
    int     error = UIENoError;

    if (x)
        p.x = *x;
    else
        p.x = 0;

    if (y)
        p.y = *y;
    else
        p.y = 0;


    if (relativeToCtrl)
        {
        errChk( GetCtrlAttribute(panel, ctrl, ATTR_TOP, &top));
        errChk( GetCtrlAttribute(panel, ctrl, ATTR_LEFT, &left));
        p.y -= top;
        p.x -= left;
        }

    if (clipToControl)
        {
        errChk( GetCtrlAttribute(panel, ctrl, ATTR_WIDTH, &r.width));
        errChk( GetCtrlAttribute(panel, ctrl, ATTR_HEIGHT, &r.height));

        if(!relativeToCtrl)
            {
            errChk( GetCtrlAttribute(panel, ctrl, ATTR_TOP, &r.top));
            errChk( GetCtrlAttribute(panel, ctrl, ATTR_LEFT, &r.left));
            }
        else
            {
            r.top = 0;
            r.left = 0;
            }

        PointPinnedToRect (p, r, &p);
        }

    if (x)
        *x = p.x;
    if (y)
        *y = p.y;

Error:
    if (error < 0)
        return error;
    else
        return 0;
}
#endif  // HAVE_CVI_RTE

/*****************************************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC EnableExtendedMouseEvents(int panel, int ctrl, double minPeriod)
{
    int             error = UIENoError;
    MouseEventsInfo newInfo = 0;
    MouseEventsInfo currInfo = 0;
    MouseEventsInfo info = 0;
    int             savedBole;
    int             locked = FALSE;

    locked = GetLock(kExtMouseEventsLock);

    if ( GetChainedCallbackData (panel, ctrl, "MouseEvents", (void **)&currInfo) >= 0)
        info = currInfo;
    else
        {
        nullChk(newInfo = calloc(sizeof(*info), 1));
        newInfo->panel = panel;
        newInfo->ctrl = ctrl;
        newInfo->signature = MOUSE_EVENTS_SIGNATURE;
        newInfo->dispose = FALSE;
        errChk( ChainCtrlCallback (panel, ctrl, MouseEventsCallback, (void *)newInfo, "MouseEvents"));
        PostDelayedCall (MouseEvents_CheckCallback, newInfo, minPeriod);
        info = newInfo;
        }

    info->dispose = FALSE;
    info->enabled = TRUE;
    info->minPeriod = minPeriod;

    savedBole = SetBOLE(FALSE);
    if (GetRelativeMouseState (panel, 0, &info->lastX, &info->lastY, &info->leftButtonDown, &info->rightButtonDown, 0) < 0)
        {
        info->lastX = 0;    /* in case of UIEHiddenPanelError */
        info->lastY = 0;
        }
    SetBOLE(savedBole);

Error:
    if (locked)
        ReleaseLock(kExtMouseEventsLock);
    if (error < 0)
        {
        free(newInfo);
        return error;
        }
    else
        return 0;
}
#endif  // HAVE_CVI_RTE

/*****************************************************************************************/

static int CVICALLBACK MouseEventsCallback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
    MouseEventsInfo info = (MouseEventsInfo)callbackData;

    switch (event)
        {
        case EVENT_DISCARD:
            info->dispose = TRUE;
            break;
        case EVENT_LEFT_CLICK:
            info->leftButtonDown = TRUE;
            break;
        case EVENT_RIGHT_CLICK:
            info->rightButtonDown = TRUE;
            break;
        }

    return 0;
}

/*****************************************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC RecessCtrlFrame(int panel, int control)
{
    int     error = 0;
    Rect    rect;
    int     decoration;
    int     numCtrls;

    errChk( GetCtrlAttribute(panel, control, ATTR_LEFT, &rect.left));
    errChk( GetCtrlAttribute(panel, control, ATTR_TOP, &rect.top));
    errChk( GetCtrlAttribute(panel, control, ATTR_WIDTH, &rect.width));
    errChk( GetCtrlAttribute(panel, control, ATTR_HEIGHT, &rect.height));

    RectGrow (&rect, -1, -1);

    errChk( GetPanelAttribute (panel, ATTR_NUM_CTRLS, &numCtrls));
    errChk( decoration = NewCtrl (panel, CTRL_RECESSED_BOX, "", rect.top, rect.left));

    errChk( ChainCtrlCallback (panel, control, NULL, (void *)decoration, "RecessedFrame"));

    errChk( SetCtrlAttribute (panel, decoration, ATTR_ZPLANE_POSITION, numCtrls - 1));
    errChk( SetCtrlAttribute (panel, control, ATTR_ZPLANE_POSITION, 0));
    errChk( SetCtrlAttribute (panel, control, ATTR_FRAME_COLOR, VAL_TRANSPARENT));

    errChk( SetCtrlAttribute(panel, decoration, ATTR_WIDTH, rect.width));
    errChk( SetCtrlAttribute(panel, decoration, ATTR_HEIGHT, rect.height));

Error:
    return error;
}
#endif  // HAVE_CVI_RTE

/*****************************************************************************************/

#if HAVE_CVI_RTE
    /* calls RecessCtrlFrame on every string and text box control in the panel and its child panels */
int CVIFUNC RecessAllCtrlFrames(int panel)
{
    int     error = 0;
    int     style;
    int     ctrl;
    int     child;

        /* loop through all controls */
    errChk( GetPanelAttribute (panel, ATTR_PANEL_FIRST_CTRL, &ctrl));

    while (ctrl > 0)
        {
        errChk( GetCtrlAttribute(panel, ctrl, ATTR_CTRL_STYLE, &style));

        if (style == CTRL_TEXT_BOX || style == CTRL_STRING)
            errChk( RecessCtrlFrame(panel, ctrl));

        errChk( GetCtrlAttribute(panel, ctrl, ATTR_NEXT_CTRL, &ctrl));
        }


        /* loop through all child panels */
    errChk( GetPanelAttribute (panel, ATTR_FIRST_CHILD, &child));

    while (child > 0)
        {
        errChk( RecessAllCtrlFrames(child));
        errChk( GetPanelAttribute (child, ATTR_NEXT_PANEL, &child));
        }

Error:
    return error;
}
#endif  // HAVE_CVI_RTE

/*****************************************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC AllocateCtrlValString(int panel, int ctrl, char **string)
{
    int error = 0;
    int length;

    *string = 0;

    errChk( GetCtrlValStringLength(panel, ctrl, &length))
    nullChk( *string  = (char *)malloc (length + 1));
    errChk( GetCtrlVal(panel, ctrl, *string));

Error:
    return error;
}
#endif  // HAVE_CVI_RTE

/*****************************************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC GetCtrlValStringLength(int panel, int ctrl, int *length)
{
    int error = 0;
    int result;
    int dataType;
    int savedState;

    errChk( GetCtrlAttribute(panel, ctrl, ATTR_DATA_TYPE, &dataType));
    if (dataType != VAL_STRING)
        errChk( UIEControlNotTypeExpectedByFunction);

    savedState = SetBOLE(FALSE);
    result = GetCtrlAttribute(panel, ctrl, ATTR_STRING_TEXT_LENGTH, length);
    SetBOLE(savedState);

    if (result < 0) /* must be a list control */
        {
        int index;

        errChk( GetCtrlIndex (panel, ctrl, &index));

        if (index < 0) /* no list item selected */
            errChk( UIENoItemsInList);

        errChk( GetValueLengthFromIndex (panel, ctrl, index, length));
        }

Error:
    return error;
}
#endif  // HAVE_CVI_RTE

/*****************************************************************************************/

#if HAVE_CVI_RTE
    /* This function makes sure only one radio button in a group is on.                 */
    /* The function takes an active button and a list of buttons which may also include */
    /* the active button.  The active button is set on and all others are set off.      */
int CVIFUNC SetSelectedRadioButtons(int panel, int activeButton, int numCtrls, ...)
{
    va_list     parmInfo;
    int         error = 0;
    ListType    ctrlList = 0;
    int         index;
    int         currCtrl;

    va_start(parmInfo, numCtrls);
    errChk( GetIntListFromParmInfo(parmInfo, numCtrls, &ctrlList));

    for (index = 1; index <= numCtrls; index++)
        {
        ListGetItem(ctrlList, &currCtrl, index);
        errChk( SetCtrlVal(panel, currCtrl, currCtrl == activeButton));
        }

Error:
    ListDispose(ctrlList);
    va_end(parmInfo);
    return error;
}
#endif  // HAVE_CVI_RTE

/*****************************************************************************************/

#if HAVE_CVI_RTE
   /* This function returns the button that is currently "on" out of a list of radio buttons */
int CVIFUNC GetSelectedRadioButtons(int panel, int *activeButton, int numCtrls, ...)
{
    va_list     parmInfo;
    int         error = 0;
    ListType    ctrlList = 0;
    int         index;
    int         currCtrl;
    int         value;

    *activeButton = 0;

    va_start(parmInfo, numCtrls);
    errChk( GetIntListFromParmInfo(parmInfo, numCtrls, &ctrlList));

    for (index = 1; index <= numCtrls; index++)
        {
        ListGetItem(ctrlList, &currCtrl, index);
        errChk( GetCtrlVal(panel, currCtrl, &value));

        if (value)
            {
            *activeButton = currCtrl;
            break;
            }
        }

Error:
    ListDispose(ctrlList);
    va_end(parmInfo);
    return error;
}
#endif  // HAVE_CVI_RTE

/*****************************************************************************************/

#if HAVE_CVI_RTE
int CVIFUNC GetMetaFontInfo(const char metaFont[], char typeface[], int *bold, int *underline, int *strikeout,
    int *italic, int *size)
{
    int             error = UIENoError;
    ThreadLocalVars *pThreadLocalVars = 0;

    pThreadLocalVars = GetThreadLocalVars();

    if (pThreadLocalVars->fontPanel <= 0)
        errChk( pThreadLocalVars->fontPanel = NewPanel (0, "Metafont info panel", -1000, -1000, 10, 10));

    if (pThreadLocalVars->txtMsg <= 0)
        errChk( pThreadLocalVars->txtMsg = NewCtrl (pThreadLocalVars->fontPanel, CTRL_TEXT_MSG, "Sample", 0, 0));

    errChk( SetCtrlAttribute(pThreadLocalVars->fontPanel, pThreadLocalVars->txtMsg, ATTR_TEXT_FONT, metaFont));
    if (bold)
        errChk( GetCtrlAttribute(pThreadLocalVars->fontPanel, pThreadLocalVars->txtMsg, ATTR_TEXT_BOLD, bold));
    if (underline)
        errChk( GetCtrlAttribute(pThreadLocalVars->fontPanel, pThreadLocalVars->txtMsg, ATTR_TEXT_UNDERLINE, underline));
    if (strikeout)
        errChk( GetCtrlAttribute(pThreadLocalVars->fontPanel, pThreadLocalVars->txtMsg, ATTR_TEXT_STRIKEOUT, strikeout));
    if (italic)
        errChk( GetCtrlAttribute(pThreadLocalVars->fontPanel, pThreadLocalVars->txtMsg, ATTR_TEXT_ITALIC, italic));
    if (size)
        errChk( GetCtrlAttribute(pThreadLocalVars->fontPanel, pThreadLocalVars->txtMsg, ATTR_TEXT_POINT_SIZE, size));

    /* Assume no right-minded metafont has a text size of 2, therefore, we will get */
    /* back the typeface name, not the metafont name                                */
    if (typeface)
        {
        errChk( SetCtrlAttribute(pThreadLocalVars->fontPanel, pThreadLocalVars->txtMsg, ATTR_TEXT_POINT_SIZE, 2));
        errChk( GetCtrlAttribute(pThreadLocalVars->fontPanel, pThreadLocalVars->txtMsg, ATTR_TEXT_FONT, typeface));
        }

Error:
    if (error < 0)
        return error;
    else
        return 0;
}
#endif  // HAVE_CVI_RTE

/***************************************************20**************************************/

    /*  Returns TRUE, FALSE, or negative error code */
#if HAVE_CVI_RTE
int CVIFUNC IsMetaFont(const char fontname[256])
{
    int error = UIENoError;
    int savedBole = SetBOLE(FALSE);

    error = GetTextDisplaySize ("sample", fontname, NULL, NULL);
    SetBOLE(savedBole);

    if (error == UIEFontNotInFontTable)
        return FALSE;

    if (error < 0)
        return error;
    else
        return TRUE;
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

/*
    Used for finding underline accelerator keys (which are delimited by two underscores)
    returns the zero based position of the first underscore, or -1 if no double underlines.
*/

int CVIFUNC AcceleratorEscapeCodeLocation(const char *text)
{
    const char  *start = text;
    int         length;

    if( text)
        {
        length = strlen(text);

        for (;length > 2; CmbStrInc((const unsigned char *)text), length--)
            if (*text == '_' && *(text+1) == '_')
                if (*(text+2) != '_')                 /* need exactly two in a row */
                    return text - start;
                else
                    for (; length > 2 && *text == '_'; text++, length--);  /* skip current block of underscores */
        }

    return -1;
}

/**************************************************************************/

/*
    Removes double underline characters (which are used to signify an accelerator key so that the
    text may be displayed.
*/

void CVIFUNC RemoveAcceleratorEscapeCode(char *text)
{
    int underlineLocation;
    int length;

    if (text)
        {
        length = strlen(text);
        underlineLocation = AcceleratorEscapeCodeLocation(text);
        if (underlineLocation >= 0)
            memmove(text + underlineLocation, text + underlineLocation + 2, length - underlineLocation - 1); /* -2 for "__" and +1 for NULL byte == -1 */
        }
}

/**************************************************************************/

    /*  returns the index of the color in the table that most closely matches the specified color */
int CVIFUNC FindClosestColorInTable (int color, const int *table, int tableSize)
{
    #define     color_sqr(x)    ((x)*(x))
    int         i;
    unsigned    pytDist, rbDist, rgDist, gbDist, dist;
    unsigned    minDist, index = -1;
    unsigned    palRed, palGreen, palBlue;
    unsigned    red, green, blue;
    unsigned    tableColor;

    red     = (color >> 16) & 0xFF;
    green   = (color >> 8) & 0xFF;
    blue    = color & 0xFF;

    for (i = 0, minDist = (unsigned)-1; i < tableSize; i++)
        {
        tableColor = (unsigned)table[i];
        palRed      = (tableColor >> 16) & 0xFF;
        palGreen    = (tableColor >> 8) & 0xFF;
        palBlue     = tableColor & 0xFF;

        pytDist = color_sqr(red - palRed) + color_sqr(green - palGreen) + color_sqr(blue - palBlue);
        rgDist = color_sqr(palRed - palGreen - red + green);
        rbDist = color_sqr(palRed - palBlue - red + blue);
        gbDist = color_sqr(palGreen - palBlue - green + blue);
        dist = 3 * pytDist + rgDist + rbDist + gbDist;
        if (minDist == -1 || dist < minDist)
            {
            index = i;
            if ((minDist = dist) == 0)
                break;
            }
        }

    return index;
}

/**************************************************************************/

    /*  *ctrlList is allocated and filled with the ID's of the controls on the panel.
        The caller should call ListDispose() when done with the list.
        If an error occurs, *ctrlList is set to NULL.
        If the return value is less than zero, then it is an UIL error code.
        Otherwise, the function succeeded.
    */

#if HAVE_CVI_RTE
int CVIFUNC GetCtrlList(int panel, ListType *ctrlList)
{
    int         error = UIENoError;
    int         ctrl;
    ListType    list = 0;

    nullChk( list = ListCreate(sizeof(int)));
    errChk( GetPanelAttribute(panel, ATTR_PANEL_FIRST_CTRL, &ctrl));

    while (ctrl)
        {
        nullChk( ListInsertItem (list, &ctrl, END_OF_LIST));
        errChk( GetCtrlAttribute(panel, ctrl, ATTR_NEXT_CTRL, &ctrl));
        }

Error:
    if (error < 0)
        {
        ListDispose(list);
        return error;
        }
    else
        {
        *ctrlList = list;
        return 0;
        }
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
static int  CVICALLBACK CompareCtrlTabOrder(void *ctrl1, void *ctrl2)
{
    int tab1, tab2;
    ThreadLocalVars *pThreadLocalVars = 0;

    pThreadLocalVars = GetThreadLocalVars();

    GetCtrlAttribute(pThreadLocalVars->tabComparePanel, *(int *)ctrl1, ATTR_CTRL_TAB_POSITION, &tab1);
    GetCtrlAttribute(pThreadLocalVars->tabComparePanel, *(int *)ctrl2, ATTR_CTRL_TAB_POSITION, &tab2);

    if (tab1 < tab2)
        return -1;
    else
    if (tab1 > tab2)
        return 1;
    else
        return 0;
}
#endif  // HAVE_CVI_RTE


#if HAVE_CVI_RTE
static int CVICALLBACK ComparePanelZPlaneOrder (void *panel1, void *panel2)
{
    int pos1, pos2;

    GetPanelAttribute (*(int *)panel1, ATTR_ZPLANE_POSITION, &pos1);
    GetPanelAttribute (*(int *)panel2, ATTR_ZPLANE_POSITION, &pos2);

    if (pos1 < pos2)
        return -1;
    else if (pos1 > pos2)
        return 1;
    else
        return 0;
}
#endif  // HAVE_CVI_RTE


#if HAVE_CVI_RTE
void CVIFUNC SortCtrlListByTabOrder(int panel, ListType ctrlList)
{
    ThreadLocalVars *pThreadLocalVars = 0;

    pThreadLocalVars = GetThreadLocalVars();

    pThreadLocalVars->tabComparePanel = panel;        /*   must be set before calling CompareCtrlTabOrder */
    ListQuickSort(ctrlList, CompareCtrlTabOrder);
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

#if HAVE_CVI_RTE
char * CVIFUNC UILEventString(int event)
{
    ThreadLocalVars *pThreadLocalVars = 0;

    pThreadLocalVars = GetThreadLocalVars();

    switch (event)
        {
        case EVENT_COMMIT:
            return "EVENT_COMMIT";
        case EVENT_VAL_CHANGED:
            return "EVENT_VAL_CHANGED";
        case EVENT_LEFT_CLICK:
            return "EVENT_LEFT_CLICK";
        case EVENT_LEFT_DOUBLE_CLICK:
            return "EVENT_LEFT_DOUBLE_CLICK";
        case EVENT_RIGHT_CLICK:
            return "EVENT_RIGHT_CLICK";
        case EVENT_RIGHT_DOUBLE_CLICK:
            return "EVENT_RIGHT_DOUBLE_CLICK";
        case EVENT_KEYPRESS:
            return "EVENT_KEYPRESS";
        case EVENT_GOT_FOCUS:
            return "EVENT_GOT_FOCUS";
        case EVENT_LOST_FOCUS:
            return "EVENT_LOST_FOCUS";
        case EVENT_IDLE:
            return "EVENT_IDLE";
        case EVENT_CLOSE:
            return "EVENT_CLOSE";
        case EVENT_PANEL_SIZE:
            return "EVENT_PANEL_SIZE";
        case EVENT_PANEL_MOVE:
            return "EVENT_PANEL_MOVE";
        case EVENT_END_TASK:
            return "EVENT_END_TASK";
        case EVENT_TIMER_TICK:
            return "EVENT_TIMER_TICK";
        case EVENT_DISCARD:
            return "EVENT_DISCARD";
        case EVENT_EXPAND:
            return "EVENT_EXPAND";
        case EVENT_COLLAPSE:
            return "EVENT_COLLAPSE";
        case EVENT_DRAG:
            return "EVENT_DRAG";
        case EVENT_DROP:
            return "EVENT_DROP";
        case EVENT_DROPPED:
            return "EVENT_DROPPED";
        case EVENT_SORT:
            return "EVENT_SORT";
        case EVENT_SELECTION_CHANGE:
            return "EVENT_SELECTION_CHANGE";
        case EVENT_HSCROLL:
            return "EVENT_HSCROLL";
        case EVENT_VSCROLL:
            return "EVENT_VSCROLL";
        default:
            sprintf(pThreadLocalVars->evtBuf, "Event %d", event);
            return pThreadLocalVars->evtBuf;
        }
}
#endif  // HAVE_CVI_RTE

/**************************************************************************/

    /*  Multiple should be > 0.   Rounds to next multiple AWAY from zero */
int CVIFUNC RoundToNextMultiple(int number, int multiple)
{
    int r;

    r = number % multiple;
    if (r == 0)
        return number;
    else
    if (r < 0)
        return number - multiple - r;
    else
        return number + multiple - r;
}


#if _NI_mswin16_ || _NI_mswin32_
/**************************************************************************/

static char  * CVIFUNC ToolBox_GetDAQErrorString (short int error)
{
    char * errorStr = NULL;

    if ((errorStr = ToolBox_InternalGetDAQErrorString (error)) == NULL)
        {
        /* It may be a warning code that doesn't have a description
           so use error description instead.                      */

        errorStr = ToolBox_InternalGetDAQErrorString ((short int)(-error));
        }

    if (errorStr == NULL)
        errorStr = "No description available. ";

    return errorStr;
}

/**************************************************************************/

static char * CVIFUNC ToolBox_InternalGetDAQErrorString (short int error)
{

    switch (error)
    {
        case 0:
            return ("No error. ");
        case -9000:
            return("cmdSyntaxErr: An error was detected in the command string; the arrangement or ordering of the characters in the string is not consistent with the expected ordering. ");
        case -9001 :
            return("cmdLimitSyntaxErr: A syntax error was detected in the limit specification part of the command string.");
        case -9002 :
            return("cmdInterChannelRateSyntaxErr: A syntax error was detected in the interchannel sample rate specification part of the command string.");
        case -10001:
            return("syntaxErr: An error was detected in the input string; the arrangement or ordering of the characters in the string is not consistent with the expected ordering. ");
        case -10002:
            return("semanticsErr: An error was detected in the input string; the syntax of the string is correct, but certain values specified in the string are inconsistent with other values specified in the string. ");
        case -10003:
            return("invalidValueErr: The value of a numeric parameter is invalid. ");
        case -10004:
            return("valueConflictErr: The value of a numeric parameter is inconsistent with another parameter, and the combination is therefore invalid. ");
        case -10005:
            return("badDeviceErr: The device or task ID parameter is invalid. ");
        case -10006:
            return("badLineErr: The line parameter is invalid. ");
        case -10007:
            return("badChanErr: A channel is out of range for the device type or input configuration, the combination of channels is not allowed, or you must reverse the scan order so that channel 0 is last. ");
        case -10008:
            return("badGroupErr: The group parameter is invalid. ");
        case -10009:
            return("badCounterErr: The counter parameter is invalid. ");
        case -10010:
            return("badCountErr: The count parameter is too small or too large for the specified counter. ");
        case -10011:
            return("badIntervalErr: The interval parameter is too small or too large for the associated counter or I/O channel. ");
        case -10012:
            return("badRangeErr: The analog input or analog output voltage range is invalid for the specified channel. ");
        case 10012:
            return("badRangeWarn: The analog input or analog output voltage range is invalid for the specified channel. ");
        case -10013:
            return("badErrorCodeErr: The driver returned an unrecognized or unlisted error code. ");
        case -10014:
            return("groupTooLargeErr: The group size is too large for the device. ");
        case -10015:
            return("badTimeLimitErr: The time limit parameter is invalid. ");
        case -10016:
            return("badReadCountErr: The read count parameter is invalid. ");
        case -10017:
            return("badReadModeErr: The read mode parameter is invalid. ");
        case -10018:
            return("badReadOffsetErr: The offset is unreachable. ");
        case -10019:
            return("badClkFrequencyErr: The frequency parameter is invalid. ");
        case -10020:
            return("badTimebaseErr: The timebase parameter is invalid. ");
        case -10021:
            return("badLimitsErr: The limits are beyond the range of the device. ");
        case -10022:
            return("badWriteCountErr: Your data array contains an incomplete update,"
                   " or you are trying to write past the end of the internal buffer,"
                   " or your output operation is continuous and the length of your array"
                   " is not a multiple of one half of the internal buffer size. ");
        case -10023:
            return("badWriteModeErr: The write mode is out of range or is disallowed. ");
        case -10024:
            return("badWriteOffsetErr: The write offset plus the write mark is greater than the internal buffer size or it must be set to 0. ");
        case 10025:
            return("limitsOutOfRangeWarn: The voltage limits are out of range for this device in the current configuration.\nAlternate limits were selected. ");
        case -10025:
            return("limitsOutOfRangeErr: The voltage limits are out of range for this device in the current configuration.\nAlternate limits were selected. ");
        case -10026:
            return("badInputBufferSpecification: The input buffer specification is invalid.\nThis error results if, for example, you try to configure a multiple-buffer acquisition for a device that does not support multiple-buffer acquisition. ");
        case -10027:
            return("badDAQEventErr: For DAQEvents 0 and 1, general value A must be greater than 0 and less than the internal buffer size.\nIf DMA is used for DAQEvent 1, general value A must divide the internal buffer size evenly,"
                   " with no remainder.\nIf the TIO-10 is used for DAQEvent 4, general value A must be 1 or 2.\nIf the TIO-10 is used for DAQEvent 4, general value A must be 1 or 2. ");
        case -10028:
            return("badFilterCutoffErr: The cutoff frequency is not valid for this device. ");
        case -10029:
            return("obsoleteFunctionErr: The function you are calling is no longer supported in this version of the driver. ");
        case -10030:
            return("badBaudRateErr: The specified baud rate for communicating with the serial port is not valid on this platform. ");
        case -10031:
            return("badChassisIDErr: The SCXI chassis specified does not correspond to a configured SCXI chassis. ");
        case -10032:
            return("badModuleSlotErr: The SCXI module slot that was specified is invalid or corresponds to an empty slot. ");
        case -10033:
            return("invalidWinHandleErr: The window handle passed to the function is invalid. ");
        case -10034:
            return("noSuchMessageErr: No configured message matches the one you tried to delete. ");
        case -10035:
            return("irrelevantAttributeError: The specified attribute is not relevant. ");
        case -10036:
            return("badYearError: The specified year is invalid. ");
        case -10037:
            return("badMonthError: The specified month is invalid. ");
        case -10038:
            return("badDayError: The specified day is invalid. ");
        case -10039:
            return("stringTooLongError: The specified input string is too long.\n"
                "For instance, DAQScope 5102 devices can only store a string up to 32 bytes"
                "in length on the calibration EEPROM.\nIn that case, please shorten the string. ");
        case -10040:
            return("badGroupSizeError: The group size is invalid. ");
        case -10041:
            return("badTaskIDError: The specified task ID is invalid.\n"
                "For instance, you may have connected a taskID from an Analog Input VI to a Digital I/O VI. ");
        case -10042:
            return("inappropriateControlCodeError: The specified control code is inappropriate for the current configuration or state. ");
        case -10043:
            return("badDivisorError: The specified divisor is invalid. ");
        case -10044:
            return("badPolarityError: The specified polarity is invalid. ");
        case -10045:
            return("badInputModeError: The specified input mode is invalid. ");
        case -10046:
            return("badExcitationError: The excitation value specified is not valid for this device. ");
        case -10047:
            return("badConnectionTypeError: The type of SCXI channel connection specified is not valid for this module. ");
        case -10048:
            return("badExcitationTypeError: The excitation type specified is not valid for this device. ");
        case -10050:
            return("badChanListError: There is more than one channel name in the channel list that corresponds to the same hardware channel. ");
        case -10079:
            return("badTrigSkipCountError: The trigger skip count is invalid. ");
        case -10080:
            return("badGainErr: The gain parameter is invalid. ");
        case -10081:
            return("badPretrigCountErr: The pretrigger sample count is invalid. ");
        case -10082:
            return("badPosttrigCountErr: The posttrigger sample count is invalid. ");
        case -10083:
            return("badTrigModeErr: The trigger mode is invalid. ");
        case -10084:
            return("badTrigCountErr: The trigger count is invalid. ");
        case -10085:
            return("badTrigRangeErr: The trigger range or trigger hysteresis window is invalid. ");
        case -10086:
            return("badExtRefErr: The external reference value is invalid. ");
        case -10087:
            return("badTrigTypeErr: The trigger type parameter is invalid. ");
        case -10088:
            return("badTrigLevelErr: The trigger level parameter is invalid. ");
        case -10089:
            return("badTotalCountErr: The total count specified is inconsistent with the buffer configuration and pretrigger scan count or with the device type. ");
        case -10090:
            return("badRPGErr: The individual range, polarity, and gain settings are valid but the combination specified is not allowed for this device. ");
        case -10091:
            return("badIterationsErr: The analog output buffer iterations count is not allowed.\nIt must be 0 (for indefinite iterations) or 1. ");
        case -10092:
            return("lowScanIntervalErr: Some devices require a time gap between the last sample in a scan and the start of the next scan.\n"
                   "The scan interval you have specified does not provide a large enough gap for the board.\n"
                   "See the SCAN_Start function in the language interface API for an explanation. ");
        case -10093:
            return("fifoModeErr: FIFO mode waveform generation cannot be used because at least one condition is not satisifed. ");
        case -10094:
            return("badCalDACconstError: The calDAC constant passed to the function is invalid. ");
        case -10095:
            return("badCalStimulusError: The calibration stimulus passed to the function is invalid. ");
        case -10096:
            return("badCalibrationConstantError: The specified calibration constant is invalid. ");
        case -10097:
            return("badCalOpError: The specified calibration operation is invalid. ");
        case -10098:
            return("badCalConstAreaError: The specified calibration constant area is invalid.\n"
                "For instance, the specified calibration constant area contains constants which cannot be modified outside the factory. ");
        case -10100:
            return("badPortWidthErr: The requested digital port width is not a multiple of the hardware port width. ");
        case -10120:
            return("gpctrBadApplicationErr: Invalid application used. ");
        case -10121:
            return("gpctrBadCounterNumberErr: Invalid counterNumber used. ");
        case -10122:
            return("gpctrBadParamValueErr: Invalid paramValue used. ");
        case -10123:
            return("gpctrBadParamIdErr: Invalid paramID used. ");
        case -10124:
            return("gpctrBadEntityIdErr: Invalid entityID used. ");
        case -10125:
            return("gpctrBadActionErr: Invalid action used. ");
        case -10126:
            return("gpctrSourceSelectError: Invalid source selected. ");
        case -10127:
            return("badCountDirError: The specified counter does not support the specified count direction. ");
        case -10128:
            return("badGateOptionError: The specified gating option is invalid. ");
        case -10129:
            return("badGateModeError: The specified gate mode is invalid. ");
        case -10130:
            return("badGateSourceError: The specified gate source is invalid. ");
        case -10131:
            return("badGateSignalError: The specified gate signal is invalid. ");
        case -10132:
            return("badSourceEdgeError: The specified source edge is invalid. ");
        case -10133:
            return("badOutputTypeError: The specified output type is invalid. ");
        case -10134:
            return("badOutputPolarityError: The specified output polarity is invalid. ");
        case -10135:
            return("badPulseModeError: The specified pulse mode is invalid. ");
        case -10136:
            return("badDutyCycleError: The specified duty cycle is invalid. ");
        case -10137:
            return("badPulsePeriodError: The specified pulse period is invalid. ");
        case -10138:
            return("badPulseDelayError: The specified pulse delay is invalid. ");
        case -10139:
            return("badPulseWidthError: The specified pulse width is invalid. ");
        case -10140:
            return("badFOUTportError: The specified frequency output (FOUT or FREQ_OUT) port is invalid. ");
        case -10141:
            return("badAutoIncrementModeError: The specified autoincrement mode is invalid. ");
        case -10150:
            return("CfgInvalidatedSysCalError: Hardware configuration has changed since last system calibration ");
        case -10151:
            return("sysCalOutofDateError: system calibration is out of date ");
        case -10180:
            return("badNotchFilterError: The specified notch filter is invalid. ");
        case -10181:
            return("badMeasModeError: The specified measurement mode is invalid. ");
        case -10200:
            return("EEPROMreadErr: Unable to read data from EEPROM. ");
        case -10201:
            return("EEPROMwriteErr: Unable to write data to EEPROM. ");
        case -10202:
            return("EEPROMwriteProtectionError: You cannot write into this location or area of your EEPROM because it is write-protected.\n"
                "You may be trying to store calibration constants into a write-protected area;  if this is the case, you should select user area of the EEPROM instead. ");
        case -10203:
            return("EEPROMinvalidLocationError: The specified EEPROM location is invalid. ");
        case -10204:
            return("EEPROMinvalidPasswordError: The password for accessing the EEPROM is incorrect. ");
        case -10240:
            return("noDriverErr: The driver interface could not locate or open the driver. ");
        case -10241:
            return("oldDriverErr: The driver is out of date. ");
        case -10242:
            return("functionNotFoundErr: The specified function is not located in the driver. ");
        case -10243:
            return("configFileErr: The driver could not locate or open the configuration file, or the format of the configuration file is not compatible with the currently installed driver. ");
        case -10244:
            return("deviceInitErr: The driver encountered a hardware-initialization error while attempting to configure the specified device. ");
        case -10245:
            return("osInitErr: The driver encountered an operating system error while attempting to perform an operation, or the operating system does not support an operation performed by the driver. ");
        case -10246:
            return("communicationsErr: The driver is unable to communicate with the specified external device. ");
        case -10247:
            return("cmosConfigErr: The CMOS configuration memory for the computer is empty or invalid, or the configuration specified does not agree with the current configuration of the computer. ");
        case -10248:
            return("dupAddressErr: The base addresses for two or more devices are the same; consequently, the driver is unable to access the specified device. ");
        case -10249:
            return("intConfigErr: The interrupt configuration is incorrect given the capabilities of the computer or device. ");
        case -10250:
            return("dupIntErr: The interrupt levels for two or more devices are the same. ");
        case -10251:
            return("dmaConfigErr: The DMA configuration is incorrect given the capabilities of the computer/DMA controller or device. ");
        case -10252:
            return("dupDMAErr: The DMA channels for two or more devices are the same. ");
        case -10253:
            return("switchlessBoardErr: NI-DAQ was unable to find one or more switchless boards you have configured using WDAQCONF. ");
        case -10254:
            return("DAQCardConfigErr: Cannot configure the DAQCard because:\n"
                   "1) The correct version of card and socket services software is not installed.\n"
                   "2) The card in the PCMCIA socket is not a DAQCard.\n"
                   "3) The base address and/or interrupt level requested are not available according to the card and socket services resource manager.\n"
                   "Try different settings or use AutoAssign in the NIDAQ configuration utility. ");
        case -10255:
            return("remoteChassisDriverInitErr: There was an error in initializing the driver for Remote SCXI. ");
        case -10256:
            return("comPortOpenErr: There was an error in opening the specified COM port. ");
        case -10257:
            return("baseAddressErr: Bad base address specified in the config utility. ");
        case -10258:
            return("dmaChannel1Err: Bad dma channel 1 specified in the config utility. ");
        case -10259:
            return("dmaChannel2Err: Bad dma channel 2 specified in the config utility. ");
        case -10260:
            return("dmaChannel3Error: Bad DMA channel 3 specified in Measurement & Automation Explorer or by the operating system. ");
        case -10261:
            return("userModeToKernelModeCallError: The user mode code failed when calling the kernel mode code. ");
        case -10340:
            return("noConnectErr: No RTSI signal/line is connected, or the specified signal and the specified line are not connected. ");
        case -10341:
            return("badConnectErr: The RTSI signal/line cannot be connected as specified. ");
        case -10342:
            return("multConnectErr: The specified RTSI signal is already being driven by a RTSI line, or the specified RTSI line is already being driven by a RTSI signal. ");
        case -10343:
            return("SCXIConfigErr: The specified SCXI configuration parameters are invalid, or the function cannot be executed given the current SCXI configuration. ");
        case -10344:
            return("chassisNotSynchedErr: The Remote SCXI unit is not synchronized with the host. Reset the chassis again to resynchronize it with the host. ");
        case -10345:
            return("chassisMemAllocErr: The required amount of memory cannot be allocated on the Remote SCXI unit for the specified operation. ");
        case -10346:
            return("badPacketErr: The packet received by the Remote SCXI unit is invalid.  Check your serial port cable connections. ");
        case -10347:
            return("chassisCommunicationErr: There was an error in sending a packet to the remote chassis.  Check your serial port cable connections. ");
        case -10348:
            return("waitingForReprogErr: The Remote SCXI unit is in reprogramming mode and is waiting for reprogramming commands from the host (NI-DAQ Configuration Utility). ");
        case -10349:
            return("SCXIModuleTypeConflictErr: The module ID read from the SCXI module conflicts with the configured module type. ");
        case -10350:
            return("CannotDetermineEntryModuleError: Neither an SCXI entry module (i.e.: the SCXI module cabled to the measurement device that performs the acquisition/control operation) "
                "has been specified by the user, nor can NI-DAQ uniquely determine the entry module for the current SCXI configuration. ");
        case -10360:
            return("DSPInitErr: The DSP driver was unable to load the kernel for its operating system. ");
        case -10370:
            return("badScanListErr: The scan list is invalid.\nThis error can result if, for example, you mix AMUX-64T channels and onboard channels, or if you scan multiplexed SCXI channels out of order. ");
        case -10380:
            return("invalidSignalSrcError: The specified signal source is invalid for the selected signal name. ");
        case -10381:
            return("invalidSignalNameError: The specified signal name is invalid. ");
        case -10382:
            return("invalidSrcSpecError: The specified source specification is invalid for the signal source or signal name. ");
        case -10383:
            return("invalidSignalDestError: The specified signal destination is invalid. ");
        case -10390:
            return("routingError: The routing manager was unable to complete the request due to a lack of resources, or because the required resources are reserved. ");
        case -10391:
            return("pfiBadLineError: The routing manager was unable to complete the request due to an invalid PFI line number. ");
        case -10392:
            return("pfiGPCTRNotRoutedError: The specified General Purpose Counter Output and/or Up/Down signal(s) are not routed to any PFI lines. ");
        case -10393:
            return("pfiDefaultLineUndefinedError: A default PFI line does not exist for the given signal.  You must specify the PFI line either explicitly in the VI, or through the PFI line configuration VI. ");
        case -10394:
            return("pfiDoubleRoutingError: Given PFI line is already reserved for a different signal, or given signal has already reserved a different PFI line. ");
        case -10400:
            return("userOwnedRsrcErr: The specified resource is owned by the user and cannot be accessed or modified by the driver. ");
        case -10401:
        case -60:
            return("unknownDeviceErr: The specified device is not a National Instruments product, or the driver does not support the device (for example, the driver was released before the device was supported). ");
        case -10402:
            return("deviceNotFoundErr: No device is located in the specified slot or at the specified address. ");
        case -10403:
            return("deviceSupportErr: The specified device does not support the requested action (the driver recognizes the device, but the action is inappropriate for the device). ");
        case -10404:
            return("noLineAvailErr: No line is available. ");
        case -10405:
            return("noChanAvailErr: No channel is available. ");
        case -10406:
            return("noGroupAvailErr: No group is available. ");
        case -10407:
            return("lineBusyErr: The specified line is in use. ");
        case -10408:
            return("chanBusyErr: The specified channel is in use. ");
        case -10409:
            return("groupBusyErr: The specified group is in use. ");
        case -10410:
            return("relatedLCGBusyErr: A related line, channel, or group is in use; if the driver configures the specified line, channel, or group,"
                   " the configuration, data, or handshaking lines for the related line, channel, or group will be disturbed. ");
        case -10411:
            return("counterBusyErr: The specified counter is in use. ");
        case -10412:
            return("noGroupAssignErr: No group is assigned, or the specified line or channel cannot be assigned to a group. ");
        case -10413:
            return("groupAssignErr: A group is already assigned, or the specified line or channel is already assigned to a group. ");
        case -10414:
            return("reservedPinErr: Selected signal indicates a pin reserved by NI-DAQ.\nYou cannot configure this pin yourself. ");
        case -10415:
            return("externalMuxSupportErr: This function does not support this device when an external multiplexer (such as an AMUX-64T or SCXI) is connected to it. ");
        case -10416:
            return("DSPDataPathBusyErr: Either DAQ or WFM can use a PC memory buffer, but not both at the same time. ");
        case -10417:
            return("SCXIModuleNotSupportedErr: At least one of the SCXI modules specified is not supported for the operation. ");
        case -10440:
            return("sysOwnedRsrcErr: The specified resource is owned by the driver and cannot be accessed or modified by the user. ");
        case -10441:
            return("memConfigErr: No memory is configured to support the current data transfer mode, or the configured memory does not support the current data transfer mode."
                   "  (If block transfers are in use, the memory must be capable of performing block transfers. ");
        case -10442:
            return("memDisabledErr: The specified memory is disabled or is unavailable given the current addressing mode. ");
        case -10443:
            return("memAlignmentErr: The transfer buffer is not aligned properly for the current data transfer mode.\n"
                   "For example, the memory buffer is at an odd address, is not aligned to a 32-bit boundary, is not aligned to a 512-bit boundary, and so on.\n"
                   "Alternatively, the driver is unable to align the buffer because the buffer is too small. ");
        case -10444:
            return("memFullErr: No more system memory is available on the heap;\n or no more memory is available on the device;\n or part of the DMA DAQ buffer is in an invalid DMA region (above 16M on AT machine)");
        case -10445:
            return("memLockErr: The transfer buffer cannot be locked into physical memory. ");
        case -10446:
            return("memPageErr: The transfer buffer contains a page break; system resources may require reprogramming when the page break is encountered. ");
        case -10447:
            return("memPageLockErr: The operating environment is unable to grant a page lock. ");
        case -10448:
            return("stackMemErr: The driver is unable to continue parsing a string input due to stack limitations. ");
        case -10449:
            return("cacheMemErr: A cache-related error occurred, or caching is not supported in the current mode. ");
        case -10450:
            return("physicalMemErr: A hardware error occurred in physical memory, or no memory is located at the specified address. ");
        case -10451:
            return("virtualMemErr: The driver is unable to make the transfer buffer contiguous in virtual memory and therefore cannot lock the buffer into physical memory; thus, you cannot use the buffer for DMA transfers. ");
        case -10452:
            return("noIntAvailErr: No interrupt level is available for use. ");
        case -10453:
            return("intInUseErr: The specified interrupt level is already in use by another device. ");
        case -10454:
            return("noDMACErr: No DMA controller is available in the system. ");
        case -10455:
            return("noDMAAvailErr: No DMA channel is available for use. ");
        case -10456:
            return("DMAInUseErr: The specified DMA channel is already in use by another device. ");
        case -10457:
            return("badDMAGroupErr: DMA cannot be configured for the specified group because it is too small, too large, or misaligned.\nConsult the user manual for the device in question to determine group ramifications with respect to DMA. ");
        case -10458:
            return("diskFullErr: The storage disk you specified is full. ");
        case -10459:
            return("DLLInterfaceErr: The DLL could not be called due to an interface error. ");
        case -10460:
            return("interfaceInteractionErr: You have attempted to mix LabVIEW 2.2 VIs and LabVIEW 3.0 VIs.\n"
                   "You may run an application consisting only of 2.2 VIs, then run the 2.2 Board Reset VI, before you can run any 3.0 VIs.\n"
                   "You may run an application consisting of only 3.0 VIs, then run the 3.0 Device Reset VI, before you can run any 2.2 VIs. ");
        case -10461:
            return("resourceReservedError: The specified resource is unavailable because it has already been reserved by another entity. ");
        case -10462:
            return("resourceNotReservedError: The specified resource has not been reserved, so the action is not allowed. ");
        case -10463:
            return("mdResourceAlreadyReservedError: Another entity has already reserved the requested resource. ");
        case -10464:
            return("mdResourceReservedError: Attempted to access a reserved resource that requires the usage of a key. ");
        case -10465:
            return("mdResourceNotReservedError: Attempting to lift a reservation off a resouce that previously had no reservation. ");
        case -10466:
            return("mdResourceAccessKeyError: The requested operation cannot be performed because the key supplied is invalid. ");
        case -10467:
            return("mdResourceNotRegisteredError: The resource requested is not registered with the minidriver. ");
        case -10480:
            return("muxMemFullErr: The scan list is too large to fit into the mux-gain memory of the board. ");
        case -10481:
            return("bufferNotInterleavedErr: You cannot use DMA to transfer data from two buffers. You may be able to use interrupts. ");
        case -10482:
            return("waveformBufferSizeError: You have specified channels with different waveform lengths.\n"
                "To fix the problem, ensure that the waveform data for every channel has the same number of array elements. ");
        case -10540:
            return("SCXIModuleNotSupportedError: At least one of the SCXI modules specified is not supported for the operation. ");
        case -10541:
            return("TRIG1ResourceConflict: CTRB1 will drive COUTB1, however CTRB1 will also drive TRIG1.\n"
                "This may cause unpredictable results when scanning the chassis. ");
        case -10542:
            return("matrixTerminalBlockError: This function requires that no Matrix terminal block is configured with the SCXI module. ");
        case -10543:
            return("noMatrixTerminalBlockError: This function requires that some matrix terminal block is configured with the SCXI module. ");
        case -10544:
            return("invalidMatrixTerminalBlockError: The type of matrix terminal block configured will not allow proper operation of this function with the given parameters. ");
        case -10560:
            return("invalidDSPhandleErr: The DSP handle input to the VI is not a valid handle. ");
        case -10561:
            return("DSPDataPathBusyError: Either DAQ or WFM can use a PC memory buffer, but not both at the same time. ");
        case -10600:
            return("noSetupErr: No setup operation has been performed for the specified resources. ");
        case -10601:
            return("multSetupErr: The specified resources have already been configured by a setup operation. ");
        case -10602:
            return("noWriteErr: No output data has been written into the transfer buffer. ");
        case -10603:
            return("groupWriteErr: The output data associated with a group must be for a single channel or must be for consecutive channels. ");
        case -10604:
            return("activeWriteErr: Once data generation has started, only the transfer buffers originally written to can be updated.\n"
                   "If DMA is active and a single transfer buffer contains interleaved channel data, new data must be provided for all output channels currently using the DMA channel. ");
        case -10605:
            return("endWriteErr: No data was written to the transfer buffer because the final data block has already been loaded. ");
        case -10606:
            return("notArmedErr: The specified resource is not armed. ");
        case -10607:
            return("armedErr: The specified resource is already armed. ");
        case 10608:
            return("noTransferInProgWarn: No transfer is in progress for the specified resource. ");
        case -10608:
            return("noTransferInProgErr: No transfer is in progress for the specified resource. ");
        case -10609:
            return("transferInProgErr: A transfer is already in progress for the specified resource. ");
        case -10610:
            return("transferPauseErr: A single output channel in a group cannot be paused if the output data for the group is interleaved. ");
        case -10611:
            return("badDirOnSomeLinesErr: Some of the lines in the specified channel are not configured for the transfer direction specified.\n"
                   "For a write transfer, some lines were configured for input.\n"
                   "For a read transfer, some lines were configured for output. ");
        case -10612:
            return("badLineDirErr: The specified line does not support the specified transfer direction. ");
        case -10613:
            return("badChanDirErr: The specified channel does not support the specified transfer direction. ");
        case -10614:
            return("badGroupDirErr: The specified group does not support the specified transfer direction. ");
        case -10615:
            return("masterClkErr: The clock configuration for the clock master is invalid. ");
        case -10616:
            return("slaveClkErr: The clock configuration for the clock slave is invalid. ");
        case -10617:
            return("noClkSrcErr: No source signal has been assigned to the clock resource. ");
        case -10618:
            return("badClkSrcErr: The specified source signal cannot be assigned to the clock resource. ");
        case -10619:
            return("multClkSrcErr: A source signal has already been assigned to the clock resource. ");
        case -10620:
            return("noTrigErr: No trigger signal has been assigned to the trigger resource. ");
        case -10621:
            return("badTrigErr: The specified trigger signal cannot be assigned to the trigger resource. ");
        case -10622:
            return("preTrigErr: The pretrigger mode is not supported or is not available in the current configuration, or no pretrigger source has been assigned. ");
        case -10623:
            return("postTrigErr: No posttrigger source has been assigned. ");
        case -10624:
            return("delayTrigErr: The delayed trigger mode is not supported or is not available in the current configuration, or no delay source has been assigned. ");
        case -10625:
            return("masterTrigErr: The trigger configuration for the trigger master is invalid. ");
        case -10626:
            return("slaveTrigErr: The trigger configuration for the trigger slave is invalid. ");
        case -10627:
            return("noTrigDrvErr: No signal has been assigned to the trigger resource. ");
        case -10628:
            return("multTrigDrvErr: A signal has already been assigned to the trigger resource. ");
        case -10629:
            return("invalidOpModeErr: The specified operating mode is invalid, or the resources have not been configured for the specified operating mode. ");
        case -10630:
            return("invalidReadErr: An attempt was made to read 0 bytes from the transfer buffer, or an attempt was made to read past the end of the transfer buffer. ");
        case -10631:
            return("noInfiniteModeErr: Continuous input or output transfers are not allowed in the current operating mode. ");
        case -10632:
            return("someInputsIgnoredErr: Certain inputs were ignored because they are not relevant in the current operating mode. ");
        case -10633:
            return("invalidRegenModeErr: This device does not support the specified analog output regeneration mode. ");
        case -10634:
            return("noContTransferInProgErr: No continuous (double buffered) transfer is in progress for the specified resource. ");
        case -10635:
            return("invalidSCXIOpModeErr:  Either the SCXI operating mode specified in a configuration call is invalid, or a module is in the wrong operating mode to execute the function call.");
        case -10636:
            return("noContWithSynchErr: You cannot start a continuous (double buffered) operation with a synchronous function call. ");
        case -10637:
            return("bufferAlreadyConfigErr: Attempt to configure a buffer after a buffer has already been configured. You can configure a buffer only once. ");
        case -10638:
            return("badClkDestError: The clock cannot be assigned to the specified destination. ");
        case -10670:
            return("rangeBadForMeasModeError: The input range is invalid for the configured measurement mode. ");
        case -10671:
            return("autozeroModeConflictError: Autozero cannot be enabled for the configured measurement mode. ");
        case -10680:
            return("badChanGainErr: All channels must have an identical setting for this device. ");
        case -10681:
            return("badChanRangeErr: All channels of this device must have the same range. ");
        case -10682:
            return("badChanPolarityErr: All channels of this device must have the same polarity. ");
        case -10683:
            return("badChanCouplingErr: All channels of this device must have the same coupling. ");
        case -10684:
            return("badChanInputModeErr: All channels of this device must have the same input range. ");
        case -10685:
            return("clkExceedsBrdsMaxConvRate: The clock rate selected exceeds the recommended maximum rate for this device. ");
        case -10686:
            return("scanListInvalidErr: A configuration change has invalidated the scan list. ");
        case -10687:
            return("bufferInvalidErr: A configuration change has invalidated the allocated buffer. ");
        case -10688:
            return("noTrigEnabledErr: The total number of scans and pretrigger scans implies that a trigger start is intended, but no trigger is enabled. ");
        case -10689:
            return("digitalTrigBErr: Digital trigger B is illegal for the total scans and pretrigger scans specified. ");
        case -10690:
            return("digitalTrigAandBErr: This device does not allow digital triggers A and B to be enabled at the same time. ");
        case -10691:
            return("extConvRestrictionErr: This device does not allow an external sample clock with an external scan clock, start trigger, or stop trigger. ");
        case -10692:
            return("chanClockDisabledErr: Cannot start the acquisition because the channel clock is disabled. ");
        case -10693:
            return("extScanClockErr: Cannot use an external scan clock when performing a single scan of a single channel. ");
        case -10694:
            return("unsafeSamplingFreqErr: The sampling frequency exceeds the safe maximum rate for the ADC, gains, and filters you are using. ");
        case -10695:
            return("DMAnotAllowedErr: You must use interrupts.\nDMA is not allowed. ");
        case -10696:
            return("multiRateModeErr: Multi-rate scanning can not be used with AMUX-64, SCXI, or pre-triggered acquisitions. ");
        case -10697:
            return("rateNotSupportedErr: NI-DAQ was unable to convert your timebase/interval pair to match the actual hardware capabilities of the specified board. ");
        case -10698:
            return("timebaseConflictErr: You cannot use this combination of scan and sample clock timebases for the specified board. ");
        case -10699:
            return("polarityConflictErr: You cannot use this combination of scan and sample clock source polarities for this operation, for the specified board. ");
        case -10700:
            return("signalConflictErr: You cannot use this combination of scan and convert clock signal sources for this operation, for the specified board. ");
        case -10701:
            return("noLaterUpdateErr: The call had no effect because the specified channel had not been set for later internal update. ");
        case -10702:
            return("prePostTriggerErr: Pretriggering and posttriggering cannot be used simultaneously on the Lab-PC+ or 1200 series boards. ");
        case -10710:
            return("noHandshakeModeErr: The specified port has not been configured for handshaking. ");
        case -10720:
            return("noEventCtrErr: The specified counter is not configured for event-counting operation. ");
        case -10740:
            return("SCXITrackHoldErr: A signal has already been assigned to the SCXI track-and-hold trigger line, or a control call was inappropriate because the specified module is not configured for one-channel operation. ");
        case -10780:
            return("sc2040InputModeErr: When you have an SC2040 attached to your device, all analog input channels must be configured for differential input mode. ");
        case -10781:
            return("outputTypeMustBeVoltageErr: The polarity of the output channel cannot be bipolar when outputting currents. ");
        case -10782:
            return("sc2040HoldModeErr: The specified operation cannot be performed with the SC-2040 configured in hold mode. ");
        case -10783:
            return("calConstPolarityConflictErr: Calibration constants in the load area have different polarity than current configuration. Loading constants from factory. ");
        case -10784:
            return("masterDeviceNotInPXISlot2: Your master device must be PXI slot number 2 in order to route clocks to slave devices ");
        case -10800:
            return("timeOutErr: The operation could not complete within the time limit. ");
        case -10801:
            return("calibrationErr: An error occurred during the calibration process. ");
        case -10802:
            return("dataNotAvailErr: The requested amount of data has not yet been acquired, or the acquisition has completed and no more data is available to read. ");
        case -10803:
            return("transferStoppedErr: The transfer has been stopped to prevent regeneration of output data. ");
        case -10804:
            return("earlyStopErr: The transfer stopped prior to reaching the end of the transfer buffer. ");
        case -10805:
            return("overRunErr: The clock source for the input transfer is faster than the maximum input-clock rate; the integrity of the data has been compromised.\n"
                   "Alternatively, the clock source for the output transfer is faster than the maximum output-clock rate; a data point was generated more than once since the update occurred before new data was available. ");
        case -10806:
            return("noTrigFoundErr: No trigger value was found in the input transfer buffer. ");
        case -10807:
            return("earlyTrigErr: The trigger occurred before sufficient pretrigger data was acquired. ");
        case -10808:
            return("LPTCommunicationErr: An error occurred in the parallel port communication with the SCXI-1200. ");
        case -10809:
            return("gateSignalErr: Attempted to start a pulse width measurement with the pulse in the active state. ");
        case -10810:
            return("internalDriverlErr: An unexpected error occurred inside the driver when performing this operation. ");
        case -10811:
            return("internalKernelErr: An unexpected error occurred inside the device kernel when performing this operation. ");
        case -10840:
            return("softwareErr: The contents or the location of the driver file was changed between accesses to the driver. ");
        case -10841:
            return("firmwareErr: The firmware does not support the specified operation, or the firmware operation could not complete due to a data-integrity problem. ");
        case -10842:
            return("hardwareErr: The hardware is not responding to the specified operation, or the response from the hardware is not consistent with the functionality of the hardware. ");
        case -10843:
            return("underFlowErr: The update rate exceeds your system's capacity to supply data to the output channel. ");
        case -10844:
            return("underWriteErr: At the time of the update for the device-resident memory, insufficient data was present in the output transfer buffer to complete the update. ");
        case -10845:
            return("overFlowErr: At the time of the update clock for the input channel, the device-resident memory was unable to accept additional data - one or more data points may have been lost. ");
        case -10846:
            return("overWriteErr: New data was written into the input transfer buffer before the old data was retrieved. ");
        case -10847:
            return("dmaChainingErr: New buffer information was not available at the time of the DMA chaining interrupt; DMA transfers will terminate at the end of the currently active transfer buffer. ");
        case -10848:
            return("noDMACountAvailErr: The driver could not obtain a valid reading from the transfer-count register in the DMA controller. ");
        case -10849:
            return("openFileErr: Unable to open a file. ");
        case -10850:
            return("closeFileErr: Unable to close a file. ");
        case -10851:
            return("fileSeekErr: Unable to seek within a file. ");
        case -10852:
            return("readFileErr: Unable to read from a file. ");
        case -10853:
            return("writeFileErr: Unable to write to a file. ");
        case -10854:
            return("miscFileErr: An error occurred accessing a file. ");
        case -10855:
            return("osUnsupportedErr: NI-DAQ does not support the current operation on this particular version of the operating system. ");
        case -10856:
            return("osErr: An unexpected error occurred from the operating system while performing the given operation. ");
        case -10857:
            return("internalKernelError: An unexpected error occurred inside the kernel of the device while performing this operation. ");
        case -10858:
            return("hardwareConfigChangedError: The system has reconfigured the device and has invalidated the existing configuration.\n"
                "The device requires reinitialization to be used again. ");
        case -10880:
            return("updateRateChangeErr: A change to the update rate is not possible at this time because:\n"
                   "1) When waveform generation is in progress, you cannot change the interval timebase.\n"
                   "2) When you make several changes in a row, you must give each change enough time to take effect before requesting further changes. ");
        case -10881:
            return("partialTransferCompleteErr: You cannot do another transfer after a successful partial transfer. ");
        case -10882:
            return("daqPollDataLossErr: The data collected on the Remote SCXI unit was overwritten before it could be transferred to the buffer in the host.\n"
                   "Try using a slower data acquisition rate if possible. ");
        case -10883:
            return("wfmPollDataLossErr: New data could not be transferred to the waveform buffer of the Remote SCXI unit to keep up with the waveform update rate.\n"
                   "Try using a slower waveform update rate if possible. ");
        case -10884:
            return("pretrigReorderErr: Could not rearrange data after a pretrigger acquisition completed. ");
        case -10885:
            return("overLoadError: The input signal exceeded the input range of the ADC. ");
        case -10920:
            return("gpctrDataLossErr: One or more data points may have been lost during buffered GPCTR operations due to speed limitations of your system. ");
        case -10940:
            return("chassisResponseTimeoutErr: No response was received from the Remote SCXI unit within the specified time limit. ");
        case -10941:
            return("reprogrammingFailedErr: Reprogramming the Remote SCXI unit was unsuccessful.  Please try again. ");
        case -10942:
            return("invalidResetSignatureErr: An invalid reset signature was sent from the host to the Remote SCXI unit. ");
        case -10943:
            return("chassisLockupError: The interrupt service routine on the remote SCXI unit is taking longer than necessary.  You do not need to reset your remote SCXI unit, however, please clear and restart your data acquisition. ");
    }

    return NULL;
}
#endif /* _NI_mswin16 || _NI_mswin32_ */

/***************************************************************************/

    /* ConvertDataToText converts raw data into printable ASCII text.  It       */
    /* takes 3 bytes of data at a time and converts it to 4 bytes of text in    */
    /* the following manner.  The two highest order bits of each data byte are  */
    /* written into the lowest order six bits of the first text byte.  The      */
    /* remaining 6 bits of each data byte are written into the lowest order     */
    /* six bits of the remaining 3 text bytes.  This gives 4 text bytes with    */
    /* values from 0-63.  By adding '0' to these values, we have values within  */
    /* the range of the printable ASCII character set.                          */
    /* returns 0 if out of memory, 1 otherwise                                  */

#define CONVERSION_UNIT_MASK (0x03)
#define ASCII_OFFSET '0'      /* start at '0' to avoid '"' (a quote would be misinterpereted as a delimiter at the start or end of an .ini file value) */

int CVIFUNC ConvertDataToText(const unsigned char *dataPtr, long dataSize, unsigned char **textPtr)
{
    int             error = UIENoError;
    long            textSize, numTriples, remainder, i, dataIndex, textIndex;
    unsigned char   temp;
    unsigned char   *textArray;

    numTriples = dataSize / 3;
    remainder = dataSize % 3;

    /* each 3 bytes of data will become 4 bytes of text */
    textSize = 4 * numTriples;

    /* The remainder can be 0, 1, or 2.  If it is 0, no more space is needed. */
    /* If the remainder is 1, then 2 additional bytes will be needed.  If the */
    /* remainder is 2, then 3 additional bytes are needed.                    */
    if(remainder)
        textSize += remainder + 1;

    nullChk(*textPtr = (unsigned char *)malloc(textSize + 1));  /* +1 for NULL */
    textArray = *textPtr;
    for(i = 0; i < numTriples; i++)
        {
        dataIndex = 3 * i;
        textIndex = 4 * i;
        temp = (unsigned char)(((dataPtr[dataIndex] & (CONVERSION_UNIT_MASK << 6)) >> 2) |
                ((dataPtr[dataIndex + 1] & (CONVERSION_UNIT_MASK << 6)) >> 4) |
                ((dataPtr[dataIndex + 2] & (CONVERSION_UNIT_MASK << 6)) >> 6));
        textArray[textIndex] = temp + ASCII_OFFSET;
        temp = (unsigned char)(dataPtr[dataIndex] & ~(CONVERSION_UNIT_MASK << 6));
        textArray[textIndex + 1] = temp + ASCII_OFFSET;
        temp = (unsigned char)(dataPtr[dataIndex + 1] & ~(CONVERSION_UNIT_MASK << 6));
        textArray[textIndex + 2] = temp + ASCII_OFFSET;
        temp = (unsigned char)(dataPtr[dataIndex + 2] & ~(CONVERSION_UNIT_MASK << 6));
        textArray[textIndex + 3] = temp + ASCII_OFFSET;
        }

    switch(remainder)
        {
        case 0:
            break;
        case 1:
            dataIndex = 3 * numTriples;
            textIndex = 4 * numTriples;
            temp = (unsigned char)(dataPtr[dataIndex] & (CONVERSION_UNIT_MASK << 6)) >> 2;
            textArray[textIndex] = temp + ASCII_OFFSET;
            temp = (unsigned char)(dataPtr[dataIndex] & ~(CONVERSION_UNIT_MASK << 6));
            textArray[textIndex + 1] =  temp + ASCII_OFFSET;
            break;
        case 2:
            dataIndex = 3 * numTriples;
            textIndex = 4 * numTriples;
            temp = (unsigned char)(((dataPtr[dataIndex] & (CONVERSION_UNIT_MASK << 6)) >> 2) |
                    ((dataPtr[dataIndex + 1] & (CONVERSION_UNIT_MASK << 6)) >> 4));
            textArray[textIndex] = temp + ASCII_OFFSET;
            temp = (unsigned char)(dataPtr[dataIndex] & ~(CONVERSION_UNIT_MASK << 6));
            textArray[textIndex + 1] = temp + ASCII_OFFSET;
            temp = (unsigned char)(dataPtr[dataIndex + 1] & ~(CONVERSION_UNIT_MASK << 6));
            textArray[textIndex + 2] = temp + ASCII_OFFSET;
            break;
        default:
            Assert(FALSE);   /* Shouldn't have this value for remainder!!! */
            break;
        }
    textArray[textSize] = (unsigned char)0;

Error:
    if(error < 0)
        {
        free(*textPtr);
        *textPtr = NULL;
        }

    return error >= 0;
}

/***************************************************************************/

    /* ConvertTextToData converts text obtained from ConvertDataToText back     */
    /* into data.  See ConvertDataToText for more information.                  */
    /* returns 0 if out of memory, 1 otherwise                                  */

int CVIFUNC ConvertTextToData(const unsigned char *textPtr, long *dataSize, unsigned char **dataPtr)
{
    int             error = UIENoError;
    long            size, numQuads, remainder, i, dataIndex, textIndex, textSize;
    unsigned char   temp;
    unsigned char   *dataArray;

    textSize = strlen((char *)textPtr);
    numQuads = textSize / 4;
    remainder = textSize % 4;

    /* each 4 bytes of text will become 3 bytes of data */
    size = numQuads * 3;

    /* The remainder can be 0, 2, or 3.  It cannot be 1, since Convert-      */
    /*  DataToText never leaves a remainder of 1.  If it is 0, no more space     */
    /*  is needed.  If the remainder is 2, 1 byte will be needed.  If it's 3, */
    /* 2 are needed.                                                         */
    if(remainder)
        size += remainder - 1;

    if (size == 0)
        goto Error;

    nullChk(*dataPtr = (unsigned char *)malloc(size));
    dataArray = *dataPtr;
    for(i = 0; i < numQuads; i++)
        {
        textIndex = 4 * i;
        dataIndex = 3 * i;
        temp = textPtr[textIndex] - ASCII_OFFSET;
        dataArray[dataIndex] = ((temp & (CONVERSION_UNIT_MASK << 4)) << 2) | (textPtr[textIndex + 1] - ASCII_OFFSET);
        dataArray[dataIndex + 1] = ((temp & (CONVERSION_UNIT_MASK << 2)) << 4) | (textPtr[textIndex + 2] - ASCII_OFFSET);
        dataArray[dataIndex + 2] = ((temp & CONVERSION_UNIT_MASK) << 6) | (textPtr[textIndex + 3] - ASCII_OFFSET);
        }

    switch(remainder)
        {
        case 0:
            break;
        case 2:
            textIndex = 4 * numQuads;
            dataIndex = 3 * numQuads;
            temp = textPtr[textIndex] - ASCII_OFFSET;
            dataArray[dataIndex] = ((temp & (CONVERSION_UNIT_MASK << 4)) << 2) | (textPtr[textIndex + 1] - ASCII_OFFSET);
            break;
        case 3:
            textIndex = 4 * numQuads;
            dataIndex = 3 * numQuads;
            temp = textPtr[textIndex] - ASCII_OFFSET;
            dataArray[dataIndex] = ((temp & (CONVERSION_UNIT_MASK << 4)) << 2) | (textPtr[textIndex + 1] - ASCII_OFFSET);
            dataArray[dataIndex + 1] = ((temp & (CONVERSION_UNIT_MASK << 2)) << 4) | (textPtr[textIndex + 2] - ASCII_OFFSET);
            break;
        default:
            Assert(FALSE);   /* Shouldn't have this value for remainder!!! */
            break;
        }

Error:
    if (dataSize)
        *dataSize = size;

    if(error < 0)
        {
        free(*dataPtr);
        *dataPtr = NULL;
        }

    return error >= 0;
}

/***************************************************************************/

typedef void    (CVIFUNC * ExgFunc)(char *, char *, long);

static  char *   CVIFUNC Partition(char *, char *, ExgFunc, CompareFunction, long);
static  char *   CVIFUNC MedOf3(char *, char *, ExgFunc, CompareFunction, long);
static  void     CVIFUNC ExchgL(long*, long*, long);
static  void     CVIFUNC ExchgW(short*, short*, long);
static  void     CVIFUNC ExchgB(char *, char *, long);

#define kStackSize  100

/*
    Sort n elements of size es pointed to by a.
    (*theCmp)(a,b) returns <0 if a<b, 0 if a==b, >0 if a>b.
    This quick sort algorithm is nonrecursive.
    The partition stack needs log2(N) elements.

NOTE:   This QSort function is used in the toolbox for cases when the
        toolbox is linked to the C libraries of 3rd party compilers.
        The CVI C library qsort() function is just a fast as this one,
        However, other vendors qsort() functions have been found to be
        much slower.
*/
static void  CVIFUNC QSort(char *array, long numElems, long elemSize,
                           CompareFunction cmpFn)
{
    char        *left, *right, *part;
    short       sPtr;
    ExgFunc     exgFn;
    char        *stack[kStackSize] = { 0 };
    int         cmpResult;

    if (numElems < 2)
        return;

    if (elemSize & 1)
        exgFn = ExchgB;
    else if (elemSize & 2)
        exgFn = (ExgFunc)ExchgW;
    else
        exgFn = (ExgFunc)ExchgL;
    left = array;
    right = array + elemSize * (numElems-1);
    sPtr = 0;

    do
        {
        if (right > left)
            {
            if (right == left + elemSize)              /* two element partition , do it quick */
                {
                cmpResult = cmpFn ? cmpFn(left, right)
                                  : ListMemBlockCmp(left, right, elemSize);
                if (cmpResult > 0)
                    exgFn(left, right, elemSize);
                left = right;
                }
            else if (right - elemSize == left + elemSize)
                {
                MedOf3(left, right, exgFn, cmpFn, elemSize);
                left = right;
                }
            else                                 /* do the comparing and swapping */
                {
                part = Partition(left, right, exgFn, cmpFn, elemSize);  /* sort smaller part now, save larger */
                if (part - left > right - part)
                    {
                    if (left != part - elemSize && sPtr < kStackSize-2)    /* no singletons on stack */
                        {
                        stack[sPtr++] = left;
                        stack[sPtr++] = part - elemSize;
                        }
                    left = part;
                    }
                else
                    {
                    if (right != part + elemSize && sPtr < kStackSize-2)   /* no singletons on stack */
                        {
                        stack[sPtr++] = part + elemSize;
                        stack[sPtr++] = right;
                        }
                    right = part;
                    }
                }
            }
        else    /* right <= left */
            {
            if (sPtr >= 2)
                {
                right = stack[--sPtr];
                left = stack[--sPtr];
                }
            else
                sPtr -= 2;      /* to terminate the loop */
            }
        } while (sPtr >= 0);
}

/***************************************************************************/

/*
Partition the array so that it is divided by value.
All elements less than value will be to its left and all elements
greater than value will be to its right.
*/
static char *  CVIFUNC Partition(char *left, char *right, ExgFunc exgFn, CompareFunction cmpFn, long elemSize)
{
    char *value, *lToRScan, *rToLScan;

    if (right > left)
        {
        value = MedOf3(left, right, exgFn, cmpFn, elemSize);
        right -= elemSize;
        if (value != right)
            {
            exgFn(value, right, elemSize);
            value = right;
            }
        rToLScan = right;
        lToRScan = left;
        do
            {
            if (cmpFn)
                {
                do lToRScan += elemSize;
                    while ((lToRScan < right) && (cmpFn(lToRScan, value) < 0));
                do rToLScan -= elemSize;
                    while ((rToLScan > left) && (cmpFn(rToLScan, value) > 0));
                }
            else
                {
                do lToRScan += elemSize;
                    while ((lToRScan < right) && (ListMemBlockCmp(lToRScan, value, elemSize) < 0));
                do rToLScan -= elemSize;
                    while ((rToLScan > left) && (ListMemBlockCmp(rToLScan, value, elemSize) > 0));
                }
            if (lToRScan < rToLScan)
                exgFn(lToRScan, rToLScan, elemSize);
            } while (lToRScan < rToLScan);

        if (lToRScan != value)
            exgFn(lToRScan, value, elemSize);
        return lToRScan;
        }
    return left;
}

/***************************************************************************/

static char *  CVIFUNC MedOf3(char * left, char * right, ExgFunc exgFn, CompareFunction cmpFn, long elemSize)
{
    char *p0, *p1, *p2;

    p0 = left;
    p2 = right;
    p1 = p0 + elemSize * (((p2-p0)/elemSize)>>1);
    if (cmpFn)
        {
        if (cmpFn(p0, p1) > 0)
            exgFn(p0, p1, elemSize);
        if (cmpFn(p0, p2) > 0)
            exgFn(p0, p2, elemSize);
        if (cmpFn(p1, p2) > 0)
            exgFn(p1, p2, elemSize);
        }
    else
        {
        if (ListMemBlockCmp(p0, p1, elemSize) > 0)
            exgFn(p0, p1, elemSize);
        if (ListMemBlockCmp(p0, p2, elemSize) > 0)
            exgFn(p0, p2, elemSize);
        if (ListMemBlockCmp(p1, p2, elemSize) > 0)
            exgFn(p1, p2, elemSize);
        }
    return p1;
}

/***************************************************************************/

static void  CVIFUNC ExchgL(register long *a, register long *b, long esize)
{
    register long n, c;
    for(n= (esize>>2); n; n--, a++, b++){ c= *a; *a= *b; *b= c; }
}

/***************************************************************************/

static void  CVIFUNC ExchgW(register short *a, register short *b, long esize)
{
    register long n;
    register short c;
    for(n= (esize>>1); n; n--, a++, b++){ c= *a; *a= *b; *b= c; }
}

/***************************************************************************/

static void  CVIFUNC ExchgB(register char * a, register char *b, long esize)
{
    register long n;
    register char c;
    for(n= esize; n; n--, a++, b++){ c= *a; *a= *b; *b= c; }
}

/***************************************************************************/


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Begin Windows-only Utilities
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

#if defined(_NI_mswin32_) && _NI_mswin32_

// Useful argument check
#ifdef VERIFY_PTR_ARG
#undef VERIFY_PTR_ARG
#endif // VERIFY_PTR_ARG
#define VERIFY_PTR_ARG(fCall) if ((fCall) == 0) \
{retVal = UIENullPointerPassed; goto Error;} else {}

#ifndef _INC_SHELLAPI
// Definitions from shellapi.h (which we don't include)
typedef struct _NOTIFYICONDATA {
    DWORD cbSize;
    HWND  hWnd;
    UINT  uID;
    UINT  uFlags;
    UINT  uCallbackMessage;
    HICON hIcon;
    TCHAR szTip[64];
    DWORD dwState;
    DWORD dwStateMask;
    TCHAR szInfo[256];
    UINT  uTimeout;
    TCHAR szInfoTitle[64];
    DWORD dwInfoFlags;
} NOTIFYICONDATA, *PNOTIFYICONDATA;
typedef HANDLE HDROP;
#define NIM_ADD         0x00000000
#define NIM_MODIFY      0x00000001
#define NIM_DELETE      0x00000002
#define NIF_MESSAGE     0x00000001
#define NIF_ICON        0x00000002
#define NIF_TIP         0x00000004

typedef struct _SHELLEXECUTEINFO{
    DWORD cbSize;
    ULONG fMask;
    HWND hwnd;
    LPCTSTR lpVerb;
    LPCTSTR lpFile;
    LPCTSTR lpParameters;
    LPCTSTR lpDirectory;
    int nShow;
    HINSTANCE hInstApp;

    // Optional members
    LPVOID lpIDList;
    LPCSTR lpClass;
    HKEY hkeyClass;
    DWORD dwHotKey;
    union {
        HANDLE hIcon;
        HANDLE hMonitor;
    };
    HANDLE hProcess;
} SHELLEXECUTEINFO, *LPSHELLEXECUTEINFO;

#define SE_ERR_FNF              2       // file not found
#define SE_ERR_PNF              3       // path not found
#define SE_ERR_ACCESSDENIED     5       // access denied
#define SE_ERR_OOM              8       // out of memory
#define SE_ERR_DLLNOTFOUND              32

/* error values for ShellExecute() beyond the regular WinExec() codes */
#define SE_ERR_SHARE                    26
#define SE_ERR_ASSOCINCOMPLETE          27
#define SE_ERR_DDETIMEOUT               28
#define SE_ERR_DDEFAIL                  29
#define SE_ERR_DDEBUSY                  30
#define SE_ERR_NOASSOC                  31


// Note CLASSKEY overrides CLASSNAME
#define SEE_MASK_CLASSNAME        0x00000001
#define SEE_MASK_CLASSKEY         0x00000003
// Note INVOKEIDLIST overrides IDLIST
#define SEE_MASK_IDLIST           0x00000004
#define SEE_MASK_INVOKEIDLIST     0x0000000c
#define SEE_MASK_ICON             0x00000010
#define SEE_MASK_HOTKEY           0x00000020
#define SEE_MASK_NOCLOSEPROCESS   0x00000040
#define SEE_MASK_CONNECTNETDRV    0x00000080
#define SEE_MASK_FLAG_DDEWAIT     0x00000100
#define SEE_MASK_DOENVSUBST       0x00000200
#define SEE_MASK_FLAG_NO_UI       0x00000400
#define SEE_MASK_UNICODE          0x00004000
#define SEE_MASK_NO_CONSOLE       0x00008000
#define SEE_MASK_ASYNCOK          0x00100000
#define SEE_MASK_HMONITOR         0x00200000

#endif /* ndef _INC_SHELLAPI */

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Dynamic linking support (to avoid static linking to SDK libs)
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

static HMODULE g_hKERNEL32                 = NULL;
static HANDLE  g_hKERNEL32Mutex            = NULL;
static int     g_KERNEL32LibInitComplete   = FALSE;
static HMODULE g_hSHELL32                  = NULL;
static HANDLE  g_hSHELL32Mutex             = NULL;
static int     g_SHELL32LibInitComplete    = FALSE;

static BOOL (WINAPI *g_pGetDiskFreeSpaceEx)(LPCTSTR directory, PULARGE_INTEGER bytesFreeToCaller,
                                            PULARGE_INTEGER totalBytes,
                                            PULARGE_INTEGER totalBytesFree);
static BOOL (WINAPI *g_pDragQueryPoint)    (HDROP hDrop, LPPOINT lppt);
static UINT (WINAPI *g_pDragQueryFile)     (HDROP hDrop, UINT iFile, LPTSTR lpszFile, UINT cch);
static VOID (WINAPI *g_pDragFinish)        (HDROP hDrop);
static VOID (WINAPI *g_pDragAcceptFiles)   (HWND hWnd, BOOL fAccept);
static BOOL (WINAPI *g_pShell_NotifyIcon)  (DWORD dwMessage, PNOTIFYICONDATA pnid);
static BOOL (WINAPI *g_pShellExecuteEx)    (LPSHELLEXECUTEINFO lpExecInfo);

static void KERNEL32CleanupIfNeeded        (void);
static void SHELL32CleanupIfNeeded         (void);
static int  GetKERNEL32FuncsIfNeeded       (void);
static int  GetSHELL32FuncsIfNeeded        (void);

static int  S_GetFreeDiskSpaceEx           (LPCTSTR directory, PULARGE_INTEGER bytesFreeToCaller,
                                            PULARGE_INTEGER totalBytes,
                                            PULARGE_INTEGER totalBytesFree, BOOL* returnOut);
static int  S_DragAcceptFiles              (HWND hWnd, BOOL fAccept);
static int  S_DragQueryPoint               (HDROP hDrop, LPPOINT lppt, BOOL* returnOut);
static int  S_DragQueryFile                (HDROP hDrop, UINT iFile, LPTSTR lpszFile, UINT cch,
                                            UINT* returnOut);
static int  S_DragFinish                   (HDROP hDrop);
static int  S_Shell_NotifyIcon             (DWORD dwMessage, PNOTIFYICONDATA pnid,
                                            BOOL* returnOut);
static int  S_ShellExecuteEx               (LPSHELLEXECUTEINFO lpExecInfo, BOOL* returnOut);

//-------------------------------------------------------------------------------------------------
// Function: GetKERNEL32FuncsIfNeeded
//
// Purpose:  If it hasn't been done already, load kernel32.dll and retrieve pointers to required
//           functions, storing them globally.
//
// Return:   0 for success or a ToolErr error code.
//-------------------------------------------------------------------------------------------------
static int GetKERNEL32FuncsIfNeeded (void)
{
    DWORD  processID;
    char   mutexName[MAX_PATH + 1];
    HANDLE htmpMutex                = NULL;

    if (g_KERNEL32LibInitComplete)
        return 0;

    // Create and acquire a temporary mutex
    processID = GetCurrentProcessId ();
    sprintf (mutexName, "NI CVI Programmer's Toolbox library lock on KERNEL32.DLL handle - %d",
             processID);
    if (!(htmpMutex = CreateMutex (NULL, FALSE, mutexName)))
        return ToolErr_ErrorLoadingDLL;
    WaitForSingleObject (htmpMutex, INFINITE);

    // We have the temporary mutex, does one a global one for the DLL handle already exist?
    if (g_hKERNEL32Mutex)
        {

        // Yes, kill the temporary one and get the global one
        ReleaseMutex (htmpMutex);
        CloseHandle (htmpMutex);
        WaitForSingleObject (g_hKERNEL32Mutex, INFINITE);
        }
    else

        // No, this will be the global one
        g_hKERNEL32Mutex = htmpMutex;

    // We now have a mutex on the DLL handle, check again to make sure another thread didn't sneak in
    if (g_KERNEL32LibInitComplete)
        {
        ReleaseMutex (g_hKERNEL32Mutex);
        return 0;
        }

    // Load the DLL and get the pointers
    if (!(g_hKERNEL32 = LoadLibrary ("kernel32.dll")))
        {
        ReleaseMutex (g_hKERNEL32Mutex);
        return ToolErr_ErrorLoadingDLL;
        }
    if (!(g_pGetDiskFreeSpaceEx = (void*) GetProcAddress (g_hKERNEL32, "GetDiskFreeSpaceExA")))
        goto FuncNotFoundError;

    // We're done, release the Mutex
    g_KERNEL32LibInitComplete = TRUE;
    ReleaseMutex(g_hKERNEL32Mutex);
    return 0;

// In the case of GetDiskFreeSpaceExA, it's not all that unlikely we'll get here...
FuncNotFoundError:

    FreeLibrary (g_hKERNEL32);
    g_hKERNEL32 = NULL;
    ReleaseMutex (g_hKERNEL32Mutex);
    CloseHandle (g_hKERNEL32Mutex);
    g_hKERNEL32Mutex = NULL;
    return ToolErr_FuncNotFound;
}

//-------------------------------------------------------------------------------------------------
// Function: KERNEL32CleanupIfNeeded
//
// Purpose:  Free mutex used for kernel32.dll if it's currently allocated.  We're not going to
//           unload the DLL here because we can't guarantee it not to crash from a RSC callback.
//
// Return:   void
//-------------------------------------------------------------------------------------------------
#if 0  /* Currently not used */
static void KERNEL32CleanupIfNeeded (void)
{
    if (g_KERNEL32LibInitComplete)
        {
        CloseHandle (g_hKERNEL32Mutex);
        g_hKERNEL32Mutex = NULL;
        g_KERNEL32LibInitComplete = FALSE;
        }
    return;
}
#endif
//-------------------------------------------------------------------------------------------------
// Function: GetSHELL32FuncsIfNeeded
//
// Purpose:  If it hasn't been done already, load shell32.dll and retrieve pointers to required
//           shell functions.
//
// Return:   0 for success or a ToolErr error-code
//-------------------------------------------------------------------------------------------------
static int GetSHELL32FuncsIfNeeded (void)
{
    char   mutexName[MAX_PATH + 1];
    DWORD  processID;
    HANDLE htmpMutex                = NULL;

    if (g_SHELL32LibInitComplete)
        return 0;

    // Create and acquire a temporary mutex
    processID = GetCurrentProcessId ();
    sprintf (mutexName, "NI CVI Programmer's Toolbox library lock on SHELL32.DLL handle - %d",
             processID);
    if (!(htmpMutex = CreateMutex (NULL, FALSE, mutexName)))
        return ToolErr_ErrorLoadingDLL;
    WaitForSingleObject (htmpMutex, INFINITE);

    // We have the temporary mutex, does one a global one for the DLL handle already exist?
    if (g_hSHELL32Mutex)
        {

        // Yes, get the global one
        ReleaseMutex (htmpMutex);
        CloseHandle (htmpMutex);
        WaitForSingleObject (g_hSHELL32Mutex, INFINITE);
        }
    else

        // No, make the temporary one the global one
        g_hSHELL32Mutex = htmpMutex;

    // We now have a mutex on the DLL handle, check again to make sure another thread didn't sneak in
    if (g_SHELL32LibInitComplete)
        {
        ReleaseMutex (g_hSHELL32Mutex);
        return 0;
        }

    // Go ahead and load the DLL and get pointers
    if (!(g_hSHELL32 = LoadLibrary ("shell32.dll")))
        {
        ReleaseMutex (g_hSHELL32Mutex);
        return ToolErr_ErrorLoadingDLL;
        }
    if (!(g_pDragQueryFile = (void*) GetProcAddress (g_hSHELL32, "DragQueryFile")))
        goto FuncNotFoundError;
    if (!(g_pDragQueryPoint = (void*) GetProcAddress (g_hSHELL32, "DragQueryPoint")))
        goto FuncNotFoundError;
    if (!(g_pDragAcceptFiles = (void*) GetProcAddress (g_hSHELL32, "DragAcceptFiles")))
        goto FuncNotFoundError;
    if (!(g_pDragFinish = (void*) GetProcAddress (g_hSHELL32, "DragFinish")))
        goto FuncNotFoundError;
    if (!(g_pShell_NotifyIcon = (void*)GetProcAddress (g_hSHELL32, "Shell_NotifyIconA")))
        goto FuncNotFoundError;
    if (!(g_pShellExecuteEx = (void*)GetProcAddress (g_hSHELL32, "ShellExecuteExA")))
        goto FuncNotFoundError;

    // We're done, release the mutex
    g_SHELL32LibInitComplete = TRUE;
    ReleaseMutex (g_hSHELL32Mutex);
    return 0;

    // We'll get here if we were unable to find a function
FuncNotFoundError:

    FreeLibrary (g_hSHELL32);
    g_hSHELL32 = 0;
    ReleaseMutex (g_hSHELL32Mutex);
    CloseHandle (g_hSHELL32Mutex);
    g_hSHELL32Mutex = NULL;
    return ToolErr_FuncNotFound;
}

//-------------------------------------------------------------------------------------------------
// Function: SHELL3232CleanupIfNeeded
//
// Purpose:  Free mutex used for shell32.dll if it's currently allocated.  We're not going to
//           unload the DLL here because we can't guarantee it not to crash from a RSC callback.
//
// Return:   void
//-------------------------------------------------------------------------------------------------
#if 0  /* Currently not used */
static void SHELL32CleanupIfNeeded (void)
{
    if (g_SHELL32LibInitComplete)
        {
        CloseHandle (g_hSHELL32Mutex);
        g_hSHELL32Mutex = NULL;
        g_SHELL32LibInitComplete = FALSE;
        }
    return;
}
#endif
//-------------------------------------------------------------------------------------------------
// Function: S_GetFreeDiskSpaceEx
//
// Purpose:  Call dynamically loaded GetDiskFreeSpaceEx function.
//
// Return:   SDK func return through returnOut, 0 for success or ToolErr directly.
//-------------------------------------------------------------------------------------------------
static int S_GetFreeDiskSpaceEx (LPCTSTR directory, PULARGE_INTEGER bytesFreeToCaller,
                                 PULARGE_INTEGER totalBytes, PULARGE_INTEGER totalBytesFree,
                                 BOOL* returnOut)
{
    int retVal;

    if (!(retVal = GetKERNEL32FuncsIfNeeded ()))
        *returnOut =  (*g_pGetDiskFreeSpaceEx) (directory, bytesFreeToCaller, totalBytes,
                                                 totalBytesFree);
    else
        {
        returnOut = FALSE;
        }
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: S_DragQueryFile
//
// Purpose:  Call a dynamically loaded SDK function
//
// Return:   SDK func return through returnOut, 0 for success or ToolErr directly.
//-------------------------------------------------------------------------------------------------
static int S_DragQueryFile (HDROP hDrop, UINT iFile, LPTSTR lpszFile, UINT cch, UINT* returnOut)
{
    int  retVal;

    if (!(retVal = GetSHELL32FuncsIfNeeded ()))
        *returnOut = (*g_pDragQueryFile) (hDrop, iFile, lpszFile, cch);
    else
        *returnOut = 0;
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: S_DragAcceptFiles
//
// Purpose:  Call a dynamically loaded SDK function
//
// Return:   0 for success or ToolErr error code
//-------------------------------------------------------------------------------------------------
static int S_DragAcceptFiles (HWND hWnd, BOOL fAccept)
{
    int retVal;

    if (!(retVal = GetSHELL32FuncsIfNeeded ()))
        (*g_pDragAcceptFiles) (hWnd, fAccept);
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: S_DragQueryPoint
//
// Purpose:  Call a dynamically loaded SDK function
//
// Return:   SDK func return through returnOut, 0 for success or ToolErr directly.
//-------------------------------------------------------------------------------------------------
static int S_DragQueryPoint (HDROP hDrop, LPPOINT lppt, BOOL* returnOut)
{
    int retVal;

    if (!(retVal = GetSHELL32FuncsIfNeeded ()))
        *returnOut = (*g_pDragQueryPoint) (hDrop, lppt);
    else
        *returnOut = FALSE;
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: S_DragFinish ()
//
// Purpose:  Call a dynamically loaded SDK function
//
// Return:   0 for success or ToolErr error code
//-------------------------------------------------------------------------------------------------
static int S_DragFinish (HDROP hDrop)
{
    int retVal;

    if (!(retVal = GetSHELL32FuncsIfNeeded ()))
        (*g_pDragFinish) (hDrop);
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: S_ShellNotifyIcon
//
// Purpose:  Call a dynamically loaded SDK function
//
// Return:   SDK func return through returnOut, 0 for success or ToolErr directly.
//-------------------------------------------------------------------------------------------------
static int S_Shell_NotifyIcon (DWORD dwMessage, PNOTIFYICONDATA pnid, BOOL* returnOut)
{
    int retVal;

    if (!(retVal = GetSHELL32FuncsIfNeeded ()))
        *returnOut = (*g_pShell_NotifyIcon) (dwMessage, pnid);
    else
        *returnOut = FALSE;
    return retVal;
}


//-------------------------------------------------------------------------------------------------
// Function: ShellExecuteEx
//
// Purpose:  Call a dynamically loaded SDK function
//
// Return:   SDK func return through returnOut, 0 for success or ToolErr directly.
//-------------------------------------------------------------------------------------------------
static int S_ShellExecuteEx(LPSHELLEXECUTEINFO lpExecInfo, BOOL* returnOut)
{
    int retVal;

    if (!(retVal = GetSHELL32FuncsIfNeeded ()))
        *returnOut = (*g_pShellExecuteEx)(lpExecInfo);
    else
        *returnOut = FALSE;
    return retVal;

}


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Win32 Registry function definitions.
//
// These are easy-to-use wrapper funcs. for reading/writing Reg strings, binary and UINT data.
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Function: RegWriteBinary
//
// Purpose:  Write raw binary data to a user-specified key value in the Registry.  Open the key if
//           exists or create a new key if it does not.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
int CVIFUNC RegWriteBinary (unsigned int userRootKey, const char* userSubKeyName, const char* userValName,
                            const unsigned char* userDataBuf, int userDataSize)
{
    int                 retVal          = 0;
    DWORD               keyDisposition;
    HKEY                huserKey;
    SECURITY_ATTRIBUTES keySecurity;

    VERIFY_PTR_ARG(userSubKeyName);
    VERIFY_PTR_ARG(userValName);
    VERIFY_PTR_ARG(userDataBuf);
    Assert(userSubKeyName);
    Assert(userValName);
    Assert(userDataBuf);
    keySecurity.lpSecurityDescriptor = NULL;
    keySecurity.bInheritHandle = FALSE;
    keySecurity.nLength = sizeof(keySecurity);
    if (RegCreateKeyEx ((HKEY)userRootKey, userSubKeyName, 0, "", REG_OPTION_NON_VOLATILE,
                        KEY_SET_VALUE, &keySecurity, &huserKey, &keyDisposition)
        != ERROR_SUCCESS)
        {
        retVal = ToolErr_CantOpenKey;
        goto Error;
        }
    if (RegSetValueEx (huserKey, userValName, 0, REG_BINARY, userDataBuf, userDataSize)
        != ERROR_SUCCESS)
        retVal = ToolErr_CantSetKeyValue;
    RegCloseKey (huserKey);

Error:
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: RegReadBinary
//
// Purpose:  Read raw binary data from the user-specified key value into the passed buffer.  Allow
//           the user to pass NULL and obtain the size of the value in the Registry in an output
//           param.  If their buffer is not big enough, return an error but still fill the output
//           parameter so they know how big it should be.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
int CVIFUNC RegReadBinary (unsigned int userRootKey, const char* userSubKeyName, const char* userValName,
                           unsigned char* userBuffer, unsigned int userBufSize, unsigned int* prealDataSize)
{
    int   retVal   = 0;
    DWORD sdkErr;
    HKEY  huserKey = NULL;
    DWORD keyType;

    VERIFY_PTR_ARG(userSubKeyName);
    VERIFY_PTR_ARG(userValName);
    VERIFY_PTR_ARG(prealDataSize);
    Assert(userSubKeyName);
    Assert(userValName);
    Assert(prealDataSize);
    if ((sdkErr = RegOpenKeyEx ((HKEY)userRootKey, userSubKeyName, 0, KEY_QUERY_VALUE, &huserKey))
        != ERROR_SUCCESS)
        {
        retVal = (sdkErr == ERROR_FILE_NOT_FOUND) ? ToolErr_MissingKey : ToolErr_CantOpenKey;
        goto Error;
        }
    if ((sdkErr = RegQueryValueEx (huserKey, userValName, NULL, &keyType, NULL, prealDataSize))
        != ERROR_SUCCESS)
        {
        retVal = (sdkErr == ERROR_FILE_NOT_FOUND) ? ToolErr_MissingKeyValue
                                                  : ToolErr_CantAccessKeyValue;
        goto Error;
        }
    if (keyType != REG_BINARY)
        {
        retVal = ToolErr_WrongKeyValueType;
        goto Error;
        }
    if (userBuffer)
        {
        if (*prealDataSize > userBufSize)

            // User's buffer's not big enough, return an error (the output size parameter will tell
            // them how big the buffer needs to be)
            retVal = UIEBufTooSmall;
        else
            {
            if (RegQueryValueEx (huserKey, userValName, NULL, NULL, userBuffer, prealDataSize)
                != ERROR_SUCCESS)
                retVal = ToolErr_CantAccessKeyValue;
            }
        }
Error:
    if (huserKey)
        RegCloseKey (huserKey);
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: RegWriteULong ()
//
// Purpose:  Write an unsigned long (DWORD) to a user-specified key value in the Registry.  Open
//           the key if it exists, or create a new key if it does not.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
int CVIFUNC RegWriteULong (unsigned int userRootKey, const char* userSubKeyName, const char* userValName,
                           unsigned long userUL, int bigEndian)
{
    int                 retVal          = 0;
    DWORD               keyDisposition;
    HKEY                huserKey        = NULL;
    SECURITY_ATTRIBUTES keySecurity;

    VERIFY_PTR_ARG(userSubKeyName);
    VERIFY_PTR_ARG(userValName);
    Assert(userSubKeyName);
    Assert(userValName);
    keySecurity.lpSecurityDescriptor = NULL;
    keySecurity.bInheritHandle = FALSE;
    keySecurity.nLength = sizeof(keySecurity);
    if (RegCreateKeyEx ((HKEY)userRootKey, userSubKeyName, 0, "", REG_OPTION_NON_VOLATILE,
                        KEY_SET_VALUE, &keySecurity, &huserKey, &keyDisposition)
        != ERROR_SUCCESS)
        {
        retVal = ToolErr_CantOpenKey;
        goto Error;
        }
    if (RegSetValueEx (huserKey, userValName, 0, (bigEndian) ? REG_DWORD_BIG_ENDIAN : REG_DWORD,
                         (BYTE *)&userUL, sizeof(unsigned long))
        != ERROR_SUCCESS)
        retVal = ToolErr_CantSetKeyValue;
    RegCloseKey (huserKey);

Error:
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: RegReadULong ()
//
// Purpose:  Read an unsigned long value from the user-specified key value.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
int CVIFUNC RegReadULong (unsigned int userRootKey, const char* userSubKeyName, const char* userValName,
                          unsigned long* puserLong, int bigEndian)
{
    int   retVal       = 0;
    DWORD sdkErr;
    DWORD keyType;
    DWORD realDataSize;
    HKEY  huserKey     = NULL;

    VERIFY_PTR_ARG(userSubKeyName);
    VERIFY_PTR_ARG(userValName);
    VERIFY_PTR_ARG(puserLong);
    Assert(userSubKeyName);
    Assert(userValName);
    Assert(puserLong);
    if ((sdkErr = RegOpenKeyEx ((HKEY)userRootKey, userSubKeyName, 0, KEY_QUERY_VALUE, &huserKey))
        != ERROR_SUCCESS)
        {
        retVal = (sdkErr == ERROR_FILE_NOT_FOUND) ? ToolErr_MissingKey : ToolErr_CantOpenKey;
        goto Error;
        }
    if ((sdkErr = RegQueryValueEx (huserKey, userValName, NULL, &keyType, NULL, &realDataSize))
        != ERROR_SUCCESS)
        {
        retVal = (sdkErr == ERROR_FILE_NOT_FOUND) ? ToolErr_MissingKeyValue
                                                  : ToolErr_CantAccessKeyValue;
        goto Error;
        }
    if (bigEndian)
        {
        if (keyType != REG_DWORD_BIG_ENDIAN)
            {
            retVal = ToolErr_WrongKeyValueType;
            goto Error;
            }
        }
    else
        {
        if ((keyType != REG_DWORD) && (keyType != REG_DWORD_LITTLE_ENDIAN))
            {
            retVal = ToolErr_WrongKeyValueType;
            goto Error;
            }
        }
    if (RegQueryValueEx (huserKey, userValName, NULL, NULL, (BYTE*)puserLong, &realDataSize)
        != ERROR_SUCCESS)
        retVal = ToolErr_CantAccessKeyValue;

Error:
    if (huserKey)
        RegCloseKey (huserKey);
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: RegWriteString
//
// Purpose:  Write an ASCII string to a user-specified key value in the Registry.  Open the key if
//           it exists, or create a new key if it does not.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
#if HAVE_CVI_RTE
int CVIFUNC RegWriteString (unsigned int userRootKey, const char* userSubKeyName, const char* userValName,
                            const unsigned char* userString)
{
    int                 retVal = 0;
    HKEY                huserKey;
    DWORD               keyDisposition;
    SECURITY_ATTRIBUTES keySecurity;
    int                 sizeAdd=GetCurrentPlatform()==kPlatformWinnt?0:1;

    VERIFY_PTR_ARG(userSubKeyName);
    VERIFY_PTR_ARG(userString);
    Assert(userSubKeyName);
    Assert(userString);
    keySecurity.lpSecurityDescriptor = NULL;
    keySecurity.bInheritHandle = FALSE;
    keySecurity.nLength = sizeof(keySecurity);
    if (RegCreateKeyEx ((HKEY)userRootKey, userSubKeyName, 0, "DfltClass",
                        REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, &keySecurity, &huserKey,
                        &keyDisposition)
        != ERROR_SUCCESS)
        {
        retVal = ToolErr_CantOpenKey;
        goto Error;
        }
    if (RegSetValueEx (huserKey, userValName, 0, REG_SZ, userString, strlen((char*)userString) + sizeAdd)
        != ERROR_SUCCESS)
        retVal = ToolErr_CantSetKeyValue;
    RegCloseKey (huserKey);

Error:
    return retVal;
}
#endif  // HAVE_CVI_RTE

//-------------------------------------------------------------------------------------------------
// Function: RegWriteStringArray
//
// Purpose:  Write an array of ASCII string to a user-specified key value in the Registry.
//           Open the key if it exists, or create a new key if it does not.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
#if HAVE_CVI_RTE
int CVIFUNC RegWriteStringArray (unsigned int userRootKey, const char* userSubKeyName, int userArrayItems,
                                const char* userValNames[], const char* userStrings[])
{
    int                 retVal=0,i=0;
    HKEY                huserKey;
    DWORD               keyDisposition;
    SECURITY_ATTRIBUTES keySecurity;
    int                 sizeAdd=GetCurrentPlatform()==kPlatformWinnt?0:1;

    VERIFY_PTR_ARG(userSubKeyName);
    VERIFY_PTR_ARG(userValNames);
    VERIFY_PTR_ARG(userStrings);
    Assert(userSubKeyName);
    Assert(userValNames);
    Assert(userStrings);
    if( userArrayItems<1 ) {
        retVal = UIEValueIsInvalidOrOutOfRange;
        goto Error;
    }
    keySecurity.lpSecurityDescriptor = NULL;
    keySecurity.bInheritHandle = FALSE;
    keySecurity.nLength = sizeof(keySecurity);
    if( RegCreateKeyEx((HKEY)userRootKey,userSubKeyName,0,"DfltClass",
                        REG_OPTION_NON_VOLATILE,KEY_SET_VALUE,&keySecurity,
                        &huserKey,&keyDisposition)!=ERROR_SUCCESS ) {
        retVal = ToolErr_CantOpenKey;
        goto Error;
    }
    for(;i<userArrayItems;i++) {
        VERIFY_PTR_ARG(userStrings[i]);
        Assert(userStrings[i]);
        if( RegSetValueEx(huserKey,userValNames[i],0,REG_SZ, (const BYTE *)userStrings[i],strlen((char*)userStrings[i])+sizeAdd)!=ERROR_SUCCESS ) {
            retVal = ToolErr_CantSetKeyValue;
            break;
        }
    }

    RegCloseKey (huserKey);

Error:
    return retVal;
}
#endif  // HAVE_CVI_RTE

//-------------------------------------------------------------------------------------------------
// Function: RegReadString ()
//
// Purpose:  Read an ASCII string from the user-specified key value into the passed buffer.  Allow
//           the user to pass NULL and simply obtain the size of the value in the Registry in an
//           output param.  If their buffer is not big enough, return an error but still fill the
//           output parameter so they know how big it should be.
//
//Return:    0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
int CVIFUNC RegReadString (unsigned int userRootKey, const char* userSubKeyName, const char* userValName,
                           unsigned char* userString, unsigned int userStringSize, unsigned int* prealStringSize)
{
    int   retVal   = 0;
    DWORD sdkErr;
    DWORD keyType;
    HKEY  huserKey = NULL;

    VERIFY_PTR_ARG(userSubKeyName);
    VERIFY_PTR_ARG(prealStringSize);
    Assert(userSubKeyName);
    Assert(prealStringSize);
    if ((sdkErr = RegOpenKeyEx ((HKEY)userRootKey, userSubKeyName, 0, KEY_QUERY_VALUE, &huserKey))
        != ERROR_SUCCESS)
        {
        retVal = (sdkErr == ERROR_FILE_NOT_FOUND) ? ToolErr_MissingKey : ToolErr_CantOpenKey;
        goto Error;
        }
    if ((sdkErr = RegQueryValueEx (huserKey, userValName, NULL, &keyType, NULL, prealStringSize))
        != ERROR_SUCCESS)
        {
        retVal = (sdkErr == ERROR_FILE_NOT_FOUND) ? ToolErr_MissingKeyValue
                                                  : ToolErr_CantAccessKeyValue;
        goto Error;
        }
    if (keyType != REG_SZ)
        {
        retVal = ToolErr_WrongKeyValueType;
        goto Error;
        }
    if (userString)
        {
        if (*prealStringSize > userStringSize)

            // User buffer's not big enough, return an error (the output size parameter will tell
            // them how big the buffer needs to be)
            retVal = UIEBufTooSmall;
        else
            {
            if (RegQueryValueEx (huserKey, userValName, NULL, NULL, userString, prealStringSize)
                != ERROR_SUCCESS)
                retVal = ToolErr_CantAccessKeyValue;
            }
        }

Error:
    if (huserKey)
        RegCloseKey (huserKey);
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: RegQueryInfoOnKey ()
//
// Purpose:  Query information on specific key and return number of subkeys, values and lengths
//           of the longest subkey name, longest value name and longest value.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
#if HAVE_CVI_RTE
int CVIFUNC RegQueryInfoOnKey(unsigned int userRootKey, const char *userSubKeyName, unsigned int *pKeys, unsigned int *pValues,
                                unsigned int *pMaxSubKeyLen, unsigned int *pMaxValueNameLen, unsigned int *pMaxValueLen)
{
    int     retVal=0;
    DWORD   sdkErr;
    DWORD   subKeys,maxSubKeyLen,values,maxValNameLen,maxValueLen;
    HKEY    huserKey=NULL;
    int     sizeAdd=GetCurrentPlatform()==kPlatformWinnt?0:1;

    VERIFY_PTR_ARG(userSubKeyName);
    Assert(userSubKeyName);

    if( (sdkErr=RegOpenKeyEx((HKEY)userRootKey,userSubKeyName,0,KEY_QUERY_VALUE,&huserKey))!=ERROR_SUCCESS ) {
        retVal = (sdkErr==ERROR_FILE_NOT_FOUND) ? ToolErr_MissingKey : ToolErr_CantOpenKey;
        goto Error;
    }
    if( (sdkErr=RegQueryInfoKey(huserKey,NULL,NULL,NULL,&subKeys,&maxSubKeyLen,NULL,&values,&maxValNameLen,&maxValueLen,NULL,NULL))!=ERROR_SUCCESS ) {
        retVal = ToolErr_CantAccessKeyValue;
        goto Error;
    }
    if( pKeys )
        *pKeys = subKeys;
    if( pValues )
        *pValues = values;
    if( pMaxSubKeyLen )
        *pMaxSubKeyLen = maxSubKeyLen - sizeAdd;
    if( pMaxValueNameLen )
        *pMaxValueNameLen = maxValNameLen;
    if( pMaxValueLen )
        *pMaxValueLen = maxValueLen;

Error:
    if( huserKey )
        RegCloseKey(huserKey);
    return retVal;
}
#endif  // HAVE_CVI_RTE

//-------------------------------------------------------------------------------------------------
// Function: RegEnumerateValue ()
//
// Purpose:  Enumerate value of specific key and return value name, data, and data type/
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
#if HAVE_CVI_RTE
int CVIFUNC RegEnumerateValue(unsigned int userRootKey, const char *userSubKeyName, unsigned int userIndex, char *valueName,
                                unsigned int *pValNameLen, void *value, unsigned int *pValueLen, int *pValueType)
{
    int     retVal=0;
    DWORD   sdkErr;
    HKEY    huserKey=NULL;
    char    itemName[MAX_PATH];
    DWORD   itemNameLen1=MAX_PATH,itemNameLen2=MAX_PATH,type1,type2,dataLen1=(DWORD)-1,dataLen2=(DWORD)-1;
    DWORD   origNameLen = 0, origValueLen = 0;
    int     platform=GetCurrentPlatform();

    VERIFY_PTR_ARG(userSubKeyName);
    Assert(userSubKeyName);
    if( valueName ) {   // If user wants valueName, he HAS to pass also its size (by reference)
        VERIFY_PTR_ARG(pValNameLen);
        Assert(pValNameLen);
        origNameLen = *pValNameLen;
    }
    if( value ) {       // If user wants data, he HAS to pass also its size (by reference)
        VERIFY_PTR_ARG(pValueLen);
        Assert(pValueLen);
        origValueLen = *pValueLen;
    }

    if( (sdkErr=RegOpenKeyEx((HKEY)userRootKey,userSubKeyName,0,KEY_QUERY_VALUE,&huserKey))!=ERROR_SUCCESS ) {
        retVal = (sdkErr==ERROR_FILE_NOT_FOUND) ? ToolErr_MissingKey : ToolErr_CantOpenKey;
        goto Error;
    }
    dataLen2 = *pValueLen;
    if( (sdkErr=RegEnumValue(huserKey,userIndex,itemName,&itemNameLen1,NULL,&type1,NULL,&dataLen1))!=ERROR_SUCCESS ) {
        retVal = ToolErr_CantAccessKeyValue;
        goto Error;
    }
    if( pValNameLen )
        *pValNameLen = itemNameLen1 - (platform==kPlatformWinnt?0:1);   // Get rid of NUL terminating character on non-NT platform
    if( pValueLen )
        *pValueLen = dataLen1;
    if( valueName && pValNameLen && itemNameLen1 > origNameLen
     || value && pValueLen && dataLen1 > origValueLen ) {   // User passed smaller buffer than he needs for the data
        retVal = UIEBufTooSmall;
        goto Error;
    }
    if( value && *pValueLen>0 )
        if( (sdkErr=RegEnumValue(huserKey,userIndex,itemName,&itemNameLen2,NULL,&type2,(BYTE*)value,&dataLen2))!=ERROR_SUCCESS
         || type1!=type2 || itemNameLen1!=itemNameLen2 || dataLen1!=dataLen2 ) {
            retVal = ToolErr_CantAccessKeyValue;
            goto Error;
        }
    if( valueName )
        strncpy(valueName,itemName,origNameLen<itemNameLen1?origNameLen:itemNameLen1+1);
    if( pValueType )
        switch( type1 ) {
            case REG_SZ:                            *pValueType = _REG_SZ; break;
            case REG_EXPAND_SZ:                     *pValueType = _REG_EXPAND_SZ; break;
            case REG_BINARY:                        *pValueType = _REG_BINARY; break;
            case REG_DWORD:                         *pValueType = _REG_DWORD; break;
            case REG_DWORD_BIG_ENDIAN:              *pValueType = _REG_DWORD_BIG_ENDIAN; break;
            case REG_LINK:                          *pValueType = _REG_LINK; break;
            case REG_MULTI_SZ:                      *pValueType = _REG_MULTI_SZ; break;
            case REG_RESOURCE_LIST:                 *pValueType = _REG_RESOURCE_LIST; break;
            case REG_FULL_RESOURCE_DESCRIPTOR:      *pValueType = _REG_FULL_RESOURCE_DESCRIPTOR; break;
            case REG_RESOURCE_REQUIREMENTS_LIST:    *pValueType = _REG_RESOURCE_REQUIREMENTS_LIST; break;
            case REG_NONE:
            default:                                *pValueType = _REG_NONE; break;
        }

Error:
    if( huserKey )
        RegCloseKey(huserKey);
    return retVal;
}
#endif  // HAVE_CVI_RTE


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Win32 system information function definitions.
//
// These are very straightforward wrappers for commonly used SDK system-info functions -- the idea
// is that they're easy.
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Function: GetWindowsColor
//
// Purpose:  Retrieve the 32-bit 0RGB color of a Win32 display element.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
int CVIFUNC GetWindowsColor (int displayElement, unsigned int* pcolor)
{
    int   retVal   = 0;
    DWORD winColor = 0;

    VERIFY_PTR_ARG(pcolor);

    // Verify the display element (GetSysColor doesn't seem to)
    if ((displayElement > 20) || (displayElement < 0))
        {
        retVal = ToolErr_InvalidDisplayElement;
        goto Error;
        }
    winColor = GetSysColor (displayElement);
    Assert(pcolor);
    *pcolor = ((winColor & 0xFF0000) >> 16) + (winColor & 0xFF00) + ((winColor & 0xFF) << 16);

Error:
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: GetCompName
//
// Purpose:  Retrieve the computer name and fill the user's buffer with it.  Assume the user's
//           buffer is at least MAX_COMPNAME_LEN bytes.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
int CVIFUNC GetCompName (char* nameBuf)
{
    int   retVal = 0;
    DWORD bufSize;

    VERIFY_PTR_ARG(nameBuf);
    bufSize = MAX_COMPNAME_LEN;
    Assert(nameBuf);
    if (GetComputerName (nameBuf, &bufSize) == FALSE)
        retVal = ToolErr_UnknownSystemError;

Error:
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: GetWindowsDirs
//
// Purpose:  Retrieve the Windows and Windows system directories and fill the user's buffers with
//           them (if they are non-NULL).  Assume the non-NULL buffers are at least
//           MAX_PATHNAME_LEN bytes.
//
// Return:   0 for success or ToolErr error code.
//-------------------------------------------------------------------------------------------------
int CVIFUNC GetWindowsDirs (char* winPathBuf, char* sysPathBuf)
{
    int retVal = 0;

    if (sysPathBuf)
        if (!GetSystemDirectory (sysPathBuf, MAX_PATHNAME_LEN))
            retVal = ToolErr_UnknownSystemError;
    if (winPathBuf)
        if (!GetWindowsDirectory (winPathBuf, MAX_PATHNAME_LEN))
            retVal = ToolErr_UnknownSystemError;

    // We're done
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: GetWinOSVersion
//
// Purpose:  Retrieve OS version information and fill the passed non-NULL user addresses.
//
// Return:   0 for success or ToolErr error code.
//-------------------------------------------------------------------------------------------------
int CVIFUNC GetWinOSVersion (unsigned long* pmajorVersion, unsigned long* pminorVersion,
                             unsigned long* pbuild, unsigned long* pplatform)
{
    int           retVal = 0;
    OSVERSIONINFO OSInfo;

    OSInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    if (!GetVersionEx (&OSInfo))
        {
        retVal = ToolErr_UnknownSystemError;
        goto Error;
        }
    if (pmajorVersion)
        *pmajorVersion = OSInfo.dwMajorVersion;
    if (pminorVersion)
        *pminorVersion = OSInfo.dwMinorVersion;
    if (pbuild)
        *pbuild = OSInfo.dwBuildNumber & 0xFFFF;
    if (pplatform)
        *pplatform = OSInfo.dwPlatformId;

Error:
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: GetKeyboardPreferences
//
// Purpose:  Retrieve information about the keyboard and fill non-NULL user addresses.
//
// Return:   o for success or ToolErr error code.
//-------------------------------------------------------------------------------------------------
int CVIFUNC GetKeyboardPreferences (int* pkeyboardType, int* pnumFuncKeys,
                                    unsigned long* prepeatDelay, unsigned long* pspeed)
{
    int retVal = 0;

    if (pkeyboardType)
        if (!(*pkeyboardType = GetKeyboardType (0)))
            retVal = ToolErr_UnknownSystemError;
    if (pnumFuncKeys)
        if (!(*pnumFuncKeys = GetKeyboardType (2)))
            retVal = ToolErr_UnknownSystemError;
    if (prepeatDelay)
        if (!SystemParametersInfo (SPI_GETKEYBOARDDELAY, 0, prepeatDelay, 0))
            retVal = ToolErr_UnknownSystemError;
    if (pspeed)
        if (!SystemParametersInfo (SPI_GETKEYBOARDSPEED, 0, pspeed, 0))
            retVal = ToolErr_UnknownSystemError;

    // We're done
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: SetKeyboardPreferences
//
// Purpose:  Set the keyboard speed and repeat delay from user inputs.
//
// Return:   0 for success or ToolErr error code.
//-------------------------------------------------------------------------------------------------
int CVIFUNC SetKeyboardPreferences (unsigned long repeatDelay, unsigned long speed)
{
    int   retVal = 0;
    DWORD oldRepeatDelay;

    if (!SystemParametersInfo (SPI_GETKEYBOARDDELAY, 0, &oldRepeatDelay, 0))
        {
        retVal = ToolErr_UnknownSystemError;
        goto Error;
        }
    if (!SystemParametersInfo (SPI_SETKEYBOARDDELAY, repeatDelay, 0, 0))
        {
        retVal = ToolErr_UnknownSystemError;
        goto Error;
        }
    if (!SystemParametersInfo (SPI_SETKEYBOARDSPEED, speed, 0, 0))
        {
        retVal = ToolErr_UnknownSystemError;
        SystemParametersInfo (SPI_SETKEYBOARDDELAY, oldRepeatDelay, 0, 0);
        }

Error:
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: GetScreenSaver
//
// Purpose:  Retrieve information about the current screensaver settings and fill the user's
//           non-NULL variables.
//
// Return:   0 for success ToolErr error code.
//-------------------------------------------------------------------------------------------------
int CVIFUNC GetScreenSaver (int* penabled, int* ptimeToActivate)
{
    int retVal = 0;

    if (penabled)
        {
        if (!SystemParametersInfo (SPI_GETSCREENSAVEACTIVE, 0, penabled, 0))
            {
            retVal = ToolErr_UnknownSystemError;
            goto Error;
            }
        }
    if (ptimeToActivate)
        if (!SystemParametersInfo (SPI_GETSCREENSAVETIMEOUT, 0, ptimeToActivate, 0))
            retVal = ToolErr_UnknownSystemError;

Error:
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: SetScreenSaver
//
// Purpose:  Set screensaver file, timeout and enabled state.  Since the screensaver has multiple
//           enabled values (it depends on the current executable), we need to write to the Reg.
//           when enabling it.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
#if HAVE_CVI_RTE
int CVIFUNC SetScreenSaver (int enabled, int timeToActivate, const char* ssFile)
{
    int retVal = 0;

    if (enabled)
        {
        VERIFY_PTR_ARG(ssFile);
        Assert(ssFile);
        if (RegWriteString (REGKEY_HKCU, "Control Panel\\Desktop", "SCRNSAVE.EXE", (const unsigned char *)ssFile))
            {
            retVal = ToolErr_UnknownSystemError;
            goto Error;
            }
        }
    if (!SystemParametersInfo (SPI_SETSCREENSAVEACTIVE, enabled, 0, SPIF_SENDCHANGE))
        retVal = ToolErr_UnknownSystemError;
    if (!SystemParametersInfo (SPI_SETSCREENSAVETIMEOUT, timeToActivate, 0, SPIF_SENDCHANGE))
        retVal = ToolErr_UnknownSystemError;

Error:
    return retVal;
}
#endif  // HAVE_CVI_RTE

//-------------------------------------------------------------------------------------------------
// Purpose: SetWallpaper
//
// Purpose: Set the desktop wallpaper bitmap.  We need to write to the Reg to fully change
//          tiled/centered settings.
//
// Return:  0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
#if HAVE_CVI_RTE
int CVIFUNC SetWallpaper (int tiled, const char* bitmapPath)
{
    int  retVal         = 0;
    unsigned char tileKeyVal[2];

    VERIFY_PTR_ARG(bitmapPath);
    tileKeyVal[1] = 0;
    tileKeyVal[0] = (tiled) ? 49:48;
    Assert(tileKeyVal);
    if (RegWriteString (REGKEY_HKCU, "Control Panel\\Desktop", "TileWallpaper", tileKeyVal))
        {
        retVal = ToolErr_UnknownSystemError;
        goto Error;
        }
    if (RegWriteString (REGKEY_HKCU, "Control Panel\\Desktop", "Wallpaper", (const unsigned char *)bitmapPath))
        {
        retVal = ToolErr_UnknownSystemError;
        goto Error;
        }
    if (!SystemParametersInfo (SPI_SETDESKWALLPAPER, 0, (char *)bitmapPath, SPIF_SENDCHANGE))
        retVal = ToolErr_UnknownSystemError;

Error:
    return retVal;
}
#endif  // HAVE_CVI_RTE

//-------------------------------------------------------------------------------------------------
// Function: GetWallpaper
//
// Purpose:  Get the current desktop wallpaper bitmap.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
#if HAVE_CVI_RTE
int CVIFUNC GetWallpaper (int* ptiled, char bitmapPath[])
{
    int  retVal         = 0;
    unsigned int  actualSize     = 0;
    unsigned char tileKeyVal[2];

    VERIFY_PTR_ARG(bitmapPath);
    VERIFY_PTR_ARG(ptiled);
    Assert(bitmapPath);
    Assert(ptiled);
    if (RegReadString (REGKEY_HKCU, "Control Panel\\Desktop", "TileWallpaper", tileKeyVal, 2,
                       &actualSize))
        {
        retVal = ToolErr_UnknownSystemError;
        goto Error;
        }
    Fmt (ptiled, "%d<%s", tileKeyVal); /* OK use of Fmt since multibyte is not an issue here */
    if (RegReadString (REGKEY_HKCU, "Control Panel\\Desktop", "Wallpaper", (unsigned char *)bitmapPath,
                       MAX_PATHNAME_LEN, &actualSize))
        {
        retVal = ToolErr_UnknownSystemError;
        goto Error;
        }

Error:
    return retVal;
}
#endif  // HAVE_CVI_RTE

//-------------------------------------------------------------------------------------------------
// Function: GetDiskSpace
//
// Purpose:  Determine the free drive space and copy it into the user's UInt64Type reference inputs.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
int CVIFUNC GetDiskSpace (const char* pdirectory, UInt64Type* ptotalBytes, UInt64Type* pbytesFree)
{
    int             retVal             = 0;
    BOOL            sdkRetVal;
    DWORD           sdkErr             = 0;
    DWORD           sectorsPerCluster;
    DWORD           bytesPerSector;
    DWORD           freeClusters;
    DWORD           totalClusters;
    ULARGE_INTEGER  totalBytesFree;

    VERIFY_PTR_ARG(pdirectory);
    VERIFY_PTR_ARG(pbytesFree);
    VERIFY_PTR_ARG(ptotalBytes);
    Assert(pdirectory);
    Assert(pbytesFree);
    Assert(ptotalBytes);

    // 0 user data first since GetLastError can't always give us a good error...
    memset (ptotalBytes, 0, sizeof(UInt64Type));
    memset (pbytesFree, 0, sizeof(UInt64Type));
    retVal = S_GetFreeDiskSpaceEx ((LPCTSTR)pdirectory, (PULARGE_INTEGER)(pbytesFree),
                                   (PULARGE_INTEGER)(ptotalBytes),
                                   (PULARGE_INTEGER)(&totalBytesFree), &sdkRetVal);
    if (!retVal && !sdkRetVal)
        {
        sdkErr = GetLastError ();
        if (sdkErr == ERROR_CALL_NOT_IMPLEMENTED)
            retVal = -1;
        }

    // If Ex wasn't there, try the old one...
    if (retVal)
        {
        if ((sdkRetVal = GetDiskFreeSpace (pdirectory, &sectorsPerCluster, &bytesPerSector,
                                          &freeClusters, &totalClusters)) != FALSE)
            {
            ptotalBytes->loBytes = bytesPerSector * sectorsPerCluster * totalClusters;
            ptotalBytes->hiBytes = 0;
            pbytesFree->loBytes = bytesPerSector * sectorsPerCluster * freeClusters;
            pbytesFree->hiBytes = 0;
            }
        }
    if (!sdkRetVal)
        {
        if (!sdkErr)
            sdkErr = GetLastError ();
        switch (sdkErr)
            {
            default:
                retVal = ToolErr_UnknownSystemError;
                break;
            }
        }

Error:
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: UInt64TypeCompareUInt
//
// Purpose:  Compare the unsigned UInt64Type to a native 32-bit unsigned int.
//
// Return:   0 if equal, < 0 if UInt64Item is smaller, > 0 if UInt64Item is greater
//-------------------------------------------------------------------------------------------------
int CVIFUNC UInt64TypeCompareUInt (UInt64Type UInt64Item, unsigned int UInt32Item)
{
    if (UInt64Item.hiBytes)
        return 1;
    else if (UInt64Item.loBytes == UInt32Item)
        return 0;
    else
        return (UInt64Item.loBytes > UInt32Item) ? 1 : -1;
}

//-------------------------------------------------------------------------------------------------
// Function: GetMemoryInfo
//
// Purpose:  Determine the available physical and virtual memory and fill the non-NULL user
//           addresses.
//
// Return:   0 for success or ToolErr error code.
//-------------------------------------------------------------------------------------------------
int CVIFUNC GetMemoryInfo (unsigned long* ppercentLoad,
                           unsigned long* ptotalPhysical, unsigned long* ptotalPage, unsigned long* ptotalVirtual,
                           unsigned long* pavailPhysical, unsigned long* pavailPage, unsigned long* pavailVirtual)
{
    MEMORYSTATUS memStat;

    memStat.dwLength = sizeof(memStat);
    GlobalMemoryStatus (&memStat);
    if (ppercentLoad)
        *ppercentLoad = memStat.dwMemoryLoad;
    if (ptotalPhysical)
        *ptotalPhysical = memStat.dwTotalPhys;
    if (pavailPhysical)
        *pavailPhysical = memStat.dwAvailPhys;
    if (ptotalPage)
        *ptotalPage = memStat.dwTotalPageFile;
    if (pavailPage)
        *pavailPage = memStat.dwAvailPageFile;
    if (ptotalVirtual)
        *ptotalVirtual = memStat.dwTotalVirtual;
    if (pavailVirtual)
        *pavailVirtual = memStat.dwAvailVirtual;
    return 0;
}

//-------------------------------------------------------------------------------------------------
// Function: GetCurrentUser
//
// Purpose:  Retrieve the current user's login name into a buffer they pass.  The buffer must be
//           at least bufferSize bytes.  If the user passes a NULL buffer, just return the needed
//           size via realNameSize with no error.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
int CVIFUNC GetCurrentUser (char nameBuf[], int bufSize, unsigned int* prealNameSize)
{
    int retVal = 0;

    VERIFY_PTR_ARG(prealNameSize);
    if (!nameBuf)
        bufSize = 0;
    Assert(prealNameSize);
    *prealNameSize = bufSize - 1;
    if (GetUserName (nameBuf, prealNameSize) == FALSE)

        // We failed, it might be OK
        if (nameBuf)
            retVal = UIEBufTooSmall;

Error:
    return retVal;
}


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Win32 message-callback functionality
//
// We keep a global List of MSG_CB_HANDLER objects -- each panel for which a user installs at least
// one message callback will get one of these handlers.  Each handler keeps a List of
// message-callback link (MSG_FUNC_LINK) objects.  Our handlers manage custom WndProcs for each
// panel, which, when they receive a message, will ask the handlers if any of their links are
// satisfied by the arriving message.  If they are, they will either call the user's callback
// directly or post a deferred call, as dictated by the user-configurable Intercept Mode attribute.
// These handlers also chain callbacks to user panels to monitor discard events and do cleanup if
// the user doesn't correctly remove the callback before getting rid of the panel.  We can't do
// this in the WndProc since we already get WM_DISCARD messages when the user changes an attribute
// that causes the window to be regenerated (like title bar attrs).
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

// Message-handling link (between a message and a callback function)
typedef struct {
    int            msgNum;
    int            callWhen;
    int            enabled;
    WinMsgCallback UserCallback;
    void*          callbackData;
} MSG_FUNC_LINK;

// Message-handler object
typedef struct {
    short    winMsgEnabled;
    int      hpanel;
    unsigned int      hwindow;
    int      processMsgs;
    ListType links;
    LONG     defaultCVIWndProc;
} MSG_CB_HANDLER;

// Message-info object for post-deferred calls
typedef struct {
    int            hpanel;
    int            msg;
    unsigned int   wParam;
    unsigned int   lParam;
    WinMsgCallback pcallbackFunc;
    void*          pcallbackData;
} MSG_DATA_REC;

// Static prototypes
static int  CVIFUNC     InstallWinMsgCallbackNoChecks (int hpanel, int msgNum,
                                                       WinMsgCallback pcallback, int callWhen,
                                                       void* pcallbackData, int* phwnd);
static void CVICALLBACK DeferredHandleWinMsgCallback  (void *msgInfoPtr);
static void CVICALLBACK DeferredReinstallHandler      (void* phandler);
static void CVIFUNC     RemoveMsgHandlerWndProc       (MSG_CB_HANDLER* phandler);
static void CVIFUNC     SetAllMsgCBHandlerProccesing  (int enabled);
static int  CVIFUNC     MakeHandlerListIfNeeded       (void);
static int  CVIFUNC     InsertMsgHandlerWndProc       (MSG_CB_HANDLER* phandler);
static int  CVIFUNC     FindMsgCBHandlerFromHPanel    (int hpanel,
                                                       MSG_CB_HANDLER** ptrToHandlerPtr);
static int  CVIFUNC     FindMsgCBHandlerFromHWND      (int hwindow,
                                                       MSG_CB_HANDLER** ptrToHandlerPtr);
static int  CVIFUNC     AddLinkToMsgCBHandler         (MSG_CB_HANDLER* phandler, int msgNum,
                                                       WinMsgCallback tempCallbackPtr,
                                                       int callWhen, void* callbackData);
static int  CVICALLBACK CompareMsgCBHandlerHPanels    (void *item1, void *item2);
static int  CVICALLBACK CompareMsgCBHandlerHWNDs      (void *item1, void *item2);
static int  CVICALLBACK CompareMsgCBLinkMsgs          (void *item1, void *item2);
static int  CVICALLBACK ChainedMsgCBPanelCallback     (int hpanel, int event, void *callbackData,
                                                       int eventData1, int eventData2);
static int  CVIFUNC     DiscardLinkFromHandler        (MSG_CB_HANDLER* phandler, int msgNum);
static void CVIFUNC     DiscardMsgCBHandlerFromPanel  (MSG_CB_HANDLER* phandler,
                                                       int handlerPosInList);
static int  CVIFUNC     CreateMsgCBHandlerForPanel    (int hpanel, MSG_CB_HANDLER** ppnewHandler);

// Global message-handler List
static ListType g_msgCBHandlers;

//-------------------------------------------------------------------------------------------------
// Function: InstallWinMsgCallback
//
// Purpose:  Attach a user's callback to a specific Windows message number when received by the
//           specified parent panel.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
#if HAVE_CVI_RTE
int CVIFUNC InstallWinMsgCallback (int hpanel, int msgNum, WinMsgCallback pcallback,
                                   int callWhen, void* pcallbackData, int* phwnd)
{
    int retVal         = 0;
    int parent;
    int userBOLEState;

    VERIFY_PTR_ARG(phwnd);
    if (!pcallback)
        {
        retVal = UIENullPointerPassed;
        goto Error;
        }
    if ((callWhen != VAL_MODE_IN_QUEUE) && (callWhen != VAL_MODE_INTERCEPT))
        {
        retVal = ToolErr_InvalidMsgCallbackMode;
        goto Error;
        }
    userBOLEState = GetBreakOnLibraryErrors ();
    SetBreakOnLibraryErrors (0);
    if (GetPanelAttribute (hpanel, ATTR_PANEL_PARENT, &parent) < 0)
        {
        retVal = UIENotAPanelHandle;
        SetBreakOnLibraryErrors (userBOLEState);
        goto Error;
        }
    SetBreakOnLibraryErrors (userBOLEState);
    if (parent)
        {
        retVal = ToolErr_PanelNotParent;
        goto Error;
        }
    retVal = InstallWinMsgCallbackNoChecks (hpanel, msgNum, pcallback, callWhen,
                                            pcallbackData, phwnd);

Error:
    return retVal;
}
#endif  // HAVE_CVI_RTE

//-------------------------------------------------------------------------------------------------
// Function: InstallWinMsgCallbackNoChecks
//
// Purpose:  Attach a user's callback to a specific Windows message number when received by the
//           specified parent panel.  This is an internally used version of
//           InstallWinMsgCallback that does not check the validity of inputs.
//
// Return:   0 for success or ToolErr/UIE error-code.
//-------------------------------------------------------------------------------------------------
#if HAVE_CVI_RTE
static int CVIFUNC InstallWinMsgCallbackNoChecks (int hpanel, int msgNum,
                                                  WinMsgCallback pcallback, int callWhen,
                                                  void* pcallbackData, int* phwnd)
{
    int             retVal            = 0;
    int             handlerPosInList;
    MSG_CB_HANDLER* phandler;

    Assert(pcallback);
    Assert(hpanel > 0);
    handlerPosInList = FindMsgCBHandlerFromHPanel (hpanel, &phandler);
    if (!handlerPosInList)
        {

        // No handler for this panel -- create one
        if ((retVal = CreateMsgCBHandlerForPanel (hpanel, &phandler)) != 0)
            goto Error;
        }

    // We have a pointer to an existing handler -- add a message-callback link to it
    if ((retVal = AddLinkToMsgCBHandler (phandler, msgNum, pcallback, callWhen, pcallbackData)) != 0)
        {
        if (!(phandler->winMsgEnabled))
            {
            DiscardMsgCBHandlerFromPanel (phandler, handlerPosInList);
            }
        }
    else
        *phwnd = phandler->hwindow;

Error:
    return retVal;
}
#endif  // HAVE_CVI_RTE

//-------------------------------------------------------------------------------------------------
// Function: RemoveWinMsgCallback
//
// Purpose:  Detach the callback from the specified message number and free associated memory.
//           Remove the message-function link from the handler's List, remove the List and the
//           Handler if there are no more links, and remove the List of handlers if there are no
//           more.
//
// Return:   0 for success or ToolErr/UIE error code
//-------------------------------------------------------------------------------------------------
#if HAVE_CVI_RTE
int CVIFUNC RemoveWinMsgCallback (int hpanel, int msgNum)
{
    int             returnVal         = 0;
    int             handlerPosInList;
    int             numLinksLeft;
    MSG_CB_HANDLER* phandler;

    if (!(handlerPosInList = FindMsgCBHandlerFromHPanel (hpanel, &phandler)))
        {
        returnVal = ToolErr_MsgCallbackNotInstalled;
        goto Error;
        }
    numLinksLeft = ListNumItems (phandler->links);
    if (DiscardLinkFromHandler (phandler, msgNum) == numLinksLeft)
        {

        // We couldn't remove it because it wasn't there
        returnVal = ToolErr_MsgCallbackNotInstalled;
        goto Error;
        }
    if (numLinksLeft == 1)

        // There will be no links now that we removed it, so kill the handler
        DiscardMsgCBHandlerFromPanel (phandler, handlerPosInList);

Error:
    return returnVal;
}
#endif  // HAVE_CVI_RTE

//-------------------------------------------------------------------------------------------------
// Function: DiscardLinkFromHandler
//
// Purpose:  Given a handler object, discard a particular message-function link and remove the
//           handler's List of links if necessary.
//
// Return:   The number of links left after the discard.  The number will not change if there was
//           no message link corresponding to the passed message.
//-------------------------------------------------------------------------------------------------
static int CVIFUNC DiscardLinkFromHandler (MSG_CB_HANDLER* phandler, int msgNum)
{
    int           numLinksLeft;
    int           linkPosInList;
    MSG_FUNC_LINK searchLink;

    searchLink.msgNum = msgNum;
    if ((linkPosInList = ListFindItem (phandler->links, &searchLink, FRONT_OF_LIST,
                                       CompareMsgCBLinkMsgs)) != 0)
        ListRemoveItem (phandler->links, 0, linkPosInList);
    if (!(numLinksLeft = ListNumItems (phandler->links)))
        {
        ListDispose (phandler->links);
        phandler->winMsgEnabled = FALSE;
        }
    return numLinksLeft;
}

//-------------------------------------------------------------------------------------------------
// Function: SetMsgCallbackAttribute
//
// Purpose:  Set an attribute of a specified message-callback link in the handler associated with
//           the specified panel.
//
// Return:   0 for success or ToolErr/UIE error-code
//-------------------------------------------------------------------------------------------------
int CVIFUNC_C SetMsgCallbackAttribute (int hpanel, int msgNum, int attribute, ...)
{
    int             intVal       = 0;
    int             posInList;
    int             retVal       = 0;
    MSG_FUNC_LINK   searchLink;
    MSG_FUNC_LINK*  plink;
    MSG_CB_HANDLER* phandler;
    va_list         parg         = 0;

    // Find the appropriate handler from the panel handle
    if (!(posInList = FindMsgCBHandlerFromHPanel (hpanel, &phandler)))
        retVal = ToolErr_MsgCallbackNotInstalled;
    else
        {
        va_start(parg, attribute);

        // Find the correct message-callback link within the handler's List
        searchLink.msgNum = msgNum;
        if (!(posInList = ListFindItem (phandler->links, &searchLink, FRONT_OF_LIST,
                                        CompareMsgCBLinkMsgs)))
            {
            retVal = ToolErr_MsgCallbackNotInstalled;
            va_end(parg);
            goto Error;
            }
        plink = ListGetPtrToItem (phandler->links, posInList);

        // Switch on the passed-in attribute and set it
        switch (attribute)
            {
            case ATTR_CALLBACK_MODE:
                intVal = va_arg(parg, int);
                if ((intVal == VAL_MODE_INTERCEPT) || (intVal == VAL_MODE_IN_QUEUE))
                    plink->callWhen = intVal;
                else
                    retVal = UIEBadAttributeValue;
                break;
            case ATTR_ENABLED:
                intVal = va_arg(parg, int);
                plink->enabled = intVal;
                break;
            default:
                retVal = UIEInvalidAttribute;
            }
        va_end(parg);
        }

Error:
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: GetMsgCallbackAttribute
//
// Purpose:  Get an attribute of a specified message-callback link in the passed panel's handler.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
int CVIFUNC GetMsgCallbackAttribute (int hpanel, int msgNum, int attribute, void* pval)
{
    int             posInList;
    int             retVal      = 0;
    MSG_FUNC_LINK   searchLink;
    MSG_FUNC_LINK*  plink;
    MSG_CB_HANDLER* phandler;

    VERIFY_PTR_ARG(pval);

    // Find the appropriate handler from the panel handle
    if (!(posInList = FindMsgCBHandlerFromHPanel (hpanel, &phandler)))
        retVal = ToolErr_MsgCallbackNotInstalled;
    else
        {

        // Find the correct message-callback link withing the handler's List
        searchLink.msgNum = msgNum;
        if (!(posInList = ListFindItem (phandler->links, &searchLink, FRONT_OF_LIST,
                                        CompareMsgCBLinkMsgs)))
            {
            retVal = ToolErr_MsgCallbackNotInstalled;
            goto Error;
            }
        plink = ListGetPtrToItem (phandler->links, posInList);

        // Switch on the passed-in attribute and get it
        switch (attribute)
            {
            case ATTR_CALLBACK_MODE:
                *(int*)pval = plink->callWhen;
                break;
            case ATTR_ENABLED:
                *(int*)pval = plink->enabled;
                break;
            default:
                retVal = UIEInvalidAttribute;
            }
        }

Error:
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: MsgHandlerWndProc
//
// Purpose:  Handle messages for the callback-enabled panel -- this is the customized WndProc
//           attached to the message-handling CVI panel.  Check a list of message/callback pairs
//           (maintained by the handler object for the window in question) for the desired message.
//           If found, we may either post a call to CVI's message queue, or call the user's
//           callback directly, according to the callback "mode" chosen by the user.  We will pass
//           the message on to the CVI WndProc if we are just posting a call, but if we are calling
//           the user callback directly then they may "swallow" it.
//
// Return:   The return from the default Win32 wndproc or the CVI default wndproc
//-------------------------------------------------------------------------------------------------
#if HAVE_CVI_RTE
static LRESULT CALLBACK MsgHandlerWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    int              handlerPosInList;
    int              linkPosInList;
    int              numLinks;
    int              threadID;
    MSG_CB_HANDLER*  phandler;
    MSG_FUNC_LINK*   plink;
    LRESULT  (CALLBACK*pdfltWndProc) (HWND, UINT, WPARAM, LPARAM);

    handlerPosInList = FindMsgCBHandlerFromHWND ((int)hwnd, &phandler);

    // CVI may occasionaly destroy and re-create our window...we need to watch for this and
    // be able to handle the new window when it gets created.  It will require us to replace
    // the CVI wndproc temporarily, then post a deferred call to reinstall this proc after
    // the window gets created.
    if (message == WM_DESTROY)
        {
        RemoveMsgHandlerWndProc (phandler);
        threadID = CurrThreadId ();
        PostDeferredCallToThread (DeferredReinstallHandler, phandler, threadID);
        }

    // Handle all other messages, looking for a handler link that is waiting for each
    else
        {
        if (phandler->winMsgEnabled)
            {
            numLinks = ListNumItems (phandler->links);
            for (linkPosInList = 1; linkPosInList <= numLinks; linkPosInList++)
                {
                plink = ListGetPtrToItem (phandler->links, linkPosInList);
                if (plink->enabled)
                    {
                    if (message == (UINT)(plink->msgNum))
                        {

                        // We've received the message -- do the callback the right way
                        if (plink->callWhen == VAL_MODE_INTERCEPT)
                            {
                            int userRetVal = 0;

                            // Process this event immediately...
                            userRetVal = (plink->UserCallback) (phandler->hpanel,
                                                                message, &wParam, (unsigned int*)&lParam,
                                                                plink->callbackData);

                            // If the user swallowed the message, skip the CVI WindProc
                            if (userRetVal)
                                goto PassOnToWindows;
                            }
                        if (plink->callWhen == VAL_MODE_IN_QUEUE)
                            {
                            MSG_DATA_REC* pmsgData;

                            // Allocate space for the user's message data and post a
                            // deferred call to our own internal function, which will call the
                            // user's
                            pmsgData = (MSG_DATA_REC*)malloc (sizeof(MSG_DATA_REC));
                            if (pmsgData)
                                {
                                pmsgData->hpanel = phandler->hpanel;
                                pmsgData->msg = message;
                                pmsgData->wParam = wParam;
                                pmsgData->lParam = lParam;
                                pmsgData->pcallbackFunc = plink->UserCallback;
                                pmsgData->pcallbackData = plink->callbackData;
                                threadID = CurrThreadId ();
                                PostDeferredCallToThread (DeferredHandleWinMsgCallback, pmsgData,
                                                          threadID);
                                }
                            }
                        }
                    }
                }
            }
        }

    // Call the CVI default WndProc
    pdfltWndProc = (void*) phandler->defaultCVIWndProc;
    return (*pdfltWndProc) (hwnd, message, wParam, lParam);

PassOnToWindows:

    // Call the SDK default WindProc
    return DefWindowProc (hwnd, message, wParam, lParam);
}
#endif  // HAVE_CVI_RTE

//-------------------------------------------------------------------------------------------------
// Function: ChainedMsgCBPanelCallback
//
// Purpose:  Respond to EVENT_DISCARDs to make sure and clean up if a user discards a panel without
//           first removing their message callbacks, as they should. This callback is chained to
//           any panels for which a user installs a message callback.
//
// Return:   0 to allow CVI to continue discarding the panel.
//-------------------------------------------------------------------------------------------------
#if HAVE_CVI_RTE
static int CVICALLBACK ChainedMsgCBPanelCallback (int hpanel, int event, void *callbackData,
                                                  int eventData1, int eventData2)
{
    int             handlerPosInList;
    MSG_CB_HANDLER* phandler;

    if (event == EVENT_DISCARD)
        {
        handlerPosInList = FindMsgCBHandlerFromHPanel (hpanel, &phandler);
        if (phandler->winMsgEnabled)
            ListDispose (phandler->links);
        DiscardMsgCBHandlerFromPanel (phandler, handlerPosInList);
        }
    return 0;
}
#endif  // HAVE_CVI_RTE

//-------------------------------------------------------------------------------------------------
// Function: DeferredHandleWinMsgCallback
//
// Purpose:  Call the user's callback function with the passed parameters.  This function is
//           called by ProcessSystemEvents () after a post-deferred call was made in the WT
//           WndProc.  We use this mechanism to call user callbacks after allowing CVI to process
//           windows messages first (if not intercepting).  The WT WndProc will have allocated
//           memory to pass us the data, and we'll need to free it here.
//
// Return:   void
//-------------------------------------------------------------------------------------------------
static void CVICALLBACK DeferredHandleWinMsgCallback (void *pvoid)
{
    MSG_DATA_REC* pmsgData;

    pmsgData = (MSG_DATA_REC*)pvoid;
    (pmsgData->pcallbackFunc) (pmsgData->hpanel, pmsgData->msg, &pmsgData->wParam,
                               &pmsgData->lParam, pmsgData->pcallbackData);
    free (pmsgData);
}

//-------------------------------------------------------------------------------------------------
// Function: DeferredReinstallHandler
//
// Purpose:  Re-install the WT WndProc for the panel associated with the passed handler object, and
//           notify all user callbacks of the new HWND window handle.  A post-deferred call to this
//           function is made in the WndProc when the window gets temporarily destroyed by CVI--
//           this is the restoration routine.
//
// Return:   void
//-------------------------------------------------------------------------------------------------
#if HAVE_CVI_RTE
static void CVICALLBACK DeferredReinstallHandler (void* phandler)
{
    int            numLinks;
    int            linkPosInList;
    MSG_FUNC_LINK* plink;

    GetPanelAttribute (((MSG_CB_HANDLER*)phandler)->hpanel, ATTR_SYSTEM_WINDOW_HANDLE,
                       &(((MSG_CB_HANDLER*)phandler)->hwindow));
    InsertMsgHandlerWndProc ((MSG_CB_HANDLER*)phandler);
    if (((MSG_CB_HANDLER*)phandler)->winMsgEnabled)
        {
        numLinks = ListNumItems (((MSG_CB_HANDLER*)phandler)->links);
        for (linkPosInList = 1; linkPosInList <= numLinks; linkPosInList++)
            {
            plink = ListGetPtrToItem (((MSG_CB_HANDLER*)phandler)->links,
                                            linkPosInList);
            (plink->UserCallback) (((MSG_CB_HANDLER*)phandler)->hpanel,
                                         EVENT_NEWHANDLE,
                                         &(((MSG_CB_HANDLER*)phandler)->hwindow), 0, NULL);
            }
        }
    return;
}
#endif  // HAVE_CVI_RTE

//-------------------------------------------------------------------------------------------------
// Function: CreateMsgCBHandlerForPanel
//
// Purpose:  Create a handler object for the specified panel, insert it into the global list (after
//           creating it if necessary), and install the WT WndProc.  Callers should assume that we
//           will manage creating all of the handler resources.  We will chain a callback to the
//           panel for which we install the new handler, and replace the default WndProc with our
//           own.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
#if HAVE_CVI_RTE
static int CVIFUNC CreateMsgCBHandlerForPanel (int hpanel, MSG_CB_HANDLER** ppnewHandler)
{
    int             retVal      = 0;
    int             hwnd;
    MSG_CB_HANDLER  newHandler;

    Assert(ppnewHandler);
    GetPanelAttribute (hpanel, ATTR_SYSTEM_WINDOW_HANDLE, &hwnd);
    newHandler.hpanel = hpanel;
    newHandler.hwindow = hwnd;
    newHandler.winMsgEnabled = FALSE;
    newHandler.processMsgs = TRUE;
    if (!g_msgCBHandlers && !(g_msgCBHandlers = ListCreate (sizeof(MSG_CB_HANDLER))))
        {
        retVal = UIEOutOfMemory;
        goto Error;
        }
    if (!ListInsertItem (g_msgCBHandlers, &newHandler, END_OF_LIST))
        {
        if (!ListNumItems (g_msgCBHandlers))
            {
            ListDispose (g_msgCBHandlers);
            g_msgCBHandlers = 0;
            }
        retVal = UIEOutOfMemory;
        goto Error;
        }

    // We need a pointer to the item as it exists in the List for the rest of the operations
    FindMsgCBHandlerFromHPanel (hpanel, ppnewHandler);
    Assert (*ppnewHandler);
    ChainPanelCallback ((*ppnewHandler)->hpanel, ChainedMsgCBPanelCallback, 0, "WT_TYPE");
    if (!InsertMsgHandlerWndProc (*ppnewHandler))
        {
        retVal = ToolErr_UnknownSystemError;
        ListRemoveItem (g_msgCBHandlers, 0, END_OF_LIST);
        if (!ListNumItems (g_msgCBHandlers))
            {
            ListDispose (g_msgCBHandlers);
            g_msgCBHandlers = 0;
            }
        }

Error:
    return retVal;
}
#endif  // HAVE_CVI_RTE

//-------------------------------------------------------------------------------------------------
// Function: DiscardMsgCBHandlerFromPanel
//
// Purpose:  Remove and discard the passed handler object from its panel.  We must reestablish the
//           CVI default WndProc, remove the handler from the global List, and discard the List if
//           there are no more handlers.
//
// Return:   void
//-------------------------------------------------------------------------------------------------
#if HAVE_CVI_RTE
static void CVIFUNC DiscardMsgCBHandlerFromPanel (MSG_CB_HANDLER* phandler, int handlerPosInList)
{
    RemoveMsgHandlerWndProc (phandler);
    UnchainPanelCallback (phandler->hpanel, "WT_TYPE");
    ListRemoveItem (g_msgCBHandlers, 0, handlerPosInList);
    if (!ListNumItems (g_msgCBHandlers))
        {
        ListDispose (g_msgCBHandlers);
        g_msgCBHandlers = 0;
        }
}
#endif  // HAVE_CVI_RTE

//-------------------------------------------------------------------------------------------------
// Function: AddLinkToMsgCBHandler
//
// Purpose:  Add a message-callback function link (MSG_FUNC_LINK) to the passed handler object.
//           The links are stored in a List member of the handler object, (a List which we will
//           create if necessary).  The winMsgEnabled member will be TRUE for the handler if it has
//           at least one MSG_FUNC_LINK (we use it as a flag to determine whether we need to create
//           a List).  Callers should assume that we will allocate the List and no concern is
//           needed for how many other links the passed handler might have.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
static int CVIFUNC AddLinkToMsgCBHandler (MSG_CB_HANDLER* phandler, int msgNum,
                                          WinMsgCallback pcallback, int callWhen,
                                          void* pcallbackData)
{
    short          i;
    int            returnVal  = 0;
    int            numLinks;
    MSG_FUNC_LINK  newLink;
    MSG_FUNC_LINK* ptempLink;

    Assert(phandler);
    Assert(pcallback);
    if (!(phandler->winMsgEnabled))
        {

        // We don't have a List for the links yet
        if (!(phandler->links = ListCreate (sizeof(MSG_FUNC_LINK))))
            {
            returnVal = UIEOutOfMemory;
            goto Error;
            }
        }

    // We only need to check for the prior existence of the link if the list is non-empty
    if (phandler->winMsgEnabled)
        {
        if ((numLinks = ListNumItems (phandler->links)) > 0)
            {
            for (i = 1; i <= numLinks; i++)
                {
                ptempLink = ListGetPtrToItem (phandler->links, i);
                if (msgNum == ptempLink->msgNum)
                    {
                    returnVal = ToolErr_MsgCallbackAlreadyInstalled;
                    goto Error;
                    }
                }
            }
        }
    newLink.msgNum = (UINT)msgNum;
    newLink.UserCallback = pcallback;
    newLink.callWhen = callWhen;
    newLink.enabled = TRUE;
    newLink.callbackData = pcallbackData;
    if (!ListInsertItem (phandler->links, &newLink, END_OF_LIST))
        {
        returnVal = UIEOutOfMemory;

        // Discard this list if this was to be the only item in it
        if (!ListNumItems (phandler->links))
            {
            ListDispose (phandler->links);
            phandler->links = 0;
            }
        goto Error;
        }
    phandler->winMsgEnabled = TRUE;

Error:
    return returnVal;
}

//-------------------------------------------------------------------------------------------------
// Function: InsertMsgHandlerWndProc
//
// Purpose:  Insert the WT WndProc for the panel having the passed handler object.  We'll store the
//           original WndProc in the handler so it can be reinstalled later.
//
// Return:   Boolean indicating success.
//-------------------------------------------------------------------------------------------------
#if HAVE_CVI_RTE
static BOOL CVIFUNC InsertMsgHandlerWndProc (MSG_CB_HANDLER* phandler)
{
    Assert(phandler);
    if (!(phandler->defaultCVIWndProc = GetWindowLong((HWND)phandler->hwindow, GWL_WNDPROC)))
        return FALSE;
    if (!SetWindowLong ((HWND)phandler->hwindow, GWL_WNDPROC, (LONG)MsgHandlerWndProc))
        {
        phandler->defaultCVIWndProc = 0;
        return FALSE;
        }
    return TRUE;
}
#endif  // HAVE_CVI_RTE

//-------------------------------------------------------------------------------------------------
// Function: RemoveMsgHandlerWndProc
//
// Purpose:  Remove the WT WndProc from the passed message handler's window and re-install the
//           origiinal.
//
// Return    void
//-------------------------------------------------------------------------------------------------
static void CVIFUNC RemoveMsgHandlerWndProc (MSG_CB_HANDLER* phandler)
{
    Assert(phandler);
    SetWindowLong ((HWND)phandler->hwindow, GWL_WNDPROC,
                   (LONG)phandler->defaultCVIWndProc);
}

//-------------------------------------------------------------------------------------------------
// Function: CompareMsgCBHandlerHWNDs
//
// Purpose:  Compare the window handle members of two MSG_CB_HANDLER structures to determine if
//           they are the same; FindMsgCBHandlerFromHWND uses this to determine the handler object
//           appropriate to a specific window.
//
// Return:   0 if the members match, else -1;
//-------------------------------------------------------------------------------------------------
static int CVICALLBACK CompareMsgCBHandlerHWNDs (void *pitem1, void *pitem2)
{
    Assert(pitem1);
    Assert(pitem2);
    if (((MSG_CB_HANDLER*)pitem1)->hwindow == ((MSG_CB_HANDLER*)pitem2)->hwindow)
        return 0;
    else
        return -1;
}

//-------------------------------------------------------------------------------------------------
// Function: FindMsgCBHandlerFromHWND
//
// Purpose:  Get a pointer to a handler object (if it exists) for the panel with the passed HWND.
//
// Return:   Handler's position in the global list (0 if not found).
//-------------------------------------------------------------------------------------------------
static int CVIFUNC FindMsgCBHandlerFromHWND (int hwindow, MSG_CB_HANDLER** pphandler)
{
    int            handlerPosInList;
    MSG_CB_HANDLER searchItem;

    Assert(pphandler);
    searchItem.hwindow = hwindow;
    handlerPosInList = ListFindItem (g_msgCBHandlers, &searchItem, FRONT_OF_LIST,
                                     CompareMsgCBHandlerHWNDs);
    if (handlerPosInList == 0)
        return 0;
    else
        {
        *pphandler = ListGetPtrToItem (g_msgCBHandlers, handlerPosInList);
        return handlerPosInList;
        }
}

//-------------------------------------------------------------------------------------------------
// Function: CompareMsgCBLinkMsgs
//
// Purpose:  Compare two MSG_FUNC_LINK items to see if they correspond to the same message number.
//
// Return:   0 if they do, else -1.
//-------------------------------------------------------------------------------------------------
static int CVICALLBACK CompareMsgCBLinkMsgs (void *pitem1, void *pitem2)
{
    Assert(pitem1);
    Assert(pitem2);
    if (((MSG_FUNC_LINK*)pitem1)->msgNum == ((MSG_FUNC_LINK*)pitem2)->msgNum)
        return 0;
    else
        return -1;
}

//-------------------------------------------------------------------------------------------------
// Function: CompareMsgCBHandlerHPanels
//
// Purpose:  Compare the hpanel members of two MSG_CB_HANDLER structures to determine if they are
//           the same.
//
// Return:   0 if they are the same, else -1
//-------------------------------------------------------------------------------------------------
static int CVICALLBACK CompareMsgCBHandlerHPanels (void *pitem1, void *pitem2)
{
    Assert(pitem1);
    Assert(pitem2);
    if (((MSG_CB_HANDLER*)pitem1)->hpanel == ((MSG_CB_HANDLER*)pitem2)->hpanel)
        return 0;
    else
        return -1;
}

//-------------------------------------------------------------------------------------------------
// Function: FindMsgCBHandlerFromHPanel
//
// Purpose:  Get the location and pointer to a MSG_CB_HANDLER object (if it exists) in the global
//           store for the panel with the passed handle.
//
// Return:   The 1-based position of the handler in the global list (or 0 if none exists).
//-------------------------------------------------------------------------------------------------
static int CVIFUNC FindMsgCBHandlerFromHPanel (int hpanel, MSG_CB_HANDLER** pphandler)
{
    int            handlerPosInList = 0;
    MSG_CB_HANDLER searchHandler;

    Assert(pphandler);
    if (g_msgCBHandlers)
        {
        searchHandler.hpanel = hpanel;
        handlerPosInList = ListFindItem (g_msgCBHandlers, &searchHandler, FRONT_OF_LIST,
                                         CompareMsgCBHandlerHPanels);
        if (handlerPosInList)
            *pphandler = ListGetPtrToItem (g_msgCBHandlers, handlerPosInList);
        }
    return handlerPosInList;
}

//-------------------------------------------------------------------------------------------------
// Function: DiscardAllMsgCBHandlers
//
// Purpose:  Free all memory for all message-callback handlers.  RSC callback calls us to clean up
//           resources if the user didn't.
//
// Return:   void
//-------------------------------------------------------------------------------------------------
#if HAVE_CVI_RTE && 0 /* Not used */
static void DiscardAllMsgCBHandlers (void)
{
    int             i, j;
    int             numHandlerItems;
    int             numLinkItems;
    MSG_CB_HANDLER* phandler;
    MSG_FUNC_LINK*  plink;

    if (g_msgCBHandlers)
        {
        numHandlerItems = ListNumItems (g_msgCBHandlers);
        for (i = 1; i <= numHandlerItems; i++)
            {
            phandler = ListGetPtrToItem (g_msgCBHandlers, 1);
            if (phandler->winMsgEnabled)
                {
                numLinkItems = ListNumItems (phandler->links);
                for (j = 1; j <= numLinkItems; j++)
                    {
                    plink = ListGetPtrToItem (phandler->links, j);
                    DiscardLinkFromHandler (phandler, plink->msgNum);
                    }
                }
            DiscardMsgCBHandlerFromPanel (phandler, i);
            }
        }
    return;
}
#endif  // HAVE_CVI_RTE

typedef struct
    {
    int         panel;
    HWND        hWnd;
    ListType    messageList;
    LONG        originalUSERDATA;
    WNDPROC     originalWindowProc;
    } SubClass;

#if HAVE_CVI_RTE
static LRESULT CALLBACK SubClass_InterceptProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int                     callbackReturnValue = 0;
    InterceptedWindowsMsg   msg = { 0 };
    SubClass                *subClass;
    LRESULT                 result;
    
    subClass = (SubClass *)GetWindowLong(hWnd, GWL_USERDATA);
    
    // if this is a intercepted message
    if (ListBinSearch(subClass->messageList, &uMsg, NULL) > 0)
        {
        msg.uMsg = uMsg;
        msg.wParam= wParam;
        msg.lParam = lParam;
        msg.result = 0;
        
        CallPanelCallback(subClass->panel, EVENT_WINDOWS_MSG, (int)&msg, 0, &callbackReturnValue);
        }
    
        
    if (callbackReturnValue == 0)   // if the panel callback did not "swallow the event"
        result = CallWindowProc(subClass->originalWindowProc, hWnd, uMsg, wParam, lParam);
    else
        result = msg.result;        // callback swallowed event and provided WinProc return value
        
    // window is going away, unhook and dispose subclass memory
    if (uMsg == WM_DESTROY)
        {
        // make sure we haven't been subclasses and the original window proc didn't unhook us
        if ((WNDPROC)GetWindowLong(hWnd, GWL_WNDPROC) == SubClass_InterceptProc)
            {
            SetWindowLong(hWnd, GWL_WNDPROC, (LONG)subClass->originalWindowProc);       
            SetWindowLong(hWnd, GWL_USERDATA, subClass->originalUSERDATA);
            ListDispose(subClass->messageList);
            free(subClass);
            }
        else
            ListClear(subClass->messageList); // we can't unhook so at least stop intercepting messages
        }
        
    return result;      
}

int CVIFUNC RouteWinMsgToPanelCallback(int panel, unsigned int uMsg)
{
    int         error = 0;
    HWND        hWnd;
    WNDPROC     winProc;
    LONG        result;
    SubClass    *subClass;
    SubClass    *newSubClass = NULL;
    
    errChk( GetPanelAttribute (panel, ATTR_SYSTEM_WINDOW_HANDLE, (int *)&hWnd));
    
    if (hWnd)
        {
        winProc = (WNDPROC)GetWindowLong(hWnd, GWL_WNDPROC);        
        
        // if we have not been hooked yet
        if (winProc != SubClass_InterceptProc)
            {
            nullChk( newSubClass = (SubClass *)calloc(1, sizeof(SubClass)));
            
            newSubClass->panel = panel;
            newSubClass->hWnd = hWnd;
            newSubClass->originalWindowProc = winProc;
            newSubClass->originalUSERDATA = GetWindowLong(hWnd, GWL_USERDATA);
            nullChk( newSubClass->messageList = ListCreate(sizeof(UINT)));

            SetLastError(0);
            result = SetWindowLong(hWnd, GWL_USERDATA, (LONG)newSubClass);      
            if (result == 0 && GetLastError())
                errChk(ToolErr_UnknownSystemError);
            newSubClass = NULL; // now owned by window, cleaned up by WM_DESTROY

            SetLastError(0);
            result = SetWindowLong(hWnd, GWL_WNDPROC, (LONG)SubClass_InterceptProc);        
            if (result == 0 && GetLastError())
                errChk(ToolErr_UnknownSystemError);
            }
            
        subClass = (SubClass *)GetWindowLong(hWnd, GWL_USERDATA);
        
        if (ListBinSearch(subClass->messageList, &uMsg, NULL) <= 0)
            nullChk( ListInsertInOrder(subClass->messageList, &uMsg, NULL));
        }

Error:
    if (newSubClass)
        {
        ListDispose(newSubClass->messageList);
        free(newSubClass);
        }
        
    return error;
}


// NOTE: Only works if no one else subclassed the same window after we did
int CVIFUNC UnrouteWinMsgToPanelCallback(int panel, unsigned int uMsg)
{
    int         error = 0;
    HWND        hWnd;
    SubClass    *subClass;
    int         foundIndex;
    
    errChk( GetPanelAttribute (panel, ATTR_SYSTEM_WINDOW_HANDLE, (int *)&hWnd));
    
    if (hWnd)
        {   
        // if we have been hooked
        if ((WNDPROC)GetWindowLong(hWnd, GWL_WNDPROC) == SubClass_InterceptProc)
            {
            subClass = (SubClass *)GetWindowLong(hWnd, GWL_USERDATA);
        
            if (uMsg == WM_NULL)
                ListClear(subClass->messageList);   // WM_NULL means unhook all messages 
            else
                {
                foundIndex = ListBinSearch(subClass->messageList, &uMsg, NULL);

                // remove the specified message, if found
                if (foundIndex > 0)
                    ListRemoveItem(subClass->messageList, NULL, foundIndex);
                }
                
            // if no more messages are hooked, unsubclass the window
            if (ListNumItems(subClass->messageList) == 0)
                {
                SetWindowLong(hWnd, GWL_WNDPROC, (LONG)subClass->originalWindowProc);       
                SetWindowLong(hWnd, GWL_USERDATA, subClass->originalUSERDATA);
                ListDispose(subClass->messageList);
                free(subClass);
                }
            }
        }

Error:
    return error;
}
#endif // HAVE_CVI_RTE

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Win32 shell FILE drag-and-drop
//
// We use the InstallWinMsgCallbackNoChecks function to install a callback for the DragAndDrop
// messages, which we enable.  Inside the callback, we call user code with the appropriate info.
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

static int CVICALLBACK DragAndDropCallback (int hpanel, int message, unsigned int* pwParam,
                                            unsigned int* plParam, void* pcallbackData);

//-------------------------------------------------------------------------------------------------
// Function: EnableDragAndDrop
//
// Purpose:  Enable drag-and-drop event notification for the passed CVI panel.  Enable
//           drag-and-drop messages and install a callback to respond to them.  From the callback,
//           notify the panel callback when files are dropped onto it.
//
// Return:   ToolErr error codes.
//-------------------------------------------------------------------------------------------------
#if HAVE_CVI_RTE
int CVIFUNC EnableDragAndDrop (int hpanel)
{
    int   BOLEState;
    int   hwnd;
    int   retVal     = 0;
    void* pvoid      = 0;

    BOLEState = GetBreakOnLibraryErrors ();
    SetBreakOnLibraryErrors (0);
    if (GetPanelAttribute (hpanel, ATTR_CALLBACK_FUNCTION_POINTER, &pvoid) < 0)
        {
            retVal = UIENotAPanelHandle;
            SetBreakOnLibraryErrors (BOLEState);
            goto Error;
        }
    SetBreakOnLibraryErrors (BOLEState);
    if (!pvoid)
        {

        // We can't call a panel's callback if it doesn't have one
        retVal = ToolErr_NoPanelCallback;
        goto Error;
        }

    // Callback data doesn't matter -- the user's panel callback will get the original callback
    // data regardless..
    if ((retVal = InstallWinMsgCallbackNoChecks (hpanel, WM_DROPFILES, DragAndDropCallback,
                                                 VAL_MODE_INTERCEPT, NULL, &hwnd)) != 0)
        goto Error;
    if ((retVal = S_DragAcceptFiles ((HWND)hwnd, TRUE)) != 0)
        RemoveWinMsgCallback (hpanel, WM_DROPFILES);

Error:
    return retVal;
}
#endif  // HAVE_CVI_RTE

//-------------------------------------------------------------------------------------------------
// Function: DisableDragAndDrop
//
// Purpose:  Disable drag-and-drop event notification for the passed CVI panel.
//
// Return:   ToolErr error code.
//-------------------------------------------------------------------------------------------------
#if HAVE_CVI_RTE
int CVIFUNC DisableDragAndDrop (int hpanel)
{
    int hwnd;
    int BOLEState;
    int retVal;

    BOLEState = GetBreakOnLibraryErrors ();
    SetBreakOnLibraryErrors (0);
    if (GetPanelAttribute (hpanel, ATTR_SYSTEM_WINDOW_HANDLE, &hwnd) < 0)
        {
        retVal = UIENotAPanelHandle;
        SetBreakOnLibraryErrors (BOLEState);
        goto Error;
        }
    if ((retVal = RemoveWinMsgCallback (hpanel, WM_DROPFILES)) == ToolErr_MsgCallbackNotInstalled)
        {
        retVal = ToolErr_DragDropNotEnabled;
        goto Error;
        }
    S_DragAcceptFiles ((HWND)hwnd, FALSE);

Error:
    return retVal;
}
#endif  // HAVE_CVI_RTE

//-------------------------------------------------------------------------------------------------
// Function: DragAndDropCallback
//
// Purpose:  Respond to Drag And Drop notification for a panel.  This function is installed as the
//           callback for WM_DROPFILES messages in EnableDragAndDrop...  When we get the message,
//           figure out the files and pass a meaningful buffer to the user's panel callback.  If
//           we get an EVENT_NEWHANDLE, then we need to re-enable Drag and Drop for the new
//           window.
//
// Return:   1 to prevent the CVI window from seeing the message.
//-------------------------------------------------------------------------------------------------
static int CVICALLBACK DragAndDropCallback (int hpanel, int message, unsigned int* pwParam,
                                            unsigned int* plParam, void* pcallbackData)
{
    unsigned int    i;
    unsigned int    numFiles;
    UINT   filenameSize = 0;
    char** pfilenames   = NULL;
    POINT  mousePoint;
    BOOL   sdkRetVal;
    UINT   retVal;

    if (message == EVENT_NEWHANDLE)

        // Re-register in the case of a window change
        S_DragAcceptFiles ((HWND)*pwParam, TRUE);
    if (message == WM_DROPFILES)
        {

        // Populate the string array
        S_DragQueryFile ((HANDLE)(*pwParam), 0xFFFFFFFF, (LPSTR)NULL, 0, &numFiles);
        pfilenames = malloc ((numFiles + 1) * sizeof(char*));
        if (pfilenames)
            {
            memset (pfilenames, 0, (numFiles + 1) * sizeof(char*));
            for (i = 0; i < numFiles; i++)
                {
                S_DragQueryFile ((HANDLE)(*pwParam), i, (LPSTR)NULL, 0, &filenameSize);
                if ((pfilenames[i] = malloc (filenameSize * sizeof(char*) + 1)) != NULL)
                    S_DragQueryFile ((HANDLE)(*pwParam), i, pfilenames[i],
                                     filenameSize + 1, &retVal);
                else
                    break;
                }

            // Call the user's panel callback and pass it the info...
            if (!S_DragQueryPoint ((HANDLE)(*pwParam), &mousePoint, &sdkRetVal) && sdkRetVal)
                CallPanelCallback (hpanel, EVENT_FILESDROPPED, (int)pfilenames, (int)(&mousePoint),
                                   0);
            }
        S_DragFinish ((HANDLE)(*pwParam));
        }
    return 1;
}


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Win32 System Tray icons
//
// Install an icon in the system tray.  If this is the first icon, then create our global handler.
// This handler data structure (of type SYS_TRAY_MGR) consists of a dummy CVI panel, with a
// message callback registered so it can respond to System Tray messages, and a List of
// SYS_TRAY_ICON objects.
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

// Initial message number for system tray icon notification
#define TRAY_ICON_MSG_BASE 56422

// Taskbar Tray icon object
typedef struct
{
    BOOL             menuVisible;
    int              huser;
    HMENU            hmenu;
    TrayIconCallback taskIconProc;
    NOTIFYICONDATA   nid;
} SYS_TRAY_ICON;

// Taskbar Tray icon handler object
typedef struct
{
    BOOL     initialized;
    HWND     hwnd;
    int      hpanel;
    ListType usedHandles;
    ListType trayIcons;
} SYS_TRAY_MGR;

// Static prototypes
static void           CVIFUNC     RemoveSysTrayIconNoChecks (SYS_TRAY_ICON* ptrayIcon);
static void           CVIFUNC     TrayMgrCloseIfEmpty       (void);
static void           CVIFUNC     TrayMgrFreeIcon           (SYS_TRAY_ICON* ptrayIcon);
static int            CVICALLBACK SysTrayIconCB             (int panelHandle, int message,
                                                             unsigned int* wParam,
                                                             unsigned int* lParam,
                                                             void* callbackData);
static int            CVIFUNC     TrayMgrGetNextFreeHandle  (void);
static int            CVIFUNC     TrayMgrStoreIcon          (SYS_TRAY_ICON* pnewIcon);
static int            CVICALLBACK CompareIconHandles        (void* ptrayIcon1, void* ptrayIcon2);
static int            CVIFUNC     TrayMgrInitIfNeeded       (void);
static HWND           CVIFUNC     TrayMgrGetHwnd            (void);
static SYS_TRAY_ICON* CVIFUNC     TrayMgrGetIconFromHandle  (int huser, int* pposInList);

// Global taskbar status icon manager
static SYS_TRAY_MGR g_trayMgr;


//-------------------------------------------------------------------------------------------------
// Function:  InstallSysTrayIcon
//
// Purpose:   Add an icon to the System Tray area of the Taskbar and let the user respond to
//            messages from it.  We'll give them a handle to the new icon via an output.
//
// Return:    0 for success or ToolErr/UIE error-code
//-------------------------------------------------------------------------------------------------
#if HAVE_CVI_RTE
int CVIFUNC InstallSysTrayIcon (const char* icoFile, const char* toolTipText, TrayIconCallback taskIconProc,
                                int* huser)
{
    int           retVal       = 0;
    int           newHandle    = 0;
    BOOL          sdkRetVal;
    SYS_TRAY_ICON newTrayIcon;

    Assert(icoFile);
    Assert(toolTipText);
    Assert(taskIconProc);
    Assert(huser);
    if ((retVal = TrayMgrInitIfNeeded ()) != 0)
        goto Error;

    newHandle = TrayMgrGetNextFreeHandle ();
    memset (&newTrayIcon, 0, sizeof(SYS_TRAY_ICON));
    newTrayIcon.taskIconProc = taskIconProc;
    newTrayIcon.nid.cbSize = sizeof(NOTIFYICONDATA);
    newTrayIcon.nid.uID = newHandle;
    newTrayIcon.nid.uCallbackMessage = TRAY_ICON_MSG_BASE; // TODO
    newTrayIcon.nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
    newTrayIcon.nid.hIcon = LoadImage (NULL, icoFile, IMAGE_ICON, 0, 0,
                                      LR_DEFAULTSIZE | LR_LOADFROMFILE );
    strncpy(newTrayIcon.nid.szTip, toolTipText, 64);
    newTrayIcon.nid.hWnd = TrayMgrGetHwnd ();
    if (TrayMgrStoreIcon (&newTrayIcon) < 1)
        {
        DestroyIcon (newTrayIcon.nid.hIcon);
        retVal = UIEOutOfMemory;
        goto Error;
        }
    if ((retVal = S_Shell_NotifyIcon (NIM_ADD, &(newTrayIcon.nid), &sdkRetVal)) != 0 || !sdkRetVal)
        {
        TrayMgrFreeIcon (&newTrayIcon);
        DestroyIcon (newTrayIcon.nid.hIcon);
        if (!sdkRetVal)
            retVal = ToolErr_UnknownSystemError;
        goto Error;
        }
    *huser = newHandle;

Error:
    return retVal;
}
#endif  // HAVE_CVI_RTE

//-------------------------------------------------------------------------------------------------
// Function: SetTrayIconAttr
//
// Purpose:  Set an attribute of a particular tray icon.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
int CVIFUNC SetTrayIconAttr (int huser, int attribute, const char* val)
{
    int            retVal          = 0;
    BOOL           sdkRetVal;
    HICON          holdIcon        = NULL;
    SYS_TRAY_ICON* ptrayIcon;
    char           oldToolText[64];

    VERIFY_PTR_ARG(val);
    if (!(ptrayIcon = TrayMgrGetIconFromHandle (huser, NULL)))
        {
        retVal = ToolErr_InvalidTrayIcon;
        goto Error;
        }
    switch (attribute)
        {
        case ATTR_TRAY_ICOFILE:
            holdIcon = ptrayIcon->nid.hIcon;
            ptrayIcon->nid.hIcon = LoadImage (NULL, val, IMAGE_ICON, 0, 0,
                                             LR_DEFAULTSIZE | LR_LOADFROMFILE );
            if (ptrayIcon->nid.hIcon)
                {
                S_Shell_NotifyIcon (NIM_MODIFY, &(ptrayIcon->nid), &sdkRetVal);
                if (sdkRetVal)
                    DestroyIcon (holdIcon);
                else
                    {
                    retVal = ToolErr_UnknownSystemError;
                    DestroyIcon (ptrayIcon->nid.hIcon);
                    ptrayIcon->nid.hIcon = holdIcon;
                    goto Error;
                    }
                }
            else
                {
                ptrayIcon->nid.hIcon = holdIcon;
                retVal = ToolErr_ErrorLoadingIconFileForTray;
                goto Error;
                }
            break;
        case ATTR_TRAY_TOOLTIPTEXT:
            strncpy (oldToolText, ptrayIcon->nid.szTip, 64);
            strncpy (ptrayIcon->nid.szTip, val, 64);
            S_Shell_NotifyIcon (NIM_MODIFY, &(ptrayIcon->nid), &sdkRetVal);
            if (!sdkRetVal)
                {
                retVal = ToolErr_UnknownSystemError;
                strncpy (ptrayIcon->nid.szTip, oldToolText, 64);
                goto Error;
                }
            break;
        default:
            retVal = UIEInvalidAttribute;
            break;
        }

Error:
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: GetTrayIconAttr
//
// Purpose:  Get an attribute of a particular tray icon.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
int CVIFUNC GetTrayIconAttr (int huser, int attribute, void* val)
{
    int            retVal     = 0;
    SYS_TRAY_ICON* ptrayIcon;

    VERIFY_PTR_ARG(val);
    if (!(ptrayIcon = TrayMgrGetIconFromHandle (huser, NULL)))
        {
        retVal = ToolErr_InvalidTrayIcon;
        goto Error;
        }
    switch (attribute)
        {
        case ATTR_TRAY_TOOLTIPTEXT_LENGTH:
            *(int*)val = strlen(ptrayIcon->nid.szTip);
            break;
        case ATTR_TRAY_TOOLTIPTEXT:
            strncpy ((char*)val, ptrayIcon->nid.szTip, 64);
            break;
        default:
            retVal = UIEInvalidAttribute;
            break;
        }

Error:
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: AttachTrayIconMenu
//
// Purpose:  Create a popup menu associated with a right-click on the status icon.  We don't pass
//           a handle back to the user, since there can only be one.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
int CVIFUNC AttachTrayIconMenu (int huser)
{
    int            retVal     = 0;
    SYS_TRAY_ICON* ptrayIcon;

    if (!(ptrayIcon = TrayMgrGetIconFromHandle (huser, NULL)))
        {
        retVal = ToolErr_InvalidTrayIcon;
        goto Error;
        }
    if (!(ptrayIcon->hmenu = CreatePopupMenu ()))
        {
        retVal = ToolErr_UnknownSystemError;
        ptrayIcon->menuVisible = FALSE;
        }
    else
        ptrayIcon->menuVisible = TRUE;

Error:
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: SetTrayIconMenuAttr
//
// Purpose:  Set an attribute of a particular tray icon's popup menu.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
int CVIFUNC SetTrayIconMenuAttr (int huser, int attribute, int val)
{
    int            retVal     = 0;
    SYS_TRAY_ICON* ptrayIcon;

    if (!(ptrayIcon = TrayMgrGetIconFromHandle (huser, NULL)))
        {
        retVal = ToolErr_InvalidTrayIcon;
        goto Error;
        }
    if (!ptrayIcon->hmenu)
        {
        retVal = ToolErr_NoMenuForTrayIcon;
        goto Error;
        }
    switch (attribute)
        {
        case ATTR_POPUP_DEFAULT_ITEM:
            if ((val < 0) || (val > GetMenuItemCount (ptrayIcon->hmenu)))
                {
                retVal = UIEBadAttributeValue;
                goto Error;
                }

            // -1 as an item index means no default
            val = (!val) ? -1 : val;
            if (!SetMenuDefaultItem (ptrayIcon->hmenu, val, FALSE))
                {
                retVal = ToolErr_UnknownSystemError;
                goto Error;
                }
            break;
        case ATTR_VISIBLE:
            ptrayIcon->menuVisible = (val) ? TRUE : FALSE;
            break;
        default:
            retVal = UIEInvalidAttribute;
            break;
        }

Error:
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: GetTrayIconMenuAttr
//
// Purpose:  Get an attribute of a particular tray icon's popup menu.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
int CVIFUNC GetTrayIconMenuAttr (int huser, int attribute, int* pval)
{
    int            retVal     = 0;
    int            item;
    SYS_TRAY_ICON* ptrayIcon;

    VERIFY_PTR_ARG(pval);
    if (!(ptrayIcon = TrayMgrGetIconFromHandle (huser, NULL)))
        {
        retVal = ToolErr_InvalidTrayIcon;
        goto Error;
        }
    if (!ptrayIcon->hmenu)
        {
        retVal = ToolErr_NoMenuForTrayIcon;
        goto Error;
        }
    switch (attribute)
        {
        case ATTR_POPUP_DEFAULT_ITEM:
            item = GetMenuDefaultItem (ptrayIcon->hmenu, FALSE, GMDI_USEDISABLED);
            if (item < 0) // no default item
                *pval = 0;
            else
                *pval = item;
            break;
        case ATTR_VISIBLE:
            *pval = ptrayIcon->menuVisible;
            break;
        default:
            retVal = UIEInvalidAttribute;
            break;
        }

Error:
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: InsertTrayIconMenuItem
//
// Purpose:  Insert an item into the menu of the specified icon.  Items are inserted sequentially
//           -- their 1-based index will grow up vertically.  Out the index of the newly inserted
//           item through an output.  We'll just keep track of items based on their insertion order
//           -- this way we can't alow them to remove items dynamically.  This is intended to keep
//           it simple and for the most probable case where the tray icon menu never changes.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
int CVIFUNC InsertTrayIconMenuItem (int huser, const char* itemName, int* pindex)
{
    int            retVal       = 0;
    int            numItems;
    MENUITEMINFO   newMenuItem;
    SYS_TRAY_ICON* ptrayIcon;

    VERIFY_PTR_ARG(pindex);
    if (!(ptrayIcon = TrayMgrGetIconFromHandle (huser, NULL)))
        {
        retVal = ToolErr_InvalidTrayIcon;
        goto Error;
        }
    if (!ptrayIcon->hmenu)
        {
        retVal = ToolErr_NoMenuForTrayIcon;
        goto Error;
        }
    numItems = GetMenuItemCount (ptrayIcon->hmenu);
    memset (&newMenuItem, 0, sizeof(MENUITEMINFO));
    newMenuItem.cbSize = sizeof(MENUITEMINFO);
    newMenuItem.fMask = MIIM_TYPE | MIIM_ID;
    newMenuItem.wID = numItems + 1;
    *pindex = newMenuItem.wID;
    if (!itemName)
        {

        // NULL means the user wants to insert a separator
        newMenuItem.fType = MFT_SEPARATOR;
        }
    else
        {
        newMenuItem.fType = MFT_STRING;
        newMenuItem.dwTypeData =  (char*)itemName;
        newMenuItem.cch = strlen (itemName);
        }
    if (!InsertMenuItem (ptrayIcon->hmenu, numItems, FALSE, &newMenuItem))
        retVal = ToolErr_UnknownSystemError;

Error:
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: SetTrayIconMenuItemAttr
//
// Purpose:  Set an attribute of a particular item on a tray icon's popup menu.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
int CVIFUNC SetTrayIconMenuItemAttr (int huser, int itemNum, int attribute, int val)
{
    int            retVal           = 0;
    MENUITEMINFO   newMenuItemInfo;
    SYS_TRAY_ICON* ptrayIcon;

    if (!(ptrayIcon = TrayMgrGetIconFromHandle (huser, NULL)))
        {
        retVal = ToolErr_InvalidTrayIcon;
        goto Error;
        }
    if (!ptrayIcon->hmenu)
        {
        retVal = ToolErr_NoMenuForTrayIcon;
        goto Error;
        }
    if ((itemNum < 1) || (itemNum > GetMenuItemCount (ptrayIcon->hmenu)))
        {
        retVal = ToolErr_InvalidTrayIconMenuItem;
        goto Error;
        }
    memset (&newMenuItemInfo, 0, sizeof(MENUITEMINFO));
    newMenuItemInfo.cbSize = sizeof(MENUITEMINFO);
    newMenuItemInfo.fMask = MIIM_STATE;
    if (!GetMenuItemInfo (ptrayIcon->hmenu, itemNum, FALSE, &newMenuItemInfo))
        retVal = ToolErr_UnknownSystemError;
    switch (attribute)
        {
        case ATTR_DIMMED:
            if (val)
                newMenuItemInfo.fState |= MFS_GRAYED;
            else
                newMenuItemInfo.fState &= ~MFS_GRAYED;
            break;
        case ATTR_CHECKED:
            if (val)
                newMenuItemInfo.fState |= MFS_CHECKED;
            else
                newMenuItemInfo.fState &= ~MFS_CHECKED;
            break;
        default:
            retVal = UIEInvalidAttribute;
            break;
        }
    if (!SetMenuItemInfo (ptrayIcon->hmenu, itemNum, FALSE, &newMenuItemInfo))
        retVal = ToolErr_UnknownSystemError;

Error:
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: GetTrayIconMenuItemAttr
//
// Purpose:  Get an attribute of a particular item on a tray icon's popup menu.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
int CVIFUNC GetTrayIconMenuItemAttr (int huser, int itemNum, int attribute, int* pval)
{
    int            retVal        = 0;
    MENUITEMINFO   menuItemInfo;
    SYS_TRAY_ICON* ptrayIcon;

    VERIFY_PTR_ARG(pval);
    if (!(ptrayIcon = TrayMgrGetIconFromHandle (huser, NULL)))
        {
        retVal = ToolErr_InvalidTrayIcon;
        goto Error;
        }
    if (!ptrayIcon->hmenu)
        {
        retVal = ToolErr_NoMenuForTrayIcon;
        goto Error;
        }
    if ((itemNum < 1) || (itemNum > GetMenuItemCount (ptrayIcon->hmenu)))
        {
        retVal = ToolErr_InvalidTrayIconMenuItem;
        goto Error;
        }
    memset (&menuItemInfo, 0, sizeof(MENUITEMINFO));
    menuItemInfo.cbSize = sizeof(MENUITEMINFO);
    menuItemInfo.fMask = MIIM_STATE;
    if (!GetMenuItemInfo (ptrayIcon->hmenu, itemNum, FALSE, &menuItemInfo))
        retVal = ToolErr_UnknownSystemError;
    switch (attribute)
        {
        case ATTR_DIMMED:
            *pval = (menuItemInfo.fState & MFS_GRAYED) ? TRUE : FALSE;
            break;
        case ATTR_CHECKED:
            *pval = (menuItemInfo.fState & MFS_CHECKED) ? TRUE : FALSE;
            break;
        default:
            retVal = UIEInvalidAttribute;
            break;
        }

Error:
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: DetachTrayIconMenu
//
// Purpose:  Destroy the menu associated with a tray icon and free all resources.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
int CVIFUNC DetachTrayIconMenu (int huser)
{
    int            retVal     = 0;
    SYS_TRAY_ICON* ptrayIcon;

    if (!(ptrayIcon = TrayMgrGetIconFromHandle (huser, NULL)))
        {
        retVal = ToolErr_InvalidTrayIcon;
        goto Error;
        }
    if (!ptrayIcon->hmenu)
        {
        retVal = ToolErr_NoMenuForTrayIcon;
        goto Error;
        }
    else
        {
        DestroyMenu (ptrayIcon->hmenu);
        ptrayIcon->hmenu = NULL;
        ptrayIcon->menuVisible = FALSE;
        }

Error:
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: RemoveSysTrayIcon
//
// Purpose:  Remove an icon from the System Tray and free all resources as far as the user is
//           concerned.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
#if HAVE_CVI_RTE
int CVIFUNC RemoveSysTrayIcon (int huser)
{
    int            retVal     = 0;
    SYS_TRAY_ICON* ptrayIcon;

    if (!(ptrayIcon = TrayMgrGetIconFromHandle (huser, NULL)))
        {
        retVal = ToolErr_InvalidTrayIcon;
        goto Error;
        }
    RemoveSysTrayIconNoChecks (ptrayIcon);
    TrayMgrCloseIfEmpty ();

Error:
    return retVal;
}
#endif  // HAVE_CVI_RTE

//-------------------------------------------------------------------------------------------------
// Function: SysTrayIconCB
//
// Purpose:  Respond to messages from all System Tray icons.  This is the callback function
//           registered to the global handler's dummy panel and should respond only to the
//           particular message we registered.  Upon receipt, we will call the user's callback.  If
//           we get a right-click and the icon has a popup menu, we'll call the user's callback and
//           then start tracking it.  We also want to make sure not to do anything else while
//           tracking such a menu.
//
// Return:   0
//-------------------------------------------------------------------------------------------------
static int CVICALLBACK SysTrayIconCB (int panelHandle, int message, unsigned int* wParam,
                                      unsigned int* lParam, void* callbackData)
{
    int            cviMsg       = 0;
    int            swallowMenu;
    BOOL           doMenu       = FALSE;
    static BOOL    trackingMenu = FALSE;
    POINT          cursorPos;
    SYS_TRAY_ICON* ptrayIcon    = NULL;

    if (!trackingMenu)
        {

        // Get the cursor first so if we need to do a menu it will be in the right location, even
        // if we call a user callback before we display it
        GetCursorPos (&cursorPos);
        ptrayIcon = TrayMgrGetIconFromHandle (*wParam, NULL);
        Assert(ptrayIcon);

        // Map Win messages to CVI messages
        Assert(lParam);
        switch (*lParam)
            {
            case WM_LBUTTONDOWN:
                cviMsg = EVENT_LEFT_CLICK;
                break;
            case WM_RBUTTONDOWN:
                cviMsg = EVENT_RIGHT_CLICK;
                if (ptrayIcon->hmenu)
                    doMenu = TRUE;
                break;
            case WM_LBUTTONUP:
                cviMsg = EVENT_LEFT_MOUSE_UP;
                break;
            case WM_RBUTTONUP:
                cviMsg = EVENT_RIGHT_MOUSE_UP;
                break;
            case WM_LBUTTONDBLCLK:
                cviMsg = EVENT_LEFT_DOUBLE_CLICK;
                break;
            case WM_RBUTTONDBLCLK:
                cviMsg = EVENT_RIGHT_DOUBLE_CLICK;
                break;
            default:
                return 1;
            }

        if (cviMsg)
            {
            Assert(ptrayIcon->taskIconProc);
            swallowMenu = ptrayIcon->taskIconProc (*wParam, cviMsg, 0);

            // Track on a popup menu if:
            //   - the user has a menu attached to this action
            //   - the menu is visible
            //   - the user did not swallow the menu by returning non-zero from their callback
            if (doMenu)
                {
                if (!swallowMenu && ptrayIcon->menuVisible)
                    {
                    int selectedItemID;

                    trackingMenu = TRUE;

                    SetForegroundWindow (g_trayMgr.hwnd);   // see http://support.microsoft.com/default.aspx?scid=KB;EN-US;Q135788&
                    selectedItemID = TrackPopupMenu (ptrayIcon->hmenu,
                        TPM_NONOTIFY | TPM_BOTTOMALIGN | TPM_RIGHTALIGN | TPM_RETURNCMD,
                        cursorPos.x, cursorPos.y, 0, TrayMgrGetHwnd (), 0);
                    PostMessage (g_trayMgr.hwnd, WM_NULL, 0, 0);
                    
                    ptrayIcon->taskIconProc (*wParam, EVENT_MENU_ITEM, selectedItemID);
                    trackingMenu = FALSE;
                    }
                }
            }
        }
    return 0;
}

//-------------------------------------------------------------------------------------------------
// Function: RemoveSysTrayIconNoChecks
//
// Purpose:  Given a pointer to a system tray icon object, remove the icon and free the associated
//           SYS_TRAY_ICON.  We assume a valid SYS_TRAY_ICON* input.
//
// Return:   void
//-------------------------------------------------------------------------------------------------
static void CVIFUNC RemoveSysTrayIconNoChecks (SYS_TRAY_ICON* ptrayIcon)
{
    BOOL sdkRetVal;

    if (ptrayIcon->hmenu)
        DestroyMenu (ptrayIcon->hmenu);
    S_Shell_NotifyIcon (NIM_DELETE, &(ptrayIcon->nid), &sdkRetVal);
    DestroyIcon (ptrayIcon->nid.hIcon);
    TrayMgrFreeIcon (ptrayIcon);
}

//-------------------------------------------------------------------------------------------------
// Function: TrayMgrInitIfNeeded
//
// Purpose:  Initialize the global data and message manager.  The manager will be responsible for
//           storing icon objects and managing their handles, and responding to messages.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
#if HAVE_CVI_RTE
static int CVIFUNC TrayMgrInitIfNeeded (void)
{
    int dummyHwnd;
    int retVal     = 0;

    if (g_trayMgr.initialized)
        return retVal;
    if ((g_trayMgr.trayIcons = ListCreate (sizeof(SYS_TRAY_ICON))) != NULL)
        {
        if ((g_trayMgr.usedHandles = ListCreate (sizeof(int))) != NULL)
            {
            if ((g_trayMgr.hpanel = NewPanel (0, "Tray Icon Message Handler", 5000, 5000, 10, 10)) < 0)
                {
                retVal = UIEOutOfMemory;
                goto Error;
                }
            GetPanelAttribute (g_trayMgr.hpanel, ATTR_SYSTEM_WINDOW_HANDLE, (int*)&g_trayMgr.hwnd);
            if (InstallWinMsgCallbackNoChecks (g_trayMgr.hpanel, TRAY_ICON_MSG_BASE,
                                               SysTrayIconCB, VAL_MODE_IN_QUEUE, NULL, &dummyHwnd)
                < 0)
                {
                retVal = UIEOutOfMemory;
                goto Error;
                }
            g_trayMgr.initialized = TRUE;
            }
        else
            {
            retVal = UIEOutOfMemory;
            goto Error;
            }
        }
    else
        {
        retVal = UIEOutOfMemory;
        goto Error;
        }
    return retVal;

Error:
    if (g_trayMgr.hpanel > 0)
        DiscardPanel (g_trayMgr.hpanel);
    g_trayMgr.hpanel = 0;
    if (g_trayMgr.trayIcons)
        {
        ListDispose (g_trayMgr.trayIcons);
        memset (g_trayMgr.trayIcons, 0, sizeof(ListType));
        }
    if (g_trayMgr.usedHandles)
        {
        ListDispose (g_trayMgr.usedHandles);
        memset (g_trayMgr.usedHandles, 0, sizeof(ListType));
        }
    return retVal;
}
#endif  // HAVE_CVI_RTE

//-------------------------------------------------------------------------------------------------
// Function: CompareIconHandles
//
// Purpose:  Compare the huser (user handle) members of two SYS_TRAY_ICON objects to determine if
//           they are equivalent.
//
// Return:   0 if they are equal, else -1
//-------------------------------------------------------------------------------------------------
static int CVICALLBACK CompareIconHandles (void* ptrayIcon1, void* ptrayIcon2)
{
    Assert(ptrayIcon1);
    Assert(ptrayIcon2);
    if (((SYS_TRAY_ICON*)ptrayIcon1)->huser == ((SYS_TRAY_ICON*)ptrayIcon2)->huser)
        return 0;
    else
        return -1;
}

//-------------------------------------------------------------------------------------------------
// Function: TrayMgrGetIconFromHandle
//
// Purpose:  Ask the global data manager to find an icon object given a handle.
//
// Return:   Pointer to the correct icon or NULL if not found, and the position of the icon in the
//           manager's list (if desired).
//-------------------------------------------------------------------------------------------------
static SYS_TRAY_ICON* CVIFUNC TrayMgrGetIconFromHandle (int huser, int* pposInList)
{
    int           posInList  = 0;
    SYS_TRAY_ICON searchIcon;

    searchIcon.huser = huser;
    if ((posInList = ListFindItem (g_trayMgr.trayIcons, &searchIcon, FRONT_OF_LIST,
                                  CompareIconHandles)) != 0)
        {
        if (pposInList)
            *pposInList = posInList;
        return ListGetPtrToItem (g_trayMgr.trayIcons, posInList);
        }
    else
        return NULL;
}

//-------------------------------------------------------------------------------------------------
// Function: TrayMgrFreeIcon
//
// Purpose:  Given a pointer to an icon object, remove it from the global store.  The assumption
//           is that this function will not free the store if this is the last item.  Use
//           MgrResetIfNoIcons to free the store.
//
// Return:   void
//-------------------------------------------------------------------------------------------------
static void CVIFUNC TrayMgrFreeIcon (SYS_TRAY_ICON* ptrayIcon)
{
    int itemPosInList;

    Assert(ptrayIcon != 0);
    itemPosInList = ListFindItem (g_trayMgr.usedHandles, &(ptrayIcon->huser), FRONT_OF_LIST, 0);
    ListRemoveItem (g_trayMgr.usedHandles, 0, itemPosInList);
    itemPosInList = ListFindItem (g_trayMgr.trayIcons, ptrayIcon, FRONT_OF_LIST,
                                  CompareIconHandles);
    ListRemoveItem (g_trayMgr.trayIcons, 0, itemPosInList);
}

//-------------------------------------------------------------------------------------------------
// Function: TrayMgrGetNumIcons
//
// Purpose:  Return the number of icons currently being managed buy the handler.
//
// Return:   Integer number of icon objects.
//-------------------------------------------------------------------------------------------------
#if 0 /* Not used */
static int CVIFUNC TrayMgrGetNumIcons (void)
{
    return ListNumItems (g_trayMgr.usedHandles);
}
#endif

//-------------------------------------------------------------------------------------------------
// Function: TrayMgrStoreIcon
//
// Purpose:  Given a pointer to a newly created icon, insert it into the global store.  The
//           assumption is that MgrInitIfNeeded must have already been called.  We will assign
//           the huser user handle member to the corect value when we insert the item.
//
// Return:   Integer 1-based handle to new icon or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
static int CVIFUNC TrayMgrStoreIcon (SYS_TRAY_ICON* pnewIcon)
{
    int retVal        = 0;
    int freeHandleVal = 1;

    Assert(pnewIcon);
    while (ListFindItem (g_trayMgr.usedHandles, &freeHandleVal, FRONT_OF_LIST, 0))
        freeHandleVal++;
    pnewIcon->huser = freeHandleVal;
    if (ListInsertItem (g_trayMgr.trayIcons, pnewIcon, END_OF_LIST))
        {
        if (ListInsertItem (g_trayMgr.usedHandles, &freeHandleVal, END_OF_LIST))
            {
            retVal = freeHandleVal;
            pnewIcon->huser = freeHandleVal;
            }
        else
            {
            int newItemPos;

            newItemPos = ListNumItems (g_trayMgr.trayIcons);
            ListRemoveItem (g_trayMgr.trayIcons, 0, newItemPos);
            retVal = UIEOutOfMemory;
            }
        }
    else
        retVal = UIEOutOfMemory;
    return retVal;
}

//-------------------------------------------------------------------------------------------------
// Function: TrayMgrGetPNthIcon
//
// Purpose:  Get a pointer to the Nth icon object.  We need this for callers who need to enumerate
//           through all icons.
//
// Return:   Pointer to the correct SYS_TRAY_ICON object in the manager's list.
//-------------------------------------------------------------------------------------------------
#if 0 /* Not used */
static SYS_TRAY_ICON* CVIFUNC TrayMgrGetPNthIcon (int n)
{
    Assert(n > 0);
    return ListGetPtrToItem (g_trayMgr.trayIcons, n);
}
#endif

//-------------------------------------------------------------------------------------------------
// Function: TrayMgrCloseIfEmpty
//
// Purpose:  Free all resources associated with the global data manager if there are no more icon
//           objects to keep track of.
//
// Return:   void
//-------------------------------------------------------------------------------------------------
#if HAVE_CVI_RTE
static void CVIFUNC TrayMgrCloseIfEmpty (void)
{
    if (!ListNumItems (g_trayMgr.trayIcons))
        {
        ListDispose (g_trayMgr.trayIcons);
        memset (&g_trayMgr.trayIcons, 0, sizeof(ListType));
        ListDispose (g_trayMgr.usedHandles);
        memset (&g_trayMgr.usedHandles, 0, sizeof(ListType));
        RemoveWinMsgCallback (g_trayMgr.hpanel, TRAY_ICON_MSG_BASE);
        DiscardPanel (g_trayMgr.hpanel);
        memset (&g_trayMgr, 0, sizeof(SYS_TRAY_MGR));
        }
}
#endif  // HAVE_CVI_RTE

//-------------------------------------------------------------------------------------------------
// Function: TrayMgrGetNextFreeHandle
//
// Purpose:  Determine the next free handle value -- what will this icon's handle be when we
//           eventually insert it.  The handle and the icon ID (as far as Win is concerned) need to
//           be the same for us to figure out stuff.
//
// Return:   Integer indicating the next available handle.
//-------------------------------------------------------------------------------------------------
static int CVIFUNC TrayMgrGetNextFreeHandle (void)
{
    int nextFreeHandle = 1;

    while (ListFindItem (g_trayMgr.usedHandles, &nextFreeHandle, FRONT_OF_LIST, 0))
        nextFreeHandle++;
    return nextFreeHandle;
}

//-------------------------------------------------------------------------------------------------
// Function: TrayMgrGetHwnd
//
// Purpose:  Determine the HWND of the manager's window.
//
// Return:   HWND.
//-------------------------------------------------------------------------------------------------
static HWND CVIFUNC TrayMgrGetHwnd (void)
{
    return g_trayMgr.hwnd;
}

//-------------------------------------------------------------------------------------------------
// Function: DiscardAllTrayIcons
//
// Purpose:  Summarily destroy all tray icon resources in the case of a user-termination -- called
//           by the RSC callback.
//
// Return:   void
//-------------------------------------------------------------------------------------------------
#if HAVE_CVI_RTE && 0
static void CVIFUNC DiscardAllTrayIcons (void)
{
    int            i;
    int            numIcons;
    SYS_TRAY_ICON* ptrayIcon;

    numIcons = TrayMgrGetNumIcons ();
    for (i = 1; i <= numIcons; i++)
        {
        ptrayIcon = TrayMgrGetPNthIcon (1);
        RemoveSysTrayIconNoChecks (ptrayIcon);
        }
    TrayMgrCloseIfEmpty ();
}
#endif  // HAVE_CVI_RTE

static int GetShellExecuteWindowStateFromUserintWindowState(int userintWindowState)
{
    switch (userintWindowState) {
        case VAL_NO_ZOOM:
            return SW_SHOWNORMAL;
        case VAL_MINIMIZE:
            return SW_SHOWMINIMIZED;
        case VAL_MAXIMIZE:
            return SW_SHOWMAXIMIZED;
        default:
            return SW_SHOWNORMAL;
    }
}

static int ConvertShellExecuteErrorsToToolErrors(int shellExecuteError)
{
    switch(shellExecuteError) {
        case SE_ERR_FNF: return UIEFileWasNotFound;
        case SE_ERR_PNF: return UIEFileWasNotFound;
        case SE_ERR_ACCESSDENIED: return UIEAccessDenied;
        case SE_ERR_OOM: return UIEOutOfMemory ;
        case SE_ERR_DLLNOTFOUND: return ToolErr_ErrorLoadingDLL;
        case SE_ERR_SHARE: return UIEAccessDenied;
        case SE_ERR_ASSOCINCOMPLETE: return ToolErr_FileAssociationNotAvailable;
        case SE_ERR_DDETIMEOUT: return ToolErr_UnknownSystemError;
        case SE_ERR_DDEFAIL:    return ToolErr_UnknownSystemError;
        case SE_ERR_DDEBUSY:    return ToolErr_UnknownSystemError;
        case SE_ERR_NOASSOC: return ToolErr_FileAssociationNotAvailable;
        default: return ToolErr_UnknownSystemError;
    }
}

int CVIFUNC OpenDocumentInDefaultViewer(const char *documentPathName, int windowState)
{
    SHELLEXECUTEINFO seInfo;
    int error = UIENoError;
    BOOL shellExecuteSucceeded = FALSE;

    memset(&seInfo, 0, sizeof(seInfo));

    seInfo.cbSize = sizeof(seInfo);
    seInfo.fMask = SEE_MASK_FLAG_NO_UI;
    seInfo.lpVerb = "open";
    seInfo.lpFile = documentPathName;
    seInfo.nShow = GetShellExecuteWindowStateFromUserintWindowState(windowState);

    if (!(error = S_ShellExecuteEx(&seInfo, &shellExecuteSucceeded))) {
        if (!shellExecuteSucceeded)
            error = ConvertShellExecuteErrorsToToolErrors((int)seInfo.hInstApp);
    }

    return error;


}

//-------------------------------------------------------------------------------------------------
// Function: PostDeferredCallToThreadAndWait
//
// Purpose:  Post a deferred callback to a particular thread and sleep until the call completes.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
#if HAVE_CVI_RTE
int CVIFUNC PostDeferredCallToThreadAndWait (DeferredCallbackPtr funcPtr, void *callbackData,
                                             unsigned long targetThreadId, int timeout)
{
    int error = UIENoError;
    double startTime = 0.;
    double timeoutMS = 0.;
    int callWaitAbandoned = 0;
    WaitDeferredCallbackData *waitDeferredCallbackData = NULL;

    if (funcPtr == NULL) {
        error = UIENullPointerPassed;
        goto Error;
    }

    nullChk (waitDeferredCallbackData = malloc (sizeof (WaitDeferredCallbackData)));
    waitDeferredCallbackData->callState = kCallNotPosted;
    waitDeferredCallbackData->userCallbackData = callbackData;
    waitDeferredCallbackData->userCallbackFuncPtr = funcPtr;
    waitDeferredCallbackData->callWaitAbandoned = 0;

    if (targetThreadId == CmtGetCurrentThreadID ())
        {
        // Call the function directly in this thread
        PostDeferredCallToThreadAndWaitCallback (waitDeferredCallbackData);
        }

    else
        {
        if (timeout != POST_CALL_WAIT_TIMEOUT_INFINITE)
            {
            timeoutMS = (double)timeout / 1000.0;
            startTime = Timer ();
            }

        // Post a deferred call to the thread and wait for it to finish execution.
        errChk (PostDeferredCallToThread (PostDeferredCallToThreadAndWaitCallback,
                                          waitDeferredCallbackData,
                                          targetThreadId));
        waitDeferredCallbackData->callState = kCallPosted;

        while (waitDeferredCallbackData->callState != kCallExecutionComplete)
            {
            if (timeout != POST_CALL_WAIT_TIMEOUT_INFINITE)
                {
                if ((Timer() - startTime) > timeoutMS)
                    {
                    /* If we timeout, the callback frees the structure. */
                    waitDeferredCallbackData->callWaitAbandoned = 1;
                    callWaitAbandoned = 1;
                    break;
                    }
                }
            Sleep (0);
            }
        }

Error:
    if ((waitDeferredCallbackData != NULL)
        && !callWaitAbandoned)
            {
            free (waitDeferredCallbackData);
            }
    return error;
}
#endif  // HAVE_CVI_RTE

//-------------------------------------------------------------------------------------------------
// Function: PostDeferredCallToThreadAndWaitCallback
//
// Purpose:  Calls the user's callback and updates the call state flags.
//
// Return:   0 for success or ToolErr/UIE error code.
//-------------------------------------------------------------------------------------------------
static void CVICALLBACK PostDeferredCallToThreadAndWaitCallback (void *callbackData)
{
    WaitDeferredCallbackData *waitDeferredCallbackData = (WaitDeferredCallbackData *)callbackData;
    int waitAbandoned = 0;

    waitDeferredCallbackData->callState = kCallExecuting;
    waitDeferredCallbackData->userCallbackFuncPtr (waitDeferredCallbackData->userCallbackData);
    waitAbandoned = waitDeferredCallbackData->callWaitAbandoned;
    waitDeferredCallbackData->callState = kCallExecutionComplete;

    /* We are responsible for freeing the structure here if a timout occurred. */
    if (waitAbandoned)
        free (waitDeferredCallbackData);
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// RSC handler to clean up system resources properly
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

#ifdef _CVI_

//-------------------------------------------------------------------------------------------------
// Function: __RunStateChangeCallback
//
// Purpose:  Handle debugging and execution state changes to clean up and to prevent
//           message-processing while in user breakpoints (it will mess up CVI if we call funcs
//           from inside the WndProc while debugging).
//
// Return:   void
//-------------------------------------------------------------------------------------------------
#if 0
void CVICDECL __RunStateChangeCallback (int runState)
{
    if (runState == kRunState_Stop)
        {
        DiscardAllTrayIcons ();
        DiscardAllMsgCBHandlers ();
        KERNEL32CleanupIfNeeded ();
        SHELL32CleanupIfNeeded ();
        }
}
#endif
#endif // _CVI_


#endif /* defined(_NI_mswin32_) && _NI_mswin32_ */

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// End of Windows-only
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
