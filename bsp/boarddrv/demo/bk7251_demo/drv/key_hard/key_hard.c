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
//2. ����: 2019-06-05
//   ����: lm
//   �°汾��: V1.0.0
//   �޸�˵��:��ֲ��ZQ1401����
//1. ����: 2009-03-10
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
#include "keyboard.h"
#include "board.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_Keyboard(const char *dev_name);
//    ModuleInstall_Keyboard(CFG_KEYBOARD_NAME);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"keyboard hard driver"//����ļ�������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"key board"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
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
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_KEYBOARD_NAME              "KEYBOARD"        //"������",���ü�������
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

#if 0       //���������Ȳ������Ĵ���
#define TOUCH_KEY_DISINFECTANT  GPIO1   // ������������
#define TOUCH_KEY_WATER1        GPIO11  // ������ˮ��1������
#define TOUCH_KEY_WATER2        GPIO10  // ������ˮ��2������
#define TOUCH_KEY_LOCK_UNLOCK   GPIO9   // ����������������
#define TOUCH_KEY_TEMP_MINUS    GPIO6   // ��ˮ��-������
#define TOUCH_KEY_TEMP_PLUS     GPIO28  // ��ˮ��+������

u32 keyboard_scan(void);
//----��ʼ������ģ��-----------------------------------------------------------
//����: ��ʼ��һ����windows�ļ��̺Ͱ�ťģ��ļ��̣��ü��̹�8������
//����: ��
//����: ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_Keyboard(const char *dev_name)
{
    static struct KeyBoardPrivate key_brd;

    bk_gpio_config_input_pup(TOUCH_KEY_DISINFECTANT);
    bk_gpio_config_input_pup(TOUCH_KEY_WATER1);
    bk_gpio_config_input_pup(TOUCH_KEY_WATER2);
    bk_gpio_config_input_pup(TOUCH_KEY_LOCK_UNLOCK);
    bk_gpio_config_input_pup(TOUCH_KEY_TEMP_MINUS);
    bk_gpio_config_input_pup(TOUCH_KEY_TEMP_PLUS);

    key_brd.read_keyboard = keyboard_scan;
    Keyboard_InstallDevice(dev_name, &key_brd);
    key_brd.vtime_limit = 0;
    key_brd.vtime_count = 100;
    key_brd.key_bak = 0;
    key_brd.key_now = 0;
    return true;
}


//----����Ӳ��ɨ��------------------------------------------------------------
//����: ��ȡ���̰���״̬��ÿ��������8bit��ʾ��32λ����ֵ���Դ��4��������ɨ�赽
//      �ĵ�һ�������������8λ�����ĸ������������8λ��ɨ�赽4�������󣬲��ټ�
//      ������ͬʱ���³���4��������ֻ��¼4����
//����:  �μ� struct KeyBoardPrivate��ע��
//----------------------------------------------------------------------------
u32 keyboard_scan(void)
{
    u32 readed;
    uint8_t i = 0;
    readed = 0;

    if (0 == gpio_input(TOUCH_KEY_LOCK_UNLOCK))
    {
        readed |= (u32)(1<<(i<<3));
        i++;
    }

    if (0 == gpio_input(TOUCH_KEY_WATER1))
    {
        readed |= (u32)(2<<(i<<3));
        i++;
    }

    if (0 == gpio_input(TOUCH_KEY_WATER2))
    {
        readed |= (u32)(3<<(i<<3));
        i++;
    }

    if (0 == gpio_input(TOUCH_KEY_DISINFECTANT))
    {
        readed |= (u32)(4<<(i<<3));
        i++;
    }

    if (i < 4)
    {
        if (0 == gpio_input(TOUCH_KEY_TEMP_MINUS))
        {
            readed |= (u32)(5<<(i<<3));
            i++;
        }
    }

    if (i < 4)
    {
        if (0 == gpio_input(TOUCH_KEY_TEMP_PLUS))
        {
            readed |= (u32)(6<<(i<<3));
            i++;
        }
    }
    return(readed);
}

#endif
extern int djy_adc_read(uint16_t channel);
u32 keyboard_scan(void);
//----��ʼ������ģ��-----------------------------------------------------------
//����: ��ʼ��һ����windows�ļ��̺Ͱ�ťģ��ļ��̣��ü��̹�8������
//����: ��
//����: ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_Keyboard(const char *dev_name)
{
    static struct KeyBoardPrivate key_brd;

    key_brd.read_keyboard = keyboard_scan;
    Keyboard_InstallDevice(dev_name, &key_brd);
    key_brd.vtime_limit = 0;
    key_brd.vtime_count = 100;
    key_brd.key_bak = 0;
    key_brd.key_now = 0;
    return true;
}

u32 keyboard_scan(void)
{
    int Vol;
    enum EasyKeyValue current_key;
    Vol = djy_adc_read(2);
    if(Vol > 1550)
    {
        if(Vol < 2200)
            current_key = COMEBACK_KEY;
        else
            current_key = NO_KEY;
    }
    else if(Vol > 950)
        current_key = VOL_UP_KEY;
    else if(Vol > 300)
        current_key = VOL_DOWN_KEY;
    else if(Vol >= 0)
        current_key = PAUSE_PLAY_KEY;

    if(djy_gpio_read(13))
        current_key = POWER_KEY;

//    printf("key = %d\r\n", current_key);
    return current_key;
}
