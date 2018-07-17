//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------

//����ģ��: GDD
//����:  zhb.
//�汾��V1.0.0
//�ļ�����: ��ť�ؼ�ʵ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2017-12-15
//   ����:  zhb.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#include <gdd.h>
#include <gdd_widget.h>
#include <gkernel.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "dbug.h"
#include "../../../../djysrc/bsp/arch/arm/arm32_stdint.h"

static bool_t Widget_SetFillColor(HWND hwnd,uint32_t color)
{
	SetWindowFillColor(hwnd,color);
	InvalidateWindow(hwnd,true);
	return true;
}

static bool_t Widget_GetFillColor(HWND hwnd,uint32_t *pcolor)
{
	return (GetWindowFillColor(hwnd,pcolor));
}

static bool_t Widget_SetHyalineColor(HWND hwnd,uint32_t color)
{
	SetWindowHyalineColor(hwnd,color);
	InvalidateWindow(hwnd,true);
	return true;
}

static bool_t Widget_GetHyalineColor(HWND hwnd,uint32_t *pcolor)
{

}


static bool_t Widget_SetTextColor(HWND hwnd,uint32_t color)
{
	SetWindowTextColor(hwnd,color);
	InvalidateWindow(hwnd,true);
	return true;
}

static bool_t Widget_GetTextColor(HWND hwnd,uint32_t *pcolor)
{
	return (GetWindowTextColor(hwnd,pcolor));
}

static bool_t Widget_SetRopCode(HWND hwnd,struct RopGroup *ptRopCode)
{
	struct RopGroup RopCode;
	memcpy(&RopCode,ptRopCode,sizeof(struct RopGroup));
	SetWindowRopCode(hwnd,RopCode);
	InvalidateWindow(hwnd,true);
	return true;
}

static bool_t Widget_EnableHyalineColor(HWND hwnd)
{
	struct RopGroup RopCode={0, 0, 0, CN_R2_COPYPEN, 0, 1, 0};
    return (Widget_SetRopCode(hwnd,&RopCode));
}



//----���ÿؼ�����-------------------------------------------------------------
//���ܣ����ÿؼ�����
//������pMsg����Ϣָ��
//���أ��̶�true
//---------------------------------------------------------------------------
bool_t Widget_SetAttr(HWND hwnd,uint8_t attrid,ptu32_t param)
{
	 if(hwnd==NULL)
	 {
	     debug_printf("gdd","Function:%s para invalid.\r\n",__FUNCTION__);
	    return false;
	 }
     switch(attrid)
     {
	   case ENUM_WIDGET_FILL_COLOR:
		   Widget_SetFillColor(hwnd,param);
		   break;
	   case ENUM_WIDGET_DRAW_COLOR:
//		   Button_SetDrawColor(hwnd,param);
		   break;
	   case ENUM_WIDGET_TEXT_COLOR:
		   Widget_SetTextColor(hwnd,param);
		   break;
	   case ENUM_WIDGET_HYALINE_COLOR:
		   Widget_SetHyalineColor(hwnd,param);
		   break;
	   case ENUM_WIDGET_ROP_CODE:
		   Widget_SetRopCode(hwnd,(struct RopGroup *)param);
		   break;
	   case ENUM_WIDGET_ENABLE_HYALINE:
		   Widget_EnableHyalineColor(hwnd);
		   break;
	   default:
		  break;

     }
     return true;
}

bool_t Widget_GetAttr(HWND hwnd,uint8_t attrid,ptu32_t *param)
{
	 if(hwnd==NULL)
	 {
	     debug_printf("gdd","Function:%s para invalid.\r\n",__FUNCTION__);
	     return false;
	 }
	 switch(attrid)
	 {
	   case ENUM_WIDGET_FILL_COLOR:
		   Widget_GetFillColor(hwnd,(u32)param);
		   break;
	   case ENUM_WIDGET_TEXT_COLOR:
		   Widget_GetTextColor(hwnd,(u32)param);
		   break;
//	   case ENUM_WIDGET_DRAW_COLOR:
////		   Button_SetDrawColor(hwnd,param);
//		   break;
//	   case ENUM_WIDGET_HYALINE_COLOR:
//		   Widget_SetHyalineColor(hwnd,param);
//		   break;
//	   case ENUM_WIDGET_ROP_CODE:
//		   Widget_SetRopCode(hwnd,(struct RopGroup *)param);
//		   break;
//	   case ENUM_WIDGET_ENABLE_HYALINE:
//		   Widget_EnableHyalineColor(hwnd);
//		   break;
	   default:
		  break;

	 }
	 return true;
}
