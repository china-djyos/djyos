//-----------------------------------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
#include <stm32f7xx_hal.h>
#include <stm32f7xx_hal_mmc.h>
#include <systime.h>

extern uint32_t SDMMC_GetCmdResp1(SDMMC_TypeDef *SDMMCx, uint8_t SD_CMD, uint32_t Timeout);
extern HAL_StatusTypeDef SDMMC_ConfigData(SDMMC_TypeDef *SDMMCx, SDMMC_DataInitTypeDef* Data);
// ============================================================================
// ���ܣ�SWITCH����
// ������
// ���أ�
// ��ע��CMD6
// ============================================================================
u32 EMMC_CMD_Switch(MMC_HandleTypeDef *pHmmc, u32 dwArg)
{
    u32 res;
    u32 response;
    u32 count;

    res = SDMMC_CmdSwitch(pHmmc->Instance, dwArg);
    if(HAL_MMC_ERROR_NONE != res)
    {
        return (-1);
    }

    // �ȴ�DAT0�ͷ�

    while(1)
    {
        DJY_EventDelay(10);
        if(count++ == SDMMC_MAX_TRIAL)
        {
            return (-1);
        }

        res = SDMMC_CmdSendStatus(pHmmc->Instance, (u32)(pHmmc->MmcCard.RelCardAdd << 16));
        if(HAL_MMC_ERROR_NONE != res)
        {
            return (-1);
        }

        response = SDMMC_GetResponse(pHmmc->Instance, SDMMC_RESP1);
        if((response & 1 << 8) && ((response & (0xF << 9)) != (0x7 << 9)))
            break;
    }

    return (0);
}
// ============================================================================
// ���ܣ���������λ��
// ������bWidth -- ����λ��bDual -- �ر��߼���
// ���أ�
// ��ע��CMD6
// ============================================================================
u32 EMMC_CMD_SwitchBusWidth(MMC_HandleTypeDef *pHmmc, u8 bWidth, u8 bDual)
{
    u32 res;
    u32 argument = 0;
    u8 index = 183;
    u8 value = 0;

    switch(bWidth)
    {
        case 1:

            value = 0;
            break;

        case 4:

            if(bDual)
                value = 5;
            else
                value = 1;
            break;

        case 8:

            if(bDual)
                value = 6;
            else
                value = 2;
            break;

        default:
            break;
    }

    argument = ((3 << 24) | (index << 16) | (value << 8));
    res = EMMC_CMD_Switch(pHmmc, argument);
    return (res);
}
// ============================================================================
// ���ܣ�������������ģʽ
// ������
// ���أ�
// ��ע��
// ============================================================================
u32 EMMC_CMD_SwitchToHighSpeed(MMC_HandleTypeDef *pHmmc)
{
    u32 res;
    u32 argument = 0;
    u8 index = 185;
    u8 value = 1;

    argument = ((3 << 24) | (index << 16) | (value << 8));

    res = EMMC_CMD_Switch(pHmmc, argument);
    return (res);
}

// ============================================================================
// ���ܣ���ȡECSD
// ������
// ���أ�-1 -- ʧ�ܣ�
// ��ע��
// ============================================================================
u32 EMMC_CMD_SendECSD(MMC_HandleTypeDef *pHmmc, u8 *pECSD, u32 dwTimeout)
{
    SDMMC_CmdInitTypeDef cmdInit;
    SDMMC_DataInitTypeDef config;
    u32 res;
    u32 *buf = (u32*)pECSD;
    u32 count = 0;
    u32 bytes = 0;
    s64 tickStart = DJY_GetSysTime();

    if((!pHmmc) || (!pECSD))
        return (-1);
#if 0
    res = SDMMC_CmdBlockLength(pHmmc->Instance, BLOCKSIZE); // Set Block Size for Card
    if(res != HAL_MMC_ERROR_NONE)
    {
        return (-1);
    }
#endif

    pHmmc->Instance->DCTRL = 0; // TODO
    config.DataTimeOut   = SDMMC_DATATIMEOUT;
    config.DataLength    = 512;
    config.DataBlockSize = SDMMC_DATABLOCK_SIZE_512B;
    config.TransferDir   = SDMMC_TRANSFER_DIR_TO_SDMMC;
    config.TransferMode  = SDMMC_TRANSFER_MODE_BLOCK;
    config.DPSM          = SDMMC_DPSM_ENABLE;
    SDMMC_ConfigData(pHmmc->Instance, &config);

    // Send CMD SEND_EXT_CSD
    cmdInit.Argument         = 0;
    cmdInit.CmdIndex         = SDMMC_CMD_HS_SEND_EXT_CSD;
    cmdInit.Response         = SDMMC_RESPONSE_SHORT;
    cmdInit.WaitForInterrupt = SDMMC_WAIT_NO;
    cmdInit.CPSM             = SDMMC_CPSM_ENABLE;
    SDMMC_SendCommand(pHmmc->Instance, &cmdInit);

    // Poll on SDMMC flags
    while(1)
    {
        if(__HAL_MMC_GET_FLAG(pHmmc, SDMMC_FLAG_RXOVERR | SDMMC_FLAG_DCRCFAIL))// | SDMMC_FLAG_DTIMEOUT))
        {
            // u32 watch = pHmmc->Instance->STA; // ���Ե�
            return (1);
        }

        if(__HAL_MMC_GET_FLAG(pHmmc, SDMMC_FLAG_RXFIFOHF)) // FIFO����������8*4�ֽ�
        {
            for(count = 0; count < 8; count++)
            {
                *(buf + count) = SDMMC_ReadFIFO(pHmmc->Instance);
            }
            buf += 8;
            bytes += 32;
        }
        else if(__HAL_MMC_GET_FLAG(pHmmc, SDMMC_FLAG_RXDAVL))
        {
            *(buf) = SDMMC_ReadFIFO(pHmmc->Instance);
            buf += 1;
            bytes +=4;
        }
        else if(__HAL_MMC_GET_FLAG(pHmmc, SDMMC_FLAG_DATAEND))
        {
            break; // �����Ѷ���
        }

        if((-1 != dwTimeout) && ((u32)(DJY_GetSysTime() - tickStart) >=  dwTimeout))
        {
            __HAL_MMC_CLEAR_FLAG(pHmmc, SDMMC_STATIC_FLAGS);
            pHmmc->ErrorCode |= HAL_MMC_ERROR_TIMEOUT;
            pHmmc->State= HAL_MMC_STATE_READY;
            return (-1);
        }

    }

    // Check for error conditions
    res = SDMMC_GetCmdResp1(pHmmc->Instance, SDMMC_CMD_HS_SEND_EXT_CSD, SDMMC_CMDTIMEOUT);
    if(res != HAL_MMC_ERROR_NONE)
    {
        return (-1);
    }

    return (bytes);
}

// ============================================================================
// ���ܣ�����������ַ
// ������
// ���أ�
// ��ע��
// ============================================================================
u32 EMMC_CMD_SetRelAddr(SDMMC_TypeDef *SDMMCx, u16 wRCA)
{
    SDMMC_CmdInitTypeDef  cmdInit;
    u32 res = SDMMC_ERROR_NONE;

    /* Send CMD3 SD_CMD_SET_REL_ADDR */
    cmdInit.Argument         = (u32)(wRCA<<16);
    cmdInit.CmdIndex         = SDMMC_CMD_SET_REL_ADDR;
    cmdInit.Response         = SDMMC_RESPONSE_SHORT;
    cmdInit.WaitForInterrupt = SDMMC_WAIT_NO;
    cmdInit.CPSM             = SDMMC_CPSM_ENABLE;
    SDMMC_SendCommand(SDMMCx, &cmdInit);

    /* Check for error conditions */
    res = SDMMC_GetCmdResp1(SDMMCx, SDMMC_CMD_SET_REL_ADDR, SDMMC_CMDTIMEOUT);

    return res;
}

// ============================================================================
// ���ܣ�����������sleep״̬
// ������
// ���أ�
// ��ע��
// ============================================================================
u32 EMMC_CMD_Sleep(SDMMC_TypeDef *SDMMCx, u32 dwRCA)
{
    SDMMC_CmdInitTypeDef  cmdInit;
    u32 res = SDMMC_ERROR_NONE;

    /* Send CMD3 SD_CMD_SET_REL_ADDR */
    cmdInit.Argument         = (u32)(((dwRCA << 16) & 0xFFFF0000) | 0x8000);
    cmdInit.CmdIndex         = SDMMC_CMD_SDMMC_SEN_OP_COND;
    cmdInit.Response         = SDMMC_RESPONSE_SHORT;
    cmdInit.WaitForInterrupt = SDMMC_WAIT_NO;
    cmdInit.CPSM             = SDMMC_CPSM_ENABLE;
    SDMMC_SendCommand(SDMMCx, &cmdInit);

    /* Check for error conditions */
    res = SDMMC_GetCmdResp1(SDMMCx, SDMMC_CMD_SDMMC_SEN_OP_COND, SDMMC_CMDTIMEOUT);

    return res;
}

// ============================================================================
// ���ܣ��������˳�sleep״̬
// ������
// ���أ�
// ��ע��
// ============================================================================
u32 EMMC_CMD_Awake(SDMMC_TypeDef *SDMMCx, u32 dwRCA)
{
    SDMMC_CmdInitTypeDef  cmdInit;
    u32 res = SDMMC_ERROR_NONE;

    /* Send CMD3 SD_CMD_SET_REL_ADDR */
    cmdInit.Argument         = (u32)((dwRCA << 16) & 0xFFFF0000);
    cmdInit.CmdIndex         = SDMMC_CMD_SDMMC_SEN_OP_COND;
    cmdInit.Response         = SDMMC_RESPONSE_SHORT;
    cmdInit.WaitForInterrupt = SDMMC_WAIT_NO;
    cmdInit.CPSM             = SDMMC_CPSM_ENABLE;
    SDMMC_SendCommand(SDMMCx, &cmdInit);

    /* Check for error conditions */
    res = SDMMC_GetCmdResp1(SDMMCx, SDMMC_CMD_SDMMC_SEN_OP_COND, SDMMC_CMDTIMEOUT);

    return res;
}
