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
// ������Ա: HM
// ����ʱ��: 9/12.2014
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

#define CN_TIMEOUT  (20*1000)
#define tagSpiReg SPI_TypeDef
static tagSpiReg volatile * const tg_SpiReg[] = {(tagSpiReg *)SPI1_BASE,
                                                 (tagSpiReg *)SPI2_BASE,
                                                 (tagSpiReg *)SPI3_BASE};

//�����ж�����ʹ�õľ�̬���ṹ��
struct SPI_IntParamSet
{
    u32 SendDataLen;
    u32 RecvDataLen;
    u32 RecvOffset;
};
// =============================================================================
//����SPI���ƿ���жϾ�̬��
//#define CFG_SPI_BUF_LEN              128
//#define CFG_SPI_DMA_BUF_LEN          128
static struct SPI_CB *s_ptSpi1_CB;
static u8 s_Spi1_Buf[CFG_SPI_BUF_LEN];
struct SPI_IntParamSet IntParamset1;

static struct SPI_CB *s_ptSpi2_CB;
static u8 s_Spi2_Buf[CFG_SPI_BUF_LEN];
struct SPI_IntParamSet IntParamset2;

static struct SPI_CB *s_ptSpi3_CB;
static u8 s_Spi3_Buf[CFG_SPI_BUF_LEN];
struct SPI_IntParamSet IntParamset3;

// =============================================================================
// ����: �ж�ʹ�ܺ�ʧ�ܺ���
// ����: tpSPI,�������ļĴ�����ָ��
// ����: ��
// =============================================================================
static void __SPI_IntEnable(volatile tagSpiReg *Reg,u32 IntSrc)
{
    Reg->CR2 |= IntSrc;
}
static void __SPI_IntDisable(volatile tagSpiReg *Reg,u32 IntSrc)
{
    Reg->CR2 &= ~IntSrc;
}

// =============================================================================
// ����: ����APB2��ʱ�Ӻ�Fre����BR�Ĵ�����д��ֵ
// ����: Fre,��Ҫ���õ�SPIʱ��
// ����: BRֵ
// =============================================================================
static u32 __SPI_BrCal(u32 Fre)
{
    u32 temp;
    temp = CN_CFG_PCLK2/Fre;
    if(temp >= 256)         temp = 7;
    else if(temp >= 128)    temp = 6;
    else if(temp >= 64)     temp = 5;
    else if(temp >= 32)     temp = 4;
    else if(temp >= 16)     temp = 3;
    else if(temp >= 8)      temp = 2;
    else if(temp >= 4)      temp = 1;
    else                    temp = 0;

    return temp;
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
    u8 br;
    br = (u8)__SPI_BrCal(Fre);
    Reg->CR1 &= ~SPI_CR1_SPE;
    Reg->CR1 &= ~SPI_CR1_BR;
    Reg->CR1 |= br<<3;
}

// =============================================================================
// ���ܣ�SPI���ƼĴ����������ã���PHA��CPOL��ʱ�ӵȣ����ݸ��ּĴ�������
// ������tpSPI,SPI��������ַ
//       ptr,����ָ��
// ���أ���
// =============================================================================
static void __SPI_Config(volatile tagSpiReg *Reg,tagSpiConfig *ptr)
{
    __SPI_SetClk(Reg,ptr->Freq);

    //set the PHA
    if(ptr->Mode & SPI_CPHA)
    {
        Reg->CR1 |= SPI_CR1_CPHA;
    }
    else
    {
        Reg->CR1 &= ~SPI_CR1_CPHA;
    }
    //set the cpol
    if(ptr->Mode & SPI_CPOL)
    {
        Reg->CR1 |= SPI_CR1_CPOL;
    }
    else
    {
        Reg->CR1 &= ~SPI_CR1_CPOL;
    }

    if(ptr->CharLen == 16)
    {
        Reg->CR1 |= SPI_CR1_DFF;
    }
    else
    {
        Reg->CR1 &= ~SPI_CR1_DFF;
    }
    if(ptr->ShiftDir == SPI_SHIFT_LSB)
    {
        Reg->CR1 |= SPI_CR1_LSBFIRST;
    }
    else
    {
        Reg->CR1 &= ~SPI_CR1_LSBFIRST;
    }
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

    //����ֻ���SPI�ڳ�ʼ��
    temp &= ~SPI_CR1_RXONLY;                   // ȫ˫��ģʽ
    temp |= SPI_CR1_SSM;                       // ���nss����
    temp |= SPI_CR1_SSI;
    temp |= SPI_CR1_MSTR;                      // SPI����
    temp &= ~SPI_CR1_DFF;                      // 8λ���ݸ�ʽ
    temp |= SPI_CR1_CPOL;                      // ����ģʽ��SCKΪ1 CPOL=1
    temp |= SPI_CR1_CPHA;                      // ���ݲ����ӵ�2��ʱ����ؿ�ʼ,CPHA=1
    //��SPI1����APB2������.ʱ��Ƶ�����Ϊ84MhzƵ��.
    temp |= 7<<3;                              // Fsck=Fpclk1/256
    temp &= ~SPI_CR1_LSBFIRST;                 // MSB First
    temp |= SPI_CR1_SPE;                       // SPI����
    Reg->CR1 = (u16)temp ;                     // ����CR1
    Reg->I2SCFGR &= (u16)(~SPI_I2SCFGR_I2SMOD);// ѡ��SPIģʽ
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
        if(Reg ==  tg_SpiReg[SPIPort])
            break;
    }
    return Board_SpiCsCtrl(SPIPort,cs,0);
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
        if(Reg ==  tg_SpiReg[SPIPort])
            break;
    }

    return Board_SpiCsCtrl(SPIPort,cs,1);
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
    if( (Reg != tg_SpiReg[CN_SPI1]) && (Reg != tg_SpiReg[CN_SPI2])
            && (Reg != tg_SpiReg[CN_SPI3]) )
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
    while ( ( (Reg->SR & SPI_SR_RXNE) == 0 ) && (time > 0))
    {
        time --;
        DJY_DelayUs(1);
    }

    if(time == 0)
        return -1;
    return Reg->DR & 0xFFFF ;
}

// =============================================================================
// ���ܣ�дSPI���ͼĴ�������ѯ��ʽ,��__SPI_TxRxPoll����
// ������Reg,�Ĵ�����ַ��wData��д����
// ���أ���
// =============================================================================
static s32 __SPI_Write( tagSpiReg * Reg,uint16_t wData )
{
    u32 time = CN_TIMEOUT;
    /* Send data */
    while ( ( (Reg->SR & SPI_SR_TXE) == 0 ) && (time > 0) )
    {
        time --;
        DJY_DelayUs(1);
    }
    if(time == 0)
        return -1;
    Reg->DR = wData;
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

    Reg->CR1 |= SPI_CR1_SPE;
    len_limit = MAX(wrSize, rdSize + recvoff);
    for (i=0;i<len_limit;i++)
    {
        if (i<wrSize)
        {
            __SPI_Write(Reg,srcAddr[i]);
        }
        else if((i>=wrSize)&&(i<len_limit))
        {
          __SPI_Write(Reg,0);
        }
        if((destAddr) && (i>=recvoff) )
        {
            destAddr[i-recvoff] = __SPI_Read(Reg);
        }
        else
        {
            __SPI_Read(Reg);
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

    if(Reg == tg_SpiReg[CN_SPI1])
        Param = &IntParamset1;
    else if(Reg == tg_SpiReg[CN_SPI2])
        Param = &IntParamset2;
    else
        Param = &IntParamset3;

    Param->SendDataLen = sendlen;
    Param->RecvDataLen = recvlen;
    Param->RecvOffset  = recvoff;

    __SPI_IntEnable(Reg,SPI_CR2_TXEIE);
    Reg->CR1 |= SPI_CR1_SPE;

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

    if(IntLine == CN_INT_LINE_SPI1)
    {
        pSCB = s_ptSpi1_CB;
        Reg = (tagSpiReg *)tg_SpiReg[CN_SPI1];
        param = &IntParamset1;
    }
    else if(IntLine == CN_INT_LINE_SPI2)
    {
        pSCB = s_ptSpi2_CB;
        Reg = (tagSpiReg *)tg_SpiReg[CN_SPI2];
        param = &IntParamset2;
    }
    else if(IntLine == CN_INT_LINE_SPI3)
    {
        pSCB = s_ptSpi3_CB;
        Reg = (tagSpiReg *)tg_SpiReg[CN_SPI3];
        param = &IntParamset3;
    }
    else
        return 0;

    spi_sr = Reg->SR;
    spi_cr2 = Reg->CR2;
    //�����ж�
    if( (spi_cr2 & SPI_CR2_TXEIE) && (spi_sr & SPI_SR_TXE)) //ʹ���˷����жϣ���TXRIS
    {
        temp = SPI_PortRead(pSCB,&ch,1);//�Ƿ���������Ҫ����

        if(temp >0)
        {
            param->SendDataLen --;
            param->RecvOffset  --;
            Reg->DR = ch;
        }
        else                                            //�������ͽ���
        {
            //�ط����ж�
            __SPI_IntDisable(Reg,SPI_CR2_TXEIE);
            param->RecvOffset -= param->SendDataLen;
            param->SendDataLen = 0;

            ch = Reg->DR;

            //��������ˣ��ж��Ƿ���Ҫ����
            if(param->RecvDataLen > 0)
            {
                //MASTERģʽ�£����ͺͽ���ͬʱ����
                Reg->DR = 0xAA;
                __SPI_IntEnable(Reg,SPI_CR2_RXNEIE);    //ʹ�ܽ����ж�
                firstrcv = 1;
            }
        }
    }
    else if((spi_cr2 & SPI_CR2_RXNEIE) && (spi_sr & SPI_SR_RXNE))    //�����ж�
    {
        ch = Reg->DR;
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
            Reg->DR = 0xAA;
        }
        else                    //���������Ѿ����
        {
            param->RecvDataLen = 0;
            __SPI_IntDisable(Reg,SPI_CR2_RXNEIE);
        }
    }

    if(param->RecvDataLen + param->SendDataLen == 0)
    {
        Reg->CR1 &= ~SPI_CR1_SPE;
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
    struct SPI_CB **pSpiCB;
    u8 IntLine;

    switch(port)
    {
    case CN_SPI1:
        SPI_Config.BusName          = "SPI1";
        SPI_Config.SPIBuf           = (u8*)&s_Spi1_Buf;
        IntLine = CN_INT_LINE_SPI1;
        pSpiCB = &s_ptSpi1_CB;
        break;
    case CN_SPI2:
        SPI_Config.BusName          = "SPI2";
        SPI_Config.SPIBuf           = (u8*)&s_Spi2_Buf;
        IntLine = CN_INT_LINE_SPI2;
        pSpiCB = &s_ptSpi2_CB;
        break;
    case CN_SPI3:
        SPI_Config.BusName          = "SPI3";
        SPI_Config.SPIBuf           = (u8*)&s_Spi3_Buf;
        IntLine = CN_INT_LINE_SPI3;
        pSpiCB = &s_ptSpi3_CB;
        break;
    default:
        return (0);
    }
    SPI_Config.SpecificFlag     = (ptu32_t)tg_SpiReg[port];
    SPI_Config.SPIBufLen        = CFG_SPI_BUF_LEN;
    SPI_Config.MultiCSRegFlag   = false;
    SPI_Config.pTransferPoll    = (TransferPoll)__SPI_TxRxPoll;
    SPI_Config.pTransferTxRx    = (TransferFunc)__SPI_TransferTxRx;
    SPI_Config.pCsActive        = (CsActiveFunc)__SPI_BusCsActive;
    SPI_Config.pCsInActive      = (CsInActiveFunc)__SPI_BusCsInActive;
    SPI_Config.pBusCtrl         = (SPIBusCtrlFunc)__SPI_BusCtrl;


    __SPI_HardConfig((u32)SPI_Config.SpecificFlag); // SPI�Ĵ�������

    __SPI_IntConfig(IntLine); // ע���ж�

    if(*pSpiCB = SPI_BusAdd(&SPI_Config)) // ��SPI�˿���ӵ�SPI����
        return (1);
    else
        return (0);
}
//-----------------------------------------------------------------------------
//����: ��װSPI�˿�
//����: SPI�˿ں�
//����: -1 -- ʧ��; 0 -- �ɹ�;
//��ע:
//-----------------------------------------------------------------------------
s32 ModuleInstall_SPI(u8 Port)
{
    if(CN_SPI1 != Port) // ���ֻ֧�ֵ�SPI1
        return (-1);
    if(0 == SPI_Initialize(Port))
        return (-1);
    return (0);
}

