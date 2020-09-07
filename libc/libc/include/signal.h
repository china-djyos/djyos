/*
 * signal.h
 *
 *  Created on: 2015Äê9ÔÂ6ÈÕ
 *      Author: zqf
 */
#include <sys/_sigset.h>

#ifndef SIGNAL_H_
#define SIGNAL_H_

#define _NSIG       64
#define _NSIG_BPW   32
#define _NSIG_WORDS (_NSIG / _NSIG_BPW)

typedef unsigned long old_sigset_t;     /* at least 32 bits */

//typedef struct {
//    unsigned long sig[_NSIG_WORDS];
//} sigset_t;
#if !defined(_SIGSET_T_DECLARED)
#define _SIGSET_T_DECLARED
typedef __sigset_t  sigset_t;
#endif

typedef void (*sighandler_t)(int);
typedef void (*__sighandler_t)(int);
struct old_sigaction {
          __sighandler_t sa_handler;
         old_sigset_t sa_mask;
         unsigned long sa_flags;
         void (*sa_restorer)(void);
};
struct sigaction {
         __sighandler_t sa_handler;
        unsigned long sa_flags;
        void (*sa_restorer)(void);
        sigset_t sa_mask;   /* mask last for extensibility */
};
struct k_sigaction {
        struct sigaction sa;
};


sighandler_t signal(int signum,sighandler_t handler);
int sigemptyset(sigset_t *set);
int sigfillset(sigset_t *set);
int sigaddset(sigset_t *set, int signum);
int sigdelset(sigset_t *set, int signum);
int sigismember(const sigset_t *set, int signum);
int sigaction( int sig, const struct sigaction *act,struct sigaction *oact );



#define SIGHUP       1
#define SIGINT       2
#define SIGQUIT      3
#define SIGILL       4
#define SIGTRAP      5
#define SIGABRT      6
#define SIGIOT       6
#define SIGBUS       7
#define SIGFPE       8
#define SIGKILL      9
#define SIGUSR1     10
#define SIGSEGV     11
#define SIGUSR2     12
#define SIGPIPE     13
#define SIGALRM     14
#define SIGTERM     15
#define SIGSTKFLT   16
#define SIGCHLD     17
#define SIGCONT     18
#define SIGSTOP     19
#define SIGTSTP     20
#define SIGTTIN     21
#define SIGTTOU     22
#define SIGURG      23
#define SIGXCPU     24
#define SIGXFSZ     25
#define SIGVTALRM   26
#define SIGPROF     27
#define SIGWINCH    28
#define SIGIO       29
#define SIGPOLL     SIGIO
/*
#define SIGLOST     29
*/
#define SIGPWR      30
#define SIGSYS      31
#define SIGUNUSED   31

/* These should not be considered constants from userland.  */
#define SIGRTMIN    32
#define SIGRTMAX    _NSIG


#define SIG_IGN   0
#define SIG_DFL   1

#ifndef SIG_BLOCK
#define SIG_BLOCK 0
#endif

#ifndef SIG_UNBLOCK
#define SIG_UNBLOCK 1
#endif

#ifndef SIG_SETMASK
#define SIG_SETMASK 2
#endif


/*
 * SA_FLAGS values:
 *
 * SA_ONSTACK is not currently supported, but will allow sigaltstack(2).
 * SA_RESTART flag to get restarting signals (which were the default long ago)
 * SA_NOCLDSTOP flag to turn off SIGCHLD when children stop.
 * SA_RESETHAND clears the handler when the signal is delivered.
 * SA_NOCLDWAIT flag on SIGCHLD to inhibit zombies.
 * SA_NODEFER prevents the current signal from being masked in the handler.
 *
 * SA_ONESHOT and SA_NOMASK are the historical Linux names for the Single
 * Unix names RESETHAND and NODEFER respectively.
 */
#define SA_NOCLDSTOP    0x00000001U
#define SA_NOCLDWAIT    0x00000002U
#define SA_SIGINFO      0x00000004U
#define SA_ONSTACK      0x08000000U
#define SA_RESTART      0x10000000U
#define SA_NODEFER      0x40000000U
#define SA_RESETHAND    0x80000000U

#define SA_NOMASK   SA_NODEFER
#define SA_ONESHOT  SA_RESETHAND

#define SA_RESTORER 0x04000000U

#endif /* SIGNAL_H_ */
