#if 0
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
// ģ������: DJY�˵�ģ��֮�û�Ӧ������
// ģ��汾: V1.00(��ʼ���汾)
// ������Ա: zqf
// ����ʱ��: 20121224
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================
#include "stdint.h"
#include "cpu_peri.h"
#include "gkernel.h"

#include "cpu_peri_lcd_touch_mouse.h"


#include "MenuDesigner.h"
#include "MenuitemApi.h"

#include "MenuData.h"
#include "MenuInitGeneral.h"
#include "MenuPeripheral_9cell.h"
#include "MenuPeripheral_pull.h"
#include "MenuInitGeneral.h"


//DISMODE��0��������1���Ź���
#define   DISMODE  1
void start_menu(void)
{
    struct InputDeviceMsg input_msg;
    s32 touch_x=0, touch_y=0;
    struct coordinate_point   mypoint={0,0};
    u8  keyvalue;
 //���Բ˵���
  s32 height,width;
  struct menu_item      *tempmenuitem;
  bool_t   result;
  struct GkWinObj *desktop;
  struct GkWinObj testwin;
  struct menu_item   *menutreenode;
  struct menu_displaypara  *operatingDispara;
  //������꽹��
//  HmiIn_SetFocusDefault(DJY_GetMyEvttId());     //todo:���ý���ĺ������ˡ�

  //��ʼ���Բ˵���
  printf("begin to test the menuitem tree!\n");
  module_init_menu(0);

  printf("Init ---end\n");
  //�����˵���
  menutreenode=create_menu_tree(NULL,g_tMenuitemModelTab);
  if(NULL==menutreenode)
  {
     printf("menuitem tree test end!----failed\n");
     return;
  }
  else
  {
      printf("menuitem tree test end!----success!\n");
   }

  printf("----This menu_display show time-------\n");
  //�������ȴ���
  desktop = GK_GetDesktop("sim_display");
  height=desktop->limit_bottom;
  width=desktop->limit_right;
  printf("Display:height=%d,width=%d\n",height,width);
  result=GK_CreateWin(desktop, &testwin, 0, 0, width, height,\
                             0x00202060, CN_WINBUF_BUF, "menu window", 0,0,0,0);

  GK_DrawText(&testwin,NULL,NULL,0,0,"Appwin",6,CN_COLOR_BLUE,0,0,CN_TIMEOUT_FOREVER);
  //��ʼ�������ǵ���ʾ����
  tempmenuitem=GetSonMenuitem(menutreenode);
  if(tempmenuitem==NULL)
  {
      printf("Failed to get the first menuitem!\n");
      return;
  }
  printf("---in main --1\n");
//DISMODE��0��������1���Ź���
  operatingDispara=menuappinit_general(tempmenuitem,&testwin,DISMODE);
  printf("---in main --2\n");


    for ( ; ; )
    {
        //printf("---in main --3\n");
//        input_msg = (struct InputDeviceMsg *)DJY_GetEventPara(NULL,NULL);
        if(HmiIn_ReadDefaultMsg(&input_msg,20*mS))
        {
            //�鿴�Ƿ񵥵㴥����
            if(input_msg->input_type == EN_HMIIN_SINGLE_TOUCH)
            {
                //���ԷŴ���
                touch_x = input_msg->input_data.SingleTouchMsg.x/CN_LCD_X_SCALE;
                touch_y = input_msg->input_data.SingleTouchMsg.y/CN_LCD_Y_SCALE;

                mypoint.x=touch_x;
                mypoint.y=touch_y;

                operatingDispara = menumousetouch_respond(operatingDispara,&mypoint);
            }
            else if(input_msg->input_type == EN_HMIIN_KEYBOARD)//�鿴�Ƿ����
            {
                keyvalue= input_msg->input_data.key_board.key_value[0];
                //�����ɿ���ȷ�ϣ�������ж��Ƿ����,���Ƕ��룬��ʾ��������
                if(input_msg->input_data.key_board.key_value[1] != CN_BREAK_CODE)
                {
                   //������Ӧ����
                    printf("key ---%d\n",keyvalue);
                   operatingDispara=menukeyboard_respond(operatingDispara,&keyvalue);//���menu�Լ��̵���
                }
            }
//          Djy_ParaUsed(0);
        }
//      DJY_WaitEvttPop(DJY_GetMyEvttId(),NULL,20*mS);
    }
}

#endif

