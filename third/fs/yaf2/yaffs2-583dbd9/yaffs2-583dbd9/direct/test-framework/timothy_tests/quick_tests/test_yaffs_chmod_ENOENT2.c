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
#include "test_yaffs_chmod_ENOENT2.h"

int test_yaffs_chmod_ENOENT2(void)
{
	int error=0;
	int output=yaffs_chmod("/non_existing_directory/foo",S_IREAD|S_IWRITE);

	if (output<0){
		error=yaffs_get_error();
		if (abs(error)==ENOENT){
			return 1;
		} else {
			print_message("different error than expected\n",2);
			return -1;
		}
	} else {
		print_message("chmoded the file (which is a bad thing)\n",2);
		return -1;
	}

}

int test_yaffs_chmod_ENOENT2_clean(void)
{
	return 1;
}
#endif // DJYOS������
