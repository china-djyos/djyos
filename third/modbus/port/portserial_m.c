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
static int      serial = -1;     //���ھ��
static BOOL     bRxEnabled;         //���ڽ���ʹ��
static BOOL     bTxEnabled;         //���ڷ���ʹ��
static UCHAR    ucBuffer[BUF_SIZE];     //���ͺͽ������ݵ���ʱ������
static int      uiRxBufferPos;          //ucBuffer���������ʱ���������ݵĳ���
static int      uiTxBufferPos;          //ucBuffer�淢������ʱ���������ݵĳ���

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
        Handle_SetTimeOut(fd2Handle(serial), 50 * 1000);        //���������ڵĳ�ʱʱ��
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
        while (read(serial, buf, sizeof(buf)));     //�Ѵ�������ջ�������ݶ����
        uiRxBufferPos = 0;  //����buf������Ϊ0
        bRxEnabled = TRUE;  //ʹ�ܽ���
    }
    else
    {
        bRxEnabled = FALSE;
    }
    if (bEnableTx)
    {
        bTxEnabled = TRUE;  //ʹ�ܷ���
        uiTxBufferPos = 0;  //����buf������Ϊ0
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

// ���������ݵ�buf�С�
// pucBuffer�����ֶ������ݵĻ��棬usNBytes��Ҫ��ȡ���ֽ�����usNBytesRead��ʵ�ʶ������ֽ���
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

// ���ڷ�������
// pucBuffer�������͵����ݻ�������usNBytes�������͵����ݳ���
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
                    /* timeout with no bytes. û�������ݡ�*/
                    break;
                }
                else if( usBytesRead > 0 )
                {
                    for( i = 0; i < usBytesRead; i++ )
                    {
                        /* Call the modbus stack and let him fill the buffers.����modbusջ��������仺������ */
                        // һ���ֽ�һ���ֽ���䵽��ucRTUBuf
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
            // һ���ֽ�һ���ֽ���䵽��ucBuffer
            //����pxMBFrameCBTransmitterEmpty��ʱ������������ucBuffer�У��������bTxEnabled�ͻ���false
            while( bTxEnabled )     
            {
                ( void )pxMBMasterFrameCBTransmitterEmpty(  );
                /* Call the modbus stack to let him fill the buffer. ����modbusջ������仺����*/
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
