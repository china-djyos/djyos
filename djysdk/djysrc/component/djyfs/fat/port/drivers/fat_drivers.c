#include <string.h>
#include "../../ff11/src/ffconf.h"
#include "../../ff11/src/ff.h"
#include "fat_drivers.h"

//
//
//
struct FatDrvFuns g_tFatDrvs[_VOLUMES];
//
// ��ffconf.h�ж�Ӧ
//
PARTITION VolToPart[_VOLUMES] =
{ { 0, 0,}, // ��Ӧ"_VOLUME_STRS"�е�"RAM"
  { 3, 0,}, // ��Ӧ"_VOLUME_STRS"�е�"NAND"��"3"��ʾ�ײ��豸��
  { 0, 0,}, // ��Ӧ"_VOLUME_STRS"�е�"CF"
  { 1, 0,}, // ��Ӧ"_VOLUME_STRS"�е�"SD"��"1"��ʾ�ײ��豸��
  { 2, 0,}  // ��Ӧ"_VOLUME_STRS"�е�"usb msc"��"2"��ʾ�ײ��豸��
};
//-----------------------------------------------------------------------------
//����:
//����:
//���:
//����:
//-----------------------------------------------------------------------------
s32 FatDrvInitialize(u8 DrvIndex, struct FatDrvFuns *DrvFuns)
{
	if((DrvIndex >= _VOLUMES) || (NULL == DrvFuns))
		return (-1);

	g_tFatDrvs[DrvIndex].DrvInitialize = DrvFuns->DrvInitialize;
	g_tFatDrvs[DrvIndex].DrvIoctl = DrvFuns->DrvIoctl;
    g_tFatDrvs[DrvIndex].DrvRead = DrvFuns->DrvRead;
	g_tFatDrvs[DrvIndex].DrvStatus = DrvFuns->DrvStatus;
	g_tFatDrvs[DrvIndex].DrvWrite = DrvFuns->DrvWrite;

	return (0);
}

