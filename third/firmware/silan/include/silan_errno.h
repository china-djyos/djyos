/*
 * silan_errno.h
 */

#ifndef __SILAN_ERRNO_H__
#define __SILAN_ERRNO_H__

#include <errno.h>

enum {
	ENONE_S             =  0,  /* No Error */
	EERROR_S            =  1,  /* Default error */
	EINVAL_S            =  2,  /* Invalid argument */
	EFAULT_S            =  3,  /* Bad address */
	ENOMEM_S            =  4,  /* Out of memory */
	ENOSPC_S            =  5,  /* No space left on device */
	EIO_S               =  6,  /* I/O error */
	EFBIG_S             =  7,  /* File too large */
	EBUSY_S             =  8,  /* Resource or device busy */
	EAGAIN_S            =  9,  /* Try again */
	ENODEV_S            = 10,  /* No such device */
	EACCES_S            = 11,  /* Permission denied */
	ENOENT_S            = 12,  /* No such file or directory */
};



#endif  //__SILAN_ERRNO_H__
