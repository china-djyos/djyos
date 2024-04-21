#if 0
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
//component name:"audio device file"//audio设备文件系统
//parent:"device file system"   //填写该组件的父组件名字，none表示没有父组件
//attribute:system              //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"device file system"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_AUDIO_DEVICE_FILE == false )
//#warning  " AUDIO_device_file  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_AUDIO_DEVICE_FILE    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure


//声卡控制块数据结构
struct AudioCB
{
    struct Object *Host;
    u32 Baud;                           // 声卡采样率
    ptu32_t AudioCardTag;               //声卡标签，可用于区别多个声卡
    UartStartSend StartSend;
    UartControl UartCtrl;
    struct MutexLCB *ReadMutex;        //互斥量,控制设备独占式读访问
    struct MutexLCB *WriteMutex;       //互斥量,控制设备独占式写访问
    struct RingBuf SendRingBuf;                //环形发送缓冲区.
    struct RingBuf RecvRingBuf;                //环形接收缓冲区.
    struct SemaphoreLCB *SendRingBufSemp;      //发送缓冲区信号量
    struct SemaphoreLCB *RecvRingBufSemp;      //接收缓冲区信号量
    u32 SendRingTrigLevel;      //发送Ring发送触发水平，当发送环形缓冲区满，将
                                //用SendRingBufSemp阻塞当前线程，直到发送缓冲
                                //区数据量降至SendRingTrigLevel时释放之。
};

s32 AUDIO_Open(struct objhandle *hdl, u32 Mode,u32 timeout);
s32 AUDIO_AppWrite(struct objhandle *hdl,u8* src_buf,u32 len,u32 offset, u32 timeout);
s32 AUDIO_Ctrl(struct objhandle *hdl,u32 cmd,va_list *arg0);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//----声卡设备open函数---------------------------------------------------------
//功能：打开声卡设备时的回调函数，使文件上下文指向声卡对象对应的声卡控制块。
//      声卡设备文件，是安装声卡时创建的，本函数不负责创建文件。
// 参数：Fp，待操作的声卡文件指针
//      Mode，打开模式，O_RDONLY、O_WRONLY、O_RDWR
//      timeout，超时时间。
// 返回：0=success，-1=error
// 备注：O_RDONLY、O_WRONLY、O_RDWR 须符合POSIX（2016）的最新定义
//-----------------------------------------------------------------------------
s32 AUDIO_Open(struct objhandle *hdl, u32 Mode, u32 timeout)
{
    struct AudioCB *ACB;

    if(hdl == NULL)
        return (-1);

    ACB = (struct AudioCB *)Device_GetDrvTag(Handle2fd(hdl));
    Handle_SetContext(hdl, (ptu32_t)ACB);  //Fd上下文指向声卡控制块struct AudioCB
    if( ! Ring_IsEmpty(&ACB->RecvRingBuf))
    {
        //文件尚未加入任何多路复用集，直接初始化即可
        Handle_SetMultiplexEvent(hdl, CN_MULTIPLEX_SENSINGBIT_READ);
    }

    if( ! Ring_IsFull(&ACB->SendRingBuf))
    {
        //文件尚未加入任何多路复用集，直接初始化即可
        Handle_SetMultiplexEvent(hdl, CN_MULTIPLEX_SENSINGBIT_WRITE);
    }
    return 0;
}

//----声卡设备APP写函数----------------------------------------------------------
//功能: 写UART设备,由应用程序调用，UART被注册为设备，调用设备操作函数
//      Driver_WriteDevice，最终会调用本函数，函数调用过程:
//      Dev_Write() ---> Dev->Write() ---> AUDIO_AppWrite()
//      该函数实现的功能说明如下:
//      1.若未开调度，则采用直接发送方式，直接发送前需先将缓冲区数据发送完成.
//      2.若开始调度，把数据写入声卡AUDIO_UCB的环形发送缓冲区中.
//      3.写入数据后，启动发送数据，启动发送的方式可为中断或轮询，由驱动决定.
//      4.若缓冲区满，则需等待缓冲区中数据降低到触发水平后，再继续写缓冲区.
//      5.有关timeout，每次等待信号量，时间都需递减.
//      6.当所有数据写入缓冲区后，判断是否阻塞发送，若为阻塞，则等待阻塞信号量再返回.
//      7.返回前，将该声卡所在的多路利用集的写感应位清除
//参数: UCB,被操作的声卡tagUartCB结构体指针数值
//      buf，数据源指针.
//      len，数据量(bytes).
//      offset,偏移量，在声卡模块中，此变量无效
//      block_option,阻塞选项，非零为阻塞方式.当block_option为true，函数会等待声卡
//         将所有数据发送完成后返回，属于阻塞发送方式；当block_option为false时，函数
//         会等待所有数据被填入声卡模块的缓冲区中后，立刻返回，非阻塞发送方式在函数
//         返回时，声卡可能仍在发送数据。
//      timeout,超时量(us).
//返回: 实际写入环形缓冲区的字符数
//-----------------------------------------------------------------------------
s32 AUDIO_AppWrite(struct objhandle *hdl, u8* src_buf, u32 len, u32 offset, u32 timeout)
{
    struct AudioCB *ACB;
    struct Object *AudioObj;
    u32 Mode;
    u32 completed = 0,written;
    uint8_t *buf;
    u32 base_time,rel_timeout = timeout;

    if(hdl == NULL)
        return 0;

    ACB = (struct AudioCB *)Handle_GetContext(hdl);
    Mode = Handle_GetMode(hdl);
    AudioObj = Handle_GetHostObj(hdl);
    buf = (uint8_t*)src_buf;
    base_time = (u32)DJY_GetSysTime();

    if(Lock_MutexPend(ACB->WriteMutex,timeout)==false)
        return completed;

    while(1)
    {
        //先PEND一次信号量，防止事先已经被释放过，发送过程中，发送完成会释放这个
        //信号量，无论block_option值是什么，都会释放。
        Lock_SempPend(ACB->BlockingSemp,0);
        written = Ring_Write(&ACB->SendRingBuf,
                            (uint8_t*)buf+completed,len-completed);
        ACB->StartSend(ACB->AudioCardTag);

        if(written != len-completed)     //缓冲区满，没有送出全部数据
        {
            completed += written;
            if(false == Lock_SempPend(ACB->SendRingBufSemp,rel_timeout))
                break;
            rel_timeout = (u32)DJY_GetSysTime() - base_time;
            if(rel_timeout >= timeout)
                break;
            else
                rel_timeout = timeout - rel_timeout;
        }
        else // 至此，需发送的数据已经全部copy到声卡设备缓冲区
        {
            if(Handle_FlagIsBlockComplete(Mode)) // 需要block complete
            {
                //如果阻塞选项是complete，还要等待发送完成的信号量。
                Lock_SempPend(ACB->BlockingSemp,rel_timeout);
            }
            completed += written;
            break;
        }
    }

    Lock_MutexPost(ACB->WriteMutex);
    //对多路复用的操作不允许在 Lock_MutexPost(ACB->WriteMutex) 之前调用，因为多路复用
    //操作内部也有互斥量，存在死锁隐患；例如 __Multiplex_Set 函数内部调用声卡打印。
    if(Ring_IsFull(&ACB->SendRingBuf))
    {
        OBJ_ClrMultiplexEvent(AudioObj, CN_MULTIPLEX_SENSINGBIT_WRITE);
    }
    return completed;
}

#pragma GCC diagnostic pop
//----声卡错误处理函数----------------------------------------------------------
//功能: 错误处理函数，发生错误时调用该函数，配置多路复用集相应的错误标识位
//参数: UCB,被操作的声卡tagUartCB结构体指针.
//      ErrNo,错误标识的比特位，该比特位必须是多路复用模块未用到感觉位，（3-30比特）
//返回: 0，错误；1，正确
//------------------------------------------------------------------------------
u32 AUDIO_ErrHandle(struct AudioCB *pUGCB, u32 dwErrNo)
{
    if(!pUGCB)
        return (0);

    OBJ_SetMultiplexEvent(pUGCB->Host, dwErrNo);
    return (1);
}

//----声卡设备控制函数---------------------------------------------------------
//功能: 声卡设备的控制函数,由应用程序调用，UART被注册为设备，调用设备操作函数
//      Dev_Ctrl，实质就是调用了该函数，函数调用过程:
//      Dev_Ctrl() ---> Dev->dCtrl() ---> AUDIO_Ctrl()
//      该函数实现的功能根据命令字符决定，说明如下:
//      1.启动停止声卡，由底层驱动实现
//      2.设置半双工发送或接收状态，由底层驱动实现
//      3.暂停和恢复接收数据，由底层驱动实现
//      4.设置波特率和硬件触发水平，由底层驱动实现
//      5.设置错误弹出事件类型
//      ......
//参数: UCB,被操作的声卡tagUartCB结构体指针.
//      cmd,操作类型
//      data,含义依cmd而定
//返回: 无意义.
//-----------------------------------------------------------------------------
s32 AUDIO_Ctrl(struct objhandle* hdl,u32 cmd, va_list *arg0)
{
    struct AudioCB *ACB;
    ptu32_t result = 0;

    if(hdl == NULL)
        return 0;
    ACB = (struct AudioCB *)Handle_GetContext(hdl);
    switch(cmd)
    {
        case CN_DEV_CTRL_START:
        case CN_DEV_CTRL_STOP:
        case CN_DEV_CTRL_SHUTDOWN:
        case CN_DEV_CTRL_SLEEP:
        case CN_DEV_CTRL_RESUME:
        case CN_AUDIO_COM_SET:
        case CN_AUDIO_RX_PAUSE:           //暂停接收
        case CN_AUDIO_RX_RESUME:          //恢复接收
        case CN_AUDIO_RECV_HARD_LEVEL:    //设置接收fifo触发水平
        case CN_AUDIO_SEND_HARD_LEVEL:    //设置发送fifo触发水平
        case CN_AUDIO_EN_RTS:             //使能rts流控
        case CN_AUDIO_DIS_RTS:            //禁止rts流控
        case CN_AUDIO_EN_CTS:             //使能cts流控
        case CN_AUDIO_DIS_CTS:            //禁止cts流控
        case CN_AUDIO_DMA_USED:           //使用dma传输
        case CN_AUDIO_DMA_UNUSED:         //禁止dma传输
        case CN_AUDIO_HALF_DUPLEX_SEND:
        case CN_AUDIO_HALF_DUPLEX_RECV:
        case CN_AUDIO_SET_BAUD:          //设置Baud
        {
            ACB->UartCtrl(ACB->AudioCardTag,cmd,arg0);
            break;
        }
        case CN_AUDIO_SEND_SOFT_LEVEL:   //设置发送软件触发水平
        {
            ACB->SendRingTrigLevel = va_arg(*arg0,u32);
            break;
        }

        default: break;
    }
    return result;
}

//----添加UART设备-------------------------------------------------------------
//功能：添加声卡设备到系统设备队列
//      a、初始化环形缓冲区
//      b、创建环形缓冲区信号量和设备互斥量
//      c、将声卡添加到设备管理模块，并初始化设备函数指针
// 参数：UCB，声卡控制块
//      Param,包含初始化UART所需参数，具体参数请查看tagUartParam结构体
//返回：声卡控制块指针，NULL失败
//-----------------------------------------------------------------------------
struct AudioCB *AUDIO_Install(struct AudioParam *Param)
{
    struct Object * AUDIO_dev;
    struct AudioCB *ACB;
    u8 *pRxRingBuf,*pTxRingBuf;

    ACB = (struct AudioCB *)M_Malloc(sizeof(struct AudioCB),0);
    if(ACB == NULL)
        goto exit_from_ucb;

    memset(ACB,0x00,sizeof(struct AudioCB));
    pRxRingBuf = (u8*)M_Malloc(Param->RxRingBufLen,0);
    if(pRxRingBuf == NULL)
        goto exit_from_rx_ring_buf;

    pTxRingBuf = (u8*)M_Malloc(Param->TxRingBufLen,0);
    if(pTxRingBuf == NULL)
        goto exit_from_tx_ring_buf;

    // 建立声卡阻塞使用的信号量
    ACB->BlockingSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"uart blocking");
    if(ACB->BlockingSemp == NULL)
        goto exit_from_blocking_semp;

    // 保护缓冲区的信号量，使缓冲区中数据量为0时阻塞写入线程，读取线程使缓冲区中
    // 数据降至trigger_level以下时释放信号量，使写入线程解除阻塞
    ACB->SendRingBufSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"uart tx buf");
    if(ACB->SendRingBufSemp == NULL)
        goto exit_from_send_buf_semp;

    ACB->RecvRingBufSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"uart rx buf");
    if(ACB->RecvRingBufSemp == NULL)
        goto exit_from_recv_buf_semp;

    // 为设备创建互斥量，提供设备的互斥访问
//  AUDIO_mutexR = Lock_MutexCreate("uart receive mutex");
//  if(AUDIO_mutexR == NULL)
//      goto exit_from_mutexR;
//
//  AUDIO_mutexT = Lock_MutexCreate("uart send mutex");
//  if(AUDIO_mutexT == NULL)
//      goto exit_from_mutexT;

    ACB->WriteMutex = Lock_MutexCreate("AUDIO_WriteMutex");
    if( NULL == ACB->WriteMutex)
        goto exit_from_mutexWite;
    ACB->ReadMutex  = Lock_MutexCreate("AUDIO_ReadMutex");
    if(NULL == ACB->ReadMutex)
        goto exit_from_mutexRead;

    ACB->SendRingTrigLevel  = (Param->TxRingBufLen)>>2;  //默认缓冲触发水平为1/16
//    UCB->RecvRingTrigLevel  = (Param->RxRingBufLen)>>4;
//    UCB->MplReadTrigLevel   = Param->RxRingBufLen + 1;
//  ACB->MplWriteTrigLevel  = Param->TxRingBufLen + 1;
    ACB->Baud               = Param->Baud;
    ACB->AudioCardTag        = Param->AudioCardTag;
    ACB->StartSend          = Param->StartSend;
    ACB->UartCtrl           = Param->UartCtrl;
//  ACB->pMultiplexUart     = NULL;                     //初始化时为NULL
//  ACB->MplUartStatus      = CN_MULTIPLEX_SENSINGBIT_WRITE;//初始时可写不可读
    Ring_Init(&ACB->SendRingBuf,pTxRingBuf,Param->TxRingBufLen);
    Ring_Init(&ACB->RecvRingBuf,pRxRingBuf,Param->RxRingBufLen);

    AUDIO_dev = Device_Create(Param->Name, //AUDIO_mutexR,AUDIO_mutexT,
                          AUDIO_Open,
                          NULL,
                          AUDIO_AppWrite,
                          NULL,
                          AUDIO_Ctrl,
                          (ptu32_t)ACB
                                   );
    ACB->Host = AUDIO_dev;
    if(AUDIO_dev == NULL)
        goto exit_from_add_device;

    info_printf("device","%s initialized in general mode.\r\n", Param->Name);
    return (ACB);

    //如果出现错误，则释放创建的资源，并返回空指针
exit_from_add_device:
    Lock_MutexDelete(ACB->ReadMutex);
exit_from_mutexRead:
    Lock_MutexDelete(ACB->WriteMutex);
exit_from_mutexWite:
//    Lock_MutexDelete(AUDIO_mutexT);
//exit_from_mutexT:
//    Lock_MutexDelete(AUDIO_mutexR);
//exit_from_mutexR:
    Lock_SempDelete(ACB->RecvRingBufSemp);
exit_from_recv_buf_semp:
    Lock_SempDelete(ACB->SendRingBufSemp);
exit_from_send_buf_semp:
    Lock_SempDelete(ACB->BlockingSemp);
exit_from_blocking_semp:
    free(pTxRingBuf);
exit_from_tx_ring_buf:
    free(pRxRingBuf);
exit_from_rx_ring_buf:
    free(ACB);
exit_from_ucb:

    error_printf("device","%s initialized in general mode failed.\r\n", Param->Name);
    return NULL;
}
#endif
