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
#include "at45db321.h"
#include "exp.h"

//�쳣�洢����˵����
//1��ʹ��AT45���1M�Ŀռ�洢�쳣��Ϣ��
//2��ʹ�ô洢�ռ�ĳ�ʼRECORD_SIZE��Ϊ�洢������Ϣ��
//3���ӵ�ַƫ��ΪRECORD_SIZE˳�����ÿ���쳣��Ϣ��ÿ���쳣��Ϣ������
//   16�ֽ��쳣��Ϣ���ȣ��Ĳ��֣��ֱ�Ϊheadinfolen,osstateinfolen,hookinfolen,throwinfolen��
//   ���쳣��Ϣ����Ϊ�Ĳ��֣��ֱ�Ϊheadinfo,osstateinfo,hookinfo,throwinfo)

#define RECPAGE_SIZE        (512)
#define RECORD_SIZE         (512*8)     //�����쳣�洢��Ϣ����
#define RECORD_START_ADDR   (0x300000)  //�洢����ʼλ��
#define RECORD_LENGTH       (0x100000)  //1M�ֽڵĴ洢�ռ�
//static u8 sRecMsgBuf[RECORD_SIZE];

typedef struct _BrdExp_R
{
    u32 RecordNo;           //�쳣�洢�����
    u32 RecordAddr;         //���쳣�洢��Ϣ�׵�ַ��ƫ���׵�ַ��
    u32 RecordLen;          //���쳣�洢��Ϣ�ܳ���
}tagExpRecMsg;

static tagExpRecMsg sLastRecMsg;
// =============================================================================
// �������ܣ���ѯbuf�����һ���쳣��¼��Ŀ
// �����������
// �����������
// ����ֵ  ����
// =============================================================================
static tagExpRecMsg *__ExpRec_Traves(u8 *buf,u32 buflen)
{
    tagExpRecMsg *ExpRecMsg = NULL;
    u32 loop;

    for(loop = 0; loop < buflen -sizeof(tagExpRecMsg); )
    {
        if((u32)buf[loop] + 1 != (u32)buf[loop + sizeof(tagExpRecMsg)])
        {
            break;
        }
        loop += sizeof(tagExpRecMsg);
    }

    ExpRecMsg = (tagExpRecMsg *)&buf[loop];

    if(ExpRecMsg->RecordNo == 0xFFFFFFFF)
    {
        return NULL;
    }
    return ExpRecMsg;
}
// =============================================================================
// �������ܣ�����һ֡�쳣��Ϣ������ʧ�洢�����洢����������������߼�ˣ����쳣�洢��������߾���
// ���������recordpara,һ֡�쳣��Ϣ
// ���������
// ����ֵ  ����enum EN_ExpDealResult
// =============================================================================
enum EN_ExpDealResult BrdExp_RecordSave(struct ExpRecordPara  *recordpara)
{
    tagExpRecMsg CurRecMsg;
    u32 addr;
    u8 buffer[16];

    if(NULL == recordpara)
        return EN_EXP_RESULT_PARAERR;

    //��һ���쳣��Ϣ
    if(sLastRecMsg.RecordLen == 0)
    {
        CurRecMsg.RecordNo = 1;
        CurRecMsg.RecordAddr = RECORD_START_ADDR + RECORD_SIZE;
    }
    else
    {
        CurRecMsg.RecordNo = sLastRecMsg.RecordNo + 1;
        CurRecMsg.RecordAddr = sLastRecMsg.RecordAddr + sLastRecMsg.RecordLen;
    }

    CurRecMsg.RecordLen = recordpara->headinfolen
                            + recordpara->hookinfolen
                            + recordpara->osstateinfolen
                            + recordpara->throwinfolen + 16;


    if( (sLastRecMsg.RecordNo > RECORD_LENGTH/sizeof(tagExpRecMsg)) ||
            (CurRecMsg.RecordAddr + CurRecMsg.RecordLen > RECORD_LENGTH +RECORD_START_ADDR))
    {
        return EN_EXP_RESULT_RECORD_NOSPACE;
    }

    //д��洢��Ϣ��Ŀ
    addr = RECORD_START_ADDR + (CurRecMsg.RecordNo - 1) * sizeof(tagExpRecMsg);
    AT45_FLASH_Write(addr,(u8*)&CurRecMsg,sizeof(tagExpRecMsg));

    //д���쳣��Ϣ
    addr = CurRecMsg.RecordAddr;
    memcpy(buffer,      (u8*)&recordpara->headinfolen,4);
    memcpy(buffer + 4,  (u8*)&recordpara->osstateinfolen,4);
    memcpy(buffer + 8,  (u8*)&recordpara->hookinfolen,4);
    memcpy(buffer + 12, (u8*)&recordpara->throwinfolen,4);
    AT45_FLASH_Write(addr ,buffer,16);

    addr += 16;
    AT45_FLASH_Write(addr,(u8*)recordpara->headinfoaddr,recordpara->headinfolen);
    addr += recordpara->headinfolen;
    AT45_FLASH_Write(addr,(u8*)recordpara->osstateinfoaddr,recordpara->osstateinfolen);
    addr += recordpara->osstateinfolen;
    AT45_FLASH_Write(addr,(u8*)recordpara->hookinfoaddr,recordpara->hookinfolen);
    addr += recordpara->hookinfolen ;
    AT45_FLASH_Write(addr,(u8*)recordpara->throwinfoaddr,recordpara->throwinfolen);

    sLastRecMsg = CurRecMsg;
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
    u32 StartAddr,EndAddr;
    u32 loop;

    StartAddr = RECORD_START_ADDR;
    EndAddr   = (RECORD_START_ADDR + RECORD_SIZE);
    for(loop = StartAddr;loop <= EndAddr;)
    {
        AT45_Page_Erase(loop);
        loop += RECPAGE_SIZE;
    }
    sLastRecMsg.RecordNo = 0;
    sLastRecMsg.RecordAddr = 0;
    sLastRecMsg.RecordLen = 0;
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
//          ����ָ���쳣��Ŀ�ĳ���
// ���������assignedno,ָ������Ŀ
// ���������recordlen,����Ŀ�ĳ���
// ����ֵ  ��FALSE,ʧ�� TRUE�ɹ�
// =============================================================================
bool_t BrdExp_RecordCheckLen(u32 assignedno, u32 *recordlen)
{
    tagExpRecMsg *pRecMsg;
    u8 buffer[sizeof(tagExpRecMsg)];
    u32 addr;

    if( (NULL == recordlen) || (assignedno > sLastRecMsg.RecordNo) )
        return false;

    addr = RECORD_START_ADDR + (assignedno-1) * sizeof(tagExpRecMsg);
    //�ȶ����洢��Ŀ��Ϣ
    AT45_FLASH_Read(addr,buffer,sizeof(tagExpRecMsg));
    //��ѯ���һ���쳣��Ϣ��λ�úʹ洢λ��
    pRecMsg = (tagExpRecMsg *)buffer;

    *recordlen = pRecMsg->RecordLen - 4*4;

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
    tagExpRecMsg *pRecMsg;
    u32 addr;
    u8 buffer[16];

    if( (NULL == recordpara) || (NULL == buf))
        return false;

    addr = RECORD_START_ADDR + (assignedno-1) * sizeof(tagExpRecMsg);
    //�ȶ����洢��Ŀ��Ϣ
    AT45_FLASH_Read(addr,buffer,sizeof(tagExpRecMsg));
    //��ѯ���һ���쳣��Ϣ��λ�úʹ洢λ��
    pRecMsg = (tagExpRecMsg *)buffer;

    addr = pRecMsg->RecordAddr;
    AT45_FLASH_Read(addr,buffer,16);
    recordpara->headinfolen = *((u32*)&buffer[0]);
    recordpara->osstateinfolen = *((u32*)&buffer[4]);
    recordpara->hookinfolen = *((u32*)&buffer[8]);
    recordpara->throwinfolen = *((u32*)&buffer[12]);

    recordpara->headinfoaddr = (ptu32_t)&buf[0];
    recordpara->osstateinfoaddr = (ptu32_t)(buf + recordpara->headinfolen);
    recordpara->hookinfoaddr = (ptu32_t)(buf
                                            + recordpara->headinfolen
                                            + recordpara->osstateinfolen);
    recordpara->throwinfoaddr = (ptu32_t)(buf
                                            + recordpara->headinfolen
                                            + recordpara->hookinfolen
                                            + recordpara->osstateinfolen);
    addr += 16;
    AT45_FLASH_Read(addr,buf,buflen);

    //--TODO modified by zqf,read order must be identical with the save order
    //       otherwise you could not decode the record message as you wish

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
    u8 buffer[50*sizeof(tagExpRecMsg)];
    tagExpRecMsg *pLastRecMsg;
    u32 addr,length;

    addr = RECORD_START_ADDR;
    length = 50*sizeof(tagExpRecMsg);

    do{
        AT45_FLASH_Read(addr,buffer,length);
        pLastRecMsg = __ExpRec_Traves(buffer,length);
        if(NULL != pLastRecMsg)
        {
            sLastRecMsg.RecordNo = pLastRecMsg->RecordNo;
            sLastRecMsg.RecordAddr = pLastRecMsg->RecordAddr;
            sLastRecMsg.RecordLen = pLastRecMsg->RecordLen;

            if( (sLastRecMsg.RecordNo%50) )
                break;
        }
        else
        {
            break;
        }
        addr += length;
    }while(addr < (RECORD_START_ADDR + RECORD_SIZE));

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

	if(AT45_FLASH_Ready())
	{
		ExpOpt.fnExpRecordScan      = BrdExp_RecordScan;
		ExpOpt.fnExpRecordSave      = BrdExp_RecordSave;
		ExpOpt.fnExpRecordClean     = BrdExp_RecordClean;
		ExpOpt.fnExpRecordGet       = BrdExp_RecordGet;
		ExpOpt.fnExpRecordCheckNum  = BrdExp_RecordCheckNum;
		ExpOpt.fnExpRecordCheckLen  = BrdExp_RecordCheckLen;

		if(!Exp_RegisterRecorder(&ExpOpt))
			return false;
		else
			return true;
	}
	else
        return false;
}

