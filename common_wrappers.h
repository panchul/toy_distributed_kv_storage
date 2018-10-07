
//
// Idiomatic common network function wrappers. For example,
// from Stevens's UNIX Network Programming book
//

#ifndef __COMMON_WRAPPERS_H__
#define __COMMON_WRAPPERS_H__

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <time.h>
#include <cassert>

#include	<time.h>

#include	<sys/types.h>	/* basic system data types */
#include	<sys/socket.h>	/* basic socket definitions */
#include	<sys/time.h>	/* timeval{} for select() */
#include	<time.h>		/* timespec{} for pselect() */
#include	<netinet/in.h>	/* sockaddr_in{} and other Internet defns */
#include	<arpa/inet.h>	/* inet(3) functions */
#include	<errno.h>
#include	<fcntl.h>		/* for nonblocking */
#include	<netdb.h>
#include	<signal.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/stat.h>	/* for S_xxx file mode constants */
#include	<sys/uio.h>		/* for iovec{} and readv/writev */
#include	<unistd.h>
#include	<sys/wait.h>
#include	<sys/un.h>		/* for Unix domain sockets */
#include	<stdarg.h>		/* ANSI C header file */
#include	<syslog.h>		/* for syslog() */


#define	MAXLINE		4096	/* max text line length */
//#define	MAXSOCKADDR  128	/* max socket address structure size */
//#define	BUFFSIZE	8192	/* buffer size for reads and writes */

/* Following could be derived from SOMAXCONN in <sys/socket.h>, but many
   kernels still #define it as 5, while actually supporting many more */
#define	LISTENQ		1024	/* 2nd argument to listen() */

#define	SA	struct sockaddr

void err_quit(const char *fmt, ...);
void err_sys(const char *fmt, ...);
// static
void err_doit(int errnoflag, int level, const char *fmt, va_list ap);
void Close(int fd);
void *Malloc(size_t size);
int tcp_connect(const char *host, const char *serv);
int Tcp_connect(const char *host, const char *serv);
void Getpeername(int fd, struct sockaddr *sa, socklen_t *salenptr);
ssize_t Recv(int fd, void *ptr, size_t nbytes, int flags);
ssize_t Read(int fd, void *ptr, size_t nbytes);
void Fputs(const char *ptr, FILE *stream);

#ifdef	HAVE_SOCKADDR_DL_STRUCT
# include	<net/if_dl.h>
#endif

char *sock_ntop_host(const struct sockaddr *sa, socklen_t salen);
char *Sock_ntop_host(const struct sockaddr *sa, socklen_t salen);
int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr);
void Write(int fd, void *ptr, size_t nbytes);
int Socket(int family, int type, int protocol);
void Bind(int fd, const struct sockaddr *sa, socklen_t salen);
void Listen(int fd, int backlog);

using namespace std;

#endif
