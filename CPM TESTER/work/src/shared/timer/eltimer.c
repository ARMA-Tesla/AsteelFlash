/****************************************************************************
 *                   E X A C T   T I M E R
 *---------------------------------------------------------------------------
 *    Copyright  © 2005 ELCOM, a.s.  All Rights Reserved.        
 *---------------------------------------------------------------------------
 *                                                                          
 * Title:       eltimer.h                                                    
 * Purpose:     High-Resolution Timer interface 
 ****************************************************************************/
#include <timer/eltimer.h>
#include <elerror.h>
#include <stdlib.h>

/*****************************************************************************
 * Static no-exported functions declaration **********************************
 *****************************************************************************/
static int32_t	timerStart(struct _STimer* me);
static double	timerGetTimeDiff(struct _STimer* me);


/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "timer_new"
/*!
 * \brief
 * This function creates a new instance of timer.
 * 
 * \param pError 
 * Returns a value indicating whether an error occurred.
 * A negative error code indicates function failure.	
 * 
 * \return 
 * Pointer to new instance of the Timer object. If the function fails, NULL
 * pointer is returned
 *
 * This function creates a new instance of the Timer object.
 */
SElExceptionPtr timer_new(
	STimerPtr*	pTimer 
)
{
	SElExceptionPtr     pexception = NULL;   
	int32_t		error = 0;
	STimerPtr	ptimer = NULL;
	DWORD		win_error;

	ptimer = calloc( 1, sizeof(STimer));
	EXCCHECKALLOC( ptimer);
	
	*pTimer = ptimer;

	ptimer->Start       = timerStart;
	ptimer->GetTimeDiff = timerGetTimeDiff;

	/* COM Initialization */
    CHECKERR( ptimer->Start(ptimer));
	CHECKWIN( QueryPerformanceFrequency(&(ptimer->_qwFreq)));

Error:
	EXCRETHROW( pexception);	 
}/* vline_new */

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "timer_delete"
/*!
 * \brief
 * This function destroys specified instance.
 * 
 * \param pTimer 
 * Pointer to instance of the Timer object cretaed by timer_new() function.
 * 
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  None\n\n
 *  ERRORS:\n
 *  None\n 
 * 
 * This function destroys specified instance and release all resources.
 */
SElExceptionPtr	timer_delete(
	STimerPtr*	pTimer
)
{

	if ( pTimer && *pTimer )
	{
		free( *pTimer);
		*pTimer = NULL;
	}

/* Error: */
	return NULL;
} /* vline_delete */

/*---------------------------------------------------------------------------*/
#undef __FUNC__
#define __FUNC__ "timerStart"
/*!
 * \brief
 * This function marks current time as start position of timer.
 * 
 * \param pTimer
 * Pointer to instance of the Timer object cretaed by timer_new() function.
 * 
 * \patam pResourceName   Resource descriptor.
 *                        Valid resources are:
 *                        "COMxx" where xx means por number for example:"COM3"   
 * 
 * \return 
 *  Returns the status code of this operation.  The status code either
 *  indicates success or describes an error or warning condition.  You examine
 *  the status code from each call to an log function to determine
 *  if an error occurred.\n
 *  The general meaning of the status code is as follows:\n
 *  Value                  Meaning\n
 *  -------------------------------\n
 *  0                      Success\n
 *  Positive Values        Warnings\n
 *  Negative Values        Errors\n\n
 *  This driver defines the following status codes:\n\n
 *  Status    Description\n
 *  -------------------------------------------------\n
 *  WARNINGS: \n
 *  None\n\n
 *  ERRORS:\n
 *  None\n\n
 *  
 * This function marks current time as start position of timer. When you will 
 * call function \p GetTimeDiff() you will obtain time difference between
 * actual time and Start time in miliseconds     
 *
 */
static int32_t	timerStart(
	struct _STimer*		pTimer
)
{
	int32_t		error = 0;
	DWORD		win_error;

	CHECKWIN( QueryPerformanceCounter(&(pTimer->_qwStart)));

Error:
	return error;
} /* timerStart */

/*---------------------------------------------------------------------------*/
#undef  __FUNC__
#define __FUNC__ "timerGetTimeDiff"
/*!
 * \brief
 * This function calculates the time difference between time when instance
 * has been created or \p Start() function nas been called. 
 * 
 * \param pVLine
 * Pointer to instance of the VLine object cretaed by vline_new() function.
 * 
 * \return 
 *  Returns the time difference between time when instance has been created
 *  by \p timer_new() function or when \p Start() function nas been called.
 *  The time is miliseconds. 
 */
static double timerGetTimeDiff(
    struct _STimer* pTimer
)
{
	int32_t    error = 0;
    double	   value = 0.0;
    DWORD	win_error;

	CHECKWIN( QueryPerformanceCounter(&(pTimer->_qwEnd)));

	value = ((double)(pTimer->_qwEnd.QuadPart - pTimer->_qwStart.QuadPart)) / pTimer->_qwFreq.QuadPart;
Error:
    if ( error < 0 ) value = -1.0;
	return value;
} /* timerGetTimeDiff */
