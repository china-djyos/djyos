/*
 * FreeModbus Libary: RT-Thread Port
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial_m.c,v 1.60 2013/08/13 15:07:05 Armink add Master Functions $
 */

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "board.h"
#include <uartctrl.h>
#if MB_MASTER_RTU_ENABLED == 1 || MB_MASTER_ASCII_ENABLED == 1

#if MB_MASTER_ASCII_ENABLED == 1
#define BUF_SIZE    513         /* must hold a complete ASCII frame. */
#else
#define BUF_SIZE    256         /* must hold a complete RTU frame. */
#endif

/* ----------------------- Static variables ---------------------------------*/
static u8 MasterSerialTransStack[0x800];
static int      serial = -1;     //串口句柄
static BOOL     bRxEnabled;         //窗口接收使能
static BOOL     bTxEnabled;         //串口发送使能
static UCHAR    ucBuffer[BUF_SIZE];     //发送和接收数据的临时缓冲区
static int      uiRxBufferPos;          //ucBuffer存接收数据时，接收数据的长度
static int      uiTxBufferPos;          //ucBuffer存发送数据时，发送数据的长度

/* ----------------------- static functions ---------------------------------*/
static ptu32_t MasterSerialTrans(void);

/* ----------------------- Start implementation -----------------------------*/
BOOL xMBMasterPortSerialInit(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits,
        eMBParity eParity)
{
    BOOL            bStatus = false; 
    struct COMParam com;
    char uart_name[20];
    u16 EvttID = 0;

    EvttID = DJY_EvttRegist(EN_INDEPENDENCE, 200, 0, 1, (void *)MasterSerialTrans, 
                            MasterSerialTransStack, sizeof(MasterSerialTransStack), "master trans");
    if (CN_EVTT_ID_INVALID == EvttID)
    {
        error_printf("modbus", "master trans Evtt regist fail\r\n");
        return bStatus;
    }

    snprintf((char *)uart_name, 16, "/dev/UART%d", ucPORT );
    if( ( serial = open((char *)uart_name, O_RDWR | O_NOCTTY ) ) < 0 )
    {
        error_printf("modbus", "open %s fail\r\n", uart_name);
        return bStatus;
    }

    com.BaudRate = ulBaudRate;
    com.DataBits = ucDataBits;
    com.StopBits = CN_UART_STOPBITS_1;
    switch (eParity)
    {
        case MB_PAR_NONE:
            com.Parity = CN_UART_PARITY_NONE;
            break;
        
        case MB_PAR_ODD:
            com.Parity = CN_UART_PARITY_ODD;
            break;

        case MB_PAR_EVEN:
            com.Parity = CN_UART_PARITY_EVEN;
            break;
        
        default:
            break;
    }

    if (-1 == fcntl(serial, CN_UART_COM_SET, &com))
    {
        error_printf("modbus", "set uart %s param fail\r\n", uart_name);
        DJY_EvttUnregist(EvttID);
        close(serial);
    }
    else
    {
        Handle_SetTimeOut(fd2Handle(serial), 50 * 1000);        //读操作串口的超时时间
        DJY_EventPop(EvttID, NULL, 0, (ptu32_t)0, (ptu32_t)0, 0);
        bStatus = true;
    }

    return bStatus;
}

void vMBMasterPortSerialEnable(BOOL bEnableRx, BOOL bEnableTx)
{
    u8 buf[BUF_SIZE];
    if (bEnableRx && bEnableTx)
    {
        error_printf("modbus", "bEnableRx and bEnableTx is ture\r\n");
        return ;
    }


    if (bEnableRx)
    {
        while (read(serial, buf, sizeof(buf)));     //把串口里接收缓存的数据都清掉
        uiRxBufferPos = 0;  //接收buf中数据为0
        bRxEnabled = TRUE;  //使能接收
    }
    else
    {
        bRxEnabled = FALSE;
    }
    if (bEnableTx)
    {
        bTxEnabled = TRUE;  //使能发送
        uiTxBufferPos = 0;  //发送buf中数据为0
    }
    else
    {
        bTxEnabled = FALSE;
    }
}

void vMBMasterPortClose(void)
{
    if( serial != -1 )
    {
        ( void )close( serial );
        serial = -1;
    }
}

BOOL xMBMasterPortSerialPutByte(CHAR ucByte)
{
    if (uiTxBufferPos >= BUF_SIZE) 
    {
        error_printf("modbus", "uiTxBufferPos(%d) >= BUF_SIZE(%d)\r\n", uiTxBufferPos, BUF_SIZE);
        return FALSE;
    }
    ucBuffer[uiTxBufferPos] = ucByte;
    uiTxBufferPos++;
    return TRUE;
}

BOOL xMBMasterPortSerialGetByte(CHAR * pucByte)
{
    if (uiRxBufferPos >= BUF_SIZE) 
    {
        error_printf("modbus", "uiRxBufferPos(%d) >= BUF_SIZE(%d)\r\n", uiRxBufferPos, BUF_SIZE);
        return FALSE;
    }
    *pucByte = ucBuffer[uiRxBufferPos];
    uiRxBufferPos++;
    return TRUE;
}

// 读串口数据到buf中。
// pucBuffer：保持读到数据的缓存，usNBytes：要读取的字节数，usNBytesRead：实际读到的字节数
BOOL prvbMBMasterPortSerialRead( UCHAR * pucBuffer, USHORT usNBytes, USHORT * usNBytesRead )
{
    BOOL            bResult = TRUE;
    ssize_t         res;
    *usNBytesRead = 0;
    if( ( res = read( serial, pucBuffer, usNBytes ) ) == -1 )
    {
        bResult = FALSE;
    }
    else
    {
        *usNBytesRead = ( USHORT ) res;
    }
    return bResult;
}

// 串口发送数据
// pucBuffer：待发送的数据缓存区，usNBytes：待发送的数据长度
BOOL prvbMBMasterPortSerialWrite( UCHAR * pucBuffer, USHORT usNBytes )
{
    ssize_t         res;
    size_t          left = ( size_t ) usNBytes;
    size_t          done = 0;
    u32 timeout = 3;

    while ((left > 0) && (timeout > 0))
    {
        if( ( res = write( serial, pucBuffer + done, left ) ) == -1 )
        {
            timeout --;
            continue;
        }
        done += res;
        left -= res;
    }
    return left == 0 ? TRUE : FALSE;
}

ptu32_t MasterSerialTrans(void)
{
    USHORT          usBytesRead;
    int             i;
    while (1)
    {
        while( bRxEnabled )
        {
            if( prvbMBMasterPortSerialRead( &ucBuffer[0], BUF_SIZE, &usBytesRead ) )
            {
                if( usBytesRead == 0 )
                {
                    /* timeout with no bytes. 没读到数据。*/
                    break;
                }
                else if( usBytesRead > 0 )
                {
                    for( i = 0; i < usBytesRead; i++ )
                    {
                        /* Call the modbus stack and let him fill the buffers.调用modbus栈并让他填充缓冲区。 */
                        // 一个字节一个字节填充到中ucRTUBuf
                        ( void )pxMBMasterFrameCBByteReceived(  );
                    }
                    uiRxBufferPos = 0;
                }
            }
            else
            {
                error_printf("modbus", "read failed on serial device\r\n");
            }
        }

        if( bTxEnabled )
        {
            // 一个字节一个字节填充到中ucBuffer
            //调用pxMBFrameCBTransmitterEmpty的时候会把数据填充操ucBuffer中，填充完了bTxEnabled就会变成false
            while( bTxEnabled )     
            {
                ( void )pxMBMasterFrameCBTransmitterEmpty(  );
                /* Call the modbus stack to let him fill the buffer. 调用modbus栈让他填充缓冲区*/
            }
            if( !prvbMBMasterPortSerialWrite( &ucBuffer[0], uiTxBufferPos ) )
            {
                error_printf("modbus", "write failed on serial device\r\n");
            }
        }
    }
    return 0;
}

#endif
