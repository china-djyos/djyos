/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"     /* FatFs lower layer API */
#if 0
#include "usbdisk.h"    /* Example: Header file of existing USB MSD control module */
#include "atadrive.h"   /* Example: Header file of existing ATA harddisk control module */
#include "sdcard.h"		/* Example: Header file of existing MMC/SDC contorl module */

/* Definitions of physical drive number for each drive */
#define ATA     0   /* Example: Map ATA harddisk to physical drive 0 */
#define MMC     1   /* Example: Map MMC/SD card to physical drive 1 */
#define USB     2   /* Example: Map USB MSD to physical drive 2 */
#else
#include "../../port/drivers/fat_drivers.h"
#include "ffconf.h"
extern struct FatDrvFuns g_tFatDrvs[_VOLUMES];
#endif


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
    BYTE pdrv       /* Physical drive nmuber to identify the drive */
)
{
#if 0
	DSTATUS stat = 0;
#endif
	int result;
#if 0
	switch (pdrv) {
	case ATA :
		result = ATA_disk_status();

        // translate the reslut code here

		return stat;

    case MMC :
        result = MMC_disk_status();

        // translate the reslut code here

        	return stat;

    case USB :
        result = USB_disk_status();

        // translate the reslut code here

		return stat;
    }
#else
    result = (g_tFatDrvs[pdrv].DrvStatus)();
    if(0 == result)
		return 0; // 成功
#endif
    return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
    BYTE pdrv               /* Physical drive nmuber to identify the drive(设备号) */
)
{
    DSTATUS stat = 0;
    int result;
#if 0
    switch (pdrv) {
    case ATA :
		result = ATA_disk_initialize();

        // translate the reslut code here

		return stat;

    case MMC :
        result = MMC_disk_initialize();

        // translate the reslut code here

            return stat;

    case USB :
        result = USB_disk_initialize();

		// translate the reslut code here

            return stat;
    }
#else
    result = (g_tFatDrvs[pdrv].DrvInitialize)();
	if(0 == result)
		return stat; // 成功
#endif
    return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
    BYTE pdrv,      /* Physical drive nmuber to identify the drive */
    BYTE *buff,     /* Data buffer to store read data */
    DWORD sector,   /* Sector address in LBA */
    UINT count      /* Number of sectors to read */
)
{
    DRESULT res = RES_OK;
    int result;
#if 0
    switch (pdrv) {
    case ATA :
        // translate the arguments here

		result = ATA_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case MMC :
		// translate the arguments here

		result = MMC_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case USB :
		// translate the arguments here

		result = USB_disk_read(buff, sector, count);

		// translate the reslut code here

			return res;
    }
#else
    result = (g_tFatDrvs[pdrv].DrvRead)(buff, sector, count);
	if(0 == result)
		return res; // 成功
#endif
    return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
    BYTE pdrv,          /* Physical drive nmuber to identify the drive */
    const BYTE *buff,   /* Data to be written */
    DWORD sector,       /* Sector address in LBA */
    UINT count          /* Number of sectors to write */
)
{
    DRESULT res = RES_OK;
    int result;
#if 0
    switch (pdrv) {
    case ATA :
        // translate the arguments here

		result = ATA_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;

	case MMC :
		// translate the arguments here

		result = MMC_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;

	case USB :
		// translate the arguments here

		result = USB_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;
    }
#else
    result = (g_tFatDrvs[pdrv].DrvWrite)((BYTE*)buff, sector, count);
	if(0 == result)
		return res; // 成功
#endif
    return RES_PARERR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
    BYTE pdrv,      /* Physical drive nmuber (0..) */
    BYTE cmd,       /* Control code */
    void *buff      /* Buffer to send/receive control data */
)
{
    DRESULT res = RES_OK;
    int result;
#if 0
    switch (pdrv) {
    case ATA :

		// Process of the command for the ATA drive

		return res;

	case MMC :

		// Process of the command for the MMC/SD card

		return res;

	case USB :

		// Process of the command the USB drive

		return res;
	}
#else
    result = (g_tFatDrvs[pdrv].DrvIoctl)(cmd, buff);
	if(0 == result)
		return res; // 成功
#endif
    return RES_PARERR;
}
#endif
