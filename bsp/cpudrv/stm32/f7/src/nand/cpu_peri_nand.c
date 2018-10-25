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
//

#include <string.h>
#include <stdlib.h>
#include <device.h>
#include <device/flash/flash.h>
#include <cpu_peri.h>
#include <djyos.h>
#include <math.h>
#include "stm32f7xx_hal_conf.h"


//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern s32 __nand_part_init(u32 bstart, u32 bcount, u32 doformat);
//    ModuleInstall_UnitMedia(__nand_part_init, CFG_PARTS, ...);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu_peri_nand"//CPU��nand����
//parent:"devfile"              //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"devfile","djyfs"  //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������������������������none����ʾ�������������
                                //����������������������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����

//%$#@num,0,100,

//%$#@enum,0,1,2,3,4,..,100
#define CFG_PARTS   0     //������
//%$#@enum_config
#define CFG_PART_START      //������ʼ
#define CFG_PART_SIZE       //������С
#define CFG_PART_OPTION     //����ѡ��
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure



#define NAND_RB                ((GPIO_GetData(GPIO_D)&PIN6)>>6) //NAND Flash����/æ����
#define NAND_ADDRESS            0X80000000  //nand flash�ķ��ʵ�ַ,��NCE3,��ַΪ:0X8000 0000
#define NAND_CMD                1<<16       //��������
#define NAND_ADDR               1<<17       //���͵�ַ

//NAND FLASH״̬
#define NSTA_READY              0X40        //nand�Ѿ�׼����
#define NSTA_ERROR              0X01        //nand����
#define NSTA_TIMEOUT            0X02        //��ʱ


//static u8 *s_pu8HammingCode;    // ECCУ����
#define s_u8SizeofHammingCode   (4)

static struct NandDescr *__nandescription; // NAND��������
static void ResetNand(void);
//static s32 StatusOfNand(void);
static bool_t WaitNandReady(void);
static u8 NAND_WaitForReady(void);

static NAND_HandleTypeDef NAND_Handler;    //NAND FLASH���

static s32 gb_NandFlashReady=-3;


//-----------------------------------------------------------------------------
//����:��ȡECC������λ/ż��λ����
//����:flag�� 0/1  = ����λ/ż��
//����:������eccֵ
//-----------------------------------------------------------------------------
u16 Get_ECC_OE(u8 flag,u32 eccval)
{
    u8 i;
    u16 eccbitnum=0;
    for(i=0;i<24;i++)
    {
        if((i%2)==flag)
        {
            if((eccval>>i)&0X01)
                eccbitnum+=1<<(i>>1);
        }
    }
    return eccbitnum;
}
//-----------------------------------------------------------------------------
//����:ECC����
//����:bakecc д��nandfalsh��eccֵ
//     rdecc����һҳʱ�����ECCֵ
//����:HAMMING_ERROR_SINGLE_BIT;//1bit ECC����
//     HAMMING_ERROR_ECC;//���� 1 bit ECC����
//-----------------------------------------------------------------------------
u32  NAND_ECC_verify(u8* buf,u32 bakecc,u32 rdecc)
{
    u16 Rdo,Rde,Bako,Bake;
    u16 eccchk=0;
    u16 errorpos=0;
    u32 bytepos=0;
    Rdo=Get_ECC_OE(1,rdecc);    //��ȡrdecc������λ
    Rde=Get_ECC_OE(0,rdecc);    //��ȡrdecc��ż��λ
    Bako=Get_ECC_OE(1,bakecc);  //��ȡbakecc������λ
    Bake=Get_ECC_OE(0,bakecc);  //��ȡbakecc��ż��λ
    eccchk=Rdo^Rde^Bako^Bake;
    if(eccchk==0XFFF)
    {
        errorpos=Rdo^Bako;
        bytepos=errorpos/8;
        buf[bytepos]^=1<<(errorpos%8);
        return HAMMING_ERROR_SINGLE_BIT;//1bit ECC����
    }
    return HAMMING_ERROR_ECC;//����bit ECC����

}

//-----------------------------------------------------------------------------
//����:
//����:
//���: ">0" -- д�ɹ�; "-2" -- дʧ��;
//����:
//-----------------------------------------------------------------------------
static s32 stm32f7_SpareProgram(u32 PageNo, const u8 *Data)
{
    u32 i;
    u32 SpareOffset = __nandescription->BytesPerPage;

    *(vu8*)(NAND_ADDRESS|NAND_CMD)=PAGE_PROGRAM_CMD_BYTE1;
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)(SpareOffset&0xff);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)((SpareOffset>>8)&0xff);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)PageNo;
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)((PageNo>>8)&0xff);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=(u8)((PageNo>>16)&0xff);
    Djy_DelayUs(1);

    for(i = 0; i < __nandescription->OOB_Size; i++)
        *(vu8*)NAND_ADDRESS=*(vu8*)Data++;

    *(vu8*)(NAND_ADDRESS|NAND_CMD)=PAGE_PROGRAM_CMD_BYTE2; // д��Main�������

    Djy_DelayUs(200);
    if(NAND_WaitForReady()!=NSTA_READY)
    {
        return (-2);
    }
    return (__nandescription->OOB_Size);
}
//-----------------------------------------------------------------------------
//����:
//����:
//���: ">0" -- ���ɹ�; "-2" -- ��ʧ��; "-3" -- nandû׼����
//����:
//-----------------------------------------------------------------------------
static s32 stm32f7_SpareRead(u32 PageNo, u8 *Data)
{
    u8 i,tolerate = 0;
    u32 SpareOffset = __nandescription->BytesPerPage;

again:

    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(PAGE_READ_CMD_BYTE1);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8) SpareOffset&0xff);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)((SpareOffset>>8)&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)(PageNo&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)((PageNo>>8&0xff)));
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)(PageNo>>16&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(PAGE_READ_CMD_BYTE2);
    if(WaitNandReady() == false)//ʱ��Ҫ��
    {
        if(tolerate < 3)
        {
            tolerate++;
            goto again;
        }
        return (-3);
    }

    for(i = 0; i < __nandescription->OOB_Size; i++)
        Data[i] = (*(vu8*)NAND_ADDRESS);

    if(NAND_WaitForReady()!=NSTA_READY)
    {
        return (-2);
    }

    return (__nandescription->OOB_Size);
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
s32  stm32f7_PageProgram(u32 PageNo, u8 *Data, u32 Flags)
{

    u32 i, EccOffset,ECC_DATE;
    u8 *Spare;
    u32 *Buf = (u32 *)Data;
    Spare = (u8*)Data + __nandescription->BytesPerPage;// ע�⣺�����ǻ�����������ͳһ�Ļ�����߼�
    EccOffset = __nandescription->OOB_Size - s_u8SizeofHammingCode;// 4���ֽ�(1-bit ECCУ��)
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

    for(i = 0; i < __nandescription->BytesPerPage >> 2; i++)
        *(vu32*)NAND_ADDRESS=*(vu32*)Buf++;

    if(HW_ECC & Flags)
    {
        while(!(NAND_Handler.Instance->SR & (1 << 6)));
        HAL_NAND_GetECC(&NAND_Handler,&ECC_DATE,1000);
        HAL_NAND_ECC_Disable(&NAND_Handler);
        *(u32*)(Spare + EccOffset)=ECC_DATE;
    }

    *(vu8*)(NAND_ADDRESS|NAND_CMD)=PAGE_PROGRAM_CMD_BYTE2;// д��Main�������

    if(NAND_WaitForReady()!=NSTA_READY)
    {
        return (-2);
    }
    if (!((SPARE_REQ & Flags) || (HW_ECC & Flags)))
        return (__nandescription->BytesPerPage);// ֻдҳ,�����˳�

    if(!(SPARE_REQ & Flags))
        memset(Spare, 0xFF, EccOffset);// δҪ��дspare,��Ĭ��д0xFF

    if(-2 == stm32f7_SpareProgram(PageNo, Spare))
        return (-2);

    return (__nandescription->BytesPerPage + __nandescription->OOB_Size);

}
//-----------------------------------------------------------------------------
//����:
//����:
//����: ">0" -- �������������ֽ�����
//      "-1" -- �����������;
//      "-2" -- ��ʧ��;
//      "-3" -- ECC����ʧ��;
//      "-4" -- nandû׼����
//��ע: ���ܶ�д�Ƿ���ȷ���������ݻش�
//-----------------------------------------------------------------------------
s32  stm32f7_PageRead(u32 PageNo, u8 *Data, u32 Flags)
{
    //�߼�:
    //
    u32 i,tolerate = 0;
    u8 *Spare;
    u32 EccRet, EccOffset;
    u32 ECCval;
    u32 *Buf = (u32 *)Data;

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
again:
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(PAGE_READ_CMD_BYTE1);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8) SpareOffset&0xff);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)((SpareOffset>>8)&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)(PageNo&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)((PageNo>>8&0xff)));
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)(PageNo>>16&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(PAGE_READ_CMD_BYTE2);
    if(WaitNandReady() == false)// ʱ��Ҫ��
    {
        if(tolerate < 3)
        {
            tolerate++;
            goto again;
        }
        return (-4);
    }

    for(i = 0; i < __nandescription->BytesPerPage >> 2; i++)
        *(vu32*)Buf++ = (*(vu32*)NAND_ADDRESS);


    if(NAND_WaitForReady()!=NSTA_READY)
    {

        return (-2);
    }
    if (!((SPARE_REQ & Flags) || (HW_ECC & Flags)))
        return (__nandescription->BytesPerPage);// ֻ��ҳ,�����˳�

    Spare = Data + __nandescription->BytesPerPage;// ע�⣺�����ǻ�����������ͳһ�Ļ�����߼�
    EccOffset = __nandescription->BytesPerPage + __nandescription->OOB_Size - s_u8SizeofHammingCode;
    if(HW_ECC & Flags)
    {
       while(!(NAND_Handler.Instance->SR & (1 << 6)));
       HAL_NAND_GetECC(&NAND_Handler,&ECCval,1000);
       HAL_NAND_ECC_Disable(&NAND_Handler);
    }
    if(-2 == stm32f7_SpareRead(PageNo, Spare))
        return (-2);

    if((HW_ECC & Flags))
    {
        for(i=0;i<s_u8SizeofHammingCode;i++)
        {
            if((ECCval>>(8*i)&0xff) != Data[EccOffset+i])//�ж�Ч�����Ƿ���ȷ
            {
                for(i=0;i<s_u8SizeofHammingCode;i++)//�ж��Ƿ�Ϊ������оƬ
                {
                   if( Data[EccOffset+i]!=0xff)
                   {    //Ч��������Ҳ��Ǹղ�����оƬ�������Ч��ķ�������

                       EccRet = NAND_ECC_verify(Data,ECCval,
                                   (u32)(Data[EccOffset]|\
                                   Data[EccOffset+1]<<(1*8)|\
                                   Data[EccOffset+2]<<(2*8)|\
                                   Data[EccOffset+3]<<(3*8)));//(u32)*(&Data[EccOffset]));

                       if (EccRet && (EccRet != HAMMING_ERROR_SINGLE_BIT))
                       {
                           TraceDrv(FLASH_TRACE_DEBUG, "cannot be fixed");
                           return (-3);
                       }
                       break;
                   }
                }
            }
            break;
        }
    }
    return (__nandescription->BytesPerPage + __nandescription->OOB_Size);
}

//-----------------------------------------------------------------------------
//����:������
//����:���
//����: "0" -- �ɹ�;"-1" -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
s32 stm32f7_BlockErase(u32 BlkNo)
{

    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(BLOCK_ERASE_CMD_BYTE1);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)((BlkNo << 6)&0xff));// 3����ַ���ڱ�ʾ����ҳ��
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)((BlkNo >> 2)&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_ADDR)=((u8)((BlkNo >> 10)&0xff));
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(BLOCK_ERASE_CMD_BYTE2);

    Djy_EventDelay( 2*mS );
    if(NAND_WaitForReady()!=NSTA_READY)
    {
        return (-1);    //�ɹ�
    }

    return (0);    //�ɹ�
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
    s32 Ret = 0,ReadState = 0;
    u32 PageNo = BlkNo * __nandescription->PagesPerBlk;

    Spare = malloc (__nandescription->OOB_Size);
    if (NULL == Spare)
        return (-2);

    for (i = 0; i < 2; i++)// ֻ�ж�ÿ�������ҳ
    {
        ReadState = stm32f7_SpareRead(PageNo + i, Spare);
        if((ReadState == -2) || (ReadState == -3))
        {
            Ret = -3;
            break;
        }
        if ((0xAA == Spare[__nandescription->BadMarkOffset]) &&
            (0xAA == Spare[__nandescription->BadMarkOffset-1]))
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
    u32 PageNo = BlkNo * __nandescription->PagesPerBlk;

    Spare = malloc (__nandescription->OOB_Size);
    if (NULL == Spare)
        return (-2);
    memset(Spare, 0xFF, __nandescription->OOB_Size);
    Spare[__nandescription->BadMarkOffset] = 0xAA;
    Spare[__nandescription->BadMarkOffset-1] = 0xAA;

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

    if(deonfi((const char*)OnfiBuf, Descr, 0))
        Ret = -1;

    //�˴��޸�Descr�Ա���NANDFLASH���1M��Ϊ�쳣��Ϣ��¼�洢����,�洢����Ϊ256M NANDFLASH
    //2��LUN��1��Lun��1024Blocks(128M),ÿ��Block��64page(128k),ÿ��Page��(2k+64)bytes��
    //�쳣��Ϣ�洢ʹ��NANDFLASH���1M�ռ䣬ʹ����8��Block,512��Pages��
    Descr->BlksPerLUN -= 8;

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
    NAND_Handler.Init.TCLRSetupTime=7;//����TCLR(tCLR=CLE��RE����ʱ)=(TCLR+TSET+2)*THCLK,THCLK=1/180M=4.6ns
    NAND_Handler.Init.TARSetupTime=7; //����TAR(tAR=ALE��RE����ʱ)=(TAR+TSET+1)*THCLK,THCLK=1/180M=4.6n��

    ComSpaceTiming.SetupTime=3;         //����ʱ��
    ComSpaceTiming.WaitSetupTime=4;    //�ȴ�ʱ��
    ComSpaceTiming.HoldSetupTime=3;    //����ʱ��
    ComSpaceTiming.HiZSetupTime=3;     //����̬ʱ��

    AttSpaceTiming.SetupTime=3;         //����ʱ��
    AttSpaceTiming.WaitSetupTime=4;    //�ȴ�ʱ��
    AttSpaceTiming.HoldSetupTime=3;    //����ʱ��
    AttSpaceTiming.HiZSetupTime=3;     //����̬ʱ��

    HAL_NAND_Init(&NAND_Handler,&ComSpaceTiming,&AttSpaceTiming);
    NAND_Reset();                       //��λNAND
    Djy_DelayUs(100*mS);
    NAND_ModeSet(4);                    //����ΪMODE4,����ģʽ
    ResetNand();
    return true;
}

//void HAL_NAND_MspInit(NAND_HandleTypeDef *hnand)
//{
//    __HAL_RCC_FMC_CLK_ENABLE();             //ʹ��FMCʱ��
//
//}
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
    WaitNandReady();        //����û����nand�Ƿ�׼���õ��жϣ���ΪĿǰ��ʹ��û�����⣬����û�м����жϡ�Ҫ���жϵøĺ����ӿ�
}
#if 0
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
#endif
//-----------------------------------------------------------------------------
//����: �ȴ�RB����Ϊĳ����ƿ
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------

static u8 NAND_WaitRB(vu8 rb)
{
    vu32 time=0;
    while(time<30000)
    {
        if(NAND_RB==rb)return 0;
        time++;
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
        gb_NandFlashReady=-3;
        return gb_NandFlashReady;
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
        gb_NandFlashReady=-2;
        return gb_NandFlashReady;
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

    __nandescription = (struct NandDescr *)&(Chip->Descr);

    Chip->Buf = (u8*)malloc(__nandescription->OOB_Size + __nandescription->BytesPerPage);// NAND�ײ㻺��
    if(NULL == Chip->Buf)
    {
        TraceDrv(FLASH_TRACE_ERROR, "out of memory!\r\n");
        free(Chip);
        gb_NandFlashReady=-2;
        return gb_NandFlashReady;
    }

    dev_add(NULL, Chip->Name, NULL, NULL, NULL, NULL, NULL, (ptu32_t)Chip);// �豸����"/dev"

    if(Flags & FLASH_ERASE_ALL)
        EarseWholeChip(Chip);
    gb_NandFlashReady=0;
    return (0);// �ɹ�;
}



bool_t NandFlash_Ready(void)
{
    if(gb_NandFlashReady)
        return false;
    else
        return true;
}




/******************************************************************************
                         �����ļ�ϵͳ����
******************************************************************************/
//#ifdef EFS_ON
//-----------------------------------------------------------------------------
//����: дС��һҳ������
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------






#define __WritePage(a,b,c) stm32f7_PageProgram(a,b,c)
#define __ReadPage(a,b,c) stm32f7_PageRead(a,b,c)
static s32 __WriteFragment(u32 PageNo, u32 Offset, const u8 *Buf, u32 Size)
{
    u32 i;

    *(vu8*)(NAND_ADDRESS|NAND_CMD) = PAGE_PROGRAM_CMD_BYTE1;

    *(vu8*)(NAND_ADDRESS|NAND_ADDR) = (Offset);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR) = (Offset>>8);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR) = (PageNo);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR) = (PageNo>>8);

    for(i = 0; i < Size; i++)
        *(vu8*)NAND_ADDRESS=(Buf[i]);

    *(vu8*)(NAND_ADDRESS|NAND_CMD) = PAGE_PROGRAM_CMD_BYTE2;

    Djy_EventDelay(700);// �г�
    if(NAND_WaitForReady()!=NSTA_READY)
    {
        return (-2);
    }
    return (Size);
}

//-----------------------------------------------------------------------------
//����: ��С��һҳ������
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static s32 __ReadFragment(u32 PageNo, u32 Offset, u8 *Buf, u32 Size)
{
    u32 i,tolerate;

again:

    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(PAGE_READ_CMD_BYTE1);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR) = (Offset);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR) = (Offset>>8);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR) = (PageNo);
    *(vu8*)(NAND_ADDRESS|NAND_ADDR) = (PageNo>>8);
    *(vu8*)(NAND_ADDRESS|NAND_CMD)=(PAGE_READ_CMD_BYTE2);

    Djy_EventDelay(25);
    if(WaitNandReady() == false)//ʱ��Ҫ��
    {
        if(tolerate < 3)
        {
            tolerate++;
            goto again;
        }
        return (-3);
    }

    for(i = 0; i < Size; i++)
        Buf[i] = (*(vu8*)NAND_ADDRESS);

    if(NAND_WaitForReady()!=NSTA_READY)
    {
        return (-2);
    }

    return (Size);
}
//-----------------------------------------------------------------------------
//����: д����
//����: BlkNo -- ���;
//      Offset -- ����ƫ��;
//      Buf -- д����;
//      Size -- д���ݴ�С
//      Flags -- 0x0 -- �޹���; 0x1 - ECC���ܿ���(���ݴ�СΪ����256)
//����:
//��ע:
//-----------------------------------------------------------------------------
u32 EFS_IF_WriteData(u32 BlkNo, u32 Offset, u8 *Buf, u32 Size, u8 Flags)
{
    s32 Ret;
    u32 PageNo = (BlkNo << 6) + (Offset >> 11);
    u32 PageOffset = Offset & 0x7FF;
    u32 WrLen = Size;

    if(!Buf)
        return (0);

    // ECC������ԴΪ256�ֽڣ��������ݺ��,3���ֽ�
    if(1 == Flags)
    {
        hamming_compute_256x(Buf, 256, Buf+256);
        WrLen = Size + 3;
        PageOffset += 3 * (PageOffset >> 8);
    }

    for(;;)
    {
        if((!PageOffset) && (WrLen >= __nandescription->BytesPerPage))
            Ret = __WritePage(PageNo, Buf, NO_ECC);
        else if((WrLen >= __nandescription->BytesPerPage) || // д���ݴ������һҳ,�����ݿ�ҳ
                (((WrLen+PageOffset) > __nandescription->BytesPerPage) && (!Flags))) // д����С��һҳ,�����ݿ�ҳ(ֻ�п��Ƿ�ECC�����)
            Ret = __WriteFragment(PageNo, PageOffset, (const u8*)Buf, (__nandescription->BytesPerPage - PageOffset));
        else
            Ret = __WriteFragment(PageNo, PageOffset, (const u8*)Buf, WrLen);

        if(Ret <= 0)
            break;// ���������д��

        WrLen -= Ret;
        if(WrLen <= 0)
            break;
        Buf += Ret;
        PageNo++;
        if(PageOffset)
            PageOffset = 0;
    }

    if(Ret > 0)
        return (Size);
    else
        return (0);
}
//-----------------------------------------------------------------------------
//����: ������
//����: BlkNo -- ���;
//      Offset -- ����ƫ��;
//      Buf -- ������;
//      Size -- �����ݴ�С
//      Flags -- 0x0 -- �޹���; 0x1 - ECC���ܿ���(���ݴ�СΪ����256)
//����:
//��ע:
//-----------------------------------------------------------------------------
u32 EFS_IF_ReadData(u32 BlkNo, u32 Offset, u8 *Buf, u32 Size, u8 Flags)
{
    s32 Ret;
    u32 PageNo = (BlkNo << 6) + (Offset >> 11);
    u32 PageOffset = Offset & 0x7FF;
    u32 RdLen = Size;

    if(!Buf)
        return (0);

    if(1 == Flags)
    {
        RdLen += 3;
        PageOffset += 3 * (PageOffset >> 8);
    }

    for(;;)
    {
        if((!PageOffset) && (RdLen >= __nandescription->BytesPerPage))
            Ret = __ReadPage(PageNo, Buf, NO_ECC);
        else if((RdLen >= __nandescription->BytesPerPage) || // д���ݴ������һҳ,�����ݿ�ҳ
                (((RdLen+PageOffset) > __nandescription->BytesPerPage) && (!Flags))) // ������С��һҳ,�����ݿ�ҳ(ֻ�з�ECC�������Ҫ����)
            Ret = __ReadFragment(PageNo, PageOffset, Buf, (__nandescription->BytesPerPage - PageOffset));
        else
            Ret = __ReadFragment(PageNo, PageOffset, Buf, RdLen);

        if(Ret <= 0)
            break;// ���������д��

        RdLen -= Ret;
        if(RdLen <= 0)
            break;
        Buf += Ret;
        PageNo++;
        if(PageOffset)
            PageOffset = 0;
    }


    if(Ret <= 0)
        return (0);

    if((1 == Flags) && (256 == Size))
    {
        u8 *HammingCode;
        u32 EccRet;

        HammingCode = Buf + 256;
        EccRet = hamming_verify_256x(Buf, 256, HammingCode);
        if (EccRet && (EccRet != HAMMING_ERROR_SINGLE_BIT))
        {
            TraceDrv(FLASH_TRACE_DEBUG, "cannot be fixed");
            return (0);
        }
    }

    return (Size);
}
//-----------------------------------------------------------------------------
//����: ����
//����: BlkNo -- ���;
//����:
//��ע:
//-----------------------------------------------------------------------------
bool_t EFS_IF_Erase(u32 BlkNo)
{
    if(0==stm32f7_BlockErase(BlkNo))
        return (TRUE);
    return (FALSE);
}
//-----------------------------------------------------------------------------
//����: ����ļ��Ƿ��д
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static u8 TempBuf[259];
s32 EFS_IF_IsFragmentWritten(u32 BlkNo, u32 Offset)
{
    u16 i;
    u32 PageOffset = Offset & 0x7FF;
    u8 EccOffset = 3 * (PageOffset >> 8);

    if(0 == EFS_IF_ReadData(BlkNo, (Offset+EccOffset), TempBuf, 259, 0))
        return (-1);

    for(i = 0; i < 259; i++)
    {
        if(0xFF != TempBuf[i])
            return (-1);
    }

    return (0);
}

//-----------------------------------------------------------------------------
//����: δʵ��
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
bool_t EFS_IF_CheckBlockReady(u32 block,u32 offset,
        u8 *buf,u32 size)
{
    u32 CurBlock,CurBlockOffset;
    u32 BlockSize;
    bool_t result = TRUE;

    if(NULL == __nandescription)
    {
        return FALSE;
    }

    CurBlock = block;
    CurBlockOffset = offset;
    BlockSize = __nandescription->BytesPerPage * __nandescription->PagesPerBlk;

    while(size)
    {
        if(-1 == EFS_IF_IsFragmentWritten(CurBlock,CurBlockOffset))
        {
            result = FALSE;
            break;
        }
        CurBlockOffset += 256;
        CurBlock += CurBlockOffset / BlockSize;
        CurBlockOffset = CurBlockOffset % BlockSize;
        size = (size > 256) ? (size - 256) :0;
    }
    return result;// �ļ�д����������ڸ�д�߼�
}


#if (0)
//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
void RawTest(void)
{
    struct FlashChip *Chip;
    u32 i;
    u8 *Buf;
    u32 TestPage = 640;

    ModuleInstall_NAND("nand", 0, 0);//��װnand�豸

    Chip = dListEntry(__nandescription, struct FlashChip, Descr);

    Buf = Chip->Buf;

    memset(Buf, 0x0, (__nandescription->BytesPerPage + __nandescription->OOB_Size));

    Chip->Ops.ErsBlk(TestPage/__nandescription->PagesPerBlk);

    Chip->Ops.RdPage(TestPage, Buf, NO_ECC | SPARE_REQ);

    //PrintBuf(Buf, (__nandescription->BytesPerPage + __nandescription->OOB_Size));

    for(i = 0; i < __nandescription->BytesPerPage; i++)
        Buf[i] = (u8)i;

    Chip->Ops.WrPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    memset(Buf, 0xFF, (__nandescription->BytesPerPage + __nandescription->OOB_Size));

    Chip->Ops.RdPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    //PrintBuf(Buf, (__nandescription->BytesPerPage + __nandescription->OOB_Size));

    //����ECC����,1λ�������
    // 1��
    Buf[1] = 0;

    Chip->Ops.WrPage(TestPage, Buf, NO_ECC);

    memset(Buf, 0xFF, (__nandescription->BytesPerPage + __nandescription->OOB_Size));

    Chip->Ops.RdPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    //PrintBuf(Buf, (__nandescription->BytesPerPage + __nandescription->OOB_Size));
    // 2��
    TestPage++;
    Chip->Ops.WrPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    Buf[2] = 0;
    Chip->Ops.WrPage(TestPage, Buf, NO_ECC);

    memset(Buf, 0xFF, (__nandescription->BytesPerPage + __nandescription->OOB_Size));

    Chip->Ops.RdPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    //PrintBuf(Buf, (__nandescription->BytesPerPage + __nandescription->OOB_Size));

    // 3��
    TestPage++;
    Chip->Ops.WrPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    Buf[3] = 1;
    Chip->Ops.WrPage(TestPage, Buf, NO_ECC);

    memset(Buf, 0xFF, (__nandescription->BytesPerPage + __nandescription->OOB_Size));

    Chip->Ops.RdPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    //PrintBuf(Buf, (__nandescription->BytesPerPage + __nandescription->OOB_Size));

    //����ECC����,2λ�������,�ǲ��ܾ�����
    TestPage++;
    Chip->Ops.WrPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    Buf[3] = 0;
    Chip->Ops.WrPage(TestPage, Buf, NO_ECC | SPARE_REQ);

    memset(Buf, 0xFF, (__nandescription->BytesPerPage + __nandescription->OOB_Size));

    Chip->Ops.RdPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    //PrintBuf(Buf, (__nandescription->BytesPerPage + __nandescription->OOB_Size));
    while(1);
}
//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
void PageTest(const u32 PageNo, const u8 Init,const u32 BlkNo )
{
    volatile u8 *Buf;
    u32 i,j;
    struct FlashChip *Chip;
    u32 ECC_Flags = 0;

    if(Init)
    {
        if(ModuleInstall_NAND("nand", 0, 0)) //��װnand�豸
            while(1);
    }

    if(HW_ECC_SUPPORTED & __nandescription->Controller)
        ECC_Flags |= HW_ECC;

    if(SW_ECC_SUPPORTED & __nandescription->Controller)
        ECC_Flags |= SW_ECC;

    if(NO_ECC_SUPPORTED & __nandescription->Controller)
        ECC_Flags |= NO_ECC;

    Chip = dListEntry(__nandescription, struct FlashChip, Descr);

    Buf = Chip->Buf;

    for(i = 0; i < __nandescription->BytesPerPage; i++)
        Buf[i] = (u8)i;

    Chip->Ops.WrPage(PageNo, Buf, HW_ECC | SPARE_REQ);

    memset(Buf, 0, (__nandescription->BytesPerPage + __nandescription->OOB_Size));

    Chip->Ops.RdPage(PageNo, Buf, HW_ECC | SPARE_REQ);
    for(i=0; i<(__nandescription->BytesPerPage); i++)
    {
        if(((u8)i) != Buf[i])
            while(1);
    }
    // ECCһλ����  todo
    Buf[1] = 0x0;
    Chip->Ops.WrPage(PageNo, Buf, NO_ECC | SPARE_REQ);

    memset(Buf, 0, (__nandescription->BytesPerPage + __nandescription->OOB_Size));

    Chip->Ops.RdPage(PageNo, Buf, HW_ECC | SPARE_REQ);
    for(i=0; i<(__nandescription->BytesPerPage); i++)
    {
        if(((u8)i) != Buf[i])
            while(1);
    }


}
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
            while(1);
    }

    for(i = 0; i < Blks; i++)
    {
        if(-1 != Chip->Ops.Special.Nand.ChkBlk(i))
            while(1);
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
    u32 i;
    u8 *Buf;
    u32 TestPages;
    u32 TestBlocks;
    u32 ECC_Flags = 0;
    u32 ErrorCount = 0;

    if(ModuleInstall_NAND("nand", 0, 0))//��װnand�豸
        while(1);

    if(HW_ECC_SUPPORTED & __nandescription->Controller)
        ECC_Flags |= HW_ECC;

    if(SW_ECC_SUPPORTED & __nandescription->Controller)
        ECC_Flags |= SW_ECC;

    if(NO_ECC_SUPPORTED & __nandescription->Controller)
        ECC_Flags |= NO_ECC;

    Chip = dListEntry(__nandescription, struct FlashChip, Descr);

    Buf = Chip->Buf;

    for(TestBlocks = 0; TestBlocks < __nandescription->BlksPerLUN; TestBlocks++)
    {
        Chip->Ops.ErsBlk(TestBlocks);
        printf("test block :%d   \r",TestBlocks);
        for(TestPages = 0; TestPages < __nandescription->PagesPerBlk; TestPages++)
        {
            u32 CurPage = TestPages + (TestBlocks * __nandescription->PagesPerBlk);
            memset(Buf, 0xAA, (__nandescription->BytesPerPage + __nandescription->OOB_Size));
            Chip->Ops.RdPage(CurPage, Buf, NO_ECC | SPARE_REQ);
            for(i=0; i<(__nandescription->BytesPerPage + __nandescription->OOB_Size); i++)
            {
                if(0xFF != Buf[i])
                {
                    ErrorCount += 1;
                    //PrintBuf(Buf, (__nandescription->BytesPerPage + __nandescription->OOB_Size));
                    while(1);
                }
            }

            PageTest(CurPage, 0,TestBlocks);
        }
    }
    for(TestBlocks = 0; TestBlocks < __nandescription->BlksPerLUN; TestBlocks++)
    {
        if(Chip->Ops.ErsBlk(TestBlocks))
            while(1);
    }
    BadMarkFunctionCheck(Chip);
    ContinuityTest(Chip);
    while(1);
}

#endif

#if 1 // �½ӿ�
#include <device/include/unit_media.h>

static s32 __nand_read(s64 unit, void *data, struct uopt opt);
static s32 __nand_write(s64 unit, void *data, struct uopt opt);
static s32 __nand_erase(s64 unit, struct uesz sz);
static u32 *badstable;
static u32 badslocation = 0;


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
            *block = unit / __nandescription->PagesPerBlk;
            break;
        }

        case totalblocks:
        {
            // args = &blocks
            *((u32*)args) =  __nandescription->BlksPerLUN * __nandescription->LUNs;
            break;
        }

        case blockunits:
        {
            // args = &units
            *((u32*)args) = __nandescription->PagesPerBlk;
            break;
        }

        case unitbytes:
        {
            // args = &bytes
            *((u32*)args) = __nandescription->BytesPerPage;
            break;
        }

        case checkbad:
        {
            if(badslocation==(u32)args)
                res = 1;
            else
                res = stm32f7_BadChk((u32)args); // args = block

            break;
        }

        case savebads:
        {
            struct uopt opt = {0};
            opt.main = 1;
            if(__nandescription->BytesPerPage
                    != __nand_write(badslocation, (void*)args, opt)) // ���������ڵ�0ҳ
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

        case format:
        {
            va_list list;
            u32 start, end;
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

            tmp = malloc(__nandescription->BytesPerPage);
            if(!tmp)
                return (-1);

            opt.main = 1;
            if(__nandescription->BytesPerPage
                    ==__nand_read(badslocation, (void*)(tmp), opt)) // �������
            {
                if(nandvalidbads((u32*)tmp))
                    escape = 1; // ���ڻ������������

                if(-1==end)
                    end = __nandescription->BlksPerLUN * __nandescription->LUNs;
                else if (start)
                    end += start;

                do
                {
                    if((badslocation==(--end))&&escape) // ������ڵ�һҳ
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
// ��ע��
// ============================================================================
static s32 __nand_read(s64 unit, void *data, struct uopt opt)
{
    u32 flags = 0;

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

        return (stm32f7_PageRead((u32)unit, (u8*)data, flags));
    }
    else
    {
        return (stm32f7_SpareRead((u32)unit, (u8*)data));
    }

    return (0);
}

// ============================================================================
// ���ܣ�nand д��
// ������unit -- д����ţ�ҳ����
//      data -- д�����ݣ�
//      opt -- д�ķ�ʽ��
// ���أ�
// ��ע��
// ============================================================================
static s32 __nand_write(s64 unit, void *data, struct uopt opt)
{
    u32 flags = 0;

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

        return (stm32f7_PageProgram((u32)unit, (u8*)data, flags));
    }
    else
    {
        return (stm32f7_SpareProgram((u32)unit, (u8*)data));
    }

    return (0);
}

// ============================================================================
// ���ܣ�nand ����
// ������unit -- ��������ţ�
//      sz -- �����ĵ�λ��unit��block��
// ���أ�
// ��ע��
// ============================================================================
static s32 __nand_erase(s64 unit, struct uesz sz)
{
    u32 block;

    if(sz.unit)
    {
        nandbadfreeunit(badstable, &unit, __nand_req);
        block = (u32)(unit / __nandescription->PagesPerBlk);
    }
    else
    {
        block = unit;
        nandbadfreeblock(badstable, &block, __nand_req);
    }

    return (stm32f7_BlockErase(block));
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 __nand_init(void)
{
    __nandescription = malloc(sizeof(*__nandescription));
    if(!__nandescription)
        return (-1);

    stm32f7_NAND_ControllerConfig();// оƬ�ܽŵȻ�������
    if(stm32f7_GetNandDescr(__nandescription))
    {
        free(__nandescription);
        __nandescription = NULL;
        return (-1);
    }

    __nandescription->ReservedBlks = 0;
    __nandescription->Controller = HW_ECC_SUPPORTED;
    __nandescription->BadMarkOffset = __nandescription->OOB_Size - 4 - 1;
    return (0);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
s32 __nand_part_init(u32 bstart, u32 bcount, u32 doformat)
{
    struct umedia *um;
    struct uopt opt;
    char name[16], part[3];
    static u8 count = 0;

    if(!__nandescription)
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
        __nand_req(format, bstart , bcount, &sz);
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

    um = malloc(sizeof(struct umedia)+__nandescription->BytesPerPage+__nandescription->OOB_Size);
    if(!um)
        return (-1);

    opt.hecc = 0;
    opt.main = 1;
    opt.necc = 1;
    opt.secc = 0;
    opt.spare = 1;
    if(-1 == bcount)
    {
        bcount = __nandescription->BlksPerLUN * __nandescription->LUNs;
        bcount -= bstart;
        um->asz = __nandescription->BytesPerPage * __nandescription->PagesPerBlk * bcount;
    }
    else
    {
        um->asz = __nandescription->BytesPerPage * __nandescription->PagesPerBlk * bcount;
    }

    um->esz = log2(__nandescription->BytesPerPage * __nandescription->PagesPerBlk); //
    um->usz = log2(__nandescription->BytesPerPage);
    um->merase = __nand_erase;
    um->mread = __nand_read;
    um->mreq = __nand_req;
    um->mwrite = __nand_write;
    um->opt = opt;
    um->type = nand;
    um->ubuf = (u8*)um + sizeof(struct umedia);
    um->ustart = bstart*__nandescription->PagesPerBlk; // ��ʼunit��
    itoa(count++, part, 10);
    sprintf(name, "nand part %s", part);
    if(um_add((const char*)name, um))
    {
        printf("\r\n: erro : device : %s addition failed.", name);
        return (-1);
    }

    printf("\r\n: info : device : %s added(start:%d, blocks:%d).", name, bstart, bcount);
    return (0);
}

#endif
