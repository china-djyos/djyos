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
 * File: $Id: mbascii.c,v 1.15 2007/02/18 23:46:48 wolti Exp $
 */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbconfig.h"
#include "mbascii.h"
#include "mbframe.h"

#include "mbcrc.h"
#include "mbport.h"

#if MB_SLAVE_ASCII_ENABLED > 0

/* ----------------------- Defines ------------------------------------------*/
#define MB_ASCII_DEFAULT_CR     '\r'    /*!< Default CR character for Modbus ASCII. */
#define MB_ASCII_DEFAULT_LF     '\n'    /*!< Default LF character for Modbus ASCII. */
#define MB_SER_PDU_SIZE_MIN     3       /*!< Minimum size of a Modbus ASCII frame. Modbus ASCII帧的最小尺寸*/
#define MB_SER_PDU_SIZE_MAX     256     /*!< Maximum size of a Modbus ASCII frame. Modbus ASCII帧的最大尺寸*/
#define MB_SER_PDU_SIZE_LRC     1       /*!< Size of LRC field in PDU. PDU中LRC字段的大小。*/
#define MB_SER_PDU_ADDR_OFF     0       /*!< Offset of slave address in Ser-PDU.Ser-PDU中从地址的偏移量。 */
#define MB_SER_PDU_PDU_OFF      1       /*!< Offset of Modbus-PDU in Ser-PDU. Ser-PDU中Modbus-PDU的偏移量。*/

/* ----------------------- Type definitions ---------------------------------*/
typedef enum
{
    STATE_RX_IDLE,              /*!< Receiver is in idle state. 接收器处于空闲状态。*/
    STATE_RX_RCV,               /*!< Frame is beeing received. 正在接收帧。*/
    STATE_RX_WAIT_EOF           /*!< Wait for End of Frame. 等待帧结束。*/
} eMBRcvState;

typedef enum
{
    STATE_TX_IDLE,              /*!< Transmitter is in idle state.发送器处于空闲状态。 */
    STATE_TX_START,             /*!< Starting transmission (':' sent). 开始传输(':' sent)。*/
    STATE_TX_DATA,              /*!< Sending of data (Address, Data, LRC). 发送数据(地址，数据，LRC)。*/
    STATE_TX_END,               /*!< End of transmission. 传输结束。*/
    STATE_TX_NOTIFY             /*!< Notify sender that the frame has been sent. 通知发送方帧已经发送。*/
} eMBSndState;

typedef enum
{
    BYTE_HIGH_NIBBLE,           /*!< Character for high nibble of byte. 字节高偏移字符。*/
    BYTE_LOW_NIBBLE             /*!< Character for low nibble of byte. 字节低偏移字符。*/
} eMBBytePos;

/* ----------------------- Static functions ---------------------------------*/
static UCHAR    prvucMBCHAR2BIN( UCHAR ucCharacter );

static UCHAR    prvucMBBIN2CHAR( UCHAR ucByte );

static UCHAR    prvucMBLRC( UCHAR * pucFrame, USHORT usLen );

/* ----------------------- Static variables ---------------------------------*/
static volatile eMBSndState eSndState;
static volatile eMBRcvState eRcvState;

/* We reuse the Modbus RTU buffer because only one buffer is needed and the RTU buffer is bigger. */
/*我们重用Modbus RTU缓冲区，因为只需要一个缓冲区，RTU缓冲区更大。*/
extern volatile UCHAR ucRTUBuf[];
static volatile UCHAR *ucASCIIBuf = ucRTUBuf;

static volatile USHORT usRcvBufferPos;
static volatile eMBBytePos eBytePos;

static volatile UCHAR *pucSndBufferCur;
static volatile USHORT usSndBufferCount;

static volatile UCHAR ucLRC;
static volatile UCHAR ucMBLFCharacter;

/* ----------------------- Start implementation -----------------------------*/
eMBErrorCode
eMBASCIIInit( UCHAR ucSlaveAddress, UCHAR ucPort, ULONG ulBaudRate, eMBParity eParity )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    ( void )ucSlaveAddress;

    ENTER_CRITICAL_SECTION(  );
    ucMBLFCharacter = MB_ASCII_DEFAULT_LF;
    // 串口初始化
    if( xMBPortSerialInit( ucPort, ulBaudRate, 7, eParity ) != TRUE )
    {
        eStatus = MB_EPORTERR;
    } //定时器初始化
    else if( xMBPortTimersInit( MB_ASCII_TIMEOUT_SEC * 20000UL ) != TRUE )
    {
        eStatus = MB_EPORTERR;
    }

    EXIT_CRITICAL_SECTION(  );

    return eStatus;
}

void
eMBASCIIStart( void )
{
    ENTER_CRITICAL_SECTION(  );
    vMBPortSerialEnable( TRUE, FALSE );
    eRcvState = STATE_RX_IDLE;
    EXIT_CRITICAL_SECTION(  );

    /* No special startup required for ASCII. */
    ( void )xMBPortEventPost( EV_READY );
}

void
eMBASCIIStop( void )
{
    ENTER_CRITICAL_SECTION(  );
    vMBPortSerialEnable( FALSE, FALSE );
    vMBPortTimersDisable(  );
    EXIT_CRITICAL_SECTION(  );
}

eMBErrorCode
eMBASCIIReceive( UCHAR * pucRcvAddress, UCHAR ** pucFrame, USHORT * pusLength )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    ENTER_CRITICAL_SECTION(  );
    // assert( usRcvBufferPos < MB_SER_PDU_SIZE_MAX );
    if (usRcvBufferPos < MB_SER_PDU_SIZE_MAX)
    {
        /* Length and CRC check */
        if( ( usRcvBufferPos >= MB_SER_PDU_SIZE_MIN )
            && ( prvucMBLRC( ( UCHAR * ) ucASCIIBuf, usRcvBufferPos ) == 0 ) )
        {
            /* Save the address field. All frames are passed to the upper layed
            * and the decision if a frame is used is done there.
            */
            *pucRcvAddress = ucASCIIBuf[MB_SER_PDU_ADDR_OFF];

            /* Total length of Modbus-PDU is Modbus-Serial-Line-PDU minus
            * size of address field and CRC checksum.
            */
            *pusLength = ( USHORT )( usRcvBufferPos - MB_SER_PDU_PDU_OFF - MB_SER_PDU_SIZE_LRC );

            /* Return the start of the Modbus PDU to the caller. */
            *pucFrame = ( UCHAR * ) & ucASCIIBuf[MB_SER_PDU_PDU_OFF];
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
eMBASCIISend( UCHAR ucSlaveAddress, const UCHAR * pucFrame, USHORT usLength )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    UCHAR           usLRC;

    ENTER_CRITICAL_SECTION(  );
    /* Check if the receiver is still in idle state. If not we where too
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

        /* Calculate LRC checksum for Modbus-Serial-Line-PDU. */
        usLRC = prvucMBLRC( ( UCHAR * ) pucSndBufferCur, usSndBufferCount );
        ucASCIIBuf[usSndBufferCount++] = usLRC;

        /* Activate the transmitter. */
        eSndState = STATE_TX_START;
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
xMBASCIIReceiveFSM( void )
{
    BOOL            xNeedPoll = FALSE;
    UCHAR           ucByte;
    UCHAR           ucResult;

    // assert( eSndState == STATE_TX_IDLE );
    // 从串口读数据
    if (eSndState == STATE_TX_IDLE)
    {
        ( void )xMBPortSerialGetByte( ( CHAR * ) & ucByte );
        switch ( eRcvState )
        {
            /* A new character is received. If the character is a ':' the input
            * buffer is cleared. A CR-character signals the end of the data
            * block. Other characters are part of the data block and their
            * ASCII value is converted back to a binary representation.
            */
        case STATE_RX_RCV:
            /* Enable timer for character timeout. */
            vMBPortTimersEnable(  );
            if( ucByte == ':' )
            {   //收到: 接下来要收到的第一个数据是高位的
                /* Empty receive buffer. */
                eBytePos = BYTE_HIGH_NIBBLE;
                usRcvBufferPos = 0;     //清空接收
            }
            else if( ucByte == MB_ASCII_DEFAULT_CR )
            {
                eRcvState = STATE_RX_WAIT_EOF;
            }
            else
            {
                // 把ASCII转成数字
                ucResult = prvucMBCHAR2BIN( ucByte );
                switch ( eBytePos )
                {
                    /* High nibble of the byte comes first. We check for
                    * a buffer overflow here. */
                case BYTE_HIGH_NIBBLE:
                    if( usRcvBufferPos < MB_SER_PDU_SIZE_MAX )
                    {
                        // 设置高位数据
                        ucASCIIBuf[usRcvBufferPos] = ( UCHAR )( ucResult << 4 );
                        eBytePos = BYTE_LOW_NIBBLE; //接下来设置低位
                        break;
                    }
                    else
                    {
                        /* not handled in Modbus specification but seems
                        * a resonable implementation. */
                        eRcvState = STATE_RX_IDLE;
                        /* Disable previously activated timer because of error state. */
                        vMBPortTimersDisable(  );
                    }
                    break;

                case BYTE_LOW_NIBBLE:
                    ucASCIIBuf[usRcvBufferPos] |= ucResult;     //把低4位填入
                    usRcvBufferPos++;
                    eBytePos = BYTE_HIGH_NIBBLE;
                    break;
                }
            }
            break;

        case STATE_RX_WAIT_EOF:
            if( ucByte == ucMBLFCharacter )
            {   //收到 /n所有数据接收完毕
                /* Disable character timeout timer because all characters are received. */
                // 启用字符超时定时器，因为所有字符都被接收。
                vMBPortTimersDisable(  );
                /* Receiver is again in idle state. */
                // 接收器再次处于空闲状态。
                eRcvState = STATE_RX_IDLE;

                /* Notify the caller of eMBASCIIReceive that a new frame was received. */
                /*通知emciireceive的调用者收到了一个新的帧。*/
                xNeedPoll = xMBPortEventPost( EV_FRAME_RECEIVED );
            }
            else if( ucByte == ':' )
            {
                // 又收到:号.清空接收缓冲区并返回接收状态。
                /* Empty receive buffer and back to receive state. */
                eBytePos = BYTE_HIGH_NIBBLE;
                usRcvBufferPos = 0;
                eRcvState = STATE_RX_RCV;

                /* Enable timer for character timeout. 启用字符超时计时器。*/
                vMBPortTimersEnable(  );
            }
            else
            {
                /* Frame is not okay. Delete entire frame. */
                // 帧不好。删除整个帧。
                eRcvState = STATE_RX_IDLE;
            }
            break;

        case STATE_RX_IDLE:
            if( ucByte == ':' )
            {   //收到:号.开始进入接收
                /* Enable timer for character timeout. */
                /*启用字符超时计时器。*/
                vMBPortTimersEnable(  );
                /* Reset the input buffers to store the frame. */
                // 重置输入缓冲区以存储帧。
                usRcvBufferPos = 0;;
                eBytePos = BYTE_HIGH_NIBBLE;
                eRcvState = STATE_RX_RCV;
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
xMBASCIITransmitFSM( void )
{
    BOOL            xNeedPoll = FALSE;
    UCHAR           ucByte;

    // assert( eRcvState == STATE_RX_IDLE );
    if (eRcvState == STATE_RX_IDLE)
    {
        switch ( eSndState )
        {
            /* Start of transmission. The start of a frame is defined by sending the character ':'. */
            // 开始传输。帧的开始通过发送字符':'来定义。
        case STATE_TX_START:
            ucByte = ':';
            xMBPortSerialPutByte( ( CHAR )ucByte ); //发送:号
            eSndState = STATE_TX_DATA;
            eBytePos = BYTE_HIGH_NIBBLE;
            break;

            /* Send the data block. Each data byte is encoded as a character hex
            * stream with the high nibble sent first and the low nibble sent
            * last. If all data bytes are exhausted we send a '\r' character
            * to end the transmission. */
        case STATE_TX_DATA:
            if( usSndBufferCount > 0 )
            {
                switch ( eBytePos )
                {
                case BYTE_HIGH_NIBBLE:
                // 发高4位,把数字转成ASCII发出
                    ucByte = prvucMBBIN2CHAR( ( UCHAR )( *pucSndBufferCur >> 4 ) );
                    xMBPortSerialPutByte( ( CHAR ) ucByte );
                    eBytePos = BYTE_LOW_NIBBLE;
                    break;

                case BYTE_LOW_NIBBLE:
                // 发低4位
                    ucByte = prvucMBBIN2CHAR( ( UCHAR )( *pucSndBufferCur & 0x0F ) );
                    xMBPortSerialPutByte( ( CHAR )ucByte );
                    pucSndBufferCur++;  //发送下一个字节
                    eBytePos = BYTE_HIGH_NIBBLE;
                    usSndBufferCount--;
                    break;
                }
            }
            else
            {
                xMBPortSerialPutByte( MB_ASCII_DEFAULT_CR );    //发送/r
                eSndState = STATE_TX_END;
            }
            break;

            /* Finish the frame by sending a LF character. */
        case STATE_TX_END:
            xMBPortSerialPutByte( ( CHAR )ucMBLFCharacter );    //发送/n
            /* We need another state to make sure that the CR character has been sent. */
            // 我们需要另一个状态来确保CR字符已经发送。
            eSndState = STATE_TX_NOTIFY;
            break;

            /* Notify the task which called eMBASCIISend that the frame has
            * been sent. */
        case STATE_TX_NOTIFY:
            eSndState = STATE_TX_IDLE;
            xNeedPoll = xMBPortEventPost( EV_FRAME_SENT );  //

            /* Disable transmitter. This prevents another transmit buffer
            * empty interrupt. */
            vMBPortSerialEnable( TRUE, FALSE );
            eSndState = STATE_TX_IDLE;
            break;

            /* We should not get a transmitter event if the transmitter is in
            * idle state.  */
        case STATE_TX_IDLE:
            /* enable receiver/disable transmitter. */
            vMBPortSerialEnable( TRUE, FALSE );
            break;
        }
    }
    else
    {
        error_printf("modbus", "The transmission state is not idle at the time of receiving\r\n");
    }

    return xNeedPoll;
}

BOOL
xMBASCIITimerT1SExpired( void )
{
    switch ( eRcvState )
    {
        /* If we have a timeout we go back to the idle state and wait for
         * the next frame.
         */
    case STATE_RX_RCV:
    case STATE_RX_WAIT_EOF:
        eRcvState = STATE_RX_IDLE;
        break;

    default:
        // assert( ( eRcvState == STATE_RX_RCV ) || ( eRcvState == STATE_RX_WAIT_EOF ) );
        break;
    }
    vMBPortTimersDisable(  );

    /* no context switch required. */
    return FALSE;
}


static          UCHAR
prvucMBCHAR2BIN( UCHAR ucCharacter )
{
    if( ( ucCharacter >= '0' ) && ( ucCharacter <= '9' ) )
    {
        return ( UCHAR )( ucCharacter - '0' );
    }
    else if( ( ucCharacter >= 'A' ) && ( ucCharacter <= 'F' ) )
    {
        return ( UCHAR )( ucCharacter - 'A' + 0x0A );
    }
    else
    {
        return 0xFF;
    }
}

static          UCHAR
prvucMBBIN2CHAR( UCHAR ucByte )
{
    if( ucByte <= 0x09 )
    {
        return ( UCHAR )( '0' + ucByte );
    }
    else if( ( ucByte >= 0x0A ) && ( ucByte <= 0x0F ) )
    {
        return ( UCHAR )( ucByte - 0x0A + 'A' );
    }
    else
    {
        /* Programming error. */
        // assert( 0 );
        error_printf("modbus", "Programming error.\r\n");
    }
    return '0';
}


// 校验LRC
static          UCHAR
prvucMBLRC( UCHAR * pucFrame, USHORT usLen )
{
    UCHAR           ucLRC = 0;  /* LRC char initialized */

    while( usLen-- )
    {
        ucLRC += *pucFrame++;   /* Add buffer byte without carry */
    }

    /* Return twos complement */
    ucLRC = ( UCHAR ) ( -( ( CHAR ) ucLRC ) );
    return ucLRC;
}

#endif
