/*
 * silan_gpio.h
 *
 * Modify Date: 
 */
#ifndef __SILAN_GPIO_H__
#define __SILAN_GPIO_H__

#include "silan_gpio_regs.h"

#define GPIO1			 0
#define GPIO2            1

#define GPIO1_MAX_NUM    22
#define GPIO2_MAX_NUM    10

#define IO_MAX_NUM       41

#define IO_INPUT		 0
#define IO_OUTPUT 		 1

/*
 *silan_gpio_input
 *addr  = SILAN_GPIO1_BASE or SILAN_GPIO2_BASE
 *pin   = GPIO_PIN(n)
 *example:
    silan_gpio_input(SILAN_GPIO1_BASE, GPIO_PIN(10));//gpio1_10 set input
 */
extern void silan_gpio_input(u32 addr, u32 pin);

/*
 *silan_gpio_output
 *addr  = SILAN_GPIO1_BASE or SILAN_GPIO2_BASE
 *pin   = GPIO_PIN(n)
 *example:
    silan_gpio_output(SILAN_GPIO1_BASE, GPIO_PIN(10));//gpio1_10 set output
 */
extern void silan_gpio_output(u32 addr, u32 pin);

/*
 *silan_gpio_set_high
 *addr  = SILAN_GPIO1_BASE or SILAN_GPIO2_BASE
 *pin   = GPIO_PIN(n)
 *example:
    silan_gpio_set_high(SILAN_GPIO1_BASE, GPIO_PIN(10));//gpio1_10 set high
 */
extern void silan_gpio_set_high(u32 addr, u32 pin);

/*
 *silan_gpio_set_low
 *addr  = SILAN_GPIO1_BASE or SILAN_GPIO2_BASE
 *pin   = GPIO_PIN(n)
 *example:
    silan_gpio_output(SILAN_GPIO1_BASE, GPIO_PIN(10));//gpio1_10 set low
 */
extern void silan_gpio_set_low(u32 addr, u32 pin);

/*
 *silan_gpio_get
 *addr  = SILAN_GPIO1_BASE or SILAN_GPIO2_BASE
 *pin   = GPIO_PIN(n)
 *example:
    tmp = silan_gpio_get(SILAN_GPIO1_BASE, GPIO_PIN(10));//get gpio1_10
 */
extern int silan_gpio_get(u32 addr, u32 pin);

/*
 *silan_gpio_init
 *example:
    silan_gpio_init();
 */
extern void silan_gpio_init(void);

inline void silan_gpio_irq_enable(u32 addr, u32 pin);

void silan_io_input(int io);
void silan_io_output(int io);
void silan_io_direction(int io, int dir);
void silan_io_set_high(int io);
void silan_io_set_low(int io);
void silan_io_set_value(int io, int val);
int silan_io_get(int io);
void silan_io_toggle(int io);
void silan_io_irq_enable(int io);
void silan_io_irq_clear(int io);
void silan_io_irq_mode(int io, int mode);

#endif // __SILAN_GPIO_H__
