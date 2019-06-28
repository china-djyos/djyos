// =============================================================================
// Copyright (C) 2012-2020 �����߲���ϵͳ�з��Ŷ�  All Rights Reserved
// �ļ���     ��spibus.c
// ģ������: djybusģ���һ���ӽ��SPI���ߣ����ļ���Ҫ����SPI���͵����ӣ����ߺ�����
//           �����Լ���SPI���ߵĲ���������Ӧ��SPI�Ĳ���API������
// ģ��汾: V1.00
// ������Ա: hm
// ����ʱ��: 02/07.2014
// =============================================================================

#include "stdint.h"
#include "stdio.h"
#include "object.h"
#include "lock.h"
#include "stdlib.h"
#include <device.h>
#include "string.h"
#include "djybus.h"
#include "spibus.h"
#include "systime.h"
#include "djyos.h"
#include "dbug.h"

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_SPIBus(void);
//    ModuleInstall_SPIBus();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"spi bus"//spibus
//parent:"djybus"    //��д������ĸ�������֣�none��ʾû�и����
//attribute:system              //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"djybus"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_SPI_BUS == false )
//#warning  " spi_bus  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_SPI_BUS    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

static struct Object *s_ptSPIBusType;

#define CN_SPI_FLAG_POLL        (1<<0)          //��ѯ�жϷ�ʽ���

//SPI���������ṹ��
struct SPI_Device
{
    struct Object *HostObj;
    u8 Cs;                                  //Ƭѡ�ź�
    bool_t AutoCs;                          //�Զ�Ƭѡ
    u8 CharLen;                             //���ݳ���
    u8 Mode;                                //ģʽѡ��
    u8 ShiftDir;                            //MSB or LSB
    u32 Freq;                               //�ٶ�,Hz
};


struct SPI_Buf
{
    u32    Offset;         //������ָ��,ָ����һ�ζ���λ��
    u32    MaxLen;        //��������󳤶�,Ԫ�ظ���.
    u8     *pBuf;             //������ָ��,�û��Լ���֤�����ٵĻ������Ƿ����趨
};
//SPI���߿��ƿ�ṹ��,��ģ��ɼ�
struct SPI_CB
{
    struct Object           *HostObj;              //��������
    struct SPI_Buf          SPI_Buf;               //������,�����첽����
    struct SemaphoreLCB     *SPI_BusSemp;           //SPI���߱����ź���
    struct SemaphoreLCB     *SPI_BlockSemp;         //���׻����������ź���
    struct SPI_DataFrame    Frame;
    struct SPI_Device       *CurrentDev;           //ռ�е�ǰ���ߵ��豸
    u16                     ErrorPopEvtt;       //�������¼�������
    bool_t                  MultiCsReg;         //�Ƿ���ж���CS�Ĵ���
    u8                      BlockOption;        //��ǰ���߲����Ƿ�����
    u8                      Flag;               //��ѯ�жϱ��
    ptu32_t                 SpecificFlag;       //���Ա��
    TransferFunc            pTransferTxRx;
    TransferPoll            pTransferPoll;
    CsActiveFunc            pCsActive;
    CsInActiveFunc          pCsInActive;
    SPIBusCtrlFunc          pBusCtrl;
};

// =============================================================================
// ���ܣ���SPI�������ͽ��ҽӵ�DjyBus�����
// ��������
// ���أ�true = �ɹ���false = ʧ��
// =============================================================================
bool_t ModuleInstall_SPIBus(void)
{
    s_ptSPIBusType = DjyBus_BusTypeAdd("SPIBus");
    if(NULL != s_ptSPIBusType)
    {
        info_printf("bus","spi bus install successful.");
        return true;
    }
    else
    {
        error_printf("bus","spi bus install failed.");
        return false;
    }
}

// =============================================================================
// ���ܣ�����SPI���߽�㵽SPI�������ͽ��,����ʼ��tagSPI_CB���ƿ�ṹ��
// ������NewSPIParam,�������߲���������˵����ϸ�����tagSPI_Param�ṹ��
// ���أ����ؽ�������Դ���ָ�룬ʧ��ʱ����NULL
// =============================================================================
struct SPI_CB *SPI_BusAdd(struct SPI_Param *NewSPIParam)
{
    struct Object *SpiDev;
    struct SPI_CB *NewSPI;
    if(NULL == NewSPIParam)
        goto exit_from_param;

    //�����ظ�����ͬ����SPI����
    if(NULL != obj_search_child(s_ptSPIBusType,(const char*)(NewSPIParam->BusName)))
        goto exit_from_readd;

    NewSPI = (struct SPI_CB *)M_Malloc(sizeof(struct SPI_CB),0);
    if(NewSPI == NULL)
        goto exit_from_malloc;

    //�����߽��ҽӵ��������ͽ����ӽ��
    SpiDev = obj_newchild(s_ptSPIBusType, (fnObjOps)-1,(ptu32_t)NewSPI,(const char*)(NewSPIParam->BusName));
    if(SpiDev == NULL)
        goto exit_from_add_node;

    //���������ź����������ź���
    NewSPI->SPI_BusSemp= Lock_SempCreate(1,1,CN_BLOCK_FIFO,"spi bus semp");
    if(NewSPI->SPI_BusSemp== NULL)
        goto exit_from_spi_bus_semp;
    NewSPI->SPI_BlockSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"spi block semp");
    if(NewSPI->SPI_BlockSemp== NULL)
        goto exit_from_spi_buf_semp;

    //tagSPI_CB���ƿ��ʼ��
    NewSPI->HostObj           = SpiDev;
    NewSPI->ErrorPopEvtt      = CN_EVTT_ID_INVALID;
    NewSPI->SpecificFlag      = NewSPIParam->SpecificFlag;
    NewSPI->MultiCsReg        = NewSPIParam->MultiCSRegFlag;
    NewSPI->pTransferTxRx     = NewSPIParam->pTransferTxRx;
    NewSPI->pCsActive         = NewSPIParam->pCsActive;
    NewSPI->pCsInActive       = NewSPIParam->pCsInActive;
    NewSPI->pBusCtrl          = NewSPIParam->pBusCtrl;
    NewSPI->pTransferPoll     = NewSPIParam->pTransferPoll;

    //��������ʼ��
    NewSPI->SPI_Buf.pBuf     = NewSPIParam->SPIBuf;
    NewSPI->SPI_Buf.MaxLen   = NewSPIParam->SPIBufLen;
    NewSPI->SPI_Buf.Offset   = 0;

    info_printf("spibus","%s added.", NewSPIParam->BusName);
    return NewSPI;

exit_from_spi_buf_semp:
    Lock_SempDelete(NewSPI->SPI_BusSemp);
exit_from_spi_bus_semp:
    obj_Delete(SpiDev);
exit_from_add_node:
    free(NewSPI);
exit_from_malloc:
exit_from_readd:
exit_from_param:
    error_printf("spibus","%s add failed.", NewSPIParam->BusName);
    return NULL;
}

// =============================================================================
// ���ܣ�ɾ����SPI���������ϵ�SPI��㣬����ɾ������������������㣬��ɾ���᲻�ɹ�,��
//       �ͷ��������ʱmalloc�Ŀ��ƿ��ڴ�
// ������DelSPI,ɾ��SPI���ƿ�ָ��
// ���أ�true,ɾ���ɹ�;false,ɾ��ʧ��
// =============================================================================
bool_t SPI_BusDelete(struct SPI_CB *DelSPI)
{
    bool_t result;
    if(NULL == DelSPI)
        return false;
    if(obj_Delete(DelSPI->HostObj))
    {
        result = false;
    }
    else
    {
        result = true;
        free(DelSPI);
    }
    return result;
}


// =============================================================================
// ���ܣ�����SPI�������ͽ����ӽ�����Ƿ�汻�������Ƶ�����
// ������BusName,���ҵ���������
// ���أ����ҵĿ��ƿ���ָ�룬δ�ҵ�ʱ����NULL
// =============================================================================
struct SPI_CB *SPI_BusFind(const char *BusName)
{
    struct Object *SPI_BusObj;
    SPI_BusObj = obj_search_child(s_ptSPIBusType,BusName);
    if(SPI_BusObj)
    {
        return (struct SPI_CB *)obj_GetPrivate(SPI_BusObj);
    }
    else
        return NULL;
}

// =============================================================================
// ���ܣ���SPI���߽���������������������Ϲҽӵ�����������ʼ����������Ӧ�Ĳ������ü�
//       ����,�������������߽����ӽ��
// ������BusName,�����������ҽӵ���������
//       DevName,��������
//       cs,������������Ƭѡ���
//       charlen,�������ַ����ȣ�����λ��
//       mode,���ô�����ģʽ,CHPA��CPOL��ϳɳɵ�ģʽ
//       freq,SPI���ߵ�ʱ��Ƶ�ʣ���λHz
//       autocs,�������豸�Ƿ��Զ�Ƭѡ
// ���أ����ƿ���ָ�룬���ʧ��ʱ����NULL
// =============================================================================
struct SPI_Device *SPI_DevAdd(const char *BusName ,const char *DevName,u8 cs,u8 charlen,
                                u8 mode,u8 shiftdir,u32 freq,u8 autocs)
{
    struct SPI_CB     *SPI;
    struct SPI_Device *NewDev;
    struct Object *NewIicObj;
    tagSpiConfig spicfg;

    //��ѯ�Ƿ�����ߴ���
    SPI = SPI_BusFind(BusName);
    if(NULL == SPI)
        return NULL;

    //���⽨��ͬ����SPI����
    if(NULL != obj_search_child(s_ptSPIBusType, DevName))
        return NULL;

    //Ϊ�µ�������㶯̬�����ڴ�
    NewDev = (struct SPI_Device *)M_Malloc(sizeof(struct SPI_Device),0);
    if(NULL == NewDev)
        return NULL;

    //Ϊ�½���ʼ��
    NewDev->Cs  = cs;
    NewDev->HostObj = obj_newchild(SPI->HostObj, (fnObjOps)-1, (ptu32_t)NewDev,  DevName);
    if(!NewDev->HostObj)
    {
        free(NewDev);
        return NULL;
    }

    NewDev->CharLen    = charlen;
    NewDev->Cs         = cs;
    NewDev->AutoCs     = autocs;
    NewDev->Mode       = mode;
    NewDev->Freq       = freq;
    NewDev->ShiftDir   = shiftdir;

    spicfg.CharLen = charlen;
    spicfg.Mode    = mode;
    spicfg.Freq    = freq;
    spicfg.ShiftDir= shiftdir;

    //���SPI�������ж���CS�Ĵ�������������������������
    if(true == SPI->MultiCsReg)
    {
        SPI_BusCtrl(NewDev,CN_SPI_CS_CONFIG,(ptu32_t)&spicfg,(ptu32_t)cs);
    }

    info_printf("spibus","device \"%s\" added to \"%s\".", DevName, BusName);
    return NewDev;
}

// =============================================================================
// ���ܣ�ɾ��SPI�����ϵ��������������ӽ����ɾ��
// ������DelDev,ɾ��������ָ��
// ���أ�true,ɾ���ɹ�;false,ɾ��ʧ��
// =============================================================================
bool_t SPI_DevDelete(struct SPI_Device *DelDev)
{
    bool_t result;
    if(NULL == DelDev)
        return false;

    if(obj_Delete(DelDev->HostObj))
    {
        result = false;
    }
    else
    {
        free(DelDev);
        result = true;
    }
    return result;
}

// =============================================================================
// ���ܣ�ɾ��SPI�����ϵ��������������ӽ����ɾ��
// ������DelDev,ɾ��������ָ��
// ���أ�true,ɾ���ɹ�;false,ɾ��ʧ��
// =============================================================================
bool_t SPI_DevDelete_s(struct SPI_Device *DelDev)
{
    bool_t result;
    if(NULL == DelDev)
        return false;

    if(obj_Delete(DelDev->HostObj))
    {
        result = false;
    }
    else
    {
        result = true;
    }
    return result;
}

// =============================================================================
// ���ܣ�����������㣬�ý���Ȼ�ǹҽ�����Ӧ��SPI���߽����
// ������BusName,��������SPI��������
//       DevName,��������
// ���أ���㸽���ṹ��ָ��
// =============================================================================
struct SPI_Device *SPI_DevFind(const char *BusName ,const char *DevName)
{
    struct Object *SPI_DevObj;
    struct SPI_CB *SPI_Bus;

    SPI_Bus = SPI_BusFind(BusName);
    if(NULL == SPI_Bus)
        return NULL;

    //ͨ��SPI���ͽ�㣬��������������
    SPI_DevObj = obj_search_child(SPI_Bus->HostObj, DevName);
    if(SPI_DevObj)
        return ((struct SPI_Device*)obj_GetPrivate(SPI_DevObj));
    else
        return NULL;
}

// =============================================================================
// ���ܣ�Ƭѡ���ͣ��ú������б����������ź�������SPI_BusSemp��Ȼ�����CS�Ƿ����
//       �Լ������üĴ�����־���ж��Ƿ���Ҫ���üĴ�������csctrl��־Ϊfalse,��ÿ��CS
//       ʹ��ʱ������Ҫ���üĴ���
// ������Dev,����ָ��
//       timeout,���������ź�����ʱʱ�䣬us
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
bool_t SPI_CsActive(struct SPI_Device *Dev,u32 timeout)
{
    struct SPI_CB *SPI;
    tagSpiConfig spicfg;
    if(NULL == Dev)
        return false;

    SPI = (struct SPI_CB *)obj_GetPrivate(obj_parent(Dev->HostObj));
    if(NULL == SPI)
        return false;

    if(Dev->AutoCs == false)                                //�ֶ����ò���Ч
    {
        if(false == Lock_SempPend(SPI->SPI_BusSemp,timeout))    //��Ҫ�ȴ����߿���
            return false;

        //���û�ж�����CS�������üĴ�������ÿ�β�������Ҫ����SPI�������üĴ�����
        //��Ϊ���CS����ͬһ�ײ������üĴ���
        if((SPI->MultiCsReg == false) && (SPI->CurrentDev != Dev))
        {
            spicfg.CharLen = Dev->CharLen;
            spicfg.Mode    = Dev->Mode;
            spicfg.Freq    = Dev->Freq;
            spicfg.ShiftDir= Dev->ShiftDir;
            SPI->pBusCtrl(SPI->SpecificFlag,CN_SPI_CS_CONFIG,
                            (ptu32_t)&spicfg,(ptu32_t)Dev->Cs);
        }
        //���ÿ��CS���ж��������üĴ�����������ÿ�ε��ö����ã��û���ͨ������pBusCtrl
        //����CS��صļĴ�������
        SPI->CurrentDev = Dev;
        SPI->pCsActive(SPI->SpecificFlag,Dev->Cs);
    }
    return true;
}

bool_t __SPI_AutoCsActive(struct SPI_CB *SPI,struct SPI_Device *Dev)
{
    tagSpiConfig spicfg;

    //���û�ж�����CS�������üĴ�������ÿ�β�������Ҫ����SPI�������üĴ�����
    //��Ϊ���CS����ͬһ�ײ������üĴ���
    if((SPI->MultiCsReg == false) && (SPI->CurrentDev != Dev))
    {
        spicfg.CharLen = Dev->CharLen;
        spicfg.Mode    = Dev->Mode;
        spicfg.Freq    = Dev->Freq;
        spicfg.ShiftDir= Dev->ShiftDir;
        SPI->pBusCtrl(SPI->SpecificFlag,CN_SPI_CS_CONFIG,
                        (ptu32_t)&spicfg,(ptu32_t)Dev->Cs);
    }
    //���ÿ��CS���ж��������üĴ�����������ÿ�ε��ö����ã��û���ͨ������pBusCtrl
    //����CS��صļĴ�������
    SPI->CurrentDev = Dev;
    SPI->pCsActive(SPI->SpecificFlag,Dev->Cs);
    return true;
}

// =============================================================================
// ���ܣ�Ƭѡ���ߣ�������Ϊ������ʽ�����ɸú����ͷ������ź���������Ƭѡ������������
//       �ߺ��ͷ��ź����ɵײ�������ɡ���Ϊ��������ʽʱ�����е��ú���ʱ������δ�����
// ������Dev,����ָ��
//       block_option,����ѡ�Ϊtrueʱ���������һ�δ���Ϊ������ʽ������Ϊ������
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
bool_t SPI_CsInactive(struct SPI_Device *Dev)
{
    struct SPI_CB *SPI;
    if(NULL == Dev)
        return false;

    SPI = (struct SPI_CB *)obj_GetPrivate(obj_parent(Dev->HostObj));
    if(NULL == SPI)
        return false;

    if(Dev->AutoCs == false)                        //�Զ�Ƭѡʱ���ֶ�������Ч
    {
        SPI->pCsInActive(SPI->SpecificFlag,Dev->Cs);
        Lock_SempPost(SPI->SPI_BusSemp);
    }
    else
    {
        ;
    }

    return true;
}

bool_t __SPI_AutoCsInactive(struct SPI_CB *SPI,u8 CS)
{
    SPI->pCsInActive(SPI->SpecificFlag,CS);
    Lock_SempPost(SPI->SPI_BusSemp);
    return true;
}

// =============================================================================
// ���ܣ����ݴ��ͺ�����������ݵķ��ͺͽ��ա��ú�����ɵĹ������£�
//       1.����������Ϊ�˱���������鷳�����ȷ������ٷ������ݣ��ֶ�ε��ã���ε���ǰ
//         ��CSActive��CsInactive����������
//       2.����Dev��������SPI���ߣ�
//       3.�����������ڷ����ֽ�������ֱ�ӽ��������뻺������
//       4.��Ϊ�������ͣ���ȴ������ź�������Ϊ����������ȴ�buf�ź�����
//       5.������ʱ�����ʱ������CS���ͷ��ź���
// ������Dev,����ָ��
//       spidata,SPI���ݽṹ��
//       block_option,����ѡ�Ϊtrueʱ���������һ�δ���Ϊ������ʽ������Ϊ������
//       timeout,��ʱ������us
// ���أ����ط���״̬����ʱ�������޴���
// =============================================================================
s32 SPI_Transfer(struct SPI_Device *Dev,struct SPI_DataFrame *spidata,
                u8 block_option,u32 timeout)
{
    struct SPI_CB *SPI;
//  struct semaphore_LCB *spi_semp;
    s32 result ;
    u32 written=0;
    u32 base_time = 0,rel_timeout = timeout;

    if(NULL == Dev)
        return CN_SPI_EXIT_PARAM_ERR;

    SPI = (struct SPI_CB *)obj_GetPrivate(obj_parent(Dev->HostObj));//���Ҹ�����������������
    if(NULL == SPI)
        return CN_SPI_EXIT_PARAM_ERR;

    base_time = (u32)DjyGetSysTime();
    //���������Զ�Ƭѡ���򱾺����ڲ�������Ƭѡ
    if(Dev->AutoCs == true)
    {
        if(false == Lock_SempPend(SPI->SPI_BusSemp,timeout))    //��Ҫ�ȴ����߿���
        {
            result = CN_SPI_EXIT_TIMEOUT;
            goto exit_from_bus_timeout;
        }
        __SPI_AutoCsActive(SPI,Dev);
    }

    Lock_SempPend(SPI->SPI_BlockSemp,0);                        //�൱�������ź���

    //��ֹ���Ի�δ�Ǽ�pTransferTxRx��ʹ����ѯ��ʽͨ��
    if((Djy_QuerySch() == false) || (SPI->pTransferTxRx == NULL)
            || (SPI->Flag & CN_SPI_FLAG_POLL))
    {
        if(SPI->pTransferPoll != NULL)
        {
            SPI->pTransferPoll(SPI->SpecificFlag,spidata->SendBuf,spidata->SendLen,
                spidata->RecvBuf,spidata->RecvLen,spidata->RecvOff,Dev->Cs);

            if(Dev->AutoCs == true)
                __SPI_AutoCsInactive(SPI,Dev->Cs);
            result = CN_SPI_EXIT_NOERR;
        }
        goto exit_from_no_err;
    }

    //�������жϷ�ʽ�շ�
    if(spidata->RecvLen)
        block_option = true;                            //���������Զ�תΪ����
    SPI->SPI_Buf.Offset = 0;                            //����ǰ����ջ�����
    SPI->Frame = *spidata;
    SPI->BlockOption = block_option;

    //�����������ʽ���һ�����������ֱ��д�뻺����
    //�����ǣ����ȷ��͵������ṩ�Ļ�������ֱ��ʣ���ֽ����ܹ���䵽������
    if((!block_option) && (spidata->SendLen <= SPI->SPI_Buf.MaxLen))
    {
        memcpy(SPI->SPI_Buf.pBuf,spidata->SendBuf,spidata->SendLen);
        written = spidata->SendLen;
    }

    //��������ʱ��Ļص�����
    if(true == SPI->pTransferTxRx(SPI->SpecificFlag,
                                spidata->SendLen,
                                spidata->RecvLen,
                                spidata->RecvOff
                                ))
    {
        rel_timeout = (u32)DjyGetSysTime();
        if(rel_timeout - base_time < timeout)
            rel_timeout = timeout - (rel_timeout - base_time);
        else
        {
            result = CN_SPI_EXIT_TIMEOUT;
            goto exit_from_timeout;
        }

        //��Ҫ�ȴ��������1.�������ͣ�2.����δȫ���������
        if((true == block_option) || (written < spidata->SendLen))
        {
            //�ȴ��жϺ����ͷ��ź���
            if(!Lock_SempPend(SPI->SPI_BlockSemp,rel_timeout))
            {
                result = CN_SPI_EXIT_TIMEOUT;
                goto exit_from_timeout;
            }
        }

        result = CN_SPI_EXIT_NOERR;
        goto exit_from_no_err;
    }else
    {
        result = CN_SPI_EXIT_UNKNOW_ERR;
        goto exit_from_timeout;
    }

exit_from_timeout:
    if(Dev->AutoCs == true)                     //�Զ�Ƭѡʱ������ǰ������Ƭѡ
    {
        __SPI_AutoCsInactive(SPI,Dev->Cs);
    }
exit_from_bus_timeout:
exit_from_no_err:
    return result;
}

// =============================================================================
// ���ܣ������ͻ��������ݣ��������������ã�����������ȡ�����ݺ�����д��Ĵ������ͣ�
//       ���������ͣ���ֱ�Ӷ����ͻ�����ָ������ݣ������������ͣ������ж��Ƿ�ʣ������
//       �Ѵﵽ�������߽磬���ѵ��ﻺ�����߽磬����д�����������ͷ������ź���
// ������SPI,SPI���ƿ�ָ��
//       buf,�����ݻ�����ָ��
//       len,�����ݳ��ȣ��ֽڵ�λ
// ���أ��ֽ���
// =============================================================================
s32 SPI_PortRead( struct SPI_CB *SPI,u8 *buf,u32 len)
{
    u32 Result = 0,RingLen,CpyLen = 0;
    u8 *pbuf;

    if((len > 0) && (SPI->Frame.SendLen > 0))
    {
        CpyLen = SPI->Frame.SendLen >= len ?len:SPI->Frame.SendLen;
        RingLen = SPI->SPI_Buf.MaxLen;

        if(SPI->BlockOption == true)                //��������ֱ�Ӷ�������
        {
            memcpy(buf,SPI->Frame.SendBuf,CpyLen);
            SPI->Frame.SendBuf += CpyLen;
        }
        else                                        //���������ͣ����������ȡ
        {
            //�ж��Ƿ�ӻ������ж�����
            if(SPI->Frame.SendLen <= RingLen)       //�ӻ������ж�����
            {
                pbuf = &SPI->SPI_Buf.pBuf[SPI->SPI_Buf.Offset];
                SPI->SPI_Buf.Offset += CpyLen;
                memcpy(buf,pbuf,CpyLen);
            }
            else                                    //��pbuf�ж�����
            {
                memcpy(buf,SPI->Frame.SendBuf,CpyLen);
                SPI->Frame.SendBuf += CpyLen;

                //��pbufʣ������д�뻺��,���ͷ������ź���
                if(SPI->Frame.SendLen - CpyLen <= RingLen)
                {
                    memcpy(SPI->SPI_Buf.pBuf,SPI->Frame.SendBuf,
                            SPI->Frame.SendLen-CpyLen);
                    Lock_SempPost(SPI->SPI_BlockSemp);
                }
            }
        }
    }

    //ֻ�з��ͽ��ն���ɵ�ʱ���������Ƭѡ
    if(SPI->Frame.SendLen + SPI->Frame.RecvLen == 0)
    {
        if(SPI->BlockOption == true)
        {
            Lock_SempPost(SPI->SPI_BlockSemp);      //��������ʱ���ͷ������ź���
        }

        if(SPI->CurrentDev->AutoCs == true)
        {
            __SPI_AutoCsInactive(SPI,SPI->CurrentDev->Cs);
        }
    }

    SPI->Frame.SendLen -= CpyLen;
    Result = CpyLen;

    return Result;
}

// =============================================================================
// ���ܣ������յ�������д���û��ṩ�Ļ������У�������������ʽ�����ʹ���û��Ļ�����
// ������SPI,SPI���ƿ�ָ��
//       buf,����ָ��
//       len,���ݳ��ȣ��ֽڵ�λ
// ���أ��ֽ���
// =============================================================================
s32 SPI_PortWrite(struct SPI_CB *SPI,u8 *buf,u32 len)
{
    u32 cpylen;
    if((len > 0) && (SPI->Frame.RecvLen > 0))
    {
        cpylen = SPI->Frame.RecvLen >= len ?len:SPI->Frame.RecvLen;
        memcpy(SPI->Frame.RecvBuf,buf,cpylen);
        SPI->Frame.RecvBuf += cpylen;
        SPI->Frame.RecvLen -= cpylen;
    }

    //ֻ�з��ͽ��ն���ɵ�ʱ���������Ƭѡ
    if(SPI->Frame.SendLen + SPI->Frame.RecvLen == 0)
    {
        if(SPI->BlockOption == true)
        {
            Lock_SempPost(SPI->SPI_BlockSemp);      //��������ʱ���ͷ������ź���
        }
        if(SPI->CurrentDev->AutoCs == true)
        {
            __SPI_AutoCsInactive(SPI,SPI->CurrentDev->Cs);
        }
    }

    return cpylen;
}

// =============================================================================
// ���ܣ�����cmd�����������data1��Data2������SPI��Ӳ����ص���������ûص�����
//       �ɵײ��������
// ������Dev,�����ṹ��ָ��
//       cmd,����
//       data1,data2,�������ݣ����ݾ�������岻ͬ
// ���أ�-1=����������,������cmd����,����Ҫ�����û���pBusCtrl,���Ǹú�������ֵ
// =============================================================================
s32 SPI_BusCtrl(struct SPI_Device *Dev,u32 cmd,ptu32_t data1,ptu32_t data2)
{
    struct SPI_CB *SPI;
    tagSpiConfig *spicfg;
    s32 result = -1;

    if(NULL == Dev)
        return -1;

    SPI = (struct SPI_CB *)obj_GetPrivate(obj_parent(Dev->HostObj));
    if(NULL == SPI)
        return -1;

    switch(cmd)
    {
    case CN_SPI_SET_CLK:
        result = SPI->pBusCtrl(SPI->SpecificFlag,cmd,data1,Dev->Cs);
        break;
    case CN_SPI_CS_CONFIG:
        spicfg = (tagSpiConfig *)data1;
        Dev->CharLen = spicfg->CharLen;
        Dev->Freq    = spicfg->Freq;
        Dev->Mode    = spicfg->Mode;
        Dev->ShiftDir= spicfg->ShiftDir;
        result = SPI->pBusCtrl(SPI->SpecificFlag,cmd,data1,Dev->Cs);
        break;
    case CN_SPI_SET_AUTO_CS_EN:
        SPI->CurrentDev->AutoCs = true;
        result = SPI->pBusCtrl(SPI->SpecificFlag,cmd,data1,Dev->Cs);
        break;
    case CN_SPI_SET_AUTO_CS_DIS:
        SPI->CurrentDev->AutoCs = false;
        result = SPI->pBusCtrl(SPI->SpecificFlag,cmd,data1,Dev->Cs);
        break;
    case CN_SPI_SET_ERROR_POP_EVENT:
        SPI->ErrorPopEvtt = data1;
        break;
    case CN_SPI_SET_POLL:
        result = SPI->pBusCtrl(SPI->SpecificFlag,cmd,data1,data2);
        SPI->Flag |=  CN_SPI_FLAG_POLL;
        break;
    case CN_SPI_SET_INT:
        SPI->Flag &= ~CN_SPI_FLAG_POLL;
        break;
    default:
        break;
    }
    return result;
}

// =============================================================================
// ���ܣ�SPI���󵯳�API����������������ʱ�����øú����������������¼������������
//       �Բ�������ʽ���ݸ��������¼�
// ������SPI,SPI���ƿ�ָ��
//       ErrNo,����������ţ���δ�յ�ACK�ź�CN_SPI_NO_ACK_ERR
// ���أ�ִ�н����-1��������0�޴���
// =============================================================================
s32 SPI_ErrPop(struct SPI_CB *SPI, u32 ErrNo)
{
    if(NULL == SPI)
        return -1;

    //�����������¼�������������Բ�����ʽ����
    Djy_EventPop(SPI->ErrorPopEvtt,NULL,0,ErrNo,0,0);

    return CN_SPI_EXIT_NOERR;
}
