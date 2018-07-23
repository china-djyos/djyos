//-----------------------------------------------------------------------------
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


//˼·:
//   ����һ��buf��ʾҳ��oob����,�����ǽ����Ƿֿ���ʾ��ԭ���Ƕ���������ʱ,ECCУ��
//   ȴ����Ҫ����ҳ��OOB���ݡ���˴�ECCУ��ʱ,ÿ�ζ����д������ҳ��OOB�����ݲ���

#include <string.h>
#include <stdlib.h>
#include <djyos.h>
#include <MKxxF12.h>
#include <device/flash/flash.h>
#include <include/device.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_myname.h****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    ModuleInstall_NAND(CFG_NAND_CHIP_NAME,CFG_NAND_FLAG,CFG_NAND_START_BLOCK);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu_peri_nand"                                            //��д�����������
//parent:"djyfs"                                                            //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp���                                                          //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                                                               //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                                                            //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//grade:init                                                                //��ʼ��ʱ������ѡֵ��none��init��main��none��ʾ�����ʼ����
                                                                            //init��ʾ�ڵ���main֮ǰ��main��ʾ��main�����г�ʼ��
//dependence:"djyfs","cpu_peri_pio","heap","driver","AtmelSamV7"            //������������������������none����ʾ�������������
                                                                            //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                                                            //����������������������г����á�,���ָ�
//weakdependence:"none"                                                     //��������������������������none����ʾ�������������
                                                                            //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                                                            //����������������������г����á�,���ָ�
//mutex:"none"                                                              //������������������������none����ʾ�������������
                                                                            //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CFG_NAND_START_BLOCK   //****�������Ƿ��Ѿ����ú�
#warning    cpu_peri_mcan�������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,0,100,
#define CFG_NAND_START_BLOCK        (0)         //"��ʼ��",nand�ļ�ϵͳʹ�õ���ʼ��
#define CFG_NAND_FLAG               (2)         //"���"
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_NAND_CHIP_NAME          ("nand")    //"оƬ��",оƬ����
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

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
static struct NandDescr *s_ptNandInfo;//
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
            TraceDrv(FLASH_TRACE_ERROR, "chip buffer do not exist!\r\n");
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
        return (-2);

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
            TraceDrv(FLASH_TRACE_ERROR,"Page %d ECC failed\r\n", PageNo);
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
                TraceDrv(FLASH_TRACE_DEBUG,"Page %d: ECC: %d bit fixed\r\n", PageNo, EccCnt);
        }
    }

    // ��buffer��ȡ�������ݴ�С
    if ((HW_ECC & Flags) && !(SPARE_REQ & Flags))
        Size = s_ptNandInfo->BytesPerPage;

    __Buf2Mem(BufNo, 0, Data, Size);

    if(Ret)
        return (Ret);// ʧ��;

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
        TraceDrv(FLASH_TRACE_ERROR, "�ڴ治��\r\n");
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
    if(DecodeONFI((const char*)OnfiBuf, Descr, 1))
    {
        free (OnfiBuf);
        return (-1);
    }

    Descr->Type = ONFI;
    free (OnfiBuf);
    return (0);
}
//-----------------------------------------------------------------------------
//����:
//����: ChipName --
//      Clean -- ������ʽ��;"1"--��;"0"--��
//����: "0" -- �ɹ�;
//      "-1" -- �����������;
//      "-2" -- �ڴ治��;
//      "-3" -- �豸��Ϣ��ȡʧ�ܡ�
//��ע:
//-----------------------------------------------------------------------------
s32 ModuleInstall_NAND(const char *ChipName, u32 Flags, u16 StartBlk)
{
    /*�߼�:
       1.оƬ�ܽŵȻ�������;
       2.(�����ж��߼�)��ȡоƬ����Ϣ;
       3.��dev��֧�½���NAND�ڵ�;
       4.��ʼ��NAND�ڵ�,����FLASH�Ĳ�������;
    */
    u32 Len;
    struct FlashChip *Chip;
    struct NandDescr ChipDesrc = {0};



    if (NULL == ChipName)
    {
        TraceDrv(FLASH_TRACE_ERROR, "unavailable param! \r\n");
        return (-1);
    }


    K70_MFC_Config();// оƬ�ܽŵȻ�������
#if 0
    printk("CFG[%04x]", NFC->CFG);// ����
#endif

    // ��ȡNAND��Ϣ
    if(K70_GetNandDescr(&ChipDesrc))
    {
        TraceDrv(FLASH_TRACE_ERROR, "����NAND��Ϣʧ��\r\n");
        return (-3);
    }

    if(StartBlk >= ChipDesrc.BlksPerLUN * ChipDesrc.LUNs)
        return (-1);

    ChipDesrc.ReservedBlks = StartBlk;
    ChipDesrc.Controller = HW_ECC_SUPPORTED;
    ChipDesrc.BadMarkOffset = ChipDesrc.OOB_Size - 8 -1; // 8ΪӲ��ECC��С
    Len = strlen (ChipName) + 1;

    Chip = (struct FlashChip*)malloc(sizeof(struct FlashChip) + Len);
    if (NULL == Chip)
    {
        TraceDrv(FLASH_TRACE_ERROR, "out of memory!\r\n");
        return (-2);
    }
    memset (Chip, 0x00, sizeof(*Chip));

    Chip->Type                    = F_NAND;
    Chip->Descr.Nand              = ChipDesrc;
    Chip->Ops.RdPage              = K70_PageRead;
    Chip->Ops.WrPage              = K70_PageProgram;
    Chip->Ops.ErsBlk              = K70_BlockErase;
    Chip->Ops.Special.Nand.ChkBlk = K70_BadChk;
    Chip->Ops.Special.Nand.MrkBad = K70_BadMark;


    strcpy(Chip->Name, ChipName);// �豸��

    s_ptNandInfo = &(Chip->Descr);

    Chip->Buf = (u8*)malloc(s_ptNandInfo->OOB_Size + s_ptNandInfo->BytesPerPage);// NAND�ײ㻺��
    if(NULL == Chip->Buf)
    {
        TraceDrv(FLASH_TRACE_ERROR, "out of memory!\r\n");
        free(Chip);
        return (-2);
    }

    dev_add(NULL, Chip->Name, NULL, NULL, NULL, NULL, NULL, (ptu32_t)Chip);// �豸����"/dev"

    if (Flags & FLASH_ERASE_ALL)
        EarseWholeChip(Chip);

    //RawTest(Chip);//todo:
    return (0);// �ɹ�;
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
        TraceDrv(FLASH_TRACE_ERROR,"NAND����ʧ��\r\n");
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
