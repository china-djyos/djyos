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
#include <device/djy_uart.h>
#include "stdlib.h"
#include "gpio_pub.h"
#include "icu_pub.h"
#include "spi/spi.h"
#include "dbug.h"
#include "spi_pub.h"
#include <arm_arch.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern int ModuleInstall_SPI(void);
//    ModuleInstall_SPI();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip spi"//spi�ӿڵ�norflash
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:pre-main               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"lock","spi bus","heap"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_SPI == false )
//#warning  " myname �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_SPI    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,30000000,
#define CFG_SPI_CLK                    30000000       //SPI ʱ�ӡ�
//%$#@num,0,1,
#define CFG_SPI_CPOL                     1           //spiʱ�Ӽ��ԣ�1��SCK�ڿ���״̬���ڸߵ�ƽ��0����֮����
#define CFG_SPI_CPHA                     1           //spiʱ����λ��1����SCK���ڵĵڶ������ز������ݡ�0����SCK���ڵĵ�һ�����ز������ݣ�
//%$#@enum,true,false,
#define CFG_SPI_FLASH_RAM_POWER         true         //�Ƿ��flash��ram�ĵ�Դ��
#define CFG_SPI_WORK_MODE_INTE          true        //����SPI�Ĺ���ģʽ��trueΪ�ж�ģʽͨ�ţ�falseΪ��ͨģʽ��
//%$#@select
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


//spi������ź�QSPI�غϣ�����������ǹر�QSPI��IO2��IO3
static void Spi_Init_Extral_Gpio(void)
{
    bk_gpio_config_output(SPI_FLASH_WP_GPIO_NUM);
    bk_gpio_output(SPI_FLASH_WP_GPIO_NUM, GPIO_INT_LEVEL_HIGH);

    bk_gpio_config_output(SPI_FLASH_HOLD_GPIO_NUM);
    bk_gpio_output(SPI_FLASH_HOLD_GPIO_NUM, GPIO_INT_LEVEL_HIGH);
}

//��flash��ram�ĵ�Դ
static void SPI_Flash_Enable_Voltage(void)
{
    UINT32 param;

    param = BLK_BIT_MIC_QSPI_RAM_OR_FLASH;
    sddev_control(SCTRL_DEV_NAME, CMD_SCTRL_BLK_ENABLE, &param);

    param = QSPI_IO_3_3V;
    sddev_control(SCTRL_DEV_NAME, CMD_QSPI_IO_VOLTAGE, &param);

    param = PSRAM_VDD_3_3V_DEF;
    sddev_control(SCTRL_DEV_NAME, CMD_QSPI_VDDRAM_VOLTAGE, &param);
}

//�ر�flash��ram�ĵ�Դ
static void SPI_Flash_Disable_Voltage(void)
{
    UINT32 param;

    param = BLK_BIT_MIC_QSPI_RAM_OR_FLASH;
    sddev_control(SCTRL_DEV_NAME, CMD_SCTRL_BLK_DISABLE, &param);
}

void SPI_Flash_Deinit(void)
{
    SPI_Flash_Disable_Voltage();
    bk_spi_master_deinit();
}

bool_t SPI_TxRx(u8* sdata,u32 slen,u8* rdata, u32 rlen, u32 recvoff)
{
    s32 result;
    u32 i, len_limit, value;
    u8 ch;

    struct spi_message spi_msg;

    if( (!sdata) || ((rlen != 0) && (!rdata)))
        return false;

    if(CFG_SPI_WORK_MODE_INTE)
    {
        spi_msg.recv_buf = rdata;
        spi_msg.recv_len = rlen;
        spi_msg.send_buf = sdata;
        spi_msg.send_len = slen;

        result = bk_spi_master_xfer(&spi_msg);
        if(result != (s32)rlen)
        {
            return false;
        }
    }
    else
    {
        if((rlen == 0) && (rdata == NULL))
        {
            for (i=0; i < slen; i++)
            {
                while(1)
                {
                    value = REG_READ(SPI_STAT);

                    if((value & TXFIFO_FULL) == 0)
                    {
                        REG_WRITE(SPI_DAT, sdata[i]);
                       break;
                    }
                    DJY_EventDelay(2);
                }
            }
            while(spi_read_rxfifo(&ch) == 1);
        }
        else
        {
            while(spi_read_rxfifo(&ch) == 1);
            len_limit = MAX(slen, rlen + recvoff);
            for (i=0; i < len_limit; i++)
            {
                if (i < slen)
                {
                    while(spi_write_txfifo(sdata[i]) == 0)
                    {
                        DJY_EventDelay(2);
                    }
                }
                else if((i >= slen)&&(i < len_limit))
                {
                    while(spi_write_txfifo(0xff) == 0)
                    {
                        DJY_EventDelay(2);
                    }
                }

                if((rdata) && (i>=recvoff))
                {

                    while(spi_read_rxfifo(&ch) == 0)
                    {
                        DJY_EventDelay(2);
                    }
                    rdata[i-recvoff] = ch;
                }
                else
                {
                    while(spi_read_rxfifo(&ch) == 0)
                    {
                        DJY_EventDelay(2);
                    }
                }
            }
        }
    }

    return true;
}

static void Spi_Configure(UINT32 rate, UINT32 mode)
{
    UINT32 param;

    /* data bit width */
    param = 0;
    spi_ctrl(CMD_SPI_SET_BITWIDTH, (void *)&param);

    /* baudrate */
//    printf("max_hz = %d \n", rate);
    spi_ctrl(CMD_SPI_SET_CKR, (void *)&rate);

    /* mode */
    if (mode & BK_SPI_CPOL)
    {
        param = 1;
        spi_ctrl(CMD_SPI_SET_CKPOL, (void *)&param);
    }
    else
    {
        param = 0;
        spi_ctrl(CMD_SPI_SET_CKPOL, (void *)&param);
    }

    /* CPHA */
    if (mode & BK_SPI_CPHA)
    {
        param = 1;
        spi_ctrl(CMD_SPI_SET_CKPHA, (void *)&param);
    }
    else
    {
        param = 0;
        spi_ctrl(CMD_SPI_SET_CKPHA, (void *)&param);
    }

    /* Master */
    param = 1;
    spi_ctrl(CMD_SPI_SET_MSTEN, (void *)&param);
    param = 3;
    spi_ctrl(CMD_SPI_SET_NSSID, (void *)&param);
    param = 1;
    spi_ctrl(CMD_SPI_INIT_MSTEN, (void *)&param);

    param = PWD_SPI_CLK_BIT;
    sddev_control(ICU_DEV_NAME, CMD_CLK_PWR_UP, &param);

    param = (IRQ_SPI_BIT);
    sddev_control(ICU_DEV_NAME, CMD_ICU_INT_ENABLE, &param);

    param = GFUNC_MODE_SPI;
    sddev_control(GPIO_DEV_NAME, CMD_GPIO_ENABLE_SECOND, &param);


    /* enable spi */
    param = 1;
    spi_ctrl(CMD_SPI_UNIT_ENABLE, (void *)&param);

//    printf("[CTRL]:0x%08x \n", REG_READ(SPI_CTRL));
}


// =============================================================================
// ���ܣ���ʼ��SPIģ��
// ��������
// ���أ�1=�ɹ���0=ʧ��
// =============================================================================
int ModuleInstall_SPI(void)
{
    u8 mode;

    spi_init();
    if(CFG_SPI_FLASH_RAM_POWER)
        SPI_Flash_Enable_Voltage();

//    Spi_Init_Extral_Gpio();

    mode = (CFG_SPI_CPOL | (CFG_SPI_CPHA << 1));

    if(CFG_SPI_WORK_MODE_INTE)
    {
        if(bk_spi_master_init(CFG_SPI_CLK, mode))
        {
            info_printf("SPI","SPI init fail.\r\n");
                return 0;
        }
    }
    else
        Spi_Configure(CFG_SPI_CLK, mode);

    return 1;
}


