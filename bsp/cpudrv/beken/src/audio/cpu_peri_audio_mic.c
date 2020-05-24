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
#include "cpu_peri.h"
#include "stdlib.h"
#include "component_config_audio.h"

static AUD_ADC_CFG_ST aud_adc;

void djy_audio_adc_open(uint16_t buf_len,uint16_t channel,
        audio_sample_rate_e freq,uint32_t linein_detect_pin)
{
    if(channel>2)
        return;
    aud_adc.buf = malloc(buf_len);
    if(aud_adc.buf==NULL)
        return;
    aud_adc.buf_len = buf_len;
    aud_adc.channels = channel;
    aud_adc.mode |= AUD_ADC_MODE_DMA_BIT;
    aud_adc.linein_detect_pin = linein_detect_pin;
    aud_adc.freq = freq;
    audio_adc_open((uint32_t)(&aud_adc));

    audio_adc_ctrl(AUD_ADC_CMD_PLAY,0);
    //linein��ⲻ����������������ڼ������������Ͳ��ܼ���ˡ�
    djy_gpio_mode(linein_detect_pin,PIN_MODE_INPUT);
}

void djy_linein_adc_open(uint16_t buf_len,uint16_t channel,
        audio_sample_rate_e freq,uint32_t linein_detect_pin)
{
    if(channel>2)
        return;
    aud_adc.buf = malloc(buf_len);
    if(aud_adc.buf==NULL)
        return;
    aud_adc.buf_len = buf_len;
    aud_adc.channels = channel;
    aud_adc.mode |= AUD_ADC_MODE_DMA_BIT;
    aud_adc.linein_detect_pin = linein_detect_pin;
    aud_adc.freq = freq;
    audio_adc_open((uint32_t)(&aud_adc));
    audio_adc_ctrl(AUD_ADC_CMD_DO_LINEIN_DETECT,0);

    audio_adc_ctrl(AUD_ADC_CMD_PLAY,0);
}

uint32_t djy_audio_adc_read(char *user_buf, uint32_t count)
{
    return audio_adc_read(user_buf, count, 0);
}

void djy_audio_adc_control( uint32_t cmd, void *args)
{
    audio_adc_ctrl(cmd,args);
}

void djy_audio_adc_close(void)
{
    audio_adc_close();
    free(aud_adc.buf);
}



