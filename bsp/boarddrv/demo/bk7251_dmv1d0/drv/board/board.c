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


#include "stdint.h"
#include "stddef.h"
#include <int.h>
#include "mem_pub.h"
#include "wdt_pub.h"
#include <shell.h>
#include "cpu_peri.h"
#include "board.h"
#include "icu_pub.h"
#include "general_dma_pub.h"
#include "bk_timer_pub.h"
#include "pwm_pub.h"
#include "mac_phy_bypass_pub.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"board config"//�����
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:required               //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"cpu onchip gpio"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_BOARD_CONFIG == false )
//#warning  " board_config  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_BOARD_CONFIG    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure



// =============================================================================
// ���ܣ�Board_GpioInit���������õ���GPIO��ʼ��
//    �����ｫ���п����õ��ĸ���ģʽ��gpio������������Ҫ��ȡ��ע�ͼ���
//    ���鲻Ҫɾ��
// ��������
// ���أ���
// =============================================================================
//
//static struct SemaphoreLCB* p7_sem=0;
//void p7_sem_init()
//{
//    if (p7_sem==0) {
//        p7_sem = semp_init(1,1,"p7_sem");
//    }
//}
//
//void p7_isr_hdr(void *args)
//{
//    if (p7_sem) {
//         Lock_SempPost(p7_sem);
//    }
//}
//
//int pend_p7_down(unsigned int timeout)
//{
//    int ret = 0;
//    ret = Lock_SempPend(p7_sem, timeout);
//    return ret;
//}
//
//���ݴ���оƬIIC�ӿڳ�ʼ��
void FT6236_Pin_Init(void)
{
    djy_gpio_mode(GPIO4,PIN_MODE_OUTPUT);        //SCL
    djy_gpio_mode(GPIO2,PIN_MODE_OUTPUT);        //SDA

    djy_gpio_mode(GPIO6,PIN_MODE_INPUT_PULLUP);  //RT_INT
    djy_gpio_mode(GPIO7,PIN_MODE_OUTPUT);        //RT_RST
}

u32 IIC_IoCtrlFunc(enum IIc_Io IO,u32 tag)
{
    switch(tag)
    {
        case 1 :
            switch(IO)
            {
            case scl_set_High : CT_IIC_SCL(1); break;
            case scl_set_Low  : CT_IIC_SCL(0); break;
            case scl_set_out  : CT_SCL_OUT();  break;
            case sda_set_High : CT_IIC_SDA(1); break;
            case sda_get      : return CT_READ_SDA;
            case sda_set_Low  : CT_IIC_SDA(0); break;
            case sda_set_out  : CT_SDA_OUT();  break;
            case sda_set_in   : CT_SDA_IN();   break;
            default:
                break;
            }
        break;
    }
    return 0;
}
void OpenBackLight()
{
    djy_gpio_write(GPIO10,1);
}

void CloseBackLight()
{
    djy_gpio_write(GPIO10,0);
}

void OpenScreen()
{
    djy_gpio_write(GPIO11,1);
    djy_gpio_write(GPIO10,1);
    FT_RST(1);
}

void CloseScreen()
{
    djy_gpio_write(GPIO10,0);
    FT_RST(0);
    djy_gpio_write(GPIO11,0);
}

static enum SpeakerState Speaker = Speaker_off;
void CloseSpeaker()
{
    djy_gpio_write(GPIO9,0);
    Speaker = Speaker_off;
}
void OpenSpeaker()
{
    djy_gpio_write(GPIO9,1);
    Speaker = Speaker_on;
}
enum SpeakerState GetSpeakerState()
{
    return Speaker;
}

// =============================================================================
// ���ܣ���������ϵͳʱ��Ϊ180M���ú�����initcpu.S�е��ã�
// ��������
// ���أ���
// =============================================================================
extern UINT32 sctrl_ctrl(UINT32 cmd, void *param);
extern void sctrl_set_cpu_clk_dco(void);
extern void sctrl_dco_cali(UINT32 speed);
void InitOsClk_180M(void)
{
    u32 param;
    param = BLK_BIT_26M_XTAL | BLK_BIT_DPLL_480M | BLK_BIT_XTAL2RF | BLK_BIT_DCO;
    sctrl_ctrl(CMD_SCTRL_BLK_ENABLE, &param);

    sctrl_dco_cali(5);      //#define DCO_CALIB_180M          0x5
    sctrl_set_cpu_clk_dco();
}

void Board_Init(void)
{
    extern void os_clk_init(void);
    extern s32 Djy_GpioInit(void);
    Djy_GpioInit();
    os_meminit();
    drv_model_init();
//  g_dd_init();
    sctrl_init();
    icu_init();
    wdt_init();
    gpio_init();
    flash_init();       //���������ܷŵ�ModuleInstall_Flash�У�sctrl_sta_ps_init����ʱ��Ҫ����flash�豸��
    gdma_init();
    bk_timer_init();
    mpb_init();                         //��ʱ�ȷ�����
    sctrl_sta_ps_init();
    pwm_init();         //����os_clk_init��CPUʱ��

    intc_init();
    os_clk_init();

//    void ble_init(void);
//    ble_init();
#if 0
    djy_gpio_mode(GPIO7,PIN_MODE_INPUT_PULLUP);  //wifiһ������
    djy_gpio_mode(GPIO3,PIN_MODE_INPUT_PULLUP);  //��Чģʽ
    djy_gpio_mode(GPIO4,PIN_MODE_INPUT_PULLUP);  //����ģʽ

    djy_gpio_mode(GPIO13,PIN_MODE_INPUT_PULLUP);   //��������

    djy_gpio_mode(GPIO9,PIN_MODE_OUTPUT);        //LED
    djy_gpio_write(GPIO9,0);

    djy_gpio_mode(GPIO8,PIN_MODE_OUTPUT);        //WIFI����״̬
    djy_gpio_write(GPIO8,0);
#else
//    djy_gpio_mode(GPIO13,PIN_MODE_INPUT_PULLUP);  //wifiһ������
//    djy_gpio_mode(GPIO10,PIN_MODE_INPUT_PULLUP);  //��ȡ����״̬

//    p7_sem_init();
//    djy_gpio_mode(GPIO7,PIN_MODE_INPUT_PULLUP);   //��������
//    djy_gpio_attach_irq(GPIO7, PIN_IRQ_MODE_FALLING, p7_isr_hdr, NULL);
//    djy_gpio_irq_enable( GPIO7, 1);


    djy_gpio_mode(GPIO13, GMODE_INPUT_PULLUP); //���Ѽ�
    djy_gpio_mode(GPIO10,PIN_MODE_OUTPUT);         //Һ������
    djy_gpio_mode(GPIO8,PIN_MODE_INPUT_PULLUP);          //����
    djy_gpio_mode(GPIO9,PIN_MODE_INPUT_PULLUP);          //����
//    djy_gpio_write(GPIO10,1);

    djy_gpio_mode(GPIO11,PIN_MODE_OUTPUT);        //Һ��+��������Դ���ƹܽ�
    djy_gpio_write(GPIO11,1);

#endif
//    void uart1_exit(void);
//    uart1_exit();
}

void Init_Cpu(void);
void SoftReset(void)
{
    Init_Cpu();
}

//���ڰ����jtag�ڱ�psramռ�ã����³��������޷��������������������ֹpsram��ʹ��jtag��
//�ر��жϺ������ѭ�������������������ԣ��÷������֣�
//1�����shell������Ӧ������ִ��jtag������ñ�������
//2��������Ӵ��룬����⵽����ʱ���������ñ��������ȴ����������롣
void stub_debug(void)
{
    atom_high_t atom;
    volatile u32 t = 1;
    atom = Int_HighAtomStart();
    printk("+++++++++enable jtag��you can contact ICE debuger++++++++++++\r\n");
    EnJtag();
    if(t)
        while(1);
    Int_HighAtomEnd(atom);
}
bool_t init_jtag(char *param)
{
    (void)param;
    sddev_control(WDT_DEV_NAME, WCMD_POWER_DOWN, 0);
    stub_debug();
    return true;
}

ADD_TO_ROUTINE_SHELL(initjtag,init_jtag,"���³�ʼ�� :COMMAND:init_jtag+enter");

void ST7789V_GpioInit(void)
{
    gpio_config(GPIO15, GMODE_OUTPUT);  //1=��д���ݶ˿ڣ�0=��д����˿�
    gpio_output(GPIO15, 1);

    gpio_config(GPIO19, GMODE_OUTPUT);  //rst
    gpio_output(GPIO19, 1);

    gpio_config(GPIO3, GMODE_OUTPUT);   //CS
    gpio_output(GPIO3, 1);
}

void SPI_RST(u32 value)
{
    djy_gpio_write(GPIO19, value);
}

void SPI_RS(u32 value)
{
    djy_gpio_write(GPIO15, value);
}

void ST7789V_CsActive(void)
{
    gpio_output(GPIO3, 0);
}
void ST7789V_CsInactive(void)
{
    gpio_output(GPIO3, 1);
}


