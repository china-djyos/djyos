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
// ������Ա: huzb
// ����ʱ��: 29/09.2015
// =============================================================================

#include <stdint.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <cpu_peri_iic.h>
#include "cpu_peri.h"
#include "cpu_peri_uart.h"
#include "stdlib.h"
#include "iicbus.h"
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


typedef struct
{
    volatile u32 CFGR;
    volatile u32 CLKR;
    volatile u32 ADR;
    volatile u32 CMDR;
    volatile u32 SR;
    volatile u32 BUFR;
}tagI2CReg;


#define CN_I2C0_BASE (0x40200000UL)
#define CN_I2C1_BASE (0x40210000UL)
#define CN_I2C2_BASE (0x41220000UL)

static volatile tagI2CReg *sgpt_I2cReg[] =
{
    (volatile tagI2CReg*)CN_I2C0_BASE,
    (volatile tagI2CReg*)CN_I2C1_BASE,
    (volatile tagI2CReg*)CN_I2C2_BASE,
};

#define CN_IIC_NUM (3)

static u32 IIC_BUF_LEN[CN_IIC_NUM]={128,128,128};

//iic������������
static u8 s_IIC1Buf[128];
static u8 s_IIC2Buf[128];
static u8 s_IIC3Buf[128];

static u8* s_IICBuf[CN_IIC_NUM]=
{
    s_IIC1Buf,
    s_IIC2Buf,
    s_IIC3Buf,
};

struct IIC_IntParamSet
{
    struct SemaphoreLCB *pDrvPostSemp;   //�ź���
    u32 TransCount;       //����������������
    u32 TransTotalLen;//���յ���������
};

static struct IIC_IntParamSet IntParamset[CN_IIC_NUM];
static struct IIC_CB* sgpt_IIC_CB[CN_IIC_NUM];
const char *IIC_Name[CN_IIC_NUM]= {"IIC1","IIC2","IIC3"};

__attribute__((weak))  void IIC_InLowPowerPinCfg(u8 iic_port)
{
    return;
}
__attribute__((weak))  void IIC_OutLowPowerPinCfg(u8 iic_port)
{
    return ;
}


// =============================================================================
// ���ܣ� ������˳��͹���ģʽ
// ������ iic_port IIC �˿ں�
//     flag ���뻹���˳��͹���ģʽ
// ���أ�true/false
// =============================================================================
//

bool_t IIC_LowPowerConfig(u8 iic_port,u8 flag)
{

   return true;
}

// =============================================================================
// ���ܣ�IIC����Ƿ��д���
// ������ reg iic����ַ
// ���أ�true/false
// =============================================================================

static bool_t  _IIC_Chek(tagI2CReg *reg)
{
    if(reg->SR & (1<<13))
    {
       return false;
    }

    return true;
}

// =============================================================================
// ����: ʹ��iic�ж�,�����뷢�͹���һ���ж�Դ��
// ����: reg,�������ļĴ�����ָ��
// ����: ��
// =============================================================================

static void __IIC_IntEnable(tagI2CReg *reg)
{
    //���жϿ���ʹ��
    reg->CFGR |= (1<<5);
    //�����ж� �������ж�ʹ��
    reg->CFGR |= (1<<10);
}

// =============================================================================
// ����: ��ֹiic�ж�,�����뷢�͹���һ���ж�Դ��
// ����: reg,�������ļĴ�����ָ��
// ����: ��
// =============================================================================

static void __IIC_IntDisable(tagI2CReg *reg)
{
    reg->CFGR &= ~(1<<5);
}

// =============================================================================
// ����: ������������
// ����: iic_port iic��ţ�
//
//
//
// ����: ��
// =============================================================================

static bool_t __IIC_TransportBaurtClkSet(u8 iic_port,u32 speed)
{
    u32 temp;

    if(iic_port > CN_IIC_NUM)
    {
        return false;
    }

    switch(speed)
    {
        case CN_IIC_SPEED_100KHZ:
            sgpt_I2cReg[iic_port]->CLKR = CN_IIC_SPEED_100KHZ;
            break;
        case CN_IIC_SPEED_400KHZ:
            sgpt_I2cReg[iic_port]->CLKR = CN_IIC_SPEED_400KHZ;
            break;
        case CN_IIC_SPEED_1000KHZ:
            sgpt_I2cReg[iic_port]->CLKR = CN_IIC_SPEED_1000KHZ;
            break;
        default:;break;
    }
    return true;
}

// =============================================================================
// ����: I/Oģ��ķ�ʽ �ͷ�û�и�λ��ʹ�����ŵ�IIC����
// ����: port I/O�� �� CN_GPIO_B
//      TS_SDA:���ź�
//      TS_SCK:���ź�
// ����: true/false
// =============================================================================

bool_t IIC_Busfree(u32 port,u32 sda_pin,u32 sck_pin)
{


    return true;
}

// =============================================================================
// ����: Ӳ��GPIO��ʼ��������ʱ��ѡ���iic��ʼ������
// ����: IIC_NO,���
// ����: true/false
// =============================================================================
static bool_t __IIC_GpioConfig(u8 iic_port)
{
    //congig i2c clock
    u8 I2cClockBit;

    switch(iic_port)
    {
       case CN_I2C1:
            I2cClockBit = 14;
            break;
       case CN_I2C2:
            I2cClockBit = 15;
            break;
       case CN_I2C3:
            I2cClockBit = 29;
            break;
       default:;break;
    }

    CLK_Enable_Peripheral_Clk(I2cClockBit);

    sgpt_I2cReg[iic_port]->CLKR = 40000000 / (4*10);
    u32 value = sgpt_I2cReg[iic_port]->CLKR;
    //���ò�ȥë��
    sgpt_I2cReg[iic_port]->CFGR &= ~(3<<3);
    sgpt_I2cReg[iic_port]->CFGR |= (0x0<<3);

    //����7Bit �ӻ���ַ
    sgpt_I2cReg[iic_port]->ADR &= ~(0x1<<15);

    //ʹ��I2c
    sgpt_I2cReg[iic_port]->CFGR |= (0x1<<0);

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
      case CN_I2C1:
          IntLine=CN_INT_LINE_IIC0;
          break;
      case CN_I2C2:
          IntLine=CN_INT_LINE_IIC1;
          break;
      case CN_I2C3:
          IntLine=CN_INT_LINE_IIC2;
          break;
          default:return;
     }

     Int_Register(IntLine);
     Int_IsrConnect(IntLine,isr);
     Int_SettoAsynSignal(IntLine);
     Int_ClearLine(IntLine);     //�����ʼ�������ķ���fifo�յ��ж�
     Int_RestoreLine(IntLine);   //�����ʼ�������ķ���fifo�յ��ж�
}

// =============================================================================
// ����: ����ֹͣʱ�򣬽���������������֮���ͨ��
// ����: reg,iic����Ĵ�����ַ
// ����: ��
// =============================================================================

static void __IIC_GenerateStop(tagI2CReg *reg)
{


}

// =============================================================================
// ���ܣ�IICд
// ������ reg,�Ĵ�����ַ
//        devaddr,���������ַ�����һbit���ֶ�д
//         buf_addr��ַд����
//       len_addr ��ַ����
//         bufд����
//       len ����
// ���أ�true/false
// =============================================================================

static bool_t __IIC_Write(tagI2CReg *reg,u8 devaddr, u32 memaddr, u32 addr_len,u8 *buf, u32 len)
{
    u32 timeout=0;
    u32 Time;
    u32 i=0;
    u32 datLoop = 0;
    u8  addressLoop;
    u32 internalDevAddress = 0;
    u32 datToSend = 0;

    //get device internal address
    //from hight to low
    internalDevAddress =  memaddr;

    while(datToSend < len)
    {

        //1��Write Dev Address
        reg->BUFR = (devaddr & 0xfe);//Write Into Device Address
        //Set Cpu Master Mode and Get Start Singnal To Send
        reg->CMDR = (1<<0);
        //Wait To Send Int Status being Set
        while(!(reg->SR & (1<<10)));
        //Must Clear It For Continue To Send
        reg->SR = 1<<10;
        //Clear Start Condition Flag
        reg->SR = 1<<8;


        //2��First to send internal dev address

        while(i < addr_len)
        {
            u8 interPartDevAddress = (internalDevAddress >> ((addr_len - 1 -i)*8)) & 0xff;
            *((volatile u8 *)&reg->BUFR) = interPartDevAddress;
            i++;
            reg->CMDR = (1<<0);
            while(!(reg->SR & (1<<10)));
            reg->SR = 1<<10;
            reg->SR = 1<<8;
        }

        //clear var i cnt for next time
        i = 0;

        //Here to send Data and generate stop signal
        reg->BUFR = buf[datLoop++];
        while(!(reg->SR & (1<<10)));
        reg->SR = 1<<10;
        reg->CMDR = (0<<0);
        while(!(reg->SR & (1<<9)));//Wait stop signal
        reg->SR = 1<<9;
        //add address
        internalDevAddress++;
        datToSend++;
//        while(!(reg->SR & (1<<9)));
//      delay(10);
//        delay_ms(3);
        DJY_DelayUs(3000);
    }
    return true;
}


// =============================================================================
// ���ܣ�IIC��
// ������ reg,�Ĵ�����ַ
//        devaddr ������ַ
//         buf�ջ�����
//       len ����
// ���أ�true/false
// =============================================================================

static bool_t __IIC_Read(tagI2CReg *reg,u8 devaddr, u32 memaddr, u32 addr_len,u8 *buf, u32 len)
{
    u32 internalDevAddress = 0;
    u32 datToRev = 0;
    u32 i = 0;
    u8 addressLoop;

    internalDevAddress =  memaddr;

    while(datToRev < len)
    {
        //1��Write Dev Address
        reg->BUFR = (devaddr & 0xfe);           //Write Into Device Address
        //Set Cpu Master Mode and Get Start Singnal To Send
        reg->CMDR = (1<<0);
        //Wait To Send Int Status being Set
        while(!(reg->SR & (1<<10)));
        //Must Clear It For Continue To Send
        reg->SR = 1<<10;
        //Clear Start Condition Flag
        reg->SR = 1<<8;


        //2��First to send internal dev address
        while(i < addr_len)
        {
            u8 interPartDevAddress = (internalDevAddress >> ((addr_len - 1 -i)*8)) & 0xff;
            *((volatile u8 *)&reg->BUFR) = interPartDevAddress;
            i++;
            reg->CMDR = (1<<0);
            while(!(reg->SR & (1<<10)));
            reg->SR = 1<<10;
            reg->SR = 1<<8;
        }

        //clear var i cnt for next time
        i = 0;

        reg->CMDR = 0x3;
        while(!(reg->SR & (1<<8) ));
        reg->SR = 0x100;
        devaddr &= ~0x01;
        devaddr |= 0x1;
        reg->BUFR = devaddr;
        while(!(reg->SR & (1<<10)));
        reg->SR |= 1<<10;

        //Waite to Rev Int
        while(!(reg->SR & (1<<10)));
        reg->SR |= ( 1<<10);
        buf[datToRev] = reg->BUFR;     //Get One Byte Data
        reg->CMDR = (1<<2) ;          //NACK+STOP
        while(!(reg->SR & (1<<9)));
        reg->SR |= (1<<9);

        datToRev++;
        internalDevAddress++;
    }

    return true;
}


// =============================================================================
// ���ܣ���ѯ��ʽ��IIC���豸������
// ������reg,�Ĵ�����ַ
//     devaddr,���������ַ�����һbit���ֶ�д
//     memaddr//�Ĵ�����ַ
//     maddrlen�ļĴ�����ַ�ĳ���
//       addr,���ʵļĴ�����ַ
//       buf, �洢������
//       len, �洢�������ĳ���
// ���أ�len,��ȡ�ɹ�;-1,��ȡʧ��
// =============================================================================

static s32 __IIC_ReadPoll(tagI2CReg *reg,u8 devaddr,u32 memaddr,
                        u8 maddrlen, u8 *buf, u32 len)
{


    if(false == __IIC_Read(reg, devaddr,memaddr, maddrlen,buf,len));
        return -1;

    return len;
}

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

static s32 __IIC_WritePoll(tagI2CReg *reg,u8 devaddr,u32 memaddr,
                        u8 maddrlen, u8 *buf, u32 len)
{

    if( __IIC_Write(reg, devaddr, memaddr, maddrlen,buf,len)==false)//д�洢����ַ
       return -1;

    return len;

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

static bool_t __IIC_WriteReadPoll(tagI2CReg *reg,u8 DevAddr,u32 MemAddr,\
                                u8 MemAddrLen,u8* Buf, u32 Length,u8 WrRdFlag)
{
    static bool_t flag;

    __IIC_IntDisable(reg);

    if(WrRdFlag == CN_IIC_WRITE_FLAG)   //д
    {
        if(Length == __IIC_WritePoll(reg,DevAddr,MemAddr,MemAddrLen,Buf,Length))
            flag = true;
        else
            flag = false;
    }
    else                                //��
    {
        if(Length == __IIC_ReadPoll(reg,DevAddr,MemAddr,MemAddrLen,Buf,Length))
            flag = true;
        else
            flag = false;
    }


    return  flag ;
}


// =============================================================================
// ����: ����дʱ������дʱ��Ĺ���Ϊ��������ַ��д�����洢��ַ��д�������洢��ַ��
//       ��ʱ������жϣ�֮�󽫻ᷢ�������жϣ����жϷ���
//       �����У�ÿ�η���һ�����ֽ�����ֱ��len��������post�ź���iic_bus_semp��������
//       ֹͣʱ��
// ����: SpecificFlag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//      DevAddr,������ַ��ǰ7���أ��ѽ��ڲ���ַ��ռ��bitλ���£��ú����轫�õ�ַ��
//               ��һλ�����������һλ��/д����;
//      MemAddr,�洢���ڲ���ַ�������͵������ϵĵ�ַ���õ�ַδ��������������ַ�ϵ�
//               ����λ��
//      MenAddrLen,�洢���ڲ���ַ�ĳ��ȣ��ֽڵ�λ��δ������������ַ����ı���λ��
//      Length,���յ������������������ݵĵ�����һ�ֽڣ���count-1��ֹͣ����ACK�źţ�
//          �����յ��ֽ���Ϊcountʱ������ֹͣʱ�򣬲��ͷ��ź���iic_semp;
//      IIC_BusSemp,�������ʱ�������ͷŵ��ź��������ͳ����IIC_PortReadʱ������������
//          �����ͷŸ��ź�������������ʱ��
// ����: TRUE��������ʱ��ɹ���FALSEʧ��
// =============================================================================

static bool_t __IIC_GenerateWriteStart(tagI2CReg *reg,
                                       u8 dev_addr,
                                       u32 mem_addr,
                                       u8 maddr_len,
                                       u32 length,
                                       struct SemaphoreLCB *iic_semp)
{
    u8 mem_addr_buf[4];
    u8 iic_port;
    //ͨ��specific_flag��������������
    if(reg==sgpt_I2cReg[CN_I2C1])
        iic_port=CN_I2C1;
    else if (reg==sgpt_I2cReg[CN_I2C2])
        iic_port=CN_I2C2;
    else if (reg==sgpt_I2cReg[CN_I2C3])
        iic_port=CN_I2C3;
    else
        return false;

    IntParamset[iic_port].TransTotalLen = length;
    IntParamset[iic_port].TransCount = 0;
    IntParamset[iic_port].pDrvPostSemp = iic_semp;           //iic_bus_semp


    __IIC_IntDisable(reg);                 //���ж�
    return true;
}


// =============================================================================
// ����: ������ʱ��������ʱ��Ĺ���Ϊ��������ַ��д�����洢��ַ��д����������ַ������
//       ��������ַ���������ʱ������жϣ��������üĴ���Ϊ����ģʽ��֮�󽫻ᷢ��
//       ���������жϣ����ж��н����յ������ݵ���IIC_PortWriteд�뻺�壬���յ�len��
//       ���������ݺ��ͷ��ź���iic_semp
// ����: SpecificFlag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//      DevAddr,������ַ��ǰ7���أ��ѽ��ڲ���ַ��ռ��bitλ���£��ú����轫�õ�ַ��
//               ��һλ�����������һλ��/д����;
//     MemAddr,�洢���ڲ���ַ�������͵������ϵĵ�ַ���õ�ַδ��������������ַ�ϵ�
//               ����λ��
//      MenAddrLen,�洢���ڲ���ַ�ĳ��ȣ��ֽڵ�λ��δ������������ַ����ı���λ��
//      Length,���յ������������������ݵĵ�����һ�ֽڣ���count-1��ֹͣ����ACK�źţ�����
//             �յ��ֽ���Ϊcountʱ������ֹͣʱ�򣬲��ͷ��ź���iic_semp;
//     IIC_BusSemp,�����ʱ���������ͷŵ��ź������������ź�����
// ����: TRUE��������ʱ��ɹ���FALSEʧ��
// =============================================================================

static bool_t __IIC_GenerateReadStart( tagI2CReg *reg,
                                       u8 dev_addr,
                                       u32 mem_addr,
                                       u8 maddr_len,
                                       u32 length,
                                       struct SemaphoreLCB *iic_semp)
{
    u8 iic_port;
    if(reg==sgpt_I2cReg[CN_I2C1])
      iic_port=CN_I2C1;
    else if(reg==sgpt_I2cReg[CN_I2C2])
      iic_port=CN_I2C2;
    else if (reg==sgpt_I2cReg[CN_I2C3])
      iic_port=CN_I2C3;
    else
      return false;

  IntParamset[iic_port].TransTotalLen = length;
  IntParamset[iic_port].TransCount = 0;
  IntParamset[iic_port].pDrvPostSemp = iic_semp;               //iic_buf_semp
  __IIC_IntDisable(reg);//���ж�

  return true;
}


// =============================================================================
// ����: �������ζ�д�ص����������ֶ�д�Ĳ�ֹͬͣʱ�򣬵����ڷ���ʱ����ֱ��ֹͣʱ��
//      ��Ϊ��������ֹͣ�ظ�ACK����ֹͣ
// ����: reg,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
// ����: ��
// =============================================================================

static void __IIC_GenerateEnd(tagI2CReg *reg)
{

}


//==============================================================================
// ���ܣ�IIC���߿��ƻص����������ϲ���ã�Ŀǰֻʵ�ֶ�IICʱ��Ƶ������
// ������TIMINGR-->λ 31:28 PRESCʱ��Ԥ��Ƶ����
//                   λ 23:20 SCLDEL[3:0]�����ݽ���ʱ��
//                   λ 19:16 SDADEL[3:0]�����ݱ���ʱ��
//                   λ 15:8 SCLH[7:0]��SCL �ߵ�ƽ����
//                   λ 7:0 SCLL[7:0]��SCL �͵�ƽ����
//   reg    ��ģ���ڼ�IIC�Ĵ�����ַ
// ���أ���
//==============================================================================

static void __IIC_ClkSet(tagI2CReg *reg,u32 data)
{

}


// =============================================================================
// ���ܣ�IIC���߿��ƻص����������ϲ���ã�Ŀǰֻʵ�ֶ�IICʱ��Ƶ������
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
               __IIC_ClkSet(reg,data1);
                 break;
           case CN_IIC_DMA_USED://ʹ��dma����

               break;
           case CN_IIC_DMA_UNUSED://��ֹdma����

               break;
           case CN_IIC_SET_POLL:           //ʹ����ѯ��ʽ���ͽ���
               __IIC_IntDisable(reg);
               break;
           default:
               return 0;
       }
       return 1;
}


// =============================================================================
// ���ܣ�        IIC�����뷢���жϷ��������ú���ʵ�ֵĹ������£�
//       1.ÿ���������һ���������ֽڷ���һ���жϣ�
//       2.���ж���ж�ʹ��ͬһ���жϺţ�������ݾ����������ʹ�õ����ĸ��жϣ�
//       3.���жϱ�־�����ж�ACK�źţ�ÿ��д�ֽڣ�������������Ӧ�޸ģ�
//       4.���մﵽ������һ���ֽ�ʱ�������ò�����ACK�źţ�
//       5.���ջ������ʱ����post�ź���IntParam->pDrvPostSemp��
//       6.���ջ������ʱ�������ֹͣʱ��
// ������i2c_int_line,�жϺţ�������û�õ�
// ���أ�true falst
// =============================================================================

static u32 __IIC_ISR(ufast_t i2c_int_line)
{


    return true;
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

bool_t ModuleInstall_I2C(u8 iic_port)
{

    struct IIC_Param IIC_Config[CN_IIC_NUM];

    IIC_Config[iic_port].BusName             = (char *)IIC_Name[iic_port];
    IIC_Config[iic_port].IICBuf              = (u8*)s_IICBuf[iic_port];
    IIC_Config[iic_port].IICBufLen           = IIC_BUF_LEN[iic_port];
    IIC_Config[iic_port].SpecificFlag        = (ptu32_t)sgpt_I2cReg[iic_port];
    IIC_Config[iic_port].pGenerateWriteStart = (WriteStartFunc)__IIC_GenerateWriteStart;
    IIC_Config[iic_port].pGenerateReadStart  = (ReadStartFunc)__IIC_GenerateReadStart;
    IIC_Config[iic_port].pGenerateEnd        = (GenerateEndFunc)__IIC_GenerateEnd;
    IIC_Config[iic_port].pBusCtrl            = (IICBusCtrlFunc)__IIC_BusCtrl;
    IIC_Config[iic_port].pWriteReadPoll      = (WriteReadPoll)__IIC_WriteReadPoll;

    __IIC_GpioConfig(iic_port);
    __IIC_IntConfig(iic_port,__IIC_ISR);

    sgpt_IIC_CB[iic_port] = IIC_BusAdd(&IIC_Config);
    if(sgpt_IIC_CB[iic_port] == NULL)
    {
        return 0;
    }

    return 1;
}


