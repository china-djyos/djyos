// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��brdexp.c
// ģ������: �쳣ģ��洢��������AT45�����1M����Ϊ�쳣�洢
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 12/03.2016
// =============================================================================
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "os.h"
#include <device/flash/flash.h>
#include "exp.h"


extern s32 Flash_PageRead(u32 Page, u8 *Data, u32 Flags);
extern s32 Flash_PageProgram(u32 Page, u8 *Data, u32 Flags);
extern s32 Flash_SectorEarse(u32 SectorNo);
//�쳣�洢����˵����
//1��ʹ��Ƭ��FLASH�����һ��sector(256k)���ڴ洢�쳣��¼��Ϣ,һ��512pages��ÿ��page 512bytes��һ��page
//  �洢һ���쳣��¼��Ϣ,��¼�쳣����ʼpageΪ((2048-256)*1024/512=3584)��
//2���ӵ�ַƫ��ΪRECORD_SIZE˳�����ÿ���쳣��Ϣ��ÿ���쳣��Ϣ������
//   16�ֽ��쳣��Ϣ���ȣ��Ĳ��֣��ֱ�Ϊheadinfolen,osstateinfolen,hookinfolen,throwinfolen��
//   ���쳣��Ϣ����Ϊ�Ĳ��֣��ֱ�Ϊheadinfo,osstateinfo,hookinfo,throwinfo)

#define RECORD_EXP_PAGE_SIZE       (512)     //�쳣�洢ҳ��С
#define RECORD_EXP_START_PAGE      (3584)    //�쳣�洢��ʼҳ
#define RECORD_EXP_END_PAGE        (4095)   //�쳣�洢����ҳ
#define RECORD_EXP_SECTOR_NO       (11)     //�쳣�洢Sector
#define RECODR_NO_MAX              (512)
#define RECORD_MAGICNUM            (0xAA55)

static u8 verify_code[10]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa};
static u8 gs_expbuf[RECORD_EXP_PAGE_SIZE];
typedef struct _BrdExp_R
{
    u32 MagicNum;           //��֤��
    u32 RecordNo;           //�쳣�洢�����
    u32 RecordAddr;         //���쳣�洢��Ϣ�׵�ַ��ƫ���׵�ַ��������ָ����page��ţ�
    u32 RecordLen;          //���쳣�洢��Ϣ�ܳ���
}tagExpRecMsg;

static tagExpRecMsg sLastRecMsg;
// =============================================================================
// �������ܣ�����һ֡�쳣��Ϣ������ʧ�洢�����洢����������������߼�ˣ����쳣�洢��������߾���
// ���������recordpara,һ֡�쳣��Ϣ
// ���������
// ����ֵ  ����enum EN_ExpDealResult
// =============================================================================
enum EN_ExpDealResult BrdExp_RecordSave(struct ExpRecordPara  *recordpara)
{
    tagExpRecMsg CurRecMsg;
    u32 addr=0;
    s32 Ret=0;
    if(NULL == recordpara)
        return EN_EXP_RESULT_PARAERR;

    //��һ���쳣��Ϣ
    if(sLastRecMsg.RecordLen == 0)
    {
        CurRecMsg.MagicNum=RECORD_MAGICNUM;
        CurRecMsg.RecordNo = 1;
        CurRecMsg.RecordAddr = RECORD_EXP_START_PAGE;
    }
    else
    {
        CurRecMsg.MagicNum=RECORD_MAGICNUM;
        CurRecMsg.RecordNo = sLastRecMsg.RecordNo + 1;
        CurRecMsg.RecordAddr = sLastRecMsg.RecordAddr + 1;
    }

    CurRecMsg.RecordLen = recordpara->headinfolen
                            + recordpara->hookinfolen
                            + recordpara->osstateinfolen
                            + recordpara->throwinfolen
                            + 16 + sizeof(tagExpRecMsg);


    if(sLastRecMsg.RecordNo >= RECORD_EXP_END_PAGE)
    {
        return EN_EXP_RESULT_RECORD_NOSPACE;
    }

    //д��洢��Ϣ��Ŀ
    memcpy(gs_expbuf,(u8 *)&CurRecMsg,sizeof(tagExpRecMsg));
    addr+=sizeof(tagExpRecMsg);
    memcpy(gs_expbuf + addr,      (u8*)&recordpara->headinfolen,4);
    addr+=4;
    memcpy(gs_expbuf + addr,  (u8*)&recordpara->osstateinfolen,4);
    addr+=4;
    memcpy(gs_expbuf + addr,  (u8*)&recordpara->hookinfolen,4);
    addr+=4;
    memcpy(gs_expbuf + addr, (u8*)&recordpara->throwinfolen,4);
    addr+=4;
    if(recordpara->headinfolen!=0)
    {
       memcpy(gs_expbuf + addr, (u8*)recordpara->headinfoaddr,recordpara->headinfolen);
       addr+=recordpara->headinfolen;
    }
    if(recordpara->osstateinfolen!=0)
    {
       memcpy(gs_expbuf + addr, (u8*)recordpara->osstateinfoaddr,recordpara->osstateinfolen);
       addr+=recordpara->osstateinfolen;
    }
    if(recordpara->hookinfolen)
    {
       memcpy(gs_expbuf + addr, (u8*)recordpara->hookinfoaddr,recordpara->hookinfolen);
       addr+=recordpara->hookinfolen;
    }
    if(recordpara->throwinfolen!=0)
    {
       memcpy(gs_expbuf + addr, (u8*)recordpara->throwinfoaddr,recordpara->throwinfolen);
    }


    Ret=Flash_PageProgram(CurRecMsg.RecordAddr,gs_expbuf,NO_ECC);
    if(Ret==-2)
    {
        return EN_EXP_RESULT_RECORD_HARDERR;
    }
    if(Ret!=-2)
    {
        memcpy(&sLastRecMsg,&CurRecMsg,sizeof(tagExpRecMsg));
    }
    return EN_EXP_RESULT_SUCCESS;
}

// =============================================================================
// �������ܣ�fnExp_RecordClean
//          ������е��쳣��Ϣ������쳣��Ϣ�洢����
// �����������
// �����������
// ����ֵ  ��TRUE �ɹ��� FALSEʧ��
// =============================================================================
bool_t BrdExp_RecordClean(void)
{
    s32 Ret;
    Ret=Flash_SectorEarse(RECORD_EXP_SECTOR_NO);
    if(Ret==-1)
    {
        printf("clear exp record info failed.\r\n");
        return false;
    }
    sLastRecMsg.MagicNum=0;
    sLastRecMsg.RecordNo = 0;
    sLastRecMsg.RecordAddr = 0;
    sLastRecMsg.RecordLen = 0;
    printf("Clear exp record info success.\r\n");
    return true;
}


// =============================================================================
// �������ܣ�fnExp_RecordCheckNum
//          �鿴һ���洢�˶������쳣��Ϣ
// ���������
// ���������recordnum,�洢���쳣��Ϣ��Ŀ��
// ����ֵ  ��FALSE,ʧ�� TRUE�ɹ�
// =============================================================================
bool_t BrdExp_RecordCheckNum(u32 *recordnum)
{
    if(NULL == recordnum)
        return false;
    *recordnum = sLastRecMsg.RecordNo;
    return true;
}
// =============================================================================
// �������ܣ�fnBrdExp_RecordCheckLen
//          �鿴ָ���쳣��Ŀ�ĳ���
// ���������assignedno,ָ������Ŀ
// ���������recordlen,����Ŀ�ĳ���
// ����ֵ  ��FALSE,ʧ�� TRUE�ɹ�
// =============================================================================
bool_t BrdExp_RecordCheckLen(u32 assignedno, u32 *recordlen)
{
    tagExpRecMsg *pRecMsg;
    s32 Ret;
    if( (NULL == recordlen) || (assignedno > sLastRecMsg.RecordNo)
            ||(assignedno >= RECODR_NO_MAX))
        return false;

    Ret=Flash_PageRead(RECORD_EXP_START_PAGE+assignedno-1,gs_expbuf,NO_ECC);
    if(Ret==-1)
    {
        return false;
    }
    pRecMsg=(tagExpRecMsg *)gs_expbuf;
    *recordlen = pRecMsg->RecordLen;
    memset(gs_expbuf,0,RECORD_EXP_PAGE_SIZE);
    return true;
}

// =============================================================================
// �������ܣ�fnExp_RecordGet
//          �Ӵ洢�����л�ȡָ����Ŀ���쳣֡��Ϣ
// ���������assignedno,ָ�����쳣֡��Ŀ
//           buflen,����������
// ���������buf,���ڴ洢��ȡָ����Ŀ���쳣��Ϣ
//          recordpara,�쳣��Ϣ�洢ʱ�Ĳ������ڴ��Ƕ�buf�ĸ������ֵĶ���
// ����ֵ  ��FALSE,ʧ�� TRUE�ɹ�
// =============================================================================
bool_t BrdExp_RecordGet(u32 assignedno,u32 buflen,u8 *buf,\
                            struct ExpRecordPara *recordpara)
{
   s32 Ret=0;
    if( (NULL == recordpara) || (NULL == buf))
        return false;
    if(assignedno>=RECODR_NO_MAX)
    {
        printf("%s function para is invalid.\r\n",__FUNCTION__);
        return false;
    }

    Ret=Flash_PageRead(RECORD_EXP_START_PAGE+assignedno-1,gs_expbuf,NO_ECC);
    if(Ret==-1)
    {
        printf("Get exp log failed.\r\n");
        memset(gs_expbuf,0,RECORD_EXP_PAGE_SIZE);
       return false;
    }
    memcpy(buf,gs_expbuf,buflen);
    recordpara->headinfolen=*((u32 *)&buf[16]);
    recordpara->osstateinfolen=*((u32 *)&buf[20]);
    recordpara->hookinfolen=*((u32 *)&buf[24]);
    recordpara->throwinfolen=*((u32 *)&buf[28]);

    recordpara->headinfoaddr=(ptu32_t)&buf[32];
    recordpara->osstateinfoaddr=(ptu32_t)(&buf[32]+recordpara->headinfolen);
    recordpara->hookinfoaddr=(ptu32_t)(&buf[32] + recordpara->headinfolen
                                                + recordpara->osstateinfolen);
    recordpara->throwinfoaddr=(ptu32_t)(&buf[32] + recordpara->headinfolen
                                                + recordpara->osstateinfolen
                                                + recordpara->hookinfolen);
    memset(gs_expbuf,0,RECORD_EXP_PAGE_SIZE);
    return true;
}

// =============================================================================
// �������ܣ�fnExp_RecordScan
//           ������ʱ��ɨ���쳣�洢��¼����ȡ�ؼ���Ϣ�����Ժ�洢
// �����������
// �����������
// ����ֵ  ����
// =============================================================================
void BrdExp_RecordScan(void)
{
    tagExpRecMsg *pLastRecMsg;
    u16 i=0;

    for(i=0;i<RECODR_NO_MAX;i++)
    {
        Flash_PageRead(RECORD_EXP_START_PAGE+i,gs_expbuf,NO_ECC);
        pLastRecMsg=(tagExpRecMsg *)gs_expbuf;
        if(pLastRecMsg->MagicNum==RECORD_MAGICNUM)
        {
            sLastRecMsg.MagicNum=RECORD_MAGICNUM;
            sLastRecMsg.RecordAddr=pLastRecMsg->RecordAddr;
            sLastRecMsg.RecordLen=pLastRecMsg->RecordLen;
            sLastRecMsg.RecordNo=pLastRecMsg->RecordNo;
        }
        else
        {
            break;
        }
    }
}

// =============================================================================
// �������ܣ�fnExp_RecordScan
//           ������ʱ��ɨ���쳣�洢��¼����ȡ�ؼ���Ϣ�����Ժ�洢
// �����������
// �����������
// ����ֵ  ����
// =============================================================================
bool_t ModuleInstall_BrdExp(ptu32_t para)
{
    struct ExpRecordOperate ExpOpt;
    ExpOpt.fnExpRecordScan      = BrdExp_RecordScan;
    ExpOpt.fnExpRecordSave      = BrdExp_RecordSave;
    ExpOpt.fnExpRecordClean     = BrdExp_RecordClean;
    ExpOpt.fnExpRecordGet       = BrdExp_RecordGet;
    ExpOpt.fnExpRecordCheckNum  = BrdExp_RecordCheckNum;
    ExpOpt.fnExpRecordCheckLen  = BrdExp_RecordCheckLen;

//      stm32f7_PageRead(RECORD_EXP_END_PAGE,gs_expbuf,HW_ECC);
//      for(i=0;i<10;i++)
//      {
//          if(gs_expbuf[i]!=verify_code[i])
//          {
//              BrdExp_RecordClean();
//              //write verify code
//              stm32f7_PageProgram(RECORD_EXP_END_PAGE,verify_code,HW_ECC);
//              break;
//          }
//      }
    memset(gs_expbuf,0,RECORD_EXP_PAGE_SIZE);
    memset(&sLastRecMsg,0,sizeof(tagExpRecMsg));

    if(!Exp_RegisterRecorder(&ExpOpt))
    {
        printf("Exp RegisterRecoder failed.\r\n");
        return false;
    }
    else
    {
        printf("Exp RegisterRecoder success.\r\n");
        return true;
    }
}

