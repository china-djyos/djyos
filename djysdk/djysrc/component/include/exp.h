//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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
// ģ������: exp.h
// ģ��汾: V1.00
// ������Ա: zhangqf_cyg
// ����ʱ��: 5:59:52 PM/Apr 17, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע

#ifndef __EXP_H__
#define __EXP_H__
#include "stdint.h"
#include "list.h"
#include "hard-exp.h"

//�쳣��Ϣ���������ֳ���15�ֽ�,������16�ֽ�(��\0)
#define CN_EXP_NAMELEN_LIMIT    16

//�쳣�����ж�,�������������˳��
enum EN_ExpAction
{
    EN_EXP_DEAL_IGNORE = 0,     //���Ը��쳣��Ϣ
    EN_EXP_DEAL_DEFAULT,        //��Ĭ�Ϸ�ʽ����
    EN_EXP_DEAL_RECORD,         //��Ҫ��¼���쳣
    EN_EXP_DEAL_RESET,          //Ӳ���������൱���ϵ�����
    EN_EXP_DEAL_REBOOT,         //��ת��������ַ,���³�ʼ���ڴ��ʱ�ӵ�
    EN_EXP_DEAL_RESTART,        //����sp���cpu��״̬��,��ת��Ԥ���غ���
    EN_EXP_DEAL_WAIT,           //�ȴ�,һ�����ڵ���,��������ѭ��
    EN_EXP_DEAL_ERROR,          //�쳣�������,������������
    EN_EXP_DEAL_LENTH,          //lenth
};

//�׳��쳣������
enum EN_ExpDealResult
{
    EN_EXP_RESULT_SUCCESS = 0,        //����ɹ�,���������¼
    EN_EXP_RESULT_IGNORE,             //�쳣������
    EN_EXP_RESULT_PARAERR,            //��������
    EN_EXP_RESULT_RECORD_NOSPACE,     //�洢�ռ䲻��
    EN_EXP_RESULT_RECORD_HARDERR,     //�洢Ӳ������
    EN_EXP_RESULT_RECORD_NOMETHOD,    //�洢��������,�޴洢����
};

//ȷ�������쳣��ʼ���
#define CN_EXP_TYPE_HARD_START      0                   //Ӳ���쳣��ʼ���
#define CN_EXP_TYPE_SYS_START       0x40000000          //ϵͳ����쳣��ʼ���
#define CN_EXP_TYPE_USER_START      0x80000000          //�û������쳣��ʼ���

//����ϵͳ����쳣���롣
#define CN_EXP_TYPE_WDT             (CN_EXP_TYPE_SYS_START+0)     //���Ź���
#define CN_EXP_TYPE_STACK_OVER      (CN_EXP_TYPE_SYS_START+1)     //ջ�������
#define CN_EXP_TYPE_ETCB_EXHAUSTED  (CN_EXP_TYPE_SYS_START+2)     //�¼����Ϳ��ƿ�ľ�
#define CN_EXP_TYPE_ECB_EXHAUSTED   (CN_EXP_TYPE_SYS_START+3)     //�¼����ƿ�ľ�
#define CN_EXP_TYPE_MEM_EVTT        (CN_EXP_TYPE_SYS_START+4)     //ע������ȼ��¼�����ʱ,�ڴ治��.
#define CN_EXP_TYPE_EVENT_EXIT      (CN_EXP_TYPE_SYS_START+5)     //�¼��������

//�쳣��ʾ��Ϣ�������׳��쳣��ʱ��ʹ�ã�
// ע������DecoderName��Ŀ�ģ���Ϊ�˱��ڱ�д���ߣ�����PC�ϣ��쳣��Ϣ�Ķ�����
//     ��ҵ����������Ʒ����Щ��Ʒ�У��쳣����ExpType�ظ��Ŀ����ԱȽϴ󣬲���
//     ���������쳣����DecoderName�Ǹ��ַ����������Ŀ����Ծͺ�С��Ҳ���ڹ���
//     ����쳣��Ϣ������ǿɶ����ַ�����DecoderName����ΪNULL��
struct ExpThrowPara
{
    u32     ExpType;                 //�쳣���ͣ��ο�CN_EXP_TYPE_HARD_START
    char    *DecoderName;            //����Ϣ������������,��15�ֽ�
    u8      *ExpInfo;                //�쳣��Ϣ��ַ
    u32     ExpInfoLen;              //��Ϣ����
    enum EN_ExpAction ExpAction;     //�쳣��������,�ο�enum EN_ExpAction
};

////////////////////////�쳣�׳���Ϣ������ģ�ͼ���ע��ע��////////////////////////
//�����쳣��Ϣ�Ľ�����ģ�ͣ�����������׳�ʱһģһ�����������Լ��Ѽ�����Ϣһ��
//�ܹ������Լ�һ���ܹ��жϳ��ǲ����Լ��Ѽ�����Ϣ
typedef bool_t (*fntExp_ThrowInfoParse)(struct ExpThrowPara *para, u32 endian);

struct ExpInfoDecoder
{
    struct dListNode DecoderList;
    fntExp_ThrowInfoParse MyDecoder;
    char *DecoderName;  //�쳣����������,����ʱ������������ҽ�����,���ó���15�ֽ�
};

struct ExpHookfunc
{
    struct sListNode HookList;
    fntExp_ThrowInfoParse MyHook;
};

// =============================================================================
// �������ܣ�Exp_RegisterThrowinfoDecoder
//          ע������쳣��Ϣ������
// ���������
//          decoder,�쳣������
//          name,�쳣����������,���ٱ�֤��ȫ�ֵ��Ҳ�����
// ���������
// ����ֵ  ��true,�ɹ�ע��;false,ע��ʧ�ܣ�
// ˵��    ��
//          1,����Ӧ���쳣���Ѿ���ע���˵�ʱ�򣬻����δ��ע����쳣�Ž���ע��
//          2,���������Ѿ����ڶ�Ӧ�������쳣�ᵼ��ע��ʧ���(ֻ�ǡ�\0��Ҳ����Ч��)
//          3,��ע��ʧ�ܵĿ��ܣ����ע���鷵�ؽ��
// =============================================================================
bool_t Exp_RegisterThrowInfoDecoder(struct ExpInfoDecoder *Decoder);


//HOOK��Ϣ�Ѽ����Լ��ṩ�洢HOOK�쳣��Ϣ�Ŀռ�
//////////////////////////////HOOKԭ��//////////////////////////////////////////
// =============================================================================
// �������ܣ�HOOKDealerԭ��
// ���������throwpara, �쳣�׳����׳����쳣������hook�ܽ�ͽ⣬���ܽ������
// ���������infoaddr,�洢�쳣��Ϣ�ĵ�ַ
//           infolen,�洢�Ѽ���Ϣ����
// ����ֵ  �� _SYSEXP_RESULT_TYPE���ô������Ḳ�ǵ�BSP�Ĵ����쳣�����
// ˵��    �� �쳣ʱ���õ�APP�ṩ���쳣����������������쳣ʱϵͳӦ�ó��������״̬
//           Ӧ�ó����ṩ�Ĺ��Ӻ�����Ӧ�ó�����ڴ���һЩ�ƺ��������ɷ��ظ��ӵ��쳣
//           ��Ϣ����Щ��Ϣ����BSP�ռ���Ӳ����ϵͳ�쳣��Ϣһ�𱣴�
//           �ù��ӵķ��ؽ�������BSP�Ĵ�����һ����Ϊ�쳣�ṹ���ж�����(һ�����
//           ����ľͰԭ�������س̶ȴ��������)
// =============================================================================
typedef  enum EN_ExpAction (*fntExp_Hook)( struct ExpThrowPara *throwpara,\
                                          ptu32_t *infoaddr,u32 *infolen);

// =============================================================================
// �������ܣ�HOOKDecoderԭ��
// ���������throwpara, �쳣�׳����׳����쳣������hook�ܽ�ͽ⣬���ܽ������
//          infoaddr,HOOK��Ϣ�洢��ַ
//          infolen, HOOK��Ϣ��Ч����
//          endian, hook��Ϣ�Ĵ洢��С��
// �����������
// ����ֵ  ��true�ɹ�  false ʧ�ܣ�û��ע������أ�
// =============================================================================
typedef  bool_t (*fntExp_HookParse)(struct ExpThrowPara *throwpara,\
                                        ptu32_t infoaddr, u32 infolen,u32 endian);

//�û�ע�����ע��HOOK�ķ���
bool_t Exp_RegisterHook(fntExp_Hook  fnHookFunc,\
                            fntExp_HookParse fnHookParse);
bool_t Exp_UnRegisterHook(void);

////////////////////////�洢����ģ�ͼ���ע��ע��//////////////////////
struct ExpRecordPara
{
    u32     headinfolen;     //ͷ��Ϣ����
    ptu32_t headinfoaddr;    //ͷ��Ϣ��ַ
    u32     osstateinfolen;  //OS״̬��Ϣ����
    ptu32_t osstateinfoaddr; //OS״̬��Ϣ��ַ
    u32     hookinfolen;     //HOOK��Ϣ����
    ptu32_t hookinfoaddr;    //HOOK��Ϣ��ַ
    u32     throwinfolen;    //throw��Ϣ����
    ptu32_t throwinfoaddr;   //throw��Ϣ��ַ
};
// =============================================================================
// �������ܣ�����һ֡�쳣��Ϣ������ʧ�洢�����洢����������������߼�ˣ����쳣�洢��������߾���
// ���������recordpara,һ֡�쳣��Ϣ
// ���������
// ����ֵ  ����enum EN_ExpDealResult
// =============================================================================
typedef  enum EN_ExpDealResult (*fntExp_RecordSave)(\
                                       struct ExpRecordPara  *recordpara);
// =============================================================================
// �������ܣ�fnExp_RecordCleanModule
//          ������е��쳣��Ϣ������쳣��Ϣ�洢����
// �����������
// �����������
// ����ֵ  ��TRUE �ɹ��� FALSEʧ��
// =============================================================================
typedef  bool_t (*fntExp_RecordClean)(void);

// =============================================================================
// �������ܣ�fnExp_RecordCheckNumModule
//          �鿴һ���洢�˶������쳣��Ϣ
// ���������
// ���������recordnum,�洢���쳣��Ϣ��Ŀ��
// ����ֵ  ��FALSE,ʧ�� TRUE�ɹ�
// =============================================================================
typedef  bool_t (*fntExp_RecordCheckNum)(u32 *recordnum);

// =============================================================================
// �������ܣ�fnExp_RecordCheckLenModule
//          ����ָ���쳣��Ŀ�ĳ���
// ���������assignedno,ָ������Ŀ
// ���������recordlen,����Ŀ�ĳ���
// ����ֵ  ��FALSE,ʧ�� TRUE�ɹ�
// =============================================================================
typedef  bool_t (*fntExp_RecordCheckLen)(u32 assignedno, u32 *recordlen);

// =============================================================================
// �������ܣ�fnExp_RecordGetModule
//          �Ӵ洢�����л�ȡָ����Ŀ���쳣֡��Ϣ
// ���������assignedno,ָ�����쳣֡��Ŀ
//           buflen,����������
// ���������buf,���ڴ洢��ȡָ����Ŀ���쳣��Ϣ
//          recordpara,�쳣��Ϣ�洢ʱ�Ĳ������ڴ��Ƕ�buf�ĸ������ֵĶ���
// ����ֵ  ��FALSE,ʧ�� TRUE�ɹ�
// =============================================================================
typedef  bool_t (*fntExp_RecordGet)(u32 assignedno,u32 buflen,u8 *buf,\
                                         struct ExpRecordPara *recordpara);

// =============================================================================
// �������ܣ�fnExp_RecordScanModule
//           ������ʱ��ɨ���쳣�洢��¼����ȡ�ؼ���Ϣ�����Ժ�洢
// �����������
// �����������
// ����ֵ  ����
// =============================================================================
typedef  void (*fntExp_RecordScan)(void);

struct ExpRecordOperate
{
    fntExp_RecordScan        fnExpRecordScan;    //�����洢��ɨ�裬
    fntExp_RecordSave        fnExpRecordSave;    //��¼һ���쳣��Ϣ
    fntExp_RecordClean       fnExpRecordClean;   //��������쳣��Ϣ
    fntExp_RecordCheckNum    fnExpRecordCheckNum;//��ȡ�洢�쳣��Ϣ����Ŀ��
    fntExp_RecordCheckLen    fnExpRecordCheckLen;//��ȡָ����Ŀ�ĳ���
    fntExp_RecordGet         fnExpRecordGet;     //��ȡָ����Ŀ���쳣��Ϣ
};
// =============================================================================
// �������ܣ�Exp_RegisterRecordOpt
//          ע���쳣��Ϣ������
// ���������opt,��Ҫע����쳣��Ϣ������
// ���������
// ����ֵ  ��FALSE,ʧ��  TRUE�ɹ���ʧ�ܵĻ���ʹ��BSPĬ�ϵĴ�����
// ע��    �������ϲ����ṹ����ָ���Ĵ�������Ӧ���ṩ������Ļ���ע�᲻�ɹ�
// =============================================================================
bool_t  Exp_RegisterRecorder(struct ExpRecordOperate *opt);

// =============================================================================
// �������ܣ�Exp_UnRegisterRecordOpt
//           ע���쳣��Ϣ������
// �����������
// �����������
// ����ֵ  ��TRUE�ɹ�  FALSEʧ��,��ʹ��Ĭ�ϵĴ洢����
// =============================================================================
bool_t Exp_UnRegisterRecorder(void);

////////////////////////////�쳣�׳�ģ��////////////////////////////////////////
// =============================================================================
// ��������:Exp_Throw
//          ���������쳣�����
// �������:throwpara,�׳����쳣��Ϣ����
// �������:ExpAction,���쳣�����մ�����
// ����ֵ  :true���ɹ��� false��ʧ��(�������ߴ洢��δ֪ԭ��)
// ˵��    :������������bsp�е��ã�Ҳ������ϵͳ�е���
//          �������Ϊ���쳣ģ�鴦���ͳһ���汾֮���������Ϊ��ͣ�ͬʱ������ֲ
//          �����쳣������֮���໥���������໥����
// =============================================================================
enum EN_ExpDealResult  Exp_Throw(struct ExpThrowPara *throwpara);
// =============================================================================
// ��������:ModuleInstall_Exp
//         ϵͳ�쳣�����ʼ��
// �������:para
// �������:��
// ����ֵ  :ptu32_t ��ʱ�޶���
// ˵��    :��Ҫ�ǳ�ʼ���洢����
// =============================================================================
void ModuleInstall_Exp(void);
const char *ExpActionName(enum EN_ExpAction action); //��ȡ�쳣��������

//THE INTERFACE FOR THE LINE MEMORY RECORDER,IF YOU HAS A LINE MEMORY THAT COULD
//MAINTAIN THE CONTENT WHEN POWER OFF, THEN YOU COULD USE THE LINE MEMORT RECODER
//JUST USE THE CONFIG INTERFACE
//this structure used for the storage the low level method
//THE METHOD COULD NEVER USE THE BLOCK METHOD:LIKE MUTEX SEMP AND SO ON
//IT MUST BE API LIKE RAW METHOD
typedef bool_t (*fnRead)(u16 offset,void *buf,u16 len);  //read length data from the media  from the offset
typedef bool_t (*fnWrite)(u16 offset,void *buf,u16 len); //write length data to the media   from the offset
typedef bool_t (*fnFormat)(u16 offset,u16 len);        //format the media
typedef bool_t (*fnInit)(void);          //initialize the hard media
typedef struct
{
    fnRead     read;
    fnWrite    write;
    fnFormat   format;
    fnInit     init;
}tagExpLowLevelOpt;
//CONFIGUER A LINE MEMORY TO THE EXCEPTION RECORDER
bool_t LineMemExpRecord_Config(tagExpLowLevelOpt *lopt,u16 memsize);
//CONFIGUER A LINE MEMORY TO THE EXCEPTION RECORDER FOR THE AUTO OR MANUAL TEST
bool_t LineMemExpRecord_ConfigTest(tagExpLowLevelOpt *lopt,u16 memsize,u32 maxlen,bool_t autotest,bool_t debugmsg);
//THIS IS A EXCEPTION RECORDER TEST ENGINE
bool_t ModuleInstall_ExpRecordTest(struct ExpRecordOperate *opt,u32 maxlen,bool_t autotest,bool_t debugmsg);
#endif /* __EXP_H__ */
