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
// Copyright (c) 2014 ����Ȩ�����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
// MenuInit_9cell.c
// ģ������: �˵���ʾģ��֮��ʼ���˵���ʾ����
// ģ��汾:
// ����ʱ��: 2012-12-31
// ����    : Administrator
//           �˵���ʾ�����ĳ�ʼ����ָ���Ƕ�����ʾģʽ��������Ӧ�����ĳ�ʼ����
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================
#include "MenuInit_9cell.h"
#include "MenuDisplay_9cell.h" //��ʾ����
#include "MenuSpace_9cell.h"    //�ռ���㼰����ʾ�����
#include "MenuPeripheral_9cell.h" //����
#include "MenuPeripheralGeneral.h"
// =========================================================================
// ��������:���˵����ʼ��Ϊ�Ź�����ʾģʽ
// �������:menuitem��Ӧ�ó���Ҫ��ʾ�ĵ�һ���˵��win��Ӧ�ó����ṩ��window
// ���������
// ����ֵ  :�Ź���ģʽ�ĳ�ʼ��ʾ����
// =========================================================================
struct menu_displaypara  *module_menu_init_9cell(struct menu_item *menuitem, struct GkWinRsc *win)
{
  struct menu_displaypara  *dispara=NULL;
   //�������
  if((NULL == menuitem)||(NULL == win))
  {
    printf("Invalidpara --module_menu_init_9cell!\n");
    return dispara;
  }

  dispara = createmenudisparabypre_9cell(NULL, menuitem, win);

  if(NULL == dispara)
  {
    printf("failed---module_menu_init_9cell\n");
    return dispara;
  }
  else
  {
     dispara->menukeyboard_action = keyboardaction_9cell;
     dispara->menumousetouch_ction = menutouchlcd_9cell;
     dispara->MyPeripheralGeneralAction=peripheralrespond_general;

     dispara->MyMenuDispalyFunction = menu_display_9cell;
     dispara->AdjustVisualMenuitem=adjustvisualmenuitem_9cell;
     dispara->CreateMenuDisparaBypre=createmenudisparabypre_9cell;
     dispara->MyMenuInitFunction=module_menu_init_9cell;
  }


  //�������Ӳ˵���
  dispara->AdjustVisualMenuitem(dispara);
  //��ʾ�˵�
  dispara->MyMenuDispalyFunction(dispara);

  return dispara;
}