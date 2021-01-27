#include <systime.h>
static inline mp_uint_t mp_hal_ticks_ms(void) {
    return DJY_GetSysTime() / 1000;
}
extern int mp_interrupt_char;
static inline void mp_hal_set_interrupt_char(char c) {
    mp_interrupt_char = c;
}
