#ifndef __CPU_PERI_UART_H__
#define __CPU_PERI_UART_H__

#ifdef __cplusplus
extern "C" {
#endif

bool_t AT32Timer_Time2Counter(u64 time, u64 *counter);
bool_t AT32Timer_Counter2Time(u64 counter,u64 *time);
bool_t ModuleInstall_HardTimer(void);

#ifdef __cplusplus
}
#endif

#endif // __CPU_PERI_UART_H__
