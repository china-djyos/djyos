/*
 * FreeModbus Libary: A portable Modbus implementation for Modbus ASCII/RTU.
 * Copyright (c) 2013 China Beijing Armink <armink.ztl@gmail.com>
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
 * File: $Id: mbrtu_m.c,v 1.60 2013/08/17 11:42:56 Armink Add Master Functions $
 */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mb_m.h"
#include "mbrtu.h"
#include "mbframe.h"

#include "mbcrc.h"
#include "mbport.h"

#if MB_MASTER_RTU_ENABLED > 0
/* ----------------------- Defines ------------------------------------------*/
#define MB_SER_PDU_SIZE_MIN     4       /*!< Minimum size of a Modbus RTU frame. */
#define MB_SER_PDU_SIZE_MAX     256     /*!< Maximum size of a Modbus RTU frame. */
#define MB_SER_PDU_SIZE_CRC     2       /*!< Size of CRC field in PDU. */
#define MB_SER_PDU_ADDR_OFF     0       /*!< Offset of slave address in Ser-PDU. */
#define MB_SER_PDU_PDU_OFF      1       /*!< Offset of Modbus-PDU in Ser-PDU. */

/* ----------------------- Type definitions ---------------------------------*/
typedef enum
{
    STATE_M_RX_INIT,              /*!< Receiver is in initial state. 接收器处于初始状态*/
    STATE_M_RX_IDLE,              /*!< Receiver is in idle state. 接收器处于空闲状态。*/
    STATE_M_RX_RCV,               /*!< Frame is beeing received. 正在接收帧。*/
    STATE_M_RX_ERROR,              /*!< If the frame is invalid. 如果帧无效。*/
} eMBMasterRcvState;

typedef enum
{
    STATE_M_TX_IDLE,              /*!< Transmitter is in idle state. 发送是空闲状态*/
    STATE_M_TX_XMIT,              /*!< Transmitter is in transfer state. 发送是发送状态*/
    STATE_M_TX_XFWR,              /*!< Transmitter is in transfer finish and wait receive state.发送器处于传输完成并等待接收状态。*/
} eMBMasterSndState;

/* ----------------------- Static variables ---------------------------------*/
static volatile eMBMasterSndState eSndState;
static volatile eMBMasterRcvState eRcvState;

static volatile UCHAR  ucMasterRTUSndBuf[MB_PDU_SIZE_MAX];      //发送缓冲位253,少了1个字节的服务器地址和2个字节的CRC
static volatile UCHAR  ucMasterRTURcvBuf[MB_SER_PDU_SIZE_MAX];
static volatile USHORT usMasterSendPDULength;   //发送PDU的长度

static volatile UCHAR *pucMasterSndBufferCur;   //主机发送缓冲地址
static volatile USHORT usMasterSndBufferCount;  //发送缓冲数据长度

static volatile USHORT usMasterRcvBufferPos;
static volatile BOOL   xFrameIsBroadcast = FALSE;

static volatile eMBMasterTimerMode eMasterCurTimerMode;     //主机的计时方式

/* ----------------------- Start implementation -----------------------------*/
eMBErrorCode
eMBMasterRTUInit(UCHAR ucPort, ULONG ulBaudRate, eMBParity eParity )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    ULONG           usTimerT35_50us;

    ENTER_CRITICAL_SECTION(  );

    /* Modbus RTU uses 8 Databits. */
    // 串口初始化
    if( xMBMasterPortSerialInit( ucPort, ulBaudRate, 8, eParity ) != TRUE )
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
            usTimerT35_50us = ( 7UL * 220000UL ) / ( 2UL * ulBaudRate );
        }
        // 初始化主用的定时器
        if( xMBMasterPortTimersInit( ( USHORT ) usTimerT35_50us ) != TRUE )
        {
            eStatus = MB_EPORTERR;
        }
    }
    EXIT_CRITICAL_SECTION(  );

    return eStatus;
}

void
eMBMasterRTUStart( void )
{
    ENTER_CRITICAL_SECTION(  );
    /* Initially the receiver is in the state STATE_M_RX_INIT. we start
     * the timer and if no character is received within t3.5 we change
     * to STATE_M_RX_IDLE. This makes sure that we delay startup of the
     * modbus protocol stack until the bus is free.
     */
    eRcvState = STATE_M_RX_INIT;
    vMBMasterPortSerialEnable( TRUE, FALSE );
    vMBMasterPortTimersT35Enable(  );

    EXIT_CRITICAL_SECTION(  );
}

void
eMBMasterRTUStop( void )
{
    ENTER_CRITICAL_SECTION(  );
    vMBMasterPortSerialEnable( FALSE, FALSE );
    vMBMasterPortTimersDisable(  );
    EXIT_CRITICAL_SECTION(  );
}

eMBErrorCode
eMBMasterRTUReceive( UCHAR * pucRcvAddress, UCHAR ** pucFrame, USHORT * pusLength )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    ENTER_CRITICAL_SECTION(  );

    if (usMasterRcvBufferPos < MB_SER_PDU_SIZE_MAX)
    {
        /* Length and CRC check */
        if( ( usMasterRcvBufferPos >= MB_SER_PDU_SIZE_MIN )
            && ( usMBCRC16( ( UCHAR * ) ucMasterRTURcvBuf, usMasterRcvBufferPos ) == 0 ) )
        {
            /* Save the address field. All frames are passed to the upper layed
            * and the decision if a frame is used is done there.
            */
            *pucRcvAddress = ucMasterRTURcvBuf[MB_SER_PDU_ADDR_OFF];

            /* Total length of Modbus-PDU is Modbus-Serial-Line-PDU minus
            * size of address field and CRC checksum.
            */
            *pusLength = ( USHORT )( usMasterRcvBufferPos - MB_SER_PDU_PDU_OFF - MB_SER_PDU_SIZE_CRC );

            /* Return the start of the Modbus PDU to the caller. */
            *pucFrame = ( UCHAR * ) & ucMasterRTURcvBuf[MB_SER_PDU_PDU_OFF];
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

eMBErrorCode
eMBMasterRTUSend( UCHAR ucSlaveAddress, const UCHAR * pucFrame, USHORT usLength )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          usCRC16;

    // 检查从机地址是否错误
    if ( ucSlaveAddress > MB_MASTER_TOTAL_SLAVE_NUM ) return MB_EINVAL;

    ENTER_CRITICAL_SECTION(  );

    /* Check if the receiver is still in idle state. 
    If not we where to slow with processing the received frame and the master sent another frame on the network. 
    We have to abort sending the frame.
     */
//     检查接收端是否仍处于空闲状态。
// 如果不是，我们将处理接收到的帧，并且主机在网络上发送另一个帧。
// 我们必须中止发送帧。
    if( eRcvState == STATE_M_RX_IDLE )
    {
        /* First byte before the Modbus-PDU is the slave address. */
        // Modbus-PDU前的第一个字节为从地址。
        pucMasterSndBufferCur = ( UCHAR * ) pucFrame - 1;
        usMasterSndBufferCount = 1;

        /* Now copy the Modbus-PDU into the Modbus-Serial-Line-PDU. */
        /*现在复制Modbus-PDU到Modbus-Serial-Line-PDU。*/
        // 设置从机地址
        pucMasterSndBufferCur[MB_SER_PDU_ADDR_OFF] = ucSlaveAddress;
        usMasterSndBufferCount += usLength;

        /* Calculate CRC16 checksum for Modbus-Serial-Line-PDU. */
        // 做号CRC校验设置号校验码
        usCRC16 = usMBCRC16( ( UCHAR * ) pucMasterSndBufferCur, usMasterSndBufferCount );
        ucMasterRTUSndBuf[usMasterSndBufferCount++] = ( UCHAR )( usCRC16 & 0xFF );
        ucMasterRTUSndBuf[usMasterSndBufferCount++] = ( UCHAR )( usCRC16 >> 8 );

        /* Activate the transmitter. */
        eSndState = STATE_M_TX_XMIT;
        vMBMasterPortSerialEnable( FALSE, TRUE );
    }
    else
    {
        eStatus = MB_EIO;
    }
    EXIT_CRITICAL_SECTION(  );
    return eStatus;
}

BOOL
xMBMasterRTUReceiveFSM( void )
{
    BOOL            xTaskNeedSwitch = FALSE;
    UCHAR           ucByte;

    if (( eSndState == STATE_M_TX_IDLE ) || ( eSndState == STATE_M_TX_XFWR ))
    {
        /* Always read the character. */
        // 从串口读取数据
        ( void )xMBMasterPortSerialGetByte( ( CHAR * ) & ucByte );

        switch ( eRcvState )
        {
            /* If we have received a character in the init state we have to
            * wait until the frame is finished.
            */
        case STATE_M_RX_INIT:
            vMBMasterPortTimersT35Enable( );
            break;

            /* In the error state we wait until all characters in the
            * damaged frame are transmitted.
            */
        case STATE_M_RX_ERROR:
            vMBMasterPortTimersT35Enable( );
            break;

            /* In the idle state we wait for a new character. If a character
            * is received the t1.5 and t3.5 timers are started and the
            * receiver is in the state STATE_RX_RECEIVCE and disable early
            * the timer of respond timeout .
            */
        case STATE_M_RX_IDLE:   //STATE_M_RX_IDLE状态下等t35时间到后就会设置城STATE_RX_IDLE
            /* In time of respond timeout,the receiver receive a frame.
            * Disable timer of respond timeout and change the transmiter state to idle.
            */
            /*在响应超时时间内，接收方接收到一个帧。
            *关闭响应超时定时器，将发送器状态改为空闲。*/
            vMBMasterPortTimersDisable( );
            eSndState = STATE_M_TX_IDLE;

            usMasterRcvBufferPos = 0;
            ucMasterRTURcvBuf[usMasterRcvBufferPos++] = ucByte;
            eRcvState = STATE_M_RX_RCV;

            /* Enable t3.5 timers. */
            vMBMasterPortTimersT35Enable( );
            break;

            /* We are currently receiving a frame. Reset the timer after
            * every character received. If more than the maximum possible
            * number of bytes in a modbus frame is received the frame is
            * ignored.
            */
        case STATE_M_RX_RCV:
            if( usMasterRcvBufferPos < MB_SER_PDU_SIZE_MAX )
            {
                ucMasterRTURcvBuf[usMasterRcvBufferPos++] = ucByte;
            }
            else
            {
                eRcvState = STATE_M_RX_ERROR;
            }
            vMBMasterPortTimersT35Enable();
            break;
        }
    }
    else
    {
        error_printf("modbus", "The transmission state is not idle or waiting to receive at the time of receiving\r\n");
    }
    return xTaskNeedSwitch;
}

BOOL
xMBMasterRTUTransmitFSM( void )
{
    BOOL            xNeedPoll = FALSE;

    if (eRcvState == STATE_M_RX_IDLE)
    {
        switch ( eSndState )
        {
            /* We should not get a transmitter event if the transmitter is in
            * idle state.  */
        case STATE_M_TX_IDLE:
            /* enable receiver/disable transmitter. */
            /*启用接收/禁用发送。*/
            vMBMasterPortSerialEnable( TRUE, FALSE );
            break;

        case STATE_M_TX_XMIT:
            /* check if we are finished. */
            if( usMasterSndBufferCount != 0 )
            {
                // 一个一个字节发送数据
                xMBMasterPortSerialPutByte( ( CHAR )*pucMasterSndBufferCur );
                pucMasterSndBufferCur++;  /* next byte in sendbuffer. */
                usMasterSndBufferCount--;
            }
            else
            {
                // 发完了，进入接收状态
                xFrameIsBroadcast = ( ucMasterRTUSndBuf[MB_SER_PDU_ADDR_OFF] == MB_ADDRESS_BROADCAST ) ? TRUE : FALSE;
                /* Disable transmitter. This prevents another transmit buffer
                * empty interrupt. */
                eSndState = STATE_M_TX_XFWR;
                /* If the frame is broadcast ,master will enable timer of convert delay,
                * else master will enable timer of respond timeout. */
                if ( xFrameIsBroadcast == TRUE )
                {
                    // 广播 转换延时使能
                    vMBMasterPortTimersConvertDelayEnable( );
                }
                else
                {
                    // 单播 接收响应超时使能
                    vMBMasterPortTimersRespondTimeoutEnable( );
                }
                vMBMasterPortSerialEnable( TRUE, FALSE );
            }
            break;

        default:
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
xMBMasterRTUTimerExpired(void)
{
    BOOL xNeedPoll = FALSE;

    switch (eRcvState)
    {
        /* Timer t35 expired. Startup phase is finished. */
        // 计时器t35已过期。启动阶段结束。
    case STATE_M_RX_INIT:
        xNeedPoll = xMBMasterPortEventPost(EV_MASTER_READY);
        break;

        /* A frame was received and t35 expired. Notify the listener that
         * a new frame was received. */
    case STATE_M_RX_RCV:
    // 接收超时，代表接收结束了，设置EV_MASTER_FRAME_RECEIVED状态，去处理这个收到的帧
        xNeedPoll = xMBMasterPortEventPost(EV_MASTER_FRAME_RECEIVED);
        break;

        /* An error occured while receiving the frame. */
        // 接收帧时发生错误
    case STATE_M_RX_ERROR:
        vMBMasterSetErrorType(EV_ERROR_RECEIVE_DATA);
        xNeedPoll = xMBMasterPortEventPost( EV_MASTER_ERROR_PROCESS );
        break;

        /* Function called in an illegal state. */
    default:

        break;
    }
    eRcvState = STATE_M_RX_IDLE;

    switch (eSndState)
    {
        /* A frame was send finish and convert delay or respond timeout expired.
         * If the frame is broadcast,The master will idle,and if the frame is not
         * broadcast.Notify the listener process error.*/
    case STATE_M_TX_XFWR:
        if ( xFrameIsBroadcast == FALSE ) {
            // 不是广播 那发送的数据就要有从机应答，超时了，设置应答超时
            vMBMasterSetErrorType(EV_ERROR_RESPOND_TIMEOUT);
            xNeedPoll = xMBMasterPortEventPost(EV_MASTER_ERROR_PROCESS);
        }
        break;
        /* Function called in an illegal state. */
    default:

        break;
    }
    eSndState = STATE_M_TX_IDLE;

    vMBMasterPortTimersDisable( );
    /* If timer mode is convert delay, the master event then turns EV_MASTER_EXECUTE status. */
    if (eMasterCurTimerMode == MB_TMODE_CONVERT_DELAY) {
        xNeedPoll = xMBMasterPortEventPost( EV_MASTER_EXECUTE );
    }

    return xNeedPoll;
}

/* Get Modbus Master send RTU's buffer address pointer.*/
// 获取主机发送RTU的缓冲区地址指针
void vMBMasterGetRTUSndBuf( UCHAR ** pucFrame )
{
    *pucFrame = ( UCHAR * ) ucMasterRTUSndBuf;
}

/* Get Modbus Master send PDU's buffer address pointer.*/
// 获取主机要发送的数据地址(PDU,不包含目的地址)
void vMBMasterGetPDUSndBuf( UCHAR ** pucFrame )
{
    *pucFrame = ( UCHAR * ) &ucMasterRTUSndBuf[MB_SER_PDU_PDU_OFF];
}

/* Set Modbus Master send PDU's buffer length.*/
// 设置主机发送的数据长度
void vMBMasterSetPDUSndLength( USHORT SendPDULength )
{
    usMasterSendPDULength = SendPDULength;
}

/* Get Modbus Master send PDU's buffer length.*/
// 获取主机发送的数据长度
USHORT usMBMasterGetPDUSndLength( void )
{
    return usMasterSendPDULength;
}

/* Set Modbus Master current timer mode.*/
// 设置Modbus Master当前定时器模式。
void vMBMasterSetCurTimerMode( eMBMasterTimerMode eMBTimerMode )
{
    eMasterCurTimerMode = eMBTimerMode;
}

/* The master request is broadcast? */
// 主请求是否位广播
BOOL xMBMasterRequestIsBroadcast( void ){
    return xFrameIsBroadcast;
}
#endif

