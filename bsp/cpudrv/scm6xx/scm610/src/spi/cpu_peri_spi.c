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
// �����Ȩ�����ʹ���߷������¶������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�����������У���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�����������У��Լ�����
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

// �ļ���     ��cpu_peri_spi.c
// ģ������: SPIģ��ײ�Ӳ������ģ��
// ģ��汾: V1.00
// ������Ա: huzb
// ����ʱ��: 2/14/2018
// =============================================================================


#include "string.h"
#include "djyos.h"
#include "cpu_peri.h"
#include "int.h"
#include "spibus.h"
#include "math.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_SPI(u8 port);
//    #if CFG_SPI1_ENABLE==1
//    ModuleInstall_SPI(CN_SPI1);
//    #endif
//
//    #if CFG_SPI2_ENABLE==1
//    ModuleInstall_SPI(CN_SPI2);
//    #endif
//
//    #if CFG_SPI3_ENABLE==1
//    ModuleInstall_SPI(CN_SPI3);
//    #endif
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip spi"//SPI��������
//parent:"spi bus"   //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"spi bus","int","lock"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_SPI == false )
//#warning  " cpu_onchip_spi  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_SPI    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,32,512,
#define CFG_SPI_BUF_LEN             128                //"SPI��������С",
#define CFG_SPI_DMA_BUF_LEN         128                //"SPI DMA��������С",
//%$#@enum,true,false,
#define CFG_SPI1_ENABLE             false              //"�Ƿ�����ʹ��SPI1",
#define CFG_SPI2_ENABLE             false              //"�Ƿ�����ʹ��SPI2",
#define CFG_SPI3_ENABLE             false              //"�Ƿ�����ʹ��SPI3",
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

//������ֻ����Cpu �����������

typedef struct
{
    volatile u32 SPCR0;
    volatile u32 SPCR1;
    volatile u32 SPCR2;
    volatile u32 SPCR3;
    volatile u32 SPSR;
    volatile u32 SPWAR;
    volatile u32 SPDR;
    volatile u32 SPNUMR;
    volatile u32 SPINTSR;
    volatile u32 SPI_SCKDIV;
}tagSpiReg;

#define CN_SPI1_BASE     (0x40600000UL)
#define CN_SPI2_BASE     (0x40610000UL)
#define CN_SPI3_BASE     (0x41620000UL)
#define CN_SPI4_BASE     (0x41630000UL)

static volatile tagSpiReg *sgpt_SpiReg[] = {

        (volatile tagSpiReg *)CN_SPI1_BASE,
        (volatile tagSpiReg *)CN_SPI2_BASE,
        (volatile tagSpiReg *)CN_SPI3_BASE,
        (volatile tagSpiReg *)CN_SPI4_BASE,
};

#define CN_TIMEOUT  (20*1000)

//�����ж�����ʹ�õľ�̬���ṹ��
struct SPI_IntParamSet
{
    u32 SendDataLen;
    u32 RecvDataLen;
    u32 RecvOffset;
};


// =============================================================================
//����SPI���ƿ���жϾ�̬��
#define CN_SPI_BUF_LEN              128
#define CN_SPI_DMA_BUF_LEN          128
static struct SPI_CB *sgpt_Spi1_CB;
static u8 s_Spi1_Buf[CN_SPI_BUF_LEN];
struct SPI_IntParamSet IntParamset1;

static struct SPI_CB *sgpt_Spi2_CB;
static u8 s_Spi2_Buf[CN_SPI_BUF_LEN];
struct SPI_IntParamSet IntParamset2;

static struct SPI_CB *sgpt_Spi3_CB;
static u8 s_Spi3_Buf[CN_SPI_BUF_LEN];
struct SPI_IntParamSet IntParamset3;

static struct SPI_CB *sgpt_Spi4_CB;
static u8 s_Spi4_Buf[CN_SPI_BUF_LEN];
struct SPI_IntParamSet IntParamset4;


// =============================================================================
// ����: �ж�ʹ�ܺ�ʧ�ܺ���
// ����: tpSPI,�������ļĴ�����ָ��
// ����: ��
// =============================================================================
static void __SPI_IntEnable(volatile tagSpiReg *Reg,u32 IntSrc)
{
    Reg->SPCR0 |= IntSrc;
}
static void __SPI_IntDisable(volatile tagSpiReg *Reg,u32 IntSrc)
{
    Reg->SPCR0 &= ~IntSrc;
}

// =============================================================================
// ����: ����APB2��ʱ�Ӻ�Fre����BR�Ĵ�����д��ֵ
// ����: Fre,��Ҫ���õ�SPIʱ��
// ����: BRֵ
// =============================================================================

#define CN_SYS_SPI_FRQ (37500000)

static u8 __SPI_BrCal(u32 Fre)
{
    u8 tempDiv;
    u8 loop;

    tempDiv = CN_SYS_SPI_FRQ/Fre;

    //Spi�ķ�Ƶϵ��-ֻ����ż��
    //�жϷ�Ƶϵ���Ƿ�Ϸ�
    loop = 1;
    while(loop <= 254/2)
    {
        if(loop*2 == tempDiv)
        {
            break;
        }
        loop++;
    }


    return tempDiv;
}

// =============================================================================
// ���ܣ�SPIʱ�����ú�����ʱ����ԴΪ50M����SCR��CPSR��Ƶ�õ�ʱ�ӣ�ʱ�Ӽ��㹫ʽΪ��
//       SCK =  PCLK / (CPSDVSR *[SCR+1])
// ������spi_dev���豸���
//      spiclk�������õ�ʱ���ٶȣ���λΪHz
// ���أ�true=�ɹ���false=ʧ��
// ˵�����������̶�SCR = 1;�������ֲᣬCPSDVSR����Ϊ2-254��ż������ˣ�Ƶ�ʷ�ΧΪ
//       12.5M ~ 100kHz
// =============================================================================
static void __SPI_SetClk(volatile tagSpiReg *Reg,u32 Fre)
{
    u8 div;
    div = (u8)__SPI_BrCal(Fre);
    Reg->SPI_SCKDIV = 0;
    Reg->SPI_SCKDIV = div;
}

// =============================================================================
// ���ܣ�SPI���ƼĴ����������ã���PHA��CPOL��ʱ�ӵȣ����ݸ��ּĴ�������
// ������tpSPI,SPI��������ַ
//       ptr,����ָ��
// ���أ���
// =============================================================================

#define CN_MODE0 (0)
#define CN_MODE1 (1)
#define CN_MODE2 (2)
#define CN_MODE3 (3)
static bool_t __SPI_BusCsActive(tagSpiReg *Reg, u8 cs);
static void __SPI_Config(volatile tagSpiReg *Reg,tagSpiConfig *ptr)
{
     bool_t Spi_CHPA = false;
     bool_t Spi_CPOL = false;
     u32 SpiPin;

     SpiPin = (u32)Reg;

    __SPI_SetClk(Reg,ptr->Freq);

    //����ģʽ ����ʱ�ߵ͵�ƽ �ĸ������շ�����
    //Mode 0 CPOL = 0 CPHA =0
    //Mode 1 CPOL = 0 CPHA =1
    //Mode 2 CPOL = 1 CPHA =0
    //Mode 2 CPOL = 1 CPHA =1
    //��ʹ�õĹ����п��ܱ��������������ܣ�����������³�ʼ��
    switch(SpiPin)
    {
        case 0x41630000:          //SPI3
            SPI3_InitPad(DISABLE);
            break;
        case 0x41620000:          //SPI2
            SPI2_InitPad();
            break;
        case 0x41610000:          //SPI1
            SPI1_InitPad();
            break;
        case 0x41600000:          //SPI0
            SPI0_InitPad();
            break;
        default:;break;
    }

    //set the PHA
    //�ж�����ģʽ

    switch(ptr->Mode)
    {
        case CN_MODE0:                  //Model 0 00
             Reg->SPCR1 &= ~(0x3<<1);
             Reg->SPCR1 |= (0x0<<1);
             break;
        case CN_MODE1:                  //Model 1 01
             Reg->SPCR1 &= ~(0x3<<1);
             Reg->SPCR1 |= (0x1<<1);
             break;
        case CN_MODE2:                  //Model 2 10
             Reg->SPCR1 &= ~(0x3<<1);
             Reg->SPCR1 |= (0x2<<1);
             break;
        case CN_MODE3:                  //Model 3 11
             Reg->SPCR1 &= ~(0x3<<1);
             Reg->SPCR1 |= (0x3<<1);
             break;
        default:;break;
    }


//    Reg->SPCR3 &= ~(1<<0);
    //Spi ʹ�� �����Զ�����ģʽ
//    Reg->SPCR3 |= (1<<1) | (1<<4);

}

// =============================================================================
// ����: SPIĬ��Ӳ����ʼ�����ã���Ҫ��ʱ�����ú�GPIOд������������
// ����: RegBaseAddr,�Ĵ�����ַ
// ����: ��
// =============================================================================
static void __SPI_HardConfig(u32 BaseAddr)
{
    tagSpiReg *Reg;
    u32 temp = 0;

    Reg = (tagSpiReg *)BaseAddr;
    //����SPIʹ��GPIO����
    //��λ�Ĵ���
    SPI_Reset_Reg(Reg);

    //����Ϊ��ģʽ
    Reg->SPCR1 |= (1<<0);
    //Ĭ��ģʽ����1
    Reg->SPCR1 |= (1<<1);

    //�շ�ʹ��
    Reg->SPCR1 |= (1<<6)|(1<<7);

    //����ֵ����
    Reg->SPCR2 |= 8<<4;
    Reg->SPCR2 |= 0<<0;

    //Ĭ������10��Ƶ
    Reg->SPI_SCKDIV = 10;

    //��ģʽ�����ݼ���ʧ��
    Reg->SPCR3 &= ~(1<<5);

    //��ģʽSSNʹ��
    Reg->SPCR3 |= (1<<0);

    //�������������޼��
    Reg->SPWAR = 0;

    //����SPI
    Reg->SPCR3 |= (1<<4);

}

// =============================================================================
// ���ܣ�SPIƬѡʹ�ܣ�ʹƬѡ��Ч,���������о��������Ƭѡ�Ĵ�������ֱ�ӷ���
// ������Reg����ģ���ڼ�SPI�Ĵ�����ַ
//       cs,Ƭѡ��
// ���أ���
// ˵����SPI��������ֻ������һ��CS������Ҫ�Ӷ��CS�������Ӧ�ó����Լ�����CS
// =============================================================================
extern bool_t Board_SpiCsCtrl(u8 SPIPort,u8 cs,u8 level);
static bool_t __SPI_BusCsActive(tagSpiReg *Reg, u8 cs)
{
    u8 SPIPort;
    for(SPIPort = 0; SPIPort < CN_SPI_NUM; SPIPort++)
    {
        if(Reg ==  sgpt_SpiReg[SPIPort])
            break;
    }

    //����ʹ��spi��SSN�źſ��ƣ�����������IO����,������ģʽ��Ƭѡ�ź�
    //return Board_SpiCsCtrl(SPIPort,cs,0);
    Reg->SPCR3 &= ~(1<<0);

    Reg->SPCR3 |= (1<<1) | (1<<4);

    return true;
}

// =============================================================================
// ���ܣ�SPIƬѡʧ�ܣ�ʹƬѡ��Ч
// ������Reg����ģ���ڼ�SPI�Ĵ�����ַ
//       cs,Ƭѡ��
// ���أ���
// =============================================================================
static bool_t __SPI_BusCsInActive(tagSpiReg *Reg, u8 cs)
{
    u8 SPIPort;
    for(SPIPort = 0; SPIPort < CN_SPI_NUM; SPIPort++)
    {
        if(Reg ==  sgpt_SpiReg[SPIPort])
            break;
    }
    //ʹ��Ƭѡ
    Reg->SPCR3 |= (1<<0);
    //ͬʱʧ����������
    Reg->SPCR3 &= ~((1<<1) | (1<<4));

    return true;
}

// =============================================================================
// ���ܣ�SPI���߿��ƻص����������ϲ���ã�Ŀǰֻʵ�ֶ�SPIʱ��Ƶ������
// ������specific_flag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//       cmd,����
//       data1,data2,���ݣ�������������
// ���أ���
// =============================================================================
static s32 __SPI_BusCtrl(tagSpiReg *Reg,u32 cmd,ptu32_t data1,ptu32_t data2)
{
    s32 result = 1;
    if( (Reg != sgpt_SpiReg[CN_SPI1]) && (Reg != sgpt_SpiReg[CN_SPI2])
            && (Reg != sgpt_SpiReg[CN_SPI3]) &&  (Reg != sgpt_SpiReg[CN_SPI4]))
        return 0;

    switch(cmd)
    {
    case CN_SPI_SET_CLK:
        __SPI_SetClk(Reg,data1);
        break;
    case CN_SPI_CS_CONFIG:
        __SPI_Config(Reg,(tagSpiConfig *)data1);
        break;
    case CN_SPI_SET_AUTO_CS_EN:
        // Ӳ����������Զ�����CS������
        break;
    case CN_SPI_SET_AUTO_CS_DIS:
        break;
    default:
        break;
    }
    return result;
}

// =============================================================================
// ���ܣ���SPI���ռĴ�������ѯ��ʽ,��__SPI_TxRxPoll����
// ������Reg,�Ĵ�����ַ
// ���أ�����������
// =============================================================================
static s32 __SPI_Read( tagSpiReg * Reg )
{
    u32 time = CN_TIMEOUT;

//    Reg->SPDR = 0xff;

    //�жϽ��ջ����Ƿ������ݿɶ�
    while (((Reg->SPSR & 0xF) == 0 ) && (time > 0))
    {
        time --;
        DJY_DelayUs(1);
    }

    if(time == 0)
    {
        printk("Rev TimeOut\r\n");
        return -1;
    }

    return Reg->SPDR;
}

// =============================================================================
// ���ܣ�дSPI���ͼĴ�������ѯ��ʽ,��__SPI_TxRxPoll����
// ������Reg,�Ĵ�����ַ��wData��д����
// ���أ���
// =============================================================================
static s32 __SPI_Write( tagSpiReg * Reg,u8 wData )
{
    u32 time = CN_TIMEOUT;

    /* Send data */

    //Ĭ�ϳ�ʼ���з���FIFO����ֵ����Ϊ8��,��ʱ��Fifo����8��ʱ��û�з���ȥ��ô,fifo�����ٽ�ȥд��

    while ((((Reg->SPSR & 0xf0)>>4) == 8 ) && (time > 0) )
    {
        time --;
        DJY_DelayUs(1);
    }

    if(time == 0)
        return -1;

    Reg->SPDR = wData;

    return 0;
}

// =============================================================================
// ���ܣ���ѯ��ʽ��д����
// ������Reg,�Ĵ�����ַ��srcAddr,����Դָ�룻wrSize��д��������destAddr������������
//       ��ַָ�룻rdSize������������recvoff,��ƫ�ƣ���δ��
// ���أ�true,��ȷ��false,����
// =============================================================================

static bool_t __SPI_TxRxPoll(tagSpiReg *Reg,u8 *srcAddr,u32 wrSize,
        u8 *destAddr, u32 rdSize,u32 recvoff,u8 cs)
{
    u32 i,len_limit;

    if( (!srcAddr) || ((rdSize != 0) && (!destAddr)))
        return false;

    Reg->SPCR3 |= (1<<4);

    len_limit = MAX(wrSize, rdSize + recvoff);

    //spi ͨ��һ��һ��
    for (i=0;i<len_limit;i++)
    {
        if (i<wrSize)
        {
            __SPI_Write(Reg,srcAddr[i]);
        }
        else if((i>=wrSize)&&(i<len_limit))
        {
          __SPI_Write(Reg,0);         //������Ͻ�������,�������������ݲ���ʱ�ӽ���
        }

        if((destAddr) && (i>=recvoff) )
        {
            destAddr[i-recvoff] = __SPI_Read(Reg);//Read Data We Need
        }
        else
        {
            __SPI_Read(Reg); //��д�����У�ͬʱ���յ����� ����Ҫ���� ��Ϸ���
        }
    }

    return true;
}

// =============================================================================
// ���ܣ�SPI�ײ��������ͽ��պ�������������ɵĹ������£�
//       1.����ṹ��̬�����������ж���ʹ�ã�
//       2.������صļĴ�����ʹ�䴦�ڷ��ͽ�������״̬;
//       3.�����жϣ����жϷ���������������շ���
// ������specific_flag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//       sendlen,�������ݳ��ȣ��ֽ�
//       recvlen,�������ݳ��ȣ��ֽ�
//       recvoff,����������ϡ�����ӽ��յ��ĵڼ����ֽڿ�ʼ�������ݣ��ֽ�
//       block_opt,����ѡ�trueʱ��Ϊ��������������ʽ��ײ������ͷ��ź���������CS
//       spi_semp,�ײ������Ҫ�ͷŵ��ź������Ƿ��ͷ���block_opt����
// ���أ�true,�޴���;false,ʧ��
// =============================================================================
static bool_t __SPI_TransferTxRx(tagSpiReg *Reg,u32 sendlen,u32 recvlen,
                                u32 recvoff)
{
    struct SPI_IntParamSet *Param=NULL;

    if(Reg == sgpt_SpiReg[CN_SPI1])
        Param = &IntParamset1;
    else if(Reg == sgpt_SpiReg[CN_SPI2])
        Param = &IntParamset2;
    else if(Reg == sgpt_SpiReg[CN_SPI3])
        Param = &IntParamset3;
    else
        Param = &IntParamset4;


    Param->SendDataLen = sendlen;
    Param->RecvDataLen = recvlen;
    Param->RecvOffset  = recvoff;

    //Spi ʹ��
    Reg->SPCR3 |= (1<<4);

    //���Ϳ��ж�ʹ��
    __SPI_IntEnable(Reg,(1<<5));


    return true;
}
// =============================================================================
// ���ܣ�SPI�����뷢���жϷ��������ú���ʵ�ֵĹ������£�
//       1.�����շ�һ�壬��˷��Ϳ��ж���Ҳ�յ������ݣ�
//       2.SPI_PortRead()�ж���������ʱ��˵�����͵������Ѿ���ɣ��ɹط����ж�
//       3.��param->RecvDataLen����0������Ҫ�������ݣ���ӽ��յ���������offset�ֽ�
//         ��ʼ�洢���ݣ�������SPI_PortRead()
//       4.
// ������spi_int_line,�жϺţ�������û�õ�
// ���أ�������
// =============================================================================
u32 SPI_ISR(ptu32_t IntLine)
{
    struct SPI_CB *pSCB=NULL;
    struct SPI_IntParamSet *param;
    tagSpiReg *Reg;
    u32 spi_sr,spi_cr2;
    u8 ch,temp;
    static u8 firstrcv = 0;

    if(IntLine == CN_INT_LINE_SPI0)
    {
        pSCB = sgpt_Spi1_CB;
        Reg = (tagSpiReg *)sgpt_SpiReg[CN_SPI1];
        param = &IntParamset1;
    }
    else if(IntLine == CN_INT_LINE_SPI1)
    {
        pSCB = sgpt_Spi2_CB;
        Reg = (tagSpiReg *)sgpt_SpiReg[CN_SPI2];
        param = &IntParamset2;
    }
    else if(IntLine == CN_INT_LINE_SPI2)
    {
        pSCB = sgpt_Spi3_CB;
        Reg = (tagSpiReg *)sgpt_SpiReg[CN_SPI3];
        param = &IntParamset3;
    }
    else if(IntLine == CN_INT_LINE_SPI3)
    {
        pSCB = sgpt_Spi4_CB;
        Reg = (tagSpiReg *)sgpt_SpiReg[CN_SPI4];
        param = &IntParamset4;
    }
    else
        return 0;

    //���Ϳ��ж�
    if((Reg->SPCR0 & (1<<5)) && (Reg->SPINTSR & (1<<5)))
    {
        temp = SPI_PortRead(pSCB,&ch,1);//�Ƿ���������Ҫ����

        if(temp >0)
        {
            param->SendDataLen --;
            param->RecvOffset  --;
            Reg->SPDR = ch;
        }
        else                                            //�������ͽ���
        {
            //�ط����ж�
            __SPI_IntDisable(Reg,(1<<5));
            param->RecvOffset -= param->SendDataLen;
            param->SendDataLen = 0;

            ch = Reg->SPDR;

            //��������ˣ��ж��Ƿ���Ҫ����
            if(param->RecvDataLen > 0)
            {
                //MASTERģʽ�£����ͺͽ���ͬʱ����
                Reg->SPDR = 0xAA;
                //ʹ�ܽ��շǿ��ж�
                __SPI_IntEnable(Reg,(1<<2));    //ʹ�ܽ����ж�
                firstrcv = 1;
            }
        }

    }

    //���շǿ��ж�
    else if((Reg->SPCR0 & (1<<2)) && (Reg->SPINTSR  & (1<<2)))    //�����ж�
    {
        ch = Reg->SPDR;
        //��������˶�������
        if(firstrcv != 1)
        {
            SPI_PortWrite(pSCB,&ch,1);
            param->RecvDataLen -= 1;
        }
        else
            firstrcv = 0;

        //�ж��Ƿ����������
        if(param->RecvDataLen > 0)
        {
            Reg->SPDR = 0xAA;
        }
        else                    //���������Ѿ����
        {
            param->RecvDataLen = 0;
            __SPI_IntDisable(Reg,(1<<2));
        }
    }

    if(param->RecvDataLen + param->SendDataLen == 0)
    {
        //spi ʹ��
        Reg->SPCR0 &= (1<<4);
    }

    return 0;
}

// =============================================================================
// ����: SPI�ж����ú���
// ����: IntLine,�ж���
// ����: ��
// =============================================================================
static void __SPI_IntConfig(u8 IntLine)
{
    //�ж��ߵĳ�ʼ��
    Int_Register(IntLine);
    Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(IntLine,SPI_ISR);
    Int_SettoAsynSignal(IntLine);
    Int_ClearLine(IntLine);
    Int_RestoreAsynLine(IntLine);
}

// =============================================================================
// ���ܣ�SPI�ײ������ĳ�ʼ�����������SPI���ߵĳ�ʼ��������Ҫ�������£�
//       1.��ʼ�����߿��ƿ�SPI_CB���ص������ͻ������ĳ�ʼ����ֵ��
//       2.Ĭ�ϵ�Ӳ����ʼ������GPIO��SPI�Ĵ����ȣ�
//       3.�жϳ�ʼ������ɶ�д�ж����ã�
//       4.����SPIBusAdd��SPIBusAdd_r�������߽�㣻
// ��������
// ���أ���ʼ���ɹ�����1��ʧ�ܷ���0
// =============================================================================
bool_t SPI_Initialize(u8 port)
{
    struct SPI_Param SPI_Config;
    struct SPI_CB **pSpiCB = NULL;
    u8 IntLine;

    switch(port)
    {
    case CN_SPI1:
        SPI_Config.BusName          = "SPI1";
        SPI_Config.SPIBuf           = (u8*)&s_Spi1_Buf;
        IntLine = CN_INT_LINE_SPI0;
        pSpiCB = &sgpt_Spi1_CB;
        break;
    case CN_SPI2:
        SPI_Config.BusName          = "SPI2";
        SPI_Config.SPIBuf           = (u8*)&s_Spi2_Buf;
        IntLine = CN_INT_LINE_SPI1;
        pSpiCB = &sgpt_Spi2_CB;
        break;
    case CN_SPI3:
        SPI_Config.BusName          = "SPI3";
        SPI_Config.SPIBuf           = (u8*)&s_Spi3_Buf;
        IntLine = CN_INT_LINE_SPI2;
        pSpiCB = &sgpt_Spi3_CB;
        break;
    case CN_SPI4:
        SPI_Config.BusName          = "SPI4";
        SPI_Config.SPIBuf           = (u8*)&s_Spi4_Buf;
        IntLine = CN_INT_LINE_SPI3;
        pSpiCB = &sgpt_Spi4_CB;
        break;

    default:
        return (0);
    }
    SPI_Config.SpecificFlag     = (ptu32_t)sgpt_SpiReg[port];
    SPI_Config.SPIBufLen        = CN_SPI_BUF_LEN;
    SPI_Config.MultiCSRegFlag   = false;
    SPI_Config.pTransferPoll    = (TransferPoll)__SPI_TxRxPoll;
    SPI_Config.pTransferTxRx    = (TransferFunc)__SPI_TransferTxRx;
    SPI_Config.pCsActive        = (CsActiveFunc)__SPI_BusCsActive;
    SPI_Config.pCsInActive      = (CsInActiveFunc)__SPI_BusCsInActive;
    SPI_Config.pBusCtrl         = (SPIBusCtrlFunc)__SPI_BusCtrl;

    *pSpiCB = SPI_BusAdd(&SPI_Config);
    if(*pSpiCB == NULL)
    {
        return (0); // ʧ��
    }
    __SPI_HardConfig((u32)SPI_Config.SpecificFlag); // SPI�Ĵ�������

    __SPI_IntConfig(IntLine); // ע���ж�

    return (1); // �ɹ�
}
//-----------------------------------------------------------------------------
//����: ��װSPI�˿�
//����: SPI�˿ں�
//����: -1 -- ʧ��; 0 -- �ɹ�;
//��ע:
//-----------------------------------------------------------------------------
s32 ModuleInstall_SPIPort(u8 Port)
{
    if(CN_SPI_NUM < Port) // ���ֻ֧�ֵ�SPI1
        return (-1);
    if(0 == SPI_Initialize(Port))
        return (-1);
    return (0);
}




