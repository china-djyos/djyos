//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
// ģ������: blackbox.h
// ģ��汾: V1.00
// ������Ա: zhangqf_cyg
// ����ʱ��: 5:59:52 PM/Apr 17, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע

#ifndef __BLACKBOX_H__
#define __BLACKBOX_H__
#include "stdint.h"
#include "list.h"
#include "hard-exp.h"

//�쳣�����ж�,�������������˳��
enum EN_BlackBoxAction
{
    EN_BLACKBOX_DEAL_IGNORE = 0,     //���Ը��쳣��Ϣ
    EN_BLACKBOX_DEAL_DEFAULT,        //��Ĭ�Ϸ�ʽ����
    EN_BLACKBOX_DEAL_RECORD,         //��Ҫ��¼���쳣
    EN_BLACKBOX_DEAL_RESET,          //Ӳ���������൱���ϵ�����
    EN_BLACKBOX_DEAL_REBOOT,         //��ת��������ַ,���³�ʼ���ڴ��ʱ�ӵ�
    EN_BLACKBOX_DEAL_RESTART,        //����sp���cpu��״̬��,��ת��Ԥ���غ���
    EN_BLACKBOX_DEAL_WAIT,           //�ȴ�,һ�����ڵ���,��������ѭ��
    EN_BLACKBOX_DEAL_ERROR,          //�쳣�������,������������
    EN_BLACKBOX_DEAL_LENTH,          //lenth
};

//�׳��쳣������
enum EN_BlackBoxDealResult
{
    EN_BLACKBOX_RESULT_SUCCESS = 0,        //����ɹ�,���������¼
    EN_BLACKBOX_RESULT_IGNORE,             //�쳣������
    EN_BLACKBOX_RESULT_PARAERR,            //��������
    EN_BLACKBOX_RESULT_RECORD_NOSPACE,     //�洢�ռ䲻��
    EN_BLACKBOX_RESULT_RECORD_HARDERR,     //�洢Ӳ������
    EN_BLACKBOX_RESULT_RECORD_NOMETHOD,    //�洢��������,�޴洢����
};

//ȷ�������쳣��ʼ���
#define CN_BLACKBOX_TYPE_HARD_START      0                   //Ӳ���쳣��ʼ���
#define CN_BLACKBOX_TYPE_SYS_START       0x40000000          //ϵͳ����쳣��ʼ���
#define CN_BLACKBOX_TYPE_USER_START      0x80000000          //�û������쳣��ʼ���

//����ϵͳ����쳣���롣
#define CN_BLACKBOX_TYPE_WDT             (CN_BLACKBOX_TYPE_SYS_START+0)     //���Ź���
#define CN_BLACKBOX_TYPE_STACK_OVER      (CN_BLACKBOX_TYPE_SYS_START+1)     //ջ�������
#define CN_BLACKBOX_TYPE_ETCB_EXHAUSTED  (CN_BLACKBOX_TYPE_SYS_START+2)     //�¼����Ϳ��ƿ�ľ�
#define CN_BLACKBOX_TYPE_ECB_EXHAUSTED   (CN_BLACKBOX_TYPE_SYS_START+3)     //�¼����ƿ�ľ�
#define CN_BLACKBOX_TYPE_MEM_EVTT        (CN_BLACKBOX_TYPE_SYS_START+4)     //ע������ȼ��¼�����ʱ,�ڴ治��.
#define CN_BLACKBOX_TYPE_EVENT_EXIT      (CN_BLACKBOX_TYPE_SYS_START+5)     //�¼��������
#define CN_BLACKBOX_TYPE_SCH_DISABLE_INT (CN_BLACKBOX_TYPE_SYS_START+6)     //��ֹ�ж�ʱ�������

//�쳣��ʾ��Ϣ�������׳��쳣��ʱ��ʹ�ã�
// ע������DecoderName��Ŀ�ģ���Ϊ�˱��ڱ�д���ߣ�����PC�ϣ��쳣��Ϣ�Ķ�����
//     ��ҵ����������Ʒ����Щ��Ʒ�У��쳣����BlackBoxType�ظ��Ŀ����ԱȽϴ󣬲���
//     ���������쳣����DecoderName�Ǹ��ַ����������Ŀ����Ծͺ�С��Ҳ���ڹ���
//     ����쳣��Ϣ������ǿɶ����ַ�����DecoderName����ΪNULL��
struct BlackBoxThrowPara
{
    u32     BlackBoxType;                 //�쳣���ͣ��ο�CN_BLACKBOX_TYPE_HARD_START
    char    *DecoderName;                 //����Ϣ������������,��15�ֽ�
    u8      *BlackBoxInfo;                //�쳣��Ϣ��ַ
    u32     BlackBoxInfoLen;              //��Ϣ����
    enum EN_BlackBoxAction BlackBoxAction;     //�쳣��������,�ο�enum EN_BlackBoxAction
};

////////////////////////�쳣�׳���Ϣ����������ע��ע��////////////////////////
//�����쳣��Ϣ�Ľ�����������������׳�ʱһģһ�����������Լ��Ѽ�����Ϣһ��
//�ܹ������Լ�һ���ܹ��жϳ��ǲ����Լ��Ѽ�����Ϣ
typedef bool_t (*fnBlackBox_ThrowInfoParse)(struct BlackBoxThrowPara *para, u32 endian);

struct BlackBoxInfoDecoder
{
    struct dListNode DecoderList;
    fnBlackBox_ThrowInfoParse MyDecoder;
    char *DecoderName;  //�쳣����������,����ʱ������������ҽ�����,���ó���15�ֽ�
};

//struct BlackBoxHookfunc
//{
//    struct sListNode HookList;
//    fnBlackBox_ThrowInfoParse MyHook;
//};

// =============================================================================
// �������ܣ�BlackBox_RegisterThrowinfoDecoder
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
bool_t BlackBox_RegisterThrowInfoDecoder(struct BlackBoxInfoDecoder *Decoder);


//HOOK��Ϣ�Ѽ����Լ��ṩ�洢HOOK�쳣��Ϣ�Ŀռ�
//////////////////////////////HOOKԭ��//////////////////////////////////////////
// =============================================================================
// �������ܣ�HOOKDealerԭ��
// ���������throwpara, �쳣�׳����׳����쳣������hook�ܽ�ͽ⣬���ܽ������
// ���������infoaddr,�洢�쳣��Ϣ�ĵ�ַ
//           infolen,�洢�Ѽ���Ϣ����
// ����ֵ  �� _SYSBLACKBOX_RESULT_TYPE���ô������Ḳ�ǵ�BSP�Ĵ����쳣�����
// ˵��    �� �쳣ʱ���õ�APP�ṩ���쳣����������������쳣ʱϵͳӦ�ó��������״̬
//           Ӧ�ó����ṩ�Ĺ��Ӻ�����Ӧ�ó�����ڴ���һЩ�ƺ��������ɷ��ظ��ӵ��쳣
//           ��Ϣ����Щ��Ϣ����BSP�ռ���Ӳ����ϵͳ�쳣��Ϣһ�𱣴�
//           �ù��ӵķ��ؽ�������BSP�Ĵ�����һ����Ϊ�쳣�ṹ���ж�����(һ�����
//           ����ľͰԭ�������س̶ȴ��������)
// =============================================================================
typedef  enum EN_BlackBoxAction (*fnBlackBox_Hook)( struct BlackBoxThrowPara *throwpara,\
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
typedef  bool_t (*fnBlackBox_HookParse)(struct BlackBoxThrowPara *throwpara,\
                                        ptu32_t infoaddr, u32 infolen,u32 endian);

//�û�ע�����ע��HOOK�ķ���
bool_t BlackBox_RegisterHook(fnBlackBox_Hook  fnHookFunc,\
                            fnBlackBox_HookParse fnHookParse);
bool_t BlackBox_UnRegisterHook(void);

////////////////////////�洢����ģ�ͼ���ע��ע��//////////////////////
struct BlackBoxRecordPara
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
// ����ֵ  ����enum EN_BlackBoxDealResult
// =============================================================================
typedef  enum EN_BlackBoxDealResult (*fnBlackBox_RecordSave)(\
                                       struct BlackBoxRecordPara  *recordpara);
// =============================================================================
// �������ܣ�������е��쳣��Ϣ������쳣��Ϣ�洢����
// �����������
// �����������
// ����ֵ  ��TRUE �ɹ��� FALSEʧ��
// =============================================================================
typedef  bool_t (*fnBlackBox_RecordClean)(void);

// =============================================================================
// �������ܣ��鿴һ���洢�˶������쳣��Ϣ
// ���������
// ���������recordnum,�洢���쳣��Ϣ��Ŀ��
// ����ֵ  ��FALSE,ʧ�� TRUE�ɹ�
// =============================================================================
typedef  bool_t (*fnBlackBox_RecordCheckNum)(u32 *recordnum);

// =============================================================================
// �������ܣ�����ָ���쳣��Ŀ�ĳ���
// ���������assignedno,ָ������Ŀ
// ���������recordlen,����Ŀ�ĳ���
// ����ֵ  ��FALSE,ʧ�� TRUE�ɹ�
// =============================================================================
typedef  bool_t (*fnBlackBox_RecordCheckLen)(u32 assignedno, u32 *recordlen);

// =============================================================================
// �������ܣ��Ӵ洢�����л�ȡָ����Ŀ���쳣֡��Ϣ
// ���������assignedno,ָ�����쳣֡��Ŀ
//           buflen,����������
// ���������buf,���ڴ洢��ȡָ����Ŀ���쳣��Ϣ
//          recordpara,�쳣��Ϣ�洢ʱ�Ĳ������ڴ��Ƕ�buf�ĸ������ֵĶ���
// ����ֵ  ��FALSE,ʧ�� TRUE�ɹ�
// =============================================================================
typedef  bool_t (*fnBlackBox_RecordGet)(u32 assignedno,u32 buflen,u8 *buf,\
                                         struct BlackBoxRecordPara *recordpara);

// =============================================================================
// �������ܣ�������ʱ��ɨ���쳣�洢��¼����ȡ�ؼ���Ϣ�����Ժ�洢
// �����������
// �����������
// ����ֵ  ����
// =============================================================================
typedef  void (*fnBlackBox_RecordScan)(void);

struct BlackBoxRecordOperate
{
    fnBlackBox_RecordScan        fnBlackBoxRecordScan;    //�����洢��ɨ�裬
    fnBlackBox_RecordSave        fnBlackBoxRecordSave;    //��¼һ���쳣��Ϣ
    fnBlackBox_RecordClean       fnBlackBoxRecordClean;   //��������쳣��Ϣ
    fnBlackBox_RecordCheckNum    fnBlackBoxRecordCheckNum;//��ȡ�洢�쳣��Ϣ����Ŀ��
    fnBlackBox_RecordCheckLen    fnBlackBoxRecordCheckLen;//��ȡָ����Ŀ�ĳ���
    fnBlackBox_RecordGet         fnBlackBoxRecordGet;     //��ȡָ����Ŀ���쳣��Ϣ
};
bool_t  BlackBox_RegisterRecorder(struct BlackBoxRecordOperate *opt);
bool_t BlackBox_UnRegisterRecorder(void);
enum EN_BlackBoxDealResult  BlackBox_ThrowExp(struct BlackBoxThrowPara *throwpara);
void ModuleInstall_BlackBox(void);
const char *BlackBox_ActionName(enum EN_BlackBoxAction action); //��ȡ�쳣��������

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
}tagBlackBoxLowLevelOpt;
//CONFIGUER A LINE MEMORY TO THE EXCEPTION RECORDER
bool_t BlackBox_NvramRecordRegister(tagBlackBoxLowLevelOpt *lopt,u16 memsize);
//CONFIGUER A LINE MEMORY TO THE EXCEPTION RECORDER FOR THE AUTO OR MANUAL TEST
bool_t BlackBox_NvramRecordRegisterTest(tagBlackBoxLowLevelOpt *lopt,u16 memsize,u32 maxlen,bool_t autotest,bool_t debugmsg);
//THIS IS A EXCEPTION RECORDER TEST ENGINE
bool_t ModuleInstall_BlackBoxRecordTest(struct BlackBoxRecordOperate *opt,u32 maxlen,bool_t autotest,bool_t debugmsg);

//�ö������ڼ���ջʹ�õ��ڴ��Ƿ��ǺϷ���ַ
typedef struct
{
    const u8   *start;
    const u32   len;
}tagVmMemItem;
//�ö������ڶ��������ǵ�����ģʽ
typedef enum
{
    EN_Start_POWERDOWN = 0,//WHICH MEANS BOOT FROM POWER DOWN
    EN_Start_HRST,         //WHICH MEANS BOOT FROM EXTERNAL HARD RESET BUT POWERON
    EN_Start_SRST,         //WHICH MEANS BOOT FROM INTERNAL SOFT RESET
    EN_Start_REBOOT,       //WHICH MEANS BOOT FROM SOFT REBOOT
    EN_Start_RELOAD,       //WHICH MEANS BOOT FROM RELOAD
    EN_Start_UNKNOWN,      //WHICH MEANS BOOT FROM UNKNOWN REASON
    EN_Start_NOTIMPLEMENT, //WHICH MEANS BOOT BSP NOT IMPLEMENT
    EN_Start_LAST,
}enStartMode;

#define CN_BOOT_LEGALKEY   0XAA55AA55  //����reboot��CPU_Reset��reload�����key�������¼���������Ϊ�쳣��¼
typedef enStartMode  (*fnGetStartMode)(void); //���ڻ�ȡ���ǵ�CPU����ģʽ�ĺ���ԭ��
//��װϵͳ������¼�洢ģ�飺tab�����ǵ�ջ����ʹ�õ��ڴ淶Χ�������Ƕ����NULL����,���ɸ��ģ�GetOsBootModeHard�����������־����ҪBSP�ṩ��GetOsBootModeSoft��
//���������־����ҪBSP�ṩ
bool_t ModuleInstall_SysStartReason(const tagVmMemItem *tab[],fnGetStartMode getmodehard,
                            fnGetStartMode getmodesoft);
//�׳��쳣������Ϣ��������ϵͳ��reboot��CPU_Reset��reload���ṩ�Ƿ���KEY��ʱ��
//�Ϸ���KEY��0xaa55aa55,�����ľ�����Ϊ�ǷǷ��ģ��ⲿ����Ҫcpu��bsp ��port��֧��
bool_t ThrowOsStartInfo(enStartMode mode);


#endif /* __BLACKBOX_H__ */
