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
    STATE_M_RX_INIT,              /*!< Receiver is in initial state. ���������ڳ�ʼ״̬*/
    STATE_M_RX_IDLE,              /*!< Receiver is in idle state. ���������ڿ���״̬��*/
    STATE_M_RX_RCV,               /*!< Frame is beeing received. ���ڽ���֡��*/
    STATE_M_RX_ERROR,              /*!< If the frame is invalid. ���֡��Ч��*/
} eMBMasterRcvState;

typedef enum
{
    STATE_M_TX_IDLE,              /*!< Transmitter is in idle state. �����ǿ���״̬*/
    STATE_M_TX_XMIT,              /*!< Transmitter is in transfer state. �����Ƿ���״̬*/
    STATE_M_TX_XFWR,              /*!< Transmitter is in transfer finish and wait receive state.���������ڴ�����ɲ��ȴ�����״̬��*/
} eMBMasterSndState;

/* ----------------------- Static variables ---------------------------------*/
static volatile eMBMasterSndState eSndState;
static volatile eMBMasterRcvState eRcvState;

static volatile UCHAR  ucMasterRTUSndBuf[MB_PDU_SIZE_MAX];      //���ͻ���λ253,����1���ֽڵķ�������ַ��2���ֽڵ�CRC
static volatile UCHAR  ucMasterRTURcvBuf[MB_SER_PDU_SIZE_MAX];
static volatile USHORT usMasterSendPDULength;   //����PDU�ĳ���

static volatile UCHAR *pucMasterSndBufferCur;   //�������ͻ����ַ
static volatile USHORT usMasterSndBufferCount;  //���ͻ������ݳ���

static volatile USHORT usMasterRcvBufferPos;
static volatile BOOL   xFrameIsBroadcast = FALSE;

static volatile eMBMasterTimerMode eMasterCurTimerMode;     //�����ļ�ʱ��ʽ

/* ----------------------- Start implementation -----------------------------*/
eMBErrorCode
eMBMasterRTUInit(UCHAR ucPort, ULONG ulBaudRate, eMBParity eParity )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    ULONG           usTimerT35_50us;

    ENTER_CRITICAL_SECTION(  );

    /* Modbus RTU uses 8 Databits. */
    // ���ڳ�ʼ��
    if( xMBMasterPortSerialInit( ucPort, ulBaudRate, 8, eParity ) != TRUE )
    {
        eStatus = MB_EPORTERR;
    }
    else
    {
        /* If baudrate > 19200 then we should use the fixed timer values
         * t35 = 1750us. Otherwise t35 must be 3.5 times the character time.
         */
        //         ���������> 19200����ô����Ӧ��ʹ�ù̶���ʱ��ֵt35 = 1750us��
        // ����t35�������ַ�ʱ���3.5����
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
            // 11��RTUģʽÿ���ַ�( 11 λ )�ĸ�ʽ��1 ��ʼλ��8 ����λ�� ���ȷ��������Чλ��1 λ��Ϊ��żУ�飬1 ֹͣλ
            // �����ǽ����usTimerT35_50us������ʱʱ���ж��ٸ�50us
            // һ������20000��50us��Ticks_per_1sΪ20000
            // Baudrate / Ticks_per_1s ����50us�ܴ����ٸ�λ�����ݣ�11/(Baudrate / Ticks_per_1s)���Ǵ�11λ������ʱ��
            // 11λΪһ���ַ���
            // 11/(Baudrate / Ticks_per_1s)��ʵ����11 * Ticks_per_1s / Baudrate 
            usTimerT35_50us = ( 7UL * 220000UL ) / ( 2UL * ulBaudRate );
        }
        // ��ʼ�����õĶ�ʱ��
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

    // ���ӻ���ַ�Ƿ����
    if ( ucSlaveAddress > MB_MASTER_TOTAL_SLAVE_NUM ) return MB_EINVAL;

    ENTER_CRITICAL_SECTION(  );

    /* Check if the receiver is still in idle state. 
    If not we where to slow with processing the received frame and the master sent another frame on the network. 
    We have to abort sending the frame.
     */
//     �����ն��Ƿ��Դ��ڿ���״̬��
// ������ǣ����ǽ�������յ���֡�����������������Ϸ�����һ��֡��
// ���Ǳ�����ֹ����֡��
    if( eRcvState == STATE_M_RX_IDLE )
    {
        /* First byte before the Modbus-PDU is the slave address. */
        // Modbus-PDUǰ�ĵ�һ���ֽ�Ϊ�ӵ�ַ��
        pucMasterSndBufferCur = ( UCHAR * ) pucFrame - 1;
        usMasterSndBufferCount = 1;

        /* Now copy the Modbus-PDU into the Modbus-Serial-Line-PDU. */
        /*���ڸ���Modbus-PDU��Modbus-Serial-Line-PDU��*/
        // ���ôӻ���ַ
        pucMasterSndBufferCur[MB_SER_PDU_ADDR_OFF] = ucSlaveAddress;
        usMasterSndBufferCount += usLength;

        /* Calculate CRC16 checksum for Modbus-Serial-Line-PDU. */
        // ����CRCУ�����ú�У����
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
        // �Ӵ��ڶ�ȡ����
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
        case STATE_M_RX_IDLE:   //STATE_M_RX_IDLE״̬�µ�t35ʱ�䵽��ͻ����ó�STATE_RX_IDLE
            /* In time of respond timeout,the receiver receive a frame.
            * Disable timer of respond timeout and change the transmiter state to idle.
            */
            /*����Ӧ��ʱʱ���ڣ����շ����յ�һ��֡��
            *�ر���Ӧ��ʱ��ʱ������������״̬��Ϊ���С�*/
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
            /*���ý���/���÷��͡�*/
            vMBMasterPortSerialEnable( TRUE, FALSE );
            break;

        case STATE_M_TX_XMIT:
            /* check if we are finished. */
            if( usMasterSndBufferCount != 0 )
            {
                // һ��һ���ֽڷ�������
                xMBMasterPortSerialPutByte( ( CHAR )*pucMasterSndBufferCur );
                pucMasterSndBufferCur++;  /* next byte in sendbuffer. */
                usMasterSndBufferCount--;
            }
            else
            {
                // �����ˣ��������״̬
                xFrameIsBroadcast = ( ucMasterRTUSndBuf[MB_SER_PDU_ADDR_OFF] == MB_ADDRESS_BROADCAST ) ? TRUE : FALSE;
                /* Disable transmitter. This prevents another transmit buffer
                * empty interrupt. */
                eSndState = STATE_M_TX_XFWR;
                /* If the frame is broadcast ,master will enable timer of convert delay,
                * else master will enable timer of respond timeout. */
                if ( xFrameIsBroadcast == TRUE )
                {
                    // �㲥 ת����ʱʹ��
                    vMBMasterPortTimersConvertDelayEnable( );
                }
                else
                {
                    // ���� ������Ӧ��ʱʹ��
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
        // ��ʱ��t35�ѹ��ڡ������׶ν�����
    case STATE_M_RX_INIT:
        xNeedPoll = xMBMasterPortEventPost(EV_MASTER_READY);
        break;

        /* A frame was received and t35 expired. Notify the listener that
         * a new frame was received. */
    case STATE_M_RX_RCV:
    // ���ճ�ʱ��������ս����ˣ�����EV_MASTER_FRAME_RECEIVED״̬��ȥ��������յ���֡
        xNeedPoll = xMBMasterPortEventPost(EV_MASTER_FRAME_RECEIVED);
        break;

        /* An error occured while receiving the frame. */
        // ����֡ʱ��������
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
            // ���ǹ㲥 �Ƿ��͵����ݾ�Ҫ�дӻ�Ӧ�𣬳�ʱ�ˣ�����Ӧ��ʱ
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
// ��ȡ��������RTU�Ļ�������ַָ��
void vMBMasterGetRTUSndBuf( UCHAR ** pucFrame )
{
    *pucFrame = ( UCHAR * ) ucMasterRTUSndBuf;
}

/* Get Modbus Master send PDU's buffer address pointer.*/
// ��ȡ����Ҫ���͵����ݵ�ַ(PDU,������Ŀ�ĵ�ַ)
void vMBMasterGetPDUSndBuf( UCHAR ** pucFrame )
{
    *pucFrame = ( UCHAR * ) &ucMasterRTUSndBuf[MB_SER_PDU_PDU_OFF];
}

/* Set Modbus Master send PDU's buffer length.*/
// �����������͵����ݳ���
void vMBMasterSetPDUSndLength( USHORT SendPDULength )
{
    usMasterSendPDULength = SendPDULength;
}

/* Get Modbus Master send PDU's buffer length.*/
// ��ȡ�������͵����ݳ���
USHORT usMBMasterGetPDUSndLength( void )
{
    return usMasterSendPDULength;
}

/* Set Modbus Master current timer mode.*/
// ����Modbus Master��ǰ��ʱ��ģʽ��
void vMBMasterSetCurTimerMode( eMBMasterTimerMode eMBTimerMode )
{
    eMasterCurTimerMode = eMBTimerMode;
}

/* The master request is broadcast? */
// �������Ƿ�λ�㲥
BOOL xMBMasterRequestIsBroadcast( void ){
    return xFrameIsBroadcast;
}
#endif

