/*
 * signal.c
 *
 *  Created on: 2015Äê9ÔÂ6ÈÕ
 *      Author: zqf
 */


#include <signal.h>


sighandler_t signal(int signum,sighandler_t handler)
{
	return SIG_IGN;
}

void alarm(int signum)
{
	return;
}

int sigemptyset(sigset_t *set)
{
	return unimpletment();
}
int sigfillset(sigset_t *set)
{
	return unimpletment();
}
int sigaddset(sigset_t *set, int signum)
{
	return unimpletment();
}
int sigdelset(sigset_t *set, int signum)
{
	return unimpletment();
}
int sigismember(const sigset_t *set, int signum)
{
	return unimpletment();
}
int sigaction( int sig, const struct sigaction *act,struct sigaction *oact )
{
	return unimpletment();
}
