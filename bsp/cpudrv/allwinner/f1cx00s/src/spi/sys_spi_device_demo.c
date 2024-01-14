#if 0
#include "sys_spi.h"
#include "sys_uart.h"
#include "sys_delay.h"
#include "sys_timer.h"



#ifdef SPI_Master_Slave_TESE
/****************************************************
SPI����ģʽ����-֧��CPUģʽ-DMAģʽ
 ���鿪����һ��������������һ�����شӻ������໥ͨ�ţ�
 ���Ӷ�ʹ��SPI1,��5��:
 PE7<->PE7,PE8<->PE8,PE9<->PE9,PE10<->PE10,GND<->GND,
 ����ʹ��PE3,PE4�ӵİ�������ָ��ӻ����յ���ָͬ��ز�ͬ�����ݣ�
 ���������ص�����Ϣ��
 ���豸ģʽ+DMA������ʱһ��������ݲ��ܴ���128KB,RT_DATA_LEN<=128KB��

****************************************************/
#include "string.h"
//dmaʱRT_DATA_LEN������128KB
#define RT_DATA_LEN 65535 //���������շ���С����-��Ҫ��ӻ�Ҫ������ͬ-��Ȼ�������ݻ����

__align(0x4) unsigned char Master_rxbuf[RT_DATA_LEN];
__align(0x4) unsigned char Master_txbuf[RT_DATA_LEN];
//����ģʽ�շ�����
void spi_Master_mode_test(void)
{
int SPI=SPI1;
int txf=0;//���ͱ�־
#if SPI_DMA_EN
    DMA TXdma;
    DMA RXdma;
  memset(&RXdma,0,sizeof(RXdma));
  memset(&TXdma,0,sizeof(TXdma));
    SPI_TXdma=&TXdma;
    SPI_RXdma=&RXdma;
#endif
    sysprintf("spi_Master_mode_test()\r\n");
    set_max_clk(20000000);
    spi_init(SPI);
#if SPI_DMA_EN
  /*------DMA��ʼ��------*/
    DMA_Init();
#endif

#define KEY1_GPIO GPIOE
#define KEY2_GPIO GPIOE
#define KEY1_PIN GPIO_Pin_3
#define KEY2_PIN GPIO_Pin_4

  GPIO_Congif(KEY1_GPIO,KEY1_PIN,GPIO_Mode_IN,GPIO_PuPd_UP);
  GPIO_Congif(KEY2_GPIO,KEY2_PIN,GPIO_Mode_IN,GPIO_PuPd_UP);
  memset(Master_txbuf,0,sizeof(Master_txbuf));
  while(1)
  {
  KOUT:
    //����ֵ
    if(GPIO_READ(KEY1_GPIO,KEY1_PIN)==0)
    {
      delay_ms(20);
      if(GPIO_READ(KEY1_GPIO,KEY1_PIN)==1)goto KOUT;
      while(GPIO_READ(KEY1_GPIO,KEY1_PIN)==0);
      Master_txbuf[0]='C';Master_txbuf[1]='M';Master_txbuf[2]='D';Master_txbuf[3]='0';
      txf=1;
    }
    if(GPIO_READ(KEY2_GPIO,KEY2_PIN)==0)
    {
      delay_ms(20);
      if(GPIO_READ(KEY2_GPIO,KEY2_PIN)==1)goto KOUT;
      while(GPIO_READ(KEY2_GPIO,KEY2_PIN)==0);
      Master_txbuf[0]='C';Master_txbuf[1]='M';Master_txbuf[2]='D';Master_txbuf[3]='1';
      txf=1;
    }
        //����
        if(txf==1)
        {
            txf=0;
            sys_spi_select(SPI);
            if(sys_spi_transfer(SPI,Master_txbuf, Master_rxbuf, 4) == 4)//����ָ��
            {
                memset(Master_rxbuf,0,sizeof(Master_rxbuf));
                memset(Master_txbuf,0,sizeof(Master_txbuf));
                delay_ms(1);//������Ҫ�ȴ��ӻ�׼���÷�������
                if(sys_spi_transfer(SPI,Master_txbuf, Master_rxbuf, RT_DATA_LEN) == RT_DATA_LEN)//��������
                {
                    sysprintf("RX:");
                    for(int i=0;i<8;i++)sysprintf("%02x,",Master_rxbuf[i]);
                    sysprintf("\r\n");
                    sysprintf("%s\r\n",Master_rxbuf);
                  //��У���
                    int sum=0;
                    for(int i=0;i<(sizeof(Master_rxbuf)-1);i++)sum+=Master_rxbuf[i];
                    if(Master_rxbuf[sizeof(Master_rxbuf)-1]==(sum&0xff))
                    {
                        sysprintf("checksum ok.\r\n");
                    }else sysprintf("checksum fail.\r\n");
                  //---
                }
            }
            sys_spi_deselect(SPI);
        }
    }
}
#include "sys_timer.h"
__align(0x4) unsigned char slave_rxbuf[RT_DATA_LEN];
__align(0x4) unsigned char slave_txbuf[RT_DATA_LEN];
__align(0x4) unsigned char slave_txbuf1[RT_DATA_LEN]="CMD0 123";
__align(0x4) unsigned char slave_txbuf2[RT_DATA_LEN]="CMD1 456";
//�豸ģʽ�շ�����
void spi_slave_mode_test(void)
{
unsigned int time=0;
int SPI=SPI1;

#if SPI_DMA_EN
    DMA TXdma;
    DMA RXdma;
  memset(&RXdma,0,sizeof(RXdma));
  memset(&TXdma,0,sizeof(TXdma));
    SPI_TXdma=&TXdma;
    SPI_RXdma=&RXdma;
#endif
    //��ʱ��
    Sys_Timer1_Init();
    time=get_time_ms();
    //----------
    sysprintf("spi_slave_mode_test\r\n");
    spi_set_slave_mode();
    spi_init(SPI);
#if SPI_DMA_EN
  /*------DMA��ʼ��------*/
    DMA_Init();
#endif

//��ʼ�����ݻ���
    memset(slave_txbuf,0,sizeof(slave_txbuf));
    memset(slave_rxbuf,0,sizeof(slave_rxbuf));
    for(int i=0;i<(sizeof(slave_txbuf1)-8-1);i++)slave_txbuf1[8+i]=i&0xff;
    for(int i=0;i<(sizeof(slave_txbuf2)-8-1);i++)slave_txbuf2[8+i]=i&0xff;
//��У���
    int sum1=0,sum2=0;
    for(int i=0;i<(sizeof(slave_txbuf1)-1);i++)sum1+=slave_txbuf1[i];
    for(int i=0;i<(sizeof(slave_txbuf2)-1);i++)sum2+=slave_txbuf2[i];
    slave_txbuf1[sizeof(slave_txbuf1)-1]=sum1&0xff;
    slave_txbuf2[sizeof(slave_txbuf2)-1]=sum2&0xff;
//---
    while(1)
    {
        /*���ջ���������*/
        if(read_rx_count(SPI)>0)
        {
            if(sys_spi_transfer(SPI,NULL,slave_rxbuf,4)==4)//ʹ��read_rx_count�ж�ʱ��tx�������ΪNULL
            {
                /*���յ�ָ��*/
                if((slave_rxbuf[0]=='C')&&(slave_rxbuf[1]=='M')&&(slave_rxbuf[2]=='D'))//
                {
                    switch(slave_rxbuf[3])
                    {
                        case '0': //ָ��1-��������1
                            if(sys_spi_transfer(SPI,slave_txbuf1,slave_rxbuf,RT_DATA_LEN) == RT_DATA_LEN)
                            {
                                sysprintf("CMD0 OK\r\n");
                            }
                            break;
                        case '1': //ָ��2-��������2
                            if(sys_spi_transfer(SPI,slave_txbuf2,slave_rxbuf,RT_DATA_LEN) == RT_DATA_LEN)
                            {
                                sysprintf("CMD1 OK\r\n");
                            }
                            break;
                    }
                }
            }
          //��ʼ�����ݻ���
            memset(slave_txbuf,0,sizeof(slave_txbuf));
            memset(slave_rxbuf,0,sizeof(slave_rxbuf));
        }
        //�����¼�
        if((get_time_ms()-time)>1000)
        {
            time=get_time_ms();
            sysprintf("RUN\r\n");
        }
    }
}
#endif


#endif
