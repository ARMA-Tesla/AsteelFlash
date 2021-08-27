/*
 *  <elSocket.h> - Definitions for module "elSocket.c".
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
 *	2007-03-05	Radomir Tomis (Radomir.Tomis@dvi.elcom.cz)
 *		Created.
 */

#if _MSC_VER >= 1000
#pragma once
#endif 

#if !defined(_EL_SOCKET_H)
#define _EL_SOCKET_H

#if _MSC_VER < 1400 && !__BCOPT__ && !_CVI_  /* Borland C/C++: Unknown preprocessor directive: 'ident' */
#if 1
#ident "@(#) elSocket.h 0.1 2007-03-05"
#else
#endif
#endif

#include <elBase.h>

#if WIN
#if !defined(_MFC_VER)		/* !MFC (!<afx.h>) */
#if _MSC_VER < 1400
#include <winsock2.h>
#endif
#include <windows.h>
#endif	/* !_MFC_VER */
#else
#include <sys/socket.h>
#include <netdb.h>
extern int h_errno;
#if DJGPP
#define WATTCP	1
#endif
#if WATTCP
#if 0
#include "wattcp.h"
#endif
#if 1
#include "tcp.h"			/* sin_mask, ... */
#endif
#endif
#endif

#if WATTCP
#if 1
#define EL_SOCKET_TICK		1
#else
#define EL_SOCKET_TICK		0
#endif
#else
#define EL_SOCKET_TICK		0
#endif

#if 0
#undef EL_SOCKET_TICK
#define EL_SOCKET_TICK		1
#endif

#if WIN
#define SOCKET_HANDLE_FMT_PFX	""
#define SOCKET_HANDLE_FMT		"u"		/* "%u" - 'typedef u_int SOCKET' */
typedef SOCKET elSocketHandle_t;
#if 1
typedef struct sockaddr_in sockaddr_in_t;
#else
typedef struct SOCKADDR sockaddr_in_t;
#endif
#else
#if EL_SOCKET_TICK
#define SOCKET_HANDLE_FMT_PFX	"0x"
#define SOCKET_HANDLE_FMT		"lx"
typedef tcp_Socket* elSocketHandle_t;
#else
#define SOCKET_HANDLE_FMT_PFX	""
#define SOCKET_HANDLE_FMT		"d"
typedef int elSocketHandle_t;
#endif
typedef struct sockaddr_in sockaddr_in_t;
#endif

typedef struct {
#if EL_SOCKET_TICK
	tcp_Socket ts;		/* TCP socket */
#endif
	elSocketHandle_t h;	/* EL_SOCKET_TICK: points to 'ts' */
	bool nonblock;
	struct {
		long recv, send;
	} timeout;
} elSocket_t;

enum { EL_SOCKET_CONNECT, EL_SOCKET_LISTEN };
enum { EL_SOCKET_INPUT, EL_SOCKET_OUTPUT, EL_SOCKET_INPUT_OUTPUT };
enum { EL_SOCKET_LOCAL, EL_SOCKET_REMOTE};
enum { EL_SOCKET_IOBUF_NONE, EL_SOCKET_IOBUF_FULL, EL_SOCKET_IOBUF_LINE };

#define EL_SOCKET_FILE_BUFSIZE		1024	/* less than ethernet MTU (~1500) */

typedef struct {
	elSocket_t *s;
	/*
	 * I/O buffer (similar to ('FILE'):
	 */
	struct SelSocketBuf {
		byte *base; /* address of allocated buffer */
		byte *ptr;	/* pointer to first available byte in buffer 'base' */
		int size;	/* size in bytes of allocated buffer 'base' */
		int cnt;	/* current number of bytes available in buffer at 'ptr' */
	} buf[2];	/* input ('SOCKET_INPUT') and output ('SOCKET_OUTPUT') buffer */
	bool nonblock;
} elSocketStream_t;
typedef elSocketStream_t EL_SOCKET_FILE;	/* similar to 'FILE */
typedef struct SelSocketBuf elSocketBuf_t;

#ifdef __cplusplus
extern "C" {			/* C Plus Plus function bindings */
#endif

elErr_t *elSocketStartup(void);
elErr_t	*elSocketCleanup(void);
elErr_t *elSocketCreate(elSocket_t **p_s);
elErr_t *elSocketSetopt(
	elSocket_t *s,int level,int optname,void *optval,int optlen
);
elErr_t *elSocketGetopt(
	elSocket_t *s,int level,int optname,void *optval,int *optlen
);
elErr_t *elSocketConnect(elSocket_t *s,struct sockaddr *addr,int addrlen);
elErr_t *elSocketBind(elSocket_t *s,struct sockaddr *addr,int addrlen);
elErr_t *elSocketListen(elSocket_t *s,int backlog);
elErr_t *elSocketAccept(
	elSocket_t *s,struct sockaddr *addr,int *addrlen,elSocket_t **p_ns
);
elErr_t *elSocketOpen(elSocket_t **p_s,int mode,TCHAR *addr,char *attrs);
elErr_t *elSocketClose(elSocket_t *s);
elErr_t *elSocketReady(elSocket_t *s,int mode,long timeout,bool *p_ready);
elErr_t *elSocketRecv(elSocket_t *s,void *buf,int len,int flags,int *p_len);
elErr_t *elSocketSend(elSocket_t *s,void *buf,int len,int flags,int *p_len);
elErr_t *elSocketShutdown(elSocket_t *s,int how);
elErr_t *elSocketGetaddr(
	elSocket_t *s,int mode,struct sockaddr *addr,int *paddrlen
);
TCHAR	*elSocketAddrVal2Str(struct sockaddr *addr,int addrlen);
elErr_t *elSocketAddrStr2Val(int mode,TCHAR *str,struct sockaddr *addr);
elErr_t *elSocketNonblock(elSocket_t *s,bool enable);

#ifdef __cplusplus
}						/* C Plus Plus function bindings */
#endif

#endif 	/* _EL_SOCKET_H */
