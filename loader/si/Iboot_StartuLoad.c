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
//����ģ��: Iboot
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ��Ӧ�ñ�̵ĺ����ļ�
//����˵��:
//�޶���ʷ:
//1. ����: 2016-06-17
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "stdio.h"
#include "xip.h"
#include <stdlib.h>
#include "string.h"
#include "cpu-optional.h"
#include "component_config_iboot.h"
#if(CN_CPU_OPTIONAL_CACHE==1)
#include "set-cache.h"
#endif
#include "dbug.h"
#include "iboot_info.h"
#include <xip.h>

struct CopyRecord{
    u32 load_start_address;
    u32 run_start_address;
    u32 size;
    u32 type;    //0:zero init;    1:copy
};

struct copy_table{
    u32 record_size;
    u32 record_cnt;
    struct CopyRecord record[1];
};

//-----------------------------------------------------------------
//���ܣ���Ӳ�������Ƿ�ǿ�ƽ���Iboot�����˺�������TRUE����ǿ������Iboot��ͨ����ʹ
//      ��һ��gpio��ͨ�����߾�����
//      ��������£������������APP���ǿ�����runiboot�����л���Iboot״̬�ģ�����
//      ��Ӳ����Ŀ���ж���
//     1���������쳣���󣬲�����shell�л�ʱ���ṩһ�����ȴ�ʩ��
//     2�����ڰ�ȫ���ǣ�APP��û�а����л����룬����������Դ�Ĺ�ϵ���õ���shell��
//��������
//���أ��ޡ�
//˵�������������漰����Ӳ�������ڱ��ļ��г�ʼ�����ر���Ҫע����ǣ����������δ
//      ���صĺ������ر��ǿ⺯����
//      �����������ṩ�����û��������ӦӲ�������Լ򵥷���false��
//-----------------------------------------------------------------
__attribute__((weak))  bool_t Iboot_IAP_IsForceIboot()
{
    return false;
}
//----ѡ�������Ŀ����-----------------------------------------------------------
//���ܣ�ѡ����ش洢�洢�����ĸ���Ŀ����,Iboot��APP��
//����: �ޡ�
//����: �ޡ�
//----------------------------------------------------------------------------
void Iboot_IAP_SelectLoadProgam(void)
{
#if (CFG_RUNMODE_BAREAPP == 1)
        Iboot_LoadPreload();   //ֱ�Ӽ�������APP
#else
    Iboot_SiIbootAppInfoInit();

    if(Iboot_IAP_IsForceIboot())//Ӳ����������iboot
    {
        Run_Iboot(HARD_SET_RUN_IBOOT);//���Ӳ����������iboot��Ϣ
    }
    if(XIP_IsRamIbootFlag())//ram�б������iboot
    {
        Run_Iboot(RAM_SET_RUN_IBOOT);
    }

#if (CFG_APP_RUNMODE == CN_RUN_FORM_FILE)
    if(false == Run_App(RUN_APP_FROM_FILE))//
#elif (CFG_APP_RUNMODE == CN_DIRECT_RUN)
    if(false == Run_App(RUN_APP_FROM_DIRECT))
#else
    #error "error �� û�ж���APP���ط�ʽ����";
#endif
        Run_Iboot(CHACK_ERROR);
#endif      //for (CFG_RUNMODE_BAREAPP == 1)

}

void Iboot_PreStart(void);

extern struct copy_table preload_copy_table;

//----Ԥ���س���---------------------------------------------------------------
//���ܣ����������������жϹ���ģ�飬��������
//����: �ޡ�
//����: �ޡ�
//----------------------------------------------------------------------------
//��ע: ��������ֲ���У��뿪��ϵͳ�йأ�Ҳ��Ŀ��Ӳ�������й�
void Iboot_LoadPreload(void)
{
//    void (*volatile pl_1st)(void) = Iboot_PreStart;

    volatile u32 *src,*des;
    volatile u32 i, j;
    for(i=0; i<preload_copy_table.record_cnt; i++) {
        src = (u32*) preload_copy_table.record[i].load_start_address;
        des = (u32*) preload_copy_table.record[i].run_start_address;
        if(preload_copy_table.record[i].type == 1) {    //copy
            if(src != des) {
                for(j=0; j<preload_copy_table.record[i].size; src++,des++) {
                    *des=*src;
                    j+=4;
                }
            }
        } else if(preload_copy_table.record[i].type == 0) {    //zero init
            for(j=0; j<preload_copy_table.record[i].size; des++) {
                *des=0;
                j+=4;
            }
        }
    }

#if CN_CPU_OPTIONAL_CACHE == 1
    Cache_CleanData();
    Cache_InvalidInst();
    Cache_config();
#endif

    Iboot_PreStart();
}

extern struct copy_table sysload_copy_table;

//----����ϵͳ�ں˼��س���-----------------------------------------------------
//���ܣ��������в���ϵͳ�ں˴��룬�Լ���siģʽ��ȫ��Ӧ�ó�����롣
//����: �ޡ�
//����: �ޡ�
//----------------------------------------------------------------------------
//��ע: ��������ֲ���У��뿪��ϵͳ�йأ�Ҳ��Ŀ��Ӳ�������й�
void Iboot_Loader(void)
{
   volatile  u32 *src,*des;
   volatile  u32 i, j;

    for(i=0; i<sysload_copy_table.record_cnt; i++) {
        src = (u32*) sysload_copy_table.record[i].load_start_address;
        des = (u32*) sysload_copy_table.record[i].run_start_address;
        if(sysload_copy_table.record[i].type == 1) {    //copy
            if(src != des) {
                for(j=0; j<sysload_copy_table.record[i].size; src++,des++) {
                    *des=*src;
                    j+=4;
                }
            }
        } else if(sysload_copy_table.record[i].type == 0) {    //zero init
            for(j=0; j<sysload_copy_table.record[i].size; des++) {
                *des=0;
                j+=4;
            }
        }
    }

#if CN_CPU_OPTIONAL_CACHE == 1
    Cache_CleanData();
    Cache_InvalidInst();
#endif
}

void Heap_StaticModuleInit(void);
void Int_Init(void);
void critical(void);
void SYS_Start(void);
void __libc_init_array(void);
void __libc_fini_array(void);
//----����ϵͳ�ں˼��س���-----------------------------------------------------
//���ܣ��������в���ϵͳ�ں˴��룬�Լ���siģʽ��ȫ��Ӧ�ó�����롣
//����: �ޡ�
//����: �ޡ�
//��ע: ��������ֲ���У��뿪��ϵͳ�йأ�Ҳ��Ŀ��Ӳ�������й�
//----------------------------------------------------------------------------
void Iboot_PreStart(void)
{
    extern void HardExp_Init(void);
    //�Դ˿��Ե���malloc�庯������׼��̬�ѷ����㷨����Ҫ�ͷ��ڴ棬Ҫ���ϸ����
    //����malloc-free���������߲�����
    Heap_StaticModuleInit();
    HardExp_Init();
    Int_Init();
    critical();
    Iboot_Loader();
    //__libc_init_array��__libc_fini_array����ld������̬����ķ�ʽ����ȫ�ֹ��������
    //��������������SIģʽ����̬������Ҫ������ơ�
    //ֻ�ܵ�loader��û���漰preloader���ʽ�������Ȳ�������c++д��
    __libc_init_array();
    SYS_Start();        //��ʼ����ϵͳ
    __libc_fini_array();
}

