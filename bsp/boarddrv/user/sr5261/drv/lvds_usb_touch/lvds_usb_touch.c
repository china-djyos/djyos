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


#include "stdint.h"
#include "stdio.h"
#include "stddef.h"
#include "Touch.h"
#include "string.h"
#include "gkernel.h"
#include "shell.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern ptu32_t ModuleInstall_Touch_usb_lvds(struct GkWinObj *desktop,char *touch_dev_name);
//    ModuleInstall_Touch_usb_lvds(GK_GetDesktop(CFG_DISPLAY_NAME),CFG_INPUTDEV_NAME);
//    extern bool_t GDD_AddInputDev(const char *InputDevName);
//    GDD_AddInputDev(CFG_INPUTDEV_NAME);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"usb_lvds"      //IIC�ӿڵĴ�����
//parent:"touch"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:�������               //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:later              //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"gkernel","rtc","gdd","lock","touch" //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"          //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������������������������none����ʾ�������������
                                //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
//%$#@end configue  ****�������ý���
//@#$%component end configure

static struct ST_TouchAdjust tg_touch_adjust;
//������������ �����ʾ���������������0��ʾ������ �����������������Ҫ���ⲿ�����������Ϊ0
__attribute__((weak))  const u8 lcd_lvds_type = 1;

extern s32 TOUCH_DirectRead(u8 *pButton, u32 *pX, u32 *pY);
extern s32 TOUCH_DeviceReady(void);
//----��ȡ����������-----------------------------------------------------------
//����: ��ȡusb_lvds�ɼ����Ĵ��������꣬����ж�㣬��ƽ��֮
//����: touch_data���ɼ���������
//����: 1=�����ʰ��£�0=����������
//-----------------------------------------------------------------------------
static ufast_t read_touch_usb_lvds(struct SingleTouchMsg *touch_data)
{
    u8 Button;u32 X; u32 Y;
    s32 tch_x,tch_y;
    if(TOUCH_DeviceReady() !=0)//USBδ����
        return 0;
    if(TOUCH_DirectRead(&Button,&X,&Y))
    {
        if(lcd_lvds_type)
        {
            tch_x = (s32)X;//���� ����
            tch_y = (s32)Y;
        }
        else
        {
            tch_x = (s32)Y;//������
            tch_y = (s32)X;
        }

        touch_data->z = (s32)Button;

        if(tg_touch_adjust.ratio_x != 0)
        {
            touch_data->x = ((tch_x << 16)- tg_touch_adjust.offset_x)
                                / tg_touch_adjust.ratio_x;
        }
        else
        {
            touch_data->x = tch_x;
        }
        if(tg_touch_adjust.ratio_y != 0)
        {
            touch_data->y = ((tch_y << 16)- tg_touch_adjust.offset_y)
                                / tg_touch_adjust.ratio_y;
        }
        else
        {
            touch_data->y = tch_y;
        }
        return (ufast_t)touch_data->z;
    }
    return 0;
}


//----������У׼---------------------------------------------------------------
//����: �������ĳߴ������Һ������ʾ������ȫһ�£���װҲ������ƫ���Ҫ����
//      У׼ϵ����ƫ������Ϊ��ø��߾��ȣ�ʹ�ö���С����
//����: display_name,����������Ӧ����ʾ����(��Դ��)//�þ�̬�����洢ÿ�ο���У׼һ��//
//����: ��
//-----------------------------------------------------------------------------
static void touch_ratio_adjust(struct GkWinObj *desktop)
{
    struct SingleTouchMsg touch_xyz0,touch_xyz1,touch_adjust;
    FILE *touch_init; u8 i;
    s32 limit_left,limit_top,limit_right,limit_bottom;

    printf("Wait Touch Device Ready...\n\r");
    while(1)
    {
        if(TOUCH_DeviceReady() ==0)//�ȴ�ö�����
            break;
        Djy_EventDelay(10*mS);
    }
    printf("Touch Device Ready...!!!! \n\r");
    limit_left = desktop->limit_left;
    limit_top = desktop->limit_top;
    limit_right = desktop->limit_right;
    limit_bottom = desktop->limit_bottom;

    GK_FillWin(desktop,CN_COLOR_WHITE,0);
    GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+50,
                   "����������", 21, CN_COLOR_BLACK, CN_R2_COPYPEN, 0);
    GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+70,
                   "��׼ȷ���ʮ�ֽ����", 21, CN_COLOR_BLACK, CN_R2_COPYPEN, 0);
    GK_Lineto(desktop,0,20,40,20,CN_COLOR_RED,CN_R2_COPYPEN,0);
    GK_Lineto(desktop,20,0,20,40,CN_COLOR_RED,CN_R2_COPYPEN,CN_TIMEOUT_FOREVER);
    GK_SyncShow(CN_TIMEOUT_FOREVER);

    while(!read_touch_usb_lvds(&touch_adjust));    //�ȴ�����
    Djy_EventDelay(20*mS);                            //������ʱ
    while(!read_touch_usb_lvds(&touch_xyz0));    //��¼��������һ��У��ֵ

    GK_FillWin(desktop,CN_COLOR_WHITE,0);

    GK_DrawText(desktop,NULL,NULL,limit_right-190,limit_bottom-80,
                   "����������", 21, CN_COLOR_BLACK, CN_R2_COPYPEN, 0);
    GK_DrawText(desktop,NULL,NULL,limit_right-190,limit_bottom-60,
                   "�ٴ�׼ȷ���ʮ�ֽ����", 21, CN_COLOR_BLACK, CN_R2_COPYPEN, 0);
    GK_Lineto(desktop,limit_right-40,limit_bottom-20,
                  limit_right,limit_bottom-20,
                  CN_COLOR_RED,CN_R2_COPYPEN,0);
    GK_Lineto(desktop,limit_right-20,limit_bottom-40,
                  limit_right-20,limit_bottom,
                  CN_COLOR_RED,CN_R2_COPYPEN,0);
    GK_SyncShow(CN_TIMEOUT_FOREVER);
    for(i=0;i<5;i++)
    {
        Djy_EventDelay(20*mS);
        if(read_touch_usb_lvds(&touch_adjust))//�ж�����
            i=0;
    }

    while(!read_touch_usb_lvds(&touch_adjust));    //�ȴ�����
    Djy_EventDelay(20*mS);                            //������ʱ
    while(!read_touch_usb_lvds(&touch_xyz1));    //��¼�������ڶ���У��ֵ

    GK_FillWin(desktop,CN_COLOR_WHITE,0);
    for(i=0;i<5;i++)
    {
        Djy_EventDelay(20*mS);
        if(read_touch_usb_lvds(&touch_adjust))//�ж�����
            i=0;
    }

    tg_touch_adjust.ratio_x = ((touch_xyz1.x - touch_xyz0.x)<<16)
                    /(limit_right - limit_left -40);
    tg_touch_adjust.offset_x = (touch_xyz0.x<<16) - 20*tg_touch_adjust.ratio_x;
    tg_touch_adjust.ratio_y = ((touch_xyz1.y - touch_xyz0.y)<<16)
                    /(limit_bottom- limit_top-40);
    tg_touch_adjust.offset_y= (touch_xyz0.y<<16) - 20*tg_touch_adjust.ratio_y;
    GK_FillWin(desktop,CN_COLOR_BLUE,0);
    GK_SyncShow(CN_TIMEOUT_FOREVER);

    touch_init = fopen("/yaffs2/touch_init.dat","w+");
    if(touch_init)
        fwrite(&tg_touch_adjust,sizeof(struct ST_TouchAdjust),1,touch_init);
    fclose(touch_init);

}

//-----------------------------------------------------------------------------
//����:��ʼ��������ģ��
//����:
//  desktop ��ʾ�����
//  touch_dev_name,����������Ӧ������
//����: true,�ɹ�;false,ʧ��
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_Touch_usb_lvds(struct GkWinObj *desktop,char *touch_dev_name)
{
    static struct SingleTouchPrivate usb_lvds;
    FILE *touch_init;

    if((touch_init = fopen("/yaffs2/touch_init.dat","r")) != NULL)
    {
        fread(&tg_touch_adjust,sizeof(struct ST_TouchAdjust),1,touch_init);
        fclose(touch_init);
    }
    else
    {
        touch_ratio_adjust(desktop);          //��ĻУ׼
    }
    usb_lvds.read_touch = read_touch_usb_lvds;//������������꺯��
    usb_lvds.touch_loc.display = NULL;        //NULL��ʾ��Ĭ������
    Touch_InstallDevice(touch_dev_name,&usb_lvds);//���������Touch

    return true;
}



