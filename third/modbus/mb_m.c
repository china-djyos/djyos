/*
 * FreeModbus Libary: A portable Modbus implementation for Modbus ASCII/RTU.
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
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
 * File: $Id: mbrtu_m.c,v 1.60 2013/08/20 11:18:10 Armink Add Master Functions $
 */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/

#include "mb.h"
#include "mb_m.h"
#include "mbconfig.h"
#include "mbframe.h"
#include "mbproto.h"
#include "mbfunc.h"

#include "mbport.h"
#if MB_MASTER_RTU_ENABLED == 1
#include "mbrtu.h"
#endif
#if MB_MASTER_ASCII_ENABLED == 1
#include "mbascii.h"
#endif
#if MB_MASTER_TCP_ENABLED == 1
#include "mbtcp.h"
#endif

#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0

#ifndef MB_PORT_HAS_CLOSE
#define MB_PORT_HAS_CLOSE 0
#endif

/* ----------------------- Static variables ---------------------------------*/

static UCHAR    ucMBMasterDestAddress;      //
static BOOL     xMBRunInMasterMode = FALSE;
static eMBMasterErrorEventType eMBMasterCurErrorType;

// modbus状态
static enum
{
    STATE_ENABLED,          //使能
    STATE_DISABLED,         //失能
    STATE_NOT_INITIALIZED,  //未初始化
    STATE_ESTABLISHED,      //连接状态
} eMBState = STATE_NOT_INITIALIZED;

/* Functions pointer which are initialized in eMBInit( ). Depending on the
 * mode (RTU or ASCII) the are set to the correct implementations.
 * Using for Modbus Master,Add by Armink 20130813
 */
static peMBFrameSend peMBMasterFrameSendCur;
static pvMBFrameStart pvMBMasterFrameStartCur;
static pvMBFrameStop pvMBMasterFrameStopCur;
static peMBFrameReceive peMBMasterFrameReceiveCur;
static pvMBFrameClose pvMBMasterFrameCloseCur;

/* Callback functions required by the porting layer. They are called when
 * an external event has happend which includes a timeout or the reception
 * or transmission of a character.
 * Using for Modbus Master,Add by Armink 20130813
 */
BOOL( *pxMBMasterFrameCBByteReceived ) ( void );
BOOL( *pxMBMasterFrameCBTransmitterEmpty ) ( void );
BOOL( *pxMBMasterPortCBTimerExpired ) ( void );

BOOL( *pxMBMasterFrameCBReceiveFSMCur ) ( void );
BOOL( *pxMBMasterFrameCBTransmitFSMCur ) ( void );

/* An array of Modbus functions handlers which associates Modbus function
 * codes with implementing functions.
 */
static xMBFunctionHandler xMasterFuncHandlers[MB_FUNC_HANDLERS_MAX] = {
#if MB_FUNC_OTHER_REP_SLAVEID_ENABLED > 0
    //TODO Add Master function define
    {MB_FUNC_OTHER_REPORT_SLAVEID, eMBFuncReportSlaveID},
#endif
#if MB_FUNC_READ_INPUT_ENABLED > 0
    {MB_FUNC_READ_INPUT_REGISTER, eMBMasterFuncReadInputRegister},
#endif
#if MB_FUNC_READ_HOLDING_ENABLED > 0
    {MB_FUNC_READ_HOLDING_REGISTER, eMBMasterFuncReadHoldingRegister},
#endif
#if MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED > 0
    {MB_FUNC_WRITE_MULTIPLE_REGISTERS, eMBMasterFuncWriteMultipleHoldingRegister},
#endif
#if MB_FUNC_WRITE_HOLDING_ENABLED > 0
    {MB_FUNC_WRITE_REGISTER, eMBMasterFuncWriteHoldingRegister},
#endif
#if MB_FUNC_READWRITE_HOLDING_ENABLED > 0
    {MB_FUNC_READWRITE_MULTIPLE_REGISTERS, eMBMasterFuncReadWriteMultipleHoldingRegister},
#endif
#if MB_FUNC_READ_COILS_ENABLED > 0
    {MB_FUNC_READ_COILS, eMBMasterFuncReadCoils},
#endif
#if MB_FUNC_WRITE_COIL_ENABLED > 0
    {MB_FUNC_WRITE_SINGLE_COIL, eMBMasterFuncWriteCoil},
#endif
#if MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED > 0
    {MB_FUNC_WRITE_MULTIPLE_COILS, eMBMasterFuncWriteMultipleCoils},
#endif
#if MB_FUNC_READ_DISCRETE_INPUTS_ENABLED > 0
    {MB_FUNC_READ_DISCRETE_INPUTS, eMBMasterFuncReadDiscreteInputs},
#endif
};

/* ----------------------- Start implementation -----------------------------*/
eMBErrorCode
eMBMasterInit( eMBMode eMode, UCHAR ucPort, ULONG ulBaudRate, eMBParity eParity )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    switch (eMode)
    {
#if MB_MASTER_RTU_ENABLED > 0
    case MB_RTU:
        pvMBMasterFrameStartCur = eMBMasterRTUStart;
        pvMBMasterFrameStopCur = eMBMasterRTUStop;
        peMBMasterFrameSendCur = eMBMasterRTUSend;
        peMBMasterFrameReceiveCur = eMBMasterRTUReceive;
        pvMBMasterFrameCloseCur = MB_PORT_HAS_CLOSE ? vMBMasterPortClose : NULL;
        pxMBMasterFrameCBByteReceived = xMBMasterRTUReceiveFSM;
        pxMBMasterFrameCBTransmitterEmpty = xMBMasterRTUTransmitFSM;
        pxMBMasterPortCBTimerExpired = xMBMasterRTUTimerExpired;

        eStatus = eMBMasterRTUInit(ucPort, ulBaudRate, eParity);
        break;
#endif
#if MB_MASTER_ASCII_ENABLED > 0
        case MB_ASCII:
        pvMBMasterFrameStartCur = eMBMasterASCIIStart;
        pvMBMasterFrameStopCur = eMBMasterASCIIStop;
        peMBMasterFrameSendCur = eMBMasterASCIISend;
        peMBMasterFrameReceiveCur = eMBMasterASCIIReceive;
        pvMBMasterFrameCloseCur = MB_PORT_HAS_CLOSE ? vMBMasterPortClose : NULL;
        pxMBMasterFrameCBByteReceived = xMBMasterASCIIReceiveFSM;
        pxMBMasterFrameCBTransmitterEmpty = xMBMasterASCIITransmitFSM;
        pxMBMasterPortCBTimerExpired = xMBMasterASCIITimerT1SExpired;

        eStatus = eMBMasterASCIIInit(ucPort, ulBaudRate, eParity );
        break;
#endif
    default:
        eStatus = MB_EINVAL;
        break;
    }

    if (eStatus == MB_ENOERR)
    {
        if (!xMBMasterPortEventInit())  //初始化主机的事件类型
        {
            /* port dependent event module initalization failed. */
            eStatus = MB_EPORTERR;
        }
        else
        {
            eMBState = STATE_DISABLED;
        }
        /* initialize the OS resource for modbus master. */
        vMBMasterOsResInit();       //就是初始化信号量
    }
    return eStatus;
}

eMBErrorCode
eMBMasterClose( void )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    if( eMBState == STATE_DISABLED )
    {
        if( pvMBMasterFrameCloseCur != NULL )
        {
            pvMBMasterFrameCloseCur(  );
        }
    }
    else
    {
        eStatus = MB_EILLSTATE;
    }
    return eStatus;
}

eMBErrorCode
eMBMasterEnable( void )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    if( eMBState == STATE_DISABLED )
    {
        /* Activate the protocol stack. */
        pvMBMasterFrameStartCur(  );
        eMBState = STATE_ENABLED;
    }
    else
    {
        eStatus = MB_EILLSTATE;
    }
    return eStatus;
}

eMBErrorCode
eMBMasterDisable( void )
{
    eMBErrorCode    eStatus;

    if(( eMBState == STATE_ENABLED ) || ( eMBState == STATE_ESTABLISHED))
    {
        pvMBMasterFrameStopCur(  );
        eMBState = STATE_DISABLED;
        eStatus = MB_ENOERR;
    }
    else if( eMBState == STATE_DISABLED )
    {
        eStatus = MB_ENOERR;
    }
    else
    {
        eStatus = MB_EILLSTATE;
    }
    return eStatus;
}

BOOL
eMBMasterIsEstablished( void )
{
    if(eMBState == STATE_ESTABLISHED)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


eMBErrorCode
eMBMasterPoll( void )
{
    static UCHAR   *ucMBFrame;
    static UCHAR    ucRcvAddress;
    static UCHAR    ucFunctionCode;
    static USHORT   usLength;
    static eMBException eException;

    int             i , j;
    eMBErrorCode    eStatus = MB_ENOERR;
    eMBMasterEventType    eEvent;
    eMBMasterErrorEventType errorType;

    /* Check if the protocol stack is ready. */
    if(( eMBState != STATE_ENABLED ) && ( eMBState != STATE_ESTABLISHED))
    {
        return MB_EILLSTATE;
    }

    /* Check if there is a event available. If not return control to caller.
     * Otherwise we will handle the event. */
    if( xMBMasterPortEventGet( &eEvent ) == TRUE )
    {
        switch ( eEvent )
        {
        case EV_MASTER_READY:
            eMBState = STATE_ESTABLISHED;
            break;

        case EV_MASTER_FRAME_RECEIVED:
            eStatus = peMBMasterFrameReceiveCur( &ucRcvAddress, &ucMBFrame, &usLength );
            /* Check if the frame is for us. If not ,send an error process event. */
            // 看看这个帧是不是为我们准备的。如果不是，则发送错误处理事件。
            if ( ( eStatus == MB_ENOERR ) && ( ucRcvAddress == ucMBMasterGetDestAddress() ) )
            {
                // 是发给我们的,去执行回调函数
                ( void ) xMBMasterPortEventPost( EV_MASTER_EXECUTE );
            }
            else
            {
                // 不是发给我们的,标记错误处理
                vMBMasterSetErrorType(EV_ERROR_RECEIVE_DATA);
                ( void ) xMBMasterPortEventPost( EV_MASTER_ERROR_PROCESS );
            }
            break;

        case EV_MASTER_EXECUTE:
            ucFunctionCode = ucMBFrame[MB_PDU_FUNC_OFF];
            eException = MB_EX_ILLEGAL_FUNCTION;
            /* If receive frame has exception .The receive function code highest bit is 1.*/
            // 如果接收帧有异常，接收函数码最高位为1
            if(ucFunctionCode >> 7) {
                // 接收到的功能码最高位是1,表示响应错误,从应答数据中得到异常码
                eException = (eMBException)ucMBFrame[MB_PDU_DATA_OFF];
            }
            else
            {
                for (i = 0; i < MB_FUNC_HANDLERS_MAX; i++)
                {
                    /* No more function handlers registered. Abort. */
                    // 遍历到功能号是0的地方,那后面的都不用再看了,肯定都是0
                    if (xMasterFuncHandlers[i].ucFunctionCode == 0) {
                        break;
                    }
                    else if (xMasterFuncHandlers[i].ucFunctionCode == ucFunctionCode) {
                        vMBMasterSetCBRunInMasterMode(TRUE);
                        /* If master request is broadcast, the master need execute function for all slave.
                        如果主请求是广播的，主请求需要对所有从请求执行函数。
                         */
                        if ( xMBMasterRequestIsBroadcast() ) {
                            // 广播的话，对所有从机执行一遍功能号对应的回调函数，去处理数据，但数据不是收到的数据
                            // 因为从机不会应答广播，而是用发送的数据去处理
                            usLength = usMBMasterGetPDUSndLength();
                            for(j = 1; j <= MB_MASTER_TOTAL_SLAVE_NUM; j++){
                                vMBMasterSetDestAddress(j);     //设置当前发送数据的目标地址
                                eException = xMasterFuncHandlers[i].pxHandler(ucMBFrame, &usLength);
                            }
                        }
                        else {
                            // 处理功能号对应的数据
                            eException = xMasterFuncHandlers[i].pxHandler(ucMBFrame, &usLength);
                        }
                        vMBMasterSetCBRunInMasterMode(FALSE);
                        break;
                    }
                }
            }
            /* If master has exception ,Master will send error process.Otherwise the Master is idle.*/
            if (eException != MB_EX_NONE) {
                vMBMasterSetErrorType(EV_ERROR_EXECUTE_FUNCTION);
                ( void ) xMBMasterPortEventPost( EV_MASTER_ERROR_PROCESS );
            }
            else {
                vMBMasterCBRequestScuuess( );
                vMBMasterRunResRelease( );      //释放信号量
            }
            break;

        case EV_MASTER_FRAME_SENT:
            /* Master is busy now. */
            vMBMasterGetPDUSndBuf( &ucMBFrame );
            // 得到目的地址,待发送的数据和发送数据的长度.把数据发送出去
            eStatus = peMBMasterFrameSendCur( ucMBMasterGetDestAddress(), ucMBFrame, usMBMasterGetPDUSndLength() );
            break;

        case EV_MASTER_ERROR_PROCESS:
            /* Execute specified error process callback function. */
            // 执行指定的错误处理回调函数。
            errorType = eMBMasterGetErrorType();    //得到错误类型
            vMBMasterGetPDUSndBuf( &ucMBFrame );
            switch (errorType) {
            case EV_ERROR_RESPOND_TIMEOUT:
            // 得到目的地址,待发送的数据和发送数据的长度
            // 但实际上只是设置了请求应答超时错误
                vMBMasterErrorCBRespondTimeout(ucMBMasterGetDestAddress(),
                        ucMBFrame, usMBMasterGetPDUSndLength());
                break;
            case EV_ERROR_RECEIVE_DATA:
            // 数据不是发给我们的,设置接收数据错误标志
                vMBMasterErrorCBReceiveData(ucMBMasterGetDestAddress(),
                        ucMBFrame, usMBMasterGetPDUSndLength());
                break;
            case EV_ERROR_EXECUTE_FUNCTION:
            // 解析数据发生异常,设置执行函数异常错误
                vMBMasterErrorCBExecuteFunction(ucMBMasterGetDestAddress(),
                        ucMBFrame, usMBMasterGetPDUSndLength());
                break;
            }
            vMBMasterRunResRelease();   //释放信号量
            break;

        default:
            break;
        }

    }
    return MB_ENOERR;
}

// xMBMasterGetCBRunInMasterMode和vMBMasterSetCBRunInMasterMode没有实际的功能，仅仅只是标记
/* Get whether the Modbus Master is run in master mode.*/
// 获取Modbus Master是否以主模式运行。
BOOL xMBMasterGetCBRunInMasterMode( void )
{
    return xMBRunInMasterMode;
}
/* Set whether the Modbus Master is run in master mode.*/
// 设置Modbus Master是否以主模式运行。
void vMBMasterSetCBRunInMasterMode( BOOL IsMasterMode )
{
    xMBRunInMasterMode = IsMasterMode;
}
/* Get Modbus Master send destination address. */
// 获取Modbus Master发送目的地址。
UCHAR ucMBMasterGetDestAddress( void )
{
    return ucMBMasterDestAddress;
}
/* Set Modbus Master send destination address. */
// 设置Modbus Master发送目的地址。
void vMBMasterSetDestAddress( UCHAR Address )
{
    ucMBMasterDestAddress = Address;
}
/* Get Modbus Master current error event type. */
// 获取主机当前的错误事件类型
eMBMasterErrorEventType eMBMasterGetErrorType( void )
{
    return eMBMasterCurErrorType;
}
/* Set Modbus Master current error event type. */
// 设置主机当前的错误事件类型
void vMBMasterSetErrorType( eMBMasterErrorEventType errorType )
{
    eMBMasterCurErrorType = errorType;
}



#endif
