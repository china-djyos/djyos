//----------------------------------------------------
// Copyright (c) 2018,Open source team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

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
// Copyright (c) 2014 ����Ȩ�ɶ����߲���ϵͳ��Դ�Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
// ģ������: �쳣���׳��Լ��쳣��Ϣ����
// ģ��汾: V1.00
// ������Ա: zhangqf_cyg
// ����ʱ��: 5:59:37 PM/Apr 17, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע:�����쳣��ͳһ���
// ���ļ�Ϊ�쳣���׳��������׳��쳣֮ǰ����ر�֤��Ը��쳣�����⴦���Ѿ�����
// ���ļ�ֻ�Ǹ��õĹ����쳣��Ϣ����ܹ�������ͳϽ�����쳣����


#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "int.h"
#include "arch_feature.h"
#include "blackbox.h"
#include "endian.h"
#include "blackbox_decoder.h"
#include "blackbox_hook.h"
#include "blackbox_osstate.h"
#include "BlackBox_Record.h"
#include "dbug.h"
#include "component_config_blackbox.h"
#include <shell.h>

const char *gBlackBoxActionName[EN_BLACKBOX_DEAL_LENTH] = {
    "IGNORE",
    "DEFAULT",
    "RECORD",
    "RESET",
    "REBOOT",
    "RESTART",
    "WAIT",
    "DELERR",
};

const char *BlackBoxActionName(enum EN_BlackBoxAction action)
{
    if(action < EN_BLACKBOX_DEAL_LENTH)
    {
        return gBlackBoxActionName[action];
    }
    else
    {
        return NULL;
    }
}

//ȫ����ͬ����ô���漰����С�˵�����
#define CN_BLACKBOX_HEADINFO_MAGICNUMBER    ((u32)(0x77777777))
#define CN_BLACKBOX_PARTIALINFO_VALID       ((u32)(0x12345678))

extern bool_t HardExp_InfoDecoderInit(void);

struct BlackBoxHeadInfo
{
    u32   recordendian;     //�쳣��¼�Ǵ�˻���С��,�������߷����쳣��¼
    u32   magicnumber;      //���ڼ����Ϣ�Ƿ���Ч
    u32   BlackBoxType;          //�쳣����
    u32   osstatevalid;     //CN_BLACKBOX_PARTIALINFO_VALID=����ϵͳ����״̬��Ϣ��Ч
    u32   osstateinfolen;   //����ϵͳ����״̬��Ϣ����
    u32   hookvalid;        //CN_BLACKBOX_PARTIALINFO_VALID=hook�ռ�����Ϣ��Ч
    u32   hookinfolen;      //hook�ռ�����Ϣ����
    u32   HookAction;       //hookҪ����쳣������,��enum EN_BlackBoxAction����
    u32   throwinfovalid;   //�׳�����Ϣ�洢���Ƿ���Ч
    u32   throwinfolen;     //�׳�����Ϣ����
    u32   ThrowAction;      //�׳�ʱҪ��Ĵ�����,��enum EN_BlackBoxAction����
    u32   BlackBoxAction;        //���ղ�ȡ�Ķ���.,��enum EN_BlackBoxAction����
    char  decodername[CN_BLACKBOX_NAMELEN_LIMIT];  //�쳣����������,���Ȳ��ó���15
                            //����Ǹ�NULLָ��,���ʾ�޽�����
};//���Կ���ʹ��bitλ��ʡ�洢�ռ䣬���ǿ�������������ʡ���˼����ֽڣ��Ķ����岻��

static bool_t s_bBlackBoxMoDuleInitState = false;
// =============================================================================
// ��������:__BlackBox_ResultMerge
//           ��HOOK�����THROW������ۺϴ���
// �������:Result_Throw,�趨���쳣������
//          Result_Hook,HOOK���쳣������
// �������:��
// ����ֵ  :_SYSBLACKBOX_RESULT_TYPE,�쳣�Ĵ�����
// ˵��    :�ڲ����ú���
// =============================================================================
static enum EN_BlackBoxAction  __BlackBox_ResultMerge(enum EN_BlackBoxAction Result_Throw,\
                                             enum EN_BlackBoxAction Result_Hook)
{
    enum EN_BlackBoxAction result;

    if(EN_BLACKBOX_DEAL_DEFAULT == Result_Hook)
    {
        result = Result_Throw;
    }
    else
    {
        result = Result_Hook;
    }
    return result;
}

//if key is 0XAA55AA55 ,then will not record in the reset reboot and restart
//else key will do record in the reset reboot or restart
extern void reset(u32 key);
extern void reboot(u32 key);
extern void restart_system(u32 key);
// =============================================================================
// ��������: �����쳣�����ս��
// �������:blackboxdealresult,�쳣���
//          blackboxinfo,�Ѽ����쳣��Ϣ
// �������:��
// ����ֵ  :�쳣�����մ�����(��enum _EN_BLACKBOX_DEAL_REASULT����)
// ˵��    :�ڲ����ú���,����쳣���������Ҫ����������ô�Ͳ��᷵�صġ�
// =============================================================================
static enum EN_BlackBoxDealResult  __BlackBox_ExecAction(u32 FinalAction,\
                                    struct BlackBoxRecordPara *blackboxinfo)
{
    enum EN_BlackBoxDealResult result;
    result = BlackBox_Record(blackboxinfo);
    if(result != EN_BLACKBOX_RESULT_SUCCESS)
    {
        printk("blackboxresultdealer:RECORD FAILED!\n\r");
    }
    switch (FinalAction)
    {
        //�����ǲ��᷵�صģ���Ȼ��Ȼ�ǳ�����
        case EN_BLACKBOX_DEAL_RESET:
            reset(0XAA55AA55);
            break;
        case EN_BLACKBOX_DEAL_REBOOT:
            reboot(0XAA55AA55);
            break;
        case EN_BLACKBOX_DEAL_RESTART:
            restart_system(0XAA55AA55);
            break;
        case EN_BLACKBOX_DEAL_WAIT:
            while(1);
            break;
        default:
            break;
    }
    return result;
}
// =============================================================================
// ��������: �׳��쳣��Ϣ
// �������:throwpara,�׳����쳣��Ϣ����
// ����ֵ  :�쳣�������ȡ���ж��������hook�����ܻᱻhook�ķ���ֵ���
// ˵��    :�������ܿ��ܲ����أ�ֱ�Ӹ�λ
// =============================================================================
enum EN_BlackBoxDealResult  BlackBox_Recorder(struct BlackBoxThrowPara *throwpara)
{
    bool_t result;
    enum EN_BlackBoxAction   HookAction;
    enum EN_BlackBoxAction   ThrowAction;
    enum EN_BlackBoxAction   FinalAction;
    enum EN_BlackBoxDealResult Result;
    ptu32_t infoaddr;
    u32 infolen;
    struct BlackBoxHeadInfo  headinfo;
    struct BlackBoxRecordPara recordpara;
    if(s_bBlackBoxMoDuleInitState == false)  //���δ��ʼ�����ܶ�ϵͳ���ö�����ʹ��
    {
        return EN_BLACKBOX_RESULT_PARAERR;
    }
    if(NULL != throwpara)   //�׳���Ч
    {
        //�׳���Ϣ����
        if(NULL != throwpara->DecoderName)
        {
            strncpy(headinfo.decodername, throwpara->DecoderName, CN_BLACKBOX_NAMELEN_LIMIT);
        }
        else
        {
            headinfo.decodername[0] = '\0';
        }
        headinfo.throwinfolen = throwpara->BlackBoxInfoLen;
        headinfo.ThrowAction = (u32)throwpara->BlackBoxAction;
        headinfo.BlackBoxType = throwpara->BlackBoxType;
        headinfo.throwinfovalid = CN_BLACKBOX_PARTIALINFO_VALID;
        ThrowAction = throwpara->BlackBoxAction;
        recordpara.throwinfoaddr = (ptu32_t)(throwpara->BlackBoxInfo);
        recordpara.throwinfolen = throwpara->BlackBoxInfoLen;
        recordpara.headinfoaddr = (ptu32_t)(&(headinfo));
        recordpara.headinfolen = (u32)(sizeof(headinfo));

        //HOOK��Ϣ���Ѽ�������Ѽ����ɹ��������׳��������¼����ֱ�ӷ��أ�
        result = BlackBox_HookDealer(throwpara, &infoaddr, &infolen, &HookAction);
        if(false == result)
        {
            HookAction = EN_BLACKBOX_DEAL_DEFAULT;
            headinfo.hookvalid = ~CN_BLACKBOX_PARTIALINFO_VALID;
            infoaddr = 0;
            infolen = 0;
        }
        else
        {
            headinfo.hookvalid = CN_BLACKBOX_PARTIALINFO_VALID;
        }
        FinalAction = __BlackBox_ResultMerge(ThrowAction, HookAction);

        if(FinalAction < EN_BLACKBOX_DEAL_RECORD)
        {
            //����¼������Ļ�����ô��ֱ�ӷ��ذ�,��ֱ����Ц
            Result = EN_BLACKBOX_RESULT_IGNORE;
        }
        else
        {
            headinfo.BlackBoxAction = (u32)FinalAction;
            headinfo.hookinfolen = infolen;
            headinfo.HookAction = HookAction;
            recordpara.hookinfoaddr = infoaddr;
            recordpara.hookinfolen = infolen;

            //����ϵͳ����״̬��Ϣ���Ѽ�(����Ѽ����ɹ����޹ش��)
            __BlackBox_OsStateInfoGather(&infoaddr, &infolen);
            headinfo.osstatevalid = CN_BLACKBOX_PARTIALINFO_VALID;
            headinfo.osstateinfolen = infolen;
            headinfo.recordendian = CN_CFG_BYTE_ORDER;
            headinfo.magicnumber = CN_BLACKBOX_HEADINFO_MAGICNUMBER;

            recordpara.osstateinfoaddr = infoaddr;
            recordpara.osstateinfolen = infolen;

            //���Ѽ�����Ϣ���Ѽ���ϣ���ô�ʹ����
            Result = __BlackBox_ExecAction(FinalAction, &recordpara);
        }
    }
    else
    {
        Result = EN_BLACKBOX_RESULT_IGNORE;
    }
    return Result;
}

// =============================================================================
// ��������: ��Ϣͷ�ֽ���ת��
// �������:blackboxinfo,�쳣��Ϣ
// �������:��
// ����ֵ  :��
// ˵��    :��blackboxinfo����֪��Ϣת���ֽ���,name��ǰ���ֽڱ�����4�ֽڶ����
// =============================================================================
void  __BlackBox_HeadinfoSwapByEndian(struct BlackBoxHeadInfo *headinfo)
{
    u32 temp;
    u32 limit;
    u32 *addr;

    temp = (u32)headinfo;
    limit = (u32)(&(headinfo->decodername[0]));
    while(temp < limit)
    {
        addr = (u32 *)temp;
        *addr = swapl(*addr);
        temp = temp +4;
    }
}
// =============================================================================
// ��������: �쳣��Ϣͷ����
// �������:headinfo,�쳣��Ϣͷ
// �������:��
// ����ֵ  :true�ɹ� falseʧ��
// ˵��    :�ڲ�����
// =============================================================================
bool_t __BlackBox_HeadinfoDecoder(struct BlackBoxHeadInfo *headinfo)
{
    debug_printf("blackbox","blackbox_headinfo:magicnumber   :0x%08x\n\r",headinfo->magicnumber);
    debug_printf("blackbox","blackbox_headinfo:record endian :%s\n\r",\
            headinfo->recordendian==CN_CFG_LITTLE_ENDIAN?"LittleEndian":"BigEndian");
    debug_printf("blackbox","blackbox_headinfo:BlackBoxType       :0x%08x\n\r",headinfo->BlackBoxType);
    debug_printf("blackbox","blackbox_headinfo:osstatevalid  :%s\n\r",\
            headinfo->osstatevalid==CN_BLACKBOX_PARTIALINFO_VALID?"Valid":"Invalid");
    debug_printf("blackbox","blackbox_headinfo:osstateinfolen:%d Bytes\n\r",headinfo->osstateinfolen);
    debug_printf("blackbox","blackbox_headinfo:hookinfo stat :%s\n\r",\
            headinfo->hookvalid==CN_BLACKBOX_PARTIALINFO_VALID?"Valid":"Invalid");
    debug_printf("blackbox","blackbox_headinfo:hookinfolen   :%d Bytes\n\r",headinfo->hookinfolen);
    debug_printf("blackbox","blackbox_headinfo:HookAction    :%s\n\r",BlackBoxActionName(headinfo->HookAction));
    debug_printf("blackbox","blackbox_headinfo:throwinfo stat:%s\n\r",\
            headinfo->throwinfovalid==CN_BLACKBOX_PARTIALINFO_VALID?"Valid":"Invalid");
    debug_printf("blackbox","blackbox_headinfo:throwinfolen  :%d Bytes\n\r",headinfo->throwinfolen);
    debug_printf("blackbox","blackbox_headinfo:ThrowAction   :%s\n\r",BlackBoxActionName(headinfo->ThrowAction));
    debug_printf("blackbox","blackbox_headinfo:BlackBoxAction     :%s\n\r",BlackBoxActionName(headinfo->BlackBoxAction));
    debug_printf("blackbox","blackbox_headinfo:DecoderName   :%s\n\r",headinfo->decodername);

    return true;
}

// =============================================================================
// ��������: �����쳣��Ϣ
// �������:recordpara,�쳣��Ϣ
// �������:��
// ����ֵ  :true�ɹ� falseʧ��
// ˵��    :�쳣��Ϣһ���Ѿ����������ڴ浱�У�������ܻ��޸�ԭ��Ϣ
// =============================================================================
bool_t  BlackBox_InfoDecoder(struct BlackBoxRecordPara *recordpara)
{
    bool_t  result = false;
    u32  endian;
    struct BlackBoxHeadInfo *headinfo;
    struct BlackBoxThrowPara throwpara;
    if(NULL == recordpara)
    {
        result = false;
        debug_printf("blackbox","BlackBox_InfoDecoder:invalid parameter!\n\r");
    }
    else if(( 0 == recordpara->headinfoaddr ) || (sizeof(struct BlackBoxHeadInfo) != recordpara->headinfolen))
    {
        result = false;
        debug_printf("blackbox","BlackBox_InfoDecoder:incomplete exception headinfo!\n\r");
    }
    else
    {
        headinfo = (struct BlackBoxHeadInfo *)(recordpara->headinfoaddr);

        //��С����Ϊ��׼��Ҫ����ΪС����0����С�˶�һ��
        if(((headinfo->recordendian == CN_CFG_LITTLE_ENDIAN)&&(CN_CFG_BYTE_ORDER != CN_CFG_LITTLE_ENDIAN))||\
           ((headinfo->recordendian != CN_CFG_LITTLE_ENDIAN)&&(CN_CFG_BYTE_ORDER == CN_CFG_LITTLE_ENDIAN)))
        {
            //��Ҫ��С��ת��
            __BlackBox_HeadinfoSwapByEndian(headinfo);
        }
        if(CN_BLACKBOX_HEADINFO_MAGICNUMBER != headinfo->magicnumber)
        {
            debug_printf("blackbox","BlackBox_InfoDecoder:headinfo has been destroyed!\n\r");
            result = false;
        }
        else
        {
            //�����׳���Ϣ����
            endian = headinfo->recordendian;
            throwpara.BlackBoxAction = headinfo->ThrowAction;
            throwpara.DecoderName = headinfo->decodername;
            throwpara.BlackBoxInfo = (u8 *)(recordpara->throwinfoaddr);
            throwpara.BlackBoxInfoLen = recordpara->throwinfolen;
            throwpara.BlackBoxType = headinfo->BlackBoxType;
            //��Ϣͷ����
            if(recordpara->headinfolen != sizeof(struct BlackBoxHeadInfo))
            {
                debug_printf("blackbox","BlackBox_InfoDecoder:headinfo incomplete!\n\r");
            }
            else
            {
                result = __BlackBox_HeadinfoDecoder(headinfo);
            }
            //OS״̬����,�������Լ��жϰ��������Ƿ��޸�
            if(recordpara->osstateinfolen != headinfo->osstateinfolen)
            {
                debug_printf("blackbox","BlackBox_InfoDecoder:osstateinfo incomplete!\n\r");
            }
            else
            {
                result = __BlackBox_OsStateInfoDecoder(recordpara->osstateinfoaddr,\
                                             recordpara->osstateinfolen, endian);
            }
            //�׳��쳣����,�������Լ��жϰ��ĳ����Ƿ����������������Ƿ��ƻ�
            if(recordpara->throwinfolen != headinfo->throwinfolen)
            {
                debug_printf("blackbox","BlackBox_InfoDecoder:throwinfo incomplete!\n\r");
            }
            else
            {
                result = BlackBox_ThrowInfodecode(&throwpara,endian);
            }
            //HOOK����,�������Լ��жϰ��ĳ����Ƿ����������������Ƿ��ƻ�
            if(recordpara->hookinfolen != headinfo->hookinfolen)
            {
                debug_printf("blackbox","BlackBox_InfoDecoder:hookinfo incomplete!\n\r");
            }
            else
            {
                result = BlackBox_HookInfoDecoder(&throwpara,recordpara->hookinfoaddr,\
                                             recordpara->hookinfolen, endian);
            }
        }
    }
    return result;
}

// =============================================================================
// ��������: �쳣�����ʼ��,ͬʱ��װ��Ӳ���쳣����Ϣ����������
// �������:para
// �������:��
// ����ֵ  :ptu32_t ��ʱ�޶���
// =============================================================================
void ModuleInstall_BlackBox(void)
{

    extern bool_t BlackBox_ShellInit();
    BlackBox_ShellInit();

    dListInit(&BlackBoxDecoderListHead);

    // ��ʼ��Ӳ���쳣��Ϣ��������,����Ҫ�ر�˵��һ��,����˭�׳��쳣˭������ԭ��,
    // ���翴�Ź��쳣,��ModuleInstall_Wdt�����е���BlackBox_RegisterThrowinfoDecoder
    // ��ʼ�����Ź����쳣��������.Ӳ���쳣��Ϣ��������,ҲӦ���ɳ�ʼ��Ӳ���쳣��
    // ģ��ע�ᡣ��Ӳ���쳣��һ��,������Ԥ����ʱ��ʼ����,����ʱ�쳣�����û��׼
    // ����,�ʱ����������ʼ��.
    HardExp_InfoDecoderInit( );
    s_bBlackBoxMoDuleInitState = true;

    return ;
}

