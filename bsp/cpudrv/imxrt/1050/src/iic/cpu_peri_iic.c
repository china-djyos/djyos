//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

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
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
// =============================================================================

// �ļ���     ��cpu_peri_iic.c
// ģ������: MIMXRT1052��iic����
// ģ��汾:
// ������Ա:    chj
// ����ʱ��: 5/4.2018
// =============================================================================
#include "cpu_peri_iic.h"
#include "fsl_clock.h"
#include "fsl_lpi2c.h"
#include "fsl_iomuxc.h"

#include "cpu_peri_int_line.h"

#include "djyos.h"
#include "int.h"
#include "iicbus.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t IIC_Init(u8 iic_port);
//    #if (CFG_IIC1_ENABLE == 1)
//    IIC_Init(CN_IIC1);
//    #endif
//    #if (CFG_IIC2_ENABLE == 1)
//    IIC_Init(CN_IIC2);
//    #endif
//    #if (CFG_IIC3_ENABLE == 1)
//    IIC_Init(CN_IIC3);
//    #endif
//    #if (CFG_IIC4_ENABLE == 1)
//    IIC_Init(CN_IIC4);
//    #endif
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip iic"//CPU��iic��������
//parent:"iicbus"    //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                  //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable               //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                 //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                 //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"iicbus","int"//������������������������none����ʾ�������������
                                 //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                 //����������������������г����á�,���ָ�
//weakdependence:"none"          //��������������������������none����ʾ�������������
                                 //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                 //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                 //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_IIC == false )
//#warning  " cpu_onchip_iic  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_IIC    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,32,512,
#define CFG_IIC1_BUF_LEN              128           //"IIC1��������С",
#define CFG_IIC2_BUF_LEN              128           //"IIC2��������С",
#define CFG_IIC3_BUF_LEN              128           //"IIC3��������С",
#define CFG_IIC4_BUF_LEN              128           //"IIC4��������С",
//%$#@enum,true,false,
#define CFG_IIC1_ENABLE               false         //"�Ƿ�����ʹ��IIC1",
#define CFG_IIC2_ENABLE               false         //"�Ƿ�����ʹ��IIC2",
#define CFG_IIC3_ENABLE               false         //"�Ƿ�����ʹ��IIC3",
#define CFG_IIC4_ENABLE               false         //"�Ƿ�����ʹ��IIC4",
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_DIVIDER (5U)

#define LPI2C_CLOCK_FREQUENCY ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (LPI2C_CLOCK_SOURCE_DIVIDER + 1U))

typedef LPI2C_Type  tagI2CReg;

static tagI2CReg volatile * const tg_I2CReg[] = {(tagI2CReg *)LPI2C1_BASE,
                                                (tagI2CReg *)LPI2C2_BASE,
                                                (tagI2CReg *)LPI2C3_BASE,
                                                (tagI2CReg *)LPI2C4_BASE};

//struct IIC_IntParamSet
//{
//    struct SemaphoreLCB *pDrvPostSemp;   //�ź���
//    u32 TransCount;       //����������������
//    u32 TransTotalLen;//���յ���������
//};

//static struct IIC_IntParamSet IntParamset[CN_IIC_NUM];
static u32 IIC_BaudRate[CN_IIC_NUM]={100000,100000,100000,100000};
const char *IIC_Name[CN_IIC_NUM]=
         {"IIC1","IIC2","IIC3","IIC4"};

static u32 IIC_BUF_LEN[CN_IIC_NUM]={CFG_IIC1_BUF_LEN,CFG_IIC2_BUF_LEN,\
        CFG_IIC3_BUF_LEN,CFG_IIC4_BUF_LEN};
//iic������������
static u8 s_IIC1Buf[CFG_IIC1_BUF_LEN];
static u8 s_IIC2Buf[CFG_IIC2_BUF_LEN];
static u8 s_IIC3Buf[CFG_IIC1_BUF_LEN];
static u8 s_IIC4Buf[CFG_IIC1_BUF_LEN];

static struct IIC_CB *s_ptIIC_CB[CN_IIC_NUM];
static u8* s_IICBuf[CN_IIC_NUM]={s_IIC1Buf,
                                 s_IIC2Buf,
                                 s_IIC3Buf,
                                 s_IIC4Buf,};

// =============================================================================
// ����: ����ʹ��iic�ж�,�����뷢�͹���һ���ж�Դ��
// ����: reg,�������ļĴ�����ָ��
// ����: ��
// =============================================================================
static void __IIC_MasterIntEnable(tagI2CReg *reg)
{
    /* ���ͺͽ����ж�ͬʱʹ��*/
    LPI2C_MasterEnableInterrupts(reg,kLPI2C_MasterTxReadyFlag|kLPI2C_MasterRxReadyFlag  );
}

// =============================================================================
// ����: ������ֹiic�ж�,�����뷢�͹���һ���ж�Դ��
// ����: reg,�������ļĴ�����ָ��
// ����: ��
// =============================================================================
static void __IIC_MasterIntDisable(tagI2CReg *reg)
{
    LPI2C_MasterDisableInterrupts(reg,kLPI2C_MasterTxReadyFlag|kLPI2C_MasterRxReadyFlag );
}

// =============================================================================
// ����: �ӻ�ʹ��iic�ж�,�����뷢�͹���һ���ж�Դ��
// ����: reg,�������ļĴ�����ָ��
// ����: ��
// =============================================================================
static void __IIC_SlaveIntEnable(tagI2CReg *reg)
{
    /* ���ͺͽ����ж�ͬʱʹ��*/
    LPI2C_SlaveEnableInterrupts(reg,kLPI2C_MasterTxReadyFlag|kLPI2C_MasterRxReadyFlag   );
}

// =============================================================================
// ����: ������ֹiic�ж�,�����뷢�͹���һ���ж�Դ��
// ����: reg,�������ļĴ�����ָ��
// ����: ��
// =============================================================================
static void __IIC_SlaveIntDisable(tagI2CReg *reg)
{
    LPI2C_SlaveDisableInterrupts(reg,kLPI2C_MasterTxReadyFlag|kLPI2C_MasterRxReadyFlag  );
}

// =============================================================================
// ����: ����IICʱ��
// ����: ��
// =============================================================================

static bool_t __IIC_ClkSet(void)
{
    CLOCK_SetMux(kCLOCK_Lpi2cMux, LPI2C_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, LPI2C_CLOCK_SOURCE_DIVIDER);
    return true;
}

// =============================================================================
// ����: IIC�ж����ú���
// ����: IntLine,�ж���
//       isr,�жϷ�����ָ��
// ����: ��
// =============================================================================
static void __IIC_IntConfig(u8 iic_port,u32 (*isr)(ptu32_t))
{
        ufast_t IntLine;
        switch (iic_port)
        {
            case CN_IIC1:
                IntLine=CN_INT_LINE_LPI2C1;
                break;
            case CN_IIC2:
                IntLine=CN_INT_LINE_LPI2C2;
                break;
            case CN_IIC3:
                IntLine=CN_INT_LINE_LPI2C3;
                break;
            case CN_IIC4:
                IntLine=CN_INT_LINE_LPI2C4;
                break;
            default:return;
        }
        Int_Register(IntLine);
        Int_IsrConnect(IntLine,isr);
        Int_SettoAsynSignal(IntLine);
        Int_ClearLine(IntLine);     //�����ʼ�������ķ���fifo�յ��ж�
        Int_RestoreAsynLine(IntLine);
}

// =============================================================================
// ����: ��������ֹͣʱ�򣬽���������������֮���ͨ��
// ����: reg,iic����Ĵ�����ַ
// ����: ��
// =============================================================================
static void __IIC_MasterGenerateStop(tagI2CReg *reg)
{
    LPI2C_MasterStop(reg);//����ֹͣλ
}

// =============================================================================
// ����: ��������ֹͣʱ�򣬽���������������֮���ͨ��
// ����: reg,iic����Ĵ�����ַ
// ����: ��
// =============================================================================
static void __IIC_SlaveGenerateStop(tagI2CReg *reg)
{
    reg->STAR = LPI2C_STAR_TXNACK_MASK;//����ֹͣλ
}

// =============================================================================
// ���ܣ�����IICд
// ������ reg,�Ĵ�����ַ
//        devaddr,���������ַ�����һbit���ֶ�д
//         memaddr�Ĵ�����ַд����
//       maddrlen �Ĵ�����ַ����
//         bufд����
//       len ����
// ���أ�true/false
// =============================================================================
static bool_t __IIC_MasterWrite(tagI2CReg *reg,u8 devaddr, u32 memaddr,u8 maddrlen,\
                                                u8 *buf, u32 len)
{
    lpi2c_master_transfer_t masterXfer = {0};
    status_t reVal = kStatus_Fail;

    if(maddrlen>4)
        return false;

    masterXfer.slaveAddress = devaddr & 0xFE;
    masterXfer.direction = kLPI2C_Write;
    masterXfer.subaddress = memaddr;
    masterXfer.subaddressSize = maddrlen;
    masterXfer.data = buf;
    masterXfer.dataSize = len;
    masterXfer.flags = kLPI2C_TransferDefaultFlag;

    reVal = LPI2C_MasterTransferBlocking(reg, &masterXfer);
    if(reVal==kStatus_Success)
        return true;
    else
        return false;
}

// =============================================================================
// ���ܣ��ӻ�IICд
// ������ reg,�Ĵ�����ַ
//        devaddr,���������ַ�����һbit���ֶ�д
//         addr_buf�Ĵ�����ַд����
//       addr_len �Ĵ�����ַ����
//         bufд����
//       len ����
// ���أ�true/false
// =============================================================================
//static bool_t __IIC_SlaveWrite(tagI2CReg *reg,u8 devaddr, u8 *addr_buf, u32 addr_len,u8 *buf, u32 len)
//{
//  lpi2c_master_transfer_t masterXfer = {0};
//  status_t reVal = kStatus_Fail;
//  u8 i=0;
//
//  if(addr_len>4)
//      return false;
//
//  masterXfer.slaveAddress = devaddr & 0xFE;
//  masterXfer.direction = kLPI2C_Write;
//  masterXfer.subaddress = 0;
//  for(i=0;i<addr_len;i++)
//  {
//      masterXfer.subaddress |= addr_buf[i]<<(8*i);
//  }
//  masterXfer.subaddressSize = addr_len;
//  masterXfer.data = buf;
//  masterXfer.dataSize = len;
//  masterXfer.flags = kLPI2C_TransferDefaultFlag;
//
//  reVal = LPI2C_MasterTransferBlocking(reg, &masterXfer);
//  if(reVal==kStatus_Success)
//      return true;
//  else
//      return false;
//}

// =============================================================================
// ���ܣ���ѯ��ʽ��IIC���豸������
// ������reg,�Ĵ�����ַ
//       devaddr,���������ַ�����һbit���ֶ�д
//         memaddr//�Ĵ�����ַ
//         maddrlen�ļĴ�����ַ�ĳ���
//       addr,���ʵļĴ�����ַ
//       buf, �洢������
//       len, �洢�������ĳ���
// ���أ�len,��ȡ�ɹ�;-1,��ȡʧ��
// =============================================================================
static bool_t __IIC_MasterRead(tagI2CReg *reg,u8 devaddr,u32 memaddr,u8 maddrlen,\
                                                u8 *buf, u32 len)
{
    lpi2c_master_transfer_t masterXfer = {0};
    status_t reVal = kStatus_Fail;

    if(maddrlen>4)
        return false;

    masterXfer.slaveAddress = devaddr & 0xFE;
    masterXfer.direction = kLPI2C_Read;
    masterXfer.subaddress = memaddr;
    masterXfer.subaddressSize = maddrlen;
    masterXfer.data = buf;
    masterXfer.dataSize = len;
    masterXfer.flags = kLPI2C_TransferDefaultFlag;
    reVal=LPI2C_MasterTransferBlocking(reg,&masterXfer);
    if(reVal==kStatus_Success)
        return true;
    else
        return false;
}

// =============================================================================
// ���ܣ���ѯ��ʽ��дIIC�豸
// ������reg,�Ĵ�����ַ
//       DevAddr,�豸��ַ
//       MemAddr,�豸�ڲ���ַ
//       MemAddrLen,�豸�ڲ���ַ����
//       Buf,�洢����
//       Length,���ݳ��ȣ��ֽ�
//       WrRdFlag,��д��ǣ�Ϊ0ʱд��1ʱΪ��
// ���أ�
// =============================================================================
static bool_t __IIC_MasterWriteReadPoll(tagI2CReg *reg,u8 DevAddr,u32 MemAddr,\
                                u8 MemAddrLen,u8* Buf, u32 Length,u8 WrRdFlag)
{
    static bool_t flag;

    if(WrRdFlag == CN_IIC_WRITE_FLAG)   //д
    {
        if(true == __IIC_MasterWrite(reg,DevAddr,MemAddr,MemAddrLen,Buf,Length))
            flag = true;
        else
            flag = false;
    }
    else                                //��
    {
        if(true == __IIC_MasterRead(reg,DevAddr,MemAddr,MemAddrLen,Buf,Length))
            flag = true;
        else
            flag = false;
    }

    return  flag ;
}

// =============================================================================
// ���ܣ�IIC���߿��ƻص����������ϲ���ã�
// ������reg,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//       cmd,����
//       data1,data2,���ݣ�������������
// ���أ���
// =============================================================================
static s32 __IIC_BusCtrl(tagI2CReg *reg,u32 cmd,u32 data1,u32 data2)
{
    if( reg == NULL )
        return 0;
    switch(cmd)
    {
        case CN_IIC_SET_CLK: //����ʱ��Ƶ��
              break;
        case CN_IIC_DMA_USED://ʹ��dma����

            break;
        case CN_IIC_DMA_UNUSED://��ֹdma����
            break;
        case CN_IIC_SET_POLL:           //ʹ����ѯ��ʽ���ͽ���
            break;
        default:
            return 0;
    }
    return 1;
}

// =============================================================================
// ���ܣ�IIC�ײ������ĳ�ʼ�����������IIC���ߵĳ�ʼ��������Ҫ�������£�
//       1.��ʼ�����߿��ƿ�IIC_CB���ص������ͻ������ĳ�ʼ����ֵ��
//       2.Ĭ�ϵ�Ӳ����ʼ������GPIO��IIC�Ĵ����ȣ�
//       3.�жϳ�ʼ������ɶ�д�ж����ã�
//       4.����IICBusAdd��IICBusAdd_r�������߽�㣻
// ������iic_port  IIC��� CN_IIC1
//       Add_mode Ѱַģʽ
// ���أ���
// =============================================================================
bool_t IIC_Init(u8 iic_port)
{
    struct IIC_Param IIC_Config;
    lpi2c_master_config_t masterConfig = {0};

    IIC_Config.BusName             = (char *)IIC_Name[iic_port];
    IIC_Config.IICBuf              = (u8*)s_IICBuf[iic_port];
    IIC_Config.IICBufLen           = IIC_BUF_LEN[iic_port];
    IIC_Config.SpecificFlag        = (ptu32_t)tg_I2CReg[iic_port];
    IIC_Config.pGenerateWriteStart = (WriteStartFunc)NULL;
    IIC_Config.pGenerateReadStart  = (ReadStartFunc)NULL;
    IIC_Config.pGenerateEnd        = (GenerateEndFunc)NULL;
    IIC_Config.pBusCtrl            = (IICBusCtrlFunc)NULL;
    IIC_Config.pWriteReadPoll      = (WriteReadPoll)__IIC_MasterWriteReadPoll;

    __IIC_ClkSet();

    LPI2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Hz = IIC_BaudRate[iic_port];
    LPI2C_MasterInit((tagI2CReg*)tg_I2CReg[iic_port], &masterConfig, LPI2C_CLOCK_FREQUENCY);
    s_ptIIC_CB[iic_port] = IIC_BusAdd(&IIC_Config);
        if(s_ptIIC_CB[iic_port])
            return false;
    return true;
}

