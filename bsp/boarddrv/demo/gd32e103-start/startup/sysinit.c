#include <gd32e10x.h>

// =============================================================================
// ���ܣ��ú���ʵ��ϵͳʱ�ӵĳ�ʼ������Ҫ������
// 1��ϵͳʱ�Ӵ��ڲ�ʱ���л����ⲿʱ�ӣ�
// 2������ HCLK��PCLK1��PCLK2��MCLK��Ƶϵ����
// 3��ʹ�����ݺ�ָ��cache��
// 4��ѡ��MCLKΪϵͳʱ��
// ��������ʱ�����ã�������board-config.h�е�CN_CFG_MCLK�ȳ�������һ�¡�
// ��������
// ���أ���
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

