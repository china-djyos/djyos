//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
// =============================================================================
#include <typedef.h>
#include "cpu_peri.h"
#include <device/include/uart.h>
#include "stdlib.h"
#include "gpio_pub.h"
#include "icu_pub.h"
#include "i2s/i2s.h"
#include "dbug.h"
#include "i2s_pub.h"
#include <arm_arch.h>
#include <general_dma_pub.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern int ModuleInstall_I2S(void);
//    ModuleInstall_I2S();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip i2s"//
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"             //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_I2S == false )
//#warning  " myname �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_I2S    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,262144,
#define CFG_I2S_RX_BUFFER_SIZE         8192       //���ջ��λ�������С��
#define CFG_I2S_TX_BUFFER_SIZE         8192       //���ͻ��λ�������С��
//%$#@enum,GDMA_CHANNEL_0,GDMA_CHANNEL_1,GDMA_CHANNEL_2,GDMA_CHANNEL_3,GDMA_CHANNEL_4,GDMA_CHANNEL_5,GDMA_CHANNEL_MAX
#define CFG_I2S_RX_DMA_CHANNEL          GDMA_CHANNEL_4         //DMAͨ����ֵԽ���ͨ�������ȼ�Խ��
#define CFG_I2S_TX_DMA_CHANNEL          GDMA_CHANNEL_3         //DMAͨ����ֵԽ���ͨ�������ȼ�Խ�ߡ�
//%$#@enum,I2S_MODE,I2S_LEFT_JUSTIFIED,I2S_RIGHT_JUSTIFIED,I2S_RESERVE,I2S_SHORT_FRAME_SYNC,I2S_LONG_FRAME_SYNC,I2S_NORMAL_2B_D,I2S_DELAY_2B_D
#define CFG_I2S_MODE                    I2S_MODE               //I2S�Ĺ���ģʽ��
//%$#@enum,I2S_LRCK_NO_TURN,I2S_LRCK_TURN
#define CFG_I2S_LRCK_SET                I2S_LRCK_NO_TURN       //I2S_LRCK_NO_TURN���͵�ƽ��ʾ���������ߵ�ƽ��ʾ��������I2S_LRCK_TURN�෴
//%$#@enum,I2S_SCK_NO_TURN,I2S_SCK_TURN
#define CFG_I2S_SCK_SET                 I2S_SCK_NO_TURN        //I2S_SCK_NO_TURN: SCLK�������ز��������������SCLK���½��ر仯.I2S_SCK_TURN�෴
//%$#@enum,I2S_MSB_FIRST,I2S_LSB_FIRST
#define CFG_I2S_MSB_LSB_SET             I2S_MSB_FIRST
//%$#@num,0,7,
#define CFG_I2S_SYNCLEN         0       //Sync���ȣ���Long Frame Syncģʽ����Ч����1~7�ֱ��Ӧ2~8��Bit Clock Cycle
#define CFG_I2S_PCM_DLEN        0       //2B+D��PCMģʽ��D�ĳ��ȣ���λ��SCK����
//%$#@select
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


struct IIS_Device g_i2s;

//#define I2S_RX_BUFFER_SIZE     8192
//#define I2S_TX_BUFFER_SIZE     8192
//#define I2S_RX_DMA_CHANNEL     GDMA_CHANNEL_4       //DMAͨ����ֵԽ���ͨ�������ȼ�Խ��
//#define I2S_TX_DMA_CHANNEL     GDMA_CHANNEL_3

#define I2S_DEFAULT_MODE (CFG_I2S_MODE| CFG_I2S_LRCK_SET| CFG_I2S_SCK_SET| CFG_I2S_MSB_LSB_SET| (CFG_I2S_SYNCLEN<<I2S_SYNC_LENGTH_BIT)| (CFG_I2S_PCM_DLEN<<I2S_PCM_DATA_LENGTH_BIT))

bool_t I2S_RX_CALLBACK = false;       //�Ƿ�ע����I2S�Ľ��ջص�����
bool_t I2S_TX_CALLBACK = false;       //�Ƿ�ע����I2S�ķ��ͻص�����

//����ֵ 0-�ɹ�������-ʧ��
static s32 IIS_SetTX_DMA_PauseAddr(UINT32 addr)
{
    GDMA_CFG_ST en_cfg;
    s32 ret = 0;

    memset(&en_cfg, 0, sizeof(GDMA_CFG_ST));

    en_cfg.channel = CFG_I2S_TX_DMA_CHANNEL;
    en_cfg.param = addr;
    ret = sddev_control(GDMA_DEV_NAME, CMD_GDMA_SET_SRC_PAUSE_ADDR, &en_cfg);
    return ret;
}
//����ֵ 0-�ɹ�������-ʧ��
static s32 IIS_SetRX_DMA_PauseAddr(UINT32 addr)
{
    GDMA_CFG_ST en_cfg;
    s32 ret = 0;

    memset(&en_cfg, 0, sizeof(GDMA_CFG_ST));

    en_cfg.channel = CFG_I2S_RX_DMA_CHANNEL;
    en_cfg.param = addr;
    ret = sddev_control(GDMA_DEV_NAME, CMD_GDMA_SET_DST_PAUSE_ADDR, &en_cfg);
    return ret;
}
// =============================================================================
// ���ܣ�I2S DMA����ģʽ����
// ������mode - 1����ģʽ��0����ģʽ
// ���أ�0-�ɹ�������-ʧ��
// =============================================================================
static s32 IIS_DMA_SetMasterSlave(u32 mode)
{
    s32 ret = 0;
    ret = sddev_control(I2S_DEV_NAME, I2S_CMD_DMA_MASTER_ENABLE, (void *)&mode);
    return ret;
}

// =============================================================================
// ���ܣ�I2S ʹ��
// ������enable - �Ƿ�ʹ�ܣ�1��ʹ�ܣ�0��ʧ��
// ���أ�0-�ɹ�������-ʧ��
// =============================================================================
static s32 IIS_Enable(u32 enable)
{
    s32 ret = 0;
    if (enable)
    {
        ret = sddev_control(I2S_DEV_NAME, I2S_CMD_UNIT_ENABLE, NULL);
    }
    else
    {
        ret = sddev_control(I2S_DEV_NAME, I2S_CMD_DISABLE_I2S, NULL);
    }
    return ret;
}

// =============================================================================
// ���ܣ�I2S DMA����ʹ��
// ������enable - �Ƿ�ʹ�ܣ�1��ʹ�ܣ�0��ʧ��
// ���أ�0-�ɹ�������-ʧ��
// =============================================================================
static s32 IIS_DMA_TxEnable(u32 enable)
{
    GDMA_CFG_ST en_cfg;
    s32 ret = 0;
    if(g_i2s.tx_enable == enable)
    {
        notice_printf("IIS","Tx current state is already %d\r\n",enable);
        return -1;
    }

    en_cfg.channel = CFG_I2S_TX_DMA_CHANNEL;
    if (enable)
        en_cfg.param = 1;
    else
        en_cfg.param = 0;
    ret = sddev_control(GDMA_DEV_NAME, CMD_GDMA_SET_DMA_ENABLE, &en_cfg);

    g_i2s.tx_enable = enable ? 1 : 0;

    return ret;
}
// =============================================================================
// ���ܣ�I2S DMA����ʹ��
// ������enable - �Ƿ�ʹ�ܣ�1��ʹ�ܣ�0��ʧ��
// ���أ���
// =============================================================================
static s32 IIS_DMA_RxEnable(u32 enable)
{
    GDMA_CFG_ST en_cfg;
    s32 ret = 0;
    if(g_i2s.rx_enable == enable)
    {
        notice_printf("IIS","Rx current state is already %d\r\n",enable);
        return -1;
    }

    en_cfg.channel = CFG_I2S_RX_DMA_CHANNEL;
    if (enable)
        en_cfg.param = 1;
    else
        en_cfg.param = 0;
    ret = sddev_control(GDMA_DEV_NAME, CMD_GDMA_SET_DMA_ENABLE, &en_cfg);

    g_i2s.rx_enable = enable ? 1 : 0;
    return ret;
}

#if I2S_RX_CALLBACK
void i2s_dma_rx_half_handler(UINT32 flag)
{
    int result;
    struct rt_i2s_bus_device *i2s = &g_i2s_bus;

    rt_kprintf("%s:%d\r\n", __FUNCTION__, __LINE__);
    //rt_kprintf("%s:%d PCM_CTRL=0x%x,PCM_CN=0x%x,PCM_STAT=0x%x\r\n", __FUNCTION__, __LINE__, REG_READ(PCM_CTRL), REG_READ(PCM_CN), REG_READ(PCM_STAT));
#ifdef PAUSE_EN
    //i2s_dma_rx_pause_addr_set((UINT32)i2s.rx_fifo + (CFG_I2S_RX_BUFFER_SIZE -4));
#endif

    int index;
    int *ptr = (int *)i2s.rx_fifo;
    for (index = 0; index < CFG_I2S_RX_BUFFER_SIZE / 8; index++, ptr++)
    {
        if (*ptr != 0)
        {
            rt_kprintf("rx[%d]=0x%x\r\n", index, *ptr);
        }
    }
}

void i2s_dma_rx_finish_handler(UINT32 flag)
{
    int result;
    struct rt_i2s_bus_device *i2s = &g_i2s_bus;

    rt_kprintf("%s:%d\r\n", __FUNCTION__, __LINE__);
    //rt_kprintf("%s:%d PCM_CTRL=0x%x,PCM_CN=0x%x,PCM_STAT=0x%x\r\n", __FUNCTION__, __LINE__, REG_READ(PCM_CTRL), REG_READ(PCM_CN), REG_READ(PCM_STAT));
#ifdef PAUSE_EN
    //i2s_dma_rx_pause_addr_set((UINT32)i2s.rx_fifo + (CFG_I2S_RX_BUFFER_SIZE / 2 -4));
#endif

    int index;
    int *ptr = (int *)i2s.rx_fifo + CFG_I2S_RX_BUFFER_SIZE / 8;
    for (index = CFG_I2S_RX_BUFFER_SIZE / 8; index < CFG_I2S_RX_BUFFER_SIZE / 4; index++, ptr++)
    {
        if (*ptr != 0)
        {
            rt_kprintf("rx[%d]=0x%x\r\n", index, *ptr);
        }
    }
}
#endif
#if I2S_TX_CALLBACK
void i2s_dma_tx_half_handler(UINT32 flag)
{
    int result;
    struct rt_i2s_bus_device *i2s = &g_i2s_bus;

    rt_kprintf("%s:%d\r\n", __FUNCTION__, __LINE__);
    //rt_kprintf("%s:%d PCM_CTRL=0x%x,PCM_CN=0x%x,PCM_STAT=0x%x\r\n", __FUNCTION__, __LINE__, REG_READ(PCM_CTRL), REG_READ(PCM_CN), REG_READ(PCM_STAT));
    result = rt_data_node_is_empty(i2s->tx_list);   //����rtt�Ľӿ������ж��Ƿ���������Ҫ����
    if (result)
    {
        //û��������Ҫ����
#ifdef PAUSE_EN
        i2s_dma_tx_pause_addr_set((UINT32)i2s->tx_fifo + (CFG_I2S_TX_BUFFER_SIZE -4));
        i2s->tx_paused = 1;
        //i2s_dma_master_enable(!i2s->tx_paused);
        i2s->tx_fill_pos = (UINT32)i2s->tx_fifo;
        i2s->tx_fill_size = CFG_I2S_TX_BUFFER_SIZE / 2;
#endif

        memset(i2s->tx_fifo, 0, CFG_I2S_TX_BUFFER_SIZE / 2);
    }
    else
    {
        //��������Ҫ����
        memset(i2s->tx_fifo, 0, CFG_I2S_TX_BUFFER_SIZE / 2);
        result = rt_data_node_read(i2s->tx_list, i2s->tx_fifo, CFG_I2S_TX_BUFFER_SIZE / 2);     //��ȡ��Ҫ���͵����ݣ�rtt�Ľӿ�

#ifdef PAUSE_EN
        if (result < (CFG_I2S_TX_BUFFER_SIZE / 2))
        {
            i2s_dma_tx_pause_addr_set((UINT32)i2s->tx_fifo + (CFG_I2S_TX_BUFFER_SIZE -4));
            i2s->tx_paused = 1;
            //i2s_dma_master_enable(!i2s->tx_paused);
            i2s->tx_fill_pos = (UINT32)i2s->tx_fifo + result;
            i2s->tx_fill_size = CFG_I2S_TX_BUFFER_SIZE / 2 - result;
        }
#endif
    }
}

void i2s_dma_tx_finish_handler(UINT32 flag)
{
    int result;
    struct rt_i2s_bus_device *i2s = &g_i2s_bus;

    rt_kprintf("%s:%d\r\n", __FUNCTION__, __LINE__);
    //rt_kprintf("%s:%d PCM_CTRL=0x%x,PCM_CN=0x%x,PCM_STAT=0x%x\r\n", __FUNCTION__, __LINE__, REG_READ(PCM_CTRL), REG_READ(PCM_CN), REG_READ(PCM_STAT));
    i2s->tx_dma_irq_cnt ++;
    result = rt_data_node_is_empty(i2s->tx_list);   //����rtt�Ľӿ������ж��Ƿ���������Ҫ����
    if (result)
    {
    #ifdef PAUSE_EN
        i2s_dma_tx_pause_addr_set((UINT32)i2s->tx_fifo + (CFG_I2S_TX_BUFFER_SIZE / 2 -4));
        i2s->tx_paused = 1;
        //i2s_dma_master_enable(!i2s->tx_paused);
        i2s->tx_fill_pos = (UINT32)i2s->tx_fifo + CFG_I2S_TX_BUFFER_SIZE / 2;
        i2s->tx_fill_size = CFG_I2S_TX_BUFFER_SIZE / 2;
    #endif
        //rt_kprintf("* ");
        memset(i2s->tx_fifo + (CFG_I2S_TX_BUFFER_SIZE / 2), 0, CFG_I2S_TX_BUFFER_SIZE / 2);
    }
    else
    {
        memset(i2s->tx_fifo + (CFG_I2S_TX_BUFFER_SIZE / 2), 0, CFG_I2S_TX_BUFFER_SIZE / 2);
        result = rt_data_node_read(i2s->tx_list, i2s->tx_fifo + (CFG_I2S_TX_BUFFER_SIZE / 2), CFG_I2S_TX_BUFFER_SIZE / 2);  //��ȡ��Ҫ���͵����ݣ�rtt�Ľӿ�
    #ifdef PAUSE_EN
        if (result < (CFG_I2S_TX_BUFFER_SIZE / 2))
        {
            i2s_dma_tx_pause_addr_set((UINT32)i2s->tx_fifo + (CFG_I2S_TX_BUFFER_SIZE / 2 -4));
            i2s->tx_paused = 1;
            //i2s_dma_master_enable(!i2s->tx_paused);
            i2s->tx_fill_pos = (UINT32)i2s->tx_fifo + CFG_I2S_TX_BUFFER_SIZE / 2 + result;
            i2s->tx_fill_size = CFG_I2S_TX_BUFFER_SIZE / 2 - result;
        }
    #endif
    }
}
#endif

// =============================================================================
// ���ܣ�I2S�豸�������ݳ�ʼ��
// ��������
// ���أ�0=�ɹ���-1=ʧ��
// =============================================================================
s32 IIS_DMA_RxInit(struct IIS_OperParam *param)
{
    GDMACFG_TPYES_ST init_cfg;
    GDMA_CFG_ST en_cfg;
    s32 res = 0;

    memset(&init_cfg, 0, sizeof(GDMACFG_TPYES_ST));
    memset(&en_cfg, 0, sizeof(GDMA_CFG_ST));

    init_cfg.dstdat_width = 32;                     //Ŀ������λ��
    init_cfg.srcdat_width = g_i2s.bits_width;       //Դ����λ��
    init_cfg.dstptr_incr = 1;                       //Ŀ��ָ���ַ����Ϊ����
    init_cfg.srcptr_incr = 0;                       //Դָ���ַ������

    init_cfg.src_start_addr = (void *)PCM_DAT0;     //Դ���ݷ�����ʼ��ַ
    init_cfg.dst_start_addr = g_i2s.rx_fifo;       //Ŀ�����ݽ�����ʼ��ַ

    init_cfg.channel = CFG_I2S_RX_DMA_CHANNEL;
    init_cfg.prio = 0;                              //���ȼ�
    //type1; loop src,    no loop dst,    no register
    //type2; no loop src, loop dst,    no register
    //type3;  loop src,    loop dst,    no register
    //type4; loop src,    no loop dst,    register
    //type5; no loop src, loop dst,    register
    //type6; loop src,    loop dst,    register
    init_cfg.u.type5.dst_loop_start_addr = g_i2s.rx_fifo;                        //Ŀ�껺������ʼ��ַ��
    init_cfg.u.type5.dst_loop_end_addr = g_i2s.rx_fifo + CFG_I2S_RX_BUFFER_SIZE;     //Ŀ�껺����������ַ��

//#if I2S_RX_CALLBACK
    if((param->i2s_rx_half_fin_handler != NULL) && (param->i2s_rx_fin_handler != NULL))
    {
        init_cfg.half_fin_handler = param->i2s_rx_half_fin_handler;     //ע��ص�����
        init_cfg.fin_handler = param->i2s_rx_fin_handler;
        I2S_RX_CALLBACK = true;
    }
    else
        info_printf("IIS","RX not registered callback functions \r\n");
//#endif

    init_cfg.src_module = GDMA_X_SRC_I2S_RX_REQ;        //Դģ��ΪI2S����
    init_cfg.dst_module = GDMA_X_DST_DTCM_WR_REQ;       //Ŀ��ģ��ΪDTCMд

    res = sddev_control(GDMA_DEV_NAME, CMD_GDMA_CFG_TYPE5, (void *)&init_cfg);
    if(res != 0)
    {
        error_printf("IIS"," IIS_DMA_RxInit fail\r\n");
        return -1;
    }
    en_cfg.channel = CFG_I2S_RX_DMA_CHANNEL;
    en_cfg.param = CFG_I2S_RX_BUFFER_SIZE; // dma ���ջ����С
    res = sddev_control(GDMA_DEV_NAME, CMD_GDMA_SET_TRANS_LENGTH, (void *)&en_cfg);
    if(res != 0)
    {
        error_printf("IIS"," IIS_DMA_RxInit fail\r\n");
        return -1;
    }
//#if !I2S_RX_CALLBACK
    if(I2S_RX_CALLBACK == false)
        rb_init_dma_write(&g_i2s.rb_dma_wr, g_i2s.rx_fifo, CFG_I2S_RX_BUFFER_SIZE, CFG_I2S_RX_DMA_CHANNEL);
    else
        info_printf("IIS","RX not init ring buf \r\n");
//#endif

    return 0;
}
// =============================================================================
// ���ܣ�I2S�豸�������ݳ�ʼ��
// ��������
// ���أ�0=�ɹ���-1=ʧ��
// =============================================================================
s32 IIS_DMA_TxInit(struct IIS_OperParam *param)
{
    GDMACFG_TPYES_ST init_cfg;
    GDMA_CFG_ST en_cfg;
    s32 res = 0;

    memset(&init_cfg, 0, sizeof(GDMACFG_TPYES_ST));
    memset(&en_cfg, 0, sizeof(GDMA_CFG_ST));

    init_cfg.dstdat_width = g_i2s.bits_width;       //Ŀ������λ��
    init_cfg.srcdat_width = 32;                     //Դ����λ��
    init_cfg.dstptr_incr = 0;                       //Ŀ��ָ���ַ����Ϊ������
    init_cfg.srcptr_incr = 1;                       //Դָ���ַ������

    init_cfg.src_start_addr = g_i2s.tx_fifo;        //Դ���ݷ�����ʼ��ַ
    init_cfg.dst_start_addr = (void *)PCM_DAT0;     //Ŀ�����ݽ�����ʼ��ַ

    init_cfg.channel = CFG_I2S_TX_DMA_CHANNEL;
    init_cfg.prio = 0;                              //���ȼ�
    init_cfg.u.type4.src_loop_start_addr = g_i2s.tx_fifo;                        //Դ��������ʼ��ַ��
    init_cfg.u.type4.src_loop_end_addr = g_i2s.tx_fifo + CFG_I2S_TX_BUFFER_SIZE;     //Դ������������ַ��
//#if    I2S_TX_CALLBACK
    if((param->i2s_tx_fin_handler != NULL) && (param->i2s_tx_half_fin_handler != NULL))
    {
        init_cfg.half_fin_handler = param->i2s_tx_half_fin_handler;
        init_cfg.fin_handler = param->i2s_tx_fin_handler;
        I2S_TX_CALLBACK = true;
    }
    else
        info_printf("IIS","TX not registered callback functions \r\n");
//#endif
    init_cfg.src_module = GDMA_X_SRC_DTCM_RD_REQ;       //Դģ��ΪDTCM��
    init_cfg.dst_module = GDMA_X_DST_I2S_TX_REQ;        //Ŀ��ģ��ΪI2S����

    res = sddev_control(GDMA_DEV_NAME, CMD_GDMA_CFG_TYPE4, (void *)&init_cfg);
    if(res != 0)
    {
        error_printf("IIS"," IIS_DMA_TxInit fail\r\n");
        return -1;
    }
    en_cfg.channel = CFG_I2S_TX_DMA_CHANNEL;
    en_cfg.param = CFG_I2S_TX_BUFFER_SIZE; // dma ���ͻ����С
    res = sddev_control(GDMA_DEV_NAME, CMD_GDMA_SET_TRANS_LENGTH, (void *)&en_cfg);
    if(res != 0)
    {
        error_printf("IIS"," IIS_DMA_TxInit fail\r\n");
        return -1;
    }
//#if !I2S_TX_CALLBACK
    if(I2S_TX_CALLBACK == false)
    {
        rb_init_dma_read(&g_i2s.wr_dma_rd, g_i2s.tx_fifo, CFG_I2S_RX_BUFFER_SIZE, CFG_I2S_TX_DMA_CHANNEL);
    }
    else
        info_printf("IIS","TX not init ring buf \r\n");
//#endif
    return res;
}

// =============================================================================
// ���ܣ���I2S�豸
// ������sample - �����ʣ� bit_width - λ��mode -��ģʽ��O_RDONLY��O_WRONLY��O_RDWR
// ���أ�0=�ɹ���-1=ʧ��
// =============================================================================
s32 IIS_Open(struct IIS_OperParam *param, u32 mode)
{
    if(mode == 0)
    {
        error_printf("IIS"," mode is error. mode = %x\r\n", mode);
        return -1;
    }

    g_i2s.open_flag = mode;
    g_i2s.tx_paused = 1;
    g_i2s.tx_fill_pos = (u32)g_i2s.tx_fifo;
    g_i2s.tx_fill_size = CFG_I2S_TX_BUFFER_SIZE / 2;
    g_i2s.sample_rate = param->sample_rate;
    g_i2s.bits_width = param->bits_width;

    i2s_configure(FIFO_LEVEL_32, g_i2s.sample_rate, g_i2s.bits_width, I2S_DEFAULT_MODE);

    if(mode & O_RDONLY)
    {
        g_i2s.rx_fifo = malloc(CFG_I2S_RX_BUFFER_SIZE);   //����i2s��DMA���ܵĽ��ջ�����
        if (g_i2s.rx_fifo == NULL)
        {
            error_printf("IIS","malloc rx_fifo failed\r\n");
            return -1;
        }
        memset(g_i2s.rx_fifo, 0, CFG_I2S_RX_BUFFER_SIZE);
        if(IIS_DMA_RxInit(param) == 0)
        {
            if(IIS_SetRX_DMA_PauseAddr(0) != 0)
            {
                free(g_i2s.rx_fifo);
                return -1;
            }
        }
        else
        {
            free(g_i2s.rx_fifo);
            return -1;
        }
    }

    if(mode & O_WRONLY)
    {
        g_i2s.tx_fifo = malloc(CFG_I2S_TX_BUFFER_SIZE);    //����i2s��DMA���ܵķ��ͻ�����
        if (g_i2s.tx_fifo == NULL)
        {
            error_printf("IIS"," malloc tx_fifo failed\r\n");
            if(mode & O_RDONLY)
                free(g_i2s.rx_fifo);
            return -1;
        }
        memset(g_i2s.tx_fifo, 0, CFG_I2S_TX_BUFFER_SIZE);
        if(IIS_DMA_TxInit(param) == 0)
        {
            if(IIS_SetTX_DMA_PauseAddr((UINT32)g_i2s.tx_fifo) != 0)
            {
                free(g_i2s.tx_fifo);
                if(mode & O_RDONLY)
                    free(g_i2s.rx_fifo);
                return -1;
            }
        }
        else
        {
            free(g_i2s.tx_fifo);
            if(mode & O_RDONLY)
                free(g_i2s.rx_fifo);
            return -1;
        }
    }

    return 0;
}

// =============================================================================
// ���ܣ�I2S�豸����
// ������buffer - ���ջ�������size - ��������С
// ���أ�ʵ�ʶ�ȡ�Ĵ�С
// =============================================================================
s32 IIS_Read(void *buffer, s32 size)
{
    s32 fill_size = 0;
    if(!(g_i2s.open_flag & O_RDONLY))
        return 0;

#if CFG_GENERAL_DMA
    if((I2S_RX_CALLBACK == false) && (g_i2s.rb_dma_wr.address) && (g_i2s.rb_dma_wr.capacity))
    {
        fill_size = rb_get_fill_size_dma_write(&g_i2s.rb_dma_wr);
        if(fill_size > size)
            fill_size = size;
        rb_read_dma_write(&g_i2s.rb_dma_wr, (UINT8 *)buffer, fill_size, 1);
    }
#endif

    return fill_size;
}

// =============================================================================
// ���ܣ�I2S�豸����
// ������buffer - ���ջ�������size - ��������С
// ���أ�ʵ�ʶ�ȡ�Ĵ�С
// =============================================================================
s32 IIS_Write(void *buffer, s32 size)
{
    s32 fill_size = 0;
    if(!(g_i2s.open_flag & O_WRONLY))
        return 0;

#if CFG_GENERAL_DMA
    if((I2S_TX_CALLBACK == false) && (g_i2s.wr_dma_rd.address) && (g_i2s.wr_dma_rd.capacity))
    {
        fill_size = rb_get_free_size_dma_read(&g_i2s.wr_dma_rd);
        if(fill_size > size)
            fill_size = size;
        rb_write_dma_read(&g_i2s.wr_dma_rd, (UINT8 *)buffer, fill_size, 1);
    }
#endif

    return fill_size;
}

// =============================================================================
// ���ܣ�I2S�豸�ر�
// ��������
// ���أ�0=�ɹ���-1=ʧ��
// =============================================================================
s32 IIS_Close(void)
{
    s32 ret = -1;
    if (g_i2s.open_flag)
    {
        if(g_i2s.open_flag & O_WRONLY)
        {
            free(g_i2s.tx_fifo);
            ret = IIS_DMA_TxEnable(0);
        }
        if(g_i2s.open_flag & O_RDONLY)
        {
            free(g_i2s.rx_fifo);
            ret = IIS_DMA_RxEnable(0);
        }
        if(ret == 0)
        {
            //i2s_dma_enable(0);
            g_i2s.open_flag = 0;
            g_i2s.tx_paused = 0;
            if(IIS_Enable(0) == 0)
                ret = 0;
        }
        memset(&g_i2s, 0, sizeof(g_i2s));
    }

    return ret;
}

// =============================================================================
// ���ܣ�I2S�豸����
// ��������
// ���أ�0=�ɹ�������=ʧ��
// =============================================================================
s32 IIS_Cotrol(enum i2s_ctrl cmd, ptu32_t data)
{
    s32 ret = 0;
//    i2s_rate_t rate;
    switch (cmd)
    {
        case IIS_DMA_RX_ENABLE:
            ret = IIS_DMA_RxEnable(data);
            break;
        case IIS_DMA_TX_ENABLE:
            ret = IIS_DMA_TxEnable(data);
            break;
        case I2S_DMA_MASTER_SLAVE:
            ret = IIS_DMA_SetMasterSlave(data);
            break;
//        case I2S_SAMPLE_RATE_SET:
//        case I2S_BIT_WIDTH_SET:
//            if ((g_i2s.sample_rate != data) && (g_i2s.bits_width != data))
//            {
//                IIS_Close();
////                if(data > 32)
////                memset(&g_i2s, 0, sizeof(g_i2s));
//                    g_i2s.sample_rate = 44100;
////                else
//                    g_i2s.bits_width = 16;
//                IIS_Open(g_i2s.sample_rate, g_i2s.bits_width, 1);
//                IIS_Cotrol(I2S_DMA_MASTER_SLAVE, 1);
//                IIS_Cotrol(IIS_DMA_TX_ENABLE, 1);
////                ret = i2s_configure(FIFO_LEVEL_32, g_i2s.sample_rate, g_i2s.bits_width, I2S_DEFAULT_MODE);
////                if(ret == 0)
////                {
////                    if (g_i2s.open_flag & O_RDONLY)
////                    {
////                        ret = IIS_DMA_RxInit();
////                        if(ret == 0)
////                            ret = IIS_SetRX_DMA_PauseAddr(0);
////                    }
////                    if (g_i2s.open_flag & O_WRONLY)
////                    {
////                        ret = IIS_DMA_TxInit();
////                        if(ret == 0)
////                            ret = IIS_SetTX_DMA_PauseAddr(0);
////                    }
////                }
//            }
//            break;

        default:
            notice_printf("IIS", "unprocessed commands %d\r\n",cmd);
            break;
    }

    return ret;
}

// =============================================================================
// ���ܣ���ʼ��I2Sģ��
// ��������
// ���أ�1=�ɹ���0=ʧ��
// =============================================================================
int ModuleInstall_I2S(void)
{
    i2s_init(0);    //��װi2sģ�飬��ʹ��i2s�жϡ�
    return 1;
}

