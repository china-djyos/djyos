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
//����ģ��:����
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: ɨ��Ӳ��������������ֻ��ֱ�Ӹ���������ǰֵ�����������
//����˵��:
//�޶���ʷ:
//2. ����: 2009-05-29
//   ����: lst
//   �°汾��: V1.1.0
//   �޸�˵��: ����key_hard_init����
//1. ����: 2009-03-10
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
#include "keyboard.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_KeyBoardHard(ptu32_t para);
//    ModuleInstall_KeyBoardHard(CFG_KEYBOARD_VTIME);
//#if(CFG_MODULE_ENABLE_GRAPHICAL_DECORATE_DEVELOPMENT == true)
//    extern bool_t GDD_AddInputDev(const char *InputDevName);
//    GDD_AddInputDev(CFG_KEYBOARD_NAME);
//#endif
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"keyboard hard driver"//��������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                                   //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                                //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                                  //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium                                //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                                  //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"key board","cpu onchip gpio"        //������������������������none����ʾ�������������
                                                  //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                                  //����������������������г����á�,���ָ�
//weakdependence:"graphical decorate development"                            //��������������������������none����ʾ�������������
                                                  //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                                  //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                                  //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_KEYBOARD_HARD_DRIVER == false )
//#warning  " keyboard_hard_driver  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_KEYBOARD_HARD_DRIVER    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100000,
#define CFG_KEYBOARD_VTIME              (100*1000)  //"����ʱ��",���÷���ʱ��
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

u32 keyboard_scan(void);
//----��ʼ������ģ��-----------------------------------------------------------
//����: ��ʼ��һ����windows�ļ��̺Ͱ�ťģ��ļ��̣��ü��̹�8������
//����: ��
//����: ��
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_KeyBoardHard(ptu32_t para)
{
    static struct KeyBoardPrivate windows_keyboard;
    key_hard_init();
    windows_keyboard.read_keyboard = keyboard_scan;
    windows_keyboard.vtime_limit = para;
    windows_keyboard.vtime_count = 100;
    windows_keyboard.key_bak = 0;
    windows_keyboard.key_now = 0;
    Keyboard_InstallDevice("keyboard driver",&windows_keyboard);
    return 1;
}

//----����Ӳ��ɨ��------------------------------------------------------------
//����: ��ȡ���̰���״̬��ÿ��������8bit��ʾ��32λ����ֵ���Դ��4��������ɨ�赽
//      �ĵ�һ�������������8λ�����ĸ������������8λ��ɨ�赽4�������󣬲��ټ�
//      ������ͬʱ���³���4��������ֻ��¼4����
//      key0=gpf0,key1=gpf2,key2=gpg3,key3=gpg11
//����: ��
//����:  �μ� struct KeyBoardPrivate��ע��
//----------------------------------------------------------------------------
u32 keyboard_scan(void)
{
    u32 readed;
    uint8_t i = 0;
    u32 reg;

    readed = 0;
    reg = GPIO_GetData(CN_GPIO_C);
    if( ! (reg & 0x0400))
    {
        readed |= (u32)(4<<(i<<3));
        i++;
    }
    if( ! (reg & 0x0800))
    {
        readed |= (u32)(3<<(i<<3));
        i++;
    }
    if( ! (reg & 0x1000))
    {
        readed |= (u32)(2<<(i<<3));
        i++;
    }
    if( ! (reg & 0x2000))
    {
        readed |= (u32)(1<<(i<<3));
        i++;
    }
    return(readed);
}
