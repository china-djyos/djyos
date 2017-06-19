//----------------------------------------------------
//Copyright (C), 2004-2011,  ������.
//��Ȩ���� (C), 2004-2011,   ������.
//����ģ��:windowsģ���ַ�����ģ��
//����:  ������
//�汾��V1.0.0
//�ļ�����: ��ȡwindows�����������룬��Ϊdjyos���ַ���������豸����djyos�����
//          �ն˵��ַ������windows��cmd����
//����˵��:
//�޶���ʷ:
// 1. ����: 2012-10-09
//   ����:  ������
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include <windows.h>
#include "stdint.h"
#include "driver.h"
#include "ring.h"
#include "cpu_peri.h"
#include "stdio.h"
#include "djyos.h"
#include "int.h"

#define cmd_buf_len  1024

static struct MultiplexObjectCB * pMultiplexCmd;      //��·����Ŀ�����ͷָ��
static u32 MplCmdStatus;
HANDLE win32_scanf;

char cmd_ptcl_recv_buf[cmd_buf_len];
//static struct DjyDevice * pg_cmd_hdl;
static struct RingBuf s_tRecvRingBuf;           //���ν��ջ�����.
static struct SemaphoreLCB *s_ptRecvRingBufSemp;
char scanf_buf[cmd_buf_len-2];
HANDLE hStdOut, hNewScreenBuffer,hStdIn;

u16 u16g_evtt_right_write;      //�豸���ֱ�д�󵯳����¼�����

u32 cmd_int(ptu32_t uart_int_line)
{
    u32 len;
    len = strlen(scanf_buf);
    //copy����Ӳ����������Э�黺����
    Ring_Write(&s_tRecvRingBuf, (u8*)scanf_buf,len);
    Lock_SempPost(s_ptRecvRingBufSemp);
    MplCmdStatus |= CN_MULTIPLEX_SENSINGBIT_READ;
    Multiplex_Set(pMultiplexCmd,MplCmdStatus);
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
/*        do
        {
//            scanf("%c",&ch);
            r =ReadConsoleA( hStdIn, // screen buffer to write to
                            &scanf_buf,        // buffer to copy from
                            256,     // col-row size of chiBuffer
                            &num,    // top left src cell in chiBuffer
                            NULL);  // dest. screen buffer rectangle
            if(num != 0)
            {
                scanf_buf[len] = ch;
                len++;
            }
            if(r == 0)
                r = GetLastError();
        }while(ch != '\r');
        scanf_buf[len] = '\0';
        len = 0;
        Int_TapLine(cn_int_line_cmd);
*/    }
}

//----��ʼ���������豸---------------------------------------------------------
//����: ����
//����: ��
//����: ������
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_Cmd(ptu32_t para)
{

    struct DjyDevice * cmd_dev;


    pMultiplexCmd = NULL;
    MplCmdStatus = CN_MULTIPLEX_SENSINGBIT_WRITE;//��ʼʱ��д���ɶ�
    Ring_Init(  &s_tRecvRingBuf,
                (u8 *)cmd_ptcl_recv_buf,
                cmd_buf_len);
    s_ptRecvRingBufSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,NULL);
    //���½���windows �ն������豸
    cmd_dev = Driver_DeviceCreate( NULL,"windows_cmd",
                                NULL,NULL,
                               (fntDevWrite) cmd_DriverWrite,
                               (fntDevRead ) cmd_DriverRead,
                               (fntDevCtrl ) cmd_DriverCtrl,
                               cmd_MultiplexAdd,
                               0
                               );
    if((cmd_dev == NULL) || (s_ptRecvRingBufSemp == NULL))
        goto exit_from_add_device;
//    pg_cmd_hdl = Driver_OpenDevice("windows_cmd",O_RDWR,0);      //�����־��
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
    Driver_DeleteDevice(cmd_dev);
    return 0;
}

bool_t cmd_MultiplexAdd (ptu32_t PrivateTag,
                         struct MultiplexSetsCB *MultiplexSets,
                         u32 DevAlias,
                         u32 SensingBit)
{
    bool_t result=false;

    result = Multiplex_AddObject(MultiplexSets,
                            &pMultiplexCmd,
                            MplCmdStatus,
                            (ptu32_t)DevAlias,SensingBit);
    return result;
}


//----uart�豸����д����-------------------------------------------------------
//����: ������д��uart�豸,һ����uart��ʹ���ߵ���.
//      1.������д�봮��serial_DCB_t�Ļ��η��ͻ�������.
//      2.�������Ӳ������,�������д��Ӳ�����ͻ�����,����������,���򲻲���
//����: uart_dev,���������豸������ָ��.
//      src_buf,����Դָ��
//      len,Ҫ���͵����г���
//����: ʵ��д�뻷�λ��������ַ���
//-----------------------------------------------------------------------------
ptu32_t cmd_DriverWrite(ptu32_t PrivateTag,u8 *buf,
                         u32 len,u32 offset,bool_t BlockOption,u32 timeout)
{
    u32 sended;

    if((len==0) || (buf == 0))
        return 0;

    sended = WriteConsole(   hStdOut, // screen buffer to write to
                    buf,        // buffer to copy from
                    len,     // col-row size of chiBuffer
                    NULL,    // top left src cell in chiBuffer
                    NULL);  // dest. screen buffer rectangle
//    for(sended = 0; sended < len; sended++)
//        putchar(buf[sended]);
    return sended;
}

//----�������豸������------------------------------------------------------
//����: ��ȡ�������豸���ջ��λ�����������
//����: PrivateTag,���������豸˽�б�ǩ��.
//      dst_buf,��������
//      len,���볤��,
//����: ʵ�ʶ�������
//----------------------------------------------------------------------------
ptu32_t cmd_DriverRead(ptu32_t PrivateTag,u8 *dst_buf,u32 len,u32 offset,u32 timeout)
{
    uint32_t completed = 0;
    uint32_t ReadLen;
    u32 base_time,rel_timeout=timeout;

    if((len==0) || ((u8*)dst_buf == NULL) )
        return 0;

    base_time = (u32)DjyGetSysTime();
    completed = Ring_Read(&s_tRecvRingBuf,(uint8_t*)dst_buf,len);
    if(completed < len)    //�����������ݲ�������ȴ�����
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
                //ÿ��pend��ʱ��Ҫ�ݼ�
                rel_timeout = (u32)DjyGetSysTime() - base_time;
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
    //���������в��������ݣ������·���ô���״̬��
    if(Ring_Check(&s_tRecvRingBuf) == 0)
    {
        MplCmdStatus &= (~CN_MULTIPLEX_SENSINGBIT_READ);
        Multiplex_Set(pMultiplexCmd,MplCmdStatus);
    }

    return completed;
}

//----�����豸���ƺ���---------------------------------------------------------
//����: �����豸�Ŀ��ƺ���
//����: uart_handle,�������Ĵ����豸ָ��.
//      cmd,��������
//      data,������cmd����
//����: ������.
//-----------------------------------------------------------------------------
ptu32_t cmd_DriverCtrl(ptu32_t PrivateTag,u32 cmd,
                   u32 data1,u32 data2)
{

    switch(cmd)
    {
        case cn_cmd_connect_recv_evtt:
            u16g_evtt_right_write = (uint16_t)data1;
            break;
        case cn_cmd_disconnect_recv_evtt:
            u16g_evtt_right_write = CN_EVTT_ID_INVALID;
            break;
        default: break;
    }
    return 0;
}


