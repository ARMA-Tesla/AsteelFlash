/*
 *  "elSocket.c" - TCP/IP socket routines.
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

#if 1
static char _ident[] = "@(#) elSocket.c 0.1 2007-03-05";
#else
#endif

#include <elBase.h>
#include <elErr.h>
#include <elErrWsa.h>
#include <elLog.h>
#include <elMem.h>
#include <elSocket.h>

static elErr_t gs_err_socket = {
	TRUE,
	{ { {EL_ERR_SOCKET},	{_T("Socket error)")} }, NULL, 1, 0 },
	elErrLog
};

static struct {
	int			debug;
	elUInt32_t	startup_cnt;
} gs;

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elSocketStartup"
elErr_t *elSocketStartup(void)
{
	EL_ERR_VAR;

	if ( !gs.startup_cnt++ )
	{
#if WATTCP
		sock_init();
		if ( gs.debug )
    		elLogD("%s(): sizeof(tcp_Socket): %d",__FUNC__,sizeof(tcp_Socket));
#endif

#if WIN

		{	/* inner block */
		WSADATA wsaData;
		int wsaerrno;

		/*
		 * Initialize Windows Socket DLL
		 */
		if ( (wsaerrno = WSAStartup(
#if 1
			MAKEWORD(1,1),	/* at least version 1.1 */
#else
			MAKEWORD(2,2),	/* at least version 2.2 */
#endif
			&wsaData)) != 0 )
		{
			elLog("%s(): WSAStartup(): %s",__FUNC__,elErrWsaStr(wsaerrno));
			EL_ERR_THROW(EL_ERR_SOCKET,NULL,NULL);
		}

		if ( gs.debug )
		{
			elLogD("wsaData:");
			elLogD("\n       Version: %d.%d",
				HIBYTE(wsaData.wVersion),
				LOBYTE(wsaData.wVersion)
			);
			elLogD("\n   HighVersion: %d.%d",
				HIBYTE(wsaData.wHighVersion),
				LOBYTE(wsaData.wHighVersion)
			);
			elLogD("\n   Description: %s",	wsaData.szDescription);
			elLogD("\n  SystemStatus: %s",	wsaData.szSystemStatus);
			elLogD("\n    MaxSockets: %hu",	wsaData.iMaxSockets);
			elLogD("\n      MaxUdpDg: %hu",	wsaData.iMaxUdpDg);
#if 0	/* non-sense characters: */
			elLogD("\n    VendorInfo: %s",	wsaData.lpVendorInfo);
#endif
		}

#if 0
		l_sock.hwnd = CreateWindow("STATIC","POP3WsaEvt",0,0,0,0,0,
						NULL,NULL,NULL,NULL);
		if ( !l_sock.hwnd )
		{
			elLog("CreateWindow(): %M");
			return -1;
		}

		/*
		 * Register Windows message for socket events:
		 */
		if ( !WaitEvtAddMessage(EVT_SOCK,WM_MYSOCK,&l_sock.msg) )
			return -1;

		l_sock.host			= host;
		l_sock.connect.done	= NO;
#endif

		}	/* inner block */

#endif	/* WIN */

	}

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elSocketCleanup"
elErr_t *elSocketCleanup(void)
{
	EL_ERR_VAR;

	if ( gs.startup_cnt )
	{
		gs.startup_cnt--;
		if ( !gs.startup_cnt )
		{
#if WIN
			if ( WSACleanup() == SOCKET_ERROR )
				elLog("%s(): WSACleanup(): %s",__FUNC__,elErrWsaGetLastErrorStr());
#endif
		}
	}

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elSocketCreate"
elErr_t *elSocketCreate(elSocket_t **p_s)
{
	EL_ERR_VAR;
	elSocketHandle_t sh;
	elSocket_t *s;

#if !SOCKET_TICK
	if ( gs.debug )
		elLogD("socket() ...");
	sh = socket(AF_INET,SOCK_STREAM,0);
	if ( gs.debug )
		elLogD("socket(): %" SOCKET_HANDLE_FMT,sh);
#if WIN
	if ( sh == INVALID_SOCKET )
	{
		elLog("%s(): socket(AF_INET,SOCK_STREAM,0): %s",
			__FUNC__,elErrWsaGetLastErrorStr());
		return NULL;
	}
#else
	if ( sh < 0 )
	{
		elLog("%s(): socket(): %m",__FUNC__);
		return NULL;
	}
#endif
#endif

    if ( gs.debug )
    	elLogD("%s(): sizeof(*s): %d",__FUNC__,sizeof(*s));
	EL_ERR_CHECK( elMemAlloc(&s,sizeof(*s)) );
	memset(s,0,sizeof(*s));
#if SOCKET_TICK
	sh = &s->ts;
#endif
	s->h = sh;

	*p_s = s;

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elSocketSetopt"
elErr_t *elSocketSetopt(
	elSocket_t *s,int level,int optname,void *optval,int optlen
)
{
	EL_ERR_VAR;

	if ( gs.debug )
    	elLogD("%s(): s->h: %" SOCKET_HANDLE_FMT ", level: %d, optname: %d",
        	__FUNC__,s->h,level,optname);

#if WIN
	if ( setsockopt(s->h,level,optname,(char *)optval,optlen)
		== SOCKET_ERROR )
	{
		elLog("%s(): setsockopt(): %s\n",__FUNC__,elErrWsaGetLastErrorStr());
		EL_ERR_THROW(EL_ERR_SOCKET,NULL,NULL);
	}
	if ( optname == SO_RCVTIMEO )
		s->timeout.recv = *(int *)optval;
	else if ( optname == SO_SNDTIMEO )
		s->timeout.send = *(int *)optval;
#elif SOCKET_TICK

#else
	if ( setsockopt(s->h,level,optname,(char *)optval,optlen) < 0 )
	{
		elLog("%s(): setsockopt(): %m\n",__FUNC__);
		return -1;
	}
#endif

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elSocketGetopt"
elErr_t *elSocketGetopt(
	elSocket_t *s,int level,int optname,void *optval,int *optlen
)
{
	EL_ERR_VAR;

	if ( gs.debug )
    	elLogD("%s(): s->h: %" SOCKET_HANDLE_FMT ", level: %d, optname: %d",
        	__FUNC__,s->h,level,optname);

#if WIN
	if ( getsockopt(s->h,level,optname,(char *)optval,optlen)
		== SOCKET_ERROR )
	{
		elLog("%s(): getsockopt(): %s\n",__FUNC__,elErrWsaGetLastErrorStr());
		EL_ERR_THROW(EL_ERR_SOCKET,NULL,NULL);
	}
#elif SOCKET_TICK

#else
	if ( getsockopt(s->h,level,optname,(char *)optval,optlen) < 0 )
	{
		elLog("%s(): getsockopt(): %m\n",__FUNC__);
		return -1;
	}
#endif

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elSocketConnect"
elErr_t *elSocketConnect(elSocket_t *s,struct sockaddr *addr,int addrlen)
{
	EL_ERR_VAR;

	int ret = 0;	/* OK */

	if ( gs.debug )
		elLogD("%s(): connect() ...",__FUNC__);

#if WIN
	if ( connect(s->h,addr,addrlen) == SOCKET_ERROR )
	{
		int serr;
		bool ready;

		serr = WSAGetLastError();
		if ( serr == WSAEWOULDBLOCK && s->nonblock )
		{
			EL_ERR_CHECK( elSocketReady(s,EL_SOCKET_OUTPUT,s->timeout.send,&ready) );
			if ( !ready )
				ret = -1;
		}
		else
			ret = -1;
		if ( ret < 0 )
		{
#if 0
			elLog("%s(): connect(): %s",__FUNC__,elErrWsaStr(serr));
#endif
			EL_ERR_THROW(EL_ERR_SOCKET,"connect()",elErrWsaStr(serr));
		}
	}
#elif SOCKET_TICK

#else
	if ( connect(s->h,addr,addrlen) < 0 )
	{
		elLog("%s(): connect(): %m",__FUNC__);
		ret = -1;
	}
#endif

	if ( gs.debug )
		elLogD("%s(): connect(): %d",__FUNC__,ret);

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elSocketBind"
elErr_t *elSocketBind(elSocket_t *s,struct sockaddr *addr,int addrlen)
{
	EL_ERR_VAR;
	int ret = 0;	/* OK */

	if ( gs.debug )
		elLogD("%s(): addr: %s",__FUNC__,elSocketAddrVal2Str(addr,addrlen));

	if ( gs.debug )
		elLog("%s(): bind() ...",__FUNC__);

#if WIN
	if ( bind(s->h,addr,addrlen) == SOCKET_ERROR )
	{
		elLog("%s(): bind(): %s",__FUNC__,elErrWsaGetLastErrorStr());
		ret = -1;
	}
#elif SOCKET_TICK

#else
	if ( bind(s->h,addr,addrlen) < 0 )
	{
		elLog("%s(): bind(): %m",__FUNC__);
		ret = -1;
	}
#endif

	if ( gs.debug )
		elLogD("%s(): bind(): %d",__FUNC__,ret);

#if 0
Error:
#endif

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elSocketListen"
elErr_t *elSocketListen(elSocket_t *s,int backlog)
{
	EL_ERR_VAR;

#if WIN
	if ( listen(s->h,backlog) == SOCKET_ERROR )
	{
		elLog("%s(): listen(): %s",__FUNC__,elErrWsaGetLastErrorStr());
		EL_ERR_THROW(EL_ERR_SOCKET,NULL,NULL);
	}
#elif SOCKET_TICK

#else
	listen(s->h,5);
#endif

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elSocketAccept"
elErr_t *elSocketAccept(
	elSocket_t *s,struct sockaddr *addr,int *addrlen,elSocket_t **p_ns
)
{
	EL_ERR_VAR;
	elSocketHandle_t nsh;
	elSocket_t *ns;

#if SOCKET_TICK
    nsh = NULL;
#else
	nsh = accept(s->h,(struct sockaddr *)addr,addrlen);

	if ( gs.debug )
	{
		elLogD("%s(): accept(): %" SOCKET_HANDLE_FMT,__FUNC__,nsh);
		elLogD("\n - remote address: %s",
			elSocketAddrVal2Str((struct sockaddr *)addr,*addrlen));
	}

#if WIN
	if ( nsh == INVALID_SOCKET )
	{
		elLog("%s(): accept(): %s",__FUNC__,elErrWsaGetLastErrorStr());
		return NULL;
	}
#else
	if ( nsh < 0 )
	{
		elLog("%s(): accept(): %m",__FUNC__);
		return NULL;
	}
#endif
#endif

	EL_ERR_CHECK( elMemAlloc(&ns,sizeof(*ns)) );
	memset(ns,0,sizeof(*ns));
	ns->h = nsh;

	*p_ns = ns;

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elSocketOpen"
elErr_t *elSocketOpen(elSocket_t **p_s,int mode,TCHAR *addr,char *attrs)
{
	EL_ERR_VAR;
	elSocket_t *s;
	struct sockaddr_in saddr;

	(void) attrs;	/* unreferenced formal parameter */

	if ( gs.debug )
    	elLogD("%s(): mode: %d, addr: %s, attrs: %s",__FUNC__,mode,addr,attrs);

	EL_ERR_CHECK( elSocketAddrStr2Val(mode,addr,(struct sockaddr *)&saddr) );

#if SOCKET_TICK
	s = NULL;
#else
	EL_ERR_CHECK( elSocketCreate(&s) );
#endif

#if 0
	if ( socket_nonblock(s,YES) < 0 )
		return NULL;
#endif

	switch( mode )
	{
	case EL_SOCKET_LISTEN:
	{	/* inner block */
		int val = 1;

		EL_ERR_CHECK(
			elSocketSetopt(s,SOL_SOCKET,SO_REUSEADDR,(char *)&val,sizeof(val))
		);

		EL_ERR_CHECK(
			elSocketBind(s,(struct sockaddr *)&saddr,sizeof(saddr))
		);

		EL_ERR_CHECK( elSocketListen(s,5) );
		break;
	}	/* inner block */

	case EL_SOCKET_CONNECT:
		EL_ERR_CHECK( elSocketConnect(s,(struct sockaddr *)&saddr,sizeof(saddr)) );
		break;

	}	/* switch( mode ) */

	*p_s = s;
	
Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elSocketClose"
elErr_t *elSocketClose(elSocket_t *s)
{
	EL_ERR_VAR;
	int ret = 0;	/* OK */

#if WIN
	if ( gs.debug )
		elLog("%s(): closesocket(%" SOCKET_HANDLE_FMT ") ...",
			__FUNC__,s->h);
	if ( closesocket(s->h) == SOCKET_ERROR )
	{
		elLog("%s(): closesocket(%" SOCKET_HANDLE_FMT "): %s",
			__FUNC__,s->h,elErrWsaGetLastErrorStr());
		ret = -1;
	}
#elif DJGPP
#if SOCKET_TICK
	if ( gs.debug )
		elLog("%s(): sock_close(%" SOCKET_HANDLE_FMT ") ...",
			__FUNC__,s->h);
	sock_close(s->h);
#if 0	/* tcp_tick() loop last very long (probably until some timeout): */
    elLogD("%s(): tcp_tick() loop ...",__FUNC__);
	while( tcp_tick(s->h) )
    	usleep(10*1000);	/* 10ms */
    elLogD("%s(): tcp_tick() loop has completed",__FUNC__);
#else
    {
    int status = 0;
    
#if 0
    elLogD("%s(): sock_wait_closed() ...",__FUNC__);
#endif
#if 1
    sock_wait_closed(s->h,1,NULL,&status);
#else
    sock_wait_closed(s->h,sock_delay,NULL,&status);
#endif
sock_err:
#if 0
	elLogD("%s(): status: %d",__FUNC__,status);
#endif
	}
#endif
#else
	if ( gs.debug )
		elLog("%s(): closesocket(%d) ...",__FUNC__,s->h);
	if ( closesocket(s->h) < 0 )
	{
		elLog("%s(): closesocket(%d): %m\n",__FUNC__,s->h);
		ret = -1;
	}
#endif
#else
	if ( gs.debug )
		elLog("%s(): close(%d) ...",__FUNC__,s->h);
	if ( close(s->h) < 0 )
	{
		elLog("%s(): close(%d): %m\n",__FUNC__,s->h);
		ret = -1;
	}
#endif

	elMemFree(&s);

#if 0
Error:
#endif

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elSocketReady"
elErr_t *elSocketReady(elSocket_t *s,int mode,long timeout,bool *p_ready)
{
	EL_ERR_VAR;
	int ret;

#if SOCKET_TICK
	ret = 1;
#elif WIN
	struct timeval tval;
	fd_set fds;

	FD_ZERO(&fds);
#if defined(_MSC_VER)
/* "warning C4127: conditional expression is constant" */
#pragma warning(disable : 4127)
#endif
	FD_SET(s->h,&fds);
#if defined(_MSC_VER)
#pragma warning(default : 4127)
#endif
	tval.tv_sec  = timeout / 1000;
	tval.tv_usec = (ulong)(timeout % 1000) * 1000;

	if ( gs.debug )
		elLogD("%s(): tval: %ld.%03ld",__FUNC__,tval.tv_sec,tval.tv_usec/1000);

	ret = select(
		1,
		mode == EL_SOCKET_INPUT  ? &fds : NULL,
		mode == EL_SOCKET_OUTPUT ? &fds : NULL,
		NULL,
		&tval
	);

	if ( ret == SOCKET_ERROR )
	{
		elLog("%s(): select(): %s",__FUNC__,elErrWsaGetLastErrorStr());
		EL_ERR_THROW(EL_ERR_SOCKET,NULL,NULL);
	}
#endif

	if ( gs.debug )
		elLogD("%s(): ret: %d",__FUNC__,ret);

	*p_ready = (bool) (ret ? TRUE : FALSE);

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elSocketRecv"
elErr_t *elSocketRecv(elSocket_t *s,void *buf,int len,int flags,int *p_len)
{
	EL_ERR_VAR;
	register int l;

	if ( gs.debug )
    	elLogD(
		"%s(): s->h: %" SOCKET_HANDLE_FMT ", buf: 0x%lX, len: %d, flags: %d",
        	__FUNC__,s->h,buf,len,flags);

	l = 0;

#if WIN
#if 0	/* timeout functionality instead of 'SO_RCVTIMEO' option: */
	{	/* inner block */
	int ret;
	long timeout;
	bool ready;

#if 1
	timeout = s->timeout.recv;
#else
	{	/* inner block */
	int len, t;

	len = sizeof(t);
	ret = getsockopt(s->h,SOL_SOCKET,SO_RCVTIMEO,(char *)&t,&len);
	timeout = t;
	}	/* inner block */
#endif

	if ( timeout )
	{
		EL_ERR_CHECK( elSocketReady(s,EL_SOCKET_INPUT,timeout,&ready) );
		if ( !ready )
			return ret;		/* error/timeout */
	}
	}	/* inner block */
#endif
	if ( s->nonblock )
	{
		bool ready;

		EL_ERR_CHECK( elSocketReady(s,EL_SOCKET_INPUT,s->timeout.recv,&ready) );
		if ( !ready )
			goto done;
	}
	l = recv(s->h,buf,len,flags);
	if ( l == SOCKET_ERROR )
	{
		elLog("%s(): recv(): %s",__FUNC__,elErrWsaGetLastErrorStr());
		EL_ERR_THROW(EL_ERR_SOCKET,NULL,NULL);
	}
#elif SOCKET_TICK
#if 1
	if ( !tcp_tick(s->h) )	/* socket closed ? */
    	return 0;
#endif
    if ( gs.debug )
    	elLogD("%s(): sock_dataready(0x%lX) ...",__FUNC__,s->h);
	if ( sock_dataready(s->h) )
	{
	    if ( gs.debug )
    		elLogD("%s(): sock_fastread() ...",__FUNC__);
		l = sock_fastread(s->h,buf,len);
	    if ( l > 0 )
	    {
        	if ( gs.debug )
    		{
	        	elLogD("%s(): sock_fastread(): %d bytes:",__FUNC__,l);
				HexDump(buf,l,NULL,"-i2");
            }
		}
        else
			l = 0;
	}
	else
		l = 0;
#else
	l = recv(s->h,buf,len,flags);
    if ( l < 0 )
	{
		elLog("%s(): recv(): %m",__FUNC__);
		return -1;
	}
#endif

done:
	if ( gs.debug > 1 )
	{
		elLogD("%s(): l = %d:",__FUNC__,l);
		if ( l )
			elLogHexDump(buf,l,NULL,"-i2");
	}

	*p_len = l;

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elSocketSend"
elErr_t *elSocketSend(elSocket_t *s,void *buf,int len,int flags,int *p_len)
{
	EL_ERR_VAR;
	register int slen;	/* sent length */

	if ( gs.debug > 1 )
    	elLogD(
		"%s(): s->h: %" SOCKET_HANDLE_FMT ", buf: 0x%lX, len: %d, flags: %d",
        	__FUNC__,s->h,buf,len,flags);

#if WIN
	slen = send(s->h,buf,len,flags);
	if ( slen == SOCKET_ERROR )
	{
		elLog("%s(): send(): %s",__FUNC__,elErrWsaGetLastErrorStr());
		EL_ERR_THROW(EL_ERR_SOCKET,NULL,NULL);
	}
#elif SOCKET_TICK

	{	/* inner block */
    register int l;

	slen = 0;
    do
    {
	    if ( gs.debug )
   			elLogD("%s(): sock_fastwrite(%d) ...",__FUNC__,len-slen);
#if 1
		if ( !tcp_tick(s->h) )	/* socket closed ? */
			return -1;
#endif
		l = sock_fastwrite(s->h,buf+slen,len-slen);
	    if ( l >= 0 )
		{
       		if ( gs.debug )
   			{
        		elLogD("%s(): sock_fastwrite(): %d bytes:",__FUNC__,l);
				HexDump(buf,l,NULL,"-i2");
			}
        	slen += l;
        	if ( slen < len )
			{
				if ( gs.debug )
                	elLogD("%s(): tcp_tick() ...",__FUNC__);
				tcp_tick(s->h);
			}
		}
	} while( l >= 0 && slen < len );
	if ( l < 0 )
	{
		elLog("%s(): sock_fastwrite(): %d",__FUNC__,l);
		return -1;
	}
	}	/* inner block */

#else
	slen = send(s->h,buf,len,flags);
	if ( slen < 0 )
	{
		elLog("%s(): send(): %m",__FUNC__);
		return -1;
	}
#endif

	if ( gs.debug > 1 )
	{
		elLogD("%s(): slen = %d:",__FUNC__,slen);
		if ( slen )
			elLogHexDump(buf,slen,NULL,"-i2");
	}

	*p_len = slen;

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elSocketShutdown"
elErr_t *elSocketShutdown(elSocket_t *s,int how)
{
	EL_ERR_VAR;
	int ret;

	if ( gs.debug )
    	elLogD("%s(): s->h: %" SOCKET_HANDLE_FMT ", how: %d",__FUNC__,s->h,how);

#if WIN
	if ( how == 0 || how == EL_SOCKET_INPUT )
#if _MSC_VER < 1400
		how = SD_RECEIVE;
#else
		how = 0;
#endif
	else if ( how == 1 || how == EL_SOCKET_OUTPUT )
#if _MSC_VER < 1400
		how = SD_SEND;
#else
		how = 1;
#endif
	else	/* if ( how == EL_SOCKET_INPUT_OUTPUT ) */
#if _MSC_VER < 1400
		how = SD_BOTH;
#else
		how = 2;
#endif
#endif

#if SOCKET_TICK
#else
	ret = shutdown(s->h,how);
#if WIN
	if ( ret == SOCKET_ERROR )
	{
		elLog("%s(): shutdown(): %s",__FUNC__,elErrWsaGetLastErrorStr());
		EL_ERR_THROW(EL_ERR_SOCKET,NULL,NULL);
	}
#else
	if ( ret < 0 )
	{
		elLog("%s(): shutdown(): %m",__FUNC__);
		return -1;
	}
#endif
#endif

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elSocketGetaddr"
elErr_t *elSocketGetaddr(
	elSocket_t *s,int mode,struct sockaddr *addr,int *paddrlen
)
{
	EL_ERR_VAR;
	int ret;

	if ( gs.debug )
    	elLogD("%s(): s->h: %" SOCKET_HANDLE_FMT ", mode: %d",__FUNC__,s->h,mode);

#if SOCKET_TICK
#if 1
	{	/* inner block */
	struct watt_sockaddr wsaddr;

	*paddrlen = sizeof(wsaddr);
	ret = (mode == EL_SOCKET_LOCAL ? _getsockname : _getpeername)
			(s->h,&wsaddr,paddrlen);
#if 1	/* map from host to network order: */
#if 1
	((struct sockaddr_in *)addr)->sin_addr.s_addr	= htonl(wsaddr.s_ip);
#else	/* little-endian order => network (big-endian) order: */
	((struct sockaddr_in *)addr)->sin_addr.s_addr	=
		  ((byte) (wsaddr.s_ip >>  0) << 24)
		| ((byte) (wsaddr.s_ip >>  8) << 16)
		| ((byte) (wsaddr.s_ip >> 16) <<  8)
		| ((byte) (wsaddr.s_ip >> 24) <<  0);
#endif
	((struct sockaddr_in *)addr)->sin_port			= htons(wsaddr.s_port);
#else
	((struct sockaddr_in *)addr)->sin_addr.s_addr	= wsaddr.s_ip;
	((struct sockaddr_in *)addr)->sin_port			= wsaddr.s_port;
#endif
	*paddrlen = sizeof(struct sockaddr_in);
	}	/* inner block */
#elif 0
    ((struct sockaddr_in *)addr)->sin_addr.s_addr =
    	mode == EL_SOCKET_LOCAL ? s->h->myaddr : s->h->hisaddr;
    ((struct sockaddr_in *)addr)->sin_port =
    	mode == EL_SOCKET_LOCAL ? s->h->myport : s->h->hisport;
#else
	memset(addr,0,sizeof(struct sockaddr_in));
#endif
#else
    ret = (mode == EL_SOCKET_LOCAL ? getsockname : getpeername)
			(s->h,addr,paddrlen);
#if WIN
	if ( ret == SOCKET_ERROR )
	{
		elLog("%s(): %s(): %s",
			__FUNC__,mode == EL_SOCKET_LOCAL ? "getsockname" : "getpeername",
			elErrWsaGetLastErrorStr()
		);
		EL_ERR_THROW(EL_ERR_SOCKET,NULL,NULL);
	}
#else
	if ( ret < 0 )
	{
		elLog("%s(): %s(): %m",
			__FUNC__,mode == EL_SOCKET_LOCAL ? "getsockname" : "getpeername");
		return -1;
	}
#endif
#endif

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elSocketAddrVal2Str"
TCHAR *elSocketAddrVal2Str(struct sockaddr *addr,int addrlen)
{
	(void) addrlen;

	return elStrTmpIdxFmt(
		EL_STR_IDX_SOCKET,
		_T("%d@%s"),
		ntohs(((struct sockaddr_in *)addr)->sin_port),
		inet_ntoa(((struct sockaddr_in *)addr)->sin_addr)
	);
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elSocketAddrStr2Val"
elErr_t *elSocketAddrStr2Val(int mode,TCHAR *str,struct sockaddr *addr)
{
	EL_ERR_VAR;
	TCHAR *host;
	TCHAR *p;
	ushort port;
	struct sockaddr_in *addr_in = (struct sockaddr_in *)addr;

	if ( gs.debug )
    	elLog(_T("%s(): str: %s"),_T(__FUNC__),str);

	/*
	 * 'str': port[@host]
	 */
	port = (ushort) _ttoi(str);
	if ( gs.debug )
		elLog(_T("%s(): port: %d"),_T(__FUNC__),port);

	p = _tcschr(str,_T('@'));
	if ( p )
		host = p+1;
	else
		host = NULL;	/* client: localhost, server: all interfaces */

	memset(addr_in,0,sizeof(struct sockaddr_in));
	addr_in->sin_family = AF_INET;
	addr_in->sin_port = htons(port);
	if ( host )
	{
		if ( _istdigit(*host) )
			addr_in->sin_addr.s_addr = inet_addr(host);
		else
		{
			struct hostent *he;

			if  ( (he = gethostbyname(host)) == NULL )
			{
#if WIN
				EL_ERR_THROW(
					EL_ERR_INVAL,NULL,
					elStrTmpFmt(_T("%s: gethostbyname(): %s"),
						host,
						elErrWsaGetLastErrorStr()
					)
				);
#else
				EL_ERR_THROW(
					EL_ERR_INVAL,NULL,
					elStrTmpFmt(_T("%s: gethostbyname(): %s"),
						host,
						strerror(h_errno)
					)
				);
#endif
			}
			addr_in->sin_family			= he->h_addrtype;
			addr_in->sin_addr.s_addr	= *(ulong *)he->h_addr;
		}
	}
	else if ( mode == EL_SOCKET_CONNECT )
		addr_in->sin_addr.s_addr = inet_addr("127.0.0.1");	/* localhost */

Error:

	return err;
}

/******************************************************************************/
#undef	__FUNC__
#define	__FUNC__	"elSocketNonblock"
elErr_t *elSocketNonblock(elSocket_t *s,bool enable)
{
	EL_ERR_VAR;
#if WIN
	u_long arg;
	int ret;

	arg = enable;
	ret = ioctlsocket(s->h,FIONBIO,&arg);
	if ( ret == SOCKET_ERROR )
	{
		elLog("%s(): ioctlsocket(FIONBIO): %s",__FUNC__,elErrWsaGetLastErrorStr());
		EL_ERR_THROW(EL_ERR_SOCKET,NULL,NULL);
	}
#elif UNIX
	int val = enable;

	if ( ioctl(s->h,FIONBIO,&val) < 0 )
	{
		elLog("%s(): ioctl(FIONBIO): %m",__FUNC__);
		return -1;
	}
#endif

	s->nonblock = enable;

Error:

	return err;
}

/******************************************************************************/
