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
#include "test_yaffs_open_ELOOP_dir.h"


int test_yaffs_unlink_ELOOP_dir(void)
{
	int output=0;
	int error_code=0;

	if (set_up_ELOOP()<0){
		print_message("failed to setup symlinks\n",2);
		return -1;
	}

	output=yaffs_unlink(ELOOP_PATH "/file");
	if (output==-1){
		error_code=yaffs_get_error();
		if (abs(error_code)==ELOOP){
			return 1;
		} else {
			print_message("different error than expected\n",2);
			return -1;
		}
	} else {
		print_message("non existant directory opened.(which is a bad thing)\n",2);
		return -1;
	}


}
int test_yaffs_unlink_ELOOP_dir_clean(void)
{
	return 1;
}
#endif // DJYOS������
