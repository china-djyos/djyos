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
// ģ������: IICģ��ײ�Ӳ���������Ĵ�������Ĳ���
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 29/09.2015
// =============================================================================

#include "stdio.h"
#include "stddef.h"
#include "cpu_peri.h"
#include "iicbus.h"
#include "endian.h"
#include "lock.h"
#include "int.h"
#include "djyos.h"
#include "board-config.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_I2C(u8 port);
//    #if CFG_I2C1_ENABLE==1
//    ModuleInstall_I2C(CN_I2C1);
//    #endif
//
//    #if CFG_I2C2_ENABLE==1
//    ModuleInstall_I2C(CN_I2C2);
//    #endif
//
//    #if CFG_I2C3_ENABLE==1
//    ModuleInstall_I2C(CN_I2C3);
//    #endif
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip iic"//CPU��iic��������
//parent:"iicbus"    //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                     //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                  //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                    //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                   //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                    //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"iicbus","int","lock"//������������������������none����ʾ�������������
                                    //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                    //����������������������г����á�,���ָ�
//weakdependence:"none"             //��������������������������none����ʾ�������������
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
//%$#@num,16,512,
#define CFG_I2C_BUF_LEN         128         //"IIC��������С",
//%$#@enum,true,false,
#define CFG_I2C1_ENABLE         false       //"�Ƿ�����ʹ��IIC1",
#define CFG_I2C2_ENABLE         false       //"�Ƿ�����ʹ��IIC2",
#define CFG_I2C3_ENABLE         false       //"�Ƿ�����ʹ��IIC3",
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

#define tagI2CReg  I2C_TypeDef
static tagI2CReg volatile * const tg_I2CReg[] = {(tagI2CReg *)I2C1_BASE,
                                                (tagI2CReg *)I2C2_BASE,
                                                (tagI2CReg *)I2C3_BASE};


static struct IIC_CB *s_ptIIC1_CB;
static struct IIC_CB *s_ptIIC2_CB;
static struct IIC_CB *s_ptIIC3_CB;

//#define CFG_I2C_BUF_LEN  128

// =============================================================================
// ========================IOģ��IIC�ӿ�=========================================
// =============================================================================
//#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
//#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
//#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))
//
//#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414
//#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410
//
//#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //out
//#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //in

//IO��������
//#define SDA_IN()  {GPIOB->MODER&=~(3<<(6*2));GPIOB->MODER|=0<<6*2;}   //PB9����ģʽ
//#define () {GPIOB->MODER&=~(3<<(6*2));GPIOB->MODER|=1<<6*2;} //PB9���ģʽ
//IO��������
//#define IIC_SCL    PBout(7) //SCL
//#define IIC_SDA    PBout(6) //SDA
//#define READ_SDA   PBin(6)  //����SDA
extern bool_t Board_IicGpioInit(u8 I2Cx);
extern void IIC_SetDaIn(u8 IICx);
extern void IIC_SetDaOut(u8 IICx);
extern void IIC_Scl(u8 IICx,u8 level);
extern void IIC_Sda(u8 IICx,u8 level);
extern u8 IIC_ReadSda(u8 IICx);

// =============================================================================
// ���ܣ�IICӲ����ʼ������Ҫ��GPIO��ʱ�Ӻ�����
// ������ ��
// ���أ���
// =============================================================================
static void IIC_Init(u8 I2Cx)
{
    Board_IicGpioInit(I2Cx);
    IIC_Scl(I2Cx,1);
    IIC_Sda(I2Cx,1);
}

// =============================================================================
// ���ܣ�����IIC��ʼ�ź�
// ������ ��
// ���أ���
// =============================================================================
static void IIC_Start(u8 I2Cx)
{
    IIC_SetDaOut(I2Cx);     //sda�����
    IIC_Sda(I2Cx,1);
    IIC_Scl(I2Cx,1);
    DJY_DelayUs(4);
    IIC_Sda(I2Cx,0);//START:when CLK is high,DATA change form high to low
    DJY_DelayUs(4);
    IIC_Scl(I2Cx,0);//ǯסI2C���ߣ�׼�����ͻ��������
}

// =============================================================================
// ���ܣ�����IICֹͣ�ź�
// ������ ��
// ���أ���
// =============================================================================
static void IIC_Stop(u8 I2Cx)
{
    IIC_SetDaOut(I2Cx);//sda�����
    IIC_Scl(I2Cx,0);
    IIC_Sda(I2Cx,0);//STOP:when CLK is high DATA change form low to high
    DJY_DelayUs(4);
    IIC_Scl(I2Cx,1);
    IIC_Sda(I2Cx,1);//����I2C���߽����ź�
    DJY_DelayUs(4);
}

// =============================================================================
// ���ܣ��ȴ�Ӧ���źŵ���
// ������ ��
// ���أ�1������Ӧ��ʧ��,0������Ӧ��ɹ�
// =============================================================================
static u8 IIC_Wait_Ack(u8 I2Cx)
{
    u8 ucErrTime=0;
    IIC_SetDaIn(I2Cx);      //SDA����Ϊ����
    IIC_Sda(I2Cx,1);
    DJY_DelayUs(1);
    IIC_Scl(I2Cx,1);
    DJY_DelayUs(1);
    while(IIC_ReadSda(I2Cx))
    {
        ucErrTime++;
        DJY_DelayUs(1);
        if(ucErrTime>250)
        {
            IIC_Stop(I2Cx);
            return 1;
        }
    }
    IIC_Scl(I2Cx,0);//ʱ�����0
    return 0;
}

// =============================================================================
// ���ܣ�����Ӧ���ź�
// ������ ��
// ���أ���
// =============================================================================
static void IIC_Ack(u8 I2Cx)
{
    IIC_Scl(I2Cx,0);
    IIC_SetDaOut(I2Cx);
    IIC_Sda(I2Cx,0);
    DJY_DelayUs(2);
    IIC_Scl(I2Cx,1);
    DJY_DelayUs(2);
    IIC_Scl(I2Cx,0);
}

// =============================================================================
// ���ܣ�������ACKӦ��
// ������ ��
// ���أ�1������Ӧ��ʧ��,0������Ӧ��ɹ�
// =============================================================================
static void IIC_NAck(u8 I2Cx)
{
    IIC_Scl(I2Cx,0);
    IIC_SetDaOut(I2Cx);
    IIC_Sda(I2Cx,1);
    DJY_DelayUs(2);
    IIC_Scl(I2Cx,1);
    DJY_DelayUs(2);
    IIC_Scl(I2Cx,0);
}

// =============================================================================
// ���ܣ�IIC����һ���ֽ�,���شӻ�����Ӧ��
// ������ ��
// ���أ�1����Ӧ��,0����Ӧ��
// =============================================================================
static void IIC_Send_Byte(u8 I2Cx,u8 txd)
{
    u8 t;
    IIC_SetDaOut(I2Cx);
    IIC_Scl(I2Cx,0);//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {
        IIC_Sda(I2Cx,(txd&0x80)>>7);
        txd<<=1;
        DJY_DelayUs(2);   //��TEA5767��������ʱ���Ǳ����
        IIC_Scl(I2Cx,1);
        DJY_DelayUs(2);
        IIC_Scl(I2Cx,0);
        DJY_DelayUs(2);
    }
}

// =============================================================================
// ���ܣ���1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
// ������ ��
// ���أ�1����Ӧ��,0����Ӧ��
// =============================================================================
static u8 IIC_Read_Byte(u8 I2Cx,unsigned char ack)
{
    unsigned char i,receive=0;
    IIC_SetDaIn(I2Cx);//SDA����Ϊ����
    for(i=0;i<8;i++ )
    {
        IIC_Scl(I2Cx,0);
        DJY_DelayUs(2);
        IIC_Scl(I2Cx,1);
        receive<<=1;
        if(IIC_ReadSda(I2Cx))
            receive++;
        DJY_DelayUs(1);
    }
    if (!ack)
        IIC_NAck(I2Cx);//����nACK
    else
        IIC_Ack(I2Cx); //����ACK
    return receive;
}

// =============================================================================
// ����: Ӳ��GPIO��ʼ����������Դ��
// ����: IIC_NO,���
// ����: ��
// =============================================================================
void __IIC_GpioConfig(u8 IIC_NO)
{
    switch(IIC_NO)
    {
    case CN_I2C1:
        IIC_Init(IIC_NO);
        break;
    case CN_I2C2:
        break;
    case CN_I2C3:
        break;
    default:
        break;
    }
}

// =============================================================================
// ���ܣ���ѯ��ʽ��IIC���豸������
// ������reg,�Ĵ�����ַ
//       devaddr,�豸��ַ
//       addr,���ʵļĴ�����ַ
//       buf,�洢����
//       len,���ݳ��ȣ��ֽ�
// ���أ�len,��ȡ�ɹ�;-1,��ȡʧ��
// =============================================================================
static u32 __IIC_ReadPoll(volatile tagI2CReg *reg,u8 devaddr,u32 memaddr,
                        u8 maddrlen, u8 *buf, u32 len)
{
    u32 i;
    u8 iic;

    for(i = 0; i < CN_I2C_NUM; i++)
    {
        if(reg == tg_I2CReg[i])
            break;
    }
    if(i == CN_I2C_NUM)
        return -1;

    iic = i;

    u8 mem_addr_buf[4];
    //����ַ����С�˱任
    fill_little_32bit(mem_addr_buf,0,memaddr);

    u8 temp=0;
    temp = (devaddr<<1)| CN_IIC_WRITE_FLAG;
    IIC_Start(iic);

    IIC_Send_Byte(iic,temp);       //����д����
    IIC_Wait_Ack(iic);
    for(i = 0; i < maddrlen; i++)
    {
        IIC_Send_Byte(iic,mem_addr_buf[i]);   //���͵͵�ַ
        IIC_Wait_Ack(iic);
    }

    temp = (devaddr<<1)| CN_IIC_READ_FLAG;
    IIC_Start(iic);
    IIC_Send_Byte(iic,temp);           //�������ģʽ
    IIC_Wait_Ack(iic);

    for(i = 0; i < len - 1; i ++)
    {
        buf[i]=IIC_Read_Byte(iic,1);
    }
    buf[i] = IIC_Read_Byte(iic,0);
    IIC_Stop(iic);//����һ��ֹͣ����
    DJY_DelayUs(1000);
    return len;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// =============================================================================
// ���ܣ���ѯ��ʽ��IIC���豸��д����
// ������reg,�Ĵ�����ַ
//       devaddr,�豸��ַ
//       memaddr,�豸�ڲ���ַ
//       maddrlen,�豸�ڲ���ַ����
//       buf,�洢����
//       len,���ݳ��ȣ��ֽ�
// ���أ�len,��ȡ�ɹ�;-1,��ȡʧ��
// =============================================================================
static u32 __IIC_WritePoll(volatile tagI2CReg *reg,u8 devaddr,u32 memaddr,
                        u8 maddrlen, u8 *buf, u32 len)
{
    u8 mem_addr_buf[4];
    u8 iic;
    u32 i;

    for(i = 0; i < CN_I2C_NUM; i++)
    {
        if(reg == tg_I2CReg[i])
            break;
    }
    if(i == CN_I2C_NUM)
        return -1;

    iic = i;
    //����ַ����С�˱任
    fill_little_32bit(mem_addr_buf,0,memaddr);

    IIC_Start(iic);

    IIC_Send_Byte(iic,0XA0);        //����д����
    IIC_Wait_Ack(iic);

    for(i = 0; i < maddrlen; i++)
    {
        IIC_Send_Byte(iic,mem_addr_buf[i]);   //���͵͵�ַ
        IIC_Wait_Ack(iic);
    }

    for(i = 0; i < len; i++)
    {
        IIC_Send_Byte(iic,buf[i]);     //�����ֽ�
        IIC_Wait_Ack(iic);
    }
    IIC_Stop(iic);//����һ��ֹͣ����
    DJY_DelayUs(1000);
    return len;
}
#pragma GCC diagnostic pop

// =============================================================================
// ����: ��ֹiic�ж�,�����뷢�͹���һ���ж�Դ��
// ����: reg,�������ļĴ�����ָ��
// ����: ��
// ע��F4�ļĴ���������F1/F3/F7/H7/L4����һ����
// =============================================================================
static void __IIC_IntDisable(tagI2CReg *reg)
{
    reg->CR2 &= ~(I2C_IT_EVT);
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
// ���أ�len,��ȡ�ɹ�;-1,��ȡʧ��
// =============================================================================
static bool_t __IIC_WriteReadPoll(ptu32_t reg,u8 DevAddr,u32 MemAddr,\
                                u8 MemAddrLen,u8* Buf, u32 Length,u8 WrRdFlag)
{
    __IIC_IntDisable((tagI2CReg *)reg);
    if(WrRdFlag == CN_IIC_WRITE_FLAG)   //д
    {
        if(Length == __IIC_WritePoll((tagI2CReg *)reg,DevAddr,MemAddr,MemAddrLen,Buf,Length))
            return true;
        else
            return false;
    }
    else                                //��
    {
        if(Length == __IIC_ReadPoll((tagI2CReg *)reg,DevAddr,MemAddr,MemAddrLen,Buf,Length))
            return true;
        else
            return false;
    }
}

// =============================================================================
// ���ܣ�IIC�ײ������ĳ�ʼ�����������IIC���ߵĳ�ʼ��������Ҫ�������£�
//       1.��ʼ�����߿��ƿ�IIC_CB���ص������ͻ������ĳ�ʼ����ֵ��
//       2.Ĭ�ϵ�Ӳ����ʼ������GPIO��IIC�Ĵ����ȣ�
//       3.�жϳ�ʼ������ɶ�д�ж����ã�
//       4.����IICBusAdd��IICBusAdd_r�������߽�㣻
// ������para,�޾�������
// ���أ���
// =============================================================================
bool_t ModuleInstall_I2C(u8 port)
{
    struct IIC_CB **ICB;
    struct IIC_Param IIC_Config;

    switch(port)
    {
    case CN_I2C1:
        IIC_Config.BusName = "IIC1";
        ICB                = &s_ptIIC1_CB;
        IIC_Config.IICBuf  = NULL;
        break;
    case CN_I2C2:
        IIC_Config.BusName = "IIC2";
        IIC_Config.IICBuf  = NULL;
        ICB                = &s_ptIIC2_CB;
        break;
    case CN_I2C3:
        IIC_Config.BusName = "IIC3";
        IIC_Config.IICBuf  = NULL;
        ICB                = &s_ptIIC3_CB;
        break;
    default: return false;
    }

    IIC_Config.IICBufLen           = CFG_I2C_BUF_LEN;
    IIC_Config.SpecificFlag        = (ptu32_t)tg_I2CReg[port];
    IIC_Config.pWriteReadPoll      = (WriteReadPoll)__IIC_WriteReadPoll;
    //stm32��IIC�������⣬�ʲ���IOģ�ⷽʽ������ʵ�����º���
    IIC_Config.pGenerateWriteStart = NULL;
    IIC_Config.pGenerateReadStart  = NULL;
    IIC_Config.pGenerateEnd        = NULL;
    IIC_Config.pBusCtrl            = NULL;

    __IIC_GpioConfig(port);
//    __IIC_HardDefaultSet(IIC_Config.SpecificFlag);
//    __IIC_IntConfig(intline,__IIC_ISR);

    *ICB = IIC_BusAdd(&IIC_Config);
    if(*ICB)
        return 0;
    else
        return 1;
}

