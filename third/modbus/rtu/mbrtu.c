/*
 * FreeModbus Libary: A portable Modbus implementation for Modbus ASCII/RTU.
 * Copyright (c) 2006-2018 Christian Walter <cwalter@embedded-solutions.at>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: $Id: mbrtu.c,v 1.18 2007/09/12 10:15:56 wolti Exp $
 */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbrtu.h"
#include "mbframe.h"

#include "mbcrc.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/
#define MB_SER_PDU_SIZE_MIN     4       /*!< Minimum size of a Modbus RTU frame. */
#define MB_SER_PDU_SIZE_MAX     256     /*!< Maximum size of a Modbus RTU frame. */
#define MB_SER_PDU_SIZE_CRC     2       /*!< Size of CRC field in PDU. */
#define MB_SER_PDU_ADDR_OFF     0       /*!< Offset of slave address in Ser-PDU. */
#define MB_SER_PDU_PDU_OFF      1       /*!< Offset of Modbus-PDU in Ser-PDU. */

/* ----------------------- Type definitions ---------------------------------*/
typedef enum
{
    STATE_RX_INIT,              /*!< Receiver is in initial state. 接收器处于初始状态*/
    STATE_RX_IDLE,              /*!< Receiver is in idle state. 接收器处于空闲状态*/
    STATE_RX_RCV,               /*!< Frame is beeing received. 正在接收帧*/
    STATE_RX_ERROR              /*!< If the frame is invalid. 如果帧无效*/
} eMBRcvState;

typedef enum
{
    STATE_TX_IDLE,              /*!< Transmitter is in idle state. 发送器处于空闲状态*/
    STATE_TX_XMIT               /*!< Transmitter is in transfer state. 发射器处于传输状态*/
} eMBSndState;

/* ----------------------- Static variables ---------------------------------*/
static volatile eMBSndState eSndState;
static volatile eMBRcvState eRcvState;

volatile UCHAR  ucRTUBuf[MB_SER_PDU_SIZE_MAX];

static volatile UCHAR *pucSndBufferCur;
static volatile USHORT usSndBufferCount;

static volatile USHORT usRcvBufferPos;

/* ----------------------- Start implementation -----------------------------*/
eMBErrorCode
eMBRTUInit( UCHAR ucSlaveAddress, UCHAR ucPort, ULONG ulBaudRate, eMBParity eParity )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    ULONG           usTimerT35_50us;

    ( void )ucSlaveAddress;
    ENTER_CRITICAL_SECTION(  );

    /* Modbus RTU uses 8 Databits. 初始化串口，数据位为8*/
    if( xMBPortSerialInit( ucPort, ulBaudRate, 8, eParity ) != TRUE )
    {
        eStatus = MB_EPORTERR;
    }
    else
    {
        /* If baudrate > 19200 then we should use the fixed timer values
         * t35 = 1750us. Otherwise t35 must be 3.5 times the character time.
         */
        //         如果波特率> 19200，那么我们应该使用固定定时器值t35 = 1750us。
        // 否则t35必须是字符时间的3.5倍。
        if( ulBaudRate > 19200 )
        {
            usTimerT35_50us = 35;       /* 1800us. */
        }
        else
        {
            /* The timer reload value for a character is given by:
             *
             * ChTimeValue = Ticks_per_1s / ( Baudrate / 11 )
             *             = 11 * Ticks_per_1s / Baudrate
             *             = 220000 / Baudrate
             * The reload for t3.5 is 1.5 times this value and similary
             * for t3.5.
             */
            // 11是RTU模式每个字符( 11 位 )的格式：1 起始位，8 数据位， 首先发送最低有效位，1 位作为奇偶校验，1 停止位
            // 计算是结果是usTimerT35_50us代表，超时时间有多少个50us
            // 一秒中有20000个50us，Ticks_per_1s为20000
            // Baudrate / Ticks_per_1s 代表50us能传多少个位的数据，11/(Baudrate / Ticks_per_1s)就是传11位所花的时间
            // 11位为一个字符。
            // 11/(Baudrate / Ticks_per_1s)其实就是11 * Ticks_per_1s / Baudrate 
            usTimerT35_50us = ( 7UL * 220000UL ) / ( 2UL * ulBaudRate );    //3.5个字符的时间
        }
        // 初始化定时器
        if( xMBPortTimersInit( ( USHORT ) usTimerT35_50us ) != TRUE )
        {
            eStatus = MB_EPORTERR;
        }
    }
    EXIT_CRITICAL_SECTION(  );

    return eStatus;
}

void
eMBRTUStart( void )
{
    ENTER_CRITICAL_SECTION(  );
    /* Initially the receiver is in the state STATE_RX_INIT. we start
     * the timer and if no character is received within t3.5 we change
     * to STATE_RX_IDLE. This makes sure that we delay startup of the
     * modbus protocol stack until the bus is free.
     */
    eRcvState = STATE_RX_INIT;
    vMBPortSerialEnable( TRUE, FALSE );     //开始接收
    vMBPortTimersEnable(  );                //启动定时器

    EXIT_CRITICAL_SECTION(  );
}

void
eMBRTUStop( void )
{
    ENTER_CRITICAL_SECTION(  );
    vMBPortSerialEnable( FALSE, FALSE );    //停止收发
    vMBPortTimersDisable(  );               //关闭定时器
    EXIT_CRITICAL_SECTION(  );
}

eMBErrorCode
eMBRTUReceive( UCHAR * pucRcvAddress, UCHAR ** pucFrame, USHORT * pusLength )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    ENTER_CRITICAL_SECTION(  );
    if (usRcvBufferPos <= MB_SER_PDU_SIZE_MAX)
    {
        /* Length and CRC check */
        if( ( usRcvBufferPos >= MB_SER_PDU_SIZE_MIN )
            && ( usMBCRC16( ( UCHAR * ) ucRTUBuf, usRcvBufferPos ) == 0 ) )
        {
            /* Save the address field. All frames are passed to the upper layed
            * and the decision if a frame is used is done there.
            */
            *pucRcvAddress = ucRTUBuf[MB_SER_PDU_ADDR_OFF];

            /* Total length of Modbus-PDU is Modbus-Serial-Line-PDU minus
            * size of address field and CRC checksum.
            */
            *pusLength = ( USHORT )( usRcvBufferPos - MB_SER_PDU_PDU_OFF - MB_SER_PDU_SIZE_CRC );

            /* Return the start of the Modbus PDU to the caller. */
            *pucFrame = ( UCHAR * ) & ucRTUBuf[MB_SER_PDU_PDU_OFF];
        }
        else
        {
            eStatus = MB_EIO;
        }
    }
    else
    {
        eStatus = MB_EINVAL;
    }

    EXIT_CRITICAL_SECTION(  );
    return eStatus;
}

// pucFrame就是上面的接收返回的ucRTUBuf地址,所以填充CRC时,直接往ucRTUBuf中填充就可以了
eMBErrorCode
eMBRTUSend( UCHAR ucSlaveAddress, const UCHAR * pucFrame, USHORT usLength )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          usCRC16;

    ENTER_CRITICAL_SECTION(  );

    /* Check if the receiver is still in idle state. If not we where to
     * slow with processing the received frame and the master sent another
     * frame on the network. We have to abort sending the frame.
     */
    if( eRcvState == STATE_RX_IDLE )
    {
        /* First byte before the Modbus-PDU is the slave address. */
        pucSndBufferCur = ( UCHAR * ) pucFrame - 1;
        usSndBufferCount = 1;

        /* Now copy the Modbus-PDU into the Modbus-Serial-Line-PDU. */
        pucSndBufferCur[MB_SER_PDU_ADDR_OFF] = ucSlaveAddress;
        usSndBufferCount += usLength;

        /* Calculate CRC16 checksum for Modbus-Serial-Line-PDU. */
        usCRC16 = usMBCRC16( ( UCHAR * ) pucSndBufferCur, usSndBufferCount );
        ucRTUBuf[usSndBufferCount++] = ( UCHAR )( usCRC16 & 0xFF );
        ucRTUBuf[usSndBufferCount++] = ( UCHAR )( usCRC16 >> 8 );

        /* Activate the transmitter. */
        eSndState = STATE_TX_XMIT;
        vMBPortSerialEnable( FALSE, TRUE );
    }
    else
    {
        eStatus = MB_EIO;
    }
    EXIT_CRITICAL_SECTION(  );
    return eStatus;
}

BOOL
xMBRTUReceiveFSM( void )
{
    BOOL            xTaskNeedSwitch = FALSE;
    UCHAR           ucByte;

    if (eSndState == STATE_TX_IDLE)
    {
        /* Always read the character. */
        ( void )xMBPortSerialGetByte( ( CHAR * ) & ucByte );

        switch ( eRcvState )
        {
            /* If we have received a character in the init state we have to
            * wait until the frame is finished.
            */
        case STATE_RX_INIT:
            vMBPortTimersEnable( );
            break;

            /* In the error state we wait until all characters in the
            * damaged frame are transmitted.
            */
        case STATE_RX_ERROR:
            vMBPortTimersEnable( );
            break;

            /* In the idle state we wait for a new character. If a character
            * is received the t1.5 and t3.5 timers are started and the
            * receiver is in the state STATE_RX_RECEIVCE.
            */
        case STATE_RX_IDLE:     //STATE_RX_INIT状态下等t35时间到后就会设置城STATE_RX_IDLE
            usRcvBufferPos = 0;
            ucRTUBuf[usRcvBufferPos++] = ucByte;
            eRcvState = STATE_RX_RCV;

            /* Enable t3.5 timers. */
            vMBPortTimersEnable( );
            break;

            /* We are currently receiving a frame. Reset the timer after
            * every character received. If more than the maximum possible
            * number of bytes in a modbus frame is received the frame is
            * ignored.
            */
        case STATE_RX_RCV:
            if( usRcvBufferPos < MB_SER_PDU_SIZE_MAX )
            {
                ucRTUBuf[usRcvBufferPos++] = ucByte;
            }
            else
            {
                eRcvState = STATE_RX_ERROR;
            }
            vMBPortTimersEnable();
            break;
        }
    }
    else
    {
        error_printf("modbus", "The transmission state is not idle at the time of receiving\r\n");
    }
    return xTaskNeedSwitch;
}

BOOL
xMBRTUTransmitFSM( void )
{
    BOOL            xNeedPoll = FALSE;

    if (eRcvState == STATE_RX_IDLE)
    {
        switch ( eSndState )
        {
            /* We should not get a transmitter event if the transmitter is in
            * idle state.  */
        case STATE_TX_IDLE:
            /* enable receiver/disable transmitter. */
            vMBPortSerialEnable( TRUE, FALSE );
            break;

        case STATE_TX_XMIT:
            /* check if we are finished. */
            // 看数据有没有发完
            if( usSndBufferCount != 0 )
            {
                xMBPortSerialPutByte( ( CHAR )*pucSndBufferCur );
                pucSndBufferCur++;  /* next byte in sendbuffer. */
                usSndBufferCount--;
            }
            else
            {
                xNeedPoll = xMBPortEventPost( EV_FRAME_SENT );  //发送完了
                /* Disable transmitter. This prevents another transmit buffer empty interrupt. */
                vMBPortSerialEnable( TRUE, FALSE );
                eSndState = STATE_TX_IDLE;
            }
            break;
        }
    }
    else
    {
        error_printf("modbus", "The receiving state is not idle at the time of transmission\r\n");
    }

    return xNeedPoll;
}

BOOL
xMBRTUTimerT35Expired( void )
{
    BOOL            xNeedPoll = FALSE;
    eMBRcvState State = eRcvState;

    vMBPortTimersDisable(  );
    eRcvState = STATE_RX_IDLE;

    switch ( State )
    {
        /* Timer t35 expired. Startup phase is finished. 计时器t35已过期。启动阶段结束。*/
    case STATE_RX_INIT:
        xNeedPoll = xMBPortEventPost( EV_READY );
        break;

        /* A frame was received and t35 expired. Notify the listener that
         * a new frame was received. */
        // 收到一个帧，t35过期。通知侦听器收到了一个新帧。
    case STATE_RX_RCV:
        xNeedPoll = xMBPortEventPost( EV_FRAME_RECEIVED );
        break;

        /* An error occured while receiving the frame. 接收帧时发生错误*/
    case STATE_RX_ERROR:
        break;

        /* Function called in an illegal state. 函数在非法状态下被调用*/
    default:
         break;
    }

    return xNeedPoll;
}
