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
 * File: $Id: mbutils.c,v 1.6 2007/02/18 23:49:07 wolti Exp $
 */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbproto.h"

/* ----------------------- Defines ------------------------------------------*/
#define BITS_UCHAR      8U

/* ----------------------- Start implementation -----------------------------*/
void
xMBUtilSetBits( UCHAR * ucByteBuf, USHORT usBitOffset, UCHAR ucNBits,
                UCHAR ucValue )
{
    USHORT          usWordBuf;
    USHORT          usMask;
    USHORT          usByteOffset;
    USHORT          usNPreBits;
    USHORT          usValue = ucValue;

    // RT_ASSERT( ucNBits <= 8 );
    // RT_ASSERT( ( size_t )BITS_UCHAR == sizeof( UCHAR ) * 8 );

    if ((ucNBits <= 8) && (( size_t )BITS_UCHAR == sizeof( UCHAR ) * 8))
    {
        /* Calculate byte offset for first byte containing the bit values starting
        * at usBitOffset. */
        // 计算从usBitOffset开始包含位值的第一个字节的字节偏移量。
        usByteOffset = ( USHORT )( ( usBitOffset ) / BITS_UCHAR );

        /* How many bits precede our bits to set. */
        // 要设置的位置所属的字节中，前有多少位
        usNPreBits = ( USHORT )( usBitOffset - usByteOffset * BITS_UCHAR );

        /* Move bit field into position over bits to set */
        // 将位字段移动到要设置的位之上
        usValue <<= usNPreBits;

        /* Prepare a mask for setting the new bits. */
        // 为设置新位准备一个掩码。
        usMask = ( USHORT )( ( 1 << ( USHORT ) ucNBits ) - 1 );
        usMask <<= usBitOffset - usByteOffset * BITS_UCHAR;

        /* copy bits into temporary storage. 复制位到临时存储*/
        usWordBuf = ucByteBuf[usByteOffset];
        usWordBuf |= ucByteBuf[usByteOffset + 1] << BITS_UCHAR;

        /* Zero out bit field bits and then or value bits into them. */
        // 将位字段位置零，然后将值位放入其中
        usWordBuf = ( USHORT )( ( usWordBuf & ( ~usMask ) ) | usValue );

        /* move bits back into storage  将位移回存储*/
        ucByteBuf[usByteOffset] = ( UCHAR )( usWordBuf & 0xFF );
        ucByteBuf[usByteOffset + 1] = ( UCHAR )( usWordBuf >> BITS_UCHAR );
    }
    else
    {
        error_printf("modbus", "param is error\r\n");
    }

}

UCHAR
xMBUtilGetBits( UCHAR * ucByteBuf, USHORT usBitOffset, UCHAR ucNBits )
{
    USHORT          usWordBuf;
    USHORT          usMask;
    USHORT          usByteOffset;
    USHORT          usNPreBits;

    /* Calculate byte offset for first byte containing the bit values starting
     * at usBitOffset. */
    // 计算从usBitOffset开始包含位值的第一个字节的字节偏移量。
    usByteOffset = ( USHORT )( ( usBitOffset ) / BITS_UCHAR );

    /* How many bits precede our bits to set. */
    /*要设置的位前面有多少位。*/
    usNPreBits = ( USHORT )( usBitOffset - usByteOffset * BITS_UCHAR );

    /* Prepare a mask for setting the new bits. */
    /*为设置新位准备一个掩码。*/
    usMask = ( USHORT )( ( 1 << ( USHORT ) ucNBits ) - 1 );

    /* copy bits into temporary storage. */
    /*复制位到临时存储。*/
    usWordBuf = ucByteBuf[usByteOffset];
    usWordBuf |= ucByteBuf[usByteOffset + 1] << BITS_UCHAR;

    /* throw away unneeded bits. */
    /*扔掉不需要的位。*/
    usWordBuf >>= usNPreBits;

    /* mask away bits above the requested bitfield. */
    /*屏蔽掉请求位域以上的位。*/
    usWordBuf &= usMask;

    return ( UCHAR ) usWordBuf;
}

eMBException
prveMBError2Exception( eMBErrorCode eErrorCode )
{
    eMBException    eStatus;

    switch ( eErrorCode )
    {
        case MB_ENOERR:
            eStatus = MB_EX_NONE;
            break;

        case MB_ENOREG:
            eStatus = MB_EX_ILLEGAL_DATA_ADDRESS;
            break;

        case MB_ETIMEDOUT:
            eStatus = MB_EX_SLAVE_BUSY;
            break;

        default:
            eStatus = MB_EX_SLAVE_DEVICE_FAILURE;
            break;
    }

    return eStatus;
}
