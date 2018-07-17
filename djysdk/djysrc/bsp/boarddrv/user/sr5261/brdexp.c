// =============================================================================
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 文件名     ：brdexp.c
// 模块描述: 异常模块存储与解读，将AT45的最好1M配置为异常存储
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 12/03.2016
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
//异常存储方案说明：
//1、使用片上FLASH的最后一个sector(256k)用于存储异常记录信息,一共512pages，每个page 512bytes，一个page
//  存储一条异常记录信息,记录异常的起始page为((2048-256)*1024/512=3584)；
//2、从地址偏移为RECORD_SIZE顺序存入每条异常信息，每条异常信息包括：
//   16字节异常信息长度（四部分，分别为headinfolen,osstateinfolen,hookinfolen,throwinfolen）
//   和异常信息（分为四部分，分别为headinfo,osstateinfo,hookinfo,throwinfo)

#define RECORD_EXP_PAGE_SIZE       (512)     //异常存储页大小
#define RECORD_EXP_START_PAGE      (3584)    //异常存储起始页
#define RECORD_EXP_END_PAGE        (4095)   //异常存储结束页
#define RECORD_EXP_SECTOR_NO       (11)     //异常存储Sector
#define RECODR_NO_MAX              (512)
#define RECORD_MAGICNUM            (0xAA55)

static u8 verify_code[10]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa};
static u8 gs_expbuf[RECORD_EXP_PAGE_SIZE];
typedef struct _BrdExp_R
{
    u32 MagicNum;           //验证码
    u32 RecordNo;           //异常存储的序号
    u32 RecordAddr;         //该异常存储信息首地址（偏移首地址，在这里指的是page编号）
    u32 RecordLen;          //该异常存储信息总长度
}tagExpRecMsg;

static tagExpRecMsg sLastRecMsg;
// =============================================================================
// 函数功能：保存一帧异常信息到非易失存储器。存储或者输出，或者两者兼顾，由异常存储方案设计者决定
// 输入参数：recordpara,一帧异常信息
// 输出参数：
// 返回值  ：见enum EN_ExpDealResult
// =============================================================================
enum EN_ExpDealResult BrdExp_RecordSave(struct ExpRecordPara  *recordpara)
{
    tagExpRecMsg CurRecMsg;
    u32 addr=0;
    s32 Ret=0;
    if(NULL == recordpara)
        return EN_EXP_RESULT_PARAERR;

    //第一条异常信息
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

    //写入存储信息条目
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
// 函数功能：fnExp_RecordClean
//          清除所有的异常信息，清除异常信息存储区域
// 输入参数：无
// 输出参数：无
// 返回值  ：TRUE 成功， FALSE失败
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
// 函数功能：fnExp_RecordCheckNum
//          查看一共存储了多少条异常信息
// 输入参数：
// 输出参数：recordnum,存储的异常信息条目数
// 返回值  ：FALSE,失败 TRUE成功
// =============================================================================
bool_t BrdExp_RecordCheckNum(u32 *recordnum)
{
    if(NULL == recordnum)
        return false;
    *recordnum = sLastRecMsg.RecordNo;
    return true;
}
// =============================================================================
// 函数功能：fnBrdExp_RecordCheckLen
//          查看指定异常条目的长度
// 输入参数：assignedno,指定的条目
// 输出参数：recordlen,该条目的长度
// 返回值  ：FALSE,失败 TRUE成功
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
// 函数功能：fnExp_RecordGet
//          从存储介质中获取指定条目的异常帧信息
// 输入参数：assignedno,指定的异常帧条目
//           buflen,缓冲区长度
// 输出参数：buf,用于存储获取指定条目的异常信息
//          recordpara,异常信息存储时的参数，在此是对buf的各个部分的定义
// 返回值  ：FALSE,失败 TRUE成功
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
// 函数功能：fnExp_RecordScan
//           开机的时候扫描异常存储记录，获取关键信息方便以后存储
// 输入参数：无
// 输出参数：无
// 返回值  ：空
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
// 函数功能：fnExp_RecordScan
//           开机的时候扫描异常存储记录，获取关键信息方便以后存储
// 输入参数：无
// 输出参数：无
// 返回值  ：空
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

