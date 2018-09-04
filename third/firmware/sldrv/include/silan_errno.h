/*
 * silan_errno.h
 */

#ifndef __SILAN_ERRNO_H__
#define __SILAN_ERRNO_H__

enum {
	SLENONE             =  0,  /* No Error */
	SLEERROR            =  1,  /* Default error */
	SLEINVAL            =  2,  /* Invalid argument */
	SLEFAULT            =  3,  /* Bad address */
	SLENOMEM            =  4,  /* Out of memory */
	SLENOSPC            =  5,  /* No space left on device */
	SLEIO               =  6,  /* I/O error */
	SLEFBIG             =  7,  /* File too large */
	SLEBUSY             =  8,  /* Resource or device busy */
	SLEAGAIN            =  9,  /* Try again */
	SLENODEV            = 10,  /* No such device */
	SLEACCES            = 11,  /* Permission denied */
	SLENOENT            = 12,  /* No such file or directory */
};

#endif  //__SILAN_ERRNO_H__
