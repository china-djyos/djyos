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

#include "audio_buf.h"

#include <stddef.h>
#include <systime.h>
#include <djyos.h>
#include <lock.h>
//#define NEW 0

//#if NEW
struct SemaphoreLCB *ring_buf_write = NULL;
struct SemaphoreLCB *ring_buf_read = NULL;
// ============================================================================
// ���ܣ���ȡ���λ������е����ݣ���ָ�������ݳ��ȶ�ȡ
// ������audio�����λ�������ַ��buf: ��������len��Ҫ��ȡ���ݳ��ȣ�timeout����ʱʱ��
// ���أ�0 -- ʧ�ܣ�
// ��ע��
// ============================================================================
s32 AudioRingBufferRead_Len(struct RingBuf *audio, s8 *buf, s32 len, u32 timeout)
{
    s32 ret = 0;

    if((buf) && (audio))
    {
        ret = Ring_Read(audio, (u8 *)buf, len);
        Lock_SempPost(ring_buf_write);  //ִ����һ�ζ��������Ĳ������ͷ��ź�����֪ͨд�������Ĳ����������пռ����д
        if(ret == 0)
        {
            //û�ж������ݣ���������һ���ź������ö�������ͣ���ó�CPU��д�������Ĳ���ִ��
            Lock_SempPend(ring_buf_read, timeout * 1000);
        }
    }

    return ret;
}

// ============================================================================
// ���ܣ������λ�������д���ݣ���ָ�������ݳ���д��
// ������audio�����λ�������ַ��buf: ��������len��Ҫд�����ݳ��ȣ�timeout����ʱʱ�䣻
// ���أ�0 -- ʧ�ܣ�
// ��ע��
// =============================================================================
s32 AudioRingBufferWrite_Len(struct RingBuf *audio, s8 *buf, s32 len, u32 timeout)
{
    s32 ret = 0, write_len = 0, write_num = 0;

    if((buf) && (audio))
    {
        while((write_num++ < 10) && (write_len != len))
        {
            ret = Ring_Write(audio, (u8 *)(buf+write_len), len-write_len);
            write_len += ret;
            Lock_SempPost(ring_buf_read);   //ִ����һ��д�������Ĳ������ͷ��ź�����֪ͨ���������Ĳ��������������ݿ��Զ�
            if((ret == 0) || (write_len != len))
            {
                //�������������ߵ�ǰʣ��ռ䲻��ʱ������һ���ź�������д������ͣ���ó�CPU�����������Ĳ���ִ��
                Lock_SempPend(ring_buf_write, timeout*1000);
            }
        }
        ret = write_len;
    }

    return ret;
}

// ============================================================================
// ���ܣ���ȡ���λ������е����ݣ�����������Ƶ�����ʣ�λ��ͨ������ʱ����ȷ����ȡ�����ݳ���
// ������audio�����λ�������ַ��buf: ��������channels��ͨ������sample_rate��������,
//      bit_wide��λ��time����ȡ����Ƶʱ��(��λms)��timeout����ʱʱ��
// ���أ�0 -- ʧ�ܣ�
// ��ע��
// ============================================================================
s32 AudioRingBufferRead_Time(struct RingBuf *audio, s8 *buf,
                    u8 channels, u32 sample_rate, u8 bit_wide, u32 time, u32 timeout)
{
    s32 ret = 0;
    s32 len;

    if((buf) && (audio) && (time))
    {
        len = channels * sample_rate * (bit_wide / 2);
        len = ((time / 1000) * len) + (len / (1000 / (time % 1000)));
        ret = AudioRingBufferRead_Len(audio, buf, len, timeout);
    }

    return ret;
}

// ============================================================================
// ���ܣ������λ�������д���ݣ�����������Ƶ�����ʣ�λ��ͨ������ʱ����ȷ��д�����ݳ���
// ������audio�����λ�������ַ��buf: ��������channels��ͨ������sample_rate��������,
//      bit_wide��λ��time����ȡ����Ƶʱ��(��λms)��timeout����ʱʱ��
// ���أ�0 -- ʧ�ܣ�
// ��ע��
// ============================================================================
s32 AudioRingBufferWrite_Time(struct RingBuf *audio, s8 *buf,
                    u8 channels, u32 sample_rate, u8 bit_wide, u32 time, u32 timeout)
{
    s32 ret = 0;
    s32 len;

    if((buf) && (audio) && (time))
    {
        len = channels * sample_rate * (bit_wide / 2);
        len = ((time / 1000) * len) + (len / (1000 / (time % 1000)));
        ret = AudioRingBufferWrite_Len(audio, buf, len, timeout);
    }

    return ret;
}

// ============================================================================
// ���ܣ�audio����������
// ������para������������
// ���أ�0 -- �ɹ���
// ��ע��
// =============================================================================
//s32 AudioBufferCtrl(struct Platform_AudioBufCtrl *ctrl)
//{
//    s32 ret;
//
//    ret = LoopBytesMgrCtrl(ctrl->CtrlMgr, ctrl->opt, ctrl->popt, ctrl->optlen);
//
//    return ret;
//}

// ============================================================================
// ���ܣ�����������
// ���������������ȣ�
// ���أ�����Ļ�������ַ��
// ��ע��
// =============================================================================
struct RingBuf *AudioRingBufferInit(u32 len)
{
    struct RingBuf *buf;

    ring_buf_write = Lock_SempCreate(1, 0, CN_BLOCK_FIFO, "audio_ring_buf_write");
    if(ring_buf_write)
    {
        ring_buf_read = Lock_SempCreate(1, 0, CN_BLOCK_FIFO, "audio_ring_buf_write");
        if(ring_buf_read)
        {
            buf = Ring_Create(len);
            if(buf)
                return buf;
            else
                Lock_SempDelete(ring_buf_read);
        }
        Lock_SempDelete(ring_buf_write);
    }

    return NULL;
}

// ============================================================================
// ���ܣ��������ͷ�
// ������para������������
// ���أ�-1 -- ��������0 -- �ɹ���bit0 = 1��������ע��ʧ�ܣ�
//      bit1 = 1��ring_buf_writeע��ʧ��
//      bit2 = 1: ring_buf_readע��ʧ��
// ��ע��
// =============================================================================
s32 AudioRingBufferDeInit(struct RingBuf *audio)
{
    s32 ret = 0;
    if(audio)
    {
        if(Ring_Destroy(audio) == -1)
            ret |= 1 << 0;
        if(Lock_SempDelete(ring_buf_write) == false)
            ret |= 1 << 1;
        if(Lock_SempDelete(ring_buf_read) == false)
            ret |= 1 << 2;
    }
    else
        ret = -1;
    return ret;
}


// ============================================================================
// ���ܣ�����������
// ������para������������
// ���أ�0 -- �ɹ���-1 -- ʧ�ܣ�
// ��ע��
// =============================================================================
s32 AudioRingBufferReset(struct RingBuf *audio)
{
    if(audio)
    {
        Ring_Clean(audio);
        return 0;
    }
    else
        return -1;
}


// ============================================================================
// ���ܣ���ȡ�������ֽ�����
// ������para������������
// ���أ������������ݳ��ȣ�
// ��ע��
// =============================================================================
u32 AudioRingBufferGetTotals(struct RingBuf *audio)
{
    if(audio)
    {
        return Ring_Check(audio);
    }
    else
        return 0xffffffff;
}

// ============================================================================
// ���ܣ���ȡ����������ֽ���
// ������para������������
// ���أ���������������ݳ��ȣ�
// ��ע��
// =============================================================================
u32 AudioRingBufferGetMax(struct RingBuf *audio)
{
    if(audio)
    {
        return Ring_Capacity(audio);
    }
    else
        return 0xffffffff;
}
