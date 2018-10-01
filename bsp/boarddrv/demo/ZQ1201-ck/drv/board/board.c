#include "csi_core.h"
#include "cpu_peri.h"

#define LED1        IO_CONFIG_PB8
#define LED2        IO_CONFIG_PA6

static void Board_GpioInit(void)
{
    silan_io_driver_config(LED1, IO_ATTR_DS_8mA);
    silan_io_func_config(LED1, IO_FUNC_GPIO);
    silan_io_output(LED1);
    silan_io_set_low(LED1);
    
    silan_io_driver_config(LED2, IO_ATTR_DS_8mA);
    silan_io_func_config(LED2, IO_FUNC_GPIO);
    silan_io_output(LED2);
    silan_io_set_low(LED2);
}

void Board_Init(void)
{
//  csi_cache_set_range(0, 0x00000000, CACHE_CRCR_4M, 1);
//  csi_cache_set_range(1, 0x22000000, CACHE_CRCR_8M, 1);
//  csi_dcache_enable();
//  csi_icache_enable();
    Board_GpioInit();
}

