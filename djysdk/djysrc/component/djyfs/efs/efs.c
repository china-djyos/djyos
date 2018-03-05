//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
//����ģ��:easyfile system
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:�ļ�ϵͳ��洢�����޹صĲ���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����:
//   ����: ����
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "efs.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <lock.h>
#include <systime.h>
#include "../djyfs/file.h"
#include <verify.h>
#include <endian.h>
#include <driver/flash/flash.h>


//#define FILE_INIT_MAX_SIZE        (0xFFFFFFFF) // ���û�����
#if 0 // ����������Ÿ��û�
#define FILE_FIXED_SIZE             (0x100000) // �̶���СΪ1M
#endif
#define CN_EFS_MAX                   1 // �����ļ�ϵͳ�ĸ���
#define CN_MAX_OPENED_FILE_NUM      (EFS_CFG_LIMIT/EFS_ITEM_LIMIT - 1) // ֧�ֵ��ļ��򿪵ĸ���
#define MUTEX_WAIT_TIME             CN_TIMEOUT_FOREVER//(2*1000*1000) // ������ʵȴ�ʱ��


tagEFS *s_pEfsList[CN_EFS_MAX]; // �����ļ�ϵͳ�б�
//static struct EfsFileInfo s_FileInfo[CN_MAX_OPENED_FILE_NUM];//���ļ�����Ϣ
volatile static u8 tgOpenedSum = 0; // �򿪵��ļ���
static char NameBuf[256];
static void NameECC(char *Name, u8 *Ecc);
static s32 ChkOrRecNameByECC(char *Name, u8 *Ecc);
static u32 dwFileMaxSize;

// ============================================================================
// ���ܣ���ȡ���û����õ�һЩ����
// ������
// ���أ�
// ��ע��
// ============================================================================
extern u32 FS_CONFIG_EFS_FILESIZE;
s32 __GetConfiguration(void)
{
    dwFileMaxSize = FS_CONFIG_EFS_FILESIZE;
}

//----����ļ������filesize-----------------------------------------------------------
//����: ���һ���ļ�������filesize��ֻ��飬�����κθ�д
//����: buf��������
//      location��filesize���ݵ�λ�ã����filesize�������ǳ����λ��
//����: 0 = �޴�1=�д�û�и���
//-----------------------------------------------------------------------------
static u32 __Efs_CheckFileSize(u8 *buf, u32 *location)
{
    u32 filesize_no, offset;
    u32 ret = 0;

    for (filesize_no = 0; filesize_no < FILE_FILESIZE_NUM;filesize_no++)
    {
        offset = filesize_no * 8;
        if(pick_little_64bit(buf + offset,0) == CN_LIMIT_UINT64)//Ϊȫ1��������޴�
            break;
        else if(pick_little_64bit(buf + offset,0) == 0)//��ʾ����д���Ĵ�С��Ϊȫ0,ֱ������
            continue;
        else if(pick_little_32bit(buf + offset,0) !=
                ~(pick_little_32bit(buf + offset + 4,0)))//�������д�
        {
            ret = 1;
            break;
        }
    }
    *location = filesize_no;
    return ret;
}
//----�������������-----------------------------------------------------------
//����: ���һ���ļ��������������������д��������֮,����д��оƬ�����޴����
//      ���ܸ�����ͬʱ�Ѹû������������ļ������ӵ�chip->files_sum��
//����: buf��������
//      index���ڼ�������
//      efs������оƬ
//����: 0 = �޴�1=�д�������2=�д��ܸ���
//-----------------------------------------------------------------------------
static u32 __Efs_CheckSingleBuf(u8 *buf, u32 index, struct EasyFS *efs)
{
    u32 offset, location, item_no,item_num;
    u32 temp_files_sum = 0;
    u32 ret = 0;

    item_num = (EFS_CFG_LIMIT > efs->block_size)?
            efs->block_size:EFS_CFG_LIMIT;
    item_num = item_num / EFS_ITEM_LIMIT;
    if(index == efs->start_block)
        item_no = 1;
    else
        item_no = 0;
    //һ�������ж����Ϣ�����ӵڶ� ����ʼ���,
    for (;item_no < item_num;item_no++)
    {
        offset = item_no * EFS_ITEM_LIMIT;
        if( (buf[offset] != 0xFF) && (buf[offset] != 0x00) )//nameΪ�ǿգ���ʾ��������
        {
            temp_files_sum++; //�����õ������ļ�����1
            offset += FILE_FILESIZE_OFF;
            if(__Efs_CheckFileSize(buf + offset,&location)) //�д���filesize��д��0
            {
                ret = 1;
                memset(buf+offset+8*location,0,8);
            }
        }
    }
    //�ѱ����������ļ����ŵ�chip->files_sum
    efs->files_sum += temp_files_sum;
    return ret;
}

//----�������-----------------------------------------------------------
//����: ���һ��(��+����)�ļ��������������������д��������֮,�������Ѿ���
//      flash�У�������д��ȥ��ͬʱ�Ѹû������������ļ������ӵ�chip->files_sum��
//����: buf��������
//      index���ڼ�������
//      efs������оƬ
//����: ��
//-----------------------------------------------------------------------------
static bool_t __Efs_CheckBlock(u8 *buf_main,u32 index_main, u8 *buf_bak,
                    u32 index_bak,struct EasyFS *efs)
{
    u32 item_no,offset,item_num;
    u32 reflash = 0, location;
    u32 temp_files_sum = 0;
    u8 * correct_filesize;

    item_num = (EFS_CFG_LIMIT > efs->block_size)?
            efs->block_size:EFS_CFG_LIMIT;
    item_num = item_num / EFS_ITEM_LIMIT;
    if(index_main == efs->start_block)
        item_no = 1;
    else
        item_no = 0;
    //�������ļ������ĵڶ�����ʼ���
    for (;item_no < item_num;item_no++)
    {
        offset = item_no * EFS_ITEM_LIMIT;
        if ((buf_main[offset] != 0xFF) || (buf_bak[offset] != 0xFF)) //�������ж��Ƿ����õ����ļ�����
        {
            if( (buf_main[offset] != 0x00) && (buf_bak[offset] != 0x00) ) //��Ϊ0�����ʾ���ļ���ɾ��
            {
                temp_files_sum++;
            }

            //������������name,startblock,maxsize,crc
            if(memcmp(buf_main+offset,buf_bak+offset,FILE_FILESIZE_OFF))//������ͬ
            {
                //����������,������������д�������ҪҪ����itemȫ������
                if(memcmp(buf_main + FILE_VERIFITY_OFF, "easyfile", 8))
                {
                    memcpy(buf_main+offset,buf_bak+offset,EFS_ITEM_LIMIT);
                }
                else    //���÷�������
                {
                    memcpy(buf_bak+offset,buf_main+offset,EFS_ITEM_LIMIT);
                }
                reflash = 1;
                continue;
            }

            //�˶������ļ�������filesize......todo:�˴������nandflash,�Ƿ�ֱ�Ӹĳ��д�������
            offset += FILE_FILESIZE_OFF;
            if(memcmp(buf_main+offset,buf_bak+offset,
                    FILE_VERIFITY_OFF - FILE_FILESIZE_OFF))//���������filesize��С��ͬ
            {
                reflash = 1;        //nandflash �д���ȥ������ͬʱ�����ÿ�
                if (__Efs_CheckFileSize(buf_main + offset, &location)) //��������д�
                {
                    correct_filesize = buf_bak+offset+8*location;
                    if(pick_little_64bit(correct_filesize,0) == CN_LIMIT_UINT64)//�������filesize���󣬵���bak��ûд
                    {
                        memset(correct_filesize, 0, 8);//�����������һ��
                        memset(buf_main + offset + 8 * location, 0, 8);
//                        efs->write_data(index_main,offset+8 * location,buf_main+offset+8*location,8);
//                        efs->write_data(index_bak, offset+8 * location,buf_bak+offset+8*location,8);
                    }
                    else if(location == FILE_FILESIZE_NUM)//����������һ��filesize
                    {
                        reflash = 1;
                        memset(buf_main+offset,0xff,FILE_VERIFITY_OFF-FILE_FILESIZE_OFF);
                        memcpy(buf_main+offset,correct_filesize,8);//������ȷfilesize����һ��λ��
                        memset(buf_bak+offset,0xff,FILE_VERIFITY_OFF-FILE_FILESIZE_OFF);
                        memcpy(buf_bak+offset,buf_main+offset,8);//������ȷfilesize����һ��λ��
                    }
                    else////�������filesize���󣬵���bak��д����bak�ļ���С��ȷ
                    {
                        //��дΪ0����filesizeд�����������Ӧ��λ��
                        memset(buf_main + offset + 8 * location, 0, 8);//�����������filesize������д��0
                        memcpy(buf_main + offset + 8 * location + 8,correct_filesize,8);//�ӱ��ݱ��п�����ȷ��filesize����һ��
                        memcpy(correct_filesize + 8,correct_filesize,8);
                        memset(correct_filesize, 0, 8);//�����������һ��

                        //�����µ�filesizeд��оƬ��
//                        efs->write_data(index_main,offset+8 * location,buf_main+offset+8*location,16);
//                        efs->write_data(index_bak, offset+8 * location,buf_bak+offset+8*location,16);
                    }
                }
                else//���������ȷ
                {
                    correct_filesize = buf_main+offset+8*location;
                    if(pick_little_64bit(correct_filesize,0) == CN_LIMIT_UINT64)//�������filesize���󣬵���bak��ûд
                    {
                        memset(correct_filesize, 0, 8);//�����������һ��
                        memset(buf_bak + offset + 8 * location, 0, 8);
//                        efs->write_data(index_main,offset+8 * location,buf_main+offset+8*location,8);
//                        efs->write_data(index_bak, offset+8 * location,buf_bak+offset+8*location,8);
                    }
                    else if(location == FILE_FILESIZE_NUM)//����������һ��filesize
                    {
                        reflash = 1;
                        memset(buf_bak+offset,0xff,FILE_VERIFITY_OFF-FILE_FILESIZE_OFF);
                        memcpy(buf_bak+offset,correct_filesize,8);//������ȷfilesize����һ��λ��
                        memset(buf_main+offset,0xff,FILE_VERIFITY_OFF-FILE_FILESIZE_OFF);
                        memcpy(buf_main+offset,buf_bak+offset,8);//������ȷfilesize����һ��λ��
                    }
                    else
                    {
                        //��bak��filesizeд��0
                        memset(buf_bak + offset + 8 * location, 0, 8);
                        memcpy(correct_filesize + 8,correct_filesize, 8); //���Ʋ�����
                        memcpy(buf_bak + offset + 8 * location + 8,correct_filesize, 8); //�������ļ�������filesize
                        memset(correct_filesize, 0, 8); //�뱸�÷�������һ��

                        //�����µ�filesizeд��оƬ��
//                        efs->write_data(index_main,offset+8 * location,buf_main+offset+8*location,16);
//                        efs->write_data(index_bak, offset+8 * location,buf_bak+offset+8*location,16);
                    }
                }
            }
        }
    }

    efs->files_sum += temp_files_sum;
    return reflash;
}
//-----------------------------------------------------------------------------
//����: �����޸������ļ����ļ���
//����:
//����:
//-----------------------------------------------------------------------------
static bool_t __Efs_CheckAllNameECC(u8 *FileInfoBuf)
{
    u32 offset,loop,fixedFiles;
    u8 *buf;

    fixedFiles = EFS_CFG_LIMIT/EFS_ITEM_LIMIT;
    for (loop = 1; loop < fixedFiles; loop++)
    {
        offset = loop * EFS_ITEM_LIMIT;
        buf = FileInfoBuf + offset;
        if( (buf[0] != 0xFF) && (buf[0] != 0x00) )
        {
            buf[FILENAME_LIMIT] = '\0';
            ChkOrRecNameByECC((char *)buf,buf+FILENAME_LIMIT+1);
        }
    }
    return true;
}
//----�޸��ļ��ߴ�-------------------------------------------------------------
//����: �޸��ļ���������ļ��ߴ粿�֡�1���������������һ������λ��д�롣2�����
//      �Ҳ�������������飬������filesize������ff��Ȼ����ļ��ߴ���µ���һ��
//      ��λ�ϣ������鲢д��֮��3����bak������ͬ�Ĳ�����
//����: fp�����������ļ�ָ��
//      newsize���µ��ļ��ߴ�
//����: ��
//-----------------------------------------------------------------------------
static void __Efs_ChangeFileSize(struct FileRsc *fp, u32 newsize)
{
    struct EfsFileInfo *fileinfo;
    struct EasyFS *efs;
    u8 *block_buf;
    u32 offset, blocksize, cfg_blocks, block_no, item_no,temp;
    u32 filesize_no,file_max_size;
    u16 crc16_check;

    fileinfo = (struct EfsFileInfo *) fp->private;
    efs = (struct EasyFS *) fp->efs;

    block_buf = efs->block_buf;
    blocksize = EFS_CFG_LIMIT;
    cfg_blocks = (EFS_CFG_LIMIT+ efs->block_size-1) / efs->block_size;

    //������ļ�����������ڵ�block
    block_no = (fileinfo->item * EFS_ITEM_LIMIT) / efs->block_size;
    block_no = block_no + efs->start_block;
    //���ļ�����������block�е�ƫ��
    offset = (fileinfo->item * EFS_ITEM_LIMIT) % efs->block_size;
    //������ļ���������block���ǵڼ����ļ�item
    item_no = offset / EFS_ITEM_LIMIT;

    //�������ļ������
    efs->read_data(block_no, offset,block_buf,EFS_ITEM_LIMIT,EF_WR_NOECC);

    ChkOrRecNameByECC((char *)(efs->block_buf),efs->block_buf+FILENAME_LIMIT+1);
    //�ļ��ߴ粻�ܴ����ļ����ߴ�
    file_max_size = pick_little_32bit(block_buf,FILE_MAXSIZE_OFF >> 2);

    if(newsize > file_max_size)
        return;

    offset = FILE_FILESIZE_OFF;
    for (filesize_no = 0; filesize_no < FILE_FILESIZE_NUM;filesize_no++)
    {
        if (pick_little_64bit(block_buf + offset,0) == CN_LIMIT_UINT64) //����������ҵ�����λ��
        {
            fill_little_32bit(block_buf, offset >> 2, newsize);
            fill_little_32bit(block_buf, (offset >> 2) + 1, ~newsize);
            temp = offset + item_no * EFS_ITEM_LIMIT;
            efs->write_data(block_no, temp, block_buf + offset, 8,EF_WR_NOECC);
            efs->write_data(block_no + cfg_blocks, temp, block_buf + offset, 8,EF_WR_NOECC);
            break;
        }
        offset += 8;
    }

    if (filesize_no == FILE_FILESIZE_NUM) //�������û�п���λ��
    {
        //�ȶ����������ݵ�buf,������
        efs->read_data(block_no, 0, block_buf,blocksize,EF_WR_NOECC);
        __Efs_CheckAllNameECC(efs->block_buf);

        efs->erase_block(block_no);
        efs->erase_block(block_no + cfg_blocks);

        offset = item_no * EFS_ITEM_LIMIT + FILE_FILESIZE_OFF;
        memset(block_buf + offset,0xFF,FILE_VERIFITY_OFF - FILE_FILESIZE_OFF);
        fill_little_32bit(block_buf, offset >> 2, newsize);
        fill_little_32bit(block_buf, (offset >> 2) + 1, ~newsize);

        crc16_check = crc16(efs->block_buf,16);
        fill_little_16bit((u8*)&temp,0,crc16_check);    //ת��ΪС��

        efs->write_data(block_no, 0, block_buf, blocksize,EF_WR_NOECC);
        efs->write_data(block_no + cfg_blocks, 0, block_buf, blocksize,EF_WR_NOECC);

        efs->write_data(efs->start_block + cfg_blocks - 1,
                efs->block_size-2, (u8*)&temp,2,EF_WR_NOECC);
        efs->write_data(block_no + 2*cfg_blocks - 1,
                efs->block_size-2, (u8*)&temp,2,EF_WR_NOECC);

    }
}

// =============================================================================
// ���ܣ������ļ�ϵͳ�����ƣ����ļ�ϵͳ�б��в����ļ�ϵͳ�Ľṹ��ָ��
// ������name,�ļ�ϵͳ��
// ���أ�NULL��δ�ҵ������򷵻�ָ��
// =============================================================================
static tagEFS* __Efs_Find(const char *name)
{
    u8 i;
    tagEFS* efs = NULL;

    for(i = 0; i <CN_EFS_MAX;i++)
    {
        if(!strcmp(s_pEfsList[i]->name,name))
        {
            efs = s_pEfsList[i];
            break;
        }
    }
    return efs;
}

// =============================================================================
// ���ܣ��½������ļ�����ʼ���ļ���ص���Ϣ���ļ���СΪ0�����ߴ�ΪĬ��
// ������filename,�ļ���
// ���أ�true,�����ɹ���false,����ʧ��
// =============================================================================
static bool_t __Efs_NewFile(tagFileRsc* fp,const char *filename,
                        tagEfsFileInfo *fileinfo)
{
    struct EasyFS *efs;

    u32 block_no,start_block;
    u32 offset;
    u8 cfg_blocks,item,item_num;
    u16 crc16_check,temp;

    if( (fp == NULL) || (strlen(filename) > FILENAME_LIMIT) )//�ļ���̫��
    {
        printf("Create New File Failed For Too Long Name!\r\n");
        return 0;
    }

    efs = fp->efs;
    if (efs->files_sum >= EFS_CFG_LIMIT / EFS_ITEM_LIMIT)
        return false; //�Ѵ�ɴ����ļ�������

    offset = 0;
    block_no = efs->start_block;
    //�����ļ�ϵͳ�ļ��������������ʹ�õ�blocks
    cfg_blocks = (EFS_CFG_LIMIT + efs->block_size-1) / efs->block_size;

    efs->read_data(block_no, offset, efs->block_buf,EFS_CFG_LIMIT,EF_WR_NOECC);

    __Efs_CheckAllNameECC(efs->block_buf);
    //����Ϊȫ0��ȫFF��λ��
    item_num = EFS_CFG_LIMIT/EFS_ITEM_LIMIT;
    for(item = 1; item < item_num; item++)  //todo
    {
        offset = item * EFS_ITEM_LIMIT;
        if(efs->block_buf[offset] == 0x00)  //��Ϊ0�����ʾ���ļ���ɾ��,�������½��ļ�
        {
            memset(efs->block_buf+offset,0xFF,EFS_ITEM_LIMIT);
            crc16_check = crc16(efs->block_buf,16);
            fill_little_16bit((u8*)&temp,0,crc16_check);    //ת��ΪС��
            //added,������CRCд���ļ�ϵͳ��Ϣ����������ֽ�
            efs->erase_block(block_no);
            efs->write_data(block_no,0,efs->block_buf,EFS_CFG_LIMIT,EF_WR_NOECC);
            efs->erase_block(block_no+cfg_blocks);
            efs->write_data(block_no+cfg_blocks,0,efs->block_buf,EFS_CFG_LIMIT,EF_WR_NOECC);

            efs->write_data(efs->start_block + cfg_blocks - 1,
                    efs->block_size-2, (u8*)&temp,2,EF_WR_NOECC);
            efs->write_data(block_no + 2*cfg_blocks - 1,
                    efs->block_size-2, (u8*)&temp,2,EF_WR_NOECC);
            break;
        }
        if(efs->block_buf[offset] == 0xFF) //�������ж��Ƿ����õ����ļ�����
        {
            break;
        }
    }

    if(item == EFS_CFG_LIMIT/EFS_ITEM_LIMIT)        //û���ҵ�����λ�û��ļ������ﵽ����
        return false;

    //�����½����ļ����ݴ洢λ��
    start_block = efs->start_block + 2*cfg_blocks +
            (item - 1)*dwFileMaxSize/efs->block_size;

    //ͬʱ��Ҫ�����ļ����ڵĿ�����
    block_no = dwFileMaxSize / efs->block_size;
    for(temp = 0; temp < block_no; temp++)
    {
        efs->erase_block(temp + start_block);
    }

    block_no = efs->start_block;
    //���³�ʼ���ļ������д��flash
    memset((char *)efs->block_buf, 0xff, EFS_ITEM_LIMIT);
    strncpy((char *)efs->block_buf, filename, FILENAME_LIMIT+1);
    efs->block_buf[FILENAME_LIMIT] = '\0';
    NameECC(filename,efs->block_buf + FILENAME_LIMIT+1);
    fill_little_32bit(efs->block_buf, FILE_STARTBLOCK_OFF >> 2,start_block);
    fill_little_32bit(efs->block_buf, FILE_MAXSIZE_OFF >> 2, dwFileMaxSize);
    fill_little_32bit(efs->block_buf, FILE_FILESIZE_OFF >> 2, 0);
    fill_little_32bit(efs->block_buf, (FILE_FILESIZE_OFF >> 2) + 1, ~0);
    memcpy(efs->block_buf + FILE_VERIFITY_OFF, "easyfile", 8);
    efs->write_data(block_no, offset, efs->block_buf,EFS_ITEM_LIMIT,EF_WR_NOECC);
    efs->write_data(block_no+cfg_blocks,offset,efs->block_buf,EFS_ITEM_LIMIT,EF_WR_NOECC);

    strncpy(fp->name,filename,FILENAME_LIMIT+1);
    //�����ļ���Ϣ����������
    fp->private = (ptu32_t)fileinfo;
    fp->file_size = 0;
    fp->ptr = 0;

    //�����ļ���Ϣ����������
    fileinfo->max_size = dwFileMaxSize;
    fileinfo->start_block = start_block;
    fileinfo->item = item;
    fileinfo->filesize = 0;

    return true;
}

// =============================================================================
// ���ܣ������ļ�ϵͳ���������������ļ�ϵͳ��У���У��
// ������efs,�ļ�ϵͳָ�룬δ��ʼ��
//      fs,�ļ�ϵͳ����
//      opt
// ���أ�true,�����ɹ���false,����ʧ��
// =============================================================================
FSRESULT Efs_Mount (tagEFS* efs, const char* fs,u8 opt)
{
    u32 loop, blocks, end_block, fileInfoSize;
    u8 *bakbuf, *block_buf;
    u8 mainblockerr = 0,bakblockerr = 0;
    u16 crc16_check,temp;
    FSRESULT result = FSR_OK;

    if(!efs->block_sum)                 //˵����chipδ��ʼ��
    {
        if(!Efs_Initlization(efs,fs))
           return FSR_NOT_READY;
    }

    bakbuf = M_MallocLc(EFS_CFG_LIMIT, 0);//ֻ���ļ���Ϣ��С
    if (bakbuf == NULL)
        return FSR_NOT_ENOUGH_CORE;

    blocks = (EFS_CFG_LIMIT + efs->block_size-1) / efs->block_size; // EFS��Ϣ�Ĵ�С
    end_block = efs->start_block + blocks; //
    fileInfoSize = EFS_CFG_LIMIT;
    block_buf = efs->block_buf;

   if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
   {
       result = FSR_DENIED;
       goto exit;
   }

    //����ļ�ϵͳ�Ƿ��Ѿ�����
    efs->read_data(efs->start_block, 0, block_buf, EFS_ITEM_LIMIT,EF_WR_NOECC);
    efs->read_data(end_block, 0, bakbuf, EFS_ITEM_LIMIT,EF_WR_NOECC);

    //�����������ƻ�������Ҫ���½����ļ�ϵͳ
    if ((memcmp(bakbuf, "easyfile", 8) ||
            memcmp(bakbuf + FILE_VERIFITY_OFF, "easyfile", 8)) &&
        (memcmp(block_buf, "easyfile", 8)||
            memcmp(block_buf + FILE_VERIFITY_OFF, "easyfile", 8)) )
    {
        printf("�ļ�ϵͳδ����!\r\n");
        result = FSR_NO_FILESYSTEM;
        Lock_MutexPost(efs->block_buf_mutex);
        goto exit;
    }

    //����ļ�ϵͳ��Ϣ��������ֽ�CRC�Ƿ���ȷ
    efs->read_data(efs->start_block + blocks - 1, efs->block_size-2,
            block_buf+fileInfoSize-2, 2,EF_WR_NOECC);
    efs->read_data(end_block + blocks - 1, efs->block_size-2,
            bakbuf+fileInfoSize-2, 2,EF_WR_NOECC);

    crc16_check = pick_little_16bit(block_buf,(fileInfoSize/2)-1);
    if(crc16_check != crc16(block_buf,16))
        mainblockerr = 1;

    crc16_check = pick_little_16bit(bakbuf,(fileInfoSize/2)-1);
    if(crc16_check != crc16(bakbuf,16)) //bakУ���
        bakblockerr = 1;

    //���¼��������ȷ��
    loop = efs->start_block;
    efs->read_data(loop, 0, block_buf, fileInfoSize,EF_WR_NOECC);
    efs->read_data(loop + blocks, 0, bakbuf, fileInfoSize,EF_WR_NOECC);

    switch((mainblockerr << 1) + (bakblockerr << 0))
    {
    case 0x03:      //����������У������������ļ�ϵͳ�Ѿ��𻵣�����
        result = FSR_NO_FILESYSTEM;
        break;
    case 0x02:      //mainbuf error, bakУ����ȷ
        temp = __Efs_CheckSingleBuf(bakbuf, loop, efs);
        if (temp == 0)
        {
            efs->erase_block(loop);
            efs->write_data(loop, 0, bakbuf, fileInfoSize,EF_WR_NOECC);

            crc16_check = crc16(bakbuf,16);
            fill_little_16bit((u8*)&temp,0,crc16_check);    //ת��ΪС��
            efs->write_data(loop + blocks - 1,
                    efs->block_size-2, (u8*)&temp,2,EF_WR_NOECC);
        }
        else if (temp == 1)
        {
            efs->erase_block(loop);
            efs->write_data(loop, 0, bakbuf, fileInfoSize,EF_WR_NOECC);
            efs->erase_block(loop + blocks);
            efs->write_data(loop + blocks, 0,
                    bakbuf, fileInfoSize,EF_WR_NOECC);

            crc16_check = crc16(bakbuf,16);
            fill_little_16bit((u8*)&temp,0,crc16_check);    //ת��ΪС��
            efs->write_data(loop + blocks - 1,
                    efs->block_size-2, (u8*)&temp,2,EF_WR_NOECC);

            efs->write_data(loop + 2*blocks - 1,
                    efs->block_size-2, (u8*)&temp,2,EF_WR_NOECC);
        }
        else
        {
            result = FSR_NO_FILESYSTEM;
        }
        break;
    case 0x01:          //��У����ȷ, bakbuf error
        temp = __Efs_CheckSingleBuf(block_buf, loop,efs);
        if (temp == 0)
        {
            efs->erase_block(loop + blocks);
            efs->write_data(loop + blocks, 0,
                    block_buf,fileInfoSize,EF_WR_NOECC);

            crc16_check = crc16(block_buf,16);
            fill_little_16bit((u8*)&temp,0,crc16_check);    //ת��ΪС��
            efs->write_data(loop + 2*blocks - 1,
                    efs->block_size-2, (u8*)&temp,2,EF_WR_NOECC);
        }
        else if (temp == 1)
        {
            efs->erase_block(loop);
            efs->write_data(loop, 0, block_buf, fileInfoSize,EF_WR_NOECC);
            efs->erase_block(loop + blocks);
            efs->write_data(loop + blocks, 0,
                    block_buf,fileInfoSize,EF_WR_NOECC);

            crc16_check = crc16(block_buf,16);
            fill_little_16bit((u8*)&temp,0,crc16_check);    //ת��ΪС��
            efs->write_data(loop + blocks - 1,
                    efs->block_size-2, (u8*)&temp,2,EF_WR_NOECC);

            efs->write_data(loop + 2*blocks - 1,
                    efs->block_size-2, (u8*)&temp,2,EF_WR_NOECC);
        }
        else
        {
            result = FSR_NO_FILESYSTEM;
        }
        break;
    case 0x00:          //main & bak all right
        if(true == __Efs_CheckBlock(block_buf,loop, bakbuf, loop+blocks, efs) )
        {
            //�������Ϣ���filesize������д��flash
            efs->erase_block(loop);
            efs->erase_block(loop+blocks);
            efs->write_data(loop, 0, block_buf, fileInfoSize,EF_WR_NOECC);
            efs->write_data(loop+blocks, 0,bakbuf, fileInfoSize,EF_WR_NOECC);

            crc16_check = crc16(block_buf,16);
            fill_little_16bit((u8*)&temp,0,crc16_check);    //ת��ΪС��
            efs->write_data(loop + blocks - 1,
                    efs->block_size-2, (u8*)&temp,2,EF_WR_NOECC);

            efs->write_data(loop + 2*blocks - 1,
                    efs->block_size-2, (u8*)&temp,2,EF_WR_NOECC);
        }
        break;
    default:
        result = FSR_NO_FILESYSTEM;
        break;
    }

    if(result == FSR_OK)
    {
        s_pEfsList[0] = efs;
        strncpy(s_pEfsList[0]->name,fs,EFS_NAME_LIMIT+1);
    }

    Lock_MutexPost(efs->block_buf_mutex);

exit:
    free(bakbuf);
    return result;
}

// =============================================================================
// ���ܣ������ļ�ϵͳ����mount���ɹ�ʱ������ã����ú�ԭ���ļ���Ϣ��������
// ������efs,�ļ�ϵͳָ��
//      path,����
//      au,
// ���أ�FSRESULT����ȷ������FSR_OK
// =============================================================================
FSRESULT Efs_Mkfs(tagEFS* efs,const char* path,u8 sfd,u32 au)
{
    u32 loop, blocks, end_block, fileInfoSize;
    u8 *block_buf;
    u16 crc16_check,temp;

    if(!efs->block_sum) // ˵����chipδ��ʼ��
    {
       if(!Efs_Initlization(efs,path))
           return FSR_NOT_READY;
    }

    blocks = (EFS_CFG_LIMIT + efs->block_size-1) / efs->block_size;
    end_block = efs->start_block + blocks;
    fileInfoSize = EFS_CFG_LIMIT;
    block_buf = efs->block_buf;

    if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
    {
       return FSR_DENIED;
    }

    memset(block_buf, 0xff, fileInfoSize);
    memcpy(block_buf, "easyfile", 8);
    memcpy(block_buf + FILE_VERIFITY_OFF, "easyfile", 8);
    fill_little_32bit(block_buf, 2, efs->block_sum);
    fill_little_32bit(block_buf, 3, efs->block_size);

    crc16_check = crc16(block_buf,16);
    fill_little_16bit((u8*)&temp,0,crc16_check); // ת��ΪС��
    for (loop = efs->start_block; loop < end_block; loop++)
    {
        efs->erase_block(loop);
        efs->erase_block(loop + blocks);
    }
    efs->write_data(efs->start_block, 0, block_buf,fileInfoSize,EF_WR_NOECC);
    efs->write_data(end_block, 0, block_buf, fileInfoSize,EF_WR_NOECC);
    //added,������CRCд���ļ�ϵͳ��Ϣ����������ֽ�
    efs->write_data(efs->start_block + blocks - 1,
            efs->block_size-2, (u8*)&temp,2,EF_WR_NOECC);
    efs->write_data(end_block + blocks - 1,
            efs->block_size-2, (u8*)&temp,2,EF_WR_NOECC);

    //ÿ���½�����fs��ŵ�ȫ�ֱ���tgNorDisk�����ں������
    s_pEfsList[0] = efs;
    strncpy(s_pEfsList[0]->name,path,EFS_NAME_LIMIT+1);
    efs->files_sum = 0;

    Lock_MutexPost(efs->block_buf_mutex);
    return FSR_OK;
}

// =============================================================================
// ���ܣ������ļ�ָ������������ĸ�Ŀ¼
// ������fp,�ļ�ָ��
//      fsname,�ļ�����
// ���أ�FSRESULT����ȷ������FSR_OK
// =============================================================================
FSRESULT Efs_FindFs(tagFileRsc* fp,char *fsname)
{
    fp->efs = __Efs_Find(fsname);
    if(NULL != fp->efs)
    {
        return FSR_OK;
    }
    return FSR_INVALID_PARAMETER;
}

// =============================================================================
// ���ܣ���ȡ�����ļ��ĳߴ�
// ������fname,��������fsname,�ļ���;filesize,�ļ��ߴ�ָ��
// ���أ�FSRESULT����ȷ������FSR_OK
// =============================================================================
FSRESULT Efs_Stat(const char *fname,const char *fsname,u32 *filesize)
{
    tagEFS *efs;
    u32 loop,block_no=0,block_offset = 0;
    u8 found = false,fixedFiles;
    u8 *hsize;
    FSRESULT ret = FSR_NO_FILE;

    efs = __Efs_Find(fsname);
    if(NULL == efs)
        return FSR_INVALID_PARAMETER;

    if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
    {
       return FSR_DENIED;
    }

    //�ж��Ƿ���ڸ��ļ�
    fixedFiles = EFS_CFG_LIMIT/EFS_ITEM_LIMIT;
    for (loop = 1; loop < fixedFiles; loop++)
    {
        block_no = loop * EFS_ITEM_LIMIT;
        block_offset = block_no % efs->block_size;
        block_no = efs->start_block + block_no / efs->block_size;
        efs->read_data(block_no, block_offset,efs->block_buf,
                                EFS_ITEM_LIMIT,EF_WR_NOECC);
        efs->block_buf[FILENAME_LIMIT] = '\0';
        ChkOrRecNameByECC((char *)(efs->block_buf),efs->block_buf+FILENAME_LIMIT+1);
        if (strncmp((const char*)efs->block_buf,fname,FILENAME_LIMIT) == 0)
        {
            found = true;
            break;
        }
    }

    if(found)       //δ�ҵ�ָ�����ļ�
    {
        for (loop = 0; loop < FILE_FILESIZE_NUM; loop++)
        {
            hsize = (efs->block_buf + FILE_FILESIZE_END - loop * 8);
            if ((pick_little_64bit(hsize,0) == CN_LIMIT_UINT64) ||
                (pick_little_64bit(hsize,0) == 0))
                continue;
            if (pick_little_32bit(hsize,0) == ~(pick_little_32bit(hsize + 4,0)))
            {
                *filesize = pick_little_32bit(efs->block_buf,
                        (FILE_FILESIZE_END - loop * 8) >> 2);
                break;
            }
        }
        ret = FSR_OK;
    }

    Lock_MutexPost(efs->block_buf_mutex);
    return ret;
}

// =============================================================================
// ���ܣ���Ŀ¼��������һ���ļ�����fname����һ���ļ�
// ������fname,��������fsname,�ļ���;FirstRdDirflg,�Ƿ�Ϊ��һ�ζ�Ŀ¼
// ���أ�FSRESULT����ȷ������FSR_OK
// =============================================================================
FSRESULT Efs_DirRead(const char *fname,const char *fsname,u32 *item)
{
    FSRESULT result = FSR_NO_FILE;
    tagEFS *efs;
    u32 loop = 1,block_no=0,block_offset = 0;
    u32 fixedFiles;

    efs = __Efs_Find(fsname);
    if(NULL == efs)
        return FSR_INVALID_PARAMETER;

    if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
    {
       return FSR_DENIED;
    }

    //�ж��Ƿ���ڸ��ļ�
    loop = *item;
    fixedFiles = EFS_CFG_LIMIT/EFS_ITEM_LIMIT;
    for(; loop < fixedFiles; loop++ )       //����fname��һ���ļ����һ���ļ�
    {
        block_no = loop * EFS_ITEM_LIMIT;
        block_offset = block_no % efs->block_size;
        block_no = efs->start_block + block_no / efs->block_size;
        efs->read_data(block_no, block_offset,
                efs->block_buf,FILENAME_LIMIT + 1 + ECC_LIMIT,EF_WR_NOECC);
        efs->block_buf[FILENAME_LIMIT] = '\0';
        ChkOrRecNameByECC((char *)(efs->block_buf),efs->block_buf+FILENAME_LIMIT+1);
        //������һ����Ч���ļ�
        if( (efs->block_buf[0] != 0x00) && (efs->block_buf[0] != 0xFF) )
        {
            memcpy(fname,efs->block_buf,FILENAME_LIMIT+1);
            result = FSR_OK;
            break;
        }
    }

    if(loop == fixedFiles)
        *item = 1;
    else
        *item = loop + 1;
    Lock_MutexPost(efs->block_buf_mutex);
    return result;
}

// =============================================================================
// ���ܣ����ļ������ļ������ڣ���ɸ���mode��ʽ�������Ƿ��½��ļ�
// ������fp,�ļ�ָ�룬δ��ʼ�����ļ�ָ��
//      name,�ļ�����
//      mode,
// ���أ�FSRESULT����ȷ������FSR_OK
// =============================================================================
FSRESULT Efs_Open(tagFileRsc* fp,const char* name,u8 mode)
{
    tagEFS* efs;
    struct EfsFileInfo *fileinfo;
    u32 loop, block_no, block_offset;
    u8 *hsize,fixedFiles,*buf;
    bool_t found = false;
    FSRESULT Ret;

    if ((fp == NULL) || (name == NULL))
        return FSR_INVALID_PARAMETER;

    if(tgOpenedSum >= CN_MAX_OPENED_FILE_NUM)
        return FSR_TOO_MANY_OPEN_FILES;

    efs = fp->efs;
    fileinfo = malloc(sizeof(tagEfsFileInfo));//&s_FileInfo[tgOpenedSum];
    buf = malloc(CN_FILE_BUF_LIMIT);

    if( (NULL == fileinfo) || (NULL == buf) || efs == NULL)
    {
        Ret = FSR_NOT_ENOUGH_CORE;
        goto exit;
    }

    memset(fileinfo,0xFF,sizeof(struct EfsFileInfo));
    memset(buf,0xFF,CN_FILE_BUF_LIMIT);

    if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
    {
       Ret = FSR_DENIED;
       goto exit;
    }

    //nand ���ļ�ɨ��̶��ļ���
    fixedFiles = EFS_CFG_LIMIT/EFS_ITEM_LIMIT;
    for (loop = 1; loop < fixedFiles; loop++)
    {
        block_no = loop * EFS_ITEM_LIMIT;
        block_offset = block_no % efs->block_size;
        block_no = efs->start_block + block_no / efs->block_size;
        efs->read_data(block_no, block_offset,
                efs->block_buf,FILENAME_LIMIT+1+ECC_LIMIT,EF_WR_NOECC);
        efs->block_buf[FILENAME_LIMIT] = '\0';
        ChkOrRecNameByECC((char *)(efs->block_buf),efs->block_buf+FILENAME_LIMIT+1);
        if (strncmp((const char*)efs->block_buf,name,FILENAME_LIMIT) == 0)
        {
            efs->read_data(block_no, block_offset,
                    efs->block_buf,EFS_ITEM_LIMIT,EF_WR_NOECC);
            found = true;
            fileinfo->item = loop;
            break;
        }
    }

    if (!found)             //δ�ҵ����ж��Ƿ����½��ļ�
    {
        if(mode & EF_OP_CREATE)
        {
            if(__Efs_NewFile(fp,name,fileinfo))
            {
                efs->files_sum++;
                fp->file_size = 0;
                Ret = FSR_OK;
            }
            else
            {
                Ret = FSR_EXIST;
            }
        }
        else
        {
            Ret = FSR_NO_FILE;
        }
    }
    else if(mode & EF_OP_OPEN)      //�˴� �ļ����ڣ��Ҵ�
    {
        //���������ļ���Դ����������struct file_rsc������Ҫ������ģ��ά���Ĳ��֡�
        for (loop = 0; loop < FILE_FILESIZE_NUM; loop++)
        {
            hsize = (efs->block_buf + FILE_FILESIZE_END - loop * 8);
            if ((pick_little_64bit(hsize,0) == CN_LIMIT_UINT64) ||
                (pick_little_64bit(hsize,0) == 0))
                continue;
            if (pick_little_32bit(hsize,0) == ~(pick_little_32bit(hsize + 4,0)))
            {
                fp->file_size = pick_little_32bit(efs->block_buf,
                        (FILE_FILESIZE_END - loop * 8) >> 2);
                break;
            }
        }

        fileinfo->start_block = pick_little_32bit(efs->block_buf,
                FILE_STARTBLOCK_OFF >> 2);
        fileinfo->max_size = pick_little_32bit(efs->block_buf,FILE_MAXSIZE_OFF >> 2);
        fileinfo->filesize = fp->file_size;
        strncpy(fp->name, name, FILENAME_LIMIT+1);
        Ret = FSR_OK;
    }
    else    //�ҵ��˸����Ƶ��ļ���ͬʱ���Ϊ�½������ش���
    {
        Ret = FSR_INVALID_NAME;
    }

    if(Ret == FSR_OK)
    {
        fp->wr_buf = buf;
        fp->buf_off = 0;
        if(mode & EF_OP_APPEND)
            fp->ptr = fileinfo->filesize;
        else
            fp->ptr = 0;
            
        fp->private = (ptu32_t)fileinfo;
        tgOpenedSum ++;
    }
    else
    {
exit:
        free(buf);
        free(fileinfo);
    }

    Lock_MutexPost(efs->block_buf_mutex);
    return Ret;
}

// =============================================================================
// ���ܣ��ر��ļ�,��Ҫ�������ͷ��ļ�ָ��Ŀռ�ʹ��ļ��ռ�
// ������fp,�ļ�ָ�룬δ��ʼ�����ļ�ָ��
// ���أ�FSRESULT����ȷ������FSR_OK
// =============================================================================
FSRESULT Efs_Close (tagFileRsc *fp)
{
    tagEfsFileInfo *fileinfo;
    struct EasyFS *efs;
    u32 block,offset;
    FSRESULT ret = FSR_OK;

    if(fp == NULL)
        return FSR_INVALID_PARAMETER;

    efs = fp->efs;
    fileinfo = (tagEfsFileInfo *)fp->private;

    if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
    {
       return FSR_DENIED;
    }
    //��wr_buf�����ݣ����Ƚ���д��flash������ECC
    if(fp->buf_off)
    {
        block = (fileinfo->filesize / efs->block_size) + fileinfo->start_block;
        offset = (fileinfo->filesize % efs->block_size) & (~0xFF); // 256���룬fp->buf_off���ܴ��ڻض����ݣ���filesize��һ����256�����

        if(!efs->query_block_ready(block, offset, fp->wr_buf, fp->buf_off))
        {
            ret = FSR_DENIED;
            goto exit;
        }

        __Efs_ChangeFileSize(fp, fp->file_size);
        //�˴�д���Сû��256��д��EF_WR_ECCֻ��Ϊ�˵ײ������һ��
        if(fp->buf_off !=
            efs->write_data(block,offset,fp->wr_buf,fp->buf_off,EF_WR_ECC))
        {
            __Efs_ChangeFileSize(fp, fileinfo->filesize);   //д��ʧ�ܣ������filesize
            ret = FSR_DISK_ERR;
            goto exit;
        }
    }

    free(fileinfo);
    free(fp->wr_buf);
    free(fp);//todo ---- ��Ӧ���ڴ˴��ͷţ���efs/port.c�����ͷţ���Ϊ������malloc
    tgOpenedSum --;
exit:
    Lock_MutexPost(efs->block_buf_mutex);
    return (ret);
}

// =============================================================================
// ���ܣ������������д��flash
// ������fp,�ļ�ָ��
// ���أ�FSRESULT����ȷ������FSR_OK
// =============================================================================
FSRESULT Efs_Flush (tagFileRsc *fp)
{
    tagEfsFileInfo *fileinfo;
    struct EasyFS *efs;
    u32 block,offset;
    FSRESULT ret = FSR_OK;

    if(fp == NULL)
        return FSR_INVALID_PARAMETER;

    efs = fp->efs;
    fileinfo = (tagEfsFileInfo *)fp->private;
    if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
    {
       return FSR_DENIED;
    }

    //��wr_buf�����ݣ����Ƚ���д��flash������ECC
    if(fp->buf_off)
    {
        block = (fileinfo->filesize / efs->block_size) + fileinfo->start_block;
        offset = fileinfo->filesize % efs->block_size;

        if(!efs->query_block_ready(block,offset,fp->wr_buf,fp->buf_off))
        {
            ret = FSR_DENIED;
            goto exit;
        }

        __Efs_ChangeFileSize(fp, fp->file_size);
        //�˴�д���Сû��256��д��EF_WR_ECCֻ��Ϊ�˵ײ������һ��
        if(fp->buf_off !=
            efs->write_data(block,offset,fp->wr_buf,fp->buf_off,EF_WR_ECC))
        {
            __Efs_ChangeFileSize(fp, fileinfo->filesize);   //д��ʧ�ܣ������filesize
            ret = FSR_DISK_ERR;
        }
        else
        {
            fileinfo->filesize = fp->ptr;
            fp->buf_off = 0;
        }
    }
exit:
    Lock_MutexPost(efs->block_buf_mutex);
    return ret;
}

// =============================================================================
// ���ܣ�ɾ���ļ�
// ������fname,�ļ���
//      fsname,�ļ�ϵͳ����
// ���أ�FSRESULT����ȷ������FSR_OK
// =============================================================================
FSRESULT Efs_Remove(const char *fname,const char *fsname)
{
    tagEFS *efs;
    u32 block_no,block_offset;
    u8 cfg_blocks,loop,fixedFiles;
    FSRESULT ret = FSR_NO_FILE;

    efs = __Efs_Find(fsname);
    if(NULL == efs)
    {
        printf("cannot fine root: \"%s\"", fsname);
        return FSR_NO_FILESYSTEM;
    }

    if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
    {
       return FSR_DENIED;
    }

    cfg_blocks = (EFS_CFG_LIMIT+ efs->block_size-1) / efs->block_size;

    fixedFiles = EFS_CFG_LIMIT/EFS_ITEM_LIMIT;
    for (loop = 1; loop < fixedFiles; loop++)
    {
        block_no = loop * EFS_ITEM_LIMIT;
        block_offset = block_no % efs->block_size;
        block_no = efs->start_block + block_no / efs->block_size;
        efs->read_data(block_no, block_offset,
                efs->block_buf,FILENAME_LIMIT+1+ECC_LIMIT,EF_WR_NOECC);
        efs->block_buf[FILENAME_LIMIT] = '\0';
        ChkOrRecNameByECC((char *)(efs->block_buf),efs->block_buf+FILENAME_LIMIT+1);
        if (strncmp((const char*)efs->block_buf,fname,FILENAME_LIMIT) == 0)
        {
            break;//�ҵ����ļ�
        }
    }
#if 1
if(loop == fixedFiles)
    printf("cannot find :%s\r\n", fname);
#endif

    if(loop < fixedFiles)
    {
        //ɾ�����ļ���Ϣ��дȫ0
        efs->read_data(block_no, block_offset, efs->block_buf,
                EFS_ITEM_LIMIT,EF_WR_NOECC);
        memset(efs->block_buf,0x0,EFS_ITEM_LIMIT);
        efs->write_data(block_no, block_offset, efs->block_buf,
                EFS_ITEM_LIMIT,EF_WR_NOECC);
        efs->write_data(block_no+cfg_blocks, block_offset,
                efs->block_buf,EFS_ITEM_LIMIT,EF_WR_NOECC);
        efs->files_sum --;
#if 1
{
        u16 CountTemp; 
        efs->read_data(block_no, block_offset, efs->block_buf,
                EFS_ITEM_LIMIT, EF_WR_NOECC);
        for(CountTemp = 0; CountTemp < EFS_ITEM_LIMIT; CountTemp++)
        {
            if(efs->block_buf[CountTemp] != 0x0)
            {
                printf("\r\nremove error in \"%s\"\r\n", fname);
                printf("remove error file in blk 0x%x offset 0x%x\r\n", block_no, block_offset);
                break;
            } 
        }
        efs->read_data(block_no+cfg_blocks, block_offset, efs->block_buf,
                EFS_ITEM_LIMIT, EF_WR_NOECC);
        for(CountTemp = 0; CountTemp < EFS_ITEM_LIMIT; CountTemp++)
        {
            if(efs->block_buf[CountTemp] != 0x0)
            {
                printf("\r\nremove error in \"%s\"\r\n", fname);
                printf("remove error file in blk 0x%x offset 0x%x\r\n", block_no+cfg_blocks, block_offset);
                break;
            }    
        }
}           
#endif

        ret = FSR_OK;
    }

    Lock_MutexPost(efs->block_buf_mutex);
    return ret;
}

// =============================================================================
// ���ܣ�seek�������������ܣ���EFS�У����ڸ��½����ļ����ú���ʵ���������ߴ磬��������
//      �ú���ʵ���ƶ���дָ�루����ʵ�ֶ�̬����Ĺ��ܣ�
// ������fp,�ļ�ָ��
//      newpos,�ļ���дָ���λ��
// ���أ�FSRESULT����ȷ������FSR_OK
// =============================================================================
FSRESULT Efs_Seek(tagFileRsc* fp,u32 newpos)
{
    tagEfsFileInfo *fileinfo;
    tagEFS *efs;
    u32 block,offset,alignsize,fsizeoff;
    FSRESULT ret = FSR_OK;

    efs = fp->efs;
    fileinfo = (tagEfsFileInfo *)fp->private;

    if(newpos > fileinfo->max_size)
        return FSR_DENIED;

    if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
    {
       return FSR_DENIED;
    }

    fsizeoff = fileinfo->filesize % CN_FILE_BUF_LEN;
    alignsize = fileinfo->filesize - fsizeoff;
    //���seek > CN_FILE_BUF_LEN,��wr_bufд��ȥ
    if( (newpos - alignsize >= CN_FILE_BUF_LEN) && (newpos > alignsize) )
    {
        block = (fileinfo->filesize / efs->block_size) + fileinfo->start_block;
        offset = fileinfo->filesize % efs->block_size;
        if(!efs->query_block_ready(block,offset,fp->wr_buf,CN_FILE_BUF_LEN))
        {
            ret =  FSR_DENIED;
            goto exit;
        }

        if(fsizeoff)        //�ж��Ƿ���Ҫ��������write
        {
            offset = alignsize % efs->block_size;
            memset(fp->wr_buf,0xFF,CN_FILE_BUF_LEN);
            efs->read_data(block,offset,fp->wr_buf,fsizeoff,EF_WR_NOECC);
        }

        __Efs_ChangeFileSize(fp, newpos - newpos % CN_FILE_BUF_LEN);//д��flash

        if(CN_FILE_BUF_LEN != efs->write_data(block,alignsize,fp->wr_buf,
                CN_FILE_BUF_LEN,EF_WR_ECC))
        {
            __Efs_ChangeFileSize(fp, fileinfo->filesize);
            ret = FSR_DISK_ERR;
        }
        else
        {
            fileinfo->filesize = newpos - newpos % CN_FILE_BUF_LEN;
            fp->file_size = newpos;
            fp->buf_off = 0;
            memset(fp->wr_buf,0xFF,CN_FILE_BUF_LEN);
        }
    }

    if(ret == FSR_OK)
    {
        fp->ptr = newpos;
    }
exit:
    Lock_MutexPost(efs->block_buf_mutex);
    return ret;
}

// =============================================================================
// ���ܣ�д�ļ�����������ļ������ߴ����ƣ��򷵻ش���
// ������fp,�ļ�ָ��
//      buf,��д������ݻ�����
//      len,д�����ݵ��ֽ���
//      bw,ʵ��д����ֽ�����ָ��
// ���أ�FSRESULT����ȷ������FSR_OK
// =============================================================================
FSRESULT Efs_Write (tagFileRsc* fp,const void *buf,u32 len,u32* bw)
{
    FSRESULT ret = FSR_OK;
    struct EfsFileInfo *fileinfo;
    struct EasyFS *efs;
    u32 block, offset_block;
    u32 write_sum, wr_point, write_len, completed = 0;
    u32 alignsize;

    *bw= 0;
    if ((fp == NULL) || (len == 0) || (buf == NULL))
        return FSR_INVALID_PARAMETER;

    if(fp->ptr < fp->file_size)
        return FSR_MKFS_ABORTED;

    //�ļ�����flashоƬָ��
    efs = fp->efs;
    fileinfo = (struct EfsFileInfo *) fp->private;
    //������������ߴ磬�򳬳����ֻᱻ����
    if (len <= (fileinfo->max_size - fp->ptr))
        write_sum = len;
    else
        write_sum = fileinfo->max_size - fp->ptr;
    wr_point = fp->ptr;

    //������ʼдλ�ô����ļ��ڵڼ����ƫ����
    block = (fileinfo->filesize / efs->block_size) + fileinfo->start_block;
    offset_block = fileinfo->filesize % efs->block_size;

    if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
    {
       return FSR_DENIED;
    }

    //�ж��Ƿ��д
    if(!efs->query_block_ready(block,offset_block,(u8*)buf,fp->buf_off + len))
    {
        ret = FSR_DENIED;
        goto exit;
    }

    memset(fp->wr_buf + fp->buf_off, 0xFF, (CN_FILE_BUF_LIMIT - fp->buf_off));
    if( fileinfo->filesize % CN_FILE_BUF_LEN )//���ȶ�����,ֻ��close,flush�����
    {
        fp->buf_off = fileinfo->filesize % CN_FILE_BUF_LEN;
        efs->read_data(block, (offset_block & (~0xFF)), fp->wr_buf, fp->buf_off, EF_WR_NOECC); // �ض����ݣ�Ϊ�˺���256д��ʱ��ECCУ��
    }

    alignsize = fileinfo->filesize - fileinfo->filesize % CN_FILE_BUF_LEN;
    if(wr_point + write_sum >= alignsize + CN_FILE_BUF_LEN )//��дfile_size
    {
        write_len = wr_point + write_sum;
        write_len = write_len - (write_len % CN_FILE_BUF_LEN);
        fileinfo->filesize = write_len;
        __Efs_ChangeFileSize(fp, write_len);    //��filesizeд��flash
    }

    while(1)
    {
        fp->buf_off = wr_point%CN_FILE_BUF_LEN;
        write_len = CN_FILE_BUF_LEN - fp->buf_off;

        if(write_sum < write_len)
        {
            write_len = write_sum;
            memcpy(fp->wr_buf + fp->buf_off,buf+completed,write_len);
            fp->buf_off += write_len;
            break;
        }
        else//write the buf to flash
        {
            memcpy(fp->wr_buf + fp->buf_off,buf+completed,write_len);
            block = (wr_point / efs->block_size) + fileinfo->start_block;
            offset_block = (wr_point % efs->block_size) - fp->buf_off;
            if(CN_FILE_BUF_LEN != efs->write_data(block,offset_block,
                    fp->wr_buf,CN_FILE_BUF_LEN,EF_WR_ECC))
            {
                write_len = 0;
                ret = FSR_DISK_ERR;
                break;
            }
            fp->buf_off = 0;
            memset(fp->wr_buf, 0xFF, CN_FILE_BUF_LIMIT); // ��ֹseek�����������һЩ����֪��ֵ��
        }
        wr_point += write_len;
        write_sum -= write_len;
        completed += write_len;
    }

    fp->ptr += completed + write_len;
    fp->file_size = fp->ptr;
    *bw = completed + write_len;

    if(ret != FSR_OK)
    {
        fileinfo->filesize = fp->ptr - fp->ptr % CN_FILE_BUF_LEN;
        __Efs_ChangeFileSize(fp, fileinfo->filesize);
    }
exit:
    Lock_MutexPost(efs->block_buf_mutex);
    return ret;
}

// =============================================================================
// ���ܣ��� �ļ�
// ������fp,�ļ�ָ��
//      buf,���ļ����ݴ洢�Ļ�����
//      len,���ļ��ֽ���
//      br,ʵ�ʶ�ȡ�����ݳ���
// ���أ�FSRESULT����ȷ������FSR_OK
// =============================================================================
FSRESULT Efs_Read (tagFileRsc* fp,void* buf,u32 len,u32* br)
{
    struct EfsFileInfo *fileinfo;
    struct EasyFS *efs;
    u32 completed = 0;
    FSRESULT ret = FSR_OK;

    u32 rd_point,buf_off,copy_len,block,blk_off,readlen;
    u8 rDataBuf[CN_FILE_BUF_LIMIT];

    *br = 0;
    if ((fp == NULL) || (len == 0) || (buf == NULL))
    {
        printf("\r\nEfs_Read - FSR_INVALID_PARAMETER\r\n");
        return FSR_INVALID_PARAMETER;
    }

    //�ļ�����flashоƬָ��
    efs = (struct EasyFS *)fp->efs;
    fileinfo = (struct EfsFileInfo *) fp->private;
    rd_point = fp->ptr;
    if(rd_point + len > fp->file_size)
        len = fp->file_size - rd_point;

    if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
    {
        printf("\r\nEfs_Read - FSR_DENIED- locked\r\n");
        return FSR_DENIED;
    }
    while(len > 0)
    {
        block = rd_point / efs->block_size + fileinfo->start_block;
        blk_off = (u32) (rd_point % efs->block_size);

        buf_off = blk_off % CN_FILE_BUF_LEN;
        if(buf_off)
        {
            blk_off -= buf_off;
        }
        if(rd_point >= fp->file_size - fp->file_size % CN_FILE_BUF_LEN)
        {
            //ֻ��close/flushʱ��ptr_buf��Ϊ0���Żᵼ��file_size������
//          eccflg = EF_WR_NOECC;
            readlen = fp->file_size - rd_point;
        }
        else
        {
//          eccflg = EF_WR_ECC;
            readlen = CN_FILE_BUF_LEN;
        }

        if(rd_point >= fileinfo->filesize)      //��fp->wr_buf��
        {
            memcpy(rDataBuf,fp->wr_buf, CN_FILE_BUF_LEN);
        }
        else
        {
            if(readlen != efs->read_data(block, blk_off,
                    rDataBuf, readlen,EF_WR_ECC))
            {
                printf("\r\nEfs_Read - FSR_DISK_ERR\r\n");
                ret = FSR_DISK_ERR ;
                break;  //�������ECC����
            }
        }

        copy_len = CN_FILE_BUF_LEN - buf_off;
        copy_len = (copy_len > len) ? len : copy_len;
        memcpy(buf + completed,&rDataBuf[buf_off],copy_len);

        completed += copy_len;
        rd_point  += copy_len;
        len       -= copy_len;
    }

    fp->ptr += completed;
    *br = completed;
    Lock_MutexPost(efs->block_buf_mutex);
    return ret;
}
// =============================================================================
// ���ܣ������ļ���ECC
// ������
// ���أ�
// =============================================================================
static void NameECC(char *Name, u8 *Ecc)
{
    memset(NameBuf, 0x00, 256);
    strcpy(NameBuf, Name);
    hamming_compute_256x((u8*)NameBuf, 256, Ecc);
}
// =============================================================================
// ���ܣ�ͨ��ECC�޸��ļ���
// ������
// ���أ�-1 -- ECCУ��ʧ��; 0 -- ECCУ��ɹ�;
// =============================================================================
static s32 ChkOrRecNameByECC(char *Name, u8 *Ecc)
{
    u32 Ret;

    if( ((Ecc[0] == 0) && (Ecc[1] == 0) && (Ecc[2] == 0)) ||
            ((Ecc[0] == 0xFF) && (Ecc[1] == 0xFF) && (Ecc[2] == 0xFF))    )
        return 0;

    memset(NameBuf, 0x00, 256);
    strcpy(NameBuf, Name);
    Ret = hamming_verify_256x((u8*)NameBuf, 256, Ecc);
    if (Ret && (Ret != HAMMING_ERROR_SINGLE_BIT))
    {
        printf("\r\nfile name \"%s\" ecc error [0x%x]\r\n", Name, Ret);
        return (-1);
    }
    return (0);
}
