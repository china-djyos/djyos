//-----------------------------------------------------------------------------
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
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������
// 3. ��������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ��������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <misc/misc.h>
#include <systime.h>
#include <dbug.h>
#include <project_config.h>
#include <djyfs/filesystems.h>
#include <device/djy_flash.h> // will be obsolete
#include <device/unit_media.h>
#include <xip.h>
#include "../w25qxx.h"

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern s32 ModuleInstall_W25qInstallXIP(const char *TargetFs,s32 bstart, s32 bend, u32 doformat);
//    ModuleInstall_W25qInstallXIP(CFG_W25Q_XIPFSMOUNT_NAME,CFG_W25Q_XIP_PART_START,
//                                              CFG_W25Q_XIP_PART_END, CFG_W25Q_XIP_PART_FORMAT);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"W25QXX XIP"//Ƭ��flash��װxip
//parent:"norflash W25QXX"//��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                         //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                      //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                        //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                       //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                        //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"device file system","norflash W25QXX"//������������������������none����ʾ�������������
                                        //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                        //����������������������г�
//weakdependence:"xip app file system","xip iboot file system"  //��������������������������none����ʾ�������������
                                        //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                        //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                        //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_W25Q_INSATALL_XIP == false )
//#warning  " w25q_insatall_xip  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_W25Q_INSATALL_XIP    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,-1,2048,
#define CFG_W25Q_XIP_PART_START      6          //������ʼ����д��ţ���Ŵ�0��ʼ����
#define CFG_W25Q_XIP_PART_END        -1         //����������-1��ʾ���һ�飬�����������6����ʼ����0����÷���ʹ�õĿ�Ϊ0��1��2��3��4��5��
//%$#@enum,true,false,
#define CFG_W25Q_XIP_PART_FORMAT     false      //����ѡ��,�Ƿ���Ҫ��ʽ���÷�����
//%$#@enum,"xip-app","xip-iboot",NULL
#define CFG_W25Q_XIPFSMOUNT_NAME   "xip-app"    //�谲װ���ļ�ϵͳ��mount�����֣�NULL��ʾ��flash�������ļ�ϵͳ
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure
// ============================================================================

// extern bool_t w25q_is_install(void);
// extern s32 __w25q_read(s64 unit, void *data, struct uopt opt);
// extern s32 __w25q_req(enum ucmd cmd, ptu32_t args, ...);
// extern s32 __w25q_write(s64 unit, void *data, struct uopt opt);
// extern s32 __w25q_erase(s64 unit, struct uesz sz);
// extern s32 __w25q_FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv);
s32 xip_w25q_write(struct __icore *core, u8 *data, u32 bytes, u32 pos);
s32 xip_w25q_read(struct __icore *core, u8 *data, u32 bytes, u32 pos);
s32 xip_w25q_erase(struct __icore *core, u32 bytes, u32 pos);


struct __xip_drv XIP_W25Q_DRV =
{
    .xip_erase_media = xip_w25q_erase,
    .xip_read_media = xip_w25q_read,
    .xip_write_media = xip_w25q_write
};
// ============================================================================
// ���ܣ�д����
// ������core -- xip�ļ�ϵͳ������Ϣ
//      data -- ���ݻ���
//      bytes -- д�ֽ��������ᳬ��һ�������С��
//      pos -- ���ݵ�ַ
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע����д�����һ��unitʱ���᳢�Բ���
// ============================================================================
s32 xip_w25q_write(struct __icore *core, u8 *data, u32 bytes, u32 pos)
{
    s32 res = -1; // RES_ERROR;

    if(W25QXX_Write(data, pos + (core->MStart * CFG_W25QXX_BYTES_PAGE), bytes))
    {
        res = 0;
    }
    return res;


//     struct umedia *um = (struct umedia *)core->vol;
//     struct uesz esz = {0};
//     struct uopt opt = {0};
//     u32 j, offset, once, more;
//     s32 left;
//     s64 unit;
//     u32 block = 0;

//     left = bytes;
//     unit = (pos / core->bufsz) + core->MStart;
//     offset = pos & (core->bufsz-1); // unit��ƫ��
//     __w25q_req(lock, CN_TIMEOUT_FOREVER);
//     while(left>0)
//     {
// #if 0
//         // �����ǰд��ҳ��һ�����е����һҳ����Ԥ��ɾ��������sector
//         // (page+1)���ڷ�ֹ��ʽ���˲�����xip�Ŀռ�
//         __w25q_req(remain, (ptu32_t)&more, &unit);
//         if(!more)
//         {
//             // +1�Ǳ�ʾ��ǰunit�ĺ���һ��
//             if(((unit-um->ustart+1)<<um->usz)<um->asz)
//             {
//                 struct uesz sz = {0};
//                 sz.unit = 1;
//                 __w25q_erase(unit+1, sz); // ������û�в����ɹ�����Ϊ�������д��Ļ������лض�У��
//             }
//         }
// #endif
//         if(__w25q_read(unit, um->ubuf, opt))
//         {
//             __w25q_req(unlock, 0); //
//             return (-1);
//         }

//         once = core->bufsz;
//         if(left<(s32)once)
//             once = left;

//         for(j=0; j<(once); j++)
//         {
//             if(um->ubuf[j+offset]!=data[j])
//             {
//                 if(0xFF!=um->ubuf[j+offset])        //�жϵ�ǰҳ�Ƿ�ΪFF
//                 {
//                     __w25q_req(unlock, 0);
//                     return (-1);
//                 }
//             }
//         }

//         memcpy((um->ubuf + offset), data, once);
//         if(__w25q_write(unit, um->ubuf, opt))
//         {
//             __w25q_req(unlock, 0);
//             return (-1);
//         }

//         left -= once;
//         offset = 0;
//         data += once;
//         if(left)
//             unit++;
//     }

//     // �����ǰд��ҳ��һ�����е����һҳ����Ԥ��ɾ��������sector
//     // (page+1)���ڷ�ֹ��ʽ���˲�����xip�Ŀռ�
//     __w25q_req(remain, (ptu32_t)&more, &unit);
//     if(!more)
//     {
//         // +1�Ǳ�ʾ��ǰunit�ĺ���һ��
//         if(((unit-core->MStart+1)* core->bufsz) >= core->ASize)
//         {
//             return (-2);
//         }
//         esz.block = 1;
// //        esz->unit = 0;
//         __w25q_req(whichblock, (ptu32_t)&block, &unit);
//         //block�ǵ�ǰҳ���ڵĿ�ţ�block+1��Ϊ�˲�����һ���飨block+1��Ҫ�����Ŀ飬����block+1+1��Ͳ����ˣ�
//         __w25q_req(format, block+1, block+1+1, &esz);
//     }

//     __w25q_req(unlock, 0); //
//     return (0);
}

// ============================================================================
// ���ܣ�������
// ������core -- xip�ļ�ϵͳ������Ϣ
//      data -- ���ݻ���
//      bytes -- ���ֽ���
//      pos -- ���ݵ�ַ
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע������Ҳ��ֻ���һ��unit��Ŀǰ
// ============================================================================
s32 xip_w25q_read(struct __icore *core, u8 *data, u32 bytes, u32 pos)
{
    s32 res = -1; // RES_ERROR;

    if(W25QXX_Read(data, pos + (core->MStart * CFG_W25QXX_BYTES_PAGE), bytes))
    {
        res = 0;
    }
    return res;

    // struct umedia *um = (struct umedia *)core->vol;
    // struct uopt opt = {0};
    // s64 unit;
    // u32 offset;
    // s32 left = bytes, once;

    // unit = (pos / core->bufsz) + core->MStart;
    // offset = pos & (core->bufsz - 1); // unit��ƫ��
    // __w25q_req(lock, CN_TIMEOUT_FOREVER);
    // while(left>0)
    // {
    //     once = MIN((core->bufsz - offset), left);
    //     if(__w25q_read(unit, um->ubuf, opt))
    //     {
    //         __w25q_req(unlock, 0); //
    //         return (-1);
    //     }

    //     memcpy(data, (um->ubuf + offset), once);
    //     left -= once;
    //     offset = 0;
    //     unit++;
    //     data += once;
    // }

    // __w25q_req(unlock, 0); //
    // return (0);
}

// ============================================================================
// ���ܣ���������
// ������core -- xip�ļ�ϵͳ������Ϣ
//       bytes -- �ֽ���
//       pos -- ���ݵ�ַ
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 xip_w25q_erase(struct __icore *core, u32 bytes, u32 pos)
{
    u32 addr = pos + (core->MStart * CFG_W25QXX_BYTES_PAGE);
    s32 res = 0;
    s32 len = (s32)bytes;

    while (len > 0)
    {
        if (false == W25QXX_EraseSector(addr))
        {
            res = -1;
            break;
        }
        addr += CFG_W25QXX_BYTES_PAGE * CFG_W25QXX_PAGES_SECTOR;
        len -= CFG_W25QXX_BYTES_PAGE * CFG_W25QXX_PAGES_SECTOR;
    }
    

    // struct uesz esz = {0};
    // s64 unit;
    // u32 erases, offset;
    // s32 left = bytes;

    // esz.unit = 1;
    // unit = (pos / core->bufsz) + core->MStart;
    // offset = pos & (core->bufsz-1); // unit��ƫ��
    // __w25q_req(lock, CN_TIMEOUT_FOREVER);
    // while(left>0)
    // {
    //     if(__w25q_req(remain, (ptu32_t)&erases, (ptu32_t)&unit))       //��ȡ��ǰҳ�����ڿ��е�λ��
    //     {
    //         printf("\r\n: erro : xipfs  : erase unit %lld failed, cannot get remain.", unit);
    //         return (-1);
    //     }

    //     if(__w25q_erase(unit, esz))
    //     {
    //         printf("\r\n: erro : xipfs  : erase unit %lld failed, with erases %d.", unit, erases);
    //         return (-1);
    //     }

    //     erases++; // ������һ����ʾ������ǰ��unit
    //     left -= ((erases * core->bufsz) - offset);
    //     unit += erases;     //��erases������һ��ĵ�һҳ
    //     offset = 0;
    // }

    return (res);
}


// ============================================================================
// ���ܣ���ʽ��xip�ļ�ϵͳ
// ������core -- xip�ļ�ϵͳ������Ϣ
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 xip_fs_format(void *core)
{
    struct uesz sz;
    sz.unit = 0;
    sz.block = 1;
    if(-1 == __W25qxx_Req(format, CFG_W25Q_XIP_PART_START , CFG_W25Q_XIP_PART_END, &sz))
    {
        warning_printf("w25q_xip"," Format failure.\r\n");
    }
    return (0);
}

//-----------------------------------------------------------------------------
// ���ܣ���װƬ��Flash����
// ������TargetFs -- Ҫ���ص��ļ�ϵͳ
//      �������� -- ��ʼ�飬���������������������6����ʼ����0����÷���ʹ�õĿ�Ϊ0��1��2��3��4��5�飬�����������飩���Ƿ��ʽ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע���������֪��Ҫ��װʲô�ļ�ϵͳ�����߲���װ�ļ�ϵͳTargetFs��NULL��TargetPart��-1��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_W25qInstallXIP(const char *TargetFs,s32 bstart, s32 bend, u32 doformat)
{
    struct FsCore *super;
    char *notfind;
    struct Object *targetobj;
    if(W25qxx_is_install() == true)
    {
        if((TargetFs != NULL) && (bstart != bend))
        {
            if(doformat)
            {
                struct uesz sz;
                sz.unit = 0;
                sz.block = 1;
                if(-1 == __W25qxx_Req(format, bstart , bend, &sz))
                {
                    warning_printf("w25q_xip"," Format failure.\r\n");
                }
            }
            targetobj = OBJ_MatchPath(TargetFs, &notfind);
            if(notfind)
            {
                error_printf("w25q_xip"," not found need to install file system.\r\n");
                return false;
            }
            super = (struct FsCore *)OBJ_GetPrivate(targetobj);
            if((strcmp(super->pFsType->pType, "XIP-APP") == 0) || (strcmp(super->pFsType->pType, "XIP-IBOOT") == 0))
            {
                if(__w25qxx_FsInstallInit(TargetFs,bstart,bend,&XIP_W25Q_DRV) == 0)
                    return true;
            }
            error_printf("w25q_xip"," need to install file system not XIP.\r\n");
        }
    }
    return false;
}