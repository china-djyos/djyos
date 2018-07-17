/*
 * inet.h
 *
 *  Created on: 2015Äê9ÔÂ6ÈÕ
 *      Author: zqf
 */

#ifndef INET_H_
#define INET_H_

#include <sys/socket.h>
in_addr_t inet_addr(const char *addr);
char *inet_ntoa(struct in_addr addr);
int inet_aton(const char *string,struct in_addr *addr);
const char *inet_ntop(int af, const void *src, char *dst, socklen_t cnt);
int inet_pton(int af, const char *src, void *dst);

#endif /* INET_H_ */
