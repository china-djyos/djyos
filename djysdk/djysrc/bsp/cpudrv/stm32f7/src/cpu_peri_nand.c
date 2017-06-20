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
//

#include <string.h>
#include <stdlib.h>
#include <driver.h>
#include <driver/flash/flash.h>
#include <cpu_peri.h>
#include <djyos.h>
#include "stm32f7xx_hal_conf.h"

#define NAND_RB                ((GPIO_GetData(GPIO_D)&PIN6)>>6) //NAND Flash����/æ����
#define NAND_ADDRESS            0X80000000  //nand flash�ķ��ʵ�ַ,��NCE3,��ַΪ:0X8000 0000
#define NAND_CMD                1<<16       //��������
#define NAND_ADDR               1<<17       //���͵�ַ

//NAND FLASH״̬
#define NSTA_READY              0X40        //nand�Ѿ�׼����
#define NSTA_ERROR              0X01        //nand����
#define NSTA_TIMEOUT            0X02        //��ʱ

//#define SetNandCommand(cmd)                 (*(vu8*)(NAND_ADDRESS|NAND_CMD)=cmd)
//#define SetNandData(data)                   (*(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)data)
//#define GetNandData()                       (*(vu8*)NAND_ADDRESS)
#define CHIP_ID          0XDC909556  //MT29F4G08ABADA
static u8 *s_pu8HammingCode;    // ECCУ����
#define s_u8SizeofHammingCode   (4)

static struct NandDescr *s_ptNandInfo;
static void ResetNand(void);
static s32 StatusOfNand(void);
static bool_t WaitNandReady(void);

static NAND_HandleTypeDef NAND_Handler;    //NAND FLASH���
//-----------------------------------------------------------------------------
//����:
//����:
//���: ">0" -- д�ɹ�; "-2" -- дʧ��;
//����:
//-----------------------------------------------------------------------------
static s32 stm32f7_SpareProgram(u32 PageNo, const u8 *Data)
{
    u32 i;
    s32 Ret;
    u32 SpareOffset = s_ptNandInfo->BytesPerPage;


    *(vu8*)(NAND_ADDRESS|NAND_CMD)=PAGE_PROGRAM_CMD_BYTE1;
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)(SpareOffset&0xff);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)((SpareOffset>>8)&0xff);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)PageNo;
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)((PageNo>>8)&0xff);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)((PageNo>>16)&0xff);
    Djy_DelayUs(1);

    for(i = 0; i < s_ptNandInfo->OOB_Size; i++)
        *(vu8*)NAND_ADDRESS=*(vu8*)Data++;

    *(vu8*)(NAND_ADDRESS|NAND_CMD)=PAGE_PROGRAM_CMD_BYTE2; // д��Main�������
    WaitNandReady();//ʱ��Ҫ��
    Ret = StatusOfNand();
    if(Ret)
        return (-2);

    return (s_ptNandInfo->OOB_Size);
}
//-----------------------------------------------------------------------------
//����:
//����:
//���: ">0" -- ���ɹ�; "-2" -- дʧ��;
//����:
//-----------------------------------------------------------------------------
static s32 stm32f7_SpareRead(u32 PageNo, u8 *Data)
{
    u8 i;
    s32 Ret;
    u32 SpareOffset = s_ptNandInfo->BytesPerPage;

    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(PAGE_READ_CMD_BYTE1);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8) SpareOffset&0xff);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)((SpareOffset>>8)&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)(PageNo&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)((PageNo>>8&0xff)));
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)(PageNo>>16&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(PAGE_READ_CMD_BYTE2);
    WaitNandReady();// ʱ��Ҫ��

    for(i = 0; i < s_ptNandInfo->OOB_Size; i++)
        Data[i] = (*(vu8*)NAND_ADDRESS);

    Ret = StatusOfNand();
    if(Ret)
        return (-2);

    return (s_ptNandInfo->OOB_Size);
}
static u8 NAND_ReadStatus(void)
{
    vu8 data=0;
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(STATUS_READ_CMD_BYTE);
    Djy_DelayUs(1);
    data=(*(vu8*)NAND_ADDRESS);
    return data;
}
static u8 NAND_WaitForReady(void)
{
    u8 status=0;
    vu32 time=0;
    while(1)
    {
        status=NAND_ReadStatus();
        if(status&NSTA_READY)break;
        time++;
        if(time>=0X1FFFF)return NSTA_TIMEOUT;
    }
    return NSTA_READY;
}

//-----------------------------------------------------------------------------
//����:
//����: Data -- ���ݿռ䡣����ΪNULL��
//      Flags --
//����: ">0" -- ������д����ֽ���;
//      "-1" -- ��������;
//      "-2" -- дʧ��;
//��ע:
//-----------------------------------------------------------------------------
static s32  stm32f7_PageProgram(u32 PageNo, u8 *Data, u32 Flags)
{

    u32 i, EccOffset,ECC_DATE;
    s32 Ret;
    u8 *Spare;
    Spare = (u8*)Data + s_ptNandInfo->BytesPerPage;// ע�⣺�����ǻ�����������ͳһ�Ļ�����߼�
    EccOffset = s_ptNandInfo->OOB_Size - s_u8SizeofHammingCode;// 4���ֽ�(1-bit ECCУ��)
    switch (Flags & MASK_ECC)
    {
        case HW_ECC :
            HAL_NAND_ECC_Enable(&NAND_Handler);
            break;
        case SW_ECC :
        case NO_ECC :
            HAL_NAND_ECC_Disable(&NAND_Handler);
            break;
        default :return (-1);
    }
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=PAGE_PROGRAM_CMD_BYTE1;
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)0;
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)(0);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)PageNo;
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)((PageNo>>8)&0xff);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)((PageNo>>16)&0xff);
    Djy_DelayUs(1);

    for(i = 0; i < s_ptNandInfo->BytesPerPage; i++)
        *(vu8*)NAND_ADDRESS=*(vu8*)Data++;

    if(HW_ECC & Flags)
    {
        HAL_NAND_GetECC(&NAND_Handler,&ECC_DATE,1000);
        HAL_NAND_ECC_Disable(&NAND_Handler);
        *(u32*)(Spare + EccOffset)=ECC_DATE;
    }

    *(vu8*)(NAND_ADDRESS|NAND_CMD)=PAGE_PROGRAM_CMD_BYTE2;// д��Main�������
    WaitNandReady();// ʱ��Ҫ��

    Ret = StatusOfNand();
    if(Ret)
        return (-2);

    if (!((SPARE_REQ & Flags) || (HW_ECC & Flags)))
        return (s_ptNandInfo->BytesPerPage);// ֻдҳ,�����˳�

    if(!(SPARE_REQ & Flags))
        memset(Spare, 0xFF, EccOffset);// δҪ��дspare,��Ĭ��д0xFF

    if(-2 == stm32f7_SpareProgram(PageNo, Spare))
        return (-2);

    return (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size);

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
static s32  stm32f7_PageRead(u32 PageNo, u8 *Data, u32 Flags)
{
    //�߼�:
    //
    u32 i;
    s32 Ret;
    u8 *Spare;
    u32 EccRet, EccOffset;
    u32 ECCval;

    switch (Flags & MASK_ECC)
    {
        case HW_ECC :
            HAL_NAND_ECC_Enable(&NAND_Handler);
            break;
        case SW_ECC :
        case NO_ECC :
            HAL_NAND_ECC_Disable(&NAND_Handler);
            break;
        default : return (-1);
    }
    u32 SpareOffset=0;
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(PAGE_READ_CMD_BYTE1);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8) SpareOffset&0xff);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)((SpareOffset>>8)&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)(PageNo&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)((PageNo>>8&0xff)));
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)(PageNo>>16&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(PAGE_READ_CMD_BYTE2);
    WaitNandReady();// ʱ��Ҫ��

    for(i = 0; i < s_ptNandInfo->BytesPerPage; i++)
        Data[i] = (*(vu8*)NAND_ADDRESS);

    Ret = StatusOfNand();
    if(Ret)
        return (-2);

    if (!((SPARE_REQ & Flags) || (HW_ECC & Flags)))
        return (s_ptNandInfo->BytesPerPage);// ֻ��ҳ,�����˳�

    Spare = Data + s_ptNandInfo->BytesPerPage;// ע�⣺�����ǻ�����������ͳһ�Ļ�����߼�
    EccOffset = s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size - s_u8SizeofHammingCode;
    if(HW_ECC & Flags)
    {
       HAL_NAND_GetECC(&NAND_Handler,&ECCval,1000);
       HAL_NAND_ECC_Disable(&NAND_Handler);
    }
    if(-2 == stm32f7_SpareRead(PageNo, Spare))
        return (-2);

    if((HW_ECC & Flags))
    {
        for(i=0;i<s_u8SizeofHammingCode;i++)
            if((ECCval>>(8*i)&0xff) != Data[EccOffset+i])//�ж�Ч�����Ƿ���ȷ
            {
                for(i=0;i<s_u8SizeofHammingCode;i++)//�ж��Ƿ�Ϊ������оƬ
                   if( Data[EccOffset+i]!=0xff)
                   {    //Ч��������Ҳ��Ǹղ�����оƬ�������Ч��ķ�������
                       EccRet = hamming_verify_256x(Data, s_ptNandInfo->BytesPerPage,& Data[EccOffset]);
                       if (EccRet && (EccRet != HAMMING_ERROR_SINGLE_BIT))
                       {
                           TraceDrv(FLASH_TRACE_DEBUG, "cannot be fixed");
                           return (-3);
                       }
                   }

            }
    }

    if(SW_ECC & Flags)
    {

        for(i = 0; i < s_u8SizeofHammingCode; i++)
        {
            s_pu8HammingCode[i] = Data[EccOffset+i];
        }

        EccRet = hamming_verify_256x(Data, s_ptNandInfo->BytesPerPage, s_pu8HammingCode);
        if (EccRet && (EccRet != HAMMING_ERROR_SINGLE_BIT))
        {
            TraceDrv(FLASH_TRACE_DEBUG, "cannot be fixed");
            return (-3);
        }
    }
    return (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size);
}

//-----------------------------------------------------------------------------
//����:������
//����:���
//����: "0" -- �ɹ�;"-1" -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
static s32 stm32f7_BlockErase(u32 BlkNo)
{
    s32 Ret;
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(BLOCK_ERASE_CMD_BYTE1);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)((BlkNo << 6)&0xff));// 3����ַ���ڱ�ʾ����ҳ��
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)((BlkNo >> 2)&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)((BlkNo >> 10)&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(BLOCK_ERASE_CMD_BYTE2);
    WaitNandReady();
    Ret = StatusOfNand();
    return (Ret);
}

//-----------------------------------------------------------------------------
//����:����ĺû�
//����:���
//����: "0" -- �ÿ�;
//      "-1" -- �ÿ�;
//      "-2" -- �ڴ治��
//      "-3" -- ��ʧ��;
//��ע:
//-----------------------------------------------------------------------------
static s32 stm32f7_BadChk(u32 BlkNo)
{
    u8 *Spare, i;
    s32 Ret = 0;
    u32 PageNo = BlkNo * s_ptNandInfo->PagesPerBlk;

    Spare = malloc (s_ptNandInfo->OOB_Size);
    if (NULL == Spare)
        return (-2);

    for (i = 0; i < 2; i++)// ֻ�ж�ÿ�������ҳ
    {
        if(-2 == stm32f7_SpareRead(PageNo + i, Spare))
        {
            Ret = -3;
            break;
        }
        if ((0xAA == Spare[s_ptNandInfo->BadMarkOffset]) &&
            (0xAA == Spare[s_ptNandInfo->BadMarkOffset-1]))
        {
            Ret = -1;
            break;
        }
    }
    free(Spare);
    return (Ret);
}
//-----------------------------------------------------------------------------
//����:
//����:
//����: "0" -- �ɹ�;
//      "-1" -- ʧ��;
//      "-2" -- �ڴ治��;
//��ע:
//-----------------------------------------------------------------------------
static s32  stm32f7_BadMark(u32 BlkNo)
{
    //�߼�:
    //    ����BAD MARKλԭ���Ƕ����Լ��Ƿ���ECCУ������, ֻ�ܽ���λ��־Ϊ����

    u8 *Spare, i;
    s32 Ret = 0;
    u32 PageNo = BlkNo * s_ptNandInfo->PagesPerBlk;

    Spare = malloc (s_ptNandInfo->OOB_Size);
    if (NULL == Spare)
        return (-2);
    memset(Spare, 0xFF, s_ptNandInfo->OOB_Size);
    Spare[s_ptNandInfo->BadMarkOffset] = 0xAA;
    Spare[s_ptNandInfo->BadMarkOffset-1] = 0xAA;

    for (i = 0; i < 2; i++)
    {
        if(-2 == stm32f7_SpareProgram(PageNo + i, Spare))
            Ret = -1;
    }
    free(Spare);
    return (Ret);
}
//-----------------------------------------------------------------------------
//����:��ȡnand flash ���ڲ���Ϣ
//����:
//����: "0" -- �ɹ�; "-1" -- ����ʧ��; "-2" -- �ڴ治��;
//��ע:
//-----------------------------------------------------------------------------
static s32 stm32f7_GetNandDescr(struct NandDescr *Descr)
{
    u16 i;
    s32 Ret = 0;
    char *OnfiBuf;

    OnfiBuf = (char*)malloc(786);
    if (NULL == OnfiBuf)
        return (-1);

    memset(OnfiBuf, 0, 786);
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(PARAMETER_PAGE_READ_CMD_BYTE);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(0);
    Djy_DelayUs(26);// ʱ��Ҫ��

    for(i = 0; i < 786; i++)
        OnfiBuf[i] = (*(vu8*)NAND_ADDRESS);

    if(DecodeONFI((const char*)OnfiBuf, Descr, 0))
        Ret = -1;
    free (OnfiBuf);
    return (Ret);
}

static bool_t NAND_Reset(void)
{
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(RESET_CMD_BYTE);  //��λNAND
    if(NAND_WaitForReady()==NSTA_READY)
        return true;//��λ�ɹ�
    else
        return false;                              //��λʧ��
}

static bool_t NAND_ModeSet(u8 mode)
{
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(RESET_CMD_BYTE);//����������������
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(0X01);       //��ַΪ0X01,����mode
    *(vu8*)NAND_ADDRESS=mode;                   //P1����,����mode
    *(vu8*)NAND_ADDRESS=0;
    *(vu8*)NAND_ADDRESS=0;
    *(vu8*)NAND_ADDRESS=0;
    if(NAND_WaitForReady()==NSTA_READY)
        return true;//�ɹ�
    else
        return false;                              //ʧ��
}

u32 NAND_ReadID(void)
{
    u8 deviceid[5];
    u32 id;
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(ID_READ_CMD_BYTE); //���Ͷ�ȡID����
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(0X00);
    //IDһ����5���ֽ�
    deviceid[0]=(*(vu8*)NAND_ADDRESS);
    deviceid[1]=(*(vu8*)NAND_ADDRESS);
    deviceid[2]=(*(vu8*)NAND_ADDRESS);
    deviceid[3]=(*(vu8*)NAND_ADDRESS);
    deviceid[4]=(*(vu8*)NAND_ADDRESS);
    //þ���NAND FLASH��IDһ��5���ֽڣ�����Ϊ�˷�������ֻȡ4���ֽ����һ��32λ��IDֵ
    //����NAND FLASH�������ֲᣬֻҪ��þ���NAND FLASH����ôһ���ֽ�ID�ĵ�һ���ֽڶ���0X2C
    //�������ǾͿ����������0X2C��ֻȡ�������ֽڵ�IDֵ��
    id=((u32)deviceid[1])<<24|((u32)deviceid[2])<<16|((u32)deviceid[3])<<8|deviceid[4];
    return id;
}
//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static bool_t  stm32f7_NAND_ControllerConfig(void)
{
    FMC_NAND_PCC_TimingTypeDef ComSpaceTiming,AttSpaceTiming;

    NAND_Handler.State=HAL_NAND_STATE_RESET;
    NAND_Handler.Instance=FMC_Bank3;
    NAND_Handler.Init.NandBank=FMC_NAND_BANK3;                      //NAND����BANK3��
    NAND_Handler.Init.Waitfeature=FMC_NAND_PCC_WAIT_FEATURE_DISABLE;//�رյȴ�����
    NAND_Handler.Init.MemoryDataWidth=FMC_NAND_PCC_MEM_BUS_WIDTH_8; //8λ���ݿ��
    NAND_Handler.Init.EccComputation=FMC_NAND_ECC_ENABLE;           //��ֹECC
    NAND_Handler.Init.ECCPageSize=FMC_NAND_ECC_PAGE_SIZE_2048BYTE;  //ECCҳ��СΪ2048�ֽ�
    NAND_Handler.Init.TCLRSetupTime=10;//����TCLR(tCLR=CLE��RE����ʱ)=(TCLR+TSET+2)*THCLK,THCLK=1/180M=4.6ns
    NAND_Handler.Init.TARSetupTime=10; //����TAR(tAR=ALE��RE����ʱ)=(TAR+TSET+1)*THCLK,THCLK=1/180M=4.6n��

    ComSpaceTiming.SetupTime=10;         //����ʱ��
    ComSpaceTiming.WaitSetupTime=10;    //�ȴ�ʱ��
    ComSpaceTiming.HoldSetupTime=10;    //����ʱ��
    ComSpaceTiming.HiZSetupTime=10;     //����̬ʱ��

    AttSpaceTiming.SetupTime=10;         //����ʱ��
    AttSpaceTiming.WaitSetupTime=10;    //�ȴ�ʱ��
    AttSpaceTiming.HoldSetupTime=10;    //����ʱ��
    AttSpaceTiming.HiZSetupTime=10;     //����̬ʱ��

    HAL_NAND_Init(&NAND_Handler,&ComSpaceTiming,&AttSpaceTiming);
    NAND_Reset();                       //��λNAND
    Djy_DelayUs(100*mS);
    NAND_ModeSet(4);                    //����ΪMODE4,����ģʽ
    ResetNand();
//    u32 ID = NAND_ReadID();
    return true;
}

void HAL_NAND_MspInit(NAND_HandleTypeDef *hnand)
{
    __HAL_RCC_FMC_CLK_ENABLE();             //ʹ��FMCʱ��

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
static void ResetNand(void)
{
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(RESET_CMD_BYTE);
    WaitNandReady();
}
//-----------------------------------------------------------------------------
//����: ���NAND�����Ƿ����
//����:
//����: 0 -- ����; -1 -- ����;
//��ע: δ�ж�busy��״̬
//-----------------------------------------------------------------------------
static s32 StatusOfNand(void)
{
    u8 Status = 0x1;
    do
    {
        *(vu8*)(NAND_ADDRESS|NAND_CMD)=(STATUS_READ_CMD_BYTE);
        Status = (*(vu8*)NAND_ADDRESS);
    }
    while(Status & 0x1);

    return(0);
}
//-----------------------------------------------------------------------------
//����: �ȴ�RB����Ϊĳ����ƿ
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------

static u8 NAND_WaitRB(vu8 rb)
{
    vu16 time=0;
    while(time<10000)
    {
        time++;
        if(NAND_RB==rb)return 0;
    }
    return 1;
}
//-----------------------------------------------------------------------------
//����: Nandʱ���߼�
//����:
//����:
//��ע: ʵ�������ж�NANDоƬ��R/Bn���ţ������ж��Ǳز����ٵ�
//-----------------------------------------------------------------------------
static bool_t WaitNandReady(void)
{

    if(NAND_WaitRB(0))
        return false;
    if(NAND_WaitRB(1))
        return false;
    return true;

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

    stm32f7_NAND_ControllerConfig();// оƬ�ܽŵȻ�������

    // ��ȡNAND��Ϣ
    if(stm32f7_GetNandDescr(&ChipDesrc))
    {
        TraceDrv(FLASH_TRACE_ERROR, "����NAND��Ϣʧ��\r\n");
        return (-3);
    }

    if(StartBlk >= ChipDesrc.BlksPerLUN * ChipDesrc.LUNs)
        return (-1);

    ChipDesrc.ReservedBlks = StartBlk;
    ChipDesrc.Controller = HW_ECC_SUPPORTED;
    ChipDesrc.BadMarkOffset = ChipDesrc.OOB_Size - 4 - 1;
    Len = strlen (ChipName) + 1;

    Chip = (struct FlashChip*)malloc(sizeof(struct FlashChip) + Len);
    if (NULL == Chip)
    {
        TraceDrv(FLASH_TRACE_ERROR, "out of memory!\r\n");
        return (-2);
    }

    memset (Chip, 0x00, sizeof(*Chip));

    Chip->Type            = F_NAND;
    Chip->Descr.Nand      = ChipDesrc;
    Chip->Ops.RdPage      = stm32f7_PageRead;
    Chip->Ops.WrPage      = stm32f7_PageProgram;
    Chip->Ops.ErsBlk      = stm32f7_BlockErase;
    Chip->Ops.Special.Nand.ChkBlk = stm32f7_BadChk;
    Chip->Ops.Special.Nand.MrkBad = stm32f7_BadMark;

    strcpy(Chip->Name, ChipName);// �豸��

    s_ptNandInfo = (struct NandDescr *)&(Chip->Descr);

    Chip->Buf = (u8*)malloc(s_ptNandInfo->OOB_Size + s_ptNandInfo->BytesPerPage);// NAND�ײ㻺��
    if(NULL == Chip->Buf)
    {
        TraceDrv(FLASH_TRACE_ERROR, "out of memory!\r\n");
        free(Chip);
        return (-2);
    }

    Driver_DeviceCreate(NULL, Chip->Name, NULL, NULL, NULL, NULL, NULL, NULL, (ptu32_t)Chip);// �豸����"/dev"

    if(Flags & FLASH_ERASE_ALL)
        EarseWholeChip(Chip);

    return (0);// �ɹ�;
}
#if (0)
u32 ErrorCount = 0;
//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
void BadMarkFunctionCheck(struct FlashChip *Chip)
{
    u32 i;
    u32 Blks;

    if(NULL == Chip)
        while(1);

    Blks = Chip->Descr.Nand.BlksPerLUN * Chip->Descr.Nand.LUNs;

    for(i = 0; i < Blks; i++)
    {
        if(Chip->Ops.Special.Nand.MrkBad(i))
        {
            printf("Check blks %d error!\r\n", i);
            ErrorCount++;
        }
    }

    for(i = 0; i < Blks; i++)
    {
        if(-1 != Chip->Ops.Special.Nand.ChkBlk(i))
        {
            printf("Check blks %d error!\r\n", i);
            ErrorCount++;

        }
    }
}

void ContinuityTest(struct FlashChip *Chip)
{
    u32 i, j, Len;
    u32 Blks;
    u8 *Temp;

    if(NULL == Chip)
        while(1);

    Blks = Chip->Descr.Nand.BlksPerLUN * Chip->Descr.Nand.LUNs;

    for(i = Chip->Descr.Nand.ReservedBlks; i < Blks; i++)
    {
        if(0 != Chip->Ops.ErsBlk(i))
        {
            /* ����ʧ�� */
            printf("block %d cannot be erased!\r\n", i);
            ErrorCount++;
        }
    }

    // �������У��
    Len = (Chip->Descr.Nand.BytesPerPage + Chip->Descr.Nand.OOB_Size);
    Temp = (u8*)malloc(Len);
    if(NULL == Temp)
        printf("malloc error!!\r\n");
    for(i = (Chip->Descr.Nand.ReservedBlks * Chip->Descr.Nand.PagesPerBlk); i < (Blks * Chip->Descr.Nand.PagesPerBlk); i++)
    {
        if(Len != Chip->Ops.RdPage(i, Temp, SPARE_REQ | NO_ECC))
        {
            ErrorCount++;
            printf("block %d cannot be erased!\r\n", i);
        }

        for(j = 0; j < Len; j++)
        {
            if(0xFF != Temp[j])
            {
                ErrorCount++;
                printf("block %d cannot be erased!\r\n", i);
            }
        }
    }
}

void ChipRawTest(void)
{
    struct FlashChip *Chip;
    u32 i;
    u8 *Buf;
    u32 TestPages;
    u32 TestBlocks;
    u32 ECC_Flags ;


    if(ModuleInstall_NAND("nand", 0, 0))//��װnand�豸
        while(1);

    Chip = Container(s_ptNandInfo, struct FlashChip, Descr);
    Buf = Chip->Buf;

    ECC_Flags=NO_ECC | SPARE_REQ;
    for(u8 num=0;num<2;num++)
    {
        for(TestBlocks = 0; TestBlocks < s_ptNandInfo->BlksPerLUN; TestBlocks++)
        {
            if(ECC_Flags&HW_ECC )
                printf("HW ECC test Blks Num %d  Now Test Num %d   \r"\
                        ,s_ptNandInfo->BlksPerLUN,TestBlocks+1);
            else
                printf("NO ECC test Blks Num %d  Now Test Num %d   \r"\
                        ,s_ptNandInfo->BlksPerLUN,TestBlocks+1);

            Chip->Ops.ErsBlk(TestBlocks);

            for(TestPages = 0; TestPages < s_ptNandInfo->PagesPerBlk; TestPages++)
            {
                u32 CurPage = TestPages + (TestBlocks * s_ptNandInfo->PagesPerBlk);

                //���Բ���һҳ
                memset(Buf, 0xAA, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));
                Chip->Ops.RdPage(CurPage, Buf, ECC_Flags);
                for(i=0; i<(s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size); i++)
                {
                    if(0xFF != Buf[i])
                    {
                        if(ECC_Flags&HW_ECC)
                            printf("ECC");
                        else
                            printf("NO ECC");
                        printf("Erase failed:Block = %d , Page = %d\n\r",TestBlocks,CurPage);
                        ErrorCount++;
                    }
                }

                //����д����һҳ
                for(i = 0; i < s_ptNandInfo->BytesPerPage; i++)
                    Buf[i] = (u8)i;
                Chip->Ops.WrPage(CurPage, Buf, ECC_Flags);
                memset(Buf, 0x55, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));
                Chip->Ops.RdPage(CurPage, Buf, ECC_Flags);

                for(i=0; i<(s_ptNandInfo->BytesPerPage); i++)
                {
                    if(((u8)i) != Buf[i])
                    {
                        if(ECC_Flags&HW_ECC)
                            printf("ECC");
                        else
                            printf("NO ECC");
                        printf("WrPage or RdPage Page Error:Block = %d ,"\
                                " Page = %d\n\r",TestBlocks,CurPage);
                        ErrorCount++;
                    }
                }
            }
        }
        ECC_Flags=HW_ECC | SPARE_REQ;
    }
    printf("\n\rBad Mark Function Check ~ing Please wait...\n\r");
    BadMarkFunctionCheck(Chip);
    ContinuityTest(Chip);
    if(ErrorCount)
        printf("Test End ErrorCount = %d \n\r",ErrorCount);
    else
        printf("Test End No Error\n\r");
}

#endif

