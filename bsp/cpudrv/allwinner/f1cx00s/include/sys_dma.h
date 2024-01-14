#ifndef __SYS_DMA_H__
#define __SYS_DMA_H__
#include <stdint.h>
typedef struct dmaInit
{
    u32 Source_Address;        //源地址
    u32 Source_Address_Type;   //源地址类型00: Linear Mode    01: IO Mode
    u32 Source_DRQ_Type;       //源类型
    u32 Source_Data_Width;     //源数据宽度
    u32 Source_Burst_Length;   //0: 1   1: 4
    //
    u32 Destination_Address;   //目标地址
    u32 Destination_Address_Type;//目标地址类型00: Linear Mode    01: IO Mode
    u32 Destination_DRQ_Type;  //目标类型
    u32 Destination_Data_Width;//目标数据宽度
    u32 Destination_Burst_Length;//0: 1   1: 4
    //
    u32 Type;                  //NDMA DDMA
    u32 Ch;                    //通道
    u32 addr;                  //寄存器地址
    /*
    Byte_Counter
    Normal DMA(4路) 最大一次传输128KB;
    Dedicated DMA(4路) 最大一次传输0x1000000;
    */
    u32 Byte_Counter;          //Byte计数
    u32 Continuous_Mode_Enable;//连续模式
    u32 Read_Byte_Counter_Enable;//读计数值使能
//
}DMA;

//DMA最大传输长度
#define NDMA_MAX_LEN 0x20000        //128KB
#define DDMA_MAX_LEN 0x1000000      //16MB

#define NDMA 0x100
#define DDMA 0x300
//-----------------------
#define DMA_ADDRESS_TYEP_LINEER             0
#define DMA_ADDRESS_TYEP_IO                 1
//-----------------------
#define DMA_DATA_WIDTH_8                    0
#define DMA_DATA_WIDTH_16                   1
#define DMA_DATA_WIDTH_32                   2
//-----------------------
#define DMA_BURST_LENGTH_1                  0
#define DMA_BURST_LENGTH_4                  1

//-----------------------
#define NDMAS_DRQ_Type_IR_Rx                0x00
#define NDMAS_DRQ_Type_SPI0_Rx              0x04
#define NDMAS_DRQ_Type_SPI1_Rx              0x05
#define NDMAS_DRQ_Type_UART0_Rx             0x08
#define NDMAS_DRQ_Type_UART1_Rx             0x09
#define NDMAS_DRQ_Type_UART2_Rx             0x0A
#define NDMAS_DRQ_Type_Audio_Codec          0x0C
#define NDMAS_DRQ_Type_TP_ADC               0x0D
#define NDMAS_DRQ_Type_Daudio               0x0E
#define NDMAS_DRQ_Type_SRAM_Memory          0x10
#define NDMAS_DRQ_Type_SDRAM_Memory         0x11
#define NDMAS_DRQ_Type_USB                  0x14
#define NDMAS_DRQ_Type_USB_EP               0x15
#define NDMAS_DRQ_Type_USB_EP2              0x16
#define NDMAS_DRQ_Type_USB_EP3              0x17
//-----------------------
#define NDMAD_DRQ_Type_OWA_Tx               0x01
#define NDMAD_DRQ_Type_SPI0_Tx              0x04
#define NDMAD_DRQ_Type_SPI1_Tx              0x05
#define NDMAD_DRQ_Type_UART_Tx              0x08
#define NDMAD_DRQ_Type_UART1_Tx             0x09
#define NDMAD_DRQ_Type_UART2_Tx             0x0A
#define NDMAD_DRQ_Type_Audio_Codec_DAC      0x0C
#define NDMAD_DRQ_Type_Daudio               0x0E
#define NDMAD_DRQ_Type_SRAM_Memory          0x10
#define NDMAD_DRQ_Type_SDRAM_Memory         0x11
#define NDMAD_DRQ_Type_USB                  0x14
#define NDMAD_DRQ_Type_USB_EP1              0x15
#define NDMAD_DRQ_Type_USB_EP2              0x16
#define NDMAD_DRQ_Type_USB_EP3              0x17
//-----------------------

//Dedicated DMA Source DRQ Type
#define DDMAS_DRQ_Type_SRAM                 0x0
#define DDMAS_DRQ_Type_SDRAM_Memory         0x1
#define DDMAS_DRQ_Type_USB                  0x4
#define DDMAS_DRQ_Type_AHB_Memory           0x9
//Dedicated DMA Destination DRQ Type
#define DDMAD_DRQ_Type_SRAM                 0x0
#define DDMAD_DRQ_Type_SDRAM_Memory         0x1
#define DDMAD_DRQ_Type_LCD_Controller       0x2
#define DDMAD_DRQ_Type_USB                  0x4
#define DDMAD_DRQ_Type_AHB_Memory           0x9

#define dmaDisable 0
#define dmaEnable  1

void DDMA_Demo(void);
void NDMA_Demo(void);

int  DMA_Config(DMA* dma);
int  DMA_Get_Full_TIP(DMA* dma);
int  DMA_Get_Half_TIP(DMA* dma);
void DMA_Disable(DMA* dma);
void DMA_Enable(DMA* dma);
void DMA_Init(void);
void DMA_Exit(void);
int DMA_Interrupt_Control_Half(DMA* dma,int c);
int DMA_Interrupt_Control_Full(DMA* dma,int c);
u32 dma_read_bcnt(DMA* dma);


#endif

