/*
 * YAFFS: Yet another FFS. A NAND-flash specific file system.
 *
 * Copyright (C) 2002-2011 Aleph One Ltd.
 *   for Toby Churchill Ltd and Brightstar Engineering
 *
 * Created by Timothy Manning <timothy@yaffs.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#if 0 // DJYOS������

#include "test_yaffs_freespace.h"



int test_yaffs_freespace(void)
{
	int output=0;
	output = yaffs_freespace(YAFFS_MOUNT_POINT);
	return output;
}


int test_yaffs_freespace_clean(void)
{
	return 1;
}

#endif // DJYOS������
