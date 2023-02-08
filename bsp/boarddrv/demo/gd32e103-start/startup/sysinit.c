#include <gd32e10x.h>

// =============================================================================
// 功能：该函数实现系统时钟的初始化，主要包括：
// 1、系统时钟从内部时钟切换到外部时钟；
// 2、配置 HCLK、PCLK1、PCLK2、MCLK分频系数；
// 3、使能数据和指令cache；
// 4、选用MCLK为系统时钟
// 本函数的时钟设置，必须与board-config.h中的CN_CFG_MCLK等常量定义一致。
// 参数：无
// 返回：无
// =============================================================================
void SysClockInit(void)
{
/* SystemInit(), GD32E103E-START xtal 25MHz
 * djysrc/third/firmware/gd32e10x/CMSIS/GD/GD32E10x/Source/system_gd32e10x.c
 */
	SystemInit();
}

void SRAM_Init(void)
{
	/* we don't have external sram */
}
