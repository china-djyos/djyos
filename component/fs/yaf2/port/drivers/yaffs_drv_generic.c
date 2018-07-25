/*
 * YAFFS: Yet Another Flash File System. A NAND-flash specific file system.
 *
 * Copyright (C) 2002-2011 Aleph One Ltd.
 *   for Toby Churchill Ltd and Brightstar Engineering
 *
 * Created by Charles Manning <charles@aleph1.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/*
 * This is an interface module for handling NAND in yaffs2 mode.
 */

/* This code calls a driver that accesses "generic" NAND. In the simulator
 * this is direceted at a file-backed NAND simulator. The NAND access functions
 * should also work with real NAND.
 *
 * This driver is designed for use in yaffs2 mode with a 2k page size and
 * 64 bytes of spare.
 *
 * The spare ares is used as follows:
 * offset 0: 2 bytes bad block marker.
 * offset 2: 8x3 bytes of ECC over the data.
 * offset 26: rest available to store Yaffs tags etc.
 */

#include "../../port/drivers/yaffs_drv_generic.h"

//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
inline struct FlashChip *Dev2Chip(struct yaffs_dev *Dev)
{
    return ((struct FlashChip *)(Dev->driver_context));
}
//-----------------------------------------------------------------------------
//����: 
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
inline u8 *ChipBuf(struct yaffs_dev *Dev)
{
	u32 ChunkSize;
    struct FlashChip  *Chip = Dev2Chip(Dev);
    
	if(F_NAND == Chip->Type)
		ChunkSize = Chip->Descr.Nand.OOB_Size + Chip->Descr.Nand.BytesPerPage;
	else if(F_NOR == Chip->Type)
		ChunkSize = Chip->Descr.Nor.BytesPerPage;
	else
		return (NULL);
	
    memset (Chip->Buf, 0xFF, ChunkSize);
	
    return Chip->Buf;
}
//-----------------------------------------------------------------------------
//����: ���ҳ�ռ��Ƿ�ȫFF
//����:
//����: 1 -- ��; 0 -- ����;
//��ע:
//----------------------------------------------------------------------------
s32 IsErasedChunk(const u8 *Buf, s32 DataLen, s32 TagLen)
{
	extern int yaffs_check_ff(u8 *buffer, int n_bytes);

	if(DataLen && (!yaffs_check_ff((u8*)Buf, DataLen)))
        return (0);

    if(TagLen && (!yaffs_check_ff((u8*)(Buf+DataLen), TagLen)))
        return (0);

    return (1);
}
