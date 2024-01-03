#ifndef __CPU_PERI_UART_H__
#define __CPU_PERI_UART_H__

#ifdef __cplusplus
extern "C" {
#endif



#define CN_UART1    0
#define CN_UART2    1
#define CN_UART3    2
#define CN_UART4    3
#define CN_UART5    4
#define CN_UART6    5
#define CN_UART7    6
#define CN_UART8    7
#define CN_UART_NUM 8


enum UART_DMA_CHANNEL
{
    DMA1_C1 = 0,
    DMA1_C2,
    DMA1_C3,
    DMA1_C4,
    DMA1_C5,
    DMA1_C6,
    DMA1_C7,
    DMA2_C1,
    DMA2_C2,
    DMA2_C3,
    DMA2_C4,
    // DMA2_C5,
    // DMA2_C6,
    // DMA2_C7,
    DMA_NONUSE
};


ptu32_t ModuleInstall_UART(ptu32_t para);

#ifdef __cplusplus
}
#endif

#endif // __CPU_PERI_UART_H__
