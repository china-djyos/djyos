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
#include "icu_pub.h"
#include "intc_pub.h"
#include "power_save_pub.h"
#include "saradc/saradc.h"
#include <lock.h>
#include <string.h>
#include <stdio.h>
#include <systime.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//extern void ModuleInstall_ADC(void);
//  ModuleInstall_ADC( );
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip adc"       //adc���
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
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

#define REG_READ(addr)          (*((volatile UINT32 *)(addr)))
#define REG_WRITE(addr, _data)  (*((volatile UINT32 *)(addr)) = (_data))

DD_HANDLE djy_adc_open(uint8_t channel, saradc_desc_t *adcDesc, uint8_t cntOfDataBuff)
{
    u32 status;
    DD_HANDLE retHdl = DD_HANDLE_UNVALID;

    if (channel > 7) // TODO merlin �����ж��ٸ�ADC�� ��12������ȷ������������д������  SARADC_ADC_CHNL_MAX)
    {
        return 0;
    }

    if (0 == adcDesc)
    {
        return 0;
    }

    adcDesc->channel = channel;
    adcDesc->data_buff_size = cntOfDataBuff;
    adcDesc->mode = (ADC_CONFIG_MODE_CONTINUE << 0) | (ADC_CONFIG_MODE_8CLK_DELAY << 2);

    adcDesc->has_data                = 0;
    adcDesc->current_read_data_cnt   = 0;
    adcDesc->current_sample_data_cnt = 0;
    adcDesc->pre_div = 0x10;
    adcDesc->samp_rate = 0x20;
//    memset(adcDesc->pData, 0, cntOfDataBuff*sizeof(*adcDesc->pData));

    retHdl = ddev_open(SARADC_DEV_NAME, &status, (UINT32)adcDesc);

    return retHdl;
}

void djy_adc_fill_buffer(DD_HANDLE handle, saradc_desc_t *adcDescOpened, uint32_t delayUs)
{
//    s64 startTimeUs = 0;
//    s64 tmpDelayUs = delayUs;
//    uint32_t hasDelayMs = 0;
    uint32_t cmd;
    uint8_t run_stop;

    if (0 == adcDescOpened)
    {
        return ;
    }

    adcDescOpened->current_read_data_cnt   = 0;
    adcDescOpened->current_sample_data_cnt = 0;

    cmd = SARADC_CMD_RUN_OR_STOP_ADC;
    run_stop = 1;
    ddev_control(handle, cmd, &run_stop);

//    startTimeUs = DJY_GetSysTime();
//    while ((DJY_GetSysTime()-startTimeUs) < tmpDelayUs)
//    {
//        if (adcDescOpened->current_sample_data_cnt == adcDescOpened->data_buff_size)
//        {
//            ddev_close(handle);
//            break;
//        }
//    }
}

void djy_adc_close(DD_HANDLE handle)
{
    ddev_close(handle);
}




// ���ADCϵͳ�������룬ͬһʱ��ֻ��ֻ��һ��ADCͨ��������
//#define ADC_TEMP_BUFFER_SIZE 2
//static saradc_desc_t tmp_single_desc;
//static UINT16 tmp_single_buff[ADC_TEMP_BUFFER_SIZE];//ADC_TEMP_BUFFER_SIZE];
//static volatile DD_HANDLE tmp_single_hdl = DD_HANDLE_UNVALID;

struct MutexLCB *pg_tadc_mutex = NULL;

int djy_adc_read(uint16_t channel) // ע�⣡���� �������ж���ʹ�ã�����
{
#define ADC_TEMP_BUFFER_SIZE 2
    saradc_desc_t tmp_single_desc;
    UINT16 tmp_single_buff[ADC_TEMP_BUFFER_SIZE];//ADC_TEMP_BUFFER_SIZE];
    volatile DD_HANDLE tmp_single_hdl = DD_HANDLE_UNVALID;
    int tryTimes = 1000;
    UINT32 status;
    UINT32 cmd;
    UINT8 run_stop;
    u16 ad_date;
    int tmpData = 0xFFFFFFFF;
    float voltage = 0.0;

    if (channel > 7) // TODO merlin �����ж��ٸ�ADC�� ��12������ȷ������������д������  SARADC_ADC_CHNL_MAX)
    {
        return tmpData;
    }

    memset(&tmp_single_desc, 0x00, sizeof(saradc_desc_t));
    tmp_single_desc.channel = channel;
    tmp_single_desc.data_buff_size = ADC_TEMP_BUFFER_SIZE;
    tmp_single_desc.mode = (ADC_CONFIG_MODE_CONTINUE << 0) | (ADC_CONFIG_MODE_8CLK_DELAY << 2);
    tmp_single_desc.has_data                = 0;
    tmp_single_desc.current_read_data_cnt   = 0;
    tmp_single_desc.current_sample_data_cnt = 0;
    tmp_single_desc.pre_div = 0x10;
    tmp_single_desc.samp_rate = 0x20;
    memset(tmp_single_buff, 0, sizeof(tmp_single_buff));
    tmp_single_desc.pData = &tmp_single_buff[0];

    Lock_MutexPend(pg_tadc_mutex, 0xffffffff);

    tmp_single_hdl = ddev_open(SARADC_DEV_NAME, &status, (UINT32)&tmp_single_desc);

    cmd = SARADC_CMD_RUN_OR_STOP_ADC;
    run_stop = 1;
    ddev_control(tmp_single_hdl, cmd, &run_stop);

    while (tryTimes--)
    {
        DJY_EventDelay(10);
        if (tmp_single_desc.current_sample_data_cnt == tmp_single_desc.data_buff_size)
        {
            ddev_close(tmp_single_hdl);

            ad_date = 0;
            for (int i=0; i<tmp_single_desc.current_sample_data_cnt; i++)
            {
                ad_date += tmp_single_desc.pData[i];
            }
            ad_date /= tmp_single_desc.current_sample_data_cnt;
            voltage = saradc_calculate(ad_date);
            tmpData = voltage * 1000;
            break;
        }
    }
    if (tryTimes <= 0) {
        ddev_close(tmp_single_hdl);
    }
    Lock_MutexPost(pg_tadc_mutex);

    return tmpData;
}
#if 1

//���ٲ���
//
s32 djy_adc_fast_read(u8 channel)
{
    UINT32 value = 0, reg;
    u8 mode = (ADC_CONFIG_MODE_CONTINUE << 0) | (ADC_CONFIG_MODE_4CLK_DELAY << 2);
    u8 pre_div = 0x10,samp_rate = 0x20,filter = 0;
    u16 ad_date;
    int voltage = 0xFFFFFFFF;

    if(channel > SARADC_ADC_CHNL_MAX){
        return SARADC_FAILURE;
    }

    reg = REG_READ(ICU_PERI_CLK_PWD);
    reg &= ~PWD_SARADC_CLK_BIT;
    REG_WRITE(ICU_PERI_CLK_PWD, reg);


    // clear fifo
    while((value & SARADC_ADC_FIFO_EMPTY) == 0) {
        REG_READ(SARADC_ADC_DATA);
        value = REG_READ(SARADC_ADC_CONFIG);
    }

    value = ((mode & SARADC_ADC_MODE_MASK) << SARADC_ADC_MODE_POSI)
        | SARADC_ADC_CHNL_EN
        | ((channel & SARADC_ADC_CHNL_MASK) << SARADC_ADC_CHNL_POSI)
        | (((mode>>2)&0x01) << SARADC_ADC_DELAY_CLK_POSI)
        | SARADC_ADC_INT_CLR
        | ((pre_div & SARADC_ADC_PRE_DIV_MASK) << SARADC_ADC_PRE_DIV_POSI)
        | ((samp_rate & SARADC_ADC_SAMPLE_RATE_MASK) << SARADC_ADC_SAMPLE_RATE_POSI)
        | ((filter & SARADC_ADC_FILTER_MASK)<< SARADC_ADC_FILTER_POSI);
    REG_WRITE(SARADC_ADC_CONFIG, value);

#if (CFG_SOC_NAME != SOC_BK7231)
    value = SARADC_ADC_SAT_ENABLE
       //| ((0x01 & SARADC_ADC_SAT_CTRL_MASK) << SARADC_ADC_SAT_CTRL_POSI);
       | ((0x03 & SARADC_ADC_SAT_CTRL_MASK) << SARADC_ADC_SAT_CTRL_POSI);
    REG_WRITE(SARADC_ADC_SATURATION_CFG, value);
#endif


    Lock_MutexPend(pg_tadc_mutex, 0xffffffff);

    value = REG_READ(SARADC_ADC_CONFIG);
    value &= ~(SARADC_ADC_CHNL_MASK << SARADC_ADC_CHNL_POSI);
    value |= (channel << SARADC_ADC_CHNL_POSI);
    value |= SARADC_ADC_CHNL_EN;
    REG_WRITE(SARADC_ADC_CONFIG, value);

    value = REG_READ(SARADC_ADC_CONFIG);
    while((value & SARADC_ADC_FIFO_EMPTY) != 0)
    {
        value = REG_READ(SARADC_ADC_CONFIG);
    }

    #if (CFG_SOC_NAME == SOC_BK7231)
    ad_date = REG_READ(SARADC_ADC_DATA)&0x03FF;
    #else
    ad_date = REG_READ(SARADC_ADC_DAT_AFTER_STA)&0xFFFF;
    #endif // (CFG_SOC_NAME == SOC_BK7231)
    voltage = saradc_calculate_fast(ad_date);
//    tmpData = voltage * 1000;

    value = REG_READ(SARADC_ADC_CONFIG);
    value &= ~(SARADC_ADC_MODE_MASK << SARADC_ADC_MODE_POSI);
    value &= ~(SARADC_ADC_CHNL_EN);
    REG_WRITE(SARADC_ADC_CONFIG, value);

    reg = REG_READ(ICU_PERI_CLK_PWD);
    reg |= PWD_SARADC_CLK_BIT;
    REG_WRITE(ICU_PERI_CLK_PWD, reg);

    Lock_MutexPost(pg_tadc_mutex);

    return voltage;
}
#endif
#if 0
static void temp_single_detect_handler2(void)
{
    if(tmp_single_desc.current_sample_data_cnt >= tmp_single_desc.data_buff_size)
    {
        #if (CFG_SOC_NAME != SOC_BK7231)
        UINT32 sum = 0, sum1, sum2=0;
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
#endif




int vbat_voltage_get(void)
{
    int i, j = 0, Vbat = 0, AverageVbat = 0;
    for(i = 0; i < 10; i++)
    {
        Vbat = djy_adc_read(0);
        Vbat = Vbat * 2;
        if((Vbat > 2700) && (Vbat < 4400))  //ȥ��һЩ�����ѹ
        {
            AverageVbat += Vbat;
            j++;
        }
    }
    AverageVbat = AverageVbat / j;
    return AverageVbat;
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
    UINT32 status;
    saradc_init();
    Int_SaveAsynLine(IRQ_SARADC);
    pg_tadc_mutex = Lock_MutexCreate("pg_tadc_mutex");
//  memset(&tmp_single_desc, 0x00, sizeof(saradc_desc_t));
//  tmp_single_desc.channel = 1;
//  tmp_single_desc.data_buff_size = ADC_TEMP_BUFFER_SIZE;
//  tmp_single_desc.mode = (ADC_CONFIG_MODE_CONTINUE << 0) | (ADC_CONFIG_MODE_4CLK_DELAY << 2);
//  tmp_single_desc.has_data                = 0;
//  tmp_single_desc.current_read_data_cnt   = 0;
//  tmp_single_desc.current_sample_data_cnt = 0;
//  tmp_single_desc.pre_div = 0x10;
//  tmp_single_desc.samp_rate = 0x20;
//  tmp_single_desc.pData = &tmp_single_buff[0];
//  tmp_single_hdl = ddev_open(SARADC_DEV_NAME, &status, (UINT32)&tmp_single_desc);

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

