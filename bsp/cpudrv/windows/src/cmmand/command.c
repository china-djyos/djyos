//----------------------------------------------------
//Copyright (C), 2004-2011,  罗侍田.
//版权所有 (C), 2004-2011,   罗侍田.
//所属模块:windows模拟字符输入模块
//作者:  罗侍田
//版本：V1.0.0
//文件描述: 截取windows的命令行输入，作为djyos的字符输入输出设备；把djyos输出到
//          终端的字符输出到windows的cmd窗口
//其他说明:
//修订历史:
// 1. 日期: 2012-10-09
//   作者:  罗侍田
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include <windows.h>
#include "stdint.h"
#include <device.h>
#include "ring.h"
#include "cpu_peri.h"
#include "stdio.h"
#include "djyos.h"
#include "int.h"
#include "multiplex.h"

#define cmd_buf_len  1024

HANDLE win32_scanf;
struct Object * sg_ptCmdDev;

char cmd_ptcl_recv_buf[cmd_buf_len];
//static struct DjyDevice * pg_cmd_hdl;
static struct RingBuf s_tRecvRingBuf;           //环形接收缓冲区.
static struct SemaphoreLCB *s_ptRecvRingBufSemp;
char scanf_buf[cmd_buf_len-2];
HANDLE hStdOut, hNewScreenBuffer,hStdIn;

ptu32_t cmd_DriverWrite(tagOFile *Kfp,u8 *buf,u32 len,u32 timeout);
ptu32_t cmd_DriverRead(tagOFile *Kfp,u8 *buf,u32 len,u32 timeout);
ptu32_t cmd_DriverCtrl(tagOFile *Kfp,u32 cmd,u32 data1,u32 data2);

u32 cmd_int(ptu32_t cmd_int_line)
{
    u32 len;
    len = strlen(scanf_buf);
    //copy整个硬件缓冲区到协议缓冲区
    Ring_Write(&s_tRecvRingBuf, (u8*)scanf_buf,len);
    Lock_SempPost(s_ptRecvRingBufSemp);
    SetObjectAccessStatus(sg_ptCmdDev, CN_MULTIPLEX_SENSINGBIT_READ);
    return 0;
}

u32 WINAPI win32_scanf_pro( LPVOID lpParameter )
{
    u32 num;
    while(1)
    {
        ReadConsoleA( hStdIn, // screen buffer to write to
                        &scanf_buf,        // buffer to copy from
                        256,     // col-row size of chiBuffer
                        (PDWORD)&num,    // top left src cell in chiBuffer
                        NULL);  // dest. screen buffer rectangle
        scanf_buf[num] = '\0';
        Int_TapLine(cn_int_line_cmd);
    }
}

//----命令行设备open函数-------------------------------------------------------
//功能：打开命令行设备时的回调函数，只把文件当前状态设置为可写。文件是安装命令行
//      模块时创建的，本函数不负责创建文件。
//参数：Fp，待操作的串口文件指针
//      Mode，打开模式
//      timeout，超时时间。
//返回：0=success，-1=error
//-----------------------------------------------------------------------------
u32 cmd_Open(tagOFile *Kfp, u32 Mode,u32 timeout)
{
    if(Kfp == NULL)
        return -1;
    of_setmevent(Kfp, CN_MULTIPLEX_SENSINGBIT_WRITE);
    return 0；
}

//----初始化命令行设备---------------------------------------------------------
//功能: 如名
//参数: 无
//返回: 无意义
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_Cmd(ptu32_t para)
{
    Ring_Init(  &s_tRecvRingBuf,
                (u8 *)cmd_ptcl_recv_buf,
                cmd_buf_len);
    s_ptRecvRingBufSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,NULL);
    //以下建立windows 终端输入设备
    sg_ptCmdDev = Device_Create("windows_cmd",
                                NULL,NULL,cmd_Open,NULL,
                               (fnDevWrite) cmd_DriverWrite,
                               (fnDevRead ) cmd_DriverRead,
                               (fnDevCtrl ) cmd_DriverCtrl,
                               0
                               );
    if((sg_ptCmdDev == NULL) || (s_ptRecvRingBufSemp == NULL))
        goto exit_from_add_device;
//    pg_cmd_hdl = Device_Open("windows_cmd",O_RDWR,0);      //打开右手句柄
    Int_Register(cn_int_line_cmd);
    Int_IsrConnect(cn_int_line_cmd,cmd_int);
    Int_SettoAsynSignal(cn_int_line_cmd);
    Int_RestoreAsynLine(cn_int_line_cmd);
    win32_scanf = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)win32_scanf_pro, 0,
                                     0, NULL );
    SetThreadAffinityMask( win32_scanf, 1 );

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    hStdIn = GetStdHandle(STD_INPUT_HANDLE);
/*    hNewScreenBuffer = CreateConsoleScreenBuffer(
                       GENERIC_READ | GENERIC_WRITE,          // read/write access
                       FILE_SHARE_READ | FILE_SHARE_WRITE,    // shared
                       NULL,                    // default security attributes
                       CONSOLE_TEXTMODE_BUFFER, // must be TEXTMODE
                       NULL);                   // reserved; must be NULL
    SetConsoleActiveScreenBuffer(hNewScreenBuffer);
*/
    return 1;

exit_from_add_device:
    Lock_SempDelete(s_ptRecvRingBufSemp);
    dev_Delete(sg_ptCmdDev);
    return 0;
}

//----uart设备左手写函数-------------------------------------------------------
//功能: 从左手写入uart设备,一般由uart的使用者调用.
//      1.把数据写入串口serial_DCB_t的环形发送缓冲区中.
//      2.如果串口硬件空闲,便把数据写入硬件发送缓冲区,并启动发送,否则不操作
//参数: uart_dev,被操作的设备的左手指针.
//      src_buf,数据源指针
//      len,要发送的序列长度
//返回: 实际写入环形缓冲区的字符数
//-----------------------------------------------------------------------------
ptu32_t cmd_DriverWrite(tagOFile *Kfp,u8 *buf,u32 len,u32 timeout)
{
    u32 sended;

    if((len==0) || (buf == 0))
        return 0;

    sended = WriteConsole(   hStdOut, // screen buffer to write to
                    buf,        // buffer to copy from
                    len,     // col-row size of chiBuffer
                    NULL,    // top left src cell in chiBuffer
                    NULL);  // dest. screen buffer rectangle
    return sended;
}

//----命令行设备读函数------------------------------------------------------
//功能: 读取命令行设备接收环形缓冲区的数据
//参数: PrivateTag,被操作的设备私有标签。.
//      dst_buf,读缓冲区
//      len,读入长度,
//返回: 实际读出长度
//----------------------------------------------------------------------------
ptu32_t cmd_DriverRead(tagOFile *Kfp,u8 *dst_buf,u32 len,u32 timeout)
{
    uint32_t completed = 0;
    uint32_t ReadLen;
    u32 base_time,rel_timeout=timeout;
    s32 Fd;

    if((Kfp == NULL)||(len==0) || ((u8*)dst_buf == NULL) )
        return 0;

//    Fd = ofno( Kfp );

    base_time = (u32)DJY_GetSysTime();
    completed = Ring_Read(&s_tRecvRingBuf,(uint8_t*)dst_buf,len);
    if(completed < len)    //缓冲区中数据不够，则等待接收
    {
        ReadLen = len - completed;
        while(1)
        {
            Lock_SempPend(s_ptRecvRingBufSemp,rel_timeout);
            completed += Ring_Read(&s_tRecvRingBuf,
                               ((u8*)dst_buf) + completed,
                               ReadLen);
            if(completed < len)
            {
                //每次pend的时间要递减
                rel_timeout = (u32)DJY_GetSysTime() - base_time;
                if(rel_timeout > timeout)
                    break;
                else
                {
                    rel_timeout = timeout - rel_timeout;
                    ReadLen = len - completed;
                }
            }
            else
                break;
        }
    }
    //若缓冲区中不再有数据，清掉多路复用触发状态。
    if(Ring_IsEmpty(&UGCB->RecvRingBuf))
    {
        // ClrFdAccessStatus(Fd, CN_MULTIPLEX_SENSINGBIT_READ);
        // fcntl(Fd, F_OF_SETACCESS, CN_MULTIPLEX_SENSINGBIT_READ);
        of_setmevent(Kfp, CN_MULTIPLEX_SENSINGBIT_READ);
    }

    return completed;
}

//----串口设备控制函数---------------------------------------------------------
//功能: 串口设备的控制函数
//参数: uart_handle,被操作的串口设备指针.
//      cmd,操作类型
//      data,含义依cmd而定
//返回: 无意义.
//-----------------------------------------------------------------------------
ptu32_t cmd_DriverCtrl(tagOFile *Kfp,u32 cmd,u32 data1,u32 data2)
{
    return 0;
}


