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
 * File: $Id: mbproto.h,v 1.14 2006/12/07 22:10:34 wolti Exp $
 */

#ifndef _MB_PROTO_H
#define _MB_PROTO_H

#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif
/* ----------------------- Defines ------------------------------------------*/
#define MB_ADDRESS_BROADCAST    ( 0 )   /*! Modbus broadcast address. 广播地址*/
#define MB_ADDRESS_MIN          ( 1 )   /*! Smallest possible slave address. 最小可能的从属地址*/
#define MB_ADDRESS_MAX          ( 247 ) /*! Biggest possible slave address. 最大可能的从属地址*/
#define MB_FUNC_NONE                          (  0 )
#define MB_FUNC_READ_COILS                    (  1 )        //读线圈
#define MB_FUNC_READ_DISCRETE_INPUTS          (  2 )        //读离散输入
#define MB_FUNC_WRITE_SINGLE_COIL             (  5 )        //写单个线圈
#define MB_FUNC_WRITE_MULTIPLE_COILS          ( 15 )        //写多个线圈
#define MB_FUNC_READ_HOLDING_REGISTER         (  3 )        //读多个保持寄存器
#define MB_FUNC_READ_INPUT_REGISTER           (  4 )        //读输入寄存器
#define MB_FUNC_WRITE_REGISTER                (  6 )        //写单个寄存器
#define MB_FUNC_WRITE_MULTIPLE_REGISTERS      ( 16 )        //写多个寄存器
#define MB_FUNC_READWRITE_MULTIPLE_REGISTERS  ( 23 )        //读写多个寄存器
#define MB_FUNC_DIAG_READ_EXCEPTION           (  7 )        //未实现
#define MB_FUNC_DIAG_DIAGNOSTIC               (  8 )        //未实现
#define MB_FUNC_DIAG_GET_COM_EVENT_CNT        ( 11 )        //未实现
#define MB_FUNC_DIAG_GET_COM_EVENT_LOG        ( 12 )        //为实现
#define MB_FUNC_OTHER_REPORT_SLAVEID          ( 17 )        //报告从机ID
#define MB_FUNC_ERROR                         ( 128 )
/* ----------------------- Type definitions ---------------------------------*/
    typedef enum
{
    MB_EX_NONE = 0x00,
    MB_EX_ILLEGAL_FUNCTION = 0x01,              //非法函数
    MB_EX_ILLEGAL_DATA_ADDRESS = 0x02,          //非法数据地址
    MB_EX_ILLEGAL_DATA_VALUE = 0x03,            //非法数据值
    MB_EX_SLAVE_DEVICE_FAILURE = 0x04,          //从机故障
    MB_EX_ACKNOWLEDGE = 0x05,
    MB_EX_SLAVE_BUSY = 0x06,                    //从机忙
    MB_EX_MEMORY_PARITY_ERROR = 0x08,           //奇偶校验错误
    MB_EX_GATEWAY_PATH_FAILED = 0x0A,           //网关路径失败
    MB_EX_GATEWAY_TGT_FAILED = 0x0B             //网关目标失败
} eMBException;

typedef         eMBException( *pxMBFunctionHandler ) ( UCHAR * pucFrame, USHORT * pusLength );

typedef struct
{
    UCHAR           ucFunctionCode;
    pxMBFunctionHandler pxHandler;
} xMBFunctionHandler;

#ifdef __cplusplus
PR_END_EXTERN_C
#endif
#endif
