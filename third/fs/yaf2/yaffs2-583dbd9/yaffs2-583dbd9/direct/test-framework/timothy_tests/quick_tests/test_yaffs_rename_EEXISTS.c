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
#include "test_yaffs_rename_EEXISTS.h"


int test_yaffs_rename_EEXISTS(void)
{
	int output=0;
	int error_code =0;
	if (yaffs_close(yaffs_open(FILE_PATH,O_CREAT | O_RDWR, FILE_MODE))==-1){
		print_message("failed to create file\n",1);
		return -1;
	}
	if (0 !=  yaffs_access(DIR_PATH,0)) {
		output = yaffs_mkdir(DIR_PATH,S_IWRITE | S_IREAD);
		if (output < 0) {
			print_message("failed to create directory\n",2);
			return -1;
		}
	}
	if (0 !=  yaffs_access(RENAME_DIR_PATH,0)) {
		output = yaffs_mkdir(RENAME_DIR_PATH,S_IWRITE | S_IREAD);
		if (output < 0) {
			error_code=yaffs_get_error();
			if (abs(error_code)!=EEXIST){
				print_message("failed to create second directory\n",2);
				return -1;
			}
		}
	}
	output= yaffs_open("/yaffs2/test_dir/dir2/file",O_CREAT | O_RDWR, FILE_MODE);
	if (output<0){
		print_message("failed to open file in the second directory\n",2);
		return -1;
	}
	output = yaffs_rename(DIR_PATH , RENAME_DIR_PATH);
	if (output<0){ 
		error_code=yaffs_get_error();
		if (abs(error_code)==ENOTEMPTY){
			return 1;
		} else {
			print_message("returned error does not match the the expected error\n",2);
			return -1;
		}
	} else{
		print_message("removed /yaffs2/ directory (which is a bad thing)\n",2);
		return -1;
	}	
}


int test_yaffs_rename_EEXISTS_clean(void)
{
	int output = 0;
	if (0 ==  yaffs_access("/yaffs2/dir2/file",0) ) {
		output = yaffs_unlink("/yaffs2/dir2/file");
		if (output < 0) {
			print_message("failed to remove the file\n",2);
			return -1;
		}
	}
	if (0 ==  yaffs_access(RENAME_PATH,0) && 0 != yaffs_access(DIR_PATH,0)) {
		output = yaffs_rename(RENAME_PATH,FILE_PATH);
		if (output < 0) {
			print_message("failed to remove the directory\n",2);
			return -1;
		}
	}
	
	return 1;

}
#endif // DJYOS������
