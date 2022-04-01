// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
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


#include <stdio.h>
#include <dirent.h>
#include <djyfs/filesystems.h>
#include <stdlib.h>
#include <device/unit_media.h>
#include <string.h>
#include <endian.h>
#include <misc/ecc_256.h>
#include <misc/crc.h>
#include <lock.h>
#include <djyfs/efs.h>
#include <dbug.h>
#include <Object.h>
#include <systime.h>

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern s32 ModuleInstall_EFS(const char *target, u32 opt, u32 config);
//    ModuleInstall_EFS(CFG_EFS_MOUNT_POINT, CFG_EFS_INSTALL_OPTION, 0);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ+
//component name:"easy file system"//easy�ļ�ϵͳ
//parent:"file system"//��д������ĸ�������֣�none��ʾû�и����
//attribute:system                    //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                    //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                      //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                    //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                      //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"file system"//������������������������none����ʾ�������������
                                      //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                      //����������������������г����á�,���ָ�
//weakdependence:"none"               //��������������������������none����ʾ�������������
                                      //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                      //����������������������г����á�,���ָ�
//mutex:"none"                        //������Ļ����������������none����ʾ�޻����������
                                      //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_EASY_FILE_SYSTEM == false )
//#warning  " easy_file_system  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_EASY_FILE_SYSTEM    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,1073741823,
#define CFG_EFS_FILE_SIZE_LIMIT           4096       //"�����ļ���С������"
#define CFG_EFS_MAX_CREATE_FILE_NUM       50         //"����������ļ���"
#define CFG_EFS_MAX_OPEN_FILE_NUM         10         //"����ͬʱ������ļ���"
//%$#@string,1,10,
#define CFG_EFS_MOUNT_POINT               "efs"      //"EFS�ļ�ϵͳ��װĿ¼"
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#define CFG_EFS_INSTALL_OPTION            MS_INSTALLCREAT      //"��װѡ��"��MS_INSTALLFORMAT,MS_INSTALLCREAT,MS_INSTALLUSE��һ�������ġ���
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure

#define MUTEX_WAIT_TIME     CN_TIMEOUT_FOREVER  //(2*1000*1000) // ������ʵȴ�ʱ��

s32 e_operations(void *opsTarget, u32 objcmd, ptu32_t OpsArgs1,
                 ptu32_t OpsArgs2, ptu32_t OpsArgs3);
struct EasyFS *s_pEfsList; // �����ļ�ϵͳ
static u8 tgOpenedSum = 0; // �򿪵��ļ���
static char NameBuf[256];
static void NameECC(char *Name, u8 *Ecc);
static s32 ChkOrRecNameByECC(char *Name, u8 *Ecc);
static s32 Efs_Verify_Install(struct FsCore *pSuper);
static u32 dwFileMaxSize;   //�ļ��ߴ�����
static u32 EfsCgfLimit;     //���ڻ���򿪵��ļ�����Ϣ����洢�ߴ�
static u32 IndexesNum;      //�ļ����������=�ȿɴ����ļ���+1
static u32 CreateMax;       //�ɴ������ļ���
static u32 FileInfoList;    //�ļ�����������ڴ�ռ�=���ɴ����ļ���+1��*256
static u32 Ram_file_info_len;   //�ڴ��б��浥���ļ���Ϣ����ռ䣬4+256
// =============================================================================
// ���ܣ�ͨ��ECC�޸��ļ��������ECC=ffffff�����ʾ����ecc�����ء���ȷ��
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
    Ret = ECC_HammingVerify256x((u8*)NameBuf, 256, Ecc);      //У���ļ���
    if (Ret && (Ret != HAMMING_ERROR_SINGLE_BIT))
    {
        printf("\r\nfile name \"%s\" ecc error [0x%x]\r\n", Name, Ret);
        return (-1);
    }
    return (0);
}
// =============================================================================
// ���ܣ������ļ���ECC
// ������Name -- �ļ����� Ecc -- ��eccֵ�ĵ�ַ
// ���أ�
// =============================================================================
static void NameECC(char *Name, u8 *Ecc)
{
    memset(NameBuf, 0x00, 256);
    strcpy(NameBuf, Name);
    ECC_HammingCompute256x((u8*)NameBuf, 256, Ecc);
}
//------------------------------------------------------------------------------
//����: ������У��λ
//����: odd -- �ļ��ߴ�����λ��ż��λ��������ֵ
//����: ���1�ĸ���Ϊ���������򷵻�0�����1�ĸ���Ϊż�������򷵻�1��
//-----------------------------------------------------------------------------
static u32 __Efs_paritycheck(u32 odd)
{
    u32 val=1;
    u32 flag=1;
    while(odd)
    {
        val ^= (odd & flag);
        odd >>= 1;
    }
    return val & 0x1;
}
//------------------------------------------------------------------------------
//����: �����ļ���С����żУ��,����λ��ż��λ����У��ֱ���ڵ�30λ�͵�31λ
//����: newsize -- �ļ���С
//����: ���ؼ�������żУ����ļ���С
//-----------------------------------------------------------------------------
static u32 __EfsFileSize_odd_even_check(u32 newsize)
{
    u32 ret = newsize,odd,even;

    if(ret > EFS_SUPPORT_MAXFILESIZE)
    {
        ret = EFS_SUPPORT_MAXFILESIZE;
        warning_printf("efs","The file size exceeds the maximum file size supported by efs ");
    }
    odd = ret & 0x15555555;     //��ȡ��������λ
    odd = __Efs_paritycheck(odd);   //��ȡ����ż����λ
    even = ret & 0x2aaaaaaa;
    even = __Efs_paritycheck(even);   //��ȡ����ż����λ
    ret = (ret | (odd << 30) | (even << 31));
    return ret;
}

//------------------------------------------------------------------------------
//����: У���ļ���С��У��λ�Ƿ�ƥ��
//����: filesize -- �ļ���С
//����: 0 -- ƥ�� �� -1 -- ��ƥ��
//-----------------------------------------------------------------------------
static s32 __EfsFileSize_verify(u32 filesize)
{
    u32 ret = filesize,odd,even;

    odd = ((ret >> 30) & 0x1);
    even = ((ret >> 31) & 0x1);

    if((odd != __Efs_paritycheck(ret & 0x15555555)) || (even != __Efs_paritycheck(ret & 0x2aaaaaaa)))
        return -1;

    return 0;
}

//----����ļ������filesize-----------------------------------------------------------
//����: ���һ���ļ�������filesize��ֻ��飬�����κθ�д
//����: buf���������������ļ���26����С
//      location��filesize���ݵ�λ�ã����filesize�������ǳ����λ��
//����: 0 = �޴�1=�д�û�и���
//-----------------------------------------------------------------------------
static u32 __Efs_CheckFileSize(u8 *buf, u32 *location)
{
    u32 filesize_no, offset;
    u32 ret = 0;

    for (filesize_no = 0; filesize_no < FILE_FILESIZE_NUM;filesize_no++)
    {
        offset = filesize_no * EFS_FILESIZE_BYTE_NUMBER;
        if(pick_little_32bit(buf + offset,0) == CN_LIMIT_UINT32)//Ϊȫ1��������޴�
            break;
        else if(pick_little_32bit(buf + offset,0) == 0)//��ʾ����д���Ĵ�С��Ϊȫ0,ֱ������
            continue;
        else if(__EfsFileSize_verify(*(u32 *) (buf + offset)))//��żУ���д�
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
//      efs���ļ�ϵͳ��������
//����: 0 = �޴�1=�д�������2=�д��ܸ���
//-----------------------------------------------------------------------------
static u32 __Efs_CheckSingleBuf(u8 *buf, u32 index, struct EasyFS *efs)
{
    u32 offset, location, item_no,item_num;
    u32 temp_files_sum = 0;
    u32 ret = 0;

    item_num = (FileInfoList > efs->block_size)?
            efs->block_size:FileInfoList;
    item_num = item_num / EFS_ITEM_LIMIT;       //һ�����д��˶����ļ�����
    if(index == efs->start_block)
        item_no = 1;            //�����efs�ļ�ϵͳ����ʼ�飬��ӵڶ�����ʼ��飬��һ��������ļ�ϵͳ��Ϣ
    else
        item_no = 0;

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
                memset(buf + offset + EFS_FILESIZE_BYTE_NUMBER * location, 0, EFS_FILESIZE_BYTE_NUMBER);   //����ļ���С������Ѵ���Ĵ�С�����ȫд0
            }
        }
    }
    //�ѱ����������ļ����ŵ�efs->files_sum
    efs->files_sum += temp_files_sum;
    return ret;
}

//----�������-----------------------------------------------------------
//����: ���һ��(��+����)�ļ��������������������д��������֮,�������Ѿ���
//      flash�У�������д��ȥ��ͬʱ�Ѹû������������ļ������ӵ�chip->files_sum��
//����: buf_main -- ���ļ������������
//      index_main -- ���ļ��������ʼ���
//      buf_bakindex -- �����ļ������������
//      index_bak -- �����ļ��������ʼ���
//      efs���ļ�ϵͳ��������
//����: 0 = ������ͬ��1=�����в��
//-----------------------------------------------------------------------------
static bool_t __Efs_CheckBlock(u8 *buf_main,u32 index_main, u8 *buf_bak,
                    u32 index_bak, struct EasyFS *efs)
{
    u32 item_no,offset,item_num;
    u32 reflash = 0, location;
    u32 temp_files_sum = 0;
    u8 * correct_filesize;
    item_num = (FileInfoList > efs->block_size)?
            efs->block_size:FileInfoList;
    item_num = item_num / EFS_ITEM_LIMIT;       //�ļ���Ŀ�����д��˶����ļ�����
    if(index_main == efs->start_block)
        item_no = 1;        //�����efs�ļ�ϵͳ����ʼ�飬��ӵڶ�����ʼ��飬��һ��������ļ�ϵͳ��Ϣ
    else
        item_no = 0;
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
                    correct_filesize = buf_bak + offset + EFS_FILESIZE_BYTE_NUMBER * location;
                    if(pick_little_32bit(correct_filesize,0) == CN_LIMIT_UINT32)//�������filesize���󣬵���bak��ûд
                    {
                        memset(correct_filesize, 0, EFS_FILESIZE_BYTE_NUMBER);//�����������һ��
                        memset(buf_main + offset + EFS_FILESIZE_BYTE_NUMBER * location, 0, EFS_FILESIZE_BYTE_NUMBER);
                    }
                    else if(location == FILE_FILESIZE_NUM)//����������һ��filesize
                    {
                        reflash = 1;
                        memset(buf_main+offset,0xff,FILE_VERIFITY_OFF-FILE_FILESIZE_OFF);
                        memcpy(buf_main+offset,correct_filesize,EFS_FILESIZE_BYTE_NUMBER);//������ȷfilesize����һ��λ��
                        memset(buf_bak+offset,0xff,FILE_VERIFITY_OFF-FILE_FILESIZE_OFF);
                        memcpy(buf_bak+offset,buf_main+offset,EFS_FILESIZE_BYTE_NUMBER);//������ȷfilesize����һ��λ��
                    }
                    else////�������filesize���󣬵���bak��д����bak�ļ���С��ȷ
                    {
                        //��дΪ0����filesizeд�����������Ӧ��λ��
                        memset(buf_main + offset + EFS_FILESIZE_BYTE_NUMBER * location, 0, EFS_FILESIZE_BYTE_NUMBER);//�����������filesize������д��0
                        memcpy(buf_main + offset + EFS_FILESIZE_BYTE_NUMBER * location + EFS_FILESIZE_BYTE_NUMBER,
                                                        correct_filesize,EFS_FILESIZE_BYTE_NUMBER);//�ӱ��ݱ��п�����ȷ��filesize����һ��
                        memcpy(correct_filesize + EFS_FILESIZE_BYTE_NUMBER,correct_filesize,EFS_FILESIZE_BYTE_NUMBER);
                        memset(correct_filesize, 0, EFS_FILESIZE_BYTE_NUMBER);//�����������һ��
                    }
                }
                else//���������ȷ
                {
                    correct_filesize = buf_main + offset + EFS_FILESIZE_BYTE_NUMBER * location;
                    if(pick_little_32bit(correct_filesize,0) == CN_LIMIT_UINT32)//�������filesize���󣬵���bak��ûд
                    {
                        memset(correct_filesize, 0, EFS_FILESIZE_BYTE_NUMBER);//�����������һ��
                        memset(buf_bak + offset + EFS_FILESIZE_BYTE_NUMBER * location, 0, EFS_FILESIZE_BYTE_NUMBER);
                    }
                    else if(location == FILE_FILESIZE_NUM)//����������һ��filesize
                    {
                        reflash = 1;
                        memset(buf_bak+offset, 0xff, FILE_VERIFITY_OFF - FILE_FILESIZE_OFF);
                        memcpy(buf_bak+offset, correct_filesize, EFS_FILESIZE_BYTE_NUMBER);//������ȷfilesize����һ��λ��
                        memset(buf_main+offset, 0xff, FILE_VERIFITY_OFF - FILE_FILESIZE_OFF);
                        memcpy(buf_main+offset, buf_bak + offset, EFS_FILESIZE_BYTE_NUMBER);//������ȷfilesize����һ��λ��
                    }
                    else
                    {
                        //��bak��filesizeд��0
                        memset(buf_bak + offset + EFS_FILESIZE_BYTE_NUMBER * location, 0, EFS_FILESIZE_BYTE_NUMBER);
                        memcpy(correct_filesize + EFS_FILESIZE_BYTE_NUMBER, correct_filesize, EFS_FILESIZE_BYTE_NUMBER); //���Ʋ�����
                        memcpy(buf_bak + offset + EFS_FILESIZE_BYTE_NUMBER * location + EFS_FILESIZE_BYTE_NUMBER,
                                                        correct_filesize, EFS_FILESIZE_BYTE_NUMBER); //�������ļ�������filesize
                        memset(correct_filesize, 0, EFS_FILESIZE_BYTE_NUMBER); //�뱸�÷�������һ��
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
//����: FileInfoBuf -- �����ļ���Ϣ�Ļ�����
//����:
//-----------------------------------------------------------------------------
static bool_t __Efs_CheckAllNameECC(u8 *FileInfoBuf)
{
    u32 offset, loop;
    u8 *buf;

    for (loop = 1; loop < IndexesNum; loop++)
    {
        offset = loop * EFS_ITEM_LIMIT;
        buf = FileInfoBuf + offset;
        if( (buf[0] != 0xFF) && (buf[0] != 0x00) )
        {
            buf[FILENAME_LIMIT] = '\0';
            ChkOrRecNameByECC((char *)buf,buf+FILENAME_LIMIT+1);  //ͨ��ECC�����޸��ļ���
        }
    }
    return true;
}
//-----------------------------------------------------------------------------
//����: ����һ��flash���е��ļ���С�ռ䣬�ļ���Ϣ�����44���ļ���С�����һ�������õģ��ú���������
//���ǰ�һ�����У����е��ļ���Ϣ�е��ļ���С����һ��������ĳ���ļ���Ϣ����10���ļ���С��ֻ�����һ������Ч��
//���ǰ9���ļ���Сɾ����ֻ������Ч���ļ���С��
//����: core -- efs�ļ�ϵͳ����file_info_bufĳ�����е�����
//����:
//-----------------------------------------------------------------------------
static void __Efs_Reset_File_Size(struct EasyFS *efs, u8 *file_info_buf)
{
    u8 *hsize, *buf;
    u32 loop, item, block_info_num, file_size = 0;
    if(FileInfoList > efs->block_size)
        block_info_num = efs->block_size / EFS_ITEM_LIMIT;
    else
        block_info_num = FileInfoList / EFS_ITEM_LIMIT;

    for (loop = 0; loop < block_info_num; loop++)
    {
        buf = file_info_buf + (loop * EFS_ITEM_LIMIT);
        if (memcmp(buf, "easyfile", 8) || memcmp(buf + FILE_VERIFITY_OFF, "easyfile", 8))
        {
            if((*buf != 0x00) && (*buf != 0xFF))
            {
                for (item = 0; item < FILE_FILESIZE_NUM; item++)
                {
                    hsize = (buf + FILE_FILESIZE_END - item * EFS_FILESIZE_BYTE_NUMBER);
                    if ((pick_little_32bit(hsize,0) == CN_LIMIT_UINT32) ||
                        (pick_little_32bit(hsize,0) == 0))
                        continue;
                    if (__EfsFileSize_verify(*(u32 *)hsize) == 0)
                    {   //��ȡĿ���ļ��Ĵ�С
                        file_size = (pick_little_32bit(buf,     //��44���ļ���С�У�����ȷ���ļ���С�������ʼ��
                                (FILE_FILESIZE_END - item * EFS_FILESIZE_BYTE_NUMBER) >> 2));
                        break;
                    }
                }
                if(item != 43)
                {
                    memset(buf + FILE_FILESIZE_OFF, 0xFF, FILE_VERIFITY_OFF - FILE_FILESIZE_OFF);
//                    file_size = __EfsFileSize_odd_even_check(file_size); //����У�����ļ���С
                    fill_little_32bit(buf, FILE_FILESIZE_OFF >> 2, file_size);
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------
//����: ˢ���ڴ�����ļ��б�
//����: efs -- efs�ļ�ϵͳ����
//����:
//-----------------------------------------------------------------------------
static bool_t __Efs_RefreshFileList(struct EasyFS *efs)
{
    char *file_name;
    u8 *file_info_buf;
    u32 i, filesize_no;

    if(efs == NULL)
        return false;

    file_info_buf = malloc(EFS_ITEM_LIMIT);
    for(i = 1; i < CFG_EFS_MAX_OPEN_FILE_NUM; i++)
    {
        file_name = (char*)(efs->file_list_buf + (i * Ram_file_info_len));
        if(*file_name != (char)0xff)
        {
            for (filesize_no = 1; filesize_no < IndexesNum; filesize_no++)
            {
                if(!efs->drv->efs_read_media(efs->start_block, filesize_no * EFS_ITEM_LIMIT, file_info_buf, EFS_ITEM_LIMIT, EF_WR_NOECC))
                    memset(file_info_buf, 0xFF, EFS_ITEM_LIMIT);
                file_info_buf[FILENAME_LIMIT] = '\0';
                ChkOrRecNameByECC((char *)file_info_buf, file_info_buf + FILENAME_LIMIT + 1);
                if (strncmp((const char*)file_info_buf ,file_name, FILENAME_LIMIT) == 0)
                {     //�ҵ�Ŀ���ļ�
                    memcpy(efs->file_list_buf + (i * Ram_file_info_len), file_info_buf, EFS_ITEM_LIMIT);
                    break;
                }
            }
        }

    }
    free(file_info_buf);
    return true;
}


//----�޸��ļ��ߴ�-------------------------------------------------------------
//����: �޸��ļ���������ļ��ߴ粿�֡�1���������������һ������λ��д�롣2�����
//      �Ҳ�������������飬������filesize������ff��Ȼ����ļ��ߴ���µ���һ��
//      ��λ�ϣ������鲢д��֮��3����bak������ͬ�Ĳ�����
//����: ob�����������ļ�����ָ��
//      newsize���µ��ļ��ߴ�
//����: ��
//-----------------------------------------------------------------------------
static void __Efs_ChangeFileSize(struct Object *ob, u32 newsize)
{
    struct EfsFileInfo *fileinfo;
    struct EasyFS *efs;
    struct FileRsc *fp;
    u8 *block_buf;
    u32 fileoffset,sizeoffset, cfg_blocks,temp;
    u32 filesize_no,file_max_size,block_no,block_addr;
    u16 crc16_check;

    if(ob == NULL)
        return ;
    fp = (struct FileRsc *)OBJ_GetPrivate(ob);
    efs = (struct EasyFS *)File_Core(ob);
    fileinfo = (struct EfsFileInfo *) fp->private;
    if((efs == NULL) || (fp == NULL) || (fileinfo == NULL))
        return ;

    cfg_blocks = (FileInfoList + efs->block_size - 1) / efs->block_size;  //�ļ��������ռ����
    block_buf = efs->file_list_buf + (fileinfo->temp_item * Ram_file_info_len);
    ChkOrRecNameByECC((char *)block_buf,block_buf+FILENAME_LIMIT+1);
    //�ļ��ߴ粻�ܴ����ļ����ߴ�
    file_max_size = pick_little_32bit(block_buf,FILE_MAXSIZE_OFF >> 2);
    if(newsize > file_max_size)
        return;

    fileoffset = fileinfo->item * EFS_ITEM_LIMIT;
    sizeoffset = FILE_FILESIZE_OFF;
    newsize = __EfsFileSize_odd_even_check(newsize); //����У�����ļ���С
    for (filesize_no = 0; filesize_no < FILE_FILESIZE_NUM; filesize_no++)
    {
        if (pick_little_32bit(block_buf + sizeoffset, 0) == CN_LIMIT_UINT32) //����������ҵ�����λ��
        {
            fill_little_32bit(block_buf, sizeoffset >> 2, newsize);
            efs->drv->efs_write_media(efs->start_block, fileoffset + sizeoffset,
                    block_buf + sizeoffset, EFS_FILESIZE_BYTE_NUMBER,EF_WR_NOECC);  //д�µ��ļ���С���ļ���Сռ4�ֽ�
            efs->drv->efs_write_media(efs->start_block + cfg_blocks, fileoffset + sizeoffset,
                    block_buf + sizeoffset, EFS_FILESIZE_BYTE_NUMBER,EF_WR_NOECC);
            break;
        }
        sizeoffset += EFS_FILESIZE_BYTE_NUMBER;
    }

    if (filesize_no == FILE_FILESIZE_NUM) //�������û�п���λ��
    {
        memset(block_buf + FILE_FILESIZE_OFF, 0xFF, FILE_VERIFITY_OFF - FILE_FILESIZE_OFF);
        fill_little_32bit(block_buf, FILE_FILESIZE_OFF >> 2, newsize);
        //�ȶ����������ݵ�buf,������
        block_buf = NULL;
        block_buf = malloc(FileInfoList);
        if(block_buf == NULL)
        {
            warning_printf("efs", "failed to modify file size \r\n");
            return ;
        }
        efs->drv->efs_read_media(efs->start_block, 0, block_buf, FileInfoList, EF_WR_NOECC);
        __Efs_CheckAllNameECC(block_buf);

        sizeoffset = fileinfo->item * EFS_ITEM_LIMIT + FILE_FILESIZE_OFF;      //���ļ����ļ���С��λ��
        memset(block_buf + sizeoffset, 0xFF, FILE_VERIFITY_OFF - FILE_FILESIZE_OFF);  //�Ѹ��ļ�ԭ�����44���ļ���С���
        fill_little_32bit(block_buf, sizeoffset >> 2, newsize);     //���µ��ļ���С���ȥ

        block_no = fileoffset / efs->block_size;
        block_addr = block_no * efs->block_size;

        efs->drv->efs_erase_media(efs->start_block + block_no);       //�������и��ļ���Ϣ�����Ŀ�
        __Efs_Reset_File_Size(efs, block_buf + block_addr);
        if((FileInfoList - block_addr) < efs->block_size)   //���Ҫд������С��һ��Ĵ�С���Ǿ���ʵ����Ҫд�Ĵ�С
            efs->drv->efs_write_media(efs->start_block + block_no, 0, block_buf + block_addr, FileInfoList - block_addr, EF_WR_NOECC);
        else
            efs->drv->efs_write_media(efs->start_block + block_no, 0, block_buf + block_addr, efs->block_size, EF_WR_NOECC);
        if((efs->start_block + cfg_blocks - 1) == (efs->start_block + block_no))   //����������Ŀ飬�Ǵ���CRCУ����ģ�������д��CRC
        {
            crc16_check = CRC_16(block_buf, 16);
            fill_little_16bit((u8*)&temp, 0, crc16_check);    //ת��ΪС��
            efs->drv->efs_write_media(efs->start_block + cfg_blocks - 1, efs->block_size-2, (u8*)&temp,2,EF_WR_NOECC);
        }

        efs->drv->efs_erase_media(efs->start_block + block_no + cfg_blocks);        //�������и��ļ���Ϣ�����Ŀ�
        if((FileInfoList - block_addr) < efs->block_size)
            efs->drv->efs_write_media(efs->start_block + block_no + cfg_blocks, 0, block_buf + block_addr, FileInfoList - block_addr, EF_WR_NOECC);
        else
            efs->drv->efs_write_media(efs->start_block + block_no + cfg_blocks, 0, block_buf + block_addr, efs->block_size, EF_WR_NOECC);
        if((efs->start_block + 2*cfg_blocks - 1) == (efs->start_block + block_no + cfg_blocks))
            efs->drv->efs_write_media(efs->start_block + 2*cfg_blocks - 1, efs->block_size-2, (u8*)&temp, 2, EF_WR_NOECC);

        __Efs_RefreshFileList(efs);
        free(block_buf);
    }
}

// =============================================================================
// ���ܣ��½������ļ�����ʼ���ļ���ص���Ϣ���ļ���СΪ0�����ߴ�ΪĬ��
// ������fp -- �ļ�˽�����ݣ�ob -- �ļ��Ķ���ָ�룻filename -- �ļ����� fileinfo -- �ļ���Ϣָ��
// ���أ�fp -- �����ɹ���NULL -- ����ʧ��
// =============================================================================
static struct FileRsc *__Efs_NewFile(struct FileRsc* fp,struct Object *ob,const char *filename,
                        struct EfsFileInfo *fileinfo)
{
    struct EasyFS *efs;
    u8 *file_info_buf;
    static u8 seat = 0;
    u32 block_no, start_block, offset,block_addr;
    u8 cfg_blocks, item, loop, fill = 0;
    u64 index_offset;
    u16 crc16_check, temp;
    if( (ob == NULL) || (strlen(filename) > FILENAME_LIMIT) )//�ļ���̫��
    {
        printf("Create New File Failed For Too Long Name!\r\n");
        return NULL;
    }

    efs = (struct EasyFS*)File_Core(ob);
    if (efs->files_sum >= CreateMax)
        return NULL; //�Ѵ�ɴ����ļ�������

    offset = 0;
    //�����ļ�ϵͳ�ļ��������������ʹ�õ�blocks
    cfg_blocks = (FileInfoList + efs->block_size-1) / efs->block_size;
    //����Ϊȫ0��ȫFF��λ��
    file_info_buf = malloc(FileInfoList);
    if(file_info_buf == NULL)
    {
        warning_printf("efs", "memory request failed \r\n");
        return NULL;
    }
    efs->drv->efs_read_media(efs->start_block, 0, file_info_buf, FileInfoList, EF_WR_NOECC);
    __Efs_CheckAllNameECC(file_info_buf);
    for(item = 1; item < IndexesNum; item++)  //��һ���ļ������������ļ������ĵڶ���λ�ã����������1
    {
        offset = item * EFS_ITEM_LIMIT;
        if(file_info_buf[offset] == 0x00)  //��Ϊ0�����ʾ���ļ���ɾ��,�������½��ļ�
        {
            block_no = offset / efs->block_size;
            block_addr = block_no * efs->block_size;
            memset(file_info_buf+offset,0xFF,EFS_ITEM_LIMIT);
            __Efs_Reset_File_Size(efs, file_info_buf + block_addr);
//            //added,������CRCд���ļ�ϵͳ��Ϣ����������ֽ�
            efs->drv->efs_erase_media(efs->start_block + block_no);       //�������и��ļ���Ϣ�����Ŀ�
            if((FileInfoList - block_addr) < efs->block_size)   //���Ҫд������С��һ��Ĵ�С���Ǿ���ʵ����Ҫд�Ĵ�С
                efs->drv->efs_write_media(efs->start_block + block_no, 0, file_info_buf + block_addr, FileInfoList - block_addr, EF_WR_NOECC);
            else
                efs->drv->efs_write_media(efs->start_block + block_no, 0, file_info_buf + block_addr, efs->block_size, EF_WR_NOECC);
            if((efs->start_block + cfg_blocks - 1) == (efs->start_block + block_no))//����������Ŀ飬�Ǵ���CRCУ����ģ�������д��CRC
            {
                crc16_check = CRC_16(file_info_buf,16);
                fill_little_16bit((u8*)&temp,0,crc16_check);    //ת��ΪС��
                efs->drv->efs_write_media(efs->start_block + cfg_blocks - 1, efs->block_size-2, (u8*)&temp, 2,EF_WR_NOECC);
            }
            efs->drv->efs_erase_media(efs->start_block + block_no + cfg_blocks);       //�������и��ļ���Ϣ�����Ŀ�
            if((FileInfoList - block_addr) < efs->block_size)
                efs->drv->efs_write_media(efs->start_block + block_no + cfg_blocks, 0, file_info_buf + block_addr, FileInfoList - block_addr, EF_WR_NOECC);
            else
                efs->drv->efs_write_media(efs->start_block + block_no + cfg_blocks, 0, file_info_buf + block_addr, efs->block_size, EF_WR_NOECC);
            if((efs->start_block + 2*cfg_blocks - 1) == (efs->start_block + block_no + cfg_blocks))
                efs->drv->efs_write_media(efs->start_block + 2*cfg_blocks - 1, efs->block_size-2, (u8*)&temp, 2,EF_WR_NOECC);
            break;
        }
        if(file_info_buf[offset] == 0xFF) //�������ж��Ƿ����õ����ļ�����
        {
            break;          //����λ��
        }
    }

    if(item == IndexesNum)        //û���ҵ�����λ�û��ļ������ﵽ����
    {
        free(file_info_buf);
        return NULL;
    }

    //�����½����ļ����ݴ洢λ��
    start_block = efs->start_block + 2*cfg_blocks + (item - 1)*((dwFileMaxSize + efs->block_size-1)/efs->block_size);
    //ͬʱ��Ҫ�����ļ��������ڵĿ�����
    block_no = (dwFileMaxSize + efs->block_size-1) / efs->block_size;
    for(temp = 0; temp < block_no; temp++)
    {
        efs->drv->efs_erase_media(temp + start_block);
    }
    block_no = efs->start_block;
    //���³�ʼ���ļ������д��flash
    memset((char *)file_info_buf + offset, 0xff, EFS_ITEM_LIMIT);
    strcpy((char *)file_info_buf + offset, filename);        //д���ļ���
    file_info_buf[FILENAME_LIMIT + offset] = '\0';
    NameECC((char *)filename, file_info_buf + offset + FILENAME_LIMIT+1);        //�ļ���ECC
    fill_little_32bit(file_info_buf + offset, FILE_STARTBLOCK_OFF >> 2,start_block);      //�ļ�������ʼ��
    fill_little_32bit(file_info_buf + offset, FILE_MAXSIZE_OFF >> 2, dwFileMaxSize);      //�ļ�����С
    fill_little_32bit(file_info_buf + offset, FILE_FILESIZE_OFF >> 2, 0xc0000000);        //��Ϊ���½����ļ����ļ���С��д0
                                                                                          //�����λ�ֱ�������λ��ż��λ����У��
    memcpy(file_info_buf + offset + FILE_VERIFITY_OFF, "easyfile", 8);              //�ļ�����ĩβдeasyfile
    efs->drv->efs_write_media(block_no, offset, file_info_buf + offset, EFS_ITEM_LIMIT, EF_WR_NOECC);
    efs->drv->efs_write_media(block_no+cfg_blocks, offset, file_info_buf + offset, EFS_ITEM_LIMIT, EF_WR_NOECC);
    memcpy(fp->name,filename,FILENAME_LIMIT+1);
    //�����ļ���Ϣ����������
    fp->private = (ptu32_t)fileinfo;
    fp->file_size = 0;
    fp->ptr = 0;

    //�����ļ���Ϣ����������
    fileinfo->max_size = dwFileMaxSize;
    fileinfo->start_block = start_block;
    fileinfo->item = item;
    fileinfo->filesize = 0;

    __Efs_RefreshFileList(efs);

    for (loop = 0; loop < CFG_EFS_MAX_OPEN_FILE_NUM; loop++)
    {
        index_offset = loop * Ram_file_info_len;
        if(efs->file_list_buf[index_offset] == 0x00)  //���Ϊ�����ʾ����ļ���ϢΪ��
        {
            memcpy((char *)efs->file_list_buf + index_offset, (const char *)(file_info_buf + offset), EFS_ITEM_LIMIT);
            fill = 1;
            fileinfo->temp_item = loop;
            fill_little_32bit(efs->file_list_buf + index_offset + EFS_ITEM_LIMIT, 0, item);
            break;
        }
    }
    if(fill == 0)
    {
        memcpy((char *)efs->file_list_buf + (seat * Ram_file_info_len), (const char *)(file_info_buf + offset), EFS_ITEM_LIMIT);
        fill_little_32bit(efs->file_list_buf + (seat * Ram_file_info_len) + EFS_ITEM_LIMIT, 0, item);
        fileinfo->temp_item = seat;
        seat++;
        if(seat > 9)
            seat = 0;
    }

    free(file_info_buf);
    return fp;
}
// =============================================================================
// ���ܣ����ļ������ļ������ڣ���ɸ���mode��ʽ�������Ƿ��½��ļ�
// ������ob -- �ļ��Ķ���ָ��
//      flags -- �򿪷�ʽ
//      uncached -- ·��δƥ��Ĳ���
// ���أ��ļ����hdl -- �ɹ��� NULL -- ʧ��
// =============================================================================
static struct objhandle *Efs_Open(struct Object *ob, u32 flags, char *uncached)
{
    struct EasyFS *efs;
    struct FileRsc *fp=NULL;
    struct EfsFileInfo *fileinfo=NULL;
    u32 loop,item;
    u64 index_offset;
    u8 *hsize,*buf=NULL;
    bool_t found = false;
    struct objhandle *hdl;
//    mode_t mode, property = 0;
    if(ob == NULL)
        return NULL;

    efs = (struct EasyFS*)File_Core(ob);
    if (efs == NULL)
        return NULL;

    if(efs->file_sys_install == false)
    {
        struct Object *current;
        current = ob;
        while( (!File_ObjIsMount(current)) && (current != OBJ_GetRoot()))
        {
            current = OBJ_GetParent(current);
        }
        if(Efs_Verify_Install((struct FsCore *)OBJ_GetPrivate(current)))
        {
            warning_printf("efs", "file system set up fail");
            return (NULL);     //�ļ�ϵͳ����ʧ��
        }
    }

    if((!uncached)&&(File_ObjIsMount(ob)))
    {
        if(!Handle_FlagIsDirectory(flags))  //û��δƥ���·��������ob�ǹ��ص㣬���Ǵ�efs�ĸ�Ŀ¼
            return (NULL);  //������Ǵ�Ŀ¼�Ĳ�����ֱ�ӷ���ʧ��
    }

    if(tgOpenedSum >= CFG_EFS_MAX_OPEN_FILE_NUM)
        return NULL;

    if(Handle_FlagIsDirectory(flags))
    {
//        property = S_IFDIR;     // Ŀ¼�߼���������������ֱ�Ӱ�obj��hal����������
        if((uncached) || (!File_ObjIsMount(ob)))
            return NULL;
    }
    else
    {
        if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
            return (NULL);

        fp = malloc(sizeof(struct FileRsc));
        fileinfo = malloc(sizeof(struct EfsFileInfo));
        buf = malloc(CN_FILE_BUF_LIMIT);
        if( (NULL == fileinfo) || (NULL == buf) || (efs == NULL) || (fp == NULL))
            goto exit;

        memset(fp, 0, sizeof(struct FileRsc));
        memset(fileinfo, 0, sizeof(struct EfsFileInfo));
        memset(buf, 0xFF, CN_FILE_BUF_LIMIT);
        //ɨ��̶��ļ���
//        for (loop = 1; loop < IndexesNum; loop++)
        for (loop = 1; loop <= CFG_EFS_MAX_OPEN_FILE_NUM; loop++)
        {
            index_offset = loop * Ram_file_info_len;
            efs->file_list_buf[index_offset + FILENAME_LIMIT] = '\0';
            ChkOrRecNameByECC((char *)(efs->file_list_buf + index_offset), efs->file_list_buf + index_offset + FILENAME_LIMIT + 1);
            if (strncmp((const char*)efs->file_list_buf + index_offset, uncached, FILENAME_LIMIT) == 0)
            {
                found = true;
                fileinfo->temp_item = loop;
                fileinfo->item = pick_little_32bit(efs->file_list_buf + index_offset + EFS_ITEM_LIMIT, 0 );
                break;
            }
        }
        if(!found)
        {
            u8 *file_info_buf, fill = 0;
            static u8 seat = 0;
            file_info_buf = malloc(EFS_ITEM_LIMIT);
            if(file_info_buf == NULL)
            {
                warning_printf("efs", "memory request failed \r\n");
                goto exit;
            }
            memset(file_info_buf, 0xFF, EFS_ITEM_LIMIT);
            for (loop = 1; loop < IndexesNum; loop++)
            {
                if(!efs->drv->efs_read_media(efs->start_block, loop * EFS_ITEM_LIMIT, file_info_buf, EFS_ITEM_LIMIT, EF_WR_NOECC))
                {
                    warning_printf("efs", "read file info fail \r\n");
                    goto exit;
                }
                file_info_buf[FILENAME_LIMIT] = '\0';
                ChkOrRecNameByECC((char *)file_info_buf, file_info_buf + FILENAME_LIMIT + 1);
                if (strncmp((const char*)file_info_buf, uncached, FILENAME_LIMIT) == 0)
                {
                    found = true;
                    for (item = 0; item < CFG_EFS_MAX_OPEN_FILE_NUM; item++)
                    {
                        index_offset = item * Ram_file_info_len;
                        if(efs->file_list_buf[index_offset] == 0x00)  //���Ϊ�����ʾ����ļ���ϢΪ��
                        {
                            memcpy((char *)efs->file_list_buf + index_offset, (const char *)file_info_buf, EFS_ITEM_LIMIT);
                            fill = 1;
                            fileinfo->temp_item = item;
                            fill_little_32bit(efs->file_list_buf + index_offset + EFS_ITEM_LIMIT, 0, loop);
                        }
                    }
                    if(fill == 0)
                    {
                        memcpy((char *)efs->file_list_buf + (seat * Ram_file_info_len), (const char *)file_info_buf, EFS_ITEM_LIMIT);
                        fill_little_32bit(efs->file_list_buf + (seat * Ram_file_info_len) + EFS_ITEM_LIMIT, 0, loop);
                        index_offset = seat * Ram_file_info_len;
                        fileinfo->temp_item = seat;
                        seat++;
                        if(seat > 9)
                            seat = 0;
                    }
                    fileinfo->item = loop;      //�Ѹ��ļ��������ļ�������е�λ�ñ�������
                    break;
                }
            }
            free(file_info_buf);
        }
        if (!found)
        {
            if(Handle_FlagIsCreate(flags))//δ�ҵ����ж��Ƿ����½��ļ�
            {
                if(__Efs_NewFile(fp, ob, uncached, fileinfo))
                    efs->files_sum++;
                else
                {
                    if (efs->files_sum >= CreateMax)
                    {
                        warning_printf("efs", "file sum exceed upper limit (%d)",CreateMax);
                    }
                    goto exit;
                }
            }
            else
            {   // �ļ������ڣ�����Ҫ���½���
                printf("\r\n : dbug : efs    : open \"%s\" failed(do not exist).", uncached);
                goto exit;
            }
        }
        else     //�˴� �ļ�����
        {
            if(Handle_FlagIsOnlyCreate(flags)) // �Ѵ��ڣ���ֻҪ���½���
            {
                printf("\r\n : dbug : efs    : open \"%s\" failed(already exist).", uncached);
                goto exit;
            }
            //���������ļ���Դ����������struct file_rsc������Ҫ������ģ��ά���Ĳ��֡�
            for (loop = 0; loop < FILE_FILESIZE_NUM; loop++)
            {
                hsize = (efs->file_list_buf + index_offset + FILE_FILESIZE_END - loop * EFS_FILESIZE_BYTE_NUMBER);
                if ((pick_little_32bit(hsize,0) == CN_LIMIT_UINT32) ||
                    (pick_little_32bit(hsize,0) == 0))
                    continue;
//                if (pick_little_32bit(hsize,0) == ~(pick_little_32bit(hsize + 4,0)))
                if(__EfsFileSize_verify(*(u32 *)(hsize)) == 0)
                {
                    fp->file_size = (pick_little_32bit(efs->file_list_buf + index_offset,     //��44���ļ���С�У�����ȷ���ļ���С�������ʼ��
                            (FILE_FILESIZE_END - loop * EFS_FILESIZE_BYTE_NUMBER) >> 2) & EFS_SUPPORT_MAXFILESIZE);
                    break;
                }
            }
            fileinfo->start_block = pick_little_32bit(efs->file_list_buf + index_offset, FILE_STARTBLOCK_OFF >> 2);          //��ȡ�ļ����ݵĿ�ʼ��
            fileinfo->max_size = pick_little_32bit(efs->file_list_buf + index_offset, FILE_MAXSIZE_OFF >> 2);       //��ȡ�ļ�����С
            fileinfo->filesize = fp->file_size;     //��ȡ�ļ���ǰ��С
            memcpy(fp->name, (const char *)(efs->file_list_buf + index_offset), FILENAME_LIMIT+1);
        }

        fp->wr_buf = buf;       //�ļ�д����
        fp->buf_off = 0;        //�ļ�д�����е�ƫ��
        if(Handle_FlagIsAppend(flags))
            fp->ptr = fileinfo->filesize;       //׷��ģʽ������ļ���ǰλ������Ϊ�ļ���С
        else
            fp->ptr = 0;

        fp->private = (ptu32_t)fileinfo;
//        property = S_IFREG;
        tgOpenedSum ++;
//        if(!OBJ_NewChild(ob, e_operations, (ptu32_t)fp, uncached))
//        {
//            printf("\r\n: erro : efs    : new file \"%s\"(virtual).", uncached);
//            goto exit;
//        }
    }

    hdl = Handle_New();
    if(!hdl)
    {
        printf("\r\n : erro : efs    : open failed(memory out).");
        goto exit;
    }

    Handle_Init(hdl, NULL, flags, (ptu32_t)0);
    //�̳в��������������˽�г�Ա�������ģʽ���� stat �� st_mode ��
    ob = OBJ_BuildTempPath(ob, e_operations, (ptu32_t)fp,uncached);
    OBJ_LinkHandle(hdl, ob);

    Lock_MutexPost(efs->block_buf_mutex);
    return hdl;

exit:
    free(buf);
    free(fileinfo);
    free(fp);
    Lock_MutexPost(efs->block_buf_mutex);
    return NULL;
}

// =============================================================================
// ���ܣ���Ŀ¼��������һ���ļ�����fname����һ���ļ�
// ������hdl -- �ļ������dentry -- Ŀ¼�ṹ
// ���أ�ȫ�����꣨1����ʧ�ܣ�-1��������һ�0����
// =============================================================================
static s32 Efs_DirRead(struct objhandle *hdl,struct dirent *dentry)
{
    s32 result = -1;
    static u8 *file_info_buf = NULL;
    struct EasyFS *efs;
    u32 loop = 1;
    u64 index_offset;
    struct Object *ob = hdl->HostObj;
    if(NULL == dentry)
        return -1;

    efs = (struct EasyFS*)File_Core(ob);
    if(NULL == efs)
        return -1;

    if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
       return -1;

    //�ж��Ƿ���ڸ��ļ�
    if(dentry->d_off < 1)
    {
        dentry->d_off = 1;
        file_info_buf = malloc(EFS_ITEM_LIMIT);
    }
    if(file_info_buf == NULL)
    {
        warning_printf("efs", "memory request failed \r\n");
        return -1;
    }
    loop = dentry->d_off;   //�ļ�������е�ƫ��
    for(; loop < IndexesNum; loop++ )       //����fname��һ���ļ����һ���ļ�
    {
        index_offset = loop * EFS_ITEM_LIMIT;
        if(!efs->drv->efs_read_media(efs->start_block, index_offset, file_info_buf, EFS_ITEM_LIMIT, EF_WR_NOECC))
            memset(file_info_buf, 0xFF, EFS_ITEM_LIMIT);

        file_info_buf[FILENAME_LIMIT] = '\0';
        ChkOrRecNameByECC((char *)file_info_buf, file_info_buf + FILENAME_LIMIT + 1);
        //������һ����Ч���ļ�
        if((*file_info_buf != 0x00) && (*file_info_buf != 0xFF))
        {
            memcpy((char *)dentry->d_name, file_info_buf, FILENAME_LIMIT+1);     //��ȡ�ļ���
            result = 0;
            break;
        }
    }

    if(loop == IndexesNum)
    {
        dentry->d_off = 1;
        if(result == 0)
        {
            result = 1;  //�����������ļ�
            free(file_info_buf);
            file_info_buf = NULL;
        }
        else
            result = -1;
    }
    else
        dentry->d_off = loop + 1;
    Lock_MutexPost(efs->block_buf_mutex);
    if(result != -1)
    {
        dentry->d_reclen = sizeof(dentry->d_name);
        dentry->d_type |= DIRENT_IS_REG;        //efs��֧�ִ���Ŀ¼���������︳ֵ����ʼ�����ļ�����
    }

    return result;
}

// =============================================================================
// ���ܣ��� �ļ�
// ������hdl,�ļ�ָ��
//     buf,���ļ����ݴ洢�Ļ�����
//     len,���ļ��ֽ���
// ���أ���ȡ��ʵ�ʳ��� -- �ɹ���0 -- ʧ�ܣ�
// =============================================================================
static u32 Efs_Read (struct objhandle *hdl, u8 *buf, u32 len)
{
    struct EfsFileInfo *fileinfo;
    struct EasyFS *efs;
    struct FileRsc *fp;
    u32 buf_off, copy_len, block, readlen, rdsz = 0, completed = 0;
    u64 rd_point, blk_off;
    u8 rDataBuf[CN_FILE_BUF_LIMIT];
    struct Object *ob = hdl->HostObj;
    //�ļ�����flashоƬָ��
    fp = (struct FileRsc *)OBJ_GetPrivate(ob);
    efs = (struct EasyFS *)File_Core(ob);
    fileinfo = (struct EfsFileInfo *)fp->private;
    if ((len == 0) || (buf == NULL) || (fp == NULL) || (efs == 0) || (fileinfo == NULL))
    {
        printf("\r\nEfs_Read - FSR_INVALID_PARAMETER\r\n");
        return 0;
    }

    rd_point = fp->ptr;     //����λ��
    if((sint64_t)(rd_point + len) > fp->file_size)
        len = fp->file_size - rd_point;     //�ļ���ǰ��ȡλ�ü���Ҫ��ȡ�ĳ��ȴ����ļ���С�Ļ�������Ҫ�������ݳ���Ϊ�ļ���С��ȥ�ļ���ǰ�Ķ�ȡλ��

    if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
    {
        printf("\r\nEfs_Read - FSR_DENIED- locked\r\n");
        return 0;
    }
    while(((s32)len) > 0)
    {
        block = rd_point / efs->block_size + fileinfo->start_block;
        blk_off =  rd_point % efs->block_size;

        buf_off = blk_off % CN_FILE_BUF_LEN;    //�ÿ�ƫ�Ƽ�����ڶ�������256�ֽ��е�ƫ��
        if(buf_off)
            blk_off -= buf_off;     //һ�ζ�ȡ256�ֽڣ������ȥbuf_off��Ϊ�˶���

        if(rd_point >= fileinfo->filesize)
        {
            memcpy(rDataBuf,fp->wr_buf, CN_FILE_BUF_LEN);    //�ļ���ȡλ�ñ��ļ���ʵ�ʴ�С�����д�����������
            readlen = CN_FILE_BUF_LEN;
        }
        else
            readlen = efs->drv->efs_read_media(block, blk_off, rDataBuf, CN_FILE_BUF_LEN,EF_WR_ECC);    //��flash�ж�����

        copy_len = CN_FILE_BUF_LEN - buf_off;   //��������ڱ��ζ�������256�ֽ��л�ȡ������Ч����
        copy_len = (copy_len > len) ? len : copy_len;   //������Ի�ȡ��������������Ҫ��ȡ�������������copy_len��ֵΪ��Ҫ��ȡ��������
        if((buf_off + copy_len) > readlen)
        {   //������������ݴ��ڱ��δ�flash�����������
            if(buf_off >= readlen)    //��ӻ����и������ݵĳ���
                copy_len = len = 0; //bufƫ�ƶ����ڱ��α��δ�flash����������ݣ���ô��copy_len��lenȫ����
            else
                copy_len = len = readlen - buf_off; //��flash����˶�����Ч���ݾ��ö�����Ч����
        }
        memcpy(buf + completed,&rDataBuf[buf_off],copy_len);

        completed += copy_len;
        rd_point  += copy_len;
        len       -= copy_len;
    }

    fp->ptr += completed;
    rdsz = completed;
    Lock_MutexPost(efs->block_buf_mutex);
    return rdsz;
}

// =============================================================================
// ���ܣ�д�ļ�����������ļ������ߴ����ƣ��򷵻ش���
// ������hdl,�ļ����
//      buf,��д������ݻ�����
//      len,д�����ݵ��ֽ���
// ���أ�FSRESULT����ȷ������FSR_OK
// =============================================================================
static u32 Efs_Write (struct objhandle *hdl, u8 *buf, u32 len)
{
    struct EfsFileInfo *fileinfo = NULL;
    struct EasyFS *efs = NULL;
    struct FileRsc *fp = NULL;
    struct Object *ob = hdl->HostObj;
    int ret = 1;
    u32 write_sum, write_len, completed = 0, rdsz = 0, block, alignsize;
    u64 wr_point, offset_block;
    //�ļ�����flashоƬָ��
    fp = (struct FileRsc *)OBJ_GetPrivate(ob);
    efs = (struct EasyFS *)File_Core(ob);
    fileinfo = (struct EfsFileInfo *) fp->private;
    if ((fp == NULL) || (len == 0) || (buf == NULL) || (efs == 0) || (fileinfo == NULL))
        return 0;

    if(fp->ptr < fp->file_size)
        return 0;       //��֧�ָ�д�����ļ��ĵ�ǰλ�ã�С���ļ���Сʱ��ʧ��

    //������������ߴ磬�򳬳����ֻᱻ����
    if (len <= (fileinfo->max_size - fp->ptr))
        write_sum = len;
    else
        write_sum = fileinfo->max_size - fp->ptr;

    if((s32)write_sum <= 0)
        return 0;       //��Ҫд�����ݳ���С�ڵ�0��ֱ�ӷ���0
    wr_point = fp->ptr;

    //������ʼдλ�ô����ļ��ڵڼ����ƫ����
    block = (fileinfo->filesize / efs->block_size) + fileinfo->start_block;
    offset_block = fileinfo->filesize % efs->block_size;

    if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
       return 0;

    //�ж��Ƿ��д
    if(!efs->drv->efs_check_block_ready(block,offset_block,(u8*)buf,fp->buf_off + len))
    {
        Lock_MutexPost(efs->block_buf_mutex);
        return 0;
    }

    memset(fp->wr_buf + fp->buf_off, 0xFF, (CN_FILE_BUF_LIMIT - fp->buf_off));
    if( fileinfo->filesize % CN_FILE_BUF_LEN )//���ȶ�����,ֻ��close,flush�����
    {
        fp->buf_off = fileinfo->filesize % CN_FILE_BUF_LEN;
        efs->drv->efs_read_media(block, (offset_block & (~0xFF)), fp->wr_buf, fp->buf_off, EF_WR_NOECC); // �ض����ݣ�Ϊ�˺���256д��ʱ��ECCУ��
    }

    alignsize = fileinfo->filesize - fileinfo->filesize % CN_FILE_BUF_LEN;  //��ȡ�ļ���С��ȥ�˲���256�ֽڳ��ȵĴ�С
    //�ļ���ǰλ�ü���Ҫд������ݴ�С���ڵ���alignsize + CN_FILE_BUF_LEN�Ļ������ȸ����ļ���С������ζ�ű���д����������һ�λ���flash��д���ݣ�
    if(wr_point + write_sum >= alignsize + CN_FILE_BUF_LEN )
    {
        write_len = wr_point + write_sum;
        write_len = write_len - (write_len % CN_FILE_BUF_LEN);      //����256�ֽڵ����ݣ����ļ�closeʱ��д��flash
        fileinfo->filesize = write_len;
        __Efs_ChangeFileSize(ob, write_len);    //��filesizeд��flash
    }

    while(1)
    {
        fp->buf_off = wr_point % CN_FILE_BUF_LEN;   //��ȡ�ļ�д����������ݳ���
        write_len = CN_FILE_BUF_LEN - fp->buf_off;  //�����ļ������ﻹ��д��������

        if(write_sum < write_len)
        {   //�ļ���������д�����ݴ��ڱ�����Ҫд��������
            write_len = write_sum;
            memcpy(fp->wr_buf + fp->buf_off,buf+completed,write_len);   //��д��д����
            fp->buf_off += write_len;
            break;
        }
        else//write the buf to flash
        {    //��д���������д��flash
            memcpy(fp->wr_buf + fp->buf_off,buf+completed,write_len);
            block = (wr_point / efs->block_size) + fileinfo->start_block;
            offset_block = (wr_point % efs->block_size) - fp->buf_off;
            if(CN_FILE_BUF_LEN != efs->drv->efs_write_media(block,offset_block,
                    fp->wr_buf,CN_FILE_BUF_LEN,EF_WR_ECC))
            {
                write_len = 0;
                ret = 0;
                break;
            }
            fp->buf_off = 0;
            memset(fp->wr_buf, 0xFF, CN_FILE_BUF_LIMIT); // д�����������ֹseek�����������һЩ����֪��ֵ��
        }
        wr_point += write_len;
        write_sum -= write_len;
        completed += write_len;
    }

    fp->ptr += completed + write_len;
    fp->file_size = fp->ptr;
    rdsz = completed + write_len;

    if(ret)
    {
        if(fileinfo->filesize != (u64)(fp->ptr - fp->ptr % CN_FILE_BUF_LEN))
        {
            fileinfo->filesize = fp->ptr - fp->ptr % CN_FILE_BUF_LEN;
            __Efs_ChangeFileSize(ob, fileinfo->filesize);    //д�ɹ������ļ���С
        }
    }

    Lock_MutexPost(efs->block_buf_mutex);
    return rdsz;
}

// =============================================================================
// ���ܣ��ر��ļ�,��Ҫ�������ͷ��ļ�ָ��Ŀռ�ʹ��ļ��ռ�
// ������hdl,�ļ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// =============================================================================
static s8 Efs_Close (struct objhandle *hdl)
{
    struct EfsFileInfo *fileinfo;
    struct EasyFS *efs;
    u32 block;
    u64 offset;
    struct FileRsc *fp;
    struct Object *ob = hdl->HostObj;
    s8 ret = 0;

    fp = (struct FileRsc *)OBJ_GetPrivate(ob);
    efs = (struct EasyFS *)File_Core(ob);
    fileinfo = (struct EfsFileInfo *)fp->private;
    if ((fp == NULL) || (efs == 0) || (fileinfo == NULL))
    {
        printk("close file param error\r\n");
        return -1;
    }
    if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
    {
        printk("close file timeout\r\n");
       return -1;
    }

    if(!Handle_FlagIsDirectory(hdl->flags)) // ��Ŀ¼�߼�
    {
        //��wr_buf�����ݣ����Ƚ���д��flash������ECC
        if(fp->buf_off)
        {
            block = (fileinfo->filesize / efs->block_size) + fileinfo->start_block;
            offset = (fileinfo->filesize % efs->block_size) & (~0xFF); // 256���룬fp->buf_off���ܴ��ڻض����ݣ���filesize��һ����256�����

            if(!efs->drv->efs_check_block_ready(block, offset, fp->wr_buf, fp->buf_off))
            {
                ret = -1;
                goto exit;
            }

            __Efs_ChangeFileSize(ob, fp->file_size);
            //�˴�д���Сû��256��д��EF_WR_ECCֻ��Ϊ�˵ײ������һ��
            if(fp->buf_off !=
                    efs->drv->efs_write_media(block,offset,fp->wr_buf,fp->buf_off,EF_WR_ECC))
            {
                __Efs_ChangeFileSize(ob, fileinfo->filesize);   //д��ʧ�ܣ������filesize
                ret = -1;
                goto exit;
            }
        }

        free(fileinfo);
        free(fp->wr_buf);
        free(fp);//todo ---- ��Ӧ���ڴ˴��ͷţ���efs/port.c�����ͷţ���Ϊ������malloc
        tgOpenedSum --;
    }
//  Handle_Delete(hdl);    //��Ŀ¼�Ļ�ɶҲ����ֱ��ɾ�����
exit:
    Lock_MutexPost(efs->block_buf_mutex);
    return (ret);
}
// =============================================================================
// ���ܣ�seek�������������ܣ���EFS�У����ڸ��½����ļ����ú���ʵ���������ߴ磬��������
//      �ú���ʵ���ƶ���дָ�루����ʵ�ֶ�̬����Ĺ��ܣ�
// ������hdl,�ļ������file_offset,ƫ�ƶ���λ�ã�whence�������￪ʼƫ��
// ���أ�FSRESULT����ȷ������FSR_OK
// =============================================================================
static off_t Efs_Seek(struct objhandle *hdl, off_t *file_offset, s32 whence)
{
    struct EfsFileInfo *fileinfo;
    struct EasyFS *efs;
    u32 block, offset, newpos = 0, fsizeoff, alignsize;
    struct FileRsc *fp;
    struct Object *ob = hdl->HostObj;

    fp = (struct FileRsc *)OBJ_GetPrivate(ob);
    efs = (struct EasyFS *)File_Core(ob);
    fileinfo = (struct EfsFileInfo *)fp->private;
    if ((fp == NULL) || (efs == 0) || (fileinfo == NULL))
        return -1;

    switch(whence)
    {
        case SEEK_SET: newpos = (*file_offset);break;    //���ļ���ʼ��ƫ��
        case SEEK_CUR: newpos = (*file_offset + fp->ptr);break;     //���ļ���ǰλ�ÿ�ʼƫ��
        case SEEK_END: newpos = (*file_offset + fp->file_size); break;     //���ļ�����λ�ÿ�ʼƫ��
        default: return (-1);// ��������
    }

    if(newpos > fileinfo->max_size)   //�ļ�λ�ó����ļ���С��ʧ��
        return -1;

    if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
        return -1;

    fsizeoff = fileinfo->filesize % CN_FILE_BUF_LEN;
    alignsize = fileinfo->filesize - fsizeoff;
    //���seek > CN_FILE_BUF_LEN,��wr_bufд��ȥ
    if( (newpos - alignsize >= CN_FILE_BUF_LEN) && (newpos > alignsize) )
    {
        block = (fileinfo->filesize / efs->block_size) + fileinfo->start_block;
        offset = fileinfo->filesize % efs->block_size;
        if(!efs->drv->efs_check_block_ready(block,offset,fp->wr_buf,CN_FILE_BUF_LEN))
        {
            Lock_MutexPost(efs->block_buf_mutex);
            return -1;
        }

        if(fsizeoff)        //�ж��Ƿ���Ҫ��������write
        {
            offset = alignsize % efs->block_size;
            memset(fp->wr_buf,0xFF,CN_FILE_BUF_LEN);
            efs->drv->efs_read_media(block ,offset, fp->wr_buf, fsizeoff, EF_WR_NOECC);
        }

        __Efs_ChangeFileSize(ob, newpos - newpos % CN_FILE_BUF_LEN);//�޸��ļ���С

        if(CN_FILE_BUF_LEN != efs->drv->efs_write_media(block, alignsize, fp->wr_buf,
                CN_FILE_BUF_LEN,EF_WR_ECC))
        {
            __Efs_ChangeFileSize(ob, fileinfo->filesize);    //���д��ʧ������ļ���С�ָ�
            Lock_MutexPost(efs->block_buf_mutex);
            return -1;
        }
        else
        {
            fileinfo->filesize = newpos - newpos % CN_FILE_BUF_LEN;    //�ļ�ʵ�ʴ�С
            fp->file_size = newpos;     //�ļ���д���������ݵĴ�С
            fp->buf_off = 0;
            memset(fp->wr_buf,0xFF,CN_FILE_BUF_LEN);
        }
    }

    fp->ptr = newpos;
    Lock_MutexPost(efs->block_buf_mutex);
    return newpos;
}

// =============================================================================
// ���ܣ�ɾ���ļ�
// ������ob,�ļ������ַ�� uncached,·����δƥ��Ĳ���
// ���أ��ɹ�  -- 0�� ʧ�� -- -1
// =============================================================================
static s32 Efs_Remove(struct Object *ob, char *uncached)
{
    struct EasyFS *efs;
    u64 index_offset=0,ram_offset;
    u8 cfg_blocks,loop;
    u8 *file_info_buf;
    char *fname;
    if(ob == NULL)
        return -1;

    efs = (struct EasyFS*)File_Core(ob);
    if(NULL == efs)
    {
        printf("cannot fine efs root");
        return -1;
    }
    if(uncached)
        fname = uncached;
    else
        fname = ob->name;

    file_info_buf = malloc(EFS_ITEM_LIMIT);
    if(file_info_buf == NULL)
    {
        warning_printf("efs", "memory request failed \r\n");
        return -1;
    }

    if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
       return -1;

    cfg_blocks = (FileInfoList+ efs->block_size-1) / efs->block_size;      //�ļ��������ռ�Ŀ���
    for (loop = 1; loop < IndexesNum; loop++)
    {
        index_offset = loop * EFS_ITEM_LIMIT;
        if(!efs->drv->efs_read_media(efs->start_block, index_offset, file_info_buf, EFS_ITEM_LIMIT, EF_WR_NOECC))
            memset(file_info_buf, 0xFF, EFS_ITEM_LIMIT);

        file_info_buf[index_offset + FILENAME_LIMIT] = '\0';
        ChkOrRecNameByECC((char *)file_info_buf, file_info_buf + FILENAME_LIMIT + 1);
        if (strncmp((const char*)file_info_buf, fname, FILENAME_LIMIT) == 0)
        {
            break;//�ҵ����ļ�
        }
    }

    if(loop == IndexesNum)
    {
        printf("cannot find :%s\r\n", fname);
        free(file_info_buf);
        Lock_MutexPost(efs->block_buf_mutex);
        return -1;
    }

    if(loop < IndexesNum)
    {
        for (loop = 0; loop < CFG_EFS_MAX_OPEN_FILE_NUM; loop++)
        {
            ram_offset = loop * Ram_file_info_len;
            efs->file_list_buf[ram_offset + FILENAME_LIMIT] = '\0';
            ChkOrRecNameByECC((char *)(efs->file_list_buf + ram_offset), efs->file_list_buf + ram_offset + FILENAME_LIMIT + 1);
            if (strncmp((const char*)efs->file_list_buf + ram_offset, fname, FILENAME_LIMIT) == 0)
                memset(efs->file_list_buf + ram_offset, 0x0, Ram_file_info_len);
        }

        //ɾ�����ļ���Ϣ��дȫ0
        memset(file_info_buf, 0x0, EFS_ITEM_LIMIT);
        efs->drv->efs_write_media(efs->start_block, index_offset, file_info_buf,
                EFS_ITEM_LIMIT,EF_WR_NOECC);
        efs->drv->efs_write_media(efs->start_block+cfg_blocks, index_offset,
                file_info_buf,EFS_ITEM_LIMIT,EF_WR_NOECC);
        efs->files_sum --;
#if 1
{       //�ж��ļ���������/���Ƿ�дΪ��0
        u16 CountTemp;
        efs->drv->efs_read_media(efs->start_block, index_offset, file_info_buf,
                EFS_ITEM_LIMIT, EF_WR_NOECC);
        for(CountTemp = 0; CountTemp < EFS_ITEM_LIMIT; CountTemp++)
        {
            if(file_info_buf[CountTemp] != 0x0)
            {
                printf("\r\nremove error in \"%s\"\r\n", fname);
                break;
            }
        }
        efs->drv->efs_read_media(efs->start_block+cfg_blocks, index_offset, file_info_buf,
                EFS_ITEM_LIMIT, EF_WR_NOECC);
        for(CountTemp = 0; CountTemp < EFS_ITEM_LIMIT; CountTemp++)
        {
            if(file_info_buf[CountTemp] != 0x0)
            {
                printf("\r\nremove error in \"%s\"\r\n", fname);
                break;
            }
        }
}
#endif
    }
    free(file_info_buf);
    Lock_MutexPost(efs->block_buf_mutex);
    return 0;
}

// =============================================================================
// ���ܣ���ȡ�ļ�״̬
// ������ob,�ļ������ַ��data�����ļ�״̬�Ľṹ��uncached,·����δƥ��Ĳ���
// ���أ��ɹ�  -- 0��ʧ��  -- -1
// =============================================================================
static s32 Efs_Stat(struct Object *ob, struct stat *data, char *uncached)
{
    struct EasyFS *efs;
    struct FileRsc* fp;
    u32 loop;
    u64 index_offset;
    u8 found = false;
    char *fname;
    u8 *hsize;
    u8 *file_info_buf;
    if(ob == NULL)
        return -1;

    if((uncached == NULL) || ((*uncached == '\0') ))
    {
        if(File_ObjIsMount(ob))
        {
            data->st_size = 0; // ��װ�㣻
            data->st_mode = S_IFDIR;
        }
        else
        {
            fp = (struct FileRsc *)OBJ_GetPrivate(ob);      //��ȡ�Դ��ļ���״̬
            data->st_size = fp->file_size;
            data->st_mode = S_IFREG|S_IRUGO|S_IWUGO;
        }
        return 0;
    }

    efs = (struct EasyFS*)File_Core(ob);
//    if(uncached)
        fname = uncached;
//    else
//        fname = ob->name;

    if(NULL == efs)
        return -1;

    if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
       return -1;

    file_info_buf = malloc(EFS_ITEM_LIMIT);
    if(file_info_buf == NULL)
    {
        warning_printf("efs", "memory request failed \r\n");
        return -1;
    }
    //�ж��Ƿ���ڸ��ļ�
    for (loop = 1; loop < IndexesNum; loop++)
    {
        index_offset = loop * EFS_ITEM_LIMIT;
        if(!efs->drv->efs_read_media(efs->start_block, index_offset, file_info_buf, EFS_ITEM_LIMIT, EF_WR_NOECC))
            memset(file_info_buf, 0xFF, EFS_ITEM_LIMIT);
        file_info_buf[FILENAME_LIMIT] = '\0';
        ChkOrRecNameByECC((char *)file_info_buf, file_info_buf + FILENAME_LIMIT + 1);
        if (strncmp((const char*)file_info_buf ,fname,FILENAME_LIMIT) == 0)
        {
            found = true;       //�ҵ�Ŀ���ļ�
            break;
        }
    }

    if(found)
    {
        for (loop = 0; loop < FILE_FILESIZE_NUM; loop++)
        {
            hsize = (file_info_buf + FILE_FILESIZE_END - loop * EFS_FILESIZE_BYTE_NUMBER);
            if ((pick_little_32bit(hsize,0) == CN_LIMIT_UINT32) ||
                (pick_little_32bit(hsize,0) == 0))
                continue;
            if (__EfsFileSize_verify(*(u32 *)hsize) == 0)
            {   //��ȡĿ���ļ��Ĵ�С
                data->st_size = (pick_little_32bit(file_info_buf,     //��44���ļ���С�У�����ȷ���ļ���С�������ʼ��
                        (FILE_FILESIZE_END - loop * EFS_FILESIZE_BYTE_NUMBER) >> 2) & EFS_SUPPORT_MAXFILESIZE);
                break;
            }
        }
        data->st_mode = S_IFREG|S_IRUGO|S_IWUGO;    //efs��ֻ���ļ�û��Ŀ¼�����������ҵ����ļ�������ȫ����ֵΪ�ļ�
        free(file_info_buf);
        Lock_MutexPost(efs->block_buf_mutex);
        return 0;
    }
    free(file_info_buf);
    Lock_MutexPost(efs->block_buf_mutex);
    return -1;
}

// =============================================================================
// ���ܣ������������д��flash
// ������hdl,�ļ����
// ���أ��ɹ� -- 0��ʧ�� -- -1
// =============================================================================
static s32 Efs_Sync (struct objhandle *hdl)
{
    struct EfsFileInfo *fileinfo;
    struct EasyFS *efs;
    struct FileRsc *fp;
    u32 block;
    u64 offset;
    struct Object *ob = hdl->HostObj;

    fp = (struct FileRsc *)OBJ_GetPrivate(ob);
    efs = (struct EasyFS *)File_Core(ob);
    fileinfo = (struct EfsFileInfo *)fp->private;
    if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
       return -1;

    //��wr_buf�����ݣ����Ƚ���д��flash������ECC
    if(fp->buf_off)
    {
        block = (fileinfo->filesize / efs->block_size) + fileinfo->start_block;
        offset = fileinfo->filesize % efs->block_size;
        if(!efs->drv->efs_check_block_ready(block, offset, fp->wr_buf, fp->buf_off))
        {
            Lock_MutexPost(efs->block_buf_mutex);
            return -1;
        }

        __Efs_ChangeFileSize(ob, fp->file_size);        //�޸��ļ���С������д����������ݣ�
        //�˴�д���Сû��256��д��EF_WR_ECCֻ��Ϊ�˵ײ������һ��
        if(fp->buf_off !=
                efs->drv->efs_write_media(block, offset, fp->wr_buf, fp->buf_off, EF_WR_ECC))
        {
            __Efs_ChangeFileSize(ob, fileinfo->filesize);   //д��ʧ�ܣ������filesize
            Lock_MutexPost(efs->block_buf_mutex);
            return -1;
        }
        else
        {
            fileinfo->filesize = fp->ptr;
            fp->buf_off = 0;
        }
    }
    Lock_MutexPost(efs->block_buf_mutex);
    return 0;
}
// ============================================================================
// ���ܣ���ʼ��efs��media������
// ������core -- efs�ļ�ϵͳ����  drv -- media�Ĳ���������
// ���أ�0 -- �ɹ���  -1 --ʧ��
// ��ע��
// ============================================================================
int efs_install_drv(struct EasyFS *core, struct __efs_media_drv *drv)
{
    if (!core || !drv)
         return (-1);
    core->drv = malloc(sizeof(*drv));
    core->drv->efs_erase_media = drv->efs_erase_media;
    core->drv->efs_read_media = drv->efs_read_media;
    core->drv->efs_write_media = drv->efs_write_media;
    core->drv->efs_check_block_ready = drv->efs_check_block_ready;
    return (0);
}
// ============================================================================
// ���ܣ���ʼ��efs����Ϣ
// ������core -- efs�ļ�ϵͳ����  pSuper -- �ļ�ϵͳ��������
// ���أ����� -- �ɹ��� NULL --ʧ��
// ��ע��
// ============================================================================
struct EasyFS *EfsInfo(struct FsCore *pSuper, u32 opts)
{
    struct umedia *media;
    struct EasyFS *core;
    u32 flash_block_size,flash_page_size,blocks_per_page,filedatablocks,allblocknum,num,filelistblock;

    if(pSuper == NULL)
        return NULL;

    media = (struct umedia*)pSuper->MediaInfo;              //��ȡflash����Ϣ
    media->mreq(blockunits,(ptu32_t)&blocks_per_page);      //��ȡflash��һ���ж���ҳ
    media->mreq(unitbytes,(ptu32_t)&flash_page_size);       //��ȡflash��ҳ��С
    flash_block_size = blocks_per_page * flash_page_size;   //��ȡflash�Ŀ��С
    allblocknum = pSuper->AreaSize / flash_block_size;      //�ܿ���
    // EFS�����ļ�ʵ����������Ҫ�Ŀ���
    filedatablocks = (CFG_EFS_FILE_SIZE_LIMIT + flash_block_size - 1) / flash_block_size;
    num = allblocknum / filedatablocks-1;     //Ԥ�ƿ��Դ���������ļ���
    //�����ļ��б�����Ҫ�Ŀ���
    filelistblock = (((num + 1) * EFS_ITEM_LIMIT) + flash_block_size - 1) / flash_block_size;
    //����ʵ���ܴ���������ļ���
    while(((num * filedatablocks) + (filelistblock * 2)) > allblocknum)
    {
        num--;
        filelistblock = (((num + 1) * EFS_ITEM_LIMIT) + flash_block_size - 1) / flash_block_size;
    }
    if(num > CFG_EFS_MAX_CREATE_FILE_NUM)
        num = CFG_EFS_MAX_CREATE_FILE_NUM;
    CreateMax = num;
    IndexesNum = num + 1;
    FileInfoList = IndexesNum * EFS_ITEM_LIMIT;
    if((CreateMax == 0) || (IndexesNum == 0) || (EfsCgfLimit == 0))
    {
        error_printf("efs","file system core parameters calculated incorrectly.\r\n");
        return NULL;
    }
    notice_printf("efs","create max file number %d .\r\n",num);

    core = malloc(sizeof(*core) + EfsCgfLimit);
    if(!core)
    {
        printf("\r\n: erro : efs    : install failed(memory out).");
        return NULL;
    }
    memset(core, 0x0, sizeof(*core) + EfsCgfLimit);
    core->block_buf_mutex = Lock_MutexCreate("EFS_BLOCK_BUF_SEMP");
    if(!core->block_buf_mutex)
    {
        free(core);
        printf("\r\n: erro : efs    : install failed(cannot create lock).");
        return NULL;
    }
    core->install_options = opts;
    pSuper->pCore = (void*)core;
    core->media = media;
    efs_install_drv(core,pSuper->MediaDrv);
    core->file_list_buf = (u8*)core + sizeof(*core);
    core->block_size = flash_block_size;
    core->block_sum = allblocknum;
    core->files_sum = 0;
    memcpy(core->name, pSuper->pTarget->name, EFS_NAME_LIMIT+1);
    core->start_block = pSuper->MediaStart / blocks_per_page;

    return core;
}
// =============================================================================
// ���ܣ������ļ�ϵͳ����mount���ɹ�ʱ������ã����ú�ԭ���ļ���Ϣ��������
// ������efs,�ļ�ϵͳָ��
//      pSuper -- �ļ�ϵͳ��������
// ���أ�0 -- �ɹ��� -1 --ʧ��
// =============================================================================
static s32 Efs_Mkfs(struct EasyFS* efs,struct FsCore *pSuper)
{
    u32 loop, blocks, cfg_end_block;
    u8 *block_buf;
    u16 crc16_check,temp;

    if(!efs->block_sum) // ˵����chipδ��ʼ��
    {
        error_printf("efs","file system was not initialized.\r\n");
        return -1;
    }

    blocks = (FileInfoList + efs->block_size-1) / efs->block_size;
    cfg_end_block = efs->start_block + blocks;
//    block_buf = efs->file_list_buf;
    if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
       return -1;

    block_buf = M_MallocLc(FileInfoList, 0);
    if(!block_buf)
    {
        Lock_MutexPost(efs->block_buf_mutex);
        return -1;
    }

    //д��efs�ļ�ϵͳ�ĺ�����Ϣ��flash��
    memset(block_buf, 0xff, FileInfoList);
    memcpy(block_buf, "easyfile", 8);
    memcpy(block_buf + FILE_VERIFITY_OFF, "easyfile", 8);
    fill_little_32bit(block_buf, 2, efs->block_sum);
    fill_little_32bit(block_buf, 3, efs->block_size);

    crc16_check = CRC_16(block_buf,16);
    fill_little_16bit((u8*)&temp,0,crc16_check); // ת��ΪС��
    for (loop = efs->start_block; loop < cfg_end_block; loop++)
    {
        efs->drv->efs_erase_media(loop);       //efs�õ������������һ��
        efs->drv->efs_erase_media(loop + blocks);       //efs�õ������������һ��
    }
    efs->drv->efs_write_media(efs->start_block, 0, block_buf, FileInfoList ,EF_WR_NOECC);
    efs->drv->efs_write_media(cfg_end_block, 0, block_buf, FileInfoList ,EF_WR_NOECC);
    //added,������CRCд���ļ�ϵͳ��Ϣ����������ֽ�
    efs->drv->efs_write_media(efs->start_block + blocks - 1,
            efs->block_size-2, (u8*)&temp,2,EF_WR_NOECC);
    efs->drv->efs_write_media(cfg_end_block + blocks - 1,
            efs->block_size-2, (u8*)&temp,2,EF_WR_NOECC);

    efs->files_sum = 0;

    if(pSuper)
    {
        //ÿ���½�����fs��ŵ�ȫ�ֱ��� s_pEfsList�����ں������
        s_pEfsList = efs;
        memcpy(s_pEfsList->name,pSuper->pTarget->name,EFS_NAME_LIMIT+1);
    }
    efs->file_sys_install = true;
    free(block_buf);
    Lock_MutexPost(efs->block_buf_mutex);
    return 0;
}

// ============================================================================
// ���ܣ��ļ�ϵͳ�ӿڣ�
// ������
// ���أ����ݸ�����Ҫ��
// ��ע��
// ============================================================================
s32 e_operations(void *opsTarget, u32 objcmd, ptu32_t OpsArgs1,
                 ptu32_t OpsArgs2, ptu32_t OpsArgs3)
{
    s32 result = CN_OBJ_CMD_EXECUTED;

    switch(objcmd)
    {
        case CN_OBJ_CMD_OPEN:
        {
            struct objhandle *hdl;
            hdl = Efs_Open((struct Object *)opsTarget, (u32)(*(u64*)OpsArgs2), (char*)OpsArgs3);
            *(struct objhandle **)OpsArgs1 = hdl;
            break;
        }

      case CN_OBJ_CMD_READDIR:
      {
          struct objhandle *hdl = (struct objhandle*)OpsArgs3;
          struct dirent *ret = (struct dirent *)OpsArgs1;

          if((ptu32_t)Efs_DirRead(hdl, ret) == 0)
              result = CN_OBJ_CMD_TRUE;
          else
              result = CN_OBJ_CMD_FALSE;
          break;
      }

        case CN_OBJ_CMD_READ:
        {
            u32 len;
            struct objhandle *devfile = (struct objhandle*)opsTarget;

            len = Efs_Read(devfile, (u8*)OpsArgs2, (u32)OpsArgs3);
            *(u32 *)OpsArgs1 = len;
            break;
        }

        case CN_OBJ_CMD_WRITE:
        {
            u32 len;
            struct objhandle *devfile = (struct objhandle*)opsTarget;

            len = Efs_Write(devfile, (u8*)OpsArgs2, (u32)OpsArgs3);
            *(u32 *)OpsArgs1 = len;
            break;
        }

        case CN_OBJ_CMD_CLOSE:
        {
            if(Efs_Close((struct objhandle*)opsTarget) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        case CN_OBJ_CMD_SEEK:
        {
            *(off_t*)OpsArgs1 = Efs_Seek((struct objhandle *)opsTarget, (off_t*)OpsArgs2, (s32)OpsArgs3);
            break;
        }

        case CN_OBJ_CMD_DELETE:
        {
            if(Efs_Remove((struct Object*)opsTarget, (char *)OpsArgs3) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        case CN_OBJ_CMD_STAT:
        {
            if(Efs_Stat((struct Object*)opsTarget, (struct stat *)OpsArgs1, (char*)OpsArgs3) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        case CN_OBJ_CMD_SYNC:
        {
            if(Efs_Sync((struct objhandle *)opsTarget) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        default:
        {
            result = CN_OBJ_CMD_UNSUPPORT;
            break;
        }
    }

    return result;
}

// ============================================================================
// ���ܣ���ʽ��efs�ļ�ϵͳ
// ������core -- �ļ� ϵͳ��������
// ���أ�0 -- �ɹ�; -1 -- ʧ��;
// ��ע��
// ============================================================================
static s32 Efs_Format(void *core)
{
    if(core == NULL)
        return -1;

    struct EasyFS *efs = (struct EasyFS *)core;
    return Efs_Mkfs(efs,NULL);
}
// ============================================================================
// ���ܣ�У���װ�ļ�ϵͳ��
// ������core -- �ļ�ϵͳ�ĺ������ݽṹ
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
static s32 Efs_Verify_Install(struct FsCore *pSuper)
{
    u32 loop, blocks, end_block, fileInfoSize;
    u8 *bakbuf, *block_buf;
    u8 mainblockerr = 0,bakblockerr = 0;
    u16 crc16_check,temp;
    u64 index_offset;

    if(pSuper == NULL)
        return -1;

    struct EasyFS *efs = (struct EasyFS *)pSuper->pCore;

    if(efs->install_options & MS_INSTALLFORMAT)
    {
        if(Efs_Mkfs(efs,pSuper) == 0)    //��װʱ���ʽ�������ļ�ϵͳ
            return 0;
        else
        {
            error_printf("efs","format fail.\r\n");
            free(efs);
            return -1;
        }
    }

    bakbuf = M_MallocLc(FileInfoList, 0);//ֻ���ļ���Ϣ��С
    if(bakbuf == NULL)
    {
        free(efs);
        return -1;
    }
    block_buf = M_MallocLc(FileInfoList, 0);
    if(block_buf == NULL)
    {
        free(bakbuf);
        free(efs);
        return -1;
    }
    blocks = (FileInfoList + efs->block_size-1) / efs->block_size; // EFS�ļ����������Ҫʹ�õĿ���
    end_block = efs->start_block + blocks; //
    fileInfoSize = FileInfoList;
//    block_buf = efs->file_list_buf;

   if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
   {
       free(bakbuf);
       free(efs);
       return -1;
   }

    //����ļ�ϵͳ�Ƿ��Ѿ�����
    if(!efs->drv->efs_read_media(efs->start_block, 0, block_buf, fileInfoSize,EF_WR_NOECC))
        goto fail;
    if(!efs->drv->efs_read_media(end_block, 0, bakbuf, fileInfoSize,EF_WR_NOECC))
        goto fail;
    //�����������ƻ�������Ҫ���½����ļ�ϵͳ
    if ((memcmp(bakbuf, "easyfile", 8) ||
            memcmp(bakbuf + FILE_VERIFITY_OFF, "easyfile", 8)) &&
        (memcmp(block_buf, "easyfile", 8)||
            memcmp(block_buf + FILE_VERIFITY_OFF, "easyfile", 8)) )
    {
        printf("�ļ�ϵͳδ����������֮!\r\n");
        Lock_MutexPost(efs->block_buf_mutex);
        goto exit;
    }

    //����ļ�ϵͳ��Ϣ��������ֽ�CRC�Ƿ���ȷ
    if(!efs->drv->efs_read_media(efs->start_block + blocks - 1, efs->block_size-2,
            block_buf+fileInfoSize-2, 2,EF_WR_NOECC))
        goto fail;
    if(!efs->drv->efs_read_media(end_block + blocks - 1, efs->block_size-2,
            bakbuf+fileInfoSize-2, 2,EF_WR_NOECC))
        goto fail;

    crc16_check = pick_little_16bit(block_buf,(fileInfoSize/2)-1);
    if(crc16_check != CRC_16(block_buf,16))
        mainblockerr = 1;

    crc16_check = pick_little_16bit(bakbuf,(fileInfoSize/2)-1);
    if(crc16_check != CRC_16(bakbuf,16)) //bakУ���
        bakblockerr = 1;

    //���¼��������ȷ��
    loop = efs->start_block;

    switch((mainblockerr << 1) + (bakblockerr << 0))
    {
    case 0x03:      //����������У������������ļ�ϵͳ�Ѿ��𻵣�����
        Lock_MutexPost(efs->block_buf_mutex);
        goto exit;
        break;
    case 0x02:      //mainbuf error, bakУ����ȷ
        temp = __Efs_CheckSingleBuf(bakbuf, loop, efs);
        if (temp == 0)
        {
            crc16_check = CRC_16(bakbuf,16);
            fill_little_16bit((u8*)&temp,0,crc16_check);    //ת��ΪС��
            for (loop = efs->start_block; loop < end_block; loop++)
                    efs->drv->efs_erase_media(loop);       //��������Ȼ������д�������ļ�����;
            if(!efs->drv->efs_write_media(efs->start_block, 0, bakbuf, fileInfoSize, EF_WR_NOECC))
                goto fail;
            if(!efs->drv->efs_write_media(loop - 1, efs->block_size-2, (u8*)&temp, 2, EF_WR_NOECC))
                goto fail;
        }
        else if (temp == 1)
        {
            crc16_check = CRC_16(bakbuf,16);
            fill_little_16bit((u8*)&temp,0,crc16_check);    //ת��ΪС��
            for (loop = efs->start_block; loop < end_block; loop++)
                    efs->drv->efs_erase_media(loop);       //��������Ȼ������д�������ļ�����;
            if(!efs->drv->efs_write_media(efs->start_block, 0, bakbuf, fileInfoSize, EF_WR_NOECC))
                goto fail;
            if(!efs->drv->efs_write_media(loop - 1, efs->block_size-2, (u8*)&temp, 2, EF_WR_NOECC))
                goto fail;

            for (loop = efs->start_block; loop < end_block; loop++)
                    efs->drv->efs_erase_media(loop + blocks);       //����������Ȼ������д�������ļ�����;
            if(!efs->drv->efs_write_media(loop, 0, bakbuf, fileInfoSize, EF_WR_NOECC))
                goto fail;
            if(!efs->drv->efs_write_media(loop + blocks - 1, efs->block_size-2, (u8*)&temp, 2, EF_WR_NOECC))
                goto fail;
        }
        else
        {
            Lock_MutexPost(efs->block_buf_mutex);
            goto exit;
        }
        break;
    case 0x01:          //��У����ȷ, bakbuf error
        temp = __Efs_CheckSingleBuf(block_buf, loop,efs);
        if (temp == 0)
        {
            crc16_check = CRC_16(block_buf,16);
            fill_little_16bit((u8*)&temp,0,crc16_check);    //ת��ΪС��

            for (loop = efs->start_block; loop < end_block; loop++)
                    efs->drv->efs_erase_media(loop + blocks);       //����������Ȼ������д�������ļ�����;
            if(!efs->drv->efs_write_media(loop, 0, block_buf, fileInfoSize, EF_WR_NOECC))
                goto fail;
            if(!efs->drv->efs_write_media(loop + blocks - 1, efs->block_size-2, (u8*)&temp, 2, EF_WR_NOECC))
                goto fail;
        }
        else if (temp == 1)
        {
            crc16_check = CRC_16(block_buf,16);
            fill_little_16bit((u8*)&temp,0,crc16_check);    //ת��ΪС��
            for (loop = efs->start_block; loop < end_block; loop++)
                    efs->drv->efs_erase_media(loop);       //��������Ȼ������д�������ļ�����;
            if(!efs->drv->efs_write_media(efs->start_block, 0, block_buf, fileInfoSize, EF_WR_NOECC))
                goto fail;
            if(!efs->drv->efs_write_media(loop - 1, efs->block_size-2, (u8*)&temp, 2, EF_WR_NOECC))
                goto fail;

            for (loop = efs->start_block; loop < end_block; loop++)
                    efs->drv->efs_erase_media(loop + blocks);       //����������Ȼ������д�������ļ�����;
            if(!efs->drv->efs_write_media(loop, 0, block_buf, fileInfoSize, EF_WR_NOECC))
                goto fail;
            if(!efs->drv->efs_write_media(loop + blocks - 1, efs->block_size-2, (u8*)&temp, 2, EF_WR_NOECC))
                goto fail;
        }
        else
        {
            Lock_MutexPost(efs->block_buf_mutex);
            goto exit;
        }
        break;
    case 0x00:          //��/������ȫ��
        if(1 == __Efs_CheckBlock(block_buf,loop, bakbuf, loop+blocks, efs) )    //��������һ��
        {
            crc16_check = CRC_16(block_buf,16);
            fill_little_16bit((u8*)&temp,0,crc16_check);    //ת��ΪС��
            //�������Ϣ���filesize������д��flash
            for (loop = efs->start_block; loop < end_block; loop++)
                    efs->drv->efs_erase_media(loop);       //��������Ȼ������д�������ļ�����;
            if(!efs->drv->efs_write_media(efs->start_block, 0, block_buf, fileInfoSize, EF_WR_NOECC))
                goto fail;
            if(!efs->drv->efs_write_media(loop - 1, efs->block_size-2, (u8*)&temp, 2, EF_WR_NOECC))
                goto fail;

            for (loop = efs->start_block; loop < end_block; loop++)
                    efs->drv->efs_erase_media(loop + blocks);       //����������Ȼ������д�������ļ�����;
            if(!efs->drv->efs_write_media(loop, 0, block_buf, fileInfoSize, EF_WR_NOECC))
                goto fail;
            if(!efs->drv->efs_write_media(loop + blocks - 1, efs->block_size-2, (u8*)&temp, 2, EF_WR_NOECC))
                goto fail;
        }
        break;
    default:
        Lock_MutexPost(efs->block_buf_mutex);
        goto exit;
        break;
    }
    for(loop = 0; loop < CFG_EFS_MAX_OPEN_FILE_NUM; loop++)
    {
        index_offset = loop * Ram_file_info_len;
        if(!efs->drv->efs_read_media(efs->start_block, loop*EFS_ITEM_LIMIT, efs->file_list_buf + index_offset, EFS_ITEM_LIMIT,EF_WR_NOECC))       //���ļ������������
            goto fail;
        fill_little_32bit(efs->file_list_buf + index_offset + EFS_ITEM_LIMIT, 0, loop + 0);
    }
    s_pEfsList = efs;
    memcpy(s_pEfsList->name,pSuper->pTarget->name,EFS_NAME_LIMIT+1);
    Lock_MutexPost(efs->block_buf_mutex);
    free(bakbuf);
    free(block_buf);
//    pSuper->pCore = (void*)efs;
    efs->file_sys_install = true;
    return 0;
exit:
    free(bakbuf);
    free(block_buf);
    if(efs->install_options & MS_INSTALLCREAT)
    {
        if(Efs_Mkfs(efs,pSuper) == 0)//�豸�ϲ������ļ�ϵͳ�����½�
            return 0;
    }
    free(efs);
    return -1;
fail:
    free(bakbuf);
    free(block_buf);
    free(efs);
    Lock_MutexPost(efs->block_buf_mutex);
    return -1;
}
// ============================================================================
// ���ܣ���װ�ļ�ϵͳ��
// ������super -- �ļ�ϵͳ�ṹ��
//      opts -- ��װ��ʽ��
//      config -- �ļ�ϵͳ���ã�
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 e_install(struct FsCore *pSuper, u32 opts, void *config)
{
    struct EasyFS *core;

    dwFileMaxSize = CFG_EFS_FILE_SIZE_LIMIT; // �ļ���С����
    if(dwFileMaxSize > EFS_SUPPORT_MAXFILESIZE)
    {
        dwFileMaxSize = EFS_SUPPORT_MAXFILESIZE;
        warning_printf("efs","The file size exceeds the maximum file size supported by efs "
                             "and now forces the maximum file size to be 0x3fffffff");
    }
    Ram_file_info_len = EFS_RAM_ITEM_BYTE + EFS_ITEM_LIMIT;
    EfsCgfLimit = Ram_file_info_len * CFG_EFS_MAX_OPEN_FILE_NUM;  //efs,�ڻ����д��ļ���Ϣ�Ĵ�С
    core = EfsInfo(pSuper, opts);
    if(core == NULL)
    {
        free(core);
        return -1;
    }

    if(opts & MS_INSTALLUSE)
        return 0;
    else
        return Efs_Verify_Install(pSuper);
}
// ============================================================================
// ���ܣ���װEASY�ļ�ϵͳ
// ������target -- ��װĿ¼��
//      opt -- ��װѡ�
//      config -- �����߼�����ʱδʹ�ã���
// ���أ�ʧ��(-1)�� �ɹ�(0)��
// ��ע:
// ============================================================================
s32 ModuleInstall_EFS(const char *target, u32 opt, u32 config)
{
    struct Object * mountobj;
    static struct filesystem *typeEFS = NULL;
    s32 res;

    if(!target)
    {
        return (-1);
    }

    res = sizeof(*typeEFS);
    if(typeEFS == NULL)
    {
        typeEFS = malloc(res);

        typeEFS->fileOps = e_operations;
        typeEFS->install = e_install;
        typeEFS->pType = "EFS";
        typeEFS->format = Efs_Format;
        typeEFS->uninstall = NULL;
    }
    res = File_RegisterFs(typeEFS);
    if(-1==res)
    {
        printf("\r\n: dbug : module : cannot register \"EFS\"<file system type>.");
        return (-1); // ʧ��;
    }

    mountobj = OBJ_NewChild(OBJ_GetRoot(), __File_MountOps, 0, target); // ������װĿ¼
    if(NULL == mountobj)
    {
        printf("\r\n: dbug : module : mount \"EFS\" failed, cannot create \"%s\"<group point>.", target);
        return (-1);
    }
//    OBJ_DutyUp(mountobj);
    opt |= MS_DIRECTMOUNT;
//  opt |= CFG_EFS_INSTALL_OPTION_APPEND;
    res = File_Mount(NULL, target, "EFS", opt, (void *)config);
    if(res == -1)
    {
        printf("\r\n: dbug : module : mount \"%s\" failed, cannot install.", "EFS");
        OBJ_Delete(mountobj);
        return (-1);
    }

    return (0);
}
