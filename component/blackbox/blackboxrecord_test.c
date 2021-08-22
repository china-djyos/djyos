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
//------------------------------------------------------------------------------
// =============================================================================

// 文件名     ：异常存储测试机
// 模块描述: 用于测试开发的异常存储模块
// 模块版本: V1.00
// 创建人员: zhangqf
// 创建时间: 12/03.2016
// =============================================================================
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>   //used for the time to set the random seed

#include <os.h>
#include <shell.h>
#include <blackbox.h>
#include "dbug.h"

#define CN_EXCEPTIONTEST_VERSION   "V1.0.0"
//param,show how many times we should do
#define CN_TEST_LENMAX   327   //A NUMBER NOT SO COMMON
#define CN_LEN_RECORDTAB 100

//used for the user to register the recorder
typedef struct
{
    struct BlackBoxRecordOperate opt;
    u32 maxlen;      //could never more than the maxlen to read or to write
    bool_t autotest; //if true then make a test task
    bool_t debugmsg; //if we will output the test details
    u8 *buf2snd;     //the buffer used to send the message
    u8 *buf2rcv;     //the buffer used to receive the message
    u16 evttid ;     //the auto task evttID
    u16 eventid;     //the auto task eventID
    u32 lentab[CN_LEN_RECORDTAB];
}tagBlackBoxRecordTestConfig;
static tagBlackBoxRecordTestConfig gTestConfig;

//SOME MESSAGES TO SHOW
static void __WelcomMsgShow(void)
{
    //output the test welcome messages;if you use it,it's my honor!
    debug_printf("null","****************EXCEPTIONRECORDERTESTVERSION******************\n\r");
    debug_printf("null","WELCOME TO EXCEPTION RECORDER TEST\n\r");
    debug_printf("null","VERSION:%s BUILDTIME:%s:%s\n\r",CN_EXCEPTIONTEST_VERSION,__DATE__,__TIME__);
    debug_printf("null","AUTHOR :%s:%s\n\r","luost","lst@djyos.com");
    debug_printf("null","*************************************************************\n\r");
    return;
}
//show the configure messages
static void __RecorderConfigShow()
{
    debug_printf("null","****************EXCEPTIONRECORDERTESTCONFIG******************\n\r");
    debug_printf("null","FUNCTION:SCAN    :0X%08X\n\r",(u32)gTestConfig.opt.fnBlackBoxRecordScan);
    debug_printf("null","FUNCTION:SAVE    :0X%08X\n\r",(u32)gTestConfig.opt.fnBlackBoxRecordSave);
    debug_printf("null","FUNCTION:GET     :0X%08X\n\r",(u32)gTestConfig.opt.fnBlackBoxRecordGet);
    debug_printf("null","FUNCTION:CLEAN   :0X%08X\n\r",(u32)gTestConfig.opt.fnBlackBoxRecordClean);
    debug_printf("null","FUNCTION:CHECKNUM:0X%08X\n\r",(u32)gTestConfig.opt.fnBlackBoxRecordCheckNum);
    debug_printf("null","FUNCTION:CHECKLEN:0X%08X\n\r",(u32)gTestConfig.opt.fnBlackBoxRecordCheckLen);
    debug_printf("null","DEBUGMSG:%s\n\r",gTestConfig.debugmsg?"DEBUGON":"DEBUGOFF");
    debug_printf("null","TESTMODE:%s\n\r",gTestConfig.autotest?"AUTOTEST":"SHELLMANUAL");
    debug_printf("null","MAXLEN  :%d\n\r",gTestConfig.maxlen);
    //show the member we used
    debug_printf("null","RCVBUF  :0X%08X\n\r",(u32)gTestConfig.buf2rcv);
    debug_printf("null","SNDBUF  :0X%08X\n\r",(u32)gTestConfig.buf2snd);
    debug_printf("null","*************************************************************\n\r");
    return;
}
static void __TestTasksShow(u32 testtimes,u32 maxlen)
{
    debug_printf("null","****************EXCEPTIONRECORDERTESTTASK********************\n\r");
    debug_printf("null","TESTTASK:TIMES:%d MSGLEN:%d(0 for random)\n\r",testtimes,maxlen);
    debug_printf("null","*************************************************************\n\r");
    return;
}
static void __TestResultShow(bool_t ret)
{
    debug_printf("null","*************************************************************\n\r");
    debug_printf("null","***********************%s*****************************\n\r",\
            ret?"SUCCESS":"FAILURE");
    debug_printf("null","*************************************************************\n\r");
    return;
}
static void __CounterLeftShow(const char *msg,s32 counter)
{
    s32 i;
    printf("\r\n");
    for(i = counter;i>0;i--)
    {
        debug_printf("null","%s:%04d(seconds)",msg,i);
        DJY_EventDelay(1000*mS);
    }
    printf("\r\n");
    return;
}



//here will random to throw a message to the recorder
//headinfole:len/4 ostatelen:len/4 hookinfolen:len/4 throwinfo:left
//make sure the len is less than the maxlen
static bool_t __MsgSave(u32 len)
{
    bool_t ret = false;
    u8 *buf = NULL;
    u32 offset = 0;
    struct BlackBoxRecordPara para;
    buf = gTestConfig.buf2snd;
    if((NULL != buf)&&(len <= gTestConfig.maxlen))
    {
        offset = 0;
        para.headinfolen = len/4;
        para.headinfoaddr = (ptu32_t)buf+offset;
        offset += len/4;
        para.osstateinfolen = len/4;
        para.osstateinfoaddr = (ptu32_t)buf+offset;
        offset += len/4;
        para.hookinfolen = len/4;
        para.hookinfoaddr = (ptu32_t)buf +offset;
        offset += len/4;
        para.throwinfolen = len - offset;
        para.throwinfoaddr = (ptu32_t)buf+offset;
        if(NULL != gTestConfig.opt.fnBlackBoxRecordSave)
        {
            if(EN_BLACKBOX_RESULT_SUCCESS == gTestConfig.opt.fnBlackBoxRecordSave(&para))
            {
                ret = true;
            }
        }
    }
    return ret;
}
//use this function to get how many messages in the recorder
static bool_t __MsgNum(u32 *num)
{
    bool_t ret = false;
    if(NULL != gTestConfig.opt.fnBlackBoxRecordCheckNum)
    {
        ret = gTestConfig.opt.fnBlackBoxRecordCheckNum(num);
    }
    return ret;
}
//use this function to get the specified message length in the recorder
//lencmp:if >0 do the compare else no compare
static bool_t __MsgLen(u32 no,u32 *len)
{
    bool_t ret = false;
    if(NULL != gTestConfig.opt.fnBlackBoxRecordCheckLen)
    {
        ret = gTestConfig.opt.fnBlackBoxRecordCheckLen(no,len);
    }
    return ret;
}

//use this function to clean all the message in the recorder
static bool_t __MsgClean(void)
{
    bool_t ret = false;
    if(NULL != gTestConfig.opt.fnBlackBoxRecordClean)
    {
        gTestConfig.opt.fnBlackBoxRecordClean();
        ret = true;
    }
    return ret;
}
//use this function to scan all the message in the recorder
static bool_t __MsgScan(void)
{
    bool_t ret = false;
    if(NULL != gTestConfig.opt.fnBlackBoxRecordScan)
    {
        gTestConfig.opt.fnBlackBoxRecordScan();
        ret = true;
    }
    return ret;
}
//use this function to get the specified message in the recorder
//make sure the len is less than the maxlen
//check the length  and check the message content
static bool_t __MsgGet(u32 no,u32 msglencmp)
{
    bool_t ret = false;
    u8 *rbuf = NULL;
    u8 *wbuf = NULL;
    u32 len;
    u32 offset = 0;
    u32 addr;
    u32 msglen;
    struct BlackBoxRecordPara para;
    memset(&para,0,sizeof(para));

    len = msglencmp;
    rbuf = gTestConfig.buf2rcv;
    wbuf = gTestConfig.buf2snd;
    if((rbuf == NULL)||(wbuf == NULL)||(len > gTestConfig.maxlen)||(NULL == gTestConfig.opt.fnBlackBoxRecordGet))
    {
        return ret;
    }
    ret = gTestConfig.opt.fnBlackBoxRecordGet(no,len,rbuf,&para);
    if(ret)
    {
        ret = false;
        //check the message length
        msglen = para.headinfolen +para.osstateinfolen +para.hookinfolen+para.throwinfolen;
        if(msglen != msglencmp)
        {
            return ret;
        }
        //check the info addr and length
        offset = 0;

        addr = (ptu32_t)rbuf + offset;  //headinfo
        if(para.headinfolen != msglencmp/4)
        {
            return ret;  //length not equal
        }
        if((para.headinfolen >0)&&(addr != para.headinfoaddr))
        {
            return ret;  //addr not equal
        }
        offset = offset+ msglen/4;

        addr = (ptu32_t)rbuf + offset;  //osstateinfo
        if(para.osstateinfolen != msglencmp/4)
        {
            return ret;  //length not equal
        }
        if((para.osstateinfolen >0)&&(addr != para.osstateinfoaddr))
        {
            return ret;  //addr not equal
        }
        offset = offset+ msglen/4;

        addr = (ptu32_t)rbuf + offset; //hookinfo
        if(para.hookinfolen != msglencmp/4)
        {
            return ret;  //length not equal
        }
        if((para.hookinfolen >0)&&(addr != para.hookinfoaddr))
        {
            return ret;  //addr not equal
        }
        offset = offset+ msglen/4;

        addr = (ptu32_t)rbuf + offset; //throw
        if(para.throwinfolen != (msglencmp-offset))
        {
            return ret;  //length not equal
        }
        if((para.throwinfolen >0)&&(addr != para.throwinfoaddr))
        {
            return ret;  //addr not equal
        }
        //check the message content
        if(0 != memcmp(rbuf,wbuf,msglen))
        {
            return ret;  //content not equal
        }
        //all has passed
        ret = true;
    }
    return ret;
}

//if len is 0,then use the rand function to generate each time
static bool_t __RecordTestMain(u32 timestotal,u32 msglen)
{
    bool_t ret =false;
    u32 len;
    u32 times = 0;
    u32 savetimes = 0;

    debug_printf("null","Step1:TEST THE SAVE FUNCTION START\n\r");
    ret = true;
    times = 0;
    while((times < timestotal)&&(ret))
    {
        if(msglen == 0)
        {
            len = (u32)rand()%gTestConfig.maxlen;
            if(len == 0)  //len could not be zero
            {
                len =gTestConfig.maxlen;
            }
        }
        else
        {
            len = msglen;
        }
        //record the length for the other test
        gTestConfig.lentab[times%CN_LEN_RECORDTAB] = len;
        ret = __MsgSave(len);
        if(gTestConfig.debugmsg)
        {
            if(ret == false)
            {
                debug_printf("null","SAVE:%08d/%08d LEN:%08d ERR\n\r",times+1,timestotal,len);
            }
            else
            {
                debug_printf("null","SAVE:%08d/%08d LEN:%08d OK",times+1,timestotal,len);
            }
        }

        times++;
    }
    savetimes = times;

    if(times  == timestotal)
    {
        debug_printf("null","Step1:SAVE FUNCTION WORKS OK\n\r");
    }
    else
    {
        debug_printf("null","Step1:SAVE FUNCTION WORKS ERR,EXIT\n\r");
        return ret;
    }

    debug_printf("null","Step2:TEST THE CHECKNUM FUNCTION START\n\r");
    u32 msgnum;
    ret = __MsgNum(&msgnum);
    if(ret)
    {
        if(gTestConfig.debugmsg)
        {
            debug_printf("null","Step2:MSGNUM:%d\n\r",msgnum);
        }
        debug_printf("null","Step2:CHECKNUM FUNCTION WORKS OK\n\r");
    }
    else
    {
        debug_printf("null","Step2:CHECKNUM FUNCTION WORKS ERR,EXIT\n\r");
        return ret;
    }

    debug_printf("null","Step3:TEST  CHECKLEN FUNCTION START\n\r");
    u32 checktimes = 0;
    u32 offset; //which measns the last one in the  length tab here
    //checktimes should be the min between(savetimes msgnum CN_LEN)
    checktimes = savetimes>msgnum?msgnum:savetimes;
    checktimes = checktimes > CN_LEN_RECORDTAB?CN_LEN_RECORDTAB:checktimes;
    offset =(savetimes-checktimes)%CN_LEN_RECORDTAB;
    ret = true;
    times =0;
    u32 msgnumoffset = 0;
    msgnumoffset = checktimes < msgnum?(msgnum - checktimes +1):1;
    //if the msgnum is more than the check times, this means that we should
    //check the last record

    while((times< checktimes)&&(ret))
    {
        u32 lencmp;
        lencmp = gTestConfig.lentab[offset];
        ret = __MsgLen(times+msgnumoffset,&len);
        if(ret&&(lencmp == len))
        {
            ret = true;
        }
        else
        {
            ret = false;
        }
        if(gTestConfig.debugmsg)
        {
            if(ret) //compare with the length tab here
            {
                debug_printf("null","LEN:%04d/%04d LEN:%04d OK",times+1,checktimes,len);
            }
            else
            {
                debug_printf("null","LEN:%04d/%04d LEN:%04d ORILEN:%04d ERR\n\r",times+1,checktimes,len, gTestConfig.lentab[offset]);
            }
        }
        offset++;
        times++;
        offset = offset %CN_LEN_RECORDTAB;
    }
    if(ret)
    {
        debug_printf("null","Step3:CHECKLEN FUNCTION WORKS OK\n\r");
    }
    else
    {
        debug_printf("null","Step3:CHECKLEN FUNCTION WORKS ERR,EXIT\n\r");
        return ret;
    }

    //check the msgget function
    debug_printf("null","Step4:TEST  CHECKLEN FUNCTION START\n\r");
    checktimes = savetimes>msgnum?msgnum:savetimes;
    checktimes = checktimes > CN_LEN_RECORDTAB?CN_LEN_RECORDTAB:checktimes;
    offset =(savetimes-checktimes)%CN_LEN_RECORDTAB;
    ret = true;
    times =0;
    while((times<checktimes)&&(ret))
    {
        len = gTestConfig.lentab[offset];
        ret = __MsgGet(times+msgnumoffset,len);
        if(ret)
        {
            debug_printf("null","GET:%d/%d LEN:%d OK",times+1,checktimes,len);
        }
        else
        {
            debug_printf("null","GET:%d/%d LEN:%d ERR\n\r",times+1,checktimes,len);
        }
        offset++;
        times++;
        offset = offset %CN_LEN_RECORDTAB;
    }
    if(ret)
    {
        debug_printf("null","Step4:GET FUNCTION WORKS OK\n\r");
    }
    else
    {
        debug_printf("null","Step4:GET FUNCTION WORKS ERR,EXIT\n\r");
        return ret;
    }

    debug_printf("null","RECORD TEST PASS!\n\r");
    return ret;
}


static bool_t __RecordTest(u32 timestotal,u32 msglen)
{
    bool_t ret = false;
    __WelcomMsgShow();
    __RecorderConfigShow();
    __TestTasksShow(timestotal,msglen);
    if(timestotal ==0)
    {
        debug_printf("null","NO TEST NEED TO DO\n\r");
    }
    else
    {
        ret = __RecordTestMain(timestotal,msglen);
        __TestResultShow(ret);
    }
    return ret;
}

//you also could use this function to make a task to do the auto test
ptu32_t __RecordTestTask(void)
{
    //do the bkpsram exception record test
    __CounterLeftShow("TEST WILL START",10);
    u32 times;
    time_t time_now;
    time_now = time(NULL);
    srand((u32)time_now);
    times = (u32)rand()%100000;
    if(times == 0 )
    {
        times =9997;
    }
    if(__RecordTest(times,0))
    {
        __CounterLeftShow("SYSTEM WILL RESET FOR ANOTHER TEST",10);
        debug_printf("null","RESET THE SYSTEM\n\r");
        CPU_Reset();
    }
    else
    {
        debug_printf("null","THE TEST TASK WILL QUIT FOR FAILURE\n\r");
    }

    return 0;
}



// =============================================================================
// 函数功能:板件异常记录存储测试机，开发的异常存储方案可以使用该函数进行初步的测试，看是否能够满足存储需求
// 输入参数：opt，异常存储操作接口
//        maxlen,单次写入的最大长度，即单条异常信息的最大长度(headinfo oststateinfo hookinfo throwinfo之和)
//               if 0 we will use the default CN_TEST_LENMAX
//        autotest,true表示将启动一个任务自动测试，否则需要在shell中手动使用相关命令进行测试
//        debugmsg,true表示输出测试过程中的详细信息（调试信息比较多），否则只输出测试进度和测试结果
// 输出参数：无
// 返回值  ：true 注册成功，false 注册失败
// 说明：注册的过程中会调用用户的注册的SCAN函数进行扫描;测试过程中会有很多内存的分配和释放操作，务必保证内存够用，内存分配机制完善
// =============================================================================
bool_t ModuleInstall_BlackBoxRecordTest(struct BlackBoxRecordOperate *opt,u32 maxlen,bool_t autotest,bool_t debugmsg)
{
    bool_t ret = false;
    //check the parameter and determine the task we will do
    //check the operation
    if(NULL == opt)
    {
        debug_printf("null","CHECK:ERR :OPT NULL,WILL EXIT\n\r");
        return ret;
    }
    if(NULL == opt->fnBlackBoxRecordScan)
    {
        debug_printf("null","CHECK:WARN:SCAN FUNCTION NULL,BUT CONTINUE\n\r");
    }
    else
    {
        opt->fnBlackBoxRecordScan();
    }
    if(NULL == opt->fnBlackBoxRecordSave)
    {
        debug_printf("null","CHECK:ERR :MSGSAVE FUNCTION NULL,WILL EXITE\n\r");
        return ret;
    }
    if(NULL == opt->fnBlackBoxRecordGet)
    {
        debug_printf("null","CHECK:ERR :MSGGET FUNCTION NULL,WILL EXITE\n\r");
        return ret;
    }
    if(NULL == opt->fnBlackBoxRecordCheckNum)
    {
        debug_printf("null","CHECK:ERR :MSGCHECKNUM FUNCTION NULL,WILL EXITE\n\r");
        return ret;
    }
    if(NULL == opt->fnBlackBoxRecordCheckLen)
    {
        debug_printf("null","CHECK:ERR :MSGCHECKLEN FUNCTION NULL,WILL EXITE\n\r");
        return ret;
    }
    if(NULL == opt->fnBlackBoxRecordClean)
    {
        debug_printf("null","CHECK:WARN:MSGCLEAN FUNCTION NULL,BUT CONTINUE\n\r");
    }
    gTestConfig.opt = *opt;
    if(0 == maxlen)
    {
        gTestConfig.maxlen = CN_TEST_LENMAX;
    }
    else
    {
        gTestConfig.maxlen = maxlen;
    }
    gTestConfig.autotest = autotest;
    gTestConfig.debugmsg = debugmsg;
    //malloc the memory from the system
    gTestConfig.buf2rcv = malloc(gTestConfig.maxlen);
    if(NULL == gTestConfig.buf2rcv)
    {
        debug_printf("null","ERR:RCVBUF MEM MALLOC ERR\n\r");
        return ret;
    }
    gTestConfig.buf2snd = malloc(gTestConfig.maxlen);
    if(NULL == gTestConfig.buf2snd)
    {
        debug_printf("null","ERR:SNDBUF MEM MALLOC ERR\n\r");
        return ret;
    }
    //here we'd better to initialize the send and receive buffer with random data
    u32 i =0;
    u8 data;
    for(i =0;i<gTestConfig.maxlen;i++)
    {
        data = (u8)(u32)rand();
        gTestConfig.buf2snd[i]=data;

    }
    for(i =0;i<gTestConfig.maxlen;i++)
    {
        data = (u8)(u32)rand();
        gTestConfig.buf2rcv[i]=data;
    }

    //install the shell command here
    ret = true;

    //if auto test,we will make a task here to do the test
    if(gTestConfig.autotest)
    {
        u16 evttID;
        evttID = DJY_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 0, (ptu32_t(*)(void))__RecordTestTask,\
                                                NULL, 0x1000, "BLACKBOXRECORDERTEST");
        if(evttID==CN_EVTT_ID_INVALID)
        {
            ret = false;
        }
        else
        {
            DJY_EventPop(evttID, NULL, 0, 0, 0, 0);
        }
    }
    return ret;
}

//you could use this function to add one messages -t times -l maxlenth
//static bool_t __RecordTestShell(char *param)
bool_t blackboxrtest(char *param)
{
    int argc = 8;
    char *argv[8];

    u32 len = CN_TEST_LENMAX;
    if(NULL!= param)
    {
        string2arg(&argc,argv,param);
        if(argc > 0) //argv[0]:subcmd
        {
            if((0 == strcmp(argv[0],"test"))&&(argc == 3)) //arg[1]:times argv[2]:length
            {

                u32 totaltimes = 1;
                totaltimes = strtol(argv[1],NULL,0);
                len = strtol(argv[2],NULL,0);
                if((totaltimes != 0)&&(len <= gTestConfig.maxlen))
                {
                    __RecordTest(totaltimes,len);
                }
                else
                {
                    debug_printf("null","paras:test testtimes(>0) msglen(<=maxlen)\n\r");
                }
            }
            else if((0 == strcmp(argv[0],"func"))&&(argc >= 2))//argv[1]:scan/clean/save/get/checknum/checklen
            {
                u32 number;
                u32 len;
                if((0 == strcmp(argv[1],"scan"))&&(argc == 2))
                {
                    if(__MsgScan())
                    {
                        debug_printf("null","Scan:OK\n\r");
                    }
                    else
                    {
                        debug_printf("null","Scan:ERR\n\r");
                    }
                }
                else if((0 == strcmp(argv[1],"clean"))&&(argc == 2))
                {
                    if(__MsgClean())
                    {
                        debug_printf("null","Clean:OK\n\r");
                    }
                    else
                    {
                        debug_printf("null","Clean:ERR\n\r");
                    }
                }
                else if((0 == strcmp(argv[1],"clean"))&&(argc == 2))
                {
                    if(__MsgClean())
                    {
                        debug_printf("null","Clean:OK\n\r");
                    }
                    else
                    {
                        debug_printf("null","Clean:ERR\n\r");
                    }
                }
                else if((0 == strcmp(argv[1],"num"))&&(argc == 2))
                {
                    if(__MsgNum(&number))
                    {
                        debug_printf("null","Num:%d OK\n\r",number);
                    }
                    else
                    {
                        debug_printf("null","Num:ERR\n\r");
                    }
                }
                else if((0 == strcmp(argv[1],"save"))&&(argc == 3))
                {
                    len = (u32)strtol(argv[2],NULL,0);
                    if(__MsgSave(len))
                    {
                        debug_printf("null","Save:%d OK\n\r",len);
                    }
                    else
                    {
                        debug_printf("null","Save:%d ERR\n\r",len);
                    }
                }
                else if((0 == strcmp(argv[1],"get"))&&(argc == 4))
                {
                    number = (u32)strtol(argv[2],NULL,0);
                    len = (u32)strtol(argv[3],NULL,0);
                    if(__MsgGet(number,len))
                    {
                        debug_printf("null","Get:%d Len:%d OK\n\r",number,len);
                    }
                    else
                    {
                        debug_printf("null","Get:%d Len:%d ERR\n\r",number,len);
                    }
                }
                else if((0 == strcmp(argv[1],"len"))&&(argc == 3))
                {
                    number = (u32)strtol(argv[2],NULL,0);
                    if(__MsgLen(number,&len))
                    {
                        debug_printf("null","Len:%d:%d OK\n\r",number,len);
                    }
                    else
                    {
                        debug_printf("null","Len:%d ERR\n\r",number);
                    }
                }
                else
                {
                    debug_printf("null","paras:subcmd:scan/clean/num/(get+number+len)/(save+len)/(len+number)\n\r");
                }
            }
            else
            {
                debug_printf("null","paras:test/func subcmdpara\n\r");
            }
        }
        else
        {
            debug_printf("null","paras:subcmd(test/func)+subcmdpara\n\r");
        }
    }
    else  //show the config
    {
        __RecorderConfigShow();
        debug_printf("null","paras:subcmd(test/func)+subcmdpara\n\r");
    }

    return true;
}
ADD_TO_ROUTINE_SHELL(blackboxrtest,blackboxrtest,"usage:blackboxrtest [test/func] [subcmdparas]");

