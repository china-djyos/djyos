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
 * File: $Id: mbfuncdisc_m.c,v 1.60 2013/10/15 8:48:20 Armink Add Master Functions  Exp $
 */



/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mb_m.h"
#include "mbframe.h"
#include "mbproto.h"
#include "mbconfig.h"

/* ----------------------- Defines ------------------------------------------*/
#define MB_PDU_REQ_READ_ADDR_OFF            ( MB_PDU_DATA_OFF + 0 )
#define MB_PDU_REQ_READ_DISCCNT_OFF         ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_REQ_READ_SIZE                ( 4 )
#define MB_PDU_FUNC_READ_DISCCNT_OFF        ( MB_PDU_DATA_OFF + 0 )
#define MB_PDU_FUNC_READ_VALUES_OFF         ( MB_PDU_DATA_OFF + 1 )
#define MB_PDU_FUNC_READ_SIZE_MIN           ( 1 )

/* ----------------------- Static functions ---------------------------------*/
eMBException    prveMBError2Exception( eMBErrorCode eErrorCode );

/* ----------------------- Start implementation -----------------------------*/
#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0
#if MB_FUNC_READ_DISCRETE_INPUTS_ENABLED > 0

/**
 * This function will request read discrete inputs.
 *
 * @param ucSndAddr salve address
 * @param usDiscreteAddr discrete start address
 * @param usNDiscreteIn discrete total number
 * @param lTimeOut timeout (-1 will waiting forever)
 *
 * @return error code
 */
eMBMasterReqErrCode
eMBMasterReqReadDiscreteInputs( UCHAR ucSndAddr, USHORT usDiscreteAddr, USHORT usNDiscreteIn, LONG lTimeOut )
{
    UCHAR                 *ucMBFrame;
    eMBMasterReqErrCode    eErrStatus = MB_MRE_NO_ERR;

    if ( ucSndAddr > MB_MASTER_TOTAL_SLAVE_NUM ) eErrStatus = MB_MRE_ILL_ARG;
    else if ( xMBMasterRunResTake( lTimeOut ) == FALSE ) eErrStatus = MB_MRE_MASTER_BUSY;
    else
    {
        vMBMasterGetPDUSndBuf(&ucMBFrame);      // ucMBFrame就是ucMasterRTUSndBuf，下面开始往发送缓冲区写数据
        vMBMasterSetDestAddress(ucSndAddr);
        ucMBFrame[MB_PDU_FUNC_OFF]                 = MB_FUNC_READ_DISCRETE_INPUTS;
        ucMBFrame[MB_PDU_REQ_READ_ADDR_OFF]        = usDiscreteAddr >> 8;
        ucMBFrame[MB_PDU_REQ_READ_ADDR_OFF + 1]    = usDiscreteAddr;
        ucMBFrame[MB_PDU_REQ_READ_DISCCNT_OFF ]    = usNDiscreteIn >> 8;
        ucMBFrame[MB_PDU_REQ_READ_DISCCNT_OFF + 1] = usNDiscreteIn;
        vMBMasterSetPDUSndLength( MB_PDU_SIZE_MIN + MB_PDU_REQ_READ_SIZE );
        ( void ) xMBMasterPortEventPost( EV_MASTER_FRAME_SENT );
        eErrStatus = eMBMasterWaitRequestFinish( );
    }
    return eErrStatus;
}

// 处理从机响应的离散寄存器数据
eMBException
eMBMasterFuncReadDiscreteInputs( UCHAR * pucFrame, USHORT * usLen )
{
    USHORT          usRegAddress;
    USHORT          usDiscreteCnt;
    UCHAR           ucNBytes;
    UCHAR          *ucMBFrame;

    eMBException    eStatus = MB_EX_NONE;
    eMBErrorCode    eRegStatus;

    /* If this request is broadcast, and it's read mode. This request don't need execute. */
    // 如果这个请求是广播的，并且它是读模式。这个请求不需要执行。
    if ( xMBMasterRequestIsBroadcast() )
    {
        eStatus = MB_EX_NONE;
    }
    else if( *usLen >= MB_PDU_SIZE_MIN + MB_PDU_FUNC_READ_SIZE_MIN )
    {
        vMBMasterGetPDUSndBuf(&ucMBFrame);
        usRegAddress = ( USHORT )( ucMBFrame[MB_PDU_REQ_READ_ADDR_OFF] << 8 );
        usRegAddress |= ( USHORT )( ucMBFrame[MB_PDU_REQ_READ_ADDR_OFF + 1] );
        usRegAddress++;

        usDiscreteCnt = ( USHORT )( ucMBFrame[MB_PDU_REQ_READ_DISCCNT_OFF] << 8 );
        usDiscreteCnt |= ( USHORT )( ucMBFrame[MB_PDU_REQ_READ_DISCCNT_OFF + 1] );

        /* Test if the quantity of coils is a multiple of 8. 
        If not last byte is only partially field with unused coils set to zero. */
        /*测试线圈数量是否为8的倍数。
        如果不是，最后一个字节只有部分字段，未使用的线圈设置为零。*/
        // 计算有多少字节数，向上取整
        if( ( usDiscreteCnt & 0x0007 ) != 0 )
        {
            ucNBytes = ( UCHAR )( usDiscreteCnt / 8 + 1 );
        }
        else
        {
            ucNBytes = ( UCHAR )( usDiscreteCnt / 8 );
        }

        /* Check if the number of registers to read is valid. 
        If not return Modbus illegal data value exception.
        检查要读取的寄存器数是否有效。
            如果没有返回Modbus非法数据值异常。
         */
        if ((usDiscreteCnt >= 1) && ucNBytes == pucFrame[MB_PDU_FUNC_READ_DISCCNT_OFF])
        {
            // 请求的输入数据，和应答的字节数一样，表示响应正常
            /* Make callback to fill the buffer. */
            // 把读到的输入状态写到对应的从机寄存器组中
            eRegStatus = eMBMasterRegDiscreteCB( &pucFrame[MB_PDU_FUNC_READ_VALUES_OFF], usRegAddress, usDiscreteCnt );

            /* If an error occured convert it into a Modbus exception. */
            if( eRegStatus != MB_ENOERR )
            {
                eStatus = prveMBError2Exception( eRegStatus );
            }
        }
        else
        {
            eStatus = MB_EX_ILLEGAL_DATA_VALUE;
        }
    }
    else
    {
        /* Can't be a valid read coil register request because the length
         * is incorrect. */
        eStatus = MB_EX_ILLEGAL_DATA_VALUE;
    }
    return eStatus;
}

#endif
#endif
