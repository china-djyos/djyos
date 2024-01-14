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
// ģ��汾:
// ������Ա:
// ����ʱ��:
// =============================================================================
#include "string.h"
#include "djyos.h"
#include "cpu_peri.h"
#include "int.h"
#include "spibus.h"
#include "dbug.h"
#include "cpu_peri_spi.h"
#include "cpu_peri_io.h"
#include "set-cache.h"
#include "sys_clock.h"
#include "sys_dma.h"
#include "math.h"
// =============================================================================
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�
//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    bool_t ModuleInstall_SPI(u8 SPI_port);
//    #if CFG_SPI0_ENABLE==1
//    ModuleInstall_SPI(CN_SPI0);
//    #endif
//    #if CFG_SPI1_ENABLE==1
//    ModuleInstall_SPI(CN_SPI1);
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
//dependence:"spi bus"//������������������������none����ʾ�������������
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
#define CFG_SPI0_ENABLE               false     //"�Ƿ�ʹ��SPI0",
#define CFG_SPI1_ENABLE               false     //"�Ƿ�ʹ��SPI1",
#define CFG_SPI0_DMA_ENABLE           true      //"SPI0�Ƿ�ʹ��DMA����",
#define CFG_SPI1_DMA_ENABLE           false     //"SPI1�Ƿ�ʹ��DMA����",
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������
//@#$%component end configure

// =============================================================================
#define CN_TIMEOUT  (200*1000)

//�����ж�����ʹ�õľ�̬���ṹ��
struct SPI_IntParamSet
{
    u32 SendDataLen;
    u32 RecvDataLen;
    u32 RecvOffset;
};

#define delay_ms(x) DJY_DelayUs(x * 1000)
#define u64_t u64
#define u32_t u32
#define u16_t u16
#define u8_t  u8
#define sysprintf  printf
// =============================================================================
//����SPI���ƿ���жϾ�̬��
//#define CFG_SPI_BUF_LEN              128
//#define CN_SPI_DMA_BUF_LEN          128
static struct SPI_CB *s_ptSpi0_CB;
static u8 s_Spi0_Buf[CFG_SPI_BUF_LEN];
struct SPI_IntParamSet IntParamset0;

static struct SPI_CB *s_ptSpi1_CB;
static u8 s_Spi1_Buf[CFG_SPI_BUF_LEN];
struct SPI_IntParamSet IntParamset1;
/*---------------------------------------------------
�޸ļ�¼
2020-04-13
    1.128x512�ֽ�д��������sys_spi.c��SPI_Flash_Write�����Ĳ���unsigned short NumByteToWrite��Ϊunsigned int NumByteToWrite
2020-08-19
    1.����DMA��дģʽ
2020-12-17
    1.����32K�飬64K���������
    2.���Ӷ�UID(ΨһID),�豸ID����
2021-06-10
    1.DMA���жϿ����������дʧ�ܣ�����һ�����⻺��virtual_buff�滻��д������NULL���������
2021-07-07
    1.virtual_buff��Ϊʹ��malloc���䣬�����ʹ��ϵͳ,����ʹ��C���malloc,��ʹ�ù̶��ڴ�����ϵͳ���ڴ����
2021-07-09
    1.���Ӷ�16MB�����ͺŶ�д��֧��,��Ҫ���ӽ���4��ַģʽ��4��ַ��д-W25Q256����������
    ��Ҫע�⣬�洢�����SPI NOR FLASHʹ��4��ַʱ��ʹ��������˳������Ӧ�ûָ�2��ַģʽ����Ϊ���ϵ�������FLASHһֱĬ��4��ַ��
    ��оƬ����2��ַģʽ��ȡ���򣬻���ɸ�λ���������һֱ��������ģʽ�����⡣
2021-07-14
    1.���Ӷ�CLK��Ƶ�����������Ƶ�����ú���,����Ƶ��̫�߶���д�������⡣
2022-05-19
    1.�������CS�ſ���-����һ��SPI���ԹҶ��SPI�豸��
2022-07-03
    1.���Ӵ��豸֧��-֧��CPU+DMAģʽ-�����շ�65K������
2022-08-19
    1.�������߶�����дģʽ��ԭ��8λ��Ϊ32λ�����Ӷ�д�ٶȡ�
----------------------------------------------------*/

int rx_dual_en=0;//0=RX���߶� 1=RX˫�߶�

#if (CFG_SPI0_DMA_ENABLE == true) || (CFG_SPI1_DMA_ENABLE ==true)
#define  SPI_DMA_EN
#endif

#if SPI_DMA_EN  //DMA����
    DMA * SPI_TXdma;
    DMA * SPI_RXdma;
#endif

//SPI �豸ģʽ��־
unsigned int spi_slave_mode_en=0;//=0����ģʽ��=1�ӻ�ģʽ
//HZ ���ʱ������
int spi_max_clk=105000000;//Ĭ�����ʱ��Ƶ��


/*---------------------------------------------------
SPI DMA����ʱ�ĵȴ�
1.�����ѯDMA״̬̫�ܼ�������DMA�ٶȽ��ͣ�������ʱ�Ǳ�Ҫ�ġ�
2.������ʱ���滻��ϵͳ��ʱ(����1ms��ʱ)ִ������������߶�����ִ��Ч�ʡ�
---------------------------------------------------*/
void spi_dma_delay(void)
{
    DJY_DelayUs(10);
}
//���ʱ������
void set_max_clk(int clk)
{
    spi_max_clk=clk;
}
/*---------------------------------------------------
����SPIΪ�豸ģʽ-��ʼ��Ҫ����spi_spi_confing()
---------------------------------------------------*/
void spi_set_slave_mode(void)
{
    spi_slave_mode_en=1;
}
/*---------------------------------------------------
�˳�SPI�豸ģʽ
---------------------------------------------------*/
void spi_exit_slave_mode(void)
{
    spi_slave_mode_en=0;
}
/*---------------------------------------------------
��ȡ���ջ������ֵ
----------------------------------------------------*/
int read_rx_count(int SPI)
{
virtual_addr_t addr =spi_get_addr(SPI);
  int len=read32(addr + SPI_FSR) & 0xff;
    return len;
}
//ʱ��ģʽ
#define Select_Clock_Divide_Rate_1_EN 0 //1=Clock_Divide_Rate_1   0=Clock_Divide_Rate_2

/*---------------------------------------------------
SPI����
---------------------------------------------------*/
void spi_spi_confing(int SPI)
{
    virtual_addr_t addr=spi_get_addr(SPI);
    u32_t val;

    /*
    if CPU_CLK=600MHZ
    AHB_CLK=CPU_CLK/2=600/2=300
    SPI_CLK=AHB_CLK / (2*(n + 1));
    [n=2 SPI_CLK=50MHZ]
    [n=1 SPI_CLK=75MHZ]
    [n=0 SPI_CLK=150MHZ]
    */

    /*
    if CPU_CLK=408MHZ
    AHB_CLK=CPU_CLK/2=408/2=204
    SPI_CLK=AHB_CLK / (2*(n + 1));


  [n=5 SPI_CLK=17MHZ]
    [n=4 SPI_CLK=20.4MHZ]
    [n=3 SPI_CLK=25.5MHZ]
    [n=2 SPI_CLK=34MHZ]
    [n=1 SPI_CLK=51MHZ]
    [n=0 SPI_CLK=102MHZ]
    */
    int spi_clk=0;
    if(spi_slave_mode_en==0)//�豸ģʽ���ó�ʼ��ʱ��
    {
        int n=0;
        int ahb_clk=get_ahb_frequency();
        sysprintf("ahb_clk= %d hz\r\n",ahb_clk);
        while(n<0xff)
        {
#if Select_Clock_Divide_Rate_1_EN
            spi_clk=ahb_clk / (1<<(n+1));
#else
            spi_clk=ahb_clk / (2*(n + 1));
#endif
            if(spi_clk>spi_max_clk)//SPI ʱ��Ƶ������
            {
                n++;
            }else
            {
                break;
            }
        }
        sysprintf("CLK n=%d\r\n",n);
        sysprintf("SPI%d initial CLK[%dHZ]...\r\n",SPI,spi_clk);
            /* Set spi clock rate control register */
        #if Select_Clock_Divide_Rate_1_EN
            write32(addr + SPI_CCR, 0x00000000|(n<<8));
        #else
            write32(addr + SPI_CCR, 0x00001000|n);
        #endif
    }
    /* Enable spi and do a soft reset */
    val = read32(addr + SPI_GCR);
    val |= ((u64_t)1 << 31) | (1 << 7) | (1 << 1) | (1 << 0);
    if(spi_slave_mode_en==1)val &= ~( 1 << 1);//�豸ģʽ
    write32(addr + SPI_GCR, val);
    while(read32(addr + SPI_GCR) & ((u64_t)1 << 31)){};
  //--------------
  if(spi_slave_mode_en==0)//�豸ģʽ���ó�ʼ��
  {
        val = read32(addr + SPI_TCR);
        //��������
        val &= ~((0x1 << 13)|(0x1 << 11));//SDM(bit13)  SDC(bit11)  ��0
        if(spi_clk>=80000000)val |=((0x0 << 13)|(0x1 << 11));//delay one cycle sample  0  1  >=80 MHz
        else if(spi_clk<=24000000)val |=((0x1 << 13)|(0x0 << 11));//normal sample  1  0  <=24 MHz
        else if(spi_clk<=40000000)val |=((0x0 << 13)|(0x0 << 11));//delay half cycle sample  0  0  <=40 MHz

        val &= ~(0x3 << 0);
        val |= (1 << 6) | (1 << 2);
//        if(spi_software_cs_en==1)val &= ~( 1 << 6);//���CS��ʹ��
        write32(addr + SPI_TCR, val);
    }
  //FIFO��λ
    val = read32(addr + SPI_FCR);
    val |= ((u64_t)1 << 31) | (1 << 15);
    write32(addr + SPI_FCR, val);
}
/*---------------------------------------------------
SPI FIFO DMAC�����ȼ�����
tx_level/rx_level = 0x0-0x40;
---------------------------------------------------*/
void sys_spi_set_fifo_request_trigger_level(int SPI,int tx_level,int rx_level)
{
  virtual_addr_t addr = spi_get_addr(SPI);
  u32_t val;
  val = read32(addr + SPI_FCR);
  val &=~( (0XFF << 16) | (0XFF << 0));//�� TX RX
  val |= ((tx_level<< 16) | (rx_level << 0));//���ô���ֵ
  write32(addr + SPI_FCR, val);
}

/*---------------------------------------------------
ʹ��DMA_DRQ
---------------------------------------------------*/
void sys_spi_enable_drq(int SPI,int tx_drq_en,int rx_drq_en)
{
    virtual_addr_t addr =spi_get_addr(SPI);

    if(tx_drq_en==0)C_BIT(addr+SPI_FCR,24);//TX_DRQ
    else S_BIT(addr+SPI_FCR,24);

    if(rx_drq_en==0)C_BIT(addr+SPI_FCR,8);//RX_DRQ
    else S_BIT(addr+SPI_FCR,8);
}
/*---------------------------------------------------
SPI��ʼ
---------------------------------------------------*/
void sys_spi_start(int SPI)
{
    virtual_addr_t addr =spi_get_addr(SPI);
    S_BIT(addr + SPI_TCR,31);
}
/*---------------------------------------------------
���ü���ֵ
---------------------------------------------------*/
void sys_spi_set_count(int SPI,int txc,int len)
{
  virtual_addr_t addr =spi_get_addr(SPI);
  write32(addr + SPI_MBC, len & 0xffffff);
  write32(addr + SPI_MTC, txc & 0xffffff);
  write32(addr + SPI_BCC, (txc & 0xffffff) | ((rx_dual_en&0xf)<<28) );
}
/*---------------------------------------------------
SPI SPI CPU����
----------------------------------------------------*/
int sys_spi_cpu_transfer(int SPI,void * txbuf, void * rxbuf, int len)
{
  virtual_addr_t addr =spi_get_addr(SPI);
  u32_t * tx32 = (u32_t *)txbuf;
  u32_t * rx32 = (u32_t *)rxbuf;
  int n, i;
  int txc=len;
  int rxc=len;
  if(!txbuf)txc=0;
  if(rx_dual_en>0xf0)txc=0;//���߽���ʱ������
  if(spi_slave_mode_en==0)
    {
        sys_spi_set_count(SPI,txc,len);
        sys_spi_start(SPI);
    }
  while(!((txc==0)&&(rxc==0)))
  {
    //*************��������*************
    if(txc>0)
    {
#define CPU_TX_LEN 32  //CPU����һ�εĳ��ȣ�0-64
      if(txc >= CPU_TX_LEN)//4Byteģʽ����
      {
        n=CPU_TX_LEN;
        if(((read32(addr + SPI_FSR)>>16) & 0xff)<=(64-n))
        {
          u32_t * buf32 = tx32;
          for(i = n; i > 0; i-=4)write32(addr + SPI_TXD, *buf32++);
          txc-=n;
          tx32+=n/4;
        }
      }else//1Byteģʽ����
      {
        n=txc;
        if(((read32(addr + SPI_FSR)>>16) & 0xff)<=(64-n))
        {
          u8_t * buf8 =(u8_t *)tx32;
          for(i = n; i > 0; i--)write8(addr + SPI_TXD, *buf8++);
          txc-=n;
        }
      }
    }
    //*************��������*************
    if(rxc>0)
    {
#define CPU_RX_LEN 32  //CPU����һ�εĳ��ȣ�0-64
      if(rxc >= CPU_RX_LEN)//4Byteģʽ����
      {
        n=CPU_RX_LEN;
        if((read32(addr + SPI_FSR) & 0xff)>=n)
        {
          u32_t val32;
          for(i = n; i > 0;i-=4)
          {
            val32 = read32(addr + SPI_RXD);
            if(rx32)*rx32++ = val32;
          }
          rxc -= n;
        }
      }else//1Byteģʽ����
      {
        n=rxc;
        if((read32(addr + SPI_FSR) & 0xff)>=n)
        {
          u8_t *rx=(u8_t *)rx32;
          u8_t val8;
          for(i = n; i > 0;i--)
          {
            val8 = read8(addr + SPI_RXD);
            *rx++ = val8;
          }
          rxc -= n;
        }
      }
    }
  }

  if(spi_slave_mode_en==0)
    {
        //�ȴ�����- ����ʱ���������ж�-DMA��FLUSH���̿�ʡ�ԡ�
        int timeout=10000;
        while( (read32(addr + SPI_TCR)&0x80000000) && ((--timeout)>0) );
        if(timeout==0)
        {
            sysprintf("ERR:sys_spi_cpu_transfer timeout.\r\n");
            return -1;
        }
    }

  return len;
}

#if SPI_DMA_EN
/*---------------------------------------------------
SPI DMA transfe
----------------------------------------------------*/
int sys_spi_dma_transfer(int SPI,DMA * TXdma,DMA * RXdma,void * txbuf, void * rxbuf, int len)
{
  virtual_addr_t addr =spi_get_addr(SPI);

  int len1=len;
    int dma_data_width=DMA_DATA_WIDTH_32;
    /*1.�豸ģʽ,lenΪ4�ı���ʱ����32λ���ݿ��,����Ϊ8λ����,�����շ�����
      2.һ�����ݲ��ܶ�δ��䣬��Ϊʱ���������������ӻ�������;��ͣ��
      ���ݳ��ȱ���len<=NDMA_MAX_LEN��*/
    if(spi_slave_mode_en==1)
    {
        if(len1%4)dma_data_width=DMA_DATA_WIDTH_8;
        if(len1>NDMA_MAX_LEN)
        {
            sysprintf("ERR:sys_spi_dma_transfer()->len>NDMA_MAX_LEN\r\n");
            return 0;
        }
    }
    //---------
    int trlen=len1>NDMA_MAX_LEN?NDMA_MAX_LEN:len1;
    unsigned int txbuf1=(unsigned int)txbuf;
    unsigned int rxbuf1=(unsigned int)rxbuf;
RESTART:
  if(txbuf!=0)
    {
        TXdma->Type=NDMA;//����
        TXdma->Ch=0;//ͨ��
        TXdma->Byte_Counter=trlen;//Byte����
        TXdma->Continuous_Mode_Enable=0;//����ģʽ
        TXdma->Read_Byte_Counter_Enable=0;//������ֵʹ��
        //---------
        TXdma->Source_Address=(unsigned int)txbuf1;//Դ��ַ
        TXdma->Source_Address_Type=DMA_ADDRESS_TYEP_LINEER;//Դ��ַ����
        TXdma->Source_DRQ_Type=NDMAS_DRQ_Type_SDRAM_Memory;//Դ����
        TXdma->Source_Data_Width=dma_data_width;//Դ���ݿ��
        TXdma->Source_Burst_Length=DMA_BURST_LENGTH_1;//BURST
        //-----------
        TXdma->Destination_Address=(unsigned int)(addr + SPI_TXD);//Ŀ���ַ
        TXdma->Destination_Address_Type=DMA_ADDRESS_TYEP_IO;//Ŀ���ַ����
        TXdma->Destination_DRQ_Type=(SPI==0?NDMAD_DRQ_Type_SPI0_Tx:NDMAD_DRQ_Type_SPI1_Tx);//Ŀ������
        TXdma->Destination_Data_Width=dma_data_width;//Ŀ�����ݿ��
        TXdma->Destination_Burst_Length=DMA_BURST_LENGTH_1;//BURST
        //-----------
        //sysprintf("�������ݴ�С: %d \r\n",TXdma->Byte_Counter);
        DMA_Config(TXdma);
        DMA_Enable(TXdma);
    }
    //---------------------------------------------------------------------------------------
    RXdma->Type=NDMA;//����
    RXdma->Ch=1;//ͨ��
    RXdma->Byte_Counter=trlen;//Byte����
    RXdma->Continuous_Mode_Enable=0;//����ģʽ
    RXdma->Read_Byte_Counter_Enable=0;//������ֵʹ��
    //---------
    RXdma->Source_Address=(unsigned int)(addr + SPI_RXD);//Դ��ַ
    RXdma->Source_Address_Type=DMA_ADDRESS_TYEP_IO;//Դ��ַ����
    RXdma->Source_DRQ_Type=(SPI==0?NDMAS_DRQ_Type_SPI0_Rx:NDMAS_DRQ_Type_SPI1_Rx);//Դ����
    RXdma->Source_Data_Width=dma_data_width;//Դ���ݿ��
    RXdma->Source_Burst_Length=DMA_BURST_LENGTH_1;//BURST
    //-----------
    RXdma->Destination_Address=(unsigned int)rxbuf1;//Ŀ���ַ
    RXdma->Destination_Address_Type=DMA_ADDRESS_TYEP_LINEER;//Ŀ���ַ����
    RXdma->Destination_DRQ_Type=NDMAD_DRQ_Type_SDRAM_Memory;//Ŀ������
    RXdma->Destination_Data_Width=dma_data_width;//Ŀ�����ݿ��
    RXdma->Destination_Burst_Length=DMA_BURST_LENGTH_1;//BURST
    //-----------
    //sysprintf("�������ݴ�С: %d \r\n",RXdma->Byte_Counter);
    DMA_Config(RXdma);
    DMA_Enable(RXdma);
  //FIFO DMA �����ȼ�����
    if(dma_data_width==DMA_DATA_WIDTH_32)
        sys_spi_set_fifo_request_trigger_level(SPI,60,4);
    else
        sys_spi_set_fifo_request_trigger_level(SPI,64,1);
    //����DMA_DRQ
    sys_spi_enable_drq(SPI,txbuf==0?0:1,1);
  if(spi_slave_mode_en==0)
    {
        //���ü���ֵ
        sys_spi_set_count(SPI,txbuf==0?0:trlen,trlen);
        //SPI��ʼ
        sys_spi_start(SPI);
    }
    while(1)
    {
        if(DMA_Get_Full_TIP(RXdma))
        {
            //sysprintf("Transfer complete...\r\n");
            break;
        }
        //��ʱ������
        spi_dma_delay();
    }
    DMA_Disable(RXdma);
  if(txbuf!=0)DMA_Disable(TXdma);
    //����128K��������
    len1-=trlen;
    if(len1>0)
    {
        txbuf1+=trlen;
        rxbuf1+=trlen;
        trlen=len1>NDMA_MAX_LEN?NDMA_MAX_LEN:len1;
        goto RESTART;
    }
    //дͨCACHE
  //sysInvalidCache();
//  sysFlushCache(D_CACHE);
    Cache_InvalidData();
    return len;
}
#endif

/*SPI����
*/
int sys_spi_transfer(int SPI,void * txbuf, void * rxbuf, int len)
{
    if(len>0xffffff)//һ������䳤��Ϊ0xffffff.
    {
        sysprintf("ERR:spi transfer max len 0xffffff,len=0x%08x\r\n",len);
        return -1;
    }
#if SPI_DMA_EN
    void *p=0;
    if(len>64)//>64 DMAģʽ-DMAģʽʱ����ָ�벻��Ϊ0
    {
        if(rxbuf==NULL)
        {
            p=malloc(len);
            if(p==0)
            {
                sysprintf("ERR:malloc() rxbuf==0 \r\n");
                return 0;
            }
            rxbuf=p;
        }
        if(((int)txbuf%4)!=0)//DMA������Ҫ4�ֽڶ���
        {
            sysprintf("ERR:malloc() txbuf alignment(%08x)\r\n",txbuf);
            return 0;
        }
        if(((int)rxbuf%4)!=0)//DMA������Ҫ4�ֽڶ���
        {
            sysprintf("ERR:malloc() rxbuf alignment(%08x)\r\n",rxbuf);
            return 0;
        }

        sys_spi_dma_transfer(SPI,SPI_TXdma,SPI_RXdma,txbuf,rxbuf,len);
        if(p)free(p);
    }else//CPUģʽ
    {
#endif
        sys_spi_cpu_transfer(SPI,txbuf,rxbuf,len);
#if SPI_DMA_EN
    }
#endif
    return len;
}

/*SPI write_then_read
*/
static int sys_spi_write_then_read(int SPI,void * txbuf, int txlen,
        void * rxbuf, int rxlen)
{

    if(sys_spi_transfer(SPI,txbuf, NULL, txlen) != txlen)
        return -1;
  if(rx_dual_en>0)rx_dual_en|=0xf0;//2�߶�
    if(sys_spi_transfer(SPI,NULL, rxbuf, rxlen) != rxlen)
        return -1;
    return 0;
}
/*SPI write_then_write
*/
static int sys_spi_write_then_write(int SPI,void * txbuf0, int txlen0, void * txbuf1, int txlen1)
{
    if(sys_spi_transfer(SPI,txbuf0, NULL, txlen0) != txlen0)
        return -1;
    if(sys_spi_transfer(SPI,txbuf1, NULL, txlen1) != txlen1)
        return -1;
    return 0;
}


/*SPI clock_��ʼ��
*/
void spi_clock_init(int SPI)
{
    S_BIT(0x01c202c0,(20+SPI));/* Deassert reset */
    S_BIT(0x01c20060,(20+SPI));/* Open the bus gate */
}

// =============================================================================
// ����: SPIĬ��Ӳ����ʼ�����ã���Ҫ��ʱ������
// ����: RegBaseAddr,�Ĵ�����ַ
// ����: ��
// =============================================================================
void spi_init(int SPI)
{
    spi_clock_init(SPI);
    spi_spi_confing(SPI);
}
//**********************************************************************************

__attribute__((weak))  bool_t Board_SpiCsCtrl(u8 SPIPort,u8 cs,u8 level)
{
    return true;
}
__attribute__((weak))  void SPI_InLowPowerPinCfg(u8 port)
{
    return;
}
__attribute__((weak))  void SPI_OutLowPowerPinCfg(u8 port)
{
    return ;
}
// =============================================================================
// ����: �ж�ʹ�ܺ�ʧ�ܺ���
// ����: tpSPI,�������ļĴ�����ָ��
// ����: ��
// =============================================================================
static void __SPI_IntEnable(struct tagSpiReg *Reg,u32 IntSrc)
{
    Reg->SPI_IER |= IntSrc;
}
static void __SPI_IntDisable(struct tagSpiReg *Reg,u32 IntSrc)
{
    Reg->SPI_IER &= ~IntSrc;
}
// =============================================================================
// ���ܣ�SPIʱ�����ú���
// ������SPI��cpu��spi�ӿں�
//      Freq�������õ�ʱ���ٶȣ���λΪHz
// ���أ�true=�ɹ���false=ʧ��
// ˵����
// =============================================================================
static void __SPI_SetClk( struct tagSpiReg * Reg,u32 Freq)
{
    u32 n=0;
    u32 spi_clk=0;
    u32 val;
    u32 ahb_clk=get_ahb_frequency();
    if(Freq > spi_max_clk)
        Freq = spi_max_clk;
    while(n<0xff)
    {
#if Select_Clock_Divide_Rate_1_EN
        spi_clk=ahb_clk / (1<<(n+1));
#else
        spi_clk=ahb_clk / (2*(n + 1));
#endif
        if(spi_clk > Freq)      //SPI ʱ��Ƶ������
        {
            n++;
        }else
        {
            break;
        }
    }
    /* Set spi clock rate control register */
#if Select_Clock_Divide_Rate_1_EN
//  write32(addr + SPI_CCR, 0x00000000|(n<<8));
    Reg->SPI_CCR = 0x00000000|(n<<8);
#else
//  write32(addr + SPI_CCR, 0x00001000|n);
    Reg->SPI_CCR = 0x00001000|n;
#endif
}

// =============================================================================
// ���ܣ�SPI���ƼĴ����������ã���PHA��CPOL��ʱ�ӵȣ����ݸ��ּĴ�������
// ������tpSPI,SPI��������ַ
//       ptr,����ָ��
// ���أ���
// =============================================================================
static void __SPI_Config(struct tagSpiReg *Reg,tagSpiConfig *ptr)
{
    __SPI_SetClk(Reg,ptr->Freq);
//    Reg->CR1 &= ~SPI_CR1_SPE;

    //set the PHA
    if(ptr->Mode & SPI_CPHA)
    {
        Reg->SPI_TCR |= SPI_CTL_PHA;
    }
    else
    {
        Reg->SPI_TCR &= ~SPI_CTL_PHA;
    }
    //set the cpol
    if(ptr->Mode & SPI_CPOL)
    {
        Reg->SPI_TCR |= SPI_CTL_POL;
    }
    else
    {
        Reg->SPI_TCR &= ~SPI_CTL_POL;
    }

    if(ptr->CharLen == 16)  //C100�ƺ���֧��16λ���䡣
    {
    }
    else
    {
    }
    if(ptr->ShiftDir == SPI_SHIFT_LSB)
    {
        Reg->SPI_TCR |= SPI_CTL_LMTF;
    }
    else
    {
        Reg->SPI_TCR &= ~SPI_CTL_LMTF;
    }
}

// =============================================================================
// ���ܣ�SPIƬѡʹ�ܣ�ʹƬѡ��Ч,���������о��������Ƭѡ�Ĵ�������ֱ�ӷ���
// ������Reg����ģ���ڼ�SPI�Ĵ�����ַ
//       cs,Ƭѡ��
// ���أ���
// =============================================================================
static bool_t __SPI_BusCsActive(u32 SPI, u8 cs)
{
    if(cs < 4)  //��CPU��4����׼CS����
    {
        virtual_addr_t addr =spi_get_addr(SPI);
        u32_t val;
        val = read32(addr + SPI_TCR);
        val &= ~((0x3 << 4) | (0x1 << 7));
        val |= ((0 & 0x3) << 4) | (0 << 7);
        write32(addr + SPI_TCR, val);
        return true;
    }
    else
    {
        //�����gpio����CS����������֪�����õ����ĸ�gpio���޴ӿ��ƣ�Ӧ��boarddrv
        //��ʵ�����º�����
        return Board_SpiCsCtrl(SPI,cs,0);
    }
}

// =============================================================================
// ���ܣ�SPIƬѡʧ�ܣ�ʹƬѡ��Ч
// ������Reg����ģ���ڼ�SPI�Ĵ�����ַ
//       cs,Ƭѡ��
// ���أ���
// =============================================================================
static bool_t __SPI_BusCsInActive(u32 SPI, u8 cs)
{
    if(cs < 4)  //��CPU��4����׼CS����
    {
        virtual_addr_t addr =spi_get_addr(SPI);
        u32_t val;
        val = read32(addr + SPI_TCR);
        val &= ~((0x3 << 4) | (0x1 << 7));
        val |= ((0 & 0x3) << 4) | (0x1 << 7);
        write32(addr + SPI_TCR, val);
        return true;
    }
    else
    {
        //�����gpio����CS����������֪�����õ����ĸ�gpio���޴ӿ��ƣ�Ӧ��boarddrv
        //��ʵ�����º�����
        return Board_SpiCsCtrl(SPI,cs,1);
    }
}

// =============================================================================
// ���ܣ�SPI���߿��ƻص����������ϲ���ã�Ŀǰֻʵ�ֶ�SPIʱ��Ƶ������
// ������specific_flag,���Ա�ǣ���ģ���ڼ�IIC�Ĵ�����ַ
//       cmd,����
//       data1,data2,���ݣ�������������
// ���أ���
// =============================================================================
static s32 __SPI_BusCtrl(u32 SPI,u32 cmd,ptu32_t data1,ptu32_t data2)
{
    s32 result = 1;
    struct tagSpiReg *Reg = (struct tagSpiReg *)spi_get_addr(SPI);

    switch(cmd)
    {
    case CN_SPI_SET_CLK:
        __SPI_SetClk(Reg,data1);
        break;

    case CN_SPI_CS_CONFIG:
//      __SPI_Config(Reg,(tagSpiConfig *)data1);    //�ж���Ƭѡ����������Ҫ
        break;

    case CN_SPI_SET_AUTO_CS_EN:
        // Ӳ����������Զ�����CS������
        break;

    case CN_SPI_SET_AUTO_CS_DIS:
        break;

    case CN_SPI_SET_POLL:
        break;

    case CN_SPI_SET_INT:
        break;
    default:        break;
    }
    return result;
}

// =============================================================================
// ���ܣ���SPI���ռĴ�������ѯ��ʽ,��__SPI_TxRxPoll����
// ������Reg,�Ĵ�����ַ
// ���أ�����������
// =============================================================================
static u8 __SPI_Read(struct tagSpiReg * Reg )
{
    u32 time = CN_TIMEOUT;
    while(((Reg->SPI_FSR & 0xff) == 0) && (time > 0))
    {
        time --;
        DJY_DelayUs(1);
    }
    if(time == 0)
    {
        *(volatile u8*)&(Reg->SPI_RXD);
        return -1;
    }
    else
        return  *(volatile u8*)&(Reg->SPI_RXD);
}

// =============================================================================
// ���ܣ�дSPI���ͼĴ�������ѯ��ʽ,��__SPI_TxRxPoll����
// ������Reg,�Ĵ�����ַ��wData��д����
// ���أ���
// =============================================================================
static s32 __SPI_Write(struct tagSpiReg * Reg,u8 wData )
{
    u32 time = CN_TIMEOUT;
    while((((Reg->SPI_FSR>>16) & 0xff) >= 64) && (time > 0))
    {
        time --;
        DJY_DelayUs(1);
    }
    if(time == 0)
        return -1;
    *(u8*)&(Reg->SPI_TXD) = wData;
    return 0;
}

// =============================================================================
// ���ܣ���ѯ��ʽ��д����
// ������Reg,�Ĵ�����ַ��srcAddr,����Դָ�룻wrSize��д��������destAddr������������
//       ��ַָ�룻rdSize������������recvoff,��ƫ��
// ���أ�true,��ȷ��false,����
// =============================================================================
static bool_t __SPI_TxRxPoll(u32 SPI,u8 *srcAddr,u32 wrSize,
        u8 *destAddr, u32 rdSize,u32 recvoff,u8 cs)
{
    u32 i,len_limit;
    volatile u32 k;
    u8 r;
    atom_low_t si;
    struct tagSpiReg *Reg = (struct tagSpiReg *)spi_get_addr(SPI);
    if( (!srcAddr) || ((rdSize != 0) && (!destAddr)))
        return false;
    len_limit = MAX(wrSize, rdSize + recvoff);

    Reg->SPI_MBC = len_limit;
    Reg->SPI_MTC = wrSize;
    Reg->SPI_BCC = wrSize;
    Reg->SPI_TCR |= SPI_CTL_XCH;
    for(i = 0; i < len_limit; i++)
    {
        if((k = i) < wrSize)
        {
//          if((Reg->SPI_BCC==0))
//              printf("spi send fifo pre error\r\n");
            __SPI_Write(Reg, srcAddr[i]);
        }
//        else if((k = i) < wrSize)
//        {
////          if((Reg->SPI_BCC==0))
////              printf("spi send fifo pre error\r\n");
//            __SPI_Write(Reg, srcAddr[i]);
//        }
//���ú���SPI_MBC��SPI_MTC��SPI_BCC���������豸ʱ����д txd �Ĵ�����CPU���Զ�������ʱ��
//      else
//      {
//          __SPI_Write(Reg, 0);
//      }
        r = __SPI_Read(Reg); //��λ����������һ�������fifo�������Ƿ���Ҫ�������������
        if((i >= recvoff) && (i < (recvoff + rdSize)))
        {
            destAddr[i - recvoff] = r;
        }
//        else if((i >= recvoff) && (i < (recvoff + rdSize)))
//        {
//            destAddr[i - recvoff] = r;
//        }
//        extern u32 disint,enint;
//        si = Int_LowAtomStart();
//        if(!len_limit)
//            disint++;
//        Int_LowAtomEnd(si);
//        if(!len_limit)
//            enint++;
        if((Reg->SPI_FSR & SPI_FIFO_TXCNT) && (Reg->SPI_BCC==0))
            printf("spi send fifo error\r\n");
    }
extern u32 fileoffset,errspi;
static u32 wordoffset,resume = 100000,loop;
    if(rdSize == 0x1000)
    {
        if(fileoffset != resume)
        {
            resume = fileoffset;
            wordoffset = 0;
        }
        for(loop = 0;loop < 0x400; loop++)
        {
            if(((u32*)destAddr)[loop] != fileoffset*0x4000+wordoffset)
            {
                errspi++;
            }
            wordoffset++;
        }
    }
//    for (wr=0;((wr<wrSize)&&(wrSize != 0))||((rd < (recvoff+rdSize))&&(rdSize != 0)); )
//    {
//        if ((wr + 4 < wrSize) && (fiforoom < 60))
//        {
//            //srcAddr���ܲ��Ƕ���ĵ�ַ
//            Reg->SPI_TXD = srcAddr[wr] + (srcAddr[wr+1]<<8) + (srcAddr[wr+2]<<16) + (srcAddr[wr+3]<<24);
//            wr +=4;
//        }
//        else if((wr < wrSize) && (fiforoom < 64))
//        {
//            *(u8*)&(Reg->SPI_TXD) = srcAddr[wr];
//            wr++;
//        }
////���ú���SPI_MBC��SPI_MTC��SPI_BCC�����豸ʱ����д txd �Ĵ�����CPU���Զ�������ʱ��
////      else if((i>=wrSize)&&(i<len_limit))
////      {
////          Reg->SPI_TXD = 0;
////      }
//        fiforoom = (Reg->SPI_FSR) & 0xff;   //����fifo������
//        for(j = 0; j< fiforoom; j++)
//        {
//            if((rd < recvoff) || (rd > recvoff + rdSize))
//            {
//                *(volatile u8*)&(Reg->SPI_RXD);
//            }
//            else
//            {
//                destAddr[rd - recvoff] = *(volatile u8*)&(Reg->SPI_RXD);
//            }
//            rd++;
//        }
//    }
//
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
static bool_t __SPI_TransferTxRx(u32 SPI,u32 sendlen,u32 recvlen,
                                u32 recvoff)
{
    struct SPI_IntParamSet *Param=NULL;
    struct tagSpiReg *Reg = (struct tagSpiReg *)spi_get_addr(SPI);

    Param->SendDataLen = sendlen;
    Param->RecvDataLen = recvlen;
    Param->RecvOffset  = recvoff;

    __SPI_IntEnable(Reg,SPI_INTEN_TR);
    Reg->SPI_TCR |= SPI_CTL_XCH;

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
u32 SPI_ISRFUNC(ptu32_t IntLine)
{
#if 0
    struct SPI_CB *pSCB=NULL;
    struct SPI_IntParamSet *param;
    tagSpiReg *Reg;
    u32 spi_sr,spi_ier;
    u8 ch,temp;
    static u8 firstrcv = 0;

    if(IntLine == CN_INT_LINE_SPI0)
    {
        pSCB = s_ptSpi1_CB;
        Reg = (tagSpiReg *)tg_SpiReg[CN_SPI1];
        param = &IntParamset1;
    }
    else if(IntLine == CN_INT_LINE_SPI1)
    {
        pSCB = s_ptSpi2_CB;
        Reg = (tagSpiReg *)tg_SpiReg[CN_SPI2];
        param = &IntParamset2;
    }
    else
        return 0;

    spi_sr = Reg->SR;
    spi_ier = Reg->IER;
    //�����ж�
    if( (spi_ier & SPI_IER_TXPIE) && (spi_sr & SPI_SR_TXP)) //ʹ���˷����жϣ���TXRIS
    {
        temp = SPI_PortRead(pSCB,&ch,1);//�Ƿ���������Ҫ����

        if(temp >0)
        {
            param->SendDataLen --;
            param->RecvOffset  --;
            *((__IO u8 *)&Reg->TXDR) = ch;
        }
        else                                            //�������ͽ���
        {
            //�ط����ж�
            __SPI_IntDisable(Reg,SPI_INTEN_TR);
            param->RecvOffset -= param->SendDataLen;
            param->SendDataLen = 0;

            ch = *((__IO u8 *)&Reg->RXDR);

            //��������ˣ��ж��Ƿ���Ҫ����
            if(param->RecvDataLen > 0)
            {
                //MASTERģʽ�£����ͺͽ���ͬʱ����
                *((__IO u8 *)&Reg->TXDR) = 0xAA;
                __SPI_IntEnable(Reg,SPI_INTEN_RR);    //ʹ�ܽ����ж�
                firstrcv = 1;
            }
        }
    }
    else if((spi_ier & SPI_IER_RXPIE) && (spi_sr & SPI_SR_RXP))    //�����ж�
    {
        ch =*((__IO u8 *)&Reg->SPI_RXD);
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
            *((__IO u8 *)&Reg->TXDR) = 0xAA;
        }
        else                    //���������Ѿ����
        {
            param->RecvDataLen = 0;
            __SPI_IntDisable(Reg,SPI_INTEN_RR);
        }
    }

    if(param->RecvDataLen + param->SendDataLen == 0)
    {
        Reg->CR1 &= ~SPI_CR1_SPE;
    }

    return 0;
#endif
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
    Int_IsrConnect(IntLine,SPI_ISRFUNC);
    Int_SettoAsynSignal(IntLine);
    Int_ClearLine(IntLine);
    Int_RestoreAsynLine(IntLine);
}
// =============================================================================
// ���ܣ� ������˳��͹���ģʽ
//     ʱ��ʹ�� ������ʹ�� GPIO������Ӧ��״̬
// ������ port ���ںţ��� CN_SPI1
//     flag ����͹��� ���˳��͹��ı�־���� SPI_IN_LOWPOWER
// ���أ�true/false
// =============================================================================

bool_t SPI_LowPowerConfig(u8 port,u8 flag)
{
    if(port>=CN_SPI_NUM)
        return false;
    if(flag == InLowPower)
    {
        SPI_InLowPowerPinCfg(port);
        return true;
    }
    else if (flag == OutLowPower)
    {
        SPI_OutLowPowerPinCfg(port);
        return true;
    }
   return false;
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
    case CN_SPI0:
        SPI_Config.BusName          = "SPI0";
        SPI_Config.SPIBuf           = (u8*)&s_Spi0_Buf;
        IntLine = CN_INT_LINE_SPI0;
        pSpiCB = &s_ptSpi0_CB;
        break;
    case CN_SPI1:
        SPI_Config.BusName          = "SPI1";
        SPI_Config.SPIBuf           = (u8*)&s_Spi1_Buf;
        IntLine = CN_INT_LINE_SPI1;
        pSpiCB = &s_ptSpi1_CB;
        break;
    default:
        return (0);
    }
    SPI_Config.SpecificFlag     = port;
    SPI_Config.SPIBufLen        = CFG_SPI_BUF_LEN;
    SPI_Config.MultiCSRegFlag   = false;
    SPI_Config.pTransferPoll    = (TransferPoll)__SPI_TxRxPoll;
    SPI_Config.pTransferTxRx    = (TransferFunc)__SPI_TransferTxRx;
    SPI_Config.pCsActive        = (CsActiveFunc)__SPI_BusCsActive;
    SPI_Config.pCsInActive      = (CsInActiveFunc)__SPI_BusCsInActive;
    SPI_Config.pBusCtrl         = (SPIBusCtrlFunc)__SPI_BusCtrl;

    spi_init((u32)SPI_Config.SpecificFlag); // SPI�Ĵ�������

    __SPI_IntConfig(IntLine); // ע���ж�
    *pSpiCB = SPI_BusAdd(&SPI_Config);
    if(*pSpiCB) // ��SPI�˿���ӵ�SPI����
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
    if(0 == SPI_Initialize(Port))
        return (-1);
    return (0);
}


