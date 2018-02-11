#include "os.h"
#include "string.h"
#include "IAP.h"
#include <cfg/iboot_config.h>
#include "stddef.h"
#include "cpu_peri.h"
#include "uartctrl.h"
#include <osboot.h>

extern const char *g_sUpdateAppName;
#define RW_SIZE						512



s32 APP_Update(void)
{
	DIR *dir;
	struct dirent *structDirent;
	char *path;
	char buffer[RW_SIZE];
	s32 res,Result;
	struct stat sourceInfo;
	u8 found = 0;
	s32 source = -1;
	s32 destination = -1;
	dir=opendir("/fat");
	if(!dir)
	{
		printf("IAP:directory \"fat\" is not exist! maybe \"FAT\" is not installed!\r\n");

		return (-1);
	}

	while((structDirent = readdir(dir)) != NULL)
	{
		if(0 == strcmp(structDirent->d_name, g_sUpdateAppName))
		{
			printf("IAP:file \"%s\" will be programmed.\r\n",
					structDirent->d_name);
			found = 1;
			break;
		}
	}
	if(!found)
	{
		printf("IAP: file \"%s\" is not found.\r\n", g_sUpdateAppName);
		Result=-1;
		goto END;
	}

	path = buffer;
	strcpy(path, "/fat/");
	strcpy(path+5, structDirent->d_name);
	source = open(path, O_RDONLY);

	if(-1 == source)
	{
		printf("IAP: cannot open source file \"%s\".\r\n", path);
		Result=-1;
		goto END;
	}

	res = fstat(source, &sourceInfo);
	if(res)
	{
		printf("IAP: cannot statistics source file \"%s\".\r\n", path);
		Result=-1;
		goto END;
	}

	strcpy(path, "/iboot/");
	strcpy(path+7, structDirent->d_name);
	destination = open(path, O_RDWR | O_CREAT);

	if(-1 == destination)
	{
		printf("IAP:cannot open destination file \"%s\"!\r\n", path);
		Result=-1;
		goto END;
	}

	while(1)
	{
		res = read(source, buffer, RW_SIZE);
		if(!res)
		{
			printf("IAP: read source file error.\r\n");
			break; // 没有数据可读
		}

		sourceInfo.st_size -= res; // 剩余

		if(res != write(destination, buffer, res))
		{
			printf("IAP:write destination file error.\r\n");
			break;
		}

		if(!sourceInfo.st_size)
		{
			printf("IAP:application update succeed.\r\n");
			Result=1;
			break; // 全部读完
		}
	}

	END:
		closedir(dir);
		if(-1 != source)
		{
			res = close(source);
			if(res)
			{
				printf("IAP:close source file failed.\r\n");
			}
		}

		if(-1 != destination)
		{
			res = close(destination);
			if(res)
			{
				printf("IAP:close destination file failed.\r\n");
			}
		}

		return (Result);


}
