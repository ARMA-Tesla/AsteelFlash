/****************************************************************************
 *                             V L I N E
 *---------------------------------------------------------------------------
 *    Copyright   2005-2006 ELCOM, a.s.  All Rights Reserved.        
 *---------------------------------------------------------------------------
 *                                                                          
 * Title:       vline.h                                                    
 * Purpose:     Vline interface (Valeo debug line interface)
 * Version:     2.0 
 ****************************************************************************/

#if !defined(__ELZIP_H__)
#define __ELZIP_H__

#include <eltypes.h>
#include <exception/Exception.h>

#if defined(__cplusplus) || defined(__cplusplus__)
	extern "C" {
#endif

typedef struct _SZip
{
	SElExceptionPtr (*DecompressFile)(struct _SZip* me, const char* archive, const char* file, const char* output);

/* privat */
	void*	zzip;
	
} SZip, *SZipPtr;

SElExceptionPtr	elzip_new(SZipPtr* pZip);
SElExceptionPtr	elzip_delete(SZipPtr* pZip);

#if defined(__cplusplus) || defined(__cplusplus__)
	}
#endif

#endif /* __ELZIP_H__*/ 
