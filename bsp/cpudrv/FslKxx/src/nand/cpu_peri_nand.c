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


//˼·:
//   ����һ��buf��ʾҳ��oob����,�����ǽ����Ƿֿ���ʾ��ԭ���Ƕ���������ʱ,ECCУ��
//   ȴ����Ҫ����ҳ��OOB���ݡ���˴�ECCУ��ʱ,ÿ�ζ����д������ҳ��OOB�����ݲ���

#include <string.h>
#include <stdlib.h>
#include <djyos.h>
#include <MKxxF12.h>
#include <device/djy_flash.h>
#include <include/device.h>
#include <device/unit_media.h>
#include <board.h>
#include <libc/misc/ecc/ecc_256.h>
#include <dbug.h>
#include <djyfs/filesystems.h>
#include <math.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//   extern s32 ModuleInstall_NAND(u32 doformat);
//   ModuleInstall_NAND(CFG_NFLASH_PART_FORMAT);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip nand"//CPU��nand����
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"device file system","file system"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_NAND == false )
//#warning  " cpu_onchip_nand  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_NAND    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@enum,true,false,
#define CFG_NFLASH_PART_FORMAT     false      //�Ƿ���Ҫ������оƬ��
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure


//����ķ�����ʼ�ͷ���������д����-1��10000��������ֵ�ɸ��ݾ���оƬ������д
//
// @brief  NFCԤ����
//
#define NFCDIV                               0x05// ʱ�ӷ�Ƶ
#define NFCFRAC                              0x00// ʱ�ӷ�Ƶ

#define NANDFLASH_STATUS_PASS                0x00
#define NANDFLASH_STATUS_ERR                 0x01

// ECC STATUS
#define ECC_STATUS_MASK                     ((u8)0x80)
#define ECC_ERR_COUNT                       ((u8)0x3F)

//
// @brief  NFC CMD
//
#define NFC_READ_PAGE_CMD_CODE              0x7EF8
#define NFC_PROGRAM_PAGE_CMD_CODE           0x7FD8
#define NFC_DMA_PROGRAM_PAGE_CMD_CODE       0xFFC0
#define NFC_BLOCK_ERASE_CMD_CODE            0x4ED8
#define NFC_READ_ID_CMD_CODE                0x4804
#define NFC_RESET_CMD_CODE                  0x4040
#define NFC_READ_STATUS_CODE                0x4880

//
// @brief  ����ȫ�ֱ���
//
s32 __nand_read(s64 unit, void *data, struct uopt opt);
s32 __nand_write(s64 unit, void *data, struct uopt opt);
s32 __nand_erase(s64 unit, struct uesz sz);
s32 __nand_req(enum ucmd cmd, ptu32_t args, ...);
static u32 *badstable;
static u32 badslocation = 0;
static bool_t sNandflashInited = false;
extern s32 deonfi(const char *data, struct NandDescr *onfi, u8 little);
static struct NandDescr *s_ptNandInfo;//
extern struct Object *s_ptDeviceRoot;
static struct MutexLCB *NandFlashLock;
struct umedia *nand_umedia;
static const char *NandFlashName = "nand";      //��flash��obj�ڵ�����
#define NFlashLockTimeOut     CN_CFG_TICK_US * 1000 * 10
static u32 *badstable = NULL;
static u32 badslocation = 0;
s32 __nand_FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv);
static s32 __nand_init(void);

//
// @brief  ���غ�������
//
static void  __SetRowCol(u32 PageNo, u32 Offset);
static void __Buf2Mem(u8 BufNo, u32 Offset, u8 *Mem, u32 Size);
static void __Mem2Buf(u8 BufNo, u32 Offset, const u8 *Mem, u32 Size);
static s32 __Status(void);
static s32 __Ready(void);
static void  K70_MFC_Config(void);
static s32 K70_GetNandDescr(struct NandDescr *Descr);

//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static void  K70_MFC_Config(void)
{
    //�߼�:
    //   ����IO�ܽ�
    //   ����NFC�Ĵ���

    // Enable the NFC clock gate in the SIM
    SIM->SCGC3 |= SIM_SCGC3_NFC_MASK;

    // Disable the MPU to allow NFC to access memory
    MPU->CESR &= 0xFFFFFFFE;

    // Set the NFC clock divder according to defines
    SIM->CLKDIV4 |= SIM_CLKDIV4_NFCDIV(NFCDIV) |SIM_CLKDIV4_NFCFRAC(NFCFRAC);

    // NAND�ܽ�����
    PORTC->PCR[17] = PORT_PCR_MUX(6) | PORT_PCR_DSE_MASK;/* Enable NFC_CE0 pad */
    PORTC->PCR[16] = PORT_PCR_MUX(6) | PORT_PCR_DSE_MASK;/* Enable the NFC_RB pad */
    PORTD->PCR[8]  = PORT_PCR_MUX(6) | PORT_PCR_DSE_MASK;/* Enable the NFC_CLE pad */
    PORTD->PCR[9]  = PORT_PCR_MUX(6) | PORT_PCR_DSE_MASK;/* Enable the NFC_ALE pad */
    PORTD->PCR[10] = PORT_PCR_MUX(6) | PORT_PCR_DSE_MASK;/* Enable the NFC_RE pad */
    PORTC->PCR[11] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_WE pad */

    PORTB->PCR[20] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA15 pad */
    PORTB->PCR[21] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA14 pad */
    PORTB->PCR[22] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA13 pad */
    PORTB->PCR[23] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA12 pad */
    PORTC->PCR[0]  = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA11 pad */
    PORTC->PCR[1]  = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA10 pad */
    PORTC->PCR[2]  = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA9 pad */
    PORTC->PCR[4]  = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA8 pad */
    PORTC->PCR[5]  = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA7 pad */
    PORTC->PCR[6]  = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA6 pad */
    PORTC->PCR[7]  = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA5 pad */
    PORTC->PCR[8]  = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA4 pad */
    PORTC->PCR[9]  = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA3 pad */
    PORTC->PCR[10] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA2 pad */
    PORTD->PCR[4]  = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA1 pad */
    PORTD->PCR[5]  = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA0 pad */

    // ����NFC�Ĵ���
    // ����CFG�Ĵ���
    NFC->CFG = NFC_CFG_STOPWERR_MASK |// д��������ֹͣ
               NFC_CFG_IDCNT(5) |
               NFC_CFG_TIMEOUT(6) |
               NFC_CFG_BITWIDTH_MASK |// 16λ��
               NFC_CFG_PAGECNT(1);
    // ����RAI�Ĵ���
    NFC->RAI = 0;// ���Զ����ӵ�ַ
    // ����SWAP�Ĵ���
    NFC->SWAP  = NFC_SWAP_ADDR1(0x7FF) | NFC_SWAP_ADDR2(0x7FF);
    // ����ISR�Ĵ���
    NFC->ISR  |= (NFC_ISR_DONECLR_MASK |
                  NFC_ISR_DONEEN_MASK  |
                  NFC_ISR_IDLECLR_MASK |
                  NFC_ISR_IDLEEN_MASK);
    NFC->RAR |= (NFC_RAR_CS0_MASK | NFC_RAR_RB0_MASK);// ʹ��NANDѡƬ
}
//-----------------------------------------------------------------------------
//����:
//����: Data -- ���ݿռ䡣����ΪNULL��
//      Flags --
//
//����: ">0" -- ������д����ֽ���;
//      "-1" -- ��������;
//      "-2" -- дʧ��;
//��ע:
//-----------------------------------------------------------------------------
s32  K70_PageProgram(u32 PageNo, const u8 *Data, u32 Flags)
{
    /*�߼�:
        K70�£���ECCд�룬ECC�ļ���ֵ����spare����β������˲����Ƿ�����
        SPARE_REQ����ECCд�붼��Ҫдspare�����ܻᵣ�ǲ���SPARE_REQȴ��ҪHW_ECC
        ʱ,���spare���ݸ��ǵ����⡣��ʵ���ϣ�K70�Ĵ�ECCд��ǰ�������Ȳ�������
        ��ECC�Զ�д��ֵ���ܲ���ȷ��
    */

    u8 *Spare;
    u8 SpareReq = 1;
    u8 BufNo = 2;// д�̶�ʹ��buffer 2
    u32 Size = s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size;
    struct FlashChip *Nand;

    if (!Data)
        return (-1);

    if(Lock_MutexPend(NandFlashLock, NFlashLockTimeOut) == false)
    {
        return (-2);
    }

    __Ready();

    // ��ҳ��main�������buffer
    __Mem2Buf(BufNo, 0, Data, s_ptNandInfo->BytesPerPage);

    if (SPARE_REQ & Flags)
        Spare = (u8*)Data + s_ptNandInfo->BytesPerPage;
    else if (HW_ECC & Flags)
    {
        // ��ҪECCУ��,�����ֲ���Ҫдspareʱ,����Ҫ
        Nand = dListEntry(s_ptNandInfo, struct FlashChip, Descr);
        Spare = Nand->Buf + s_ptNandInfo->BytesPerPage;// spare�ռ� */
        if (NULL == Spare)
        {
            Lock_MutexPost(NandFlashLock);
            error_printf("nand driver", "chip buffer do not exist!\r\n");
            return (-1);
        }
    }
    else
    {
        SpareReq = 0;
        Size = s_ptNandInfo->BytesPerPage;
    }

    // ��ҳ��spare�������buffer
    if (SpareReq)
        __Mem2Buf(BufNo, s_ptNandInfo->BytesPerPage, Spare, s_ptNandInfo->OOB_Size);


    // ecc ������
    switch (MASK_ECC & Flags)
    {
        case HW_ECC :
            /* 4 error correct ���� 8bytes ECC */
            NFC->CFG &= (~NFC_CFG_ECCMODE_MASK);
            NFC->CFG |= NFC_CFG_ECCMODE(1);
            break;
        case SW_ECC :
        case NO_ECC :
            NFC->CFG &= (~NFC_CFG_ECCMODE_MASK);
            break;
    }

    __SetRowCol(PageNo, 0);

    NFC->SECSZ = NFC_SECSZ_SIZE(Size + 1);/* ��Ϊ��16λ */

    /* Write the NFC_CMD2 register with the command byte and code for an erase */
    NFC->CMD2 = NFC_CMD2_BYTE1(PAGE_PROGRAM_CMD_BYTE1) |
                NFC_CMD2_CODE(NFC_PROGRAM_PAGE_CMD_CODE) |
                NFC_CMD2_BUFNO(BufNo);

    /* Write the NFC_CMD1 register with the command byte2 and byte3 for an erase */
    NFC->CMD1 = NFC_CMD1_BYTE2(PAGE_PROGRAM_CMD_BYTE2) |
                NFC_CMD1_BYTE3(STATUS_READ_CMD_BYTE);


    NFC->ISR  |= (NFC_ISR_DONECLR_MASK |
                  NFC_ISR_DONEEN_MASK |
                  NFC_ISR_IDLECLR_MASK |
                  NFC_ISR_IDLEEN_MASK);

    /* Set Start Bit to send command to the NAND flash */
    NFC->CMD2 |= NFC_CMD2_BUSY_START_MASK;

#if 0
        PrintBuf((u8 *)Data, (2112));/* ���� */
#endif

    /* Wait for start/busy bit to clear indicating command is done */
    if(__Status())
    {
        Lock_MutexPost(NandFlashLock);
        return (-2);
    }
    Lock_MutexPost(NandFlashLock);
    if ((HW_ECC & Flags) && !(SPARE_REQ & Flags))
      // ����ҪECCдʱ�ķ���ֵ��
      // ����ʵ������д����spare�����,���ﴦ����Ϊ�����û��о�������д���پ��Ƕ���
        return (s_ptNandInfo->BytesPerPage);
    else
        return (Size);

}
//-----------------------------------------------------------------------------
//����:
//����:
//����: ">0" -- �������������ֽ�����
//      "-1" -- �����������;
//      "-2" -- ��ʧ��;
//      "-3" -- ECC����ʧ��;
//��ע: ���ܶ�д�Ƿ���ȷ���������ݻش�
//-----------------------------------------------------------------------------
s32  K70_PageRead(u32 PageNo, u8 *Data, u32 Flags)
{
    /*�߼�:
        K70�£���ECC����Ĭ�ϻ�ȡspare������ΪECCֵ�����spare����β����
    */
    u32 Size;
    u8 BufNo = 0;// ���̶�ʹ��buffer 0
    u32 EccStatusAddr = s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size;// ECC status��ŵ�λ��
    s32 Ret = 0;

    if (!Data)
        return (-1);

    if(Lock_MutexPend(NandFlashLock, NFlashLockTimeOut) == false)
    {
        return (-2);
    }

    __Ready();

    // ��Ҫ���豸��ȡ�������ݴ�С
    if ((SPARE_REQ & Flags) || (HW_ECC & Flags))
        Size = s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size;
    else
        Size = s_ptNandInfo->BytesPerPage;

    // ECC ������
    switch (Flags & MASK_ECC)
    {
        case HW_ECC :
            NFC->CFG &= ~(NFC_CFG_ECCMODE_MASK | NFC_CFG_ECCAD_MASK );
            NFC->CFG |= (NFC_CFG_ECCMODE(1) |// 4 error correct ���� 8bytes ECC
                         NFC_CFG_ECCAD(EccStatusAddr >> 3) |// ECC STATUS��ַ
                         NFC_CFG_ECCSRAM_MASK);// ʹ��ECC STATUS
            break;
        case SW_ECC :
        case NO_ECC :
            NFC->CFG &= ~(NFC_CFG_ECCMODE_MASK |
                          NFC_CFG_ECCAD_MASK );
            break;
    }

    __SetRowCol(PageNo, 0);

    NFC->SECSZ =  NFC_SECSZ_SIZE(Size + 1);

    // Write the NFC_CMD2 register with the command byte and code for an erase
    NFC->CMD2 = (NFC_CMD2_BYTE1(PAGE_READ_CMD_BYTE1) |
                 NFC_CMD2_CODE(NFC_READ_PAGE_CMD_CODE) |
                 NFC_CMD2_BUFNO(BufNo));

    // Write the NFC_CMD1 register with the command byte2 and byte3 for an erase
    NFC->CMD1 = NFC_CMD1_BYTE2(PAGE_READ_CMD_BYTE2) |
                NFC_CMD1_BYTE3(STATUS_READ_CMD_BYTE);

    NFC->ISR |= (NFC_ISR_DONECLR_MASK | NFC_ISR_DONEEN_MASK |
                 NFC_ISR_IDLECLR_MASK | NFC_ISR_IDLEEN_MASK);

    // Set Start Bit to send command to the NAND flash
    NFC->CMD2 |= NFC_CMD2_BUSY_START_MASK;

    if(__Status())
        Ret = -2;// ����,��ʧ��;

    // ECCУ��
    if ( HW_ECC & Flags)
    {
        // buffer 0�Ŀռ�ȡ���ġ�С��SOC����ECC status�ĵ�ַ��4
        u8 EccStatus = *(u8 *)(NFC_BASE + EccStatusAddr + 0x4);

        if (EccStatus & ECC_STATUS_MASK)
        {
            error_printf("nand driver", "Page %d ECC failed\r\n", PageNo);
#if 0
        PrintBuf((u8 *)NFC_BASE, (2300));/* ���� */
        printk("\r\nECC STATUS: %x\r\n", EccStatus);/* ���� */
#endif
            Ret = -3;
        }
        else
        {
            u8 EccCnt;
            EccCnt = EccStatus & ECC_ERR_COUNT;
            if (0 != EccCnt)
                debug_printf("nand driver", "Page %d: ECC: %d bit fixed\r\n", PageNo, EccCnt);
        }
    }

    // ��buffer��ȡ�������ݴ�С
    if ((HW_ECC & Flags) && !(SPARE_REQ & Flags))
        Size = s_ptNandInfo->BytesPerPage;

    __Buf2Mem(BufNo, 0, Data, Size);

    if(Ret)
    {
        Lock_MutexPost(NandFlashLock);
        return (Ret);// ʧ��;
    }
    Lock_MutexPost(NandFlashLock);
    return (Size);
}
//-----------------------------------------------------------------------------
//����:
//����:
//����: "0" -- �ɹ�;"-1" -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
s32 K70_BlockErase(u32 BlkNo)
{
    if(Lock_MutexPend(NandFlashLock, NFlashLockTimeOut) == false)
    {
        return (-1);
    }

    __Ready();

    // Make sure the column address is cleared - not needed for block erase
    NFC->CAR = 0x0;
    NFC->RAR &= (u32)0xFF000000;// Row�ĵ�ַΪ����
    NFC->RAR |= BlkNo << 6; // Row��ַ���Ǻ���PAx[5..0]���ֵ�

    NFC->CFG &= ~NFC_CFG_ECCMODE_MASK;// ����������Ҫ�ر�ECC

    // Write the NFC_CMD2 register with the command byte and code for an erase
    NFC->CMD2 = NFC_CMD2_BYTE1(BLOCK_ERASE_CMD_BYTE1) |
                NFC_CMD2_CODE(NFC_BLOCK_ERASE_CMD_CODE);

    // Write the NFC_CMD1 register with the command byte2 and byte3 for an erase
    NFC->CMD1 = NFC_CMD1_BYTE2(BLOCK_ERASE_CMD_BYTE2) |
                NFC_CMD1_BYTE3(STATUS_READ_CMD_BYTE);

    NFC->ISR  |= (NFC_ISR_DONECLR_MASK |
                  NFC_ISR_DONEEN_MASK  |
                  NFC_ISR_IDLECLR_MASK |
                  NFC_ISR_IDLEEN_MASK);

    // Set Start Bit to send command to the NAND flash
    NFC->CMD2 |= NFC_CMD2_BUSY_START_MASK;

    while (NFC->CMD2 & NFC_CMD2_BUSY_START_MASK);
    Lock_MutexPost(NandFlashLock);
    return (__Status());
}

//-----------------------------------------------------------------------------
//����:
//����:
//����: "0" -- �ÿ�;
//      "-1" -- ����;
//      "-2" -- �ڴ治��
//      "-3" -- ��ʧ��;
//��ע:
//-----------------------------------------------------------------------------
s32 K70_BadChk(u32 BlkNo)
{
    /*�߼�:
       ��Ϊ������ͬ,bad mark�ĵ�ַ���ܲ�һ��,���Ըú�������������
    */

    u8 BadMark[2];
    u32 PageNo = (BlkNo * s_ptNandInfo->PagesPerBlk);// (���)ת(ҳ��)
    //u32 BadMarkOffet = (s_ptNandInfo->BytesPerPage >> 1) + 1;// 16λ
    u32 BadMarkOffet = ((s_ptNandInfo->BytesPerPage + s_ptNandInfo->BadMarkOffset) >> 1) + 1;
    u32 Size = 2;// 16λ��BAD MARKλ��2bytes
    u8 BufNo = 0;// ���̶�ʹ��BUF 0

    NFC->CFG &= ~NFC_CFG_ECCMODE_MASK;// �ر�ECC

    __SetRowCol(PageNo, BadMarkOffet);

    NFC->SECSZ =  NFC_SECSZ_SIZE(Size + 1);

    // Write the NFC_CMD2 register with the command byte and code for an erase
    NFC->CMD2 = (NFC_CMD2_BYTE1(PAGE_READ_CMD_BYTE1) |
                 NFC_CMD2_CODE(NFC_READ_PAGE_CMD_CODE) |
                 NFC_CMD2_BUFNO(BufNo));

    // Write the NFC_CMD1 register with the command byte2 and byte3 for an erase
    NFC->CMD1 = NFC_CMD1_BYTE2(PAGE_READ_CMD_BYTE2) |
                NFC_CMD1_BYTE3(STATUS_READ_CMD_BYTE);

    NFC->ISR  |= (NFC_ISR_DONECLR_MASK |
                  NFC_ISR_DONEEN_MASK  |
                  NFC_ISR_IDLECLR_MASK |
                  NFC_ISR_IDLEEN_MASK);

    // Set Start Bit to send command to the NAND flash
    NFC->CMD2 |= NFC_CMD2_BUSY_START_MASK;

    // Wait for start/busy bit to clear indicating command is done
    if(__Status())
        return (-3);

    // �жϻ����־λ
    __Buf2Mem(BufNo, 2, BadMark, Size);// ����ƫ����Ϊ2��CPU�ṹ�й�
    if (0xAAAA == *(u16*)BadMark)
        return (-1);

    return (0);
}
//-----------------------------------------------------------------------------
//����:
//����:
//����: "0" -- �ɹ�; "-1" -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
s32  K70_BadMark(u32 BlkNo)
{
    /*�߼�:
        ����BAD MARKλԭ���Ƕ����Լ��Ƿ���ECCУ������, ֻ�ܽ���λ��־Ϊ����
    */

    u8 BufNo = 2;// д�̶�ʹ��buffer 2
    u32 Size = 2;
    u32 PageNo = (BlkNo * s_ptNandInfo->PagesPerBlk);// (���)ת(ҳ��)
    //u32 BadMarkOffet = (s_ptNandInfo->BytesPerPage >> 1) + 1;// 16λ��
    u32 BadMarkOffet = ((s_ptNandInfo->BytesPerPage + s_ptNandInfo->BadMarkOffset) >> 1) + 1;
    u8 BadMark[2] = {0xAA, 0xAA};

    NFC->CFG &= ~NFC_CFG_ECCMODE_MASK;// �ر�ECC

    NFC->SECSZ = NFC_SECSZ_SIZE(Size + 1);

    __Mem2Buf(BufNo, 2, BadMark, Size);// �������ø�CPU�ṹ�й�

    __SetRowCol(PageNo, BadMarkOffet);// �����BadMarkOffet��CPU�ṹ�й�

    // ������˻����ҳд��
    // Write the NFC_CMD2 register with the command byte and code for an erase
    NFC->CMD2 = NFC_CMD2_BYTE1(PAGE_PROGRAM_CMD_BYTE1) |
                NFC_CMD2_CODE(NFC_PROGRAM_PAGE_CMD_CODE) |
                NFC_CMD2_BUFNO(BufNo);

    // Write the NFC_CMD1 register with the command byte2 and byte3 for an erase
    NFC->CMD1 = NFC_CMD1_BYTE2(PAGE_PROGRAM_CMD_BYTE2) |
                NFC_CMD1_BYTE3(STATUS_READ_CMD_BYTE);


    NFC->ISR |= (NFC_ISR_DONECLR_MASK |
                 NFC_ISR_DONEEN_MASK |
                 NFC_ISR_IDLECLR_MASK |
                 NFC_ISR_IDLEEN_MASK);

    // Set Start Bit to send command to the NAND flash
    NFC->CMD2 |= NFC_CMD2_BUSY_START_MASK;

    return (__Status());
}
//-----------------------------------------------------------------------------
//����:
//����:
//����: "0" -- �ɹ�; "-1" -- ����ʧ��.
//��ע:
//-----------------------------------------------------------------------------
static s32 K70_GetNandDescr(struct NandDescr *Descr)
{
    u32 CfgBack;
    u8 *OnfiBuf;
    u32 OnfiSize = 768;
    u8 BufNo = 0; /* ���̶�ʹ��buffer 0 */

    OnfiBuf = malloc (OnfiSize);
    if (NULL == OnfiBuf)
    {
        error_printf("nand driver", "�ڴ治��\r\n");
        return (-1);
    }

    __Ready();

    /* ��ONFIʱ,ǿ��ʹ��8λ��ԭ�Ĵ���ֵ�ȱ��� */
    CfgBack = NFC->CFG;

    /* ʹ��8Bit��� */
    NFC->CFG = NFC_CFG_STOPWERR_MASK |
               NFC_CFG_TIMEOUT(6) |
               NFC_CFG_PAGECNT(1);

    /* Write the NFC_CMD2 register with the command byte and code for a
       reset */
    NFC->CMD2 = NFC_CMD2_BYTE1(PARAMETER_PAGE_READ_CMD_BYTE) |
                NFC_CMD2_CODE(0x4860) |
                NFC_CMD2_BUFNO(BufNo);

    /* ����ISR */
    NFC->ISR |= (NFC_ISR_DONECLR_MASK |
                 NFC_ISR_DONEEN_MASK  |
                 NFC_ISR_IDLECLR_MASK |
                 NFC_ISR_IDLEEN_MASK);

    /* Set Start Bit to send command to the NAND flash */
    NFC->CMD2 |= NFC_CMD2_BUSY_START_MASK;

    /* �ȴ�������ɣ���Ҫ��STATUS.δʹ��DONE_MASK,��Ϊ��Ҫȡ���� */
    while (!(NFC->ISR & NFC_ISR_IDLE_MASK));

    NFC->CFG = CfgBack;

    __Buf2Mem(BufNo, 0, OnfiBuf, OnfiSize);
#if 0
    PrintBuf(OnfiBuf, OnfiSize);/* ���� */
#endif
    if(deonfi((const char*)OnfiBuf, Descr, 1))
    {
        free (OnfiBuf);
        return (-1);
    }

    Descr->Type = ONFI;
    free (OnfiBuf);
    return (0);
}
//-----------------------------------------------------------------------------
// ���ܣ���װnand����
// ������  doformat -- �Ƿ��ʽ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
//-----------------------------------------------------------------------------
s32 ModuleInstall_NAND(u32 doformat)
{
    if(!s_ptNandInfo)
    {
        if(__nand_init())
        {
            printf("\r\n: erro : device : nand initialization failed(init).");
            return (-1);
        }
    }

    if(doformat)
    {
        struct uesz sz;
        sz.unit = 0;
        sz.block = 1;
        __nand_req(format, 0 , -1, &sz);
    }

    if(!badstable)
    {
        badstable = nandbuildbads(__nand_req);
        if(!badstable)
        {
            printf("\r\n: erro : device : nand initialization failed(bad table).");
            return (-1);
        }
    }

    nand_umedia = malloc(sizeof(struct umedia)+s_ptNandInfo->BytesPerPage+s_ptNandInfo->OOB_Size);
    if(!nand_umedia)
        return (-1);

    nand_umedia->mreq = __nand_req;
    nand_umedia->type = nand;
    nand_umedia->ubuf = (u8*)nand_umedia + sizeof(struct umedia);

    if(!Device_Create((const char*)NandFlashName, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)nand_umedia)))
    {
        printf("\r\n: erro : device : %s addition failed.", NandFlashName);
        free(nand_umedia);
        return (-1);
    }
    sNandflashInited = true;
    return (0);// �ɹ�;
}
// =============================================================================
// ���ܣ��ж�nandflash�Ƿ�װ
// ������  ��
// ���أ��ѳɹ���װ��true����δ�ɹ���װ��false����
// ��ע��
// =============================================================================
bool_t Nandflash_is_install(void)
{
    return sNandflashInited;
}
/******************************************************************************
                         PRIVATE FUNCTION(����˽�к���)
******************************************************************************/
//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static void  __SetRowCol(u32 PageNo, u32 Offset)
{
    NFC->CAR = Offset & 0x7FF;

    NFC->RAR &= 0xFF000000;//ROW�ĵ�ַΪ����
    NFC->RAR |= (PageNo & 0x1FFFF);
}
//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static void __Buf2Mem(u8 BufNo, u32 Offset, u8 *Mem, u32 Size)
{
    u32 BufAddr;

    BufAddr = BufNo << 12;//buffer�ĵ�ַ
    memcpy(Mem, (u8 *)(NFC_BASE + BufAddr + Offset), Size);
}

//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static void __Mem2Buf(u8 BufNo, u32 Offset,  const u8 *Mem, u32 Size)
{
    u32 BufAddr;

    BufAddr = BufNo << 12;//buffer�ĵ�ַ
    memcpy((u8 *)(NFC_BASE + BufAddr + Offset), Mem, Size);
}
//-----------------------------------------------------------------------------
//����:
//����:
//����: "0" -- �ɹ�; "-1" -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
static s32 __Status(void)
{
    u32 ISR;
    ISR = NFC->ISR;// ����
    while (!(NFC->ISR & NFC_ISR_IDLE_MASK));

    ISR = NFC->ISR;// ����
    if (NFC->SR2 & NANDFLASH_STATUS_ERR)
    {
        error_printf("nand driver", "NAND����ʧ��\r\n");
        return (-1);
    }
    else
    {
        /* ��ISR */
        NFC->ISR |= (NFC_ISR_IDLECLR_MASK | NFC_ISR_DONECLR_MASK);
        return (0);
    }

}
//-----------------------------------------------------------------------------
//����: todo:
//����:
//����: "1" -- ׼����; "0" -- δ׼���á�
//��ע:
//-----------------------------------------------------------------------------
static s32 __Ready(void)
{
    /* ���R/B_n�ܽ� */
    //return (StructEntry(s_ptNandInfo, struct NandChip, Lock));/* Chip->Lock */
    return (1);
}


#if 0
//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
void PageTest(const u32 PageNo, const u8 Init)
{
    volatile u8 *Buf;
    u32 i, Ret;
    struct FlashChip *Chip;
    u32 ECC_Flags = 0;

    if(Init)
    {
        if(ModuleInstall_NAND("nand", 0, 0)) //��װnand�豸
            while(1);
    }

    if(HW_ECC_SUPPORTED & s_ptNandInfo->Controller)
        ECC_Flags |= HW_ECC;

    if(SW_ECC_SUPPORTED & s_ptNandInfo->Controller)
        ECC_Flags |= SW_ECC;

    if(NO_ECC_SUPPORTED & s_ptNandInfo->Controller)
        ECC_Flags |= NO_ECC;

    Chip = dListEntry(s_ptNandInfo, struct FlashChip, Descr);

    Buf = Chip->Buf;

    for(i = 0; i < s_ptNandInfo->BytesPerPage; i++)
        Buf[i] = (u8)0xFF;

    Chip->Ops.WrPage(PageNo, Buf, ECC_Flags | SPARE_REQ);

    memset(Buf, 0, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    Ret = Chip->Ops.RdPage(PageNo, Buf, ECC_Flags | SPARE_REQ);
    for(i=0; i<(s_ptNandInfo->BytesPerPage); i++)
    {
        //if(((u8)i) != Buf[i])
        if(((u8)0xFF) != Buf[i])
            while(1);
    }

    // ECCһλ����
    Buf[1] = 0xF0;

    Chip->Ops.WrPage(PageNo, Buf, NO_ECC);

    memset(Buf, 0, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    Ret = Chip->Ops.RdPage(PageNo, Buf, ECC_Flags | SPARE_REQ);
    for(i=0; i<(s_ptNandInfo->BytesPerPage); i++)
    {
//      if(((u8)i) != Buf[i])
        if(((u8)0xFF) != Buf[i])
            while(1);
    }

}

void ContinuityTest(struct FlashChip *Chip)
{
     u32 i, j, Len;
    u32 Blks, Pages;
    s32 BadBlks = 0;
    u8 *Temp;

    if(NULL == Chip)
        while(1);

    Blks = Chip->Descr.Nand.BlksPerLUN * Chip->Descr.Nand.LUNs;

    for(i = Chip->Descr.Nand.ReservedBlks; i < Blks; i++)
    {
        if(0 != Chip->Ops.ErsBlk(i))
        {
            /* ����ʧ�� */
//          TraceDrv(NAND_TRACE_ERROR, "block %d cannot be erased!\r\n", i);
            while(1);
        }
    }

    // �������У��
    Len = (Chip->Descr.Nand.BytesPerPage + Chip->Descr.Nand.OOB_Size);
    Temp = (u8*)malloc(Len);
    if(NULL == Temp)
        while(1);// �ڴ治��
    for(i = (Chip->Descr.Nand.ReservedBlks * Chip->Descr.Nand.PagesPerBlk); i < (Blks * Chip->Descr.Nand.PagesPerBlk); i++)
    {
        if(Len != Chip->Ops.RdPage(i, Temp, SPARE_REQ | NO_ECC))
            while(1);
        for(j = 0; j < Len; j++)
        {
            if(0xFF != Temp[j])
                while(1);
        }
    }
}

void ChipRawTest(void)
{
    struct FlashChip *Chip;
    u32 i, Ret;
    u8 *Buf;
    u32 TestPages;
    u32 TestBlocks;
    u32 ECC_Flags = 0;
    u32 ErrorCount = 0;

    if(ModuleInstall_NAND("nand", 0, 0))//��װnand�豸
        while(1);

    if(HW_ECC_SUPPORTED & s_ptNandInfo->Controller)
        ECC_Flags |= HW_ECC;

    if(SW_ECC_SUPPORTED & s_ptNandInfo->Controller)
        ECC_Flags |= SW_ECC;

    if(NO_ECC_SUPPORTED & s_ptNandInfo->Controller)
        ECC_Flags |= NO_ECC;

    Chip = dListEntry(s_ptNandInfo, struct FlashChip, Descr);

    Buf = Chip->Buf;

    for(TestBlocks = 0; TestBlocks < s_ptNandInfo->BlksPerLUN; TestBlocks++)
    {
        Chip->Ops.ErsBlk(TestBlocks);

        for(TestPages = 0; TestPages < s_ptNandInfo->PagesPerBlk; TestPages++)
        {
            u32 CurPage = TestPages + (TestBlocks * s_ptNandInfo->PagesPerBlk);
            memset(Buf, 0xAA, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));
            Ret = Chip->Ops.RdPage(CurPage, Buf, NO_ECC | SPARE_REQ);
            for(i=0; i<(s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size); i++)
            {
                if(0xFF != Buf[i])
                {
                    ErrorCount += 1;
                    PrintBuf(Buf, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));
                    //while(1);
                }
            }

            PageTest(CurPage, 0);
        }
    }


    for(TestBlocks = 0; TestBlocks < s_ptNandInfo->BlksPerLUN; TestBlocks++)
    {
        if(Chip->Ops.ErsBlk(TestBlocks))
            while(1);
    }

    ContinuityTest(Chip);
    while(1);
}
#else // �½ӿ�

// ============================================================================
// ���ܣ�nand ����
// ������ucmd -- ���
//      ���� -- ���������
// ���أ�
// ��ע��
// ============================================================================
s32 __nand_req(enum ucmd cmd, ptu32_t args, ...)
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
            *block = unit / s_ptNandInfo->PagesPerBlk;
            break;
        }

        case totalblocks:
        {
            // args = &blocks
            *((u32*)args) =  s_ptNandInfo->BlksPerLUN * s_ptNandInfo->LUNs;
            break;
        }

        case blockunits:
        {
            // args = &units
            *((u32*)args) = s_ptNandInfo->PagesPerBlk;
            break;
        }

        case unitbytes:
        {
            // args = &bytes
            *((u32*)args) = s_ptNandInfo->BytesPerPage;
            break;
        }

        case checkbad:
        {
            if(badslocation==(u32)args)
                res = 1;
            else
                res = K70_BadChk((u32)args); // args = block

            break;
        }

        case savebads:
        {
            struct uopt opt = {0};
            opt.main = 1;
            if(0 != __nand_write(badslocation, (void*)args, opt)) // ���������ڵ�0ҳ
                res = -1;

            break;
        }

        case getbads:
        {
            struct uopt opt = {0};
            u32 **table = (u32**)args;

            opt.main = 1;
            if(!(*table))
            {
                res = 1;
            }
            else
            {
                res = __nand_read(badslocation, (void*)(*table), opt); // ���������ڵ�0ҳ
            }

            break;
        }

        case markbad:
        {
            res = K70_BadMark((u32)args); // args = block
            break;
        }

        case format:
        {
            va_list list;
            s32 start, end;
            u8 *tmp, escape = 0;
            struct uesz *sz;
            struct uopt opt = {0};

            start = (u32)args;
            va_start(list, args);
            end = va_arg(list, u32);
            sz = (struct uesz*)va_arg(list, u32);
            va_end(list);

            if(!sz->block)
                return (-1);

            tmp = malloc(s_ptNandInfo->BytesPerPage);
            if(!tmp)
                return (-1);

            opt.main = 1;
            if(0 ==__nand_read(badslocation, (void*)(tmp), opt)) // �������
            {
                if(nandvalidbads((u32*)tmp))
                    escape = 1; // ���ڻ������������

                if(-1==(s32)end)
                    end = s_ptNandInfo->BlksPerLUN * s_ptNandInfo->LUNs;

                do
                {
                    if((badslocation==(--end)) && escape) // ������ڵ�һҳ
                        continue;

                    if(__nand_erase((s64)end, *sz))
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

// ============================================================================
// ���ܣ�nand ����
// ������unit -- ������ţ�ҳ����
//      data -- �������ݣ�
//      opt -- ���ķ�ʽ��
// ���أ��ɹ� -- ��0����ʧ�� -- ��-1��
// ��ע��
// ============================================================================
s32 __nand_read(s64 unit, void *data, struct uopt opt)
{
    u32 flags = 0;
    s32 res;
    nandbadfreeunit(badstable, &unit, __nand_req);
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

        res = K70_PageRead((u32)unit, (u8*)data, flags);
        if (!((SPARE_REQ & flags) || (HW_ECC & flags)))
        {
            if(res != (s32)s_ptNandInfo->BytesPerPage)
            {
                return (-1);
            }
        }
        else
        {
            if(res != (s32)(s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size))
            {
                return (-1);
            }
        }
    }
//    else
//    {
//        res = K70_PageRead((u32)unit, (u8*)data);
//        if(res != (s32)s_ptNandInfo->OOB_Size)
//        {
//            return (-1);
//        }
//    }

    return (0);
}

// ============================================================================
// ���ܣ�nand д��
// ������unit -- д����ţ�ҳ����
//      data -- д�����ݣ�
//      opt -- д�ķ�ʽ��
// ���أ��ɹ� -- ��0����ʧ�� -- ��-1��
// ���أ�
// ��ע��
// ============================================================================
s32 __nand_write(s64 unit, void *data, struct uopt opt)
{
    u32 flags = 0;
    s32 res;
    nandbadfreeunit(badstable, &unit, __nand_req);
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

        res = K70_PageProgram((u32)unit, (u8*)data, flags);
        if (!((SPARE_REQ & flags) || (HW_ECC & flags)))
        {
            if(res != (s32)(s_ptNandInfo->BytesPerPage))
            {
                return (-1);
            }
        }
        else
        {
            if(res != (s32)(s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size))
            {
                return (-1);
            }
        }
    }
//    else
//    {
//        res = K70_PageProgram((u32)unit, (u8*)data);
//        if(res != (s32)s_ptNandInfo->OOB_Size)
//        {
//            return (-1);
//        }
//    }

    return (0);
}

// ============================================================================
// ���ܣ�nand ����
// ������unit -- ��������ţ�
//      sz -- �����ĵ�λ��unit��block��
// ���أ��ɹ� -- ��0����ʧ�� -- ��-1��
// ��ע��
// ============================================================================
s32 __nand_erase(s64 unit, struct uesz sz)
{
    u32 block;

    if(sz.unit)
    {
        nandbadfreeunit(badstable, &unit, __nand_req);
        block = (u32)(unit / s_ptNandInfo->PagesPerBlk);
    }
    else
    {
        block = unit;
        nandbadfreeblock(badstable, &block, __nand_req);
    }

    return (K70_BlockErase(block));
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 __nand_init(void)
{
    s_ptNandInfo = malloc(sizeof(*s_ptNandInfo));
    if(!s_ptNandInfo)
        return (-1);

    K70_MFC_Config();// оƬ�ܽŵȻ�������
    if(K70_GetNandDescr(s_ptNandInfo))
    {
        free(s_ptNandInfo);
        s_ptNandInfo = NULL;
        return (-1);
    }
    NandFlashLock = Lock_MutexCreate("Nand Flash Lock");        //����nand�Ļ�����
    s_ptNandInfo->ReservedBlks = 0;
    s_ptNandInfo->Controller = HW_ECC_SUPPORTED;
    s_ptNandInfo->BadMarkOffset = s_ptNandInfo->OOB_Size - 4 - 1;
    return (0);
}

// ============================================================================
// ���ܣ���nand���ļ�ϵͳ��������
// ������fs -- ��Ҫ���ص��ļ�ϵͳ��mediadrv -- ý��������
//       bstart -- ��ʼ�飬bend -- �����飨�������ÿ飬ֻ���ÿ����һ�飩
// ���أ�0 -- �ɹ��� -1 -- ʧ��
// ��ע��
// ============================================================================
s32 __nand_FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv)
{
    char *FullPath,*notfind;
    struct Object *targetobj;
    struct FsCore *super;
    s32 res,BlockNum;
    targetobj = OBJ_MatchPath(fs, &notfind);
    if(notfind)
    {
        error_printf("nand"," not found need to install file system.");
        return -1;
    }
    super = (struct FsCore *)OBJ_GetPrivate(targetobj);
    super->MediaInfo = nand_umedia;
    super->MediaDrv = mediadrv;

    if(-1 == (s32)bend)
    {
        bend = s_ptNandInfo->BlksPerLUN * s_ptNandInfo->LUNs;
        BlockNum = bend - bstart;
    }
    else
    {
        BlockNum = bend - bstart;
    }
    super->AreaSize = s_ptNandInfo->BytesPerPage * s_ptNandInfo->PagesPerBlk * BlockNum;
    super->MediaStart = bstart*s_ptNandInfo->PagesPerBlk; // ��ʼunit��

    res = strlen(NandFlashName)+strlen(s_ptDeviceRoot->name) + 1;
    FullPath = malloc(res);
    memset(FullPath, 0, res);
    sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,NandFlashName); //��ȡ���豸��ȫ·��
    File_BeMedia(FullPath,fs); //�����豸�����ļ�ϵͳ
    free(FullPath);

    printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, bstart, bend);
    return (0);
}


