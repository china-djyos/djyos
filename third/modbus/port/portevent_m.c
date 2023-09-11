/*
 * FreeModbus Libary: RT-Thread Port
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
 * File: $Id: portevent_m.c v 1.60 2013/08/13 15:07:05 Armink add Master Functions$
 */

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mb_m.h"
#include "mbport.h"
#include "port.h"

#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0
/* ----------------------- Defines ------------------------------------------*/
/* ----------------------- Variables ----------------------------------------*/
static struct SemaphoreLCB xMasterRunRes;
static struct SemaphoreLCB xMasterEventLock;
static struct MutexLCB xMasterSetEventLock;
static struct SemaphoreLCB xMasterEventExeRetLock;
static eMBMasterEventType  xMasterOsEvent;
static eMBMasterRequestResType  xMasterOsEventExeRet;
/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBMasterPortEventInit( void )
{
    if (Lock_SempCreate_s(&xMasterEventLock, 1, 0, CN_BLOCK_PRIO, "master event"))
    {
        if (Lock_SempCreate_s(&xMasterEventExeRetLock, 1, 0, CN_BLOCK_PRIO, "master event execute ret"))
        {
            if (Lock_MutexCreate_s(&xMasterSetEventLock, "master set event"))
            {
                return TRUE;
            }
            else
            {
                error_printf("modbus", "xMasterSetEventLock mutex create fail\r\n");
            }
            Lock_SempDelete_s(&xMasterEventExeRetLock);
        }
        else
        {
            error_printf("modbus", "xMasterEventExeRetLock semp create fail\r\n");
        }

        Lock_SempDelete_s(&xMasterEventLock);
    }
    else
    {
        error_printf("modbus", "xMasterEventLock semp create fail\r\n");
    }
    return FALSE;
}

BOOL
xMBMasterPortEventPost( eMBMasterEventType eEvent )
{
    //���������Ŀ���Ƿ�ֹ���ú�Ҫ���������ͻȻ��Ӧ�˳�ʱ�жϣ���Ȼ���xMasterOsEvent״̬���ó���������
    //�����������ݵ�״̬�Ͷ��ˣ�������֮�������˷���״̬��post�ź�������һ������ȥ�������¼���
    // �����������1��������������ȼ��ȷ����¼�����ͳ�ʱ��������ȼ����ߣ���ô�����������úú󣬻�ȥ�ȴ���������
    // ��ʱ����Ϊ�¼������ȳ�ʱ��������ȼ��ߣ������л���ȥ�����ͣ���ȥ����ʱ��
    // 2��������������ȼ��ȷ����¼�����ͳ�ʱ��������ȼ����ͣ���ô�����������úú󣬾�һ�����ȴ������¼�����Ϊ��ʱ�¼�
    // �ᱻ����������ס��
    Lock_MutexPend(&xMasterSetEventLock, CN_TIMEOUT_FOREVER);
    xMasterOsEvent = eEvent;
    Lock_SempPost(&xMasterEventLock);
    Lock_MutexPost(&xMasterSetEventLock);
    return TRUE;
}

BOOL
xMBMasterPortEventGet( eMBMasterEventType * eEvent )
{
    Lock_SempPend(&xMasterEventLock, CN_TIMEOUT_FOREVER);
    *eEvent = xMasterOsEvent;
    return TRUE;
}
/**
 * This function is initialize the OS resource for modbus master.
 * Note:The resource is define by OS.If you not use OS this function can be empty.
 *
 */
void vMBMasterOsResInit( void )
{
    if (NULL == Lock_SempCreate_s(&xMasterRunRes, 1, 1, CN_BLOCK_PRIO, "master res"))
    {
        error_printf("modbus", "semp create fail\r\n");
    }
}

/**
 * This function is take Mobus Master running resource.
 * ��ȡMobus Master������Դ��
 * Note:The resource is define by Operating System.If you not use OS this function can be just return TRUE.
 *
 * @param lTimeOut the waiting time.
 *
 * @return resource taked result
 */
BOOL xMBMasterRunResTake( LONG lTimeOut )
{
    /*If waiting time is -1 .It will wait forever */
    // ��ȡ�ź���
    return Lock_SempPend(&xMasterRunRes, lTimeOut);
}

/**
 * This function is release Mobus Master running resource.
 * �ͷ�Mobus Master������Դ��
 * Note:The resource is define by Operating System.If you not use OS this function can be empty.
 *
 */
void vMBMasterRunResRelease( void )
{
    /* release resource */
    // �ͷ��ź���
    Lock_SempPost(&xMasterRunRes);
}

/**
 * This is modbus master respond timeout error process callback function.
 * @note There functions will block modbus master poll while execute OS waiting.
 * So,for real-time of system.Do not execute too much waiting process.
 *����modbus����Ӧ��ʱ������ص�������
��Щ��������ִ��OS�ȴ�ʱ����modbus����ѯ��
*��ˣ�����ϵͳ��ʵʱ�ԡ���Ҫִ�й���ĵȴ����̡�
 * @param ucDestAddress destination salve address
 * @param pucPDUData PDU buffer data
 * @param ucPDULength PDU buffer length
 *
 */
void vMBMasterErrorCBRespondTimeout(UCHAR ucDestAddress, const UCHAR* pucPDUData,
        USHORT ucPDULength) {
    /**
     * @note This code is use OS's event mechanism for modbus master protocol stack.
     * If you don't use OS, you can change it.
     */

    /* You can add your code under here. */

    UNUSED(ucDestAddress);
    UNUSED(pucPDUData);
    UNUSED(ucPDULength);

    xMasterOsEventExeRet = EV_MASTER_ERROR_RESPOND_TIMEOUT;
    Lock_SempPost(&xMasterEventExeRetLock);
}

/**
 * This is modbus master receive data error process callback function.
 * @note There functions will block modbus master poll while execute OS waiting.
 * So,for real-time of system.Do not execute too much waiting process.
 *
 * @param ucDestAddress destination salve address
 * @param pucPDUData PDU buffer data
 * @param ucPDULength PDU buffer length
 *
 */
void vMBMasterErrorCBReceiveData(UCHAR ucDestAddress, const UCHAR* pucPDUData,
        USHORT ucPDULength) {
    /**
     * @note This code is use OS's event mechanism for modbus master protocol stack.
     * If you don't use OS, you can change it.
     */

    /* You can add your code under here. */

    UNUSED(ucDestAddress);
    UNUSED(pucPDUData);
    UNUSED(ucPDULength);

    xMasterOsEventExeRet = EV_MASTER_ERROR_RECEIVE_DATA;
    Lock_SempPost(&xMasterEventExeRetLock);
}

/**
 * This is modbus master execute function error process callback function.
 * @note There functions will block modbus master poll while execute OS waiting.
 * So,for real-time of system.Do not execute too much waiting process.
 *
 * @param ucDestAddress destination salve address
 * @param pucPDUData PDU buffer data
 * @param ucPDULength PDU buffer length
 *
 */
void vMBMasterErrorCBExecuteFunction(UCHAR ucDestAddress, const UCHAR* pucPDUData,
        USHORT ucPDULength) {
    /**
     * @note This code is use OS's event mechanism for modbus master protocol stack.
     * If you don't use OS, you can change it.
     */

    /* You can add your code under here. */
    UNUSED(ucDestAddress);
    UNUSED(pucPDUData);
    UNUSED(ucPDULength);

    xMasterOsEventExeRet = EV_MASTER_ERROR_EXECUTE_FUNCTION;
    Lock_SempPost(&xMasterEventExeRetLock);
}

/**
 * This is modbus master request process success callback function.
 * @note There functions will block modbus master poll while execute OS waiting.
 * So,for real-time of system.Do not execute too much waiting process.
 *����modbus��������̳ɹ��ص�������
��Щ��������ִ��OS�ȴ�ʱ����modbus����ѯ��
*��ˣ�����ϵͳ��ʵʱ�ԡ���Ҫִ�й���ĵȴ����̡�
 */
void vMBMasterCBRequestScuuess( void ) {
    /**
     * @note This code is use OS's event mechanism for modbus master protocol stack.
     * If you don't use OS, you can change it.
     */
    // ���õ�ǰ���¼��Ǵ���ɹ�

    /* You can add your code under here. */
    xMasterOsEventExeRet = EV_MASTER_PROCESS_SUCESS;
    Lock_SempPost(&xMasterEventExeRetLock);
}

/**
 * This function is wait for modbus master request finish and return result.
 * Waiting result include request process success, request respond timeout, receive data error and execute function error.
 * You can use the above callback function.
 * @note If you are use OS, you can use OS's event mechanism. Otherwise you have to run much user custom delay for waiting.
 *
 * @return request error code
 * ��������ǵȴ�modbus��������ɲ����ؽ����
*�ȴ��������������ɹ���������Ӧ��ʱ���������ݴ����ִ�к�������
*������ʹ�������ص�������
�����ʹ��OS�������ʹ��OS���¼����ơ��������������д����û��Զ����ӳٵȴ���
@��������������
 */
eMBMasterReqErrCode eMBMasterWaitRequestFinish( void ) {
    eMBMasterReqErrCode    eErrStatus = MB_MRE_NO_ERR;
    Lock_SempPend(&xMasterEventExeRetLock, CN_TIMEOUT_FOREVER);

    switch (xMasterOsEventExeRet)
    {
        case EV_MASTER_PROCESS_SUCESS:      //֡��������
            break;
        case EV_MASTER_ERROR_RESPOND_TIMEOUT:
        {   //�ӻ�Ӧ��ʱ
            eErrStatus = MB_MRE_TIMEDOUT;
            break;
        }
        case EV_MASTER_ERROR_RECEIVE_DATA:
        {   //����֡���ݴ���
            eErrStatus = MB_MRE_REV_DATA;
            break;
        }
        case EV_MASTER_ERROR_EXECUTE_FUNCTION:
        {   //ִ�к�������
            eErrStatus = MB_MRE_EXE_FUN;
            break;
        }
        default:
            break;
    }
    return eErrStatus;
}

#endif
