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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
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
// 功能：读取环形缓冲区中的数据，按指定的数据长度读取
// 参数：audio：环形缓冲区地址，buf: 缓冲区，len：要读取数据长度，timeout：超时时间
// 返回：0 -- 失败；
// 备注：
// ============================================================================
s32 AudioRingBufferRead_Len(struct RingBuf *audio, s8 *buf, s32 len, u32 timeout)
{
    s32 ret = 0;

    if((buf) && (audio))
    {
        ret = Ring_Read(audio, (u8 *)buf, len);
        Lock_SempPost(ring_buf_write);  //执行完一次读缓冲区的操作，释放信号量，通知写缓冲区的操作，现在有空间可以写
        if(ret == 0)
        {
            //没有读到数据，这里请求一个信号量，让读操作暂停，让出CPU给写缓冲区的操作执行
            Lock_SempPend(ring_buf_read, timeout * 1000);
        }
    }

    return ret;
}

// ============================================================================
// 功能：往环形缓冲区中写数据，按指定的数据长度写入
// 参数：audio：环形缓冲区地址，buf: 缓冲区，len：要写的数据长度，timeout：超时时间；
// 返回：0 -- 失败；
// 备注：
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
            Lock_SempPost(ring_buf_read);   //执行完一次写缓冲区的操作，释放信号量，通知读缓冲区的操作，现在有数据可以读
            if((ret == 0) || (write_len != len))
            {
                //缓冲区以满或者当前剩余空间不足时，请求一个信号量，让写操作暂停，让出CPU给读缓冲区的操作执行
                Lock_SempPend(ring_buf_write, timeout*1000);
            }
        }
        ret = write_len;
    }

    return ret;
}

// ============================================================================
// 功能：读取环形缓冲区中的数据，按给定的音频采样率，位宽，通道数，时间来确定读取的数据长度
// 参数：audio：环形缓冲区地址，buf: 缓冲区，channels：通道数，sample_rate，采样率,
//      bit_wide：位宽；time：读取的音频时间(单位ms)：timeout：超时时间
// 返回：0 -- 失败；
// 备注：
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
// 功能：往环形缓冲区中写数据，按给定的音频采样率，位宽，通道数，时间来确定写的数据长度
// 参数：audio：环形缓冲区地址，buf: 缓冲区，channels：通道数，sample_rate，采样率,
//      bit_wide：位宽；time：读取的音频时间(单位ms)：timeout：超时时间
// 返回：0 -- 失败；
// 备注：
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
// 功能：audio缓冲区配置
// 参数：para：配网参数；
// 返回：0 -- 成功；
// 备注：
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
// 功能：缓冲区申请
// 参数：缓冲区长度；
// 返回：申请的缓冲区地址；
// 备注：
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
// 功能：缓冲区释放
// 参数：para：配网参数；
// 返回：-1 -- 参数错误；0 -- 成功；bit0 = 1：缓冲区注销失败，
//      bit1 = 1：ring_buf_write注销失败
//      bit2 = 1: ring_buf_read注销失败
// 备注：
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
// 功能：缓冲区重置
// 参数：para：配网参数；
// 返回：0 -- 成功；-1 -- 失败；
// 备注：
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
// 功能：获取缓冲区字节总数
// 参数：para：配网参数；
// 返回：缓冲区的数据长度；
// 备注：
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
// 功能：获取缓冲区最大字节数
// 参数：para：配网参数；
// 返回：缓冲区的最大数据长度；
// 备注：
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
