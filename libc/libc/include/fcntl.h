
#ifndef __FCNTL_H__
#ifdef __cplusplus
extern "C" {
#endif
#define __FCNTL_H__
#include <_ansi.h>
//
#include <stddef.h>

//O_RDONLY ��ֻ����ʽ���ļ�
//O_WRONLY ��ֻд��ʽ���ļ�
//O_RDWR �Կɶ�д��ʽ���ļ�. ������������ǻ����, Ҳ���ǲ���ͬʱʹ��, ���������е��������OR(|)��������.
//O_CREAT �����򿪵��ļ����������Զ��������ļ�.
//O_EXCL ���O_CREAT Ҳ������, ��ָ���ȥ����ļ��Ƿ����. �ļ����������������ļ�, ���򽫵��´��ļ�����. ����, ��O_CREAT ��O_EXCL ͬʱ����, �������򿪵��ļ�Ϊ��������, �����ļ�ʧ��.
//O_NOCTTY ������򿪵��ļ�Ϊ�ն˻��豸ʱ, �򲻻Ὣ���ն˻����ɽ��̿����ն˻�.
//O_TRUNC ���ļ����ڲ����Կ�д�ķ�ʽ��ʱ, ���������ļ�������Ϊ0, ��ԭ�����ڸ��ļ�������Ҳ����ʧ.
//O_APPEND ����д�ļ�ʱ����ļ�β��ʼ�ƶ�, Ҳ������д������ݻ��Ը��ӵķ�ʽ���뵽�ļ�����.
//O_NONBLOCK �Բ�����ϵķ�ʽ���ļ�, Ҳ���������������ݶ�ȡ��ȴ�, �����������ؽ���֮��.
//O_NDELAY ͬO_NONBLOCK.
//O_SYNC ��ͬ���ķ�ʽ���ļ�.
//O_NOFOLLOW �������pathname ��ָ���ļ�Ϊһ��������, �������ļ�ʧ��.
//O_DIRECTORY �������pathname ��ָ���ļ�����ΪһĿ¼, �������ļ�ʧ�ܡ�

// ����ΪOperation mode
#ifndef O_ACCMODE
 #define O_ACCMODE                  0x00000003      //(00000003)/* ģʽ����λ */
#else
 #warning "O_ACCMODE" duplicate definition!
#endif
//ע��O_RDONLY��O_WRONLY��O_RDWR ����POSIX��2016�������¶���
//    �ɰ汾�У����Ƿֱ���0��1��2
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
//O_SYNC������Ϊ�˼����ϵĴ��룬�´��벻������ʹ��
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

//djyos���ӵĶ��壬�������豸�ļ�
#ifndef O_BLOCK_COMPLETE
 #define O_BLOCK_COMPLETE           0x80000000      //Write����������Ǵ������
                                                    //����д��buffer�������
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

//fcntl �����������룬������POSIX�涨�Ĳ���
#define F_DUPFD             0                   // Duplicate fildes
#define F_DUPFD_CLOEXEC     1                   // As F_DUPFD, but set close-on-exec flag
#define F_GETFD             2                   // Get fildes flags (close on exec)
#define F_SETFD             3                   // Set fildes flags (close on exec)
#define F_GETFL             4                   // ��ȡ�ļ���״̬��־
#define F_SETFL             5                   // �����ļ���״̬��־��
#define F_GETOWN            6                   // Get owner - for ASYNC
#define F_SETOWN            7                   // Set owner - for ASYNC
#define F_GETLK             8                   // Get record-locking information
#define F_SETLK             9                   // Set or Clear a record-lock (Non-Blocking)
#define F_SETLKW            10                  // Set or Clear a record-lock (Blocking)
#define F_RDLCK             11                  // read lock
#define F_WRLCK             12                  // write lock
#define F_UNLCK             13                  // remove lock(s)G

//fcntl �����������룬������djyos����Ĳ���
#define F_LOCAL             (0x1000)               // �ļ�ϵͳ˽�����
#define F_SETCONTEXT        (F_LOCAL+0)         // �����ļ���˽�������ģ�
#define F_GETCONTEXT        (F_LOCAL+1)         // ��ȡ�ļ���˽�������ģ�
#define F_SETEVENT          (F_LOCAL+2)         // �����ļ��Ķ�·�����߼���
#define F_CLREVENT          (F_LOCAL+3)         // ����ļ��Ķ�·�����߼���
#define F_SETTIMEOUT        (F_LOCAL+4)         // ����ͬ��IO�������ʱ�䣻
#define F_GETTIMEOUT        (F_LOCAL+5)         // ��ȡͬ��IO�������ʱ�䣻
#define F_GETTAG            (F_LOCAL+6)         // ��ȡ�ļ���ǩ��
#define F_SETTAG            (F_LOCAL+7)         // �����ļ���ǩ��
#define F_GETFILE           (F_LOCAL+8)        // ��ȡ�ļ��Ľṹ��Ϣ��

// �豸ģ����Ҫ������
#define F_DEV_M             (0x2000)
#define F_SETDRVTAG         (F_DEV_M)           // �����豸������ǩ��
#define F_GETDRVTAG         (F_DEV_M+1)         // ��ȡ�豸��������ǩ��
#define F_SETUSERTAG        (F_DEV_M+2)         // �����豸���û���ǩ��
#define F_GETUSERTAG        (F_DEV_M+3)         // ��ȡ�豸���û���ǩ��
#define F_DSTART            (F_DEV_M+4)         // �����豸����Щ�ܿ��Ƶ�Դ���豸��Ҫ
#define F_DSTOP             (F_DEV_M+5)         // �����豸����Щ�ܿ��Ƶ�Դ���豸��Ҫ
#define F_DSHUTDOWN         (F_DEV_M+6)         // �رյ�Դ,�ϵ�ǰ��Ҫ�ض�������豸��Ҫ
#define F_DSLEEP            (F_DEV_M+7)         // �豸����͹���״̬,���ڵ�Դ����
#define F_DRESUME           (F_DEV_M+8)         // �豸�ָ�,�ӵ͹���״̬����
#define F_DCHECK            (F_DEV_M+9)         // ����豸״̬
#define F_DHOOK             (F_DEV_M+10)        // ����������/���/����ʱ�ص�����
#define F_DBLOCK_BUFFER     (F_DEV_M+11)        // Write����������Ƿ��͵�������
#define F_DBLOCK_COPLETE    (F_DEV_M+12)        // Write����������Ǵ������

#define F_DEV_USER          (F_DEV_M+256)       // �豸�����������ͬ�豸������������ͬ��

// STDIOģ��
#define F_STDIO_M           (0x3000)
#define F_STDIO_REDRIECT    (F_STDIO_M)         // STDIO�ض���
#define F_STDIO_MULTI_ADD   (F_STDIO_M+1)       // ����µ��ļ���STDIN��·���ü�
#define F_STDIO_MULTI_DEL   (F_STDIO_M+2)       // ���ļ���STDIN��·���ü�ɾ��

//�ļ�����״̬��λ���룬ֻ�е�24bit��Ч������16~23bit�������û�����ģ�ϵͳֻ��
//��0~15bit��
//��fd������ object �����ԡ���һ��object���򿪶�Σ����ܴ������ fd
//���ļ���дʱ������CN_FDFLAG_IN��־���ᱻ���ã�����һ����ȡ���ݺ�
//��fd�� CN_FDFLAG_IN ��־���ܻᱻ���������һ��fd�� CN_FDFLAG_IN ��־��Ȼ�ᱣ����
#define CN_FDFLAG_IN        0x1   //��ʾ��Ӧ���ļ����������Զ��������Զ�SOCKET�����رգ���
#define CN_FDFLAG_OUT       0x2   //��ʾ��Ӧ���ļ�����������д��
#define CN_FDFLAG_PRI       0x4   //��ʾ��Ӧ���ļ��������н��������ݿɶ�������tcp/ip�д������ݵ�������
#define CN_FDFLAG_ERR       0x8   //��ʾ��Ӧ���ļ���������������
#define CN_FDFLAG_HUP       0x10  //��ʾ��Ӧ���ļ����������Ҷϣ�
// seek��ز���

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
