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
// ģ��汾: V1spi
// ������Ա:
// ����ʱ��:
// =============================================================================

#include "djyos.h"
#include "int.h"
#include "cpu_peri.h"
#include "spibus.h"

// =============================================================================
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�
//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    bool_t SPI_Init(u8 SPI_port);
//    #if CFG_SPI1_ENABLE==1
//    ModuleInstall_SPI(CN_SPI1);
//    #endif
//    #if CFG_SPI2_ENABLE==1
//    ModuleInstall_SPI(CN_SPI2)
//    #endif
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip spi"//SPI��������
//parent:"spi bus"   //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"spi bus","cpu onchip gpio"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_SPI == false )
//#warning  " cpu_onchip_spi  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_SPI    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,32,512,
#define CFG_SPI_BUF_LEN               128       //"SPI��������С",
//%$#@enum,true,false,
#define CFG_SPI1_ENABLE               false     //"�Ƿ�ʹ��SPI1",
#define CFG_SPI2_ENABLE               false     //"�Ƿ�ʹ��SPI2",
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������
//@#$%component end configure

// =============================================================================


// =============================================================================
#if(0)
typedef struct _SPI_CTRL_
{
    vu32 SPISR;
    vu32 SPICR;
    vu32 SPISSR;
    vu32 REV0;
    vu32 SPITR;
    vu32 SPIRR;
    vu32 SPITR1;
    vu32 SPIRR1;
}tagSpiReg;

#define CN_SPI1_BASE        (0x41040000+0x80)
#define CN_SPI2_BASE        (0x410A0000+0x80)
#define CN_FIFO_LEN         (32)

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
#define CN_SPI1_BUF_LEN      128
struct SPI_IntParamSet IntParamset1;

#define CN_SPI2_BUF_LEN      128
struct SPI_IntParamSet IntParamset2;

static struct SPI_CB *spSpiCb[CN_SPI_NUM];

#define SPI_INTR_EN_MCU4    (0x42030000 + 0x610)
#define SPI_INTR_STS_MCU4   (0x42030000 + 0x510)
// =============================================================================
__attribute__((weak)) bool_t Board_SpiCsCtrl(u8 port,u8 cs,u8 en)
{
    return true;    //accomplished in board.c if needed
}
// =============================================================================
// ����: �ж�ʹ�ܺ�ʧ�ܺ���
// ����: tpSPI,�������ļĴ�����ָ��
// ����: ��
// =============================================================================
static void __Spi_IntEnable(volatile tagSpiReg *Reg)
{
    Reg->SPICR |= (1<<6);
}
static void __Spi_IntDisable(volatile tagSpiReg *Reg)
{
    Reg->SPICR &= ~(1<<6);
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
static void __Spi_SetClk(volatile tagSpiReg *tpSPI,u32 spisck)
{
       u32 temp,i;
        if(tpSPI == NULL)
            return;

    switch(spisck)
    {
        //ϵͳ��Ƶ����80M ���з�Ƶ
         case CN_SPI_SPEEK_40M:                  //2��Ƶ 000
             tpSPI->SPICR &= ~((3<<3) | (1<<8));//����0,ϵͳƵ�ʶ���Ƶֱ��������0����000
             break;

         case CN_SPI_SPEEK_20M:                  //4��Ƶ 010
             tpSPI->SPICR &= ~((3<<3) | (1<<8)); //����0
             tpSPI->SPICR |= 0x2 << 3;
             break;
         case CN_SPI_SPEEK_10M:              //8��Ƶ 011
             tpSPI->SPICR &= ~((3<<3) | (1<<8)); //����0
             tpSPI->SPICR |= 0x3 << 3;
             break;

         case CN_SPI_SPEEK_5M:                  //16��Ƶ 100
             tpSPI->SPICR &= ~((3<<3) | (1<<8)); //����0
             tpSPI->SPICR |= 0x1 << 8;
             break;

         case CN_SPI_SPEEK_2_5M:                //32��Ƶ 101
             tpSPI->SPICR &= ~((3<<3) | (1<<8)); //����0
             tpSPI->SPICR |= (0x1 << 8) | (0x1 << 3) ;
             break;
         case CN_SPI_SPEEK_13_33M:              //6��Ƶ  110
             tpSPI->SPICR &= ~((3<<3) | (1<<8)); //����0
             tpSPI->SPICR |=(0x1 << 8)|(0x2 <<3);
             break;

         case CN_SPI_SPEEK_6_67M:              //12 ��Ƶ 111
             tpSPI->SPICR &= ~((3<<3) | (1<<8)); //����0
             tpSPI->SPICR |=(0x1 << 8)|(0x3 <<3);
             break;
         default:
             printk("spisck arg set err\r\n");
             tpSPI->SPICR &= ~((3<<3) | (1<<8));//����0,ϵͳƵ�ʶ���Ƶֱ��������0����000
             break;
    }
}

// =============================================================================
// ���ܣ�SPI���ƼĴ����������ã���PHA��CPOL��ʱ�ӵȣ����ݸ��ּĴ�������
// ������tpSPI,SPI��������ַ
//       ptr,����ָ��
// ���أ���
// =============================================================================
static void __Spi_Config(volatile tagSpiReg *Reg,tagSpiConfig *ptr)
{
    __Spi_SetClk(Reg,ptr->Freq);

    //set the PHA
    if(ptr->Mode & SPI_CPHA)
    {
        Reg->SPICR |= 1<<2;
    }
    else
    {
        Reg->SPICR &= ~(1<<2);
    }
    //set the cpol
    if(ptr->Mode & SPI_CPOL)
    {
        Reg->SPICR |= 1<<1;
    }
    else
    {
        Reg->SPICR &= ~(1<<1);
    }

    if(ptr->CharLen <= 32 && ptr->CharLen >= 4)
    {
        Reg->SPICR &= ~(0x3F<<15);
        Reg->SPICR |= ptr->CharLen;
    }
}

// =============================================================================
// ����: SPIĬ��Ӳ����ʼ�����ã���Ҫ��ʱ�����ú�GPIOд������������
// ����: RegBaseAddr,�Ĵ�����ַ
// ����: ��
// =============================================================================
static void __Spi_HardDefaultSet(u8 port)
{
    tagSpiReg * Reg;
    if(port == CN_SPI1)
        Reg = (tagSpiReg *)CN_SPI1_BASE;
    else if(port == CN_SPI2)
        Reg = (tagSpiReg *)CN_SPI2_BASE;
    else
        return;

    Reg->SPICR      = 0;
    __Spi_SetClk(Reg,5000*000);             //Ĭ������Ϊ5M
    Reg->SPICR      |= (1<<27);             //FLUSH FIFO
    Reg->SPICR      |= (1<<28) | (1<<29);   //FIFO EN
    Reg->SPICR      |= (8<<15);             //8bits,others default
    Reg->SPICR      |= (7<<24);             //DISABLE CS
}

// =============================================================================
// ���ܣ�SPIƬѡʹ�ܣ�ʹƬѡ��Ч
// ������Reg����ģ���ڼ�SPI�Ĵ�����ַ
//       cs,Ƭѡ��
// ���أ���
// ˵����
// =============================================================================
static bool_t __Spi_BusCsActive(tagSpiReg *Reg, u8 cs)
{
    u8 port = CN_SPI1;

    if((u32)Reg != CN_SPI1_BASE)
    {
        port = CN_SPI2;
    }

    if(cs < 4)
    {
        Reg->SPICR |= (7<<24);
        Reg->SPICR &= ~(1<<cs);
    }

    Board_SpiCsCtrl(port,cs,1);

    return true;
}

// =============================================================================
// ���ܣ�SPIƬѡʧ�ܣ�ʹƬѡ��Ч
// ������Reg����ģ���ڼ�SPI�Ĵ�����ַ
//       cs,Ƭѡ��
// ���أ���
// =============================================================================
static bool_t __Spi_BusCsInActive(tagSpiReg *Reg, u8 cs)
{
    u8 port = CN_SPI1;

    if((u32)Reg != CN_SPI1_BASE)
    {
        port = CN_SPI2;
    }

    if(cs < 4)
    {
        Reg->SPICR |= (7<<24);
    }

    Board_SpiCsCtrl(port,cs,0);
    return true;
}

// =============================================================================
// ���ܣ�SPI���߿��ƻص����������ϲ���ã�Ŀǰֻʵ�ֶ�SPIʱ��Ƶ������
// ������Reg,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//       cmd,����
//       data1,data2,���ݣ�������������
// ���أ���
// =============================================================================
static s32 __Spi_BusCtrl(tagSpiReg *Reg,u32 cmd,ptu32_t data1,ptu32_t data2)
{
    s32 result = 1;
    if(Reg == NULL)
        return 0;

    switch(cmd)
    {
    case CN_SPI_SET_CLK:
        __Spi_SetClk(Reg,data1);
        break;
    case CN_SPI_CS_CONFIG:
        __Spi_Config(Reg,(tagSpiConfig *)data1);
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

static u32 __Spi_Write(tagSpiReg *Reg,u8 Data)
{
    while(!(Reg->SPISR & (1<<3)));          //xmit empty

    Reg->SPITR1 = Data;
    Reg->SPICR |= (1<<5);                   //start
    while(!(Reg->SPISR & (1<<7)))   ;       //done

    while(!(Reg->SPISR & (1<<2)))   ;       //rcv full

    return (u32)(Reg->SPIRR1);
}
// =============================================================================
// ���ܣ���ѯ��ʽspi��д���ݣ�����ƫ�ƴ˴����˼򻯴���ֱ����Ϊ��sendlen��ͬ��������
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
static bool_t __Spi_TxRxPoll(tagSpiReg *Reg,u8* srcAddr,u32 wrSize,
                            u8* destAddr,u32 rdSize,u32 recvoff,u8 cs)
{
    u32 i,data,len_limit;

    if(wrSize + rdSize == 0)
        return false;
    len_limit = (wrSize > (rdSize + recvoff))? wrSize:(rdSize + recvoff);

    __Spi_IntDisable(Reg);
    Reg->SPICR |= (1<<7);               //ENABLE SPI
    while(!(Reg->SPISR & (1<<17)))     //RXFIFO NOT EMPTY
    {
        i = Reg->SPIRR;                 //����FIFO
    }
    Reg->SPICR |= (1<<28)|(1<<29);      //FIFO NOT NEEDED

    for (i=0;i<len_limit;)
    {
        if (i<wrSize)
        {
            __Spi_Write(Reg,srcAddr[i]);
        }
        else if((i>=wrSize)&&(i<len_limit))
        {
            data = __Spi_Write(Reg,0);
        }
        if((destAddr) && (i>=recvoff) )
        {
            destAddr[i-recvoff] = (u8)data;
        }
    }
    Reg->SPICR &= ~(1<<5);                  //disable start
    Reg->SPICR &= ~(1<<7);                  //disable spi
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
static bool_t __Spi_TransferTxRx(tagSpiReg *Reg,u32 sendlen,u32 recvlen,
                                u32 recvoff)
{
    u8 i,ch;
    struct SPI_CB *SPI_SCB;
    struct SPI_IntParamSet *Param=NULL;

    if((u32)Reg == CN_SPI1_BASE)
    {
        Param = &IntParamset1;
        SPI_SCB = spSpiCb[CN_SPI1];
    }

    if((u32)Reg == CN_SPI2_BASE)
    {
        Param = &IntParamset2;
        SPI_SCB = spSpiCb[CN_SPI2];
    }

    __Spi_IntDisable(Reg);
    Reg->SPICR |= (1<<7);                   //ENABLE SPI
    Reg->SPICR &= ~((1<<28)|(1<<29));       //FIFO ENABLE
    while(!(Reg->SPISR & (1<<17)))          //RXFIFO NOT EMPTY
    {
        i = Reg->SPIRR;                 //����FIFO
    }

    Param->SendDataLen = sendlen;
    Param->RecvDataLen = recvlen;
    Param->RecvOffset  = recvoff;

    sendlen = (sendlen > CN_FIFO_LEN)?CN_FIFO_LEN:sendlen;
    for(i = 0; i < sendlen; i++)
    {
        if(SPI_PortRead(SPI_SCB,&ch,1))
            Reg->SPITR = ch;
        else
            break;
    }

    Param->SendDataLen -= sendlen;

    Reg->SPICR |= (1<<5);                   //START
    __Spi_IntEnable(Reg);

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
u32 Spi_ISR(ptu32_t IntLine)
{
    struct SPI_CB *SPI_SCB=NULL;
    struct SPI_IntParamSet *param;
    tagSpiReg *Reg;
    u32 status;
    u8 ch[32],i=0,trans;

    status = *(u32*)SPI_INTR_STS_MCU4;
    if(status & (1<<4))
    {
        SPI_SCB = spSpiCb[CN_SPI1];
        Reg = (tagSpiReg *)SPI_SCB->SpecificFlag;
        param = &IntParamset1;
    }
    else if(status & (1<<5))
    {
        SPI_SCB = spSpiCb[CN_SPI2];
        Reg = (tagSpiReg *)SPI_SCB->SpecificFlag;
        param = &IntParamset2;
    }
    else
    {
        return 0;
    }

    if(param->RecvDataLen > 0)
    {
        while(!(Reg->SPISR & (1<<17)))          //RXFIFO_EMPTY
        {
            ch[0] = Reg->SPIRR;
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
                {
                    break;
                }
            }
        }
    }

    if((param->SendDataLen > 0) || (param->RecvDataLen > 0))
    {
        if(Reg->SPISR & (1<<25))                    //TXFIFO_EMPTY
        {
            trans = SPI_PortRead(SPI_SCB,ch,32);
            if(trans >0)
            {
                for(i = 0; i < trans; i++)
                {
                    Reg->SPITR = ch[i];
                }
                param->SendDataLen -= trans;
            }
            else
            {
                trans = (param->RecvDataLen > 32)?32:param->RecvDataLen;
                for(i = 0; i < trans; i++)
                {
                    Reg->SPITR = 0x00;
                }
            }
        }
    }

    if(param->SendDataLen + param->RecvDataLen == 0)
    {
        __Spi_IntDisable(Reg);
        Reg->SPICR &= ~(1<<5);                  //disable start
        Reg->SPICR &= ~(1<<7);                  //disable spi
    }
    else
    {
        Reg->SPICR |= (1<<5);                   //start
    }

    return 0;
}

// =============================================================================
// ����: SPI�ж����ú���
// ����: IntLine,�ж���
// ����: ��
// =============================================================================
static void __Spi_IntConfig(u8 port)
{
    u8 IntLine = CN_INT_LINE_SPI;

    if(port == CN_SPI1)
    {
        *(u32*)SPI_INTR_EN_MCU4 |= (1<<4);
    }
    else
    {
        *(u32*)SPI_INTR_EN_MCU4 |= (1<<5);
    }
    //�ж��ߵĳ�ʼ��
    Int_Register(IntLine);
    Int_SetClearType(IntLine,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(IntLine,Spi_ISR);
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
bool_t ModuleInstall_SPI1(ptu32_t para)
{
    struct SPI_Param Spi_Config;
    static struct SPI_CB s_Spi1_CB;
    static u8 s_SPI1_Buf[CN_SPI1_BUF_LEN];

    Spi_Config.BusName          = "SPI1";
    Spi_Config.SPIBuf           = (u8*)&s_SPI1_Buf;
    Spi_Config.SPIBufLen        = CN_SPI1_BUF_LEN;
    Spi_Config.SpecificFlag     = CN_SPI1_BASE;
    Spi_Config.MultiCSRegFlag   = false;
    Spi_Config.pTransferTxRx    = (TransferFunc)__Spi_TransferTxRx;
    Spi_Config.pCsActive        = (CsActiveFunc)__Spi_BusCsActive;
    Spi_Config.pCsInActive      = (CsInActiveFunc)__Spi_BusCsInActive;
    Spi_Config.pBusCtrl         = (SPIBusCtrlFunc)__Spi_BusCtrl;
    Spi_Config.pTransferPoll    = (TransferPoll)__Spi_TxRxPoll;
    spSpiCb[CN_SPI1] = &s_Spi1_CB;

    __Spi_HardDefaultSet(CN_SPI1);
    __Spi_IntConfig(CN_SPI1);

    if(NULL == SPI_BusAdd_s(&s_Spi1_CB,&Spi_Config))
        return 0;
    return 1;
}

bool_t ModuleInstall_SPI2(ptu32_t para)
{
    struct SPI_Param Spi_Config;
    static struct SPI_CB s_Spi2_CB;
    static u8 s_SPI2_Buf[CN_SPI2_BUF_LEN];

    Spi_Config.BusName          = "SPI2";
    Spi_Config.SPIBuf           = (u8*)&s_SPI2_Buf;
    Spi_Config.SPIBufLen        = CN_SPI2_BUF_LEN;
    Spi_Config.SpecificFlag     = CN_SPI2_BASE;
    Spi_Config.MultiCSRegFlag   = false;
    Spi_Config.pTransferTxRx    = (TransferFunc)__Spi_TransferTxRx;
    Spi_Config.pCsActive        = (CsActiveFunc)__Spi_BusCsActive;
    Spi_Config.pCsInActive      = (CsInActiveFunc)__Spi_BusCsInActive;
    Spi_Config.pBusCtrl         = (SPIBusCtrlFunc)__Spi_BusCtrl;
    Spi_Config.pTransferPoll    = (TransferPoll)__Spi_TxRxPoll;
    spSpiCb[CN_SPI2] = &s_Spi2_CB;

    __Spi_HardDefaultSet(CN_SPI2);
    __Spi_IntConfig(CN_SPI2);

    if(NULL == SPI_BusAdd_s(&s_Spi2_CB,&Spi_Config))
        return 0;
    return 1;
}
#endif

