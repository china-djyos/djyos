/*
 * FreeModbus Libary: user callback functions and buffer define in master mode
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
 * File: $Id: user_mb_app_m.c,v 1.60 2013/11/23 11:49:05 Armink $
 */
#include "user_mb_app_m.h"

/*-----------------------Master mode use these variables----------------------*/
#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0
//Master mode:DiscreteInputs variables
//主模式:离散输入变量
USHORT   usMDiscInStart                             = M_DISCRETE_INPUT_START;   //寄存器起始地址
#if      M_DISCRETE_INPUT_NDISCRETES%8                  //应该是寄存器的位数，以字节位单位，向上取整得到所需要的字节个数
UCHAR    ucMDiscInBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_DISCRETE_INPUT_NDISCRETES/8+1];
#else
// 二维数组，对应各各从机的所有寄存器
UCHAR    ucMDiscInBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_DISCRETE_INPUT_NDISCRETES/8];
#endif
//Master mode:Coils variables
//主模式:线圈变量
USHORT   usMCoilStart                               = M_COIL_START;     //寄存器起始地址
#if      M_COIL_NCOILS%8                        //应该是寄存器的位数，以字节位单位，向上取整得到所需要的字节个数
UCHAR    ucMCoilBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_COIL_NCOILS/8+1];
#else
// 二维数组，对应各各从机的所有寄存器
UCHAR    ucMCoilBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_COIL_NCOILS/8];
#endif
//Master mode:Input Register variables
//主模式:输入寄存器变量
USHORT   usMRegInStart                              = M_REG_INPUT_START;    //寄存器起始地址
// 二维数组，对应各各从机的所有寄存器
USHORT   usMRegInBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_REG_INPUT_NREGS];
//Master mode:Holding Register variables
//主模式:保存寄存器变量
USHORT   usMRegHoldStart                            = M_REG_HOLDING_START;  //寄存器起始地址
// 二维数组，对应各各从机的所有寄存器
USHORT   usMRegHoldBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_REG_HOLDING_NREGS];

static UCHAR ucPortNum = 0;
static ULONG ulPortBaudrate = 0;
static eMBParity ePortParity = 0;

static u8 MasterPollStack[0x800];

/**
 * Modbus master input register callback function.
 *
 * @param pucRegBuffer input register buffer
 * @param usAddress input register address
 * @param usNRegs input register number
 *
 * @return result
 */
eMBErrorCode eMBMasterRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          iRegIndex;
    USHORT *        pusRegInputBuf;
    USHORT          REG_INPUT_START;
    USHORT          REG_INPUT_NREGS;
    USHORT          usRegInStart;

    // 找到对应从机的寄存器组
    pusRegInputBuf = usMRegInBuf[ucMBMasterGetDestAddress() - 1];
    REG_INPUT_START = M_REG_INPUT_START;
    REG_INPUT_NREGS = M_REG_INPUT_NREGS;
    usRegInStart = usMRegInStart;

    /* it already plus one in modbus function method. */
    // 它已经在modbus函数方法中加了1
    usAddress--;

    if ((usAddress >= REG_INPUT_START)
            && (usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS))
    {
        iRegIndex = usAddress - usRegInStart;   //找到寄存器下标
        while (usNRegs > 0)
        {
            // 把寄存器数据写入从机对应的寄存器组中
            pusRegInputBuf[iRegIndex] = *pucRegBuffer++ << 8;
            pusRegInputBuf[iRegIndex] |= *pucRegBuffer++;
            iRegIndex++;    //操作下一个寄存器
            usNRegs--;
        }
    }
    else
    {
        // 返回的寄存器数据范围不正确
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

/**
 * Modbus master holding register callback function.
 *
 * @param pucRegBuffer holding register buffer
 * @param usAddress holding register address
 * @param usNRegs holding register number
 * @param eMode read or write
 *
 * @return result
 */
eMBErrorCode eMBMasterRegHoldingCB(UCHAR * pucRegBuffer, USHORT usAddress,
        USHORT usNRegs, eMBRegisterMode eMode)
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          iRegIndex;
    USHORT *        pusRegHoldingBuf;
    USHORT          REG_HOLDING_START;
    USHORT          REG_HOLDING_NREGS;
    USHORT          usRegHoldStart;

    // 找到对应从机的寄存器组
    pusRegHoldingBuf = usMRegHoldBuf[ucMBMasterGetDestAddress() - 1];
    REG_HOLDING_START = M_REG_HOLDING_START;
    REG_HOLDING_NREGS = M_REG_HOLDING_NREGS;
    usRegHoldStart = usMRegHoldStart;
    /* if mode is read, the master will write the received data to buffer. */
    // 如果是读模式，主机将把接收到的数据写入缓冲区。
    // eMode = MB_REG_WRITE;

    /* it already plus one in modbus function method. */
    usAddress--;

    if ((usAddress >= REG_HOLDING_START)
            && (usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS))
    {
        iRegIndex = usAddress - usRegHoldStart;
        switch (eMode)
        {
        /* read current register values from the protocol stack. */
        /*从协议栈读取当前寄存器的值。*/
        case MB_REG_READ:
            while (usNRegs > 0)
            {
                *pucRegBuffer++ = (UCHAR) (pusRegHoldingBuf[iRegIndex] >> 8);
                *pucRegBuffer++ = (UCHAR) (pusRegHoldingBuf[iRegIndex] & 0xFF);
                iRegIndex++;
                usNRegs--;
            }
            break;
        /* write current register values with new values from the protocol stack. */
        /*用协议栈中的新值写入当前寄存器值。*/
        case MB_REG_WRITE:
            while (usNRegs > 0)
            {
                pusRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                pusRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                iRegIndex++;
                usNRegs--;
            }
            break;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

/**
 * Modbus master coils callback function.
 *
 * @param pucRegBuffer coils buffer
 * @param usAddress coils address
 * @param usNCoils coils number
 * @param eMode read or write
 *
 * @return result
 */
eMBErrorCode eMBMasterRegCoilsCB(UCHAR * pucRegBuffer, USHORT usAddress,
        USHORT usNCoils, eMBRegisterMode eMode)
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          iRegIndex , iRegBitIndex , iNReg;
    UCHAR *         pucCoilBuf;
    USHORT          COIL_START;
    USHORT          COIL_NCOILS;
    USHORT          usCoilStart;
    iNReg =  usNCoils / 8 + 1;

    pucCoilBuf = ucMCoilBuf[ucMBMasterGetDestAddress() - 1];
    COIL_START = M_COIL_START;
    COIL_NCOILS = M_COIL_NCOILS;
    usCoilStart = usMCoilStart;

    /* if mode is read,the master will write the received date to buffer. */
    // eMode = MB_REG_WRITE;

    /* it already plus one in modbus function method. */
    usAddress--;

    if ((usAddress >= COIL_START)
            && (usAddress + usNCoils <= COIL_START + COIL_NCOILS))
    {
        iRegIndex = (USHORT) (usAddress - usCoilStart) / 8;
        iRegBitIndex = (USHORT) (usAddress - usCoilStart) % 8;
        switch (eMode)
        {
         /* read current coil values from the protocol stack. */
        case MB_REG_READ:
            while (iNReg > 0)
            {
                *pucRegBuffer++ = xMBUtilGetBits(&pucCoilBuf[iRegIndex++],
                        iRegBitIndex, 8);
                iNReg--;
            }
            pucRegBuffer--;
            /* last coils */
            usNCoils = usNCoils % 8;
            /* filling zero to high bit */
            *pucRegBuffer = *pucRegBuffer << (8 - usNCoils);
            *pucRegBuffer = *pucRegBuffer >> (8 - usNCoils);
            break;

        /* write current coil values with new values from the protocol stack. */
        case MB_REG_WRITE:
            while (iNReg > 1)
            {
                xMBUtilSetBits(&pucCoilBuf[iRegIndex++], iRegBitIndex, 8,
                        *pucRegBuffer++);
                iNReg--;
            }
            /* last coils */
            usNCoils = usNCoils % 8;
            /* xMBUtilSetBits has bug when ucNBits is zero */
            if (usNCoils != 0)
            {
                xMBUtilSetBits(&pucCoilBuf[iRegIndex++], iRegBitIndex, usNCoils,
                        *pucRegBuffer++);
            }
            break;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

/**
 * Modbus master discrete callback function.
 *
 * @param pucRegBuffer discrete buffer
 * @param usAddress discrete address
 * @param usNDiscrete discrete number
 *
 * @return result
 */
eMBErrorCode eMBMasterRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          iRegIndex , iRegBitIndex , iNReg;
    UCHAR *         pucDiscreteInputBuf;
    USHORT          DISCRETE_INPUT_START;
    USHORT          DISCRETE_INPUT_NDISCRETES;
    USHORT          usDiscreteInputStart;
    iNReg =  usNDiscrete / 8 + 1;

    pucDiscreteInputBuf = ucMDiscInBuf[ucMBMasterGetDestAddress() - 1];
    DISCRETE_INPUT_START = M_DISCRETE_INPUT_START;
    DISCRETE_INPUT_NDISCRETES = M_DISCRETE_INPUT_NDISCRETES;
    usDiscreteInputStart = usMDiscInStart;

    /* it already plus one in modbus function method. */
    usAddress--;

    if ((usAddress >= DISCRETE_INPUT_START)
            && (usAddress + usNDiscrete    <= DISCRETE_INPUT_START + DISCRETE_INPUT_NDISCRETES))
    {
        iRegIndex = (USHORT) (usAddress - usDiscreteInputStart) / 8;
        iRegBitIndex = (USHORT) (usAddress - usDiscreteInputStart) % 8;

        /* write current discrete values with new values from the protocol stack. */
        while (iNReg > 1)
        {
            xMBUtilSetBits(&pucDiscreteInputBuf[iRegIndex++], iRegBitIndex, 8,
                    *pucRegBuffer++);
            iNReg--;
        }
        /* last discrete */
        usNDiscrete = usNDiscrete % 8;
        /* xMBUtilSetBits has bug when ucNBits is zero */
        if (usNDiscrete != 0)
        {
            xMBUtilSetBits(&pucDiscreteInputBuf[iRegIndex++], iRegBitIndex,
                    usNDiscrete, *pucRegBuffer++);
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

static ptu32_t mb_master_poll(void)
{
    // 主机模式暂时只支持RTU
    eMBMasterInit(MB_RTU, ucPortNum, ulPortBaudrate, ePortParity);
    eMBMasterEnable();

    while (1)
    {
        eMBMasterPoll();
    }

    return 0;
}

// =============================================================================
// 函数功能:modbus主机功能初始化，目前主机只支持RTU
// 输入参数:无
// 返回值 : true -- 成功；false -- 失败
// =============================================================================
bool_t ModuleInstall_ModbusMaster(UCHAR ucNum, ULONG ulBaudrate, eMBParity eParity)
{
    u16 EvttID = 0;

    EvttID = DJY_EvttRegist(EN_INDEPENDENCE, 189, 0, 1, (void *)mb_master_poll, 
                                    MasterPollStack, sizeof(MasterPollStack), "master poll");
    if (CN_EVTT_ID_INVALID != EvttID)
    {
        ucPortNum = ucNum;
        ulPortBaudrate = ulBaudrate;
        ePortParity = eParity;
        if (CN_EVENT_ID_INVALID != DJY_EventPop(EvttID, NULL, 0, 0, 0, 0))
        {
            while (1)
            {
                if (eMBMasterIsEstablished())
                {
                    break;
                }
                else
                {
                    DJY_EventDelay(2000);
                }
            }
            return TRUE;
        }
        else
        {
            error_printf("modbus", "master poll event pop fail\r\n");
            DJY_EvttUnregist(EvttID);
        }
    }
    else
    {
        error_printf("modbus", "master poll Evtt regist fail\r\n");
    }

    return FALSE;
}

// 获取指定从机指定地址的保持寄存器值
eMBErrorCode eMBMasterRegHoldingCB_Get(UCHAR ucSndAddr, UCHAR * pucRegBuffer, USHORT usAddress,
        USHORT usNRegs)
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          iRegIndex;
    USHORT *        pusRegHoldingBuf;
    USHORT          REG_HOLDING_START;
    USHORT          REG_HOLDING_NREGS;
    USHORT          usRegHoldStart;

    if ((ucSndAddr - 1) >= MB_MASTER_TOTAL_SLAVE_NUM)
    {
        return MB_EINVAL;
    }

    // 找到对应从机的寄存器组
    pusRegHoldingBuf = usMRegHoldBuf[ucSndAddr - 1];
    REG_HOLDING_START = M_REG_HOLDING_START;
    REG_HOLDING_NREGS = M_REG_HOLDING_NREGS;
    usRegHoldStart = usMRegHoldStart;

    if ((usAddress >= REG_HOLDING_START)
            && (usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS))
    {
        iRegIndex = usAddress - usRegHoldStart;

        /* read current register values from the protocol stack. */
        /*从协议栈读取当前寄存器的值。*/
        memcpy(pucRegBuffer, &pusRegHoldingBuf[iRegIndex], usNRegs * sizeof(USHORT));
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

// 获取指定从机指定地址的输入寄存器值
eMBErrorCode eMBMasterRegInputCB_Get(UCHAR ucSndAddr, UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          iRegIndex;
    USHORT *        pusRegInputBuf;
    USHORT          REG_INPUT_START;
    USHORT          REG_INPUT_NREGS;
    USHORT          usRegInStart;

    if ((ucSndAddr - 1) >= MB_MASTER_TOTAL_SLAVE_NUM)
    {
        return MB_EINVAL;
    }

    // 找到对应从机的寄存器组
    pusRegInputBuf = usMRegInBuf[ucSndAddr- 1];
    REG_INPUT_START = M_REG_INPUT_START;
    REG_INPUT_NREGS = M_REG_INPUT_NREGS;
    usRegInStart = usMRegInStart;

    if ((usAddress >= REG_INPUT_START)
            && (usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS))
    {
        iRegIndex = usAddress - usRegInStart;   //找到寄存器下标
        memcpy(pucRegBuffer, &pusRegInputBuf[iRegIndex], usNRegs * sizeof(USHORT));
    }
    else
    {
        // 返回的寄存器数据范围不正确
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

// 获取指定从机指定地址的线圈
eMBErrorCode eMBMasterRegCoilsCB_Get(UCHAR ucSndAddr, UCHAR * pucRegBuffer, USHORT usAddress,
        USHORT usNCoils)
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          iRegIndex , iRegBitIndex , iNReg;
    UCHAR *         pucCoilBuf;
    USHORT          COIL_START;
    USHORT          COIL_NCOILS;
    USHORT          usCoilStart;
    iNReg =  usNCoils / 8 + 1;

    if ((ucSndAddr - 1) >= MB_MASTER_TOTAL_SLAVE_NUM)
    {
        return MB_EINVAL;
    }

    pucCoilBuf = ucMCoilBuf[ucSndAddr - 1];
    COIL_START = M_COIL_START;
    COIL_NCOILS = M_COIL_NCOILS;
    usCoilStart = usMCoilStart;

    /* if mode is read,the master will write the received date to buffer. */
    // eMode = MB_REG_WRITE;

    if ((usAddress >= COIL_START)
            && (usAddress + usNCoils <= COIL_START + COIL_NCOILS))
    {
        iRegIndex = (USHORT) (usAddress - usCoilStart) / 8;
        iRegBitIndex = (USHORT) (usAddress - usCoilStart) % 8;

         /* read current coil values from the protocol stack. */
        while (iNReg > 0)
        {
            *pucRegBuffer++ = xMBUtilGetBits(&pucCoilBuf[iRegIndex++],
                    iRegBitIndex, 8);
            iNReg--;
        }
        pucRegBuffer--;
        /* last coils */
        usNCoils = usNCoils % 8;
        /* filling zero to high bit */
        *pucRegBuffer = *pucRegBuffer << (8 - usNCoils);
        *pucRegBuffer = *pucRegBuffer >> (8 - usNCoils);
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

// 获取指定从机指定地址的离散输入
eMBErrorCode eMBMasterRegDiscreteCB_Get(UCHAR ucSndAddr, UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          iRegIndex , iRegBitIndex , iNReg;
    UCHAR *         pucDiscreteInputBuf;
    USHORT          DISCRETE_INPUT_START;
    USHORT          DISCRETE_INPUT_NDISCRETES;
    USHORT          usDiscreteInputStart;
    iNReg =  usNDiscrete / 8 + 1;

    if ((ucSndAddr - 1) >= MB_MASTER_TOTAL_SLAVE_NUM)
    {
        return MB_EINVAL;
    }

    pucDiscreteInputBuf = ucMDiscInBuf[ucSndAddr - 1];
    DISCRETE_INPUT_START = M_DISCRETE_INPUT_START;
    DISCRETE_INPUT_NDISCRETES = M_DISCRETE_INPUT_NDISCRETES;
    usDiscreteInputStart = usMDiscInStart;

    if ((usAddress >= DISCRETE_INPUT_START)
            && (usAddress + usNDiscrete    <= DISCRETE_INPUT_START + DISCRETE_INPUT_NDISCRETES))
    {
        iRegIndex = (USHORT) (usAddress - usDiscreteInputStart) / 8;
        iRegBitIndex = (USHORT) (usAddress - usDiscreteInputStart) % 8;

        /* write current discrete values with new values from the protocol stack. */
        while (iNReg > 1)
        {
            xMBUtilSetBits(&pucDiscreteInputBuf[iRegIndex++], iRegBitIndex, 8,
                    *pucRegBuffer++);
            iNReg--;
        }
        /* last discrete */
        usNDiscrete = usNDiscrete % 8;
        /* xMBUtilSetBits has bug when ucNBits is zero */
        if (usNDiscrete != 0)
        {
            xMBUtilSetBits(&pucDiscreteInputBuf[iRegIndex++], iRegBitIndex,
                    usNDiscrete, *pucRegBuffer++);
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

#endif
