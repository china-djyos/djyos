//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
//����:  LiuWei.
//�汾��V1.0.0
//�ļ�����: �������뼯
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2014-11-10
//   ����:  LiuWei.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include <gui/gk_display.h>
#include    "gdd_private.h"
#include    <gui/gdd_timer.h>
#include "list.h"
#include "pool.h"
#include "component_config_gdd.h"

extern HWND    GDD_InitGddDesktop(struct GkWinObj *desktop);
extern bool_t GDD_CursorInit(void);

#define CN_HWND_INIT_CAPACITAL 10
struct MemCellPool *g_ptHwndPool;
static struct WINDOW s_tHwndInitPool[CN_HWND_INIT_CAPACITAL];
/*============================================================================*/

static  struct MutexLCB *gdd_mutex_lock=NULL;

bool_t    __GDD_Lock(void)
{
    Lock_MutexPend(gdd_mutex_lock, CN_TIMEOUT_FOREVER);
    return TRUE;
}

void    __GDD_Unlock(void)
{
    Lock_MutexPost(gdd_mutex_lock);
}

void    GDD_WindowInit(void);


/*============================================================================*/

ptu32_t GDD_TimerScan(void);
bool_t GDD_InputDevInit(void);

//----GDD������-----------------------------------------------------------------
//����: GDD����ִ�к���,�ú������᷵��.
//��������
//���أ���
//------------------------------------------------------------------------------
void ModuleInstall_GDD(struct GkWinObj *desktop)
{
    u16 evtt;
    HWND pGddWin=NULL;
    gdd_mutex_lock =Lock_MutexCreate(NULL);
    if(gdd_mutex_lock == NULL)
        return;

    g_ptHwndPool = Mb_CreatePool(s_tHwndInitPool,CN_HWND_INIT_CAPACITAL,
                    sizeof(struct WINDOW), 16, 1000, "hwnd");
    pGddWin=GDD_InitGddDesktop(desktop);
    GDD_WindowInit();
    GDD_TimerInit();
    GDD_InputDevInit( );
    GDD_SetFocusWindow(pGddWin);
    //gdd��ʱ��ɨ��
    evtt = DJY_EvttRegist(  EN_CORRELATIVE, CFG_GUI_RUN_PRIO, 0, 0,
                          GDD_TimerScan, NULL,1024,"gdd timer");
    if (evtt != CN_EVTT_ID_INVALID)
    {
        DJY_EventPop(evtt, NULL, 0, 0, 0, 0);
    }


//���´�����������Ӧ��gkernel���͵���Ϣ����ʱ���š�
//  evtt = DJY_EvttRegist(  EN_CORRELATIVE, CFG_GUI_RUN_PRIO, 0, 0,
//                        GDD_GetGK_Message, NULL,2048,"wait gk task");
//  if (evtt != CN_EVTT_ID_INVALID)
//  {
//      DJY_EventPop(evtt, NULL, 0, 0, 0, 0);
//  }

}

void ModuleInstall_Gdd_AND_Desktop(void)
{
    struct GkWinObj *desktop;
    if(CFG_DESKTOP_BUF == true)
        desktop = GK_CreateDesktop(CFG_DISPLAY_NAME,CFG_DESKTOP_NAME,
                                (s32)CFG_DESKTOP_WIDTH,(s32)CFG_DESKTOP_HEIGHT,
                                (u32)CFG_FILL_COLOR, (u32)CN_WINBUF_BUF,
                                (u16)CFG_DESKTOP_FORMAT,(u32)CFG_GRAY_BASE_COLOR);
    else
        desktop = GK_CreateDesktop(CFG_DISPLAY_NAME,CFG_DESKTOP_NAME,
                                (s32)CFG_DESKTOP_WIDTH,(s32)CFG_DESKTOP_HEIGHT,
                                (u32)CFG_FILL_COLOR, (u32)CN_WINBUF_NONE,
                                (u16)CFG_DESKTOP_FORMAT,(u32)CFG_GRAY_BASE_COLOR);
    if(desktop == NULL)
    {
        printf("�����������");
//      while( 1 );             //��ʼ��ʱ������ѭ������
    }
    else
    {
        ModuleInstall_GDD(desktop);
    }
}
/*============================================================================*/
