#if 0
#include "sys_spi.h"
#include "sys_uart.h"
#include "sys_delay.h"
#include "sys_timer.h"



#ifdef SPI_Master_Slave_TESE
/****************************************************
SPI主从模式测试-支持CPU模式-DMA模式
 两块开发板一个下载主机程序一个下载从机程序，相互通信，
 主从都使用SPI1,共5线:
 PE7<->PE7,PE8<->PE8,PE9<->PE9,PE10<->PE10,GND<->GND,
 主机使用PE3,PE4接的按键发送指令，从机接收到不同指令返回不同的数据，
 串口输出相关调试信息。
 【设备模式+DMA】传输时一次最大数据不能大于128KB,RT_DATA_LEN<=128KB。

****************************************************/
#include "string.h"
//dma时RT_DATA_LEN不大于128KB
#define RT_DATA_LEN 65535 //主从数据收发大小定义-主要与从机要定义相同-不然接收数据会出错

__align(0x4) unsigned char Master_rxbuf[RT_DATA_LEN];
__align(0x4) unsigned char Master_txbuf[RT_DATA_LEN];
//主机模式收发数据
void spi_Master_mode_test(void)
{
int SPI=SPI1;
int txf=0;//发送标志
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
  /*------DMA初始化------*/
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
    //读键值
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
        //发送
        if(txf==1)
        {
            txf=0;
            sys_spi_select(SPI);
            if(sys_spi_transfer(SPI,Master_txbuf, Master_rxbuf, 4) == 4)//发送指令
            {
                memset(Master_rxbuf,0,sizeof(Master_rxbuf));
                memset(Master_txbuf,0,sizeof(Master_txbuf));
                delay_ms(1);//这里需要等待从机准备好发送数据
                if(sys_spi_transfer(SPI,Master_txbuf, Master_rxbuf, RT_DATA_LEN) == RT_DATA_LEN)//接收数据
                {
                    sysprintf("RX:");
                    for(int i=0;i<8;i++)sysprintf("%02x,",Master_rxbuf[i]);
                    sysprintf("\r\n");
                    sysprintf("%s\r\n",Master_rxbuf);
                  //求校验和
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
//设备模式收发数据
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
    //定时器
    Sys_Timer1_Init();
    time=get_time_ms();
    //----------
    sysprintf("spi_slave_mode_test\r\n");
    spi_set_slave_mode();
    spi_init(SPI);
#if SPI_DMA_EN
  /*------DMA初始化------*/
    DMA_Init();
#endif

//初始化数据缓存
    memset(slave_txbuf,0,sizeof(slave_txbuf));
    memset(slave_rxbuf,0,sizeof(slave_rxbuf));
    for(int i=0;i<(sizeof(slave_txbuf1)-8-1);i++)slave_txbuf1[8+i]=i&0xff;
    for(int i=0;i<(sizeof(slave_txbuf2)-8-1);i++)slave_txbuf2[8+i]=i&0xff;
//求校验和
    int sum1=0,sum2=0;
    for(int i=0;i<(sizeof(slave_txbuf1)-1);i++)sum1+=slave_txbuf1[i];
    for(int i=0;i<(sizeof(slave_txbuf2)-1);i++)sum2+=slave_txbuf2[i];
    slave_txbuf1[sizeof(slave_txbuf1)-1]=sum1&0xff;
    slave_txbuf2[sizeof(slave_txbuf2)-1]=sum2&0xff;
//---
    while(1)
    {
        /*接收缓存有数据*/
        if(read_rx_count(SPI)>0)
        {
            if(sys_spi_transfer(SPI,NULL,slave_rxbuf,4)==4)//使用read_rx_count判断时，tx缓存必须为NULL
            {
                /*接收到指令*/
                if((slave_rxbuf[0]=='C')&&(slave_rxbuf[1]=='M')&&(slave_rxbuf[2]=='D'))//
                {
                    switch(slave_rxbuf[3])
                    {
                        case '0': //指令1-发送数据1
                            if(sys_spi_transfer(SPI,slave_txbuf1,slave_rxbuf,RT_DATA_LEN) == RT_DATA_LEN)
                            {
                                sysprintf("CMD0 OK\r\n");
                            }
                            break;
                        case '1': //指令2-发送数据2
                            if(sys_spi_transfer(SPI,slave_txbuf2,slave_rxbuf,RT_DATA_LEN) == RT_DATA_LEN)
                            {
                                sysprintf("CMD1 OK\r\n");
                            }
                            break;
                    }
                }
            }
          //初始化数据缓存
            memset(slave_txbuf,0,sizeof(slave_txbuf));
            memset(slave_rxbuf,0,sizeof(slave_rxbuf));
        }
        //其它事件
        if((get_time_ms()-time)>1000)
        {
            time=get_time_ms();
            sysprintf("RUN\r\n");
        }
    }
}
#endif


#endif
