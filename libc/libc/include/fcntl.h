
#ifndef __FCNTL_H__
#ifdef __cplusplus
extern "C" {
#endif
#define __FCNTL_H__
#include <_ansi.h>
//
#include <stddef.h>

//O_RDONLY 以只读方式打开文件
//O_WRONLY 以只写方式打开文件
//O_RDWR 以可读写方式打开文件. 上述三种旗标是互斥的, 也就是不可同时使用, 但可与下列的旗标利用OR(|)运算符组合.
//O_CREAT 若欲打开的文件不存在则自动建立该文件.
//O_EXCL 如果O_CREAT 也被设置, 此指令会去检查文件是否存在. 文件若不存在则建立该文件, 否则将导致打开文件错误. 此外, 若O_CREAT 与O_EXCL 同时设置, 并且欲打开的文件为符号连接, 则会打开文件失败.
//O_NOCTTY 如果欲打开的文件为终端机设备时, 则不会将该终端机当成进程控制终端机.
//O_TRUNC 若文件存在并且以可写的方式打开时, 此旗标会令文件长度清为0, 而原来存于该文件的资料也会消失.
//O_APPEND 当读写文件时会从文件尾开始移动, 也就是所写入的数据会以附加的方式加入到文件后面.
//O_NONBLOCK 以不可阻断的方式打开文件, 也就是无论有无数据读取或等待, 都会立即返回进程之中.
//O_NDELAY 同O_NONBLOCK.
//O_SYNC 以同步的方式打开文件.
//O_NOFOLLOW 如果参数pathname 所指的文件为一符号连接, 则会令打开文件失败.
//O_DIRECTORY 如果参数pathname 所指的文件并非为一目录, 则会令打开文件失败。

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
#ifndef O_EXEC
 #define O_EXEC                     0x00000004      //(00000004)
#else
 #warning "O_EXEC" duplicate definition!
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

//djyos增加的定义，仅用于设备文件
#ifndef O_BLOCK_COMPLETE
 #define O_BLOCK_COMPLETE           0x80000000      //Write的完成条件是传输完成
                                                    //否则写入buffer即算完成
#else
 #warning "O_BLOCK_COMPLETE" duplicate definition!
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

//fcntl 函数的命令码，以下是djyos定义的部分
#define F_LOCAL             (0x1000)               // 文件系统私有命令；
#define F_SETCONTEXT        (F_LOCAL+0)         // 设置文件的私有上下文；
#define F_GETCONTEXT        (F_LOCAL+1)         // 获取文件的私有上下文；
#define F_SETEVENT          (F_LOCAL+2)         // 设置文件的多路复用逻辑；
#define F_CLREVENT          (F_LOCAL+3)         // 清除文件的多路复用逻辑；
#define F_SETTIMEOUT        (F_LOCAL+4)         // 设置同步IO阻塞的最长时间；
#define F_GETTIMEOUT        (F_LOCAL+5)         // 获取同步IO阻塞的最长时间；
#define F_GETTAG            (F_LOCAL+6)         // 获取文件标签；
#define F_SETTAG            (F_LOCAL+7)         // 设置文件标签；
#define F_GETFILE           (F_LOCAL+8)        // 获取文件的结构信息；

// 设备模块需要的命令
#define F_DEV_M             (0x2000)
#define F_SETDRVTAG         (F_DEV_M)           // 设置设备驱动标签；
#define F_GETDRVTAG         (F_DEV_M+1)         // 获取设备的驱动标签；
#define F_SETUSERTAG        (F_DEV_M+2)         // 设置设备的用户标签；
#define F_GETUSERTAG        (F_DEV_M+3)         // 获取设备的用户标签；
#define F_DSTART            (F_DEV_M+4)         // 启动设备，有些能控制电源的设备需要
#define F_DSTOP             (F_DEV_M+5)         // 启动设备，有些能控制电源的设备需要
#define F_DSHUTDOWN         (F_DEV_M+6)         // 关闭电源,断电前需要特定处理的设备需要
#define F_DSLEEP            (F_DEV_M+7)         // 设备进入低功耗状态,用于电源管理
#define F_DRESUME           (F_DEV_M+8)         // 设备恢复,从低功耗状态唤醒
#define F_DCHECK            (F_DEV_M+9)         // 检查设备状态
#define F_DHOOK             (F_DEV_M+10)        // 设置有输入/输出/错误时回调函数
#define F_DBLOCK_BUFFER     (F_DEV_M+11)        // Write的完成条件是发送到缓冲区
#define F_DBLOCK_COPLETE    (F_DEV_M+12)        // Write的完成条件是传输完成

#define F_DEV_USER          (F_DEV_M+256)       // 设备驱动命令集，不同设备的驱动可以相同；

// STDIO模块
#define F_STDIO_M           (0x3000)
#define F_STDIO_REDRIECT    (F_STDIO_M)         // STDIO重定向
#define F_STDIO_MULTI_ADD   (F_STDIO_M+1)       // 添加新的文件到STDIN多路复用集
#define F_STDIO_MULTI_DEL   (F_STDIO_M+2)       // 将文件从STDIN多路复用集删除

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
s32 fcntl(s32 fd,s32 cmd,...);

#ifdef __cplusplus
}
#endif
#endif  /* !__FCNTL_H__ */
