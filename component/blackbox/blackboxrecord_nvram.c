//-----------------------------------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY BLACKBOXRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合以下三条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然:
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得的书面许可，不允许移植到非
//    都江堰操作系统环境下运行。

// 免责声明:本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
// =============================================================================

// 文件名  ：brdblackbox.c
// 模块描述: 黑盒子模块异常信息存储模块，适用场合：1、NVRAM中存储异常信息，有些工程设计
//          有一小片RAM，是由RTC电池供电的（例如STM32片内有4K），掉电不丢失。2、对于
//          允许字节为单位擦写的EEPROM，也可以参照这个编写。
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 12/03.2016
// =============================================================================
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <os.h>
#include <blackbox.h>
#include <shell.h>
#include "dbug.h"

#define CN_ITEM_STATE_INUSE  (1<<0)
#define CN_ITEM_STATE_HEAD   (1<<1)
#define CN_ITEM_STATE_TAIL   (1<<2)
#define CN_ITEM_MAGIC        0x87654321         //the content before the blackbox information must be magic number

typedef struct
{
    u16 headinfolen;
    u16 ostatelen;
    u16 hookinfolen;
    u16 throwinfolen;
}tagLenTab;
typedef struct
{
    u32 magic;  //if it is not the magic, then it must be invalid
    u32 no;     //it is the magic no,increased one by one
    u16 state;  //means used or not
    u16 size;   //means how many data it could storage
    u16 msglen; //it is the message in the item
    u16 nxtptr; //point to the nxt item if it exist
    tagLenTab lentab;
}tagRingItem;
#define CN_ITEM_HDRLEN  sizeof(tagRingItem)
#define CN_ITEM_MIN     (0X20 + CN_ITEM_HDRLEN)    //AT LEAST 32 BYTES OF DATA SPACE FOR ONE ITEM

typedef struct
{
    u32 magic;    //if not correct, then maybe destroyed
    u32 maxno;    //the max no has been used
    u16 nxtptr;   //point to the next valid item
}tagController;
//static tagController    gController;
#define CN_CONTROLLER_BASE  0
#define CN_CONTROLLER_MAGIC (~CN_ITEM_MAGIC)
#define CN_ITEM_BASE        sizeof(tagController)
#define CN_CTRL_SIZE        sizeof(tagController)

typedef struct
{
    u32                size;
//  tagController      ctrl;
    tagBlackBoxLowLevelOpt  lopt;
}tagMemContrl;
static tagMemContrl  gMemCtrl;
#define CN_LINEMEM_MINI  (CN_CTRL_SIZE +CN_ITEM_MIN)
////////////////////low level media interface///////////////////////////////////
static bool_t __MemReadL(u16 offset,void *buf,u16 len)
{
    bool_t ret = false;
    if(NULL == gMemCtrl.lopt.read)
    {
        return ret;
    }
    if((offset + len) <=gMemCtrl.size)
    {
        ret = gMemCtrl.lopt.read(offset,buf,len);
    }
    return ret;
}
//write the bkpsram len bytes to addr from buf
static bool_t __MemWriteL(u16 offset,void *buf,u16 len)
{
    bool_t ret = false;
    if(NULL == gMemCtrl.lopt.write)
    {
        return ret;
    }
    if((offset + len) <= gMemCtrl.size)
    {
        ret =gMemCtrl.lopt.write(offset,buf,len);
    }
    return ret;
}
//zero bkpsram
static bool_t __MemFormatL(u16 offset,u16 len)
{
    bool_t ret = false;
    if(NULL == gMemCtrl.lopt.format)
    {
        return ret;
    }
    if((offset + len) <= gMemCtrl.size)
    {
        ret =gMemCtrl.lopt.format(offset,len);
    }
    return ret;
}
static bool_t __MemInitL(void)
{
    bool_t ret = false;
    if(NULL == gMemCtrl.lopt.init)
    {
        return ret;
    }
    ret =gMemCtrl.lopt.init();
    return ret;
}
/////////////////////////////////////////////////////////////////////
//RAM DISK MAP
//controller item1 item2 item3 ......
//ITEM MAP
//itemhead msg
/////////////////////////////////////////////////////////////////////
//GET AND SET THE CONTROLLER
static bool_t __ControllerGet(tagController *ctrl)
{
    bool_t ret;
    ret =__MemReadL(CN_CONTROLLER_BASE,(u8 *)ctrl,sizeof(tagController));
    if((ret)&&(ctrl->magic == CN_CONTROLLER_MAGIC))
    {
        ret = true;
    }
    else
    {
        ret = false;
    }
    return ret;
}
static bool_t __ControllerSet(tagController *ctrl)
{
    bool_t ret;
    ret = __MemWriteL(CN_CONTROLLER_BASE,(u8 *)ctrl,sizeof(tagController));
    return ret;
}


//GET AND SET THE ITEM
//use this function to get the item
static bool_t  __ItemGet(u16 offset,tagRingItem *item)
{
    bool_t ret = false;
    ret = __MemReadL(offset,(u8 *)item,sizeof(tagRingItem));
    if(ret &&(item->magic == CN_ITEM_MAGIC))
    {
        ret = true;
    }
    else
    {
        ret = false;
    }
    return ret;
}

//get the item which has the same no and in use state
static bool_t  __ItemGetByNo(u32 no,tagRingItem *item,u8 *buf)
{
    u16 offset = CN_ITEM_BASE;
    bool_t ret = true;
    while(ret)
    {
        ret = __ItemGet(offset,item);
        if(ret)
        {
            if((item->state&CN_ITEM_STATE_INUSE)&&(item->no == no))
            {
                ret = true;
                if(NULL != buf) //at the same time,cpy the message here
                {
                    offset += CN_ITEM_HDRLEN;
                    __MemReadL(offset,buf,item->msglen);
                }
                break;
            }
            else
            {
                if(item->state & CN_ITEM_STATE_TAIL)
                {
                    break;  //reach the tail
                }
                else
                {
                    offset = item->nxtptr;
                }
            }
        }
        else
        {
            break;
        }
    }
    return ret;
}
//check all the item and find the maxno which in use
static bool_t __ItemGetMaxNo(u32 *no)
{
    bool_t ret = false;
    vu32 maxno = 0;
    u16 offset;
    tagRingItem item;
    //check it one by one
    offset = CN_ITEM_BASE;
    while(__ItemGet(offset,&item))
    {
        ret = true;
        if(item.no > maxno)
        {
            maxno = item.no;
            *no = maxno;
        }
        if(item.state& CN_ITEM_STATE_TAIL)
        {
            break;
        }
        else
        {
            offset = item.nxtptr;
        }
    }
    return ret;
}

//find how many item is in use
static bool_t __ItemGetUsedNum(u32 *num)
{
    bool_t ret = false;
    vu32 numused = 0;
    u16 offset;
    tagRingItem item;
    //check it one by one
    *num = numused;
    offset = CN_ITEM_BASE;
    while(__ItemGet(offset,&item))
    {
        ret = true;
        if(item.state & CN_ITEM_STATE_INUSE)
        {
            numused++;
            *num = numused;
        }
        if(item.state& CN_ITEM_STATE_TAIL)
        {
            break;
        }
        else
        {
            offset = item.nxtptr;
        }
    }
    return ret;
}

//use this function to set the item
static void __ItemSet(u16 offset,tagRingItem *item)
{
    __MemWriteL(offset,(u8 *)item,sizeof(tagRingItem));
    return;
}
//use this function to set the item and its messages
static void __ItemSetMsg(u16 offset,tagRingItem *item,struct BlackBoxRecordPara *msg)
{
    u8 *addr;
    u16 len;
    //fill the length tab
    item->lentab.headinfolen = msg->headinfolen;
    item->lentab.ostatelen = msg->osstateinfolen;
    item->lentab.hookinfolen = msg->hookinfolen;
    item->lentab.throwinfolen = msg->throwinfolen;

    addr = (u8 *)item;
    len = sizeof(tagRingItem);
    __MemWriteL(offset,addr,len);
    offset += len;

    len = msg->headinfolen;
    addr =(u8 *)msg->headinfoaddr;
    __MemWriteL(offset,addr,len);
    offset += len;

    len = msg->osstateinfolen;
    addr =(u8 *)msg->osstateinfoaddr;
    __MemWriteL(offset,addr,len);
    offset += len;

    len = msg->hookinfolen;
    addr =(u8 *)msg->hookinfoaddr;
    __MemWriteL(offset,addr,len);
    offset += len;

    len = msg->throwinfolen;
    addr =(u8 *)msg->throwinfoaddr;
    __MemWriteL(offset,addr,len);
    offset += len;

    return;
}
//extend the item to the size to be equal or more than len
static void __ItemExtend(u16 offset,tagRingItem *item,u16 len)
{
    u16 nxtptr;
    tagRingItem itemtmp;

    nxtptr = item->nxtptr;
    while((item->size < len)&&(__ItemGet(nxtptr,&itemtmp)))
    {
        item->nxtptr = itemtmp.nxtptr;
        item->size +=itemtmp.size + CN_ITEM_HDRLEN;
        if(itemtmp.state&CN_ITEM_STATE_TAIL)
        {
            item->state |= CN_ITEM_STATE_TAIL;
            break;
        }
        else
        {
            nxtptr = item->nxtptr;
        }
    }
    if(item->state & CN_ITEM_STATE_INUSE)
    {
        item->state &= (~CN_ITEM_STATE_INUSE);
    }
    item->magic = CN_ITEM_MAGIC;
    item->no = 0;
    item->msglen = 0;
    __ItemSet(offset,item);

    return;
}
//this function used to split the item into two,reserve the len size
//make sure the item has more size than the len, and the size
//write new item to offset+CN_ITEM_HDRLEN+LEN
static void __ItemSplit(tagRingItem *item,u16 offset,u16 len)
{
    tagRingItem itemtmp;

    itemtmp.nxtptr = item->nxtptr;
    item->nxtptr = offset+CN_ITEM_HDRLEN+len;
    itemtmp.magic = CN_ITEM_MAGIC;
    item->magic = CN_ITEM_MAGIC;
    itemtmp.size = item->size - len - CN_ITEM_HDRLEN;
    item->size = len;
    itemtmp.msglen = 0;
    item->msglen = 0;
    itemtmp.no = 0;
    itemtmp.no = 0;
    itemtmp.state = 0;
    if(item->state&CN_ITEM_STATE_TAIL)
    {
        item->state &= (~CN_ITEM_STATE_TAIL);
        itemtmp.state |= CN_ITEM_STATE_TAIL;
    }
    //flag it to no use
    if(item->state & CN_ITEM_STATE_INUSE)
    {
        item->state &= (~CN_ITEM_STATE_INUSE);
    }
    __ItemSet(item->nxtptr,&itemtmp);
    __ItemSet(offset,item);
    return;
}
//use this function to add a item to the ramdisk,the parameter is safe
static bool_t __ItemAdd(struct BlackBoxRecordPara  *msg)
{
    bool_t ret = false;

    tagController ctrl;
    tagRingItem item;
    u16 msglen;
    u16 offset;
    msglen = msg->headinfolen + msg->hookinfolen + msg->osstateinfolen + msg->throwinfolen;
    if((msglen + CN_ITEM_HDRLEN)> gMemCtrl.size)
    {
        return ret;
    }
    ret = __ControllerGet(&ctrl);
    if(false == ret)//disk not format or destroyed
    {
        return ret;
    }

    offset = ctrl.nxtptr;
    ret = __ItemGet(offset,&item);
    if(ret == false)//may be destroyed
    {
        return ret;
    }
    if(item.size >= msglen)  //ok, we could use the item
    {
        //first to see if could split into two
        if(item.size > (msglen + CN_ITEM_MIN))
        {
            __ItemSplit(&item,offset,msglen);
        }
        //ok, now scale the old one
        item.no = ++ctrl.maxno;
        item.msglen = msglen;
        item.state |= CN_ITEM_STATE_INUSE;
        __ItemSetMsg(offset,&item,msg);
        ctrl.nxtptr = item.nxtptr;
        __ControllerSet(&ctrl);
        ret = true;
    }
    else
    {
        if(0 ==(item.state&CN_ITEM_STATE_TAIL))
        {
            __ItemExtend(offset,&item,msglen);
        }
        else
        {
            //erase the item for use
            item.no = 0;
            item.msglen = 0;
            __ItemSet(offset,&item);
        }
        if(item.size < msglen)  //it is tail or extend still not fit,so turn back
        {
            offset = CN_ITEM_BASE;
            __ItemGet(offset,&item);
            if(item.size < msglen)  //extend the head
            {
                __ItemExtend(offset,&item,msglen);
            }
        }
        if(item.size >=msglen)
        {
            //first to see if could split into two
            if(item.size > (msglen + CN_ITEM_MIN))
            {
                __ItemSplit(&item,offset,msglen);
            }
            //ok, now scale the old one
            item.no = ++ctrl.maxno;
            item.msglen = msglen;
            item.state |= CN_ITEM_STATE_INUSE;
            __ItemSetMsg(offset,&item,msg);
            ctrl.nxtptr = item.nxtptr;
            __ControllerSet(&ctrl);
            ret = true;
        }
    }
    return ret;
}
//USE THIS FUNCTION TO FORMAT THE CONTROLLER AND  ITEM RAM SPACE
static void __LineMemDiskFormat(void)
{
    tagRingItem item;
    tagController ctrl;

    //first initialize the ram disk
    __MemFormatL(CN_CONTROLLER_BASE,gMemCtrl.size);
    //write the ctroller and format the ramdisk to the map
    memset(&ctrl,0,sizeof(ctrl));
    ctrl.magic = CN_CONTROLLER_MAGIC;
    ctrl.nxtptr = CN_ITEM_BASE;
    ctrl.maxno = 0;
    __ControllerSet(&ctrl);

    item.magic = CN_ITEM_MAGIC;
    item.no = 0;
    item.state = CN_ITEM_STATE_HEAD|CN_ITEM_STATE_TAIL;
    item.size = gMemCtrl.size - CN_ITEM_HDRLEN - CN_ITEM_BASE;
    item.msglen = 0;
    item.nxtptr = CN_ITEM_BASE;
    __ItemSet(CN_ITEM_BASE,&item);  //write it to the ramdisk
    return;
}
//////////////////////////EXCEPTION RECORD METHOD INTERFACE/////////////////////
// =============================================================================
// 函数功能：保存一帧异常信息到非易失存储器。存储或者输出，或者两者兼顾，由异常存储方案设计者决定
// 输入参数：recordpara,一帧异常信息
// 输出参数：
// 返回值  ：见enum EN_BlackBoxDealResult
// =============================================================================
static enum EN_BlackBoxDealResult __RecordSave(struct BlackBoxRecordPara  *recordpara)
{
    enum EN_BlackBoxDealResult ret;
    //we need a atom to protect the process
    atom_low_t  atom;
    atom = Int_LowAtomStart();
    if(__ItemAdd(recordpara))
    {
        ret = EN_BLACKBOX_RESULT_SUCCESS;
    }
    else
    {
        ret = EN_BLACKBOX_RESULT_RECORD_NOSPACE;
    }
    Int_LowAtomEnd(atom);

    return ret;
}
// =============================================================================
// 函数功能：清除所有的异常信息，清除异常信息存储区域
// 输入参数：无
// 输出参数：无
// 返回值  ：TRUE 成功， FALSE失败
// =============================================================================
static bool_t __RecordClean(void)
{
    atom_low_t  atom;
    atom = Int_LowAtomStart();
    __LineMemDiskFormat();
    Int_LowAtomEnd(atom);
    return true;
}
// =============================================================================
// 函数功能：查看一共存储了多少条异常信息
// 输入参数：
// 输出参数：recordnum,存储的异常信息条目数
// 返回值  ：FALSE,失败 TRUE成功
// =============================================================================
static bool_t __RecordCheckNum(u32 *recordnum)
{
    bool_t ret = false;
    atom_low_t  atom;

    if(NULL != recordnum)
    {
        atom = Int_LowAtomStart();
        ret =__ItemGetUsedNum(recordnum);
        Int_LowAtomEnd(atom);
    }
    return ret;
}
// =============================================================================
// 函数功能：查看指定异常条目的长度
// 输入参数：assignedno,指定的条目
// 输出参数：recordlen,该条目的长度
// 返回值  ：FALSE,失败 TRUE成功
// =============================================================================
static bool_t __RecordCheckLen(u32 assignedno, u32 *recordlen)
{
    u32 no;
    u32 nomax = 0;
    u32 numused = 0;
    tagRingItem item;
    atom_low_t  atom;
    bool_t ret = false;

    if((NULL != recordlen)&&(assignedno > 0))
    {
        atom = Int_LowAtomStart();
        if(__ItemGetMaxNo(&nomax)&&__ItemGetUsedNum(&numused))
        {
            if((assignedno <= numused)&&(nomax > 0))
            {
                no = nomax - numused +assignedno;
                ret = __ItemGetByNo(no,&item,NULL);
            }
        }
        Int_LowAtomEnd(atom);
        if(ret)
        {
            *recordlen = item.msglen;
        }
    }
    return ret;
}

// =============================================================================
// 函数功能：从存储介质中获取指定条目的异常帧信息
// 输入参数：assignedno,指定的异常帧条目
//        buflen,缓冲区长度
// 输出参数：buf,用于存储获取指定条目的异常信息
//        recordpara,异常信息存储时的参数，在此是对buf的各个部分的定义
// 返回值  ：FALSE,失败 TRUE成功
// =============================================================================
static bool_t __RecordGet(u32 assignedno,u32 buflen,u8 *buf,\
                            struct BlackBoxRecordPara *recordpara)
{
    //check it one by one
    u32 no;
    u32 nomax = 0;
    u32 numused = 0;
    tagRingItem item;
    atom_low_t  atom;
    bool_t ret = false;

    if((assignedno > 0)&&(buflen > 0)&&(NULL != buf)&&(NULL != recordpara))
    {
        atom = Int_LowAtomStart();
        if(__ItemGetMaxNo(&nomax)&&__ItemGetUsedNum(&numused))
        {
            if((assignedno <= numused)&&(nomax > 0))
            {
                no = nomax - numused +assignedno;
                ret = __ItemGetByNo(no,&item,buf);
            }
        }
        Int_LowAtomEnd(atom);
        if(ret)
        {
            recordpara->headinfolen = item.lentab.headinfolen;
            recordpara->headinfoaddr =item.lentab.headinfolen?((ptu32_t)buf):(ptu32_t)NULL;
            buf += item.lentab.headinfolen;

            recordpara->osstateinfolen = item.lentab.ostatelen;
            recordpara->osstateinfoaddr =item.lentab.ostatelen?((ptu32_t)buf):(ptu32_t)NULL;
            buf += item.lentab.ostatelen;

            recordpara->hookinfolen = item.lentab.hookinfolen;
            recordpara->hookinfoaddr =item.lentab.hookinfolen?((ptu32_t)buf):(ptu32_t)NULL;
            buf += item.lentab.hookinfolen;

            recordpara->throwinfolen = item.lentab.throwinfolen;
            recordpara->throwinfoaddr =item.lentab.throwinfolen?((ptu32_t)buf):(ptu32_t)NULL;
            buf += item.lentab.throwinfolen;
        }
    }
    return ret;
}

// =============================================================================
// 函数功能：开机的时候扫描异常存储记录，获取关键信息方便以后存储
// 输入参数：无
// 输出参数：无
// 返回值  ：空
// =============================================================================
static void __RecordScan(void)
{
    bool_t ret ;
    tagController ctrl;
    tagRingItem item;
    ret = __ControllerGet(&ctrl);
    //check it one by one
    if(ret == false)
    {
        //format the ramdisk
        debug_printf("null","Controller Not Format,will do the format!\n\r");
        __LineMemDiskFormat();
    }
    else
    {
        ret = __ItemGet(ctrl.nxtptr,&item);
        if(ret == false)
        {
            debug_printf("null","Item Get Failed,will do the format!\n\r");
            __LineMemDiskFormat();
        }
    }
    return;
}
///////////////LINE MEMORY DEBUG METHOD////////////////////////////////////////
static void __LineMemCtrlShow(void)
{
    tagController ctrl;
    if(__ControllerGet(&ctrl))
    {
        debug_printf("null","Ctroller:Magic:0x%08x Nxtptr:0x%04x MaxNo:0x%08x\n\r",\
                ctrl.magic,ctrl.nxtptr,ctrl.maxno);
    }
    else
    {
            debug_printf("null","Ctroller Not Fomat Yet!\n\r");
    }
}
//use this function to show the item
static void __LineMemItemShow()
{
    u8 i =1;
    u16 offset;
    tagRingItem item;

        debug_printf("null","%-4s %-4s %-4s %-4s %-4s %-4s %-4s %-4s %-8s\n\r",
                "No","Off","NPTR","Size","Msgl","Head","Tail","Inuse","Seq");
    offset = CN_ITEM_BASE;
    while(__ItemGet(offset,&item))
    {
        debug_printf("null","%-4d %-4x %-4x %-4d %-4d %-4s %-4s %-4s %-8x\n\r",\
                i++,offset,item.nxtptr,item.size,item.msglen,\
                item.state&CN_ITEM_STATE_HEAD?"YES":"NO",\
                        item.state&CN_ITEM_STATE_TAIL?"YES":"NO",\
                                item.state&CN_ITEM_STATE_INUSE?"YES":"NO",\
                                        item.no);
        if(item.state&CN_ITEM_STATE_TAIL)
        {
            break;
        }
        else
        {
            offset = item.nxtptr;
        }
    }
    return;
}
//show the meminfo in line memory
static void __LineMemByteShow(void)
{
    //show the bkpsram
    u32 i =0;
    u8 *buf;
    u32 len = gMemCtrl.size;
    buf = malloc(len);
    if(NULL != buf)
    {
        __MemReadL(CN_CONTROLLER_BASE,buf,len);
        for(i =0;i < len;i+= 16)
        {
            debug_printf("null","%04x:%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n\r",
                    i,buf[i+0],buf[i+1],buf[i+2],buf[i+3],buf[i+4],buf[i+5],buf[i+6],buf[i+7],buf[i+8],buf[i+9],buf[i+10],buf[i+11],buf[i+12],buf[i+13],buf[i+14],buf[i+15]);
        }
        free(buf);
    }
}
//this is the line memory debug command
//static bool_t __LineMemShell(char *param)

bool_t linemem(char *param)
{
    if(NULL != param)
    {
        if(0 == strcmp(param,"byte"))
        {
            __LineMemByteShow();
        }
        else if(0 == strcmp(param,"item"))
        {
            __LineMemCtrlShow();
            __LineMemItemShow();
        }
        else if(0 == strcmp(param,"state"))
        {
            __LineMemCtrlShow();
        }
    }
    else
    {
        __LineMemCtrlShow();
    }
    return true;
}


//static struct ShellCmdRsc gLineMemDebugCmdRsc[CN_LineMemDebug_NUM];

// =============================================================================
// 函数功能:Line Mem for the Exception Record Configure
// 输入参数：lopt,the line memory lowlevel interface ,NEVER BE NULL AND ITS MEMBER
//        memsize, the line memory size,must be more than CN_LINEMEM_MINI(better more than 128 bytes)
//        debug, if true we will do the record test
// 输出参数：
// 返回值  ：
// =============================================================================
bool_t BlackBox_NvramRecordRegister(tagBlackBoxLowLevelOpt *lopt,u16 memsize)
{
    bool_t ret = false;
    struct BlackBoxRecordOperate BlackBoxOpt;
    //check the parameter is ok
    if((NULL == lopt)||(NULL == lopt->read)||(NULL == lopt->write)||\
       (NULL== lopt->format)||(NULL == lopt->init)||(memsize < CN_LINEMEM_MINI))
    {
        debug_printf("null","%s:make sure lopt and its member NOT NULL, and  memory size more than CN_LINEMEM_MINI\n\r",\
                __FUNCTION__);
        return ret;
    }
    gMemCtrl.lopt = *lopt;
    gMemCtrl.size = memsize;
    //first we need to do the hard memory initialize here
    ret = __MemInitL();
    if(false == ret)
    {
        debug_printf("null","%s:memory hard initialize failed\n\r",__FUNCTION__);
        return ret;
    }
    //then register the line memory record method to the exception module
    BlackBoxOpt.fnBlackBoxRecordScan      = __RecordScan;
    BlackBoxOpt.fnBlackBoxRecordSave      = __RecordSave;
    BlackBoxOpt.fnBlackBoxRecordClean     = __RecordClean;
    BlackBoxOpt.fnBlackBoxRecordGet       = __RecordGet;
    BlackBoxOpt.fnBlackBoxRecordCheckNum  = __RecordCheckNum;
    BlackBoxOpt.fnBlackBoxRecordCheckLen  = __RecordCheckLen;
    ret = BlackBox_RegisterRecorder(&BlackBoxOpt);
    if(false == ret)
    {
        debug_printf("null","%s:register recorder failed\r\n",__FUNCTION__);
        return ret;
    }
    return ret;
}
// =============================================================================
// 函数功能:Line Mem for the Exception Record Configure test(ONLY USED IN DEVELOPMENT TEST)
// 输入参数：lopt,the line memory lowlevel interface ,NEVER BE NULL AND ITS MEMBER
//        memsize, the line memory size,must be more than CN_LINEMEM_MINI(better more than 128 bytes)
//        autest,if true do the auto test
//        maxlen,the max record item could generate
//        debugmsg,if true out put the test messages details,else only output the process
// 输出参数：
// 返回值  ：true configure success while false failed
// =============================================================================
bool_t BlackBox_NvramRecordRegisterTest(tagBlackBoxLowLevelOpt *lopt,u16 memsize,u32 maxlen,bool_t autotest,bool_t debugmsg)
{

    bool_t ret = false;
    struct BlackBoxRecordOperate BlackBoxOpt;
    //check the parameter is ok
    if((NULL == lopt)||(NULL == lopt->read)||(NULL == lopt->write)||\
       (NULL== lopt->format)||(NULL == lopt->init)||(memsize < CN_LINEMEM_MINI))
    {
        debug_printf("null","%s:make sure lopt and its member NOT NULL, and  memory size more than CN_LINEMEM_MINI\n\r",\
                __FUNCTION__);
        return ret;
    }
    gMemCtrl.lopt = *lopt;
    gMemCtrl.size = memsize;
    //first we need to do the hard memory initialize here
    ret = __MemInitL();
    if(false == ret)
    {
        debug_printf("null","%s:memory hard initialize failed\n\r",__FUNCTION__);
        return ret;
    }
    //then register the line memory record method to the exception module
    BlackBoxOpt.fnBlackBoxRecordScan      = __RecordScan;
    BlackBoxOpt.fnBlackBoxRecordSave      = __RecordSave;
    BlackBoxOpt.fnBlackBoxRecordClean     = __RecordClean;
    BlackBoxOpt.fnBlackBoxRecordGet       = __RecordGet;
    BlackBoxOpt.fnBlackBoxRecordCheckNum  = __RecordCheckNum;
    BlackBoxOpt.fnBlackBoxRecordCheckLen  = __RecordCheckLen;
    ret = ModuleInstall_BlackBoxRecordTest(&BlackBoxOpt,maxlen,autotest,debugmsg);
    if(false == ret)
    {
        debug_printf("null","%s:register recorder failed\r\n",__FUNCTION__);
        return ret;
    }

    return ret;
}
ADD_TO_ROUTINE_SHELL(linemem,linemem,"usage:linemem [item/state/byte](default state)");

