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
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
//所属模块: uart驱动程序(公共接口部分)
//作者:  罗侍田.
//版本：V1.0.0
//文件描述:
//其他说明:
//修订历史:
//1. 日期:2013-11-10
//   作者:  罗侍田.
//   新版本号：1.0.0
//   修改说明: 移植字44b0的1.0.1版
//------------------------------------------------------------------------------
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <objhandle.h>
#include <ring.h>
#include <systime.h>
#include <lock.h>
#include <djyos.h>
#include <multiplex.h>
#include <device.h>
#include <device/djy_uart.h>
#include <int.h>
#include "dbug.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"uart device file"//uart设备系统
//parent:"device file system"   //填写该组件的父组件名字，none表示没有父组件
//attribute:system              //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"device file system","lock","heap","ring buffer and line buffer"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_UART_DEVICE_FILE == false )
//#warning  " uart_device_file  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_UART_DEVICE_FILE    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure


//一般模式状态结构
struct UartGeneralCB
{
    struct Object *Host;
    struct SemaphoreLCB *BlockingSemp;         // 阻塞信号量
//  struct MultiplexObjectCB * pMultiplexUart;      // 多路复用目标对象头指针
    u32 Baud;                                  // 串口当前波特率
    ptu32_t UartPortTag;                       //串口标签
    UartStartSend StartSend;
    UartControl UartCtrl;
    struct MutexLCB *ReadMutex;        //互斥量,控制设备独占式读访问
    struct MutexLCB *WriteMutex;       //互斥量,控制设备独占式写访问
    struct RingBuf SendRingBuf;                //环形发送缓冲区.
    struct RingBuf RecvRingBuf;                //环形接收缓冲区.
    struct SemaphoreLCB *SendRingBufSemp;      //发送缓冲区信号量
    struct SemaphoreLCB *RecvRingBufSemp;      //接收缓冲区信号量
//  u32 RecvRingTrigLevel;      //串口Ring接收触发水平,即收到多少数据时释放信号量
    u32 SendRingTrigLevel;      //发送Ring发送触发水平，当发送环形缓冲区满，将
                                //用SendRingBufSemp阻塞当前线程，直到发送缓冲
                                //区数据量降至SendRingTrigLevel时释放之。
//  u32 MplReadTrigLevel;       //多路复用可读触发水平
//  u32 MplWriteTrigLevel;      //多路复用可写触发水平
};

// 串口状态控制结构
struct UartPollCB
{
    struct Object *Host;
    struct SemaphoreLCB *BlockingSemp;              //阻塞信号量
//  struct MultiplexObjectCB * pMultiplexUart;      //多路复用目标对象头指针
    u32 Baud;                                       //串口当前波特率
    struct MutexLCB *ReadMutex;        //互斥量,控制设备独占式读访问
    struct MutexLCB *WriteMutex;       //互斥量,控制设备独占式写访问
    ptu32_t UartPortTag;                            //串口标签
    UartSendPkg SendPkg;
    UartControl UartCtrl;
    u32 RecvLen;                                    //接收到的数据长度
};

s32 UART_Open(struct objhandle *hdl, u32 Mode,u32 timeout);
s32 UART_AppWrite(struct objhandle *hdl,u8* src_buf,u32 len,u32 offset, u32 timeout);
s32 UART_AppRead(struct objhandle *hdl,u8* dst_buf,u32 len,u32 offset, u32 timeout);
s32 UART_Ctrl(struct objhandle *hdl,u32 cmd,va_list *arg0);
s32 UART_Poll_Open(struct objhandle *hdl, u32 Mode,u32 timeout);
s32 UART_PollAppWrite(struct objhandle *hdl,u8* src_buf,u32 len,u32 offset, u32 timeout);
s32 UART_PollAppRead(struct objhandle *hdl,u8* dst_buf,u32 len,u32 offset, u32 timeout);
s32 UART_PollCtrl(struct objhandle* hdl,u32 cmd,va_list *arg0);
struct UartGeneralCB *UART_InstallGeneral(struct UartParam *Param);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//----串口设备open函数---------------------------------------------------------
//功能：打开串口设备时的回调函数，使文件上下文指向串口对象对应的串口控制块。
//      串口设备文件，是安装串口时创建的，本函数不负责创建文件。
// 参数：Fp，待操作的串口文件指针
//      Mode，打开模式，O_RDONLY、O_WRONLY、O_RDWR
//      timeout，超时时间。
// 返回：0=success，-1=error
// 备注：O_RDONLY、O_WRONLY、O_RDWR 须符合POSIX（2016）的最新定义
//-----------------------------------------------------------------------------
s32 UART_Open(struct objhandle *hdl, u32 Mode, u32 timeout)
{
    struct UartGeneralCB *UGCB;

    if(hdl == NULL)
        return (-1);

    UGCB = (struct UartGeneralCB *)Device_GetDrvTag(Handle2fd(hdl));
    Handle_SetContext(hdl, (ptu32_t)UGCB);  //Fd上下文指向串口控制块struct UartGeneralCB
    if( ! Ring_IsEmpty(&UGCB->RecvRingBuf))
    {
        //文件尚未加入任何多路复用集，直接初始化即可
        Handle_SetMultiplexEvent(hdl, CN_MULTIPLEX_SENSINGBIT_READ);
    }

    if( ! Ring_IsFull(&UGCB->SendRingBuf))
    {
        //文件尚未加入任何多路复用集，直接初始化即可
        Handle_SetMultiplexEvent(hdl, CN_MULTIPLEX_SENSINGBIT_WRITE);
    }
    return 0;
}

//----串口设备APP写函数----------------------------------------------------------
//功能: 写UART设备,由应用程序调用，UART被注册为设备，调用设备操作函数
//      Driver_WriteDevice，最终会调用本函数，函数调用过程:
//      Dev_Write() ---> Dev->Write() ---> UART_AppWrite()
//      该函数实现的功能说明如下:
//      1.若未开调度，则采用直接发送方式，直接发送前需先将缓冲区数据发送完成.
//      2.若开始调度，把数据写入串口uart_UCB的环形发送缓冲区中.
//      3.写入数据后，启动发送数据，启动发送的方式可为中断或轮询，由驱动决定.
//      4.若缓冲区满，则需等待缓冲区中数据降低到触发水平后，再继续写缓冲区.
//      5.有关timeout，每次等待信号量，时间都需递减.
//      6.当所有数据写入缓冲区后，判断是否阻塞发送，若为阻塞，则等待阻塞信号量再返回.
//      7.返回前，将该串口所在的多路利用集的写感应位清除
//参数: UCB,被操作的串口tagUartCB结构体指针数值
//      buf，数据源指针.
//      len，数据量(bytes).
//      offset,偏移量，在串口模块中，此变量无效
//      block_option,阻塞选项，非零为阻塞方式.当block_option为true，函数会等待串口
//         将所有数据发送完成后返回，属于阻塞发送方式；当block_option为false时，函数
//         会等待所有数据被填入串口模块的缓冲区中后，立刻返回，非阻塞发送方式在函数
//         返回时，串口可能仍在发送数据。
//      timeout,超时量(us).
//返回: 实际写入环形缓冲区的字符数
//-----------------------------------------------------------------------------
s32 UART_AppWrite(struct objhandle *hdl, u8* src_buf, u32 len, u32 offset, u32 timeout)
{
    struct UartGeneralCB *UGCB;
    struct Object *UartObj;
    u32 Mode;
    u32 completed = 0,written;
    uint8_t *buf;
    u32 base_time,rel_timeout = timeout;

    if(hdl == NULL)
        return 0;

    UGCB = (struct UartGeneralCB *)Handle_GetContext(hdl);
    Mode = Handle_GetMode(hdl);
    UartObj = Handle_GetHostObj(hdl);
    buf = (uint8_t*)src_buf;
    base_time = (u32)DJY_GetSysTime();

    if(Lock_MutexPend(UGCB->WriteMutex,timeout)==false)
        return completed;

    while(1)
    {
        //先PEND一次信号量，防止事先已经被释放过，发送过程中，发送完成会释放这个
        //信号量，无论block_option值是什么，都会释放。
        Lock_SempPend(UGCB->BlockingSemp,0);
        written = Ring_Write(&UGCB->SendRingBuf,
                            (uint8_t*)buf+completed,len-completed);
        UGCB->StartSend(UGCB->UartPortTag);

        if(written != len-completed)     //缓冲区满，没有送出全部数据
        {
            completed += written;
            if(false == Lock_SempPend(UGCB->SendRingBufSemp,rel_timeout))
                break;
            rel_timeout = (u32)DJY_GetSysTime() - base_time;
            if(rel_timeout >= timeout)
                break;
            else
                rel_timeout = timeout - rel_timeout;
        }
        else // 至此，需发送的数据已经全部copy到串口设备缓冲区
        {
            if(Handle_FlagIsBlockComplete(Mode)) // 需要block complete
            {
                //如果阻塞选项是complete，还要等待发送完成的信号量。
                Lock_SempPend(UGCB->BlockingSemp,rel_timeout);
            }
            completed += written;
            break;
        }
    }

    Lock_MutexPost(UGCB->WriteMutex);
    //对多路复用的操作不允许在 Lock_MutexPost(UGCB->WriteMutex) 之前调用，因为多路复用
    //操作内部也有互斥量，存在死锁隐患；例如 __Multiplex_Set 函数内部调用串口打印。
    if(Ring_IsFull(&UGCB->SendRingBuf))
    {
        OBJ_ClrMultiplexEvent(UartObj, CN_MULTIPLEX_SENSINGBIT_WRITE);
    }
    return completed;
}

//----串口设备APP读函数----------------------------------------------------------
//功能: 读UART设备,由应用程序调用，UART被注册为设备，调用设备操作函数Driver_ReadDevice,
//      实质就是调用了该函数，函数调用过程:
//      Dev_Read() ---> Dev->Read() ---> UART_AppRead()
//      该函数实现的功能说明如下:
//      1.读出缓冲区中数据，若缓冲区中数据足够，则直接返回
//      2.若数据不够，则以缓冲容量的一半为单位，分批次接收数据
//      3.在等待数据前，需先将信号量清除，每次等待时间需从总超时时间扣除
//      4.返回前，将该串口所在的多路利用集的读感应位清除
//参数: UCB,被操作的串口tagUartCB结构体指针数值
//      dst_buf,应用程序提供的接收缓冲区
//      len,读入长度,字节
//      offset,偏移量,在串口模块中，此变量无效
//      timeout,读超时参数,微秒
//返回: 实际读出长度
//------------------------------------------------------------------------------
s32 UART_AppRead(struct objhandle *hdl,u8* dst_buf,u32 len, u32 offset, u32 timeout)
{
    struct UartGeneralCB *UGCB;
    uint32_t completed = 0;
//    s32 Fd;

    if(hdl == NULL)
        return 0;

//    Fd = Handle2fd(hdl);
    UGCB = (struct UartGeneralCB *)Handle_GetContext(hdl);

    if(Lock_MutexPend(UGCB->ReadMutex,timeout)==false)
        return 0;
    if(Ring_IsEmpty(&UGCB->RecvRingBuf))    //缓冲区空，等待接收信号量
    {
        Lock_SempPend(UGCB->RecvRingBufSemp,timeout);

        completed = Ring_Read(&UGCB->RecvRingBuf,(uint8_t*)dst_buf,len);
    }
    else
        completed = Ring_Read(&UGCB->RecvRingBuf,(uint8_t*)dst_buf,len);

    Lock_MutexPost(UGCB->ReadMutex);

    //若缓冲区中不再有数据，清掉多路复用触发状态。
    //对多路复用的操作不允许在 Lock_MutexPost(UGCB->ReadMutex) 之前调用，因为多路复用
    //操作内部也有互斥量，存在死锁隐患；例如 __Multiplex_Set 函数内部调用串口打印。
    if(Ring_IsEmpty(&UGCB->RecvRingBuf))
    {
        //标记1。
        Handle_ClrMultiplexEvent(hdl, CN_MULTIPLEX_SENSINGBIT_READ);
        //若因抢占，在“标记1”处 UART_PortWrite 被调用，本if将成立
        if( ! Ring_IsEmpty(&UGCB->RecvRingBuf))
        {
            Handle_SetMultiplexEvent(hdl, CN_MULTIPLEX_SENSINGBIT_READ);
        }
    }

    return completed;
}

#pragma GCC diagnostic pop

//----串口设备端口写函数-------------------------------------------------------
//功能: 从端口写UART,由底层驱动调用,该函数实现的功能说明如下:
//      1.把数据写入串口uart_UCB的环形接收缓冲区中.
//      2.如果缓冲区原来是空的，则发送信号量以及设置多路复用集合，通知上层取取数
//      3.发生错误时，置位多路复用集错误感应位
//参数: UCB,被操作的串口tagUartCB结构体指针.
//      buf，数据源指针。
//      len，数据量(bytes)
//返回: 实际写入环形缓冲区的字符数
//-----------------------------------------------------------------------------
// ============================================================================
u32 UART_PortWrite(struct UartGeneralCB *pUGCB, u8 *pBuf, u32 dwLen)
{
    u16 recv_bytes;
    u32 check;

    if((!pUGCB) || (dwLen == 0))
        return (0);

    recv_bytes = Ring_Write(&pUGCB->RecvRingBuf, pBuf, dwLen);
    if(recv_bytes != dwLen)
    {
        OBJ_SetMultiplexEvent(pUGCB->Host, CN_UART_BUF_OVER_ERR);
    }

    check = Ring_Check(&pUGCB->RecvRingBuf);
    if(check == recv_bytes)
    {
        OBJ_SetMultiplexEvent(pUGCB->Host, CN_MULTIPLEX_SENSINGBIT_READ);
        Lock_SempPost(pUGCB->RecvRingBufSemp);
    }

    return recv_bytes;
}

//----串口设备端口读函数---------------------------------------------------------
//功能: 从端口读取UART设备,由底层驱动调用,该函数实现的功能说明如下:
//      1.从uart_UCB缓冲区中读取数据到底层驱动提供的dst_buf.
//      2.检查本次从缓冲区中读出数据前后是否跨过发送触发水平，若跨过了发送触发水平，
//        则发送信号量，通知应用层，可以继续写数据到缓冲区.
//      3.达到写触发水平时，置位多路复用集读标志感觉位
//      4.当读不到数据时，即缓冲区中数据为空时，释放阻塞信号量.
//参数: UCB,被操作的串口tagUartCB结构体指针.
//      dst_buf,读缓冲区
//      len,读入长度,
//返回: 实际读出长度
//------------------------------------------------------------------------------
u32 UART_PortRead(struct UartGeneralCB *pUGCB, u8 *pBuf, u32 dwLen)
{
    u32 check, result = 0;

    if(!pUGCB)
        return (0);

    result = (ptu32_t)Ring_Read(&pUGCB->SendRingBuf, pBuf, dwLen);
    check = Ring_Check(&pUGCB->SendRingBuf);
    if((check <= pUGCB->SendRingTrigLevel)
            && (check + result > pUGCB->SendRingTrigLevel))
    {
        Lock_SempPost(pUGCB->SendRingBufSemp);
        OBJ_SetMultiplexEvent(pUGCB->Host, CN_MULTIPLEX_SENSINGBIT_WRITE);
    }
//  if((check <= UGCB->MplWriteTrigLevel)
//          && (check + result > UGCB->MplWriteTrigLevel))
//  {
//      OBJ_SetMultiplexEvent(UartObj, CN_MULTIPLEX_SENSINGBIT_WRITE);
//  }
    if(result == 0)
    {
        Lock_SempPost(pUGCB->BlockingSemp);
    }

    return result;
}

//----串口错误处理函数----------------------------------------------------------
//功能: 错误处理函数，发生错误时调用该函数，配置多路复用集相应的错误标识位
//参数: UCB,被操作的串口tagUartCB结构体指针.
//      ErrNo,错误标识的比特位，该比特位必须是多路复用模块未用到感觉位，（3-30比特）
//返回: 0，错误；1，正确
//------------------------------------------------------------------------------
u32 UART_ErrHandle(struct UartGeneralCB *pUGCB, u32 dwErrNo)
{
    if(!pUGCB)
        return (0);

    OBJ_SetMultiplexEvent(pUGCB->Host, dwErrNo);
    return (1);
}

//----串口设备控制函数---------------------------------------------------------
//功能: 串口设备的控制函数,由应用程序调用，UART被注册为设备，调用设备操作函数
//      Dev_Ctrl，实质就是调用了该函数，函数调用过程:
//      Dev_Ctrl() ---> Dev->dCtrl() ---> UART_Ctrl()
//      该函数实现的功能根据命令字符决定，说明如下:
//      1.启动停止串口，由底层驱动实现
//      2.设置半双工发送或接收状态，由底层驱动实现
//      3.暂停和恢复接收数据，由底层驱动实现
//      4.设置波特率和硬件触发水平，由底层驱动实现
//      5.设置错误弹出事件类型
//      ......
//参数: UCB,被操作的串口tagUartCB结构体指针.
//      cmd,操作类型
//      data,含义依cmd而定
//返回: 无意义.
//-----------------------------------------------------------------------------
s32 UART_Ctrl(struct objhandle* hdl,u32 cmd, va_list *arg0)
{
    struct UartGeneralCB *UGCB;
    ptu32_t result = 0;

    if(hdl == NULL)
        return 0;
    UGCB = (struct UartGeneralCB *)Handle_GetContext(hdl);
    switch(cmd)
    {
        case CN_DEV_CTRL_START:
        case CN_DEV_CTRL_STOP:
        case CN_DEV_CTRL_SHUTDOWN:
        case CN_DEV_CTRL_SLEEP:
        case CN_DEV_CTRL_RESUME:
        case CN_UART_COM_SET:
        case CN_UART_RX_PAUSE:           //暂停接收
        case CN_UART_RX_RESUME:          //恢复接收
        case CN_UART_RECV_HARD_LEVEL:    //设置接收fifo触发水平
        case CN_UART_SEND_HARD_LEVEL:    //设置发送fifo触发水平
        case CN_UART_EN_RTS:             //使能rts流控
        case CN_UART_DIS_RTS:            //禁止rts流控
        case CN_UART_EN_CTS:             //使能cts流控
        case CN_UART_DIS_CTS:            //禁止cts流控
        case CN_UART_DMA_USED:           //使用dma传输
        case CN_UART_DMA_UNUSED:         //禁止dma传输
        case CN_UART_HALF_DUPLEX_SEND:
        case CN_UART_HALF_DUPLEX_RECV:
        case CN_UART_SET_BAUD:          //设置Baud
        {
            UGCB->UartCtrl(UGCB->UartPortTag,cmd,arg0);
            break;
        }
        case CN_UART_SEND_SOFT_LEVEL:   //设置发送软件触发水平
        {
            UGCB->SendRingTrigLevel = va_arg(*arg0,u32);
            break;
        }

        default: break;
    }
    return result;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//----串口设备open函数---------------------------------------------------------
//功能：打开串口设备时的回调函数，使文件上下文指向串口对象对应的串口控制块。
//      串口设备文件，是安装串口时创建的，本函数不负责创建文件。
//参数：hdl，待操作的串口文件指针
//      Mode，打开模式
//      timeout，超时时间。
//返回：0=success，-1=error
//-----------------------------------------------------------------------------
s32 UART_Poll_Open(struct objhandle *hdl, u32 Mode,u32 timeout)
{
    struct UartPollCB *UPCB;

    if(hdl == NULL)
        return (-1);
    UPCB = (struct UartPollCB*)Device_GetDrvTag(Handle2fd(hdl));
    Handle_SetContext(hdl,(ptu32_t)UPCB);  //Fd上下文指向串口控制块struct UartGeneralCB
    if(UPCB->RecvLen != 0)
    {
        //文件尚未加入任何多路复用集，直接初始化即可
        Handle_SetMultiplexEvent(hdl, CN_MULTIPLEX_SENSINGBIT_READ);
    }
    else
    {
        //文件尚未加入任何多路复用集，直接初始化即可
        Handle_SetMultiplexEvent(hdl, CN_MULTIPLEX_SENSINGBIT_READ
                               + CN_MULTIPLEX_SENSINGBIT_WRITE);
    }
    return (0);
}
#pragma GCC diagnostic pop

//----串口设备端口写函数-------------------------------------------------------
//功能:  写出具到端口。不经过环形缓冲区直接写
//参数: UCB,被操作的串口tagUartCB结构体指针.
//      buf，数据源指针。
//      len，数据量(bytes)
//返回: 实际写入的字符数
//-----------------------------------------------------------------------------
s32 UART_PollAppWrite(struct objhandle *hdl, u8* src_buf, u32 len, u32 offset, u32 timeout)
{
    struct UartPollCB *UPCB;
    s32 completed;

    if((hdl == NULL))
        return 0;

    offset = offset;
    UPCB = (struct UartPollCB *)Handle_GetContext(hdl);
    completed = UPCB->SendPkg(UPCB->UartPortTag, src_buf, len);

    Lock_SempPend(UPCB->BlockingSemp,timeout);
    return (completed);
}

//----串口设备端口读函数-------------------------------------------------------
//功能: 直接从端口读取数据不经过环形缓冲区
//参数: UCB,被操作的串口tagUartCB结构体指针.
//      dst_buf,读缓冲区
//      len,读入长度,
//返回: 实际读出长度
//-----------------------------------------------------------------------------

s32 UART_PollAppRead(struct objhandle *hdl, u8* dst_buf, u32 len, u32 offset, u32 timeout)
{
    struct UartPollCB *UPCB;
    s32 RcvLen ;
    bool_t ErrorFlag = false;

    if((hdl == NULL))
        return 0;

    offset = offset;
    UPCB = (struct UartPollCB *)Handle_GetContext(hdl);

    if(Lock_MutexPend(UPCB->ReadMutex,timeout)==false)
        return 0;
//   Lock_SempPend(UPCB->RecvRingBufSemp,timeout);
    //Int_CutTrunk();
    Int_SaveAsynSignal();

    if(UPCB->RecvLen <= len)
    {
        RcvLen = UPCB->RecvLen;
    }
    else
    {
        RcvLen = len;
        ErrorFlag = true;
    }
    //由driver把数据copy给用户，同时清接收缓冲区，poll模式要求用户每次读设备都
    //读完缓冲区中的全部数据。
    fcntl(Handle2fd(hdl), CN_UART_RECV_BUF, dst_buf, RcvLen);
//  UPCB->UartCtrl(UPCB->UartPortTag,CN_UART_RECV_BUF,(u32)dst_buf,RcvLen);

    Handle_ClrMultiplexEvent(hdl, CN_MULTIPLEX_SENSINGBIT_READ);
    UPCB->RecvLen = 0;
    //Int_ContactTrunk();
    Int_RestoreAsynSignal();

    if(ErrorFlag)
        UART_PollPortRead(UPCB);
    Lock_MutexPost(UPCB->ReadMutex);
    return (RcvLen);
}

//----串口设备端口写函数-------------------------------------------------------
//功能: 从端口写UART,由于Poll模式下，接收缓冲区由driver管理，故本函数只是起到多
//      路复用通知作用。
//参数: UCB,被操作的串口tagUartCB结构体指针.
//      len，数据量(bytes)
//返回: 实际写入环形缓冲区的字符数
//-----------------------------------------------------------------------------
u32 UART_PollPortWrite(struct UartPollCB *pUPCB, u32 dwLen)
{
    if(!pUPCB)
        return (0);

    if(pUPCB->RecvLen == 0)
    {
        OBJ_SetMultiplexEvent(pUPCB->Host, CN_MULTIPLEX_SENSINGBIT_READ);
    }

    pUPCB->RecvLen += dwLen;
    return (dwLen);
}

//----串口设备端口读函数---------------------------------------------------------
//功能: 从端口读取UART设备,由底层驱动调用,驱动程序务必在完成发送时才调用本函数。
//      函数并不会真正读取APP发送的数据，只会起到设置多路复用集状态以及阻塞发送
//      的通知作用。
//参数: UCB,被操作的串口tagUartCB结构体指针.
//返回: 0
//------------------------------------------------------------------------------
u32 UART_PollPortRead(struct UartPollCB *pUPCB)
{
    if(!pUPCB)
        return (0);

    OBJ_SetMultiplexEvent(pUPCB->Host, CN_MULTIPLEX_SENSINGBIT_WRITE);
    Lock_SempPost(pUPCB->BlockingSemp);

    return (0);

}

//----串口错误处理函数----------------------------------------------------------
//功能: 错误处理函数，发生错误时调用该函数，配置多路复用集相应的错误标识位
//参数: UCB,被操作的串口tagUartCB结构体指针.
//      ErrNo,错误标识的比特位，该比特位必须是多路复用模块未用到感觉位，（3-30比特）
//返回: 0，错误；1，正确
//------------------------------------------------------------------------------
u32 UART_PollErrHandle(struct UartPollCB *pUPCB, u32 dwErrNo)
{
    if(!pUPCB)
        return (0);

    OBJ_SetMultiplexEvent(pUPCB->Host, dwErrNo);

    return (1);
}

//----串口Poll设备控制函数---------------------------------------------------------
//功能: 串口设备的控制函数,由应用程序调用，UART被注册为设备，调用设备操作函数
//      Driver_CtrlDevice，实质就是调用了该函数，函数调用过程:
//      Dev_Ctrl() ---> Dev->dCtrl() ---> UART_Ctrl()
//      该函数实现的功能根据命令字符决定，说明如下:
//      1.启动停止串口，由底层驱动实现
//      2.设置半双工发送或接收状态，由底层驱动实现
//      3.暂停和恢复接收数据，由底层驱动实现
//      4.设置波特率和硬件触发水平，由底层驱动实现
//      5.设置错误弹出事件类型
//      ......
//参数: UCB,被操作的串口tagUartCB结构体指针.
//      cmd,操作类型
//      data,含义依cmd而定
//返回: 无意义.
//-----------------------------------------------------------------------------
s32 UART_PollCtrl(struct objhandle* hdl,u32 cmd, va_list *arg0)
{
    struct UartPollCB *UPCB;
    s32 result = 0;

    if(hdl == NULL)
        return (0);

    UPCB = (struct UartPollCB *)Handle_GetContext(hdl);
    result = UPCB->UartCtrl(UPCB->UartPortTag,cmd,arg0);

    return (result);
}

//----添加UART设备-------------------------------------------------------------
//功能：添加串口设备到系统设备队列
//      a、初始化环形缓冲区
//      b、创建环形缓冲区信号量和设备互斥量
//      c、将串口添加到设备管理模块，并初始化设备函数指针
// 参数：UCB，串口控制块
//      Param,包含初始化UART所需参数，具体参数请查看tagUartParam结构体
//返回：串口控制块指针，NULL失败
//-----------------------------------------------------------------------------
#if 0
struct Object * __UART_InstallGeneral(struct UartParam *Param)
{
    struct Object * uart_dev;
    struct UartGeneralCB *UGCB;
    struct MutexLCB *uart_mutexR,*uart_mutexT;
    u8 *pRxRingBuf,*pTxRingBuf;


    UGCB = (struct UartGeneralCB *)M_Malloc(sizeof(struct UartGeneralCB),0);
    if(UGCB == NULL)
        goto exit_from_ucb;
    memset(UGCB,0x00,sizeof(struct UartGeneralCB));
    pRxRingBuf = (u8*)M_Malloc(Param->RxRingBufLen,0);
    if(pRxRingBuf == NULL)
        goto exit_from_rx_ring_buf;
    pTxRingBuf = (u8*)M_Malloc(Param->TxRingBufLen,0);
    if(pTxRingBuf == NULL)
        goto exit_from_tx_ring_buf;

    // 建立串口阻塞使用的信号量
    UGCB->BlockingSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"uart blocking");
    if(UGCB->BlockingSemp == NULL)
        goto exit_from_blocking_semp;
    // 保护缓冲区的信号量，使缓冲区中数据量为0时阻塞写入线程，读取线程使缓冲区中
    // 数据降至trigger_level以下时释放信号量，使写入线程解除阻塞
    UGCB->SendRingBufSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"uart tx buf");
    if(UGCB->SendRingBufSemp == NULL)
        goto exit_from_send_buf_semp;
    UGCB->RecvRingBufSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"uart rx buf");
    if(UGCB->RecvRingBufSemp == NULL)
        goto exit_from_recv_buf_semp;
    // 为设备创建互斥量，提供设备的互斥访问
    uart_mutexR = Lock_MutexCreate("uart receive mutex");
    if(uart_mutexR == NULL)
        goto exit_from_mutexR;
    uart_mutexT = Lock_MutexCreate("uart send mutex");
    if(uart_mutexT == NULL)
        goto exit_from_mutexT;

//  UCB->WriteMutex = Lock_MutexCreate("UART_WriteMutex");
//  if( NULL == UCB->WriteMutex)
//      goto exit_from_mutexWite;
//  UCB->ReadMutex  = Lock_MutexCreate("UART_ReadMutex");
//  if(NULL == UCB->ReadMutex)
//      goto exit_from_mutexRead;

    UGCB->SendRingTrigLevel  = (Param->TxRingBufLen)>>2;  //默认缓冲触发水平为1/16
//    UCB->RecvRingTrigLevel  = (Param->RxRingBufLen)>>4;
//    UCB->MplReadTrigLevel   = Param->RxRingBufLen + 1;
//  UGCB->MplWriteTrigLevel  = Param->TxRingBufLen + 1;
    UGCB->Baud               = Param->Baud;
    UGCB->UartPortTag        = Param->UartPortTag;
    UGCB->StartSend                  = Param->StartSend;
    UGCB->UartCtrl           = Param->UartCtrl;
//  UGCB->pMultiplexUart     = NULL;                     //初始化时为NULL
//  UGCB->MplUartStatus      = CN_MULTIPLEX_SENSINGBIT_WRITE;//初始时可写不可读
    Ring_Init(&UGCB->SendRingBuf,pTxRingBuf,Param->TxRingBufLen);
    Ring_Init(&UGCB->RecvRingBuf,pRxRingBuf,Param->RxRingBufLen);

    uart_dev = Dev_Create((char*)Param->Name,uart_mutexR,uart_mutexT,
                                    UART_Open,
                                    NULL,
                                    UART_AppWrite,
                                    UART_AppRead,
                                    UART_Ctrl,
                                    (ptu32_t)UGCB
                                   );
    UGCB->Host = uart_dev;
    if(uart_dev == NULL)
        goto exit_from_add_device;

    printk("%s Install General Mode Succeeded!\r\n",Param->Name);
    return uart_dev;
    //如果出现错误，则释放创建的资源，并返回空指针
exit_from_add_device:
//    Lock_MutexDelete(UCB->ReadMutex);
//exit_from_mutexRead:
//    Lock_MutexDelete(UCB->WriteMutex);
//exit_from_mutexWite:
    Lock_MutexDelete(uart_mutexT);
exit_from_mutexT:
    Lock_MutexDelete(uart_mutexR);
exit_from_mutexR:
    Lock_SempDelete(UGCB->RecvRingBufSemp);
exit_from_recv_buf_semp:
    Lock_SempDelete(UGCB->SendRingBufSemp);
exit_from_send_buf_semp:
    Lock_SempDelete(UGCB->BlockingSemp);
exit_from_blocking_semp:
    free(pTxRingBuf);
exit_from_tx_ring_buf:
    free(pRxRingBuf);
exit_from_rx_ring_buf:
    free(UGCB);
exit_from_ucb:
    return NULL;
}
#else
struct UartGeneralCB *UART_InstallGeneral(struct UartParam *Param)
{
    struct Object * uart_dev;
    struct UartGeneralCB *UGCB;
    u8 *pRxRingBuf,*pTxRingBuf;

    UGCB = (struct UartGeneralCB *)M_Malloc(sizeof(struct UartGeneralCB),0);
    if(UGCB == NULL)
        goto exit_from_ucb;

    memset(UGCB,0x00,sizeof(struct UartGeneralCB));
    pRxRingBuf = (u8*)M_Malloc(Param->RxRingBufLen,0);
    if(pRxRingBuf == NULL)
        goto exit_from_rx_ring_buf;

    pTxRingBuf = (u8*)M_Malloc(Param->TxRingBufLen,0);
    if(pTxRingBuf == NULL)
        goto exit_from_tx_ring_buf;

    // 建立串口阻塞使用的信号量
    UGCB->BlockingSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"uart blocking");
    if(UGCB->BlockingSemp == NULL)
        goto exit_from_blocking_semp;

    // 保护缓冲区的信号量，使缓冲区中数据量为0时阻塞写入线程，读取线程使缓冲区中
    // 数据降至trigger_level以下时释放信号量，使写入线程解除阻塞
    UGCB->SendRingBufSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"uart tx buf");
    if(UGCB->SendRingBufSemp == NULL)
        goto exit_from_send_buf_semp;

    UGCB->RecvRingBufSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"uart rx buf");
    if(UGCB->RecvRingBufSemp == NULL)
        goto exit_from_recv_buf_semp;

    // 为设备创建互斥量，提供设备的互斥访问
//  uart_mutexR = Lock_MutexCreate("uart receive mutex");
//  if(uart_mutexR == NULL)
//      goto exit_from_mutexR;
//
//  uart_mutexT = Lock_MutexCreate("uart send mutex");
//  if(uart_mutexT == NULL)
//      goto exit_from_mutexT;

    UGCB->WriteMutex = Lock_MutexCreate("UART_WriteMutex");
    if( NULL == UGCB->WriteMutex)
        goto exit_from_mutexWite;
    UGCB->ReadMutex  = Lock_MutexCreate("UART_ReadMutex");
    if(NULL == UGCB->ReadMutex)
        goto exit_from_mutexRead;

    UGCB->SendRingTrigLevel  = (Param->TxRingBufLen)>>2;  //默认缓冲触发水平为1/16
//    UCB->RecvRingTrigLevel  = (Param->RxRingBufLen)>>4;
//    UCB->MplReadTrigLevel   = Param->RxRingBufLen + 1;
//  UGCB->MplWriteTrigLevel  = Param->TxRingBufLen + 1;
    UGCB->Baud               = Param->Baud;
    UGCB->UartPortTag        = Param->UartPortTag;
    UGCB->StartSend          = Param->StartSend;
    UGCB->UartCtrl           = Param->UartCtrl;
//  UGCB->pMultiplexUart     = NULL;                     //初始化时为NULL
//  UGCB->MplUartStatus      = CN_MULTIPLEX_SENSINGBIT_WRITE;//初始时可写不可读
    Ring_Init(&UGCB->SendRingBuf,pTxRingBuf,Param->TxRingBufLen);
    Ring_Init(&UGCB->RecvRingBuf,pRxRingBuf,Param->RxRingBufLen);

    uart_dev = Device_Create(Param->Name, //uart_mutexR,uart_mutexT,
                          UART_Open,
                          NULL,
                          UART_AppWrite,
                          UART_AppRead,
                          UART_Ctrl,
                          (ptu32_t)UGCB
                                   );
    UGCB->Host = uart_dev;
    if(uart_dev == NULL)
        goto exit_from_add_device;

    info_printf("device","%s initialized in general mode.", Param->Name);
    return (UGCB);

    //如果出现错误，则释放创建的资源，并返回空指针
exit_from_add_device:
    Lock_MutexDelete(UGCB->ReadMutex);
exit_from_mutexRead:
    Lock_MutexDelete(UGCB->WriteMutex);
exit_from_mutexWite:
//    Lock_MutexDelete(uart_mutexT);
//exit_from_mutexT:
//    Lock_MutexDelete(uart_mutexR);
//exit_from_mutexR:
    Lock_SempDelete(UGCB->RecvRingBufSemp);
exit_from_recv_buf_semp:
    Lock_SempDelete(UGCB->SendRingBufSemp);
exit_from_send_buf_semp:
    Lock_SempDelete(UGCB->BlockingSemp);
exit_from_blocking_semp:
    free(pTxRingBuf);
exit_from_tx_ring_buf:
    free(pRxRingBuf);
exit_from_rx_ring_buf:
    free(UGCB);
exit_from_ucb:

    error_printf("device","%s initialized in general mode failed.", Param->Name);
    return NULL;
}
#endif

#if 0
struct Object * __UART_InstallPoll(struct UartParam *Param)
{
    struct Object * uart_dev;
    struct UartPollCB *UPCB;
    struct MutexLCB *uart_mutexR,*uart_mutexT;

    UPCB = (struct UartPollCB *)M_Malloc(sizeof(struct UartPollCB),0);
    if(UPCB == NULL)
        goto exit_from_ucb;
    memset(UPCB,0x00,sizeof(struct UartPollCB));

    //建立串口阻塞使用的信号量
    UPCB->BlockingSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"uart blocking");
    if(UPCB->BlockingSemp == NULL)
        goto exit_from_blocking_semp;
    //为设备创建互斥量，提供设备的互斥访问
    uart_mutexR = Lock_MutexCreate("uart receive mutex");
    if(uart_mutexR == NULL)
        goto exit_from_mutexR;
    uart_mutexT = Lock_MutexCreate("uart send mutex");
    if(uart_mutexT == NULL)
        goto exit_from_mutexT;
    UPCB->Baud               = Param->Baud;
    UPCB->UartPortTag        = Param->UartPortTag;
    UPCB->UartCtrl           = Param->UartCtrl;
//  UPCB->pMultiplexUart     = NULL;                     //初始化时为NULL
//  UPCB->MplUartStatus      = CN_MULTIPLEX_SENSINGBIT_WRITE;//初始时可写不可读

    uart_dev = Dev_Create((char*)Param->Name,uart_mutexR,uart_mutexT,
                                    UART_Poll_Open,
                                    NULL,
                                    UART_PollAppWrite,
                                    UART_PollAppRead,
                                    UART_PollCtrl,
                                    (ptu32_t)UPCB
                                   );
    UPCB->Host = uart_dev;
    if(uart_dev == NULL)
        goto exit_from_add_device;

    printk("%s Install Poll Mode Succeeded!\r\n",Param->Name);

    return uart_dev;
    //如果出现错误，则释放创建的资源，并返回空指针
exit_from_add_device:
    Lock_MutexDelete(uart_mutexT);
exit_from_mutexT:
    Lock_MutexDelete(uart_mutexR);
exit_from_mutexR:
    Lock_SempDelete(UPCB->BlockingSemp);
exit_from_blocking_semp:
    free(UPCB);
exit_from_ucb:
    return NULL;
}
#else
struct UartPollCB *UART_InstallPoll(struct UartParam *Param)
{
    struct Object * uart_dev;
    struct UartPollCB *UPCB;
    struct MutexLCB *uart_mutexR,*uart_mutexT;

    UPCB = (struct UartPollCB *)M_Malloc(sizeof(struct UartPollCB),0);
    if(UPCB == NULL)
        goto exit_from_ucb;

    memset(UPCB,0x00,sizeof(struct UartPollCB));
    //建立串口阻塞使用的信号量
    UPCB->BlockingSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"uart blocking");
    if(UPCB->BlockingSemp == NULL)
        goto exit_from_blocking_semp;

    //为设备创建互斥量，提供设备的互斥访问
    uart_mutexR = Lock_MutexCreate("uart receive mutex");
    if(uart_mutexR == NULL)
        goto exit_from_mutexR;

    uart_mutexT = Lock_MutexCreate("uart send mutex");
    if(uart_mutexT == NULL)
        goto exit_from_mutexT;

    UPCB->Baud               = Param->Baud;
    UPCB->UartPortTag        = Param->UartPortTag;
    UPCB->UartCtrl           = Param->UartCtrl;
//  UPCB->pMultiplexUart     = NULL;                     //初始化时为NULL
//  UPCB->MplUartStatus      = CN_MULTIPLEX_SENSINGBIT_WRITE;//初始时可写不可读

    uart_dev = Device_Create((const char*)Param->Name,// uart_mutexR, uart_mutexT,
                        UART_Poll_Open,
                        NULL,
                        UART_PollAppWrite,
                        UART_PollAppRead,
                        UART_PollCtrl,
                        (ptu32_t)UPCB
                       );
    UPCB->Host = uart_dev;
    if(uart_dev == NULL)
        goto exit_from_add_device;

    info_printf("device","%s initialized in poll mode.", Param->Name);
    return (UPCB);

    //如果出现错误，则释放创建的资源，并返回空指针
exit_from_add_device:
    Lock_MutexDelete(uart_mutexT);
exit_from_mutexT:
    Lock_MutexDelete(uart_mutexR);
exit_from_mutexR:
    Lock_SempDelete(UPCB->BlockingSemp);
exit_from_blocking_semp:
    free(UPCB);
exit_from_ucb:
    error_printf("device","%s initialized in poll mode failed.", Param->Name);
    return (NULL);
}
#endif
#if 0
//----添加UART设备-------------------------------------------------------------
//功能：添加串口设备到系统设备队列
//      a、初始化环形缓冲区
//      b、创建环形缓冲区信号量和设备互斥量
//      c、将串口添加到设备管理模块，并初始化设备函数指针
//参数：UCB，串口控制块
//      Param,包含初始化UART所需参数，具体参数请查看tagUartParam结构体
//返回：串口控制块指针，NULL失败
//-----------------------------------------------------------------------------
struct Object * UART_InstallPort(struct UartParam *Param)
{
    if(Param == NULL)
        return NULL;

    if(Param->mode == CN_UART_POLL)
    {
        return __UART_InstallPoll(Param);
    }
    else
    {
        return __UART_InstallGeneral(Param);
    }
}
#endif
