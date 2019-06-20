//----------------------------------------------------
// Copyright (c) 2018, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2018������Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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
//����ģ��:
//���ߣ�
//�汾��V1.0.0
//�ļ�����:
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����:
//   ����:
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#include <typedef.h>
#include "drv_model_pub.h"
#include "sys_ctrl_pub.h"
#include "saradc_pub.h"

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip adc"       //adc���
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"cpu onchip gpio"  //������������������������none����ʾ�������������
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_ADC == false )
//#warning  "   ADC�������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_ADC    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,flase,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure


static saradc_desc_t tmp_single_desc;
static UINT16 tmp_single_buff[10];//ADC_TEMP_BUFFER_SIZE];
static volatile DD_HANDLE tmp_single_hdl = DD_HANDLE_UNVALID;


void djy_adc_mode(uint32_t channel, uint32_t mode)
{
//    switch (mode)
//    {
//    case PIN_MODE_INPUT:
//        bk_gpio_config_input(pin);
//        break;
//
//    case PIN_MODE_INPUT_PULLUP:
//        bk_gpio_config_input_pup(pin);
//        break;
//
//    case PIN_MODE_INPUT_PULLDOWN:
//        bk_gpio_config_input_pdwn(pin);
//        break;
//
//    case PIN_MODE_OUTPUT:
//        bk_gpio_config_output(pin);
//        break;
//    }
}

float djy_adc_read(uint16_t pin)
{
//    return saradc_calculate(pin);
//    UINT32 cmd;
//    UINT8 mode;
//
//    cmd = SARADC_CMD_SET_MODE;
//    mode = 3;
//    UINT32 retDdev = ddev_control(tmp_single_hdl, cmd, &mode);
//
//    cmd = SARADC_CMD_SET_CHANNEL;
//    saradc_chan_t chnl;
//    chnl.enable = 1;
//    chnl.channel = 1;
//    retDdev = ddev_control(tmp_single_hdl, cmd, &chnl);
//
//    cmd = SARADC_CMD_RUN_OR_STOP_ADC;
//    UINT8 run_stop = 1;
//    retDdev = ddev_control(tmp_single_hdl, cmd, &run_stop);
//
//
//    printf("Can't open saradc, have you register this device?\r\n");



    UINT32 status;
    UINT32 cmd;
    UINT8 run_stop;

    os_memset(&tmp_single_desc, 0x00, sizeof(saradc_desc_t));
    tmp_single_desc.channel = 1;
    tmp_single_desc.data_buff_size = 10;
    tmp_single_desc.mode = (ADC_CONFIG_MODE_CONTINUE << 0)
                           | (ADC_CONFIG_MODE_8CLK_DELAY << 2);

    tmp_single_desc.has_data                = 0;
    tmp_single_desc.current_read_data_cnt   = 0;
    tmp_single_desc.current_sample_data_cnt = 0;
    tmp_single_desc.pre_div = 0x10;
    tmp_single_desc.samp_rate = 0x20;
    memset(tmp_single_buff, 0, sizeof(tmp_single_buff));
    tmp_single_desc.pData = &tmp_single_buff[0];

    tmp_single_hdl = ddev_open(SARADC_DEV_NAME, &status, (UINT32)&tmp_single_desc);

    cmd = SARADC_CMD_RUN_OR_STOP_ADC;
    run_stop = 1;
    ddev_control(tmp_single_hdl, cmd, &run_stop);

    while (1)
    {
        if (tmp_single_desc.current_sample_data_cnt == tmp_single_desc.data_buff_size)
        {
            ddev_close(tmp_single_hdl);
            break;
        }
    }

    printf("buff:%p,%d,%d,%d,%d,%d\r\n", tmp_single_desc.pData,
                   tmp_single_desc.pData[0], tmp_single_desc.pData[1],
                   tmp_single_desc.pData[2], tmp_single_desc.pData[3],
                   tmp_single_desc.pData[4]);

    float voltage = saradc_calculate(tmp_single_desc.pData[4]);

    printf("voltage is [%f]\r\n", voltage);



    return SARADC_SUCCESS;
//    extern UINT32 ddev_control(DD_HANDLE handle, UINT32 cmd, VOID *param);
}

//int djy_adc_attach_irq(GPIO_INDEX pin,uint32_t mode,
//                            void (*hdr)(void *args), void *args)
//{
//    if (pin >= GPIONUM)
//        return -1;
//
//    gpio_dev.irq_desc[pin].handler = hdr;
//    gpio_dev.irq_desc[pin].param   = args;
//    if (mode == PIN_IRQ_MODE_RISING)
//    {
//        gpio_dev.irq_desc[pin].mode = GMODE_INPUT_PULLDOWN;
//    }
//    else if (mode == PIN_IRQ_MODE_FALLING)
//    {
//        gpio_dev.irq_desc[pin].mode = GMODE_INPUT_PULLUP;
//    }
//    return 0;
//}
//
//
//int djy_adc_irq_enable( GPIO_INDEX pin, uint32_t enabled)
//{
//    if (pin >= GPIONUM)
//        return -1;
//
//    if (enabled)
//        gpio_int_enable(pin, gpio_dev.irq_desc[pin].mode, gpio_irq_dispatch);
//    else
//        gpio_int_disable(pin);
//    return 0;
//}

static void temp_single_detect_handler2(void)
{
    if(tmp_single_desc.current_sample_data_cnt >= tmp_single_desc.data_buff_size)
    {
        #if (CFG_SOC_NAME != SOC_BK7231)
        UINT32 sum = 0, sum1, sum2;
        //turnon_PA_in_temp_dect();
//        temp_single_get_disable();
        printf("buff:%p,%d,%d,%d,%d,%d\r\n", tmp_single_desc.pData,
                       tmp_single_desc.pData[0], tmp_single_desc.pData[1],
                       tmp_single_desc.pData[2], tmp_single_desc.pData[3],
                       tmp_single_desc.pData[4]);

        sum1 = tmp_single_desc.pData[1] + tmp_single_desc.pData[2];
        sum2 += tmp_single_desc.pData[3] + tmp_single_desc.pData[4];
        sum = sum1 / 2 + sum1 / 2;
        sum = sum / 2;
        tmp_single_desc.pData[0] = sum;
        #else
        turnon_PA_in_temp_dect();
        temp_single_get_disable();
        printf("buff:%p,%d,%d,%d,%d,%d\r\n", tmp_single_desc.pData,
                       tmp_single_desc.pData[0], tmp_single_desc.pData[1],
                       tmp_single_desc.pData[2], tmp_single_desc.pData[3],
                       tmp_single_desc.pData[4]);
        #endif // (CFG_SOC_NAME != SOC_BK7231)

//        rtos_set_semaphore(&tmp_single_semaphore);
    }
}

static void temp_single_get_desc_init(void)
{
//    os_memset(&tmp_single_buff[0], 0, sizeof(tmp_single_buff));
//
////    saradc_config_param_init(&tmp_single_desc);
//    tmp_single_desc.channel = ADC_TEMP_SENSER_CHANNEL;
//    tmp_single_desc.pData = &tmp_single_buff[0];
//    tmp_single_desc.data_buff_size = ADC_TEMP_BUFFER_SIZE;
//    tmp_single_desc.p_Int_Handler = temp_single_detect_handler;
}

void ModuleInstall_ADC(void)
{
//    GLOBAL_INT_DECLARATION();

//    temp_single_get_desc_init();
//
//    status = BLK_BIT_TEMPRATURE_SENSOR;
//    sddev_control(SCTRL_DEV_NAME, CMD_SCTRL_BLK_ENABLE, &status);
//
//#if (CFG_SOC_NAME == SOC_BK7231)
//    turnoff_PA_in_temp_dect();
//#endif // (CFG_SOC_NAME == SOC_BK7231)

//    GLOBAL_INT_DISABLE();
//    saradc_config_param_init(&tmp_single_desc);
//    tmp_single_desc.pData = &tmp_single_buff[0];
//    tmp_single_desc.data_buff_size = 10;
//    tmp_single_desc.p_Int_Handler = temp_single_detect_handler2;
//
//    tmp_single_hdl = ddev_open(SARADC_DEV_NAME, &status, (UINT32)&tmp_single_desc);
//    if(DD_HANDLE_UNVALID == tmp_single_hdl)
//    {
////        GLOBAL_INT_RESTORE();
//        printf("Can't open saradc, have you register this device?\r\n");
//        return SARADC_FAILURE;
//    }
//    GLOBAL_INT_RESTORE();
}

