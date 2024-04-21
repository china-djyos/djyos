/*
 * FreeModbus Libary: user callback functions and buffer define in slave mode
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
 * File: $Id: user_mb_app.c,v 1.60 2013/11/23 11:49:05 Armink $
 */
#include "user_mb_app.h"

/*------------------------Slave mode use these variables----------------------*/
//Slave mode:DiscreteInputs variables
// 从模式:离散输入变量
USHORT   usSDiscInStart                               = S_DISCRETE_INPUT_START; //寄存器起始地址
#if S_DISCRETE_INPUT_NDISCRETES%8               //应该是寄存器的位数，以字节位单位，向上取整得到所需要的字节个数
UCHAR    ucSDiscInBuf[S_DISCRETE_INPUT_NDISCRETES/8+1];
#else
UCHAR    ucSDiscInBuf[S_DISCRETE_INPUT_NDISCRETES/8]  ; //从机的寄存器组
#endif
//Slave mode:Coils variables
//从模式:线圈变量
USHORT   usSCoilStart                                 = S_COIL_START;   //寄存器起始地址
#if S_COIL_NCOILS%8                     //应该是寄存器的位数，以字节位单位，向上取整得到所需要的字节个数
UCHAR    ucSCoilBuf[S_COIL_NCOILS/8+1]                ;
#else
UCHAR    ucSCoilBuf[S_COIL_NCOILS/8]                  ; //从机的寄存器组
#endif
//Slave mode:Input Register variables
// 从模式:输入寄存器变量
USHORT   usSRegInStart                                = S_REG_INPUT_START;  //寄存器起始地址
USHORT   usSRegInBuf[S_REG_INPUT_NREGS]               ; //从机的寄存器组
//Slave mode:Holding Register variables
//从模式:保存寄存器变量
USHORT   usSRegHoldStart                              = S_REG_HOLDING_START;    //寄存器起始地址
USHORT   usSRegHoldBuf[S_REG_HOLDING_NREGS]           ; //从机的寄存器组

static u8 SlavePollStack[0x800];

static UCHAR ucSlaveAddr = 0;            //从机地址
static enum eMBSlaveTransferMode ucModbusSlaveMode;
static UCHAR ucPortNum = 0;
static ULONG ulPortBaudrate = 115200;
static eMBParity ePortParity = MB_PAR_NONE;
static USHORT usTCP_PortNum = 0;           //0代表使用默认端口号



/**
 * Modbus slave input register callback function.
 *
 * @param pucRegBuffer input register buffer
 * @param usAddress input register address
 * @param usNRegs input register number
 *
 * @return result
*/
// 读输入寄存器
eMBErrorCode eMBRegInputCB(UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          iRegIndex;
    USHORT *        pusRegInputBuf;
    USHORT          REG_INPUT_START;
    USHORT          REG_INPUT_NREGS;
    USHORT          usRegInStart;

    pusRegInputBuf = usSRegInBuf;
    REG_INPUT_START = S_REG_INPUT_START;
    REG_INPUT_NREGS = S_REG_INPUT_NREGS;
    usRegInStart = usSRegInStart;

    /* it already plus one in modbus function method. */
    usAddress--;

    if ((usAddress >= REG_INPUT_START)
            && (usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS))
    {
        iRegIndex = usAddress - usRegInStart;
        while (usNRegs > 0)
        {
            *pucRegBuffer++ = (UCHAR) (pusRegInputBuf[iRegIndex] >> 8);
            *pucRegBuffer++ = (UCHAR) (pusRegInputBuf[iRegIndex] & 0xFF);
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

/**
 * Modbus slave holding register callback function.
 *
 * @param pucRegBuffer holding register buffer
 * @param usAddress holding register address
 * @param usNRegs holding register number
 * @param eMode read or write
 * @return result
 */
eMBErrorCode eMBRegHoldingCB(UCHAR * pucRegBuffer, USHORT usAddress,
        USHORT usNRegs, eMBRegisterMode eMode)
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          iRegIndex;
    USHORT *        pusRegHoldingBuf;
    USHORT          REG_HOLDING_START;
    USHORT          REG_HOLDING_NREGS;
    USHORT          usRegHoldStart;

    pusRegHoldingBuf = usSRegHoldBuf;
    REG_HOLDING_START = S_REG_HOLDING_START;
    REG_HOLDING_NREGS = S_REG_HOLDING_NREGS;
    usRegHoldStart = usSRegHoldStart;

    /* it already plus one in modbus function method. */
    usAddress--;

    if ((usAddress >= REG_HOLDING_START)
            && (usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS))
    {
        iRegIndex = usAddress - usRegHoldStart;
        switch (eMode)
        {
        /* read current register values from the protocol stack. */
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
 * Modbus slave coils callback function.
 *
 * @param pucRegBuffer coils buffer
 * @param usAddress coils address
 * @param usNCoils coils number
 * @param eMode read or write
 *
 * @return result
 */
eMBErrorCode eMBRegCoilsCB(UCHAR * pucRegBuffer, USHORT usAddress,
        USHORT usNCoils, eMBRegisterMode eMode)
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          iRegIndex , iRegBitIndex , iNReg;
    UCHAR *         pucCoilBuf;
    USHORT          COIL_START;
    USHORT          COIL_NCOILS;
    USHORT          usCoilStart;
    iNReg =  usNCoils / 8 + 1;

    pucCoilBuf = ucSCoilBuf;
    COIL_START = S_COIL_START;
    COIL_NCOILS = S_COIL_NCOILS;
    usCoilStart = usSCoilStart;

    /* it already plus one in modbus function method. */
    usAddress--;

    if( ( usAddress >= COIL_START ) &&
        ( usAddress + usNCoils <= COIL_START + COIL_NCOILS ) )
    {
        iRegIndex = (USHORT) (usAddress - usCoilStart) / 8;
        iRegBitIndex = (USHORT) (usAddress - usCoilStart) % 8;
        switch ( eMode )
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
 * Modbus slave discrete callback function.
 *
 * @param pucRegBuffer discrete buffer
 * @param usAddress discrete address
 * @param usNDiscrete discrete number
 *
 * @return result
 */
eMBErrorCode eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          iRegIndex , iRegBitIndex , iNReg;
    UCHAR *         pucDiscreteInputBuf;
    USHORT          DISCRETE_INPUT_START;
    USHORT          DISCRETE_INPUT_NDISCRETES;
    USHORT          usDiscreteInputStart;
    iNReg =  usNDiscrete / 8 + 1;

    pucDiscreteInputBuf = ucSDiscInBuf;
    DISCRETE_INPUT_START = S_DISCRETE_INPUT_START;
    DISCRETE_INPUT_NDISCRETES = S_DISCRETE_INPUT_NDISCRETES;
    usDiscreteInputStart = usSDiscInStart;

    /* it already plus one in modbus function method. */
    usAddress--;

    if ((usAddress >= DISCRETE_INPUT_START)
            && (usAddress + usNDiscrete    <= DISCRETE_INPUT_START + DISCRETE_INPUT_NDISCRETES))
    {
        iRegIndex = (USHORT) (usAddress - usDiscreteInputStart) / 8;
        iRegBitIndex = (USHORT) (usAddress - usDiscreteInputStart) % 8;

        while (iNReg > 0)
        {
            *pucRegBuffer++ = xMBUtilGetBits(&pucDiscreteInputBuf[iRegIndex++],
                    iRegBitIndex, 8);
            iNReg--;
        }
        pucRegBuffer--;
        /* last discrete */
        usNDiscrete = usNDiscrete % 8;
        /* filling zero to high bit */
        *pucRegBuffer = *pucRegBuffer << (8 - usNDiscrete);
        *pucRegBuffer = *pucRegBuffer >> (8 - usNDiscrete);
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

static ptu32_t mb_slave_poll(void)
{
    eMBErrorCode res = MB_ENOERR;
    if (MB_RTU_MODE == ucModbusSlaveMode)
    {
#if MB_SLAVE_RTU_ENABLED == true
        res = eMBInit(MB_RTU, ucSlaveAddr, ucPortNum, ulPortBaudrate, ePortParity);
#else
        error_printf("modbus", "Please open RTU mode first\r\n");
#endif
    }
    else if (MB_ASCII_MODE == ucModbusSlaveMode)
    {
#if MB_SLAVE_ASCII_ENABLED == true
        res = eMBInit(MB_ASCII, ucSlaveAddr, ucPortNum, ulPortBaudrate, ePortParity);
#else
        error_printf("modbus", "Please open RTU mode first\r\n");
#endif
    }
    else if (MB_TCP_MODE == ucModbusSlaveMode)
    {
#if MB_SLAVE_TCP_ENABLED == true
        res = eMBTCPInit(usTCP_PortNum);
#else
        error_printf("modbus", "Please open TCP mode first\r\n");
#endif
    }
    if (MB_ENOERR == res)
    {
        eMBEnable();
        while (1)
        {
            eMBPoll();
        }
    }

    return 0;
}

// =============================================================================
// 函数功能:modbus主机功能初始化，目前主机只支持RTU
// 输入参数:无
// 返回值 : true -- 成功；false -- 失败
// =============================================================================
bool_t ModuleInstall_ModbusSlave(UCHAR ucAddr, enum eMBSlaveTransferMode ucMode,
                    UCHAR ucNum, ULONG ulBaudrate, eMBParity eParity, USHORT usTCP_Port)
{
    u16 EvttID = 0;

    EvttID = DJY_EvttRegist(EN_INDEPENDENCE, 190, 0, 1, (void *)mb_slave_poll, 
                                    SlavePollStack, sizeof(SlavePollStack), "slave poll");
    if (CN_EVTT_ID_INVALID != EvttID)
    {
        ucSlaveAddr = ucAddr;
        ucModbusSlaveMode = ucMode;
        ucPortNum = ucNum;
        ulPortBaudrate = ulBaudrate;
        ePortParity = eParity;
        usTCP_PortNum = usTCP_Port;
        if (CN_EVENT_ID_INVALID != DJY_EventPop(EvttID, NULL, 0, 0, 0, 0))
        {
            return TRUE;
        }
        else
        {
            error_printf("modbus", "slave poll event pop fail\r\n");
            DJY_EvttUnregist(EvttID);
        }
    }
    else
    {
        error_printf("modbus", "slave poll Evtt regist fail\r\n");
    }

    return FALSE;
}


