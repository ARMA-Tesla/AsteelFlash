/*
+------------------------------------------------------------------------------+
| applicom ========                                                            |
| ====international                                                            |
|                                                                              |
|  Copyright (c) 2000 Applicom International. All rights reserved.             |
+------------------------------------------------------------------------------+
  Description .......:  Include Declarations file for CVI
                             
+------------------------------------------------------------------------------+
  File Name..........: APPLICOMIO.H
  Version............: 2.1
  Date...............: 12/10/2000
+-- [WARNING DO NOT MODIFY THIS FILE]------------------------------------------+
*/
#ifndef __APPLICOMIO_HEADER_FILE__
  #define __APPLICOMIO_HEADER_FILE__

#include <cvidef.h>
typedef unsigned short WORD;
typedef unsigned char  BYTE;
typedef unsigned long  DWORD;



/* Translate a card number to a chan number */

#define CARD2CHAN(x) ((WORD)((x-1)*4))

/* Function prototype */ 

BOOL _stdcall AuInitBus_io   (DWORD* pdwStatus);
BOOL _stdcall AuExitBus_io   (DWORD* pdwStatus);


BOOL _stdcall AuWriteReadMsg_io (WORD    wChan,
             	                 WORD    wNes,
                               	 DWORD   dwMsgParam,
                                 WORD    wNbTx,
                                 BYTE*   lpbyBufTx,
                                 WORD*   pwNbRx,
                                 BYTE*   lpbyBufRx,
                                 DWORD*  pdwStatus);
 
#endif