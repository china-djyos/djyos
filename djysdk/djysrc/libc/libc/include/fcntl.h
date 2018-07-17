
#ifndef __FCNTL_H__
#ifdef __cplusplus
extern "C" {
#endif
#define __FCNTL_H__
#include <_ansi.h>
//
#include <stddef.h>
// 解释为Operation mode
#ifndef O_ACCMODE
 #define O_ACCMODE                  0x00000003      //(00000003)/* 模式屏蔽位 */
#else
 #warning "O_ACCMODE" duplicate definition!
#endif
//注：O_RDONLY、O_WRONLY、O_RDWR 符合POSIX（2016）的最新定义
//    旧版本中，他们分别是0、1、2
#ifndef O_RDONLY
 #define O_RDONLY                   0x00000001      //(00000001)
#else
 #warning "O_RDONLY" duplicate definition!
#endif
#ifndef O_WRONLY
 #define O_WRONLY                   0x00000002      //(00000002)
#else
 #warning "O_WRONLY" duplicate definition!
#endif
#ifndef O_RDWR
 #define O_RDWR                     0x00000003      //(00000003)
#else
 #warning "O_RDWR" duplicate definition!
#endif
#ifndef O_CREAT
 #define O_CREAT                    0x00000040      //(00000100)
#else
 #warning "O_CREATE" duplicate definition!
#endif
#ifndef O_EXCL
 #define O_EXCL                     0x00000080      //(00000200)
#else
 #warning "O_EXCL" duplicate definition!
#endif
#ifndef O_NOCTTY
 #define O_NOCTTY                   0x00000100      //(00000400)
#else
 #warning "O_NOCTTY" duplicate definition!
#endif
#ifndef O_TRUNC
 #define O_TRUNC                    0x00000200      //(00001000)
#else
 #warning "O_TRUNC" duplicate definition!
#endif
#ifndef O_APPEND
 #define O_APPEND                   0x00000400      //(00002000)
#else
 #warning "O_APPEND" duplicate definition!
#endif
#ifndef O_NONBLOCK
 #define O_NONBLOCK                 0x00000800      //(00004000)
#else
 #warning "O_NONBLOCK" duplicate definition!
#endif
#ifndef O_DSYNC
 #define O_DSYNC                    0x00001000      //(00010000)
#else
 #warning "O_DSYNC" duplicate definition!
#endif
//O_SYNC定义是为了兼容老的代码，新代码不允许再使用
#ifndef O_SYNC
#define __O_SYNC                    0x00100000      //04000000
#define O_SYNC      (__O_SYNC|O_DSYNC)
#endif
#ifndef FASYNC
#define FASYNC                      0x00002000      //00020000    /* fcntl, for BSD compatibility */
#endif
#ifndef O_DIRECT
 #define O_DIRECT                   0x00004000      //(00040000)
#else
 #warning "O_DIRECT" duplicate definition!
#endif
#ifndef O_LARGEFILE
 #define O_LARGEFILE                0x00008000      //(00100000)
#else
 #warning "O_LARGEFILE" duplicate definition!
#endif
#ifndef O_DIRECTORY
#define O_DIRECTORY                 0x00010000      //(00200000)
#else
 #warning "O_DIRECTORY" duplicate definition!
#endif
#ifndef O_NOFOLLOW
 #define O_NOFOLLOW                 0x00020000      //(00400000)
#else
 #warning "O_NOFOLLOW" duplicate definition!
#endif
#ifndef O_NOATIME
#define O_NOATIME                   0x00040000      //(01000000)
#else
 #warning "O_NOATIME" duplicate definition!
#endif
#ifndef O_CLOEXEC
 #define O_CLOEXEC                  0x00080000      //(02000000)
#else
 #warning "O_CLOEXEC" duplicate definition!
#endif

// stat查询，解释为Specification，特性
#ifndef S_IFMT
 #define S_IFMT                     0x0000F000      //(00170000)// 1111-0000-0000-0000 文件类型屏蔽位
#else
 #warning "S_IFMT" duplicate definition!
#endif
#ifndef S_IFSOCK
 #define S_IFSOCK                   0x0000c000      //(00140000)// 1100-0000-0000-0000
#else
 #warning "S_IFSOCK" duplicate definition!
#endif
#ifndef S_IFLNK
 #define S_IFLNK                    0x0000a000      //(00120000)// 1010-0000-0000-0000
#else
 #warning "S_IFlINK" duplicate definition!
#endif
#ifndef S_IFREG
 #define S_IFREG                    0x00008000      //(00100000)// 1000-0000-0000-0000
#else
 #warning "S_IFREG" duplicate definition!
#endif
#ifndef S_IFBLK
 #define S_IFBLK                    0x00006000      //(00060000)// 0110-0000-0000-0000
#else
 #warning "S_IFBLK" duplicate definition!
#endif
#ifndef S_IFDIR
 #define S_IFDIR                    0x00004000      //(00040000)// 0100-0000-0000-0000
#else
 #warning "S_IFDIR" duplicate definition!
#endif
#ifndef S_IFCHR
 #define S_IFCHR                    0x00002000      //(00020000)// 0010-0000-0000-0000
#else
 #warning "S_IFCHR" duplicate definition!
#endif
#ifndef S_IFIFO
 #define S_IFIFO                    0x00001000      //(00010000)// 0001-0000-0000-0000
#else
 #warning "S_IFIFO" duplicate definition!
#endif
#ifndef __S_IBUF // 私有定义，表示文件允许在用户态下进行缓存
 #define __S_IBUF                   0x00003000      // 表示可以作为C文件可以作缓冲；
#else
 #error "__S_IBUF" duplicate definition!
#endif

/* For machines which care - */
#if defined (_WIN32) || defined (__CYGWIN__)
#define _FBINARY        0x10000
#define _FTEXT          0x20000
#define _FNOINHERIT 0x40000

#define O_BINARY    _FBINARY
#define O_TEXT      _FTEXT
#define O_NOINHERIT _FNOINHERIT
/* O_CLOEXEC is the Linux equivalent to O_NOINHERIT */
#define O_CLOEXEC   _FNOINHERIT

/* The windows header files define versions with a leading underscore.  */
#define _O_RDONLY   O_RDONLY
#define _O_WRONLY   O_WRONLY
#define _O_RDWR     O_RDWR
#define _O_APPEND   O_APPEND
#define _O_CREAT    O_CREAT
#define _O_TRUNC    O_TRUNC
#define _O_EXCL     O_EXCL
#define _O_TEXT     O_TEXT
#define _O_BINARY   O_BINARY
#define _O_RAW      O_BINARY
#define _O_NOINHERIT    O_NOINHERIT
#endif      //for defined (_WIN32) || defined (__CYGWIN__)


/* XXX close on exec request; must match UF_EXCLOSE in user.h */
#define FD_CLOEXEC  1   /* posix */

//fcntl 函数的命令码，以下是POSIX规定的部分
#define F_DUPFD             0                   // Duplicate fildes
#define F_DUPFD_CLOEXEC     1                   // As F_DUPFD, but set close-on-exec flag
#define F_GETFD             2                   // Get fildes flags (close on exec)
#define F_SETFD             3                   // Set fildes flags (close on exec)
#define F_GETFL             4                   // 获取文件的状态标志
#define F_SETFL             5                   // 设置文件的状态标志；
#define F_GETOWN            6                   // Get owner - for ASYNC
#define F_SETOWN            7                   // Set owner - for ASYNC
#define F_GETLK             8                   // Get record-locking information
#define F_SETLK             9                   // Set or Clear a record-lock (Non-Blocking)
#define F_SETLKW            10                  // Set or Clear a record-lock (Blocking)
#define F_RDLCK             11                  // read lock
#define F_WRLCK             12                  // write lock
#define F_UNLCK             13                  // remove lock(s)G
#define F_SETREPRESENT      14                  // 设置文件的表示；
#define F_GETREPRESENT      15                  // 获取文件的表示；

#define F_OF                100                 // 文件系统私有命令；
#define F_OF_GET            (F_OF+1)            // 获取文件的描述；
#define F_OF_SETCONTEXT     (F_OF+2)            // 设置文件的私有上下文；
#define F_OF_GETCONTEXT     (F_OF+3)            // 获取文件的私有上下文；
#define F_SETEVENT          (F_OF+4)            // 设置文件的多路复用逻辑；
#define F_CLREVENT          (F_OF+5)            // 清除文件的多路复用逻辑；
#define F_OF_GETTIMEOUT     (F_OF+6)            // 设置同步IO阻塞的最长时间
#define F_OF_SETTIMEOUT     (F_OF+7)            // 设置同步IO阻塞的最长时间
#define F_OF_CLRTAG         (F_OF+8)            // 清除文件TAG
#define F_OF_SETTAG         (F_OF+9)            // 设置文件TAG

#define F_MODULE            (200)               // 各模块的私有命令，不同文件系统的命令值可以相同；
#define F_STDIO_REDRIECT    (F_MODULE)          // STDIO重定向
#define F_STDIO_MULTI_ADD   (F_MODULE+1)        // 添加新的文件到STDIN多路复用集
#define F_STDIO_MULTI_DEL   (F_MODULE+2)        // 将文件从STDIN多路复用集删除

#define F_SETDDTAG           (F_MODULE)
#define F_GETDDTAG           (F_MODULE+1)
#define F_SETDUTAG           (F_MODULE+2)
#define F_GETDUTAG           (F_MODULE+3)

#if 0
#define CN_FILE_STATICDATA  (1<<0)              // 文件用于存储静态数据
#define CN_FILE_FLOWDATA    (1<<1)              // 文件用于存储流式，不可写缓冲，不可seek
//文件访问状态，位掩码，只有低24bit有效，其中16~23bit是留给用户定义的，系统只能
//用0~15bit。
//是fd而不是 object 的属性。如一个object被打开多次，可能创建多个 fd
//当文件可写时，两个CN_FDFLAG_IN标志都会被设置，其中一个读取数据后，
//其fd的 CN_FDFLAG_IN 标志可能会被清掉，但另一个fd的 CN_FDFLAG_IN 标志仍然会保留。
#define CN_FDFLAG_IN        0x1   //表示对应的文件描述符可以读（包括对端SOCKET正常关闭）；
#define CN_FDFLAG_OUT       0x2   //表示对应的文件描述符可以写；
#define CN_FDFLAG_PRI       0x4   //表示对应的文件描述符有紧急的数据可读（例如tcp/ip有带外数据到来）；
#define CN_FDFLAG_ERR       0x8   //表示对应的文件描述符发生错误；
#define CN_FDFLAG_HUP       0x10  //表示对应的文件描述符被挂断；
// seek相关参数
#define SEEK_SET            0 // 文件开头位置
#define SEEK_CUR            1 // 文件当前位置
#define SEEK_END            2 // 文件结尾位置
#endif
/*#include <sys/stdtypes.h>*/

#ifndef __CYGWIN__
/* file segment locking set data type - information passed to system by user */
struct flock {
    short   l_type;     /* F_RDLCK, F_WRLCK, or F_UNLCK */
    short   l_whence;   /* flag to choose starting offset */
    long    l_start;    /* relative offset, in bytes */
    long    l_len;      /* length, in bytes; 0 means lock to EOF */
    short   l_pid;      /* returned with F_GETLK */
    short   l_xxx;      /* reserved for future use */
};
#endif /* __CYGWIN__ */

#ifndef _POSIX_SOURCE
/* extended file segment locking set data type */
struct eflock {
    short   l_type;     /* F_RDLCK, F_WRLCK, or F_UNLCK */
    short   l_whence;   /* flag to choose starting offset */
    long    l_start;    /* relative offset, in bytes */
    long    l_len;      /* length, in bytes; 0 means lock to EOF */
    short   l_pid;      /* returned with F_GETLK */
    short   l_xxx;      /* reserved for future use */
    long    l_rpid;     /* Remote process id wanting this lock */
    long    l_rsys;     /* Remote system id wanting this lock */
};
#endif  /* !_POSIX_SOURCE */

s32 open(const char *pathname, s32 fmt, ...);
s32 close(s32 fd);
s32 read(s32 fd, void *buf, size_t count);
s32 write(s32 fd, const void *buf, size_t count);
int fcntl(int fd,int cmd,...);

#ifdef __cplusplus
}
#endif
#endif  /* !__FCNTL_H__ */
