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
//所属模块: 锁模块
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 管道模块
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2010-10-01
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "stdint.h"

#if 0
//管道控制块
//当读管道时，如果管道中没有足够的数据量，将阻塞在信号量中，写入数据时使管道中的
//数据量超过level，则立即释放信号量。
//写管道时，如果写满了，将阻塞在信号量中，读管道使得管道中的数据量低于level时，
//将释放信号量。
//管道是单线程组件，

//特别注意:管道可能死锁，程序员应该自己注意不要发生死锁。死锁条件为:
//1、请求从管道读的数据量超过管道长度，管道写满后将阻塞，因为读数据量一直得不到
//   满足，故阻塞将持续下去。
//2、整体模式下，请求的数据量超过(管道长度-最后一次整体写入数据量)，将出现和1一
//   样的效果。
 struct PipePCB
{
    struct RingBuf pipe_buf;               //环形发送缓冲区.
    struct SemaphoreLCB *pipe_write_semp;  //写缓冲区锁
    struct SemaphoreLCB *pipe_read_semp;   //读缓冲区锁
    u32    write_level;                     //写触发水平
    u32    read_level;                      //读触发水平
    bool_t whole_mode;          //整体模式，true=确保每次写入完整记录，比如要写
                                //入10个字节，但pipe_buf中只有9个空位时，将阻塞
                                //false=先写入9个，然后阻塞
};


static struct DjyDevice * s_pPipeParentDevice;
#if(CN_CFG_PIPES_LIMIT != 0)
static struct PipePCB s_tPipeMemBlock[CN_CFG_PIPES_LIMIT];
#endif
static struct MemCellPool *s_ptPipePool;  //信号量结构内存池头指针

//----初始化pipe模块模块-------------------------------------------------------
//功能：在"dev"设备下创建"pipe"设备，作为后续创建pipe设备的父设备，这是一个哑
//      设备，无任何具体的功能。
//参数：para，调用者传入的参数，本模块未使用
//返回：1=成功，0=失败
//-----------------------------------------------------------------------------
ptu32_t Pipe_ModuleInit(ptu32_t para)
{
    static struct MemCellPool pipe_pool;

    para = para;        //消除编译器告警
    s_pPipeParentDevice = Driver_DevAddDevice( Driver_DevGetRootDevice(),"pipe",
                                 NULL,NULL,      //无信号量保护
                                 (devWriteFunc) DJY_NullFunc ,
                                 (devReadFunc ) DJY_NullFunc,
                                 (devCtrlFunc ) DJY_NullFunc ,
                                 (devWriteFunc ) DJY_NullFunc ,
                                 (devReadFunc  ) DJY_NullFunc ,
                                 (devCtrlFunc  ) DJY_NullFunc,
                                 (ptu32_t)NULL   //key设备私有数据
                                 );
    if(s_pPipeParentDevice == NULL)
        return 0;

    //初始化pipe控制块内存池
#if(CN_CFG_PIPES_LIMIT != 0)
    s_ptPipePool = Mb_CreatePool_r( &pipe_pool,
                                    (void*)s_tPipeMemBlock,
                                    CN_CFG_PIPES_LIMIT,
                                    sizeof(struct PipePCB),
                                    0,0,
                                    "pipe控制块池");
#else
    s_ptPipePool = Mb_CreatePool_r( &pipe_pool,
                                    NULL,
                                    CN_CFG_PIPES_LIMIT,
                                    sizeof(struct PipePCB),
                                    0,0,
                                    "pipe控制块池");
#endif
    return 1;
}


//----创建一个管道-----------------------------------------------------------
//功能：创建一个管道
//参数：buf,管道使用的缓冲区，须由使用者分配提供。
//      size，缓冲区大小
//      whole，true=一次性操作，即要求读写操作一次性完成，false=非一次性操作，
//          写入时，有多少空位写入多少，读取时，管道中有多少数据读取多少
//      name，管道的名字，所指向的字符串内存区不能是局部变量
//返回：新建立的信号量指针
//-----------------------------------------------------------------------------
struct DjyDevice * Pipe_Create(u8* buf,u32 size,bool_t whole,
                             u32 write_level,u32 read_level,char *name)
{
    struct PipePCB *pipe;
    struct SemaphoreLCB *read_dev_semp,*write_dev_semp;
    struct DjyDevice * pipe_device;

    pipe = (struct PipePCB *)Mb_Malloc(s_ptPipePool, 0);
    if(pipe == NULL)
        goto exit_from_malloc_PCB;
    //初始处于无信号状态，须读操作释放才处于有信号状态
    pipe->pipe_write_semp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,name);
    if(pipe->pipe_write_semp == NULL)
        goto exit_from_pipe_write_semp;
    //初始处于无信号状态，须写操作释放才处于有信号状态
    pipe->pipe_read_semp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,name);
    if(pipe->pipe_read_semp == NULL)
        goto exit_from_pipe_read_semp;
    read_dev_semp = Lock_SempCreate(1,1,CN_BLOCK_FIFO,NULL);
    if(read_dev_semp == NULL)
        goto exit_from_dev_read_semp;
    write_dev_semp = Lock_SempCreate(1,1,CN_BLOCK_FIFO,NULL);
    if(write_dev_semp == NULL)
        goto exit_from_dev_write_semp;
    pipe_device = Driver_DevAddDevice(s_pPipeParentDevice,
                                 name,
                                 read_dev_semp,write_dev_semp,
                                 NULL,
                                 Pipe_DevRead,
                                 Pipe_DevCtrl,
                                 Pipe_DevWrite,
                                 NULL,
                                 Pipe_DevCtrl,
                                 (ptu32_t)pipe       );
    if(pipe_device == NULL)
        goto exit_from_create_device;

    Ring_Init(&pipe->pipe_buf,buf,size);
    pipe->write_level = write_level;
    pipe->read_level = read_level;
    pipe->whole_mode = whole;
    return pipe_device;

exit_from_create_device:
    Lock_SempDelete(write_dev_semp);
exit_from_dev_write_semp:
    Lock_SempDelete(read_dev_semp);
exit_from_dev_read_semp:
    Lock_SempDelete(pipe->pipe_read_semp);
exit_from_pipe_read_semp:
    Lock_SempDelete(pipe->pipe_write_semp);
exit_from_pipe_write_semp:
    Mb_Free(s_ptPipePool,pipe);
exit_from_malloc_PCB:
    return false;
}

//----读管道-------------------------------------------------------------------
//功能: 从管道中读数据
//参数: pipe_dev，目标管道设备
//      res，保留参数
//      dst_buf，读出数据存放地址
//      len，读出长度
//返回: 总是返回len
//-----------------------------------------------------------------------------
ptu32_t Pipe_DevRead(struct DjyDevice * pipe_dev,ptu32_t dst_buf,
                                        ptu32_t len,u32 timeout)
{
    struct PipePCB *pipe;
    u32 completed=0,temp;
    pipe = (struct PipePCB *)Driver_DevGetMyTag(pipe_dev);
    if(len > Ring_Capacity(&pipe->pipe_buf))    //一次读取数据不能超过管道容量
        return 0;
    temp = Ring_Check(&pipe->pipe_buf);
    while(1)
    {
        completed += Ring_Read(&pipe->pipe_buf,(u8*)(dst_buf+completed),
                               (u32)len-completed);
        if(completed < (u32)len)    //管道中的数据量不足
        {
            Lock_SempPend(pipe->pipe_read_semp,timeout);
        }else
        {
            break;
        }
    }
    //如果读之前超过写入门槛且读之后低于写入门槛，post信号量
    if( (temp >= pipe->write_level)
          &&((temp - completed) < pipe->write_level))
    {
        Lock_SempPost(pipe->pipe_write_semp);
    }
    return len;
}

//----写管道-------------------------------------------------------------------
//功能: 把数据写入管道中
//参数: pipe_dev，目标管道设备
//      src_buf，写入数据存放地址
//      res，保留参数
//      len，写入长度
//返回: 实际写入数据量
//-----------------------------------------------------------------------------
u32 Pipe_DevWrite(struct DjyDevice * pipe_dev,ptu32_t src_buf,
                            ptu32_t len,u32 timeout)
{
    struct PipePCB *pipe;
    u32 completed = 0, temp;
    pipe = (struct PipePCB *)Driver_DevGetMyTag(pipe_dev);
    temp = Ring_Check(&pipe->pipe_buf);
    while(1)
    {
        if(pipe->whole_mode == true)        //整体模式
        {
            //管道中的剩余空间不足
            if((Ring_Capacity(&pipe->pipe_buf)-Ring_Check(&pipe->pipe_buf))
                        < (u32)len)
            {
                Lock_SempPend(pipe->pipe_write_semp,timeout);
            }else
            {
                Ring_Write(&pipe->pipe_buf,(u8*)src_buf,(u32)len);
                break;
            }
        }else
        {
            completed +=Ring_Write(&pipe->pipe_buf,(u8*)src_buf,len -completed);
            if(completed >= len)
                break;
        }
    }

    //如果写之前不到读取门槛且读之后超过写入门槛，post信号量
    if( (temp < pipe->read_level)
          &&((temp + completed) >= pipe->read_level))
    {
        Lock_SempPost(pipe->pipe_read_semp);
    }
    return completed;
}

//----控制管道-----------------------------------------------------------------
//功能: 空函数
//-----------------------------------------------------------------------------
ptu32_t Pipe_DevCtrl(struct DjyDevice * pipe_dev,u32 cmd,
                        u32 data1,u32 data2)
{
    return 0;
}
//----删除一个管道-----------------------------------------------------------
//功能：删除一个管道
//参数：mutex，被删除的管道设备
//返回：无
//-----------------------------------------------------------------------------
bool_t Pipe_Delete(struct DjyDevice * pipe_dev)
{
    struct PipePCB *pipe;
    struct SemaphoreLCB *pipe_left_semp,*pipe_right_semp;
    pipe = (struct PipePCB *)Driver_DevGetMyTag(pipe_dev);
    pipe_left_semp = pipe_dev->left_semp;
    pipe_right_semp = pipe_dev->right_semp;
    if(Driver_DevDeleteDevice(pipe_dev))
    {
        Lock_SempDelete(pipe_left_semp);
        Lock_SempDelete(pipe_right_semp);
        Lock_SempDelete(pipe->pipe_read_semp);
        Lock_SempDelete(pipe->pipe_write_semp);
        Mb_Free(s_ptPipePool,pipe);
        return true;
    }else
        return false;
}

//----查询管道容量-----------------------------------------------------------
//功能：查询一个管道设备的容量，
//参数：pipe_dev，被查询的管道句柄
//返回：管道容量
//-----------------------------------------------------------------------------
u32 Pipe_Capacity(struct DjyDevice * pipe_dev)
{
    struct PipePCB *pipe;
    pipe = (struct PipePCB *)Driver_DevGetMyTag(pipe_dev);
    return Ring_Capacity(&pipe->pipe_buf);
}

//----查询管道中数据量---------------------------------------------------------
//功能：查询一个管道设备中的数据量，
//参数：pipe_dev，被查询的管道句柄
//返回：管道中的数据量
//-----------------------------------------------------------------------------
u32 Pipe_Check(struct DjyDevice * pipe_dev)
{
    struct PipePCB *pipe;
    pipe = (struct PipePCB *)Driver_DevGetMyTag(pipe_dev);
    return Ring_Check(&pipe->pipe_buf);
}

#endif
