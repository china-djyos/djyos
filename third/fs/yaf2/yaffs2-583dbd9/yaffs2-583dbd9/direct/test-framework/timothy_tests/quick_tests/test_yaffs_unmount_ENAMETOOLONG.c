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
#include "test_yaffs_unmount_ENAMETOOLONG.h"

int test_yaffs_unmount_ENAMETOOLONG(void)
{
	int output=0;
	int x=0;
	int error_code=0;
	int file_name_length=1000000;
	char file_name[file_name_length];

	for (x=0; x<file_name_length -1; x++){
		file_name[x]='a';
	}
	file_name[file_name_length-2]='\0';
	
	output=yaffs_unmount(file_name);

	if (output<0){
		error_code=yaffs_get_error();
		if (abs(error_code)== ENAMETOOLONG){
			return 1;
		} else {
			print_message("different error than expected\n",2);
			return -1;
		}
	} else {
		print_message("mounted a too long mount point name.(which is a bad thing)\n",2);
		return -1;
	}
}

int test_yaffs_unmount_ENAMETOOLONG_clean(void)
{
	return 1;
}
#endif // DJYOS������
