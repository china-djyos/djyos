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
#include "test_yaffs_truncate_ENOENT.h"


int test_yaffs_truncate_ENOENT(void)
{
	int error=0;
	int output=0;


	output= yaffs_truncate("/yaffs2/non_existing_file",FILE_SIZE_TRUNCATED );
	if (output<0){
		error=yaffs_get_error();
		if (abs(error)==ENOENT){
			return 1;
		} else {
			print_message("received a different error than expected\n",2);
			return -1;
		}
	} else{
		print_message("truncated a nonexisting file\n",2);
		return -1;
	}		

}

int test_yaffs_truncate_ENOENT_clean(void)
{
	return 1;
}
#endif // DJYOS������
