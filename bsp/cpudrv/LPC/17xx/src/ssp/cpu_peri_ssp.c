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

// �ļ���     ��cpu_peri_ssp.c
// ģ������: SPIģ��ײ�Ӳ������ģ��,����LPC17xx������SSP��һ��SPI����ȻSSP�����ϼ�
//          ��SPI,���ǼĴ��������ݣ���ˣ���ģ����ݰ��ʹ��SSP
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 27/08.2014
// =============================================================================

#include "djyos.h"
#include "cpu_peri.h"
#include "spibus.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    #if CFG_SSP0_ENABLE==1
//    extern bool_t ModuleInstall_SSP0(void);
//    ModuleInstall_SPI(CN_SSP0);
//    #endif
//
//    #if CFG_SSP1_ENABLE==1
//    extern bool_t ModuleInstall_SSP1(void);
//    ModuleInstall_SPI(CN_SSP1);
//    #endif
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip spi"//CPU��spi��������
//parent:"spi bus"   //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"spi bus","int"//������������������������none����ʾ�������������
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
//%$#@num,16,512,
#define CFG_SSP0_BUF_LEN             64    //"SSP0��������С",
#define CFG_SSP1_BUF_LEN             64    //"SSP1��������С",
//%$#@enum,true,false,
#define CFG_SSP0_ENABLE             0      //"����SSP0ʹ��",
#define CFG_SSP1_ENABLE             0      //"����SSP1ʹ��",
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================
#define CN_SSP0_BASE        LPC_SSP0_BASE
#define CN_SSP1_BASE        LPC_SSP1_BASE
#define tagSpiReg           LPC_SSP_TypeDef
#define CN_FIFO_LEN         8

//�����ж�����ʹ�õľ�̬���ṹ��
struct SPI_IntParamSet
{
//  struct semaphore_LCB *pDrvPostSemp; //�ź���
    u32 SendDataLen;
    u32 RecvDataLen;
    u32 RecvOffset;
//  u8 CurrentCS;
//  u8 block_option;
};
// =============================================================================
//����SPI���ƿ���жϾ�̬��
//#define CFG_SSP0_BUF_LEN      32
struct SPI_IntParamSet IntParamset0;

//#define CFG_SSP1_BUF_LEN      32
struct SPI_IntParamSet IntParamset1;

static struct SPI_CB *sp_SSP_CB[CN_SSP_NUM];
// =============================================================================
// ����: �ж�ʹ�ܺ�ʧ�ܺ���
// ����: tpSPI,�������ļĴ�����ָ��
// ����: ��
// =============================================================================
static void __SSP_TxIntEnable(volatile tagSpiReg *Reg)
{
    Reg->IMSC |= (1<<3);
}
static void __SSP_TxIntDisable(volatile tagSpiReg *Reg)
{
    Reg->IMSC &= ~(1<<3);
}
static void __SSP_RxIntEnable(volatile tagSpiReg *Reg)
{
    Reg->IMSC |= (1<<1)|(1<<2);
}
static void __SSP_RxIntDisable(volatile tagSpiReg *Reg)
{
    Reg->IMSC &= ~((1<<1) | (1<<2));
}
// =============================================================================
// ���ܣ�SPIʱ�����ú�����ʱ����ԴΪ25M����SCR��CPSR��Ƶ�õ�ʱ�ӣ�ʱ�Ӽ��㹫ʽΪ��
//       SCK =  PCLK / (CPSDVSR *[SCR+1])
// ������spi_dev���豸���
//      spiclk�������õ�ʱ���ٶȣ���λΪHz
// ���أ�true=�ɹ���false=ʧ��
// ˵�����������̶�SCR = 1;�������ֲᣬCPSDVSR����Ϊ2-254��ż������ˣ�Ƶ�ʷ�ΧΪ
//       12.5M ~ 100kHz
// =============================================================================
static void __SSP_SetClk(volatile tagSpiReg *tpSPI,u32 spisck)
{
    u32 temp;
    if(tpSPI == NULL)
        return;
    temp = 25 * 1000000 / spisck;
    temp = temp - (temp%2);
    if(temp > 254)  temp = 254;
    if(temp < 2)    temp = 2;

    tpSPI->CPSR = temp;
}

// =============================================================================
// ���ܣ�SPI���ų�ʼ������ʼ��Ϊ����ʹ�ã�����ʱ�Ӻ�����
// ������spi_no,SPI��������
// ���أ���
// =============================================================================
static void __SSP_GpioInit(u32 spi_no)
{
    if(spi_no == CN_SSP0)
    {
        LPC_SC->PCONP |= (1<<21);

        // MISO,MOSI,SCK,SSEL����
        LPC_PINCON->PINSEL0  |= (2 << 30);  //sck0
        LPC_SC->PCLKSEL1 &= ~(3<<10);
        LPC_SC->PCLKSEL1 |= (00<<10);   //cclk/2 = 25M
        LPC_PINCON->PINSEL1  |= (2 << 0) | (2 << 2) | (2 << 4);
        LPC_PINCON->PINMODE1 &= ~0x000000FF;
    }
    else
    {
        // Enable clock for SSP1, clock = CCLK / 2
        LPC_SC->PCONP    |= (1<<10);
        LPC_SC->PCLKSEL0 &= ~(3<<20);
        LPC_SC->PCLKSEL0 |= (00<<20);   //cclk/4 = 25M
        /* Connect MOSI, MISO, and SCK to SSP peripheral*/
        LPC_PINCON->PINSEL0  |= (2 << 18) | (2 << 16) | (2 << 14);
        LPC_PINCON->PINMODE0 &= ~0x000FF000;
    }
}

// =============================================================================
// ���ܣ�SPI���ƼĴ����������ã���PHA��CPOL��ʱ�ӵȣ����ݸ��ּĴ�������
// ������tpSPI,SPI��������ַ
//       ptr,����ָ��
// ���أ���
// =============================================================================
static void __SSP_Config(volatile tagSpiReg *Reg,tagSpiConfig *ptr)
{
    __SSP_SetClk(Reg,ptr->Freq);

    //set the PHA
    if(ptr->Mode & SPI_CPHA)
    {
        Reg->CR0 |= 1<<7;
    }
    else
    {
        Reg->CR0 &= ~(1<<7);
    }
    //set the cpol
    if(ptr->Mode & SPI_CPOL)
    {
        Reg->CR0 |= 1<<6;
    }
    else
    {
        Reg->CR0 &= ~(1<<6);
    }

    if(ptr->CharLen <= 16 && ptr->CharLen >= 4)
    {
        Reg->CR0 &= ~(0xF);
        Reg->CR0 = ptr->CharLen - 1;
    }
}

// =============================================================================
// ����: SPIĬ��Ӳ����ʼ�����ã���Ҫ��ʱ�����ú�GPIOд������������
// ����: RegBaseAddr,�Ĵ�����ַ
// ����: ��
// =============================================================================
static void __SSP_HardDefaultSet(u8 port)
{
    tagSpiReg * Reg;
    if(port == CN_SSP0)
        Reg = (tagSpiReg *)CN_SSP0_BASE;
    else if(port == CN_SSP1)
        Reg = (tagSpiReg *)CN_SSP1_BASE;
    else
        return;

    //����SPIʹ��GPIO����
    __SSP_GpioInit(port);

    Reg->CR0        = 0x00C7;       //SCR = 0;CPOL=1, CPHA=1,8bits
    Reg->CPSR       = 0x02;         //CLK = 50M/(0x02 * (1+SCR)) = 25M
    Reg->CR1        = 0x02;         //ENABLE SSP
}

// =============================================================================
// ���ܣ�SPIƬѡʹ�ܣ�ʹƬѡ��Ч
// ������Reg����ģ���ڼ�SPI�Ĵ�����ַ
//       cs,Ƭѡ��
// ���أ���
// ˵����SSP��������ֻ������һ��CS������Ҫ�Ӷ��CS�������Ӧ�ó����Լ�����CS
// =============================================================================
static bool_t __SSP_BusCsActive(tagSpiReg *Reg, u8 cs)
{
    bool_t result = true;
    if((u32)Reg == CN_SSP0_BASE)
        LPC_GPIO0->FIOCLR |= (0x1<<16);
    else if((u32)Reg == CN_SSP1_BASE)
        LPC_GPIO0->FIOCLR |= (0x1<<6);
    else
        result = false;
    return result;
}

// =============================================================================
// ���ܣ�SPIƬѡʧ�ܣ�ʹƬѡ��Ч
// ������Reg����ģ���ڼ�SPI�Ĵ�����ַ
//       cs,Ƭѡ��
// ���أ���
// =============================================================================
static bool_t __SSP_BusCsInActive(tagSpiReg *Reg, u8 cs)
{
    bool_t result = true;
    if((u32)Reg == CN_SSP0_BASE)
        LPC_GPIO0->FIOSET |= (0x1<<16);
    else if((u32)Reg == CN_SSP1_BASE)
        LPC_GPIO0->FIOSET |= (0x1<<6);
    else
        result = false;
    return result;
}

// =============================================================================
// ���ܣ�SPI���߿��ƻص����������ϲ���ã�Ŀǰֻʵ�ֶ�SPIʱ��Ƶ������
// ������Reg,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//       cmd,����
//       data1,data2,���ݣ�������������
// ���أ���
// =============================================================================
static s32 __SSP_BusCtrl(tagSpiReg *Reg,u32 cmd,ptu32_t data1,ptu32_t data2)
{
    s32 result = 1;
    if(Reg == NULL)
        return 0;

    switch(cmd)
    {
    case CN_SPI_SET_CLK:
        __SSP_SetClk(Reg,data1);
        break;
    case CN_SPI_CS_CONFIG:
        __SSP_Config(Reg,(tagSpiConfig *)data1);
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
// ���ܣ���ѯ��ʽSSP��д���ݣ�����ƫ�ƴ˴����˼򻯴���ֱ����Ϊ��sendlen��ͬ��������
//       ����ٽ�������
// ������Reg,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//       srcaddr,��������Դ
//       sendlen,�������ݳ��ȣ��ֽڵ�λ
//       destaddr,�������ݻ�����
//       recvlen,�������ݳ��ȣ��ֽڵ�λ
//       recvoff,������ϡ�ֽڣ��ܶ�ʱ��Ϊ0
//       Byte,���͵��ֽ�
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
static bool_t __SSP_TxRxPoll(tagSpiReg *Reg,u8* srcaddr,u32 sendlen,
                            u8* destaddr,u32 recvlen,u32 recvoff,u8 cs)
{
    u32 i,j,len;
    u8 ch =0xaa;
    __SSP_TxIntDisable(Reg);
    __SSP_RxIntDisable(Reg);

    while(Reg->SR & (1<<2))     //SSP_SR_RNE
    {
        i = Reg->DR;            //����FIFO
    }

    //�˴��򻯴������ͺ��ٽ�������
    for(i = 0; i < sendlen/CN_FIFO_LEN; i++)            //��������
    {
        if(i == sendlen/CN_FIFO_LEN - 1)
        {
            len = sendlen%CN_FIFO_LEN;
        }
        else
        {
            len = CN_FIFO_LEN;
        }
        for(j = 0;j < len; j++)
        {
            Reg->DR = srcaddr[i*CN_FIFO_LEN + j];
        }
        while (!(Reg->SR & 0x04));
    }

    //�����Ҫ��������
    for(i = 0; i < recvlen/CN_FIFO_LEN; i++)            //��������
    {
        if(i == sendlen/CN_FIFO_LEN - 1)
        {
            len = sendlen % CN_FIFO_LEN;
        }
        else
        {
            len = CN_FIFO_LEN;
        }
        for(j = 0;j < len; j++)
        {
            Reg->DR = 0xFF;
        }
        while (!(Reg->SR & 0x04));
        for(j = 0; j < len; j ++)
        {
            destaddr[i*8+j]=Reg->DR;
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
// ���أ�true,�޴���;false,ʧ��
// =============================================================================
static bool_t __SSP_TransferTxRx(tagSpiReg *Reg,u32 sendlen,u32 recvlen,
                                u32 recvoff)
{
    struct SPI_IntParamSet *Param=NULL;

    if((u32)Reg == CN_SSP0_BASE)
        Param = &IntParamset0;

    if((u32)Reg == CN_SSP1_BASE)
        Param = &IntParamset1;

    __SSP_TxIntDisable(Reg);
    __SSP_RxIntDisable(Reg);

    while(Reg->SR & (1<<2))//RNE,��ս��ջ�����
        Reg->DR;

    Param->SendDataLen = sendlen;
    Param->RecvDataLen = recvlen;
    Param->RecvOffset  = recvoff;

    __SSP_RxIntEnable(Reg);
    __SSP_TxIntEnable(Reg);//ʹ�ܷ��Ϳ��жϣ��ж��н��շ�������

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
u32 SSP_ISR(ptu32_t IntLine)
{
    struct SPI_CB *SPI_SCB=NULL;
    struct SPI_IntParamSet *param;
    tagSpiReg *Reg;
    u8 ch[8],num=0,trans;

    if(IntLine == CN_INT_LINE_SSP0)
    {
        SPI_SCB = sp_SSP_CB[CN_SSP0];
        Reg = (tagSpiReg *)SPI_SCB->SpecificFlag;
        param = &IntParamset0;
    }

    if(IntLine == CN_INT_LINE_SSP1)
    {
        SPI_SCB = sp_SSP_CB[CN_SSP1];
        Reg = (tagSpiReg *)SPI_SCB->SpecificFlag;
        param = &IntParamset1;
    }

    if(SPI_SCB == NULL)
        return 0;

    //�����ж�
    if( (Reg->IMSC & (1<<3)) && (Reg->RIS & (1<<3)))    //ʹ���˷����жϣ���TXRIS
    {
        if(Reg->SR & (1<<0))        //TNF
        {
            trans = SPI_PortRead(SPI_SCB,ch,4);
            if(trans >0)
            {
                for(num = 0; num < trans; num++)
                {
                    Reg->DR = ch[num];
                }
                param->SendDataLen -= trans;
            }
            else
            {
                if(param->SendDataLen == 0)
                __SSP_TxIntDisable(Reg);
//              break;
            }
        }
    }


    //�����ж�
    if((Reg->IMSC & ((1<<2) | (1<<1)))  && (Reg->RIS & ((1<<2) | (1<<1))))
    {
        if(param->RecvDataLen > 0)
        {
            while(Reg->SR & (1<<2))//RNE
            {
                ch[0] = Reg->DR;
                if(param->RecvOffset > 0)
                    param->RecvOffset--;
                else
                {
                    if(param->RecvDataLen > 0)
                    {
                        SPI_PortWrite(SPI_SCB,ch,1);
                        param->RecvDataLen--;
                    }
                    else
                        break;
                }
            }
            if((param->SendDataLen == 0) && (param->RecvDataLen > 0))
            {
                //������ɣ�����Ҫ�������ݣ����Ҫ�������Ϸ�����������
                Reg->DR = 0xaa;
            }
        }
        else
        {
            __SSP_RxIntDisable(Reg);
        }
    }

    if(param->SendDataLen + param->RecvDataLen == 0)
    {
//      __SSP_TxIntDisable(Reg);
        __SSP_RxIntDisable(Reg);
    }


    return 0;
}

// =============================================================================
// ����: SPI�ж����ú���
// ����: IntLine,�ж���
// ����: ��
// =============================================================================
static void __SSP_IntConfig(u8 port)
{
    u8 IntLine;
    if(port == CN_SSP0)
        IntLine = CN_INT_LINE_SSP0;
    else if(port == CN_SSP1)
        IntLine = CN_INT_LINE_SSP1;
    else
        return;
    //�ж��ߵĳ�ʼ��
    Int_Register(IntLine);
    Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(IntLine,SSP_ISR);
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
bool_t ModuleInstall_SSP0(void)
{
    struct SPI_Param SSP_Config;
    static struct SPI_CB *s_ptSSP0_CB;
    static u8 s_SSP0_Buf[CFG_SSP0_BUF_LEN];

    SSP_Config.BusName          = "SSP0";
    SSP_Config.SPIBuf           = (u8*)&s_SSP0_Buf;
    SSP_Config.SPIBufLen        = CFG_SSP0_BUF_LEN;
    SSP_Config.SpecificFlag     = CN_SSP0_BASE;
    SSP_Config.MultiCSRegFlag   = false;
    SSP_Config.pTransferTxRx    = (TransferFunc)__SSP_TransferTxRx;
    SSP_Config.pCsActive        = (CsActiveFunc)__SSP_BusCsActive;
    SSP_Config.pCsInActive      = (CsInActiveFunc)__SSP_BusCsInActive;
    SSP_Config.pBusCtrl         = (SPIBusCtrlFunc)__SSP_BusCtrl;
    SSP_Config.pTransferPoll    = (TransferPoll)__SSP_TxRxPoll;
    sp_SSP_CB[CN_SSP0] = s_ptSSP0_CB;

    __SSP_HardDefaultSet(CN_SSP0);
    __SSP_IntConfig(CN_SSP0);

    if(s_ptSSP0_CB = SPI_BusAdd(&SSP_Config)) // ��SPI�˿���ӵ�SPI����
        return (1);
    else
        return (0);
}
bool_t ModuleInstall_SSP1(void)
{
    struct SPI_Param SSP_Config;
    static struct SPI_CB *s_ptSSP1_CB;
    static u8 s_SSP1_Buf[CFG_SSP1_BUF_LEN];

    SSP_Config.BusName          = "SSP1";
    SSP_Config.SPIBuf           = (u8*)&s_SSP1_Buf;
    SSP_Config.SPIBufLen        = CFG_SSP1_BUF_LEN;
    SSP_Config.SpecificFlag     = CN_SSP1_BASE;
    SSP_Config.MultiCSRegFlag   = false;
    SSP_Config.pTransferTxRx    = (TransferFunc)__SSP_TransferTxRx;
    SSP_Config.pCsActive        = (CsActiveFunc)__SSP_BusCsActive;
    SSP_Config.pCsInActive      = (CsInActiveFunc)__SSP_BusCsInActive;
    SSP_Config.pBusCtrl         = (SPIBusCtrlFunc)__SSP_BusCtrl;
    SSP_Config.pTransferPoll    = (TransferPoll)__SSP_TxRxPoll;
    sp_SSP_CB[CN_SSP1] = s_ptSSP1_CB;

    __SSP_HardDefaultSet(CN_SSP1);
    __SSP_IntConfig(CN_SSP1);

    if(s_ptSSP1_CB = SPI_BusAdd(&SSP_Config)) // ��SPI�˿���ӵ�SPI����
        return (1);
    else
        return (0);
}


