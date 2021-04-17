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

#include <string.h>
#include <stdlib.h>
#include <cpu_peri.h>
#include <djyos.h>
#include <device.h>
#include <device/djy_flash.h>
#include <spibus.h>
#include <djyfs/filesystems.h>
#include <dbug.h>
#include <device/nand.h>
#include <gd5f1gq5x.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern s32 ModuleInstall_Gd5f1g(void);
//    ModuleInstall_Gd5f1g();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"gd5f1gq5x"	//SPI��QSPI�ӿڵ�flashоƬ
//parent:"none"//��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"cpu onchip qspi",//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"        //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_NORFLASH_GD5F1G == false )
//#warning  " nandflash_GD5F1G  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_NORFLASH_GD5F1G    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@string,1,20,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

								
								

#define ECC_ENABLE_SPARE_WRITE_LEN          64          //Ĭ����ʹ����оƬ��Ӳ��ECC�ģ���64�ֽ�����Ӳ��ECC�ˣ�������������

struct MutexLCB *gd5f1g_lock;           //оƬ������ʱ���
struct umedia *nand_umedia = NULL;
struct NandDescr __nandescription; // NAND��������

static u32 *bads_table = NULL;
static s32 bads_location = 0;
static bool_t is_bads_table = false;
static bool_t Gd5f1gInited = false;
static const char *NandFlashName = "nand";      //��flash��obj�ڵ�����
static u8 *Spare_BadChk = NULL;
static u8 *Spare_BadMark = NULL;

extern struct Object *s_ptDeviceRoot;
extern s32 deonfi(const struct NandONFI *data, struct NandDescr *onfi, u8 little);

s32 Gd5f1g_Ctrl(enum ucmd cmd, ptu32_t args, ...);
//-----------------------------------------------------------------------------
//����: ��״̬�Ĵ���
//����: regno��״̬�Ĵ������
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
u8 Gd5f1g_ReadSR(enum GetFeatures regno, u8 *sta)
{
    u8 command = 0;
    switch(regno)
    {
        case Get_Protection:
            command=Gd5f1g_RdFeaturesReg_Protection;
            break;
        case Get_Feature1:
            command=Gd5f1g_RdFeaturesReg_Feature1;
            break;
        case Get_Status1:
            command=Gd5f1g_RdFeaturesReg_Status1;
            break;
        case Get_Feature2:
            command=Gd5f1g_RdFeaturesReg_Feature2;
            break;
        case Get_Status2:
            command=Gd5f1g_RdFeaturesReg_Status2;
            break;
        default:
            command=Gd5f1g_RdFeaturesReg_Feature1;
            break;
    }
    if(QSPI_Send_CMD(Gd5f1g_ReadFeaturesReg,command,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_8_BITS,QSPI_DATA_1_LINE) == false)
        return false;

    return QSPI_Receive(sta, 1);
}

//-----------------------------------------------------------------------------
//����: д״̬�Ĵ���
//����: regno��״̬�Ĵ�����ţ�data: ��д�������
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
u8 Gd5f1g_WriteSR(enum SetFeatures regno, u8 data)
{
    u8 command = 0;
    switch(regno)
    {
        case Set_Protection:
            command=Gd5f1g_WrFeaturesReg_Protection;
            break;
        case Set_Feature1:
            command=Gd5f1g_WrFeaturesReg_Feature1;
            break;
        case Set_Feature2:
            command=Gd5f1g_WrFeaturesReg_Feature2;
            break;
        default:
            command=Gd5f1g_WrFeaturesReg_Feature1;
            break;
    }

    if(QSPI_Send_CMD(Gd5f1g_WriteFeaturesReg,command,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_8_BITS,QSPI_DATA_1_LINE) == false)
        return false;
    return QSPI_Transmit(&data, 1);
}

//-----------------------------------------------------------------------------
//����: �ȴ�����
//����: ��
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t Gd5f1g_WaitBusy(u32 timeout)
{
    bool_t ret = true;
    u32 time = 0;
    u8 sta = 0;
    if(Gd5f1g_ReadSR(Get_Status1, &sta) == false)
        return false;
    while((sta & 0x01) == 0x01)   //�ȴ�����
    {
        DJY_EventDelay(100);
        time ++;
        if(time > timeout)
        {
            ret = false;
            error_printf("W25q","Wait busy timeput.\r\n");
            break;
        }
        if(Gd5f1g_ReadSR(Get_Status1, &sta) == false)
            return false;
    }
    return ret;
}

//-----------------------------------------------------------------------------
//����: дʹ�ܵȴ�
//����: ��
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t Gd5f1g_WriteEnableWait(void)
{
    u8 sta = 0;
    u32 wait = 40;
    if(Gd5f1g_ReadSR(Get_Status1, &sta))
    {
        while((sta & 2) != 2)
        {
            wait --;
            if(wait == 0)
                return false;
            sta = 0;
            Gd5f1g_ReadSR(Get_Status1, &sta);
        }
    }

    return true;
}

//-----------------------------------------------------------------------------
//����: дʹ��
//����: ��
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t Gd5f1g_SetWriteEnable(void)
{
    if(QSPI_Send_CMD(Gd5f1g_WriteEnable,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE) == false)
        return false;

    return Gd5f1g_WriteEnableWait();
}


//-----------------------------------------------------------------------------
//����: ��������
//����: addr��������ַ
//����: 0 -- �ɹ�; -1 -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
s32 Gd5f1g_EraseBlock(u32 block_no)
{
    u8 sta = 0;

    if(block_no >= (__nandescription.BlksPerLUN * __nandescription.LUNs))
    {
        error_printf("gd5f1g", "erase : block_no = %d. exceed max limit\r\n", block_no);
        return (-1);
    }

    block_no = block_no << 6;
    Lock_MutexPend(gd5f1g_lock,CN_TIMEOUT_FOREVER);

    if(Gd5f1g_WaitBusy(5000) == false)
    {
        Lock_MutexPost(gd5f1g_lock);
        return -1;
    }

    if(Gd5f1g_SetWriteEnable())
    {
        if(QSPI_Send_CMD(Gd5f1g_BlockErase,block_no,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_24_BITS,QSPI_DATA_NONE))
        {
            if(Gd5f1g_WaitBusy(5000) == false)
            {
                Lock_MutexPost(gd5f1g_lock);
                return -1;
            }
            if(Gd5f1g_ReadSR(Get_Status1, &sta))
            {
                if((sta & 4) == 4)
                {
                    Lock_MutexPost(gd5f1g_lock);
                    return -1;
                }
                else
                {
                    Lock_MutexPost(gd5f1g_lock);
                    return 0;
                }
            }
        }
    }
    Lock_MutexPost(gd5f1g_lock);
    return -1;
}

//-----------------------------------------------------------------------------
//����: ��flashҳ�ı�����
//����: buf�����ݻ�������PageNo�����ĵ�ַ��len����ȡ���ֽ���
//����: �������ֽ���
//��ע:
//-----------------------------------------------------------------------------
u32 Gd5f1g_ReadSpare(u32 PageNo, u8* buf)
{
    u8 ecc_1 = 0;
    s32 res = 0;

    s64 t1,t2,t3;

    t1 = DJY_GetSysTime();
    if((buf == NULL) || (PageNo >= (__nandescription.BlksPerLUN * __nandescription.LUNs * __nandescription.PagesPerBlk)))
    {
        error_printf("gd5f1g", "read spare param fail. buf = %x, PageNo =%d.\r\n", buf, PageNo);
        return res;
    }
    Lock_MutexPend(gd5f1g_lock,CN_TIMEOUT_FOREVER);
    t2 = DJY_GetSysTime();
    t3 = t2 - t1;

    t1 = DJY_GetSysTime();
    if(Gd5f1g_WaitBusy(5000))
    {
        t2 = DJY_GetSysTime();
        t3 = t2 - t1;

        t1 = DJY_GetSysTime();
        if(QSPI_Send_CMD(Gd5f1g_ReadData,PageNo,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_24_BITS,QSPI_DATA_NONE))
        {
            t2 = DJY_GetSysTime();
            t3 = t2 - t1;

            t1 = DJY_GetSysTime();
            if(Gd5f1g_WaitBusy(5000))
            {
                t2 = DJY_GetSysTime();
                t3 = t2 - t1;

                t1 = DJY_GetSysTime();
                //__nandescription.BytesPerPage << 8,������˵�ǲ�Ҫ����8λ�ģ�������Ϊ��ַ������Ҳ��12λд����24λ�����������ڵ�����8λ������
                if(QSPI_Send_CMD(Gd5f1g_ReaddaraCachex4, __nandescription.BytesPerPage << 8,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_24_BITS,QSPI_DATA_4_LINES))
                {
                    t2 = DJY_GetSysTime();
                    t3 = t2 - t1;

                    t1 = DJY_GetSysTime();
                    if(QSPI_Receive(buf, __nandescription.OOB_Size))
                    {
                        t2 = DJY_GetSysTime();
                        t3 = t2 - t1;

                        t1 = DJY_GetSysTime();
                        if(Gd5f1g_ReadSR(Get_Status1, &ecc_1))
                        {
                            t2 = DJY_GetSysTime();
                            t3 = t2 - t1;
                            ecc_1 = ecc_1 >> 4;
                            ecc_1 &= 3;
                            if(ecc_1 != 2)
                                res = __nandescription.OOB_Size;
                        }
                    }
                }
            }
        }
    }
    Lock_MutexPost(gd5f1g_lock);
    return res;
}

//-----------------------------------------------------------------------------
//����: дflash��ҳ������
//����: buf�����ݻ�������addr��д�ĵ�ַ��len��д���ֽ���
//����: д��ȥ���ֽ���
//��ע:
//-----------------------------------------------------------------------------
u32 Gd5f1g_WriteSpare(u32 PageNo, u8* buf)
{
    u8 sta = 0;
    s32 res = 0;

    if((buf == NULL) || (PageNo >= (__nandescription.BlksPerLUN * __nandescription.LUNs * __nandescription.PagesPerBlk)))
    {
        error_printf("gd5f1g", "write spare param fail. buf = %x, PageNo =%d.\r\n", buf, PageNo);
        return res;
    }
    Lock_MutexPend(gd5f1g_lock,CN_TIMEOUT_FOREVER);

    if(Gd5f1g_WaitBusy(5000))
    {
        if(QSPI_Send_CMD(Gd5f1g_PageProgramx4,__nandescription.BytesPerPage, 0, QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_16_BITS,QSPI_DATA_4_LINES))
        {
            if(QSPI_Transmit(buf, __nandescription.OOB_Size))
            {
                if(Gd5f1g_SetWriteEnable())
                {
                    if(QSPI_Send_CMD(Gd5f1g_PageProgramExecute,PageNo,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_24_BITS,QSPI_DATA_NONE))
                    {
                        if(Gd5f1g_WaitBusy(5000))
                        {
                            if(Gd5f1g_ReadSR(Get_Status1, &sta) == true)
                            {
                                if((sta & 8) != 8)
                                    res = __nandescription.OOB_Size;
                            }
                        }
                    }
                }
            }
        }
    }
    Lock_MutexPost(gd5f1g_lock);
    return res;
}

//-----------------------------------------------------------------------------
//����: ��flashҳ
//����: buf�����ݻ�������PageNo�����ĵ�ַ��len����ȡ���ֽ���
//����: �������ֽ���;
//��ע:
//-----------------------------------------------------------------------------
u32 Gd5f1g_ReadPage(u32 PageNo, u8* buf, u32 Flags)
{
    u8 ecc_1 = 0;
    s32 res = 0;
    u8 ecc = 0;
    u32 len = 0;

    if((buf == NULL) || (PageNo >= (__nandescription.BlksPerLUN * __nandescription.LUNs * __nandescription.PagesPerBlk)))
    {
        error_printf("gd5f1g", "read page param fail. buf = %x, PageNo = %d.\r\n", buf, PageNo);
        return res;
    }

    Lock_MutexPend(gd5f1g_lock,CN_TIMEOUT_FOREVER);

    switch (Flags & MASK_ECC)
    {
        case HW_ECC :
            ecc = 1;
            break;
        case SW_ECC :
        case NO_ECC :
            ecc = 0;
            break;
        default :
            ecc = 0;
            break;
    }

    if(Gd5f1g_WaitBusy(5000))
    {
        //QSPI�����ٶ����ݣ���ַΪaddr��4�ߴ������ݣ�32λ��ַ��4�ߴ����ַ��4�ߴ���ָ�2�������ڣ�
        if(QSPI_Send_CMD(Gd5f1g_ReadData,PageNo,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_24_BITS,QSPI_DATA_NONE))
        {
            if(Gd5f1g_WaitBusy(5000))
            {
                if(QSPI_Send_CMD(Gd5f1g_ReaddaraCachex4,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_24_BITS,QSPI_DATA_4_LINES))
                {
                    if(SPARE_REQ & Flags)
                        len = __nandescription.BytesPerPage + __nandescription.OOB_Size;
                    else
                        len = __nandescription.BytesPerPage;
                    if(QSPI_Receive(buf, len))
                    {
                        if((ecc) && (Gd5f1g_ReadSR(Get_Status1, &ecc_1)))
                        {
                            ecc_1 = ecc_1 >> 4;
                            ecc_1 &= 3;
                            if(ecc_1 != 2)
                                res = len;
                        }
                        else
                            res = len;
                    }
                }
            }
        }
    }
    Lock_MutexPost(gd5f1g_lock);
    return res;
}

//-----------------------------------------------------------------------------
//����: дһҳflash
//����: buf�����ݻ�������addr��д�ĵ�ַ��len��д���ֽ���
//����: д���ֽ���;
//��ע:
//-----------------------------------------------------------------------------
u32 Gd5f1g_WritePage(u32 PageNo, u8* buf, u32 Flags)
{
    u8 sta = 0;
    s32 res = 0;
    u32 len = 0;
    if((buf == NULL) || (PageNo >= (__nandescription.BlksPerLUN * __nandescription.LUNs * __nandescription.PagesPerBlk)))
    {
        error_printf("gd5f1g", "write page param fail. buf = %x, PageNo = %d.\r\n", buf, PageNo);
        return res;
    }

    Lock_MutexPend(gd5f1g_lock,CN_TIMEOUT_FOREVER);
    if(Gd5f1g_WaitBusy(5000))
    {
        if(QSPI_Send_CMD(Gd5f1g_PageProgramx4,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_16_BITS,QSPI_DATA_4_LINES))
        {
            if(SPARE_REQ & Flags)
                len = __nandescription.BytesPerPage + __nandescription.OOB_Size;
            else
                len = __nandescription.BytesPerPage;
            if(QSPI_Transmit(buf, len))
            {
                if(Gd5f1g_SetWriteEnable())
                {
                    //QSPI��ҳд����ַΪaddr��4�ߴ������ݣ�32λ��ַ��4�ߴ����ַ��4�ߴ���ָ�0�������ڣ�
                    if(QSPI_Send_CMD(Gd5f1g_PageProgramExecute,PageNo,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_24_BITS,QSPI_DATA_NONE))
                    {
                        if(Gd5f1g_WaitBusy(5000))
                        {
                            if(Gd5f1g_ReadSR(Get_Status1, &sta) == true)
                            {
                                if((sta & 8) != 8)
                                    res = len;
                            }
                        }
                    }
                }
            }
        }
    }
    Lock_MutexPost(gd5f1g_lock);
    return res;
}

//-----------------------------------------------------------------------------
//����: ���������Ķ�
// ������unit -- ������ţ�ҳ����
//      data -- �������ݣ�
//      opt -- ���ķ�ʽ��
//����: 0 -- �ɹ�; -1 -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
s32 Gd5f1g_ReadPage_BadBlk(s32 page, void *data, struct uopt opt)
{
    u32 flags = 0;
    u32 res;
    s64 offset_page = (s64)page;

    if(bads_table == NULL)
    {
        error_printf("gd5f1g", "read bads_table = NULL\r\n");
        return (-1);
    }

    if(data == NULL)
    {
        error_printf("gd5f1g", "ReadPage_BadBlk param fail data = %x.\r\n",data);
        return (-1);
    }

    nandbadfreeunit(bads_table, &offset_page, Gd5f1g_Ctrl);

    if(offset_page >= (__nandescription.BlksPerLUN * __nandescription.LUNs * __nandescription.PagesPerBlk))
    {
        error_printf("gd5f1g", "read :offset_page = %d. exceed max limit\r\n", offset_page);
        return (-1);
    }

    if(opt.hecc)
        flags |= HW_ECC;
    else if(opt.secc)
        flags |= SW_ECC;
    else
        flags |= NO_ECC;

    if(opt.main)
    {
        if(opt.spare)
            flags |= SPARE_REQ;

        res = Gd5f1g_ReadPage((u32)offset_page, (u8*)data, flags);
        if (!((SPARE_REQ & flags) || (HW_ECC & flags)))
        {
            if(res != __nandescription.BytesPerPage)
            {
                error_printf("gd5f1g", "read fail\r\n");
                return (-1);
            }
        }
        else
        {
            if(res != (__nandescription.BytesPerPage + __nandescription.OOB_Size))
            {
                error_printf("gd5f1g", "read fail and OOB\r\n");
                return (-1);
            }
        }
    }
    else
    {
        res = Gd5f1g_ReadSpare((u32)offset_page, (u8*)data);
        if(res != __nandescription.OOB_Size)
        {
            error_printf("gd5f1g", "read OOB fail.\r\n");
            return (-1);
        }
    }

    return (0);
}

// ============================================================================
// ���ܣ�����������д��
// ������unit -- д����ţ�ҳ����
//      data -- д�����ݣ�
//      opt -- д�ķ�ʽ��
// ���أ��ɹ� -- ��0����ʧ�� -- ��-1��
// ���أ�
// ��ע��
// ============================================================================
s32 Gd5f1g_WritePage_BadBlk(s32 page, void *data, struct uopt opt)
{
    u32 flags = 0;
    u32 res;
    s64 offset_page = (s64)page;

    if(bads_table == NULL)
    {
        error_printf("gd5f1g", "write bads_table = NULL\r\n");
        return (-1);
    }

    if(data == NULL)
    {
        error_printf("gd5f1g", "WritePage_BadBlk param fail data = %x.\r\n",data);
        return (-1);
    }

    nandbadfreeunit(bads_table, &offset_page, Gd5f1g_Ctrl);

    if(offset_page >= (__nandescription.BlksPerLUN * __nandescription.LUNs * __nandescription.PagesPerBlk))
    {
        error_printf("gd5f1g", "write : offset_page = %d. exceed max limit\r\n", offset_page);
        return (-1);
    }

    if(opt.hecc)
        flags |= HW_ECC;
    else if(opt.secc)
        flags |= SW_ECC;
    else
        flags |= NO_ECC;

    if(opt.main)
    {
        if(opt.spare)
            flags |= SPARE_REQ;

        res = Gd5f1g_WritePage((u32)offset_page, (u8*)data, flags);
        if (!((SPARE_REQ & flags) || (HW_ECC & flags)))
        {
            if(res != (__nandescription.BytesPerPage))
            {
                error_printf("gd5f1g", "write fail\r\n");
                return (-1);
            }
        }
        else
        {
            if(res != (__nandescription.BytesPerPage + __nandescription.OOB_Size))
            {
                error_printf("gd5f1g", "write fail and OOB\r\n");
                return (-1);
            }
        }
    }
    else
    {
        warning_printf("gd5f1g", "opt.main = %d, opt.spare =%d.\r\n",opt.main,opt.spare);
    }

    return (0);
}

// ============================================================================
// ���ܣ����������Ĳ���
// ������unit -- ��������ţ�
//      sz -- �����ĵ�λ��unit��block��
// ���أ��ɹ� -- ��0����ʧ�� -- ��-1��
// ��ע��
// ============================================================================
s32 Gd5f1g_Erase_BadBlk(s32 block)
{
    u32 offset_block = (u32)block;
    if(bads_table == NULL)
    {
        error_printf("gd5f1g", "erase bads_table = NULL\r\n");
        return (-1);
    }

    nandbadfreeblock(bads_table, &offset_block, Gd5f1g_Ctrl);

    if(offset_block >= (__nandescription.BlksPerLUN * __nandescription.LUNs))
    {
        error_printf("gd5f1g", "erase : offset_block = %d. exceed max limit\r\n", offset_block);
        return (-1);
    }

    return (Gd5f1g_EraseBlock(offset_block));
}

//-----------------------------------------------------------------------------
//����: Gd5f1g_Qspiʹ��
//����: ��
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t Gd5f1g_QspiEnable(void)
{
    u8 sta2;
    bool_t ret = true;

    if(Gd5f1g_ReadSR(Get_Feature1, &sta2) == false)       //��״̬�Ĵ���2��ֵ
        return false;
    if((sta2 & 0X01)==0)           //QEλδʹ��
    {
        sta2 |= 1<<0;              //ʹ��QEλ
        if(Gd5f1g_WriteSR(Set_Feature1, sta2) == false) //д״̬�Ĵ���2
            ret = false;

        sta2 = 0;
        if(Gd5f1g_ReadSR(Get_Feature1, &sta2))       //��״̬�Ĵ���2��ֵ
        {
            if((sta2 & 0X01)==0)           //QEλδʹ��
                ret = false;
        }
        else
            ret = false;
    }
    return ret;
}

//-----------------------------------------------------------------------------
//����: ��ID
//����: ��
//����: оƬID
//��ע:
//-----------------------------------------------------------------------------
u16 Gd5f1g_ReadID(void)
{
    u8 temp[2];
    u16 deviceid;
    bool_t ret = false;

    if(Gd5f1g_WaitBusy(5000) == false)
    {
        return false;
    }

//    if(W25QXX_QPI_MODE)
//    {
//        //QSPI����ַλ0��4�ߴ������ݣ�24λ��ַ��4�ߴ����ַ��4�ߴ���ָ��޿����ڣ�2�ֽ�����
//        ret = QSPI_Send_CMD(W25X_ManufactDeviceID,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_24_BITS,QSPI_DATA_4_LINES);
//    }
//    else
//    {
        //SPI����ַλ0�����ߴ������ݣ�24λ��ַ�����ߴ����ַ�����ߴ���ָ��޿����ڣ�2�ֽ�����
        ret = QSPI_Send_CMD(Gd5f1g_DeviceID,0,8,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_1_LINE);
//    }
    if(ret)
    {
        QSPI_Receive(temp,2);
        deviceid=(temp[0] << 8) | temp[1];
    }
    else
        deviceid = 0;
    return deviceid;
}

static bool_t Gd5f1g_ReadParamPage(struct NandONFI *onfi)
{
    u8 sta;
    bool_t res = false;
    if(Gd5f1g_WaitBusy(5000))
    {
        if(Gd5f1g_ReadSR(Get_Feature1, &sta))
        {
            if(!(sta & (1 << 6)))
            {
                sta |= 1 << 6;
                if(Gd5f1g_WriteSR(Get_Feature1, sta))
                {   sta = 0;
                    Gd5f1g_ReadSR(Get_Feature1, &sta);
                }
            }

            if(sta & (1 << 6))
            {
                if(QSPI_Send_CMD(Gd5f1g_ReadData,ParameterPage_Addr,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_24_BITS,QSPI_DATA_NONE))
                {
                    if(Gd5f1g_WaitBusy(5000))
                    {
                        if(QSPI_Send_CMD(Gd5f1g_ReaddaraCache,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_24_BITS,QSPI_DATA_1_LINE))
                        {
                            if(QSPI_Receive((u8 *)onfi, sizeof(struct NandONFI)))
                            {
                                res = true;
                            }
                        }
                    }
                }
                sta &= ~(1 << 6);
                if(Gd5f1g_WriteSR(Get_Feature1, sta))
                {
                    if(Gd5f1g_ReadSR(Get_Feature1, &sta))
                    {
                        if(sta & (1 << 6))
                            res = false;
                    }
                }
            }
        }
    }

    return res;
}

//-----------------------------------------------------------------------------
//����: ��ǻ���
//����:
//����: "0" -- �ɹ�;
//      "-1" -- ʧ��;
//      "-2" -- �ڴ治��;
//��ע:
//-----------------------------------------------------------------------------
static s32 Gd5f1g_BadMark(u32 BlkNo)
{
    //�߼�:
    //    ����BAD MARKλԭ���Ƕ����Լ��Ƿ���ECCУ������, ֻ�ܽ���λ��־Ϊ����

    u8 i;
    s32 Ret = 0;
    u32 PageNo = BlkNo * __nandescription.PagesPerBlk;

    memset(Spare_BadMark, 0xFF, __nandescription.OOB_Size);
    Spare_BadMark[__nandescription.BadMarkOffset] = 0xAA;
    Spare_BadMark[__nandescription.BadMarkOffset-1] = 0xAA;

    for (i = 0; i < 2; i++)
    {
        if(0 == Gd5f1g_WriteSpare(PageNo + i, Spare_BadMark))
            Ret = -1;
    }
    return (Ret);
}

//-----------------------------------------------------------------------------
//����:����ĺû�
//����:���
//����: "0" -- �ÿ�;
//      "-1" -- ����;
//      "-2" -- �ڴ治��
//      "-3" -- ��ʧ��;
//��ע:
//-----------------------------------------------------------------------------
static s32 Gd5f1g_BadChk(u32 BlkNo)
{
    u8 i;
    s32 Ret = 0,ReadState = 0;
    u32 PageNo = BlkNo * __nandescription.PagesPerBlk;

    if(is_bads_table)
    {
        for (i = 0; i < 2; i++)// ֻ�ж�ÿ�������ҳ
        {
            ReadState = Gd5f1g_ReadSpare(PageNo + i, Spare_BadChk);
            if(ReadState != __nandescription.OOB_Size)
            {
                Ret = -3;
                break;
            }
            if ((0xAA == Spare_BadChk[__nandescription.BadMarkOffset]) &&
                (0xAA == Spare_BadChk[__nandescription.BadMarkOffset-1]))
            {
                Ret = -1;
                break;
            }
        }
    }
    else
    {
        if(Gd5f1g_ReadSpare(PageNo, Spare_BadChk) != 0)
        {
            for(i = 0; i < __nandescription.OOB_Size; i++)
            {
                if(Spare_BadChk[i] != 0xff)
                    break;
            }
            if(i != __nandescription.OOB_Size)
            {
                Gd5f1g_BadMark(BlkNo);
                Ret = -1;
            }
        }
    }
    return (Ret);
}

// ============================================================================
// ���ܣ�nand ����
// ������ucmd -- ���
//      ���� -- ���������
// ���أ����ݲ�ͬ������в�ͬ�ķ���ֵ
// ��ע��
// ============================================================================
s32 Gd5f1g_Ctrl(enum ucmd cmd, ptu32_t args, ...)
{
    s32 res = 0;

    switch(cmd)
    {
        case whichblock:
        {
            va_list list;
            u32 *block;
            s64 unit;

            block = (u32*)args;
            va_start(list, args);
            unit = (s64)va_arg(list, u32);
            va_end(list);
            *block = unit / __nandescription.PagesPerBlk;
            break;
        }

        case totalblocks:
        {
            *((u32*)args) =  __nandescription.BlksPerLUN * __nandescription.LUNs;
            break;
        }

        case blockunits:
        {
            *((u32*)args) = __nandescription.PagesPerBlk;
            break;
        }

        case unitbytes:
        {
            *((u32*)args) = __nandescription.BytesPerPage;
            break;
        }
        case sparebytes:
        {
            *((u32*)args) = __nandescription.OOB_Size;
            break;
        }

        case checkbad:
        {
            if(bads_location == (s32)args)
                res = 1;
            else
                res = Gd5f1g_BadChk((u32)args); // args = block

            break;
        }

        case savebads:
        {
            if(0 == Gd5f1g_WritePage(bads_location, (void*)args, 0)) // ���������ڵ�0ҳ
                res = -1;

            break;
        }

        case getbads:
        {
            u32 **table = (u32**)args;
            if(!(*table))
            {
                res = 1;
            }
            else
            {
                res = Gd5f1g_ReadPage(bads_location, (void*)(*table), HW_ECC);
            }

            break;
        }

        case markbad:
        {
            res = Gd5f1g_BadMark((u32)args); // args = block
            break;
        }

        case format:
        {
            va_list list;
            s32 start, end;
            u8 *tmp, escape = 0;

            start = (u32)args;
            va_start(list, args);
            end = va_arg(list, u32);
            va_end(list);

            tmp = malloc(__nandescription.BytesPerPage);
            if(!tmp)
                return (-1);

            if(Gd5f1g_ReadPage(bads_location, (void*)(tmp), HW_ECC)) // �������
            {
                if(nandvalidbads((u32*)tmp))
                    escape = 1; // ���ڻ������������

                if(-1 == end)
                    end = __nandescription.BlksPerLUN * __nandescription.LUNs;

                do
                {
                    if((bads_location == (--end)) && escape) //������ڵ�һҳ
                        continue;

                    if(Gd5f1g_EraseBlock((u32)end))
                    {
                        res = -1;
                        break;
                    }
                }
                while(end!=start);
            }
            else
            {
                res = -1;
            }

            free(tmp);
            break;
        }

        default: res = -1; break;
    }

    return (res);
}


//-----------------------------------------------------------------------------
//����:��ȡnand flash ���ڲ���Ϣ
//����:
//����: "0" -- �ɹ�; "-1" -- ����ʧ��; "-2" -- �ڴ治��;
//��ע:
//-----------------------------------------------------------------------------
static s32 Gd5f1g_GetNandDescr(struct NandDescr *Descr)
{
    s32 Ret = -1;
    struct NandONFI *onfi_buf;

    onfi_buf = (struct NandONFI *)malloc(sizeof(struct NandONFI));
    if (NULL == onfi_buf)
        return -2;

    memset(onfi_buf, 0, sizeof(struct NandONFI));
    if(Gd5f1g_ReadParamPage(onfi_buf))
    {
        if(deonfi((const struct NandONFI *)onfi_buf, Descr, 0) == 0)
            Ret = 0;
    }
    free (onfi_buf);
    return (Ret);
}

//-----------------------------------------------------------------------------
//����: Gd5f1g��ʼ��
//����: ��
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t Gd5f1g_Init(void)
{
    u16 chip_id;
    u8 sta;
    bool_t ret = false;

    if(Gd5f1g_GetNandDescr(&__nandescription))
    {
        return false;
    }
    __nandescription.ReservedBlks = 0;
    __nandescription.Controller = HW_ECC_SUPPORTED;
    __nandescription.OOB_Size = ECC_ENABLE_SPARE_WRITE_LEN;
    __nandescription.BadMarkOffset = 0x30;          //�������У�0x30~0x33���ǲ���ECC�����ġ�д�������棬����Ӱ����������ݵĲ���
    gd5f1g_lock = Lock_MutexCreate("gd25q16c Lock");
    if(!gd5f1g_lock)
    {
        error_printf("gd5f1g"," create gd5f1g flash lock fali \r\n.");
        return false;
    }
    if(QSPI_Send_CMD(Gd5f1g_SoftReset,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE))
    {
        DJY_EventDelay(1000);

        chip_id = Gd5f1g_ReadID();
        if(chip_id != 0Xc851)
            error_printf("gd5f1g","chip ID error.\r\n");
        else
        {
            if(Gd5f1g_QspiEnable())
            {
                if(Gd5f1g_ReadSR(Get_Protection,&sta))
                {
                    if(sta & (7 << 3))
                    {
                        sta &= ~(7 << 3);
                        if(Gd5f1g_WriteSR(Set_Protection, sta))
                        {
                            if(Gd5f1g_ReadSR(Get_Protection,&sta))
                            {
                                if(!(sta & (7 << 3)))
                                    ret = true;
                            }
                        }
                    }
                    else
                        ret = true;
                }
            }
        }
    }
    return ret;
}
//-----------------------------------------------------------------------------
//����: nand_flashģ�鰲װ
//����:
//����: 0 -- �ɹ�; -1 -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
s32 ModuleInstall_Gd5f1g(void)
{
    u8 *buf;
    if(Gd5f1g_Init() == false)
    {
        error_printf("gd5f1g", "device : initialization failed(init).\r\n");
        return -1;
    }

    buf = malloc(__nandescription.BytesPerPage);
    if (NULL == buf)
        return -1;
    if(Gd5f1g_ReadPage(0, buf, HW_ECC))
    {
        if(nandvalidbads((u32 *)buf))
            is_bads_table = true;
        else
            is_bads_table = false;

        free(buf);

        Spare_BadChk = malloc(__nandescription.OOB_Size);
        if (NULL == Spare_BadChk)
        {
            return (-1);
        }

        Spare_BadMark = malloc(__nandescription.OOB_Size);
        if (NULL == Spare_BadChk)
        {
            free(Spare_BadChk);
            return (-1);
        }

        if(!bads_table)
        {
            bads_table = nandbuildbads(Gd5f1g_Ctrl);
            if(!bads_table)
            {
                error_printf("gd5f1g", "device : initialization failed(bad table).\r\n");
                free(Spare_BadMark);
                free(Spare_BadChk);
                return (-1);
            }
        }

        nand_umedia = malloc(sizeof(struct umedia)+__nandescription.BytesPerPage+__nandescription.OOB_Size);
        if(!nand_umedia)
            return (-1);
        memset(nand_umedia,0,sizeof(struct umedia));

        nand_umedia->mreq = Gd5f1g_Ctrl;
        nand_umedia->type = nand;
        nand_umedia->ubuf = (u8*)nand_umedia + sizeof(struct umedia);

        if(!Device_Create((const char*)NandFlashName, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)nand_umedia)))
        {
            error_printf("gd5f1g", " device : %s addition failed.\r\n", NandFlashName);
            free(nand_umedia);
            free(Spare_BadMark);
            free(Spare_BadChk);
            return (-1);
        }

        Gd5f1gInited = true;
    }
    else
        free(buf);
    return 0;

}

// =============================================================================
// ���ܣ��ж�nandflash�Ƿ�װ
// ������  ��
// ���أ��ѳɹ���װ��true����δ�ɹ���װ��false����
// ��ע��
// =============================================================================
bool_t Gd5f1g_IsInstall(void)
{
    return Gd5f1gInited;
}


// ============================================================================
// ���ܣ���nand���ļ�ϵͳ��������
// ������fs -- ��Ҫ���ص��ļ�ϵͳ��mediadrv -- ý��������
//       bstart -- ��ʼ�飬bend -- �����飨�������ÿ飬ֻ���ÿ����һ�飩
// ���أ�0 -- �ɹ��� -1 -- ʧ��
// ��ע��
// ============================================================================
s32 __Gd5f1g_FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv)
{
    char *FullPath,*notfind;
    struct Object *targetobj;
    struct FsCore *super;
    s32 res,BlockNum;

    if(mediadrv == NULL)
        return -1;
    targetobj = OBJ_MatchPath(fs, &notfind);
    if(notfind)
    {
        error_printf("gd5f1g"," not found need to install file system.");
        return -1;
    }
    super = (struct FsCore *)OBJ_GetPrivate(targetobj);
    super->MediaInfo = nand_umedia;             //��nand����Ϣ�ŵ��ļ�ϵͳ�ĺ������ݽṹ����
    super->MediaDrv = mediadrv;

    if(-1 == (s32)bend)
    {
        bend = __nandescription.BlksPerLUN * __nandescription.LUNs;  //�����ļ�ϵͳ��������Ľ�����
        BlockNum = bend - bstart;       //����һ���ж��ٿ�
    }
    else
    {
        BlockNum = bend - bstart;
    }
    super->AreaSize = __nandescription.BytesPerPage * __nandescription.PagesPerBlk * BlockNum;        //�����ļ�ϵͳ��������һ���ж����ֽ�
    super->MediaStart = bstart * __nandescription.PagesPerBlk; // ��ʼunit��

    res = strlen(NandFlashName)+strlen(s_ptDeviceRoot->name) + 1;
    FullPath = malloc(res);
    memset(FullPath, 0, res);
    sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name, NandFlashName); //��ȡ���豸��ȫ·��
    File_BeMedia(FullPath,fs); //�����豸�����ļ�ϵͳ
    free(FullPath);

    printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, bstart, bend);
    return (0);
}
