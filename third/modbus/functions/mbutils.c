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
        // �����usBitOffset��ʼ����λֵ�ĵ�һ���ֽڵ��ֽ�ƫ������
        usByteOffset = ( USHORT )( ( usBitOffset ) / BITS_UCHAR );

        /* How many bits precede our bits to set. */
        // Ҫ���õ�λ���������ֽ��У�ǰ�ж���λ
        usNPreBits = ( USHORT )( usBitOffset - usByteOffset * BITS_UCHAR );

        /* Move bit field into position over bits to set */
        // ��λ�ֶ��ƶ���Ҫ���õ�λ֮��
        usValue <<= usNPreBits;

        /* Prepare a mask for setting the new bits. */
        // Ϊ������λ׼��һ�����롣
        usMask = ( USHORT )( ( 1 << ( USHORT ) ucNBits ) - 1 );
        usMask <<= usBitOffset - usByteOffset * BITS_UCHAR;

        /* copy bits into temporary storage. ����λ����ʱ�洢*/
        usWordBuf = ucByteBuf[usByteOffset];
        usWordBuf |= ucByteBuf[usByteOffset + 1] << BITS_UCHAR;

        /* Zero out bit field bits and then or value bits into them. */
        // ��λ�ֶ�λ���㣬Ȼ��ֵλ��������
        usWordBuf = ( USHORT )( ( usWordBuf & ( ~usMask ) ) | usValue );

        /* move bits back into storage  ��λ�ƻش洢*/
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
    // �����usBitOffset��ʼ����λֵ�ĵ�һ���ֽڵ��ֽ�ƫ������
    usByteOffset = ( USHORT )( ( usBitOffset ) / BITS_UCHAR );

    /* How many bits precede our bits to set. */
    /*Ҫ���õ�λǰ���ж���λ��*/
    usNPreBits = ( USHORT )( usBitOffset - usByteOffset * BITS_UCHAR );

    /* Prepare a mask for setting the new bits. */
    /*Ϊ������λ׼��һ�����롣*/
    usMask = ( USHORT )( ( 1 << ( USHORT ) ucNBits ) - 1 );

    /* copy bits into temporary storage. */
    /*����λ����ʱ�洢��*/
    usWordBuf = ucByteBuf[usByteOffset];
    usWordBuf |= ucByteBuf[usByteOffset + 1] << BITS_UCHAR;

    /* throw away unneeded bits. */
    /*�ӵ�����Ҫ��λ��*/
    usWordBuf >>= usNPreBits;

    /* mask away bits above the requested bitfield. */
    /*���ε�����λ�����ϵ�λ��*/
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
