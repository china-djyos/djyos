// =============================================================================
// Copyright (C) 2012-2020 �����߲���ϵͳ�з��Ŷ�  All Rights Reserved
// �ļ���     ��cpu_peri_iic.c
// ģ������: FRAMģ�����������������������������Ա���
// ģ��汾: V1.00
// ������Ա: hm
// ����ʱ��: 20/06.2014
// =============================================================================
#include "stdint.h"
#include "cpu_peri.h"
#include "stdio.h"
#include "endian.h"
#include "fram_fm24cl64b.h"
#include "iicbus.h"
#include "endian.h"
#include "fram_fm24cl64b.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//   extern ptu32_t ModuleInit_Fram(void);
//   ModuleInit_Fram();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"fram fm24c164b"//iic�ӿڵ�����
//parent:"iicbus"    //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                   //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                  //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                 //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                  //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"iicbus","lock","cpu onchip iic"//������������������������none����ʾ�������������
                                  //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                  //����������������������г����á�,���ָ�
//weakdependence:"none"           //��������������������������none����ʾ�������������
                                  //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                  //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                  //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_FRAM_FM24C164B == false )
//#warning  " fram_fm24c164b  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header             //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_FRAM_FM24C164B    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_FRAM_BUS_NAME              "IIC0"       //"IIC����",��������ʹ�õ�IIC��������
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#define CFG_FRAM_ADDR                 (0x80)             //"��ַ",����ĵ�ַ��Ӳ����ַ��
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

#define CFG_FRAM_MAX_ADDR             (0x2000) //�����С���ֽڵ�λ
struct MutexLCB *  ptSemID_Fram;       // IIC���������־
static struct IIC_Device *s_ptFRAM_Dev=NULL;
#define CFG_FM24CL64B_TIMEOUT         (2000*mS)
#define CFG_FM24CL64B_CLK_FRE         (100*1000)      //�����ٶȣ���λHz
// =============================================================================
// ���ܣ�������ָ����ַ��ʼDstlen���ȵ�����
// ������fram_no,����ţ�Ϊ0��1
//       Fram_DstAddr��Ŀ��洢��Ԥ��ȡ���ݵĵ�ַ
//       pDstBuf����Ž��յ������ݻ����ַ
//       DstLen�� Ԥ��ȡ���ݵĳ���
// ���أ�ret, ʵ�ʶ������ֽ���
// =============================================================================
u32 FRAM_Read_Data(u32 Fram_DstAddr,u8 *pDstBuf,u32 DstLen)
{
    u32 ret = 0;
    if(Fram_DstAddr + DstLen> CFG_FRAM_MAX_ADDR)
        return 0;
    ret=IIC_Read(s_ptFRAM_Dev, Fram_DstAddr, pDstBuf, DstLen, CFG_FM24CL64B_TIMEOUT);
    return ret;
}
// =============================================================================
// ���ܣ�д����ָ����ַ��ʼDstlen���ȵ�����
// ������fram_no,����ţ�Ϊ0��1
//      Fram_DstAddr��Ŀ��洢��Ԥд���ݵ�ַ
//      pSrcBuf���������ݻ����ַ
//      SrcLen�� �������ݳ���
// ���أ�ret, ʵ��д����ֽ���
// =============================================================================
u32 FRAM_Write_Data(u32 Fram_DstAddr,u8 *pSrcBuf,u32 SrcLen)
{
    u32 ret = 0;
    if(Fram_DstAddr + SrcLen > CFG_FRAM_MAX_ADDR)
        return 0;
    ret=IIC_Write(s_ptFRAM_Dev, Fram_DstAddr, pSrcBuf, SrcLen,0, CFG_FM24CL64B_TIMEOUT);
    return ret;
}
// =============================================================================
// ���ܣ�д�ֵ����硣������ָ����ַ˫�ֳ��ȵ�����
// ������fram_no,����ţ�Ϊ0��1
//       Fram_DstAddr��Ŀ��洢��Ԥ����ַ
// ���أ�u32data,���ض���������
// =============================================================================
u32 FRAM_Read_DWord(u32 Fram_DstAddr)
{
    u32 u32data,buf[4];
    u32  rLen = 4;
    u32 ret = 0;

    ret = FRAM_Read_Data(Fram_DstAddr,(u8 *)&buf,rLen);
    if(ret != rLen)
        return 0;
    //��С��ת��
    u32data = pick_little_32bit((u8 *)&buf,0);
    return u32data;
}
// =============================================================================
// ���ܣ�д�ֵ����磬д˫�����ݵ�ָ����ַ��FRAM
// ������fram_no,����ţ�Ϊ0��1
//       Fram_DstAddr��Ŀ��洢��Ԥд��ַ
// ���أ�ret, ���������룬�޴���ΪERR_N0_ERROR
// =============================================================================
u32 FRAM_Write_DWord(u32 Fram_DstAddr,u32 u32data)
{
    u32  rLen = 4;
    u8 buf[4];
    u32 ret = 0;

    fill_little_32bit((u8 *)&buf,0,u32data);
    ret = FRAM_Write_Data(Fram_DstAddr,(u8 *)&buf,rLen);
    return ret;
}

// =============================================================================
// ���ܣ���˫�֣�������ָ����ַ˫�ֳ��ȵ�����
// ������fram_no,����ţ�Ϊ0��1
//       Fram_DstAddr��Ŀ��洢��Ԥ����ַ
// ���أ�u16data,���ض���������
// =============================================================================
u16 FRAM_Read_Word(u32 Fram_DstAddr)
{
    u16 u16data,buf[2];
    u8  rLen = 2;
    u32 ret = 0;

    ret = FRAM_Read_Data(Fram_DstAddr,(u8 *)&buf,rLen);
    if(ret != rLen)
        return 0;
    //��С�˴洢�ж�ȡ�������ݣ�ת��Ϊ���ģʽ
    u16data = pick_little_16bit((u8 *)&buf,0);
    return u16data;
}

// =============================================================================
// ���ܣ�д�ֵ����磬д�����ݵ�ָ����ַ��FRAM
// ������fram_no,����ţ�Ϊ0��1
//       Fram_DstAddr��Ŀ��洢��Ԥд��ַ
//       u16data,д�������
// ���أ�ret, ���������룬�޴���ΪERR_N0_ERROR
// =============================================================================
u32 FRAM_Write_Word( u32 Fram_DstAddr,u16 u16data)
{
    u8  rLen = 2,buf[2];
    u32 ret = 0;

    //����˸ĳ�С�� ����С�˷�ʽ�洢��FRAM
    fill_little_16bit((u8 *)&buf,0,u16data);
    ret = FRAM_Write_Data(Fram_DstAddr,(u8 *)&buf,rLen);
    return ret;
}
// =============================================================================
// ���ܣ�������д�ֵ����磬������WORD��ȡ��������
// ��������ȡWORD��ַ
// ���أ���ȡ��WORD����
// =============================================================================
u16 FRAM_Read_Word_r(u16 wAddress )
{
    u16    wData = 0;
    if(-1 == Lock_MutexPend( ptSemID_Fram, CN_TIMEOUT_FOREVER))  // ��ȡдȨ��
    {
        printf("FRAM1_Read_Word_r ��ȡдȨ��ʧ��\n");
        return  false;
    }
    wData = FRAM_Read_Word(wAddress );   // д����
    Lock_MutexPost( ptSemID_Fram );
    return    wData;
}

// =============================================================================
// ���ܣ��źű��������ݣ������u8��ȡ��������
// ��������ȡWORD��ַ, ���滺���� ��ȡ����
// ���أ� ��ȡ���ݳ���
// =============================================================================
u32 FRAM_Read_Data_r( u16 wAddress, u8 *pbyBuf, u32 dwLen )
{
    register u32      dwStatus = 0;

    if(-1 == Lock_MutexPend( ptSemID_Fram, CN_TIMEOUT_FOREVER))   // ��ȡдȨ��
    {
        printf("FRAM1_Read_Data ��ȡдȨ��ʧ��\n");
        return  false;
    }
    dwStatus = FRAM_Read_Data(wAddress, pbyBuf, dwLen );// д����
    Lock_MutexPost( ptSemID_Fram );

    return    dwStatus;
}

// =============================================================================
// ���ܣ��źű������֣�������WORDд��������
// ��������ȡWORD��ַ, д������
// ���أ�  д�����ݳ���
// =============================================================================
s32 FRAM_Write_Word_r(u16 wAddress, u16 wData )
{
    s32    Status = 0;
    if(-1 == Lock_MutexPend( ptSemID_Fram, CN_TIMEOUT_FOREVER))       // ��ȡдȨ��
    {
        printf("FRAM1_Write_Word_r ��ȡдȨ��ʧ��\n");
        return  false;
    }
    Status = FRAM_Write_Word(wAddress, wData);   // д����
    Lock_MutexPost( ptSemID_Fram);
    return    Status;
}

// =============================================================================
// ���ܣ��źű���д���ݣ������u8д��������
// ��������ȡWORD��ַ, д������
// ���أ�  д�����ݳ���
// =============================================================================
u32 FRAM_Write_Data_r(u16 wAddress, u8 *pbyBuf, u32 dwLen)
{
    register u32      dwStatus = 0;
    if(-1 == Lock_MutexPend( ptSemID_Fram, CN_TIMEOUT_FOREVER))   // ��ȡдȨ��
    {
        printf("FRAM1_Write_Data_r ��ȡдȨ��ʧ��\n");
        return  false;
    }
    dwStatus = FRAM_Write_Data(wAddress, pbyBuf, dwLen );// д����
    Lock_MutexPost( ptSemID_Fram );

    return    dwStatus;
}

// =============================================================================
// ���ܣ���ʼ��framоƬ������IIC������ʼ������
// ����: para,����ţ�0��1
// ����: true= �ɹ���false=ʧ��
// =============================================================================
ptu32_t ModuleInit_Fram(void)
{
    ptSemID_Fram = Lock_MutexCreate("Fram0 Lock");

    s_ptFRAM_Dev = IIC_DevAdd(CFG_FRAM_BUS_NAME,"FRAM_FM24CL64B",CFG_FRAM_ADDR,1,17);
    if(s_ptFRAM_Dev)
    {
        IIC_BusCtrl(s_ptFRAM_Dev,CN_IIC_SET_CLK,CFG_FM24CL64B_CLK_FRE,0);
        IIC_BusCtrl(s_ptFRAM_Dev,CN_IIC_SET_POLL,0,0);
        return true;
    }
    else
    {
        Lock_MutexDelete(ptSemID_Fram);
        return false;
    }
}
