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

#define CN_UART_NUM 5

ptu32_t ModuleInstall_UART(ptu32_t para);

#ifdef __cplusplus
}
#endif

#endif // __CPU_PERI_UART_H__

