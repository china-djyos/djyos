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

static AUD_DAC_CFG_ST aud_dac = {0};

void djy_audio_dac_close(void);
void djy_audio_dac_open(uint16_t buf_len,uint16_t channel,
                                    audio_sample_rate_e freq)
{
    if(channel>2) channel=2;
    if(buf_len<0) buf_len=1024;

    aud_dac.buf = realloc(aud_dac.buf, buf_len);
    if(aud_dac.buf==0){
        printf("err: djy_audio_dac_open failed!\r\n");
        aud_dac.buf_len = 0;
    }
    else {
        aud_dac.buf_len = buf_len;
    }
    aud_dac.channels = channel;
    aud_dac.freq = freq;
    aud_dac.dma_mode = 1;
    audio_dac_open(&aud_dac);
    audio_dac_ctrl(AUD_DAC_CMD_PLAY,0);
}

uint32_t djy_audio_dac_ctrl(uint32_t cmd, void *param)
{
    if(aud_dac.buf == NULL) return 0;
    return audio_dac_ctrl(cmd, param);
}

uint32_t djy_audio_dac_write(char *user_buf, uint32_t count)
{
    if(aud_dac.buf == NULL) return 0;
    return audio_dac_write(user_buf, count, (uint32_t)&aud_dac);
}

void djy_audio_dac_close(void)
{
    unsigned char *buf_temp = 0;
    int len_temp = 0;
    audio_dac_close();
    if(aud_dac.buf) {
        free(aud_dac.buf);
        aud_dac.buf = NULL;
    }
    memset(&aud_dac,  0,  sizeof(aud_dac));
}
