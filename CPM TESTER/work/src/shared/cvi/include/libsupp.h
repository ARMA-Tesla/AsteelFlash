/*============================================================================*/
/*                        L a b W i n d o w s / C V I                         */
/*----------------------------------------------------------------------------*/
/*    Copyright (c) National Instruments 1987-1999.  All Rights Reserved.     */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Title:       libsupp.h                                                     */
/* Purpose:     Include file for LabWindows/CVI callback and execution state  */
/*              determination support.                                        */
/*                                                                            */
/*============================================================================*/

#include "cvidef.h"
#include "cvirte.h"

#ifdef __cplusplus
    extern "C" {
#endif

/******************************************
Run-State-Change Callbacks:
   Certain libraries need to be notified when the user program starts,
suspends, continues, or stops.  Examples of such libraries are those that
have asynchronous callbacks; they must not call the user's callbacks when 
the user's program in not executing.  The library can be informed of 
changes in the user program's status through a callback.  

   If a library wishes to be notified of changes in the run state, it can
have the callback automatically installed by naming it
__RunStateChangeCallback, and including it in the library.  This callback
must be in an object file (or library), rather than in a source file.
Multiple libraries may contain functions with this name, since it is not
entered into the global name space. The prototype for the callback is

   void CVICALLBACK __RunStateChangeCallback(int action).

Libraries using asynchronous callbacks:
   If a library calls a user's function asynchronously (i.e through 
interrupts or signals), it must announce the callback by calling
EnterAsyncCallback() before calling the callback, and ExitAsyncCallback()
after calling the callback.  This enables user protection to be turned off
for an asynchronous callback.   EnterAsyncCallback() and ExitAsyncCallback()
take one parameter which is a pointer to a buffer of size
ASYNC_CALLBACK_ENV_SIZE; the same buffer must be passed into
ExitAsyncCallback() as was passed into EnterAsyncCallback() because the
buffer is used to store state information.
*******************************************/

#define ASYNC_CALLBACK_ENV_SIZE 2048

void CVIFUNC EnterAsyncCallback(void *storageSpace);
void CVIFUNC ExitAsyncCallback(void *storageSpace);

enum {
    kRunState_Start,
    kRunState_Suspend,
    kRunState_Resume,
    kRunState_AbortingExecution,
    kRunState_Stop,
    kRunState_EnableCallbacks,
    kRunState_DisableCallbacks
};

/**********************************************************************
EXAMPLE 1
---------
    kRunState_Start
    kRunState_EnableCallbacks
        // user program execution begins //
            .
            .
        // a breakpoint or runtime error occurs, or user presses the TerminateExecution key combination //
    kRunState_DisableCallbacks
    kRunState_Suspend
        // program execution is suspended;  CVI envirnonment resumes //
            .
            .
        // user requests the execution be resumed, via the "Continue", "Step Over", etc., commands //
    kRunState_Resume
    kRunState_EnableCallbacks
        // user program execution resumes //
            .
            .
        // user program execution completes normally //
    kRunState_DisableCallbacks
    kRunState_Stop


EXAMPLE 2
---------
    kRunState_Start
    kRunState_EnableCallbacks
        // user program execution begins //
            .
            .
        // a breakpoint or runtime error occurs, or user presses the TerminateExecution key combination //
    kRunState_DisableCallbacks
    kRunState_Suspend
        // program execution is suspended;  CVI envirnonment resumes //
            .
            .
        // user selects the Terminate Execution command //
    kRunState_DisableCallbacks   // even though callbacks already disabled //
    kRunState_AbortingExecution
        // long jump out of user program    //
    kRunState_DisableCallbacks   // even though callbacks already disabled //
    kRunState_Stop


*******************************************/

#ifdef __cplusplus
    }
#endif
